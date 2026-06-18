# House and Basement Rendering Issues

> **Update (June 2026 — ROOMSYNC-P1):** the per-room workarounds described
> below are **obsolete for any room registered in `gameRooms[]`**. The
> global fix in
> [`docs/rendering/global-room-sync.md`](../rendering/global-room-sync.md)
> handles auto-resync on teleport / room boundary crossings and clips
> the host's mover/sobj transmit windows to the player's room
> automatically. To add a new basement / custom-coordinate room, append
> one line to `gameRooms[]` in `src/common/function_both.cpp` — no
> changes to `getscreenoffset()`, `loop_host.cpp`, `loop_client.cpp`,
> or teleport call sites required. Read this document as historical
> root-cause analysis only.

This document describes how to diagnose and solve issues related to rendering custom house basements or specialized map regions in Ultima VI Online.

## Symptom: The "Disappearing Player" Bug

When a player teleports to a custom map region (like a basement), the following symptoms may occur:
1.  **Immobilization**: The player cannot move after arriving at the destination (e.g., at the bottom of a ladder).
2.  **Disappearance**: The player character sprite disappears from the screen.
3.  **Visual Glitches**: The background map might look incorrect, or the viewport might be stuck in a "black" area or an unrelated part of the world.

## Root Causes

These issues are typically caused by two factors:

### 1. Viewport Undefined State
The engine uses `getscreenoffset` (in `src/common/function_both.cpp`) to determine where the camera should be centered. If the coordinates are outside the hardcoded "known" regions (like the overworld or Gargoyle lands), the viewport falls into an "undefined" state.
In this state, the camera may scroll erratically or jump to `(0,0)` if it hits boundary logic, causing the player to be off-screen.

### 2. Network Desync (Stale Data)
When a player teleports a large distance, the client may still have movers and objects from the previous location in its local buffers. If the host doesn't explicitly force a resync, the client might try to render the player relative to old data, leading to the character disappearing or the client failing to process new movement commands.

### 3. Sub-map Boundary Artifacts
The world map is composed of multiple 256x256 sub-maps. If a custom region is placed too close to a sub-map boundary (e.g., at `x=1280`), a centered camera will attempt to display tiles and movers from the adjacent sub-map (e.g., `x < 1280`). This can lead to "Ghost NPCs" (monsters from the other map appearing in the basement) and visual glitches.

## How to Diagnose
...
2.  **Check Viewport Logic**: Look at `getscreenoffset` in `src/common/function_both.cpp`. Check if your coordinates are covered by an existing `if` block.
3.  **Identify Boundary Proximity**: If your coordinates are close to a multiple of 256 (especially `1024`, `1280`, `1536`, `1792`), you may be hitting boundary artifacts.
4.  **Observe Client Logs**: If the player disappears but can still "walk around" (as seen by other players or in server logs), it's likely a viewport or resync issue.

## How to Solve

### Step 1: Placement and Viewport
For the best results with a tracking camera:
1.  **Avoid Boundaries**: Place custom map regions away from sub-map boundaries (multiples of 256). For example, instead of `x=1280`, use `x=1600`.
2.  **Stabilize the Viewport**: Add a dedicated region check in `getscreenoffset` and `getscreenoffset_legacy` within `src/common/function_both.cpp`.

For a small basement, you can choose between a **fixed** viewport or a **dynamic** tracking viewport.
...
*Example (Guardian's Guild Basement):*
Coordinates were moved to `1600, 319` to `1611, 334`.
We used a dynamic tracking region from `1600, 256` to `1791, 511` (Map 3) to allow smooth movement without boundary artifacts.

### Step 2: Force Network Resync
In the server logic (usually `src/server/loop_host.cpp`), ensure that any teleportation event (using a ladder, moongate, or spell) sets the `resync` flag for the player.

```cpp
tplayer->resync = 1;
```

This flag triggers a Packet Type 35 (Resync), which tells the client to clear all local movers/objects and prepare for a fresh state at the new coordinates.

## Summary Checklist
- [ ] Define the basement region in `src/common/function_both.cpp`.
- [ ] Set `*mapx` and `*mapy` to a fixed value for small basements.
- [ ] Update both `getscreenoffset` and `getscreenoffset_legacy`.
- [ ] Ensure `tplayer->resync = 1` is called after the `partyadd` or teleportation call in `loop_host.cpp`.
