// viewport.cpp — RW-P2.1 implementation of lighting_alloc/lighting_free.
//
// The lighting buffers `ls`, `ls_moon1..ls_moon4` were previously fixed-size
// arrays declared in src/common/globals.inc:
//   unsigned char ls[1024*768];
//   unsigned char ls_moon1[1024*768]; (etc.)
// They are now `unsigned char*` pointers (still defined in globals.inc) that
// this file allocates at client startup via `lighting_alloc(w, h)`. All
// existing usage sites either already work with a pointer (`ls[idx]`,
// `memcpy(ls,...)`, `ls_moonN[idx]=...`) or were updated alongside this
// change (`&ls`/`sizeof(ls)` callers).

#include "stdafx.h"
#include <stdlib.h> // malloc/free
#include <string.h> // memset

// Pull in the externs for ls/ls_moon* and the Windows-only OutputDebugStringA
// pieces. We don't include data_client.h directly to avoid dragging in the
// full client header chain; the externs we need are tiny. Linkage is C++
// (matches the declarations in data_client.h).
extern unsigned char* ls;
extern unsigned char* ls_moon1;
extern unsigned char* ls_moon2;
extern unsigned char* ls_moon3;
extern unsigned char* ls_moon4;

#include "viewport.h"

namespace u6o { namespace client {

namespace {
    int g_lighting_w = 0;
    int g_lighting_h = 0;

    // RW-P2.2: active back-buffer dimensions. Initialized to the legacy
    // 1024x768 floor so every site that calls backbufferW()/H() before
    // the first recreateBackbuffers() (i.e. at startup, before
    // setup_client.inc creates `ps`) still gets sane values that match
    // the just-created surface. recreateBackbuffers() updates these
    // atomically with the surface re-allocation.
    int g_active_w = kBackbufferLegacyW;
    int g_active_h = kBackbufferLegacyH;

    void free_one(unsigned char*& p) {
        if (p) { free(p); p = nullptr; }
    }
}

int backbufferW()        { return g_active_w; }
int backbufferH()        { return g_active_h; }
int lightingStride()     { return g_active_w; }
int lightingTotalBytes() { return g_active_w * g_active_h; }

// Setter is internal — only function_client.cpp's recreateBackbuffers()
// implementation calls it, after the surfaces have been re-allocated
// successfully. Exposed via a non-namespaced extern "C++" friend
// function so we don't have to plumb a full namespaced header.
void set_active_backbuffer_dims(int w, int h) {
    g_active_w = w;
    g_active_h = h;
}

void lighting_free() {
    free_one(ls);
    free_one(ls_moon1);
    free_one(ls_moon2);
    free_one(ls_moon3);
    free_one(ls_moon4);
    g_lighting_w = 0;
    g_lighting_h = 0;
}

bool lighting_alloc(int w, int h) {
    if (w <= 0 || h <= 0) return false;
    if (g_lighting_w == w && g_lighting_h == h && ls && ls_moon1 && ls_moon2 && ls_moon3 && ls_moon4) {
        return true; // already sized correctly
    }
    lighting_free();

    const size_t bytes = (size_t)w * (size_t)h;
    ls       = (unsigned char*)malloc(bytes);
    ls_moon1 = (unsigned char*)malloc(bytes);
    ls_moon2 = (unsigned char*)malloc(bytes);
    ls_moon3 = (unsigned char*)malloc(bytes);
    ls_moon4 = (unsigned char*)malloc(bytes);
    if (!ls || !ls_moon1 || !ls_moon2 || !ls_moon3 || !ls_moon4) {
        lighting_free();
        return false;
    }
    // Match the BSS-zero behavior of the previous static arrays. The
    // moonN buffers are overwritten by setup_client.inc before first use,
    // but ls is read by getsound() / lightshow before the first frame
    // populates it — zero-init keeps that path well-defined.
    memset(ls,       0, bytes);
    memset(ls_moon1, 0, bytes);
    memset(ls_moon2, 0, bytes);
    memset(ls_moon3, 0, bytes);
    memset(ls_moon4, 0, bytes);

    g_lighting_w = w;
    g_lighting_h = h;
    return true;
}

}} // namespace u6o::client

