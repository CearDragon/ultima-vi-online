# Crash Report — OOB read on `tplayer->sobj_bufsize[bufx][bufy]` in objbuffer render loop

- **Date analyzed**: 2026-05-27
- **Exception Code**: 0xC0000005 (ACCESS_VIOLATION, read)
- **Exception Address (runtime)**: 0x00A6109E
- **EXE-relative RVA**: 0x4109E (EXE runtime base 0x00A20000)
- **Faulting symbol**: `WinMain + 0x2713E` at `src/client/loop_client.cpp:7257`
- **Build under test**:
  - EXE: `bin/client/debug/Ultima VI Online.exe` (LastWriteTime: 2026-05-26 16:20:57, pre-fix)
  - PDB: `build/Ultima VI Online.pdb` (LastWriteTime: 2026-05-26 16:20:57, matched)
- **Dump source**: `tools/crash/crash-reports/archive/crash_20260527_102540.dmp`
  (originally `tools/crash/crash.dmp`, captured 2026-05-27 10:00:21)
- **Status**: Fixed
- **Related**: `2026-05-26_C0000005_loop_client-sobj_bufsize.md` — same root-cause class
  (same fixed-size `sobj_bufsize[96][72]` buffer, same `bufx`/`bufy` lag),
  different access site in the same source file.

## Stack (top frames, EXE only)

| #  | RVA       | Symbol                | file:line                          |
|----|-----------|-----------------------|------------------------------------|
| 0  | 0x4109E   | `WinMain + 0x2713E`   | `src/client/loop_client.cpp:7257`  |
| 1+ | 0x5CE5D, 0x5CD3A, 0x5CBDD, 0x5CEC8 | bogus EBP-chain frames (`operator new`, `_ltod3`, `_dtoul3_legacy`, `pre_c_initialization`) caused by inline-asm `ebp` munging in `WinMain` (warning `C4731` on lines 8492/8512/8514). Frame 0 is the reliable one. | — |

External frames `kernel32.dll+0x15D49`, `ntdll.dll+0x6E12B`, `ntdll.dll+0x6E0B1`
are the SEH return path — diagnostic noise.

## Register evidence

- `EIP = 0x00A6109E` → faulting instruction bytes `0F B7 14 41` =
  `movzx edx, word ptr [ecx+eax*2]` (read a `WORD` from a 2D
  `unsigned short` array).
- `ECX = 0`, `EAX = 0` → NULL-base + zero-index read. Identical opcode and
  register profile to the 2026-05-26 crash at `loop_client.cpp:6825`; the
  shifted RVA (0x4109E vs 0x3E957) is purely the post-fix code layout
  shift in the same object file.

## Root cause

`bufx`/`bufy` are computed at `loop_client.cpp:7255` as

```cpp
bufx = mapx - tplayer->sobj_bufoffx;
bufy = mapy - tplayer->sobj_bufoffy;
```

The per-player buffer storage in `struct player` (`data_both.h:166-169`) is
hard-coded to `[96][72]`. Host-streamed `sobj_bufoffx/y` briefly lags the
client's `tpx/tpy` while the player walks (host is the producer; the client
sees the previous frame's offset for one tick), and the iteration domain
`x = 0..viewTilesX()+1`, `y = 0..viewTilesY()+1` is bounded only by the
*viewport*, not by the *buffer*. When the lag and viewport size combine,
`bufx` or `bufy` exceeds 95/71 and the read walks past the end of the
`sobj_bufsize[96][72]` storage into unmapped (or unrelated) heap.

The bug is a **sibling** of the 2026-05-26 crash. That earlier fix added a
bounds guard at `loop_client.cpp:6808` around the visibility-loop access,
but the *same source file* has two additional unguarded access sites with
the same `bufx`/`bufy` index computation: the objbuffer render loop
(line 7257, faulting) and the floating-objbuffer render loop (line 7914,
not yet hit but identical pattern).

## Fix

- File(s) changed: `src/client/loop_client.cpp`
  - Lines ~7257–7385 (objbuffer render loop): wrapped in
    `if ((bufx>=0)&&(bufx<96)&&(bufy>=0)&&(bufy<72))` block.
  - Lines ~7914–7986 (objbuffer floating render loop): same guard
    applied prophylactically — same access pattern would have crashed
    identically.
- Change type: bounds guard (mirror of sibling pattern).
- Sibling pattern reused: `src/client/loop_client.cpp:6808` and
  `src/client/function_client.cpp:529`.
- Plan bridge: `docs/plan-dynamicObjectBuffer.md` phase `DOB-P0.2`.

Out-of-buffer tiles now fall through to the default (empty) rendering
state instead of dereferencing past the end of the `[96][72]` storage.
Comment in the code names this crash report and the prior one.

## Verification

- Rebuilt target: `u6oclient2` (clean rebuild, only pre-existing `C4731`
  warnings on `WinMain` inline-asm `ebp` munging at lines 8492/8512/8514).
- New EXE timestamp: 2026-05-27 10:24:39.
- Static cross-check: `grep_search` for `sobj_bufsize[bufx]` in client
  sources now shows **every** access site is either guarded or inside
  the host's owned-data write path (`loop_host.cpp` lines 2085/2121 —
  producer-side, bounded by the host's own buffer-extent loop counters).
- Smoke test: user to verify by reproducing the original walk path.

## Follow-ups

- Long-term fix tracked as `DOB-P2`: replace fixed `[96][72]` storage with
  per-player `Dynamic2DArray` sized from `viewTilesX/Y`. Once shipped, all
  three guards in `loop_client.cpp` and the one in `function_client.cpp`
  become redundant and can be removed in one pass.
- Stale generated copies in `src/client/loop_client.inc` (lines 5093,
  5490, 6143) contain the same unguarded pattern. If the `.inc` file is
  ever regenerated from `loop_client.cpp` (or is hand-edited as a parallel
  build artifact), the same guard must be carried over. Confirm with the
  build-system owner whether `.inc` is still consumed.

## Recurrence log

- 2026-05-27 — first occurrence at this access site (`loop_client.cpp:7257`).
  Sibling of 2026-05-26 fix at `loop_client.cpp:6825`.

