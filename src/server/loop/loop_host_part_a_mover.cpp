// LHS-P3.5: part_a_mover -- CONTINUES MEGA A. WIRE-COUPLED (mover encode).
// LHS brace-seam: CONTINUES blocks opened in loop_host_part_a_save.cpp.
// Contents: mover add/remove + lighting (mvobjskip, mover_add_*,
// MV_LIGHTBRIGHT_assumed, MV_TX_* fill window). Move-only; no U6O_VERSION bump.
                    //mover update

                    //create new mover list
                    i = -1;
                    // RW dynamic-objects fix: iterate an MV_TX_W x MV_TX_H rectangle
                    // centered on the avatar (via MV_TX_OFFX/Y) so NPCs, monsters,
                    // animals, and other players are filled across the entire resizable
                    // client viewport -- not just the legacy 32x24 area. The host's
                    // tpx/tpy is the legacy view origin (avatar at column 15 / row 11
                    // since host-side viewTilesX/Y returns 32/24). All four sites --
                    // fill, offscreen-prune, encode (.cpp + .inc), decode (client) --
                    // must stay in lock-step or movers misdecode.
                    for (y = 0; y < MV_TX_H; y++) {
                        for (x = 0; x < MV_TX_W; x++) {
                            mapx = tpx + x - MV_TX_OFFX;
                            mapy = tpy + y - MV_TX_OFFY;

                            // ROOMSYNC-P1: same room-isolation rule as the
                            // sobj fill above -- if the player is in a
                            // registered isolated room, drop movers from
                            // tiles outside it. The historical "ghost NPC
                            // from gargoyle land in the basement" bug was
                            // this filter being missing.
                            if (playerroom_inroom) {
                                if ((mapx < playerroom_x0) || (mapx > playerroom_x1) ||
                                    (mapy < playerroom_y0) || (mapy > playerroom_y1))
                                    goto moverbuffer_outofrange;
                            }
                            if (mapx < 0) goto moverbuffer_outofrange;
                            if (mapx > 2047) goto moverbuffer_outofrange;
                            if (mapy < 0) goto moverbuffer_outofrange;
                            if (mapy > 1023) goto moverbuffer_outofrange;
                            // ROOMSYNC-P1.3: remember the display tile before the view-
                            // redirector branch below can overwrite mapx/mapy.
                            mv_dispx = mapx;
                            mv_dispy = mapy;
                            myobj = od[mapy][mapx];
                        mvobjskip:
                            if (myobj) {
                                if (myobj->type == 416) {
                                    //view redirector
                                    if (myobj->more2) {
                                        bt[mapy][mapx] = bt[myobj->more2 >> 16][myobj->more2 & 65535];
                                        mapx = myobj->more2 & 65535;
                                        mapy = myobj->more2 >> 16;
                                        myobj = od[myobj->more2 >> 16][myobj->more2 & 65535];
                                        goto mvobjskip;
                                    }
                                } //view redirector
                                if (!tclass_mover[myobj->type]) {
                                    myobj = (object *) myobj->next;
                                    goto mvobjskip;
                                }
                                //object is a mover class

                                //check for npc or crt pointer
                                //XXXX UNOCCUPIED BOATS, SKIFFS, RAFTS WON'T HAVE A CRT OR NPC POINTER
                                //if ((myobj->info&(2+4))==0){
                                //myobj=(object*)myobj->next;
                                //goto mvobjskip;
                                //}

                                if (!myobj->more) {
                                    if ((myobj->type & 1023) != 414) {
                                        if ((myobj->type & 1023) != 415) {
                                            //not a raft or skiff
                                            myobj = (object *) myobj->next;
                                            goto mvobjskip;
                                        }
                                    }
                                }


                                //add it to the list
                                i++;
                                mv_object[i] = myobj;
                                // RW dynamic-objects bug fix (2026-05-28): store the mover's actual
                                // world coords (mapx/mapy), not the legacy `tpx+x-1` offset. The
                                // fill loop above now iterates an MV_TX_W x MV_TX_H window with
                                // mapx = tpx+x-MV_TX_OFFX, but this assignment was left at the
                                // pre-fix `-1` offset, so every mover was stored 15 tiles east /
                                // 11 tiles south of the NPC's true position. Symptoms: NPCs and
                                // boats rendered scattered across the world on unwalkable tiles,
                                // duplicates appearing as movers walked (the host re-added them
                                // at fresh wrong coords each frame), avatar offset to bottom-
                                // right, and "walking just moves the camera" because the player's
                                // own mover sprite drifted away from the real player position.
                                // Not a wire change -- mv_x/mv_y are host-side state only.
                                // ROOMSYNC-P1.3: store the DISPLAY tile, not the (possibly
                                // redirected) source tile. Using the post-redirect source coord
                                // here put movers ~950 tiles outside the transmit window inside
                                // redirector interiors (the shop), so the diff encoder bit-packed
                                // an out-of-range offset and corrupted the whole mover stream --
                                // desyncing slot 0 (the avatar): character vanished, camera moved.
                                mv_x[i] = mv_dispx;
                                mv_y[i] = mv_dispy;
                                mv_type[i] = myobj->type & 1023;
                                mv_dir[i] = objgetdir(myobj->type);
                                mv_frame[i] = OBJGETDIR_FRAME;
                                mv_flags[i] = 0;
                                mv_playerid[i] = 0;
                                mv_hpmp[i] = 0;
                                mv_ktar[i] = 0;
                                mv_more[i] = 0;
                                if (mv_type[i] == 366) mv_frame[i] = myobj->type >> 10; //EXCEPTION: tanglevine tendril
                                if (mv_type[i] == 413) mv_frame[i] = myobj->type >> 10; //EXCEPTION: silver serpent


                                if (myobj->info & 2) {
                                    //npc
                                    tnpc = (npc *) myobj->more;

                                    //horse with rider? set more
                                    if (mv_type[i] == 431) {
                                        mv_more[i] = tnpc->originalobjtype & 1023;
                                    }

                                    if (tnpc->protection) mv_flags[i] |= MV_PROTECT;
                                    if (tnpc->light) mv_flags[i] |= MV_LIGHTBRIGHT;
                                    if (myobj3 = tnpc->armour) {
                                        if (myobj3->type == 81) mv_flags[i] |= MV_STORMCLOAK; //storm cloak
                                    } //armour
                                    //NEWCODE
                                    if (tnpc->negatemagic) {
                                        mv_flags[i] |= MV_STORMCLOAK; //storm cloak
                                    } //negatemagic

                                    //NEWCODEEND
                                    if (tnpc->flags & 2) mv_flags[i] |= MV_INVISIBLE;
                                    if (tnpc->flags & 4) mv_flags[i] |= MV_SLEEP;
                                    if (tplayer2 = tnpc->player) {
                                        //has player pointer
                                        if (tnpc->light == 0) {
                                            if (tnpc->player == tplayer) mv_flags[i] |= MV_LIGHTGLOW;
                                        } //light
                                        if (tplayer2->party[0] == myobj) {
                                            //first player
                                            mv_playerid[i] = tplayer2->id;
                                            if (tplayer2->typing) {
                                                if (tplayer != tplayer2) mv_flags[i] |= MV_TYPING;
                                            } //typing
                                            if (tplayer2->speaking) {
                                                if (tplayer != tplayer2) mv_flags[i] |= MV_SPEAKING;
                                            } //speaking
                                            //hp/mp bars 0-15hp, 0-7mp, 1poisioned
                                            f = (float) tnpc->hp / (float) tnpc->hp_max;
                                            z = f * 16;
                                            if (z == 16) z = 15;
                                            f = (float) tnpc->mp / (float) tnpc->mp_max;
                                            z2 = f * 8;
                                            if (z2 == 8) z2 = 7;
                                            z = z + (z2 << 4);
                                            if (tnpc->flags & 1) z += 128;
                                            mv_hpmp[i] = z;
                                        } //tplayer2->party[0]==myobj
                                    } else {
                                        //no player pointer
                                        if (tnpc->schedule) {
                                            if (tnpc->schedule_i != -1) {
                                                if (tnpc->schedule[tnpc->schedule_i].type == 0x91)
                                                    mv_flags[i] |= MV_SLEEP; //0x91=sleeping!
                                            } //tnpc->schedule_i!=-1
                                        } //tnpc->schedule
                                    } //no player pointer
                                } //npc

                                if (myobj->info & 4) {
                                    //crt
                                    crt = (creature *) myobj->more;
                                    if (crt->flags & 8) mv_flags[i] |= MV_PARALYZE;
                                    if (crt->flags & 4) mv_flags[i] |= MV_SLEEP;
                                    if (crt->flags & 2) {
                                        if (crt->al == 1) {
                                            i--;
                                            goto mover_add_complete;
                                        } //creature is not friendly allegiance, don't display at all
                                        mv_flags[i] |= MV_INVISIBLE;
                                    }
                                    for (z = 0; z <= 9; z++) {
                                        if (tplayer->ktar_wait[z]) {
                                            if ((object *) tplayer->ktar[z]->more == myobj) {
                                                mv_ktar[i] = z + 1;
                                            }
                                        }
                                    } //z
                                } //crt

                            mover_add_complete:

                                myobj = (object *) myobj->next;
                                goto mvobjskip;
                            } //myobj

                        moverbuffer_outofrange:;
                        }
                    }
                    mv_i = i + 1;


                    //compare to current mover list
                    ZeroMemory(&mv_last, sizeof(mv_last));
                    ZeroMemory(&mv_new, sizeof(mv_new));


                    //******this code happens whether or not an update is sent******
                    // RW dynamic-objects fix follow-up (2026-05-28): this prune MUST
                    // force a scene-update send whenever it removes a mover. Otherwise
                    // the host shrinks tplayer->mv_i locally and never tells the
                    // client, so the client keeps the stale entry; every subsequent
                    // remove/move/add index is then off-by-N on the client and the
                    // wrong mover slots get overwritten -- producing duplicated NPCs,
                    // a ghost-avatar that lags the camera, and "walking just moves
                    // the camera" because the player's own mover slot (typically
                    // index 0) gets reassigned to a different mover on the client.
                    // Was latent at the legacy 32x24 prune because that bound
                    // almost always coincided with a player-MOVE message setting
                    // sceneupdaterequired anyway; the widened MV_TX bounds made the
                    // silent-prune case more likely and exposed the bug.
                    //remove all offscreen objects in client's array
                    i = 0;
                mover_removeoffscreen_next:
                    if (i < tplayer->mv_i) {
                        x = tplayer->mv_x[i] - tpx;
                        y = tplayer->mv_y[i] - tpy;
                        // RW dynamic-objects fix: bounds match the centered fill window so
                        // movers visible anywhere in the resized viewport stay alive instead
                        // of being pruned at the legacy 32x24 edge.
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
                            sceneupdaterequired = 1; //force send so client's mirror-prune runs
                            goto mover_removeoffscreen_next;
                        } //x,y boundary check
                        i++;
                        goto mover_removeoffscreen_next;
                    } //i check


                    //1. REMOVE
                    i = 0;
                mover_removecheck_next:
                    if (i < tplayer->mv_i) {
                        for (i2 = 0; i2 < mv_i; i2++) {
                            if (mv_new[i2] == 0) {
                                //is new x,y offset out of range? if so the object will be removed and recreated
                                x = mv_x[i2] - tplayer->mv_x[i];
                                if (abs(x) > 3) goto mover_remove_outofrange;
                                y = mv_y[i2] - tplayer->mv_y[i];
                                if (abs(y) > 3) goto mover_remove_outofrange;
                                if (mover_offseti[x + 3][y + 3] == 255) goto mover_remove_outofrange;
                                if (tplayer->mv_type[i] == mv_type[i2]) {
                                    if (tplayer->mv_type[i] == 413) {
                                        //silver serp
                                        if (x || y) goto mover_remove_outofrange;
                                        if (tplayer->mv_frame[i] != mv_frame[i2]) goto mover_remove_outofrange;
                                        goto mover_remove_link;
                                    }
                                    if (tplayer->mv_type[i] == 366) {
                                        //tanglevine tendril (not core)
                                        if (x || y) goto mover_remove_outofrange;
                                        if (tplayer->mv_frame[i] != mv_frame[i2]) goto mover_remove_outofrange;
                                        goto mover_remove_link;
                                    }

                                    if (tplayer->mv_object[i] == mv_object[i2]) {
                                        //obj check
                                    mover_remove_link:

                                        if (tplayer->mv_playerid[i] == mv_playerid[i2]) {
                                            mv_last[i] = i2;
                                            mv_new[i2] = MOVER_FOUND; //link
                                            i++;
                                            goto mover_removecheck_next;
                                        } //playerid
                                    } //object
                                } //type
                            } //mv_new[i2]==0
                        mover_remove_outofrange:;
                        } //i2

                        //remove mover i message
                        BITSadd(t, &bitsi, 1, 1); //remove a/another mover
                        BITSadd(t, &bitsi, i, getnbits(tplayer->mv_i));
                        //index of mover to remove based on current buffer
                        sceneupdaterequired = 1;

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
                        goto mover_removecheck_next;
                    } //i check
                    z = 0;
                    BITSadd(t, &bitsi, z, 1); //no more movers to remove

                    //2.1 MOVE MOVER
                    for (i = 0; i < tplayer->mv_i; i++) {
                        i2 = mv_last[i];
                        x = mv_x[i2] - tplayer->mv_x[i];
                        y = mv_y[i2] - tplayer->mv_y[i];
                        if (x || y) {
                            sceneupdaterequired = 1;
                            BITSadd(t, &bitsi, 1, 1); //move a/another mover
                            BITSadd(t, &bitsi, i, getnbits(tplayer->mv_i)); //index of mover to move in current buffer
                            BITSadd(t, &bitsi, mover_offseti[x + 3][y + 3], 5);

                            //update client data
                            //HORSE SWAP FIX (HORSE FIX)

                            if ((tplayer->mv_type[i] == 431) &&
                                //( ((mv_dir[i2]==0)&&(mv_x[i2]!=tplayer->mv_x[i])    ) || ((mv_dir[i2]==1)&&(mv_y[i2]!=tplayer->mv_y[i])) || ((mv_dir[i2]==2)&&(mv_x[i2]!=tplayer->mv_x[i])) || ((mv_dir[i2]==3)&&(mv_y[i2]!=tplayer->mv_y[i])) )
                                (((mv_dir[i2] == 0) && (
                                      (mv_x[i2] != tplayer->mv_x[i]) || (mv_y[i2] >= tplayer->mv_y[i]))) || (
                                     (mv_dir[i2] == 1) && (
                                         (mv_y[i2] != tplayer->mv_y[i]) || (mv_x[i2] <= tplayer->mv_x[i]))) || (
                                     (mv_dir[i2] == 2) && (
                                         (mv_x[i2] != tplayer->mv_x[i]) || (mv_y[i2] <= tplayer->mv_y[i]))) || (
                                     (mv_dir[i2] == 3) && (
                                         (mv_y[i2] != tplayer->mv_y[i]) || (mv_x[i2] >= tplayer->mv_x[i]))))
                            ) {
                                //NOHORSEFIXNECESSARY code negates the HORSE SWAP FIX patch under certain conditions to allow for diagonal movement with a frame change
                                if ((mv_dir[i2] == 0) && (mv_y[i2] < tplayer->mv_y[i])) goto NOHORSEFIXNECESSARY;
                                if ((mv_dir[i2] == 2) && (mv_y[i2] > tplayer->mv_y[i])) goto NOHORSEFIXNECESSARY;

                                tplayer->mv_x[i] = mv_x[i2];
                                tplayer->mv_y[i] = mv_y[i2];
                                //change client frame number to trick the server into sending a dir/frame update message too
                                if (tplayer->mv_frame[i] == mv_frame[i2]) {
                                    tplayer->mv_frame[i]++;
                                    if (tplayer->mv_frame[i] == 2) tplayer->mv_frame[i] = 0;
                                }
                            } else {
                            NOHORSEFIXNECESSARY:
                                tplayer->mv_x[i] = mv_x[i2];
                                tplayer->mv_y[i] = mv_y[i2];
                                tplayer->mv_dir[i] = mv_dir[i2];
                                tplayer->mv_frame[i] = mv_frame[i2];
                            } //HORSE SWAP FIX
                        } //x||y
                    } //i
                    BITSadd(t, &bitsi, 0, 1); //no more movers to move


                    //2.2 MOVER DIRECTION/FRAME CHANGE
                    /*
                        1 change mover frame/dir
                        ? index of mover
                        [2] if mover CAN have a direction it is set here, otherwise this can be omitted and the movers frame only will be incremented
                        */
                    for (i = 0; i < tplayer->mv_i; i++) {
                        i2 = mv_last[i];
                        if ((tplayer->mv_dir[i] != mv_dir[i2]) || (tplayer->mv_frame[i] != mv_frame[i2])) {
                            BITSadd(t, &bitsi, 1, 1);
                            BITSadd(t, &bitsi, i, getnbits(tplayer->mv_i)); //index of mover to move in current buffer
                            BITSadd(t, &bitsi, mv_dir[i2], 2);
                            tplayer->mv_dir[i] = mv_dir[i2];
                            tplayer->mv_frame[i] = mv_frame[i2];
                            sceneupdaterequired = 1;
                        }
                    } //i
                    BITSadd(t, &bitsi, 0, 1); //no more movers to direction/frame change


                    //2.3 MOVER STATE CHANGE
                    z3 = getnbits(tplayer->mv_i);
                    for (i = 0; i < tplayer->mv_i; i++) {
                        i2 = mv_last[i];
                        i3 = 0;


                        if ((tplayer->mv_flags[i] & MV_LIGHTBRIGHT) != (mv_flags[i2] & MV_LIGHTBRIGHT)) {
                            if (mv_flags[i2] & MV_LIGHTGLOW) {
                                if (tplayer->mv_flags[i] & MV_LIGHTBRIGHT) {
                                    goto MV_LIGHTBRIGHT_assumed;
                                }
                            }

                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 0, 4);
                        }
                    MV_LIGHTBRIGHT_assumed:
                        if ((tplayer->mv_flags[i] & MV_LIGHTGLOW) != (mv_flags[i2] & MV_LIGHTGLOW)) {
                            if (mv_flags[i2] & MV_LIGHTBRIGHT) {
                                if (tplayer->mv_flags[i] & MV_LIGHTGLOW) {
                                    goto MV_LIGHTGLOW_assumed;
                                }
                            }
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 1, 4);
                        }
                    MV_LIGHTGLOW_assumed:
                        if ((tplayer->mv_flags[i] & MV_INVISIBLE) != (mv_flags[i2] & MV_INVISIBLE)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 2, 4);
                        }
                        if ((tplayer->mv_flags[i] & MV_PARALYZE) != (mv_flags[i2] & MV_PARALYZE)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 3, 4);
                        }
                        if ((tplayer->mv_flags[i] & MV_SLEEP) != (mv_flags[i2] & MV_SLEEP)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 4, 4);
                        }
                        if ((tplayer->mv_flags[i] & MV_PROTECT) != (mv_flags[i2] & MV_PROTECT)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 5, 4);
                        }
                        if ((tplayer->mv_flags[i] & MV_STORMCLOAK) != (mv_flags[i2] & MV_STORMCLOAK)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 6, 4);
                        }
                        if ((tplayer->mv_flags[i] & MV_TYPING) != (mv_flags[i2] & MV_TYPING)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 7, 4);
                        }
                        if (tplayer->mv_hpmp[i] != mv_hpmp[i2]) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 8, 4);
                            BITSadd(t, &bitsi, mv_hpmp[i2], 8);
                        }
                        if (tplayer->mv_ktar[i] != mv_ktar[i2]) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 9, 4);
                            BITSadd(t, &bitsi, mv_ktar[i2], 4);
                        }
                        if ((tplayer->mv_flags[i] & MV_SPEAKING) != (mv_flags[i2] & MV_SPEAKING)) {
                            if (i3 == 0) {
                                i3 = 1;
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, i, z3);
                            } else { BITSadd(t, &bitsi, 1, 1); }
                            BITSadd(t, &bitsi, 10, 4);
                        }
                        if (i3) {
                            BITSadd(t, &bitsi, 0, 1);
                            sceneupdaterequired = 1;
                            tplayer->mv_flags[i] = mv_flags[i2];
                            tplayer->mv_hpmp[i] = mv_hpmp[i2];
                            tplayer->mv_ktar[i] = mv_ktar[i2];
                            tplayer->mv_more[i] = mv_more[i2];
                        }
                    } //i
                    BITSadd(t, &bitsi, 0, 1); //no more movers' states to change/toggle

                    //3. ADD NEW MOVER
                    for (i2 = 0; i2 < mv_i; i2++) {
                        if (mv_new[i2] == 0) {
                            sceneupdaterequired = 1;
                            z = 1;
                            BITSadd(t, &bitsi, z, 1); //add a/another mover
                            // RW dynamic-objects fix: encode x,y across the full
                            // MV_TX_W x MV_TX_H window with MV_TX_BITS bits (was
                            // y*34+x in 10 bits, tied to the legacy 32x24 view).
                            // Client decoder must use the same MV_TX_W,
                            // MV_TX_BITS, MV_TX_OFFX/Y.
                            x = mv_x[i2] - tpx + MV_TX_OFFX; //0..MV_TX_W-1
                            y = mv_y[i2] - tpy + MV_TX_OFFY; //0..MV_TX_H-1
                            z = y * MV_TX_W + x;
                            BITSadd(t, &bitsi, z, MV_TX_BITS); //x,y offset
                            z = mv_type[i2];
                            BITSadd(t, &bitsi, z, 10); //type YET TO BE OPTOMIZED

                            if (z == 413) {
                                //silver serp
                                BITSadd(t, &bitsi, mv_frame[i2], 4);
                                goto mover_add_special;
                            }
                            if (z == 366) {
                                //tanglevine tendril (not core)
                                BITSadd(t, &bitsi, mv_frame[i2], 3);
                                goto mover_add_special;
                            }
                            //check if direction is required (z3 is number of bits to use to send direction)
                            z3 = 2;
                            if (obji[sprlnk[z]].v4 == 3) z3 = 0; //"bird"
                            if (z == 365) z3 = 0; //tanglevine core
                            if (z == 374) z3 = 0; //hydra core
                            if (z == 375) z3 = 0; //slime
                            if (z == 391) z3 = 0; //farmer hoeing
                            if (z == 415) z3 = 0; //raft
                            BITSadd(t, &bitsi, mv_dir[i2], z3);
                        mover_add_special:

                            //***STATE CHANGES***
                            if (mv_ktar[i2]) {
                                BITSadd(t, &bitsi, 1, 1); //a/another state change/toggle is required
                                BITSadd(t, &bitsi, 9, 4); //ktar update
                                BITSadd(t, &bitsi, mv_ktar[i2], 4);
                            }
                            if (mv_playerid[i2]) {
                                BITSadd(t, &bitsi, 1, 1); //a/another state change/toggle is required
                                BITSadd(t, &bitsi, 8, 4);
                                BITSadd(t, &bitsi, mv_hpmp[i2], 8);
                                BITSadd(t, &bitsi, mv_playerid[i2], 32);
                            }
                            if (mv_flags[i2] & MV_LIGHTBRIGHT) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 0, 4);
                            }
                            if (mv_flags[i2] & MV_LIGHTGLOW) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 1, 4);
                            }
                            if (mv_flags[i2] & MV_INVISIBLE) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 2, 4);
                            }
                            if (mv_flags[i2] & MV_PARALYZE) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 3, 4);
                            }
                            if (mv_flags[i2] & MV_SLEEP) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 4, 4);
                            }
                            if (mv_flags[i2] & MV_PROTECT) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 5, 4);
                            }
                            if (mv_flags[i2] & MV_STORMCLOAK) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 6, 4);
                            }
                            if (mv_flags[i2] & MV_TYPING) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 7, 4);
                            }
                            if (mv_flags[i2] & MV_SPEAKING) {
                                BITSadd(t, &bitsi, 1, 1);
                                BITSadd(t, &bitsi, 10, 4);
                            }

                            //mv_more(rider type,...)
                            if (mv_more[i2]) {
                                BITSadd(t, &bitsi, 1, 1); //a/another state change/toggle is required
                                BITSadd(t, &bitsi, 11, 4);
                                BITSadd(t, &bitsi, mv_more[i2], 16);
                            }


                            z = 0;
                            BITSadd(t, &bitsi, z, 1); //no more state changes required

                            //add temp mover data to client list
                            i = tplayer->mv_i;
                            tplayer->mv_x[i] = mv_x[i2];
                            tplayer->mv_y[i] = mv_y[i2];
                            tplayer->mv_type[i] = mv_type[i2];
                            tplayer->mv_dir[i] = mv_dir[i2];
                            tplayer->mv_frame[i] = mv_frame[i2];
                            tplayer->mv_object[i] = mv_object[i2];
                            tplayer->mv_flags[i] = mv_flags[i2];
                            tplayer->mv_hpmp[i] = mv_hpmp[i2];
                            tplayer->mv_playerid[i] = mv_playerid[i2];
                            tplayer->mv_ktar[i] = mv_ktar[i2];
                            tplayer->mv_more[i] = mv_more[i2];
                            tplayer->mv_i++;
                        } //mv_new[i2]==0
                    } //i2
                    z = 0;
                    BITSadd(t, &bitsi, z, 1);

                    if (sceneupdaterequired) {
                        t->l = bitsi >> 3;
                        if (bitsi & 7) t->l++;
                        NET_send(NETplayer, tplayer->net, t);
                        tplayer->updatemessage = 1;
                    } else {
                        tplayer->updatemessage = 0;
                    }
                } else {
                    //tplayer->updatemessage
                    tplayer->updatemessage = 0;
                }

                /*
        1. remove
        2.1 change
        2.2 move
        3. add


        ***1. REMOVE MOVER***
        1 if =1 remove a/another mover
        {
        [?] index of mover to remove based on current buffer
        }
        ***2.1 CHANGE/TOGGLE STATE***
        1 if =1 change/toggle a/another mover's state
        {
        [?] index of mover in current buffer
        a:
        4 state change id
        0 LIGHTBRIGHT (LIGHTGLOW assumed off)
        1 LIGHTGLOW (LIGHTBRIGHT assumed off)
        2 INVISIBLE
        3 PARALYZE
        4 SLEEP
        5 PROTECT
        6 STORMCLOAK
        7 TYPING
        8 hpmp{		8 new hpmp byte		}
        9 ktar{		4 new ktar number	}
        1 if =1 another state needs to be changed, goto a
        }
        ***2.2 CHANGE FRAME (THIS IS ONLY SENT IF MOVER HAS NOT MOVED TO ANOTHER SQUARE, IN THAT CASE IT CAN BE ASSUMED)***
        1 if =1 change animation frame of a/another mover
        {
        [?] index of mover to change the animation frame of
        }
        ***2.3 MOVE***
        1 if =1 move a/another mover
        {
        [?] index of mover to move in current buffer
        5 number representing a special x,y offset from current location to move to
        }
        ***3. ADD NEW MOVER***
        1 if =1 add a/another mover
        {
        10 x,y offset to add mover
        8-10 (number of bits will depend on yet to be implemented optomization) mover's object type
        2 direction mover is facing
        a:
        1 if =1 a/another state change/toggle is required
        {
        4 state change id
        0 LIGHTBRIGHT (LIGHTGLOW assumed off)
        1 LIGHTGLOW (LIGHTBRIGHT assumed off)
        2 INVISIBLE
        3 PARALYZE
        4 SLEEP
        5 PROTECT
        6 STORMCLOAK
        7 TYPING
        8 hpmp+playerid{	8  new hpmp byte
        32 playerid			}
        9 ktar{				4 new ktar number	}
        goto a
        }
        }
        */


                //flags/pointers used while comparing buffers
                //unsigned long mv_last[MVLISTLAST+1];
                //unsigned long mv_new[MVLISTLAST+1];


