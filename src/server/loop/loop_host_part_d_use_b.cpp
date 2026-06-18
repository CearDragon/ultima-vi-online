// LHS-P6.7: part_d_use_b -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: item-use dispatch part 2 (useinventoryitem, magicfan_used,
// lastchanceuse_loop). Move-only.
                    useinventoryitem:

                        //if musical instrument is in inventory, then it can be used
                        if ((i == 153) || (i == 158) || (i == 296) || (i == 156) || (i == 157)) {
                            //panpipes, lute, xylophone, harpsichord, harp
                            //***assume proximity ok***
                            //set client in music mode with selected instrument
                            txtset(t, "??");
                            t->d2[0] = 24;
                            if (i == 153) t->d2[1] = 3;
                            if (i == 158) t->d2[1] = 0;
                            if (i == 296) t->d2[1] = 4;
                            if (i == 156) t->d2[1] = 1;
                            if (i == 157) t->d2[1] = 2;
                            tplayer->instrument = t->d2[1];
                            tplayer->playinstrument = 2;
                            NET_send(NETplayer, tplayer->net, t);
                            goto finishuse;
                        }

                        /*
            //if musical instrument is in inventory, then it can be used
            if ((i==153)||(i==158)||(i==296)){ //panpipes, lute, xylophone
            //set client in music mode




            goto finishuse;
            }
            */


                        if (i == 169) {
                            //ducky
                            myobj3 = tplayer->party[0];
                            i3 = SFnew(myobj3->x, myobj3->y);
                            sf[i3].type = 4; //sound
                            sf[i3].x = myobj3->x;
                            sf[i3].y = myobj3->y;
                            sf[i3].wait = 1.0f; //NULL
                            sf[i3].more = 5; //ducky
                            goto finishuse;
                        }

                        if (i == OBJ_GEM) {
                            //gems
                            x3 = 10;
                            if (x3 > 255) x3 = 255; //cap
                            if (x3 > tplayer->peer) {
                                tplayer->peer = x3;
                                txtset(t, "??");
                                t->d2[0] = 48;
                                t->d2[1] = tplayer->peer;
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            if (myobj->more2 != 1) {
                                //more than 1!
                                myobj->more2--;
                                goto finishuse;
                            }
                            OBJremove(myobj);
                            OBJrelease(myobj);
                            myobj = NULL;
                            goto finishuse;
                        }

                        if (i == 212) {
                            //magic fan
                            x3 = tplayer->windx;
                            y3 = tplayer->windy;
                            if ((x3 == 0) && (y3 == 0)) {
                                x3 = windx;
                                y3 = windy;
                            }
                            if ((x3 == 0) && (y3 == 0)) {
                                x3 = 0;
                                y3 = 1;
                                goto magicfan_used;
                            }
                            if ((x3 == 0) && (y3 == 1)) {
                                x3 = -1;
                                y3 = 1;
                                goto magicfan_used;
                            }
                            if ((x3 == -1) && (y3 == 1)) {
                                x3 = -1;
                                y3 = 0;
                                goto magicfan_used;
                            }
                            if ((x3 == -1) && (y3 == 0)) {
                                x3 = -1;
                                y3 = -1;
                                goto magicfan_used;
                            }
                            if ((x3 == -1) && (y3 == -1)) {
                                x3 = 0;
                                y3 = -1;
                                goto magicfan_used;
                            }
                            if ((x3 == 0) && (y3 == -1)) {
                                x3 = 1;
                                y3 = -1;
                                goto magicfan_used;
                            }
                            if ((x3 == 1) && (y3 == -1)) {
                                x3 = 1;
                                y3 = 0;
                                goto magicfan_used;
                            }
                            if ((x3 == 1) && (y3 == 0)) {
                                x3 = 1;
                                y3 = 1;
                                goto magicfan_used;
                            }
                            if ((x3 == 1) && (y3 == 1)) {
                                x3 = 0;
                                y3 = 1;
                                goto magicfan_used;
                            }
                        magicfan_used:
                            tplayer->windx = x3;
                            tplayer->windy = y3;
                            txtset(t, "??");
                            t->d2[0] = 14;
                            t->d2[1] = (tplayer->windx + 1) + (tplayer->windy + 1) * 4;
                            NET_send(NETplayer, tplayer->net, t);
                            goto finishuse;
                        }

                        if (i == 93) {
                            //sextant
                            //*sextants 304,360
                            myobj = tplayer->party[tplayer->selected_partymember];
                            x2 = 0;
                            if (myobj->y >= 256) x2 = 2;
                            if (myobj->x < 1024) x2 = 1;
                            if (x2) {
                                if (x2 == 1) {
                                    x = (myobj->x + 1024 - 304) / 8;
                                    x -= 128;
                                    y = (myobj->y + 1024 - 360) / 8;
                                    y -= 128;
                                } else {
                                    x = ((myobj->x - 1024) + 256 - 76) / 2;
                                    x -= 128;
                                    y = ((myobj->y - 256) + 256 - 90) / 2;
                                    y -= 128;
                                }
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtnumint(t2, abs(y));
                                txtadd(t, t2);
                                if (y > 0) txtadd(t, "'S, ");
                                else txtadd(t, "'N, ");
                                txtnumint(t2, abs(x));
                                txtadd(t, t2);
                                if (x > 0) txtadd(t, "'E");
                                else txtadd(t, "'W");
                                NET_send(NETplayer, tplayer->net, t);
                            } else {
                                txtset(t, "?Not usable here!");
                                t->d2[0] = 8;
                                NET_send(NETplayer, tplayer->net, t);
                            }
                            goto finishuse;
                        }


                        if (i == 270) {
                            //use balloon plans
                            //scan for required items
                            for (x6 = 0; x6 <= 7; x6++) {
                                if (tplayer->party[x6]) {
                                    tnpc3 = (npc *) tplayer->party[x6]->more;
                                    x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                                }
                            } //!=NULL,x6
                            OBJlist_last = NULL;
                            y7 = 0;
                            for (y6 = 0; y6 < x5; y6++) {
                                if ((OBJlist_list[y6]->type & 1023) == 421) y7 |= 1; //silk bag
                                if ((OBJlist_list[y6]->type & 1023) == 422) y7 |= 2; //basket
                                if ((OBJlist_list[y6]->type & 1023) == 284) y7 |= 4; //rope
                                if ((OBJlist_list[y6]->type & 1023) == 283) y7 |= 8; //anchor
                                if ((OBJlist_list[y6]->type & 1023) == 147) y7 |= 16; //cauldron
                            }
                            if (y7 != 31) {
                                txtset(
                                    t,
                                    "?Requires a balloon basket, a mammoth silk bag, rope, an anchor and a cauldron.");
                                t->d2[0] = 8;
                                NET_send(NETplayer, tplayer->net, t);
                                goto finishuse;
                            }
                            //remove required items
                            for (y6 = 0; y6 < x5; y6++) {
                                if (((OBJlist_list[y6]->type & 1023) == 421) && (y7 & 1)) {
                                    y7 -= 1;
                                    OBJremove(OBJlist_list[y6]);
                                }
                                if (((OBJlist_list[y6]->type & 1023) == 422) && (y7 & 2)) {
                                    y7 -= 2;
                                    OBJremove(OBJlist_list[y6]);
                                }
                                if (((OBJlist_list[y6]->type & 1023) == 284) && (y7 & 4)) {
                                    y7 -= 4;
                                    OBJremove(OBJlist_list[y6]);
                                }
                                if (((OBJlist_list[y6]->type & 1023) == 283) && (y7 & 8)) {
                                    y7 -= 8;
                                    OBJremove(OBJlist_list[y6]);
                                }
                                if (((OBJlist_list[y6]->type & 1023) == 147) && (y7 & 16)) {
                                    y7 -= 16;
                                    OBJremove(OBJlist_list[y6]);
                                }
                            }
                            //add balloon to inventory (make sure it's a quest item)
                            myobj3 = OBJnew();
                            myobj3->type = 420;
                            myobj3->info |= 256; //quest item
                            tnpc3 = (npc *) tplayer->party[0]->more;
                            additemroot(tnpc3, myobj3);
                            goto finishuse;
                        }

                        if (i == 420) {
                            //balloon
                            if (tplayer->craft == NULL) {
                                if (usinghorse(tplayer) == NULL) {
                                    if ((tplayer->party[0]->x < 1024) || (
                                            (tplayer->party[0]->y >= 256) && (tplayer->party[0]->y <= 512) && (
                                                tplayer->party[0]->x <= 1280))) {
                                        if (tplayer->selected_partymember) {
                                            //change selected_partymember to 0
                                            if (tplayer->party[tplayer->selected_partymember]) {
                                                tnpc3 = (npc *) tplayer->party[tplayer->selected_partymember]->more;
                                                if (tnpc3->order == 0) tnpc3->order = 1;
                                                tnpc3->follow = tplayer->party[0];
                                            }
                                            tnpc3 = (npc *) tplayer->party[0]->more;
                                            if (tnpc3->order == 1) tnpc3->order = 0;
                                            tnpc3->follow = tplayer->party[0];
                                            tplayer->selected_partymember = 0;
                                        } //tplayer->selected_partymember


                                        //remove item from inventory
                                        myobj2 = myobj;
                                        OBJremove(myobj2);

                                        //create a balloon where the avatar is standing
                                        myobj = tplayer->party[0];
                                        x = myobj->x;
                                        y = myobj->y;

                                        myobj2->type = 423 + 1024 * 3;
                                        OBJadd(x, y, myobj2);
                                        static mlobj *mmyobj; //array size varies
                                        mmyobj = (mlobj *) malloc(sizeof(object *) * 3);
                                        myobj2->more = mmyobj;
                                        myobj = OBJnew();
                                        mmyobj->obj[0] = myobj; //middle
                                        myobj->type = 423 + 0 * 1024;
                                        myobj->info |= 8; //<-base object
                                        OBJadd(x - 1, y - 1, myobj);
                                        myobj->more = myobj2;
                                        myobj = OBJnew();
                                        mmyobj->obj[1] = myobj; //middle
                                        myobj->type = 423 + 1 * 1024;
                                        myobj->info |= 8; //<-base object
                                        OBJadd(x, y - 1, myobj);
                                        myobj->more = myobj2;
                                        myobj = OBJnew();
                                        mmyobj->obj[2] = myobj; //middle
                                        myobj->type = 423 + 2 * 1024;
                                        myobj->info |= 8; //<-base object
                                        OBJadd(x - 1, y, myobj);
                                        myobj->more = myobj2;

                                        //use balloon (the one we just created :)
                                        tplayer->craft_con = (object *) myobj2->more;
                                        myobj2->more = tplayer->party[0]->more;
                                        myobj2->info = tplayer->party[0]->info;
                                        tplayer->craft = tplayer->party[0];
                                        for (i4 = 7; i4 >= 0; i4--) {
                                            if (tplayer->party[i4] != NULL) {
                                                OBJremove(tplayer->party[i4]);
                                                VLNKremove(tplayer->party[i4]);
                                                VLNKsremove(tplayer->party[i4]);
                                                tnpc3 = (npc *) tplayer->party[i4]->more;
                                                tnpc3->target = NULL;
                                            }
                                        }
                                        tplayer->party[0] = myobj2;
                                        if (myobj2->info & 32768) myobj2->info -= 32768;
                                    } //x,y
                                } //usinghorse
                            } //->craft==NULL
                            goto finishuse;
                        }

                        if (itemused == 0) {
                            myobj = NULL;
                            goto retryuse;
                        }

                        if (itemused == 1) {
                            //No use-able object has been located on top, scan for items which may be used whilst not the top item
                            myobj = od[y][x];
                        lastchanceuse_loop:
                            i = myobj->type & 1023;
                            myobj2 = tplayer->party[0];
                            if (abs(myobj2->x - x) > 1) goto lastchanceuse_playeronlyuse_skip;
                            if (abs(myobj2->y - y) > 1) goto lastchanceuse_playeronlyuse_skip;

                            if (use_noladdercheck == 0) {
                                if ((i == 305) || (i == 326) || (i == 308)) goto lastchanceuse_ladder;
                                //ladder or dungeon or hole
                            }

                        lastchanceuse_playeronlyuse_skip:
                            if (myobj->next) {
                                myobj = (object *) myobj->next;
                                goto lastchanceuse_loop;
                            }
                        } //itemused==1

                    finishuse:;
                        goto skip_pickup;
                    } //action==1


                    if (tplayer->action == 2) {
                        //look ->primary surface
                        x = tpx + tplayer->mx / 32;
                        y = tpy + tplayer->my / 32;
                        myobj = OBJfindlast(x, y);
                        if (myobj == NULL) {
                            myobj = OBJfindlastall(x, y);
                            if (myobj == NULL) goto looknullobject;
                        }
                        if (myobj->info & 8) myobj = (object *) myobj->more; //redirect!


                        if (myobj->type == 51) {
                            //force field
                            myobj2 = (object *) myobj->prev;
                            if (tplayer->GNPCflags[3] & (1 << (myobj2->type >> 10))) myobj = (object *) myobj->prev;
                            if (tplayer->GNPCflags[4] & (1 << (myobj2->type >> 10))) myobj = (object *) myobj->prev;
                        } //force field

                        if ((myobj->type >= 242) && (myobj->type <= 249)) {
                            //skip rune LOOK
                            if (tplayer->GNPCflags[5] & (1 << (myobj->type - 242))) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == (275 + 2 * 1024)) {
                            //yellow potion
                            if (myobj->info & 256) {
                                //quest
                                if (tplayer->GNPCflags[32] & myobj->more2) myobj = (object *) myobj->prev;
                                if (myobj == NULL) goto looknullobject;
                            } //quest
                        } //yellow potion

                        if ((myobj->type >= 400) && (myobj->type <= 407)) {
                            //skip map(0-7) LOOK
                            if (tplayer->GNPCflags[7] & (1 << (myobj->type - 400))) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 62) {
                            //vortex cube SKIPLOOK
                            if ((myobj->x == 1939) && (myobj->y == 57)) {
                                if (tplayer->GNPCflags[6] & (1 << 1)) myobj = (object *) myobj->prev;
                                if (myobj == NULL) goto looknullobject;
                            }
                            if ((myobj->x == 923) && (myobj->y == 850)) {
                                if ((tplayer->GNPCflags[18] & 1) == 0) myobj = (object *) myobj->prev;
                                if (myobj == NULL) goto looknullobject;
                            }
                        }

                        if (myobj->type == 389) {
                            //silver tablet
                            if (tplayer->GNPCflags[6] & (1 << 2)) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }
                        if (myobj->type == 390) {
                            //silver tabletII
                            if (tplayer->GNPCflags[6] & (1 << 3)) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 270) {
                            //balloon plans
                            if (tplayer->GNPCflags[6] & (1 << 4)) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 395) {
                            //broken gargoyle lens
                            if (tplayer->GNPCflags[6] & (1 << 5)) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj == nuggetsfix) {
                            //nuggets
                            if ((tplayer->GNPCflags[6] & (1 << 6)) || (tplayer->online < 1536))
                                myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 59) {
                            //codex
                            if (tplayer->GNPCflags[18] & 8) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 394) {
                            //brit lens SKIPLOOK
                            if ((tplayer->GNPCflags[18] & 2) == 0) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }
                        if (myobj->type == 396) {
                            //garg lens SKIPLOOK
                            if ((tplayer->GNPCflags[18] & 4) == 0) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 416) {
                            //nothing!
                            myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->type == 173) {
                            //trap
                            if ((myobj->info & (1 << 9)) == 0) myobj = (object *) myobj->prev;
                            if (myobj == NULL) goto looknullobject;
                        }

                        if (myobj->info & 4) {
                            //invisible crt
                            crt = (creature *) myobj->more;
                            if (crt->flags & 2) {
                                myobj = (object *) myobj->prev;
                                if (myobj == NULL) goto looknullobject;
                            } //2
                        } //4

                        for (x2 = 0; x2 <= nresu; x2++) {
                            //dead NPC? (only player who owns the dead npc can extract)
                            if (resu[x2]) {
                                if (resu_body[x2] == myobj) {
                                    if (tplayer == resu_player[x2]) {
                                        //check if item is next to player
                                        z2 = 0;
                                        for (z = 0; z <= 7; z++) {
                                            if (myobj2 = tplayer->party[z]) {
                                                if (abs(myobj2->x - myobj->x) <= 1) {
                                                    if (abs(myobj2->y - myobj->y) <= 1) {
                                                        z2 = 1;
                                                    }
                                                }
                                            }
                                        }
                                        if (z2 == 1) {
                                            x3 = 0;
                                            tnpc = (npc *) resu[x2]->more;
                                        leaveitems3:
                                            if (myobj3 = (object *) tnpc->items->more) {
                                                x3 = 1;
                                                OBJremove(myobj3);
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                                goto leaveitems3;
                                            }
                                            if (myobj3 = tnpc->helm) {
                                                x3 = 1;
                                                tnpc->helm = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->neck) {
                                                x3 = 1;
                                                tnpc->neck = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->wep_left) {
                                                x3 = 1;
                                                if (myobj3->type == (90 + 1024)) myobj3->type = 90;
                                                tnpc->wep_left = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->wep_right) {
                                                x3 = 1;
                                                if (myobj3->type == (90 + 1024)) myobj3->type = 90;
                                                tnpc->wep_right = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->armour) {
                                                x3 = 1;
                                                tnpc->armour = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->ring_left) {
                                                x3 = 1;
                                                tnpc->ring_left = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->ring_right) {
                                                x3 = 1;
                                                tnpc->ring_right = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (myobj3 = tnpc->boots) {
                                                x3 = 1;
                                                tnpc->boots = NULL;
                                                OBJadd(myobj->x, myobj->y, myobj3);
                                            }
                                            if (x3) {
                                                tnpc->baseitem = tnpc->items;
                                                tnpc->baseitem_offset = 0;
                                                txtset(t, "?You search ");
                                                txtadd(t, tnpc->name);
                                                txtadd(t, "'s dead body.");
                                                t->d2[0] = 8;
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto skip_pickup;
                                            } //x3
                                        } //z2
                                    }
                                }
                            }
                        }


                        //if it is a container, retrieve items inside (if empty give stats)
                        if (myobj->more) {
                            x2 = myobj->type & 1023;
                            x3 = myobj->type >> 10;
                            x4 = 0;
                            if ((obji[sprlnk[x2]].flags & 1024) && (x3 == 0)) x4 = 1;
                            if (x2 == 168) x4 = 1; //remains
                            if (x2 == 339) x4 = 1; //dead body
                            if (x2 == 176) x4 = 1; //drawers
                            if (x2 == 177) x4 = 1; //desk
                            if (x2 == 335) x4 = 1;
                            if (x2 == 134) x4 = 1; //carcass
                            if (x2 == 340) x4 = 1; //garg
                            if (x2 == 341) x4 = 1; //cyclops
                            if ((x2 >= 432) && (x2 <= 447)) x4 = 1;

                            //other special cases?
                            if (x4 == 1) {
                                //extract items out of container
                                //check if item is next to player
                                z2 = 0;
                                for (z = 0; z <= 7; z++) {
                                    myobj2 = tplayer->party[z];
                                    if (myobj2 != NULL) {
                                        if (abs(myobj2->x - myobj->x) <= 1) {
                                            if (abs(myobj2->y - myobj->y) <= 1) {
                                                z2 = 1;
                                            }
                                        }
                                    }
                                }
                                if (z2 == 1) {
                                    if (myobj->more) OBJsave(myobj->x, myobj->y);
                                extractobj:
                                    myobj2 = (object *) myobj->more;
                                    if (myobj2) {
                                        OBJremove(myobj2);
                                        myobj2->info |= 112;
                                        OBJadd(myobj->x, myobj->y, myobj2);
                                        goto extractobj;
                                    }
                                    goto skip_pickup;
                                } //z2
                            } //x4==1
                        } //->more


                        //f9=0;
