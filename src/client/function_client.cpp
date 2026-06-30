#include "function_client.h"
#include "define_client.h"
#include "viewport.h" // RW-P2: backbufferW/H, lightingStride, lightingTotalBytes
//#include <windows.h>
#pragma warning(disable: 4018 4244)
void function_client_init(void) {
    GETINPUT_tab_pressed = 0;
    GETINPUT_txt = NULL;
    GETINPUT_enterpressed = NULL;
    GETINPUT_old = NULL; //used to detect new pointers
    GSs = 0;
    GSx = 0;
    GSy = 0;
    midikeyboard_set = -1;
    musickeyboard_set = 255;
    clientinstrument = 0;
    playinstrument = 0;
    midipause = 0;
    getsound_MOVERSOUND = 0;
    AMBIENTLIGHT_LIGHTVALUE = 15;
    AMBIENTLIGHT_SHOWSUN = FALSE;
    STATUSMESSdisplaying = txtnew(); //the message currently being displayed
    STATUSMESSwait = 0.0f;
    STATUSMESSskipok = 0; //the message will be skipped if any messages are pending
    STATUSMESSt = txtnew(); //temp txt for building messages (included to aid conversion from older system)
    STATUSMESSpending = txtnew();
    GETSETTING_RAW = txtnew(); //the actualt text between the square brackets [...]
    li2_t = txtnew();
};

//for compatibility the client supports this function
object *OBJnew_local() {
    static object *newobject;
    newobject = (object *) malloc(sizeof(object));
    ZeroMemory(newobject, sizeof(object));
    return newobject;
}

//GetInput functions
//tab_pressed allows program to trap the tab key
//once trapped it also counts as an enterpressed, so serves a dual purpose
//otherwise tab key inserts an undefined amount of spaces
//it MUST be set after a call to getinput_setup
void GETINPUT_setup(txt *input_pointer, void *enterpressed_pointer, unsigned long maxlength) {
    GETINPUT_txt = input_pointer;
    GETINPUT_enterpressed = (unsigned char *) enterpressed_pointer;
    GETINPUT_maxlength = maxlength;
    GETINPUT_tab_pressed = 0;
}

txt *GETINPUT_current() {
    return GETINPUT_txt;
}

void GETINPUT_stop() {
    if (GETINPUT_enterpressed) {
        GETINPUT_enterpressed[0] = TRUE;
        GETINPUT_enterpressed = NULL;
    }
    GETINPUT_txt = NULL;
}

#include "new_getspr.inc"

bool keyhit(unsigned short k) {
    //returns TRUE if key has been pressed
    if (key[k] == FALSE) return FALSE;
    key[k] = FALSE;
    return TRUE;
}

bool u6okeyhit(unsigned char i) {
    //returns TRUE if key has been pressed
    if (U6OK[i][0]) {
        if (U6OK[i][1] == 0) {
            //only single key required
            if (key[U6OK[i][0]] == FALSE) return FALSE;
            key[U6OK[i][0]] = FALSE;
            return TRUE;
        }
        //double key combo?
        if (keyon[U6OK[i][0]]) {
            if (key[U6OK[i][1]] == FALSE) return FALSE;
            key[U6OK[i][1]] = FALSE;
            return TRUE;
        } //first key held
    } //exists
    return FALSE;
}

bool u6okeyon(unsigned char i) {
    //returns TRUE if key has been pressed
    if (U6OK[i][0]) {
        if (U6OK[i][1] == 0) {
            //only single key required
            if (keyon[U6OK[i][0]]) return TRUE;
            return FALSE;
        }
        //double key combo?
        if (keyon[U6OK[i][0]]) {
            if (keyon[U6OK[i][1]]) return TRUE;
        } //first key held
    } //exists
    return FALSE;
}

void u6okeyseton(unsigned char i) {
    if (U6OK[i][0]) {
        //exists
        if (U6OK[i][1] == 0) {
            //only single key required
            keyon[U6OK[i][0]] = 1;
        }
        //double key combo?
        keyon[U6OK[i][0]] = 1;
        keyon[U6OK[i][1]] = 1;
    } //exists
}

void u6okeysetoff(unsigned char i) {
    if (U6OK[i][0]) {
        //exists
        if (U6OK[i][1] == 0) {
            //only single key required
            keyon[U6OK[i][0]] = 0;
        }
        //double key combo?
        keyon[U6OK[i][0]] = 0;
        keyon[U6OK[i][1]] = 0;
    } //exists
}

void u6o_ensure_control_defaults() {
    if (U6OK[U6OK_CAMERATOGGLE][0] == 0) {
        U6OK[U6OK_CAMERATOGGLE][0] = VK_TAB;
    }
    if (U6OK[U6OK_RESPAWNFOLLOWERS][0] == 0) {
        U6OK[U6OK_RESPAWNFOLLOWERS][0] = VK_SHIFT;
        U6OK[U6OK_RESPAWNFOLLOWERS][1] = VK_F1;
    }
}

void GETINPUT_update() {
    static long i, i2;
    if (GETINPUT_txt != NULL) {
        //GETINPUT active, trap ascii keys
        if (GETINPUT_txt != GETINPUT_old) {
            for (i = 0; i <= 65535; i++) { keyasc[i] = FALSE; } //clear ascii buffer
        }

        //check for PASTE
        if (keyon[VK_SHIFT]) {
            if (keyhit(VK_INSERT)) {
                goto getinput_paste;
            }
        }
        if (keyon[VK_CONTROL]) {
            keyasc[86] = FALSE;
            keyasc[118] = FALSE;
            if (keyhit(86)) {
            getinput_paste:
                static unsigned char *getinput_clipboardtxt;
                if (OpenClipboard(NULL)) {
                    getinput_clipboardtxt = (unsigned char *) GetClipboardData(CF_TEXT);
                    if (getinput_clipboardtxt) {
                        i = 0;
                    getinput_paste_next:
                        if (i2 = getinput_clipboardtxt[i]) {
                            if ((i2 >= 32) && (i2 <= 126)) {
                                if ((GETINPUT_txt->l < GETINPUT_maxlength) || GETINPUT_maxlength == 0) {
                                    txtNEWLEN(GETINPUT_txt, GETINPUT_txt->l + 1);
                                    GETINPUT_txt->d[GETINPUT_txt->l - 1] = i2;
                                }
                            }
                            i++;
                            goto getinput_paste_next;
                        }
                    }
                    CloseClipboard();
                }
                for (i = 0; i <= 65535; i++) {
                    if ((i < 0x25) || (i > 0x28)) {
                        if (i != VK_CONTROL) {
                            if (i != VK_SHIFT) {
                                key[i] = FALSE;
                                keyon[i] = FALSE;
                            }
                        }
                    }
                } //clear pressed key buffers
            }
        }


        for (i = 32; i <= 126; i++) {
            if (keyasc[i]) {
                keyasc[i] = FALSE;
                if ((GETINPUT_txt->l < GETINPUT_maxlength) || GETINPUT_maxlength == 0) {
                    txtNEWLEN(GETINPUT_txt, GETINPUT_txt->l + 1);
                    GETINPUT_txt->d[GETINPUT_txt->l - 1] = i;
                }
                for (i2 = 0; i2 <= 65535; i2++)
                    if ((i2 < 0x25) || (i2 > 0x28)) {
                        key[i2] = FALSE;
                        keyon[i2] = FALSE;
                    } //clear pressed key buffers
            }
        }
        if (keyasc[8]) {
            keyasc[8] = FALSE;
            if (GETINPUT_txt->l != 0) txtNEWLEN(GETINPUT_txt, GETINPUT_txt->l - 1);
            for (i = 0; i <= 65535; i++)
                if ((i < 0x25) || (i > 0x28)) {
                    key[i] = FALSE;
                    keyon[i] = FALSE;
                } //clear pressed key buffers
        }

        if (keyasc[9]) {
            //tab
            keyasc[9] = FALSE;
            if (GETINPUT_tab_pressed == GETINPUT_TAB_PRESSED_INIT) {
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED;
                goto getinput_tabreturn;
            }
            if (GETINPUT_tab_pressed == GETINPUT_TAB_PRESSED_STOP) {
                for (i = 1; i <= 8; i++) {
                    if ((GETINPUT_txt->l < GETINPUT_maxlength) || GETINPUT_maxlength == 0) {
                        txtNEWLEN(GETINPUT_txt, GETINPUT_txt->l + 1);
                        GETINPUT_txt->d[GETINPUT_txt->l - 1] = 32;
                    }
                } //i
            }
            for (i = 0; i <= 65535; i++)
                if ((i < 0x25) || (i > 0x28)) {
                    key[i] = FALSE;
                    keyon[i] = FALSE;
                } //clear pressed key buffers
        } //tab

        if (u6okeyhit(U6OK_TALK_DONE)) {
        getinput_tabreturn:
            GETINPUT_txt = NULL;
            if (GETINPUT_enterpressed != NULL) {
                GETINPUT_enterpressed[0] = TRUE;
                GETINPUT_enterpressed = NULL;
            }
            for (i = 0; i <= 65535; i++)
                if ((i < 0x25) || (i > 0x28)) {
                    key[i] = FALSE;
                    keyon[i] = FALSE;
                } //clear pressed key buffers
        }
    }
    GETINPUT_old = GETINPUT_txt;
}


void mididown(unsigned char instrument, unsigned char key) {
    if (midiout_setup) midiOutShortMsg(midiout_handle, 0x007E0090 + key * 256 + instrument);

    if (midipause == 0) {
        midipause = 1;
        if (U6O_DISABLEMUSIC == FALSE) {
            u6omidi->Stop();
        }
    }

    midikeystack[instrument][key]++;
    midikeywait[instrument][key] = ett;
}

void midiup(unsigned char instrument, unsigned char key) {
    if (key == 255) return;


    if (midikeystack[instrument][key]) {
        midikeystack[instrument][key]--;
        if (midikeystack[instrument][key] == 0) {
            if (instrument == 3)
                if (midiout_setup)
                    midiOutShortMsg(midiout_handle, 0x00000080 + key * 256 + instrument);
        } //midikeystack[instrument][key]==0
    } //midikeystack[instrument][key]
} //midiup

void LIGHTnew(unsigned short x, unsigned short y, unsigned long light_data_offset, unsigned short x_axis_size) {
    if (timelval == 0) return;
    // RW-P2.3: read back-buffer dims through the viewport.h accessors so this
    // function tracks the active back-buffer size when it eventually becomes
    // dynamic. Today the accessors return the legacy 1024/768 constants and
    // behavior is identical to the previous hard-coded literals.
    static long asm_copy_vc_bytesx, asm_copy_vc_sourceoffset, asm_copy_vc_destoffset, asm_copy_vc_sourceskip,
            asm_copy_vc_destskip, asm_copy_vc_rows;
    static long x2, y2, x3, y3, x4, y4;
    static long xoff, yoff;
    const long bbW = backbufferW();
    const long bbH = backbufferH();
    const long lsStride = lightingStride();
    xoff = x - x_axis_size / 2;
    xoff <<= 5;
    yoff = y - x_axis_size / 2;
    yoff <<= 5;
    x_axis_size <<= 5;
    x2 = xoff; //start dest x offset
    x3 = 0; //start source x offset
    x4 = x_axis_size; //displayed "pixels" of x axis
    if (xoff < 0) {
        x4 += xoff;
        x2 = 0;
        x3 = -xoff;
    }
    if ((xoff + x_axis_size) > bbW) x4 -= xoff + x_axis_size - bbW;
    y2 = yoff; //starting dest y offset
    y3 = 0; //starting source y offset
    y4 = x_axis_size; //rows on screen
    if (yoff < 0) {
        y4 += yoff;
        y2 = 0;
        y3 = -yoff;
    }
    if ((yoff + x_axis_size) > bbH) y4 -= yoff + x_axis_size - bbH;
    asm_copy_vc_bytesx = x4;
    asm_copy_vc_sourceskip = x_axis_size - asm_copy_vc_bytesx;
    asm_copy_vc_destskip = lsStride - asm_copy_vc_bytesx;
    asm_copy_vc_sourceoffset = y3 * x_axis_size + light_data_offset + x3;
    asm_copy_vc_destoffset = (y2 * lsStride) + (unsigned long) ls + x2;
    asm_copy_vc_rows = y4;
    _asm{
            push esi
            push edi
            push ebx
            mov ecx,asm_copy_vc_rows
            mov edx,asm_copy_vc_bytesx
            mov esi,asm_copy_vc_sourceoffset
            mov edi,asm_copy_vc_destoffset
            asm_light1:
            mov ebx,esi
            add ebx,edx
            asm_light0:
            mov eax,[esi]
            add [edi],eax
            add esi,4
            add edi,4
            cmp esi,ebx
            jne asm_light0
            add edi,asm_copy_vc_destskip
            add esi,asm_copy_vc_sourceskip
            dec ecx
            jnz asm_light1
            pop ebx
            pop edi
            pop esi
            } //_asm
} //LIGHTnew


// =====================================================================
// RW-P2.2: backbuffer recreation. Called from the dirtyClientSize
// handler in loop_client.cpp when the user resizes the window. Releases
// and re-creates the `ps`/`ps5`
// DirectDraw surfaces, re-allocates the lighting buffers, patches FRAME
// pointers (vf, fs) that referenced the old `ps`, and clears the new
// surface to black so unrendered regions don't show stale pixels.
//
// RW-P2.3-asm (2026-05-21) — HORIZONTAL GROWTH UNBLOCKED
// sf32, sf32z, im32z (and g32, g32z) have been rewritten in function_client.h
// as C++ loops that read d->d.lPitch at runtime, replacing the hard-rolled
// inline-ASM bodies (fast3/4/5.asm etc.) that hard-coded +2048 between dest
// rows. The newW > kBackbufferLegacyW clamp is now removed; the back-buffer
// can grow horizontally up to kBackbufferMaxW.
//
// Height growth is also re-enabled. The world-tile renderer only writes into
// the upper kBackbufferLegacyH rows; the new outer region starts as black
// (cleared by cls() below) and remains black until RW-P4 expands the world
// view. The lighting/stormcloak passes use lightingTotalBytes() / lightingStride()
// so they follow any height change already.
// =====================================================================
namespace u6o {
    namespace client {
        bool recreateBackbuffers(int newW, int newH) {
            // Clamp to [kBackbufferLegacy*, kBackbufferMax*].
            if (newW < kBackbufferLegacyW) newW = kBackbufferLegacyW;
            if (newH < kBackbufferLegacyH) newH = kBackbufferLegacyH;
            if (newW > kBackbufferMaxW) newW = kBackbufferMaxW;
            if (newH > kBackbufferMaxH) newH = kBackbufferMaxH;

            if (newW == backbufferW() && newH == backbufferH()) {
                return true; // already at requested size
            }

            // Capture the OLD ps pointer so we can patch only FRAMEs that
            // actually referenced it. fs (the full-screen intro/menu overlay)
            // toggles its graphic between ps (during intros/menus) and NULL
            // (gameplay). Re-attaching ps unconditionally during gameplay
            // re-shows fs with offset_x=1024, and since fs->graphic == ps,
            // the FRAME display loop copies ps onto itself shifted right by
            // 1024 pixels — duplicating the rendered world side-by-side. See
            // loop_client.cpp:2005,2203,2348 + loop_client.inc:938,1136,1281
            // for the gameplay-time `fs->graphic = NULL` assignments.
            surf *old_ps = ps;

            // Release the old surfaces. `free(surf*)` calls ->Release() on
            // the IDirectDrawSurface and frees the wrapper struct.
            // NOTE: ps5 is intentionally NOT resized. It is used solely as a
            // 768x768 scratch surface (24 tiles * 32px) for the minimap / gem
            // peer view. The downstream call `img(ps6, ps5)` (and the analogous
            // `img(minimaptilesurf, ps5)`) resamples the ENTIRE ps5 surface into
            // a fixed-size destination cell, and the 60-px-per-cell layout in
            // minimap_surf_new / minimap_frame->graphic only lines up when ps5
            // has the original 1024x768 dimensions:
            //     ps6   80x60: 768/1024 * 80 = 60, 768/768 * 60 = 60
            //     mtile 80x60 (or 160x120 for minimaptype==1): same ratio
            // Growing ps5 with the backbuffer shrinks the 24x24-tile content to
            // only a fraction of each destination cell, producing the black gaps
            // visible when a player uses a Gem on a resized window. Keeping ps5
            // at its original size restores the legacy behavior on all window
            // sizes (ps5 is never sampled by the main world view).
            if (ps) {
                free(ps);
                ps = NULL;
            }

            // Re-create at the new size. Match the original setup_client.inc
            // flag choice: ps is 16bpp sysmem. (MPRES-P2.3b retired the 32-bpp
            // ps3 conversion helper — the modern presenter samples ps->o's
            // RGB565 directly, so no per-display-format helper surface is needed.)
            ps = newsurf(newW, newH, SURF_SYSMEM16);

            if (!ps) {
                return false; // allocation failure — leave dims at old values
            }

            // Pitch sanity check: in debug builds, log if DD's reported row
            // stride doesn't match `newW * 2` bytes. The original distortion
            // bug was caused by the lighting buffer (`malloc`'d at exactly
            // `newW * newH` bytes) and the surface's row stride disagreeing at
            // widths DirectDraw pads above `newW * 2`.
#ifdef _DEBUG
            {
                long expectedPitch = (long) newW * 2;
                if ((long) ps->d.lPitch != expectedPitch) {
                    char dbg[160];
                    wsprintfA(dbg,
                              "[u6o] recreateBackbuffers: dwWidth=%lu lPitch=%ld (expected %ld)\n",
                              (unsigned long) ps->d.dwWidth, (long) ps->d.lPitch, expectedPitch);
                    OutputDebugStringA(dbg);
                }
            }
#endif

            // RW-P2.3: adopt the surface's ACTUAL pixel pitch as the lighting
            // stride before (re)allocating the lighting buffers. DirectDraw pads
            // `ps->d.lPitch` above `newW * 2` at widths that aren't aligned to
            // its granularity (i.e. most intermediate window sizes between the
            // legacy 1024 and a maximized/aligned width). The lighting-compose
            // passes (lightshow0 asm, moon memcpy, stormcloak, ps_fakebuffer)
            // walk `ls`/`ps->o` linearly assuming row stride == lightingStride()
            // pixels, so they must use lPitch/2 — not newW — or the lit overlay
            // skews diagonally (top-right → bottom-left). Earlier attempts that
            // made lPitch/2 the canonical *world* width broke the tile/wire
            // frame; decoupling the lighting stride from backbufferW() is the
            // fix. Set BEFORE lighting_alloc() so it sizes/strides to match.
            set_lighting_stride((int) (ps->d.lPitch / 2));

            // Re-allocate the lighting buffers at the surface pitch so the
            // linear-walk inline asm in the lighting compose pass at
            // loop_client.cpp:8294ff stays in lockstep with `ps->o`.
            if (!lighting_alloc(newW, newH) || !visibility_alloc(newW, newH)) {
                return false;
            }

            // Patch FRAME pointers that held a reference to the OLD ps.
            // Critical: only patch if the FRAME was actually pointing at the
            // old ps. fs->graphic is NULL during gameplay (the intro/menu
            // logic clears it when the overlay is dismissed); patching it
            // back to ps would revive a hidden overlay positioned at
            // offset_x=1024 and produce the "world rendered twice
            // side-by-side" duplication described in the resizable-window
            // bug.png.
            if (vf &&vf
            
            ->
            graphic == old_ps
            )
            vf->graphic = ps;
            if (fs &&fs
            
            ->
            graphic == old_ps
            )
            fs->graphic = ps;

            // Clear ps to black so the area outside the legacy 1024x768 (where
            // the world tile renderer still draws) starts clean. The world
            // overdraws the upper-left 1024x768 every frame; the new outer
            // region is never written by the renderer, so without this you'd
            // see whatever heap/DD memory happened to be there.
            cls(ps, 0);

            // Publish the new dims. Done last so any concurrent reader on the
            // same thread (we're single-threaded for graphics, but be careful
            // anyway) sees consistent state: surfaces are valid before
            // backbufferW()/H() return the new values.
            set_active_backbuffer_dims(newW, newH);
            return true;
        }
    }
} // namespace u6o::client


// s333 backup player mv info
void backupplayermvinfon1(player *tplayer) {
    combatinfoplayerprevinit = 1;
    combatinfoplayerprev.mv_i = tplayer->mv_i;
    for (int i = 0; i < tplayer->mv_i; i++) {
        if ((tplayer->mv_x[i] != tplayer->x) && (tplayer->mv_y[i] != tplayer->y)) {
            combatinfoplayerprev.mv_x[i] = tplayer->mv_x[i];
            combatinfoplayerprev.mv_y[i] = tplayer->mv_y[i];
            combatinfoplayerprev.mv_type[i] = tplayer->mv_type[i];
        } else {
            combatinfoplayerprev.mv_x[i] = 0;
            combatinfoplayerprev.mv_y[i] = 0;
            combatinfoplayerprev.mv_type[i] = 0;
        }
    }
}


// s333 get best-guess type of object at coordinate (if possible)
int getobjtypen1b(player p, int mapx, int mapy, int exactcoord) {
    int objtype = -1;
    int distx, disty; // , dist;
    int bestdistx = 6;
    int bestdisty = 6;
    //int bestdist = 100;
    int bestdisti = -1;

    resultinfon1 = 0;
    if (combatinfoplayerprevinit) {
        for (int i = 0; i < p.mv_i; i++) {
            if ((p.mv_x[i] == mapx) && (p.mv_y[i] == mapy)) {
                //objtype = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
                objtype = p.mv_type[i];
                break;
            }
        }

        if (objtype == 0)
            objtype = -1;
        else
            resultinfon1 = 1;

        if (!exactcoord && (objtype == -1)) {
            for (int i = 0; i < p.mv_i; i++) {
                distx = abs(p.mv_x[i] - mapx);
                disty = abs(p.mv_y[i] - mapy);

                if (((distx < bestdistx) && (disty <= bestdisty)) || ((distx <= bestdistx) && (disty < bestdisty))) {
                    //dist = distx + disty;
                    //if (dist < bestdist) {
                    //bestdist = dist;
                    bestdistx = distx;
                    bestdisty = disty;
                    bestdisti = i;
                }
            }

            if (bestdisti != -1) {
                objtype = p.mv_type[bestdisti];
                resultinfon1 = 2;
            }
        }
    }

    /*
	txt* t3 = txtnew();
	txtset(t3, "using best guess");
	LOGadd(t3);
	*/

    return objtype;
}

// s333 get type of object at coordinate
int getobjtypen1(player *tplayer, int mapx, int mapy) {
    int objtype = -1;

    resultinfon1 = 0;
    for (int i = 0; i < tplayer->mv_i; i++) {
        if ((tplayer->mv_x[i] == mapx) && (tplayer->mv_y[i] == mapy)) {
            //objtype = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
            objtype = tplayer->mv_type[i];
            resultinfon1 = 1;
            break;
        }
    }

    if (objtype == -1) {
        //mapx=tpx+x-1; mapy=tpy+y-1;
        int bufx = mapx - tplayer->sobj_bufoffx;
        int bufy = mapy - tplayer->sobj_bufoffy;

        if ((bufx >= 0) && (bufx < 96) && (bufy >= 0) && (bufy < 72)) {
            if (int i = tplayer->sobj_bufsize[bufx][bufy]) {
                objtype = tplayer->sobj[bufx][bufy][i - 1];
                resultinfon1 = 2;
            }

            if ((objtype == -1) && (mapx >= 0) && (mapx < 1024) && (mapy >= 0) && (mapy < 2048)) {
                if (int i = tobjfixed_index[mapx][mapy]) {
                    if ((tplayer->sobj_tempfixed[bufx][bufy] & 1) == 0)
                        if ((i >= 0) && (i < 65536))
                            if ((i + tobjfixed_type[i] >= 0) && (i + tobjfixed_type[i] < 65536))
                                objtype = tobjfixed_type[i + tobjfixed_type[i]];
                    resultinfon1 = 3;
                }
            }
        }

        /*
		if (objtype == -1) {
			if (int i = objfixed_index[mapx][mapy]) {
				objtype = objfixed_type[i + objfixed_type[i]];
			}
		}
		*/
    } //else {
    //if (obji[sprlnk[objtype & 1023]].v4 == 1) //"avatar"
    //objtype = OBJ_AVATAR;
    //}


    /*
	for (y = 0; y <= 25; y++) {
		for (x = 0; x <= 33; x++) {
			mapx = tpx + x - 1;
			mapy = tpy + y - 1;
			for (i = 0; i < tplayer->mv_i; i++) {
				if (tplayer->mv_x[i] == mapx) {
					if (tplayer->mv_y[i] == mapy) {
						x2 = x - 1; y2 = y - 1;
						x3 = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
						myobj->type = x3;

						//non-pass specific changes
						//if (tplayer->mv_flags[i] & MV_PARALYZE) keyframe = 0;
						//check if sleeping
						//if (tplayer->mv_flags[i] & MV_SLEEP) {
						z3 = myobj->type & 1023; z2 = 0;
						//	if (z3 == 376) z2 = 339 + 6 * 1024;
						//}
						txtset(t3, "[");
						txtnumint(t, tplayer->mv_type[i]);
						txtadd(t3, t);
						txtadd(t3, "] ");
						txtnumint(t, z3);
						txtadd(t3, t);
						txtadd(t3, ": ");
						txtnumint(t, x);
						txtadd(t3, t);
						txtadd(t3, ", ");
						txtnumint(t, y);
						txtadd(t3, t);
						txtadd(t3, " // ");
						txtnumint(t, mapx);
						txtadd(t3, t);
						txtadd(t3, ", ");
						txtnumint(t, mapy);
						txtadd(t3, t);
						txtadd(t3, " // ");
						txtnumint(t, sfx[i3].x);
						txtadd(t3, t);
						txtadd(t3, ", ");
						txtnumint(t, sfx[i3].y);
						txtadd(t3, t);
						txtadd(t3, " // ");
						LOGadd(t3);
					}
				}
			}
		}
	}
	*/

    return objtype;
}

// s333
int checkdeadobjtypen1(int typeparam) {
    int objtypedead = 0;
    int type;

    if (typeparam >= 0)
        type = typeparam & 1023;
    else
        type = typeparam;

    switch (type) {
        case -1:
            objtypedead = 0;
            break;
        case OBJ_DEAD_ANIMAL:
        case OBJ_DEAD_BODY:
        case OBJ_DEAD_CYCLOPS:
        case OBJ_DEAD_GARGOYLE:
        case OBJ_DEAD_GIANT_RAT:
        case OBJ_DEAD_GIANT_BAT:
        case OBJ_DEAD_REAPER:
        case OBJ_DEAD_SHEEP:
        case OBJ_DEAD_DOG:
        case OBJ_DEAD_DEER:
        case OBJ_DEAD_WOLF:
        case OBJ_DEAD_GREMLIN:
        case OBJ_DEAD_MOUSE:
        case OBJ_DEAD_BIRD:
        case OBJ_DEAD_SNAKE:
        case OBJ_DEAD_RABBIT:
        case OBJ_DEAD_SPIDER:
        case OBJ_DEAD_DRAKE:
        case OBJ_DEAD_MONGBAT:
        case OBJ_DEAD_CAT:
            objtypedead = 1;
            break;
        default:
            objtypedead = 0;
            break;
    }

    return objtypedead;
}

// s333
int convertdeadobjtypen1(int typeparam) {
    int objtype;
    int type;

    if (typeparam >= 0)
        type = typeparam & 1023;
    else
        type = typeparam;

    switch (type) {
        case -1:
            objtype = -1;
            break;
        case OBJ_DEAD_ANIMAL:
            objtype = -2;
            break;
        case OBJ_DEAD_BODY:
            objtype = -2;
            // 0 = headless, 2 = gypsy?, 3 = mage/woman, 4 = guard, 5 = swashbucker, 6 = fighter
            break;
        case OBJ_DEAD_CYCLOPS:
            objtype = OBJ_CYCLOPS;
            break;
        case OBJ_DEAD_GARGOYLE:
            objtype = OBJ_GARGOYLE_WINGED;
            break;
        case OBJ_DEAD_GIANT_RAT:
            objtype = OBJ_GIANT_RAT;
            break;
        case OBJ_DEAD_GIANT_BAT:
            objtype = OBJ_GIANT_BAT;
            break;
        case OBJ_DEAD_REAPER:
            objtype = OBJ_REAPER;
            break;
        case OBJ_DEAD_SHEEP:
            objtype = OBJ_SHEEP;
            break;
        case OBJ_DEAD_DOG:
            objtype = OBJ_DOG;
            break;
        case OBJ_DEAD_DEER:
            objtype = OBJ_DEER;
            break;
        case OBJ_DEAD_WOLF:
            objtype = OBJ_WOLF;
            break;
        case OBJ_DEAD_GREMLIN:
            objtype = OBJ_GREMLIN;
            break;
        case OBJ_DEAD_MOUSE:
            objtype = OBJ_MOUSE;
            break;
        case OBJ_DEAD_BIRD:
            objtype = OBJ_BIRD;
            break;
        case OBJ_DEAD_SNAKE:
            objtype = OBJ_SNAKE;
            break;
        case OBJ_DEAD_RABBIT:
            objtype = OBJ_RABBIT;
            break;
        case OBJ_DEAD_SPIDER:
            objtype = OBJ_GIANT_SPIDER;
            break;
        case OBJ_DEAD_DRAKE:
            objtype = OBJ_DRAKE;
            break;
        case OBJ_DEAD_MONGBAT:
            objtype = OBJ_MONGBAT;
            break;
        case OBJ_DEAD_CAT:
            objtype = OBJ_CAT;
            break;
        default:
            objtype = -1;
            break;
    }

    //txt* tt22 = txtnew();
    //txtnumint(tt22, type);
    //LOGadd(tt22);
    return objtype;
}

// s333 check object type is creature
int checkobjtypecreaturen1(int typeparam) {
    int checkresult = -2;
    int type;

    if (typeparam >= 0)
        type = typeparam & 1023;
    else
        type = typeparam;

    switch (type) {
        case -1:
            //txtset(txtdesc, "INVALID");
            break;
        case OBJ_GIANT_RAT:
        case OBJ_INSECTS:
        case OBJ_GIANT_BAT:
        case OBJ_GIANT_SQUID:
        case OBJ_SEA_SERPENT:
        case OBJ_REAPER:
        case OBJ_SHEEP:
        case OBJ_DOG:
        case OBJ_DEER:
        case OBJ_WOLF:
        case OBJ_GHOST:
        case OBJ_GREMLIN:
        case OBJ_MOUSE:
        case OBJ_GAZER:
        case OBJ_BIRD:
        case OBJ_CORPSER:
        case OBJ_SNAKE:
        case OBJ_RABBIT:
        case OBJ_ROT_WORMS:
        case OBJ_GIANT_SPIDER:
        case OBJ_GARGOYLE_WINGED:
        case OBJ_GARGOYLE_WINGLESS:
        case OBJ_ACID_SLUG:
        case OBJ_TANGLE_VINE_POD:
        case OBJ_TANGLE_VINE:
        case OBJ_DAEMON:
        case OBJ_SKELETON:
        case OBJ_DRAKE:
        case OBJ_HEADLESS:
        case OBJ_TROLL:
        case OBJ_MONGBAT:
        case OBJ_WISP:
        case OBJ_HYDRA_MOUTH:
        case OBJ_SLIME:
        case OBJ_FIGHTER:
        case OBJ_SWASHBUCKLER:
        case OBJ_MAGE:
        case OBJ_VILLAGER:
        case OBJ_MERCHANT:
        case OBJ_CHILD:
        case OBJ_GUARD:
        case OBJ_JESTER:
        case OBJ_PEASANT:
        case OBJ_FARMER:
        case OBJ_MUSICIAN:
        case OBJ_WOMAN:
        case OBJ_CAT:
        case OBJ_LORD_BRITISH:
        case OBJ_AVATAR:
        case OBJ_DRAGON:
        case OBJ_SHIP:
        case OBJ_SILVER_SERPENT:
        case OBJ_CYCLOPS:
        //case OBJ_HYDRA_ARMS:
        //txtset(txtdesc, "");
        //break;
        case OBJ_GIANT_SCORPION:
        case OBJ_GIANT_ANT:
        case OBJ_COW:
        case OBJ_ALLIGATOR:
        case OBJ_HORSE:
            checkresult = 1;
            break;
        case OBJ_CHEST:
        case OBJ_WEB:
            checkresult = 2;
            break;
        default:
            checkresult = -2;
            break;
    }

    return checkresult;
}

// s333 get description of object type (for creature)
void getobjdescn1(txt *txtdesc, int typeparam) {
    if (txtdesc) {
        int type;

        if (typeparam >= 0)
            type = typeparam & 1023;
        else
            type = typeparam;

        switch (type) {
            case -1:
                txtset(txtdesc, "INVALID");
                break;
            case OBJ_GIANT_RAT:
                txtset(txtdesc, "Giant rat");
                break;
            case OBJ_INSECTS:
                txtset(txtdesc, "Insect");
                break;
            case OBJ_GIANT_BAT:
                txtset(txtdesc, "Giant bat");
                break;
            case OBJ_GIANT_SQUID:
                txtset(txtdesc, "Giant squid");
                break;
            case OBJ_SEA_SERPENT:
                txtset(txtdesc, "Sea serpent");
                break;
            case OBJ_REAPER:
                txtset(txtdesc, "Reaper");
                break;
            case OBJ_SHEEP:
                txtset(txtdesc, "Sheep");
                break;
            case OBJ_DOG:
                txtset(txtdesc, "Dog");
                break;
            case OBJ_DEER:
                txtset(txtdesc, "Deer");
                break;
            case OBJ_WOLF:
                txtset(txtdesc, "Wolf");
                break;
            case OBJ_GHOST:
                txtset(txtdesc, "Ghost");
                break;
            case OBJ_GREMLIN:
                txtset(txtdesc, "Gremlin");
                break;
            case OBJ_MOUSE:
                txtset(txtdesc, "Mouse");
                break;
            case OBJ_GAZER:
                txtset(txtdesc, "Gazer");
                break;
            case OBJ_BIRD:
                txtset(txtdesc, "Bird");
                break;
            case OBJ_CORPSER:
                txtset(txtdesc, "Corpser");
                break;
            case OBJ_SNAKE:
                txtset(txtdesc, "Snake");
                break;
            case OBJ_RABBIT:
                txtset(txtdesc, "Rabbit");
                break;
            case OBJ_ROT_WORMS:
                txtset(txtdesc, "Rot worms");
                break;
            case OBJ_GIANT_SPIDER:
                txtset(txtdesc, "Giant spider");
                break;
            case OBJ_GARGOYLE_WINGED:
                txtset(txtdesc, "Winged gargoyle");
                break;
            case OBJ_GARGOYLE_WINGLESS:
                txtset(txtdesc, "Gargoyle");
                break;
            case OBJ_ACID_SLUG:
                txtset(txtdesc, "Acid slug");
                break;
            case OBJ_TANGLE_VINE_POD:
                txtset(txtdesc, "Tangle vine pod");
                break;
            case OBJ_TANGLE_VINE:
                txtset(txtdesc, "Tangle vine");
                break;
            case OBJ_DAEMON:
                txtset(txtdesc, "Daemon");
                break;
            case OBJ_SKELETON:
                txtset(txtdesc, "Skeleton");
                break;
            case OBJ_DRAKE:
                txtset(txtdesc, "Drake");
                break;
            case OBJ_HEADLESS:
                txtset(txtdesc, "Headless");
                break;
            case OBJ_TROLL:
                txtset(txtdesc, "Troll");
                break;
            case OBJ_MONGBAT:
                txtset(txtdesc, "Mongbat");
                break;
            case OBJ_WISP:
                txtset(txtdesc, "Wisp");
                break;
            case OBJ_HYDRA_MOUTH:
                txtset(txtdesc, "Hydra");
                break;
            case OBJ_SLIME:
                txtset(txtdesc, "Slime");
                break;
            case OBJ_FIGHTER:
                txtset(txtdesc, "Fighter");
                break;
            case OBJ_SWASHBUCKLER:
                txtset(txtdesc, "Swashbuckler");
                break;
            case OBJ_MAGE:
                txtset(txtdesc, "Mage");
                break;
            case OBJ_VILLAGER:
                txtset(txtdesc, "Villager");
                break;
            case OBJ_MERCHANT:
                txtset(txtdesc, "Merchant");
                break;
            case OBJ_CHILD:
                txtset(txtdesc, "Child");
                break;
            case OBJ_GUARD:
                txtset(txtdesc, "Guard");
                break;
            case OBJ_JESTER:
                txtset(txtdesc, "Jester");
                break;
            case OBJ_PEASANT:
                txtset(txtdesc, "Peasant");
                break;
            case OBJ_FARMER:
                txtset(txtdesc, "Farmer");
                break;
            case OBJ_MUSICIAN:
                txtset(txtdesc, "Musician");
                break;
            case OBJ_WOMAN:
                txtset(txtdesc, "Woman");
                break;
            case OBJ_CAT:
                txtset(txtdesc, "Cat");
                break;

            case OBJ_LORD_BRITISH:
                txtset(txtdesc, "Lord British");
                break;
            case OBJ_AVATAR:
                txtset(txtdesc, "A player");
                break;
            case OBJ_DRAGON:
                txtset(txtdesc, "Dragon");
                break;
            case OBJ_SHIP:
                txtset(txtdesc, "A ship");
                break;
            case OBJ_SILVER_SERPENT:
                txtset(txtdesc, "Silver serpent");
                break;
            case OBJ_CYCLOPS:
                txtset(txtdesc, "Cyclops");
                break;
            //case OBJ_HYDRA_ARMS:
            //txtset(txtdesc, "");
            //break;
            case OBJ_GIANT_SCORPION:
                txtset(txtdesc, "Giant scorpion");
                break;
            case OBJ_GIANT_ANT:
                txtset(txtdesc, "Giant ant");
                break;
            case OBJ_COW:
                txtset(txtdesc, "Cow");
                break;
            case OBJ_ALLIGATOR:
                txtset(txtdesc, "Alligator");
                break;
            case OBJ_HORSE:
                txtset(txtdesc, "Horse");
                break;

            case OBJ_CHEST:
                txtset(txtdesc, "A chest");
                break;
            case OBJ_WEB:
                txtset(txtdesc, "A web");
                break;

            default:
                txtset(txtdesc, "Something (???)");
                break;
        }

        //txt* tt22 = txtnew();
        //txtnumint(tt22, type);
        //LOGadd(tt22);
    }
}

unsigned short objgettype(unsigned short type, unsigned char dir, unsigned char frm) {
    switch (type) {
        case OBJ_SKIFF:
            return type + (dir << 10);
        case OBJ_SHIP:
            return type + ((dir * 2 + 9) << 10);
        case OBJ_GARGOYLE_WINGED:
            return type + ((dir * 12) << 10) + (frm << 12) + 3 * 1024;
        case OBJ_GARGOYLE_WINGLESS:
            return type + ((dir * 3) << 10) + (frm << 10);
        case OBJ_CYCLOPS:
            return type + (dir << 13) + (frm << 12) + 3 * 1024;
        case OBJ_DRAGON:
            return type + (dir << 11);
        case OBJ_BALLOON:
            return type + 3 * 1024; //hot air balloon
    }
    switch (obji[sprlnk[type]].v4) {
        case 1: //"avatar"
            return type + (dir << 12) + ((frm & 3) << 10);
        case 4: //"rabbit"
            return type + (dir << 10);
        case 6: //"drake"
            return type + (dir << 11);
        case 2: //"rat" type
        case 8: //"ant" type
            return type + (dir << 11) + (frm << 10);
    }
    // normal case:
    return type + (frm << 10);
}

unsigned char objgetnextframe(unsigned short type, unsigned char frm) {
    switch (type) {
        case OBJ_GARGOYLE_WINGED:
            frm++;
            if (frm > 2) frm = 0;
            return frm;
        case OBJ_GARGOYLE_WINGLESS:
            frm++;
            if (frm > 2) frm = 0;
            return frm;
        case OBJ_CYCLOPS:
            frm++;
            if (frm > 1) frm = 0;
    }
    switch (obji[sprlnk[type]].v4) {
        case 1: //"avatar" type
            if (frm & 128) {
                //special frame check
                frm = 0;
                return frm;
            }
            frm++;
            if (frm > 2) {
                frm = 1 + 128;
                return frm;
            }
            return frm;
        case 2: //"rat" type
        case 8: //"ant" type
            frm++;
            if (frm > 1) frm = 0;
            return frm;
    }
    return frm; //return frm, mover does not support multiple frames
}


void getsound(unsigned short type, long x, long y) {
    //  static long z; // luteijn: I doubt static is needed.
    long z;
    static unsigned char SNDticktock_done = FALSE;
    static unsigned char SNDfire_done = FALSE;

    if (!vischeck[x][y]) return;
    if (int(ett) == int(ett - et)) {
        SNDticktock_done = FALSE;
        SNDfire_done = FALSE;
        return;
    };

    switch (type & 1023) {
        case OBJ_FOUNTAIN:
            soundplay2(u6osound[3], u6osound_volume[3]);
            return;
        case OBJ_PROTECTION_FIELD:
            soundplay2(u6osound[33], u6osound_volume[33]);
            return;
        case OBJ_ELECTRIC_FIELD:
            soundplay2(u6osound[SOUND_FIELD], u6osound_volume[SOUND_FIELD]);
            return;
        case OBJ_LOG_SAW:
            soundplay2(u6osound[15], u6osound_volume[15]);
            return;
        case OBJ_CLOCK:
            if (type & 1024) {
                // one part to tick is enough..
                if (SNDticktock_done == FALSE) {
                    soundplay2(u6osound[17], u6osound_volume[17]);
                    SNDticktock_done = TRUE;
                }
            }
            return;
        /* luteijn: these fire checks are in a particular order to allow to fall through properly */
        case OBJ_COOKFIRE:
            if ((!(type & (5 << 10))) && (!(type & (11 << 10)))) {
                return;
            }
        case OBJ_CAMPFIRE:
        case OBJ_FIREPLACE:
            if (!(type & 1024)) {
                //doused so no sound
                return;
            }
        case OBJ_BRAZIER:
            if (!(type >> 10)) {
                // this also makes holy braziers sound
                return;
            }
        case OBJ_FIRE_FIELD:
        case OBJ_FIRE:
        case OBJ_OVEN:
        case OBJ_VOLCANO:
        case OBJ_STOVE:
        case OBJ_LAMPPOST:
            if (((type & 1023) == OBJ_LAMPPOST) && (!(type & (3 << 10)))) {
                // only lamp part is noisy
                return;
            }
            if (SNDfire_done == FALSE) {
                soundplay2(u6osound[16], u6osound_volume[16]);
                SNDfire_done = TRUE;
            }
            return;
    }


    if (getsound_MOVERSOUND == 0) return;

    z = 0; //flap
    if ((type & 1023) == 344) z = 1;
    if ((type & 1023) == 369) z = 1;
    if ((type & 1023) == 411) z = 1;
    if (z) {
        static unsigned char SNDflap = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDflap == FALSE) {
                soundplay2(u6osound[24], u6osound_volume[24]);
            }
            SNDflap = TRUE;
        } else {
            SNDflap = FALSE;
        }
    }

    if ((type & 1023) == 348) {
        static unsigned char SNDsheep_bleet = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDsheep_bleet == FALSE) {
                if (rnd * 8.0f < 1.0f) soundplay2(u6osound[11], u6osound_volume[11]);
            }
            SNDsheep_bleet = TRUE;
        } else {
            SNDsheep_bleet = FALSE;
        }
    }

    if ((type & 1023) == 428) {
        static unsigned char SNDcow_bleet = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDcow_bleet == FALSE) {
                if (rnd * 16.0f < 1.0f) soundplay2(u6osound[12], u6osound_volume[12]);
            }
            SNDcow_bleet = TRUE;
        } else {
            SNDcow_bleet = FALSE;
        }
    }

    if ((type & 1023) == 349) {
        static unsigned char SNDdog = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDdog == FALSE) {
                if (rnd * 4.0f < 1.0f) soundplay2(u6osound[21], u6osound_volume[21]);
            }
            SNDdog = TRUE;
        } else {
            SNDdog = FALSE;
        }
    }

    if ((type & 1023) == 430) {
        static unsigned char SNDhorse = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDhorse == FALSE) {
                if (rnd * 16.0f < 1.0f) soundplay2(u6osound[22], u6osound_volume[22]);
            }
            SNDhorse = TRUE;
        } else {
            SNDhorse = FALSE;
        }
    }

    if ((type & 1023) == 358) {
        static unsigned char SNDsnake = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDsnake == FALSE) {
                if (rnd * 8.0f < 1.0f) soundplay2(u6osound[23], u6osound_volume[23]);
            }
            SNDsnake = TRUE;
        } else {
            SNDsnake = FALSE;
        }
    }

    if ((type & 1023) == 356) {
        //bird
        static unsigned char SNDbird_bleet = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDbird_bleet == FALSE) {
                if (rnd * 8.0f < 1.0f) soundplay2(u6osound[13], u6osound_volume[13]);
            }
            SNDbird_bleet = TRUE;
        } else {
            SNDbird_bleet = FALSE;
        }
    }

    if ((type & 1023) == 388) {
        //cat.wav
        static unsigned char SNDdone = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDdone == FALSE) {
                if (rnd * 8.0f < 1.0f) soundplay2(u6osound[25], u6osound_volume[25]);
            }
            SNDdone = TRUE;
        } else {
            SNDdone = FALSE;
        }
    }

    if ((type & 1023) == 351) {
        //wolf.wav
        static unsigned char SNDdone = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDdone == FALSE) {
                if (rnd * 8.0f < 1.0f) soundplay2(u6osound[26], u6osound_volume[26]);
            }
            SNDdone = TRUE;
        } else {
            SNDdone = FALSE;
        }
    }

    if ((type & 1023) == 343) {
        //insects.wav
        static unsigned char SNDdone = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDdone == FALSE) {
                if (rnd * 8.0f < 1.0f) soundplay2(u6osound[30], u6osound_volume[30]);
            }
            SNDdone = TRUE;
        } else {
            SNDdone = FALSE;
        }
    }

    if ((type & 1023) == 146) {
        //snore.wav
        static unsigned char SNDdone = FALSE;
        if (int(ett) != int(ett - et)) {
            if (SNDdone == FALSE) {
                if (rnd * 32.0f < 1.0f) soundplay2(u6osound[34], u6osound_volume[34]);
            }
            SNDdone = TRUE;
        } else {
            SNDdone = FALSE;
        }
    }
} //getsound

void getlight(unsigned short type, long x, long y) {
    if (!vischeck[x][y]) return;

    int z = 0;
    unsigned short frame = type >> 10;
    type = type & 1023;
    if (frame > 0) {
        // only if the object is ON
        switch (type) {
            case OBJ_CANDLE:
            case OBJ_BRAZIER:
            case OBJ_CAMPFIRE:
                z = 11;
                break;
            case OBJ_CANDELABRA:
                z = 13;
                break;
            case OBJ_FIREPLACE:
                if (frame & 1) {
                    // frame 2 is also off 
                    z = 7;
                }
                break;
            case OBJ_CHEST:
                if (frame == 3) {
                    //(only if magic locked)
                    z = 5;
                }
                break;
            case OBJ_STOVE:
                z = 7;
                break;
                break;
            case OBJ_COOKFIRE:
                if (frame == 4) {
                    z = 9;
                }
                break;
            case OBJ_LAMPPOST:
                if (frame = 3) {
                    z = 13;
                }
                break;
            case OBJ_MOONGATE:
            case OBJ_RED_GATE:
                z = 13;
                break;
        }
    }
    switch (type) {
        // always on types 
        case OBJ_MAGIC_HELM:
        case OBJ_MAGIC_SHIELD:
        case OBJ_MAGIC_ARMOUR:
        case OBJ_GLASS_SWORD:
        case OBJ_MAGIC_BOW:
        case OBJ_SNAKE_AMULET:
        case OBJ_AMULET_OF_SUBMISSION:
        case OBJ_GEM:
        case OBJ_STAFF:
        case OBJ_LIGHTNING_WAND:
        case OBJ_FIRE_WAND:
        case OBJ_PROTECTION_RING:
        case OBJ_REGENERATION_RING:
        case OBJ_INVISIBILITY_RING:
            z = 3;
            break;
        case OBJ_FIRE:
        case OBJ_OVEN:
        case OBJ_HEATSOURCE:
        case OBJ_POISON_FIELD:
        case OBJ_SLEEP_FIELD:
            z = 5;
            break;
        case OBJ_PROTECTION_FIELD:
        case OBJ_WISP:
            z = 7;
            break;
        case OBJ_FIRE_FIELD:
            z = 9;
            break;
        case OBJ_FORCE_FIELD:
            z = 11;
            break;
        case OBJ_LIGHTSOURCE:
            z = 13;
            break;
    }
    if ((type >= 297) && (type <= 300)) {
        //magically locked doors
        if (frame == 13) z = 5;
        if (frame == 15) z = 5;
    }

    switch (z) {
        case 2: LIGHTnew(x, y, (unsigned long) &ls3b, 3);
            break;
        case 3: LIGHTnew(x, y, (unsigned long) &ls3, 3);
            break;
        case 4: LIGHTnew(x, y, (unsigned long) &ls5b, 5);
            break;
        case 5: LIGHTnew(x, y, (unsigned long) &ls5, 5);
            break;
        case 7: LIGHTnew(x, y, (unsigned long) &ls7, 7);
            break;
        case 9: LIGHTnew(x, y, (unsigned long) &ls9, 9);
            break;
        case 11: LIGHTnew(x, y, (unsigned long) &ls11, 11);
            break;
        case 13: LIGHTnew(x, y, (unsigned long) &ls13, 13);
            break;
    }
} //getlight

//portraits 2.0 functions
void loadportrait(unsigned short i, surf *s) {
    //s is assumed to be a valid source of the original portrait which will not be deallocated/changed
    // MM-P9.3: portrait reload leak (dominant idle leak). loadportrait is called
    // from the type-43 net handler every time the host pushes portrait data —
    // including repeated refreshes for the local player and nearby NPCs. The
    // previous doublesize/halfsize surfaces for this slot were overwritten below
    // without being released, leaking ~30KB of DirectDraw SURF_SYSMEM16 per
    // reload (they accumulate in surflist[] and progressively slow every blit).
    // Release them before rebuilding. These two are only ever used as transient
    // img0 blit sources (getportrait_doublesize/_halfsize), never stored as a
    // long-lived ->graphic, so freeing here cannot dangle a cached pointer.
    // The 56x64 `s` is the SAME cached surface as the previous portrait[i] (the
    // type-43 handler now reuses it), so portrait[i]'s pointer stays stable and
    // inpf->graphic (== getportrait(i)) does not dangle. This runs in the main
    // loop's message pass, not concurrently with rendering.
    if (portrait_loaded[i]) {
        if (portrait_doublesize[i]) free(portrait_doublesize[i]);
        if (portrait_halfsize[i]) free(portrait_halfsize[i]);
        portrait_doublesize[i] = NULL;
        portrait_halfsize[i] = NULL;
    }
    portrait_loaded[i] = TRUE;
    portrait[i] = s;
    portrait_doublesize[i] = newsurf(112, 128, SURF_SYSMEM16);
    img(portrait_doublesize[i], portrait[i]);

    portrait_halfsize[i] = newsurf(28, 32, SURF_SYSMEM16);
    //resample to halfsize image using brightness correction
    cls(portrait_halfsize[i], 0);
    static long x, y, z, z2, z3, z4, i2, r, g, b, zz, zz2, zz3, zz4, brightness;
    for (y = 0; y <= 31; y++) {
        for (x = 0; x <= 27; x++) {
            z = portrait[i]->o2[y * 112 + x * 2];
            zz = (z & 31) + ((z >> 6) & 31) + ((z >> 11) & 31);
            z2 = portrait[i]->o2[y * 112 + x * 2 + 1];
            zz2 = (z2 & 31) + ((z2 >> 6) & 31) + ((z2 >> 11) & 31);
            z3 = portrait[i]->o2[y * 112 + x * 2 + 56];
            zz3 = (z3 & 31) + ((z3 >> 6) & 31) + ((z3 >> 11) & 31);
            z4 = portrait[i]->o2[y * 112 + x * 2 + 1 + 56];
            zz4 = (z4 & 31) + ((z4 >> 6) & 31) + ((z4 >> 11) & 31);
            brightness = 0;
            if (zz > brightness) brightness = zz;
            if (zz2 > brightness) brightness = zz2;
            if (zz3 > brightness) brightness = zz3;
            if (zz4 > brightness) brightness = zz4;
            brightness = brightness * 12 / 16;
            i2 = 0;
            if (z) i2++;
            if (z2) i2++;
            if (z3) i2++;
            if (z4) i2++;
            if (i2 >= 2) {
                b = 0;
                if (z) b += (z & 31);
                if (z2) b += (z2 & 31);
                if (z3) b += (z3 & 31);
                if (z4) b += (z4 & 31);
                b /= i2;
                g = 0;
                if (z) g += ((z >> 5) & 63);
                if (z2) g += ((z2 >> 5) & 63);
                if (z3) g += ((z3 >> 5) & 63);
                if (z4) g += ((z4 >> 5) & 63);
                g /= i2;
                r = 0;
                if (z) r += ((z >> 11) & 31);
                if (z2) r += ((z2 >> 11) & 31);
                if (z3) r += ((z3 >> 11) & 31);
                if (z4) r += ((z4 >> 11) & 31);
                r /= i2;
                if ((r + b + g) < brightness) {
                    brightness = brightness - (r + g + b);
                    brightness /= 3;
                    r += brightness;
                    g += brightness;
                    b += brightness;
                }
                portrait_halfsize[i]->o2[y * 28 + x] = b + (g << 5) + (r << 11);
            }
        }
    }
}

surf *getportrait(unsigned short i) {
    if (portrait_loaded[i]) {
        return portrait[i];
    }
    if (portrait_requested[i] == FALSE) {
        portrait_requested[i] = TRUE;
        txtsetchar(portrait_request_txt, 42);
        txtaddshort(portrait_request_txt, i);
        NET_send(NETplayer, NULL, portrait_request_txt);
    }
    return PORTRAIT_UNAVAILABLE;
}

surf *getportrait_doublesize(unsigned short i) {
    if (portrait_loaded[i]) {
        return portrait_doublesize[i];
    }
    if (portrait_requested[i] == FALSE) {
        portrait_requested[i] = TRUE;
        txtsetchar(portrait_request_txt, 42);
        txtaddshort(portrait_request_txt, i);
        NET_send(NETplayer, NULL, portrait_request_txt);
    }
    return PORTRAIT_UNAVAILABLE;
}

surf *getportrait_halfsize(unsigned short i) {
    if (portrait_loaded[i]) {
        return portrait_halfsize[i];
    }
    if (portrait_requested[i] == FALSE) {
        portrait_requested[i] = TRUE;
        txtsetchar(portrait_request_txt, 42);
        txtaddshort(portrait_request_txt, i);
        NET_send(NETplayer, NULL, portrait_request_txt);
    }
    return PORTRAIT_UNAVAILABLE;
}


unsigned char getambientlight(unsigned short x, unsigned short y) {
    if ((x >= 1280) && (y >= 344) && (x <= 1336) && (y <= 394)) {
        AMBIENTLIGHT_SHOWSUN = TRUE;
        return 0;
    } //top floor of keep
    if ((x >= 1024) && (y >= 616) && (x < 1072) && (y < 656)) {
        AMBIENTLIGHT_SHOWSUN = TRUE;
        return 0;
    } //island floor II
    if ((x >= 1280) && (y >= 256) && (x < 1351) && (y < 319)) {
        AMBIENTLIGHT_SHOWSUN = TRUE;
        return 0;
    } //castle floor II
    if ((x >= 1242) && (y >= 512) && (x <= 1265) && (y <= 538)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //underground of keep

    if ((x >= 1808) && (y >= 424) && (x < 1902) && (y < 527)) {
        AMBIENTLIGHT_SHOWSUN = TRUE;
        return 0;
    } //kat's high level maze
    if ((x >= 1280) && (y >= 395) && (x < 1341) && (y <= 432)) {
        AMBIENTLIGHT_SHOWSUN = TRUE;
        return 0;
    } //2nd Floor Shop in brit

    if ((x >= 1024) && (y >= 0) && (x < 1072) && (y < 48)) {
        AMBIENTLIGHT_SHOWSUN = TRUE;
        return 0;
    } //spirituality shrine

    if ((x >= 1780) && (y >= 256) && (x <= 2047) && (y <= 657)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //kat's high level dungeon underground

    if ((x >= 1280) && (y >= 319) && (x <= 1291) && (y <= 334)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //"guardians" guild hall (underground)
    if ((x >= 1024) && (y >= 512) && (x <= 1258) && (y <= 614)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //newbie dungeon
    if ((x >= 1072) && (y >= 616) && (x <= 1141) && (y <= 669)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //island (underground)
    if ((x >= 1292) && (y >= 319) && (x <= 1309) && (y <= 333)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //
    if ((x >= 1310) && (y >= 319) && (x <= 1319) && (y <= 335)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //
    if ((x >= 1320) && (y >= 319) && (x <= 1326) && (y <= 324)) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //
    //if ((x>=1327)&&(y>=319)&&(x<=1358)&&(y<=343))  {AMBIENTLIGHT_SHOWSUN=FALSE; AMBIENTLIGHT_LIGHTVALUE=15; return 1;}//within toth's house

    if ((x >= 1024) && (!((y >= 256) && (x < 1280) && (y < 512)))) {
        AMBIENTLIGHT_SHOWSUN = FALSE;
        AMBIENTLIGHT_LIGHTVALUE = 15;
        return 1;
    } //underground 
    AMBIENTLIGHT_SHOWSUN = TRUE;
    return 0;
}


void txtmakeu6ocompatible(txt *t) {
    static long i, ok, x;
txtmakeu6ocompatible_loop:
    for (i = 0; i < t->l; i++) {
        ok = 0;
        x = t->d2[i];
        if ((x >= 32) && (x <= 126)) ok = 1;
        if (ok == 0) {
            if (i != (t->l - 1)) {
                //if not the last character, shift the rest of the string
                memcpy(&t->d2[i], &t->d2[i + 1], t->l - i - 1);
            }
            t->l--;
            t->d2[t->l] = 0;
            goto txtmakeu6ocompatible_loop;
        } //ok==0
    } //i
    for (i = 0; i < t->l; i++) {
        x = t->d2[i];
        if (x == 64) t->d2[i] = 7; //@
        if (x == 92) t->d2[i] = 8; //"\"
    } //i
}


void STATUSMESSadd(txt *t) {
    static long i, i2;
    // MM-P9.5: release any cached on-surface text DC on `ps` before this
    // event-driven GetDC. The per-frame text path (txtout/txtouts) may leave a
    // cached DC held on `ps`; DirectDraw would fail this GetDC while one is held.
    surf_text_dc_release(ps);
    ps->s->GetDC(&taghdc);
    {
        HGDIOBJ _old_tag_font = SelectObject(taghdc, fnt1);
    if (STATUSMESSpending->l) txtaddchar(STATUSMESSpending, 13);
    i2 = STATUSMESSpending->l; //starting scan position
    for (i = 0; i < t->l; i++) {
        txtaddchar(STATUSMESSpending, t->d2[i]);
        tagxy.cx = 0;
        tagxy.cy = 0;
        GetTextExtentPoint32(taghdc, &STATUSMESSpending->d[i2], STATUSMESSpending->l - i2, &tagxy);
        if ((tagxy.cx >= 1008) && (i < (t->l - 1))) {
            txtaddchar(STATUSMESSpending, 46);
            txtaddchar(STATUSMESSpending, 46);
            txtaddchar(STATUSMESSpending, 46);
            txtaddchar(STATUSMESSpending, 13);
            i2 = STATUSMESSpending->l;
        } //tagxy.cx>=1008
    } //i
        SelectObject(taghdc, _old_tag_font);
    }
    ps->s->ReleaseDC(taghdc);
}

void STATUSMESSadd(const char *t) {
    static long i, i2, i3;
    i3 = strlen(t);
    // MM-P9.5: release any cached on-surface text DC on `ps` before this GetDC
    // (see the txt* overload above for the rationale).
    surf_text_dc_release(ps);
    ps->s->GetDC(&taghdc);
    {
        HGDIOBJ _old_tag_font = SelectObject(taghdc, fnt1);
    if (STATUSMESSpending->l) txtaddchar(STATUSMESSpending, 13);
    i2 = STATUSMESSpending->l; //starting scan position
    for (i = 0; i < i3; i++) {
        txtaddchar(STATUSMESSpending, t[i]);
        tagxy.cx = 0;
        tagxy.cy = 0;
        GetTextExtentPoint32(taghdc, &STATUSMESSpending->d[i2], STATUSMESSpending->l - i2, &tagxy);
        if ((tagxy.cx >= 1008) && (i < (i3 - 1))) {
            txtaddchar(STATUSMESSpending, 46);
            txtaddchar(STATUSMESSpending, 46);
            txtaddchar(STATUSMESSpending, 46);
            txtaddchar(STATUSMESSpending, 13);
            i2 = STATUSMESSpending->l;
        } //tagxy.cx>=1008
    } //i
        SelectObject(taghdc, _old_tag_font);
    }
    ps->s->ReleaseDC(taghdc);
}


// s555
void STATUSMESSadd(txt *t, int skippable) {
    static txt *t2 = txtnew();

    if (skippable)
        txtsetchar(t2, 255);

    txtadd(t2, t);
    STATUSMESSadd(t2);
}

void STATUSMESSadd(const char *t, int skippable) {
    static txt *t2 = txtnew();

    txtset(t2, t);
    STATUSMESSadd(t2, skippable);
}

void STATUSMESSadd(const char *t, int skippable, int num) {
    static txt *t2 = txtnew();
    static txt *t3 = txtnew();

    txtset(t2, t);
    txtnumint(t3, num);
    txtadd(t2, t3);
    STATUSMESSadd(t2, skippable);
}


// Draw-time word wrap for the "view previous status message" log. STATUSMESSadd
// pre-wraps incoming text to ~1008px assuming it is drawn from x≈0, but the log
// arrow is draggable, so a line that fit at the left edge runs off the right
// edge once the arrow is moved. This re-wraps a single stored line to whatever
// pixel budget the current draw position leaves, using GetTextExtentExPoint to
// find the break point and backing up to the last space for clean word wrap.
int STATUSMESSwrapline(txt *src, long maxwidth, txt **out, int maxlines) {
    static HDC hdc;
    static SIZE sz;
    int fit;
    long start, end, brk, i;
    int n = 0;

    if (maxlines < 1) return 0;
    if (maxwidth < 1) maxwidth = 1;

    // Empty source still consumes one (blank) line, matching legacy spacing.
    if (src->l <= 0) {
        txtset(out[0], "");
        return 1;
    }

    // MM-P9.5: release any cached on-surface text DC on `ps` before this GetDC
    // (see STATUSMESSadd above for the rationale).
    surf_text_dc_release(ps);
    ps->s->GetDC(&hdc);
    {
        HGDIOBJ _old_hdc_font = SelectObject(hdc, fnt1naa);

    start = 0;
    while ((start < src->l) && (n < maxlines)) {
        long avail = src->l - start;
        fit = 0;
        sz.cx = 0;
        sz.cy = 0;
        GetTextExtentExPoint(hdc, &src->d[start], (int) avail, (int) maxwidth, &fit, NULL, &sz);
        if (fit >= avail) {
            end = src->l; // remainder fits on this line
        } else {
            brk = start + fit;
            if (brk <= start) brk = start + 1; // guarantee forward progress
            // prefer to break at the last space before the hard limit
            long sp = -1;
            for (i = brk - 1; i > start; i--) {
                if (src->d[i] == ' ') { sp = i; break; }
            }
            end = (sp > start) ? sp : brk;
        }
        long len = end - start;
        if (len < 1) len = 1;
        txtNEWLEN(out[n], -len);
        memcpy(out[n]->d, &src->d[start], len);
        n++;
        // advance to next line, skipping the run of spaces we broke on
        start = end;
        while ((start < src->l) && (src->d[start] == ' ')) start++;
    }

        SelectObject(hdc, _old_hdc_font);
    }
    ps->s->ReleaseDC(hdc);

    if (n == 0) { txtset(out[0], ""); n = 1; }
    return n;
}


//X returned directly! unsigned long GETSETTING_OPTION;//a number from 0-? indicating the option chosen
//if getsetting returns non-zero it succeeded
long getsetting(const char *d) {
    static file *tfh;
    static txt *t = txtnew();
    static txt *t2 = txtnew();
    static txt *t3 = txtnew();
    static long i, i2, i3, i4, i5;
    txtset(t3, d);
    tfh = open("settings.txt");
    if (tfh->h != HFILE_ERROR) {
    getsetting_readnextline:
        i3 = seek(tfh);
        if (i3 < lof(tfh)) {
            txtfilein(t, tfh);
            if (t->l) {
                txtset(t2, "{");
                i = txtsearch(t, t2);
                txtright(t, t->l - i);
                txtset(t2, ",");
                i = txtsearch(t, t2);
                txtleft(t, i - 1);
                if (txtsamewithoutcase(t, t3)) {
                    seek(tfh, i3);
                    txtfilein(t, tfh);
                    txtset(t2, "[");
                    i = txtsearch(t, t2);
                    txtright(t, t->l - i);
                    txtset(t2, "]");
                    i = txtsearch(t, t2);
                    txtset(GETSETTING_RAW, t);
                    txtleft(GETSETTING_RAW, i - 1); //txtright(t,t->l-i);


                    //get instruction
                    txtset(t2, "{");
                    i = txtsearch(t, t2);
                    txtright(t, t->l - i);
                    txtset(t2, ",");
                    i = txtsearch(t, t2);
                    txtright(t, t->l - i); //skip name of setting

                    i3 = 0; //if ==1 another instruction awaits
                    txtset(t2, ",");
                    i = txtsearch(t, t2);
                    if (i == 0) i = 10000;
                    txtset(t2, "}");
                    i2 = txtsearch(t, t2);
                    if (i2 == 0) i2 = 10000;
                    if (i2 < i) i = i2;
                    else i3 = 1;
                    txtset(t3, t);
                    txtleft(t3, i - 1);
                    txtright(t, t->l - i);


                    txtset(t2, "CHOICE");
                    if (txtsamewithoutcase(t3, t2)) {
                        i4 = 0;
                    getsetting_choice_nextchoice:
                        txtset(t2, ",");
                        i = txtsearch(t, t2);
                        if (i == 0) i = 10000;
                        txtset(t2, "}");
                        i2 = txtsearch(t, t2);
                        if (i2 == 0) i2 = 10000;
                        if (i2 < i) i = i2;
                        else i3 = 1;
                        txtset(t3, t);
                        txtleft(t3, i - 1);
                        txtright(t, t->l - i);
                        i4++;
                        if (txtsamewithoutcase(t3, GETSETTING_RAW)) {
                            close(tfh);
                            return i4;
                        }
                        if (i3) goto getsetting_choice_nextchoice;
                        close(tfh);
                        return FALSE;
                    } //CHOICE

                    close(tfh);
                    return FALSE;
                } //txtsamewithoutcase
            } //t->l
            goto getsetting_readnextline;
        }
        close(tfh);
    } //tfh->h!=HFILE_ERROR
    return FALSE;
}

// Rewrites settings.txt in place, replacing (or appending) a single
// `{NAME, [VALUE]}` integer entry. Preserves every other line.
//
// Read back with:
//     txtset(GETSETTING_RAW, "");      // clear so absence is detectable
//     getsetting(name);                // return value is ignored
//     if (GETSETTING_RAW->l > 0) {
//         long v = (long)txtnum(GETSETTING_RAW);
//     }
// getsetting() only populates GETSETTING_RAW when the named entry
// exists in the file, so the explicit reset above is what distinguishes
// "missing" from "present and zero".
//
// Failure modes are silent on purpose:
//   - settings.txt missing: a fresh file is created with just the new line.
//   - write failure: the previous file is left intact (the entire
//     replacement content is buffered before the truncate-open).
// Failing to persist UI state must never break the game session.
void setsetting_int(const char *name, long value) {
    static file *tfh;
    static txt *line = txtnew();
    static txt *out = txtnew();
    static txt *needle = txtnew();
    static txt *lowerline = txtnew();
    static txt *numbuf = txtnew();
    static long i, sz;

    txtNEWLEN(out, 0);

    // Build the prefix we recognize, e.g. "{window_maximized,". Comparison
    // is case-insensitive — we lower a copy of each line and compare its
    // leading bytes against `needle`.
    txtset(needle, "{");
    txtadd(needle, name);
    txtadd(needle, ",");
    txtlcase(needle);

    // -- Phase 1: read every line, drop any matching the key --
    tfh = open2("settings.txt", OF_READ | OF_SHARE_COMPAT);
    if (tfh->h != HFILE_ERROR) {
        for (;;) {
            i = seek(tfh);
            sz = lof(tfh);
            if (i >= sz) break;
            txtfilein(line, tfh);
            if (line->l == 0) continue;

            txtset(lowerline, line);
            txtlcase(lowerline);
            bool match = false;
            if (lowerline->l >= needle->l) {
                match = true;
                for (long j = 0; j < needle->l; j++) {
                    if (lowerline->d[j] != needle->d[j]) {
                        match = false;
                        break;
                    }
                }
            }
            if (match) continue; // drop existing entry; we'll re-append it

            if (out->l) txtadd(out, "\r\n");
            txtadd(out, line);
        }
        close(tfh);
    }

    // -- Phase 2: append the new entry --
    if (out->l) txtadd(out, "\r\n");
    txtadd(out, "{");
    txtadd(out, name);
    txtadd(out, ", [");
    txtnumint(numbuf, value);
    txtadd(out, numbuf);
    txtadd(out, "]}\r\n");

    // -- Phase 3: truncate-write. OF_CREATE truncates on open per the
    //    OpenFile() contract used throughout the project (see
    //    e.g. loop_client.cpp:1986 userinfo.txt rewrite). --
    tfh = open2("settings.txt", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    if (tfh->h != HFILE_ERROR) {
        put(tfh, out->d, out->l);
    }
    close(tfh);
}

// rrr added new mode handling
void refresh() {
    // MPRES-P2.1/P2.2 (2026-06-29): collapsed to the single live present path.
    //
    // This high-level refresh() historically fanned out across three window modes
    // (smallwindow + windowsizecyclenum 0 / 1, else full-screen) and, within each,
    // a `dxrefresh` DirectDraw-primary path — the p16to32 / p16to16 inline-asm
    // RGB565→display-format converters blitting into the `vs` primary surface via
    // the intermediate surfaces ps2/ps3/ps4/psnew1/psnew1b — versus a plain
    // refresh(<surf>) present.
    //
    // The single-window cleanup (Option A, 2026-05-20) retired every alternate
    // window. `smallwindow` and `dxrefresh` are initialized FALSE in
    // data_client.cpp and assigned nowhere else in the client (verified by grep
    // across src/), and `windowsizecyclenum` stays 0. So at runtime ONLY the
    // `else` (full-screen) + non-dxrefresh arm was ever reachable, i.e.
    // `refresh(ps)`. Collapsing to that arm deletes the now-unreachable inline-asm
    // present converters and the `vs`-primary blits with zero observable change
    // (semantics-preserving — the GPU presenter already samples ps->o's RGB565
    // directly, so the 16→32 / 16→16 CPU converters are dead here).
    //
    // MPRES-P2.3 (2026-06-29): deleted the trivially-dead present surfaces —
    // `vs` and `psnew1` (never allocated) and `ps2`/`ps4` (allocated but never read).
    //
    // MPRES-P2.3b (2026-06-30): deleted `ps3` (the 32-bpp format-conversion helper:
    // its only reader was the removed p16to32 converter, so the modern presenter —
    // which samples ps->o's RGB565 directly — made it dead on every display; the
    // DDRAW_display_pixelformat bit-count gate that allocated it is gone) and the
    // always-FALSE `dxrefresh` bool (its only use was the focus-skip goto in the
    // brace-seam loop_client_part_refresh_tail.cpp, removed; the live skiprefresh:
    // label there is still reached from the nodisplay / !clientframe skips).
    // Still LIVE and intentionally kept: `psnew1b` (the in-game UI/panel compose
    // surface). `DDRAW_display_pixelformat` is retained — it is still used to set
    // the pixel format on newly created surfaces (myddraw.cpp), just no longer in
    // the present path.
    refresh(ps); //16->? 1024x768
} //refresh()

//screen log
void scrlog(const char *d) {
    static txt *t = txtnew();
    static unsigned long i;
    txtset(t, d);
    LOGadd(t);

    if (SCRLOG_FILEONLY == FALSE) {
        memcpy(ps->o, &ps->o2[16384], 1540096);
        for (i = 770048; i < 786432; i++) {
            ps->o2[i] = (2 << 11) + (27 << 5) + 18;
        }
        txtfnt = systemfont;
        txtout(ps, 0, 752, t);
        if (intro_ultimavi) {
            img(ps, 741, 0, intro_ultimavi2);
            img0(ps, 741, 0, intro_ultimavi);
        }
    }

    refresh();
}

void applyscaleuipanelwidget(int uipi, int uiwi, int uisi, float scalex, float scaley) {
    uipanelsizex[uipi][uiwi][uisi] = uipanelsizex[uipi][uiwi][uisi] * scalex;
    uipanelsizey[uipi][uiwi][uisi] = uipanelsizey[uipi][uiwi][uisi] * scaley;
}

// obsolete
// RW-P3.3 (2026-06-02): see function_client.h for the rationale. One-shot,
// clamped, on-screen first placement for the floating hideable FRAME panels.
void placeFloatingPanelFirstShow(FRAME *f, int homeX, int homeY, int shown) {
    if (f == NULL || f->graphic == NULL) return;
    const int bw = backbufferW();
    const int bh = backbufferH();
    const int w = (int) f->graphic->d.dwWidth;
    const int h = (int) f->graphic->d.dwHeight;
    // Clamp the home position so the whole panel fits inside the current back
    // buffer (a position restored from cltset2 may have been saved while the
    // window was larger). Keep the top-left corner on screen.
    if (homeX + w > bw) homeX = bw - w;
    if (homeY + h > bh) homeY = bh - h;
    if (homeX < 0) homeX = 0;
    if (homeY < 0) homeY = 0;
    f->offset_y = (short) homeY;
    // Park at home when shown, or in the hidden slot (off the right edge) so
    // the hide/show toggle can recover home via offset_x -= kPanelHideDeltaX.
    f->offset_x = (short) (shown ? homeX : homeX + kPanelHideDeltaX);
    f->positioned = true;
}

void updateoptioninfo() {
    static txt *t = txtnew();
    static txt *t2 = txtnew();
    int x, y;
    surf *optioninfosurf = uipanelsurf[uipaneloptioninfo][UI_WIDGET_DEF][UI_STATE_DEF];

    img(optioninfosurf, statusbar_b255);

    txtfntoldn1 = txtfnt;
    txtfnt = systemfont;
    //txtout(ps, 0, 752, t);
    txtcol = rgb(255, 255, 255);
    x = 5;
    y = 5;

    if (showenhancehostn1) {
        if (enhancehostn1) {
            txtset(t, "Server options: enabled");
            //txtnumint(t2, enhancehostn1);
            //txtadd(t, t2);
        } else
            txtset(t, "Server options: default");

        txtout(optioninfosurf, x, y, t);
        y += 16;
    }

    if (enhancen1) {
        txtset(t, "Game option: alternate ");
        txtnumint(t2, enhancen1);
        txtadd(t, t2);
    } else
        txtset(t, "Game option: default");

    txtout(optioninfosurf, x, y, t);

    txtfnt = txtfntoldn1;
}


// r999 new
int gethituipaneli(int x, int y) {
    int uipaneli = -1;
    int uipi, uiwi, uisi;
    //int uiscalex, uiscaley;

    //uiscalex = UI_NOSCALE;
    //uiscaley = UI_NOSCALE;

    uiwi = UI_WIDGET_DEF;
    uisi = UI_STATE_DEF;

    for (uipi = 0; uipi < uipanelcount; uipi++) {
        if ((uipanelhitenable[uipi][uiwi][uisi] == 1)
            && (x >= uipanelx[uipi][uiwi][uisi]) && (x < uipanelx[uipi][uiwi][uisi] + uipanelsizex[uipi][uiwi][uisi])
            && (y >= uipanely[uipi][uiwi][uisi]) && (y < uipanely[uipi][uiwi][uisi] + uipanelsizey[uipi][uiwi][uisi])) {
            uipaneli = uipi;
            break;
        }
    }

    return uipaneli;
}

// r999 new
int gethituipanelwidgeti(int x, int y, int uipaneli) {
    int uipanelwidgeti = -1;
    int uipi, uiwi, uisi;
    //int uiscalex, uiscaley;

    //uiscalex = UI_NOSCALE;
    //uiscaley = UI_NOSCALE;

    //uiwi = UI_WIDGET_DEF;
    uisi = UI_STATE_DEF;

    if (uipaneli > 0)
        uipi = uipaneli;
    else
        uipi = gethituipaneli(x, y);

    for (uiwi = 1; uiwi < uipanelwidgetcount[uipi]; uiwi++) {
        if ((uipanelhitenable[uipi][uiwi][uisi] == 1)
            && (x >= uipanelx[uipi][uiwi][uisi]) && (x < uipanelx[uipi][uiwi][uisi] + uipanelsizex[uipi][uiwi][uisi])
            && (y >= uipanely[uipi][uiwi][uisi]) && (y < uipanely[uipi][uiwi][uisi] + uipanelsizey[uipi][uiwi][uisi])) {
            uipanelwidgeti = uiwi;
            break;
        }
    }

    return uipanelwidgeti;
}

// r999 new
int gethituipanelwidgeti(int x, int y) {
    int uipanelwidgeti = -1;
    int uipi, uisi; // uiwi, 
    //int uiscalex, uiscaley;

    //uiscalex = UI_NOSCALE;
    //uiscaley = UI_NOSCALE;

    //uiwi = UI_WIDGET_DEF;
    uisi = UI_STATE_DEF;
    uipi = gethituipaneli(x, y);
    uipanelwidgeti = gethituipanelwidgeti(x, y, uipi);

    return uipanelwidgeti;
}

// r999 new
int testhituipanel(int x, int y, int uipaneli) {
    //int uipaneli = -1;
    int uipi, uiwi, uisi;
    //int uiscalex, uiscaley;
    int testhitresult = 0;

    //uiscalex = UI_NOSCALE;
    //uiscaley = UI_NOSCALE;

    uipi = uipaneli;
    uiwi = UI_WIDGET_DEF;
    uisi = UI_STATE_DEF;

    if ((x >= uipanelx[uipi][uiwi][uisi]) && (x < uipanelx[uipi][uiwi][uisi] + uipanelsizex[uipi][uiwi][uisi])
        && (y >= uipanely[uipi][uiwi][uisi]) && (y < uipanely[uipi][uiwi][uisi] + uipanelsizey[uipi][uiwi][uisi])) {
        testhitresult = 1;
    }

    return testhitresult;
}

#ifdef loadimage
#undef loadimage
#endif
#define loadimage loadimage

surf *loadimage2(LPCSTR name) {
    //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
    return loadimage(name);
}

surf *loadimage2(txt *name) {
    //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
    return loadimage(name);
}

surf *loadimage2(LPCSTR name, long flags) {
    //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
    return loadimage(name, flags);
}

surf *loadimage2(txt *name, long flags) {
    //txtset(li2_t,"Loading .BMP image ["); txtadd(li2_t,name); txtadd(li2_t,"]"); scrlog(li2_t->d);
    return loadimage(name, flags);
}

// ============================================================================
// MDD: client map-data download driver (see docs/plans/plan-clientMapDownload.md
// and the MDD block in define_both.h).
//
// On connect the host sends MSG_MAPMANIFEST (per-file length + FNV-1a/32
// checksum). For each fixed-object file (objfixed.bin, tobjfix.bin) whose live
// in-RAM copy does NOT already match the manifest, the client either loads a
// current copy from its .\dr\hostcache\ cache or pulls it from the host in
// flow-controlled MAP_CHUNK_BYTES slices (one request in flight at a time),
// verifies the assembled file against the manifest checksum, caches it, and
// swaps it into the live objfixed_*/tobjfixed_* arrays. bt.bin is skipped --
// base tiles stream live via scene-update message 31 in multiplayer.
//
// SAFETY: every failure path (malformed manifest, OOM, bad checksum, missing
// chunk, disconnect) simply leaves the setup-loaded local .\dr\ data in place,
// so the worst case is exactly the legacy "client may render slightly stale
// fixed objects" behavior -- never a crash or a blank world.
// ============================================================================
static unsigned char  MAPDL_state = 0;        // 0 idle, 1 downloading, 2 complete
static unsigned long  MAPDL_len[MAP_FILE_COUNT];
static unsigned long  MAPDL_sum[MAP_FILE_COUNT];
static int            MAPDL_file = -1;         // file id of the in-flight download, or -1
static unsigned long  MAPDL_off = 0;           // bytes assembled so far for MAPDL_file
static unsigned char *MAPDL_buf = NULL;        // assembly buffer for MAPDL_file
static int            MAPDL_cursor = 0;        // next file id to consider in MAPDL_advance
static unsigned char  MAPDL_downloaded_any = 0;
static txt           *MAPDL_req = NULL;        // scratch MSG_MAPCHUNK_REQ message

// objfixed.bin / tobjfix.bin byte layout = index array then type array.
#define MAPDL_IDX_BYTES   (2048u * 1024u * 2u) // objfixed_index[1024][2048] (u16)
#define MAPDL_TYPE_BYTES  (65536u * 2u)        // objfixed_type[65536]       (u16)

static const char *MAPDL_basename(int fileId) {
    switch (fileId) {
        case MAP_FILE_BT:       return "bt.bin";
        case MAP_FILE_OBJFIXED: return "objfixed.bin";
        case MAP_FILE_TOBJFIX:  return "tobjfix.bin";
        default:                return "unknown.bin";
    }
}

static const char *MAPDL_cache_bin(int fileId) {
    static char b[128];
    wsprintfA(b, ".\\dr\\hostcache\\%s", MAPDL_basename(fileId));
    return b;
}

static const char *MAPDL_cache_sum(int fileId) {
    static char b[128];
    wsprintfA(b, ".\\dr\\hostcache\\%s.sum", MAPDL_basename(fileId));
    return b;
}

static void MAPDL_ensure_cachedir(void) {
    CreateDirectoryA(".\\dr", NULL);          // harmless if it already exists
    CreateDirectoryA(".\\dr\\hostcache", NULL);
}

// FNV-1a/32 over the two live in-RAM arrays in file-byte order, matching what
// the host checksummed on disk. Only OBJFIXED / TOBJFIX have live arrays.
static unsigned long MAPDL_live_checksum(int fileId) {
    unsigned long s = MAP_checksum_init();
    if (fileId == MAP_FILE_OBJFIXED) {
        s = MAP_checksum_update(s, &objfixed_index, MAPDL_IDX_BYTES);
        s = MAP_checksum_update(s, &objfixed_type, MAPDL_TYPE_BYTES);
    } else if (fileId == MAP_FILE_TOBJFIX) {
        s = MAP_checksum_update(s, &tobjfixed_index, MAPDL_IDX_BYTES);
        s = MAP_checksum_update(s, &tobjfixed_type, MAPDL_TYPE_BYTES);
    } else {
        return ~MAPDL_sum[fileId]; // BT has no live array; never "already current"
    }
    return MAP_checksum_final(s);
}

// Swap a verified file image into the live fixed-object arrays.
static void MAPDL_apply(int fileId, const unsigned char *buf, unsigned long len) {
    if (len != MAPDL_IDX_BYTES + MAPDL_TYPE_BYTES) return; // unexpected -> keep local
    if (fileId == MAP_FILE_OBJFIXED) {
        memcpy(&objfixed_index, buf, MAPDL_IDX_BYTES);
        memcpy(&objfixed_type, buf + MAPDL_IDX_BYTES, MAPDL_TYPE_BYTES);
    } else if (fileId == MAP_FILE_TOBJFIX) {
        memcpy(&tobjfixed_index, buf, MAPDL_IDX_BYTES);
        memcpy(&tobjfixed_type, buf + MAPDL_IDX_BYTES, MAPDL_TYPE_BYTES);
    }
}

// MDD-P3.2/P3.3: if a current cached copy exists on disk, verify and apply it.
// Returns true if the live arrays now hold the manifest-current file.
static bool MAPDL_load_cache(int fileId, unsigned long len, unsigned long sum) {
    unsigned long cachedsum = 0;
    file *sfh = open2((LPCSTR) MAPDL_cache_sum(fileId), OF_READ | OF_SHARE_COMPAT);
    if (sfh->h == HFILE_ERROR) { close(sfh); return false; }
    if (lof(sfh) < 4) { close(sfh); return false; }
    seek(sfh, 0);
    get(sfh, &cachedsum, 4);
    close(sfh);
    if (cachedsum != sum) return false;            // sidecar says cache is stale

    file *fh = open2((LPCSTR) MAPDL_cache_bin(fileId), OF_READ | OF_SHARE_COMPAT);
    if (fh->h == HFILE_ERROR) { close(fh); return false; }
    if ((unsigned long) lof(fh) != len) { close(fh); return false; }
    unsigned char *buf = (unsigned char *) malloc(len);
    if (!buf) { close(fh); return false; }
    seek(fh, 0);
    get(fh, buf, (long) len);
    close(fh);
    if (MAP_checksum(buf, len) != sum) { free(buf); return false; } // half-written/corrupt
    MAPDL_apply(fileId, buf, len);
    free(buf);
    return true;
}

// MDD-P3.1: persist a verified file image. The sidecar .sum is written LAST and
// is the commit marker -- MAPDL_load_cache trusts a cached file only when its
// sidecar checksum matches AND the file re-hashes correctly, so an interrupted
// write is simply re-downloaded next time (never loaded half-baked).
static void MAPDL_store(int fileId, const unsigned char *buf, unsigned long len) {
    MAPDL_ensure_cachedir();
    file *fh = open2((LPCSTR) MAPDL_cache_bin(fileId),
                     OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    if (fh->h != HFILE_ERROR) put(fh, (void *) buf, (long) len);
    close(fh);
    unsigned long sum = MAPDL_sum[fileId];
    file *sfh = open2((LPCSTR) MAPDL_cache_sum(fileId),
                      OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    if (sfh->h != HFILE_ERROR) put(sfh, &sum, 4);
    close(sfh);
    MAPDL_apply(fileId, buf, len);
}

// MDD-P2.3/P4.3: ask the host for the next slice of the in-flight file. One
// request in flight at a time -- the host's reply drives the next request, so
// the transfer is self-throttling and never floods the send loop.
static void MAPDL_send_request(void) {
    unsigned long remaining = MAPDL_len[MAPDL_file] - MAPDL_off;
    unsigned long want = (remaining > MAP_CHUNK_BYTES) ? MAP_CHUNK_BYTES : remaining;
    if (!MAPDL_req) MAPDL_req = txtnew();
    txtset(MAPDL_req, "?");
    MAPDL_req->d2[0] = MSG_MAPCHUNK_REQ;
    txtaddchar(MAPDL_req, (unsigned char) MAPDL_file);
    txtaddlong(MAPDL_req, MAPDL_off);
    txtaddlong(MAPDL_req, want);
    NET_send(NETplayer, NULL, MAPDL_req);   // client -> host (host is socket 0)
}

static void MAPDL_finish(void) {
    MAPDL_state = 2;
    if (MAPDL_downloaded_any) STATUSMESSadd("Map data synchronized with host.");
    scrlog("MDD: map data sync complete");
}

// Make the live arrays for fileId match the manifest. Returns true if it
// STARTED a download (caller must wait for chunks); false if the file was
// already current (from RAM or the disk cache) and the caller should advance.
static bool MAPDL_provision(int fileId) {
    unsigned long wantlen = MAPDL_len[fileId];
    unsigned long want = MAPDL_sum[fileId];
    if (wantlen == 0) return false;                       // host has no such file
    if (MAPDL_live_checksum(fileId) == want) return false; // shipped/local already matches
    if (MAPDL_load_cache(fileId, wantlen, want)) return false; // cache was current
    if (MAPDL_buf) { free(MAPDL_buf); MAPDL_buf = NULL; }
    MAPDL_buf = (unsigned char *) malloc(wantlen);
    if (!MAPDL_buf) return false;                          // OOM -> keep local data
    MAPDL_file = fileId;
    MAPDL_off = 0;
    MAPDL_downloaded_any = 1;
    if (fileId == MAP_FILE_OBJFIXED) scrlog("MDD: downloading fixed objects (objfixed.bin) from host...");
    else if (fileId == MAP_FILE_TOBJFIX) scrlog("MDD: downloading top fixed objects (tobjfix.bin) from host...");
    MAPDL_send_request();
    return true;
}

// Walk the file list provisioning each; stop and wait when a download starts.
static void MAPDL_advance(void) {
    while (MAPDL_cursor < MAP_FILE_COUNT) {
        int f = MAPDL_cursor;
        MAPDL_cursor++;
        if (f == MAP_FILE_BT) continue;     // base tiles stream live in multiplayer
        if (MAPDL_provision(f)) return;     // download started -> wait for chunks
    }
    MAPDL_finish();
}

void MAPDL_on_manifest(txt *t) {
    if (t->l < (long) (1 + MAP_FILE_COUNT * 8)) return; // malformed -> ignore, keep local
    for (int f = 0; f < MAP_FILE_COUNT; f++) {
        memcpy(&MAPDL_len[f], &t->d2[1 + f * 8], 4);
        memcpy(&MAPDL_sum[f], &t->d2[1 + f * 8 + 4], 4);
    }
    if (MAPDL_buf) { free(MAPDL_buf); MAPDL_buf = NULL; } // discard any prior partial
    MAPDL_file = -1;
    MAPDL_off = 0;
    MAPDL_cursor = 0;
    MAPDL_downloaded_any = 0;
    MAPDL_state = 1;
    MAPDL_advance();
}

void MAPDL_on_chunk(txt *t) {
    if (MAPDL_state != 1) return;
    if (t->l < 10) return;
    unsigned char file = t->d2[1];
    unsigned long off, count;
    memcpy(&off, &t->d2[2], 4);
    memcpy(&count, &t->d2[6], 4);
    if ((int) file != MAPDL_file) return;   // foreign / stale response
    if (off != MAPDL_off) return;           // out of order (shouldn't happen on TCP)
    if (!MAPDL_buf) return;
    if (count == 0) {                       // host could not serve -> abandon, keep local
        free(MAPDL_buf);
        MAPDL_buf = NULL;
        MAPDL_file = -1;
        MAPDL_advance();
        return;
    }
    if (off + count > MAPDL_len[file]) count = MAPDL_len[file] - off;
    if ((unsigned long) t->l < 10 + count) return; // truncated payload -> bail (keep local)
    memcpy(MAPDL_buf + off, &t->d2[10], count);
    MAPDL_off += count;
    if (MAPDL_off < MAPDL_len[file]) {
        MAPDL_send_request();               // pull the next slice
        return;
    }
    // file fully assembled -> verify against the manifest checksum
    if (MAP_checksum(MAPDL_buf, MAPDL_len[file]) == MAPDL_sum[file]) {
        MAPDL_store(file, MAPDL_buf, MAPDL_len[file]);
    } else {
        scrlog("MDD: downloaded map file failed checksum -- keeping local copy");
    }
    free(MAPDL_buf);
    MAPDL_buf = NULL;
    MAPDL_file = -1;
    MAPDL_advance();
}

// MM-P9.1: Clean up input message history linked list (inpmess_mostrecent).
// Allocated in loop_client_part_game_open.cpp:344,825 when player types chat.
// Prevents unbounded memory growth from chat history accumulation.
//
// IMPORTANT: the chat input handler (loop_client_part_game_open.cpp) and the
// startup code (setup_client.inc) require inpmess_mostrecent to ALWAYS be a
// valid node: it derefs inpmess_mostrecent->t and walks ->next to a NULL tail.
// So this must not leave the head NULL — it frees every node (including the old
// empty sentinel) and then re-creates a fresh empty sentinel, exactly mirroring
// setup_client.inc's initialization.
void cleanup_input_message_history(void) {
    inpmess_index *current = inpmess_mostrecent;
    while (current != NULL) {
        inpmess_index *temp = current;
        current = current->next;
        if (temp->t) {
            free(temp->t);  // Free the txt object
            temp->t = NULL;
        }
        free(temp);  // Free the inpmess_index struct
    }
    // Re-establish the empty sentinel the chat code depends on.
    inpmess_mostrecent = (inpmess_index *) malloc(sizeof(inpmess_index));
    inpmess_mostrecent->t = txtnew();
    inpmess_mostrecent->next = NULL;
}

// MM-P9.2: Clean up player name list (idlst_name[]).
// Allocated in loop_client_part_world_render.cpp:870 when players are discovered.
// Prevents unbounded memory growth from accumulating player names.
void cleanup_player_namelist(void) {
    static unsigned long i;
    for (i = 0; i <= (unsigned long)idlstn && i < 1024; i++) {
        if (idlst_name[i] != NULL) {
            free(idlst_name[i]);  // Free the txt object
            idlst_name[i] = NULL;
        }
    }
    idlstn = -1;  // Reset the player list counter
}


