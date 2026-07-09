#ifndef _INC_VOICEOVER
#define _INC_VOICEOVER

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file voiceover.h
 * @brief NPC dialog-to-audio voiceover mapping system.
 *
 * This module provides a JSON-driven mapping layer that associates NPC port numbers
 * and dialog text prefixes with voiceover audio files (.ogg). The mapping enables
 * automatic audio playback when the client receives NPC messages that match configured
 * text prefixes. Relative audio file paths are resolved relative to the loaded
 * mapping file. WAV uses the existing DirectSound path; OGG/MP3 use WinMM/MCI.
 *
 * Design:
 * - JSON asset file (`assets/audio_map.json`) defines NPC port + text prefix → audio file mappings
 * - Loaded once at startup; lookup is O(1) per port, O(n) scan within lines (small n)
 * - On-demand audio loading via existing `soundload()` / `soundplay2()` DirectSound API
 * - Graceful fallback on JSON parse failure or missing audio file (silent, logged)
 */

/**
 * Load the NPC voiceover mapping from JSON asset file.
 *
 * @param json_path Path to audio_map.json (e.g., "audio_map.json" or absolute path)
 * @return true if load successful; false if parse error, file not found, or allocation failure.
 *         On failure, voiceovers are disabled (all lookups return NULL), and error is logged.
 *
 * Call once at startup (e.g., during soundsetup() or client game init).
 */
bool voiceover_load_map(const char* json_path);

/**
 * Look up a voiceover audio filename by NPC port and dialog text.
 *
 * @param npc_port NPC port number (e.g., 202 for Chuckles)
 * @param text Dialog text (raw from NPC message; matched against configured prefixes)
 * @return Pointer to audio filename (e.g., "Chuckles.0.ogg") if a match found;
 *         NULL if no mapping exists for this port+text pair.
 *         Caller must pass result to soundload() to play.
 *
 * Thread-safe if called only from main frame loop (no concurrent mutations).
 * Lookup is O(1) port hash + O(n) prefix scan; n is small (typically 1-5 lines per NPC).
 */
const char* voiceover_lookup_by_port_and_prefix(int npc_port, const char* text);

/**
 * Play a mapped voiceover, if any, for the given NPC message.
 *
 * @param npc_port NPC port number from the incoming speech message.
 * @param text NPC speech text from the incoming message.
 * @param volume Current client master volume in the legacy 0..255 range.
 */
void voiceover_play_for_message(int npc_port, const char* text, unsigned char volume);

/**
 * Shutdown and free all voiceover map resources.
 *
 * Call once at client shutdown (or before reload/reconfig).
 * Safe to call multiple times.
 */
void voiceover_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* _INC_VOICEOVER */

