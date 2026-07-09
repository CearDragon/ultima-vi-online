# Plan: NPC Dialog-to-Audio Mapping System

**TL;DR:** Create a JSON asset file mapping NPC port + text prefix to voiceover `.ogg` files. Hook the mapping lookup into the client's network message decoder (message type 3 / NPC text) at [loop_client_part_net.cpp ~L105](file:///c%3A/Users/Owner/CLionProjects/ultima-vi-online/src/client/loop/loop_client_part_net.cpp). When a match fires, play the audio via `soundplay2()` from the existing DirectSound system.

## Implementation Strategy

### Mapping Strategy
- **Strategy**: Option A — NPC port + text prefix (human-readable, collision-resistant)
- **Mapping Table**: Asset file (`assets/audio_map.json`)
- **Text Prefix Length**: 6–12 characters (distinctive enough to avoid false positives, future-proof)

### Steps

#### Phase 1: Audio Mapping Asset

1. **Create `assets/audio_map.json`**
   - Format: 
     ```json
     {
       "npc_port_mappings": [
         {
           "npc_port": <chuckles_port_number>,
           "lines": [
             {
               "text_prefix": "Ho eyo he hum",
               "audio_file": "Chuckles.0.ogg"
             }
           ]
         }
       ]
     }
     ```
   - Lookup Chuckles' exact port from NPC binary data
   - Store text prefixes that are distinctive within Chuckles' dialog tree

2. **Document port discovery process**
   - Extract NPC.BIN to find Chuckles (index 8) port number
   - Verify against existing karma_npc_scan.py or manual inspection

#### Phase 2: Client-Side Voiceover Infrastructure

3. **Add voiceover data structure** in `src/client/voiceover.h` (new file):
   - `struct VoiceoverLine { int text_prefix_len; char text_prefix[32]; const char* audio_file; }`
   - `struct VoiceoverNPC { int port; VoiceoverLine* lines; int line_count; }`
   - Root lookup structure or map-like container

4. **Add voiceover loader** in `src/client/voiceover.cpp` (new file):
   - `bool load_voiceover_map(const char* json_path)` — parse `audio_map.json` into memory
   - `const char* lookup_voiceover_by_port_and_prefix(int port, const char* text)` — find match
   - Call from `soundsetup()` or early client init (before game loop starts)
   - Graceful fallback if JSON missing or parse fails (log to `scrlog()`, disable voiceovers)

5. **Add voiceover header** (`src/client/voiceover.h`):
   - Export `load_voiceover_map()` and `lookup_voiceover_by_port_and_prefix()`
   - Include guard, minimal dependencies

#### Phase 3: Integration with Message Decoder

6. **Inject hook in `loop_client_part_net.cpp` (~L105–115)**
   - Location: after `sfx[i3].p = dbgt5;` (text assignment)
   - Logic:
     ```cpp
     // NEW: Check for voiceover match
     if (sfx[i3].more >= 194) {  // NPC port range (194+ is NPC, <194 is player)
         const char* audio_file = lookup_voiceover_by_port_and_prefix(sfx[i3].more, dbgt5->d2);
         if (audio_file) {
             sound* vo = soundload(audio_file);
             if (vo) {
                 soundplay2(vo, u6osound_volume[<default_voiceover_slot>]);
             }
         }
     }
     ```
   - Use `replace_string_in_file` (tight context) to avoid corrupting brace seams
   - **Do NOT use `insert_edit_into_file`** on this file (brace-seam fragment)

7. **Define voiceover volume slot** in `src/client/data_client.cpp`:
   - Add `u6osound_volume[<idx>]` entry for voiceovers (or reuse existing SFX volume)
   - Document in comments

#### Phase 4: Startup Integration

8. **Call voiceover loader during client init**
   - Add to `soundsetup()` (after DirectSound init) or a dedicated voiceover init phase
   - Location: `src/client/sound.cpp` or early game-open sequence
   - Ensure JSON is loaded before first game frame

#### Phase 5: Testing

9. **Verify integration**
   - Build `client` and `both` targets
   - Trigger Chuckles conversation
   - Confirm:
     - Audio file loads without error
     - `.ogg` plays when greeting line appears
     - Audio timing syncs with text display (no stuttering, correct volume)
     - No double-plays or missed plays on repeated lines

## Further Considerations

### Audio Loading Strategy
- **Pre-cache vs. On-demand**?
  - **On-demand + async** (recommended): Load audio on first use, background thread or quiet frame
  - Faster startup, minimal memory footprint, requires async coordination
  - Use existing thread pool if available (check `sound.cpp` for threading model)

### Error Handling
- JSON parse fails → log to `scrlog()`, gracefully disable voiceovers
- Audio file not found → log warning, continue with text-only
- Audio device error → catch and silence (already handled by `soundplay2()`)

### Multi-NPC Future Expansion
- JSON format is extensible; adding NPCs requires only:
  - New port + text-prefix entries in `audio_map.json`
  - New `.ogg` files in `assets/game_files/client/oog/`
  - Zero code changes

### Wire Protocol & Versioning
- **Not a wire change** — NPC messages already contain port + text; this is client-side only
- **No `U6O_VERSION` bump** required

### Performance Notes
- Text prefix matching is O(1) per NPC port (hash lookup), O(n) scan within lines (small n)
- Audio load latency: DirectSound `soundload()` is sync, ~10–50ms typical per .ogg
- Recommend pre-cache for hot paths or async load on first-line-only

## Session Handoff

### Initial State
- Plan created; Chuckles voiceover audio file (`Chuckles.0.ogg`) already in `assets/game_files/client/oog/`
- Audio mapping strategy chosen: port + text prefix (asset JSON)
- Ready for Phase 1 (asset file creation) and Phase 2 (infrastructure)

### Next Steps
1. Discover exact Chuckles NPC port number
2. Create `assets/audio_map.json` with Chuckles entry
3. Implement `src/client/voiceover.h` + `src/client/voiceover.cpp`
4. Integrate hook into `loop_client_part_net.cpp`
5. Build and test with Chuckles conversation

### Blockers / Open Questions
- Chuckles port number — need to extract from NPC.BIN or clarify if known
- Exact text of Chuckles greeting line — verify against voiceover timing
- Volume level for voiceovers — default SFX volume or separate control?

## Branch & Commit Convention

When work begins, create and switch to:
```powershell
git checkout -b plan/npcVoiceoverMapping
```

Tag commits with phase ID if long-running:
```
[NPC-VO-P1] Create audio_map.json asset
[NPC-VO-P2] Implement voiceover infrastructure
[NPC-VO-P3] Integrate message decoder hook
```

