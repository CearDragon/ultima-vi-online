$ErrorActionPreference = 'Stop'

$sourcePath = Join-Path $PSScriptRoot '..\src\server\loop\loop_host_part_d_use_a.cpp'
$source = [System.IO.File]::ReadAllText($sourcePath)
$lockedDoorBranch = [regex]::Match(
    $source,
    'if \(i2 >= 8\) \{(?<body>.*?)goto finishuse; //door is locked!',
    [System.Text.RegularExpressions.RegexOptions]::Singleline
)

if (-not $lockedDoorBranch.Success) {
    throw 'Could not find the locked-door use branch.'
}

$body = $lockedDoorBranch.Groups['body'].Value
$requiredFragments = @(
    't->d2[0] = 2;',
    't2->ds[0] = 3;',
    't2->ds[0] = myobj->x;',
    't2->ds[0] = myobj->y - 1;',
    'txtset(t3, "It''s locked!");',
    'txtadd(t, t3);'
)

foreach ($fragment in $requiredFragments) {
    if (-not $body.Contains($fragment)) {
        throw "Locked-door feedback is not positioned above the door; missing: $fragment"
    }
}

if ($body.Contains('txtsetchar(t, 8);')) {
    throw 'Locked-door feedback still uses the screen-edge status-message channel.'
}

Write-Host 'Locked-door feedback uses positioned world text.'