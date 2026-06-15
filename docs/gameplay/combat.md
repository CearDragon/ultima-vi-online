# Combat

Combat in Ultima VI Online is **entirely host-authoritative**. All hit rolls,
damage, dodge, and armor math run in `src/server/loop_host.cpp` and the
helpers in `src/server/function_host.cpp`; the client only renders the results
(attack/miss/damage "special effects", `sf[]`). Stats come from the per-NPC
`struct npc` (`s`=strength, `d`=dexterity, `i`=intelligence, `arm`=worn armor,
`hp`); item stats come from the shared `obji[]` table (`struct obji_i` in
`src/common/data_both.h`).

> Source of truth: `src/server/loop_host.cpp` (the player-attack block near
> the `skip_hth*` labels ~15700–17000, and the creature-attack block
> ~12640–12800), plus `getequiparmourvalue`, `getequiph2hwepdamage`,
> `ENHANCEget_attack`, `ENHANCEget_defense` in `function_host.cpp`.
> Line numbers were accurate at the time of writing; treat them as starting
> points.

`rnd` is the engine RNG; `rnd * N` yields a value in `[0, N)` (float). This
note appears throughout because every roll below uses it.

---

## Item stat fields (`obji[]`)

Each base tile/item type has an `obji_i` entry (`data_both.h` ~250):

| Field | Combat meaning                                                   |
|-------|------------------------------------------------------------------|
| `v1`  | **weapon base damage**                                           |
| `v2`  | **armor/defense value** (worn)                                   |
| `v5`  | **weapon damage type** → selects the STR/DEX/INT scaling (`wep_dt`) |
| `v6`  | `(v6>>8)` = creature **dodge** rating (defender)                 |
| `v7`  | `(v7>>8)` = creature **armor** rating (defender, monsters)       |
| `v8`  | `(v8&255)` = creature **natural attack** damage; `(v8>>8)*4` ≈ hp base |

The "weapon damage type" (`v5`) maps to a scaling rule applied at damage time
(see **Damage**, `wep_dt`).

---

## "+" enhancement modifiers (weapon/armor plus)

A weapon or armor object stores its enchantment in the object's `more2` field;
the host decodes it per-object (`function_host.cpp` ~4463):

```cpp
ENHANCEget_attack(obj)  = obj->more2 & 0x0F          // weapon "+N" to damage
ENHANCEget_defense(obj) = (obj->more2 & 0xF0) >> 4   // armor  "+N" to defense
```

- So both attack-plus and defense-plus range **0–15**, packed into the low and
  high nibble of `more2` respectively.
- Excluded item types (return 0 regardless of `more2`): staff (78), lightning
  wand (79), fire wand (80), glass sword (48), protection ring (256),
  decorative sword (141), and any "multiple"/stackable item (flag 4096).
- Weapon plus only applies when the item actually has `v1` (is a weapon);
  armor plus only when it has `v2` (is armor).

These enhancements are added wherever the raw `v1`/`v2` is read:

```cpp
weapondamage = obji[...].v1 + ENHANCEget_attack(obj);   // attack
armourvalue  = obji[...].v2 + ENHANCEget_defense(obj);  // defense
```

---

## Worn armor total (`npc->arm`)

When equipment changes (`upflags & 64`), the host recomputes the wearer's total
armor (`loop_host.cpp` ~26613):

```cpp
arm = sum over { helm, wep_right, wep_left, armour, boots, neck,
                 ring_right, ring_left } of (obji[type].v2 + ENHANCEget_defense)
if (protection) arm += 10;                 // In Pro spell / protection ring
// low-level primary-avatar training-wheels bonus:
if (primary avatar && exp < 1600 && lev <= 2) arm += (20 - exp/80);
```

`getequiparmourvalue(items)` does the same sum but walks a **loose inventory
list** (used for monsters whose gear lives in their drop bag), with an
`armourmask` so two helms / two of the same slot don't stack.

---

## Player / party member attacking

### 1. Reach & weapon selection
Each hand (`wep_right`, then `wep_left`) is processed. Range is `x2` (Chebyshev
distance). Most weapons are melee (`x2 <= 1`); reach-2 weapons (halberd 47,
morning star 40) and ranged weapons (sling 33, bow 41, magic bow 54, crossbow
42, triple crossbow 50, boomerang 49, thrown spear 36) have special handling
and consume/῾throw ammo. Bare-handed attack falls back to `x = 1`,
`wep_dt = 1`.

The raw weapon damage accumulator is:
```cpp
x += obji[sprlnk[wep->type & 1023]].v1 + ENHANCEget_attack(wep);
```

### 2. Hit / miss roll (ranged & projectile path)
```cpp
// loop_host.cpp ~16557 (skip_hth2)
x7 = rnd * (tnpc->d + 1);   // attacker dexterity roll, [0, d]
x8 = rnd * 16;              // [0, 16)
if (x7 < x8) { x = 0; miss = 1; }   // MISS
```
So **to-hit scales with the attacker's DEX** against a flat 0–15 threshold; a
DEX of ~16+ beats the threshold most of the time. (Melee uses the dodge step
below rather than this pre-roll.) On a ranged miss, bows/crossbows have a 50%
chance to drop a recoverable arrow/bolt at the target tile.

### 3. Damage roll
Once a hit lands, the base weapon damage `x` is scaled by the weapon's damage
type (`wep_dt`, derived from `obji.v5`) and the attacker's stats
(`loop_host.cpp` ~16827):

| `wep_dt` | Scaling `f` (with `x` = weapon base)              | Typical weapons         |
|----------|---------------------------------------------------|-------------------------|
| 1        | `x/32 * (STR + DEX*0.25)`                          | heavy melee (STR)       |
| 2        | `x/32 * (DEX*0.75 + STR*0.25)`                     | balanced melee          |
| 3        | `x/32 * (DEX*1.75)`                                | bows / crossbows (DEX)  |
| 4        | `x/32 * (min(STR,DEX) + |STR-DEX|*0.25)`           | "lower of STR/DEX"      |
| 5        | `x/32 * (INT*0.125 + 8)`                           | wands (magical)         |

Then the actual rolled damage:
```cpp
x2 = rnd * (x*2 + f + 1) - x;     // weapon damage roll, centered so it can graze
if (x2 > x) x2 = x;               // cap at weapon base
if (craft) x2 = rnd * 31;         // ship cannons: flat 0–30, no graze
```

### 4. Armor reduction (defender)
```cpp
x2 -= (int)( rnd * ( (obji[target.type].v7 >> 8) + 2
                     + getequiparmourvalue(target.items) ) );  // monster armor
if (x2 < 0) x2 = 0;
```
For a monster target this is its natural armor (`v7>>8`) plus any gear in its
bag; for a player/party target the equivalent worn-armor value applies.

### 5. Dodge (defender)
Unless the weapon is `wep_dt==3` (bows — can't be dodged) or a cannon:
```cpp
if (int(rnd*32) <= int(rnd * ((obji[target.type].v6 >> 8) + 1)))
    x2 = 0;   // enemy dodged
```
So higher defender **dodge rating (`v6>>8`)** more often nullifies the hit.

### 6. Apply
```cpp
crt->hp -= x2;
if (crt->hp < 0) crt->hp = 0;
```
Special cases: tanglevine tendril (366) and the player's own horse (430 w/ flag
128) are set invincible (`hp = 32767`); the glass sword (48) does `x = 1023`
(near-instant kill) and then **shatters** (see Durability).

---

## Monsters / NPCs attacking

When a creature is adjacent to its target (`abs(dx)<=1 && abs(dy)<=1`,
`loop_host.cpp` ~12640):

### 1. Target armor roll
```cpp
if (target is a player/NPC struct) x5 = rnd * (tnpc->arm + 1);     // worn arm
else x5 = rnd * ((obji[target.type].v7 >> 8) + 2
                 + getequiparmourvalue(target.items));
```

### 2. Attack damage
```cpp
x6 = obji[attacker.type].v8 & 255;             // natural attack power
x7 = getequiph2hwepdamage(attacker.items);     // best weapon in its bag
if (x7 > x6) x6 = x7;                           // use the better of the two
x6 = rnd * (x6 + 1);                            // roll
x6 -= x5;                                        // subtract target armor roll
```

### 3. Dodge (the target may evade)
```cpp
x7 = rnd * 32;
if (target is player/NPC) x8 = rnd * (tnpc->d + 1);              // target DEX
else x8 = rnd * ((obji[target.type].v6 >> 8) + 1);              // target dodge
if (x7 > x8) {            // failed to dodge → take the hit
    tnpc->hp -= x6;
}
```
So a defender's **DEX (or `v6` dodge rating)** is the evasion stat against
monster attacks — at DEX 32 you dodge ~¼–½ of hits, at DEX 64 nearly all.

### 4. On-hit riders
- **Poison** chance (`rnd*32 == 0`) from mouse/rat/rotworm/scorpion/serpent/
  slime/snake/spider/squid.
- **Corpser** (357): `rnd*16 == 0` halves target hp ("dragged under!").

---

## Weapon durability

There is **no gradual wear/durability** system — weapons and armor do not
degrade with use. The only "consumption" mechanics are:

| Mechanic            | Behavior                                                       |
|---------------------|----------------------------------------------------------------|
| **Glass sword (48)**| One-use: deals `x = 1023` then is destroyed (`wep_left/right = NULL`, `upflags |= 64`). Excluded from "+" enhancement. |
| **Arrows (55)**     | Bow consumes one per shot (`more2--`, removed at 0); ~50% recoverable on miss. |
| **Bolts (56)**      | Crossbow/triple-crossbow consume per shot; recoverable like arrows. |
| **Thrown spear (36)**| Leaves the hand, lands as a pick-up at the target; a new spear from inventory is auto-equipped if available. |

`more2` on a weapon is the **enhancement nibble + quantity** (for stackables),
*not* a durability counter — so "weapon durability" is effectively N/A except
for the glass sword's single-use nature.

---

## Attack timing (cooldown)

Not damage, but governs attack rate. After an attack the host adds a wait based
on attacker DEX (`loop_host.cpp` ~16650):
```cpp
f = tnpc->d / 64; if (f > 1) f = 1;
f = 2.0 - (f * 0.25);          // ~2.0s at DEX 0 → ~1.75s at DEX 64
// dual-wield / spiked-helm multi-hit multiply f by 1.125–1.25
tnpc->wait_attack += f;
```
Higher DEX → shorter cooldown (and DEX also drives to-hit and dodge), making it
the single most valuable combat stat.

---

## Summary

| Question                | Answer                                                            |
|-------------------------|-------------------------------------------------------------------|
| Weapon hit chance       | Ranged: `rnd*(DEX+1) < rnd*16` ⇒ miss (DEX-scaled). Melee: resolved by the defender dodge roll. |
| Monster hit chance      | Hits unless target dodges: `rnd*32 > rnd*(DEX+1)` (or `v6` dodge for monsters). |
| Damage calculation      | `roll(x*2 + f + 1) - x`, capped at `x`; `f` from `wep_dt` (STR/DEX/INT) — then minus armor roll, then dodge can zero it. |
| Weapon "+" modifier     | `more2 & 0x0F` (0–15) added to weapon `v1` damage.                 |
| Armor "+" modifier      | `(more2 & 0xF0) >> 4` (0–15) added to each armor piece's `v2`.     |
| Weapon durability       | None (no wear). Glass sword = one-shot; arrows/bolts/thrown spears are consumables. |
| Key stat                | **DEX** — drives to-hit, dodge, and attack speed.                  |

---

## Worked examples (real data)

These use **actual values pulled from the host data files**
(`dr/OBJINFO.U6O` via `dr/U6SPR.LNK`) with `tools/combat_stats_dump.py`. To
keep things readable, the in-code variable names are replaced with plain words:

- **Weapon Power** = the weapon's base damage (`v1`)
- **Your Strength / Dexterity / Intellect** = your character's STR / DEX / INT
- **Armor Rating** = the target's total worn defense
- **Dodge Rating** = the target's evasion stat
- A *roll of N* means "a random number from 0 up to N".

The data shows weapons with Power ranging from **2** (a dagger-class weapon) up
to **30** (a great two-handed weapon), the one-shot glass sword at **255**,
body armor from **2** (leather) to **10** (plate), and monsters ranging from a
**4-HP** critter to a **1020-HP** boss.

### Example 1 — Mid-tier sword hit on a lightly-armored foe

Setup (all real numbers):
- Weapon Power = **8** (a one-handed sword, damage type 2 = "balanced")
- Your Strength = **24**, Your Dexterity = **20**
- Target = a 32-HP creature with Armor Rating **2**, Dodge Rating **16**

1. **Stat bonus** (damage type 2 → DEX 75% + STR 25%):
   `bonus = 8 / 32 × (20 × 0.75 + 24 × 0.25) = 0.25 × (15 + 6) = 5.25`
2. **Damage roll** = `roll(8×2 + 5.25 + 1) − 8` = `roll(22.25) − 8`.
   A middling roll of ~14 → `14 − 8 = 6`. (Capped at the Power of 8.)
3. **Armor**: subtract `roll(ArmorRating + natural + 2)` ≈ `roll(4)` → say 1.
   `6 − 1 = 5`.
4. **Dodge check**: `roll(32)` vs `roll(DodgeRating+1)=roll(17)`. If the first
   is ≤ the second the hit is dodged (here, often a hit lands).
5. **Result:** about **5 damage** → target drops from 32 to 27 HP.

### Example 2 — Same swing, but the target is well-armored

Change only the target to a tougher creature: Armor Rating **10**, Dodge
Rating **27** (real values from the data's heavier monsters).

- Steps 1–2 are identical → ~6 raw damage.
- **Armor**: subtract `roll(10 + 2 + 2) = roll(14)` → often 6–7.
  `6 − 7 → 0` (clamped). 
- Even when armor doesn't fully absorb it, the high **Dodge Rating 27** means
  `roll(28)` frequently beats `roll(32)`, zeroing the hit.
- **Result:** many swings do **0**; you need higher Power / Strength / Dexterity
  (or a bow, which **ignores dodge**) to reliably hurt this target.

### Example 3 — Bow shot (Dexterity weapon, can't be dodged)

- Weapon Power = **6**, damage type **3** ("bow")
- Your Dexterity = **30**
- Target: 80-HP creature, Armor Rating **0**

1. **Hit check** (ranged): miss if `roll(Dexterity+1) < roll(16)`, i.e.
   `roll(31) < roll(16)`. With DEX 30 you clear the 0–15 bar most of the time.
2. **Stat bonus** (type 3 → DEX ×1.75): `6 / 32 × (30 × 1.75) = 0.1875 × 52.5 ≈ 9.8`
3. **Damage roll** = `roll(6×2 + 9.8 + 1) − 6 = roll(22.8) − 6`; a ~13 roll → **7**.
4. **No dodge step** for bows, and Armor 0 here → full **7 damage**.
5. **Result:** ~7 per arrow, and high DEX makes both the hit *and* the damage
   scale — which is why DEX is the standout combat stat.

### Example 4 — A monster swinging back at you

- Monster natural attack = **20** (a strong creature)
- You are wearing armor totaling **Armor Rating 14** (e.g. plate body + helm +
  shield), Your Dexterity = **30**

1. **Your armor soak** = `roll(ArmorRating+1) = roll(15)` → say 7.
2. **Monster damage** = `roll(naturalAttack+1) = roll(21)` → say 12,
   then minus your soak: `12 − 7 = 5`.
3. **Your dodge** = monster lands it only if `roll(32) > roll(Dexterity+1)`,
   i.e. `roll(32) > roll(31)` — with DEX 30 you evade a large share outright.
4. **Result:** when it connects, ~**5 damage**; with good DEX many swings miss
   entirely. Stacking Armor Rating shrinks the hits, DEX makes them whiff.

### Example 5 — Reading a "+" enchantment

A weapon's "+" lives in the low nibble of its enchant byte, armor's "+" in the
high nibble:

- Enchant byte `0x07` on a sword → **+7 damage** (`0x07 & 0x0F = 7`); a Power-8
  sword effectively swings at Power **15**.
- Enchant byte `0x30` on plate → **+3 defense** (`(0x30 & 0xF0) >> 4 = 3`); a
  defense-10 plate counts as **13**.
- Enchant byte `0x35` on a single item is both at once only if it qualifies as
  both weapon and armor (rare) — normally one nibble is meaningful per item.

> Reproduce / refresh these numbers any time with
> `python tools/combat_stats_dump.py` (weapons, armor, and monster table).

## Notes for maintainers
- Combat is host-only; never compute hit/damage client-side. The client trusts
  `sf[]` effect packets (`type 1` = hit, `type 16` = miss/no-damage, with
  `x2` = damage shown).
- `wep_dt` is read from `obji.v5` per weapon; if you add a weapon, set `v5` to
  the intended scaling rule (1–5) or it defaults to the unscaled `f = x`.
- The `arm` recompute only runs on equipment change (`upflags & 64`); if you
  add a new equip slot, add it to that sum (~loop_host.cpp 26613) or it won't
  protect the wearer.
