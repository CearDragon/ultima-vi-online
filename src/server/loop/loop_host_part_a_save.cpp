// LHS-P3.2: part_a_save -- MEGA A start (per-player UPDATE-message builder).
// LHS brace-seam: CONTINUES the if(NEThost){ block; OPENS the for(tpl..){ and
// if(playerlist[tpl]){ blocks that are CLOSED in loop_host_part_a_display_sfx.cpp.
// Contents: save/login/password (save_update1, gotprevsave, save_empty).


    for (tpl = 0; tpl <= playerlist_last; tpl++) {
        //create and send UPDATE message


        if (playerlist[tpl]) {
            //current player
            if (playerlist[tpl]->net != NULL) {
                //net connection available
                tplayer = playerlist[tpl];

                tplayer->online += et;

                /*
        // luteijn: keep client in check, but this is too often?
	// luteijn: it is, so now only sending it when saving.
        //send time
        txtset(t2,"?"); t2->d2[0]=32;
        txtset(t3,"12345678"); t3->dd[0]=btime2;
        txtadd(t2,t3);
        NET_send(NETplayer,tplayer->net,t2);
        */

                //doorclose: set/reset wait timer for door if player is near
                if (myobj = tplayer->party[0]) {
                    for (i = 0; i <= doorclose_last; i++) {
                        x = abs(doorclose_obj[i]->x - myobj->x);
                        y = abs(doorclose_obj[i]->y - myobj->y);
                        if ((x <= 16) && (y <= 12)) {
                            doorclose_wait[i] = 1.0f;
                        } //<=8
                    } //i
                } //->party[0]

                //leverchange: set/reset wait timer if player is near lever OR portcullis
                if (myobj = tplayer->party[0]) {
                    for (i = 0; i <= leverchange_last; i++) {
                        x = abs(leverchange_obj[i]->x - myobj->x);
                        y = abs(leverchange_obj[i]->y - myobj->y);
                        if ((x <= 16) && (y <= 12)) {
                            leverchange_wait[i] = 1.0f;
                        } //<=8
                        myobj2 = (object *) leverchange_obj[i]->more;
                    leverchange0:
                        if (myobj2) {
                            x = abs(myobj2->x - myobj->x);
                            y = abs(myobj2->y - myobj->y);
                            if ((x <= 16) && (y <= 12)) {
                                leverchange_wait[i] = 1.0f;
                            } //<=8
                            myobj2 = (object *) myobj2->next;
                            goto leverchange0;
                        }
                    } //i
                } //->party[0]


                //orb
                // luteijn: this updates the orb destinations a player has visited.
                // visited means he was 8 or less squares from the spot.
                //GNPCflags[22]: bitvector for available orb destinations
                if (myobj = tplayer->party[0]) {
                    x2 = 1;
                    for (y = 0; y <= 4; y++) {
                        for (x = 0; x <= 4; x++) {
                            x3 = abs(myobj->x - orbx[x][y]);
                            y3 = abs(myobj->y - orby[x][y]);
                            if ((x3 <= 8) && (y3 <= 8)) {
                                tplayer->GNPCflags[22] |= x2;
                            }
                            x2 = x2 << 1;
                        }
                    } //x,y
                } //party[0]


                if (tplayer->guardianstatuewait) {
                    f = 1.0f;
                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                        tplayer->guardianstatuewait--;
                    }
                }

                tplayer->wizardeyetimeleft -= et;
                if (tplayer->wizardeyetimeleft < 0.0f) tplayer->wizardeyetimeleft = 0.0f;
                tplayer->wizardeyemovewait -= et;
                if (tplayer->wizardeyemovewait < 0.0f) tplayer->wizardeyemovewait = 0.0f;


                tplayer->speaking -= et;
                if (tplayer->speaking < 0.0f) tplayer->speaking = 0.0f;

                if (tplayer->rest) {
                    tplayer->rest--;
                    if (tplayer->rest == 16) {
                        txtset(t, "??");
                        t->d2[0] = 250;
                        t->d2[1] = 0;
                        NET_send(NETplayer, tplayer->net, t);
                    } //rest==16
                    if (tplayer->rest == 0) {
                        tplayer->GNPCflags[253] |= (1 + 2);
                    } //rest==0
                } //rest

                //check trap indexes
                for (i = 0; i <= 7; i++) {
                    if (myobj = tplayer->party[i]) {
                        if ((myobj->x != tplayer->trapx[i]) || (myobj->y != tplayer->trapy[i])) {
                            tplayer->trapx[i] = 0;
                            tplayer->trapy[i] = 0;
                        }
                    }
                }

                //add storm cloak fields
                for (i = 0; i <= 7; i++) {
                    if (myobj = tplayer->party[i]) {
                        if (myobj->x) {
                            //not in void
                            tnpc = (npc *) myobj->more;
                            if (myobj2 = tnpc->armour) {
                                if (myobj2->type == 81) {
                                    stormcloak_last++;
                                    stormcloak_x[stormcloak_last] = myobj->x;
                                    stormcloak_y[stormcloak_last] = myobj->y;
                                    stormcloak_player[stormcloak_last] = tplayer;

                                    if ((unsigned long)(ett) != (unsigned long)(ett - et)) {
                                        myobj2->more2++;
                                        if (myobj2->more2 >= 1024) {
                                            //~16-32mins
                                            i2 = rnd * 1024;
                                            if (!i2) {
                                                tnpc->armour = NULL;
                                                OBJrelease(myobj2);
                                                tnpc->upflags |= 64;
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, tnpc->name);
                                                txtadd(t, "'s storm cloak vanished!");
                                                NET_send(NETplayer, tplayer->net, t);
                                            } //!i2
                                        } //16-32
                                    } //et
                                } //81
                                else if (tnpc->negatemagic != NULL) { goto negatemagic_field; }
                            } //armour
                            else if (tnpc->negatemagic != NULL) {
                            negatemagic_field:
                                stormcloak_last++;
                                stormcloak_x[stormcloak_last] = myobj->x;
                                stormcloak_y[stormcloak_last] = myobj->y;
                                stormcloak_player[stormcloak_last] = tplayer;
                            }
                        } //->x
                    } //party
                } //i


                if (tplayer->party[0]) {
                    if ((tplayer->party[0]->x == 368) && (tplayer->party[0]->y == 386)) tplayer->kallor = 0;
                    if (tplayer->kallor) {
                        f = 1.0f;
                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                            tplayer->kallor--;
                            if (tplayer->kallor == 0) {
                                static unsigned char tplayer_craft;
                                tplayer_craft = FALSE;
                                if (tplayer->craft) {
                                    tplayer_craft = TRUE;
                                    if (((tplayer->party[0]->type & 1023) == 412) || (
                                            (tplayer->party[0]->type & 1023) == 414) || (
                                            (tplayer->party[0]->type & 1023) == 415)) {
                                        //exit raft/skiff/boat
                                        myobj2 = tplayer->party[0]; //boat (temp, used for positioning)
                                        VLNKremove(tplayer->party[0]);
                                        VLNKsremove(tplayer->party[0]); //remove vlnk(s)
                                        if ((tplayer->party[0]->type & 1023) == 412)
                                            tplayer->party[0]->more = tplayer->craft_con;
                                        else tplayer->party[0]->more = NULL;
                                        tplayer->party[0]->info = tplayer->party[0]->info & (1 << 9);
                                        tplayer->party[0] = tplayer->craft;
                                        tplayer->craft = NULL;
                                        for (i4 = 7; i4 >= 0; i4--) {
                                            if (tplayer->party[i4] != NULL) {
                                                OBJadd(myobj2->x, myobj2->y, tplayer->party[i4]);
                                            }
                                        }
                                    } //412/414/415
                                    //balloon
                                    if ((tplayer->party[0]->type & 1023) == 423) {
                                        myobj2 = tplayer->party[0]; //boat (temp, used for positioning)
                                        use_craft(tplayer, myobj2);
                                    } //423
                                } //craft

                                for (x4 = 0; x4 <= 7; x4++) {
                                    if (tplayer->party[x4]) {
                                        //player is alive (or they would not be listed)
                                        tnpc = (npc *) tplayer->party[x4]->more;

                                        txtset(t, "??");
                                        t->d2[0] = 252;
                                        t->d2[1] = x4;
                                        NET_send(NETplayer, tplayer->net, t);
                                        VLNKremove(tplayer->party[x4]);
                                        VLNKsremove(tplayer->party[x4]); //remove VLNK(s)

                                        //only take xp if player is standing on a non-void-type square and not 0,0
                                        if (tplayer->party[x4]->x && tplayer->party[x4]->y) {
                                            //not at 0,0
                                            if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x] & 1023) != 0) {
                                                if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x] & 1023) != 255) {
                                                    if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x] & 1023) !=
                                                        254) {
                                                        if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x] & 1023) !=
                                                            253) {
                                                            if ((bt[tplayer->party[x4]->y][tplayer->party[x4]->x] &
                                                                 1023) != 252) {
                                                                //tnpc->exp-=(tnpc->exp/16);//kal lor
                                                                if (easymodehostn1) {
                                                                    tnpc->exp -= new1_getexpdeduction(tnpc, 1); // c111
                                                                } else {
                                                                    // original
                                                                    tnpc->exp -= (tnpc->exp / 16); //kal lor
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                        if (tnpc->horse) {
                                            horsedismount(tplayer->party[x4]);
                                            if (tplayer_craft) {
                                                if (HORSEDISMOUNT_HORSEOBJECT) {
                                                    //send horse back to Britain's stables free of charge
                                                    OBJmove_allow = TRUE;
                                                    OBJmove2(HORSEDISMOUNT_HORSEOBJECT, 280, 441);
                                                    OBJmove2(HORSEDISMOUNT_HORSEOBJECT, 279, 441);
                                                    OBJmove_allow = FALSE;
                                                }
                                            }
                                        }

                                        OBJmove_allow = TRUE;
                                        OBJmove2(tplayer->party[x4], 256 + 128 - 16, 256 + 128 + 2);
                                        //place avatar on the map
                                        OBJmove_allow = FALSE;


                                        if (tnpc->hp < 30) tnpc->hp = 30;
                                        tnpc->upflags |= 1; //update all
                                        tnpc->flags &= (65535 - 1); //remove poisoned flag
                                        if (x4 == 0) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "\x022KAL LOR!\x022");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                    }
                                }
                            } //tplayer->kallor==0
                        } //f
                    } //tplayer->kallor
                } else {
                    //tplayer->party[0]
                    tplayer->kallor = 0;
                }


                tplayer->ktar_display -= et;
                if (tplayer->ktar_display < 0.0f) tplayer->ktar_display = 0;
                for (i = 0; i <= 9; i++) {
                    if (tplayer->ktar_wait[i]) {
                        f = 1.0f;
                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                            tplayer->ktar_wait[i]--;
                        }
                        //distance check
                        if (myobj = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]) {
                            if (myobj2 = (object *) tplayer->ktar[i]->more) {
                                x = abs(myobj->x - myobj2->x);
                                y = abs(myobj->y - myobj2->y);
                                if ((x > 16) || (y > 12)) tplayer->ktar_wait[i] = 0;
                            } else {
                                tplayer->ktar_wait[i] = 0;
                            }
                        } //->party
                    } //wait
                } //i


                //windnew update
                if (windnew) {
                    tplayer->windx = 0;
                    tplayer->windy = 0;
                    txtset(t, "??");
                    t->d2[0] = 14;
                    t->d2[1] = (windx + 1) + (windy + 1) * 4;
                    NET_send(NETplayer, tplayer->net, t);
                }

                if (tplayer->xray) {
                    f = 1.0f;
                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                        tplayer->xray--;
                    }
                }

                if (tplayer->peer) {
                    //peer time reduce
                    f = 1.0f;
                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                        tplayer->peer--;
                    }
                }

                //update midi
                if (myobj = tplayer->party[0]) {
                    x = myobj->x;
                    y = myobj->y;
                    tnpc = (npc *) myobj->more;
                    // random 50% chance to get theme or forest
                    i = rnd * 2;
                    if (i == 1) {
                        i = 5; //overland: forest midi
                    } else {
                        i = 11; //U6 theme
                    }

                    if (x >= 1024) i = 3; //underground: dungeon midi
                    if ((x >= 1024) && (y >= 256) && (x < 1280) && (y < 512)) i = 6; //gargoyle lands: xylophone midi
                    if ((x >= 1280) && (y >= 256) && (x < 1351) && (y < 319)) i = 1;
                    //castle floor II: britan theme midi
                    if ((x >= 243) && (y >= 307) && (x < 406) && (y < 454)) i = 1; //britan: britan theme midi
                    if ((x >= 560) && (y >= 60) && (x < 648) && (y < 120)) i = 10; //minoc: stones midi
                    if ((x >= 1024) && (y >= 616) && (x < 1072) && (y < 656)) i = 11;
                    //island floor II: forest theme midi
                    if ((x >= 1320) && (y >= 319) && (x <= 1326) && (y <= 324)) i = 1; //tower II

                    if ((x >= 1280) && (y >= 344) && (x <= 1336) && (y <= 394)) i = 11; //top floor of keep
                    if ((x >= 1242) && (y >= 512) && (x <= 1265) && (y <= 538)) i = 11; //underground of keep
                    if ((x >= 220) && (y >= 663) && (x <= 243) && (y <= 688)) i = 11; //ground floor of keep


                    //txtset(t,"??"); t->d2[0]=11; t->d2[1]=7; NET_send(NETplayer,tplayer->net,t);
                    i2 = bt[y][x] & 255;
                    if ((i2 >= 8) && (i2 <= 15)) i = 7;

                    /*
          if (tnpc->target!=NULL){ //*FIX THIS
          i=9;
          }
          */
                    if ((i2 >= 16) && (i2 <= 47)) goto nomidichange; //shore

                    if (i != tplayer->midi) {
                        tplayer->midi = i;
                        txtset(t, "??");
                        t->d2[0] = 11;
                        t->d2[1] = i;
                        NET_send(NETplayer, tplayer->net, t);
                    }
                }
            nomidichange:

                /*
        if (t->d2[0]==11){ //type 11: set next midi
        txtright(t,t->l-1);
        nmidi=t->d2[0];
        goto CLIENT_donemess;
        }//11
        */


                for (x = 0; x <= 7; x++) {
                    if (tplayer->party[x]) {
                        myobj2 = tplayer->party[x];
                        tnpc = (npc *) myobj2->more;
                        if (tnpc->hp <= 0) {
                            //if (tplayer->craft) {tnpc->hp=0; goto exitloop1;} //*2004 TEMP
                            //death at sea!
                            if (tplayer->craft) {
                                if (((tplayer->party[0]->type & 1023) == 412) || (
                                        (tplayer->party[0]->type & 1023) == 414) || (
                                        (tplayer->party[0]->type & 1023) == 415)) {
                                    //exit raft/skiff/boat
                                    VLNKremove(tplayer->party[0]);
                                    VLNKsremove(tplayer->party[0]); //remove vlnk(s)
                                    if ((tplayer->party[0]->type & 1023) == 412)
                                        tplayer->party[0]->more = tplayer->craft_con;
                                    else tplayer->party[0]->more = NULL;
                                    tplayer->party[0]->info = tplayer->party[0]->info & (1 << 9);
                                    tplayer->party[0] = tplayer->craft;
                                    tplayer->craft = NULL;
                                    for (i4 = 7; i4 >= 0; i4--) {
                                        if (tplayer->party[i4] != NULL) {
                                            OBJadd(myobj2->x, myobj2->y, tplayer->party[i4]);

                                            //horse fix for death at sea
                                            horsedismount(tplayer->party[i4]);
                                            if (HORSEDISMOUNT_HORSEOBJECT) {
                                                //send horse back to Britain's stables free of charge
                                                OBJmove_allow = TRUE;
                                                OBJmove2(HORSEDISMOUNT_HORSEOBJECT, 280, 441);
                                                OBJmove2(HORSEDISMOUNT_HORSEOBJECT, 279, 441);
                                                OBJmove_allow = FALSE;
                                            }
                                        }
                                    }
                                    myobj2 = tplayer->party[0];
                                } //412/414/415


                                //balloon
                                if ((tplayer->party[0]->type & 1023) == 423) {
                                    use_craft(tplayer, myobj2);
                                    /*VLNKremove(tplayer->party[0]); VLNKsremove(tplayer->party[0]); //remove vlnk(s)
                  tplayer->party[0]->more=tplayer->craft_con;
                  tplayer->party[0]->info=tplayer->party[0]->info&(1<<9);
                  tplayer->party[0]=tplayer->craft;
                  tplayer->craft=NULL;
                  for (i4=7;i4>=0;i4--){
                  if (tplayer->party[i4]!=NULL){
                  OBJadd(myobj2->x,myobj2->y,tplayer->party[i4]);
                  }
                  }
                  //remove balloon from map + return item to inventory
                  static mlobj *mmyobj; //array size varies
                  mmyobj=(mlobj*)myobj2->more;
                  OBJremove(mmyobj->obj[0]); OBJrelease(mmyobj->obj[0]);
                  OBJremove(mmyobj->obj[1]); OBJrelease(mmyobj->obj[1]);
                  OBJremove(mmyobj->obj[2]); OBJrelease(mmyobj->obj[2]);
                  myobj2->type=420; myobj2->info|=256;
                  OBJremove(myobj2);
                  //tnpc=(npc*)tplayer->party[0]->more; //already set!
                  additemroot(tnpc,myobj2);
                  tnpc->upflags|=32;*/
                                    myobj2 = tplayer->party[0];
                                } //423
                            } //death at sea

                            horsedismount(myobj2); //this won't work at sea (but this is not currently implemented)

                        login_dead:


                            VLNKremove(myobj2);
                            VLNKsremove(myobj2); //remove VLNK(s)

                            x2 = myobj2->type;
                            x3 = myobj2->x;
                            y3 = myobj2->y;

                            OBJmove_allow = TRUE;
                            OBJmove2(myobj2, DEATHPOSX, DEATHPOSY);
                            OBJmove_allow = FALSE;

                            x4 = x2;
                            if ((x2 & 1023) == 376) x2 = 339 + 6 * 1024;
                            if ((x2 & 1023) == 377) x2 = 339 + 5 * 1024;
                            if ((x2 & 1023) == 378) x2 = 339 + 3 * 1024;
                            if ((x2 & 1023) == 379) x2 = 339 + 2 * 1024;
                            if ((x2 & 1023) == 380) x2 = 339 + 2 * 1024;
                            if ((x2 & 1023) == 382) x2 = 339 + 4 * 1024;
                            if ((x2 & 1023) == 381) x2 = 339 + 8 * 1024; //2004
                            if ((x2 & 1023) == 383) x2 = 339 + 8 * 1024;
                            if ((x2 & 1023) == 384) x2 = 339 + 5 * 1024;
                            if ((x2 & 1023) == 385) x2 = 339 + 8 * 1024;
                            if ((x2 & 1023) == 386) x2 = 339 + 7 * 1024;
                            if ((x2 & 1023) == 387) x2 = 339 + 3 * 1024;
                            if ((x2 & 1023) == 410) x2 = 339 + 7 * 1024;
                            if ((x2 & 1023) == 363) x2 = 339 + 0 * 1024; //gargoyle! Bah Lem
                            if ((x2 & 1023) == 354) x2 = 339 + 10 * 1024; //Sherry!

                            //if (x2!=x4){
                            if (OBJfindlast(x3, y3) == NULL) {
                                //blood
                                myobj3 = OBJnew();
                                myobj3->type = 338 + (int(rnd * 3) * 1024);
                                myobj3->info |= 112;
                                OBJadd(x3, y3, myobj3);
                            }
                            myobj3 = OBJnew();
                            myobj3->type = x2;
                            myobj3->info |= 112;
                            OBJadd(x3, y3, myobj3);

                            tnpc->baseitem = tnpc->items;
                            tnpc->baseitem_offset = 0;

                            myobj4 = myobj3;

                            if (x != 0) {
                                //drop inventory items if NOT primary party member

                                if (tplayer->selected_partymember == x) {
                                    tplayer->pathfind = 0;
                                    tnpc->order = 1;
                                    tplayer->selected_partymember = 0;
                                    if (tplayer->party[0]) {
                                        tnpc3 = (npc *) tplayer->party[0]->more;
                                        tnpc3->order = 0;
                                    }
                                }


                                if (tnpc->converse != 201) {
                                    //only if not a permanent party member
                                leaveitems2:
                                    if (myobj3 = (object *) tnpc->items->more) {
                                        OBJremove(myobj3);
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                        goto leaveitems2;
                                    }
                                    if (myobj3 = tnpc->helm) {
                                        tnpc->helm = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->neck) {
                                        tnpc->neck = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->wep_left) {
                                        if (myobj3->type == (90 + 1024)) myobj3->type = 90;
                                        tnpc->wep_left = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->wep_right) {
                                        if (myobj3->type == (90 + 1024)) myobj3->type = 90;
                                        tnpc->wep_right = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->armour) {
                                        tnpc->armour = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->ring_left) {
                                        tnpc->ring_left = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->ring_right) {
                                        tnpc->ring_right = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    if (myobj3 = tnpc->boots) {
                                        tnpc->boots = NULL;
                                        OBJadd(myobj4->x, myobj4->y, myobj3);
                                    }
                                    tnpc->baseitem = tnpc->items;
                                    tnpc->baseitem_offset = 0;
                                }

                                tnpc->hp = 30;
                                tnpc->upflags |= 1; //update all upon respawn
                                tnpc->flags &= (65535 - 1); //remove poisoned flag

                                //add NPC to resurrect list
                                for (x4 = 0; x4 <= nresu; x4++) {
                                    if (resu[x4] == NULL) goto newresurrect2;
                                } //x4
                                nresu++;
                                x4 = nresu;
                            newresurrect2:
                                resu[x4] = tplayer->party[x];
                                resu_body[x4] = myobj4;
                                resu_body_type[x4] = myobj4->type;
                                resu_wait[x4] = 16; //should be 1 hour (but what is host closes during this hour?)
                                resu_player[x4] = tplayer;
                                resu_partymember[x4] = x;
                                resu_x[x4] = myobj4->x;
                                resu_y[x4] = myobj4->y;
                            } //x!=0


                            //remove NPC

                            if (x == 0) {
                                tplayer->pathfind = 0;
                                tplayer->kallor = 0;
                                tplayer->px = x3;
                                tplayer->py = y3;
                                tplayer->selected_partymember = 0;
                                for (x4 = 1; x4 <= 7; x4++) {
                                    if (tplayer->party[x4]) {
                                        horsedismount(tplayer->party[x4]);
                                        //this won't work at sea (but this is not currently implemented)
                                        txtset(t, "??");
                                        t->d2[0] = 252;
                                        t->d2[1] = x4;
                                        NET_send(NETplayer, tplayer->net, t);
                                        VLNKremove(tplayer->party[x4]);
                                        VLNKsremove(tplayer->party[x4]); //remove VLNK(s)
                                        OBJmove_allow = TRUE;
                                        OBJmove2(tplayer->party[x4], DEATHPOSX, DEATHPOSY);
                                        OBJmove_allow = FALSE;
                                        //tplayer->party[x4]=NULL; ***other party members are not really gone!***
                                    }
                                } //x4

                                //send online message
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, tnpc->name);
                                txtadd(t, " killed!");
                                for (x4 = 0; x4 <= playerlist_last; x4++) {
                                    if (playerlist[x4]) {
                                        //current player
                                        if (playerlist[x4]->net != NULL) {
                                            //net connection available
                                            if (playerlist[x4]->party[0]) {
                                                if (playerlist[x4] != tplayer) {
                                                    NET_send(NETplayer, playerlist[x4]->net, t);
                                                }
                                            }
                                        }
                                    }
                                }

                                if (tplayer->karma <= 0) {
                                    //karma<=0
                                    OBJmove_allow = TRUE;
                                    OBJmove2(tplayer->party[0], DEATHPOSX, DEATHPOSY);
                                    OBJmove_allow = FALSE;
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, "An unending darkness engulfs thee...");
                                    NET_send(NETplayer, tplayer->net, t);
                                    //tplayer->idle_connect=64;
                                    /* if player owns a house, then free the house, but don't make a house chest so that its possible to recover the house items. 
                  and making the house chest would be a problem, because its made when logging in*/
                                    if (houseowner(tplayer, tplayer->GNPCflags[28])) {
                                        housesav[tplayer->GNPCflags[28]].gold = 0;
                                        housesav[tplayer->GNPCflags[28]].flags--; //remove house occupied flag
                                        housesav_update();
                                        tplayer->GNPCflags[28] = 0;
                                    }
                                } else {
                                    //karma>0

                                    //add player to resurrect list
                                    for (x4 = 0; x4 <= nresu; x4++) {
                                        if (resu[x4] == NULL) goto newresurrect;
                                    } //x4
                                    nresu++;
                                    x4 = nresu;
                                newresurrect:
                                    resu[x4] = tplayer->party[0];
                                    resu_body[x4] = myobj3;
                                    resu_body_type[x4] = myobj3->type;
                                    resu_wait[x4] = 16; //should be 1 hour (but what is host closes during this hour?)
                                    resu_player[x4] = tplayer;
                                    resu_partymember[x4] = 0;
                                    resu_x[x4] = myobj4->x;
                                    resu_y[x4] = myobj4->y;

                                    //like death
                                    OBJmove_allow = TRUE;
                                    OBJmove2(tplayer->party[0], DEATHPOSX, DEATHPOSY);
                                    OBJmove_allow = FALSE;

                                    /*
                  //move player to castle!
                  OBJmove_allow=TRUE;
                  OBJmove2(tplayer->party[0],256+128-16,256+128+2); //place avatar on the map
                  OBJmove_allow=FALSE;
                  */


                                    tnpc->hp = 30;
                                    tnpc->upflags |= 1; //update all upon respawn
                                    tnpc->flags &= (65535 - 1); //remove poisoned flag


                                    //player *resu_player[65536]; //player object belongs to
                                    //unsigned char resu_partymember[65536]; //party member index


                                    //txtset(t,"?"); t->d2[0]=8; txtadd(t,"An unending darkness engulfs thee... A voice in the darkness intones, \x022KAL LOR!\x022");
                                    if (tplayer->GNPCflags[25] == 0) {
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "An unending darkness engulfs thee... (press F1 to resurrect)");
                                    } else {
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(
                                            t,
                                            "An unending darkness engulfs thee... (press F1 to use your insurance and resurrect at healer)");
                                    }
                                    NET_send(NETplayer, tplayer->net, t);


                                    //goto exitloop1;
                                } //karma
                            } //x==0


                            //send message stating the NPC has died

                            //TESTING
                            txtset(t, "??");
                            t->d2[0] = 252;
                            t->d2[1] = x;
                            NET_send(NETplayer, tplayer->net, t);


                            tplayer->party[x] = NULL;
                            if (!x) inbritanniacheck();

                            if (login_dead_callback) goto login_dead2;

                            goto exitloop1;
                            //replace with dead bodies!!!, and blood!
                            //myobj=OBJnew();
                            //myobj->type=
                            //tplayer->party[x]=NULL;
                        } //hp<=0
                    } //active
                } //x
            exitloop1:;


                playerlist[tpl]->globalmessage_wait -= et;
                if (playerlist[tpl]->globalmessage_wait < 0) playerlist[tpl]->globalmessage_wait = 0;
                playerlist[tpl]->idle += et;
                playerlist[tpl]->idle_connect += et;
                playerlist[tpl]->save_update += et;


                if (!playerlist[tpl]->party[0]) playerlist[tpl]->idle = 0; //can't idle if dead


                //metalhead*if ((exitrequest)&&(tpl!=0)) {playerlist[tpl]->save_update=-1; goto save_update1;} //update all players!
                if (exitrequest) {
                    //update all players!
#ifdef CLIENT
                    if (tpl) {
                        txtset(t, "??");
                        t->d2[0] = 250;
                        t->d2[1] = 1;
                        NET_send(NETplayer, playerlist[tpl]->net, t);
                    } //host quit!
#endif
                    playerlist[tpl]->save_update = 0;
                    goto save_update1;
                }

                //MessageBox(NULL,"Saving #1","Ultima 6 Online",MB_OK);

                //metalhead*if ((playerlist[tpl]->save_update>=512)&&(tpl!=0)) {playerlist[tpl]->save_update=-1; goto save_update1;}
                if (playerlist[tpl]->save_update >= 512) {
                    playerlist[tpl]->save_update = -1;
                    goto save_update1;
                }

                if (playerlist[tpl]->GNPCflags[253] & 1) {
                    playerlist[tpl]->GNPCflags[253]--;
                    goto autoleavebritannia;
                }

                if (playerlist[tpl]->party[0]) x = 16;
                else x = 64;
                // r555 idle timeout check???
#ifdef CLIENT
                if ((playerlist[tpl]->idle_connect >= x) && (tpl != 0)) goto connect_failed; //(16sec)

                if ((playerlist[tpl]->idle >= 8192) && (tpl != 0)) {  //remove idle player (not host) (~2 hours)
#else
                if ((playerlist[tpl]->idle_connect >= x)) goto connect_failed; //(16sec)

                if ((playerlist[tpl]->idle >= 8192)) {
                    //remove idle player (not host) (~2 hours)
#endif
                connect_failed:
                    txtset(t, "??");
                    t->d2[0] = 250;
                    t->d2[1] = 9;
                    NET_send(NETplayer, playerlist[tpl]->net, t); //leaving britannia (idle)
                autoleavebritannia:

                    //static txt *u6o_user_name=txtnew(); txtset(u6o_user_name,"UNKNOWN");
                    //static txt *u6o_user_password=txtnew(); txtset(u6o_user_password,"UNKNOWN");

                    //save player
                save_update1:

                    //MessageBox(NULL,"Saving","Ultima 6 Online",MB_OK);

                    //U6O_ERRORTAG


                    if (playerlist[tpl]->name->l == 0) goto save_failed;
                    if (playerlist[tpl]->password->l == 0) goto save_failed;
                    txtset(t, "UNKNOWN");
                    if (txtsame(playerlist[tpl]->name, t)) goto save_failed;
                    if (txtsame(playerlist[tpl]->password, t)) goto save_failed;


                    //find prev savefile
                    i2 = -1;
                    for (i = 0; i <= SAVESLOTLAST; i++) {
                        //search save buffer for player info!
                        if (save_buffer[i]) {
                            if (txtsame(save_username[i], playerlist[tpl]->name)) {
                                if (txtsame(save_password[i], playerlist[tpl]->password)) {
                                    i2 = i; //i2=prev file to remove
                                    goto gotprevsave;
                                } //password
                            } //name
                        } //save_buffer[i]
                    } //i
                gotprevsave:

                    //died from lack of Karma?
                    if ((playerlist[tpl]->party[0] == NULL) && (playerlist[tpl]->karma <= 0)) goto save_complete;
                    //skip save then remove


                    //230-237 x,y location of a dead partymember
                    for (i3 = 0; i3 <= 7; i3++) tplayer->GNPCflags[230 + i3] = 0; //not dead!


                    for (i3 = 0; i3 <= 7; i3++) party_ok[i3] = 1;
                    for (i3 = 0; i3 <= nresu; i3++) {
                        if (resu[i3]) {
                            if (resu_player[i3] == playerlist[tpl]) {
                                party_ok[resu_partymember[i3]] = 0;
                                //set location of dead body
                                tplayer->GNPCflags[230 + resu_partymember[i3]] = resu_x[i3] + (resu_y[i3] << 16);
                                playerlist[tpl]->party[resu_partymember[i3]] = resu[i3];
                            }
                        }
                    }
                    if (playerlist[tpl]->party[0] == NULL) goto save_complete;
                    //ERROR! could not find in resurrection table

                    tnpc = (npc *) playerlist[tpl]->party[0]->more;
                    txtset(t, "New Player");
                    if (txtsame(tnpc->name, t)) goto save_complete; //if a prev savegame exists it will be deleted


                    //find empty savefile
                    for (i = 0; i <= SAVESLOTLAST; i++) {
                        if (save_buffer[i] == 0) goto save_empty;
                    }
                    goto save_failed; //ERROR! all savefile slots are full
                save_empty:

                    //implement new password?
                    if (playerlist[tpl]->newpassword->l) {
                        //check if new password conflicts with existing savegame, if so the password won't change
                        for (i3 = 0; i3 <= SAVESLOTLAST; i3++) {
                            if (save_buffer[i3]) {
                                if (txtsame(playerlist[tpl]->name, save_username[i3])) {
                                    if (txtsame(playerlist[tpl]->newpassword, save_password[i3])) {
                                        goto newpassword_unavailable;
                                    }
                                }
                            }
                        }
                        txtset(playerlist[tpl]->password, playerlist[tpl]->newpassword);
                    }
                newpassword_unavailable:
                    txtset(playerlist[tpl]->newpassword, "");

                    txtset(save_username[i], playerlist[tpl]->name);
                    txtset(save_password[i], playerlist[tpl]->password);
                    tnpc = (npc *) playerlist[tpl]->party[0]->more;
                    txtset(save_name[i], tnpc->name);
                    save_buffer[i] = 1;

                    txtset(t4, "");
                    /*
	     txtset(t,"??"); t->ds[0]=3; txtadd(t4,t); 
	   */
                    txtset(t, "??");
                    t->ds[0] = SAVEVER;
                    txtadd(t4, t);

                    if (SAVEVER == 5) {
                        txtset(t, "?");
                        t->d2[0] = PW_ENCRYPTION;
                        txtadd(t4, t);
                    }

                    txtset(t, "?");
                    t->d2[0] = playerlist[tpl]->name->l;
                    txtadd(t4, t);
                    txtadd(t4, playerlist[tpl]->name);
                    txtset(t, "?");
                    t->d2[0] = playerlist[tpl]->password->l;
                    txtadd(t4, t);
                    txtadd(t4, playerlist[tpl]->password);

                    txtset(t, "??");
                    t->ds[0] = playerlist[tpl]->party[0]->x;
                    if (party_ok[0] == 0) t->ds[0] = 256 + 128 - 16;
                    txtadd(t4, t);
                    txtset(t, "??");
                    t->ds[0] = playerlist[tpl]->party[0]->y;
                    if (party_ok[0] == 0) t->ds[0] = 256 + 128 + 2;
                    txtadd(t4, t);


                    txtset(t, "?");
                    t->d2[0] = playerlist[tpl]->male_female;
                    txtadd(t4, t);
                    txtNEWLEN(t, 256 * 4);
                    memcpy(t->d, &playerlist[tpl]->NPCflags, 256 * 4);
                    txtadd(t4, t);
                    //GLOBAL FLAG 255=craft type
                    playerlist[tpl]->GNPCflags[255] = 0;
                    tplayer->GNPCflags[254] = ett;
                    if (playerlist[tpl]->craft) {
                        if ((playerlist[tpl]->party[0]->type & 1023) == 412) playerlist[tpl]->GNPCflags[255] = 1; //ship
                        if ((playerlist[tpl]->party[0]->type & 1023) == 414) playerlist[tpl]->GNPCflags[255] = 2;
                        //skiff
                        if ((playerlist[tpl]->party[0]->type & 1023) == 415) playerlist[tpl]->GNPCflags[255] = 3; //raft
                        if ((playerlist[tpl]->party[0]->type & 1023) == 423) playerlist[tpl]->GNPCflags[255] = 4;
                        //balloon
                    }

                    //*THIS LINE HAS BEEN EDITED TO ONLY ALLOW PRIMARY PLAYER'S CHARACTER TO BE SAVED!
                    x3 = 0;
                    for (x2 = 1; x2 <= 7; x2++) {
                        //ONLY IF NOT A CORE NPC!
                        if (playerlist[tpl]->party[x2]) {
                            tnpc3 = (npc *) playerlist[tpl]->party[x2]->more;
                            if (tnpc3->converse == 201) x3++;
                        }
                    }
                    playerlist[tpl]->GNPCflags[252] = x3; //no. companion party members

                    static SYSTEMTIME systime;
                    GetLocalTime(&systime);
                    x3 = (systime.wDay - 1) + (systime.wMonth - 1) * 32 + systime.wYear * (32 * 12);
                    playerlist[tpl]->GNPCflags[250] = x3; //250 YMD of last save


                    if (houseowner(playerlist[tpl], playerlist[tpl]->GNPCflags[28])) {
                        //set number of storage positions
                        playerlist[tpl]->GNPCflags[29] = housestoragenext[playerlist[tpl]->GNPCflags[28]];
                    } else {
                        //NULL house info
                        playerlist[tpl]->GNPCflags[28] = 0;
                        playerlist[tpl]->GNPCflags[29] = 0;


                        //MessageBox(NULL,"HOUSESTORAGECHECK: [28]=0","Ultima 6 Online",MB_OK);
                        //txtnumint(t9,houseowner_FAILVALUE);
                        //MessageBox(NULL,t9->d,"Ultima 6 Online",MB_OK);
                    }
                    //if playerlist[tpl]->GNPCflags[28]!=0 house is valid

                    //247&248 TEMP food values for npc 0-7, 1 byte each (only used when saving/loading)
                    playerlist[tpl]->GNPCflags[247] = 0;
                    playerlist[tpl]->GNPCflags[248] = 0;
                    for (x2 = 0; x2 <= 7; x2++) {
                        if (playerlist[tpl]->party[x2]) {
                            tnpc3 = (npc *) playerlist[tpl]->party[x2]->more;
                            x3 = tnpc3->food;
                            if (x2 == 0) playerlist[tpl]->GNPCflags[247] |= x3;
                            if (x2 == 1) playerlist[tpl]->GNPCflags[247] |= (x3 << 8);
                            if (x2 == 2) playerlist[tpl]->GNPCflags[247] |= (x3 << 16);
                            if (x2 == 3) playerlist[tpl]->GNPCflags[247] |= (x3 << 24);
                            if (x2 == 4) playerlist[tpl]->GNPCflags[248] |= x3;
                            if (x2 == 5) playerlist[tpl]->GNPCflags[248] |= (x3 << 8);
                            if (x2 == 6) playerlist[tpl]->GNPCflags[248] |= (x3 << 16);
                            if (x2 == 7) playerlist[tpl]->GNPCflags[248] |= (x3 << 24);
                        }
                    }

                    txtNEWLEN(t, 256 * 4);
                    memcpy(t->d, &playerlist[tpl]->GNPCflags, 256 * 4);
                    txtadd(t4, t);
                    txtset(t, "????");
                    t->df[0] = playerlist[tpl]->karma;
                    txtadd(t4, t);


                    for (x8 = 0; x8 <= 7; x8++) {
                        if (playerlist[tpl]->party[x8]) {
                            tnpc = (npc *) playerlist[tpl]->party[x8]->more;
                            if ((x8 == 0) || (tnpc->converse == 201)) {
                                //***NOT A U6 NPC***

                                txtset(t, "??");
                                t->ds[0] = playerlist[tpl]->party[x8]->type;
                                if (playerlist[tpl]->craft) {
                                    if (x8 == 0) t->ds[0] = playerlist[tpl]->craft->type;
                                }
                                if (tnpc->horse) {
                                    //z3=playerlist[tpl]->party[x8]->type>>11;//z3 is the direction of the horse

                                    t->ds[0] = tnpc->originalobjtype;

                                    //exit(tnpc->originalobjtype);
                                }

                                txtadd(t4, t); //player type!
                                txtset(t, "?");
                                t->d2[0] = tnpc->name->l;
                                txtadd(t4, t);
                                txtadd(t4, tnpc->name);
                                txtset(t, "??");
                                t->ds[0] = tnpc->port;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->hp;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->hp_max;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->mp;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->mp_max;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->s;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->d;
                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->i;
                                txtadd(t4, t);
                                txtset(t, "????");
                                t->dl[0] = tnpc->exp;


                                //FOLLOWING LINE REMOVED TO ALLOW FOR LOGGING BACK IN AS DEAD
                                //if (party_ok[x8]==0) t->dl[0]=tnpc->exp-int(tnpc->exp/8); //deduct xp required for ressurection ***FIXED 2005***


                                txtadd(t4, t);
                                txtset(t, "??");
                                t->ds[0] = tnpc->lev;
                                txtadd(t4, t);
                                txtset(t, "?");
                                t->d2[0] = tnpc->flags;
                                txtadd(t4, t);

                                //npc->OBJ
                                txtset(t, "");
                                txtset(t2, "????");
                                x5 = 0;
                                for (x2 = 0; x2 <= 9; x2++) {
                                    if (x2 == 0) myobj = tnpc->helm;
                                    if (x2 == 1) myobj = tnpc->wep_right;
                                    if (x2 == 2) myobj = tnpc->wep_left;
                                    if (x2 == 3) myobj = tnpc->armour;
                                    if (x2 == 4) myobj = tnpc->boots;
                                    if (x2 == 5) myobj = tnpc->neck;
                                    if (x2 == 6) myobj = tnpc->ring_right;
                                    if (x2 == 7) myobj = tnpc->ring_left;
                                    if (x2 == 8) myobj = (object *) tnpc->items; //includes the bag!
                                    if (x2 == 9) myobj = playerlist[tpl]->mobj; //*ONLY PARTY[0]
                                    if (x8 && (x2 == 9)) myobj = NULL;


                                    if (myobj) {
                                        t2->dl[0] = x5 + 1;
                                        txtadd(t, t2);
                                    } else {
                                        t2->dl[0] = 0;
                                        txtadd(t, t2); //no object
                                    }


                                    x5 = OBJlist(myobj);
                                }
                                OBJlist_last = NULL;

