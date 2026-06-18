<#
    loop_split_oracle_host.ps1  —  LHS regression oracle for the loop_host.cpp split.

    Host sibling of tools/loop_split_oracle.ps1 (which targets the CLIENT TU).
    See docs/plans/plan-loopHostSplit.md, phase LHS-P0.1.

    The "pure relocation" invariant (plan invariant 1) is: moving code between
    #include'd chunk files must NOT change the preprocessed TOKEN STREAM of the
    HOST translation unit.

    Why not a binary hash? The MSVC debug EXE embeds a fresh PDB GUID + PE
    timestamp/checksum on every link, so it is never byte-stable (verified for
    the client split, see plan-loopClientSplit.md LCS-P0.1).

    Why not a raw hash of `cl /EP` output? `/EP` preserves source blank-line
    layout, so adding an #include file boundary shifts a few blank lines and
    changes the raw bytes WITHOUT changing a single token. We therefore compare
    the WHITESPACE-NORMALIZED preprocessor output (all runs of whitespace
    collapsed to one space, trimmed). That is exactly the token stream the
    compiler sees.

    Host defines: -DHOST -DCONSOLE -DWIN32 -D_DEBUG -D_WINDOWS and NO -DCLIENT.
    These are the `host` target's compile definitions from CMakeLists.txt
    (HOST;CONSOLE in the Debug config). The `#ifdef HOST` guard at u6o7.cpp:689
    means HOST must be defined for the host loop to appear in the preprocessor
    output at all.

    Optionally pass -Both to snapshot the `both` TU instead (-DHOST -DCLIENT,
    no -DCONSOLE): a secondary oracle that exercises the `#ifdef CLIENT`
    branches inside loop_host.cpp that the pure `host` config does not.

    Usage
    -----
      # Capture / refresh the baseline (only when you intentionally accept the
      # current state as the reference, e.g. right after LHS-P0):
      pwsh tools/loop_split_oracle_host.ps1 -Baseline

      # Check the current tree against the saved baseline (every phase):
      pwsh tools/loop_split_oracle_host.ps1

      # The `both`-config secondary oracle:
      pwsh tools/loop_split_oracle_host.ps1 -Both
      pwsh tools/loop_split_oracle_host.ps1 -Both -Baseline

    Requires a VS x86 dev environment on PATH (run from a "x64_x86 Native Tools"
    prompt, or after vcvarsamd64_x86.bat). Exits 0 on match, 1 on mismatch.
#>
[CmdletBinding()]
param(
    [switch]$Baseline,
    [switch]$Both,
    [string]$HashFile
)

$ErrorActionPreference = "Stop"
Set-Location (Split-Path $PSScriptRoot -Parent)

# Default the hash file based on which TU we snapshot.
if (-not $HashFile) {
    $HashFile = if ($Both) { "tools/loop_split_host_both_oracle_baseline.sha256" }
                else        { "tools/loop_split_host_oracle_baseline.sha256" }
}

# Host vs both compile definitions. HOST is mandatory (gates the host loop).
$defs = if ($Both) { "-DHOST -DCLIENT -DWIN32 -D_DEBUG -D_WINDOWS" }
        else        { "-DHOST -DCONSOLE -DWIN32 -D_DEBUG -D_WINDOWS" }

$tu  = "src\common\u6o7.cpp"
$tmp = [IO.Path]::GetTempFileName()

# Preprocess the host TU (token stream only; /EP strips comments and #line).
# cl writes its banner ("u6o7.cpp") to stderr; under -EA Stop PowerShell would
# treat that as a terminating error, so relax EA across the native call.
$savedEA = $ErrorActionPreference
$ErrorActionPreference = "Continue"
& cmd /c "cl /nologo /EP /TP $defs -Isrc\common\include -Isrc\common -Isrc\client -Isrc\server $tu > `"$tmp`" 2>nul"
$cl = $LASTEXITCODE
$ErrorActionPreference = $savedEA
if ($cl -ne 0) {
    Write-Error "cl preprocessing failed (exit $cl). Are you in a VS x86 dev shell?"
    Remove-Item $tmp -ErrorAction SilentlyContinue
    exit 2
}

$raw = [IO.File]::ReadAllText($tmp)
Remove-Item $tmp -ErrorAction SilentlyContinue
$normalized = ($raw -replace '\s+', ' ').Trim()

$sha = [System.Security.Cryptography.SHA256]::Create()
$bytes = [Text.Encoding]::UTF8.GetBytes($normalized)
$hash = -join ($sha.ComputeHash($bytes) | ForEach-Object { $_.ToString("x2") })

$label = if ($Both) { "both (HOST+CLIENT)" } else { "host (HOST+CONSOLE)" }

if ($Baseline) {
    Set-Content -LiteralPath $HashFile -Value $hash -NoNewline
    Write-Output "Baseline $label token-stream SHA256 written to $HashFile :"
    Write-Output "  $hash"
    exit 0
}

if (-not (Test-Path $HashFile)) {
    Write-Error "No baseline at $HashFile. Run with -Baseline first."
    exit 2
}
$expected = (Get-Content -LiteralPath $HashFile -Raw).Trim()
if ($hash -eq $expected) {
    Write-Output "OK: $label token stream matches baseline."
    Write-Output "  $hash"
    exit 0
} else {
    Write-Output "MISMATCH: $label token stream changed!"
    Write-Output "  expected: $expected"
    Write-Output "  actual  : $hash"
    Write-Output "The relocation was NOT pure - you edited code while moving it. Revert and redo."
    exit 1
}

