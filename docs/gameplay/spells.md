# Spells

This document inventories every player-cast spell currently defined in Ultima VI Online,
ordered by circle, with current implementation status and code-level behavior notes.

Primary source files:
- `src/common/setup_both_basic.h` (canonical spell list + reagent masks)
- `src/server/loop/loop_host_part_d_cast.cpp`
- `src/server/loop/loop_host_part_d_summon_untrap.cpp`
- `src/server/loop/loop_host_part_d_weather_mass.cpp`
- `src/server/loop/loop_host_part_d_chainbolt.cpp`
- `src/common/spell_code/spell_code.cpp` (helper spell functions used by a subset)

## Casting rules that apply to all spells

- Spell id format: `(circle << 4) + slot`, where circle/slot are 0-based.
- Circle gate: caster level must be at least `circle + 1`.
- MP cost: `circle + 1` MP.
- Reagents: one unit of each required reagent bit is consumed when cast result is success or fail.
- Range gate: many targeted casts require `abs(dx) <= 8` and `abs(dy) <= 8` from caster screen offset.
- On-host checks such as `stormcloakcheck2(...)`, terrain checks, and LOS checks (`OBJcheckbolt`) may reject targets.
- **Shared roll pattern:** most non-damage spells resolve success with `x3 = floor(rnd*(8 + 5*circle))`
  vs `x4 = floor(rnd*(INT + N))`, succeeding when `x4 >= x3`. This is written per-spell below as
  `floor(rnd*K) < floor(rnd*(INT+N))` (equivalent form). Damage/effect magnitude uses
  `floor(rnd*(base + int_scaling))` where `INT` is the caster's intelligence.
- **Cast delay:** after a cast the caster incurs `wait_cast += 4.0 - (INT/32.0)` — higher INT casts faster.
- SFX / projectile effect "type N" values below refer to the spell-effect id emitted to clients; they do
  not change resolution, only presentation.
- Result codes in cast flow:
  - `1`: success
  - `2`: invalid target / cannot cast there
  - `3`: fail roll (still consumes MP/reagents)
  - `0`: not implemented in host cast path (shows "U6O can't cast this spell yet!")

### How to read the worked examples

- `rnd` is a uniform random float in `[0, 1)`, so `floor(rnd*K)` is an integer in `0 .. K-1`
  (each value roughly `1/K` likely). Where an example needs a concrete draw it states the assumed `rnd`.
- Unless noted, examples assume a **mid-level mage with `INT = 30`** casting on an in-range, valid target.
- `>>` is an integer right shift: `INT>>1 = 15`, `INT>>2 = 7`, `INT>>3 = 3`, `INT>>4 = 1` for `INT = 30`.

## Reagent abbreviations

- `BLAC` = Black Pearl
- `BLOO` = Blood Moss
- `GARL` = Garlic
- `GINS` = Ginseng
- `MAND` = Mandrake
- `NIGH` = Nightshade
- `SPID` = Spider Silk
- `SULF` = Sulfurous Ash

---

## First Circle (circle 0)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | CreateFood | `GARL\|GINS\|MAND` | Implemented | `spell_create_food`: target tile/container check; food amount roll `amount = floor(rnd * (4 + (INT >> 4)))`; creates meat object type 129 with `more2=amount` if `amount>0`. **Example** (`INT=30`): `4 + (30>>4) = 4 + 1 = 5`, so `amount = floor(rnd*5)` in `0..4`; with `rnd=0.6` -> `amount = 3` meat portions. `amount=0` produces nothing. |
| 1 | DetectMagic | `NIGH\|SULF` | **Unimplemented** | No host cast branch; falls through to "U6O can't cast this spell yet!" (result `0`). Helper `spell_detect_magic` returns `SPELL_NOTDONE`. |
| 2 | DetectTrap | `NIGH\|SULF` | Implemented | `spell_detect_trap` (cast at caster position): success roll `floor(rnd*13) < floor(rnd*(INT+1))`; scans a 41x33 rectangle (`x +/-20, y +/-16`); reveals trap objects (type 173) by setting the visible flag `info \|= (1<<9)`; reports the count found to the caster. **Example** (`INT=30`): success needs `floor(rnd_a*13) < floor(rnd_b*31)`; with `rnd_a=0.30`->`3` and `rnd_b=0.40`->`12`, `3 < 12` = success and every trap in the 41x33 box is revealed. |
| 3 | Cure | `GARL\|GINS` | Implemented | Single target. Success roll `floor(rnd*13) < floor(rnd*(INT+1))`. On success clears the poisoned flag (`flags &= ~1`) on the target player NPC; blue-ball SFX (type 14). Otherwise result `3` (fail). **Example** (`INT=30`): `floor(rnd_a*13)` vs `floor(rnd_b*31)`; with `rnd_a=0.9`->`11` and `rnd_b=0.1`->`3`, `11 < 3` is false -> fail (poison remains). |
| 4 | Douse | `GARL\|BLAC` | Implemented | `spell_douse`: valid on brazier (206) / campfire (253) / fireplace (164) / candle (122) / candelabra (145) / powder keg (223); success roll `floor(rnd*13) < floor(rnd*(INT+1))`; toggles lit state (or powder-keg behavior) + blue-bolt SFX (`SF_BLUE_BOLT`). **Example** (`INT=30`): same `13` vs `INT+1=31` roll as Cure; with `rnd_a=0.2`->`2` and `rnd_b=0.5`->`15`, `2 < 15` = success -> flame extinguished. |
| 5 | Harm | `NIGH\|SPID` | Implemented | Single target creature (`target=1`). Damage `dmg = floor(rnd*(24 + (INT>>1)))`; result `3` (fail) if `dmg==0`; blue-ball projectile SFX (type 14) + attack SFX. Damage applied through the shared creature-attack path (`spellattcrt1`). **Example** (`INT=30`): `24 + (30>>1) = 24 + 15 = 39`, so `dmg = floor(rnd*39)` in `0..38`; with `rnd=0.5` -> `19` damage. |
| 6 | Heal | `GINS\|SPID` | Implemented | Single target player NPC. Heal `heal = ((floor(rnd*(16 + (INT>>2))) + floor(rnd*(16 + (INT>>2)))) >> 1)` (average of two rolls); clamps to hp max; result `3` (fail) if `heal==0`; blue-ball SFX (type 14). **Example** (`INT=30`): `16 + (30>>2) = 16 + 7 = 23`; two rolls, say `floor(0.4*23)=9` and `floor(0.7*23)=16`, `heal = (9+16)>>1 = 12` HP. |
| 7 | Awaken | `GINS\|GARL` | Implemented | Wakes sleeping NPC/player targets. Success roll `floor(rnd*13) < floor(rnd*(INT+1))`. For NPCs, clears the sleep schedule state `0x91` (with excluded coordinates 568/570/572/574 at row 364); for players, clears the sleep flag (`flags &= ~4`). Blue-ball SFX (type 14). **Example** (`INT=30`): `floor(rnd_a*13)` vs `floor(rnd_b*31)`; with `rnd_a=0.1`->`1`, `rnd_b=0.3`->`9`, `1 < 9` = success -> target wakes. |
| 8 | Ignite | `SULF\|BLAC` | Implemented | `spell_ignite`: valid on fire objects (206/253/164/122/145/223), powder kegs, and web (53); success roll `floor(rnd*13) < floor(rnd*(INT+1))`; toggles lit state, can arm a powder keg (fuse timer `info \|= 7680`), destroys web; red-bolt SFX (`SF_RED_BOLT`). **Example** (`INT=30`): identical `13` vs `31` roll; a lit result on a powder keg starts its fuse countdown. |
| 9 | Light | `SULF` | Implemented | Single target player NPC buff. `x3 = floor(rnd*(16 + (INT>>2)))`; if nonzero sets `light = min(255, 1 + x3)`. **Example** (`INT=30`): `16 + (30>>2) = 23`, `x3 = floor(rnd*23)`; with `rnd=0.5`->`11`, `light = min(255, 1+11) = 12`. |

## Second Circle (circle 1)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | Infravision | `NIGH\|SULF` | **Unimplemented** | No host cast branch; falls through to "U6O can't cast this spell yet!" (result `0`). Helper `spell_infravision` returns placeholder `0`. |
| 1 | MagicArrow | `SULF\|BLAC` | Implemented | Single target creature (`target=1`). Damage `dmg = floor(rnd*(32 + INT))`; result `3` (fail) if `dmg==0`; arrow projectile SFX (type 2) + attack SFX. Damage applied through the shared creature-attack path. **Example** (`INT=30`): `32 + 30 = 62`, `dmg = floor(rnd*62)` in `0..61`; with `rnd=0.25` -> `15` damage. |
| 2 | Poison | `NIGH\|BLOO\|BLAC` | Implemented | Single target creature. Success roll `floor(rnd*18) < floor(rnd*(INT+1))`; on success sets the poison flag (`flags \|= 1`) on the creature; poison SFX (type 18) + attack SFX. **Example** (`INT=30`): `floor(rnd_a*18)` vs `floor(rnd_b*31)`; with `rnd_a=0.3`->`5`, `rnd_b=0.5`->`15`, `5 < 15` = success -> target poisoned. |
| 3 | Reappear | `SPID\|BLOO\|BLAC` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). Helper `spell_reappear` returns placeholder `0`. |
| 4 | Sleep | `NIGH\|SPID\|BLAC` | Implemented | Single target creature. Duration `x3 = floor(rnd*(32 + (INT>>1)))`; result `3` (fail) if `x3==0`; otherwise `crt->wait += x3` and applies asleep + paralyzed flags (`flags \|= (4+8)`). Sleep SFX (type 20). Immunity list (types 364/427/357/367/362/363/355/352/374/343/347/360/426/368/375/345/365/373) blocks the effect. **Example** (`INT=30`): `32 + (30>>1) = 32 + 15 = 47`, `x3 = floor(rnd*47)`; with `rnd=0.6`->`28`, target's `wait += 28`. |
| 5 | Telekinesis | `BLOO\|MAND\|BLAC` | Implemented | `spell_telekinesis`: valid on lever (268) / switch (174) / crank (288); calls `use_toggle`. Note: current helper uses `(int)rnd * ...` precedence, making both rolls effectively 0, so valid targets always pass. **Example**: on a lever, `(int)rnd` truncates to `0` so `0 < 0` is false but the toggle still fires -- any valid lever/switch/crank in range flips. |
| 6 | Trap | `SPID\|NIGH` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). Helper `spell_trap` returns placeholder `0`. |
| 7 | UnlockMagic | `SULF\|BLOO` | Implemented | Target door (oaken..steel, `type>>10 >= 12`) or chest (base + `3*1024`). Success roll `floor(rnd*18) < floor(rnd*(INT+1))`; on success reverts the magic-lock variant of the door/chest. **Example** (`INT=30`): `floor(rnd_a*18)` vs `floor(rnd_b*31)`; with `rnd_a=0.1`->`1`, `rnd_b=0.6`->`18`, `1 < 18` = success -> lock removed. |
| 8 | Untrap | `SULF\|BLOO` | Implemented | Finds a trap object (type 173) at the target tile stack. Success roll `floor(rnd*18) < floor(rnd*(INT+1))`; on success `OBJremove`s the trap and sends "Trap destroyed!" to the caster. **Example** (`INT=30`): same `18` vs `31` roll as UnlockMagic; success removes the trap object from the tile. |
| 9 | Vanish | `GARL\|BLOO\|BLAC` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). Helper `spell_vanish` returns placeholder `0`. |

## Third Circle (circle 2)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | Curse | `SULF\|NIGH\|GARL` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 1 | DispelField | `BLAC\|SULF` | Implemented | Target field object types 317..320. Success roll `floor(rnd*23) < floor(rnd*(INT+1))`; on success `OBJremove`s the field. **Example** (`INT=30`): `floor(rnd_a*23)` vs `floor(rnd_b*31)`; with `rnd_a=0.4`->`9`, `rnd_b=0.7`->`21`, `9 < 21` = success -> field cleared. |
| 2 | Fireball | `SULF\|BLAC` | Implemented | Single target creature. Damage `dmg = floor(rnd*(48 + INT + (INT>>1)))`; result `3` (fail) if `dmg==0`; fire SFX (type 8). Fire-immunity list (411/369/352/374/368/373/367) sets damage to 0; double-damage list (364/357/347/360/375/365 -- vine/slug/slime family) doubles damage. **Example** (`INT=30`): `48 + 30 + (30>>1) = 48 + 30 + 15 = 93`, `dmg = floor(rnd*93)`; with `rnd=0.5`->`46`. Against a slime (double-damage) -> `92`; against a fire-immune daemon -> `0`. |
| 3 | GreatLight | `SULF\|MAND` | Implemented | Single target player NPC buff. `x3 = floor(rnd*(64 + INT))`; if nonzero sets `light = min(255, 1 + x3)`. **Example** (`INT=30`): `64 + 30 = 94`, `x3 = floor(rnd*94)`; with `rnd=0.5`->`47`, `light = min(255, 48) = 48`. |
| 4 | MagicLock | `SULF\|BLOO\|GARL` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 5 | MassAwaken | `GINS\|GARL` | Implemented | 5x5 diamond AOE. Per-target success roll `floor(rnd*23) < floor(rnd*(INT+10))`; wakes sleepers (clears NPC sleep schedule and clears the player sleep flag). SFX type 19 (area) + type `14 + 4*256`. **Example** (`INT=30`): per target `floor(rnd_a*23)` vs `floor(rnd_b*40)`; with `rnd_a=0.5`->`11`, `rnd_b=0.5`->`20`, `11 < 20` = that target wakes (rolled independently for each sleeper in the diamond). |
| 6 | MassSleep | `GINS\|NIGH\|SPID` | Implemented | 5x5 diamond AOE. For each non-immune creature `crt->wait += floor(rnd*(32 + (INT>>1)))` and applies asleep + paralyzed flags (`flags \|= (4+8)`). Uses the same immunity list as Sleep. SFX type 19 + type `20 + 4*256`. **Example** (`INT=30`): per creature `32 + 15 = 47`, `floor(rnd*47)`; with `rnd=0.4`->`18` added to that creature's `wait`. |
| 7 | Peer | `NIGH\|MAND` | Implemented | Self-target scry. Success roll `floor(rnd*23) < floor(rnd*(INT+10))`; then `peerValue = floor(rnd*(32 + (INT>>3)))` capped to 255; updates the client (msg `d2[0]=48`) only if higher than the current value. **Example** (`INT=30`): on success, `32 + (30>>3) = 32 + 3 = 35`, `peerValue = floor(rnd*35)`; with `rnd=0.8`->`28` (applied only if `> current`). |
| 8 | Protection | `SULF\|GINS\|GARL` | Implemented | Single target player NPC buff. `prot = floor(rnd*(32 + (INT>>1)))` capped 255; applies to `tnpc2->protection` if greater than current. Blue-ball SFX (type 14). **Example** (`INT=30`): `32 + 15 = 47`, `prot = floor(rnd*47)`; with `rnd=0.6`->`28`, sets protection to `28` if that beats the current value. |
| 9 | RepelUndead | `GARL\|SULF` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |

## Fourth Circle (circle 3)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | Animate | `SULF\|MAND\|BLOO` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 1 | Conjure | `SPID\|MAND` | Implemented | Target tile must be land-passable and empty. Success roll `floor(rnd*28) < floor(rnd*(INT+10))`; on success spawns one creature -- `rnd*5` selects rat (342) / snake (358) / bat (344) / troll (371) / spider (361). Spawned creature: `hp = (base v8 >> 8) * 4`, `mp = rnd*9`, `al = 4` (friendly), `respawn_x = 1024` (despawn timer), carries a hidden bag (188). **Example** (`INT=30`): success roll `floor(rnd_a*28)` vs `floor(rnd_b*40)`; on success `floor(rnd_c*5)=2` -> a bat is summoned as a friendly ally. |
| 2 | Disable | `NIGH\|SPID\|MAND` | Implemented | Single target creature. Kill-threshold roll `((floor(rnd*(192+3*INT)) + floor(rnd*(192+3*INT))) >> 1)`; if roll `>= crt->hp` then deals `hp-1` (leaves target at 1 HP). SFX types 1 + 15. **Example** (`INT=30`): `192 + 3*30 = 282`; two rolls `floor(0.5*282)=141` and `floor(0.7*282)=197`, threshold `(141+197)>>1 = 169`. Vs a `120`-HP target, `169 >= 120` -> target dropped to `1` HP. |
| 3 | FireField | `BLAC\|SULF\|SPID` | Implemented | Target passable tile with no blocking field/web/trap stack (317/318/320/173/53). Success roll `floor(rnd*28) < floor(rnd*(INT+1))`; on success creates field object type 317 (`info += (2<<4)`). **Example** (`INT=30`): `floor(rnd_a*28)` vs `floor(rnd_b*31)`; with `rnd_a=0.3`->`8`, `rnd_b=0.6`->`18`, `8 < 18` = success -> fire field placed. |
| 4 | GreatHeal | `GINS\|SPID\|MAND` | Implemented | Single target player NPC. Heal `heal = ((floor(rnd*(64+INT)) + floor(rnd*(64+INT))) >> 1)`; clamps to hp max; result `3` (fail) if `heal==0`; SFX type 14. **Example** (`INT=30`): `64 + 30 = 94`; rolls `floor(0.4*94)=37` and `floor(0.8*94)=75`, `heal = (37+75)>>1 = 56` HP. |
| 5 | Locate | `NIGH` | Implemented | Self-target. Success roll `floor(rnd*28) < floor(rnd*(INT+1))`; computes and reports map-relative latitude/longitude (N/S/E/W) coordinate text from the caster's world position. **Example** (`INT=30`): on a successful `28` vs `31` roll, reports e.g. "12N 30E" derived from the caster's tile. |
| 6 | MassCure | `GARL\|GINS` | Implemented | 5x5 diamond AOE. Per-target success roll `floor(rnd*13) < floor(rnd*(INT+10))`; clears the poison flag on each player NPC in area. SFX type 19 + type 14. **Example** (`INT=30`): per target `floor(rnd_a*13)` vs `floor(rnd_b*40)`; the wide `40` cap makes success very likely, so most allies in the diamond are cured. |
| 7 | PoisonField | `NIGH\|SPID\|BLAC` | Implemented | Target passable tile. Success roll `floor(rnd*28) < floor(rnd*(INT+1))`; on success creates field object type 318. **Example** (`INT=30`): same `28` vs `31` gate as FireField; success drops a poison field on the tile. |
| 8 | SleepField | `GINS\|SPID\|BLAC` | Implemented | Target passable tile. Success roll `floor(rnd*28) < floor(rnd*(INT+1))`; on success creates field object type 320. **Example** (`INT=30`): same `28` vs `31` gate; success drops a sleep field. |
| 9 | WindChange | `SULF\|BLOO` | Implemented | Self-target. Success roll `floor(rnd*28) < floor(rnd*(INT+1))`; rotates the wind vector (`windx`/`windy`) through the 8 compass states and sends a weather update packet (msg `d2[0]=14`). **Example** (`INT=30`): on a successful `28` vs `31` roll, wind steps one notch (e.g. N -> NE). |

## Fifth Circle (circle 4)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | EnergyField | `MAND\|SPID\|BLAC` | Implemented | Target passable tile with no blocking field stack (317/318/320/173). Success roll `floor(rnd*33) < floor(rnd*(INT+1))`; on success creates protection-field object type 319. **Example** (`INT=30`): `floor(rnd_a*33)` vs `floor(rnd_b*31)`; the `33` cap slightly exceeds `INT+1=31`, so success is a near-coin-flip at this INT. |
| 1 | Explosion | `MAND\|SULF\|BLAC\|BLOO` | Implemented | 5x5 diamond AOE. Each target rolls `dmg = floor(rnd*(48 + INT + (INT>>1)))`; fire SFX (type 8). Fire-immunity list (411/369/352/374/368/373/367) zeros damage; double-damage list (364/357/347/360/375/365) doubles it -- same lists as Fireball/FlameWind. **Example** (`INT=30`): `48 + 30 + 15 = 93` per target; with `rnd=0.5`->`46` to each non-immune creature in the diamond (doubled vs slimes, `0` vs fire-immune). |
| 2 | InsectSwarm | `BLOO\|SPID\|SULF` | Implemented | Count `n = floor(rnd*(4 + (INT/16)))`; result `3` (fail) if `n==0`. For each of `n` insects, up to 16 placement retries pick a nearby passable tile (`target +/- 2`); spawns insect creature type 343 (`hp = (base v8 >> 8) * 4`, `mp = rnd*9`, `al = 4`, `respawn_x = 1024`, hidden bag). **Example** (`INT=30`): `4 + (30/16) = 4 + 1 = 5`, `n = floor(rnd*5)` in `0..4`; with `rnd=0.6`->`3` insects spawned (or fail if `n=0`). |
| 3 | Invisibility | `NIGH\|BLOO` | Implemented | Single target player NPC buff. `dur = floor(rnd*(32 + (INT>>1)))` capped 255; applied to invisibility timer if greater than current. SFX type 19. **Example** (`INT=30`): `32 + 15 = 47`, `dur = floor(rnd*47)`; with `rnd=0.7`->`32`. |
| 4 | Lightning | `BLAC\|MAND\|SULF` | Implemented | Single target creature. Damage `dmg = floor(rnd*(64 + 2*INT))`; result `3` (fail) if `dmg==0`; lightning-bolt SFX (type 5) + attack SFX. **Example** (`INT=30`): `64 + 2*30 = 124`, `dmg = floor(rnd*124)`; with `rnd=0.5`->`62` damage. |
| 5 | Paralyze | `SPID\|SULF\|NIGH\|BLAC` | Implemented | Single target creature. `x3 = floor(rnd*(32 + (INT>>1)))`; result `3` (fail) if `x3==0`; otherwise `crt->wait += x3` and sets paralyzed flag (`flags \|= 8`). SFX type 14 + attack. **Example** (`INT=30`): `32 + 15 = 47`, `x3 = floor(rnd*47)`; with `rnd=0.4`->`18` added to target's `wait`. |
| 6 | PickPocket | `BLOO\|SPID\|NIGH` | Implemented | Success roll `floor(rnd*33) < floor(rnd*(INT+1))`. Special NPC case for the guild belt (converse 180 / `GNPCflags[6]`); otherwise steals or randomly splits a creature inventory item (gold split via type 88) and moves it to the caster's inventory. SFX type 9. **Example** (`INT=30`): `floor(rnd_a*33)` vs `floor(rnd_b*31)`; near coin-flip at this INT -- success lifts one item from the target's bag. |
| 7 | Reveal | `SPID\|NIGH\|MAND` | Implemented | Self-centered AOE. Success roll `floor(rnd*33) < floor(rnd*(INT+1))`; reveals invisible creatures within a 15x15 window where `abs(dx)+abs(dy) <= 10` by clearing the invisibility flag (`flags &= ~2`). **Example** (`INT=30`): on a successful `33` vs `31` roll, an invisible foe `4` east + `3` north (`\|dx\|+\|dy\| = 7 <= 10`) becomes visible. |
| 8 | Seance | `BLOO\|MAND\|NIGH\|SPID\|SULF` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 9 | X-ray | `MAND\|SULF` | Implemented | Single target player NPC buff. `xray = floor(rnd*(8 + (INT>>3)))` capped 255; sends an update packet (msg `d2[0]=33`) when the value increases. **Example** (`INT=30`): `8 + (30>>3) = 8 + 3 = 11`, `xray = floor(rnd*11)` in `0..10`; with `rnd=0.5`->`5`. |

## Sixth Circle (circle 5)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | Charm | `BLAC\|NIGH\|SPID` | Implemented | Single hostile creature (`al==1` only). `charm = floor(rnd*(128 + 2*INT))` capped 255; on nonzero switches allegiance to friendly (`al = 4`, `flags \|= 32`) and clears its current target. SFX type 14. **Example** (`INT=30`): `128 + 2*30 = 188`, `charm = floor(rnd*188)`; any nonzero roll (e.g. `rnd=0.5`->`94`) flips the enemy to your side. |
| 1 | Clone | `SULF\|SPID\|BLOO\|GINS\|NIGH\|MAND` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 2 | Confuse | `MAND\|NIGH` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 3 | FlameWind | `SULF\|BLOO\|MAND` | Implemented | Wind-path multi-hit spell (path from `getwindspell`); each hit damage `dmg = floor(rnd*(48 + INT + (INT>>1)))`; wind SFX (type 22). Same fire-immunity list (411/369/352/374/368/373/367) and double-damage list (364/357/347/360/375/365) as Fireball/Explosion. **Example** (`INT=30`): per-hit `48 + 30 + 15 = 93`, `floor(rnd*93)`; a wind that touches 3 tiles rolls damage separately at each. |
| 4 | HailStorm | `BLOO\|BLAC\|MAND` | Implemented | Multi-wave random AOE around target: 4 waves x `3 + floor(rnd*(1 + (INT-24)/8))` hits. Per hit `dmg = floor(rnd*(16 + (INT>>1)))`; 0-damage rolls skip damage but still emit projectile effects. Ice SFX (type `13 + wave*512`). **Example** (`INT=30`): hits/wave `3 + floor(rnd*(1 + (30-24)/8)) = 3 + floor(rnd*1) = 3`; per-hit `16 + 15 = 31`, `floor(rnd*31)`, e.g. `rnd=0.5`->`15` per icy strike. |
| 5 | MassProtect | `SULF\|GINS\|GARL\|MAND` | Implemented | 5x5 diamond AOE. One roll `prot = floor(rnd*(32 + (INT>>1)))` capped 255; applied to each player NPC in area if stronger than current. SFX type 14. **Example** (`INT=30`): `32 + 15 = 47`, single `prot = floor(rnd*47)`; with `rnd=0.6`->`28` applied to every ally in the diamond who has `< 28`. |
| 6 | NegateMagic | `GARL\|MAND\|SULF` | Implemented | Single target player NPC anti-magic buff. `neg = floor(rnd*(32 + (INT>>1)))` capped 255; applied to `negatemagic` if stronger than current. **Example** (`INT=30`): `32 + 15 = 47`, `neg = floor(rnd*47)`; with `rnd=0.5`->`23`. |
| 7 | PoisonWind | `NIGH\|SULF\|BLOO` | Implemented | Wind-path AOE (path from `getwindspell`); wind SFX (type 23). Each hit attempts poison with roll `floor(rnd*18) < floor(rnd*(INT+10))`; sets the poison flag (`flags \|= 1`) on success. **Example** (`INT=30`): per hit `floor(rnd_a*18)` vs `floor(rnd_b*40)`; the `40` cap makes each poison attempt land often along the wind path. |
| 8 | Replicate | `SULF\|SPID\|BLOO\|GINS\|NIGH` | Implemented | Duplicates the top item at the target tile after extensive exclusion checks (no quest items `info&256`, no weightless/equippable/damage/armour/container items, plus a large explicit type blacklist and a blanket exclusion of all types `>= 342`). Requires a free adjacent tile for the clone. Success roll `floor(rnd*38) < floor(rnd*(INT+1))`; the clone is marked temporary (`info \|= 112`) and stripped of the stealing flag. **Example** (`INT=30`): `floor(rnd_a*38)` vs `floor(rnd_b*31)` -- the `38` cap exceeds `31`, so at `INT=30` it fails slightly more often than it succeeds. |
| 9 | Web | `SPID` | Implemented | Success roll `floor(rnd*38) < floor(rnd*(INT+100))`; places web objects (type 53) in a 3x3 area. Non-spider creatures (not type 361) caught in the web get `crt->wait += floor(rnd*(32 + (INT>>1))/2)` and the paralyzed flag (`flags \|= 8`). SFX type 9. **Example** (`INT=30`): success roll uses cap `INT+100 = 130` vs `38`, so it almost always succeeds; a caught troll gets `crt->wait += floor(rnd*47/2) = floor(rnd*23)`, e.g. `rnd=0.5`->`11`. |

## Seventh Circle (circle 6)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | ChainBolt | `BLAC\|MAND\|SULF\|BLOO` | Implemented | Single target creature start; damage `dmg = floor(rnd*(64 + 2*INT))` per hit. On a hit it can arc to up to 8 chained targets, each found by a random nearby search (`target + ((rnd*17-8)+(rnd*17-8))/2`) with a per-hop retry/miss budget of `128 - 16*hits + 2*INT`. Bolt SFX (type `5 + hits*512`). **Example** (`INT=30`): per-hit `64 + 60 = 124`, `floor(rnd*124)`, e.g. `62`; after 3 hops the search budget is `128 - 16*3 + 2*30 = 128 - 48 + 60 = 140` retries before the chain ends. |
| 1 | Enchant | `SPID\|MAND\|SULF` | Implemented | Target must be a staff (type 78). Success roll `floor(rnd*43) < floor(rnd*(INT+1))` grants pending enchant slots (`info \|= (5<<9)`) up to a 25-spell limit; a separate cast path stores spell charges on the staff. **Example** (`INT=30`): `floor(rnd_a*43)` vs `floor(rnd_b*31)` -- cap `43 > 31`, so at `INT=30` enchanting frequently fails; higher INT closes the gap. |
| 2 | EnergyWind | `MAND\|NIGH\|SULF\|BLOO` | Implemented | Wind-path multi-hit (path from `getwindspell`); each hit damage `dmg = floor(rnd*(64 + 2*INT))`; `0` roll fails that hit. Wind SFX (type 24). **Example** (`INT=30`): per hit `64 + 60 = 124`, `floor(rnd*124)`; a wind covering 4 tiles rolls damage separately per tile (a `0` roll simply misses that tile). |
| 3 | Fear | `NIGH\|MAND\|GARL` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 4 | GateTravel | `SULF\|BLAC\|MAND` | Implemented | Target passable/empty tile. Success roll `floor(rnd*43) < floor(rnd*(INT+100))`; creates a paired moongate: object type `84 + 1024` and a linked type 84 (`info = (3<<4)`) at the target area. **Example** (`INT=30`): cap `INT+100 = 130` vs `43`, so success is highly likely; a gate pair appears linking caster and destination. |
| 5 | Kill | `BLAC\|NIGH\|SULF` | Implemented | Single target creature. Kill-threshold roll `((floor(rnd*(512+8*INT)) + floor(rnd*(512+8*INT))) >> 1)`; must be `>= crt->hp` to kill. Immunity list (undead/daemon/wisp/skeleton/ghost: 352/368/373/367) sets zero damage. SFX types 1 + 15. **Example** (`INT=30`): `512 + 8*30 = 752`; rolls `floor(0.5*752)=376` and `floor(0.4*752)=300`, threshold `(376+300)>>1 = 338`. Vs a `300`-HP dragon, `338 >= 300` -> killed; vs a skeleton -> `0` (immune). |
| 6 | MassCurse | `SULF\|NIGH\|GARL\|MAND` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 7 | MassInvisibility | `MAND\|NIGH\|BLOO\|BLAC` | Implemented | 5x5 diamond AOE. One roll `dur = floor(rnd*(32 + (INT>>1)))` capped 255; applied to each player NPC in area if stronger than current. SFX type 19. **Example** (`INT=30`): `32 + 15 = 47`, single `dur = floor(rnd*47)`; with `rnd=0.7`->`32` applied to every ally in range. |
| 8 | WingStrike | `BLOO\|SPID\|MAND\|SULF` | Implemented | MSP-P4.2: directed physical sweep. Aim chooses an 8-direction heading; the dragon-wing projectile starts 8 tiles behind the caster, passes through the caster, and ends 8 tiles in front (17 crossed tiles total). Only creatures on those crossed tiles are damaged. Per-hit `dmg = floor(rnd*(64 + 2*INT))`; a `0` roll skips that tile. **No elemental immunities** (physical damage). SFX: thrown `OBJ_DRAGON` projectile using the dragon sprite block beginning at frame 30, with cardinal facing chosen from the cast direction. `spelltarget = 1` (tile cursor). **Example** (`INT=30`): per hit `64 + 60 = 124`, `floor(rnd*124)`, e.g. `87`; casting east sweeps the line `x-8 .. x+8` through the caster, rolling separately for each creature on that line. |
| 9 | WizardEye | `BLOO\|NIGH\|MAND\|SULF\|BLAC\|SPID` | Implemented | If not already active, duration `dur = floor(rnd*(INT+1))`; result `3` (fail) if `dur==0`; initializes the wizard-eye camera position at the caster and sends an update packet (msg `d2[0]=36`). **Example** (`INT=30`): `dur = floor(rnd*31)` in `0..30`; with `rnd=0.6`->`18` ticks of remote viewing (a `rnd` low enough to yield `0` fails). |

## Eighth Circle (circle 7)

| Slot | Spell | Reagents | Status | Behavior / calculations |
|---|---|---|---|---|
| 0 | DeathWind | `MAND\|NIGH\|SULF\|BLOO` | Implemented | Wind-path death spell (path from `getwindspell`); wind SFX (type 25). Each hit rolls a kill-threshold `((floor(rnd*(512+8*INT)) + floor(rnd*(512+8*INT))) >> 1)` that must be `>= crt->hp`. Immunity list (undead/daemon/wisp/skeleton/ghost: 352/368/373/367) sets zero damage. **Example** (`INT=30`): per hit uses the same `512 + 240 = 752` threshold math as Kill (avg of two `0..751` rolls ~= `375`), applied to each creature along the wind path. |
| 1 | Eclipse | `MAND\|SULF\|NIGH\|GARL\|BLOO` | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). |
| 2 | MassCharm | `BLAC\|NIGH\|SPID\|MAND` | Implemented | 5x5 diamond AOE. Per-target charm roll `floor(rnd*(128 + 2*INT))` capped 255; on nonzero converts each hostile creature to friendly. SFX type 14. **Example** (`INT=30`): per target `128 + 60 = 188`, `floor(rnd*188)`; almost any roll is nonzero, so nearly every hostile in the diamond flips. |
| 3 | MassKill | `BLAC\|NIGH\|MAND\|SULF` | Implemented | 5x5 diamond AOE. Per-target kill-threshold roll `((floor(rnd*(512+8*INT)) + floor(rnd*(512+8*INT))) >> 1)` vs `crt->hp`. Immunity list (352/368/373/367) sets zero damage. SFX type 15. **Example** (`INT=30`): per target the `752` threshold averages ~= `375`, so any creature in the diamond with `hp <= ~375` (and not immune) is likely killed. |
| 4 | Resurrect | `GARL\|GINS\|SPID\|SULF\|BLOO\|MAND` | Implemented | Requires a valid dead body (type 339). Blocked in sacred/quest zones, housing, storage, non-walkable, and void regions. Success roll `floor(rnd*48) < floor(rnd*(INT+50))` (with a resurrect-list match); restores the player avatar/party and removes the body. **Example** (`INT=30`): cap `INT+50 = 80` vs `48`, so success is likely; `floor(rnd_a*48)` vs `floor(rnd_b*80)` e.g. `20 < 40` -> revived. |
| 5 | Slime | `BLOO\|NIGH\|MAND` | Implemented | 5x5 diamond AOE conversion on hostile creatures. Kill-threshold roll `((floor(rnd*(640+10*INT)) + floor(rnd*(640+10*INT))) >> 1)` vs `crt->hp`; on success replaces the victim with a slime creature (type 375, `info \|= (3<<9)` splitting). Tanglevine (366) is immune; horse (430 + flag 128) is invincible. **Example** (`INT=30`): `640 + 10*30 = 940`; avg of two `0..939` rolls ~= `470`, so a `< ~470`-HP hostile is likely turned into a slime. |
| 6 | Summon | `MAND\|GARL\|BLOO` | Implemented | Target passable empty tile. Success roll `floor(rnd*48) < floor(rnd*(INT+10))`; summons a daemon (type 367, `hp = (base v8 >> 8) * 4`, `respawn_x = 1024`); 25% chance hostile (`al = 1`), otherwise friendly (`al = 4`). **Example** (`INT=30`): success roll `floor(rnd_a*48)` vs `floor(rnd_b*40)` -- cap `48 > 40`, so it fails more often than not at `INT=30`; on success a `floor(rnd_c*4)==0` (25%) daemon is hostile. |
| 7 | TimeStop | `MAND\|GARL\|BLOO` | Implemented | Large-area vortex (`-32..+32`, 65x65 window). Base strength `x3 = floor(rnd*(64 + INT))`; each creature within range gets `crt->wait += x3 - dist*(x3/32)` and the paralyzed flag (`flags \|= 8`) if still positive after the distance falloff. **Example** (`INT=30`): `64 + 30 = 94`, `x3 = floor(rnd*94)`, say `47`. A creature `8` tiles away gains `47 - 8*(47/32) = 47 - 8*1 = 39` wait (integer `x3/32 = 1`). |
| 8 | Tremor | `BLOO\|SULF\|MAND` | Implemented | Wide AOE scan (`x +/-31, y +/-23`). Per creature `dmg = floor(rnd*(80 + INT + (INT>>1))) - 3*max(\|dx\|,\|dy\|)`; applies only if `dmg > 0`. SFX type 26. **Example** (`INT=30`): `80 + 30 + 15 = 125`; a creature `5` tiles away, `floor(0.6*125)=75`, `75 - 3*5 = 60` damage. A distant creature `20` tiles away with a low roll (`floor(0.2*125)=25`) takes `25 - 60 < 0` -> no damage. |
| 9 | Armageddon | `255` (all reagents) | **Unimplemented** | No host cast branch; falls through to "can't cast yet" (result `0`). Helper `spell_armageddon` is a placeholder returning 0. |

---

## Unimplemented spell backlog (by circle)

These are defined in spell tables but currently missing from the host cast implementation path.
Implementation plan (phase IDs `MSP-P*`): `../plans/in-progress/plan-missingSpells.md`.

- Circle 1: `DetectMagic`
- Circle 2: `Infravision`, `Reappear`, `Trap`, `Vanish`
- Circle 3: `Curse`, `MagicLock`, `RepelUndead`
- Circle 4: `Animate`
- Circle 5: `Seance`
- Circle 6: `Clone`, `Confuse`
- Circle 7: `Fear`, `MassCurse`
- Circle 8: `Eclipse`, `Armageddon`

## Notes for upcoming implementation work

- Keep behavior-safe integration with cast flow in `loop_host_part_d_*` fragments.
- New spell logic should preserve existing `i2` semantics (`1/2/3`) so reagent and MP behavior stays consistent.
- If adding helper functions in `src/common/spell_code/spell_code.cpp`, wire them into host cast branches explicitly.
- Circle/slot IDs and reagents are data-driven by `setup_both_basic.h`; update docs if spell names/reagents change.
- Numeric object types cited above (e.g. field 317-320, trap 173, web 53, meat 129, daemon 367) are raw
  `type & 1023` base ids; the high bits (`type >> 10`) select the object variant/frame. Creature `flags`
  bits referenced: `1` = poisoned, `2` = invisible, `4` = asleep, `8` = paralyzed, `32` = charmed.
- Recurring immunity groups: fire-immune = `411/369/352/374/368/373/367`; death/kill-immune (undead,
  daemon, wisp, skeleton, ghost) = `352/368/373/367`; fire double-damage (vine/slug/slime family) =
  `364/357/347/360/375/365`. The full Sleep/MassSleep immunity list is broader (see the Sleep row).
- Worked examples above assume `INT = 30` and a stated `rnd` draw; they illustrate magnitude, not fixed
  outcomes (actual `rnd` is uniform in `[0,1)` per draw).

