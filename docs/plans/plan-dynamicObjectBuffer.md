# Dynamic Per-Player Object Buffer Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before starting the next phase.

> Source of truth: this file. Reference phase IDs in PR titles/descriptions
> (e.g. "DOB-P3.2: thread viewport dims through host send-objects loop").
>
> When resuming work in a new session/context window: read **§ Session
> Handoff** at the bottom first, then jump to the lowest-numbered phase
> that is not ✅. Every phase has a self-contained "What to read first"
> list so context can be rebuilt cheaply.

---

## Goal

Remove the fixed `96 × 72` ceiling on the per-player object-scene buffer
(`sobj_buf2bytes`, `sobj_bufsize`, `sobj`, `sobj_tempfixed` in
`struct player`) so that `viewTilesX()` / `viewTilesY()` can grow to fill
arbitrarily large back buffers — up to `kBackbufferMaxW / 32` and
`kBackbufferMaxH / 32` — without the world-render loops walking off the
end of the buffer (the C0000005 read at `loop_client.cpp:6825`,
documented in RW-P4.11).

After this work the cap `kViewportTilesXMax = 63` /
`kViewportTilesYMax = 47` (added 2026-05-26 in `viewport.h`) can be
removed or raised to the back-buffer-derived maximum (currently
120 × 75 at the 3840 × 2400 cap).

## Non-goals

- Not changing the tile size (still 32 px).
- Not changing the host's authoritative world model — only the
  per-player object **transmit/render** buffer is affected.
- Not introducing a new wire-protocol version for unrelated reasons;
  if a bump is needed it carries only this change.
- Not removing the dead `smallwindow` / `windowsizecyclenum` branches
  (separate RW cleanup sweep).

## Background — why this matters

The world-render visibility loop in `loop_client.cpp:6762..6850` iterates
`x ∈ [0..viewTilesX()+1]`, `y ∈ [0..viewTilesY()+1]`, computes

```cpp
bufx = tpx + x - 1 - tplayer->sobj_bufoffx;
bufy = tpy + y - 1 - tplayer->sobj_bufoffy;
```

and reads `tplayer->sobj_bufsize[bufx][bufy]`. The host keeps
`sobj_bufoffx == tpx - 32` and `sobj_bufoffy == tpy - 24` (i.e. player at
buffer slot (32, 24)), so the safe range is

```
viewTilesX() ≤ 96 - 32 - 1 = 63
viewTilesY() ≤ 72 - 24 - 1 = 47
```

Raising `kBackbufferMaxW/H` to 3840 × 2400 lets `viewTilesX/Y` reach
120 × 75 — well past those bounds — which is exactly how the
intermittent walking-around crash showed up.

A full audit of the literal `96` / `72` / `32` / `24` references that
this plan must touch lives in `docs/resizable-window-hotspots.md`
(re-grep before each phase; the file is the ground truth).

---

## DOB-P0 — Safety net & instrumentation (do FIRST, no behavior changes)

- ⬜ **DOB-P0.1** Grep-audit and table every literal `96`, `72`, `32`,
  `24` in:
  - `src/common/data_both.h`
  - `src/common/data_host.h`
  - `src/common/globals.inc`
  - `src/client/loop_client.cpp`
  - `src/client/loop_client.inc`
  - `src/client/function_client.cpp`
  - `src/client/function_client.h`
  - `src/server/loop_host.cpp`
  - `src/server/function_host.cpp` (if it exists at that point)
  Classify each as: (a) buffer-dim related, (b) viewport-tile related,
  (c) unrelated. Output: append a section "Dynamic Object Buffer
  hotspots" to `docs/resizable-window-hotspots.md`. _(target: ~40-60
  hits in (a)+(b).)_

- ⬜ **DOB-P0.2** Add a debug-only sanity-check macro
  `DOB_BUF_AT(plr, bx, by)` that wraps every `sobj_bufsize[bx][by]` /
  `sobj_buf2bytes[bx][by]` / `sobj[bx][by]` / `sobj_tempfixed[bx][by]`
  access and asserts `0 ≤ bx < W && 0 ≤ by < H` in Debug. Convert all
  current accesses mechanically. Build, smoke-test, no behavior change.
  _(Goal: turn every future out-of-bounds bug into an assert instead of
  a heap walk.)_

- ⬜ **DOB-P0.3** Tag the current behavior with a baseline: take a 30-min
  walking-around session at the **legacy 1024 × 768** window size and
  record (in `docs/dob-baseline.md`) framerate, memory footprint
  (`GetProcessMemoryInfo`), and any logged DOB_BUF_AT asserts. This is
  the regression baseline P4 must match.

**Exit criteria**: hotspot table committed; every fixed-buffer access in
both `u6oclient2` and `u6oh` builds goes through `DOB_BUF_AT`; baseline
recorded; full client+host build clean.

---

## DOB-P1 — Introduce dynamic dims, keep storage static

The cheapest first move: replace literal `96` / `72` with named constants
and runtime variables, but keep the storage size unchanged. This lets
us thread the dims through every API surface without changing layout
yet, and de-risks the actual storage refactor in P2.

- ⬜ **DOB-P1.1** Add to `src/common/data_both.h` (or a new
  `src/common/object_buffer_dims.h`):
  ```cpp
  // Hard cap = current static array dim. P2 will lift these.
  enum : int {
      kSobjBufWMax = 96,
      kSobjBufHMax = 72
  };
  // Active per-player dims (set by host, mirrored to client).
  // P1 invariant: equals kSobjBufWMax / kSobjBufHMax for every player.
  ```
  And to `struct player`:
  ```cpp
  unsigned short sobj_bufW;  // active width  (P1: always 96)
  unsigned short sobj_bufH;  // active height (P1: always 72)
  ```
  Default-initialize both to 96 / 72 wherever a player is constructed
  or zero-init'd (`loop_host.cpp` player-create paths, `data_client.cpp`
  `tplayer` setup, etc.).

- ⬜ **DOB-P1.2** Update `DOB_BUF_AT` to assert against
  `plr->sobj_bufW` / `plr->sobj_bufH` instead of literal constants.

- ⬜ **DOB-P1.3** Convert every literal-`96` / literal-`72` usage from
  the P0.1 audit to `tplayer->sobj_bufW` / `sobj_bufH` (client) and
  `playerlist[i]->sobj_bufW/H` (host). Keep the player-center constants
  (`32`, `24`) as literals for now — they move in P3.

- ⬜ **DOB-P1.4** Update the wire serializer for `struct player`:
  - If `player` is byte-blitted between host and client, just bumping
    the struct size requires both ends to be rebuilt together. Document
    this in the changelog as a hard requirement (no mixed-version
    interop in this phase).
  - If there's a per-field protocol, add `sobj_bufW/H` to the wire
    format with a version byte bump.
  Locate the relevant code via:
  ```text
  grep -rn "sizeof(player)\|memcpy.*player\|player_serialize" src/
  ```

**Exit criteria**: all dim literals replaced with `sobj_bufW/H`; client
and host rebuild and connect; legacy 1024 × 768 session indistinguishable
from baseline (DOB-P0.3); no new DOB_BUF_AT asserts during a walk-around
test.

---

## DOB-P2 — Move storage off `struct player`

The four arrays are huge (~ 96 × 72 × (2 + 2 + 4 + 4) = ~ 80 KB per
player, fixed) and embedded in the wire/save shape. Moving them out
both removes the layout-stability constraint **and** lets each player
have a different size.

- ⬜ **DOB-P2.1** Define a `SobjBuffers` POD that owns the four arrays
  as `Dynamic2DArray<…>` (template already exists, used by `vis`/`ls`):
  ```cpp
  struct SobjBuffers {
      int W = 0, H = 0;
      Dynamic2DArray<unsigned short>   buf2bytes;
      Dynamic2DArray<unsigned short>   bufsize;
      Dynamic2DArray<unsigned short*>  data;        // was sobj[][]
      Dynamic2DArray<unsigned long>    tempfixed;
      bool alloc(int w, int h);   // (re)allocate to w×h, zero-init
      void free();
  };
  ```

- ⬜ **DOB-P2.2** Replace the four array members on `struct player` with
  a single `SobjBuffers sobjbuf;` (and drop the now-redundant
  `sobj_bufW` / `sobj_bufH` from P1; live on `SobjBuffers::W/H`).
  Update every accessor (mechanical from the P1 step — all reads
  already go through `DOB_BUF_AT`, now point at
  `plr->sobjbuf.bufsize(bx, by)`).

- ⬜ **DOB-P2.3** Wire-protocol changes:
  - **Host → client world stream**: the per-player object buffer is
    sent (or implicitly memcpy'd) from host to client every frame.
    Introduce a per-player handshake: when the client connects (or
    when the client's `viewTilesX/Y` changes), send `RESIZE_SOBJ_BUF
    {W, H}` to the host; the host calls `sobjbuf.alloc(W, H)` for that
    player and from then on streams the new geometry.
  - **Save game**: if `player` is persisted with embedded sobj buffers,
    write a converter (`SobjBuffers` are runtime caches — easier to
    just *not* persist them and rebuild from world state on load).
    Confirm via the host save/load code (`loop_host.cpp` — search
    `fwrite.*player` and `fread.*player`).

- ⬜ **DOB-P2.4** Add per-player allocation lifetime hooks:
  - Player connect / spawn → `sobjbuf.alloc(default_W, default_H)`.
  - Player disconnect / despawn → `sobjbuf.free()`.
  - Resize request from client → `sobjbuf.alloc(newW, newH)` followed
    by full re-population (treat as "first frame" so `sobj_bufoffx/y`
    gets re-centered, see P3).

**Exit criteria**: `struct player` no longer contains the four arrays;
sizeof(player) shrinks by ~ 80 KB; client and host re-allocate on
connect; legacy session at 1024 × 768 still binary-identical screen
output to baseline (DOB-P0.3) within tile-render tolerance; no new
DOB_BUF_AT asserts in a 30-min walking-around test at legacy size.

---

## DOB-P3 — Player-centered offset arithmetic

Today both host and client hard-code "player lives at buffer slot
(32, 24)" via `sobj_bufoffx = tpx - 32; sobj_bufoffy = tpy - 24;`
(`loop_client.inc:3950`, `loop_host.cpp:1746` etc.). With variable
buffer sizes the recenter must use `W/2, H/2`.

- ⬜ **DOB-P3.1** Add helpers in `src/common/object_buffer_dims.h`:
  ```cpp
  inline int sobj_center_x(const player* p) { return p->sobjbuf.W / 2; }
  inline int sobj_center_y(const player* p) { return p->sobjbuf.H / 2; }
  inline short sobj_recenter_offx(const player* p, int tpx) {
      return (short)(tpx - sobj_center_x(p));
  }
  inline short sobj_recenter_offy(const player* p, int tpy) {
      return (short)(tpy - sobj_center_y(p));
  }
  ```

- ⬜ **DOB-P3.2** Replace every literal `tpx - 32` / `tpy - 24` used to
  set `sobj_bufoffx/y` with these helpers. Same for the "current buffer
  extents" check at `loop_client.inc:3937` / `loop_host.cpp:1885`:
  ```cpp
  x6 = x5 + p->sobjbuf.W - 1;
  y6 = y5 + p->sobjbuf.H - 1;
  ```

- ⬜ **DOB-P3.3** Audit `+1` / `+2` fencepost offsets in the render
  loops (visibility 6745-6850, x-ray 6859-6890, edges 6892-6920, object
  buffer 7007-7300, plus the `_apply.cpp` mirror loops). Confirm each
  one's max bufx/bufy is still strictly `< W` / `< H` for the active
  per-player buffer size. Add `static_assert`s where the relationship
  is compile-time, asserts where runtime.

**Exit criteria**: re-center math is symmetric (player always at
buffer-center, not slot 32/24); a 1024 × 768 client connected to a host
running with `kSobjBufW/H = 96/72` defaults shows identical scene to
DOB-P0.3 baseline; no DOB_BUF_AT asserts in any window size up to
`kBackbufferMaxW/H`.

---

## DOB-P4 — Lift the viewport cap

- ⬜ **DOB-P4.1** Compute the maximum supported per-player buffer dims
  from `kBackbufferMaxW/H` plus a safety margin (e.g. `+4` tiles each
  side for the `+1/+2` fenceposts plus a 2-tile look-ahead margin):
  ```cpp
  constexpr int kSobjBufWAbsMax = (kBackbufferMaxW / 32) + 8; // 128
  constexpr int kSobjBufHAbsMax = (kBackbufferMaxH / 32) + 8; // 83
  ```

- ⬜ **DOB-P4.2** On every window resize (the existing `dirtyClientSize`
  handler in `ui_panels_apply.cpp`), if the new
  `viewTilesX()/viewTilesY()` exceeds the player's current
  `sobjbuf.W/H`, send the `RESIZE_SOBJ_BUF` request to the host. Cap
  to `kSobjBufW/HAbsMax`.

- ⬜ **DOB-P4.3** Remove (or raise) `kViewportTilesXMax = 63` /
  `kViewportTilesYMax = 47` in `viewport.h`. Drop the clamp inside
  `viewTilesX()` / `viewTilesY()` in `viewport.cpp`. Leave the named
  constants in place but set them to `kSobjBufW/HAbsMax - 4` (margin
  for the fencepost loops) so a regression in P3 still trips the cap
  rather than crashing.

- ⬜ **DOB-P4.4** Regression sweep:
  - 30-min walking-around session at 1024 × 768 (must match baseline).
  - Same at 1920 × 1080 (mid-cap).
  - Same at 3840 × 2400 (full cap) — this is the configuration that
    triggered the original crash.
  - Confirm no DOB_BUF_AT asserts and no `c0000005` crashes.
  - Memory footprint per player at 3840 × 2400: target ≤ 400 KB
    (vs. ~ 80 KB at 96 × 72). Document actual numbers.

**Exit criteria**: viewport cap removed; user can drag the window to any
size up to `kBackbufferMaxW/H` and walk around indefinitely without
crash; legacy size produces baseline-identical output.

---

## DOB-P5 — Cleanup & follow-ups

- ⬜ **DOB-P5.1** Remove `DOB_BUF_AT` debug asserts (or leave compiled
  in for Debug builds only, gated on `U6O_DEBUG`).
- ⬜ **DOB-P5.2** Delete the obsolete RW-P4.11 cap comment / constants
  in `viewport.h`. Update `docs/plan-resizableWindow.md` to mark
  RW-P4.11 as superseded by this plan.
- ⬜ **DOB-P5.3** Update `docs/resizable-window-hotspots.md` to mark
  the resolved hotspots and add a "post-DOB" status column.
- ⬜ **DOB-P5.4** Doc pass: this file's "Goal" section becomes the
  retrospective; promote § Session Handoff to a "Lessons" section.

---

## Risk register

| # | Risk | Mitigation |
|---|------|------------|
| R1 | Wire-protocol change desyncs old clients/hosts. | DOB-P2.3 introduces explicit `RESIZE_SOBJ_BUF` handshake; bump protocol version byte at the same commit. Ship server + client together. |
| R2 | Save games embed `sobj_*` arrays directly. | DOB-P2.3 confirms via grep; if so, write a converter that drops the arrays (they're caches). |
| R3 | Off-by-one in the recenter math hides objects near the buffer edge. | DOB-P3.3 fence-post audit + DOB_BUF_AT asserts catch this in Debug; DOB-P4.4 regression sweep at three window sizes catches visual regressions. |
| R4 | Per-player ~400 KB at full cap × 100 concurrent players = 40 MB extra on host. | Acceptable on any modern server box; document in P4.4 results. If tight, gate per-player max at smaller dim until needed. |
| R5 | `Dynamic2DArray` was designed for client-only use; host build (`u6oh`) may not include `viewport.cpp`. | Move the template into `src/common/dynamic2darray.h` if it isn't already common. Verify in P2.1. |
| R6 | Two long-lived branches (this work + ongoing RW-P5+ work) drift the literal-`96/72` audit. | DOB-P0.1 re-runs as a pre-merge gate; PRs touching the four arrays must reference a DOB-P phase ID. |

---

## File touch-map (initial estimate, refine in DOB-P0.1)

| File | Phase touched | Notes |
|------|--------------|-------|
| `src/common/data_both.h` | P1, P2 | `struct player` layout change |
| `src/common/object_buffer_dims.h` *(new)* | P1, P3 | constants + helpers |
| `src/common/dynamic2darray.h` *(may be new — verify)* | P2 | promote from client-only |
| `src/client/loop_client.cpp` | P1, P2, P3, P4 | render & vis loops |
| `src/client/loop_client.inc` | P1, P2, P3 | recenter loop, fixedobj loop |
| `src/client/function_client.cpp` | P2, P4 | tplayer init, resize hook |
| `src/client/viewport.h` / `.cpp` | P4 | cap removal |
| `src/client/ui_panels_apply.cpp` | P4 | window-resize → buffer-resize |
| `src/server/loop_host.cpp` | P1, P2, P3 | send-objects loop, ZeroMemory sites |
| `src/server/function_host.cpp` | P2 | per-player alloc/free hooks |
| Save/load code (TBD via grep) | P2.3 | drop or version sobj_* arrays |
| Network handshake code (TBD via grep) | P2.3 | `RESIZE_SOBJ_BUF` opcode |

---

## Session handoff

> **READ THIS FIRST when picking up the plan in a new session.**

State of the plan (update at the end of every session, with date & who):

- **2026-05-26 (initial draft)** — Plan committed. No code changes yet.
  Phase 1 = the cap added in RW-P4.11 (`kViewportTilesXMax = 63`,
  `kViewportTilesYMax = 47`) is in place and protects the crash; this
  plan is the path to remove that cap properly. Start at **DOB-P0.1**.

To pick up cleanly:

1. `git log --grep "DOB-P"` to see what's landed.
2. Scan this file top-to-bottom for the first non-✅ checkbox; that's
   the resume point.
3. Re-read the **What to read first** list under that phase's section
   (TODO: each phase should add one as it begins).
4. Re-run `grep -rn "sobj_buf\|sobj_tempfixed" src/` and diff against
   the P0.1 audit if it exists — drift means another change touched
   these arrays since last session.
5. If a phase's exit criteria say "run the walking-around regression",
   the steps are recorded in `docs/dob-baseline.md` (created in
   DOB-P0.3). Repeat them at every phase exit, not just P4.4.

### Open questions to resolve before P2

- ⬜ Is `struct player` byte-blitted on the wire, or per-field
  serialized? Determines whether P1.4 needs a protocol bump.
- ⬜ Is `sobj_*` persisted in save games? Determines whether P2.3
  needs a save converter.
- ⬜ Does `Dynamic2DArray<T>` already compile in the host build, or is
  it currently `#ifdef CLIENT`-gated?
- ⬜ What's the actual concurrent-player ceiling on the host? Drives
  the R4 memory budget.

Answer these as part of DOB-P0.1 and record the answers here so the
next session doesn't re-derive them.

