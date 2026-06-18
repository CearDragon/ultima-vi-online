// =============================================================================
// LHS-P2.2: part_tail_npcframe -- CONTINUES + CLOSES the host per-tick block.
// LHS brace-seam: CONTINUES the if(NEThost){ block opened in
// loop_host_part_open_housekeeping.cpp and CLOSES it (trailing }//NEThost).
// MUST stay LAST in the umbrella. Contents: NPC-frame UPDATE for(tpl..) loop
// (mover/mobj encode -- WIRE), oul cleanup, the #ifndef _DEBUG 24h auto-quit,
// autoexit_deny:, and the closing brace. WIRE-COUPLED: move-only, no
// U6O_VERSION bump.
// =============================================================================

    //2003: send NPC-frame update message to client
    for (tpl = 0; tpl <= playerlist_last; tpl++) {
        if (playerlist[tpl]) {
            //current player
            if (playerlist[tpl]->net != NULL) {
                //net connection exists


                //2003: mobj update (this should be incorporated into the NPC update message if possible)
                if (playerlist[tpl]->mobj != playerlist[tpl]->mobj_last) {
                    txtset(t, "?");
                    txtset(t2, "??");
                    t->d2[0] = 4;
                    if (playerlist[tpl]->mobj != NULL) {
                        t2->ds[0] = playerlist[tpl]->mobj->type;
                    } else { t2->ds[0] = 0xFFFF; }
                    txtadd(t, t2);
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                }
                playerlist[tpl]->mobj_last = playerlist[tpl]->mobj;


                for (i = 0; i <= 7; i++) {
                    if ((playerlist[tpl]->party[i] != NULL) && ((playerlist[tpl]->party[0] != NULL))) {
                        tnpc = (npc *) playerlist[tpl]->party[i]->more; //shortcut
                        if (tnpc->upflags) {
                            //upflags (any)
                            //1		name, portriat, str, dex, int, lev, hp_max, mp_max (all)
                            //2		hp
                            //4     mp
                            //8     xp
                            //16    weight (automatically set if inventory/equipped items change)
                            //32    inventory, bag icon
                            //64    equipped items

                            //tnpc->upflags=1;
                            if (tnpc->upflags & 1) tnpc->upflags |= (2 + 4 + 8 + 16 + 32 + 64);
                            if (tnpc->upflags & 32) tnpc->upflags |= 16;
                            if (tnpc->upflags & 64) tnpc->upflags |= 16;

                            if (tnpc->upflags & 32) {
                                //reagents changed? spellbook active?
                                if (tnpc->wep_right) {
                                    if (tnpc->wep_right->type == 57) {
                                        tnpc->upflags |= 128;
                                    }
                                }
                                if (tnpc->wep_left) {
                                    if (tnpc->wep_left->type == 57) {
                                        tnpc->upflags |= 128;
                                    }
                                }
                            }
                            //WHAT ABOUT WHEN WE TAKE THE SPELLBOOK AWAY!!!!!


                            //if (tplayer->mobj->type==57){ //ready spellbook
                            //tnpc->upflags|=128;
                            //}
                            //tnpc->wep_right=tplayer->mobj;


                            if (tnpc->upflags & 16) {
                                //recalculate the weight the player is carrying
                                x = 0;
                                x += WTfind((object *) tnpc->helm);
                                x += WTfind((object *) tnpc->neck);
                                x += WTfind((object *) tnpc->wep_left);
                                x += WTfind((object *) tnpc->wep_right);
                                x += WTfind((object *) tnpc->armour);
                                x += WTfind((object *) tnpc->ring_left);
                                x += WTfind((object *) tnpc->ring_right);
                                x += WTfind((object *) tnpc->boots);
                                tnpc->wt2 = x;
                                x += (WTfind((object *) tnpc->items) - 20);
                                tnpc->wt = x;
                            }


                            //need an internal loop for each NPC
                            txtset(t, "????");
                            txtset(t2, "??");
                            txtset(t3, "?");
                            txtset(t4, "????");
                            txtset(t6, "?");
                            t->d2[0] = 3; //update NPC frame message
                            t->d2[1] = i; //frame index 0-7
                            t->ds[1] = tnpc->upflags;


                            if (tnpc->upflags & 32) {
                                //validate inventory position (scroll up as necessary)
                            revalinv:
                                x = -1;
                                myobj = (object *) tnpc->baseitem;
                                myobj = (object *) myobj->more;
                            invval:
                                if (myobj) {
                                    x++;
                                    myobj = (object *) myobj->next;
                                    goto invval;
                                }
                                if (x == -1) x = 0;
                                if (x < tnpc->baseitem_offset) {
                                    if (tnpc->baseitem_offset >= 4) { tnpc->baseitem_offset -= 4; } else {
                                        tnpc->baseitem_offset = 0;
                                    }
                                    goto revalinv;
                                }

                                t3->d2[0] = 0;
                                txtadd(t, t3); //x2 <--
                                //x2(64)   +display up arrow (only if flag32 set)
                                //x2(128)   +display down arrow (only if flag32 set)
                                //x2(?)  +bag icon (only if flag32 set)

                                txtsetchar(t5, 46); //tnpc non-sellable objects message (currently seperate)
                                txtaddchar(t5, i);
                                txtaddshort(t5, 0);
                                y3 = 0;

                                //11111 111
                                //tnpc=tnpc;
                                //player inventory
                                myobj = (object *) tnpc->baseitem;
                                myobj = (object *) myobj->more;
                                x2 = 0; //number of items to display
                                x = 61;
                                y = 61;
                                x3 = tnpc->baseitem_offset;
                                if (x3) x2 += 64; //up arrow
                            dnib:
                                if (myobj != NULL) {
                                    if (x3 > 0) {
                                        x3--;
                                        goto skipbio1b;
                                    }


                                    t2->ds[0] = myobj->type;

                                    if (myobj->info & 32768) {
                                        t5->ds[1] |= (1 << y3);
                                    }
                                    y3++;

                                    txtadd(t, t2);
                                    //quantity,keys and item + can't exist in same object so changed to else if this should fix oild flask bug also
                                    i6 = 0;
                                    if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].flags & 4096) {
                                        //items that have quantity
                                        t2->ds[0] = myobj->more2;
                                        txtadd(t, t2);
                                    } else if ((myobj->type & 1023) == 64) {
                                        //key
                                        t2->ds[0] = myobj->more2;
                                        txtadd(t, t2);
                                    } else if (obji[sprlnk[myobj->type & 1023] + (myobj->type >> 10)].v1 || obji[
                                                   sprlnk[myobj->type & 1023] + (myobj->type >> 10)].v2) {
                                        //v1 (damage), send + info to client
                                        i6 = ENHANCEget_attack(myobj);
                                        if (i6 == 0) {
                                            i6 = ENHANCEget_defense(myobj);
                                        }
                                        t6->d2[0] = i6;
                                        txtadd(t, t6);
                                    } else if ((myobj->type & 1023) == 448) {
                                        //horse papers, send + info to client 
                                        myobj2 = (object *) myobj->more;
                                        crt2 = (creature *) myobj2->more;
                                        if (crt2->respawn_x & 15) {
                                            i6 = crt2->respawn_x & 15;
                                        }
                                        t6->d2[0] = i6;
                                        txtadd(t, t6);
                                    }

                                    x2++;
                                    x = x + 16;
                                    if (x > 109) {
                                        x = 61;
                                        y = y + 16;
                                    }
                                    if (y > 109) {
                                        if (myobj->next != NULL) x2 += 128;
                                        goto diskipb;
                                    }
                                    if (myobj->next != NULL) {
                                    skipbio1b:
                                        myobj = (object *) myobj->next;
                                        goto dnib;
                                    }
                                }
                            diskipb:;
                                //return (x2&0xF);
                                if (tnpc->baseitem != tnpc->items) {
                                    t2->ds[0] = tnpc->baseitem->type;
                                    txtadd(t, t2);
                                    x2 += 32; //bag item
                                }
                                t->d2[4] = x2; //<--x2

                                NET_send(NETplayer, playerlist[tpl]->net, t5); //send not4sale info
                            } //tnpc->upflags&32

                            if (tnpc->upflags & 64) {
                                //eqp items update
                                if (tnpc->helm != NULL) t2->ds[0] = tnpc->helm->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->wep_right != NULL) t2->ds[0] = tnpc->wep_right->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->wep_left != NULL) t2->ds[0] = tnpc->wep_left->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->armour != NULL) t2->ds[0] = tnpc->armour->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->boots != NULL) t2->ds[0] = tnpc->boots->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->neck != NULL) t2->ds[0] = tnpc->neck->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->ring_right != NULL) t2->ds[0] = tnpc->ring_right->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                if (tnpc->ring_left != NULL) t2->ds[0] = tnpc->ring_left->type;
                                else t2->ds[0] = 0;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->wt2;
                                txtadd(t, t2);
                                //as equipment has changed recalculate arm value!
                                static unsigned short npc_arm;
                                npc_arm = 0;
                                myobj = tnpc->helm;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->wep_right;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->wep_left;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->armour;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->boots;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->neck;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->ring_right;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                myobj = tnpc->ring_left;
                                if (myobj != NULL)
                                    npc_arm += obji[sprlnk[myobj->type & 1023]].v2 + ENHANCEget_defense(
                                        myobj);
                                tnpc->arm = npc_arm;
                                if (tnpc->protection) tnpc->arm += 10;
                                if (tnpc->player) {
                                    if (tnpc->player->party[0]) {
                                        if ((npc *) tnpc->player->party[0]->more == tnpc) {
                                            if (tnpc->exp < 1600) {
                                                if (tnpc->lev <= 2) {
                                                    tnpc->arm += (20 - tnpc->exp / 80);
                                                }
                                            }
                                        }
                                    }
                                }
                            } //tnpc->upflags&64

                            if (tnpc->upflags & 1) {
                                //1		name, portriat, str, dex, int, lev, hp_max, mp_max (does not change often)
                                t3->d2[0] = tnpc->name->l;
                                txtadd(t, t3);
                                txtadd(t, tnpc->name);
                                t2->ds[0] = tnpc->port;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->s;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->d;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->i;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->lev;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->hp_max;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->mp_max;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->wt_max;
                                txtadd(t, t2);
                                t2->ds[0] = tnpc->wt2_max;
                                txtadd(t, t2);
                            } //tnpc->upflags&1

                            if (tnpc->upflags & 16) {
                                //wt
                                t2->ds[0] = tnpc->wt;
                                txtadd(t, t2);
                            } //tnpc->upflags&16

                            if (tnpc->upflags & 2) {
                                //hp
                                if (tnpc->hp < 0) tnpc->hp = 0;
                                if (tnpc->flags & 1) t2->ds[0] = (unsigned long) tnpc->hp | 32768;
                                else t2->ds[0] = tnpc->hp;
                                txtadd(t, t2);
                            } //tnpc->upflags&2

                            if (tnpc->upflags & 4) {
                                //mp
                                t2->ds[0] = tnpc->mp;
                                txtadd(t, t2);
                            } //tnpc->upflags&4

                            if (tnpc->upflags & 8) {
                                //xp
                                t4->dl[0] = tnpc->exp;
                                txtadd(t, t4);
                            } //tnpc->upflags&8


                            if (tnpc->upflags & 128) {
                                ZeroMemory(&tspell, sizeof(tspell));
                                //enumerate spells
                                if (myobj = (object *) tnpc->wep_left) {
                                    if (myobj->type == 57) {
                                        myobj = (object *) myobj->more;
                                    nextspell:
                                        if (myobj) {
                                            tspell[myobj->more2] = 1;
                                            if (myobj->next) {
                                                myobj = (object *) myobj->next;
                                                goto nextspell;
                                            }
                                        }
                                    }
                                }
                                if (myobj = (object *) tnpc->wep_right) {
                                    if (myobj->type == 57) {
                                        myobj = (object *) myobj->more;
                                    nextspell2:
                                        if (myobj) {
                                            //NEW GPF THIS LINE!
                                            tspell[myobj->more2] = 1;

                                            if (myobj->next) {
                                                myobj = (object *) myobj->next;
                                                goto nextspell2;
                                            }
                                        }
                                    }
                                }

                                // c222 lastused spellbook always available (even after unreadying)
                                if (easymodehostn1) {
                                    if (myobj = (object *) playerspellbook) {
                                        myobj = (object *) myobj->more;
                                        while (myobj) {
                                            tspell[myobj->more2] = 1;
                                            myobj = (object *) myobj->next;
                                            //if (myobj->next) {myobj=(object*)myobj->next; goto nextspell2;}
                                        }
                                    }
                                }


                                //enumerate reagents
                                ZeroMemory(&treagent, sizeof(treagent));
                                x = OBJlist((object *) tnpc->items->more); //x5=last object number???
                                OBJlist_last = NULL;
                                for (x2 = 0; x2 < x; x2++) {
                                    x3 = OBJlist_list[x2]->type;
                                    if ((x3 >= 65) && (x3 <= 72)) {
                                        treagent[x3 - 65] += OBJlist_list[x2]->more2;
                                    }
                                }

                                //add spell number
                                //spellreagent[..]
                                x4 = 0; //number of spells in spellbook(s)
                                for (x = 0; x <= 255; x++) {
                                    if (tspell[x]) {
                                        x4++;

                                        x3 = 65536;
                                        for (x2 = 0; x2 <= 7; x2++) {
                                            if (spellreagent[x] & (1 << x2)) {
                                                if (treagent[x2] < x3) x3 = treagent[x2];
                                            } //reagent
                                        } //x2

                                        if (x3 < 65536) {
                                            //add spell number
                                            tspell[x] += x3;
                                        }
                                    } //tspell
                                } //x spell

                                //add message
                                t3->d2[0] = x4;
                                txtadd(t, t3); //numbers of spells
                                for (x = 0; x <= 255; x++) {
                                    if (tspell[x]) {
                                        //txtnumint(t5,x);
                                        //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


                                        t3->d2[0] = x;
                                        txtadd(t, t3);
                                        t2->ds[0] = tspell[x];
                                        txtadd(t, t2);
                                    }
                                }
                            } //128

                            NET_send(NETplayer, playerlist[tpl]->net, t); //send update message
                            tnpc->upflags = 0;
                        } //upflags
                    }
                } //party member loop
            } //connection exists
        } //current player
    } //tpl


    if (ouln != -1) {
        for (i = 0; i <= ouln; i++) {
            if (oul[i]) {
                if (oul[i]->info & 32768) oul[i]->info -= 32768;
            }
        }
    }

#ifndef _DEBUG
    /* this was originally at client loop so I moved it here */
    if (ett >= 86400) {
        /*quit every 24h if not debugging*/
        for (i = 1; i <= playerlist_last; i++) {
            if (playerlist[i]) {
                //current player
                if (playerlist[i]->net != INVALID_NET) {
                    //net connection available
                    goto autoexit_deny;
                }
            }
        }
        u6orevive = 255;
        seek(u6orevive_fh, 0);
        put(u6orevive_fh, &u6orevive, 1);
        exitrequest = TRUE;
        exitrequest_noconfirm = TRUE;
    } //ett
#endif
autoexit_deny:

    revive_infiniteloopexit_i++;
} //NEThost!=NULL
