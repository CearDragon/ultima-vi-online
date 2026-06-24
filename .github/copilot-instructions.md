# Ultima VI Online — Copilot agent instructions

Concise, repo-specific guidance. Read this in full before editing.

## Project direction: modernization in progress

This project is undergoing a **deliberate, large-scale modernization refactor**
away from its 1990s C-with-extensions origins toward clean, readable,
well-documented modern C++ (C++17/20). **Embrace this direction**: remove
`goto` chains, give variables meaningful names, extract duplication into
well-named functions, replace magic numbers/strings with named constants,
convert inline assembly to portable C++, introduce namespaces where they help,
and add Doxygen documentation. Prefer to *improve* the code you touch rather
than mirror the legacy style — and new code should be modern from the start.

- **Use the `cpp-modernizer` agent** (`.github/agents/cpp-modernizer.agent.md`)
  for refactoring work. It encodes the safe-modernization discipline for this
  repo — risk tiers, verification methods, and how to handle wire/save code,
  hot paths, and inline asm. The whole project is in scope, hot paths included.
- **The one rule that never bends: behavior preservation.** Modernization must
  not change the network wire format, `.sav` byte layout, rendered pixels, RNG
  draw sequence, or observable timing. The transformation must be
  *semantics-preserving*; prove it (characterization tests, byte-stream / pixel
  captures, benchmarks) before shipping — see the agent's §Verification.
- **Hot paths and inline asm ARE in scope** (the old "don't touch the hot
  paths" guidance is retired). They simply carry the highest verification bar:
  pixel/byte-exact golden comparison **plus** a before/after benchmark proving
  no meaningful performance regression.
- Track larger modernization efforts under `docs/modernization/` (a sibling of
  `docs/plans/`), tagging commits/comments with a phase ID like the existing
  plans do.

## Build topology

- Two binaries from one source tree:
  - `client` — client (`CLIENT` preprocessor macro defined)
  - `host` — host (no `CLIENT` macro)
  - `both` — combined "full" target (defines both `HOST` and `CLIENT`)
- Source layout:
  - `src/common/` — shared by both binaries (`data_both.h`, `define_both.h`,
    `function_both.{h,cpp}`, `globals.inc`, `u6o7.cpp` — holds WinMain and the
    main loop).
  - `src/client/` — client-only (`viewport.{h,cpp}`, `function_client.{h,cpp}`,
    `ui_*`, and the per-frame loop, now decomposed under `src/client/loop/`).
  - `src/server/` — host-only (`function_host.{h,cpp}`, `host.inc`,
    `setup_host.inc`, and the per-tick loop, now decomposed under
    `src/server/loop/`).
- **The per-frame / per-tick loops are now split into ordered `#include`
  fragments** — the monolithic `loop_client.cpp` and `loop_host.cpp` are gone:
  - Client: `src/client/loop/loop_client_all.cpp` (umbrella) `#include`s
    `loop_client_part_*.cpp` in order. See `docs/plans/plan-loopClientSplit.md`
    and `src/client/loop/README.md`.
  - Host: `src/server/loop/loop_host_all.cpp` (umbrella) `#include`s
    `loop_host_part_*.cpp` in order. See `docs/plans/plan-loopHostSplit.md` and
    `src/server/loop/README.md` for the full part-layout table.
  - `u6o7.cpp` `#include`s the two umbrellas inside the main loop (under
    `#ifdef CLIENT` / `#ifdef HOST`). The fragment files are **NOT translation
    units** — they are raw statement blocks concatenated into one function
    scope, share locals declared in `u6o7.cpp`, contain intentionally
    unbalanced braces (brace-seam parts), and have `goto`s that cross fragment
    boundaries. Edit them with `replace_string_in_file` (tight context) or the
    `tools/loop_split_*` raw-byte tools; **never** `insert_edit_into_file` (it
    re-balances braces and corrupts these files).
- Build dirs: `cmake-build-debug/` (VS) and `cmake-build-debug-visual-studio/`
  (Ninja). Outputs land in `bin/{client,host}/debug/`.
- **Build command (use this exact form):**

  ```powershell
  cmake.exe --build cmake-build-debug --target <target> -j 18
  ```

  where `<target>` is `client`, `host`, or `both`. **Always build through
  this pre-configured `cmake-build-debug/` tree** — do **not** re-run `cmake`
  to configure a fresh build directory from scratch. The existing tree already
  has the DirectX 7 / Win32 SDK include paths (under `src/common/include`) and
  the MSVC x86 toolchain wired up; a from-scratch configure in an agent shell
  routinely fails with **missing Windows header files** (`windows.h`,
  `ddraw.h`, etc.). If `cmake-build-debug/` is missing or stale, prefer the IDE
  (CLion/VS) run configurations in `.run/` over hand-configuring — see the
  README's *Shared run configurations* section.
- Prefer the **shared IDE run configurations** (`.run/{client,host,both,
  generate_icons}.run.xml`) and the **shared CMake profiles** (`.idea/cmake.xml`,
  Debug + Release) when working in CLion — they encode the correct target,
  working directory, and build-before-run wiring. They are committed to the
  repo on purpose; reuse them rather than inventing one-off configs.
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

## Plan lifecycle — where plans live by status

Plans under `docs/plans/` are organized by **lifecycle status folder**. When
you generate or move a plan, put it in the folder that matches its state:

- `docs/plans/todo/` — **new / not-yet-started** plans. **Always create a new
  plan here.**
- `docs/plans/in-progress/` — move a plan here **when work first begins** on it
  (its first phase is picked up).
- `docs/plans/done/` — move a plan here **when it is finished** (all phases ✅).

Rules:

1. A plan lives in exactly one status folder; moving it *is* the status change.
2. Update any links to a plan when you move it (the README, the
   `docs/plans/.../modernization/README.md` master index, and any
   `RW-P*` / `DOB-P*` / `MSRV-P*` code-comment references).
3. Keep the phase-ID tagging convention — the folder tracks lifecycle, the
   phase checkboxes inside the plan track granular progress.

## Resizable-window / dynamic-object plans

Two long-running, checkbox-tracked plans gate most rendering / wire changes:

- `docs/plans/done/plan-resizableWindow.md` — phases `RW-P0` … `RW-P5`.
- `docs/plans/todo/plan-dynamicObjectBuffer.md` — phases `DOB-P0` … `DOB-P5`.

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

## Code style — legacy today, modern tomorrow

- **Legacy baseline (what you'll find):** C-with-C++-extensions, raw 2D arrays,
  `goto`-driven control flow, single-letter scratch vars (`x`, `y`, `i`, `i2`,
  `z`, `z2`, …) — frequently `static` locals reused as ad-hoc globals across
  labels — and very large functions.
- **Direction (where we're going):** modern, readable C++. When you touch a
  region, **prefer to modernize it** — de-`goto`, name things, extract helpers,
  add docs — rather than mirror the legacy style. The whole project, hot paths
  included, is in scope. The only hard constraint is **behavior preservation**
  (§Project direction): prove the change is semantics-preserving, and for hot
  paths/asm add a benchmark. Run refactors through the `cpp-modernizer` agent.
- **The static-scratch-reuse trap:** a single-letter local is often `static`
  and reused as a pseudo-global across labels/sections. Rename or extract **all**
  of its uses together, or none — a partial rename splits one variable into two
  and silently changes behavior. Confirm every use with `grep_search` first.
- New shared constants go in `src/common/define_both.h`. New client-only display
  constants go in `src/client/viewport.h`. Always add a comment block that
  explains the math, the wire coupling (if any), and the invariants — mirror the
  style of `kViewportTilesXMax`, `MV_TX_W`, `kPanelHide*`. Prefer reusing the
  already-planned names in `docs/resizable-window-hotspots.md` for the
  `96/72/32/24` family over inventing new ones.

## Tooling

- `cpp-modernizer` agent (`.github/agents/cpp-modernizer.agent.md`) — the
  safe-modernization workflow (risk tiers, equivalence verification, hot-path
  and inline-asm handling). Drive refactors through it.
- `tools/loop_split_*` — the loop-decomposition toolkit (raw-byte, brace-aware):
  `loop_split_scan_host.ps1` (host-aware brace/seam + goto/label inventory),
  `loop_split_extract.ps1` (byte-faithful line-range move), `loop_split_banner.ps1`
  (safe banner prepend), `loop_split_commentcheck.ps1` (is a line inside a block
  comment?), `loop_split_oracle*.ps1` (preprocessor token-stream oracle for
  *pure relocation* — note: NOT valid for behavior-changing modernization).
- `tools/symbolize.exe` + `tools/analyze_dump.ps1` — crash-dump analysis;
  inputs in `tools/crash/`, bug captures in `tools/bug-images/`.
- `tools/build_icon.ps1` — icon regen (matches CMake's `generate_icons`
  target).
- `assets/map_patches/*.txt` — host data files loaded at startup, not
  generated.

## Don'ts (correctness rails — these still bind during modernization)

These are about **correctness**, not style, so they survive the modernization
push. (The old "don't modernize hot paths / don't use STL in hot paths" rules
are **retired** — hot paths are now refactorable *with* benchmarks + behavioral
proof; see §Project direction.)

- Don't change the wire format, `struct player` byte layout, or `.sav` format
  as a side effect of a refactor. A correct modernization changes **no** wire
  bytes — if yours would, that's a behavior change: stop. Genuine wire changes
  require encoder + decoder updated together in **both** the `.cpp` and the
  `.inc` mirror, in **both** client and host, **and** a `U6O_VERSION` bump, all
  in one commit.
- Don't bump `U6O_VERSION` for non-wire changes.
- Don't compute mover/sobj positions from the client's dynamic `tpx`/`tpy` when
  interpreting host messages — use `tpx_legacy`/`tpy_legacy` (the host emits the
  legacy 32×24 reference frame).
- Don't rename mover/sobj buffer fields or reorder `struct player` until you've
  accounted for every byte-blit and wire use (rename *all* uses together).
- Don't ship a refactor without a behavioral-equivalence check appropriate to
  its risk tier (cpp-modernizer §Verification). Hot-path / inline-asm changes
  additionally need a before/after benchmark.
- Don't use `insert_edit_into_file` on the brace-seam loop fragments under
  `src/{client,server}/loop/` — it corrupts their intentionally-unbalanced
  braces.

## New-session quick-start

1. Read the **Session handoff** sections of the plans under `docs/plans/`
   (incl. the loop-split plans) and any active records under
   `docs/modernization/`. Plans are filed by status: `docs/plans/todo/`,
   `docs/plans/in-progress/`, `docs/plans/done/` (see §Plan lifecycle) — the
   `in-progress/` folder is the fastest way to see what's actively being worked.
2. `grep -n "\.inc" CMakeLists.txt` to see which `.inc` files compile.
3. The per-frame/per-tick loops live in `src/client/loop/` and
   `src/server/loop/` as ordered `#include` fragments behind the umbrellas
   `loop_client_all.cpp` / `loop_host_all.cpp`, which `u6o7.cpp` `#include`s into
   the main loop (they are not standalone sources). See each dir's `README.md`.
4. If touching encode/decode or buffer dims: server encoder + server decoder
   (if any) + client encoder/decoder + `U6O_VERSION` — all in one PR.
5. For refactoring/modernization, use the `cpp-modernizer` agent; capture an
   equivalence baseline before editing.
6. Tag code comments with the relevant phase ID (`RW-P4.11:`, `DOB-P3.2:`,
   `LHS-P*`, or a `docs/modernization/` phase ID).
7. Build with `cmake.exe --build cmake-build-debug --target <client|host|both>
   -j 18` against the existing build tree — never configure a fresh one (it
   fails on missing Windows headers). Prefer the shared `.run/` configs in CLion.

