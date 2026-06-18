// LHS-P4.4: part_b_housestore_restore -- CONTINUES MEGA B. WIRE.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch.
// Contents: house-storage restore (housestoragerestore0/1/2).
    housestoragerestore0:

        //create objects (being careful of object->next & object->prev)
        x = 0; //first item in set
        x2 = -1; //index
    sv2nextitem:
        if (t4->dl[0] != 0) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];

            x2++;
            sv2o[x2] = myobj2;


            if (housestoragerestore == 0) {
                if ((x2 < (sv2op[9] - 1)) || (sv2op[9] == 0)) stealing(playerlist[tpl], myobj2);
                //mobj won't be classed as being stolen
            }

            myobj2->more2 = t4->dl[1];


            //if (housestoragerestore){
            //txtnumint(t5,myobj2->type); txtadd(t5,":type"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
            //txtnumint(t5,myobj2->more2); txtadd(t5,":more2"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
            //}


            //txtnumint(t5,myobj2->type&1023);
            //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

            //GPF
            if (x) {
                //link it to prev item
                myobj2->prev = sv2o[x2 - 1];
                sv2o[x2 - 1]->next = myobj2;
                //txtset(t5,"linked to prev item"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
            }


            myobj3 = myobj2;


            if (myobj3->type == 448) {
                //horse papers (load)
                /*
        myobj2=(object*)OBJlist_list[y6]->more;
        if (myobj2->x){//horse is not being ridden
        t2->dl[0]=myobj2->x+myobj2->y*2048;//use as few bits as possible(21 bits total)
        }else{//horse is being ridden by a partymember
        t2->dl[0]=0;
        //scan to detect which partymember
        for(z3=0;z3<7;z3++){
        if (myobj3=playerlist[tpl]->party[z3]){
        tnpc3=(npc*)myobj3->more;
        if (tnpc3->horse==myobj2){
        t2->dl[0]=z3;
        }//tnpc3->horse==myobj2
        }//party[z3]
        }//z3
        }//myobj2->x
        */
                z2 = myobj3->more2 & 2097151;

                //2097151=111111111111111111111b (21bits in binary)
                //static long hc=0;
                //hc++;
                //if (hc==4) exit(z2);
                //if (z2!=903451) if (z2!=0) exit(z2);

                //t->d2[t->l-1]&=225;//11100001b
                //t->d2[t->l-1]+=(z4<<1);//npc#
                //((myobj3->info>>9)&15)

                //oldsave patch
                if (z2 <= 7) {
                    npchorsepapers[z2] = myobj3;
                    goto oldsave_horse;
                }

                if (((myobj3->info >> 9) & 15) == 8) {
                    //create horse and set deed to point to horse
                    movernew(430, z2 & 2047, z2 >> 11, MOVERNEW_FLAG_NORESPAWN);
                    ((creature *) MOVERNEW_OBJECT->more)->flags |= 128;
                    ((creature *) MOVERNEW_OBJECT->more)->al = 0;
                    ((creature *) MOVERNEW_OBJECT->more)->respawn_y = tpl;
                    ((creature *) MOVERNEW_OBJECT->more)->respawn_x = (myobj3->more2 >> 21) & 255;

                    OBJmove_allow = TRUE;
                    if (((myobj3->more2 >> 29) & 3) == 1) {
                        OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x - 1, MOVERNEW_OBJECT->y);
                        OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x + 1, MOVERNEW_OBJECT->y);
                    }
                    if (((myobj3->more2 >> 29) & 3) == 2) {
                        OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x, MOVERNEW_OBJECT->y - 1);
                        OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x, MOVERNEW_OBJECT->y + 1);
                    }
                    if (((myobj3->more2 >> 29) & 3) == 3) {
                        OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x + 1, MOVERNEW_OBJECT->y);
                        OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x - 1, MOVERNEW_OBJECT->y);
                    }
                    OBJmove_allow = FALSE;

                    myobj3->more = MOVERNEW_OBJECT;

                    //If horse is inside a house evict it
                    x7 = MOVERNEW_OBJECT->x;
                    y7 = MOVERNEW_OBJECT->y;
                    x6 = housecheck(x7, y7);
                    if (x6) {
                        if (houseowner(playerlist[tpl], x6)) goto houseboundrycheck_horse0;
                        //move horse to entrance
                        OBJmove_allow = TRUE;
                        OBJmove2(MOVERNEW_OBJECT, houseentrancex[x6], houseentrancey[x6]);
                        OBJmove_allow = FALSE;
                        goto houseboundrycheck_horse0;
                    }
                houseboundrycheck_horse0:;


                    //remove horse rear end!
                    //OBJremove((object*)((creature*)MOVERNEW_OBJECT->more)->more);
                } else {
                    //set horse as being ridden
                    //t->d2[t->l-1]&=225;//11100001b
                    //t->d2[t->l-1]+=(z4<<1);//npc#
                    //((myobj3->info>>9)&15)
                    if (npchorsepapers[((myobj3->info >> 9) & 15)]) exit(576);
                    npchorsepapers[((myobj3->info >> 9) & 15)] = myobj3;
                }
            } //horse papers (load)
        oldsave_horse:

            if ((myobj3->type & 1023) == 414) {
                //skiff
                iskiffx++;
                iskiff[iskiffx] = myobj3;
            }

            if (myobj3->type == 149) {
                //deed

                //ok lets add this ship if it's not current ship
                if (myobj3->more2 == NULL) {
                    //inventory deed!
                    ideedx++;
                    ideed[ideedx] = myobj3;
                    goto ideed_done;
                }

                if (myobj3->more2 != 0xFFFFFFFF) {
                    //create ship/skiff!
                    if (myobj3->info & (1 << 9)) {
                        //skiff
                        myobj5 = OBJnew();
                        myobj5->type = 414;
                        OBJadd(myobj3->more2 & 65535, myobj3->more2 >> 16, myobj5);
                        myobj3->more = (object *) myobj5;
                    } else {
                        //ship

                        myobj5 = OBJnew();
                        myobj5->type = 412 + 9 * 1024;
                        OBJadd(myobj3->more2 & 65535, myobj3->more2 >> 16, myobj5);
                        static mlobj *mmyobj; //array size varies
                        mmyobj = (mlobj *) malloc(sizeof(object *) * 5);
                        myobj5->more = mmyobj;
                        myobj = OBJnew();
                        mmyobj->obj[0] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[1] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[2] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[3] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[4] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj3->more = myobj5; //deed->ship

                        OBJmove_allow = TRUE;
                        OBJmove2(myobj5, myobj5->x, myobj5->y + 1);
                        OBJmove2(myobj5, myobj5->x, myobj5->y - 1);
                        OBJmove_allow = FALSE;
                        myobj3->more = (object *) myobj5;
                    }
                } else {
                    myobj6 = myobj3;
                } //set as current deed
            } //149
        ideed_done:


            x = 1;

            txtright(t4, t4->l - 8); //remove item
            goto sv2nextitem;
        } else {
            //==0
            //txtset(t5,"break"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

            if (t4->dl[1] != 0) {
                x = 0;
                txtright(t4, t4->l - 4);
                goto sv2nextitem;
            } //remove 0
            txtright(t4, t4->l - 4); //remove last 0
            txtright(t4, t4->l - 4); //remove last 0

            //txtset(t5,"final break"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
        }
        //txtset(t5,"extraction finished"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


        //but what about container links?
        for (x = 0; x <= x2; x++) {
            if (obji[sprlnk[sv2o[x]->type & 1023]].flags & 1024) {
                //container
                if (sv2o[x]->type == 57) playerlist[tpl]->GNPCflags[30] = 1; //spellbook exists?
                if (sv2o[x]->more2) {
                    myobj2 = sv2o[sv2o[x]->more2 - 1];
                    sv2o[x]->more = myobj2;
                    myobj2->prev = sv2o[x]; //not sure if this is required but hey!
                } //->more2
            } //multiple
        } //x

        if (housestoragerestore) goto housestoragerestore1;

        if (sv2op[0]) tnpc->helm = sv2o[sv2op[0] - 1];
        if (sv2op[1]) tnpc->wep_right = sv2o[sv2op[1] - 1];
        if (sv2op[2]) tnpc->wep_left = sv2o[sv2op[2] - 1];
        if (sv2op[3]) tnpc->armour = sv2o[sv2op[3] - 1];
        if (sv2op[4]) tnpc->boots = sv2o[sv2op[4] - 1];
        if (sv2op[5]) tnpc->neck = sv2o[sv2op[5] - 1];
        if (sv2op[6]) tnpc->ring_right = sv2o[sv2op[6] - 1];
        if (sv2op[7]) tnpc->ring_left = sv2o[sv2op[7] - 1];
        if (sv2op[8]) {
            /* tnpc->items was already set before !! and the bag item was never removed so It caused mem leak */
            OBJremove(tnpc->items);
            OBJrelease(tnpc->items);
            tnpc->items = sv2o[sv2op[8] - 1]; //includes the bag!
            tnpc->baseitem = tnpc->items;
        }
        if (playerlist[tpl]->party[1] == NULL) {
            if (sv2op[9]) playerlist[tpl]->mobj = sv2o[sv2op[9] - 1];
        }

        if (playerlist[tpl]->GNPCflags[252]) {
            playerlist[tpl]->GNPCflags[252]--;
            //create a new NPC
            x8 = 1;
        findemptyparty:
            if (playerlist[tpl]->party[x8]) {
                x8++;
                goto findemptyparty;
            }

            myobj = OBJnew();
            playerlist[tpl]->party[x8] = myobj; //player<-

            tnpc = (npc *) malloc(sizeof(npc));
            ZeroMemory(tnpc, sizeof(npc));
            myobj->more = tnpc;
            myobj->info |= 2; //myobj<-
            //create an INVISIBLE container for player's items
            myobj2 = OBJnew();
            myobj2->type = 188; //bag (INVISIBLE)
            tnpc->items = myobj2;
            tnpc->baseitem = myobj2;
            tnpc->order = 1;
            tnpc->follow = playerlist[tpl]->party[0];
            tnpc->name = txtnew();
            tnpc->player = playerlist[tpl];
            tnpc->upflags = 1;


            OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj);

            //txtnumint(t5,0); txtadd(t5,":objadd"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


            goto addnewpartymember;
        }

        //txtnumint(t5,t4->ds[0]); txtadd(t5,":firstobj"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);


        //28 house number (not valid if player has been evicted!)
        //29 number of house storage positions saved

        // s111 increase house storage max slots
        //    static unsigned char housestoragerestorei,housechest;
        static unsigned char housechest;
        static unsigned int housestoragerestorei;
        housechest = 0;
        if (playerlist[tpl]->GNPCflags[28]) {
            //MessageBox(NULL,"HOUSESTORAGECHECK: gflags say still a house owner","Ultima 6 Online",MB_OK);


            if (houseowner(playerlist[tpl], playerlist[tpl]->GNPCflags[28]) == 0) {
                //player no longer owns house registered in player structure
                housechest = 1;
                playerlist[tpl]->GNPCflags[28] = 0;
            }
        }

        for (housestoragerestorei = 0; housestoragerestorei < playerlist[tpl]->GNPCflags[29]; housestoragerestorei++) {
            //housestoragerestorei=0;

            if (t4->dl[0]) {
                housestoragerestore = 1;
                goto housestoragerestore0;
            housestoragerestore1:
                housestoragerestore = 0;

                myobj2 = sv2o[0];

                if (housechest == 0) {
                housestoragerestore2:
                    myobj3 = (object *) myobj2->next;

                    if (obji[sprlnk[myobj2->type & 1023]].weight) {
                        //FIX for red gate/balloon and other issues
                        OBJadd(housestoragex[playerlist[tpl]->GNPCflags[28]][housestoragerestorei],
                               housestoragey[playerlist[tpl]->GNPCflags[28]][housestoragerestorei], myobj2);
                    }


                    if (myobj3) {
                        myobj2 = myobj3;
                        goto housestoragerestore2;
                    }
                } else {
                    //put items in a chest and 
                    myobj3 = OBJnew();
                    myobj3->type = 98 + 1024;
                    myobj3->more = myobj2;
                    myobj2->prev = myobj3;
                    tnpc3 = (npc *) playerlist[tpl]->party[0]->more;
                    additemroot(tnpc3, myobj3);
                } //==0
            } else {
                txtright(t4, t4->l - 4);
            }
        }

        //housestoragerestore1:

        //for (x2=0;x2<playerlist[tpl]->GNPCflags[29];x2++){
        //myobj=(object*)od[housestoragey[playerlist[tpl]->GNPCflags[28]][x2]][housestoragex[playerlist[tpl]->GNPCflags[28]][x2]]->next;
        //x5=0;
        //housestorageadd=1; goto housestorageadd0;
        //housestorageadd1: housestorageadd=0;
        //}

        //fix inventory skiff deeds
        for (x = 0; x <= iskiffx; x++) {
            if (ideedx >= x) {
                iskiff[x]->more = ideed[x];
                ideed[x]->more = iskiff[x];
            } else {
                iskiff[x]->info |= (1 << 9); //free skiff!
                iskiff[x]->more2 = NULL; //null deed
            }
        }

        for (x = 0; x <= ideedx; x++) {
            if (iskiffx < x) {
                OBJremove(ideed[x]); //remove invalid deed!
            }
        }


        goto sv2_done;

    sv1_objects:
        //equipment
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->helm = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->wep_right = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->wep_left = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->armour = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->boots = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->neck = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->ring_right = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);
        if (t4->ds[0]) {
            myobj2 = OBJnew();
            myobj2->type = t4->ds[0];
            myobj2->info = t4->ds[1];
            myobj2->more2 = t4->dl[1];
            tnpc->ring_left = myobj2;
            stealing(playerlist[tpl], myobj2);
        }
        txtright(t4, t4->l - 8);


        x = t4->ds[0];
        txtright(t4, t4->l - 2); //item n
        for (x2 = 0; x2 < x; x2++) {
            myobj3 = OBJnew();
            myobj3->type = t4->ds[0];
            myobj3->info = t4->ds[1];
            myobj3->more2 = t4->dl[1];

            //PATCH! 1.0
            if (myobj3->type == 64) {
                //key A
                if (myobj3->more2 & 32768) myobj3->more2 -= 32768;
            }


            stealing(playerlist[tpl], myobj3);
            additem(tnpc, myobj3);


            if (myobj3->type == 149) {
                //deed
                //ok lets add this ship if it's not current ship
                if (myobj3->more2 != 0xFFFFFFFF) {
                    //create ship/skiff!
                    if (myobj3->info & (1 << 9)) {
                        //skiff
                        myobj5 = OBJnew();
                        myobj5->type = 414;
                        OBJadd(myobj3->more2 & 65535, myobj3->more2 >> 16, myobj5);
                        myobj3->more = (object *) myobj5;
                    } else {
                        //ship

                        myobj5 = OBJnew();
                        myobj5->type = 412 + 9 * 1024;
                        OBJadd(myobj3->more2 & 65535, myobj3->more2 >> 16, myobj5);
                        static mlobj *mmyobj; //array size varies
                        mmyobj = (mlobj *) malloc(sizeof(object *) * 5);
                        myobj5->more = mmyobj;
                        myobj = OBJnew();
                        mmyobj->obj[0] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[1] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[2] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[3] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj = OBJnew();
                        mmyobj->obj[4] = myobj;
                        myobj->info |= 8;
                        myobj->type = myobj5->type;
                        myobj->more = (object *) myobj5;
                        OBJadd(myobj5->x, myobj5->y, myobj);
                        myobj3->more = myobj5; //deed->ship

                        OBJmove_allow = TRUE;
                        OBJmove2(myobj5, myobj5->x, myobj5->y + 1);
                        OBJmove2(myobj5, myobj5->x, myobj5->y - 1);
                        OBJmove_allow = FALSE;
                        myobj3->more = (object *) myobj5;
                    }
                } else {
                    myobj6 = myobj3;
                } //set as current deed
            } //149


            txtright(t4, t4->l - 8);
        }


    sv2_done:
        playerlist[tpl]->karma = f;
        stealing_MESSAGE = TRUE;

        //move player to front door of house if not the owner
        x2 = playerlist[tpl]->party[0]->x;
        y2 = playerlist[tpl]->party[0]->y;
        x = housecheck(x2, y2);
        if (x) {
            if (houseowner(playerlist[tpl], x)) goto houseboundrycheck0;
            //trespassing! move player to entrance
            for (x3 = 7; x3 >= 0; x3--) {
                if (playerlist[tpl]->party[x3]) {
                    OBJremove(playerlist[tpl]->party[x3]);
                    OBJadd(houseentrancex[x], houseentrancey[x], playerlist[tpl]->party[x3]);
                }
            } //x3,->party[x3]
            goto houseboundrycheck0;
        } //x
    houseboundrycheck0:

        //place selected partymembers on horses
        for (x = 0; x <= 7; x++) {
            if (npchorsepapers[x]) {
                myobj2 = playerlist[tpl]->party[x];
                tnpc2 = (npc *) myobj2->more;

                z2 = npchorsepapers[x]->more2 & 2097151;
                if ((z2 <= 7) || (z2 == 2097151)) {
                    movernew(430, myobj2->x, myobj2->y, MOVERNEW_FLAG_NORESPAWN);
                    myobj = MOVERNEW_OBJECT;
                } else {
                    movernew(430, z2 & 2047, z2 >> 11, MOVERNEW_FLAG_NORESPAWN);
                    myobj = MOVERNEW_OBJECT;
                }

                ((creature *) MOVERNEW_OBJECT->more)->flags |= 128;
                ((creature *) MOVERNEW_OBJECT->more)->al = 0;
                ((creature *) MOVERNEW_OBJECT->more)->respawn_y = tpl;
                ((creature *) MOVERNEW_OBJECT->more)->respawn_x = (npchorsepapers[x]->more2 >> 21) & 255;
                //remove horse rear end!
                //OBJremove((object*)((creature*)MOVERNEW_OBJECT->more)->more);

                OBJmove_allow = TRUE;
                if (((npchorsepapers[x]->more2 >> 29) & 3) == 1) {
                    OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x - 1, MOVERNEW_OBJECT->y);
                    OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x + 1, MOVERNEW_OBJECT->y);
                }
                if (((npchorsepapers[x]->more2 >> 29) & 3) == 2) {
                    OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x, MOVERNEW_OBJECT->y - 1);
                    OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x, MOVERNEW_OBJECT->y + 1);
                }
                if (((npchorsepapers[x]->more2 >> 29) & 3) == 3) {
                    OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x + 1, MOVERNEW_OBJECT->y);
                    OBJmove2(MOVERNEW_OBJECT, MOVERNEW_OBJECT->x - 1, MOVERNEW_OBJECT->y);
                }
                OBJmove_allow = FALSE;

                x7 = MOVERNEW_OBJECT->x;
                y7 = MOVERNEW_OBJECT->y; //check if the horse is inside a house
                x6 = housecheck(x7, y7);
                if (x6) {
                    if (houseowner(playerlist[tpl], x6)) goto houseboundrycheck_horse1;
                    //move horse to entrance
                    OBJmove_allow = TRUE;
                    OBJmove2(MOVERNEW_OBJECT, houseentrancex[x6], houseentrancey[x6]);
                    OBJmove_allow = FALSE;
                    goto houseboundrycheck_horse1;
                } //x
            houseboundrycheck_horse1:;


                npchorsepapers[x]->more = myobj;
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
            }
        }

        //version 1+2, myobj6=pointer to deed of current ship
        x = playerlist[tpl]->GNPCflags[255];
        if (x) {
            //put player into craft (this is the really unfun bit!)

            if (x == 1) {
                //ship
                //build and place skiff facing new direction

                myobj3 = OBJnew();
                myobj3->type = 412 + 9 * 1024;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj3);

                static mlobj *mmyobj; //array size varies
                mmyobj = (mlobj *) malloc(sizeof(object *) * 5);
                myobj3->more = mmyobj;

                myobj = OBJnew();
                mmyobj->obj[0] = myobj;
                myobj->info |= 8;
                myobj->type = myobj3->type;
                myobj->more = (object *) myobj3;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj);
                myobj = OBJnew();
                mmyobj->obj[1] = myobj;
                myobj->info |= 8;
                myobj->type = myobj3->type;
                myobj->more = (object *) myobj3;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj);
                myobj = OBJnew();
                mmyobj->obj[2] = myobj;
                myobj->info |= 8;
                myobj->type = myobj3->type;
                myobj->more = (object *) myobj3;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj);
                myobj = OBJnew();
                mmyobj->obj[3] = myobj;
                myobj->info |= 8;
                myobj->type = myobj3->type;
                myobj->more = (object *) myobj3;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj);
                myobj = OBJnew();
                mmyobj->obj[4] = myobj;
                myobj->info |= 8;
                myobj->type = myobj3->type;
                myobj->more = (object *) myobj3;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj);

                playerlist[tpl]->craft_con = (object *) myobj3->more;
                myobj3->more = playerlist[tpl]->party[0]->more;
                myobj3->info = playerlist[tpl]->party[0]->info;
                playerlist[tpl]->craft = playerlist[tpl]->party[0];
                for (x2 = 0; x2 <= 7; x2++) {
                    if (playerlist[tpl]->party[x2]) {
                        OBJremove(playerlist[tpl]->party[x2]);
                        tnpc3 = (npc *) playerlist[tpl]->party[x2]->more;
                        if (tnpc3->horse) OBJremove((object *) tnpc3->more);
                    }
                }

                playerlist[tpl]->party[0] = myobj3;
                if (myobj3->info & 32768) myobj3->info -= 32768; //?

                OBJmove_allow = TRUE;
                OBJmove2(myobj3, playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y + 1);
                OBJmove2(myobj3, playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y - 1);
                OBJmove_allow = FALSE;

                if (myobj6) myobj6->more = (object *) myobj3; //allocate current deed
            } //ship

            if (x == 2) {
                //skiff
                //build and place skiff facing new direction
                myobj3 = OBJnew();
                myobj3->type = 414;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj3);
                myobj3->more = playerlist[tpl]->party[0]->more;
                myobj3->info = playerlist[tpl]->party[0]->info;
                playerlist[tpl]->craft = playerlist[tpl]->party[0];
                for (x2 = 0; x2 <= 7; x2++) { if (playerlist[tpl]->party[x2])OBJremove(playerlist[tpl]->party[x2]); }
                playerlist[tpl]->party[0] = myobj3;
                if (myobj3->info & 32768) myobj3->info -= 32768;

                if (myobj6) {
                    myobj6->more = (object *) myobj3; //allocate current deed
                } else {
                    myobj3->info |= (1 << 9);
                }
            } //skiff

            if (x == 3) {
                //raft
                //build and place skiff facing new direction
                myobj3 = OBJnew();
                myobj3->type = 415;
                OBJadd(playerlist[tpl]->party[0]->x, playerlist[tpl]->party[0]->y, myobj3);
                myobj3->more = playerlist[tpl]->party[0]->more;
                myobj3->info = playerlist[tpl]->party[0]->info;
                playerlist[tpl]->craft = playerlist[tpl]->party[0];
                for (x2 = 0; x2 <= 7; x2++) { if (playerlist[tpl]->party[x2])OBJremove(playerlist[tpl]->party[x2]); }
                playerlist[tpl]->party[0] = myobj3;
                if (myobj3->info & 32768) myobj3->info -= 32768; //?
            } //raft

            if (x == 4) {
                //balloon //build balloon
                myobj2 = OBJnew();
                //create a balloon where the avatar is standing
                x = playerlist[tpl]->party[0]->x;
                y = playerlist[tpl]->party[0]->y;
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
                playerlist[tpl]->craft_con = (object *) myobj2->more;
                myobj2->more = playerlist[tpl]->party[0]->more;
                myobj2->info = playerlist[tpl]->party[0]->info;
                playerlist[tpl]->craft = playerlist[tpl]->party[0];
                for (x2 = 0; x2 <= 7; x2++) { if (playerlist[tpl]->party[x2])OBJremove(playerlist[tpl]->party[x2]); }
                playerlist[tpl]->party[0] = myobj2;
            } //x==4
        } //x


        tnpc = (npc *) playerlist[tpl]->party[0]->more;

        if (playerlist[tpl]->GNPCflags[253] & 2) {
            //rest
            playerlist[tpl]->GNPCflags[253] -= 2;
            x = (unsigned long) ett - playerlist[tpl]->GNPCflags[254];
            if ((x >= 1536) || (x < 0)) {
                //~24min offline
                for (x = 0; x <= 7; x++) {
                    if (playerlist[tpl]->party[x]) {
                        tnpc2 = (npc *) playerlist[tpl]->party[x]->more;
                        tnpc2->hp = tnpc2->hp_max; //heal
                        tnpc2->mp = tnpc2->mp_max; //full mp
                    }
                }
            }
        }

        playerlist[tpl]->GNPCflags[251]++; //251 number of times entered britannia (as at last save)

        if (playerlist[tpl]->GNPCflags[19]) {
            //disable showing of lenses
            playerlist[tpl]->GNPCflags[22] |= 0xFFFFFFFF; //make all destinations available
            if (playerlist[tpl]->GNPCflags[18] & 2) playerlist[tpl]->GNPCflags[18] -= 2;
            if (playerlist[tpl]->GNPCflags[18] & 4) playerlist[tpl]->GNPCflags[18] -= 4;
            if (playerlist[tpl]->GNPCflags[18] & 16) playerlist[tpl]->GNPCflags[18] -= 16;
            //allow engame sequence to be replayed!
            //remove amulet of submission FIX
            tnpc = (npc *) playerlist[tpl]->party[0]->more;
            if (tnpc->neck) {
                if (tnpc->neck->type == 76) tnpc->neck = NULL;
            }
        } else {
            //ORBFIX: if a player was given all gate destinations incorrectly reset to 0
            if (playerlist[tpl]->GNPCflags[22] == 0xFFFFFFFF) playerlist[tpl]->GNPCflags[22] = 0;
        } //19

        //duplicate inparty name fix
        for (x = 0; x <= 6; x++) {
            if (playerlist[tpl]->party[x]) {
                tnpc2 = (npc *) playerlist[tpl]->party[x]->more;
                txtset(t, tnpc2->name);
                txtucase(t);
                txtset(t4, tnpc2->name);
                for (x2 = x + 1; x2 <= 7; x2++) {
                    if (playerlist[tpl]->party[x2]) {
                        tnpc2 = (npc *) playerlist[tpl]->party[x2]->more;
                        txtset(t2, tnpc2->name);
                        txtucase(t2);
                        if (txtsame(t, t2)) {
                            //we can assume that the name is not already indexed
                            y = 2;
                        inpartynamefix0:
                            txtset(t5, t);
                            if (y == 2) txtset(t3, "II");
                            if (y == 3) txtset(t3, "III");
                            if (y == 4) txtset(t3, "IV");
                            if (y == 5) txtset(t3, "V");
                            if (y == 6) txtset(t3, "VI");
                            if (y == 7) txtset(t3, "VII");
                            if (y == 8) txtset(t3, "VIII");
                            txtadd(t5, t3);
                            for (x3 = 0; x3 <= 7; x3++) {
                                if (playerlist[tpl]->party[x3]) {
                                    tnpc3 = (npc *) playerlist[tpl]->party[x3]->more;
                                    txtset(t6, tnpc3->name);
                                    txtucase(t6);
                                    if (txtsame(t5, t6)) {
                                        //no is unavailable
                                        y++;
                                        goto inpartynamefix0;
                                    } //txtsame
                                } //party[x3]
                            } //x3
                            if (y == 2) txtadd(tnpc2->name, "II");
                            if (y == 3) txtadd(tnpc2->name, "III");
                            if (y == 4) txtadd(tnpc2->name, "IV");
                            if (y == 5) txtadd(tnpc2->name, "V");
                            if (y == 6) txtadd(tnpc2->name, "VI");
                            if (y == 7) txtadd(tnpc2->name, "VII");
                            if (y == 8) txtadd(tnpc2->name, "VIII");
                        } //txtsame
                    }
                }
            }
        }

        //247&248 TEMP food values for npc 0-7, 1 byte each (only used when saving/loading)
        for (x2 = 0; x2 <= 7; x2++) {
            if (playerlist[tpl]->party[x2]) {
                tnpc3 = (npc *) playerlist[tpl]->party[x2]->more;
                if (x2 == 0) x3 = (playerlist[tpl]->GNPCflags[247]) & 255;
                if (x2 == 1) x3 = (playerlist[tpl]->GNPCflags[247] >> 8) & 255;
                if (x2 == 2) x3 = (playerlist[tpl]->GNPCflags[247] >> 16) & 255;
                if (x2 == 3) x3 = (playerlist[tpl]->GNPCflags[247] >> 24) & 255;
                if (x2 == 4) x3 = (playerlist[tpl]->GNPCflags[248]) & 255;
                if (x2 == 5) x3 = (playerlist[tpl]->GNPCflags[248] >> 8) & 255;
                if (x2 == 6) x3 = (playerlist[tpl]->GNPCflags[248] >> 16) & 255;
                if (x2 == 7) x3 = (playerlist[tpl]->GNPCflags[248] >> 24) & 255;
                tnpc3->food = x3;
            }
        }

        //remove house key(s),...
        //party inventory list(s) & m(ouse)obj(ect) (does not include equipped items!)
        for (x6 = 0; x6 <= 7; x6++) {
            if (playerlist[tpl]->party[x6]) {
                tnpc3 = (npc *) playerlist[tpl]->party[x6]->more;
                x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
            }
        } //!=NULL,x6
        x5 = OBJlist(playerlist[tpl]->mobj); //include mobj
        OBJlist_last = NULL;
        for (y6 = 0; y6 < x5; y6++) {
            if (OBJlist_list[y6]->type == 64) {
                //key
                if ((OBJlist_list[y6]->more2 & 0xFF00) == 256) {
                    //house key
                    OBJremove(OBJlist_list[y6]);
                }
            }
        } //y6
        //mouse object
        if (playerlist[tpl]->mobj) {
            if (playerlist[tpl]->mobj->type == 64) {
                //key
                if ((playerlist[tpl]->mobj->more2 & 0xFF00) == 256) {
                    //house key
                    playerlist[tpl]->mobj = NULL;
                }
            }
        }


        //add house key
        if (houseowner(playerlist[tpl], playerlist[tpl]->GNPCflags[28])) {
            tnpc3 = (npc *) playerlist[tpl]->party[0]->more;
            myobj3 = OBJnew();
            myobj3->type = 64;
            myobj3->info = 256;
            myobj3->more2 = 256 + playerlist[tpl]->GNPCflags[28]; //house key
            additemroot(tnpc3, myobj3);
        } //add house key

        // r555 debug: give house key
        /*
	if (TRUE) {
		//give key (256+housenumber)
		static object *myobj3 = OBJnew(); myobj3->type = 64; myobj3->info = 256; myobj3->more2 = 256 + 47;
		static object *myobj4 = tnpc3->items; static object *myobj5 = (object*)myobj4->more;
		if (myobj5) {
			myobj3->next = myobj5;
			myobj5->prev = myobj3;
			myobj3->prev = myobj4;
			myobj4->more = myobj3;
		}
		else {
			myobj3->next = NULL;
			myobj3->prev = myobj4;
			myobj4->more = myobj3;
		}
		tnpc3->upflags |= 32; //inv
		//tplayer->GNPCflags[28]=x2; //set housenumber
	}
	*/


        /*
    //equipped items
    txtset(t,"????????");
    myobj=tnpc->helm; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->wep_right; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->wep_left; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->armour; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->boots; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->neck; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->ring_right; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    myobj=tnpc->ring_left; t->dl[0]=0; t->dl[1]=0; if (myobj) {t->ds[0]=myobj->type; t->ds[1]=myobj->info; t->dl[1]=myobj->more2;} txtadd(t4,t);
    //items?
    //light?
    */
        txtset(t, "Player_Entered:");
        txtadd(t, tnpc->name);
        LOGadd(t);

        //ban player
        tfh = open2("ban.txt", OF_READWRITE | OF_SHARE_COMPAT); //log
