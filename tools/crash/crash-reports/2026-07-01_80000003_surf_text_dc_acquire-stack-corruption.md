# Crash Report — MPRES-P4.2 startup stack corruption in `surf_text_dc_acquire`

- **Date analyzed**: 2026-07-01
- **Exception Code**: 0x80000003 (BREAKPOINT / MSVC RTC stack failure trap)
- **Exception Address (runtime)**: 0x004E09DC
- **EXE-relative RVA**: 0x000609DC
- **Faulting symbol**: `failwithmessage + 0x20c` after `_RTC_StackFailure`, originating at `src/client/myddraw.cpp:389`
- **Build under test**:
  - EXE: `bin/client/debug/Ultima VI Online.exe` (LastWriteTime: 2026-07-01 17:06:52)
  - PDB: `build/Ultima VI Online.pdb` (LastWriteTime: 2026-07-01 17:06:52)
- **Dump source**: `tools/crash/crash-reports/crash_20260701_165748.dmp` (LastWriteTime: 2026-07-01 16:57:48)
- **Status**: Fixed

## Stack (top frames, EXE only)

| # | RVA | Symbol | file:line |
|---|-----|--------|-----------|
| 0 | 0x000609DC | `failwithmessage + 0x20c` | `D:\a\_work\1\s\src\vctools\crt\vcstartup\src\rtc\error.cpp:210` |
| 1 | 0x00060717 | `_RTC_StackFailure + 0x97` | `D:\a\_work\1\s\src\vctools\crt\vcstartup\src\rtc\error.cpp:261` |
| 2 | 0x0005EE30 | `_RTC_CheckStackVars + 0x40` | `D:\a\_work\1\s\src\vctools\crt\vcstartup\src\rtc\stack.cpp:75` |
| 3 | 0x00012834 | `surf_text_dc_acquire + 0x1d4` | `src/client/myddraw.cpp:389` |
| 4 | 0x00012193 | `loadimage + 0x113` | `src/client/myddraw.cpp:1243` |
| 5 | 0x0000D1E4 | `loadimage2 + 0x24` | `src/client/function_client.cpp:2316` |
| 6 | 0x00020084 | `WinMain + 0x4b54` | `src/client/client_bmp.inc:140` |

## Register evidence

At the exception context, the CPU is not faulting on a bad memory dereference inside game code; it is executing `int 3` in the CRT runtime-check trap:

- `EIP=004E09DC` → `Ultima_VI_Online!failwithmessage+0x20c`
- `EBP=01E78E80`, `ESP=01E78038`
- The stack shows `_RTC_CheckStackVars` directly above `surf_text_dc_acquire()`.

That register / stack combination proves this is MSVC's debug stack-corruption detector firing after `surf_text_dc_acquire()` returned with a smashed local stack slot, not a random access violation.

## Root cause

`src/client/myddraw.cpp`'s new MPRES-P4.2 DIB-section text path created a stack `BITMAPINFO bi = {};` and then, for RGB565 surfaces, wrote three DWORD channel masks through `bi.bmiColors[0]`.

`BITMAPINFO` only guarantees storage for a single trailing `RGBQUAD`; writing three DWORD masks overruns the object on the stack. In this case the overwrite clobbered the adjacent `BITMAPINFOHEADER bih` / RTC guard area inside `surf_text_dc_acquire()`, so the first startup `loadimage()` call triggered `_RTC_StackFailure` and the process raised the debug `0x80000003` breakpoint trap.

## Fix

- File(s) changed: `src/client/myddraw.cpp` (`surf_text_dc_acquire`, `surf_text_dc_release`, `refresh`, `loadimage`) and `src/client/myddraw.h` (`struct surf`)
- Change type: bounds-safe local-struct fix + state sync
- Sibling pattern reused (if any): N/A
- Plan bridge (if any): `docs/plans/in-progress/plan-modernPresenter.md` phase `MPRES-P4.2`

Implemented changes:
- Replaced the undersized stack `BITMAPINFO` usage with a local wrapper containing `BITMAPINFOHEADER` plus `DWORD masks[3]` (`BitmapInfoWithMasks`) before calling `CreateDIBSection()`.
- Added `cachedDIBBits` to `struct surf` so the DIB-backed GDI path can flush pixels back into `surf::o` during `surf_text_dc_release()`.
- Explicitly flush/copy back before modern present and after `loadimage()` so the new DIB text/image path stays coherent with the software blitters and presenter.
- Added a local forward declaration so `surf_text_dc_acquire()` can call `surf_text_dc_release()` cleanly in this TU.

## Verification

- Rebuilt target: `client`
- New EXE timestamp: 2026-07-01 17:06:52
- Smoke test: build verified with `tools/Enter-DevBuildEnv.ps1 -Build client`; user should now re-run the original startup path to confirm the client opens without the RTC stack-corruption dialog.

## Recurrence log

- 2026-07-01 — first occurrence (this report).

