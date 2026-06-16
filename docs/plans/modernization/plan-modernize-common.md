# `src/common/` Modernization Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before the next phase starts.

> Source of truth: this file.
> Phase ID prefix: **`MCOM-P*`** ("Modernize COMmon"). Tag PR titles and code
> comments with the phase ID (`MCOM-P3.2:`, etc.).
> Read [`README.md`](./README.md) first — it owns the Prime Directive, the risk
> tiers (T0–T3), the `MOD-P0` shared safety net, and the coordination rules with
> the in-flight `DOB` / `LH` / `RW` plans.
> Execute with the **`cpp-modernizer` agent**.

---

## 0. What is in scope

`src/common/` is shared by all three targets; every change must compile and
behave identically under **`client`, `host`, and `both`**.

| File | Lines | Role | Dominant hazard |
|---|---|---|---|
| `define_both.h` | 1031 | shared `#define`s: `U6O_VERSION`, wire constants (`MV_TX_*`, `SOBJ_TX_*`), `OBJ_*` (452), `MSG_*`, `SF_*`, `KEY*`, `UI_*`, `SOUND_*`, `BUILD_*` | wire-coupled constants (T2) |
| `data_both.h` | 587 | `struct object/player/npc/creature/sfxtype/…`, `int1..uint4` typedefs, ~150 `extern`s | `.sav`/wire byte layout (T2) |
| `globals.inc` | 1437 | the actual definitions of the shared `extern`s | byte-blit / order |
| `u6o7.cpp` | 1281 | `WinMain`, the main loop, crash handler, include hub | T1/T3 main loop |
| `house.cpp` | 1267 | house construction / map-patch geometry | T1 |
| `function_both.cpp` | 665 | `BITSadd`/`BITSget` (wire bit-packers), sockets threads (goto-heavy), `LOGadd`, `SHA2`, `getscreenoffset*`, `getroom`/`sameroom` | T2 bit-packers + T1 sockets |
| `function_both.h` | 105 | shared prototypes + `getnbits` macro | header hygiene |
| `txt.cpp` / `mytxt.h` | 538 / 94 | the custom `txt` string type (+1 inline `_asm`) | T2 (byte/length) + T3 asm |
| `myfile.{cpp,h}` | 403 / 89 | the custom `file` type (already dual-backend per LH-P2) | coordinate with LH |
| `random/` | ~320 | RNG (`Randomc`, Mersenne) | **RNG order (T2)** |
| `spell_code/`, `use_code/` | 395 / 205 | host gameplay tables | T1 |
| `platform/` | ~430 | LH portability seam | **owned by LH-P\*** — coordinate only |
| `data_both.cpp`, `stdafx.*`, `Xenkan.cpp`, `secret_*.inc`, `setup_both*.{h,inc}` | small | init/glue | T0/T1 |

**Out of scope here (owned by other plans):** the `sobj_*` / `mv_*` wire buffers
inside `struct player` and the `MV_TX_*` / `SOBJ_TX_*` constants
(**`DOB-P*`**); the `platform/plat_*.h` shim internals (**`LH-P*`**).

---

## MCOM-P1 — Header hygiene & constants

- ⬜ `MCOM-P1.1` Add `#pragma once` alongside the existing `#ifndef` guards in
  every `src/common/*.h` (`define_both.h`, `data_both.h`, `function_both.h`,
  `mytxt.h`, `myfile.h`, `setup_both_basic.h`). Keep the legacy guard for now.
- ⬜ `MCOM-P1.2` Convert the **non-wire** numeric `#define`s in `define_both.h`
  to `inline constexpr` in a `u6o::` namespace, with deprecated macro aliases:
  equip-slot coords (`helmx`…`necky`), `KEY*`, `SF_*`, `MSG_*`, `SOUND_*`,
  `BUILD_*`, `UI_*`, `MVLISTLAST`, `SOCKETLAST`, `MOVERNEW_FLAG_*`, `PF*`,
  `WPF_*`, `GETINPUT_*`, reagent flags. Each keeps a comment block (unit, math,
  coupling) mirroring `kViewportTilesXMax`. **T0** — values unchanged.
- ⬜ `MCOM-P1.3` Convert the 452 `OBJ_*` defines to a single strongly-typed
  `enum class ObjType : uint16_t { … }` (or a `constexpr` table) with the legacy
  `OBJ_*` macros aliasing the enumerators. Values must not change (they index
  `bt[]`, `obji[]`, and appear on the wire). **T2** for any site that compares an
  on-wire/`.sav` type — those stay byte-identical; verify with a save round-trip.
- ⬜ `MCOM-P1.4` Document the **wire-coupled** constants in place but do **not**
  change their values: `U6O_VERSION`, `MV_TX_*`, `SOBJ_TX_*` (the latter two are
  `DOB`-owned). Add `static_assert`s that pin the derived bit-widths
  (`MV_TX_BITS`, `SOBJ_TX_BITS`) to their documented `ceil(log2(...))` so a
  future edit that breaks the invariant fails to compile. **T0** (asserts only).
- ⬜ `MCOM-P1.5` Replace the `rnd`/`rndold` function-like macros and the
  `U6O2_*`/`getnbits` macros with `inline`/`constexpr` functions where it does
  **not** change evaluation (RNG order!) — `getnbits` first (pure), `rnd` only
  after confirming call-order identity. **T2** (RNG).
- **Exit:** every `.h` has `#pragma once`; non-wire magic numbers are named
  `constexpr`/`enum class`; wire constants documented + `static_assert`-pinned;
  tri-target build green; `U6O_VERSION` untouched.

## MCOM-P2 — The `txt` string + `file` types

- ⬜ `MCOM-P2.1` Add Doxygen docs to `struct txt` and every `txt*` API in
  `mytxt.h`, documenting the union aliasing, the `l`/`bl` length/capacity
  contract, and which call sites depend on exact bytes/length (wire/`.sav`).
- ⬜ `MCOM-P2.2` De-`goto` and modernize `txt.cpp` internals one function at a
  time (T1), preserving exact byte/length semantics. The inline `_asm` at
  `txt.cpp:147` is **T3**: convert to C++ behind the same signature, match
  integer width/signedness (`shr` vs `sar`), keep the asm under
  `#ifdef U6O_LEGACY_ASM` during A/B validation, prove byte-identical output +
  benchmark.
- ⬜ `MCOM-P2.3` Wrap `txt*` lifetime in a `TxtPtr` RAII (custom deleter calling
  the existing `free(txt*)`); migrate **one** subsystem as proof, leaving the raw
  API intact for the rest. **T1.**
- ⏭ `MCOM-P2.4` (defer) Evaluate a `std::string_view` boundary adapter for
  read-only `txt*` params — only at API boundaries, never where length/bytes are
  serialized. Coordinate with server/client plans that share these signatures.
- **Exit:** `txt`/`file` documented; `txt.cpp` `goto`-free; the inline asm is
  C++ behind a validated flag; one RAII-migrated subsystem builds tri-target.

## MCOM-P3 — Wire bit-packers & sockets (`function_both.cpp`)

- ⬜ `MCOM-P3.1` **T2 capture first.** Before touching `BITSadd`/`BITSget`,
  build a byte-stream harness in `tools/modernize/` that drives a matrix of
  `(value, nbits, start)` (including the `nbits>24` recursion path) and dumps the
  resulting buffer hex. Save goldens.
- ⬜ `MCOM-P3.2` Modernize `BITSadd`/`BITSget`: remove the `static` locals
  (`lp`/`shift`), name the fields, add a Doxygen block stating the exact bit
  layout, and keep the produced bit stream **byte-identical** (diff vs P3.1
  golden). **T2.** `U6O_VERSION` untouched.
- ⬜ `MCOM-P3.3` De-`goto` the socket threads (`sockets_send`,
  `sockets_receive`, `sockets_disconnect`) into structured loops/guard clauses;
  preserve evaluation order and the `volatile` flag polling exactly. **T1**
  (characterize against a recorded packet fixture from `MOD-P0`). Note the
  `malloc`/`free` buffer churn here for the RAII pass in P6.
- ⬜ `MCOM-P3.4` De-`goto` `LOGadd` (the `logadd0..2` chains are already dead in
  a comment — delete the dead labels; tidy the live path). Document `SHA2`,
  `getscreenoffset`/`getscreenoffset_legacy` (note the host-emits-legacy-32×24
  invariant), `getroom`/`sameroom`. **T0/T1.**
- **Exit:** bit-packers byte-proven identical; socket threads `goto`-free and
  fixture-equivalent; dead labels gone; tri-target build green.

## MCOM-P4 — `WinMain` & the main loop (`u6o7.cpp`)

- ⬜ `MCOM-P4.1` Document the include-hub contract at the top of `u6o7.cpp`
  (why the `#include` order is load-bearing, that the loop umbrellas are
  concatenated into the function scope, which locals the fragments rely on).
- ⬜ `MCOM-P4.2` Extract the crash-dump / signal-handler and the
  `WinMain`-vs-`main` entry glue into small named functions (T1); keep the
  `_WIN32` / POSIX split coordinated with `LH-P5`.
- ⬜ `MCOM-P4.3` Extract the frame-pacing / `mainloop:` timing logic into a named
  helper. **T3-adjacent** (it gates the per-frame cadence): characterize the
  frame-skip math and benchmark; do **not** change the tick/draw ordering or the
  RNG-relative timing. Leave the `goto mainloop` seam to the loop umbrellas
  intact.
- ⬜ `MCOM-P4.4` Replace the `#pragma warning(disable: …)` blanket suppressions
  with scoped fixes where the warning is genuinely benign, or document why each
  must remain. **T0.**
- **Exit:** `WinMain`/entry glue named + documented; frame pacing extracted with
  benchmark parity; main-loop `#include` seams unchanged; tri-target build green.

## MCOM-P5 — Encapsulate shared global state (`data_both.h` / `globals.inc`)

- ⬜ `MCOM-P5.1` Inventory every `extern` in `data_both.h` and classify into
  cohesive groups: `InputState` (`key*`, `mx/my`, `mb*`, `wheel_move`),
  `WindowState` (`clientW/H`, `blit_*`, `hInst`, `sz*`), `TileTables`
  (`bt[][]`, `tclass_*`, `objpassflags`, `sprlnk`), `MoverCompareBuffers`
  (`mv_*` locals — **DOB-adjacent, coordinate**), `StormCloakState`,
  `BritannianClock` (`btime*`, `bday`, `btimeh`), `BuildTables` (`hlocx/y`,
  `arena*`), `NetState`, `Misc`. Record the byte-blit / wire / `.sav` use of
  each group.
- ⬜ `MCOM-P5.2` For each **non-wire, non-blitted** group, introduce a `struct`
  in its own header and a single instance, with deprecated `#define`/reference
  shims mapping the old global names to the new accessors during transition.
  **T1.** Do **not** start with any group flagged byte-blitted or wire/`.sav`.
- ⬜ `MCOM-P5.3` Add Doxygen field docs to `struct object`, `player`, `npc`,
  `creature`, `sfxtype`, `objectinfo` — especially the bit-meaning comments on
  `object::info` and `npc::upflags`. **T0** (comments only; no layout change).
- ⬜ `MCOM-P5.4` Replace the `int1/uint1/int2/uint2/int4/uint4` typedefs with
  fixed-width `std::int8_t…uint32_t` aliases **only where size is identical on
  the i386 (-m32) ABI**; never widen a field in a serialized struct (`int`→
  `int32_t` ok; `int`→`size_t`/`long` not ok). **T2** for any serialized struct.
- **Exit:** extern inventory complete with blit/wire flags; ≥2 non-wire groups
  encapsulated behind shims; all shared structs documented; tri-target build
  green; no `.sav`/wire byte changed (save round-trip verified).

## MCOM-P6 — Memory ownership, `house.cpp`, RNG, gameplay tables

- ⬜ `MCOM-P6.1` Audit the `malloc`/`free` in `function_both.cpp` (socket
  receive buffers) and replace with a typed allocation helper or RAII; pair every
  allocation with its deleter. **T1** (Application-Verifier clean on the fixture).
- ⬜ `MCOM-P6.2` Modernize `house.cpp`: name the construction geometry magic
  numbers (reuse `BUILD_*` constants), extract repeated wall/floor/tile-option
  blocks into named helpers, de-`goto`. **T1** (golden-capture a representative
  house build).
- ⬜ `MCOM-P6.3` Document `random/` and add a `static_assert`/test pinning the
  draw sequence; modernize only the surface (naming, `constexpr`) **without
  changing the generated sequence or call order**. **T2 (RNG).**
- ⬜ `MCOM-P6.4` Modernize `spell_code/` and `use_code/` tables into documented
  `constexpr` data + small dispatch helpers (T1); these feed host gameplay so
  characterize against a host smoke fixture.
- **Exit:** zero raw `malloc`/`free` in `function_both.cpp`; `house.cpp`
  `goto`-free + named constants; RNG sequence pinned + unchanged; gameplay tables
  documented; tri-target build green.

## MCOM-P7 — Finalize & enforce

- ⬜ `MCOM-P7.1` Remove the deprecated macro/reference shims introduced in P1/P5
  once all call sites (including server/client and the `.inc` mirrors) are
  migrated; confirm with `grep_search`.
- ⬜ `MCOM-P7.2` Raise `/W4 /permissive-` on the shared TUs (where the three
  targets allow) and clear new warnings introduced by this plan.
- ⬜ `MCOM-P7.3` Update the §6 dashboard in `README.md` and write the
  `docs/modernization/common-*.md` execution records (scope, tier, evidence).
- **Exit:** shims gone; shared headers warning-clean; dashboard + records
  updated; `client`/`host`/`both` build green and behaviorally equivalent.

---

## Invariants (apply to EVERY phase)

1. **Tri-target.** Build `client`, `host`, and `both` after every increment.
2. **Wire/`.sav` frozen.** No byte on the wire or in `.sav` changes; do not bump
   `U6O_VERSION`; the `MV_TX_*` / `SOBJ_TX_*` / `sobj_*` / `mv_*` family belongs
   to `DOB-P*` — coordinate, don't co-opt.
3. **RNG order frozen.** Any touch near `random/` requires call-count + order
   proof.
4. **Platform seam is LH's.** Do not modernize `platform/plat_*.h` here.
5. **Prove the tier.** No T2 ships without a byte-stream/round-trip diff; no T3
   without pixel/byte proof + benchmark.

## Session handoff

- **2026-06-16 (drafted).** Plan committed; no code yet. Complete `MOD-P0`
  (shared safety net) first, then start at **`MCOM-P1.1`**. Before P1.3/P1.4/P5,
  re-read the `DOB` plan's handoff so the wire-buffer boundary stays clean.

