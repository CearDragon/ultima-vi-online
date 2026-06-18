// LHS-P6.9: part_d_lockpick_drop -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: lockpick (lockpick, lockpick2), drop (dropallow, checkvortexcube).
// Move-only.
                                        lockpick:
                                            myobj2 = OBJfindlastall(x, y);
                                            if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //<-redirect
                                            myobj2 = OBJfindlastall(myobj2->x, myobj2->y); //2004
                                            //GPF!


                                            //txtnumint(t5,myobj2->more2);
                                            //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                                            //txtnumint(t5,tplayer->mobj->more2);
                                            //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                                            if ((myobj2->more2 == 0) && (tplayer->mobj->type == 63)) {
                                                if ((myobj->type >> 12) != 2) goto skip_pickup;
                                                myobj3 = (object *) tplayer->party[0];
                                                tnpc = (npc *) myobj3->more;
                                                stealing(tplayer, tplayer->mobj);
                                                if (((rnd * 64) + (rnd * tnpc->i)) >= 56) goto lockpick2; //success
                                                x2 = rnd * 2;
                                                if (x2) {
                                                    //break pick!
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "Key broke!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    if (tplayer->mobj->more2 == 1) tplayer->mobj = NULL;
                                                    else tplayer->mobj->more2--;
                                                    goto skip_pickup;
                                                }
                                            }
                                            if ((myobj2->more2) && (tplayer->mobj->type == 63)) goto skip_pickup;
                                            if (myobj2->more2 == tplayer->mobj->more2) {
                                                stealing(tplayer, tplayer->mobj);
                                            lockpick2:
                                                //unlock/relock door!
                                                //myobj->more2=
                                                if (myobj->info & 8) myobj = (object *) myobj->more; //<-redirect
                                                x2 = myobj->type >> 12;
                                                if ((x2 != 2) && (tplayer->mobj->type == 63)) goto skip_pickup;
                                                if (x2 == 1) {
                                                    //unlocked->lock
                                                    myobj->type = (myobj->type & 4095) + (2 << 12);
                                                    myobj3 = (object *) myobj->more;
                                                    myobj3->type = (myobj3->type & 4095) + (2 << 12);
                                                    if (enhancehostn1) {
                                                        txtsetchar(t, 8);
                                                        txtaddchar(t, 255);
                                                        txtadd(t, "Door locked!");
                                                        NET_send(NETplayer, tplayer->net, t); // f111
                                                    }
                                                }
                                                if (x2 == 2) {
                                                    myobj->type = (myobj->type & 4095) + (1 << 12);
                                                    myobj3 = (object *) myobj->more;
                                                    myobj3->type = (myobj3->type & 4095) + (1 << 12);
                                                    if (enhancehostn1) {
                                                        txtsetchar(t, 8);
                                                        txtaddchar(t, 255);
                                                        txtadd(t, "Door unlocked!");
                                                        NET_send(NETplayer, tplayer->net, t); // f111
                                                    }
                                                }
                                                goto skip_pickup;
                                            }
                                        }
                                    } //closed
                                } //door


                                goto skip_pickup;
                            } //use item on item


                            //}else{


                            if (myobj->info & 2) {
                                tnpc2 = (npc *) myobj->more;
                                if (tnpc2->player == tplayer) {
                                    tnpc = tnpc2;
                                    i = -1;
                                    if (tnpc->player) {
                                        if (tnpc->player->party[0]) {
                                            if ((npc *) tnpc->player->party[0]->more == tnpc) {
                                                i = 0;
                                            }
                                        }
                                    }
                                    goto sprite_drop;
                                }
                            }

                            if (i == 394) {
                                if ((x == 921) && (y == 851)) {
                                    if (tplayer->mobj->type == 394) {
                                        goto dropallow;
                                    }
                                }
                            }

                            if (i == 396) {
                                if ((x == 925) && (y == 851)) {
                                    if (tplayer->mobj->type == 396) {
                                        goto dropallow;
                                    }
                                }
                            }

                            if (i == 62) {
                                if ((x == 923) && (y == 850)) {
                                    if (tplayer->mobj->type == 62) {
                                        goto dropallow;
                                    }
                                }
                            }

                            if (obji[i2].flags & 512) {
                            dropallow:
                                z2 = 9;
                                myobj2 = NULL;
                                for (z = 0; z <= 7; z++) {
                                    if (myobj3 = tplayer->party[z]) {
                                        if (OBJcheckbolt(myobj3->x, myobj3->y, x, y) == FALSE) {
                                            z3 = abs(myobj3->x - x);
                                            z4 = abs(myobj3->y - y);
                                            if (z4 > z3) z3 = z4;
                                            if (z3 < z2) {
                                                myobj2 = myobj3;
                                                z2 = z3;
                                            }
                                        }
                                    }
                                }
                                if (myobj2) {
                                    if ((tplayer->mobj->type & 1023) == 420) {
                                        //skiff drop
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "You cannot put a balloon here!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto itemdropped2;
                                    }


                                    if (tplayer->mobj->info & 256) {
                                        //drop quest item

                                        //brit lens
                                        if (tplayer->mobj->type == 394) {
                                            if ((x == 921) && (y == 851)) {
                                                tplayer->GNPCflags[18] |= 2;
                                                tplayer->mobj = NULL; //clear selected mouse object
                                                goto itemdropped2;
                                            }
                                        }

                                        //garg lens
                                        if (tplayer->mobj->type == 396) {
                                            if ((x == 925) && (y == 851)) {
                                                tplayer->GNPCflags[18] |= 4;
                                                tplayer->mobj = NULL; //clear selected mouse object
                                                goto itemdropped2;
                                            }
                                        }

                                        //vortex cube
                                        if (tplayer->mobj->type == 62) {
                                            if ((x == 923) && (y == 850)) {
                                                //check internal contents of vortex cube!
                                                x2 = 0;
                                                myobj2 = (object *) tplayer->mobj->more;
                                            checkvortexcube:
                                                if (myobj2) {
                                                    x2 |= (1 << (myobj2->type >> 10));
                                                    if (myobj2 = (object *) myobj2->next) goto checkvortexcube;
                                                }
                                                if (x2 != 255) {
                                                    txtset(t, "?Place the eight moonstones within the Vortex Cube.");
                                                    t->d2[0] = 8;
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto itemdropped2;
                                                }
                                                tplayer->GNPCflags[18] |= 1;
                                                tplayer->mobj = NULL; //clear selected mouse object
                                                goto itemdropped2;
                                            }
                                        }

                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "You cannot put a quest item here!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto itemdropped2;
                                    }


                                    if ((tplayer->mobj->type & 1023) == 414) {
                                        //skiff drop
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "You cannot put a skiff here!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto itemdropped2;
                                    }

                                    if (tplayer->mobj->type == 149) {
                                        //deed drop
                                        //in current boat?
                                        if ((object *) tplayer->mobj->more == tplayer->party[0]) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You cannot drop this deed now!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped2;
                                        }
                                        //currently holding the boat this deed points to?
                                        myobj3 = (object *) tplayer->mobj->more;
                                        //txtnumint(t5,myobj->more2);
                                        if ((myobj3->x == 0) && (myobj3->y == 0)) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You must drop the skiff first!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped2;
                                        }
                                    }

                                    if (tplayer->mobj->type == 448) {
                                        //horse papers
                                        if (((object *) tplayer->mobj->more)->x == 0) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You cannot drop these horse papers now!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped2;
                                        }
                                        crt2 = (creature *) ((object *) tplayer->mobj->more)->more;
                                        crt2->respawn_y = 0xFFFF;
                                    } //448

                                    x5 = OBJlist(tplayer->mobj); //x5=last object number
                                    OBJlist_last = NULL;

                                    //check if drop location is a house storage shelf
                                    for (i3 = 1; i3 <= 255; i3++) {
                                        for (i4 = 0; i4<housestoragenext[i3]; i4++) {
                                            if (x == housestoragex[i3][i4]) {
                                                if (y == housestoragey[i3][i4]) {
                                                    //stolen items check
                                                    for (y6 = 0; y6 < x5; y6++) {
                                                        if (OBJlist_list[y6]->info & 128) {
                                                            //stolen item!
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put stolen things here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto itemdropped2;
                                                        } //stolen item!
                                                    } //y6

                                                    //8 stack limit check
                                                    myobj = od[housestoragey[i3][i4]][housestoragex[i3][i4]];
                                                    x2 = 0;
                                                countstacklimit:
                                                    if (myobj->next) {
                                                        x2++;
                                                        myobj = (object *) myobj->next;
                                                        goto countstacklimit;
                                                    }
                                                    if (x2 >= 8) {
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(
                                                            t,
                                                            "You cannot stack more than 8 items here! Put items in containers to store more here.");
                                                        NET_send(NETplayer, tplayer->net, t);
                                                        goto itemdropped2;
                                                    }
                                                }
                                            } //x,y
                                        } //i4
                                    } //i3


                                    //clear notforsale marks
                                    for (y6 = 0; y6 < x5; y6++) {
                                        OBJlist_list[y6]->info |= 32768;
                                        OBJlist_list[y6]->info ^= 32768;
                                    } //y6


                                    /*
                  if (tplayer->mobj->info&128){//stolen item
                  for (i3=0;i3<=15;i3++) if (stolenitemwarning[i3]==tplayer->mobj) goto stolenitem_alreadyadded;
                  for (i3=0;i3<=15;i3++){
                  if (stolenitemwarning[i3]==NULL){
                  stolenitemwarning[i3]=tplayer->mobj;
                  stolenitemwarning_wait[i3]=2.0f;
                  stolenitemwarning_player[i3]=tplayer;
                  goto stolenitem_alreadyadded;
                  }//==NULL
                  }//i3
                  }//stolen item
                  stolenitem_alreadyadded:
                  */
                                    if (tplayer->mobj->info & 128) {
                                        //stolen item
                                        i3 = SFnew(x, y); //destination is more important than the source
                                        sf[i3].type = 21; //stolen item warning
                                        sf[i3].x = x;
                                        sf[i3].y = y;
                                        sf[i3].more = tplayer->mobj->type;
                                        sf[i3].wait = 2.0f;
                                    }


                                    //drop sfx!
                                    i3 = SFnew(x, y); //destination is more important than the source
                                    sf[i3].type = 9; //GENERIC throw object
                                    sf[i3].x = myobj2->x;
                                    sf[i3].y = myobj2->y;
                                    sf[i3].x2 = x;
                                    sf[i3].y2 = y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = tplayer->mobj->type;

                                    //drop sound effect
                                    if ((tplayer->mobj->type & 1023) == 88) {
                                        //gold
                                        if (tplayer->mobj->more2 >= 5) {
                                            i3 = SFnew(x, y);
                                            sf[i3].type = 4; //sound
                                            sf[i3].x = x;
                                            sf[i3].y = y;
                                            sf[i3].wait = 1.0f; //NULL
                                            sf[i3].more = 6; //coinmany
                                        } else {
                                            i3 = SFnew(x, y);
                                            sf[i3].type = 4; //sound
                                            sf[i3].x = x;
                                            sf[i3].y = y;
                                            sf[i3].wait = 1.0f; //NULL
                                            sf[i3].more = 7; //coinfew
                                        }
                                    }

                                    //smash it?
                                    i4 = 0;
                                    if ((tplayer->mobj->type & 1023) == 275) i4 = 1; //potions
                                    if (tplayer->mobj->type == 115) i4 = 1; //
                                    if (tplayer->mobj->type == 116) i4 = 1; //
                                    if (tplayer->mobj->type == 117) i4 = 1; //
                                    if (tplayer->mobj->type == 118) i4 = 1; //
                                    if (tplayer->mobj->type == 119) i4 = 1; //
                                    if (tplayer->mobj->type == 120) i4 = 1; //mug
                                    if (tplayer->mobj->type == 123) {
                                        //mirror
                                        if ((abs(myobj2->x - x) > 1) || (abs(myobj2->y - y) > 1)) {
                                            sf[i3].wait += 32768;
                                            tplayer->mobj->type = 123 + 1024 * 2;
                                            stealing(tplayer, tplayer->mobj);
                                        }
                                    }
                                    if ((tplayer->mobj->type == 179) || (tplayer->mobj->type == 180)) {
                                        //bucket of milk/water
                                        if ((abs(myobj2->x - x) > 1) || (abs(myobj2->y - y) > 1)) {
                                            tplayer->mobj->type = 178;
                                            sf[i3].wait = tplayer->mobj->type;
                                            stealing(tplayer, tplayer->mobj);
                                        }
                                    }
                                    if (tplayer->mobj->type == 92) i4 = 1; //silver snake venom
                                    if (tplayer->mobj->type == 161) i4 = 1; //jug
                                    if (tplayer->mobj->type == 83) i4 = 1; //oil flask
                                    if (tplayer->mobj->type == 48) i4 = 1; //glass sword
                                    if (tplayer->mobj->type == 183) i4 = 1; //jar
                                    if (tplayer->mobj->type == 184) i4 = 1; //jar with honey
                                    if ((abs(myobj2->x - x) <= 1) && (abs(myobj2->y - y) <= 1)) i4 = 0;
                                    if (i4) {
                                        sf[i3].wait += 32768;
                                        OBJrelease(tplayer->mobj);
                                        stealing(tplayer, tplayer->mobj);
                                    } else {
                                        OBJadd(x, y, tplayer->mobj);
                                    }
                                    tplayer->mobj = NULL; //clear selected mouse object

                                    goto itemdropped2;

                                    //}//<=8
                                    //}
                                    //}}
                                } //myobj2

                            itemdropped2:;
                            } //bt flag
                        } //myobj!=NULL
                        else {
                            //myobj==NULL hence no item underneath!

                            if (tplayer->action == 1) {
                                //use item on basetile

                                if (tplayer->mobj->type == 87) {
                                    //orb of the moons
                                    if (tplayer->GNPCflags[23] == 0) {
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "You can't figure out how to use it!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto skip_pickup;
                                    }
                                    if (bt[y][x] & 1024) {
                                        //walkable location
                                        if (myobj2 = tplayer->party[tplayer->selected_partymember]) {
                                            if (abs(myobj2->x - x) <= 2) {
                                                if (abs(myobj2->y - y) <= 2) {
                                                    tnpc = (npc *) myobj2->more;
                                                    if (tnpc->wait_disable <= 0) {
                                                        myobj3 = OBJnew();
                                                        myobj3->type = 84 + 1024;
                                                        myobj3->info = 3 << 4; //set flags as temp OBJ
                                                        myobj3->more2 =
                                                                (x - myobj2->x + 2) + (y - myobj2->y + 2) * 5 + 1;
                                                        OBJadd(x, y, myobj3);
                                                        myobj3 = OBJnew();
                                                        myobj3->type = 84;
                                                        myobj3->info = 3 << 4; //set flags as temp OBJ
                                                        myobj3->more2 =
                                                                (x - myobj2->x + 2) + (y - myobj2->y + 2) * 5 + 1;
                                                        OBJadd(x - 1, y, myobj3);
                                                        tnpc->wait_disable += 8.0f;
                                                    } else {
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(t, tnpc->name);
                                                        txtadd(t, " busy!");
                                                        NET_send(NETplayer, tplayer->net, t);
                                                    }
                                                }
                                            } //x,y
                                        } //myobj2
                                    } //&1024
                                    goto skip_pickup;
                                } //orb

                                //myobj3=OBJnew(); myobj3->type=87; myobj3->more2=0; myobj3->info=256; //orb of moons
                                /*
                myobj2=OBJnew();
                myobj2->type=84+1024;
                myobj2->info=3<<4; //set flags as temp OBJ
                OBJadd(tpx+x,tpy+y,myobj2);
                myobj2=OBJnew();
                myobj2->type=84;
                myobj2->info=3<<4; //set flags as temp OBJ
                OBJadd(tpx+x-1,tpy+y,myobj2);
                */


                                z2 = 1;
                                for (z = 0; z <= 7; z++) {
                                    if (myobj2 = tplayer->party[z]) {
                                        if (abs(myobj2->x - x) <= 1) {
                                            if (abs(myobj2->y - y) <= 1) {
                                                z2 = 0;
                                            }
                                        }
                                    }
                                }
                                if (z2) goto skip_pickup;

                                if ((tplayer->mobj->type & 1023) == 264) {
                                    //fishing line
                                    if (((bt[y][x] & 1023) >= 8) && ((bt[y][x] & 1023) < 16)) {
                                        //in water
                                        tnpc = (npc *) tplayer->party[0]->more;
                                        if (tnpc->wait_disable <= 0) {
                                            if ((rnd * 8.0f) < 1.0f) {
                                                //add fish!
                                                myobj3 = OBJnew();
                                                myobj3->type = 265; //<-fish
                                                f = WTfind(myobj3);
                                                if ((tnpc->wt + f) <= tnpc->wt_max) {
                                                    additemroot(tnpc, myobj3);
                                                    tnpc->upflags |= 32;
                                                } //weight check
                                            } else {
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, "Didn't get a fish.");
                                                NET_send(NETplayer, tplayer->net, t);
                                            }
                                            tnpc->wait_disable += 8.0f;
                                        } else {
                                            //wait
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, tnpc->name);
                                            txtadd(t, " busy!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                    }
                                    goto skip_pickup;
                                }

                                if ((tplayer->mobj->type & 1023) == 104) {
                                    //shovel
                                    if (od[y][x] == NULL) {
                                        if (((bt[y][x] & 1023) >= 108) && ((bt[y][x] & 1023) < 112)) {
                                            //on soil
                                            if ((x == 707) && (y == 835)) {
                                                //hole location 707 835
                                                if (tplayer->craft == NULL) {
                                                    if (usinghorse(tplayer) == NULL) {
                                                        if (tplayer->GNPCflags[8] == 1) {
                                                            //know location

                                                            //if another resurrectable body is being carried drop it near entrance
                                                            x5 = 0;
                                                            for (x6 = 0; x6 <= 7; x6++) {
                                                                if (myobj3 = getpartymemberobj(tplayer, x6)) {
                                                                    tnpc3 = (npc *) myobj3->more;
                                                                    tnpc3->upflags |= 32;
                                                                    x5 = OBJlist((object *) tnpc3->items->more);
                                                                    //x5=last object number
                                                                }
                                                            } //!=NULL,x6
                                                            OBJlist_last = NULL;
                                                            for (y6 = 0; y6 < x5; y6++) {
                                                                if ((OBJlist_list[y6]->type & 1023) == 339) {
                                                                    //dead body
                                                                    //scan resurrect list
                                                                    for (i3 = 0; i3 <= nresu; i3++) {
                                                                        if (resu[i3]) {
                                                                            if (resu_body[i3] == OBJlist_list[y6]) {
                                                                                OBJremove(OBJlist_list[y6]);
                                                                                OBJadd(704, 832, OBJlist_list[y6]);
                                                                                goto pirateholefix_bodyremoved;
                                                                            }
                                                                        }
                                                                    }
                                                                pirateholefix_bodyremoved:;
                                                                }
                                                            }

                                                            //transport party to underground
                                                            for (i4 = 7; i4 >= 0; i4--) {
                                                                if (tplayer->party[i4] != NULL) {
                                                                    OBJremove(tplayer->party[i4]);
                                                                    OBJadd(179 + 1024, 211, tplayer->party[i4]);
                                                                } //->party
                                                            } //i4
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You dig a hole.");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                        } else {
                                                            //dont know location
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "Why dig here?");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                        }
                                                        goto skip_pickup;
                                                    } //horse
                                                } //craft
                                            } //hole x,y
                                            if ((tplayer->party[0]->x >= 1024) && (tplayer->party[0]->y < 256)) {
                                                //underground //TODO do real underground check
                                                tnpc = (npc *) tplayer->party[0]->more;
                                                if (tnpc->wait_disable <= 0) {
                                                    if ((rnd * 12.0f) < 1.0f) {
                                                        myobj3 = OBJnew();
                                                        myobj3->type = 89;
                                                        myobj3->more2 = 1;
                                                        //fountain?
                                                        myobj3->info |= 112;
                                                        OBJadd(x, y, myobj3);
                                                    } else {
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(t, "Nothing!");
                                                        NET_send(NETplayer, tplayer->net, t);
                                                    }
                                                    tnpc->wait_disable += 8.0f - ((tplayer->mobj->more2) & 15) * 0.5f;
                                                } else {
                                                    //wait
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, tnpc->name);
                                                    txtadd(t, " busy!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                }
                                            } //underground
                                        } //soil
                                    } //no objects
                                    goto skip_pickup;
                                } //shovel


                                if ((tplayer->mobj->type & 1023) == OBJ_TMAP) {
                                    //treasure map
                                    if (od[y][x] == NULL) {
                                        //if (((bt[y][x]&1023)==1)||(((bt[y][x]&1023)>=52)&&((bt[y][x]&1023)<64))||(((bt[y][x]&1023)>=73)&&((bt[y][x]&1023)<96))||(((bt[y][x]&1023)>=108)&&((bt[y][x]&1023)<112))){
                                        // f111 fix for unable to dig up treasure if it's on a river/coast tile (make it consistent with treasure map creation)
                                        // from TMAP creation: while (((bt[y7+(x2-3)*2][x7+(x1-3)*2]&1023)>=8)&&((bt[y7+(x2-3)*2][x7+(x1-3)*2]&1023)<16)) { //check that the map marker is not in the sea.
                                        x2 = 0;
                                        if (enhancehostn1) {
                                            if (!(((bt[y][x] & 1023) >= 8) && ((bt[y][x] & 1023) < 16)))
                                                x2 = 1;
                                        } else {
                                            if (((bt[y][x] & 1023) == 1) || (
                                                    ((bt[y][x] & 1023) >= 52) && ((bt[y][x] & 1023) < 64)) || (
                                                    ((bt[y][x] & 1023) >= 73) && ((bt[y][x] & 1023) < 96)) || (
                                                    ((bt[y][x] & 1023) >= 108) && ((bt[y][x] & 1023) < 112)))
                                                x2 = 1;
                                        }
                                        //if (((bt[y][x]&1023)==1)||(((bt[y][x]&1023)>=52)&&((bt[y][x]&1023)<64))||(((bt[y][x]&1023)>=73)&&((bt[y][x]&1023)<96))||(((bt[y][x]&1023)>=108)&&((bt[y][x]&1023)<112))){
                                        if (x2) {
                                            tnpc = (npc *) tplayer->party[0]->more;
                                            x2 = 0;
                                            if ((tnpc->wep_right) && (tnpc->wep_right->type & 1023) == OBJ_SHOVEL) {
                                                x2 = 1;
                                            }
                                            if ((tnpc->wep_left) && (tnpc->wep_left->type & 1023) == OBJ_SHOVEL) {
                                                x2 = 2;
                                            }
                                            if (x2) {
                                                if (tnpc->wait_disable <= 0) {
                                                    x3 = 1;
                                                    if (abs((int) (((tplayer->mobj->more2 & 1023) - x) <= 2)) && (abs(
                                                            ((int) ((tplayer->mobj->more2 >> 10) & 1023) - y) <= 2))) {
                                                        x3 = 2 + rnd * 2;
                                                        object *item000001 = OBJaddnew(
                                                            x, y, OBJ_CHEST + 1024 * x3, 112, 0); //chest
                                                        for (x3 = 0; x3 < 24; x3++) {
                                                            x4 = randchestitem();
                                                            x5 = x4 >> 10;
                                                            x4 = x4 & 1023;
                                                            object *item000002 =
                                                                    OBJaddnew(x, y, x4 + 1024 * 0, 112, x5);
                                                            /*for (y7=0;y7<1000000000;y7++) { //for testing the new enhance system
                                ENHANCEnewn(item000002,4,4);
                              }*/
                                                            ENHANCEnewn(item000002, 2, 4);
                                                            OBJremove(item000002);
                                                            //remove from the map (so it can go in a container)
                                                            OBJaddtocontainer(item000001, item000002);
                                                            //put this item inside its parent
                                                        }
                                                        tplayer->mobj = NULL;
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(t, "You dig up a chest.");
                                                        NET_send(NETplayer, tplayer->net, t);
                                                        x3 = 2; //double wait time for digging up the treasure.
                                                    } else {
                                                        //txtset(t,"?"); t->d2[0]=8; txtadd(t,"Nothing!"); NET_send(NETplayer,tplayer->net,t);
                                                        // f111 for incorrect treasure map markers
                                                        if (enhancehostn1) {
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "Nothing! You dig x=");
                                                            txtnumint(t2, x);
                                                            txtadd(t, t2);
                                                            txtadd(t, " y=");
                                                            txtnumint(t2, y);
                                                            txtadd(t, t2);
                                                            txtadd(t, " | Map marker x=");
                                                            txtnumint(t2, tplayer->mobj->more2 & 1023);
                                                            txtadd(t, t2);
                                                            txtadd(t, " y=");
                                                            txtnumint(t2, (tplayer->mobj->more2 >> 10) & 1023);
                                                            txtadd(t, t2);
                                                            NET_send(NETplayer, tplayer->net, t);
                                                        } else {
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "Nothing!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                        }
                                                    }
                                                    if (x2 == 1) {
                                                        tnpc->wait_disable += (
                                                            8.0f - ((tnpc->wep_right->more2) & 15) * 0.5f) * x3;
                                                    } else {
                                                        tnpc->wait_disable += (
                                                            8.0f - ((tnpc->wep_left->more2) & 15) * 0.5f) * x3;
                                                    }
                                                } else {
                                                    //wait
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, tnpc->name);
                                                    txtadd(t, " busy!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                }
                                            } else {
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, "You need to equip a shovel to dig.");
                                                NET_send(NETplayer, tplayer->net, t);
                                            }
                                        }
                                    } //no objects
                                    goto skip_pickup;
                                } //treasure map
                                //other items?


                                goto skip_pickup;
                            } //end use item on basetile

                            if ((tplayer->mobj->type & 1023) == 414) {
                                //skiff drop
                                z2 = 0;
                                if (od[y][x]) z2 = 1;
                                z = bt[y][x] & 1023;
                                if ((z < 8) || (z >= 48)) {
                                    //not water
                                    if ((bt[y][x] & 1024) == 0) {
                                        //not walkable
                                        z2 = 1;
                                    }
                                }
                                if (z2) {
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, "You cannot put a skiff here!");
                                    NET_send(NETplayer, tplayer->net, t);
                                    goto itemdropped;
                                }
                                goto skiffdrop;
                            }

                            if (bt[y][x] & 1024) {
