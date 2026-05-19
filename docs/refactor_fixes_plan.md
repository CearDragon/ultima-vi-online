# Refactor Branch Fix Plan: Slimes and Triple Crossbow

## 1. Slime Split Bounds Checking Underflow Fix
**Issue:** 
When Slimes replicate on the map edge, subtracting 1 from `slimex` or `slimey` if they are at 0 causes an integer underflow because they are defined as `unsigned long`, resulting in `4294967295`. This leads to a `0xC0000005` segmentation fault when attempting to access the `od[slimey][slimex]` array.

**Action Plan:**
- Target Files: `loop_host.cpp` and `loop_host.inc`
- Add coordinate bounds validation wrapper before `goto slimedivide;`.
- Ensure limits (`slimex < 2048 && slimey < 1024`) are enforced on both the `x` and `y` axes before any mapping arrays are accessed in the division loops.

## 2. Triple Crossbow Center Map Crash Fix
**Issue:** 
The Triple Crossbow crashes when fired in the center of the map. This is likely an array out-of-bounds error or poor offset logic related to checking central coordinates, tied to standard execution tracks in `loop_host.cpp` (e.g., `triplecrossbow_shot_x2`, `triplecrossbow_refire`, `ENHANCEget_attack`).

**Action Plan:**
- Target Files: `loop_host.cpp`
- Audit `triplecrossbow_*` logic segments.
- Identify how center-map coordinates (likely around 1024x512) interact with the crossbow's 3-way projectile offsetting or targeting logic.
- Prevent invalid offset calculations from causing memory corruption when projecting the three bolts simultaneously on boundary or center origin map coordinates.

## 3. Crash Logs Improvement
**Issue:**
When the game crashes, there are no error outputs printed to `log.txt` or the console, requiring inspection of the Windows Event Viewer.

**Action Plan:**
- Target Files: `u6o7.cpp`
- Implemented `MyUnhandledExceptionFilter` using Win32 API structured exception handling.
- Registered via `SetUnhandledExceptionFilter()` inside `_tWinMain()`.
- Captures memory violation codes and addresses (`ExceptionCode` and `ExceptionAddress`) and outputs them to both a local `crash.txt` backup log and the standard `log.txt` via `LOGadd()`.
- Added textual stack trace dumping using `CaptureStackBackTrace` to print out execution frame pointers.
- Integrated `MiniDumpWriteDump` from `<dbghelp.h>` to generate a full `crash.dmp` file whenever an unhandled exception occurs. This allows opening the crash in Visual Studio or WinDbg to see exact lines of code, variable states, and threads at the exact moment of the crash.

## 4. Deployment
- Checkout the `refactor` branch.
- Apply the Slime patch to both `.cpp` and `.inc`.
- Apply the Tripe Crossbow bug patch.
- Compile checking against Visual Studio 2022 Dev command prompt locally (`-arch=x86`) to verify memory safety implementations.
- Commit the finalized fixes to the refactor branch.
