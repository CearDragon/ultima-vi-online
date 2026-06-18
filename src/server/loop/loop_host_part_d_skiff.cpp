// LHS-P6.9: part_d_skiff -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: skiff drop/pickup (skiffdrop, skiffpickup). Move-only.
                            skiffdrop:
                                //can it be dropped from any party member in the location?
                                //for (z=0;z<=7;z++){
                                //myobj2=tplayer->party[z];
                                //if (myobj2!=NULL){
                                //if (OBJcheckbolt(myobj2->x,myobj2->y,x,y)==FALSE){
                                //if ((abs(myobj2->x-x)<=8)&&(abs(myobj2->y-y)<=8)){

                                z2 = 9;
                                myobj2 = NULL;
                                for (z = 0; z <= 7; z++) {
                                    if (myobj3 = tplayer->party[z]) {
                                        if (OBJcheckbolt(myobj3->x, myobj3->y, x, y) == FALSE) {
                                            z3 = abs(myobj3->x - x);
                                            z4 = abs(myobj3->y - y);
                                            if (z4 > z3) z3 = z4;
                                            if (z3 < z2) {
                                                myobj2 = myobj3;
                                                z2 = z3;
                                            }
                                        }
                                    }
                                }
                                if (myobj2) {
                                    if ((tplayer->mobj->type & 1023) == 420) {
                                        //balloon drop
                                        if ((abs(myobj2->x - x) > 1) || (abs(myobj2->y - y) > 1)) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You cannot throw a balloon!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped;
                                        }
                                        if (tplayer->craft == NULL) {
                                            if (usinghorse(tplayer) == NULL) {
                                                if ((tplayer->party[tplayer->selected_partymember]->x < 1024) || (
                                                        tplayer->party[tplayer->selected_partymember]->y >= 256)) {
                                                    //use balloon
                                                    myobj = tplayer->mobj;
                                                    itemused = 2;
                                                    i = myobj->type & 1023;
                                                    i2 = myobj->type >> 10;
                                                    tplayer->mobj = NULL;
                                                    goto useinventoryitem;
                                                }
                                            }
                                        }
                                        goto itemdropped;
                                    }

                                    if (tplayer->mobj->info & 256) {
                                        //drop quest item
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "You cannot put a quest item here!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto itemdropped;
                                    }

                                    if ((tplayer->mobj->type & 1023) == 414) {
                                        //skiff drop
                                        z2 = 0;
                                        if (abs(myobj2->x - x) == 1) {
                                            z = bt[myobj2->y][x] & 1023;
                                            if ((z < 8) || (z >= 48)) {
                                                //not water
                                                if ((bt[myobj2->y][x] & 1024) == 0) {
                                                    //not walkable
                                                    z2 = 1;
                                                }
                                            }
                                        }
                                        if (abs(myobj2->y - y) == 1) {
                                            z = bt[y][myobj2->x] & 1023;
                                            if ((z < 8) || (z >= 48)) {
                                                //not water
                                                if ((bt[y][myobj2->x] & 1024) == 0) {
                                                    //not walkable
                                                    z2 = 1;
                                                }
                                            }
                                        }
                                        if ((abs(myobj2->x - x) > 1) || (abs(myobj2->y - y) > 1)) z2 = 1;
                                        if (z2) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You cannot throw a skiff!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped;
                                        }
                                    }

                                    if (tplayer->mobj->type == 149) {
                                        //deed drop
                                        //in current boat?
                                        if ((object *) tplayer->mobj->more == tplayer->party[0]) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You cannot drop this deed now!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped;
                                        }
                                        //currently holding the boat this deed points to?
                                        myobj3 = (object *) tplayer->mobj->more;
                                        if ((myobj3->x == 0) && (myobj3->y == 0)) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You must drop the skiff first!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped;
                                        }
                                    }

                                    if (tplayer->mobj->type == 448) {
                                        //horse papers
                                        if (((object *) tplayer->mobj->more)->x == 0) {
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "You cannot drop these horse papers now!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto itemdropped;
                                        }
                                        crt2 = (creature *) ((object *) tplayer->mobj->more)->more;
                                        crt2->respawn_y = 0xFFFF;
                                    } //448


                                    x5 = OBJlist(tplayer->mobj); //x5=last object number
                                    OBJlist_last = NULL;
                                    //clear notforsale marks
                                    for (y6 = 0; y6 < x5; y6++) {
                                        OBJlist_list[y6]->info |= 32768;
                                        OBJlist_list[y6]->info ^= 32768;
                                    } //y6


                                    /*
                  if (tplayer->mobj->info&128){//stolen item
                  for (i3=0;i3<=15;i3++) if (stolenitemwarning[i3]==tplayer->mobj) goto stolenitem_alreadyadded2;
                  for (i3=0;i3<=15;i3++){
                  if (stolenitemwarning[i3]==NULL){
                  stolenitemwarning[i3]=tplayer->mobj;
                  stolenitemwarning_wait[i3]=2.0f;
                  stolenitemwarning_player[i3]=tplayer;
                  goto stolenitem_alreadyadded2;
                  }//==NULL
                  }//i3
                  }//stolen item
                  stolenitem_alreadyadded2:
                  */
                                    if (tplayer->mobj->info & 128) {
                                        //stolen item
                                        i3 = SFnew(x, y); //destination is more important than the source
                                        sf[i3].type = 21; //stolen item warning
                                        sf[i3].x = x;
                                        sf[i3].y = y;
                                        sf[i3].more = tplayer->mobj->type;
                                        sf[i3].wait = 2.0f;
                                    }

                                    //OBJadd(x,y,tplayer->mobj);
                                    //drop sfx!
                                    i3 = SFnew(x, y); //destination is more important than the source
                                    sf[i3].type = 9; //GENERIC throw object
                                    sf[i3].x = myobj2->x;
                                    sf[i3].y = myobj2->y;
                                    sf[i3].x2 = x;
                                    sf[i3].y2 = y;
                                    sf[i3].more = 0xFFFF;
                                    sf[i3].wait = tplayer->mobj->type;

                                    //drop sound effect
                                    if ((tplayer->mobj->type & 1023) == 88) {
                                        //gold
                                        if (tplayer->mobj->more2 >= 5) {
                                            i3 = SFnew(x, y);
                                            sf[i3].type = 4; //sound
                                            sf[i3].x = x;
                                            sf[i3].y = y;
                                            sf[i3].wait = 1.0f; //NULL
                                            sf[i3].more = 6; //coinmany
                                        } else {
                                            i3 = SFnew(x, y);
                                            sf[i3].type = 4; //sound
                                            sf[i3].x = x;
                                            sf[i3].y = y;
                                            sf[i3].wait = 1.0f; //NULL
                                            sf[i3].more = 7; //coinfew
                                        }
                                    }

                                    //smash it?
                                    i4 = 0;
                                    if ((tplayer->mobj->type & 1023) == 275) i4 = 1; //potions
                                    if (tplayer->mobj->type == 115) i4 = 1; //
                                    if (tplayer->mobj->type == 116) i4 = 1; //
                                    if (tplayer->mobj->type == 117) i4 = 1; //
                                    if (tplayer->mobj->type == 118) i4 = 1; //
                                    if (tplayer->mobj->type == 119) i4 = 1; //
                                    if (tplayer->mobj->type == 120) i4 = 1; //mug
                                    if (tplayer->mobj->type == 123) {
                                        //mirror
                                        if ((abs(myobj2->x - x) > 1) || (abs(myobj2->y - y) > 1)) {
                                            sf[i3].wait += 32768;
                                            tplayer->mobj->type = 123 + 1024 * 2;
                                            stealing(tplayer, tplayer->mobj);
                                        }
                                    }
                                    if ((tplayer->mobj->type == 179) || (tplayer->mobj->type == 180)) {
                                        //bucket of milk/water
                                        if ((abs(myobj2->x - x) > 1) || (abs(myobj2->y - y) > 1)) {
                                            tplayer->mobj->type = 178;
                                            sf[i3].wait = tplayer->mobj->type;
                                            stealing(tplayer, tplayer->mobj);
                                        }
                                    }
                                    if (tplayer->mobj->type == 92) i4 = 1; //silver snake venom
                                    if (tplayer->mobj->type == 161) i4 = 1; //jug
                                    if (tplayer->mobj->type == 83) i4 = 1; //oil flask
                                    if (tplayer->mobj->type == 48) i4 = 1; //glass sword
                                    if (tplayer->mobj->type == 183) i4 = 1; //jar
                                    if (tplayer->mobj->type == 184) i4 = 1; //jar with honey
                                    if ((abs(myobj2->x - x) <= 1) && (abs(myobj2->y - y) <= 1)) i4 = 0;
                                    if (i4) {
                                        sf[i3].wait += 32768;
                                        OBJrelease(tplayer->mobj);
                                        stealing(tplayer, tplayer->mobj);
                                    } else {
                                        OBJadd(x, y, tplayer->mobj);
                                    }

                                    tplayer->mobj = NULL; //clear selected mouse object
                                    goto itemdropped;
                                    //}//<=8
                                    //}
                                    //}}
                                } //myobj2

                            itemdropped:;
                            } //bt flag
                        } //myobj==NULL
                    } //clicked in vf

                    goto skip_pickup;
                } //mobj!=NULL
                //mobj==NULL

                //pickup:
                if ((tplayer->mf == 255) && (tplayer->key & KEYmbclick)) {
                    if (tplayer->action == 0) {
                        x = tpx + tplayer->mx / 32;

                        y = tpy + tplayer->my / 32;
                        myobj = OBJfindlast(x, y);
                        if (myobj != NULL) {
                            if (myobj->type == 51) {
                                //force field
                                myobj2 = (object *) myobj->prev;
                                if (playerlist[tpl]->GNPCflags[3] & (1 << (myobj2->type >> 10)))
                                    myobj = (object *) myobj->prev;
                            } //force field


                            i = myobj->type;
                            //i2=i>>10; //26/11/2004 removed for Sherry compatibility
                            i2 = sprlnk[i & 1023];


                            if (((obji[i2].flags & 128) != 0) || (keyon[VK_SPACE] && U6O_DEBUG)) {
                                //check if item is next to player
                                static unsigned char pickup_partymember;
                                z2 = 0;
                                for (z = 7; z >= 0; z--) {
                                    myobj2 = tplayer->party[z];
                                    if (myobj2 != NULL) {
                                        if (abs(myobj2->x - myobj->x) <= 1) {
                                            if (abs(myobj2->y - myobj->y) <= 1) {
                                                z2 = 1;
                                                pickup_partymember = z;
                                            }
                                        }
                                    }
                                }
                                if (z2 == 1) {
                                    if ((myobj->info & 112) == 0) {
                                        //this is a fixed map object

                                        //void OBJsave(unsigned short x,unsigned short y){

                                        OBJsave(myobj->x, myobj->y);
                                        //MessageBox(NULL,"OBJsave() called","Ultima 6 Online",MB_OK);

                                        //copy everythng! yes everything!
                                        //*note no temp flash is needed as a warning

                                        //scan through the next list, if we find a container, add it to the list
                                        //if there were any container items copy them, then try again
                                        //idea! set temp flag while scanning!! (if possible)


                                        //set temp flag of any object that can be picked up/used
                                    }


                                    if ((myobj->type & 1023) == 73) {
                                        //moonstone
                                        if ((tplayer->GNPCflags[4] & (1 << (myobj->type >> 10))) == 0) {
                                            tplayer->GNPCflags[4] |= (1 << (myobj->type >> 10));
                                            //duplicate it
                                            myobj2 = OBJnew();
                                            myobj2->type = myobj->type;
                                            myobj2->info = myobj->info;
                                            tplayer->mobj = myobj2;
                                        }
                                        goto nopickup;
                                    }

                                    if ((myobj->type >= 242) && (myobj->type <= 249)) {
                                        //rune
                                        if (myobj->info & 256) {
                                            //must be a quest item
                                            if ((tplayer->GNPCflags[5] & (1 << (myobj->type - 242))) == 0) {
                                                tplayer->GNPCflags[5] |= (1 << (myobj->type - 242));
                                                //duplicate it
                                                myobj2 = OBJnew();
                                                myobj2->type = myobj->type;
                                                myobj2->info = myobj->info;
                                                tplayer->mobj = myobj2;
                                            }
                                            goto nopickup;
                                        }
                                    }

                                    if (myobj->type == (275 + 2 * 1024)) {
                                        //potions
                                        if (myobj->info & 256) {
                                            if ((tplayer->GNPCflags[32] & myobj->more2) == 0) {
                                                tplayer->GNPCflags[32] |= myobj->more2;
                                                //duplicate it
                                                myobj2 = OBJnew();
                                                myobj2->type = myobj->type;
                                                myobj2->info = myobj->info;
                                                tplayer->mobj = myobj2;
                                            }
                                            goto nopickup;
                                        }
                                    }


                                    if ((myobj->type >= 400) && (myobj->type <= 407)) {
                                        //map(0-7)
                                        if (myobj->info & 256) {
                                            //must be a quest item
                                            if ((tplayer->GNPCflags[7] & (1 << (myobj->type - 400))) == 0) {
                                                tplayer->GNPCflags[7] |= (1 << (myobj->type - 400));
                                                //duplicate it
                                                myobj2 = OBJnew();
                                                myobj2->type = myobj->type;
                                                myobj2->info = myobj->info;
                                                tplayer->mobj = myobj2;
                                            }
                                            goto nopickup;
                                        }
                                    }

                                    if (myobj->type == 62) {
                                        //vortex cube
                                        if ((myobj->x == 1939) && (myobj->y == 57)) {
                                            if ((tplayer->GNPCflags[6] & (1 << 1)) == 0) {
                                                tplayer->GNPCflags[6] |= (1 << 1);
                                                //duplicate it
                                                myobj2 = OBJnew();
                                                myobj2->type = myobj->type;
                                                myobj2->info = myobj->info;
                                                tplayer->mobj = myobj2;
                                            }
                                            goto nopickup;
                                        }
                                        if ((myobj->x == 923) && (myobj->y == 850)) {
                                            goto nopickup;
                                        }
                                    }

                                    if (myobj->type == 389) {
                                        //silver tablet
                                        if ((tplayer->GNPCflags[6] & (1 << 2)) == 0) {
                                            tplayer->GNPCflags[6] |= (1 << 2);
                                            //duplicate it
                                            myobj2 = OBJnew();
                                            myobj2->type = myobj->type;
                                            myobj2->info = myobj->info;
                                            tplayer->mobj = myobj2;
                                        }
                                        goto nopickup;
                                    }
                                    if (myobj->type == 390) {
                                        //silver tabletII
                                        if ((tplayer->GNPCflags[6] & (1 << 3)) == 0) {
                                            tplayer->GNPCflags[6] |= (1 << 3);
                                            //duplicate it
                                            myobj2 = OBJnew();
                                            myobj2->type = myobj->type;
                                            myobj2->info = myobj->info;
                                            tplayer->mobj = myobj2;
                                        }
                                        goto nopickup;
                                    }

                                    if (myobj->type == 270) {
                                        //balloon plans
                                        if ((tplayer->GNPCflags[6] & (1 << 4)) == 0) {
                                            tplayer->GNPCflags[6] |= (1 << 4);
                                            //duplicate it
                                            myobj2 = OBJnew();
                                            myobj2->type = myobj->type;
                                            myobj2->info = myobj->info;
                                            tplayer->mobj = myobj2;
                                        }
                                        goto nopickup;
                                    }

                                    if (myobj->type == 395) {
                                        //broken gargoyle lens
                                        if ((tplayer->GNPCflags[6] & (1 << 5)) == 0) {
                                            tplayer->GNPCflags[6] |= (1 << 5);
                                            //duplicate it
                                            myobj2 = OBJnew();
                                            myobj2->type = myobj->type;
                                            myobj2->info = myobj->info;
                                            tplayer->mobj = myobj2;
                                        }
                                        goto nopickup;
                                    }

                                    if (myobj == nuggetsfix) {
                                        //nuggets
                                        if (tplayer->online >= 1536) {
                                            if ((tplayer->GNPCflags[6] & (1 << 6)) == 0) {
                                                tplayer->GNPCflags[6] |= (1 << 6);
                                                //duplicate it
                                                myobj2 = OBJnew();
                                                myobj2->type = myobj->type;
                                                myobj2->info = myobj->info;
                                                myobj2->more2 = myobj->more2;
                                                tplayer->mobj = myobj2;
                                            }
                                        }
                                        goto nopickup;
                                    }

                                    if (myobj->type == 394) {
                                        //brit lens
                                        goto nopickup;
                                    }
                                    if (myobj->type == 396) {
                                        //garg lens
                                        goto nopickup;
                                    }

                                    if (myobj->type == 173) {
                                        //trap
                                        if ((myobj->info & (1 << 9)) == 0) goto nopickup;
                                        OBJsave(myobj->x, myobj->y);
                                        myobj->more2 = tplayer->id; //id
                                        myobj->info |= (1 << 9);
                                        if (tnpc->protection == 0) {
                                            tnpc = (npc *) tplayer->party[pickup_partymember]->more;
                                            x6 = tnpc->hp >> 3;
                                            if (!x6) x6 = 1; //-1/8th health
                                            tnpc->hp -= x6;
                                            i3 = SFnew(tplayer->party[pickup_partymember]->x,
                                                       tplayer->party[pickup_partymember]->y);
                                            sf[i3].type = 1; //attack
                                            sf[i3].x2 = x6;
                                            sf[i3].x = tplayer->party[pickup_partymember]->x;
                                            sf[i3].y = tplayer->party[pickup_partymember]->y;
                                            sf[i3].wait = 0.125f; //NULL
                                            sf[i3].more = 1;
                                            tnpc->upflags |= 2; //hp
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, tnpc->name);
                                            txtadd(t, " wounded!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        } //protection
                                    } //173

                                    if ((myobj->type & 1023) == 414) {
                                        //skiff pickup
                                        if (myobj->info & 2) goto nopickup; //in use!
                                        if ((myobj->info & (1 << 9)) == 0) {
                                            //NOT free skiff
                                            //deed check
                                            for (x6 = 0; x6 <= 7; x6++) {
                                                if (tplayer->party[x6]) {
                                                    tnpc3 = (npc *) tplayer->party[x6]->more;
                                                    x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                                                }
                                            } //!=NULL,x6
                                            OBJlist_last = NULL;
                                            for (y6 = 0; y6 < x5; y6++) {
                                                if (OBJlist_list[y6]->type == 149) {
                                                    //deed
                                                    if (OBJlist_list[y6]->more == myobj) goto skiffpickup;
                                                }
                                            }
                                        } //NOT free skiff
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "A deed is required!");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto nopickup;
                                    }
                                skiffpickup:

                                    //if (keyon[VK_SHIFT]) CLIENTplayer->key|=KEYquan;
                                    if (tplayer->key & KEYquan) {
                                        if (tplayer->quan) {
                                            if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].flags & 4096) {
                                                if (tplayer->quan < myobj->more2) {
                                                    myobj2 = OBJnew();
                                                    myobj2->type = myobj->type;
                                                    myobj2->info = myobj->info;
                                                    myobj2->more2 = tplayer->quan;
                                                    myobj->more2 -= tplayer->quan;
                                                    tplayer->mobj = myobj2;
                                                    goto nopickup;
                                                } //<
                                            } //multiple
                                        } //->quan
                                    } //KEYquan
                                    tplayer->mobj = myobj;
                                    OBJremove(tplayer->mobj);

                                    //if (objb_last>28039) MessageBox(NULL,"OBJnew: prep1","Ultima 6 Online",MB_OK);

                                nopickup:;
                                }
                            } //flag check
                        } //mobj!=NULL
                    } //action0

                    if (tplayer->action == 1) {
                        //use!

                        x = tpx + tplayer->mx / 32;
                        y = tpy + tplayer->my / 32;

                        z2 = 0;
                        for (z = 0; z <= 7; z++) {
                            myobj2 = tplayer->party[z];
                            if (myobj2 != NULL) {
                                if (abs(myobj2->x - x) <= 1) {
                                    if (abs(myobj2->y - y) <= 1) {
                                        z2 = 1;
                                    }
                                }
                            }
                        }
                        if (z2 == 0) goto finishuse;

                        myobj = tplayer->party[tplayer->selected_partymember];
                        //tnpc=(npc*)myobj->more;

                        /*
            x2=myobj->x;
            y2=myobj->y;
            if (OBJcheckbolt(x2,y2,x,y)){
            myobj=OBJnew();
            myobj->type=338+2048;
            OBJadd(OBJcheckbolt_x,OBJcheckbolt_y,myobj);
            }else{
            //u6ob
            i3=SFnew(x,y); //destination is more important than the source
            sf[i3].type=2; //arrow
            sf[i3].x=x2;
            sf[i3].y=y2;
            sf[i3].x2=x;
            sf[i3].y2=y;
            sf[i3].more=0xFFFF;
            sf[i3].wait=1;
            }
            */


                        itemused = 0;
                        myobj = OBJfindlast(x, y);
