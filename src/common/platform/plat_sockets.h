// LH-P1.1 / LH-P3: Socket portability shim.
//
// On _WIN32 this is a pass-through to <winsock2.h> (plus <ws2tcpip.h> for
// newer name resolution). On POSIX it maps the exact Winsock surface the host
// uses (see the Background table in docs/plans/plan-linuxHost.md) onto BSD
// sockets, so call sites in function_both.cpp / setup_host.inc /
// function_host.cpp can stay byte-for-byte the same.
//
// Host Winsock surface being mapped:
//   WSAStartup/WSACleanup, WSADATA, socket/bind/listen/accept/connect,
//   setsockopt, ioctlsocket(FIONBIO), send/recv/shutdown, closesocket,
//   INVALID_SOCKET, SOCKET_ERROR, WSAGetLastError, WSAEWOULDBLOCK,
//   SD_RECEIVE/SD_SEND/SD_BOTH, sockaddr_in, htons, INADDR_ANY,
//   TCP_NODELAY, SOL_SOCKET, SO_RCVBUF/SO_SNDBUF, SOMAXCONN.
//
// NOTE: Wiring the call sites to these shims is LH-P3; this header only
// establishes the mappings. Keep additions here, not scattered #ifdefs.
#ifndef U6O_PLAT_SOCKETS_H
#define U6O_PLAT_SOCKETS_H

#ifdef _WIN32

// --- Windows: forward to the real headers (winsock2 BEFORE windows.h) ------
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#else // ---------------------------------------------------------------------

// --- POSIX: BSD-socket equivalents of the Winsock names the host uses ------
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// Winsock models a socket as an opaque unsigned handle; BSD uses an int fd.
typedef int SOCKET;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

// shutdown() "how" values.
#ifndef SD_RECEIVE
#define SD_RECEIVE SHUT_RD
#define SD_SEND    SHUT_WR
#define SD_BOTH    SHUT_RDWR
#endif

// Winsock reports the last error via a function; POSIX uses errno. The host
// only ever compares against WSAEWOULDBLOCK in the recv loop.
#ifndef WSAEWOULDBLOCK
#define WSAEWOULDBLOCK EWOULDBLOCK
#endif
inline int WSAGetLastError(void) { return errno; }

// Winsock startup/teardown have no POSIX analogue — make them no-ops so the
// setup_host.inc / shutdown call sites compile unchanged.
typedef struct { int unused; } WSADATA;
inline int  WSAStartup(unsigned short /*version*/, WSADATA * /*data*/) { return 0; }
inline int  WSACleanup(void) { return 0; }

// closesocket -> close.
inline int closesocket(SOCKET s) { return ::close(s); }

// ioctlsocket(s, FIONBIO, &arg): the host only uses it to toggle non-blocking
// mode. Map onto fcntl(O_NONBLOCK) rather than FIONBIO so behavior is portable
// across libc implementations.
inline int ioctlsocket(SOCKET s, long cmd, unsigned long *argp) {
    if (cmd == FIONBIO && argp) {
        int flags = ::fcntl(s, F_GETFL, 0);
        if (flags == -1) return SOCKET_ERROR;
        if (*argp) flags |= O_NONBLOCK;
        else       flags &= ~O_NONBLOCK;
        return (::fcntl(s, F_SETFL, flags) == -1) ? SOCKET_ERROR : 0;
    }
    return SOCKET_ERROR;
}

// send()/recv()/shutdown()/bind()/listen()/accept()/connect()/setsockopt()/
// htons()/INADDR_ANY/SOMAXCONN/TCP_NODELAY/SOL_SOCKET/SO_* and struct
// sockaddr_in are all provided by the POSIX headers above with compatible
// signatures (the host casts buffers to char*, which is accepted).

#endif // _WIN32

#endif // U6O_PLAT_SOCKETS_H

