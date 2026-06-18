// LHS-P6.3: part_d_chainbolt -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: chainbolt + spell-attack (chainbolt_*, spellattcrt1). Move-only.
                                chainbolt_gotnexttarget:
                                    chainbolt_miss = 0;
                                    crt = (creature *) myobj2->more;
                                    i9 = i3;
                                    i8 = x2; //backup!
                                    x2 = rnd * (64 + tnpc->i * 2);
                                    if (x2 == 0) {
                                        x2 = i8;
                                        i3 = i9;
                                        goto chainbolt_spelldone;
                                    }
                                    i2 = 1;
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 5 + chainbolt_hits * 2 * 256;
                                    if (chainbolt_hits) {
                                        sf[i3].x = spellx2;
                                        sf[i3].y = spelly2;
                                    } else {
                                        sf[i3].x = myobj->x;
                                        sf[i3].y = myobj->y;
                                    }
                                    sf[i3].x2 = myobj2->x;
                                    sf[i3].y2 = myobj2->y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = 1;
                                    i3 = SFnew(myobj2->x, myobj2->y);
                                    sf[i3].type = 1 + chainbolt_hits * 2 * 256; //attack
                                    sf[i3].x2 = x2;
                                    sf[i3].x = myobj2->x;
                                    sf[i3].y = myobj2->y;
                                    sf[i3].wait = 0.125f; //NULL
                                    sf[i3].more = 1;
                                    spellattcrt = TRUE;
                                    goto spellattcrt0;
                                chainbolt_spelldone:
                                    if (chainbolt_hits <= 7) {
                                        //random scanning
                                        spellx2 = ((long(rnd * 17) - 8) + (long(rnd * 17) - 8)) / 2;
                                        spelly2 = ((long(rnd * 17) - 8) + (long(rnd * 17) - 8)) / 2;
                                        if (spellx2 && spelly2) {
                                            //both not 0
                                            spellx2 += spellx;
                                            spelly2 += spelly;
                                            if (spellx2 >= 0) {
                                                if (spelly2 >= 0) {
                                                    if (spellx2 < 2048) {
                                                        if (spelly2 < 1024) {
                                                            //map boundries check
                                                            if (!stormcloakcheck2(spellx2, spelly2, tplayer)) {
                                                                if (!OBJcheckbolt(spellx, spelly, spellx2, spelly2)) {
                                                                    myobj2 = OBJfindlast(spellx2, spelly2);
                                                                    if (myobj2 == NULL)
                                                                        myobj2 = OBJfindlastall(
                                                                            spellx2, spelly2);
                                                                    if (myobj2) {
                                                                        if (myobj2->info & 4) {
                                                                            //<-crt
                                                                            chainbolt_hits++;
                                                                            i9 = spellx;
                                                                            spellx = spellx2;
                                                                            spellx2 = i9;
                                                                            i9 = spelly;
                                                                            spelly = spelly2;
                                                                            spelly2 = i9;
                                                                            goto chainbolt_gotnexttarget;
                                                                        } //crt
                                                                    } //myobj2
                                                                } //checkbolt
                                                            } //stormcloakcheck
                                                        }
                                                    }
                                                }
                                            } //boundries
                                        } //both not 0
                                        chainbolt_miss++;
                                        i9 = 128 - chainbolt_hits * 16 + tnpc->i * 2;
                                        if (chainbolt_miss < i9) goto chainbolt_spelldone;
                                    } //chainbolthits
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //chainbolt

                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 8)) {
                            //tremor //txtset(spellname[(7<<4)+8],"Tremor"); spellreagent[(7<<4)+8]=BLOO|SULF|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            i2 = 1;
                            for (spelly2 = tpy + y - 23; spelly2 <= tpy + y + 23; spelly2++) {
                                for (spellx2 = tpx + x - 31; spellx2 <= tpx + x + 31; spellx2++) {
                                    myobj2 = OBJfindlast(spellx2, spelly2);
                                    if (myobj2 == NULL) myobj2 = OBJfindlastall(spellx2, spelly2);
                                    if (myobj2) {
                                        if (myobj2->info & 4) {
                                            //<-crt
                                            if (spellx2 >= 0) {
                                                if (spelly2 >= 0) {
                                                    if (spellx2 < 2048) {
                                                        if (spelly2 < 1024) {
                                                            //map boundries check
                                                            if (!stormcloakcheck2(spellx2, spelly2, tplayer)) {
                                                                crt = (creature *) myobj2->more;
                                                                i9 = i3;
                                                                i8 = x2; //backup!
                                                                x3 = abs(spellx2 - (tpx + x));
                                                                x4 = abs(spelly2 - (tpy + y));
                                                                if (x4 > x3) x3 = x4;
                                                                x3 *= 3;
                                                                x2 = rnd * (80 + tnpc->i + (tnpc->i >> 1));
                                                                x2 -= x3;
                                                                if (x2 <= 0) {
                                                                    x2 = i8;
                                                                    i3 = i9;
                                                                    goto tremor_spelldone;
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
                                                            } //stormcloakcheck
                                                        }
                                                    }
                                                }
                                            } //boundries
                                        }
                                    } //myobj2,crt
                                tremor_spelldone:;
                                }
                            }
                            i9 = SFnew(tpx + x, tpy + y);
                            sf[i9].type = 26; //tremor
                            sf[i9].wait = 1;
                            goto spelldone;
                        } //tremor


                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 1)) {
                            //explosion //txtset(spellname[(4<<4)+1],"Explosion"); spellreagent[(4<<4)+1]=MAND|SULF|BLAC|BLOO;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;

                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    explosionblocked;

                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt

                                                crt = (creature *) myobj2->more;
                                                i9 = i3;
                                                i8 = x2; //backup!
                                                x2 = rnd * (48 + tnpc->i + (tnpc->i >> 1));
                                                if (x2 == 0) {
                                                    i2 = 1;
                                                    x2 = i8;
                                                    i3 = i9;
                                                    goto explosion_spelldone;
                                                }
                                                //fire resistant???
                                                i3 = myobj2->type & 1023;
                                                if (i3 == 411) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //dragon
                                                if (i3 == 369) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //drake
                                                if (i3 == 352) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //ghost
                                                if (i3 == 374) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //hydra
                                                if (i3 == 369) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //drake
                                                if (i3 == 368) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //skeleton
                                                if (i3 == 373) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
                                                } //wisp
                                                if (i3 == 367) {
                                                    x2 = 0;
                                                    goto IFimmuneflame2;
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

                                            IFimmuneflame2:
                                                spellattcrt = TRUE;
                                                goto spellattcrt0;
                                            } //crt
                                        } //myobj2

                                    explosion_spelldone:;

                                        if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                            i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                            //destination is more important than the source
                                            sf[i9].type = 8; //wand
                                            sf[i9].x = tpx + x;
                                            sf[i9].y = tpy + y;
                                            sf[i9].x2 = tpx + x + spellx;
                                            sf[i9].y2 = tpy + y + spelly;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }

                                    explosionblocked:;
                                    } //!=4
                                }
                            } //spell x,y
                            i2 = 1;

                            goto spelldone;
                        } //explosion


                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 2)) {
                            //fireball //txtset(spellname[(2<<4)+2],"Fireball"); spellreagent[(2<<4)+2]=SULF|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    i9 = i3;
                                    i8 = x2; //backup!
                                    x2 = rnd * (48 + tnpc->i + (tnpc->i >> 1));
                                    if (x2 == 0) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    }
                                    //fire resistant???
                                    i3 = myobj2->type & 1023;
                                    if (i3 == 411) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //dragon
                                    if (i3 == 369) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //drake
                                    if (i3 == 352) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //ghost
                                    if (i3 == 374) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //hydra
                                    if (i3 == 369) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //drake
                                    if (i3 == 368) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //skeleton
                                    if (i3 == 373) {
                                        x2 = 0;
                                        goto IFimmuneflame;
                                    } //wisp
                                    if (i3 == 367) {
                                        x2 = 0;
                                        goto IFimmuneflame;
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

                                IFimmuneflame:
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 8; //wand
                                    sf[i3].x = myobj->x;
                                    sf[i3].y = myobj->y;
                                    sf[i3].x2 = myobj2->x;
                                    sf[i3].y2 = myobj2->y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = 1;
                                    spellattcrt = TRUE;
                                    goto spellattcrt0;
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //fireball


                        if (CASTSPELL_SPELLTYPE == ((1 << 4) + 1)) {
                            //magic arrow //txtset(spellname[(1<<4)+1],"MagicArrow"); spellreagent[(1<<4)+1]=SULF|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    i9 = i3;
                                    i8 = x2; //backup!
                                    x2 = rnd * (32 + tnpc->i);
                                    if (x2 == 0) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    }
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 2; //arrow
                                    sf[i3].x = myobj->x;
                                    sf[i3].y = myobj->y;
                                    sf[i3].x2 = myobj2->x;
                                    sf[i3].y2 = myobj2->y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = 1;
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
                            goto spelldone;
                        } //magic arrow

                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 0)) {
                            //charm //txtset(spellname[(5<<4)+0],"Charm"); spellreagent[(5<<4)+0]=BLAC|NIGH|SPID; //level 6
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    if (crt->al == 1) {
                                        x3 = rnd * (128 + (tnpc->i * 2));
                                        if (x3 > 255) x3 = 255;
                                        if (x3 == 0) {
                                            i2 = 3;
                                            goto spelldone;
                                        }
                                        i2 = 1;
                                        crt->al = 4;
                                        crt->target = NULL;
                                        crt->flags |= 32;
                                        if (x3 > crt->charm) crt->charm = x3;
                                        i9 = SFnew(myobj2->x, myobj2->y);
                                        //destination is more important than the source
                                        sf[i9].type = 14; //blue ball
                                        sf[i9].x = myobj->x;
                                        sf[i9].y = myobj->y;
                                        sf[i9].x2 = myobj2->x;
                                        sf[i9].y2 = myobj2->y;
                                        sf[i9].more = 0xFFFF;
                                        sf[i9].wait = 1;
                                    } //al==1
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //charm

                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 2)) {
                            //mass charm //txtset(spellname[(7<<4)+2],"MassCharm"); spellreagent[(7<<4)+2]=BLAC|NIGH|SPID|MAND;
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            i2 = 1;
                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    masscharmblocked;

                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                crt = (creature *) myobj2->more;
                                                if (crt->al == 1) {
                                                    x3 = rnd * (128 + (tnpc->i * 2));
                                                    if (x3 > 255) x3 = 255;
                                                    if (x3) {
                                                        crt->al = 4;
                                                        crt->target = NULL;
                                                        crt->flags |= 32;
                                                        if (x3 > crt->charm) crt->charm = x3;
                                                    } //x3
                                                } //al==1
                                            } //crt
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
                                    masscharmblocked:;
                                    }
                                }
                            } //4,x,y
                            goto spelldone;
                        } //mass charm

                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 5)) {
                            //harm //txtset(spellname[(0<<4)+5],"Harm"); spellreagent[(0<<4)+5]=NIGH|SPID;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    i9 = i3;
                                    i8 = x2; //backup!
                                    x2 = rnd * (24 + (tnpc->i >> 1)); //32
                                    if (x2 == 0) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    }
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 14; //blue ball
                                    sf[i3].x = myobj->x;
                                    sf[i3].y = myobj->y;
                                    sf[i3].x2 = myobj2->x;
                                    sf[i3].y2 = myobj2->y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = 1;
                                    i3 = SFnew(myobj2->x, myobj2->y);
                                    sf[i3].type = 1; //attack
                                    sf[i3].x2 = x2;
                                    sf[i3].x = myobj2->x;
                                    sf[i3].y = myobj2->y;
                                    sf[i3].wait = 0.125f; //NULL
                                    sf[i3].more = 1;
                                    spellattcrt = TRUE;
                                    goto spellattcrt0;

                                    //return branch for varied spells
                                spellattcrt1:
                                    i2 = 1;
                                    x2 = i8;
                                    i3 = i9; //<-
                                    if (CASTSPELL_SPELLTYPE == ((4 << 4) + 1)) goto explosion_spelldone;
                                    if (CASTSPELL_SPELLTYPE == ((7 << 4) + 3)) goto masskill_spelldone;
                                    if (CASTSPELL_SPELLTYPE == ((5 << 4) + 3)) goto flamewind_spelldone;
                                    if (CASTSPELL_SPELLTYPE == ((6 << 4) + 2)) goto energywind_spelldone;
                                    if (CASTSPELL_SPELLTYPE == ((7 << 4) + 0)) goto deathwind_spelldone;
                                    if (CASTSPELL_SPELLTYPE == ((6 << 4) + 0)) goto chainbolt_spelldone;
                                    if (CASTSPELL_SPELLTYPE == ((7 << 4) + 8)) goto tremor_spelldone;
                                    if (CASTSPELL_SPELLTYPE == 65535) goto explosion_spelldone2;
                                    //NEWCODE
                                    if (CASTSPELL_SPELLTYPE == ((5 << 4) + 4)) goto hailstorm_spelldone;
                                    //NEWCODEEND
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //harm

                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 7)) {
                            //time stop //txtset(spellname[(7<<4)+7],"TimeStop"); spellreagent[(7<<4)+7]=MAND|GARL|BLOO;
                            //creates a localized vortex, strongest at the centre
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            x3 = rnd * (64 + tnpc->i);
                            if (x3 == 0) {
                                i2 = 3;
                                goto spelldone;
                            }
                            for (spellx = -32; spellx <= 32; spellx++) {
                                for (spelly = -32; spelly <= 32; spelly++) {
                                    if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                        goto
                                                timestopblocked;

                                    myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                    if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                    if (myobj2) {
                                        if (myobj2->info & 4) {
                                            //<-crt
                                            crt = (creature *) myobj2->more;
                                            f = (float) x3 / 32.0f;
                                            if (abs(spellx) > abs(spelly)) f2 = abs(spellx);
                                            else f2 = abs(spelly);
                                            crt->wait += ((float) x3 - f2 * f);
                                            crt->flags |= 8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                            i9 = SFnew(myobj2->x, myobj2->y);
                                            sf[i9].type = 1; //attack
                                            sf[i9].x2 = 0;
                                            sf[i9].x = myobj2->x;
                                            sf[i9].y = myobj2->y;
                                            sf[i9].wait = 0.125f; //NULL
                                            sf[i9].more = 1;
                                        } //crt
                                    } //myobj2
                                timestopblocked:;
                                }
                            } //spellx,spelly
                            i2 = 1;
                            goto spelldone;
                        } //time stop

                        if (CASTSPELL_SPELLTYPE == ((1 << 4) + 4)) {
                            //sleep //txtset(spellname[(1<<4)+4],"Sleep"); spellreagent[(1<<4)+4]=NIGH|SPID|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    x3 = rnd * (32 + (tnpc->i >> 1));
                                    if (x3 == 0) {
                                        i2 = 3;
                                        goto spelldone;
                                    }
                                    i2 = 1;
                                    i9 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i9].type = 20; //pink ball
                                    sf[i9].x = myobj->x;
                                    sf[i9].y = myobj->y;
                                    sf[i9].x2 = myobj2->x;
                                    sf[i9].y2 = myobj2->y;
                                    sf[i9].more = 0xFFFF;
                                    sf[i9].wait = 1;
                                    //immune to sleep?
                                    x4 = myobj2->type & 1023;
                                    if (x4 == 364) goto spelldone; //acid slug
                                    if (x4 == 427) goto spelldone; //ant
                                    if (x4 == 357) goto spelldone; //corpser
                                    if (x4 == 367) goto spelldone; //daemon
                                    if (x4 == 362) goto spelldone; //winged garg
                                    if (x4 == 363) goto spelldone; //garg
                                    if (x4 == 355) goto spelldone; //gazer
                                    if (x4 == 352) goto spelldone; //ghost
                                    if (x4 == 374) goto spelldone; //hydra
                                    if (x4 == 343) goto spelldone; //insects
                                    if (x4 == 347) goto spelldone; //reaper
                                    if (x4 == 360) goto spelldone; //rotworms
                                    if (x4 == 426) goto spelldone; //scorpion
                                    if (x4 == 368) goto spelldone; //skeleton
                                    if (x4 == 375) goto spelldone; //slime
                                    if (x4 == 345) goto spelldone; //squid
                                    if (x4 == 365) goto spelldone; //tanglevine
                                    if (x4 == 373) goto spelldone; //wisp
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
                            goto spelldone;
                        } //sleep

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 5)) {
                            //paralyze //txtset(spellname[(4<<4)+5],"Paralyze"); spellreagent[(4<<4)+5]=SPID|SULF|NIGH|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    x3 = rnd * (32 + (tnpc->i >> 1));
                                    if (x3 == 0) {
                                        i2 = 3;
                                        goto spelldone;
                                    }
                                    crt->wait += x3;
                                    crt->flags |= 8; //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                    i9 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i9].type = 14; //blue ball
                                    sf[i9].x = myobj->x;
                                    sf[i9].y = myobj->y;
                                    sf[i9].x2 = myobj2->x;
                                    sf[i9].y2 = myobj2->y;
                                    sf[i9].more = 0xFFFF;
                                    sf[i9].wait = 1;
                                    i9 = SFnew(myobj2->x, myobj2->y);
                                    sf[i9].type = 1; //attack
                                    sf[i9].x2 = 0;
                                    sf[i9].x = myobj2->x;
                                    sf[i9].y = myobj2->y;
                                    sf[i9].wait = 0.125f; //NULL
                                    sf[i9].more = 1;
                                    i2 = 1;
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //paralyze

                        if (CASTSPELL_SPELLTYPE == ((1 << 4) + 2)) {
                            //poison //txtset(spellname[(1<<4)+2],"Poison"); spellreagent[(1<<4)+2]=NIGH|BLOO|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    crt = (creature *) myobj2->more;
                                    i9 = i3;
                                    i8 = x2; //backup!
                                    x3 = rnd * (8 + 5 * 2);
                                    x4 = rnd * (tnpc->i + 1);
                                    if (x4 >= x3) {
                                        //success
                                        crt->flags |= 1;
                                        i3 = SFnew(myobj2->x, myobj2->y);
                                        //destination is more important than the source
                                        sf[i3].type = 18; //green ball
                                        sf[i3].x = myobj->x;
                                        sf[i3].y = myobj->y;
                                        sf[i3].x2 = myobj2->x;
                                        sf[i3].y2 = myobj2->y;
                                        sf[i3].more = 0xFFFF;
                                        sf[i3].wait = 1;
                                        i3 = SFnew(myobj2->x, myobj2->y);
                                        sf[i3].type = 1; //attack
                                        sf[i3].x2 = 0;
                                        sf[i3].x = myobj2->x;
                                        sf[i3].y = myobj2->y;
                                        sf[i3].wait = 0.125f; //NULL
                                        sf[i3].more = 1;
                                        x2 = 0;
                                        spellattcrt = TRUE;
                                        goto spellattcrt0;
                                    } else { i2 = 3; } //spell failed
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //poison


                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 8)) {
                            //PROTECTION //txtset(spellname[(2<<4)+8],"Protection"); spellreagent[(2<<4)+8]=SULF|GINS|GARL;
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
                                            if (x3 > tnpc2->protection) {
                                                tnpc2->protection = x3;
                                                tnpc->upflags |= 64;
                                            }
                                            i9 = SFnew(myobj2->x, myobj2->y);
                                            //destination is more important than the source
                                            sf[i9].type = 14; //blue ball
                                            sf[i9].x = myobj->x;
                                            sf[i9].y = myobj->y;
                                            sf[i9].x2 = myobj2->x;
                                            sf[i9].y2 = myobj2->y;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                            i2 = 1;
                                        }
                                        if (x3 == 0) i2 = 3;
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //protection

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 9)) {
                            //xray //txtset(spellname[(4<<4)+9],"X-ray"); spellreagent[(4<<4)+9]=MAND|SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        x3 = rnd * (8 + (tnpc->i >> 3)); //int can only double spell effectiveness!
                                        if (x3) {
                                            if (x3 > 255) x3 = 255; //cap
                                            if (x3 > tnpc2->player->xray) {
                                                tnpc2->player->xray = x3;
                                                //send xray update message
                                                txtset(t, "??");
                                                t->d2[0] = 33;
                                                t->d2[1] = tnpc2->player->xray;
                                                NET_send(NETplayer, tnpc2->player->net, t);
                                            }
                                            i2 = 1;
                                        }
                                        if (x3 == 0) i2 = 3;
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //xray

                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 5)) {
                            //mass protect //txtset(spellname[(5<<4)+5],"MassProtect"); spellreagent[(5<<4)+5]=SULF|GINS|GARL|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            x3 = rnd * (32 + (tnpc->i >> 1)); //int can only double spell effectiveness!
                            if (x3) {
                                if (x3 > 255) x3 = 255; //cap
                                i2 = 1;
                                for (spellx = -2; spellx <= 2; spellx++) {
                                    for (spelly = -2; spelly <= 2; spelly++) {
                                        if ((abs(spellx) + abs(spelly)) != 4) {
                                            if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                                goto
                                                        massprotectblocked;
                                            myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                            if (myobj2 == NULL)
                                                myobj2 = OBJfindlastall(tpx + x + spellx,
                                                                        tpy + y + spelly);
                                            if (myobj2) {
                                                if (myobj2->info & 2) {
                                                    //<-npc
                                                    tnpc2 = (npc *) myobj2->more;
                                                    if (tnpc2->player) {
                                                        if (x3 > tnpc2->protection) {
                                                            tnpc2->protection = x3;
                                                            tnpc->upflags |= 64;
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
                                        massprotectblocked:;
                                        } //4
                                    }
                                } //spellx,spelly
                            } else { i2 = 3; } //failed
                            goto spelldone;
                        } //mass protect

                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 7)) {
                            //mass invis //txtset(spellname[(6<<4)+7],"MassInvisibility"); spellreagent[(6<<4)+7]=MAND|NIGH|BLOO|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;

                            x3 = rnd * (32 + (tnpc->i >> 1)); //int can only double spell effectiveness!
                            if (x3) {
                                if (x3 > 255) x3 = 255; //cap
                                i2 = 1;

                                for (spellx = -2; spellx <= 2; spellx++) {
                                    for (spelly = -2; spelly <= 2; spelly++) {
                                        if ((abs(spellx) + abs(spelly)) != 4) {
                                            if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                                goto
                                                        invisibilityblocked;

                                            myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                            if (myobj2 == NULL)
                                                myobj2 = OBJfindlastall(tpx + x + spellx,
                                                                        tpy + y + spelly);
                                            if (myobj2) {
                                                if (myobj2->info & 2) {
                                                    //<-npc
                                                    tnpc2 = (npc *) myobj2->more;
                                                    if (tnpc2->player) {
                                                        if (x3 > tnpc2->invisibility) tnpc2->invisibility = x3;
                                                    } //player
                                                } //npc
                                            } //myobj2
                                            if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                                i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                                //destination is more important than the source
                                                sf[i9].type = 19; //blue bolt
                                                sf[i9].x = tpx + x;
                                                sf[i9].y = tpy + y;
                                                sf[i9].x2 = tpx + x + spellx;
                                                sf[i9].y2 = tpy + y + spelly;
                                                sf[i9].more = 0xFFFF;
                                                sf[i9].wait = 1;
                                            }
                                        invisibilityblocked:;
                                        } //4
                                    }
                                } //spellx,spelly
                            } else { i2 = 3; } //failed
                            goto spelldone;
                        } //mass invis

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 3)) {
                            //invis //txtset(spellname[(4<<4)+3],"Invisibility"); spellreagent[(4<<4)+3]=NIGH|BLOO;
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
                                            if (x3 > tnpc2->invisibility) tnpc2->invisibility = x3;
                                            i9 = SFnew(myobj2->x, myobj2->y);
                                            //destination is more important than the source
                                            sf[i9].type = 19; //blue bolt
                                            sf[i9].x = myobj->x;
                                            sf[i9].y = myobj->y;
                                            sf[i9].x2 = myobj2->x;
                                            sf[i9].y2 = myobj2->y;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                            i2 = 1;
                                        }
                                        if (x3 == 0) i2 = 3;
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //invis

                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 3)) {
                            //great light //txtset(spellname[(2<<4)+3],"GreatLight"); spellreagent[(2<<4)+3]=SULF|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        x3 = rnd * (64 + tnpc->i); //int can only double spell effectiveness!
                                        if (x3) {
                                            x4 = 1 + x3;
                                            if (x4 > 255) x4 = 255; //cap
                                            tnpc2->light = x4;
                                            i2 = 1;
                                        }
                                        if (x3 == 0) i2 = 3;
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //great light


                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 9)) {
                            //light //txtset(spellname[(0<<4)+9],"Light"); spellreagent[(0<<4)+9]=SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        x3 = rnd * (16 + (tnpc->i >> 2)); //int can only double spell effectiveness!
                                        if (x3) {
                                            x4 = 1 + x3;
                                            if (x4 > 255) x4 = 255; //cap
                                            tnpc2->light = x4;
                                            i2 = 1;
                                        }
                                        if (x3 == 0) i2 = 3;
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //light


                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 7)) {
                            //awaken //txtset(spellname[(0<<4)+7],"Awaken"); spellreagent[(0<<4)+7]=GINS|GARL;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player == NULL) {
                                        if (tnpc2->schedule_last == 0x91) {
                                            if (((tpx + x) == 568) && ((tpy + y) == 364)) goto spelldone;
                                            if (((tpx + x) == 570) && ((tpy + y) == 364)) goto spelldone;
                                            if (((tpx + x) == 572) && ((tpy + y) == 364)) goto spelldone;
                                            if (((tpx + x) == 574) && ((tpy + y) == 364)) goto spelldone;
                                            i2 = 1;
                                            x3 = rnd * (8 + 5 * 1);
                                            x4 = rnd * (tnpc->i + 1);
                                            if (x4 >= x3) {
                                                //success //set npc schedule to unknown! and make them just stand there
                                                tnpc2->schedule_i = -1;
                                                tnpc2->schedule_last = 0;
                                                tnpc2->wait_walk += 64; //1mins
                                                //NEWCODE
                                                i9 = SFnew(myobj2->x, myobj2->y);
                                                //destination is more important than the source
                                                sf[i9].type = 14; //blue ball
                                                sf[i9].x = myobj->x;
                                                sf[i9].y = myobj->y;
                                                sf[i9].x2 = myobj2->x;
                                                sf[i9].y2 = myobj2->y;
                                                sf[i9].more = 0xFFFF;
                                                sf[i9].wait = 1;
                                                //NEWCODEEND
                                            } else { i2 = 3; } //spell failed
                                        } //sleeping
                                    } //!player
                                    if (tnpc2->player) {
                                        if (tnpc2->flags & 4) {
                                            i2 = 1;
                                            x3 = rnd * (8 + 5 * 1);
                                            x4 = rnd * (tnpc->i + 1);
                                            if (x4 >= x3) {
                                                //success
                                                tnpc2->flags -= 4;
                                                tnpc2->wait_disable = 0;
                                                //NEWCODE
                                                i9 = SFnew(myobj2->x, myobj2->y);
                                                //destination is more important than the source
                                                sf[i9].type = 14; //blue ball
                                                sf[i9].x = myobj->x;
                                                sf[i9].y = myobj->y;
                                                sf[i9].x2 = myobj2->x;
                                                sf[i9].y2 = myobj2->y;
                                                sf[i9].more = 0xFFFF;
                                                sf[i9].wait = 1;
                                                //NEWCODEEND
                                            } else { i2 = 3; } //spell failed
                                        } //4, sleeping
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //awaken

                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 6)) {
                            //summon //txtset(spellname[(7<<4)+6],"Summon"); spellreagent[(7<<4)+6]=MAND|GARL|BLOO;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            spellx = tpx + x;
                            spelly = tpy + y;
                            if ((spellx < 0) || (spellx > 2047) || (spelly < 0) || (spelly > 1023)) goto summonfail;
                            if (((bt[spelly][spellx] >> 10) & 1) == 0) goto summonfail; //land passable
                            if (od[spelly][spellx]) goto summonfail;
                            x3 = rnd * (8 + 5 * 8); //int can only double spell effectiveness!
                            x4 = rnd * (tnpc->i + 10);
                            if (x4 >= x3) {
                                //success
                                i2 = 1;
                                myobj2 = OBJnew();
                                myobj2->type = 367; //daemon!
                                myobj2->more = malloc(sizeof(creature));
                                ZeroMemory(myobj2->more, sizeof(creature));
                                crt = (creature *) myobj2->more;
                                crt->crt_struct = TRUE;
                                crt->hp = (obji[sprlnk[myobj2->type & 1023]].v8 >> 8) * 4;
                                crt->mp = rnd * 9;
                                crt->al = 1;
                                x3 = rnd * 4; //25% chance of evil allegiance!
                                if (x3) crt->al = 4;
                                crt->respawn_x = 1024; //seconds till spawned crt will disappear
                                myobj9 = OBJnew();
                                myobj9->type = 188;
                                crt->items = myobj9; //(not included)bag
                                myobj2->info |= 4; //<-crt
                                OBJadd(spellx, spelly, myobj2);
                            } else { i2 = 3; } //spell failed
