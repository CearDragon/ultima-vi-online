// LH-P5.2: Host-side stubs for graphics/audio entry points that the shared
// host setup/entry code still references, but whose implementation TUs
// (myddraw.cpp / frame.cpp / dmusic.cpp / sound.cpp) are NOT compiled into the
// headless Linux host (see the LH-P6 CMake source list). These declarations
// keep u6o7.cpp / setup_host.inc link-clean on a server with no display.
//
// Definitions live in plat_stubs.cpp, which is compiled ONLY in the non-Windows
// host build. On Windows this header is inert.
#ifndef U6O_PLAT_STUBS_H
#define U6O_PLAT_STUBS_H

#ifndef _WIN32

// From frame.h (severed on the host): initializes the UI-panel globals. The
// dedicated host draws nothing, so this is a no-op; the panel globals are
// statically zero-initialized anyway.
void frame_init(void);

// From myddraw.h (severed on the host): would create the DirectDraw primary
// surface. The host call site is gated under _WIN32, but declare it so any
// stray reference still links.
bool setupddraw(void);

#endif // !_WIN32

#endif // U6O_PLAT_STUBS_H

