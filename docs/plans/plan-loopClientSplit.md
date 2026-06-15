# `loop_client.cpp` Decomposition Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before the next phase starts.

> Source of truth: this file. Tag PR titles and code comments with the phase ID
> (`LCS-P2.3:`, etc.).
> Phase ID prefix: **`LCS-P*`** ("Loop-Client Split").

---

## 0. Why this plan exists

`src/client/loop_client.cpp` is **13,075 lines / ~465 KB**. It is too large for
AI agents (and humans) to read or edit reliably. This plan breaks it into
smaller, logically-grouped files under **`src/client/loop/`**.

**This is a mechanical relocation, NOT a refactor.** No behaviour changes, no
control-flow rewrites, no de-`goto`-ing, no STL, no signature changes. We only
*move* contiguous line ranges into new files and `#include` them back in the
exact same order. A separate future plan can modernize the code.

### Critical facts about this file (read before touching anything)

These were verified by analysis on 2026-06-15; re-verify if the file changed.

1. **It is not a translation unit.** It is a raw statement block `#include`d
   into the middle of the main loop in `src/common/u6o7.cpp:703`
   (inside `WinMain`, between labels `mainloop:` and the frame-skip logic). It
   has no top-level function, no includes of its own, and relies on locals
   declared in `u6o7.cpp` (`t`, `t2`, `t3`, `i`, `i2`, `x`, `y`, `et`, `f`,
   `ps`, etc.).

2. **Brace depth starts and ends at 0.** The file is a flat sequence of
   top-level statements/blocks. This is what makes a sequential `#include`
   split legal: the preprocessor concatenates the chunks back into one TU
   before compilation, so anything that worked across line boundaries still
   works across `#include` boundaries — **provided every cut is made at brace
   depth 0** (or at a documented interior brace seam, see LCS-P4).

3. **`goto` is pervasive but almost entirely internal.** ~180 `goto`s and
   ~150 labels. After concatenation they remain in the same function scope, so
   they keep working across chunk boundaries. The only labels referenced that
   live **outside** this file are:
   - `mainloop` (in `u6o7.cpp`) — one-way exit jump, unaffected by splitting.
   - `intro_refresh` — defined **inside** this file (line ~12677); jumped to
     from both the intro block and the in-game block. It MUST stay reachable
     from both, i.e. it must remain in the concatenated TU (it will).
   The three "external-looking" targets `flash_skip2`, `U6Ohostlink1return`,
   and a third checked are all **commented-out / dead** — no live cross-file
   gotos other than `mainloop`.

4. **Two mega-blocks dominate the file** and cannot be split at depth 0:
   - `~1608–4054` — `if (intro) { … }`: the intro / login / create-character /
     transfer-character / main-menu state machine (~2,450 lines).
   - `~4056–12676` — `{ … }`: the in-game per-frame block. Contains the network
     message-dispatch loop (`CLIENT_readnext:` … `CLIENT_donemess:`), world
     render, lighting, movers/sobj decode, UI panels, status messages, SFX.
     (~8,600 lines — the worst offender.)
   These need an **interior** split (LCS-P4) that relocates the brace endpoints
   in a documented way.

### Where the cuts are legal (depth-0 boundaries)

Block-end lines where brace depth returns to 0 (legal cut points), as of
2026-06-15:
`98, 143, 148, 218, 955, 980, 999, 1035, 1082, 1119, 1160, 1168, 1313, 1430,
1441, 1463, 1514, 1532, 1550, 1605, 4054, 12676, 12768, 12783, 12826, 12869,
12933, 12954, 13032, 13042, 13074`.

> ⚠️ These line numbers WILL drift if the file is edited. Always re-run the
> brace-depth scan (LCS-P0.2) immediately before making a cut, and cut on the
> *current* boundary, not the number written here.

---

## LCS-P0 — Safety net & tooling (do FIRST, no moves yet)

- ⬜ LCS-P0.1 Capture a baseline `client` (and `both`) build; record exact
  warning count and a byte-identical baseline binary hash. This is the
  regression oracle: every later phase must reproduce an identical binary
  (the preprocessor output is unchanged by a pure `#include` split).
- ⬜ LCS-P0.2 Add a repeatable analysis script `tools/loop_split_scan.ps1` that
  prints, for `loop_client.cpp`: (a) current line count, (b) all brace-depth-0
  boundary line numbers, (c) all `goto` targets not defined in-file. Document
  the invocation here. (The one-off PowerShell used to author this plan is the
  starting point.)
- ⬜ LCS-P0.3 Create the destination directory `src/client/loop/` with a
  `README.md` that states the include-order contract (see LCS-P1.1) and links
  back to this plan.
- ⬜ LCS-P0.4 Confirm the `client`/`both` targets reach `loop_client.cpp` via
  `u6o7.cpp` only (grep proves a single `#include "loop_client.cpp"`), and that
  `loop_client.inc` no longer exists (it was deleted; see `docs/ui/README.md`
  §9). Record the include site (`u6o7.cpp:703`).
- **Exit:** Baseline build + binary hash recorded; scan script runs; empty
  `src/client/loop/` exists; include site documented.

## LCS-P1 — Establish the umbrella include mechanism

The goal: replace the single `#include "loop_client.cpp"` in `u6o7.cpp` with a
single `#include "loop/loop_client_all.cpp"` umbrella that, in turn, `#include`s
the chunk files **in order**. This isolates `u6o7.cpp` from future chunk churn.

- ⬜ LCS-P1.1 Create `src/client/loop/loop_client_all.cpp` containing only an
  ordered list of `#include` directives plus a header comment documenting:
  - the include-order contract (order is load-bearing; chunks share one
    function scope; never reorder without re-checking goto/label reachability),
  - the brace-seam convention used by mega-block chunks (LCS-P4),
  - a pointer to this plan.
- ⬜ LCS-P1.2 Move the entire current `loop_client.cpp` body verbatim into
  `src/client/loop/loop_client_part_00.cpp` (a single chunk, no other change).
  Make `loop_client_all.cpp` `#include` just that one part.
- ⬜ LCS-P1.3 Point `u6o7.cpp` at the umbrella: change line 703 from
  `#include "loop_client.cpp"` to `#include "loop/loop_client_all.cpp"`.
  Keep the old `loop_client.cpp` file in place but empty/redirecting until
  LCS-P5 to ease bisection, OR delete and update `update_cmake.py`/CMake — pick
  one and note it here.
- ⬜ LCS-P1.4 Rebuild `client` + `both`. Verify **byte-identical** binary to
  LCS-P0.1 (a pure file-shuffle must not change codegen). Fix include-path /
  relative-path issues (`#include` is relative to the including file's dir; the
  parts are under `loop/` so cross-references stay simple).
- **Exit:** Build is green and binary-identical; `u6o7.cpp` includes only the
  umbrella; one giant part file exists and compiles.

## LCS-P2 — Split the easy head & tail (depth-0 cuts only)

These regions are outside the two mega-blocks and split cleanly at depth-0
boundaries. Do these first to build confidence in the mechanism before tackling
the mega-blocks.

> For every part below: cut on the *current* depth-0 boundary (re-run
> LCS-P0.2), move the range into the new file, add the `#include` to the
> umbrella in the same position, rebuild, confirm binary-identical.

- ⬜ LCS-P2.1 **`part_input_top` — lines ~1–~218.** Equip-slot macro
  overrides (`helmx`…`necky`), mouse-wheel key synthesis, MIDI-keyboard
  passthrough, and other pre-frame input setup. Ends at the ~218 boundary.
  (Macros must precede every later chunk that uses them — keep this part
  first.)
- ⬜ LCS-P2.2 **`part_panel_hittest` — lines ~218–~1313.** Panel/frame
  hit-testing and click routing (`checkpanel:` / `checkobj:` /
  `skipbaseframe:` / `maskcheckfailed:` / `checkdone:`), small dialogs
  (vial-mix preamble). Respect interior labels — they all live inside this
  range. Cut on the ~1313 boundary.
- ⬜ LCS-P2.3 **`part_misc_prelude` — lines ~1313–~1605.** Remaining small
  top-level blocks before the intro state machine (cut at the ~1605
  boundary, just before `if (intro)`).
- ⬜ LCS-P2.4 **`part_refresh_tail` — lines ~12676–~13074 (EOF).** The shared
  `intro_refresh:` refresh block, MIDI/WAV info loops (`midiinfo_next:`,
  `wavinfo_next:`), status-message timing, font-leak workaround. Cut at the
  ~12676 boundary (start of `intro_refresh:`) through EOF.
- **Exit:** Head (3 parts) and tail (1 part) extracted; each cut produced a
  binary-identical build; the two mega-blocks remain as one residual part.

## LCS-P3 — Split the intro / menu state machine (`if (intro) { … }`)

The block `if (intro) { … }` (~1608–4054) is one top-level statement, so it
cannot be cut at depth 0 internally. Use the **brace-seam convention**:

> **Brace-seam convention.** When an interior cut is needed, the chunk that
> *opens* a brace ends with that brace left **open**, and the next chunk
> *begins* mid-block. Each such file gets a banner comment:
> `// LCS brace-seam: this file OPENS a block closed in <next part>` /
> `// LCS brace-seam: this file CONTINUES a block opened in <prev part>`.
> Cuts are made only at interior **depth-1** statement boundaries (where the
> block's direct child statements end), never inside a nested expression.

- ⬜ LCS-P3.1 Map the intro block's depth-1 statement boundaries (re-run the
  scan limited to lines 1608–4054, tracking depth relative to the block).
  Record the candidate interior cut lines and the labels they contain
  (`intro_done:`, `login:`, `maxminmini:`, `transferachar_*`, `createachar_*`,
  `autoformat_*_cull3`, `vquesnew:`/`nextvques:`/`ab_noselection:`,
  `vialmix2:`).
- ⬜ LCS-P3.2 **`part_intro_a`** — block open + intro animation/backdrop +
  main-menu selection, up to the first clean depth-1 seam. Opens the `if
  (intro)` block; banner = OPENS.
- ⬜ LCS-P3.3 **`part_intro_b`** — login / username-password entry / autoformat
  culling, between seams. Banner = CONTINUES.
- ⬜ LCS-P3.4 **`part_intro_c`** — create-character / transfer-character flows,
  between seams. Banner = CONTINUES.
- ⬜ LCS-P3.5 **`part_intro_d`** — remainder through `intro_done:` and the
  closing `}` at ~4054 (which ends with `goto intro_refresh;`). Banner =
  CONTINUES + CLOSES. Verify `intro_done:` and `goto intro_refresh` straddle
  correctly across the chunk boundary (they're in the same TU, so fine).
- ⬜ LCS-P3.6 Rebuild after the full intro split; confirm binary-identical.
- **Exit:** Intro state machine is 3–4 files, each ≲ 1,000 lines, build
  binary-identical, brace seams documented.

## LCS-P4 — Split the in-game per-frame block (`{ … }`, the big one)

The block `{ … }` (~4056–12676, ~8,600 lines) is the largest. Same brace-seam
convention as LCS-P3. Aim for ~8–12 files of < 1,000 lines each, grouped by the
natural label/section structure already in the code.

- ⬜ LCS-P4.1 Map the in-game block's depth-1 statement boundaries and the
  section labels within (re-run scan on 4056–12676). Record candidate seams.
- ⬜ LCS-P4.2 **`part_game_open`** — block open + time/btime update + early
  per-frame setup (from `{` at ~4056). Banner = OPENS.
- ⬜ LCS-P4.3 **`part_game_input`** — keyboard/mouse command handling, spell
  targeting (`gotspell:`, `ktarcast:`, `pathmove_force:`,
  `directionalmove_force:`, `mousemove_finish:`, alt-use/alt-look). Banner =
  CONTINUES.
- ⬜ LCS-P4.4 **`part_net_read`** — network read loop entry `CLIENT_readnext:`
  through the start of message dispatch. Banner = CONTINUES.
- ⬜ LCS-P4.5 **`part_net_scene`** — `scene_update_message:` ground-object
  (sobj) decode (`sobj_copyloop*c`, `screen*shiftokc`, `sobj_copiedpos*c`).
  ⚠️ **Wire-coupled** — read `.github/copilot-instructions.md` §Wire protocol
  and the DOB plan before touching; this is a *move only*, change nothing.
  Banner = CONTINUES.
- ⬜ LCS-P4.6 **`part_net_movers`** — mover decode
  (`changestate:`, `oum_*`, `mover_remove*`, `mover_move*`,
  `mover_dirfrmchange*`, `mover_statechange*`, `mover_add*`, `getnbits3:`).
  ⚠️ **Wire-coupled** (legacy `tpx_legacy/tpy_legacy` shim). Move only. Banner =
  CONTINUES.
- ⬜ LCS-P4.7 **`part_net_misc`** — remaining message handlers through
  `CLIENT_donemess:` (`getid_more:`, `lluc_nextpixel:`, voice/global/ignore
  message handling, volume). Banner = CONTINUES.
- ⬜ LCS-P4.8 **`part_world_render`** — world-tile draw loop, visibility
  (`vis_scan2:`, `viewfind_skip:`), xray, mover-in-bed, mask/offset
  (`mover_square:`, `passok:`, `passskip:`, `usenormalmask:`,
  `specialmaskused:`, `onhorse_specialoffsetused:`). ⚠️ Hot path — keep style;
  **no STL, no reorg.** Banner = CONTINUES.
- ⬜ LCS-P4.9 **`part_lighting_clouds`** — lighting/cloud/flash effects
  (`flash_disable*:`, `flash_skip:`, `cloudadded:`, `asm_lightshow*:`).
  Banner = CONTINUES.
- ⬜ LCS-P4.10 **`part_floating_text`** — on-screen display & floating/status
  text (`osdisplay_ktar*:`, `wraptext_recheck:`, `txtconlog_done:`,
  `txtsf_done:`, `donesf*:`, rune/garg edit, `multiple*_added:`). Banner =
  CONTINUES.
- ⬜ LCS-P4.11 **`part_player_walk`** — walk-through / movement resolution
  (`walkthru_*:`, `skiprefresh2:`, `skiplevnext:`, `dni:`, `keyjmp:`),
  quick-stats (`qkstf_mcdone:`), underground/black-key
  (`underground:`, `nextblackkey:`, `blackkeycheck_done:`, `gotkey:`). Banner =
  CONTINUES.
- ⬜ LCS-P4.12 **`part_panel_draw`** — per-frame panel draw + cltset mirror +
  drag clamp (`checkpanel2:`, `displaypanel:`, `displayobj:`, `inpf2crop:`),
  endgame message, SFX shift (`shiftdown3:`), block CLOSE at ~12676. Banner =
  CONTINUES + CLOSES. ⚠️ Touches FRAME globals & `cltset` (see
  `docs/ui/README.md`) — move only.
- ⬜ LCS-P4.13 Rebuild after the full in-game split; confirm binary-identical.
- **Exit:** In-game block is ~8–12 files, each < 1,000 lines (a couple of
  dense render/net parts may run slightly over — note any exceptions),
  build binary-identical, every brace seam documented with banners.

## LCS-P5 — Finalize, document, enforce

- ⬜ LCS-P5.1 Delete the now-empty original `src/client/loop_client.cpp` (or the
  redirect shim from LCS-P1.3). Update any docs/links that referenced it by
  line number (`docs/ui/README.md` lines 52/89/146/238/239/287/342/354/380/
  397/398/427/447/464–465, `docs/resizable-window-hotspots.md` rows).
- ⬜ LCS-P5.2 Update `update_cmake.py` / `CMakeLists.txt` and the build presets
  so the new `src/client/loop/*.cpp` parts are tracked as sources/headers for
  IDE indexing even though they are `#include`d (mirror how `loop_host.cpp` /
  `.inc` files are listed but compiled via `#include`).
- ⬜ LCS-P5.3 Re-run the full scan; verify no part exceeds ~1,000 lines (flag
  exceptions), every `goto` target still resolves in-TU, brace depth across
  the concatenated parts still nets to 0.
- ⬜ LCS-P5.4 Add a short section to `docs/ui/README.md` (or a new
  `src/client/loop/README.md`) describing the part layout and the
  include-order + brace-seam contract, so future agents edit the right small
  file instead of recreating a monolith.
- ⬜ LCS-P5.5 Final build of `client`, `host` (must be unaffected), and `both`;
  confirm `client`/`both` binaries are identical to the LCS-P0.1 baseline.
- **Exit:** Monolith gone; `src/client/loop/` holds the ordered parts +
  README; `client`/`both` build green and binary-identical to baseline; docs
  updated.

---

## Invariants (apply to EVERY phase)

1. **Pure relocation.** Never edit code while moving it. A cut/paste must leave
   the preprocessed token stream identical → binary identical. If the binary
   changes, you accidentally edited something — revert and redo.
2. **Cut only at brace depth 0** (LCS-P2) **or a documented depth-1 brace seam**
   (LCS-P3/P4). Never cut inside a nested expression, a `for(;;)` header, a
   line-broken statement (this codebase splits statements across many lines —
   e.g. `OBJremove\n(\nx\n);`), or between a label and the statement it labels.
3. **Order is load-bearing.** All parts share one function scope after
   concatenation. Keep the equip-slot `#define`s first; keep `intro_refresh:`
   reachable from both the intro and in-game chunks; never reorder parts
   without re-validating goto/label reachability.
4. **Wire-coupled parts (LCS-P4.5/4.6) and hot paths (LCS-P4.8) are move-only.**
   Do not touch encode/decode, buffer layout, `MV_TX_*`, `tpx_legacy`, or
   introduce STL. Do **not** bump `U6O_VERSION` — this plan changes no wire
   format.
5. **Re-scan before every cut.** Line numbers in this doc are a 2026-06-15
   snapshot and drift as soon as the file is touched.

## Session handoff

- Current first non-✅ phase: **LCS-P0.1**.
- Nothing has been moved yet. `loop_client.cpp` is still the 13,075-line
  monolith, `#include`d at `u6o7.cpp:703`.
- Analysis already done (this document): include mechanism, brace-depth-0
  boundary list, goto/label inventory, the two mega-block ranges
  (`~1608–4054` intro, `~4056–12676` in-game). Re-verify with LCS-P0.2 before
  acting.

