#pragma once

// LH-P6: resource.h is the Win32 resource-ID header (src/client/Resource.h),
// consumed only by the windowing/menu/icon code that is itself gated under
// #ifdef _WIN32 (see u6o7.cpp WndProc/InitInstance/MyRegisterClass). The
// headless Linux host references none of those IDs. It must also be guarded
// here because Linux filesystems are case-sensitive: the include spells
// "resource.h" but the file on disk is "Resource.h", so an unguarded include
// fails to resolve inside the container (it only worked under WSL because the
// /mnt/c mount is case-insensitive). On Windows _WIN32 is always defined, so
// this is a no-op for the MSVC build.
#ifdef _WIN32
#include "resource.h"
#endif
