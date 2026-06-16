// LHS-P4.8: part_b_leave_resurrect_house -- CONTINUES + CLOSES MEGA B. WIRE.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch and
// CLOSES it (} //read client message), then folds in the interB block
// (addsave*, tempcheck, respawn_*). Contents: leave-britannia (leaveitems),
// resurrect (resurrect_success), buyhouse (buyhouse_failed), ship attack
// (kshipattack), doneclmess, + interB. Move-only; no U6O_VERSION bump.
                            leaveitems:
                                if (myobj3 = (object *) tnpc->items->more) {
                                    OBJremove(myobj3);
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                    goto leaveitems;
                                }
                                if (myobj3 = tnpc->helm) {
                                    tnpc->helm = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->neck) {
                                    tnpc->neck = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->wep_left) {
                                    if (myobj3->type == (90 + 1024)) myobj3->type = 90;
                                    tnpc->wep_left = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->wep_right) {
                                    if (myobj3->type == (90 + 1024)) myobj3->type = 90;
                                    tnpc->wep_right = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->armour) {
                                    tnpc->armour = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->ring_left) {
                                    tnpc->ring_left = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->ring_right) {
                                    tnpc->ring_right = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }
                                if (myobj3 = tnpc->boots) {
                                    tnpc->boots = NULL;
                                    OBJadd(tplayer->party[x2]->x, tplayer->party[x2]->y, myobj3);
                                }

                                tnpc->baseitem = tnpc->items;
                                tnpc->baseitem_offset = 0;

                                OBJmove_allow = TRUE;
                                OBJmove2(tplayer->party[x2], tplayer->party[x2]->x, tplayer->party[x2]->y + 1);
                                OBJmove2(tplayer->party[x2], tplayer->party[x2]->x, tplayer->party[x2]->y - 1);
                                OBJmove_allow = FALSE;

                                //send message stating the NPC has died/left
                                txtset(t5, "??");
                                t5->d2[0] = 252;
                                t5->d2[1] = x2;
                                NET_send(NETplayer, tplayer->net, t5);
                                if ((tnpc->converse) && (tnpc->converse == 201)) {
                                    //not core npc
                                    tplayer->party[x2]->info |= 112; //set as temp item
                                }
                                tplayer->party[x2] = NULL;
                                tplayer->selected_partymember = 0;

                                tnpc->player = NULL;
                                tnpc->order = 2; //follow schedule
                                tnpc->follow = NULL;

                                goto leavefinished;
                            } //==tnpc
                        } //->party[x2]
                    } //x2

                leavefinished:
                    x++;
                    goto nextinstruction;
                }


                if (npcbin[x].l1 == 20) {
                    //cure!
                    if (CONpartymember == 0) {
                        for (x2 = 0; x2 <= 7; x2++) {
                            if (tplayer->party[x2]) {
                                tnpc3 = (npc *) tplayer->party[x2]->more;
                                tnpc3->flags &= (65535 - 1); //remove poisoned flag
                                tnpc3->upflags |= 2;
                            }
                        } //x2
                    } else {
                        //!=0
                        tnpc3 = (npc *) tplayer->party[CONpartymember - 1]->more;
                        if ((tnpc3->flags & 1) == 0) CONerr = 1;
                        tnpc3->flags &= (65535 - 1); //remove poisoned flag
                        tnpc3->upflags |= 2;
                    }
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 21) {
                    //heal!
                    if (CONpartymember == 0) {
                        for (x2 = 0; x2 <= 7; x2++) {
                            if (tplayer->party[x2]) {
                                tnpc3 = (npc *) tplayer->party[x2]->more;
                                tnpc3->hp = tnpc3->hp_max;
                                tnpc3->mp = tnpc3->mp_max;
                                tnpc3->upflags |= (2 + 4);
                            }
                        } //x2
                    } else {
                        //!=0
                        tnpc3 = (npc *) tplayer->party[CONpartymember - 1]->more;
                        if (tnpc3->hp == tnpc3->hp_max) {
                            if (tnpc3->mp == tnpc3->mp_max) {
                                CONerr = 1; //ERR_ALREADY
                            }
                        }
                        tnpc3->hp = tnpc3->hp_max;
                        tnpc3->mp = tnpc3->mp_max;
                        tnpc3->upflags |= (2 + 4);
                    }
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 23) {
                    //ifparty
                    txtNEWLEN(t2, npcbin[x].l3);
                    memcpy(t2->d2, &npcinf[npcbin[x].l2], npcbin[x].l3);
                    for (x2 = 1; x2 <= 7; x2++) {
                        if (tplayer->party[x2]) {
                            tnpc3 = (npc *) tplayer->party[x2]->more;
                            txtset(t3, tnpc3->name);
                            txtlcase(t3);
                            if (txtsame(t2, t3)) {
                                x++;
                                goto nextinstruction;
                            } //txtsame
                        }
                    } //x2
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }


                if (npcbin[x].l1 == 24) {
                    //levelup!
                    CONerr = 1;
                    if (CONpartymember) {
                        if (tplayer->party[CONpartymember - 1]) {
                            //valid value?
                            tnpc3 = (npc *) tplayer->party[CONpartymember - 1]->more;
                            //entitled to levelup?
                            //x3=(800<<(tnpc3->lev-1)); //required xp to level
                            // c111 new required xp to level
                            if (easymodehostn1) {
                                x3 = new1_getexprequired(tnpc3);
                            } else {
                                // original
                                x3 = (800 << (tnpc3->lev - 1)); //required xp to level
                            }
                            if (tnpc3->exp >= x3) {
                                //levelup
                                tnpc3->lev++;
                                tnpc3->upflags |= 1;
                                tnpc3->hp_max = tnpc3->lev * 30;
                                tnpc3->s += npcbin[x].l2;
                                tnpc3->d += npcbin[x].l3;
                                tnpc3->i += npcbin[x].l4;
                                tnpc3->wt_max = tnpc3->s * 4 * 100;
                                tnpc3->wt2_max = tnpc3->s * 50;
                                if (CONpartymember == 1) {
                                    tnpc3->mp_max = tnpc3->i * 2;
                                } else {
                                    tnpc3->mp_max = get_mp_max(tnpc3->i);
                                    //tnpc3->wt_max/=2;//halve wt_max for non-primary partymember
                                    tnpc3->wt_max = tnpc3->s * 3 * 100;
                                }
                                CONerr = 0;
                                inbritanniacheck();
                            } //x3
                        }
                    }
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 25) {
                    //rest!
                    if (tplayer->rest == 0) tplayer->rest = 32;
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 26) {
                    //resurrect!
                    //scan inventory of all players for bodies & crossreference these with ressurect lists
                    CONerr = 1;
                    for (x6 = 0; x6 <= 7; x6++) {
                        if (tplayer->party[x6]) {
                            tnpc3 = (npc *) tplayer->party[x6]->more;
                            x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                        }
                    } //!=NULL,x6
                    OBJlist_last = NULL;
                    for (y6 = 0; y6 < x5; y6++) {
                        if ((OBJlist_list[y6]->type & 1023) == 339) {
                            //dead body
                            CONerr = 2;
                            //scan resurrect list
                            for (i3 = 0; i3 <= nresu; i3++) {
                                if (resu[i3]) {
                                    if (resu_body[i3] == OBJlist_list[y6]) {
                                        /*
                    object *resu[65536]; //object
                    object *resu_body[65536]; //dead body object
                    float resu_wait[65536]; //time until object automatically resurrected
                    player *resu_player[65536]; //player object belongs to
                    unsigned char resu_partymember[65536]; //party member index
                    long nresu=-1;
                    */
                                        if (resu_player[i3]) {
                                            //has a player pointer

                                            ////resu_player[i3]->party[0]=resu[i3]; //assume current player
                                            resu_player[i3]->party[resu_partymember[i3]] = resu[i3];

                                            //tnpc2=(npc*)resu[i3]->more; tnpc2->exp-=int(tnpc2->exp/4);

                                            if (resu_partymember[i3] == 0) {
                                                resu_player[i3]->GNPCflags[25] = 0; //used insurance!

                                                for (x4 = 1; x4 <= 7; x4++) {
                                                    if (resu_player[i3]->party[x4]) {
                                                        tnpc3 = (npc *) resu_player[i3]->party[x4]->more;
                                                        tnpc3->upflags = 1;
                                                        OBJmove_allow = TRUE;
                                                        OBJmove2(resu_player[i3]->party[x4],
                                                                 tplayer->party[tplayer->selected_partymember]->x,
                                                                 tplayer->party[tplayer->selected_partymember]->y);
                                                        //place party on the map
                                                        OBJmove_allow = FALSE;
                                                    }
                                                } //x4
                                            }

                                            OBJmove_allow = TRUE;
                                            OBJmove2(resu[i3], tplayer->party[tplayer->selected_partymember]->x,
                                                     tplayer->party[tplayer->selected_partymember]->y);
                                            //place avatar on the map
                                            OBJmove_allow = FALSE;

                                            OBJremove(OBJlist_list[y6]); //remove body
                                            tnpc3 = (npc *) resu[i3]->more;
                                            resu[i3] = NULL;

                                            if (resu_partymember[i3] == 0) {
                                                txtset(t5, "?");
                                                t5->d2[0] = 8;
                                                txtadd(t5, "A voice in the darkness intones, \x022");
                                                txtadd(t5, "DOMAN THIXUS ANRETU!\x022");
                                                NET_send(NETplayer, resu_player[i3]->net, t5);
                                                txtset(t5, "?");
                                                t5->d2[0] = 8;
                                                txtadd(t5, tnpc3->name);
                                                txtadd(t5, " resurrected by healer.");
                                                for (x2 = 0; x2 <= playerlist_last; x2++) {
                                                    //create and send UPDATE message
                                                    if (playerlist[x2]) {
                                                        //current player
                                                        if (playerlist[x2]->net != NULL) {
                                                            //net connection available
                                                            if (playerlist[x2]->party[0]) {
                                                                if (playerlist[x2] != resu_player[i3]) {
                                                                    NET_send(NETplayer, playerlist[x2]->net, t5);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            inbritanniacheck();
                                            CONerr = 0;
                                            goto resurrect_success;
                                        } //player
                                    } //resu_body==obj
                                } //resu
                            } //i
                        } //339
                    }

                resurrect_success:


                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 28) {
                    //ifnpc
                    for (y2 = myobj->y - 8; y2 <= (myobj->y + 8); y2++) {
                        for (x2 = myobj->x - 8; x2 <= (myobj->x + 8); x2++) {
                            if (myobj3 = OBJfindlast(x2, y2)) {
                                if (myobj3->info & 2) {
                                    tnpc3 = (npc *) myobj3->more;
                                    if (tnpc3->converse == npcbin[x].l2) {
                                        x++;
                                        goto nextinstruction;
                                    }
                                } //2
                            } //myobj3
                        }
                    } //x,y
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 29) {
                    //buyhouse!
                    if (tplayer->party[0]) {
                        tnpc3 = (npc *) tplayer->party[0]->more;
                        //if= ERROR,1/txt He rustles through his papers.\"You must be mistaken, I'd know if that house was available."
                        //if= ERROR,2/txt "$G, Lord British allows only one house per person."
                        //if= ERROR,3/txt "$G, perhaps you should spend more time exploring Britannia before deciding where to live."
                        x2 = CON_gv(npcbin[x].l2); //housenumber
                        //already owns house?
                        for (x3 = 1; x3 <= 255; x3++) {
                            if (houseowner(tplayer, x3)) {
                                CONerr = 2;
                                goto buyhouse_failed;
                            }
                        }
                        if (housesav[x2].flags & 1) {
                            CONerr = 1;
                            goto buyhouse_failed;
                        } //already owned?
                        // r555 debug: player requirement check for buying house
                        if (tnpc3->lev <= 2) {
                            CONerr = 3;
                            goto buyhouse_failed;
                        } //inexperienced?
                        //set house username & charactername 32-byte strings
                        txtNEWLEN(t3, -32);
                        for (x3 = 0; x3 < 32; x3++) {
                            t3->d2[x3] = 0;
                            if (tplayer->name->l > x3) t3->d2[x3] = tplayer->name->d2[x3];
                        }
                        txtNEWLEN(t4, -32);
                        for (x3 = 0; x3 < 32; x3++) {
                            t4->d2[x3] = 0;
                            if (tnpc3->name->l > x3) t4->d2[x3] = tnpc3->name->d2[x3];
                        }
                        //set house.sav buffer
                        housesav[x2].flags |= 1;
                        housesav[x2].gold = housecost[x2] * 3; //3 days
                        for (x3 = 0; x3 < 32; x3++) {
                            housesav[x2].username[x3] = t3->d2[x3];
                            housesav[x2].charactername[x3] = t4->d2[x3];
                        }
                        housesav_update(); //save house.sav buffer to file
                        //give key (256+housenumber)
                        myobj3 = OBJnew();
                        myobj3->type = 64;
                        myobj3->info = 256;
                        myobj3->more2 = 256 + x2;
                        myobj4 = tnpc3->items;
                        myobj5 = (object *) myobj4->more;
                        if (myobj5) {
                            myobj3->next = myobj5;
                            myobj5->prev = myobj3;
                            myobj3->prev = myobj4;
                            myobj4->more = myobj3;
                        } else {
                            myobj3->next = NULL;
                            myobj3->prev = myobj4;
                            myobj4->more = myobj3;
                        }
                        tnpc3->upflags |= 32; //inv
                        tplayer->GNPCflags[28] = x2; //set housenumber
                    } //party[0]
                buyhouse_failed:
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 30) {
                    //economysetup!
                    x2 = CON_gv(npcbin[x].l2); //index
                    x3 = CON_gv(npcbin[x].l3); //initial amount
                    x4 = CON_gv(npcbin[x].l4); //change per Britannian day (+10000 means negative)
                    if (economy_setup[CONnpc][x2] == FALSE) {
                        economy_setup[CONnpc][x2] = TRUE;
                        economy_value[CONnpc][x2] = x3;
                        economy_limit[CONnpc][x2] = x3 * 2; //DEFAULT LIMIT IS DOUBLE INIT. VALUE
                        economy_change[CONnpc][x2] = x4;
                        //adjust value based on hours of already elapsed gameplay
                        d = ett / 3600.0f; //d=days in Brit.(realtime hours)
                        x3 = rnd * 100.0f;
                        d2 = ((float) x3) / 100.0f * 3.0f + 1.0f; //1-4 (average of 2.5)
                        d /= d2; //account for exponential factor
                        x3 = d;
                        x3 *= economy_change[CONnpc][x2];
                        economy_value[CONnpc][x2] += x3;
                        if (economy_value[CONnpc][x2] < 0) economy_value[CONnpc][x2] = 0;
                        if (economy_value[CONnpc][x2] > economy_limit[CONnpc][x2])
                            economy_value[CONnpc][x2] = economy_limit[CONnpc][x2];
                    }
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 31) {
                    //economygetvalue!
                    x2 = CON_gv(npcbin[x].l2); //index
                    CONreg[0] = economy_value[CONnpc][x2];
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 32) {
                    //economysetvalue!
                    x2 = CON_gv(npcbin[x].l2); //index
                    x3 = CON_gv(npcbin[x].l3); //new value
                    economy_value[CONnpc][x2] = x3;
                    if (economy_value[CONnpc][x2] < 0) economy_value[CONnpc][x2] = 0;
                    if (economy_value[CONnpc][x2] > economy_limit[CONnpc][x2])
                        economy_value[CONnpc][x2] = economy_limit[CONnpc][x2];
                    x++;
                    goto nextinstruction;
                }

                // Typo? economysetlimit should be instruction #33 per NPC2BIN4.BAS, Xenkan 2010-12-04
                //if (npcbin[x].l1==32){//economysetlimit!
                if (npcbin[x].l1 == 33) {
                    //economysetlimit!
                    x2 = CON_gv(npcbin[x].l2); //index
                    x3 = CON_gv(npcbin[x].l3); //new limit
                    economy_limit[CONnpc][x2] = x3;
                    x++;
                    goto nextinstruction;
                }

#pragma region
                if (npcbin[x].l1 == 34) // bulkreset
                {
                    memset(tplayer->bulktypes, 0, sizeof(tplayer->bulktypes));
                    memset(tplayer->bulkqty, 0, sizeof(tplayer->bulkqty));
                    tplayer->bulkprice = 0;
                    x++;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 35) // bulktype type,price
                {
                    uint2 type, price, qty;
                    type = npcbin[x].l2;
                    price = npcbin[x].l3;
                    qty = 0;

                    // search party inventories for item type
                    object **Inv;
                    uint4 InvCount;
                    tplayer->GetPartyInventory(&Inv, &InvCount);
                    for (int i = 0; i < InvCount; i++) {
                        if (Inv[i]->type != type ||
                            Inv[i]->IsStealing() ||
                            Inv[i]->IsNotForSale())
                            continue;

                        if (Inv[i]->IsStackable())
                            qty += Inv[i]->GetQuantity();
                        else
                            qty++;
                    }
                    if (qty == 0) {
                        x++;
                        goto nextinstruction;
                    }

                    // find next available sell slot
                    int slot;
                    for (slot = 0; tplayer->bulktypes[slot] != 0; slot++);

                    // add this item type to sell list and price
                    tplayer->bulktypes[slot] = type;
                    tplayer->bulkqty[slot] = qty;
                    tplayer->bulkprice += qty * price;

                    x++;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 36) // bulksell
                {
                    // verify player still has all items included in quote
                    object **Inv;
                    uint4 InvCount;
                    uint2 type, qty;
                    int i, j;
                    tplayer->GetPartyInventory(&Inv, &InvCount);
                    for (i = 0; tplayer->bulkqty[i]; i++) {
                        type = tplayer->bulktypes[i];
                        qty = 0;
                        for (j = 0; j < InvCount; j++) {
                            if (Inv[j]->type != type ||
                                Inv[j]->IsStealing() ||
                                Inv[j]->IsNotForSale())
                                continue;

                            if (Inv[j]->IsStackable())
                                qty += Inv[j]->GetQuantity();
                            else
                                qty++;
                        }
                        if (qty < tplayer->bulkqty[i]) {
                            CONerr = 1; // player dropped item
                            x++;
                            goto nextinstruction;
                        }
                    }

                    // remove items
                    for (i = 0; tplayer->bulkqty[i]; i++) {
                        type = tplayer->bulktypes[i];
                        qty = tplayer->bulkqty[i];
                        for (j = 0; j < InvCount && qty; j++) {
                            if (Inv[j]->type != type ||
                                Inv[j]->IsStealing() ||
                                Inv[j]->IsNotForSale())
                                continue;

                            if (Inv[j]->IsStackable()) {
                                if (Inv[j]->GetQuantity() > qty) {
                                    Inv[j]->SetQuantity(Inv[j]->GetQuantity() - qty);
                                    qty = 0;
                                } else {
                                    qty -= Inv[j]->GetQuantity();
                                    OBJremove(Inv[j]);
                                    OBJrelease(Inv[j]);
                                }
                            } else // not stackable
                            {
                                qty--;
                                OBJremove(Inv[j]);
                                OBJrelease(Inv[j]);
                            }
                        }
                    }

                    // give gold
                    object *gold;
                    gold = OBJnew();
                    gold->type = OBJ_GOLD_COIN;
                    gold->SetQuantity(tplayer->bulkprice);
                    additemroot((npc *) tplayer->party[0]->more, gold);

                    // update party's inventory
                    npc *n;
                    for (int p = 0; p < 8 && tplayer->party[p]; p++) {
                        n = (npc *) tplayer->party[p]->more;
                        n->upflags |= 32; // update inventory
                    }

                    x++;
                    goto nextinstruction;
                }
#pragma endregion Bulk sell patch, Xenkan 2010-12-04

                //more instructions here
            } //myobj!=NULL
        talk_skipnullobject:;
            txtset(t2, t);
            //txtset(t,"NPCmessage ["); txtadd(t,t2); txtadd(t,"] ("); txtnumint(t2,ett); txtadd(t,t2); txtadd(t,")"); txtfileout(t,log2);
            goto doneclmess;
        } //7


        //theory is after receiving a message from host's client we can't reply in the same turn


        //txtset(t2,"?"); t2->d2[0]=10; //type 10: U6OID info
        //NET_send(NETplayer,NETplayer,t2);


        if (t->d2[0] == 9) {
            //type 9: request U6OID info


            txtright(t, t->l - 1);
            tu6oid = t->dl[0];


            //whose id is that anyway!!
            for (x = 0; x <= playerlist_last; x++) {
                if (playerlist[x]) {
                    //current
                    if (playerlist[x]->id == tu6oid) {
                        txtset(t2, "?");
                        t2->d2[0] = 10; //type 10: U6OID info
                        txtadd(t2, t); //U6OID
                        tnpc = (npc *) playerlist[x]->party[0]->more;
                        txtset(t3, "?");
                        t3->d2[0] = tnpc->name->l;
                        txtadd(t2, t3); //->l
                        txtadd(t2, tnpc->name);
                        //colour
                        txtaddchar(t2, playerlist[x]->GNPCflags[245] & 255);
                        txtaddchar(t2, (playerlist[x]->GNPCflags[245] >> 8) & 255);
                        txtaddchar(t2, (playerlist[x]->GNPCflags[245] >> 16) & 255);
                        NET_send(NETplayer, playerlist[tpl]->net, t2);
                        goto doneclmess;
                    } //id
                } //current
            } //x
            goto doneclmess;
        } //9


        if (t->d2[0] == 13) {
            //->quan
            txtright(t, 2);
            playerlist[tpl]->quan = t->ds[0];
            goto doneclmess;
        } //13


        if (t->d2[0] == 15) {
            //keyboard targeting request
            //scan screen from center to outer edges
            //as each enemy is found check if they are on our list of 10
            //if ktar_wait==0 slot is empty and ready to be filled
            //remember to remove any old virtual links before assigning a new one
            if (playerlist[tpl]->ktar_display < 1.0f) {
                if (myobj = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]) {
                    for (i = 1; i <= 767; i++) {
                        x = myobj->x + ktar_x[i];
                        y = myobj->y + ktar_y[i];
                        myobj2 = OBJfindlast(x, y);
                        if (myobj2 == NULL) myobj2 = OBJfindlastall(x, y);
                        if (myobj2) {
                            if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //redirect
                            if (myobj2->info & 4) {
                                //<-crt
                                crt = (creature *) myobj2->more;
                                for (i2 = 0; i2 <= 9; i2++) {
                                    if (playerlist[tpl]->ktar_wait[i2]) {
                                        if ((object *) playerlist[tpl]->ktar[i2]->more == myobj2) goto ktar_gottarget;
                                    }
                                }

                                //add target if room exists
                                for (i2 = 0; i2 <= 9; i2++) {
                                    if (!playerlist[tpl]->ktar_wait[i2]) {
                                        VLNKremove(playerlist[tpl]->ktar[i2]);
                                        VLNKsremove(playerlist[tpl]->ktar[i2]);
                                        playerlist[tpl]->ktar[i2]->more = myobj2;
                                        VLNKnew(playerlist[tpl]->ktar[i2], myobj2,
                                                (unsigned long) &playerlist[tpl]->ktar[i2]->more);
                                        playerlist[tpl]->ktar_wait[i2] = 64;
                                        goto ktar_gottarget;
                                    }
                                }
                            }
                        }
                    ktar_gottarget:;
                    } //i
                } //->party[
            } //1.0f
            playerlist[tpl]->ktar_display = 2.0f;
            goto doneclmess;
        } //15

        if (t->d2[0] == 16) {
            //keyboard target
            x = t->d2[1];
            if (playerlist[tpl]->ktar_wait[x]) {
                if (playerlist[tpl]->ktar[x]->more) {
                    //set as target
                    if (playerlist[tpl]->craft) {
                        if ((playerlist[tpl]->party[0]->type & 1023) == 412) {
                            //ship
                            goto kshipattack;
                        }
                    }
                    if (playerlist[tpl]->craft == NULL) {
                    kshipattack:
                        myobj = (object *) playerlist[tpl]->ktar[x]->more;
                        crt = (creature *) myobj->more;
                        if (crt->flags & 2) goto doneclmess;
                        for (x2 = 0; x2 <= 7; x2++) {
                            if (playerlist[tpl]->party[x2]) {
                                myobj2 = playerlist[tpl]->party[x2];
                                tnpc = (npc *) myobj2->more;
                                tnpc->order = 3;
                                tnpc->target = myobj;
                                VLNKsremove(myobj2); //remove prev vlnk
                                VLNKnew(myobj2, myobj, (unsigned long) &tnpc->target); //add vlnk
                                if (playerlist[tpl]->craft) goto kshipattack2; //exit loop after processing party[0]
                            }
                        }
                    }
                kshipattack2:;
                }
            }
            goto doneclmess;
        } //16

        if (t->d2[0] == 17) {
            //break off combat
            for (x2 = 0; x2 <= 7; x2++) {
                if (playerlist[tpl]->party[x2]) {
                    VLNKsremove(playerlist[tpl]->party[x2]);
                    tnpc2 = (npc *) playerlist[tpl]->party[x2]->more;
                    tnpc2->target = NULL;
                    tnpc2->order = 3;
                    if (x2 == playerlist[tpl]->selected_partymember) tnpc2->order = 0;
                } //->party
            } //x2
            goto doneclmess;
        } //17

        if (t->d2[0] == 19) {
            //typing...
            playerlist[tpl]->typing = 1;
            goto doneclmess;
        } //19

        if (t->d2[0] == 20) {
            //typing... CANCEL
            playerlist[tpl]->typing = 0;
            goto doneclmess;
        } //20


        //send pathfinding message
        if (t->d2[0] == 21) {
            //player pathfind
            if (myobj = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]) {
                tnpc = (npc *) myobj->more;
                txtright(t, t->l - 1);
                x2 = t->ds[0];
                y2 = t->ds[1];
                // RW: extend pathfind range so a right-click anywhere in the
                // resizable client's rendered view actually finds a path. The
                // legacy cap of 16 tiles is exactly half the original 32x24
                // view, so it silently rejected clicks near the edges once the
                // client viewport grew (kViewportTilesXMax=63, YMax=47 in
                // src/client/viewport.h => avatar can be ~32 tiles from a
                // clicked tile in the worst case). 48 covers the full max view
                // with margin; wpf_weight[512][512] easily holds z=2*48+2=98.
                WPF_OBJECT = myobj;
                wpf_pathfind(tnpc->path, myobj->x, myobj->y, x2, y2, 48, 0, 0);
                if (WPF_RETURN == WPF_PATHFOUND) {
                    if (WPF_PATHLENGTH) {
                        tnpc->path_max = WPF_PATHLENGTH;
                        tnpc->pathn = WPF_PATHLENGTH;
                        playerlist[tpl]->pathfind = 1; //use pathfinding formula!
                        goto doneclmess;
                    }
                    playerlist[tpl]->pathfind = 0;
                    tnpc->path_max = 0;
                    tnpc->pathn = 0;
                }
            } //party
            goto doneclmess;
        } //21


        if (t->d2[0] == 25) {
            //play MIDI note(s)
            //broadcast instrument and notes to all in-range players instantly
            txtright(t, t->l - 1);
            txtset(t2, "??");
            t2->d2[0] = 27;
            t2->d2[1] = playerlist[tpl]->instrument;
            txtadd(t2, t);
            if (myobj = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]) {
                for (x = 0; x <= playerlist_last; x++) {
                    if (playerlist[x]) {
                        //current
                        if (playerlist[x]->net != INVALID_NET) {
                            if (myobj2 = playerlist[x]->party[playerlist[x]->selected_partymember]) {
                                //proximity check
                                if (abs(myobj2->x - myobj->x) <= 16) {
                                    if (abs(myobj2->y - myobj->y) <= 12) {
                                        NET_send(NETplayer, playerlist[x]->net, t2);
                                    }
                                }
                            } //myobj2
                        }
                    }
                } //x valid
            } //myobj
            goto doneclmess;
        } //25

        if (t->d2[0] == 26) {
            //stop MIDI note(s)
            //broadcast instrument and notes to all in-range players instantly
            txtright(t, t->l - 1);
            txtset(t2, "??");
            t2->d2[0] = 28;
            t2->d2[1] = playerlist[tpl]->instrument;
            txtadd(t2, t);
            if (myobj = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]) {
                for (x = 0; x <= playerlist_last; x++) {
                    if (playerlist[x]) {
                        //current
                        if (playerlist[x]->net != INVALID_NET) {
                            if (myobj2 = playerlist[x]->party[playerlist[x]->selected_partymember]) {
                                //proximity check
                                if (abs(myobj2->x - myobj->x) <= 16) {
                                    if (abs(myobj2->y - myobj->y) <= 12) {
                                        NET_send(NETplayer, playerlist[x]->net, t2);
                                    }
                                }
                            } //myobj2
                        }
                    }
                } //x valid
            } //myobj
            goto doneclmess;
        } //26

        /*
    txtset(t,"?"); t->d2[0]=25; //play MIDI keys
    txtset(t2,"?");
    //multiple keys can point to a single sound
    for (i=0;i<=255;i++){
    if (keyhit(i)){
    if (t->l<9){
    t2->d2[0]=midikeyboard2[i];
    txtadd(t,t2);
    midikeyboard2_keyon[i]=1;
    }
    }//keyhit
    }//i
    if (t->l>1){
    NET_send(NETplayer,NULL,t);
    }

    txtset(t,"?"); t->d2[0]=26; //stop MIDI keys
    txtset(t2,"?");
    for (i=0;i<=255;i++){
    if (keyon[i]==NULL){
    if (midikeyboard2_keyon[i]){
    t2->d2[0]=midikeyboard2[i]; txtadd(t,t2);
    midikeyboard2_keyon[i]=0;
    }}}
    if (t->l>1){
    NET_send(NETplayer,NULL,t);
    }
    */

        //if (playerlist[tpl]->net==1){
        //goto U6Ohostlink1;
        //U6Ohostlink1return:;
        //}

    doneclmess:

        goto nextclientmessage;
    } //read client message
} //host


if
(NEThost) {
    //host schedule info
    f = 16.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        u6orevive++;
        if (u6orevive == 200) u6orevive = 0;
        seek(u6orevive_fh, 0);
        put(u6orevive_fh, &u6orevive, 1);
    }

    u6opi = 40;

    f = 64.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        inbritanniacheck();
    }

    u6opi = 41;

    //host log report
    f = 60.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        //every minute
        txtset(t, "[In_Britania:");
        txtadd(t, inbritannia);
        txtadd(t, "]");
        txtnumint(t2, framerate);
        txtadd(t, "[Framerate:");
        txtadd(t, t2);
        txtadd(t, "]");
        txtnumint(t2, vlnkb_last);
        txtadd(t, "[vlnkb_last(MAX=65536):");
        txtadd(t, t2);
        txtadd(t, "]");
        txtnumint(t2, objb_last);
        txtadd(t, "[objb_last(MAX=524228):");
        txtadd(t, t2);
        txtadd(t, "]");
        LOGadd(t);
    }


    //if (ett>=86400){

    /*
  f=4.0f; if (int((ett/f))!=int((ett/f)-(et/f))){ //8min delay till removal
  txtset(t,"!");
  txtadd(t," vlnkb_last="); txtnumint(t2,vlnkb_last); txtadd(t,t2);
  txtadd(t," vlnkb_free_last="); txtnumint(t2,vlnkb_free_last); txtadd(t,t2);
  txtadd(t," framerate="); txtnumint(t2,framerate); txtadd(t,t2);
  txtadd(t," ("); txtnumint(t2,ett); txtadd(t,t2); txtadd(t,")"); txtfileout(t,log2);
  }
  */


    /*
  long objsave_last=-1;
  unsigned short objsave_x[65536];
  unsigned short objsave_y[65536];
  object *objsave_obj[65536]; //pointer to first saved object
  float objsave_wait[65536];
  */
    u6opi = 42;

    //add saved objects
    for (i = 0; i <= objsave_last; i++) {
        if (objsave_obj[i]) {
            objsave_wait[i] -= et;
            if (objsave_wait[i] <= 0) {
                //1st pass: destroy ALL temp objects (except deeds and selected critical items)
                myobj3 = NULL; //add saved objects after this object
                if (myobj = od[objsave_y[i]][objsave_x[i]]) {
                addsaveclear:
                    myobj2 = (object *) myobj->next;
                    if ((myobj->type & 1023) == 414) goto addsaveclearskip; //skiff
                    if ((myobj->type & 1023) == 149) goto addsaveclearskip; //deed
                    //add more here
                    if (myobj->info & 112) {
                        //temp object
                        OBJremove(myobj);
                    } else {
                        //object wasn't removed!
                        if (objfloatflags[(myobj->type >> 10) + sprlnk[myobj->type & 1023]] == NULL) {
                            if (obji[sprlnk[myobj->type & 1023]].v4 == NULL) {
                                //not crt/npc
                                myobj3 = myobj; //set as base item if NOT floating
                            } //not crt/npc
                        } //float==NULL
                    } //info&112

                addsaveclearskip:
                    if (myobj2) {
                        myobj = myobj2;
                        goto addsaveclear;
                    }
                }

                u6opi = 421;


                if (myobj3) {
                    //temp set to end of stack
                    myobj4 = (object *) myobj3->next;
                    myobj3->next = NULL;
                } else {
                    myobj4 = od[objsave_y[i]][objsave_x[i]];
                    od[objsave_y[i]][objsave_x[i]] = NULL;
                }

                //add objects
                myobj = objsave_obj[i];


            addsaveobj:
                myobj2 = (object *) myobj->next;

                myobj->next = NULL;
                myobj->prev = NULL;
                OBJadd(objsave_x[i], objsave_y[i], myobj);

                //multi-tile container FIX
                x = 0;
                y = 0;
                if (myobj->type == (176 + 1024)) x = -1;
                if (myobj->type == (176 + 3 * 1024)) y = -1;
                if (myobj->type == (168 + 1024)) x = -1;
                if (myobj->type == (168 + 3 * 1024)) y = -1;
                if (myobj->type == (341 + 3 * 1024)) {
                    y = -1;
                    x = -1;
                }
                if (myobj->type == (340 + 3 * 1024)) {
                    y = -1;
                    x = -1;
                }
                if (x || y) {
                addsave_mtcfix2:
                    if (myobj5 = od[myobj->y + y][myobj->x + x]) {
                    addsave_mtcfix:
                        if ((myobj5->type & 1023) == (myobj->type & 1023)) {
                            if (myobj5->info & 8) {
                                myobj5->more = (object *) myobj;
                            } //info&8
                        } //type&1023
                        if (myobj5 = (object *) myobj5->next) goto addsave_mtcfix;
                    } //myobj5
                    if ((x == -1) && (y == -1) && (myobj->type == (341 + 3 * 1024))) {
                        y = -1;
                        x = 0;
                        goto addsave_mtcfix2;
                    }
                    if ((x == 0) && (y == -1) && (myobj->type == (341 + 3 * 1024))) {
                        y = 0;
                        x = -1;
                        goto addsave_mtcfix2;
                    }
                    if ((x == -1) && (y == -1) && (myobj->type == (340 + 3 * 1024))) {
                        y = -1;
                        x = 0;
                        goto addsave_mtcfix2;
                    }
                    if ((x == 0) && (y == -1) && (myobj->type == (340 + 3 * 1024))) {
                        y = 0;
                        x = -1;
                        goto addsave_mtcfix2;
                    }
                } //x||y

                if (myobj2) {
                    myobj = myobj2;
                    goto addsaveobj;
                }
                if (myobj4) {
                    myobj->next = myobj4;
                    myobj4->prev = myobj;
                }

                OBJcheckflags(objsave_x[i], objsave_y[i]);
                objsave_obj[i] = NULL; //invalidate
            } //wait<=0
        } //objsave_obj
    } //i

    u6opi = 43;
    //remove temporary objects
    f = 64.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        //8min delay till removal
        //f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){ //8min delay till removal

        i = 2048 * 1024;
        while (i--) {
            if (myobj = od[objr_y][objr_x]) {
            tempcheck:
                myobj2 = (object *) myobj->next;


                //skip SPECIAL temp objects
                for (i2 = 0; i2 <= nresu; i2++) {
                    if (resu[i2]) {
                        //if in resurrect list DO NOT remove!
                        if (resu_body[i2] == myobj) goto skiptempobj;
                    }
                }
                if ((myobj->type & 1023) == 412) goto skiptempobj; //ship
                if ((myobj->type & 1023) == 414) goto skiptempobj; //skiff
                if ((myobj->type & 1023) == 149) goto skiptempobj; //deed
                if (myobj->type == OBJ_HORSE_PAPERS) goto skiptempobj; //horse papers
                if (obji[sprlnk[myobj->type & 1023]].flags & 1024) {
                    //container
                    if ((myobj->type & 1023) != OBJ_DEAD_CYCLOPS) {
                        if ((myobj->type & 1023) != OBJ_DEAD_GARGOYLE) {
                            if ((myobj->type & 1023) != OBJ_DEAD_REAPER) {
                                if ((myobj->type & 1023) != OBJ_HORSE_CARCASS) {
                                    //carcass
                                    if (obji[sprlnk[myobj->type & 1023]].weight == NULL) goto skiptempobj;
                                    //NULL weight container
                                }
                            }
                        }
                    }
                }

                //house storage exception
                for (i3 = 1; i3 <= 255; i3++) {
                    for (i4 = 0; i4<housestoragenext[i3]; i4++) {
                        if (myobj->x == housestoragex[i3][i4]) {
                            if (myobj->y == housestoragey[i3][i4]) {
                                goto skiptempobj;
                            }
                        } //x,y
                    } //i4
                } //i3


                //-1/remove object
                i2 = (myobj->info >> 4) & 7;
                if (i2) {
                    i2--;
                    myobj->info &= (32767 - 112);
                    myobj->info += (i2 << 4);
                    if (i2 == 0) {
                        if (myobj->info & 2) {
                            //NPC
                            tnpc = (npc *) myobj->more;
                            free(tnpc);
                        }
                        OBJremove(myobj);
                        OBJrelease(myobj);
                    }
                }

            skiptempobj:
                if (myobj2) {
                    myobj = myobj2;
                    goto tempcheck;
                }
            }
            objr_x++;
            if (objr_x > 2047) {
                objr_x = 0;
                objr_y++;
            }
            if (objr_y > 1023) objr_y = 0;
        }
    }

    u6opi = 44;
    //respawn info
    //respawn_last++;
    //respawn[respawn_last]=myobj2;
    //respawn_delay[respawn_last]=16;
    //fruit tart, dog lead?, dog home?
    /*
  void *respawn[16384];
  unsigned short respawn_delay[16384]; //number of seconds till creature will respawn
  //*note: creature will not respawn if player is too near (eg. 8 squares or less)
  long respawn_last=-1;
  */


    f = 1.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        for (i = 0; i <= respawn_last; i++) {
            if (respawn[i]) {
                respawn_delay[i]--;
                if (respawn_delay[i] == 0) {
                    crt = (creature *) respawn[i]->more;

                    u6opi = 441;
                    //player nearby?

                    x = crt->respawn_x;
                    y = crt->respawn_y; //GPF

                    z = 12;
                    if ((x >= 1024) && (!((y >= 256) && (x < 1280) && (y < 512)))) z = 8;
                    //not on overland map or garglands
                    for (i2 = 0; i2 <= playerlist_last; i2++) {
                        if (playerlist[i2]) {
                            //current player
                            if (playerlist[i2]->net != INVALID_NET) {
                                //net connection available
                                if (playerlist[i2]->party[0]) {
                                    //if abs(playerlist[i2]->party[0]->x-crt->respawn_x)<=
                                    x2 = playerlist[i2]->party[0]->x;
                                    y2 = playerlist[i2]->party[0]->y;
                                    x2 = abs(x - x2);
                                    y2 = abs(y - y2);
                                    if ((x2 <= z) || (y2 <= z)) {
                                        respawn_delay[i] = 512; //~8mins
                                        goto respawn_failed;
                                    } //x2<=z,y2<=z
                                }
                            }
                        } //net
                    } //i2
                    u6opi = 442;
                    if (crt->flags & 32) {
                        //revert charmed creature!
                        crt->al = 1;
                        crt->charm = 0;
                    }

                    //revert passive->aggresive
                    if ((respawn[i]->type & 1023) == 382) {
                        //guard
                        if (crt->respawn_x < 1024) crt->al = 4; //not underground
                    }
                    if ((respawn[i]->type & 1023) == 373) crt->al = 0; //wisp
                    if ((respawn[i]->type & 1023) == 430) crt->al = 0; //horse
                    if ((respawn[i]->type & 1023) == 428) crt->al = 0; //cow
                    if ((respawn[i]->type & 1023) == 356) crt->al = 0; //bird
                    if ((respawn[i]->type & 1023) == 348) crt->al = 0; //sheep
                    if ((respawn[i]->type & 1023) == 350) crt->al = 0; //deer
                    if ((respawn[i]->type & 1023) == 388) {
                        //cat
                        if ((crt->respawn_x >= 400) && (crt->respawn_y >= 576) && (crt->respawn_x <= 415) && (
                                crt->respawn_y <= 591))
                            crt->al = 0; //Dr Cat's cats
                    }
                    if ((respawn[i]->type & 1023) == 352) {
                        //ghost
                        if ((crt->respawn_x == 139) && (crt->respawn_y == 196)) crt->al = 0; //abby ghosts
                    }

                    crt->hp = crt->respawn_hp;
                    crt->mp = rnd * 9;
                    crt->flags = 0;
                    myobj2 = respawn[i];


                    //2006 monster drops
                    static object *invbag, *invobj, *xpgoldobj;
                    invbag = crt->items;
                    if (crt->al == 1) {
                        //aggressive
                        addu6monsterdropitems(myobj2);

                        //integrate XP based gold
                        x9 = rnd * 8;
                        if (x9 == 0) {
                            x9 = obji[sprlnk[myobj2->type & 1023]].v5;
                            x9 = rnd * (x9 + 1);
                            if (x9) {
                                if (xpgoldobj = (object *) invbag->more) {
                                respawn_goldscan:
                                    if (xpgoldobj->type == OBJ_GOLD_COIN) {
                                        xpgoldobj->more2 += x9;
                                        goto respawn_goldadded;
                                    }
                                    xpgoldobj = (object *) xpgoldobj->next;
                                    if (xpgoldobj) goto respawn_goldscan;
                                }
                                invobj = OBJnew();
                                invobj->type = 88;
                                invobj->more2 = x9;
                                invobj->info |= 112;
                                invobj->next = invbag->more;
                                invobj->prev = invbag;
                                invbag->more = invobj;
                                if (invobj->next) ((object *) invobj->next)->prev = invobj;
                            }
                        }
                    respawn_goldadded:

                        goto respawn_u6inventoryadded;
                    } //al==1
                    if (((myobj2->type & 1023) == 428) || ((myobj2->type & 1023) == 348) || (
                            (myobj2->type & 1023) == 350)) {
                        //cow/sheep/deer
                        x9 = rnd * 2;
                        if (x9) {
                            invobj = OBJnew();
                            invobj->type = 129;
                            invobj->more2 = 1; //add meat
                            invobj->info |= 112;
                            invobj->next = invbag->more;
                            invobj->prev = invbag;
                            invbag->more = invobj;
                            if (invobj->next) ((object *) invobj->next)->prev = invobj;
                        } //x9
                    } //cow/sheep/deer
                respawn_u6inventoryadded:
                    /*
          //add gold/items to inventory
          //add gold
          if (crt->al==1){
          x9=obji[sprlnk[myobj2->type&1023]].v5>>2;
          //x9++; //plus 1 for rookie luck
          x9=rnd*(x9+1);
          if (x9){
          myobj8=OBJnew(); myobj8->type=88; myobj8->more2=x9;
          //items<-gold
          myobj6=crt->items; myobj7=(object*)myobj6->more;
          if (myobj7!=NULL){
          myobj8->next=myobj7; myobj7->prev=myobj8; myobj8->prev=myobj6; myobj6->more=myobj8;
          }else{
          myobj8->next=NULL; myobj8->prev=myobj6; myobj6->more=myobj8;
          }
          }//x9
          }//al==1
          //add meat
          if (((myobj2->type&1023)==428)||((myobj2->type&1023)==348)||((myobj2->type&1023)==350)){//cow&sheep&deer
          x9=rnd*2; if (x9){
          myobj8=OBJnew(); myobj8->type=129; myobj8->more2=1; //<-meat
          myobj6=crt->items; myobj7=(object*)myobj6->more;
          if (myobj7!=NULL){
          myobj8->next=myobj7; myobj7->prev=myobj8; myobj8->prev=myobj6; myobj6->more=myobj8;
          }else{
          myobj8->next=NULL; myobj8->prev=myobj6; myobj6->more=myobj8;
          }
          }//x9
          }//cow
          */

                    if ((myobj2->type & 1023) == 375) myobj2->info |= (3 << 9); //slime divide!

                    OBJmove_allow = TRUE;
                    OBJmove2(respawn[i], crt->respawn_x, crt->respawn_y);
                    //silver serpent "recoil"
                    if ((respawn[i]->type & 1023) == 413) {
                        OBJmove2(respawn[i], crt->respawn_x, crt->respawn_y + 1);
                        OBJmove2(respawn[i], crt->respawn_x + 1, crt->respawn_y + 1);
                        OBJmove2(respawn[i], crt->respawn_x + 1, crt->respawn_y);
                        OBJmove2(respawn[i], crt->respawn_x, crt->respawn_y);
                        OBJmove2(respawn[i], crt->respawn_x, crt->respawn_y + 1);
                        OBJmove2(respawn[i], crt->respawn_x + 1, crt->respawn_y + 1);
                        OBJmove2(respawn[i], crt->respawn_x + 1, crt->respawn_y);
                        OBJmove2(respawn[i], crt->respawn_x, crt->respawn_y);
                    }
                    OBJmove_allow = FALSE;

                    crt->target = NULL;
                    respawn[i] = NULL;

                respawn_failed:;
                } //delay
            } //respawn[i]
        } //i
    } //f
