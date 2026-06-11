# Global Room Sync (ROOMSYNC-P1 / P1.1 / P1.2)

**Status:** Shipped, June 2026 (`feature/global-rendering`).
**Replaces:** Per-room patches in `getscreenoffset()` / `getscreenoffset_legacy()`, the
`gg_basement_room` flag in `loop_host.cpp`, the basement coordinate triple
in `loop_client.cpp`, and the "remember to call `tplayer->resync = 1`"
ritual in the [historical map-rendering README](../map_rendering/README.md).

## Problem statement

For years, every time we added an isolated room at a custom world coordinate
(Guardian Guild basement at `(1280..1291, 319..333)`, Toth's house at
`(1327..1358, 319..343)`, planned guild basements, dungeon test rooms, etc.)
the same three-symptom bug appeared:

1. **Player gets stuck.** Keypresses fire `KEYup/down/left/right`, the host
   sees the input, the camera scrolls — but the avatar sprite never moves.
2. **Player and party members disappear.** The avatar vanishes from the
   client even though the server still tracks it (the player can be heard
   by others in chat and still appears in `WHO`).
3. **Random NPCs / items pop in and out** of the room as the player walks
   around — sometimes a guard from outside the wall walks through it, then
   vanishes the next frame.

Every prior fix was per-room: a new `if (x >= ... && y >= ...)` block in
`getscreenoffset()`, a new branch in the host fill loop, a new camera
override in two client sites, plus a developer-discipline note that "any
ladder / moongate handler should call `tplayer->resync = 1` after
`partyadd()`". The discipline note was almost never followed.

This document describes the global fix that retires that pattern.

## Root cause (three concurrent bugs in one apparent symptom)

### Cause 1: cross-region stream bleed

The host fills per-player **mover** and **sobj** (scene-object) transmit
windows by walking a rectangle centered on the avatar:

- mover window: `MV_TX_W * MV_TX_H = 65 * 49` tiles (`define_both.h`).
- sobj window:  `SOBJ_TX_W * SOBJ_TX_H = 79 * 63` tiles.

When the player is at, say, `(1285, 326)` in the Guardian Guild basement,
the mover window spans world tiles `x = 1254 .. 1318`, which crosses the
`x = 1280` boundary back into the gargoyle lands. NPCs, ground items, and
doors that live on the gargoyle-side tiles get streamed into the basement
view. Two visible effects:

- A wandering gargoyle at `(1276, 326)` is rendered as if it were inside
  the basement wall, drifts a few frames, then vanishes when its real
  schedule wakes up and walks it east.
- The client decoder hands those foreign movers IDs in `mv_x[]` /
  `mv_y[]` — they consume real index slots, every slot below them shifts
  on prune, and the avatar's slot can get reassigned.

### Cause 2: index reassignment on silent prune

When the player teleports (ladder, moongate, red gate, `partyadd()`) the
host's per-player buffer still contains the pre-teleport movers. The host
then runs an "offscreen prune" pass that walks `tplayer->mv_x[i]` and
shuffles every entry outside the new window down by one. **The May 2026
fix** (`sceneupdaterequired = 1` on offscreen prune) ensures the host at
least *sends* a scene update after that prune, so the client's mirror
prune runs too. But the shuffled `mv_x[]` is still walked by the subsequent
remove/move/add diff encoder, which compares index 0 to index 0, index 1
to index 1, etc. If index 0 used to be the avatar and index 0 is now an
NPC that survived the prune, the avatar's mover state (position,
direction, frame, playerid, hp/mp) is written into the NPC's slot. The
avatar's sprite stops being drawn (no slot has playerid == its id) while
the camera, which reads `tplayer->x/y` directly from the **"x,y change"
field at the top of the scene-update packet**, keeps tracking the
phantom position the avatar should be at.

This is exactly the user-visible "I can move the camera but my sprite is
stuck and then disappears" symptom.

### Cause 3: client camera frame ≠ host wire frame

`getscreenoffset()` returns `tpx, tpy` clamped to the bounds of three
hardcoded regions: the main overworld (`x,y <= 1023`), the gargoyle lands
(`x in 1024..1279, y in 256..511`), and Toth's house. Everything else
falls through to a generic "clamp to world edge" branch. For an isolated
room at `(1280..1291)` — one tile east of the gargoyle clamp — the
camera anchor *jumps* by up to 17 columns when the player steps from
`x = 1279` into `x = 1280`. The mover/sobj diff encoders run with the
new anchor, but the client's `tplayer->sobj_bufoffx` and `mv_x[]` are
still anchored to the pre-jump value for one frame. That's another
opportunity for the diff encoder to overwrite the wrong slot.

The historical workaround was a per-room `if` block in `loop_client.cpp`
that bypassed `getscreenoffset()` and re-centered the camera; but it
existed only for the Guardian Guild basement, and was missed for every
other custom region.

## The fix: one registry, one auto-resync rule

### Registry (data-driven)

[`src/common/function_both.h`](../../src/common/function_both.h) /
[`src/common/function_both.cpp`](../../src/common/function_both.cpp)
expose:

```cpp
struct GameRoom { long x0, y0; long x1, y1; };
int  getroom (long x, long y, long *rx0, long *ry0, long *rx1, long *ry1);
int  sameroom(long ax, long ay, long bx, long by);
```

The single static table `gameRooms[]` in `function_both.cpp` lists every
isolated-room rectangle in the world. Adding a new basement / hidden
chamber / instance is **a one-line edit to this table**:

```cpp
static const GameRoom gameRooms[] = {
    { 1280, 319, 1291, 333 },   // Guardian Guild basement
    // { 1600, 256, 1791, 511 }, // <-- example: a new guild basement
};
```

Everything below queries this table; nothing else has to change.

### Host-side use ([`src/server/loop_host.cpp`](../../src/server/loop_host.cpp))

In the scene-update setup, after `getscreenoffset(x, y, &tpx, &tpy)`:

```cpp
playerroom_inroom = (unsigned char)getroom(x, y,
    &playerroom_x0, &playerroom_y0,
    &playerroom_x1, &playerroom_y1);
```

The sobj fill loop and the mover fill loop both consult this cached
flag + bounds before each tile is processed:

```cpp
if (playerroom_inroom) {
    if ((mapx < playerroom_x0) || (mapx > playerroom_x1) ||
        (mapy < playerroom_y0) || (mapy > playerroom_y1))
        goto objbuffer_outofrange;   // or moverbuffer_outofrange
}
```

This eliminates **cross-region stream bleed** (Cause 1) for *every*
registered room with no per-room code path.

### Auto-resync trigger ([`src/server/loop_host.cpp`](../../src/server/loop_host.cpp))

At the top of every scene-update build, before the existing
`if (tplayer->resync)` flush handler, the host now runs:

```cpp
if (myobj = tplayer->party[tplayer->selected_partymember]) {
    roomsync_targetx = myobj->x;
    roomsync_targety = myobj->y;
    if (tplayer->wizardeyetimeleft) {
        roomsync_targetx = tplayer->wizardeyex;
        roomsync_targety = tplayer->wizardeyey;
    }
} else {
    roomsync_targetx = tplayer->px;
    roomsync_targety = tplayer->py;
}
if (tplayer->x || tplayer->y) {
    roomsync_dx = roomsync_targetx - (long)tplayer->x;
    roomsync_dy = roomsync_targety - (long)tplayer->y;
    if (roomsync_dx < -3 || roomsync_dx > 3 ||
        roomsync_dy < -3 || roomsync_dy > 3) {
        tplayer->resync = 1;   // teleport (jump > 3 tiles)
    } else if (!sameroom(roomsync_targetx, roomsync_targety,
                         tplayer->x, tplayer->y)) {
        tplayer->resync = 1;   // crossed a registered room boundary
    }
}
```

That's the entire fix for **Cause 2**:

- Any teleport that moves the avatar more than 3 tiles in a single frame
  (ladder, moongate, red gate, `partyadd`, `OBJmove2`, death/respawn,
  arena exit, …) auto-flushes both buffers and triggers a packet-35
  (resync) reply. The diff encoder then sees `tplayer->mv_i == 0` and
  cleanly re-`ADD`s every mover from scratch — no off-by-N reshuffles,
  no slot reassignment.
- The same flush fires when the player walks one tile across a
  registered room boundary (e.g. from `x = 1280` to `x = 1279` exiting
  the basement, or any future door-tile that bridges room↔world). This
  guarantees the host-side fill window switches from "room-bounded" to
  "open world" with a clean slate.

**No teleport-site code change is required.** Every existing call site
(`partyadd()`, the ladder branch in `use_*`, every `OBJmove2()` that
moves the avatar) now triggers resync automatically the next frame.

The threshold of 3 tiles matches `mover_offseti[][]`, the host's 5-bit
move-delta encoder that can express `dx, dy in [-3 .. +3]`. Any larger
jump is necessarily encoded as remove+add anyway; setting `resync = 1`
just upgrades the partial-rebuild to a full one.

### P1.1 follow-up: first-scene and camera-anchor guards (June 2026)

The initial ROOMSYNC-P1 commit left two edges uncovered, and both
reproduced as the user-reported "first visit to the shop after a cold
server start triggers the bug, subsequent visits are fine" symptom:

1. **First scene update for any player.** A freshly malloc'd /
   `ZeroMemory`'d player struct (the `NETconnect` path around
   `loop_host.cpp:4577`) starts with `tplayer->x == tplayer->y == 0`.
   The original P1 trigger guarded all of its delta / room / camera
   checks with `if (tplayer->x || tplayer->y)`, so the very first
   emit fell through to the normal diff path with an empty mover
   buffer and was forced to ADD every visible mover from an empty
   `tplayer->mv_x[]` in a single packet. On dense spawn areas — the
   shop at `(x=1280..1341, y=376..432)`, which sits right against
   the `x=1280` gargoyle-land clamp boundary, is the canary — that
   single packet was observed to leave slot 0 bound to the wrong
   mover after a cold start, exactly reproducing the avatar-vanish
   bug. Once the player had moved at all, every subsequent visit's
   transition was caught by one of the other triggers and worked
   correctly. The P1.1 fix forces the first scene emit to be a
   packet-35 (explicit flush + rebuild) so both ends start from an
   unambiguous zero state:

   ```cpp
   // Before the existing if (tplayer->x || tplayer->y) block:
   if (!tplayer->x && !tplayer->y) {
       tplayer->resync = 1;
   }
   ```

2. **Camera-anchor jump on a 1-tile walk across a region clamp.** The
   hardcoded `getscreenoffset()` clamp for gargoyle land ends at
   `x = 1279`; the very next tile `x = 1280` falls into the default
   fall-through branch which has no clamp at all. A player who walks
   that single tile causes the host's camera anchor `tpx` to shift by
   up to ~17 columns in one frame. The host's mover/sobj transmit
   window then shifts the same amount, the per-player 96x72 sobj
   buffer has to relocate, and the diff encoder is forced to emit a
   large batch of remove+add operations — same failure mode as a
   true teleport, but with the player coordinate delta only being 1
   so neither the `dx>3` nor the `sameroom()` triggers fire. P1.1
   adds an explicit camera-jump check inside the same trigger block:

   ```cpp
   // Inside the if (tplayer->x || tplayer->y) block, AFTER the
   // delta/room checks. Only run if neither of them already fired.
   } else {
       static long ar_oldtpx, ar_oldtpy;
       static long ar_newtpx, ar_newtpy;
       static long ar_dtpx, ar_dtpy;
       getscreenoffset((long)tplayer->x, (long)tplayer->y,
                       &ar_oldtpx, &ar_oldtpy);
       getscreenoffset(roomsync_targetx, roomsync_targety,
                       &ar_newtpx, &ar_newtpy);
       ar_dtpx = ar_newtpx - ar_oldtpx;
       ar_dtpy = ar_newtpy - ar_oldtpy;
       if (ar_dtpx < -3 || ar_dtpx > 3 ||
           ar_dtpy < -3 || ar_dtpy > 3) {
           tplayer->resync = 1;
       }
   }
   ```

   This also covers the symmetric case of walking *out* of a clamped
   region into the default zone, the gargoyle `y` edges at `y=256`
   and `y=512`, the overworld edge at `x/y=1023`, and any future
   region boundary added to `getscreenoffset()`.

Both P1.1 guards are tagged `ROOMSYNC-P1.1:` in `loop_host.cpp`.

### P1.2 follow-up: periodic safety-resync heartbeat (June 2026)

P1 and P1.1 together cover every observable *transition* — teleport,
room boundary, sub-map / region-clamp camera jump, first scene
update. But they're all **event-based**: the host only re-evaluates
the resync decision when the player's avatar coordinate changes.

A player who sits completely still in a busy area (the original "shop
reproducer": "after sitting still in the shop for a while I eventually
lost control of the player") can still experience slow drift in their
per-player `tplayer->mv_x[]` slot order over time. Plausible drift
sources, none of which surface as a player coord delta:

- NPC scheduled movement reshuffling slot indices as nearby merchants
  walk in and out of the mover transmit window.
- Object spawn / despawn (timed candles burning out, dropped items
  decaying, monster respawn timers) inside the window.
- Mover object pointer reuse after `OBJrelease` / `OBJnew` -- the
  diff encoder's `mv_object[i] == mv_object[i2]` identity check is
  pointer-equality, so a freed-and-reallocated pointer can match the
  *wrong* identity if it happens to be the same address.
- Other-player movement through the player's window.

P1.2 puts a **bounded self-heal ceiling** on any such drift: a
host-only `resync_timer` on each `player` struct accumulates `et`
each scene-update tick, and once it crosses
`ROOMSYNC_HEARTBEAT_SECONDS` (defined in `define_both.h`, currently
`60.0f`) the host force-sets `resync = 1` and zeroes the timer. The
existing resync handler then flushes both ends and rebuilds cleanly.

```cpp
// Added at the top of every scene-update build, BEFORE the existing
// P1 / P1.1 event-based triggers, so heartbeat counts toward the
// trigger evaluation.
if (tplayer->x || tplayer->y) {
    tplayer->resync_timer += et;
    if (tplayer->resync_timer >= ROOMSYNC_HEARTBEAT_SECONDS) {
        tplayer->resync = 1;
        tplayer->resync_timer = 0.0f;
    }
} else {
    tplayer->resync_timer = 0.0f;
}
```

Design choices:

- **60 seconds.** Short enough that the user notices at most one
  minute of degraded behaviour before recovery, long enough that the
  resync packet (a flush + full mover/sobj add list — typically a few
  KB on a dense area like the shop) is amortised cleanly over the
  per-player bandwidth budget. Tunable via the
  `ROOMSYNC_HEARTBEAT_SECONDS` macro without any code change.

- **`resync_timer` is host-only state.** It lives on the `player`
  struct (so it's allocated and zeroed by the existing
  `malloc` + `ZeroMemory` `NETconnect` path) but the client never
  reads or writes it. The struct layout change is therefore not a
  wire-protocol change. `U6O_VERSION` is intentionally NOT bumped.

- **Skip while `tplayer->x == 0 && tplayer->y == 0`.** That's the
  P1.1 first-scene state; the first-scene rule already fires a
  resync there and we don't want to double-trigger. Zeroing the
  timer in this branch also means the heartbeat clock starts from
  "first frame with a real position", not from `NETconnect`.

- **Phase-staggered naturally.** Different players connect at
  different times, so their heartbeat phases are inherently
  decorrelated -- no thundering herd of simultaneous resyncs across
  the player population.

Tagged `ROOMSYNC-P1.2:` in `src/common/data_both.h` (field),
`src/common/define_both.h` (threshold), and `src/server/loop_host.cpp`
(trigger).

### Client-side use ([`src/client/loop_client.cpp`](../../src/client/loop_client.cpp))

The two camera-follow override sites (scene-update at packet-31 / packet-35
intake, and per-frame render right before the world tile draw) now share
the same predicate:

```cpp
getscreenoffset(x, y, &tpx, &tpy);
if (getroom(x, y, NULL, NULL, NULL, NULL)) {
    tpx = x - (viewTilesX()/2 - 1);
    tpy = y - (viewTilesY()/2 - 1);
}
```

`tpx_legacy / tpy_legacy` (used for wire decode of mover/sobj offsets)
stays in the legacy 32x24 reference frame — that's what the host's
`getscreenoffset()` returns since the host build of `viewTilesX/Y` is
locked at 32/24. Only the dynamic render camera follows the room, which
keeps **input ↔ render** in lock-step (Cause 3 fixed).

Adding a new room to `gameRooms[]` *automatically* enables both:
- centered follow on the client, and
- room-bounded streaming on the host,

without touching `loop_client.cpp` or `loop_host.cpp`.

## What did NOT change (and why)

- **Wire format.** No bit width, window dimension, or message opcode was
  modified. `U6O_VERSION` is intentionally **not** bumped — old clients
  and new hosts (and vice versa) wire-compat correctly. The only on-the-wire
  difference is "the host now sends *more* packet-35 resyncs in
  situations where it previously sent partial diff packets that the
  client misdecoded"; both ends were already required to support packet
  35 since the `/RESYNC` chat command exists.
- **The hardcoded regions in `getscreenoffset()` (overworld /
  gargoyle / Toth).** These are wire-coupled with the host's
  `tpx`/`tpy` emit reference frame; rewriting them would change every
  sobj/mover decode offset and break compatibility. The registry layers
  *on top of* the existing function instead of replacing it.
- **The 96 × 72 per-player sobj buffer** (`sobj_bufsize[][]` etc.).
  Resizing it is `DOB-P2+` work and orthogonal to this fix.

## Verifying the fix

### Build

```powershell
# In the VS x86 developer prompt:
cd cmake-build-debug-visual-studio
cmake --build . --target host
cmake --build . --target client
cmake --build . --target both
```

All three should compile clean (one pre-existing `WinMain` inline-asm
warning unrelated to this work).

### Behavioural regression tests

| Test | Expected after fix |
|---|---|
| Use Guardian Guild ladder; arrive at basement `(1286, 323)` | Avatar visible at centre, walks immediately. No gargoyles inside walls. |
| Use a moongate from overworld to anywhere | First frame after arrival shows all party members and *only* destination NPCs. |
| Red gate from `(0, 0)` overworld to a basement | No "stuck for one tick" symptom. |
| Spam `/RESYNC` | Identical behaviour to pre-fix (resync was always available manually). |
| Walk one tile across a basement door (`(1280, y) ↔ (1279, y)`) | Camera re-anchors; no mover ghosts on either side. |
| Add a NEW basement to `gameRooms[]` (no other code change) | Same correct behaviour at the new coordinates without per-site patches. |
| **Cold server start, log in with saved position inside the shop (`x=1280..1341, y=376..432`)** | **P1.1: First scene update fires as packet-35 resync, avatar is visible & responsive from the first frame.** |
| **Walk from gargoyle land into the shop (cross `x=1279 → x=1280`)** | **P1.1: Camera-jump guard fires resync, mover slot 0 stays bound to the avatar across the transition.** |
| **Sit completely still in the shop for > `ROOMSYNC_HEARTBEAT_SECONDS` (default 60s)** | **P1.2: Heartbeat fires a forced resync; avatar stays controllable indefinitely. The exact symptom "after sitting still in the shop for a while I lost control" no longer reproduces because any accumulated drift self-heals at most once per minute.** |

### Adding a new room

1. Edit `src/common/function_both.cpp`:
   ```cpp
   static const GameRoom gameRooms[] = {
       { 1280, 319, 1291, 333 },   // Guardian Guild basement
       { /*x0*/, /*y0*/, /*x1*/, /*y1*/ },   // your new room (inclusive bounds)
   };
   ```
2. Build. That's it.

Constraints:
- Rooms must not overlap (the table is searched linearly; first hit wins).
- Bounds are inclusive on both axes.
- A room must be at least as wide/tall as `MV_TX_W / MV_TX_H` to *not*
  trigger the streaming filter? **No** — the filter clamps the window
  *into* the room, so any size works. A 1×1 room would only show its
  own tile, which is probably not what you want, but is mechanically fine.
- The room rectangle must contain all walkable area the player can reach
  via that room's entrance. Disconnected sub-areas need their own
  registry entry.

## Files touched

| File | Change |
|---|---|
| `src/common/function_both.h` | `GameRoom`, `getroom()`, `sameroom()` declarations. |
| `src/common/function_both.cpp` | Registry, `getroom()`, `sameroom()` definitions; cleaned stale basement comment in `getscreenoffset()` / `getscreenoffset_legacy()`. |
| `src/common/data_both.h` | `resync_timer` field on `player` (P1.2). |
| `src/common/define_both.h` | `ROOMSYNC_HEARTBEAT_SECONDS` macro (P1.2). |
| `src/server/loop_host.cpp` | (1) Auto-resync trigger at top of scene update (P1) + first-scene and camera-anchor-jump guards (P1.1) + periodic safety-resync heartbeat (P1.2). (2) `gg_basement_room` → cached `playerroom_*` from `getroom()`. (3+4) Sobj + mover fill loops use cached bounds. All edits tagged `ROOMSYNC-P1:` / `ROOMSYNC-P1.1:` / `ROOMSYNC-P1.2:`. |
| `src/client/loop_client.cpp` | Two camera-follow overrides (scene-update intake + per-frame render) call `getroom()` instead of hardcoding the basement bounds. Tagged `ROOMSYNC-P1:`. |

## Relationship to existing work

- **`docs/map_rendering/README.md`** — describes the *symptoms* this fix
  retires. The "Force network resync" / "stabilise viewport" steps in
  that document are now automatic; the per-room checklist is obsolete
  for any room added to `gameRooms[]`.
- **`docs/rendering/basements/`** — historical root-cause analyses from
  prior ad-hoc fixes. Still useful as background. The post-fix
  behaviour follows the same "data-driven, no per-room code" pattern
  those docs were converging on.
- **`docs/plans/plan-resizableWindow.md` (RW-P*)** — orthogonal. Room
  filtering is independent of viewport size; the room rect is clipped
  before mover/sobj data even reaches the encode loop.
- **`docs/plans/plan-dynamicObjectBuffer.md` (DOB-P*)** — orthogonal,
  but synergistic. When DOB-P2+ resizes the per-player sobj buffer, the
  registry will continue to clip rooms to their bounds — the buffer
  will just be bigger.

## Future work (not in scope for ROOMSYNC-P1)

- **Tile rendering void.** Out-of-room base tiles (`bt[][]`) are still
  drawn by the client even when the player is inside a registered room.
  For most rooms this is harmless (the walls obscure them) but at
  sub-map boundaries the foreign tiles can peek through outside the
  walls. A follow-up could check `sameroom(tplayer->x, tplayer->y, mapx,
  mapy)` in the base-tile + objfixed render loops and skip-with-black-fill.
- **objfixed / tfixedobj filtering.** Same idea for static decoration
  arrays. The mover and sobj streams (the dynamic content the user
  actually notices in motion) are already filtered.
- **Multiple-instance rooms.** If we ever support per-party instanced
  basements, the registry needs to be per-player rather than global.

## Test plan (manual)

Until the project grows a head-less integration test harness, verify by
walking the regression-test table above on a local host+client pair.
Watch for:

1. Avatar always visible during and immediately after ladder transitions.
2. Party members all arrive and animate correctly.
3. No NPC, item, or door from outside the basement bounds is ever drawn
   inside the room view.
4. `/RESYNC` is rarely needed anymore — if a user reports they had to
   type it, that's a bug worth filing.

