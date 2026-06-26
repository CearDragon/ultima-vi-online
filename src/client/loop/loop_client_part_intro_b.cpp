// LCS-P3.3: part_intro_b — intro state machine, continued.
// LCS brace-seam: this file CONTINUES the `if (intro) { ... }` block opened in
// loop_client_part_intro_a.cpp and closed in loop_client_part_intro_d.cpp.
// Contains intro states 200/202 (login / username-password / main menu). Pure
// relocation — do not edit. See docs/plans/plan-loopClientSplit.md.

    if (intro == 200) {
        if (intro_setup == 0) {
            intro_setup = 1;

            tfh = open2("userinfo.txt", OF_READWRITE | OF_SHARE_COMPAT);
            if (tfh->h != HFILE_ERROR) {
                txtfilein(u6o_user_name, tfh);
                txtfilein(u6o_user_password, tfh);
                close(tfh);
            } else {
                txtset(u6o_user_name, "UNKNOWN");
                txtset(u6o_user_password, "UNKNOWN");
            }

            f = 0;
        }


        if (NEThost) {
            goto login; //very stupid way of doing this, just a quick fix
        }


        img0(ps, 18, 25, intro_ultimavi);

        txtset(t, "Create a Character");
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
        y5 = 64 * 2 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 160, 0);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

        txtset(t, "Journey Onward");
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
        y5 = 64 * 3 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 160, 0);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

        txtset(t, "Transfer a Character");
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
        y5 = 64 * 4 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 160, 0);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

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
        txtcol = rgb(255, 160, 0);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

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
        txtcol = rgb(255, 160, 0);
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;

        txtset(t, "Name Display Colour");
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
        y5 = 64 * 7 + 22;
        txtcol = rgb(0, 0, 0);
        txtout(ps, x5, y5, t);
        txtout(ps, x5 + 1, y5, t);
        txtout(ps, x5 + 2, y5, t);
        txtout(ps, x5 + 2, y5 + 1, t);
        txtout(ps, x5 + 2, y5 + 2, t);
        txtout(ps, x5 + 1, y5 + 2, t);
        txtout(ps, x5, y5 + 2, t);
        txtout(ps, x5, y5 + 1, t);
        txtcol = rgb(255, 160, 0);
        if (u6o_namecolour) txtcol = u6o_namecolour;
        txtout(ps, x5 + 1, y5 + 1, t);
        txtfnt = fnt1;


        f += et;


        x = mx;
        y = my;


        if ((x >= 0) && (x < 320) && (y >= 128) && (y < 192)) {
            txtset(t, "Create a Character");
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
            y5 = 64 * 2 + 22;
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
        }

        if ((x >= 0) && (x < 320) && (y >= 192) && (y < 256)) {
            txtset(t, "Journey Onward");
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
            y5 = 64 * 3 + 22;
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
            txtset(t, "USER NAME: ");
            txtadd(t, u6o_user_name);
            txtadd(t, "        USER PASSWORD: ");
            txtadd(t, u6o_user_password);
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 256 + 64, 736 + 4, t);
        }

        if ((x >= 0) && (x < 320) && (y >= 256) && (y < 320)) {
            txtset(t, "Transfer a Character");
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
            y5 = 64 * 4 + 22;
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
            txtset(t, "CHANGE USER NAME AND USER PASSWORD");
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 256 + 64, 736 + 4, t);
        }

        if ((x >= 0) && (x < 320) && (y >= (256 + 64)) && (y < (320 + 64))) {
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
            txtset(t, "CHANGE DEFAULT KEYBOARD KEYS");
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 256 + 64 + 64, 736 + 4, t);
        }

        if ((x >= 0) && (x < 320) && (y >= (256 + 64 + 64)) && (y < (320 + 64 + 64))) {
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
            txtset(t, "UPLOAD A CUSTOM PORTRAIT FOR AN EXISTING CHARACTER");
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 256 + 64, 736 + 4, t);
        }


        if ((x >= 0) && (x < 320) && (y >= (256 + 64 + 64 + 64)) && (y < (320 + 64 + 64 + 64))) {
            txtset(t, "Name Display Colour");
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
            y5 = 64 * 7 + 22;
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
            if (u6o_namecolour) {
                x6 = u6o_namecolour & 255; //blue
                x7 = (u6o_namecolour >> 8) & 255; //green
                x8 = (u6o_namecolour >> 16) & 255; //red
                x6 += 64;
                if (x6 > 255) x6 = 255;
                x7 += 64;
                if (x7 > 255) x7 = 255;
                x8 += 64;
                if (x8 > 255) x8 = 255;
                txtcol = rgb(x8, x7, x6);
            }
            txtout(ps, x5 + 1, y5 + 1, t);
            txtfnt = fnt1;
            txtset(t, "SET THE COLOUR TO DISPLAY YOUR CHARACTER'S NAME WITH WHEN SENDING MESSAGES");
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 256 - 64 - 32, 736 + 4, t);
        }


        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if ((x >= 0) && (x < 320) && (y >= 128) && (y < 192)) {
                intro = 201;
                intro_setup = 0;
                goto intro_done;
            }
            if ((x >= 0) && (x < 320) && (y >= 256) && (y < 320)) {
                intro = 202;
                intro_setup = 0;
                goto intro_done;
            }
            if ((x >= 0) && (x < 320) && (y >= (256 + 64)) && (y < (320 + 64))) {
                intro = 206;
                intro_setup = 0;
                goto intro_done;
            }
            if ((x >= 0) && (x < 320) && (y >= (256 + 64 + 64)) && (y < (320 + 64 + 64))) {
                intro = 204;
                intro_setup = 0;
                goto intro_done;
            }

            if ((x >= 0) && (x < 320) && (y >= (256 + 64 + 64 + 64)) && (y < (320 + 64 + 64 + 64))) {
                x5 = getcol(u6o_namecolour);
                //assume a 24bit colour returned

                x6 = x5 & 255; //blue
                x7 = (x5 >> 8) & 255; //green
                x8 = (x5 >> 16) & 255; //red
                if ((x6 + x7 + x8) < 255) {
                    MessageBox(
                        NULL,
                        "That colour cannot be used because it's too dark! (The sum of the red, green and blue channels must be at least 255)",
                        "Ultima 6 Online", MB_OK);
                } else {
                    u6o_namecolour = x5; //initially sent as 24bit
                }

                goto intro_done;
            }

            //journey onward
            if ((x >= 0) && (x < 320) && (y >= 192) && (y < 256)) {
            login:
                txtset(t, "UNKNOWN");
                if (!txtsame(t, u6o_user_name)) {
                    if (!txtsame(t, u6o_user_password)) {
                        tfh = open(".\\dr\\controls.bin");
                        get(tfh, &U6OK, 512);
                        get(tfh, &U6OK_FLAGS, 128);
                        close(tfh);
                        u6o_ensure_control_defaults();

                        fs->offset_x = 1024;
                        fs->graphic = NULL;
                        intro = 0;
                        intro_setup = 0;
                        goto intro_done;
                    }
                }
                MessageBox(NULL, "Select CREATE A CHARACTER if you are a new player.", "Ultima 6 Online", MB_OK);
            }
        }
    } //intro==200


    //transfer a character


    if (intro == 202) {
        static txt *tusername2 = txtnew();
        static txt *tuserpassword2 = txtnew();
        static txt *tnewuserpassword2 = txtnew();
        static unsigned char intro_ep = 1;
        static unsigned char intro_ti = 0;
        if (intro_setup == 0) {
            intro_setup = 1;
            txtset(tusername2, "");
            txtset(tuserpassword2, "");
            txtset(tnewuserpassword2, "");
            f = 0;
            //select username text field
            intro_ep = 0;
            intro_ti = 2;
            GETINPUT_setup(tusername2, &intro_ep, 16);
            GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
        }
        f += et;

        img0(ps, 1024 - 25 - 22 - 2, 2, intro_back);
        img0(ps, 18, 25, intro_ultimavi);
        txtset(t, "Transfer a Character");
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
        y5 = 64 * 4 + 22;
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


        img0(ps, 0, 256 + 64 + 8, intro_tacinfo);

        txtcol = rgb(252, 200, 20);
        txtset(t, tusername2);
        if (((long) (f * 4) & 1) && (intro_ti == 2) && (intro_ep == 0)) txtadd(t, "|");
        img(ps, 120 + 4, 320 + 4 + 4, intro_ifield);
        txtout(ps, 120 + 4 + 4, 320 + 4 - 1, t);
        txtset(t, tuserpassword2);
        if (((long) (f * 4) & 1) && (intro_ti == 3) && (intro_ep == 0)) txtadd(t, "|");
        img(ps, 120 + 4, 320 + 32 + 4 + 4, intro_ifield);
        txtout(ps, 120 + 4 + 4, 320 + 32 + 4 - 1, t);
        txtset(t, tnewuserpassword2);
        if (((long) (f * 4) & 1) && (intro_ti == 4) && (intro_ep == 0)) txtadd(t, "|");
        img(ps, 120 + 4, 320 + 32 + 4 + 4 + 96, intro_ifield);
        txtout(ps, 120 + 4 + 4, 320 + 32 + 4 - 1 + 96, t);

        x = mx;
        y = my;

        if ((x >= 0) && (x < 320) && (y >= (352 + 96)) && (y < (384 + 96))) {
            txtset(t, "ONLY SET IF CHANGING YOUR EXISTING PASSWORD");
            txtcol = rgb(255, 255, 255);
            txtouts(ps, 256 + 64, 736 + 4, t);
        }


        //switch to next field if tab pressed!
        if (GETINPUT_tab_pressed == GETINPUT_TAB_PRESSED) {
            GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_STOP;
            if (intro_ti == 4) {
                intro_ep = 0;
                intro_ti = 2;
                GETINPUT_setup(tusername2, &intro_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
                goto transferachar_newfieldselected;
            }
            if (intro_ti == 2) {
                intro_ep = 0;
                intro_ti = 3;
                GETINPUT_setup(tuserpassword2, &intro_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
                goto transferachar_newfieldselected;
            }
            if (intro_ti == 3) {
                intro_ep = 0;
                intro_ti = 4;
                GETINPUT_setup(tnewuserpassword2, &intro_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
                goto transferachar_newfieldselected;
            }
        } //GETINPUT_TAB_PRESSED
    transferachar_newfieldselected:

        if (keyhit(VK_RETURN)) goto transferachar_enterkey;


        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if ((x > (1024 - 21 - 4 - 26)) && (y < (21 + 4))) {
                if (intro_ep == 0) {
                    GETINPUT_stop();
                }
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }


            if ((x >= 0) && (x < 320) && (y >= 320) && (y < 352)) {
                intro_ep = 0;
                intro_ti = 2;
                GETINPUT_setup(tusername2, &intro_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
            }
            if ((x >= 0) && (x < 320) && (y >= 352) && (y < 384)) {
                intro_ep = 0;
                intro_ti = 3;
                GETINPUT_setup(tuserpassword2, &intro_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
            }
            if ((x >= 0) && (x < 320) && (y >= (352 + 96)) && (y < (384 + 96))) {
                intro_ep = 0;
                intro_ti = 4;
                GETINPUT_setup(tnewuserpassword2, &intro_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
            }

            if ((x >= 0) && (x < 320) && (y >= 384) && (y < 416)) {
            transferachar_enterkey:

                if (tusername2->l == 0) {
                    MessageBox(NULL, "Please select a USER NAME", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }
                if (tuserpassword2->l == 0) {
                    MessageBox(NULL, "Please select a USER PASSWORD", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }

                if (intro_ep == 0) {
                    GETINPUT_stop();
                }
                txtset(u6o_user_name, tusername2);
                txtset(u6o_user_password, tuserpassword2);
                if (tnewuserpassword2->l) {
                    txtset(u6o_new_user_password, tnewuserpassword2);
                }

                tfh = open2("userinfo.txt", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                txtfileout(u6o_user_name, tfh);
                if (tnewuserpassword2->l) txtfileout(u6o_new_user_password, tfh);
                else txtfileout(u6o_user_password, tfh);
                close(tfh);

                //save walkthru pos
                txtset(t, ".\\dr\\walkthru.pos");
                tfh = open2(t, OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                put(tfh, &walkthru_pos, 4); //lines to skip of the walkthru
                close(tfh);
                walkthru_pos_skip = 0;

                tfh = open(".\\dr\\controls.bin");
                get(tfh, &U6OK, 512);
                get(tfh, &U6OK_FLAGS, 128);
                close(tfh);
                u6o_ensure_control_defaults();

                fs->offset_x = 1024;
                fs->graphic = NULL;
                intro = 0;
                intro_setup = 0;
                goto intro_done;
            }
        } //->mouse_click
    } //intro==202
