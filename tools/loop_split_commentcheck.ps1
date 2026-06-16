<#
    loop_split_commentcheck.ps1  —  LHS cut-safety helper.

    Reports, for each candidate cut line, whether that line begins INSIDE a
    /* ... */ block comment. Cutting an #include'd part file inside a block
    comment yields MSVC C1071 ("unexpected end of file found in comment"),
    because a block comment cannot span an #include boundary. This bit the
    MEGA A split (a 448-line commented-out reference block) — see
    docs/plans/plan-loopHostSplit.md.

    A cut line is SAFE only if insideBlockComment = False AND it is a clean
    statement boundary (blank / `}` / `;` / label). This tool checks the first
    condition; eyeball the second from the printed line.

    Usage
    -----
      pwsh tools/loop_split_commentcheck.ps1 -File src/server/loop/loop_host_part_00.cpp -Lines 1081,2130,2565,3278
      # Optionally restrict the walk start (depth/comment tracked from -From):
      pwsh tools/loop_split_commentcheck.ps1 -File <f> -Lines 17600,18200 -From 17260
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory)][string]$File,
    [Parameter(Mandatory)][int[]]$Lines,
    [int]$From = 1
)
$ErrorActionPreference = "Stop"
if (-not (Test-Path $File)) { Write-Error "File not found: $File"; exit 1 }
$L = Get-Content -LiteralPath $File
$want = @{}; foreach ($c in $Lines) { $want[$c] = $true }
$maxLine = ($Lines | Measure-Object -Maximum).Maximum
$inBC = $false
$state = @{}
for ($ln = $From; $ln -le $maxLine; $ln++) {
    if ($want.ContainsKey($ln)) { $state[$ln] = $inBC }
    $line = $L[$ln - 1]; $chars = $line.ToCharArray(); $i = 0; $n = $chars.Length
    while ($i -lt $n) {
        $c = $chars[$i]
        if ($inBC) {
            if ($c -eq '*' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '/') { $inBC = $false; $i += 2; continue }
            $i++; continue
        }
        if ($c -eq '/' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '/') { break }
        if ($c -eq '/' -and $i + 1 -lt $n -and $chars[$i + 1] -eq '*') { $inBC = $true; $i += 2; continue }
        if ($c -eq '"') { $i++; while ($i -lt $n) { if ($chars[$i] -eq '\') { $i += 2; continue }; if ($chars[$i] -eq '"') { $i++; break }; $i++ }; continue }
        if ($c -eq "'") { $i++; while ($i -lt $n) { if ($chars[$i] -eq '\') { $i += 2; continue }; if ($chars[$i] -eq "'") { $i++; break }; $i++ }; continue }
        $i++
    }
}
$bad = 0
foreach ($k in ($Lines | Sort-Object)) {
    $s = $state[$k]
    if ($s) { $bad++ }
    "line {0,6}: insideBlockComment = {1,-5}  | {2}" -f $k, $s, $L[$k - 1]
}
Write-Output ""
if ($bad -gt 0) { Write-Output "UNSAFE: $bad cut line(s) fall inside a block comment. Pick different lines." }
else { Write-Output "OK: all candidate cut lines are outside block comments (verify each is also a clean statement boundary)." }

