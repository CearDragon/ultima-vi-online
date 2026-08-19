$ErrorActionPreference = 'Stop'

$root = Join-Path $PSScriptRoot '..'
$settingsHeader = [System.IO.File]::ReadAllText((Join-Path $root 'src\client\data_client.h'))
$setup = [System.IO.File]::ReadAllText((Join-Path $root 'src\client\setup_client.inc'))
$shutdown = [System.IO.File]::ReadAllText((Join-Path $root 'src\common\u6o7.cpp'))
$worldRender = [System.IO.File]::ReadAllText((Join-Path $root 'src\client\loop\loop_client_part_world_render.cpp'))
$panelDraw = [System.IO.File]::ReadAllText((Join-Path $root 'src\client\loop\loop_client_part_panel_draw.cpp'))

$settingsStruct = [regex]::Match(
    $settingsHeader,
    'struct client_settings \{(?<body>.*?)\n\};',
    [System.Text.RegularExpressions.RegexOptions]::Singleline
)
if (-not $settingsStruct.Success) {
    throw 'Could not find client_settings.'
}
if (-not $settingsStruct.Groups['body'].Value.TrimEnd().EndsWith('unsigned long name_display_colour;')) {
    throw 'name_display_colour must remain the final append-only settings field.'
}

$defaultIndex = $setup.IndexOf('cltset2.name_display_colour=0;')
$readIndex = $setup.IndexOf('get(tfh,&cltset2,clientsettingsbytes);')
if ($defaultIndex -lt 0 -or $readIndex -lt 0 -or $defaultIndex -gt $readIndex) {
    throw 'The old-file name colour default must be established before settings are read.'
}
if (-not $setup.Contains('if (clientsettingsbytes>(long)sizeof(client_settings)) clientsettingsbytes=sizeof(client_settings);')) {
    throw 'settings.bin loading is not bounded to client_settings size.'
}
if (-not $setup.Contains('u6o_namecolour=cltset2.name_display_colour;')) {
    throw 'The persisted name colour is not restored.'
}
if (-not $shutdown.Contains('cltset.name_display_colour = u6o_namecolour;')) {
    throw 'The live name colour is not saved.'
}

if (-not $worldRender.Contains('txtouts(ps,x+1,y+1,t);')) {
    throw 'Player names do not use the shared one-pixel outline helper.'
}
if (-not $panelDraw.Contains('txtouts(ps,x+1,yk+1,t);')) {
    throw 'Previous status messages do not use the shared one-pixel outline helper.'
}

Write-Host 'Name rendering and backward-compatible colour persistence checks passed.'