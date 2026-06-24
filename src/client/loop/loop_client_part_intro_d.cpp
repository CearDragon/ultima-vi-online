// LCS-P3.5: part_intro_d — intro state machine, final chunk.
// LCS brace-seam: this file CONTINUES + CLOSES the `if (intro) { ... }` block
// opened in loop_client_part_intro_a.cpp. Contains intro state 201
// (create-a-character) plus the `intro_done:` label, the intro timer tail, and
// the closing `}` whose last statement is `goto intro_refresh;`. MUST stay the
// last intro part — its trailing `}` balances the brace opened in intro_a, and
// `intro_refresh:` lives in loop_client_part_refresh_tail.cpp (same TU). Pure
// relocation — do not edit. See docs/plans/plan-loopClientSplit.md.

    //create a char
    if (intro == 201) {
        static unsigned char sex = 0;
        static unsigned char portn = 0;
        static unsigned char typen = 0;
        static txt *tname2 = txtnew();
        static txt *tusername2 = txtnew();
        static txt *tuserpassword2 = txtnew();
        static unsigned char intro201_ep = 1;
        static unsigned char intro201_ti = 0;
        static object *typen_obj = OBJnew_local();

        if (intro_setup == 0) {
            intro_setup = 1;
            txtset(tname2, "");
            txtset(tusername2, "");
            txtset(tuserpassword2, "");
            sex = 0;
            portn = 0;
            typen = 0;
            intro201_ep = 1;
            intro201_ti = 0;

            //select username text field
            intro201_ep = 0;
            intro201_ti = 2;
            GETINPUT_setup(tusername2, &intro201_ep, 16);
            GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;

            f = 0;
        }
        f += et;

        //show port
        x3 = portn;
        if (sex) {
            if (x3 == 0) x4 = 194;
            if (x3 == 1) x4 = 195;
            if (x3 == 2) x4 = 197;
            if (x3 == 3) x4 = 198;
            if (x3 == 4) x4 = 202;
            if (x3 == 5) x4 = 203;
            if (x3 == 6) x4 = 207;
        } else {
            if (x3 == 0) x4 = 196;
            if (x3 == 1) x4 = 199;
            if (x3 == 2) x4 = 200;
            if (x3 == 3) x4 = 201;
            if (x3 == 4) x4 = 204;
            if (x3 == 5) x4 = 205;
            if (x3 == 6) x4 = 206;
        }

        img0(ps, 99 + 8, 168 + 320, getportrait(x4));

        img0(ps, 1024 - 25 - 22 - 2, 2, intro_back);
        img0(ps, 18, 25, intro_ultimavi);
        txtset(t, "Create a Character");
        txtfnt = fnt7;
        tagxy.cx = 0;
        tagxy.cy = 0;
        ps->s->GetDC(&taghdc);
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

        if (intro201_ep || fs->mouse_click) {
            //show auto-formatting
            //name
            txtset(t5, tname2);
        autoformat_name_cull3:
            for (i3 = 0; i3 < t5->l; i3++) {
                x = t5->d2[i3];
                x2 = 0; //cull?
                if (x < 48) x2 = 1;
                if ((x > 57) && (x < 65)) x2 = 1;
                if ((x > 90) && (x < 97)) x2 = 1;
                if (x > 122) x2 = 1;
                if ((x == 45) && (t5->d2[i3 + 1] != 45)) x2 = 0; //-
                if ((x == 39) && (t5->d2[i3 + 1] != 39)) x2 = 0; //'
                if ((x == 46) && (t5->d2[i3 + 1] != 46)) x2 = 0; //.
                if (x == 95) {
                    t5->d2[i3] = 32;
                    x = 32;
                    x2 = 0;
                    goto autoformat_name_cull3;
                } //_ to space
                if ((x == 32) && (t5->d2[i3 + 1] != 32)) {
                    if (i3 && (i3 != (t5->l - 1))) x2 = 0;
                }
                if (x2) {
                    txtset(t6, t5);
                    txtright(t6, t5->l - i3 - 1);
                    txtleft(t5, i3);
                    txtadd(t5, t6);
                    goto autoformat_name_cull3;
                }
            }
            if (t5->l > 16) txtleft(t5, 16);
            txtset(tname2, t5);
            //username
            txtset(t5, tusername2);
            txtucase(t5);
        autoformat_username_cull3:
            for (i3 = 0; i3 < t5->l; i3++) {
                x = t5->d2[i3];
                x2 = 0; //cull?
                if (x < 48) x2 = 1;
                if (x > 90) x2 = 1;
                if ((x > 57) && (x < 65)) x2 = 1;
                if (x2) {
                    txtset(t6, t5);
                    txtright(t6, t5->l - i3 - 1);
                    txtleft(t5, i3);
                    txtadd(t5, t6);
                    goto autoformat_username_cull3;
                }
            }
            if (t5->l > 16) txtleft(t5, 16);
            txtset(tusername2, t5);
            //password
            txtset(t5, tuserpassword2);
            txtucase(t5);
        autoformat_password_cull3:
            for (i3 = 0; i3 < t5->l; i3++) {
                x = t5->d2[i3];
                x2 = 0; //cull?
                if (x < 48) x2 = 1;
                if (x > 90) x2 = 1;
                if ((x > 57) && (x < 65)) x2 = 1;
                if (x2) {
                    txtset(t6, t5);
                    txtright(t6, t5->l - i3 - 1);
                    txtleft(t5, i3);
                    txtadd(t5, t6);
                    goto autoformat_password_cull3;
                }
            }
            if (t5->l > 16) txtleft(t5, 16);
            txtset(tuserpassword2, t5);
        } //intro201_ep||fs->mouse_click


        txtcol = rgb(252, 200, 20);
        txtset(t, tusername2);
        if (((long) (f * 4) & 1) && (intro201_ti == 2) && (intro201_ep == 0)) txtadd(t, "|");
        img(ps, 99 + 4, 320 + 4 + 4, intro_ifield);
        txtout(ps, 99 + 4 + 4, 320 + 4 - 1, t);
        txtset(t, tuserpassword2);
        if (((long) (f * 4) & 1) && (intro201_ti == 3) && (intro201_ep == 0)) txtadd(t, "|");
        img(ps, 99 + 4, 320 + 32 + 4 + 4, intro_ifield);
        txtout(ps, 99 + 4 + 4, 320 + 32 + 4 - 1, t);
        txtset(t, tname2);
        if (((long) (f * 4) & 1) && (intro201_ti == 1) && (intro201_ep == 0)) txtadd(t, "|");
        img(ps, 99 + 4, 320 + 32 * 2 + 4 + 4, intro_ifield);
        txtout(ps, 99 + 4 + 4, 320 + 32 * 2 + 4 - 1, t);

        x3 = 410;
        if (typen != 0) x3 = 375 + typen;
        x4 = (long) (f * 2.0f) % 16;
        typen_obj->type = x3 + x4 * 1024;
        getspr(typen_obj);
        img0(ps, 99 + 8 + 12, 136 + 320 - 8, bt32);


        img0(ps, 0, 320, intro_newchar2);


        //switch to next field if tab pressed!
        if (GETINPUT_tab_pressed == GETINPUT_TAB_PRESSED) {
            GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_STOP;
            if (intro201_ti == 1) {
                intro201_ep = 0;
                intro201_ti = 2;
                GETINPUT_setup(tusername2, &intro201_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
                goto createachar_newfieldselected;
            }
            if (intro201_ti == 2) {
                intro201_ep = 0;
                intro201_ti = 3;
                GETINPUT_setup(tuserpassword2, &intro201_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
                goto createachar_newfieldselected;
            }
            if (intro201_ti == 3) {
                intro201_ep = 0;
                intro201_ti = 1;
                GETINPUT_setup(tname2, &intro201_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
                goto createachar_newfieldselected;
            }
        } //GETINPUT_TAB_PRESSED
    createachar_newfieldselected:

        if (keyhit(VK_RETURN)) goto createachar_enterkey;


        x = mx;
        y = my;
        if (fs->mouse_click) {
            fs->mouse_click = NULL;

            if ((x > (1024 - 21 - 4 - 26)) && (y < (21 + 4))) {
                if (intro201_ep == 0) {
                    GETINPUT_stop();
                }
                intro = 200;
                intro_setup = 0;
                goto intro_done;
            }

            if ((x >= 0) && (x < 320) && (y >= 416) && (y < 448)) {
                sex++;
                if (sex > 1) sex = 0;
            }

            if ((x >= 0) && (x < 320) && (y >= 448) && (y < 480)) {
                typen++;
                if (typen > 12) typen = 0;
            }

            if ((x >= 0) && (x < 320) && (y >= 480) && (y < 512)) {
                portn++;
                if (portn > 6) portn = 0;
            }


            if ((x >= 0) && (x < 320) && (y >= 320) && (y < 352)) {
                intro201_ep = 0;
                intro201_ti = 2;
                GETINPUT_setup(tusername2, &intro201_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
            }
            if ((x >= 0) && (x < 320) && (y >= 352) && (y < 384)) {
                intro201_ep = 0;
                intro201_ti = 3;
                GETINPUT_setup(tuserpassword2, &intro201_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
            }
            if ((x >= 0) && (x < 320) && (y >= 384) && (y < 416)) {
                intro201_ep = 0;
                intro201_ti = 1;
                GETINPUT_setup(tname2, &intro201_ep, 16);
                GETINPUT_tab_pressed = GETINPUT_TAB_PRESSED_INIT;
            }

            if ((x >= 0) && (x < 320) && (y >= 576) && (y < 608)) {
                //continue
            createachar_enterkey:

                //error checking MUST OCCUR HERE!
                if (tusername2->l == 0) {
                    MessageBox(NULL, "Please select a USER NAME", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }
                if (tuserpassword2->l == 0) {
                    MessageBox(NULL, "Please select a USER PASSWORD", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }
                if (tname2->l == 0) {
                    MessageBox(NULL, "Please select a NAME", "Ultima 6 Online", MB_OK);
                    goto intro_done;
                }

                if (NEThost == NULL) {
                    //check if username or charname is taken
                    txtsetchar(t, 246);
                    txtaddchar(t, tusername2->l);
                    txtadd(t, tusername2);
                    txtaddchar(t, tname2->l);
                    txtadd(t, tname2);
                    NET_send(NETplayer, NULL, t);

                    //wait for a reply
                    static unsigned long cac_time;
                    cac_time = timeGetTime();
                cac_waitforhost:
                    x3 = 0;
                    i = 0;
                    if (socketclient_ri[i]->d[socketclient_ri[i]->next]->l) {
                        //buffered data available
                        txtset(t, socketclient_ri[i]->d[socketclient_ri[i]->next]);
                        txtNEWLEN(socketclient_ri[i]->d[socketclient_ri[i]->next], 0);
                        socketclient_ri[i]->next++;
                        x3 = 1;
                    } //->l
                    if (timeGetTime() > (cac_time + 8000)) goto intro_done;
                    if (!x3) goto cac_waitforhost;
                    if (t->d2[0] != 245) goto cac_waitforhost; //incorrect message!

                    //0 neither username or charname is taken
                    //1 username is taken
                    //2 charname is taken
                    if (t->d2[1] == 1) {
                        MessageBox(NULL, "USER NAME taken. Please choose a different USER NAME", "Ultima 6 Online",
                                   MB_OK);
                        goto intro_done;
                    }
                    if (t->d2[1] == 2) {
                        MessageBox(NULL, "Character name taken. Please choose a different NAME", "Ultima 6 Online",
                                   MB_OK);
                        goto intro_done;
                    }
                } //NEThost==NULL


                if (intro201_ep == 0) {
                    GETINPUT_stop();
                }

                //allocate info
                txtset(u6o_user_name, tusername2);
                txtset(u6o_user_password, tuserpassword2);
                txtset(u6o_name, tname2);
                u6o_malefemale = sex;
                u6o_type = typen;
                x3 = portn;
                if (sex) {
                    if (x3 == 0) x4 = 194;
                    if (x3 == 1) x4 = 195;
                    if (x3 == 2) x4 = 197;
                    if (x3 == 3) x4 = 198;
                    if (x3 == 4) x4 = 202;
                    if (x3 == 5) x4 = 203;
                    if (x3 == 6) x4 = 207;
                } else {
                    if (x3 == 0) x4 = 196;
                    if (x3 == 1) x4 = 199;
                    if (x3 == 2) x4 = 200;
                    if (x3 == 3) x4 = 201;
                    if (x3 == 4) x4 = 204;
                    if (x3 == 5) x4 = 205;
                    if (x3 == 6) x4 = 206;
                }
                u6o_portrait = x4 - 194;
                intro = 102;
                intro_setup = 0;
                goto intro_done; //->gypsy intro
            }
        }
    } //intro==201


intro_done:


    img0(ps, 1024 - 21 - 2, 2, intro_x);


    intro_timer = f;
    goto intro_refresh;
}
