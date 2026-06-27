// LHS-P5.6: part_c_npcwalk_follow -- CONTINUES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_c_objupdate.cpp.
// Contents: npc_walk, follow/path (follow_*). Move-only: no STL, no reorg.
                                npc_walk:

                                    myobj2 = OBJfindlastall(myobj->x, myobj->y);
                                    x3 = OBJmove2(myobj, myobj->x + x2, myobj->y + y2);

                                    if (myobj2 != NULL) {
                                        if (((myobj2->type & 1023) == 301) && (x3 == 0)) {
                                            //close door behind NPC
                                            myobj2 = OBJfindlast(myobj2->x, myobj2->y);
                                            if (myobj2 != NULL) {
                                                x2 = myobj2->type & 1023;
                                                if ((x2 >= 297) && (x2 <= 300)) {
                                                    //assume not locked
                                                    x = myobj2->type >> 10;
                                                    if (x & 1) {
                                                        myobj3 = myobj2;
                                                        myobj2 = (object *) myobj2->more;
                                                    } else { myobj3 = (object *) myobj2->more; }
                                                    if (x < 4) {
                                                        x = x >> 1;
                                                        x = x << 1;
                                                        x = x & 2;
                                                        myobj2->type = x2 + ((x + 4) * 1024);
                                                        myobj3->type = x2 + ((x + 5) * 1024);
                                                        OBJcheckflags(myobj2->x, myobj2->y);
                                                        OBJcheckflags(myobj3->x, myobj3->y);
                                                    } else {
                                                        x = x >> 1;
                                                        x = x << 1;
                                                        x = x & 2;
                                                        myobj2->type = x2 + (x * 1024);
                                                        myobj3->type = x2 + ((x + 1) * 1024);
                                                        OBJcheckflags(myobj2->x, myobj2->y);
                                                        OBJcheckflags(myobj3->x, myobj3->y);
                                                    } //<4

                                                    x2 = myobj2->x;
                                                    y2 = myobj2->y;
                                                    i3 = SFnew(x2, y2);
                                                    //i3 points to new sf
                                                    sf[i3].type = 4;
                                                    sf[i3].x = x2;
                                                    sf[i3].y = y2;
                                                    sf[i3].wait = 1.0f; //NULL
                                                    sf[i3].more = 2;

                                                    tnpc->wait_walk += 0.0625f;
                                                    tnpc->wait_walk += (rnd * 0.125f);
                                                }
                                            }
                                        } //close door
                                    } //!=NULL

                                    if (x3) {
                                        //blocked
                                        myobj2 = myobj;
                                        if (myobj3 = OBJfindlast(myobj2->x + x2, myobj2->y + y2)) {
                                            //nonplayernpc->horse swap
                                            if ((myobj3->type & 1023) == 431) {
                                                if (myobj3->info & 8) {
                                                    //S->H(back)
                                                    myobj4 = (object *) myobj3->more;
                                                    tnpc2 = (npc *) myobj4->more;
                                                    x3 = myobj4->x;
                                                    y3 = myobj4->y;
                                                    x4 = 0;
                                                    y4 = 0;
                                                    //check adjacent squares of horse's head for walkability
                                                    if (bt[y3 - 1][x3] & 1024) {
                                                        //walkable
                                                        x4 = x3;
                                                        y4 = y3 - 1;
                                                    }
                                                    if (bt[y3][x3 + 1] & 1024) {
                                                        //walkable
                                                        if ((x4 == 0) && (y4 == 0)) {
                                                            x4 = x3 + 1;
                                                            y4 = y3;
                                                        }
                                                        x5 = rnd * 2;
                                                        if (x5) {
                                                            x4 = x3 + 1;
                                                            y4 = y3;
                                                        }
                                                    }
                                                    if (bt[y3 + 1][x3] & 1024) {
                                                        //walkable
                                                        if ((x4 == 0) && (y4 == 0)) {
                                                            x4 = x3;
                                                            y4 = y3 + 1;
                                                        }
                                                        x5 = rnd * 2;
                                                        if (x5) {
                                                            x4 = x3;
                                                            y4 = y3 + 1;
                                                        }
                                                    }
                                                    if (bt[y3][x3 - 1] & 1024) {
                                                        //walkable
                                                        if ((x4 == 0) && (y4 == 0)) {
                                                            x4 = x3 - 1;
                                                            y4 = y3;
                                                        }
                                                        x5 = rnd * 2;
                                                        if (x5) {
                                                            x4 = x3 - 1;
                                                            y4 = y3;
                                                        }
                                                    }
                                                    if (x4 || y4) {
                                                        OBJmove_allow = TRUE;
                                                        OBJmove2(myobj2, myobj3->x, myobj3->y);
                                                        OBJmove2(myobj4, x4, y4);
                                                        OBJmove_allow = FALSE;
                                                        goto doneswap2;
                                                    }
                                                    //if no adjacent square is available then move on top of the back of the horse
                                                    OBJmove_allow = TRUE;
                                                    OBJmove2(myobj2, myobj3->x, myobj3->y);
                                                    OBJmove_allow = FALSE;
                                                    goto doneswap2;
                                                }
                                                //else S->H(front)
                                                //S->H(front)
                                                myobj4 = myobj3;
                                                tnpc2 = (npc *) myobj4->more;
                                                x3 = ((object *) tnpc2->more)->x;
                                                y3 = ((object *) tnpc2->more)->y;
                                                //if (x3==0) goto badhorseswap;
                                                x4 = 0;
                                                y4 = 0;
                                                //check adjacent squares of horse's head for walkability
                                                if (bt[y3 - 1][x3] & 1024) {
                                                    //walkable
                                                    x4 = x3;
                                                    y4 = y3 - 1;
                                                }
                                                if (bt[y3][x3 + 1] & 1024) {
                                                    //walkable
                                                    if ((x4 == 0) && (y4 == 0)) {
                                                        x4 = x3 + 1;
                                                        y4 = y3;
                                                    }
                                                    x5 = rnd * 2;
                                                    if (x5) {
                                                        x4 = x3 + 1;
                                                        y4 = y3;
                                                    }
                                                }
                                                if (bt[y3 + 1][x3] & 1024) {
                                                    //walkable
                                                    if ((x4 == 0) && (y4 == 0)) {
                                                        x4 = x3;
                                                        y4 = y3 + 1;
                                                    }
                                                    x5 = rnd * 2;
                                                    if (x5) {
                                                        x4 = x3;
                                                        y4 = y3 + 1;
                                                    }
                                                }
                                                if (bt[y3][x3 - 1] & 1024) {
                                                    //walkable
                                                    if ((x4 == 0) && (y4 == 0)) {
                                                        x4 = x3 - 1;
                                                        y4 = y3;
                                                    }
                                                    x5 = rnd * 2;
                                                    if (x5) {
                                                        x4 = x3 - 1;
                                                        y4 = y3;
                                                    }
                                                }
                                                if (x4 || y4) {
                                                    OBJmove_allow = TRUE;
                                                    OBJmove2(myobj2, myobj4->x, myobj4->y);
                                                    OBJmove2(myobj4, x3, y3);
                                                    OBJmove2(myobj4, x4, y4);
                                                    OBJmove_allow = FALSE;
                                                    goto doneswap2;
                                                }
                                                //if no adjacent square is available a regular swap will occur...
                                            } //horse swap
                                            //badhorseswap:

                                            if (myobj3->info & (2 + 4)) {
                                                //swap with npc/crt

                                                if (tnpc->converse == 130) {
                                                    //smith
                                                    tnpc->wait_walk += 1.0f + rnd * 2.0f; //wait to reduce code overhead
                                                    tnpc->pathn = 0; //path was blocked but another path may exist!
                                                    goto NPC_skip1;
                                                }

                                                //if ((myobj2->type&1023)==354){ //mouse
                                                if (myobj4 = OBJfindlastall(myobj2->x, myobj2->y)) {
                                                    if ((myobj4->type & 1023) == 213) goto NPC_skip1; //mousehole
                                                }
                                                if (myobj4 = OBJfindlastall(myobj2->x, myobj2->y)) {
                                                    if (myobj4 = (object *) myobj4->prev) {
                                                        if (((myobj4->type & 1023) == 310) && (myobj4->type & 1024))
                                                            goto NPC_skip1; //portcullis
                                                    }
                                                }
                                                if (myobj4 = OBJfindlastall(myobj2->x + x2, myobj2->y + y2)) {
                                                    if ((myobj4->type & 1023) == 213) goto NPC_skip1; //mousehole
                                                }
                                                if (myobj4 = OBJfindlastall(myobj2->x + x2, myobj2->y + y2)) {
                                                    if (myobj4 = (object *) myobj4->prev) {
                                                        if (((myobj4->type & 1023) == 310) && (myobj4->type & 1024))
                                                            goto NPC_skip1; //portcullis
                                                    }
                                                }
                                                //}//mouse


                                                //swap with NPC
                                                x2 = myobj2->x;
                                                y2 = myobj2->y;
                                                x3 = myobj3->x;
                                                y3 = myobj3->y;


                                                //if ((abs(myobj->x-tnpc->schedule[x2].x)==0)&&(abs(myobj->y-tnpc->schedule[x2].y)==0)){
                                                //tnpc->schedule_i=x2;
                                                //if (schedule_i==0x91){


                                                x4 = 1; //bedswap2

                                                if (myobj3->info & 2) {
                                                    tnpc2 = (npc *) myobj3->more;
                                                    if (tnpc2->schedule) {
                                                        if (tnpc2->schedule_i != -1) {
                                                            if (tnpc2->schedule[tnpc2->schedule_i].type == 0x91) {
                                                                x4 = 0;
                                                            }
                                                        }
                                                    }
                                                    if (tnpc2->converse == 130) x4 = 0;
                                                    //smith (other npc cannot swap with smith)
                                                }

                                                if (x4) {
                                                    //myobj4=od[y3][x3]; if ((myobj4->type&1023)!=163){ //bedswap

                                                    OBJmove_allow = TRUE;
                                                    OBJmove2(myobj2, x3, y3);
                                                    OBJmove2(myobj3, x2, y2);
                                                    OBJmove_allow = FALSE;

                                                doneswap2:

                                                    tnpc->wait_walk += 0.0625f; //regular move minimum wait
                                                    //tnpc->wait+=(rnd*0.125f);
                                                    tnpc->wait_walk += (rnd * 1.0f); //+swap wait
                                                    //tnpc->pathn++; //failed to move

                                                    if (myobj3->info & 2) tnpc2->pathn = 0;
                                                    //other NPC's path needs updating

                                                    goto NPC_skip1;
                                                } //bedswap2
                                            } //<-npc
                                            //}
                                        } //!=NULL


                                        //blocked! by a passthrough? ->use
                                        myobj2 = OBJfindlast(myobj->x + x2, myobj->y + y2);
                                        if (myobj2 != NULL) {
                                            x3 = myobj2->type;
                                            //if ((x2==278)||(x2==280)){

                                            if (x3 == OBJ_PASSTHROUGH_V) {
                                                //v-passthrough
                                                if (myobj2->more == NULL) {
                                                    myobj2->more = OBJnew();
                                                    myobj3 = (object *) myobj2->more;
                                                    myobj3->more = myobj2; //back link
                                                    myobj3->type = OBJ_PASSTHROUGH_V + 1 * 1024;
                                                    OBJadd(myobj2->x - 1, myobj2->y - 1, myobj3);
                                                } else {
                                                    OBJadd(myobj2->x - 1, myobj2->y - 1, (object *) myobj2->more);
                                                }
                                                x = myobj2->x;
                                                y = myobj2->y - 1;
                                                OBJremove(myobj2);
                                                myobj2->type = OBJ_PASSTHROUGH_V + 2 * 1024;
                                                OBJadd(x, y, myobj2);

                                                tnpc->wait_walk += 0.0625f;
                                                tnpc->wait_walk += (rnd * 0.125f);
                                                tnpc->pathn++; //failed to move
                                                goto NPC_skip1;
                                            } //278


                                            if (x3 == OBJ_PASSTHROUGH_H) {
                                                //h-passthrough
                                                if (myobj2->more == NULL) {
                                                    myobj2->more = OBJnew();
                                                    myobj3 = (object *) myobj2->more;
                                                    myobj3->more = myobj2; //back link
                                                    myobj3->type = OBJ_PASSTHROUGH_H + 1 * 1024;
                                                    OBJadd(myobj2->x - 1, myobj2->y - 1, myobj3);
                                                } else {
                                                    OBJadd(myobj2->x - 1, myobj2->y - 1, (object *) myobj2->more);
                                                }
                                                x = myobj2->x - 1;
                                                y = myobj2->y;
                                                OBJremove(myobj2);
                                                myobj2->type = OBJ_PASSTHROUGH_H + 2 * 1024;
                                                OBJadd(x, y, myobj2);

                                                tnpc->wait_walk += 0.0625f;
                                                tnpc->wait_walk += (rnd * 0.125f);
                                                tnpc->pathn++; //failed to move
                                                goto NPC_skip1;
                                            } //280
                                        } //myobj2


                                        //blocked! by a door? ->use
                                        myobj2 = OBJfindlast(myobj->x + x2, myobj->y + y2);
                                        if (myobj2 != NULL) {
                                            x3 = myobj2->type & 1023;
                                            if ((x3 >= 297) && (x3 <= 300)) {
                                                x2 = x3;
                                                //assume not locked
                                                x = myobj2->type >> 10;
                                                if ((myobj2->type & 1024)) {
                                                    //ignore the secondary door parts
                                                    if (x < 8) {
                                                        if (x & 1) {
                                                            myobj3 = myobj2;
                                                            myobj2 = (object *) myobj2->more;
                                                        } else { myobj3 = (object *) myobj2->more; }
                                                        if (x < 4) {
                                                            x = x >> 1;
                                                            x = x << 1;
                                                            x = x & 2;
                                                            myobj2->type = x2 + ((x + 4) * 1024);
                                                            myobj3->type = x2 + ((x + 5) * 1024);
                                                            OBJcheckflags(myobj2->x, myobj2->y);
                                                            OBJcheckflags(myobj3->x, myobj3->y);
                                                        } else {
                                                            x = x >> 1;
                                                            x = x << 1;
                                                            x = x & 2;
                                                            myobj2->type = x2 + (x * 1024);
                                                            myobj3->type = x2 + ((x + 1) * 1024);
                                                            OBJcheckflags(myobj2->x, myobj2->y);
                                                            OBJcheckflags(myobj3->x, myobj3->y);
                                                        } //<4

                                                        x2 = myobj2->x;
                                                        y2 = myobj2->y;
                                                        i3 = SFnew(x2, y2);
                                                        //i3 points to new sf
                                                        sf[i3].type = 4;
                                                        sf[i3].x = x2;
                                                        sf[i3].y = y2;
                                                        sf[i3].wait = 1.0f; //NULL
                                                        sf[i3].more = 1;

                                                        tnpc->wait_walk += 0.0625f;
                                                        tnpc->wait_walk += (rnd * 0.125f);
                                                        tnpc->pathn++; //failed to move
                                                        goto NPC_skip1;
                                                    } //x2<8
                                                }
                                            }
                                        }


                                        tnpc->wait_walk += 1.0f + rnd * 2.0f; //wait to reduce code overhead
                                        tnpc->pathn = 0; //path was blocked but another path may exist!
                                        goto NPC_skip1;
                                    } //door

                                    //f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                                    //tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);

                                    //npc walk speed
                                    //tnpc->wait_walk+=0.0625f;
                                    //tnpc->wait_walk+=(rnd*0.125f);

                                    tnpc->wait_walk += (0.1875f + (rnd * 0.0625f));
                                    //tnpc->wait_walk+=(0.25f+(rnd*0.125f));

                                    goto NPC_skip1;
                                } //order2
                            } //waitOK
                            goto NPC_skip1;
                        }

                        //have valid tplayer pointer! myobj=our object!
                        //if (tplayer->party[tplayer->selected_partymember]!=myobj){


                        //tplayer is valid


                        static unsigned char horse_movediagonal;
                        static short horse_movediagonalx, horse_movediagonaly;
                        horse_movediagonal = 0;

                        //dv++;
                        //dv=tnpc->wait_walk;
                        //dv2=et/32.0f;
                        //tnpc->wait_walk-=(et/32.0f);
                        //if (tnpc->wait_walk<0) tnpc->wait_walk=0;

                        static unsigned char selected_partymember;
                        selected_partymember = 0;
                        if (tplayer->party[tplayer->selected_partymember] == myobj) selected_partymember = 1;


                        if ((selected_partymember) && (keyon[VK_SPACE] && U6O_DEBUG)) tnpc->wait_walk = 0;


                        //partymember npc
                        //AUTOPICKUPadd(myobj,myobj3);
                        //check surrounding squares for items to pickup


                        if (tnpc->wait_disable == 0.0f) {
                            if (tnpc->wait_attack == 0.0f) {
                                for (y2 = -1; y2 <= 1; y2++) {
                                    for (x2 = -1; x2 <= 1; x2++) {
                                        if (x2 || y2) {
                                            //autopickupcheckloop:
                                            if (myobj3 = OBJfindlast(myobj->x + x2, myobj->y + y2)) {
                                                if (AUTOPICKUP_OBJECTVALID[myobj3->type & 1023]) {
                                                    if (myobj4 = AUTOPICKUPcheck(tplayer, myobj3)) {
                                                        //MessageBox(NULL,"myobj4 could have been recovered","Ultima 6 Online",MB_OK);
                                                        OBJremove(myobj3);
                                                        tnpc3 = (npc *) myobj4->more;

                                                        static object *invbag, *invobj;
                                                        invbag = tnpc3->items;
                                                        invobj = myobj3;

                                                        //inventory add
                                                        txtsetchar(t, 2); //sfx message
                                                        txtaddshort(t, 1); //number of sfx effects
                                                        txtaddshort(t, 27); //inventory add
                                                        txtaddshort(t, myobj4->x); //x
                                                        txtaddshort(t, myobj4->y); //y
                                                        txtaddshort(t, myobj3->type); //graphic (x2)
                                                        txtaddshort(t, 0); //unused placeholder for y2
                                                        txtaddshort(t, 0); //unused placeholder for more
                                                        txtaddfloat(t, 0.25f);
                                                        //time to display message for (in this case half a second)
                                                        NET_send(NETplayer, tplayer->net, t);

                                                        tnpc3->upflags |= 32; //inv

                                                        //join/combine items in inventory
                                                        if (obji[sprlnk[invobj->type & 1023] + (invobj->type >> 10)].
                                                            flags & 4096) {
                                                            //multiple/stackable
                                                            if (myobj5 = (object *) invbag->more) {
                                                            joinitem2:
                                                                if (myobj5->type == invobj->type) {
                                                                    //same type
                                                                    if ((myobj5->info & 128) == (invobj->info & 128)) {
                                                                        if (((invobj->type & 1023) == 90) && (
                                                                                myobj5->info != invobj->info))
                                                                            goto
                                                                                    cantjoin2;
                                                                        //cant join half burnt torches
                                                                        myobj5->more2 += invobj->more2;
                                                                        OBJrelease(invobj);
                                                                        goto joinitem_finished2;
                                                                    } //same stealing type
                                                                } //same type
                                                                if (myobj5->next) {
                                                                    myobj5 = (object *) myobj5->next;
                                                                    goto joinitem2;
                                                                }
                                                            } //myobj5=(object*)invbag->more
                                                        } //4096
                                                    cantjoin2:
                                                        invobj->info |= 112;
                                                        invobj->next = invbag->more;
                                                        invobj->prev = invbag;
                                                        invbag->more = invobj;
                                                        if (invobj->next) ((object *) invobj->next)->prev = invobj;

                                                        //autoready item?
                                                        if (obji[sprlnk[invobj->type & 1023]].flags & 8) {
                                                            //check for any two handed weapons!
                                                            if (myobj5 = tnpc3->wep_left) {
                                                                if (obji[sprlnk[myobj5->type & 1023]].flags & 16)
                                                                    goto
                                                                            cantautoready;
                                                            }
                                                            if (myobj5 = tnpc3->wep_right) {
                                                                if (obji[sprlnk[myobj5->type & 1023]].flags & 16)
                                                                    goto
                                                                            cantautoready;
                                                            }
                                                            //ready item if possible
                                                            if (tnpc3->wep_left == NULL) {
                                                                OBJremove(invobj);
                                                                tnpc3->wep_left = invobj;
                                                                tnpc3->upflags |= 64;
                                                                goto autoreadycomplete;
                                                            }
                                                            if (tnpc3->wep_right == NULL) {
                                                                OBJremove(invobj);
                                                                tnpc3->wep_right = invobj;
                                                                tnpc3->upflags |= 64;
                                                                goto autoreadycomplete;
                                                            }
                                                        } //autoready item
                                                    autoreadycomplete:
                                                    cantautoready:

                                                    joinitem_finished2:
                                                        tnpc->wait_attack += 0.25f;
                                                        goto autopickup_gotitem;
                                                    }
                                                } //AUTOPICKUP_OBJECTVALID
                                            }
                                        }
                                    }
                                }
                            } //wait_attack
                        } //wait_disable
                    autopickup_gotitem:

                        //wizard eye movement
                        if (selected_partymember) {
                            if (tplayer->wizardeyetimeleft) {
                                x2 = 0;
                                y2 = 0;
                                if ((tplayer->key & KEYleft) || (tplayer->key & KEYleft2)) x2--;
                                if ((tplayer->key & KEYright) || (tplayer->key & KEYright2)) x2++;
                                if ((tplayer->key & KEYup) || (tplayer->key & KEYup2)) y2--;
                                if ((tplayer->key & KEYdown) || (tplayer->key & KEYdown2)) y2++;
                                if (tplayer->key & KEYdown2) tplayer->key -= KEYdown2; //clear dir2 key
                                if (tplayer->key & KEYup2) tplayer->key -= KEYup2;
                                if (tplayer->key & KEYleft2) tplayer->key -= KEYleft2;
                                if (tplayer->key & KEYright2) tplayer->key -= KEYright2;

                                if (tplayer->wizardeyemovewait == 0.0f) {
                                    tplayer->wizardeyemovewait = 0.1f / 0.8f;

                                    if (x2 || y2) {
                                        x2 += tplayer->wizardeyex;
                                        y2 += tplayer->wizardeyey;
                                        //map boundries check
                                        if (x2 >= 1) {
                                            if (y2 >= 1) {
                                                if (x2 < 2047) {
                                                    if (y2 < 1023) {
                                                        x3 = 1;

                                                        if ((bt[y2][x2] & 255) >= 252) x3 = 0; //cannot enter void!

                                                        if (x2 >= 0) {
                                                            if (y2 >= 0) {
                                                                if (x2 < 1024) {
                                                                    if (y2 < 1024) {
                                                                        //britannia mainland
                                                                        goto wizeye_norestriction;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        if (x2 >= 1024) {
                                                            if (y2 >= 256) {
                                                                if (x2 < (1024 + 256)) {
                                                                    if (y2 < 512) {
                                                                        //garg lands
                                                                        goto wizeye_norestriction;
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        //restricted area checks
                                                        if ((btflags[bt[y2][x2] & 1023] & (1 + 2 + 4 + 8)) == 0) {
                                                            //1=land passabe
                                                            //2=air passable
                                                            //4=sea passable
                                                            //8=bolt passable
                                                            x3 = 0;
                                                        }
                                                    wizeye_norestriction:


                                                        if (x3) {
                                                            if (myobj2 = od[y2][x2]) {
                                                            wizeye_laddercheck:
                                                                myobj3 = myobj2;
                                                                if ((myobj3->info & 8) == 8)
                                                                    myobj3 = (object *) myobj3->more; //redirector
                                                                x4 = myobj3->type & 1023;
                                                                if ((x4 == 305) || (x4 == 326) || (x4 == 308)) {
                                                                    //ladder or dungeon or hole
                                                                    x2 = myobj3->more2 & 0xFFFF;
                                                                    y2 = myobj3->more2 >> 16;
                                                                    goto wizeye_gotladder;
                                                                }
                                                                if (myobj2->next) {
                                                                    myobj2 = (object *) myobj2->next;
                                                                    goto wizeye_laddercheck;
                                                                }
                                                            }
                                                        wizeye_gotladder:

                                                            tplayer->wizardeyex = x2;
                                                            tplayer->wizardeyey = y2;
                                                        } //x3
                                                    }
                                                }
                                            }
                                        }
                                    } //x2||y2
                                } //f
                                goto dontmove;
                            }
                        }


                        if ((!tnpc->wait_walk) && (!tnpc->wait_disable)) {
                            if (selected_partymember) {
                                //selected party member


                                x = 0;
                                x2 = 0;
                                y2 = 0;

                                if ((tplayer->key & KEYleft) || (tplayer->key & KEYleft2)) x2--;
                                if ((tplayer->key & KEYright) || (tplayer->key & KEYright2)) x2++;
                                if ((tplayer->key & KEYup) || (tplayer->key & KEYup2)) y2--;
                                if ((tplayer->key & KEYdown) || (tplayer->key & KEYdown2)) y2++;

                                if (tplayer->key & KEYdown2) tplayer->key -= KEYdown2; //clear dir2 key
                                if (tplayer->key & KEYup2) tplayer->key -= KEYup2;
                                if (tplayer->key & KEYleft2) tplayer->key -= KEYleft2;
                                if (tplayer->key & KEYright2) tplayer->key -= KEYright2;


                                if (tnpc->horse && (tplayer->craft == NULL)) {
                                    //horse diagonal movement

                                    //1. precheck all three related squares
                                    if (x2 && y2) {
                                        x3 = myobj->x + x2;
                                        y3 = myobj->y + y2;
                                        static unsigned char xok, yok, xyok;
                                        xok = 0;
                                        yok = 0;
                                        xyok = 0;


                                        if (bt[y3][x3] & 1024) {
                                            //walkable
                                            xyok = 1;
                                        } else {
                                            //not walkable
                                            //check if there is an npc to swap with
                                            if (myobj3 = OBJfindlast(x3, y3)) {
                                                if (myobj3->info & 8) myobj3 = (object *) myobj3->more;
                                                if (myobj3->info & 2) xyok = 1;
                                            } //myobj3
                                        } //xy

                                        if (bt[myobj->y][x3] & 1024) {
                                            //walkable
                                            xok = 1;
                                        } else {
                                            //not walkable
                                            //check if there is an npc to swap with
                                            if (myobj3 = OBJfindlast(x3, myobj->y)) {
                                                if (myobj3->info & 8) myobj3 = (object *) myobj3->more;
                                                if (myobj3->info & 2) xok = 1;
                                            } //myobj3
                                        } //x

                                        if (bt[y3][myobj->x] & 1024) {
                                            //walkable
                                            yok = 1;
                                        } else {
                                            //not walkable
                                            //check if there is an npc to swap with
                                            if (myobj3 = OBJfindlast(myobj->x, y3)) {
                                                if (myobj3->info & 8) myobj3 = (object *) myobj3->more;
                                                if (myobj3->info & 2) yok = 1;
                                            } //myobj3
                                        } //y

                                        if (xyok) {
                                            if (xok) {
                                                horse_movediagonal = 1;
                                                horse_movediagonalx = 0;
                                                horse_movediagonaly = y2;
                                                y2 = 0; //mask y (it will be done on the second move)
                                                goto horsemovepossible;
                                            }

                                            if (yok) {
                                                horse_movediagonal = 1;
                                                horse_movediagonalx = x2;
                                                horse_movediagonaly = 0;
                                                x2 = 0; //mask x (it will be done on the second move)
                                                goto horsemovepossible;
                                            }
                                            x2 = 0;
                                            y2 = 0;
                                        }

                                        if (xyok == 0) {
                                            //check if horse can slide along a wall
                                            if (yok == 1) {
                                                x2 = 0;
                                                goto horsemovepossible;
                                            }
                                            if (xok == 1) {
                                                y2 = 0;
                                                goto horsemovepossible;
                                            }
                                            x2 = 0;
                                            y2 = 0;
                                        }

                                        //OLD CODE: if (x2&&y2){x2=0; y2=0;} //can't move diagonally on a horse
                                    } //diagonal movement
                                } //horse


                            horsemovepossible:
                            horse_finishdiagonalmove:

                                if (x2 || y2) {
                                    if (((myobj->x + x2) == 923) && ((myobj->y + y2) == 872)) {
                                        //416 sacred quest
                                        static txt *txttmp1;

                                        if (!tplayer->GNPCflags[17]) {
                                            if (!tplayer->guardianstatuewait) {
                                                txttmp1 = txtnew();
                                                txtset(txttmp1,
                                                       "\x022Thou art not upon a Sacred Quest!\x022\\\x022Passage denied!\x022");
                                                i3 = SFnew(923, 872); //i3 points to new sf
                                                sf[i3].type = 3; //message to user
                                                sf[i3].x = 923;
                                                sf[i3].y = 872 - 2;
                                                sf[i3].wait = 5;
                                                sf[i3].more = 187;
                                                sf[i3].p = txttmp1;
                                                tplayer->guardianstatuewait = 8;
                                            }
                                            goto dontmove;
                                        }
                                        //sacred quest
                                        x2 = 923;
                                        if (y2 == -1) y2 = 871;
                                        if (y2 == 1) y2 = 873;
                                        if (tplayer->craft == NULL) {
                                            //for (i4=7;i4>=0;i4--){
                                            //if (tplayer->party[i4]){
                                            //OBJremove(tplayer->party[i4]);
                                            //OBJadd(x2,y2,tplayer->party[i4]);
                                            //}
                                            //}
                                            partyadd(tplayer, x2, y2);

                                            if (!tplayer->guardianstatuewait) {
                                                txttmp1 = txtnew();
                                                txtset(txttmp1, "\x022Passage granted!\x022");
                                                i3 = SFnew(923, 872); //i3 points to new sf
                                                sf[i3].type = 3; //message to user
                                                sf[i3].x = 923;
                                                sf[i3].y = 872 - 2;
                                                sf[i3].wait = 5;
                                                sf[i3].more = 187;
                                                sf[i3].p = txttmp1;
                                                tplayer->guardianstatuewait = 8;
                                            }
                                        } //craft
                                        goto dontmove;
                                    } //x,y
                                } //x2||y2

                                if (tplayer->pathfind) {
                                    tplayer->pathfind = 0;
                                    if ((x2 == 0) && (y2 == 0)) {
                                        tplayer->pathfind = 1;
                                        //events which should stop a pathfind:
                                        //i. switch party members
                                        //ii. party member dies
                                        //iii. path becomes blocked
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFright) x2 = 1;
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFleft) x2 = -1;
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFup) y2 = -1;
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFdown) y2 = 1;
                                        if (tnpc->pathn > 0) {
                                            //fix for pathfinding crash.
                                            tnpc->pathn--;
                                        }
                                        if (tnpc->pathn == 0) tplayer->pathfind = 0;
                                    } //x2==0,y2==0
                                } //pathfind

                                //BALLOON OVERRIDE!
                                if ((tplayer->party[0]->type & 1023) == 423) {
                                    //free flying with fan
                                    if (tplayer->mobj && tplayer->mobj->type == 212) {
                                        goto selected_partymember_move;
                                    }
                                    x2 = windx;
                                    y2 = windy;
                                    if ((tplayer->windx) || (tplayer->windy)) {
                                        x2 = tplayer->windx;
                                        y2 = tplayer->windy;
                                    }
                                }
                                x3 = tplayer->party[0]->x;
                                y3 = tplayer->party[0]->y;
                                if ((abs(x3 - (myobj->x + x2)) <= 8) && (abs(y3 - (myobj->y + y2)) <= 8)) {
                                    if ((x2 != 0) || (y2 != 0)) goto selected_partymember_move;
                                }
                            } //selected party member

                            if (tnpc->order == 1) {
                                //try to move towards avatar
                                if (myobj2 = tnpc->follow) {
                                    x2 = 0;
                                    y2 = 0;
                                    x3 = myobj->x - myobj2->x;
                                    y3 = myobj->y - myobj2->y;
                                    if (abs(x3) > abs(y3)) x4 = abs(x3);
                                    else x4 = abs(y3); //x4=distance from selected partymember
                                    if (x4 <= 1) goto dontmove;
                                    //get party order of npc
                                    y5 = 1;
                                    for (y4 = 0; y4 <= 7; y4++) {
                                        if (tplayer->party[y4]) {
                                            if (tplayer->party[y4] == myobj) goto follow_gotpartyorder;
                                            //<-GPF HERE (latest)
                                            if (tnpc->follow != tplayer->party[y4]) {
                                                y5++;
                                            }
                                        }
                                    }
                                follow_gotpartyorder:

                                    if ((x4 == 2) && (y5 >= 3)) goto dontmove;
                                    if ((x4 == 3) && (y5 >= 6)) goto dontmove;
                                    /*
                  if (x4==3){
                  x4=rnd*2; if (x4) goto dontmove;
                  }
                  if (x4==2){
                  x4=rnd*3; if (x4) goto dontmove;
                  }
                  */

                                    //get path
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
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFright) x2=1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFleft) x2=-1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFup) y2=-1;
                  if (tnpc->path[tnpc->path_max-tnpc->pathn]==PFdown) y2=1;
                  tnpc->pathn--;
                  */

                                    if (tnpc->pathn) {
                                        //use current path
                                    follow_getpath:
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFright) x2 = 1;
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFleft) x2 = -1;
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFup) y2 = -1;
                                        if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFdown) y2 = 1;
                                        if (tnpc->pathn > 0) {
                                            //fix for pathfinding crash.
                                            tnpc->pathn--;
                                        }
                                        goto follow_gotpath;
                                    }

                                    z = abs(myobj2->x - myobj->x);
                                    z2 = abs(myobj2->y - myobj->y);
                                    if (z2 > z) z = z2;
                                    z += 32;
                                    if (z > 250) z = 250;
                                    WPF_OBJECT = myobj;
                                    wpf_pathfind(tnpc->path, myobj->x, myobj->y, myobj2->x, myobj2->y, z, 0, 1);
                                    //|1|PATHFIND THROUGH OWN PARTY+16
                                    if (WPF_RETURN == WPF_PATHFOUND) {
                                        if (WPF_PATHLENGTH) {
                                            tnpc->path_max = WPF_PATHLENGTH;
                                            tnpc->pathn = WPF_PATHLENGTH;
                                            goto follow_getpath;
                                        }
                                    }
                                    //no path possible/(already at destination[can't be this value!])
                                    tnpc->pathn = 0;
                                    x2 = 0;
                                    y2 = 0;
                                    tnpc->wait_walk += 0.5f;
                                    goto donemove;

                                follow_gotpath:
                                    //avoid hazards
                                    z = abs(myobj2->x - myobj->x);
                                    z2 = abs(myobj2->y - myobj->y);
                                    if ((z <= 5) && (z2 <= 5)) {
                                        z = wpf_dangercheck(myobj->x, myobj->y, myobj);
                                        z2 = wpf_dangercheck(myobj->x + x2, myobj->y + y2, myobj);
                                        if (z2 > z) {
                                            x2 = 0;
                                            y2 = 0;
                                        }
                                    } //z&&z2

                                    //integrate player move and partymember->follow move
                                    if (tplayer && tplayer->camera_freeze) {
                                        if (myobj->x + x2 < tplayer->frozen_tpx || myobj->x + x2 >= tplayer->frozen_tpx + tplayer->frozen_vtx ||
                                            myobj->y + y2 < tplayer->frozen_tpy || myobj->y + y2 >= tplayer->frozen_tpy + tplayer->frozen_vty) {
                                            x2 = 0;
                                            y2 = 0;
                                        }
                                    }

                                selected_partymember_move:
                                    x3 = 0;

                                    //wait! diagonal?!?
                                    if ((abs(x2) + abs(y2)) > 1) {
                                        if (tplayer->craft == NULL) {
                                            if ((bt[myobj->y + y2][myobj->x] & 1024) + (
                                                    bt[myobj->y][myobj->x + x2] & 1024) == 0)
                                                goto NPC_skip1;
                                        }
                                    }
                                    if (OBJmove2(myobj, myobj->x + x2, myobj->y + y2) == 0) x3 = 1;


                                    if (x3 == 0) {
                                        //failed!


                                        if ((abs(x2) + abs(y2)) > 1) {
                                            //diagonal
                                            //attempt single movement along a plane
                                            if (OBJmove2(myobj, myobj->x, myobj->y + y2) == 0) {
                                                x3 = 1;
                                                x2 = 0;
                                                goto slidemove;
                                            }
                                            if (OBJmove2(myobj, myobj->x + x2, myobj->y) == 0) {
                                                x3 = 1;
                                                y2 = 0;
                                                goto slidemove;
                                            }
                                        }
                                    }
