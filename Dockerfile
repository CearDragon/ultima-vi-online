# escape=`
FROM mcr.microsoft.com/dotnet/framework/runtime:4.8-windowsservercore-ltsc2022

# Set working directory
WORKDIR "C:/u6o7-host"

# Create necessary directories
RUN powershell -Command "New-Item -ItemType Directory -Force -Path assets\map_patches | Out-Null"

# Copy the host executable. It must sit at the SAME level as the game
# files/folders below so its relative path opens (".\\ultima6\\...",
# ".\\host\\...", "motd.txt", ...) resolve.
# Note: This assumes you have built the host in Release mode
COPY ["bin/host/release/Ultima VI Online Host.exe", "."]

# Copy the full host runtime tree (ultima6/, host/, dr/, save/, *.txt) directly
# beside the executable in C:/u6o7-host. Windows containers use a
# case-insensitive filesystem, so the host's lower-case path opens resolve
# against the bundled upper-case names as-is.
COPY ["assets/game_files/host/", "."]

# Map patches are compiled into the binary; ship them too for layout parity.
COPY ["assets/map_patches", "assets/map_patches"]

# Create a default dns.txt to specify the port
RUN powershell -Command "Set-Content -Path dns.txt -Value 'localhost:22'"

# Expose the game port
EXPOSE 22

# Run the host
# We use "host" parameter to ensure it runs in host mode if not compiled as dedicated
CMD ["Ultima VI Online Host.exe"]
