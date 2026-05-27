# Image assets

## `icon.png`

Source for the multi-resolution Windows icon. The CMake icon pipeline
(`tools/build_icon.ps1`) consumes this at build time and generates
`*.ico` + `src/common/u6o7.rc`, both of which are git-ignored. Replace
`icon.png` and rebuild to re-skin the executable's icon.

## `splash.png` (optional)

Pre-game splash screen image. Shown for 3 seconds (see
`u6o::client::splash::kDefaultDurationMs` in `src/client/splash.h`)
when `Ultima VI Online.exe` (CLIENT build) starts, before the main
window is created.

### Supported formats

Anything GDI+ can open: **PNG**, JPG, BMP, GIF. PNG is recommended.

### Sizing

The splash window is sized to the image's native dimensions, clamped to a
max of **1280×720** while preserving aspect ratio. Anything smaller than
the cap is shown at native size; anything larger is downscaled with
high-quality bicubic. If you want a specific on-screen size, author the
image at that size and stay at or under 1280×720.

### Where the runtime looks for the image (first match wins)

Probed by `src/client/splash.cpp::kDefaultSearchPaths`, relative to the
EXE's working directory:

1. `.\splash.png`
2. `.\dr\splash.png`
3. `.\assets\images\splash.png`
4. `assets\images\splash.png`

For local dev that means dropping a `splash.png` directly in
`bin/client/debug/` next to the EXE is the simplest path. For shipping
builds, package it under `dr/` alongside the existing asset folder.

### Fallback behavior

If no splash image is found (or GDI+ fails to load it), the splash window
still appears for the full 3 seconds, painted black with a centered
"Ultima VI Online" title. The game then starts normally. Failures in the
splash code path *never* prevent the game from launching — see the
defensive guards at the bottom of `splash.cpp::Run`.

### Disabling the splash

There is no runtime flag yet. To skip the splash for an automated test
run, either comment out the `u6o::client::splash::Run(...)` call in
`src/common/u6o7.cpp::_tWinMain` or wrap it in a new command-line flag
(treat that as a follow-up if the need is recurring).

