<#
.SYNOPSIS
Renames legacy numeric player saves to USERNAME.SAV.

.DESCRIPTION
Reads the username embedded in each numeric .SAV file, applies the same
uppercase A-Z/0-9 and 16-character normalization as the host, validates the
entire migration for collisions, and only then performs any renames. Save file
contents are never modified.

Save versions 1, 2, 4, and 5 are read directly. Legacy version 3 saves are
decrypted in memory using the original MSVC rand() sequence; the file itself
is still only renamed.

.EXAMPLE
.\tools\migrate_save_filenames.ps1 -SaveDirectory .\host\save -WhatIf
.\tools\migrate_save_filenames.ps1 -SaveDirectory .\host\save
#>
[CmdletBinding(SupportsShouldProcess = $true, ConfirmImpact = 'Medium')]
param(
    [Parameter(Mandatory = $true)]
    [ValidateNotNullOrEmpty()]
    [string]$SaveDirectory
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

function Get-MsvcEncryptionTable {
    $table = [byte[]]::new(65536)
    [uint32]$state = 128
    for ($index = 0; $index -lt $table.Length; $index++) {
        $state = [uint32](([uint64]$state * 214013 + 2531011) -band 0xFFFFFFFFL)
        $randomValue = ($state -shr 16) -band 0x7FFF
        $table[$index] = [byte]($randomValue -shr 7)
    }
    return ,$table
}

function Get-DecryptedVersion3Bytes([byte[]]$Bytes) {
    if ($Bytes.Length -lt 8) {
        throw 'Version 3 save is too short to contain its encryption trailer.'
    }

    $result = [byte[]]$Bytes.Clone()
    $table = Get-MsvcEncryptionTable
    $offset = [int][BitConverter]::ToSingle($result, $result.Length - 4)
    $previousPlainByte = 0
    for ($index = 2; $index -lt $result.Length - 4; $index++) {
        $value = [int]$result[$index] - [int]$table[$offset]
        $offset = ($offset + 1) -band 0xFFFF
        if ($value -lt 0) { $value += 256 }
        $value -= $previousPlainByte
        if ($value -lt 0) { $value += 256 }
        $value = $value -bxor 0xFF
        $previousPlainByte = $value
        $result[$index] = [byte]$value
    }
    return ,$result
}

function Get-SaveUsername([System.IO.FileInfo]$SaveFile) {
    $bytes = [System.IO.File]::ReadAllBytes($SaveFile.FullName)
    if ($bytes.Length -lt 4) {
        throw "Save is too short to contain a username: $($SaveFile.FullName)"
    }

    $version = [BitConverter]::ToUInt16($bytes, 0)
    if ($version -eq 3) {
        $bytes = Get-DecryptedVersion3Bytes $bytes
    }
    elseif ($version -notin @(1, 2, 4, 5)) {
        throw "Unsupported save version $version in $($SaveFile.FullName)"
    }

    $lengthOffset = if ($version -eq 5) { 3 } else { 2 }
    $usernameOffset = $lengthOffset + 1
    $usernameLength = [int]$bytes[$lengthOffset]
    if ($usernameLength -lt 1 -or $usernameOffset + $usernameLength -gt $bytes.Length) {
        throw "Invalid username length in $($SaveFile.FullName)"
    }

    $embeddedUsername = [Text.Encoding]::GetEncoding(28591).GetString(
        $bytes, $usernameOffset, $usernameLength)
    $normalizedUsername = -join $embeddedUsername.ToUpperInvariant().ToCharArray().Where({
        ($_ -ge 'A' -and $_ -le 'Z') -or ($_ -ge '0' -and $_ -le '9')
    })
    if ($normalizedUsername.Length -gt 16) {
        $normalizedUsername = $normalizedUsername.Substring(0, 16)
    }
    if ($normalizedUsername.Length -eq 0) {
        throw "Username normalizes to an empty filename in $($SaveFile.FullName)"
    }
    return $normalizedUsername
}

$directory = Get-Item -LiteralPath $SaveDirectory
if (-not $directory.PSIsContainer) {
    throw "SaveDirectory is not a directory: $SaveDirectory"
}

$migration = @(
    Get-ChildItem -LiteralPath $directory.FullName -File |
        Where-Object { $_.Name -match '^\d+\.sav$' } |
        Sort-Object { [int64]$_.BaseName } |
        ForEach-Object {
            $username = Get-SaveUsername $_
            [PSCustomObject]@{
                Source = $_
                Target = Join-Path $directory.FullName ($username + '.SAV')
            }
        }
)

$duplicateTargets = $migration | Group-Object { $_.Target.ToUpperInvariant() } |
    Where-Object Count -gt 1
if ($duplicateTargets) {
    $conflicts = $duplicateTargets | ForEach-Object {
        ($_.Group.Source.Name -join ', ') + ' -> ' + $_.Group[0].Target
    }
    throw "Multiple saves map to the same username filename:`n$($conflicts -join "`n")"
}

$sourcePaths = @{}
foreach ($item in $migration) { $sourcePaths[$item.Source.FullName.ToUpperInvariant()] = $true }
$existingPaths = @{}
Get-ChildItem -LiteralPath $directory.FullName -File | ForEach-Object {
    $existingPaths[$_.FullName.ToUpperInvariant()] = $true
}
foreach ($item in $migration) {
    $targetKey = $item.Target.ToUpperInvariant()
    if ($existingPaths.ContainsKey($targetKey) -and -not $sourcePaths.ContainsKey($targetKey)) {
        throw "Target already exists; no files were renamed: $($item.Target)"
    }
}

$approved = @()
foreach ($item in $migration) {
    if ($item.Source.FullName.Equals($item.Target, [StringComparison]::OrdinalIgnoreCase)) {
        Write-Host "Already named: $($item.Source.Name)"
        continue
    }
    if ($PSCmdlet.ShouldProcess($item.Source.FullName, "Rename to $([IO.Path]::GetFileName($item.Target))")) {
        $approved += $item
    }
}

$renamed = 0
$staged = @()
try {
    foreach ($item in $approved) {
        $stagingPath = Join-Path $directory.FullName ('.u6o-save-migration-' + [Guid]::NewGuid() + '.tmp')
        Move-Item -LiteralPath $item.Source.FullName -Destination $stagingPath
        $staged += [PSCustomObject]@{
            Original = $item.Source.FullName
            Staging = $stagingPath
            Target = $item.Target
        }
    }
    foreach ($item in $staged) {
        Move-Item -LiteralPath $item.Staging -Destination $item.Target
        $renamed++
    }
}
catch {
    for ($index = $staged.Count - 1; $index -ge 0; $index--) {
        $item = $staged[$index]
        $rollbackSource = if (Test-Path -LiteralPath $item.Staging) { $item.Staging } else { $item.Target }
        if (Test-Path -LiteralPath $rollbackSource) {
            Move-Item -LiteralPath $rollbackSource -Destination $item.Original -Force
        }
    }
    throw
}

Write-Host "Validated $($migration.Count) numeric save file(s); renamed $renamed."