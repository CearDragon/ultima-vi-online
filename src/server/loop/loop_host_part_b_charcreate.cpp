// LHS-P4.3: part_b_charcreate -- CONTINUES MEGA B. WIRE.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch.
// Contents: ban (ban_nextplayer), character create/restore
// (save_createcharacter, save_restored).
    ban_nextplayer:
        txtfilein(t6, tfh);
        if (txtsamewithoutcase(t6, tname)) {
            playerlist[tpl]->GNPCflags[253] |= 1;
            txtset(t7, "??");
            t7->d2[0] = 250;
            t7->d2[1] = 8;
            NET_send(NETplayer, playerlist[tpl]->net, t7);
        }
        if (t6->l) goto ban_nextplayer;
        close(tfh);


        //create custom character portrait (if required)
        /*
    if (t5->d2[0]==1){//upload character portrait
    tcustomportrait_upload=1; txtright(t5,t5->l-1);
    memcpy(&tcustomportrait,t5->d,7168); txtright(t5,t5->l-7168);
    goto getsetupmessageinfo_more;
    }
    */


        tnpc3 = (npc *) playerlist[tpl]->party[0]->more;

        //grant full privileges to admin players
        x = 0;
        while (admins[x]) {
            if (txtsame(tnpc3->name, admins[x])) {
                for (i = 40; i <= 47; i++) playerlist[tpl]->GNPCflags[i] = 0xFFFFFFFF;
                break;
            }
            x++;
            if (x == ADMINSMAX) { break; }
        }

        //portraits 2.0 backup original portrait so it can be restored
        if (playerlist[tpl]->GNPCflags[34] == NULL) playerlist[tpl]->GNPCflags[34] = tnpc3->port;
        tnpc3->port = playerlist[tpl]->GNPCflags[34];

        if (tcustomportrait_upload) {
            if ((playerlist[tpl]->GNPCflags[33] & 2) == 0) {
                //not blocked from uploading custom portraits
                if (tnpc3->lev >= 3) {
                    if (tnpc3->exp >= 1000) {
                        tnpc3->exp -= 1000;

                        HOST_portrait_data[HOST_portrait_next] = (unsigned short *) malloc(7168);
                        memcpy(HOST_portrait_data[HOST_portrait_next], &tcustomportrait, 7168);
                        HOST_portrait_loaded[HOST_portrait_next] = 1;
                        tnpc3->port = HOST_portrait_next;
                        HOST_portrait_next++;

                        //add to database!
                        //get/add index
                        z = 0;
                        tfh = open(".\\save\\port\\index.txt");
                        txtset(t2, "=");
                        txtset(t4, tnpc3->name);
                        txtucase(t4);
                    saveportindex_loop:
                        txtfilein(t, tfh);
                        if (x = txtsearch(t, t2)) {
                            txtset(t3, t);
                            txtleft(t3, x - 1);
                            z = txtnum(t3);
                            txtright(t, t->l - x); //name
                            if (txtsame(t, t4)) {
                                goto saveportindex_found;
                            } //txtsame(t3,t4)
                            goto saveportindex_loop;
                        }
                        seek(tfh, lof(tfh));
                        z++;
                        txtnumint(t, z);
                        txtadd(t, t2);
                        txtadd(t, t4);
                        txtfileout(t, tfh);
                    saveportindex_found:
                        close(tfh);
                        //create file
                        static unsigned char bmpdata[54];
                        txtset(t, ".\\save\\port\\");
                        txtnumint(t2, z);
                        txtadd(t, t2);
                        txtadd(t, ".BMP");
                        tfh = open2(t->d, OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                        tfh2 = open(".\\host\\bmpdata.bmp");
                        get(tfh2, &bmpdata, 54);
                        close(tfh2);
                        put(tfh, &bmpdata, 54);
                        for (x2 = 63; x2 >= 0; x2--) {
                            for (x = 0; x < 56; x++) {
                                y = tcustomportrait[x2 * 56 + x] & 31;
                                y *= 8;
                                put(tfh, &y, 1);
                                y2 = tcustomportrait[x2 * 56 + x] & 2016;
                                y2 >>= 5;
                                y2 *= 4;
                                put(tfh, &y2, 1);
                                y3 = tcustomportrait[x2 * 56 + x] & 63488;
                                y3 >>= 11;
                                y3 *= 8;
                                put(tfh, &y3, 1);
                            }
                        }
                        close(tfh);
                        playerlist[tpl]->GNPCflags[33] |= 1; //check for existing custom portrait on entry
                        goto custromportraitload_skip;
                    } //tnpc3->exp>=1000
                } //tnpc3->lev>=3
            } //not blocked from uploading custom portraits
        } //tcustomportrait_upload

        //customportraitload
        if (playerlist[tpl]->GNPCflags[33] & 1) {
            tfh = open(".\\save\\port\\index.txt");
            txtset(t2, "=");
            txtset(t4, tnpc3->name);
            txtucase(t4);
        custromportraitload_loop:
            txtfilein(t, tfh);
            if (x = txtsearch(t, t2)) {
                txtset(t3, t);
                txtleft(t3, x - 1);
                z = txtnum(t3);
                txtright(t, t->l - x); //name
                if (txtsame(t, t4)) {
                    txtset(t, ".\\save\\port\\");
                    txtadd(t, t3);
                    txtadd(t, ".BMP");
                    //ADD SECURITY CHECKS HERE
                    static surf *custromportraitload_surf;
                    custromportraitload_surf = NULL;
                    custromportraitload_surf = loadimage(t->d, SURF_SYSMEM16);
                    if (custromportraitload_surf) {
                        HOST_portrait_data[HOST_portrait_next] = (unsigned short *) malloc(7168);
                        memcpy(HOST_portrait_data[HOST_portrait_next], custromportraitload_surf->o, 7168);
                        HOST_portrait_loaded[HOST_portrait_next] = 1;
                        tnpc3->port = HOST_portrait_next;
                        HOST_portrait_next++;
                    }
                    goto custromportraitload_found;
                } //txtsame(t3,t4)
                goto custromportraitload_loop;
            }
        } //playerlist[tpl]->GNPCflags[33]&1
    custromportraitload_found:
    custromportraitload_skip:


        goto save_restored;
    save_createcharacter: //new player
        txtset(tnpc->name, tname);
        tnpc->lev = 1;
        tnpc->exp = 370; //same as in original, incentive not to die!
        tnpc->hp = 30;
        tnpc->hp_max = 30;
        tnpc->s = 0;
        tnpc->d = 0;
        tnpc->i = 0;
        x = -1;
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_COMPASSION } //i3,d3
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_VALOR } //i3,s3
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_JUSTICE } //i3,X
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_SACRIFICE } //i3,X
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_HONOR } //i3,X
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_SPIRITUALITY } //i3,a1
        x++;
        if (t6->d[x] == 0) { U6O_HONESTY } else { U6O_HUMILITY } //i3,X
        x++;
        if (t6->d[x] == 0) { U6O_COMPASSION } else { U6O_VALOR } //d3,s3
        x++;
        if (t6->d[x] == 0) { U6O_COMPASSION } else { U6O_JUSTICE } //d3,X
        x++;
        if (t6->d[x] == 0) { U6O_COMPASSION } else { U6O_SACRIFICE } //d3,X
        x++;
        if (t6->d[x] == 0) { U6O_COMPASSION } else { U6O_HONOR } //d3,X
        x++;
        if (t6->d[x] == 0) { U6O_COMPASSION } else { U6O_SPIRITUALITY } //d3,a1
        x++;
        if (t6->d[x] == 0) { U6O_COMPASSION } else { U6O_HUMILITY } //d3,X
        x++;
        if (t6->d[x] == 0) { U6O_VALOR } else { U6O_JUSTICE } //s3,X
        x++;
        if (t6->d[x] == 0) { U6O_VALOR } else { U6O_SACRIFICE } //s3,X
        x++;
        if (t6->d[x] == 0) { U6O_VALOR } else { U6O_HONOR } //s3,X
        x++;
        if (t6->d[x] == 0) { U6O_VALOR } else { U6O_SPIRITUALITY } //s3,a1
        x++;
        if (t6->d[x] == 0) { U6O_VALOR } else { U6O_HUMILITY } //s3,X
        x++;
        if (t6->d[x] == 0) { U6O_JUSTICE } else { U6O_SACRIFICE } //i,d
        x++;
        if (t6->d[x] == 0) { U6O_JUSTICE } else { U6O_HONOR } //s,d
        x++;
        if (t6->d[x] == 0) { U6O_JUSTICE } else { U6O_SPIRITUALITY } //X,a1
        x++;
        if (t6->d[x] == 0) { U6O_JUSTICE } else { U6O_HUMILITY } //OK,X
        x++;
        if (t6->d[x] == 0) { U6O_SACRIFICE } else { U6O_HONOR } //s,i
        x++;
        if (t6->d[x] == 0) { U6O_SACRIFICE } else { U6O_SPIRITUALITY } //X,OK
        x++;
        if (t6->d[x] == 0) { U6O_SACRIFICE } else { U6O_HUMILITY } //a1,X
        x++;
        if (t6->d[x] == 0) { U6O_HONOR } else { U6O_SPIRITUALITY } //X,a1
        x++;
        if (t6->d[x] == 0) { U6O_HONOR } else { U6O_HUMILITY } //OK,X
        x++;
        if (t6->d[x] == 0) { U6O_SPIRITUALITY } else { U6O_HUMILITY } //a1,X
        x = tnpc->s - 8;
        if (x < 1) x = 1;
        tnpc->s = x;
        x = tnpc->d - 8;
        if (x < 1) x = 1;
        tnpc->d = x;
        x = tnpc->i - 8;
        if (x < 1) x = 1;
        tnpc->i = x;
        tnpc->mp = tnpc->i * 2;
        tnpc->mp_max = tnpc->i * 2;
        tnpc->wt = 0;
        tnpc->wt_max = tnpc->s * 4 * 100;
        tnpc->wt2 = 0;
        tnpc->wt2_max = tnpc->s * 50;

        //Debug new players get high stats, gold, exp, all spells, 
        if (U6O_DEBUG) {
            tnpc->lev = 6;
            //tnpc->lev=20;
            tnpc->exp = 410000;
            tnpc->s = tnpc->s + 3;
            tnpc->d = tnpc->d + 3;
            tnpc->i = tnpc->i + 18;
            //tnpc->s=888; //tnpc->d=888; tnpc->i=888;
            tnpc->wt = 0;
            tnpc->wt_max = tnpc->s * 4 * 100;
            tnpc->wt2 = 0;
            tnpc->wt2_max = tnpc->s * 50;
            //tnpc->mp_max=640; tnpc->mp=640;
            tnpc->hp_max = 180;
            tnpc->hp = 180;
            //tnpc->hp_max=8800; tnpc->hp=8800;
            for (x = 0; x <= 9; x++) {
                x2 = 88;
                x3 = 0;
                x4 = 0;
                if (x == 8) { x2 = 57; } //spell
                if (x <= 7) {
                    x2 = 58;
                    for (x4 = 0; x4 <= 8; x4++) {
                        x3 = (x << 4) + x4;
                        myobj3 = OBJnew();
                        myobj3->type = x2;
                        myobj3->more2 = x3;
                        myobj3->info = 0; //add item(s)
                        additem(tnpc, myobj3);
                    }
                    x3 = (x << 4) + 9;
                    x4 = 0;
                } //spell
                if (x2 == 88) x3 = 30000; //<-1000 gold
                myobj3 = OBJnew();
                myobj3->type = x2;
                myobj3->more2 = x3;
                myobj3->info = x4; //add item(s)
                additem(tnpc, myobj3);
            } //x

            myobj3 = OBJnew();
            myobj3->type = 65;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 66;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 67;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 68;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 69;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 70;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 71;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);
            myobj3 = OBJnew();
            myobj3->type = 72;
            myobj3->more2 = 1000; //<-reagents
            additem(tnpc, myobj3);


            myobj3 = OBJnew();
            myobj3->type = 87;
            myobj3->more2 = 0;
            myobj3->info = 0; //add item: orb of the moons
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 242;
            myobj3->more2 = 0;
            myobj3->info = 0; //rune of honesty
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 243;
            myobj3->more2 = 0;
            myobj3->info = 0; //rune of compassion
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 244;
            myobj3->more2 = 0;
            myobj3->info = 0; //rune of valor
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 81;
            myobj3->more2 = 0;
            myobj3->info = 0; //storm cloack
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 258;
            myobj3->more2 = 0;
            myobj3->info = 0; //invis ring
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 420;
            myobj3->more2 = 0;
            myobj3->info = 0; //balloon
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 212;
            myobj3->more2 = 0;
            myobj3->info = 0; //fan
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 50;
            myobj3->more2 = 15; //<-tcrossbow
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 23;
            myobj3->more2 = 255; //<-marmour
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 4;
            myobj3->more2 = 255; //<-spiked helm
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 35;
            myobj3->more2 = 255; //<-main gauche
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 35;
            myobj3->more2 = 255; //<-main gauche
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 13;
            myobj3->more2 = 255; //<-spiked shield
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 13;
            myobj3->more2 = 255; //<-spiked shield
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 24;
            myobj3->more2 = 255; //<-spiked collar
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 109;
            myobj3->more2 = 15; //<-rolling pin
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 109;
            myobj3->more2 = 15; //<-rolling pin
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 8;
            myobj3->more2 = 255; //<-magic helm
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 16;
            myobj3->more2 = 255; //<-magic shield
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 27;
            myobj3->more2 = 255; //<-leather boots
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 54;
            myobj3->more2 = 15; //<-magic bow
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 43;
            myobj3->more2 = 15; //<-sword
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 43;
            myobj3->more2 = 15; //<-sword
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 39;
            myobj3->more2 = 15; //<-mace
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 39;
            myobj3->more2 = 15; //<-mace
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 40;
            myobj3->more2 = 15; //<-morning star
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 40;
            myobj3->more2 = 15; //<-morning star
            additem(tnpc, myobj3);

            myobj3 = OBJnew();
            myobj3->type = 47;
            myobj3->more2 = 15; //<-halberd
            additem(tnpc, myobj3);
        } //U6O_DEBUG


        //add items
        myobj3 = OBJnew();
        myobj3->type = 90;
        myobj3->more2 = 3; //<-torch
        additem(tnpc, myobj3);

        myobj3 = OBJnew();
        myobj3->type = 128;
        myobj3->more2 = 2; //<-bread
        additem(tnpc, myobj3);

        myobj3 = OBJnew();
        myobj3->type = 129;
        myobj3->more2 = 1; //<-meat
        additem(tnpc, myobj3);

        myobj3 = OBJnew();
        myobj3->type = 210; //<-ribs/big meat
        additem(tnpc, myobj3);

        myobj3 = OBJnew();
        myobj3->type = 74;
        myobj3->more2 = 0; //<-ankh
        myobj3->info |= 256; //quest item
        additem(tnpc, myobj3);

        myobj3 = OBJnew();
        myobj3->type = 34;
        myobj3->more2 = 0; //<-club
        additem(tnpc, myobj3);

        if ((btime >= 16) || (btime <= 5)) {
            //only give players who enter at night
            myobj3 = OBJnew();
            myobj3->type = 90 + 1 * 1024;
            myobj3->more2 = 1; //<-torch in hand
            tnpc->wep_left = myobj3;
        }

        static SYSTEMTIME systime;
        GetLocalTime(&systime);
        x3 = (systime.wDay - 1) + (systime.wMonth - 1) * 32 + systime.wYear * (32 * 12);
        playerlist[tpl]->GNPCflags[249] = x3; //249 YMD of character creation

        txtset(t, "New_Player_Entered:");
        txtadd(t, tnpc->name);
        LOGadd(t);
    save_restored:

        playerlist[tpl]->port = ((npc *) playerlist[tpl]->party[0]->more)->port;
        playerlist[tpl]->npcname = txtnew();
        txtset(playerlist[tpl]->npcname, ((npc *) playerlist[tpl]->party[0]->more)->name);


        //if (tpl==0){
        //if (U6O_DEBUG){
        //tnpc->lev=8;
        //tnpc->s=64; tnpc->d=64; tnpc->i=64;
        //tnpc->mp_max=255; tnpc->mp=255;
        //tnpc->hp_max=255; tnpc->hp=255;
        //playerlist[tpl]->GNPCflags[6]=0;

        //playerlist[tpl]->wizardeyetimeleft=0.0f;
        //playerlist[tpl]->wizardeyex=0;
        //playerlist[tpl]->wizardeyey=0;

        //for (x=0;x<=2;x++){
        //x2=88; x3=0; x4=0;
        //if (x==0) {x2=58; x3=(6<<4)+9;}//spell
        //if (x2==88) x3=20000;//<-1000 gold
        //myobj3=OBJnew(); myobj3->type=x2; myobj3->more2=x3; myobj3->info=x4;//add item(s)
        //additem(tnpc,myobj3);
        //}//x

        //}//U6O_DEBUG
        //}//tpl==0


        playerlist[tpl]->net = tnet;
        playerlist[tpl]->id = U6ONEWID;
        U6ONEWID++;
        playerlist[tpl]->GNPCflags[242] = socketclient_ip[tnet]; //update IP

        //log IP
        txtset(t, "Player_IP:");
        txtnumint(t2, playerlist[tpl]->GNPCflags[242] & 255);
        txtadd(t, t2);
        txtadd(t, ".");
        txtnumint(t2, (playerlist[tpl]->GNPCflags[242] >> 8) & 255);
        txtadd(t, t2);
        txtadd(t, ".");
        txtnumint(t2, (playerlist[tpl]->GNPCflags[242] >> 16) & 255);
        txtadd(t, t2);
        txtadd(t, ".");
        txtnumint(t2, (playerlist[tpl]->GNPCflags[242] >> 24) & 255);
        txtadd(t, t2);
        LOGadd(t);

        tnpc = (npc *) playerlist[tpl]->party[0]->more;
        txtset(t, "?");
        t->d2[0] = 8;
        txtadd(t, "Entered Britannia: ");
        txtadd(t, tnpc->name);
        for (x = 0; x <= playerlist_last; x++) {
            if (playerlist[x]) {
                if (playerlist[x]->net != INVALID_NET) {
                    if (x != tpl) {
                        NET_send(NETplayer, playerlist[x]->net, t);
                    }
                }
            }
        }
        inbritanniacheck();
        txtset(t, "?");
        t->d2[0] = 8;
        txtadd(t, inbritannia);
        NET_send(NETplayer, playerlist[tpl]->net, t);


        txtset(t3, motd);
        txtset(t, "?");
        t->d2[0] = 2;
        txtset(t2, "??");
        t2->ds[0] = 1;
        txtadd(t, t2);
        t2->ds[0] = 3;
        txtadd(t, t2);
        t2->ds[0] = 2047;
        txtadd(t, t2);
        t2->ds[0] = 1023;
        txtadd(t, t2);
        t2->ds[0] = t3->l;
        txtadd(t, t2);
        t2->ds[0] = 0;
        txtadd(t, t2); //unused
        t2->ds[0] = 187;
        txtadd(t, t2); //more=port
        txtset(t2, "????");
        t2->df[0] = 2;
        txtadd(t, t2); //wait
        txtadd(t, t3);
        NET_send(NETplayer, playerlist[tpl]->net, t);

        txtset(t3, "??");
        t3->d2[0] = 14;
        t3->d2[1] = (windx + 1) + (windy + 1) * 4;
        NET_send(NETplayer, playerlist[tpl]->net, t3);


        //Compensation
        //***KALDOSH BIT0 USED***
        txtset(t, "Kaldosh");
        if (txtsame(t, tnpc->name)) {
            if ((playerlist[tpl]->GNPCflags[246] & 1) == 0) {
                playerlist[tpl]->GNPCflags[246] |= 1;
                myobj3 = OBJnew();
                myobj3->type = 87;
                myobj3->more2 = 0;
                myobj3->info = 0; //add item: orb of the moons
                additemroot(tnpc, myobj3);
            }
        }

        if (tnamecolour) {
            playerlist[tpl]->GNPCflags[245] = tnamecolour;
        } else {
            if (playerlist[tpl]->GNPCflags[245] == 0) {
            setnamecolour_getrandom:
                x = rnd * 256;
                x2 = rnd * 256;
                x3 = rnd * 256;
                if ((x + x2 + x3) < 256) goto setnamecolour_getrandom;
                playerlist[tpl]->GNPCflags[245] = x + (x2 << 8) + (x3 << 16);
            }
        }
        //send namecolour to client
        /*
    txtsetchar(t,246);
    txtaddchar(t,playerlist[tpl]->GNPCflags[245]&255);
    txtaddchar(t,(playerlist[tpl]->GNPCflags[245]>>8)&255);
    txtaddchar(t,(playerlist[tpl]->GNPCflags[245]>>16)&255);
    NET_send(NETplayer,playerlist[tpl]->net,t);
    */

        playerlist[tpl]->GNPCflags[36] = 0; //cat abuse flag reset!

        //auto-sysban
        txtset(t2, tnpc->name);
        txtucase(t2);
