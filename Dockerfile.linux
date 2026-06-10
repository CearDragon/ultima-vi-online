# syntax=docker/dockerfile:1
#
# LH-P7: Headless Linux host image for Ultima VI Online.
#
# Multi-stage:
#   1. build   — compile the 32-bit (-m32) host via the CMake `if(NOT WIN32)`
#                branch (see CMakeLists.txt / docs/plans/plan-linuxHost.md).
#   2. runtime — slim image with only the 32-bit C/C++ runtime + game data.
#
# All host runtime data is bundled in the repo under assets/game_files/host/
# (ultima6/ map data, host/ NPC data, dr/, save/, and the top-level *.txt) and is
# copied next to the binary below — nothing extra needs staging in the build
# context.
#
#   docker build -f Dockerfile.linux -t u6o-host:latest .

# ---------------------------------------------------------------------------
# Stage 1: build the headless host (i386)
# ---------------------------------------------------------------------------
FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
        g++-multilib \
        cmake \
        ninja-build \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

# Configure + build the Linux host. Mirrors `cmake --preset linux-host` but
# spelled out so the build context doesn't depend on the preset's host-OS
# condition. Output: bin/host/linux/u6o-host (32-bit ELF).
RUN cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build-linux

# ---------------------------------------------------------------------------
# Stage 2: minimal runtime
# ---------------------------------------------------------------------------
FROM debian:bookworm-slim AS runtime

# 32-bit runtime libraries for the -m32 binary.
RUN dpkg --add-architecture i386 \
    && apt-get update && apt-get install -y --no-install-recommends \
        libc6:i386 \
        libstdc++6:i386 \
        libgcc-s1:i386 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /u6o-host

# LH-P7.1: the host opens its data with paths relative to the working directory
# (".\\ultima6\\...", ".\\host\\...", ".\\save\\...", "motd.txt", ...), so the
# executable must live at the SAME level as those files/folders. Everything
# below lands directly in /u6o-host beside the binary.

# Host executable.
COPY --from=build /src/bin/host/linux/u6o-host ./u6o-host

# Bundled host game files. assets/game_files/host/ already contains the full
# runtime tree the host expects next to the binary: ultima6/ (chunks, map, ...),
# host/ (NPC + flag data), dr/, save/, and the top-level *.txt (ban/dns/motd/log).
COPY assets/game_files/host/ ./

# Map patches are compiled into the binary (house.cpp #includes them); ship them
# too so the on-disk layout matches the Windows image.
COPY assets/map_patches/ ./assets/map_patches/

# LH-P7.1: NO case-folding of the game data. Windows is case-insensitive, so the
# host mixes path case freely: lowercase hard-coded strings (".\\host\\..."),
# UPPER-case shipped data (CHUNKS, MAP, SAVEGAME/OBJBLKxx), and names it BUILDS
# at runtime in UPPER ("objblk" + (x+65) -> "objblkEB"). No single rename rule
# satisfies all three. Instead the POSIX file shim resolves case per path
# segment at open() time (see src/common/myfile.cpp u6o_resolve_ci), so the data
# is shipped verbatim in its original case.

# Listen address/port (dns.txt: "<host>:<port>"). Must match the k8s Service /
# containerPort. INADDR_ANY is used for bind, so the host part is informational.
RUN echo "localhost:22" > dns.txt \
    && mkdir -p save

EXPOSE 22

# The dedicated host assumes the "host" role; pass it explicitly for parity
# with the Windows image. SIGTERM/SIGINT trigger a graceful save-and-exit
# (see u6o_posix_term_handler in u6o7.cpp).
CMD ["./u6o-host", "host"]

