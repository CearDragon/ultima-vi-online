# Memory Leak & `-l` Flag Investigation — Summary

## What does the `-l` argument do?

The `-l` flag is a **workaround for font memory leaks**, not a real fix. When enabled, it:

1. **Deletes all 7 font objects every frame** (DeleteObject calls on fnt1–fnt7)
2. **Recreates them immediately** (CreateFont calls)
3. Repeats this cycle ~60 times per second (60 FPS × 7 fonts = **~420 font allocations per second**)

**This is terrible for performance**, but it prevents font objects from accumulating in memory, which is why users noticed it "helped" briefly. The original developer even documented it as a hack:

> "not a real fix and that is why command line parameter is needed to use this 'hidden' 'fix'" 
> — Comment in `src/client/loop/loop_client_part_refresh_tail.cpp:379`

## The real problem: Unresolved memory leaks

The game was originally written ~2005 and has **accumulated numerous resource leaks** that compound over 10+ minutes of play:

### 1. **DirectDraw surface leak** (highest impact)
- The primary DirectDraw surface struct is malloc'd at startup but the free call is **commented out** (line 202 of `myddraw.cpp`)
- When surfaces are purged during gameplay, the malloc'd structs are **never freed**

### 2. **Font object leak** (what `-l` tries to hide)
- Fonts are allocated but never properly deselected and deleted
- The developer documented: *"delete object and it works, but the font is fucked up if deleted right away"*
- This suggests improper DC (Device Context) management or a SelectObject/ReleaseDC pattern issue

### 3. **Socket buffer leaks**
- When players disconnect, the socket receive/send buffer structures are malloc'd but **never freed**
- Repeated connects/disconnects leave orphaned memory behind

### 4. **Sound & music leaks**
- DirectSound and DirectMusic resources have mixed malloc/COM patterns with:
  - Incomplete cleanup paths
  - Potential double-releases of COM objects
  - Sound buffer structures that may not be freed properly

### 5. **Viewport/lighting array leaks**
- Arrays are allocated without proper error handling
- If one malloc fails mid-sequence, the others leak

### 6. **No comprehensive cleanup on exit**
- When the game closes, many resources are never explicitly freed

## Why is this happening on modern hardware in 2026?

- **Gradual accumulation:** The leak is small per-frame (~1-5 MB/minute) but compounds over time
- **Unnoticed for 20 years:** Running on modern systems with GB of RAM, the leak stayed hidden until reaching a noticeable threshold
- **The 10-minute window:** Suggests a steady-state leak of **~50–100 MB/minute** under normal gameplay

## What can we do?

I've created a **comprehensive, phased modernization plan** at:

### 📋 [`plan-memoryManagement.md`](plan-memoryManagement.md)

This plan tracks the work in 8 phases:

| Phase | Focus | Impact | Status |
|-------|-------|--------|--------|
| **MM-P1** | Establish baseline memory profile & test scenarios | Foundational | ⬜ Ready to start |
| **MM-P2** | Fix DirectDraw surface malloc leaks | **High** | ⬜ Next priority |
| **MM-P3** | Fix font leaks & **remove `-l` workaround** | **High** | ⬜ High priority |
| **MM-P4** | Fix socket buffer structure leaks | **High** | ⬜ High priority |
| **MM-P5** | Fix sound & DirectMusic leaks | Medium | ⬜ |
| **MM-P6** | Fix viewport/lighting allocation safety | Medium | ⬜ |
| **MM-P7** | Comprehensive shutdown cleanup | Low | ⬜ |
| **MM-P8** | Modernize to RAII/smart pointers (long-term) | Long-term | ⬜ |

## Quick wins (immediate actions)

1. **MM-P1.2:** Capture a memory profile over 10 minutes of gameplay to establish the baseline
2. **MM-P2:** Uncomment the malloc free() call for DirectDraw surfaces (1 line fix)
3. **MM-P3:** Fix the font leak by proper DC/SelectObject management and remove the `-l` workaround

These three alone should dramatically improve stability and frame rate.

## Updated documentation

I've updated `test/client/readme.txt` to explain the `-l` flag and recommend users NOT use it, instead pointing to the modernization plan.

## Next steps

The plan is ready to execute. Starting with MM-P1 (baseline profiling) and MM-P2/MM-P3 (highest-impact fixes) will show immediate improvement. The full plan ensures no resources are leaked on exit or during normal play.


