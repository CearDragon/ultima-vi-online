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

