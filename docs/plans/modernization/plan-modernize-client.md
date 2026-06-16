# `src/client/` Modernization Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before the next phase starts.

> Source of truth: this file.
> Phase ID prefix: **`MCLI-P*`** ("Modernize CLIent"). Tag PR titles and code
> comments with the phase ID (`MCLI-P4.3:`, etc.).
> Read [`README.md`](./README.md) first — Prime Directive, risk tiers (T0–T3),
> the `MOD-P0` shared safety net, and the coordination rules (especially with
> the **`RW-P*`** resizable-window plan, which owns viewport/lighting).
> Execute with the **`cpp-modernizer` agent`**.

---

## 0. What is in scope

`src/client/` is compiled only into `client` and `both` (`CLIENT` defined).
It is the Win32 + DirectDraw/DirectMusic front end.
The per-frame loop was already decomposed into ordered `#include` fragments by
**`LCS-P*`** (complete) — those fragments are the raw material for the hot-path
phases here.

| File(s) | Lines | Role | Dominant hazard |
|---|---|---|---|
| `loop/loop_client_part_*.cpp` (13 parts) | ~13,000 | per-frame block (intro state machine, in-game input, **world render + lighting**, **net decode**, panels) | T3 render/asm + T2 wire (`net`) — brace-seam, move-only edit tools |
| `function_client.cpp` | 2492 | sprites (`getspr`), input, status messages, settings I/O, **5 inline `_asm` blocks** | T3 asm + T1 |
| `myddraw.cpp` / `.h` | 1349 / 107 | DirectDraw surface wrapper, **6 inline `_asm` blocks** | T3 asm |
| `data_client.h` | 696 | ~400 `extern`s: surfaces, fonts, cursors, UI, audio, intro | global encapsulation |
| `viewport.{h,cpp}` | 283 / 289 | **already modern** (`u6o::client`, dynamic dims) — owned by `RW-P*` | coordinate only |
| `ui_layout.{h,cpp}` | 200 / 160 | **already modern** (`enum class UiAnchor`) | extend |
| `ui_panels_apply.cpp` | 335 | anchor application (RW-P3) | extend |
| `frame.{h,cpp}` | 167 / 105 | `FRAME` widget struct | T1 |
| `sound.cpp`, `dmusic.cpp`, `splash.cpp` | 201 / 711 / 198 | audio + splash | T1 |
| `define_client.h` | 79 | `U6OK_*` key-action defines | header hygiene |
| `setup_client.inc`, `client_bmp.inc`, `new_getspr.inc` | 922 / 419 / 356 | startup + sprite glue (non-TU includes) | T1 |
| `data_client.cpp`, `Resource.h` | small | init/glue | T0 |

**Coordination:** `viewport.*`, the lighting buffers (`ls`, `ls_moon*`,
`lightingStride`), and `kViewportTilesXMax` are **`RW-P*`-owned** — modernize
*around* them and reuse the planned `96/72/32/24` constant names from
`docs/resizable-window-hotspots.md`.

---

## MCLI-P1 — Header hygiene & constants

- ⬜ `MCLI-P1.1` Add `#pragma once` to every `src/client/*.h` alongside existing
  guards (`define_client.h`, `data_client.h`, `function_client.h`, `frame.h`,
  `myddraw.h`, `sound.h`, `dmusic.h`, `ui_layout.h`). **T0.**
- ⬜ `MCLI-P1.2` Convert the `U6OK_*` key-action `#define`s in `define_client.h`
  to `enum class ClientKey : int { … }` (or `inline constexpr`) with legacy
  aliases; values index `U6OK[128][2]` tables so they must not change. **T0.**
- ⬜ `MCLI-P1.3` Document `OPTION_HIRES` and the sprite-layout assumptions
  (already partly documented in `function_client.h`'s `g32`/`sf32` comments).
- **Exit:** all client `.h` use `#pragma once`; key actions named via enum;
  build of `client` + `both` green.

## MCLI-P2 — Inline assembly → C++ (`myddraw.cpp`, `function_client.cpp`) ⚠️ T3

The single highest-risk client work. The five C++ blitters in
`function_client.h` (`g32`, `g32z`, `sf32`, `sf32z`, `im32z`) are the **template**
— they already replaced asm and read the destination pitch at runtime (RW-P2.3).
Follow that pattern.

- ⬜ `MCLI-P2.1` **Pixel-capture harness first.** Build a `tools/modernize/`
  harness that renders representative source tiles/scanlines through each target
  blit into an off-screen surface and dumps the destination bytes; add a
  `QueryPerformanceCounter` microbenchmark. Save goldens.
- ⬜ `MCLI-P2.2` Convert the 6 `_asm{}` blocks in `myddraw.cpp` (lines ~397,
  507, 913, 1043, 1146, 1289) to C++ behind their exact signatures, one at a
  time. Match `shr`/`sar` semantics with correct signed/unsigned types. Keep the
  asm under `#ifdef U6O_LEGACY_ASM` for A/B until pixel-exact + benchmark parity.
- ⬜ `MCLI-P2.3` Convert the 5 `_asm` blocks in `function_client.cpp` (lines
  ~318, 2056, 2089, 2133, 2166, 2252 — incl. `LIGHTnew`, `refresh`). The
  `refresh()` and `LIGHTnew()` paths are lighting/back-buffer coupled — **read
  the active pitch from the surface at runtime** (do not hard-code `*1024`/
  `*2048`), coordinating with `RW-P*`. Pixel-exact + benchmark.
- **Exit:** zero `_asm` in `myddraw.cpp`/`function_client.cpp`; each conversion
  proven pixel-identical + within benchmark tolerance; `U6O_LEGACY_ASM` flag
  removed after sign-off.

## MCLI-P3 — `function_client.cpp` god-functions & helpers

- ⬜ `MCLI-P3.1` De-`goto` and decompose `getspr` (sprite assembly), the
  status-message system (`STATUSMESS*`), and `getsetting`/`setsetting_int`
  (settings.txt parse/rewrite) into named helpers with Doxygen. **T1** — note
  `getspr` is per-frame-ish; benchmark if it sits on the draw path.
- ⬜ `MCLI-P3.2` Modernize input handling (`u6okey*`, `keyhit`, `GETINPUT_*`)
  and the portrait cache (`loadportrait`/`getportrait*`) into small documented
  units. **T1.**
- ⬜ `MCLI-P3.3` Replace remaining magic numbers (font indices, sprite strides,
  surface sizes) with named constants; route any back-buffer-size literal through
  the `viewport.h` accessors rather than `1024`/`768`/`2048`. **T1**, coordinate
  with `RW-P*`.
- **Exit:** no function in `function_client.cpp` exceeds ~150 lines; settings &
  status systems documented; size literals named/routed; `client`+`both` green.

## MCLI-P4 — The `loop_client` parts (per-frame block) ⚠️ T2/T3

These are brace-seam, non-TU fragments. **Edit only with
`replace_string_in_file` (tight context) or the `tools/loop_split_*` tools —
never `insert_edit_into_file`.** Modernize one self-contained region at a time;
do not move a `goto` whose label lives in another part unless you hoist both
sides into a real function.

- ⬜ `MCLI-P4.1` **Intro state machine** (`intro_a..d`): convert the
  `if (intro){ … }` integer-state dispatch (states 100/101/102/200/…/201) into a
  documented `enum class IntroState` + structured handlers, preserving the exact
  per-state side-effect order and the `intro_refresh:` / `mainloop` seams. **T1**
  (golden screenshots from `MOD-P0`/`RW-P0.3`).
- ⬜ `MCLI-P4.2` **Panel hit-test / draw** (`panel_hittest`, `panel_draw`):
  extract the click-routing and per-frame draw into named helpers routed through
  `ui_layout` (`ResolveRect`) where `RW-P3` already started. **T1.**
- ⬜ `MCLI-P4.3` **`net` part (sobj/mover decode)** ⚠️ **WIRE (T2).** Byte-stream
  capture the decode first (feed recorded host packets, dump the decoded
  buffers). Then de-`goto`/name without changing a single decoded value; keep
  `tpx_legacy`/`tpy_legacy` usage exactly. `U6O_VERSION` untouched. Coordinate
  with `DOB-P*` (it owns the buffer dims).
- ⬜ `MCLI-P4.4` **`world_render` + lighting** ⚠️ **HOT PATH (T3).** The ~3,490-
  line tile rasterizer + lighting/clouds, incl. the `_asm` at ~1825 and the
  pitch-coupled loops. Pixel-capture + benchmark first; convert asm to C++
  reading `lightingStride()`; de-`goto` the tile loops in small, separately
  pixel-verified steps. Coordinate with `RW-P*`. Note: the known `C4731` warning
  here is pre-existing — eliminating the asm should clear it (record before/after).
- ⬜ `MCLI-P4.5` **`game_open`, `player_walk`, `refresh_tail`, `input_top`,
  `misc_prelude`:** de-`goto` and name the in-game input/command dispatch and
  movement resolution incrementally. **T1/T2** (movement reads input wire state —
  characterize).
- **Exit:** each part has its `goto` chains structured *within* the part (cross-
  part jumps documented, not severed); the `net` decode is byte-proven and
  `world_render` pixel-proven + benchmarked; brace seams intact; `client`+`both`
  build green with no new warnings (C4731 cleared where asm removed).

## MCLI-P5 — Encapsulate client global state (`data_client.h`)

- ⬜ `MCLI-P5.1` Inventory the ~400 `extern`s and group: `Surfaces` (`ps*`,
  sprite sheets, portraits, intro art), `Fonts`, `Cursors`, `AudioState`
  (midi/wav/voice/mixer), `UiPanels` (`uipanel*`, `FRAME*`s), `Minimap/Tmap`,
  `LightingState` (**RW-owned — coordinate**), `IntroState`, `InputState`,
  `Settings` (`cltset*`), `Clouds`, `VoiceChat`, `Misc`.
- ⬜ `MCLI-P5.2` Encapsulate the **non-RW, non-wire** groups behind structs +
  deprecated reference shims (start with `Fonts`, `Cursors`, `Clouds`,
  `VoiceChat`). **T1.**
- ⬜ `MCLI-P5.3` Document `client_settings` (the positional, untagged file
  format — new fields must be appended last) and `FRAME`/`FRM_*` in `frame.h`.
- **Exit:** extern inventory complete with RW/wire flags; ≥3 non-RW groups
  encapsulated; `client_settings`/`FRAME` documented; `client`+`both` green.

## MCLI-P6 — Audio, splash, startup glue

- ⬜ `MCLI-P6.1` Modernize `sound.cpp` and `dmusic.cpp` (DirectMusic wrapper):
  RAII for device/handle lifetimes, named constants, Doxygen. **T1.**
- ⬜ `MCLI-P6.2` Modernize `splash.cpp` (already partly modern — `#pragma once`)
  and the startup `setup_client.inc` glue into named init steps. **T1.**
- ⬜ `MCLI-P6.3` Document `client_bmp.inc` / `new_getspr.inc` (non-TU sprite
  glue) and the include contract; decide whether either can become a real TU.
- **Exit:** audio uses RAII handle wrappers; startup is named steps;
  `client`+`both` green.

## MCLI-P7 — Finalize & enforce

- ⬜ `MCLI-P7.1` Remove deprecated shims from P1/P5 once all call sites migrate.
- ⬜ `MCLI-P7.2` Raise warnings on client TUs; confirm the `world_render` C4731
  is gone (asm removed) or document any residual.
- ⬜ `MCLI-P7.3` Update the §6 dashboard in `README.md`; write
  `docs/modernization/client-*.md` records.
- **Exit:** shims gone; client warning-clean (sans documented exceptions);
  records + dashboard updated; `client`/`both` behaviorally + pixel equivalent.

---

## Invariants (apply to EVERY phase)

1. **Pixel-exact.** Any render/lighting/asm change ships only with a destination-
   surface byte-equality proof **and** a benchmark (T3).
2. **Wire-frozen.** The `net` decode and any input encode change no bytes; use
   `tpx_legacy`/`tpy_legacy`; don't bump `U6O_VERSION`; coordinate with `DOB-P*`.
3. **RW boundary.** `viewport.*`, lighting buffers, and `kViewportTilesXMax` are
   `RW-P*`-owned — read through their accessors, don't re-hard-code `1024/768/2048`.
4. **Brace-seam parts.** Use `replace_string_in_file` / `tools/loop_split_*`,
   never `insert_edit_into_file`, on `loop/loop_client_part_*.cpp`. Don't sever a
   cross-part `goto`.
5. **`client` + `both` both build** after every increment.

## Session handoff

- **2026-06-16 (drafted).** Plan committed; no code yet. Finish `MOD-P0` first,
  then start at **`MCLI-P1.1`**. Before P2/P4.4 (asm/render) re-read the `RW`
  plan's handoff; before P4.3 (net decode) re-read the `DOB` plan's handoff.

