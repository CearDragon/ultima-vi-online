// LHS-P5.3: part_c_crtenum -- CONTINUES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_c_objupdate.cpp.
// Contents: summoned-creature removal + creature enumeration/targeting
// (crtenum_*, invalidatetarget). Move-only: no STL, no reorg.
                            summonedcreature_remove:
                                myobj2 = myobj;
                                myobj4 = (object *) crt->items->more; //will point to first item
                            dropgoldmore2:
                                if (myobj4) {
                                    myobj5 = (object *) myobj4->next;
                                    OBJmove_allow = TRUE;
                                    OBJmove(myobj4, myobj2->x, myobj2->y);
                                    OBJmove_allow = FALSE;
                                    myobj4->info |= 112; //set flags as temp OBJ
                                    if (myobj5) {
                                        myobj4 = myobj5;
                                        goto dropgoldmore2;
                                    }
                                }
                                crtrespawn(myobj2);
                                goto donemove;
                            } //hp<=0


                            static unsigned char crt_casti = 0;


                            f = 0.25f;
                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                //cast
                                x2 = rnd * 32;
                                if (x2 == 0) {
                                    crt->cast = 1;
                                } //x2==0
                            } //cast

                            f = 8.0f - ((float) (obji[sprlnk[myobj->type & 1023]].v7 & 255)) / 8.0f; //mp+ interval
                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                //increase mp
                                crt->mp++;
                                if (crt->mp > 8) crt->mp = 8;
                                if (crt->mp == 8) crt->cast = 1; //force cast on max mp
                            }

                            static object *validate_obj;
                            validate_obj = NULL;

                            crt->wait -= et;
                            if (crt->wait < 0) crt->wait = 0;

                            if (crt->target) {
                                f = 2.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    //reselect target
                                    if ((crt->al == 1) || (crt->al == 4)) {
                                        //not al==3(passive turned aggresive)
                                        x2 = rnd * 32;
                                        if (x2 == 0) {
                                            f = 0.5f + rnd * 0.5f;
                                            if (crt->wait < f) crt->wait = f;
                                            VLNKsremove(myobj);
                                            crt->target = NULL;
                                            crt->pathn = 0;
                                            goto donemove;
                                        }
                                    } //al
                                } //reselect target


                                f = 0.5f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    //validate target (~once every 4 sec)
                                    if ((crt->al == 1) || (crt->al == 4)) {
                                        //not al==3(passive turned aggresive)
                                        x2 = rnd * 8;
                                        if (x2 == 0) {
                                            validate_obj = crt->target;
                                            VLNKsremove(myobj);
                                            crt->target = NULL;
                                            crt->pathn = 0;
                                        }
                                    }
                                } //validate target
                            } //crt->target

                            if (crt->wait == 0) {
                                if (crt->flags & 8) crt->flags -= 8;
                                //unparalyze //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                if (crt->flags & 4) crt->flags -= 4;
                                //awaken //1=poison, 2=invisible, 4=asleep, 8=paralyzed


                                if (crt->al == 0) {
                                    x2 = rnd * 4;
                                    x3 = x;
                                    y3 = y;
                                    if (x2 == 0) x3--;
                                    if (x2 == 1) x3++;
                                    if (x2 == 2) y3--;
                                    if (x2 == 3) y3++;
                                    f = 0.5f + rnd * 0.5f;
                                    if (crt->wait < f) crt->wait = f;
                                    goto crt_walk; //wander
                                } //al==0

                                //STEP 1: ENUMERATE TARGETS
                                /*
                struct crtenum_struct{
                char x;
                char y;
                };
                crtenum_struct crtenum[1073];
                object* crtenum_pathok_castok[1024]; short crtenum_pathok_castok_i;
                object* crtenum_pathok[1024]; short crtenum_pathok_i;
                object* crtenum_castok[1024]; short crtenum_castok_i;


                significant changes:
                can't target on diagonals (goes for party npc too)
                should CHECK for best target randomly every 8 seconds, keeping current target only if it is the best
                MUST ALWAYS SELECT A TARGET FROM THE "BEST" GROUP
                BEST ORDER:
                1. PATH+MAGIC
                2. PATH ONLY
                3. MAGIC ONLY
                these 3 changes will fix 99% of combat issues


                */
                                if (!crt->target) {
                                    x4 = myobj->type & 1023;
                                    x5 = 1; //movement type
                                    if (x4 == 365) x5 = -1; //tanglevine
                                    if (x4 == 347) x5 = -1; //reaper
                                    if (x4 == 345) x5 = 2; //squid
                                    if (x4 == 346) x5 = 2; //sea serpent
                                    if (x4 == 343) x5 = 3; //insects
                                    if (x4 == 344) x5 = 3; //bat
                                    if (x4 == 356) x5 = 3; //bird
                                    if (x4 == 362) x5 = 3; //winged gargoyle
                                    if (x4 == 369) x5 = 3; //drake
                                    if (x4 == 372) x5 = 3; //mongbat
                                    if (x4 == 373) x5 = 3; //wisp
                                    if (x4 == 411) x5 = 3; //dragon
                                    if (x4 == 352) x5 = 4; //GHOST (4=unlimited!)
                                    if (x4 == 366) x5 = 5; //tanglevine tendril

                                    //ERROR!!! DO PASSIVE MONSTERS NEED TO ENUMERATE??? NO THEY DO NOT!!
                                    //on surface max distance is 18
                                    //max distance underground is 10
                                    i4 = 18;
                                    if ((x >= 1024) && (!((y >= 256) && (x < 1280) && (y < 512)))) i4 = 10;
                                    // fixed so that excludes gargland properly
                                    ZeroMemory(&wpf_sourcedest, sizeof(wpf_sourcedest));

                                    WPF_OBJECT = myobj;
                                    if (x5 != -1) wpf_pathfind(NULL, x, y, 0, 0, i4, x5, 8);
                                    //find all accessable squares

                                    target_struct = 2 + 4; //target NPC + CRT
                                    if (crt->al == 4) target_struct = 4; //target CRT

                                    crtenum_pathok_castok_i = -1;
                                    crtenum_pathok_i = -1;
                                    crtenum_castok_i = -1;
                                    i4 = 1072;
                                    if ((x >= 1024) && (!((y >= 256) && (x < 1280) && (y < 512)))) i4 = 440;
                                    for (x2 = 0; x2 <= i4; x2++) {
                                        x3 = x + crtenum[x2].x;
                                        y3 = y + crtenum[x2].y;
                                        if (myobj2 = od[y3][x3]) {
                                            if ((x3 >= 0) && (x3 <= 2047) && (y3 >= 0) && (y3 <= 1023)) {
                                            crtenum_next:
                                                if (myobj2->info & target_struct) {
                                                    //gpg top right corner of map dead guys
                                                    tnpc2 = (npc *) myobj2->more;
                                                    i3 = 1;
                                                    if (target_struct == 4) {
                                                        //targeting CRT only, crt is friendly
                                                        if (tnpc2->al != 1) i3 = 0;
                                                    } else {
                                                        //targeting NPC + CRT, crt is NOT friendly
                                                        if (myobj2->info & 2) {
                                                            //target_struct=2;
                                                            if (tnpc2->player) {
                                                                if (((myobj->type & 1023) == 362) || (
                                                                        (myobj->type & 1023) == 363)) {
                                                                    //gargoyle
                                                                    if (myobj->x >= 1024) {
                                                                        if (myobj->y >= 256) {
                                                                            if (myobj->x < 1280) {
                                                                                if (myobj->y < 512) {
                                                                                    //amulet of submission check
                                                                                    tplayer2 = tnpc2->player;
                                                                                    if (tplayer2->GNPCflags[12]) i3 = 0;
                                                                                    //has amulet of submission or has finished game
                                                                                    //beh lem check! (the tricky one) (if he is dead gargs attack, and this is only natural)
                                                                                    for (x4 = 1; x4 <= 7; x4++) {
                                                                                        if (tplayer2->party[x4]) {
                                                                                            tnpc3 = (npc *) tplayer2->
                                                                                                    party[x4]->more;
                                                                                            if (tnpc3->converse == 162)
                                                                                                i3 = 0;
                                                                                        } //tplayer2->party[x4]
                                                                                    } //x4
                                                                                }
                                                                            }
                                                                        }
                                                                    } //x,y,x,y
                                                                } //gargoyle
                                                            } else {
                                                                i3 = 0; //crt can't attack passive NPC
                                                            }
                                                        } else {
                                                            //target_struct=4;
                                                            if (tnpc2->al != 4) i3 = 0;
                                                            //crt can't attack al==1 or passive monsters
                                                        } //info&2
                                                    } //target_struct==4
                                                    if (tnpc2->flags & 2) i3 = 0; //crt can't target invisible CRT/NPC
                                                    if (i3) {
                                                        i3 = 3;
                                                        //check path finding
                                                        x4 = abs(myobj->x - myobj2->x);
                                                        y4 = abs(myobj->y - myobj2->y);
                                                        if ((x4 <= 1) && (y4 <= 1)) goto crtenum_close;
                                                        if (x5 == -1) {
                                                            i3--;
                                                            goto crtenum_close;
                                                        }
                                                        i5 = 1;
                                                        if (wpf_sourcedest[myobj2->x - WPF_OFFSETX][
                                                                myobj2->y - WPF_OFFSETY] & 1)
                                                            i5 = 0;
                                                        if (wpf_sourcedest[myobj2->x - WPF_OFFSETX - 1][
                                                                myobj2->y - WPF_OFFSETY] & 1)
                                                            i5 = 0;
                                                        if (wpf_sourcedest[myobj2->x - WPF_OFFSETX + 1][
                                                                myobj2->y - WPF_OFFSETY] & 1)
                                                            i5 = 0;
                                                        if (wpf_sourcedest[myobj2->x - WPF_OFFSETX][
                                                                myobj2->y - WPF_OFFSETY - 1] & 1)
                                                            i5 = 0;
                                                        if (wpf_sourcedest[myobj2->x - WPF_OFFSETX][
                                                                myobj2->y - WPF_OFFSETY + 1] & 1)
                                                            i5 = 0;
                                                        i3 -= i5;
                                                    crtenum_close:
                                                        //check casting
                                                        if ((abs(x - x3) <= 8) && (abs(y - y3) <= 8)) {
                                                            //in spell range!
                                                            if (OBJcheckbolt(x, y, x3, y3)) i3 -= 2; //blocked
                                                        } else {
                                                            i3 -= 2;
                                                        }
                                                        if (i3 == 3) {
                                                            crtenum_pathok_castok_i++;
                                                            crtenum_pathok_castok[crtenum_pathok_castok_i] = myobj2;
                                                        }
                                                        if (i3 == 2) {
                                                            crtenum_castok_i++;
                                                            crtenum_castok[crtenum_castok_i] = myobj2;
                                                        }
                                                        if (i3 == 1) {
                                                            crtenum_pathok_i++;
                                                            crtenum_pathok[crtenum_pathok_i] = myobj2;
                                                        }
                                                    } //i3
                                                } //myobj2->info&target_struct
                                                if (myobj2->next) {
                                                    myobj2 = (object *) myobj2->next;
                                                    goto crtenum_next;
                                                }
                                            } //x&y check
                                        } //myobj2
                                    } //x2


                                    //choose a target based on enumerated info
                                    if ((crtenum_pathok_castok_i == -1) && (crtenum_pathok_i == -1) && (
                                            crtenum_castok_i == -1)) {
                                        //no target!
                                        validate_obj = NULL;
                                        x2 = rnd * 4;
                                        x3 = x;
                                        y3 = y;
                                        if (x2 == 0) x3--;
                                        if (x2 == 1) x3++;
                                        if (x2 == 2) y3--;
                                        if (x2 == 3) y3++;
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        goto crt_walk; //wander
                                    }

                                    if (crtenum_pathok_castok_i != -1) {
                                        if (validate_obj) {
                                            for (x2 = 0; x2 <= crtenum_pathok_castok_i; x2++) {
                                                if (crtenum_pathok_castok[x2] == validate_obj) {
                                                    validate_obj = NULL;
                                                    myobj2 = crtenum_pathok_castok[x2];
                                                    goto crtenum_gottarget;
                                                }
                                            } //x2
                                            validate_obj = NULL;
                                        } //validate_obj
                                        x2 = 0;
                                    crtenum_selecttarget:
                                        myobj2 = crtenum_pathok_castok[x2];
                                        y2 = rnd * 2;
                                        if (y2 || (crtenum_pathok_castok_i == x2)) goto crtenum_gottarget;
                                        if (crtenum_pathok_castok_i > x2) {
                                            x2++;
                                            goto crtenum_selecttarget;
                                        }
                                    }

                                    if (crtenum_pathok_i != -1) {
                                        if (validate_obj) {
                                            for (x2 = 0; x2 <= crtenum_pathok_i; x2++) {
                                                if (crtenum_pathok[x2] == validate_obj) {
                                                    validate_obj = NULL;
                                                    myobj2 = crtenum_pathok[x2];
                                                    goto crtenum_gottarget;
                                                }
                                            } //x2
                                            validate_obj = NULL;
                                        } //validate_obj
                                        x2 = 0;
                                    crtenum_selecttarget2:
                                        myobj2 = crtenum_pathok[x2];
                                        y2 = rnd * 2;
                                        if (y2 || (crtenum_pathok_i == x2)) goto crtenum_gottarget;
                                        if (crtenum_pathok_i > x2) {
                                            x2++;
                                            goto crtenum_selecttarget2;
                                        }
                                    }

                                    if (crtenum_castok_i != -1) {
                                        if (validate_obj) {
                                            for (x2 = 0; x2 <= crtenum_castok_i; x2++) {
                                                if (crtenum_castok[x2] == validate_obj) {
                                                    validate_obj = NULL;
                                                    myobj2 = crtenum_castok[x2];
                                                    goto crtenum_gottarget;
                                                }
                                            } //x2
                                            validate_obj = NULL;
                                        } //validate_obj
                                        x2 = 0;
                                    crtenum_selecttarget3:
                                        myobj2 = crtenum_castok[x2];
                                        y2 = rnd * 2;
                                        if (y2 || (crtenum_castok_i == x2)) goto crtenum_gottarget;
                                        if (crtenum_castok_i > x2) {
                                            x2++;
                                            goto crtenum_selecttarget3;
                                        }
                                    }
                                crtenum_gottarget:
                                    VLNKsremove(myobj); //remove any prev vlnk(s) created
                                    crt->target = myobj2;
                                    crt->pathn = 0;
                                    VLNKnew(myobj, crt->target, (unsigned long) &crt->target); //add new vlnk to target
                                    f = 0.5f + rnd * 0.5f;
                                    if (crt->wait < f) crt->wait = f;

                                    if (myobj2->info & 2) {
                                        //play battle music!
                                        tnpc = (npc *) myobj2->more;
                                        if (tnpc->player) {
                                            if (obji[sprlnk[myobj->type & 1023]].v5 >= 10) {
                                                //battle music: melee
                                                txtset(t, "????");
                                                t->d2[0] = 30;
                                                t->d2[1] = 9; //melee.mid
                                                t->ds[1] = 32; //seconds to play midi for
                                                NET_send(NETplayer, tnpc->player->net, t);
                                            }
                                        }
                                    }
                                    goto donemove;
                                } //->target==NULL

                                //STEP 2: REVALIDATE TARGET
                                myobj2 = crt->target;
                                //empty skiff/boat/...?
                                if ((myobj2->info & (2 + 4)) == 0) goto invalidatetarget;
                                tnpc = (npc *) myobj2->more;
                                crt2 = (creature *) tnpc;
                                target_struct = myobj2->info & (2 + 4);
                                //invisible?
                                if (tnpc->flags & 2) goto invalidatetarget;
                                //recheck allegiance
                                if (target_struct == 4) {
                                    //targeting CRT
                                    if ((crt->al == 1) && (crt2->al == 1)) goto invalidatetarget;
                                    if ((crt->al == 4) && (crt2->al == 4)) goto invalidatetarget;
                                } else {
                                    //targeting NPC
                                    if (!tnpc->player) goto invalidatetarget;
                                    if (crt->al == 4) goto invalidatetarget;
                                }
                                goto revalidatetargetok;
                            invalidatetarget:
                                f = 0.5f + rnd * 0.5f;
                                if (crt->wait < f) crt->wait = f;
                                VLNKsremove(myobj);
                                crt->target = NULL;
                                crt->pathn = 0;
                                goto donemove;
                            revalidatetargetok:

                                //STEP 3: CAST
                                //after casting if nothing else is possible we go back to the enumeration phase


                                //have target!
                                //attack with a spell?
                                //crt spell
                                //in range?
                                x2 = crt->target->x;
                                y2 = crt->target->y;

                                if (crt->cast) {
                                    if (stormcloakcheck(x, y)) goto crtcantcast;
                                    crt_casti = rnd * 8;
                                    crt->cast = NULL;
                                    x4 = myobj->type & 1023;

                                    //summon! KXC
                                    x3 = 0;

                                    if ((x4 == 411) && ((crt_casti & 1) == 1)) x3 = 1; //dragon

                                    if (x3) {
                                        //75% change of unselecting spell
                                        x3 = rnd * 4;
                                        if (x3) x3 = 0;
                                        else x3 = 1;
                                    }

                                    if (x3) {
                                        if (crt->mp >= 8) {
                                            crt->mp -= 8; //mp
                                            x3 = rnd * 3;
                                            x3--;
                                            y3 = rnd * 3;
                                            y3--;
                                            x3 += x;
                                            y3 += y;
                                            if ((bt[y3][x3] >> 10) & 1) {
                                                myobj3 = OBJnew();
                                                myobj3->type = 367; //daemon!
                                                myobj3->more = malloc(sizeof(creature));
                                                ZeroMemory(myobj3->more, sizeof(creature));
                                                crt = (creature *) myobj3->more;
                                                crt->crt_struct = TRUE;
                                                crt->hp = (obji[sprlnk[myobj3->type & 1023]].v8 >> 8) * 4;
                                                crt->mp = rnd * 9;
                                                crt->al = 1;
                                                x4 = rnd * 4;
                                                if (x4 == 0) crt->al = 4; //25% chance of good allegiance!
                                                crt->respawn_x = 1024; //seconds till spawned crt will disappear
                                                myobj9 = OBJnew();
                                                myobj9->type = 188;
                                                crt->items = myobj9; //(not included)bag
                                                myobj3->info |= 4; //<-crt
                                                OBJadd(x3, y3, myobj3);
                                                crt->wait += 1.0f + rnd * 1.0f;
                                                goto donemove;
                                            } //land passable
                                        } //mp
                                    } //x3==1 summon

                                    x3 = 0; //invisibility! SL
                                    if ((x4 == 367) && ((crt_casti & 2) == 2)) x3 = 1; //deamon
                                    if (x4 == 352) x3 = 1; //ghost

                                    if (x3) {
                                        if (crt->mp >= 5) {
                                            crt->mp -= 5; //mp
                                            i3 = SFnew(x, y); //destination is more important than the source
                                            sf[i3].type = 19;
                                            sf[i3].x = x;
                                            sf[i3].y = y;
                                            sf[i3].x2 = x;
                                            sf[i3].y2 = y;
                                            sf[i3].more = 0xFFFF;
                                            sf[i3].wait = 1;
                                            crt->flags |= 2;
                                            crt->wait += 1.0f + rnd * 1.0f;
                                            goto donemove;
                                        } //mp
                                    } //x3==1, invisibility done

                                    //type 2: direct attack spells
                                    if ((abs(x - x2) <= 8) && (abs(y - y2) <= 8)) {
                                        //in spell range!
                                        if (OBJcheckbolt(x, y, x2, y2) == NULL) {
                                            //not blocked
                                            if (stormcloakcheck(x2, y2)) goto crtcantcast;

                                            //paralyze! AXP
                                            x3 = 0;
                                            if ((x4 == 362) && ((crt_casti & 1) == 1)) x3 = 1; //winged gargoyle
                                            if (x3) {
                                                //50% change of unselecting spell
                                                x3 = rnd * 2;
                                                if (x3) x3 = 0;
                                                else x3 = 1;
                                            }

                                            if ((x4 == 378) && ((crt_casti & 2) == 2)) x3 = 1; //mage
                                            if (x3) {
                                                if (target_struct == 2) {
                                                    if (tnpc->wait_disable) goto already_paralyzed;
                                                }

                                                if (crt->mp >= 5) {
                                                    crt->mp -= 5; //mp
                                                    i3 = SFnew(x2, y2); //destination is more important than the source
                                                    sf[i3].type = 14; //blue ball
                                                    sf[i3].x = x;
                                                    sf[i3].y = y;
                                                    sf[i3].x2 = x2;
                                                    sf[i3].y2 = y2;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    x3 = obji[sprlnk[myobj->type & 1023]].v7 & 255;
                                                    x3 /= 4;
                                                    x3 = x3 + (rnd * x3);
                                                    if (target_struct == 2) {
                                                        if (x3) {
                                                            tnpc->wait_disable += x3;
                                                            tnpc->flags |= 8; //paralyzed
                                                            if (tnpc->player) {
                                                                txtset(t, "?");
                                                                t->d2[0] = 8;
                                                                txtadd(t, tnpc->name);
                                                                txtadd(t, " paralyzed!");
                                                                NET_send(NETplayer, tnpc->player->net, t);
                                                            }
                                                        }
                                                    } else {
                                                        tnpc->wait_walk += x3;
                                                    }
                                                    crt->wait += 1.0f + rnd * 1.0f;
                                                    goto donemove;
                                                } //mp
                                            } //x3==1 paralyze
                                        already_paralyzed:

                                            //lightning! OG
                                            x3 = 0;
                                            if ((x4 == 347) && ((crt_casti & 1) == 0)) x3 = 1; //reaper
                                            if ((x4 == 362) && ((crt_casti & 1) == 0)) x3 = 1; //winged gargoyle
                                            if (x4 == 355) x3 = 1; //gazer
                                            if ((x4 == 378) && ((crt_casti & 1) == 0)) x3 = 1; //mage
                                            if ((x4 == 369) && ((crt_casti & 1) == 0)) x3 = 1; //drake
                                            if (x3) {
                                                if (crt->mp >= 5) {
                                                    crt->mp -= 5; //mp
                                                    i3 = SFnew(x2, y2); //destination is more important than the source
                                                    sf[i3].type = 5; //wand
                                                    sf[i3].x = x;
                                                    sf[i3].y = y;
                                                    sf[i3].x2 = x2;
                                                    sf[i3].y2 = y2;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    if (tnpc->lev == 1 && (tnpc->exp < 1600)) {
                                                        goto donemove;
                                                    }
                                                    x3 = rnd * (48);
                                                    //it should do same damage as player lightning spell and armour definelty doesn't reduce the damage it didn't in U6 so why it should in u6o.
                                                    //maybe armour should reduce some damage, but definetly not much.

                                                    //x3=rnd*(16+((obji[sprlnk[myobj->type&1023]].v7&255)/2));
                                                    //x3=rnd*(obji[sprlnk[myobj->type&1023]].v7&255);

                                                    if (target_struct == 2) x5 = rnd * (tnpc->arm + 1);
                                                    else
                                                        x5 = rnd * ((obji[sprlnk[crt->target->type & 1023]].v7 >> 8) +
                                                                    1 + 1 + getequiparmourvalue(
                                                                        (object *) crt2->items->more));
                                                    x3 -= x5 / 4;
                                                    if (x3 < 0) x3 = 0; //npc armour!

                                                    //repel!
                                                    //if (target_struct==2) x5=rnd*tnpc->i; else x5=rnd*(obji[sprlnk[crt->target->type&1023]].v7&255);
                                                    //x6=rnd*(32+4*5);
                                                    //if (x5>=x6) x3=0;
                                                    if (x3) {
                                                        tnpc->hp -= x3;

                                                        if (target_struct == 2) {
                                                            tnpc->upflags |= 2; //hp
                                                        } else {
                                                            //target_struct!=2
                                                            if (tnpc->flags & 4) {
                                                                //wake creature if asleep!
                                                                ((creature *) tnpc)->wait = 0;
                                                                if (tnpc->flags & 8) tnpc->flags -= 8;
                                                                //unparalyze //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                                                if (tnpc->flags & 4) tnpc->flags -= 4;
                                                                //awaken //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                                            } //wake creature if asleep!
                                                        } //target_struct==2

                                                        i3 = SFnew(x2, y2);
                                                        sf[i3].type = 1; //attack
                                                        sf[i3].x2 = x3;
                                                        sf[i3].x = x2;
                                                        sf[i3].y = y2;
                                                        sf[i3].wait = 0.125f; //NULL
                                                        sf[i3].more = 1;
                                                    }
                                                    crt->wait += 1.0f + rnd * 1.0f;
                                                    goto donemove;
                                                } //mp
                                            } //x3==1
                                            //lightning done

                                            //magic arrow! OJ
                                            x3 = 0;
                                            if ((x4 == 347) && ((crt_casti & 1) == 1)) x3 = 1; //reaper
                                            if ((x4 == 367) && ((crt_casti & 1) == 0)) x3 = 1; //deamon
                                            if (x4 == 345) x3 = 1; //squid
                                            if ((x4 == 369) && ((crt_casti & 1) == 0)) x3 = 1; //drake
                                            if (x3) {
                                                if (crt->mp >= 2) {
                                                    crt->mp -= 2; //mp
                                                    i3 = SFnew(x2, y2); //destination is more important than the source
                                                    sf[i3].type = 2;
                                                    sf[i3].x = x;
                                                    sf[i3].y = y;
                                                    sf[i3].x2 = x2;
                                                    sf[i3].y2 = y2;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    if (tnpc->lev == 1 && tnpc->exp < 1600) {
                                                        goto donemove;
                                                    }
                                                    x3 = rnd * 24;
                                                    //x3=rnd*(8+((obji[sprlnk[myobj->type&1023]].v7&255)/4));
                                                    //x3=rnd*((obji[sprlnk[myobj->type&1023]].v7&255)/2);

                                                    if (target_struct == 2) x5 = rnd * (tnpc->arm + 1);
                                                    else
                                                        x5 = rnd * ((obji[sprlnk[crt->target->type & 1023]].v7 >> 8) +
                                                                    1 + 1 + getequiparmourvalue(
                                                                        (object *) crt2->items->more));
                                                    x3 -= x5 / 4;
                                                    if (x3 < 0) x3 = 0; //npc armour!

                                                    //repel!
                                                    //if (target_struct==2) x5=rnd*tnpc->i; else x5=rnd*(obji[sprlnk[crt->target->type&1023]].v7&255);
                                                    //x6=rnd*(32+4*2);
                                                    //if (x5>=x6) x3=0;

                                                    if (x3) {
                                                        tnpc->hp -= x3;
                                                        if (target_struct == 2) tnpc->upflags |= 2; //hp
                                                        i3 = SFnew(x2, y2);
                                                        sf[i3].type = 1; //attack
                                                        sf[i3].x2 = x3;
                                                        sf[i3].x = x2;
                                                        sf[i3].y = y2;
                                                        sf[i3].wait = 0.125f; //NULL
                                                        sf[i3].more = 1;
                                                    }
                                                    crt->wait += 1.0f + rnd * 1.0f;
                                                    goto donemove;
                                                } //mp
                                            } //x3==1
                                            //magic arrow done


                                            x3 = 0; //fireball! PF
                                            if (x4 == 346) x3 = 1; //sea serpant
                                            if ((x4 == 411) && ((crt_casti & 1) == 0)) x3 = 1; //dragon
                                            if ((x4 == 369) && ((crt_casti & 1) == 1)) x3 = 1; //drake
                                            if ((x4 == 367) && ((crt_casti & 1) == 1)) x3 = 1; //deamon
                                            if ((x4 == 378) && ((crt_casti & 1) == 1)) x3 = 1; //mage
                                            if (x3) {
                                                if (crt->mp >= 3) {
                                                    crt->mp -= 3; //mp
                                                    i3 = SFnew(x2, y2); //destination is more important than the source
                                                    sf[i3].type = 8;
                                                    sf[i3].x = x;
                                                    sf[i3].y = y;
                                                    sf[i3].x2 = x2;
                                                    sf[i3].y2 = y2;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;

                                                    if (tnpc->lev == 1 && tnpc->exp < 1600) {
                                                        goto donemove;
                                                    }
                                                    x3 = rnd * 32;
                                                    //x3=rnd*(12+((obji[sprlnk[myobj->type&1023]].v7&255)/3));
                                                    //x3=rnd*((obji[sprlnk[myobj->type&1023]].v7&255)*24/32);

                                                    //txtnumint(t5,tnpc->arm);
                                                    //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                                                    if (target_struct == 2) x5 = rnd * (tnpc->arm + 1);
                                                    else
                                                        x5 = rnd * ((obji[sprlnk[crt->target->type & 1023]].v7 >> 8) +
                                                                    1 + 1 + getequiparmourvalue(
                                                                        (object *) crt2->items->more));
                                                    x3 -= x5 / 4;
                                                    if (x3 < 0) x3 = 0; //npc armour!

                                                    //repel!
                                                    //if (target_struct==2) x5=rnd*tnpc->i; else x5=rnd*(obji[sprlnk[crt->target->type&1023]].v7&255);
                                                    //x6=rnd*(32+4*3);
                                                    //if (x5>=x6) x3=0;

                                                    if (x3) {
                                                        tnpc->hp -= x3;
                                                        if (target_struct == 2) tnpc->upflags |= 2; //hp
                                                        i3 = SFnew(x2, y2);
                                                        sf[i3].type = 1; //attack
                                                        sf[i3].x2 = x3;
                                                        sf[i3].x = x2;
                                                        sf[i3].y = y2;
                                                        sf[i3].wait = 0.125f; //NULL
                                                        sf[i3].more = 1;
                                                    }
                                                    crt->wait += 1.0f + rnd * 1.0f;
                                                    goto donemove;
                                                } //mp
                                            } //x3==1
                                            //fireball done
                                        } //not blocked
                                    } //in spell range
                                } //castnow!
