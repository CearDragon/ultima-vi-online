#include "stdafx.h"
#include "voiceover.h"
#include "function_client.h"
#include "sound.h"

#include <mmsystem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace {
constexpr int kVoiceoverMaxNpcs = 256;
constexpr int kVoiceoverMaxLinesPerNpc = 16;
constexpr int kVoiceoverMaxPrefixLen = 96;
constexpr int kVoiceoverMaxFilenameLen = MAX_PATH;
constexpr const char* kVoiceoverAlias = "u6ovoice";

struct VoiceoverLine {
    char text_prefix[kVoiceoverMaxPrefixLen];
    char audio_file[kVoiceoverMaxFilenameLen];
    char resolved_audio_path[MAX_PATH];
    char decoded_wav_path[MAX_PATH];
    sound* cached_sound;
    bool audio_resolved;
    bool pcm_load_attempted;
};

struct VoiceoverNpc {
    int port;
    int line_count;
    VoiceoverLine lines[kVoiceoverMaxLinesPerNpc];
};

VoiceoverNpc g_voiceover_map[kVoiceoverMaxNpcs];
int g_voiceover_npc_count = 0;
bool g_voiceover_loaded = false;
bool g_voiceover_mci_open = false;
char g_voiceover_map_path[MAX_PATH] = "";
char g_voiceover_map_dir[MAX_PATH] = "";

void logVoiceoverMessage(const char* message) {
    scrlog(message);
}

void logVoiceoverFormat(const char* format, int value1, int value2 = -1) {
    char buffer[256];
    if (value2 >= 0) {
        sprintf_s(buffer, sizeof(buffer), format, value1, value2);
    } else {
        sprintf_s(buffer, sizeof(buffer), format, value1);
    }
    scrlog(buffer);
}

char* trimLeft(char* text) {
    while (*text && (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')) {
        text++;
    }
    return text;
}

void trimRightInPlace(char* text) {
    size_t length = strlen(text);
    while (length > 0) {
        const char ch = text[length - 1];
        if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n' && ch != ',') {
            break;
        }
        text[length - 1] = '\0';
        length--;
    }
}

bool startsWith(const char* text, const char* prefix) {
    return strncmp(text, prefix, strlen(prefix)) == 0;
}

bool containsCaseInsensitive(const char* text, const char* needle) {
    if (!text || !needle || !needle[0]) return false;
    const size_t needle_len = strlen(needle);
    const size_t text_len = strlen(text);
    if (needle_len > text_len) return false;

    for (size_t i = 0; i + needle_len <= text_len; i++) {
        if (_strnicmp(text + i, needle, needle_len) == 0) {
            return true;
        }
    }
    return false;
}

bool fileExists(const char* path) {
    const DWORD attributes = GetFileAttributesA(path);
    return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool pathIsAbsolute(const char* path) {
    if (!path || !path[0]) return false;
    if ((strlen(path) >= 2) && path[1] == ':') return true;
    return path[0] == '\\' || path[0] == '/';
}

bool hasSlash(const char* path) {
    return strchr(path, '\\') != NULL || strchr(path, '/') != NULL;
}

const char* fileExtension(const char* path) {
    const char* dot = strrchr(path, '.');
    return dot ? dot : "";
}

void extractDirectory(const char* path, char* out_dir, size_t out_dir_size) {
    out_dir[0] = '\0';
    if (!path || !path[0]) return;

    const char* last_backslash = strrchr(path, '\\');
    const char* last_forwardslash = strrchr(path, '/');
    const char* last_separator = last_backslash;
    if (last_forwardslash && (!last_separator || last_forwardslash > last_separator)) {
        last_separator = last_forwardslash;
    }
    if (!last_separator) return;

    size_t copy_length = static_cast<size_t>(last_separator - path);
    if (copy_length >= out_dir_size) copy_length = out_dir_size - 1;
    memcpy(out_dir, path, copy_length);
    out_dir[copy_length] = '\0';
}

void joinPath(char* out_path, size_t out_path_size, const char* directory, const char* leaf) {
    if (!directory || !directory[0]) {
        sprintf_s(out_path, out_path_size, "%s", leaf);
        return;
    }

    const char separator = (directory[strlen(directory) - 1] == '\\' || directory[strlen(directory) - 1] == '/') ? '\0' : '\\';
    if (separator) {
        sprintf_s(out_path, out_path_size, "%s\\%s", directory, leaf);
    } else {
        sprintf_s(out_path, out_path_size, "%s%s", directory, leaf);
    }
}

bool extractQuotedValue(const char* src, char* dst, size_t dst_size) {
    const char* colon = strchr(src, ':');
    if (!colon) return false;

    const char* opening_quote = strchr(colon, '"');
    if (!opening_quote) return false;
    opening_quote++;

    const char* closing_quote = strchr(opening_quote, '"');
    if (!closing_quote) return false;

    size_t copy_length = static_cast<size_t>(closing_quote - opening_quote);
    if (copy_length >= dst_size) copy_length = dst_size - 1;
    memcpy(dst, opening_quote, copy_length);
    dst[copy_length] = '\0';
    return true;
}

bool extractIntegerValue(const char* src, int* value) {
    const char* colon = strchr(src, ':');
    if (!colon) return false;
    *value = atoi(colon + 1);
    return true;
}

bool resolveExistingPath(const char* requested_path, char* resolved_path, size_t resolved_path_size) {
    const char* candidates[] = {
        requested_path,
        ".\\audio_map.json",
        ".\\assets\\game_files\\client\\audio_map.json",
        ".\\assets\\audio_map.json",
        ".\\..\\..\\..\\assets\\game_files\\client\\audio_map.json",
        ".\\..\\..\\..\\assets\\audio_map.json"
    };

    for (const char* candidate : candidates) {
        if (!candidate || !candidate[0]) continue;
        if (!fileExists(candidate)) continue;
        sprintf_s(resolved_path, resolved_path_size, "%s", candidate);
        return true;
    }

    return false;
}

bool resolveAudioPath(VoiceoverLine* line) {
    if (line->audio_resolved) {
        return line->resolved_audio_path[0] != '\0';
    }

    line->audio_resolved = true;
    line->resolved_audio_path[0] = '\0';

    if (!line->audio_file[0]) return false;

    if (pathIsAbsolute(line->audio_file) && fileExists(line->audio_file)) {
        sprintf_s(line->resolved_audio_path, sizeof(line->resolved_audio_path), "%s", line->audio_file);
        return true;
    }

    char candidate[MAX_PATH];
    if (g_voiceover_map_dir[0]) {
        joinPath(candidate, sizeof(candidate), g_voiceover_map_dir, line->audio_file);
        if (fileExists(candidate)) {
            sprintf_s(line->resolved_audio_path, sizeof(line->resolved_audio_path), "%s", candidate);
            return true;
        }
    }

    if (hasSlash(line->audio_file) && fileExists(line->audio_file)) {
        sprintf_s(line->resolved_audio_path, sizeof(line->resolved_audio_path), "%s", line->audio_file);
        return true;
    }

    const char* extension = fileExtension(line->audio_file);
    const char* subdirectory = ".\\voice\\";
    if (_stricmp(extension, ".ogg") == 0) subdirectory = ".\\ogg\\";
    else if (_stricmp(extension, ".wav") == 0) subdirectory = ".\\wav\\";
    else if (_stricmp(extension, ".mp3") == 0) subdirectory = ".\\mp3\\";

    sprintf_s(candidate, sizeof(candidate), "%s%s", subdirectory, line->audio_file);
    if (fileExists(candidate)) {
        sprintf_s(line->resolved_audio_path, sizeof(line->resolved_audio_path), "%s", candidate);
        return true;
    }

    if (g_voiceover_map_dir[0]) {
        char bundle_dir[MAX_PATH];
        joinPath(bundle_dir, sizeof(bundle_dir), g_voiceover_map_dir, subdirectory + 2);
        joinPath(candidate, sizeof(candidate), bundle_dir, line->audio_file);
        if (fileExists(candidate)) {
            sprintf_s(line->resolved_audio_path, sizeof(line->resolved_audio_path), "%s", candidate);
            return true;
        }
    }

    return false;
}

void closeMciVoiceover() {
    char command[64];
    sprintf_s(command, sizeof(command), "close %s", kVoiceoverAlias);
    mciSendStringA(command, NULL, 0, NULL);
    g_voiceover_mci_open = false;
}

void logMciError(const char* stage, MCIERROR code, const char* command) {
    char error_text[256];
    if (!mciGetErrorStringA(code, error_text, (UINT) sizeof(error_text))) {
        sprintf_s(error_text, sizeof(error_text), "Unknown MCI error");
    }

    char log_line[768];
    sprintf_s(log_line,
              sizeof(log_line),
              "[VO] %s failed (code=%lu): %s | cmd=%s",
              stage,
              (unsigned long) code,
              error_text,
              command ? command : "<none>");
    logVoiceoverMessage(log_line);
}

bool tryMciOpen(const char* path, const char* open_template, const char* stage) {
    char command[1024];
    sprintf_s(command, sizeof(command), open_template, path, kVoiceoverAlias);
    MCIERROR error = mciSendStringA(command, NULL, 0, NULL);
    if (error != 0) {
        logMciError(stage, error, command);
        return false;
    }
    return true;
}

bool playStreamingVoiceover(const char* path, unsigned char volume) {
    closeMciVoiceover();

    char full_path[MAX_PATH];
    DWORD full_path_len = GetFullPathNameA(path, MAX_PATH, full_path, NULL);
    const char* open_path = path;
    if (full_path_len > 0 && full_path_len < MAX_PATH) {
        open_path = full_path;
    }

    // Try a few MCI open forms because OGG support depends on installed
    // filters/codecs and some systems only accept one command variant.
    if (!tryMciOpen(open_path, "open \"%s\" type mpegvideo alias %s", "MCI open (mpegvideo)") &&
        !tryMciOpen(open_path, "open \"%s\" alias %s", "MCI open (auto)") &&
        !tryMciOpen(open_path, "open \"%s\" type MPEGVideo alias %s", "MCI open (MPEGVideo)")) {
        return false;
    }

    g_voiceover_mci_open = true;
    char command[1024];
    const unsigned int mci_volume = (static_cast<unsigned int>(volume) * 1000U) / 255U;
    sprintf_s(command, sizeof(command), "setaudio %s volume to %u", kVoiceoverAlias, mci_volume);
    MCIERROR error = mciSendStringA(command, NULL, 0, NULL);
    if (error != 0) {
        logMciError("MCI setaudio", error, command);
    }

    sprintf_s(command, sizeof(command), "play %s", kVoiceoverAlias);
    error = mciSendStringA(command, NULL, 0, NULL);
    if (error != 0) {
        logMciError("MCI play", error, command);
        return false;
    }

    return true;
}

bool runCommandHidden(const char* command_line, unsigned long* exit_code_out = NULL) {
    if (!command_line || !command_line[0]) return false;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    char command_buffer[2048];
    sprintf_s(command_buffer, sizeof(command_buffer), "%s", command_line);

    if (!CreateProcessA(NULL,
                        command_buffer,
                        NULL,
                        NULL,
                        FALSE,
                        CREATE_NO_WINDOW,
                        NULL,
                        NULL,
                        &si,
                        &pi)) {
        if (exit_code_out) *exit_code_out = GetLastError();
        return false;
    }

    WaitForSingleObject(pi.hProcess, 20000);
    DWORD exit_code = 1;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    if (exit_code_out) *exit_code_out = exit_code;
    return exit_code == 0;
}

unsigned long hashPathForCache(const char* text) {
    unsigned long h = 2166136261u;
    if (!text) return h;
    while (*text) {
        h ^= (unsigned char) *text;
        h *= 16777619u;
        text++;
    }
    return h;
}

bool decodeOggToWavExternal(const char* input_path, const char* output_path) {
    if (!input_path || !output_path) return false;

    deletefile((char*) output_path);

    char command[2048];
    unsigned long status_code = 0;

    char input_dir[MAX_PATH];
    extractDirectory(input_path, input_dir, sizeof(input_dir));
    char input_dir_oggdec[MAX_PATH];
    input_dir_oggdec[0] = '\0';
    if (input_dir[0]) {
        joinPath(input_dir_oggdec, sizeof(input_dir_oggdec), input_dir, "oggdec.exe");
    }

    // 1) Dedicated local decoder if bundled by the runtime asset pack.
    sprintf_s(command, sizeof(command), "\".\\ogg\\oggdec.exe\" -q -w \"%s\" \"%s\"", output_path, input_path);
    if (runCommandHidden(command, &status_code) && fileExists(output_path)) return true;

    sprintf_s(command, sizeof(command), "\".\\ogg\\oggdec.exe\" -q -w \"%s\" \"%s\"", output_path, input_path);
    if (runCommandHidden(command, &status_code) && fileExists(output_path)) return true;

    if (input_dir_oggdec[0]) {
        sprintf_s(command, sizeof(command), "\"%s\" -q -w \"%s\" \"%s\"", input_dir_oggdec, output_path, input_path);
        if (runCommandHidden(command, &status_code) && fileExists(output_path)) return true;
    }

    sprintf_s(command, sizeof(command), "\".\\oggdec.exe\" -q -w \"%s\" \"%s\"", output_path, input_path);
    if (runCommandHidden(command, &status_code) && fileExists(output_path)) return true;

    // 2) Fallback: ffmpeg if available on PATH.
    sprintf_s(command,
              sizeof(command),
              "ffmpeg -y -loglevel error -i \"%s\" \"%s\"",
              input_path,
              output_path);
    if (runCommandHidden(command, &status_code) && fileExists(output_path)) return true;

    static char decode_log[512];
    sprintf_s(decode_log,
              sizeof(decode_log),
              "[VO] OGG decode command attempts failed (last status=%lu). Looked for oggdec in .\\ogg, .\\ogg, input dir, and cwd.",
              status_code);
    logVoiceoverMessage(decode_log);

    return false;
}

bool playWaveVoiceover(VoiceoverLine* line, unsigned char volume) {
    if (!resolveAudioPath(line)) return false;

    if (!line->pcm_load_attempted) {
        line->pcm_load_attempted = true;
        line->cached_sound = soundload(line->resolved_audio_path);
    }

    if (!line->cached_sound) return false;

    return soundplay2(line->cached_sound, 255, volume) != NULL;
}

bool playOggVoiceover(VoiceoverLine* line, unsigned char volume) {
    if (!resolveAudioPath(line)) return false;

    if (!line->pcm_load_attempted) {
        line->pcm_load_attempted = true;
        if (!line->decoded_wav_path[0]) {
            unsigned long cache_id = hashPathForCache(line->resolved_audio_path);
            sprintf_s(line->decoded_wav_path,
                      sizeof(line->decoded_wav_path),
                      ".\\voice\\vo_cache_%08lx.wav",
                      cache_id);
        }
        if (!decodeOggToWavExternal(line->resolved_audio_path, line->decoded_wav_path)) {
            logVoiceoverMessage("[VO] OGG decode failed (oggdec/ffmpeg unavailable or decode error)");
            return false;
        }
        line->cached_sound = soundload(line->decoded_wav_path);
        if (!line->cached_sound) {
            logVoiceoverMessage("[VO] Failed to load decoded OGG WAV cache file");
            return false;
        }
    }

    if (!line->cached_sound) return false;
    return soundplay2(line->cached_sound, 255, volume) != NULL;
}

VoiceoverLine* findVoiceoverLine(int npc_port, const char* text) {
    if (!g_voiceover_loaded || !text) return NULL;

    for (int npc_index = 0; npc_index < g_voiceover_npc_count; npc_index++) {
        if (g_voiceover_map[npc_index].port != npc_port) continue;

        for (int line_index = 0; line_index < g_voiceover_map[npc_index].line_count; line_index++) {
            VoiceoverLine* line = &g_voiceover_map[npc_index].lines[line_index];
            const size_t prefix_length = strlen(line->text_prefix);
            if (prefix_length && containsCaseInsensitive(text, line->text_prefix)) {
                return line;
            }
        }
        return NULL;
    }

    return NULL;
}
}

void voiceover_shutdown(void) {
    closeMciVoiceover();

    for (int npc_index = 0; npc_index < g_voiceover_npc_count; npc_index++) {
        for (int line_index = 0; line_index < g_voiceover_map[npc_index].line_count; line_index++) {
            VoiceoverLine* line = &g_voiceover_map[npc_index].lines[line_index];
            if (line->cached_sound) {
                free(line->cached_sound);
                line->cached_sound = NULL;
            }
        }
    }

    memset(g_voiceover_map, 0, sizeof(g_voiceover_map));
    g_voiceover_npc_count = 0;
    g_voiceover_loaded = false;
    g_voiceover_map_path[0] = '\0';
    g_voiceover_map_dir[0] = '\0';
}

bool voiceover_load_map(const char* json_path) {
    voiceover_shutdown();

    if (!resolveExistingPath(json_path, g_voiceover_map_path, sizeof(g_voiceover_map_path))) {
        logVoiceoverMessage("[VO] audio_map.json not found; voiceovers disabled");
        return FALSE;
    }

    extractDirectory(g_voiceover_map_path, g_voiceover_map_dir, sizeof(g_voiceover_map_dir));

    FILE* map_file = fopen(g_voiceover_map_path, "rb");
    if (!map_file) {
        logVoiceoverMessage("[VO] Failed to open audio_map.json");
        return FALSE;
    }

    fseek(map_file, 0, SEEK_END);
    const long file_size = ftell(map_file);
    fseek(map_file, 0, SEEK_SET);

    char* buffer = (char*) malloc(static_cast<size_t>(file_size) + 1U);
    if (!buffer) {
        fclose(map_file);
        logVoiceoverMessage("[VO] Failed to allocate audio map buffer");
        return FALSE;
    }

    if (fread(buffer, 1, static_cast<size_t>(file_size), map_file) != static_cast<size_t>(file_size)) {
        fclose(map_file);
        free(buffer);
        logVoiceoverMessage("[VO] Failed to read audio_map.json");
        return FALSE;
    }
    fclose(map_file);
    buffer[file_size] = '\0';

    VoiceoverNpc* current_npc = NULL;
    char pending_prefix[kVoiceoverMaxPrefixLen] = "";
    bool found_mapping_header = false;

    for (char* line = strtok(buffer, "\n"); line != NULL; line = strtok(NULL, "\n")) {
        char* trimmed = trimLeft(line);
        trimRightInPlace(trimmed);

        if (startsWith(trimmed, "\"npc_port_mappings\"")) {
            found_mapping_header = true;
            continue;
        }

        if (startsWith(trimmed, "\"npc_port\"")) {
            if (g_voiceover_npc_count >= kVoiceoverMaxNpcs) break;

            current_npc = &g_voiceover_map[g_voiceover_npc_count];
            memset(current_npc, 0, sizeof(VoiceoverNpc));
            if (extractIntegerValue(trimmed, &current_npc->port)) {
                g_voiceover_npc_count++;
            } else {
                current_npc = NULL;
            }
            pending_prefix[0] = '\0';
            continue;
        }

        if (startsWith(trimmed, "\"text_prefix\"")) {
            extractQuotedValue(trimmed, pending_prefix, sizeof(pending_prefix));
            continue;
        }

        if (startsWith(trimmed, "\"audio_file\"") && current_npc && pending_prefix[0]) {
            if (current_npc->line_count < kVoiceoverMaxLinesPerNpc) {
                VoiceoverLine* voiceover_line = &current_npc->lines[current_npc->line_count];
                if (extractQuotedValue(trimmed, voiceover_line->audio_file, sizeof(voiceover_line->audio_file))) {
                    sprintf_s(voiceover_line->text_prefix, sizeof(voiceover_line->text_prefix), "%s", pending_prefix);
                    current_npc->line_count++;
                }
            }
            pending_prefix[0] = '\0';
        }
    }

    free(buffer);

    if (!found_mapping_header || g_voiceover_npc_count == 0) {
        voiceover_shutdown();
        logVoiceoverMessage("[VO] audio_map.json parsed with no NPC mappings");
        return FALSE;
    }

    g_voiceover_loaded = true;
    for (int npc_index = 0; npc_index < g_voiceover_npc_count; npc_index++) {
        logVoiceoverFormat("[VO] Loaded NPC port %d with %d line(s)",
                           g_voiceover_map[npc_index].port,
                           g_voiceover_map[npc_index].line_count);
    }
    logVoiceoverFormat("[VO] Loaded voiceover map: %d NPC(s)", g_voiceover_npc_count);
    return TRUE;
}

const char* voiceover_lookup_by_port_and_prefix(int npc_port, const char* text) {
    VoiceoverLine* line = findVoiceoverLine(npc_port, text);
    return line ? line->audio_file : NULL;
}

void voiceover_play_for_message(int npc_port, const char* text, unsigned char volume) {
    VoiceoverLine* line = findVoiceoverLine(npc_port, text);
    if (!line) return;
    if (!resolveAudioPath(line)) {
        logVoiceoverMessage("[VO] Audio file for mapped line was not found");
        return;
    }

    const char* extension = fileExtension(line->resolved_audio_path);
    if (_stricmp(extension, ".wav") == 0) {
        playWaveVoiceover(line, volume);
        return;
    }
    if (_stricmp(extension, ".ogg") == 0) {
        if (!playOggVoiceover(line, volume)) {
            logVoiceoverMessage("[VO] Failed to decode/play OGG voiceover file");
        }
        return;
    }

    if (!playStreamingVoiceover(line->resolved_audio_path, volume)) {
        logVoiceoverMessage("[VO] Failed to play streaming voiceover file");
    }
}







