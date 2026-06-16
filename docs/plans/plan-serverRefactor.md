# Server Modernization Plan (Trackable)

> ⚠️ **SUPERSEDED (2026-06-16).** This plan predates the `LHS` loop split — its
> P4.4 still describes a monolithic `loop_host.cpp` (~30k lines) that no longer
> exists (the host loop is now 35 ordered `#include` fragments under
> `src/server/loop/`). The active, loop-split-aware successor is
> [`modernization/plan-modernize-server.md`](./modernization/plan-modernize-server.md)
> (phase prefix `MSRV-P*`), sequenced by
> [`modernization/README.md`](./modernization/README.md). This file is kept for
> historical reference; do **not** start new work from it.

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before starting the next phase.

> Source of truth: this file. Keep PR descriptions linking back to the phase IDs (P1.2, etc.).
> Original brief lives in `plan-serverRefactor.prompt.md`.

---

## P0 — Safety net (do FIRST, no behaviour changes)

- ⬜ P0.1 Capture a baseline build (Debug + Release) and record warnings count.
- ⬜ P0.2 Add a "golden run" smoke test: start host, connect 1 client, log 60s of packets to a recorded fixture.
- ⬜ P0.3 Enable `/W4 /permissive-` (MSVC) on the server target only; suppress (don't fix) existing warnings via `#pragma` so deltas are visible.
- ⬜ P0.4 Add `clang-format` config matching current brace style; format only NEW files going forward.
- **Exit:** Repeatable build + smoke test command documented in this file.

## P1 — Headers & constants (`define_host.h`, header hygiene)

- ⬜ P1.1 Replace `#ifndef DEFINE_HOST_H` guard with `#pragma once` in all `src/server/*.h`. _(`define_host.h`, `data_host.h`, `function_host.h` use `#pragma once` alongside the existing guards.)_
- ⬜ P1.2 Convert numeric `#define`s in `define_host.h` to `inline constexpr`. _(`u6o::server::kSaveSlotLast`, `kAdminsMax`, `kHirelingsMax`, `kHouseMax`, `kHouseStorageSlotMax`, `kDeathPos{X,Y}`, `kSaveVersion`, `kMoverFound`, `kFirstClient`. Legacy macros remain as deprecated aliases until P7.)_
- ⬜ P1.3 Convert spell-result `#define`s to `enum class SpellResult { NotDone, Success, Invalid, Failure }`. _(Legacy `SPELL_*` macros now alias to `static_cast<int>(SpellResult::…)`.)_
- ⬜ P1.4 Delete `U6O_HONESTY`-style stat-mutating macros; replace with `inline` functions taking `npc&` (`applyHonestyShrine`, …) in a new `ShrineEffects.h`. _(All 56 call sites in `loop_host.cpp` migrated; macros removed from `define_host.h`.)_
- ⬜ P1.5 Fix `#endif FUNCTION_HOST_H` (invalid token after `#endif`) → `#endif // FUNCTION_HOST_H`.
- ⬜ P1.6 Split `function_host.h` into themed headers: `Networking.h`, `AutoPickup.h`, `HouseSystem.h`, `Pathfinding.h`, `Conversation.h`, `Stealing.h`, `Wind.h`, `ObjectOps.h`, `Misc.h`. Old header becomes a thin umbrella include.
- **Exit:** No magic numbers from `define_host.h` remain in server `.cpp`/`.inc` files; build green.

## P2 — Eliminate dangerous overloads & raw memory (`function_host.cpp`)

- ⬜ P2.1 Rename `void free(player*)`, `free(creature*)`, `free(npc*)` to `destroyPlayer`, `destroyCreature`, `destroyNpc`; update all call sites.
- ⬜ P2.2 Replace the `malloc(sizeof(object*) * N)` for `mlobj` with a typed helper. _(Full RAII conversion deferred until P5.3 `ObjectManager` introduction.)_
- ⬜ P2.3 Audit every `free(...)` call → ensure each is paired with the right allocator; document allocator per type at the top of a new `Memory.md` table.
- ⬜ P2.4 Wrap `txt*` lifetime in a `TxtPtr` RAII (custom deleter calling existing `txtfree`); migrate one subsystem first as proof.
- **Exit:** Zero raw `malloc`/`free` calls in `function_host.cpp`; ASan/Application-Verifier clean on smoke test.

## P3 — Encapsulate global state (`data_host.h` / `data_host.cpp`)

- ⬜ P3.1 Inventory every `extern` in `data_host.h`; classify into: WorldGrid, ObjectPool, VlnkPool, ResurrectQueue, RespawnQueue, MoongateState, WindState, HouseSaves, EconomyTables, ScheduleTables, NpcRegistry, SfxBuffer, WizardEyeState, AutoSetupState, KeybTarget, MoverNew, ReviveLoopExit, Misc.
- ⬜ P3.2 For each group, create a struct (e.g. `struct ObjectPool { std::vector<object> buffer; std::vector<uint32_t> freeList; ... };`) in its own header.
- ⬜ P3.3 Aggregate all groups in a `ServerState` singleton (constructed at startup); replace `extern` access with `ServerState::Get().objectPool.…` via a deprecation shim (inline references mapping old names to new accessors during transition).
- ⬜ P3.4 Remove the `#ifndef CLIENT` Win32 stub block from `data_host.h` — move to a server-only `WindowsStubs.cpp` or delete if truly unused.
- ⬜ P3.5 Fix bug in `data_host_init`: local `object* nuggetsfix = NULL;` shadowed the global.
- ⬜ P3.6 Replace `unsigned char btu6[1024][1024]`, `od[1024][2048]`, etc. with strongly-typed `Grid2D<T, W, H>` template (bounds-checked in Debug).
- **Exit:** `extern` count in `data_host.h` reduced to 0; old names exist only as deprecated forwarders.

## P4 — Eliminate `.inc` files (largest effort)

- ⬜ P4.1 `setup_host.inc` → `ServerStartup.cpp` with `void RunServerStartup()` calling: `LogStartupTime()`, `InitWinsock()`, `BindListenSocket()`, etc.
- ⬜ P4.2 `host_setup.h` → split into `MapFixes.cpp` (LB castle drawbridge, Sutek, Codex, potions, runes, balloon plans, …) with one named function per fix and a `WorldFixesRegistry` running them in order. Replace magic tile numbers with named `OBJ_*` constants in a shared `ObjectTypes.h`.
- ⬜ P4.3 `host.inc` (3662 lines) → segment by responsibility into `BanList.cpp`, `MoongateInit.cpp`, `OrbDestinations.cpp`, `EconomySetup.cpp`, `RespawnInit.cpp`, etc. Each gets a header & a function called from `ServerStartup`.
- ⬜ P4.4 the body-only `loop_host.cpp` (~30k lines, compiled via `#include` into `u6o7.cpp`) → introduce `class HostLoop` with `Tick()` calling sub-routines: `ProcessIncomingMessages()`, `UpdateMovers()`, `UpdateDoors()`, `UpdateLevers()`, `UpdateMoongates()`, `UpdateWeather()`, `UpdateRespawns()`, `UpdateAutoSave()`, `UpdateWizardEyes()`, `DispatchSfx()`. Each new TU is < 1000 lines.
- ⬜ P4.5 Delete the `goto`-based message dispatch (`nextclientmessage:` / `host_gotmessage:`) → replace with a `MessageDispatcher` mapping packet-id → handler function pointer/`std::function`.
- ⬜ P4.6 Re-format the line-broken legacy code (`OBJremove\n(\nmyobj3\n);`) using `clang-format` once it's relocated into `.cpp` files.
- **Exit:** `src/server/*.inc` and `host_setup.h` deleted; `u6o7.cpp` no longer `#include`s server `.inc` files.

## P5 — Decompose god-functions (`function_host.cpp`)

- ⬜ P5.1 `addu6monsterdropitems` (~600 lines) → `MonsterLootTable` data + `LootRoller::Roll(monster, out)`; remove `goto karma_spec` via early-return helpers.
- ⬜ P5.2 `wpf_pathfind` (~750 lines, 7 `goto` labels) → `Pathfinder` class with `Init`, `Step`, `Reconstruct`; replace WPF globals with members.
- ⬜ P5.3 `OBJremove`, `OBJaddtocontainer`, `OBJmove`, `OBJfindlast*`, `OBJlist` → `ObjectManager` class; eliminate `OBJtmp/OBJtmp2/OBJtmp3` scratch globals.
- ⬜ P5.4 `AUTOPICKUP*` globals + functions → `AutoPickupQueue` class (ring buffer of `Entry`).
- ⬜ P5.5 `housesav_update`, `house()`, `housecheck`, `houseowner` → `HouseSystem` class owning the `HOUSEMAX` arrays.
- ⬜ P5.6 `CON*` globals → `ConversationContext`; pass by reference.
- ⬜ P5.7 Replace remaining `goto` flow with structured loops/early return.
- **Exit:** No function in `src/server` exceeds 150 lines; `goto` count = 0.

## P6 — Function signatures & types

- ⬜ P6.1 Convert `unsigned char` flag params to `bool` or `enum class` flags.
- ⬜ P6.2 Replace `unsigned char* d` (raw byte buffer) params with `std::span<std::byte>`; `txt*` text params with `std::string_view` at boundaries (`txt*` retained internally until P7).
- ⬜ P6.3 Apply camelCase to public symbols; snake_case for locals; PascalCase for types — document in this file's "Style" appendix.
- ⬜ P6.4 Mark all read-only params `const`; mark non-throwing helpers `noexcept`.
- **Exit:** Server public API (umbrella header) uses no raw C-style buffers.

## P7 — Magic numbers & strings

- ⬜ P7.1 Build `ObjectTypes.h` enumerating every `OBJ_*` referenced (current scattered defines + numeric literals like `269`, `275`, `416`, `334`, `297..300`, `414`, `415`).
- ⬜ P7.2 Replace tile-class shifts (`+ N * 1024`) with `MakeObjectType(base, variant)` helper; document the 10-bit base / variant layout.
- ⬜ P7.3 Move file paths (`".\\dr\\objfixed.bin"`, `"ban.txt"`, …) to a `Paths` namespace using `std::filesystem::path`.
- ⬜ P7.4 Move user-facing strings to a `ServerStrings.h` (prep for future localization).
- **Exit:** No bare numeric literals > 9 in server logic except in tables/enums; no string literals outside `ServerStrings`/`Paths`.

## P8 — Cleanup & enforcement

- ⬜ P8.1 Re-enable `/W4 /WX` server-wide (remove suppressions added in P0.3).
- ⬜ P8.2 Add CI step running `clang-tidy` (modernize-*, readability-*, cppcoreguidelines-*) on `src/server`.
- ⬜ P8.3 Update `update_cmake.py` and `CMakeLists.txt` to glob the new file layout.
- ⬜ P8.4 Archive original files under `src/server/legacy/` for historical diffing, then remove after one stable release.
- **Exit:** Server target builds warning-free at `/W4 /WX`; `clang-tidy` baseline established.

---

## Cross-cutting tracking

| Metric | Baseline | Current | Target |
|---|---|---|---|
| Server `.cpp` LOC | TBD | TBD | -30% |
| Largest function (lines) | ~750 (`wpf_pathfind`) | ~750 | ≤150 |
| `goto` occurrences (server) | 17+ (function_host) + loop_host labels | TBD | 0 |
| `.inc` files in `src/server` | 3 + `host_setup.h` | 3 + 1 | 0 |
| `extern` count in `data_host.h` | ~120 | 0 | 0 |
| Magic literals (numeric > 9) | very high | TBD | <50 (data tables only) |

Update this table at the end of every phase.

## Style appendix (target)

- Types: `PascalCase` (`ObjectManager`, `HouseSystem`).
- Functions / methods: `camelCase` (`addItem`, `findLast`).
- Local variables: `camelCase` (`partyMember`, `tileId`).
- Constants / enumerators: `kCamelCase` (`kHouseMax`) or `PascalCase` enum-class members.
- Files: `PascalCase.{h,cpp}` for new TUs (`HouseSystem.h`).
- Prefer `#pragma once`, `enum class`, `constexpr`, `std::array`, `std::vector`, `std::span`, `std::string_view`, `std::unique_ptr`.

## Session log (append-only)

