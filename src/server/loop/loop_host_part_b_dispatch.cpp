// LHS-P4.2: part_b_dispatch -- MEGA B start (client-message dispatch). WIRE.
// LHS brace-seam: CONTINUES the part_00 fragment; OPENS the if(x3){ dispatch
// wrapper that is CLOSED in loop_host_part_b_leave_resurrect_house.cpp.
// Contents: interA prelude (free_socketclient, nextclientmessage, socket
// accept/read loop), host_gotmessage:, player_add, username/password format.
// Move-only: no message-format edits, no U6O_VERSION bump.

    if (sfbuffersend) {
        //clear sf buffer
        sfn = 0; //NULL sf pointer (0 is never used)
        for (y = 0; y <= 255; y++) {
            for (x = 0; x <= 255; x++) {
                //clear sf buffer
                if (i3 = sfi[y][x]) {
                    if (sf[i3].type == SF_TXT_PORTRAIT) free((txt *) sf[i3].p); //free txt
                    sfi[y][x] = 0;
                }
            }
        }
        sfbuffersend = 0;
    } //sfbuffersend

    //sockets: add new client
    if (newsocket != INVALID_SOCKET) {
        //static unsigned char socket1taken=1;//changes to 2 once socket1 is taken

        for (i = 1; i <= socketclientlast; i++) {
            if (socketclient[i] == INVALID_SOCKET) { goto free_socketclient; }
        }
        socketclientlast++;
        i = socketclientlast;
    free_socketclient:
        //if (i==1) socket1taken=2;

        socketclient[i] = newsocket;
        socketclient_ip[i] = newsocket_ip;

        //(re)init info
        socketclient_packetsize[i] = 0;
        socketclient_verified[i] = 0;
        socket_timeout[i] = 0;
        socket_disconnect[i] = 0;
        if (socketclient_si[i]) {
            //reinit thread info

            /*
      unsigned short i;//u6o socket connection array index
      OK txt *d[256];//array of txt*s
      OK unsigned char next;
      OK unsigned char nextfree;
      //other variables for temp use?
      ? long x4,x2,y2;
      txt *t;
      unsigned char exit_thread;
      unsigned long thread_id;
      */

            for (i2 = 0; i2 <= 255; i2++) {
                txtNEWLEN(socketclient_si[i]->d[i2], 0);
                txtNEWLEN(socketclient_ri[i]->d[i2], 0);
            }
            socketclient_si[i]->next = 0;
            socketclient_si[i]->nextfree = 0;
            socketclient_ri[i]->next = 0;
            socketclient_ri[i]->nextfree = 0;


            socketclient_si[i]->i = newsocket;
            socketclient_ri[i]->i = newsocket;
            socketclient_si[i]->exit_thread = 0;
            socketclient_ri[i]->exit_thread = 0;
            socketclient_ri[i]->t->l = 0;
            socketclient_ri[i]->t->d2[0] = 0;
        } else {
            //init thread info

            socketclient_si[i] = (sockets_info *) malloc(sizeof(sockets_info));
            socketclient_ri[i] = (sockets_info *) malloc(sizeof(sockets_info));
            ZeroMemory(socketclient_si[i], sizeof(sockets_info));
            ZeroMemory(socketclient_ri[i], sizeof(sockets_info));
            socketclient_si[i]->i = newsocket;
            socketclient_ri[i]->i = newsocket;
            for (i2 = 0; i2 <= 255; i2++) {
                socketclient_si[i]->d[i2] = txtnew();
                socketclient_ri[i]->d[i2] = txtnew();
            }
            socketclient_si[i]->next = 0;
            socketclient_si[i]->nextfree = 0;
            socketclient_ri[i]->next = 0;
            socketclient_ri[i]->nextfree = 0;
            socketclient_ri[i]->t = txtnew();
            txtNEWLEN(socketclient_ri[i]->t, -65536);
            socketclient_ri[i]->t->l = 0;
            socketclient_ri[i]->t->d2[0] = 0;
        }


        //COULD THIS BE THE BUG? IT DOESN'T CREATE A SEND SOCKET FOR I=1
        //if (i!=1){
        CreateThread(NULL, 0, sockets_send, (void *) i, 0, (unsigned long *) &socketclient_si[i]->thread_id);
        //}else{
        //socketclient_si[i]->thread_id=0;
        //}
        CreateThread(NULL, 0, sockets_receive, (void *) i, 0, (unsigned long *) &socketclient_ri[i]->thread_id);

        newsocket = INVALID_SOCKET;
    }

    static unsigned long tnet;
nextclientmessage:

    x3 = 0; //message available flag

    for (i = 1; i <= socketclientlast; i++) {
        if (socketclient[i] != INVALID_SOCKET) {
            //if socket has not sent any info (including keep-alive messages) for 8 minutes, then it is disconnected
            f = 64.0f;
            if (int((ett / f)) != int((ett / f) - (et / f))) {
                socket_timeout[i]++;
                if (socket_timeout[i] >= 8) {
                    if (socket_disconnect[i] == 0) socket_disconnect[i] = 1;
                }
            }

            if (socket_disconnect[i] == 1) {
                for (i2 = 0; i2 <= playerlist_last; i2++) {
                    if (playerlist[i2]) {
                        if (playerlist[i2]->net == i) playerlist[i2]->net = INVALID_NET; //set ->net pointer as invalid
                    }
                } //playerlist[i2],i2
                CreateThread(NULL, 0, sockets_disconnect, (void *) i, 0, (unsigned long *) &i2);
                socket_disconnect[i] = 2; //socket has been disconnected
                goto socket_disconnected;
            }

            //net_read()
            if (socketclient_ri[i]->d[socketclient_ri[i]->next]->l) {
                //buffered data available
                socket_timeout[i] = 0;
                txtset(t, socketclient_ri[i]->d[socketclient_ri[i]->next]);
                txtNEWLEN(socketclient_ri[i]->d[socketclient_ri[i]->next], 0);
                socketclient_ri[i]->next++;
                x3 = 1;
                tnet = i;
                goto host_gotmessage;
            } //->l
        } //!=INVALID_SOCKET
    socket_disconnected:;
    } //i
host_gotmessage:


    if (x3) {
        if (t->d2[0] == 255) goto player_add;
        if (t->d2[0] == 249) goto player_add; //host character!


        if (t->d2[0] == 246) {
            //check if player username AND/OR character name is available
            txtright(t, t->l - 1);
            i = t->d2[0];
            txtright(t, t->l - 1);
            txtset(t2, t);
            txtleft(t2, i); //user name
            txtright(t, t->l - i);
            i = t->d2[0];
            txtright(t, t->l - 1);
            txtset(t3, t);
            txtleft(t3, i); //character name
            txtright(t, t->l - i);
            //format names
            txtset(tusername, t2);
            txtset(tname, t3);
            format_usernames = 1;
            goto format_usernames0;
        format_usernames1:
            format_usernames = 0;

            //MessageBox(NULL,tusername->d,"Ultima 6 Online",MB_OK);
            //MessageBox(NULL,tname->d,"Ultima 6 Online",MB_OK);

            //check if names exist in saves
            i2 = 0;
            for (i = 0; i <= SAVESLOTLAST; i++) {
                //search save buffer for player info!
                if (save_buffer[i]) {
                    if (txtsamewithoutcase(save_username[i], tusername)) {
                        i2 = 1;
                    }
                    if (txtsamewithoutcase(save_name[i], tname)) {
                        i2 = 2;
                    }
                } //save_buffer[i]
            } //i
            //check if names exist in currently online players
            //todo!
            txtsetchar(t, 245);
            txtaddchar(t, i2);
            NET_send(NETplayer, tnet, t);
            goto doneclmess;
        }

        // MDD-P2.1/P2.2: client requested a slice of a baked map file. Serve it
        // straight from disk, bounds-checked against the manifest length, as
        // MSG_MAPCHUNK_RESP. Handled here -- ahead of the playerlist lookup --
        // so it works whether or not the requesting socket has fully logged in
        // yet (it needs only the socket index tnet, not a player object).
        // WIRE (MSG_MAPCHUNK_REQ -> MSG_MAPCHUNK_RESP), see define_both.h.
        if (t->d2[0] == MSG_MAPCHUNK_REQ) {
            unsigned char mreq_file = t->d2[1];
            unsigned long mreq_off, mreq_len;
            memcpy(&mreq_off, &t->d2[2], 4);
            memcpy(&mreq_len, &t->d2[6], 4);
            if (mreq_file < MAP_FILE_COUNT) {
                unsigned long mreq_filelen = MAP_manifest_len[mreq_file];
                if (mreq_len > MAP_CHUNK_BYTES) mreq_len = MAP_CHUNK_BYTES;
                if (mreq_off > mreq_filelen) mreq_off = mreq_filelen;
                if (mreq_off + mreq_len > mreq_filelen) mreq_len = mreq_filelen - mreq_off;
                if (mreq_len) {
                    file *mreq_fh = open2((LPCSTR) MAP_file_path(mreq_file), OF_READ | OF_SHARE_COMPAT);
                    if (mreq_fh->h != HFILE_ERROR) {
                        seek(mreq_fh, (long) mreq_off);
                        get(mreq_fh, MAP_chunkbuf, (long) mreq_len);
                    } else {
                        mreq_len = 0; // file missing -- reply empty; client keeps local data
                    }
                    close(mreq_fh);
                }
                txtset(t2, "?");
                t2->d2[0] = MSG_MAPCHUNK_RESP;
                txtaddchar(t2, mreq_file);
                txtaddlong(t2, mreq_off);
                txtaddlong(t2, mreq_len);
                if (mreq_len) {
                    txtNEWLEN(t3, (long) mreq_len);
                    memcpy(t3->d2, MAP_chunkbuf, mreq_len);
                    txtadd(t2, t3);
                }
                NET_send(NETplayer, tnet, t2);
            }
            goto doneclmess;
        }

        //KEEP ALIVE MESSAGE 251 IS POSSIBLE WITH OR WITHOUT A LOGGED IN PLAYER

        //check playerlist[] for tnet
        for (tpl = 0; tpl <= playerlist_last; tpl++) {
            if (playerlist[tpl]) {
                //current player
                if (playerlist[tpl]->net == tnet) goto foundclient;
            } //current player
        }

        goto doneclmess; //ignore message
    player_add: //add new player


        //send time
        txtset(t2, "?");
        t2->d2[0] = 32;
        txtset(t3, "12345678");
        t3->dd[0] = btime2;
        txtadd(t2, t3);
        NET_send(NETplayer, tnet, t2);

        static unsigned char usetpl0;
        usetpl0 = 0;
        if (t->d2[0] == 249) usetpl0 = 1;


        txtset(t5, t); //t5<-t
        txtright(t5, t5->l - 1);

        if (t5->ds[0] != U6O_VERSION) {
            //version 5.5 check
            txtset(t, "?");
            t->d2[0] = 254;
            NET_send(NETplayer, tnet, t); //incorrect version
            char version_msg[256];
            sprintf(version_msg, "Add_Player->Failed:Incorrect_Version (Client: %d, Host: %d)", (int) t5->ds[0],
                    (int) U6O_VERSION);
            txtset(t, version_msg);
            LOGadd(t);
            printf("%s\n", version_msg);
            goto doneclmess;
        }

        // MDD-P1.2: version accepted -- send this client the map-file manifest
        // (per-file byte length + FNV-1a/32 checksum). The client compares it
        // against its cached/local copies and pulls only the stale files via
        // MSG_MAPCHUNK_REQ. Built in a dedicated scratch txt so it cannot
        // disturb the setup-message parse still walking t5. WIRE (MSG_MAPMANIFEST)
        // -- see define_both.h / docs/plans/plan-clientMapDownload.md.
        {
            static txt *MAPmanifest_t = txtnew();
            txtset(MAPmanifest_t, "?");
            MAPmanifest_t->d2[0] = MSG_MAPMANIFEST;
            for (int mfid = 0; mfid < MAP_FILE_COUNT; mfid++) {
                txtaddlong(MAPmanifest_t, MAP_manifest_len[mfid]);
                txtaddlong(MAPmanifest_t, MAP_manifest_sum[mfid]);
            }
            NET_send(NETplayer, tnet, MAPmanifest_t);
        }

        txtright(t5, t5->l - 2);

        y9 = t5->d2[0]; //pw encryption
        txtright(t5, t5->l - 1);
        //get setup message info

        x = t5->d2[0];
        txtright(t5, t5->l - 1);
        txtNEWLEN(tname, x);
        memcpy(tname->d2, t5->d2, x);
        txtright(t5, t5->l - x); //name
        tmale_female = t5->d2[0];
        txtright(t5, t5->l - 1);
        if (tmale_female > 1) tmale_female = 1;
        tport = 194;
        x = t5->ds[0];
        if ((x >= 0) && (x <= 13)) tport = 194 + x;
        txtright(t5, t5->l - 2);
        ttype = 410;
        x = t5->d2[0];
        if ((x > 0) && (x <= 12)) ttype = 375 + x;
        txtright(t5, t5->l - 1);
        txtset(t6, t5);
        txtleft(t6, 28);
        txtright(t5, t5->l - 28); //t6<-virtue answers
        x = t5->d2[0];
        txtright(t5, t5->l - 1);
        txtNEWLEN(tusername, x);
        memcpy(tusername->d2, t5->d2, x);
        txtright(t5, t5->l - x);
        txtucase(tusername); //user_name
        x = t5->d2[0];
        txtright(t5, t5->l - 1);
        txtNEWLEN(tuserpassword, x);
        memcpy(tuserpassword->d2, t5->d2, x);
        txtright(t5, t5->l - x); //user_password
        /*if (y9==0) {
      txtucase(tuserpassword); //user_password
    }*/
        tcreatecharacter = t5->d2[0];
        txtright(t5, t5->l - 1);
        x = t5->d2[0];
        txtright(t5, t5->l - 1);
        txtNEWLEN(tnewuserpassword, x);
        memcpy(tnewuserpassword->d2, t5->d2, x);
        txtright(t5, t5->l - x); //new_user_password
        /*if (y9==0) {
      txtucase(tnewuserpassword); //new_user_password
    }*/
        tcustomportrait_upload = 0;
        tnamecolour = 0;
    getsetupmessageinfo_more:
        if (t5->l) {
            //... (more info)
            if (t5->d2[0] == 1) {
                //upload character portrait
                tcustomportrait_upload = 1;
                txtright(t5, t5->l - 1);
                memcpy(&tcustomportrait, t5->d, 7168);
                txtright(t5, t5->l - 7168);
                goto getsetupmessageinfo_more;
            }

            if (t5->d2[0] == 2) {
                txtright(t5, t5->l - 1); //u6o_namecolour
                tnamecolour = t5->dl[0] & 0xFFFFFF;
                txtright(t5, t5->l - 3);
                goto getsetupmessageinfo_more;
            } //u6o_namecolour


            if (u6o_namecolour) {
                txtaddchar(t, 2);
                txtaddshort(t, u6o_namecolour & 65535);
                txtaddchar(t, (u6o_namecolour >> 16) & 255);
            } //u6o_namecolour
        } //t5->l


        //autoformat: name, username, password, newuserpassword (all temp)
    format_usernames0:
        //name
        txtset(t5, tname);
    autoformat_name_cull2:
        for (i3 = 0; i3 < t5->l; i3++) {
            x = t5->d2[i3];
            x2 = 0; //cull?
            if (x < 48) x2 = 1;
            if ((x > 57) && (x < 65)) x2 = 1;
            if ((x > 90) && (x < 97)) x2 = 1;
            if (x > 122) x2 = 1;
            if ((x == 45) && (t5->d2[i3 + 1] != 45)) x2 = 0; //-
            if ((x == 39) && (t5->d2[i3 + 1] != 39)) x2 = 0; //'
            if ((x == 46) && (t5->d2[i3 + 1] != 46)) x2 = 0; //.
            if (x == 95) {
                t5->d2[i3] = 32;
                x = 32;
                x2 = 0;
                goto autoformat_name_cull2;
            } //_ to space
            if ((x == 32) && (t5->d2[i3 + 1] != 32)) {
                if (i3 && (i3 != (t5->l - 1))) x2 = 0;
            }
            if (x2) {
                txtset(t6, t5);
                txtright(t6, t5->l - i3 - 1);
                txtleft(t5, i3);
                txtadd(t5, t6);
                goto autoformat_name_cull2;
            }
        }
        if (t5->l > 16) txtleft(t5, 16);
        txtset(tname, t5);
        //username
        txtset(t5, tusername);
        txtucase(t5);
    autoformat_username_cull2:
        for (i3 = 0; i3 < t5->l; i3++) {
            x = t5->d2[i3];
            x2 = 0; //cull?
            if (x < 48) x2 = 1;
            if (x > 90) x2 = 1;
            if ((x > 57) && (x < 65)) x2 = 1;
            if (x2) {
                txtset(t6, t5);
                txtright(t6, t5->l - i3 - 1);
                txtleft(t5, i3);
                txtadd(t5, t6);
                goto autoformat_username_cull2;
            }
        }
        if (t5->l > 16) txtleft(t5, 16);
        txtset(tusername, t5);
        if (format_usernames) goto format_usernames1;
        //password
        if (y9 == SHA2_ENCRYPTION) {
            //add some salt and hash it again for save file
            txtset(t9, tusername);
            txtadd(t9, tuserpassword);
            if (txtsame(tnewuserpassword, tuserpassword)) txtset(tnewuserpassword, "");
            SHA2(t9, tuserpassword);
            txtset(t, "UNKNOWN");
            SHA2(t, t2);
            if (txtsame(tnewuserpassword, t2)) txtset(tnewuserpassword, "");
            if (tnewuserpassword->l) {
                txtset(t9, tusername);
                txtadd(t9, tnewuserpassword);
                SHA2(t9, tnewuserpassword);
            }
        } else {
            txtset(t5, tuserpassword);
            if (PW_ENCRYPTION == SHA2_ENCRYPTION) {
                //encrypt the old password and set as new pw
                SHA2(tuserpassword, tnewuserpassword);
                txtset(t9, tusername);
                txtadd(t9, tnewuserpassword);
                SHA2(t9, tnewuserpassword);
            }
            txtucase(t5);
        autoformat_password_cull2:
            for (i3 = 0; i3 < t5->l; i3++) {
                x = t5->d2[i3];
                x2 = 0; //cull?
                if (x < 48) x2 = 1;
                if (x > 90) x2 = 1;
                if ((x > 57) && (x < 65)) x2 = 1;
                if (x2) {
                    txtset(t6, t5);
                    txtright(t6, t5->l - i3 - 1);
                    txtleft(t5, i3);
                    txtadd(t5, t6);
                    goto autoformat_password_cull2;
                }
            }
            if (t5->l > 16) txtleft(t5, 16);
            txtset(tuserpassword, t5);
            //new password
            txtset(t5, tnewuserpassword);
            txtucase(t5);
        autoformat_password_cull4:
            for (i3 = 0; i3 < t5->l; i3++) {
                x = t5->d2[i3];
                x2 = 0; //cull?
                if (x < 48) x2 = 1;
                if (x > 90) x2 = 1;
                if ((x > 57) && (x < 65)) x2 = 1;
                if (x2) {
                    txtset(t6, t5);
                    txtright(t6, t5->l - i3 - 1);
                    txtleft(t5, i3);
                    txtadd(t5, t6);
                    goto autoformat_password_cull4;
                }
            }
            if (t5->l > 16) txtleft(t5, 16);
            txtset(tnewuserpassword, t5);
            //really a new password?
            txtset(t, "UNKNOWN");
            if (txtsame(tnewuserpassword, t)) txtset(tnewuserpassword, "");
            if (txtsame(tnewuserpassword, tuserpassword)) txtset(tnewuserpassword, "");
            if (PW_ENCRYPTION == SHA2_ENCRYPTION) {
                //encrypt the old password and set as new pw
                SHA2(t9, tnewuserpassword);
            }
        }

        //NULL username, password or character-name?
        x = 0;
        if (tusername->l == 0) x = 1;
        if (tuserpassword->l == 0) x = 1;
        if (tname->l == 0) x = 1;
        //"UNKNOWN" username or password?
        if (txtsame(tusername, t)) x = 1;
        if (txtsame(tuserpassword, t)) x = 1;
        if (x) {
            txtset(t, "??");
            t->d2[0] = 250;
            t->d2[1] = 2;
            NET_send(NETplayer, tnet, t);
            goto doneclmess;
        }

        static long journeyonward_i;

        if (tcreatecharacter) {
            //username and character-name must be unique
            for (i = 0; i <= SAVESLOTLAST; i++) {
                if (save_buffer[i]) {
                    if (txtsame(save_username[i], tusername)) {
                        txtset(t, "??");
                        t->d2[0] = 250;
                        t->d2[1] = 3;
                        NET_send(NETplayer, tnet, t);
                        goto doneclmess;
                    }
                    txtset(t, save_name[i]);
                    txtucase(t);
                    txtset(t2, tname);
                    txtucase(t2);
                    if (txtsame(t, t2)) {
                        txtset(t, "??");
                        t->d2[0] = 250;
                        t->d2[1] = 4;
                        NET_send(NETplayer, tnet, t);
                        goto doneclmess;
                    }
                }
            }
        } else {
            //!tcreatecharacter
            //username and password must exist
            for (i = 0; i <= SAVESLOTLAST; i++) {
                if (save_buffer[i]) {
                    if (txtsame(save_username[i], tusername)) {
                        if (txtsame(save_password[i], tuserpassword)) {
                            journeyonward_i = i;
                            goto check_username_password_ok;
                        }
                        if (y9 == PW_ENCRYPTION) {
                            txtset(t, "???");
                            t->d2[0] = 250;
                            t->d2[1] = 6;
                            t->d2[2] = 0;
                            NET_send(NETplayer, tnet, t);
                            goto doneclmess;
                            //request password without encryption (later replace with save file spesific encryption if needed)
                        } else {
                            //journeyonward_i=i; goto check_username_password_ok; //for pw reset
                            txtset(t, "??");
                            t->d2[0] = 250;
                            t->d2[1] = 5;
                            NET_send(NETplayer, tnet, t);
                            goto doneclmess;
                        }
                    }
                }
            } //i,save_buffer
            if (y9 == PW_ENCRYPTION) {
                txtset(t, "???");
                t->d2[0] = 250;
                t->d2[1] = 6;
                t->d2[2] = 0;
                NET_send(NETplayer, tnet, t);
                goto doneclmess;
                //request password without encryption (later replace with save file spesific encryption if needed)
            } else {
                txtset(t, "??");
                t->d2[0] = 250;
                t->d2[1] = 5;
                NET_send(NETplayer, tnet, t);
                goto doneclmess;
            }
        } //tcreatecharacter check
    check_username_password_ok:

        //if player is online, re-assign net pointer
        if (!tcreatecharacter) {
            for (i = 0; i <= playerlist_last; i++) {
                if (playerlist[i]) {
                    //current player
                    if (txtsame(tusername, playerlist[i]->name)) {
                        if (txtsame(tuserpassword, playerlist[i]->password)) {
                            if (socket_disconnect[playerlist[i]->net] == 0) socket_disconnect[playerlist[i]->net] = 1;
                            playerlist[i]->net = tnet; //re-establish connection
                            for (i2 = 0; i2 <= 7; i2++) {
                                if (playerlist[i]->party[i2]) {
                                    tnpc = (npc *) playerlist[i]->party[i2]->more;
                                    tnpc->upflags = 1;
                                } //party
                            } //i2

                            //flush object & mover buffers
                            playerlist[i]->x = 0;
                            playerlist[i]->y = 0;
                            playerlist[i]->sobj_bufoffx = 0;
                            playerlist[i]->sobj_bufoffy = 0;
                            playerlist[i]->mv_i = 0;
                            for (y = 0; y <= 71; y++) {
                                for (x = 0; x <= 95; x++) {
                                    playerlist[i]->sobj_bufsize[x][y] = 0;
                                    playerlist[i]->sobj_tempfixed[x][y] = 0;
                                }
                            }

                            playerlist[i]->idle = 0;
                            playerlist[i]->idle_connect = 0;

                            goto doneclmess; //player never left!
                        }
                    } //name,password
                } //current player
            } //i
        } //!tcreatecharacter

        //create player structure (using default values)
        if (usetpl0) {
            tpl = 0;
            i = 0;
            playerlist_last++;
            goto gotfreeindex;
        }
        for (i = 1; i <= playerlist_last; i++) {
            if (playerlist[i] == NULL) {
                //free index
                tpl = i;
                goto gotfreeindex;
            } //free index
        } //i
        playerlist_last++;
        tpl = playerlist_last;
        i = tpl; //no free index, allocate new index
    gotfreeindex:

        playerlist[tpl] = (player *) malloc(sizeof(player));
        ZeroMemory(playerlist[tpl], sizeof(player));


        playerlist[tpl]->talk_target = OBJnew();


        for (x = 0; x <= 9; x++) playerlist[tpl]->ktar[x] = OBJnew(); //setup NULL keyboard targeting objects
        playerlist[tpl]->net = tnet;
        playerlist[tpl]->name = txtnew();
        txtset(playerlist[tpl]->name, tusername);
        playerlist[tpl]->password = txtnew();
        txtset(playerlist[tpl]->password, tuserpassword);
        playerlist[tpl]->newpassword = txtnew();
        txtset(playerlist[tpl]->newpassword, tnewuserpassword);
        playerlist[tpl]->idle = 0;
        playerlist[tpl]->idle_connect = 0;
        playerlist[tpl]->karma = 100;
        playerlist[tpl]->male_female = tmale_female;
        playerlist[tpl]->mixer_volume = 255; //assume 100% mixer volume until updated

        //add player's sprite
        myobj = OBJnew();
        playerlist[i]->party[0] = myobj; //player<-
        myobj->type = ttype; //avatar (default)
        OBJadd(256 + 128 - 16, 256 + 128 + 2, myobj); //place avatar on the map

        //Step 3: create a NPC structure, the player's stats
        tnpc = (npc *) malloc(sizeof(npc));
        ZeroMemory(tnpc, sizeof(npc));
        myobj->more = tnpc;
        myobj->info |= 2; //myobj<-
        //create an INVISIBLE container for player's items
        myobj2 = OBJnew();
        myobj2->type = 188; //bag (INVISIBLE)
        myobj2->info |= 256; //quest items OK
        tnpc->items = myobj2;
        tnpc->baseitem = myobj2;
        tnpc->port = tport;
        tnpc->order = 0;
        tnpc->follow = playerlist[tpl]->party[0];
        tnpc->name = txtnew();
        tnpc->player = playerlist[tpl];
        tnpc->upflags = 1;
        if (tcreatecharacter) goto save_createcharacter;

        i = journeyonward_i;
        txtset(t2, ".\\save\\");
        txtnumint(t, i);
        txtadd(t, ".sav");
        txtadd(t2, t);
        tfh = open2(t2, OF_READWRITE | OF_SHARE_COMPAT);
        txtNEWLEN(t4, lof(tfh));
        get(tfh, t4->d2, t4->l);
        close(tfh);

        save_version = t4->ds[0];
        if (save_version == 3) {
            //DECRYPT3.0
            decrypt(t4);
            save_version = 2;
        }

        txtright(t4, t4->l - 2);
        if (save_version == 5) {
            txtright(t4, t4->l - 1);
            //for now do nothing with pw encrypt version will be useful later if some one implements a new encryption
        }
        x = t4->d2[0];
        txtright(t4, t4->l - 1 - x); //skip name (already known)
        x = t4->d2[0];
        txtright(t4, t4->l - 1 - x); //skip password (already known)

        x = t4->ds[0];
        txtright(t4, t4->l - 2);
        y = t4->ds[0];
        txtright(t4, t4->l - 2);
        OBJremove(playerlist[tpl]->party[0]);
        OBJadd(x, y, playerlist[tpl]->party[0]);
        playerlist[tpl]->male_female = t4->d2[0];
        txtright(t4, t4->l - 1);
        memcpy(&playerlist[tpl]->NPCflags, t4->d, 256 * 4);
        txtright(t4, t4->l - (256 * 4));
        memcpy(&playerlist[tpl]->GNPCflags, t4->d, 256 * 4);
        txtright(t4, t4->l - (256 * 4));
        playerlist[tpl]->karma = t4->df[0];
        txtright(t4, t4->l - 4);
        f = playerlist[tpl]->karma;
        stealing_MESSAGE = FALSE;

        if (save_version == 1) {
            if (t4->ds[0]) {
                myobj2 = OBJnew();
                myobj2->type = t4->ds[0];
                myobj2->info = t4->ds[1];
                myobj2->more2 = t4->dl[1];
                playerlist[tpl]->mobj = myobj2;
            }
            txtright(t4, t4->l - 8); //->mobj
        }

        myobj6 = NULL; //set to current deed!
        static object *iskiff[256], *ideed[256];
        static short iskiffx, ideedx;
        iskiffx = -1;
        ideedx = -1;
        static object *npchorsepapers[8];
        ZeroMemory(&npchorsepapers, sizeof(npchorsepapers));


    addnewpartymember:
        //KNOWN BUGS: multiple primary boats!, iskiff arrays(position), ->conversation is incorrect!


        //NPC
        //myobj,tnpc
        myobj->type = t4->ds[0];
        txtright(t4, t4->l - 2);

        x = t4->d2[0];
        txtright(t4, t4->l - 1);
        txtset(t, t4);
        txtleft(t, x);
        txtright(t4, t4->l - x);
        txtset(tnpc->name, t);
        if (playerlist[tpl]->party[1] == NULL) {
            //primary player
            txtset(tnpc->name, save_name[i]); //autoname edit
            txtset(tname, tnpc->name); //for ignore.txt???
        }

        //Kalour FIX
#ifdef CLIENT
        txtset(t, "Kalour");
        if (txtsame(t, tnpc->name)) {
            OBJremove(playerlist[tpl]->party[0]);
            OBJadd(387, 372, playerlist[tpl]->party[0]); //in swamp
        }
#endif
        tnpc->port = t4->ds[0];
        txtright(t4, t4->l - 2);
        if (playerlist[tpl]->party[1]) {
            //if there is second party member then this is not the primary player!
            tnpc->converse = 201;
            tnpc->schedule = &schedule2[0][0];
            tnpc->schedule_i = -1; //unknown
            tnpc->order = 1; //schedule
        }

        tnpc->hp = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->hp_max = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->mp = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->mp_max = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->s = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->d = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->i = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->exp = t4->dl[0];
        txtright(t4, t4->l - 4);
        tnpc->lev = t4->ds[0];
        txtright(t4, t4->l - 2);
        tnpc->flags = t4->d2[0];
        txtright(t4, t4->l - 1);


        tnpc->wt = 0;
        tnpc->wt_max = tnpc->s * 4 * 100;
        tnpc->wt2 = 0;
        tnpc->wt2_max = tnpc->s * 50;

        if (playerlist[tpl]->party[1]) {
            //if set, not primary partymember
            tnpc->mp_max = get_mp_max(tnpc->i);
            //tnpc->wt_max/=2;//halve wt_max for non-primary partymember
            tnpc->wt_max = tnpc->s * 3 * 100;
        } else {
            tnpc->mp_max = tnpc->i * 2;
        }

        if (save_version == 1) goto sv1_objects;

        //***HUGE VERSION DIFFERENCES AHEAD!***
        /*
    create an array to align contain object pointers
    extract objects from string (pointers will come first)

    */
        static object *sv2o[16384];
        static unsigned long sv2op[10];
        sv2op[0] = t4->dl[0];
        sv2op[1] = t4->dl[1];
        sv2op[2] = t4->dl[2];
        sv2op[3] = t4->dl[3];
        sv2op[4] = t4->dl[4];
        sv2op[5] = t4->dl[5];
        sv2op[6] = t4->dl[6];
        sv2op[7] = t4->dl[7];
        sv2op[8] = t4->dl[8];
        //txtnumint(t5,t4->dl[8]); txtadd(t5,":inv index"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);
        sv2op[9] = t4->dl[9];
        //txtnumint(t5,t4->dl[9]); txtadd(t5,":mobj index"); MessageBox(NULL,t5->d,"Ultima 6 Online",MB_OK);

        txtright(t4, t4->l - 10 * 4);


        /*-7168
    txtset(t2,"418.BIN");
    tfh=open2(t2,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
    put(tfh,t4->d2,t4->l);
    close(tfh);
    exit2(4);
    */

