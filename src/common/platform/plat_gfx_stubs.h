// LH-P6: minimal stubs for the few graphics types/functions that leak into
// host-compiled code. globals.inc instantiates client UI-panel / surface
// globals (shared file), and loop_host.cpp's custom-portrait loader calls
// loadimage(). The headless host never renders, so these are opaque/no-op:
// panel pointers stay null and loadimage() returns null (server-side custom
// portraits are simply not loaded — the call site already null-checks).
#ifndef U6O_PLAT_GFX_STUBS_H
#define U6O_PLAT_GFX_STUBS_H

#ifndef _WIN32

// Minimal DirectDraw surface. Host code only touches the pixel pointer `o`,
// and only when the surface is non-null (never, with the stub loader).
struct surf {
    unsigned char *o;
};

// UI-panel types from frame.h (severed on the host). Host code only ever holds
// pointers to these, so tag-only declarations are enough.
struct FRAME;
struct FRM_ONOFF;
struct FRM_TYPE;
struct FRM_IMAGE;
struct FRM_INPUT;
struct FRM_TXT;
struct FRM_LIST;
struct FRM_LIST_ITEM;

// loadimage() memory-class flag the host passes; value is irrelevant here.
#ifndef SURF_SYSMEM16
#define SURF_SYSMEM16 0
#endif

// Severed image loader — returns null on the headless host (definition in
// plat_stubs.cpp).
surf *loadimage(const char *name, int memflags);

#endif // !_WIN32

#endif // U6O_PLAT_GFX_STUBS_H

