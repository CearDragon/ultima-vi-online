<#
    loop_split_scan.ps1  —  LCS-P0.2 analysis tool for the loop_client.cpp split.

    Purpose
    -------
    Prints the data an agent needs *before* making a cut while executing
    docs/plans/plan-loopClientSplit.md:

      (a) current line count of the target file,
      (b) every brace-depth-0 boundary line (legal depth-0 cut points),
      (c) every `goto` target that is NOT defined as a label in-file
          (i.e. cross-file / external jump targets — these constrain ordering).

    The brace-depth tracker is intentionally simple: it ignores braces that
    appear inside // line comments, /* block comments */, "string" literals and
    'c' char literals. It does NOT try to understand preprocessor conditionals
    (this file has essentially none at top level). Re-verify by eye at any
    surprising boundary.

    Usage
    -----
      pwsh tools/loop_split_scan.ps1
      pwsh tools/loop_split_scan.ps1 -File src/client/loop/loop_client_part_00.cpp
      pwsh tools/loop_split_scan.ps1 -StartLine 1608 -EndLine 4054   # depth relative scan
      pwsh tools/loop_split_scan.ps1 -Boundaries                     # only the depth-0 list

    See docs/plans/plan-loopClientSplit.md (phase LCS-P0.2).
#>
[CmdletBinding()]
param(
    [string]$File = "src/client/loop_client.cpp",
    [int]$StartLine = 0,
    [int]$EndLine = 0,
    [switch]$Boundaries
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $File)) {
    Write-Error "File not found: $File"
    exit 1
}

$lines = Get-Content -LiteralPath $File
$total = $lines.Count

# --- brace-depth walk -------------------------------------------------------
# Tracks { } depth while skipping comments and string/char literals.
# Records the 1-based line number whenever depth returns to the baseline (0,
# or 0 relative to a windowed scan).
$depth = 0
$inBlockComment = $false
$boundaryLines = New-Object System.Collections.Generic.List[int]
$lineNo = 0

# Windowed scan: when StartLine/EndLine given, depth is tracked relative to the
# window start (so interior depth-1 seams show as "depth 0" within the window).
$winStart = if ($StartLine -gt 0) { $StartLine } else { 1 }
$winEnd   = if ($EndLine   -gt 0) { $EndLine   } else { $total }

foreach ($line in $lines) {
    $lineNo++
    if ($lineNo -lt $winStart) { continue }
    if ($lineNo -gt $winEnd)   { break }

    $chars = $line.ToCharArray()
    $i = 0
    $n = $chars.Length
    while ($i -lt $n) {
        $c = $chars[$i]
        if ($inBlockComment) {
            if ($c -eq '*' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '/') {
                $inBlockComment = $false; $i += 2; continue
            }
            $i++; continue
        }
        if ($c -eq '/' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '/') { break }           # // comment: rest of line
        if ($c -eq '/' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '*') { $inBlockComment = $true; $i += 2; continue }
        if ($c -eq '"') {                                                                   # skip string literal
            $i++
            while ($i -lt $n) {
                if ($chars[$i] -eq '\') { $i += 2; continue }
                if ($chars[$i] -eq '"') { $i++; break }
                $i++
            }
            continue
        }
        if ($c -eq "'") {                                                                   # skip char literal
            $i++
            while ($i -lt $n) {
                if ($chars[$i] -eq '\') { $i += 2; continue }
                if ($chars[$i] -eq "'") { $i++; break }
                $i++
            }
            continue
        }
        if ($c -eq '{') { $depth++ }
        elseif ($c -eq '}') {
            $depth--
            if ($depth -eq 0) { $boundaryLines.Add($lineNo) }
        }
        $i++
    }
}

if ($Boundaries) {
    $boundaryLines -join ", "
    exit 0
}

# --- goto / label inventory -------------------------------------------------
$labels = New-Object System.Collections.Generic.HashSet[string]
$gotos  = New-Object System.Collections.Generic.List[string]
$lineNo = 0
foreach ($line in $lines) {
    $lineNo++
    # label definition:  ^optionalspace identifier :   (not  :: , not  ? : )
    if ($line -match '^\s*([A-Za-z_]\w*)\s*:(?!:)') {
        # Exclude common false positives: default:, case X:, public:/private:, ternary fragments.
        $cand = $Matches[1]
        if ($cand -notin @('default','case','public','private','protected')) {
            [void]$labels.Add($cand)
        }
    }
    foreach ($m in [regex]::Matches($line, '\bgoto\s+([A-Za-z_]\w*)\s*;')) {
        $gotos.Add($m.Groups[1].Value)
    }
}
$externalTargets = $gotos | Sort-Object -Unique | Where-Object { -not $labels.Contains($_) }

Write-Output "==== loop_split_scan: $File ===="
Write-Output "Total lines           : $total"
Write-Output ""
Write-Output "Brace-depth-0 boundary lines (legal depth-0 cut points):"
Write-Output ("  " + ($boundaryLines -join ", "))
Write-Output ""
Write-Output "Final brace depth (should be 0 for a legal flat block): $depth"
Write-Output ""
Write-Output "goto targets NOT defined as labels in this file (cross-file / external):"
if ($externalTargets) { $externalTargets | ForEach-Object { Write-Output "  $_" } }
else { Write-Output "  (none)" }

