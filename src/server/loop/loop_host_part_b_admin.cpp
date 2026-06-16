// LHS-P4.5: part_b_admin -- CONTINUES MEGA B. WIRE + admin.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch.
// Contents: sysban (sysban_nextplayer) + start of the slash-command/privilege
// dispatch. Cross-ref docs/admin/README.md (line refs there are stale).
        tfh = open2("sysban.txt", OF_READWRITE | OF_SHARE_COMPAT);
        if (tfh->h != HFILE_ERROR) {
        sysban_nextplayer:
            txtfilein(t, tfh);
            txtucase(t);
            if (txtsame(t, t2)) {
                txtsetchar(t3, 247);
                txtaddchar(t3, 1);
                NET_send(NETplayer, playerlist[tpl]->net, t3); //SYSBAN ON
            }
            if (t->l) goto sysban_nextplayer;
            close(tfh);
        }


        for (x = 0; x <= 7; x++) {
            if (playerlist[tpl]->party[x]) {
                if (playerlist[tpl]->GNPCflags[230 + x]) {
                    myobj2 = playerlist[tpl]->party[x];
                    tnpc2 = (npc *) myobj2->more;
                    //move them to death spot
                    OBJmove_allow = TRUE;
                    OBJmove2(myobj2, playerlist[tpl]->GNPCflags[230 + x] & 65535,
                             playerlist[tpl]->GNPCflags[230 + x] >> 16);
                    OBJmove_allow = FALSE;
                    playerlist[tpl]->GNPCflags[230 + x] = 0;
                    //now kill them using a callback!
                    //the callback will do all the hard work for me
                    tplayer = playerlist[tpl];
                    myobj2 = tplayer->party[x];
                    tnpc = (npc *) myobj2->more;
                    login_dead_callback = 1;
                    goto login_dead;
                login_dead2:
                    login_dead_callback = 0;
                }
            }
        }
        //tplayer->GNPCflags[230+resu_partymember[i3]]=resu_x[i3]+(resu_y[i3]<<16);


        goto doneclmess;
    foundclient:


        playerlist[tpl]->idle_connect = 0;
        if (t->d2[0] == 251) goto doneclmess;

        if (playerlist[tpl]->playinstrument) {
            //stop playing instrument?
            x2 = 0;
            x = t->d2[0];
            if (x == 5) x2 = 1;
            if (x == 6) x2 = 1;
            if (x == 7) x2 = 1;
            if (x == 12) x2 = 1;
            if (x == 13) x2 = 1;
            if (x == 15) x2 = 1;
            if (x == 16) x2 = 1;
            if (x == 17) x2 = 1;
            if (x == 19) x2 = 1;
            if (x == 20) x2 = 1;
            if (x == 21) x2 = 1;
            if (x2) {
                playerlist[tpl]->playinstrument--;
                if (playerlist[tpl]->playinstrument == 0) {
                    txtset(t2, "?");
                    t2->d2[0] = 29;
                    NET_send(NETplayer, playerlist[tpl]->net, t2);
                } //==0
            } //x2
        } //playinstrument

        //cancel wizardeye
        if (playerlist[tpl]->wizardeyetimeleft) {
            x2 = 0;
            x = t->d2[0];
            if (x == 6) x2 = 1;
            if (x == 7) x2 = 1;
            if (x == 12) x2 = 1;
            if (x == 13) x2 = 1;
            if (x == 15) x2 = 1;
            if (x == 16) x2 = 1;
            if (x == 17) x2 = 1;
            if (x == 19) x2 = 1;
            if (x == 20) x2 = 1;
            if (x == 21) x2 = 1;
            if (x2) {
                txtset(t2, "??");
                t2->d2[0] = 36;
                t2->d2[1] = 0;
                NET_send(NETplayer, playerlist[tpl]->net, t2);
                playerlist[tpl]->wizardeyetimeleft = 0.0f;
            }
        }


        //process client message

        //???? if (NETplayer==tnet) CLIENTplayer=playerlist[tpl]; //CLIENTplayer<-tpl switch to player tpl


        if (t->d2[0] == 244) {
            //send leaving britannia message
            if (playerlist[tpl]->party[0]) {
                myobj = playerlist[tpl]->party[0];
                tnpc = (npc *) myobj->more;
                //1. send status message to ALL PLAYERS
                txtsetchar(t2, 8);
                txtadd(t2, "Leaving Britannia: ");
                txtadd(t2, tnpc->name);
                for (z = 0; z <= playerlist_last; z++) {
                    if (z != tpl) {
                        if (playerlist[z]) {
                            if (playerlist[z]->net) {
                                NET_send(NETplayer, playerlist[z]->net, t2);
                            }
                        }
                    }
                }
                //2. send a local text sfx message
                //Virtue @is @leaving @Britannia (the red text is specifically to stop people pretending to leave)
                i3 = SFnew(myobj->x, myobj->y);
                sf[i3].type = 3;
                sf[i3].x = myobj->x;
                sf[i3].y = myobj->y - 2;
                sf[i3].wait = 2;
                sf[i3].more = tnpc->port;
                static txt *inouttxt;
                inouttxt = txtnew();
                txtset(inouttxt, "");
                txtset(t2, tnpc->name);
                txtadd(t2, " is leaving Britannia.");
                txtaddcolouredname(inouttxt, t2, playerlist[tpl]);
                sf[i3].p = inouttxt;
            } //party[0]
            goto doneclmess;
        }

        if (t->d2[0] == 248) {
            //respawn
            playerlist[tpl]->respawn = 255;
            if (t->l == 2) playerlist[tpl]->respawn = t->d2[1];
            goto doneclmess;
        }

        if (t->d2[0] == 5) {
            playerlist[tpl]->idle = 0;


            txtright(t, t->l - 1);
            if (playerlist[tpl]->readmessage == FALSE) {
                memcpy(&playerlist[tpl]->mf, t->d, (DWORD) & playerlist[tpl]->mobj - (DWORD) & playerlist[tpl]->mf);
                playerlist[tpl]->readmessage = TRUE;
            } //not second general input message
            else {
                //second input message (interpret carefully)


                //update arrow keys
                x = t->d2[7] + (t->d2[8] << 8) + (t->d2[9] << 16) + (t->d2[10] << 24);

                if (playerlist[tpl]->key & KEYup) playerlist[tpl]->key -= KEYup;
                playerlist[tpl]->key += (x & KEYup);
                if (playerlist[tpl]->key & KEYdown) playerlist[tpl]->key -= KEYdown;
                playerlist[tpl]->key += (x & KEYdown);
                if (playerlist[tpl]->key & KEYleft) playerlist[tpl]->key -= KEYleft;
                playerlist[tpl]->key += (x & KEYleft);
                if (playerlist[tpl]->key & KEYright) playerlist[tpl]->key -= KEYright;
                playerlist[tpl]->key += (x & KEYright);

                playerlist[tpl]->key |= (x & KEYup2);
                playerlist[tpl]->key |= (x & KEYdown2);
                playerlist[tpl]->key |= (x & KEYleft2);
                playerlist[tpl]->key |= (x & KEYright2);
            } //second general input message

            //cancel wizardeye
            if (playerlist[tpl]->key & (KEYmbclick + KEYbreakcombat + KEYquan + KEYf1)) {
                if (playerlist[tpl]->wizardeyetimeleft) {
                    if (playerlist[tpl]->key & KEYmbclick) playerlist[tpl]->key -= KEYmbclick;
                    txtset(t2, "??");
                    t2->d2[0] = 36;
                    t2->d2[1] = 0;
                    NET_send(NETplayer, playerlist[tpl]->net, t2);
                    playerlist[tpl]->wizardeyetimeleft = 0.0f;
                }
            }
            goto doneclmess;
        } //5


        if (t->d2[0] == 37) {
            if (t->l <= 3072) {
                //IMPORTANT: ANY VOICE DATA ABOVE 3K IS IGNORED
                if (playerlist[tpl]->GNPCflags[33] & 4) {
                    txtsetchar(t, 250);
                    txtaddchar(t, 10);
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }
                if (myobj = getprimarypartymemberobj(playerlist[tpl])) {
                    tnpc = (npc *) myobj->more;
                    txtsetchar(t2, 38);
                    txtaddshort(t2, tnpc->port);

                    txtset(t4, tnpc->name);
                    txtadd(t4, ":");
                    txtset(t3, "");
                    txtaddcolouredname(t3, t4, playerlist[tpl]);
                    txtaddchar(t2, t3->l);
                    txtadd(t2, t3);

                    txtaddshort(t2, playerlist[tpl]->x);
                    txtaddshort(t2, playerlist[tpl]->y);

                    txtset(t3, t2); //t3=info but no voice data
                    txtright(t, t->l - 1);
                    txtadd(t2, t); //add compressed voice data
                    x2 = playerlist[tpl]->x;
                    y2 = playerlist[tpl]->y;

                    for (z = 0; z <= playerlist_last; z++) {
                        if (z != tpl) {
                            if (playerlist[z]) {
                                if (playerlist[z]->net) {
                                    //x3=playerlist[z]->x-15; y3=playerlist[z]->y-11;
                                    //if (x3<0) x3=0; if (y3<0) y3=0; if (x3>2016) x3=2016; if (y3>1000) y3=1000;
                                    getscreenoffset(playerlist[z]->x, playerlist[z]->y, &x3, &y3);

                                    if ((x2 >= x3) && (x2 < (x3 + 32))) {
                                        if ((y2 >= y3) && (y2 < (y3 + 24))) {
                                            if (playerlist[z]->mixer_volume) {
                                                NET_send(NETplayer, playerlist[z]->net, t2);
                                            } else {
                                                NET_send(NETplayer, playerlist[z]->net, t3);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    } //x2 loop & checks
                } //myobj
            } //data size limit
            goto doneclmess;
        } //37

        if (t->d2[0] == 39) {
            //update player->miver_volume on host
            playerlist[tpl]->mixer_volume = t->d2[1];
            goto doneclmess;
        } //39

        if (t->d2[0] == 40) {
            //get volume levels of other players in area
            txtset(t, "?");
            txtset(t2, "????");
            txtset(t3, "?");
            t->d2[0] = 41;
            //(x2,y2)=top-left corner of requesting player's screen
            //x2=playerlist[tpl]->x-15; y2=playerlist[tpl]->y-11; if (x2<0) x2=0; if (y2<0) y2=0; if (x2>2016) x2=2016; if (y2>1000) y2=1000;
            getscreenoffset(playerlist[tpl]->x, playerlist[tpl]->y, &x2, &y2);

            for (z = 0; z <= playerlist_last; z++) {
                if (z != tpl) {
                    if (playerlist[z]) {
                        if (playerlist[z]->net) {
                            x3 = playerlist[z]->x;
                            y3 = playerlist[z]->y;
                            if ((x3 >= x2) && (x3 < (x2 + 32))) {
                                if ((y3 >= y2) && (y3 < (y2 + 24))) {
                                    if (myobj = playerlist[z]->party[0]) {
                                        txtaddlong(t, playerlist[z]->id);
                                        txtaddchar(t, playerlist[z]->mixer_volume);
                                    } else {
                                        //send volume at last known position
                                        txtaddlong(t, 0xFFFFFFFF); //ID not being sent, use static position
                                        txtaddshort(t, x3);
                                        txtaddshort(t, y3);
                                        txtaddchar(t, playerlist[z]->mixer_volume);
                                    }
                                }
                            }
                        }
                    }
                }
            } //z loop & checks
            if (t->l > 1) {
                NET_send(NETplayer, playerlist[tpl]->net, t);
            }
            if (playerlist[tpl]->GNPCflags[33] & 4) goto doneclmess; //don't update "speaking..." if banned
            playerlist[tpl]->speaking = 3.0f;
            goto doneclmess;
        } //40


        if (t->d2[0] == 42) {
            //request portrait data
            ////goto doneclmess;

            txtright(t, t->l - 1);
            x2 = t->ds[0];
            if (HOST_portrait_loaded[x2]) {
                txtsetchar(t, 43);
                txtaddshort(t, x2);
                /*
        LOSS-LESS COMPRESS PORTRAIT DATA
        loop:
        2bits    (V0-V3)

        V0 repeat last known (last known starts off as black)
        {
        5bits    1-32 repeats
        loop
        }

        V1 can't be created
        {
        5:5:5bits    fixed colour
        loop
        }

        V2 accend
        {
        3bits    blue
        3bits    green
        3bits    red
        loop
        }

        V3 decend
        {
        3bits    blue
        3bits    green
        3bits    red
        loop
        }

        */
                static unsigned long bitsi;
                bitsi = 0;

                static unsigned short *sp;
                txtNEWLEN(t2, -16384);

                static long r, g, b, r2, g2, b2, r3, g3, b3, repeat;

                sp = HOST_portrait_data[x2];
                b = 0;
                g = 0;
                r = 0;
                repeat = 0;

                for (x3 = 0; x3 <= 3583; x3++) {
                    b2 = sp[x3] & 31;
                    g2 = (sp[x3] >> 6) & 31;
                    r2 = (sp[x3] >> 11) & 31;

                    if ((b == b2) && (g == g2) && (r == r2) && (repeat < 32)) {
                        repeat++;
                        goto llc_pixeldone;
                    }
                    if (repeat) {
                        BITSadd(t2, &bitsi, 0, 2);
                        BITSadd(t2, &bitsi, repeat - 1, 5);
                        repeat = 0;
                    }

                    b3 = b2;
                    if (b3 < b) b3 += 32;
                    g3 = g2;
                    if (g3 < g) g3 += 32;
                    r3 = r2;
                    if (r3 < r) r3 += 32;
                    if (((b3 - b) <= 7) && ((g3 - g) <= 7) && ((r3 - r) <= 7)) {
                        //compression possible
                        if (((b3 - b) >= 0) && ((g3 - g) >= 0) && ((r3 - r) >= 0)) {
                            //compression possible

                            //if ((b2>=b)&&(g2>=g)&&(r2>=r)){//ascending
                            //if (((b2-b)<=7)&&((g2-g)<=7)&&((r2-r)<=7)){//compression possible
                            BITSadd(t2, &bitsi, 2, 2);
                            BITSadd(t2, &bitsi, b3 - b, 3);
                            BITSadd(t2, &bitsi, g3 - g, 3);
                            BITSadd(t2, &bitsi, r3 - r, 3);
                            goto llc_pixeldone;
                        }
                    }

                    b3 = b2;
                    if (b3 > b) b3 -= 32;
                    g3 = g2;
                    if (g3 > g) g3 -= 32;
                    r3 = r2;
                    if (r3 > r) r3 -= 32;
                    if (((b3 - b) >= -7) && ((g3 - g) >= -7) && ((r3 - r) >= -7)) {
                        //compression possible
                        if (((b3 - b) <= 0) && ((g3 - g) <= 0) && ((r3 - r) <= 0)) {
                            //compression possible

                            //if ((b2<=b)&&(g2<=g)&&(r2<=r)){//decending
                            //if (((b-b2)<=7)&&((g-g2)<=7)&&((r-r2)<=7)){//compression possible
                            BITSadd(t2, &bitsi, 3, 2);
                            BITSadd(t2, &bitsi, b - b3, 3);
                            BITSadd(t2, &bitsi, g - g3, 3);
                            BITSadd(t2, &bitsi, r - r3, 3);
                            goto llc_pixeldone;
                        }
                    }

                    BITSadd(t2, &bitsi, 1, 2); //no optomization was possible :(
                    BITSadd(t2, &bitsi, b2, 5);
                    BITSadd(t2, &bitsi, g2, 5);
                    BITSadd(t2, &bitsi, r2, 5);

                llc_pixeldone:
                    b = b2;
                    g = g2;
                    r = r2;
                } //x3

                if (repeat) {
                    BITSadd(t2, &bitsi, 0, 2);
                    BITSadd(t2, &bitsi, repeat - 1, 5);
                }
                t2->l = bitsi >> 3;
                if (bitsi & 7) t2->l++;
                t2->d2[t2->l] = 0;

                //exit(t2->l);
                //4776


                //BITSadd(t,&bitsi,z,8);//scene update
                /*

        3959 Kaldosh
        4447 Mose
        4714 Virtue

        1	if on the last known colour value (default is black) should be repeated
        if(on){

        loop
        }
        1	if on the following colour can't be "created"
        if (on){
        5:5:5 fixed colour
        loop
        }
        1   if on acending else decending











        static unsigned short *sp;
        txtNEWLEN(t2,-7168);
        //memcpy(t2->d,HOST_portrait_data[x2],7168);

        //6233

        //11 11 11
        /*
        bit 0: rle compression (yes/no)
        bit 1: increase brightness (yes/no)
        bits [2-3]: blue
        bits [4-5]: green
        bits [6-7]: red


        0 rle compression
        1 increase brightness
        2 decrease brightness
        3 ?


        bit 0 
        bit 1

        (if sensitive compression means increase brightness)
        bit 2-3 red (0-3)
        bit 2-3 green (0-3)
        bit 2-3 blue (0-3)

        next 6 bits






        sp=HOST_portrait_data[x2];
        y2=-1;
        y4=0;
        x5=0;
        for (x3=0;x3<=3584;x3++){
        y3=sp[x3]; if (y3&32) y3-=32;
        if ((y3!=y2)||(y4==128)||(x3==3584)){
        if (y2!=-1){
        if (y4){
        //repeats
        t2->d2[x5]=(y2&255)+32; x5++;
        t2->d2[x5]=(y2>>8)&255; x5++;
        t2->d2[x5]=y4-1; x5++;
        }else{//y4
        //no repeats
        t2->d2[x5]=y2&255; x5++;
        t2->d2[x5]=(y2>>8)&255; x5++;
        }//y4
        }//y2!=-1
        y2=y3;
        y4=0;
        }else{
        y4++;
        }
        }//x3
        t2->d2[x5]=0;
        t2->l=x5;

        //rescan using sensitive compression avoiding rle content
        static long r,g,b,r2,g2,b2;
        txtNEWLEN(t3,-7168);
        y2=-1;//last known colour value
        x5=0;
        for (x3=0;x3<t2->l;x3++){
        y3=t2->d2[x3];
        if (y3&32){//rle content
        y2=-1; //(y3-32)+(t2->d2[x3+1]<<8);//last known color value
        t3->d2[x5]=y3; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++;
        goto skiprlecontent;
        }
        if (y2!=-1){
        //is y3 close to y2
        b=y2&31; g=(y2>>6)&31; r=(y2>>11)&31;
        b2=y3&31; g2=(y3>>6)&31; r2=(y3>>11)&31;
        y4=0;
        if ((b2>=b)&&(g2>=g)&&(r2>=r)) y4=1;
        if ((b2<=b)&&(g2<=g)&&(r2<=r)) y4=-1;
        if (y4==1){
        if (((b2-b)<=3)&&((g2-g)<=3)&&((r-r2)<=3)){
        MessageBox(NULL,"saved another assending byte","Ultima 6 Online",MB_OK);
        }
        }
        if (y4==-1){
        if (((b-b2)<=3)&&((g-g2)<=3)&&((r-r2)<=3)){
        MessageBox(NULL,"saved another decending byte","Ultima 6 Online",MB_OK);
        }
        }
        //add as is
        t3->d2[x5]=y3; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++;
        }else{
        //add as is
        t3->d2[x5]=y3; x5++; x3++;
        t3->d2[x5]=t2->d2[x3]; x5++;
        }
        y2=y3;//last known colour value
        skiprlecontent:;
        }//x3
        t3->d2[x5]=0;
        t3->l=x5;








        exit (t2->l);

        */


                txtadd(t, t2);
                NET_send(NETplayer, playerlist[tpl]->net, t);
            } //HOST_portrait_loaded[x2]
            goto doneclmess;
        } //42


        if (t->d2[0] == 45) {
            if (myobj = playerlist[tpl]->party[t->d2[1]]) {
                tnpc = (npc *) myobj->more;
                x2 = t->ds[1] - 120;
                y2 = t->ds[2] - 120;
                if ((x2 >= 0) && (y2 >= 0)) {
                    x2 /= 32;
                    y2 /= 32;
                    x2 = y2 * 4 + x2;
                    x2 += tnpc->baseitem_offset;
                    if (myobj = (object *) tnpc->baseitem->more) {
                    setnot4sale:
                        if (myobj) {
                            if (x2) {
                                x2--;
                                myobj = (object *) myobj->next;
                                goto setnot4sale;
                            }

                            if ((obji[sprlnk[myobj->type & 1023]].flags & (1 + 2 + 4 + 8 + 16 + 32 + 64)) == 0) {
                                txtsetchar(t2, 8);
                                txtaddchar(t2, 255);
                                txtadd(t2, "Only wieldable items may be marked as not for sale.");
                                NET_send(NETplayer, playerlist[tpl]->net, t2);
                                goto doneclmess;
                            }

                            if (myobj->info & 256) {
                                txtsetchar(t2, 8);
                                txtaddchar(t2, 255);
                                txtadd(t2, "Quest items cannot be marked as not for sale.");
                                NET_send(NETplayer, playerlist[tpl]->net, t2);
                                goto doneclmess;
                            }

                            if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].flags & 4096) {
                                txtsetchar(t2, 8);
                                txtaddchar(t2, 255);
                                txtadd(t2, "That cannot be marked as not for sale.");
                                NET_send(NETplayer, playerlist[tpl]->net, t2);
                                goto doneclmess;
                            }

                            if (obji[sprlnk[myobj->type & 1023]].flags & 1024) {
                                //container
                                txtsetchar(t2, 8);
                                txtaddchar(t2, 255);
                                txtadd(t2, "That cannot be marked as not for sale.");
                                NET_send(NETplayer, playerlist[tpl]->net, t2);
                                goto doneclmess;
                            }

                            if (myobj->type == 16) {
                                //magic shield
                                txtsetchar(t2, 8);
                                txtaddchar(t2, 255);
                                txtadd(t2, "That cannot be marked as not for sale.");
                                NET_send(NETplayer, playerlist[tpl]->net, t2);
                                goto doneclmess;
                            }

                            myobj->info ^= 32768;
                            tnpc->upflags |= 32;
                        } //myobj
                    } //myobj of baseitem->more
                } //x2,y2
                goto doneclmess;
            }
        } //45
