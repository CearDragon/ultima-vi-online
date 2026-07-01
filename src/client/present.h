// MPRES-P1 — Modern swap-chain presenter facade (Direct3D 11 + DXGI).
//
// Behavior-preserving, flag-gated, DEFAULT-OFF alternative to the legacy
// DirectDraw7 `blit_letterbox` present in myddraw.cpp. The client software
// rasterizer keeps writing RGB565 into `surf::o`; this header exposes a minimal
// free-function facade that uploads that finished framebuffer to a dynamic
// B5G6R5 texture and presents it through a DXGI swap chain, letterboxed with the
// EXACT same math as blit_letterbox (so blit_offx/blit_offy/blit_scale — and thus
// mouse mapping — are unchanged). No wire/.sav/RNG impact; client render only.
//
// See docs/modernization/MPRES-P1-presenter.md and
// docs/plans/in-progress/plan-modernPresenter.md (phase MPRES-P1).
#ifndef U6O_CLIENT_PRESENT_H
#define U6O_CLIENT_PRESENT_H

#ifdef CLIENT

// Forward declaration only — present.cpp pulls the full definition from
// myddraw.h. Callers (myddraw.cpp) already have `struct surf` in scope.
struct surf;

namespace u6o {
namespace client {

/// @brief Present the finished RGB565 framebuffer through a D3D11/DXGI swap chain.
///
/// Lazily creates the D3D11 device + DXGI swap chain on first call (for the
/// global `hWnd`), (re)creates a dynamic `DXGI_FORMAT_B5G6R5_UNORM` texture sized
/// to the source surface, uploads `s->o` honoring `s->lPitch` per row, draws it
/// point-sampled and letterboxed (aspect-preserving, never-upscale, centered with
/// black bars), and Presents with vsync. Publishes `blit_offx`/`blit_offy`/
/// `blit_scale` (and `clientW`/`clientH`) identically to `blit_letterbox`, so
/// WndProc/panel-hittest mouse mapping is unchanged.
///
/// @param s The source surface (its RGB565 bytes are never modified).
/// @return true if the modern path handled the present; false if D3D11 was
///         unavailable or any init/present step failed (caller must then fall
///         back to the legacy DirectDraw present).
/// @note Point-sampling (`D3D11_FILTER_MIN_MAG_MIP_POINT`) reproduces the legacy
///       `COLORONCOLOR` `StretchBlt`. The GPU does the RGB565->display-format
///       conversion with zero CPU conversion. (MPRES-P1)
bool present_modern(const surf* s);

/// @brief Release all D3D11/DXGI objects held by the presenter.
/// @note Safe to call if the presenter was never initialized (no-op). Invoked on
///       the client exit path (ddrawshutdown) before the DirectDraw releases.
///       (MPRES-P1)
void present_modern_shutdown();

} // namespace client
} // namespace u6o

#endif // CLIENT

#endif // U6O_CLIENT_PRESENT_H
