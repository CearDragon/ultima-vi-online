# Resizable Window & Dynamic Viewport Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before starting the next phase.

> Source of truth: this file. Reference phase IDs in PR titles/descriptions
> (e.g. "RW-P2.3: anchor sidebar to client right edge").

---

## Goal

Allow the client window to be **minimized, maximized, and freely resized**
while:

- Keeping the game art at native pixel scale (no blurry upscale).
- Filling the extra space with **more visible game content** (more world
  tiles around the player, UI panels anchored to client edges) instead of
  black letterbox bars.
- Preserving exact behavior at the legacy 1024×768 client size so existing
  saves, screenshots, key bindings, UI mods, and golden tests remain valid.

## Non-goals

- Not changing the underlying tile size (32 px) or art assets.
- Not changing the host/server protocol unless P5 specifically requires it.
- Not adding HiDPI / DPI-aware scaling (separate future effort).
- Not introducing a new graphics backend (DirectDraw stays for now).

## Single-window-mode cleanup (Option A, 2026-05-20)

Before this plan started P2 work, the client supported **four window modes**
that all had to be considered for resize:

1. Main classic 1024×768 (`hWnd2`) — kept.
2. Small classic 512×384 (`hWnd3`) — **removed**.
3. "N1 enhanced" alternate window (`hWnd4`) at ~1200×900 — **removed**.
4. WS_POPUP fullscreen fallback (`hWnd2` alt) — **removed**.

Locking to Mode 1 only halved the surface area of subsequent phases
(one back buffer to make dynamic, one UI layout to retarget). The
`smallwindow` and `windowsizecyclenum` globals still exist (pinned to
`FALSE` / `0`); the dead `if (smallwindow) { … }` and
`if (windowsizecyclenum == 1) { … }` branches in `loop_client.cpp` and
`function_client.cpp` are left in place as a follow-up cleanup sweep
rather than ripped out in the same commit (they're harmless because the
gate is permanently false). The "M" key now plainly minimizes the window
instead of cycling modes.

## What is already in place (baseline, May 2026)

- ✅ `WS_OVERLAPPEDWINDOW` style (sysmenu + min/max + thickframe) on
  `hWnd2`.
- ✅ `blit_letterbox(hWndDst, srcdc, srcW, srcH)` helper in
  `src/client/myddraw.cpp`: aspect-preserving blit with black bars,
  never upscales beyond 1:1.
- ✅ `blit_offx`, `blit_offy`, `blit_scale` globals
  (`src/common/globals.inc`, `src/common/data_both.h`) published every
  frame by `refresh()`.
- ✅ `WM_MOUSEMOVE` in `WndProc` translates raw client-pixel coordinates
  back to source-surface coordinates using those globals.
- ✅ Single-window-mode cleanup completed (Modes 2/3/4 removed; see above).
- ⏳ The back buffer is **still fixed at 1024×768**, so maximize currently
  produces native-pixel rendering centered with black margin (no extra
  game content in the margin yet — that's what this plan addresses).

---

## RW-P0 — Safety net (do FIRST, no behavior changes)

- ✅ **RW-P0.1** Catalogue every hard-coded `1024`, `768`, `512`, `384`, `32`,
  `24`, `34`, `26` literal in `src/client/**` and `src/common/u6o7.cpp`.
  Output: `docs/resizable-window-hotspots.md` listing file:line, current
  value, semantic meaning (e.g. "viewport tile width", "back-buffer height",
  "panel x"). Use this as the ground truth checklist for later phases.
  _(2026-05-20: catalogue committed at `docs/resizable-window-hotspots.md`,
  6 categories, ~75 hotspots, 8 highest-density files identified.)_
- ✅ **RW-P0.2** Add a Debug-only `LogClientMetrics()` called once per second
  that prints `clientW`, `clientH`, `srcW`, `srcH`, `blit_offx/y`,
  `blit_scale`, current `windowsizecyclenum`, and the active `hWnd`. Keeps
  feedback tight while changing rendering. _(2026-05-20: inline
  `OutputDebugStringA` block in `loop_client.cpp` per-second tick, gated by
  `#ifdef _DEBUG`. Also a one-shot log on each `OnClientResized` event.)_
- ⏭ **RW-P0.3** Capture **golden screenshots** of every major UI surface
  (main view, party screen, spellbook, inventory, conversation, map, intro,
  endgame) at the legacy 1024×768 client size. Store under
  `test/client/golden/1024x768/` for later regression comparison.
  _(Deferred: requires running the game and walking through each UI.
  Owner: human tester. Not blocking RW-P1 since that phase makes no
  visual changes at the legacy 1024×768 size.)_
- ✅ **RW-P0.4** Add a `client_settings` flag `windowResize = false` (default
  off) gating every behavior change in this plan. Allows merging incremental
  work without affecting players until the feature is ready.
  _(2026-05-20: `bool windowResize` global in `globals.inc` /
  `data_both.h`, parsed from `WINDOW_RESIZE` setting in `settings.txt`
  during client init in `function_client.cpp`. Defaults FALSE.)_
- **Exit:** Hotspot catalogue committed; golden screenshots stored;
  `windowResize` flag wired through `getsetting` / `client_settings`.
  _(Status 2026-05-20: 3 of 4 done. P0.3 deferred as a manual QA task;
  proceeding to P1 since P1 changes are non-visual at legacy size.)_

---

## RW-P1 — Window plumbing (resize input pipeline)

- ✅ **RW-P1.1** Add `WS_THICKFRAME` to `hWnd2` and `hWnd4` so the user can
  freely drag-resize, not only maximize. Keep `hWnd3` (the small fallback
  window) without `WS_THICKFRAME` for now. _(2026-05-20: switched
  `hWnd2` in `u6o7.cpp:494` and `hWnd4` in `function_client.cpp:2041` to
  `WS_OVERLAPPEDWINDOW`. The `WS_POPUP` fullscreen-fallback variant in
  `u6o7.cpp:499` and `hWnd3` in `u6o7.cpp:506` are intentionally
  unchanged. **Superseded 2026-05-20 by Option A**: `hWnd3`/`hWnd4`/popup
  fallback are now removed; only `hWnd2` is created.)_
- ✅ **RW-P1.2** Handle `WM_GETMINMAXINFO` in `WndProc` to enforce a sane
  minimum client size (e.g. 800×600) so the renderer never has to cope with
  tiny windows during early development. _(2026-05-20: enforced 800×600
  client minimum plus measured non-client overhead via
  `GetSystemMetrics(SM_CXSIZEFRAME / SM_CYSIZEFRAME / SM_CYCAPTION)`.)_
- ✅ **RW-P1.3** Handle `WM_SIZE` in `WndProc`. On non-`SIZE_MINIMIZED`
  events: store new client dims into globals `clientW`, `clientH`; set
  `dirtyClientSize = true`. Don't touch surfaces from inside `WndProc`.
  _(2026-05-20: handler at `u6o7.cpp` `WM_SIZE` case; `clientW`,
  `clientH`, `dirtyClientSize` declared in `globals.inc` /
  `data_both.h`. `blit_letterbox` also publishes `clientW/clientH` every
  frame as a belt-and-braces fallback.)_
- ✅ **RW-P1.4** In `loop_client.cpp` main tick, detect `dirtyClientSize` and
  call `OnClientResized(clientW, clientH)` which (initially) does nothing
  except re-publish `blit_*` globals — proving the event path works.
  _(2026-05-20: inline handler in `loop_client.cpp` after the per-second
  metrics block. Logs the resize event in Debug builds; reserved for
  surface recreation in RW-P2.2 and UI re-layout in RW-P3.3.)_
- ⏭ **RW-P1.5** Persist last-known client size + maximized flag to
  `client_settings`; restore on launch via `ShowWindow` + `MoveWindow` /
  `SetWindowPlacement`.
  _(Deferred: `settings.txt` is read-only via `getsetting()`; persistence
  needs a new write path. Not blocking RW-P2 work — windows simply open
  at the legacy 1024×768 size each launch until this lands. Tracked.)_
- **Exit:** User can drag-resize and maximize, position is remembered, and
  `OnClientResized` fires reliably. Rendering still letterboxed at 1024×768
  (no visual change yet).
  _(Status 2026-05-20: P1.5 deferred — drag-resize and maximize work,
  position is **not yet** remembered across runs but `OnClientResized`
  fires reliably in Debug logs. Acceptable to enter P2 in this state.)_

---

## RW-P2 — Dynamic back buffer (`ps`, `ps3`, `ps5`, lighting buffers)

- 🟡 **RW-P2.1** Replace fixed-size lighting arrays with heap-allocated
  buffers sized `clientW * clientH` (or rounded up to a tile multiple):
  - `ls`, `ls_moon1..4` (currently `unsigned char[1024*768]`).
  - Centralize size + (re)allocation in a new `ViewportBuffers` struct in
    `src/client/viewport_buffers.{h,cpp}`. No behavior change while size
    stays at 1024×768.
  _(2026-05-20 — partial: created `src/client/viewport.h` as the
  centralization seam with accessors `backbufferW()`, `backbufferH()`,
  `lightingStride()`, `lightingTotalBytes()`, plus a `recreateBackbuffers()`
  no-op stub. Today the accessors return the legacy `1024`/`768`
  constants, so behavior is bit-identical. The actual heap allocation of
  the `ls`/`ls_moon*` buffers and the inline-asm `mov ebp, 786432` pixel
  count in the lighting-compose pass are **not yet dynamic** — those land
  in the next RW-P2 commit after manual testing of the seam.)_
- 🟡 **RW-P2.2** Wrap `newsurf(1024,768,…)` calls in `setup_client.inc` with
  `newsurf(currentBackbufferW(), currentBackbufferH(), …)`. Provide
  `RecreateBackbuffers(newW, newH)` that frees and reallocates `ps`, `ps3`,
  `ps5`, `psnew1`, `psnew1b`. Call from `OnClientResized` (gated by
  `windowResize` flag).
  _(2026-05-20 — partial: `recreateBackbuffers(int newW, int newH)` is now
  called from the `OnClientResized` block in `loop_client.cpp` whenever
  `dirtyClientSize` flips and `windowResize` is on. The call is currently
  a no-op stub in `viewport.h`. The real work — releasing the DirectDraw
  surfaces and reallocating at the new client size — is the next step.
  `setup_client.inc` `newsurf(1024,768,…)` calls have **not** been
  rewritten yet; they should become `newsurf(backbufferW(), backbufferH(),
  …)` in the same commit that flips the seam to dynamic.)_
- 🟡 **RW-P2.3** Audit `function_client.cpp` blits and clamps for hard-coded
  `1024` / `768` / `>1024` / `>=768` checks; replace with
  `currentBackbufferW()` / `currentBackbufferH()`. Hotspots already known:
  view-blit clamp at ~line 332 (`(xoff + x_axis_size) > 1024`).
  _(2026-05-20 — first hot path converted: `LIGHTnew()` in
  `function_client.cpp` now reads `backbufferW()`, `backbufferH()`, and
  `lightingStride()` for its row-stride math (`y2 * lsStride` replacing
  `y2<<10`), the row-pitch skip, and both edge clamps. Behavior at the
  legacy 1024×768 is identical (compiler folds the constants). The
  remaining hot paths still to convert are the ones listed in
  `docs/resizable-window-hotspots.md` Section A — most importantly the
  `lightshow0` inline-asm loop at `loop_client.cpp:8264` (`mov ebp,
  786432`), the `&ls + sizeof(ls)` references at `loop_client.cpp:6579`,
  and the `ls_off=(y7<<10)+x7` etc. block at `loop_client.cpp:8070`.)_
- ⬜ **RW-P2.4** Update `refresh()` so `srcW/srcH` passed to
  `blit_letterbox` come from the actual surface dims (already does via
  `s->d.dwWidth/dwHeight` — verify and add an assert that they equal the
  current backbuffer size).
- ⬜ **RW-P2.5** Stress test: continuously toggle window between 800×600,
  1024×768, 1600×1200, and maximized while a player walks. Watch for surface
  recreation leaks (Application Verifier handle leak rule).
- **Exit:** Back buffer size follows client size at runtime. With
  `windowResize` enabled, the maximized window paints correctly to its full
  client area (UI in original positions, no upscale, no black bars).
  _(Status 2026-05-20: P2.1/P2.2/P2.3 partial — centralization seam in
  place, one hot path migrated, no behavior change at default
  resolution. Next RW-P2 commit must (a) flip the accessors in
  `viewport.h` from constants to `extern` runtime variables published by
  `recreateBackbuffers()`, (b) heap-allocate `ls`/`ls_moon*` and rewrite
  the asm pixel count + the `&ls`-vs-`ls` idiom at the 4 sites in
  `loop_client.cpp`, (c) free/recreate the DirectDraw surfaces inside
  `recreateBackbuffers()`, (d) finish converting the remaining hot paths
  in `loop_client.cpp` and `function_client.cpp`. Manual testing required
  between (a)+(b) and (c)+(d) since each step changes runtime behavior.)_

---

## RW-P3 — UI anchor system (panels follow window edges)

- ⬜ **RW-P3.1** Introduce `enum class UiAnchor { TopLeft, TopRight,
  BottomLeft, BottomRight, TopCenter, BottomCenter, LeftCenter,
  RightCenter, Center };` and `struct UiPlacement { UiAnchor anchor; int
  offsetX; int offsetY; int sizeX; int sizeY; };` in
  `src/client/ui_layout.{h,cpp}`. Provide `ResolveRect(UiPlacement, int
  clientW, int clientH)`.
- ⬜ **RW-P3.2** Inventory every absolute panel/widget coordinate (sidebar,
  minimap, status bar, action bar, action-talk bar, party-member panels,
  spellbook overlays, message log, character/inventory screen, intro
  overlays). Output: a static `kBuiltinPanels[]` table mapping each panel
  to a `UiPlacement` whose offsets reproduce **exact** legacy positions
  when `clientW=1024, clientH=768`.
- ⬜ **RW-P3.3** Reroute every panel's draw call through
  `ResolveRect(...)`. Keep legacy globals (`panelx[i]`, `panely[i]`,
  `panelnew[i].offset_x/y`) but recompute them every frame from the
  placements + current client size. Verify against golden screenshots from
  RW-P0.3.
- ⬜ **RW-P3.4** Convert hard-coded equipment-slot offsets in
  `define_both.h` (`helmx=52`, `wep_rightx=20`, `armourx=52`, `bootsx=52`,
  …) into a `kEquipSlotLayout` table relative to the inventory panel's
  origin. Recompute on resize.
- ⬜ **RW-P3.5** Migrate `function_client.cpp` minimap and party-frame
  positioning (`minimapnewx`, `panely2[0]`, `panelnew[panelminimap]`, …) to
  use `UiPlacement::BottomLeft` / `BottomRight` so they stick to the
  client corner instead of pixel-1024.
- ⬜ **RW-P3.6** Mouse hit-testing: confirm that the input remap in
  `WndProc` already feeds correct coordinates into the panel hit logic.
  Add a `Debug` overlay that draws each panel's resolved rect and the
  cursor position for visual verification.
- **Exit:** With `windowResize=true` and a maximized window, all UI panels
  appear in their correct corners/edges, equipment screen is laid out
  correctly, and mouse interactions land on the right widgets at every
  tested client size.

---

## RW-P4 — Expand the world view (more tiles around the player)

- ⬜ **RW-P4.1** Replace fixed view dimensions (`32`, `24`, `34+2`, `26+2`)
  with `viewTilesX = (currentBackbufferW() - sidePanelW()) / 32`,
  `viewTilesY = (currentBackbufferH() - bottomPanelH()) / 32`. Centralize
  in `Viewport.h` (`viewTilesX()`, `viewTilesY()`,
  `viewPixelW()`, `viewPixelH()`, `viewOffsetX()`, `viewOffsetY()`).
- ⬜ **RW-P4.2** Convert fixed-size visibility arrays to dynamically sized
  containers (heap-allocated, resized on viewport change):
  - `vis[34+2][26+2]`, `vis_window`, `vis_chair`, `vis_bed`, `vis_slime`
  - `vischeck[32][24]`, `nonvis[32][24]`
  - `visalways[256][1024]` (bit array — re-evaluate; may not need resizing).
- ⬜ **RW-P4.3** Update the world-render inner loops to iterate
  `0..viewTilesX-1`, `0..viewTilesY-1` instead of `0..31`, `0..23`. Update
  centering math (`x - x_axis_size/2`, etc.) accordingly.
- ⬜ **RW-P4.4** Update lighting/fog/storm-cloak passes to operate over the
  dynamic view extents. Verify `ls3..ls13` blur kernels still operate at
  the original tile granularity (these are 32-px blocks; should be safe to
  reuse unchanged at the per-tile level).
- ⬜ **RW-P4.5** Sprite/object scene buffer (`sobj[96][72]` in `player`
  struct, host-side): determine if 96×72 is a sufficient cap for any
  client view we want to support (maximized 1920×1080 = 60×34 tiles
  visible — fits). If not, extend the protocol in RW-P5; otherwise
  document the cap as an explicit `kMaxClientViewTilesX/Y` constant.
- ⬜ **RW-P4.6** Verify minimap centering still works when the world view
  is wider/taller than legacy.
- **Exit:** Maximizing the window reveals more game tiles around the
  player; UI hugs the edges; world state (NPCs, objects, lighting)
  remains correct at every visible tile; the legacy 1024×768 size is
  bit-identical to pre-feature behavior.

---

## RW-P5 — Server-side view extent (only if RW-P4.5 forces it)

- ⬜ **RW-P5.1** If a client requests a view larger than the current
  `sobj[96][72]` cap, decide between (a) raising the cap (memory cost on
  host), (b) capping the client view at a server-defined max, or (c) a
  protocol message letting the client advertise its view extent.
- ⬜ **RW-P5.2** Implement chosen option with version negotiation; old
  clients keep working.
- ⬜ **RW-P5.3** Update host scene-streaming code (`sobj_buf*`,
  `MESSAGEupdate*`) to honor the negotiated extent.
- **Exit:** Wide-view clients receive complete state; legacy clients are
  unaffected; protocol diff documented in `docs/protocol-changes.md`.

---

## RW-P6 — Polish

- ⬜ **RW-P6.1** Fullscreen toggle (Alt+Enter): borderless window covering
  the desktop, reusing the same dynamic-backbuffer path. Restore previous
  windowed placement on exit.
- ⬜ **RW-P6.2** Scaling option for users who *want* upscale (e.g. 200%
  integer scale) — re-introduce the upscale path in `blit_letterbox`
  behind a `client_settings.uiScale` enum.
- ⬜ **RW-P6.3** DPI awareness: declare the process per-monitor DPI-aware
  via manifest, react to `WM_DPICHANGED` (move/resize the window per
  Microsoft guidance, but still render at native pixels).
- ⬜ **RW-P6.4** Remove `windowResize` feature flag once stable; default
  on for everyone.
- ⬜ **RW-P6.5** Update `docs/ExternInventory.md` to reflect new globals
  (`blit_*`, `clientW`, `clientH`, `viewTilesX`, …) or, if RW-P3 introduces
  a settings struct, document it there.
- **Exit:** Feature is on by default, fullscreen works, DPI changes are
  handled, and documentation is up to date.

---

## Risk register

| ID  | Risk                                                                        | Mitigation                                                                  |
| --- | --------------------------------------------------------------------------- | --------------------------------------------------------------------------- |
| R1  | Hard-coded 1024/768 literals scattered across thousands of lines            | RW-P0.1 catalogue + grep guardrails in CI                                   |
| R2  | DirectDraw surface re-creation can leak if not paired correctly             | Centralize in `RecreateBackbuffers`; run Application Verifier in RW-P2.5    |
| R3  | UI mods / saves expecting fixed pixel positions                             | Keep legacy 1024×768 path bit-identical; gate behind `windowResize` flag    |
| R4  | Expanded view leaks fog-of-war / line-of-sight info beyond server intent    | RW-P5 negotiation; cap client view to server-approved extent                |
| R5  | Maximized window on multi-monitor produces oversized backbuffer + perf hit  | Cap backbuffer size; add `kMaxClientViewTilesX/Y` constant in RW-P4.5       |
| R6  | Mouse coordinate transform drift if `blit_*` globals are stale              | Already published every frame in `refresh()`; assert in Debug build         |

## Style appendix

- New code: `PascalCase` for types, `camelCase` for functions and locals,
  `kPascalCase` for compile-time constants. (Matches existing
  `plan-serverRefactor.md` convention.)
- New files use `#pragma once`.
- All public API additions live in headers under `src/client/` with a
  doc comment block describing arguments and side effects.
- No new global mutable state outside an explicit `ClientState` /
  `ViewportState` struct introduced in RW-P2 / RW-P3.

