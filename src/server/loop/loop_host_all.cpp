// =============================================================================
// loop_host_all.cpp  —  umbrella include for the host per-tick block.
//
// LHS-P1.1: This file replaces the single `#include "loop_host.cpp"` that used
// to live at src/common/u6o7.cpp:691 (inside the main loop, under
// `#ifdef HOST` / `if (NEThost){ … }`). u6o7.cpp now does exactly one
// `#include "loop/loop_host_all.cpp"`, and THIS file `#include`s the ordered
// chunk parts. That isolates u6o7.cpp from future chunk churn.
//
// ---------------------------------------------------------------------------
// THE WHOLE FILE IS ONE BLOCK (read before editing)
// ---------------------------------------------------------------------------
//  The entire former loop_host.cpp body is a single `if (NEThost) { … }` block
//  (the inner one — u6o7.cpp's own `#ifdef HOST` guard wraps the include). So:
//    * the FIRST part (part_open_housekeeping) OPENS `if (NEThost) {` and must
//      stay first forever;
//    * the LAST part (part_tail_npcframe) CONTINUES + CLOSES that block and
//      must stay last forever;
//    * every other part CONTINUES the block.
//  There are NO brace-depth-0 interior cut points in this file. Every interior
//  cut is a host-aware brace seam — see src/server/loop/README.md.
//
// ---------------------------------------------------------------------------
// INCLUDE-ORDER CONTRACT (load-bearing)
// ---------------------------------------------------------------------------
//  * None of the part_*.cpp files is a translation unit. They are raw statement
//    blocks that the preprocessor concatenates, IN THIS ORDER, into the middle
//    of the main loop. They have no includes of their own and rely on locals
//    declared in u6o7.cpp (t, t2, t3, i, i2, x, y, et, ett, tpl, tplayer,
//    myobj, ...).
//  * ORDER IS EVERYTHING. All parts share one block scope after concatenation.
//    Never reorder these includes without re-validating goto/label reachability
//    and the brace seams with tools/loop_split_scan_host.ps1.
//  * `goto` works across part boundaries (same scope post-concat). There are NO
//    live cross-file gotos out of this file; the four external-looking targets
//    (badhorseswap, CLIENT_donemess, triplecrossbow_miss2, U6Ohostlink1) are
//    all dead/commented.
//
// ---------------------------------------------------------------------------
// BRACE-SEAM CONVENTION (for the mega-block splits, LHS-P2..P6)
// ---------------------------------------------------------------------------
//  A part that opens a brace ends with it left OPEN; the next part begins
//  mid-block. Each such file carries a banner comment:
//      // LHS brace-seam: this file OPENS a block closed in <next part>
//      // LHS brace-seam: this file CONTINUES a block opened in <prev part>
//      // LHS brace-seam: this file CONTINUES + CLOSES ...
//  Because the preprocessor concatenates the files before parsing, the
//  unbalanced braces in individual files are expected and correct.
//
// ---------------------------------------------------------------------------
// WIRE-COUPLED PARTS ARE MOVE-ONLY
// ---------------------------------------------------------------------------
//  The sobj/mover encode (MEGA A), client-message dispatch (MEGA B), per-player
//  INPUT (MEGA D), and the tail NPC-frame encode are wire-coupled. Moving them
//  must not change the token stream: no encode/decode edits, no buffer-layout
//  changes, no STL, and do NOT bump U6O_VERSION — this split changes no wire
//  format.
//
// See docs/plans/plan-loopHostSplit.md (phase prefix LHS-P*) and
// src/server/loop/README.md.
// =============================================================================

// LHS-P1.2 / LHS-P2: ordered chunk list. part_open_housekeeping OPENS the
// `if (NEThost) {` block and MUST stay first; part_tail_npcframe CONTINUES +
// CLOSES it and MUST stay last. The residual part_00 (the four mega-blocks +
// inter-block statements) sits between them and is carved further in
// LHS-P3..P6 (its sub-parts are inserted in source order in place of part_00).
#include "loop_host_part_open_housekeeping.cpp"  // OPENS `if (NEThost) {`
// LHS-P3: MEGA A (per-player UPDATE-message builder) -- `for (tpl..) { if
// (playerlist[tpl]) { ... } }`. Cut at section labels validated comment-safe
// (the display/sfx boundary sits BEFORE a 448-line commented-out reference
// block; cutting inside it caused MSVC C1071). part_a_save OPENS the for +
// if(playerlist) blocks; the middle parts CONTINUE; part_a_display_sfx
// CONTINUES + CLOSES them. WIRE-COUPLED (sobj/mover encode in part_a_sobj /
// part_a_mover) -- move-only. MUST stay in this order & contiguous.
#include "loop_host_part_a_save.cpp"          // OPENS for+if: save/login/password
#include "loop_host_part_a_housestore.cpp"    // CONTINUES: housestorage add + housescan
#include "loop_host_part_a_sobj.cpp"          // CONTINUES: sobj buffer build (WIRE)
#include "loop_host_part_a_mover.cpp"         // CONTINUES: mover add/remove + lighting (WIRE)
#include "loop_host_part_a_display_sfx.cpp"   // CONTINUES+CLOSES: dead-block + display + SFX
// LHS-P4: MEGA B (client-message dispatch from host_gotmessage:) -- wrapped in
// `if (x3) { ... } //read client message`. part_b_dispatch OPENS that wrapper
// (and folds in the interA socket-read prelude); the middle parts CONTINUE it;
// part_b_leave_resurrect_house CONTINUES + CLOSES it and folds in interB.
// WIRE-COUPLED (reads client messages) + admin commands -- move-only. MUST stay
// in this order & contiguous.
#include "loop_host_part_b_dispatch.cpp"              // OPENS if(x3): interA + dispatch + player_add + username
#include "loop_host_part_b_housestore_restore.cpp"    // CONTINUES: house-storage restore
#include "loop_host_part_b_charcreate.cpp"            // CONTINUES: ban + char create/restore
#include "loop_host_part_b_admin.cpp"                 // CONTINUES: sysban + admin dispatch start
#include "loop_host_part_b_admin_exec.cpp"            // CONTINUES: privilege + slash-command exec
#include "loop_host_part_b_slash.cpp"                 // CONTINUES: anotherslashcommand
#include "loop_host_part_b_chat_npc.cpp"              // CONTINUES: chat/talk + NPC conversation
#include "loop_host_part_b_leave_resurrect_house.cpp" // CONTINUES+CLOSES: leave/resurrect/house/ship + interB
// LHS-P5: MEGA C (object/creature/NPC AI update) -- `if (ouln!=-1) { for (i..) {
// ... } }`. HOT PATH (performance-sensitive). One deeply-nested block; cut at
// section labels validated comment-safe. part_c_objupdate OPENS the
// if(ouln)/for(i) blocks; the middle parts CONTINUE; part_c_npcattack_c
// CONTINUES + CLOSES them. Move-only: no STL, no hot-path reorg. MUST stay in
// this order & contiguous.
#include "loop_host_part_c_objupdate.cpp"       // OPENS if(ouln)/for(i): obj update + powderkeg
#include "loop_host_part_c_crtenum.cpp"         // CONTINUES: summoned removal + creature enum/target
#include "loop_host_part_c_npccast.cpp"         // CONTINUES: NPC spellcasting + npc actions
#include "loop_host_part_c_traps_schedule.cpp"  // CONTINUES: traps/poison + schedules
#include "loop_host_part_c_npcwalk_follow.cpp"  // CONTINUES: npc_walk + follow/path
#include "loop_host_part_c_npcattack_a.cpp"     // CONTINUES: slidemove + npcattack2hnd
#include "loop_host_part_c_npcattack_b.cpp"     // CONTINUES: crossbow0 + oilflask
#include "loop_host_part_c_npcattack_c.cpp"     // CONTINUES+CLOSES: hth + spellattcrt0 + attackdone
// LHS-P6: MEGA D (per-player INPUT processor) -- `for (tpl..) { if (playerlist..)
// { ... } }`. The largest mega-block; one deeply-nested block, cut at section
// labels validated comment-safe. part_d_open OPENS the for(tpl)/if(playerlist)
// blocks; the middle parts CONTINUE; part_d_ship CONTINUES + CLOSES them.
// WIRE-COUPLED (reads/acts on client input) -- move-only, no U6O_VERSION bump.
// MUST stay in this order & contiguous.
#include "loop_host_part_d_open.cpp"            // OPENS for(tpl)/if: input open + resurrect + switchparty
#include "loop_host_part_d_cast.cpp"            // CONTINUES: cast_spell0 + staffcast + nextspell
#include "loop_host_part_d_chainbolt.cpp"       // CONTINUES: chainbolt + spellattcrt1
#include "loop_host_part_d_summon_untrap.cpp"   // CONTINUES: summon/conjure + untrap + pickpocket
#include "loop_host_part_d_weather_mass.cpp"    // CONTINUES: wind/weather + mass spells + spelldone
#include "loop_host_part_d_joinitem_wep.cpp"    // CONTINUES: joinitem + weapon equip
#include "loop_host_part_d_lockpick_drop.cpp"   // CONTINUES: lockpick + drop + checkvortexcube
#include "loop_host_part_d_skiff.cpp"           // CONTINUES: skiff drop/pickup
#include "loop_host_part_d_use_a.cpp"           // CONTINUES: item-use dispatch part 1
#include "loop_host_part_d_use_b.cpp"           // CONTINUES: item-use dispatch part 2
#include "loop_host_part_d_look.cpp"            // CONTINUES: look/examine
#include "loop_host_part_d_ship.cpp"            // CONTINUES+CLOSES: ship attack + MEGA D end
#include "loop_host_part_tail_npcframe.cpp"       // CONTINUES + CLOSES (NPC-frame encode)

