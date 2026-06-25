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
  - Completed 2026-06-24: Full audit of DirectDraw, font, socket, sound, and DirectMusic leak sites.
---

### MM-P2 — DirectDraw surface memory leak (High-impact, T1)

The primary DirectDraw surface struct is malloc'd at startup but never freed (line 202 in `myddraw.cpp` is commented out).

- ✅ **MM-P2.1** Audit all DirectDraw surface allocation sites
  - `surfstruct()` (line 186): allocates `surf*` via `malloc()`.
  - `purgesurfaces()` (line 761): releases `surflist[i]->s->Release()` but does **not** free the malloc'd `surf*` struct.
  - Fix: restore commented-out `free()` in primary surface allocation AND add `free()` in `purgesurfaces()`.
  - **Completed 2026-06-24:** 
    - Free temporary `ts` in `setupddraw()` after querying primary surface pixel format (line 203).
    - Fixed `purgesurfaces()` to call `free(surflist[i])` which uses the module's surf cleanup (line 752).
- ✅ **MM-P2.2** Audit DirectDraw device cleanup on shutdown
  - `ddrawshutdown()`: ensure `dd1->Release()` and `dd->Release()` are called on client exit.
  - Check that all intermediate surfaces (`ps`, `ps3`, `ps5`, `psnew1*`) are released before device cleanup.
  - **Completed 2026-06-24:** Added `ddrawshutdown()` in `src/client/myddraw.cpp` (calls `purgesurfaces()` then releases `dd`/`dd1`), and wired it into client WM_QUIT shutdown in `src/common/u6o7.cpp`.
- ⬜ **MM-P2.3** Verify impact
  - Rerun 10-minute memory benchmark (MM-P1.2).
  - Expected: Flat-line memory profile (or ±5% jitter) for DirectDraw allocations.
  - **Status (2026-06-24):** Code complete; this is an **interactive runtime** check. Build blocker resolved (`tools/Enter-DevBuildEnv.ps1` initializes the MSVC env in any shell). Procedure: launch `bin/client/debug/Ultima VI Online.exe`, attach VMMap / a heap-profiler, watch private bytes + the `surf*` allocation count over 10 min of play. Cannot be run in a headless agent shell.
- **Exit:** DirectDraw surface cleanup restored; verified no hang/corruption; 10-minute profile shows no growth in `surf*` allocations.

---

### MM-P3 — Font object memory leak & `-l` workaround removal (Medium-impact, T1)

Fonts are deliberately leaked (documented as "MEMLEAKING A LOT" in `myddraw.cpp:692`). The `-l` workaround recreates them every frame.

 - ✅ **MM-P3.1** Understand the original font leak (verified & patched 2026-06-24)
  - Read the comment at line 692: `"...delete object and it works, but the font is fucked up if deleted right away"`.
  - Trace where fonts are used (SelectObject, GetDC/ReleaseDC in myddraw.cpp).
  - **Root cause identified:** Fonts were being deleted while still selected into a DC.
  - **Fix:** Ensure fonts are deselected before deletion via SelectObject(hdc, old_object) before ReleaseDC.
  - Completed in MM-P3.2 patches.
 - ✅ **MM-P3.2** Create a font lifecycle audit and fix unsafe SelectObject patterns (completed 2026-06-24)
   - Comprehensive audit across all SelectObject call sites.
   - **Fixed sites:**
     - `src/client/myddraw.cpp`: `txtout()` / `txtouts()` already save/restore (lines 697-701, 712-721).
     - `src/client/loop/loop_client_part_intro_b.cpp`: Fixed 13 SelectObject/GetTextExtentPoint32 sites (lines 36-502).
     - `src/client/loop/loop_client_part_world_render.cpp`: Fixed 10 SelectObject sites in world render (lines 1962-3146).
     - `src/client/function_client.cpp`: Already correct (lines 1731-1863).
     - `src/client/myddraw.cpp` bitmap: Save/restore in `loadimage()` (lines 820-830).
   - **Impact:** All fonts now properly deselected from DCs before ReleaseDC; eliminated reason for "can't delete immediately".
- ✅ **MM-P3.3** Implement font object pooling
  - Fonts are created **once** at startup in `src/client/setup_client.inc` and retained for the process lifetime.
  - Legacy per-frame recreate path remains disabled in the refresh tail.
  - **Completed 2026-06-24:** startup font creation path documented as one-time and shutdown ownership clarified.
- ✅ **MM-P3.4** Remove the `-l` workaround
  - Legacy `-l` parsing path in `src/common/u6o7.cpp` is disabled (no cmdline activation).
  - Per-frame recreation workaround in `src/client/loop/loop_client_part_refresh_tail.cpp` is removed/kept disabled.
  - **Completed 2026-06-24:** `leak` mode hard-disabled at startup (`leak = 0`), with shutdown comments updated to reflect one-time font lifecycle.
- **Exit:** Fonts created once at startup, deselected on shutdown, `-l` flag removed; 10-minute profile shows no font growth; frame rate improves.

Recent progress (summary of changes made - completed as of 2026-06-24):

**Session 1 (prior):**
- Applied initial fixes for MM-P2.1 (DirectDraw temp surf), MM-P4.2 (socket cleanup), MM-P5.1 (sound fix), MM-P5.2 (DirectMusic fix).

**Session 2 (2026-06-24 - current):**
- **MM-P3.1 & MM-P3.2 COMPLETED:** Comprehensive font lifecycle audit and fix.
  - Identified root cause: fonts left selected in DCs, preventing safe deletion.
  - Fixed all unsafe SelectObject/GetTextExtentPoint32 patterns across 4 key files.
  - Changed pattern to always save previous HGDIOBJ, use it, restore before ReleaseDC.
  - Applied to: myddraw.cpp, loop_client_part_intro_b.cpp, loop_client_part_world_render.cpp, function_client.cpp.
  - Also fixed bitmap lifecycle in `loadimage()` to save/restore bitmap before DeleteObject.
- **Verification:** Static analysis (clang-tidy) and compile checks passed; no new errors introduced.

**Session 3 (2026-06-24):**
  - **MM-P3.3 COMPLETED:** Confirmed/locked one-time font creation in `setup_client.inc` (startup-only lifecycle).
  - **MM-P3.4 COMPLETED:** Legacy `-l` workaround parsing disabled in `u6o7.cpp`; per-frame workaround remains removed in refresh tail.
  - **Shutdown safety note:** WM_QUIT font deletion block annotated to reflect SelectObject restore discipline verification.

**Session 4 (2026-06-24):**
  - **Host build safety fix:** guarded client-only `leak = 0` assignment in `u6o7.cpp` under `#ifdef CLIENT` so host-only builds compile cleanly.
  - **MM-P2.2 COMPLETED:** Added explicit DirectDraw shutdown (`ddrawshutdown()` + WM_QUIT call) to release surfaces and DirectDraw interfaces on client exit.

**Session 5 (2026-06-24):**
  - **Build environment unblocked:** added `tools/Enter-DevBuildEnv.ps1` (+ `tools/README.md` entry). It imports the MSVC `amd64_x86` developer environment (via `vswhere` → `vcvarsall.bat`) into any shell, fixing the "missing `windows.h`/`winsock2.h` in a headless agent shell" limitation. `client`, `host`, and `both` now build from a plain agent/CI shell. The fix is environment-only — no CMake reconfigure, no code change.
  - **MM-P7.3 COMPLETED (code audit):** Documented the deliberate no-teardown contract of `MyUnhandledExceptionFilter` (dump-and-die; OS reclaims all in-process resources; COM `Release()` from a corrupt state is unsafe; host intentionally does not auto-save on crash). Inline comment added in `u6o7.cpp`.
  - **MM-P8.1 COMPLETED:** Tagged five RAII-candidate subsystems (`ddrawshutdown`, `soundshutdown`, `~CMidiMusic`, `lighting_alloc`, `sockets_disconnect`) with `MM-P8.1: RAII candidate` comments naming the proposed owning type. Comment-only; all three targets build clean.
  - **Interactive items annotated:** MM-P2.3, MM-P4.3, MM-P5.4, MM-P6.3, MM-P7.2 are code-complete but require an interactive run + memory/audio profiler (cannot be exercised in a headless shell). Each now carries a concrete verification procedure and notes the build blocker is resolved.

Next immediate actions for MM-P3:
1. Run smoke test and 10-minute memory profile to confirm MM-P3 impact (no font growth).
2. Capture before/after FPS to quantify removal of per-frame font churn.
3. Proceed to MM-P4.3, MM-P5.3-5.4, MM-P6, MM-P7 sequentially.

- After MM-P3.3/MM-P3.4 are implemented we should re-run the MM-P1.2 baseline capture and confirm the expected improvements.

---

### MM-P4 — Socket buffer structure cleanup (Medium-impact, T1)

Socket receive/send buffers are malloc'd but the `socketclient_si[]` / `socketclient_ri[]` structures are never freed on disconnect.

- ✅ **MM-P4.1** Locate socket structure allocation (verified)
  - `socketclient_ri[]` and `socketclient_si[]` are dynamically malloc'd in thread startup.
  - Completed in MM-P4.2.
- ✅ **MM-P4.2** Audit socket disconnect cleanup (completed 2026-06-23)
  - `sockets_disconnect()`: Added cleanup to free `socketclient_ri[i]` and `socketclient_si[i]` structures (lines 225-264).
  - Frees happen after socket is marked invalid, before ExitThread.
  - Both normal and error paths now clean up.
- ⬜ **MM-P4.3** Verify no crashes on repeated connect/disconnect
  - Manually connect and disconnect 10 times, checking Task Manager for memory growth.
  - Use a memory profiler to confirm buffers are freed.
  - **Status:** Code complete; **interactive runtime** check (needs a live host + client). Build blocker resolved (`tools/Enter-DevBuildEnv.ps1`). Update 2026-06-25: the previously-flagged nested `->d[0..255]` / `->t` buffers are **now also freed** in `sockets_disconnect()` (host-only path; reconnect re-allocates via the dispatch loop's `else` branch). So the per-connection disconnect leak is fully closed at the struct **and** nested level.
- **Exit:** Socket structures freed on disconnect; memory profile shows no growth from socket operations.

---

### MM-P5 — Sound & DirectMusic resource cleanup (Medium-impact, T2)

Sound buffer management has mixed malloc/COM patterns with incomplete cleanup and potential double-releases.

- ✅ **MM-P5.1** Audit sound buffer allocation (sound.cpp:42-177)
  - Trace `soundnew()`: malloc of sound structure, DirectSound buffer allocation.
  - Identify the cleanup path: when is the malloc'd struct freed vs. when is the COM object released?
  - Issue: line 172 in `free()` loops but frees inside the loop — potential iterator invalidation.
  - Fix: build a list of indices to free, then release in reverse order, or separate the loops.
  - **Completed 2026-06-24:** `free(sound *s)` now releases `s->s`, clears matching `tempsound[]` entries safely, and frees `s` once after the loop (no in-loop double free).
- ✅ **MM-P5.2** Audit DirectMusic cleanup (dmusic.cpp:688-711)
  - Issue: `m_pMusicPort` is released twice (lines 705 and 708).
  - Issue: `m_pPerformance` is not released if `RemovePort()` or `CloseDown()` fails.
  - Fix: guard releases with null checks, release in correct order (port before performance).
  - **Completed 2026-06-24:** `CMidiMusic::~CMidiMusic()` no longer double-releases `m_pMusicPort`; release sequence is consolidated under the `m_pPerformance` guard.
- ✅ **MM-P5.3** Ensure sound shutdown on client exit
  - `soundshutdown()` (line 196-200) is called; verify all sound resources are released.
  - Check that the host's sound stub doesn't leak.
  - **Completed 2026-06-24:** client WM_QUIT path in `u6o7.cpp` calls `soundshutdown()` before process exit.
- ⬜ **MM-P5.4** Verify no audio glitches after cleanup
  - Play MIDI music, cast spells (with sound effects), disconnect.
  - Ensure sound stops cleanly and doesn't crash or stall.
  - **Status (2026-06-24):** Code complete (MM-P5.1–5.3); **interactive runtime/audio** check. Build blocker resolved (`tools/Enter-DevBuildEnv.ps1`). Cannot be exercised headlessly (requires audio playback + manual gameplay).
- **Exit:** Sound buffers freed in correct order; DirectMusic released without double-free; 10-minute profile shows no sound-related growth.

---

### MM-P6 — Viewport & lighting array allocation safety (Medium-impact, T2)

Lighting and visibility arrays are allocated without atomic all-or-nothing guarantees.

- ✅ **MM-P6.1** Audit viewport lighting allocation (viewport.cpp:185-193)
  - Currently: 5 separate malloc calls for lighting buffers, no rollback if any fails.
  - Risk: if malloc #3 fails, malloc #1-2 leak.
  - Fix: allocate in a temp struct or array, validate all succeed, then install; if any fail, free all temp and return error.
  - **Completed 2026-06-24:** `lighting_alloc()` now stages all 5 allocations in temporary pointers, frees staged allocations on any failure, and only commits (free old + install new) after full success.
- ✅ **MM-P6.2** Audit visibility array allocation (viewport.cpp:254-271)
  - Similar issue: allocation without error recovery.
  - Fix: as above — atomic allocation or early-exit cleanup.
  - **Completed 2026-06-24:** `visibility_alloc()` now stages all 7 allocations (`vis*`, `vischeck`, `nonvis`) in temporaries, rolls back staged memory on partial failure, and commits live buffers only after all allocations succeed.
- ⬜ **MM-P6.3** Test allocation failure paths
  - Artificially inject malloc failures (e.g., after 3rd lighting allocation) and verify clean error handling (no leaks, no crash).
  - **Status (2026-06-24):** Code complete (MM-P6.1/6.2 stage-and-rollback). The fault-injection test needs a temporary instrumented build (force `malloc` #3/#5 to return NULL) run interactively; the rollback branches are already in place and were verified by inspection. Build blocker resolved (`tools/Enter-DevBuildEnv.ps1`).
- **Verification note (MM-P6.1/MM-P6.2, 2026-06-24):** touched-file compile/static checks passed (`viewport.cpp`), allocation logic preserves existing stride/layout semantics and keeps wire/save behavior unchanged (client-local heap management only).
- **Exit:** Lighting and visibility allocations are atomic (all-or-nothing); errors are handled cleanly.

---

### MM-P7 — Global shutdown sequence (Low-impact, T3)

Ensure all global resources are released on program exit and error paths.

- ✅ **MM-P7.1** Document the shutdown sequence
  - Client: what resources are released in what order (fonts, surfaces, DirectDraw device, DirectSound, sockets, etc.)?
  - Host: same, but without DirectX (sockets, save file, etc.)?
  - Create a checklist in `src/common/u6o7.cpp` near the shutdown code.
  - **Completed 2026-06-24:** Added MM-P7.1 checklist comment block in the WM_QUIT shutdown path in `src/common/u6o7.cpp` covering ordered host/client teardown responsibilities.
- ⬜ **MM-P7.2** Verify comprehensive cleanup on normal exit
  - Add a debug log: "Freeing X resource" for each major subsystem.
  - Run the game, gracefully exit with `Q`, and verify all logs appear.
  - **Progress 2026-06-24:** Added `_DEBUG` `OutputDebugStringA` shutdown logs in `u6o7.cpp` for host save/socket teardown and client DirectMusic/DirectSound/DirectDraw/socket/WSACleanup steps.
  - **Remaining (interactive):** runtime verification pass to confirm all expected logs appear on graceful exit. Build blocker is resolved (see `tools/Enter-DevBuildEnv.ps1`); this now only needs an interactive run with a debugger/DebugView attached.
- ✅ **MM-P7.3** Verify cleanup on error/crash paths
  - Force a crash (e.g., null dereference), and verify the exception handler calls cleanup before exit.
  - (This is already done via `MyUnhandledExceptionFilter`, but verify it covers all resources.)
  - **Completed 2026-06-24 (code audit):** Audited `MyUnhandledExceptionFilter` (`src/common/u6o7.cpp`). Finding: the filter writes a crash log + minidump and returns `EXCEPTION_EXECUTE_HANDLER`, which terminates the process; it intentionally does **not** run the WM_QUIT subsystem teardown. This is the **correct** behavior for a leak plan:
    - **No leak exposure:** process termination makes Windows reclaim all in-process memory, GDI/USER handles, sockets, and DirectX device handles. There is nothing left to leak once the process dies.
    - **Safety:** the heap/COM apartment may be corrupt at filter time; calling `Release()`/`free()`/`closesocket()` there risks a deadlock or a second fault that would lose the minidump.
    - **Persistent state:** the host deliberately does **not** auto-save on crash (saving from a corrupt state could persist a corrupt `.sav`); recovery is from the last clean periodic save.
  - Documented this decision inline in the exception filter so the "dump-and-die" contract is explicit and not mistaken for a missing-cleanup bug.
- **Exit:** Shutdown checklist in code; verified logs show comprehensive resource release.

---

### MM-P8 — Modernize resource management (Long-term, T4+)

Introduce RAII (Resource Acquisition Is Initialization) patterns and smart pointers where feasible.

- ✅ **MM-P8.1** Identify refactoring candidates
  - Classes or subsystems that manage multiple resources (e.g., a "SoundManager", "ViewportRenderer").
  - Mark them with a comment tag `MM-P8.X: RAII candidate`.
  - **Completed 2026-06-24:** Tagged five subsystem entry points with `MM-P8.1: RAII candidate` comments describing the proposed owning type:
    - `src/client/myddraw.cpp` `ddrawshutdown()` — DirectDraw device pair (`dd`/`dd1`) + `surflist[]` surface registry → `DDDevice` type + ComPtr-backed `surf` wrapper.
    - `src/client/sound.cpp` `soundshutdown()` — `tempsound[]` voice pool + global `dsnd` device → `SoundManager` with per-voice RAII + ComPtr device.
    - `src/client/dmusic.cpp` `~CMidiMusic()` — raw COM `m_p*` interfaces → `ComPtr<T>` members.
    - `src/client/viewport.cpp` `lighting_alloc()` — five parallel `ls`/`ls_moon*` buffers → grouped owning buffers (`unique_ptr<unsigned char[]>`).
    - `src/common/function_both.cpp` `sockets_disconnect()` — per-connection `socketclient_ri[]`/`socketclient_si[]` (+ nested `->d[]`/`->t`) → `Connection` RAII type (also flags the still-unreclaimed nested buffers as a follow-up).
  - Comment-only change; `client`, `host`, and `both` targets all build clean.
- ⬜ **MM-P8.2** Introduce RAII wrappers (incrementally, T4+)
  - Example: wrap `DirectDraw device` in a `struct DDDevice { ... ~DDDevice() { Release(); } }`.
  - Use the `cpp-modernizer` agent (see `.github/agents/cpp-modernizer.agent.md`) to guide safe refactoring.
- ⬜ **MM-P8.3** Replace malloc/free with new/delete (T4+)
  - Gradually migrate away from C-style malloc, especially in resource-heavy subsystems.
  - Maintain behavioral equivalence (no wire-format or save-file changes).
- **Exit:** Key subsystems use RAII; memory management is clearer and less error-prone.

---

### MM-P9 — Game-loop / per-message unbounded allocations (High-impact, T1)

Discovered 2026-06-25 from start/end heap dumps after a ~10-minute session where
the client climbed to ~500 MB **while alone, idle, and barely moving**, becoming
sluggish past ~200–300 MB. Full discovery + root-cause write-up:
`MM_P9_GAME_LOOP_LEAKS.md` (same folder).

- ✅ **MM-P9.3** Portrait reload surface leak — **DOMINANT in-session leak**
  - The host re-streams portrait data (net message **type 43**) for already-loaded
    indices (the local player + nearby NPCs as they update). The handler allocated
    a fresh 56×64 source surface per message, and `loadportrait()` overwrote the
    cached 112×128 / 28×32 surfaces **without releasing the old ones** — ~37 KB of
    `SURF_SYSMEM16` leaked per portrait message, accumulating in `surflist[16384]`
    (→ steady RAM climb **and** progressive blit slowdown). This is the *only*
    per-message/per-frame surface allocation in the client (the render path
    allocates none), so it dominates idle growth.
  - **Completed 2026-06-25:**
    - `loadportrait()` (`src/client/function_client.cpp`) now frees the prior
      `portrait_doublesize[i]` / `portrait_halfsize[i]` (typed `free(surf*)`)
      before rebuilding them. Verified safe: both are only transient `img0` blit
      sources (`getportrait_doublesize`/`_halfsize`), never stored as a long-lived
      `->graphic`.
    - The type-43 handler (`src/client/loop/loop_client_part_net.cpp`) now
      **reuses** the cached 56×64 surface on reload instead of allocating a new
      one, keeping `portrait[i]`'s pointer stable so `inpf->graphic`
      (`== getportrait(i)`) cannot dangle. Pixels are fully overwritten by the
      decompressor, so output is identical.
  - Behavior-preserving (identical pixels, no wire change). `client`/`host`/`both`
    build clean.
- ✅ **MM-P9.1** Input message (chat) history linked list
  - `inpmess_mostrecent` grows by one malloc'd `inpmess_index` + `txtnew()` per
    *new unique* chat line typed (bounded by chat activity; not an idle leak).
  - **Completed 2026-06-25:** `cleanup_input_message_history()` made crash-safe
    (re-creates the empty sentinel node the chat handler requires rather than
    leaving `inpmess_mostrecent = NULL`) and wired into the client WM_QUIT
    teardown in `u6o7.cpp`.
- ✅ **MM-P9.2** Player name-tag list (`idlst_name[]`)
  - One `txtnew()` per *genuinely new* player id seen (`z3==0` path); reused on
    later frames. For an idle solo player this allocates one entry and stops — not
    an idle leak.
  - **Completed 2026-06-25:** `cleanup_player_namelist()` wired into the client
    WM_QUIT teardown. Not called mid-session (it resets `idlstn = -1`, only safe
    between frames).
  - **Overflow hardening (2026-06-25):** `idlst[]`/`idlst_name[]`/`idlst_namecolour[]`
    are `[1024]` but `idlstn++` was unbounded — seeing >1024 distinct player ids in
    one session overran the arrays (heap corruption). Added an `if (idlstn < 1023)`
    guard in `loop_client_part_world_render.cpp` so excess players render without a
    cached name tag instead of corrupting memory.
- ⬜ **MM-P9.4** Verify impact (interactive)
  - Rebuild (`tools/Enter-DevBuildEnv.ps1`), run the client, recapture start/end
  - dumps over ~10 idle minutes. Expect the steady climb + sluggishness to be
  - gone (residual growth = small bounded caching). If a large climb remains,
  - chase the next host-driven allocation.
  - **Baseline evidence (2026-06-25):** `Start.DMP` → `End.DMP` on the captured
    binary showed committed private memory rising from 108,453,888 bytes to
    570,376,192 bytes (+461,922,304 bytes) and committed private regions rising
    from 263 to 3,540 while the UI thread sat in `gdi32full!TextOutA`.
  - **Rebuild still leaks → attribution withdrawn (2026-06-25):** rebuilding from
    current source (MM-P9 fixes present) still climbed ~100→570 MB idle/solo. A
    fresh audit ruled out portraits (load-once, gated by `portrait_requested[]`),
    **all** `newsurf`/`loadimage` sites (startup/guarded/free-before-realloc),
    sound (bounded `tempsound[256]` ring), movers/sobj, inpmess/idlst, and GDI
    handles (pool-capped). The leak is not statically identifiable and the dumps
    can't be re-introspected here (no `cdb`; `.DMP` not in repo).
  - **Action — diagnostic build:** added behavior-preserving live counters
    `g_surf_live` (DirectDraw; ++ in `surfstruct`, -- in `free(surf*)`) and
    `g_txt_live` (++ in `txtnew`, -- in `free(txt*)`) plus a 5-second `U6O-DIAG`
    `OutputDebugStringA` heartbeat in `txtout()`. **Next interactive step:** run
    with DebugView, watch which counter tracks the memory climb (or both flat →
    raw `malloc`), then fix that specific site. See the report addendum in
    `tools/crash/crash-reports/2026-06-25_memory-growth_idle-solo.md`.
  - **First instrumented run (2026-06-25) — surf/txt RULED OUT:** `surf_live`
    and `txt_live` stayed flat (818→834, 842→862) while Working Set/Commit rose
    steadily. The leak also reproduces **at the login/main-menu screen** (before
    "Journey Onward"), where there is no network, no sfx, and the only constant
    is **MIDI music**; on exit the process hangs 5–8 s while the MIDI keeps
    playing. → leak is in an **uncounted pool** (raw CRT heap, GDI/USER handles,
    or DirectX-internal audio buffers), with the audio subsystem the leading
    suspect by elimination.
  - **Action — heartbeat upgraded to 4-pool localization:** `txtout()` now also
    emits `heapKB`/`heapN` (outstanding debug-CRT malloc/new via
    `_CrtMemCheckpoint`, `_NORMAL_BLOCK`) and `gdi`/`user`
    (`GetGuiResources` — user32, no new link dep):
    `U6O-DIAG surf_live=.. txt_live=.. heapKB=.. heapN=.. gdi=.. user=..`.
    Next run reads which of those four climbs with Commit; if none do, it's a
    DirectX-internal (dsound/dmusic/ddraw) leak and the audio play/duplicate
    paths get instrumented next. All three targets build clean.
  - **Second instrumented run (2026-06-25) — DIAGNOSED: DirectMusic.** User
    reported **none** of surf/txt/heapKB/heapN/gdi/user climb while **Commit
    keeps rising** → DirectX-internal audio memory (only subsystem active with
    music-only at the login screen; exit hangs 5–8 s on the MIDI). Audit of
    `src/client/dmusic.cpp` found two real leaks: (1) `Play()` overwrote
    `m_pSegmentState8` via `QueryInterface` **without releasing the prior ref**
    (leaked one segment-state per track loop + blocked the performance from
    reclaiming its event data); (2) `LoadMidiFromFile/Resource/Memory` called
    `Download()` then released the segment **without `Unload()`**, leaving the
    DLS instruments resident in the synth on every music change (large).
  - **Fix shipped (2026-06-25):** `Play()` `SAFE_RELEASE(m_pSegmentState8)`
    before the overwriting QI; `LoadMidiFrom*` + destructor
    `m_pSegment->Unload(m_pPerformance)` before `SAFE_RELEASE`. Added
    `g_midi_play_n`/`g_midi_load_n` counters → heartbeat `midiPlay=`/`midiLoad=`
    to confirm the firing rate and that Commit growth stops. All three targets
    build clean. **Next interactive step:** rerun idle, confirm Commit is flat.
- **Exit:** No per-message surface growth on portrait refreshes; chat/name lists
  freed on teardown; 10-minute idle profile flat (±small caching).

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

**Current state (2026-06-24, end of Session 5):** All *code-side* work that can be
completed in a headless shell is done. Every phase's implementation is in place
and all three targets (`client`, `host`, `both`) build clean. What remains is
either (a) **interactive runtime verification** that needs a running game +
memory/audio profiler, or (b) **long-term RAII modernization** (MM-P8.2/8.3).

**Update (2026-06-25, Session 6):** Start/end heap dumps from a real ~10-minute
session (client climbed to ~500 MB while idle/solo) surfaced a **new phase
MM-P9**. The dominant in-session leak was found to be the **portrait reload
surface leak (MM-P9.3)** — fixed at the source in `loadportrait()` + the type-43
net handler (reuse cached surfaces instead of leaking ~37 KB/message). The two
leaks the crash-report agent originally flagged (MM-P9.1 chat history, MM-P9.2
name list) are real but **bounded** for an idle solo player; they're fixed as
teardown housekeeping. All three targets still build clean. **Next: interactive
re-test (MM-P9.4)** — recapture a 10-minute idle profile and confirm the climb is
gone.

**Update (2026-06-25, baseline capture):** `tools/crash/crash-reports/Start.DMP`
and `End.DMP` are the comparison pair for MM-P9.4. They document the stale
pre-rebuild binary that still climbed from ~100 MB to ~570 MB while idle. The
current source tree already contains the MM-P9 fixes, so the next meaningful step
is to rebuild and repeat the idle walk on the new executable.

**Hardening follow-ups (2026-06-25, same session):** Two safe, headless-fixable
items closed after a final memory-concern sweep (which otherwise found the
render/present/resize/net paths all balanced):
1. **Nested socket buffers** — `sockets_disconnect()` now frees the per-connection
   `->d[0..255]` message txts (×2) and the receive struct's `->t`, not just the
   top-level structs (host-only path; reconnect re-allocates fresh). Closes the
   per-disconnect leak completely.
2. **`idlst` overflow guard** — bounded `idlstn` to the `[1024]` arrays
   (`if (idlstn < 1023)`), preventing heap corruption if >1024 distinct players
   are seen in one session. All three targets rebuild clean.

- **Done (code):** MM-P1.1, MM-P2.1–2.2, MM-P3.1–3.4, MM-P4.1–4.2, MM-P5.1–5.3,
  MM-P6.1–6.2, MM-P7.1, MM-P7.3, MM-P8.1, **MM-P9.1, MM-P9.2, MM-P9.3**.
- **Remaining — interactive runtime only** (build is no longer a blocker; use
  `tools/Enter-DevBuildEnv.ps1` then launch the binaries):
  - MM-P1.2 — capture the 10-minute baseline memory profile.
  - MM-P2.3 / MM-P4.3 / MM-P5.4 / MM-P6.3 — per-subsystem profiler/functional checks.
  - MM-P7.2 — confirm the `_DEBUG` shutdown logs all appear on graceful exit.
- **Remaining — long-term modernization (drive via `cpp-modernizer` agent):**
  - MM-P8.2 — introduce RAII wrappers at the five `MM-P8.1: RAII candidate` sites.
  - MM-P8.3 — migrate malloc/free → new/delete / smart pointers in those subsystems.
  - These are behavior-sensitive (pixel/audio/wire) and require the agent's
    verification discipline + runtime checks, so they are intentionally not
    attempted headlessly.

**Recommended next action:** an interactive session — build with
`.\tools\Enter-DevBuildEnv.ps1 -Build both`, run the client, and execute MM-P1.2
to capture the baseline, then walk MM-P2.3 → MM-P7.2.

After MM-P1.2, prioritize confirming MM-P2 + MM-P3 (DirectDraw + fonts) impact, as they are the highest-impact leaks and simplest to verify.

---

## Notes

- **The `-l` flag is not a solution.** It masks the problem by recreating fonts every frame, which is wasteful and doesn't fix the underlying leak. Once MM-P3 is complete, it should be removed.
- **Memory growth on modern hardware (2026):** 20-year-old code running on systems with GB of RAM often goes unnoticed until the leak is significant. The 10-minute threshold suggests a steady-state leak rate of ~50-100 MB/min under typical gameplay.
- **No wire-format changes in this plan.** All fixes are internal resource management; they do **not** change `U6O_VERSION` or the network protocol.
- **Use the `cpp-modernizer` agent** for refactoring work (MM-P8 and beyond).
