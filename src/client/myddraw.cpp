#include "stdafx.h"
#include "resource.h"
#include <memory>
//#include <d3d.h> *REDUNDANT
#pragma warning(disable: 4018 4244 4731)
#include "myfile.h"
#include "commdlg.h"
// MM-P9 diagnostic (2026-06-25): debug-CRT heap checkpoint for the txtout()
// heartbeat. Lets us read outstanding malloc/new bytes to tell a raw-heap leak
// apart from a DirectX-internal one. Debug build only; harmless if absent.
#ifdef _DEBUG
#include <crtdbg.h>
#endif
// MM-P9 diagnostic (2026-06-25): toolhelp for a per-process thread count. All
// of CreateToolhelp32Snapshot/Thread32First/Next live in kernel32 (already
// linked), so this adds no new link dependency.
#include <tlhelp32.h>
#include <memory>
// r999
#include "define_both.h"
#include "viewport.h" // RW-P2.4: backbufferW()/H() for blit_letterbox sanity check
#include "present.h"  // MPRES-P1: modern D3D11/DXGI present facade (default OFF)


//darklight.cpp extern
extern RECT desktop_rect;
extern bool smallwindow;
extern HWND hWnd;
extern HWND hWnd2;
extern HWND hWnd3;
extern HINSTANCE hInst;

// rrrd
extern HWND hWnd4;
extern unsigned int resxz;
extern unsigned int resyz;
extern unsigned int windowsizecyclenum;
extern unsigned int resxs;
extern unsigned int resys;
extern unsigned int resxn1m;
extern unsigned int resyn1m;
extern double scalexm;
extern double scaleym;
extern unsigned int resxo;
extern unsigned int resyo;

// Letterbox blit transform globals (defined in globals.inc, declared in
// data_both.h). refresh() updates these every frame so WndProc can translate
// raw client-pixel mouse coordinates back into source-surface coordinates.
extern long blit_offx;
extern long blit_offy;
extern double blit_scale;
extern long clientW;
extern long clientH;
extern bool dirtyClientSize;
// (windowResize removed 2026-05-27 — always-on now.)

// Blit the source-surface DC `srcdc` (size srcW x srcH) into window `hWndDst`
// while preserving the source aspect ratio. Letterbox bars are filled black.
// Also publishes the resulting transform via blit_offx/blit_offy/blit_scale
// so mouse input maps back to source coordinates correctly.
static void blit_letterbox(HWND hWndDst, HDC srcdc, long srcW, long srcH) {
    // RW-P2.4: defensive check — the only surfaces we currently letterbox are
    // back-buffer-sized (ps/psnew1b), so srcW/srcH should equal the
    // current viewport.h-published back-buffer size. If they ever diverge
    // (e.g. someone passes a half-res or minimap surface), we want a loud
    // warning in debug builds rather than silently producing the wrong
    // blit_scale. In release builds this is a no-op so there's zero cost on
    // the hot path.
#ifdef _DEBUG
    if (srcW != backbufferW() || srcH != backbufferH()) {
        char dbgbuf[160];
        wsprintfA(dbgbuf,
                  "[u6o] blit_letterbox: srcW/H=%ldx%ld != backbuffer=%dx%d\n",
                  srcW, srcH, backbufferW(), backbufferH());
        OutputDebugStringA(dbgbuf);
    }
#endif
    RECT cr;
    GetClientRect(hWndDst, &cr);
    long cW = cr.right - cr.left;
    long cH = cr.bottom - cr.top;
    // Publish the live client dimensions for OnClientResized / debug logging
    // (RW-P1.3). Use the raw GetClientRect values; clamping below is purely
    // for the StretchBlt math that follows.
    clientW = cW;
    clientH = cH;
    if (cW < 1) cW = 1;
    if (cH < 1) cH = 1;

    double sx = (double) cW / (double) srcW;
    double sy = (double) cH / (double) srcH;
    double s = (sx < sy) ? sx : sy;
    // Never upscale beyond native pixels. When the client area is larger than
    // the source surface (e.g. the window was maximized), keep the game image
    // at 1:1 zoom and let the extra space become black border. This preserves
    // the original art scale instead of stretching it.
    if (s > 1.0) s = 1.0;
    if (s <= 0.0) s = 1.0;

    long dstW = (long) (srcW * s + 0.5);
    long dstH = (long) (srcH * s + 0.5);
    long dstX = (cW - dstW) / 2;
    long dstY = (cH - dstH) / 2;

    HDC winhdc = GetDC(hWndDst);

    // Letterbox bars (black). Only fill the bars that exist.
    HBRUSH black = (HBRUSH) GetStockObject(BLACK_BRUSH);
    if (dstY > 0) {
        RECT r = {0, 0, cW, dstY};
        FillRect(winhdc, &r, black);
    }
    if (dstY + dstH < cH) {
        RECT r = {0, dstY + dstH, cW, cH};
        FillRect(winhdc, &r, black);
    }
    if (dstX > 0) {
        RECT r = {0, dstY, dstX, dstY + dstH};
        FillRect(winhdc, &r, black);
    }
    if (dstX + dstW < cW) {
        RECT r = {dstX + dstW, dstY, cW, dstY + dstH};
        FillRect(winhdc, &r, black);
    }

    if (dstW == srcW && dstH == srcH) {
        BitBlt(winhdc, dstX, dstY, srcW, srcH, srcdc, 0, 0, SRCCOPY);
    } else {
        // MM-P9 fix (2026-06-25): use COLORONCOLOR, NOT HALFTONE, for the
        // per-frame downscale present. SetStretchBltMode(HALFTONE) + StretchBlt
        // leaks GDI/kernel-heap memory on every call (it allocates an internal
        // halftone palette that is not reliably freed). Because that memory is
        // committed by win32k on the process's behalf, it shows up as a steady
        // private/commit climb (~0.4 MB/s at 16 fps here) while the GDI *object*
        // count, USER handles, kernel handles, CRT heap and our surface/txt
        // counters all stay flat — which is exactly the leak signature we saw.
        // This path runs every frame whenever the window is not 1:1 with the
        // back-buffer (the menu often presents 1:1 via the BitBlt branch above,
        // which is why the leak was far faster in-game). COLORONCOLOR allocates
        // nothing per call. Trade-off: downscaled present is point-sampled
        // (sharper/aliased) instead of HALFTONE-smoothed; the back-buffer pixels
        // themselves are unchanged and native (s==1.0) presents still BitBlt.
        SetStretchBltMode(winhdc, COLORONCOLOR);
        StretchBlt(winhdc, dstX, dstY, dstW, dstH, srcdc, 0, 0, srcW, srcH, SRCCOPY);
    }

    ReleaseDC(hWndDst, winhdc);

    // Publish transform for input mapping.
    blit_offx = dstX;
    blit_offy = dstY;
    blit_scale = s;
}


//direct draw surface structures and functions
DWORD txtcol = 0xFFFFFF;
HFONT txtfnt = NULL;

// MM-P9 diagnostic (2026-06-25): live DirectDraw-surface count. ++ in
// surfstruct() (every newsurf), -- in free(surf*). Logged by the 5-second
// heartbeat in txtout() so a memory climb can be attributed to (or cleared of)
// leaked surfaces. Behavior-preserving (a single long).
long g_surf_live = 0;

// MM-P9 diagnostic (2026-06-26): per-frame leak BISECTION toggle. Set from the
// command line in WinMain (see u6o7.cpp): pass "diagpresent1" or "diagpresent2".
//   0 (default) = normal rendering, no behavior change.
//   1 = skip the per-frame present in refresh() (the window GetDC + BitBlt and
//       the back-buffer IDirectDrawSurface::GetDC). Isolates whether the leak
//       lives in the final present path.
//   2 = also skip the per-string IDirectDrawSurface::GetDC text draw in
//       txtout()/txtouts(). Isolates the on-surface text DC churn.
// The "constant rate at all window sizes" symptom rules out blit-AREA scaling
// and points at a fixed-cost-per-frame, driver-dependent allocation; the most
// likely source is repeated IDirectDrawSurface::GetDC/ReleaseDC cycles, which
// some WDDM ddraw7-emulation drivers leak per call regardless of surface size.
// Read the result by watching Task Manager's commit / the U6O-DIAG commitKB:
//   * mode 1 flattens commit       -> leak is the refresh() present.
//   * mode 1 climbs, mode 2 flat   -> leak is the txtout()/txtouts() text DCs.
//   * mode 2 still climbs          -> leak is elsewhere (world-render Blt/img,
//                                     Lock/Unlock, or DirectX audio).
// Default 0 keeps shipped behavior identical; remove once the leak is found.
int g_diag_present_mode = 0;

// MM-P9.5 (2026-06-27): cached on-surface text-DC toggle (gating switch so the
// fix can be A/B'd on real NVIDIA hardware before it ships as the only path).
//   1 (default) = NEW cached-DC path: txtout()/txtouts() acquire the
//                 IDirectDrawSurface DC ONCE (lazily) and reuse it across
//                 consecutive text draws; it is released the next time a
//                 DirectDraw method (Blt/Flip/Lock/GetDC) runs on that surface.
//                 This collapses the per-string GetDC/ReleaseDC churn that
//                 leaks under NVIDIA's legacy DirectDraw emulation.
//   0 = LEGACY path: GetDC/ReleaseDC around every individual text string
//       (exactly the pre-MM-P9.5 behavior). Selected with command-line
//       substring "oldtextdc" (parsed in u6o7.cpp), mirroring how
//       "diagpresent1/2" sets g_diag_present_mode.
// Both paths issue the IDENTICAL GDI call sequence per TextOut (same surface,
// same DC type, same font / text-colour / bk-mode state), so rendered pixels
// are byte-for-byte identical; only the GetDC/ReleaseDC *count* per frame
// differs. Watch the U6O-DIAG `commitKB` heartbeat: with the new path ON the
// idle commit climb should drop sharply vs `oldtextdc`.
int g_text_dc_cache = 1;

// MPRES-P1 (2026-06-29): modern swap-chain present gating switch (A/B-able on
// real hardware via command line, exactly like g_text_dc_cache).
//   1 (default, MPRES-P1.5) = MODERN path: refresh(surf*) calls
//       u6o::client::present_modern(), which uploads ps->o (RGB565) into a
//       dynamic B5G6R5 D3D11 texture and presents it point-sampled + letterboxed
//       through a DXGI swap chain. The letterbox dst rect/scale use the IDENTICAL
//       blit_letterbox math and publish the same blit_offx/blit_offy/blit_scale,
//       so mouse mapping is unchanged. If D3D11 init/present fails, present_modern()
//       returns false and refresh() falls through to the legacy present below — so
//       this can never break rendering.
//   0 = LEGACY path: refresh(surf*) presents via the cached IDirectDrawSurface DC
//       + blit_letterbox BitBlt/StretchBlt (the pre-MPRES shipped behavior).
// MPRES-P1.5 (2026-06-29): flipped the default to 1 after hardware sign-off
// (golden present + mouse-mapping parity + look-text confirmed on NVIDIA). The
// legacy path stays reachable for one cycle via the command-line substring
// "legacypresent" (parsed in u6o7.cpp), which sets this back to 0; "modernpresent"
// is now a redundant no-op kept so existing launch scripts keep working.
int g_present_modern = 1;

// MM-P9.6 diagnostic (2026-06-26): localize the residual ~120 KB/s NVIDIA leak
// that persists with every per-frame IDirectDrawSurface::GetDC suppressed
// (diagpresent mode 2). The only per-frame DirectDraw operations left are Blts,
// in three categories. These cumulative counters are emitted by the txtout()
// heartbeat so a run can attribute the commit climb: for each category compute
// Δcommit / Δcount between two heartbeats = bytes leaked per Blt of that kind.
// Behavior-preserving (three longs ++'d at the existing Blt sites).
long g_blt_fill_n = 0; // cls()            — DDBLT_COLORFILL
long g_blt_copy_n = 0; // img(d,s) / img(d,s,rect) — plain DDBLT copy
long g_blt_key_n = 0;  // img0(d,s)        — DDBLT_KEYSRC (colour-keyed) copy

// MM-P9.6 bisection: skip ONE Blt category to confirm it is the leak — watch the
// heartbeat commitKB go flat. Opt-in via command-line substring "diagbltskip1/2/3"
// (parsed in u6o7.cpp, mirroring diagpresent). Default 0 = normal rendering.
//   1 = skip cls() colour-fill   (screen may show stale pixels — cosmetic only)
//   2 = skip img()/img() copy    (composited surfaces won't draw)
//   3 = skip img0() keyed copy   (transparent sprites won't draw)
// Skipping breaks that category's VISUALS only; the game keeps running so the
// idle commit slope is still measurable.
int g_diag_blt_skip = 0;

// MPRES-P4.2: keep this layout identical to the mirror in myddraw.h.
struct surf {
    DWORD dwWidth, dwHeight;
    long lPitch;
    int bpp;

    union {
        unsigned long *o;
        unsigned char *o1;
        unsigned short *o2;
    };

    HDC cachedDIBDC;
    HBITMAP cachedDIBBitmap;
    std::unique_ptr<unsigned char[]> ownedPixels;

    surf() : dwWidth(0), dwHeight(0), lPitch(0), bpp(0), o(nullptr),
             cachedDIBDC(nullptr), cachedDIBBitmap(nullptr), ownedPixels(nullptr) {}
};

surf *surflist[16384];


// r999
extern surf *uipanelsurf[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelx[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanely[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizey[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern float uipanelscalex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern float uipanelscaley[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelhitenable[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelusedefaultstatedata[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];

extern int uipaneli[UI_PANEL_MAX][UI_PANELWIDGET_MAX];
extern int uipanelcount;
extern int uipanelwidgetcount[UI_PANEL_MAX];
extern int uipanelsidebar, uipanelactionbar1, uipanelactionbar2, uipanelactiontalkbar1, uipanelactiontalkbar2,
        uipanelminimap;


bool setupddraw() {
    // MPRES-P4.2: DirectDraw removed. All surface allocation is now owned memory
    // in a fixed RGB565 (16-bit) / RGB888 (32-bit) layout; no device to init.
    ZeroMemory(&surflist[0], sizeof(surf *) * 16384);
    return TRUE;
}

surf *surfstruct() {
    static surf *ts;
    static long i;
    ts = new surf();
    // MPRES-P4.2: no longer need DDSURFACEDESC2 initialization.
    g_surf_live++; // MM-P9 diagnostic: live surface count
    for (i = 0; i < 16384; i++) {
        if (surflist[i] == NULL) {
            surflist[i] = ts;
            return ts;
        }
    }
    return ts;
}

surf *newsurf(long x, long y, long flags) {
    surf *ts = surfstruct();
    ts->dwWidth = x;
    ts->dwHeight = y;

    // MPRES-P4.2: determine pixel format (bpp = bytes per pixel).
    int bpp = 2;
    if (flags & 1) bpp = 4;      // SURF_SYSMEM = 32-bit
    if (flags & 64) bpp = 2;     // SURF_SYSMEM16 = 16-bit
    ts->bpp = bpp;

    // DWORD-aligned pitch (bytes) to preserve lighting-stride invariant (RW-P2.3).
    const long pitch = (x * bpp + 3L) & ~3L;
    ts->lPitch = pitch;

    // MPRES-P4.2: allocate owned memory for all non-PRIMARY surfaces.
    if (!(flags & 32)) {
        const size_t sz = static_cast<size_t>(pitch) * static_cast<size_t>(y);
        ts->ownedPixels = std::make_unique<unsigned char[]>(sz > 0 ? sz : 1);
        ts->o = reinterpret_cast<unsigned long *>(ts->ownedPixels.get());
    }
    // PRIMARY surface (flags & 32): o remains nullptr (set elsewhere during init).

    return ts;
}

// MM-P9.5 (2026-06-27): cached on-surface text-DC helpers. See surf::cachedTextDC
// in myddraw.h and the g_text_dc_cache toggle above for the full rationale.
//
// surf_text_dc_acquire(): return the surface's cached GDI DC, lazily creating it
// via a single IDirectDrawSurface::GetDC the first time. Mirrors the legacy
// per-string GetDC exactly (same surface, same DC type) so TextOut output is
// pixel-identical; only the number of GetDC calls per frame changes. On GetDC
// failure cachedTextDC stays NULL and we return NULL — the same observable
// outcome as the legacy code, which also ignored GetDC's return and operated on
// whatever HDC came back.
HDC surf_text_dc_acquire(surf *s) {
    // MPRES-P4.2: create a cached DIB-section DC for text rendering.
    // DIBs allow GDI to draw directly into our pixel buffer.
    if (s == NULL || s->o == NULL) return NULL;  // PRIMARY surface (no pixels)
    if (s->cachedDIBDC != NULL) return s->cachedDIBDC;
    
    BITMAPINFOHEADER bih = {};
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = (LONG)s->dwWidth;
    bih.biHeight = -(LONG)s->dwHeight;  // negative = top-down orientation
    bih.biPlanes = 1;
    bih.biBitCount = s->bpp * 8;  // 16 or 32 bpp
    bih.biCompression = BI_RGB;
    
    // For 16-bpp RGB565, set color masks (DIB_BITFIELDS format).
    BITMAPINFO bi = {};
    bi.bmiHeader = bih;
    if (s->bpp == 2) {  // RGB565
        bi.bmiHeader.biCompression = BI_BITFIELDS;
        unsigned int *pMasks = (unsigned int *)&bi.bmiColors[0];
        pMasks[0] = 0xF800;  // R: 5 bits
        pMasks[1] = 0x07E0;  // G: 6 bits
        pMasks[2] = 0x001F;  // B: 5 bits
    }
    
    // Create a device-independent bitmap (DIB section).
    // The returned pointer will reference our pixel buffer directly.
    HDC hdc = CreateCompatibleDC(NULL);
    if (hdc == NULL) return NULL;
    
    void *pBits = s->o;  // Start with our surface's pixel buffer
    HBITMAP hbmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pBits, NULL, 0);
    if (hbmp == NULL) {
        DeleteDC(hdc);
        return NULL;
    }
    
    // Select the DIB into the DC so GDI operations draw into it.
    SelectObject(hdc, hbmp);
    
    // Cache the DIB and DC for reuse across text calls.
    s->cachedDIBBitmap = hbmp;
    s->cachedDIBDC = hdc;
    return hdc;
}

// surf_text_dc_release(): release the cached DIB DC (if any).
// With DIB sections, we don't need to release before Blt/etc operations,
// so this is mainly for cleanup on surface destruction.
void surf_text_dc_release(surf *s) {
    if (s == NULL) return;
    if (s->cachedDIBDC != NULL) {
        DeleteDC(s->cachedDIBDC);
        s->cachedDIBDC = NULL;
    }
    if (s->cachedDIBBitmap != NULL) {
        DeleteObject(s->cachedDIBBitmap);
        s->cachedDIBBitmap = NULL;
    }
}

void pset(surf *s, long x, long y, DWORD c) {
    if (x < 0) return;
    if (y < 0) return;
    if (y >= s->dwHeight) return;
    if (x >= s->dwWidth) return;
    if (s->o == NULL) return;
    s->o[y * s->lPitch / 4 + x] = c;
    return;
}

DWORD point(surf *s, long x, long y) {
    if (x < 0) return 0xFFFFFFFF;
    if (y < 0) return 0xFFFFFFFF;
    if (y >= s->dwHeight) return 0xFFFFFFFF;
    if (x >= s->dwWidth) return 0xFFFFFFFF;
    if (s->o == NULL) return 0;
    return s->o[y * s->lPitch / 4 + x] & 0xFFFFFF;
}

void cls(surf *s, DWORD c) {
    // MM-P9.6: count + optional skip (colour-fill category).
    g_blt_fill_n++;
    if (g_diag_blt_skip == 1) return;
    
    // MPRES-P4.2: all non-PRIMARY surfaces have owned pixel buffers.
    if (s->o == NULL) return;  // PRIMARY surface (no pixel buffer); no-op.
    
    const DWORD w = s->dwWidth;
    const DWORD h = s->dwHeight;
    const long pitch = s->lPitch;
    unsigned char *row = (unsigned char *) s->o;
    if (s->bpp == 4) {
        const unsigned long v32 = (unsigned long) c;
        for (DWORD yy = 0; yy < h; yy++) {
            unsigned long *px = (unsigned long *) row;
            for (DWORD xx = 0; xx < w; xx++) px[xx] = v32;
            row += pitch;
        }
    } else {
        const unsigned short v16 = (unsigned short) c;
        for (DWORD yy = 0; yy < h; yy++) {
            unsigned short *px = (unsigned short *) row;
            for (DWORD xx = 0; xx < w; xx++) px[xx] = v16;
            row += pitch;
        }
    }
}

// rrr refresh(surf* s)
//
// Option A (single-window-mode cleanup, 2026-05-20): the client now has only
// one window (hWnd2 / hWnd) so refresh() collapses to a single
// blit_letterbox call. The previous multi-branch implementation switched
// between hWnd2 / hWnd3 / hWnd4 based on smallwindow + windowsizecyclenum
// and recomputed scalexm/scaleym for mouse mapping. None of that is needed
// any more — blit_letterbox already publishes blit_offx/offy/scale every
// frame, and the WndProc mouse handler maps client coords back through
// those globals.
void refresh(surf *s) {
    // MM-P9.6 + MPRES-P4.2: present via the cached text DC (now DIB-section based).
    if (g_diag_present_mode >= 1) return; // diag: skip present entirely
#ifdef CLIENT
    // MPRES-P1: modern swap-chain present (default OFF via g_present_modern).
    if (g_present_modern) {
        // Modern presenter reads s->o directly; GdiFlush() ensures any pending
        // text rendering is flushed into the pixels.
        GdiFlush();
        if (u6o::client::present_modern(s)) return; // modern path handled it
    }
#endif
    // Legacy GDI present: acquire cached text DC and blit to window.
    HDC ddhdc = surf_text_dc_acquire(s);
    if (ddhdc != NULL) {
        blit_letterbox(hWnd, ddhdc, (long) s->dwWidth, (long) s->dwHeight);
    }
} //refresh end


//ebx/edi/esi are NOT backed up!!

//assembly passing variables


void img(surf *d, long x, long y, surf *s) {
    //static variables
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long asm_copy_vc_extra2bytes;
    static long x2, y2, x3, y3, x4, y4;
    static long sx, sy, dx, dy; //size of source and destination x and y axis
    //surfaces valid?
    if (s == NULL) return;
    if (d == NULL) return;
    //offscreen?
    dx = d->dwWidth;
    if (x >= dx) return;
    dy = d->dwHeight;
    if (y >= dy) return;
    sx = s->dwWidth;
    if (-x >= sx) return;
    sy = s->dwHeight;
    if (-y >= sy) return;
    x2 = x; //starting dest x offset
    x3 = 0; //starting source x offset
    x4 = sx; //pixels onscreen of x axis
    //part of the image is onscreen
    if (x < 0) {
        x4 += x;
        x2 = 0;
        x3 = -x;
    }
    if ((x + sx) > dx) x4 -= x + sx - dx;
    //x is established, now for y
    y2 = y; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = sy; //rows on screen
    if (y < 0) {
        y4 += y;
        y2 = 0;
        y3 = -y;
    }
    if ((y + sy) > dy) y4 -= y + sy - dy;
    asm_copy_vc_bytesx = x4 * 2;
    asm_copy_vc_sourceskip = (long) s->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->lPitch;
    asm_copy_vc_rows = y4;
    if (asm_copy_vc_bytesx & 2) {
        asm_copy_vc_bytesx -= 2;
        asm_copy_vc_extra2bytes = 1;
    } else { asm_copy_vc_extra2bytes = 0; }
    _asm{
            push esi
            push edi
            push ebx
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_copy1:
            mov ebx,esi
            add ebx,edx
            and edx,edx
            jz asm_copy7
            asm_copy0:
            mov eax,[esi]
            add esi,4

            //and eax,4158584798
            //shr eax,1
            //and DWORD PTR [edi],4158584798
            //shr DWORD PTR [edi],1
            //add [edi],eax

            mov [edi],eax


            add edi,4
            cmp esi,ebx
            jne asm_copy0
            cmp asm_copy_vc_extra2bytes,0
            je asm_copy3
            asm_copy7:
            mov ax,[esi]
            add esi,2
            mov [edi],ax
            add edi,2
            asm_copy3:
            add esi,asm_copy_vc_sourceskip
            add edi,asm_copy_vc_destskip
            dec ecx
            jnz asm_copy1
            pop ebx
            pop edi
            pop esi
            }
} //img(...)

//changes an image that would have has a colour key to use 0 for that colour, existing 0 changed to 1 greenscale!
void img0_0key(surf *s, unsigned short c) {
    static unsigned long i;
    static unsigned short c2;
    for (i = 0; i < (s->lPitch / 2 * s->dwHeight); i++) {
        c2 = s->o2[i];
        if (c2 == c) {
            s->o2[i] = 0;
        } else {
            if (c2 == 0) s->o2[i] = 32;
        }
    }
} //img0_0key(...)

void img0(surf *d, long x, long y, surf *s) {
    //static variables
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long asm_copy_vc_extra2bytes;
    static long x2, y2, x3, y3, x4, y4;
    static long sx, sy, dx, dy; //size of source and destination x and y axis
    //surfaces valid?
    if (s == NULL) return;
    if (d == NULL) return;
    //offscreen?
    dx = d->dwWidth;
    if (x >= dx) return;
    dy = d->dwHeight;
    if (y >= dy) return;
    sx = s->dwWidth;
    if (-x >= sx) return;
    sy = s->dwHeight;
    if (-y >= sy) return;
    x2 = x; //starting dest x offset
    x3 = 0; //starting source x offset
    x4 = sx; //pixels onscreen of x axis
    //part of the image is onscreen
    if (x < 0) {
        x4 += x;
        x2 = 0;
        x3 = -x;
    }
    if ((x + sx) > dx) x4 -= x + sx - dx;
    //x is established, now for y
    y2 = y; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = sy; //rows on screen
    if (y < 0) {
        y4 += y;
        y2 = 0;
        y3 = -y;
    }
    if ((y + sy) > dy) y4 -= y + sy - dy;
    asm_copy_vc_bytesx = x4 * 2;
    asm_copy_vc_sourceskip = (long) s->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->lPitch;
    asm_copy_vc_rows = y4;
    if (asm_copy_vc_bytesx & 2) {
        asm_copy_vc_bytesx -= 2;
        asm_copy_vc_extra2bytes = 1;
    } else { asm_copy_vc_extra2bytes = 0; }
    _asm{
            push esi
            push edi
            push ebx
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_copy1:
            mov ebx,esi
            add ebx,edx
            and edx,edx
            jz asm_copy7
            asm_copy0:
            mov eax,[esi]
            and ax,ax
            jz asm_copy3
            mov [edi],ax
            asm_copy3:
            add esi,4
            shr eax,16
            add edi,2
            and ax,ax
            jz asm_copy4
            mov [edi],ax
            asm_copy4:
            add edi,2
            cmp esi,ebx
            jne asm_copy0
            cmp asm_copy_vc_extra2bytes,0
            je asm_copy5
            asm_copy7:
            mov ax,[esi]
            and ax,ax
            jz asm_copy6
            mov [edi],ax
            asm_copy6:
            add esi,2
            add edi,2
            asm_copy5:
            add esi,asm_copy_vc_sourceskip
            add edi,asm_copy_vc_destskip
            dec ecx
            jnz asm_copy1
            pop ebx
            pop edi
            pop esi
            }
} //img0(...)


/*
void img(surf* d,long x,long y,surf* s)
{
if (s==NULL) return;
if (d==NULL) return;
static RECT r1,r2;
r1.top=y;
r1.bottom=y+s->dwHeight;
r1.left=x;
r1.right=x+s->dwWidth;
r2.left=0;
r2.top=0;
r2.bottom=s->dwHeight;
r2.right=s->dwWidth;

if (r1.right>d->dwWidth)
{
r2.right-=r1.right-d->dwWidth;
r1.right=d->dwWidth;
}
if (r1.bottom>d->dwHeight)
{
r2.bottom-=r1.bottom-d->dwHeight;
r1.bottom=d->dwHeight;
}
if (r1.left<0)
{
r2.left-=r1.left;
r1.left=0;
}
if (r1.top<0)
{
r2.top-=r1.top;
r1.top=0;
}
d->s->Blt(&r1,s->s,&r2,DDBLT_WAIT,NULL);
return;
}
*/

void img(surf *d, surf *s) {
    if (s == NULL || d == NULL) return;
    // MPRES-P4.1: text DCs are no longer needed for software copies, but we
    // still release them for surfaces that might still use DD (PRIMARY, etc.).
    surf_text_dc_release(d);
    surf_text_dc_release(s);
    // MM-P9.6: count + optional skip (plain-copy category).
    g_blt_copy_n++;
    if (g_diag_blt_skip == 2) return;

    // MPRES-P4.1: software copy via owned pixel buffer.
    // Both surfaces must have owned buffers for software blit.
    if (d->o == NULL || s->o == NULL) {
        // MPRES-P4.2: DirectDraw removed. Cannot blit if either surface lacks owned pixels.
        return;
    }
    const DWORD sw = s->dwWidth, sh = s->dwHeight;
    const DWORD dw = d->dwWidth, dh = d->dwHeight;
    const int bpp = s->bpp;
    const long sp = s->lPitch, dp = d->lPitch;
    const auto *src = reinterpret_cast<const unsigned char *>(s->o);
    auto *dst = reinterpret_cast<unsigned char *>(d->o);

    if (sw == dw && sh == dh) {
        // Same dimensions: fast row-by-row memcpy.
        const DWORD row_bytes = dw * static_cast<DWORD>(bpp);
        for (DWORD y = 0; y < dh; y++)
            memcpy(dst + y * dp, src + y * sp, row_bytes);
    } else {
        // Different dimensions: nearest-neighbour scale (up or down).
        for (DWORD dy = 0; dy < dh; dy++) {
            const DWORD sy = dy * sh / dh;
            const unsigned char *srow = src + sy * sp;
            unsigned char *drow = dst + dy * dp;
            for (DWORD dx = 0; dx < dw; dx++) {
                const DWORD sx = dx * sw / dw;
                memcpy(drow + dx * bpp, srow + sx * bpp, bpp);
            }
        }
    }
}


// r999 img to handle resizing and positioning — copies full source into dest rect.
void img(surf *d, surf *s, int x, int y, int x2, int y2) {
    // MM-P9.5: release cached text DCs on both surfaces.
    surf_text_dc_release(d);
    surf_text_dc_release(s);
    // MM-P9.6: count + optional skip (plain-copy category).
    g_blt_copy_n++;
    if (g_diag_blt_skip == 2) return;

    // MPRES-P4.1: software copy into dest rect.
    if (d->o == NULL || s->o == NULL) {
        // MPRES-P4.2: DirectDraw removed. Cannot blit if either surface lacks owned pixels.
        return;
    }
    const DWORD sw = s->dwWidth, sh = s->dwHeight;
    const DWORD dw = d->dwWidth, dh = d->dwHeight;
    const int bpp = s->bpp;
    const long sp = s->lPitch, dp = d->lPitch;
    const auto *src = reinterpret_cast<const unsigned char *>(s->o);
    auto *dst = reinterpret_cast<unsigned char *>(d->o);

    // Clip dest rect to dest surface bounds.
    const int cx1 = max(x, 0), cy1 = max(y, 0);
    const int cx2 = min(x2, static_cast<int>(dw));
    const int cy2 = min(y2, static_cast<int>(dh));
    if (cx1 >= cx2 || cy1 >= cy2) return;

    const int dest_rw = x2 - x, dest_rh = y2 - y;
    if (dest_rw == static_cast<int>(sw) && dest_rh == static_cast<int>(sh)) {
        // Same size: row-by-row memcpy with dest-rect clipping.
        const int src_ox = cx1 - x, src_oy = cy1 - y;
        const DWORD row_bytes = static_cast<DWORD>(cx2 - cx1) * static_cast<DWORD>(bpp);
        for (int ry = 0; ry < cy2 - cy1; ry++)
            memcpy(dst + (cy1 + ry) * dp + cx1 * bpp,
                   src + (src_oy + ry) * sp + src_ox * bpp,
                   row_bytes);
    } else {
        // Different size: nearest-neighbour scale into dest rect.
        for (int dy = cy1; dy < cy2; dy++) {
            const int sy = static_cast<int>((dy - y) * static_cast<long long>(sh) / dest_rh);
            if (sy < 0 || sy >= static_cast<int>(sh)) continue;
            unsigned char *drow = dst + dy * dp;
            const unsigned char *srow = src + sy * sp;
            for (int dx = cx1; dx < cx2; dx++) {
                const int sx = static_cast<int>((dx - x) * static_cast<long long>(sw) / dest_rw);
                if (sx < 0 || sx >= static_cast<int>(sw)) continue;
                memcpy(drow + dx * bpp, srow + sx * bpp, bpp);
            }
        }
    }
}

// r999
void imguiw(surf *d, int uipaneli, int uiwidgeti, int uistatei, surf *s) {
    if (uipanelusedefaultstatedata[uipaneli][uiwidgeti][uistatei] == 1)
        img(d, uipanelx[uipaneli][uiwidgeti][UI_STATE_DEF], uipanely[uipaneli][uiwidgeti][UI_STATE_DEF], s);
    else
        img(d, uipanelx[uipaneli][uiwidgeti][uistatei], uipanely[uipaneli][uiwidgeti][uistatei], s);
}

void imguiw(surf *d, int uipaneli, int uiwidgeti, int uistatei) {
    /*
	if (uipanelusedefaultstatedata[uipaneli][uiwidgeti][uistatei] == 1)
		img(d, uipanelx[uipaneli][uiwidgeti][UI_STATE_DEF], uipanely[uipaneli][uiwidgeti][UI_STATE_DEF], uipanelsurf[uipaneli][uiwidgeti][uistatei]);
	else
		img(d, uipanelx[uipaneli][uiwidgeti][uistatei], uipanely[uipaneli][uiwidgeti][uistatei], uipanelsurf[uipaneli][uiwidgeti][uistatei]);
	*/
    imguiw(d, uipaneli, uiwidgeti, uistatei, uipanelsurf[uipaneli][uiwidgeti][uistatei]);
}

void imguip(surf *d, int uipaneli, surf *s) {
    imguiw(d, uipaneli, UI_WIDGET_DEF, UI_STATE_DEF, s);
}

void imguip(surf *d, int uipaneli) {
    imguiw(d, uipaneli, UI_WIDGET_DEF, UI_STATE_DEF);
}


DWORD fixcol(DWORD c) {
    static unsigned char r, g, b;
    r = c & 255;
    g = (c & 0xFF00) >> 8;
    b = (c & 0xFF0000) >> 16;
    return 65536 * r + 256 * g + b;
}


/*void clear_font(HGDIOBJ fnt) {
  DeleteObject(fnt);
  if(!fnt1) {
    fnt1=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt1;
  }
  if(!fnt1naa) {
    fnt1naa=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt1naa;
  }  
  if(!fnt2) {
    fnt2=CreateFont(16,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt2;
  }  
  if(!fnt3) {
    fnt3=CreateFont(8,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Terminal");
    txtfnt=fnt3;
  }
  //fnt4=CreateFont(18,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
  if(!fnt4) {
    fnt4=CreateFont(22,8,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Avatar");
    txtfnt=fnt4;
  }
  //rune
  if(!fnt5) {
    fnt5=CreateFont(22,NULL,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Ultima Runes");
    txtfnt=fnt5;
  }
  //gargish
  if(!fnt6) {
    fnt6=CreateFont(22,0,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"Gargish");
    txtfnt=fnt6;
  }
  //u6o2 (used for startup menu)
  if(!fnt7) {
    fnt7=CreateFont(24,9,0,0,0,NULL,NULL,NULL,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"PerryGothic");
    txtfnt=fnt7;
  }
}*/

// MM-P9 diagnostic (2026-06-25): process-wide probes for the heartbeat. These
// catch leaks that bypass every other counter: thread stacks (threads), kernel
// objects (handles), and driver/DirectX-internal commits (private bytes). All
// are resolved dynamically or via kernel32/toolhelp, so no new link dependency.

// Private (committed) bytes for this process. Resolves GetProcessMemoryInfo at
// runtime (kernel32!K32GetProcessMemoryInfo first, then psapi.dll) so we never
// link psapi.lib. Returns bytes, or 0 if unavailable.
static SIZE_T _diag_private_bytes() {
    // Local mirror of PROCESS_MEMORY_COUNTERS_EX (x86 layout) so we don't need
    // psapi.h (which can drag in a #pragma comment(lib, "psapi.lib")).
    struct _DIAG_PMCEX {
        DWORD  cb;
        DWORD  PageFaultCount;
        SIZE_T PeakWorkingSetSize;
        SIZE_T WorkingSetSize;
        SIZE_T QuotaPeakPagedPoolUsage;
        SIZE_T QuotaPagedPoolUsage;
        SIZE_T QuotaPeakNonPagedPoolUsage;
        SIZE_T QuotaNonPagedPoolUsage;
        SIZE_T PagefileUsage;
        SIZE_T PeakPagefileUsage;
        SIZE_T PrivateUsage;
    };
    typedef BOOL(WINAPI *PGPMI)(HANDLE, void *, DWORD);
    static PGPMI pfn = NULL;
    static bool tried = false;
    if (!tried) {
        tried = true;
        HMODULE hk = GetModuleHandleA("kernel32.dll");
        if (hk) pfn = (PGPMI) GetProcAddress(hk, "K32GetProcessMemoryInfo");
        if (!pfn) {
            HMODULE hp = LoadLibraryA("psapi.dll");
            if (hp) pfn = (PGPMI) GetProcAddress(hp, "GetProcessMemoryInfo");
        }
    }
    if (!pfn) return 0;
    _DIAG_PMCEX pmc;
    ZeroMemory(&pmc, sizeof(pmc));
    pmc.cb = sizeof(pmc);
    if (pfn(GetCurrentProcess(), &pmc, sizeof(pmc))) return pmc.PrivateUsage;
    return 0;
}

// Open kernel handle count for this process. Resolved dynamically because the
// legacy SDK headers here may not declare GetProcessHandleCount.
static DWORD _diag_handle_count() {
    typedef BOOL(WINAPI *PGPHC)(HANDLE, PDWORD);
    static PGPHC pfn = NULL;
    static bool tried = false;
    if (!tried) {
        tried = true;
        HMODULE hk = GetModuleHandleA("kernel32.dll");
        if (hk) pfn = (PGPHC) GetProcAddress(hk, "GetProcessHandleCount");
    }
    if (!pfn) return 0;
    DWORD n = 0;
    if (pfn(GetCurrentProcess(), &n)) return n;
    return 0;
}

// Live thread count for this process (toolhelp snapshot).
static long _diag_thread_count() {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) return -1;
    THREADENTRY32 te;
    ZeroMemory(&te, sizeof(te));
    te.dwSize = sizeof(te);
    DWORD pid = GetCurrentProcessId();
    long n = 0;
    if (Thread32First(snap, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) n++;
        } while (Thread32Next(snap, &te));
    }
    CloseHandle(snap);
    return n;
}

void txtout(surf *s, long x, long y, txt *t)
{
    // MM-P9 diagnostic (2026-06-25): 5-second heartbeat. txtout() is called
    // many times per frame, so this is a convenient always-available hook to
    // sample the resource pools without touching the brace-seam loop fragments.
    // It emits one OutputDebugString line every ~5s reporting the live
    // DirectDraw-surface and txt-object counts PLUS the outstanding debug-CRT
    // heap (malloc/new) bytes/blocks and the process GDI+USER handle counts.
    // Watch it in DebugView / the debugger alongside Task Manager's commit:
    //   * surf_live / txt_live climb  -> DirectDraw-surface / txt leak
    //   * heapKB / heapN climb        -> raw malloc/new leak (find via _CrtSetBreakAlloc)
    //   * gdi / user climb            -> GDI / USER handle leak
    //   * NONE climb but commit does  -> DirectX-internal (dsound/dmusic/ddraw) leak
    // Remove once the leak is identified. Cheap: a GetTickCount compare; the
    // sampling + log only fire every 5s.
    {
        static DWORD _diag_last = 0;
        DWORD _diag_now = GetTickCount();
        if (_diag_now - _diag_last >= 5000) {
            _diag_last = _diag_now;
            long _diag_heap_kb = -1;
            long _diag_heap_n = -1;
#ifdef _DEBUG
            // _NORMAL_BLOCK (index 1) is where malloc/new land in the debug CRT.
            _CrtMemState _diag_ms;
            _CrtMemCheckpoint(&_diag_ms);
            _diag_heap_kb = (long) (_diag_ms.lSizes[1] / 1024);
            _diag_heap_n = (long) _diag_ms.lCounts[1];
#endif
            DWORD _diag_gdi = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
            DWORD _diag_user = GetGuiResources(GetCurrentProcess(), GR_USEROBJECTS);
            // MM-P9 diagnostic: process-wide commit/handles/threads — these catch
            // leaks invisible to every other counter (DirectX-driver memory,
            // kernel handles, thread stacks). commitKB is the ground truth that
            // every line now self-correlates against.
            unsigned long _diag_commit_kb = (unsigned long) (_diag_private_bytes() / 1024);
            DWORD _diag_handles = _diag_handle_count();
            long _diag_threads = _diag_thread_count();
            // MM-P9 diagnostic: cumulative DirectMusic call counts (dmusic.cpp,
            // compiled into both host and client) and DirectSound voice-ring
            // counts (sound.cpp). sound.cpp is CLIENT/both-only — the host has no
            // sound source — so reference g_snd_* only under CLIENT to keep the
            // host link clean; the host never renders, so -1 sentinels are fine.
            extern long g_midi_play_n;
            extern long g_midi_load_n;
#ifdef CLIENT
            extern long g_snd_dup_n;
            extern long g_snd_live;
            long _diag_snd_dup = g_snd_dup_n;
            long _diag_snd_live = g_snd_live;
#else
            long _diag_snd_dup = -1;
            long _diag_snd_live = -1;
#endif
            char _diag[512];
            wsprintfA(_diag,
                      "U6O-DIAG presentMode=%d commitKB=%lu handles=%lu threads=%ld surf=%ld txt=%ld heapKB=%ld heapN=%ld gdi=%lu user=%lu midiPlay=%ld midiLoad=%ld sndDup=%ld sndLive=%ld bltFill=%ld bltCopy=%ld bltKey=%ld bltSkip=%d\n",
                      g_diag_present_mode,
                      _diag_commit_kb, _diag_handles, _diag_threads,
                      g_surf_live, g_txt_live, _diag_heap_kb, _diag_heap_n, _diag_gdi, _diag_user,
                      g_midi_play_n, g_midi_load_n, _diag_snd_dup, _diag_snd_live,
                      g_blt_fill_n, g_blt_copy_n, g_blt_key_n, g_diag_blt_skip);
            OutputDebugStringA(_diag);
        }
    }
    // MM-P9 diagnostic (2026-06-26): mode >= 2 skips the per-string DirectDraw
    // GetDC text draw (after the heartbeat above, so the log keeps flowing).
    // Isolates whether the per-frame txtout() GetDC/ReleaseDC churn is the leak.
    if (g_diag_present_mode >= 2) return;
    // MPRES-P4.2: DirectDraw removed; always use cached DIB-section DC for text rendering.
    HDC pdc = surf_text_dc_acquire(s);
    if (pdc == NULL) return;  // Surface has no pixels (PRIMARY or uninitialized)
    {
        HGDIOBJ old_font = SelectObject(pdc, txtfnt);
        if ((txtcol & 0xFF000000) == 0) SetBkMode(pdc, TRANSPARENT);
        else SetBkMode(pdc, OPAQUE);
        SetTextColor(pdc, fixcol(txtcol));
        TextOut(pdc, x, y, t->d, t->l);
        SelectObject(pdc, old_font);
    }
    return;
}

void txtouts(surf *s, long x, long y, txt *t) //creates a shadow behind the text (8,8,8)
{
    // MM-P9 diagnostic (2026-06-26): mode >= 2 skips the per-string DirectDraw
    // GetDC text draw (see txtout()). Default (0) is unchanged behavior.
    if (g_diag_present_mode >= 2) return;
    // MPRES-P4.2: DirectDraw removed; always use cached DIB-section DC for text rendering.
    HDC pdc = surf_text_dc_acquire(s);
    if (pdc == NULL) return;  // Surface has no pixels (PRIMARY or uninitialized)
    {
        HGDIOBJ old_font = SelectObject(pdc, txtfnt);
        if ((txtcol & 0xFF000000) == 0) SetBkMode(pdc, TRANSPARENT);
        else SetBkMode(pdc, OPAQUE);
        SetTextColor(pdc, 8 + 8 * 256 + 8 * 65536); //8,8,8
        TextOut(pdc, x - 1, y, t->d, t->l);
        TextOut(pdc, x + 1, y, t->d, t->l);
        TextOut(pdc, x, y - 1, t->d, t->l);
        TextOut(pdc, x, y + 1, t->d, t->l);
        SetTextColor(pdc, fixcol(txtcol));
        TextOut(pdc, x, y, t->d, t->l);
        SelectObject(pdc, old_font);
    }
    return;
}


DWORD getcol(DWORD c) {
    static CHOOSECOLOR ccx;
    static unsigned long cccs[32];
    ShowWindow(hWnd, SW_HIDE);
    ccx.lStructSize = sizeof(CHOOSECOLOR);
    ccx.hInstance = NULL;
    ccx.Flags = CC_FULLOPEN | CC_RGBINIT;
    ccx.hwndOwner = NULL;
    ccx.lCustData = NULL;
    ccx.lpCustColors = &cccs[0];
    ccx.lpfnHook = NULL;
    ccx.lpTemplateName = NULL;
    ccx.rgbResult = fixcol(c);
    if (ChooseColor(&ccx) == NULL) ccx.rgbResult = 0;
    ShowWindow(hWnd, SW_SHOW);
    return fixcol(ccx.rgbResult);
}

void purgesurfaces() {
    static long i;
    for (i = 0; i < 16384; i++) {
        if (surflist[i] != NULL) {
            // Use the module's surf cleanup which releases the surface and
            // frees the malloc'd surf struct (free(surf*) is defined below).
            free(surflist[i]);
        }
    }
    return;
}

void ddrawshutdown() {
    // MPRES-P4.2: DirectDraw removed. All surfaces are now RAII with owned buffers.
    // Just tear down modern present resources (if used) and purge the surface list.
#ifdef CLIENT
    u6o::client::present_modern_shutdown();
#endif
    purgesurfaces();
}

/*
void img0(surf* d,long x,long y,surf* s)
{
static RECT r1,r2;
r1.top=y;
r1.bottom=y+s->dwHeight;
r1.left=x;
r1.right=x+s->dwWidth;
r2.left=0;
r2.top=0;
r2.bottom=s->dwHeight;
r2.right=s->dwWidth;

if (r1.right>d->dwWidth)
{
r2.right-=r1.right-d->dwWidth;
r1.right=d->dwWidth;
}
if (r1.bottom>d->dwHeight)
{
r2.bottom-=r1.bottom-d->dwHeight;
r1.bottom=d->dwHeight;
}
if (r1.left<0)
{
r2.left-=r1.left;
r1.left=0;
}
if (r1.top<0)
{
r2.top-=r1.top;
r1.top=0;
}
r1.right--;
r2.right--;
d->s->Blt(&r1,s->s,&r2,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
r1.right++;
r2.right++;
r1.left=r1.right-1;
r2.left=r2.right-1;
d->s->Blt(&r1,s->s,&r2,DDBLT_WAIT|DDBLT_KEYSRC,NULL);
return;
}
*/

void img0(surf *d, surf *s) {
    // MM-P9.5: release cached text DCs before any surface operation.
    surf_text_dc_release(d);
    surf_text_dc_release(s);
    // MM-P9.6: count + optional skip (keyed-copy category).
    g_blt_key_n++;
    if (g_diag_blt_skip == 3) return;

    // MPRES-P4.1: software keyed copy — skip pixels equal to 0 (color key).
    // MPRES-P4.2: DirectDraw removed; a missing owned buffer means no-op.
    if (d->o == NULL || s->o == NULL) {
        return;
    }
    const DWORD w = s->dwWidth, h = s->dwHeight;
    // img0 only operates on same-size surfaces; any mismatch is a no-op now
    // that the DD Blt fallback is gone (no caller relies on it).
    if (w != d->dwWidth || h != d->dwHeight) {
        return;
    }
    const long sp = s->lPitch, dp = d->lPitch;
    const auto *src = reinterpret_cast<const unsigned char *>(s->o);
    auto *dst = reinterpret_cast<unsigned char *>(d->o);

    if (s->bpp == 2) {
        for (DWORD y = 0; y < h; y++) {
            const auto *srow = reinterpret_cast<const unsigned short *>(src + y * sp);
            auto *drow = reinterpret_cast<unsigned short *>(dst + y * dp);
            for (DWORD x = 0; x < w; x++)
                if (srow[x]) drow[x] = srow[x];
        }
    } else {
        for (DWORD y = 0; y < h; y++) {
            const auto *srow = reinterpret_cast<const unsigned long *>(src + y * sp);
            auto *drow = reinterpret_cast<unsigned long *>(dst + y * dp);
            for (DWORD x = 0; x < w; x++)
                if (srow[x]) drow[x] = srow[x];
        }
    }
}

surf *loadimage(LPCSTR name, long flags) {
    static HBITMAP bmh; //handle to loaded bitmap
    static BITMAP bm; //bitmap info buffer
    static long bmx, bmy; //width, height
    static surf *s; //temp surf pointer, for new image
    static HDC bdc, didc; //bitmap device, DIB device
    bmh = (HBITMAP) LoadImage(hInst, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (bmh == NULL) return NULL;
    GetObject(bmh, sizeof(BITMAP), &bm);
    bmx = (DWORD) bm.bmWidth;
    bmy = (DWORD) bm.bmHeight;
    s = newsurf(bmx, bmy, flags); //1=SURF_SYSMEM
    if (s == NULL || s->o == NULL) {
        DeleteObject(bmh);
        return NULL;
    }
    
    // MPRES-P4.2: BitBlt BMP into surface via DIB section (DirectDraw removed).
    bdc = CreateCompatibleDC(NULL);
    HGDIOBJ _old_bmp = SelectObject(bdc, bmh);
    
    // Get the cached DIB DC for the surface (or create one) and BitBlt into it.
    didc = surf_text_dc_acquire(s);
    if (didc != NULL) {
        BitBlt(didc, 0, 0, bmx, bmy, bdc, 0, 0, SRCCOPY);
    }
    
    SelectObject(bdc, _old_bmp);
    DeleteDC(bdc);
    DeleteObject(bmh);
    return s;
}

surf *loadimage(LPCSTR name) {
    return loadimage(name, NULL);
}

surf *loadimage(txt *name) {
    return loadimage(name->d, NULL);
}

surf *loadimage(txt *name, long flags) {
    return loadimage(name->d, flags);
}

void free(surf *s) {
    static long i;
    if (s == NULL) return;
    for (i = 0; i < 16384; i++) {
        if (surflist[i] == s) surflist[i] = NULL;
    }
    // MPRES-P4.2: release any cached DIB DC before releasing the surface.
    surf_text_dc_release(s);
    // Release owned pixel buffer (DirectDraw removed; now just RAII cleanup).
    s->ownedPixels.reset();
    delete s;
    g_surf_live--; // MM-P9 diagnostic
    return;
}

void imgt0(surf *d, long x, long y, surf *s) {
    //static variables
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long asm_copy_vc_extra2bytes;
    static long x2, y2, x3, y3, x4, y4;
    static long sx, sy, dx, dy; //size of source and destination x and y axis
    //surfaces valid?
    if (s == NULL) return;
    if (d == NULL) return;
    //offscreen?
    dx = d->dwWidth;
    if (x >= dx) return;
    dy = d->dwHeight;
    if (y >= dy) return;
    sx = s->dwWidth;
    if (-x >= sx) return;
    sy = s->dwHeight;
    if (-y >= sy) return;
    x2 = x; //starting dest x offset
    x3 = 0; //starting source x offset
    x4 = sx; //pixels onscreen of x axis
    //part of the image is onscreen
    if (x < 0) {
        x4 += x;
        x2 = 0;
        x3 = -x;
    }
    if ((x + sx) > dx) x4 -= x + sx - dx;
    //x is established, now for y
    y2 = y; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = sy; //rows on screen
    if (y < 0) {
        y4 += y;
        y2 = 0;
        y3 = -y;
    }
    if ((y + sy) > dy) y4 -= y + sy - dy;
    asm_copy_vc_bytesx = x4 * 2;
    asm_copy_vc_sourceskip = (long) s->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->lPitch;
    asm_copy_vc_rows = y4;
    if (asm_copy_vc_bytesx & 2) {
        asm_copy_vc_bytesx -= 2;
        asm_copy_vc_extra2bytes = 1;
    } else { asm_copy_vc_extra2bytes = 0; }
    _asm{
            push esi
            push edi
            push ebx
            push ebp
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_copy1:
            mov ebx,esi
            add ebx,edx
            and edx,edx
            jz asm_copy7
            asm_copy0:
            mov eax,[esi]
            mov ebp,[edi]
            add esi,4

            and ax,ax
            jz asm_copy_imgt0_1
            and bp,63454
            and ax,63454
            shr bp,1
            shr ax,1
            add ax,bp
            mov [edi],ax
            asm_copy_imgt0_1:

            shr ebp,16
            shr eax,16
            add edi,2

            and ax,ax
            jz asm_copy_imgt0_2
            and bp,63454
            and ax,63454
            shr bp,1
            shr ax,1
            add ax,bp
            mov [edi],ax
            asm_copy_imgt0_2:

            add edi,2

            cmp esi,ebx
            jne asm_copy0
            cmp asm_copy_vc_extra2bytes,0
            je asm_copy3
            asm_copy7:


            mov ax,[esi]
            mov bp,[edi]
            add esi,2

            and ax,ax
            jz asm_copy_imgt0_3
            and bp,63454
            and ax,63454
            shr bp,1
            shr ax,1
            add ax,bp
            mov [edi],ax
            asm_copy_imgt0_3:

            add edi,2
            asm_copy3:
            add esi,asm_copy_vc_sourceskip
            add edi,asm_copy_vc_destskip
            dec ecx
            jnz asm_copy1
            pop ebp
            pop ebx
            pop edi
            pop esi
            }
} //img(...)


//transparent image function
void imgt(surf *d, long x, long y, surf *s) {
    //static variables
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long asm_copy_vc_extra2bytes;
    static long x2, y2, x3, y3, x4, y4;
    static long sx, sy, dx, dy; //size of source and destination x and y axis
    //surfaces valid?
    if (s == NULL) return;
    if (d == NULL) return;
    //offscreen?
    dx = d->dwWidth;
    if (x >= dx) return;
    dy = d->dwHeight;
    if (y >= dy) return;
    sx = s->dwWidth;
    if (-x >= sx) return;
    sy = s->dwHeight;
    if (-y >= sy) return;
    x2 = x; //starting dest x offset
    x3 = 0; //starting source x offset
    x4 = sx; //pixels onscreen of x axis
    //part of the image is onscreen
    if (x < 0) {
        x4 += x;
        x2 = 0;
        x3 = -x;
    }
    if ((x + sx) > dx) x4 -= x + sx - dx;
    //x is established, now for y
    y2 = y; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = sy; //rows on screen
    if (y < 0) {
        y4 += y;
        y2 = 0;
        y3 = -y;
    }
    if ((y + sy) > dy) y4 -= y + sy - dy;
    asm_copy_vc_bytesx = x4 * 2;
    asm_copy_vc_sourceskip = (long) s->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->lPitch;
    asm_copy_vc_rows = y4;
    if (asm_copy_vc_bytesx & 2) {
        asm_copy_vc_bytesx -= 2;
        asm_copy_vc_extra2bytes = 1;
    } else { asm_copy_vc_extra2bytes = 0; }
    _asm{
            push esi
            push edi
            push ebx
            push ebp
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_copy1:
            mov ebx,esi
            add ebx,edx
            and edx,edx
            jz asm_copy7
            asm_copy0:
            mov eax,[esi]
            mov ebp,[edi]
            add esi,4
            and eax,4158584798
            and ebp,4158584798
            shr eax,1
            shr ebp,1
            add eax,ebp
            mov [edi],eax
            add edi,4
            cmp esi,ebx
            jne asm_copy0
            cmp asm_copy_vc_extra2bytes,0
            je asm_copy3
            asm_copy7:
            mov ax,[esi]
            mov bp,[edi]
            add esi,2
            and ax,63454
            and bp,63454
            shr ax,1
            shr bp,1
            add ax,bp
            mov [edi],ax
            add edi,2
            asm_copy3:
            add esi,asm_copy_vc_sourceskip
            add edi,asm_copy_vc_destskip
            dec ecx
            jnz asm_copy1
            pop ebp
            pop ebx
            pop edi
            pop esi
            }
} //img(...)


//75% transparency!
void img75t0(surf *d, long x, long y, surf *s) {
    //static variables
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long asm_copy_vc_extra2bytes;
    static long x2, y2, x3, y3, x4, y4;
    static long sx, sy, dx, dy; //size of source and destination x and y axis
    //surfaces valid?
    if (s == NULL) return;
    if (d == NULL) return;
    //offscreen?
    dx = d->dwWidth;
    if (x >= dx) return;
    dy = d->dwHeight;
    if (y >= dy) return;
    sx = s->dwWidth;
    if (-x >= sx) return;
    sy = s->dwHeight;
    if (-y >= sy) return;
    x2 = x; //starting dest x offset
    x3 = 0; //starting source x offset
    x4 = sx; //pixels onscreen of x axis
    //part of the image is onscreen
    if (x < 0) {
        x4 += x;
        x2 = 0;
        x3 = -x;
    }
    if ((x + sx) > dx) x4 -= x + sx - dx;
    //x is established, now for y
    y2 = y; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = sy; //rows on screen
    if (y < 0) {
        y4 += y;
        y2 = 0;
        y3 = -y;
    }
    if ((y + sy) > dy) y4 -= y + sy - dy;
    asm_copy_vc_bytesx = x4 * 2;
    asm_copy_vc_sourceskip = (long) s->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->lPitch;
    asm_copy_vc_rows = y4;
    if (asm_copy_vc_bytesx & 2) {
        asm_copy_vc_bytesx -= 2;
        asm_copy_vc_extra2bytes = 1;
    } else { asm_copy_vc_extra2bytes = 0; }
    _asm{
            push esi
            push edi
            push ebx
            push ebp
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_copy1:
            mov ebx,esi
            add ebx,edx
            and edx,edx
            jz asm_copy7
            asm_copy0:
            mov eax,[esi]
            mov ebp,[edi]
            add esi,4

            and ax,ax
            jz asm_copy_imgt0_1
            and bp,59292
            and ax,63454
            shr bp,2 //bp=25% of dest
            shr ax,1 //ax=50% of source
            add bp,ax //bp=25%(bp)+50%(ax)=75%
            and ax,63454
            shr ax,1 //ax=25% of source
            add bp,ax //bp=75%(bp)+25%(ax)=100%
            mov [edi],bp
            asm_copy_imgt0_1:

            shr ebp,16
            shr eax,16
            add edi,2

            and ax,ax
            jz asm_copy_imgt0_2

            and bp,59292
            and ax,63454
            shr bp,2 //25%
            shr ax,1 //50%
            add bp,ax
            and ax,63454
            shr ax,1 //50%
            add bp,ax
            mov [edi],bp

            asm_copy_imgt0_2:

            add edi,2

            cmp esi,ebx
            jne asm_copy0
            cmp asm_copy_vc_extra2bytes,0
            je asm_copy3
            asm_copy7:


            mov ax,[esi]
            mov bp,[edi]
            add esi,2

            and ax,ax
            jz asm_copy_imgt0_3

            and bp,59292
            and ax,63454
            shr bp,2 //25%
            shr ax,1 //50%
            add bp,ax
            and ax,63454
            shr ax,1 //50%
            add bp,ax
            mov [edi],bp

            asm_copy_imgt0_3:

            add edi,2
            asm_copy3:
            add esi,asm_copy_vc_sourceskip
            add edi,asm_copy_vc_destskip
            dec ecx
            jnz asm_copy1
            pop ebp
            pop ebx
            pop edi
            pop esi
            }
} //img(...)


//75% transparency!
void img75t(surf *d, long x, long y, surf *s) {
    //static variables
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long asm_copy_vc_extra2bytes;
    static long x2, y2, x3, y3, x4, y4;
    static long sx, sy, dx, dy; //size of source and destination x and y axis
    //surfaces valid?
    if (s == NULL) return;
    if (d == NULL) return;
    //offscreen?
    dx = d->dwWidth;
    if (x >= dx) return;
    dy = d->dwHeight;
    if (y >= dy) return;
    sx = s->dwWidth;
    if (-x >= sx) return;
    sy = s->dwHeight;
    if (-y >= sy) return;
    x2 = x; //starting dest x offset
    x3 = 0; //starting source x offset
    x4 = sx; //pixels onscreen of x axis
    //part of the image is onscreen
    if (x < 0) {
        x4 += x;
        x2 = 0;
        x3 = -x;
    }
    if ((x + sx) > dx) x4 -= x + sx - dx;
    //x is established, now for y
    y2 = y; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = sy; //rows on screen
    if (y < 0) {
        y4 += y;
        y2 = 0;
        y3 = -y;
    }
    if ((y + sy) > dy) y4 -= y + sy - dy;
    asm_copy_vc_bytesx = x4 * 2;
    asm_copy_vc_sourceskip = (long) s->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->lPitch;
    asm_copy_vc_rows = y4;
    if (asm_copy_vc_bytesx & 2) {
        asm_copy_vc_bytesx -= 2;
        asm_copy_vc_extra2bytes = 1;
    } else { asm_copy_vc_extra2bytes = 0; }
    _asm{
            push esi
            push edi
            push ebx
            push ebp
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_copy1:
            mov ebx,esi
            add ebx,edx
            and edx,edx
            jz asm_copy7
            asm_copy0:

            mov eax,[esi]
            mov ebp,[edi]
            and eax,4158584798 //11110111110111101111011111011110
            and ebp,3885819804 //11100111100111001110011110011100
            shr eax,1 //eax=50% of source
            shr ebp,2 //ebp=25% of dest
            add esi,4
            add ebp,eax //ebp=25%(ebp)+50%(eax)=75%
            and eax,4158584798 //11110111110111101111011111011110
            shr eax,1 //eax=25% of source
            add ebp,eax //bp=75%(bp)+25%(ax)=100%
            mov [edi],ebp
            add edi,4

            cmp esi,ebx
            jne asm_copy0
            cmp asm_copy_vc_extra2bytes,0
            je asm_copy3
            asm_copy7:

            mov ax,[esi]
            mov bp,[edi]
            add esi,2
            and bp,59292
            and ax,63454
            shr bp,2 //25%
            shr ax,1 //50%
            add bp,ax
            and ax,63454
            shr ax,1 //50%
            add bp,ax
            mov [edi],bp
            add edi,2

            asm_copy3:
            add esi,asm_copy_vc_sourceskip
            add edi,asm_copy_vc_destskip
            dec ecx
            jnz asm_copy1
            pop ebp
            pop ebx
            pop edi
            pop esi
            }
} //img(...)

