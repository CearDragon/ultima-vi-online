// LHS-P6.8: part_d_look -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: look/examine (inventory_look, horsepaperslook_*, houselook_*).
// Move-only.
                    inventory_look:

                        //if it's a player give a different description with their name
                        if (myobj->info & 2) {
                            //npc
                            tnpc2 = (npc *) myobj->more;
                            if (tnpc2->player) {
                                tplayer2 = (player *) tnpc2->player;

                                /*
                if (tplayer2!=tplayer){//not a member of our party
                if (tnpc2->port){
                txtset(t5,"?"); t5->d2[0]=2;
                txtset(t7,"??"); t7->ds[0]=1; txtadd(t5,t7);
                t7->ds[0]=28; txtadd(t5,t7);//type=portrait look
                t7->ds[0]=myobj->x; txtadd(t5,t7);
                t7->ds[0]=myobj->y; txtadd(t5,t7);
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=tnpc2->port; txtadd(t5,t7);//more=port
                txtset(t7,"????"); t7->df[0]=1.0f; txtadd(t5,t7); //wait
                NET_send(NETplayer,tplayer->net,t5);
                }
                }
                */

                                //send npc look message
                                /*
                [44]
                [portrait]
                [length of name]
                [name data(if any!)]
                [1 byte value with bits indicating what is equipped]
                order is:
                helm,wep-right,wep-left,armour,boots,ring right, ring left,neck
                2 bytes for type, and 1 byte for a plus value per item



                */

                                if (tnpc2->port) {
                                    txtsetchar(t5, 44);
                                    txtaddshort(t5, tnpc2->port);
                                    //name is ALWAYS known for npcs with a ->player pointer
                                    txtaddchar(t5, tnpc2->name->l);
                                    txtadd(t5, tnpc2->name);

                                    i3 = 0xFFFFFF;
                                    if (tplayer2->party[0] == myobj) {
                                        i3 = tplayer2->GNPCflags[245];
                                    }
                                    txtaddchar(t5, i3 & 0xFF);
                                    txtaddchar(t5, (i3 >> 8) & 0xFF);
                                    txtaddchar(t5, (i3 >> 16) & 0xFF);

                                    txtset(t6, "");
                                    i3 = 0;
                                    i5 = 1;
                                    for (i4 = 0; i4 < 8; i4++) {
                                        if (i4 == 0) myobj3 = tnpc2->helm;
                                        if (i4 == 1) myobj3 = tnpc2->wep_right;
                                        if (i4 == 2) myobj3 = tnpc2->wep_left;
                                        if (i4 == 3) myobj3 = tnpc2->armour;
                                        if (i4 == 4) myobj3 = tnpc2->boots;
                                        if (i4 == 5) myobj3 = tnpc2->ring_right;
                                        if (i4 == 6) myobj3 = tnpc2->ring_left;
                                        if (i4 == 7) myobj3 = tnpc2->neck;
                                        if (myobj3) {
                                            i3 += i5;
                                            //type
                                            txtaddshort(t6, myobj3->type);
                                            //+ bonus
                                            i6 = ENHANCEget_attack(myobj3);
                                            if (!i6) i6 = ENHANCEget_defense(myobj3);
                                            txtaddchar(t6, i6);
                                        } //myobj3
                                        i5 <<= 1;
                                    } //i4
                                    txtaddchar(t5, i3);
                                    txtadd(t5, t6);
                                    NET_send(NETplayer, tplayer->net, t5);
                                } //tnpc2->port


                                if (tplayer2->party[0] == myobj) {
                                    txtsetchar(t, 8);
                                    txtaddchar(t, 255);
                                    txtadd(t, "Thou dost see ");
                                    if (tplayer == tplayer2) {
                                        txtadd(t, "yourself.");
                                        goto skip_playername2;
                                    }
                                    txtadd(t, tnpc2->name);
                                    txtadd(t, ". (Level ");
                                    txtnumint(t2, tnpc2->lev);
                                    txtadd(t, t2);
                                    i3 = tnpc2->hp_max;
                                    txtset(t2, ")");
                                    if (tnpc2->hp < i3) txtset(t2, ", grazed)");
                                    if (tnpc2->hp < (i3 * 4 / 5)) txtset(t2, ", barely wounded)");
                                    if (tnpc2->hp < (i3 * 3 / 5)) txtset(t2, ", lightly wounded)");
                                    if (tnpc2->hp < (i3 * 2 / 5)) txtset(t2, ", heavily wounded)");
                                    if (tnpc2->hp < (i3 / 5)) txtset(t2, ", critically hurt)");
                                    txtadd(t, t2);
                                skip_playername2:
                                    goto skip_playername;
                                }
                                if (tnpc2->name) {
                                    //MUST BE A PARTY MEMBER!
                                    txtsetchar(t, 8);
                                    txtaddchar(t, 255);
                                    txtadd(t, "Thou dost see ");
                                    txtadd(t, tnpc2->name);
                                    txtadd(t, ". (Level ");
                                    txtnumint(t2, tnpc2->lev);
                                    txtadd(t, t2);
                                    i3 = tnpc2->hp_max;
                                    txtset(t2, ")");
                                    if (tnpc2->hp < i3) txtset(t2, ", grazed)");
                                    if (tnpc2->hp < (i3 * 4 / 5)) txtset(t2, ", barely wounded)");
                                    if (tnpc2->hp < (i3 * 3 / 5)) txtset(t2, ", lightly wounded)");
                                    if (tnpc2->hp < (i3 * 2 / 5)) txtset(t2, ", heavily wounded)");
                                    if (tnpc2->hp < (i3 / 5)) txtset(t2, ", critically hurt)");
                                    txtadd(t, t2);
                                    goto skip_playername;
                                }
                            } else {
                                //no player pointer npc!


                                //portrait look (non-player npc)
                                if (tnpc2->port) {
                                    txtsetchar(t5, 44);
                                    txtaddshort(t5, tnpc2->port);


                                    //name is ALWAYS known for npcs with a ->player pointer
                                    //txtaddchar(t5,tnpc2->name->l);
                                    //txtadd(t5,tnpc2->name);

                                    //set t6 to stock answer
                                    i3 = objname[(myobj->type & 1023) * 2];
                                    i4 = objname[(myobj->type & 1023) * 2 + 1];
                                    txtNEWLEN(t6, i4);
                                    memcpy(&t6->d2[0], &objname2[i3], i4);

                                    //retrieve name if known
                                    //to have a name they must have a converse number
                                    if (i4 = tnpc2->converse) {
                                        if (i4 != 201) {
                                            if (i4 == 255) i4 = 0; //Dupre correction
                                            if ((tplayer->NPCflags[i4] & 256) >> 8) {
                                                tfh = open(".\\host\\npcname.txt");
                                                for (i3 = 0; i3 <= i4; i3++) {
                                                    txtfilein(t6, tfh);
                                                }
                                                close(tfh);
                                            }
                                        } else {
                                            //201!
                                            txtset(t6, tnpc2->name);
                                        }
                                    }

                                    txtaddchar(t5, t6->l);
                                    txtadd(t5, t6);


                                    i3 = 0xFFFFFF;
                                    txtaddchar(t5, i3 & 0xFF);
                                    txtaddchar(t5, (i3 >> 8) & 0xFF);
                                    txtaddchar(t5, (i3 >> 16) & 0xFF);

                                    txtset(t6, "");
                                    i3 = 0;
                                    i5 = 1;
                                    for (i4 = 0; i4 < 8; i4++) {
                                        if (i4 == 0) myobj3 = tnpc2->helm;
                                        if (i4 == 1) myobj3 = tnpc2->wep_right;
                                        if (i4 == 2) myobj3 = tnpc2->wep_left;
                                        if (i4 == 3) myobj3 = tnpc2->armour;
                                        if (i4 == 4) myobj3 = tnpc2->boots;
                                        if (i4 == 5) myobj3 = tnpc2->ring_right;
                                        if (i4 == 6) myobj3 = tnpc2->ring_left;
                                        if (i4 == 7) myobj3 = tnpc2->neck;
                                        if (myobj3) {
                                            i3 += i5;
                                            //type
                                            txtaddshort(t6, myobj3->type);
                                            //+ bonus
                                            i6 = ENHANCEget_attack(myobj3);
                                            if (!i6) i6 = ENHANCEget_defense(myobj3);
                                            txtaddchar(t6, i6);
                                        } //myobj3
                                        i5 <<= 1;
                                    } //i4
                                    txtaddchar(t5, i3);
                                    txtadd(t5, t6);
                                    NET_send(NETplayer, tplayer->net, t5);
                                } //tnpc2->port


                                /*
                if (tnpc2->port){
                txtset(t5,"?"); t5->d2[0]=2;
                txtset(t7,"??"); t7->ds[0]=1; txtadd(t5,t7);
                t7->ds[0]=28; txtadd(t5,t7);//type=portrait look
                t7->ds[0]=myobj->x; txtadd(t5,t7);
                t7->ds[0]=myobj->y; txtadd(t5,t7);
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=0; txtadd(t5,t7); //unused
                t7->ds[0]=tnpc2->port; txtadd(t5,t7);//more=port
                txtset(t7,"????"); t7->df[0]=1.0f; txtadd(t5,t7); //wait
                NET_send(NETplayer,tplayer->net,t5);
                }
                */
                            }
                        }

                        //have object
                        //create a description & send a message


                        //txtset(t,"?");
                        //t->d2[0]=8;
                        txtsetchar(t, 8);
                        txtaddchar(t, 255);


                        //t2<-object name
                        x2 = objname[(myobj->type & 1023) * 2];
                        x3 = objname[(myobj->type & 1023) * 2 + 1];
                        txtNEWLEN(t2, x3);
                        memcpy(&t2->d2[0], &objname2[x2], x3);

                        txtset(t3, "dead body");
                        if (txtsame(t2, t3)) {
                            for (i3 = 0; i3 <= nresu; i3++) {
                                if (resu[i3]) {
                                    if (resu_body[i3] == myobj) {
                                        tnpc2 = (npc *) resu[i3]->more;
                                        txtset(t2, "Thou dost see ");
                                        txtadd(t2, tnpc2->name);
                                        goto gotgrammar;
                                    }
                                }
                            }
                        }


                        //check for crt pointer
                        if (myobj->info & 4) {
                            crt2 = (creature *) myobj->more;

                            //horse (owned by a player)
                            if ((myobj->type & 1023) == 430) {
                                if (crt2->flags & 128) {
                                    txtset(t, "?");
                                    txtsetchar(t, 8);
                                    txtaddchar(t, 255);
                                    txtadd(t, "Thou dost see ");

                                horsepaperslook_getmoreinfo:

                                    if (crt2->respawn_y == 0xFFFF) {
                                        txtadd(t, "a ");
                                    } else {
                                        if (playerlist[crt2->respawn_y]) {
                                            if (playerlist[crt2->respawn_y]->net) {
                                                if (myobj2 = getprimarypartymemberobj(playerlist[crt2->respawn_y])) {
                                                    txtadd(t, ((npc *) myobj2->more)->name);
                                                    txtadd(t, "'s ");
                                                    goto houselook_foundownersname;
                                                }
                                            }
                                        }
                                        txtadd(t, "a ");
                                    }
                                houselook_foundownersname:
                                    //grain state
                                    if (((crt2->respawn_x >> 4) & 3) == 3) txtadd(t, "full horse ");
                                    if (((crt2->respawn_x >> 4) & 3) == 2) txtadd(t, "well fed horse ");
                                    if (((crt2->respawn_x >> 4) & 3) == 1) txtadd(t, "hungry horse ");
                                    if (((crt2->respawn_x >> 4) & 3) == 0) txtadd(t, "starving horse ");
                                    //horseshoes state
                                    if (((crt2->respawn_x >> 6) & 3) == 3) txtadd(t, "with new horseshoes");
                                    if (((crt2->respawn_x >> 6) & 3) == 2) txtadd(t, "with good horseshoes");
                                    if (((crt2->respawn_x >> 6) & 3) == 1) txtadd(t, "with reasonable horseshoes");
                                    if (((crt2->respawn_x >> 6) & 3) == 0) txtadd(t, "with worn horseshoes");
                                    if (crt2->respawn_x & 15) {
                                        txtnumint(t2, crt2->respawn_x & 15);
                                        txtadd(t, ", it has won ");
                                        txtadd(t, t2);
                                        if ((crt2->respawn_x & 15) == 1) txtadd(t, " race!");
                                        else txtadd(t, " races!");
                                    } else {
                                        txtadd(t, ".");
                                    }
                                    goto skiphorsewithowner;
                                }
                            }


                            i3 = (obji[sprlnk[myobj->type & 1023]].v8 >> 8) * 4;
                            txtset(t5, t2);
                            txtset(t2, "");
                            if (crt2->hp < i3) txtset(t2, "grazed ");
                            if (crt2->hp < (i3 * 4 / 5)) txtset(t2, "barely wounded ");
                            if (crt2->hp < (i3 * 3 / 5)) txtset(t2, "lightly wounded ");
                            if (crt2->hp < (i3 * 2 / 5)) txtset(t2, "heavily wounded ");
                            if (crt2->hp < (i3 / 5)) txtset(t2, "critically hurt ");
                            txtadd(t2, t5);
                        }


                        if (myobj->type == 78) {
                            //staff
                            txtset(t2, "");
                            if (myobj->more2 & 65535) {
                                txtset(t3, spellname[myobj->more2 & 65535]);
                                txtlcase(t3);
                                txtadd(t2, t3);
                                txtadd(t2, " ");
                            }
                            txtadd(t2, "staff");
                            if (myobj->more2 & 65535) {
                                txtadd(t2, " (x");
                                txtnumint(t3, myobj->more2 >> 16);
                                txtadd(t2, t3);
                                txtadd(t2, ")");
                            }
                        }


                        if (myobj->type == 81) {
                            txtset(t2, "new");
                            if (myobj->more2 != 0) txtset(t2, "barely used");
                            if (myobj->more2 >= 512) txtset(t2, "worn");
                            if (myobj->more2 >= 1024) txtset(t2, "old");
                            txtadd(t2, " storm cloak");
                        }

                        if ((myobj->type & 1023) == 98) {
                            i3 = myobj->type >> 10;
                            if (i3 == 0) txtset(t2, "open chest");
                            if (i3 == 2) txtset(t2, "locked chest");
                            if (i3 == 3) txtset(t2, "magically locked chest");
                        }

                        i3 = myobj->type & 1023;
                        if ((i3 >= 297) && (i3 <= 300)) {
                            txtset(t5, t2);
                            i3 = myobj->type >> 12;
                            if (i3 == 0) txtset(t2, "open ");
                            if (i3 == 1) txtset(t2, "");
                            if (i3 == 2) txtset(t2, "locked ");
                            if (i3 == 3) txtset(t2, "magically locked ");
                            txtadd(t2, t5);
                        } //i3

                        //check if it's a spell
                        txtset(t3, "spell");
                        if (txtsame(t2, t3)) {
                            //txtset(t3,
                            txtset(t2, spellname[myobj->more2]);
                            txtlcase(t2);
                            txtadd(t2, " spell");
                        }

                        txtset(t3, "fire wand");
                        if (txtsame(t2, t3)) {
                            //txtset(t3,
                            txtset(t2, "new");
                            if (myobj->more2 != 0) txtset(t2, "smoldering");
                            if (myobj->more2 >= 64) txtset(t2, "flaming");
                            if (myobj->more2 >= 192) txtset(t2, "blazing");
                            txtadd(t2, " fire wand");
                        }

                        txtset(t3, "lightning wand");
                        if (txtsame(t2, t3)) {
                            //txtset(t3,
                            txtset(t2, "new");
                            if (myobj->more2 != 0) txtset(t2, "glowing");
                            if (myobj->more2 >= 64) txtset(t2, "sparkling");
                            if (myobj->more2 >= 192) txtset(t2, "brilliant");
                            txtadd(t2, " lightning wand");
                        }

                        txtset(t3, "invisibility ring");
                        if (txtsame(t2, t3)) {
                            txtset(t2, "new");
                            if (myobj->more2 != 0) txtset(t2, "glowing");
                            if (myobj->more2 >= 64) txtset(t2, "shining");
                            if (myobj->more2 >= 192) txtset(t2, "brilliant");
                            txtadd(t2, " invisibility ring");
                        }

                        txtset(t3, "regeneration ring");
                        if (txtsame(t2, t3)) {
                            txtset(t2, "new");
                            if (myobj->more2 != 0) txtset(t2, "glowing");
                            if (myobj->more2 >= 32) txtset(t2, "shining");
                            if (myobj->more2 >= 96) txtset(t2, "brilliant");
                            txtadd(t2, " regeneration ring");
                        }

                        txtset(t3, "protection ring");
                        if (txtsame(t2, t3)) {
                            txtset(t2, "new");
                            if (myobj->more2 != 0) txtset(t2, "glowing");
                            if (myobj->more2 >= 1024) txtset(t2, "shining");
                            if (myobj->more2 >= 3072) txtset(t2, "brilliant");
                            txtadd(t2, " protection ring");
                        }

                        txtset(t3, "torch");
                        if (txtsame(t2, t3)) {
                            x3 = (myobj->info >> 9) & 15;
                            txtset(t2, "new");
                            if (x3 != 0) txtset(t2, "old");
                            if (x3 >= 4) txtset(t2, "half used");
                            if (x3 >= 12) txtset(t2, "almost new");
                            txtadd(t2, " torch");
                            if (myobj->more2 > 1) txtadd(t2, "e");
                        }

                        txtset(t3, "ship deed");
                        if (txtsame(t2, t3)) {
                            if (myobj->info & (1 << 9)) txtset(t2, "skiff deed");
                            else txtset(t2, "ship deed");
                        }


                        if (myobj->type == OBJ_TMAP) {
                            //treasure map
                            x5 = (myobj->more2) & 1023;
                            y2 = (myobj->more2 >> 10) & 1023;
                            x6 = (myobj->more2 >> 20) & 7;
                            y3 = (myobj->more2 >> 23) & 7;
                            x4 = (myobj->more2 >> 26) & 7;
                            y4 = (myobj->more2 >> 29) & 7;
                            x5 += (x6 - 3) * 2;
                            if (x5 < 0) { x5 = 0; }
                            if (x5 >= 1024) { x5 = 1023; }
                            y2 += (y3 - 3) * 2;
                            if (y2 < 0) { y2 = 0; }
                            if (y2 >= 1024) { y2 = 1023; }
                            txtset(t8, "?");
                            t8->d2[0] = 49;
                            txtset(t9, "????");
                            t9->dl[0] = (x5 + 1024 * y2 + 1024 * 1024 * x4 + 1024 * 1024 * 8 * y4);
                            txtadd(t8, t9);
                            NET_send(NETplayer, tnpc->player->net, t8);
                        }

                        //apply grammar rules // luteijn Original uses special flags for this!
                        //exceptions
                        if ((myobj->type & 1023) == 409) {
                            txtadd(t, "Thou dost see ");
                            goto gotgrammar;
                        } //LB
                        if ((myobj->type & 1023) == 360) {
                            txtadd(t, "Thou dost see ");
                            goto gotgrammar;
                        } //rot worms
                        if ((myobj->type & 1023) == 343) {
                            txtadd(t, "Thou dost see ");
                            goto gotgrammar;
                        } //insects
                        if ((myobj->type & 1023) == 375) {
                            txtadd(t, "Thou dost see ");
                            goto gotgrammar;
                        } //slime
                        if ((myobj->type & 1023) == 135) {
                            txtadd(t, "Thou dost see ");
                            goto gotgrammar;
                        } //horse chops
                        //monsters
                        if (obji[sprlnk[myobj->type & 1023]].v4) goto amonster;
                        //nc non-countable
                        if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].v6 & 1) {
                            txtadd(t, "Thou dost see ");
                            goto gotgrammar;
                        }
                        if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].flags & 4096) {
                            //multiple
                            if (myobj->more2 > 1) {
                                txtadd(t, "Thou dost see ");
                                txtnumint(t3, myobj->more2);
                                txtadd(t, t3);
                                txtadd(t, " ");
                                //of-> +s of
                                txtset(t3, " of ");
                                if (x2 = txtsearch(t2, t3)) {
                                    txtset(t3, t2);
                                    txtleft(t3, x2 - 1);
                                    if ((myobj->type & 1023) == 128) txtset(t3, "loave"); //loaf->loaves
                                    txtset(t4, t2);
                                    txtright(t4, t2->l - x2 + 1);
                                    txtadd(t3, "s");
                                    txtadd(t3, t4);
                                    txtset(t2, t3);
                                    goto gotgrammar;
                                }
                                //default: +s
                                txtadd(t2, "s");
                                goto gotgrammar;
                            } //>1
                        } //multiple
                    amonster:
                        //the + CAPITAL LETTER
                        x2 = t2->d2[0];
                        if ((x2 >= 65) && (x2 <= 90)) {
                            txtadd(t, "Thou dost see the ");
                            goto gotgrammar;
                        }
                        // vowel?
                        x3 = 0;
                        if (x2 == 97) x3 = 1; //a
                        if (x2 == 101) x3 = 1; //e
                        if (x2 == 105) x3 = 1; //i
                        if (x2 == 111) x3 = 1; //o
                        if (x2 == 117) x3 = 1; //u
                        if (x3) {
                            txtadd(t, "Thou dost see an ");
                            goto gotgrammar;
                        }
                        txtadd(t, "Thou dost see a ");


                    gotgrammar:
                        txtadd(t, t2);

                        x2 = ENHANCEget_attack(myobj);
                        if (!x2) x2 = ENHANCEget_defense(myobj);
                        if (x2) {
                            txtadd(t, " +");
                            txtnumint(t2, x2);
                            txtadd(t, t2);
                        }

                        if ((myobj->type & 1023) == OBJ_POCKET_WATCH && myobj->more2 != 0) {
                            //different construction phases in the quest
                            if (myobj->more2 == 1) {
                                txtadd(t, " mold");
                            }
                            if (myobj->more2 == 2) {
                                txtadd(t, " frame");
                            }
                            if (myobj->more2 == 3) {
                                txtadd(t, " that is missing the glass crystal");
                            }
                            if (myobj->more2 == 4) {
                                txtadd(t, " that is almost done");
                            }
                        }

                        f = WTfind(myobj);
                        f9 = f;
                        if (f) {
                            txtadd(t, ", weighing ");
                            x2 = f / 100;
                            txtnumint(t2, x2);
                            txtadd(t, t2);
                            if ((float) x2 != (f / 100.0f)) {
                                txtadd(t, ".");
                                txtnumint(t2, (int) f % 100);
                                if (t2->l == 1) txtadd(t, "0");
                                if (t2->l == 2) {
                                    if (t2->d2[1] == 48) txtleft(t2, 1);
                                }
                                txtadd(t, t2);
                            }
                            if (f == 1.0f) txtadd(t, " stone");
                            else txtadd(t, " stones");
                        }

                        if (myobj->type == 448) {
                            //horse papers
                            txtadd(t, ", for ");
                            myobj = (object *) myobj->more;
                            crt2 = (creature *) myobj->more;
                            goto horsepaperslook_getmoreinfo;
                        }


                        txtadd(t, ".");


                        if ((obji[sprlnk[myobj->type & 1023]].v1) && (obji[sprlnk[myobj->type & 1023]].v2)) {
                            txtadd(t, " It can do ");
                            x2 = obji[sprlnk[myobj->type & 1023]].v1 + ENHANCEget_attack(myobj);
                            txtnumint(t2, x2);
                            txtadd(t, t2);
                            txtadd(t, " and absorb ");
                            x2 = obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(myobj);
                            txtnumint(t2, x2);
                            txtadd(t, t2);
                            if (x2 == 1) txtadd(t, " point of damage");
                            else txtadd(t, " points of damage");
                            txtadd(t, ".");
                            goto complex_done;
                        }

                        if (obji[sprlnk[myobj->type & 1023]].v1) {
                            txtadd(t, " It can do ");
                            x2 = obji[sprlnk[myobj->type & 1023]].v1 + ENHANCEget_attack(myobj);
                            txtnumint(t2, x2);
                            txtadd(t, t2);
                            if (x2 == 1) txtadd(t, " point of damage");
                            else txtadd(t, " points of damage");
                            txtadd(t, ".");
                        }

                        if (obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(myobj)) {
                            if ((myobj->type & 1023) == 27) txtadd(t, " They can absorb ");
                            else txtadd(t, " It can absorb "); //boots exception
                            x2 = obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(myobj);
                            txtnumint(t2, x2);
                            txtadd(t, t2);
                            if (x2 == 1) txtadd(t, " point of damage");
                            else txtadd(t, " points of damage");
                            txtadd(t, ".");
                        }
                    complex_done:


                        //if ((ol[i].type&1)==0) myobj->info|=128; //128 1=stealing, 0=ok to take
                        if (myobj->info & 128) {
                            if (f9) {
                                //has weight
                                txtadd(t, " Taking this is stealing!");
                            }
                        }

                        if (myobj->info & 256) {
                            if (f9) {
                                //has weight
                                txtadd(t, " This is a quest item!");
                            }
                        }


                        if (myobj->type == OBJ_SUNDIAL) {
                            //sundial
                            if ((btime > 5) && (btime < 20)) goto thetimeis;
                        }
                        if ((myobj->type & 1023) == OBJ_CLOCK || (
                                (myobj->type & 1023) == OBJ_POCKET_WATCH && myobj->more2 == 0)) {
                            //clock
                        thetimeis:
                            txtadd(t, " The time is ");
                            x = btime;
                            x2 = (btime - (float) x) * 60.0f;
                            if (x > 12) x -= 12;
                            if (x == 0) x = 12;
                            txtnumint(t2, x);
                            txtadd(t, t2);
                            txtadd(t, ":");
                            txtnumint(t2, x2);
                            if (t2->l == 1) txtadd(t, "0");
                            txtadd(t, t2);
                            if (btime >= 12.0f) txtadd(t, " P.M.");
                            else txtadd(t, " A.M.");
                        }

                        //txtadd(t,".");

                    skiphorsewithowner:
                    skip_playername:
                        NET_send(NETplayer, tplayer->net, t);

                        //sign //scroll //picture //book //gsign //codex //circles //tombstone //cross
                        if (((myobj->type & 1023) == 332) || ((myobj->type & 1023) == 152) || (
                                (myobj->type & 1023) == 143) || ((myobj->type & 1023) == 151) || (
                                (myobj->type & 1023) == 333) || ((myobj->type & 1023) == 59) || (
                                (myobj->type & 1023) == 61) || ((myobj->type & 1023) == 254) || (
                                (myobj->type & 1023) == 255)) {
                            if (myobj->more2) {
                                if (myobj->more2 & 65536) {
                                    //house sign!
                                    x2 = myobj->more2 & 65535;
                                    if (housesav[x2].flags & 1) {
                                        //occupied
                                        txtset(t, "?");
                                        txtset(t3, "");
                                        for (x3 = 0; x3 <= 31; x3++) {
                                            if (housesav[x2].charactername[x3]) {
                                                t->d2[0] = housesav[x2].charactername[x3];
                                                txtadd(t3, t);
                                            }
                                        }
                                        txtadd(t3, "'s House");

                                        if (x2 == 47 && (myobj->x) != 79) txtadd(t3, "\\Beware of the drakes!");
                                        goto housesign;
                                    }

                                    txtset(t3, "For Sale\\No.");
                                    txtnumint(t4, x2);
                                    txtadd(t3, t4);
                                    if (x2 == 47 && (myobj->x) != 79) txtadd(t3, "\\Beware of the drakes!");
                                    goto housesign;
                                }


                                x2 = tsign[(myobj->more2 - 1) * 2];
                                x3 = tsign[(myobj->more2 - 1) * 2 + 1];
                                txtNEWLEN(t, x3);
                                memcpy(&t->d2[0], &tsign2[x2], x3);
                                txtset(t3, t);
                                //1garg 2normal 3runes
                                txtset(t, "?");
                                t->d2[0] = 2;
                                if (x = txtsearch(t3, t)) {
                                    if (tnpc->player->GNPCflags[9]) {
                                        txtset(t, t3);
                                        txtleft(t3, x - 1);
                                        txtright(t, t->l - x);
                                        txtadd(t3, t);
                                    } else {
                                        txtleft(t3, x - 2);
                                    }
                                } //2
                            housesign:

                                //edit t3?
                                //MessageBox(NULL,t3->d,"Ultima 6 Online",MB_OK);

                                //create PRIVATE sfx message
                                txtset(t, "?");
                                t->d2[0] = 2;
                                txtset(t2, "??");
                                t2->ds[0] = 1;
                                txtadd(t, t2);
                                t2->ds[0] = 3;
                                txtadd(t, t2);
                                t2->ds[0] = myobj->x - 1;
                                if (myobj->x == 0)
                                    t2->ds[0] = tnpc->player->party[tnpc->player->selected_partymember]->x - 1;
                                txtadd(t, t2);
                                t2->ds[0] = myobj->y - 1;
                                if (myobj->y == 0)
                                    t2->ds[0] = tnpc->player->party[tnpc->player->selected_partymember]->y - 1;
                                txtadd(t, t2);
                                t2->ds[0] = t3->l;
                                txtadd(t, t2);
                                t2->ds[0] = 0;
                                txtadd(t, t2); //unused
                                t2->ds[0] = 187;
                                txtadd(t, t2); //more=port blank!
                                txtset(t2, "????");
                                t2->df[0] = 2;
                                txtadd(t, t2); //wait
                                txtadd(t, t3);
                                NET_send(NETplayer, tplayer->net, t);
                            }
                        }


                        /*
            if (t->d2[0]==8){ //set mouse object
            exit2(8);
            goto CLIENT_donemess;
            }
            */


                        goto skip_pickup;
                    looknullobject:; //2003: skip null objects
                        //report info about the basetile being looked at
                        x2 = bt[y][x] & 1023;
                        if (x2 == 0) goto skip_pickup; //nothing!
                        if (x2 == 1) txtset(t, "grass");
                        if (x2 >= 2) txtset(t, "swamp");
                        if (x2 >= 5) txtset(t, "a shrub");
                        if (x2 >= 6) txtset(t, "a bush");
                        if (x2 >= 8) txtset(t, "water");
                        if (x2 >= 16) txtset(t, "shore");
                        if (x2 >= 48) txtset(t, "a tree");
                        if (x2 >= 52) txtset(t, "grass");
                        if (x2 >= 64) txtset(t, "a tree");
                        if (x2 >= 72) txtset(t, "a dead tree");
                        if (x2 >= 73) txtset(t, "scrub");
                        if (x2 >= 78) txtset(t, "a shrub");
                        if (x2 >= 79) txtset(t, "earth");
                        if (x2 >= 96) txtset(t, "an embankment");
                        if (x2 >= 108) txtset(t, "earth");
                        if (x2 >= 112) txtset(t, "a mountain");
                        if (x2 >= 125) txtset(t, "snowcaps");
                        if (x2 >= 140) txtset(t, "a wall");
                        if (x2 >= 188) txtset(t, "a field");
                        if (x2 >= 189) txtset(t, "crops");
                        if (x2 >= 192) txtset(t, "an arrow slit");
                        if (x2 >= 196) txtset(t, "a window");
                        if (x2 >= 198) txtset(t, "a crenellation");
                        if (x2 >= 200) txtset(t, "a window");
                        if (x2 == 206) goto skip_pickup; //nothing!
                        if (x2 >= 207) txtset(t, "a wall");
                        if (x2 >= 208) txtset(t, "the floor");
                        if (x2 >= 217) txtset(t, "wet earth");
                        if (x2 >= 218) txtset(t, "a puddle");
                        if (x2 >= 219) txtset(t, "a pool");
                        if (x2 >= 220) txtset(t, "the floor");
                        if (x2 >= 221) txtset(t, "lava");
                        if (x2 >= 224) txtset(t, "a pyramid");
                        if (x2 >= 240) txtset(t, "an earthen wall");
                        if (x2 >= 252) txtset(t, "the ethereal void");
                        if (x2 >= 256) goto skip_pickup; //nothing!
                        txtset(t2, "Thou dost see ");
                        txtadd(t2, t);
                        txtadd(t2, ".");
                        txtsetchar(t, 8);
                        txtaddchar(t, 255);
                        txtadd(t, t2);
                        NET_send(NETplayer, tplayer->net, t);
                        goto skip_pickup;
                    } //action==2 look

                    if (tplayer->action == 3) {
                        //attack

                        if (tplayer->craft) {
                            if ((tplayer->party[0]->type & 1023) == 412) {
                                //ship
                                goto shipattack;
                            }
                        }

                        if (tplayer->craft == NULL) {
