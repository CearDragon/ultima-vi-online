# MM-P9 Discovery Report — Game-Loop Unbounded Allocations (2026-06-25)

## ⚠️ Correction / root-cause update (2026-06-25, follow-up session)

A code re-audit (triggered by the symptom *"~500 MB while alone, idle, and
barely moving"*) found that the two leaks originally reported below
(**MM-P9.1 inpmess**, **MM-P9.2 idlst**) are **real but bounded** for an idle
solo player, so they **cannot** be the dominant cause of the in-session climb:

- **MM-P9.1 (chat history)** only grows when *you type a new unique message*.
- **MM-P9.2 (player name list)** only allocates for a *genuinely new player id*
  (`z3==0` path); `idlstn` is initialized once to `-1` and the entry is reused
  on every later frame. Alone → it allocates **one** `txt` (yourself) and stops.

Neither grows while you stand still. The **dominant, continuous, in-session
leak is a DirectDraw surface leak in the portrait reload path** — see
**MM-P9.3** below. That is what produces both the steady private-bytes climb and
the progressive sluggishness (surfaces pile up in `surflist[]`, slowing every
blit). MM-P9.1/MM-P9.2 are retained as **housekeeping** (they matter across long
sessions / repeated reconnects) and are now wired into client teardown.

---

### MM-P9.3: Portrait reload surface leak  ⟵ DOMINANT in-session leak

**File(s):**
`src/client/loop/loop_client_part_net.cpp` (type-43 handler, ~line 1662),
`src/client/function_client.cpp` (`loadportrait`, ~line 1517)

**Leak pattern:**
```cpp
// type-43 "receive portrait data" — runs every time the host pushes a portrait
static surf *receiveport = NULL;
receiveport = newsurf(56, 64, SURF_SYSMEM16);   // NEW 56x64 surface every message
...
loadportrait(x2, receiveport);                  // stores it as portrait[x2]

// loadportrait():
portrait[i] = s;
portrait_doublesize[i] = newsurf(112, 128, SURF_SYSMEM16);  // overwrites prior — leaked (~28 KB)
portrait_halfsize[i]   = newsurf(28, 32,  SURF_SYSMEM16);   // overwrites prior — leaked (~1.8 KB)
```

**Root cause:**
- The host re-streams portrait data (message type 43) for already-loaded
  indices — the local player and nearby NPCs as they move / update. Each such
  message allocated a **fresh** 56×64 source surface *and* `loadportrait`
  overwrote the cached 112×128 and 28×32 surfaces for that slot **without
  releasing the previous ones**.
- Net effect: **~37 KB of `SURF_SYSMEM16` leaked per portrait message**, each
  also consuming a `surflist[16384]` slot. This is the only per-message /
  per-frame *surface* allocation in the whole client (the render path allocates
  no surfaces), which is why it dominates and why it degrades blit speed.

**Why it looks like "idle" growth:** you don't have to move — the host keeps
sending portrait refreshes for you and surrounding NPCs, so the leak advances
while you stand still.

**Fix implemented (this session):**
1. **`loadportrait()`** now releases the prior `portrait_doublesize[i]` /
   `portrait_halfsize[i]` (via the typed `free(surf*)`) before rebuilding them.
   These are only ever used as transient `img0` blit sources
   (`getportrait_doublesize`/`_halfsize`), never stored as a long-lived
   `->graphic`, so freeing is safe.
2. The **type-43 handler** now **reuses** the existing cached 56×64 surface on
   reload (`if (portrait_loaded[x2] && portrait[x2]) receiveport = portrait[x2];`)
   instead of allocating a new one. The decompressor fully overwrites its
   pixels, and keeping the same pointer means `inpf->graphic`
   (which can hold `getportrait(x2) == portrait[x2]`) never dangles.

Result: **zero** net surface growth on portrait refreshes (steady-state), which
removes the dominant idle leak and the surface-churn sluggishness.

**Build status:** ✅ `client`, `host`, and `both` all build clean.

---

## Original report (MM-P9.1 / MM-P9.2)

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

### Done (2026-06-25 follow-up session)

1. **MM-P9.3 (dominant) fixed at the source** — portrait reload no longer leaks
   surfaces (see the correction section at the top). This is the fix that
   addresses the reported idle climb + sluggishness.
2. **MM-P9.1 cleanup made crash-safe** — `cleanup_input_message_history()` now
   re-creates the empty sentinel node the chat handler requires instead of
   leaving `inpmess_mostrecent = NULL` (which would have crashed the next
   keystroke). 
3. **MM-P9.1 / MM-P9.2 cleanups wired** into the client teardown path in
   `src/common/u6o7.cpp` (single-threaded WM_QUIT shutdown — no UAF/cross-thread
   risk). They were intentionally **not** called mid-session: `cleanup_player_namelist`
   resets `idlstn = -1`, which is only safe between frames, and the in-session
   growth of both lists is bounded anyway.

### Remaining (interactive)

1. **Retest the 10-minute memory profile** with the new binary (use
   `tools/Enter-DevBuildEnv.ps1` to build, then run the client). Expectation:
   the steady idle climb is gone; residual growth should be small, bounded
   game-state caching. If a large climb remains, capture start/end dumps again —
   there may be a further host-driven allocation to chase.

### Long-Term

- **Consider RAII for `surf`/`txt`** (MM-P8 modernization) so reload paths can't
  reintroduce this class of leak.

---

## Summary

**Corrected impact:** The dominant in-session leak for an idle/solo player is
**MM-P9.3 (portrait reload surface leak)** — ~37 KB of `SURF_SYSMEM16` per
portrait message the host pushes, accumulating in `surflist[]`. This is the
fix expected to flatten the idle climb and remove the surface-churn sluggishness.
**MM-P9.1 (chat history)** and **MM-P9.2 (player name list)** are real but
**bounded** for an idle solo player; they are fixed as housekeeping (freed on
client teardown) and matter mainly for very long sessions / repeated reconnects.

**Risk:** Low. MM-P9.3 is behavior-preserving (identical pixels; reused
scratch surface is fully overwritten before use; no `->graphic` dangles). The
MM-P9.1/9.2 cleanups run only at single-threaded shutdown.

**Status Code:** MM-P9 — MM-P9.1, MM-P9.2, MM-P9.3 all implemented; `client` /
`host` / `both` build clean; pending an interactive 10-minute profile re-test.


