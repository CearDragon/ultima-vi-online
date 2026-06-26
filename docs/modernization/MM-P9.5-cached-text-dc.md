# MM-P9.5 — Cached on-surface text DC (NVIDIA legacy-DirectDraw GetDC leak fix)

**Risk tier:** T3-adjacent (per-frame client render path; pixel-exact required).
Behavior-PRESERVING: rendered pixels identical, only the per-frame
`IDirectDrawSurface4::GetDC`/`ReleaseDC` *count* changes. No wire/save/RNG
impact (client-render only).

## Problem (measured ground truth — not re-investigated)

On Windows 11 + NVIDIA RTX 5090 (driver 32.0.15.9579) the client commits
~0.4 MB/s of kernel/GDI-heap memory while idle; on Intel Iris Xe it does not.
The `txtout()` 5-second heartbeat (`U6O-DIAG ...`) showed ONLY `commitKB`
climbing — `gdi`/`user`/`surf`/`txt`/`heapKB`/handles/threads all flat. That is
the textbook NVIDIA legacy-ddraw `IDirectDrawSurface::GetDC`/`ReleaseDC`
emulation leak: each GetDC/ReleaseDC cycle leaks a small fixed amount regardless
of surface size.

Bisection with the existing `g_diag_present_mode` toggle:
- mode 1 (skip present) → ~463 KB/s idle climb
- mode 2 (also skip per-string text GetDC) → ~120 KB/s

⇒ the per-string text GetDC in `txtout()`/`txtouts()` is ~343 KB/s (74% of the
leak) and is the target of THIS task. The ~120 KB/s residual in mode 2 is a
separate non-GetDC source — OUT OF SCOPE.

## Approach (primary, chosen): per-surface cached text DC with lazy release

Keep rendering text with the SAME `TextOut` onto the SAME DirectDraw surface DC
(byte-for-byte identical regardless of font antialiasing/ClearType), but acquire
the DC ONCE and reuse it across consecutive text draws instead of
GetDC/ReleaseDC per string.

- New field `HDC surf::cachedTextDC` (NULL when none held). `surf` is a
  client-only runtime wrapper, never byte-blitted to disk/wire (only `sizeof`'d
  for `malloc`/`ZeroMemory`), so appending a field is safe.
- `surf_text_dc_acquire(surf*)` — lazily `GetDC` once, return the cached DC.
- `surf_text_dc_release(surf*)` — `ReleaseDC` + NULL it; **no-op** if none held.
- `txtout()`/`txtouts()` acquire and DO NOT release per string.
- DirectDraw forbids `Blt`/`Flip`/`Lock`/`GetDC` while a DC is held, so
  `surf_text_dc_release()` is called at the top of EVERY DirectDraw method-call
  site (see exhaustive list below). Net GetDC count drops from "per text string"
  to "per text run between DD Blts" — a handful per frame.

### Pixel-identity proof

Both paths execute the IDENTICAL GDI sequence per `TextOut` (same surface, same
DC type, same `SelectObject(txtfnt)`, `SetTextColor`, and bk-mode), so output is
byte-for-byte identical. The only state that a *persistent* DC could carry
between draws is **bk-mode** (set to `TRANSPARENT` only when `txtcol`'s top byte
is 0). On the cached path we therefore restore the legacy fresh-DC default
(`OPAQUE`) explicitly in the `else` branch — a no-op on a fresh DC, identical
pixels on a reused one. No other GDI attribute that affects `TextOut` is ever
mutated (bk-color, text-align, clip, mapping mode all untouched → stay at the
DD-GetDC defaults in both paths).

The asm `img()`/`imgt()`/`img0`-style blitters write the persisted Lock pointer
`->o` as RAW MEMORY and call NO IDirectDrawSurface method, so they do NOT need a
release (and releasing around them would re-introduce per-call churn). Only
actual `IDirectDrawSurface` method calls (Blt/Flip/Lock/GetDC/ReleaseDC) require
the DC released.

## Gating flag (A/B on real NVIDIA hardware)

`int g_text_dc_cache` in `myddraw.cpp`, default **1 (new path ON)**.
Command-line substring `oldtextdc` (parsed in `u6o7.cpp`, mirroring
`diagpresent1/2`) sets it to 0 → legacy per-string GetDC/ReleaseDC path, kept
byte-identical so it is a faithful A/B baseline. `g_diag_present_mode` is
unchanged and still fully working.

## Files changed

| File | Change |
|------|--------|
| `src/client/myddraw.h` | Added `HDC cachedTextDC` to `struct surf` (mirror); declared `surf_text_dc_release()`. |
| `src/client/myddraw.cpp` | Synced `struct surf` field; added `int g_text_dc_cache = 1`; added `surf_text_dc_acquire()`/`surf_text_dc_release()`; inserted release at every DD method site; new cached-vs-legacy path in `txtout()`/`txtouts()`. |
| `src/common/u6o7.cpp` | Parse `oldtextdc` → `g_text_dc_cache = 0` (under `#ifdef CLIENT`). |
| `src/client/function_client.cpp` | Release cached text DC on `ps` before the three event-driven `ps->s->GetDC` sites (`STATUSMESSadd` ×2, `STATUSMESSwrapline`) — correctness: those GetDCs would fail `DDERR_DCALREADYCREATED` if a cached DC were held. |

## Exhaustive `surf_text_dc_release()` insertion sites

Found via `grep` of the whole `src/client` + `src/common` tree for
`->(Blt|Flip|Lock|GetDC|ReleaseDC)(`. (No `Flip` anywhere; loop fragments call
the wrappers below, not raw DD methods — confirmed by grep returning nothing in
`src/client/loop/`.)

**myddraw.cpp**
- `cls()` — `Blt` colour-fill → release `s`.
- `refresh()` — its own `GetDC` present → release `s` (before the diag
  early-return, so the surface ends every frame DC-free).
- `img(surf* d, surf* s)` — `Blt` → release `d` **and** `s`.
- `img(surf* d, surf* s, int,int,int,int)` — `Blt` → release `d` **and** `s`.
- `img0(surf* d, surf* s)` — keyed `Blt` → release `d` **and** `s`.
- `loadimage()` — `GetDC` on a fresh surface → release `s` (no-op; kept for
  exhaustiveness/discipline).
- `free(surf*)` — before `->Release()` → release `s`. Covers
  `purgesurfaces()` and `recreateBackbuffers()`'s `free(ps)`/`free(ps3)`.

**function_client.cpp** (event-driven, not per-frame, but DD `GetDC` on `ps`):
- `STATUSMESSadd(txt*)`, `STATUSMESSadd(const char*)`, `STATUSMESSwrapline()`
  — release `ps` before `ps->s->GetDC(...)`.

**Not requiring release (verified):**
- Raw-memory asm blitters (`img(d,x,y,s)`, `imgt`, `imgt0`, `img75t`,
  `img75t0`, `img0(d,x,y,s)`, `pset`, `point`) — write `->o` only, no DD method.
- `setupddraw()` primary-surface `GetDC`/`GetPixelFormat`/`Release` and
  `newsurf()` `CreateSurface`/`Lock` — operate on brand-new surfaces whose
  `cachedTextDC` is `ZeroMemory`'d to NULL.

## Build status

`client`, `both`, `host` all build & link clean (only the pre-existing
`C4731` inline-asm `ebp` warnings in `loop_client_part_world_render.cpp`; zero
new warnings).

## A/B test instructions

Run the client with the `txtout()` heartbeat visible in DebugView and watch the
`U6O-DIAG ... commitKB=...` line:

- **New path (default, ON):** launch normally (no extra flag). `commitKB` idle
  climb should drop sharply vs the legacy path.
- **Legacy path (baseline):** add command-line substring **`oldtextdc`**.
  `commitKB` should climb at the old ~343 KB/s-attributable rate.
- Optional cross-check: `diagpresent2` still flattens the text-GetDC component
  entirely (unchanged diagnostic).

## Residual risk for visual verification

- Confirm on-screen text (UI, chat, name tags, status log, option info) looks
  IDENTICAL with the new path vs `oldtextdc` — same antialiasing, same shadow,
  same opaque/transparent backgrounds. The change is engineered to be
  self-evidently pixel-identical (same `TextOut`s, same order, same DC type;
  bk-mode explicitly normalized on the persistent DC), so only a quick visual
  confirmation is needed.
- The ~120 KB/s mode-2 residual leak is a separate, non-GetDC source and is NOT
  addressed here.

