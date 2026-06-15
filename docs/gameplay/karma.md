# Karma

Karma is the moral / virtue stat that governs whether a player's soul can be
recovered after death. It is a **per-player, host-authoritative** value: it
lives only in the server's `struct player` and is never trusted to the client.

> Source of truth: `src/server/loop_host.cpp`,
> `src/server/function_host.cpp`, `src/common/data_both.h`.
> Line numbers below were accurate at the time of writing; treat them as
> starting points, not guarantees.

---

## Storage

```cpp
// src/common/data_both.h  (struct player)
float karma;
object *sitem[1024];      // stolen-items list (drives the stealing penalty)
unsigned short sitem_next; // next free index into sitem[]
```

- `karma` is a **float**, even though it is displayed as an integer and most
  adjustments are whole numbers. The stealing penalty (below) can subtract
  fractional amounts (e.g. `gold/16`), so the fractional part is meaningful.
- It is persisted in the player `.sav` file as a 4-byte float (`t4->df[0]`),
  read back on login and re-applied after the inventory is rebuilt.

---

## Starting value

A newly created character starts at **100**:

```cpp
// src/server/loop_host.cpp  (character creation, ~line 4690)
playerlist[tpl]->karma = 100;
```

On login, karma is loaded from the save file rather than reset:

```cpp
// loop_host.cpp ~4758
playerlist[tpl]->karma = t4->df[0];
f = playerlist[tpl]->karma;   // stashed in f while inventory is rebuilt
...
// loop_host.cpp ~5452 (sv2_done)
playerlist[tpl]->karma = f;   // restored after load
```

---

## What lowers karma

### 1. Stealing (`stealing()` — function_host.cpp ~4317)

When a player takes an object flagged as owned (`obj2->info & 128`) that they
haven't already stolen, karma drops. The size of the hit depends on the item:

| Item taken                                   | Karma penalty            |
|----------------------------------------------|--------------------------|
| Gold (`type == 88`)                          | `more2 / 16`             |
| Arrows (`type == 55`)                        | `more2 / 16`             |
| Bolts (`type == 56`)                         | `more2 / 16`             |
| Other "multiple"/stackable item (flag `4096`)| `more2 / 4`              |
| Any other single owned item                  | `2`                      |

Each stolen object is recorded in `sitem[]` so the same item can't be
penalized twice. When a penalty is applied the player is told:

> "You feel less virtuous than before!"

(only sent when `stealing_MESSAGE == TRUE`, which is suppressed during the
login inventory rebuild so loading a save doesn't spam the message).

### 2. Attacking gargoyles while wearing the Amulet of Submission (loop_host.cpp ~17123)

If a player (or a party member whose `converse == 162`) is bound by the
Amulet of Submission (`GNPCflags[12]`) and attacks a gargoyle
(`type 362`/`363`) inside the gargoyle region
(`1024 <= x < 1280`, `256 <= y < 512`):

- karma `-= 2`
- the attack grants **no experience** (`goto noexp`)
- message: "You feel less virtuous than before!"

### 3. NPC-scripted karma changes (CON_sv / FLG_KARMA — function_host.cpp ~4276)

The NPC conversation VM can set a player's karma directly via the `KARMA`
pseudo-variable (`65536 + 15`), but it is **gated by a one-shot flag**
(`FLG_KARMA`, bit `2048` of `NPCflags[CONnpc]`):

```cpp
if ((tplayer->NPCflags[CONnpc] & 2048) == 0) {  // only the first time
    tplayer->NPCflags[CONnpc] |= 2048;
    if (v2 < tplayer->karma) {                  // if it's a reduction
        // "You feel less virtuous than before!"
    }
    tplayer->karma = v2;
}
```

So a given NPC can adjust a player's karma **once** (per the FLG_KARMA flag),
typically as a story/quest consequence. Reads use `KARMA` (returns
`tplayer->karma`) and `FLG_KARMA` (returns the bit), letting scripts branch on
the player's virtue.

---

## What raises karma

- **Completing the Sacred Quest** (the endgame, loop_host.cpp ~24308): when
  the player finishes the quest at the codex (`x==923, y==850`,
  `GNPCflags[18] & 7 == 7`), karma `+= 20` (alongside +3 STR/DEX/INT to the
  primary party member). This is the only positive karma award in the host
  loop besides scripted NPC sets.
- **NPC scripts** (CON_sv KARMA, above) may set karma to a higher value as a
  one-time reward, subject to the FLG_KARMA gate.

There is **no passive regeneration** — karma does not tick back up over time.

---

## NPCs that adjust karma (conversation scripts)

The host conversation VM stores scripts in `host/NPC.BIN` (one 16-byte
instruction = four little-endian int32s `{opcode, l2, l3, l4}`), indexed per
NPC by `host/NPCI.BIN` (`npci[]`). Karma is the conversation variable
`KARMA = 65536+15 = 65551`. Instructions that write it use the arithmetic
opcodes:

| Opcode | Meaning | Effect on karma          |
|--------|---------|--------------------------|
| `4`    | `=`     | set (gated by FLG_KARMA) |
| `9`    | `+=`    | **raise**                |
| `10`   | `-=`    | **lower**                |
| `11`   | `*=`    | scale                    |
| `12`   | `/=`    | scale                    |

A reproducible scan is checked in at `tools/karma_npc_scan.py`
(`python tools/karma_npc_scan.py`). As of this writing it finds **95** karma
writes across **42** NPCs — all `+=`/`-=` (no `=`/`*=`/`/=` in the shipped
data). Most NPCs pair a small `+=`/`-=` (a virtue test: a right/wrong answer),
while the eight Shrine/Virtue NPCs only ever award karma.

### Pure karma *gains* (no penalty branch)

| NPC | Name          | Award        |
|-----|---------------|--------------|
| 108 | Dr Cat        | `+2`         |
| 151 | Shubin        | `+2`, `+1`   |
| 161 | Finn          | `+2`         |
| 190 | Honesty       | `+5`         |
| 191 | Compassion    | `+5`         |
| 192 | Valor         | `+5`         |
| 193 | Justice       | `+5`         |
| 194 | Sacrifice     | `+5`         |
| 195 | Honor         | `+5`         |
| 196 | Spirituality  | `+5`         |
| 197 | Humility      | `+5`         |

(The eight virtues 190–197 are the Shrine meditation rewards: `+5` each.)

### NPCs with both raise and lower branches (virtue tests)

These NPCs reward a "good"/correct response and penalize the opposite within
the same conversation. Net effect depends on the player's choices.

| NPC | Name         | Karma writes                          |
|-----|--------------|---------------------------------------|
| 11  | Ariana       | `-1, +1, -2, +2, -2`                   |
| 40  | Derydlus     | `+1, -1`                              |
| 41  | Zellivan     | `+1, -1`                              |
| 42  | Nomaan       | `+1, -1`                              |
| 44  | Van Kellian  | `+1, -1`                              |
| 45  | Heftimus     | `-2, +4, +2`                          |
| 47  | Culham       | `-1, +1`                              |
| 48  | Jerris       | `-1, +1`                              |
| 54  | Andrea       | `-1, +1`                              |
| 58  | Boskin       | `-1, +1, -2, -2`                      |
| 59  | Pridgarm     | `-5, +7, +5`                          |
| 90  | Antonio      | `-1, +1`                              |
| 92  | Conor        | `-1, +1`                              |
| 97  | Patrick      | `-1, +1`                              |
| 103 | Dorin        | `-1, +1`                              |
| 110 | Elad         | `+2, -2`                              |
| 114 | Homer        | `-2, +1, -1, +2`                      |
| 122 | Gertan       | `-1, +1`                              |
| 125 | Ahrmaand     | `-1, +1`                              |
| 130 | Smith        | `-1, +1`                              |
| 132 | Karina       | `+2, -2, +2, -2, +2, -2`              |
| 133 | Kador        | `+2, -2, -2`                          |
| 141 | Papa         | `-1, +1, +2`                          |
| 155 | de Hugh      | `-1, +1`                              |
| 157 | Koranada     | `+2, -1, +1`                          |
| 167 | Valkadesh    | `+1, -1`                              |
| 174 | Farmer Nash  | `-1, +1`                              |
| 175 | Farmer Krill | `-1, +1`                              |
| 178 | Mandrake     | `-1, +1, +2, -2, +2, -2`              |
| 183 | Ybarra       | `-2, -2, +2`                          |
| 199 | Wisp         | `-1, +1`                              |

> The largest single swings in the shipped scripts are Pridgarm (`-5` / `+7`)
> and the four virtue shrines (`+5`). Note all NPC karma changes via the VM's
> `KARMA` variable go through `CON_sv`, which prints "You feel less virtuous
> than before!" when the new value is lower.

To regenerate this table after editing the NPC data, re-run
`python tools/karma_npc_scan.py` and update the lists above.

---

## How karma impacts gameplay

### Death — the central mechanic

Karma's primary effect is at **death**. When the player's primary body dies
(loop_host.cpp ~887):

#### karma > 0 — resurrection allowed

The player is added to the **resurrect list** (`resu[]`), their body is
recorded, and:

- body is moved to the death position `DEATHPOSX, DEATHPOSY` (= `2032, 1012`,
  see `define_host.h`).
- HP is reset to 30, poisoned flag cleared, `resu_wait` set to ~16 (intended
  ~1 hour).
- The player is prompted to resurrect with **F1**:
  - no insurance (`GNPCflags[25] == 0`): "...press F1 to resurrect"
  - with insurance: "...press F1 to use your insurance and resurrect at healer"

#### karma <= 0 — permanent death ("unending darkness")

```cpp
if (tplayer->karma <= 0) {
    // body moved to DEATHPOSX/DEATHPOSY
    // "An unending darkness engulfs thee..."
    // if the player owns a house: zero its gold, clear the occupied flag,
    //   release ownership (GNPCflags[28] = 0) — house items remain recoverable
}
```

The player is **not** added to the resurrect list. The character is gone:
on the next save pass the dead, karma-≤0 player is skipped and removed
rather than saved:

```cpp
// loop_host.cpp ~1083
if ((playerlist[tpl]->party[0] == NULL) && (playerlist[tpl]->karma <= 0))
    goto save_complete; // skip save, then remove
```

A house owned by such a player is freed (but its chest is intentionally **not**
auto-created, so the items can still be recovered).

### Bleeding out at zero karma

Independently of the death handler, while a living player's karma is `<= 0`
the host actively drains their HP (loop_host.cpp ~13596):

```cpp
if (tplayer->karma <= 0) {
    tplayer->karma = 0;       // clamp to 0
    // roughly once per second, tnpc->hp-- and flag an update
}
```

So reaching zero karma is effectively a death sentence: the avatar loses 1 HP
per second until it dies, and that death is then the permanent ("unending
darkness") variety described above.

---

## Summary table

| Aspect              | Value / behavior                                            |
|---------------------|-------------------------------------------------------------|
| Type / storage      | `float karma` in `struct player` (host only); saved to .sav |
| Starting value      | `100` at character creation                                 |
| Lowered by          | Stealing owned items; attacking gargoyles under the Amulet of Submission; one-shot NPC scripts |
| Raised by           | Completing the Sacred Quest (+20); one-shot NPC scripts     |
| Passive regen       | None                                                         |
| At/below 0 (alive)  | Clamped to 0; loses ~1 HP/sec until death                   |
| Death with karma >0 | Added to resurrect list; F1 to resurrect (insurance optional)|
| Death with karma ≤0 | Permanent death ("unending darkness"); no resurrection; character removed on next save; owned house released (items recoverable) |

---

## Notes for maintainers

- Karma lives entirely on the host; Don't add client-side karma state.
- The `sitem[]`/`sitem_next` ring buffer (1024 entries) is what prevents
  double-penalizing the same stolen object — don't reorder or shrink it
  without understanding the stealing path in `function_host.cpp`.
- `DEATHPOSX/DEATHPOSY` (`2032,1012`) is the shared "you died" coordinate used
  for both resurrection-eligible and permanent deaths.
