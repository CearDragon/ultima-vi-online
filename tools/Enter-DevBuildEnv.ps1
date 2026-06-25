<#
.SYNOPSIS
    Initializes the MSVC (x86) developer environment in the current PowerShell
    session so the Ninja-based CMake build can find the Windows SDK / Win32
    headers (windows.h, winsock2.h, stdlib.h, ddraw.h, ...).

.DESCRIPTION
    The repo builds the x86 target with the MSVC `Hostx64/x86` toolchain via
    CLion's bundled Ninja generator. With Ninja, `cl.exe` resolves system
    headers and import libraries from the INCLUDE / LIB environment variables.
    The IDE (and a venv-activated dev shell) populate those by running the
    Visual Studio developer environment; a bare agent / CI shell does NOT, which
    is why such shells fail with "cannot open source file 'windows.h'".

    This script locates Visual Studio with `vswhere`, imports the
    `amd64_x86` developer environment from `vcvarsall.bat` into the current
    session, and (optionally) runs the build. Dot-source it so the environment
    persists in your session:

        . .\tools\Enter-DevBuildEnv.ps1

    Or initialize and build in one shot:

        .\tools\Enter-DevBuildEnv.ps1 -Build both

.PARAMETER Build
    Optional CMake target to build after the environment is initialized
    (client | host | both). Builds against the pre-configured
    cmake-build-debug/ tree with -j 18, matching the repo's standard command.

.PARAMETER Arch
    vcvarsall architecture argument. Defaults to amd64_x86 (x64 host building
    the x86 target), which matches the configured Hostx64/x86 toolchain.

.EXAMPLE
    . .\tools\Enter-DevBuildEnv.ps1
    cmake.exe --build cmake-build-debug --target both -j 18

.EXAMPLE
    .\tools\Enter-DevBuildEnv.ps1 -Build both
#>
[CmdletBinding()]
param(
    [ValidateSet('client', 'host', 'both')]
    [string]$Build,

    [string]$Arch = 'amd64_x86'
)

$ErrorActionPreference = 'Stop'

function Find-VsInstallPath {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path $vswhere) {
        $path = & $vswhere -latest -products * `
            -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
            -property installationPath
        if ($path) { return $path.Trim() }
    }
    # Fallback: probe the well-known 2022 edition roots.
    foreach ($edition in 'Enterprise', 'Professional', 'Community', 'BuildTools') {
        $candidate = "C:\Program Files\Microsoft Visual Studio\2022\$edition"
        if (Test-Path (Join-Path $candidate 'VC\Auxiliary\Build\vcvarsall.bat')) {
            return $candidate
        }
    }
    throw "Could not locate a Visual Studio install with the VC x86/x64 toolset. Install the 'Desktop development with C++' workload."
}

if ($env:INCLUDE -and $env:LIB -and (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    Write-Host "MSVC environment already initialized (INCLUDE/LIB/cl.exe present)." -ForegroundColor Green
}
else {
    $vsPath = Find-VsInstallPath
    $vcvars = Join-Path $vsPath 'VC\Auxiliary\Build\vcvarsall.bat'
    if (-not (Test-Path $vcvars)) {
        throw "vcvarsall.bat not found at: $vcvars"
    }

    Write-Host "Initializing MSVC '$Arch' environment from:`n  $vcvars" -ForegroundColor Cyan

    # vcvarsall.bat is a batch file; run it in cmd, then re-import the resulting
    # environment back into this PowerShell session.
    cmd /c "`"$vcvars`" $Arch > nul && set" | ForEach-Object {
        if ($_ -match '^(.*?)=(.*)$') {
            Set-Item -Path "Env:$($matches[1])" -Value $matches[2]
        }
    }

    if (-not $env:INCLUDE -or -not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
        throw "Failed to initialize the MSVC environment (INCLUDE/cl.exe still missing)."
    }
    Write-Host ("Done. INCLUDE has {0} entries, LIB has {1} entries; cl.exe is on PATH." -f `
        (($env:INCLUDE -split ';').Count), (($env:LIB -split ';').Count)) -ForegroundColor Green
}

if ($Build) {
    Write-Host "Building target '$Build' ..." -ForegroundColor Cyan
    & cmake.exe --build cmake-build-debug --target $Build -j 18
    exit $LASTEXITCODE
}

