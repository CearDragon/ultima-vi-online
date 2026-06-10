#ifdef _WIN32
#include "stdafx.h"
#endif
#include "myfile.h"
#include "mytxt.h"

#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <strings.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
// On POSIX, file-open failures are reported to the host log rather than a GUI
// MessageBox. Forward-declared to avoid pulling the heavy function_both.h
// (which includes <winsock2.h>) into this translation unit.
void LOGadd(txt *t);
#endif

/*
typedef struct file
{
HFILE h;
} file;
*/

txt *file_error;
txt *file_error_name;

#ifdef _WIN32
// ======================= Windows (legacy Win32) backend ====================

file *open(LPCSTR name) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(name, &fs, OF_READWRITE | OF_SHARE_COMPAT);
    if (tf->h == HFILE_ERROR) {
        file_error_name = txtnew();
        txtset(file_error_name, name);
        file_error = txtnew();
        txtset(file_error, "File ");
        txtadd(file_error, file_error_name);
        txtadd(file_error, " not found");
        MessageBox(NULL, file_error->d, "Ultima 6 Online", MB_OK);
    }
    return tf;
}

file *open2(LPCSTR name, unsigned long flags) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(name, &fs, flags);
    return tf;
}

file *open2(txt *t, unsigned long flags) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(t->d, &fs, flags);
    return tf;
}

/*
typedef struct txt
{
char* d; //pointer to data
long l; //length of text in buffer
long bl; //length of current buffer
} txt;
*/

file *open(txt *t) {
    static file *tf;
    static OFSTRUCT fs;
    tf = (file *) malloc(sizeof(file));
    tf->h = HFILE_ERROR;
    tf->h = OpenFile(t->d, &fs, OF_READWRITE | OF_SHARE_COMPAT);
    if (tf->h == HFILE_ERROR) {
        txtset(file_error_name, t);
        file_error = txtnew();
        txtset(file_error, "File ");
        txtadd(file_error, file_error_name);
        txtadd(file_error, " not found");
        MessageBox(NULL, file_error->d, "Ultima 6 Online", MB_OK);
    }
    return tf;
}

void get(file *filepointer, void *destoffset, long bytes) {
    if (filepointer->h != HFILE_ERROR) _hread(filepointer->h, destoffset, bytes);
    return;
}

void put(file *filepointer, void *sourceoffset, long bytes) {
    if (filepointer->h != HFILE_ERROR) _hwrite(filepointer->h, (LPCSTR) sourceoffset, bytes);
    return;
}

void seek(file *filepointer, long fileoffset) {
    if (filepointer->h != HFILE_ERROR) _llseek(filepointer->h, fileoffset, FILE_BEGIN);
    return;
}

void close(file *filepointer) {
    if (filepointer->h != HFILE_ERROR) _lclose(filepointer->h);
    free((void *) filepointer);
    return;
}

long seek(file *filepointer) {
    if (filepointer->h == HFILE_ERROR) return 0;
    return _llseek(filepointer->h, 0, FILE_CURRENT);
}

long lof(file *filepointer) {
    if (filepointer->h == HFILE_ERROR) return 0;
    static long i, i2;
    i = _llseek(filepointer->h, 0, FILE_CURRENT);
    i2 = _llseek(filepointer->h, 0, FILE_END);
    _llseek(filepointer->h, i, FILE_BEGIN);
    return i2;
}

long loadfile_FILESIZE;

void *loadfile(LPCSTR name) {
    static file *f;
    static void *v;
    f = open(name);
    if (f->h != HFILE_ERROR) return NULL;
    loadfile_FILESIZE = lof(f);
    v = malloc(loadfile_FILESIZE);
    get(f, v, loadfile_FILESIZE);
    close(f);
    return v;
}

void waitforfile(LPCSTR name) {
    static OFSTRUCT fs;
    static HFILE hfile;
waitforfile_retry:
    hfile = OpenFile(name, &fs, OF_SHARE_EXCLUSIVE | OF_READWRITE);
    if (hfile == HFILE_ERROR) goto waitforfile_retry;
    _lclose(hfile);
}

void deletefile(LPCSTR name) {
    static OFSTRUCT fs;
    OpenFile(name, &fs, OF_DELETE);
    return;
}

#else // ===================== POSIX (Linux) backend =========================

// Translate the OF_* access bits the host passes into open(2) flags. Every
// helper here reads AND writes through the handle, but the access bits are
// honored faithfully. OF_CREATE maps to create-or-truncate, matching Win32
// OpenFile ("if the file exists, it is truncated to zero length") — the host
// relies on this when rewriting house.sav and the per-player saves.
static int u6o_oflags(unsigned long flags) {
    int o;
    switch (flags & 0x3) { // OF_READ=0, OF_WRITE=1, OF_READWRITE=2
        case OF_WRITE:     o = O_WRONLY; break;
        case OF_READWRITE: o = O_RDWR;   break;
        default:           o = O_RDONLY; break;
    }
    if (flags & OF_CREATE) o |= O_CREAT | O_TRUNC;
    return o;
}

// open()/open2() always return a non-NULL file* (mirroring the Win32 backend);
// failure is signaled via h == HFILE_ERROR, which every call site checks.
static file *u6o_make(HFILE h) {
    file *tf = (file *) malloc(sizeof(file));
    tf->h = h;
    return tf;
}

// The legacy code spells every path Windows-style (".\\save\\house.sav",
// ".\\ultima6\\chunks", ...). On Linux a backslash is an ordinary filename
// character, so translate '\' -> '/' before hitting the filesystem. This lets
// all ~hundreds of call sites keep their original path strings unchanged.
static const char *u6o_normpath(const char *name, char *buf, size_t bufsz) {
    size_t i = 0;
    for (; name[i] && i + 1 < bufsz; i++) buf[i] = (name[i] == '\\') ? '/' : name[i];
    buf[i] = 0;
    return buf;
}

// Case-insensitive directory lookup: find an entry in `dir` whose name equals
// `want` ignoring case, copying the real on-disk spelling into `found`.
static bool u6o_ci_lookup(const char *dir, const char *want, char *found, size_t foundsz) {
    DIR *d = ::opendir(dir[0] ? dir : ".");
    if (!d) return false;
    bool ok = false;
    struct dirent *e;
    while ((e = ::readdir(d)) != NULL) {
        if (strcasecmp(e->d_name, want) == 0) {
            snprintf(found, foundsz, "%s", e->d_name);
            ok = true;
            break;
        }
    }
    ::closedir(d);
    return ok;
}

// Resolve an already-'/'-normalized path case-insensitively, component by
// component, against the real filesystem.
//
// Why this exists: Windows is case-insensitive, so the host freely mixes the
// case of paths — hard-coded lowercase strings (".\\host\\crtenum.bin"), the
// shipped game data in UPPER (CHUNKS, MAP, OBJBLKxx under SAVEGAME), and names
// it BUILDS at runtime in UPPER (host.inc constructs "objblk" + (x+65)/(y+65)
// → "objblkEB"). On a case-sensitive Linux filesystem none of these line up,
// and there is NO single rename rule that fixes it (lowercasing the data
// breaks the runtime-UPPER "objblkEB" lookups; leaving it breaks the lowercase
// hard-coded ones). So instead of renaming game files we resolve case here:
// for each segment we try the exact spelling first, and on a miss scan the
// parent directory for a case-insensitive match and adopt the real name.
//
// If a segment has no match (a save file being created for the first time, or
// a genuinely missing file) we keep the requested spelling for that segment
// and append the remainder verbatim — so O_CREAT makes the file under the
// requested name and a real miss still reports the requested name to the log.
static const char *u6o_resolve_ci(const char *path, char *out, size_t outsz) {
    // Fast path: the exact path already exists (common once saves are written
    // by the host itself, and on a case-preserving/insensitive filesystem).
    if (::access(path, F_OK) == 0) {
        snprintf(out, outsz, "%s", path);
        return out;
    }

    out[0] = 0;
    size_t olen = 0;
    const char *p = path;
    bool failed = false; // once a segment can't be matched, stop scanning

    if (*p == '/') { out[olen++] = '/'; out[olen] = 0; p++; }

    while (*p) {
        const char *seg = p;
        while (*p && *p != '/') p++;
        size_t seglen = (size_t) (p - seg);
        if (*p == '/') p++;

        char want[256];
        if (seglen >= sizeof want) seglen = sizeof want - 1;
        memcpy(want, seg, seglen);
        want[seglen] = 0;

        // Directory to scan for this segment = what we've resolved so far.
        const char *dir = (olen == 0) ? "." : out;

        // Build the exact candidate path for an access() probe.
        char cand[1024];
        if (olen == 0) snprintf(cand, sizeof cand, "%s", want);
        else if (olen == 1 && out[0] == '/') snprintf(cand, sizeof cand, "/%s", want);
        else snprintf(cand, sizeof cand, "%s/%s", out, want);

        char chosen[256];
        if (::access(cand, F_OK) == 0) {
            snprintf(chosen, sizeof chosen, "%s", want);            // exact hit
        } else if (!failed && u6o_ci_lookup(dir, want, chosen, sizeof chosen)) {
            // adopted the real on-disk spelling
        } else {
            failed = true;
            snprintf(chosen, sizeof chosen, "%s", want);            // keep requested
        }

        // Append chosen segment to out.
        if (olen == 0) olen += snprintf(out, outsz, "%s", chosen);
        else if (olen == 1 && out[0] == '/') olen += snprintf(out + olen, outsz - olen, "%s", chosen);
        else olen += snprintf(out + olen, outsz - olen, "/%s", chosen);
    }

    if (olen == 0) snprintf(out, outsz, "%s", path);
    return out;
}

// Normalize '\' -> '/' and resolve case in one step. All open paths go through
// this so the host's original Windows-style, mixed-case path strings work
// unchanged on a case-sensitive Linux filesystem.
static const char *u6o_realpath(const char *name, char *buf, size_t bufsz) {
    char nb[1024];
    u6o_normpath(name, nb, sizeof nb);
    return u6o_resolve_ci(nb, buf, bufsz);
}

// Mirror the Win32 "File <name> not found" diagnostic, but to the host log.
static void u6o_notfound(const char *name) {
    file_error_name = txtnew();
    txtset(file_error_name, name);
    file_error = txtnew();
    txtset(file_error, "File ");
    txtadd(file_error, file_error_name);
    txtadd(file_error, " not found");
    LOGadd(file_error);
}

file *open(LPCSTR name) {
    char nb[1024];
    int fd = ::open(u6o_realpath(name, nb, sizeof nb), O_RDWR);
    if (fd < 0) u6o_notfound(name);
    return u6o_make(fd < 0 ? HFILE_ERROR : fd);
}

file *open2(LPCSTR name, unsigned long flags) {
    char nb[1024];
    int fd = ::open(u6o_realpath(name, nb, sizeof nb), u6o_oflags(flags), 0666);
    return u6o_make(fd < 0 ? HFILE_ERROR : fd);
}

file *open2(txt *t, unsigned long flags) {
    return open2((LPCSTR) t->d, flags);
}

file *open(txt *t) {
    char nb[1024];
    int fd = ::open(u6o_realpath(t->d, nb, sizeof nb), O_RDWR);
    if (fd < 0) u6o_notfound(t->d);
    return u6o_make(fd < 0 ? HFILE_ERROR : fd);
}

void get(file *filepointer, void *destoffset, long bytes) {
    if (filepointer->h != HFILE_ERROR) {
        ssize_t n = ::read(filepointer->h, destoffset, (size_t) bytes);
        (void) n;
    }
}

void put(file *filepointer, void *sourceoffset, long bytes) {
    if (filepointer->h != HFILE_ERROR) {
        ssize_t n = ::write(filepointer->h, sourceoffset, (size_t) bytes);
        (void) n;
    }
}

void seek(file *filepointer, long fileoffset) {
    if (filepointer->h != HFILE_ERROR) ::lseek(filepointer->h, fileoffset, SEEK_SET);
}

void close(file *filepointer) {
    if (filepointer->h != HFILE_ERROR) ::close(filepointer->h);
    free((void *) filepointer);
}

long seek(file *filepointer) {
    if (filepointer->h == HFILE_ERROR) return 0;
    return (long) ::lseek(filepointer->h, 0, SEEK_CUR);
}

long lof(file *filepointer) {
    if (filepointer->h == HFILE_ERROR) return 0;
    long i = (long) ::lseek(filepointer->h, 0, SEEK_CUR);
    long i2 = (long) ::lseek(filepointer->h, 0, SEEK_END);
    ::lseek(filepointer->h, i, SEEK_SET);
    return i2;
}

long loadfile_FILESIZE;

void *loadfile(LPCSTR name) {
    // NOTE: behavior preserved verbatim from the Win32 backend, including the
    // long-standing inverted success check (returns NULL when the open
    // succeeds). Do not "fix" here — that would be a behavior change.
    file *f = open(name);
    if (f->h != HFILE_ERROR) return NULL;
    loadfile_FILESIZE = lof(f);
    void *v = malloc(loadfile_FILESIZE);
    get(f, v, loadfile_FILESIZE);
    close(f);
    return v;
}

void waitforfile(LPCSTR name) {
    // Win32 spun on OF_SHARE_EXCLUSIVE | OF_READWRITE until it could open the
    // file. POSIX share semantics differ; the host's intent is "block until
    // the file is present/openable", so retry an O_RDWR open. Resolve case
    // each iteration so the file is found once it appears under any case.
    for (;;) {
        char nb[1024];
        int fd = ::open(u6o_realpath(name, nb, sizeof nb), O_RDWR);
        if (fd >= 0) {
            ::close(fd);
            return;
        }
    }
}

void deletefile(LPCSTR name) {
    char nb[1024];
    ::remove(u6o_realpath(name, nb, sizeof nb));
}

#endif // _WIN32
