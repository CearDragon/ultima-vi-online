// LHS-P3.3: part_a_housestore -- CONTINUES MEGA A (inside for/if(playerlist)).
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_a_save.cpp.
// Contents: house storage add + housescan (housestorageadd0/1, housescan0/1/2).
                            housestorageadd0:

                                //OBJlist->txtlist
                                for (y6 = 0; y6 < x5; y6++) {
                                    t2->ds[0] = OBJlist_list[y6]->type;
                                    t2->ds[1] = OBJlist_list[y6]->info;


                                    //invalid item!
                                    if (obji[sprlnk[OBJlist_list[y6]->type & 1023]].weight == 0) {
                                        if (obji[sprlnk[OBJlist_list[y6]->type & 1023]].flags & 1024) {
                                            //CONTAINER!
                                            //bag
                                            t2->ds[0] = OBJ_BAG; //change item type to a bag
                                            t2->ds[1] = 112; //set info to avoid conflict
                                        } else {
                                            //rubber duck
                                            t2->ds[0] = 169; //ducky
                                            t2->ds[1] = 112; //set info to avoid conflict
                                            txtset(newt1, "invalid item make duck ------------------------ ");
                                            txtnumint(t6, OBJlist_list[y6]->type);
                                            txtadd(newt1, t6);
                                            LOGadd(newt1);
                                        }
                                    }


                                    txtadd(t, t2);
                                    t2->dl[0] = OBJlist_list[y6]->more2;


                                    //if (housestorageadd){
                                    //txtnumint(t5,t2->ds[0]); txtadd(t5,":type"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
                                    //}

                                    //txtnumint(t5,OBJlist_list[y6]->type&1023);
                                    //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


                                    if (t2->dl[0]) {
                                        //->more!=NULL
                                        if (obji[sprlnk[OBJlist_list[y6]->type & 1023]].flags & 1024) {
                                            //container
                                            for (y7 = 0; y7 < x5; y7++) {
                                                //link
                                                if ((object *) OBJlist_list[y6]->more == OBJlist_list[y7])
                                                    t2->dl[0] = y7 + 1;
                                            } //y7
                                        } //multiple
                                    } //->more!=NULL

                                    if (OBJlist_list[y6]->type == 448) {
                                        //horse papers (save)
                                        myobj2 = (object *) OBJlist_list[y6]->more;

                                        //latest bug, caused by mose, dunno what he did
                                        //myobj2 is obviously invalid! it should point to the horses head... yet clearly did not!

                                        //step 1. somehow get rid of deed while on horse (such as asking partymember to leave)
                                        //step 2. exit (horse is now released)
                                        //step 3. deed is now stuffed

                                        //if (myobj2==NULL) exit(123);


                                        //NEW GPF THIS LINE! x100!
                                        if (myobj2->x) {
                                            //horse is not being ridden (so it doesn't matter if we are in a boat or not)

                                            t2->dl[0] = myobj2->x + myobj2->y * 2048;
                                            //use as few bits as possible(21 bits total)
                                            //21

                                            t2->dl[0] += (((creature *) myobj2->more)->respawn_x << 21);
                                            //8

                                            //add direction
                                            t2->dl[0] += (((myobj2->type >> 11) & 3) << 29);
                                            //2

                                            t->d2[t->l - 1] &= 225; //11100001b
                                            t->d2[t->l - 1] += (8 << 1); //horse without rider!

                                            if (playerlist[tpl]->save_update != -1) {
                                                //player is really leaving, remove horse
                                                OBJmove_allow = TRUE;
                                                OBJmove2(myobj2, DEATHPOSX, DEATHPOSY);
                                                OBJmove_allow = FALSE;
                                            }
                                        } else {
                                            //horse is being ridden by a partymember
                                            t2->dl[0] = 0;
                                            //scan to detect which partymember is on that horse
                                            z4 = 0;
                                            for (z3 = 0; z3 <= 7; z3++) {
                                                if (myobj3 = playerlist[tpl]->party[z3]) {
                                                    tnpc3 = (npc *) myobj3->more;
                                                    if ((z3 == 0) || (tnpc3->converse == 201)) {
                                                        if (tnpc3->horse == myobj2) {
                                                            //t2->dl[0]=z4;
                                                            t2->dl[0] = myobj3->x + myobj3->y * 2048;
                                                            //use as few bits as possible(21 bits total)

                                                            //!horse in boat alert! this will set it to 0!
                                                            if (playerlist[tpl]->craft) {
                                                                //it MUST be in a boat
                                                                t2->dl[0] = 2097151; //111111111111111111111b
                                                            }


                                                            t2->dl[0] += (
                                                                ((creature *) ((object *) tnpc3->horse)->more)->
                                                                respawn_x << 21);

                                                            //add direction
                                                            t2->dl[0] += (((myobj3->type >> 11) & 3) << 29);

                                                            t->d2[t->l - 1] &= 225; //11100001b
                                                            t->d2[t->l - 1] += (z4 << 1); //npc#


                                                            goto save_gotridingnpc;
                                                        } //tnpc3->horse==myobj2
                                                        z4++;
                                                    } //converse
                                                } //party[z3]
                                            } //z3
                                            //WARNING! None of the "savable" members of the party are riding this horse!
                                            //at this point the only alternative is that a non-savable partymember is riding the horse
                                            for (z3 = 0; z3 <= 7; z3++) {
                                                if (myobj3 = playerlist[tpl]->party[z3]) {
                                                    tnpc3 = (npc *) myobj3->more;
                                                    if ((z3 != 0) && (tnpc3->converse != 201)) {
                                                        if (tnpc3->horse == myobj2) {
                                                            t2->dl[0] = myobj3->x + myobj3->y * 2048;
                                                            //use as few bits as possible(21 bits total)

                                                            if (playerlist[tpl]->craft) {
                                                                //it MUST be in a boat
                                                                //t2->dl[0]=2097151;//111111111111111111111b

                                                                //if (HORSEDISMOUNT_HORSEOBJECT){//send horse back to Britain's stables free of charge
                                                                //OBJmove_allow=TRUE;
                                                                //OBJmove2(HORSEDISMOUNT_HORSEOBJECT,430,280);
                                                                //OBJmove2(HORSEDISMOUNT_HORSEOBJECT,429,280);
                                                                //OBJmove_allow=FALSE;

                                                                t2->dl[0] = 280 + 441 * 2048;
                                                                //return horse belonging to temp. partymember to stables in Brit.
                                                                //430,280
                                                            }


                                                            t2->dl[0] += (
                                                                ((creature *) ((object *) tnpc3->horse)->more)->
                                                                respawn_x << 21);
                                                            //add direction
                                                            t2->dl[0] += (((myobj3->type >> 11) & 3) << 29);

                                                            t->d2[t->l - 1] &= 225; //11100001b
                                                            t->d2[t->l - 1] += (8 << 1); //horse without rider!

                                                            goto save_gotridingnpc;
                                                        } //tnpc3->horse==myobj2
                                                    } //converse
                                                } //party[z3]
                                            } //z3

                                            exit(5534);
                                        } //myobj2->x
                                    save_gotridingnpc:;
                                        //save horse "status"
                                    } //horse papers (save)


                                    if (OBJlist_list[y6]->type == 149) {
                                        //deed
                                        myobj2 = (object *) OBJlist_list[y6]->more; //myobj2(boat)
                                        if (myobj2 == playerlist[tpl]->party[0]) {
                                            t2->dl[0] = 0xFFFFFFFF;
                                        } else {
                                            t2->dl[0] = myobj2->x + (myobj2->y << 16);
                                            if (playerlist[tpl]->save_update != -1) {
                                                //remove boat (ship)
                                                if ((myobj2->info & 2) == 0) {
                                                    //empty boat?
                                                    if (t2->dl[0]) {
                                                        //not in inventory
                                                        OBJmove_allow = TRUE;
                                                        OBJmove2(myobj2, DEATHPOSX, DEATHPOSY);
                                                        OBJmove_allow = FALSE;
                                                    } //not in inventory
                                                } //empty
                                            } //save_update
                                        }
                                    } //149

                                    txtadd(t, t2);


                                    if (OBJlist_list[y6]->next == NULL) {
                                        t2->dl[0] = 0;
                                        txtadd(t, t2);
                                    } //group finished
                                } //y6
                                t2->dl[0] = 0;
                                txtadd(t, t2); //0+0=all finished

                                //*ADD LOOP FOR MORE PARTY MEMBERS
                                txtadd(t4, t);

                                if (housestorageadd) goto housestorageadd1;
                            } //NOT A U6 NPC
                        } //->party[x8]
                    } //x8 [252]

                    //save house storage objects
                    for (x2 = 0; x2<playerlist[tpl]->GNPCflags[29]; x2++) {
                        myobj = (object *) od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[
                            playerlist[tpl]->GNPCflags[28]][x2]]->next;
                        x5 = 0;
                        txtset(t, "");
                        txtset(t2, "????");
                        x5 = OBJlist(myobj);
                        OBJlist_last = NULL;
                        housestorageadd = 1;
                        goto housestorageadd0;
                    housestorageadd1:
                        housestorageadd = 0;
                        //remove storage objects ONLY if player is leaving
                        if (playerlist[tpl]->save_update != -1) {
                            myobj = (object *) od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[
                                playerlist[tpl]->GNPCflags[28]][x2]]->next;
                            if (myobj) {
                                od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[playerlist[tpl]->
                                    GNPCflags[28]][x2]]->next = NULL;
                                OBJcheckflags(housestoragex[playerlist[tpl]->GNPCflags[28]][x2],
                                              housestoragey[playerlist[tpl]->GNPCflags[28]][x2]);
                            }
                        } //playerlist[tpl]->save_update!=-1
                    } //x2

                    /* luteijn 20080731: as of version 4.0 (=2.0), skip encrypting the file! 
          //ENCRYPT3.0
          encrypt(t4);
          */

                    txtset(t2, ".\\save\\");
                    txtnumint(t, i);
                    txtadd(t, ".sav");
                    txtadd(t2, t);
                    tfh = open2(t2, OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                    put(tfh, t4->d2, t4->l);
                    close(tfh);
                    /* mose: disabling this new save format for now. I really don't have time to make this work.
           luteijn a new format for naming the savegames 
          txtset(t2,"./save/"); 
          txtset(t,"?/"); 
          t->d[0]=playerlist[tpl]->name->d[0];
          if ( (t->d[0] < 'A') || (t->d[0] > 'Z')) {
            t->d[0]='@'; // weird character names go here
          }
          txtadd(t2,t); 
          txtadd(t2,playerlist[tpl]->name); 
          txtadd(t2,"-"); 
          txtadd(t2,playerlist[tpl]->npcname); 
          txtadd(t2,".u6o");   /*idea is to atomically move this over the real save later 
          LOGadd(t2); // scrlog is client only!
          tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
          if (tfh->h!=HFILE_ERROR) {
            put(tfh,t4->d2,t4->l); /* FIXME: NO RETURN CODE, HOW DO WE KNOW IT WORKED so we can overwrite the existing save?  
            close(tfh);

            /* TODO: if saving went well, move the .u6o file to .sav 
          } else {
            txtset(t,"Failed to open new format savefile"); 
            LOGadd(t); 
            /* TODO: find out what went wrong and retry.  
          }*/


                    // luteijn: sent a time update message at this point 
                    // This is done to keep the times reasonably in sync without sending time too often
                    // FIXME: use a small function for this eventually.
                    //send time
                    txtset(t2, "?");
                    t2->d2[0] = 32;
                    txtset(t3, "12345678");
                    t3->dd[0] = btime2;
                    txtadd(t2, t3);
                    NET_send(NETplayer, playerlist[tpl]->net, t2);

                save_complete:

                    //remove prev save file for USER_NAME & USER_PASSWORD!
                    if (i2 != -1) {
                        txtset(t2, ".\\save\\");
                        txtnumint(t, i2);
                        txtadd(t, ".sav");
                        txtadd(t2, t);
                        DeleteFile(t2->d);
                        save_buffer[i2] = 0;
                    }

                save_failed:
                    if (playerlist[tpl]->save_update == -1) {
                        for (i3 = 0; i3 <= 7; i3++) {
                            if (party_ok[i3] == 0) playerlist[tpl]->party[i3] = NULL; //set back to dead
                        }
                        playerlist[tpl]->save_update = 0;
                        goto save_update2;
                    }

                    //auto-lock house & evict all NPC
                    if (i4 = playerlist[tpl]->GNPCflags[28]) {
                        for (i3 = 0; i3<housepnext[i4]; i3++) {
                            x2 = housepx[i4][i3];
                            y2 = housepy[i4][i3];
                            if (myobj = od[y2][x2]) {
                            housescan0:
                                if ((myobj->type & 1023) == 301) {
                                    //doorway
                                    if (myobj->info & 8) myobj2 = (object *) myobj->more;
                                    else myobj2 = myobj; //redirect
                                    if (myobj2->more2) {
                                        //lock number exists
                                        //door WILL be the first object (for player housing, exceptions exist, such an horse ranch in Britain)
                                        myobj3 = od[myobj2->y][myobj2->x];
                                    housescan1:
                                        if (myobj3) {
                                            x3 = myobj3->type & 1023; //GPF
                                            if ((x3 < 297) || (x3 > 300)) {
                                                myobj3 = (object *) myobj3->next;
                                                goto housescan1;
                                            }
                                            myobj3->type &= 4095;
                                            myobj3->type |= 8192;
                                            OBJcheckflags(myobj3->x, myobj3->y);
                                            myobj3 = (object *) myobj3->more;
                                            myobj3->type &= 4095;
                                            myobj3->type |= 8192;
                                            OBJcheckflags(myobj3->x, myobj3->y);
                                        } //lock number exists
                                    } //myobj3
                                } //doorway
                                if (myobj->info & 2) {
                                    //evict all NPC
                                    myobj2 = (object *) myobj->next; //backup ->next pointer
                                    OBJmove_allow = TRUE;
                                    OBJmove2(myobj, houseentrancex[i4], houseentrancey[i4]);
                                    OBJmove_allow = FALSE;
                                    myobj = myobj2;
                                    if (myobj) goto housescan0;
                                    goto housescan2;
                                } //evict all NPC
                                if (myobj->info & 4 && (myobj->type & 1023) == 430) {
                                    //evict horses
                                    myobj2 = (object *) myobj->next; //backup ->next pointer
                                    OBJmove_allow = TRUE;
                                    OBJmove2(myobj, houseentrancex[i4], houseentrancey[i4]);
                                    OBJmove_allow = FALSE;
                                    myobj = myobj2;
                                    if (myobj) goto housescan0;
                                    goto housescan2;
                                } //evict horses
                                if (myobj = (object *) myobj->next) goto housescan0; //GPF
                            } //myobj
                        housescan2:;
                        } //i3
                    } //i4 (housenumber)

                    //disconnect socket
                    if (playerlist[tpl]->net != INVALID_NET) {
                        if (socket_disconnect[playerlist[tpl]->net] == 0) socket_disconnect[playerlist[tpl]->net] = 1;
                    }
                    //return CORE NPC party members
                    for (i3 = 1; i3 <= 7; i3++) {
                        if (playerlist[tpl]->party[i3]) {
                            tnpc3 = (npc *) playerlist[tpl]->party[i3]->more;
                            if ((tnpc3->converse) && (tnpc3->converse != 201)) {
                                horsedismount(playerlist[tpl]->party[i3]);
                                if (HORSEDISMOUNT_HORSEOBJECT) {
                                    OBJmove_allow = TRUE;
                                    OBJmove2(HORSEDISMOUNT_HORSEOBJECT, DEATHPOSX, DEATHPOSY);
                                    OBJmove_allow = FALSE;
                                    //ideally horse crt should be deallocated
                                }


                                tnpc3->player = NULL;
                                tnpc3->upflags = 0;
                                tnpc3->order = 2; //schedule
                                tnpc3->follow = NULL;
                                tnpc3->hp = 30;
                                //tnpc3->exp-=(tnpc->exp/8); ***REMOVED BECAUSE NPC HAS NOT ACTUALLY DIED, JUST LEFT***
                                tnpc3->flags = 0;
                                OBJmove_allow = TRUE;
                                OBJmove2(playerlist[tpl]->party[i3], tnpc3->schedule[0].x, tnpc3->schedule[0].y);
                                OBJmove_allow = FALSE;
                                playerlist[tpl]->party[i3] = NULL;
                            }
                        }
                    }

                    //remove any of player's party from resurrect list
                    for (i3 = 0; i3 <= nresu; i3++) {
                        if (resu[i3]) {
                            if (resu_player[i3] == playerlist[tpl]) {
                                //is this a core NPC (if so then resurrect them at original pos!)
                                tnpc3 = (npc *) resu[i3]->more;
                                if ((tnpc3->converse) && (tnpc3->converse != 201)) {
                                    tnpc3->player = NULL;
                                    tnpc3->upflags = 0;
                                    tnpc3->order = 2; //schedule
                                    tnpc3->follow = NULL;
                                    tnpc3->hp = 30;
                                    //tnpc3->exp-=(tnpc->exp/8);
                                    if (easymodehostn1) {
                                        tnpc3->exp -= new1_getexpdeduction(tnpc3, 2); // c111
                                    } else {
                                        // original
                                        tnpc3->exp -= (tnpc->exp / 8);
                                    }
                                    tnpc3->flags = 0;
                                    OBJmove_allow = TRUE;
                                    OBJmove2(resu[i3], tnpc3->schedule[0].x, tnpc3->schedule[0].y);
                                    OBJmove_allow = FALSE;
                                }

                                resu[i3] = NULL;
                            } //player
                        } //resu
                    } //i3

                    if (playerlist[tpl]->party[0]) {
                        tnpc2 = (npc *) playerlist[tpl]->party[0]->more;
                        txtset(t2, "?");
                        t2->d2[0] = 8;
                        txtadd(t2, "Left Britannia: ");
                        txtadd(t2, tnpc2->name);
                        for (x = 0; x <= playerlist_last; x++) {
                            if (playerlist[x]) {
                                if (playerlist[x]->net != INVALID_NET) {
                                    if (x != tpl) {
                                        NET_send(NETplayer, playerlist[x]->net, t2);
                                    }
                                }
                            }
                        }
                    }
                    for (i = 0; i <= 7; i++) {
                        //remove party members from map
                        if (playerlist[tpl]->party[i]) {
                            VLNKremove(playerlist[tpl]->party[i]);
                            VLNKsremove(playerlist[tpl]->party[i]); //remove VLNK(s)
                            horsedismount(playerlist[tpl]->party[i]);
                            if (HORSEDISMOUNT_HORSEOBJECT) {
                                OBJmove_allow = TRUE;
                                OBJmove2(HORSEDISMOUNT_HORSEOBJECT, DEATHPOSX, DEATHPOSY);
                                OBJmove_allow = FALSE;
                                //ideally horse crt should be deallocated
                            }
                            OBJmove_allow = TRUE;
                            OBJmove2(playerlist[tpl]->party[i], DEATHPOSX, DEATHPOSY);
                            OBJmove_allow = FALSE;
                        }
                    }

                    //check for house sold evict
                    if (houseowner(tplayer, tplayer->GNPCflags[28]) && housesav[tplayer->GNPCflags[28]].gold == 0) {
                        housesav[tplayer->GNPCflags[28]].flags--; //remove house occupied flag
                        housesav_update();
                    }
                    //free NPC items and structures
                    for (i = 0; i <= 7; i++) {
                        x5 = 0;
                        if (playerlist[tpl]->party[i]) {
                            tnpc = (npc *) playerlist[tpl]->party[i]->more;

                            if (i == 0) {
                                txtset(t, "Left_Britannia:");
                                txtadd(t, tnpc->name);
                                LOGadd(t);
                                myobj = playerlist[tpl]->mobj; //*ONLY PARTY[0]
                            } else {
                                myobj = NULL;
                            }
                            if (myobj) x5 = OBJlist(myobj);
                            OBJlist_last = NULL;
                            for (y6 = 0; y6 < x5; y6++) {
                                OBJremove(OBJlist_list[y6]);
                                OBJrelease(OBJlist_list[y6]);
                            }
                            free(tnpc);
                            //remove from object update list
                            if (ouln != -1) {
                                for (i2 = 0; i2 <= ouln; i2++) {
                                    if (playerlist[tpl]->party[i] == oul[i2]) {
                                        oul[i2] = NULL;
                                    }
                                }
                            }
                            OBJremove(playerlist[tpl]->party[i]);
                            OBJrelease(playerlist[tpl]->party[i]);
                        } //->party
                    } //i
                    //free player structure
                    free(playerlist[tpl]);
                    playerlist[tpl] = NULL;
                    inbritanniacheck();

                    goto idle_skip;
                }

            save_update2:
                //add party to fixed update listq
                for (i = 0; i <= 7; i++) {
                    if (playerlist[tpl]->party[i]) {
                        x = playerlist[tpl]->party[i]->x;
                        y = playerlist[tpl]->party[i]->y;
                    addupdateobjs_wizardeye:
                        if (x || y) {
                            //must not be at 0,0, if so it's not on map

                            //tpx=x-15; tpy=y-11;
                            //if (tpx<0) tpx=0; if (tpy<0) tpy=0;
                            //if (tpx>2016) tpx=2016;
                            //if (tpy>1000) tpy=1000;
                            getscreenoffset(x, y, &tpx, &tpy);

                            for (y = -8; y <= (23 + 8); y++) {
                                for (x = -8; x <= (31 + 8); x++) {
                                    if (((x + tpx) < 0) || ((y + tpy) < 0) || ((x + tpx) > 2047) || ((y + tpy) > 1023))
                                        myobj = NULL;
                                    else myobj = od[y + tpy][x + tpx];


                                nextobj2:
                                    if (myobj != NULL) {
                                        if (myobj->type == 416) {
                                            //view redirector
                                            if (myobj->more2) {
                                                myobj = od[myobj->more2 >> 16][myobj->more2 & 65535];
                                                goto nextobj2;
                                            }
                                        }


                                        i3 = 0;
                                        myobj5 = myobj;

                                        //CRASH BANDICOOT
                                        if (myobj5->info & 8) myobj5 = (object *) myobj5->more; //<-redirect


                                        if (myobj5->info & 2) i3 = 1;
                                        if (myobj5->info & 4) i3 = 1;
                                        if (myobj5->type == (223 + 1024)) i3 = 1; //lit powder keg!!!
                                        if (i3 == 1) {
                                            if ((myobj5->info & 32768) == 0) {
                                                //don't add more than once
                                                myobj5->info = myobj5->info | 32768; //object is active
                                                ouln++;
                                                oul[ouln] = myobj5;
                                            }
                                        }

                                        if (myobj->next != NULL) {
                                            myobj = (object *) myobj->next;
                                            goto nextobj2;
                                        }
                                    } //object exists
                                }
                            }
                        } //x||y
                    } //party member OK

                    if (i == 7) {
                        if (playerlist[tpl]->wizardeyetimeleft) {
                            i++;
                            x = playerlist[tpl]->wizardeyex;
                            y = playerlist[tpl]->wizardeyey;
                            goto addupdateobjs_wizardeye;
                        }
                    }
                } //i


                //2005 objects update message
                //assume buffer needs to be resent

                //shift buffer to new location
                /*
        short sobj_bufoffx,sobj_bufoffy; //x,y offset of left corner of entire buffer
        unsigned short sobj_buf2bytes[96][72]; //number of 2 byte elements allocated
        unsigned short sobj_bufsize[96][72]; //number of 2 byte elements actually used
        unsigned short *sobj[96][72]; //scene object raw data
        unsigned long  sobj_tempfixed[96][72]; //temp fixed objects
        */

                static unsigned long bitsi, bitsi2;
                static unsigned short *tp2;
                static unsigned char sceneupdaterequired;
                // ROOMSYNC-P1: cached "is the selected player inside a registered
                // isolated room, and if so what are its bounds" -- replaces the
                // legacy hardcoded gg_basement_room flag. Filled by getroom() once
                // per scene update and consumed by the sobj + mover fill loops to
                // suppress streaming of objects/movers from neighbouring open
                // map. See docs/rendering/global-room-sync.md.
                static unsigned char playerroom_inroom;
                static long playerroom_x0, playerroom_y0, playerroom_x1, playerroom_y1;
                // ROOMSYNC-P1: scratch values for the pre-flush auto-resync check.
                static long roomsync_targetx, roomsync_targety;
                static long roomsync_dx, roomsync_dy;
                // ROOMSYNC-P1.3: DISPLAY tile of the mover currently being added.
                // A type-416 view redirector (the shop interior at x=1280..1341 /
                // y=395..432 is built almost entirely from redirectors pointing back
                // to the overworld building footprint at x~330..376 / y~361..394)
                // overwrites mapx/mapy with the redirect SOURCE tile while the fill
                // loop walks it. Preserve the in-window DISPLAY tile here, exactly
                // like the sobj loop keeps bufx/bufy from before its own redirect.
                static long mv_dispx, mv_dispy;

                if (!tplayer->updatemessage) {
                    txtNEWLEN(t, -1048576); //create 1MB buffer
                    txtNEWLEN(t2, -1048576); //create 1MB buffer
                    bitsi = 0;
                    bitsi2 = 0;
                    sceneupdaterequired = 0;

                    // ROOMSYNC-P1.2: periodic safety-resync heartbeat. Even
                    // with every event-based trigger below firing on
                    // teleport / room cross / camera jump / first scene,
                    // a player who sits completely still in a busy area
                    // (the shop reproducer) can still see slow drift in
                    // their per-player mover slot order over time --
                    // NPC scheduled movement, mover object spawn /
                    // OBJrelease churn, and similar host-side activity
                    // that doesn't show up as a player coord delta.
                    // Once per ROOMSYNC_HEARTBEAT_SECONDS (defined in
                    // define_both.h) force an unconditional resync so
                    // any accumulated drift self-heals on a bounded
                    // schedule. Skip when tplayer->x/y is still zero
                    // (first frame -- the P1.1 first-scene rule handles
                    // that case and we'd otherwise double-resync).
                    if (tplayer->x || tplayer->y) {
                        tplayer->resync_timer += et;
                        // ROOMSYNC-P1.5: inside a registered isolated room a STATIONARY
                        // bystander has no coordinate delta to force a scene-update send,
                        // so it relies entirely on the sobj/mover diff to learn that ANOTHER
                        // player rearranged the ground items. Shorten the safety heartbeat
                        // dramatically while in-room so any missed diff self-heals in a
                        // fraction of a second instead of waiting for an unrelated event
                        // (the other player logging out, a teleport, the 60s heartbeat) --
                        // exactly the reported "two players in the guild basement see items
                        // in the wrong place until someone logs out" bug. The room is tiny
                        // so the packet-35 rebuild is cheap. See define_both.h
                        // ROOMSYNC_ROOM_HEARTBEAT_SECONDS.
                        static float roomsync_hb_interval;
                        roomsync_hb_interval = ROOMSYNC_HEARTBEAT_SECONDS;
                        if (getroom((long)tplayer->x, (long)tplayer->y, 0, 0, 0, 0))
                            roomsync_hb_interval = ROOMSYNC_ROOM_HEARTBEAT_SECONDS;
                        if (tplayer->resync_timer >= roomsync_hb_interval) {
                            tplayer->resync = 1;
                            tplayer->resync_timer = 0.0f;
                        }
                    } else {
                        tplayer->resync_timer = 0.0f;
                    }

                    // ROOMSYNC-P1: AUTO-RESYNC TRIGGER. The historical root cause
                    // of "player disappears / can't move / NPCs flicker" inside
                    // basements was that the host kept its per-player mover and
                    // sobj buffers across discontinuous coordinate jumps (ladder
                    // use, moongate, red gate, partyadd, etc.) and across crosses
                    // of an isolated-room boundary. The diff encoder then walked
                    // pre-jump mover indices against post-jump tile contents and
                    // shipped remove/move/add bits that overwrote the wrong
                    // client-side slots -- famously the avatar's own slot 0,
                    // which then got reassigned to some NPC. The avatar sprite
                    // vanished while the camera kept tracking the never-updated
                    // tplayer->x/y.
                    //
                    // The fix is to detect either condition BEFORE the existing
                    // resync branch flushes buffers, and let that branch run.
                    // No per-teleport code change required: every ladder /
                    // partyadd / spell that moves the avatar > 3 tiles or
                    // crosses a registered room border auto-resyncs now.
                    //
                    // Skip when tplayer->x/y is (0, 0): that's the initial
                    // connect state -- the first scene update is already a
                    // clean build and forcing another resync would waste a
                    // frame. The same myobj selection logic runs again below
                    // when the actual position is emitted; we just need it
                    // early here so the resync decision is made before the
                    // existing tplayer->resync handler clears the buffers.
                    if (myobj = tplayer->party[tplayer->selected_partymember]) {
                        roomsync_targetx = myobj->x;
                        roomsync_targety = myobj->y;
                        if (tplayer->wizardeyetimeleft) {
                            roomsync_targetx = tplayer->wizardeyex;
                            roomsync_targety = tplayer->wizardeyey;
                        }
                    } else {
                        roomsync_targetx = tplayer->px;
                        roomsync_targety = tplayer->py;
                    }
                    // ROOMSYNC-P1.1: first-scene resync. A freshly malloc'd /
                    // ZeroMemory'd player struct (NETconnect path in
                    // loop_host.cpp around L4577) starts with x == y == 0.
                    // The normal diff stream then has to ADD every visible
                    // mover in a single packet from a guaranteed-empty
                    // tplayer->mv_x[] -- on a dense spawn area like the
                    // shop (x=1280..1341 / y=376..432, sitting right against
                    // the gargoyle land x=1280 clamp boundary) this was
                    // observed to leave slot 0 of the per-player mover list
                    // bound to the wrong mover after a cold server start, so
                    // the avatar sprite vanished and input went nowhere.
                    // Subsequent visits to the same area never reproduced
                    // because by then tplayer->x/y were non-zero and the
                    // delta / room / camera-jump triggers below were able
                    // to catch the transition. Forcing the first scene
                    // emit to be a packet-35 (explicit flush + rebuild)
                    // makes both ends start from an unambiguous zero state.
                    if (!tplayer->x && !tplayer->y) {
                        tplayer->resync = 1;
                    }
                    if (tplayer->x || tplayer->y) {
                        roomsync_dx = roomsync_targetx - (long)tplayer->x;
                        roomsync_dy = roomsync_targety - (long)tplayer->y;
                        if (roomsync_dx < -3 || roomsync_dx > 3 ||
                            roomsync_dy < -3 || roomsync_dy > 3) {
                            tplayer->resync = 1; //teleport: force clean rebuild
                        } else if (!sameroom(roomsync_targetx, roomsync_targety,
                                             tplayer->x, tplayer->y)) {
                            tplayer->resync = 1; //room boundary crossed
                        } else {
                            // ROOMSYNC-P1.1: camera-anchor jump. Even when
                            // the player coordinate delta is small (a 1-tile
                            // walk), the camera anchor tpx/tpy returned by
                            // getscreenoffset() can jump by up to ~17 tiles
                            // in a single frame when the player crosses one
                            // of the hardcoded region boundaries in
                            // function_both.cpp -- the gargoyle clamp ends
                            // at x=1279, the very next tile x=1280 falls
                            // into the default fall-through branch which
                            // has no clamp. When tpx shifts that much in
                            // one frame the host's mover/sobj transmit
                            // window also shifts, the per-player 96x72
                            // sobj buffer has to relocate, and the diff
                            // encoder is forced to emit a large batch of
                            // remove+add operations on a single bit-stream.
                            // That batch had the same slot-misalignment
                            // failure mode as a true teleport. Detect the
                            // camera jump and upgrade the implicit partial
                            // rebuild to an explicit full one.
                            static long ar_oldtpx, ar_oldtpy;
                            static long ar_newtpx, ar_newtpy;
                            static long ar_dtpx, ar_dtpy;
                            getscreenoffset((long)tplayer->x, (long)tplayer->y,
                                            &ar_oldtpx, &ar_oldtpy);
                            getscreenoffset(roomsync_targetx, roomsync_targety,
                                            &ar_newtpx, &ar_newtpy);
                            ar_dtpx = ar_newtpx - ar_oldtpx;
                            ar_dtpy = ar_newtpy - ar_oldtpy;
                            if (ar_dtpx < -3 || ar_dtpx > 3 ||
                                ar_dtpy < -3 || ar_dtpy > 3) {
                                tplayer->resync = 1;
                            }
                        }
                    }

                    z = 31;
                    if (tplayer->resync) {
                        //flush object & mover buffers
                        tplayer->x = 0;
                        tplayer->y = 0;
                        tplayer->sobj_bufoffx = 0;
                        tplayer->sobj_bufoffy = 0;
                        tplayer->mv_i = 0;
                        for (y = 0; y <= 71; y++) {
                            for (x = 0; x <= 95; x++) {
                                tplayer->sobj_bufsize[x][y] = 0;
                                tplayer->sobj_tempfixed[x][y] = 0;
                            }
                        }
                        z = 35;
                        tplayer->resync = 0;
                    } //resync
                    BITSadd(t, &bitsi, z, 8); //scene update

                    //txtnumint(t3,t->d2[1]);
                    //MessageBox(NULL,t3->d,"Ultima 6 Online",MB_OK);


                    tplayer = playerlist[tpl];
                    //get player offset
                    if (myobj = tplayer->party[tplayer->selected_partymember]) {
                        x = myobj->x;
                        y = myobj->y;
                        if (tplayer->wizardeyetimeleft) {
                            x = tplayer->wizardeyex;
                            y = tplayer->wizardeyey;
                        }
                    } else {
                        x = tplayer->px;
                        y = tplayer->py; //location where player died
                    }
                    //only send if changed
                    if ((x != tplayer->x) || (y != tplayer->y)) {
                        z = 1;
                        BITSadd(t, &bitsi, z, 1);
                        BITSadd(t, &bitsi, x, 11);
                        BITSadd(t, &bitsi, y, 10);
                        tplayer->x = x;
                        tplayer->y = y;
                        sceneupdaterequired = 1;
                    } else {
                        z = 0;
                        BITSadd(t, &bitsi, z, 1);
                    }


                    //get screen offset
                    //tpx=x-15; tpy=y-11;
                    //if (tpx<0) tpx=0; if (tpy<0) tpy=0;
                    //if (tpx>2016) tpx=2016;
                    //if (tpy>1000) tpy=1000;
                    getscreenoffset(x, y, &tpx, &tpy);
                    // ROOMSYNC-P1: cache the player's room bounds once for
                    // this scene update. The sobj fill loop and the mover
                    // fill loop both consult playerroom_inroom + bounds to
                    // skip tiles that aren't part of the same isolated room
                    // as the player. Replaces the hardcoded
                    // gg_basement_room flag, so a new room added to the
                    // registry in src/common/function_both.cpp gets correct
                    // streaming with no host-side code change.
                    playerroom_inroom = (unsigned char)getroom(x, y,
                        &playerroom_x0, &playerroom_y0,
                        &playerroom_x1, &playerroom_y1);

                    //does screen+1 fit inside current buffer?
                    // RW sobj-fix: screen+1 in legacy frame grew from [tpx-1, tpx+32]
                    // (legacy 32x24 + 1 fence) to [tpx-SOBJ_S1_LEFT, tpx+SOBJ_S1_RIGHT]
                    // (max viewport 63x47 + 1 fence). Same on Y. The 96x72 sobj buffer
                    // still has room. See define_both.h.
                    x3 = tpx - SOBJ_S1_LEFT;
                    y3 = tpy - SOBJ_S1_TOP;
                    x4 = tpx + SOBJ_S1_RIGHT;
                    y4 = tpy + SOBJ_S1_BOTTOM; //screen+1
                    x5 = tplayer->sobj_bufoffx;
                    y5 = tplayer->sobj_bufoffy;
                    x6 = x5 + 96 - 1;
                    y6 = y5 + 72 - 1; //current buffer extents
                    //i. if the screen+1 buffer fits within buffer don't relocate
                    if (x3 >= x5) {
                        if (x4 <= x6) {
                            if (y3 >= y5) {
                                if (y4 <= y6) {
                                    goto screen1buffershiftcomplete;
                                }
                            }
                        }
                    }
                    //ii. clear buffer if screen+1 buffer is entirely outside
                    i = 0;
                    if (x4 < x5) i = 1;
                    if (x3 > x6) i = 1;
                    if (y4 < y5) i = 1;
                    if (y3 > y6) i = 1;
                    if (i) {
                        ZeroMemory(&tplayer->sobj_bufsize, 96 * 72 * 2);
                        ZeroMemory(&tplayer->sobj_tempfixed, 96 * 72 * 4);
                        tplayer->sobj_bufoffx = tpx - 32;
                        tplayer->sobj_bufoffy = tpy - 24;
                        goto screen1buffershiftcomplete;
                    }
                    //iii. relocate screen+1 buffer
                    //calculate new buffer location in x2,y2
                    x2 = tplayer->sobj_bufoffx;
                    if (x2 > x3) x2 = x3;
                    if ((x2 + 95) < x4) x2 = x4 - 95;
                    y2 = tplayer->sobj_bufoffy;
                    if (y2 > y3) y2 = y3;
                    if ((y2 + 71) < y4) y2 = y4 - 71;
                    //modified for loop
                    if (x2 >= tplayer->sobj_bufoffx) {
                        if (y2 >= tplayer->sobj_bufoffy) i = 0;
                        else i = 2;
                    } else {
                        if (y2 >= tplayer->sobj_bufoffy) i = 1;
                        else i = 3;
                    }
                    if (i == 0) {
                        y9 = 0;
                        x9 = 0;
                    }
                    if (i == 1) {
                        y9 = 0;
                        x9 = 95;
                    }
                    if (i == 2) {
                        y9 = 71;
                        x9 = 0;
                    }
                    if (i == 3) {
                        y9 = 71;
                        x9 = 95;
                    }
                sobj_copyloop1:
                    x8 = x2 - tplayer->sobj_bufoffx + x9;
                    y8 = y2 - tplayer->sobj_bufoffy + y9;
                    if (x8 >= 0) {
                        if (x8 <= 95) {
                            if (y8 >= 0) {
                                if (y8 <= 71) {
                                    //copy position
                                    tplayer->sobj_tempfixed[x9][y9] = tplayer->sobj_tempfixed[x8][y8];
                                    i2 = tplayer->sobj_buf2bytes[x9][y9];
                                    tplayer->sobj_buf2bytes[x9][y9] = tplayer->sobj_buf2bytes[x8][y8];
                                    tplayer->sobj_buf2bytes[x8][y8] = i2;
                                    tp2 = tplayer->sobj[x9][y9];
                                    tplayer->sobj[x9][y9] = tplayer->sobj[x8][y8];
                                    tplayer->sobj[x8][y8] = tp2;
                                    tplayer->sobj_bufsize[x9][y9] = tplayer->sobj_bufsize[x8][y8];
                                    tplayer->sobj_bufsize[x8][y8] = 0;
                                    goto sobj_copiedpos1;
                                }
                            }
                        }
                    }
                    //clear position
                    tplayer->sobj_tempfixed[x9][y9] = 0;
                    tplayer->sobj_bufsize[x9][y9] = 0;
                sobj_copiedpos1:;
                    if (i == 0) {
                        x9++;
                        if (x9 >= 96) {
                            y9++;
                            x9 = 0;
                        }
                        if (y9 < 72) goto sobj_copyloop1;
                    }
                    if (i == 1) {
                        x9--;
                        if (x9 < 0) {
                            y9++;
                            x9 = 95;
                        }
                        if (y9 < 72) goto sobj_copyloop1;
                    }
                    if (i == 2) {
                        x9++;
                        if (x9 >= 96) {
                            y9--;
                            x9 = 0;
                        }
                        if (y9 >= 0) goto sobj_copyloop1;
                    }
                    if (i == 3) {
                        x9--;
                        if (x9 < 0) {
                            y9--;
                            x9 = 95;
                        }
                        if (y9 >= 0) goto sobj_copyloop1;
                    }
                    tplayer->sobj_bufoffx = x2;
                    tplayer->sobj_bufoffy = y2;
                screen1buffershiftcomplete:
                    //is screen+1 of buffer and visual area EXACTLY THE SAME?
                    static unsigned char screenplus1check;
                    screenplus1check = 1;
                    goto screenplus1check_entry;
                screenplus1check_return:
                    screenplus1check = 0;
                    //screen+1 was NOT exactly the same, so perform a screen+8 update
                    sceneupdaterequired = 1;
                    z = 1;
                    BITSadd(t, &bitsi, z, 1); //screen+8 update required


                    // RW sobj-fix: screen+8 in legacy frame grew from [tpx-8, tpx+39]
                    // to [tpx-SOBJ_TX_OFFX, tpx+SOBJ_TX_W-1-SOBJ_TX_OFFX]. SOBJ_TX_OFFX/Y
                    // double as the screen+8 fence by construction. See define_both.h.
                    x3 = tpx - SOBJ_TX_OFFX;
                    y3 = tpy - SOBJ_TX_OFFY;
                    x4 = tpx + SOBJ_TX_W - 1 - SOBJ_TX_OFFX;
                    y4 = tpy + SOBJ_TX_H - 1 - SOBJ_TX_OFFY;
                    x5 = tplayer->sobj_bufoffx;
                    y5 = tplayer->sobj_bufoffy;
                    x6 = x5 + 96 - 1;
                    y6 = y5 + 72 - 1; //current buffer extents
                    //i. if the screen+8 buffer fits within buffer don't relocate
                    if (x3 >= x5) {
                        if (x4 <= x6) {
                            if (y3 >= y5) {
                                if (y4 <= y6) {
                                    goto bufferrelocatecomplete0;
                                }
                            }
                        }
                    }
                    //ii. clear buffer if screen+8 buffer is entirely outside
                    i = 0;
                    if (x4 < x5) i = 1;
                    if (x3 > x6) i = 1;
                    if (y4 < y5) i = 1;
                    if (y3 > y6) i = 1;
                    if (i) {
                        ZeroMemory(&tplayer->sobj_bufsize, 96 * 72 * 2);
                        ZeroMemory(&tplayer->sobj_tempfixed, 96 * 72 * 4);
                        tplayer->sobj_bufoffx = tpx - 32;
                        tplayer->sobj_bufoffy = tpy - 24;
                        //MessageBox(NULL,"shiftbuffer: entire buffer cleared","Ultima 6 Online",MB_OK);
                        goto bufferrelocatecomplete0;
                    }
                    //iii. relocate buffer
                    //calculate new buffer location in x2,y2
                    x2 = tplayer->sobj_bufoffx;
                    if (x2 > x3) x2 = x3;
                    if ((x2 + 95) < x4) x2 = x4 - 95;
                    y2 = tplayer->sobj_bufoffy;
                    if (y2 > y3) y2 = y3;
                    if ((y2 + 71) < y4) y2 = y4 - 71;
                    //modified for loop
                    if (x2 >= tplayer->sobj_bufoffx) {
                        if (y2 >= tplayer->sobj_bufoffy) i = 0;
                        else i = 2;
                    } else {
                        if (y2 >= tplayer->sobj_bufoffy) i = 1;
                        else i = 3;
                    }
                    if (i == 0) {
                        y9 = 0;
                        x9 = 0;
                    }
                    if (i == 1) {
                        y9 = 0;
                        x9 = 95;
                    }
                    if (i == 2) {
                        y9 = 71;
                        x9 = 0;
                    }
                    if (i == 3) {
                        y9 = 71;
                        x9 = 95;
                    }
