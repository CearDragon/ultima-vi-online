// LHS-P6.3: part_d_cast -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: player spellcasting entry (cast_spell0, staffcast0, ktarcast2,
// nextspell3/4). Move-only.
                    cast_spell0:
                        staff_cast_spell = 0;
                    staffcast0:

                        if (stormcloakcheck2(myobj->x, myobj->y, tplayer)) {
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, tnpc->name);
                                txtadd(t, " cannot cast magic at this time!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            goto castfailed;
                        }


                        CASTSPELL_SPELLTYPE = 0;

                        static long CASTSPELL_ENUMERATEDINVENTORYOBJS;
                        CASTSPELL_ENUMERATEDINVENTORYOBJS = 0;

                        if (!staff_cast_spell) {
                            //enumerate reagents!
                            ZeroMemory(&treagent, sizeof(treagent));
                            x4 = OBJlist((object *) tnpc->items->more);
                            OBJlist_last = NULL;
                            for (x5 = 0; x5 < x4; x5++) {
                                x3 = OBJlist_list[x5]->type;
                                if ((x3 >= 65) && (x3 <= 72)) {
                                    treagent[x3 - 65] += OBJlist_list[x5]->more2;
                                }
                            }
                            CASTSPELL_ENUMERATEDINVENTORYOBJS = x4; //number of inventory objects
                        } //!staff_cast_spell

                        if (!cast_spell) {
                            x = tplayer->mx;
                            y = tplayer->my & 255;
                            //fix x and y if using ktar
                            if (x & 32768) {
                                //spell ktar
                                x -= 32768;
                                if ((x >= 0) && (x <= 9)) {
                                    if (tplayer->ktar_wait[x]) {
                                        if (tplayer->ktar[x]->more) {
                                            myobj3 = (object *) tplayer->ktar[x]->more;
                                            crt = (creature *) myobj3->more;
                                            if (crt->flags & 2) goto castfailed;
                                            y = myobj3->y - tpy;
                                            x = myobj3->x - tpx;
                                            goto ktarcast2;
                                        } //ktar
                                    } //wait
                                } //x
                                goto castfailed;
                            }
                        ktarcast2:
                            CASTSPELL_SPELLTYPE = tplayer->my >> 8;
                        }
                        if (staff_cast_spell) CASTSPELL_SPELLTYPE = tplayer->staffcast_staff->more2 & 65535;

                        i2 = 0; //set to 1 if cast successful

                        if (tplayer->craft) {
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, "Not while aboard ship!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            goto castfailed;
                        }

                        if (!staff_cast_spell) {
                            if (tnpc->lev < ((CASTSPELL_SPELLTYPE >> 4) + 1)) {
                                if (!cast_spell) {
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, tnpc->name);
                                    txtadd(t, " is not ready for this circle!");
                                    NET_send(NETplayer, tplayer->net, t);
                                }
                                goto castfailed;
                            }

                            //check if spell actually exists in spellbook (safeguard!)
                            ZeroMemory(&tspell, sizeof(tspell));
                            //enumerate spells
                            if (myobj2 = (object *) tnpc->wep_left) {
                                if (myobj2->type == 57) {
                                    myobj2 = (object *) myobj2->more;
                                nextspell3:
                                    if (myobj2) {
                                        tspell[myobj2->more2] = 1;
                                        if (myobj2->next) {
                                            myobj2 = (object *) myobj2->next;
                                            goto nextspell3;
                                        }
                                    }
                                }
                            }
                            if (myobj2 = (object *) tnpc->wep_right) {
                                if (myobj2->type == 57) {
                                    myobj2 = (object *) myobj2->more;
                                nextspell4:
                                    if (myobj2) {
                                        tspell[myobj2->more2] = 1;
                                        if (myobj2->next) {
                                            myobj2 = (object *) myobj2->next;
                                            goto nextspell4;
                                        }
                                    }
                                }
                            }

                            // c222 spells lookup from lastused spellbook
                            if (easymodehostn1) {
                                if (tspell[CASTSPELL_SPELLTYPE] == 0) {
                                    if (myobj2 = playerspellbook) {
                                        if (myobj2->type == 57) {
                                            myobj2 = (object *) myobj2->more;
                                            while (myobj2) {
                                                tspell[myobj2->more2] = 1;
                                                myobj2 = (object *) myobj2->next;
                                                //if (myobj2->next) { myobj2 = (object*)myobj2->next; }
                                            }
                                        }
                                    }
                                }
                            }


                            if (tspell[CASTSPELL_SPELLTYPE] == 0) {
                                if (!cast_spell) {
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, "No spell!");
                                    NET_send(NETplayer, tplayer->net, t);
                                }
                                goto castfailed;
                            }

                            //check reagents for spell
                            for (x3 = 0; x3 <= 7; x3++) {
                                if (spellreagent[CASTSPELL_SPELLTYPE] & (1 << x3)) {
                                    if (treagent[x3] == 0) {
                                        if (!cast_spell) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "No reagents!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                        goto castfailed;
                                    }
                                }
                            }

                            //check spellpoints for spell
                            if (tnpc->mp < ((CASTSPELL_SPELLTYPE >> 4) + 1)) {
                                if (!cast_spell) {
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, "Not enough magic points!");
                                    NET_send(NETplayer, tplayer->net, t);
                                }
                                goto castfailed;
                            }
                        } //!staff_cast_spell

                        //in range?
                        if ((abs((tpx + x) - myobj->x) > 8) || (abs((tpy + y) - myobj->y) > 8)) {
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, "Out of range!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            goto castfailed;
                        }

                        if (tnpc->wait_cast || tnpc->wait_disable) {
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, tnpc->name);
                                txtadd(t, " busy!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            goto castfailed;
                        }

                        //staff enchant?
                        if (CASTSPELL_SPELLTYPE != ((6 << 4) + 1)) {
                            //not the enchant spell
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) {
                                i2 = 2;
                                if (myobj2->type == 78) {
                                    //dest location has a staff
                                    if ((CASTSPELL_SPELLTYPE == (myobj2->more2 & 65535)) || (
                                            (myobj2->more2 & 65535) == 0)) {
                                        x3 = (myobj2->info >> 9) & 7;
                                        if (x3) {
                                            //enchantment possible!

                                            if (OBJcheckbolt(myobj->x, myobj->y, tpx + x, tpy + y)) {
                                                if (!cast_spell) {
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "Blocked!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                }
                                                goto castfailed;
                                            }

                                            i2 = 1;
                                            myobj2->more2 |= CASTSPELL_SPELLTYPE;
                                            myobj2->info = myobj2->info & (0xFFFF - (7 << 9));
                                            x3--;
                                            myobj2->info |= (x3 << 9);
                                            x3 = myobj2->more2 >> 16;
                                            myobj2->more2 = myobj2->more2 & 0xFFFF;
                                            x3++;
                                            myobj2->more2 |= (x3 << 16);
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "Staff successfully enchanted with the ");
                                            txtset(t2, spellname[CASTSPELL_SPELLTYPE]);
                                            txtlcase(t2);
                                            txtadd(t, t2);
                                            txtadd(t, " spell (x");
                                            txtnumint(t2, myobj2->more2 >> 16);
                                            txtadd(t, t2);
                                            txtadd(t, ").");
                                            if ((myobj2->info >> 9) & 7) {
                                                txtadd(t, " ");
                                                txtnumint(t2, (myobj2->info >> 9) & 7);
                                                txtadd(t, t2);
                                                txtadd(t, " more enchantments left.");
                                            }
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto spelldone;
                                        } //x3
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(
                                            t,
                                            "You must cast enchant upon this staff before enchanting it with spells!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto spelldone;
                                    } //CASTSPELL_SPELLTYPE
                                    goto spelldone;
                                } //staff
                            } //findlast
                        } //CASTSPELL_SPELLTYPE!=enchant spell

                        //3. cast spell
                        static long spellx, spelly, spellx2, spelly2, spellz, spellz2;

                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 9)) {
                            //wizard eye //txtset(spellname[(6<<4)+9],"WizardEye"); spellreagent[(6<<4)+9]=BLOO|NIGH|MAND|SULF|BLAC|SPID;
                            i2 = 2; //implemented!
                            if (tnpc->player->wizardeyetimeleft == 0.0f) {
                                if (i9 = rnd * (tnpc->i + 1)) {
                                    i2 = 1;
                                    tnpc->player->wizardeyetimeleft = i9;
                                    tnpc->player->wizardeyemovewait = 0.0f;
                                    tnpc->player->wizardeyex = myobj->x;
                                    tnpc->player->wizardeyey = myobj->y;
                                    txtset(t, "??");
                                    t->d2[0] = 36;
                                    t->d2[1] = i9;
                                    NET_send(NETplayer, tnpc->player->net, t);
                                } else {
                                    i2 = 3;
                                }
                            } //timeleft==0
                            goto spelldone;
                        } //wizardeye

                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 7)) {
                            //peer txtset(spellname[(2<<4)+7],"Peer"); spellreagent[(2<<4)+7]=NIGH|MAND;
                            i2 = 2; //implemented!
                            x3 = rnd * (8 + 5 * 3);
                            x4 = rnd * (tnpc->i + 10);
                            if (x4 >= x3) {
                                //success
                                x3 = rnd * (32 + (tnpc->i >> 3));
                                if (x3) {
                                    if (x3 > 255) x3 = 255; //cap
                                    if (x3 > tnpc->player->peer) {
                                        tnpc->player->peer = x3;
                                        //send peer update message, changed to 48, 34 was already in use, but not in client side, so I don't know if it messed up anything or not. probably not
                                        txtset(t, "??");
                                        t->d2[0] = 48;
                                        t->d2[1] = tnpc->player->peer;
                                        NET_send(NETplayer, tnpc->player->net, t);
                                    }
                                    i2 = 1;
                                }
                                if (x3 == 0) i2 = 3;
                            } else { i2 = 3; } //fail
                            goto spelldone;
                        } //peer


                        //txtset(spellname[(1<<4)+5],"Telekinesis"); spellreagent[(1<<4)+5]=BLOO|MAND|BLAC;
                        if (CASTSPELL_SPELLTYPE == ((1 << 4) + 5)) {
                            //telekinesis   
                            i2 = spell_telekinesis(tplayer, tnpc, NULL, tpx + x, tpy + y);
                            goto spelldone;
                        } //telekinesis


                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 0)) {
                            //death wind //txtset(spellname[(7<<4)+0],"DeathWind"); spellreagent[(7<<4)+0]=MAND|NIGH|SULF|BLOO; //level 8
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            spellx = tpx + x;
                            spelly = tpy + y;
                            getwindspell(myobj->x, myobj->y, spellx, spelly);
                            //add special effects
                            for (z = 0; z <= WINDSPELL_boltn; z++) {
                                z2 = SFnew(WINDSPELL_boltx[z], WINDSPELL_bolty[z]);
                                sf[z2].type = 25; //multiple death bolts
                                sf[z2].x = myobj->x;
                                sf[z2].y = myobj->y;
                                sf[z2].x2 = WINDSPELL_boltx[z];
                                sf[z2].y2 = WINDSPELL_bolty[z];
                                sf[z2].more = 0xFFFF;
                                sf[z2].wait = 1;
                            } //z
                            for (spellz = 0; spellz <= WINDSPELL_n; spellz++) {
                                if (spellx = WINDSPELL_x[spellz]) {
                                    spelly = WINDSPELL_y[spellz];
                                    if (stormcloakcheck2(spellx, spelly, tplayer) == 0) {
                                        myobj2 = OBJfindlast(spellx, spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(spellx, spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                crt = (creature *) myobj2->more;
                                                i9 = i3;
                                                i8 = x2; //backup!
                                                x2 = rnd * (512 + tnpc->i * 8);
                                                x3 = rnd * (512 + tnpc->i * 8);
                                                x2 = ((x2 + x3) >> 1);
                                                if (x2 < crt->hp) {
                                                    i2 = 1;
                                                    x2 = i8;
                                                    i3 = i9;
                                                    goto deathwind_spelldone;
                                                }
                                                i3 = myobj2->type & 1023;
                                                if (i3 == 352) {
                                                    x2 = 0;
                                                    goto IDimmunedeath8;
                                                } //ghost
                                                if (i3 == 368) {
                                                    x2 = 0;
                                                    goto IDimmunedeath8;
                                                } //skeleton
                                                if (i3 == 373) {
                                                    x2 = 0;
                                                    goto IDimmunedeath8;
                                                } //wisp
                                                if (i3 == 367) {
                                                    x2 = 0;
                                                    goto IDimmunedeath8;
                                                } //daemon
                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                sf[i3].type = 1; //attack
                                                sf[i3].x2 = x2;
                                                sf[i3].x = myobj2->x;
                                                sf[i3].y = myobj2->y;
                                                sf[i3].wait = 0.125f; //NULL
                                                sf[i3].more = 1;
                                            IDimmunedeath8:
                                                spellattcrt = TRUE;
                                                goto spellattcrt0;
                                            } //myobj2
                                        } //crt
                                    } //stormcloakcheck2
                                } //spellx check
                            deathwind_spelldone:;
                            } //spellz
                            i2 = 1;
                            goto spelldone;
                        } //deathwind


                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 2)) {
                            //energy wind //txtset(spellname[(6<<4)+2],"EnergyWind"); spellreagent[(6<<4)+2]=MAND|NIGH|SULF|BLOO;
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            spellx = tpx + x;
                            spelly = tpy + y;
                            getwindspell(myobj->x, myobj->y, spellx, spelly);
                            //add special effects
                            for (z = 0; z <= WINDSPELL_boltn; z++) {
                                z2 = SFnew(WINDSPELL_boltx[z], WINDSPELL_bolty[z]);
                                sf[z2].type = 24; //multiple lightning!
                                sf[z2].x = myobj->x;
                                sf[z2].y = myobj->y;
                                sf[z2].x2 = WINDSPELL_boltx[z];
                                sf[z2].y2 = WINDSPELL_bolty[z];
                                sf[z2].more = 0xFFFF;
                                sf[z2].wait = 1;
                            } //z
                            for (spellz = 0; spellz <= WINDSPELL_n; spellz++) {
                                if (spellx = WINDSPELL_x[spellz]) {
                                    spelly = WINDSPELL_y[spellz];
                                    if (stormcloakcheck2(spellx, spelly, tplayer) == 0) {
                                        myobj2 = OBJfindlast(spellx, spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(spellx, spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                crt = (creature *) myobj2->more;
                                                i9 = i3;
                                                i8 = x2; //backup!
                                                x2 = rnd * (64 + tnpc->i * 2);
                                                if (x2 == 0) {
                                                    i2 = 3;
                                                    x2 = i8;
                                                    i3 = i9;
                                                    goto energywind_spelldone;
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
                                            } //myobj2
                                        } //crt
                                    } //stormcloakcheck2
                                } //spellx check
                            energywind_spelldone:;
                            } //spellz
                            i2 = 1;
                            goto spelldone;
                        } //energywind

                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 7)) {
                            //poison wind //txtset(spellname[(5<<4)+7],"PoisonWind"); spellreagent[(5<<4)+7]=NIGH|SULF|BLOO;
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            spellx = tpx + x;
                            spelly = tpy + y;
                            getwindspell(myobj->x, myobj->y, spellx, spelly);
                            //add special effects
                            for (z = 0; z <= WINDSPELL_boltn; z++) {
                                z2 = SFnew(WINDSPELL_boltx[z], WINDSPELL_bolty[z]);
                                sf[z2].type = 23; //multiple green balls
                                sf[z2].x = myobj->x;
                                sf[z2].y = myobj->y;
                                sf[z2].x2 = WINDSPELL_boltx[z];
                                sf[z2].y2 = WINDSPELL_bolty[z];
                                sf[z2].more = 0xFFFF;
                                sf[z2].wait = 1;
                            } //z
                            for (spellz = 0; spellz <= WINDSPELL_n; spellz++) {
                                if (spellx = WINDSPELL_x[spellz]) {
                                    spelly = WINDSPELL_y[spellz];
                                    if (stormcloakcheck2(spellx, spelly, tplayer) == 0) {
                                        myobj2 = OBJfindlast(spellx, spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(spellx, spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                crt = (creature *) myobj2->more;
                                                x3 = rnd * (8 + 5 * 2);
                                                x4 = rnd * (tnpc->i + 10);
                                                if (x4 >= x3) {
                                                    //success
                                                    crt->flags |= 1;
                                                    z2 = SFnew(myobj2->x, myobj2->y);
                                                    sf[z2].type = 1; //attack
                                                    sf[z2].x2 = 0;
                                                    sf[z2].x = myobj2->x;
                                                    sf[z2].y = myobj2->y;
                                                    sf[z2].wait = 0.125f; //NULL
                                                    sf[z2].more = 1;
                                                } //success
                                            } //crt
                                        } //myobj2
                                    } //stormcloakcheck2
                                } //spellx check
                            } //spellz
                            i2 = 1;
                            goto spelldone;
                        } //poisonwind

                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 3)) {
                            //flame wind //txtset(spellname[(5<<4)+3],"FlameWind"); spellreagent[(5<<4)+3]=SULF|BLOO|MAND;
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            spellx = tpx + x;
                            spelly = tpy + y;
                            getwindspell(myobj->x, myobj->y, spellx, spelly);
                            //add special effects
                            for (z = 0; z <= WINDSPELL_boltn; z++) {
                                z2 = SFnew(WINDSPELL_boltx[z], WINDSPELL_bolty[z]);
                                sf[z2].type = 22; //multiple fireballs
                                sf[z2].x = myobj->x;
                                sf[z2].y = myobj->y;
                                sf[z2].x2 = WINDSPELL_boltx[z];
                                sf[z2].y2 = WINDSPELL_bolty[z];
                                sf[z2].more = 0xFFFF;
                                sf[z2].wait = 1;
                            } //z
                            for (spellz = 0; spellz <= WINDSPELL_n; spellz++) {
                                if (spellx = WINDSPELL_x[spellz]) {
                                    spelly = WINDSPELL_y[spellz];
                                    if (stormcloakcheck2(spellx, spelly, tplayer) == 0) {
                                        myobj2 = OBJfindlast(spellx, spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(spellx, spelly);
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
                                                    goto flamewind_spelldone;
                                                }
                                                //fire resistant???
                                                i3 = myobj2->type & 1023;
                                                if (i3 == 411) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //dragon
                                                if (i3 == 369) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //drake
                                                if (i3 == 352) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //ghost
                                                if (i3 == 374) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //hydra
                                                if (i3 == 369) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //drake
                                                if (i3 == 368) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //skeleton
                                                if (i3 == 373) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
                                                } //wisp
                                                if (i3 == 367) {
                                                    x2 = 0;
                                                    goto IFimmuneflame4;
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
                                            IFimmuneflame4:
                                                spellattcrt = TRUE;
                                                goto spellattcrt0;
                                            } //myobj2
                                        } //crt
                                    } //stormcloakcheck2
                                } //spellx check
                            flamewind_spelldone:;
                            } //spellz
                            i2 = 1;
                            goto spelldone;
                        } //flamewind


                        if (OBJcheckbolt(myobj->x, myobj->y, tpx + x, tpy + y)) {
                            if (!cast_spell) {
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtadd(t, "Blocked!");
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            goto castfailed;
                        }
                        //spell blocked???????????????????????????????????????????????

                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 4)) {
                            //resurrect //txtset(spellname[(7<<4)+4],"Resurrect"); spellreagent[(7<<4)+4]=GARL|GINS|SPID|SULF|BLOO|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2) {
                                if ((myobj2->type & 1023) == 339) {
                                    //dead body

                                    //***RESRTICTIONS: SOME PLACES IN BRITANNIA DEAD BODIES CANNOT BE RESURRECTED***
                                    x4 = tpx + x;
                                    y4 = tpy + y;
                                    //1. SACRED QUEST AREA x1 899 y1 811 x2 948 y2 872
                                    if (x4 >= 899) {
                                        if (x4 <= 948) {
                                            if (y4 >= 811) {
                                                if (y4 <= 872) {
                                                    goto spelldone;
                                                }
                                            }
                                        }
                                    }
                                    //2. PLAYER HOUSING
                                    x6 = housecheck(x4, y4);
                                    if (x6) {
                                        goto spelldone;
                                    }
                                    //3. PLAYER HOUSING - STORAGE
                                    for (x5 = 1; x5 <= 255; x5++) {
                                        for (y5 = 0; y5<housestoragenext[x5]; y5++) {
                                            if (x4 == housestoragex[x5][y5]) {
                                                if (y4 == housestoragey[x5][y5]) {
                                                    goto spelldone;
                                                }
                                            } //x,y
                                        } //y5
                                    } //x5

                                    //4. WALKABLE SQUARE
                                    if ((bt[y4][x4] & 1024) == 0) {
                                        goto spelldone;
                                    }

                                    //5. SHRINE IN THE VOID
                                    if (x4 >= 1024) {
                                        if (x4 <= 1071) {
                                            if (y4 >= 0) {
                                                if (y4 <= 47) {
                                                    goto spelldone;
                                                }
                                            }
                                        }
                                    }


                                    //scan resurrect list
                                    for (i3 = 0; i3 <= nresu; i3++) {
                                        if (resu[i3]) {
                                            if (resu_body[i3] == myobj2) {
                                                if (resu_player[i3]) {
                                                    //has a player pointer

                                                    //resurrection can be performed
                                                    i2 = 1;
                                                    x3 = rnd * (8 + 5 * 8);
                                                    x4 = rnd * (tnpc->i + 50);
                                                    if (x4 >= x3) {
                                                        //success
                                                        //resu_player[i3]->party[0]=resu[i3]; //assume current player
                                                        resu_player[i3]->party[resu_partymember[i3]] = resu[i3];

                                                        if (resu_partymember[i3] == 0) {
                                                            for (x4 = 1; x4 <= 7; x4++) {
                                                                if (resu_player[i3]->party[x4]) {
                                                                    tnpc3 = (npc *) resu_player[i3]->party[x4]->more;
                                                                    tnpc3->upflags = 1;
                                                                    OBJmove_allow = TRUE;
                                                                    OBJmove2(resu_player[i3]->party[x4], myobj2->x,
                                                                             myobj2->y); //place party on the map
                                                                    OBJmove_allow = FALSE;
                                                                }
                                                            } //x4
                                                        }

                                                        OBJmove_allow = TRUE;
                                                        OBJmove2(resu[i3], myobj2->x, myobj2->y);
                                                        //place avatar on the map
                                                        OBJmove_allow = FALSE;
                                                        OBJremove(myobj2); //remove body
                                                        tnpc3 = (npc *) resu[i3]->more;
                                                        resu[i3] = NULL;
                                                        txtset(t5, "?");
                                                        t5->d2[0] = 8;
                                                        txtadd(t5, "A voice in the darkness intones, \x022");
                                                        txtadd(t5, "IN MANI CORP!\x022");
                                                        NET_send(NETplayer, resu_player[i3]->net, t5);

                                                        txtset(t5, "?");
                                                        t5->d2[0] = 8;
                                                        txtadd(t5, tnpc3->name);
                                                        txtadd(t5, " resurrected by ");
                                                        txtadd(t5, tnpc->name);
                                                        txtadd(t5, ".");
                                                        for (x3 = 0; x3 <= playerlist_last; x3++) {
                                                            //create and send UPDATE message
                                                            if (playerlist[x3]) {
                                                                //current player
                                                                if (playerlist[x3]->net != NULL) {
                                                                    //net connection available
                                                                    if (playerlist[x3]->party[0]) {
                                                                        if (playerlist[x3] != tplayer) {
                                                                            NET_send(NETplayer, playerlist[x3]->net,
                                                                                t5);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        inbritanniacheck();
                                                        i2 = 1;
                                                    } else {
                                                        i2 = 3;
                                                        if (resu_partymember[i3] == 0) goto spelldone_donttakereagents;
                                                        //add bug!
                                                    }
                                                    goto resurrect_spell_done;
                                                } //player
                                            } //resu_body==obj
                                        } //resu
                                    } //i3
                                } //dead body
                            } //myobj2
                        resurrect_spell_done:
                            goto spelldone;
                        } //resurrect


                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 4)) {
                            //great heal //txtset(spellname[(3<<4)+4],"GreatHeal"); spellreagent[(3<<4)+4]=GINS|SPID|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        //perform light heal (1 to 64->128 hp)
                                        x3 = rnd * (64 + tnpc->i); //int can only double spell effectiveness!
                                        x4 = rnd * (64 + tnpc->i); //int can only double spell effectiveness!
                                        x3 = (x3 + x4) >> 1;

                                        tnpc2->hp += x3;
                                        if (tnpc2->hp > tnpc2->hp_max) tnpc2->hp = tnpc2->hp_max;
                                        tnpc2->upflags |= 2;
                                        i2 = 1;
                                        if (x3 == 0) i2 = 3;
                                        if (x3) {
                                            i9 = SFnew(myobj2->x, myobj2->y);
                                            //destination is more important than the source
                                            sf[i9].type = 14; //blue ball
                                            sf[i9].x = myobj->x;
                                            sf[i9].y = myobj->y;
                                            sf[i9].x2 = myobj2->x;
                                            sf[i9].y2 = myobj2->y;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //great heal


                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 3)) {
                            //mass kill! //txtset(spellname[(7<<4)+3],"MassKill"); spellreagent[(7<<4)+3]=BLAC|NIGH|MAND|SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;

                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    masskillblocked;

                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if (myobj2->info & 4) {
                                                //<-crt

                                                crt = (creature *) myobj2->more;
                                                i9 = i3;
                                                i8 = x2; //backup!
                                                x2 = rnd * (512 + tnpc->i * 8);
                                                x3 = rnd * (512 + tnpc->i * 8);
                                                x2 = ((x2 + x3) >> 1);
                                                if (x2 < crt->hp) {
                                                    i2 = 1;
                                                    x2 = i8;
                                                    i3 = i9;
                                                    goto masskill_spelldone;
                                                } //failed!
                                                i3 = myobj2->type & 1023;
                                                if (i3 == 352) {
                                                    x2 = 0;
                                                    goto IDimmunedeath2;
                                                } //ghost
                                                if (i3 == 368) {
                                                    x2 = 0;
                                                    goto IDimmunedeath2;
                                                } //skeleton
                                                if (i3 == 373) {
                                                    x2 = 0;
                                                    goto IDimmunedeath2;
                                                } //wisp
                                                if (i3 == 367) {
                                                    x2 = 0;
                                                    goto IDimmunedeath2;
                                                } //daemon

                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                sf[i3].type = 1; //attack
                                                sf[i3].x2 = x2;
                                                sf[i3].x = myobj2->x;
                                                sf[i3].y = myobj2->y;
                                                sf[i3].wait = 0.125f; //NULL
                                                sf[i3].more = 1;
                                            IDimmunedeath2:
                                                spellattcrt = TRUE;
                                                goto spellattcrt0;
                                            } //crt
                                        } //myobj2

                                    masskill_spelldone:;

                                        if ((abs(spellx) == 2) || (abs(spelly) == 2)) {
                                            i9 = SFnew(tpx + x + spellx, tpy + y + spelly);
                                            //destination is more important than the source
                                            sf[i9].type = 15; //kill blast
                                            sf[i9].x = tpx + x;
                                            sf[i9].y = tpy + y;
                                            sf[i9].x2 = tpx + x + spellx;
                                            sf[i9].y2 = tpy + y + spelly;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }

                                    masskillblocked:;
                                    } //!=4
                                }
                            } //spell x,y
                            i2 = 1;

                            goto spelldone;
                        } //mass kill!

                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 2)) {
                            //disable! //txtset(spellname[(3<<4)+2],"Disable"); spellreagent[(3<<4)+2]=NIGH|SPID|MAND;
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
                                    //x2=rnd*(256+tnpc->i*4);
                                    //x3=rnd*(256+tnpc->i*4);
                                    x2 = rnd * (192 + tnpc->i * 3);
                                    x3 = rnd * (192 + tnpc->i * 3);
                                    x2 = ((x2 + x3) >> 1);
                                    if (x2 < crt->hp) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    } //failed!
                                    x2 = crt->hp - 1;
                                    i3 = SFnew(myobj2->x, myobj2->y);
                                    sf[i3].type = 1; //attack
                                    sf[i3].x2 = x2;
                                    sf[i3].x = myobj2->x;
                                    sf[i3].y = myobj2->y;
                                    sf[i3].wait = 0.125f; //NULL
                                    sf[i3].more = 1;
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 15; //kill/disable blast
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
                        } //disable!

                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 5)) {
                            //kill! //txtset(spellname[(6<<4)+5],"Kill"); spellreagent[(6<<4)+5]=BLAC|NIGH|SULF;
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
                                    x2 = rnd * (512 + tnpc->i * 8);
                                    x3 = rnd * (512 + tnpc->i * 8);
                                    x2 = ((x2 + x3) >> 1);
                                    if (x2 < crt->hp) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    } //failed!
                                    i3 = myobj2->type & 1023;
                                    if (i3 == 352) {
                                        x2 = 0;
                                        goto IDimmunedeath;
                                    } //ghost
                                    if (i3 == 368) {
                                        x2 = 0;
                                        goto IDimmunedeath;
                                    } //skeleton
                                    if (i3 == 373) {
                                        x2 = 0;
                                        goto IDimmunedeath;
                                    } //wisp
                                    if (i3 == 367) {
                                        x2 = 0;
                                        goto IDimmunedeath;
                                    } //daemon

                                    i3 = SFnew(myobj2->x, myobj2->y);
                                    sf[i3].type = 1; //attack
                                    sf[i3].x2 = x2;
                                    sf[i3].x = myobj2->x;
                                    sf[i3].y = myobj2->y;
                                    sf[i3].wait = 0.125f; //NULL
                                    sf[i3].more = 1;
                                IDimmunedeath:
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 15; //kill blast
                                    sf[i3].x = myobj->x;
                                    sf[i3].y = myobj->y;
                                    sf[i3].x2 = myobj2->x;
                                    sf[i3].y2 = myobj2->y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = 1;
                                    if (x2 == 0) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    }
                                    spellattcrt = TRUE;
                                    goto spellattcrt0;
                                } //crt
                            } //myobj2
                            goto spelldone;
                        } //kill!

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 4)) {
                            //lightning //txtset(spellname[(4<<4)+4],"Lightning"); spellreagent[(4<<4)+4]=BLAC|MAND|SULF;
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
                                    x2 = rnd * (64 + tnpc->i * 2);
                                    if (x2 == 0) {
                                        i2 = 3;
                                        x2 = i8;
                                        i3 = i9;
                                        goto spelldone;
                                    }
                                    i3 = SFnew(myobj2->x, myobj2->y); //destination is more important than the source
                                    sf[i3].type = 5;
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
                        } //lightning

                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 0)) {
                            //chain bolt //txtset(spellname[(6<<4)+0],"ChainBolt"); spellreagent[(6<<4)+0]=BLAC|MAND|SULF|BLOO; //level 7
                            static long chainbolt_hits, chainbolt_miss;
                            chainbolt_hits = 0;
                            chainbolt_miss = 0;
                            i2 = 2; //implemented!
                            spellx = tpx + x;
                            spelly = tpy + y;
                            if (stormcloakcheck2(spellx, spelly, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(spellx, spelly);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(spellx, spelly);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                                if (myobj2->info & 4) {
                                    //<-crt
                                    i2 = 3;
