// LH-P1.1 / LH-P4.3: Win32 base-type / macro shim.
//
// On _WIN32 this forwards to <windows.h>. On POSIX it defines the handful of
// Win32 typedefs and helper macros the host code spells out directly
// (DWORD, WORD, BYTE, BOOL, LPVOID, LPCSTR, HANDLE, WINAPI, ZeroMemory,
// MAKEWORD, _snprintf, …) so the legacy sources compile unchanged.
//
// i386 note: the Linux host is built -m32, so these widths match the Win32
// build exactly (DWORD=32-bit, pointers 32-bit). Do NOT widen them — the
// .sav/wire layouts depend on it. (Endianness is assumed little-endian x86;
// see R6 in the plan.)
#ifndef U6O_PLAT_TYPES_H
#define U6O_PLAT_TYPES_H

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#else // ---------------------------------------------------------------------

#include <cstdint>
#include <cstring>  // memset, for ZeroMemory
#include <cstdio>   // snprintf, for _snprintf

// Win32 base integer aliases. These mirror the EXACT underlying types Win32
// uses (not just matching widths), because the legacy code freely interchanges
// e.g. DWORD and `unsigned long` — it casts thread-id pointers as
// `(unsigned long*)` to fill an `LPDWORD`, and the wire/save code uses
// `unsigned long` for 32-bit fields. On the i386 (-m32) build `unsigned long`
// is 32-bit, so `DWORD == unsigned long` is both width-correct AND
// type-compatible with those casts. (Do NOT use uint32_t here: that is
// `unsigned int` on Linux x86, which makes `unsigned long*` -> `DWORD*` a hard
// type error.)
typedef unsigned long  DWORD;
typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef int            BOOL;
typedef unsigned int   UINT;
typedef long           LONG;
typedef unsigned long  ULONG;

// Pointer/handle aliases.
typedef void *        LPVOID;
typedef const char *  LPCSTR;
typedef char *        LPSTR;
typedef void *        HANDLE;
typedef DWORD *       LPDWORD;

// Calling-convention / entry-point annotations are meaningless on POSIX.
#ifndef WINAPI
#define WINAPI
#endif
#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef __stdcall
#define __stdcall
#endif

// Booleans the legacy code relies on (normally from windef.h).
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Small helper macros used throughout the host sources.
#ifndef ZeroMemory
#define ZeroMemory(dest, len) memset((dest), 0, (len))
#endif
#ifndef MAKEWORD
#define MAKEWORD(a, b) \
    ((WORD)(((BYTE)((a) & 0xff)) | (((WORD)((BYTE)((b) & 0xff))) << 8)))
#endif

// MSVC's _snprintf maps to standard snprintf (truncation semantics differ on
// overflow, but the host call sites size their buffers explicitly).
#ifndef _snprintf
#define _snprintf snprintf
#endif

#endif // _WIN32

#endif // U6O_PLAT_TYPES_H

