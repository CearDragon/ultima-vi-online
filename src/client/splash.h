// splash.h — minimal pre-game splash screen for the Ultima VI Online client.
//
// Shows a borderless, centered, no-taskbar popup window with a PNG image (or
// a graceful text-only fallback) for at least the requested duration in
// milliseconds. The call is blocking but pumps window messages, so the
// splash repaints correctly if the user drags another window over it during
// the wait.
//
// Usage (from _tWinMain, CLIENT only):
//     splash_run(hInstance,
//                u6o::client::splash::DefaultSearchPaths(),
//                u6o::client::splash::kDefaultDurationMs);
//
// Designed so that on failure (image missing, GDI+ init failed, window
// creation failed) the function still sleeps for the requested duration and
// returns cleanly — the game must always start.
#pragma once

#include <windows.h>

namespace u6o {
    namespace client {
        namespace splash {
            // Three seconds, as requested by product. Adjust here if the duration ever
            // becomes configurable.
            constexpr DWORD kDefaultDurationMs = 4000;

            // Null-terminated list of UTF-16 paths the splash will probe in order for
            // the splash image. The first one that opens wins. Keep this array
            // statically allocated; the splash only reads from it.
            const wchar_t *const*DefaultSearchPaths();

            // Show the splash, then return no earlier than `minDurationMs` after the
            // call. Always returns; failures are swallowed (the game starts regardless).
            void Run(HINSTANCE hInstance,
                     const wchar_t *const*searchPaths,
                     DWORD minDurationMs);
        }
    }
} // namespace u6o::client::splash