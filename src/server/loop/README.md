# `src/server/loop/` — split parts of the host per-tick block

These files are the decomposed pieces of what used to be the monolithic
`src/server/loop_host.cpp` (26,875 lines / ~1.1 MB at the 2026-06-16 snapshot).
See the trackable plan:
[`docs/plans/plan-loopHostSplit.md`](../../../docs/plans/done/plan-loopHostSplit.md)
(phase prefix `LHS-P*`).

## What this directory is

`loop_host_all.cpp` is the **umbrella**. `src/common/u6o7.cpp` does a single
`#include "loop/loop_host_all.cpp"` from inside the main loop (at `u6o7.cpp:691`,
inside `#ifdef HOST` / `if (NEThost){ … }`). The umbrella in turn `#include`s the
`loop_host_part_*.cpp` chunks **in order**.

## The whole file is ONE block (read this first)

Unlike the client split (which had ~30 brace-depth-0 cut points), **the entire
host loop body is a single `if (NEThost) { … }` block**. There are NO
brace-depth-0 interior cut points. Therefore:

- The **first** part (`part_open_housekeeping`) OPENS the `if (NEThost) {` and
  must stay first forever.
- The **last** part (`part_tail_npcframe`) CONTINUES + CLOSES that block and
  must stay last forever.
- Every other part CONTINUES the block (it neither opens nor closes the outer
  brace).

Cuts are made only at **host-aware brace seams** — a `}` that returns the
absolute brace depth to the level of a direct child of the outer block (depth 1)
or, inside a mega-block, to the level of a direct child of that mega-block
(depth 2/3). Never cut inside a nested expression, a `for(;;)` header, a
statement this codebase has split across many physical lines, or between a label
and its statement.

## The brace-seam convention (interior cuts)

A part that opens a brace ends with it left **OPEN**; the next part begins
mid-block. Each such file carries a banner comment (prepended with
`tools/loop_split_banner.ps1`, **never** the IDE edit tool — see Editing rules):

```text
// LHS brace-seam: this file OPENS a block closed in <next part>
// LHS brace-seam: this file CONTINUES a block opened in <prev part>
// LHS brace-seam: this file CONTINUES + CLOSES a block opened in <prev part>
```

Because the preprocessor concatenates the files before parsing, the unbalanced
braces in individual files are expected and correct.

## The include-order contract (load-bearing)

1. **None of these files is a translation unit.** They are raw statement blocks
   that the preprocessor concatenates, IN ORDER, into the middle of the main
   loop. They have no includes of their own and rely on locals declared in
   `u6o7.cpp` (`t`, `t2`, `t3`, `i`, `i2`, `x`, `y`, `et`, `ett`, `tpl`,
   `tplayer`, `myobj`, …).
2. **Order is everything.** All parts share one block scope after concatenation.
   `part_open_housekeeping` stays first, `part_tail_npcframe` stays last. Never
   reorder parts without re-validating goto/label reachability and the brace
   seams with `tools/loop_split_scan_host.ps1`.
3. **`goto` works across part boundaries** (same scope post-concat). There are
   **no live cross-file gotos out of this file** — the four external-looking
   targets (`badhorseswap`, `CLIENT_donemess`, `triplecrossbow_miss2`,
   `U6Ohostlink1`) are all dead/commented.

## Wire-coupled parts are move-only

The sobj/mover encode in MEGA A, the client-message dispatch in MEGA B, the
per-player INPUT in MEGA D, and the NPC-frame encode in the tail are
**wire-coupled**. Moving them must not change the preprocessed token stream: no
encode/decode edits, no buffer-layout changes, no `MV_TX_*` / `sobj_*` /
`tpx_legacy` changes, no STL, and **do not** bump `U6O_VERSION` — this split
changes no wire format.

## Part layout (final, after LHS-P2..P6)

The umbrella `#include`s these 35 parts in exactly this order. `part_open_*`
must stay first (OPENS the `if (NEThost) {` block) and `part_tail_*` last
(CLOSES it). Line counts include the banner comment; re-run
`tools/loop_split_scan_host.ps1 -File <part>` for the live count.

| # | Part file | Lines | Role / contents | Brace-seam | Wire |
|---|---|---|---|---|---|
| 1 | `loop_host_part_open_housekeeping.cpp` | 271 | block open + per-tick housekeeping (economy, house payments, leverchange0, MIDI) | OPENS `if(NEThost){` | |
| 2 | `loop_host_part_a_save.cpp` | 1084 | MEGA A: save/login/password (save_update1, gotprevsave, save_empty) | OPENS for+if(playerlist) | ⚠️ |
| 3 | `loop_host_part_a_housestore.cpp` | 1052 | house storage add + housescan | CONTINUES | |
| 4 | `loop_host_part_a_sobj.cpp` | 439 | sobj buffer build (sobj_copyloop, objbuffer*) | CONTINUES | ⚠️ |
| 5 | `loop_host_part_a_mover.cpp` | 717 | mover add/remove + lighting (mvobjskip, mover_add_*, MV_TX_*) | CONTINUES | ⚠️ |
| 6 | `loop_host_part_a_display_sfx.cpp` | 570 | dead reference block + display_override + SFX | CONTINUES+CLOSES A | |
| 7 | `loop_host_part_b_dispatch.cpp` | 817 | MEGA B: interA socket read + host_gotmessage + player_add + username | OPENS if(x3) | ⚠️ |
| 8 | `loop_host_part_b_housestore_restore.cpp` | 981 | house-storage restore (housestoragerestore0/1/2) | CONTINUES | ⚠️ |
| 9 | `loop_host_part_b_charcreate.cpp` | 659 | ban + character create/restore | CONTINUES | ⚠️ |
| 10 | `loop_host_part_b_admin.cpp` | 664 | sysban + slash/privilege dispatch start | CONTINUES | ⚠️ |
| 11 | `loop_host_part_b_admin_exec.cpp` | 1084 | privilege-change + slash-command exec/gating | CONTINUES | ⚠️ |
| 12 | `loop_host_part_b_slash.cpp` | 727 | anotherslashcommand (/WHO, debug backtick, …) | CONTINUES | ⚠️ |
| 13 | `loop_host_part_b_chat_npc.cpp` | 1187 | chat/talk + NPC conversation (npccon_cull) | CONTINUES | ⚠️ |
| 14 | `loop_host_part_b_leave_resurrect_house.cpp` | 1331 | leave/resurrect/buyhouse/ship + doneclmess + interB | CONTINUES+CLOSES B | ⚠️ |
| 15 | `loop_host_part_c_objupdate.cpp` | 407 | MEGA C: object update + powderkeg/explosions | OPENS if(ouln)/for(i) | |
| 16 | `loop_host_part_c_crtenum.cpp` | 740 | summoned removal + creature enum/target (crtenum_*) | CONTINUES | |
| 17 | `loop_host_part_c_npccast.cpp` | 733 | NPC spellcasting (castheal/cure, cast_spell1/2), paidnpc | CONTINUES | |
| 18 | `loop_host_part_c_traps_schedule.cpp` | 759 | traps/poison + schedules (SCHFND_retry, gotschedule) | CONTINUES | |
| 19 | `loop_host_part_c_npcwalk_follow.cpp` | 971 | npc_walk + follow/path | CONTINUES | |
| 20 | `loop_host_part_c_npcattack_a.cpp` | 981 | slidemove + npcattack2hnd | CONTINUES | |
| 21 | `loop_host_part_c_npcattack_b.cpp` | 829 | crossbow0 + oilflask_new | CONTINUES | |
| 22 | `loop_host_part_c_npcattack_c.cpp` | 609 | hth_* + spellattcrt0 + attackdone + npcattackclose | CONTINUES+CLOSES C | |
| 23 | `loop_host_part_d_open.cpp` | 272 | MEGA D: input open + resurrect-followers + switchparty | OPENS for(tpl)/if | ⚠️ |
| 24 | `loop_host_part_d_cast.cpp` | 1044 | cast_spell0 + staffcast0 + nextspell3/4 | CONTINUES | ⚠️ |
| 25 | `loop_host_part_d_chainbolt.cpp` | 1062 | chainbolt_* + spellattcrt1 | CONTINUES | ⚠️ |
| 26 | `loop_host_part_d_summon_untrap.cpp` | 836 | summon/conjure + untrap + pickpocket | CONTINUES | ⚠️ |
| 27 | `loop_host_part_d_weather_mass.cpp` | 873 | wind/weather + mass spells + spelldone | CONTINUES | ⚠️ |
| 28 | `loop_host_part_d_joinitem_wep.cpp` | 876 | joinitem + weapon equip (wep_*_skip) | CONTINUES | ⚠️ |
| 29 | `loop_host_part_d_lockpick_drop.cpp` | 740 | lockpick + drop + checkvortexcube | CONTINUES | ⚠️ |
| 30 | `loop_host_part_d_skiff.cpp` | 590 | skiff drop/pickup | CONTINUES | ⚠️ |
| 31 | `loop_host_part_d_use_a.cpp` | 723 | item-use dispatch 1 (retryuse, usehorse_*) | CONTINUES | ⚠️ |
| 32 | `loop_host_part_d_use_b.cpp` | 591 | item-use dispatch 2 (useinventoryitem, magicfan_used) | CONTINUES | ⚠️ |
| 33 | `loop_host_part_d_look.cpp` | 836 | look/examine (inventory_look, houselook_*, horsepaperslook_*) | CONTINUES | ⚠️ |
| 34 | `loop_host_part_d_ship.cpp` | 487 | ship attack (shipattack, shipattack2) | CONTINUES+CLOSES D | ⚠️ |
| 35 | `loop_host_part_tail_npcframe.cpp` | 481 | NPC-frame encode + oul cleanup + #ifndef _DEBUG quit + close | CONTINUES+CLOSES `if(NEThost){` | ⚠️ |

**Over-size exceptions (≳1,000 lines).** Seven parts run slightly over because
they are deeply-nested goto regions with no interior brace seams (splitting
further would orphan structure): `part_a_save` (1084), `part_a_housestore`
(1052), `part_b_admin_exec` (1084), `part_b_chat_npc` (1187),
`part_b_leave_resurrect_house` (1331), `part_d_cast` (1044), `part_d_chainbolt`
(1062). A future modernization pass (not this relocation) could restructure
them.

## Stale docs (cite `loop_host.cpp:<line>` — the monolith is gone)

The monolith was `git mv`'d and decomposed; any `loop_host.cpp:<line>`
reference no longer resolves. Find the symbol/label in the matching part above
(or grep `src/server/loop/`). Known stale referers:

- `docs/admin/README.md` (lines 24, 55, 68, 87, 91, 105, 143–149) → admin
  commands now live in `part_b_admin`, `part_b_admin_exec`, `part_b_slash`.
- `docs/housing/guardian-guild.md` (~143, ~156) → housing logic in
  `part_b_housestore_restore` / `part_d_*`.
- `README.md` (334, 371) → "host loop" is now `src/server/loop/` (umbrella
  `loop_host_all.cpp`).
- `docs/map_rendering/README.md` (11, 58) → resync/teleport logic in the
  `part_d_*` input parts.

## Tooling

- `tools/loop_split_scan_host.ps1` — **host-aware** brace-seam + goto-target
  scan (preprocessor-blind `loop_split_scan.ps1` miscounts this file). Use
  `-Boundaries`, `-SeamLevel`, `-StartLine/-EndLine` for windowed mega-block
  interior scans.
- `tools/loop_split_extract.ps1` — byte-faithful line-range move between parts.
- `tools/loop_split_banner.ps1` — **safe** raw-byte banner prepend. Use this,
  **never `insert_edit_into_file`**, on the brace-seam parts: the IDE edit tool
  tries to brace-balance these intentionally-unbalanced files and corrupts them.
- `tools/loop_split_oracle_host.ps1` — token-stream regression oracle (the
  "pure relocation" check) for the `host` TU (`-DHOST -DCONSOLE`); pass `-Both`
  for the `both` TU (`-DHOST -DCLIENT`) secondary oracle. Run from a VS x86 dev
  shell; expect `OK: … token stream matches baseline`.

## Editing rules

- **Pure relocation only.** Moving code must not change the preprocessed token
  stream. Validate with `tools/loop_split_oracle_host.ps1` after every cut.
- Tag code comments and PR titles with the phase ID (`LHS-P4.5:`, etc.).
- Docs that cite `loop_host.cpp:<line>` are stale once the monolith is gone;
  find the symbol/label in the matching part below.

