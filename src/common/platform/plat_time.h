// LH-P1.1 / LH-P4.2: Time/sleep portability shim.
//
// On _WIN32 this forwards to the Win32 timing API (via <windows.h> from
// plat_types.h, plus <mmsystem.h> for timeGetTime). On POSIX it maps the
// exact timing surface the host uses onto clock_gettime / nanosleep /
// localtime_r.
//
// Mapped surface:
//   timeGetTime()                 (host.inc seed, u6o7.cpp main-loop pacing)
//   Sleep(ms) / SleepEx(ms, b)    (thread loops, shutdown waits)
//   SYSTEMTIME + GetLocalTime()   (setup_host.inc log timestamp, crash filter)
#ifndef U6O_PLAT_TIME_H
#define U6O_PLAT_TIME_H

#include "plat_types.h"

#ifdef _WIN32

#include <mmsystem.h> // timeGetTime (links winmm on the MSVC build)

#else // ---------------------------------------------------------------------

#include <ctime>

// timeGetTime(): milliseconds from a monotonic clock. The host only uses
// deltas (oldtime/newtime pacing, RNG seed), so the epoch is irrelevant.
inline DWORD timeGetTime(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (DWORD)((DWORD)ts.tv_sec * 1000u + (DWORD)(ts.tv_nsec / 1000000));
}

// Sleep / SleepEx: millisecond sleeps. The bAlertable flag (APC delivery) has
// no host-visible effect here, so it's ignored.
inline void Sleep(DWORD ms) {
    struct timespec req;
    req.tv_sec  = (time_t)(ms / 1000);
    req.tv_nsec = (long)((ms % 1000) * 1000000L);
    nanosleep(&req, nullptr);
}
inline DWORD SleepEx(DWORD ms, BOOL /*bAlertable*/) { Sleep(ms); return 0; }

// SYSTEMTIME / GetLocalTime: only the fields the host formats are populated.
typedef struct _SYSTEMTIME {
    WORD wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

inline void GetLocalTime(LPSYSTEMTIME st) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tmv;
    localtime_r(&ts.tv_sec, &tmv);
    st->wYear         = (WORD)(tmv.tm_year + 1900);
    st->wMonth        = (WORD)(tmv.tm_mon + 1);
    st->wDayOfWeek    = (WORD)tmv.tm_wday;
    st->wDay          = (WORD)tmv.tm_mday;
    st->wHour         = (WORD)tmv.tm_hour;
    st->wMinute       = (WORD)tmv.tm_min;
    st->wSecond       = (WORD)tmv.tm_sec;
    st->wMilliseconds = (WORD)(ts.tv_nsec / 1000000);
}

#endif // _WIN32

#endif // U6O_PLAT_TIME_H

