// LHS-P4.5: part_b_slash -- CONTINUES MEGA B. WIRE + admin.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch.
// Contents: anotherslashcommand: (more /-commands: /WHO, debug backtick, etc.).
        anotherslashcommand:

            // t111
            if (easymodehostn1) {
                txtset(t2, t);
                txtucase(t2);
                txtset(t3, "`ADD");
                //    if (txtsame(t2,t3)){
                if ((txtsearch(t2, t3) == 1)) {
                    int mobnum = 0;
                    int mobcount = 0;

                    txtset(t3, "`ADD1");
                    if (txtsame(t2, t3))
                        mobnum = OBJ_SWASHBUCKLER;

                    txtset(t3, "`ADD2");
                    if (txtsame(t2, t3))
                        mobnum = OBJ_FIGHTER;

                    txtset(t3, "`ADD3");
                    if (txtsame(t2, t3))
                        mobnum = OBJ_GUARD;

                    txtset(t3, "`ADD9");
                    if (txtsame(t2, t3))
                        mobnum = OBJ_GARGOYLE_WINGED + 1024 * 3;

                    //txtsetchar(t,8); //txtaddchar(t,255);
                    //txtadd(t,"Mobs spawned!");
                    //NET_send(NETplayer,playerlist[tpl]->net,t);

                    int basex = 252; // 254
                    int basey = 448; // 448
                    int hx, hy, hi, hi2;
                    //int mi = 0;
                    //unsigned long moverflagbackup = MOVERNEW_GLOBALFLAGS;
                    //MOVERNEW_GLOBALFLAGS=0;
                    //int mobcountnew = 0;
                    //object* o;
                    //for (hi = 0; hi < mobcountnew; hi++) {
                    //OBJremove(moblistnew[hi]);
                    //OBJrelease(moblistnew[hi]);
                    //}

                    //mobcount = 0;
                    //mobcountnew = 0;

                    /*
		//bucket
		//hx=266; hy=448;
		hx=basex+12; hy=basey;
		OBJaddnew(hx+2,hy+3,OBJ_BUCKET+1024*0,0,0);//bucket
		//OBJaddnew(hx+1,hy+6,OBJ_BUCKET+1024*0,0,0);//bucket
		//mobs
		//OBJaddnew(1132,655,OBJ_SWASHBUCKLER+1024*0,0,0);
		//OBJaddnew(1135,655,OBJ_FIGHTER+1024*0,0,0);
		//hx=255; hy=448+1;
		hx=basex+2; hy=basey+1;
		for (hi2=hy; hi2 < hy+7; hi2++) {
			for (hi=hx; hi < hx+2; hi++) {
				if (mobnum == 0) {
					if (hi2 % 2 == 0)
						//moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_SWASHBUCKLER + 1024 * 0, 0, 0);
					OBJaddnew(hi,hi2,OBJ_SWASHBUCKLER+1024*0,0,0);
					//o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
					//OBJaddnew(hi,hi2,OBJ_DRAKE+1024*0,0,0);
					else
						//moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_FIGHTER + 1024 * 0, 0, 0);
					OBJaddnew(hi,hi2,OBJ_FIGHTER+1024*0,0,0);
					//o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
				//ENHANCEnewn(o, 0, 2);
				}
				else
					//moblistnew[mi] = OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);
					OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);

				//o=OBJaddnew(hi,hi2,OBJ_SWORD+1024*0,0,0);
				//ENHANCEnewn(o, 0, 4);
				mobcount++;
				//mi++;

				if (mobcount >= addmobnum)
					break;
			}

			if (mobcount >= addmobnum)
				break;
		}
		//mobcount = mi;
		//mobcountnew = mi;
		*/

                    /*
		hx=basex+12; hy=basey;
		OBJaddnew(hx+1,hy+1,OBJ_DEAD_BODY+1024*1,0,0);//bucket
		OBJaddnew(hx+2,hy+1,OBJ_DEAD_BODY+1024*2,0,0);//bucket
		OBJaddnew(hx+3,hy+1,OBJ_DEAD_BODY+1024*3,0,0);//bucket
		OBJaddnew(hx+1,hy+2,OBJ_DEAD_BODY+1024*4,0,0);//bucket
		OBJaddnew(hx+2,hy+2,OBJ_DEAD_BODY+1024*5,0,0);//bucket
		OBJaddnew(hx+3,hy+2,OBJ_DEAD_BODY+1024*6,0,0);//bucket
		OBJaddnew(hx+1,hy+3,OBJ_WOMAN+1024*0,0,0);//bucket
		OBJaddnew(hx+2,hy+3,OBJ_MAGE+1024*0,0,0);//bucket
		OBJaddnew(hx+3,hy+3,OBJ_PEASANT+1024*0,0,0);//bucket
		*/

                    int arenaid = getarenaid(playerlist[tpl]);
                    hx = basex + 12;
                    hy = basey;
                    //OBJaddnew(hx+2,hy+3,OBJ_BUCKET+1024*0,0,0);//bucket

                    txtsetchar(t, 8); // status msg

                    //mobs
                    if (arenaid >= 0) {
                        hx = arenalocx[arenaid] + 2;
                        hy = arenalocy[arenaid] + 1;
                        //object* mmm;
                        //creature* ccc;

                        for (hi2 = hy; hi2 < hy + 7; hi2++) {
                            for (hi = hx; hi < hx + 2; hi++) {
                                if (mobnum == 0) {
                                    if (hi2 % 2 == 0)
                                        //moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_SWASHBUCKLER + 1024 * 0, 0, 0);
                                        OBJaddnew(hi, hi2, OBJ_SWASHBUCKLER + 1024 * 0, 0, 0);
                                        //o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
                                        //OBJaddnew(hi,hi2,OBJ_DRAKE+1024*0,0,0);
                                    else
                                        //moblistnew[mi] = OBJaddnew(hi, hi2, OBJ_FIGHTER + 1024 * 0, 0, 0);
                                        OBJaddnew(hi, hi2, OBJ_FIGHTER + 1024 * 0, 0, 0);
                                    //o=OBJaddnew(hi,hi2,OBJ_MAGIC_HELM+1024*0,0,0);
                                    //ENHANCEnewn(o, 0, 2);
                                } else
                                //moblistnew[mi] = OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);
                                    OBJaddnew(hi, hi2, mobnum + 1024 * 0, 0, 0);

                                //ccc = (creature*)mmm->more;
                                //ccc->al = 1;
                                //o=OBJaddnew(hi,hi2,OBJ_SWORD+1024*0,0,0);
                                //ENHANCEnewn(o, 0, 4);
                                mobcount++;
                                //mi++;

                                if (mobcount >= arenaaddmobnum[arenaid])
                                    break;
                            }

                            if (mobcount >= arenaaddmobnum[arenaid])
                                break;
                        }

                        txtadd(t, "Arena #");
                        txtnumint(t3, arenaid + 1);
                        txtadd(t, t3);
                        txtadd(t, ": ");
                        txtadd(t, "Mobs spawned!");
                    } else {
                        txtadd(t, "You must be in an arena to spawn mobs.");
                    }

                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                // t111
                txtset(t2, t);
                txtucase(t2);
                txtset(t3, "`SET");
                //    if (txtsame(t2,t3)){
                if (num = (txtsearch(t2, t3) == 1)) {
                    int l = t2->l - num - 4;
                    int arenaid = getarenaid(playerlist[tpl]);

                    txtsetchar(t, 8); // status msg

                    if (arenaid >= 0) {
                        txtadd(t, "Arena #");
                        txtnumint(t3, arenaid + 1);
                        txtadd(t, t3);
                        txtadd(t, ": ");

                        if (l > 0) {
                            txtright(t2, l);
                            num = (int) txtnum(t2);

                            if (num > 0) {
                                arenaaddmobnum[arenaid] = num;
                                txtadd(t, "Mob spawn amount set to ");
                                txtadd(t, t2);
                            }
                        } else
                            num = -1;

                        if (num <= 0) {
                            txtadd(t, "Mob spawn amount is invalid.");
                        }
                    } else {
                        txtadd(t, "You must be in an arena to set the spawn amount.");
                    }

                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t2, t);
                txtucase(t2);
                txtset(t3, "`KARMA");
                if ((txtsearch(t2, t3) == 1)) {
                    txtsetchar(t, 8); //txtaddchar(t,255);
                    txtadd(t, "Your karma is ");
                    //txtnumint(t2, tplayer->karma);
                    txtnumint(t2, playerlist[tpl]->karma);
                    txtadd(t, t2);

                    // 1076,620 to 74,84
                    //unsigned long zz = 5505098;
                    /*
		unsigned long zz = 25231719;
		txtset(t3, " Coord x=");
		txtnumint(t4, zz&1023); txtadd(t3, t4); txtadd(t3, " y=");
		txtnumint(t4, (zz>>16)&1023); txtadd(t3, t4);
		LOGadd(t3);
		*/
                    // 1330, 410
                    // 1310, 420 2nd floor view to 1st
                    // BTset(1310,420,111);
                    // OBJaddnew(1310,420,416+1024*0,0,25231719);//pointing at something // OBJ_NOTHING
                    // 1310,320 views to 359, 385
                    // 1308,411 2nd floor ladder

                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t2, t);
                txtucase(t2);
                txtset(t3, "`LOC");
                if ((txtsearch(t2, t3) == 1)) {
                    txtsetchar(t, 8); //txtaddchar(t,255);
                    txtadd(t, "Your location is x=");
                    txtnumint(t2, playerlist[tpl]->x);
                    txtadd(t, t2);
                    txtadd(t, " y=");
                    txtnumint(t2, playerlist[tpl]->y);
                    txtadd(t, t2);

                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }
            } // t111


            txtset(t2, t);
            txtucase(t2);
            txtset(t3, "WHO");
            if (txtsame(t2, t3)) {
                txtsetchar(t, 8); //txtaddchar(t,255);
                txtadd(t, inbritannia);
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;
            }

            txtset(t3, "RESYNC");
            if (txtsame(t2, t3)) {
                playerlist[tpl]->resync = 1;
                txtset(t, "?");
                t->d2[0] = 8;
                txtadd(t, "Resync request received.");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                // luteijn: let's see if this helps.
                //send time
                txtset(t2, "?");
                t2->d2[0] = 32;
                txtset(t3, "12345678");
                t3->dd[0] = btime2;
                txtadd(t2, t3);
                NET_send(NETplayer, tplayer->net, t2);

                goto doneclmess;
            }


            txtset(t3, "/RETURNBODYFOLLOWERS");
            if (txtsame(t2, t3)) {
                //IMPORTANT NOTE: RETURNBODYFOLLOWERS MAY CAUSE SOME VISUAL INVENTORY PROBLEMS
                //ignore request if player is not dead
                for (i2 = 1; i2 <= 7; i2++) {
                    //locate player's body on resurrect list
                    for (i = 0; i <= nresu; i++) {
                        if (resu[i]) {
                            if (resu_player[i] == playerlist[tpl]) {
                                if (resu_partymember[i] == i2) {
                                    goto returnbodyfollowers_getresuindex;
                                }
                            }
                        }
                    }
                    goto returnbodyfollowers_next; //unlisted!
                returnbodyfollowers_getresuindex:
                    //remove current body
                    //IMPORTANT NOTE: IF A RECREATED OBJECT OF THE SAME TYPE USING SAME POINTER EXISTED THE FOLLOWING CHECK WOULD PRODUCE UNPREDICTABLE RESULTS
                    if (resu_body[i]->type == resu_body_type[i]) {
                        //object has not been released or reused
                        //check that resu_body[i] is not an mobj of a player
                        for (z = 0; z <= playerlist_last; z++) {
                            if (playerlist[z]) {
                                if (playerlist[z]->net) {
                                    if (playerlist[z]->mobj == resu_body[i]) playerlist[z]->mobj = NULL;
                                }
                            }
                        }
                        OBJremove(resu_body[i]);
                        OBJrelease(resu_body[i]);
                    } //resu_body[i]->type==resu_body_type[i]
                    //create a new body at death spot
                    myobj = OBJnew();
                    myobj->type = resu_body_type[i];
                    myobj->info |= 112;
                    OBJadd(resu_x[i], resu_y[i], myobj);
                    //link with new dead body
                    resu_body[i] = myobj;
                returnbodyfollowers_next:;
                } //i2
                goto doneclmess;
            }

            txtset(t3, "/RETURNBODY");
            if (txtsame(t2, t3)) {
                //IMPORTANT NOTE: RETURNBODY MAY CAUSE SOME VISUAL INVENTORY PROBLEMS
                //ignore request if player is not dead
                if (playerlist[tpl]->party[0]) goto doneclmess;
                //locate player's body on resurrect list
                for (i = 0; i <= nresu; i++) {
                    if (resu[i]) {
                        if (resu_player[i] == playerlist[tpl]) {
                            if (resu_partymember[i] == 0) {
                                goto returnbody_getresuindex;
                            }
                        }
                    }
                }
                goto doneclmess; //unlisted!
            returnbody_getresuindex:
                //remove current body
                //IMPORTANT NOTE: IF A RECREATED OBJECT OF THE SAME TYPE USING SAME POINTER EXISTED THE FOLLOWING CHECK WOULD PRODUCE UNPREDICTABLE RESULTS
                if (resu_body[i]->type == resu_body_type[i]) {
                    //object has not been released or reused
                    //check that resu_body[i] is not an mobj of a player
                    for (z = 0; z <= playerlist_last; z++) {
                        if (playerlist[z]) {
                            if (playerlist[z]->net) {
                                if (playerlist[z]->mobj == resu_body[i]) playerlist[z]->mobj = NULL;
                            }
                        }
                    }
                    OBJremove(resu_body[i]);
                    OBJrelease(resu_body[i]);
                } //resu_body[i]->type==resu_body_type[i]
                //create a new body at death spot
                myobj = OBJnew();
                myobj->type = resu_body_type[i];
                myobj->info |= 112;
                OBJadd(resu_x[i], resu_y[i], myobj);
                //link with new dead body
                resu_body[i] = myobj;
                goto doneclmess;
            }

            if (x5 && (playerlist[tpl]->party[0] == NULL)) goto dglobal3;
            if (myobj = playerlist[tpl]->party[0]) {
                txtset(t2, "KAL LOR");
                if (txtsame(t, t2)) {
                    tplayer = playerlist[tpl];
                    if ((tplayer->party[0]->x == 368) && (tplayer->party[0]->y == 386)) goto help_skip;
                    if (tplayer->kallor) goto help_skip;
                    tplayer->kallor = 32;
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "\x022KAL LOR!\x022 (You will receive help in 30 seconds.)");
                    NET_send(NETplayer, tplayer->net, t);
                help_skip:
                    goto doneclmess;
                }

            dglobal3:

                static unsigned char me_message;
                me_message = 0;
                txtset(t2, "/ME ");
                txtset(t3, t);
                txtucase(t3);
                txtleft(t3, 4);
                if (txtsame(t3, t2)) {
                    txtright(t, t->l - 4);
                    if (t->l == 0) goto doneclmess;
                    me_message = 1;
                }

                if (x5) i3 = SFnew(2047, 1023);
                else i3 = SFnew(myobj->x, myobj->y);

                //i3 points to new sf
                sf[i3].type = 3; //message to users

                if (playerlist[tpl]->party[0]) {
                    sf[i3].x = myobj->x;
                    sf[i3].y = myobj->y - 2;
                }

                if (x5) {
                    sf[i3].x = 2047;
                    sf[i3].y = 1023;
                }

                sf[i3].wait = 2;
                tnpc = NULL;
                if (playerlist[tpl]->party[0]) tnpc = (npc *) myobj->more;
                if (playerlist[tpl]->party[0]) {
                    sf[i3].more = tnpc->port;
                    //addnpcport2sf(unsigned long n,npc *tnpc)
                    //addnpcport2sf(i3,tnpc);
                } else {
                    sf[i3].more = playerlist[tpl]->port;
                }


                static txt *inouttxt;
                inouttxt = txtnew();

                if (playerlist[tpl]->party[0]) {
                    txtset(t2, tnpc->name);
                } else {
                    txtset(t2, playerlist[tpl]->npcname);
                }
                txtset(inouttxt, "");


                if (me_message) {
                    txtaddcolouredname(inouttxt, t2, playerlist[tpl]);
                    txtadd(inouttxt, " ");
                    txtadd(inouttxt, t);
                } else {
                    txtadd(t2, ":");
                    txtaddcolouredname(inouttxt, t2, playerlist[tpl]);
                    txtadd(inouttxt, " \x022");
                    txtadd(inouttxt, t);
                    txtadd(inouttxt, "\x022");
                }

                sf[i3].p = inouttxt;
                //txtset(t,"message ["); txtadd(t,inouttxt); txtadd(t,"] ("); txtnumint(t2,ett); txtadd(t,t2); txtadd(t,")"); txtfileout(t,log2);
                if (tnpc) {
                    txtset(t, "Message:");
                    txtadd(t, tnpc->name);
                    txtadd(t, ":");
                    txtadd(t, inouttxt);
                    LOGadd(t);
                }
            }
            goto doneclmess;
        } //6

        if (t->d2[0] == 34) {
            //chat
            playerlist[tpl]->typing = 0;
            playerlist[tpl]->idle = 0;
            if (playerlist[tpl]->party[0] == NULL) goto doneclmess;
            if (myobj = (object *) playerlist[tpl]->talk_target->more) {
                //check if target is onscreen
                //tpx=playerlist[tpl]->x-15; tpy=playerlist[tpl]->y-11; if (tpx<0) tpx=0; if (tpy<0) tpy=0; if (tpx>2016) tpx=2016; if (tpy>1000) tpy=1000;
                getscreenoffset(playerlist[tpl]->x, playerlist[tpl]->y, &tpx, &tpy);


                x = myobj->x;
                y = myobj->y;
                if ((x >= tpx) && (y >= tpy) && (x <= (tpx + 31)) && (y <= (tpy + 23))) {
                    x = x - tpx;
                    y = y - tpy;
                    txtright(t, t->l - 1);
                    goto chat;
                } //x,y check
                txtset(t, "?");
                t->d2[0] = 8;
                txtadd(t, "You cannot chat to people you cannot see.");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;
            } //->more
            txtset(t, "?");
            t->d2[0] = 8;
            txtadd(t, "Chatting to yourself again?");
            NET_send(NETplayer, playerlist[tpl]->net, t);
            goto doneclmess; //no talk target or invalid talk target
        } //34

        if (t->d2[0] == 7) {
            //text message to NPC/player (NPC message)
            playerlist[tpl]->typing = 0;
            playerlist[tpl]->idle = 0;
            if (playerlist[tpl]->party[0] == NULL) goto doneclmess;

            txtright(t, t->l - 1);
            x = t->ds[0] / 32;
            y = t->ds[1] / 32;
            txtright(t, t->l - 4);

        chat:

            //tpx=playerlist[tpl]->x-15; tpy=playerlist[tpl]->y-11; if (tpx<0) tpx=0; if (tpy<0) tpy=0; if (tpx>2016) tpx=2016; if (tpy>1000) tpy=1000;
            getscreenoffset(playerlist[tpl]->x, playerlist[tpl]->y, &tpx, &tpy);

            myobj = OBJfindlast(tpx + x, tpy + y);
            if (!myobj) myobj = OBJfindlastall(tpx + x, tpy + y);
            if (myobj) {
                if (myobj->info & 8) myobj = (object *) myobj->more;


                static unsigned char npcmess_shrine = 0;
                npcmess_shrine = 0;
                if (myobj->type == OBJ_FORCE_FIELD) {
                    //force field
                    myobj2 = (object *) myobj->prev;
                    if (playerlist[tpl]->GNPCflags[3] & (1 << (myobj2->type >> 10)))
                        npcmess_shrine = 190 + (myobj2->type >> 10);
                } //force field
                if ((myobj->type & 1023) == OBJ_WISP) npcmess_shrine = 199; //wisp
                if ((myobj->type & 1023) == OBJ_STOCKS) {
                    if ((myobj->type >> 11) & 1) {
                        npcmess_shrine = 179; //man in stocks
                    }
                }
                if ((myobj->type & 1023) == OBJ_SHRINE) {
                    myobj2 = (object *) myobj->prev;
                    if ((myobj2->type & 1023) == 329) npcmess_shrine = 198; //alter of singularity
                }
                if ((myobj->type & 1023) == OBJ_GUARD) {
                    if (myobj->info & 4) {
                        //crt
                        crt = (creature *) myobj->more;
                        if (crt->al == 4) npcmess_shrine = 200; //guard
                    } //4
                } //382
                if (npcmess_shrine) goto npcmess_shrine1;


                //assume NPC type
                if ((myobj->info & 2) == 0) goto talk_skipnullobject;
                tnpc = (npc *) myobj->more;
                if (tnpc == NULL) goto talk_skipnullobject;

                //PRIVATE MESSAGE TO CLOSE PLAYER?
                if (tnpc->player) {
                    if (tnpc->player == playerlist[tpl]) {
                        //send message to OUR OWN PARTY MEMBER
                        if (playerlist[tpl]->party[playerlist[tpl]->selected_partymember] != myobj)
                            goto
                                    talk_skipprivatemessage; //to SELECTED PARTY MEMBER
                        goto talk_privatemessage;
                    }

                    if (tnpc->player->party[0]) {
                        if ((npc *) tnpc->player->party[0]->more == tnpc) {
                            //party leader
                            //send a SPECIAL sfx buffer to a single player (can be tested by clicking on oneself)
                        talk_privatemessage:

                            static unsigned char me_message;
                            me_message = 0;
                            txtset(t2, "/ME ");
                            txtset(t3, t);
                            txtucase(t3);
                            txtleft(t3, 4);
                            if (txtsame(t3, t2)) {
                                txtright(t, t->l - 4);
                                me_message = 1;
                            }

                            if (t->l) {
                                tnpc2 = (npc *) playerlist[tpl]->party[0]->more;
                                txtset(t4, tnpc2->name);
                                txtset(t3, "");

                                if (me_message) {
                                    txtaddcolouredname(t3, t4, playerlist[tpl]);
                                    txtadd(t3, " ");
                                    txtadd(t3, t);
                                } else {
                                    txtadd(t4, ":");
                                    txtaddcolouredname(t3, t4, playerlist[tpl]);
                                    txtadd(t3, " \x022");
                                    txtadd(t3, t);
                                    txtadd(t3, "\x022");
                                }


                                //txtadd(t3,": \x022");
                                //txtadd(t3,t);
                                //txtadd(t3,"\x022");
                                txtset(t, "?");
                                t->d2[0] = 2;
                                txtset(t2, "??");
                                t2->ds[0] = 1;
                                txtadd(t, t2);
                                t2->ds[0] = 3;
                                txtadd(t, t2);
                                t2->ds[0] = 2048;
                                txtadd(t, t2);
                                t2->ds[0] = 1023;
                                txtadd(t, t2);
                                t2->ds[0] = t3->l;
                                txtadd(t, t2);
                                t2->ds[0] = 0;
                                txtadd(t, t2); //unused
                                t2->ds[0] = tnpc2->port;
                                txtadd(t, t2); //more=port
                                txtset(t2, "????");
                                t2->df[0] = 2;
                                txtadd(t, t2); //wait
                                txtadd(t, t3);
                                NET_send(NETplayer, tnpc->player->net, t);
                                if (tnpc->player == playerlist[tpl]) {
                                    txtset(t, "?");
                                    t->d2[0] = 8;
                                    txtadd(t, "Talking to yourself?");
                                    NET_send(NETplayer, playerlist[tpl]->net, t);
                                } else {
                                    NET_send(NETplayer, playerlist[tpl]->net, t);
                                }
                            } //t->l
                            goto doneclmess;
                        }
                    }
                }
            talk_skipprivatemessage:

                if ((abs(myobj->x - playerlist[tpl]->party[0]->x) > 8) || (
                        abs(myobj->y - playerlist[tpl]->party[0]->y) > 8)) {
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "Out of range!");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                /*
        if (OBJcheckbolt(playerlist[tpl]->party[0]->x,playerlist[tpl]->party[0]->y,myobj->x,myobj->y)){
        txtset(t,"?"); t->d2[0]=8; txtadd(t,"Blocked!"); NET_send(NETplayer,playerlist[tpl]->net,t);
        goto doneclmess;
        }
        */


                x4 = 1; //bedswap2
                if (tnpc->schedule) {
                    if (tnpc->schedule_i != -1) { if (tnpc->schedule[tnpc->schedule_i].type == 0x91) { x4 = 0; } }
                }

                if ((tnpc->converse != 120) && (tnpc->converse != 121) && (tnpc->converse != 122) && (
                        tnpc->converse != 123)) {
                    //allow talking to sick warriors in Cove
                    if (x4 == 0) {
                        txtset(t, "?");
                        t->d2[0] = 8;
                        txtadd(t, "No response!");
                        NET_send(NETplayer, playerlist[tpl]->net, t);
                        goto doneclmess;
                    } //No response!
                }

                //turn and face talker and begin "listening!"
                if (myobj != playerlist[tpl]->party[0])
                    OBJdir(myobj, playerlist[tpl]->party[0]->x,
                           playerlist[tpl]->party[0]->y);

                if (tnpc->player == NULL) {
                    //not a party member!
                    if (tnpc->wait_walk < 16.0f) tnpc->wait_walk = 16.0f;
                }

                CONnpc2 = tnpc; //only valid if #converse is derived from an NPC pointer!

            npcmess_shrine1:
                tplayer = playerlist[tpl];

                //set as talk target!
                VLNKremove(tplayer->talk_target);
                VLNKsremove(tplayer->talk_target);
                tplayer->talk_target->more = myobj;
                VLNKnew(tplayer->talk_target, myobj, (unsigned long) &tplayer->talk_target->more);


                //RESET temp flags
                ZeroMemory(&CONreg, sizeof(CONreg));
                CONerr = 0;
                CONnumber = 0;
                CONqual = 0xFFFFFFFF;
                CONport = 0xFFFFFFFF;
                CONhousecost = 0;
                CONhouseinitialcost = 0;
                CONnumber = txtnum(t); //NUMBER
                if (CONnumber < 0) CONnumber = 0;
                if (CONnumber > 32767) CONnumber = 32767;


                CONpartymember = 0;
                for (x2 = 0; x2 <= 7; x2++) {
                    if (tplayer->party[x2]) {
                        txtset(t2, t);
                        txtucase(t2);
                        tnpc3 = (npc *) tplayer->party[x2]->more;
                        txtset(t3, tnpc3->name);
                        txtucase(t3);
                        if (txtsame(t2, t3)) {
                            CONpartymember = x2 + 1; //(1-8)
                        }
                    }
                }


                txtset(t9, t);

                if (t->l == 0) txtset(t, "look"); //default

                //remove strange characters from t!
