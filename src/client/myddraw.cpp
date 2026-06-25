#include "stdafx.h"
#include "resource.h"
#include <ddraw.h>
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
// r999
#include "define_both.h"
#include "viewport.h" // RW-P2.4: backbufferW()/H() for blit_letterbox sanity check


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
    // back-buffer-sized (ps/psnew1/psnew1b), so srcW/srcH should equal the
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
        SetStretchBltMode(winhdc, HALFTONE);
        SetBrushOrgEx(winhdc, 0, 0, NULL);
        StretchBlt(winhdc, dstX, dstY, dstW, dstH, srcdc, 0, 0, srcW, srcH, SRCCOPY);
    }

    ReleaseDC(hWndDst, winhdc);

    // Publish transform for input mapping.
    blit_offx = dstX;
    blit_offy = dstY;
    blit_scale = s;
}


//direct draw surface structures and functions
IDirectDraw *dd1 = NULL;
IDirectDraw4 *dd = NULL;
DWORD txtcol = 0xFFFFFF;
HFONT txtfnt = NULL;

// MM-P9 diagnostic (2026-06-25): live DirectDraw-surface count. ++ in
// surfstruct() (every newsurf), -- in free(surf*). Logged by the 5-second
// heartbeat in txtout() so a memory climb can be attributed to (or cleared of)
// leaked surfaces. Behavior-preserving (a single long).
long g_surf_live = 0;

struct surf {
    DDSURFACEDESC2 d;
    LPDIRECTDRAWSURFACE4 s;

    union {
        unsigned long *o;
        unsigned char *o1;
        unsigned short *o2;
    };

    //IDirect3DTexture2* t; //only valid if SURF_TEX flag is used *REDUNDANT
};

extern surf *vs;

surf *surflist[16384];

DDPIXELFORMAT DDRAW_display_pixelformat;


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
    DirectDrawCreate(NULL, &dd1, NULL);
    if (FAILED(dd1->SetCooperativeLevel(hWnd, DDSCL_NORMAL))) return FALSE;
    if (dd1 == NULL) return FALSE;
    dd1->QueryInterface(IID_IDirectDraw4, (void **) &dd);
    if (dd == NULL) return FALSE;
    dd->Initialize(NULL);
    if (FAILED(dd->SetCooperativeLevel(hWnd, DDSCL_NORMAL | DDSCL_NOWINDOWCHANGES))) return FALSE;
    //if (FAILED(dd->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE|DDSCL_NOWINDOWCHANGES|DDSCL_FULLSCREEN))) return FALSE;


    static surf *ts;
    ts = (surf *) malloc(sizeof(surf));
    ZeroMemory(ts, sizeof(surf));
    ts->d.dwSize = sizeof(DDSURFACEDESC2);
    ts->d.dwFlags = DDSD_CAPS;
    ts->d.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    if (dd->CreateSurface(&ts->d, &ts->s, NULL) != DD_OK) {
        MessageBox(NULL, "CreateSurface failed: primary", "Ultima 6 Online", MB_OK);
        exit(1);
    }
    ZeroMemory(&DDRAW_display_pixelformat, sizeof(DDRAW_display_pixelformat));
    DDRAW_display_pixelformat.dwSize = sizeof(DDRAW_display_pixelformat);
    ts->s->GetPixelFormat(&DDRAW_display_pixelformat);
    //exit(DDRAW_display_pixelformat.dwGBitMask);
    ts->s->Release();
    // ts was allocated only to query the primary surface pixel format; free it.
    free((void *) ts);
    //static long i;
    ZeroMemory(&surflist[0], sizeof(surf *) * 16384);
    return TRUE;
}

surf *surfstruct() {
    static surf *ts;
    static long i;
    ts = (surf *) malloc(sizeof(surf));
    ZeroMemory(ts, sizeof(surf));
    ts->d.dwSize = sizeof(DDSURFACEDESC2);
    g_surf_live++; // MM-P9 diagnostic: live DirectDraw-surface count
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
    if (flags & 32) {
        ts->d.dwFlags = DDSD_CAPS;
        ts->d.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
        goto gotpixelformat;
    }
    ts->d.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS | DDSD_PIXELFORMAT;
    ts->d.dwWidth = x;
    ts->d.dwHeight = y;
    ts->d.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; //default
    if ((flags & 1) || (flags & 64)) ts->d.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    if (flags & 16) ts->d.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    //if (flags&2) ts->d.ddsCaps.dwCaps+=DDSCAPS_3DDEVICE; 
    if (flags & 64) {
        ts->d.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        ts->d.ddpfPixelFormat.dwFlags = DDPF_RGB;
        ts->d.ddpfPixelFormat.dwRGBBitCount = 16;
        ts->d.ddpfPixelFormat.dwRBitMask = 63488;
        ts->d.ddpfPixelFormat.dwGBitMask = 2016;
        ts->d.ddpfPixelFormat.dwBBitMask = 31;
        goto gotpixelformat;
    }
    if (flags & 1) {
        ts->d.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        ts->d.ddpfPixelFormat.dwFlags = DDPF_RGB;
        ts->d.ddpfPixelFormat.dwRGBBitCount = 32;
        ts->d.ddpfPixelFormat.dwRBitMask = 0xFF0000;
        ts->d.ddpfPixelFormat.dwGBitMask = 0x00FF00;
        ts->d.ddpfPixelFormat.dwBBitMask = 0x0000FF;
        goto gotpixelformat;
    }
    ts->d.ddpfPixelFormat = DDRAW_display_pixelformat;
gotpixelformat:
    //if (flags&4) ts->d.ddsCaps.dwCaps=DDSCAPS_TEXTURE;
    /*
if (flags&8) {
ts->d.ddsCaps.dwCaps=DDSCAPS_ZBUFFER;
if (flags&1) ts->d.ddsCaps.dwCaps+=DDSCAPS_SYSTEMMEMORY;
ts->d.ddpfPixelFormat.dwFlags=DDPF_ZBUFFER;
ts->d.ddpfPixelFormat.dwRGBBitCount=32;
ts->d.ddpfPixelFormat.dwRBitMask=0x0;
ts->d.ddpfPixelFormat.dwGBitMask=0xFFFFFFFF;
ts->d.ddpfPixelFormat.dwBBitMask=0x0;
}
*/
    if (dd->CreateSurface(&ts->d, &ts->s, NULL) != DD_OK) {
        if (ts->d.ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) {
            ts->d.ddsCaps.dwCaps ^= DDSCAPS_VIDEOMEMORY;
            ts->d.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
            MessageBox(NULL, "CreateSurface failed: VIDEOMEMORY", "Ultima 6 Online", MB_OK);
            exit(1);
            if (dd->CreateSurface(&ts->d, &ts->s, NULL) == DD_OK) goto ns_sysmem;
        }
        MessageBox(NULL, "CreateSurface failed", "Ultima 6 Online", MB_OK);
        exit(1);
    }
ns_sysmem:

    if ((flags & 1) || (flags & 64) || (flags & 32)) {
    ddgetlock:
        if (DD_OK != ts->s->Lock(NULL, &ts->d, DDLOCK_WAIT, NULL)) goto ddgetlock;
        ts->o = (unsigned long *) ts->d.lpSurface;
        ts->s->Unlock(NULL);
    }
    //if (flags&4) { *REDUNDANT
    //ts->s->QueryInterface(IID_IDirect3DTexture2,(void**)&ts->t);
    //}
    static DDCOLORKEY cc;
    cc.dwColorSpaceHighValue = 0;
    cc.dwColorSpaceLowValue = 0;
    ts->s->SetColorKey(DDCKEY_SRCBLT, &cc);
    return ts;
}

void pset(surf *s, long x, long y, DWORD c) {
    if (x < 0) return;
    if (y < 0) return;
    if (y >= s->d.dwHeight) return;
    if (x >= s->d.dwWidth) return;
    if (s->o == NULL) return;
    s->o[y * s->d.lPitch / 4 + x] = c;
    return;
}

DWORD point(surf *s, long x, long y) {
    if (x < 0) return 0xFFFFFFFF;
    if (y < 0) return 0xFFFFFFFF;
    if (y >= s->d.dwHeight) return 0xFFFFFFFF;
    if (x >= s->d.dwWidth) return 0xFFFFFFFF;
    if (s->o == NULL) return 0;
    return s->o[y * s->d.lPitch / 4 + x] & 0xFFFFFF;
}

void cls(surf *s, DWORD c) {
    static DDBLTFX b;
    b.dwSize = sizeof(DDBLTFX);
    b.dwFillColor = c;
    s->s->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &b);
    return;
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
    HDC ddhdc;
    s->s->GetDC(&ddhdc);
    blit_letterbox(hWnd, ddhdc, (long) s->d.dwWidth, (long) s->d.dwHeight);
    s->s->ReleaseDC(ddhdc);
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
    dx = d->d.dwWidth;
    if (x >= dx) return;
    dy = d->d.dwHeight;
    if (y >= dy) return;
    sx = s->d.dwWidth;
    if (-x >= sx) return;
    sy = s->d.dwHeight;
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
    asm_copy_vc_sourceskip = (long) s->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->d.lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->d.lPitch;
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
    for (i = 0; i < (s->d.lPitch / 2 * s->d.dwHeight); i++) {
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
    dx = d->d.dwWidth;
    if (x >= dx) return;
    dy = d->d.dwHeight;
    if (y >= dy) return;
    sx = s->d.dwWidth;
    if (-x >= sx) return;
    sy = s->d.dwHeight;
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
    asm_copy_vc_sourceskip = (long) s->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->d.lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->d.lPitch;
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
r1.bottom=y+s->d.dwHeight;
r1.left=x;
r1.right=x+s->d.dwWidth;
r2.left=0;
r2.top=0;
r2.bottom=s->d.dwHeight;
r2.right=s->d.dwWidth;

if (r1.right>d->d.dwWidth)
{
r2.right-=r1.right-d->d.dwWidth;
r1.right=d->d.dwWidth;
}
if (r1.bottom>d->d.dwHeight)
{
r2.bottom-=r1.bottom-d->d.dwHeight;
r1.bottom=d->d.dwHeight;
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
    if (s == NULL) return;
    if (d == NULL) return;
    d->s->Blt(NULL, s->s, NULL, DDBLT_WAIT, NULL);
}


// r999 img to handle resizing and positioning
void img(surf *d, surf *s, int x, int y, int x2, int y2) {
    RECT drect;
    drect.left = x;
    drect.right = x2;
    drect.top = y;
    drect.bottom = y2;
    d->s->Blt(&drect, s->s, NULL, DDBLT_WAIT, NULL);
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
            // MM-P9 diagnostic: cumulative DirectMusic call counts (defined in
            // dmusic.cpp) so the leak's firing rate is visible and the audio-leak
            // fixes can be confirmed.
            extern long g_midi_play_n;
            extern long g_midi_load_n;
            char _diag[256];
            wsprintfA(_diag,
                      "U6O-DIAG surf_live=%ld txt_live=%ld heapKB=%ld heapN=%ld gdi=%lu user=%lu midiPlay=%ld midiLoad=%ld\n",
                      g_surf_live, g_txt_live, _diag_heap_kb, _diag_heap_n, _diag_gdi, _diag_user,
                      g_midi_play_n, g_midi_load_n);
            OutputDebugStringA(_diag);
        }
    }
    HDC pdc;
    s->s->GetDC(&pdc);
    {
        HGDIOBJ old_font = SelectObject(pdc, txtfnt);
        if ((txtcol & 0xFF000000) == 0) SetBkMode(pdc, TRANSPARENT);
        SetTextColor(pdc, fixcol(txtcol));
        TextOut(pdc, x, y, t->d, t->l);
        SelectObject(pdc, old_font);
    }
    s->s->ReleaseDC(pdc);
    return;
}

void txtouts(surf *s, long x, long y, txt *t) //creates a shadow behind the text (8,8,8)
{
    HDC pdc;
    s->s->GetDC(&pdc);
    {
        HGDIOBJ old_font = SelectObject(pdc, txtfnt);
        if ((txtcol & 0xFF000000) == 0) SetBkMode(pdc, TRANSPARENT);
        SetTextColor(pdc, 8 + 8 * 256 + 8 * 65536); //8,8,8
        TextOut(pdc, x - 1, y, t->d, t->l);
        TextOut(pdc, x + 1, y, t->d, t->l);
        TextOut(pdc, x, y - 1, t->d, t->l);
        TextOut(pdc, x, y + 1, t->d, t->l);
        SetTextColor(pdc, fixcol(txtcol));
        TextOut(pdc, x, y, t->d, t->l);
        SelectObject(pdc, old_font);
    }
    s->s->ReleaseDC(pdc);
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
    // MM-P2.2: release all tracked surfaces first, then the DirectDraw
    // interfaces. This keeps COM teardown ordering explicit on client exit.
    //
    // MM-P8.1: RAII candidate — the DirectDraw device pair (dd/dd1) plus the
    // surflist[] surface registry are a textbook RAII subsystem. A future
    // "DDDevice" type (ctor = CreateDD/QueryInterface, dtor = this teardown)
    // and a ComPtr-backed surf wrapper would make this explicit shutdown call
    // unnecessary and remove the malloc/Release split in surfstruct()/free().
    purgesurfaces();
    if (dd) {
        dd->Release();
        dd = NULL;
    }
    if (dd1) {
        dd1->Release();
        dd1 = NULL;
    }
}

/*
void img0(surf* d,long x,long y,surf* s)
{
static RECT r1,r2;
r1.top=y;
r1.bottom=y+s->d.dwHeight;
r1.left=x;
r1.right=x+s->d.dwWidth;
r2.left=0;
r2.top=0;
r2.bottom=s->d.dwHeight;
r2.right=s->d.dwWidth;

if (r1.right>d->d.dwWidth)
{
r2.right-=r1.right-d->d.dwWidth;
r1.right=d->d.dwWidth;
}
if (r1.bottom>d->d.dwHeight)
{
r2.bottom-=r1.bottom-d->d.dwHeight;
r1.bottom=d->d.dwHeight;
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
    d->s->Blt(NULL, s->s, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
}

surf *loadimage(LPCSTR name, long flags) {
    static HBITMAP bmh; //handle to loaded bitmap
    static BITMAP bm; //bitmap info buffer
    static long bmx, bmy; //width, height
    static surf *s; //temp surf pointer, for new image
    static HDC sdc, bdc; //surface device, bitmap device
    bmh = (HBITMAP) LoadImage(hInst, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (bmh == NULL) return NULL;
    GetObject(bmh, sizeof(BITMAP), &bm);
    bmx = (DWORD) bm.bmWidth;
    bmy = (DWORD) bm.bmHeight;
    s = newsurf(bmx, bmy, flags); //1=SURF_SYSMEM
    bdc = CreateCompatibleDC(NULL);
    // Select the loaded bitmap into the temporary DC, saving the previous object
    // so we can restore it before deleting the bitmap. Deleting a GDI object
    // while it's still selected into a DC is undefined and can leak resources.
    HGDIOBJ _old_bmp = SelectObject(bdc, bmh);
    s->s->GetDC(&sdc);
    BitBlt(sdc, 0, 0, bmx, bmy, bdc, 0, 0, SRCCOPY);
    s->s->ReleaseDC(sdc);
    // Restore the previous object into the DC before deleting the bitmap and DC.
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
    for (i = 0; i < 16384; i++) {
        if (surflist[i] == s) surflist[i] = NULL;
    }
    s->s->Release();
    free((void *) s);
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
    dx = d->d.dwWidth;
    if (x >= dx) return;
    dy = d->d.dwHeight;
    if (y >= dy) return;
    sx = s->d.dwWidth;
    if (-x >= sx) return;
    sy = s->d.dwHeight;
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
    asm_copy_vc_sourceskip = (long) s->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->d.lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->d.lPitch;
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
    dx = d->d.dwWidth;
    if (x >= dx) return;
    dy = d->d.dwHeight;
    if (y >= dy) return;
    sx = s->d.dwWidth;
    if (-x >= sx) return;
    sy = s->d.dwHeight;
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
    asm_copy_vc_sourceskip = (long) s->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->d.lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->d.lPitch;
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
    dx = d->d.dwWidth;
    if (x >= dx) return;
    dy = d->d.dwHeight;
    if (y >= dy) return;
    sx = s->d.dwWidth;
    if (-x >= sx) return;
    sy = s->d.dwHeight;
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
    asm_copy_vc_sourceskip = (long) s->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->d.lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->d.lPitch;
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
    dx = d->d.dwWidth;
    if (x >= dx) return;
    dy = d->d.dwHeight;
    if (y >= dy) return;
    sx = s->d.dwWidth;
    if (-x >= sx) return;
    sy = s->d.dwHeight;
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
    asm_copy_vc_sourceskip = (long) s->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = (long) d->d.lPitch - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = (unsigned long) s->o + x3 * 2 + y3 * (long) s->d.lPitch;
    asm_copy_vc_destoffset = (unsigned long) d->o + x2 * 2 + y2 * (long) d->d.lPitch;
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