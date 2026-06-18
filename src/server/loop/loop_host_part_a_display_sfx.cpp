// LHS-P3.6: part_a_display_sfx -- CONTINUES + CLOSES MEGA A.
// LHS brace-seam: CONTINUES + CLOSES the if(playerlist[tpl]){ and for(tpl..){
// blocks opened in loop_host_part_a_save.cpp (trailing braces at MEGA A end).
// Starts with a large commented-out reference block, then display_override,
// flash, SFX (sfxglobal, addsf, sfxglobal2).
                /*
        if (!playerlist[tpl]->updatemessage){
        static unsigned char *p,*p2;
        txtNEWLEN(t,-1048576); //create 1MB buffer
        p=t->d2;

        *p=1; p++;
        *(float*)p=btime2; p+=4;

        //central object offset->x,y
        if (playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        x=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->x;
        y=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->y;
        }else{
        x=playerlist[tpl]->px; y=playerlist[tpl]->py;
        }
        //screen offset->tpx,tpy
        tpx=x-15; tpy=y-11;
        if (tpx<0) tpx=0; if (tpy<0) tpy=0;
        if (tpx>2016) tpx=2016;
        if (tpy>1000) tpy=1000;

        //add tpx,tpy to message
        *(unsigned short*)p=tpx; p+=2;
        *(unsigned short*)p=tpy; p+=2;

        if (playerlist[tpl]->xray){ //xray!
        for (x2=0;x2<=65535;x2++) pfa[x2]=PFs+PFd;
        }else{
        pathfind2(x,y,NULL);
        }

        //send blank squares
        //with no obstructions this routine will still send 7 bytes (equivalent to only 2 objects!)
        //BUT it is optomized basic obstructions caused by buildings/caves etc.
        //OLD METHOD WAS: starting type(1), number of type(2), (assume type change)number of type(2)
        //NEW METHOD: number of type+type*128(1), ...
        //benefit: halved bandwidth in basic/complex situations
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


        //update basetiles message
        //update objects message
        static long updatex,updatey;
        x2=0; x3=0;
        p2=p; p+=2; //create 2 bytes space to set number of objects
        for (y=0;y<=23;y++){ for (x=0;x<=31;x++){
        updatex=tpx+x; updatey=tpy+y;

        myobj=od[updatey][updatex];

        nextobj: if (myobj){

        if (myobj->type==416){//view redirector
        if (myobj->more2){
        bt[updatey][updatex]=bt[myobj->more2>>16][myobj->more2&65535]; //update basetile flags (is this best location?)
        updatex=myobj->more2&65535; updatey=myobj->more2>>16;//change currect scan location
        myobj=od[myobj->more2>>16][myobj->more2&65535];//change current object
        goto nextobj;
        }
        }//view redirector



        i2=myobj->type&1023; //get object identity

        if (myobj->info&256){ //quest/don'tsend
        if (obji[sprlnk[i2]].weight==0){
        goto display_skip;
        }

        if (i2==73){ //moonstone
        if (playerlist[tpl]->GNPCflags[4]&(1<<(myobj->type>>10))) goto display_skip;
        }

        if (myobj->type==(275+2*1024)){//yellow potion
        if (myobj->info&256){//quest
        if (playerlist[tpl]->GNPCflags[32]&myobj->more2) goto display_skip;
        }//quest
        }//yellow potion

        if ((i2>=242)&&(i2<=249)){ //rune
        if (playerlist[tpl]->GNPCflags[5]&(1<<(i2-242))) goto display_skip;
        }
        if ((i2>=400)&&(i2<=407)){ //map(0-7)
        if (playerlist[tpl]->GNPCflags[7]&(1<<(i2-400))) goto display_skip;
        }
        if (i2==62){ //vortex cube
        if ((myobj->x==923)&&(myobj->y==850)){
        if ((playerlist[tpl]->GNPCflags[18]&1)==0) goto display_skip;
        }
        if ((myobj->x==1939)&&(myobj->y==57)){
        if (playerlist[tpl]->GNPCflags[6]&(1<<1)) goto display_skip;
        }
        }
        if (i2==389){ //silver tablet
        if (playerlist[tpl]->GNPCflags[6]&(1<<2)) goto display_skip;
        }
        if (i2==390){ //silver tabletII
        if (playerlist[tpl]->GNPCflags[6]&(1<<3)) goto display_skip;
        }
        if (i2==270){ //balloon plans
        if (playerlist[tpl]->GNPCflags[6]&(1<<4)) goto display_skip;
        }
        if (i2==395){ //broken gargoyle lens
        if (playerlist[tpl]->GNPCflags[6]&(1<<5)) goto display_skip;
        }
        if (i2==394){ //brit lens
        if ((playerlist[tpl]->GNPCflags[18]&2)==0) goto display_skip;
        }
        if (i2==396){ //garg lens
        if ((playerlist[tpl]->GNPCflags[18]&4)==0) goto display_skip;
        }
        }//quest/don't send

        if (myobj==nuggetsfix){ //nuggets
        if (playerlist[tpl]->online<1536) goto display_skip;
        if (playerlist[tpl]->GNPCflags[6]&(1<<6)) goto display_skip;
        }

        if (i2==59){ //codex
        if (playerlist[tpl]->GNPCflags[18]&8) goto display_skip;
        }

        if (i2==51){ //force field
        myobj2=(object*)myobj->prev;
        if (playerlist[tpl]->GNPCflags[3]&(1<<(myobj2->type>>10))) goto display_skip;
        }//force field

        if (i2==173){ //trap
        if ((myobj->info&(1<<9))==0) goto display_skip;
        }//trap

        crt=NULL;
        if (myobj->info&4){ //crt
        crt=(creature*)myobj->more;
        if (crt->flags&2) goto display_skip;
        }else{//!crt
        if (myobj->info&8){ //indirect crt
        if (((object*)myobj->more)->info&4) crt=(creature*)((object*)myobj->more)->more;
        }
        }//crt

        if (pfa[x+2+y*256+512]&(PFs+PFd)){

        //DISPLAY DOORS ON RANGE EDGES???

        //SUPPORT FOR VIEW REDIRECTORS
        if (i2==334) goto display_override; //secret door
        if (i2==213) goto display_override; //mousehole
        if (myobj2=OBJfindlastall(updatex,updatey)){
        if ((myobj2->type&1023)==301) goto display_override;
        }
        if (myobj2=OBJfindlastall(updatex,updatey+1)){
        if ((myobj2->type&1023)==301) goto display_override;
        }
        if (myobj2=OBJfindlastall(updatex+1,updatey)){
        if ((myobj2->type&1023)==301) goto display_override;
        }




        if (pfa[x+2+y*256+512+1]&(PFs+PFd)){
        if (pfa[x+2+y*256+512+256]&(PFs+PFd)){
        if (pfa[x+2+y*256+512+256+1]&(PFs+PFd)){
        display_override:

        //length from prev obj (stored extendable from 1 to 2 bytes)
        if (x2<=127){
        *p=x2; p++;
        }else{ //>127
        *p=(x2&127)+128; p++;
        *p=x2>>7; p++;
        }


        if (myobj->info&128){//stolen item
        for (z=0;z<=15;z++){
        if (stolenitemwarning[z]==myobj){
        if (stolenitemwarning_player[z]!=playerlist[tpl]){
        *(unsigned short*)p=1023+7*1024; p+=2;
        }
        }//==myobj
        }//z
        }//stolen item


        if (myobj->info&4){ //crt
        if (crt->flags&8){ //paralyzed!
        *(unsigned short*)p=1023+3*1024; p+=2;
        }//8, paralyzed
        if (playerlist[tpl]->ktar_display){
        for (z=0;z<=9;z++){
        if (playerlist[tpl]->ktar_wait[z]){
        if ((object*)playerlist[tpl]->ktar[z]->more==myobj){
        *(unsigned short*)p=1022+z*1024; p+=2;
        }
        }
        }
        }
        }//crt

        if (crt){ //direct or indirect crt
        //sleeping?
        if (crt->flags&4){
        z=myobj->type&1023; z2=0;
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024;
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        //monsters
        if (z==370) z2=339+1*1024; //headless
        if (z==371) z2=339+0*1024; //troll
        if (z==424){ //cyclops
        z2=340+((myobj->type>>10)&3)*1024;
        if (((myobj->type>>10)&3)!=1){
        if (bt[y+tpy][x+tpx]&1024) bt[y+tpy][x+tpx]-=1024; //set as blocked!
        }
        }
        if (z2){
        *(unsigned short*)p=z2; p+=2; goto type_or;
        }
        }//sleeping?
        }//direct or indirect crt

        if (myobj->info&2){
        tnpc=(npc*)myobj->more;

        if (tnpc->player){ //has player pointer
        if (tnpc->protection){*(unsigned short*)p=1023+4*1024; p+=2;}

        if (tnpc->light){
        *(unsigned short*)p=1023+1*1024; p+=2;
        }else{
        if (tnpc->player==playerlist[tpl]){
        *(unsigned short*)p=1023+2*1024; p+=2;
        }//->light
        }//tnpc->player

        if (myobj3=tnpc->armour){
        if (myobj3->type==81){ //storm cloak
        *(unsigned short*)p=1023+6*1024; p+=2;
        }//81
        }//armour


        if (obji[sprlnk[myobj->type&1023]].v4==1){ //humantype
        tplayer2=(player*)tnpc->player;

        if (tplayer2->party[0]==myobj){ //first player


        //first player of their party
        if (tplayer2->typing){
        if (tplayer!=tplayer2){ //not the same party member
        *(unsigned short*)p=1023+5*1024; p+=2;
        }
        }

        *(unsigned short*)p=myobj->type+16384; //16384=extra info
        if (tnpc->flags&4){ //if asleep switch to sleeping object
        z=myobj->type&1023; z2=0;
        //sleeping NPC schedule, but not in bed
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024; //2004
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        if (z2!=0){
        *(unsigned short*)p=z2+16384; //16384=extra info
        }
        }//asleep
        if (tnpc->flags&2) *(unsigned short*)p|=32768; //invisible
        p+=2;

        //U6OID    4bytes vs name(10)+colour(2)+unique id for messaging
        *(unsigned long*)p=tplayer2->id; p+=4;

        //addhp&mp bars (uses 1 byte of code) 0-15, 0-7, 1bit stating poisioned
        f=(float)tnpc->hp/(float)tnpc->hp_max; z=f*16; if (z==16) z=15;
        f=(float)tnpc->mp/(float)tnpc->mp_max; z2=f*8; if (z2==8) z2=7;
        z=z+(z2<<4); if (tnpc->flags&1) z+=128;
        *p=z; p++;
        goto type_or;
        }

        }
        }

        if (tnpc->flags&(2+4)){//2,4
        *(unsigned short*)p=myobj->type;
        if (tnpc->flags&4){ //if asleep switch to sleeping object
        z=myobj->type&1023; z2=0;
        //sleeping NPC schedule, but not in bed
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024; //2004
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        if (z2!=0){
        *(unsigned short*)p=z2;
        }
        }//asleep
        if (tnpc->flags&2) *(unsigned short*)p|=32768; //invisible
        p+=2;
        goto type_or;
        }//2,4

        if (tnpc->schedule){ if (tnpc->schedule_i!=-1){ if (tnpc->schedule[tnpc->schedule_i].type==0x91){ //sleeping!
        if (od[y+tpy][x+tpx]->type==163){
        *(unsigned short*)p=146; p+=2; goto type_or;
        }
        if (od[y+tpy][x+tpx]->type==(163+6144)){
        *(unsigned short*)p=146+1024; p+=2; goto type_or;
        }
        z=myobj->type&1023;
        z2=0;
        //sleeping NPC schedule, but not in bed
        if (z==376) z2=339+6*1024;
        if (z==377) z2=339+5*1024;
        if (z==378) z2=339+3*1024;
        if (z==379) z2=339+2*1024;
        if (z==380) z2=339+2*1024;
        if (z==381) z2=339+8*1024; //2004
        if (z==382) z2=339+4*1024;
        if (z==383) z2=339+8*1024;
        if (z==384) z2=339+5*1024;
        if (z==385) z2=339+8*1024;
        if (z==386) z2=339+7*1024;
        if (z==387) z2=339+3*1024;
        if (z==410) z2=339+7*1024;
        if (z2!=0){
        *(unsigned short*)p=z2; p+=2; goto type_or;
        }
        }}} //sleeping!

        }//->npc

        *(unsigned short*)p=myobj->type;
        if ((myobj->type&1023)==338) goto noflash; //blood
        if ((myobj->type&1023)==317) goto noflash; //317		fire field
        if ((myobj->type&1023)==318) goto noflash; //318		poison field
        if ((myobj->type&1023)==319) goto noflash; //319		protection field
        if ((myobj->type&1023)==320) goto noflash; //320		sleep field
        if ((myobj->type&1023)==84){ //red gate flash
        if (((myobj->info&112)>>4)>=2) goto flash;
        goto noflash;
        }
        if ((myobj->info&112)==16){
        flash:
        *(unsigned short*)p|=32768; //flashing
        }
        noflash:

        p+=2;
        type_or:

        x3++; //number of objects
        x2=0;

        }
        }
        }

        }//pf2!

        display_skip:
        if (myobj->next!=NULL){myobj=(object*)myobj->next; goto nextobj;}
        } //object exists
        x2++;
        }} //x,y loop
        *(unsigned short*)p2=x3;

        //selected partymember map position (if x==0 he is not onscreen)
        if (myobj=playerlist[tpl]->party[playerlist[tpl]->selected_partymember]){
        *(unsigned short*)p=myobj->x; p+=2;
        *(unsigned short*)p=myobj->y; p+=2;
        //selected partymember type
        *(unsigned short*)p=myobj->type; p+=2;
        }else{
        *(unsigned short*)p=0; p+=2;
        *(unsigned short*)p=0; p+=2;
        //selected partymember type
        *(unsigned short*)p=0; p+=2;
        }



        *p=0; t->l=(unsigned long)p-(unsigned long)t->d2; //finish direct string access


        static unsigned long chksum;
        chksum=0;
        for (x=5;x<t->l;x++){
        chksum=chksum+1+(t->d2[x]+1)*(x+1);
        }
        if (chksum!=playerlist[tpl]->MESSAGEupdate_chksum){
        NET_send(NETplayer,playerlist[tpl]->net,t);
        playerlist[tpl]->MESSAGEupdate_chksum=chksum;
        playerlist[tpl]->updatemessage=1; //a basetile+object update message was sent last "clientframe"
        } //chksum!=

        }else{playerlist[tpl]->updatemessage=0;} //updatemessage
        */


                //2003: fix input fields
                playerlist[tpl]->key2 = playerlist[tpl]->key; //2003: required backup key (client and host)
                playerlist[tpl]->readmessage = FALSE; //basic input not overwritten by second message

                //clear some keys by default while keeping others
                if (playerlist[tpl]->key & KEYmb1) playerlist[tpl]->key -= KEYmb1;
                if (playerlist[tpl]->key & KEYu) playerlist[tpl]->key -= KEYu;
                if (playerlist[tpl]->key & KEYl) playerlist[tpl]->key -= KEYl;
                if (playerlist[tpl]->key & KEYmbclick) playerlist[tpl]->key -= KEYmbclick;


                if (sfbuffersend) {
                    //create SF message 
                    // send sfx message
                    //[2]how many sfx
                    //[sfx2]
                    //...
                    txtset(mess1, "");
                    txtset(t, "?");
                    txtset(t2, "??");
                    txtset(t3, "????");
                    txtadd(mess1, t2); //<-must be updated later
                    i = 0;

                    if (playerlist[tpl]->party[playerlist[tpl]->selected_partymember]) {
                        x = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->x;
                        y = playerlist[tpl]->party[playerlist[tpl]->selected_partymember]->y;
                    } else {
                        x = playerlist[tpl]->px;
                        y = playerlist[tpl]->py;
                    }

                    //tpx=x-15; tpy=y-11;
                    //if (tpx<0) tpx=0; if (tpy<0) tpy=0;
                    //if (tpx>2016) tpx=2016;
                    //if (tpy>1000) tpy=1000;
                    getscreenoffset(x, y, &tpx, &tpy);
                    tpx >>= 3;
                    tpy >>= 3;
                    for (y = tpy; y <= tpy + 3; y++) {
                        for (x = tpx; x <= tpx + 4; x++) {
                        sfxglobal:
                            if (sfi[y][x]) {
                                i3 = sfi[y][x];
                            addsf:
                                t2->d2[0] = sf[i3].type & 255;
                                t2->d2[1] = sf[i3].type >> 8;
                                txtadd(mess1, t2);
                                t2->d2[0] = sf[i3].x & 255;
                                t2->d2[1] = sf[i3].x >> 8;
                                txtadd(mess1, t2);
                                t2->d2[0] = sf[i3].y & 255;
                                t2->d2[1] = sf[i3].y >> 8;
                                txtadd(mess1, t2);
                                static txt *dbgt3;
                                if (sf[i3].type == SF_TXT_PORTRAIT) {
                                    dbgt3 = (txt *) sf[i3].p; //length of appended text message
                                    //dbgt3=(txt*)&dbgt3;
                                    //exit (dbgt3->l);
                                    t2->ds[0] = dbgt3->l & 0xFFFF;
                                    txtadd(mess1, t2);
                                    t2->ds[0] = 0;
                                    txtadd(mess1, t2); //unused
                                } else {
                                    t2->d2[0] = sf[i3].x2 & 255;
                                    t2->d2[1] = sf[i3].x2 >> 8;
                                    txtadd(mess1, t2);
                                    t2->d2[0] = sf[i3].y2 & 255;
                                    t2->d2[1] = sf[i3].y2 >> 8;
                                    txtadd(mess1, t2);
                                }
                                t2->d2[0] = sf[i3].more & 255;
                                t2->d2[1] = sf[i3].more >> 8;
                                txtadd(mess1, t2);
                                t2->d2[0] = sf[i3].wait2[0];
                                t2->d2[1] = sf[i3].wait2[1];
                                txtadd(mess1, t2);
                                t2->d2[0] = sf[i3].wait2[2];
                                t2->d2[1] = sf[i3].wait2[3];
                                txtadd(mess1, t2);
                                if (sf[i3].type == SF_TXT_PORTRAIT) {
                                    txtadd(mess1, dbgt3); //append message
                                }
                                i++; //nsf
                                if (sf[i3].next) {
                                    i3 = sf[i3].next;
                                    goto addsf;
                                }
                            } //sfi!=NULL

                            if ((x == (2047 >> 3)) && (y == (1023 >> 3))) goto sfxglobal2;
                            if ((y == tpy + 3) && (x == tpx + 4)) {
                                x = 2047 >> 3;
                                y = 1023 >> 3;
                                goto sfxglobal;
                            }
                        }
                    } //x,y
                sfxglobal2:
                    mess1->d2[0] = i & 255;
                    mess1->d2[1] = i >> 8; //set number of entries

                    //if (playerlist[tpl]->net!=NULL){
                    if (mess1->l > 2) {
                        //avoid sending a NULL buffer
                        txtset(t, "?");
                        t->d2[0] = MSG_SF; //update SF
                        txtadd(t, mess1);
                        NET_send(NETplayer, playerlist[tpl]->net, t);
                    }
                } //sfbuffersend
            } //net connection
        } //current player
    idle_skip:;
    } //tpl loop
