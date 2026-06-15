#ifndef FUNCTION_CLIENT_H
#define FUNCTION_CLIENT_H
/* To be able to include this and then link against the object file
 * only prototypes and externs go in here, and MAYBE some inline functions 
 * actual global variable definitions should be moved to u6o7.cpp itself. (#include "global_vars.h")
 */
#include "data_client.h"
#include "function_both.h"
#include "viewport.h" // RW-P2: backbufferW/H, lightingStride, recreateBackbuffers
#include "ui_layout.h" // RW-P3: UiAnchor/UiPlacement, RepositionAnchoredPanels

/* external global variables */
//GetInput variables
//tab_pressed allows program to trap the tab key
//once trapped it also counts as an enterpressed, so serves a dual purpose
//otherwise tab key inserts an undefined amount of spaces
//it MUST be set after a call to getinput_setup
extern unsigned char GETINPUT_tab_pressed;
extern txt *GETINPUT_txt;
extern unsigned char *GETINPUT_enterpressed;
extern txt *GETINPUT_old; //used to detect new pointers
extern unsigned long GETINPUT_maxlength; //maximum length of GETINPUT_txt (0=infinite)
extern long gs_i; //getspr static data
extern long gs_i2; //getspr static data
extern long gs_x; //getspr static data
extern long gs_y; //getspr static data
extern long gs_t; //getspr static data
extern unsigned long GSs;
extern unsigned long GSx;
extern unsigned long GSy;
extern unsigned char midikeyboard2[256]; //reverse of midikeybaord array!
extern unsigned char midikeyboard2_keyon[256]; //whether key is being held or not
extern short midikeyboard_set;
extern unsigned char musickeyboard_set;
extern unsigned char midikeystack[16][256];
extern float midikeywait[16][256];
extern unsigned char clientinstrument;
extern unsigned char playinstrument;
extern unsigned char midipause;
extern unsigned char getsound_MOVERSOUND;
extern unsigned char AMBIENTLIGHT_LIGHTVALUE;
extern unsigned char AMBIENTLIGHT_SHOWSUN;
extern txt *STATUSMESSprev[8]; //the previous 8 status messages are stored here
extern txt *STATUSMESSdisplaying; //the message currently being displayed
extern float STATUSMESSwait;
extern unsigned char STATUSMESSskipok; //the message will be skipped if any messages are pending
extern txt *STATUSMESSt; //temp txt for building messages (included to aid conversion from older system)
extern txt *STATUSMESSpending;
extern txt *GETSETTING_RAW; //the actualt text between the square brackets [...]
extern txt *li2_t;

void applyscaleuipanelwidget(int uipi, int uiwi, int uisi, float scalex, float scaley);

int gethituipaneli(int x, int y);

int gethituipanelwidgeti(int x, int y, int uipaneli);

int gethituipanelwidgeti(int x, int y);

int testhituipanel(int x, int y, int uipaneli);

// s333 get type of object at coordinate
void backupplayermvinfon1(player * tplayer);

//int isobjonscreenn1(int mapxn, int mapyn, int tpxn, int tpyn);
int getobjtypen1(player *tplayer, int mapx, int mapy);

//int getobjtypen1b(player p, int mapx, int mapy);
int getobjtypen1b(player p, int mapx, int mapy, int exactcoord);

int checkdeadobjtypen1(int typeparam);

int convertdeadobjtypen1(int typeparam);

int checkobjtypecreaturen1(int typeparam);

// s333 get description of object type (for creature)
void getobjdescn1(txt *txtdesc, int typeparam);

// s555
void updateoptioninfo();


// RW-P3.3 (2026-06-02): one-shot, on-screen first placement for the floating
// hideable panels (minimap_frame, tmap_frame, party_spellbook_frame[]).
// Clamps the requested "home" (default or cltset2-restored) fully inside the
// current back buffer, then parks the panel at its home (shown!=0) or in its
// hidden slot home+kPanelHideDeltaX (shown==0), and sets FRAME::positioned so
// the placement only happens once. Guarantees the panel is fully visible the
// first time it appears on a resized window. See frame.h FRAME::positioned.
void placeFloatingPanelFirstShow(FRAME *f, int homeX, int homeY, int shown);


/* function prototypes */
void function_client_init(void);


//for compatibility the client supports this function
object *OBJnew_local();

void GETINPUT_setup(txt *input_pointer, void *enterpressed_pointer, unsigned long maxlength);

txt *GETINPUT_current();

void GETINPUT_stop();

// RW-P2.3-asm: All five sprite-blit inline functions have been rewritten
// as C++ loops that read the destination row stride from d->d.lPitch at
// runtime.  The old implementations used `y*2048` and the included
// fast*.asm bodies hard-coded `+2048` between destination rows, which
// assumed ps->d.lPitch == 2048 (i.e. a 1024-pixel-wide back-buffer).
// The new C++ versions work for any back-buffer width.
//
// Source sprite layouts (unchanged from the original asm bodies):
//   g32 / g32z : bt8 sprite sheet, HIRES mode, source pitch = s->d.lPitch
//                (512 bytes = 256 px wide), 32 source rows → 32 dest rows 1:1.
//   sf32 / sf32z: sfx8 sprite sheet, source pitch = s->d.lPitch
//                (4096 bytes = 2048 px wide), 16 source rows → 32 dest rows
//                (2× vertical pixel-doubling).
//   im32z      : spr8/spr84 sequential sprites, source stride = 64 bytes
//                (32 px wide at 2 bpp), 32 source rows → 32 dest rows 1:1.

// g32 — opaque basetile blit (OPTION_HIRES: source stride = s->d.lPitch,
//        32 source rows → 32 dest rows, 1:1, no transparency).
inline void g32(surf *d, unsigned long x, unsigned long y, surf *s, unsigned long i) {
    const unsigned long srcPitch = (unsigned long) s->d.lPitch;
    const unsigned long dstPitch = (unsigned long) d->d.lPitch;
    const unsigned char *src = (const unsigned char *) s->o
                               + ((i / 8) * srcPitch * 32) + ((i & 7) * 64);
    unsigned char *dst = (unsigned char *) d->o + x * 2 + y * dstPitch;
    for (int row = 0; row < 32; row++) {
        const unsigned long *sw = (const unsigned long *) src;
        unsigned long *dw = (unsigned long *) dst;
        for (int dw_i = 0; dw_i < 16; dw_i++) dw[dw_i] = sw[dw_i];
        src += srcPitch;
        dst += dstPitch;
    }
}

// g32z — transparent dirt-overlay blit (same source layout as g32,
//         skips pixels where colour == 0).
inline void g32z(surf *d, unsigned long x, unsigned long y, surf *s, unsigned long i) {
    const unsigned long srcPitch = (unsigned long) s->d.lPitch;
    const unsigned long dstPitch = (unsigned long) d->d.lPitch;
    const unsigned char *src = (const unsigned char *) s->o
                               + ((i / 8) * srcPitch * 32) + ((i & 7) * 64);
    unsigned char *dst = (unsigned char *) d->o + x * 2 + y * dstPitch;
    for (int row = 0; row < 32; row++) {
        const unsigned short *sw = (const unsigned short *) src;
        unsigned short *dw = (unsigned short *) dst;
        for (int px = 0; px < 32; px++) {
            unsigned short v = sw[px];
            if (v) dw[px] = v;
        }
        src += srcPitch;
        dst += dstPitch;
    }
}

// sf32 — opaque sprite blit from sfx8 with 2× vertical pixel-doubling.
//         Source: 16 rows at 2*srcPitch intervals → 32 dest rows (each source
//         row written to two consecutive dest rows).
inline void sf32(surf *d, unsigned long x, unsigned long y, surf *s, unsigned long i) {
    const unsigned long srcPitch = (unsigned long) s->d.lPitch;
    const unsigned long dstPitch = (unsigned long) d->d.lPitch;
    const unsigned char *src = (const unsigned char *) s->o
                               + ((i / 32) * srcPitch * 32) + ((i & 31) * 64);
    unsigned char *dst = (unsigned char *) d->o + x * 2 + y * dstPitch;
    for (int row = 0; row < 16; row++) {
        const unsigned long *sw = (const unsigned long *) src;
        unsigned long *dw0 = (unsigned long *) dst;
        unsigned long *dw1 = (unsigned long *) (dst + dstPitch);
        for (int dw_i = 0; dw_i < 16; dw_i++) {
            unsigned long v = sw[dw_i];
            dw0[dw_i] = v;
            dw1[dw_i] = v;
        }
        src += 2 * srcPitch;
        dst += 2 * dstPitch;
    }
}

// sf32z — transparent sprite blit from sfx8 with 2× vertical pixel-doubling.
//          Same layout as sf32; skips pixels where colour == 0.
inline void sf32z(surf *d, unsigned long x, unsigned long y, surf *s, unsigned long i) {
    const unsigned long srcPitch = (unsigned long) s->d.lPitch;
    const unsigned long dstPitch = (unsigned long) d->d.lPitch;
    const unsigned char *src = (const unsigned char *) s->o
                               + ((i / 32) * srcPitch * 32) + ((i & 31) * 64);
    unsigned char *dst = (unsigned char *) d->o + x * 2 + y * dstPitch;
    for (int row = 0; row < 16; row++) {
        const unsigned short *sw = (const unsigned short *) src;
        unsigned short *dw0 = (unsigned short *) dst;
        unsigned short *dw1 = (unsigned short *) (dst + dstPitch);
        for (int px = 0; px < 32; px++) {
            unsigned short v = sw[px];
            if (v) {
                dw0[px] = v;
                dw1[px] = v;
            }
        }
        src += 2 * srcPitch;
        dst += 2 * dstPitch;
    }
}

// im32z — transparent sprite blit from spr8/spr84 sequential storage.
//          Source: sprite i at byte offset i*2048, stride 64 bytes (32 px × 2 bpp),
//          32 source rows → 32 dest rows 1:1.  Skips pixels where colour == 0.
inline void im32z(surf *d, unsigned long x, unsigned long y, surf *s, unsigned long i) {
    const unsigned long dstPitch = (unsigned long) d->d.lPitch;
    const unsigned char *src = (const unsigned char *) s->o + i * 64 * 32;
    unsigned char *dst = (unsigned char *) d->o + x * 2 + y * dstPitch;
    for (int row = 0; row < 32; row++) {
        const unsigned short *sw = (const unsigned short *) src;
        unsigned short *dw = (unsigned short *) dst;
        for (int px = 0; px < 32; px++) {
            unsigned short v = sw[px];
            if (v) dw[px] = v;
        }
        src += 64; // 32 px × 2 bpp = 64 bytes per source row
        dst += dstPitch;
    }
}

void getspr(object * obj); //loads bt32 with a spr (uses keyframe) FIXME: candidate for optimising


bool keyhit(unsigned short k);

bool u6okeyhit(unsigned char i); //returns TRUE if key has been pressed
bool u6okeyon(unsigned char i); //returns TRUE if key has been pressed

void u6okeyseton(unsigned char i);

void u6okeysetoff(unsigned char i);

void GETINPUT_update();

void mididown(unsigned char instrument, unsigned char key);

void midiup(unsigned char instrument, unsigned char key);

void LIGHTnew(unsigned short x, unsigned short y, unsigned long light_data_offset, unsigned short x_axis_size);

//LIGHTnew has inline assembly FIXME

unsigned short objgettype(unsigned short type, unsigned char dir, unsigned char frm);

unsigned char objgetnextframe(unsigned short type, unsigned char frm);

void getsound(unsigned short type, long x, long y);

void getlight(unsigned short type, long x, long y);

//portraits 2.0 functions
void loadportrait(unsigned short i, surf *s);

surf *getportrait(unsigned short i);

surf *getportrait_doublesize(unsigned short i);

surf *getportrait_halfsize(unsigned short i);

unsigned char getambientlight(unsigned short x, unsigned short y);

void txtmakeu6ocompatible(txt * t); /* filters out nasty characters that would mess up things */
void STATUSMESSadd(txt * t);

void STATUSMESSadd(const char *t);

// s555
void STATUSMESSadd(txt *t, int skippable);

void STATUSMESSadd(const char *t, int skippable);

void STATUSMESSadd(const char *t, int skippable, int num);

// Word-wrap one already-built status line `src` to a pixel width of
// `maxwidth`, measuring with the fnt1naa font the status log is drawn with.
// Fills out[0..n-1] in reading order (out must hold at least `maxlines`
// pre-allocated txt*), hard-breaking any single word wider than maxwidth so
// progress is always made. Returns the number of physical lines produced
// (>=1, clamped to maxlines). Used by the draggable "view previous status
// message" log so text re-wraps to stay on-screen wherever the arrow sits.
int STATUSMESSwrapline(txt *src, long maxwidth, txt **out, int maxlines);


long getsetting(const char *d);

// Rewrite settings.txt in place, replacing/appending one
// `{NAME, [VALUE]}` integer entry. See implementation in
// function_client.cpp for the failure model and the recommended
// read-back idiom (clear GETSETTING_RAW first, then call getsetting,
// then test GETSETTING_RAW->l). Used to persist session UI state such
// as WINDOW_MAXIMIZED / WINDOW_W / WINDOW_H / WINDOW_X / WINDOW_Y.
void setsetting_int(const char *name, long value);

void refresh(); // FIXME Inline assembly alert!

void scrlog(const char *d); //screen log

surf *loadimage2(LPCSTR name);

surf *loadimage2(txt * name);

surf *loadimage2(LPCSTR name, long flags);

surf *loadimage2(txt *name, long flags);

#endif /* FUNCTION_CLIENT_H */