// LHS-P5.7: part_c_npcattack_a -- CONTINUES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_c_objupdate.cpp.
// Contents: slidemove + NPC attack (npcattack2hnd). Move-only: no STL, no reorg.
                                slidemove:


                                    if (x3) {
                                        //if (tnpc->horse){


                                        if (tnpc->horse && (tplayer->craft == NULL)) {
                                            //wear down horseshoes?
                                            x4 = rnd * 10000;
                                            if (x4 == 0) {
                                                crt2 = (creature *) tnpc->horse->more;
                                                x4 = (crt2->respawn_x >> 6) & 3;
                                                if (x4) x4--;
                                                crt2->respawn_x &= (0xFFFF - (3 << 6));
                                                crt2->respawn_x += (x4 << 6);
                                            } //x4==0

                                            //NEW horse wait
                                            //8fps-16fps
                                            //food 0-3 *2 0
                                            //shoe 0-3 *2 0-6
                                            //race 0-7(15) 0-8
                                            //total 200.25->0.0625
                                            //(0.25-0.0625=0.1875)
                                            //(0.1875/20=0.009375)


                                            crt2 = (creature *) tnpc->horse->more;
                                            //this should fix the "all horse speeds same bug" and random crashes caused by uninitialized crt2
                                            f = 0.125;
                                            f2 = ((crt2->respawn_x >> 4) & 3) * 3; //food(0-15)
                                            f2 += (((crt2->respawn_x >> 6) & 3) * 3); //horseshoes(0-15)
                                            f2 += (crt2->respawn_x & 15);
                                            //races won(0-8) ***note: if horse races won>7 wait becomes less than 0.0625!***
                                            f2 *= 0.00260417f; //0.0625/24
                                            f -= f2;

                                            if (horse_movediagonal == 1) f *= 1.4142f;
                                            if (horse_movediagonal != 2) tnpc->wait_walk += f;

                                            goto wait_override;
                                        } //->horse;

                                        if ((myobj->type & 1023) == 423) {
                                            //balloon
                                            f = 0.25f;
                                            if ((abs(x2) + abs(y2)) > 1) f *= 1.4142f; //diagonal!
                                            tnpc->wait_walk += f;
                                            goto wait_override;
                                        }
                                        if ((myobj->type & 1023) == 412) {
                                            //boat
                                            //take into account wind direction & sea direction
                                            //x2 and y2 are relative direction
                                            f = 0.0625f;
                                            if ((abs(x2) + abs(y2)) > 1) f *= 1.4142f; //diagonal!
                                            tnpc->wait_walk += f;
                                            //additional current wait?


                                            goto wait_override;
                                        }

                                        f = (float) tnpc->d / 64.0f;
                                        if (f > 1.0f) f = 1.0f;
                                        //f=(0.25f-0.0625f-f*0.125f); //pre Mar
                                        f = (0.25f - 0.0625f - f * 0.125f);
                                        f = f * 0.875f;
                                        if ((abs(x2) + abs(y2)) > 1) f *= 1.4142f; //diagonal!
                                        tnpc->wait_walk += f;

                                        //if (selected_partymember){
                                        //f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                                        //tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);
                                        //}else{
                                        ////tnpc->wait_walk+=(0.0625f+(rnd*0.125f));
                                        //f=(float)tnpc->d/64.0f; if (f>1.0f) f=1.0f;
                                        //tnpc->wait_walk+=(0.25f-0.0625f-f*0.125f);
                                        //}

                                    wait_override:;
                                    } //x3


                                    if ((x3 == 0) && (tplayer->craft == NULL)) {
                                        //NPC is blocked! try swap
                                        myobj2 = myobj; //temp fix
                                        myobj3 = OBJfindlast(myobj2->x + x2, myobj2->y + y2);


                                        if (myobj3) {
                                            //swapping into the back of a horse
                                            if ((myobj3->type & 1023) == 431) {
                                                if (myobj3->info & 8) {
                                                    myobj4 = (object *) myobj3->more;
                                                    tnpc2 = (npc *) myobj4->more;
                                                    if ((tnpc2->player != tnpc->player) || (selected_partymember)) {
                                                        //if (tnpc->horse){
                                                        if (tnpc->horse && (tplayer->craft == NULL)) {
                                                            //MessageBox(NULL,"horse 2 horse swap!","Ultima 6 Online",MB_OK);

                                                            //swap both "types"
                                                            //invert both horse directions
                                                            x2 = myobj2->x;
                                                            y2 = myobj2->y;
                                                            z2 = myobj2->type >> 11;
                                                            x3 = myobj4->x;
                                                            y3 = myobj4->y;
                                                            z3 = myobj4->type >> 11;
                                                            OBJmove_allow = TRUE;

                                                            OBJmove2(myobj2, x3, y3);
                                                            z = z3;
                                                            x4 = 0;
                                                            y4 = 0;
                                                            if (z == 0) y4 = -1;
                                                            if (z == 1) x4 = 1;
                                                            if (z == 2) y4 = 1;
                                                            if (z == 3) x4 = -1;
                                                            OBJmove2(myobj2, myobj2->x + x4, myobj2->y + y4);
                                                            OBJmove2(myobj2, myobj2->x - x4 * 2, myobj2->y - y4 * 2);


                                                            OBJmove2(myobj4, x2, y2);
                                                            z = z2;
                                                            x4 = 0;
                                                            y4 = 0;
                                                            if (z == 0) y4 = -1;
                                                            if (z == 1) x4 = 1;
                                                            if (z == 2) y4 = 1;
                                                            if (z == 3) x4 = -1;
                                                            OBJmove2(myobj4, myobj4->x + x4, myobj4->y + y4);
                                                            OBJmove2(myobj4, myobj4->x - x4 * 2, myobj4->y - y4 * 2);

                                                            OBJmove_allow = FALSE;
                                                            goto doneswap;
                                                        } else {
                                                            //S->H(back)
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
                                                                goto doneswap;
                                                            }
                                                            //if no adjacent square is available then move on top of the back of the horse
                                                            OBJmove_allow = TRUE;
                                                            OBJmove2(myobj2, myobj3->x, myobj3->y);
                                                            OBJmove_allow = FALSE;
                                                            goto doneswap;
                                                        } //->horse
                                                    } //not in same party or is leader
                                                    goto NPC_skip1;
                                                } //&8

                                                //swap with head of a horse
                                                myobj4 = myobj3;
                                                tnpc2 = (npc *) myobj4->more;
                                                //if (((object*)(tnpc2->more))->x==0) goto NPC_skip1;//!

                                                if ((tnpc2->player != tnpc->player) || (selected_partymember)) {
                                                    //if (tnpc->horse){
                                                    if (tnpc->horse && (tplayer->craft == NULL)) {
                                                        //swap both "types"
                                                        //invert both horse directions
                                                        x2 = myobj2->x;
                                                        y2 = myobj2->y;
                                                        z2 = myobj2->type >> 11;
                                                        x3 = myobj4->x;
                                                        y3 = myobj4->y;
                                                        z3 = myobj4->type >> 11;
                                                        OBJmove_allow = TRUE;

                                                        OBJmove2(myobj2, x3, y3);
                                                        z = z3;
                                                        x4 = 0;
                                                        y4 = 0;
                                                        if (z == 0) y4 = -1;
                                                        if (z == 1) x4 = 1;
                                                        if (z == 2) y4 = 1;
                                                        if (z == 3) x4 = -1;
                                                        OBJmove2(myobj2, myobj2->x - x4, myobj2->y - y4);
                                                        OBJmove2(myobj2, myobj2->x + x4, myobj2->y + y4);

                                                        OBJmove2(myobj4, x2, y2);
                                                        z = z2;
                                                        x4 = 0;
                                                        y4 = 0;
                                                        if (z == 0) y4 = -1;
                                                        if (z == 1) x4 = 1;
                                                        if (z == 2) y4 = 1;
                                                        if (z == 3) x4 = -1;
                                                        OBJmove2(myobj4, myobj4->x - x4, myobj4->y - y4);
                                                        OBJmove2(myobj4, myobj4->x + x4, myobj4->y + y4);

                                                        OBJmove_allow = FALSE;
                                                        goto doneswap;
                                                    } else {
                                                        //S->H(front)
                                                        x3 = ((object *) tnpc2->more)->x;
                                                        y3 = ((object *) tnpc2->more)->y;
                                                        //if (x3==0) goto NPC_skip1;//!
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
                                                            goto doneswap;
                                                        }
                                                        //if no adjacent square is available a regular swap will occur...
                                                    } //->horse
                                                } //not in same party or is leader
                                            } //431


                                            if (myobj3->info & 2) {
                                                //<-npc
                                                tnpc2 = (npc *) myobj3->more;


                                                if (tnpc2->player) {
                                                    if (tnpc2->player->craft) goto NPC_skip1;
                                                }

                                                if (tnpc2->converse == 128) goto NPC_skip1; //pushmepullyou
                                                if (tnpc2->converse == 130) goto NPC_skip1; //smith
                                                if (tnpc2->converse == 187) goto NPC_skip1;
                                                if (tnpc2->converse == 188) goto NPC_skip1;
                                                if (tnpc2->converse == 189) goto NPC_skip1;

                                                if ((tnpc2->player != tnpc->player) || (selected_partymember)) {
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


                                                    x2 = myobj2->x;
                                                    y2 = myobj2->y;
                                                    x3 = myobj3->x;
                                                    y3 = myobj3->y;


                                                    //}//mouse

                                                    //swap with NPC

                                                    //myobj4=od[y3][x3];
                                                    //if ((myobj4->type&1023)!=163){
                                                    //tnpc2=(npc*)myobj3->more;
                                                    x4 = 1; //bedswap2
                                                    if (tnpc2->schedule) {
                                                        if (tnpc2->schedule_i != -1) {
                                                            if (tnpc2->schedule[tnpc2->schedule_i].type == 0x91) {
                                                                x4 = 0;
                                                            }
                                                        }
                                                    }
                                                    if (x4) {
                                                        //if (tnpc->horse){//player is on a horse, however target is not! (or it would have already been checked)
                                                        if (tnpc->horse && (tplayer->craft == NULL)) {
                                                            //CHECKED: can't swap diagonally
                                                            z = myobj2->type >> 11;
                                                            x4 = 0;
                                                            y4 = 0;
                                                            if (z == 0) y4 = -1;
                                                            if (z == 1) x4 = 1;
                                                            if (z == 2) y4 = 1;
                                                            if (z == 3) x4 = -1;
                                                            x2 -= x4;
                                                            y2 -= y4;
                                                            OBJmove_allow = TRUE;
                                                            OBJmove2(myobj2, x3, y3);
                                                            OBJmove2(myobj3, x2, y2);
                                                            OBJmove_allow = FALSE;
                                                            goto doneswap;
                                                        }


                                                        OBJmove_allow = TRUE;
                                                        OBJmove2(myobj2, x3, y3);
                                                        OBJmove2(myobj3, x2, y2);
                                                        OBJmove_allow = FALSE;


                                                    doneswap:
                                                        if (selected_partymember) tnpc->wait_walk += 0.125f;
                                                        else tnpc->wait_walk += (rnd * 1.0f);
                                                        //***************

                                                        if (horse_movediagonal == 1) {
                                                            horse_movediagonal = 2;
                                                            x2 = horse_movediagonalx;
                                                            y2 = horse_movediagonaly;
                                                            goto horse_finishdiagonalmove;
                                                        }


                                                        //tnpc->pathn++; //failed to move
                                                        //error: other characters path has been altered
                                                        tnpc2->pathn = 0;
                                                        goto NPC_skip1;
                                                    } //bedswap
                                                } //not in party
                                            } //<-npc
                                        } //!=NULL


                                        //blocked! by a door? ->use
                                        if (selected_partymember == 0) {
                                            myobj2 = OBJfindlast(myobj->x + x2, myobj->y + y2);
                                            if (myobj2 != NULL) {
                                                x2 = myobj2->type & 1023;
                                                if ((x2 >= 297) && (x2 <= 300)) {
                                                    if ((myobj2->type >> 10) < 8) {
                                                        //check if locked!
                                                        if ((myobj2->type & 1024)) {
                                                            //ignore the secondary door parts
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
                                                            sf[i3].more = 1;

                                                            f = (float) tnpc->d / 64.0f;
                                                            if (f > 1.0f) f = 1.0f;
                                                            tnpc->wait_walk += (0.25f - 0.0625f - f * 0.125f);
                                                            tnpc->pathn++; //failed to move
                                                            goto donemove;
                                                        }
                                                    }
                                                }
                                            }
                                        } //end (blocked by door)

                                        tnpc->pathn = 0;
                                        if (selected_partymember) tplayer->pathfind = 0;
                                    dontmove:
                                        if (!selected_partymember) tnpc->wait_walk += 0.5f;
                                    } //x3==0 (blocked)

                                    if (horse_movediagonal == 1) {
                                        horse_movediagonal = 2;
                                        x2 = horse_movediagonalx;
                                        y2 = horse_movediagonaly;
                                        goto horse_finishdiagonalmove;
                                    }
                                } //tnpc->follow
                                goto donemove;
                            } //order==1 (follow)
                        } //wait_walk

                        if (tnpc->order == 3) {
                            //attack target, if target==NULL find target
                            //txtset(t, "ATTACK!"); LOGadd(t); // t222

                            if (tnpc->target == NULL) {
                                //chk vlnk
                                if (tplayer->party[tplayer->selected_partymember] == myobj) tnpc->order = 0;
                                else tnpc->order = 1;
                                tnpc->follow = tplayer->party[tplayer->selected_partymember];
                                goto skipnpcattack;
                            }

                            static unsigned char spikedhelm_attack;
                            static unsigned char spikedhelm_onlyattack;
                            static object *spikedhelm;
                            spikedhelm_attack = 0;
                            spikedhelm_onlyattack = 0;
                            if (spikedhelm = tnpc->helm) {
                                if (spikedhelm->type == 4) {
                                    if (tplayer->craft == NULL) {
                                        spikedhelm_attack = 1;
                                    }
                                }
                            }

                            myobj3 = tnpc->wep_left;
                            myobj4 = tnpc->wep_right;
                            if (myobj3 && myobj4) {
                                //object in both hands
                                //can't attack with equiped items!
                                if ((spikedhelm_attack == 0) && (obji[sprlnk[myobj3->type & 1023]].v1 == 0) && (
                                        obji[sprlnk[myobj4->type & 1023]].v1 == 0))
                                    goto skipnpcattack;

                                if (spikedhelm_attack) {
                                    if ((obji[sprlnk[myobj3->type & 1023]].v1 == 0) && (
                                            obji[sprlnk[myobj4->type & 1023]].v1 == 0))
                                        spikedhelm_onlyattack = 1;
                                }
                            }

                            static unsigned char attack_item;
                            attack_item = FALSE;
                            myobj2 = tnpc->target;
                            if (tnpc->target->info & 4) crt = (creature *) myobj2->more;
                            else attack_item = TRUE;

                            if (tnpc->wait_disable) goto skipnpcattack; //can't move or attack!

                            if (tnpc->wait_attack) {
                                if (selected_partymember) goto skipnpcattack; //movement is manual!
                                if (tnpc->wait_walk) goto skipnpcattack; //can't move closer anyway!
                                //i. check minimum range of all equipped weapons
                                x2 = 65536; //distance required to attack (-1=can't attack!)
                                for (x3 = 0; x3 <= 1; x3++) {
                                    if (x3 == 0) myobj3 = tnpc->wep_right; //right hand weapon
                                    if (x3 == 1) myobj3 = tnpc->wep_left; //left hand weapon
                                    if (myobj3) {
                                        if (obji[sprlnk[myobj3->type & 1023]].v1) x4 = 1;
                                        else x4 = 65536;
                                        x5 = myobj3->type & 1023;
                                        if (x5 == OBJ_BOW) x4 = 8;
                                        if (x5 == OBJ_MAGIC_BOW) x4 = 8;
                                        if (x5 == OBJ_TRIPLE_CROSSBOW) x4 = 8;
                                        if (x5 == OBJ_SLING) x4 = 4;
                                        if (x5 == OBJ_LIGHTNING_WAND) x4 = 8;
                                        if (x5 == OBJ_FIRE_WAND) x4 = 8;
                                        if (x5 == OBJ_CROSSBOW) x4 = 8;
                                        if (x5 == OBJ_BOOMERANG) x4 = 8;
                                        if (x5 == OBJ_FLASK_OF_OIL) x4 = 8;
                                        if (x5 == OBJ_DAGGER) x4 = 8;
                                        if (x5 == OBJ_THROWING_AXE) x4 = 8;
                                        if (x5 == OBJ_SPEAR) x4 = 8;
                                        if (x5 == OBJ_HALBERD) x4 = 2; //halberd
                                        if (x5 == OBJ_MORNING_STAR) x4 = 2; //morning star
                                        if (obji[sprlnk[myobj3->type & 1023]].flags & 16) {
                                            x2 = x4;
                                            goto npcattack2hnd;
                                        }
                                        if (x4 < x2) x2 = x4;
                                    } else {
                                        //bare hands attack possible, close in on target
                                        x2 = 1;
                                    }
                                } //x3
                            npcattack2hnd:
                                if (spikedhelm_attack) x2 = 1; //set minimum range required to use weapon
                                if (tplayer->craft) x2 = 8;

                                //ii. if distance 2 target is more than minimum range close in on target
                                x = abs(myobj->x - myobj2->x);
                                y = abs(myobj->y - myobj2->y);
                                if (x > y) x4 = x;
                                else x4 = y; //x4=distance to target
                                if ((x4 > x2) && (x2 != 65536)) goto npcattackclose;
                                goto skipnpcattack; //doesn't need to move and can't attack!
                            } //tnpc->wait_attack


                            //if (crt->hp<=0){
                            //tnpc->order=1; tnpc->follow=tplayer->party[tplayer->selected_partymember]; goto skipnpcattack;
                            //}

                            //check distance?
                            x = abs(myobj->x - myobj2->x);
                            y = abs(myobj->y - myobj2->y);
                            if (x > y) x2 = x;
                            else x2 = y; //x2=distance to target


                            //if ((x<=1)&&(y<=1)) x2=1; //close


                            //add l&r hand dam
                            x = 0;
                            x9 = 0;

                            static unsigned char miss;
                            static unsigned char wep_dt[2];
                            wep_dt[0] = 0;
                            wep_dt[1] = 0;
                            //RESERVED! x,x9
                            miss = 0;


                            static unsigned char triplecrossbow_shot;
                            triplecrossbow_shot = 0;
                            static unsigned short triplecrossbow_shot_x2;
                            static unsigned short triplecrossbow_shot_y2;
                            static unsigned short triplecrossbow_shot_x3;
                            static unsigned short triplecrossbow_shot_y3;

                            static unsigned short npcattackweptype;


                            if (spikedhelm_onlyattack) {
                                if (x2 > 1) goto attacknotpossible; //close in
                                spikedhelm_attack = 0;
                                spikedhelm_onlyattack = 0; //clear set variables
                                myobj3 = NULL;
                                i3 = 1;
                                wep_dt[1] = 1;
                                x = 4;

                                if (tplayer->craft == NULL) OBJdir(myobj, myobj2->x, myobj2->y);

                                //add wait
                                f = (float) tnpc->d / 64.0f;
                                if (f > 1.0f) f = 1.0f;
                                f = 2.0f - (f * 0.25f);
                                tnpc->wait_attack += f;

                                //add sfx
                                i4 = SFnew(myobj2->x, myobj2->y);
                                sf[i4].type = 1; //attack
                                sf[i4].x = myobj2->x;
                                sf[i4].y = myobj2->y;
                                sf[i4].x2 = 0;
                                sf[i4].wait = 0.125f; //NULL
                                sf[i4].more = 1;

                                goto spikedhelm_entrypoint;
                            } else {
                                if (spikedhelm_attack) {
                                    if (x2 > 1) spikedhelm_attack = 0; //not within range
                                }
                            }

                            for (x3 = 0; x3 <= 1; x3++) {
                                npcattackweptype = 0;


                                if (x3 == 0) myobj3 = tnpc->wep_right; //right hand weapon
                                if (x3 == 1) myobj3 = tnpc->wep_left; //left hand weapon

                                if (tplayer->craft) {
                                    //ship cannons
                                    myobj3 = NULL;
                                    if (x3 == 0) {
                                        if (x2 <= 8) {
                                            //do a MAJOR jump into the attack section!!!
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) goto skip_hth1;
                                            i3 = SFnew(myobj2->x, myobj2->y);
                                            //destination is more important than the source
                                            sf[i3].type = 17;
                                            sf[i3].x = myobj->x;
                                            sf[i3].y = myobj->y;
                                            sf[i3].x2 = myobj2->x;
                                            sf[i3].y2 = myobj2->y;
                                            sf[i3].more = 0xFFFF;
                                            sf[i3].wait = 1;
                                            x += 30;
                                            goto skip_hth2;
                                        }
                                    }
                                    goto skip_hth1;
                                }

                                if (myobj3) {
                                    npcattackweptype = myobj3->type & 1023;


                                    if (x3) wep_dt[0] = obji[sprlnk[myobj3->type & 1023]].v5;
                                    else wep_dt[1] = obji[sprlnk[myobj3->type & 1023]].v5;


                                    //special case(s)


                                    if (x2 <= 8) {
                                        //long range

                                        if ((myobj3->type & 1023) == 47) {
                                            //halberd
                                            if (x2 == 2) {
                                                //only check if range 2
                                                if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                    //blocked! (will be processed as out of range)
                                                } else {
                                                    x7 = myobj2->x;
                                                    if (myobj->x > (myobj2->x + 1)) x7++;
                                                    if (myobj->x < (myobj2->x - 1)) x7--;
                                                    y7 = myobj2->y;
                                                    if (myobj->y > (myobj2->y + 1)) y7++;
                                                    if (myobj->y < (myobj2->y - 1)) y7--;

                                                    if (myobj2->x != x7) {
                                                        //discrepancy x
                                                        x5 = x7;
                                                        y5 = myobj2->y;
                                                        x6 = 0;
                                                        if (myobj9 = OBJfindlast(x5, y5)) {
                                                            if (myobj9->info & 8) myobj9 = (object *) myobj9->more;
                                                            if (myobj9->info & (2 + 4)) x6 = 1;
                                                            //something living is in the way
                                                        } //myobj9
                                                        if (bt[y5][x5] & 1024) x6 = 1; //character can walk there
                                                        if (x6 == 0) goto halberd_failedcheck;
                                                    }

                                                    if (myobj2->y != y7) {
                                                        //discrepancy y
                                                        x5 = myobj2->x;
                                                        y5 = y7;
                                                        x6 = 0;
                                                        if (myobj9 = OBJfindlast(x5, y5)) {
                                                            if (myobj9->info & 8) myobj9 = (object *) myobj9->more;
                                                            if (myobj9->info & (2 + 4)) x6 = 1;
                                                            //something living is in the way
                                                        } //myobj9
                                                        if (bt[y5][x5] & 1024) x6 = 1; //character can walk there
                                                        if (x6 == 0) goto halberd_failedcheck;
                                                    }

                                                    x7 = myobj2->x;
                                                    if (myobj->x > myobj2->x) x7++;
                                                    if (myobj->x < myobj2->x) x7--;
                                                    y7 = myobj2->y;
                                                    if (myobj->y > myobj2->y) y7++;
                                                    if (myobj->y < myobj2->y) y7--;

                                                    if ((x7 != myobj2->x) && (y7 != myobj2->y)) {
                                                        x5 = x7;
                                                        y5 = y7;
                                                        x6 = 0;
                                                        if (myobj9 = OBJfindlast(x5, y5)) {
                                                            if (myobj9->info & 8) myobj9 = (object *) myobj9->more;
                                                            if (myobj9->info & (2 + 4)) x6 = 1;
                                                            //something living is in the way
                                                        } //myobj9
                                                        if (bt[y5][x5] & 1024) x6 = 1; //character can walk there
                                                        if (x6 == 0) goto halberd_failedcheck;
                                                    }

                                                    goto hth_norangecheck;
                                                halberd_failedcheck:;
                                                } //OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)
                                            } //==2
                                        } //halberd

                                        if ((myobj3->type & 1023) == 40) {
                                            //morning star
                                            if (x2 == 2) {
                                                //only check if range 2
                                                if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                    //blocked! (will be processed as out of range)
                                                } else {
                                                    x7 = myobj2->x;
                                                    if (myobj->x > (myobj2->x + 1)) x7++;
                                                    if (myobj->x < (myobj2->x - 1)) x7--;
                                                    y7 = myobj2->y;
                                                    if (myobj->y > (myobj2->y + 1)) y7++;
                                                    if (myobj->y < (myobj2->y - 1)) y7--;

                                                    if (myobj2->x != x7) {
                                                        //discrepancy x
                                                        x5 = x7;
                                                        y5 = myobj2->y;
                                                        x6 = 0;
                                                        if (myobj9 = OBJfindlast(x5, y5)) {
                                                            if (myobj9->info & 8) myobj9 = (object *) myobj9->more;
                                                            if (myobj9->info & (2 + 4)) x6 = 1;
                                                            //something living is in the way
                                                        } //myobj9
                                                        if (bt[y5][x5] & 1024) x6 = 1; //character can walk there
                                                        if (x6 == 0) goto morningstar_failedcheck;
                                                    }

                                                    if (myobj2->y != y7) {
                                                        //discrepancy y
                                                        x5 = myobj2->x;
                                                        y5 = y7;
                                                        x6 = 0;
                                                        if (myobj9 = OBJfindlast(x5, y5)) {
                                                            if (myobj9->info & 8) myobj9 = (object *) myobj9->more;
                                                            if (myobj9->info & (2 + 4)) x6 = 1;
                                                            //something living is in the way
                                                        } //myobj9
                                                        if (bt[y5][x5] & 1024) x6 = 1; //character can walk there
                                                        if (x6 == 0) goto morningstar_failedcheck;
                                                    }

                                                    x7 = myobj2->x;
                                                    if (myobj->x > myobj2->x) x7++;
                                                    if (myobj->x < myobj2->x) x7--;
                                                    y7 = myobj2->y;
                                                    if (myobj->y > myobj2->y) y7++;
                                                    if (myobj->y < myobj2->y) y7--;

                                                    if ((x7 != myobj2->x) && (y7 != myobj2->y)) {
                                                        x5 = x7;
                                                        y5 = y7;
                                                        x6 = 0;
                                                        if (myobj9 = OBJfindlast(x5, y5)) {
                                                            if (myobj9->info & 8) myobj9 = (object *) myobj9->more;
                                                            if (myobj9->info & (2 + 4)) x6 = 1;
                                                            //something living is in the way
                                                        } //myobj9
                                                        if (bt[y5][x5] & 1024) x6 = 1; //character can walk there
                                                        if (x6 == 0) goto morningstar_failedcheck;
                                                    }

                                                    goto hth_norangecheck;
                                                morningstar_failedcheck:;
                                                } //OBJcheckbolt(myobj->x,myobj->y,myobj2->x,myobj2->y)
                                            } //==2
                                        } //morning star

                                        if ((myobj3->type & 1023) == 41) {
                                            //bow
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                OBJlist_last = NULL;
                                                for (y6 = 0; y6 < x5; y6++) {
                                                    if (OBJlist_list[y6]->type == 55) {
                                                        //arrow
                                                        OBJlist_list[y6]->more2--;
                                                        if (OBJlist_list[y6]->more2 == 0) OBJremove(OBJlist_list[y6]);
                                                        tnpc->upflags |= 32;
                                                        i3 = SFnew(myobj2->x, myobj2->y);
                                                        //destination is more important than the source
                                                        sf[i3].type = 2; //arrow
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
                                                    } //->type==55
                                                } //y6
                                                goto skipnpcattack;
                                            } //OBJcheckbolt
                                            goto skip_hth2;
                                        } //bow

                                        if ((myobj3->type & 1023) == 54) {
                                            //magic bow
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                OBJlist_last = NULL;
                                                for (y6 = 0; y6 < x5; y6++) {
                                                    if (OBJlist_list[y6]->type == 55) {
                                                        //arrow
                                                        OBJlist_list[y6]->more2--;
                                                        if (OBJlist_list[y6]->more2 == 0) OBJremove(OBJlist_list[y6]);
                                                        tnpc->upflags |= 32;
                                                        i3 = SFnew(myobj2->x, myobj2->y);
                                                        //destination is more important than the source
                                                        sf[i3].type = 2; //arrow
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
                                                    } //->type==55
                                                } //y6
                                                goto skipnpcattack;
                                            } //OBJcheckbolt
                                            goto skip_hth2;
                                        } //magic bow


                                        if ((myobj3->type & 1023) == 50) {
                                            //triple crossbow
                                            if (OBJcheckbolt(myobj->x, myobj->y, myobj2->x, myobj2->y)) {
                                                //it would hit the wall (ignore)
                                            } else {
                                                x5 = OBJlist((object *) tnpc->items->more); //x5=last object number
                                                OBJlist_last = NULL;
                                                for (y6 = 0; y6 < x5; y6++) {
                                                    if (OBJlist_list[y6]->type == 56) {
                                                        //bolt
                                                        if (OBJlist_list[y6]->more2 >= 3) {
                                                            //3 bolts
                                                            OBJlist_list[y6]->more2 -= 3;
                                                            if (OBJlist_list[y6]->more2 == 0)
                                                                OBJremove(
                                                                    OBJlist_list[y6]);
                                                            tnpc->upflags |= 32;

                                                            i3 = SFnew(myobj2->x, myobj2->y);
                                                            //destination is more important than the source
                                                            sf[i3].type = 6; //bolt
                                                            sf[i3].x = myobj->x;
                                                            sf[i3].y = myobj->y;
                                                            sf[i3].x2 = myobj2->x;
                                                            sf[i3].y2 = myobj2->y;
                                                            sf[i3].more = 0xFFFF;
                                                            sf[i3].wait = 1;

                                                            triplecrossbow_shot = 2;

                                                            //2 more shots
                                                            x6 = myobj2->x;
                                                            y6 = myobj2->y;
                                                            x7 = myobj2->x;
                                                            y7 = myobj2->y;
                                                            if ((x6 > myobj->x) && (y6 > myobj->y)) {
                                                                y6--;
                                                                x7--;
                                                                goto crossbow0;
                                                            }
                                                            if ((x6 > myobj->x) && (y6 < myobj->y)) {
                                                                x6--;
                                                                y7++;
                                                                goto crossbow0;
                                                            }
                                                            if ((x6 < myobj->x) && (y6 < myobj->y)) {
                                                                y6++;
                                                                x7++;
                                                                goto crossbow0;
                                                            }
                                                            if ((x6 < myobj->x) && (y6 > myobj->y)) {
                                                                x6++;
                                                                y7--;
                                                                goto crossbow0;
                                                            }
                                                            if (x6 > myobj->x) {
                                                                y6--;
                                                                y7++;
                                                                goto crossbow0;
                                                            }
                                                            if (x6 < myobj->x) {
                                                                y6++;
                                                                y7--;
                                                                goto crossbow0;
                                                            }
                                                            if (y6 < myobj->y) {
                                                                x6--;
                                                                x7++;
                                                                goto crossbow0;
                                                            }
                                                            if (y6 > myobj->y) {
                                                                x6++;
                                                                x7--;
                                                                goto crossbow0;
                                                            }
