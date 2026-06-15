# Ultima VI Online — Copilot agent instructions

Concise, repo-specific guidance. Read this in full before editing.

## Build topology

- Two binaries from one source tree:
  - `client` — client (`CLIENT` preprocessor macro defined)
  - `host` — host (no `CLIENT` macro)
  - `both` — combined "full" target (defines both `HOST` and `CLIENT`)
- Source layout:
  - `src/common/` — shared by both binaries (`data_both.h`, `define_both.h`,
    `function_both.{h,cpp}`, `globals.inc`).
  - `src/client/` — client-only (`loop_client.cpp`, `viewport.{h,cpp}`,
    `function_client.{h,cpp}`, `ui_*`).
  - `src/server/` — host-only (`loop_host.cpp`, `function_host.{h,cpp}`,
    `host.inc`, `setup_host.inc`).
- Build dirs: `cmake-build-debug/` (VS) and `cmake-build-debug-visual-studio/`
  (Ninja). Outputs land in `bin/{client,host}/debug/`.
- `viewport.h` exposes display sizing functions on the client and stub
  fallbacks for the host (the host's `viewTilesX()` / `viewTilesY()` return
  the legacy **32 / 24**, intentionally — see §Wire protocol below).

## Wire protocol — where most bugs land

- Host ↔ client messages bit-pack offsets with hard-coded multipliers and
  widths. Examples currently in the tree:
  - sobj (ground items) scene update: `y*48+x` in 11 bits, 48×40 fill window.
  - mover (NPCs / monsters / animals / other players): `y*MV_TX_W+x` in
    `MV_TX_BITS` bits, `MV_TX_W × MV_TX_H` fill window (see
    `define_both.h`). Was `y*34+x` / 10 bits / 34×26 pre-2026-05-28.
- Rules:
  1. Encoder and decoder MUST change together, in **both** the `.cpp` and the
     `.inc` mirror, in **both** client and host.
  2. Per-player buffers in `struct player` (`sobj_*`, `mv_*`) are sometimes
     byte-blitted; changing layout is a wire change.
  3. **Bump `U6O_VERSION` in `src/common/define_both.h` whenever the wire
     format changes.** Mixed-version binaries silently misdecoding fields is
     worse than a connect refusal.
- Host-side `getscreenoffset()` uses `viewTilesX()`/`viewTilesY()`, which on
  the host returns **legacy 32×24**. So the host always emits screen offsets
  in the legacy reference frame. The client compensates with
  `getscreenoffset_legacy()` → `tpx_legacy` / `tpy_legacy` when decoding
  host messages. **Do not** remove either side of this shim, and **do not**
  use the client's dynamic `tpx`/`tpy` to decode host-emitted offsets — that
  was the "movers teleport then disappear" desync.

## Resizable-window / dynamic-object plans

Two long-running, checkbox-tracked plans gate most rendering / wire changes:

- `docs/plans/plan-resizableWindow.md` — phases `RW-P0` … `RW-P5`.
- `docs/plans/plan-dynamicObjectBuffer.md` — phases `DOB-P0` … `DOB-P5`.

Hotspot tables (literal `96`/`72`/`32`/`24` audit, etc.) live in
`docs/resizable-window-hotspots.md`.

Before touching `kViewportTilesXMax`, `sobj_buf*`, `sobj_tempfixed`,
`mv_*`, `MV_TX_*`, `sobj_bufoffx/y`, `backbufferW/H`, `lighting_*`, or any
ID prefixed `RW-P*` / `DOB-P*`:

1. Re-read the **Session handoff** section at the bottom of the relevant
   plan to find the first non-✅ phase.
2. Re-grep the hotspot file; it's the ground truth.
3. Tag code comments and PR titles with the phase ID (`RW-P4.11:`,
   `DOB-P3.2:`, etc.).

## Code style

- C-with-C++-extensions. Raw 2D arrays, `goto`-driven control flow,
  single-letter loop vars (`x`, `y`, `i`, `i2`, `z`, `z2`, …), `static`
  locals reused as scratch globals across labels.
- **Match the surrounding style.** Do not refactor toward modern C++ in hot
  paths (`loop_host.cpp` mover/sobj loops, `loop_client.cpp` world-render
  loops) — the fixed arrays and goto chains are intentional and
  performance-sensitive.
- New shared constants go in `src/common/define_both.h`. New client-only
  display constants go in `src/client/viewport.h`. Always add a comment
  block that explains the math, the wire coupling (if any), and the
  invariants — mirror the style of `kViewportTilesXMax`, `MV_TX_W`,
  `kPanelHide*`.

## Tooling

- `tools/symbolize.exe` + `tools/analyze_dump.ps1` — crash-dump analysis;
  inputs in `tools/crash/`, bug captures in `tools/bug-images/`.
- `tools/build_icon.ps1` — icon regen (matches CMake's `generate_icons`
  target).
- `assets/map_patches/*.txt` — host data files loaded at startup, not
  generated.

## Don'ts

- Don't rename mover/sobj buffer fields or reorder `struct player` casually
  — it's wire-coupled.
- Don't introduce STL containers in the mover/sobj/world-render hot paths.
- Don't compute mover/sobj positions from the client's dynamic `tpx`/`tpy`
  when interpreting host messages — use `tpx_legacy`/`tpy_legacy`.
- Don't bump `U6O_VERSION` for non-wire changes. **Do** bump it for every
  wire change, in the same commit.

## New-session quick-start

1. Read the **Session handoff** sections of both plans under `docs/plans/`.
2. `grep -n "\.inc" CMakeLists.txt` to see which `.inc` files compile.
3. Remember `loop_host.cpp` / `loop_client.cpp` are compiled by being
   `#include`d into `src/common/u6o7.cpp`, not as standalone sources.
4. If touching encode/decode or buffer dims: server encoder + server
   decoder (if any) + client encoder/decoder + `U6O_VERSION` — all in one
   PR.
5. Tag code comments with the phase ID (`RW-P4.11:`, `DOB-P3.2:`, …).

