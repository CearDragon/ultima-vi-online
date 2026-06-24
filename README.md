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
- [Linux host (headless)](#linux-host-headless)
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
├── CMakeLists.txt              # Three EXE targets: both, host, client
├── CMakePresets.json           # `linux-host` preset (headless i386 host build)
├── CMake/                      # Toolchain helpers (Default*.cmake, Utils.cmake)
├── Dockerfile.linux            # Headless Linux host image (multi-stage, i386)
├── docker-compose.yml          # One-command host: port + save volume + restart
├── k8s/                        # Kubernetes manifests for the Linux host
├── assets/
│   ├── images/icon.png         # Source PNG for window/EXE icons
│   ├── game_files/host/        # Bundled host runtime data (ultima6/, host/, …)
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

> The dedicated **host** additionally builds headless on **Linux (i386)** for
> Docker/Kubernetes — see [Linux host (headless)](#linux-host-headless).

| Target        | Output EXE name                | Defines               | Subsystem | Purpose                                                                                                |
| ------------- | ------------------------------ | --------------------- | --------- | ------------------------------------------------------------------------------------------------------ |
| `both`        | `Ultima VI Online Full.exe`    | `HOST` + `CLIENT`     | WINDOWS   | Single-binary build that can host **and** connect as a client. Useful for local testing.               |
| `host`        | `Ultima VI Online Host.exe`    | `HOST` + `CONSOLE`    | WINDOWS   | Dedicated server / host only. No client renderer compiled in.                                          |
| `client`      | `Ultima VI Online.exe`         | `CLIENT`              | WINDOWS   | Pure client. Connects to a remote `host`. This is the EXE shipped in `u6o.zip` for end users.          |

Output locations:

```text
bin/client/debug   ← both + client debug builds
bin/client/release ← both + client release builds
bin/host/debug     ← host debug build
bin/host/release   ← host release build
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

- Windows 10/11 build host (this section covers the Win32 client/host/both
  builds; for the headless Linux host see
  [Linux host (headless)](#linux-host-headless)).
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
EXEs, or build individual targets (`both`, `host`, `client`).

#### Fast incremental builds (recommended)

Once `cmake-build-debug/` has been configured **once**, build individual
targets against that existing tree — this is the recommended day-to-day form
and the one automated agents should use:

```powershell
# <target> is one of: client, host, both
cmake.exe --build cmake-build-debug --target client -j 18
cmake.exe --build cmake-build-debug --target host   -j 18
cmake.exe --build cmake-build-debug --target both   -j 18
```

`-j 18` parallelizes the compile across cores; tune the number to your CPU.

> **Don't reconfigure a throwaway build directory just to compile.** Building
> through the already-configured `cmake-build-debug/` tree reuses its bundled
> DirectX 7 / Win32 SDK include paths (`src/common/include`) and the MSVC x86
> toolchain. Spinning up a fresh build dir in a bare shell frequently fails
> with **missing Windows header files** (`windows.h`, `ddraw.h`, …). If you
> must reconfigure, do it from a **Developer PowerShell** so the Windows SDK
> environment is present.

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

### Shared run configurations and CMake profiles

The repo ships **shared, version-controlled** JetBrains/CLion run
configurations and CMake profiles so everyone builds and launches the targets
the same way. **Prefer these over hand-rolling your own** — they encode the
correct target, working directory, and build-before-run wiring:

| File | What it provides |
| --------------------------------- | ----------------------------------------------------------------------------- |
| `.run/client.run.xml`             | Runs the `client` target with its working dir set to `test/client/`.          |
| `.run/host.run.xml`               | Runs the `host` target.                                                       |
| `.run/both.run.xml`               | Runs the combined `both` (host + client) target.                              |
| `.run/generate_icons.run.xml`     | Runs the icon-generation target (`tools/build_icon.ps1`).                     |
| `.idea/cmake.xml`                 | Shared CMake profiles: **Debug** and **Release** (both enabled).              |

In CLion these appear automatically in the **Run/Debug configuration**
dropdown and the **CMake profile** selector — pick one and build/run. They map
directly onto the `client` / `host` / `both` CMake targets described above, so
they stay in sync with the command-line `--target` builds.

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

The shipping client (`Ultima VI Online.exe`, from the `client` target)
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

Run `Ultima VI Online Host.exe` (the `host` target). It listens on the port
configured in its `dns.txt` and serves all connected clients from the same
world state. The host also requires the `ultima6/` source assets for the
initial world load.

### Combined host + client (testing)

`Ultima VI Online Full.exe` (the `both` target) hosts the world *and*
connects to it locally — convenient for solo testing of server-side changes
without standing up a separate host process.

---

## Linux host (headless)

The dedicated **host** also builds and runs on Linux as a headless, no-GUI
process so the server can run in a Docker container (e.g. on Kubernetes).
Only the host is portable — the client stays Windows/DirectX-only.

Key properties (see [`docs/plans/plan-linuxHost.md`](docs/plans/done/plan-linuxHost.md)
for the full design):

- **32-bit (`-m32`).** Built as i386 so `struct`/`txt` layout and the wire +
  `.sav` formats stay byte-compatible with Windows clients and hosts. The
  protocol version is **not** bumped — a Windows client connects to a Linux
  host unchanged.
- **Thin platform shim.** All Win32 calls are wrapped behind
  `src/common/platform/` (`#ifdef _WIN32` pass-through on Windows, POSIX
  sockets/pthreads/`clock_gettime`/stdio elsewhere). The MSVC build is
  untouched.
- **Case-insensitive file access.** The host mixes path case freely
  (lowercase hard-coded strings, UPPER-case game data, names it builds in
  UPPER at runtime like `objblkEB`). The POSIX file shim resolves case per
  path segment at `open()` (`src/common/myfile.cpp` `u6o_resolve_ci`), so the
  game files are shipped verbatim — no renaming/case-folding.
- **Graceful shutdown.** `SIGTERM`/`SIGINT` trigger a save-and-exit
  (`u6o_posix_term_handler`), so `docker stop` / `kubectl delete` persist the
  world.

### Native Linux build

Prerequisites (Debian/Ubuntu):

```bash
sudo apt-get install g++-multilib cmake ninja-build   # 32-bit libstdc++/libc/pthread
```

Build with the bundled preset (or the spelled-out commands):

```bash
cmake --preset linux-host && cmake --build --preset linux-host
# equivalently:
cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-linux
```

Output: `bin/host/linux/u6o-host` (a 32-bit ELF). The CMake `if (NOT WIN32)`
branch builds **only** the `host` target and skips the Win32 resource/MASM/
DirectX paths.

To run it natively, the binary must sit **next to** the host game data
(`ultima6/`, `host/`, `save/`, `dns.txt`, `motd.txt`, …). The repo bundles a
ready-to-run tree under `assets/game_files/host/`:

```bash
cp bin/host/linux/u6o-host assets/game_files/host/
cd assets/game_files/host
echo "localhost:22" > dns.txt        # "<host>:<port>"; bind is INADDR_ANY
./u6o-host host
```

You should see `This is a dedicated u6o host running on port 22.`

### Docker container

The image is defined by [`Dockerfile.linux`](Dockerfile.linux) (multi-stage:
compile the 32-bit host, then a slim runtime with the i386 C/C++ runtime + the
bundled game data copied next to the binary).

The simplest path is the provided [`docker-compose.yml`](docker-compose.yml),
which captures the published port, a persistent save volume, auto-restart, and
graceful shutdown:

```bash
docker compose up -d --build     # build the image and start the host
docker compose logs -f           # follow host logs
docker compose stop              # graceful save-and-exit (SIGTERM, 60s grace)
docker compose down              # stop + remove container (the save volume persists)
```

The host listens on container port **22** (from `dns.txt`). The compose file
maps it to host port `22` so a client on the same machine connects with the
default `dns.txt` (`localhost:22`) — no client changes needed. If port 22 is
taken (e.g. a local SSH server), change only the left side of the mapping in
`docker-compose.yml` (e.g. `"2222:22"`) and point the client's `dns.txt` at
`<host>:2222`.

Saves are kept in the named volume `u6o-save` (mounted at `/u6o-host/save`),
so they survive `docker compose down` and image rebuilds.

Prefer plain Docker? The equivalent without compose:

```bash
docker build -f Dockerfile.linux -t u6o-host:latest .
docker volume create u6o-save
docker run -d --name u6o-host --restart unless-stopped \
    -p 22:22 -v u6o-save:/u6o-host/save u6o-host:latest
docker stop -t 60 u6o-host        # graceful save-and-exit
```

For Kubernetes, see [`k8s/deployment.yaml`](k8s/deployment.yaml) and
[`k8s/README-k8s.md`](k8s/README-k8s.md).

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
| Main host loop                   | `loop_host.cpp` (compiled via `#include` into `u6o7.cpp`) |
| Host bootstrap                   | `setup_host.inc`, `host_setup.h`                       |
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

Plans are filed in **lifecycle status folders** so the current state of any
effort is visible at a glance:

| Folder | Meaning | When a plan moves here |
| ----------------------------- | -------------------------- | --------------------------------------------- |
| `docs/plans/todo/`            | Not yet started            | A new plan is **always created here**.        |
| `docs/plans/in-progress/`     | Actively being worked      | Move it here when its first phase is picked up. |
| `docs/plans/done/`            | Finished (all phases ✅)   | Move it here when the work is complete.       |

A plan lives in exactly one folder — moving the file *is* the status change.
Update any links (this README, the modernization master index, and code-comment
phase tags) when you move one.

Representative plans:

- [`plan-resizableWindow.md`](docs/plans/done/plan-resizableWindow.md) — RW-P1..Pn
  phased plan to make the client window freely resizable. Phases include the
  dynamic backbuffer recreation (RW-P2), viewport expansion (RW-P3/P4), and
  UI re-layout work. *(done)*
- [`plan-dynamicObjectBuffer.md`](docs/plans/todo/plan-dynamicObjectBuffer.md) —
  replacing the fixed-size object pools with growable arenas. *(todo)*
- [`plan-memoryManagement.md`](docs/plans/in-progress/plan-memoryManagement.md) —
  memory-leak investigation and management work. *(in-progress)*
- [`plan-serverRefactor.md`](docs/plans/plan-serverRefactor.md) — incremental
  decomposition of the monolithic host loop.

When adding new long-form work, create `plan-<feature>.md` in
`docs/plans/todo/` and link to it from the relevant code path with an `RW-P*` /
`SR-P*` / `DOB-P*` tag in comments so future readers can find context. Promote
it to `in-progress/` when work begins and `done/` when it lands.

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

### Crash analysis with WinDbg / CDB (recommended)

The richest crash triage (and what the **`crash-report` agent** prefers) uses
the **Debugging Tools for Windows** — `cdb.exe`, `windbg.exe`, and friends.
Install the WinDbg kit from PowerShell and put `cdb.exe` on your `PATH`:

```powershell
# Install the modern WinDbg (ships the debugging engine + cdb.exe).
winget install --id Microsoft.WinDbg --source winget

# Or install the classic "Debugging Tools for Windows" via the Windows SDK,
# which lays cdb.exe down under the Windows Kits Debuggers folder:
winget install --id Microsoft.WindowsSDK --source winget
```

The classic console debugger `cdb.exe` typically lands at:

```text
C:\Program Files (x86)\Windows Kits\10\Debuggers\x86\cdb.exe   # 32-bit (this project)
C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\cdb.exe   # 64-bit
```

Add the appropriate `Debuggers\x86` (or `x64`) directory to your **system
`PATH`** so `cdb` resolves from any shell — for the current user, persistently:

```powershell
$dbg = 'C:\Program Files (x86)\Windows Kits\10\Debuggers\x86'
[Environment]::SetEnvironmentVariable(
    'Path',
    ([Environment]::GetEnvironmentVariable('Path', 'User') + ';' + $dbg),
    'User')
# Open a new shell, then verify:
cdb -version
```

With `cdb.exe` on `PATH`, the `crash-report` agent symbolicates against
`build/Ultima VI Online.pdb` and walks the stack automatically. The
dependency-free `analyze_dump.ps1` + `symbolize.exe` path remains available as
a fallback when the WinDbg kit isn't installed.

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
launching `both` (combined host + client) and exercising the affected code
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

