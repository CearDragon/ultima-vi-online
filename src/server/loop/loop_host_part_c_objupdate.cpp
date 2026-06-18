// LHS-P5.2: part_c_objupdate -- MEGA C start (object/creature/NPC AI update).
// HOT PATH (performance-sensitive). LHS brace-seam: CONTINUES the part_00
// fragment; OPENS the if(ouln!=-1){ for(i..){ blocks CLOSED in
// loop_host_part_c_npcattack_c.cpp. Contents: object update entry +
// powderkeg/explosions (powderkeg_*, extractobj6). Move-only: no STL, no reorg.

    u6opi = 5;
    //update objects/creatures/npc
    if (ouln != -1) {
        for (i = 0; i <= ouln; i++) {
            if (myobj = oul[i]) {
                //skip objects in void!
                if ((myobj->y >= 1000) && (myobj->x >= 2016)) goto skip_ouli;
                //if ((myobj->y>=256)&&(myobj->x>=1280)) goto skip_ouli; //2005: free map area


                i2 = myobj->type & 1023;
                //i,i2 reserved, all other free

                if (myobj->type == (223 + 1024)) {
                    //powder keg
                    f = 0.25f;
                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                        x = (myobj->info >> 9) & 15;
                        x--;
                        myobj->info = (myobj->info & (0xFFFF - 7680)) + (x << 9);
                        if (!x) {
                            static object *powderkeg;
                            powderkeg = myobj;

                            myobj = NULL;
                            tnpc = NULL;
                            for (x2 = 0; x2 <= playerlist_last; x2++) {
                                if (playerlist[x2]) {
                                    tplayer2 = playerlist[x2];
                                    if (tplayer2->id == powderkeg->more2) {
                                        if (tplayer2->party[0]) {
                                            myobj = tplayer2->party[0];
                                            tnpc = (npc *) myobj->more;
                                        }
                                        goto powderkeg_gotsource;
                                    }
                                }
                            }
                        powderkeg_gotsource:

                            static long spellx, spelly;

                            //copied from "explosion spell"
                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (OBJcheckbolt(powderkeg->x, powderkeg->y, powderkeg->x + spellx,
                                                         powderkeg->y + spelly))
                                            goto explosionblocked2;

                                        myobj2 = OBJfindlast(powderkeg->x + spellx, powderkeg->y + spelly);
                                        if (myobj2 == NULL)
                                            myobj2 = OBJfindlastall(powderkeg->x + spellx,
                                                                    powderkeg->y + spelly);
                                        if (myobj2) {
                                            if ((myobj2->info & 8) == 0) {
                                                //no redirector

                                                x2 = myobj2->type & 1023;
                                                x3 = myobj2->type >> 10;

                                                if ((x2 >= 297) && (x2 <= 300)) {
                                                    //door
                                                    if ((x3 >= 4) && (x3 < 12)) {
                                                        //closed but not magically locked
                                                        x4 = 4;
                                                        if (x3 >= 8) {
                                                            //door is locked and needs unlocking
                                                            myobj3 = OBJfindlastall(myobj2->x, myobj2->y);
                                                            if (myobj3->info & 8) myobj3 = (object *) myobj3->more;
                                                            //<-redirect
                                                            if (myobj3->more2) goto powderkeg_needskey;
                                                            x4 = 8;
                                                        }
                                                        myobj2->type =
                                                                (myobj2->type & 1023) + (
                                                                    ((myobj2->type >> 10) - x4) << 10);
                                                        OBJcheckflags(myobj2->x, myobj2->y);
                                                        myobj2 = (object *) myobj2->more;
                                                        myobj2->type =
                                                                (myobj2->type & 1023) + (
                                                                    ((myobj2->type >> 10) - x4) << 10);
                                                        OBJcheckflags(myobj2->x, myobj2->y);
                                                        if (tnpc) {
                                                            if (tnpc->player) {
                                                                txtset(t, "?The door was blown open!");
                                                                t->d2[0] = 8;
                                                                NET_send(NETplayer, tnpc->player->net, t);
                                                            }
                                                        }
                                                        goto powderkeg_dooropened;
                                                    }
                                                }
                                            powderkeg_needskey:

                                                if ((myobj2->type & 1023) == 98) {
                                                    //chest
                                                    if ((myobj2->type >> 10) <= 2) {
                                                        //not magically locked
                                                        //destroy chest and extract objects
                                                        x3 = myobj2->x;
                                                        y3 = myobj2->y;
                                                        OBJsave(x3, y3);
                                                        VLNKremove(myobj2);
                                                        OBJremove(myobj2);
                                                    extractobj6:
                                                        myobj3 = (object *) myobj2->more;
                                                        if (myobj3) {
                                                            OBJremove(myobj3);
                                                            myobj3->info |= 112;
                                                            OBJadd(x3, y3, myobj3);
                                                            goto extractobj6;
                                                        }
                                                    }
                                                } //98

                                                if ((myobj2->type & 1023) == 223) {
                                                    //powder keg
                                                    if (myobj2 != powderkeg) {
                                                        if ((myobj2->more2 == 0) || (
                                                                myobj2->more2 == powderkeg->more2)) {
                                                            OBJsave(myobj2->x, myobj2->y);
                                                            myobj2->info = (myobj2->info & (0xFFFF - 7680)) + (1 << 9);
                                                            myobj2->type = 223 + 1 * 1024;
                                                            myobj2->more2 = powderkeg->more2;
                                                        }
                                                    }
                                                }

                                                if (myobj2->info & 4) {
                                                    //<-crt
                                                    crt = (creature *) myobj2->more;
                                                    x2 = rnd * 96;
                                                    if (x2 == 0) goto explosion_spelldone2;
                                                    //fire resistant???
                                                    i3 = myobj2->type & 1023;
                                                    if (i3 == 411) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //dragon
                                                    if (i3 == 369) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //drake
                                                    if (i3 == 352) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //ghost
                                                    if (i3 == 374) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //hydra
                                                    if (i3 == 369) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //drake
                                                    if (i3 == 368) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //skeleton
                                                    if (i3 == 373) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //wisp
                                                    if (i3 == 367) {
                                                        x2 = 0;
                                                        goto IFimmuneflame3;
                                                    } //daemon
                                                    //double damage?
                                                    if (i3 == 364) x2 *= 2; //acid slug
                                                    if (i3 == 357) x2 *= 2; //corpser
                                                    if (i3 == 347) x2 *= 2; //reaper
                                                    if (i3 == 360) x2 *= 2; //rot worms
                                                    if (i3 == 375) x2 *= 2; //slime
                                                    if (i3 == 365) x2 *= 2; //tangle vine
                                                    i3 = SFnew(myobj2->x, myobj2->y);
                                                    sf[i3].type = 1; //attack
                                                    sf[i3].x2 = x2;
                                                    sf[i3].x = myobj2->x;
                                                    sf[i3].y = myobj2->y;
                                                    sf[i3].wait = 0.125f; //NULL
                                                    sf[i3].more = 1;
                                                    CASTSPELL_SPELLTYPE = 65535;
                                                    spellattcrt = TRUE;
                                                    goto spellattcrt0;
                                                IFimmuneflame3:;
                                                } //crt

                                                if (myobj2->info & 2) {
                                                    //npc
                                                    tnpc2 = (npc *) myobj2->more;
                                                    if (tnpc2->player) {
                                                        if (tnpc2->player->id == powderkeg->more2) {
                                                            x2 = rnd * 48;
                                                            x3 = rnd * (tnpc2->arm + 1);
                                                            x2 -= x3;
                                                            if (x2 < 0) x2 = 0;
                                                            if (x2) {
                                                                tnpc2->hp -= x2;
                                                                tnpc2->upflags |= 2; //hp
                                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                                sf[i3].type = 1; //attack
                                                                sf[i3].x2 = x2;
                                                                sf[i3].x = myobj2->x;
                                                                sf[i3].y = myobj2->y;
                                                                sf[i3].wait = 0.125f; //NULL
                                                                sf[i3].more = 1;
                                                            } //x2
                                                        } //id
                                                    } //player
                                                } //npc
                                            } //no redirector
                                        } //myobj2
                                    explosion_spelldone2:
                                    powderkeg_dooropened:

                                        if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                            i9 = SFnew(powderkeg->x + spellx, powderkeg->y + spelly);
                                            sf[i9].type = 8; //wand
                                            sf[i9].x = powderkeg->x;
                                            sf[i9].y = powderkeg->y;
                                            sf[i9].x2 = powderkeg->x + spellx;
                                            sf[i9].y2 = powderkeg->y + spelly;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }

                                    explosionblocked2:;
                                    } //!=4
                                }
                            } //spell x,y

                            OBJremove(powderkeg);
                        } //!x
                    } //f
                    goto skip_ouli;
                }


                if (myobj->info & 4) {
                    //<-crt
                    static unsigned char target_struct;
                    target_struct = 2;

                    x = myobj->x;
                    y = myobj->y;
                    if (crt = (creature *) myobj->more) {
                        if (crt->hp != NULL) {
                            if (crt->flags & 2) {
                                //invisible!
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    x2 = rnd * 64;
                                    if (!x2) crt->flags -= 2;
                                    x2 = rnd * 16;
                                    if (!x2) VLNKremove(myobj);
                                } //1.0
                            } //2


                            if (crt->flags & 32) {
                                //charmed!
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    if (crt->charm > 0) {
                                        crt->charm--;
                                    } else {
                                        crt->flags -= 32;
                                        crt->al = 1;
                                    }
                                } //1.0
                            } //32

                            if (crt->flags & 1) {
                                //poisoned?
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    x2 = rnd * 2;
                                    if (x2 == 0) {
                                        if (crt->hp) crt->hp--;
                                        i3 = SFnew(myobj->x, myobj->y);
                                        sf[i3].type = 1; //attack
                                        sf[i3].x2 = 1;
                                        sf[i3].x = myobj->x;
                                        sf[i3].y = myobj->y;
                                        sf[i3].wait = 0.125f; //NULL
                                        sf[i3].more = 1;
                                    }
                                }
                            }


                            if (myobj2 = (object *) myobj->prev) {
                                if (myobj2->type == OBJ_POISON_FIELD) {
                                    //poison field
                                    crt->flags |= 1; //poison creature
                                }

                                if (myobj2->type == OBJ_FIRE_FIELD) {
                                    //fire field -8hp/sec
                                    i3 = myobj->type & 1023;
                                    x2 = 2;
                                    if (i3 == 411) x2 = 0; //dragon
                                    if (i3 == 369) x2 = 0; //drake
                                    if (i3 == 352) x2 = 0; //ghost
                                    if (i3 == 374) x2 = 0; //hydra
                                    if (i3 == 369) x2 = 0; //drake
                                    if (i3 == 368) x2 = 0; //skeleton
                                    if (i3 == 373) x2 = 0; //wisp
                                    if (i3 == 367) x2 = 0; //daemon
                                    //double damage?
                                    if (i3 == 364) x2 *= 2; //acid slug
                                    if (i3 == 357) x2 *= 2; //corpser
                                    if (i3 == 347) x2 *= 2; //reaper
                                    if (i3 == 360) x2 *= 2; //rot worms
                                    if (i3 == 375) x2 *= 2; //slime
                                    if (i3 == 365) x2 *= 2; //tangle vine
                                    if (x2) {
                                        f = 0.5f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            i3 = SFnew(myobj->x, myobj->y);
                                            sf[i3].type = 1; //attack
                                            sf[i3].x2 = x2; //attack
                                            sf[i3].x = myobj->x;
                                            sf[i3].y = myobj->y;
                                            sf[i3].wait = 0.125f; //NULL
                                            sf[i3].more = 1;
                                            crt->hp--;
                                        } //f
                                    } //x2
                                } //fire field


                                if (myobj2->type == OBJ_SLEEP_FIELD) {
                                    //sleep field
                                    x2 = 4 + rnd * 16;
                                    //immune to sleep?
                                    i3 = myobj->type & 1023;
                                    if (i3 == 364) x2 = 0; //acid slug
                                    if (i3 == 427) x2 = 0; //ant
                                    if (i3 == 357) x2 = 0; //corpser
                                    if (i3 == 367) x2 = 0; //daemon
                                    if (i3 == 362) x2 = 0; //winged garg
                                    if (i3 == 363) x2 = 0; //garg
                                    if (i3 == 355) x2 = 0; //gazer
                                    if (i3 == 352) x2 = 0; //ghost
                                    if (i3 == 374) x2 = 0; //hydra
                                    if (i3 == 343) x2 = 0; //insects
                                    if (i3 == 347) x2 = 0; //reaper
                                    if (i3 == 360) x2 = 0; //rotworms
                                    if (i3 == 426) x2 = 0; //scorpion
                                    if (i3 == 368) x2 = 0; //skeleton
                                    if (i3 == 375) x2 = 0; //slime
                                    if (i3 == 345) x2 = 0; //squid
                                    if (i3 == 365) x2 = 0; //tanglevine
                                    if (i3 == 373) x2 = 0; //wisp
                                    if ((crt->flags & 4) == 0) {
                                        //NOTE: sleep uses a paralyze effect, but to identify the difference visually (sleeping body) asleep flag is set
                                        f = 1.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            crt->wait += x2;
                                            crt->flags |= (4 + 8); //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                        } //f
                                    }
                                } //sleep field


                                if (myobj2->type == OBJ_WEB) {
                                    //web paralyze monsters
                                    x2 = rnd * 3;
                                    if (x2 == 0) {
                                        x2 = 4 + rnd * 8;
                                        //immune to sleep?
                                        i3 = myobj->type & 1023;
                                        if (i3 == OBJ_GIANT_SPIDER) x2 = 0; //giant spider
                                        if ((crt->flags & 8) == 0) {
                                            //make creature stuck in the web
                                            f = 1.0f;
                                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                                crt->wait += x2;
                                                crt->flags |= 8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                            } //f
                                        }
                                    }
                                } //web
                            } //prev object

                            if (crt->respawn_hp == 0) {
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    crt->respawn_x--;
                                    if (crt->respawn_x == 0) goto summonedcreature_remove;
                                }
                            }

                            if ((myobj->type & 1023) == 366) crt->hp = 32767; //invincible tanglevine tendril!
                            if ((myobj->type & 1023) == 430) if (crt->flags & 128) crt->hp = 32767; //invincible horse


                            //creature died!
                            if (crt->hp <= 0) {
