#ifndef VIEWPORT_H
#define VIEWPORT_H

// RW-P4.2: Dynamic 2D Array wrapper mimicking raw 2D arrays but with dynamic striding.
template <typename T>
struct Dynamic2DArray {
    T* data;
    int stride;

    struct Row {
        T* row_data;
        T& operator[](int col) {
            return row_data[col];
        }
        const T& operator[](int col) const {
            return row_data[col];
        }
    };

    Row operator[](int row) {
        return Row{ data + row * stride };
    }
    const Row operator[](int row) const {
        return Row{ data + row * stride };
    }

    operator bool() const { return data != nullptr; }
};

// Resizable-window plan (docs/plan-resizableWindow.md): single source of
// truth for the client's back-buffer dimensions, lighting-buffer stride,
// and related sizing constants.
//
// State as of RW-P2.2 (2026-05-20):
//   * `backbufferW()`/`backbufferH()` are now runtime-mutable: they
//     return the active back-buffer dimensions, which start at the
//     legacy 1024x768 floor and grow up to (kBackbufferMaxW,
//     kBackbufferMaxH) when `recreateBackbuffers(newW, newH)` is
//     called from the dirtyClientSize handler.
//   * `recreateBackbuffers` releases and re-creates `ps` (and `ps3`/
//     `ps5` when present), re-allocates `ls`/`ls_moon1..4` via
//     `lighting_alloc`, and patches the FRAME pointers (`vf`, `fs`)
//     whose `graphic` field referenced the old `ps`. Implementation
//     lives in function_client.cpp where the FRAME globals are
//     visible.
//   * Pitch-coupled hot paths in `loop_client.cpp` (world tile
//     rasterizer at lines 6989/7134, stormcloak lighting at 8061ff)
//     read the row stride from `lightingStride()` so they follow the
//     active dims. Intro/title and scrlog literals are intentionally
//     left at hard-coded `*1024` for now (out-of-scope per user
//     direction).

namespace u6o { namespace client {

// Lower clamp on the active back-buffer size — the legacy 1024x768
// dimensions that every renderer hot path was originally written
// against. The active dims never go below this; if the client window
// is smaller, blit_letterbox handles it via downscaling/letterbox.
enum : int {
    kBackbufferLegacyW = 1024,
    kBackbufferLegacyH = 768
};

// Upper clamp on the active back-buffer size. Sized to fit native 4K
// monitors (3840x2160) plus a little headroom for ultrawide / vertical
// stacks. Bounding the upper end keeps memory cost predictable and
// stops a pathologically large client window from triggering
// allocation failures mid-game. Going past these dims simply caps the
// active viewport; the rest of the window stays as letterbox bars.
//
// RW-P4.10 (2026-05-22): raised from 1920x1200 to 3840x2400 so that on
// 1440p / 4K monitors the back buffer fills the entire client area
// instead of leaving black letterbox bars on the sides. The dragged-
// panel clamp in loop_client.cpp keys off backbufferW/H, so growing
// the back buffer is sufficient to let users park UI panels in what
// used to be the letterbox bars (outside the legacy 1024x768 game
// view). Memory cost at the new cap: ~16 MB per 16bpp surface
// (ps, ps5), ~33 MB for the 32bpp helper (ps3, only on non-16bpp
// displays), and ~46 MB across the five lighting buffers. Total ~110
// MB worst case, which is a non-issue on any machine that can drive
// a 4K display.
enum : int {
    kBackbufferMaxW = 3840,
    kBackbufferMaxH = 2400
};

// Draggable-UI hide sentinel constants (RW-P4.9, 2026-05-22).
//
// The legacy code hides a UI panel by adding 2048 to `offset_x` (or
// 1536 to `offset_y`) and detects "hidden" via `offset_x >= 1024` /
// `offset_y >= 768`. Those thresholds also acted as the per-frame
// snap-back guard for the always-on inventory panels (see
// loop_client.cpp:10227), which made it impossible to drag a panel
// past x=1024 / y=768 — every frame the guard would interpret the
// dragged position as the hide-sentinel and subtract 2048.
//
// With `windowResize=true` the back buffer can grow up to
// kBackbufferMaxW x kBackbufferMaxH, so the legacy thresholds sit
// inside the user-visible drag area. These constants move the
// thresholds (and the matching deltas) safely past the max
// back-buffer dimensions, so any drag inside the window stays in the
// "visible" range and only an explicit hide call moves the panel
// into the "hidden" range.
//
// Invariants:
//   kPanelHideThresholdX > kBackbufferMaxW
//   kPanelHideDeltaX     >= kPanelHideThresholdX   (so adding the
//       delta to ANY visible offset lands in hidden range)
//   Same on the Y axis.
//
// At the legacy 1024x768 client size the in-game behavior is
// unchanged — `offset_x += kPanelHideDeltaX` still pushes a panel
// well off the back-buffer's right edge, and the per-frame guard
// only fires for panels that were actually hidden via the sentinel
// rather than dragged by the user.
enum : int {
    kPanelHideThresholdX = 4096,
    kPanelHideDeltaX     = 4096,
    kPanelHideThresholdY = 4096,
    kPanelHideDeltaY     = 4096
};

// RW-P4.11 (2026-05-26): hard cap on the world-view tile dimensions.
//
// The server-side per-player object buffer `sobj_bufsize[96][72]` (see
// data_both.h) is fixed at 96x72 tiles, positioned with the player
// near (32, 24) inside the buffer (see loop_client.inc:3950 — the
// buffer is reset to `tpx-32, tpy-24` whenever the player drifts past
// its extents). The world-render loops in loop_client.cpp iterate
// `x in [0..viewTilesX()+1]` and read `tplayer->sobj_bufsize[bufx][bufy]`
// where `bufx = tpx + x - 1 - sobj_bufoffx`. With sobj_bufoffx == tpx-32
// the safe range is `bufx in [31..bufx+viewTilesX()]`, so we need
// `viewTilesX() + 31 < 96`, i.e. `viewTilesX() <= 63`. Same arithmetic
// on Y with sobj_bufoffy == tpy-24 gives `viewTilesY() <= 47`.
//
// Before this cap, raising kBackbufferMaxW/H past ~96*32 / 72*32 let
// the user resize the window large enough that viewTilesX/Y exceeded
// these bounds. The per-frame visibility loop then walked past the
// end of `sobj_bufsize` and eventually hit unmapped memory, producing
// an intermittent C0000005 read crash while walking around the map
// (loop_client.cpp:6825).
//
// The world view stops growing past these caps; back-buffer columns/
// rows past `viewTilesX()*32 / viewTilesY()*32` are cleared to black
// each frame (see clear-vis loop in loop_client.cpp).
enum : int {
    kViewportTilesXMax = 63,
    kViewportTilesYMax = 47
};

#ifdef CLIENT
// Active back-buffer dimensions. Bodies live in viewport.cpp; the
// values are read every frame on hot paths so they're declared
// extern (no inlining of a constant return).
int backbufferW();
int backbufferH();

// RW-P4.1: Dynamic view tiles and pixel calculations
int viewTilesX();
int viewTilesY();
int viewPixelW();
int viewPixelH();
int viewOffsetX();
int viewOffsetY();

// Row stride (in pixels) of the `ls`/`ls_moon*` lighting buffers AND
// the `ps` DirectDraw back-buffer (16-bpp pixels). Both are
// re-allocated together by `recreateBackbuffers` so they stay in
// lock-step; the renderer assumes pitch == width for the whole
// back-buffer family.
int lightingStride();

// Total byte count of one lighting buffer == active_W * active_H.
// Used for memcpy/ZeroMemory over `ls`/`ls_moon*` and as the loop
// count in the lighting-compose and 16->32 pixel-format inline-asm
// loops.
int lightingTotalBytes();

// Resize the back-buffer family to (newW, newH) clamped to
// [kBackbufferLegacy*, kBackbufferMax*]. Releases and re-creates the
// `ps`/`ps3`/`ps5` DirectDraw surfaces, re-allocates the lighting
// buffers, patches FRAME pointers (vf, fs) that referenced the old
// `ps`, and clears the new surface to black so unrendered regions
// don't show stale pixels. Idempotent: returns immediately if the
// active dims already match.
bool recreateBackbuffers(int newW, int newH);

// Allocate / free the heap-resident lighting buffers (RW-P2.1).
// `lighting_alloc` is idempotent: same dims → reuse, different dims
// → free and re-malloc. Both implemented in viewport.cpp.
bool lighting_alloc(int w, int h);
void lighting_free();

// Allocate / free the heap-resident visibility buffers (RW-P4.2).
bool visibility_alloc(int w, int h);
void visibility_free();

// Internal: update the active back-buffer dims globals after a
// successful surface recreation. Called only from
// `recreateBackbuffers`s implementation in function_client.cpp.
void set_active_backbuffer_dims(int w, int h);
#else
// Host fallbacks so u6oh doesn't require viewport.cpp
inline int backbufferW() { return 1024; }
inline int backbufferH() { return 768; }
inline int viewTilesX() { return 32; }
inline int viewTilesY() { return 24; }
inline int viewPixelW() { return 1024; }
inline int viewPixelH() { return 768; }
inline int viewOffsetX() { return 0; }
inline int viewOffsetY() { return 0; }
inline int lightingStride() { return 1024; }
inline int lightingTotalBytes() { return 1024 * 768; }
inline bool recreateBackbuffers(int, int) { return true; }
inline bool lighting_alloc(int, int) { return true; }
inline void lighting_free() {}
inline bool visibility_alloc(int, int) { return true; }
inline void visibility_free() {}
inline void set_active_backbuffer_dims(int, int) {}
#endif

}} // namespace u6o::client

// Unqualified shims so existing C-style call sites (and inline asm
// preludes) can use the names without a `u6o::client::` prefix.
inline int backbufferW()        { return u6o::client::backbufferW(); }
inline int backbufferH()        { return u6o::client::backbufferH(); }
inline int viewTilesX()         { return u6o::client::viewTilesX(); }
inline int viewTilesY()         { return u6o::client::viewTilesY(); }
inline int viewPixelW()         { return u6o::client::viewPixelW(); }
inline int viewPixelH()         { return u6o::client::viewPixelH(); }
inline int viewOffsetX()        { return u6o::client::viewOffsetX(); }
inline int viewOffsetY()        { return u6o::client::viewOffsetY(); }
inline int lightingStride()     { return u6o::client::lightingStride(); }
inline int lightingTotalBytes() { return u6o::client::lightingTotalBytes(); }
inline bool recreateBackbuffers(int newW, int newH) {
    return u6o::client::recreateBackbuffers(newW, newH);
}
inline bool lighting_alloc(int w, int h) { return u6o::client::lighting_alloc(w, h); }
inline void lighting_free() { u6o::client::lighting_free(); }
inline bool visibility_alloc(int w, int h) { return u6o::client::visibility_alloc(w, h); }
inline void visibility_free() { u6o::client::visibility_free(); }

// Unqualified aliases for the panel-hide constants (RW-P4.9) and the
// back-buffer dim caps (RW-P4.10). Call sites in loop_client.cpp use
// these without a namespace prefix.
enum : int {
    kBackbufferLegacyW   = u6o::client::kBackbufferLegacyW,
    kBackbufferLegacyH   = u6o::client::kBackbufferLegacyH,
    kBackbufferMaxW      = u6o::client::kBackbufferMaxW,
    kBackbufferMaxH      = u6o::client::kBackbufferMaxH,
    kPanelHideThresholdX = u6o::client::kPanelHideThresholdX,
    kPanelHideDeltaX     = u6o::client::kPanelHideDeltaX,
    kPanelHideThresholdY = u6o::client::kPanelHideThresholdY,
    kPanelHideDeltaY     = u6o::client::kPanelHideDeltaY
};

#endif // VIEWPORT_H

