# MM-P1.2 Memory Baseline Test Procedure (Fresh Rebuild)

**Status:** Ready to execute  
**Binary version:** 6/25/2026 11:50 AM (all MM-P2–MM-P7 fixes included)  
**Test date:** June 25, 2026

---

## Context

The previous memory dumps (6/25 Start/End) were taken from a **stale binary (6/15 build)** that did NOT contain any of the memory leak fixes. Those dumps have been archived as:
- `tools/crash/crash-reports/archive/Stale_Start_*.DMP`
- `tools/crash/crash-reports/archive/Stale_End_*.DMP`

This procedure will establish the **true baseline** with all fixes applied.

---

## Test Procedure

### Phase 1: Prepare

1. **Launch Task Manager** and open the **Performance** tab to watch memory.
2. **Optional but recommended:** Use Windows Performance Analyzer or VMMap for detailed heap analysis.
3. **Verify binary:** Check that `bin/client/debug/Ultima VI Online.exe` has `LastWriteTime = 6/25/2026 11:50+ AM`.

### Phase 2: Start Dump (Baseline)

1. **Launch the client:**
   ```powershell
   Start-Process "C:\Users\Owner\CLionProjects\ultima-vi-online\bin\client\debug\Ultima VI Online.exe"
   ```

2. **Allow 30 seconds** for full initialization (loading fonts, DirectDraw surfaces, DirectSound).

3. **Take Start memory dump:**
   - From `Ultima VI Online` process in Task Manager, right-click → **Create dump file**.
   - Save to: `C:\Users\Owner\CLionProjects\ultima-vi-online\tools\crash\Ultima VI Online - Start.DMP`
   - Note the reported **Private Memory** value from Task Manager (e.g., 369 MB).

### Phase 3: Sustained Play (10 minutes)

1. **Gameplay:**
   - **First 2 minutes:** Single-player mode, load a character, move around.
   - **Next 4 minutes:** Cast spells, pick up items, open UI panels, trigger sound effects.
   - **Final 4 minutes:** Stay in-world, continue movement. Observe frame rate and smoothness.
   
   **Important:** Leave the game window **visible and responsive**. Do not minimize.

2. **Watch for:**
   - Frame rate (should stay at ~60 FPS or near stable).
   - **Choppiness threshold:** the plan notes that around 300–400 MB RAM, gameplay becomes choppy. If you don't see choppiness at a higher memory level, that's a sign the leaks are fixed.
   - Memory growth in Task Manager (should be flat or slow growth, NOT steep).

### Phase 4: End Dump (After 10 minutes)

1. **Take End memory dump:**
   - From Task Manager, right-click the process → **Create dump file**.
   - Save to: `C:\Users\Owner\CLionProjects\ultima-vi-online\tools\crash\Ultima VI Online - End.DMP`
   - Note the reported **Private Memory** value (e.g., 450 MB, 500 MB, etc.).

2. **Calculate growth:**
   - Δ Memory = (End Memory - Start Memory)
   - Expected (with fixes): ~50–100 MB growth max (minor caching, textures, normal operation).
   - Expected (if leaks still present): 200+ MB growth (same pattern as old stale binary).

3. **Gracefully exit the game** (Quit from menu or press Q).

---

## Success Criteria

| Metric | Success | Failure |
|--------|---------|---------|
| **Memory growth (10 min)** | ≤ 100 MB | > 200 MB |
| **Frame rate** | Stable 60 FPS or near | Choppy, frame-dropping |
| **Gameplay smoothness** | Smooth, no lag around 300–400 MB | Choppy/unplayable at high memory |
| **No crashes** | Client exits cleanly | Crashes or hangs |

---

## Next Steps After Test

1. **If SUCCESS (≤ 100 MB growth, smooth gameplay):**
   - All MM-P2–MM-P7 fixes are working correctly.
   - Proceed to individual subsystem verification:
     - MM-P2.3: Confirm DirectDraw surfaces not accumulating.
     - MM-P3.3–3.4: Confirm font frame rate improved (measure FPS before/after `-l` removal).
     - MM-P4.3: Repeated connect/disconnect test.
     - MM-P5.4: Audio playback check.
     - MM-P6.3: Allocation failure injection (optional).
     - MM-P7.2: Confirm shutdown logs appear (run with debugger attached).
   - Move to MM-P8 (RAII modernization).

2. **If PARTIAL SUCCESS (100–200 MB growth, but still playable):**
   - One or more leaks may not be fully patched.
   - Proceed to Phase 5 (heap analysis with cdb.exe) to identify what's still leaking.

3. **If FAILURE (200+ MB growth, choppy at 300–400 MB):**
   - The fixes may not have been compiled in, or there are additional leaks not covered by MM-P2–P7.
   - Proceed to Phase 5 (heap analysis).

---

## Phase 5: Heap Analysis (If Needed)

If memory growth is still high, run this to identify remaining leak sources:

```powershell
$cdb = "C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\cdb.exe"
$startDump = "C:\Users\Owner\CLionProjects\ultima-vi-online\tools\crash\Ultima VI Online - Start.DMP"
$endDump = "C:\Users\Owner\CLionProjects\ultima-vi-online\tools\crash\Ultima VI Online - End.DMP"
$exe = "C:\Users\Owner\CLionProjects\ultima-vi-online\bin\client\debug\Ultima VI Online.exe"
$pdbDir = "C:\Users\Owner\CLionProjects\ultima-vi-online\build"

# Compare heap sizes between start and end
& $cdb -z $startDump -y "$pdbDir" -i $exe -lines -c "!address -summary; q"
& $cdb -z $endDump -y "$pdbDir" -i $exe -lines -c "!address -summary; q"
```

Then escalate to the `crash-report` agent with the two new dumps for detailed analysis.

---

## Troubleshooting

**"Binary won't launch"**
- Verify DirectX 7 compatibility mode is enabled on the EXE (right-click → Properties → Compatibility).
- Verify `bin/client/debug/` has the full output directory structure.
- Try: `.\tools\Enter-DevBuildEnv.ps1` then `.\bin\client\debug\Ultima VI Online.exe`.

**"Task Manager doesn't show Private Memory"**
- Right-click columns in Task Manager → add "Private Working Set" column.

**"Memory dumps won't create"**
- Ensure you have admin rights.
- Task Manager → Processes tab → right-click → Task Manager must be running as Administrator.

---

## Artifacts to Archive After Test

1. New memory dumps: `tools/crash/Ultima VI Online - {Start,End}.DMP`
2. Task Manager screenshots (optional, for manual review).
3. Frame rate benchmark (if you captured it).

Document the results in a new section under MM-P1.2 in `docs/plans/in-progress/plan-memoryManagement.md`.

---

## References

- **Memory plan:** `docs/plans/in-progress/plan-memoryManagement.md` (Session handoff § MM-P1.2)
- **Leak reference:** `docs/plans/in-progress/MEMORY_LEAK_QUICK_REFERENCE.md`
- **Binary build log:** This session's rebuild completed at 11:50 AM.
- **Archived old dumps:** `tools/crash/crash-reports/archive/Stale_{Start,End}_*.DMP`

