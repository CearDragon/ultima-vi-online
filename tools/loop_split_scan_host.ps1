<#
    loop_split_scan_host.ps1  —  LHS-P0.2 host-aware analysis tool for the
    loop_host.cpp split. Host sibling of tools/loop_split_scan.ps1.

    Why a separate tool?
    --------------------
    tools/loop_split_scan.ps1 is preprocessor-blind. loop_host.cpp contains an
    `#ifdef CLIENT ... #else ... #endif` block (lines 1032-1041 in the
    2026-06-16 snapshot) where BOTH branches open a `{` but only one `}` closes
    it. A blind brace counter over-counts by +1 there, reports a bogus final
    depth of 1, and therefore an EMPTY depth-0 boundary list. See
    docs/plans/plan-loopHostSplit.md fact 4.

    What "host-aware" means here
    ----------------------------
    The whole file is one `if (NEThost) { ... }` block (fact 2). We follow the
    HOST compile branch while counting braces:
      * `#ifdef CLIENT`  -> skip the true-branch (CLIENT is undefined on host).
      * `#else` of that  -> keep it (the host branch).
      * `#ifndef CLIENT` -> keep; its `#else` is skipped.
      * `#ifdef HOST`    -> keep; `#ifndef HOST` skipped.
      * every other conditional (`#ifndef _DEBUG`, `#if ...`) -> COUNTED as-is.
        The `#ifndef _DEBUG` tail block is brace-balanced, so counting it is
        harmless to the net depth AND preserves its interior seam (26870),
        matching the plan's documented seam list.
    Preprocessor directive lines (those whose first non-blank char is `#`) never
    contribute braces themselves.

    Seam model
    ----------
    The outer `if (NEThost) {` puts the whole body at absolute brace depth 1.
    A "depth-1 seam" is a `}` that returns the absolute depth to `-SeamLevel`
    (default 1) — i.e. the close of a direct child statement of the outer block.
    Those are the only legal *top-level* cut points for this file.

    For the mega-block interior splits (LHS-P3..P6), pass `-SeamLevel 2` (or 3
    for the `if(ouln!=-1){ for(i...){ } }` nest in MEGA C) together with
    `-StartLine`/`-EndLine`: depth is always tracked from line 1 (so absolute
    depth is correct), but only boundaries inside the window are reported.

    Usage
    -----
      pwsh tools/loop_split_scan_host.ps1
      pwsh tools/loop_split_scan_host.ps1 -Boundaries
      pwsh tools/loop_split_scan_host.ps1 -File src/server/loop/loop_host_part_00.cpp
      pwsh tools/loop_split_scan_host.ps1 -SeamLevel 2 -StartLine 266 -EndLine 4105

    See docs/plans/plan-loopHostSplit.md (phase LHS-P0.2).
#>
[CmdletBinding()]
param(
    [string]$File = "src/server/loop_host.cpp",
    [int]$SeamLevel = 1,
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

# --- host-aware brace-depth walk -------------------------------------------
# Absolute { } depth, skipping comments and string/char literals, and skipping
# the brace contributions of preprocessor branches that the HOST config does
# not compile. Records the 1-based line number whenever a `}` returns the
# absolute depth to $SeamLevel, restricted to [winStart, winEnd].
$depth = 0
$inBlockComment = $false
$boundaryLines = New-Object System.Collections.Generic.List[int]
$lineNo = 0

# Preprocessor-conditional stack. Each entry is $true when the CURRENT branch
# is skipped (its braces must NOT be counted under the host config).
$ppStack = New-Object System.Collections.Generic.Stack[bool]
function Test-Skipping($stack) {
    foreach ($s in $stack) { if ($s) { return $true } }
    return $false
}

$winStart = if ($StartLine -gt 0) { $StartLine } else { 1 }
$winEnd   = if ($EndLine   -gt 0) { $EndLine   } else { $total }

foreach ($line in $lines) {
    $lineNo++

    # Preprocessor directive? Update the conditional stack; never scan braces.
    $trimmed = $line.TrimStart()
    if ($trimmed.StartsWith('#')) {
        $d = $trimmed.Substring(1).TrimStart()
        if ($d -match '^ifdef\s+(\w+)') {
            $sym = $Matches[1]
            # host config: HOST defined, CLIENT undefined, _DEBUG defined.
            $skip = ($sym -eq 'CLIENT')                       # #ifdef CLIENT -> skip
            $ppStack.Push($skip)
        }
        elseif ($d -match '^ifndef\s+(\w+)') {
            $sym = $Matches[1]
            $skip = ($sym -eq 'HOST')                         # #ifndef HOST -> skip; #ifndef CLIENT/_DEBUG -> keep
            $ppStack.Push($skip)
        }
        elseif ($d -match '^if\b') {
            $ppStack.Push($false)                             # unknown #if -> count
        }
        elseif ($d -match '^else\b') {
            if ($ppStack.Count -gt 0) { $top = $ppStack.Pop(); $ppStack.Push(-not $top) }
        }
        elseif ($d -match '^elif\b') {
            if ($ppStack.Count -gt 0) { [void]$ppStack.Pop(); $ppStack.Push($false) }
        }
        elseif ($d -match '^endif\b') {
            if ($ppStack.Count -gt 0) { [void]$ppStack.Pop() }
        }
        continue
    }

    $skipping = Test-Skipping $ppStack

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
        if ($c -eq '/' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '/') { break }
        if ($c -eq '/' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '*') { $inBlockComment = $true; $i += 2; continue }
        if ($c -eq '"') {
            $i++
            while ($i -lt $n) {
                if ($chars[$i] -eq '\') { $i += 2; continue }
                if ($chars[$i] -eq '"') { $i++; break }
                $i++
            }
            continue
        }
        if ($c -eq "'") {
            $i++
            while ($i -lt $n) {
                if ($chars[$i] -eq '\') { $i += 2; continue }
                if ($chars[$i] -eq "'") { $i++; break }
                $i++
            }
            continue
        }
        if (-not $skipping) {
            if ($c -eq '{') { $depth++ }
            elseif ($c -eq '}') {
                $depth--
                if ($depth -eq $SeamLevel -and $lineNo -ge $winStart -and $lineNo -le $winEnd) {
                    $boundaryLines.Add($lineNo)
                }
            }
        }
        $i++
    }
}

if ($Boundaries) {
    $boundaryLines -join ", "
    exit 0
}

# --- goto / label inventory (identical semantics to loop_split_scan.ps1) ----
$labels = New-Object System.Collections.Generic.HashSet[string]
$gotos  = New-Object System.Collections.Generic.List[string]
$lineNo = 0
foreach ($line in $lines) {
    $lineNo++
    if ($line -match '^\s*([A-Za-z_]\w*)\s*:(?!:)') {
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

Write-Output "==== loop_split_scan_host: $File ===="
Write-Output "Total lines           : $total"
Write-Output "Seam level (abs depth): $SeamLevel"
if ($StartLine -or $EndLine) { Write-Output "Window                : $winStart..$winEnd" }
Write-Output ""
Write-Output "Host-aware depth-$SeamLevel seam lines (legal cut points):"
Write-Output ("  " + ($boundaryLines -join ", "))
Write-Output ""
Write-Output "Seam count            : $($boundaryLines.Count)"
Write-Output "Final brace depth (host-aware; should be 0 for the full file): $depth"
Write-Output ""
Write-Output "goto targets NOT defined as labels in this file (external/dead):"
if ($externalTargets) { $externalTargets | ForEach-Object { Write-Output "  $_" } }
else { Write-Output "  (none)" }

