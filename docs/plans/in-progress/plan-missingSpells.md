# Missing Spell Implementation (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update a task's marker when its state changes; append `_(date: note)_` on completion.

> Source of truth: this file. Reference phase IDs in commits/PR titles (e.g. `MSP-P2.3: ...`).
> Companion docs: `../../gameplay/spells.md` (implemented-spell mechanics + shared roll
> pattern), `../../gameplay/ultima-vi-online-missing-spell-summary.md` (original-game behavior).

## Goal

Implement the 17 spells defined in the spell tables (`../../../src/common/setup_both_basic.h`) but
missing from the host cast path, so every castable spell either works or is deliberately
redesigned for multiplayer. Rebalance where the original single-player design is an exploit
or griefing vector in an MMO (Armageddon, Magic Lock, Vanish, Clone, Animate).

## Non-goals

- No new wire message types or `U6O_VERSION` bump — every spell below must be expressible
  with existing packets (SF effects, txt messages, existing update packets). If a spell
  truly needs a new packet, stop and split it into its own plan.
- No changes to already-implemented spells except the explicitly listed bug decision (MSP-P0.3).
- No client-side rendering work beyond reusing existing SF effect types.

## How the cast path works (read before coding)

- Dispatch: `../../../src/server/loop/loop_host_part_d_cast.cpp` (+ `_summon_untrap.cpp`,
  `_chainbolt.cpp`, `_weather_mass.cpp`) — a chain of
  `if (CASTSPELL_SPELLTYPE == ((circle << 4) + slot)) { ... goto spelldone; }` blocks.
  These are **brace-seam loop fragments**: edit with `replace_string_in_file` (tight
  context) only — **never** `insert_edit_into_file`.
- Result code `i2`: `1` success, `2` invalid target ("Can't cast there!"), `3` fail roll
  (still consumes MP/reagents), `0` unimplemented. Set `i2 = 2; //implemented!` at branch top.
- Shared epilogue at `spelldone:` in `loop_host_part_d_weather_mass.cpp` handles MP,
  reagents, and cast delay — do NOT duplicate that logic in branches.
- **Preferred structure for new spells:** put the logic in a helper in
  `../../../src/common/spell_code/spell_code.cpp` (macro `U6O_SPELL_FUNCTION` in `spell_code.h`,
  returns `SPELL_SUCCESS/FAILURE/INVALID/NOTDONE` from `../../../src/server/define_host.h`) and call
  it from a minimal dispatch branch: `i2 = spell_xxx(tplayer, tnpc, NULL, tpx + x, tpy + y);`
  This keeps the fragile fragments small. Stubs already exist for: `detect_magic`,
  `infravision`, `reappear`, `trap`, `vanish`, `armageddon`.
- Roll convention (keep it): success = `floor(rnd*(8 + 5*circle)) < floor(rnd*(INT + N))`;
  magnitude = `floor(rnd*(base + INT-scaling))`. Match the tables in `spells.md`.
- Useful existing state: creature/npc `flags` bits `1`=poison `2`=invisible `4`=asleep
  `8`=paralyzed `16`=fear `32`=charmed (`data_both.h`); `charm` wear-off timer;
  player `invisibility` ticks; global light `timelval` (`globals.inc`, 0=bright 15=dark).
- After each spell: build `host`, run the `both` target, cast the spell in-game with and
  without reagents/MP/level, on valid and invalid targets. Update `spells.md` rows
  (Implemented + behavior/example) in the same commit.

## MSP-P0 — Groundwork (do FIRST)

- ⬜ **MSP-P0.1** Add a `flee` behavior hook: the `16`=fear flag exists on creatures but
  grep whether any AI reads it. If unread, add minimal host-AI handling (feared creature
  moves away from its target, decrementing a fear timer field or reusing `charm`-style
  wear-off). Needed by Fear and RepelUndead. No wire change — movement is already synced.
- ⬜ **MSP-P0.2** Add a curse effect representation: pick storage for a temporary stat
  debuff (suggest a `curse` timer byte parallel to `charm` on `struct npc`/`creature` —
  **check `struct player` byte-blit/wire coupling first**; if adding a field to a
  wire-blitted struct is required, store curses host-side in a parallel array instead).
  Needed by Curse and MassCurse.
- ⬜ **MSP-P0.3** Decision task: `spell_telekinesis` has an `(int)rnd * (...)` precedence
  bug making its rolls always 0 (documented in `spells.md`). Decide with the maintainer:
  fix (behavior change — players will start failing telekinesis) or keep. Record decision
  here either way; do not silently fix.
- ⬜ **MSP-P0.4** Sweep `setup_both_basic.h` for the 17 spells' `spelltarget[]` values;
  set/verify targeting mode per spell (self/tile/creature) so the client prompts correctly.
- **Exit:** fear + curse mechanisms exist and are host-only (no wire/layout change);
  targeting table verified; P0.3 decision recorded.

## MSP-P1 — Information spells (low risk, no combat balance)

- ⬜ **MSP-P1.1** **DetectMagic** (C1): target object; success roll `13` vs `INT+1`.
  Report via `NET_send(NETplayer, ...)` txt whether the object is magical (enchanted staff
  charges via `more2`, magic door/chest variants, wands) and remaining charges. Fill in
  helper `spell_detect_magic`.
- ⬜ **MSP-P1.2** **Seance** (C5): redesign for MMO — original talked to dead NPCs. Here:
  target a body (type 339) or grave; success roll `33` vs `INT+1`; if the body maps to a
  converse-capable NPC, open its conversation with a "ghostly" prefix line; otherwise emit
  flavor text. Must NOT resurrect and must NOT work on player bodies (info-leak/harassment
  vector — a killer could interrogate victims).
- **Exit:** both castable in-game; `spells.md` rows updated.

## MSP-P2 — Utility/object spells (dupe-risk group — read exploit notes)

- ⬜ **MSP-P2.1** **Vanish** (C2) + **MSP-P2.2 Reappear** (C2): implement as a pair in one
  effort. Vanish hides a *simple, non-container, non-quest* world object (reuse the
  invisible-object bit `info & (1<<9)` inverse or a dedicated hidden marker) with a
  host-side timer; Reappear (or timer expiry) restores it.
  **Exploit rails:** never valid on objects inside housing/storage zones you don't own,
  quest items (`info & 256`), containers with contents, or anything another player is
  interacting with — vanish must not become item-griefing or a theft-flag bypass
  (apply `stealing()` rules as if taking the object). Object must persist through `.sav`
  identically when not vanished — prefer a temp flag that auto-clears, not a layout change.
- ⬜ **MSP-P2.3** **Trap** (C2): place a magical trap (reuse trap object type 173, marked
  temp `info |= 112`) on a door/chest tile. Cap: one active caster-placed trap per player
  (track host-side) to prevent trap-carpet griefing; trap damage uses the shared
  creature-attack path so kill credit/karma works.
- ⬜ **MSP-P2.4** **MagicLock** (C3): lock a door/chest by switching it to the magic-lock
  variant (inverse of the implemented UnlockMagic).
  **Exploit rail (explicit in the summary doc):** the original allowed MagicLock →
  UnlockMagic to strip an *ordinary* lock. Prevent: only apply to currently-UNLOCKED
  plain doors/chests, and make UnlockMagic on a magic-locked object restore its exact
  pre-lock variant (store original variant in the object's spare bits, not a new field).
  Also refuse in housing zones not owned by the caster (door-griefing).
- ⬜ **MSP-P2.5** **Animate** (C4): redesigned-lite — original animated arbitrary objects
  with retained abilities (wand attacks = balance hazard). Here: animate a small whitelist
  of mundane objects (e.g. brooms/chairs) into a weak friendly temp creature
  (`respawn_x = 1024` despawn pattern from Conjure), consuming the object. NO ability
  retention, NO animating stacked/valuable items (dupe rail: object is removed before the
  creature spawns, single commit of both steps).
- **Exit:** all five castable; each exploit rail verified by attempting the exploit in-game.

## MSP-P3 — Debuff spells

- ⬜ **MSP-P3.1** **Curse** (C3): single creature; success roll `23` vs `INT+1`; on
  success apply the MSP-P0.2 curse state — suggested effect: attack/defense rolls and INT
  treated as reduced ~25% while cursed, duration `floor(rnd*(32+(INT>>1)))` ticks. SFX
  type 14 pattern.
- ⬜ **MSP-P3.2** **MassCurse** (C7): 5×5 diamond AOE version (match MassSleep's loop
  shape); per-target roll `43` vs `INT+10`. Like the original, hits friends AND foes in
  the area — document that in `spells.md` (it's the balance counterweight).
- ⬜ **MSP-P3.3** **Confuse** (C6): AOE (5×5 diamond); per-target roll `38` vs `INT+1`;
  on success clear the creature's current target + add wander/no-aggro for
  `floor(rnd*(32+(INT>>1)))` ticks (reuse the fear/flee hook minus the flee vector, or
  clear-target + `wait` bump if a no-aggro timer is too invasive). Creatures only — never
  affects players.
- ⬜ **MSP-P3.4** **Fear** (C7): AOE; per-target roll `43` vs `INT+10`; sets flag 16 +
  fear timer from MSP-P0.1 on hostile creatures. Immunity: undead/daemon list
  (352/368/373/367) are fearless.
- ⬜ **MSP-P3.5** **RepelUndead** (C3): AOE (screen-range, e.g. `|dx|+|dy| <= 10` like
  Reveal); affects ONLY undead family (skeleton 352, ghost 373, plus gazer/undead types —
  audit creature table first); sets fear flag + timer, roll `23` vs `INT+10` per target.
- **Exit:** all four castable; feared creatures visibly flee; curse measurably reduces
  hit rate (test vs a training dummy creature); PvP: none of these apply to player
  characters directly (creatures/NPCs only) — verified.

## MSP-P4 — Combat/summon spells

- ⬜ **MSP-P4.1** **Clone** (C6): target creature (NOT players — dupe/harassment rail);
  success roll `38` vs `INT+1`. Spawn a copy of the creature's type with Conjure-pattern
  temp fields (`respawn_x = 1024`, friendly `al = 4` toward caster, hidden bag 188).
  **Exploit rails:** clone carries NO copy of the original's inventory/gold (empty bag),
  is temporary, and creature types ≥ boss/quest tier are excluded (reuse Replicate's
  `>= 342`-style blacklist judgment — audit which types are safe).
- ✅ **MSP-P4.2** **WingStrike** (C7): multi-target damage in a 3-wide line/cone toward
  the target. Implemented as a directed Chebyshev-distance swath (center + ±1 perp per
  step, up to 8 steps). `dmg = floor(rnd*(64 + 2*INT))` per hit; `0` roll skips tile.
  No elemental immunities (physical). SFX type 24, `spelltarget = 1`. Dispatch block in
  `loop_host_part_d_cast.cpp`; `spellattcrt1` routing added in `loop_host_part_d_chainbolt.cpp`;
  `spelltarget[(6<<4)+8] = 1` added in `setup_both_basic.h`. _(date: 2026-08-19)_
- **Exit:** both castable; clone of a gold-carrying creature drops nothing on death.

## MSP-P5 — World-state spells (highest blast radius, do LAST)

- ⬜ **MSP-P5.1** **Infravision** (C2): player buff timer (parallel to `invisibility`
  ticks — same storage caution as MSP-P0.2). Effect: caster's client renders creatures
  in darkness. Investigate first: how does the host cull mover visibility by light?
  If visibility is client-side only, this may need the buff value sent in an existing
  player-update packet — find one with spare bits; if none exists, ⏭ defer and note here
  (new packet = out of scope per Non-goals).
- ⬜ **MSP-P5.2** **Eclipse** (C8): world darkness — original mainly reduced monster
  sight range. Implement as a timed host-side push of `timelval` toward darkness
  (light updates already reach clients — verify via the weather/time update path before
  coding), auto-reverting after `floor(rnd*(64+INT))` ticks. **MMO rail:** cap duration
  (~1 game hour), no stacking (recast while active = fail), and it affects the whole
  shard — consider region-local only if `timelval` push proves too disruptive; record
  the choice here.
- ⬜ **MSP-P5.3** **Armageddon** (C8): the original irreversibly killed nearly all life —
  **never replicate in an MMO** (summary doc flags it as game-ending). Redesign: a
  dramatic, self-sacrificial AOE — massive Tremor-style damage in a large radius around
  the caster, kills the caster, heavy karma penalty, long cooldown via `wait_cast`;
  NO effect on players outside consensual-PvP rules, NO NPC/quest-giver deaths
  (use the Resurrect-style sacred/quest-zone exclusions). Requires all 8 reagents
  (mask 255 already in table). Alternatively ⏭ keep unimplemented — record the
  maintainer's choice here before coding.
- **Exit:** each spell's world-state change reverts cleanly on timer; host restart
  mid-effect leaves no stuck darkness/dead zones (`.sav` untouched by transient state).

## Standing rules for every task

1. Helper-first: logic in `spell_code.cpp`, thin branch in the loop fragment
   (`replace_string_in_file` only in fragments).
2. No `U6O_VERSION` bump, no `struct player` layout change, no `.sav` format change.
   Transient spell state lives host-side.
3. Same commit: code + `spells.md` row update (+ this file's checkbox) + removal of the
   spell from the "Unimplemented spell backlog" list in `spells.md`.
4. Test matrix per spell: no reagents / no MP / low level / invalid target / valid target /
   fail roll path (`i2 == 3` still consumes MP+reagents) — run the `both` target.
5. Modernize as you go (named constants for object types you touch, Doxygen on new
   helpers) per repo conventions; new shared constants → `define_both.h`.

## Session handoff

> **READ THIS FIRST when picking up the plan in a new session.**

- **2026-08-12 (initial draft)** — Plan created from `../../gameplay/spells.md` +
  `../../gameplay/ultima-vi-online-missing-spell-summary.md` audit. Nothing started.
  Next step: branch `plan/missingSpells`, move this file to ``,
  start at **MSP-P0.1**. Key call-site facts already researched: dispatch pattern and
  `spelldone:` epilogue are in `loop_host_part_d_cast.cpp` / `_weather_mass.cpp`;
  `SPELL_*` result constants are in `../../../src/server/define_host.h`; helper stubs for 6 of
  the 17 spells already exist in `spell_code.cpp`.

- **2026-08-19 (MSP-P4.2 WingStrike)** — Implemented WingStrike `(6<<4)+8` as a
  directed 3-wide physical line attack in `loop_host_part_d_cast.cpp` (block after
  FlameWind); `spellattcrt1` routing added to `loop_host_part_d_chainbolt.cpp`;
  `spelltarget[(6<<4)+8]=1` added to `setup_both_basic.h`. Builds cleanly (`host`
  and `both`). `spells.md` row and backlog updated.
  Next open MSP-P4 task: **MSP-P4.1** (Clone).

