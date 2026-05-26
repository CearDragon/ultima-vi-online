param(
    [string]$Dump = "C:\repos\ultima-vi-online\crash.dmp",
    [string]$Exe  = "C:\repos\ultima-vi-online\bin\client\debug\Ultima VI Online.exe",
    [string]$Pdb  = "C:\repos\ultima-vi-online\build\Ultima VI Online.pdb"
)

$ErrorActionPreference = 'Stop'

# --- Load dump into memory ---
$bytes = [System.IO.File]::ReadAllBytes($Dump)
$ms = New-Object System.IO.MemoryStream(,$bytes)
$br = New-Object System.IO.BinaryReader($ms)

function Seek([int]$rva) { $ms.Position = $rva }
function R32() { return $br.ReadUInt32() }
function R64() { return $br.ReadUInt64() }

# --- Header ---
$sig = R32; $ver = R32; $numStreams = R32; $dirRva = R32
if ($sig -ne 0x504D444D) { throw "Not a MDMP file (sig=0x$($sig.ToString('X'))" }
Write-Host "Minidump streams: $numStreams"

# --- Directory ---
Seek $dirRva
$dirs = @()
for ($i=0; $i -lt $numStreams; $i++) {
    $dirs += [pscustomobject]@{ Type=R32; Size=R32; Rva=R32 }
}

function GetDir([int]$type) { return $dirs | Where-Object { $_.Type -eq $type } | Select-Object -First 1 }

# --- ExceptionStream (6) ---
$exc = GetDir 6
$exceptionAddress = $null
$threadId = $null
$ctxRva = 0; $ctxSize = 0
if ($exc) {
    Seek $exc.Rva
    $threadId = R32; $null = R32 # alignment
    $excCode = R32; $excFlags = R32; $excRecord = R64; $excAddr = R64
    $numParams = R32; $null = R32
    # 15 * UINT64 params
    for ($p=0; $p -lt 15; $p++) { $null = R64 }
    $ctxSize = R32; $ctxRva = R32
    $exceptionAddress = $excAddr
    Write-Host ("Exception thread: {0} (0x{0:X})  Code: 0x{1:X8}  Address: 0x{2:X8}" -f $threadId, $excCode, $excAddr)
    Write-Host ("Context RVA: 0x{0:X} (size {1})" -f $ctxRva, $ctxSize)
}

# --- Read CONTEXT (x86) ---
$eip = $esp = $ebp = 0
if ($ctxRva -gt 0) {
    Seek $ctxRva
    $contextFlags = R32
    # skip Dr0..Dr7 (6 * 4) and FloatSave (112)
    $ms.Position = $ctxRva + 140
    $segGs=R32;$segFs=R32;$segEs=R32;$segDs=R32
    $edi=R32;$esi=R32;$ebx=R32;$edx=R32;$ecx=R32;$eax=R32
    $ebp=R32;$eip=R32;$segCs=R32;$eflags=R32;$esp=R32;$segSs=R32
    Write-Host ("ContextFlags=0x{0:X}  EIP=0x{1:X8} ESP=0x{2:X8} EBP=0x{3:X8}" -f $contextFlags,$eip,$esp,$ebp)
    Write-Host ("EAX=0x{0:X8} EBX=0x{1:X8} ECX=0x{2:X8} EDX=0x{3:X8} ESI=0x{4:X8} EDI=0x{5:X8}" -f $eax,$ebx,$ecx,$edx,$esi,$edi)
}

# --- ModuleListStream (4) ---
$modules = @()
$mod = GetDir 4
if ($mod) {
    Seek $mod.Rva
    $numMods = R32
    for ($m=0; $m -lt $numMods; $m++) {
        $base = R64
        $size = R32
        $chk = R32
        $tds = R32
        $nameRva = R32
        # skip VS_FIXEDFILEINFO (52), CvRecord(8), MiscRecord(8), Reserved0(8), Reserved1(8)
        $ms.Position += 52 + 8 + 8 + 8 + 8
        # read name
        $cur = $ms.Position
        Seek $nameRva
        $nameLen = R32
        $nameBytes = $br.ReadBytes($nameLen)
        $name = [System.Text.Encoding]::Unicode.GetString($nameBytes)
        $ms.Position = $cur
        $modules += [pscustomobject]@{ Base=$base; Size=$size; End=($base+$size); TimeStamp=$tds; Name=$name }
    }
}

function FindModule([uint64]$addr) {
    foreach ($m in $modules) {
        if ($addr -ge $m.Base -and $addr -lt $m.End) {
            return [pscustomobject]@{ Mod=$m; Offset=($addr - $m.Base) }
        }
    }
    return $null
}

Write-Host "`n=== Modules ==="
$modules | ForEach-Object {
    "{0,-60} base=0x{1:X8} size=0x{2:X}" -f (Split-Path $_.Name -Leaf), $_.Base, $_.Size
} | Out-Host

Write-Host "`n=== Exception ==="
$f = FindModule ([uint64]$exceptionAddress)
if ($f) {
    "Exception in module {0} at +0x{1:X}" -f (Split-Path $f.Mod.Name -Leaf), $f.Offset
} else {
    "Exception address 0x{0:X8} not in any loaded module (likely jump into garbage memory)" -f $exceptionAddress
}

# --- MemoryListStream (5) to gather stack region(s) ---
$memRegions = @()
$mem = GetDir 5
if ($mem) {
    Seek $mem.Rva
    $numRanges = R32
    for ($r=0; $r -lt $numRanges; $r++) {
        $start = R64
        $sz = R32
        $rva = R32
        $memRegions += [pscustomobject]@{ Start=$start; Size=$sz; End=($start+$sz); Rva=$rva }
    }
}
# Memory64ListStream (9) fallback
$mem64 = GetDir 9
if ($mem64) {
    Seek $mem64.Rva
    $numRanges = R64
    $baseRva = R64
    $cursor = $baseRva
    for ($r=0; $r -lt $numRanges; $r++) {
        $start = R64
        $sz = R64
        $memRegions += [pscustomobject]@{ Start=$start; Size=[uint32]$sz; End=($start+$sz); Rva=[uint32]$cursor }
        $cursor += $sz
    }
}

function ReadDword([uint64]$addr) {
    foreach ($r in $memRegions) {
        if ($addr -ge $r.Start -and ($addr + 4) -le $r.End) {
            $off = [int]($addr - $r.Start)
            $ms.Position = $r.Rva + $off
            return $br.ReadUInt32()
        }
    }
    return $null
}

# --- Stack walk via EBP chain ---
Write-Host "`n=== Stack walk (EBP chain) ==="
$frame = 0
$curEbp = [uint64]$ebp
$curEip = [uint64]$eip
$first = $true
$visited = @{}
while ($curEbp -ne 0 -and -not $visited.ContainsKey([string]$curEbp)) {
    $visited[[string]$curEbp] = $true
    $modInfo = FindModule $curEip
    $tag = if ($modInfo) {
        "{0}+0x{1:X}" -f (Split-Path $modInfo.Mod.Name -Leaf), $modInfo.Offset
    } else { "<unknown>" }
    "  [{0,2}] EBP=0x{1:X8}  RetAddr=0x{2:X8}  {3}" -f $frame, $curEbp, $curEip, $tag | Out-Host

    $savedEbp = ReadDword $curEbp
    $retAddr  = ReadDword ($curEbp + 4)
    if ($savedEbp -eq $null -or $retAddr -eq $null) {
        Write-Host "  (cannot read frame: ran out of stack memory)"
        break
    }
    if ($savedEbp -eq 0 -or $retAddr -eq 0) { break }
    $curEbp = [uint64]$savedEbp
    $curEip = [uint64]$retAddr
    $frame++
    if ($frame -gt 60) { break }
}

# --- Try to dump bytes near EIP and around ESP for context ---
function HexDump([uint64]$addr, [int]$count) {
    foreach ($r in $memRegions) {
        if ($addr -ge $r.Start -and $addr -lt $r.End) {
            $off = [int]($addr - $r.Start)
            $avail = [int]($r.End - $addr)
            $take = [Math]::Min($count, $avail)
            $ms.Position = $r.Rva + $off
            $buf = $br.ReadBytes($take)
            return ($buf | ForEach-Object { $_.ToString('X2') }) -join ' '
        }
    }
    return "<not in dump>"
}

Write-Host "`n=== Bytes at EIP (0x$($eip.ToString('X8'))) ==="
HexDump ([uint64]$eip) 32 | Out-Host

Write-Host "`n=== Stack at ESP (0x$($esp.ToString('X8')), 128 bytes) ==="
$dump = HexDump ([uint64]$esp) 128
$dump | Out-Host

# --- Try to symbolize using PDB via dia2dump if present, otherwise via funclist from PDB strings ---
# Simple heuristic: look for "Ultima VI Online.exe" address inside any executable module
Write-Host "`n=== Done ==="

