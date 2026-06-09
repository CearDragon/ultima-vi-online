# Housing, storage, saves, and the Guardian Guild

This document explains how player housing, house storage shelves, and the
related save files work on the **host**, and then documents the Guardian Guild
special case: owner-independent, persistent guild storage backed by
`save\guardianobjs.sav`.

Everything here is host-side (`HOST` / `both` builds). The client never owns any
of this state; it only renders what the host sends.

---

## 1. Map patches and how houses are built

House layouts live in `assets/map_patches/*.txt`. Despite the `.txt` extension
these are **C++ source fragments** that are `#include`d directly into the body
of `house()` in `src/common/house.cpp`:

```cpp
// src/common/house.cpp
void house() {
    // ...zeroes house arrays...
#include "../../assets/map_patches/house1.txt"
#include "../../assets/map_patches/house2.txt"
    // ...
#include "../../assets/map_patches/guardianguild.txt"
    // ...
}
```

So a patch file can call any host function in scope (`BTset`, `OBJaddnew`,
`OBJnew`, etc.) and reference host globals (`housestoragex`, `basehousenumber`,
`patchx`, `housenumber`, …). `house()` runs **once at startup**, from
`setup_host.inc`:

```cpp
// src/server/setup_host.inc
house();
#include "host_setup.h"
guardianguild_load();   // (added for the guild; see §6)
```

Key primitives used by patches:

- `BTset(x, y, tile)` — sets the base map tile at `(x+patchx, y+patchy)`. If a
  `housenumber != 65535` is active, the tile is also added to that house's
  perimeter list (`housepx/housepy/housepnext`) and bounding box
  (`housex1/x2`, `housey1/y2`).
- `OBJaddnew(x, y, type, info, more2)` — creates a world object on a tile (the
  `od[y][x]` linked list) — furniture, doors, signs, ladders, etc.
- `housenumber` — the "current" house id used by `BTset`. `65535` means
  "non-house tiles".
- `basehousenumber` — base id offset (a runtime constant, `20`); patches use the
  `basehousenumber + N` convention so house ids don't collide.

### House numbering

- `basehousenumber` is initialized to `20` in `function_host.cpp`
  (`function_host_init`), **but it is NOT constant** — several map patches
  reassign it as they load (e.g. `bryan.txt` sets it to 48, `mose.txt` to 47,
  `steel.txt` to 52, `spiritwood.txt` to 99). Each patch then registers its
  houses as `basehousenumber + N` relative to whatever value is current while
  *that* patch runs. So:
  - The absolute house id a patch produces depends on the include order in
    `house()` and on what the preceding patches left `basehousenumber` set to.
  - After `house()` finishes, `basehousenumber` holds whatever the **last**
    patch left it at (currently 52, from `steel.txt`) — **not** 20.
  - **Do not compute a stable house id as `basehousenumber + offset` outside the
    patch that registered it.** Code that runs later (e.g. the guild
    save/load below) must use the **absolute** house id, or it will key off the
    wrong (usually empty) house. This was the original "nothing saved" bug.
- `HOUSEMAX` is `256`; valid ids are `1..255` (`define_host.h`).
- Absolute house ids actually in use after all patches load top out around
  `124`; `54..99` and `125..255` are free (confirmed by a one-shot startup dump
  of every house with `housestoragenext[h] > 0`).
- Two different patches can collide on the same absolute id. For example
  `20+100` (the guild's first attempt) equals `99+21`, which `spiritwood.txt`
  also uses — so house 120 ends up holding **both** the guild's shelves and
  spiritwood's. Pick a guild id that no `basehousenumber + offset` can reach.

---

## 2. The object model (`od`, `object`, `OBJlist`)

World objects are nodes in a per-tile linked list:

```cpp
struct object {
    unsigned short type;   // sprite + flag bits; (type & 1023) = base sprite id
    unsigned short info;   // quantity/position/flags (see data_both.h for bit meanings)
    unsigned short x, y;
    union { void *more; unsigned long more2; }; // child-list pointer OR scalar
    void *prev;            // previous sibling (NULL = first on tile / first in container)
    void *next;            // next sibling (NULL = last)
};
```

- `od[y][x]` is the head `object*` for a tile. Objects stacked on a tile are
  chained via `->next`.
- A **container** (its `obji[sprlnk[type & 1023]].flags & 1024` is set) holds
  its contents via `->more` (the first child), and children chain via `->next`.
  `OBJaddtocontainer()` **prepends** to `->more`.
- `OBJadd(x, y, obj)` appends `obj` to the end of `od[y][x]`'s `->next` chain
  (with a special-case re-order for "float" objects), and calls
  `OBJcheckflags`.
- `OBJlist(root)` flattens an object tree into the global `OBJlist_list[]`
  array and returns the count. Order is: the object, then its `->next`
  siblings, then (breadth-first) the contents of any containers encountered.
  Parents always precede their children in the resulting list. Callers set
  `OBJlist_last = 0` before the first call (the codebase resets it to `NULL`
  between independent calls).

> The shelf furniture itself is `od[y][x]` (the head). Dropped items begin at
> `od[y][x]->next`. **The storage code always operates on `->next`**, never the
> furniture head — otherwise the furniture would be saved/removed too.

---

## 3. House storage shelves

A storage shelf is a normal furniture object placed by a patch, whose tile is
**registered** as a storage slot for some house id:

```cpp
OBJaddnew(1281,331, 279+1024*1, 0, 0);            // the shelf furniture object
housestoragex[H][housestoragenext[H]] = patchx + 1281;
housestoragey[H][housestoragenext[H]] = patchy + 331;
housestoragenext[H]++;                            // one more slot for house H
```

Relevant globals (`globals.inc` / `function_host.h`):

| Global | Meaning |
| --- | --- |
| `housestoragenext[HOUSEMAX]` | number of registered storage slots for a house |
| `housestoragex/ y[HOUSEMAX][HOUSESTORAGESLOTMAX]` | tile coords of each slot |
| `HOUSESTORAGESLOTMAX` | `600` (`define_host.h`) |
| `housestorageadd` / `housestoragerestore` | flags used by the player save/restore goto blocks |

### Storage-shelf gameplay rules

When a player drops an item, the host scans **all** houses to decide whether the
drop target is a storage shelf (`loop_host.cpp`, ~`for (i3=1; i3<=255; i3++)`):

- **Stolen items** (`info & 128`) cannot be placed on a shelf.
- **8-item stack limit** per shelf tile ("Put items in containers to store more
  here.").

These rules are keyed on the tile being *any* house's storage slot — there is
no owner check at drop time — so they work communally regardless of who, if
anyone, owns the house.

### Ownership and "decay" (the default behavior)

A player's owned house id is stored in `GNPCflags[28]`. On **logout/save**
(`loop_host.cpp`, label `housestorageadd`):

1. For each storage slot of `GNPCflags[28]`, the items at `od[...]->next` are
   serialized into the player's `.sav` (count recorded in `GNPCflags[29]`).
2. If the player is really leaving (`save_update != -1`), those items are then
   **detached from the world** (`od[...]->next = NULL`).

On **login/restore** (label `housestoragerestore`), the saved items are
re-created and `OBJadd`-ed back onto the same shelves — *unless* the player no
longer owns that house, in which case the items are dumped into a chest in their
inventory instead.

> This is what the Guardian Guild request calls "decay": shelf contents are
> tied to the building's owner, vanish from the world when that owner logs out,
> and only reappear when they log back in. There is **no time-based ground-item
> decay** anywhere in the codebase — "decay" here means exactly this
> owner-tied save/remove behavior.

---

## 4. Save files (host)

All host saves live under `.\save\`. File I/O goes through the thin wrappers in
`src/common/myfile.cpp` (`open2`, `get`, `put`, `close`, `lof`) and the `txt`
buffer type in `src/common/txt.cpp`.

| File | Written by | Read by | Contents |
| --- | --- | --- | --- |
| `save\<id>.sav` | `loop_host.cpp` player-save block | `host.inc` login | One per player. Party, inventory, equipped items, **house storage contents** (count in `GNPCflags[29]`), house id (`GNPCflags[28]`), horses/ships/deeds, etc. |
| `save\house.sav` | `housesav_update()` (`function_host.cpp`) | `host.inc` startup | The 256-entry `housesav[]` table: per-house `flags` (occupied bit) and `gold` balance. Periodically re-saved (~every 300 ticks) and on rent/eviction events. |
| `save\guardianobjs.sav` | `guardianguild_save()` | `guardianguild_load()` | **New.** All items on the Guardian Guild shelves (see §5/§6). |

### The `txt` buffer & file format conventions

`txt` is a growable byte buffer with overlaid views:

```cpp
struct txt {
    union { char *d; unsigned char *d2; unsigned short *ds; unsigned long *dl; float *df; double *dd; };
    long l;   // current length
    long bl;  // allocated length
};
```

- `txtNEWLEN(t, -n)` allocates an `n`-byte buffer without zeroing (used right
  before `get()` to read a whole file: `txtNEWLEN(t, -lof(tfh)); get(tfh, t->d2, t->l);`).
- `txtaddshort` / `txtaddlong` append little-endian `u16`/`u32`.
- `txtright(t, t->l - n)` consumes (drops) the leftmost `n` bytes — that's how
  the legacy player-restore parser walks a save stream front-to-back.
- `house.sav` (and the new guild file) are written **unencrypted**: a 2-byte
  marker `12345` precedes `house.sav` data; the legacy encrypt/decrypt path is
  dead code.

`open2()` always returns a non-NULL `file*`; on failure `file::h == HFILE_ERROR`.
`get`/`put`/`close` are all no-ops (except the `free`) on an error handle, so the
"file doesn't exist yet" case is just: open, check `h == HFILE_ERROR`, `close`,
return.

---

## 5. Shutdown and startup hooks

The host has no window; it runs a console (`CONSOLE` build). Graceful shutdown
is **`q` then `y`** at the console, handled in `src/common/u6o7.cpp`:

1. `KeyEventProc` sets `exitrequest = TRUE`.
2. The main loop sees `exitrequest && !inbritannia_totalplayers` and calls
   `PostQuitMessage(0)`.
3. `GetMessage` returns `0` (WM_QUIT) and the `#ifdef HOST` shutdown block runs,
   then `ExitProcess(0)`.

That HOST shutdown block is the single reliable graceful-exit point, so it's
where the guild save is invoked:

```cpp
// src/common/u6o7.cpp  (HOST shutdown block, before ExitProcess)
#ifdef HOST
    if (NEThost) {
        guardianguild_save();   // persist guild shelves
        closesocket(u6osocket);
        // ...close client sockets...
    }
#endif
```

Startup loads happen in `setup_host.inc`, **after** `house()` has registered the
shelf slots and `host_setup.h` has populated the world:

```cpp
house();
#include "host_setup.h"
guardianguild_load();
```

> Force-quitting (Ctrl-C) bypasses this block and will **not** save the guild
> shelves. A crash likewise won't. If stronger durability is wanted, add a
> periodic autosave next to the existing `housesav_update()` tick in
> `loop_host.cpp` (that would require mirroring the change into
> `loop_host.inc`).

---

## 6. The Guardian Guild

Source: `assets/map_patches/guardianguild.txt`. The guild occupies two connected
map levels, joined by a single ladder (`OBJ 305`):

- **Ground floor** — surface region around `x≈400-423, y≈320-351`. Ladder
  **top** at `(401,339)`. Contains a desk, tables, barrels, a sign, and
  doorframes — **no storage shelves**.
- **Basement** — lower region around `x≈1280-1291, y≈319-334`, reached by
  climbing **down** the ladder (**bottom** at `(1286,323)`). This is where the
  storage shelves are.

The **storage shelves are the only storage in the guild**, and they are all in
the **basement**: the L-shaped cluster of seven `279` ("table-with-storage")
tiles:

```
(1281,330) (1281,331) (1281,332) (1281,333)
                                  (1282,333) (1283,333) (1284,333)
```

There are no chests or other container furniture acting as storage anywhere else
in the patch (ground floor or basement). When the original request referred to
"two areas for storage," that meant the two *registrations* described below
(`+0` and `+26`) that both pointed at this same basement cluster — not two
physically separate rooms.

### Before this change

The same 7 shelf tiles were registered **twice**:

- to `basehousenumber + 0` (the shared scratch house `20`), and
- to `basehousenumber + 26` (the guild building house `46`, which is ownable —
  `houseinitialcost = housecost = 1`, with an entrance at `(368, 386)`).

Because house `46` (and `20`) can be owned, the shelves were owner-tied: their
contents were saved into the owner's `.sav` and removed from the world on logout
("decay"), and restored on login. This is the behavior the request asked to
change "for the Guardian Guild only".

### After this change

The shelves are now registered **once**, to a dedicated, **non-ownable** storage
house, and persisted independently to `save\guardianobjs.sav`.

1. **Dedicated storage house** — `define_host.h`:

   ```cpp
   #define GUARDIANGUILD_STORAGE_HOUSE 200   // absolute house id
   ```

   This is an **absolute** house id (not `basehousenumber + offset`) for two
   reasons learned the hard way:
   - `basehousenumber` is mutated by later patches (see §1). The first attempt
     used `basehousenumber + 100`, which resolved to house **120** at
     registration time (basehousenumber was 20) but to house **152** at
     save/load time (steel left it at 52). Save/load keyed off house 152, which
     was empty — so **nothing persisted**. An absolute id is immune.
   - House **120** is *also* produced by `spiritwood.txt` (`99 + 21`), so it
     held both the guild's shelves and spiritwood's — keying on it would sweep
     spiritwood storage into the guild file. House **200** is unreachable by any
     patch's `basehousenumber + offset` (max basehousenumber is 99) and is clear
     of every id in use (top ~124).

   House 200 is never made purchasable (no `housecost`), so no player ever has
   `GNPCflags[28] == 200` → the per-player logout save/remove path **never
   touches these shelves** → items do not decay. The tiles are still real
   `housestorage` slots, so the stolen-item block, 8-stack limit, and communal
   drop handling keep working.

2. **Patch change** — `guardianguild.txt`:
   - Both storage rooms' shelf tiles (7 surface + 7 basement = **14 slots**) are
     registered to the absolute `GUARDIANGUILD_STORAGE_HOUSE`.
   - Each surface tile gets a `279` furniture head (the basement already had
     them); without a head, dropped items become the tile head and the storage
     save/load (which operates on `->next`) would skip them — that was why the
     surface room never saved.
   - The redundant `+26` duplicate registration was deleted (it referenced the
     basement tiles and would have caused double-counting / double-restore).
   - The guild **building** (`house 46`) is left ownable exactly as before; only
     its storage was decoupled.

3. **Persistence** — `function_host.cpp`: `guardianguild_save()` /
   `guardianguild_load()`, declared in `function_host.h`. Both use
   `gh = GUARDIANGUILD_STORAGE_HOUSE` (the absolute id) and iterate
   `housestoragenext[gh]` slots, so they cover **both rooms** automatically. Each
   emits a `GUARDIANGUILD: load/save house=200 shelfslots=14 ...` line to
   `log.txt` for verification.

### `guardianobjs.sav` file format

Host-only, little-endian. **Not a wire message**, so it does *not* bump
`U6O_VERSION`.

```
u16 magic       = 0x6747 ('Gg')
u16 version     = 1
u32 tilecount   = housestoragenext[guildhouse]
repeat tilecount times:
    u32 nobj                 // flattened object count for this shelf tile
    repeat nobj times:
        u16 type
        u16 info
        u32 more2            // raw more2 (quantity/etc) for non-containers; 0 for containers
        u32 parent           // 0 = sits directly on the shelf tile;
                             //   else 1-based index, within THIS tile's object
                             //   list, of the container holding this object
```

### Save algorithm (`guardianguild_save`)

For each shelf slot of the guild house:

1. `root = od[ty][tx]->next` — skip the furniture head; only stacked items.
2. `OBJlist(root)` to flatten items + container contents (parents precede
   children).
3. Write `nobj`, then per object:
   - Apply the same **invalid-item guard** the player-save uses: a weightless
     object is rewritten to a bag (if it's a container) or a rubber ducky
     (`type 169`), `info 112`, so it can't corrupt on reload.
   - `more2` is written raw for non-containers; `0` for containers (their
     pointer union is meaningless to persist).
   - `parent` is found by walking `->prev`: a child's prev-chain leads back to
     the container whose `->more` starts the chain; a top-level shelf item's
     prev-chain ends at the furniture head / `NULL` → `parent = 0`.

The whole buffer is written with `put()` to `.\save\guardianobjs.sav`
(overwriting), so emptied shelves correctly shrink the file.

### Load algorithm (`guardianguild_load`)

Called at startup after `house()` has registered the shelf slots:

1. Open `.\save\guardianobjs.sav`; if missing (first run) or shorter than the
   8-byte header, return.
2. Validate `magic` and `version`.
3. For each tile (guarded against the shelf layout having shrunk), read `nobj`
   and recreate each object with `OBJnew()`:
   - `parent == 0` → `OBJadd(tx, ty, o)` (appends after the furniture head,
     preserving order).
   - otherwise → append to the **end** of the parent container's `->more` chain
     (preserving original order; the parent always exists already because
     parents precede children in the stream).

### Invariants / gotchas

- **Always skip the furniture head** (`od[...]->next`) on save; `OBJadd` on load
  re-appends after the furniture that `house()` placed. Never serialize or
  recreate the shelf furniture itself.
- The dedicated storage house **must stay non-ownable**. If a purchase path ever
  assigns house id `120` to a player, logout would start wiping the shelves
  again. Keep `housecost`/`houseinitialcost` unset for it.
- `parent` indices are **per-tile** (each tile's object list restarts at index
  `0`), matching the per-tile `OBJlist` flatten.
- This file format is independent of the player `.sav` format; changing it only
  requires keeping `guardianguild_save`/`guardianguild_load` in sync (bump the
  local `version` field if the layout changes). No `U6O_VERSION` bump — it is
  not a host↔client wire format.

---

## 7. File / symbol reference

| Concern | Location |
| --- | --- |
| Guild patch (tiles, furniture, storage registration) | `assets/map_patches/guardianguild.txt` |
| Dedicated storage-house offset + rationale | `src/server/define_host.h` (`GUARDIANGUILD_STORAGE_HOUSEOFFSET`) |
| `guardianguild_save` / `guardianguild_load` | `src/server/function_host.cpp`, declared in `function_host.h` |
| Startup load hook | `src/server/setup_host.inc` (after `house()` / `host_setup.h`) |
| Shutdown save hook | `src/common/u6o7.cpp` (HOST WM_QUIT block) |
| House build entry point | `src/common/house.cpp` `house()` |
| Player save/restore + storage add/remove | `src/server/loop_host.cpp` (labels `housestorageadd`, `housestoragerestore`) |
| `house.sav` write / read | `function_host.cpp` `housesav_update()` / `src/server/host.inc` |
| Object model / `OBJlist` / `OBJadd` | `src/server/function_host.cpp`; `struct object` in `src/common/data_both.h` |
| File + `txt` helpers | `src/common/myfile.{h,cpp}`, `src/common/txt.cpp`, `src/common/mytxt.h` |

## 8. Building the host

The project builds **x86**. Use the matching VS2022 environment so the Windows
SDK headers resolve for the pinned `cl.exe`:

```powershell
cmd /c '"C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsamd64_x86.bat" && cmake --build "cmake-build-debug-visual-studio" --target host'
```

Targets: `host` (dedicated server), `client`, `both` (combined full build that
defines both `HOST` and `CLIENT`). Outputs land in `bin\{host,client}\debug\`.

