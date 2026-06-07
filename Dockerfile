# Escape character for Dockerfile (Windows uses backtick by default in some builders, but backslash is common)
# escape=`

FROM mcr.microsoft.com/windows/servercore:ltsc2022

# Set working directory
WORKDIR C:\u6o7-host

# Create necessary directories
RUN mkdir ultima6 assets\map_patches save dr

# Copy the host executable
# Note: This assumes you have built the host in Release mode
COPY bin\host\release\Ultima` VI` Online` Host.exe .

# Copy assets
COPY assets\map_patches assets\map_patches

# Copy map data (ultima6 folder)
# This folder must contain chunks, map, etc.
# If these files are not in the repo, they must be provided at build time
COPY ultima6 ultima6

# Create a default dns.txt to specify the port
RUN echo localhost:2593 > dns.txt

# Expose the game port
EXPOSE 2593

# Run the host
# We use "host" parameter to ensure it runs in host mode if not compiled as dedicated
CMD ["Ultima VI Online Host.exe", "host"]
