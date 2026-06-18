// LHS-P4.6: part_b_chat_npc -- CONTINUES MEGA B. WIRE.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch.
// Contents: chat/talk + NPC conversation (npccon_cull, instruction interpreter,
// shrine). Deeply nested; no interior depth-1 seams.
            npccon_cull:
                for (x2 = 0; x2 < t->l; x2++) {
                    x3 = t->d2[x2];
                    if ((x3 < 48) || ((x3 > 57) && (x3 < 65)) || ((x3 > 90) && (x3 < 97)) || (x3 > 122)) {
                        txtset(t2, t);
                        txtleft(t, x2);
                        txtright(t2, t2->l - x2 - 1);
                        txtadd(t, t2);
                        goto npccon_cull;
                    }
                }

                txtlcase(t);

                //NPC converse interpreter
                //<-NPC's object(myobj) + <-*player(tplayer)


                //if (t->l<=2){
                //txtset(t,"unknown");
                //}
                //next instruction

                static bool lookchk = FALSE;
                lookchk = FALSE;

                if (npcmess_shrine) {
                    x9 = npcmess_shrine;
                    goto npcmess_shrine2;
                }

                tnpc = (npc *) myobj->more;
                x9 = tnpc->converse;
                if (x9 == 255) x9 = 0; //*Dupre

            npcmess_shrine2:

                CONnpc = x9;
                x = npci[CONnpc]; //first instruction index

                //before checking first instruction, check for in-party commands

                if (!npcmess_shrine) {
                    if (tnpc->player == tplayer) {
                        //partymember
                        /*
            i3=0;
            txtset(t2,"dont"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"stop"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"not"); if (txtsearch(t,t2)) i3=1;
            if (i3){

            txtset(t2,"poti");
            if (txtsearch(t,t2)){
            txtset(t2,"yell");
            if (txtsearch(t,t2)){
            if (tnpc->command&1) tnpc->command-=1;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't use yellow potions.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"red");
            if (txtsearch(t,t2)){
            if (tnpc->command&2) tnpc->command-=2;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't use red potions.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"poti"

            txtset(t2,"eat");
            if (txtsearch(t,t2)){
            if (tnpc->command&4) tnpc->command-=4;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I'll refrain from eating.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }

            txtset(t2,"cast");
            if (txtsearch(t,t2)){
            txtset(t2,"cure");
            if (txtsearch(t,t2)){
            if (tnpc->command&32) tnpc->command-=32;
            if (tnpc->command&64) tnpc->command-=64;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't cast cure.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"heal");
            if (txtsearch(t,t2)){
            if (tnpc->command&8) tnpc->command-=8;
            if (tnpc->command&16) tnpc->command-=16;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I won't cast heal spells.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"cast"

            }//negative

            txtset(t2,"poti");
            if (txtsearch(t,t2)){
            txtset(t2,"yell");
            if (txtsearch(t,t2)){
            tnpc->command|=1;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I shall use yellow potions if heavily wounded.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"red");
            if (txtsearch(t,t2)){
            tnpc->command|=2;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I shall use red potions if poisoned.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"poti"

            txtset(t2,"eat");
            if (txtsearch(t,t2)){
            tnpc->command|=4;
            static txt *txttmp0;
            txttmp0=txtnew();
            txtset(txttmp0,"\x022I shall eat or drink when hungry.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }

            txtset(t2,"cast");
            if (txtsearch(t,t2)){
            i3=0;
            txtset(t2,"all"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"ever"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"part"); if (txtsearch(t,t2)) i3=1;
            txtset(t2,"cure");
            if (txtsearch(t,t2)){
            if (!i3) tnpc->command|=32; else tnpc->command|=64;
            static txt *txttmp0;
            txttmp0=txtnew();
            if (!i3) txtset(txttmp0,"\x022I shall cast cure if poisoned.\x022"); else txtset(txttmp0,"\x022I shall cast cure if anyone in our party is poisoned.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            txtset(t2,"heal");
            if (txtsearch(t,t2)){
            if (!i3) tnpc->command|=8; else tnpc->command|=16;
            static txt *txttmp0;
            txttmp0=txtnew();
            if (!i3) txtset(txttmp0,"\x022I shall cast heal spells if heavily wounded.\x022"); else txtset(txttmp0,"\x022I shall cast heal spells if anyone in our party is heavily wounded.\x022");
            i3=SFnew(myobj->x,myobj->y); //i3 points to new sf
            sf[i3].type=3; //message to user
            sf[i3].x=myobj->x;
            sf[i3].y=myobj->y-2;
            sf[i3].wait=5;
            sf[i3].more=tnpc->port;
            //addnpcport2sf(i3,tnpc);
            sf[i3].p=txttmp0;
            goto doneclmess;
            }
            }//"cast"

            */

                        if (tnpc->converse == NULL) goto talk_skipnullobject;
                    } //tplayer
                } //!npcmess_shrine (->npc)

            nextinstruction:


                if (npcbin[x].l1 == 1) {
                    //if=keyword goto (ifkey)
                    //long txtsearch(txt*t,txt*t2);

                    txtNEWLEN(t2, npcbin[x].l3);
                    memcpy(t2->d2, &npcinf[npcbin[x].l2], npcbin[x].l3);


                ifkey_next:
                    txtset(t4, t2);
                    txtset(t3, ",");
                    if (x2 = txtsearch(t2, t3)) {
                        txtleft(t4, x2 - 1);
                        txtright(t2, t2->l - x2);
                    } else {
                        txtset(t2, "");
                    }
                    //txtset(t3," "); txtadd(t3,t4); txtset(t4,t3); //add a space before the keyword
                    //txtset(t5," "); txtadd(t5,t);
                    if (txtsearch(t, t4)) {
                        //t2=keyword string
                        txtset(t3, "look");
                        if (txtsearch(t4, t3)) lookchk = TRUE;
                        x++;
                        goto nextinstruction;
                    }
                    if (t2->l) goto ifkey_next;

                    //it is possible t2 is a truncated version of t if it is 4 characters long
                    //if (t2->l==4){ //short keyword
                    //txtset(t3,t); if (t3->l>4) txtleft(t3,4);
                    //if (txtsearch(t2,t3)){
                    //x++; goto nextinstruction;
                    //}
                    //}


                    x = npcbin[x].l4;
                    goto nextinstruction;
                }


                if ((npcbin[x].l1 == 2) || (npcbin[x].l1 == 22)) {
                    //display txt and exit || display txt and DONT exit
                    i3 = SFnew(myobj->x, myobj->y); //i3 points to new sf
                    sf[i3].type = 3; //message to user
                    sf[i3].x = myobj->x;
                    sf[i3].y = myobj->y - 2;
                    sf[i3].wait = 5;
                    tnpc = (npc *) myobj->more;
                    if (npcmess_shrine == 0) {
                        //GPFGPF
                        sf[i3].more = tnpc->port;
                        //portrait upper byte may be used to store more information (or it could be stored in the message)
                        //addnpcport2sf(i3,tnpc);
                    } else {
                        sf[i3].more = 187; //blank!
                        if (npcmess_shrine == 199) sf[i3].more = 191; //wisp
                        if (npcmess_shrine == 179) sf[i3].more = 180; //man in stocks
                        if (npcmess_shrine == 200) sf[i3].more = 192; //guard
                    }

                    if (CONport != 0xFFFFFFFF) sf[i3].more = CONport;
                    CONport = 0xFFFFFFFF;
                    static txt *inouttxt2;
                    inouttxt2 = txtnew();
                    txtNEWLEN(inouttxt2, npcbin[x].l3);
                    memcpy(inouttxt2->d2, &npcinf[npcbin[x].l2], npcbin[x].l3);

                    if (lookchk) {
                        txtset(t2, "look");
                        if (txtsame(t, t2)) {
                            txtset(t2, "You see ");
                            txtadd(t2, inouttxt2);
                            txtset(inouttxt2, t2);
                            lookchk = 0;
                        }
                    }

                    txtset(t2, "ambidextrous!"); //ambidextrous (FIX: M'sieur Loubet training +1 DEX)
                    if (txtsearch(inouttxt2, t2)) {
                        tnpc2 = (npc *) tplayer->party[0]->more;
                        tnpc2->d++;
                        tnpc2->upflags |= 1;
                    }

                insert_txt:

                    /*
          if ((tnpc->i>=tnpc->s)&&(tnpc->i>=tnpc->d)) myobj->type=378; //i
          if ((tnpc->d>=tnpc->s)&&(tnpc->d>=tnpc->i)) myobj->type=386; //d
          if ((tnpc->s>=tnpc->d)&&(tnpc->s>=tnpc->i)) myobj->type=376; //s
          if ((tnpc->i>tnpc->s)&&(tnpc->d>tnpc->s))  myobj->type=377; //i+d

          txtset(t2,"$NAME"); //$NAME
          if (x2=txtsearch(inouttxt2,t2)){
          txtset(t2,inouttxt2); txtleft(t2,x2-1);
          txtset(t3,inouttxt2); txtright(t3,inouttxt2->l-x2-2);

          //txtnumint(t4,CONreg[0]); txtadd(t2,t4);
          txtadd(t2,


          txtadd(t2,t3); txtset(inouttxt2,t2);
          goto insert_txt;
          }
          */
                    //CONnpc=tnpc; //only valid if #converse is derived from an NPC pointer!


                    txtset(t2, "$RATE"); //$RATE
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 4);
                        //assume +3 per level, anything above min threshold costs double!!!
                        x2 = 37 + 3 * CONnpc2->lev;
                        if ((CONnpc2->s + CONnpc2->d + CONnpc2->i) > x2) txtadd(t2, "24");
                        else txtadd(t2, "8");
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$SDI"); //$SDI
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 3);
                        txtadd(t2, "(str:");
                        txtnumint(t4, CONnpc2->s);
                        txtadd(t2, t4);
                        txtadd(t2, ", dex:");
                        txtnumint(t4, CONnpc2->d);
                        txtadd(t2, t4);
                        txtadd(t2, ", int:");
                        txtnumint(t4, CONnpc2->i);
                        txtadd(t2, t4);
                        txtadd(t2, ")");
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$CLASS"); //$CLASS
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 5);
                        txtset(t4, "ranger");
                        if ((CONnpc2->i > CONnpc2->s) && (CONnpc2->i > CONnpc2->d)) {
                            txtset(t4, "mage");
                            goto gotclass;
                        }
                        if ((CONnpc2->d > CONnpc2->s) && (CONnpc2->d > CONnpc2->i)) {
                            txtset(t4, "bard");
                            goto gotclass;
                        }
                        if ((CONnpc2->s > CONnpc2->d) && (CONnpc2->s > CONnpc2->i)) {
                            txtset(t4, "fighter");
                            goto gotclass;
                        }
                        if ((CONnpc2->s > CONnpc2->d) && (CONnpc2->i > CONnpc2->d)) {
                            txtset(t4, "paladin");
                            goto gotclass;
                        }
                        if ((CONnpc2->s > CONnpc2->i) && (CONnpc2->d > CONnpc2->i)) {
                            txtset(t4, "tinker");
                            goto gotclass;
                        }
                        if ((CONnpc2->d > CONnpc2->s) && (CONnpc2->i > CONnpc2->s)) {
                            txtset(t4, "druid");
                            goto gotclass;
                        }
                        x2 = CONnpc2->s + CONnpc2->d + CONnpc2->i;
                        if (x2 <= 52) txtset(t4, "shepard");
                    gotclass:
                        /*
            Honesty (Truth - Mage) i3
            Compassion (Love - Bard)  d3
            Valor (Courage - Fighter) s3
            Justice (Truth and Love - Druid) s1 i1
            Sacrifice (Love and Courage - Tinker) s1 d1
            Honor (Truth and Courage - Paladin) d1 i1
            Spirituality (Truth and Love and Courage - Ranger) s1 d1 i1
            Humility (None - Shepherd) N/A

            */
                        txtadd(t2, t4);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }


                    txtset(t2, "$NAME"); //$NAME
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 4);
                        txtadd(t2, CONnpc2->name);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$HIRE"); //$HIRE
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 4);
                        txtadd(t2, "24");
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$V1"); //$V1
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 2);
                        txtnumint(t4, CONreg[1]);
                        txtadd(t2, t4);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$V0"); //$V0
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 2);
                        txtnumint(t4, CONreg[0]);
                        txtadd(t2, t4);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$PARTYMEMBER"); //$PARTYMEMBER party member's name
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 11);
                        tnpc2 = (npc *) tplayer->party[CONpartymember - 1]->more;
                        txtadd(t2, tnpc2->name);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$P"); //$P name
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 1);
                        tnpc2 = (npc *) tplayer->party[0]->more;
                        txtadd(t2, tnpc2->name);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$Y"); //$Y name (originally of talking party member)
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 1);
                        tnpc2 = (npc *) tplayer->party[0]->more;
                        txtadd(t2, tnpc2->name);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$G"); //$G
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 1);
                        if (tplayer->male_female) txtadd(t2, "milady");
                        else txtadd(t2, "milord");
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$T"); //$T morning afternoon evening
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 1);
                        if (btime < 12) txtadd(t2, "morning");
                        if ((btime >= 12) && (btime <= 18)) txtadd(t2, "afternoon");
                        if (btime > 18) txtadd(t2, "evening");
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }

                    txtset(t2, "$Z"); //$Z
                    if (x2 = txtsearch(inouttxt2, t2)) {
                        txtset(t2, inouttxt2);
                        txtleft(t2, x2 - 1);
                        txtset(t3, inouttxt2);
                        txtright(t3, inouttxt2->l - x2 - 1);
                        txtadd(t2, t9);
                        txtadd(t2, t3);
                        txtset(inouttxt2, t2);
                        goto insert_txt;
                    }
                    sf[i3].p = inouttxt2;

                    if (npcbin[x].l1 == 22) {
                        x++;
                        goto nextinstruction;
                    }
                }

                if (npcbin[x].l1 == 3) {
                    //jmp
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 4) {
                    //=
                    CON_sv(npcbin[x].l2, npcbin[x].l3);
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 5) {
                    //if=
                    x3 = CON_gv(npcbin[x].l3);
                    x2 = CON_gv(npcbin[x].l2);
                    if (x2 == x3) {
                        //=
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 6) {
                    //item
                    x2 = CON_gv(npcbin[x].l2); //type
                    x3 = CON_gv(npcbin[x].l3); //info/qty

                    myobj4 = myobj; //backup!
                    tnpc2 = (npc *) tplayer->party[0]->more;
                    myobj3 = OBJnew();
                    myobj3->type = x2;
                    myobj3->more2 = x3;
                    if ((tnpc2->wt + WTfind(myobj3)) <= tnpc2->wt_max) {
                        additemroot(tnpc2, myobj3);
                    } //weight
                    tnpc2->upflags |= 32; //inv
                    myobj = myobj4; //<-backup
                    x++;
                    goto nextinstruction;
                }


                //buy/give
                if (npcbin[x].l1 == 7) {
                    //buy
                    x2 = CON_gv(npcbin[x].l2); //type
                    x3 = CON_gv(npcbin[x].l3); //number of items (1-?) *0 is invalid
                    x4 = CON_gv(npcbin[x].l4); //gold


                    myobj4 = myobj; //backup!
                    tnpc2 = (npc *) tplayer->party[0]->more;

                    //check gold
                    for (x6 = 0; x6 <= 7; x6++) {
                        if (tplayer->party[x6]) {
                            tnpc3 = (npc *) tplayer->party[x6]->more;
                            x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                        }
                    } //!=NULL,x6
                    OBJlist_last = NULL;
                    y7 = 0;
                    y8 = 0;
                    for (y6 = 0; y6 < x5; y6++) {
                        if (OBJlist_list[y6]->type == 88) {
                            //gold
                            if (y7 < x4) {
                                if (OBJlist_list[y6]->info & 128) y8 = 1;
                            }
                            y7 += OBJlist_list[y6]->more2;
                        }
                    }
                    if (y7 < x4) {
                        CONerr = 1;
                        goto buy_nogold;
                    } //not enough gold

                    if (CONqual != 0xFFFFFFFF) {
                        if (CONqual & 16384) {
                            //ignore stolen gold!
                            CONqual -= 16384;
                            if (y8) {
                                CONerr = 1;
                                goto buy_nogold;
                            } //not enough gold
                        }
                    }

                    if (x2 == 0) goto buy_removegold;
                buy_newitem:

                    myobj3 = OBJnew();
                    myobj3->type = x2;

                    if (obji[sprlnk[x2 & 1023] + (x2 >> 10)].flags & 4096) myobj3->more2 = 1;
                    //temp =1 to find weight of single object
                    f = WTfind(myobj3) * x3;
                    myobj3->more2 = 0; //*can override

                    if (CONqual != 0xFFFFFFFF) {
                        if (CONqual & 32768) {
                            CONqual -= 32768;
                            myobj3->info |= 256;
                        } //quest
                        myobj3->more2 = CONqual;
                    }


                    if (obji[sprlnk[x2 & 1023] + (x2 >> 10)].flags & 4096) {
                        myobj3->more2 = x3;
                        x3 = 1;
                    }

                    //x3 now equals number of objects to allocate

                    if (myobj3->info & 256) goto questitem_skipweightcheck2;
                    if ((tnpc2->wt + f) <= tnpc2->wt_max) {
                    questitem_skipweightcheck2:
                        if (y8) myobj3->info |= 128;
                        stealing(tplayer, myobj3);

                        ENHANCEnewn(myobj3, 8, 8);

                        myobj6 = myobj3;
                        if (myobj6->type == 76) {
                            if (!tnpc2->neck) goto amuletofsubmission0;
                            myobj3 = tnpc2->neck;
                        }
                        additemroot(tnpc2, myobj3);
                        x3--;
                        if (x3) goto buy_newitem;

                        if (myobj6->type == 76) {
                        amuletofsubmission0:
                            tnpc2->neck = myobj6;
                            tnpc2->upflags |= 64;
                        }

                        //buy horse papers
                        if (myobj3->type == 448) {
                            if (CONnpc == 25) movernew(430, 280, 441, MOVERNEW_FLAG_NORESPAWN);
                            if (CONnpc == 78) movernew(430, 412, 802, MOVERNEW_FLAG_NORESPAWN);
                            ((creature *) MOVERNEW_OBJECT->more)->flags |= 128;
                            ((creature *) MOVERNEW_OBJECT->more)->al = 0;
                            static long horseval;
                            //locate tpl index
                            ((creature *) MOVERNEW_OBJECT->more)->respawn_y = 0xFFFF;
                            for (horseval = 0; horseval <= playerlist_last; horseval++) {
                                if (playerlist[horseval]) {
                                    if (playerlist[horseval] == tplayer) {
                                        ((creature *) MOVERNEW_OBJECT->more)->respawn_y = horseval;
                                        goto buyhorse_foundtpl;
                                    }
                                }
                            }
                        buyhorse_foundtpl:
                            //set initial horse status (races won, food, horseshoes)
                            horseval = 0;
                            if (((long) (rnd * 2)) == 0) {
                            buyhorseaddracewon:
                                if (horseval < 15) {
                                    if (((long) (rnd * 2)) == 0) {
                                        horseval++;
                                        goto buyhorseaddracewon;
                                    }
                                }
                            }
                            if (CONnpc == 25)
                                ((creature *) MOVERNEW_OBJECT->more)->respawn_x =
                                        horseval + (2 << 4) + (1 << 6);
                            if (CONnpc == 78)
                                ((creature *) MOVERNEW_OBJECT->more)->respawn_x =
                                        horseval + (1 << 4) + (0 << 6);

                            myobj3->more = MOVERNEW_OBJECT;
                            //the fact that this is a player's horse needs to be set somewhere...
                        } //buy horse papers

                        //buy skiff/ship?
                        if ((myobj3->type & 1023) == 149) {
                            if (CONqual == 0) {
                                myobj5 = OBJnew();
                                myobj5->type = 412 + 9 * 1024;
                                if (CONnpc == 26) OBJadd(316, 441, myobj5); //arty
                                if (CONnpc == 119) OBJadd(592, 622, myobj5); //fentrissa
                                if (CONnpc == 51) OBJadd(164, 872, myobj5); //peer
                                if (CONnpc == 70) OBJadd(603, 119, myobj5); //trebor

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
                            } //CONqual

                            if (CONqual == 1) {
                                myobj5 = OBJnew();
                                myobj5->type = 414;
                                if (CONnpc == 26) OBJadd(316, 441, myobj5); //arty
                                if (CONnpc == 119) OBJadd(592, 622, myobj5); //fentrissa
                                if (CONnpc == 51) OBJadd(164, 872, myobj5); //peer
                                if (CONnpc == 70) OBJadd(603, 119, myobj5); //trebor
                                myobj3->info |= (1 << 9);
                                myobj3->more = myobj5; //deed->skiff
                            } //CONqual
                        } //149

                        tnpc2->upflags |= 32; //inv

                    buy_removegold:

                        //remove gold!
                        //recreate OBJlist
                        for (x6 = 0; x6 <= 7; x6++) {
                            if (tplayer->party[x6]) {
                                tnpc3 = (npc *) tplayer->party[x6]->more;
                                x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                            }
                        } //!=NULL,x6
                        OBJlist_last = NULL;

                        for (y6 = 0; y6 < x5; y6++) {
                            if (OBJlist_list[y6]->type == 88) {
                                //gold
                                myobj5 = OBJlist_list[y6];
                                x7 = myobj5->more2; //25
                                x7 -= x4; //25-10=10
                                x4 -= myobj5->more2; //=-15

                                //txtnumint(t5,x4);
                                //MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

                                if (x7 <= 0) {
                                    OBJremove(myobj5);
                                    OBJrelease(myobj5);
                                } else {
                                    myobj5->more2 = x7;
                                }
                                if (x4 <= 0) goto buy_gotgold;
                            }
                        }
                    buy_gotgold:

                        //update ALL! party inventory windows
                        for (x6 = 0; x6 <= 7; x6++) {
                            if (tplayer->party[x6]) {
                                tnpc3 = (npc *) tplayer->party[x6]->more;
                                tnpc3->upflags |= 32; //inv
                            }
                        } //!=NULL,x6
                    } else { CONerr = 2; } //weight


                buy_nogold:
                    CONqual = 0xFFFFFFFF; //reset
                    myobj = myobj4; //<-backup
                    x++;
                    goto nextinstruction;
                }


                if (npcbin[x].l1 == 8) {
                    //if!=
                    x3 = CON_gv(npcbin[x].l3);
                    x2 = CON_gv(npcbin[x].l2);
                    if (x2 != x3) {
                        //=
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 9) {
                    //+
                    CON_sv(npcbin[x].l2, CON_gv(npcbin[x].l2) + CON_gv(npcbin[x].l3));
                    x++;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 10) {
                    //-
                    CON_sv(npcbin[x].l2, CON_gv(npcbin[x].l2) - CON_gv(npcbin[x].l3));
                    x++;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 11) {
                    //*
                    CON_sv(npcbin[x].l2, CON_gv(npcbin[x].l2) * CON_gv(npcbin[x].l3));
                    x++;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 12) {
                    //div /
                    CON_sv(npcbin[x].l2, CON_gv(npcbin[x].l2) / CON_gv(npcbin[x].l3));
                    x++;
                    goto nextinstruction;
                }


                if (npcbin[x].l1 == 13) {
                    //ifitem
                    tnpc2 = (npc *) tplayer->party[0]->more;
                    for (x6 = 0; x6 <= 7; x6++) {
                        if (tplayer->party[x6]) {
                            tnpc3 = (npc *) tplayer->party[x6]->more;
                            x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                        }
                    } //!=NULL,x6
                    OBJlist_last = NULL;
                    y7 = 0; //count
                    for (y6 = 0; y6 < x5; y6++) {
                        if (CON_gv(npcbin[x].l2) == 185) {
                            if ((OBJlist_list[y6]->type & 1023) == 185) goto ifitem_clothtype;
                        }
                        if (OBJlist_list[y6]->type == CON_gv(npcbin[x].l2)) {
                        ifitem_clothtype:
                            if ((CONqual == 0xFFFFFFFF) || (CONqual == OBJlist_list[y6]->more2)) {
                                if ((OBJlist_list[y6]->info & 32768) == 0) {
                                    //skip if not for sale
                                    if (obji[sprlnk[OBJlist_list[y6]->type & 1023] + (OBJlist_list[y6]->type >> 10)].
                                        flags & 4096)
                                        y7 += OBJlist_list[y6]->more2;
                                    else y7++;
                                }
                            } //qual
                        }
                    }
                    CONqual = 0xFFFFFFFF; //reset
                    if (y7 >= CON_gv(npcbin[x].l3)) {
                        //=
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }


                if (npcbin[x].l1 == 14) {
                    //sell
                    x2 = CON_gv(npcbin[x].l2); //type
                    x3 = CON_gv(npcbin[x].l3); //info/qty
                    if (x3 == 0) x3 = 1;
                    x4 = CON_gv(npcbin[x].l4); //gold


                    myobj4 = myobj; //backup!
                    tnpc2 = (npc *) tplayer->party[0]->more;
                    for (x6 = 0; x6 <= 7; x6++) {
                        if (tplayer->party[x6]) {
                            tnpc3 = (npc *) tplayer->party[x6]->more;
                            x5 = OBJlist((object *) tnpc3->items->more); //x5=last object number
                        }
                    } //!=NULL,x6
                    OBJlist_last = NULL;
                    y7 = 0; //quantity
                    for (y6 = 0; y6 < x5; y6++) {
                        if (x2 == 185) {
                            if ((OBJlist_list[y6]->type & 1023) == 185) goto sell_clothtype;
                        }
                        if (OBJlist_list[y6]->type == x2) {
                            //has item?
                        sell_clothtype:
                            if ((CONqual == 0xFFFFFFFF) || (CONqual == OBJlist_list[y6]->more2)) {
                                if ((OBJlist_list[y6]->info & 32768) == 0) {
                                    //skip if not for sale
                                    if (obji[sprlnk[OBJlist_list[y6]->type & 1023] + (OBJlist_list[y6]->type >> 10)].
                                        flags & 4096)
                                        y7 += OBJlist_list[y6]->more2;
                                    else y7++;
                                }
                            } //qual
                        }
                    }
                    if (y7 < x3) {
                        CONerr = 1;
                        goto sell_noitem;
                    }

                    //remove item!
                    y8 = 0;

                    for (y6 = 0; y6 < x5; y6++) {
                        if (x2 == 185) {
                            if ((OBJlist_list[y6]->type & 1023) == 185) goto sell_clothtype2;
                        }
                        if (OBJlist_list[y6]->type == x2) {
                            //item->type
                        sell_clothtype2:
                            if ((CONqual == 0xFFFFFFFF) || (CONqual == OBJlist_list[y6]->more2)) {
                                if ((OBJlist_list[y6]->info & 32768) == 0) {
                                    //skip if not for sale
                                    myobj5 = OBJlist_list[y6];
                                    if (myobj5->info & 128) y8 = 1; //stealing!
                                    if (obji[sprlnk[OBJlist_list[y6]->type & 1023] + (OBJlist_list[y6]->type >> 10)].
                                        flags & 4096)
                                        x8 = OBJlist_list[y6]->more2;
                                    else x8 = 1;
                                    x7 = x8;
                                    x7 -= x3;
                                    x3 -= x8;
                                    if (x7 <= 0) {
                                        OBJremove(myobj5);
                                        OBJrelease(myobj5);
                                    } else {
                                        myobj5->more2 = x7;
                                    }
                                    if (x3 <= 0) goto sell_gotitem;
                                } //not for sale
                            } //qual
                        } //type
                    } //y6
                sell_gotitem:

                    if (x4) {
                        myobj3 = OBJnew();
                        myobj3->type = 88;
                        myobj3->more2 = x4; //gold
                        if (y8) myobj3->info |= 128; //stealing!
                        stealing(tplayer, myobj3);
                        additemroot(tnpc2, myobj3);
                    } //x4

                    tnpc2->upflags |= 32; //inv


                    //update ALL party inventory windows
                    for (x6 = 0; x6 <= 7; x6++) {
                        if (tplayer->party[x6]) {
                            tnpc3 = (npc *) tplayer->party[x6]->more;
                            tnpc3->upflags |= 32; //inv
                        }
                    } //!=NULL,x6

                sell_noitem:
                    CONqual = 0xFFFFFFFF; //reset
                    myobj = myobj4; //<-backup
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 15) {
                    //if<
                    x3 = CON_gv(npcbin[x].l3);
                    x2 = CON_gv(npcbin[x].l2);
                    if (x2 < x3) {
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 16) {
                    //if>
                    x3 = CON_gv(npcbin[x].l3);
                    x2 = CON_gv(npcbin[x].l2);
                    if (x2 > x3) {
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 17) {
                    //if<=
                    x3 = CON_gv(npcbin[x].l3);
                    x2 = CON_gv(npcbin[x].l2);
                    if (x2 <= x3) {
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }
                if (npcbin[x].l1 == 18) {
                    //if>=
                    x3 = CON_gv(npcbin[x].l3);
                    x2 = CON_gv(npcbin[x].l2);
                    if (x2 >= x3) {
                        x++;
                        goto nextinstruction;
                    }
                    x = npcbin[x].l4;
                    goto nextinstruction;
                }

                //join error messages
                //1 already in party
                //2 party is full
                //3 in different party

                //leave errors
                //1 not in party!
                //2 not near home

                if (npcbin[x].l1 == 19) {
                    //join!
                    if (tnpc->player) {
                        if (tplayer == tnpc->player) {
                            CONerr = 1;
                            goto joined;
                        } //(1)already in party
                        CONerr = 3;
                        goto joined; //(3)in different party
                    }

                    for (x2 = 1; x2 <= 7; x2++) {
                        if (tplayer->party[x2] == NULL) {
                            //check no dead player is assigned to this slot
                            for (x3 = 0; x3 <= nresu; x3++) {
                                if (resu[x3]) {
                                    if (resu_player[x3] == tplayer) {
                                        if (resu_partymember[x3] == x2) goto joinblocked_indexused;
                                    }
                                }
                            }
                            tplayer->party[x2] = myobj;
                            tnpc->player = tplayer;
                            tnpc->order = 1;
                            tnpc->follow = tplayer->party[0];
                            tnpc->upflags |= 1; //all
                            tnpc->wait_walk = 0;
                            myobj->info &= (32655); //remove temp flag 32767-112
                            for (i = 0; i <= HIRELINGS_MAX - 1; i++) {
                                if (hirl_obj[i] == myobj) {
                                    hirl_obj[i] = 0; //if in hireling list remove
                                }
                            }
                            goto joined;
                        }
                    joinblocked_indexused:;
                    }
                    CONerr = 2; //party is full
                joined:;
                    x++;
                    goto nextinstruction;
                }

                if (npcbin[x].l1 == 27) {
                    //leave!
                    if (tnpc->player != tplayer) {
                        CONerr = 1;
                        goto leavefinished;
                    } //(1)not in party
                    for (x2 = 0; x2 <= 7; x2++) {
                        if (tplayer->party[x2]) {
                            if ((npc *) tplayer->party[x2]->more == tnpc) {
                                tnpc->pathn = 0; //NULL autopath
                                for (x3 = 0; x3 <= 31; x3++) {
                                    if (tnpc->schedule[x3].hour != 255) {
                                        //x4=pathfind(tnpc->path,tplayer->party[x2]->x,tplayer->party[x2]->y,tnpc->schedule[x3].x,tnpc->schedule[x3].y,NULL);
                                        //if ((x4>0)&&(x4<=64)) goto leavepathok;
                                        WPF_NEXTTO = 1;
                                        WPF_OBJECT = tplayer->party[x2];
                                        wpf_pathfind(tnpc->path, tplayer->party[x2]->x, tplayer->party[x2]->y,
                                                     tnpc->schedule[x3].x, tnpc->schedule[x3].y, 64, 0, 4);
                                        if (WPF_RETURN == WPF_PATHFOUND) goto leavepathok;
                                    } //!=255
                                } //x3
                                CONerr = 2;
                                goto leavefinished; //(2)not near home
                            leavepathok:

                                horsedismount(tplayer->party[x2]);

