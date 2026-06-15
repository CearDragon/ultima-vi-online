<#
    loop_split_extract.ps1  —  mechanical line-range relocation for the LCS plan.

    Moves a contiguous 1-based line range [Start..End] out of -Source and into a
    new -Dest file, leaving the remaining lines in -Source. Pure byte-faithful
    relocation: it preserves the source's newline style (CRLF/LF) and trailing
    newline so the preprocessed token stream is unchanged (see LCS invariant 1
    and tools/loop_split_oracle.ps1).

    It does NOT touch the umbrella include list — the caller adds the new
    #include manually in the correct order (order is load-bearing).

    Usage
    -----
      pwsh tools/loop_split_extract.ps1 `
        -Source src/client/loop/loop_client_part_00.cpp `
        -Dest   src/client/loop/loop_client_part_input_top.cpp `
        -Start 1 -End 221

    The new -Dest file is written verbatim with the same newline style; -Source
    is rewritten without those lines. Prints the resulting line counts.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory)] [string]$Source,
    [Parameter(Mandatory)] [string]$Dest,
    [Parameter(Mandatory)] [int]$Start,
    [Parameter(Mandatory)] [int]$End
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $Source)) { Write-Error "Source not found: $Source"; exit 1 }
if ($Start -lt 1 -or $End -lt $Start) { Write-Error "Bad range $Start..$End"; exit 1 }

$raw = [IO.File]::ReadAllText($Source)
$nl = if ($raw -match "`r`n") { "`r`n" } else { "`n" }
$endsWithNL = $raw.EndsWith($nl)

# Split into physical lines without losing content. Trim a single trailing
# newline first so we don't get a phantom empty element.
$body = if ($endsWithNL) { $raw.Substring(0, $raw.Length - $nl.Length) } else { $raw }
$arr = $body -split [regex]::Escape($nl)
$count = $arr.Count
if ($End -gt $count) { Write-Error "End $End exceeds line count $count"; exit 1 }

# 1-based -> 0-based slices.
$moved = $arr[($Start - 1)..($End - 1)]
$beforeIdxEnd = $Start - 2
$afterIdxStart = $End
$before = if ($Start -gt 1) { $arr[0..$beforeIdxEnd] } else { @() }
$after  = if ($End -lt $count) { $arr[$afterIdxStart..($count - 1)] } else { @() }
$remain = @($before + $after)

# Reassemble with the original newline + trailing-newline behavior.
$destText   = ($moved  -join $nl) + $(if ($endsWithNL) { $nl } else { "" })
$sourceText = ($remain -join $nl) + $(if ($endsWithNL) { $nl } else { "" })

[IO.File]::WriteAllText($Dest, $destText)
[IO.File]::WriteAllText($Source, $sourceText)

Write-Output "Moved $($moved.Count) lines ($Start..$End) -> $Dest"
Write-Output "Source now has $($remain.Count) lines: $Source"

