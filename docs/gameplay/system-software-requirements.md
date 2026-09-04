# System and Software Requirements

This document defines the supported system, software, runtime-data, and
network requirements for Ultima VI Online (U6O). It separates the historical
player-facing minimums from the requirements of the current source tree.

## Product scope

- U6O is a fan-made multiplayer reimagining of *Ultima VI: The False Prophet*.
- The client renders the game and handles input; the host is authoritative for
  the world, NPC AI, combat, spells, object updates, and player state.
- The client, dedicated host, and combined `both` executable are built from the
  same source tree. The client remains Windows/DirectX-only; the dedicated host
  can also run headlessly on Linux/i386.

## End-user client requirements

### Minimum hardware and peripherals

The original player manual lists these minimum requirements:

- 500 MHz CPU.
- Monitor capable of `1024×768` or better.
- Internet connection.
- Mouse.
- Keyboard.

Speakers or headphones and a microphone are recommended but not required. A
microphone is only relevant to optional voice features; audio output is needed
to hear music, sound effects, and voice clips.

### Supported client software

- Windows client runtime. The legacy manual says Windows 98 or newer and
  DirectX 7.0 or newer.
- For the current source/build environment, use Windows 10 or 11 with the
  bundled Win32/DirectX-compatible libraries. The project is not a 64-bit
  client: `/MACHINE:X86` is required because the renderer uses x86 assembly
  blitters and legacy DirectX calling conventions.
- No separate DirectX SDK installation is required for a repository build;
  the required headers and libraries are under `src/common/include` and
  `src/common/lib`.

### Required runtime files

The client executable must run beside a bundle with this structure (the
reference bundle is `test/client/`):

```text
client-directory/
├── dr/                 # UI bitmaps and panel assets
├── midi/               # music
├── voice/              # voice clips, when available
├── wav/                # sound effects
├── ultima6/            # files from the user's original Ultima 6 copy
├── settings.txt        # client settings
├── dns.txt             # host:port, normally localhost:22
├── userinfo.txt        # cached account/character information
├── talk.txt            # recent chat log
└── ban.txt, ignore.txt # local block lists
```

U6O does not redistribute the original game data. Players must own an
original copy of Ultima 6 and place the required files in `ultima6/`.
Graphics, sounds, music, and other original game assets are loaded at runtime;
they are not a substitute for the engine binaries.

### Display and input

- `1024×768` is the baseline display size recorded by the legacy manual.
- The client also supports a half-size `512×384` window and the modern
  resizable-window path; the `ps5` minimap/gem-peer surface remains pinned to
  a `1024×768` aspect ratio.
- A mouse is required for pointing, picking up and using items, targeting,
  and dragging movable windows. Right mouse button dragging is used for
  movable windows; normal actions use the left button.
- A keyboard is required for movement/actions and text communication. Core
  controls include `A` for attack, `T` for chat, `U` for use, `L` for look,
  `S` for sound controls, `M` for window size, `Esc` for cancel, and `Q` to
  quit.

## Dedicated host requirements

### Windows host

The Windows `host` executable requires the host runtime data beside it,
including `ultima6/`, `host/`, `save/`, `dns.txt`, and `motd.txt`. It listens
on the port specified by `dns.txt`; the default development configuration is
`localhost:22`.

The Windows host is also x86/Win32. The `host` target has no client renderer,
but it shares the byte-compatible world, save, and network data structures
with the client.

### Linux headless host

The dedicated host is supported on Linux/i386 for Docker, Kubernetes, or a
native headless deployment. The client cannot run on this path.

Required native build prerequisites on Debian/Ubuntu are:

- CMake `3.16` or newer.
- Ninja when using the bundled Ninja preset.
- `g++-multilib` and the 32-bit C/C++ runtime, pthread, and math libraries.
- An i386-capable environment; the host is built with `-m32`.

The Linux host must run beside the same style of game-data tree as the Windows
host. The platform shim resolves the mixed-case paths used by the original
data files. `SIGTERM` and `SIGINT` perform a save-and-exit so container
shutdown does not discard the world state.

### Container and orchestration requirements

- Docker can build and run the headless host using `Dockerfile.linux` and the
  provided compose configuration.
- The container exposes port `22` by default; if the host machine already uses
  that port, map another host port and set the client `dns.txt` accordingly.
- Persistent storage must be mounted for `/u6o-host/save`; the named
  `u6o-save` volume in the compose setup preserves saves across container
  replacement and image rebuilds.
- Kubernetes deployments require a persistent save volume and a service that
  exposes the configured host port.

## Build-time software requirements

These requirements apply to developers building the current source tree, not
to players using a prebuilt client.

### Windows build

- Windows 10 or 11.
- Visual Studio 2019 or newer, with **Desktop development with C++**.
- MSVC x86 (`v143`) components; the project is hard-coded to `Win32`/x86.
- CMake `3.16` or newer.
- PowerShell 5 or newer for the icon-generation step.
- An existing configured build tree such as `cmake-build-debug`.

The supported targets are:

| Target | Output | Purpose |
|---|---|---|
| `client` | `Ultima VI Online.exe` | Windows client connecting to a host |
| `host` | `Ultima VI Online Host.exe` | Dedicated Windows host |
| `both` | `Ultima VI Online Full.exe` | Combined host and client for local testing |

### Build invariants

- Do not build the Windows targets as x64 until the inline assembly and legacy
  calling conventions have been ported.
- The client and host must retain compatible 32-bit structure and wire layouts.
- The Linux host is also built as i386 so `.sav` data and host/client messages
  remain byte-compatible; no protocol version change is needed merely for a
  Linux host.
- Use the existing configured build tree rather than a fresh bare build
  directory, because the repository's bundled Windows headers and libraries
  are part of the expected configuration.

## Functional software requirements

The following behaviors are requirements of the implemented game systems:

- **Authoritative simulation:** hit rolls, damage, dodge, armor, spell
  resolution, object updates, NPC AI, and karma changes are calculated by the
  host. The client displays the resulting effects and state; it must not make
  independent combat or karma decisions.
- **Gameplay networking:** client and host must use the same protocol and
  byte-compatible `player` buffers. Host screen offsets use the legacy `32×24`
  frame when encoded for clients.
- **World and object processing:** the host processes timed objects and
  creatures. Implemented object behavior includes powder-keg fuse countdowns,
  five-by-five explosion effects, door opening where allowed, chest destruction
  and item extraction, chained kegs, creature damage, and player-party damage.
- **Status effects:** creature updates support poison, invisibility, charm,
  fear, curse, fire-field damage, sleep/paralysis fields, and webs, subject to
  the implemented immunity and invulnerability rules.
- **Spells and resources:** spell casting requires a spellbook, known spell,
  sufficient circle level, mana, and reagents. Successful and failed casts
  consume the required resources according to the host cast flow; some spells
  listed in `docs/gameplay/spells.md` remain intentionally unimplemented.
- **Persistence:** player and world saves must preserve the existing `.sav`
  layout. Karma is a host-owned floating-point value persisted in the save;
  it starts at `100` for a new character, has no passive regeneration, and
  controls resurrection eligibility at death.
- **Original asset ownership:** runtime loading of Ultima 6 data is mandatory;
  missing or invalid original data is an installation problem, not a reason to
  vendor those assets in this repository.

## Verification criteria

An installation or build satisfies this requirements document when:

1. The selected executable starts from its expected runtime-data directory.
2. The client can connect to the configured host and receive the world state.
3. The host can load the original game data and save its world/player state.
4. The combined `both` target can be used for local end-to-end verification.
5. A headless Linux host, when used, runs as i386 and remains connectable by an
   unchanged Windows client.

There is currently no automated test framework. End-to-end verification is
performed by running `both` and exercising the affected gameplay path.

## Authoritative references

- [`README.md`](../../README.md) — build, run, Windows, Linux host, and runtime
  bundle requirements.
- [`assets/game_files/client/readme.txt`](../../assets/game_files/client/readme.txt)
  — historical player minimums and controls.
- [`combat.md`](combat.md) — host-authoritative combat requirements.
- [`spells.md`](spells.md) — spell prerequisites and implemented status.
- [`karma.md`](karma.md) — karma storage, persistence, and death behavior.
- `src/server/loop/loop_host_part_c_objupdate.cpp` — timed object, explosion,
  creature, NPC, and field-effect processing.
- `CMakeLists.txt` — current Windows and Linux/i386 build definitions.