#ifndef _INC_MYDDRAW
#define _INC_MYDDRAW
#define rgb(r, g, b) (b+((g)<<8)+((r)<<16))
//#define STRICT
//#define INITGUID
//#define D3D_OVERLOADS *REDUNDANT
#include <ddraw.h>

//#include <d3d.h> *REDUNDANT
//#include <d3dutil.h>
//#include <d3dmath.h>
struct surface {
    DDSURFACEDESC2 d;
    LPDIRECTDRAWSURFACE4 s;
    union {
        unsigned long *o;
        unsigned char *o1;
        unsigned short *o2;
    };
//IDirect3DTexture2* t; //only valid if SURF_TEX flag is used *REDUNDANT
};
extern IDirectDraw *dd1;
extern IDirectDraw4 *dd;

#define SURF_VIDMEM 0 //surface in video memory compatible with primary surface
#define SURF_SYSMEM 1 //888 RGB surface in system memory (DirectAccess OK)
//#define SURF_3D 2 //*REDUNDANT
//#define SURF_TEX 4 //*REDUNDANT
//#define SURF_ZBUF 8 //*REDUNDANT
#define SURF_SYSVID 16 //compatible with primary surface in system memory
#define SURF_PRIMARY 32 //primary surface
#define SURF_SYSMEM16 64 //565 RGB surface in system memory (DirectAccess OK)

#include "mytxt.h"

surface *surfstruct();

surface *newsurf(long x, long y, long flags);

bool setupddraw();

void pset(surface *s, long x, long y, DWORD c);

DWORD point(surface *s, long x, long y);

void cls(surface *s, DWORD c);

void img(surface *d, long x, long y, surface *s);

void txtout(surface *s, long x, long y, txt *t);

void txtouts(surface *s, long x, long y, txt *t);

DWORD fixcol(DWORD c);

DWORD getcol(DWORD c);

extern DWORD txtcol;
extern HFONT txtfnt;
extern DDPIXELFORMAT DDRAW_display_pixelformat;

void purgesurfaces();

void img0(surface *d, long x, long y, surface *s);

surface *loadimage(LPCSTR name);

surface *loadimage(txt *name);

surface *loadimage(LPCSTR name, long flags);

surface *loadimage(txt *name, long flags);

void free(surface *s);

void refresh(surface *pSurface);

void img(surface *d, surface *s);

void img0(surface *d, surface *s);

void img0_0key(surface *s, unsigned short c);

void imgt(surface *d, long x, long y, surface *s);

void imgt0(surface *d, long x, long y, surface *s);

void img75t0(surface *d, long x, long y, surface *s);

void img75t(surface *d, long x, long y, surface *s);

// r999 img to handle resizing and positioning
void img(surface *d, surface *s, int x, int y, int x2, int y2);

// r999
void imguiw(surface *d, int uipaneli, int uiwidgeti, int uistatei, surface *s);

void imguiw(surface *d, int uipaneli, int uiwidgeti, int uistatei);

void imguip(surface *d, int uipaneli);

void imguip(surface *d, int uipaneli, surface *s);


#endif
