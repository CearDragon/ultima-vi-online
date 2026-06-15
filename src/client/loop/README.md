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

