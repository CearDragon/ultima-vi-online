// LHS-P6.5: part_d_weather_mass -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: wind/weather (windchange_cast) + mass spells (massawaken_*,
// masssleep_*) + spelldone. Move-only.
                            windchange_cast:
                                tplayer->windx = x3;
                                tplayer->windy = y3;
                                txtset(t, "??");
                                t->d2[0] = 14;
                                t->d2[1] = (tplayer->windx + 1) + (tplayer->windy + 1) * 4;
                                NET_send(NETplayer, tplayer->net, t);
                            } else { i2 = 3; }
                            goto spelldone;
                        } //WindChange

                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 8)) {
                            //sleep field txtset(spellname[(3<<4)+8],"SleepField"); spellreagent[(3<<4)+8]=GINS|SPID|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            x3 = 0;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) { x3 = myobj2->type & 1023; }
                            if ((x3 == 317) || (x3 == 318) || (x3 == 320) || (x3 == 173)) { goto spelldone; }
                            if (bt[tpy + y][tpx + x] & 1024) {
                                i2 = 1;
                                x3 = rnd * (8 + 5 * 4);
                                x4 = rnd * (tnpc->i + 1);
                                if (x4 >= x3) {
                                    //success
                                    myobj2 = OBJnew();
                                    myobj2->type = 320; //sleep
                                    myobj2->more2 = tnpc->player->id;
                                    OBJadd(tpx + x, tpy + y, myobj2);
                                    myobj2->info += (2 << 4);
                                    OBJcheckflags(myobj2->x, myobj2->y);
                                } else { i2 = 3; } //spell failed
                            } //location OK
                            goto spelldone;
                        } //sleep field

                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 6)) {
                            //mass cure //txtset(spellname[(3<<4)+6],"MassCure"); spellreagent[(3<<4)+6]=GARL|GINS;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            i9 = SFnew(tpx + x, tpy + y); //destination is more important than the source
                            sf[i9].type = 19; //blue bolt
                            sf[i9].x = myobj->x;
                            sf[i9].y = myobj->y;
                            sf[i9].x2 = tpx + x;
                            sf[i9].y2 = tpy + y;
                            sf[i9].more = 0xFFFF;
                            sf[i9].wait = 1;
                            i2 = 1;
                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    masscureblocked;
                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 2) {
                                                //<-npc
                                                tnpc2 = (npc *) myobj2->more;
                                                if (tnpc2->player) {
                                                    x3 = rnd * (8 + 5 * 1);
                                                    x4 = rnd * (tnpc->i + 10);
                                                    if (x4 >= x3) {
                                                        //success
                                                        tnpc2->flags &= (65535 - 1); //remove poisoned flag
                                                        tnpc2->upflags |= 2;
                                                    }
                                                } //player
                                            } //npc
                                        } //myobj2
                                        if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                            i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                            //destination is more important than the source
                                            sf[i9].type = 14; //blue ball
                                            sf[i9].x = tpx + x;
                                            sf[i9].y = tpy + y;
                                            sf[i9].x2 = tpx + x + spellx;
                                            sf[i9].y2 = tpy + y + spelly;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }
                                    masscureblocked:;
                                    } //4
                                }
                            } //spellx,spelly
                            goto spelldone;
                        } //mass cure


                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 5)) {
                            //mass awaken //txtset(spellname[(2<<4)+5],"MassAwaken"); spellreagent[(2<<4)+5]=GINS|GARL;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            i9 = SFnew(tpx + x, tpy + y); //destination is more important than the source
                            sf[i9].type = 19; //blue bolt
                            sf[i9].x = myobj->x;
                            sf[i9].y = myobj->y;
                            sf[i9].x2 = tpx + x;
                            sf[i9].y2 = tpy + y;
                            sf[i9].more = 0xFFFF;
                            sf[i9].wait = 1;
                            i2 = 1;
                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    massawakenblocked;
                                        if (myobj2) {
                                            if (myobj2->info & 2) {
                                                //<-npc
                                                x3 = rnd * (8 + 5 * 3);
                                                x4 = rnd * (tnpc->i + 10);
                                                if (x4 >= x3) {
                                                    //success
                                                    tnpc2 = (npc *) myobj2->more;
                                                    if (tnpc2->player == NULL) {
                                                        if (tnpc2->schedule_last == 0x91) {
                                                            if (((tpx + x) == 568) && ((tpy + y) == 364))
                                                                goto
                                                                        massawaken_npca;
                                                            if (((tpx + x) == 570) && ((tpy + y) == 364))
                                                                goto
                                                                        massawaken_npca;
                                                            if (((tpx + x) == 572) && ((tpy + y) == 364))
                                                                goto
                                                                        massawaken_npca;
                                                            if (((tpx + x) == 574) && ((tpy + y) == 364))
                                                                goto
                                                                        massawaken_npca;
                                                            tnpc2->schedule_i = -1;
                                                            tnpc2->schedule_last = 0;
                                                            tnpc2->wait_walk += 64; //1mins
                                                        } //sleeping
                                                    } //!player
                                                    if (tnpc2->player) {
                                                        if (tnpc2->flags & 4) {
                                                            tnpc2->flags -= 4;
                                                            tnpc2->wait_disable = 0;
                                                        } //4, sleeping
                                                    } //player
                                                } //fail
                                            } //npc
                                        } //myobj2

                                    massawaken_npca:
                                        if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                            i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                            //destination is more important than the source
                                            sf[i9].type = 14 + 4 * 256; //blue ball
                                            sf[i9].x = tpx + x;
                                            sf[i9].y = tpy + y;
                                            sf[i9].x2 = tpx + x + spellx;
                                            sf[i9].y2 = tpy + y + spelly;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }
                                    massawakenblocked:;
                                    } //4
                                }
                            } //spellx,spelly
                            goto spelldone;
                        } //mass awaken


                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 6)) {
                            //mass sleep //txtset(spellname[(2<<4)+6],"MassSleep"); spellreagent[(2<<4)+6]=GINS|NIGH|SPID;
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            i9 = SFnew(tpx + x, tpy + y); //destination is more important than the source
                            sf[i9].type = 19; //blue bolt
                            sf[i9].x = myobj->x;
                            sf[i9].y = myobj->y;
                            sf[i9].x2 = tpx + x;
                            sf[i9].y2 = tpy + y;
                            sf[i9].more = 0xFFFF;
                            sf[i9].wait = 1;
                            i2 = 1;
                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    masssleepblocked;

                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                crt = (creature *) myobj2->more;
                                                x3 = rnd * (32 + (tnpc->i >> 1));
                                                //immune to sleep?
                                                x4 = myobj2->type & 1023;
                                                if (x4 == 364) goto masssleep_immune; //acid slug
                                                if (x4 == 427) goto masssleep_immune; //ant
                                                if (x4 == 357) goto masssleep_immune; //corpser
                                                if (x4 == 367) goto masssleep_immune; //daemon
                                                if (x4 == 362) goto masssleep_immune; //winged garg
                                                if (x4 == 363) goto masssleep_immune; //garg
                                                if (x4 == 355) goto masssleep_immune; //gazer
                                                if (x4 == 352) goto masssleep_immune; //ghost
                                                if (x4 == 374) goto masssleep_immune; //hydra
                                                if (x4 == 343) goto masssleep_immune; //insects
                                                if (x4 == 347) goto masssleep_immune; //reaper
                                                if (x4 == 360) goto masssleep_immune; //rotworms
                                                if (x4 == 426) goto masssleep_immune; //scorpion
                                                if (x4 == 368) goto masssleep_immune; //skeleton
                                                if (x4 == 375) goto masssleep_immune; //slime
                                                if (x4 == 345) goto masssleep_immune; //squid
                                                if (x4 == 365) goto masssleep_immune; //tanglevine
                                                if (x4 == 373) goto masssleep_immune; //wisp
                                                //NOTE: sleep uses a paralyze effect, but to identify the difference visually (sleeping body) asleep flag is set
                                                crt->wait += x3;
                                                crt->flags |= (4 + 8); //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                                i9 = SFnew(myobj2->x, myobj2->y);
                                                sf[i9].type = 1; //attack
                                                sf[i9].x2 = 0;
                                                sf[i9].x = myobj2->x;
                                                sf[i9].y = myobj2->y;
                                                sf[i9].wait = 0.125f; //NULL
                                                sf[i9].more = 1;
                                            } //crt
                                        } //myobj2
                                    masssleep_immune:
                                        if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                            i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                            //destination is more important than the source
                                            sf[i9].type = 20 + 4 * 256; //pink ball
                                            sf[i9].x = tpx + x;
                                            sf[i9].y = tpy + y;
                                            sf[i9].x2 = tpx + x + spellx;
                                            sf[i9].y2 = tpy + y + spelly;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }
                                    masssleepblocked:;
                                    }
                                }
                            } //4,x,y
                            goto spelldone;
                        } //mass sleep


                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 6)) {
                            //negate magic //txtset(spellname[(5<<4)+6],"NegateMagic"); spellreagent[(5<<4)+6]=GARL|MAND|SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        x3 = rnd * (32 + (tnpc->i >> 1)); //int can only double spell effectiveness!
                                        if (x3) {
                                            if (x3 > 255) x3 = 255; //cap
                                            if (x3 > tnpc2->negatemagic) { tnpc2->negatemagic = x3; }
                                            i2 = 1;
                                        }
                                        if (x3 == 0) i2 = 3;
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //negate magic


                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 4)) {
                            //hail storm //txtset(spellname[(5<<4)+4],"HailStorm"); spellreagent[(5<<4)+4]=BLOO|BLAC|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            x4 = 4;
                            for (x4; x4 > 0; x4--) {
                                x3 = 3 + rnd * (1 + (tnpc->i - 24) / 8);
                                for (x3; x3 > 0; x3--) {
                                    spellx = rnd * (9) - 6;
                                    spelly = rnd * (9) - 6;
                                    if (spellx <= -3) { spellx = rnd * (3) - 1; }
                                    if (spelly <= -3) { spelly = rnd * (3) - 1; }
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    hailstorm_blocked;
                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                crt = (creature *) myobj2->more;
                                                i9 = i3;
                                                i8 = x2; //backup!
                                                x2 = rnd * (16 + (tnpc->i >> 1));
                                                if (x2 == 0) {
                                                    i2 = 1;
                                                    x2 = i8;
                                                    i3 = i9;
                                                    goto hailstorm_spelldone;
                                                }
                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                sf[i3].type = 1; //attack
                                                sf[i3].x2 = x2;
                                                sf[i3].x = myobj2->x;
                                                sf[i3].y = myobj2->y;
                                                sf[i3].wait = 0.125f; //NULL
                                                sf[i3].more = 1;
                                                spellattcrt = TRUE;
                                                goto spellattcrt0;
                                            } //crt
                                        } //myobj2
                                    hailstorm_spelldone:;
                                        x5 = x4 / 2;
                                        i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                        //destination is more important than the source
                                        sf[i9].type = 13 + x5 * 2 * 256; //sling bullet
                                        sf[i9].x = tpx + x + spellx - 2;
                                        sf[i9].y = tpy + y + spelly - 1 - x4;
                                        sf[i9].x2 = tpx + x + spellx;
                                        sf[i9].y2 = tpy + y + spelly;
                                        sf[i9].more = 0xFFFF;
                                        sf[i9].wait = 1;
                                    hailstorm_blocked:;
                                    } //!=4
                                } //x3
                            } //x4
                            i2 = 1;

                            goto spelldone;
                        } //hail storm


                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 8)) {
                            //ignite //txtset(spellname[(0<<4)+8],"Ignite"); spellreagent[(0<<4)+8]=SULF|BLAC;
                            i2 = spell_ignite(tplayer, tnpc, NULL, tpx + x, tpy + y);
                            goto spelldone;
                        } //ignite


                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 4)) {
                            //douse //txtset(spellname[(0<<4)+4],"Douse"); spellreagent[(0<<4)+4]=GARL|BLAC;
                            i2 = spell_douse(tplayer, tnpc, NULL, tpx + x, tpy + y);
                            goto spelldone;
                        } //douse


                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 7)) {
                            //reveal //txtset(spellname[(4<<4)+7],"Reveal"); spellreagent[(4<<4)+7]=SPID|NIGH|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            i2 = 1;
                            x3 = rnd * (8 + 5 * 5);
                            x4 = rnd * (tnpc->i + 1);
                            if (x4 >= x3) {
                                i2 = 1;
                                for (spellx = -7; spellx <= 7; spellx++) {
                                    for (spelly = -7; spelly <= 7; spelly++) {
                                        if ((abs(spellx) + abs(spelly)) <= 10) {
                                            myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                            if (myobj2 == NULL)
                                                myobj2 = OBJfindlastall(tpx + x + spellx,
                                                                        tpy + y + spelly);
                                            if (myobj2) {
                                                if (myobj2->info & 4) {
                                                    //<-crt
                                                    crt = (creature *) myobj2->more;
                                                    if (crt->flags & 2) {
                                                        crt->flags -= 2; //if creature invisible make visible.
                                                    } //flags
                                                } //crt
                                            } //myobj2
                                        }
                                    }
                                } //spellx,y
                            } //success
                            else { i2 = 3; } //fail
                        } //reveal

                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 9)) {
                            //web //txtset(spellname[(5<<4)+9],"Web"); spellreagent[(5<<4)+9]=SPID;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            x3 = rnd * (8 + 5 * 6);
                            x4 = rnd * (tnpc->i + 100);
                            if (x4 >= x3) {
                                //success
                                i2 = 1;
                                i9 = SFnew(tpx + x, tpy + y); //destination is more important than the source
                                sf[i9].type = 9; //generic throw object
                                sf[i9].x = myobj->x;
                                sf[i9].y = myobj->y;
                                sf[i9].x2 = tpx + x;
                                sf[i9].y2 = tpy + y;
                                sf[i9].more = 0xFFFF;
                                sf[i9].wait = 53;
                                for (spellx = -1; spellx <= 1; spellx++) {
                                    for (spelly = -1; spelly <= 1; spelly++) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    web_blocked;
                                        if ((x3 == 317) || (x3 == 318) || (x3 == 320) || (x3 == 173) || (x3 == 53)) {
                                            goto web_blocked;
                                        }
                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (bt[tpy + y + spelly][tpx + x + spellx] & 1024) {
                                            myobj2 = OBJnew();
                                            myobj2->type = 53; //web
                                            myobj2->more2 = tnpc->player->id;
                                            myobj2->info |= 32;
                                            //if tile has an object, place web under it 
                                            OBJtmp = od[tpy + y + spelly][tpx + x + spellx];
                                            if (OBJtmp) {
                                                OBJtmp->prev = myobj2;
                                                myobj2->next = OBJtmp;
                                                myobj2->x = tpx + x + spellx;
                                                myobj2->y = tpy + y + spelly;
                                                od[tpy + y + spelly][tpx + x + spellx] = myobj2;
                                            } else {
                                                OBJadd(tpx + x + spellx, tpy + y + spelly, myobj2);
                                            }
                                        } else if (myobj2 &&myobj2
                                        
                                        ->
                                        info & 4
                                        )
                                        {
                                            crt = (creature *) myobj2->more;
                                            if ((myobj2->type & 1023) != 361) {
                                                x3 = rnd * (32 + (tnpc->i >> 1) / 2);
                                                crt->wait += x3;
                                                crt->flags |= 8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                            }
                                            myobj2 = OBJnew();
                                            myobj2->type = 53; //web
                                            myobj2->more2 = tnpc->player->id;
                                            myobj2->info |= 112; //set flags as temp OBJ
                                            //if tile has an object, place web under it 
                                            OBJtmp = od[tpy + y + spelly][tpx + x + spellx];
                                            if (OBJtmp) {
                                                OBJtmp->prev = myobj2;
                                                myobj2->next = OBJtmp;
                                                od[tpy + y + spelly][tpx + x + spellx] = myobj2;
                                            } else {
                                                OBJadd(tpx + x + spellx, tpy + y + spelly, myobj2);
                                            }
                                        } //location OK
                                    web_blocked:;
                                    }
                                } //spellx,spelly
                            } else { i2 = 3; } //spell failed
                        } //web


                        //NEWCODEEND


                    spelldone:

                        if (i2 == 0) {
                            //not implemented!
                            txtset(t, "?");
                            t->d2[0] = 8;
                            txtadd(t, "U6O can't cast this spell yet!");
                            NET_send(NETplayer, tplayer->net, t);
                        }

                        //4. display words of power if spell was a success


                        //5. remove mp&reagents if successful
                        if ((i2 == 1) || (i2 == 3)) {
                            if (!staff_cast_spell) {
                                //subtract mp
                                tnpc->mp -= ((CASTSPELL_SPELLTYPE >> 4) + 1);
                                tnpc->upflags |= 4;
                                //remove reagents
                                x4 = spellreagent[CASTSPELL_SPELLTYPE];
                                for (x2 = 0; x2 < CASTSPELL_ENUMERATEDINVENTORYOBJS; x2++) {
                                    x3 = OBJlist_list[x2]->type; //gpf
                                    if ((x3 >= 65) && (x3 <= 72)) {
                                        x3 -= 65;
                                        if (x4 & (1 << x3)) {
                                            x4 -= (1 << x3);
                                            OBJlist_list[x2]->more2--;
                                            if (OBJlist_list[x2]->more2 == 0) OBJremove(OBJlist_list[x2]);
                                        } //x4
                                    } //x3
                                } //x2
                                tnpc->upflags |= 32;
                            } //!staff_cast_spell

                        spelldone_donttakereagents:

                            if (staff_cast_spell) {
                                x3 = tplayer->staffcast_staff->more2 >> 16;
                                x3--;
                                if (x3) {
                                    tplayer->staffcast_staff->more2 &= 65535;
                                    tplayer->staffcast_staff->more2 |= (x3 << 16);
                                } else tplayer->staffcast_staff->more2 = 0;
                                //all spells used, revert to unclassified staff
                            }

                            //spell delay
                            tnpc->wait_cast += 4.0f - ((float) tnpc->i / 32.0f);
                            //txtset(t,"??"); t->d2[0]=18; t->d2[1]=iparty; NET_send(NETplayer,tplayer->net,t); //set cast icon!
                        }


                        if (i2 == 2) {
                            //invalid target
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, "Can't cast there!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                        }

                        if (i2 == 3) {
                            //spell failed!
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, "Spell failed!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                        }


                    castfailed:;
                        if (cast_spell) {
                            if (cast_spell == 1) {
                                cast_spell = 0;
                                goto cast_spell1;
                            }
                            if (cast_spell == 2) {
                                cast_spell = 0;
                                goto cast_spell2;
                            }
                        }

                        goto skip_pickup;
                    } //i
                } //mbclick


                //use and other stuff
                if (tplayer->mobj != NULL) {
                    //drop item
                    i = tplayer->mf;
                    if (i <= 7) {
                        if (tplayer->party[i]) {
                            if (tplayer->key & KEYmbclick) {
                                tnpc = (npc *) tplayer->party[i]->more; //*tnpc


                                //scroll inventory down
                                if ((tplayer->mx >= (111 - 3)) && (tplayer->my >= (228 - 3))) {
                                    if ((tplayer->mx <= (121 + 3)) && (tplayer->my <= (243 + 3))) {
                                        tnpc->upflags |= 32;
                                        //is scroll down available?
                                        myobj = (object *) tnpc->baseitem;
                                        myobj = (object *) myobj->more;
                                        x = 61;
                                        y = 61;
                                        x3 = tnpc->baseitem_offset;
                                    dniz2:
                                        if (myobj != NULL) {
                                            if (x3 > 0) {
                                                x3--;
                                                goto skipbio1z2;
                                            }
                                            x = x + 16;
                                            if (x > 109) {
                                                x = 61;
                                                y = y + 16;
                                            }
                                            if (y > 109) {
                                                if (myobj->next != NULL) {
                                                    tnpc->baseitem_offset += 4;
                                                }
                                                goto diskipz2;
                                            }
                                            if (myobj->next != NULL) {
                                            skipbio1z2:
                                                myobj = (object *) myobj->next;
                                                goto dniz2;
                                            }
                                        }
                                    diskipz2:;
                                        goto skip_pickup;
                                    }
                                }

                                //scroll inventory up
                                if ((tplayer->mx >= (111 - 3)) && (tplayer->my >= (197 - 3))) {
                                    if ((tplayer->mx <= (121 + 3)) && (tplayer->my <= (212 + 3))) {
                                        if (tnpc->baseitem_offset >= 4) { tnpc->baseitem_offset -= 4; } else {
                                            tnpc->baseitem_offset = 0;
                                        }
                                        tnpc->upflags |= 32;
                                        goto skip_pickup;
                                    }
                                }


                                //clicked on opened "bag" icon ->close bag
                                if ((tplayer->mx >= (90)) && (tplayer->my >= (128 + 3)) && (tplayer->mx < ((90) + 32))
                                    && (tplayer->my < ((128 + 3) + 32))) {
                                    if (tnpc->baseitem != tnpc->items) {
                                        //backwards traverse
                                        myobj2 = tnpc->baseitem;
                                        myobj = (object *) tnpc->baseitem->prev;
                                    getprev9:
                                        if ((myobj->more != myobj2) || (
                                                (obji[sprlnk[myobj->type & 1023]].flags & 1024) == 0)) {
                                            myobj2 = myobj;
                                            myobj = (object *) myobj->prev;
                                            goto getprev9;
                                        }
                                        tnpc->baseitem = myobj;
                                        tnpc->baseitem_offset = 0;
                                        tnpc->upflags |= 32;
                                        goto skip_pickup;
                                    } //tnpc->baseitem!=tnpc->items
                                } //x,y

                                if (tplayer->action == 2) {
                                    //look, opens inventory container items 2004
                                    tnpc->upflags |= 32;
                                    if ((tplayer->mx >= 122) && (tplayer->my >= 122)) {
                                        x = (tplayer->mx - 122) / 32;
                                        y = (tplayer->my - 122) / 32;
                                        i2 = x + (y * 4);
                                        myobj = tnpc->baseitem;
                                        myobj = (object *) myobj->more;
                                        i3 = tnpc->baseitem_offset;
                                    fo2c:
                                        if (i3 > 0) {
                                            i3--;
                                            myobj = (object *) myobj->next;
                                            goto fo2c;
                                        }
                                        if (myobj != NULL) {
                                            if (i2 > 0) {
                                                if (myobj->next != NULL) {
                                                    myobj = (object *) myobj->next;
                                                    i2--;
                                                    goto fo2c;
                                                } else { myobj = NULL; }
                                            }
                                        }
                                        if (myobj != NULL) {
                                            if ((myobj->type == (OBJ_CHEST + 1024 * 2)) || (
                                                    myobj->type == (OBJ_CHEST + 1024 * 3)))
                                                goto inventory_look;
                                            //magically/locked chests

                                            for (i3 = 0; i3 <= nresu; i3++) {
                                                if (resu[i3]) {
                                                    if (resu_body[i3] == myobj) {
                                                        goto inventory_look; //block looking inside resurrectable bodies
                                                    }
                                                }
                                            }

                                            if (obji[sprlnk[myobj->type & 1023]].flags & 1024) {
                                                //2003: check container flag
                                                tnpc->baseitem = myobj;
                                                tnpc->baseitem_offset = 0;
                                                goto skip_pickup;
                                            }
                                            goto inventory_look;
                                        } //myobj!=NULL
                                        goto skip_pickup;
                                    } //if x&y

                                    //what about equipped items?
                                    x2 = tplayer->mx;
                                    y2 = tplayer->my;
                                    x3 = helmx;
                                    y3 = helmy;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->helm) goto inventory_look;
                                    }
                                    x3 = wep_rightx;
                                    y3 = wep_righty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->wep_right) goto inventory_look;
                                    }
                                    x3 = wep_leftx;
                                    y3 = wep_lefty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->wep_left) goto inventory_look;
                                    }
                                    x3 = armourx;
                                    y3 = armoury;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->armour) goto inventory_look;
                                    }
                                    x3 = bootsx;
                                    y3 = bootsy;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->boots) goto inventory_look;
                                    }
                                    x3 = neckx;
                                    y3 = necky;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->neck) goto inventory_look;
                                    }
                                    x3 = ring_leftx;
                                    y3 = ring_lefty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->ring_left) goto inventory_look;
                                    }
                                    x3 = ring_rightx;
                                    y3 = ring_righty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (myobj = tnpc->ring_right) goto inventory_look;
                                    }

                                    goto skip_pickup;
                                } //action==2


                                //tnpc->update=1;

                                if (tplayer->my < 122) goto port_drop;
                                if (tplayer->mx >= 122) {
                                    if (tplayer->my >= 122) {
                                    port_drop:
                                    sprite_drop:

                                        if (tplayer->mobj->info & 256) goto questitem_skipweightcheck;
                                        //NEWCODE
                                        if (U6O_DEBUG) {
                                            goto questitem_skipweightcheck; //for debugging
                                        }
                                        //NEWCODEEND
                                        if ((tnpc->wt + WTfind(tplayer->mobj)) <= tnpc->wt_max) {
                                        questitem_skipweightcheck:


                                            if (tnpc->baseitem->type == 57) {
                                                if (tplayer->mobj->type != 58) {
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "Only spells can go into the spellbook!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                }
                                            }

                                            if (tnpc->baseitem->type == 62) {
                                                //vortex cube
                                                if ((tplayer->mobj->type & 1023) != 73) {
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "Only moonstones can go into the vortex cube!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                }
                                            }

                                            if (tplayer->mobj->info & 256) {
                                                //drop quest item
                                                if ((i != 0) || ((tnpc->baseitem->info & 256) == 0)) {
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "You cannot put a quest item here!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                } //not primary player OR not quest bag
                                            }


                                            if ((tplayer->mobj->type & 1023) == 414) {
                                                //skiff
                                                if (tnpc->baseitem != tnpc->items) {
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "You cannot put a skiff in there!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                }
                                            } //414

                                            if (tplayer->mobj->type == 149) {
                                                //deed
                                                if (tnpc->baseitem != tnpc->items) {
                                                    if ((tnpc->baseitem->info & 256) == 0) {
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(t, "You cannot put a deed in there!");
                                                        NET_send(NETplayer, tplayer->net, t);
                                                        goto skip_pickup;
                                                    }
                                                }
                                                if (i) {
                                                    //not the primary player
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, tnpc->name);
                                                    txtadd(t, " cannot be given a deed!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                }
                                                //if (tnpc->converse!=201){ if (tnpc->converse!=0){
                                                //txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot be given a deed!"); NET_send(NETplayer,tplayer->net,t);
                                                //goto skip_pickup;
                                                //}}
                                            } //149

                                            if (tplayer->mobj->type == 448) {
                                                //horse papers
                                                if (tnpc->baseitem != tnpc->items) {
                                                    if ((tnpc->baseitem->info & 256) == 0) {
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(t, "You cannot put the horse papers in there!");
                                                        NET_send(NETplayer, tplayer->net, t);
                                                        goto skip_pickup;
                                                    }
                                                }
                                                if (i) {
                                                    //not the primary player
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, tnpc->name);
                                                    txtadd(t, " cannot carry horse papers!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                }
                                                //if (tnpc->converse!=201){ if (tnpc->converse!=0){
                                                //txtset(t,"?"); t->d2[0]=8; txtadd(t,tnpc->name); txtadd(t," cannot carry horse papers!"); NET_send(NETplayer,tplayer->net,t);
                                                //goto skip_pickup;
                                                //}}
                                            } //448


                                            //check that current container is NOT a dead monster (they are not to be used as containers)
                                            if (((tnpc->baseitem->type & 1023) > 432) && (
                                                    (tnpc->baseitem->type & 1023) <= 447)) {
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, "You cannot put items in here!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            }


                                            tnpc->upflags |= 32; //inv
                                            stealing(tplayer, tplayer->mobj);


                                            myobj = tnpc->baseitem;
                                            myobj2 = (object *) myobj->more;


                                            if (myobj2 != NULL) {
                                                //scan for multiple item
                                                if (obji[sprlnk[tplayer->mobj->type & 1023] + (
                                                             tplayer->mobj->type >> 10)].flags & 4096) {
                                                    //multiple
                                                    myobj3 = myobj2;
