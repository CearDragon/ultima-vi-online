#ifndef _INC_MYFILE
#define _INC_MYFILE

// LH-P2: file I/O portability seam.
//
// The HOST opens/reads/writes its data and save files through this tiny
// wrapper. On Windows it is backed by the legacy Win32 OpenFile/_hread/_hwrite
// family (unchanged). On POSIX it is backed by open/read/write/lseek
// (see myfile.cpp). To keep every call site compiling unchanged, the `file`
// struct member stays named `h`, and `HFILE`, `HFILE_ERROR` and the small
// subset of `OF_*` flags the host passes to open2() are provided here on
// non-Windows with their exact Win32 numeric values.
//
// Wire/save invariant: this layer changes HOW bytes are read/written, never
// WHICH bytes — the .sav/.bin formats are byte-identical across platforms.

#ifdef _WIN32

#include <winsock2.h>

struct file {
    HFILE h;
};

#include "mytxt.h"
#include <io.h>

#else // ---------------------------------------------------------------------

#include "platform/plat_types.h" // LPCSTR, DWORD, etc.

// Win32 file handle modeled as a POSIX file descriptor on this platform.
typedef int HFILE;
#ifndef HFILE_ERROR
#define HFILE_ERROR (-1)
#endif

// Subset of Win32 OpenFile() OF_* flags used by the host, with the exact
// values from winbase.h so callers in other TUs (host.inc, loop_host.cpp,
// function_host.cpp) that build `OF_READWRITE | OF_CREATE | ...` keep working.
#ifndef OF_READ
#define OF_READ             0x00000000
#define OF_WRITE            0x00000001
#define OF_READWRITE        0x00000002
#define OF_SHARE_COMPAT     0x00000000
#define OF_SHARE_EXCLUSIVE  0x00000010
#define OF_SHARE_DENY_WRITE 0x00000020
#define OF_SHARE_DENY_READ  0x00000030
#define OF_SHARE_DENY_NONE  0x00000040
#define OF_DELETE           0x00000200
#define OF_CREATE           0x00001000
#endif

struct file {
    HFILE h;
};

#include "mytxt.h"

#endif // _WIN32

file *open(LPCSTR name);

file *open2(LPCSTR name, unsigned long flags);

void get(file *filepointer, void *destoffset, long bytes);

void seek(file *filepointer, long fileoffset);

void close(file *filepointer);

void put(file *filepointer, void *sourceoffset, long bytes);

long seek(file *filepointer);

long lof(file *filepointer);

file *open(txt * t);

file *open2(txt *t, unsigned long flags);

extern long loadfile_FILESIZE;

void *loadfile(LPCSTR name);

void waitforfile(LPCSTR name);

void deletefile(LPCSTR name);
#endif