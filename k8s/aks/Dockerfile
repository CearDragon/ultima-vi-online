# syntax=docker/dockerfile:1
#
# AKS host image: build only the Linux host executable and run it from /host.
# Runtime game data is expected from a mounted volume at /host.

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

# Build the Linux host executable.
RUN cmake -S . -B build-linux -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build-linux --target host

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

# Keep the executable outside the mounted path so it remains available when
# /host is over-mounted by the Kubernetes volume.
COPY --from=build /src/bin/host/linux/u6o-host /usr/local/bin/u6o-host
RUN chmod +x /usr/local/bin/u6o-host

# The host opens data relative to the working directory.
WORKDIR /host

EXPOSE 22

CMD ["/usr/local/bin/u6o-host", "host"]

