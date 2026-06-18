// LHS-P3.4: part_a_sobj -- CONTINUES MEGA A. WIRE-COUPLED (sobj encode).
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_a_save.cpp.
// Contents: sobj buffer build (sobj_copyloop, screenplus1check_*, objbuffer*).
// Move-only: no encode/layout edits, no U6O_VERSION bump.
                sobj_copyloop:
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
                                    goto sobj_copiedpos0;
                                }
                            }
                        }
                    }
                    //clear position
                    tplayer->sobj_tempfixed[x9][y9] = 0;
                    tplayer->sobj_bufsize[x9][y9] = 0;
                sobj_copiedpos0:;

                    if (i == 0) {
                        x9++;
                        if (x9 >= 96) {
                            y9++;
                            x9 = 0;
                        }
                        if (y9 < 72) goto sobj_copyloop;
                    }
                    if (i == 1) {
                        x9--;
                        if (x9 < 0) {
                            y9++;
                            x9 = 95;
                        }
                        if (y9 < 72) goto sobj_copyloop;
                    }
                    if (i == 2) {
                        x9++;
                        if (x9 >= 96) {
                            y9--;
                            x9 = 0;
                        }
                        if (y9 >= 0) goto sobj_copyloop;
                    }
                    if (i == 3) {
                        x9--;
                        if (x9 < 0) {
                            y9--;
                            x9 = 95;
                        }
                        if (y9 >= 0) goto sobj_copyloop;
                    }
                    tplayer->sobj_bufoffx = x2;
                    tplayer->sobj_bufoffy = y2;
                    //MessageBox(NULL,"shiftbuffer: buffer shifted","Ultima 6 Online",MB_OK);
                bufferrelocatecomplete0:
                    //ERROR is it overwriting data it needs to read as it goes
                    //OPTOMIZATION!!! switch the pointers to the data and data buffer lengths instead of the data
                    //[17:25] galleondragon: and screen+8 area is reloaded in centre
                    /*
          for (y9=0;y9<=71;y9++){
          for (x9=0;x9<=95;x9++){
          if (tplayer->sobj_tempfixed[x9][y9]||tplayer->sobj_bufsize[x9][y9]){
          objbufferf->graphic->o2[x9+y9*96]=0xFFFF;
          }else{
          objbufferf->graphic->o2[x9+y9*96]=31;
          }
          }}
          */

                    //MessageBox(NULL,"update required","Ultima 6 Online",MB_OK);

                screenplus1check_entry:

                    //refresh pathfinding array
                    //if (tplayer->xray){ //xray!
                    //for (x2=0;x2<=65535;x2++) pfa[x2]=PFs+PFd;
                    //}else{
                    //pathfind2(x,y,NULL);
                    //}

                    //buffer must be updated while message is created!
                    static long mapx, mapy, bufx, bufy;
                    static unsigned short vbuf[1024];
                    // RW sobj-fix: iterate SOBJ_TX_W x SOBJ_TX_H (was 48x40). Window is
                    // sized to cover the max resizable viewport (63x47) + an 8-tile
                    // fence on each side ("screen+8" in legacy lingo). mapx,mapy still
                    // use the legacy tpx/tpy reference frame -- the offset is now
                    // SOBJ_TX_OFFX/Y (was a literal 8). bufx,bufy then index the per-
                    // player 96x72 sobj_*[][] buffers, which already have enough slack
                    // to hold the larger window (see define_both.h capacity check).
                    // All five sites -- this fill loop, the screenplus1check inner
                    // gate just below, the three tempfixed encodes, and the per-square
                    // object encode in the t2 stream -- must use the same constants;
                    // the client decoder in loop_client.cpp must mirror them or sobj
                    // positions misdecode.
                    for (y = 0; y < SOBJ_TX_H; y++) {
                        for (x = 0; x < SOBJ_TX_W; x++) {
                            mapx = tpx + x - SOBJ_TX_OFFX;
                            mapy = tpy + y - SOBJ_TX_OFFY;
                            bufx = mapx - tplayer->sobj_bufoffx;
                            bufy = mapy - tplayer->sobj_bufoffy;

                            /*
            x4=0; z4=2;
            for (y=0;y<=25;y++){ for (x=0;x<=33;x++){
            if (pfa[x+y*256+257]&(PFs+PFd)) z5=1; else z5=0;
            if (z5!=z4){
            if (z4!=2){ //not the first pass
            *p=x4+z4*128; p++;
            }//z4!=2
            x4=0; //number of squares in a row -1
            z4=z5; //type (1 or 0)
            }else{
            if (x4==127){
            *p=127+z4*128; p++;
            x4=0;
            }else{
            x4++;
            }
            }
            }}//x,y
            *p=x4+z4*128; p++; //save last row
            */


                            if (screenplus1check) {
                                // RW sobj-fix: inner gate skips tiles outside the screen+1
                                // inset. Inset = SOBJ_TX_OFFX - SOBJ_S1_LEFT = 7 (legacy fence
                                // delta 8-1). Was hardcoded 7/40/7/32 for the legacy 48x40
                                // window. See define_both.h SOBJ_S1_INSET.
                                if (x < SOBJ_S1_INSET) goto objbufnoupdate0;
                                if (y < SOBJ_S1_INSET) goto objbufnoupdate0;
                                if (x > (SOBJ_TX_W - 1 - SOBJ_S1_INSET)) goto objbufnoupdate0;
                                if (y > (SOBJ_TX_H - 1 - SOBJ_S1_INSET)) goto objbufnoupdate0;
                            }

                            i = 0;

                            // ROOMSYNC-P1.4: if the player is in a registered isolated room, only
                            // stream sobj entries from tiles inside that same room. Prevents
                            // items/doors/signs from the neighbouring open map (e.g. gargoyle
                            // land at x<1280 next to the Guardian Guild basement) from bleeding
                            // into the basement view. This check MUST run HERE -- after bufx/bufy
                            // and i=0 are set, jumping to the SAME objbuffer_outofrange handler
                            // the world-bounds checks below use. The original ROOMSYNC-P1
                            // placement jumped BEFORE bufx/bufy/i were initialised for the
                            // current tile, so objbuffer_outofrange compared a STALE buffer cell
                            // against a STALE object count, corrupting the per-cell sobj diff
                            // state for in-room cells touched on prior iterations. That is why
                            // ground items dropped inside the Guardian Guild basement never
                            // synced to other clients. Treating an out-of-room tile exactly like
                            // an out-of-world tile (current cell, i==0) lets the existing
                            // length-check path correctly clear the cell instead.
                            if (playerroom_inroom) {
                                if ((mapx < playerroom_x0) || (mapx > playerroom_x1) ||
                                    (mapy < playerroom_y0) || (mapy > playerroom_y1))
                                    goto objbuffer_outofrange;
                            }

                            if (mapx < 0) goto objbuffer_outofrange;
                            if (mapx > 2047) goto objbuffer_outofrange;
                            if (mapy < 0) goto objbuffer_outofrange;
                            if (mapy > 1023) goto objbuffer_outofrange;
                            myobj = od[mapy][mapx];
                        objskip0:
                            if (myobj) {
                                if (objvisible(tplayer, myobj) == NULL) {
                                    myobj = (object *) myobj->next;
                                    goto objskip0;
                                }
                                if (myobj->type == 416) {
                                    //view redirector
                                    if (myobj->more2) {
                                        bt[mapy][mapx] = bt[myobj->more2 >> 16][myobj->more2 & 65535];
                                        mapx = myobj->more2 & 65535;
                                        mapy = myobj->more2 >> 16;
                                        myobj = od[myobj->more2 >> 16][myobj->more2 & 65535];
                                        goto objskip0;
                                    }
                                } //view redirector
                                if (!tclass_object[myobj->type]) {
                                    myobj = (object *) myobj->next;
                                    goto objskip0;
                                }
                            } //myobj

                            if (i3 = tobjfixed_index[mapy][mapx]) {
                                i4 = tobjfixed_type[i3];
                                i5 = 1;
                                i6 = 1;
                                z3 = getnbits(i4);
                                for (i2 = i3 + 1; i2 <= (i3 + i4); i2++) {
                                    if (myobj) {
                                        if ((myobj->type & 1023) == OBJ_BLOOD) goto noflash0; //blood
                                        if ((myobj->type & 1023) == OBJ_FIRE_FIELD) goto noflash0; //317 fire field
                                        if ((myobj->type & 1023) == OBJ_POISON_FIELD) goto noflash0; //318 poison field
                                        if ((myobj->type & 1023) == OBJ_PROTECTION_FIELD) goto noflash0;
                                        //319 protection field
                                        if ((myobj->type & 1023) == OBJ_SLEEP_FIELD) goto noflash0; //320 sleep field
                                        if ((myobj->type & 1023) == OBJ_WEB) goto noflash0; //53 web
                                        if ((myobj->type & 1023) == OBJ_RED_GATE) {
                                            //red gate
                                            if (((myobj->info & 112) >> 4) >= 2) i6 = 0;
                                            goto noflash0;
                                        } //red gate
                                        if ((myobj->info & 112) == 16) i6 = 0;
                                    noflash0:
                                        if (objvisible(tplayer, myobj) == NULL) myobj = (object *) myobj->next;
                                    }

                                    if (myobj && i6) {
                                        if (myobj->type == tobjfixed_type[i2]) {
                                            //object still exists
                                            if (tplayer->sobj_tempfixed[bufx][bufy] & i5) {
                                                //but buffer flag is set, saying it is not there!
                                                if (screenplus1check) goto screenplus1check_return;
                                                //********SCREENPLUS1CHECK********
                                                tplayer->sobj_tempfixed[bufx][bufy] -= i5;
                                                z = 1;
                                                BITSadd(t, &bitsi, z, 1); //edit another tempfixed object
                                                // RW sobj-fix: was y*48+x in 11 bits; now y*SOBJ_TX_W+x
                                                // in SOBJ_TX_BITS. Client decoder mirrors this.
                                                z = y * SOBJ_TX_W + x;
                                                BITSadd(t, &bitsi, z, SOBJ_TX_BITS); //x,y offset
                                                if (z3) {
                                                    z = i2 - i3 - 1;
                                                    BITSadd(t, &bitsi, z, z3);
                                                } //which tempfixed obj

                                                //MessageBox(NULL,"tempfixed change","Ultima 6 Online",MB_OK);
                                            } //sobj_tempfixed[bufx][bufy]&i5
                                            myobj = (object *) myobj->next;
                                        } else {
                                            //object does not exist
                                            if ((tplayer->sobj_tempfixed[bufx][bufy] & i5) == 0) {
                                                if (screenplus1check) goto screenplus1check_return;
                                                //********SCREENPLUS1CHECK********
                                                tplayer->sobj_tempfixed[bufx][bufy] |= i5;
                                                z = 1;
                                                BITSadd(t, &bitsi, z, 1); //edit another tempfixed object
                                                z = y * SOBJ_TX_W + x;
                                                BITSadd(t, &bitsi, z, SOBJ_TX_BITS); //x,y offset (RW sobj-fix)
                                                if (z3) {
                                                    z = i2 - i3 - 1;
                                                    BITSadd(t, &bitsi, z, z3);
                                                } //which tempfixed obj

                                                //MessageBox(NULL,"tempfixed change","Ultima 6 Online",MB_OK);

                                                i6 = 0;
                                            } //(sobj_tempfixed[bufx][bufy]&i5)==0
                                        }
                                    } else {
                                        //object is not present
                                        if ((tplayer->sobj_tempfixed[bufx][bufy] & i5) == 0) {
                                            if (screenplus1check) goto screenplus1check_return;
                                            //********SCREENPLUS1CHECK********
                                            tplayer->sobj_tempfixed[bufx][bufy] |= i5;
                                            z = 1;
                                            BITSadd(t, &bitsi, z, 1); //edit another tempfixed object
                                            z = y * SOBJ_TX_W + x;
                                            BITSadd(t, &bitsi, z, SOBJ_TX_BITS); //x,y offset (RW sobj-fix)
                                            if (z3) {
                                                z = i2 - i3 - 1;
                                                BITSadd(t, &bitsi, z, z3);
                                            } //which tempfixed obj

                                            //MessageBox(NULL,"tempfixed change","Ultima 6 Online",MB_OK);
                                        } //(sobj_tempfixed[bufx][bufy]&i5)==0
                                    }
                                    i5 <<= 1;
                                } //i2
                            } //tobjfixed_index[mapy][mapx]

                            wizardeyesadded = 0;

                        nextbufobj0:
                            if (myobj) {
                                if (objvisible(tplayer, myobj) == NULL) {
                                    myobj = (object *) myobj->next;
                                    goto nextbufobj0;
                                }
                                if (!tclass_object[myobj->type]) {
                                    myobj = (object *) myobj->next;
                                    goto nextbufobj0;
                                }
                                vbuf[i] = myobj->type;
                                if ((myobj->type & 1023) == OBJ_BLOOD) goto noflash1; //blood
                                if ((myobj->type & 1023) == OBJ_FIRE_FIELD) goto noflash1; //317 fire field
                                if ((myobj->type & 1023) == OBJ_POISON_FIELD) goto noflash1; //318 poison field
                                if ((myobj->type & 1023) == OBJ_PROTECTION_FIELD) goto noflash1; //319 protection field
                                if ((myobj->type & 1023) == OBJ_SLEEP_FIELD) goto noflash1; //320 sleep field
                                if ((myobj->type & 1023) == OBJ_WEB) goto noflash1; //53 web
                                if ((myobj->type & 1023) == OBJ_RED_GATE) {
                                    //red gate
                                    if (((myobj->info & 112) >> 4) >= 2) vbuf[i] |= 32768;
                                    goto noflash1;
                                } //red gate
                                if ((myobj->info & 112) == 16) vbuf[i] |= 32768;
                            noflash1:

                                i++;
                                myobj = (object *) myobj->next;
                                goto nextbufobj0;
                            } //myobj

                            //add wizard eye if necessary
                            if (!wizardeyesadded) {
                                for (wizardeyesi2 = 1; wizardeyesi2 <= wizardeyesi; wizardeyesi2++) {
                                    if (mapx == wizardeyesx[wizardeyesi2]) {
                                        if (mapy == wizardeyesy[wizardeyesi2]) {
                                            if (tplayer->wizardeyetimeleft) {
                                                if ((tplayer->wizardeyex == mapx) && (tplayer->wizardeyey == mapy))
                                                    goto
                                                            wizardeyesloopnext;
                                            }
                                            wizardeyesadded = 1;
                                            myobj = wizardeyesobj;
                                            goto nextbufobj0;
                                        wizardeyesloopnext:;
                                        }
                                    } //x,y
                                } //wizardeyesi2
                            } //!wizardeyesadded


                            //short sobj_bufoffx,sobj_bufoffy; //x,y offset of left corner of entire buffer
                            //unsigned short sobj_buf2bytes[96][72]; //number of 2 byte elements allocated
                            //unsigned short sobj_bufsize[96][72]; //number of 2 byte elements actually used
                            //unsigned short *sobj[96][72]; //scene object raw data
                            //unsigned long  sobj_tempfixed[96][72]; //temp fixed objects
                            //compare buffers

                            //length check
                        objbuffer_outofrange:
                            if (tplayer->sobj_bufsize[bufx][bufy] != i) goto objbufupdate0;
                            if (i == 0) goto objbufnoupdate0;

                            tp2 = tplayer->sobj[bufx][bufy];
                            for (i2 = 0; i2 < i; i2++) {
                                if (tp2[i2] != vbuf[i2]) goto objbufupdate0;
                            }
                            goto objbufnoupdate0;
                        objbufupdate0:
                            if (screenplus1check) goto screenplus1check_return; //********SCREENPLUS1CHECK********

                            //update allocated buffer size
                            if (tplayer->sobj_buf2bytes[bufx][bufy] < i) {
                                if (tplayer->sobj[bufx][bufy]) free(tplayer->sobj[bufx][bufy]);
                                tplayer->sobj[bufx][bufy] = (unsigned short *) malloc(i * 2);
                                tplayer->sobj_buf2bytes[bufx][bufy] = i;
                            }


                            z = 1;
                            BITSadd(t2, &bitsi2, z, 1); //1 edit objects of another square
                            z = y * SOBJ_TX_W + x;
                            BITSadd(t2, &bitsi2, z, SOBJ_TX_BITS); //screen offset of square (RW sobj-fix)

                            //update buffer info
                            tp2 = tplayer->sobj[bufx][bufy];
                            for (i2 = 0; i2 < i; i2++) {
                                tp2[i2] = vbuf[i2];

                                z = 1;
                                BITSadd(t2, &bitsi2, z, 1);
                                z = vbuf[i2] & (65535 - 32768);
                                BITSadd(t2, &bitsi2, z, 16);
                                //10 object graphic index (optomized ***YET TO BE OPTOMIZED***)
                                z = 0;
                                if (vbuf[i2] & 32768) z = 1;
                                BITSadd(t2, &bitsi2, z, 1);
                            } //i2
                            z = 0;
                            BITSadd(t2, &bitsi2, z, 1);


                            tplayer->sobj_bufsize[bufx][bufy] = i;


                            /*
             1 edit objects of another square
             {
             11 screen offset of square
             {
             1 a/another object exists on this square
             10 object graphic index (optomized)
             1 object is flashing
             }
             }

             */


                            //...
                        objbufnoupdate0:;

                            //***** DONT FORGET FLASHING OBJECTS HERE AND IN TEMPFIXED BUFFER
                            //***** DONT FORGET NOT TO UPDATE BLACKED OUT SQUARES
                            //or off screen
                        }
                    }


                    if (screenplus1check) {
                        screenplus1check = 0;
                        z = 0;
                        BITSadd(t, &bitsi, z, 1); //screen+8 update unnecessary
                    } else {
                        z = 0;
                        BITSadd(t, &bitsi, z, 1); //no more tempfixed objects
                        z = 0;
                        BITSadd(t2, &bitsi2, z, 1); //no more squares of objects to edit
                    }

                    //add second bitstream to first bitstream
                    if (bitsi2) {
                        i = 0;
                        i2 = bitsi2 >> 3; //i2 is number of whole bytes
                        for (i = 0; i < i2; i++) {
                            BITSadd(t, &bitsi, t2->d2[i], 8);
                        }
                        i3 = bitsi2 % 8;
                        if (i3) BITSadd(t, &bitsi, t2->d2[i2], i3);
                    }


