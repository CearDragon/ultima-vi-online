# MPRES-P1 — Modern swap-chain presenter (retire DirectDraw 7 from the present path)

**Risk tier:** T3 (per-frame client render/present path; on-screen result must
match the legacy `BitBlt`/`StretchBlt` present, point-sampled). Behavior-
PRESERVING: the rasterizer and the `ps->o` RGB565 byte layout are untouched; only
*how the finished framebuffer reaches the window* changes. No wire/`.sav`/RNG
impact (client-render only). **Do not** bump `U6O_VERSION`.

Tracks `docs/plans/in-progress/plan-modernPresenter.md` (phases `MPRES-P*`).

---

## MPRES-P0.1 — Baseline (recorded)

Tri-target **Debug** build is green against the pre-configured
`cmake-build-debug/` tree (MSVC `amd64_x86`, x86 output):

| Target | Result | Warnings |
|---|---|---|
| `client` | links `bin/client/debug/Ultima VI Online.exe` | only pre-existing `C4731` (inline-asm `ebp`, `loop_client_part_world_render.cpp`) |
| `host`   | links `bin/host/debug/Ultima VI Online Host.exe` | none new |
| `both`   | links `bin/client/debug/Ultima VI Online Full.exe` | only pre-existing `C4731` |

> **Build environment note (important for any agent/session):** the agent shell
> does **not** inherit the MSVC Developer environment, so `cl.exe` cannot find
> `windows.h`/`winsock2.h` even against the existing `cmake-build-debug/` tree.
> Initialise it once per shell before building:
>
> ```powershell
> $vc = "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
> cmd /c "`"$vc`" amd64_x86 && set" | ForEach-Object {
>     if ($_ -match '^(.*?)=(.*)$') { Set-Item -Path "env:$($matches[1])" -Value $matches[2] }
> }
> cmake.exe --build cmake-build-debug --target both -j 18
> ```
>
> `amd64_x86` matches the cached `Hostx64\x86` cross toolset. **Never** configure
> a fresh build dir (it fails on missing Windows headers).

Release config is **not** built here — the committed tree is Debug-only
(`cmake-build-debug/`). Build Release via the CLion/VS profiles in `.idea/cmake.xml`
when validating on hardware.

MM-P9.x present/text `GetDC` point-fixes are in place (`g_text_dc_cache`
default 1, cached present DC in `refresh(surf*)`), so the modern present's win is
cleanly attributable.

---

## MPRES-P0.4 — API decision: **Direct3D 11 + DXGI swap chain** (chosen)

### Decision

Present the existing RGB565 system-memory framebuffer (`ps->o`) through a
**Direct3D 11 device + DXGI swap chain**, uploading it to a **dynamic
`DXGI_FORMAT_B5G6R5_UNORM` texture** and drawing it as a point-sampled
full-screen quad, letterboxed by a viewport computed with the *same* math as
`blit_letterbox`.

### Why D3D11 and not Direct2D (the plan's listed fallback)

The plan offered Direct2D-on-DXGI as the "simpler" alternative. It is rejected as
the primary because **Direct2D has no native 16-bpp RGB565 bitmap format**
(`ID2D1Bitmap` supports essentially `B8G8R8A8`/`R8G8B8A8`/`A8`). Using D2D would
force a CPU RGB565→BGRA32 conversion every frame — i.e. it would *re-introduce*
exactly the `p16to32*` asm converters MPRES-P2 exists to delete. D3D11 supports
`DXGI_FORMAT_B5G6R5_UNORM` as a shader-resource/texture format, so the GPU
samples RGB565 directly with **zero CPU conversion**, which is the structural win
this plan is after.

GDI `StretchDIBits` from an app-owned DIB remains the documented fallback of last
resort (no new deps, still removes `IDirectDrawSurface::GetDC` from the present);
it is not chosen because it keeps a GDI present and a CPU scale.

### Equivalence-capture idiom (how P1+ proves parity)

The present is the one place the plan explicitly allows the *scaled* on-screen
result to differ only by **scaling filter**. Source `ps->o` pixels stay
bit-exact; we reproduce the current `COLORONCOLOR` `StretchBlt` with
**point sampling** (`D3D11_FILTER_MIN_MAG_MIP_POINT`).

Three capture scenarios for legacy-vs-modern comparison of a *fixed* `ps->o`:

1. **1:1** (client area == back-buffer): expect **byte-exact** (both are a 1:1
   copy; `blit_letterbox` already `BitBlt`s without stretch at `s==1.0`).
2. **Letterboxed / downscaled** (`s < 1.0`): expect point-sampled equality;
   document any single-pixel rounding delta in the scale arithmetic and get
   sign-off (this is the sanctioned present-path filter exception).
3. **Maximized** (client larger than back-buffer): `blit_letterbox` clamps
   `s` to `1.0` (no upscale) and centres with black bars — the presenter must
   reproduce the **identical** dst rect and bar fill.

In all three, **`blit_offx` / `blit_offy` / `blit_scale` must be identical** to
the legacy values for the same client/back-buffer sizes — `WndProc` mouse
mapping and `loop_client_part_panel_hittest.cpp` read them every frame. The
mouse-mapping check (click a known UI hotspot at several window sizes and confirm
it maps to the same source widget) is the cheapest high-signal parity test.

### Letterbox math the presenter MUST mirror (from `blit_letterbox`)

```
sx = clientW / srcW ; sy = clientH / srcH ; s = min(sx, sy)
if (s > 1.0) s = 1.0          // never upscale past native
if (s <= 0.0) s = 1.0
dstW = round(srcW * s) ; dstH = round(srcH * s)
dstX = (clientW - dstW) / 2 ; dstY = (clientH - dstH) / 2
// publish: blit_offx = dstX ; blit_offy = dstY ; blit_scale = s
// black-fill the bars outside the dst rect
```

`srcW/srcH` are `s->d.dwWidth`/`s->d.dwHeight`; the RGB565 row stride to upload
is `s->d.lPitch` bytes (DirectDraw pads above `srcW*2` at some widths — the
texture upload must honour `lPitch` per row, not assume `srcW*2`).

---

## MPRES-P0.2 / P0.3 — Golden capture + benchmark (design recorded; execution pending hardware)

These two require **running** the client (real window + driver), which is a
user-hardware task; the harness *design* is recorded here so P1.4 can execute it:

- **Golden present capture (P0.2):** drive a fixed `ps->o` (e.g. a deterministic
  test pattern or a frozen frame) through the present at the three window
  geometries above; capture the on-screen pixels (e.g. `BitBlt` the window DC to
  a DIB, or DXGI back-buffer readback) and diff legacy vs modern. Gate behind the
  `modernpresent` switch so both paths render the identical `ps->o`.
- **Present microbenchmark (P0.3):** time N presents of a fixed `ps->o`
  (per-present ms + FPS) on the legacy path now, and on the modern path in P1.4;
  require no meaningful regression.
- **Leak proof (the point):** on NVIDIA, idle `commitKB` (the `U6O-DIAG`
  heartbeat) must be **flat** with `modernpresent` on, with **zero**
  `IDirectDrawSurface::GetDC` in the present path.

---

## Session handoff (this doc)

- **2026-06-29.** MPRES-P0.1 (Debug tri-target baseline) recorded; API decided
  (**D3D11 + DXGI**, D2D rejected for lack of native RGB565). P0.2/P0.3 designs
  recorded; their *execution* and P1.4/P1.5 sign-off are user-hardware tasks.
  Next: land the flag-gated `Presenter` (P1.1–P1.3, default **off**), then run
  the P0.2/P0.3/P1.4 captures on the NVIDIA box before flipping the default
  (P1.5). P2–P5 are gated on that sign-off.
