# `src/client/loop/` — split parts of the client per-frame block

These files are the decomposed pieces of what used to be the monolithic
`src/client/loop_client.cpp` (≈13,000 lines). See the trackable plan:
[`docs/plans/plan-loopClientSplit.md`](../../../docs/plans/plan-loopClientSplit.md)
(phase prefix `LCS-P*`).

## What this directory is

`loop_client_all.cpp` is the **umbrella**. `src/common/u6o7.cpp` does a single
`#include "loop/loop_client_all.cpp"` from inside `WinMain` (between the
`mainloop:` label and the frame-skip logic). The umbrella in turn `#include`s
the `loop_client_part_*.cpp` chunks **in order**.

## Part layout (final, after LCS-P0..P4)

The umbrella `#include`s these parts in exactly this order. Re-run
`tools/loop_split_scan.ps1 -File <part>` for the current line count of any part.

| # | Part file | Role / contents | Brace-seam |
|---|---|---|---|
| 1 | `loop_client_part_input_top.cpp` | equip-slot `#define`s (`helmx`…`necky`) + pre-frame input + exit-request | — (depth-0) |
| 2 | `loop_client_part_panel_hittest.cpp` | mouse scaling + panel hit-test / click routing (`checkpanel:`…`checkdone:`) | — (depth-0) |
| 3 | `loop_client_part_misc_prelude.cpp` | small blocks before the intro state machine | — (depth-0) |
| 4 | `loop_client_part_intro_a.cpp` | intro states 100/101/102 (animation/gypsy/backdrop) | OPENS `if (intro){` |
| 5 | `loop_client_part_intro_b.cpp` | intro states 200/202 (main menu / transfer / login) | CONTINUES |
| 6 | `loop_client_part_intro_c.cpp` | intro states 206/203/204/205 (controls / key remap / portrait) | CONTINUES |
| 7 | `loop_client_part_intro_d.cpp` | intro state 201 (create char) + `intro_done:` + `goto intro_refresh;` | CONTINUES + CLOSES |
| 8 | `loop_client_part_game_open.cpp` | in-game `{` + setup + input/command + spell targeting + active-player output msgs | OPENS in-game `{` |
| 9 | `loop_client_part_net.cpp` | `CLIENT_readnext:`…`read local message` (sobj/mover decode) — ⚠️ WIRE-COUPLED | CONTINUES |
| 10 | `loop_client_part_world_render.cpp` | `if(mess_UPDATEps&&clientframe)` world draw + lighting/clouds + floating text — ⚠️ HOT PATH | CONTINUES |
| 11 | `loop_client_part_player_walk.cpp` | `skiprefresh2:` + movement resolution + tmap/upflags | CONTINUES |
| 12 | `loop_client_part_panel_draw.cpp` | per-frame panel draw + endgame + sfx shift + the in-game block's closing `}` | CONTINUES + CLOSES |
| 13 | `loop_client_part_refresh_tail.cpp` | shared `intro_refresh:` + MIDI/WAV info + status timing + font-leak fix → `goto mainloop` | — (depth-0) |

Three parts intentionally exceed ~1,000 lines (`world_render` ≈3,490, `net`
≈1,505, `game_open` ≈1,567): each is a single depth-1 statement with no interior
depth-1 seam, so it cannot be sub-divided under the brace-depth invariants. A
later modernization pass (not this plan) could de-`goto`/restructure them.

> **Docs referencing `loop_client.cpp:<line>` are stale.** `docs/ui/README.md`
> and `docs/resizable-window-hotspots.md` cite the old monolith by line number.
> The file no longer exists; find the symbol/label in the matching part above.

## Tooling

- `tools/loop_split_scan.ps1` — brace-depth-0 boundary + goto-target scan
  (`-Boundaries`, `-StartLine/-EndLine` for windowed depth-1 seam scans).
- `tools/loop_split_extract.ps1` — byte-faithful line-range move between parts.
- `tools/loop_split_banner.ps1` — **safe** raw-byte banner prepend. Use this,
  **never `insert_edit_into_file`**, on the brace-seam parts: the IDE edit tool
  tries to brace-balance these intentionally-unbalanced files and corrupts them.
- `tools/loop_split_oracle.ps1` — token-stream regression oracle (the
  "pure relocation" check). Run from a VS x86 dev shell; expect
  `OK: token stream matches baseline`.

## The include-order contract (load-bearing)

1. **None of these files is a translation unit.** They are raw statement
   blocks concatenated by the preprocessor into the middle of `WinMain`. They
   have no includes of their own and rely on locals declared in `u6o7.cpp`
   (`t`, `t2`, `i`, `i2`, `x`, `y`, `et`, `f`, `ps`, …).
2. **Order is everything.** All parts share one function scope after
   concatenation. The equip-slot `#define`s in the first part must precede
   every later part that uses them. `intro_refresh:` (defined in the tail
   part) must stay reachable from both the intro and the in-game parts. Never
   reorder parts in the umbrella without re-checking goto/label reachability.
3. **`goto` works across files.** ~180 gotos / ~150 labels remain in the same
   function scope after concatenation, so cross-part jumps are legal. The only
   *external* target is `mainloop` (in `u6o7.cpp`, a one-way exit). The
   apparent externals `flash_skip2` / `U6Ohostlink1return` are dead/commented;
   `getnbits3` is a same-line label.

## The brace-seam convention (interior cuts)

The two mega-blocks (`if (intro) { … }` and the in-game `{ … }`) cannot be cut
at brace depth 0. When a part has to split one of them, the cut is made only at
an interior **depth-1 statement boundary**, and each side carries a banner:

```
// LCS brace-seam: this file OPENS a block closed in <next part>
// LCS brace-seam: this file CONTINUES a block opened in <prev part>
// LCS brace-seam: this file CONTINUES + CLOSES a block opened in <prev part>
```

Never cut inside a nested expression, a `for(;;)` header, or a statement that
this codebase has split across many physical lines.

## Editing rules

- **Pure relocation only.** Moving code must not change the preprocessed token
  stream. Validate with `tools/loop_split_scan.ps1` and by re-preprocessing the
  client TU (see the plan, phase LCS-P0).
- Wire-coupled parts (sobj/mover decode) and hot render/lighting paths are
  **move-only**: no encode/decode edits, no buffer-layout changes, no STL, and
  **do not** bump `U6O_VERSION` — this split changes no wire format.
- Tag code comments and PR titles with the phase ID (`LCS-P4.6:`, etc.).
