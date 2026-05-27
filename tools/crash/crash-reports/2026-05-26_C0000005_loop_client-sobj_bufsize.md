# Crash Report — OOB read on `tplayer->sobj_bufsize[bufx][bufy]` in visibility loop

- **Date analyzed**: 2026-05-26
- **Exception Code**: 0xC0000005 (ACCESS_VIOLATION, read)
- **Exception Address (runtime)**: 0x0016E957
- **EXE-relative RVA**: 0x3E957 (EXE runtime base 0x00130000)
- **Faulting symbol**: `WinMain + 0x24A27` at `src/client/loop_client.cpp:6825`
- **Build under test**:
  - EXE: `bin/client/debug/Ultima VI Online.exe` (pre-fix build, 2026-05-26)
  - PDB: `build/Ultima VI Online.pdb` (matched)
- **Dump source**: `tools/crash/crash-reports/archive/crash_20260526.dmp`
  (legacy location: original was at `crash.dmp` in repo root)
- **Status**: Fixed

## Stack (top frames, EXE only)

| # | RVA      | Symbol            | file:line                          |
|---|----------|-------------------|------------------------------------|
| 0 | 0x3E957  | `WinMain + 0x24A27` | `src/client/loop_client.cpp:6825` |
| 1 | 0x18DFF  | (CRT thunk / SEH path — frames below are unreliable due to inline-asm `ebp` munging in `WinMain`) | — |

External frames `0x76D3F782` (kernel32) and `0x7740D8CD` / `0x7740D7C1`
(ntdll) are the standard SEH return path — diagnostic noise.

## Register evidence

- `EIP = 0x0016E957` → faulting instruction was `movzx edx, word ptr [ecx+eax*2]`
  (reading a `WORD` from a 2D `unsigned short` array).
- `ECX = 0`, `EAX = 0` → the row-base pointer the instruction tried to
  dereference was effectively NULL + 0. Combined with the source at line
  6825 (`tplayer->sobj_bufsize[bufx][bufy]`), this is an out-of-bounds
  index walking off the end of the per-player `[96][72]` fixed-size array
  on `struct player`, landing in unmapped heap memory.

## Root cause

`bufx` / `bufy` are computed at `loop_client.cpp:6764` as

```cpp
bufx = mapx - tplayer->sobj_bufoffx;
bufy = mapy - tplayer->sobj_bufoffy;
```

The per-player buffer storage in `struct player` (`data_both.h:166-169`) is
hard-coded to `[96][72]`. When the host's streamed `sobj_bufoffx/y` lags the
client's `tpx/tpy` by even one tile while walking — or when a viewport
config just barely fits inside the existing `RW-P4.11` cap — `bufx`/`bufy`
exceed 95/71 and the access walks past the end of the struct. The crash is
intermittent because whether it faults depends on heap layout immediately
after the buffer.

The sibling code path at `function_client.cpp:529` already had a guard
`(bufx >= 0) && (bufx < 96) && (bufy >= 0) && (bufy < 72)`; the visibility
loop was missing it.

This scenario is enumerated in `docs/plan-dynamicObjectBuffer.md` as the
motivating evidence for phase `DOB-P0.2` (add bounds asserts before the
long-term storage refactor).

## Fix

- File(s) changed: `src/client/loop_client.cpp` (lines 6797–6846)
- Change type: bounds guard (mirror of sibling pattern)
- Sibling pattern reused: `src/client/function_client.cpp:529`
- Plan bridge: `docs/plan-dynamicObjectBuffer.md` phase `DOB-P0.2`

Out-of-buffer tiles now leave `vis[x+1][y+1]` at its default instead of
dereferencing past the end of `sobj_bufsize` / `sobj` / `sobj_tempfixed`.

## Verification

- Rebuilt target: `u6oclient2` (clean build, only pre-existing `C4731`
  warnings on `WinMain` inline-asm `ebp` munging).
- New EXE timestamp: 2026-05-26 (post-fix).
- Smoke test: user to verify by reproducing the original walk path that
  triggered the crash.

## Follow-ups

- Long-term fix tracked as `DOB-P2`: replace fixed `[96][72]` storage with
  per-player `Dynamic2DArray` sized from `viewTilesX/Y`. Once shipped, the
  guard added in this fix becomes redundant and can be removed.

## Recurrence log

- 2026-05-26 — first occurrence (this report).

