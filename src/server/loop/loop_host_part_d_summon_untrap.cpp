// LHS-P6.4: part_d_summon_untrap -- CONTINUES MEGA D. WIRE.
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_d_open.cpp.
// Contents: summon/conjure/replicate (summonfail, conjfail, isretry, gotreploc),
// untrap (untrap_*), pickpocket (pickpocketcrt*). Move-only.
                        summonfail:
                            goto spelldone;
                        } //summon

                        if (CASTSPELL_SPELLTYPE == ((7 << 4) + 5)) {
                            //slime //txtset(spellname[(7<<4)+5],"Slime"); spellreagent[(7<<4)+5]=BLOO|NIGH|MAND;
                            i2 = 2;
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;

                            for (spellx = -2; spellx <= 2; spellx++) {
                                for (spelly = -2; spelly <= 2; spelly++) {
                                    if ((abs(spellx) + abs(spelly)) != 4) {
                                        if (stormcloakcheck2(tpx + x + spellx, tpy + y + spelly, tplayer))
                                            goto
                                                    slimeblocked;

                                        myobj2 = OBJfindlast(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x + spellx, tpy + y + spelly);
                                        if (myobj2) {
                                            if ((myobj2->info & 8) == 0) {
                                                //no redirector
                                                if (myobj2->info & 4) {
                                                    //<-crt
                                                    crt = (creature *) myobj2->more;
                                                    if (crt->al == 1) {
                                                        if (((tpx + x + spellx) < 0) || ((tpx + x + spellx) > 2047) || (
                                                                (tpy + y + spelly) < 0) || ((tpy + y + spelly) > 1023))
                                                            goto slimefail;
                                                        if ((btflags[bt[tpy + y + spelly][tpx + x + spellx] & 1023] & 1)
                                                            == 0)
                                                            goto slimefail; //land passable

                                                        if ((myobj2->type & 1023) == 366) goto slimefail;
                                                        //invincible tanglevine tendril!
                                                        if ((myobj2->type & 1023) == 430)
                                                            if (crt->flags & 128)
                                                                goto
                                                                        slimefail; //invincible horse


                                                        x3 = rnd * (640 + tnpc->i * 10); //128 better than kill spell!
                                                        x4 = rnd * (640 + tnpc->i * 10);
                                                        x3 = ((x3 + x4) >> 1);

                                                        if (x3 >= crt->hp) {
                                                            //success
                                                            i2 = 1;

                                                            //kill/remove creature
                                                            x3 = x2;

                                                            myobj4 = (object *) crt->items->more;
                                                            //will point to first item
                                                        dropgoldmore3:
                                                            if (myobj4) {
                                                                myobj5 = (object *) myobj4->next;
                                                                OBJremove(myobj4);
                                                                if (myobj5) {
                                                                    myobj4 = myobj5;
                                                                    goto dropgoldmore3;
                                                                }
                                                            }
                                                            crtrespawn(myobj2);
                                                            x2 = x3;

                                                            myobj2 = OBJnew();
                                                            myobj2->type = 375; //slime
                                                            myobj2->more = malloc(sizeof(creature));
                                                            ZeroMemory(myobj2->more, sizeof(creature));
                                                            crt = (creature *) myobj2->more;
                                                            crt->crt_struct = TRUE;
                                                            crt->hp = (obji[sprlnk[myobj2->type & 1023]].v8 >> 8) * 4;
                                                            crt->mp = rnd * 9;
                                                            crt->al = 1;
                                                            crt->respawn_x = 1024;
                                                            //seconds till spawned crt will disappear
                                                            myobj9 = OBJnew();
                                                            myobj9->type = 188;
                                                            crt->items = myobj9; //(not included)bag
                                                            myobj2->info |= 4; //<-crt
                                                            myobj2->info |= (3 << 9); //slime divide!
                                                            OBJadd(tpx + x + spellx, tpy + y + spelly, myobj2);
                                                        } else {
                                                            if (i2 == 2) i2 = 3; //spell failed
                                                        }
                                                    } //al==1
                                                }
                                            }
                                        } //myobj2
                                    slimefail:;
                                    slimeblocked:;
                                    }
                                }
                            } //4,x,y

                            goto spelldone;
                        } //slime


                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 1)) {
                            //conjure //txtset(spellname[(3<<4)+1],"Conjure"); spellreagent[(3<<4)+1]=SPID|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            spellx = tpx + x;
                            spelly = tpy + y;
                            if ((spellx < 0) || (spellx > 2047) || (spelly < 0) || (spelly > 1023)) goto conjfail;
                            if (((bt[spelly][spellx] >> 10) & 1) == 0) goto conjfail; //land passable
                            if (od[spelly][spellx]) goto conjfail;
                            x3 = rnd * (8 + 5 * 4); //int can only double spell effectiveness!
                            x4 = rnd * (tnpc->i + 10);
                            if (x4 >= x3) {
                                //success
                                i2 = 1;
                                myobj2 = OBJnew();
                                x3 = rnd * 5;
                                myobj2->type = 342; //rat
                                if (x3 == 1) myobj2->type = 358; //snake
                                if (x3 == 2) myobj2->type = 344; //bat
                                if (x3 == 3) myobj2->type = 371; //troll
                                if (x3 == 4) myobj2->type = 361; //spider
                                myobj2->more = malloc(sizeof(creature));
                                ZeroMemory(myobj2->more, sizeof(creature));
                                crt = (creature *) myobj2->more;
                                crt->crt_struct = TRUE;
                                crt->hp = (obji[sprlnk[myobj2->type & 1023]].v8 >> 8) * 4;
                                crt->mp = rnd * 9;
                                crt->al = 4;
                                crt->respawn_x = 1024; //seconds till spawned crt will disappear
                                myobj9 = OBJnew();
                                myobj9->type = 188;
                                crt->items = myobj9; //(not included)bag
                                myobj2->info |= 4; //<-crt
                                OBJadd(spellx, spelly, myobj2);
                            } else { i2 = 3; } //spell failed
                        conjfail:
                            goto spelldone;
                        } //conjure

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 2)) {
                            //insect swarm //txtset(spellname[(4<<4)+2],"InsectSwarm"); spellreagent[(4<<4)+2]=BLOO|SPID|SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;

                            x3 = rnd * (4 + (tnpc->i / 16)); //int can only double spell effectiveness!
                            if (x3 == 0) {
                                i2 = 3;
                                goto spelldone;
                            }
                            for (x4 = 1; x4 <= x3; x4++) {
                                x5 = 16;
                            isretry:
                                spellx = tpx + x + rnd * 5 - 2;
                                spelly = tpy + y + rnd * 5 - 2;
                                x5--;
                                if (x5) {
                                    if ((spellx < 0) || (spellx > 2047) || (spelly < 0) || (spelly > 1023))
                                        goto isretry
                                                ;
                                    if (((bt[spelly][spellx] >> 10) & (1 + 2 + 4)) == 0) goto isretry;
                                    //land, air OR sea passable
                                    if (od[spelly][spellx]) goto isretry;
                                    if (stormcloakcheck2(spellx, spelly, tplayer)) goto isretry;

                                    myobj2 = OBJnew();
                                    myobj2->type = 343; //insects
                                    myobj2->more = malloc(sizeof(creature));
                                    ZeroMemory(myobj2->more, sizeof(creature));
                                    crt = (creature *) myobj2->more;
                                    crt->crt_struct = TRUE;
                                    crt->hp = (obji[sprlnk[myobj2->type & 1023]].v8 >> 8) * 4;
                                    crt->mp = rnd * 9;
                                    crt->al = 4;
                                    crt->respawn_x = 1024; //seconds till spawned crt will disappear
                                    myobj9 = OBJnew();
                                    myobj9->type = 188;
                                    crt->items = myobj9; //(not included)bag
                                    myobj2->info |= 4; //<-crt
                                    OBJadd(spellx, spelly, myobj2);
                                } //x5
                            } //x4
                            i2 = 1;
                            goto spelldone;
                        } //insect swarm


                        if (CASTSPELL_SPELLTYPE == ((5 << 4) + 8)) {
                            //replicate //txtset(spellname[(5<<4)+8],"Replicate"); spellreagent[(5<<4)+8]=SULF|SPID|BLOO|GINS|NIGH;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            //COST OF SPELL IS reagents=8 gold, 50% fail rate ->16gold per cast!

                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) {
                                if (myobj2->info & 256) goto cantreplicate; //quest item
                                if (obji[sprlnk[myobj2->type & 1023]].weight == 0) goto cantreplicate; //no weight!
                                if (obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].v1) goto cantreplicate;
                                //v1 (damage)
                                if (obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].v2) goto cantreplicate;
                                //v2 (armour)
                                if (obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].flags & 127)
                                    goto
                                            cantreplicate; //can be equipped
                                if (obji[sprlnk[myobj2->type & 1023]].flags & 1024) goto cantreplicate; //container
                                //EXCEPTIONS!
                                if ((myobj2->type & 1023) == 87) goto cantreplicate; //orb of the moons(kals)
                                if (myobj2->type == (223 + 1024)) goto cantreplicate; //lit powderkeg
                                if ((myobj2->type & 1023) == 414) goto cantreplicate; //skiff
                                if ((myobj2->type & 1023) == 149) goto cantreplicate; //deed
                                if ((myobj2->type & 1023) == 275) goto cantreplicate; //potion
                                if ((myobj2->type & 1023) == 58) goto cantreplicate; //spell
                                if ((myobj2->type & 1023) == 64) goto cantreplicate; //key
                                if ((myobj2->type & 1023) == 151) goto cantreplicate; //book
                                if ((myobj2->type & 1023) == 152) goto cantreplicate; //scroll
                                if ((myobj2->type & 1023) == 77) goto cantreplicate; //gem
                                if ((myobj2->type & 1023) == 93) goto cantreplicate; //sextant
                                if ((myobj2->type & 1023) >= 342) goto cantreplicate; //giant rat->horse
                                //***THE ABOVE LINE NEGATES ALL OBJECTS ABOVE TYPE 342, THIS HAS BEEN LEFT AS IS TO AVOID MISTAKES WHEN ADDING NEW ITEMS LATER***
                                if (myobj2->type == 448) goto cantreplicate; //horse papers

                                //PENDING quest items
                                if ((myobj2->type & 1023) == 61) goto cantreplicate; //book of circles
                                if ((myobj2->type & 1023) == 212) goto cantreplicate; //fan
                                if ((myobj2->type & 1023) == 277) goto cantreplicate;
                                //yew board (***possibly not a quest item)

                                //find safe location for replicated object!
                                x3 = tpx + x;
                                x4 = tpy + y;
                                x3++;
                                if (myobj3 = OBJfindlast(x3, x4)) {
                                    if (obji[sprlnk[myobj3->type & 1023] + (myobj3->type >> 10)].flags & 512)
                                        goto
                                                gotreploc;
                                }
                                if (bt[x4][x3] & 1024) goto gotreploc;
                                x3 = tpx + x;
                                x4 = tpy + y;
                                x3--;
                                if (myobj3 = OBJfindlast(x3, x4)) {
                                    if (obji[sprlnk[myobj3->type & 1023] + (myobj3->type >> 10)].flags & 512)
                                        goto
                                                gotreploc;
                                }
                                if (bt[x4][x3] & 1024) goto gotreploc;
                                x3 = tpx + x;
                                x4 = tpy + y;
                                x4++;
                                if (myobj3 = OBJfindlast(x3, x4)) {
                                    if (obji[sprlnk[myobj3->type & 1023] + (myobj3->type >> 10)].flags & 512)
                                        goto
                                                gotreploc;
                                }
                                if (bt[x4][x3] & 1024) goto gotreploc;
                                x3 = tpx + x;
                                x4 = tpy + y;
                                x4--;
                                if (myobj3 = OBJfindlast(x3, x4)) {
                                    if (obji[sprlnk[myobj3->type & 1023] + (myobj3->type >> 10)].flags & 512)
                                        goto
                                                gotreploc;
                                }
                                if (bt[x4][x3] & 1024) goto gotreploc;
                                x3 = tpx + x;
                                x4 = tpy + y;
                                if (myobj3 = OBJfindlast(x3, x4)) {
                                    if (obji[sprlnk[myobj3->type & 1023] + (myobj3->type >> 10)].flags & 512)
                                        goto
                                                gotreploc;
                                }
                                if (bt[x4][x3] & 1024) goto gotreploc;
                                goto cantreplicate; //no space!

                            gotreploc:
                                spellx = rnd * (8 + 5 * 6);
                                spelly = rnd * (tnpc->i + 1);
                                if (spelly >= spellx) {
                                    //success
                                    i2 = 1;
                                    myobj3 = OBJnew();
                                    myobj3->type = myobj2->type;
                                    myobj3->info = myobj2->info;
                                    myobj3->more2 = myobj2->more2;
                                    if (obji[sprlnk[myobj2->type & 1023] + (myobj2->type >> 10)].flags & 4096)
                                        myobj3->more2 = 1;
                                    myobj3->info |= 112; //set flags as temp OBJ
                                    if (myobj3->info & 128) myobj3->info -= 128; //remove stealing flag!
                                    OBJadd(x3, x4, myobj3);
                                } else {
                                    i2 = 3;
                                }
                            } //myobj2
                        cantreplicate:
                            goto spelldone;
                        } //replicate


                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 4)) {
                            //gate travel txtset(spellname[(6<<4)+4],"GateTravel"); spellreagent[(6<<4)+4]=SULF|BLAC|MAND;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            if (od[tpy + y][tpx + x]) goto gatetravel_cantcreate;
                            if ((bt[tpy + y][tpx + x] & 1024) == 0) goto gatetravel_cantcreate;
                            i2 = 1;
                            x3 = rnd * (8 + 5 * 7);
                            x4 = rnd * (tnpc->i + 100);
                            if (x4 >= x3) {
                                //success
                                myobj2 = OBJnew();
                                myobj2->type = 84 + 1024;
                                myobj2->info = 3 << 4; //set flags as temp OBJ
                                OBJadd(tpx + x, tpy + y, myobj2);
                                myobj2 = OBJnew();
                                myobj2->type = 84;
                                myobj2->info = 3 << 4; //set flags as temp OBJ
                                OBJadd(tpx + x - 1, tpy + y, myobj2);
                            } else { i2 = 3; } //spell failed

                        gatetravel_cantcreate:
                            goto spelldone;
                        } //gatetravel

                        if (CASTSPELL_SPELLTYPE == ((1 << 4) + 8)) {
                            //untrap //txtset(spellname[(1<<4)+8],"Untrap"); spellreagent[(1<<4)+8]=SULF|BLOO;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            if (myobj2 = od[tpy + y][tpx + x]) {
                            untrap_nextobj:
                                if (myobj2->type == 173) goto untrap_trapfound;
                                if (myobj2->next) {
                                    myobj2 = (object *) myobj2->next;
                                    goto untrap_nextobj;
                                }
                            }
                            goto spelldone; //no trap to remove!
                        untrap_trapfound:
                            i2 = 3;
                            x3 = rnd * (8 + 5 * 2);
                            x4 = rnd * (tnpc->i + 1);
                            if (x4 < x3) goto spelldone;
                            i2 = 1;
                            OBJremove(myobj2);
                            txtset(t, "?");
                            t->d2[0] = 8;
                            txtadd(t, "Trap destroyed!");
                            NET_send(NETplayer, tplayer->net, t);
                            goto spelldone;
                        } //untrap

                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 2)) {
                            //detect trap txtset(spellname[(0<<4)+2],"DetectTrap"); spellreagent[(0<<4)+2]=NIGH|SULF;
                            object *caster_obj = npc_to_obj(tnpc, tplayer);
                            i2 = spell_detect_trap(tplayer, tnpc, NULL, caster_obj->x, caster_obj->y);
                            // Always relative to position of caster, not casting point or player
                            goto spelldone;
                        } //detect trap

                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 0)) {
                            //create food txtset(spellname[(0<<4)+0],"CreateFood"); spellreagent[(0<<4)+0]=GARL|GINS|MAND; //level 1
                            i2 = spell_create_food(tplayer, tnpc, NULL, tpx + x, tpy + y);
                            goto spelldone;
                        } //create food

                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 3)) {
                            //fire field txtset(spellname[(3<<4)+3],"FireField"); spellreagent[(3<<4)+3]=BLAC|SULF|SPID;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            //NEWCODE
                            x3 = 0;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) { x3 = myobj2->type & 1023; }
                            if ((x3 == 317) || (x3 == 318) || (x3 == 320) || (x3 == 173) || (x3 == 53)) {
                                goto spelldone;
                            }
                            //NEWCODEEND
                            if (bt[tpy + y][tpx + x] & 1024) {
                                i2 = 1;
                                x3 = rnd * (8 + 5 * 4);
                                x4 = rnd * (tnpc->i + 1);
                                if (x4 >= x3) {
                                    //success
                                    myobj2 = OBJnew();
                                    myobj2->type = 317; //fire
                                    myobj2->more2 = tnpc->player->id;
                                    OBJadd(tpx + x, tpy + y, myobj2);
                                    myobj2->info += (2 << 4);
                                    OBJcheckflags(myobj2->x, myobj2->y);
                                } else { i2 = 3; } //spell failed
                            } //location OK
                            goto spelldone;
                        } //fire field

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 0)) {
                            //energy field //txtset(spellname[(4<<4)+0],"EnergyField"); spellreagent[(4<<4)+0]=MAND|SPID|BLAC; //level 5
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            //NEWCODE
                            x3 = 0;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) { x3 = myobj2->type & 1023; }
                            if ((x3 == 317) || (x3 == 318) || (x3 == 320) || (x3 == 173)) { goto spelldone; }
                            //NEWCODEEND
                            if (bt[tpy + y][tpx + x] & 1024) {
                                i2 = 1;
                                x3 = rnd * (8 + 5 * 5);
                                x4 = rnd * (tnpc->i + 1);
                                if (x4 >= x3) {
                                    //success
                                    myobj2 = OBJnew();
                                    myobj2->type = 319; //protection field
                                    myobj2->more2 = tnpc->player->id;
                                    OBJadd(tpx + x, tpy + y, myobj2);
                                    myobj2->info += (2 << 4);
                                    OBJcheckflags(myobj2->x, myobj2->y);
                                } else { i2 = 3; } //spell failed
                            } //location OK
                            goto spelldone;
                        } //energy field

                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 7)) {
                            //poison field txtset(spellname[(3<<4)+7],"PoisonField"); spellreagent[(3<<4)+7]=NIGH|SPID|BLAC;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            x3 = 0;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) { x3 = myobj2->type & 1023; }
                            if ((x3 == 317) || (x3 == 318) || (x3 == 320) || (x3 == 173) || (x3 == 53)) {
                                goto spelldone;
                            }
                            if (bt[tpy + y][tpx + x] & 1024) {
                                i2 = 1;
                                x3 = rnd * (8 + 5 * 4);
                                x4 = rnd * (tnpc->i + 1);
                                if (x4 >= x3) {
                                    //success
                                    myobj2 = OBJnew();
                                    myobj2->type = 318; //poison field
                                    myobj2->more2 = tnpc->player->id;
                                    OBJadd(tpx + x, tpy + y, myobj2);
                                    myobj2->info += (2 << 4);
                                    OBJcheckflags(myobj2->x, myobj2->y);
                                } else { i2 = 3; } //spell failed
                            } //location OK
                            goto spelldone;
                        } //poison field

                        if (CASTSPELL_SPELLTYPE == ((2 << 4) + 1)) {
                            //dispel field //txtset(spellname[(2<<4)+1],"DispelField"); spellreagent[(2<<4)+1]=BLAC|SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) {
                                x3 = myobj2->type & 1023;
                                if ((x3 >= 317) && (x3 <= 320)) {
                                    i2 = 1;
                                    x3 = rnd * (8 + 5 * 3);
                                    x4 = rnd * (tnpc->i + 1);
                                    if (x4 >= x3) {
                                        //success
                                        OBJsave(myobj2->x, myobj2->y);
                                        OBJremove(myobj2);
                                    } else { i2 = 3; } //spell failed
                                } //x3
                            } //myobj2
                            goto spelldone;
                        } //dispel field


                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 5)) {
                            //locate //txtset(spellname[(3<<4)+5],"Locate"); spellreagent[(3<<4)+5]=NIGH;
                            i2 = 2; //implemented!
                            x3 = rnd * (8 + 5 * 4);
                            x4 = rnd * (tnpc->i + 1);
                            if (x4 >= x3) {
                                //success
                                i2 = 1;
                                if (myobj->x < 1024) {
                                    x4 = (myobj->x + 1024 - 304) / 8;
                                    x4 -= 128;
                                    y4 = (myobj->y + 1024 - 360) / 8;
                                    y4 -= 128;
                                } else {
                                    x4 = ((myobj->x & 255) + 256 - 76) / 2;
                                    x4 -= 128;
                                    y4 = ((myobj->y & 255) + 256 - 90) / 2;
                                    y4 -= 128;
                                }
                                txtset(t, "?");
                                t->d2[0] = 8;
                                txtnumint(t2, abs(y4));
                                txtadd(t, t2);
                                if (y4 > 0) txtadd(t, "'S, ");
                                else txtadd(t, "'N, ");
                                txtnumint(t2, abs(x4));
                                txtadd(t, t2);
                                if (x4 > 0) txtadd(t, "'E");
                                else txtadd(t, "'W");
                                NET_send(NETplayer, tplayer->net, t);
                            } else { i2 = 3; }
                            goto spelldone;
                        } //locate

                        if (CASTSPELL_SPELLTYPE == ((4 << 4) + 6)) {
                            //pickpocket //txtset(spellname[(4<<4)+6],"PickPocket"); spellreagent[(4<<4)+6]=BLOO|SPID|NIGH;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more;

                                x5 = 0;
                                x6 = 0;
                                if (myobj2->info & 2) {
                                    //<-npc
                                    x5 = 1;
                                    tnpc2 = (npc *) myobj2->more;
                                } //npc
                                if (myobj2->info & 4) {
                                    //<-crt
                                    x5 = 2;
                                    crt = (creature *) myobj2->more;
                                } //crt
                                if (x5) {
                                    x3 = rnd * (8 + 5 * 5);
                                    x4 = rnd * (tnpc->i + 1);
                                    if (x4 >= x3) {
                                        //success
                                        i2 = 1;
                                        if (x5 == 1) {
                                            //->npc
                                            if (tnpc2->converse == 180) {
                                                if ((tplayer->GNPCflags[6] & (1 << 0)) == 0) {
                                                    tplayer->GNPCflags[6] |= (1 << 0);
                                                    myobj3 = OBJnew();
                                                    myobj3->type = 25;
                                                    myobj3->info |= 256; //guild belt
                                                    i9 = SFnew(myobj->x, myobj->y);
                                                    //destination is more important than the source
                                                    sf[i9].type = 9; //GENERIC throw object
                                                    sf[i9].x = myobj2->x;
                                                    sf[i9].y = myobj2->y;
                                                    sf[i9].x2 = myobj->x;
                                                    sf[i9].y2 = myobj->y;
                                                    sf[i9].more = 0xFFFF;
                                                    sf[i9].wait = myobj3->type;
                                                    //add to inventory
                                                    additemroot(tnpc, myobj3);

                                                    tnpc->upflags |= 32; //inv
                                                    goto spelldone;
                                                } //global flag
                                            } //80
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "Nothing!");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto spelldone;
                                        } //npc

                                        //crt
                                        if (myobj3 = (object *) crt->items->more) {
                                            if ((myobj3->type & 1023) == 98) {
                                                if (myobj3->next == NULL) goto pickpocketcrtnothing;
                                            }
                                        pickpocketcrtnext:
                                            x3 = rnd * 2;
                                            if (myobj3->next && x3) {
                                                myobj3 = (object *) myobj3->next;
                                                goto pickpocketcrtnext;
                                            }
                                            if ((myobj3->type & 1023) == 98) {
                                                myobj3 = (object *) crt->items->more;
                                                goto pickpocketcrtnext;
                                            }

                                            if (myobj3->type == 88) {
                                                if (myobj3->more2 > 32) {
                                                    myobj4 = OBJnew();
                                                    myobj4->type = 88;
                                                    myobj4->info = 112;
                                                    myobj4->more2 = (rnd * (myobj3->more2 - 1)) + 1;
                                                    myobj3->more2 -= myobj4->more2;
                                                    myobj3 = myobj4;
                                                    goto pickpocketcrtskipremove;
                                                }
                                            }

                                            OBJremove(myobj3);
                                        pickpocketcrtskipremove:

                                            i9 = SFnew(myobj->x, myobj->y);
                                            //destination is more important than the source
                                            sf[i9].type = 9; //GENERIC throw object
                                            sf[i9].x = myobj2->x;
                                            sf[i9].y = myobj2->y;
                                            sf[i9].x2 = myobj->x;
                                            sf[i9].y2 = myobj->y;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = myobj3->type;
                                            //add to inventory
                                            additemroot(tnpc, myobj3);
                                            tnpc->upflags |= 32; //inv
                                        } else {
                                        pickpocketcrtnothing:
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(t, "Nothing!");
                                            NET_send(NETplayer, tplayer->net, t);
                                        }
                                    } else { i2 = 3; } //failed
                                } //x3
                            } //myobj2
                            goto spelldone;
                        } //pickpocket

                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 3)) {
                            //cure //txtset(spellname[(0<<4)+3],"Cure"); spellreagent[(0<<4)+3]=GARL|GINS;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        x3 = rnd * (8 + 5 * 1);
                                        x4 = rnd * (tnpc->i + 1);
                                        if (x4 >= x3) {
                                            //success
                                            i2 = 1;
                                            tnpc2->flags &= (65535 - 1); //remove poisoned flag
                                            tnpc2->upflags |= 2;
                                            i9 = SFnew(myobj2->x, myobj2->y);
                                            //destination is more important than the source
                                            sf[i9].type = 14; //blue ball
                                            sf[i9].x = myobj->x;
                                            sf[i9].y = myobj->y;
                                            sf[i9].x2 = myobj2->x;
                                            sf[i9].y2 = myobj2->y;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        } else { i2 = 3; } //success
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //cure

                        if (CASTSPELL_SPELLTYPE == ((0 << 4) + 6)) {
                            //heal
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            myobj2 = OBJfindlast(tpx + x, tpy + y);
                            if (myobj2 == NULL) myobj2 = OBJfindlastall(tpx + x, tpy + y);
                            if (myobj2) {
                                if (myobj2->info & 2) {
                                    //<-npc
                                    tnpc2 = (npc *) myobj2->more;
                                    if (tnpc2->player) {
                                        //perform light heal (1 to 16->32 hp)
                                        x3 = rnd * (16 + (tnpc->i >> 2)); //int can only double spell effectiveness!
                                        x4 = rnd * (16 + (tnpc->i >> 2)); //int can only double spell effectiveness!
                                        x3 = (x3 + x4) >> 1;

                                        tnpc2->hp += x3;
                                        if (tnpc2->hp > tnpc2->hp_max) tnpc2->hp = tnpc2->hp_max;
                                        tnpc2->upflags |= 2;
                                        i2 = 1;
                                        if (x3 == 0) i2 = 3;
                                        if (x3) {
                                            i9 = SFnew(myobj2->x, myobj2->y);
                                            //destination is more important than the source
                                            sf[i9].type = 14; //blue ball
                                            sf[i9].x = myobj->x;
                                            sf[i9].y = myobj->y;
                                            sf[i9].x2 = myobj2->x;
                                            sf[i9].y2 = myobj2->y;
                                            sf[i9].more = 0xFFFF;
                                            sf[i9].wait = 1;
                                        }
                                    } //player
                                } //npc
                            } //myobj2
                            goto spelldone;
                        } //heal

                        if (CASTSPELL_SPELLTYPE == ((6 << 4) + 1)) {
                            //enchant //txtset(spellname[(6<<4)+1],"Enchant"); spellreagent[(6<<4)+1]=SPID|MAND|SULF;
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) {
                                if (myobj2->type == 78) {
                                    //staff
                                    if ((myobj2->more2 >> 16) <= 20) {
                                        //25 spell limit (20+5=25)
                                        if ((myobj2->info & (7 << 9)) == 0) {
                                            //no pending enchantments
                                            x3 = rnd * (8 + 5 * 7);
                                            x4 = rnd * (tnpc->i + 1);
                                            if (x4 >= x3) {
                                                //success
                                                i2 = 1;
                                                //allow for 5 new spells to be added
                                                myobj2->info |= (5 << 9);
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(t, "The staff glows brightly!");
                                                NET_send(NETplayer, tplayer->net, t);
                                                txtset(t, "?");
                                                t->d2[0] = 8;
                                                txtadd(
                                                    t,
                                                    "To enchant this staff cast spells upon it. (You can now add up to 5 identical spells to this staff)");
                                                NET_send(NETplayer, tplayer->net, t);
                                                goto spelldone;
                                            } else {
                                                i2 = 3;
                                                goto spelldone;
                                            } //fail
                                            txtset(t, "?");
                                            t->d2[0] = 8;
                                            txtadd(
                                                t,
                                                "The staff already glows! To enchant this staff cast other spells upon it.");
                                            NET_send(NETplayer, tplayer->net, t);
                                            goto spelldone;
                                        } //no pending enchantments
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtnumint(t2, myobj2->more2 >> 16);
                                        txtadd(t, t2);
                                        txtadd(
                                            t,
                                            " spells are already held within this staff! No more may be added at this time.");
                                        NET_send(NETplayer, tplayer->net, t);
                                        goto spelldone;
                                    } //25 spell limit
                                } //staff
                            } //myobj2
                            goto spelldone;
                        } //enchant

                        if (CASTSPELL_SPELLTYPE == ((1 << 4) + 7)) {
                            //unlockmagic
                            i2 = 2; //implemented!
                            if (stormcloakcheck2(tpx + x, tpy + y, tplayer)) goto spelldone;
                            if (myobj2 = OBJfindlast(tpx + x, tpy + y)) {
                                if (myobj2->info & 8) myobj2 = (object *) myobj2->more; //<-redirect
                                x3 = myobj2->type & 1023;
                                if ((x3 >= OBJ_OAKEN_DOOR) && (x3 <= OBJ_STEEL_DOOR)) {
                                    if ((myobj2->type >> 10) >= 12) {
                                        i2 = 1;
                                        x3 = rnd * (8 + 5 * 2);
                                        x4 = rnd * (tnpc->i + 1);
                                        if (x4 >= x3) {
                                            //success
                                            myobj2->type -= (8 * 1024);
                                            myobj3 = (object *) myobj2->more;
                                            myobj3->type -= (8 * 1024);
                                        } else { i2 = 3; } //success
                                    } //>=12
                                } //x3

                                if (myobj2->type == (OBJ_CHEST + (3 * 1024))) {
                                    i2 = 1;
                                    x3 = rnd * (8 + 5 * 2);
                                    x4 = rnd * (tnpc->i + 1);
                                    if (x4 >= x3) {
                                        //success
                                        OBJsave(myobj2->x, myobj2->y);
                                        myobj2->type -= (2 * 1024);
                                    } else { i2 = 3; } //success
                                } //chest
                            } //myobj2
                            goto spelldone;
                        } //unlock

                        if (CASTSPELL_SPELLTYPE == ((3 << 4) + 9)) {
                            //WindChange txtset(spellname[(3<<4)+9],"WindChange"); spellreagent[(3<<4)+9]=SULF|BLOO;
                            i2 = 2; //implemented!
                            x3 = rnd * (8 + 5 * 4);
                            x4 = rnd * (tnpc->i + 1);
                            if (x4 >= x3) {
                                //success
                                i2 = 1;
                                x3 = tplayer->windx;
                                y3 = tplayer->windy;
                                if ((x3 == 0) && (y3 == 0)) {
                                    x3 = windx;
                                    y3 = windy;
                                }
                                if ((x3 == 0) && (y3 == 0)) {
                                    x3 = 0;
                                    y3 = 1;
                                    goto windchange_cast;
                                }
                                if ((x3 == 0) && (y3 == 1)) {
                                    x3 = -1;
                                    y3 = 1;
                                    goto windchange_cast;
                                }
                                if ((x3 == -1) && (y3 == 1)) {
                                    x3 = -1;
                                    y3 = 0;
                                    goto windchange_cast;
                                }
                                if ((x3 == -1) && (y3 == 0)) {
                                    x3 = -1;
                                    y3 = -1;
                                    goto windchange_cast;
                                }
                                if ((x3 == -1) && (y3 == -1)) {
                                    x3 = 0;
                                    y3 = -1;
                                    goto windchange_cast;
                                }
                                if ((x3 == 0) && (y3 == -1)) {
                                    x3 = 1;
                                    y3 = -1;
                                    goto windchange_cast;
                                }
                                if ((x3 == 1) && (y3 == -1)) {
                                    x3 = 1;
                                    y3 = 0;
                                    goto windchange_cast;
                                }
                                if ((x3 == 1) && (y3 == 0)) {
                                    x3 = 1;
                                    y3 = 1;
                                    goto windchange_cast;
                                }
                                if ((x3 == 1) && (y3 == 1)) {
                                    x3 = 0;
                                    y3 = 1;
                                    goto windchange_cast;
                                }
