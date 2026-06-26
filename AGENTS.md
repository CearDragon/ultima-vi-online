# Ultima VI Online — AI agent guide

Multiplayer reimagining of *Ultima VI*, descended from 1990s C-with-extensions
and undergoing a deliberate modernization to clean C++17/20. Tile art/sound is
loaded at runtime from a user-owned original copy — never vendor game assets.

## Architecture: one source tree, three binaries
- `CMakeLists.txt` builds `client`, `host`, and `both` from the same sources,
  differentiated only by `#ifdef CLIENT` / `#ifdef HOST` (the `both` target
  defines both). `src/common/u6o7.cpp` holds `WinMain` + the main loop.
- Layout: `src/common/` (shared world state, RNG, asm blitters, wire defs),
  `src/client/` (DirectDraw renderer, UI, input), `src/server/` (world sim, AI).
- **`.inc` files are source, not headers** — textually `#include`d once into a
  single TU. Shared globals live in `src/common/globals.inc` (+ `extern` in the
  matching `data_*.h`). 32-bit only (`/MACHINE:X86`) — inner loops use x86
  inline asm in `src/common/inline_asm/fast*.asm`.
- The dedicated **host** also builds headless on Linux/i386 (Docker/k8s) behind
  the `src/common/platform/` shim; layout stays byte-compatible (no version bump).

## The decomposed game loop (read before editing it)
The per-frame/per-tick loops are split into ordered `#include` fragments under
`src/client/loop/` (`loop_client_all.cpp` umbrella) and `src/server/loop/`
(`loop_host_all.cpp`). See each dir's `README.md` for the part table.
- Fragments are **NOT translation units**: raw statement blocks sharing locals
  from `u6o7.cpp`, with intentionally **unbalanced braces** (brace seams) and
  `goto`s crossing file boundaries. Order is load-bearing.
- Edit them with `replace_string_in_file` (tight context) or `tools/loop_split_*`
  raw-byte tools. **Never `insert_edit_into_file`** — it re-balances braces and
  corrupts these files.

## Wire protocol — where most bugs land (`src/common/define_both.h`)
- Host↔client messages bit-pack offsets with hard-coded multipliers/widths, e.g.
  movers encode `y*MV_TX_W+x` in `MV_TX_BITS` bits over an `MV_TX_W×MV_TX_H`
  window (currently 65×… / 12 bits).
- A wire change means: encoder **and** decoder updated together, in **both** the
  `.cpp` **and** its `.inc` mirror, in **both** client and host, **and** a bump
  of `U6O_VERSION` (currently 15) — all in one commit. Never bump it otherwise.
- The host emits screen offsets in the legacy **32×24** frame (`viewTilesX/Y()`
  return legacy on host). Decode host messages with `tpx_legacy`/`tpy_legacy`,
  **never** the client's dynamic `tpx`/`tpy` (that was the mover-teleport desync).
- `struct player` buffers (`sobj_*`, `mv_*`) are byte-blitted — reordering them
  is a wire change. Rename *all* uses of a field together or none.

## Build & run
- Build through the existing tree (never configure a fresh dir — it fails on
  missing Windows headers): `cmake.exe --build cmake-build-debug --target <client|host|both> -j 18`.
  In CLion, prefer the shared `.run/*.run.xml` configs and `.idea/cmake.xml` profiles.
- No automated tests: verify by running `both` (host+client in one process) and
  exercising the path. Runtime asset bundle is `test/client/` (drop the EXE there).

## Conventions
- **Modernize what you touch** (de-`goto`, name things, extract helpers, add
  Doxygen) via the `cpp-modernizer` agent — but **behavior preservation is
  absolute**: no change to wire bytes, `.sav` layout, pixels, RNG order, or
  timing. Hot paths/asm are in scope but need pixel/byte-exact + benchmark proof.
- **Static-scratch trap:** single-letter locals (`x`,`y`,`i`,`i2`,`z`…) are often
  `static` and reused as pseudo-globals across labels — `grep_search` every use
  before renaming, or you split one variable into two.
- New shared constants → `define_both.h`; client display constants → `viewport.h`,
  with a comment explaining the math + wire coupling (mirror `MV_TX_W`).
- Plans live by lifecycle in `docs/plans/{todo,in-progress,done}/`; moving the
  file *is* the status change. Tag code comments/PRs with phase IDs (`RW-P4.11:`,
  `DOB-P3.2:`, `LHS-P*`). Hotspot ground truth: `docs/resizable-window-hotspots.md`.
- Detailed agent rules: `.github/copilot-instructions.md`. UI reference: `docs/ui/README.md`.

