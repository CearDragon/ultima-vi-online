// LHS-P6.9: part_d_ship -- CONTINUES + CLOSES MEGA D. WIRE.
// LHS brace-seam: CONTINUES + CLOSES the for(tpl..){ and if(playerlist..){
// blocks opened in loop_host_part_d_open.cpp (trailing braces at MEGA D end).
// Contents: ship attack (shipattack, shipattack2). Move-only; no U6O_VERSION.
                        shipattack:
                            x = tpx + tplayer->mx / 32;
                            y = tpy + tplayer->my / 32;
                            myobj = OBJfindlast(x, y);
                            if (myobj != NULL) {
                                //is it a member of our party? if so break off combat
                                //for (x2=0;x2<=7;x2++){
                                //if (tplayer->party[x2]==myobj){

                                if (myobj->info & 2) {
                                    //if NPC break off combat
                                    for (x2 = 0; x2 <= 7; x2++) {
                                        if (tplayer->party[x2]) {
                                            VLNKsremove(tplayer->party[x2]);
                                            tnpc2 = (npc *) tplayer->party[x2]->more;
                                            tnpc2->target = NULL;
                                            tnpc2->order = 3;
                                            if (x2 == tplayer->selected_partymember) tnpc2->order = 0;
                                        } //->party
                                    } //x2
                                    goto skip_pickup;
                                }

                                //}
                                //}

                                if (myobj->info & 8) myobj = (object *) myobj->more; //redirect
                                if (myobj->info & 4) {
                                    //<-crt

                                    //myobj2=tplayer->party[0];
                                    //tnpc=(npc*)myobj2->more;
                                    //tnpc->order=3;
                                    //tnpc->target=myobj;
                                    //set other party members to attack as well
                                    for (x2 = 0; x2 <= 7; x2++) {
                                        if (tplayer->party[x2]) {
                                            myobj2 = tplayer->party[x2];
                                            tnpc = (npc *) myobj2->more;
                                            tnpc->order = 3;
                                            tnpc->target = myobj;
                                            VLNKsremove(myobj2); //remove prev vlnk
                                            VLNKnew(myobj2, myobj, (unsigned long) &tnpc->target); //add vlnk
                                            if (tplayer->craft) goto shipattack2; //exit loop after processing party[0]
                                        }
                                    }
                                } //<-crt
                            shipattack2:

                                //attack item (locked chest, door, spider web, other breakable items) (lets take spider web as an example)

                                if ((myobj->type & 1023) == OBJ_CHEST) goto targetitem;
                                if (myobj->type == OBJ_WEB) {
                                    //web
                                targetitem:
                                    myobj2 = tplayer->party[tplayer->selected_partymember];
                                    tnpc = (npc *) myobj2->more;
                                    tnpc->order = 3;
                                    tnpc->target = myobj;
                                    VLNKsremove(myobj2); //remove prev vlnk
                                    VLNKnew(myobj2, myobj, (unsigned long) &tnpc->target); //add vlnk
                                }
                            } //!=NULL

                            //set creature as a target for attack oh no! it's gonna die

                            //unsigned char order;
                            //0=none
                            //1=follow party leader
                            //2=schedule
                            //3=attack target
                        } //craft

                        goto skip_pickup;
                    } //action==3 attack
                } //primary surface click

                if (tplayer->mf <= 7) {
                    if (tplayer->party[tplayer->mf]) {
                        //2003 Xtnpc=(npc*)activeplayer->party[tplayer->mf]->more;
                        tnpc = (npc *) tplayer->party[tplayer->mf]->more;


                        if (tplayer->key & KEYmbclick) {
                            //tnpc->upflags|=(32+64); //inv+eqp

                            //tnpc->update=1;

                            //clicked on opened "bag" icon ->close bag
                            if ((tplayer->mx >= (90)) && (tplayer->my >= (128 + 3)) && (tplayer->mx < ((90) + 32)) && (
                                    tplayer->my < ((128 + 3) + 32))) {
                                if (tnpc->baseitem != tnpc->items) {
                                    //backwards traverse
                                    myobj2 = tnpc->baseitem;
                                    myobj = (object *) tnpc->baseitem->prev;
                                getprev10:

                                    //TROLL GPF NEXT LINE (BACKSCANNING THROUGH INVENTORY?)
                                    if ((myobj->more != myobj2) || (
                                            (obji[sprlnk[myobj->type & 1023]].flags & 1024) == 0)) {
                                        myobj2 = myobj;
                                        myobj = (object *) myobj->prev;
                                        goto getprev10;
                                    }

                                    tnpc->baseitem = myobj;
                                    tnpc->baseitem_offset = 0;
                                    tnpc->upflags |= 32;
                                    goto skip_pickup;
                                }
                            } //x,y

                            if (tplayer->action == 0) {
                                //scroll inventory down
                                if ((tplayer->mx >= (111 - 3)) && (tplayer->my >= (228 - 3))) {
                                    if ((tplayer->mx <= (121 + 3)) && (tplayer->my <= (243 + 3))) {
                                        tnpc->upflags |= 32;
                                        //is scroll down available?
                                        myobj = (object *) tnpc->baseitem;
                                        myobj = (object *) myobj->more;
                                        x = 61;
                                        y = 61;
                                        x3 = tnpc->baseitem_offset;
                                    dniz:
                                        if (myobj != NULL) {
                                            if (x3 > 0) {
                                                x3--;
                                                goto skipbio1z;
                                            }
                                            x = x + 16;
                                            if (x > 109) {
                                                x = 61;
                                                y = y + 16;
                                            }
                                            if (y > 109) {
                                                if (myobj->next != NULL) {
                                                    tnpc->baseitem_offset += 4;
                                                }
                                                goto diskipz;
                                            }
                                            if (myobj->next != NULL) {
                                            skipbio1z:
                                                myobj = (object *) myobj->next;
                                                goto dniz;
                                            }
                                        }
                                    diskipz:;
                                        goto skip_pickup;
                                    }
                                }

                                //scroll inventory up
                                if ((tplayer->mx >= (111 - 3)) && (tplayer->my >= (197 - 3))) {
                                    if ((tplayer->mx <= (121 + 3)) && (tplayer->my <= (212 + 3))) {
                                        if (tnpc->baseitem_offset >= 4) { tnpc->baseitem_offset -= 4; } else {
                                            tnpc->baseitem_offset = 0;
                                        }
                                        tnpc->upflags |= 32;
                                        goto skip_pickup;
                                    }
                                }


                                if ((tplayer->mx >= 122) && (tplayer->my >= 122)) {
                                    x = (tplayer->mx - 122) / 32;
                                    y = (tplayer->my - 122) / 32;
                                    i2 = x + (y * 4);
                                    myobj = tnpc->baseitem;
                                    myobj = (object *) myobj->more;
                                    i3 = tnpc->baseitem_offset;
                                fo2:
                                    if (i3 > 0) {
                                        i3--;
                                        myobj = (object *) myobj->next;
                                        goto fo2;
                                    }
                                    if (myobj != NULL) {
                                        if (i2 > 0) {
                                            if (myobj->next != NULL) {
                                                myobj = (object *) myobj->next;
                                                i2--;
                                                goto fo2;
                                            } else { myobj = NULL; }
                                        }

                                        if (myobj) {
                                            if (tplayer->key & KEYquan) {
                                                if (tplayer->quan) {
                                                    if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].flags &
                                                        4096) {
                                                        if (tplayer->quan < myobj->more2) {
                                                            myobj2 = OBJnew();
                                                            myobj2->type = myobj->type;
                                                            myobj2->info = myobj->info;
                                                            myobj2->more2 = tplayer->quan;
                                                            myobj->more2 -= tplayer->quan;
                                                            tplayer->mobj = myobj2;
                                                            goto nopickup2;
                                                        } //<
                                                    } //multiple
                                                } //->quan
                                            } //KEYquan
                                        }

                                        tplayer->mobj = myobj;
                                        if (tplayer->mobj != NULL) OBJremove(tplayer->mobj);

                                    nopickup2:
                                        tnpc->upflags |= 32;
                                    }
                                } //inventory block
                                else {
                                    //click outside inventory

                                    //2003: new position pickup (no mouse object)
                                    x2 = tplayer->mx;
                                    y2 = tplayer->my;
                                    x3 = helmx;
                                    y3 = helmy;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->helm;
                                        tnpc->helm = NULL;
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = wep_rightx;
                                    y3 = wep_righty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->wep_right;
                                        tnpc->wep_right = NULL;
                                        if (tplayer->mobj) {
                                            //if ((tplayer->mobj->type&1023)==258) {tnpc->flags&=(255-2);} //invisibility ring
                                            if ((tplayer->mobj->type & 1023) == 90) { tplayer->mobj->type = 90; }
                                            if (tplayer->mobj->type == 57) {
                                                //unready spellbook
                                                tnpc->upflags |= 128;
                                            }
                                        }
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = wep_leftx;
                                    y3 = wep_lefty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->wep_left;
                                        tnpc->wep_left = NULL;
                                        if (tplayer->mobj) {
                                            //if ((tplayer->mobj->type&1023)==258) {tnpc->flags&=(255-2);} //invisibility ring
                                            if ((tplayer->mobj->type & 1023) == 90) { tplayer->mobj->type = 90; }
                                            if (tplayer->mobj->type == 57) {
                                                //unready spellbook
                                                tnpc->upflags |= 128;
                                            }
                                        }
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = armourx;
                                    y3 = armoury;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->armour;
                                        tnpc->armour = NULL;
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = bootsx;
                                    y3 = bootsy;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->boots;
                                        tnpc->boots = NULL;
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = neckx;
                                    y3 = necky;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        if (tnpc->neck) {
                                            if (tnpc->neck->type == 76) {
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, "Magical energy prevents you from removing the amulet!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto mobj_gotitem;
                                            }
                                        }
                                        tplayer->mobj = tnpc->neck;
                                        tnpc->neck = NULL;
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = ring_leftx;
                                    y3 = ring_lefty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->ring_left;
                                        tnpc->ring_left = NULL;
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }
                                    x3 = ring_rightx;
                                    y3 = ring_righty;
                                    if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                        tplayer->mobj = tnpc->ring_right;
                                        tnpc->ring_right = NULL;
                                        tnpc->upflags |= 64;
                                        goto mobj_gotitem;
                                    }

                                mobj_gotitem:;
                                } //else outside inventory
                            } //action==0


                            if (tplayer->action == 2) {
                                //look, opens inventory container items
                                tnpc->upflags |= 32;
                                if ((tplayer->mx >= 122) && (tplayer->my >= 122)) {
                                    x = (tplayer->mx - 122) / 32;
                                    y = (tplayer->my - 122) / 32;
                                    i2 = x + (y * 4);
                                    myobj = tnpc->baseitem;
                                    myobj = (object *) myobj->more;
                                    i3 = tnpc->baseitem_offset;
                                fo2b:
                                    if (i3 > 0) {
                                        i3--;
                                        myobj = (object *) myobj->next;
                                        goto fo2b;
                                    }
                                    if (myobj != NULL) {
                                        if (i2 > 0) {
                                            if (myobj->next != NULL) {
                                                myobj = (object *) myobj->next;
                                                i2--;
                                                goto fo2b;
                                            } else { myobj = NULL; }
                                        }
                                    }
                                    if (myobj) {
                                        if ((myobj->type == (98 + 1024 * 2)) || (myobj->type == (98 + 1024 * 3)))
                                            goto
                                                    inventory_look; //magically/locked chests
                                        for (i3 = 0; i3 <= nresu; i3++) {
                                            if (resu[i3]) {
                                                if (resu_body[i3] == myobj) {
                                                    goto inventory_look; //block looking inside resurrectable bodies
                                                }
                                            }
                                        }
                                        if (obji[sprlnk[myobj->type & 1023]].flags & 1024) {
                                            //2003: check container flag
                                            tnpc->baseitem = myobj;
                                            tnpc->baseitem_offset = 0;
                                            goto inventory_look;
                                            //goto skip_pickup;
                                        }
                                        goto inventory_look;
                                    } //myobj
                                    goto skip_pickup;
                                } //if x&y

                                //what about equipped items?
                                x2 = tplayer->mx;
                                y2 = tplayer->my;
                                x3 = helmx;
                                y3 = helmy;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->helm) goto inventory_look;
                                }
                                x3 = wep_rightx;
                                y3 = wep_righty;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->wep_right) goto inventory_look;
                                }
                                x3 = wep_leftx;
                                y3 = wep_lefty;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->wep_left) goto inventory_look;
                                }
                                x3 = armourx;
                                y3 = armoury;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->armour) goto inventory_look;
                                }
                                x3 = bootsx;
                                y3 = bootsy;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->boots) goto inventory_look;
                                }
                                x3 = neckx;
                                y3 = necky;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->neck) goto inventory_look;
                                }
                                x3 = ring_leftx;
                                y3 = ring_lefty;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->ring_left) goto inventory_look;
                                }
                                x3 = ring_rightx;
                                y3 = ring_righty;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->ring_right) goto inventory_look;
                                }

                                goto skip_pickup;
                            } //action==2


                            if (tplayer->action == 1) {
                                //use inventory item
                                tnpc->upflags |= 32;
                                if ((tplayer->mx >= 122) && (tplayer->my >= 122)) {
                                    x = (tplayer->mx - 122) / 32;
                                    y = (tplayer->my - 122) / 32;
                                    i2 = x + (y * 4);
                                    myobj = tnpc->baseitem;
                                    myobj = (object *) myobj->more;
                                    i3 = tnpc->baseitem_offset;
                                fo2d:
                                    if (i3 > 0) {
                                        i3--;
                                        myobj = (object *) myobj->next;
                                        goto fo2d;
                                    }
                                    if (myobj != NULL) {
                                        if (i2 > 0) {
                                            if (myobj->next != NULL) {
                                                myobj = (object *) myobj->next;
                                                i2--;
                                                goto fo2d;
                                            } else { myobj = NULL; }
                                        }
                                    }
                                    if (myobj != NULL) {
                                        itemused = 2;
                                        i = myobj->type & 1023;
                                        i2 = myobj->type >> 10;
                                        goto useinventoryitem;
                                    }
                                } //if x&y

                                //use staff //staff use
                                x2 = tplayer->mx;
                                y2 = tplayer->my;
                                x3 = wep_rightx;
                                y3 = wep_righty;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->wep_right) {
                                        if (myobj->type == 78) goto staff_use0;
                                    }
                                }
                                x3 = wep_leftx;
                                y3 = wep_lefty;
                                if ((x2 >= x3) && (y2 >= y3) && (x2 < (x3 + 32)) && (y2 < (y3 + 32))) {
                                    if (myobj = tnpc->wep_left) {
                                        if (myobj->type == 78) goto staff_use0;
                                    }
                                }
                                myobj = NULL;
                            staff_use0:
                                if (myobj) {
                                    if (myobj->more2 >> 16) {
                                        txtset(t, "?");
                                        t->d2[0] = 23; //staff cast
                                        txtset(t2, "??");
                                        t2->ds[0] = myobj->more2 & 65535;
                                        txtadd(t, t2); //spell type
                                        NET_send(NETplayer, tplayer->net, t);
                                        tplayer->staffcast_npc = tplayer->party[tplayer->mf];
                                        //object* of npc using the staff
                                        tplayer->staffcast_staff = myobj; //object* of staff
                                    } //num. spells !=0
                                } //myobj

                                goto skip_pickup;
                            } //use inventory item
                        } //mbclick
                    } //valid mf value
                } //mf<=7
            skip_pickup:;
            } //check if player is alive and selected party member is valid
        } //current player
    } //player INPUT update (HOST controlled)
