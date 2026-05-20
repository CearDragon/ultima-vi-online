#ifndef VIEWPORT_H
#define VIEWPORT_H

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

// Upper clamp on the active back-buffer size. Sized to fit common
// modern monitors (1920x1080, 1920x1200, 4K downscaled to 1080p).
// Bounding the upper end keeps memory cost predictable and stops a
// pathologically large client window from triggering allocation
// failures mid-game. Going past these dims simply caps the active
// viewport; the rest of the window stays as letterbox bars.
enum : int {
    kBackbufferMaxW = 1920,
    kBackbufferMaxH = 1200
};

// Active back-buffer dimensions. Bodies live in viewport.cpp; the
// values are read every frame on hot paths so they're declared
// extern (no inlining of a constant return).
int backbufferW();
int backbufferH();

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

// Internal: update the active back-buffer dims globals after a
// successful surface recreation. Called only from
// `recreateBackbuffers`'s implementation in function_client.cpp.
void set_active_backbuffer_dims(int w, int h);

}} // namespace u6o::client

// Unqualified shims so existing C-style call sites (and inline asm
// preludes) can use the names without a `u6o::client::` prefix.
inline int backbufferW()        { return u6o::client::backbufferW(); }
inline int backbufferH()        { return u6o::client::backbufferH(); }
inline int lightingStride()     { return u6o::client::lightingStride(); }
inline int lightingTotalBytes() { return u6o::client::lightingTotalBytes(); }
inline bool recreateBackbuffers(int newW, int newH) {
    return u6o::client::recreateBackbuffers(newW, newH);
}
inline bool lighting_alloc(int w, int h) { return u6o::client::lighting_alloc(w, h); }
inline void lighting_free() { u6o::client::lighting_free(); }

#endif // VIEWPORT_H

