#ifndef _INC_MYDDRAW
#define _INC_MYDDRAW
#define rgb(r,g,b) (b+((g)<<8)+((r)<<16))
#include <memory>

// MPRES-P4.2: lean surface descriptor — DirectDraw removed.
struct surf {
    DWORD dwWidth, dwHeight;
    long lPitch;
    int bpp;  // bytes per pixel (2 or 4)

    union {
        unsigned long *o;
        unsigned char *o1;
        unsigned short *o2;
    };

    // MPRES-P4.2: cached DIB-section DC for text rendering (replaces DD GetDC).
    // The DIB owns its own temporary pixel store; cachedDIBBits points at it so
    // surf_text_dc_release() can flush GDI output back into `o` before destroy.
    HDC cachedDIBDC;
    HBITMAP cachedDIBBitmap;
    unsigned char *cachedDIBBits;

    // MPRES-P3.1/P4.2: owned framebuffer storage for all non-PRIMARY surfaces.
    std::unique_ptr<unsigned char[]> ownedPixels;

    // Constructor: initialize all fields to zero.
    surf() : dwWidth(0), dwHeight(0), lPitch(0), bpp(0), o(nullptr),
             cachedDIBDC(nullptr), cachedDIBBitmap(nullptr), cachedDIBBits(nullptr), ownedPixels(nullptr) {}
};

#define SURF_VIDMEM 0 //surface in video memory compatible with primary surface
#define SURF_SYSMEM 1 //888 RGB surface in system memory (DirectAccess OK)
//#define SURF_3D 2 //*REDUNDANT
//#define SURF_TEX 4 //*REDUNDANT
//#define SURF_ZBUF 8 //*REDUNDANT
#define SURF_SYSVID 16 //compatible with primary surface in system memory
#define SURF_PRIMARY 32 //primary surface
#define SURF_SYSMEM16 64 //565 RGB surface in system memory (DirectAccess OK)

#include "mytxt.h"

surf *surfstruct();

surf *newsurf(long x, long y, long flags);

bool setupddraw();

void pset(surf *s, long x, long y, DWORD c);

DWORD point(surf *s, long x, long y);

void cls(surf *s, DWORD c);

void img(surf *d, long x, long y, surf *s);

void txtout(surf *s, long x, long y, txt *t);

void txtouts(surf *s, long x, long y, txt *t);

// MM-P9.5: release any cached on-surface text DC held on `s` (see surf::
// cachedTextDC). MUST be called before any IDirectDrawSurface method
// (Blt/Flip/Lock/GetDC) on `s`, because DirectDraw forbids those while a DC is
// held on the surface. No-op when no DC is held, so it is cheap and safe to
// call from event-driven sites (e.g. the name-tag / status-message GetDC paths
// in function_client.cpp) as well as from the per-frame Blt/present wrappers.
void surf_text_dc_release(surf *s);

DWORD fixcol(DWORD c);

DWORD getcol(DWORD c);

extern DWORD txtcol;
extern HFONT txtfnt;
// MPRES-P4.2: acquire the surface's cached DIB-section DC for GDI text/measurement.
// The DIB is recreated from the current `->o` pixels on each acquire so CPU
// blits and GDI text stay coherent without DirectDraw.
HDC surf_text_dc_acquire(surf *s);

void purgesurfaces();

void ddrawshutdown();

void img0(surf *d, long x, long y, surf *s);

surf *loadimage(LPCSTR name);

surf *loadimage(txt * name);

surf *loadimage(LPCSTR name, long flags);

surf *loadimage(txt *name, long flags);

void free(surf *s);

void refresh(surf *s);

void img(surf *d, surf *s);

void img0(surf *d, surf *s);

void img0_0key(surf *s, unsigned short c);

void imgt(surf *d, long x, long y, surf *s);

void imgt0(surf *d, long x, long y, surf *s);

void img75t0(surf *d, long x, long y, surf *s);

void img75t(surf *d, long x, long y, surf *s);

// r999 img to handle resizing and positioning
void img(surf *d, surf *s, int x, int y, int x2, int y2);

// r999
void imguiw(surf *d, int uipaneli, int uiwidgeti, int uistatei, surf *s);

void imguiw(surf *d, int uipaneli, int uiwidgeti, int uistatei);

void imguip(surf *d, int uipaneli);

void imguip(surf *d, int uipaneli, surf *s);


#endif