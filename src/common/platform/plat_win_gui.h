// LH-P3 / LH-P5.2: Win32 GUI/handle/message type stubs (non-Windows).
//
// The host shares headers with the (Windows-only) client, so a handful of
// Win32 windowing types and the ubiquitous MessageBox error-reporter leak into
// host-compiled translation units (e.g. function_host.cpp's "ERROR CORRECTION"
// paths, data_host.h's hWnd globals). The dedicated Linux host is headless, so
// these are provided as opaque/no-op stubs purely so the shared declarations
// PARSE and the few runtime calls degrade gracefully (MessageBox -> stderr).
//
// This is deliberately minimal: only the symbols actually referenced by
// host-compiled code are stubbed. The real windowing/message-pump severance
// (the entry point, WndProc, the splash, DirectDraw) is LH-P5.
#ifndef U6O_PLAT_WIN_GUI_H
#define U6O_PLAT_WIN_GUI_H

#ifndef _WIN32

#include "plat_types.h"
#include <cstdio> // stderr for the MessageBox stub

// Opaque window/handle types. The host never dereferences these; they exist so
// shared declarations like `extern HWND hWnd;` compile.
typedef void *HWND;
typedef void *HINSTANCE;
typedef void *HMENU;
typedef void *HACCEL;
typedef void *HDC;
typedef void *HGDIOBJ;
typedef void *HBITMAP;
typedef void *HBRUSH;
typedef void *HFONT;
typedef void *HICON;
typedef void *HCURSOR;

// Window-message scalar types (i386 widths: 32-bit).
typedef unsigned short ATOM;
typedef intptr_t       LRESULT;
typedef uintptr_t      WPARAM;
typedef intptr_t       LPARAM;

// Calling-convention annotation used on WndProc-style declarations.
#ifndef CALLBACK
#define CALLBACK
#endif

// MessageBox flag(s) the host passes. Only MB_OK is used today.
#ifndef MB_OK
#define MB_OK 0x00000000
#endif
#ifndef IDOK
#define IDOK 1
#endif

// Headless MessageBox: the host uses MessageBox(NULL, text, caption, MB_OK) as
// an error reporter ("ERROR CORRECTION: ..."). On a server we surface these on
// stderr (captured by the container/k8s log) and continue, returning IDOK as a
// real modal dialog's default would.
inline int MessageBox(HWND /*hWnd*/, const char *text, const char *caption,
                      unsigned int /*type*/) {
    fprintf(stderr, "[MessageBox] %s: %s\n",
            caption ? caption : "Ultima 6 Online",
            text ? text : "");
    return IDOK;
}

#endif // !_WIN32

#endif // U6O_PLAT_WIN_GUI_H

