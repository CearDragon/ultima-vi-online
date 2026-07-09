### Plan for Implementing MP3 Support in Ultima VI Online

This plan outlines the modernization of the audio system to support high-quality MP3 playback alongside the legacy MIDI implementation, allowing players to toggle between them via the in-game options menu.

---

### 1. Data Structures & Settings Persistence
*   **Extend `client_settings`**: In `src\client\data_client.h`, append `unsigned char music_format` to the `client_settings` struct. 
    *   `0` = MIDI (Default)
    *   `1` = MP3
*   **Update `settings.bin` handling**: In `src\client\setup_client.inc`, ensure the new field is read from the binary settings file. Since it is appended to the end, it will maintain backward compatibility with older `settings.bin` files (they will simply leave the value at its pre-initialized default).
*   **Add Options Menu Entry**: In `src\client\function_client.cpp`, add a new entry to `g_menuSettings` under the `Audio` category:
    ```cpp
    {"Audio", "Music format", MS_CHOICE, "MUSICFORMAT", 0, 0, 2, {"MIDI", "MP3"}, {0}},
    ```

### 2. Music Player Abstraction (Refactoring)
To support multiple backends, the current `u6omidi` global (a raw `CMidiMusic` pointer) must be abstracted:
*   **Define `IMusicPlayer` Interface**: Create `src\client\music_player.h` with virtual methods: `Initialize()`, `LoadFromFile(path)`, `Play()`, `Stop()`, `IsPlaying()`, and `SetVolume(0-255)`.
*   **Polymorphic Global**: Rename `u6omidi` to `u6omusic` in `src\common\globals.inc` and change its type to `IMusicPlayer*`.
*   **Refactor `CMidiMusic`**: Update `src\client\dmusic.cpp` so `CMidiMusic` inherits from `IMusicPlayer`. This wraps the existing DirectMusic logic.

### 3. MP3 Backend Implementation
*   **New Class `CMp3Music`**: Implement in `src\client\mp3_player.cpp` using the `IMusicPlayer` interface.
*   **Decoder**: Integrate a lightweight, public-domain decoder like **minimp3** (single header) to avoid adding external DLL dependencies.
*   **Playback**: Use `IDirectSoundBuffer` (part of the existing `dsnd` setup) for output.
    *   **Simple Path**: Decode the entire MP3 to a PCM memory buffer on `LoadFromFile`. Most *Ultima VI* tracks are short (1-3 minutes), making this memory-efficient enough for 32-bit targets.
    *   **Volume Mapping**: Map the 0-255 game volume to DirectSound’s logarithmic decibel scale (`SetVolume`).

### 4. Dynamic Path & Resource Logic
*   **Folder Structure**: Maintain two parallel folders in the client assets:
    *   `assets\game_files\client\midi\` (Existing `.mid` files)
    *   `assets\game_files\client\mp3\` (New `.mp3` files)
*   **Loading Logic**: Update the music loading block in `src\client\loop\loop_client_part_refresh_tail.cpp` (around line 175). Construct the file path dynamically based on the `music_format` setting:
    ```cpp
    if (music_format == 1) {
        txtset(t4, ".\\mp3\\"); txtadd(t4, filename_base); txtadd(t4, ".mp3");
    } else {
        txtset(t4, ".\\midi\\"); txtadd(t4, filename_base); txtadd(t4, ".mid");
    }
    ```
*   **Runtime Engine Swap**: In `HandleOptionsCommand` (`src\client\function_client.cpp`), add logic to swap the active engine when the format is changed. This stops the current track, deletes the old player instance, creates the new one (MIDI or MP3), and triggers a reload of the current music ID.

### 5. Implementation Steps for Developers
1.  **Add `minimp3.h`** to `src\client\`.
2.  **Define the `IMusicPlayer` interface** and update `CMidiMusic` to match.
3.  **Implement `CMp3Music`** using `DirectSound` for output.
4.  **Update the Options Menu** data and the `HandleOptionsCommand` logic.
5.  **Modify the loop fragments** in `loop_client_part_refresh_tail.cpp` to use the polymorphic `u6omusic` pointer.
6.  **Verify**: Ensure MIDI still works as intended, then test MP3 by placing a `.mp3` file (e.g., `U6-THEME.mp3`) in the `mp3` folder and toggling the setting.

### Compatibility Note
This change is **client-side only**. It does not modify the wire protocol (`U6O_VERSION` remains unchanged) as the host continues to send numeric music IDs that the client maps to local files via `midiinfo.txt`.