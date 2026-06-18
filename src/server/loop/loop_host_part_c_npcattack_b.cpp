// LHS-P5.7: part_c_npcattack_b -- CONTINUES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_c_objupdate.cpp.
// Contents: ranged/thrown NPC attack (crossbow0, oilflask_new). Move-only.
                                                        crossbow0:
                                                            if (x6 < 0 || x6 >= 2048) x6 = myobj2->x;
                                                            if (y6 < 0 || y6 >= 1024) y6 = myobj2->y;
                                                            if (x7 < 0 || x7 >= 2048) x7 = myobj2->x;
                                                            if (y7 < 0 || y7 >= 1024) y7 = myobj2->y;
                                                            triplecrossbow_shot_x2 = x6;
                                                            triplecrossbow_shot_y2 = y6;
                                                            triplecrossbow_shot_x3 = x7;
                                                            triplecrossbow_shot_y3 = y7;

                                                            i3 = SFnew(myobj2->x, myobj2->y);
                                                            //destination is more important than the source
                                                            sf[i3].type = 6; //bolt
                                                            sf[i3].x = myobj->x;
                                                            sf[i3].y = myobj->y;
                                                            sf[i3].x2 = x6;
                                                            sf[i3].y2 = y6;
                                                            sf[i3].more = 0xFFFF;
                                                            sf[i3].wait = 1;

                                                            i3 = SFnew(myobj2->x, myobj2->y);
                                                            //destination is more important than the source
                                                            sf[i3].type = 6; //bolt
                                                            sf[i3].x = myobj->x;
                                                            sf[i3].y = myobj->y;
                                                            sf[i3].x2 = x7;
                                                            sf[i3].y2 = y7;
                                                            sf[i3].more = 0xFFFF;
                                                            sf[i3].wait = 1;


                                                            x += obji[sprlnk[myobj3->type & 1023]].v1 +
                                                                    ENHANCEget_attack(myobj3);
                                                            if (x3) wep_dt[0] = 3;
                                                            else wep_dt[1] = 3;

                                                            goto skip_hth2;
                                                        } //3 arrows
                                                    } //->type==56
                                                } //y6
                                                goto skipnpcattack;
                                            } //OBJcheckbolt
                                            goto skip_hth2;
                                        } //triple crossbow


                                        if ((myobj3->type & 1023) == 33) {
                                            //sling
                                            if (x2 <= 4) {
                                                //short range
                                                if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                    //it would hit the wall (ignore)
                                                } else {
                                                    i3 = SFnew(myobj2->x, myobj2->y);
                                                    //destination is more important than the source
                                                    sf[i3].type = 13; //arrow
                                                    sf[i3].x = myobj->x;
                                                    sf[i3].y = myobj->y;
                                                    sf[i3].x2 = myobj2->x;
                                                    sf[i3].y2 = myobj2->y;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(
                                                        myobj3);
                                                } //OBJcheckbolt
                                            } //short range
                                            goto skip_hth2;
                                        } //sling

                                        if ((myobj3->type & 1023) == 79) {
                                            //lightning wand
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                //u6ob
                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                //destination is more important than the source
                                                sf[i3].type = 5; //wand
                                                sf[i3].x = myobj->x;
                                                sf[i3].y = myobj->y;
                                                sf[i3].x2 = myobj2->x;
                                                sf[i3].y2 = myobj2->y;
                                                sf[i3].more = 0xFFFF;
                                                sf[i3].wait = 1;
                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);
                                                if (x3) wep_dt[0] = 5;
                                                else wep_dt[1] = 5;
                                                //destroy item???
                                                x8 = myobj3->more2;


                                                if (x8 == 0) x8 = 256;
                                                x8--;
                                                myobj3->more2 = x8;
                                                if (x8 == 0) {
                                                    //if (x3==0) {OBJrelease(tnpc->wep_right); tnpc->wep_right=NULL;} //right hand weapon
                                                    //if (x3==1) {OBJrelease(tnpc->wep_left); tnpc->wep_left=NULL;} //left hand weapon
                                                    if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                    if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon
                                                    tnpc->upflags |= 64;
                                                }
                                            }
                                            goto skip_hth2;
                                        }

                                        if ((myobj3->type & 1023) == 80) {
                                            //fire wand
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                //u6ob
                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                //destination is more important than the source
                                                sf[i3].type = 8; //wand
                                                sf[i3].x = myobj->x;
                                                sf[i3].y = myobj->y;
                                                sf[i3].x2 = myobj2->x;
                                                sf[i3].y2 = myobj2->y;
                                                sf[i3].more = 0xFFFF;
                                                sf[i3].wait = 1;
                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);
                                                if (x3) wep_dt[0] = 5;
                                                else wep_dt[1] = 5;
                                                //destroy item???
                                                x8 = myobj3->more2;
                                                if (x8 == 0) x8 = 256;
                                                x8--;
                                                myobj3->more2 = x8;

                                                if (x8 == 0) {
                                                    //if (x3==0) {OBJrelease(tnpc->wep_right); tnpc->wep_right=NULL;} //right hand weapon
                                                    //if (x3==1) {OBJrelease(tnpc->wep_left); tnpc->wep_left=NULL;} //left hand weapon
                                                    if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                    if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon

                                                    tnpc->upflags |= 64;
                                                }
                                            }
                                            goto skip_hth2;
                                        }


                                        if ((myobj3->type & 1023) == 42) {
                                            //crossbow
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                OBJlist_last = NULL;
                                                for (y6 = 0; y6 < x5; y6++) {
                                                    if (OBJlist_list[y6]->type == 56) {
                                                        //bolt
                                                        OBJlist_list[y6]->more2--;
                                                        if (OBJlist_list[y6]->more2 == 0) OBJremove(OBJlist_list[y6]);
                                                        tnpc->upflags |= 32;
                                                        i3 = SFnew(myobj2->x, myobj2->y);
                                                        //destination is more important than the source
                                                        sf[i3].type = 6; //crossbow
                                                        sf[i3].x = myobj->x;
                                                        sf[i3].y = myobj->y;
                                                        sf[i3].x2 = myobj2->x;
                                                        sf[i3].y2 = myobj2->y;
                                                        sf[i3].more = 0xFFFF;
                                                        sf[i3].wait = 1;
                                                        x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(
                                                            myobj3);
                                                        if (x3) wep_dt[0] = 3;
                                                        else wep_dt[1] = 3;
                                                        goto skip_hth2;
                                                    } //==56
                                                } //y6
                                                goto skipnpcattack;
                                            }
                                            goto skip_hth2;
                                        }

                                        if ((myobj3->type & 1023) == 49) {
                                            //boomerang
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                //u6ob
                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                //destination is more important than the source
                                                sf[i3].type = 7; //boomerang
                                                sf[i3].x = myobj->x;
                                                sf[i3].y = myobj->y;
                                                sf[i3].x2 = myobj2->x;
                                                sf[i3].y2 = myobj2->y;
                                                sf[i3].more = 0xFFFF;
                                                sf[i3].wait = 2;
                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);

                                                //drop it by accident?
                                                x8 = rnd * (tnpc->d + 1);
                                                if (x8 == 0) {
                                                    if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                    if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon
                                                    //insert myobj3
                                                    myobj3->prev = myobj2->prev;
                                                    myobj3->x = myobj2->x;
                                                    myobj3->y = myobj2->y;
                                                    if (myobj2->prev) {
                                                        myobj4 = (object *) myobj2->prev;
                                                        myobj4->next = myobj3;
                                                    } else {
                                                        od[myobj2->y][myobj2->x] = myobj3;
                                                    }
                                                    myobj2->prev = myobj3;
                                                    myobj3->next = myobj2;
                                                    myobj3->info |= 112; //set flags as temp OBJ
                                                    AUTOPICKUPadd(myobj, myobj3);
                                                    tnpc->upflags |= 64;
                                                    //equip new one
                                                    x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                    OBJlist_last = NULL;
                                                    for (y6 = 0; y6 < x5; y6++) {
                                                        if (OBJlist_list[y6]->type == 49) {
                                                            //boomerang
                                                            OBJremove(OBJlist_list[y6]);
                                                            if (x3 == 0) tnpc->wep_right = OBJlist_list[y6];
                                                            //right hand weapon
                                                            if (x3 == 1) tnpc->wep_left = OBJlist_list[y6];
                                                            //left hand weapon
                                                            tnpc->upflags |= 32;
                                                            goto boomerang_new;
                                                        }
                                                    }
                                                boomerang_new:;
                                                }
                                            }
                                            goto skip_hth2;
                                        } //boomerang


                                        if ((myobj3->type & 1023) == 83) {
                                            //oil flask
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                //u6ob

                                                i3 = SFnew(myobj2->x, myobj2->y);
                                                //destination is more important than the source
                                                sf[i3].type = 9; //GENERIC throw object
                                                sf[i3].x = myobj->x;
                                                sf[i3].y = myobj->y;
                                                sf[i3].x2 = myobj2->x;
                                                sf[i3].y2 = myobj2->y;
                                                sf[i3].more = 0xFFFF;
                                                sf[i3].wait = 83;

                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);
                                                //toss it!
                                                //x9=rnd*32;
                                                //if (x9==0){

                                                if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon


                                                myobj9 = OBJnew();
                                                myobj9->type = 317; //fire
                                                //insert object9
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

                                                myobj9->more2 = tnpc->player->id;
                                                myobj9->info &= (0xFFFF - 112);
                                                myobj9->info |= (2 << 4);

                                                myobj9->x = myobj2->x;
                                                myobj9->y = myobj2->y;
                                                OBJcheckflags(myobj9->x, myobj9->y);


                                                //equip new one?
                                                x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                OBJlist_last = NULL;
                                                for (y6 = 0; y6 < x5; y6++) {
                                                    if (OBJlist_list[y6]->type == 83) {
                                                        //flask
                                                        OBJlist_list[y6]->more2--;
                                                        if (OBJlist_list[y6]->more2 == 0) {
                                                            OBJremove(OBJlist_list[y6]);
                                                        }

                                                        myobj9 = OBJnew();
                                                        myobj9->type = 83;
                                                        myobj9->more2 = 1;

                                                        if (x3 == 0) tnpc->wep_right = myobj9; //right hand weapon
                                                        if (x3 == 1) tnpc->wep_left = myobj9; //left hand weapon

                                                        tnpc->upflags |= 32;
                                                        goto oilflask_new;
                                                    }
                                                }
                                            oilflask_new:


                                                tnpc->upflags |= 64;
                                                //32    inventory, bag icon
                                                //64    equipped items
                                                //}
                                            }
                                            goto skip_hth2;
                                        }


                                        if ((myobj3->type & 1023) == 38) {
                                            //dagger
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //blocked!
                                            } else {
                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);
                                                if (x2 > 1) {
                                                    //throw
                                                    i3 = SFnew(myobj2->x, myobj2->y);
                                                    //destination is more important than the source
                                                    sf[i3].type = 10; //dagger
                                                    sf[i3].x = myobj->x;
                                                    sf[i3].y = myobj->y;
                                                    sf[i3].x2 = myobj2->x;
                                                    sf[i3].y2 = myobj2->y;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                    if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon
                                                    //insert object3
                                                    myobj3->prev = myobj2->prev;
                                                    myobj3->x = myobj2->x;
                                                    myobj3->y = myobj2->y;
                                                    if (myobj2->prev) {
                                                        myobj4 = (object *) myobj2->prev;
                                                        myobj4->next = myobj3;
                                                    } else {
                                                        od[myobj2->y][myobj2->x] = myobj3;
                                                    }
                                                    myobj2->prev = myobj3;
                                                    myobj3->next = myobj2;
                                                    myobj3->info |= 112; //set flags as temp OBJ
                                                    AUTOPICKUPadd(myobj, myobj3);
                                                    tnpc->upflags |= 64;
                                                    //equip new one?
                                                    x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                    OBJlist_last = NULL;
                                                    for (y6 = 0; y6 < x5; y6++) {
                                                        if (OBJlist_list[y6]->type == 38) {
                                                            //dagger
                                                            OBJremove(OBJlist_list[y6]);
                                                            if (x3 == 0) tnpc->wep_right = OBJlist_list[y6];
                                                            //right hand weapon
                                                            if (x3 == 1) tnpc->wep_left = OBJlist_list[y6];
                                                            //left hand weapon
                                                            tnpc->upflags |= 32;
                                                            goto dagger_new;
                                                        }
                                                    }
                                                dagger_new:;
                                                } else { goto skip_hthx; } //throw
                                            }
                                            goto skip_hth2;
                                        }

                                        if ((myobj3->type & 1023) == 37) {
                                            //throwing axe
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //blocked!
                                            } else {
                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);
                                                if (x2 > 1) {
                                                    //throw
                                                    i3 = SFnew(myobj2->x, myobj2->y);
                                                    //destination is more important than the source
                                                    sf[i3].type = 11; //throwing axe
                                                    sf[i3].x = myobj->x;
                                                    sf[i3].y = myobj->y;
                                                    sf[i3].x2 = myobj2->x;
                                                    sf[i3].y2 = myobj2->y;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                    if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon
                                                    //insert object3
                                                    myobj3->prev = myobj2->prev;
                                                    myobj3->x = myobj2->x;
                                                    myobj3->y = myobj2->y;
                                                    if (myobj2->prev) {
                                                        myobj4 = (object *) myobj2->prev;
                                                        myobj4->next = myobj3;
                                                    } else {
                                                        od[myobj2->y][myobj2->x] = myobj3;
                                                    }
                                                    myobj2->prev = myobj3;
                                                    myobj3->next = myobj2;
                                                    myobj3->info |= 112; //set flags as temp OBJ
                                                    AUTOPICKUPadd(myobj, myobj3);
                                                    tnpc->upflags |= 64;
                                                    //equip new one?
                                                    x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                    OBJlist_last = NULL;
                                                    for (y6 = 0; y6 < x5; y6++) {
                                                        if (OBJlist_list[y6]->type == 37) {
                                                            //throwing axe
                                                            OBJremove(OBJlist_list[y6]);
                                                            if (x3 == 0) tnpc->wep_right = OBJlist_list[y6];
                                                            //right hand weapon
                                                            if (x3 == 1) tnpc->wep_left = OBJlist_list[y6];
                                                            //left hand weapon
                                                            tnpc->upflags |= 32;
                                                            goto axe_new;
                                                        }
                                                    }
                                                axe_new:;
                                                } else { goto skip_hthx; } //throw
                                            }
                                            goto skip_hth2;
                                        }

                                        if ((myobj3->type & 1023) == 36) {
                                            //spear
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //blocked!
                                            } else {
                                                x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(myobj3);
                                                if (x2 > 1) {
                                                    //throw
                                                    i3 = SFnew(myobj2->x, myobj2->y);
                                                    //destination is more important than the source
                                                    sf[i3].type = 12; //spear
                                                    sf[i3].x = myobj->x;
                                                    sf[i3].y = myobj->y;
                                                    sf[i3].x2 = myobj2->x;
                                                    sf[i3].y2 = myobj2->y;
                                                    sf[i3].more = 0xFFFF;
                                                    sf[i3].wait = 1;
                                                    if (x3 == 0) tnpc->wep_right = NULL; //right hand weapon
                                                    if (x3 == 1) tnpc->wep_left = NULL; //left hand weapon
                                                    //insert object3
                                                    myobj3->prev = myobj2->prev;
                                                    myobj3->x = myobj2->x;
                                                    myobj3->y = myobj2->y;
                                                    if (myobj2->prev) {
                                                        myobj4 = (object *) myobj2->prev;
                                                        myobj4->next = myobj3;
                                                    } else {
                                                        od[myobj2->y][myobj2->x] = myobj3;
                                                    }
                                                    myobj2->prev = myobj3;
                                                    myobj3->next = myobj2;
                                                    myobj3->info |= 112; //set flags as temp OBJ
                                                    AUTOPICKUPadd(myobj, myobj3);

                                                    tnpc->upflags |= 64;
                                                    //equip new one?
                                                    x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                    OBJlist_last = NULL;
                                                    for (y6 = 0; y6 < x5; y6++) {
                                                        if (OBJlist_list[y6]->type == 36) {
                                                            //spear
                                                            OBJremove(OBJlist_list[y6]);
                                                            if (x3 == 0) tnpc->wep_right = OBJlist_list[y6];
                                                            //right hand weapon
                                                            if (x3 == 1) tnpc->wep_left = OBJlist_list[y6];
                                                            //left hand weapon
                                                            tnpc->upflags |= 32;
                                                            goto spear_new;
                                                        }
                                                    }
                                                spear_new:;
                                                } else { goto skip_hthx; } //throw
                                            }

                                            goto skip_hth2;
                                        }

                                        goto skip_hthx; //not long range!


                                    skip_hth2: //skipped other weapon types, projectile weapon

                                        //miss?
                                        if (x) {
                                            //can do damage
                                            x7 = rnd * (tnpc->d + 1);
                                            x8 = rnd * 16;
                                            /* this should be dependant on target being attacked I think and with some weapons it should be easier to hit (ranged weapons need higher dex and heavy melee weapons lower) */
                                            if (x7 < x8) {
                                                x = 0;
                                                miss = 1;

                                                //if bow, magic bow, or crossbow, drop an arrow at the destination 50% of the time
                                                if ((npcattackweptype == 41) || (npcattackweptype == 54) || (
                                                        npcattackweptype == 42) || (npcattackweptype == 50)) {
                                                    x7 = rnd * 2;
                                                    if (x7) {
                                                        //50% chance
                                                        myobj9 = OBJnew();
                                                        AUTOPICKUPadd(myobj, myobj9);
                                                        myobj9->type = 55; //arrow
                                                        if ((npcattackweptype == 42) || (npcattackweptype == 50))
                                                            myobj9->type = 56; //bolt
                                                        myobj9->more2 = 1;
                                                        //insert object9
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
                                                    } //x7
                                                } //npcattackweptype

                                                if (triplecrossbow_shot) {
                                                    if (tnpc->wait_attack || tnpc->wait_disable) goto skipnpcattack;
                                                    f = (float) tnpc->d / 64.0f;
                                                    if (f > 1.0f) f = 1.0f;
                                                    tnpc->wait_attack += 2.0 - (f * 0.25f);
                                                    goto triplecrossbow_miss;
                                                }
                                            } //missed!
                                        } //x
                                        goto skip_hth1;
                                    } //x2<=8 (long range)

                                skip_hthx: //not a long range weapon

                                    if (x2 <= 1) {
                                    hth_norangecheck:
                                        if (x == 0)
                                            x += obji[sprlnk[myobj3->type & 1023]].v1 + ENHANCEget_attack(
                                                myobj3);
                                        if (myobj3->type == 48) x = 1023;
                                    } //x2<=1
                                } //myobj3

                                if ((x3 == 1) && (x == 0) && (x9 == 0) && (x2 <= 1)) {
                                    //attack with bare hands!
                                    wep_dt[1] = 1;
                                    x = 1;
                                }

                            skip_hth1:;


                                if (x3 == 0) {
                                    x9 = x;
                                    x = 0;
                                }
                            } //x3


                            if (x || x9) {
                                //attack possible

                                //ERROR! it fires the arrow but can't actually attack because attack waiting in ON!
                                //this could cause hell for NPC!
                                //


                                if (tplayer->craft == NULL) OBJdir(myobj, myobj2->x, myobj2->y);

                                f = (float) tnpc->d / 64.0f;
                                if (f > 1.0f) f = 1.0f;
                                f = 2.0f - (f * 0.25f);
                                if (spikedhelm_attack) {
                                    if (x && x9) f *= 1.25f;
                                    else f *= 1.125f;
                                } else {
                                    if (x && x9) f *= 1.125f;
                                }
                                tnpc->wait_attack += f;

                                i4 = SFnew(myobj2->x, myobj2->y);
                                sf[i4].type = 1; //attack
                                sf[i4].x = myobj2->x;
                                sf[i4].y = myobj2->y;
                                sf[i4].x2 = 0;
                                sf[i4].wait = 0.125f; //NULL
                                sf[i4].more = 1;


                                for (i3 = 0; i3 <= 1; i3++) {
                                    if (i3) x = x9;
                                    if (x) {
                                        if (i3 == 0) myobj3 = tnpc->wep_left;
                                        else myobj3 = tnpc->wep_right;
                                        //NOTE: this value may not point to an actual weapon!
                                    triplecrossbow_refire:
                                    spikedhelm_entrypoint:


                                        if (attack_item) {
                                            if (myobj2->type == 53) {
                                                if (myobj3) {
                                                    if (myobj3->type == 48) {
                                                        //smash glass sword!
                                                        if (i3 == 0) tnpc->wep_left = NULL;
                                                        else tnpc->wep_right = NULL;
                                                        tnpc->upflags |= 64;
                                                    }
                                                }

                                                //web strength
                                                x2 = rnd * (x + 1); //attack strength
                                                x3 = rnd * (128 + 1); //web strength
                                                if (x2 > x3) {
                                                    OBJsave(myobj2->x, myobj2->y);
                                                    VLNKremove(myobj2);
                                                    OBJremove(myobj2);
                                                    myobj2 = NULL;
                                                    goto attackdone;
                                                } //x2>x3
                                            } //53

                                            if ((myobj2->type & 1023) == 98) {
                                                //chest
                                                x2 = 0;
                                                if (myobj2->type <= (98 + 1024)) x2 = 1; //closed/opened
                                                if (myobj2->type == (98 + 2048)) x2 = 1; //locked
                                                if (x2) {
                                                    if (myobj3) {
                                                        if (myobj3->type == 48) {
                                                            //smash glass sword!
                                                            if (i3 == 0) tnpc->wep_left = NULL;
                                                            else tnpc->wep_right = NULL;
                                                            tnpc->upflags |= 64;
                                                        }
                                                    }

                                                    x2 = rnd * (x + 1); //attack 
                                                    x3 = rnd * (256 + 1); //chest
                                                    if (x2 > x3) {
                                                        x3 = myobj2->x;
                                                        y3 = myobj2->y;
                                                        OBJsave(x3, y3);

                                                        VLNKremove(myobj2);
                                                        //VLNKsremove(myobj2);
                                                        OBJremove(myobj2);

                                                        //extract items
                                                        myobj = myobj2;
                                                    extractobj5:
                                                        myobj2 = (object *) myobj->more;
                                                        if (myobj2 != NULL) {
                                                            OBJremove(myobj2);
                                                            myobj2->info |= 112;
                                                            OBJadd(x3, y3, myobj2);
                                                            goto extractobj5;
                                                        }
                                                        myobj2 = NULL;
                                                        goto attackdone;
                                                    } //x2
                                                } //x2>x3
                                            } //98

                                            goto attack_item_skip2;
                                        }


                                        if (myobj3) {
                                            if (myobj3->type == OBJ_FIRE_WAND) {
                                                //fire wand
                                                x2 = myobj2->type & 1023;
                                                if (x2 == 411) x = 0; //dragon
                                                if (x2 == 369) x = 0; //drake
                                                if (x2 == 352) x = 0; //ghost
                                                if (x2 == 374) x = 0; //hydra
                                                if (x2 == 369) x = 0; //drake
                                                if (x2 == 368) x = 0; //skeleton
                                                if (x2 == 373) x = 0; //wisp
                                                if (x2 == 367) x = 0; //daemon
                                                //double damage?
                                                if (x2 == 364) x *= 2; //acid slug
                                                if (x2 == 357) x *= 2; //corpser
                                                if (x2 == 347) x *= 2; //reaper
                                                if (x2 == 360) x *= 2; //rot worms
                                                if (x2 == 375) x *= 2; //slime
                                                if (x2 == 365) x *= 2; //tangle vine
                                            }
                                        }


                                        //slime divide!
                                        if ((myobj2->type & 1023) == OBJ_SLIME) {
                                            static unsigned long slimex, slimey;
                                            if ((myobj2->info & (15 << 9)) == 0) goto slimedivide_failed;
                                            //test surrounding squares
                                            slimex = myobj2->x + 1;
                                            slimey = myobj2->y;
                                            if (slimex < 2048 && slimey < 1024)
                                                if (
                                                    (od[slimey][slimex] == NULL) && ((bt[slimey][slimex] >> 10) & 1))
                                                    goto
                                                            slimedivide;
                                            slimex = myobj2->x - 1;
                                            slimey = myobj2->y;
                                            if (slimex < 2048 && slimey < 1024)
                                                if (
                                                    (od[slimey][slimex] == NULL) && ((bt[slimey][slimex] >> 10) & 1))
                                                    goto
                                                            slimedivide;
                                            slimex = myobj2->x;
                                            slimey = myobj2->y + 1;
                                            if (slimex < 2048 && slimey < 1024)
                                                if (
                                                    (od[slimey][slimex] == NULL) && ((bt[slimey][slimex] >> 10) & 1))
                                                    goto
                                                            slimedivide;
                                            slimex = myobj2->x;
                                            slimey = myobj2->y - 1;
                                            if (slimex < 2048 && slimey < 1024)
                                                if (
                                                    (od[slimey][slimex] == NULL) && ((bt[slimey][slimex] >> 10) & 1))
                                                    goto
                                                            slimedivide;
                                            goto slimedivide_failed;
                                        slimedivide: {
                                                object *newSlimeObj = OBJnew();
                                                newSlimeObj->type = 375;
                                                newSlimeObj->more = malloc(sizeof(creature));
                                                ZeroMemory(newSlimeObj->more, sizeof(creature));
                                                crt3 = (creature *) newSlimeObj->more;
                                                crt3->crt_struct = TRUE;
                                                crt3->hp = (obji[sprlnk[newSlimeObj->type & 1023]].v8 >> 8) * 4;
                                                crt3->mp = 0;
                                                crt3->al = 1;
                                                crt3->respawn_x = 1024; //seconds till spawned crt will disappear
                                                myobj9 = OBJnew();
                                                myobj9->type = 188;
                                                crt3->items = myobj9; //(not included)bag
                                                newSlimeObj->info |= 4; //<-crt
                                                OBJmove2(newSlimeObj, slimex, slimey);
                                                x2 = (myobj2->info >> 9) & 15;
                                                x2--;
                                                myobj2->info &= (0xFFFF - (15 << 9));
                                                myobj2->info |= (x2 << 9);
                                                newSlimeObj->info |= (x2 << 9);
                                            }
                                        slimedivide_failed:;
                                        }


                                        f = x; //default (unclassified) bonus
                                        //OLD VALUES
                                        //if (wep_dt[i3]==1) f=(float)x/32.0f*(float)tnpc->s; //100% STR
                                        //if (wep_dt[i3]==2) f=(float)x/32.0f*(((float)tnpc->s/2.0f)+((float)tnpc->d/2.0f)); //50% STR + 50% DEX
                                        //if (wep_dt[i3]==3) f=(float)x/32.0f*((float)tnpc->d*2.0f); //200% DEX (bows, crossbows)
                                        if (wep_dt[i3] == 1)
                                            f = (float) x / 32.0f * ((float) tnpc->s + (float) tnpc->d * 0.25f);
                                        //STR*100%+DEX*25%
                                        if (wep_dt[i3] == 2)
                                            f = (float) x / 32.0f * ((float) tnpc->d * 0.75f + (float) tnpc->s * 0.25f);
                                        //DEX*75%+STR*25%
                                        if (wep_dt[i3] == 3) f = (float) x / 32.0f * ((float) tnpc->d * 1.75f);
                                        //DEX*175%
                                        if (wep_dt[i3] == 4) {
                                            //DEX*100% or STR*100%, whichever is lower
                                            if (tnpc->s <= tnpc->d) {
                                                f = (float) x / 32.0f * (
                                                        (float) tnpc->s + ((float) tnpc->d - (float) tnpc->s) * 0.25f);
                                            } else {
                                                f = (float) x / 32.0f * (
                                                        (float) tnpc->d + ((float) tnpc->s - (float) tnpc->d) * 0.25f);
                                            }
                                        }
                                        if (wep_dt[i3] == 5) f = (float) x / 32.0f * ((float) tnpc->i * 0.125f + 8);
                                        //INT*12.5% wands are magical weapons, so they should have some base damage bonus in u6 they were quite good */
                                        x2 = rnd * ((float) (x * 2) + f + 1.0f) - (float) x; //weapon damage
                                        if (x2 > x) x2 = x;


                                        if (tplayer->craft) x2 = rnd * 31.0f; //cannons (can't graze target!)

                                        x2 -= (int) (
                                            rnd * ((obji[sprlnk[myobj2->type & 1023]].v7 >> 8) + 1 + 1 +
                                                   getequiparmourvalue((object *) crt->items->more))); //enemy arm +1
                                        if (x2 < 0) x2 = 0;

                                        if (tplayer->craft) goto nododge;
                                        if (wep_dt[i3] == 3) goto nododge;
                                        if (int(rnd * 32) <= int(
                                                rnd * ((obji[sprlnk[myobj2->type & 1023]].v6 >> 8) + 1))) {
                                            x2 = 0; //enemy dodged the attack!
                                        }
                                    nododge:

                                        sf[i4].x2 += x2;
                                        if (sf[i4].x2 == 0) sf[i4].type = 16;
                                        else sf[i4].type = 1; //change attack to a miss type?

                                        if (x2) {
                                            if (myobj3) {
                                                if (myobj3->type == 48) {
                                                    //smash glass sword!
                                                    if (i3 == 0) tnpc->wep_left = NULL;
                                                    else tnpc->wep_right = NULL;
                                                    tnpc->upflags |= 64;
                                                }
                                            }
                                        }

                                        //x7=rnd*32; //0-127
                                        //x8=rnd*(tnpc->d+1);
                                        //if (x7>x8){ //couldn't dodge the attack!

