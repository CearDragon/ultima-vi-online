// LCS-P4.4-4.7: part_net — network read/dispatch chunk of the in-game block.
// LCS brace-seam: CONTINUES the in-game `{ ... }` block (opened in game_open,
// closed in panel_draw). Contains the CLIENT_readnext: read loop, the
// scene_update_message: ground-object (sobj) decode, the mover decode
// (changestate:/getnbits3:/mover_*), and the rest through CLIENT_donemess: and
// the `} //read local message` close.
// ⚠️ WIRE-COUPLED (sobj y*48+x / mover MV_TX_*; tpx_legacy/tpy_legacy shim).
// Move only — do not touch encode/decode, no STL, do NOT bump U6O_VERSION.
// See .github/copilot-instructions.md §Wire protocol and the DOB plan.


//null SF message to avoid errors
txtset(mess_SF, "??");
mess_SF
->
d2 [0]=
0;
mess_SF
->
d2 [1]=
0;


//if (NEThost==NULL){ //client


CLIENT_readnext : x3
=
0;
i=
0;
if
(socketclient_ri[i]
->
d [socketclient_ri[i]
->
next
]
->
l
)
{ //buffered data available
    txtset(t,socketclient_ri[i]->d[socketclient_ri[i]->next]);
    txtNEWLEN(socketclient_ri[i]->d[socketclient_ri[i]->next],0);
    socketclient_ri[i]->next++;
    x3=1;
  }//->l


if
(x3) {
    //U6Ohostlink1:

    if (t->l != 0) {
        if (t->d2[0] == 1) {
            scrlog("Time update via message 1. DANGER WILL ROBINSON DANGER");
            txtset(mess_UPDATEps, t);
            txtright(mess_UPDATEps, mess_UPDATEps->l - 1);
            if (!NEThost) btime2 = mess_UPDATEps->df[0];
            txtright(mess_UPDATEps, mess_UPDATEps->l - 4);

            goto CLIENT_donemess;
        }

        if (t->d2[0] == 2) {
            if (nodisplay) goto CLIENT_donemess;

            txtset(mess_SF, t);
            txtright(mess_SF, mess_SF->l - 1);
            //SF (mess_SF) -> local SF
            i5 = mess_SF->d2[0] + (mess_SF->d2[1] << 8);
            i4 = 2; //message offset
            for (i = 0; i < i5; i++) {
                for (i3 = 0; i3 <= 255; i3++) {
                    if (sfx[i3].type == 0) {
                        sfx[i3].type = mess_SF->d2[i4] + (mess_SF->d2[i4 + 1] << 8);
                        i4 += 2;
                        sfx[i3].x = mess_SF->d2[i4] + (mess_SF->d2[i4 + 1] << 8);
                        i4 += 2;
                        sfx[i3].y = mess_SF->d2[i4] + (mess_SF->d2[i4 + 1] << 8);
                        i4 += 2;
                        sfx[i3].x2 = mess_SF->d2[i4] + (mess_SF->d2[i4 + 1] << 8);
                        i4 += 2;
                        sfx[i3].y2 = mess_SF->d2[i4] + (mess_SF->d2[i4 + 1] << 8);
                        i4 += 2;
                        sfx[i3].more = mess_SF->d2[i4] + (mess_SF->d2[i4 + 1] << 8);
                        i4 += 2;
                        sfx[i3].wait2[0] = mess_SF->d2[i4];
                        i4++;
                        sfx[i3].wait2[1] = mess_SF->d2[i4];
                        i4++;
                        sfx[i3].wait2[2] = mess_SF->d2[i4];
                        i4++;
                        sfx[i3].wait2[3] = mess_SF->d2[i4];
                        i4++;
                        if (sfx[i3].type == 3) {
                            //get message
                            static txt *dbgt5;
                            dbgt5 = txtnew(); //NEW TXT ALLOCATED

                            txtNEWLEN(dbgt5, sfx[i3].x2);
                            memcpy(&dbgt5->d2[0], &mess_SF->d2[i4], sfx[i3].x2);
                            i4 += sfx[i3].x2;
                            sfx[i3].p = dbgt5;
                            txtset(t, (txt *) sfx[i3].p);
                            txtset(t2, "?");
                            t2->d2[0] = 92;
                            z = txtsearch(t, t2);
                            if (z == 0) z = t->l;
                            else z--;
                            f = (float) z / 10.0f;
                            if (f < 1)f = 1;
                            if (f > 4)f = 4;
                            sfx[i3].wait = f;
                            //new text message
                            //check for and elimate position conflicts
                            if ((sfx[i3].x == 2047) && (sfx[i3].y == 1023)) goto shiftdown_skip;
                            if ((sfx[i3].x == 2048) && (sfx[i3].y == 1023)) goto shiftdown_skip;
                        shiftdown:
                            for (i2 = 0; i2 <= 255; i2++) {
                                if ((sfx[i2].type == 3) && (i2 != i3)) {
                                    if ((sfx[i3].y == sfx[i2].y) || (sfx[i3].y == (sfx[i2].y + 1)) || (
                                            sfx[i3].y == (sfx[i2].y - 1))) {
                                        sfx[i3].y = sfx[i2].y + 2;
                                        goto shiftdown;
                                    }
                                }
                            }
                        shiftdown_skip:;

                            //***** PLAYER MESSAGE MUTING AND IGNORING *****
                            if (sfx[i3].more >= 194) {
                                //player port range
                                txtset(t3, ": \"");
                                if (x2 = txtsearch((txt *) sfx[i3].p, t3)) {
                                    txtset(t4, (txt *) sfx[i3].p);
                                    txtleft(t4, x2 - 1);
                                    txtucase(t4);
                                    if (t4->d2[0] == 6) {
                                        //check if a coloured name header exists, and if so remove it
                                        txtright(t4, t4->l - 5);
                                    }
                                    //exact search needed
                                    tfh = open2("ignore.txt", OF_READWRITE | OF_SHARE_COMPAT); //log
                                ignore_nextplayer:
                                    txtfilein(t6, tfh);
                                    txtucase(t6);
                                    txtset(t7, "[MUTE]");
                                    txtadd(t7, t4);
                                    if (txtsame(t6, t7)) {
                                        txtleft((txt *) sfx[i3].p, x2 - 1);
                                        txtadd((txt *) sfx[i3].p, ": \"...\"");
                                        goto muted;
                                    }
                                    txtset(t7, "[IGNORE]");
                                    txtadd(t7, t4);
                                    if (txtsame(t6, t7)) {
                                        sfx[i3].type = 0;
                                        goto ignored;
                                    }
                                    if (t6->l) goto ignore_nextplayer;
                                    close(tfh);
                                } else {
                                    txtset(t4, (txt *) sfx[i3].p);
                                    txtucase(t4);

                                    tfh = open2("ignore.txt", OF_READWRITE | OF_SHARE_COMPAT); //log
                                ignore_nextplayerspace:
                                    txtfilein(t6, tfh);
                                    txtucase(t6); //read file line

                                    txtset(t5, "[MUTE]");
                                    txtadd(t5, t4);
                                    txtleft(t5, t6->l);
                                    if (txtsame(t5, t6)) {
                                        txtleft((txt *) sfx[i3].p, t5->l - 6);
                                        txtadd((txt *) sfx[i3].p, " ...");
                                        goto muted;
                                    }
                                    txtset(t5, "[IGNORE]");
                                    txtadd(t5, t4);
                                    txtleft(t5, t6->l);
                                    if (txtsame(t5, t6)) {
                                        sfx[i3].type = 0;
                                        goto ignored;
                                    }
                                    if (t6->l) goto ignore_nextplayerspace;
                                    close(tfh);
                                } //no speech marks
                            } //player port range
                        muted:;
                        ignored:;
                        } //text special effect
                        goto mess_SFok;
                    }
                } //i3, type==0
            mess_SFok:;
            } //i

            goto CLIENT_donemess;
        }

        // s333 updates (no changes; ignore this)
        if (t->d2[0] == 3) {
            i2 = t->d2[1]; //party member index
            x4 = t->ds[1]; //update flags
            txtright(t, t->l - 4);
            //1		name, portriat, str, dex, int, lev, hp_max, mp_max (all)
            //2		hp
            //4     mp
            //8     xp
            //16    weight (automatically set if inventory/equipped items change)
            //32    inventory, bag icon
            //64    equipped items


            tplay->party[i2] = tplayobj[i2];


            tnpc = (npc *) tplay->party[i2]->more;
            tnpc->upflags |= x4;


            if (x4 & 32) {
                x3 = t->d2[0]; //no-objs+up/down arrows
                txtright(t, t->l - 1);
                x5 = x3;
                x3 &= 31; //clear arrow flags, num of items
                x2 = 0;
                myobj = tnpc->items;
                myobj->more = NULL; //clear old data (in case these is no new data)
                tnpc->baseitem = tnpc->items;
                tnpc->baseitem_offset = 0;
                if (x5 & 64) {
                    //up arrow
                    tnpc->baseitem_offset = 4;
                    for (x = 0; x <= 3; x++) {
                        myobj2 = tobj_i[i2][x2];
                        ZeroMemory(tobj_i[i2][x2], sizeof(object));
                        if (x2 == 0) {
                            myobj->more = myobj2;
                        } else {
                            myobj->next = myobj2;
                        }
                        x2++;
                        myobj = myobj2;
                    }
                }

            dbg1:
                if ((x2 - tnpc->baseitem_offset) < x3) {
                    x = t->ds[0];
                    i5 = 2;
                    myobj2 = tobj_i[i2][x2];
                    ZeroMemory(tobj_i[i2][x2], sizeof(object));
                    myobj2->type = x;
                    if (obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].flags & 4096) {
                        //temp quantity
                        txtright(t, t->l - 2);
                        myobj2->more2 = t->ds[0];
                    } else if ((myobj2->type & 1023) == 64) {
                        //key
                        txtright(t, t->l - 2);
                        myobj2->more2 = t->ds[0];
                    } else if (obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].v1 || //v1 (damage) + info
                               obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].v2 || (myobj2->type & 1023) ==
                               448) {
                        //v2 (armour) + info and horse papers + info
                        txtright(t, t->l - 2);
                        myobj2->more2 = t->d2[0];
                        i5 = 1;
                    }

                    if (x2 == 0) {
                        myobj->more = myobj2;
                    } else {
                        myobj->next = myobj2;
                    }
                    myobj = myobj2;
                    x2++;
                    txtright(t, t->l - i5);
                    goto dbg1;
                }
                if (x5 & 128) {
                    //down arrow
                    myobj2 = tobj_i[i2][x2];
                    ZeroMemory(tobj_i[i2][x2], sizeof(object));
                    myobj->next = myobj2;
                    x2++;
                }

                if (x5 & 32) {
                    //bag
                    x = t->ds[0];
                    txtright(t, t->l - 2);
                    myobj2 = tobj_i[i2][x2];
                    ZeroMemory(tobj_i[i2][x2], sizeof(object));
                    myobj2->type = x;
                    myobj2->more = tnpc->items->more;
                    tnpc->items->more = myobj2;
                    tnpc->baseitem = myobj2;
                }
            } //x4&32


            if (x4 & 64) {
                x = t->ds[0];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][0];
                    myobj->type = x;
                }
                tnpc->helm = myobj;
                x = t->ds[1];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][1];
                    myobj->type = x;
                }
                tnpc->wep_right = myobj;
                x = t->ds[2];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][2];
                    myobj->type = x;
                }
                tnpc->wep_left = myobj;
                x = t->ds[3];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][3];
                    myobj->type = x;
                }
                tnpc->armour = myobj;
                x = t->ds[4];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][4];
                    myobj->type = x;
                }
                tnpc->boots = myobj;
                x = t->ds[5];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][5];
                    myobj->type = x;
                }
                tnpc->neck = myobj;
                x = t->ds[6];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][6];
                    myobj->type = x;
                }
                tnpc->ring_right = myobj;
                x = t->ds[7];
                myobj = NULL;
                if (x != 0) {
                    myobj = tobj_e[i2][7];
                    myobj->type = x;
                }
                tnpc->ring_left = myobj;
                tnpc->wt2 = t->ds[8];
                txtright(t, t->l - 18);
            } //x4&64

            if (x4 & 1) {
                //1		name, portriat, str, dex, int, lev, hp_max, mp_max (does not change often)
                x = t->d2[0];
                txtNEWLEN(tnpc->name, x);
                memcpy(&tnpc->name->d2[0], &t->d2[1], x);

                if (i2 == 0) txtset(namelast, tnpc->name);
                txtright(t, t->l - (x + 1));
                tnpc->port = t->ds[0];
                if (i2 == 0) portlast = tnpc->port;

                tnpc->s = t->ds[1];
                tnpc->d = t->ds[2];
                tnpc->i = t->ds[3];
                tnpc->lev = t->ds[4];
                if ((i2 == 0) && (tnpc->lev > 1)) U6O_WALKTHRU = FALSE;
                tnpc->hp_max = t->ds[5];
                tnpc->mp_max = t->ds[6];
                tnpc->wt_max = t->ds[7];
                tnpc->wt2_max = t->ds[8];
                txtright(t, t->l - 18);
            } //x4&1


            if (x4 & 16) {
                //wt
                tnpc->wt = t->ds[0];
                txtright(t, t->l - 2);
            } //x4&16
            if (x4 & 2) {
                //hp
                // s333 hp changed (no changes; ignore this)
                /*
		  if (combatinfo) {
			  int hp = tnpc->hp;
			  int newhp = t->ds[0];
			  //txtset(t2, "hp: ");
			  //txtnumint(t3, hp);
			  //txtadd(t2, t3);
			  //txtadd(t2, " // new hp: ");
			  //txtnumint(t3, newhp);
			  //txtadd(t2, t3);
			  //LOGadd(t2);
			  if (newhp < hp) {
				  txtset(t3, "You lost ");
				  txtnumint(t4, hp - newhp);
				  txtadd(t3, t4);
				  txtadd(t3, " hp");
				  STATUSMESSadd(t3);
			  }
		  }
		  */

                tnpc->hp = t->ds[0];
                txtright(t, t->l - 2);
            } //x4&2
            if (x4 & 4) {
                //mp
                x = tnpc->mp;
                client_spellwait[i2] = 0;
                tnpc->mp = t->ds[0];
                txtright(t, t->l - 2);
                if ((x4 & 1) == 0) {
                    //not setting up a new npc
                    //if (tnpc->mp<x) client_spellwait[i2]=1;
                    // f222 fix sometimes spellwait not getting triggered
                    if (enhancen1) {
                        if (tnpc->mp <= x) client_spellwait[i2] = 1;
                    } else {
                        // original
                        if (tnpc->mp < x) client_spellwait[i2] = 1;
                    }
                } //(x4&1)==0
            } //x4&4
            if (x4 & 8) {
                //xp
                // s333 combat info xp change
                if (combatinfo) {
                    x = tnpc->exp;
                    txtsetchar(t3, 255); // make combat info status messages skippable
                    txtadd(t3, "I: ");
                    txtadd(t3, tnpc->name);

                    if (x < t->dl[0])
                        txtadd(t3, " has gained ");
                    else if (x > t->dl[0])
                        txtadd(t3, " has lost ");

                    txtnumint(t4, abs((int) (t->dl[0] - x)));
                    txtadd(t3, t4);
                    txtadd(t3, " experience.");
                    STATUSMESSadd(t3);
                }
                tnpc->exp = t->dl[0];
                txtright(t, t->l - 4);
            } //x4&8


            if (x4 & 128) {
                //spellbook
                ZeroMemory(&spell[i2][0], 512);
                x = t->d2[0];
                txtright(t, t->l - 1); //number of spells


                for (x2 = 0; x2 < x; x2++) {
                    x3 = t->d2[0];
                    txtright(t, t->l - 1); //spell type


                    spell[i2][x3] = t->ds[0];
                    txtright(t, t->l - 2); //cast!
                }
                if (x) {
                    if (party_spellbook_frame[i2]->positioned == false) {
                        // RW-P3.3 (2026-06-02): first-show placement via FRAME::positioned
                        // + clamp to the current back buffer (see minimap/tmap). This block
                        // only runs when the character has spells (the panel is shown), so
                        // park it at its home position.
                        int hx = i2 * 32, hy = 128 - 16 + i2 * 32 + 256; //default
                        if (cltset2_restored) {
                            if (cltset2.party_spellbook_frame_offset_x[i2] != 32767) {
                                hx = cltset2.party_spellbook_frame_offset_x[i2];
                                hy = cltset2.party_spellbook_frame_offset_y[i2];
                                cltset2.party_spellbook_frame_offset_x[i2] = 32767;
                            }
                        }
                        placeFloatingPanelFirstShow(party_spellbook_frame[i2], hx, hy, 1);
                    }


                    if (party_spellbook_frame[i2]->offset_x > kPanelHideThresholdX)
                        party_spellbook_frame[i2]->offset_x -= kPanelHideDeltaX;
                } else {
                    if (party_spellbook_frame[i2]->offset_x < kPanelHideThresholdX)
                        party_spellbook_frame[i2]->offset_x += kPanelHideDeltaX;
                }

                //check spellbookpage! (if invalid select first spellbook page only!)

                for (x2 = 0; x2 <= 15; x2++) {
                    if (spell[i2][((spellbook_page[i2] - 1) << 4) + x2]) goto validpage;
                }
                for (x2 = 0; x2 <= 255; x2++) {
                    if (spell[i2][x2]) {
                        spellbook_page[i2] = (x2 >> 4) + 1;
                        goto validpage;
                    }
                }
            validpage:;
            } //spellbook


            goto CLIENT_donemess;
        }

        // r333 this is where mouse/cursor object is set (displaying it is done somewhere else); no changes are made here
        if (t->d2[0] == 4) {
            //set mouse object
            txtright(t, t->l - 1);
            static object *mobj_local = NULL;
            if (mobj_local == NULL) mobj_local = OBJnew_local();
            mobj_local->type = t->ds[0];
            CLIENTplayer->mobj = mobj_local;
            if (t->ds[0] == 0xFFFF) CLIENTplayer->mobj = NULL;
            /*
		txtset(t2, "offsetx ");
		txtnumint(t3, party_frame[0]->offset_x);
		txtadd(t2, t3);
		txtadd(t2, " // offsety ");
		txtnumint(t3, party_frame[0]->offset_y);
		txtadd(t2, t3);
		LOGadd(t2);
		*/


            goto CLIENT_donemess;
        }

        //8 recieve status message (white text at bottom of the screen, not conversation)
        if (t->d2[0] == 8) {
            txtright(t, t->l - 1);
            STATUSMESSadd(t);


            // r888 food status
            checkstatusmessage = 1;

            if (checkstatusmessage) {
                //txtset(t2,t); txtucase(t2);
                txtset(t3, "is hungry!");
                //      if (txtsame(t2,t3)){
                if ((txtsearch(t, t3) > 0)) {
                    checkstatusmessage = 0;
                    //MessageBox(NULL,"hungry!","Ultima 6 Online",MB_OK);
                    foodstatus = 1;
                }
            }

            if (checkstatusmessage) {
                txtset(t3, "eats the");
                if ((txtsearch(t, t3) > 0)) {
                    checkstatusmessage = 0;
                    foodstatus = 2;
                }
            }

            if (checkstatusmessage) {
                txtset(t3, "drinks the");
                if ((txtsearch(t, t3) > 0)) {
                    checkstatusmessage = 0;
                    foodstatus = 2;
                }
            }

            if (checkstatusmessage) {
                txtset(t3, "full!");
                if ((txtsearch(t, t3) > 0)) {
                    checkstatusmessage = 0;
                    foodstatus = 3;
                }
            }


            goto CLIENT_donemess;
        }

        if (t->d2[0] == 10) {
            //type 10: get U6OID info


            txtright(t, t->l - 1);
            tu6oid = t->dl[0];
            txtright(t, t->l - 4);

            //scan list of U6OIDs, add name
            if (idlstn != -1) {
                for (z2 = 0; z2 <= idlstn; z2++) {
                    if (tu6oid == idlst[z2]) {
                        x = t->d2[0];
                        txtright(t, t->l - 1);
                        txtset(idlst_name[z2], t);
                        txtleft(idlst_name[z2], x);
                        txtright(t, t->l - x);
                        idlst_namecolour[z2] = t->dl[0] & 0xFFFFFF;
                        txtright(t, t->l - 3);
                    getid_more:
                        if (t->l) {
                            if (t->d2[0] == 1) {
                                txtright(t, t->l - 1);

                                goto getid_more;
                            } //1
                            //...
                        } //t->l

                        //store MY ID
                        if (tplay->party[0]) {
                            tnpc3 = (npc *) tplay->party[0]->more;
                            if (txtsame(idlst_name[z2], tnpc3->name)) {
                                clientplayerid = idlst[z2];
                                namecolour = idlst_namecolour[z2];
                                tnpc3->upflags |= 1;
                            }
                        }
                    } //==
                } //z2
            } //!=-1
            goto CLIENT_donemess;
        } //10

        if (t->d2[0] == 11) {
            //type 11: set background midi
            if (U6O_DISABLEMUSIC) goto CLIENT_donemess;
            if (midi_background == 0) {
                //stop playing intro immediately
                u6omidi->Stop(); //stop playing background midi
            isplayingwait3:
                if (u6omidi->IsPlaying() == S_OK) goto isplayingwait3;
            }

            midi_background = t->d2[1];
            goto CLIENT_donemess;
        } //11

        if (t->d2[0] == 30) {
            //type 30: set foreground midi & midi_foreground_wait
            midi_foreground = t->d2[1];
            midi_foreground_wait = t->ds[1];
            goto CLIENT_donemess;
        } //30

        if (t->d2[0] == 14) {
            //type 14: set wind
            windx2 = (t->d2[1] & 3) - 1;
            windy2 = (t->d2[1] >> 2) - 1;
            goto CLIENT_donemess;
        } //14

        if (t->d2[0] == 18) {
            //type 18: client_spellwait returned to 0
            if (tplay->party[t->d2[1]]) {
                client_spellwait[t->d2[1]] = 0;
                tnpc = (npc *) tplay->party[t->d2[1]]->more;
                tnpc->upflags |= 4;
            }
            goto CLIENT_donemess;
        } //18

        if (t->d2[0] == 22) {
            //type 22: play endgame sequence!
            endgame = 1;
            goto CLIENT_donemess;
        } //22

        if (t->d2[0] == 23) {
            //type 23: set staff cast cursor
            txtright(t, t->l - 1);
            userkey = 4;
            userspell = t->ds[0];
            userspellbook = 16; //+8==24 staff override value!
            if (spelltarget[userspell] == 1) {
                txtset(t, "?");
                t->d2[0] = 15;
                NET_send(NETplayer, NULL, t); //keyboard targeting request
                ktar_display = 2.0f;
            }
            goto CLIENT_donemess;
        } //23

        if (t->d2[0] == 24) {
            //type 24: play instrument!
            clientinstrument = t->d2[1];

            playinstrument = 1;

            if (clientinstrument == 0) STATUSMESSadd("You begin playing the lute. (Press ESC when finished)");
            if (clientinstrument == 1) STATUSMESSadd("You begin playing the harpsichord. (Press ESC when finished)");
            if (clientinstrument == 2) STATUSMESSadd("You begin playing the harp. (Press ESC when finished)");
            if (clientinstrument == 3) STATUSMESSadd("You begin playing the panpipes. (Press ESC when finished)");
            if (clientinstrument == 4) STATUSMESSadd("You begin playing the xylophone. (Press ESC when finished)");

            if (musickeyboard->offset_x >= kPanelHideThresholdX) musickeyboard->offset_x -= kPanelHideDeltaX;
            goto CLIENT_donemess;
        } //24

        if (t->d2[0] == 27) {
            //type 27: play MIDI note(s): client
            for (x = 2; x < t->l; x++) {
                mididown(t->d2[1], t->d2[x]);
            }

            goto CLIENT_donemess;
        } //27

        if (t->d2[0] == 28) {
            //type 28: stop MIDI note(s): client
            for (x = 2; x < t->l; x++) {
                midiup(t->d2[1], t->d2[x]);
            }
            goto CLIENT_donemess;
        } //28

        if (t->d2[0] == 29) {
            //type 29: stop playing instrument
            if (playinstrument) {
                if (clientinstrument == 0) STATUSMESSadd("You finish playing the lute.");
                if (clientinstrument == 1) STATUSMESSadd("You finish playing the harpsichord.");
                if (clientinstrument == 2) STATUSMESSadd("You finish playing the harp.");
                if (clientinstrument == 3) STATUSMESSadd("You finish playing the panpipes.");
                if (clientinstrument == 4) STATUSMESSadd("You finish playing the xylophone.");

                playinstrument = 0;
                if (musickeyboard->offset_x < kPanelHideThresholdX) musickeyboard->offset_x += kPanelHideDeltaX;
            }
            goto CLIENT_donemess;
        } //29

        if (t->d2[0] == 35) {
            //flush buffers & scene update
            static player *tplayer_backup;
            tplayer_backup = tplayer;
            tplayer = tplay;
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
            tplayer = tplayer_backup;


            goto scene_update_message;
        } //35

        if (t->d2[0] == 31) {
            //scene update
        scene_update_message:

            static unsigned long bitsi, bitsi2;
            static unsigned short *tp2;

            static player *tplayer_backup;
            tplayer_backup = tplayer;
            tplayer = tplay;
            bitsi = 8;

            if (BITSget(t, &bitsi, 1)) {
                //x,y change
                tplayer->x = BITSget(t, &bitsi, 11);
                tplayer->y = BITSget(t, &bitsi, 10);

                // Auto-unfreeze if player moved outside the frozen viewport (e.g. teleport/ladder)
                if (camera_freeze) {
                    if (tplayer->x < tpx || tplayer->x >= tpx + (long) viewTilesX() ||
                        tplayer->y < tpy || tplayer->y >= tpy + (long) viewTilesY()) {
                        camera_freeze = 0;
                    }
                }
            }

            x = tplayer->x;
            y = tplayer->y;

            static long tpx_legacy, tpy_legacy;
            getscreenoffset_legacy(x, y, &tpx_legacy, &tpy_legacy);
            if (!camera_freeze) {
                getscreenoffset(x, y, &tpx, &tpy);
                // ROOMSYNC-P1: generic isolated-room follow camera override. For any
                // (x, y) inside a registered room, force a centered-on-player camera
                // -- bypassing the world-edge clamps that getscreenoffset() applies
                // in known regions. tpx_legacy/tpy_legacy above stays in the legacy
                // (host) reference frame because that's what the host's sobj/mover
                // encoders use; ONLY the dynamic render camera (tpx/tpy) follows.
                // See docs/rendering/global-room-sync.md.
                if (getroom(x, y, NULL, NULL, NULL, NULL)) {
                    tpx = x - (viewTilesX() / 2 - 1);
                    tpy = y - (viewTilesY() / 2 - 1);
                }
            }


            ctpx2 = tplayer->x;
            ctpy2 = tplayer->y;
            if (!camera_freeze) {
                ctpx = tpx;
                ctpy = tpy;
            }

            //screen+1 shift
            // RW sobj-fix: screen+1 grew from legacy 32x24 + 1 fence to
            // max-viewport (63x47) + 1 fence. tpx_legacy/tpy_legacy is the host's
            // emit reference frame; the 96x72 sobj buffer still has slack to
            // hold the larger window. See define_both.h.
            x3 = tpx_legacy - SOBJ_S1_LEFT;
            y3 = tpy_legacy - SOBJ_S1_TOP;
            x4 = tpx_legacy + SOBJ_S1_RIGHT;
            y4 = tpy_legacy + SOBJ_S1_BOTTOM;
            x5 = tplayer->sobj_bufoffx;
            y5 = tplayer->sobj_bufoffy;
            x6 = x5 + 96 - 1;
            y6 = y5 + 72 - 1; //current buffer extents
            //i. if the screen+1 buffer fits within buffer don't relocate
            if (x3 >= x5) {
                if (x4 <= x6) {
                    if (y3 >= y5) {
                        if (y4 <= y6) {
                            goto screen1shiftokc;
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
                tplayer->sobj_bufoffx = tpx_legacy - 32;
                tplayer->sobj_bufoffy = tpy_legacy - 24;
                goto screen1shiftokc;
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
        sobj_copyloop1c:
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
                            goto sobj_copiedpos1c;
                        }
                    }
                }
            }
            //clear position
            tplayer->sobj_tempfixed[x9][y9] = 0;
            tplayer->sobj_bufsize[x9][y9] = 0;
        sobj_copiedpos1c:;
            if (i == 0) {
                x9++;
                if (x9 >= 96) {
                    y9++;
                    x9 = 0;
                }
                if (y9 < 72) goto sobj_copyloop1c;
            }
            if (i == 1) {
                x9--;
                if (x9 < 0) {
                    y9++;
                    x9 = 95;
                }
                if (y9 < 72) goto sobj_copyloop1c;
            }
            if (i == 2) {
                x9++;
                if (x9 >= 96) {
                    y9--;
                    x9 = 0;
                }
                if (y9 >= 0) goto sobj_copyloop1c;
            }
            if (i == 3) {
                x9--;
                if (x9 < 0) {
                    y9--;
                    x9 = 95;
                }
                if (y9 >= 0) goto sobj_copyloop1c;
            }
            tplayer->sobj_bufoffx = x2;
            tplayer->sobj_bufoffy = y2;
        screen1shiftokc:;

            if (BITSget(t, &bitsi, 1)) {
                //obj buffer has changed
                //screen+8 shift
                // RW sobj-fix: screen+8 grew from legacy 32x24 + 8 fence to
                // max-viewport (63x47) + 8 fence. SOBJ_TX_OFFX/Y are the screen+8
                // fence distances by construction. See define_both.h.
                x3 = tpx_legacy - SOBJ_TX_OFFX;
                y3 = tpy_legacy - SOBJ_TX_OFFY;
                x4 = tpx_legacy + SOBJ_TX_W - 1 - SOBJ_TX_OFFX;
                y4 = tpy_legacy + SOBJ_TX_H - 1 - SOBJ_TX_OFFY;
                x5 = tplayer->sobj_bufoffx;
                y5 = tplayer->sobj_bufoffy;
                x6 = x5 + 96 - 1;
                y6 = y5 + 72 - 1; //current buffer extents
                //i. if the screen+8 buffer fits within buffer don't relocate
                if (x3 >= x5) {
                    if (x4 <= x6) {
                        if (y3 >= y5) {
                            if (y4 <= y6) {
                                goto screen8shiftokc;
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
                    tplayer->sobj_bufoffx = tpx_legacy - 32;
                    tplayer->sobj_bufoffy = tpy_legacy - 24;
                    goto screen8shiftokc;
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
            sobj_copyloopc:
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
                                goto sobj_copiedpos0c;
                            }
                        }
                    }
                }
                //clear position
                tplayer->sobj_tempfixed[x9][y9] = 0;
                tplayer->sobj_bufsize[x9][y9] = 0;
            sobj_copiedpos0c:;

                if (i == 0) {
                    x9++;
                    if (x9 >= 96) {
                        y9++;
                        x9 = 0;
                    }
                    if (y9 < 72) goto sobj_copyloopc;
                }
                if (i == 1) {
                    x9--;
                    if (x9 < 0) {
                        y9++;
                        x9 = 95;
                    }
                    if (y9 < 72) goto sobj_copyloopc;
                }
                if (i == 2) {
                    x9++;
                    if (x9 >= 96) {
                        y9--;
                        x9 = 0;
                    }
                    if (y9 >= 0) goto sobj_copyloopc;
                }
                if (i == 3) {
                    x9--;
                    if (x9 < 0) {
                        y9--;
                        x9 = 95;
                    }
                    if (y9 >= 0) goto sobj_copyloopc;
                }
                tplayer->sobj_bufoffx = x2;
                tplayer->sobj_bufoffy = y2;
            screen8shiftokc:;

            changestate:
                if (BITSget(t, &bitsi, 1)) {
                    // RW sobj-fix: was BITSget 11 + y%48 + tpx_legacy-8 offset; now
                    // SOBJ_TX_BITS + y%SOBJ_TX_W + tpx_legacy-SOBJ_TX_OFFX. Must match
                    // host encoder in loop_host.cpp. See define_both.h.
                    y = BITSget(t, &bitsi, SOBJ_TX_BITS);
                    x = y % SOBJ_TX_W;
                    y /= SOBJ_TX_W;

                    x2 = tpx_legacy + x - SOBJ_TX_OFFX;
                    y2 = tpy_legacy + y - SOBJ_TX_OFFY;
                    i3 = 0;
                    if (x2 >= 0 && x2 < 2048 && y2 >= 0 && y2 < 1024)
                        i3 = tobjfixed_index[y2][x2];
                    i4 = tobjfixed_type[i3];
                    z = BITSget(t, &bitsi, getnbits(i4));
                    i5 = 1 << z;

                    x3 = x2 - tplayer->sobj_bufoffx;
                    y3 = y2 - tplayer->sobj_bufoffy;
                    if (tplayer->sobj_tempfixed[x3][y3] & i5) tplayer->sobj_tempfixed[x3][y3] -= i5;
                    else tplayer->sobj_tempfixed[x3][y3] |= i5;
                    goto changestate;
                }

                static unsigned short vbuf[1024];
            oum_getnextsquare:
                if (BITSget(t, &bitsi, 1)) {
                    //if =1 set object of a/another square on the map
                    // RW sobj-fix: matches host encoder bit-width / multiplier / offset.
                    y = BITSget(t, &bitsi, SOBJ_TX_BITS);
                    x = y % SOBJ_TX_W;
                    y /= SOBJ_TX_W;
                    x2 = tpx_legacy + x - SOBJ_TX_OFFX;
                    y2 = tpy_legacy + y - SOBJ_TX_OFFY;
                    x3 = x2 - tplayer->sobj_bufoffx;
                    y3 = y2 - tplayer->sobj_bufoffy;
                    i = 0; //vbuf index
                oum_getnextobj:
                    if (BITSget(t, &bitsi, 1)) {
                        //if =1 a/another object exists on this square
                        vbuf[i] = BITSget(t, &bitsi, 16);
                        vbuf[i] |= (BITSget(t, &bitsi, 1) * 32768);
                        i++;
                        goto oum_getnextobj;
                    } //if =1 a/another object exists on this square
                    //update allocated buffer size
                    if (tplayer->sobj_buf2bytes[x3][y3] < i) {
                        if (tplayer->sobj[x3][y3]) free(tplayer->sobj[x3][y3]);
                        tplayer->sobj[x3][y3] = (unsigned short *) malloc(i * 2);
                        tplayer->sobj_buf2bytes[x3][y3] = i;
                    }
                    //update buffer info
                    tp2 = tplayer->sobj[x3][y3];
                    for (i2 = 0; i2 < i; i2++) {
                        tp2[i2] = vbuf[i2];
                    }
                    tplayer->sobj_bufsize[x3][y3] = i;
                    goto oum_getnextsquare;
                } //if =1 set object of a/another square on the map


                //update mover buffer


                /*
                   ***OBJECT UPDATE MESSAGE*** [OFFICIAL]
                   b:
                   1	if =1 set object of a/another square on the map
                   {
                   11	x,y location offset of square within buffer+8
                   a:
                   1	if =1 a/another object exists on this square
                   {
                   10 object graphic index (optomized)
                   1 object is flashing
                   goto a
                   }
                   goto b
                   }
                   */
            } //object buffer changed


            //remove all offscreen objects in client's array
            // RW dynamic-objects fix follow-up (2026-05-28): bounds MUST match
            // the host's mover transmit window. The host now fills + emits movers
            // across the full MV_TX_W x MV_TX_H rectangle centered on the avatar
            // (see define_both.h MV_TX_OFFX / MV_TX_OFFY). When this client-side
            // prune was still hardcoded to the legacy 32x24 box, every mover the
            // host placed outside that box got removed locally before processing
            // the host's remove/move/add messages -- which then targeted the
            // wrong mv_x[] indices because both sides had reshuffled the array
            // differently. The visible symptom was NPCs/ships/party members
            // teleporting, ships drawn on land, and the avatar going invisible
            // when index 0 got reassigned to a different mover.
        mover_removeoffscreen_restartc:
            for (i = 0; i < tplayer->mv_i; i++) {
                x = tplayer->mv_x[i] - tpx_legacy;
                y = tplayer->mv_y[i] - tpy_legacy;
                if ((x < -MV_TX_OFFX) || (x > (MV_TX_W - 1 - MV_TX_OFFX)) || (y < -MV_TX_OFFY) || (
                        y > (MV_TX_H - 1 - MV_TX_OFFY))) {
                    //reshuffle array
                    for (i3 = i + 1; i3 < tplayer->mv_i; i3++) {
                        tplayer->mv_x[i3 - 1] = tplayer->mv_x[i3];
                        tplayer->mv_y[i3 - 1] = tplayer->mv_y[i3];
                        tplayer->mv_type[i3 - 1] = tplayer->mv_type[i3];
                        tplayer->mv_dir[i3 - 1] = tplayer->mv_dir[i3];
                        tplayer->mv_frame[i3 - 1] = tplayer->mv_frame[i3];
                        tplayer->mv_object[i3 - 1] = tplayer->mv_object[i3];
                        tplayer->mv_flags[i3 - 1] = tplayer->mv_flags[i3];
                        tplayer->mv_hpmp[i3 - 1] = tplayer->mv_hpmp[i3];
                        tplayer->mv_playerid[i3 - 1] = tplayer->mv_playerid[i3];
                        tplayer->mv_ktar[i3 - 1] = tplayer->mv_ktar[i3];
                        tplayer->mv_more[i3 - 1] = tplayer->mv_more[i3];
                    } //i3
                    tplayer->mv_i--;


                    goto mover_removeoffscreen_restartc;
                } //x,y boundary check
            } //i


            //1. REMOVE
        mover_remove_next:
            if (BITSget(t, &bitsi, 1)) {
                i = BITSget(t, &bitsi, getnbits(tplayer->mv_i));

                //reshuffle array
                for (i3 = i + 1; i3 < tplayer->mv_i; i3++) {
                    tplayer->mv_x[i3 - 1] = tplayer->mv_x[i3];
                    tplayer->mv_y[i3 - 1] = tplayer->mv_y[i3];
                    tplayer->mv_type[i3 - 1] = tplayer->mv_type[i3];
                    tplayer->mv_dir[i3 - 1] = tplayer->mv_dir[i3];
                    tplayer->mv_frame[i3 - 1] = tplayer->mv_frame[i3];
                    tplayer->mv_object[i3 - 1] = tplayer->mv_object[i3];
                    tplayer->mv_flags[i3 - 1] = tplayer->mv_flags[i3];
                    tplayer->mv_hpmp[i3 - 1] = tplayer->mv_hpmp[i3];
                    tplayer->mv_playerid[i3 - 1] = tplayer->mv_playerid[i3];
                    tplayer->mv_ktar[i3 - 1] = tplayer->mv_ktar[i3];
                    tplayer->mv_more[i3 - 1] = tplayer->mv_more[i3];
                } //i3
                tplayer->mv_i--;


                goto mover_remove_next;
            }

            /*
                   ***2.3 MOVE***
                   1 if =1 move a/another mover
                   {
                   [?] index of mover to move in current buffer
                   5 number representing a special x,y offset from current location to move to
                   }
                   */
            /*
                   for (i=0;i<tplayer->mv_i;i++){
                   i2=mv_last[i];
                   x=mv_x[i2]-tplayer->mv_x[i]; y=mv_y[i2]-tplayer->mv_y[i];
                   if (x||y){
                   BITSadd(t,&bitsi,1,1);//move a/another mover
                   z4=tplayer->mv_i; z2=1; z3=0; getnbits3: if (z2<z4){z3++; z2<<=1; goto getnbits3;}//z3=nbits required to store which tempfixed obj has changed
                   if (z3) BITSadd(t,&bitsi,i,z3);//index of mover to move in current buffer
                   BITSadd(t,&bitsi,mover_offseti[x+3][y+3],5);
                   }//x||y
                   }//i
                   BITSadd(t,&bitsi,0,1);//no more movers to move
                   */
        mover_move_next:
            if (BITSget(t, &bitsi, 1)) {
                i = BITSget(t, &bitsi, getnbits(tplayer->mv_i));
                z = BITSget(t, &bitsi, 5);
                x = mover_offsetx[z];
                y = mover_offsety[z];
                tplayer->mv_x[i] += x;
                tplayer->mv_y[i] += y;


                if (x > 0) tplayer->mv_dir[i] = 1;
                if (x < 0) tplayer->mv_dir[i] = 3;
                if (y < 0) tplayer->mv_dir[i] = 0;
                if (y > 0) tplayer->mv_dir[i] = 2;

                tplayer->mv_frame[i] = objgetnextframe(tplayer->mv_type[i], tplayer->mv_frame[i]);

                goto mover_move_next;
            }


            //2.2 MOVER DIRECTION/FRAME CHANGE
            /*
                 1 change mover frame/dir
                 ? index of mover
                 [2] if mover CAN have a direction it is set here, otherwise this can be omitted and the movers frame only will be incremented
                 */
            /*
                 for (i=0;i<tplayer->mv_i;i++){
                 i2=mv_last[i];
                 if ((tplayer->mv_dir[i]!=mv_dir[i2])||(tplayer->mv_frame[i]!=mv_frame[i2])){
                 BITSadd(t,&bitsi,1,1);
                 BITSadd(t,&bitsi,i,getnbits(tplayer->mv_i));//index of mover to move in current buffer
                 BITSadd(t,&bitsi,mv_dir[i2],2);
                 tplayer->mv_dir[i]=mv_dir[i2];
                 tplayer->mv_frame[i]=mv_frame[i2];
                 sceneupdaterequired=1;
                 }
                 }//i
                 BITSadd(t,&bitsi,0,1);//no more movers to direction/frame change
                 */
        mover_dirfrmchange_next:
            if (BITSget(t, &bitsi, 1)) {
                i = BITSget(t, &bitsi, getnbits(tplayer->mv_i));
                tplayer->mv_dir[i] = BITSget(t, &bitsi, 2);
                tplayer->mv_frame[i] = objgetnextframe(tplayer->mv_type[i], tplayer->mv_frame[i]);
                goto mover_dirfrmchange_next;
            }


        mover_statechange_next:
            if (BITSget(t, &bitsi, 1)) {
                i = BITSget(t, &bitsi, getnbits(tplayer->mv_i));
            mover_statechange_next2:
                z = BITSget(t, &bitsi, 4);
                if (z == 0) {
                    tplayer->mv_flags[i] ^= MV_LIGHTBRIGHT;
                    if (tplayer->mv_flags[i] & MV_LIGHTBRIGHT) {
                        if (tplayer->mv_flags[i] & MV_LIGHTGLOW) tplayer->mv_flags[i] -= MV_LIGHTGLOW;
                    }
                } //0
                if (z == 1) {
                    tplayer->mv_flags[i] ^= MV_LIGHTGLOW;
                    if (tplayer->mv_flags[i] & MV_LIGHTGLOW) {
                        if (tplayer->mv_flags[i] & MV_LIGHTBRIGHT) tplayer->mv_flags[i] -= MV_LIGHTBRIGHT;
                    }
                } //1
                if (z == 2) tplayer->mv_flags[i] ^= MV_INVISIBLE;
                if (z == 3) tplayer->mv_flags[i] ^= MV_PARALYZE;
                if (z == 4) tplayer->mv_flags[i] ^= MV_SLEEP;
                if (z == 5) tplayer->mv_flags[i] ^= MV_PROTECT;
                if (z == 6) tplayer->mv_flags[i] ^= MV_STORMCLOAK;
                if (z == 7) tplayer->mv_flags[i] ^= MV_TYPING;
                if (z == 8) {
                    tplayer->mv_hpmp[i] = BITSget(t, &bitsi, 8);
                }
                if (z == 9) {
                    tplayer->mv_ktar[i] = BITSget(t, &bitsi, 4);
                }
                if (z == 10) tplayer->mv_flags[i] ^= MV_SPEAKING;
                if (BITSget(t, &bitsi, 1)) goto mover_statechange_next2;
                goto mover_statechange_next;
            }

            //3. ADD
        mover_add_next:
            if (BITSget(t, &bitsi, 1)) {
                i = tplayer->mv_i;
                // RW dynamic-objects fix: host now encodes mover
                // positions across the full MV_TX_W x MV_TX_H window
                // (centered on the avatar via MV_TX_OFFX/Y) in
                // MV_TX_BITS bits. Was 10 bits + y*34+x tied to the
                // legacy 32x24 view, which caused NPCs/monsters/other
                // players outside the legacy box to never reach the
                // client. tpx_legacy is the host's reference origin
                // (avatar at col 15 / row 11 of the legacy view); the
                // -MV_TX_OFFX/Y shift recenters the decoded position
                // on the avatar so it matches the host's centered
                // emit. See define_both.h.
                y = BITSget(t, &bitsi, MV_TX_BITS);
                x = y % MV_TX_W;
                y /= MV_TX_W;
                x = x + tpx_legacy - MV_TX_OFFX;
                y = y + tpy_legacy - MV_TX_OFFY;
                tplayer->mv_x[i] = x;
                tplayer->mv_y[i] = y;
                z = BITSget(t, &bitsi, 10);
                //special cases exist
                tplayer->mv_type[i] = z;
                tplayer->mv_frame[i] = 0;
                tplayer->mv_dir[i] = 0;

                if (z == 413) {
                    //silver serp
                    tplayer->mv_frame[i] = BITSget(t, &bitsi, 4);
                    goto mover_add_specialc;
                }
                if (z == 366) {
                    //tanglevine tendril (not core)
                    tplayer->mv_frame[i] = BITSget(t, &bitsi, 3);
                    goto mover_add_specialc;
                }
                //check if direction is required (z3 is number of bits to use to send direction)
                z3 = 2;
                if (obji[sprlnk[z]].v4 == 3) z3 = 0; //"bird"
                if (z == 365) z3 = 0; //tanglevine core
                if (z == 374) z3 = 0; //hydra core
                if (z == 375) z3 = 0; //slime
                if (z == 391) z3 = 0; //farmer hoeing
                if (z == 415) z3 = 0; //raft
                tplayer->mv_dir[i] = BITSget(t, &bitsi, z3);
            mover_add_specialc:

                tplayer->mv_flags[i] = 0;
                tplayer->mv_hpmp[i] = 0;
                tplayer->mv_playerid[i] = 0;
                tplayer->mv_ktar[i] = 0;
                tplayer->mv_more[i] = 0;

            mover_add_getstate_next:
                if (BITSget(t, &bitsi, 1)) {
                    z2 = BITSget(t, &bitsi, 4);

                    if (z2 == 0) tplayer->mv_flags[i] |= MV_LIGHTBRIGHT;
                    if (z2 == 1) tplayer->mv_flags[i] |= MV_LIGHTGLOW;
                    if (z2 == 2) tplayer->mv_flags[i] |= MV_INVISIBLE;
                    if (z2 == 3) tplayer->mv_flags[i] |= MV_PARALYZE;
                    if (z2 == 4) tplayer->mv_flags[i] |= MV_SLEEP;
                    if (z2 == 5) tplayer->mv_flags[i] |= MV_PROTECT;
                    if (z2 == 6) tplayer->mv_flags[i] |= MV_STORMCLOAK;
                    if (z2 == 7) tplayer->mv_flags[i] |= MV_TYPING;
                    if (z2 == 8) {
                        tplayer->mv_hpmp[i] = BITSget(t, &bitsi, 8);
                        tplayer->mv_playerid[i] = BITSget(t, &bitsi, 32);
                    }
                    if (z2 == 9) {
                        tplayer->mv_ktar[i] = BITSget(t, &bitsi, 4);
                    }
                    if (z2 == 10) tplayer->mv_flags[i] |= MV_SPEAKING;

                    if (z2 == 11) {
                        tplayer->mv_more[i] = BITSget(t, &bitsi, 16);
                    }


                    goto mover_add_getstate_next;
                }

                tplayer->mv_i++;
                goto mover_add_next;
            }

            tplayer = tplayer_backup;
            goto CLIENT_donemess;
        } //31

        if (t->d2[0] == 32) {
            //send time
            if (U6O_DEBUG) {
                scrlog("Time update via message 32.");
                txtset(t2, "Client time was: ");
                txtset(t3, "                                ");
                _snprintf((char *) t3->d2, 32, "%e", btime2);
                txtadd(t2, t3);
                scrlog((const char *) t2->d2);
            }
            txtright(t, 8);
            if (!NEThost) btime2 = t->dd[0];
            if (U6O_DEBUG) {
                txtset(t2, "Server time was: ");
                txtset(t3, "                                ");
                _snprintf((char *) t3->d2, 32, "%e", btime2);
                txtadd(t2, t3);
                scrlog((const char *) t2->d2);
            }
            goto CLIENT_donemess;
        } //send time

#ifdef SECURE
#include "secret_b.inc"
#endif

        if (t->d2[0] == 33) {
            //xray update
            xray = t->d2[1];
            goto CLIENT_donemess;
        } //xray update

        if (t->d2[0] == 36) {
            //wizardeyetimeleft update
            wizardeyetimeleft = t->d2[1];
            goto CLIENT_donemess;
        } //wizardeyetimeleft update


        if (t->d2[0] == 254) {
            //incorrect version
            MessageBox(NULL, "U6O incorrect version, please download new version", "Ultima 6 Online", MB_OK);
            setupfail = TRUE;
            goto CLIENT_donemess;
        }

        if (t->d2[0] == 252) {
            //remove NPC from temp party
            x = t->d2[1];
            tplay->party[x] = NULL;
            if (party_spellbook_frame[x]->offset_x < kPanelHideThresholdX)
                party_spellbook_frame[x]->offset_x += kPanelHideDeltaX;
            if (party_frame[x]->offset_x < kPanelHideThresholdX) party_frame[x]->offset_x += kPanelHideDeltaX;
            qkstf_update = TRUE;
            goto CLIENT_donemess;
        }

        if (t->d2[0] == 250) {
            if (t->d2[1] == 0) MessageBox(NULL, "Leaving Britannia (Return after 30 minutes to fully heal)",
                                          "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 1) MessageBox(NULL, "Leaving Britannia (Host has quit)", "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 2) MessageBox(NULL, "Invalid name, username or password", "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 3) MessageBox(NULL, "Username already taken (Select a different username)",
                                          "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 4) MessageBox(NULL, "Character name already taken (Select a different character name)",
                                          "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 5) MessageBox(NULL, "Username or Password Incorrect", "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 6) {
                //resend password with specified encryption.
                pw_encrypt = t->d2[2];
                setup_message = FALSE;
                goto intro_done;
            }
            if (t->d2[1] == 7) MessageBox(NULL, "Leaving Britannia (Administration forced you to leave)",
                                          "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 8) MessageBox(
                NULL, "Leaving Britannia (Administration has currently banned your character)", "Ultima 6 Online",
                MB_OK);
            if (t->d2[1] == 9) MessageBox(NULL, "Leaving Britannia (Idle)", "Ultima 6 Online", MB_OK);
            if (t->d2[1] == 10) {
                MessageBox(NULL, "Voice Chat Disabled (Blocked by administration, your message was not sent to others)",
                           "Ultima 6 Online", MB_OK);
                goto CLIENT_donemess;
            }
            DestroyWindow(hWnd);
            goto CLIENT_donemess;
        }

        if (t->d2[0] == 38) {
            //receive voice message
            txtright(t, t->l - 1);
        localvoicemessage_begin:
            x2 = t->ds[0];
            txtright(t, t->l - 2); //port
            txtNEWLEN(t5, -t->d2[0]);
            txtright(t, t->l - 1);
            memcpy(t5->d, t->d, t5->l);
            txtright(t, t->l - t5->l); //name
            x3 = t->ds[0];
            txtright(t, t->l - 2);
            y3 = t->ds[0];
            txtright(t, t->l - 2);

            //***** PLAYER VOICE MESSAGE MUTING AND IGNORING *****
            txtset(t4, t5);
            txtucase(t4);
            tfh = open2("ignore.txt", OF_READWRITE | OF_SHARE_COMPAT);
        ignore_nextplayer2:
            txtfilein(t6, tfh);
            txtucase(t6);
            txtset(t7, "[MUTE]");
            txtadd(t7, t4);
            if (txtsame(t6, t7)) {
                txtNEWLEN(t, 0);
                goto muted2;
            }
            txtset(t7, "[IGNORE]");
            txtadd(t7, t4);
            if (txtsame(t6, t7)) {
                if (localvoicemessage_return) {
                    localvoicemessage_return = 0;
                    goto localvoicemessage_returnpoint;
                }
                goto CLIENT_donemess;
            }
            if (t6->l) goto ignore_nextplayer2;
            close(tfh);
        muted2:;


            if (t->l) {
                //1. build file
                tfh = open2(".\\voice\\voicein.spx", OF_READWRITE | OF_CREATE);
                put(tfh, t->d2, t->l);
                close(tfh);
                //2. decode sound
                deletefile(".\\voice\\voicein.wav");
                PROCESS_INFORMATION pi;
                STARTUPINFO si;
                ZeroMemory(&pi, sizeof(pi));
                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(STARTUPINFO);
                si.dwFlags = STARTF_USESIZE | STARTF_USESHOWWINDOW;
                si.wShowWindow = SW_HIDE;
                CreateProcess(NULL, ".\\voice\\speexdec.exe .\\voice\\voicein.spx .\\voice\\voicein.wav", NULL, NULL,
                              FALSE, 0, NULL, NULL, &si, &pi);
                waitforfile(".\\voice\\voicein.wav");
                //3. load sound
                static sound *voicein;
                voicein = soundload(".\\voice\\voicein.wav");
                //4. play sound
                x4 = u6ovolume;
                u6ovolume = u6ovoicevolume;
                soundplay2(voicein, 255); //should reflect voice volume controls!!
                u6ovolume = x4;
                //5. delete (primary) sound
                voicein->s->Release();
                free((void *) voicein);
            }

            txtadd(t5, " (SPEAKING)?");
            t5->d2[t5->l - 1] = 255;

            //add sfx message
            for (i3 = 0; i3 <= 255; i3++) {
                if (sfx[i3].type == 0) {
                    sfx[i3].type = 3;
                    sfx[i3].x = x3;
                    sfx[i3].y = y3 - 2;
                    sfx[i3].x2 = t5->l;
                    static txt *dbgt5;
                    dbgt5 = txtnew();
                    txtset(dbgt5, t5);
                    sfx[i3].p = dbgt5;
                    z = t5->l;
                    f = (float) z / 10.0f;
                    if (f < 1)f = 1;
                    if (f > 4)f = 4;
                    sfx[i3].wait = f;
                    sfx[i3].more = x2;
                shiftdown4:
                    for (i2 = 0; i2 <= 255; i2++) {
                        if ((sfx[i2].type == 3) && (i2 != i3)) {
                            if ((sfx[i3].y == sfx[i2].y) || (sfx[i3].y == (sfx[i2].y + 1)) || (
                                    sfx[i3].y == (sfx[i2].y - 1))) {
                                sfx[i3].y = sfx[i2].y + 2;
                                goto shiftdown4;
                            }
                        }
                    }

                    if (localvoicemessage_return) {
                        localvoicemessage_return = 0;
                        goto localvoicemessage_returnpoint;
                    }
                    goto CLIENT_donemess;
                }
            }
            if (localvoicemessage_return) {
                localvoicemessage_return = 0;
                goto localvoicemessage_returnpoint;
            }
            goto CLIENT_donemess;
        } //38

        if (t->d2[0] == 41) {
            //receive volume levels of nearby players
            txtright(t, t->l - 1);
        receivenextvolume:
            x2 = t->dl[0];
            txtright(t, t->l - 4); //id
            if (x2 == 0xFFFFFFFF) {
                //ID not being sent, use static position
                voicechat_listeningplayeri++;
                voicechat_listeningplayerx[voicechat_listeningplayeri] = t->ds[0];
                txtright(t, t->l - 2);
                voicechat_listeningplayery[voicechat_listeningplayeri] = t->ds[0];
                txtright(t, t->l - 2);


                x3 = t->d2[0];
                txtright(t, t->l - 1); //volume
                voicechat_listeningplayervolume[voicechat_listeningplayeri] = x3;
            } else {
                x3 = t->d2[0];
                txtright(t, t->l - 1); //volume
                if (idlstn != -1) {
                    for (z2 = 0; z2 <= idlstn; z2++) {
                        if (x2 == idlst[z2]) {
                            idlst_volume[z2] = x3;
                        } //x2==idlst[z2]
                    } //z2
                } //!=-1
            }
            voicechat_listeningplayers |= 1;
            if (x3) voicechat_listeningplayers |= 2; //player can hear voice
            if (t->l) goto receivenextvolume;
            goto CLIENT_donemess;
        } //41

        if (t->d2[0] == 43) {
            //receive portrait data from host
            txtright(t, t->l - 1);
            x2 = t->ds[0];
            txtright(t, t->l - 2);

            // MM-P9.3: portrait reload surface leak (dominant idle leak). The
            // host re-pushes portrait data (message type 43) for already-loaded
            // indices — the local player and nearby NPCs as they update — so
            // unconditionally allocating a fresh 56x64 surface here leaked one
            // DirectDraw SURF_SYSMEM16 per message (piling up in surflist[] and
            // slowing every blit). Reuse the existing cached portrait surface on
            // reload: its pixels are fully overwritten by the decompressor below,
            // and keeping the same pointer means inpf->graphic (which can hold
            // getportrait(x2) == portrait[x2]) never dangles. Only allocate the
            // first time this index is seen.
            static surf *receiveport = NULL;
            if (portrait_loaded[x2] && portrait[x2]) receiveport = portrait[x2];
            else receiveport = newsurf(56, 64, SURF_SYSMEM16);


            /*
        LOSS-LESS COMPRESS PORTRAIT DATA
        loop:
        2bits    (V0-V3)

        V0 repeat last known (last known starts off as black)
        {
        5bits    1-32 repeats
        loop
        }

        V1 can't be created
        {
        5:5:5bits    fixed colour
        loop
        }

        V2 accend
        {
        3bits    blue
        3bits    green
        3bits    red
        loop
        }

        V3 decend
        {
        3bits    blue
        3bits    green
        3bits    red
        loop
        }

        */
            static unsigned long bitsi;
            static long r, g, b, r2, g2, b2, r3, g3, b3, repeat;

            bitsi = 0;
            b = 0;
            g = 0;
            r = 0;
            repeat = 0;

            x3 = 0;
        lluc_nextpixel:
            y2 = BITSget(t, &bitsi, 2);

            if (y2 == 0) {
                y3 = BITSget(t, &bitsi, 5) + 1;
                for (y4 = 1; y4 <= y3; y4++) {
                    receiveport->o2[x3] = b + (g << 6) + (r << 11);
                    x3++;
                } //y4
            } //y2==0

            if (y2 == 1) {
                b = BITSget(t, &bitsi, 5);
                g = BITSget(t, &bitsi, 5);
                r = BITSget(t, &bitsi, 5);
                receiveport->o2[x3] = b + (g << 6) + (r << 11);
                x3++;
            } //y2==1

            if (y2 == 2) {
                b += BITSget(t, &bitsi, 3);
                if (b > 31) b = b - 32;
                g += BITSget(t, &bitsi, 3);
                if (g > 31) g = g - 32;
                r += BITSget(t, &bitsi, 3);
                if (r > 31) r = r - 32;
                receiveport->o2[x3] = b + (g << 6) + (r << 11);
                x3++;
            } //y2==2

            if (y2 == 3) {
                b -= BITSget(t, &bitsi, 3);
                if (b < 0) b = b + 32;
                g -= BITSget(t, &bitsi, 3);
                if (g < 0) g = g + 32;
                r -= BITSget(t, &bitsi, 3);
                if (r < 0) r = r + 32;
                receiveport->o2[x3] = b + (g << 6) + (r << 11);
                x3++;
            } //y2==3

            if (x3 < 3584) goto lluc_nextpixel;

            loadportrait(x2, receiveport);
            if (myobj = tplay->party[0]) {
                tnpc = (npc *) myobj->more;
                if (tnpc->port == x2) tnpc->upflags |= 1;
            } //tplay->party[0]
            goto CLIENT_donemess;
        } //43

        if (t->d2[0] == 44) {
            //receive portrait look message
            txtright(t, t->l - 1);
            portraitlook_wait = 8.0f;
            if (statusmessage_viewnpc->offset_x >= kPanelHideThresholdX)
                statusmessage_viewnpc->offset_x -= kPanelHideDeltaX;
            portraitlook_portrait = t->ds[0];
            txtright(t, t->l - 2);
            if (portraitlook_name == NULL) portraitlook_name = txtnew();
            txtNEWLEN(portraitlook_name, -t->d2[0]);
            txtright(t, t->l - 1);
            memcpy(portraitlook_name->d, t->d, portraitlook_name->l);
            txtright(t, t->l - portraitlook_name->l);
            portraitlook_namecolour = t->dl[0] & 0xFFFFFF;
            txtright(t, t->l - 3);
            portraitlook_equip = t->d2[0];
            txtright(t, t->l - 1);
            x3 = 1;
            for (x2 = 0; x2 < 8; x2++) {
                if (portraitlook_equip & x3) {
                    portraitlook_type[x2] = t->ds[0];
                    txtright(t, t->l - 2);
                    portraitlook_plusbonus[x2] = t->d2[0];
                    txtright(t, t->l - 1);
                } else {
                    portraitlook_type[x2] = 0;
                }
                x3 <<= 1;
            }

            if (portraitlook_equip) {
                statusmessage_viewnpc->graphic = viewnpc_temp;
                img(viewnpc_temp, 0, 0, viewnpc);
            } else {
                statusmessage_viewnpc->graphic = viewnpc2_temp;
                img(viewnpc2_temp, 0, 0, viewnpc2);
                if (statusmessage_viewnpc->offset_x <= -112) statusmessage_viewnpc->offset_x = -112 + 8;
            }
            img0(statusmessage_viewnpc->graphic, 0, 0, getportrait_doublesize(portraitlook_portrait));


            goto CLIENT_donemess;
        } //44 receive portrait look message


        if (t->d2[0] == 46) {
            //receive not4sale info
            not4sale_flags[t->d2[1]] = t->ds[1];
            goto CLIENT_donemess;
        } //46 receive not4sale info

        if (t->d2[0] == 48) {
            //peer update
            peer = t->d2[1];
            goto CLIENT_donemess;
        } //peer update

        if (t->d2[0] == 49) {
            //treasure map look message
            tmap = 32;
            txtright(t, t->l - 1);
            //treasuremap
            x3 = (t->dl[0]) & 1023;
            y3 = (t->dl[0] >> 10) & 1023;
            x4 = (t->dl[0] >> 20) & 7;
            y4 = (t->dl[0] >> 23) & 7;
            if (droploc) {
                txtset(t3, "L ");
                txtnumint(t4, tplay->x);
                txtadd(t3, t4);
                txtadd(t3, ",");
                txtnumint(t4, tplay->y);
                txtadd(t3, t4);
                txtadd(t3, " | M ");
                txtnumint(t4, x3);
                txtadd(t3, t4);
                txtadd(t3, ",");
                txtnumint(t4, y3);
                txtadd(t3, t4);
                STATUSMESSadd(t3);
            }
            x3 += (x4 - 3) * 12;
            y3 += (y4 - 3) * 12;
            if (x3 < 0) { x3 = 0; }
            if (x3 >= 1024) { x3 = 1023; }
            if (y3 < 0) { y3 = 0; }
            if (y3 >= 1024) { y3 = 1023; }
            img(ps7, -(x3), -(y3), treasuremap);
            img(tmap_marker, 0, 0, tmap_markers);
            img0(ps7, 60 - (x4 - 3) * 12, 60 - (y4 - 3) * 12, tmap_marker);
            img(ps8, ps7);
            img0(tmap_frame->graphic, 8, 8, ps8);
            goto CLIENT_donemess;
        } //treasure map look message

        if (t->d2[0] == MSG_MAPMANIFEST) {
            //MDD-P1.3: host map-file manifest -> start / refresh the map-data sync
            MAPDL_on_manifest(t);
            goto CLIENT_donemess;
        } //60

        if (t->d2[0] == MSG_MAPCHUNK_RESP) {
            //MDD-P2.3: a downloaded map-file slice -> assemble / verify / cache
            MAPDL_on_chunk(t);
            goto CLIENT_donemess;
        } //62
    } //l!=0
CLIENT_donemess:


    //if (NEThost) goto U6Ohostlink1return;

    goto CLIENT_readnext;
} //read local message