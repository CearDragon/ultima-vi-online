# escape=`
FROM mcr.microsoft.com/dotnet/framework/runtime:4.8-windowsservercore-ltsc2022

# ---------------------------------------------------------------------------
# Layout contract
# ---------------------------------------------------------------------------
# The dedicated host resolves EVERY data path relative to its working
# directory, e.g. ".\dr\...", ".\host\npc.bin", ".\ultima6\chunks",
# ".\save\...", plus root config files (ban.txt, dns.txt, motd.txt,
# admins.txt). So the executable MUST live in the same folder as those data
# folders/files, and that folder MUST be the working directory — otherwise
# setup crashes on the first missing file and the container exits.
#
# Everything is staged under C:\host: the exe sits next to all the files and
# folders that ship in assets/game_files/host.
WORKDIR C:\host

# 1) The host executable. Build it first in Release (CMake "host" target ->
#    bin/host/release/). The MSVC runtime is statically linked (/MT), so no
#    Visual C++ redistributable is required in the image.
COPY ["bin/host/release/Ultima VI Online Host.exe", "Ultima VI Online Host.exe"]

# 2) All runtime data: copies the *contents* of assets/game_files/host
#    (dr\, host\, ultima6\, save\, ban.txt, dns.txt, motd.txt, ...) straight
#    into C:\host, next to the executable.
#
#    NOTE: map patches are #include-compiled into the binary at build time
#    (see src/common/house.cpp), so assets/map_patches is intentionally NOT
#    copied — it is not read at runtime.
COPY ["assets/game_files/host/", "./"]

# 3) Entry point wrapper that streams the host's log.txt to the container's
#    stdout and propagates the exit code, so startup failures are visible in
#    `docker logs` instead of a silent exit.
COPY ["docker-entrypoint.ps1", "docker-entrypoint.ps1"]

# Listen port. dns.txt is read at startup; the value after the colon in
# "host:port" is the TCP port the server binds (on INADDR_ANY). Override the
# shipped dns.txt so the container's port is explicit and matches EXPOSE.
RUN powershell -NoProfile -Command "Set-Content -Path 'C:\host\dns.txt' -Value 'localhost:22'"

# Expose the game port.
EXPOSE 22

# Launch via the wrapper so the container's lifetime tracks the host process
# and its log/exit code are surfaced (no more silent exits).
ENTRYPOINT ["powershell", "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", "C:\\host\\docker-entrypoint.ps1"]
