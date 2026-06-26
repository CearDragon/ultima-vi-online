# Modern Presenter — retire DirectDraw 7 from the client present path (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Phase prefix: **`MPRES-P*`** (tag code comments / commits / PR titles with the phase ID).

> Executed by the **`cpp-modernizer` agent** under the Prime Directive in
> `docs/plans/todo/modernization/README.md` (§1) and the rules in
> `.github/copilot-instructions.md`. This is a **behavior-preserving** rendering
> modernization, **not** a feature change. No wire/`.sav`/RNG impact (client
> render only); **do not** bump `U6O_VERSION`.

---

## Overview

The client is a **software rasterizer**: every tile/sprite/text pixel is written
by the inline-asm/raw blitters straight into `ps->o`, a 16-bpp **RGB565
system-memory** framebuffer. DirectDraw 7 is used for almost nothing but
(1) allocating that buffer, (2) clearing it (`cls`, already a raw memory fill —
MM-P9.6), and (3) **presenting** it to the window.

On Windows 10/11 + WDDM there is no DirectDraw hardware path. `ddraw.dll` is a
thin shim re-expressed over GDI + Direct3D/DXGI, and each vendor's user-mode
driver re-implements it differently. NVIDIA's `IDirectDrawSurface::GetDC`
emulation **leaks ~6 KB per call**; Intel's does not. The whole MM-P9 saga
(text `GetDC`, `cls` colour-fill, present `GetDC`) was a series of point-fixes
for *one root cause*: **we present through emulated DirectDraw 7.**

This plan removes that root cause by presenting through a **modern swap chain**
(Direct3D 11 / DXGI, with Direct2D-on-DXGI as the simpler alternative), leaving
the rasterizer and the `ps->o` RGB565 format **completely untouched**. The GPU
does the RGB565→display-format conversion and the aspect-preserving scale that
the inline-asm `p16to32*`/`p16to16*` converters and `blit_letterbox`'s
`StretchBlt` do today.

End state: **no `IDirectDraw*` interface anywhere in the client**, so the entire
NVIDIA legacy-emulation risk surface (per-`GetDC`, per-`Blt`, present-path
quirks) is gone — not just the three leaks MM-P9 patched.

---

## What this plan touches (and what it must NOT)

**Replaces (client-only, `src/client/myddraw.cpp` + `function_client.cpp`):**

| Symbol | File:line (approx) | Role today | After |
|---|---|---|---|
| `setupddraw()` | `myddraw.cpp:271` | `DirectDrawCreate`, primary surface `vs`, `DDRAW_display_pixelformat` | create DXGI device + swap chain for `hWnd` |
| `refresh(surf *s)` | `myddraw.cpp:505` | low-level present (cached-DC `BitBlt` to window) | upload `s->o` → present via swap chain |
| `blit_letterbox()` | `myddraw.cpp:60` | aspect scale + publishes `blit_offx/offy/blit_scale` | same math → swap-chain viewport; **publish identical globals** |
| `refresh()` | `function_client.cpp:2080` | per-mode compose + `p16to32`/`p16to16` asm convert + `img(vs,…)` / `refresh(psX)` | one path: present `ps->o`; GPU does convert + scale |
| `ddrawshutdown()` | `myddraw.cpp:1124` | release `dd`/`dd1`/surfaces | release swap-chain/device |
| `newsurf()` (later phases) | `myddraw.cpp:320` | DD sysmem/vidmem/primary surface alloc | plain owned RGB565 buffers (`->o`/`lPitch` preserved) |
| `img`/`img0` 2-/6-arg DD `Blt`s (later) | `myddraw.cpp` | `IDirectDrawSurface::Blt` (`bltCopy`/`bltKey`) | software/asm blits |

**Must NOT change (behavior preservation):**

- The rasterizer and `ps->o` **RGB565 byte layout** — every blitter keeps
  writing the same bytes in the same order. (T3 pixel-exact.)
- `blit_offx` / `blit_offy` / `blit_scale` semantics — `WndProc` mouse mapping
  and `loop_client_part_panel_hittest.cpp` read these every frame; the mapped
  client→source coordinates must be **identical**.
- Wire format, `struct player`, `.sav`, RNG order, `U6O_VERSION`.
- The **headless Linux host** — it never renders; the presenter is
  `#ifdef CLIENT` + Win32-only and sits behind the existing
  `src/common/platform/` seam. `host` and `both` must keep building.
- 32-bit / `/MACHINE:X86` — D3D11/DXGI are fully available in x86.

---

## API choice (decide in MPRES-P0, recommendation baked in)

Recommended: **Direct3D 11 + DXGI swap chain (flip model where available)**.

- `DXGI_FORMAT_B5G6R5_UNORM` is a native texture format → upload `ps->o`
  **directly, no CPU conversion** (deletes the `p16to32*`/`p16to16*` asm).
- A single dynamic texture (`Map`/`memcpy` the RGB565 rows, respecting
  `lPitch`) drawn as a full-screen quad; GPU samples + scales.
- **Point-sampling** (`D3D11_FILTER_MIN_MAG_MIP_POINT`) reproduces the current
  `COLORONCOLOR` `StretchBlt` exactly; linear is an optional later toggle.
- Letterbox = a viewport (or quad rect) inset to preserve aspect, computed by
  the **same math** as `blit_letterbox`, feeding the same `blit_*` globals.
- Native vsync via `IDXGISwapChain::Present(1, …)`; no GDI present, no DDraw.

Alternative (simpler, fewer shader bits): **Direct2D on a DXGI swap chain** —
`ID2D1Bitmap` from the RGB565 buffer, `DrawBitmap` with the letterbox dest rect
and `D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR`. Equivalent result; pick in
P0 based on which the agent can verify pixel-exactly fastest. Fallback of last
resort (no new deps, still removes `IDirectDrawSurface::GetDC`): GDI
`StretchDIBits` from an app-owned DIB.

---

## Phases

### MPRES-P0 — Baseline, scaffolding, API decision (Foundational, T0/T3-prep)

- ⬜ **MPRES-P0.1** Tri-target baseline build (`client`/`host`/`both`, Debug +
  Release); record warning counts (note pre-existing `C4731`). Confirm
  MM-P9.x is flat first (the leak this structurally cures must already be
  point-fixed so deltas are attributable).
- ⬜ **MPRES-P0.2** Golden present-capture harness: dump the **final on-screen
  pixels** for a fixed `ps->o` across the present paths (fullscreen 1:1,
  downscaled/letterboxed, and a non-16bpp display path) so P1+ can prove the
  swap-chain present matches the legacy `BitBlt`/`StretchBlt` byte-for-byte
  (point-sampled) — or document the *exact, intended* scaling-filter delta if
  bit-exact at all window sizes is impractical (the present is the one spot
  where the scaled result, not the source pixels, may differ by filter).
- ⬜ **MPRES-P0.3** Present microbenchmark (frames/sec + per-present ms) on the
  legacy path, to prove no regression in P1.
- ⬜ **MPRES-P0.4** Decide **D3D11 vs D2D** (recommendation above); record in
  `docs/modernization/MPRES-P1-presenter.md` with the equivalence-capture idiom.
- **Exit:** baseline + golden capture + benchmark recorded; API chosen.

---

### MPRES-P1 — Swap-chain presenter behind `refresh(surf*)`, flag-gated (High-impact, T3)

Introduce the modern presenter without removing any DDraw yet, so it can be
A/B'd on the NVIDIA box exactly like the MM-P9 toggles.

- ⬜ **MPRES-P1.1** Add a `u6o::client` **`Presenter`** (RAII; `ComPtr` members)
  that owns the DXGI device + swap chain for `hWnd`, a dynamic RGB565 texture,
  and the scale state. Header in a new `src/client/present.{h,cpp}` (modern from
  the start; Doxygen-documented).
- ⬜ **MPRES-P1.2** Implement `Presenter::present(const surf* s)`: upload
  `s->o` (stride `s->d.lPitch`, `dwWidth`×`dwHeight`) into the texture, draw it
  letterboxed to the client rect, `Present`. Compute `dstX/dstY/scale` with the
  **same** formula as `blit_letterbox` and write `blit_offx/blit_offy/blit_scale`
  so input mapping is unchanged.
- ⬜ **MPRES-P1.3** Gate it: `refresh(surf*)` calls the presenter when a new
  switch `g_present_modern` (default **0** initially) is set, else the existing
  cached-DC `BitBlt`. Parse a command-line `modernpresent` in `u6o7.cpp`
  (mirror `diagpresent`/`oldtextdc`). Wire `Presenter` create/resize/destroy
  into `setupddraw` success, `WM_SIZE`/`recreateBackbuffers`, and the WM_QUIT /
  `ddrawshutdown` teardown.
- ⬜ **MPRES-P1.4** Verify (T3): golden present match (P0.2) at 1:1, letterboxed,
  and maximized; mouse-mapping parity (`blit_*` identical → click a known UI
  hotspot at several window sizes); benchmark vs P0.3 (no meaningful
  regression); on NVIDIA, `commitKB` **flat** with `modernpresent` (no
  `IDirectDrawSurface::GetDC` in the present path at all).
- ⬜ **MPRES-P1.5** Flip `g_present_modern` default to **1** after sign-off; keep
  the legacy path reachable via the flag for one cycle.
- **Exit:** modern present is default; pixels + input mapping verified identical;
  no present-path DDraw `GetDC`; perf parity.

---

### MPRES-P2 — Collapse the high-level `refresh()` compose/convert branches (T3)

Route every `function_client.cpp::refresh()` branch through the presenter and
delete the now-dead format-conversion + primary-surface paths.

- ⬜ **MPRES-P2.1** Replace the `dxrefresh` branches (`p16to32`/`p16to16` asm →
  `img(vs,…)` to the DD primary) with `presenter.present(ps)`. The GPU samples
  RGB565, so the 16→32/16→16 converters and the `vs` primary become dead.
- ⬜ **MPRES-P2.2** Replace the `smallwindow` / `windowsizecyclenum` 0/1 and
  fullscreen branches (`refresh(ps2/ps4/psnew1/psnew1b/ps)`) with a single
  `presenter.present(ps)`; the GPU viewport handles the scaling those
  intermediate surfaces did. (Coordinate with **RW-P\***: use `backbufferW/H`
  and the established scale math; do not change viewport sizing semantics.)
- ⬜ **MPRES-P2.3** Delete `dxrefresh`, the primary surface `vs`, and the
  intermediate present surfaces (`ps2`/`ps3`/`ps4`/`psnew1`/`psnew1b`) **once
  proven unreferenced**; retire `DDRAW_display_pixelformat` usage in the present.
- ⬜ **MPRES-P2.4** Verify (T3): golden capture across all former modes; this is
  the riskiest pixel step (it removes asm converters) — capture each former
  branch's output and diff. Benchmark.
- **Exit:** one present path; no DD primary; format conversion is GPU-side;
  inline `_asm` present converters removed (T3 metric down).

---

### MPRES-P3 — Owned framebuffers (replace `newsurf` sysmem alloc) (T1/T3)

Make the work surfaces plain RAII RGB565 buffers so allocation no longer needs
DirectDraw.

- ⬜ **MPRES-P3.1** Introduce a `Surface`/buffer type (or extend `surf`) backed
  by an owned `std::unique_ptr<uint8_t[]>` for sysmem surfaces, exposing the
  **same** `->o` pointer and `d.lPitch`/`dwWidth`/`dwHeight` the blitters use, so
  `img`/`img0`/`imgt*`/`cls`/`getpixel` are byte-for-byte unchanged.
- ⬜ **MPRES-P3.2** Route `newsurf` sysmem allocations (SURF_SYSMEM / SURF_SYSMEM16)
  to the owned buffer; keep DD vidmem/primary only where still needed (until P4).
  Match `lPitch` (incl. any historical alignment) so lighting-stride invariants
  (RW-P2.3) hold.
- ⬜ **MPRES-P3.3** Verify: surface byte-dump equality for a representative blit
  matrix; lighting/stormcloak overlay unchanged; tri-target build.
- **Exit:** sysmem surfaces are owned memory; no DDraw needed to allocate them.

---

### MPRES-P4 — Remove the remaining DirectDraw `Blt`s and the device (T3)

- ⬜ **MPRES-P4.1** Convert the 2-/6-arg `img(d,s[,rect])` and `img0(d,s)` DD
  `Blt`s (and the `cls` videomem fallback) to software/asm copies over `->o`
  (the keyed/colour-key path mirrors `imgt0`). These are the `bltCopy`/`bltKey`
  categories from MM-P9.6 — low per-frame frequency, but they keep DDraw alive.
- ⬜ **MPRES-P4.2** Delete `IDirectDraw*` entirely: `dd`/`dd1`, `setupddraw`'s
  DDraw bits, `ddrawshutdown`'s DDraw bits, `ddraw.h`/`ddraw.lib` from the
  client build. `surf` loses its `LPDIRECTDRAWSURFACE4`.
- ⬜ **MPRES-P4.3** Verify (T3): full golden pixel matrix + benchmark; confirm
  `host`/`both` unaffected; zero new warnings.
- **Exit:** **no DirectDraw in the client.** Inline-asm/blit pixel output
  unchanged; NVIDIA emulation risk surface eliminated.

---

### MPRES-P5 — Cleanup & decommission diagnostics (T0/T1)

- ⬜ **MPRES-P5.1** Remove the MM-P9 diagnostic scaffolding now that the cause is
  cured: the `U6O-DIAG` heartbeat, `bltFill/bltCopy/bltKey` counters, and the
  `diagpresent`/`diagbltskip`/`oldtextdc`/`modernpresent` switches (keep
  whichever the team wants as a short-lived escape hatch, then delete).
- ⬜ **MPRES-P5.2** Update docs: this plan → `docs/plans/done/`; fold the MM-P9
  records; refresh `docs/resizable-window-hotspots.md` present-path rows and the
  modernization master index dashboard (inline `_asm` count, DDraw removed).
- **Exit:** diagnostics gone; docs reflect the modern present; plan filed done.

---

## Verification strategy (per `cpp-modernizer` §Verification, T3)

1. **Golden present capture** (MPRES-P0.2): on-screen pixel dump for a fixed
   `ps->o`, compared legacy-vs-modern at 1:1, letterboxed, and maximized — for
   each former `refresh()` branch.
2. **Mouse-mapping parity:** `blit_offx/offy/blit_scale` identical at multiple
   window sizes; a scripted click maps to the same source tile/UI widget.
3. **Benchmark:** per-present ms + FPS, legacy vs modern; no meaningful
   regression (the GPU present should match or beat the CPU `StretchBlt`).
4. **Leak proof (the point):** on NVIDIA, idle `commitKB` flat with the modern
   present; **zero** `IDirectDrawSurface::GetDC`/`Blt` per frame.
5. **Tri-target build**, zero new warnings, after every phase.

The one negotiable: the *scaling filter* of the on-screen result. Source
(`ps->o`) pixels stay bit-exact; point-sampling reproduces the current
`COLORONCOLOR` present. If a chosen API can't be made bit-exact at every window
size, document the exact intended delta and get sign-off (this is explicitly the
present-path exception, not a rasterizer change).

---

## Coordination (per modernization README §5)

- **RW-P\*** owns viewport sizing / lighting (`backbufferW/H`, `lighting_*`,
  `blit_*` scale math). The presenter must **consume** those, not redefine them;
  reuse the letterbox math in `blit_letterbox` verbatim.
- **MCLI-P\*** owns `myddraw.cpp` + inline asm generally. This plan is the
  present-path slice; sequence the asm-converter removal (P2) and blit
  conversion (P4) with MCLI so they don't collide.
- **MM-P9** (in `docs/plans/done/memory-management/plan-memoryManagement.md`) is
  the point-fix series this plan structurally supersedes; P5 retires its
  diagnostics. MM-P9 verification is complete (idle `commitKB` flat on NVIDIA).
- **DOB-P\*** / **LH-P\*** untouched (no wire buffers, no platform shim changes;
  host doesn't render).

---

## Session handoff

- **2026-06-26 (drafted).** New plan; no code yet. Start at **MPRES-P0.1**.
  Prereq: confirm MM-P9.6 idle `commitKB` is flat on NVIDIA (the present-`GetDC`
  cache fix) so the modern present's win is cleanly attributable. Drive every
  phase through the **`cpp-modernizer` agent** with the T3 golden-pixel +
  benchmark discipline above. Recommended API: **D3D11 + DXGI** (RGB565 texture,
  point-sampled full-screen quad); D2D-on-DXGI is the simpler fallback.

