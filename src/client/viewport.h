#ifndef VIEWPORT_H
#define VIEWPORT_H

// Resizable-window plan (docs/plan-resizableWindow.md): single source of
// truth for the client's back-buffer dimensions, lighting-buffer stride,
// and related sizing constants.
//
// Current state (RW-P2 step 1, 2026-05-20):
//   * The constants below are still **fixed at 1024x768**. The accessors
//     return those constants. Behavior is bit-identical to the legacy
//     hard-coded literals they're replacing.
//   * The point of this header is to give the rest of the renderer a
//     single, named call site to read back-buffer dimensions through.
//     Once every hot path consults these accessors instead of literal
//     `1024`/`768`, the next step (after manual testing) is to flip the
//     accessors from constants to runtime-mutable globals fed by
//     OnClientResized() / RecreateBackbuffers().
//
// Future state (RW-P2 step 2, post-testing):
//   * The static lighting arrays (`ls`, `ls_moon1..4`) become heap
//     allocations sized to the active back-buffer.
//   * The DirectDraw surfaces (`ps`, `ps3`, `ps5`, `psnew1`, `psnew1b`)
//     are released and re-created when the client area changes.
//   * The hard-coded `mov ebp, 786432` in the lighting-compose inline
//     asm at loop_client.cpp:8278 becomes a runtime memory operand.
//
// This header intentionally has no client/host #ifdef gates because the
// sole consumer is client code. It's also intentionally header-only: no
// state is owned here today. When the constants flip to runtime values,
// the storage moves into globals.inc with externs in data_both.h.

namespace u6o { namespace client {

// Maximum back-buffer dimensions the client will ever allocate. Currently
// equal to the legacy 1024x768; sized to be raised when the static
// lighting buffers become heap-allocated. Static back-buffer surfaces
// today live at exactly this size. Declared as an enum (rather than
// `inline constexpr`) so the header compiles cleanly under pre-C++17
// MSVC, which is what this project targets.
enum : int {
    kBackbufferLegacyW = 1024,
    kBackbufferLegacyH = 768
};

// Active back-buffer width in pixels. Game logic and renderer hot paths
// should call this rather than referencing the literal `1024`. Today it
// returns the legacy constant; in the next step it will read a global
// updated by RecreateBackbuffers().
inline int backbufferW() { return kBackbufferLegacyW; }
inline int backbufferH() { return kBackbufferLegacyH; }

// Row stride (in bytes) of the `ls` / `ls_moon*` lighting buffers. These
// are paletted byte buffers, so stride == width. Used by LIGHTnew() and
// the lighting-compose pass.
inline int lightingStride() { return backbufferW(); }

// Total byte count of one lighting buffer. Used for memcpy/ZeroMemory
// over `ls` and `ls_moon*`. Replaces `1024*768` literals and the implicit
// `mov ebp, 786432` in the lighting-compose inline asm.
inline int lightingTotalBytes() { return backbufferW() * backbufferH(); }

// Called from OnClientResized() in the main loop when `windowResize` is
// enabled (RW-P0.4 feature flag). Today this is a no-op stub: it logs the
// requested dimensions in Debug builds and returns. The lighting buffers
// and DirectDraw surfaces are still pinned at the legacy 1024x768. The
// next step (after manual testing) wires this up to:
//   * Release and re-create `ps`, `ps3`, `ps5`, `psnew1`, `psnew1b`.
//   * (Once the lighting arrays are heap-allocated) re-allocate `ls`,
//     `ls_moon1..4` to the new size.
//   * Update the live values returned by backbufferW()/backbufferH().
// Returns true on success; today always returns true since it's a no-op.
inline bool recreateBackbuffers(int /*newW*/, int /*newH*/) {
    // Intentional no-op stub. See header comment.
    return true;
}

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

#endif // VIEWPORT_H

