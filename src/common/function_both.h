#ifndef FUNCTION_BOTH_H
#define FUNCTION_BOTH_H
/* To be able to include this and the link link against the object file
 * only prototypes and externs go in here, and MAYBE some inline functions 
 * actual global variable definitions should be moved to u6o7.cpp itself. (#include "global_vars.h")
 */
#ifdef _WIN32
#include <winsock2.h>
#else
#include "platform/platform.h" // LH-P3: BSD sockets + Win32 type/thread/time shims
#endif
#include "data_both.h"
#include "math.h"

/* external global variables section */
extern unsigned long BITSleftmask[33]; //mask of index-many bits to keep
extern unsigned char OBJGETDIR_FRAME;

/* function headers */
ATOM MyRegisterClass(HINSTANCE hInstance);

BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void function_both_init(void);

void BITSadd(txt *t, unsigned long *start, unsigned long value, unsigned long nbits);

unsigned long BITSget(txt *t, unsigned long *start, unsigned long nbits);

void LOGadd(txt * t);

DWORD WINAPI sockets_send(LPVOID i);

DWORD WINAPI sockets_receive(LPVOID i);

DWORD WINAPI sockets_disconnect(LPVOID i);

bool NET_send(void *s, unsigned long d, txt *t);

void getscreenoffset(long x, long y, long *mapx, long *mapy);

void getscreenoffset_legacy(long x, long y, long *mapx, long *mapy);

// MDD: shared map-download primitives (host computes, client verifies).
//
// Content checksum is FNV-1a/32 over the raw file bytes -- cheap, allocation
// free, and identical on the Win32 client and the i386 Linux host (both
// little-endian, 32-bit). The streaming form lets the client fold each
// arriving chunk into a running hash and lets it hash the two non-contiguous
// in-memory arrays (index then type) without concatenating them, while the
// one-shot form is what the host uses on the baked .bin files. The streaming
// functions obey  MAP_checksum(d,n) == final(update(init(), d, n)).
unsigned long MAP_checksum_init(void);
unsigned long MAP_checksum_update(unsigned long state, const void *data, unsigned long len);
unsigned long MAP_checksum_final(unsigned long state);
unsigned long MAP_checksum(const void *data, unsigned long len);

// Returns the host-side source / client-side fallback path (".\\dr\\bt.bin",
// ".\\dr\\objfixed.bin", ".\\dr\\tobjfix.bin") for a MAP_FILE_* id, or NULL
// if the id is out of range.
const char *MAP_file_path(int fileId);

// ROOMSYNC-P1: Global isolated-room registry. See docs/rendering/global-room-sync.md
// for the design and rationale.
//
// An "isolated room" is any rectangular world region that is logically
// disconnected from the surrounding map but lives in the same 2048x1024 bt[]
// / od[] grid. Examples: the Guardian Guild basement at x=1280..1291,
// y=319..333. Such a room sits in a stretch of world coords that the host's
// mover/sobj transmit windows would otherwise overlap into the neighbouring
// open map -- pulling foreign NPCs, items, and base tiles into the room
// view and silently desyncing the per-player mover/sobj buffers.
//
// Both helpers are pure / read-only; they consult a static table and can be
// called freely from host and client.
struct GameRoom {
    long x0, y0; // inclusive top-left world coords
    long x1, y1; // inclusive bottom-right world coords
};

// If (x, y) is inside a registered isolated room, returns 1 and writes the
// room's bounds into the out params (any may be NULL). Returns 0 if (x, y)
// is in the open world (overworld, gargoyle lands, or unregistered area).
int getroom(long x, long y, long *rx0, long *ry0, long *rx1, long *ry1);

// Returns 1 if (ax, ay) and (bx, by) are in the SAME isolated room, OR if
// both are in the open world. Returns 0 if they're in different rooms or if
// one is inside an isolated room and the other is not. This is the single
// authoritative predicate used by the host fill loops, the client tile
// renderer, and the auto-resync trigger to decide whether two tiles should
// be considered "visible" from each other.
int sameroom(long ax, long ay, long bx, long by);

//getnbits returns the number of bits required to store n combinations
//if combinations is 1, getnbits returns 0 (only 1 combination requires 0 bits represent)
//if combinations is 2, getnbits returns 1
//unsigned char getnbits(unsigned long combinations);

// luteijn: basically this does (x?log2(x):0) BUT: if answer is not integer, we
// have to round up, not truncate or down.  can't just truncate and add 1,
// since that messes up exact answers and we lose combatibility with the real
// client...  since we already link the maths lib anyway, we might as well just
// put in a macro here, since this is used extensively when decoding the update
// messages.
#define getnbits(x) ((unsigned char) ceil(x?((logf((float)x)/logf(2.0))):0))
// the way this is used, this makes sense. FIXME still ugly.

unsigned char objgetdir(unsigned short type);

void txtaddcolouredname(txt * t, txt * name, player * p);


void SHA2(txt * t, txt * t2);


// c111
unsigned long new1_getexprequired(unsigned short level);

unsigned long new1_getexprequired(npc * npc);


#endif /* FUNCTION_BOTH_H */