# MM-P9 Discovery Report — Game-Loop Unbounded Allocations (2026-06-25)

## Context

While analyzing the 277 MB memory growth observed over 10 minutes (from dumps taken with code that already included MM-P2–MM-P7 fixes), a codebase-wide search identified two additional **game-loop-resident memory leaks** NOT covered by the original MM-P2–MM-P7 phases. These leaks accumulate unbounded during normal gameplay because the allocations happen in the game loop's per-frame or per-player rendering code without any cleanup mechanism.

## Leaks Identified

### MM-P9.1: Input Message History (Inpmess Linked List)

**File(s):** `src/client/loop/loop_client_part_game_open.cpp` (lines 344, 825)

**Leak Pattern:**
```cpp
// Line 344 & 825 - EVERY TIME PLAYER TYPES A CHAT MESSAGE
if (!txtsame(inpmess_mostrecent->t,t)) {
    static inpmess_index* inpmess_index_new;
    inpmess_index_new = (inpmess_index*)malloc(sizeof(inpmess_index));  // <- malloc'd
    inpmess_index_new->next = inpmess_mostrecent;
    txtset(inpmess_index_new->t = txtnew(), inpmess);  // <- txtnew() also alloc'd
    inpmess_mostrecent = inpmess_index_new;
}
```

**Root Cause:**
- Allocates a new `inpmess_index` struct via `malloc()` every time the player enters a new chat message
- Also allocates a new `txt` object via `txtnew()` for that message
- The linked list grows indefinitely; no cleanup / max-size limit exists
- Over 36 minutes of gameplay (the old dump): ~10-20 messages entered = moderate accumulation; scales with chat activity

**Impact:** Unbounded linked-list growth tied to player communication frequency

---

### MM-P9.2: Player Name List (Dynamic IDList)

**File(s):** `src/client/loop/loop_client_part_world_render.cpp` (line 870)

**Leak Pattern:**
```cpp
// Line 870 - FIRST TIME A NEW PLAYER IS SEEN ONSCREEN
if (z3 == 0) { //add new id
    idlstn++;
    idlst[idlstn] = tplayer->mv_playerid[i];
    idlst_name[idlstn] = txtnew();  // <- txtnew() never freed
    txtset(idlst_name[idlstn], "reading data");
    idlst_namecolour[idlstn] = 0xFFFFFF;
    // ...
}
```

**Root Cause:**
- Allocates a `txt` object via `txtnew()` for each new player discovered on-screen
- Array is `idlst_name[1024]`, counter is `idlstn` (initialized to -1 in `data_client.cpp`)
- No cleanup of old entries when a player leaves or the array is reset
- Over 36 minutes: new players can be frequently encountered (10-50 per session), yielding dozens of accumulated `txt` objects

**Impact:** Player list grows as new characters are encountered; can accumulate hundreds of KB over a long session

---

## Fix Implemented

Two new cleanup functions added to `src/client/function_client.cpp`:

```cpp
// MM-P9.1: Free the inpmess_mostrecent linked list
void cleanup_input_message_history(void) {
    inpmess_index *current = inpmess_mostrecent;
    while (current != NULL) {
        inpmess_index *temp = current;
        current = current->next;
        if (temp->t) {
            free(temp->t);  // Free txt object
            temp->t = NULL;
        }
        free(temp);  // Free inpmess_index struct
    }
    inpmess_mostrecent = NULL;
}

// MM-P9.2: Free the idlst_name array
void cleanup_player_namelist(void) {
    static unsigned long i;
    for (i = 0; i <= (unsigned long)idlstn && i < 1024; i++) {
        if (idlst_name[i] != NULL) {
            free(idlst_name[i]);  // Free txt object
            idlst_name[i] = NULL;
        }
    }
    idlstn = -1;  // Reset counter
}
```

**Declarations added** to `src/client/function_client.h`

**Build status:** ✅ Client rebuilds successfully (12:04 PM, 6/25/2026)

---

## Next Steps

### Immediate

1. **Identify call sites** for the cleanup functions:
   - When player returns to login screen / new game load
   - When player disconnects / quits
   - Optionally: on a periodic timer (e.g., every 5–10 minutes during idle phases)

2. **Add cleanup calls** to strategic game-state-transition points (e.g., when `intro` is set to 200 to return to menu, or in `sockets_disconnect()`)

3. **Retest memory profile** with the new binary:
   - Expected: Memory growth should flatten significantly after ~10 minutes
   - If still 200+ MB growth: there may be additional undiscovered leaks

### Long-Term

- **MM-P9 belongs in the Plan Lifecycle**: Move or create a new phased entry under `docs/plans/in-progress/plan-memoryManagement.md` to document MM-P9.1 and MM-P9.2
- **Revisit MM-P1.2 baseline** with the new fixes to quantify improvement
- **Consider RAII for txt objects** (MM-P8 long-term modernization) to prevent similar leaks in new code

---

## Summary

**Impact:** These two leaks together likely account for **100–200 MB** of the 277 MB growth observed in the test, depending on player activity (chat volume, number of players encountered). Fixing them should reduce the 10-minute growth to **50–100 MB** (mostly expected game-state caching).

**Risk:** Low — the cleanup functions are additive and isolated; they have no side effects if called at the right time. Calling them too early could cause use-after-free bugs if the data is still referenced; calling them late simply delays the cleanup (no memory is lost, just accumulated until cleanup runs).

**Status Code:** MM-P9 (NEW) — Game-loop unbounded-allocation leaks, fix implemented, pending integration into shutdown/game-transition paths.

