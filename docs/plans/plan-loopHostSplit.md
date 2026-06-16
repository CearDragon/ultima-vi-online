# `loop_host.cpp` Decomposition Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before the next phase starts.

> Source of truth: this file. Tag PR titles and code comments with the phase ID
> (`LHS-P3.2:`, etc.).
> Phase ID prefix: **`LHS-P*`** ("Loop-Host Split").
> Sibling plan (already complete, read it for the mechanics): the client split
> `docs/plans/plan-loopClientSplit.md` (phase prefix `LCS-P*`). This plan reuses
> the same tooling and the same "pure relocation" discipline, with the
> host-specific differences called out below.

---

## 0. Why this plan exists

`src/server/loop_host.cpp` is **26,875 lines / ~1.1 MB** (2026-06-16 snapshot).
It is too large for AI agents (and humans) to read or edit reliably — the
`read_file` tool refuses it outright ("File too large"), so it can only be
inspected through windowed terminal reads. This plan breaks it into smaller,
logically-grouped files under **`src/server/loop/`**.

**This is a mechanical relocation, NOT a refactor.** No behaviour changes, no
control-flow rewrites, no de-`goto`-ing, no STL, no signature changes, no
modernization. We only *move* contiguous line ranges into new files and
`#include` them back in the exact same order. A separate future plan can
modernize the code.

### Critical facts about this file (read before touching anything)

These were verified by analysis on 2026-06-16; re-verify if the file changed.

1. **It is not a translation unit.** It is a raw statement block `#include`d
   into the middle of the main loop in `src/common/u6o7.cpp:691`, inside a
   `#ifdef HOST` / `if (NEThost){ … }` guard (u6o7.cpp:689–693). It has no
   top-level function and relies on locals declared in `u6o7.cpp` (`t`, `t2`,
   `t3`, `i`, `i2`, `x`, `y`, `et`, `ett`, `tpl`, `tplayer`, `myobj`, …).
   The include site looks like:

   ```cpp
   #ifdef HOST
   if (NEThost){
   #include "loop_host.cpp"
   }
   #endif
   ```

2. **The ENTIRE file is one `if (NEThost) { … }` block.** This is the single
   biggest difference from the client split. The file opens at line 1 with
   `#include "define_both.h"` (an auto-completion helper, guarded, compiles to
   nothing), then at lines 5–6:

   ```cpp
   if
   (NEThost) {
       //host
   ```

   …and closes at line 26875 with `} //NEThost!=NULL`. (This is a *second*,
   nested `if (NEThost)` — the outer one is u6o7.cpp's own guard.) Because the
   whole body is wrapped in this one block, **there are NO brace-depth-0
   interior cut points** the way the client file had ~30 of them. Every chunk
   we create is a **brace-seam** chunk: the first part OPENS the
   `if (NEThost) {`, the last part CLOSES it, and everything in between
   CONTINUES it. There is no "easy depth-0 head/tail" phase like the client's
   LCS-P2 — the head and tail are just the shallowest depth-1 regions.

3. **`goto` is pervasive (315 labels) but there are NO live cross-file gotos.**
   The host-aware scan flags four "external-looking" goto targets; all four are
   **dead** (commented out), so after concatenation every live `goto` resolves
   in-TU with no external ordering constraint:
   - `badhorseswap` — both occurrences `//`-commented (14272, 14327).
   - `U6Ohostlink1` — both occurrences `//`-commented (11049, 11050).
   - `CLIENT_donemess` — both `goto`s (638, 25854) are inside `/* … */` block
     comments (dead). The label itself lives in the client loop; never make
     these live.
   - `triplecrossbow_miss2` — the `goto` (17347) is inside a `/* … */` block
     comment (dead); no such label exists.
   Unlike the client file (which jumps to `mainloop` in `u6o7.cpp`),
   `loop_host.cpp` has **no** live jump out of the file. This makes ordering
   constraints looser than the client's, but the **brace-seam discipline is
   stricter** (see fact 2).

4. **The naive brace scanner miscounts — a host-aware scan is required.** The
   file has four preprocessor blocks; one of them breaks a preprocessor-blind
   brace count:
   - `#ifdef CLIENT … #endif` at 1004–1011 (self-balanced, harmless).
   - `#ifdef CLIENT … #else … #endif` at **1032–1041** — **both** branches open
     a `{` (lines 1035 and 1039) but only one `}` closes it. A blind counter
     over-counts by +1 here, which is why `tools/loop_split_scan.ps1` reports a
     bogus "final brace depth 1" and an **empty** depth-0 boundary list.
   - `#ifdef CLIENT … #endif` at 4830–4836 (self-balanced, harmless).
   - `#ifndef _DEBUG … #endif` at 26852–26871 (self-balanced, harmless).
   A scan that follows the **host** compile branch (skip `#ifdef CLIENT`, keep
   `#else`/`#ifndef CLIENT`) yields the correct result: **final depth 0** and
   **30 depth-1 seams**. The `both` target (which defines `CLIENT` too) takes
   the *other* branch at 1032–1041 but is also balanced; the depth-1 seam line
   numbers are identical in both configs because that block sits deep inside
   mega-block A, far from any depth-1 seam.

5. **FOUR mega-blocks dominate the file** — each is a single depth-1 child
   statement of the outer `if (NEThost) {`, so each needs its own *interior*
   (depth-2+) brace-seam split (LHS-P3…P6), exactly like the client's two
   mega-blocks needed LCS-P3/P4.

### Structure map (2026-06-16 snapshot)

Host-aware depth-1 seams (the `}` lines that close a direct child statement of
the outer `if (NEThost) {`):
`22, 48, 81, 97, 114, 127, 140, 164, 189, 213, 227, 237, 248, 263, 4105, 4120,
4206, 4248, 11056, 11069, 11076, 11100, 11242, 11326, 11521, 17520, 26403,
26841, 26850, 26870`. Final host-aware depth: **0**.

| Region | Lines (approx) | Size | Role | Notes |
|---|---|---|---|---|
| **Head** | 1–263 | ~260 | block open + per-tick housekeeping: economy adjust, house payments, `leverchange0`, `negatemagic_field`, `nomidichange` (MIDI) | OPENS `if (NEThost) {` |
| **MEGA A** | 266–4105 | ~3,840 | `for (tpl…)` "create and send UPDATE message": save/login/password (`save_update1`, `gotprevsave`, `save_empty`), house storage (`housestorageadd0/1`), `housescan`, **sobj buffer build** (`sobj_copyloop`, `screenplus1check`, `objbuffer`), **mover add/remove + lighting** (`mvobjskip`, `mover_add_*`, `MV_LIGHTBRIGHT_assumed`), `display_override`, `flash`, `sfxglobal` | ⚠️ **WIRE** (sobj/mover encode) |
| inter | 4106–4248 | ~140 | `free_socketclient`, `nextclientmessage` | small |
| **MEGA B** | 4249–11056 | ~6,810 | client message dispatch from `host_gotmessage:`: `player_add`, username/password format, character create/restore (`save_createcharacter`, `save_restored`), house-storage restore, ban/unban, **privilege/admin commands** (`privelegechange_*`, `/GRANT*`, `anotherslashcommand`), chat/talk + NPC conversation (`npcmess_shrine*`, `npccon_cull`), buy/sell (`buy_*`, `sell_*`, `buyhorse_*`), leave-britannia, resurrect, buyhouse, ship attack | ⚠️ **WIRE** (reads client messages) + admin |
| inter | 11057–11521 | ~465 | `addsaveclear`, `addsaveobj`, `tempcheck`, `respawn_goldscan` | small-ish |
| **MEGA C** | 11522–17520 | ~6,000 | `if (ouln!=-1){ for(i…){ … } }` object/creature/NPC AI update: powderkeg/explosions, creature enumeration + targeting (`crtenum_*`), NPC spellcasting (`castheal*`, `castcure*`, `cast_spell*`), npc actions, traps/poison, schedules (`SCHFND_retry`, `gotschedule`), `npc_walk`, follow/path, NPC attack (`npcattack2hnd`, `crossbow0`, `oilflask_new`, `hth_*`, `spellattcrt*`) | ⚠️ **AI HOT PATH** |
| **MEGA D** | 17523–26403 | ~8,880 | `for (tpl…)` "process INPUT" (largest): movement, player spellcasting (`cast_spell0`, `staffcast0`, `chainbolt_*`, immune checks), summon/conjure/replicate, untrap, pickpocket, weather/mass spells, **item-use dispatch** (`retryuse`, `use_getprevitem`, `usehorse_*`, `useinventoryitem`, `magicfan_used`), look/examine (`inventory_look`, `houselook_*`, `horsepaperslook_*`), drop, lockpick, ship attack | ⚠️ **WIRE** (input) + huge |
| **Tail** | 26404–26875 | ~470 | `for (tpl…)` "send NPC-frame update to client" (mover/mobj encode), `oul` cleanup, `#ifndef _DEBUG` 24h-quit, `autoexit_deny:`, closing `} //NEThost` | ⚠️ **WIRE** (mover/mobj encode); CLOSES the block |

> ⚠️ These line numbers WILL drift the moment the file is edited. Always re-run
> the host-aware scan (LHS-P0.2) immediately before making a cut, and cut on the
> *current* seam, not the number written here.

---

## LHS-P0 — Safety net & tooling (do FIRST, no moves yet)

- ⬜ LHS-P0.1 Capture a baseline `host` (and `both`) build; record the exact
  warning count. As with the client (see LCS-P0.1), the MSVC debug EXE is **not
  byte-stable** across rebuilds (`/ZI` PDB GUID + PE timestamp), so a
  binary-hash oracle is unusable. Instead create a **host preprocessor-output
  oracle**: `tools/loop_split_oracle_host.ps1` (copy of
  `tools/loop_split_oracle.ps1` with the host defines), preprocessing the host
  TU with:

  ```text
  cl /nologo /EP /TP -DHOST -DCONSOLE -DWIN32 -D_DEBUG -D_WINDOWS \
     -Isrc\common\include -Isrc\common -Isrc\client -Isrc\server src\common\u6o7.cpp
  ```

  (Note `-DHOST -DCONSOLE` and **no** `-DCLIENT` — these are the `host` target's
  compile definitions from `CMakeLists.txt`. The `#ifdef HOST` guard at
  u6o7.cpp:689 means HOST must be defined for the host loop to appear in the
  preprocessor output at all.) Save the whitespace-normalized token-stream
  SHA256 to `tools/loop_split_host_oracle_baseline.sha256`. **Every later phase
  must re-preprocess and match this hash.** Optionally also snapshot the `both`
  TU (`-DHOST -DCLIENT`) as a secondary oracle, since `both` exercises the
  `#ifdef CLIENT` branches that pure `host` does not.

- ⬜ LHS-P0.2 Add host-aware scanning. The existing `tools/loop_split_scan.ps1`
  is preprocessor-blind and miscounts this file (fact 4 above). Either add a
  `-HostView` switch to it or add a sibling `tools/loop_split_scan_host.ps1`
  that, while walking braces, tracks `#ifdef CLIENT` / `#ifndef CLIENT` /
  `#ifdef HOST` / `#else` / `#endif` nesting and counts braces only in the
  host-active branches (skip `#ifdef CLIENT`, keep its `#else`). Verify it
  reproduces the 2026-06-16 snapshot: **26,875 lines**, **final host-aware depth
  0**, **30 depth-1 seams** = `22, 48, 81, 97, 114, 127, 140, 164, 189, 213,
  227, 237, 248, 263, 4105, 4120, 4206, 4248, 11056, 11069, 11076, 11100,
  11242, 11326, 11521, 17520, 26403, 26841, 26850, 26870`, and the four dead
  external goto targets (`badhorseswap`, `CLIENT_donemess`,
  `triplecrossbow_miss2`, `U6Ohostlink1`). Support a windowed mode
  (`-StartLine`/`-EndLine`, depth tracked relative to the window) so the
  mega-block interior seams (LHS-P3…P6) can be enumerated.

- ⬜ LHS-P0.3 Create the destination directory `src/server/loop/` with a
  `README.md` that states the include-order contract, the
  **whole-file-is-one-block** brace-seam convention (the first part OPENS, the
  last CLOSES), the wire-coupling warning for the A/B/D/tail parts, and links
  back to this plan.

- ⬜ LHS-P0.4 Confirm the `host`/`both` targets reach `loop_host.cpp` via
  `u6o7.cpp` only (grep proves a single `#include "loop_host.cpp"`), record the
  include site (`u6o7.cpp:691`), and confirm `loop_host.cpp` is **not** listed
  as a source in `CMakeLists.txt` (only `u6o7.cpp` is; the host loop is reached
  by `#include`). Confirm the sibling host `.inc` files (`setup_host.inc` at
  u6o7.cpp:382, `host.inc`) are separate include sites and are **out of scope**
  for this plan — do not touch them.

- **Exit:** Baseline build + warning count recorded; host oracle captures a
  baseline hash; host-aware scan reproduces the snapshot above; empty
  `src/server/loop/` exists with a README; include site documented.

## LHS-P1 — Establish the umbrella include mechanism

Goal: replace the single `#include "loop_host.cpp"` in `u6o7.cpp` with a single
`#include "loop/loop_host_all.cpp"` umbrella that, in turn, `#include`s the
chunk files **in order**. This isolates `u6o7.cpp` from future chunk churn.

- ⬜ LHS-P1.1 Create `src/server/loop/loop_host_all.cpp` containing only an
  ordered list of `#include` directives plus a header comment documenting: the
  include-order contract (order is load-bearing; chunks share one block scope;
  never reorder without re-checking goto/label reachability and the brace
  seams), the whole-file-is-one-block convention, the wire-coupled parts, and a
  pointer to this plan.

- ⬜ LHS-P1.2 Move the entire current `loop_host.cpp` body verbatim into
  `src/server/loop/loop_host_part_00.cpp` (a single chunk, no other change) via
  `git mv` (preserves history). Make `loop_host_all.cpp` `#include` just that
  one part.

- ⬜ LHS-P1.3 Point `u6o7.cpp` at the umbrella: change line 691 from
  `#include "loop_host.cpp"` to `#include "loop/loop_host_all.cpp"`. The
  `#ifdef HOST` / `if (NEThost){` guard and the trailing `}` in `u6o7.cpp`
  stay exactly as they are — they wrap the umbrella the same way they wrapped
  the monolith.

- ⬜ LHS-P1.4 Rebuild `host` + `both`. Run the host oracle (LHS-P0.1) → must
  print `OK`. (Token-stream identity is the "pure file-shuffle" invariant; a
  true byte-identical binary is not achievable — see LCS-P0.1.) Fix include-path
  issues (`#include` is relative to the including file's dir; the parts live
  under `loop/` so cross-references stay simple).

- **Exit:** Build green; host oracle `OK`; `u6o7.cpp` includes only the
  umbrella; one giant part file exists and compiles.

## LHS-P2 — Carve off the head & tail (brace-seam OPEN / CLOSE parts)

Because the whole file is one block (fact 2), there is no depth-0 head/tail to
lift out cleanly. Instead, do the two **boundary** parts first to establish the
OPEN/CLOSE seams before tackling the mega-blocks. Use the **brace-seam
convention**:

> **Brace-seam convention.** When an interior cut is needed, the chunk that
> *opens* a brace ends with that brace left **open**, and the next chunk
> *begins* mid-block. Each such file gets a banner comment (prepended with
> `tools/loop_split_banner.ps1`, **never** the IDE edit tool, which
> brace-balances and corrupts these files):
> `// LHS brace-seam: this file OPENS a block closed in <next part>` /
> `// LHS brace-seam: this file CONTINUES a block opened in <prev part>` /
> `// LHS brace-seam: this file CONTINUES + CLOSES a block opened in <prev part>`.
> Cuts are made only at host-aware interior brace seams, never inside a nested
> expression, a `for(;;)` header, a line-split statement, or between a label and
> its statement.

- ⬜ LHS-P2.1 **`part_open_housekeeping`** — lines 1–~263. The `#include
  "define_both.h"` helper line, the block open `if (NEThost) {`, and the head
  per-tick housekeeping (economy adjust, house payments, `leverchange0`,
  `negatemagic_field`, MIDI/`nomidichange`). Cut on the current depth-1 seam at
  ~263 (just before MEGA A's `for (tpl…)`). Banner = **OPENS**. Keep this part
  **first** in the umbrella forever (it opens the block).

- ⬜ LHS-P2.2 **`part_tail_npcframe`** — lines ~26404–26875 (EOF). The
  NPC-frame update `for (tpl…)` loop (mover/mobj encode — ⚠️ WIRE), the `oul`
  cleanup, the `#ifndef _DEBUG` 24h-quit, `autoexit_deny:`, and the closing
  `} //NEThost`. Cut on the current depth-1 seam at ~26403 (end of MEGA D)
  through EOF. Banner = **CONTINUES + CLOSES**. Keep this part **last** in the
  umbrella forever (it closes the block). Move-only — do not touch the encode.

- **Exit:** OPEN part (1) and CLOSE part (1) extracted; each cut keeps the host
  oracle `OK`; the residual part_00 now holds exactly the four mega-blocks +
  the small inter-block statements, bounded by an OPEN part before and a CLOSE
  part after.

## LHS-P3 — Split MEGA A (per-player UPDATE + sobj/mover encode) ⚠️ WIRE

MEGA A (~266–4105, ~3,840 lines) is the per-player `for (tpl…)` UPDATE-message
builder. Same brace-seam convention. Aim for ~5 files of < 1,000 lines each,
grouped by the natural label/section structure. The first A-part CONTINUES the
outer block; the last A-part closes the `for` and CONTINUES the outer block.

- ⬜ LHS-P3.1 Map MEGA A's interior brace seams (windowed host-aware scan on the
  *current* range) and record candidate cut lines + the labels they contain
  (`save_update1`, `gotprevsave`, `save_empty`, `housestorageadd0/1`,
  `housescan0/1`, `sobj_copyloop1`, `screenplus1check_*`, `objbufupdate0`,
  `mvobjskip`, `mover_add_*`, `MV_LIGHTBRIGHT_assumed`, `display_override`,
  `flash`, `sfxglobal`, `sfxglobal2`).
- ⬜ LHS-P3.2 **`part_a_save`** — save/login/password handling. Banner per seam.
- ⬜ LHS-P3.3 **`part_a_housestore`** — house storage add + housescan.
- ⬜ LHS-P3.4 **`part_a_sobj`** — sobj buffer build (`sobj_copyloop`,
  `screenplus1check`, `objbuffer*`). ⚠️ **WIRE** — move-only.
- ⬜ LHS-P3.5 **`part_a_mover`** — mover add/remove + lighting assumptions
  (`mvobjskip`, `mover_add_*`, `MV_LIGHT*_assumed`). ⚠️ **WIRE** — move-only.
- ⬜ LHS-P3.6 **`part_a_display_sfx`** — `display_override`, `flash`, SFX
  (`sfxglobal`, `addsf`, `sfxglobal2`) through the end of MEGA A; closes the
  `for`. Banner = CONTINUES (still inside the outer block).
- ⬜ LHS-P3.7 Rebuild; host oracle `OK`. Note the inter-block statements
  4106–4248 (`free_socketclient`, `nextclientmessage`) stay in the residual
  part for now (fold into LHS-P4's first part).
- **Exit:** MEGA A is ~5 files, each ≲ 1,000 lines (flag exceptions), oracle
  `OK`, brace seams banner-documented, **no encode/decode edits**, `U6O_VERSION`
  untouched.

## LHS-P4 — Split MEGA B (client message dispatch) ⚠️ WIRE + admin

MEGA B (~4249–11056, ~6,810 lines) is the `host_gotmessage:` client-message
dispatch. Aim for ~6–8 files of < 1,000 lines each.

- ⬜ LHS-P4.1 Map MEGA B's interior seams + labels (`host_gotmessage`,
  `player_add`, `getsetupmessageinfo_more`, `check_username_password_ok`,
  `save_createcharacter`, `save_restored`, `housestoragerestore*`,
  `ban_nextplayer`, `sysban_nextplayer`, `privelegechange_*`,
  `anotherslashcommand`, `chat`, `talk_*`, `npcmess_shrine*`, `npccon_cull`,
  `buy_*`, `sell_*`, `buyhorse_*`, `leaveitems`, `resurrect_success`,
  `buyhouse_failed`, `kshipattack`, `doneclmess`).
- ⬜ LHS-P4.2 **`part_b_dispatch`** — message dispatch entry + `player_add` +
  setup-message info. Includes the 4106–4248 inter-block prelude
  (`free_socketclient`, `nextclientmessage`).
- ⬜ LHS-P4.3 **`part_b_charcreate`** — username/password formatting, character
  create/restore (`save_createcharacter`, `save_restored`).
- ⬜ LHS-P4.4 **`part_b_housestore_restore`** — house-storage restore, `sv1/sv2`.
- ⬜ LHS-P4.5 **`part_b_admin`** — ban/unban + privilege/`/`-commands
  (`privelegechange_*`, `anotherslashcommand`, `/GRANT*`, `/WHO`, debug
  backtick). Cross-ref `docs/admin/README.md` (its `loop_host.cpp:NNNN` line
  refs will go stale — record the new part mapping in the loop README).
- ⬜ LHS-P4.6 **`part_b_chat_npc`** — chat/talk + NPC conversation (shrine,
  `npccon_cull`, instruction interpreter).
- ⬜ LHS-P4.7 **`part_b_buysell`** — buy/sell, questitem, buyhorse.
- ⬜ LHS-P4.8 **`part_b_leave_resurrect_house`** — leave-britannia, resurrect,
  buyhouse, ship attack, through `doneclmess` + the small inter-block
  statements 11057–11521 (`addsave*`, `tempcheck`, `respawn_*`).
- ⬜ LHS-P4.9 Rebuild; host oracle `OK`.
- **Exit:** MEGA B is ~6–8 files, each ≲ 1,000 lines (flag exceptions), oracle
  `OK`, seams documented, **no message-format edits**, `U6O_VERSION` untouched.

## LHS-P5 — Split MEGA C (object/creature/NPC AI) ⚠️ HOT PATH

MEGA C (~11522–17520, ~6,000 lines) is the AI update block. ⚠️ Performance-
sensitive — move-only, no STL, no reorg. Aim for ~6–8 files of < 1,000 lines.

- ⬜ LHS-P5.1 Map MEGA C's interior seams + labels (`powderkeg_*`,
  `extractobj6`, `summonedcreature_remove`, `crtenum_*`, `invalidatetarget`,
  `already_paralyzed`, `crtcantcast`, `crt_walk`, `castheal_*`, `castcure_*`,
  `cast_spell1/2`, `npcactiondone`, `paidnpc`, `trap_*`, `poisonremoved`,
  `SCHFND_retry`, `gotschedule`, `npc_walk`, `follow_*`, `slidemove`,
  `npcattack2hnd`, `crossbow0`, `oilflask_new`, `hth_*`, `spellattcrt0`,
  `attackdone`, `npcattackclose`).
- ⬜ LHS-P5.2 **`part_c_objupdate`** — object update entry + powderkeg/
  explosions.
- ⬜ LHS-P5.3 **`part_c_crtenum`** — creature enumeration + targeting
  (`crtenum_*`, target validation).
- ⬜ LHS-P5.4 **`part_c_npccast`** — NPC spellcasting (`castheal*`, `castcure*`,
  `cast_spell*`), npc actions, `paidnpc`.
- ⬜ LHS-P5.5 **`part_c_traps_schedule`** — traps, poison, schedules
  (`SCHFND_retry`, `gotschedule`).
- ⬜ LHS-P5.6 **`part_c_npcwalk_follow`** — `npc_walk`, follow/path, `slidemove`,
  swaps.
- ⬜ LHS-P5.7 **`part_c_npcattack`** — NPC attack (`npcattack2hnd`, `crossbow0`,
  `oilflask_new`, `hth_*`, `spellattcrt0`) through the end of MEGA C (closes the
  `if (ouln!=-1)` / `for`).
- ⬜ LHS-P5.8 Rebuild; host oracle `OK`.
- **Exit:** MEGA C is ~6–8 files, each ≲ 1,000 lines (flag exceptions), oracle
  `OK`, seams documented, **no STL / no hot-path reorg**.

## LHS-P6 — Split MEGA D (per-player INPUT) ⚠️ WIRE + the big one

MEGA D (~17523–26403, ~8,880 lines) is the largest — the per-player INPUT
`for (tpl…)` processor. ⚠️ WIRE (reads/acts on client input). Aim for ~9–11
files of < 1,000 lines.

- ⬜ LHS-P6.1 Map MEGA D's interior seams + labels
  (`resurrectfollowers_failed`, `switchpartymember`, `cast_spell0`,
  `staffcast0`, `ktarcast2`, `nextspell3/4`, `chainbolt_*`, `spellattcrt1`,
  `summonfail`, `conjfail`, `isretry`, `gotreploc`, `untrap_*`,
  `pickpocketcrt*`, `windchange_cast`, `massawaken_npca`, `masssleep_immune`,
  `spelldone`, `joinitem`, `wep_right_skip`, `wep_left_skip`, `lockpick*`,
  `dropallow`, `checkvortexcube`, `skiffdrop`, `skiffpickup`, `retryuse`,
  `use_getprevitem`, `usehorse_*`, `useinventoryitem`, `magicfan_used`,
  `lastchanceuse_*`, `inventory_look`, `houselook_*`, `horsepaperslook_*`,
  `shipattack*`, `nextspell/nextspell2`).
- ⬜ LHS-P6.2 **`part_d_open`** — input loop open + resurrect-followers +
  switch-party.
- ⬜ LHS-P6.3 **`part_d_cast`** — `cast_spell0`, `staffcast`, `chainbolt`,
  immune checks, spell-attack.
- ⬜ LHS-P6.4 **`part_d_summon_untrap`** — summon/conjure/replicate, untrap,
  pickpocket.
- ⬜ LHS-P6.5 **`part_d_weather_mass`** — wind/weather + mass spells +
  `spelldone`.
- ⬜ LHS-P6.6 **`part_d_use_a`** — item-use dispatch part 1
  (`retryuse`…`useinventoryitem`).
- ⬜ LHS-P6.7 **`part_d_use_b`** — item-use dispatch part 2 (`usehorse_*`,
  `magicfan_used`, `lastchanceuse_*`).
- ⬜ LHS-P6.8 **`part_d_look`** — look/examine (`inventory_look`, `houselook_*`,
  `horsepaperslook_*`, sign/grammar).
- ⬜ LHS-P6.9 **`part_d_misc`** — ship attack, drop, lockpick, biofeedback,
  `nextspell/nextspell2` through the end of MEGA D (closes the `for`).
- ⬜ LHS-P6.10 Rebuild; host oracle `OK`.
- **Exit:** MEGA D is ~9–11 files, each ≲ 1,000 lines (flag exceptions), oracle
  `OK`, seams documented, **no input/format edits**, `U6O_VERSION` untouched.

## LHS-P7 — Finalize, document, enforce

- ⬜ LHS-P7.1 Confirm the original `src/server/loop_host.cpp` is gone (it was
  `git mv`'d in LHS-P1.2; no redirect shim). Record a single authoritative
  "docs referencing `loop_host.cpp:<line>` are stale" note with the part mapping
  in `src/server/loop/README.md`, and list the known stale referers:
  `docs/admin/README.md` (lines 24, 55, 68, 87, 91, 105, 143–149),
  `docs/housing/guardian-guild.md` (~143, ~156), `README.md` (334, 371). Rewrite
  those line-number refs only if cheap and safe; otherwise leave the docs
  byte-pristine and rely on the part mapping + symbol/label grep (mirrors
  LCS-P5.1).
- ⬜ LHS-P7.2 (optional, may defer like LCS-P5.2) Add the new
  `src/server/loop/*.cpp` parts to the `host` and `both` targets'
  `Source_Files`/`Header_Files` lists in `CMakeLists.txt` for IDE indexing
  (they are compiled only via `#include`, so they become non-compiled members,
  like the existing `.inc` files). Re-verify `client` is untouched.
- ⬜ LHS-P7.3 Re-run the host-aware scan; verify no part exceeds ~1,000 lines
  (flag and justify exceptions — some dense AI/use-dispatch parts may run over),
  every live `goto` target resolves in-TU, and brace depth across the
  concatenated parts still nets to 0.
- ⬜ LHS-P7.4 Finalize `src/server/loop/README.md`: the final part-layout table
  (every part, role, brace-seam role, wire flag), the over-size exceptions note,
  the stale-docs note, and a Tooling section (host oracle invocation + the
  warning to use `tools/loop_split_banner.ps1` / `tools/loop_split_extract.ps1`,
  not the IDE edit tool, on brace-seam parts).
- ⬜ LHS-P7.5 Final build of `host`, `both`, and `client` (must be unaffected —
  `client` does not include `loop_host.cpp`); confirm the host oracle (and the
  optional `both` oracle) still print `OK`.
- **Exit:** Monolith gone; `src/server/loop/` holds the ordered parts + README;
  `host`/`both` build green and host-oracle-identical to baseline; `client`
  unaffected; docs updated.

---

## Invariants (apply to EVERY phase)

1. **Pure relocation.** Never edit code while moving it. A cut/paste must leave
   the preprocessed token stream identical. After EVERY cut, run the host oracle
   (`tools/loop_split_oracle_host.ps1`) from a VS x86 dev shell; it must print
   `OK`. A MISMATCH means the move was not pure — revert and redo. Only
   re-baseline if you *deliberately* change the token stream (you should not
   during this plan).
2. **Every cut is a host-aware brace seam.** There are NO depth-0 cut points in
   this file — the whole thing is one `if (NEThost) {` block. The first part
   OPENS it, the last part CLOSES it, all others CONTINUE it. Never cut inside a
   nested expression, a `for(;;)` header, a line-split statement (this codebase
   splits statements across many lines), or between a label and its statement.
   Always compute seams with the **host-aware** scan (a preprocessor-blind scan
   is wrong here — fact 4).
3. **Order is load-bearing.** All parts share one block scope after
   concatenation. Keep `part_open_housekeeping` first and `part_tail_npcframe`
   last; never reorder parts without re-validating goto/label reachability and
   the brace seams.
4. **Wire-coupled parts (MEGA A sobj/mover encode, MEGA B client-message
   dispatch, MEGA D input, the tail NPC-frame encode) are move-only.** Do not
   touch encode/decode, buffer layout, `MV_TX_*`, `sobj_*`, `tpx_legacy`, or
   introduce STL. Do **not** bump `U6O_VERSION` — this plan changes no wire
   format. (If you find yourself wanting to, you are refactoring, not
   relocating — stop.)
5. **Re-scan before every cut.** Line numbers in this doc are a 2026-06-16
   snapshot and drift as soon as the file is touched.
6. **Never use `insert_edit_into_file` on a brace-seam part.** It brace-balances
   the intentionally-unbalanced files and corrupts them (this bit the client
   split — see LCS-P4.13). Use `tools/loop_split_extract.ps1` for cuts and
   `tools/loop_split_banner.ps1` for banners (raw byte operations).

## Session handoff

- **Plan authored 2026-06-16; execution NOT started.** All phases are ⬜.
- The 26,875-line `src/server/loop_host.cpp` is a single `if (NEThost) { … }`
  block (`#include`d at `u6o7.cpp:691` under `#ifdef HOST`). It decomposes into:
  an OPEN head part, four mega-blocks (A: per-player UPDATE + sobj/mover encode;
  B: `host_gotmessage:` client-message dispatch; C: object/creature/NPC AI;
  D: per-player INPUT — the biggest), and a CLOSE tail part (NPC-frame encode).
- **Start here:** do LHS-P0 (build the **host** oracle
  `tools/loop_split_oracle_host.ps1` with `-DHOST -DCONSOLE`, and the host-aware
  scan), then LHS-P1 (umbrella + `git mv` to
  `src/server/loop/loop_host_part_00.cpp`), then carve P2 boundaries, then split
  P3→P6 bottom-up (extract the *last* sub-part of each mega-block first so line
  numbers above the cut don't drift mid-phase, mirroring the client split).
- **Tooling reuse:** `tools/loop_split_extract.ps1` and
  `tools/loop_split_banner.ps1` work as-is. The scan and oracle need host
  variants (LHS-P0.1/0.2). After every cut, re-run the host oracle from a VS
  x86 dev shell (`vcvarsamd64_x86.bat`).
- **Wire/version:** nothing in this plan changes the wire format; leave
  `U6O_VERSION` alone.

