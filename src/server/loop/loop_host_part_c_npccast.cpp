// LHS-P5.4: part_c_npccast -- CONTINUES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_c_objupdate.cpp.
// Contents: NPC spellcasting (crtcantcast, crt_walk, castheal/castcure,
// cast_spell1/2), npc actions (npcactiondone, paidnpc). Move-only.
                            crtcantcast:


                                if ((abs(x - x2) <= 1) && (abs(y - y2) <= 1)) {
                                    //close ->attack
                                    //lower hp of target

                                    //tnpc=(npc*)crt->target->more; THERE IS NO PURPOSE FOR THIS LINE OF CODE!


                                    if (target_struct == 2) x5 = rnd * (tnpc->arm + 1);
                                    else
                                        x5 = rnd * ((obji[sprlnk[crt->target->type & 1023]].v7 >> 8) + 1 + 1 +
                                                    getequiparmourvalue((object *) crt2->items->more));


                                    //x5=rnd*(tnpc->arm+1);

                                    //x6=rnd*((obji[sprlnk[myobj->type&1023]].v5/2+1)+1);


                                    //old
                                    //x6=rnd*((obji[sprlnk[myobj->type&1023]].v5>>2)+1);

                                    //enemies have the same deficiets on their attacks as players

                                    //x6=rnd*(((obji[sprlnk[myobj->type&1023]].v8&255)+1)*2+1)-((obji[sprlnk[myobj->type&1023]].v8&255)+1);

                                    //damage enemy can do
                                    //x6=rnd*((obji[sprlnk[myobj->type&1023]].v8&255)+1+1); ***LATEST***


                                    x6 = obji[sprlnk[myobj->type & 1023]].v8 & 255;

                                    //GPF!
                                    //if (((object*)crt->items)==NULL) exit( 789);
                                    x7 = getequiph2hwepdamage((object *) crt->items->more);
                                    if (x7 > x6) {
                                        //txtnumint(t7,x6);
                                        //txtadd(t7,"->");
                                        //txtnumint(t8,x7);
                                        //txtadd(t7,t8);
                                        //MessageBox(NULL,t7->d,"Ultima 6 Online",MB_OK);

                                        x6 = x7;
                                    }


                                    x6++;
                                    x6 = rnd * x6;
                                    x6 -= x5;

                                    static unsigned char crtatt_damage = 0;
                                    crtatt_damage = 0;

                                    if (x6 > 0) {
                                        //attack will do damage!


                                        //can NPC dodge the attack?

                                        //generate rnd number up to ?
                                        //if its not 0 you dodged the attack!

                                        //even with dex 64 you still get hit half the time!!!

                                        //generate 2 random numbers
                                        //m is 0-31
                                        //d is 0-tnpc->d
                                        //if d is larger than m you dodged the attack

                                        //if you have 32 dex you'll dodge half attacks on average
                                        //if you have 64 dex you'll dodge all attacks on average!!

                                        //currently 32 dex only dodges 1/4 of attacks


                                        x7 = rnd * 32; //0-127

                                        if (target_struct == 2) x8 = rnd * (tnpc->d + 1);
                                        else x8 = rnd * ((obji[sprlnk[crt->target->type & 1023]].v6 >> 8) + 1);


                                        //if (int(rnd*32)<=int(rnd*((obji[sprlnk[myobj2->type&1023]].v6>>8)+1))){


                                        if (x7 > x8) {
                                            //couldn't dodge the attack!
                                            tnpc->hp -= x6;
                                            crtatt_damage = 1;


                                            //poison target?
                                            x5 = myobj->type & 1023;
                                            x7 = 0;
                                            if (x5 == 354) x7 = 1; //mouse
                                            if (x5 == 342) x7 = 1; //rat
                                            if (x5 == 360) x7 = 1; //rotworms
                                            if (x5 == 426) x7 = 1; //scorp
                                            if (x5 == 413) x7 = 1; //silver serp
                                            if (x5 == 375) x7 = 1; //slime
                                            if (x5 == 358) x7 = 1; //snake
                                            if (x5 == 361) x7 = 1; //spider
                                            if (x5 == 345) x7 = 1; //squid
                                            if (x7) {
                                                x7 = rnd * 32;
                                                if (x7 == 0) {
                                                    if (target_struct == 2) {
                                                        if (tnpc->player) {
                                                            if ((tnpc->flags & 1) == 0) {
                                                                txtset(t, "?");
                                                                t->d2[0] = 8;
                                                                txtadd(t, tnpc->name);
                                                                txtadd(t, " poisoned!");
                                                                NET_send(NETplayer, tnpc->player->net, t);
                                                            } //flags&1
                                                        } //player
                                                        tnpc->upflags |= 2; //hp
                                                    } //2
                                                    tnpc->flags |= 1; //poison NPC
                                                } //x7==0
                                            } //x7

                                            if ((myobj->type & 1023) == 357) {
                                                //corpser
                                                x7 = rnd * 16;
                                                if (x7 == 0) {
                                                    if (target_struct == 2) {
                                                        if (tnpc->player) {
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, tnpc->name);
                                                            txtadd(t, " dragged under!");
                                                            NET_send(NETplayer, tnpc->player->net, t);
                                                        } //player
                                                    } //2
                                                    tnpc->hp = tnpc->hp >> 1;
                                                    crt->wait += 8.0f;
                                                } //x7
                                            } //357


                                            //txtnumint(t5,x6);
                                            //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                                        }
                                    }


                                    //unsigned long upflags; //update NPC flags
                                    //1		name, portriat, str, dex, int, lev, hp_max, mp_max
                                    //2		hp
                                    //4     mp
                                    //8     xp
                                    //16    weight (automatically set if inventory/equipped items change)
                                    //32    inventory, bag icon
                                    //64    equipped items
                                    if (target_struct == 2) tnpc->upflags |= 2; //hp
                                    i3 = SFnew(x2, y2);
                                    sf[i3].type = 1; //attack
                                    if (crtatt_damage == 0) sf[i3].type = 16;
                                    sf[i3].x = x2;
                                    sf[i3].x2 = 0;
                                    if (crtatt_damage) sf[i3].x2 = x6;
                                    sf[i3].y = y2;
                                    sf[i3].wait = 0.125f; //NULL
                                    sf[i3].more = 1;


                                    //change direction
                                    OBJdir(myobj, x2, y2);

                                    f = (float) (obji[sprlnk[myobj->type & 1023]].v6 >> 8) / 64.0f;
                                    if (f > 1) f = 1;
                                    crt->wait += 2.0 - (f * 0.25f);
                                    //crt->wait+=1.0f;
                                } //close
                                else {
                                    //far ->move towards

                                    if ((myobj->type & 1023) == 365) {
                                        //tanglevine
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        VLNKsremove(myobj);
                                        crt->target = NULL;
                                        crt->pathn = 0;
                                        goto donemove;
                                    }
                                    if ((myobj->type & 1023) == 347) {
                                        //reaper
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        VLNKsremove(myobj);
                                        crt->target = NULL;
                                        crt->pathn = 0;
                                        goto donemove;
                                    }
                                    x5 = 1; //flags
                                    if ((myobj->type & 1023) == 345) x5 = 2; //squid
                                    if ((myobj->type & 1023) == 346) x5 = 2; //sea serpent
                                    if ((myobj->type & 1023) == 343) x5 = 3; //insects
                                    if ((myobj->type & 1023) == 344) x5 = 3; //bat
                                    if ((myobj->type & 1023) == 356) x5 = 3; //bird
                                    if ((myobj->type & 1023) == 362) x5 = 3; //winged gargoyle
                                    if ((myobj->type & 1023) == 369) x5 = 3; //drake
                                    if ((myobj->type & 1023) == 372) x5 = 3; //mongbat
                                    if ((myobj->type & 1023) == 373) x5 = 3; //wisp
                                    if ((myobj->type & 1023) == 411) x5 = 3; //dragon
                                    if ((myobj->type & 1023) == 352) x5 = 4; //GHOST (4=unlimited!)
                                    if ((myobj->type & 1023) == 366) {
                                        //tanglevive tendril
                                        x5 = 5;
                                        x4 = rnd * 4;
                                        if (x4) {
                                            f = 0.5f + rnd * 0.5f;
                                            if (crt->wait < f) crt->wait = f;
                                            crt->pathn = 0;
                                            goto donemove;
                                        }
                                    }
                                    /*
                  WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,x2,y2,16,0,0);
                  if (WPF_RETURN==WPF_PATHFOUND){
                  if (WPF_PATHLENGTH){
                  tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
                  playerlist[tpl]->pathfind=1; //use pathfinding formula!
                  goto doneclmess;
                  }
                  playerlist[tpl]->pathfind=0; tnpc->path_max=0; tnpc->pathn=0;
                  }
                  }//party
                  */


                                    if (crt->pathn) goto crt_gotpath;
                                    x4 = 32;
                                    if ((x >= 1024) && (!((y >= 256) && (x < 1280) && (y < 512)))) x4 = 16;
                                    WPF_NEXTTO = 1;
                                    WPF_OBJECT = myobj;
                                    wpf_pathfind(crt->path, x, y, x2, y2, x4, x5, 0);
                                    //SPECIAL NOTE: REAPER AND TANGLEVINE SKIP THE ABOVE LINE OF CODE

                                    x4 = 0;
                                    if (WPF_RETURN == WPF_PATHFOUND) {
                                        if (WPF_PATHLENGTH) {
                                            x4 = 1;
                                            if (WPF_PATHLENGTH > 4) WPF_PATHLENGTH = 4;
                                            crt->path_max = WPF_PATHLENGTH;
                                            crt->pathn = WPF_PATHLENGTH;
                                        }
                                    }


                                    if (x4 == 0) {
                                        //PATHFIND_NEXTTO=1; x4=pathfind(crt->path,x,y,x2,y2,x5);
                                        //x5=28+4; if ((x2>=1024)&&(y2<256)) x5=16;
                                        //if ((x4==0)||(x4>x5)){
                                        crt->pathn = 0;
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        VLNKsremove(myobj);
                                        crt->target = NULL;
                                        crt->pathn = 0;
                                        x2 = rnd * 4;
                                        x3 = x;
                                        y3 = y;
                                        if (x2 == 0) x3--;
                                        if (x2 == 1) x3++;
                                        if (x2 == 2) y3--;
                                        if (x2 == 3) y3++;
                                        goto crt_walk; //wander
                                    }

                                crt_gotpath:
                                    x3 = x;
                                    y3 = y;
                                    /*
                  if (crt->path[0]==PFright) x3++;
                  if (crt->path[0]==PFleft) x3--;
                  if (crt->path[0]==PFup) y3--;
                  if (crt->path[0]==PFdown) y3++;
                  */
                                    if (crt->path[crt->path_max - crt->pathn] == PFright) x3++;
                                    if (crt->path[crt->path_max - crt->pathn] == PFleft) x3--;
                                    if (crt->path[crt->path_max - crt->pathn] == PFup) y3--;
                                    if (crt->path[crt->path_max - crt->pathn] == PFdown) y3++;
                                    crt->pathn--;


                                crt_walk:
                                    if ((myobj->type & 1023) == 366) {
                                        //tendril
                                        if (abs(x3 - crt->respawn_x) >= 24) {
                                            f = 0.5f + rnd * 0.5f;
                                            if (crt->wait < f) crt->wait = f;
                                            goto donemove;
                                        }
                                        if (abs(y3 - crt->respawn_y) >= 24) {
                                            f = 0.5f + rnd * 0.5f;
                                            if (crt->wait < f) crt->wait = f;
                                            goto donemove;
                                        }
                                    }
                                    if ((myobj->type & 1023) == 365) {
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        VLNKsremove(myobj);
                                        crt->target = NULL;
                                        crt->pathn = 0;
                                        goto donemove;
                                    } //tanglevine
                                    if ((myobj->type & 1023) == 347) {
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        VLNKsremove(myobj);
                                        crt->target = NULL;
                                        crt->pathn = 0;
                                        goto donemove;
                                    } //reaper
                                    if (OBJmove2(myobj, x3, y3)) {
                                        //blocked!
                                        f = 0.5f + rnd * 0.5f;
                                        if (crt->wait < f) crt->wait = f;
                                        VLNKsremove(myobj);
                                        crt->target = NULL;
                                        crt->pathn = 0;
                                        goto donemove;
                                    }
                                    f = (float) (obji[sprlnk[myobj->type & 1023]].v6 >> 8) / 64.0f;
                                    if (f > 1.0f) f = 1.0f;
                                    crt->wait += (0.125f + 0.25f - 0.0625f - f * 0.125);
                                } //move towards
                            } //wait
                        } //alive check
                    } //crt!=NULL
                    goto donemove;
                } //crt

                i3 = 0;
                //if ((i2>=376)&&(i2<=387))i3=1;
                //if ((i2>=409)&&(i2<=410))i3=1;
                if (myobj->info & 2) i3 = 1; //2003 has NPC struct

                if (i3 == 1) {
                    //NPC type (player/partymember/NPC)
                    tnpc = (npc *) myobj->more;
                    if (tnpc) {
                        //tnpc!=NULL

                        //walk
                        f = et;
                        if (tnpc->wait_attack || tnpc->wait_cast) f = et / 2.0f;
                        if (tnpc->wait_attack && tnpc->wait_cast) f = et / 3.0f;
                        //cast
                        f2 = et;
                        if (tnpc->wait_walk || tnpc->wait_attack) f2 = et / 1.333333f;
                        if (tnpc->wait_walk && tnpc->wait_attack) f2 = et / 1.666666f;
                        //attack
                        f3 = et;
                        if (tnpc->wait_walk || tnpc->wait_cast) f3 = et / 1.333333f;
                        if (tnpc->wait_walk && tnpc->wait_cast) f3 = et / 1.666666f;


                        if (tnpc->horse) {
                            if (tnpc->player->craft == NULL) {
                                //walk
                                f = et; //walk speed is never slowed by other activities
                                //cast
                                f2 = et;
                                if (tnpc->wait_attack) f2 = et / 1.333333f;
                                //attack
                                f3 = et;
                                if (tnpc->wait_cast) f3 = et / 1.333333f;
                            } //!craft
                        } //tnpc->horse


                        tnpc->wait_walk -= f;
                        if (tnpc->wait_walk < 0) tnpc->wait_walk = 0;
                        f5 = tnpc->wait_cast;
                        tnpc->wait_cast -= f2;
                        if (tnpc->wait_cast < 0) tnpc->wait_cast = 0;
                        tnpc->wait_attack -= f3;
                        if (tnpc->wait_attack < 0) tnpc->wait_attack = 0;
                        tnpc->wait_disable -= et;
                        if (tnpc->wait_disable < 0) tnpc->wait_disable = 0;
                        if (tnpc->flags & 8) {
                            if (tnpc->wait_disable == 0) {
                                tnpc->flags -= 8;
                                if (tnpc->player) {
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, tnpc->name);
                                    txtadd(t, " free!");
                                    NET_send(NETplayer, tnpc->player->net, t);
                                }
                            }
                        }

                        tplayer = tnpc->player;

                        if (tplayer) {
                            if (myobj2 = tplayer->party[tplayer->selected_partymember]) {
                                if (myobj == myobj2) goto npcactiondone; //selected partymember
                            }

                            f = 0.5f;
                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                x2 = rnd * 8;
                                if (!x2) {
                                    if (tnpc->command & (8 + 16)) {
                                        //heal
                                        for (x2 = 0; x2 <= 7; x2++) {
                                            if (myobj2 = tplayer->party[x2]) {
                                                tnpc2 = (npc *) myobj2->more;
                                                if ((tnpc->command & 16) || (tnpc == tnpc2)) {
                                                    if (tnpc2->hp <= (tnpc2->hp_max >> 1)) goto castheal_selected;
                                                }
                                            }
                                        }
                                        goto castheal_done;
                                    castheal_selected:
                                        static object *castheal_target;
                                        castheal_target = myobj2;
                                        x2 = ((0 << 4) + 6); //heal
                                        if (tnpc2->lev >= 3) {
                                            //attempt a great heal
                                            x2 = ((3 << 4) + 4); //great heal
                                        }
                                    castheal_retry:
                                        tpx = myobj2->x;
                                        tpy = myobj2->y;
                                        x = 0;
                                        y = 0;
                                        cast_spell = 1;
                                        goto cast_spell0; //goto spell
                                    cast_spell1: //return #1
                                        if ((i2 != 1) && (i2 != 3)) {
                                            if (castheal_target) {
                                                myobj2 = castheal_target;
                                                tnpc2 = (npc *) myobj2;
                                                castheal_target = NULL;
                                                x2 = ((0 << 4) + 6); //heal
                                                goto castheal_retry;
                                            }
                                        }
                                        castheal_target = NULL;
                                        goto npcactiondone;
                                    } //heal
                                castheal_done:

                                    if (tnpc->command & (32 + 64)) {
                                        //cure
                                        for (x2 = 0; x2 <= 7; x2++) {
                                            if (myobj2 = tplayer->party[x2]) {
                                                tnpc2 = (npc *) myobj2->more;
                                                if ((tnpc->command & 64) || (tnpc == tnpc2)) {
                                                    if (tnpc2->flags & 1) goto castcure_selected;
                                                }
                                            }
                                        }
                                        goto castcure_done;
                                    castcure_selected:
                                        tpx = myobj2->x;
                                        tpy = myobj2->y;
                                        x = 0;
                                        y = 0;
                                        x2 = ((0 << 4) + 3); //cure
                                        cast_spell = 2;
                                        goto cast_spell0; //goto spell
                                    cast_spell2: //return #2
                                        goto npcactiondone;
                                    } //cure
                                castcure_done:

                                    if (tnpc->command & 1) {
                                        //yellowpotion
                                        if (tnpc->hp <= (tnpc->hp_max >> 1)) {
                                            //hp half or less?
                                            //scan inventory for a potion
                                            x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                            OBJlist_last = NULL;
                                            for (y6 = 0; y6 < x5; y6++) {
                                                if (OBJlist_list[y6]->type == (275 + 1024 * 2)) {
                                                    tnpc->hp += 30;
                                                    if (tnpc->hp > tnpc->hp_max) tnpc->hp = tnpc->hp_max;
                                                    tnpc->upflags |= 2;
                                                    OBJremove(OBJlist_list[y6]);
                                                    tnpc->upflags |= 32;
                                                    goto npcactiondone;
                                                } //type
                                            } //y6
                                        } //hp
                                    } //yellowpotion

                                    if (tnpc->command & 2) {
                                        //redpotion
                                        if (tnpc->flags & 1) {
                                            //poisoned
                                            //scan inventory for a potion
                                            x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                            OBJlist_last = NULL;
                                            for (y6 = 0; y6 < x5; y6++) {
                                                if (OBJlist_list[y6]->type == (275 + 1024 * 1)) {
                                                    tnpc->flags &= (65535 - 1); //remove poisoned flag
                                                    tnpc->upflags |= 2;
                                                    OBJremove(OBJlist_list[y6]);
                                                    tnpc->upflags |= 32;
                                                    goto npcactiondone;
                                                } //type
                                            } //y6
                                        } //poisoned
                                    } //redpotion


                                    if (tnpc->command & 4) {
                                        //eat
                                        if (!tnpc->food) {
                                            x6 = OBJlist((object *) tnpc->items->more); //x6=last object number
                                            OBJlist_last = NULL;
                                            for (y6 = 0; y6 < x6; y6++) {
                                                x2 = 0;
                                                x3 = OBJlist_list[y6]->type & 1023;
                                                if (x3 == 95) x2 = 6; //95 grapes 3 B *6
                                                if (x3 == 96) x2 = 1; //96 butter 1 C *1
                                                if (x3 == 115) {
                                                    x2 = 5;
                                                    x5 = 1;
                                                } //115 wine 6 6 5 5 4 6 6 4 4 =5 C *5
                                                if (x3 == 116) {
                                                    x2 = 4;
                                                    x5 = 1;
                                                } //116 mead 4 5 3 2 5 4 4 4 3 =4 C *4
                                                if (x3 == 117) {
                                                    x2 = 4;
                                                    x5 = 1;
                                                } //117 ale 3 7 2 5 3 3 3 4 2 =4 C *4
                                                if (x3 == 128) x2 = 12; //128 bread 3 3 (2 loaves) c A *12
                                                if (x3 == 129) x2 = 16; //129 meat (legs) c 4 3 3 4 4 4 4 4 4 A *16
                                                if (x3 == 130) x2 = 8; //130 rolls 1 2 2 A *m
                                                if (x3 == 131) x2 = 12; //131 cake 8 5 B *12
                                                if (x3 == 132) x2 = 10; //132 cheese 6 5 B *10
                                                if (x3 == 133) x2 = 16; //133 ham (looks like steak) 3 5 A *16
                                                if (x3 == 135) x2 = 28; //135 horse chops (gargoyle food) A *28
                                                if (x3 == 179) {
                                                    x2 = 2;
                                                    x5 = 1;
                                                } //179 bucket of water *this item needs modifying after use (1) B *2
                                                if (x3 == 180) {
                                                    x2 = 8;
                                                    x5 = 1;
                                                } //180 bucket of milk *this item needs modifying after use 3 5 =4 B *8
                                                if (x3 == 184) x2 = 10;
                                                //184 jar of honey *this item needs modifying after use 10 C *10
                                                if (x3 == 209) x2 = 12; //209 small hanging meat 6 B *12
                                                if (x3 == 210) x2 = 28; //210 big hanging meat 9 6 A =7 *28
                                                if (x3 == 265) x2 = 6; //265 fish 3 B *6
                                                if (x2) {
                                                    x2 *= 4;
                                                    tnpc->food = x2;
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtaddchar(t, 255);
                                                    txtadd(t, tnpc->name);
                                                    if (x5) txtadd(t, " drinks the ");
                                                    else txtadd(t, " eats the ");
                                                    y2 = objname[(OBJlist_list[y6]->type & 1023) * 2];
                                                    y3 = objname[(OBJlist_list[y6]->type & 1023) * 2 + 1];
                                                    txtNEWLEN(t2, y3);
                                                    memcpy(&t2->d2[0], &objname2[y2], y3);
                                                    txtadd(t, t2);
                                                    txtadd(t, ".");
                                                    NET_send(NETplayer, tnpc->player->net, t);
                                                    tnpc->upflags |= 32;
                                                    if (obji[sprlnk[OBJlist_list[y6]->type & 1023] + (
                                                                 OBJlist_list[y6]->type >> 10)].flags & 4096) {
                                                        if (OBJlist_list[y6]->more2 != 1) {
                                                            //more than 1!
                                                            OBJlist_list[y6]->more2--;
                                                            goto npcactiondone;
                                                        }
                                                    }
                                                    if ((x3 == 179) || (x3 == 180)) {
                                                        OBJlist_list[y6]->type = 178;
                                                        goto npcactiondone;
                                                    }
                                                    if (x3 == 184) {
                                                        OBJlist_list[y6]->type = 183;
                                                        goto npcactiondone;
                                                    }
                                                    OBJremove(OBJlist_list[y6]);
                                                    goto npcactiondone;
                                                } //x2
                                            } //y6
                                        } //food
                                    } //eat
                                } //8
                            } //0.5
                        npcactiondone:


                            //$rate$
                            //x2=37+3*CONnpc2->lev;
                            //if ((CONnpc2->s+CONnpc2->d+CONnpc2->i)>x2) txtadd(t2,"16"); else txtadd(t2,"8");
                            //deduct gold

                            //1day=1hour of real time
                            if (tnpc->converse == 201) {
                                if ((tnpc->s + tnpc->d + tnpc->i) > (37 + 3 * tnpc->lev)) f = 24.0f;
                                else f = 8.0f;
                                f = 4096.0f / f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                    for (x6 = 0; x6 <= 7; x6++) {
                                        if (tplayer->party[x6]) {
                                            tnpc3 = (npc *) tplayer->party[x6]->more;
                                            if (tnpc3 != tnpc) {
                                                x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                                            }
                                        }
                                    } //!=NULL,x6
                                    OBJlist_last = NULL;

                                    for (y6 = 0; y6 < x5; y6++) {
                                        if (OBJlist_list[y6]->type == 88) {
                                            //gold
                                            OBJlist_list[y6]->more2--;
                                            if (!OBJlist_list[y6]->more2) {
                                                OBJremove(OBJlist_list[y6]);
                                                OBJrelease(OBJlist_list[y6]);
                                            } //more2
                                            for (x6 = 0; x6 <= 7; x6++) {
                                                if (tplayer->party[x6]) {
                                                    tnpc3 = (npc *) tplayer->party[x6]->more;
                                                    tnpc3->upflags |= 32; //inv
                                                }
                                            }
                                            goto paidnpc;
                                        } //88
                                    } //y6
                                    if (tplayer->GNPCflags[64]) tplayer->GNPCflags[64]--; //take payment from bank!
                                } //f
                            } //201
                        paidnpc:

                            if (f5 && (tnpc->wait_cast == 0)) {
                                //but which party member are we??
                                for (i4 = 0; i4 <= 7; i4++) {
                                    if (tplayer->party[i4] == myobj) {
                                        txtset(t, "??");
                                        t->d2[0] = 18;
                                        t->d2[1] = i4;
                                        NET_send(NETplayer, tplayer->net, t); //release cast icon!
                                    } //==myobj
                                } //i4
                            } //f5&&(tnpc->wait_cast==0)


                            if (myobj2 = (object *) myobj->prev) {
                                /* MOONGATES MUST BE USED 2005
                if (myobj2->type==(84+1024)){ //red gate
                if (tplayer->party[0]==myobj){ //primary player
                if (tplayer->selected_partymember==0){
                i4=(myobj2->info&112)>>4;
                if (i4==1){
                for (i4=7;i4>=0;i4--){
                if (tplayer->party[i4]!=NULL){
                OBJremove(tplayer->party[i4]);
                OBJadd(256+128-16,256+128+2,tplayer->party[i4]);
                }
                }
                }//i4
                }//partymember
                }//==party[0]
                }//red gate

                if (myobj2->type==(85+1024)){ //moongate
                if (tplayer->party[0]==myobj){ //primary player
                if (tplayer->selected_partymember==0){
                //move party, if not already at correct phase!
                if (moonphase!=255){
                if ((myobj->x!=moongatex[moonphase])||(myobj->y!=moongatey[moonphase])){
                for (i4=7;i4>=0;i4--){
                if (tplayer->party[i4]!=NULL){
                OBJremove(tplayer->party[i4]);
                OBJadd(moongatex[moonphase],moongatey[moonphase],tplayer->party[i4]);
                }
                }
                }//x!= y!=
                }//moonphase
                }//partymember
                }//==party[0]
                }//moongate
                */

                                if (myobj2->type == 172) {
                                    //spikes
                                    f = 0.5f;
                                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                                        if (tnpc->protection == 0) {
                                            if (tnpc->hp) tnpc->hp--;
                                            if (tnpc->hp) tnpc->hp--;
                                            if (tnpc->hp) tnpc->hp--;
                                            if (tnpc->hp) tnpc->hp--;
                                            i3 = SFnew(myobj->x, myobj->y);
                                            sf[i3].type = 1; //attack
                                            sf[i3].x2 = 4;
                                            sf[i3].x = myobj->x;
                                            sf[i3].y = myobj->y;
                                            sf[i3].wait = 0.125f; //NULL
                                            sf[i3].more = 1;
                                            tnpc->upflags |= 2; //hp
                                        } //protection
                                    } //f
                                } //spikes

                                if (myobj2->type == 173) {
                                    //trap
                                    if ((!myobj2->more2) || (myobj2->more2 == tplayer->id)) {
                                        if (tnpc->protection == 0) {
                                            for (x2 = 0; x2 <= 7; x2++) {
                                                if (tplayer->party[x2] == myobj) {
                                                    if ((myobj->x == tplayer->trapx[x2]) && (
                                                            myobj->y == tplayer->trapy[x2]))
                                                        goto trap_alreadyhurt;
                                                    goto trap_gotpartymemberindex;
                                                }
                                            }
                                            goto trap_alreadyhurt;
