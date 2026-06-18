<#
    loop_split_oracle.ps1  —  LCS regression oracle for the loop_client.cpp split.

    The "pure relocation" invariant (see docs/plans/plan-loopClientSplit.md,
    phase LCS-P0) is: moving code between #include'd chunk files must NOT change
    the preprocessed TOKEN STREAM of the client TU.

    Why not a binary hash? The MSVC debug EXE embeds a fresh PDB GUID + PE
    timestamp/checksum on every link, so it is never byte-stable (verified).

    Why not a raw hash of `cl /EP` output? `/EP` preserves source blank-line
    layout, so adding an #include file boundary shifts a few blank lines and
    changes the raw bytes WITHOUT changing a single token. We therefore compare
    the WHITESPACE-NORMALIZED preprocessor output (all runs of whitespace
    collapsed to one space, trimmed). That is exactly the token stream the
    compiler sees.

    Usage
    -----
      # Capture / refresh the baseline (only when you intentionally accept the
      # current state as the reference, e.g. right after LCS-P0):
      pwsh tools/loop_split_oracle.ps1 -Baseline

      # Check the current tree against the saved baseline (every phase):
      pwsh tools/loop_split_oracle.ps1

    Requires a VS x86 dev environment on PATH (run from a
    "x64_x86 Native Tools" prompt, or after vcvarsamd64_x86.bat). Exits 0 on
    match, 1 on mismatch.
#>
[CmdletBinding()]
param(
    [switch]$Baseline,
    [string]$HashFile = "tools/loop_split_oracle_baseline.sha256"
)

$ErrorActionPreference = "Stop"
Set-Location (Split-Path $PSScriptRoot -Parent)

$tu  = "src\common\u6o7.cpp"
$tmp = [IO.Path]::GetTempFileName()

# Preprocess the client TU (token stream only; /EP strips comments and #line).
# cl writes its banner ("u6o7.cpp") to stderr; under -EA Stop PowerShell would
# treat that as a terminating error, so relax EA across the native call.
$savedEA = $ErrorActionPreference
$ErrorActionPreference = "Continue"
& cmd /c "cl /nologo /EP /TP -DCLIENT -DWIN32 -D_DEBUG -D_WINDOWS -Isrc\common\include -Isrc\common -Isrc\client -Isrc\server $tu > `"$tmp`" 2>nul"
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

if ($Baseline) {
    Set-Content -LiteralPath $HashFile -Value $hash -NoNewline
    Write-Output "Baseline token-stream SHA256 written to $HashFile :"
    Write-Output "  $hash"
    exit 0
}

if (-not (Test-Path $HashFile)) {
    Write-Error "No baseline at $HashFile. Run with -Baseline first."
    exit 2
}
$expected = (Get-Content -LiteralPath $HashFile -Raw).Trim()
if ($hash -eq $expected) {
    Write-Output "OK: token stream matches baseline."
    Write-Output "  $hash"
    exit 0
} else {
    Write-Output "MISMATCH: token stream changed!"
    Write-Output "  expected: $expected"
    Write-Output "  actual  : $hash"
    Write-Output "The relocation was NOT pure - you edited code while moving it. Revert and redo."
    exit 1
}

