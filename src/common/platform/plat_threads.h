// LH-P1.1 / LH-P4.1: Thread portability shim.
//
// On _WIN32 this forwards to the Win32 thread API (via <windows.h>, pulled in
// by plat_types.h). On POSIX it maps the exact thread surface the host uses
// onto pthreads, so the existing `DWORD WINAPI proc(LPVOID)` thread bodies
// (sockets_accept / sockets_send / sockets_receive / sockets_disconnect /
// revive_infiniteloopexit) compile and run unchanged.
//
// Mapped surface: CreateThread, ExitThread, TerminateThread,
// LPTHREAD_START_ROUTINE, thread HANDLE/DWORD id.
//
// IMPORTANT (validated in LH-P4): the legacy disconnect path stores the
// numeric thread id and later calls TerminateThread((void*)thread_id, 0).
// To keep that working we make the returned HANDLE and the written thread id
// be the SAME pthread_t value (it fits in a 32-bit handle on the i386 build),
// and map TerminateThread -> pthread_cancel. The disconnect path always
// requests a cooperative exit first and waits, so cancellation is a
// last-resort, matching the Win32 semantics closely enough.
#ifndef U6O_PLAT_THREADS_H
#define U6O_PLAT_THREADS_H

#include "plat_types.h"

#ifndef _WIN32

#include <pthread.h>

// Win32 thread-proc type: DWORD WINAPI fn(LPVOID).
typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID);

namespace u6o_plat {
struct ThreadStart {
    LPTHREAD_START_ROUTINE proc;
    LPVOID arg;
};
inline void *thread_trampoline(void *p) {
    ThreadStart ts = *static_cast<ThreadStart *>(p);
    delete static_cast<ThreadStart *>(p);
    // Discard the DWORD return; the host never reads a thread exit code.
    (void)ts.proc(ts.arg);
    return nullptr;
}
} // namespace u6o_plat

// CreateThread(lpSA, dwStackSize, proc, arg, flags, lpThreadId).
// Only proc/arg/lpThreadId are meaningful to the host. Returns a HANDLE that
// is the pthread_t value (also written to *lpThreadId), or NULL on failure.
inline HANDLE CreateThread(void * /*lpThreadAttributes*/,
                           unsigned long /*dwStackSize*/,
                           LPTHREAD_START_ROUTINE proc,
                           LPVOID arg,
                           DWORD /*dwCreationFlags*/,
                           LPDWORD lpThreadId) {
    pthread_t tid;
    u6o_plat::ThreadStart *ts = new u6o_plat::ThreadStart{proc, arg};
    if (pthread_create(&tid, nullptr, u6o_plat::thread_trampoline, ts) != 0) {
        delete ts;
        return (HANDLE)0;
    }
    pthread_detach(tid);
    if (lpThreadId) *lpThreadId = (DWORD)tid;
    return (HANDLE)tid;
}

// ExitThread(exitCode): end the calling thread.
inline void ExitThread(DWORD /*exitCode*/) { pthread_exit(nullptr); }

// TerminateThread(handle, exitCode): forced kill — last resort. The handle is
// the pthread_t we returned from CreateThread.
inline BOOL TerminateThread(HANDLE h, DWORD /*exitCode*/) {
    return pthread_cancel((pthread_t)h) == 0 ? TRUE : FALSE;
}

#endif // !_WIN32

#endif // U6O_PLAT_THREADS_H

