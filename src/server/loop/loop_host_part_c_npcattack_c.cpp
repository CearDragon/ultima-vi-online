// LHS-P5.7: part_c_npcattack_c -- CONTINUES + CLOSES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES + CLOSES the for(i..){ and if(ouln!=-1){ blocks
// opened in loop_host_part_c_objupdate.cpp. Contents: hand-to-hand + spell
// attack (hth_*, spellattcrt0, attackdone, npcattackclose). Move-only.
                                    spellattcrt0:

                                        if (myobj) {
                                            //valid source of attack exists
                                            if (crt->al == 0) {
                                                //passive
                                                if (((myobj2->type & 1023) != 430) || ((crt->flags & 128) == 0)) {
                                                    //horse exception
                                                    crt->al = 3; //passive turned aggressive
                                                    crt->target = myobj;
                                                    VLNKsremove(myobj2); //remove prev vlnk
                                                    VLNKnew(myobj2, crt->target, (unsigned long) &crt->target);
                                                    //add vlnk
                                                } //horse exception
                                            }
                                            if (crt->al == 4) {
                                                //defend itself!
                                                crt->target = myobj;
                                                VLNKsremove(myobj2); //remove prev vlnk
                                                VLNKnew(myobj2, crt->target, (unsigned long) &crt->target); //add vlnk
                                            }
                                        }

                                        if (crt->flags & 4) {
                                            if (x2) {
                                                //wake creature if asleep!
                                                crt->wait = 0;
                                                if (crt->flags & 8) crt->flags -= 8;
                                                //unparalyze //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                                if (crt->flags & 4) crt->flags -= 4;
                                                //awaken //1=poison, 2=invisible, 4=asleep, 8=paralyzed
                                            }
                                        }

                                        crt->hp -= x2;
                                        if (crt->hp < 0) crt->hp = 0;


                                        if ((myobj2->type & 1023) == 366) crt->hp = 32767;
                                        //invincible tanglevine tendril!
                                        if ((myobj2->type & 1023) == 430) if (crt->flags & 128) crt->hp = 32767;
                                        //invincible horse

                                        if (crt->hp <= 0) {
                                            if (tnpc) {
                                                //valid tnpc
                                                tnpc->player->midi = 255; //invalid number!
                                                if (tnpc->player->craft == NULL) {
                                                    //*2004 TEMP

                                                    if (myobj2->type == 355) {
                                                        myobj4 = OBJnew();
                                                        myobj4->type = 343; //insects
                                                        myobj4->more = malloc(sizeof(creature));
                                                        ZeroMemory(myobj4->more, sizeof(creature));
                                                        crt3 = (creature *) myobj4->more;
                                                        crt3->crt_struct = TRUE;
                                                        crt3->hp = (obji[sprlnk[myobj4->type & 1023]].v8 >> 8) * 4;
                                                        crt3->mp = rnd * 9;
                                                        crt3->al = 1;
                                                        crt3->respawn_x = 1024;
                                                        //seconds till spawned crt will disappear
                                                        myobj9 = OBJnew();
                                                        myobj9->type = 188;
                                                        crt3->items = myobj9; //(not included)bag
                                                        myobj4->info |= 4; //<-crt
                                                        OBJadd(myobj2->x, myobj2->y, myobj4);
                                                    }
                                                    myobj4 = NULL;
                                                    if (mover_blood[myobj2->type & 1023]) {
                                                        myobj4 = OBJnew();
                                                        myobj4->type = 338 + (int(rnd * 3) * 1024);
                                                        myobj4->info |= 112;
                                                    }


                                                    if (mover_body[myobj2->type & 1023]) {
                                                        //if it's a cyclops or a winged garg, check surrounding squares for practicality
                                                        //of a body drop
                                                        if ((mover_body[myobj2->type & 1023] == (340 + 1024 * 3)) || (
                                                                mover_body[myobj2->type & 1023] == (341 + 1024 * 3))) {
                                                            //RULE 1: all basetiles must have an object walkable flag
                                                            if ((btflags[bt[myobj2->y][myobj2->x] & 1023] & 1) == 0)
                                                                goto nospaceforbody;
                                                            if ((btflags[bt[myobj2->y - 1][myobj2->x] & 1023] & 1) == 0)
                                                                goto nospaceforbody;
                                                            if ((btflags[bt[myobj2->y][myobj2->x - 1] & 1023] & 1) == 0)
                                                                goto nospaceforbody;
                                                            if ((btflags[bt[myobj2->y - 1][myobj2->x - 1] & 1023] & 1)
                                                                == 0)
                                                                goto nospaceforbody;
                                                            //RULE 2: only 1 object may exist on the square at the time of the drop
                                                            //FACT: the object is a 4 part mover, there WILL be a first object on each square
                                                            if (od[myobj2->y][myobj2->x]->next) goto nospaceforbody;
                                                            if (od[myobj2->y - 1][myobj2->x]->next) goto nospaceforbody;
                                                            if (od[myobj2->y][myobj2->x - 1]->next) goto nospaceforbody;
                                                            if (od[myobj2->y - 1][myobj2->x - 1]->next)
                                                                goto
                                                                        nospaceforbody;
                                                        }
                                                        if (mover_body[myobj2->type & 1023] == (134 + 1024 * 1)) {
                                                            //carcass
                                                            if ((btflags[bt[myobj2->y][myobj2->x] & 1023] & 1) == 0)
                                                                goto nospaceforbody;
                                                            if ((btflags[bt[myobj2->y][myobj2->x - 1] & 1023] & 1) == 0)
                                                                goto nospaceforbody;
                                                            if (od[myobj2->y][myobj2->x]->next) goto nospaceforbody;
                                                            if (od[myobj2->y][myobj2->x - 1]) goto nospaceforbody;
                                                        }

                                                        myobj5 = OBJnew();
                                                        myobj5->type = mover_body[myobj2->type & 1023];
                                                        if ((mover_body[myobj2->type & 1023] >= 432) && (
                                                                mover_body[myobj2->type & 1023] <= 447)) {
                                                            myobj5->type |= (myobj2->type >> 10 << 10);
                                                        }
                                                        myobj5->info |= 112;
                                                        //edit type if/as necessary
                                                        //4 part
                                                        if (((myobj2->type & 1023) == 344) || (
                                                                (myobj2->type & 1023) == 347) || (
                                                                (myobj2->type & 1023) == 353) || (
                                                                (myobj2->type & 1023) == 356)) {
                                                            myobj5->type =
                                                                    (myobj5->type & 1023) + (int(rnd * 4) * 1024);
                                                        }
                                                        //8 part flappers
                                                        if (((myobj2->type & 1023) == 369) || (
                                                                (myobj2->type & 1023) == 372)) {
                                                            if ((myobj5->type & 1024) == 0) {
                                                                myobj5->type = myobj5->type + (int(rnd * 2) * 1024);
                                                            }
                                                        }

                                                        if (mover_blood[myobj2->type & 1023]) {
                                                            if ((mover_body[myobj2->type & 1023] >= 432) && (
                                                                    mover_body[myobj2->type & 1023] <= 447)) {
                                                                myobj4->type = 338 + (((myobj5->type >> 10) % 3) << 10);
                                                            }
                                                        }

                                                        if (myobj4) {
                                                            if (myobj6 = od[myobj2->y][myobj2->x]) {
                                                                od[myobj2->y][myobj2->x] = myobj4;
                                                                myobj4->next = myobj6;
                                                                myobj6->prev = myobj4;
                                                                myobj4->x = myobj2->x;
                                                                myobj4->y = myobj2->y;
                                                            } else {
                                                                OBJadd(myobj2->x, myobj2->y, myobj4);
                                                            }
                                                        }

                                                        /*
                            if (((obj->type&1023)>=432)&&((obj->type&1023)<=447)){//blood of dead crt
                            im32z(ps,GSx,GSy,spr84[3],171+((obj->type>>10)%3));
                            }
                            */

                                                        OBJadd(myobj2->x, myobj2->y, myobj5);


                                                        if ((mover_body[myobj2->type & 1023] == (340 + 1024 * 3)) || (
                                                                mover_body[myobj2->type & 1023] == (341 + 1024 * 3))) {
                                                            myobj4 = OBJnew();
                                                            myobj4->type = myobj5->type - 1024;
                                                            myobj4->info |= 112;
                                                            myobj4->info |= 8;
                                                            myobj4->more = myobj5;
                                                            OBJadd(myobj2->x - 1, myobj2->y, myobj4);
                                                            myobj4 = OBJnew();
                                                            myobj4->type = myobj5->type - 2048;
                                                            myobj4->info |= 112;
                                                            myobj4->info |= 8;
                                                            myobj4->more = myobj5;
                                                            OBJadd(myobj2->x, myobj2->y - 1, myobj4);
                                                            myobj4 = OBJnew();
                                                            myobj4->type = myobj5->type - 3072;
                                                            myobj4->info |= 112;
                                                            myobj4->info |= 8;
                                                            myobj4->more = myobj5;
                                                            OBJadd(myobj2->x - 1, myobj2->y - 1, myobj4);
                                                        }
                                                        if (mover_body[myobj2->type & 1023] == (134 + 1024 * 1)) {
                                                            myobj4 = OBJnew();
                                                            myobj4->type = myobj5->type - 1024;
                                                            myobj4->info |= 112;
                                                            myobj4->info |= 8;
                                                            myobj4->more = myobj5;
                                                            OBJadd(myobj2->x - 1, myobj2->y, myobj4);
                                                        }


                                                        if (myobj4 = (object *) crt->items->more) {
                                                            myobj4->prev = myobj5;
                                                            myobj5->more = myobj4;
                                                            crt->items->more = NULL;
                                                        }
                                                    } else {
                                                    nospaceforbody:

                                                        if (myobj4) {
                                                            if (myobj6 = od[myobj2->y][myobj2->x]) {
                                                                od[myobj2->y][myobj2->x] = myobj4;
                                                                myobj4->next = myobj6;
                                                                myobj6->prev = myobj4;
                                                                myobj4->x = myobj2->x;
                                                                myobj4->y = myobj2->y;
                                                            } else {
                                                                OBJadd(myobj2->x, myobj2->y, myobj4);
                                                            }
                                                        }

                                                        //drop gold/items at death spot!
                                                        myobj4 = (object *) crt->items->more; //will point to first item
                                                    dropgoldmore:
                                                        if (myobj4) {
                                                            myobj5 = (object *) myobj4->next;
                                                            OBJmove_allow = TRUE;
                                                            OBJmove(myobj4, myobj2->x, myobj2->y);
                                                            OBJmove_allow = FALSE;
                                                            myobj4->info |= 112; //set flags as temp OBJ
                                                            if (myobj5) {
                                                                myobj4 = myobj5;
                                                                goto dropgoldmore;
                                                            }
                                                        }
                                                    }

                                                    if (crt->al == 1) {
                                                        //only add exp if agressive

                                                        //amulet of submission check (no exp + karma loss for attacking gargoyles)
                                                        if (((myobj2->type & 1023) == 362) || (
                                                                (myobj2->type & 1023) == 363)) {
                                                            //gargoyle
                                                            if (myobj2->x >= 1024) {
                                                                if (myobj2->y >= 256) {
                                                                    if (myobj2->x < 1280) {
                                                                        if (myobj2->y < 512) {
                                                                            tplayer3 = tnpc->player;
                                                                            if (tplayer3->GNPCflags[12]) {
                                                                                tplayer3->karma -= 2;
                                                                                txtset(
                                                                                    t,
                                                                                    "?You feel less virtuous than before!");
                                                                                t->d2[0] = 8;
                                                                                NET_send(NETplayer, tplayer3->net, t);
                                                                                goto noexp;
                                                                            }
                                                                            for (x2 = 1; x2 <= 7; x2++) {
                                                                                if (tplayer3->party[x2]) {
                                                                                    if (((npc *) tplayer3->party[x2]->
                                                                                        more)->converse == 162) {
                                                                                        tplayer3->karma -= 2;
                                                                                        txtset(
                                                                                            t,
                                                                                            "?You feel less virtuous than before!");
                                                                                        t->d2[0] = 8;
                                                                                        NET_send(
                                                                                            NETplayer, tplayer3->net,
                                                                                            t);
                                                                                        goto noexp;
                                                                                    }
                                                                                } //tplayer3->party[x2]
                                                                            } //x2
                                                                        }
                                                                    }
                                                                }
                                                            } //x,y,x,y
                                                        } //gargoyle

                                                        tnpc->exp += obji[sprlnk[myobj2->type & 1023]].v5;
                                                        //*needs to change
                                                        //Inexperienced character armour bonus changed to 50%
                                                        if (tnpc->player) {
                                                            if (tnpc->player->party[0]) {
                                                                if ((npc *) tnpc->player->party[0]->more == tnpc) {
                                                                    if (tnpc->exp < 1600) {
                                                                        if (tnpc->lev <= 2) {
                                                                            //tnpc->arm+=(20-tnpc->exp/80);
                                                                            tnpc->upflags |= 64;
                                                                            txtset(t, "?");
                                                                            t->d2[0] = 8;
                                                                            txtadd(
                                                                                t,
                                                                                "Inexperienced character armour bonus is ");
                                                                            txtnumint(t2, 100 - tnpc->exp / 16);
                                                                            txtadd(t, t2);
                                                                            txtadd(t, "%");
                                                                            NET_send(NETplayer, tnpc->player->net, t);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        tnpc->upflags |= 8;
                                                    } //al==1
                                                } //->craft
                                            } //tnpc
                                        noexp:

                                            //passive turned agressive cat? if so set global flag
                                            if (tplayer3 = tnpc->player) {
                                                if (crt->al == 3) {
                                                    //passive turned agressive
                                                    if ((myobj2->type & 1023) == 388) {
                                                        //cat
                                                        tplayer3->GNPCflags[36] = 1;
                                                    } //cat
                                                } //3
                                            } //->player
                                            crtrespawn(myobj2);
                                            if (spellattcrt) {
                                                spellattcrt = FALSE;
                                                goto spellattcrt1;
                                            }


                                            //object can now safely be destroyed (if not in update list)

                                            myobj2 = NULL;
                                            goto attackdone;
                                        }
                                        if (spellattcrt) {
                                            spellattcrt = FALSE;
                                            goto spellattcrt1;
                                        }
                                    attack_item_skip2:;
                                    } //(x)
                                } //i3

                            attackdone:

                                if (spikedhelm_attack) {
                                    spikedhelm_attack = 0; //clear set variables
                                    if (myobj2) {
                                        myobj3 = NULL;
                                        i3 = 1;
                                        wep_dt[1] = 1;
                                        x = 4;
                                        goto spikedhelm_entrypoint;
                                    }
                                }


                                if (triplecrossbow_shot) {
                                triplecrossbow_miss:
                                    triplecrossbow_shot--;

                                    if (triplecrossbow_shot) {
                                        //2nd shot
                                        x6 = 4; //75% chance of bolt recovery
                                        myobj2 = OBJfindlast(triplecrossbow_shot_x2, triplecrossbow_shot_y2);
                                        if (myobj2 == NULL)
                                            myobj2 = OBJfindlastall(triplecrossbow_shot_x2,
                                                                    triplecrossbow_shot_y2);
                                        if (myobj2) {
                                            //if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                                            if (myobj2->info & 4) {
                                                //<-crt
                                                x7 = rnd * (tnpc->d + 1);
                                                x8 = rnd * 16;
                                                if (x7 >= x8) {
                                                    //didn't miss!
                                                    i3 = 1;
                                                    wep_dt[0] = 3;
                                                    wep_dt[1] = 3;
                                                    x = 12;

                                                    i4 = SFnew(myobj2->x, myobj2->y);
                                                    sf[i4].type = 1; //attack
                                                    sf[i4].x = triplecrossbow_shot_x2;
                                                    sf[i4].y = triplecrossbow_shot_y2;
                                                    sf[i4].x2 = 0;
                                                    sf[i4].wait = 0.125f; //NULL
                                                    sf[i4].more = 1;

                                                    crt = (creature *) myobj2->more;
                                                    attack_item = 0;
                                                    goto triplecrossbow_refire;
                                                } //didn't miss
                                                x6 = 2; //50% chance of bolt recovery
                                            } //crt
                                        } //myobj2

                                        x7 = rnd * x6;
                                        if (x7) {
                                            //recover bolt
                                            if (myobj2) if (myobj2->info & 2) x6 = 2; //npc
                                            if (x6 == 2) {
                                                //x6: bolt under crt/npc
                                                myobj9 = OBJnew();
                                                AUTOPICKUPadd(myobj, myobj9);
                                                myobj9->type = 56;
                                                myobj9->more2 = 1;
                                                myobj9->prev = myobj2->prev;
                                                myobj9->x = myobj2->x;
                                                myobj9->y = myobj2->y;
                                                if (myobj2->prev) {
                                                    myobj4 = (object *) myobj2->prev;
                                                    myobj4->next = myobj9;
                                                } else {
                                                    od[myobj2->y][myobj2->x] = myobj9;
                                                }
                                                myobj2->prev = myobj9;
                                                myobj9->next = myobj2;
                                                myobj9->info |= 112; //set flags as temp OBJ
                                                myobj9->x = myobj2->x;
                                                myobj9->y = myobj2->y;
                                                OBJcheckflags(myobj9->x, myobj9->y);
                                            } else {
                                                //x6: bolt on ground
                                                if (bt[triplecrossbow_shot_y2][triplecrossbow_shot_x2] & 1024) {
                                                    //walkable
                                                    myobj9 = OBJnew();
                                                    myobj9->type = 56;
                                                    myobj9->more2 = 1;
                                                    myobj9->info = 112;
                                                    OBJadd(triplecrossbow_shot_x2, triplecrossbow_shot_y2, myobj9);
                                                    AUTOPICKUPadd(myobj, myobj9);
                                                } //walkable
                                            } //x6
                                        } //x7

                                        triplecrossbow_shot--;
                                    } //triplecrossbow_shot

                                    //3rd/final shot

                                    /*
                  x7=rnd*(tnpc->d+1); x8=rnd*16; if (x7<x8) goto triplecrossbow_miss2;
                  myobj2=OBJfindlast(triplecrossbow_shot_x3,triplecrossbow_shot_y3); if (myobj2==NULL) myobj2=OBJfindlastall(triplecrossbow_shot_x3,triplecrossbow_shot_y3);
                  if (myobj2){
                  //if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                  if (myobj2->info&4){ //<-crt
                  i3=1;
                  x=12;
                  wep_dt[0]=3; wep_dt[1]=3;
                  i4=SFnew(myobj2->x,myobj2->y);
                  sf[i4].type=1; //attack
                  sf[i4].x=triplecrossbow_shot_x3;
                  sf[i4].y=triplecrossbow_shot_y3;
                  sf[i4].x2=0;
                  sf[i4].wait=0.125f; //NULL
                  sf[i4].more=1;
                  goto triplecrossbow_refire;
                  }
                  }
                  */

                                    x6 = 4; //75% chance of bolt recovery
                                    myobj2 = OBJfindlast(triplecrossbow_shot_x3, triplecrossbow_shot_y3);
                                    if (myobj2 == NULL)
                                        myobj2 = OBJfindlastall(triplecrossbow_shot_x3,
                                                                triplecrossbow_shot_y3);
                                    if (myobj2) {
                                        //if (myobj2->info&8) myobj2=(object*)myobj2->more; //redirect
                                        if (myobj2->info & 4) {
                                            //<-crt
                                            x7 = rnd * (tnpc->d + 1);
                                            x8 = rnd * 16;
                                            if (x7 >= x8) {
                                                //didn't miss!
                                                i3 = 1;
                                                wep_dt[0] = 3;
                                                wep_dt[1] = 3;
                                                x = 12;

                                                i4 = SFnew(myobj2->x, myobj2->y);
                                                sf[i4].type = 1; //attack
                                                sf[i4].x = triplecrossbow_shot_x3;
                                                sf[i4].y = triplecrossbow_shot_y3;
                                                sf[i4].x2 = 0;
                                                sf[i4].wait = 0.125f; //NULL
                                                sf[i4].more = 1;
                                                crt = (creature *) myobj2->more;
                                                attack_item = 0;
                                                goto triplecrossbow_refire;
                                            } //didn't miss
                                            x6 = 2; //50% chance of bolt recovery
                                        } //crt
                                    } //myobj2

                                    x7 = rnd * x6;
                                    if (x7) {
                                        //recover bolt
                                        if (myobj2) if (myobj2->info & 2) x6 = 2; //npc
                                        if (x6 == 2) {
                                            //x6: bolt under crt/npc
                                            myobj9 = OBJnew();
                                            AUTOPICKUPadd(myobj, myobj9);
                                            myobj9->type = 56;
                                            myobj9->more2 = 1;
                                            myobj9->prev = myobj2->prev;
                                            myobj9->x = myobj2->x;
                                            myobj9->y = myobj2->y;
                                            if (myobj2->prev) {
                                                myobj4 = (object *) myobj2->prev;
                                                myobj4->next = myobj9;
                                            } else {
                                                od[myobj2->y][myobj2->x] = myobj9;
                                            }
                                            myobj2->prev = myobj9;
                                            myobj9->next = myobj2;
                                            myobj9->info |= 112; //set flags as temp OBJ
                                            myobj9->x = myobj2->x;
                                            myobj9->y = myobj2->y;
                                            OBJcheckflags(myobj9->x, myobj9->y);
                                        } else {
                                            //x6: bolt on ground
                                            if (bt[triplecrossbow_shot_y3][triplecrossbow_shot_x3] & 1024) {
                                                //walkable
                                                myobj9 = OBJnew();
                                                myobj9->type = 56;
                                                myobj9->more2 = 1;
                                                myobj9->info = 112;
                                                OBJadd(triplecrossbow_shot_x3, triplecrossbow_shot_y3, myobj9);
                                                AUTOPICKUPadd(myobj, myobj9);
                                            } //walkable
                                        } //x6
                                    } //x7
                                } //triplecrossbow_shot: end
                            } //x>0 attack possible
                            else {
                                //attack was NOT possible, close in on target!

                                if (miss) {
                                    //miss!
                                    if (tplayer->craft == NULL) OBJdir(myobj, myobj2->x, myobj2->y);
                                    f = (float) tnpc->d / 64.0f;
                                    if (f > 1.0f) f = 1.0f;
                                    tnpc->wait_attack += 2.0 - (f * 0.25f);
                                    goto skipnpcattack;
                                }

                            attacknotpossible:

                                if (selected_partymember) goto skipnpcattack; //movement is manual
                                if (tnpc->wait_walk) goto skipnpcattack; //can't move closer anyway!

                            npcattackclose:

                                myobj2 = tnpc->target; //activeplayer->party[activeplayer->selected_partymember];
                                x2 = 0;
                                y2 = 0;


                                WPF_NEXTTO = 1;
                                WPF_OBJECT = myobj;
                                wpf_pathfind(tnpc->path, myobj->x, myobj->y, myobj2->x, myobj2->y, 32, 0, 2);
                                x3 = 0;
                                if (WPF_RETURN == WPF_PATHFOUND) {
                                    x3 = WPF_PATHLENGTH;
                                }
                                if (x3 == 0) {
                                    //can't get within rage of target! or ALREADY THERE!
                                    tnpc->order = 1;
                                    tnpc->follow = tplayer->party[tplayer->selected_partymember];
                                    goto skipnpcattack;
                                }


                                /*
                PATHFIND_NEXTTO=1; x3=pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,1);
                if ((x3>32)||(x3==0)) {PATHFIND_NEXTTO=1; x3=pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,NULL);}
                if ((x3>32)||(x3==0)){ //can't get within rage of target! or ALREADY THERE!
                tnpc->order=1; tnpc->follow=tplayer->party[tplayer->selected_partymember]; goto skipnpcattack;
                }
                */

                                if (tnpc->path[0] == PFright) x2 = 1;
                                if (tnpc->path[0] == PFleft) x2 = -1;
                                if (tnpc->path[0] == PFup) y2 = -1;
                                if (tnpc->path[0] == PFdown) y2 = 1;

                                x3 = 0;
                                if (x2) {
                                    if (OBJmove2(myobj, myobj->x + x2, myobj->y) == 0) x3++;
                                }
                                if ((y2) && (x3 == 0)) {
                                    if (OBJmove2(myobj, myobj->x, myobj->y + y2) == 0) x3++;
                                }
                                if (x3) {
                                    f = (float) tnpc->d / 64.0f;
                                    if (f > 1.0f) f = 1.0f;
                                    tnpc->wait_walk += (0.25f - 0.0625f - f * 0.125f);
                                }
                            } //close in on target (couldn't attack)

                        skipnpcattack:;
                        } //end attack


                        //} //NPC, not selected partymember
                    NPC_skip1:; //NPC (not player/party) skip
                    } //CRITICAL ERROR CORRECT
                } //NPCtype

            donemove:;

            skip_ouli:;
            } //oul[i]!=NULL
        } //i
    } //oul!=0
