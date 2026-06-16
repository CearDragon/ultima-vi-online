// LHS-P6.6: part_d_use_a -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: item-use dispatch part 1 (retryuse, use_getprevitem,
// lastchanceuse_ladder, usehorse_*). Move-only.
                    retryuse:
                        if (myobj == NULL) {
                            myobj = OBJfindlastall(x, y);
                            itemused = 1;
                        } //check floating objects
                        if (myobj == NULL) {
                            //basetile exceptions
                            /* disabled until katish's dungeon is implemented
              //secret maze entry/exit
              if ((x==193)&&(y==515)){
                if (tplayer->craft==NULL){//not in a balloon (or a boat!)
                  if (usinghorse(tplayer)==NULL){//noone is riding a horse
                    partyadd(tplayer,1876,438);
                  }}}
              if ((x==1876)&&(y==437)){
                if (tplayer->craft==NULL){//not in a balloon (or a boat!)
                  if (usinghorse(tplayer)==NULL){//noone is riding a horse
                    partyadd(tplayer,192,514);
                  }}}*/
                            goto finishuse;
                        }


                        //next items are not people, scan backwards through living creatures to top item!
                    use_getprevitem:
                        if ((myobj->type & 1023) != 430) {
                            //horses excepted(because you can use a horse)
                            if ((myobj->type & 1023) != 431) {
                                //horses excepted(because you can use a horse)
                                if (obji[sprlnk[myobj->type & 1023]].v4) {
                                    myobj = (object *) myobj->prev;
                                    if (myobj) goto use_getprevitem;
                                    goto finishuse;
                                }
                            }
                        }

                        static object *use_beforeredirect;

                        i = myobj->type & 1023;
                        i2 = myobj->type >> 10;
                        use_beforeredirect = myobj;
                        if ((myobj->info & 8) == 8) myobj = (object *) myobj->more; //redirector


                        //***primary player only objects
                        //crafts, ladders, etc


                        if (tplayer->craft) {
                            //if in a craft player can only exit that craft
                            if (myobj != tplayer->party[0]) goto finishuse;
                        }


                        //horse exception: you cannot enter a craft if any partymember is on a horse
                        static unsigned char use_noladdercheck;
                        use_noladdercheck = 0;
                        for (x3 = 0; x3 <= 7; x3++) {
                            if (myobj3 = tplayer->party[x3]) {
                                tnpc3 = (npc *) myobj3->more;
                                if (tnpc3->horse) {
                                    use_noladdercheck = 1;
                                    goto use_horseexceptionskip;
                                }
                            } //myobj3
                        } //x3


                        myobj2 = tplayer->party[0];
                        if (abs(myobj2->x - x) > 1) goto playeronlyuse_skip;
                        if (abs(myobj2->y - y) > 1) goto playeronlyuse_skip;


                        //skiff or raft
                        if (i == 414 || i == 415) {
                            use_craft(tplayer, myobj); /* luteijn: replaces whole shebang */
                            goto finishuse;
                        }

                        //exit balloon
                        if (i == 423) {
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
                            goto finishuse;
                        }


                        if ((i == 305) || (i == 326) || (i == 308)) {
                            //ladder or dungeon or hole
                        lastchanceuse_ladder:
                            if (tplayer->craft == NULL) {
                                //choose a point at the ladder and relocate the whole party to that square
                                x2 = myobj->more2 & 0xFFFF;
                                y2 = myobj->more2 >> 16;

                                //for (i4=7;i4>=0;i4--){
                                //if (tplayer->party[i4]!=NULL){
                                //OBJremove(tplayer->party[i4]);
                                //OBJadd(x2,y2,tplayer->party[i4]);
                                //}
                                //}
                                partyadd(tplayer, x2, y2);
                            } //->craft
                            goto finishuse;
                        }


                    use_horseexceptionskip:

                        if ((myobj->type & 1023) == 84) {
                            //red gate, use gate, use moongate


                            if (myobj->more2 == 0xFFFFFFFF) {
                                //to toth's fantastic house ;)
                                x2 = 1342;
                                y2 = 339;
                                goto tothsgate;
                            }
                            if (myobj->more2 == 0xFFFFFFFE) {
                                //from toth's fantastic house ;)
                                x2 = 1362;
                                y2 = 322;
                                goto tothsgate;
                            }


                            i4 = (myobj->info & 112) >> 4;
                            if (i4 == 1) {
                                x3 = 1;
                                y3 = 2;
                                if (myobj->more2) {
                                    //created by orb?


                                    x3 = myobj->more2 - 1;
                                    y3 = x3 / 5;
                                    x3 = x3 - y3 * 5;
                                }
                                x2 = orbx[x3][y3];
                                y2 = orby[x3][y3];
                                //NEWCODE make one square left from avatar moongate go to player's house.
                                if (x3 == 1 && y3 == 2) {
                                    if (i5 = playerlist[tpl]->GNPCflags[28]) {
                                        x2 = houseentrancex[i5];
                                        y2 = houseentrancey[i5];
                                    }
                                }
                                if (U6O_DEBUG) {
                                    goto tothsgate; //for debugging
                                }
                                //NEWCODEEND
                                x3 = 1 << (x3 + y3 * 5);
                                if (tplayer->GNPCflags[22] & x3) {
                                    //been to dest?
                                tothsgate:

                                    //for (i4=7;i4>=0;i4--){
                                    //if (tplayer->party[i4]!=NULL){
                                    //OBJremove(tplayer->party[i4]);
                                    //OBJadd(x2,y2,tplayer->party[i4]);
                                    //}//!=NULL
                                    //}//i4
                                    partyadd(tplayer, x2, y2);
                                } else {
                                    //&x3
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(
                                        t, "You cannot enter a red moongate with a destination you have never visited!");
                                    NET_send(NETplayer, tplayer->net, t);
                                } //&x3
                            } //i4
                        } //red gate

                        if ((myobj->type & 1023) == 85) {
                            //moongate
                            if (moonphase != 255) {
                                //for (i4=7;i4>=0;i4--){
                                //if (tplayer->party[i4]!=NULL){
                                //OBJremove(tplayer->party[i4]);
                                //OBJadd(moongatex[moonphase],moongatey[moonphase],tplayer->party[i4]);
                                //}
                                //}
                                partyadd(tplayer, moongatex[moonphase], moongatey[moonphase]);
                            } //moonphase
                        } //moongate

                        //boat
                        if (i == 412) {
                            use_craft(tplayer, myobj);
                            goto finishuse;
                        }


                    playeronlyuse_skip:


                        if (i == 431) {
                            //horse(with rider)
                            tnpc2 = (npc *) myobj->more;
                            if (tnpc2->player == tplayer) {
                                //so you can't dismount other players!

                                //find a walkable location to dismount

                                x3 = myobj->x;
                                y3 = myobj->y; //set dismount location

                                //set as current location for ease of coding for now

                                //restore the horses head
                                myobj5 = tnpc2->horse;
                                tnpc2->horse = NULL;
                                crt2 = (creature *) myobj5->more;
                                crt2->wait += 8.0f;
                                OBJadd(myobj->x, myobj->y, myobj5);
                                myobj5->type = myobj->type - 431 + 430;

                                //restore the horses tail
                                myobj6 = (object *) tnpc2->more;
                                myobj6->more = myobj5;
                                myobj6->type = myobj6->type - 431 + 430;

                                //restore the partymember
                                tnpc2->more = NULL;
                                myobj->type = tnpc2->originalobjtype;
                                tnpc2->originalobjtype = NULL;
                                //move partymember to the side of the horse if possible

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
                                if ((x4 == 0) && (y4 == 0)) {
                                    //try from horse's tail
                                    x3 = myobj6->x;
                                    y3 = myobj6->y;
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
                                } //try from horse's tail
                                if (x4 || y4) {
                                    OBJmove_allow = TRUE;
                                    OBJmove2(myobj, x4, y4);
                                    OBJmove_allow = FALSE;
                                }
                            } //so you can't dismount other players
                            goto finishuse;
                        } //horse(with rider)


                        if (i == 430) {
                            //horse(unbridled)
                            for (x3 = -1; x3 <= 7; x3++) {
                                x4 = x3;
                                if (x3 == -1) x4 = tplayer->selected_partymember;
                                if (myobj2 = tplayer->party[x4]) {
                                    if (((abs(myobj2->x - use_beforeredirect->x) <= 1) && (
                                             abs(myobj2->y - use_beforeredirect->y) <= 1)) || (
                                            (abs(myobj2->x - myobj->x) <= 1) && (abs(myobj2->y - myobj->y) <= 1))) {
                                        tnpc2 = (npc *) myobj2->more;
                                        if (tnpc2->horse == NULL) {
                                            //check that npc is not currently on a horse
                                            //lastly, check if we own the deed to that horse!
                                            for (x6 = 0; x6 <= 7; x6++) {
                                                if (tplayer->party[x6]) {
                                                    tnpc3 = (npc *) tplayer->party[x6]->more;
                                                    x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                                                }
                                            } //!=NULL,x6
                                            OBJlist_last = NULL;
                                            for (y6 = 0; y6 < x5; y6++) {
                                                if (OBJlist_list[y6]->type == 448) {
                                                    //horse papers
                                                    if ((object *) OBJlist_list[y6]->more == myobj)
                                                        goto
                                                                usehorse_gotpapers;
                                                }
                                            }
                                            x5 = 0;
                                            //is this a wild horse?
                                            if ((abs(myobj->x - 280) <= 32) && (abs(myobj->y - 441) <= 32)) x5 = 1;
                                            if (myobj->info & 4) {
                                                crt2 = (creature *) myobj->more;
                                                if (crt2->flags & 128) x5 = 1;
                                            }
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            if (x5) txtadd(t, "You don't own this horse!");
                                            else txtadd(t, "You cannot ride a wild horse!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto finishuse;

                                        usehorse_gotpapers:
                                            tnpc2->originalobjtype = myobj2->type;
                                            OBJremove(myobj2);
                                            OBJadd(myobj->x, myobj->y, myobj2);
                                            myobj2->type = myobj->type - 430 + 431;
                                            crt2 = (creature *) myobj->more;
                                            tnpc2->more = crt2->more;
                                            myobj3 = (object *) crt2->more;
                                            myobj3->more = myobj2; //make rear end of horse point to partymember(myobj2)
                                            myobj3->type = myobj3->type - 430 + 431;
                                            OBJremove(myobj); //remove front end of horse
                                            tnpc2->horse = myobj; //backup pointer to front half of horse for later
                                            goto finishuse;
                                        }
                                    }
                                }
                            }
                            goto finishuse;
                        } //horse(unbridled)


                        if (i == 163) {
                            //use bed in your own house to rest
                            if (i4 = playerlist[tpl]->GNPCflags[28]) {
                                //check if the bed is within boundaries
                                x7 = myobj->x;
                                y7 = myobj->y;
                                if (x7 >= housex1[i4]) {
                                    if (x7 <= housex2[i4]) {
                                        if (y7 >= housey1[i4]) {
                                            if (y7 <= housey2[i4]) {
                                                for (y6 = 0; y6<housepnext[i4]; y6++) {
                                                    if (x7 == housepx[i4][y6]) {
                                                        if (y7 == housepy[i4][y6]) {
                                                            if (houseowner(playerlist[tpl], i4)) { tplayer->rest = 17; }
                                                            goto finishuse;
                                                        }
                                                    } //housepx,housepy
                                                } //y
                                            }
                                        }
                                    }
                                } //boundries check
                            }
                            goto finishuse;
                        }

                        if (i == 223) {
                            //powder keg
                            OBJsave(myobj->x, myobj->y);
                            if (i2) {
                                myobj->type = i;
                                myobj->more2 = tplayer->id;
                                goto finishuse;
                            }
                            myobj->type = i + 1 * 1024;
                            myobj->info |= 7680; //1111000000000 obj reserved timer
                            myobj->more2 = tplayer->id;
                            stealing(tplayer, myobj);
                            goto finishuse;
                        }

                        if (i == 334) {
                            //secret door
                            use_basic_toggle(NULL, myobj);
                            OBJcheckflags(x, y);
                            goto finishuse;
                        }

                        if (i == 164) {
                            //fireplace
                            use_basic_toggle(NULL, myobj);
                            goto finishuse;
                        }

                        if (i == 206) {
                            //brazier
                            if (myobj->type >> 10 < 2) {
                                use_basic_toggle(NULL, myobj);
                            } else {
                                /* purple/blue brazier can't be doused */
                            }
                            goto finishuse;
                        }

                        if (i == 253) {
                            //campfire
                            use_basic_toggle(NULL, myobj);
                            goto finishuse;
                        }

                        if (i == 268) {
                            //lever
                            use_toggle(NULL, myobj);
                            goto finishuse;
                        }

                        if (i == 288) {
                            //crank
                            use_toggle(NULL, myobj);
                            goto finishuse;
                        }

                        if (i == 174) {
                            //switch
                            use_toggle(NULL, myobj);
                            goto finishuse;
                        }
                        if (i == 122) {
                            //candle
                            OBJsave(myobj->x, myobj->y);
                            use_basic_toggle(NULL, myobj);
                            goto finishuse;
                        }
                        if (i == 145) {
                            //candelabra
                            OBJsave(myobj->x, myobj->y);
                            use_basic_toggle(NULL, myobj);
                            goto finishuse;
                        }
                        if (i == 186) {
                            //barrel
                            OBJsave(myobj->x, myobj->y);
                            use_basic_toggle(NULL, myobj);
                            goto finishuse;
                        }
                        if (i == 192) {
                            //crate
                            OBJsave(myobj->x, myobj->y);
                            use_basic_toggle(NULL, myobj);
                            goto finishuse;
                        }
                        if (i == 98) {
                            //chest
                            if (myobj->type >> 10 < 2) {
                                //can only use unlocked chests
                                OBJsave(myobj->x, myobj->y);
                                use_basic_toggle(NULL, myobj);
                            }
                            goto finishuse;
                        }

                        if (i == 62) {
                            //vortex cube use

                            if (tplayer->GNPCflags[19]) {
                                //replay endgame sequence?
                                if ((myobj->x == 923) && (myobj->y == 850)) {
                                    if ((tplayer->GNPCflags[18] & 16) == 0) {
                                        tplayer->GNPCflags[18] |= (2 + 4); //restore viewing of lenses
                                        txtset(t, "?");
                                        t->d2[0] = 22;
                                        NET_send(NETplayer, tplayer->net, t); //play endgame sequence!
                                        tplayer->GNPCflags[18] |= 16; //endgame sequence played!
                                    }
                                }
                                goto finishuse;
                            }

                            if ((tplayer->GNPCflags[18] & 7) == 7) {
                                if ((myobj->x == 923) && (myobj->y == 850)) {
                                    if (tplayer->party[0]) {
                                        tplayer->GNPCflags[18] |= 8; //don't show codex!
                                        tplayer->GNPCflags[19] = 1; //sacred quest complete!!!

                                        tplayer->karma += 20.0f;
                                        tnpc3 = (npc *) tplayer->party[0]->more;
                                        tnpc3->s += 3;
                                        tnpc3->d += 3;
                                        tnpc3->i += 3;
                                        tnpc3->mp_max = tnpc3->i * 2; //primary party member INT*2
                                        tnpc3->wt_max = tnpc3->s * 4 * 100;
                                        tnpc3->wt2_max = tnpc3->s * 50;
                                        //remove amulet of submission
                                        if (tnpc3->neck) {
                                            if (tnpc3->neck->type == 76) tnpc3->neck = NULL;
                                        }
                                        tnpc3->upflags |= 1;

                                        myobj3 = OBJnew();
                                        myobj3->type = 87;
                                        myobj3->more2 = 0;
                                        myobj3->info = 256; //orb of moons
                                        additemroot(tnpc3, myobj3);
                                        tplayer->GNPCflags[22] |= 0xFFFFFFFF; //make all destinations available

                                        txtset(t, "?");
                                        t->d2[0] = 22;
                                        NET_send(NETplayer, tplayer->net, t); //play endgame sequence!
                                        tplayer->GNPCflags[18] |= 16; //endgame sequence played!
                                    }
                                }
                            }
                            goto finishuse;
                        }


                        /*
            if (i==90){ //torch
            if (i2==0) myobj->type=i+1*1024;
            if (i2>=1) myobj->type=i;
            goto finishuse;
            }
            */
                        if (i == OBJ_PASSTHROUGH_V) {
                            //v-passthrough
                            if (i2 == 0) {
                                if (myobj->more == NULL) {
                                    myobj->more = OBJnew();
                                    myobj2 = (object *) myobj->more;
                                    myobj2->more = myobj; //back link
                                    myobj2->type = OBJ_PASSTHROUGH_V + 1 * 1024;
                                    OBJadd(myobj->x - 1, myobj->y - 1, myobj2);
                                } else {
                                    OBJadd(myobj->x - 1, myobj->y - 1, (object *) myobj->more);
                                }
                                x = myobj->x;
                                y = myobj->y - 1;
                                OBJremove(myobj);
                                myobj->type = OBJ_PASSTHROUGH_V + 2 * 1024;
                                OBJadd(x, y, myobj);
                                goto finishuse;
                            } else {
                                if (i2 == 1) {
                                    myobj2 = myobj;
                                    myobj = (object *) myobj->more;
                                } else { myobj2 = (object *) myobj->more; }
                                if (myobj2 != NULL) OBJremove(myobj2);
                                x = myobj->x;
                                y = myobj->y + 1;
                                OBJremove(myobj);
                                myobj->type = OBJ_PASSTHROUGH_V;
                                OBJadd(x, y, myobj);
                                goto finishuse;
                            }
                        }

                        if (i == OBJ_PASSTHROUGH_H) {
                            //h-passthrough
                            if (i2 == 0) {
                                if (myobj->more == NULL) {
                                    myobj->more = OBJnew();
                                    myobj2 = (object *) myobj->more;
                                    myobj2->more = myobj; //back link
                                    myobj2->type = OBJ_PASSTHROUGH_H + 1 * 1024;
                                    OBJadd(myobj->x - 1, myobj->y - 1, myobj2);
                                } else {
                                    OBJadd(myobj->x - 1, myobj->y - 1, (object *) myobj->more);
                                }
                                x = myobj->x - 1;
                                y = myobj->y;
                                OBJremove(myobj);
                                myobj->type = OBJ_PASSTHROUGH_H + 2 * 1024;
                                OBJadd(x, y, myobj);
                                goto finishuse;
                            } else {
                                if (i2 == 1) {
                                    myobj2 = myobj;
                                    myobj = (object *) myobj->more;
                                } else { myobj2 = (object *) myobj->more; }
                                if (myobj2 != NULL) OBJremove(myobj2);
                                x = myobj->x + 1;
                                y = myobj->y;
                                OBJremove(myobj);
                                myobj->type = OBJ_PASSTHROUGH_H;
                                OBJadd(x, y, myobj);
                                goto finishuse;
                            }
                        }

                        for (i4 = 297; i4 <= 300; i4++) {
                            if (i == i4) {
                                //oak door, ...
                                if (i2 >= 8) {
                                    txtsetchar(t, 8);
                                    txtaddchar(t, 255);
                                    txtadd(t, "It's locked!");
                                    NET_send(NETplayer, tplayer->net, t);
                                    goto finishuse; //door is locked!
                                }
                                if (myobj->info & 8) myobj = (object *) myobj->more; //<-
                                myobj2 = (object *) myobj->more;
                                i2 = myobj->type >> 10;

                                if (i2 & 1) {
                                    myobj2 = myobj;
                                    myobj = (object *) myobj->more;
                                } else { myobj2 = (object *) myobj->more; }

                                if (i2 < 4) {
                                    i2 = i2 >> 1;
                                    i2 = i2 << 1;
                                    i2 = i2 & 2;
                                    myobj->type = i + ((i2 + 4) * 1024);
                                    myobj2->type = i + ((i2 + 5) * 1024);
                                    OBJcheckflags(myobj->x, myobj->y);
                                    OBJcheckflags(myobj2->x, myobj2->y);

                                    x2 = myobj->x;
                                    y2 = myobj->y;
                                    i3 = SFnew(x2, y2);
                                    //i3 points to new sf
                                    sf[i3].type = 4;
                                    sf[i3].x = x2;
                                    sf[i3].y = y2;
                                    sf[i3].wait = 1.0f; //NULL
                                    sf[i3].more = 2;

                                    goto finishuse;
                                } else {
                                    i2 = i2 >> 1;
                                    i2 = i2 << 1;
                                    i2 = i2 & 2;
                                    myobj->type = i + (i2 * 1024);
                                    myobj2->type = i + ((i2 + 1) * 1024);
                                    OBJcheckflags(myobj->x, myobj->y);
                                    OBJcheckflags(myobj2->x, myobj2->y);

                                    x2 = myobj->x;
                                    y2 = myobj->y;
                                    i3 = SFnew(x2, y2);
                                    //i3 points to new sf
                                    sf[i3].type = 4;
                                    sf[i3].x = x2;
                                    sf[i3].y = y2;
                                    sf[i3].wait = 1.0f; //NULL
                                    sf[i3].more = 1;

                                    goto finishuse;
                                } //<4
                            } //i==i4
                        } //i4 loop


