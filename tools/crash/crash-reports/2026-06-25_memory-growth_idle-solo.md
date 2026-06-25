# Memory Growth Report — Idle solo client climbs from ~100 MB to ~570 MB

- **Date analyzed**: 2026-06-25
- **Scenario**: logged into the current client, stood still for ~13 minutes, no user actions
- **Artifacts**:
  - `tools/crash/crash-reports/Start.DMP`
  - `tools/crash/crash-reports/End.DMP`
- **Build under test**:
  - EXE: `bin/client/debug/Ultima VI Online.exe` (LastWriteTime: 2026-06-25 14:34:15)
  - PDB: `build/Ultima VI Online.pdb` (LastWriteTime: 2026-06-25 14:34:15)
- **Status**: Open in the captured binary; mitigated in current source via the MM-P9 fixes already in tree

## Executive summary

The two dumps show a large, steady increase in committed private memory while the client was idle:

| Metric | Start.DMP | End.DMP | Delta |
|---|---:|---:|---:|
| Process uptime at capture | 0:00:28 | 0:32:30 | +0:32:02 |
| Committed memory | 380,944,384 bytes | 842,866,688 bytes | +461,922,304 bytes |
| Committed private regions | 263 | 3,540 | +3,277 |
| Committed private bytes | 108,453,888 bytes | 570,376,192 bytes | +461,922,304 bytes |

That is the key signal: the client is not merely reserving address space, it is actually committing and retaining hundreds of megabytes of private memory over a single idle session.

## Stack evidence from the end dump

`cdb` shows the active UI thread in GDI text rendering when the end snapshot was taken:

| Thread | Top observed frames |
|---|---|
| 0 | `gdi32full!SelectBrushLocal` → `gdi32full!GetFontRealizationInfo` → `gdi32full!GdiRealizationInfo` → `gdi32full!LpkUseGDIWidthCache` → `gdi32full!LpkPSMTextOut` → `gdi32full!LpkExtTextOut` → `gdi32full!ExtTextOutAImpl` → `gdi32full!TextOutA` |

This does not point to a crash; it simply confirms the process was actively painting UI text while memory had already ballooned.

## What grew

The end dump contains far more committed private regions than the start dump, which is consistent with repeated allocations that are not being released. The largest committed private region in the end snapshot is a newly present ~17.9 MB block (`0x11B4000`), and the total count of committed private regions jumped from 263 to 3,540.

That pattern is much more consistent with repeated surface / heap allocations than with a single large reserve or image mapping.

## Root cause

The growth pattern matches the already-documented **MM-P9 portrait reload surface leak**:

- the host re-streams portrait data for already-loaded portrait indices,
- the old binary allocated a fresh `56x64` `SURF_SYSMEM16` source surface for each type-43 portrait packet,
- `loadportrait()` then overwrote the cached `portrait_doublesize[]` / `portrait_halfsize[]` surfaces without releasing the old ones,
- each reload therefore leaked committed private memory and also accumulated `surf` entries in `surflist[]`, which explains the progressive sluggishness once memory rose into the hundreds of megabytes.

The important nuance is that the current source tree already contains the MM-P9 fixes:

- `src/client/function_client.cpp:1517-1537` now frees the previous portrait derivatives before rebuilding them.
- `src/client/loop/loop_client_part_net.cpp:1661-1673` reuses the cached portrait surface instead of allocating a new one for every refresh.
- `src/client/function_client.cpp:2765-2795` and `src/common/u6o7.cpp:519-525` add the MM-P9.1 / MM-P9.2 teardown cleanup.

So these dumps are best treated as a **pre-rebuild baseline** captured from a stale binary that still exhibited the leak. The source-side fix is already in the repository, but the executable under test was built before the verification rerun.

## Why the slowdown appears around ~500 MB

The client becomes choppy because the leak is not just consuming memory; it is also increasing the amount of surface and heap state the renderer has to carry around. Once the committed-private working set grows large enough, every frame becomes more expensive and the game starts to lag even though no player action is occurring.

## Progress-tracked plan to resolve it

The work is already tracked in `docs/plans/in-progress/plan-memoryManagement.md` under **MM-P9**. The remaining verification phase is:

- **MM-P9.4** — rebuild from the current source and rerun the 10-minute idle benchmark

### Recommended next actions

1. Rebuild `client`, `host`, and `both` from the current source tree.
2. Rerun the idle-session test for ~10 minutes.
3. Capture new start/end dumps and compare committed private memory.
4. If the curve is flat, close out MM-P9.4 as verified.
5. If the curve still climbs, continue down the next host-driven allocation candidate.

## Notes for future investigations

- These dumps are memory snapshots, not an exception crash, so there is no faulting EIP to symbolize.
- The most useful baseline for future comparisons is committed private bytes, not just task-manager working set.
- The current plan file already contains the full MM-P9 root-cause write-up; this report is the concise evidence record for this particular idle-session capture.

---

## Investigation addendum (2026-06-25, follow-up) — attribution corrected

The user **rebuilt from the current source** (which already contains the MM-P9
portrait fixes) and the idle climb **still reproduces** (~100 MB → ~570 MB). That
contradicts this report's "stale binary / MM-P9.3 portrait reload" conclusion, so
that attribution is **withdrawn**. A fresh source audit established:

- The MM-P9.3 portrait fix is present and correct, and `getportrait*` gates
  requests via `portrait_requested[]` (never reset), so the host sends portrait
  data (type 43) **once per index** — portraits load once, bounded. Not the leak.
- **Every** `newsurf`/`loadimage` call site in the client is startup-only,
  guarded by a one-time flag, or frees-before-realloc (resize, `qkstf`,
  `loadportrait`, `receiveport` reuse). There is **no per-frame surface
  creation**, so surfaces should not leak.
- Movers/sobj write fixed arrays (sobj frees-before-realloc); DirectSound voices
  are a bounded `tempsound[256]` ring; `inpmess`/`idlst` are bounded; the
  status-log wrap txts are reused; GDI handles are pool-capped (cannot account
  for 461 MB). All ruled out.

Conclusion: the previous two agent reports (inpmess/idlst, then portraits) both
**mis-attributed** the leak. The true leaker is not identifiable by static
reading and the dumps can't be re-introspected here (no `cdb`/WinDbg, and the
`.DMP` files are not in the repo).

### Action taken — diagnostic build (ship + measure)

Added low-risk, behavior-preserving live-count instrumentation so the next run
**measures** the leaking pool instead of guessing:

- `src/common/txt.cpp` / `mytxt.h` — `g_txt_live` (++ in `txtnew`, -- in `free(txt*)`).
- `src/client/myddraw.cpp` — `g_surf_live` (++ in `surfstruct`, -- in `free(surf*)`),
  plus a **5-second heartbeat** in `txtout()` that emits
  `U6O-DIAG surf_live=<n> txt_live=<n>` via `OutputDebugStringA`.

All three targets build clean.

### How to use it (one short run)

1. Launch the rebuilt client and attach **DebugView** (Sysinternals) or run under
   the debugger; filter on `U6O-DIAG`.
2. Stand idle ~3–5 minutes while watching the heartbeat **alongside Task
   Manager's "Commit size" / private bytes** for the client.
3. Read the result:
   - `surf_live` climbs in lockstep with memory → a **DirectDraw surface** leak
     (re-audit `newsurf` callers / `free(surf*)`; there is a hidden per-frame
     surface allocation or a missing free).
   - `txt_live` climbs → a **txt** leak (find the per-frame/per-message `txtnew`
     without a matching `free`).
   - **Both flat** while memory climbs → a **raw `malloc`** leak (next suspects:
     per-message buffers, map-download `MAPDL_*`, DirectSound duplicates).
4. Report the three numbers (surf_live, txt_live, commit) at start and after a
   few minutes; that uniquely identifies the pool and I'll fix the specific site.

### Result of the first instrumented run (2026-06-25) — surf/txt RULED OUT

User ran the diagnostic client idle. Findings:

- `surf_live` and `txt_live` are **flat** (818→834, 842→862 — bounded jitter,
  not growth) while Working Set and Commit **rise steadily**. → The leak is
  **neither** a DirectDraw-surface **nor** a `txt` leak. Every prior theory
  (inpmess/idlst, portraits, surfaces) is now empirically dead.
- **The leak reproduces at the login/main-menu screen**, *before* "Journey
  Onward", with `surf_live`/`txt_live` flat there too. At that screen there is
  **no network**, **no sfx**, and the only per-frame work is render (verified
  clean — `txtout`/`txtouts`/menu all pair `GetDC`/`ReleaseDC`) plus a single
  DirectMusic `IsPlaying()` poll. The audible constant is **MIDI music**.
- On exit the process **hangs 5–8 s while the MIDI keeps playing**, then ends —
  consistent with DirectMusic/DirectSound still holding committed buffers.

This points away from our counted pools and toward an **uncounted** pool: raw
CRT heap (malloc/new), GDI/USER handles, or **DirectX-internal** memory
(dsound/dmusic/ddraw). Music being the sole common factor at the login screen
makes the audio subsystem the leading suspect by elimination.

### Action taken — heartbeat upgraded to 4-pool localization

`src/client/myddraw.cpp` heartbeat now also reports (every 5 s):

```
U6O-DIAG surf_live=<n> txt_live=<n> heapKB=<n> heapN=<n> gdi=<n> user=<n>
```

- `heapKB` / `heapN` — outstanding debug-CRT heap (malloc/new) bytes & block
  count (`_CrtMemCheckpoint`, `_NORMAL_BLOCK`). Debug build only.
- `gdi` / `user` — process GDI and USER handle counts (`GetGuiResources`,
  user32 — no new link dependency).

Interpretation on the next run, alongside Task Manager **Commit**:

| What climbs | Leak is in | Next step |
|---|---|---|
| `heapKB`/`heapN` | raw `malloc`/`new` | `_CrtSetBreakAlloc(heapN-at-leak)` to catch the call site |
| `gdi` / `user` | GDI / USER handle | find the unmatched Create*/GetDC |
| `surf_live` / `txt_live` | DirectDraw surface / txt | (already ruled out, but re-confirm) |
| **none**, but Commit rises | **DirectX-internal** (dsound/dmusic/ddraw) | instrument the audio play/duplicate paths |

All three targets build clean.



