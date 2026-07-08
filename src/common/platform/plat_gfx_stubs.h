// LH-P6: minimal stubs for the few graphics types/functions that leak into
// host-compiled code. globals.inc instantiates client UI-panel / surface
// globals (shared file), and loop_host.cpp's custom-portrait loader calls
// loadimage(). The headless host never renders, so the UI-panel pointers stay
// null. loadimage() is NOT a no-op, however: the host must read the custom
// portrait BMPs it stores under .\save\port\ to serve them to clients, so
// plat_stubs.cpp implements a GDI-free 24/32-bit BMP -> RGB565 decoder that
// reproduces the Windows loadimage(...,SURF_SYSMEM16) pixel format exactly.
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

// Custom-portrait BMP decoder for the headless host (definition in
// plat_stubs.cpp). Reads a 24/32-bit BI_RGB BMP into a top-down RGB565 buffer;
// returns null if the file is missing or an unsupported format.
surf *loadimage(const char *name, int memflags);

#endif // !_WIN32

#endif // U6O_PLAT_GFX_STUBS_H

