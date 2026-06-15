// =============================================================================
// loop_client_all.cpp  —  umbrella include for the client per-frame block.
//
// LCS-P1.1: This file replaces the single `#include "loop_client.cpp"` that
// used to live at src/common/u6o7.cpp:703 (inside WinMain, between the
// `mainloop:` label and the frame-skip logic). u6o7.cpp now does exactly one
// `#include "loop/loop_client_all.cpp"`, and THIS file `#include`s the ordered
// chunk parts. That isolates u6o7.cpp from future chunk churn.
//
// ---------------------------------------------------------------------------
// INCLUDE-ORDER CONTRACT (load-bearing — read before editing)
// ---------------------------------------------------------------------------
//  * None of the part_*.cpp files is a translation unit. They are raw
//    statement blocks that the preprocessor concatenates, IN THIS ORDER, into
//    the middle of WinMain. They have no includes of their own and rely on
//    locals declared in u6o7.cpp (t, t2, i, i2, x, y, et, f, ps, ...).
//  * ORDER IS EVERYTHING. All parts share one function scope after
//    concatenation:
//      - the equip-slot `#define`s in the first part must precede every later
//        part that uses them, so the first part stays first;
//      - `intro_refresh:` (defined in the tail part) must remain reachable
//        from BOTH the intro part and the in-game part — it is in the same TU,
//        so any order that keeps it present works, but do not move it out of
//        the concatenation.
//    Never reorder these includes without re-validating goto/label
//    reachability with tools/loop_split_scan.ps1.
//  * `goto` works across part boundaries (same function scope post-concat).
//    The only genuinely external jump target is `mainloop` (in u6o7.cpp, a
//    one-way exit). `flash_skip2` / `U6Ohostlink1return` are dead/commented;
//    `getnbits3` is a same-line label.
//
// ---------------------------------------------------------------------------
// BRACE-SEAM CONVENTION (for the mega-block splits, LCS-P3 / LCS-P4)
// ---------------------------------------------------------------------------
//  The two mega-blocks (`if (intro) { ... }` and the in-game `{ ... }`) cannot
//  be cut at brace depth 0. When a part splits one of them, the cut is made
//  only at an interior depth-1 statement boundary and each side carries a
//  banner comment:
//      // LCS brace-seam: this file OPENS a block closed in <next part>
//      // LCS brace-seam: this file CONTINUES a block opened in <prev part>
//      // LCS brace-seam: this file CONTINUES + CLOSES ...
//  A part that opens a brace ends with it left OPEN; the next part begins
//  mid-block. Because the preprocessor concatenates the files before parsing,
//  the unbalanced braces in individual files are expected and correct.
//
// See docs/plans/plan-loopClientSplit.md (phase prefix LCS-P*) and
// src/client/loop/README.md.
// =============================================================================

// LCS-P1.2: single residual chunk (the whole former monolith). Subsequent
// phases (LCS-P2..P4) carve this into ordered part_*.cpp files inserted above
// in source order; this comment marks the insertion list.

// LCS-P2.1: part_input_top — equip-slot #define overrides (helmx..necky) +
// pre-frame input setup + exit-request handling (former lines ~1..221). MUST
// stay first: the equip-slot macros it defines are used by later parts.
#include "loop_client_part_input_top.cpp"
// LCS-P2.2: part_panel_hittest — mouse scaling + panel/frame hit-testing and
// click routing (checkpanel:/checkobj:/skipbaseframe:/checkdone:) +
// new-resolution inventory simulation (former lines ~222..1313, now ~1..1094
// of the residual chunk at cut time).
#include "loop_client_part_panel_hittest.cpp"
// LCS-P2.3: part_misc_prelude — remaining small top-level blocks after the
// hit-test routine and before the intro/menu state machine (former lines
// ~1313..1605). Ends just before `if (intro) {`.
#include "loop_client_part_misc_prelude.cpp"
// LCS-P3: the intro / main-menu state machine `if (intro) { ... }`, split
// across four files at interior depth-1 brace seams (each `} //intro==N`).
// These four parts MUST stay in this order and contiguous: intro_a OPENS the
// `if (intro)` block, _b/_c CONTINUE it, _d CONTINUES + CLOSES it (its closing
// `}` ends with `goto intro_refresh;`). Splitting them or reordering breaks
// the brace balance / the goto.
#include "loop_client_part_intro_a.cpp"   // OPENS: intro states 100/101/102
#include "loop_client_part_intro_b.cpp"   // CONTINUES: states 200/202 (login/menu)
#include "loop_client_part_intro_c.cpp"   // CONTINUES: states 206/203/204/205 (create/transfer)
#include "loop_client_part_intro_d.cpp"   // CONTINUES+CLOSES: state 201 + timer tail + `}`
#include "loop_client_part_00.cpp"
// LCS-P2.4: part_refresh_tail — the shared `intro_refresh:` refresh block
// through EOF (MIDI/WAV info loops, status-message timing, font-leak
// workaround; former lines ~12676..13074). MUST stay LAST: `intro_refresh:` is
// jumped to from both the intro part and the in-game part (now in part_00),
// and `goto mainloop` exits to u6o7.cpp. Same TU after concat, so reachable.
#include "loop_client_part_refresh_tail.cpp"
