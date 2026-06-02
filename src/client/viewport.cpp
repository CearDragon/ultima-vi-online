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
#include "viewport.h"

// Pull in the externs for ls/ls_moon* and the Windows-only OutputDebugStringA
// pieces. We don't include data_client.h directly to avoid dragging in the
// full client header chain; the externs we need are tiny. Linkage is C++
// (matches the declarations in data_client.h).
extern unsigned char *ls;
extern unsigned char *ls_moon1;
extern unsigned char *ls_moon2;
extern unsigned char *ls_moon3;
extern unsigned char *ls_moon4;

// RW-P4.2: Dynamic 2D Visibility array externs
extern Dynamic2DArray<unsigned char> vis;
extern Dynamic2DArray<unsigned char> vis_window;
extern Dynamic2DArray<unsigned char> vis_chair;
extern Dynamic2DArray<unsigned char> vischeck;
extern Dynamic2DArray<unsigned char> vis_bed;
extern Dynamic2DArray<unsigned char> vis_slime;
extern Dynamic2DArray<unsigned char> nonvis;

extern int uipanelsidebar;
extern int uipanelsizex[20][11][6]; // UI_PANEL_MAX x UI_PANELWIDGET_MAX x UI_WIDGETSTATE_MAX

// (windowResize extern removed 2026-05-27 — always-on now.)

namespace u6o {
    namespace client {
        namespace {
            int g_lighting_w = 0;
            int g_lighting_h = 0;

            // RW-P2.3: row stride (in PIXELS) of the ls/ls_moon* buffers and the
            // composited `ps` surface. This is the surface's actual pixel pitch
            // (ps->d.lPitch / 2), which DirectDraw may pad ABOVE the world width
            // (backbufferW()) at widths that aren't aligned to its internal
            // granularity. Keeping the lighting buffers and the linear-walk
            // compose passes on the surface's real pitch — rather than newW — is
            // what stops the lit overlay from skewing diagonally (top-right →
            // bottom-left) at intermediate window sizes. Seeded to the legacy
            // 1024 floor; recreateBackbuffers()/setup overwrite it via
            // set_lighting_stride(ps->d.lPitch / 2) before lighting_alloc().
            int g_lighting_stride = kBackbufferLegacyW;

            // RW-P2.2: active back-buffer dimensions. Initialized to the legacy
            // 1024x768 floor so every site that calls backbufferW()/H() before
            // the first recreateBackbuffers() (i.e. at startup, before
            // setup_client.inc creates `ps`) still gets sane values that match
            // the just-created surface. recreateBackbuffers() updates these
            // atomically with the surface re-allocation.
            int g_active_w = kBackbufferLegacyW;
            int g_active_h = kBackbufferLegacyH;

            void free_one(unsigned char *&p) {
                if (p) {
                    free(p);
                    p = nullptr;
                }
            }

            inline int sidePanelW() {
                return (uipanelsidebar >= 0) ? uipanelsizex[uipanelsidebar][0][0] : 260;
            }

            inline int bottomPanelH() {
                return 0;
            }
        }


        int backbufferW() { return g_active_w; }
        int backbufferH() { return g_active_h; }
        // RW-P2.3: lightingStride() is the surface's PIXEL pitch (lPitch/2), not
        // the world width. See g_lighting_stride for why these can differ.
        int lightingStride() { return g_lighting_stride; }
        int lightingTotalBytes() { return g_lighting_stride * g_active_h; }

        int viewTilesX() {
            // (windowResize early-return removed 2026-05-27. The legacy 32-tile
            // floor is now expressed through the same g_active_w / 32 path; the
            // active back buffer starts at kBackbufferLegacyW so the legacy
            // 32-tile width is the natural minimum.)
            // RW-P4.8 (2026-05-22): sidebar is no longer anchored to the right
            // edge of the window (see ui_panels_apply.cpp). The world view now
            // fills the full back-buffer width; the legacy sidebar (still at
            // its 1024-relative x) draws on top of the world via the FRAME
            // display loop. Previously we subtracted sidePanelW() to leave the
            // sidebar an exclusive strip on the right edge.
            //
            // RW-P4.11 (2026-05-26): clamp to kViewportTilesXMax so the world
            // render loops can't walk past the end of the fixed-size
            // `sobj_bufsize[96][72]` per-player buffer. See the constant's
            // definition in viewport.h for the derivation.
            int tiles = g_active_w / 32;
            if (tiles > kViewportTilesXMax) tiles = kViewportTilesXMax;
            return tiles;
        }

        int viewTilesY() {
            // (windowResize early-return removed 2026-05-27 — see viewTilesX.)
            // RW-P4.11 (2026-05-26): clamp to kViewportTilesYMax. See note
            // above and the constant's definition in viewport.h.
            int tiles = (g_active_h - bottomPanelH()) / 32;
            if (tiles > kViewportTilesYMax) tiles = kViewportTilesYMax;
            return tiles;
        }

        int viewPixelW() {
            return viewTilesX() * 32;
        }

        int viewPixelH() {
            return viewTilesY() * 32;
        }

        int viewOffsetX() {
            return 0;
        }

        int viewOffsetY() {
            return 0;
        }

        // Setter is internal — only function_client.cpp's recreateBackbuffers()
        // implementation calls it, after the surfaces have been re-allocated
        // successfully. Exposed via a non-namespaced extern "C++" friend
        // function so we don't have to plumb a full namespaced header.
        void set_active_backbuffer_dims(int w, int h) {
            g_active_w = w;
            g_active_h = h;
        }

        // RW-P2.3: publish the active `ps` surface's pixel pitch so the lighting
        // buffers and every linear-walk compose pass (lightshow0 asm, moon
        // memcpy, stormcloak, ps_fakebuffer) advance in lockstep with `ps->o`.
        // Called from recreateBackbuffers() and setup_client.inc with
        // ps->d.lPitch / 2 BEFORE lighting_alloc(). Must be > 0; ignored
        // otherwise so a bad surface report can't zero the stride.
        void set_lighting_stride(int pixelStride) {
            if (pixelStride > 0) g_lighting_stride = pixelStride;
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
            // RW-P2.3: allocate at the active surface's PIXEL pitch
            // (g_lighting_stride), not the world width `w`. DirectDraw pads the
            // ps row stride above newW*2 bytes at non-aligned widths; if ls/
            // ls_moon* used `w` as their stride while the compose passes walk
            // ps at lPitch, the lit overlay drifts one (lPitch - w*2) per row →
            // the diagonal skew. set_lighting_stride() seeds g_lighting_stride
            // from ps->d.lPitch/2 before we get here. `w` is still validated
            // above but no longer drives the row stride.
            const int stride = (g_lighting_stride > 0) ? g_lighting_stride : w;
            if (g_lighting_w == stride && g_lighting_h == h && ls && ls_moon1 && ls_moon2 && ls_moon3 && ls_moon4) {
                return true; // already sized correctly
            }
            lighting_free();

            const size_t bytes = (size_t) stride * (size_t) h;
            ls = (unsigned char *) malloc(bytes);
            ls_moon1 = (unsigned char *) malloc(bytes);
            ls_moon2 = (unsigned char *) malloc(bytes);
            ls_moon3 = (unsigned char *) malloc(bytes);
            ls_moon4 = (unsigned char *) malloc(bytes);
            if (!ls || !ls_moon1 || !ls_moon2 || !ls_moon3 || !ls_moon4) {
                lighting_free();
                return false;
            }
            // Match the BSS-zero behavior of the previous static arrays. The
            // moonN buffers are overwritten by setup_client.inc before first use,
            // but ls is read by getsound() / lightshow before the first frame
            // populates it — zero-init keeps that path well-defined.
            memset(ls, 0, bytes);
            memset(ls_moon1, 0, bytes);
            memset(ls_moon2, 0, bytes);
            memset(ls_moon3, 0, bytes);
            memset(ls_moon4, 0, bytes);

            // Store the allocated stride (not the world width) as the cache key
            // so the idempotency check above matches what lightingStride()
            // returns.
            g_lighting_w = stride;
            g_lighting_h = h;
            return true;
        }

        namespace {
            int g_visibility_w = 0;
            int g_visibility_h = 0;

            void free_visibility_array(Dynamic2DArray<unsigned char> &arr) {
                if (arr.data) {
                    free(arr.data);
                    arr.data = nullptr;
                }
                arr.stride = 0;
            }
        }

        void visibility_free() {
            free_visibility_array(vis);
            free_visibility_array(vis_window);
            free_visibility_array(vis_chair);
            free_visibility_array(vis_bed);
            free_visibility_array(vis_slime);
            free_visibility_array(vischeck);
            free_visibility_array(nonvis);
            g_visibility_w = 0;
            g_visibility_h = 0;
        }

        bool visibility_alloc(int w, int h) {
            if (w <= 0 || h <= 0) return false;
            if (g_visibility_w == w && g_visibility_h == h && vis && vis_window && vis_chair && vis_bed && vis_slime &&
                vischeck && nonvis) {
                return true;
            }
            visibility_free();

            int tilesX = w / 32;
            int tilesY = h / 32;

            int padX = tilesX + 4;
            int padY = tilesY + 4;

            size_t padSize = (size_t) padX * padY;
            size_t viewSize = (size_t) tilesX * tilesY;

            vis.data = (unsigned char *) malloc(padSize);
            vis_window.data = (unsigned char *) malloc(padSize);
            vis_chair.data = (unsigned char *) malloc(padSize);
            vis_bed.data = (unsigned char *) malloc(padSize);
            vis_slime.data = (unsigned char *) malloc(padSize);
            vischeck.data = (unsigned char *) malloc(viewSize);
            nonvis.data = (unsigned char *) malloc(viewSize);

            if (!vis.data || !vis_window.data || !vis_chair.data || !vis_bed.data || !vis_slime.data || !vischeck.data
                || !nonvis.data) {
                visibility_free();
                return false;
            }

            vis.stride = padY;
            vis_window.stride = padY;
            vis_chair.stride = padY;
            vis_bed.stride = padY;
            vis_slime.stride = padY;
            vischeck.stride = tilesY;
            nonvis.stride = tilesY;

            memset(vis.data, 0, padSize);
            memset(vis_window.data, 0, padSize);
            memset(vis_chair.data, 0, padSize);
            memset(vis_bed.data, 0, padSize);
            memset(vis_slime.data, 0, padSize);
            memset(vischeck.data, 0, viewSize);
            memset(nonvis.data, 0, viewSize);

            g_visibility_w = w;
            g_visibility_h = h;
            return true;
        }
    }
} // namespace u6o::client