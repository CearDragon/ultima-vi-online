// LHS-P5.5: part_c_traps_schedule -- CONTINUES MEGA C. HOT PATH.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_c_objupdate.cpp.
// Contents: traps/poison (trap_*, poisonremoved), schedules (SCHFND_retry,
// gotschedule). Move-only: no STL, no reorg.
                                        trap_gotpartymemberindex:
                                            tplayer->trapx[x2] = myobj2->x;
                                            tplayer->trapy[x2] = myobj2->y;
                                            if ((myobj2->info & (1 << 9)) == 0) {
                                                OBJsave(myobj2->x, myobj2->y);
                                                myobj2->info |= (1 << 9); //make trap visible
                                            }
                                            x2 = tnpc->hp >> 2;
                                            if (!x2) x2 = 1; //-25% health
                                            tnpc->hp -= x2;
                                            i3 = SFnew(myobj->x, myobj->y);
                                            sf[i3].type = 1; //attack
                                            sf[i3].x2 = x2;
                                            sf[i3].x = myobj->x;
                                            sf[i3].y = myobj->y;
                                            sf[i3].wait = 0.125f; //NULL
                                            sf[i3].more = 1;
                                            tnpc->upflags |= 2; //hp
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, tnpc->name);
                                            txtadd(t, " wounded!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        } //protection
                                    }
                                } //trap
                            trap_alreadyhurt:

                                if (myobj2->type == 320) {
                                    //sleep field
                                    if ((myobj2->more2 == tplayer->id) || (myobj2->more2 == 0)) {
                                        if ((tnpc->flags & 4) == 0) {
                                            f = 1.0f;
                                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                                if (tnpc->protection == 0) {
                                                    tnpc->flags |= 4; //asleep!
                                                    f = 8.0f + rnd * 24.0f;
                                                    if (tnpc->wait_disable < f) tnpc->wait_disable = f;
                                                }
                                            }
                                        }
                                    }
                                }

                                if (myobj2->type == 53) {
                                    //web paralyze player
                                    if ((myobj2->more2 == tplayer->id) || (myobj2->more2 == 0)) {
                                        if ((tnpc->flags & 8) == 0) {
                                            f = 0.5f;
                                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                                x3 = rnd * 3;
                                                if (x3 == 0) {
                                                    f = 4.0f + rnd * 12.0f;
                                                    if (tnpc->wait_disable < f) tnpc->wait_disable = f;
                                                    tnpc->flags |= 8; //paralyzed
                                                    if (tnpc->player) {
                                                        txtset(t, "?");
                                                        t->d2[0] = 8;
                                                        txtadd(t, tnpc->name);
                                                        txtadd(t, " is stuck in a web!");
                                                        NET_send(NETplayer, tnpc->player->net, t);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                if (myobj2->type == 318) {
                                    //poison field
                                    if ((myobj2->more2 == tplayer->id) || (myobj2->more2 == 0)) {
                                        if (tnpc->protection == 0) {
                                            if ((tnpc->flags & 1) == 0) {
                                                tnpc->flags |= 1; //poison NPC
                                                tnpc->upflags |= 2; //hp
                                            }
                                        }
                                    }
                                }

                                if (myobj2->type == 317) {
                                    //fire field -4hp/sec
                                    if ((myobj2->more2 == tplayer->id) || (myobj2->more2 == 0)) {
                                        f = 0.5f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            if (tnpc->protection == 0) {
                                                if (tnpc->hp) tnpc->hp--; //-2!
                                                if (tnpc->hp) tnpc->hp--;
                                                i3 = SFnew(myobj->x, myobj->y);
                                                sf[i3].type = 1; //attack
                                                sf[i3].x2 = 2;
                                                sf[i3].x = myobj->x;
                                                sf[i3].y = myobj->y;
                                                sf[i3].wait = 0.125f; //NULL
                                                sf[i3].more = 1;
                                                tnpc->upflags |= 2; //hp
                                            }
                                        } //f
                                    } //id
                                } //fire field


                                if ((myobj2->type & 1023) == 366) {
                                    //tanglevine tendril
                                    f = 0.5f;
                                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                                        if (tnpc->protection == 0) {
                                            if (tnpc->horse == NULL) {
                                                if (tnpc->hp) tnpc->hp--;
                                                i3 = SFnew(myobj->x, myobj->y);
                                                sf[i3].type = 1; //attack
                                                sf[i3].x2 = 1;
                                                sf[i3].x = myobj->x;
                                                sf[i3].y = myobj->y;
                                                sf[i3].wait = 0.125f; //NULL
                                                sf[i3].more = 1;
                                                tnpc->upflags |= 2; //hp
                                            } //not on horse
                                        }
                                    } //f
                                } //tanglevine tendril


                                //sleep field?
                                //concept when player first steps onto sleep field they fall asleep, but eventually they awake
                                //and have a brief oportunity to escape, before being put back to sleep again
                                //sleep usually lasts a very long and annoying time, like 16 seconds!
                            }

                            //experiential xp
                            f = 256.0f;
                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                if (tplayer->idle <= 256) {
                                    if (tplayer->party[0]) {
                                        if ((object *) tplayer->party[0]->more == myobj) {
                                            tnpc->exp++;
                                            tnpc->upflags |= 8;
                                        }
                                    }
                                } //idle
                            }

                            x = bt[myobj->y][myobj->x] & 255;
                            if ((x >= 3) && (x <= 6)) {
                                if (tplayer->craft) goto poison_skip;
                                if (tnpc->horse) goto poison_skip;
                                if (tnpc->boots != NULL) {
                                    if (tnpc->boots->type == 28) {
                                        goto poison_skip;
                                    } //->type!=SWAMPBOOTS
                                } //->boots!=NULL
                                if (tnpc->protection) goto poison_skip;
                                if ((tnpc->flags & 1) == 0) {
                                    tnpc->flags |= 1; //poison NPC
                                    tnpc->upflags |= 2; //hp
                                }
                            poison_skip:;
                            }

                            if ((x >= 221) && (x <= 223)) {
                                //lava!
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    if ((tnpc->protection == 0) && (tplayer->craft == NULL)) {
                                        x2 = 0;
                                        if (x == 221) x2 = 8;
                                        if (x == 222) x2 = 16;
                                        if (x == 223) x2 = 32;
                                        tnpc->hp -= x2;
                                        i3 = SFnew(myobj->x, myobj->y);
                                        sf[i3].type = 1; //attack
                                        sf[i3].x2 = x2;
                                        sf[i3].x = myobj->x;
                                        sf[i3].y = myobj->y;
                                        sf[i3].wait = 0.125f; //NULL
                                        sf[i3].more = 1;
                                        tnpc->upflags |= 2; //hp
                                    } //->protection==0
                                }
                            }


                            if (tnpc->flags & 1) {
                                //poisoned?
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    if (tnpc->lev == 1 && tnpc->exp < 1600) {
                                        if ((rnd * 32.0f) < 1.0f) {
                                            if (tplayer->party[0]) {
                                                if ((npc *) tplayer->party[0]->more == tnpc) {
                                                    tnpc->flags &= (65535 - 1); //remove poisoned flag
                                                    tnpc->upflags |= 2;
                                                    txtset(t, "?");
                                                    t->d2[0] = 8;
                                                    txtadd(t, "Inexperienced character's poison cured!");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto poisonremoved;
                                                }
                                            }
                                        }
                                    }
                                    x = rnd * 8;
                                    if (x == 0) {
                                        tnpc->hp--;
                                        i3 = SFnew(myobj->x, myobj->y);
                                        sf[i3].type = 1; //attack
                                        sf[i3].x2 = 1;
                                        sf[i3].x = myobj->x;
                                        sf[i3].y = myobj->y;
                                        sf[i3].wait = 0.125f; //NULL
                                        sf[i3].more = 1;
                                        tnpc->upflags |= 2;
                                    }
                                }
                            } //poisoned
                        poisonremoved:

                            //horse food reduction
                            if (tnpc->horse) {
                                f = 60.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    x = rnd * 180;
                                    if (x == 0) {
                                        crt2 = (creature *) tnpc->horse->more;
                                        x = (crt2->respawn_x >> 4) & 3;
                                        if (x) x--;
                                        crt2->respawn_x &= (0xFFFF - (3 << 4));
                                        crt2->respawn_x += (x << 6);
                                    } //x==0
                                } //f
                            } //->horse

                            if (tplayer->karma <= 0) {
                                if (tplayer->party[0]) {
                                    if ((npc *) tplayer->party[0]->more == tnpc) {
                                        tplayer->karma = 0;
                                        f = 1.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            tnpc->hp--;
                                            tnpc->upflags |= 2;
                                        }
                                    }
                                }
                            }


                            //add mp
                            //x3=rnd*(32+(tnpc->i>>1)); //int can only double spell effectiveness!

                            if ((float) tnpc->i / 4.0 >= 32.0) {
                                f = 0.05f;
                            } else {
                                f = 32.0f - ((float) tnpc->i / 4.0f);
                            }
                            if (int((ett / f)) != int((ett / f) - (et / f))) {
                                if (tnpc->mp != tnpc->mp_max) {
                                    tnpc->mp++;
                                    if (tnpc->mp > tnpc->mp_max) tnpc->mp = tnpc->mp_max;
                                    tnpc->upflags |= 4;
                                }
                            }

                            //sleeping? WAKE UP!
                            if (tnpc->flags & 4) {
                                if (tnpc->wait_disable <= 0) {
                                    tnpc->flags -= 4;
                                }
                            }


                            //light ->~17min
                            //great light ->~68min
                            if (tnpc->light >= 2) {
                                f = 32.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    tnpc->light--;
                                }
                            }


                            if (tnpc->light == 1) tnpc->light = 0;

                            //tnpc->light=0;
                            //burn down torch?
                            //f=64.0f; if (int((ett/f))!=int((ett/f)-(et/f))){


                            if (tnpc->wep_left != NULL) {
                                if (tnpc->wep_left->type == (90 + 1024)) {
                                    x = (tnpc->wep_left->info >> 9) & 15;
                                    if (x == 1) {
                                        //burn down
                                        tnpc->wep_left = NULL;
                                        tnpc->upflags |= 64;
                                    } else {
                                        f = 64.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            if (x == 0) x = 16;
                                            x--;
                                            tnpc->wep_left->info &= (65535 - (15 << 9));
                                            tnpc->wep_left->info |= (x << 9);
                                        }
                                        if (tnpc->light == 0) tnpc->light = 1;
                                    }
                                }
                            }

                            if (tnpc->wep_right != NULL) {
                                if (tnpc->wep_right->type == (90 + 1024)) {
                                    x = (tnpc->wep_right->info >> 9) & 15;
                                    if (x == 1) {
                                        //burn down
                                        tnpc->wep_right = NULL;
                                        tnpc->upflags |= 64;
                                    } else {
                                        f = 64.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            if (x == 0) x = 16;
                                            x--;
                                            tnpc->wep_right->info &= (65535 - (15 << 9));
                                            tnpc->wep_right->info |= (x << 9);
                                        }
                                        if (tnpc->light == 0) tnpc->light = 1;
                                    }
                                }
                            }


                            //}//f64

                            //f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){

                            //if (tnpc->flags&2) tnpc->flags-=2;


                            if (tnpc->protection) {
                                //protection!
                                f = 4.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    tnpc->protection--;
                                    if (tnpc->protection == 0) tnpc->upflags |= 64; //update ->arm value
                                }
                            }

                            //NEWCODE
                            if (tnpc->negatemagic) {
                                //negatemagic
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    tnpc->negatemagic--;
                                }
                            }
                            //NEWCODEEND

                            //invisibility ring(s)
                            tnpc->flags &= (255 - 2);
                            if (tnpc->invisibility) {
                                tnpc->flags |= 2;
                                f = 1.0f;
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    tnpc->invisibility--;
                                }
                                goto skipinvisrings;
                            }


                            if (tnpc->ring_left != NULL) {
                                if (tnpc->ring_left->type == 258) {
                                    //invis
                                    x = tnpc->ring_left->more2;
                                    if (x == 0) x = 64 * 4; //~4min
                                    f = 1.0f;
                                    if (int((ett / f)) != int((ett / f) - (et / f))) x--;
                                    tnpc->ring_left->more2 = x;
                                    if (x == 0) {
                                        tnpc->ring_left = NULL;
                                        tnpc->upflags |= 64;
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, tnpc->name);
                                        txtadd(t, "'s invisibility ring vanished!");
                                        NET_send(NETplayer, tplayer->net, t);
                                    } else {
                                        tnpc->flags |= 2;
                                        goto skipinvisrings;
                                    }
                                }
                            }
                            if (tnpc->ring_right != NULL) {
                                if (tnpc->ring_right->type == 258) {
                                    x = tnpc->ring_right->more2;
                                    if (x == 0) x = 64 * 4; //~4min
                                    f = 1.0f;
                                    if (int((ett / f)) != int((ett / f) - (et / f))) x--;
                                    tnpc->ring_right->more2 = x;
                                    if (x == 0) {
                                        tnpc->ring_right = NULL;
                                        tnpc->upflags |= 64;
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, tnpc->name);
                                        txtadd(t, "'s invisibility ring vanished!");
                                        NET_send(NETplayer, tplayer->net, t);
                                    } else { tnpc->flags |= 2; }
                                }
                            }
                        skipinvisrings:
                            if (tnpc->flags & 2) {
                                if (tplayer->craft) tnpc->flags &= (255 - 2);
                            }

                            if (tnpc->ring_left != NULL) {
                                if (tnpc->ring_left->type == 257) {
                                    //regeneration ring
                                    if ((tnpc->hp > 0) && (tnpc->hp < tnpc->hp_max)) {
                                        f = 1.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            x = tnpc->ring_left->more2;
                                            if (x == 0) x = 128; //128hp
                                            x--;
                                            tnpc->ring_left->more2 = x;
                                            tnpc->hp++;
                                            tnpc->upflags |= 2;
                                            if (x == 0) {
                                                tnpc->ring_left = NULL;
                                                tnpc->upflags |= 64;
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, tnpc->name);
                                                txtadd(t, "'s regeneration ring vanished!");
                                                NET_send(NETplayer, tplayer->net, t);
                                            }
                                        } //f
                                    } //need hp
                                }
                            }
                            if (tnpc->ring_right != NULL) {
                                if (tnpc->ring_right->type == 257) {
                                    //regeneration ring
                                    if ((tnpc->hp > 0) && (tnpc->hp < tnpc->hp_max)) {
                                        f = 1.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            x = tnpc->ring_right->more2;
                                            if (x == 0) x = 128; //128hp
                                            x--;
                                            tnpc->ring_right->more2 = x;
                                            tnpc->hp++;
                                            tnpc->upflags |= 2;
                                            if (x == 0) {
                                                tnpc->ring_right = NULL;
                                                tnpc->upflags |= 64;
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, tnpc->name);
                                                txtadd(t, "'s regeneration ring vanished!");
                                                NET_send(NETplayer, tplayer->net, t);
                                            }
                                        } //f
                                    } //need hp
                                }
                            }
                            //increase int as well attempt - Cear
                            if (tnpc->ring_right != NULL) {
                                if (tnpc->ring_right->type == 257) //regeneration ring
                                {
                                    if ((tnpc->mp > 0) && (tnpc->mp < tnpc->mp_max)) {
                                        f = 1.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            x = tnpc->ring_right->more2;
                                            if (x == 0) x = 128; //128hp
                                            x--;
                                            tnpc->ring_right->more2 = x;
                                            tnpc->mp++;
                                            tnpc->upflags |= 4;
                                            if (x == 0) {
                                                tnpc->ring_right = NULL;
                                                tnpc->upflags |= 64;
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, tnpc->name);
                                                txtadd(t, "'s regeneration ring vanished!");
                                                NET_send(NETplayer, tplayer->net, t);
                                            }
                                        } //f
                                    } //need hp
                                }
                            }
                            if (tnpc->ring_left != NULL) {
                                if (tnpc->ring_left->type == 257) //regeneration ring
                                {
                                    if ((tnpc->mp > 0) && (tnpc->mp < tnpc->mp_max)) {
                                        f = 1.0f;
                                        if (int((ett / f)) != int((ett / f) - (et / f))) {
                                            x = tnpc->ring_left->more2;
                                            if (x == 0) x = 128; //128hp
                                            x--;
                                            tnpc->ring_left->more2 = x;
                                            tnpc->mp++;
                                            tnpc->upflags |= 4;
                                            if (x == 0) {
                                                tnpc->ring_right = NULL;
                                                tnpc->upflags |= 64;
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, tnpc->name);
                                                txtadd(t, "'s regeneration ring vanished!");
                                                NET_send(NETplayer, tplayer->net, t);
                                            }
                                        } //f
                                    } //need hp
                                }
                            }


                            if (tnpc->ring_left != NULL) {
                                if (tnpc->ring_left->type == 256) {
                                    //protection ring
                                    f = 1.0f;
                                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                                        x = tnpc->ring_left->more2;
                                        if (x == 0) x = 4096; //~1hr
                                        x--;
                                        tnpc->ring_left->more2 = x;
                                        if (x == 0) {
                                            tnpc->ring_left = NULL;
                                            tnpc->upflags |= 64;
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, tnpc->name);
                                            txtadd(t, "'s protection ring vanished!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                    } //f
                                }
                            }
                            if (tnpc->ring_right != NULL) {
                                if (tnpc->ring_right->type == 256) {
                                    //
                                    f = 1.0f;
                                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                                        x = tnpc->ring_right->more2;
                                        if (x == 0) x = 4096; //~1hr
                                        x--;
                                        tnpc->ring_right->more2 = x;
                                        if (x == 0) {
                                            tnpc->ring_right = NULL;
                                            tnpc->upflags |= 64;
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, tnpc->name);
                                            txtadd(t, "'s protection ring vanished!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                    } //f
                                }
                            }


                            if (tnpc->food) {
                                //has food
                                f = 16.0f;

                                //make fighters (high STR players) heal faster using food
                                //STR 16 is same, STR 48 is double
                                if (tnpc->s > 16) {
                                    f2 = tnpc->s;
                                    f2 = (f2 - 16.0f) / 4.0f;
                                    f -= f2;
                                }
                                if (int((ett / f)) != int((ett / f) - (et / f))) {
                                    tnpc->food--;
                                    if (!tnpc->food) {
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, tnpc->name);
                                        txtadd(t, " is hungry!");
                                        NET_send(NETplayer, tplayer->net, t);
                                    }
                                    if (tnpc->hp < tnpc->hp_max) tnpc->hp++;
                                    tnpc->upflags |= 2;
                                }
                            }


                            //}//f1
                        } //tplayer


                        if (tplayer == NULL) {
                            //NPC, not party member
                            if (tnpc->wait_walk == 0) {
                                //wait
                                if (tnpc->order == 2) {
                                    //schedule

                                    //*IMPORTANT* needs to check for nearest schedule, regardless of order, 
                                    //*MORE IMPORTANTLY* it needs to be day indepentant, reading a schedule from the prev
                                    //day if necessary

                                    //assuming a valid schedule exists...
                                    x3 = -1;
                                    f = btime + (bday * 24 - 24);
                                SCHFND_retry:
                                    f3 = 65536;

                                    //find nearest prev schedule in the list
                                    for (x2 = 0; x2 <= 31; x2++) {
                                        //find schedule
                                        if (tnpc->schedule[x2].hour != 255) {
                                            f2 = tnpc->schedule[x2].hour;
                                            if (tnpc->schedule[x2].day) {
                                                f2 += (tnpc->schedule[x2].day * 24 - 24);
                                                x4 = 1;
                                            } else {
                                                f2 += (bday * 24 - 24);
                                                x4 = 0; //use current day
                                            }
                                            if (f2 < f) {
                                                if ((f - f2) <= f3) {
                                                    if (x4) {
                                                        x3 = x2;
                                                    } else {
                                                        if ((f - f2) < f3) x3 = x2;
                                                    } //x4
                                                    f3 = (f - f2); //new closest sch time
                                                } //(f-f2)<=f3
                                            } //f2<f
                                        } //!=255
                                    } //x2
                                    if (x3 == -1) {
                                        x3--;
                                        f = 24 * 8;
                                        goto SCHFND_retry;
                                    }
                                    if (x3 == -2) goto NPC_skip1;
                                    x2 = x3;
                                    tnpc->schedule_last = tnpc->schedule[x2].type;

                                    if ((abs(myobj->x - tnpc->schedule[x2].x) == 0) && (
                                            abs(myobj->y - tnpc->schedule[x2].y) == 0)) {
                                        tnpc->schedule_i = x2;
                                    }
                                    if (x2 != tnpc->schedule_i) tnpc->schedule_i = -1;

                                    if (tnpc->schedule_i == x2) {
                                        if ((tnpc->schedule[x2].type == 0x8F) || (tnpc->schedule[x2].type == 0x90)) {
                                            //wander aimlessly!!!
                                            if ((abs(myobj->x - tnpc->schedule[x2].x) <= 4) && (
                                                    abs(myobj->y - tnpc->schedule[x2].y) <= 4)) {
                                                x = rnd * 4;
                                                x2 = 0;
                                                y2 = 0;
                                                if (x == 0) x2--;
                                                if (x == 1) x2++;
                                                if (x == 2) y2--;
                                                if (x == 3) y2++;
                                                tnpc->wait_walk += 1.0f + rnd * 2.0f;
                                                tnpc->pathn = 0; //clear pathfind
                                                goto npc_walk; //walk randomly
                                            } //<4
                                            tnpc->schedule_i = -1;
                                        }
                                    }

                                    /*

                  for (x2=0;x2<=31;x2++){ //find schedule
                  if (x2==31) goto gotschedule; //last index


                  if ((tnpc->schedule[x2].day==0)||(tnpc->schedule[x2].day==bday)){ //valid day
                  f=tnpc->schedule[x2].hour;
                  x3=1;
                  sch_sametimeday:
                  f2=tnpc->schedule[x2+x3].hour;
                  if (f2==255) goto gotschedule; //next index is NULL (use the current schedule)
                  if ((tnpc->schedule[x2+x3].day!=0)&&(tnpc->schedule[x2+x3].day!=bday)){x3++; goto sch_sametimeday;} //next sch is for a different day, ignore
                  if (f2==f) {x3++; goto sch_sametimeday;} //next sch is for the same time, ignore
                  if ((btime>=f)&&(btime<=f2)) goto gotschedule;
                  } //valid day
                  }//schedule 0-31 loop
                  goto NPC_skip1; //schedule not found ->exit
                  gotschedule: //index x2
                  if (btime<tnpc->schedule[x2].hour) NPC_skip1; //false schedule read by accident!
                  */

                                    //gotschedule:

                                    //goto .x,.y

                                    x3 = myobj->x - tnpc->schedule[x2].x;
                                    y3 = myobj->y - tnpc->schedule[x2].y;
                                    if ((x3 == 0) && (y3 == 0)) goto NPC_skip1;

                                    if (tnpc->pathn == 0) {
                                        /*
                    tnpc->path_max=pathfind(tnpc->path,myobj->x,myobj->y,tnpc->schedule[x2].x,tnpc->schedule[x2].y,NULL);
                    //if (tnpc->path_max==0) tnpc->path_max=pathfind(tnpc->path,myobj->x,myobj->y,tnpc->schedule[x2].x,tnpc->schedule[x2].y,NULL);
                    if (tnpc->path_max==0){ tnpc->pathn=0; tnpc->wait_walk+=1.0f+rnd*2.0f; goto NPC_skip1;}
                    tnpc->pathn=tnpc->path_max;
                    */
                                        /*
                    z=abs(myobj2->x-myobj->x); z2=abs(myobj2->y-myobj->y); if (z2>z) z=z2;
                    z+=32; if (z>250) z=250;
                    WPF_OBJECT=myobj; wpf_pathfind(tnpc->path,myobj->x,myobj->y,myobj2->x,myobj2->y,z,0,1);//|1|PATHFIND THROUGH OWN PARTY+16
                    if (WPF_RETURN==WPF_PATHFOUND){
                    if (WPF_PATHLENGTH){
                    tnpc->path_max=WPF_PATHLENGTH; tnpc->pathn=WPF_PATHLENGTH;
                    goto follow_getpath;
                    }
                    }
                    */
                                        /*
                    x3=abs(tnpc->schedule[x2].x-myobj->x); y3=abs(tnpc->schedule[x2].y-myobj->y); if (y3>x3) x3=y3;
                    x3+=32;
                    //if (x2>250) exit2(55);

                    if (x3>250){
                    txtnumint(t5,tnpc->converse); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                    tnpc->pathn=0; tnpc->wait_walk+=1.0f+rnd*2.0f; goto NPC_skip1;
                    }
                    */

                                        //if (x3>dv) dv=x3;

                                        x3 = abs(tnpc->schedule[x2].x - myobj->x);
                                        y3 = abs(tnpc->schedule[x2].y - myobj->y);
                                        if (y3 > x3) x3 = y3;
                                        x3 += 64;
                                        if (x3 > 250) {
                                            x3 = 250;
                                        }
                                        WPF_OBJECT = myobj;
                                        wpf_pathfind(tnpc->path, myobj->x, myobj->y, tnpc->schedule[x2].x,
                                                     tnpc->schedule[x2].y, x3, 0, 4);
                                        if (WPF_RETURN == WPF_NOPATHFOUND) {
                                            tnpc->pathn = 0;
                                            tnpc->wait_walk += 1.0f + rnd * 2.0f;
                                            goto NPC_skip1;
                                        }
                                        tnpc->path_max = WPF_PATHLENGTH;
                                        tnpc->pathn = WPF_PATHLENGTH;
                                    }
                                    //path
                                    x2 = 0;
                                    y2 = 0;
                                    if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFright) x2 = 1;
                                    if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFleft) x2 = -1;
                                    if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFup) y2 = -1;
                                    if (tnpc->path[tnpc->path_max - tnpc->pathn] == PFdown) y2 = 1;
                                    if (tnpc->pathn > 0) {
                                        //fix for pathfinding crash.
                                        tnpc->pathn--;
                                    }

