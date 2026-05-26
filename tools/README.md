# tools/
Build-time utilities invoked by `CMakeLists.txt`.
## `build_icon.ps1`
Wraps a single square PNG in an ICO file as the only sub-image, then
lets Windows handle every size the shell needs. This avoids GDI+'s
mediocre pre-rasterized scaling at small sizes (16/20/24 px); the
DWM / Windows Imaging Component scaler used at display time produces
noticeably cleaner results from a 256x256 source than per-size
bicubic downsamples.
```
assets/images/icon.png        (you edit this; must be square)
        |                     | <= 256 px: embed verbatim
        |                     |  > 256 px: downscale ONCE to 256 with HQ bicubic
        v
${CMAKE_BINARY_DIR}/generated/icons/u6o7.ico  (single PNG sub-image)
${CMAKE_BINARY_DIR}/generated/icons/small.ico (identical copy)
        v  configure_file(src/common/u6o7.rc.in -> generated/u6o7.rc)
${CMAKE_BINARY_DIR}/generated/u6o7.rc
        v  RC compile
        v  Linker
Ultima VI Online*.exe
```
### Refresh the icon
1. Drop a new square PNG at `assets/images/icon.png`.
2. Build any target. CMake re-runs `build_icon.ps1`, re-emits the `.rc`,
   re-compiles it, and relinks the EXEs.
No source-code touchups needed.
### Tips for crisp small renders
- Use a **square** source (the script warns if not).
- Native source size of exactly **256x256** is the gold standard: the
  PNG is embedded verbatim with zero re-encoding.
- Larger sources (e.g. 1024x1024) work fine; the script pre-scales them
  to 256 once, with high-quality bicubic settings.
- Avoid sources with sub-16-pixel details (tiny text, hairline borders);
  they will alias at title-bar size no matter the scaler. If you need a
  bespoke 16x16/32x32 frame, hand-author it and re-add a multi-entry
  path here (see git history for the old multi-size script).
### Run by hand
```powershell
powershell -ExecutionPolicy Bypass `
    -File tools\build_icon.ps1 `
    -SourcePng assets\images\icon.png `
    -OutputIco "C:\path\to\u6o7.ico,C:\path\to\small.ico"
```
When invoked via `powershell.exe -File`, the `-OutputIco` argument is a
single comma- or semicolon-separated string (PowerShell's `[string[]]`
binder for that mode does not auto-split). When dot-sourced from another
PS session you can pass a real `@(...)` array.