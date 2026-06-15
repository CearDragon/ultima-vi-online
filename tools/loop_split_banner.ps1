<#
    loop_split_banner.ps1  —  safely prepend a comment banner to an LCS part.

    The IDE edit tool tries to brace-balance these brace-seam part files (which
    are intentionally unbalanced) and corrupts them. This helper instead reads
    the file's raw bytes, prepends the banner text + the file's own newline,
    and writes it back verbatim — touching nothing else. Comments are stripped
    by `cl /EP`, so this never changes the token stream.

    -Banner is the comment block (use real newlines; they'll be normalized to
    the file's newline style). Each line should already start with `//`.

    Usage:
      pwsh tools/loop_split_banner.ps1 -File src/client/loop/x.cpp -Banner @"
      // LCS-P4.2: ...
      // ...
      "@
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory)] [string]$File,
    [Parameter(Mandatory)] [string]$Banner
)
$ErrorActionPreference = "Stop"
if (-not (Test-Path $File)) { Write-Error "File not found: $File"; exit 1 }

$raw = [IO.File]::ReadAllText($File)
$nl  = if ($raw -match "`r`n") { "`r`n" } else { "`n" }

# Normalize the banner's line endings to the file's, ensure it ends with one nl.
$bannerLines = $Banner -split "`r`n|`n"
$bannerText  = ($bannerLines -join $nl)
if (-not $bannerText.EndsWith($nl)) { $bannerText += $nl }

[IO.File]::WriteAllText($File, $bannerText + $raw)
Write-Output "Prepended $($bannerLines.Count)-line banner to $File"

