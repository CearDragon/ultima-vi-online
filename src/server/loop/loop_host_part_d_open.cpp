// LHS-P6.2: part_d_open -- MEGA D start (per-player INPUT processor). WIRE.
// LHS brace-seam: CONTINUES the part_00 fragment; OPENS the for(tpl..){
// if(playerlist..){ blocks CLOSED in loop_host_part_d_ship.cpp. Contents: input
// loop open, resurrect-followers (resurrectfollowers_failed), switch-party
// (switchpartymember). Move-only: no input/format edits, no U6O_VERSION bump.

    u6opi = 5;
    for (tpl = 0; tpl <= playerlist_last; tpl++) {
        //process INPUT
        if (playerlist[tpl]) {
            //current player

            tplayer = playerlist[tpl];

            if (tplayer->mobj) {
                tplayer->mobj->info |= 112; //set mobj flags as temp OBJ

                if ((tplayer->mobj->type & 1023) == 223) {
                    //powder keg
                    tplayer->mobj->more2 = tplayer->id; //id
                    if (tplayer->mobj->type != 223) {
                        tplayer->mobj->type = 223;
                        tplayer->mobj_last = NULL; //update message: mobj has changed
                    }
                } //223

                if ((tplayer->mobj->type & 1023) == 173) {
                    //trap
                    tplayer->mobj->more2 = tplayer->id; //id
                } //173

                //set "new" horse owner in respawn_y
                if (tplayer->mobj->type == 448) {
                    if (myobj = (object *) tplayer->mobj->more) {
                        //GPF latest
                        if (myobj->info & 4) {
                            //crt
                            crt = (creature *) myobj->more;
                            crt->respawn_y = tpl;
                        } //crt
                    } //->more
                } //448
            } //mobj


            //followers resurrection
            if (tplayer->respawn == 254) {
                tplayer->respawn = 0;
                if (tplayer->party[0] == NULL) {
                    txtsetchar(t, 8);
                    txtadd(t, "You must first resurrect yourself!");
                    NET_send(NETplayer, tplayer->net, t);
                    goto resurrectfollowers_failed;
                }
                if (tplayer->craft) {
                    txtsetchar(t, 8);
                    txtadd(t, "Not now!");
                    NET_send(NETplayer, tplayer->net, t);
                    goto resurrectfollowers_failed;
                }
                i2 = 0;
                for (i = 0; i <= nresu; i++) {
                    if (resu[i]) {
                        if (resu_player[i] == tplayer) {
                            if (resu_partymember[i] >= 1) {
                                i2 = 1;
                                tplayer->party[resu_partymember[i]] = resu[i];
                                tnpc = (npc *) resu[i]->more;
                                //tnpc->exp-=(tnpc->exp/8);
                                if (easymodehostn1) {
                                    tnpc->exp -= new1_getexpdeduction(tnpc, 2); // c111
                                } else {
                                    // original
                                    tnpc->exp -= (tnpc->exp / 8);
                                }
                                resu[i] = NULL;
                                OBJmove_allow = TRUE;
                                OBJmove2(tplayer->party[resu_partymember[i]], tplayer->party[0]->x,
                                         tplayer->party[0]->y);
                                OBJmove_allow = FALSE;
                                txtset(t5, "?");
                                t5->d2[0] = 8;
                                txtadd(t5, tnpc->name);
                                txtadd(t5, " resurrected. (F1)");
                                for (x2 = 0; x2 <= playerlist_last; x2++) {
                                    //create and send UPDATE message
                                    if (playerlist[x2]) {
                                        //current player
                                        if (playerlist[x2]->net != INVALID_NET) {
                                            //net connection available
                                            if (playerlist[x2]->party[0]) {
                                                NET_send(NETplayer, playerlist[x2]->net, t5);
                                            }
                                        }
                                    }
                                }
                            } //0
                        } //player
                    } //resu
                } //i
                if (i2) {
                    inbritanniacheck();
                    goto resurrectfollowers_failed;
                }
                txtsetchar(t, 8);
                txtadd(t, "None of your followers are dead!");
                NET_send(NETplayer, tplayer->net, t);
            } //respawn
        resurrectfollowers_failed:

            //self-resurrection?
            if (tplayer->respawn == 255) {
                tplayer->respawn = 0;
                //check ressurection tables for player's pointer
                //return player to party[0]
                for (i = 0; i <= nresu; i++) {
                    if (resu[i]) {
                        if (resu_player[i] == tplayer) {
                            if (resu_partymember[i] == 0) {
                                /*
                object *resu[65536]; //object
                object *resu_body[65536]; //dead body object
                float resu_wait[65536]; //time until object automatically resurrected
                player *resu_player[65536]; //player object belongs to
                unsigned char resu_partymember[65536]; //party member index
                long nresu=-1;
                */
                                tplayer->party[0] = resu[i];
                                if (tplayer->GNPCflags[25] == 0) {
                                    tnpc = (npc *) resu[i]->more;
                                    //tnpc->exp-=(tnpc->exp/8);//res by f1
                                    if (easymodehostn1) {
                                        tnpc->exp -= new1_getexpdeduction(tnpc, 2); // c111
                                    } else {
                                        // original
                                        tnpc->exp -= (tnpc->exp / 8); //res by f1
                                    }
                                    x2 = 256 + 128 - 16;
                                    y2 = 256 + 128 + 2;
                                } else {
                                    //resurrect at brit healer's bed and remove insurance
                                    tplayer->GNPCflags[25] = 0;
                                    x2 = 362;
                                    y2 = 362;
                                }

                                resu[i] = NULL;

                                for (x4 = 1; x4 <= 7; x4++) {
                                    if (tplayer->party[x4]) {
                                        tnpc2 = (npc *) tplayer->party[x4]->more;
                                        tnpc2->upflags = 1;
                                        OBJmove_allow = TRUE;
                                        OBJmove2(tplayer->party[x4], x2, y2); //place party on the map
                                        OBJmove_allow = FALSE;
                                    }
                                } //x4

                                OBJmove_allow = TRUE;
                                OBJmove2(tplayer->party[0], x2, y2); //place avatar on the map
                                OBJmove_allow = FALSE;

                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, "A voice in the darkness intones, \x022KAL LOR!\x022");
                                NET_send(NETplayer, tplayer->net, t);

                                txtset(t5, "?");
                                t5->d2[0] = 8;
                                txtadd(t5, tnpc->name);
                                txtadd(t5, " resurrected. (F1)");
                                for (x2 = 0; x2 <= playerlist_last; x2++) {
                                    //create and send UPDATE message
                                    if (playerlist[x2]) {
                                        //current player
                                        if (playerlist[x2]->net != INVALID_NET) {
                                            //net connection available
                                            if (playerlist[x2]->party[0]) {
                                                if (playerlist[x2] != tplayer) {
                                                    NET_send(NETplayer, playerlist[x2]->net, t5);
                                                }
                                            }
                                        }
                                    }
                                }
                                inbritanniacheck();
                                goto resurrect_failed;
                            } //0
                        } //player
                    } //resu
                } //i
                txtsetchar(t, 8);
                txtadd(t, "You are not dead!");
                NET_send(NETplayer, tplayer->net, t);
            } //respawn
        resurrect_failed:


            if (playerlist[tpl]->party[0]) {
                //move selected party member


                //tplayer=playerlist[tpl];

                myobj = tplayer->party[tplayer->selected_partymember];
                tnpc = (npc *) myobj->more;

                //tpx=myobj->x-15; tpy=myobj->y-11;
                //if (tpx<0) tpx=0;
                //if (tpy<0) tpy=0;
                //if (tpx>2016) tpx=2016;
                //if (tpy>1000) tpy=1000;
                getscreenoffset(myobj->x, myobj->y, &tpx, &tpy);


                //cast spell
                if (tplayer->key & KEYmbclick) {
                    i = tplayer->mf;

                    //change party member?
                    if ((i >= 16) && (i <= 23)) {
                        if (tplayer->craft == NULL) {
                            i -= 16;
                            x = 0;
                            for (i2 = 0; i2 <= 7; i2++) {
                                if (tplayer->party[i2]) {
                                    if (x == i) {
                                        if (tplayer->party[i2]->x) {
                                            //not in the void!
                                            tplayer->selected_partymember = i2;
                                            tplayer->pathfind = 0;
                                            goto switchpartymember;
                                        }
                                    }
                                    x++;
                                } //->party[i2]
                            } //i2
                        } //craft
                        goto skip_pickup;
                    switchpartymember:
                        for (x2 = 0; x2 <= 7; x2++) {
                            if (tplayer->party[x2]) {
                                myobj2 = tplayer->party[x2];
                                tnpc = (npc *) myobj2->more;
                                tnpc->follow = tplayer->party[tplayer->selected_partymember];
                                if (x2 == tplayer->selected_partymember) {
                                    if (tnpc->order != 3) tnpc->order = 0;
                                } else {
                                    if (tnpc->order != 3) tnpc->order = 1;
                                }
                            }
                        }
                        goto skip_pickup;
                    }

                    if (i == 24) {
                        //staff cast
                        myobj = tplayer->staffcast_npc;
                        tnpc = (npc *) myobj->more;
                        //x2=tplayer->staffcast_staff->more2&65535;
                        staff_cast_spell = 1;
                        goto staffcast0;
                    }

                    if ((i >= 8) && (i <= 15)) {
                        static unsigned char iparty;
                        iparty = i - 8;
                        myobj = tplayer->party[i - 8];
                        if (!myobj) {
                            goto castfailed;
                        }
                        tnpc = (npc *) myobj->more;
