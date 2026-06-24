# Memory Management & Leak Remediation Plan (Trackable)

## Overview

Users report the game experiences significant memory leaks and slowdown after ~10 minutes of play on modern hardware. The legacy codebase (originally written ~2005) has numerous resource management issues accumulated over 20+ years:

1. **DirectDraw surfaces** allocated but never freed
2. **Font objects** recreated every frame as a workaround (via `-l` flag) instead of fixing the leak
3. **Socket buffer structures** malloc'd but never freed on disconnect
4. **Sound/DirectMusic resources** with incomplete cleanup and double-releases
5. **Viewport/lighting arrays** allocated with no atomic all-or-nothing guarantee
6. **Global resource cleanup** missing on shutdown

The `-l` command-line workaround (discovered in `test/client/readme.txt`) deletes and recreates all 7 font objects every single frame — a band-aid that masks the real problem while making performance worse.

## Problem statement

The game can run for 10+ minutes before memory pressure becomes noticeable, suggesting:
- Gradual accumulation of unreleased DirectDraw/sound/socket resources
- Font recreation loop consuming CPU every frame (when `-l` is enabled)
- No comprehensive resource cleanup on player disconnect or game shutdown
- Mixed malloc/COM-object management with incomplete error recovery

This plan addresses the root causes in phases, starting with the highest-impact leaks and moving toward comprehensive resource management discipline.

---

## Phases

### MM-P1 — Analysis & characterization (Foundational)

Establish a baseline understanding and reproducible test case.

- ✅ **MM-P1.1** Identify all memory-leak hotspots in the codebase (see Background section below).
- ⬜ **MM-P1.2** Create a benchmark: measure memory growth over 10 minutes of gameplay
  - Build a test scenario (connect, move around, chat, cast spells, use inventory) that exercises the main UI and network loops.
  - Capture heap snapshots at 0m, 5m, 10m using a profiler (e.g., Windows Task Manager, Visual Studio Memory Profiler, or Dr. Memory).
  - Document the baseline: typical memory growth rate (MB/min), peak allocations, and which subsystems dominate.
- ⬜ **MM-P1.3** Verify that `-l` flag reproduces the original symptom
  - Measure frame rate and memory growth with `-l` enabled vs. disabled.
  - Confirm that recreation loop (7 fonts × 60 FPS = ~420 font creations/sec) is CPU-measurable but doesn't fix the underlying leak.
- **Exit:** Baseline memory profile (text table: time, heap size, top allocators); reproduction of `-l` behavior.

---

### MM-P2 — DirectDraw surface memory leak (High-impact, T1)

The primary DirectDraw surface struct is malloc'd at startup but never freed (line 202 in `myddraw.cpp` is commented out).

- ⬜ **MM-P2.1** Audit all DirectDraw surface allocation sites
  - `surfstruct()` (line 186): allocates `surf*` via `malloc()`.
  - `purgesurfaces()` (line 761): releases `surflist[i]->s->Release()` but does **not** free the malloc'd `surf*` struct.
  - Fix: restore commented-out `free()` in primary surface allocation AND add `free()` in `purgesurfaces()`.
- ⬜ **MM-P2.2** Audit DirectDraw device cleanup on shutdown
  - `ddrawshutdown()`: ensure `dd1->Release()` and `dd->Release()` are called on client exit.
  - Check that all intermediate surfaces (`ps`, `ps3`, `ps5`, `psnew1*`) are released before device cleanup.
- ⬜ **MM-P2.3** Verify impact
  - Rerun 10-minute memory benchmark (MM-P1.2).
  - Expected: Flat-line memory profile (or ±5% jitter) for DirectDraw allocations.
- **Exit:** DirectDraw surface cleanup restored; verified no hang/corruption; 10-minute profile shows no growth in `surf*` allocations.

---

### MM-P3 — Font object memory leak & `-l` workaround removal (Medium-impact, T1)

Fonts are deliberately leaked (documented as "MEMLEAKING A LOT" in `myddraw.cpp:692`). The `-l` workaround recreates them every frame.

- ⬜ **MM-P3.1** Understand the original font leak
  - Read the comment at line 692: `"...delete object and it works, but the font is fucked up if deleted right away"`.
  - Trace where fonts are used (SelectObject, GetDC/ReleaseDC in myddraw.cpp).
  - Hypothesis: fonts are being deleted while still selected into a DC, or the DC is released before fonts are deselected.
  - Expected fix: ensure fonts are deselected before deletion, and DCs are properly managed.
- ⬜ **MM-P3.2** Create a font lifecycle audit
  - List all `CreateFont()` call sites.
  - For each, trace the DC and SelectObject pattern:
    - Is the font selected into a DC? 
    - Is it deselected before deletion?
    - Is the DC obtained via `GetDC()`? Is it released?
  - Document the fix in a comment near the CreateFont sites.
- ⬜ **MM-P3.3** Implement font object pooling
  - Instead of recreating fonts every frame (or leaking them), implement a simple pool:
    - Create fonts **once** at startup (in `setup_client.inc`).
    - Deselect them properly before shutdown.
    - If `-l` is still enabled, it becomes a no-op (fonts are already persistent).
  - Verify fonts render correctly and no corruption occurs.
- ⬜ **MM-P3.4** Remove the `-l` workaround
  - Delete the `-l` flag parsing in `u6o7.cpp:349` and the workaround loop in `loop_client_part_refresh_tail.cpp:381`.
  - Update `test/client/readme.txt` to remove the `-l` recommendation.
  - Expected: Frame rate should improve significantly (no per-frame font deletion/creation overhead).
- **Exit:** Fonts created once at startup, deselected on shutdown, `-l` flag removed; 10-minute profile shows no font growth; frame rate improves.

---

### MM-P4 — Socket buffer structure cleanup (Medium-impact, T1)

Socket receive/send buffers are malloc'd but the `socketclient_si[]` / `socketclient_ri[]` structures are never freed on disconnect.

- ⬜ **MM-P4.1** Locate socket structure allocation
  - Find where `socketclient_si` and `socketclient_ri` are initialized.
  - Check if they are static arrays or dynamically allocated.
  - If dynamic, identify the malloc site; if static, verify bounds.
- ⬜ **MM-P4.2** Audit socket disconnect cleanup
  - `sockets_disconnect()` (around line 225-264): currently only closes the socket handle.
  - Add cleanup: free any malloc'd buffers **and** the structure pointers if dynamic.
  - Ensure cleanup happens both on normal disconnect and error paths (e.g., timeout, protocol error).
- ⬜ **MM-P4.3** Verify no crashes on repeated connect/disconnect
  - Manually connect and disconnect 10 times, checking Task Manager for memory growth.
  - Use a memory profiler to confirm buffers are freed.
- **Exit:** Socket structures freed on disconnect; memory profile shows no growth from socket operations.

---

### MM-P5 — Sound & DirectMusic resource cleanup (Medium-impact, T2)

Sound buffer management has mixed malloc/COM patterns with incomplete cleanup and potential double-releases.

- ⬜ **MM-P5.1** Audit sound buffer allocation (sound.cpp:42-177)
  - Trace `soundnew()`: malloc of sound structure, DirectSound buffer allocation.
  - Identify the cleanup path: when is the malloc'd struct freed vs. when is the COM object released?
  - Issue: line 172 in `free()` loops but frees inside the loop — potential iterator invalidation.
  - Fix: build a list of indices to free, then release in reverse order, or separate the loops.
- ⬜ **MM-P5.2** Audit DirectMusic cleanup (dmusic.cpp:688-711)
  - Issue: `m_pMusicPort` is released twice (lines 705 and 708).
  - Issue: `m_pPerformance` is not released if `RemovePort()` or `CloseDown()` fails.
  - Fix: guard releases with null checks, release in correct order (port before performance).
- ⬜ **MM-P5.3** Ensure sound shutdown on client exit
  - `soundshutdown()` (line 196-200) is called; verify all sound resources are released.
  - Check that the host's sound stub doesn't leak.
- ⬜ **MM-P5.4** Verify no audio glitches after cleanup
  - Play MIDI music, cast spells (with sound effects), disconnect.
  - Ensure sound stops cleanly and doesn't crash or stall.
- **Exit:** Sound buffers freed in correct order; DirectMusic released without double-free; 10-minute profile shows no sound-related growth.

---

### MM-P6 — Viewport & lighting array allocation safety (Medium-impact, T2)

Lighting and visibility arrays are allocated without atomic all-or-nothing guarantees.

- ⬜ **MM-P6.1** Audit viewport lighting allocation (viewport.cpp:185-193)
  - Currently: 5 separate malloc calls for lighting buffers, no rollback if any fails.
  - Risk: if malloc #3 fails, malloc #1-2 leak.
  - Fix: allocate in a temp struct or array, validate all succeed, then install; if any fail, free all temp and return error.
- ⬜ **MM-P6.2** Audit visibility array allocation (viewport.cpp:254-271)
  - Similar issue: allocation without error recovery.
  - Fix: as above — atomic allocation or early-exit cleanup.
- ⬜ **MM-P6.3** Test allocation failure paths
  - Artificially inject malloc failures (e.g., after 3rd lighting allocation) and verify clean error handling (no leaks, no crash).
- **Exit:** Lighting and visibility allocations are atomic (all-or-nothing); errors are handled cleanly.

---

### MM-P7 — Global shutdown sequence (Low-impact, T3)

Ensure all global resources are released on program exit and error paths.

- ⬜ **MM-P7.1** Document the shutdown sequence
  - Client: what resources are released in what order (fonts, surfaces, DirectDraw device, DirectSound, sockets, etc.)?
  - Host: same, but without DirectX (sockets, save file, etc.)?
  - Create a checklist in `src/common/u6o7.cpp` near the shutdown code.
- ⬜ **MM-P7.2** Verify comprehensive cleanup on normal exit
  - Add a debug log: "Freeing X resource" for each major subsystem.
  - Run the game, gracefully exit with `Q`, and verify all logs appear.
- ⬜ **MM-P7.3** Verify cleanup on error/crash paths
  - Force a crash (e.g., null dereference), and verify the exception handler calls cleanup before exit.
  - (This is already done via `MyUnhandledExceptionFilter`, but verify it covers all resources.)
- **Exit:** Shutdown checklist in code; verified logs show comprehensive resource release.

---

### MM-P8 — Modernize resource management (Long-term, T4+)

Introduce RAII (Resource Acquisition Is Initialization) patterns and smart pointers where feasible.

- ⬜ **MM-P8.1** Identify refactoring candidates
  - Classes or subsystems that manage multiple resources (e.g., a "SoundManager", "ViewportRenderer").
  - Mark them with a comment tag `MM-P8.X: RAII candidate`.
- ⬜ **MM-P8.2** Introduce RAII wrappers (incrementally, T4+)
  - Example: wrap `DirectDraw device` in a `struct DDDevice { ... ~DDDevice() { Release(); } }`.
  - Use the `cpp-modernizer` agent (see `.github/agents/cpp-modernizer.agent.md`) to guide safe refactoring.
- ⬜ **MM-P8.3** Replace malloc/free with new/delete (T4+)
  - Gradually migrate away from C-style malloc, especially in resource-heavy subsystems.
  - Maintain behavioral equivalence (no wire-format or save-file changes).
- **Exit:** Key subsystems use RAII; memory management is clearer and less error-prone.

---

## Background: Identified leak sites

### Critical leaks (MM-P2, MM-P3, MM-P4, MM-P5)

| File | Lines | Issue | Severity | Phase |
|------|-------|-------|----------|-------|
| `src/client/myddraw.cpp` | 202 | Primary DirectDraw surface malloc never freed (commented out) | Critical | MM-P2 |
| `src/client/myddraw.cpp` | 761-769 | `purgesurfaces()` releases COM surfaces but not malloc'd `surf*` structs | Critical | MM-P2 |
| `src/client/myddraw.cpp` | 691-717 | Font leak documented; `-l` workaround recreates every frame | High | MM-P3 |
| `src/client/loop/loop_client_part_refresh_tail.cpp` | 381 | `-l` workaround loop: 7 fonts × 60 FPS = ~420 creations/sec | High | MM-P3 |
| `src/common/function_both.cpp` | 176-184 | Socket RX buffer malloc without freeing `socketclient_ri` struct | High | MM-P4 |
| `src/common/function_both.cpp` | 293-300 | Socket TX buffer malloc without freeing `socketclient_si` struct | High | MM-P4 |
| `src/common/function_both.cpp` | 225-264 | `sockets_disconnect()` doesn't free structures | High | MM-P4 |
| `src/client/sound.cpp` | 42-177 | Sound buffer malloc/free with mixed COM management | Medium | MM-P5 |
| `src/client/dmusic.cpp` | 688-711 | DirectMusic destructor: double-release of port, missing performance release | Medium | MM-P5 |
| `src/client/viewport.cpp` | 185-193 | Lighting allocation: 5 mallocs without atomic guarantee | Medium | MM-P6 |
| `src/client/viewport.cpp` | 254-271 | Visibility allocation: no error recovery | Medium | MM-P6 |

### Cleanup gaps (MM-P7)

- `src/common/u6o7.cpp`: No comprehensive shutdown documentation or checklist.
- Host-side shutdown: socket/guild save cleanup present but incomplete.
- Global destructors: no RAII, manual cleanup is error-prone.

---

## Verification strategy (per `cpp-modernizer` discipline)

Each phase includes explicit verification before shipping:

1. **Baseline capture** (MM-P1.2): 10-minute memory profile, peak allocations.
2. **Regression test** after each phase: re-run memory profile, expect flat-line or improved.
3. **Functional test**: manual gameplay (connect, move, cast, chat, disconnect) to ensure no corruption.
4. **Profiler validation**: confirm malloc sites are no longer growing; use a heap dump tool.
5. **Frame-rate benchmark** (especially MM-P3): measure FPS before/after `-l` removal.

---

## Session handoff

Starting from the top:

1. **Next action:** Run MM-P1.1 (analysis done); begin MM-P1.2 (capture baseline memory profile).
2. **Then:** Proceed in order — MM-P2, MM-P3, MM-P4, MM-P5, MM-P6, MM-P7.
3. **Long-term:** MM-P8 (RAII modernization) is lower-priority and can be staged with other C++ refactoring efforts.

After MM-P1.2, prioritize MM-P2 + MM-P3 (DirectDraw + fonts) as they are the highest-impact leaks and simplest to verify.

---

## Notes

- **The `-l` flag is not a solution.** It masks the problem by recreating fonts every frame, which is wasteful and doesn't fix the underlying leak. Once MM-P3 is complete, it should be removed.
- **Memory growth on modern hardware (2026):** 20-year-old code running on systems with GB of RAM often goes unnoticed until the leak is significant. The 10-minute threshold suggests a steady-state leak rate of ~50-100 MB/min under typical gameplay.
- **No wire-format changes in this plan.** All fixes are internal resource management; they do **not** change `U6O_VERSION` or the network protocol.
- **Use the `cpp-modernizer` agent** for refactoring work (MM-P8 and beyond).


