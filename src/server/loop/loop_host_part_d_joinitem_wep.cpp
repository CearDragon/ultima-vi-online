// LHS-P6.6: part_d_joinitem_wep -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: join-item (joinitem) + weapon equip (wep_right_skip,
// wep_left_skip). Move-only.
                                                joinitem:
                                                    if (myobj3->type == tplayer->mobj->type) {
                                                        //same type
                                                        if ((tplayer->mobj->info & 128) == (myobj3->info & 128)) {
                                                            if (((myobj3->type & 1023) == 90) && (
                                                                    myobj3->info != tplayer->mobj->info))
                                                                goto cantjoin;
                                                            //cant join half burnt torches
                                                            myobj3->more2 += tplayer->mobj->more2;
                                                            OBJrelease(tplayer->mobj);
                                                            tplayer->mobj = NULL;
                                                            goto joinitem_finished;
                                                        } //same stealing type
                                                    } //same type
                                                    if (myobj3->next) {
                                                        myobj3 = (object *) myobj3->next;
                                                        goto joinitem;
                                                    }
                                                } //4096
                                            cantjoin:

                                                //if (obji[sprlnk[myobj2->type&1023]+(myobj2->type>>10)].flags&4096){
                                                //scan inventory list and append object if possible
                                                //}else{

                                                tplayer->mobj->next = myobj2;
                                                myobj2->prev = tplayer->mobj;
                                                tplayer->mobj->prev = myobj;
                                                myobj->more = tplayer->mobj;
                                                tplayer->mobj = NULL; //clear selected mouse object
                                                //}
                                            } else {
                                                tplayer->mobj->next = NULL;
                                                tplayer->mobj->prev = myobj;
                                                myobj->more = tplayer->mobj;
                                                tplayer->mobj = NULL; //clear selected mouse object
                                            }
                                        joinitem_finished:;
                                        } //weight

                                        goto skip_pickup; //mobj may have been set to 0!
                                    }
                                } //mx,my
                                //quest_skipdrop:

                                //drop equip item
                                //2003: no switching
                                x2 = tplayer->mx;
                                y2 = tplayer->my;
                                //NEWCODE
                                if (U6O_DEBUG) {
                                    goto debug_skipweightcheck; //for debugging
                                }

                                if ((tnpc->wt + WTfind(tplayer->mobj)) <= tnpc->wt_max) {
                                debug_skipweightcheck:;
                                    //NEWCODEEND
                                    x3 = helmx;
                                    y3 = helmy;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->helm == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 1) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }
                                                    stealing(tplayer, tplayer->mobj);
                                                    tnpc->helm = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = wep_rightx;
                                    y3 = wep_righty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->wep_right == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & (8 + 16)) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tnpc->wep_left != NULL) {
                                                        if (obji[sprlnk[tnpc->wep_left->type & 1023] + (
                                                                     tnpc->wep_left->type >> 10)].flags & 16)
                                                            goto
                                                                    skip_pickup;
                                                    }
                                                    if (obji[sprlnk[tplayer->mobj->type & 1023] + (
                                                                 tplayer->mobj->type >> 10)].flags & 16) {
                                                        //2 handed
                                                        if (tnpc->wep_left != NULL) goto skip_pickup;
                                                    }
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }

                                                    stealing(tplayer, tplayer->mobj);

                                                    if ((tplayer->mobj->type & 1023) == 83) {
                                                        //oil flask
                                                        if (tplayer->mobj->more2 > 1) {
                                                            //seperate item
                                                            myobj9 = OBJnew();
                                                            myobj9->type = 83;
                                                            myobj9->more2 = tplayer->mobj->more2 - 1;
                                                            tplayer->mobj->more2 = 1;
                                                            tnpc->wep_right = tplayer->mobj;
                                                            tplayer->mobj = myobj9;
                                                            goto wep_right_skip;
                                                        } //>1
                                                    } //83

                                                    if ((tplayer->mobj->type & 1023) == 90) {
                                                        //torch
                                                        //tnpc->light=1;
                                                        tplayer->mobj->type = 90 + 1024;
                                                        if (tplayer->mobj->more2 > 1) {
                                                            //seperate item
                                                            myobj9 = OBJnew();
                                                            myobj9->type = 90;
                                                            myobj9->more2 = tplayer->mobj->more2 - 1;
                                                            tplayer->mobj->more2 = 1;
                                                            tnpc->wep_right = tplayer->mobj;
                                                            tplayer->mobj = myobj9;
                                                            goto wep_right_skip;
                                                        } //>1
                                                    } //90

                                                    if (tplayer->mobj->type == 57) {
                                                        //ready spellbook
                                                        tnpc->upflags |= 128;
                                                        // c222 keep lastused spellbook
                                                        if (easymodehostn1)
                                                            playerspellbook = tplayer->mobj;
                                                    }

                                                    tnpc->wep_right = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                wep_right_skip:
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = wep_leftx;
                                    y3 = wep_lefty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->wep_left == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & (8 + 16)) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tnpc->wep_right != NULL) {
                                                        if (obji[sprlnk[tnpc->wep_right->type & 1023] + (
                                                                     tnpc->wep_right->type >> 10)].flags & 16)
                                                            goto
                                                                    skip_pickup;
                                                    }
                                                    if (obji[sprlnk[tplayer->mobj->type & 1023] + (
                                                                 tplayer->mobj->type >> 10)].flags & 16) {
                                                        //2 handed
                                                        if (tnpc->wep_right != NULL) goto skip_pickup;
                                                    }
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }

                                                    stealing(tplayer, tplayer->mobj);

                                                    if ((tplayer->mobj->type & 1023) == 83) {
                                                        //oil flask
                                                        if (tplayer->mobj->more2 > 1) {
                                                            //seperate item
                                                            myobj9 = OBJnew();
                                                            myobj9->type = 83;
                                                            myobj9->more2 = tplayer->mobj->more2 - 1;
                                                            tplayer->mobj->more2 = 1;
                                                            tnpc->wep_left = tplayer->mobj;
                                                            tplayer->mobj = myobj9;
                                                            goto wep_left_skip;
                                                        } //>1
                                                    } //83

                                                    if ((tplayer->mobj->type & 1023) == 90) {
                                                        //torch
                                                        //tnpc->light=1;
                                                        tplayer->mobj->type = 90 + 1024;
                                                        if (tplayer->mobj->more2 > 1) {
                                                            //seperate item
                                                            myobj9 = OBJnew();
                                                            myobj9->type = 90;
                                                            myobj9->more2 = tplayer->mobj->more2 - 1;
                                                            tplayer->mobj->more2 = 1;
                                                            tnpc->wep_left = tplayer->mobj;
                                                            tplayer->mobj = myobj9;
                                                            goto wep_left_skip;
                                                        } //>1
                                                    } //90

                                                    if (tplayer->mobj->type == 57) {
                                                        //ready spellbook
                                                        tnpc->upflags |= 128;
                                                        // c222 keep lastused spellbook
                                                        if (easymodehostn1)
                                                            playerspellbook = tplayer->mobj;
                                                    }

                                                    tnpc->wep_left = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                wep_left_skip:
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = armourx;
                                    y3 = armoury;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->armour == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 4) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }

                                                    stealing(tplayer, tplayer->mobj);

                                                    tnpc->armour = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = bootsx;
                                    y3 = bootsy;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->boots == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 64) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }
                                                    stealing(tplayer, tplayer->mobj);

                                                    tnpc->boots = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = neckx;
                                    y3 = necky;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->neck == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 2) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }
                                                    stealing(tplayer, tplayer->mobj);

                                                    tnpc->neck = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = ring_leftx;
                                    y3 = ring_lefty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->ring_left == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 32) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }

                                                    stealing(tplayer, tplayer->mobj);

                                                    //if ((tplayer->mobj->type&1023)==258){ //invisibility ring (oooooh!)
                                                    //tnpc->flags|=2; //set invisibility flag
                                                    //}//258

                                                    tnpc->ring_left = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }

                                    x3 = ring_rightx;
                                    y3 = ring_righty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->ring_right == NULL) {
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 32) {
                                                if ((tnpc->wt2 + WTfind(tplayer->mobj)) <= tnpc->wt2_max) {
                                                    if (tplayer->mobj->info & 256) {
                                                        if (i) {
                                                            //quest item->party member
                                                            txtset(t, "?");
                                                            t->d2[0] = 8;
                                                            txtadd(t, "You cannot put a quest item here!");
                                                            NET_send(NETplayer, tplayer->net, t);
                                                            goto skip_pickup;
                                                        }
                                                    }

                                                    stealing(tplayer, tplayer->mobj);


                                                    tnpc->ring_right = tplayer->mobj;
                                                    tplayer->mobj = NULL;
                                                    tnpc->upflags |= 64; //update flags
                                                    goto skip_pickup;
                                                }
                                            }
                                        }
                                    }
                                } //weight check!
                            } //mbclick
                        } //valid i(mf)
                    } //i(mf)<=7


                    //drop item on primary surface
                    if ((tplayer->mf == 255) && (tplayer->key & KEYmbclick)) {
                        x = tplayer->mx / 32;
                        y = tplayer->my / 32;
                        x += tpx;
                        y += tpy;
                        myobj = OBJfindlast(x, y);


                        if (myobj != NULL) {
                            i = myobj->type;
                            i2 = sprlnk[i & 1023];
                            //i2=i>>10; //26/11/2004 removed for Sherry compatibility


                            if (tplayer->action == 1) {
                                //use item on item


                                //PROXIMITY CHECK REQUIRED HERE!
                                z2 = 1;
                                for (z = 0; z <= 7; z++) {
                                    if (myobj2 = tplayer->party[z]) {
                                        if (abs(myobj2->x - myobj->x) <= 1) {
                                            if (abs(myobj2->y - myobj->y) <= 1) {
                                                z2 = 0;
                                            }
                                        }
                                    }
                                }
                                if (z2) goto skip_pickup;

                                if ((tplayer->mobj->type & 1023) == 178) {
                                    //empty bucket
                                    if ((myobj->type & 1023) == 428) {
                                        //cow
                                        tplayer->mobj->type = 180; //milk bucket
                                        tplayer->mobj_last = NULL; //->send message
                                        goto skip_pickup;
                                    }
                                    if ((myobj->type & 1023) == 233) {
                                        //well
                                        tplayer->mobj->type = 179; //water bucket
                                        tplayer->mobj_last = NULL; //->send message
                                        goto skip_pickup;
                                    }
                                }

                                if ((tplayer->mobj->type & 1023) == 183) {
                                    //empty jar
                                    if ((myobj->type & 1023) == 182) {
                                        //beehive
                                        if ((myobj->info & (15 << 9)) == 0) {
                                            //empty?
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "Empty!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto skip_pickup;
                                        }

                                        tplayer->mobj->type = 184; //jar of honey
                                        tplayer->mobj_last = NULL; //->send message

                                        if (myobj->info & 112) {
                                            //temp object?
                                            x2 = (myobj->info >> 9) & 15;
                                            x2--;
                                            myobj->info &= (0xFFFF - (15 << 9));
                                            myobj->info |= (x2 << 9);
                                        }

                                        goto skip_pickup;
                                    }
                                }

                                //grain on horse
                                if (tplayer->mobj->type == 166) {
                                    //grain
                                    if ((myobj->type & 1023) == 431) {
                                        txtsetchar(t, 8);
                                        txtadd(t, "Not now!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto skip_pickup;
                                    }
                                    if ((myobj->type & 1023) == 430) {
                                        if (myobj->info & 8) myobj = (object *) myobj->more;
                                        if (myobj->info & 4) {
                                            crt = (creature *) myobj->more;
                                            if (crt->flags & 128) {
                                                if (((crt->respawn_x >> 4) & 3) != 3) {
                                                    crt->respawn_x |= (3 << 4);
                                                    stealing(tplayer, tplayer->mobj);
                                                    tplayer->mobj = NULL;
                                                    txtsetchar(t, 8);
                                                    txtaddchar(t, 255);
                                                    txtadd(t, "You pour the grain into the horse's feed bag.");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                } //!=3
                                                txtsetchar(t, 8);
                                                txtaddchar(t, 255);
                                                txtadd(t, "This horse's feed bag is already full!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            } else {
                                                stealing(tplayer, tplayer->mobj);
                                                tplayer->mobj = NULL;
                                                txtsetchar(t, 8);
                                                txtaddchar(t, 255);
                                                txtadd(t, "You feed the horse the sack of grain.");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            }
                                        } //crt
                                    } //horse(without rider)
                                } //grain

                                //horseshoes on horse
                                if (tplayer->mobj->type == 202) {
                                    //horseshoes
                                    if ((myobj->type & 1023) == 431) {
                                        txtsetchar(t, 8);
                                        txtadd(t, "Not now!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto skip_pickup;
                                    }
                                    if ((myobj->type & 1023) == 430) {
                                        if (myobj->info & 8) myobj = (object *) myobj->more;
                                        if (myobj->info & 4) {
                                            crt = (creature *) myobj->more;
                                            if (crt->flags & 128) {
                                                if (((crt->respawn_x >> 6) & 3) != 3) {
                                                    crt->respawn_x |= (3 << 6);
                                                    stealing(tplayer, tplayer->mobj);
                                                    tplayer->mobj = NULL;
                                                    txtsetchar(t, 8);
                                                    txtaddchar(t, 255);
                                                    txtadd(t, "You fit a new horseshoe upon each hoof.");
                                                    NET_send(NETplayer, tplayer->net, t);
                                                    goto skip_pickup;
                                                } //!=3
                                                txtsetchar(t, 8);
                                                txtaddchar(t, 255);
                                                txtadd(t, "This horse is already fitted with new horseshoes!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            } else {
                                                stealing(tplayer, tplayer->mobj);
                                                tplayer->mobj = NULL;
                                                txtsetchar(t, 8);
                                                txtaddchar(t, 255);
                                                txtadd(t, "You fit the horse with new horseshoes.");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            }
                                        } //crt
                                    } //horse(without rider)
                                } //horseshoes


                                //potions
                                if (tplayer->mobj->type == (275 + 1 * 1024)) {
                                    //red potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player) {
                                            tnpc->flags &= (65535 - 1); //remove poisoned flag
                                            tnpc->upflags |= 2;
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                            goto skip_pickup;
                                        }
                                    }
                                }

                                //use fan on balloon while flying
                                if (tplayer->mobj->type == (212)) {
                                    //fan
                                    if (myobj->info & 2 && (myobj->type & 1023) == 423) {
                                        //<-NPC
                                        //check if walking passable without balloon
                                        i4 = myobj->x;
                                        i5 = myobj->y;
                                        OBJremove(myobj);
                                        if (bt[i5][i4] & 1024) {
                                            OBJadd(i4, i5, myobj);
                                            use_craft(tplayer, myobj);
                                        } else {
                                            OBJadd(i4, i5, myobj);
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "Not here!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                        goto skip_pickup;
                                    }
                                }

                                if (tplayer->mobj->type == (275 + 2 * 1024)) {
                                    //yellow potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player) {
                                            tnpc->hp += 30;
                                            if (tnpc->hp > tnpc->hp_max) tnpc->hp = tnpc->hp_max;
                                            tnpc->upflags |= 2;
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                            goto skip_pickup;
                                        }
                                    }
                                }

                                if (tplayer->mobj->type == (275 + 3 * 1024)) {
                                    //green potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player == tplayer) {
                                            tnpc->flags |= 1; //set poisoned flag
                                            tnpc->upflags |= 2;
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                        }
                                        goto skip_pickup;
                                    }
                                }

                                //if (tnpc->invisibility){
                                if (tplayer->mobj->type == (275 + 6 * 1024)) {
                                    //black potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player) {
                                            if (tnpc->invisibility < 32) tnpc->invisibility = 32;
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                            goto skip_pickup;
                                        }
                                    }
                                }

                                if (tplayer->mobj->type == (275 + 5 * 1024)) {
                                    //purple potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player) {
                                            if (tnpc->protection < 32) tnpc->protection = 32; //~2mins
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                            tnpc->upflags |= 64; //update ->arm value
                                            goto skip_pickup;
                                        }
                                    }
                                }

                                if (tplayer->mobj->type == (275 + 7 * 1024)) {
                                    //white potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player) {
                                            if (tnpc->player->xray < 8) {
                                                tnpc->player->xray = 8;
                                                //send xray update message
                                                txtset(t, "??");
                                                t->d2[0] = 33;
                                                t->d2[1] = tnpc->player->xray;
                                                NET_send(NETplayer, tnpc->player->net, t);
                                            }
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                            goto skip_pickup;
                                        }
                                    }
                                }

                                if (tplayer->mobj->type == (275 + 0 * 1024)) {
                                    //blue potion
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player) {
                                            if (tnpc->flags & 4) {
                                                //sleep flag
                                                tnpc->flags -= 4;
                                                tnpc->wait_disable = 0;
                                            } //4
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                            goto skip_pickup;
                                        }
                                    }
                                }

                                if (tplayer->mobj->type == (275 + 4 * 1024)) {
                                    //orange potion (sleep)
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player == tplayer) {
                                            tnpc->flags |= 4; //set sleep flag
                                            f = rnd * 32.0f + 32.0f;
                                            if (tnpc->wait_disable < f) tnpc->wait_disable = f;
                                            stealing(tplayer, tplayer->mobj);
                                            tplayer->mobj = NULL;
                                        }
                                        goto skip_pickup;
                                    }
                                }


                                x2 = 0;
                                x3 = tplayer->mobj->type;
                                x5 = 0;
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
                                if (x3 == 184) x2 = 10; //184 jar of honey *this item needs modifying after use 10 C *10
                                if (x3 == 209) x2 = 12; //209 small hanging meat 6 B *12
                                if (x3 == 210) x2 = 28; //210 big hanging meat 9 6 A =7 *28
                                if (x3 == 265) x2 = 6; //265 fish 3 B *6
                                if (x2) {
                                    x2 *= 4;
                                    if (myobj->info & 2) {
                                        //<-NPC
                                        tnpc = (npc *) myobj->more;
                                        if (tnpc->player == tplayer) {
                                            x4 = tnpc->food;
                                            if ((x4 + x2) > 255) {
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, tnpc->name);
                                                txtadd(t, " full!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            }
                                            x4 += x2;
                                            tnpc->food = x4;
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtaddchar(t, 255);
                                            txtadd(t, tnpc->name);
                                            if (x5) txtadd(t, " drinks the ");
                                            else txtadd(t, " eats the ");
                                            y2 = objname[(tplayer->mobj->type & 1023) * 2];
                                            y3 = objname[(tplayer->mobj->type & 1023) * 2 + 1];
                                            txtNEWLEN(t2, y3);
                                            memcpy(&t2->d2[0], &objname2[y2], y3);
                                            txtadd(t, t2);
                                            txtadd(t, ".");
                                            NET_send(NETplayer, tplayer->net, t);

                                            stealing(tplayer, tplayer->mobj);

                                            //change or destroy item?
                                            if (obji[sprlnk[tplayer->mobj->type & 1023] + (tplayer->mobj->type >> 10)].
                                                flags & 4096) {
                                                if (tplayer->mobj->more2 != 1) {
                                                    //more than 1!
                                                    tplayer->mobj->more2--;
                                                    goto skip_pickup;
                                                }
                                            }

                                            if ((x3 == 179) || (x3 == 180)) {
                                                tplayer->mobj->type = 178;
                                                tplayer->mobj_last = NULL;
                                                goto skip_pickup;
                                            }
                                            if (x3 == 184) {
                                                tplayer->mobj->type = 183;
                                                tplayer->mobj_last = NULL;
                                                goto skip_pickup;
                                            }
                                            OBJremove(tplayer->mobj);
                                            OBJrelease(tplayer->mobj);
                                            tplayer->mobj = NULL; //?
                                            //stupid comment like in ultima7??
                                        }
                                        goto skip_pickup;
                                    }
                                }


                                if ((tplayer->mobj->type >= 242) && (tplayer->mobj->type <= 249)) {
                                    //rune
                                    if (myobj2 = (object *) myobj->prev) {
                                        if ((myobj2->type & 1023) == 73) {
                                            //moonstone
                                            if ((myobj2->type >> 10) == (tplayer->mobj->type - 242)) {
                                                //same type
                                                tplayer->GNPCflags[3] |= (1 << (myobj2->type >> 10));
                                                goto skip_pickup;
                                            } //same type
                                        }
                                    } //moonstone
                                } //rune


                                if (tplayer->mobj->type == 63) {
                                    //lockpick
                                    if (myobj->type == (98 + 2048)) {
                                        //locked chest
                                        myobj3 = (object *) tplayer->party[0];
                                        tnpc = (npc *) myobj3->more;
                                        stealing(tplayer, tplayer->mobj);
                                        if (((rnd * 64) + (rnd * tnpc->i)) >= 56) {
                                            //success
                                            OBJsave(myobj->x, myobj->y);
                                            myobj->type = OBJ_CHEST + 1024;
                                            if (enhancehostn1) {
                                                txtsetchar(t, 8);
                                                txtaddchar(t, 255);
                                                txtadd(t, "Chest unlocked!");
                                                NET_send(NETplayer, tplayer->net, t); // f111
                                            }
                                        } else {
                                            x2 = rnd * 2;
                                            if (x2) {
                                                //break pick!
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, "Key broke!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                if (tplayer->mobj->more2 == 1) tplayer->mobj = NULL;
                                                else tplayer->mobj->more2--;
                                            } //x2
                                        }
                                        goto skip_pickup;
                                    }
                                }


                                //lock pick on chest??

                                //WOW is that a door at all???

                                x2 = myobj->type & 1023;
                                if ((x2 >= 297) && (x2 <= 300)) {
                                    //door
                                    if ((myobj->type >> 10) >= 4) {
                                        //closed
                                        if (tplayer->mobj->type == 63) goto lockpick; //lockpick
                                        if (tplayer->mobj->type == 64) {
                                            //key
