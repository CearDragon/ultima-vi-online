// LCS-P3.4: part_intro_c — intro state machine, continued.
// LCS brace-seam: this file CONTINUES the `if (intro) { ... }` block opened in
// loop_client_part_intro_a.cpp and closed in loop_client_part_intro_d.cpp.
// Contains intro states 206/203/204/205 (custom controls / create-character /
// custom portrait / name-colour flows). Pure relocation — do not edit. See
// docs/plans/plan-loopClientSplit.md.

if
(intro
==
206
)
 {
        //custom keys: main menu
        if (intro_setup == 0) {
            f = 0;
            intro_setup = 1;
        }
        img0(ps, 1024 - 25 - 22 - 2, 2, intro_back);
        img0(ps, 18, 25, intro_ultimavi);
        txtset(t, "Custom Controls");
        txtfnt = fnt7;
        tagxy.cx = 0;
        tagxy.cy = 0;
        surf_text_dc_release(ps); ps->s->GetDC(&taghdc);
        {
            HGDIOBJ _old = SelectObject(taghdc, txtfnt);
            GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
            SelectObject(taghdc, _old);
        }
        ps->s->ReleaseDC(taghdc);
        x5 = 160 - tagxy.cx / 2;
        y5 = 64 * 5 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 255, 96);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;


        txtcol = rgb(255, 255, 255);

        x = 320;
        y = 256;
        x += 32;
        txtset(t, "RESTORE DEFAULT CONTROLS");
        txtouts(ps, x, y, t);
        x -= 32;
        img0(ps, x, y, intro_next);
        y += 64;
        x += 32;
        txtset(t, "CHANGE CURRENT CUSTOM CONTROLS");
        txtouts(ps, x, y, t);
        x -= 32;
        img0(ps, x, y, intro_next);


        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if ((mx >= x) && (mx < (x + 22)) && (my >= (y - 64)) && (my < (y + 22 - 64))) {
                memcpy(&U6OK_TEMP, &U6OK_DEFAULT, 512);
                memcpy(&U6OK_TEMP_FLAGS, &U6OK_DEFAULT_FLAGS, 128);
                tfh = open2(".\\dr\\controls.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                put(tfh, &U6OK_TEMP, 512);
                put(tfh, &U6OK_TEMP_FLAGS, 128);
                close(tfh);
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }

            if ((mx >= x) && (mx < (x + 22)) && (my >= y) && (my < (y + 22))) {
                intro = 203;
                intro_setup = 0;
                goto intro_done;
            }

            if ((mx > (1024 - 21 - 4 - 26)) && (my < (21 + 4))) {
                //back
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }
        } //->mouse_click
    } //intro==206


if
(intro
==
203
)
 {
        //custom keys
        static unsigned char nextkey;
        static unsigned short key1, key2;

        if (intro_setup == 0) {
            tfh = open(".\\dr\\controls.bin");
            get(tfh, &U6OK_TEMP, 512);
            get(tfh, &U6OK_TEMP_FLAGS, 128);
            close(tfh);
            // ensure defaults for the live U6OK array if needed, although this loads into _TEMP
            // but the remapping menu will eventually copy _TEMP to live.
            // Wait, I should ensure defaults in _TEMP too!
            if (U6OK_TEMP[U6OK_CAMERATOGGLE][0] == 0) {
                U6OK_TEMP[U6OK_CAMERATOGGLE][0] = VK_TAB;
            }
            if (U6OK_TEMP[U6OK_RESPAWNFOLLOWERS][0] == 0) {
                U6OK_TEMP[U6OK_RESPAWNFOLLOWERS][0] = VK_SHIFT;
                U6OK_TEMP[U6OK_RESPAWNFOLLOWERS][1] = VK_F1;
            }
            key1 = 0;
            key2 = 0;
            nextkey = 0;
            intro_setup = 1;
            f = 0;
        }

        img0(ps, 1024 - 25 - 22 - 2, 2, intro_back);
        img0(ps, 18, 25, intro_ultimavi);
        txtset(t, "Custom Controls");
        txtfnt = fnt7;
        tagxy.cx = 0;
        tagxy.cy = 0;
        surf_text_dc_release(ps); ps->s->GetDC(&taghdc);
        {
            HGDIOBJ _old = SelectObject(taghdc, txtfnt);
            GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
            SelectObject(taghdc, _old);
        }
        ps->s->ReleaseDC(taghdc);
        x5 = 160 - tagxy.cx / 2;
        y5 = 64 * 5 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 255, 96);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

        img0(ps, 536, 420, intro_ccsave1);
        img0(ps, 536, 484, intro_ccsave3);

        if (nextkey != 0) img0(ps, 466, 416, intro_back);
        if (nextkey != 72) {
            if ((U6OK_TEMP[nextkey][0] != U6OK_DEFAULT[nextkey][0]) || (
                    U6OK_TEMP[nextkey][1] != U6OK_DEFAULT[nextkey][1]) || (
                    U6OK_TEMP_FLAGS[nextkey] != U6OK_DEFAULT_FLAGS[nextkey])) {
                img0(ps, 536, 420, intro_ccsave2);
            }
            img0(ps, 665, 416, intro_next);
            if (U6OK_TEMP_FLAGS[nextkey] & 2) {
                if (U6OK_TEMP_FLAGS[nextkey] & 1) img(ps, 720, 411, instantclickok);
                else img(ps, 720, 411, instantclickx);
            }
        }


        x = 320;
        y = 128;
        txtcol = rgb(18 * 2, 108 * 2, 255); //(3<<11)+(46<<5)+27;

        txtset(t, "Use the green arrows to select which controls to change");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "Only change the controls you need to, then select SAVE ALL");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "To use a combination (eg. SHIFT+T), hold the 1st key then press the 2nd key");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "You may also use SHIFT, CTRL or ALT independently as non-combination keys");
        txtouts(ps, x, y, t);
        y += 32;

        txtcol = rgb(255, 64, 64);
        txtset(t, "CAUTION");
        txtouts(ps, x, y, t);
        y += 32;
        txtcol = rgb(18 * 2, 108 * 2, 255);
        txtset(t, "All keys can used, but some controls may conflict with each other!");
        txtouts(ps, x, y, t);
        y += 32;

        txtset(t, "Press SAVE ALL button to save custom controls");
        i = -1;
        i++;
        if (nextkey == i) txtset(t, "Press WALK RIGHT (WALK EAST) key now [ default key: RIGHT ARROW ]");
        i++;
        if (nextkey == i) txtset(t, "Press WALK LEFT (WALK WEST) key now [ LEFT ARROW ]");
        i++;
        if (nextkey == i) txtset(t, "Press WALK UP (WALK NORTH) key now [ UP ARROW ]");
        i++;
        if (nextkey == i) txtset(t, "Press WALK DOWN (WALK SOUTH) key now [ DOWN ARROW ]");
        i++;
        if (nextkey == i) txtset(t, "Press TALK key now [ T ]");
        i++;
        if (nextkey == i) txtset(t, "Press GLOBAL TALK key now [ SHIFT+T ]");
        i++;
        if (nextkey == i) txtset(t, "Press MESSAGE SEND key now [ ENTER ]");
        i++;
        if (nextkey == i) txtset(t, "Press ATTACK key now [ A ]");
        i++;
        if (nextkey == i) txtset(t, "Press LOOK key now [ L ]");
        i++;
        if (nextkey == i) txtset(t, "Press USE key now [ U ]");
        i++;
        if (nextkey == i) txtset(t, "Press MAXIMIZE/MINIMIZE key now [ M ]");
        i++;
        if (nextkey == i) txtset(t, "Press QUIT key now [ Q ]");
        i++;
        if (nextkey == i) txtset(t, "Press SOUND key now [ S ]");
        i++;
        if (nextkey == i) txtset(t, "Press CANCEL key now [ ESC ]");
        i++;
        if (nextkey == i) txtset(t, "Press RETYPE/MESSAGE RECALL key now [ F3 ]");
        i++;
        if (nextkey == i) txtset(t, "Press RESPAWN key now [ F1 ]");

        i++;
        if (nextkey == i) txtset(t, "Press SELECT 1ST PARTY MEMBER key now [ 1 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 2ND PARTY MEMBER key now [ 2 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 3RD PARTY MEMBER key now [ 3 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 4TH PARTY MEMBER key now [ 4 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 5TH PARTY MEMBER key now [ 5 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 6TH PARTY MEMBER key now [ 6 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 7TH PARTY MEMBER key now [ 7 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 8TH PARTY MEMBER key now [ 8 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 9TH PARTY MEMBER key now [ 9 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 10TH PARTY MEMBER key now [ 0 ]");

        i++;
        if (nextkey == i) txtset(t, "Press SELECT 1ST NUMBERED TARGET key now [ 1 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 2ND NUMBERED TARGET key now [ 2 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 3RD NUMBERED TARGET key now [ 3 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 4TH NUMBERED TARGET key now [ 4 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 5TH NUMBERED TARGET key now [ 5 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 6TH NUMBERED TARGET key now [ 6 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 7TH NUMBERED TARGET key now [ 7 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 8TH NUMBERED TARGET key now [ 8 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 9TH NUMBERED TARGET key now [ 9 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 10TH NUMBERED TARGET key now [ 0 ]");

        i++;
        if (nextkey == i) txtset(t, "Press SELECT 1ST RECALL SPELL key now [ F5 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 2ND RECALL SPELL key now [ F6 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 3RD RECALL SPELL key now [ F7 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 4TH RECALL SPELL key now [ F8 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 5TH RECALL SPELL key now [ F9 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 6TH RECALL SPELL key now [ F10 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 7TH RECALL SPELL key now [ F11 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SELECT 8TH RECALL SPELL key now [ F12 ]");

        i++;
        if (nextkey == i) txtset(t, "Press SET 1ST RECALL SPELL key now [ SHIFT+F5 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 2ND RECALL SPELL key now [ SHIFT+F6 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 3RD RECALL SPELL key now [ SHIFT+F7 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 4TH RECALL SPELL key now [ SHIFT+F8 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 5TH RECALL SPELL key now [ SHIFT+F9 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 6TH RECALL SPELL key now [ SHIFT+F10 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 7TH RECALL SPELL key now [ SHIFT+F11 ]");
        i++;
        if (nextkey == i) txtset(t, "Press SET 8TH RECALL SPELL key now [ SHIFT+F12 ]");

        i++;
        if (nextkey == i) txtset(t, "Press ENTER PARTIAL AMOUNT/QUANTITY key now [ SHIFT ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 1 FOR PARTIAL AMOUNT/QUANTITY key now [ 1 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 2 FOR PARTIAL AMOUNT/QUANTITY key now [ 2 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 3 FOR PARTIAL AMOUNT/QUANTITY key now [ 3 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 4 FOR PARTIAL AMOUNT/QUANTITY key now [ 4 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 5 FOR PARTIAL AMOUNT/QUANTITY key now [ 5 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 6 FOR PARTIAL AMOUNT/QUANTITY key now [ 6 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 7 FOR PARTIAL AMOUNT/QUANTITY key now [ 7 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 8 FOR PARTIAL AMOUNT/QUANTITY key now [ 8 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 9 FOR PARTIAL AMOUNT/QUANTITY key now [ 9 ]");
        i++;
        if (nextkey == i) txtset(t, "Press NUMBER 0 FOR PARTIAL AMOUNT/QUANTITY key now [ 0 ]");

        i++;
        if (nextkey == i) txtset(t, "Press SCROLL UP key now [ MOUSE WHEEL UP ]");
        i++;
        if (nextkey == i) txtset(t, "Press SCROLL DOWN key now [ MOUSE WHEEL DOWN ]");

        i++;
        if (nextkey == i) txtset(t, "Press ALTERNATIVE ATTACK key now [ NOT USED ]");
        i++;
        if (nextkey == i) txtset(t, "Press ALTERNATIVE LOOK key now [ NOT USED ]");
        i++;
        if (nextkey == i) txtset(t, "Press ALTERNATIVE USE key now [ NOT USED ]");

        i++;
        if (nextkey == i) txtset(t, "Press CHAT (CONTINUE CONVERSATION) key now [ CONTROL+T ]");

        i++;
        if (nextkey == i) txtset(t, "Press VOICE CHAT key now [ V ]");

        i++;
        if (nextkey == i) txtset(t, "Press MARK/UNMARK AS DO-NOT-SELL key now [ X ]");

        i++;
        if (nextkey == i) txtset(t, "Press RESPAWN FOLLOWERS key now [ SHIFT+F1 ]");


        tagxy.cx = 0;
        tagxy.cy = 0;
        surf_text_dc_release(ps); ps->s->GetDC(&taghdc);
        {
            HGDIOBJ _old = SelectObject(taghdc, txtfnt);
            GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
            SelectObject(taghdc, _old);
        }
        ps->s->ReleaseDC(taghdc);
        x2 = tagxy.cx / 2;
        txtcol = rgb(255, 255, 255);
        txtouts(ps, 512 + 64 - x2, 128 + 64 + 64 + 64 + 22, t);

        if (nextkey != 72) {
            txtset(t, "Waiting.");
            x = ett * 2.0f;
            x &= 3;
            for (x2 = 0; x2 <= x; x2++) {
                txtadd(t, ".");
            }
            tagxy.cx = 0;
            tagxy.cy = 0;
            surf_text_dc_release(ps); ps->s->GetDC(&taghdc);
            {
                HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                SelectObject(taghdc, _old);
            }
            ps->s->ReleaseDC(taghdc);
            x2 = tagxy.cx / 2;
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 512 + 64 - x2, 128 + 64 + 64 + 64 + 22 + 32, t);

            for (i = 0; i <= 65535; i++) {
                if (keyhit(i)) {
                    if (!key1) key1 = i;
                    else key2 = i;
                }
            }
            if (key1) {
                if (keyon[key1] == 0) {
                    U6OK_TEMP[nextkey][0] = key1;
                    U6OK_TEMP[nextkey][1] = key2;
                    key1 = 0;
                    key2 = 0;
                    nextkey++;
                }
            }
        }


        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if (nextkey != 72) {
                if (U6OK_TEMP_FLAGS[nextkey] & 2) {
                    if ((mx >= 720) && (mx < (720 + 84)) && (my >= 411) && (my < (411 + 32))) {
                        if (U6OK_TEMP_FLAGS[nextkey] & 1) U6OK_TEMP_FLAGS[nextkey]--;
                        else U6OK_TEMP_FLAGS[nextkey] |= 1;
                    }
                }
            }

            if (nextkey != 72) {
                if ((mx >= 536) && (mx < (536 + 81)) && (my >= 420) && (my < (420 + 16))) {
                    if ((U6OK_TEMP[nextkey][0] != U6OK_DEFAULT[nextkey][0]) || (
                            U6OK_TEMP[nextkey][1] != U6OK_DEFAULT[nextkey][1]) || (
                            U6OK_TEMP_FLAGS[nextkey] != U6OK_DEFAULT_FLAGS[nextkey])) {
                        U6OK_TEMP[nextkey][0] = U6OK_DEFAULT[nextkey][0];
                        U6OK_TEMP[nextkey][1] = U6OK_DEFAULT[nextkey][1];
                        U6OK_TEMP_FLAGS[nextkey] = U6OK_DEFAULT_FLAGS[nextkey];
                        nextkey++;
                        key1 = 0;
                        key2 = 0;
                        goto intro_done;
                    }
                }
            }

            if ((mx >= 536) && (mx < (536 + 60)) && (my >= 484) && (my < (484 + 16))) {
                //save all
                tfh = open2(".\\dr\\controls.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                put(tfh, &U6OK_TEMP, 512);
                put(tfh, &U6OK_TEMP_FLAGS, 128);
                close(tfh);
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }

            if ((mx >= 466) && (mx < (466 + 22)) && (my >= 416) && (my < (416 + 22))) {
                if (nextkey) {
                    nextkey--;
                    key1 = 0;
                    key2 = 0;
                    goto intro_done;
                }
            }

            if ((mx >= 665) && (mx < (665 + 22)) && (my >= 416) && (my < (416 + 22))) {
                if (nextkey != 72) {
                    nextkey++;
                    key1 = 0;
                    key2 = 0;
                    goto intro_done;
                }
            }

            if ((mx > (1024 - 21 - 4 - 26)) && (my < (21 + 4))) {
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }
        } //->mouse_click
    } //intro==203


static surf *customport;


if
(intro
==
204
)
 {
        //custom portrait
        if (intro_setup == 0) {
            intro_setup = 1;
            f = 0;
        }

        img0(ps, 1024 - 25 - 22 - 2, 2, intro_back);
        img0(ps, 18, 25, intro_ultimavi);
        txtset(t, "Custom Portrait");
        txtfnt = fnt7;
        tagxy.cx = 0;
        tagxy.cy = 0;
            surf_text_dc_release(ps); ps->s->GetDC(&taghdc);
            {
                HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                SelectObject(taghdc, _old);
            }
            ps->s->ReleaseDC(taghdc);
        x5 = 160 - tagxy.cx / 2;
        y5 = 64 * 6 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 255, 96);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

        x = 320;
        y = 128;
        txtcol = rgb(18 * 2, 108 * 2, 255); //(3<<11)+(46<<5)+27;

        txtset(t, "CUSTOM PORTRAIT UPLOAD RULES");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "1. The portrait must portray the Avatar as being human.");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "2. The portrait must consist mostly of the Avatar's face.");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "3. The gender of the portrait must match that of the Avatar.");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "4. The portrait must not be mistakable for any other original or custom portrait.");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "5. The portrait must not contain nudity or adult content.");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "6. The portrait must be in keeping with the spirit and timeframe of Ultima 6.");
        txtouts(ps, x, y, t);
        y += 32;
        y += 32;
        txtcol = rgb(255, 64, 64);
        txtset(t, "WARNING!");
        if ((long) (ett * 2) & 1) txtouts(ps, x, y, t);
        x += 96;
        txtset(t, "UPLOADING AN INAPPROPRIATE PORTRAIT WILL PERMANENTLY");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "DISABLE THIS FEATURE FOR USE WITH YOUR CHARACTER");

        txtouts(ps, x, y, t);
        y += 32;
        x -= 96;

        txtcol = rgb(18 * 2, 108 * 2, 255);
        y += 32;
        txtset(t, "LEVEL RESTRICTION: ONLY CHARACTERS WHO ARE LEVEL 3 OR ABOVE");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "XP COST: 1000 EXPERIENCE POINTS ARE TAKEN PER UPLOAD");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "FORMAT: ANY .BMP TYPE (DIMENTIONS 56x64)");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "TRANSPARENT COLOUR: BLACK (RGB[0,0,0] AFTER CONVERSION TO 16BIT COLOUR)");
        txtouts(ps, x, y, t);
        y += 32;
        y += 32;
        txtcol = rgb(255, 255, 255);
        x += 128;
        x += 64;
        txtset(t, "SELECT PORTRAIT FILE TO UPLOAD");
        txtouts(ps, x, y, t);
        x -= 32;
        img0(ps, x, y, intro_next);


        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if ((mx >= x) && (mx < (x + 22)) && (my >= y) && (my < (y + 22))) {
                txtgetfilename(t);
                if (t->l == 0) {
                    goto intro_done;
                }
                customport = loadimage(t->d, SURF_SYSMEM16);
                if (customport == NULL) {
                    txtadd(t, " is not a .BMP file");
                    MessageBox(NULL, t->d, "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }
                if (customport->d.dwWidth != 56) {
                    MessageBox(NULL, "BMP must be 56 pixels wide", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }
                if (customport->d.dwHeight != 64) {
                    MessageBox(NULL, "BMP must be 64 pixels high", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }
                intro = 205;
                intro_setup = 0;
                goto intro_done;
            }

            if ((mx > (1024 - 21 - 4 - 26)) && (my < (21 + 4))) {
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }
        } //->mouse_click
    } //intro==204


if
(intro
==
205
)
 {
        //confirm custom portrait
        if (intro_setup == 0) {
            intro_setup = 1;
            f = 0;
        }
        img0(ps, 1024 - 25 - 22 - 2, 2, intro_back);
        img0(ps, 18, 25, intro_ultimavi);
        txtset(t, "Custom Portrait");
        txtfnt = fnt7;
        tagxy.cx = 0;
        tagxy.cy = 0;
            surf_text_dc_release(ps); ps->s->GetDC(&taghdc);
            {
                HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                SelectObject(taghdc, _old);
            }
            ps->s->ReleaseDC(taghdc);
        x5 = 160 - tagxy.cx / 2;
        y5 = 64 * 6 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 255, 96);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

        x = ett / 3.0f;
        x &= 15;
        if (x > 7) {
            y = 1;
            x -= 8;
        } else y = 0;
        img(bt32, -x * 32, -y * 32, bt8[0]);
        y2 = ett * 16;
        y2 &= 31;
        for (y = 0; y <= 24; y++) {
            img(ps, 320, y * 32 - y2, bt32);
            img(ps, 320 + 32, y * 32 - y2, bt32);
        }
        img0(ps, 320 + 4, 128, customport);

        x = 320 + 64 + 32;
        y = 128 + 24;
        txtcol = rgb(18 * 2, 108 * 2, 255); //(3<<11)+(46<<5)+27;

        txtset(t, "Check that the portrait appears correctly on the backgrounds.");
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "Pay attention to areas of the portrait that should or should not be transparent.");
        txtouts(ps, x, y, t);

        y += 128 - 32;
        txtcol = rgb(255, 255, 255);
        x += 32;
        txtset(t, "UPLOAD THIS PORTRAIT WHEN I ENTER BRITANNIA");
        txtouts(ps, x, y, t);
        x -= 32;
        img0(ps, x, y, intro_next);

        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if ((mx >= x) && (mx < (x + 22)) && (my >= y) && (my < (y + 22))) {
                //copy all data to client port buffer
                for (x2 = 0; x2 < 3584; x2++) {
                    customportrait[x2] = customport->o2[x2];
                }
                customportrait_upload = 1;
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }

            if ((mx > (1024 - 21 - 4 - 26)) && (my < (21 + 4))) {
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }
        } //->mouse_click
    } //intro==205
