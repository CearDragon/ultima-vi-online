// LHS-P4.5: part_b_admin_exec -- CONTINUES MEGA B. WIRE + admin.
// LHS brace-seam: CONTINUES the if(x3){ dispatch opened in part_b_dispatch.
// Contents: privilege-change + slash-command execution/gating (deeply nested;
// no interior depth-1 seams). Cross-ref docs/admin/README.md.

        /*
    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==5)){
    userkey=0; CLIENTplayer->key-=KEYmbclick;
    //send message to host
    if (CLIENTplayer->mf<=7){
    txtsetchar(t2,45); txtaddchar(t2,CLIENTplayer->mf);
    txtaddshort(t2,CLIENTplayer->mx); txtaddshort(t2,CLIENTplayer->my);
    NET_send(NETplayer,NULL,t2);
    }//CLIENTplayer->mf<=7
    }//not4sale
    */


        if ((t->d2[0] == 6) || (t->d2[0] == 12)) {
            playerlist[tpl]->typing = 0;
            playerlist[tpl]->idle = 0;

            x5 = 0;
            if (t->d2[0] == 12) {
                x5 = 1; //global message
                if (playerlist[tpl]->globalmessage_wait != 0) {
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtaddchar(t, 255);
                    txtadd(t, "Wait 10 seconds between sending global messages!");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }
                playerlist[tpl]->globalmessage_wait = 11 - 1;
            }

            txtright(t, t->l - 1);


            if (t->d2[0] == 47) {
                //"/"
                //PRIVILEGES: change
                /*
        PRIVILEGES: notes
        40-47		16x2-bit privilege settings representing privilege level from 0-3
        PRIVILEGE LEVEL 1	You can use the power on others eg. /BAN John
        You can revoke this LEVEL 1 PRIVILEGE from others eg. /UNGRANT BAN John (with the same privilege level or less)
        PRIVILEGE LEVEL 2	You can use the power on others eg. /BAN John
        You can give another the ability to use that power eg. /GRANT BAN John
        You can revoke this LEVEL 2 PRIVILEGE from others eg. /UNGRANT2 BAN John (with the same privilege level or less)
        PRIVILEGE LEVEL 3	You can use the power on others eg. /BAN John
        You can give another the ability to use that power eg. /GRANT BAN John
        You can give another the ability to give that power to others eg. /GRANT2 BAN John
        You can give another the ability to "give another the ability to give that power to others" eg. /GRANT3 BAN John
        You can revoke this LEVEL 3 PRIVILEGE from others eg. /UNGRANT3 BAN John (with the same privilege level or less)
        */
                // /WHERE -- report the player's own current tile coordinates back
                // to themselves as a system message. Handy for locating map tiles
                // (e.g. which storage shelf you are standing on). Not gated on any
                // privilege; reading your own position is harmless.
                {
                    static txt *wheretxt = txtnew();
                    static object *whereobj;
                    txtset(wheretxt, t);
                    txtright(wheretxt, wheretxt->l - 1); // strip the leading '/'
                    txtucase(wheretxt);
                    txtset(t3, "WHERE");
                    if (txtsearch(wheretxt, t3) == 1) {
                        whereobj = playerlist[tpl]->party[0];
                        txtset(t, "?");
                        t->d2[0] = 8; // system message colour/type
                        txtadd(t, "Location: x=");
                        txtnumint(t3, whereobj ? whereobj->x : playerlist[tpl]->px);
                        txtadd(t, t3);
                        txtadd(t, " y=");
                        txtnumint(t3, whereobj ? whereobj->y : playerlist[tpl]->py);
                        txtadd(t, t3);
                        NET_send(NETplayer, playerlist[tpl]->net, t);
                        goto doneclmess;
                    }
                }
                static unsigned char privileges_index;
                txtset(t2, t);
                txtright(t2, t2->l - 1);
                static unsigned char privileges_levelrequiredtochange;
                static unsigned char privileges_set;
                static unsigned char privileges_leveltochange;
                txtset(t3, "GRANT ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_leveltochange = 1;
                    privileges_set = TRUE;
                    privileges_levelrequiredtochange = 2;
                    goto privelegechange_requested;
                }
                txtset(t3, "UNGRANT ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_leveltochange = 1;
                    privileges_set = FALSE;
                    privileges_levelrequiredtochange = 1;
                    goto privelegechange_requested;
                }
                txtset(t3, "GRANT2 ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_leveltochange = 2;
                    privileges_set = TRUE;
                    privileges_levelrequiredtochange = 3;
                    goto privelegechange_requested;
                }
                txtset(t3, "UNGRANT2 ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_leveltochange = 2;
                    privileges_set = FALSE;
                    privileges_levelrequiredtochange = 2;
                    goto privelegechange_requested;
                }
                txtset(t3, "GRANT3 ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_leveltochange = 3;
                    privileges_set = TRUE;
                    privileges_levelrequiredtochange = 3;
                    goto privelegechange_requested;
                }
                txtset(t3, "UNGRANT3 ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_leveltochange = 3;
                    privileges_set = FALSE;
                    privileges_levelrequiredtochange = 3;
                    goto privelegechange_requested;
                }
                goto privelegechange_skip;
            privelegechange_requested:
                txtright(t2, t2->l - t3->l);
                txtset(t3, "BAN ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 0;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "UNBAN ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 1;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "KICK ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 2;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "DELETEPORTRAIT ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 3;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "BLOCKPORTRAITUPLOAD ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 4;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "UNBLOCKPORTRAITUPLOAD ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 5;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "BANVOICECHAT ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 6;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "UNBANVOICECHAT ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 7;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "SETMOTD ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 8;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "SYSBAN ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 9;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "UNSYSBAN ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 9;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "BANIP ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 10;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "UNBANIP ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 11;
                    goto privelegechange_gotcommand;
                }
                txtset(t3, "GETIP ");
                if (txtsearch(t2, t3) == 1) {
                    privileges_index = 12;
                    goto privelegechange_gotcommand;
                }

                //...
                txtset(t, "?");
                t->d2[0] = 8;
                txtadd(t, "Privileges change failed! (name of privilege to change is invalid)");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;
            privelegechange_gotcommand:
                //PRIVILEGES: check requesting player has sufficient privileges
                static unsigned char privileges_requestingplayerlevel;
                x3 = privileges_index / 16; //globalflags[x3]
                x4 = privileges_index - x3 * 16; //sub-index
                x4 *= 2; //multiply sub-index to get shifting factor
                privileges_requestingplayerlevel = (playerlist[tpl]->GNPCflags[x3 + 40] >> x4) & 3;
                if (privileges_requestingplayerlevel < privileges_levelrequiredtochange) goto privilege_denied;


                /*
        //set x6 to (0-2)privilege level required
        if (x2&1){
        x6=x2/2;
        }else{
        if (x2<=1) x6=1; else x6=2;
        }
        x4=x3/32;//DWORD privilege is stored at (0-3)
        x5=x3&31;//bit privilege is stored at
        x6=x4+x6*4+40;
        if ((playerlist[tpl]->GNPCflags[x6]&(1<<x5))==0) goto privilege_denied;
        x6=x4+(x2/2)*4+40;
        */

                txtright(t2, t2->l - t3->l);
                //check name against characters currently in britannia
                for (y2 = 0; y2 <= playerlist_last; y2++) {
                    if (playerlist[y2]) {
                        if (myobj2 = getprimarypartymemberobj(playerlist[y2])) {
                            tnpc2 = (npc *) myobj2->more;
                            if (txtsamewithoutcase(tnpc2->name, t2)) {
                                //PRIVILEGES: check requesting player's privilege level is equal or higher than target player's privilege level
                                static unsigned char privileges_targetplayerlevel;
                                privileges_targetplayerlevel = (playerlist[y2]->GNPCflags[x3 + 40] >> x4) & 3;
                                if (privileges_requestingplayerlevel < privileges_targetplayerlevel)
                                    goto
                                            privilege_denied;
                                x5 = privileges_targetplayerlevel;
                                //PRIVILEGES: edit privileges
                                if (privileges_set) {
                                    if (privileges_targetplayerlevel < privileges_leveltochange) {
                                        privileges_targetplayerlevel = privileges_leveltochange;
                                    } else {
                                        txtsetchar(t2, 8);
                                        txtadd(t2, "Privileges change failed! (player already has these privileges)");
                                        NET_send(NETplayer, playerlist[tpl]->net, t2);
                                        goto doneclmess;
                                    }
                                } else {
                                    //privileges_set: remove privilege
                                    if (privileges_targetplayerlevel > (privileges_leveltochange - 1)) {
                                        privileges_targetplayerlevel = privileges_leveltochange - 1;
                                    } else {
                                        txtsetchar(t2, 8);
                                        txtadd(
                                            t2,
                                            "Privileges change failed! (player does not currently have this privilege so privileges have not been changed)");
                                        NET_send(NETplayer, playerlist[tpl]->net, t2);
                                        goto doneclmess;
                                    }
                                } //privileges_set
                                playerlist[y2]->GNPCflags[x3 + 40] -= (x5 << x4); //remove old privileges
                                playerlist[y2]->GNPCflags[x3 + 40] += (privileges_targetplayerlevel << x4);
                                //add new privileges
                                txtset(t2, "?");
                                t2->d2[0] = 8;
                                txtadd(t2, "Privileges changed successfully! (");
                                txtadd(t2, t);
                                txtadd(t2, ")");
                                NET_send(NETplayer, playerlist[tpl]->net, t2);
                                //record change on file record in save folder
                                txtset(t2, "UNKNOWN");
                                if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                    tnpc2 = (npc *) myobj2->more;
                                    txtset(t2, tnpc2->name);
                                }
                                txtadd(t2, ": ");
                                txtadd(t2, t);
                                tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                if (tfh->h == HFILE_ERROR)
                                    tfh = open2(".\\save\\adminlog.txt",
                                                OF_READWRITE | OF_CREATE);
                                seek(tfh, lof(tfh));
                                txtfileout(t2, tfh);
                                close(tfh);
                                goto doneclmess;
                            } //name==t2
                        } //party[0]
                    }
                } //y2 loop
                txtset(t, "?");
                t->d2[0] = 8;
                txtadd(t, "Privileges change failed! (no player named ");
                txtadd(t, t2);
                txtadd(t, " is currently online, check uppercase/lowercase)");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;
            privelegechange_skip:

                //slash commands
                txtset(t3, "KICK ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 2;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = FIRST_CLIENT; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (((playerlist[x4]->GNPCflags[privileges_index / 16 + 40] >> (
                                              (privileges_index % 16) * 2)) & 3) > (
                                            (playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> (
                                                 (privileges_index % 16) * 2)) & 3))
                                        goto privilege_denied2;
                                    playerlist[x4]->GNPCflags[253] |= 1;
                                    txtset(t, "??");
                                    t->d2[0] = 250;
                                    t->d2[1] = 7;
                                    NET_send(NETplayer, playerlist[x4]->net, t);
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t3, "BAN ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 0;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    txtset(t9, "Kalour");
                    if (txtsamewithoutcase(t9, t2)) goto privilege_denied2;
                    x5 = 0;
                    for (x4 = 0; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (x4 == 0) {
                                        txtset(t, "?");
                                        t->d2[0] = 8;
                                        txtadd(t, "No player named ");
                                        txtadd(t, t2);
                                        txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                                        NET_send(NETplayer, playerlist[tpl]->net, t);
                                        goto doneclmess;
                                    }
                                    x5 = 1;
                                }
                            }
                        }
                    }
                    tfh = open2("ban.txt", OF_READWRITE | OF_SHARE_COMPAT);
                    seek(tfh, lof(tfh));
                    txtfileout(t2, tfh);
                    close(tfh);
                    if (x5) {
                        txtset(t3, "?");
                        t3->d2[0] = 8;
                        txtadd(t3, t2);
                        txtadd(t3, " has been banned!");
                        NET_send(NETplayer, playerlist[tpl]->net, t3);
                    } else {
                        txtset(t3, "?");
                        t3->d2[0] = 8;
                        txtadd(t3, t2);
                        txtadd(t3, " is not currently in Britannia but has been banned!");
                        NET_send(NETplayer, playerlist[tpl]->net, t3);
                    }
                    txtset(t2, "UNKNOWN");
                    if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                        tnpc2 = (npc *) myobj2->more;
                        txtset(t2, tnpc2->name);
                    }
                    txtadd(t2, ": ");
                    txtadd(t2, t);
                    tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                    if (tfh->h == HFILE_ERROR) tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_CREATE);
                    seek(tfh, lof(tfh));
                    txtfileout(t2, tfh);
                    close(tfh);
                    goto doneclmess;
                }

                txtset(t3, "UNBAN ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 1;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    txtucase(t2);
                    tfh = open2("ban.txt", OF_READWRITE | OF_SHARE_COMPAT);
                    y = 0;
                unban_loop:
                    x4 = seek(tfh);
                    txtfilein(t3, tfh);
                    if (t3->l) {
                        txtucase(t3);
                        if (txtsame(t2, t3)) {
                            y = 1;
                            seek(tfh, x4);
                            for (y2 = 0; y2 < t3->l; y2++) t3->d2[y2] = 42;
                            txtfileout(t3, tfh);
                        } //txtsame(t2,t3)
                    } //t3->l
                    if (x4 < lof(tfh)) goto unban_loop;
                    close(tfh);
                    if (y) {
                        txtset(t4, "?");
                        t4->d2[0] = 8;
                        txtadd(t4, t2);
                        txtadd(t4, " was unbanned successfully!");
                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                    } else {
                        txtset(t4, "?");
                        t4->d2[0] = 8;
                        txtadd(t4, "No player named ");
                        txtadd(t4, t2);
                        txtadd(t4, " is currently banned!");
                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                    }
                    txtset(t2, "UNKNOWN");
                    if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                        tnpc2 = (npc *) myobj2->more;
                        txtset(t2, tnpc2->name);
                    }
                    txtadd(t2, ": ");
                    txtadd(t2, t);
                    tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                    if (tfh->h == HFILE_ERROR) tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_CREATE);
                    seek(tfh, lof(tfh));
                    txtfileout(t2, tfh);
                    close(tfh);
                    goto doneclmess;
                }

                txtset(t3, "DELETEPORTRAIT ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 3;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = 0; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (((playerlist[x4]->GNPCflags[privileges_index / 16 + 40] >> (
                                              (privileges_index % 16) * 2)) & 3) > (
                                            (playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> (
                                                 (privileges_index % 16) * 2)) & 3))
                                        goto privilege_denied2;
                                    if (playerlist[x4]->GNPCflags[33] & 1) {
                                        playerlist[x4]->GNPCflags[33]--;
                                        playerlist[x4]->GNPCflags[253] |= 1;
                                        //kick to stop player using existing portrait
                                        tnpc2->exp += 1000;
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, "'s custom portrait deleted! (1000xp returned)");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                        txtset(t2, "UNKNOWN");
                                        if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                            tnpc2 = (npc *) myobj2->more;
                                            txtset(t2, tnpc2->name);
                                        }
                                        txtadd(t2, ": ");
                                        txtadd(t2, t);
                                        tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                        if (tfh->h == HFILE_ERROR)
                                            tfh = open2(".\\save\\adminlog.txt",
                                                        OF_READWRITE | OF_CREATE);
                                        seek(tfh, lof(tfh));
                                        txtfileout(t2, tfh);
                                        close(tfh);
                                        txtset(t, "??");
                                        t->d2[0] = 250;
                                        t->d2[1] = 7;
                                        NET_send(NETplayer, playerlist[x4]->net, t);
                                    } else {
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " is not using a custom portrait!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    }
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t3, "BLOCKPORTRAITUPLOAD ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 4;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = FIRST_CLIENT; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (((playerlist[x4]->GNPCflags[privileges_index / 16 + 40] >> (
                                              (privileges_index % 16) * 2)) & 3) > (
                                            (playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> (
                                                 (privileges_index % 16) * 2)) & 3))
                                        goto privilege_denied2;
                                    if ((playerlist[x4]->GNPCflags[33] & 2) == 0) {
                                        playerlist[x4]->GNPCflags[33] |= 2;
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " can no longer upload custom portraits!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                        txtset(t2, "UNKNOWN");
                                        if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                            tnpc2 = (npc *) myobj2->more;
                                            txtset(t2, tnpc2->name);
                                        }
                                        txtadd(t2, ": ");
                                        txtadd(t2, t);
                                        tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                        if (tfh->h == HFILE_ERROR)
                                            tfh = open2(".\\save\\adminlog.txt",
                                                        OF_READWRITE | OF_CREATE);
                                        seek(tfh, lof(tfh));
                                        txtfileout(t2, tfh);
                                        close(tfh);
                                    } else {
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " already cannot upload custom portraits!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    }
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t3, "UNBLOCKPORTRAITUPLOAD ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 5;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = 0; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (playerlist[x4]->GNPCflags[33] & 2) {
                                        playerlist[x4]->GNPCflags[33] -= 2;
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " can now upload custom portraits again!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                        txtset(t2, "UNKNOWN");
                                        if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                            tnpc2 = (npc *) myobj2->more;
                                            txtset(t2, tnpc2->name);
                                        }
                                        txtadd(t2, ": ");
                                        txtadd(t2, t);
                                        tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                        if (tfh->h == HFILE_ERROR)
                                            tfh = open2(".\\save\\adminlog.txt",
                                                        OF_READWRITE | OF_CREATE);
                                        seek(tfh, lof(tfh));
                                        txtfileout(t2, tfh);
                                        close(tfh);
                                    } else {
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " can already upload custom portraits!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    }
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }


                txtset(t3, "BANVOICECHAT ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 6;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = FIRST_CLIENT; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (((playerlist[x4]->GNPCflags[privileges_index / 16 + 40] >> (
                                              (privileges_index % 16) * 2)) & 3) > (
                                            (playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> (
                                                 (privileges_index % 16) * 2)) & 3))
                                        goto privilege_denied2;
                                    if ((playerlist[x4]->GNPCflags[33] & 4) == 0) {
                                        playerlist[x4]->GNPCflags[33] |= 4;
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " is now banned from using voice chat!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                        txtset(t2, "UNKNOWN");
                                        if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                            tnpc2 = (npc *) myobj2->more;
                                            txtset(t2, tnpc2->name);
                                        }
                                        txtadd(t2, ": ");
                                        txtadd(t2, t);
                                        tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                        if (tfh->h == HFILE_ERROR)
                                            tfh = open2(".\\save\\adminlog.txt",
                                                        OF_READWRITE | OF_CREATE);
                                        seek(tfh, lof(tfh));
                                        txtfileout(t2, tfh);
                                        close(tfh);
                                    } else {
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " is already banned from using voice chat!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    }
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t3, "UNBANVOICECHAT ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 7;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = 0; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (playerlist[x4]->GNPCflags[33] & 4) {
                                        playerlist[x4]->GNPCflags[33] -= 4;
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " can now use voice chat again!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                        txtset(t2, "UNKNOWN");
                                        if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                            tnpc2 = (npc *) myobj2->more;
                                            txtset(t2, tnpc2->name);
                                        }
                                        txtadd(t2, ": ");
                                        txtadd(t2, t);
                                        tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                        if (tfh->h == HFILE_ERROR)
                                            tfh = open2(".\\save\\adminlog.txt",
                                                        OF_READWRITE | OF_CREATE);
                                        seek(tfh, lof(tfh));
                                        txtfileout(t2, tfh);
                                        close(tfh);
                                    } else {
                                        txtset(t4, "?");
                                        t4->d2[0] = 8;
                                        txtadd(t4, t2);
                                        txtadd(t4, " can already use voice chat!");
                                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    }
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                }

                txtset(t3, "SETMOTD ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 8;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    txtset(motd, t2);
                    tfh = open2("motd.txt", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                    txtfileout(motd, tfh);
                    close(tfh);
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "MOTD set to \"");
                    txtadd(t, t2);
                    txtadd(t, "\"");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                } //SETMOTD


                txtset(t3, "SYSBAN ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 9;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = FIRST_CLIENT; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (((playerlist[x4]->GNPCflags[privileges_index / 16 + 40] >> (
                                              (privileges_index % 16) * 2)) & 3) > (
                                            (playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> (
                                                 (privileges_index % 16) * 2)) & 3))
                                        goto privilege_denied2;
                                    txtsetchar(t, 247);
                                    txtaddchar(t, 1);
                                    NET_send(NETplayer, playerlist[x4]->net, t); //SYSBAN ON
                                    txtset(t4, "?");
                                    t4->d2[0] = 8;
                                    txtadd(t4, t2);
                                    txtadd(t4, " is now system banned! (In Britannia time limited to 8 seconds)");
                                    NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                } //SYSBAN

                txtset(t3, "UNSYSBAN ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 9;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = 0; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    if (((playerlist[x4]->GNPCflags[privileges_index / 16 + 40] >> (
                                              (privileges_index % 16) * 2)) & 3) > (
                                            (playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> (
                                                 (privileges_index % 16) * 2)) & 3))
                                        goto privilege_denied2;
                                    txtsetchar(t, 247);
                                    txtaddchar(t, 0);
                                    NET_send(NETplayer, playerlist[x4]->net, t); //SYSBAN OFF
                                    txtset(t4, "?");
                                    t4->d2[0] = 8;
                                    txtadd(t4, t2);
                                    txtadd(
                                        t4,
                                        " has been sent a release system ban message! (If the client has already quit this message may not have been received)");
                                    NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                } //UNSYSBAN

                txtset(t3, "BANIP ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 10;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    txtset(t9, "Kalour");
                    if (txtsamewithoutcase(t9, t2)) goto privilege_denied2;
                    for (x4 = FIRST_CLIENT; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    tfh = open2("banip.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                    if (tfh->h == HFILE_ERROR)
                                        tfh = open2("banip.txt",
                                                    OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
                                    seek(tfh, lof(tfh));
                                    txtset(t4, "");
                                    txtnumint(t3, playerlist[x4]->GNPCflags[242] & 255);
                                    txtadd(t4, t3);
                                    txtadd(t4, ".");
                                    txtnumint(t3, (playerlist[x4]->GNPCflags[242] >> 8) & 255);
                                    txtadd(t4, t3);
                                    txtadd(t4, ".");
                                    txtnumint(t3, (playerlist[x4]->GNPCflags[242] >> 16) & 255);
                                    txtadd(t4, t3);
                                    txtadd(t4, ".");
                                    txtnumint(t3, (playerlist[x4]->GNPCflags[242] >> 24) & 255);
                                    txtadd(t4, t3);
                                    txtfileout(t4, tfh);
                                    close(tfh);
                                    txtset(t3, "?");
                                    t3->d2[0] = 8;
                                    txtadd(t3, t2);
                                    txtadd(t3, "'s IP has been banned!");
                                    NET_send(NETplayer, playerlist[tpl]->net, t3);
                                    txtset(t2, "UNKNOWN");
                                    if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                                        tnpc2 = (npc *) myobj2->more;
                                        txtset(t2, tnpc2->name);
                                    }
                                    txtadd(t2, ": ");
                                    txtadd(t2, t);
                                    tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                                    if (tfh->h == HFILE_ERROR)
                                        tfh = open2(".\\save\\adminlog.txt",
                                                    OF_READWRITE | OF_CREATE);
                                    seek(tfh, lof(tfh));
                                    txtfileout(t2, tfh);
                                    close(tfh);
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                } //BANIP

                txtset(t3, "UNBANIP ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 11;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    txtucase(t2);
                    tfh = open2("banip.txt", OF_READWRITE | OF_SHARE_COMPAT);
                    if (tfh->h == HFILE_ERROR) {
                        txtset(t4, "?");
                        t4->d2[0] = 8;
                        txtadd(t4, "The IP ");
                        txtadd(t4, t2);
                        txtadd(t4, " is not currently banned!");
                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                        goto doneclmess;
                    }
                    y = 0;
                unbanip_loop:
                    x4 = seek(tfh);
                    txtfilein(t3, tfh);
                    if (t3->l) {
                        txtucase(t3);
                        if (txtsame(t2, t3)) {
                            y = 1;
                            seek(tfh, x4);
                            for (y2 = 0; y2 < t3->l; y2++) t3->d2[y2] = 46; //"."
                            txtfileout(t3, tfh);
                        } //txtsame(t2,t3)
                    } //t3->l
                    if (x4 < lof(tfh)) goto unbanip_loop;
                    close(tfh);
                    if (y) {
                        txtset(t4, "?");
                        t4->d2[0] = 8;
                        txtadd(t4, "The IP ");
                        txtadd(t4, t2);
                        txtadd(t4, " was unbanned successfully!");
                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                        txtset(t2, "UNKNOWN");
                        if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) {
                            tnpc2 = (npc *) myobj2->more;
                            txtset(t2, tnpc2->name);
                        }
                        txtadd(t2, ": ");
                        txtadd(t2, t);
                        tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_SHARE_COMPAT);
                        if (tfh->h == HFILE_ERROR) tfh = open2(".\\save\\adminlog.txt", OF_READWRITE | OF_CREATE);
                        seek(tfh, lof(tfh));
                        txtfileout(t2, tfh);
                        close(tfh);
                    } else {
                        txtset(t4, "?");
                        t4->d2[0] = 8;
                        txtadd(t4, "The IP ");
                        txtadd(t4, t2);
                        txtadd(t4, " is not currently banned!");
                        NET_send(NETplayer, playerlist[tpl]->net, t4);
                    }
                    goto doneclmess;
                } //UNBANIP

                txtset(t3, "GETIP ");
                if (txtsearch(t2, t3) == 1) {
                    txtright(t2, t2->l - t3->l);
                    privileges_index = 12;
                    if (((playerlist[tpl]->GNPCflags[privileges_index / 16 + 40] >> ((privileges_index % 16) * 2)) & 3)
                        == 0)
                        goto privilege_denied;
                    for (x4 = 0; x4 <= playerlist_last; x4++) {
                        if (playerlist[x4]) {
                            if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                tnpc2 = (npc *) myobj2->more;
                                if (txtsamewithoutcase(tnpc2->name, t2)) {
                                    //FOLLOWING LINE IS UNREQUIRED FOR GETIP
                                    //if (((playerlist[x4]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)>((playerlist[tpl]->GNPCflags[privileges_index/16+40]>>((privileges_index%16)*2))&3)) goto privilege_denied2;
                                    txtset(t4, "?");
                                    t4->d2[0] = 8;
                                    txtadd(t4, t2);
                                    txtadd(t4, "'s IP is ");
                                    txtnumint(t, playerlist[x4]->GNPCflags[242] & 255);
                                    txtadd(t4, t);
                                    txtadd(t4, ".");
                                    txtnumint(t, (playerlist[x4]->GNPCflags[242] >> 8) & 255);
                                    txtadd(t4, t);
                                    txtadd(t4, ".");
                                    txtnumint(t, (playerlist[x4]->GNPCflags[242] >> 16) & 255);
                                    txtadd(t4, t);
                                    txtadd(t4, ".");
                                    txtnumint(t, (playerlist[x4]->GNPCflags[242] >> 24) & 255);
                                    txtadd(t4, t);
                                    NET_send(NETplayer, playerlist[tpl]->net, t4);
                                    goto doneclmess;
                                }
                            }
                        }
                    }
                    txtset(t, "?");
                    t->d2[0] = 8;
                    txtadd(t, "No player named ");
                    txtadd(t, t2);
                    txtadd(t, " is in Britannia! (check uppercase/lowercase)");
                    NET_send(NETplayer, playerlist[tpl]->net, t);
                    goto doneclmess;
                } //GETIP
                txtset(t4, t2);
                txtucase(t4);
                txtset(t3, "QUITHOST");
                if (txtsame(t4, t3)) {
                    if (myobj2 = getprimarypartymemberobj(playerlist[tpl])) { tnpc2 = (npc *) myobj2->more; }
                    x4 = 0;
                    while (admins[x4]) {
                        if (txtsame(tnpc2->name, admins[x4])) {
                            u6orevive = 255;
                            seek(u6orevive_fh, 0);
                            put(u6orevive_fh, &u6orevive, 1);
                            exitrequest = TRUE;
                            exitrequest_noconfirm = TRUE;
                            //exitrequest is required for the host + client version to quit properly.
                            for (x4 = 0; x4 <= playerlist_last; x4++) {
                                if (playerlist[x4]) {
                                    if (myobj2 = getprimarypartymemberobj(playerlist[x4])) {
                                        /* send host quit message to all players */
                                        txtset(t, "??");
                                        t->d2[0] = 250;
                                        t->d2[1] = 1;
                                        NET_send(NETplayer, playerlist[x4]->net, t);
                                    }
                                }
                            }
                            goto doneclmess;
                        }
                        x4++;
                        if (x4 == ADMINSMAX) { break; }
                    }
                    goto privilege_denied;
                }
                txtset(t4, t2);
                txtucase(t4);
                txtset(t3, "ME ");
                if (txtsearch(t4, t3) == 1) goto anotherslashcommand;
                txtset(t4, t2);
                txtucase(t4);
                txtset(t3, "RETURNBODY");
                if (txtsearch(t4, t3) == 1) goto anotherslashcommand;

                txtsetchar(t, 8);
                txtadd(t, "Unknown slash (/) command!");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;

            privilege_denied:
                txtset(t, "?");
                t->d2[0] = 8;
                txtadd(t, "Incorrect privileges!");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;
            privilege_denied2:
                txtset(t, "?");
                t->d2[0] = 8;
                txtadd(t, "This player's privilege level is higher than yours!");
                NET_send(NETplayer, playerlist[tpl]->net, t);
                goto doneclmess;
            } //"/"
