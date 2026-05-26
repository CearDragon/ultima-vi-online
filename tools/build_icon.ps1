# Build a PNG-in-ICO from a single square PNG.
#
# Philosophy: embed the SOURCE image (or a single 256x256 downscale if
# it is larger than 256) as the only icon sub-image, then let Windows'
# own image scaler render every size the shell asks for. The Windows
# scaler (DWM/Imaging Component) handles 256->16/32/48 etc. with much
# better quality than GDI+ bicubic, and most legacy GDI+ pre-rasterized
# sizes look soft anyway when the source is detail-dense (text, fine
# borders) - which is what we hit with the Ultima VI Online icon art.
#
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools\build_icon.ps1 `
#       -SourcePng assets\images\icon.png `
#       -OutputIco "<path1>,<path2>,..."
#
# Invoked from CMakeLists.txt via add_custom_command, so editing the
# source PNG triggers a build-time re-emit of every output. Safe to run
# by hand too.
#
# - Skips rebuild if every output exists and is newer than the source.
# - ICO format: ICONDIR (6) + 1 ICONDIRENTRY (16) + PNG payload.
param(
    [Parameter(Mandatory = $true)] [string] $SourcePng,
    # When invoked via powershell.exe -File, the [string[]] binder does
    # NOT auto-split on whitespace/commas, so accept either a real PS
    # array (dot-sourced) OR a single delimited string and normalize.
    [Parameter(Mandatory = $true)] $OutputIco
)
$ErrorActionPreference = 'Stop'
if ($OutputIco -is [string]) {
    $OutputIco = $OutputIco -split '[,;]' | Where-Object { $_ -ne '' }
}
if (-not (Test-Path -LiteralPath $SourcePng)) {
    Write-Error "Source PNG not found: $SourcePng"
    exit 1
}
$srcInfo  = Get-Item -LiteralPath $SourcePng
$srcBytes = [IO.File]::ReadAllBytes((Resolve-Path -LiteralPath $SourcePng))
# Up-to-date check.
$allFresh = $true
foreach ($o in $OutputIco) {
    if (-not (Test-Path -LiteralPath $o)) { $allFresh = $false; break }
    if ((Get-Item -LiteralPath $o).LastWriteTimeUtc -lt $srcInfo.LastWriteTimeUtc) {
        $allFresh = $false; break
    }
}
if ($allFresh) {
    Write-Host "build_icon.ps1: outputs up to date, skipping ($SourcePng)"
    exit 0
}
# --- Parse PNG dimensions from IHDR --------------------------------------
# PNG layout: 8-byte signature, then chunks. First chunk is always IHDR:
#   offset  8 : length (4 BE, == 13)
#   offset 12 : type   (4, "IHDR")
#   offset 16 : width  (4 BE)
#   offset 20 : height (4 BE)
$pngSig = @(137,80,78,71,13,10,26,10)
for ($i = 0; $i -lt 8; $i++) {
    if ($srcBytes[$i] -ne $pngSig[$i]) {
        Write-Error "Source is not a PNG: $SourcePng (signature mismatch)"
        exit 1
    }
}
$wBytes = [byte[]]@($srcBytes[16], $srcBytes[17], $srcBytes[18], $srcBytes[19])
$hBytes = [byte[]]@($srcBytes[20], $srcBytes[21], $srcBytes[22], $srcBytes[23])
[Array]::Reverse($wBytes); [Array]::Reverse($hBytes)
$srcW = [BitConverter]::ToUInt32($wBytes, 0)
$srcH = [BitConverter]::ToUInt32($hBytes, 0)
if ($srcW -ne $srcH) {
    Write-Warning ("build_icon.ps1: source is {0}x{1}, not square. " +
                   "Windows will distort the icon when scaling. " +
                   "Crop or pad to a square PNG.") -f $srcW, $srcH
}
# --- Pick the payload ----------------------------------------------------
# If the source is already <=256 px, embed verbatim - byte-perfect
# reproduction of the artist's PNG, no GDI+ resampling artifacts. Above
# 256 we downscale once (with the best GDI+ settings) and ship that;
# Windows handles all further sizing from the 256 entry.
$nativePayload = $false
if ($srcW -le 256 -and $srcH -le 256) {
    $payloadBytes = $srcBytes
    $payloadW = $srcW
    $payloadH = $srcH
    $nativePayload = $true
} else {
    Add-Type -AssemblyName System.Drawing
    $srcImg = [System.Drawing.Image]::FromFile((Resolve-Path -LiteralPath $SourcePng))
    try {
        $bmp = New-Object System.Drawing.Bitmap 256, 256
        try {
            $g = [System.Drawing.Graphics]::FromImage($bmp)
            try {
                $g.InterpolationMode  = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
                $g.SmoothingMode      = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
                $g.PixelOffsetMode    = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
                $g.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality
                $g.DrawImage($srcImg, (New-Object System.Drawing.Rectangle 0, 0, 256, 256))
            } finally { $g.Dispose() }
            $ms = New-Object IO.MemoryStream
            $bmp.Save($ms, [System.Drawing.Imaging.ImageFormat]::Png)
            $payloadBytes = $ms.ToArray()
            $payloadW = 256
            $payloadH = 256
        } finally { $bmp.Dispose() }
    } finally { $srcImg.Dispose() }
}
# --- Assemble single-entry ICO ------------------------------------------
# ICONDIRENTRY width/height are 1 byte each; 0 means 256 (or any
# dimension that does not fit in a byte). For >256 native payloads we
# also write 0 - Windows reads the PNG IHDR for the truth and treats
# the entry as "high-DPI fallback".
$dimW = if ($payloadW -ge 256) { 0 } else { $payloadW }
$dimH = if ($payloadH -ge 256) { 0 } else { $payloadH }
$out = New-Object IO.MemoryStream
$bw  = New-Object IO.BinaryWriter $out
$bw.Write([uint16]0)              # reserved
$bw.Write([uint16]1)              # type = icon
$bw.Write([uint16]1)              # one entry
$bw.Write([byte]$dimW)
$bw.Write([byte]$dimH)
$bw.Write([byte]0)                # colors (paletted only; PNG is direct color)
$bw.Write([byte]0)                # reserved
$bw.Write([uint16]1)              # planes
$bw.Write([uint16]32)             # bpp
$bw.Write([uint32]$payloadBytes.Length)
$bw.Write([uint32](6 + 16))       # payload offset (right after dir+entry)
$bw.Write($payloadBytes)
$bw.Flush()
$icoBytes = $out.ToArray()
foreach ($o in $OutputIco) {
    $dir = [System.IO.Path]::GetDirectoryName($o)
    if ($dir -and -not (Test-Path -LiteralPath $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
    [IO.File]::WriteAllBytes($o, $icoBytes)
    $tag = if ($nativePayload) { "native ${srcW}x${srcH}" } else { "downscaled to 256x256" }
    Write-Host ("build_icon.ps1: wrote {0} bytes ({1}) -> {2}" -f `
                $icoBytes.Length, $tag, $o)
}