// LH-P5.2: definitions of the severed-graphics host stubs. Compiled ONLY in the
// Linux host build (added to the LH-P6 CMake source list); never compiled on
// Windows. See plat_stubs.h for the rationale.
#ifndef _WIN32

#include "plat_stubs.h"

// UI panels are never used by the dedicated host; the panel globals are
// statically zero-initialized, so initialization is a no-op here.
void frame_init(void) {}

// The headless host has no display surface; report "success" so any gated
// caller proceeds.
bool setupddraw(void) { return true; }

#endif // !_WIN32

