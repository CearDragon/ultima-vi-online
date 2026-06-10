// LH-P1.1: Platform abstraction umbrella header.
//
// Purpose
// -------
// Provide a single seam that lets the HOST build compile on both MSVC/Win32
// and a POSIX (Linux) toolchain without touching the goto-heavy, fixed-array
// hot paths in loop_host.cpp / function_host.cpp.
//
// Contract (READ THIS before editing any plat_*.h):
//   * On _WIN32 every shim is a PASS-THROUGH to the existing Windows headers
//     and macros. Including <platform.h> on Windows must NOT change a single
//     observable behavior of the legacy build — it only forwards.
//   * On non-Windows the shims map the small Win32 surface the host uses onto
//     POSIX equivalents (BSD sockets, pthreads, clock_gettime, stdio).
//   * This is a portability seam, NOT a refactor. Do not "modernize" call
//     sites; keep the legacy style. See docs/plans/plan-linuxHost.md.
//
// Wire/save invariant:
//   The Linux host is built i386 (-m32) so sizeof(long)==4 and pointers are
//   32-bit, preserving struct/`txt`/.sav layout and wire compatibility with
//   Windows clients. None of these shims change data layout. U6O_VERSION is
//   intentionally NOT bumped by this work (non-wire change).
//
// Include order matters on Windows: <winsock2.h> must precede <windows.h>,
// so plat_sockets.h is pulled in first.
#ifndef U6O_PLATFORM_H
#define U6O_PLATFORM_H

#include "plat_sockets.h" // must come before any <windows.h> on Win32
#include "plat_types.h"
#include "plat_win_gui.h"
#include "plat_threads.h"
#include "plat_time.h"

#endif // U6O_PLATFORM_H

