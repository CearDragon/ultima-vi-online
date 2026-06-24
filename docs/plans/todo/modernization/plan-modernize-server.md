# `src/server/` Modernization Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before the next phase starts.

> Source of truth: this file.
> Phase ID prefix: **`MSRV-P*`** ("Modernize SeRVer"). Tag PR titles and code
> comments with the phase ID (`MSRV-P5.2:`, etc.).
> Read [`README.md`](README.md) first — Prime Directive, risk tiers (T0–T3),
> the `MOD-P0` shared safety net, and the coordination rules.
> **Supersedes** [`docs/plans/plan-serverRefactor.md`](../../plan-serverRefactor.md)
> (which predates the `LHS` loop split and is now loop-stale). That plan's `P*`
> content is folded in below and made loop-split-aware.
> Execute with the **`cpp-modernizer` agent**.

---

## 0. What is in scope

`src/server/` is compiled only into `host` and `both` (`HOST` defined; `host`
also defines `CONSOLE`). It is the authoritative game simulation.
The per-tick loop was decomposed into 35 ordered `#include` fragments by
**`LHS-P*`** (complete) — those are the raw material for the hot-path phases here.

| File(s) | Lines | Role | Dominant hazard |
|---|---|---|---|
| `function_host.cpp` | 5820 | god-functions: `wpf_pathfind` (~750), `addu6monsterdropitems` (~600), `OBJ*` manager, `AUTOPICKUP*`, house system, `CON*`, `free()` overloads, **1 `__asm` (~4769, `roundfloat`)** | T1/T2 + T3 asm |
| `loop/loop_host_part_*.cpp` (35 parts) | ~26,000 | per-tick block: per-player UPDATE (**sobj/mover encode**), client-message **dispatch**, **AI update**, per-player **INPUT**, NPC-frame encode | T2 wire + T3 AI — brace-seam, move-only edit tools |
| `host.inc` | 3094 | startup: ban list, moongates, orb destinations, economy, respawn init | T1 (non-TU include) |
| `host_setup.h` | 1243 | map fixes (LB castle, Sutek, Codex, potions, runes, balloon) — magic tile numbers | T1 (non-TU include) |
| `setup_host.inc` | 259 | server startup (winsock, listen socket) | T1 (non-TU include) |
| `data_host.h` | 291 | ~120 `extern`s: `od[][]`, object pool `objb[]`, vlnk, respawn/resurrect queues, moongates, wind, houses, economy, schedules, crtenum, wizard-eye, player list | global encapsulation + raw grids |
| `function_host.h` | 404 | host prototypes + ~120 `extern`s + invalid `#endif FUNCTION_HOST_H` token | header hygiene |
| `define_host.h` | 72 | `SAVEVER`, `HOUSEMAX`, `U6O_*` stat-mutating macros, `SPELL_*` | constants + dangerous macros |
| `data_host.cpp` | 72 | `data_host_init` (has a shadowing bug noted below) | T1 |

**Coordination:** the sobj/mover encode in the loop parts and the `MV_TX_*` /
`SOBJ_TX_*` / `sobj_*` / `mv_*` family are **`DOB-P*`-owned** (T2/wire) — encode
parts are move/structure-only until DOB lands or is coordinated. The
`platform/plat_*.h` seam is **`LH-P*`-owned**.

---

## MSRV-P1 — Header hygiene, constants & dangerous macros

- ⬜ `MSRV-P1.1` Fix the invalid `#endif FUNCTION_HOST_H` token →
  `#endif // FUNCTION_HOST_H`; add `#pragma once` to `define_host.h`,
  `data_host.h`, `function_host.h`, `host_setup.h`. **T0.**
- ⬜ `MSRV-P1.2` Convert numeric `#define`s in `define_host.h` to `inline
  constexpr` in `u6o::server` with deprecated aliases: `kSaveSlotLast`,
  `kAdminsMax`, `kHirelingsMax`, `kHouseMax`, `kHouseStorageSlotMax`,
  `kDeathPos{X,Y}`, `kSaveVersion`, `kMoverFound`, `kFirstClient`,
  `kGuardianGuildStorageHouse`. **T0** (values unchanged — `SAVEVER` is `.sav`
  coupled; keep the value).
- ⬜ `MSRV-P1.3` Convert the spell-result `SPELL_*` defines to
  `enum class SpellResult { NotDone, Success, Invalid, Failure }` with the legacy
  `SPELL_*` macros aliasing `static_cast<int>(...)`. **T0.**
- ⬜ `MSRV-P1.4` Delete the `U6O_HONESTY`-style **stat-mutating macros**
  (`tnpc->i+=3;` etc.) and replace with `inline` functions taking `npc&`
  (`applyHonestyShrine(npc&)`, …) in a new `ShrineEffects.h`. Migrate all call
  sites in the loop parts. **T1** (characterize a shrine interaction).
- ⬜ `MSRV-P1.5` Split `function_host.h` into themed headers (`Networking.h`,
  `AutoPickup.h`, `HouseSystem.h`, `Pathfinding.h`, `Conversation.h`,
  `Stealing.h`, `ObjectOps.h`, `Misc.h`); old header becomes a thin umbrella.
  **T1.**
- **Exit:** invalid `#endif` fixed; all server `.h` use `#pragma once`; numeric
  constants named; `SPELL_*` is an enum class; stat macros gone; `function_host.h`
  is an umbrella; `host`+`both` green.

## MSRV-P2 — Eliminate dangerous overloads & raw memory (`function_host.cpp`)

- ⬜ `MSRV-P2.1` Rename the `free(player*)`, `free(creature*)`, `free(npc*)`
  **overloads of `free`** to `destroyPlayer` / `destroyCreature` / `destroyNpc`;
  update every call site (grep first — they shadow `std::free`). **T1.**
- ⬜ `MSRV-P2.2` Replace the `malloc(sizeof(object*) * N)` for `mlobj` (lines
  ~1253/1279) and `malloc(sizeof(creature))` (~1133) with typed helpers; pair
  each with its release. Full RAII deferred to P5.3 (`ObjectManager`). **T1.**
- ⬜ `MSRV-P2.3` Document the allocator-per-type table in a new
  `docs/modernization/server-memory.md`; audit every `free(...)` is paired with
  the right allocator. **T1** (Application-Verifier clean on the host smoke run).
- ⬜ `MSRV-P2.4` Convert the `__asm` `roundfloat` (~4769) to portable C++ behind
  the same signature — coordinate with `LH-P1.2`, which already guards it
  (`__asm` on Windows, `__builtin_lrintf` elsewhere). Confirm identical rounding
  (half-to-even vs truncation) for the host's input range. **T3** (benchmark +
  value parity).
- **Exit:** zero `free`-overloads; zero bare `malloc`/`free` in
  `function_host.cpp`; `roundfloat` portable + value-identical; `host`+`both`
  green.

## MSRV-P3 — Encapsulate global state (`data_host.h` / `data_host.cpp`)

- ⬜ `MSRV-P3.1` Inventory every `extern` in `data_host.h` (~120) and classify:
  `WorldGrid` (`od[][]`, `btu6[][]`), `ObjectPool` (`objb[]`, `objb_free[]`),
  `VlnkPool`, `ResurrectQueue` (`resu*`), `RespawnQueue`, `MoongateState`,
  `WindState`, `HouseSaves` (`housesav[]`), `EconomyTables`, `ScheduleTables`,
  `CrtEnumState`, `WizardEyeState`, `AutoSetupState`, `PlayerList`, `SfxBuffer`,
  `Misc`. Record the `.sav` / wire / byte-blit use of each.
- ⬜ `MSRV-P3.2` Fix the `data_host_init` bug: local `object* nuggetsfix = NULL;`
  shadows the global `nuggetsfix`. **T1** (characterize the nugget fix path).
- ⬜ `MSRV-P3.3` For each **non-`.sav`, non-wire** group, create a `struct` in
  its own header; aggregate into a `ServerState` accessed via inline shims that
  map old global names to the new accessors during transition. **T1.** Start with
  groups that are *not* byte-blitted or persisted.
- ⬜ `MSRV-P3.4` Replace the raw `unsigned char btu6[1024][1024]`,
  `object* od[1024][2048]`, etc. with a `Grid2D<T, W, H>` template (bounds-checked
  in Debug, zero-overhead in Release). **T1** (Debug bounds check must not change
  Release behavior; benchmark the hot grid accesses).
- ⬜ `MSRV-P3.5` Move the `#ifndef CLIENT` Win32 stub block out of `data_host.h`
  into a server-only `WindowsStubs.cpp` (or delete if unused). **T1.**
- **Exit:** extern inventory complete with persist/wire flags; shadowing bug
  fixed; ≥2 non-persisted groups encapsulated; grids are `Grid2D`; stubs moved;
  `host`+`both` green and `.sav` round-trip unchanged.

## MSRV-P4 — Eliminate the `.inc` startup files

- ⬜ `MSRV-P4.1` `setup_host.inc` → `ServerStartup.cpp` with `void
  RunServerStartup()` calling `logStartupTime()`, `initWinsock()`,
  `bindListenSocket()`, etc. (coordinate the socket calls with `LH-P3`/`P5`).
  **T1.**
- ⬜ `MSRV-P4.2` `host_setup.h` → `MapFixes.cpp`: one named function per fix (LB
  castle drawbridge, Sutek, Codex, potions, runes, balloon plans) run in order by
  a `WorldFixesRegistry`. Replace the magic tile numbers with the shared `OBJ_*`
  enum (`MCOM-P1.3`). **T2-adjacent** (these write world tiles that the encode
  later transmits — golden-capture a fresh world build and diff).
- ⬜ `MSRV-P4.3` `host.inc` (3094 lines) → segment by responsibility:
  `BanList.cpp`, `MoongateInit.cpp`, `OrbDestinations.cpp`, `EconomySetup.cpp`,
  `RespawnInit.cpp`, each with a header and a function called from
  `RunServerStartup`. Re-format the line-broken legacy code once relocated.
  **T1.**
- **Exit:** `src/server/*.inc` and `host_setup.h` are gone; `u6o7.cpp` no longer
  `#include`s server `.inc` files; a fresh world build is byte-identical to the
  pre-refactor world; `host`+`both` green.

## MSRV-P5 — Decompose god-functions (`function_host.cpp`)

- ⬜ `MSRV-P5.1` `addu6monsterdropitems` (~600 lines) → a `MonsterLootTable`
  data structure + `LootRoller::roll(monster, out)`; remove `goto karma_spec` via
  early-return helpers. **T2 (RNG)** — the loot roll consumes RNG; preserve draw
  count + order exactly (capture before/after roll sequences).
- ⬜ `MSRV-P5.2` `wpf_pathfind` (~750 lines, 7 `goto` labels) → a `Pathfinder`
  class with `init`/`step`/`reconstruct`; move the `wpf_*` globals to members.
  **T1** (characterize a fixed set of source/dest pairs → identical paths).
- ⬜ `MSRV-P5.3` `OBJremove`/`OBJaddtocontainer`/`OBJmove`/`OBJfindlast*`/
  `OBJlist`/`OBJnew` → an `ObjectManager` class; eliminate the `OBJtmp/OBJtmp2/
  OBJtmp3` scratch globals. **T1** (object pool is hot-ish — benchmark add/remove).
- ⬜ `MSRV-P5.4` `AUTOPICKUP*` globals + functions → an `AutoPickupQueue` (ring
  buffer of `Entry`). **T1.**
- ⬜ `MSRV-P5.5` `housesav_update`/`house()`/`housecheck`/`houseowner` →
  `HouseSystem` owning the `HOUSEMAX` arrays; document the `.sav`-coupled
  `housesav_info` byte layout (do not change it). **T2** for the save path.
- ⬜ `MSRV-P5.6` `CON*` conversation globals → `ConversationContext` passed by
  reference. **T1.**
- ⬜ `MSRV-P5.7` Replace remaining `function_host.cpp` `goto` flow with structured
  loops / early return. **T1.**
- **Exit:** no function in `function_host.cpp` exceeds ~150 lines; `goto` count in
  the file = 0; RNG draw order in `LootRoller` proven identical; object/house
  managers benchmarked; `host`+`both` green.

## MSRV-P6 — Modernize the `loop_host` parts (per-tick block) ⚠️ T2/T3

Brace-seam, non-TU fragments. **Edit only with `replace_string_in_file` (tight
context) or the `tools/loop_split_*` tools — never `insert_edit_into_file`.**
Work one self-contained label region at a time; don't sever a cross-part `goto`.
Re-run `tools/loop_split_scan_host.ps1` before each cut.

- ⬜ `MSRV-P6.1` **MEGA C — AI update (T3 hot path)**, parts `c_objupdate`,
  `c_crtenum`, `c_npccast`, `c_traps_schedule`, `c_npcwalk_follow`,
  `c_npcattack_{a,b,c}`. De-`goto` the creature-enum / targeting / attack chains
  into named helpers; **benchmark each tick segment** before/after; **RNG
  draw-order frozen** (combat + AI consume RNG — capture sequences). No STL in the
  innermost loops unless benchmark-proven.
- ⬜ `MSRV-P6.2` **MEGA A — UPDATE + sobj/mover encode (T2 wire)**, parts
  `a_save`, `a_housestore`, `a_sobj`, `a_mover`, `a_display_sfx`. The `a_save`
  and `a_housestore` non-encode regions are T1 (de-`goto`/name); the `a_sobj`/
  `a_mover` **encode** is **DOB-owned** — structure/relabel only, byte-stream
  prove the emitted update message is identical, `U6O_VERSION` untouched.
- ⬜ `MSRV-P6.3` **MEGA B — client-message dispatch (T2 + admin)**, parts
  `b_dispatch`, `b_housestore_restore`, `b_charcreate`, `b_admin`,
  `b_admin_exec`, `b_slash`, `b_chat_npc`, `b_leave_resurrect_house`. Replace the
  `host_gotmessage:` `goto` dispatch with a documented `MessageDispatcher`
  (packet-id → handler) — **the decoded bytes per message must be identical**
  (byte-stream capture). Update the stale `loop_host.cpp:<line>` refs in
  `docs/admin/README.md`.
- ⬜ `MSRV-P6.4` **MEGA D — per-player INPUT (T2 wire)**, parts `d_open`,
  `d_cast`, `d_chainbolt`, `d_summon_untrap`, `d_weather_mass`, `d_joinitem_wep`,
  `d_lockpick_drop`, `d_skiff`, `d_use_a`, `d_use_b`, `d_look`, `d_ship`. De-`goto`
  the item-use / spell / look dispatch; input is read from the wire so
  characterize against recorded input messages; RNG-frozen for spell/combat.
- ⬜ `MSRV-P6.5` **Tail — NPC-frame encode (T2 wire)**, `tail_npcframe`. The
  mover/mobj encode is move/structure-only + byte-proven; the `oul` cleanup and
  `#ifndef _DEBUG` quit are T1.
- **Exit:** each part's `goto` chains are structured *within* the part (cross-part
  jumps documented); MEGA C benchmarked with no regression; the encode/decode/
  dispatch parts byte-proven identical; `U6O_VERSION` untouched; `host`+`both`
  build green.

## MSRV-P7 — Types, magic numbers & strings

- ⬜ `MSRV-P7.1` Convert `unsigned char` flag params to `bool` / `enum class`
  flags; mark read-only params `const`, non-throwing helpers `noexcept`. **T1.**
- ⬜ `MSRV-P7.2` Replace remaining tile-class shifts (`+ N * 1024`) with a named
  `makeObjectType(base, variant)` helper documenting the 10-bit base / variant
  layout; replace numeric literals (`269`, `275`, `334`, `416`, …) with the
  shared `OBJ_*` enum. **T1.**
- ⬜ `MSRV-P7.3` Move file paths (`".\\dr\\objfixed.bin"`, `"ban.txt"`, …) into a
  `Paths` namespace; move user-facing strings to `ServerStrings.h`. **T1** (mind
  any `txt`-byte-exact comparison).
- **Exit:** no bare numeric literal > 9 in server logic outside tables/enums; no
  string literals outside `ServerStrings`/`Paths`; `host`+`both` green.

## MSRV-P8 — Finalize & enforce

- ⬜ `MSRV-P8.1` Remove deprecated shims from P1/P3 once all call sites migrate.
- ⬜ `MSRV-P8.2` Raise `/W4 /permissive-` on the server target; clear new
  warnings; add a `clang-tidy` (`modernize-*`, `readability-*`,
  `cppcoreguidelines-*`) baseline over `src/server`.
- ⬜ `MSRV-P8.3` Update `CMakeLists.txt` for the new file layout; ensure the new
  `loop/` parts and split startup TUs are listed for IDE indexing (compiled via
  `#include`, so `HEADER_FILE_ONLY` where appropriate).
- ⬜ `MSRV-P8.4` Update the §6 dashboard in `README.md`; write
  `docs/modernization/server-*.md` records; mark `plan-serverRefactor.md` fully
  superseded.
- **Exit:** server builds warning-free at `/W4`; `clang-tidy` baseline set; CMake
  globs the new layout; records + dashboard updated; `host`/`both` behaviorally
  + `.sav`/wire equivalent to baseline.

---

## Invariants (apply to EVERY phase)

1. **`host` + `both` both build** after every increment.
2. **Wire/`.sav` frozen.** Encode/decode/dispatch parts change no bytes; the
   `MV_TX_*`/`SOBJ_TX_*`/`sobj_*`/`mv_*` family is `DOB-P*`-owned; `housesav_info`
   and the save buffers keep their byte layout; do not bump `U6O_VERSION`.
3. **RNG order frozen.** AI, combat, and loot consume RNG — any touch requires a
   call-count + order proof.
4. **Hot path = benchmark.** MEGA C / AI / object-pool changes need a before/after
   benchmark with no meaningful regression.
5. **Brace-seam parts.** Use `replace_string_in_file` / `tools/loop_split_*`,
   never `insert_edit_into_file`, on `loop/loop_host_part_*.cpp`; re-scan before
   each cut; don't sever a cross-part `goto`.
6. **Platform seam is LH's.** Coordinate any `platform/` or socket-call change
   with `LH-P*`.

## Session handoff

- **2026-06-16 (drafted; supersedes `plan-serverRefactor.md`).** Plan committed;
  no code yet. Finish `MOD-P0` first, then start at **`MSRV-P1.1`**. Before
  P4.2 / P6 re-read the `DOB` plan handoff (wire buffers) and the `LH` plan
  handoff (sockets/platform). The 35-part loop layout + brace-seam rules live in
  `src/server/loop/README.md`.

