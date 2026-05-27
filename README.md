# Ultima VI Online (U6O)

A fan-made multiplayer (MMORPG) reimagining of Origin Systems' *Ultima VI:
The False Prophet*. The original tile graphics, sounds, and music are loaded
at runtime from a copy of the original game files — **you must own an
original copy of Ultima 6 to play U6O**. This repository contains only the
engine, server, and supporting assets.

> Not affiliated with Origin Systems or Electronic Arts.

---

## Table of contents

- [Project layout](#project-layout)
- [Build targets](#build-targets)
- [Building](#building)
- [Running](#running)
- [Where things live](#where-things-live)
- [Documentation and plans](#documentation-and-plans)
- [Tools](#tools)
- [Tests / runtime asset bundle](#tests--runtime-asset-bundle)
- [Map patches and assets](#map-patches-and-assets)
- [Conventions and notes](#conventions-and-notes)

---

## Project layout

```text
ultima-vi-online/
├── CMakeLists.txt              # Three EXE targets: u6o7, u6oh, u6oclient2
├── CMake/                      # Toolchain helpers (Default*.cmake, Utils.cmake)
├── assets/
│   ├── images/icon.png         # Source PNG for window/EXE icons
│   └── map_patches/            # Plain-text overlays applied on top of U6 map data
├── src/
│   ├── client/                 # Client-only code (renderer, UI, input, sound)
│   ├── server/                 # Host/server code (world simulation, AI, sockets)
│   └── common/                 # Shared headers, math, RNG, asm primitives, secret
├── docs/                       # Developer docs: UI reference, plans, hotspots
├── tools/                      # Build-time helpers + crash-dump triage
├── test/                       # Runtime bundle (assets, settings, midi, wav, etc.)
├── bin/                        # Final EXE output (client/, host/)
├── build/                      # PDB output (matches MSVC defaults)
├── cmake-build-debug*/         # IDE-managed CMake build trees (CLion / VS)
└── u6o.zip                     # Packaged distribution bundle
```

---

## Build targets

`CMakeLists.txt` defines three Win32 executables. All target **x86 (Win32)**
because the rendering inner loops still rely on 32-bit inline assembly
(`src/common/inline_asm/fast*.asm`) and the original DirectPlay / DirectDraw
calling conventions.

| Target        | Output EXE name                | Defines               | Subsystem | Purpose                                                                                                |
| ------------- | ------------------------------ | --------------------- | --------- | ------------------------------------------------------------------------------------------------------ |
| `u6o7`        | `Ultima VI Online Full.exe`    | `HOST` + `CLIENT`     | WINDOWS   | Single-binary build that can host **and** connect as a client. Useful for local testing.               |
| `u6oh`        | `Ultima VI Online Host.exe`    | `HOST` + `CONSOLE`    | WINDOWS   | Dedicated server / host only. No client renderer compiled in.                                          |
| `u6oclient2`  | `Ultima VI Online.exe`         | `CLIENT`              | WINDOWS   | Pure client. Connects to a remote `u6oh`. This is the EXE shipped in `u6o.zip` for end users.          |

Output locations:

```text
bin/client/debug   ← u6o7 + u6oclient2 debug builds
bin/client/release ← u6o7 + u6oclient2 release builds
bin/host/debug     ← u6oh debug build
bin/host/release   ← u6oh release build
build/             ← PDBs for all configs
```

### Required SDKs and libs

The CMake target links against the DirectX 7-era SDK that ships with the
project under `src/common/lib/`:

- `ddraw`, `dsound`, `dplay`, `dplayx`, `dxguid`
- `fdv_dmvr` (DirectMusic helper, vendored)
- `mylib` (vendored helper library)
- Standard Win32: `user32`, `kernel32`, `gdi32`, `winmm`, `ws2_32`, `wininet`,
  `vfw32`, `ole32`, `oleaut32`, `uuid`, `shell32`, `comdlg32`, `advapi32`,
  `gdiplus` (client only), `winspool`, `odbc32`, `odbccp32`

Headers/`.lib`s for the above are bundled under `src/common/include` and
`src/common/lib`, so no separate DirectX SDK install is required.

### Compiler / linker

- **Toolchain:** MSVC (Visual Studio 2019+ or MSBuild 14+). Tested with the
  v143 toolset.
- **Runtime library:** `/MTd` (Debug), `/MD` (Release).
- **Arch:** `/MACHINE:X86` is hard-coded — 64-bit builds are not supported
  until the inline-asm modules are ported.
- **Optimization:** Debug uses `/Od /RTC1 /ZI`; Release uses `/Ob2 /Oi /Ot
  /Zi` with `/GL-` (whole-program optimization explicitly disabled because of
  the asm modules) and `/OPT:REF /OPT:ICF`.

---

## Building

### Prerequisites

- Windows 10/11 build host (cross-compilation is not supported).
- Visual Studio 2019 or newer with the **Desktop development with C++**
  workload and the **MSVC v143 (x86)** components.
- CMake **≥ 3.16**.
- PowerShell 5+ (the icon build step calls `tools/build_icon.ps1`).

### Generate and build (Visual Studio solution)

From a Developer PowerShell:

```powershell
# From the repo root.
cmake -S . -B cmake-build-debug -A Win32
cmake --build cmake-build-debug --config Debug
# or:
cmake --build cmake-build-debug --config Release
```

This produces `u6o7.sln` in `cmake-build-debug/` which can also be opened
directly in Visual Studio. Build the `ALL_BUILD` project to compile all three
EXEs, or build individual targets (`u6o7`, `u6oh`, `u6oclient2`).

### Generate and build (Ninja / CLion)

CLion's bundled CMake works out of the box; it stores its tree in
`cmake-build-debug-visual-studio/` and uses Ninja with the MSVC toolchain.
The build directory is checked into `.gitignore` but is reused by the
editor.

```powershell
cmake -S . -B cmake-build-debug-visual-studio -G Ninja `
      -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl
cmake --build cmake-build-debug-visual-studio
```

### The icon pipeline

Building any target re-runs `tools/build_icon.ps1` to rasterize
`assets/images/icon.png` into `${CMAKE_BINARY_DIR}/generated/icons/u6o7.ico`
and `small.ico`. The `generated/u6o7.rc` resource script is then materialized
from `src/common/u6o7.rc.in` via `configure_file()` and linked into every
EXE. To change the icon, drop a new square PNG at `assets/images/icon.png`
and rebuild — no source changes needed. See [`tools/README.md`](tools/README.md).

---

## Running

### Client

The shipping client (`Ultima VI Online.exe`, from the `u6oclient2` target)
expects a runtime asset bundle alongside it. The reference bundle is in
`test/client/`:

```text
test/client/
├── dr/                  # UI bitmaps and panel assets (also packed as dr.7z)
├── midi/                # Music tracks
├── voice/, wav/         # Sound effects and voice clips
├── ultima6/             # Drop your original Ultima 6 files here
├── settings.txt         # Per-user settings (window size, sound, MINIMAP mode, …)
├── dns.txt              # Server host:port to connect to (default localhost:22)
├── userinfo.txt         # Cached account / character info
├── talk.txt             # Recent chat log
└── ban.txt, ignore.txt  # Local block lists
```

To run a freshly built client against a local host:

1. Copy or symlink the compiled `Ultima VI Online.exe` into `test/client/`.
2. Place your legitimate Ultima 6 game files into `test/client/ultima6/`.
3. Edit `test/client/dns.txt` if you want to connect to a remote server
   (`host:port` format; default port `22`).
4. Launch the EXE.

### Host

Run `Ultima VI Online Host.exe` (the `u6oh` target). It listens on the port
configured in its `dns.txt` and serves all connected clients from the same
world state. The host also requires the `ultima6/` source assets for the
initial world load.

### Combined host + client (testing)

`Ultima VI Online Full.exe` (the `u6o7` target) hosts the world *and*
connects to it locally — convenient for solo testing of server-side changes
without standing up a separate host process.

---

## Where things live

### Client (`src/client/`)

| Concern                                | Files                                                                                                  |
| -------------------------------------- | ------------------------------------------------------------------------------------------------------ |
| Main client frame loop                 | `loop_client.cpp`, `loop_client.inc` (legacy copy used by older targets)                               |
| Client setup / boot                    | `setup_client.inc`                                                                                     |
| Data definitions and globals           | `data_client.h`, `data_client.cpp`, `define_client.h`                                                  |
| Helper functions                       | `function_client.cpp`, `function_client.h`                                                             |
| DirectDraw wrapper and surfaces        | `myddraw.cpp/h` — `surf*`, `img()`, `cls()`, `pset()`, `sf32`/`g32` blitters                           |
| Sound / DirectMusic                    | `sound.cpp/h`, `dmusic.cpp/h`                                                                          |
| Viewport and resizable world rendering | `viewport.cpp/h`                                                                                       |
| UI layout system (RW-Pn)               | `ui_layout.cpp/h`, `ui_panels_apply.cpp`                                                               |
| Movable in-game windows (FRAME system) | `frame.cpp/h`                                                                                          |
| Sprite tables                          | `new_getspr.inc`, `old_getspr.inc`                                                                     |
| Splash / intro screens                 | `splash.cpp/h`                                                                                         |
| Win32 resource identifiers             | `Resource.h`, plus the generated `u6o7.rc` (icons, version info)                                       |
| Bitmap manifest                        | `client_bmp.inc`                                                                                       |

### Server / host (`src/server/`)

| Concern                          | Files                                                  |
| -------------------------------- | ------------------------------------------------------ |
| Main host loop                   | `loop_host.cpp`, `loop_host.inc`                       |
| Host bootstrap                   | `setup_host.inc`, `host_setup.h`, `host_setup.inc`     |
| Helper functions                 | `function_host.cpp/h`                                  |
| Data definitions and globals     | `data_host.cpp/h`, `define_host.h`                     |
| Misc host-side resources         | `host.inc`                                             |

### Shared (`src/common/`)

| Concern                                   | Files                                                              |
| ----------------------------------------- | ------------------------------------------------------------------ |
| Shared globals / object table             | `globals.inc`                                                      |
| Shared headers and helpers                | `data_both.{h,cpp}`, `define_both.h`, `function_both.{h,cpp}`      |
| Object-type IDs (e.g. `OBJ_GEM`, `OBJ_TMAP`) | `define_both.h`                                                 |
| Random number generation                  | `random/` (`rand.h`, `random.{h,cpp}`, `randi.cpp`, `standard.h`)  |
| Spell scripts and use-code                | `spell_code/`, `use_code/`                                         |
| Inline 32-bit asm blitters                | `inline_asm/fast.asm`, `fast2hi.asm`, `fast3..5.asm`, `fasthi.asm` |
| Text utilities                            | `txt.cpp`, `mytxt.h`                                               |
| File I/O                                  | `myfile.cpp/h`                                                     |
| House / building helpers                  | `house.cpp`                                                        |
| Game-day / world tick state               | `Xenkan.cpp`                                                       |
| Setup orchestration                       | `setup_both.inc`, `setup_both_basic.h/inc`                         |
| Win32 resource template                   | `u6o7.rc.in` → `${CMAKE_BINARY_DIR}/generated/u6o7.rc`             |
| Vendored DX7 / mylib headers and libs     | `include/`, `lib/`                                                 |
| Obfuscated server protocol pieces         | `secret_a.inc`, `secret_b.inc`                                     |

### Gameplay landmarks (quick index)

| What                              | Where to look first                                                              |
| --------------------------------- | -------------------------------------------------------------------------------- |
| Object type IDs (e.g. gems, gold) | `src/common/define_both.h` (`OBJ_*` macros)                                      |
| Gem "peer" / birds-eye minimap    | `src/client/loop_client.cpp` (`if (peer)` block, and `minimaptype` block)        |
| Resizable window backbuffers      | `src/client/function_client.cpp` → `recreateBackbuffers()`                        |
| World view / camera               | `src/client/viewport.cpp`, `viewTilesX()`, `viewTilesY()`                        |
| Inventory                         | `docs/ExternInventory.md` plus the FRAME widgets in `frame.cpp`                  |
| Spells                            | `src/common/spell_code/`                                                         |
| "Use" interactions (`U` key)      | `src/common/use_code/`                                                           |
| Random chest contents             | `src/server/function_host.cpp` (`chest[chestn++]= …` blocks)                     |
| World tick / AI                   | `src/server/loop_host.cpp`                                                       |
| Auto-pickup behavior              | `src/server/function_host.cpp` (`AUTOPICKUPadd_selfmanagement:`)                 |

---

## Documentation and plans

All developer-facing docs live under `docs/`.

### Reference docs

- [`docs/ui/README.md`](docs/ui/README.md) — comprehensive reference for the
  client UI: every on-screen element, the C++ globals that back it, the two
  parallel UI systems (`FRAME*` legacy panels and the newer `uipanel*`
  system), drag/persist/validate mechanics.
- [`docs/resizable-window-hotspots.md`](docs/resizable-window-hotspots.md) —
  catalog of hard-coded `1024×768` literals throughout the codebase that
  interact with window resizing. Cross-referenced from the UI doc as
  "hotspots-§X".
- [`docs/ExternInventory.md`](docs/ExternInventory.md) — design notes for the
  external (always-on) inventory window.

### Multi-phase refactor plans (`docs/plans/`)

- [`plan-resizableWindow.md`](docs/plans/plan-resizableWindow.md) — RW-P1..Pn
  phased plan to make the client window freely resizable. Phases include the
  dynamic backbuffer recreation (RW-P2), viewport expansion (RW-P3/P4), and
  UI re-layout work.
- [`plan-dynamicObjectBuffer.md`](docs/plans/plan-dynamicObjectBuffer.md) —
  replacing the fixed-size object pools with growable arenas.
- [`plan-serverRefactor.md`](docs/plans/plan-serverRefactor.md) — incremental
  decomposition of the monolithic host loop.

When adding new long-form work, add a `plan-<feature>.md` to
`docs/plans/` and link to it from the relevant code path with an `RW-P*` /
`SR-P*` / `DOB-P*` tag in comments so future readers can find context.

---

## Tools

`tools/` holds build-time helpers and crash triage utilities.

| Tool                          | What it does                                                                                                |
| ----------------------------- | ----------------------------------------------------------------------------------------------------------- |
| `build_icon.ps1`              | Invoked by CMake to rasterize `assets/images/icon.png` into `u6o7.ico` + `small.ico`. See [`tools/README.md`](tools/README.md). |
| `analyze_dump.ps1`            | Symbolicates and summarizes minidumps emitted by a crashing client/host.                                    |
| `symbolize.cpp` / `symbolize.exe` | Address-to-symbol resolver used by `analyze_dump.ps1`. Rebuild with `cl symbolize.cpp dbghelp.lib` if needed. |
| `crash/crash-reports/`        | Drop landing area for `.dmp` files collected from users for triage.                                         |
| `bug-images/`                 | Screenshots attached to bug reports (e.g. `bug.png`).                                                       |

---

## Tests / runtime asset bundle

`test/` currently houses the **runtime asset bundle** rather than unit
tests. `test/client/` mirrors the directory layout the client expects to
find next to itself at launch (sound, music, UI bitmaps, the
`ultima6/` original game files, and the per-install `settings.txt` /
`dns.txt` / `userinfo.txt`). It is the most convenient place to drop a
freshly built `Ultima VI Online.exe` and run it end-to-end.

The legacy `test/client/readme.txt` is the player-facing manual (system
requirements, key bindings, how to talk to NPCs, etc.) and is a useful
reference when wiring up new gameplay features.

There is no automated test framework today; verification is done by
launching `u6o7` (combined host + client) and exercising the affected code
path manually.

---

## Map patches and assets

`assets/map_patches/` contains plain-text overlays applied on top of the
original Ultima 6 map at world-load time. Each file represents a discrete
edit (a new house, a fixed forest tile, a shop interior, etc.) and is
loaded by the host during setup. Files in `assets/map_patches/back/` are
archived prior revisions kept for reference.

`assets/images/` holds source art used by the build pipeline (currently
just the EXE icon).

---

## Conventions and notes

- **32-bit only.** The world renderer and inner blit loops still live in
  hand-rolled x86 inline assembly. Until those are ported, `MACHINE:X86`
  is mandatory.
- **No precompiled headers** (`/Y-` is set globally) so headers are
  individually self-contained.
- **`HOST` / `CLIENT` defines.** Code paths that should compile into only
  the host or only the client are gated with `#ifdef HOST` / `#ifdef CLIENT`.
  The combined `u6o7` target defines both.
- **`.inc` files** are not headers — they are textually included once into
  a single translation unit (typically the loop or setup files). Treat
  them as source, not as reusable headers, and keep them out of the
  include path.
- **Globals.** The shared world state lives in `src/common/globals.inc`.
  Adding a new global usually means adding it there (with an `extern` in
  the matching `data_*.h`) so all three EXEs link consistently.
- **Resizable-window changes** should consult
  `docs/resizable-window-hotspots.md` first — many gameplay surfaces
  (`ps`, `ps3`, `ps5`, `psnew1`, `psnew1b`, `minimaptilesurf*`) have subtle
  size dependencies that aren't obvious from the call site.
- **Renderer scratch surfaces:** `ps5` is intentionally pinned at
  `1024×768` (the minimap/gem-peer view depends on that exact aspect
  ratio); only `ps`, `ps3`, and the `psnew1*` panel surfaces follow the
  window size. See the comments in
  `src/client/function_client.cpp::recreateBackbuffers()` and
  `src/client/setup_client.inc` next to the `ps5` allocation.

