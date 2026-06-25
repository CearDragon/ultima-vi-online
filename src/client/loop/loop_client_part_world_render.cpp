// LCS-P4.8-4.10: part_world_render — world render + lighting + floating text.
// LCS brace-seam: CONTINUES the in-game `{ ... }` block. This is the single
// big `if (mess_UPDATEps->l && clientframe) { ... }` statement (no interior
// depth-1 seam, hence one large part): world-tile draw, visibility (vis_scan2:),
// xray, mover/mask/offset, lighting/clouds/flash (flash_disable:/cloudadded:/
// asm_lightshow:), and on-screen/floating text (osdisplay_ktar:). Ends with the
// `} //(mess_UPDATEps->l&&clientframe)` close.
// ⚠️ HOT PATH — keep the fixed arrays / goto chains / inline asm exactly as-is.
// Move only — no STL, no reorg, no edits.
  //} //client (+host link)


  u6opi=3;





  if (clientframe&&tplay->x){
    directionalmove_only=0;




    static object* fakeobj=OBJnew_local();

    tplayer=tplay;


    x=ett;
    x=x%24; //x now between 0 and 23
    if (x>12) x=24-(12-(x-12));
    f=float(x)*1.25;
    x5=f;
    x5=0;
    if (btime>=22) x5=15;
    if (btime<=3) x5=15;
    if ((btime>=7)&&(btime<=18)) x5=0;
    if ((btime>=3)&&(btime<=7)){ //sunrise
      f=15.0f-((btime-3.0f)/4.0f*15.0f); //0->15
      x5=f;
    }
    if ((btime>=18)&&(btime<=22)){ //sunset
      f=(btime-18.0f)/4.0f*15.0f; //0->15
      x5=f;
    }
    timelval=x5;

    if (getambientlight(tplay->x,tplay->y)){
      timelval=AMBIENTLIGHT_LIGHTVALUE;
    }




    if (wizardeyetimeleft) timelval=0;

    if (nodisplay) goto skiprefresh2;


    // RW-P2.2 / RW-P3.3 trails fix: when the active back-buffer is
    // larger than the legacy 1024x768 world view, clear the whole
    // back-buffer to black at the start of each frame. The world tile
    // renderer overdraws the upper-left 1024x768 area every frame, but
    // anything outside that rect (extended right/bottom strips when the
    // window has been enlarged) is never overwritten by the world pass
    // ? so stale pixels (UI panels' previous positions, drifted clouds)
    // accumulated as visible trails. A single DD COLORFILL is
    // hardware-fast.
    //
    // (windowResize gate removed 2026-05-27 — the back buffer can grow
    // beyond 1024x768 whenever the window is enlarged.)
    if (backbufferW() > 1024 || backbufferH() > 768) {
        cls(ps, 0);
    }

    // RW-P2.3: route lighting buffer copy size through viewport.h accessor.
    // RW-P2.1: ls/ls_moonN are now heap pointers (lighting_alloc), so we
    // pass the pointers directly instead of `&ls`/`&ls_moonN`.
    {
      const unsigned long lsBytes = (unsigned long)lightingTotalBytes();
      if (moonlight==0) ZeroMemory(ls,lsBytes); //clear array
      if (moonlight==1) memcpy(ls,ls_moon1,lsBytes);
      if (moonlight==2) memcpy(ls,ls_moon2,lsBytes);
      if (moonlight==3) memcpy(ls,ls_moon3,lsBytes);
      if (moonlight==4) memcpy(ls,ls_moon4,lsBytes);
    }

    //calculate tpx,tpy from current x,y
    getscreenoffset(tplayer->x,tplayer->y,&tpx,&tpy);
    // ROOMSYNC-P1: must match the scene-update camera override above. If the
    // two camera assignment sites disagree (one centers in the room, the
    // other applies world-edge clamps) the input/render mapping diverges
    // and keypresses move the avatar to one tile while the camera renders
    // at another -- the user-visible "ghost camera / stuck character"
    // symptom we're eliminating. See docs/rendering/global-room-sync.md.
    if (getroom(tplayer->x, tplayer->y, NULL, NULL, NULL, NULL)) {
      tpx = tplayer->x - (viewTilesX()/2 - 1);
      tpy = tplayer->y - (viewTilesY()/2 - 1);
    }




    keyframe_backup=keyframe;
    oceantiles=0; rivertiles=0; britlens=0; garglens=0;

    osn=-1;
    stormcloak_last2=-1;


    //init vis arrays
    {
      int padX = viewTilesX() + 4;
      int padY = viewTilesY() + 4;
      for (y=0;y<padY;y++){ for (x=0;x<padX;x++){
        vis[x][y]=1;
        vis_window[x][y]=0;
        vis_chair[x][y]=0;
        vis_bed[x][y]=0;
        vis_slime[x][y]=0;
      }}
    }
    //get vis
    static long mapx,mapy,bufx,bufy;
    static unsigned short *tp2;
    myobj=fakeobj;
    {
      int limitY = viewTilesY() + 1;
      int limitX = viewTilesX() + 1;
      for (y=0;y<=limitY;y++){ for (x=0;x<=limitX;x++){
        x2=tpx+x-1; y2=tpy+y-1; if ((x2>=0)&&(y2>=0)&&(x2<=2047)&&(y2<=1023)){
        mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
        //get basetile-vis
        i=bt[y2][x2]&1023;
        i2=0;//visible
        if ((i>=192)&&(i<=207)) vis_window[x+1][y+1]=1;//window basetile
        if ((i>=140)&&(i<188)) i2=1;
        if ((i>=240)&&(i<252)) i2=1;
        if ((i>=192)&&(i<208)) i2=1;
        vis[x+1][y+1]=i2;
        //get fixedobj-vis
        if (i=objfixed_index[tpy+y-1][tpx+x-1]){
          i2=objfixed_type[i];
          for (i3=i+1;i3<=(i+i2);i3++){
            x3=objfixed_type[i3];
            myobj->type=x3;
            if ((x3>=29)&&(x3<=32)){//overlay floor: edit basetile
              if (x3==29) x4=108;
              if (x3==30) x4=208;
              if (x3==31) x4=210;
              if (x3==32) x4=213;
              bt[y2][x2]&=(0xFFFF-1023);
              bt[y2][x2]|=x4;
            }
            if ((myobj->type&1023)==301){
              if ((myobj->type&1024)==0) vis[x+1][y+1]=1;//top/left of doorframe always blocks
            }
            if ((myobj->type&1023)==213) vis[x+1][y+1]=1;//mousehole
            if (myobj->type==(327+2048)) vis_chair[x+1][y+1]=3;//throne
            if (myobj->type==163) vis_bed[x+1][y+1]=1;//bed (horizontal)
            if (myobj->type==163+(6*1024)) vis_bed[x+1][y+1]=2;//bed (vertical)
          }//i3
        }//i
        // DOB-P0.2 / RW-P4.11 follow-up (crash at loop_client.cpp:6825,
        // 2026-05-26): tplayer->sobj_bufoffx/y are streamed by the host
        // and can briefly lag the client's tpx/tpy while walking, which
        // pushes bufx/bufy outside the fixed-size sobj_bufsize[96][72]
        // (and sobj[][] / sobj_tempfixed[][]) arrays. Reading off the
        // end produced an unmapped-memory dereference (EXCEPTION_
        // ACCESS_VIOLATION, see crash.dmp). Mirror the bounds guard
        // already present in function_client.cpp:529 here so out-of-
        // buffer tiles fall through to the default vis state instead of
        // walking off the struct. Remove once DOB-P2+ replaces the
        // fixed 96x72 storage with a per-player Dynamic2DArray sized
        // from viewTilesX/Y.
        if ((bufx>=0)&&(bufx<96)&&(bufy>=0)&&(bufy<72)){
        //get tfixedobj-vis
        if (i=tobjfixed_index[tpy+y-1][tpx+x-1]){
          i2=tobjfixed_type[i];
          i5=1;
          for (i3=i+1;i3<=(i+i2);i3++){
            if ((tplayer->sobj_tempfixed[bufx][bufy]&i5)==0){
              x3=tobjfixed_type[i3];
              myobj->type=x3;
              x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
              if ((x5==334)&&((x4&1024)==0)) vis[x+1][y+1]=1;//secret door(closed)
              if ((myobj->type&1023)==298) vis_window[x+1][y+1]=1;//windowed door
              if ((myobj->type&1023)==252) vis_chair[x+1][y+1]=(myobj->type>>10)+1;//chair
              if ((x3&1023)!=x3){ //possibly buildable
                if (tclass_build[x3-1024]&2){//horizontal
                  x2--;
                  x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                  x2++;
                }
                if (tclass_build[x3-1024]&4){//vertical
                  y2--;
                  x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                  y2++;
                }
              }//build
            }//&i5
            i5<<=1;
          }//i3
        }//i
        //get objectbuffer-vis
        if (i=tplayer->sobj_bufsize[bufx][bufy]){
          tp2=tplayer->sobj[bufx][bufy];
          for (i2=0;i2<i;i2++){
            x3=tp2[i2];
            myobj->type=x3;
            x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
            if ((x5==334)&&((x4&1024)==0)) vis[x+1][y+1]=1;//secret door(closed)
            if ((myobj->type&1023)==252) vis_chair[x+1][y+1]=(myobj->type>>10)+1;//chair
            if ((x3&1023)!=x3){ //possibly buildable!
              if (tclass_build[x3-1024]&2){//horizontal
                x2--;
                x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                x2++;
              }
              if (tclass_build[x3-1024]&4){//vertical
                y2--;
                x4=myobj->type; x5=x4&1023; if ((x5>=297)&&(x5<=300)) if (x4>=4096) vis[x+1][y+1]=1;//door(s)
                y2++;
              }
            }//build
          }//i2
        }//i
        }//bufx/bufy in-range

        //get visalways
        x3=x2>>3; x4=x2&7;
        if (visalways[x3][y2]&(1<<x4)){
          vis[x+1][y+1]=0;
        }
        //get x,y vis complete
      }else vis[x+1][y+1]=1;
    }}
    }

    if (xray){
      int xrayY = viewTilesY() + 2;
      int xrayX = viewTilesX() + 2;
      for (y=1;y<=xrayY;y++){ for (x=1;x<=xrayX;x++){
        vis[x][y]=4;
      }}
      goto viewfind_skip;
    }

    //pathfind
    static unsigned char vis_index_x[16384];
    static unsigned char vis_index_y[16384];
    i=-1; //last index set
    i2=0; //last index checked
    mapx=tpx-2; mapy=tpy-2; x=tplayer->x-mapx; y=tplayer->y-mapy;
    //add windows
    if (vis_window[x][y-1]) vis[x][y-1]=0;
    if (vis_window[x+1][y]) vis[x+1][y]=0;
    if (vis_window[x][y+1]) vis[x][y+1]=0;
    if (vis_window[x-1][y]) vis[x-1][y]=0;
    i++; vis_index_x[i]=x; vis_index_y[i]=y;
vis_scan2:
    x=vis_index_x[i2]; y=vis_index_y[i2];
    if (vis[x-1][y]==0){i++; vis_index_x[i]=x-1; vis_index_y[i]=y; vis[x-1][y]=4;}
    if (vis[x+1][y]==0){i++; vis_index_x[i]=x+1; vis_index_y[i]=y; vis[x+1][y]=4;}
    if (vis[x][y-1]==0){i++; vis_index_x[i]=x; vis_index_y[i]=y-1; vis[x][y-1]=4;}
    if (vis[x][y+1]==0){i++; vis_index_x[i]=x; vis_index_y[i]=y+1; vis[x][y+1]=4;}
    if (i2<i){i2++; goto vis_scan2;}
    //0=visible, but blocked by somthing else
    //1=blocked
    //2=window
    //4=visible
    //8=edge visible
    {
      int edgesY = viewTilesY() + 2;
      int edgesX = viewTilesX() + 2;
      for (y=1;y<=edgesY;y++){ for (x=1;x<=edgesX;x++){ //find visible edges
        i=vis[x][y];
        if (i==4){
          if (vis[x+1][y]!=4) vis[x+1][y]=8;
          if (vis[x-1][y]!=4) vis[x-1][y]=8;
          if (vis[x][y+1]!=4) vis[x][y+1]=8;
          if (vis[x][y-1]!=4) vis[x][y-1]=8;
          if (vis[x-1][y-1]!=4) vis[x-1][y-1]=8;
          if (vis[x+1][y-1]!=4) vis[x+1][y-1]=8;
          if (vis[x+1][y+1]!=4) vis[x+1][y+1]=8;
          if (vis[x-1][y+1]!=4) vis[x-1][y+1]=8;
        }
      }}
    }
viewfind_skip:

    //get vischeck
    if (vischeck.data) {
        memset(vischeck.data, 0, viewTilesX() * viewTilesY());
    }
    {
      int tilesX = viewTilesX();
      int tilesY = viewTilesY();
      for (y=0;y<tilesY;y++){ for (x=0;x<tilesX;x++){
        if (vis[x+2][y+2]&4) vischeck[x][y]=1;
        if (vis[x+2][y+2]&8){
          if (vis[x+3][y+2]&4) vischeck[x][y]=1;
          if (vis[x+3][y+3]&4) vischeck[x][y]=1;
          if (vis[x+2][y+3]&4) vischeck[x][y]=1;
        }//&8
      }}//x,y
    }

    //base tiles
    {
      int tilesX = viewTilesX();
      int tilesY = viewTilesY();
      for (y=0;y<tilesY;y++){ for (x=0;x<tilesX;x++){
        i=bt[y+tpy][x+tpx]&1023;
        z=0;
        if ((i>=221)&&(i<224)) z=3; //lava
        if ((i>=2)&&(i<6)) z=3; //swamp
        if (z==3) LIGHTnew(x,y,(unsigned long)&ls3b,3);
        if ((i>=8)&&(i<48)){ //ocean and coast
          if (i<=15) oceantiles++; else rivertiles++;
          x2=i&7;
          y2=i/8;
          i2=i-8;
          x4=0; if (i2>=8) {i2=wateri[i2-8]; x4=1; }
          i2=i2*8+keyframe;
          x3=i2&31;
          y3=i2/32;
          y3+=4;
          sf32(ps,x*32,y*32,sfx8,i2+128);
          if (x4==1) g32z(ps,x*32,y*32,bt8[0],i);
        }else{//not ocean
          i2=0;
          if (i==252){i2=keyframe; i=14;}
          if (i==253){i2=keyframe; i=15;}
          if (i==254){i2=keyframe; i=0;}
          if ((i>=221)&&(i<=223)){i2=keyframe; i=i-210;}
          if ((i>=217)&&(i<=219)){i2=keyframe; i=i-209;}
          if (i<=7){i2=keyframe;} //changed
          g32(ps,x*32,y*32,bt8[i2],i);
        }
      }}
    }

    if (oceantiles||rivertiles){
      if (oceantiles>=576){
        f=9.0f; if (int((ett/f))!=int((ett/f)-(et/f))) soundplay2(u6osound[29],u6osound_volume[29]);
      }else{
        f=4.0f; if (int((ett/f))!=int((ett/f)-(et/f))) soundplay2(u6osound[9],u6osound_volume[9]);
      }
    }


    //minimap tiles its done in a VERY stupid way, but looks like the asm code doesn't like other size surfaces, so this will have to do for now.
    if(peer) {
      for (x5=0;x5<4;x5++) { for (y5=0;y5<4;y5++) {
        for (y=y5*24;y<24+y5*24;y++){ for (x=x5*24;x<24+x5*24;x++){
          if (((y+tpy-35)>=1024)||((y+tpy-35)<0)||((x+tpx-31)>=2048)||((x+tpx-31)<0)) {
            i=0;
          }
          else {
            i=bt[y+tpy-35][x+tpx-31]&1023;
          }
          if ((i>=8)&&(i<48)){ //ocean and coast
            if (i<=15) oceantiles++; else rivertiles++;
            i2=i-8;
            x4=0; if (i2>=8) {i2=wateri[i2-8]; x4=1; }
            sf32(ps5,(x-x5*24)*32,(y-y5*24)*32,sfx8,i2+128);
            if (x4==1) g32z(ps5,(x-x5*24)*32,(y-y5*24)*32,bt8[0],i);
          }else{//not ocean
            i2=0;
            if (i==252){i=14;}
            if (i==253){i=15;}
            if (i==254){i=0;}
            if ((i>=221)&&(i<=223)){i=i-210;}
            if ((i>=217)&&(i<=219)){i=i-209;}
            g32(ps5,(x-x5*24)*32,(y-y5*24)*32,bt8[0],i);
          }
        }}
        img(ps6,ps5);
        img0(minimap_frame->graphic,8+60*x5,8+60*y5,ps6);
      }}
    }


    //objfixed (non floating)
    myobj=fakeobj;
    for (y=viewTilesY()+1;y>=0;y--){ for (x=viewTilesX()+1;x>=0;x--){
      x2=x-1; y2=y-1;
      mapx=tpx+x-1; mapy=tpy+y-1; if ((mapx!=2048)&&(mapy!=1024)){

        if (i=objfixed_index[tpy+y-1][tpx+x-1]){
          i2=objfixed_type[i];
          for (i3=i+1;i3<=(i+i2);i3++){
            x3=objfixed_type[i3];
            myobj->type=x3;






            if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
              if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                GSs=1; GSx=x2*32; GSy=y2*32;
                getspr(myobj);
                getsound(myobj->type,x2,y2);
                getlight(myobj->type,x2,y2);
              }
            }}}}






            if ((x3&1023)!=x3){ //possibly buildable!
              if (tclass_build[x3-1024]&1){//square
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-2048;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-3072;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2++;
              }
              if (tclass_build[x3-1024]&2){//horizontal
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++;
              }
              if (tclass_build[x3-1024]&4){//vertical
                y2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                y2++;
              }
            }//build
          }//i3
        }//i

      }//mapx,mapy
    }}//x,y


    //tobjfixed
    myobj=fakeobj;
    for (y=0;y<=viewTilesY()+1;y++){ for (x=0;x<=viewTilesX()+1;x++){
      x2=x-1; y2=y-1;
      mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
      if ((mapx!=2048)&&(mapy!=1024)){

        if (i=tobjfixed_index[tpy+y-1][tpx+x-1]){
          i2=tobjfixed_type[i];
          i5=1;
          for (i3=i+1;i3<=(i+i2);i3++){
            if ((tplayer->sobj_tempfixed[bufx][bufy]&i5)==0){

              x3=tobjfixed_type[i3];
              myobj->type=x3;

              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){




                if (refreshcount&1){ for (x4=0;x4<=stolenitemwarningn;x4++){ if (stolenitemwarningx[x4]==mapx){ if (stolenitemwarningy[x4]==mapy){ if (stolenitemwarningtype[x4]==myobj->type){
                  static unsigned long *ps_realoffset;
                  // RW-P2.2 / RW-P4.10: scratch buffer sized to
                  // kBackbufferMaxW so getspr() ? which writes at the
                  // active back-buffer pitch ? can't overflow when the
                  // user has resized the window beyond the legacy 1024
                  // pitch. Must track kBackbufferMaxW in viewport.h.
                  static unsigned short ps_fakebuffer[u6o::client::kBackbufferMaxW*32];
                  static long siw_x,siw_y,siw_r,siw_g,siw_b;
                  for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                    ps_fakebuffer[siw_y*lightingStride()+siw_x]=0;
                  }}
                  ps_realoffset=ps->o;
                  ps->o=(unsigned long*)&ps_fakebuffer;
                  GSs=1; GSx=0; GSy=0;
                  getspr(myobj);
                  ps->o=ps_realoffset;
                  for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                    if (x5=ps_fakebuffer[siw_y*lightingStride()+siw_x]){
                      x6=x5>>11;
                      x7=(x5&0x7E0)>>6; if (x7>x6) x6=x7;
                      x7=x5&0x1F; if (x7>x6) x6=x7;
                      x6=x6<<11;
                      // RW-P2.2: route through lightingStride() == active ps row pitch in pixels.
                      ps->o2[(y2*32+siw_y)*lightingStride()+x2*32+siw_x]=x6;
                    }//x5
                  }}
                  goto stolenitemwarningflash;
                }}}}}

                //endgame lenses
                if (myobj->type==394){
                  if (endgame){
                    if (endgame_message>=7) goto lens_hide;
                  }
                  britlens=1;
                }
                if (myobj->type==396){
                  if (endgame){
                    if (endgame_message>=12) goto lens_hide;
                  }
                  garglens=1;
                }


                GSs=1; GSx=x2*32; GSy=y2*32;
                getspr(myobj);
stolenitemwarningflash:
lens_hide:






                getsound(myobj->type,x2,y2);
                getlight(myobj->type,x2,y2);

              }}}}





              if ((x3&1023)!=x3){ //possibly buildable!


                if (tclass_build[x3-1024]&1){//square

                  x2--;
                  if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                    myobj->type=x3-1024;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}
                  x2++; y2--;
                  if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                    myobj->type=x3-2048;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}
                  x2--;
                  if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                    myobj->type=x3-3072;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}
                  x2++; y2++;
                }
                if (tclass_build[x3-1024]&2){//horizontal
                  x2--;
                  if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                    myobj->type=x3-1024;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}

                  x2++;
                }
                if (tclass_build[x3-1024]&4){//vertical
                  y2--;
                  if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                    myobj->type=x3-1024;
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }}}}

                  y2++;
                }
              }//build
            }//&i5
            i5<<=1;
          }//i3
        }//i

      }//mapx,mapy
    }}//x,y


    x5option++; if (x5option>7) x5option=0;

    //objbuffer
    myobj=fakeobj;
    for (y=0;y<=viewTilesY()+1;y++){ for (x=0;x<=viewTilesX()+1;x++){
      mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
      x2=x-1; y2=y-1;
      // DOB-P0.2 follow-up (crash at loop_client.cpp:7257, 2026-05-27):
      // sibling of the guarded block at line 6808. Same root cause —
      // host-streamed sobj_bufoffx/y briefly lags the client's tpx/tpy
      // while walking and pushes bufx/bufy outside the fixed
      // sobj_bufsize[96][72] / sobj[][] arrays. Mirror the same guard
      // already present at loop_client.cpp:6808 and function_client.cpp:529.
      // Remove once DOB-P2+ replaces fixed 96x72 storage with a per-player
      // Dynamic2DArray sized from viewTilesX/Y.
      if ((bufx>=0)&&(bufx<96)&&(bufy>=0)&&(bufy<72)){
      if (i=tplayer->sobj_bufsize[bufx][bufy]){
        tp2=tplayer->sobj[bufx][bufy];
        for (i2=0;i2<i;i2++){
          x3=tp2[i2];
          myobj->type=x3;

          x5=0;//flash?
          if (myobj->type&32768){
            if ((sprlnk[(myobj->type+1)&1023]-sprlnk[myobj->type&1023])>32) goto flash_disable2;
            myobj->type-=32768; x3=myobj->type;

            x5=1; //goto flash_skip2;
          }
flash_disable2:

          if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
            if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating


              getsound(myobj->type,x2,y2);
              getlight(myobj->type,x2,y2);



              if (refreshcount&1){ for (x4=0;x4<=stolenitemwarningn;x4++){ if (stolenitemwarningx[x4]==mapx){ if (stolenitemwarningy[x4]==mapy){ if (stolenitemwarningtype[x4]==myobj->type){
                static unsigned long *ps_realoffset;
                // RW-P2.2 / RW-P4.10: scratch buffer sized to
                // kBackbufferMaxW. See note at the first occurrence
                // (~line 7118).
                static unsigned short ps_fakebuffer[u6o::client::kBackbufferMaxW*32];
                static long siw_x,siw_y,siw_r,siw_g,siw_b;
                for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                  ps_fakebuffer[siw_y*lightingStride()+siw_x]=0;
                }}
                ps_realoffset=ps->o;
                ps->o=(unsigned long*)&ps_fakebuffer;
                GSs=1; GSx=0; GSy=0;
                getspr(myobj);
                ps->o=ps_realoffset;
                for (siw_y=0;siw_y<=31;siw_y++){ for (siw_x=0;siw_x<=31;siw_x++){
                  if (x5=ps_fakebuffer[siw_y*lightingStride()+siw_x]){
                    x6=x5>>11;
                    x7=(x5&0x7E0)>>6; if (x7>x6) x6=x7;
                    x7=x5&0x1F; if (x7>x6) x6=x7;
                    x6=x6<<11;
                    // RW-P2.2: route through lightingStride() == active ps row pitch in pixels.
                    ps->o2[(y2*32+siw_y)*lightingStride()+x2*32+siw_x]=x6;
                  }//x5
                }}
                goto stolenitemwarningflash2;
              }}}}}


              if (x5){
                if (myobj->type==(84+1024)){ getspr(myobj); imgt0(ps,x2*32,y2*32,bt32); goto generatinggate1;}
              }

              if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }



generatinggate1:;

stolenitemwarningflash2:;
            }
          }}}}
          if ((x3&1023)!=x3){ //possibly buildable
            if (tclass_build[x3-1024]&1){//square
              x2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-1024;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }


                }
              }}}}
              x2++; y2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-2048;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

                }
              }}}}
              x2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-3072;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

                }
              }}}}
              x2++; y2++;
            }
            if (tclass_build[x3-1024]&2){//horizontal
              x2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-1024;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating

                  if (x5){
                    if (myobj->type==84){ getspr(myobj); imgt0(ps,x2*32,y2*32,bt32); goto generatinggate0;}
                  }


                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

generatinggate0:;

                }
              }}}}
              x2++;
            }
            if (tclass_build[x3-1024]&4){//vertical
              y2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-1024;
                if ((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)==0){//not floating
                  if (x5){getspr(myobj);if (x5option&2) img75t0(ps,x2*32,y2*32,bt32);if ((x5option==0)||(x5option==1)) imgt0(ps,x2*32,y2*32,bt32);if ((x5option==4)||(x5option==5)) img0(ps,x2*32,y2*32,bt32);}else{ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj); }

                }
              }}}}
              y2++;
            }
          }//build
          //flash_skip2:;
        }//i2
      }//i
      }//bufx/bufy in-range
    }}//objbuffer end


    //PASS 0: tanglevine tendrils ONLY
    //PASS 1: non-float
    //PASS 2: float
    static unsigned char flash_skip;
    for (z=0;z<=2;z++){//pass
      for (y=0;y<=viewTilesY()+1;y++){ for (x=0;x<=viewTilesX()+1;x++){
        mapx=tpx+x-1; mapy=tpy+y-1;
        for (i=0;i<tplayer->mv_i;i++){
          if (tplayer->mv_x[i]==mapx){ if (tplayer->mv_y[i]==mapy){
            x2=x-1; y2=y-1;
            x3=objgettype(tplayer->mv_type[i],tplayer->mv_dir[i],tplayer->mv_frame[i]);
            myobj->type=x3;

            //non-pass specific changes
            if (tplayer->mv_flags[i]&MV_PARALYZE) keyframe=0;
            flash_skip=0; if (tplayer->mv_flags[i]&MV_INVISIBLE){

              flash_skip=1;
            }
            //check if sleeping
            if (tplayer->mv_flags[i]&MV_SLEEP){
              z3=myobj->type&1023; z2=0;
              if (z3==376) z2=339+6*1024;
              if (z3==377) z2=339+5*1024;
              if (z3==378) z2=339+3*1024;
              if (z3==379) z2=339+2*1024;
              if (z3==380) z2=339+2*1024;
              if (z3==381) z2=339+8*1024;
              if (z3==382) z2=339+4*1024;
              if (z3==383) z2=339+8*1024;
              if (z3==384) z2=339+5*1024;
              if (z3==385) z2=339+8*1024;
              if (z3==386) z2=339+7*1024;
              if (z3==387) z2=339+3*1024;
              if (z3==410) z2=339+7*1024;
              if (z3==409) z2=myobj->type;//LB FIX
              if (z2){
                if (vis_bed[x2+2][y2+2]==1){
                  myobj->type=146; x3=myobj->type; goto moverinbed;
                }//1
                if (vis_bed[x2+2][y2+2]==2){
                  myobj->type=146+1024; x3=myobj->type; goto moverinbed;
                }//2
              }//z2
              //monsters
              if (z3==370) z2=339+1*1024;//headless
              if (z3==371) z2=339+0*1024;//troll
              if (z3==424) z2=340+3*1024;//cyclops
              if (z3==362) z2=341+3*1024;//winged gargoyle
              if (z2){myobj->type=z2; x3=z2;}
            }//MV_SLEEP
moverinbed:
            //end: non-pass specific changes


            //first pass only changes
            if (z==0){
              if ((x3&1023)==375) vis_slime[x+1][y+1]=1;//slime vis check
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                if (vischeck[x2][y2]){
                  if (tplayer->mv_flags[i]&MV_LIGHTBRIGHT) LIGHTnew(x2,y2,(unsigned long)&ls13,13);
                  if (tplayer->mv_flags[i]&MV_LIGHTGLOW) LIGHTnew(x2,y2,(unsigned long)&ls5b,5);
                  if (tplayer->mv_flags[i]&MV_STORMCLOAK){stormcloak_last2++; stormcloak_x2[stormcloak_last2]=x2; stormcloak_y2[stormcloak_last2]=y2;}
                  if (tplayer->mv_playerid[i]){
                    if ((mapx==tplayer->x)&&(mapy==tplayer->y)){
                      //423 balloon
                      //414 skiff
                      //412 ship
                      //415 raft
                      z3=myobj->type&1023;
                      if ((z3==423)||(z3==414)||(z3==412)||(z3==415)){
                        directionalmove_only=1;
                      }//z3
                    }//(mapx==tplayer->x)&&(mapy==tplayer->y)
                    z3=0;//display name
                    if (idlstn!=-1){
                      for (z2=0;z2<=idlstn;z2++){
                        if (tplayer->mv_playerid[i]==idlst[z2]){
                          osn++; osx[osn]=x2*32+16; osy[osn]=y2*32; osi[osn]=z2;
                          if ((x3&1023)==412) osy[osn]-=32;
                          if ((x3&1023)==423) osy[osn]-=32;
                          //horse edit
                          if (tplayer->mv_type[i]==431){//horse&rider
                            if (tplayer->mv_dir[i]==1){
                              osx[osn]-=16;
                              osy[osn]-=12;
                            }
                            if (tplayer->mv_dir[i]==3){
                              osx[osn]+=16;
                              osy[osn]-=12;
                            }
                            if (tplayer->mv_dir[i]==2){
                              osy[osn]-=32;
                            }
                          }


                          oshpmp[osn]=tplayer->mv_hpmp[i];
                          z3=1;
                          osvol[osn]=-1; if (idlst[z2]!=clientplayerid) osvol[osn]=idlst_volume[z2];
                        }
                      }//z2
                    }//!=-1
                    if (z3==0){ //add new id
                      // MM-P9.2 hardening: idlst[], idlst_name[], idlst_namecolour[]
                      // are sized [1024] and indexed by idlstn (starts at -1).
                      // idlstn++ had no bound, so encountering >1024 distinct
                      // player ids in one session overran the arrays (heap
                      // corruption / crash). Cap at the last valid slot (1023);
                      // any further new players simply render without a cached
                      // name-tag entry instead of corrupting memory.
                      if (idlstn < 1023){
                      idlstn++;
                      idlst[idlstn]=tplayer->mv_playerid[i];
                      idlst_name[idlstn]=txtnew();
                      txtset(idlst_name[idlstn],"reading data");
                      idlst_namecolour[idlstn]=0xFFFFFF;
                      txtset(t2,"?"); t2->d2[0]=9; txtset(t3,"????"); t3->dl[0]=tplayer->mv_playerid[i]; txtadd(t2,t3); NET_send(NETplayer,NULL,t2);
                      }
                    }//z3==0
                    if (tplayer->mv_flags[i]&MV_TYPING){
                      osn++;
                      osx[osn]=x2*32+16;
                      osy[osn]=y2*32;
                      osi[osn]=16+1024;
                    }//tplayer->MV_TYPING

                    if (tplayer->mv_flags[i]&MV_SPEAKING){
                      osn++;
                      osx[osn]=x2*32+16;
                      osy[osn]=y2*32;
                      osi[osn]=17+1024;
                    }//tplayer->MV_SPEAKING


                  }//playedid
                  if (tplayer->mv_ktar[i]){
                    if (ktar_display){
                      osn++;
                      osx[osn]=x2*32+16;
                      osy[osn]=y2*32;
                      osi[osn]=tplayer->mv_ktar[i]-1+1024;
                    }
                  }//ktar

                }//vischeck
              }}}}

            }//z==0
            //end: first pass only changes


            //PASS 0: selected movers only
            if (z==0){
              if ((x3&1023)==366) goto passok;//tanglevine tendril
              if ((x3&1023)==414) goto passok;//skiff
              goto passskip;
            }
            if (z==1){
              if ((x3&1023)==366) goto passskip;//tanglevine tendril
              if ((x3&1023)==414) goto passskip;//skiff
            }







passok:









            if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
              if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))||(z==0)){//floating check

                if (vis_chair[x2+2][y2+2]){
                  if (obji[sprlnk[myobj->type&1023]].v4==1){//"avatar"
                    if ((myobj->type&1023)!=363){//not a gargoyle!
                      myobj->type=objgettype(tplayer->mv_type[i],vis_chair[x2+2][y2+2]-1,3);
                    }
                  }
                }


                if ((x3&1023)==375){//slime
                  x4=0;
                  if (vis_slime[x+1][y+1-1]) x4|=1;
                  if (vis_slime[x+1+1][y+1]) x4|=2;
                  if (vis_slime[x+1][y+1+1]) x4|=4;
                  if (vis_slime[x+1-1][y+1]) x4|=8;
                  myobj->type=375+x4*1024;
                }//slime

                if (flash_skip==0){
                  GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);
                }else{
                  getspr(myobj); imgt0(ps,x2*32,y2*32,bt32);
                }

                getsound_MOVERSOUND=1; getsound(myobj->type,x2,y2); getsound_MOVERSOUND=0;
                getlight(myobj->type,x2,y2);

              }
            }}}}





            if ((x3&1023)!=x3){ //possibly buildable!


              if (x3==(340+3*1024)) goto mover_square;
              if (x3==(341+3*1024)) goto mover_square;
              if (tclass_build[x3-1024]&1){//square
mover_square:


                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-2048;
                  if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-3072;
                  if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2++;
              }
            }//build
            if (tclass_mover[x3]&2){//2 part mover (cow,ant,etc.)
              x4=(x3>>11)&3;
              if (x4==0) y2++;
              if (x4==1) x2--;
              if (x4==2) y2--;
              if (x4==3) x2++;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3+1024*8;

                x8=0;//double headed animal?
                x7=mapx; y7=mapy;
                if ((x7>=747)&&(x7<=829)&&(y7>=905)&&(y7<=983)) x8=1; //on island
                if ((x7>=792)&&(x7<=796)&&(y7>=937)&&(y7<=938)) x8=0; //in cage
                if ((x7==1475)&&(y7==244)) x8=1; //pushmepullyou
                if (x8){
                  x8=myobj->type>>11;
                  x8-=4;
                  if (x8==0) x8=2; else if (x8==2) x8=0; else if (x8==1) x8=3; else if (x8==3) x8=1;
                  myobj->type=(myobj->type&2047)+x8*2048;
                }//x8

                if (((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1)&&(z==2))||((objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]==0)&&(z==1))){//floating check

                  if (flash_skip==0){
                    GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);
                  }else{
                    getspr(myobj); imgt0(ps,x2*32,y2*32,bt32);
                  }


                }
              }}}}
              if (x4==0) y2--;
              if (x4==1) x2++;
              if (x4==2) y2++;
              if (x4==3) x2--;
            }//2 part mover

            if ((x3&1023)==412){//ship
              if (z==2){//float
                x4=(x3>>11)&3; x5=x2; y5=y2;
                if (x4==0){
                  myobj->type=x3-1*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5-1; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==2){
                  myobj->type=x3-1*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5-1; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==1){
                  myobj->type=x3-1*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5+1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==3){
                  myobj->type=x3-1*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-8*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3-9*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+8*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+7*1024; x2=x5+1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                x2=x5; y2=y5;
              }//float
            }//boat

            if ((x3&1023)==411){//dragon
              if (z==2){//float
                //use random
                x4=(x3>>11)&3; x5=x2; y5=y2;
                x6=0; //head (should change from 0 to 1 randomly)
                y6=0; //tail
                y6=ett+(float)x2+(float)y2; y6&=1;
                x6=ett/1.4f+(float)x2+(float)y2; x6&=1;

                if (x4==0){
                  myobj->type=x3+8*1024+x6*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==1){
                  myobj->type=x3+8*1024+x6*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==2){
                  myobj->type=x3+8*1024+x6*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                if (x4==3){
                  myobj->type=x3+8*1024+x6*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+16*1024+y6*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+24*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                  myobj->type=x3+32*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                }
                x2=x5; y2=y5;
              }//float
            }//dragon

            if ((x3&1023)==374){//hydra
              if (z==2){//float
                x5=x2; y5=y2;
                myobj->type=425+0*1024; x2=x5; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+4*1024; x2=x5+1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+8*1024; x2=x5+1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+12*1024; x2=x5+1; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+16*1024; x2=x5; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+20*1024; x2=x5-1; y2=y5+1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+24*1024; x2=x5-1; y2=y5; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                myobj->type=425+28*1024; x2=x5-1; y2=y5-1; if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){ GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);}}}}
                x2=x5; y2=y5;
              }//float
            }//hydra

            //mover_flash_skip:;

passskip:;

            //after pass 1 effects
            if (z==1){
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                if (vischeck[x2][y2]){
                  if (tplayer->mv_flags[i]&MV_PROTECT) sf32z(ps,x2*32,y2*32,sfx8,3*32+19+(keyframe>>1));
                }
              }}}}
            }//z==1
            //end: after pass 1 effects

            //after pass 2 extras
            if (z==2){


              if (tplayer->mv_type[i]==431){
                if (tplayer->mv_more[i]){

                  myobj->type=tplayer->mv_more[i]+(tplayer->mv_dir[i]*4+1)*1024;

                  if (tplayer->mv_more[i]==354){//mouse
                    myobj->type=tplayer->mv_more[i]+tplayer->mv_dir[i]*1024;
                    getspr(myobj);
                    x5=x2*32; y5=y2*32;
                    if (tplayer->mv_dir[i]==0){
                      y5+=21;
                      x5-=1;
                    }
                    if (tplayer->mv_dir[i]==1){
                      x5-=23;
                      y5-=3;
                    }
                    if (tplayer->mv_dir[i]==2){
                      y5-=25;
                      x5-=7;
                    }
                    if (tplayer->mv_dir[i]==3){
                      x5+=23;
                      y5-=11;
                    }
                    goto onhorse_specialoffsetused;
                  }//mouse

                  if (tplayer->mv_more[i]==363){//garg
                    myobj->type=tplayer->mv_more[i]+(tplayer->mv_dir[i]*3+1)*1024;
                  }



                  getspr(myobj);




                  if ((tplayer->mv_more[i]==410)||(tplayer->mv_more[i]==387)||(tplayer->mv_more[i]==386)||(tplayer->mv_more[i]==378)){

                    if ((tplayer->mv_more[i]==410)||(tplayer->mv_more[i]==386)){
                      if ((tplayer->mv_dir[i]==1)||(tplayer->mv_dir[i]==2)) goto usenormalmask;
                    }

                    //special masks
                    if ((tplayer->mv_dir[i]==0)||(tplayer->mv_dir[i]==2)){
                      for (x5=0;x5<1024;x5++){
                        if (horsemaskdress->o2[x5]) bt32->o2[x5]=0;
                      }
                      img0(bt32,0,0,horsemaskdressb);
                    }else{
                      for (x5=0;x5<1024;x5++){
                        if (horsemask2dress->o2[x5]) bt32->o2[x5]=0;
                      }
                      img0(bt32,0,0,horsemask2dressb);
                    }

                    goto specialmaskused;
                  }
usenormalmask:
                  //apply masks
                  if ((tplayer->mv_dir[i]==0)||(tplayer->mv_dir[i]==2)){
                    for (x5=0;x5<1024;x5++){
                      if (horsemask->o2[x5]) bt32->o2[x5]=0;
                    }


                  }else{
                    for (x5=0;x5<1024;x5++){
                      if (horsemask2->o2[x5]) bt32->o2[x5]=0;
                    }


                  }

specialmaskused:



                  x5=x2*32; y5=y2*32;
                  if (tplayer->mv_dir[i]==0){
                    y5+=19;
                    x5-=7;
                  }
                  if (tplayer->mv_dir[i]==1){
                    x5-=21;
                    y5-=7;
                  }
                  if (tplayer->mv_dir[i]==2){
                    y5-=21;
                    x5-=7;
                  }
                  if (tplayer->mv_dir[i]==3){
                    x5+=17;
                    y5-=7;
                  }


onhorse_specialoffsetused:

                  if (flash_skip==0){
                    img0(ps,x5,y5,bt32);
                  }else{
                    imgt0(ps,x5,y5,bt32);
                  }






                }}

            }//z==2

            keyframe=keyframe_backup;
          }}//mapx,mapy
        }//i
      }}//x,y
    }//z(pass)

    //objbuffer (floating)
    myobj=fakeobj;
    for (y=0;y<=viewTilesY()+1;y++){ for (x=0;x<=viewTilesX()+1;x++){
      mapx=tpx+x-1; mapy=tpy+y-1; bufx=mapx-tplayer->sobj_bufoffx; bufy=mapy-tplayer->sobj_bufoffy;
      x2=x-1; y2=y-1;
      // DOB-P0.2 follow-up (crash at loop_client.cpp:7257, 2026-05-27):
      // same sobj_bufoffx/y lag pattern, applied prophylactically to the
      // floating-objbuffer pass which would have crashed identically on
      // the next viewport size. See loop_client.cpp:6808 / 7257 /
      // function_client.cpp:529 for the same guard.
      if ((bufx>=0)&&(bufx<96)&&(bufy>=0)&&(bufy<72)){
      if (i=tplayer->sobj_bufsize[bufx][bufy]){
        tp2=tplayer->sobj[bufx][bufy];
        for (i2=0;i2<i;i2++){
          x3=tp2[i2];
          myobj->type=x3;
          //flashing?
          if (myobj->type&32768){
            if ((sprlnk[(myobj->type+1)&1023]-sprlnk[myobj->type&1023])>32) goto flash_disable;
            myobj->type-=32768;
            if (refreshcount&1) goto flash_skip;
          }
flash_disable:
          if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
            if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
              GSs=1; GSx=x2*32; GSy=y2*32; getspr(myobj);
              getsound(myobj->type,x2,y2);
              getlight(myobj->type,x2,y2);
            }
          }}}}
          if ((x3&1023)!=x3){//might be buildable
            if (tclass_build[x3-1024]&1){//square
              x2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-1024;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2++; y2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-2048;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-3072;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2++; y2++;
            }
            if (tclass_build[x3-1024]&2){//horizontal
              x2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-1024;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              x2++;
            }
            if (tclass_build[x3-1024]&4){//vertical
              y2--;
              if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                myobj->type=x3-1024;
                if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                  GSs=1; GSx=x2*32; GSy=y2*32;
                  getspr(myobj);
                }
              }}}}
              y2++;
            }
          }//build
flash_skip:;
        }//i2
      }//i
      }//bufx/bufy in-range
    }}//objbuffer (floating) end


    //objfixed (floating)
    myobj=fakeobj;
    for (y=0;y<=viewTilesY()+1;y++){ for (x=0;x<=viewTilesX()+1;x++){
      x2=x-1; y2=y-1;
      mapx=tpx+x-1; mapy=tpy+y-1; if ((mapx!=2048)&&(mapy!=1024)){

        if (i=objfixed_index[tpy+y-1][tpx+x-1]){
          i2=objfixed_type[i];
          for (i3=i+1;i3<=(i+i2);i3++){
            x3=objfixed_type[i3];
            myobj->type=x3;
            if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
              if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                GSs=1; GSx=x2*32; GSy=y2*32;
                getspr(myobj);
                getsound(myobj->type,x2,y2);
                getlight(myobj->type,x2,y2);
              }
            }}}}


            if ((x3&1023)!=x3){ //possibly buildable!


              if (tclass_build[x3-1024]&1){//square
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-2048;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-3072;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++; y2++;
              }
              if (tclass_build[x3-1024]&2){//horizontal
                x2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                x2++;
              }
              if (tclass_build[x3-1024]&4){//vertical
                y2--;
                if (x2>=0){ if (x2<viewTilesX()){ if (y2>=0){ if (y2<viewTilesY()){
                  myobj->type=x3-1024;
                  if (objfloatflags[(myobj->type>>10)+sprlnk[myobj->type&1023]]&1){//floating
                    GSs=1; GSx=x2*32; GSy=y2*32;
                    getspr(myobj);
                  }
                }}}}
                y2++;
              }
            }//build
          }//i3
        }//i

      }//mapx,mapy

    }}//x,y


    for (y=2;y<=viewTilesY()+1;y++){ for (x=2;x<=viewTilesX()+1;x++){ //overwrite objects on reverse side of wall
      if (vis[x][y]==8){
        mapx=tpx+x-2; mapy=tpy+y-2;
        i=bt[mapy][mapx]&1023;
        if (((i>=140)&&(i<=207))||((i>=240)&&(i<=251))){
          if (((vis[x][y+1]&12)==0)||((vis[x+1][y]&12)==0)||((vis[x+1][y+1]&12)==0)){
            g32(ps,(x-2)*32,(y-2)*32,bt8[0],i);
          }
        }//i
      }//8
    }}
    for (y=2;y<=viewTilesY()+1;y++){ for (x=2;x<=viewTilesX()+1;x++){//edit edge basetiles
      if (vis[x][y]==8){
        mapx=tpx+x-2; mapy=tpy+y-2;
        i=bt[mapy][mapx]&1023;
        i2=i;
        if (i==146){//corner going down to straight accross
          if ((vis[x][y+1]&12)==0) i=144;
          if ((vis[x+1][y]&12)==0) i=145;
        }
        if (i==162){
          if ((vis[x][y+1]&12)==0) i=160;
          if ((vis[x+1][y]&12)==0) i=161;
        }
        if (i==178){
          if ((vis[x][y+1]&12)==0) i=176;
          if ((vis[x+1][y]&12)==0) i=177;
        }
        if ((i>=140)&&(i<188)){//check corners
          i3=0;
          if (i>=160) i3=1;
          if (i>=176) i3=2;
          if ((vis[x][y-1]&12)==0){ if ((vis[x+1][y]&12)==0){
            if (i3==0) i=152;
            if (i3==1) i=152+16;
            if (i3==2) i=152+32;
          }}
          if ((vis[x][y+1]&12)==0){ if ((vis[x-1][y]&12)==0){
            if (i3==0) i=152+1;
            if (i3==1) i=152+16+1;
            if (i3==2) i=152+32+1;
          }}
          if ((vis[x][y+1]&12)==0){ if ((vis[x+1][y]&12)==0){
            if (i3==0) i=152-3;
            if (i3==1) i=152+16-3;
            if (i3==2) i=152+32-3;
          }}
        }//i
        if (i!=i2){
          g32(ps,(x-2)*32,(y-2)*32,bt8[0],i);
        }
      }//8
    }}
    for (y=2;y<=viewTilesY()+1;y++){ for (x=2;x<=viewTilesX()+1;x++){ //black corners
      if (vis[x][y]==8){
        mapx=tpx+x-2; mapy=tpy+y-2;
        i=bt[mapy][mapx]&1023;
        if ((i>=140)&&(i<188)){
          if ((vis[x+1][y]&12)==0){ if ((vis[x][y-1]&12)==0){
            sf32z(ps,(x-2)*32,(y-2)*32,sfx8,10);
          }}
          if ((vis[x-1][y]&12)==0){ if ((vis[x][y+1]&12)==0){
            sf32z(ps,(x-2)*32,(y-2)*32,sfx8,11);
          }}
        }//i
      }//8
    }}
    //set black squares
    myobj=fakeobj;
    myobj->type=331+22*1024;
    for (y=0;y<viewTilesY();y++){ for (x=0;x<viewTilesX();x++){
      if (vis[x+2][y+2]<4){
        GSs=1; GSx=x*32; GSy=y*32;
        getspr(myobj);
      }
    }}







    if (endgame){
      if (endgame_timer<=140.0f){
        myobj=fakeobj;

        i=0;
        if ((endgame_message>=2)&&(endgame_message<=6)) i=1;
        if (i){
          myobj->type=84+1024; GSx=(921-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=84; GSx=(920-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

        i=0;
        if (endgame_message>=3) i=1;
        if (i){
          myobj->type=409+4*1024; GSx=(921-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=378+9*1024; GSx=(920-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

        i=0;
        if ((endgame_message>=9)&&(endgame_message<=11)) i=1;
        if (i){
          myobj->type=84+1024; GSx=(926-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=84; GSx=(925-tpx)*32; GSy=(850-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

        i=0;
        if (endgame_message>=10) i=1;
        if (i){
          myobj->type=363+9*1024; GSx=(925-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=363+7*1024; GSx=(925+2-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+36*1024; GSx=(925-tpx)*32; GSy=(849-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+37*1024; GSx=(925+1-tpx)*32; GSy=(849-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+38*1024; GSx=(925-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
          myobj->type=362+39*1024; GSx=(925+1-tpx)*32; GSy=(849+1-tpy)*32;
          if ((GSx>=0)&&(GSx<=992)&&(GSy>=0)&&(GSy<=736)){GSs=1; getspr(myobj);}
        }

      }
    }


    if (wizardeyetimeleft){
      myobj=fakeobj;
      myobj->type=52;
      GSs=1; GSx=(tplayer->x-tpx)*32; GSy=(tplayer->y-tpy)*32; getspr(myobj);
    }

    if ((minimap_frame->offset_x>=kPanelHideThresholdX)&&peer){
      minimap_frame->offset_x-=kPanelHideDeltaX;
    }else if((minimap_frame->offset_x<=kPanelHideThresholdX)&&!peer){
      minimap_frame->offset_x+=kPanelHideDeltaX;
    }

    if ((tmap_frame->offset_x>=kPanelHideThresholdX)&&tmap){
      tmap_frame->offset_x-=kPanelHideDeltaX;
    }else if((tmap_frame->offset_x<=kPanelHideThresholdX)&&!tmap){
      tmap_frame->offset_x+=kPanelHideDeltaX;
    }

    for (i=0;i<=stormcloak_last2;i++){
      x=stormcloak_x2[i]; y=stormcloak_y2[i];
      z=ett*12; z%=14; if (z>7) z=14-z;
      ls2_p=(unsigned char*)&stormcloak[z][0]; z2=7;
      x7=(x-z2)*32; y7=(y-z2)*32; x8=(x+z2+1)*32; y8=(y+z2+1)*32; //screen rect
      x9=-x7; y9=-y7; //offset inside lsX array
      // RW-P2.3: route crop bounds and ls stride through viewport.h accessors.
      { const long bbW=backbufferW(), bbH=backbufferH();
        if (x7<0) x7=0; if (y7<0) y7=0; if (x8>bbW) x8=bbW; if (y8>bbH) y8=bbH; }
      const int stride = lightingStride();
      ls_off=y7*stride+x7; ls_off_add=stride-(x8-x7); //starting offset/add
      ls2_off=(y7+y9)*((z2*2+1)*32)+x7+x9; ls2_off_add=((z2*2+1)*32)-(x8-x7);
      for (y6=y7;y6<y8;y6++){ for (x6=x7;x6<x8;x6++){
        if (z=ls2_p[ls2_off]){
          z+=ls_off;
          if ((z/stride)!=(ls_off/stride)) z=(ls_off/stride*stride)+(stride-1);
          ps->o2[ls_off]=ps->o2[z];
        }//z
        ls_off++; ls2_off++;
      } ls_off+=ls_off_add; ls2_off+=ls2_off_add; } //x6, y6
    }//i


    //tremor
    if (tremor){

      i2=tremor*16;
      for (i=0;i<i2;i++){

        //equal weighting
        x=rnd*1280-255; y=rnd*1024-255;
        cls(surf_tremor,0);
        img(surf_tremor,-x,-y,ps);

        for (z=0;z<65536;z++){
          if (surf_tremorcirclemask->o2[z]){
            surf_tremor->o2[z]=0;
          }else{
            if (surf_tremor->o2[z]==0) surf_tremor->o2[z]=1;
          }
        }

        if (tremor==1){
          x2=rnd*3-1; y2=rnd*3-1;
        }
        if (tremor==2){
          x2=rnd*5-2; y2=rnd*5-2;
        }
        if (tremor==3){
          x2=rnd*7-3; y2=rnd*7-3;
        }

        x+=x2; y+=y2;
        img0(ps,x-1,y-1,surf_tremor);

      }//i
    }



    tremor=0;



    if (cloudloaded==0){ cloudloaded=1;
    for (i=0;i<=3;i++){
      if (i==0) cloudimg[i][1]=loadimage(".\\dr\\cld4.bmp",SURF_SYSMEM16);
      if (i==1) cloudimg[i][1]=loadimage(".\\dr\\cld5.bmp",SURF_SYSMEM16);
      if (i==2) cloudimg[i][1]=loadimage(".\\dr\\cld6.bmp",SURF_SYSMEM16);
      if (i==3) cloudimg[i][1]=loadimage(".\\dr\\cld7.bmp",SURF_SYSMEM16);
      x=cloudimg[i][1]->d.dwWidth; y=cloudimg[i][1]->d.dwHeight;
      cloudimg[i][0]=newsurf(x,y,SURF_SYSMEM16);
      cloudimg[i][2]=newsurf(x,y,SURF_SYSMEM16);
      cloudimg[i][3]=newsurf(x,y,SURF_SYSMEM16);
      for (y2=0;y2<y;y2++){
        for (x2=0;x2<x;x2++){
          i2=cloudimg[i][1]->o2[y2*x+x2];
          if ((i2&31)<=8) i2=0;
          cloudimg[i][1]->o2[y2*x+x2]=i2;
          cloudimg[i][0]->o2[y2*x+x2]=0; if (i2) cloudimg[i][0]->o2[y2*x+x2]=1<<5;//shadow
          cloudimg[i][2]->o2[y2*x+x2]=i2; if ((i2&31)<15) cloudimg[i][2]->o2[y2*x+x2]=0;
          cloudimg[i][3]->o2[y2*x+x2]=i2; if ((i2&31)<24) cloudimg[i][3]->o2[y2*x+x2]=0;
        }}
    }//i
    }

    //remove out of range clouds
    i2=1024;//max distance before removal
    x=tpx*32+512; y=tpy*32+384;//screen centre
    for (i=0;i<=31;i++){ if (cloudactive[i]){
      x2=cloudx[i]+cloudimg[cloudtype[i]][0]->d.dwWidth/2; y2=cloudy[i]+cloudimg[cloudtype[i]][0]->d.dwHeight/2-cloudheight[i]/2;//approx. centre of cloud
      if ((abs(x2-x)>=i2)||(abs(y2-y)>=i2)){
        cloudactive[i]=FALSE;//remove cloud
      }//range>i2
    }}

    //count number of clouds
    i2=0;
    for (i=0;i<=31;i++){
      if (cloudactive[i]) i2++;
    }


    x2=(long)btime2%28;
    x2-=20;
    if (x2>=0){
      if (x2==0) cloudidealnum=2;
      if (x2==1) cloudidealnum=4;
      if (x2==2) cloudidealnum=6;
      if (x2==3) cloudidealnum=8;
      if (x2==4) cloudidealnum=6;
      if (x2==5) cloudidealnum=4;
      if (x2==6) cloudidealnum=2;
    }else{
      cloudidealnum=1;
    }

cloudadded:
    if (i2<cloudidealnum){

      for (i=0;i<=31;i++){
        if (cloudactive[i]==FALSE){ cloudactive[i]=TRUE;



        cloudtype[i]=rnd*4;

        cloudheight[i]=256+(rnd*256);

        i3=rnd*4;
        if (i3==0){
          cloudx[i]=long(x+rnd*(1024+cloudimg[cloudtype[i]][0]->d.dwWidth)-cloudimg[cloudtype[i]][0]->d.dwWidth-512);
          cloudy[i]=y-384-cloudimg[cloudtype[i]][0]->d.dwHeight;
        }
        if (i3==1){
          cloudx[i]=long(x+rnd*(1024+cloudimg[cloudtype[i]][0]->d.dwWidth)-cloudimg[cloudtype[i]][0]->d.dwWidth-512);
          cloudy[i]=y+384+cloudheight[i];
        }
        if (i3==2){
          cloudx[i]=x-512-cloudimg[cloudtype[i]][0]->d.dwWidth;
          cloudy[i]=long(y+rnd*(768+cloudimg[cloudtype[i]][0]->d.dwHeight+cloudheight[i])-384-cloudimg[cloudtype[i]][0]->d.dwHeight);
        }
        if (i3==3){
          cloudx[i]=x+512;
          cloudy[i]=long(y+rnd*(768+cloudimg[cloudtype[i]][0]->d.dwHeight+cloudheight[i])-384-cloudimg[cloudtype[i]][0]->d.dwHeight);
        }

        if (firstclouds){
          cloudx[i]=long(x+rnd*(1024+cloudimg[cloudtype[i]][0]->d.dwWidth)-cloudimg[cloudtype[i]][0]->d.dwWidth-512);
          cloudy[i]=long(y+rnd*(768+cloudimg[cloudtype[i]][0]->d.dwHeight+cloudheight[i])-384-cloudimg[cloudtype[i]][0]->d.dwHeight);
        }



















        i2++;
        goto cloudadded;
        }
      }
    }
    firstclouds=0;

    if (noclouds==FALSE){
      x=tplay->x; y=tplay->y;
      if ((x>0)&&(y>0)&&(x<1024)&&(y<1024)){

        if ((bt[y][x]&1023)<192){

          for (i=0;i<=31;i++){
            if (cloudactive[i]){
              if (timelval!=15){
                imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32,cloudimg[cloudtype[i]][0]);
              }
            }
          }


          for (i2=256;i2<=(256+256);i2++){
            for (i=0;i<=31;i++){
              if (cloudheight[i]==i2){
                if (cloudactive[i]){
                  imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32-cloudheight[i],cloudimg[cloudtype[i]][1]);
                  imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32-cloudheight[i],cloudimg[cloudtype[i]][2]);
                  if (timelval!=15){
                    imgt0(ps,cloudx[i]-tpx*32,cloudy[i]-tpy*32-cloudheight[i],cloudimg[cloudtype[i]][3]);
                  }
                }
              }
            }
          }

        }else{//outside
          firstclouds=1;
        }//inside

      }//x,y on surface check
    }//noclouds

    for (i=0;i<=31;i++){
      if (cloudactive[i]){


        x=(cloudheight[i]-256)/64;
        cloudx[i]+=((float)windx2*(1+x));
        cloudy[i]+=((float)windy2*(1+x));
      }
    }

    if (x5=timelval){
      i=(unsigned long)ls; // RW-P2.1: ls is now a heap pointer.
      i2=(unsigned long)ps->o;
      i3=(unsigned long)lval;
      // RW-P2.3: load total pixel count from viewport.h accessor instead of
      // hard-coding 786432 (=1024*768) into the inline asm immediate.
      unsigned long _lsTotal = (unsigned long)lightingTotalBytes();
      _asm{
        ;preserve registers
          push ebx
          push esi
          push edi
          ;init registers
          mov esi,i ;z
          mov edi,i2
          mov edx,i3
          mov ecx,x5
          push ebp
          mov ebp,_lsTotal
          ;main loop
asm_lightshow0:
        mov al,[esi]
        mov ebx,ecx
          cmp bl,al
          ja asm_lightshow1
          xor ebx,ebx
          jmp asm_lightshow2
asm_lightshow1:
        sub bl,al
          shl ebx,16
asm_lightshow2:
        mov bx,[edi] ;screen value
          shl ebx,1
          add ebx,edx
          mov ax,[ebx]
        mov [edi],ax
          inc esi
          add edi,2
          dec ebp
          jnz asm_lightshow0
          pop ebp
          pop edi
          pop esi
          pop ebx
      }
    }

    if (britlens){


      img0(ps,(920-tpx)*32,(851-tpy)*32,blr[keyframe>>1]);


    }

    if (garglens){
      img0(ps,(924-tpx)*32-2,(851-tpy)*32,glr[keyframe>>1]);
    }


    //display onscreen text
    if (osn!=-1){
      for (z=0;z<=osn;z++){

        if (osi[z]>=1024){
          z2=osi[z]-1024;

          if (z2==16){
            txtset(t,"typing");
            x=ett*2.0f; x&=3;
            if (x==1) txtadd(t,".");
            if (x==2) txtadd(t,"..");
            if (x==3) txtadd(t,"...");
            x=osx[z];
            y=osy[z]+16;
            txtfnt=fnt3;
            goto osdisplay_ktar;
          }

          if (z2==17){
            txtset(t,"speaking");
            x=ett*2.0f; x&=3;
            if (x==1) txtadd(t,".");
            if (x==2) txtadd(t,"..");
            if (x==3) txtadd(t,"...");
            x=osx[z];
            y=osy[z]+16;
            txtfnt=fnt3;
            goto osdisplay_ktar;
          }


          if ((userkey!=3)&&(userkey!=4)) goto osdisplay_ktar_skip;
          z2++;
          if (z2==10) z2=0;
          x=osx[z];
          y=osy[z]-8;
          if ((timelval-ls[x+(y+8+16)*lightingStride()])==15) goto osdisplay_ktar_skip;
          txtnumint(t,z2);
          txtfnt=fnt2;
          goto osdisplay_ktar;
        }

        z2=oshpmp[z];

        if (z2&128) img(statusbar,0,0,statusbar_g128); else img(statusbar,0,0,statusbar_r128);
        img(statusbar,0,5,statusbar_b128);
        if (z2&15){
          if (z2&128) img(statusbar,-32+2+(z2&15)*2,0,statusbar_g255); else img(statusbar,-32+2+(z2&15)*2,0,statusbar_r255);
        }
        if ((z2>>4)&7) img(statusbar,-32+4+((z2>>4)&7)*4,5,statusbar_b255);
        img(ps,osx[z]-16,osy[z]-9,statusbar);



        if (voicechat_recording){
          if (osvol[z]!=-1){
            if (x2=osvol[z]){
              x=osx[z]-45; y=osy[z]+32;
              img0(ps,x,y,vm_volumem);

              f=64.0f/255.0f*(float)x2;
              img(ps,x+17+f,y+1,vm_voltab2m);



            }else{
              x=osx[z]-8; y=osy[z]+32;
              img0(ps,x,y,vm_volmmute);
            }



          }
        }


        x=osx[z];
        y=osy[z]-8;
        txtset(t,idlst_name[osi[z]]);
        txtfnt=fnt3;
        txtcol=idlst_namecolour[osi[z]];
osdisplay_ktar:
        tagxy.cx=0; tagxy.cy=0;
        ps->s->GetDC(&taghdc);
        {
            HGDIOBJ _old = SelectObject(taghdc, txtfnt);
            GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
            SelectObject(taghdc, _old);
        }
        ps->s->ReleaseDC(taghdc);
        x-=tagxy.cx/2;
        x2=txtcol;
        txtcol=rgb(0,0,0);
        txtout(ps,x+1,y+1-1,t);
        txtout(ps,x+1-1,y+1,t);
        txtout(ps,x+1+1,y+1,t);
        txtout(ps,x+1,y+1+1,t);
        txtcol=x2;
        txtout(ps,x+1,y+1,t);
        txtfnt=fnt1;
        txtcol=rgb(255,255,255);
osdisplay_ktar_skip:;
      }
    }

    //voice chat: volume meters of dead players (if any)
    if (voicechat_recording){
      for (z=1;z<=voicechat_listeningplayeri;z++){
        x=(voicechat_listeningplayerx[z]-tpx)*32+16; y=(voicechat_listeningplayery[z]-tpy)*32;
        if (x2=voicechat_listeningplayervolume[z]){
          x=x-45; y=y+32;
          img0(ps,x,y,vm_volumem);
          f=64.0f/255.0f*(float)x2;
          img(ps,x+17+f,y+1,vm_voltab2m);
        }else{
          x=x-8; y=y+32;
          img0(ps,x,y,vm_volmmute);
        }//x2
      }//z
    }//voicechat_recording

    //update local SF
    stolenitemwarningn=-1;//reset number of stolen item warnings onscreen
    for(i3=0;i3<=255;i3++){
      if (sfx[i3].type!=0){
        i4=sfx[i3].type;

        if (i4>=256){//time delayed effect!
          f=0.0625f; if (int((ett/f))!=int((ett/f)-(et/f))){//dec every 1/16th of a sec
            i5=(i4>>8)-1;
            sfx[i3].type=(i4&255)+(i5<<8);
          }//dec
          goto donesf2;
        }//time delayed effect!

        if (i4==1){ //physical attack
		  // s222 check attack target
		  if (combatinfo) {
			  hittarget = 10; // "something" is hit

			  // s222 player is hit
			  if ((tplay->x == sfx[i3].x) && (tplay->y == sfx[i3].y))
				  hittarget = 1;
		  }

          img(bt32,-32,0,sfx8);

          if ((sfx[i3].wait<=0.04f)||(sfx[i3].wait>=0.85f)){
            imgt0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }else{
            img0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }


          //number above?
          if (sfx[i3].x2){
            txtfnt=fnt1;
            txtnumint(t,sfx[i3].x2);
            x=(sfx[i3].x-tpx)*32;
            if (t->l==1) x+=10;
            if (t->l==2) x+=5;
            y=(sfx[i3].y-tpy)*32-16+sfx[i3].wait*16.0f;
            txtcol=rgb(0,0,0); txtout(ps,x,y,t); txtout(ps,x+2,y+2,t); txtout(ps,x+2,y,t); txtout(ps,x,y+2,t);
            txtcol=rgb(255,0,0); txtout(ps,x+1,y+1,t);
          }
		  // s333 set "some" damage if no number (for hitting webs, etc?)
		  else {
			  if (combatinfo) {
				  if (hittarget != 1)
					hittarget = 20;
				  //hittarget = 0; // suppress
				  txtset(t, "some (?)");
			  }
		  }


          if (sfx[i3].more==1){
            sfx[i3].more=0;
            //soundplay2(u6osound[0],u6osound_volume[0]);

			// s333 combat info for hits
			if (combatinfo && (hittarget > 0)) {
				//txtnumint(t, sfx[i3].x2); // t is set above at: if (sfx[i3].x2){
				txtsetchar(t3, 255); // make combat info status messages skippable
				objremovedn1 = 0;
				sfxonscreenn1 = 0;

				if (hittarget == 1) {
					txtadd(t3, "P: ");
					txtadd(t3, tnpc->name);
					sfxonscreenn1 = 1;
				} else {
					// something was hit
					if ((sfx[i3].x >= tpx - 1) && (sfx[i3].x <= tpx - 1 + 33) && (sfx[i3].y >= tpy - 1) && (sfx[i3].y <= tpy - 1 + 25)) {
						// sfx is on-screen
						sfxonscreenn1 = 1;

						// something was hit, attempt to get object type of the something
						objtypen1 = getobjtypen1(tplayer, sfx[i3].x, sfx[i3].y);

						if (objtypen1 == -1) {
							if (sfx[i3].x2) { // has damage number
								// a creature was hit here but nothing is here --> must have died. check backup to guess creature
								objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 0); //0

								if (resultinfon1 == 2)
									objremovedn1 = 4;
								else
									objremovedn1 = 1;
							} else {
								// an item (or maybe a creature) was hit here but can't get objtype --> maybe destroyed? check backup to guess/find item (chest, web, etc.)
								objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 1);
								if (objtypen1 != -1)
									objremovedn1 = 2;
							}
						} else {
							// something was hit and object (type) was found. attempt to convert it from a <dead body> to the corresponding (alive) creature, if possible
							objremovedn1 = convertdeadobjtypen1(objtypen1);

							if (objremovedn1 == -2) {
								// convert success: a dead body (of something) --> check backup to guess creature
								objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 0); //0

								if (resultinfon1 == 2)
									objremovedn1 = 4;
								else
									objremovedn1 = 1;
							} else if (objremovedn1 > 0) {
								// convert success: a dead <something> --> is converted to the corresponding (alive) creature
								objtypen1 = objremovedn1;
								objremovedn1 = 3;
							} else {
								// convert fail: not a <dead body> --> check is it a creature
								objremovedn1 = checkobjtypecreaturen1(objtypen1);

								if (objremovedn1 != 1) {
									// not a dead body and not a creature --> some item (or loot) is/was on the ground/spot
									if (sfx[i3].x2) { // has damage number
										// a creature was hit here but only item is on the spot --> creature is missing --> creature must have died --> check backup to guess creature
										objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 0); //0

										if (resultinfon1 == 2)
											objremovedn1 = 4;
										else
											objremovedn1 = 1;
									} else {
										if (objremovedn1 == 2) {
											// a chest/web is found --> item not destroyed
											objremovedn1 = 0;
										} else {
											// something (not a creature) was hit here but only item is on the spot --> something is destroyed --> check backup to guess something (chest, web, etc.)
											objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 1);
											objremovedn1 = 1;
											//objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 1);
											//if (objtypen1 != -1)
											//	objremovedn1 = 2;
										}
									}
								} else
									// a creature was hit and is still alive (not dead)
									objremovedn1 = 0;
							}

						}
					} else {
						// sfx is not on-screen --> check backup to guess creature
						objtypen1 = getobjtypen1b(combatinfoplayerprev, sfx[i3].x, sfx[i3].y, 1); //0
					}

					if (hittarget >= 20) {
						txtadd(t3, "O: ");
						//txtadd(t3, "Object");
					}
					else {
						txtadd(t3, "C: ");
						//txtadd(t3, "Creature (or another player)");
					}

					if (objtypen1 == -1) {
						txtset(t4, "Something (?)");
					} else {
						if (objremovedn1 && (objremovedn1 != 3)) {
							getobjdescn1(t5, objtypen1);
							//txtset(t4, "(");
							//txtadd(t4, t5);
							txtset(t4, t5);
							//txtadd(t4, "?)");
							if (objremovedn1 == 4)
								txtadd(t4, " (??)");
							else
								txtadd(t4, " (?)");
						} else {
							getobjdescn1(t4, objtypen1);
							if (!sfxonscreenn1)
								txtadd(t4, " (?*)");
						}
					}

					txtadd(t3, t4);
				}


				txtadd(t3, " is hit for ");
				txtadd(t3, t);
				txtadd(t3, " damage");

				if (objremovedn1) {
					if (hittarget >= 20) {
						if (objremovedn1 == 2)
							txtadd(t3, " (and is destroyed?)");
						else
							txtadd(t3, " and is destroyed!");
					} else if (hittarget >= 10) {
						if (objremovedn1 == 2)
							txtadd(t3, " (and is killed?)");
						else
							txtadd(t3, " and is killed!");
					}
				} else
					txtadd(t3, ".");

				STATUSMESSadd(t3);
				//if (!objremovedn1)
				//if ((sfx[i3].x >= tpx - 1) && (sfx[i3].x <= tpx - 1 + 33) && (sfx[i3].y >= tpy - 1) && (sfx[i3].y <= tpy - 1 + 25)) {
				if (sfxonscreenn1) {
					if (objremovedn1) {
						if (ett - combatinfoplayerprevett > 2.0) {
							combatinfoplayerprevett = ett;
							backupplayermvinfon1(tplayer);
						}
						else
							combatinfoplayerprevett = ett;
					}
					else if (ett - combatinfoplayerprevett > 0.5) {
						combatinfoplayerprevett = ett;
						backupplayermvinfon1(tplayer);
					}
				}

				/*
				txtset(t3, "-- movers: ");
				for (y = 0; y <= 25; y++) {
					for (x = 0; x <= 33; x++) {
						mapx = tpx + x - 1;
						mapy = tpy + y - 1;
						for (i = 0; i < tplayer->mv_i; i++) {
							if (tplayer->mv_x[i] == mapx) {
								if (tplayer->mv_y[i] == mapy) {
									x2 = x - 1; y2 = y - 1;
									x3 = objgettype(tplayer->mv_type[i], tplayer->mv_dir[i], tplayer->mv_frame[i]);
									myobj->type = x3;

									//non-pass specific changes
									//if (tplayer->mv_flags[i] & MV_PARALYZE) keyframe = 0;
									//check if sleeping
									//if (tplayer->mv_flags[i] & MV_SLEEP) {
										z3 = myobj->type & 1023; z2 = 0;
									//	if (z3 == 376) z2 = 339 + 6 * 1024;
									//}
									txtset(t3, "[");
									txtnumint(t, tplayer->mv_type[i]);
									txtadd(t3, t);
									txtadd(t3, "] ");
									txtnumint(t, z3);
									txtadd(t3, t);
									txtadd(t3, ": ");
									txtnumint(t, x);
									txtadd(t3, t);
									txtadd(t3, ", ");
									txtnumint(t, y);
									txtadd(t3, t);
									txtadd(t3, " // ");
									txtnumint(t, mapx);
									txtadd(t3, t);
									txtadd(t3, ", ");
									txtnumint(t, mapy);
									txtadd(t3, t);
									txtadd(t3, " // ");
									txtnumint(t, sfx[i3].x);
									txtadd(t3, t);
									txtadd(t3, ", ");
									txtnumint(t, sfx[i3].y);
									txtadd(t3, t);
									txtadd(t3, " // ");
									LOGadd(t3);
								}
							}
						}
					}
				}

				//LOGadd(t3);
				*/
			}

			// combat sound for player getting hit
			soundplay2(u6osound[SOUND_COMBAT_HIT], u6osound_volume[SOUND_COMBAT_HIT]);
          }
          goto donesf;
        }//1

        if (i4==16){ //physical attack miss!
		  // s222 check attack target
		  if (combatinfo) {
			  hittarget = 10; // "something" is hit/miss

			  // s222 player is hit
			  if ((tplay->x == sfx[i3].x) && (tplay->y == sfx[i3].y))
				  hittarget = 1;
		  }

          img(bt32,-32*14,-32*3,sfx8);

          if ((sfx[i3].wait<=0.04f)||(sfx[i3].wait>=0.85f)){
            imgt0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }else{
            img0(ps,(sfx[i3].x-tpx)*32,(sfx[i3].y-tpy)*32,bt32);
          }


		  // s222 add "miss" text
		  if (enhancen1) {
			  txtfnt = fnt1;
			  //txtnumint(t,sfx[i3].x2);
			  txtset(t, "miss");
			  x = (sfx[i3].x - tpx) * 32;
			  //if (t->l==1) x+=10;
			  //if (t->l==2) x+=5;
			  y = (sfx[i3].y - tpy) * 32 - 16 + sfx[i3].wait*16.0f;
			  txtcol = rgb(0, 0, 0); txtout(ps, x, y, t); txtout(ps, x + 2, y + 2, t); txtout(ps, x + 2, y, t); txtout(ps, x, y + 2, t);
			  txtcol = rgb(255, 0, 0); txtout(ps, x + 1, y + 1, t);
		  }


		  if (combatinfo && (hittarget > 0)) {
			  if ((sfx[i3].x >= tpx - 1) && (sfx[i3].x <= tpx - 1 + 33) && (sfx[i3].y >= tpy - 1) && (sfx[i3].y <= tpy - 1 + 25)) {
				  // sfx is on-screen
				  if (ett - combatinfoplayerprevett > 0.5) {
					  combatinfoplayerprevett = ett;
					  backupplayermvinfon1(tplayer);
				  }
			  }
		  }
		  /*
		  txtset(t3, "loc ");
		  txtnumint(t4, tplay->x);
		  txtadd(t3, t4);
		  txtadd(t3, ",");
		  txtnumint(t4, tplay->y);
		  txtadd(t3, t4);
		  LOGadd(t3);

		  txtset(t3, "sfx ");
		  txtnumint(t4, sfx[i3].x);
		  txtadd(t3, t4);
		  txtadd(t3, ",");
		  txtnumint(t4, sfx[i3].y);
		  txtadd(t3, t4);
		  LOGadd(t3);
		  */


          if (sfx[i3].more==1){
            sfx[i3].more=0;
            //soundplay2(u6osound[0],u6osound_volume[0]);

			// s333 combat info for "miss"
			/*
			if (combatinfo) {
				if (hittarget == 1) {
					txtset(t3, "Creature misses ");
					txtadd(t3, tnpc->name);
				} else {
					txtset(t3, "An attack on creature misses");
				}

				STATUSMESSadd(t3);
			}
			*/

			// combat sound for "miss"
			soundplay2(u6osound[SOUND_COMBAT_HIT], u6osound_volume[SOUND_COMBAT_HIT]);
          }
          goto donesf;
        }//1


        if (i4==5) goto pw_jmp;
        if (i4==6) goto pw_jmp;
        if (i4==7) goto pw_jmp;
        if (i4==8) goto pw_jmp;
        if (i4==9) goto pw_jmp;
        if (i4==10) goto pw_jmp;
        if (i4==11) goto pw_jmp;
        if (i4==12) goto pw_jmp;
        if (i4==13) goto pw_jmp;
        if (i4==14) goto pw_jmp; //blue ball
        if (i4==15) goto pw_jmp; //kill
        if (i4==17) goto pw_jmp; //cannon ball
        if (i4==18) goto pw_jmp; //green ball
        if (i4==19) goto pw_jmp; //blue bolt
        if (i4==20) goto pw_jmp; //pink ball
        if (i4==28) goto pw_jmp; //red bolt
        if (i4==29) goto pw_jmp; //red ball

        if (i4==2){ //arrow
pw_jmp:


          f3=sqrt((float)(abs(sfx[i3].x2-sfx[i3].x)*abs(sfx[i3].x2-sfx[i3].x)+abs(sfx[i3].y2-sfx[i3].y)*abs(sfx[i3].y2-sfx[i3].y)))/24.0f;
          if (sfx[i3].more==0xFFFF){ //setup


            sfx[i3].more=0; //index type *up arrow
            if (i4==9){
              //if ((unsigned long)sfx[i3].wait&32768) {soundplay2(u6osound[18],u6osound_volume[18]); sfx[i3].wait-=32768;}
              if ((unsigned long)sfx[i3].wait&32768) {soundplay2(u6osound[18],u6osound_volume[18]); sfx[i3].wait-=32768;}
              sfx[i3].more=sfx[i3].wait; sfx[i3].wait=1;
            }



            f=abs(sfx[i3].x2-sfx[i3].x);
            f2=abs(sfx[i3].y2-sfx[i3].y);

            if (f||f2){
              if (f2>f){
                f4=2/f2;
                f=(sfx[i3].x2-sfx[i3].x); f2=(sfx[i3].y2-sfx[i3].y);
                if (f2<0) y=-2; if (f2>0) y=2;
                f=f*f4;
                x=0; if (f<-0.5) x=-1; if (f<-1.5) x=-2; if (f>0.5) x=1; if (f>1.5) x=2;
                if (i4!=9) sfx[i3].more=qkdir[x+2][y+2];
              }else{
                f4=2/f;
                f=(sfx[i3].x2-sfx[i3].x); f2=(sfx[i3].y2-sfx[i3].y);
                if (f<0) x=-2; if (f>0) x=2;
                f2=f2*f4;
                y=0; if (f2<-0.5) y=-1; if (f2<-1.5) y=-2; if (f2>0.5) y=1; if (f2>1.5) y=2;
                if (i4!=9) sfx[i3].more=qkdir[x+2][y+2];
              }
            }else{ //both f and f2 ==0
              if (i4!=9) sfx[i3].more=qkdir[2][0]; //up!
            }

            sfx[i3].wait=f3-(0.75f/24.0f);
            if (i4==7) {sfx[i3].wait=(f3*2)-(0.75f/24.0f); soundplay2(u6osound[10],u6osound_volume[10]);}//*boomerang
            if (sfx[i3].wait<0) sfx[i3].wait=0;
			// s222 new sound for bow
            //if (i4==2) soundplay2(u6osound[7],u6osound_volume[7]);
			if (i4 == 2) {
				soundplay2(u6osound[SOUND_COMBAT_BOW], u6osound_volume[SOUND_COMBAT_BOW]);
			}

			// s222 new sound for crossbow
            //if (i4==6) soundplay2(u6osound[8],u6osound_volume[8]);
			if (i4 == 6) {
				soundplay2(u6osound[SOUND_COMBAT_CROSSBOW], u6osound_volume[SOUND_COMBAT_CROSSBOW]);
			}

            if (i4==17) soundplay2(u6osound[14],u6osound_volume[14]);

			// s222 new sound for fireball
            //if (i4==8) soundplay2(u6osound[20],u6osound_volume[20]);
			if (i4 == 8) {
				soundplay2(u6osound[SOUND_COMBAT_FIREBALL], u6osound_volume[SOUND_COMBAT_FIREBALL]);
			}


          }//first instance only!

          if (f3){
            f=(sfx[i3].x2-sfx[i3].x)*32;
            f=f/f3*(f3-sfx[i3].wait); //move per second
            f2=(sfx[i3].y2-sfx[i3].y)*32;
            f2=f2/f3*(f3-sfx[i3].wait); //move per second
            x=f;
            y=f2;
          }else{
            x=(sfx[i3].x2-sfx[i3].x)*32; y=(sfx[i3].y2-sfx[i3].y)*32;
          }


          if (i4==7){ //*boomerang
            if (sfx[i3].wait>f3){
              f=(sfx[i3].x2-sfx[i3].x)*32;
              f=f/f3*(f3-(sfx[i3].wait-f3)); //move per second
              f2=(sfx[i3].y2-sfx[i3].y)*32;
              f2=f2/f3*(f3-(sfx[i3].wait-f3)); //move per second
              x=f;
              y=f2;
            }else{
              f=(sfx[i3].x2-sfx[i3].x)*32;
              f=f/f3*(f3-(f3-sfx[i3].wait)); //move per second
              f2=(sfx[i3].y2-sfx[i3].y)*32;
              f2=f2/f3*(f3-(f3-sfx[i3].wait)); //move per second
              x=f;
              y=f2;
              if (sfx[i3].wait<=(0.75f/24.0f)) {sfx[i3].wait=0; goto donesf;} //finish and don't draw
            }
            img(bt32,keyframe2*-32,-96,u6ob);
          }//boomerang


          if (i4==2) img(bt32,sfx[i3].more*-32,-32,u6ob);
          if (i4==5) img(bt32,sfx[i3].more*-32,0,u6ob);
          if (i4==6) img(bt32,sfx[i3].more*-32,-64,u6ob); //bolt
          if (i4==8) img(bt32,sfx[i3].more*-32,-128,u6ob); //fire wand
          if (i4==9){
            static object *GENERICthrow_obj=NULL;
            if (GENERICthrow_obj==NULL) GENERICthrow_obj=OBJnew_local();
            GENERICthrow_obj->type=sfx[i3].more;
            getspr(GENERICthrow_obj);
          }
          if (i4==10) img(bt32,keyframe2*-32-256,-96,u6ob); //dagger
          if (i4==11) img(bt32,keyframe2*-32,-160,u6ob); //axe
          if (i4==12) img(bt32,sfx[i3].more*-32,-192,u6ob); //spear
          if (i4==13) img(bt32,-8*32,0,sfx8); //rock! sling

          if (i4==14) img(bt32,(keyframe2&3)*-32-256,-160,u6ob); //blue ball!
          if (i4==15) img(bt32,sfx[i3].more*-32,-224,u6ob); //kill
          if (i4==17) img(bt32,-9*32,0,sfx8); //cannon
          if (i4==18) img(bt32,(keyframe2&3)*-32-256-128,-160,u6ob); //green ball!
          if (i4==19) img(bt32,sfx[i3].more*-32,-256,u6ob); //blue bolt
          if (i4==20) img(bt32,(keyframe2&3)*-32,-288,u6ob); //pink ball!
          if (i4==28) img(bt32,sfx[i3].more*-32,-320,u6ob); //red bolt
          if (i4==29) img(bt32,(keyframe2&3)*-32-128,-288,u6ob); //red ball!

          if (i4==8){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==14){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==18){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==19){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==20){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==5){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==28){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}
          if (i4==29){img75t0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;}


          img0(ps,(sfx[i3].x-tpx)*32+x,(sfx[i3].y-tpy)*32+y,bt32); goto donesf;
        }//2,...


        if (i4==3){ //global portrait message
          txtset(t,(txt*)sfx[i3].p);
          txtset(t2,"?"); t2->d2[0]=92;
          z=txtsearch(t,t2);

          if (sfx[i3].wait==0){ //send message to message log and remove


            txtset(t3,t);

            txtset(t,"P");//standard portrait
            txtnumint(t2,sfx[i3].more); txtadd(t,t2);//portrait number
            txtset(t2,"/"); txtadd(t,t2);




            txtset(t4,t3);
            if (z){
              txtleft(t4,z-1);
            }

            if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)){
              txtset(t5,"?"); t5->ds[0]=4; txtadd(t,t5);
            }

            if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)){
              txtset(t5,"?"); t5->ds[0]=5; txtadd(t,t5);
            }


            //fetch colourname if available
            txtset(t6,"");
            if (t4->d2[0]==6){
              txtset(t6,"6CCCN...");
              t6->d2[0]=6;
              t6->d2[1]=t4->d2[1];
              t6->d2[2]=t4->d2[2];
              t6->d2[3]=t4->d2[3];
              t6->d2[4]=3;
            }
            static unsigned char wraptext_firstline;
            wraptext_firstline=1;
            //check if data exceeds 1 line inc port! 16 across
wraptext_recheck:
            ps->s->GetDC(&taghdc);
            {
                HGDIOBJ _old = SelectObject(taghdc, fnt1); // not valid for garg text or runes!
                tagxy.cx = 0; tagxy.cy = 0; GetTextExtentPoint32(taghdc, t4->d, t4->l, &tagxy); // get width of t4
                SelectObject(taghdc, _old);
            }
            ps->s->ReleaseDC(taghdc);
            if (t6->l&&wraptext_firstline) tagxy.cx-=64;
            if ((64+tagxy.cx)>=1024){//original string is more than 1024 chars
              txtset(t7,"");//clear t7


              //move t4 string into t7 until it fits exactly
movet4intot7:
              txtaddchar(t7,t4->d2[0]); txtright(t4,t4->l-1);

              ps->s->GetDC(&taghdc);
              {
                  HGDIOBJ _old = SelectObject(taghdc, fnt1); // not valid for garg text or runes!
                  tagxy.cx = 0; tagxy.cy = 0; GetTextExtentPoint32(taghdc, t7->d, t7->l, &tagxy);
                  SelectObject(taghdc, _old);
              }
              ps->s->ReleaseDC(taghdc);
              if (t6->l&&wraptext_firstline) tagxy.cx-=64;
              if ((64+tagxy.cx)<1024) goto movet4intot7;


              if (wraptext_firstline){ wraptext_firstline=0;
              txtset(t8,t);  txtadd(t8,t7); txtfileout(t8,messagelog);

              }else{
                txtset(t8,t); txtadd(t8,t6); txtadd(t8,t7); txtfileout(t8,messagelog);
              }

              goto wraptext_recheck;


            }
            txtfnt=fnt1;
            if (wraptext_firstline==FALSE) txtadd(t,t6);

            txtadd(t,t4);


            txtfileout(t,messagelog);




            if (autoscroll==0.0f) textdisplayi=0xFFFFFFF; //position list at last index!


            if (z){
              txtright(t3,t3->l-z);
              txtset((txt*)sfx[i3].p,t3);

              txtset(t4,"?"); t4->d2[0]=92;
              z=txtsearch(t3,t4);
              if (z==0) z=t->l; else z--;
              f=(float)z/10.0f;
              if (f<1)f=1;
              if(f>4)f=4;
              sfx[i3].wait=f;
            }else{
              //last item
              free((txt*)sfx[i3].p); //free memory
            }





updatetextdisplay:

            //reconfigure mess_frm to display most recent items
            //1. seek pos 0 in message log
            //2. scan till the end moving each txt* index up 1 till finished
            x5=-1;

            for (x4=0;x4<=7;x4++){
              txtset(con_txt[x4],"");
            }

            seek(messagelog,0);
            x2=lof(messagelog);
seekmore:
            txtfilein(t,messagelog);
            x5++;
            x3=seek(messagelog);

            for (x4=6;x4>=0;x4--){
              txtset(con_txt[x4+1],con_txt[x4]);
            }
            txtset(con_txt[0],t);


            if ((x3<x2)&&(x5<textdisplayi)) goto seekmore;
            if (x3>=x2) {textdisplayi=x5; autoscroll=0.0f;}


            seek(messagelog,x2);

            cls(con_frm_img->graphic,0);
            for (x4=0;x4<=7;x4++){

              txtset(t,con_txt[x4]);
              if (t->l){

                txtset(t2,"/");
                if (t->d2[0]==80){//"P"
                  txtright(t,t->l-1); x3=txtsearch(t,t2);
                  z4=txtnum(t); txtright(t,t->l-x3);
                  img0(con_frm_img->graphic,0,256-32-32*x4,getportrait_halfsize(z4));
                }


                //rune font?
                txtfnt=fnt1;
                if (t->d2[0]==3){
                  txtright(t,t->l-1);
                  txtfnt=fnt5;
                  //edit special characters!
runeedit2:
                  for (x5=0;x5<t->l;x5++){
                    if ((t->d2[x5]==84)&&(t->d2[x5+1]==72)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C4"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//TH
                    if ((t->d2[x5]==69)&&(t->d2[x5+1]==69)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C1"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//EE
                    if ((t->d2[x5]==78)&&(t->d2[x5+1]==71)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C2"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//NG
                    if ((t->d2[x5]==69)&&(t->d2[x5+1]==65)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C0"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//EA
                    if ((t->d2[x5]==83)&&(t->d2[x5+1]==84)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C3"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit2;}//ST
                  }//x5
                }//3

                if (t->d2[0]==1){
                  txtright(t,t->l-1);
                  txtfnt=fnt6;
                  //edit special characters!
gargedit2:
                  for (x5=0;x5<t->l;x5++){
                    if ((t->d2[x5]==99)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x043"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//ch->C
                    if ((t->d2[x5]==103)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x047"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//gl->G
                    if ((t->d2[x5]==104)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x048"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//hl->H
                    if ((t->d2[x5]==107)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04B"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//kl->K
                    if ((t->d2[x5]==110)&&(t->d2[x5+1]==103)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04E"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//ng->N
                    if ((t->d2[x5]==110)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04C"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//nl->L
                    if ((t->d2[x5]==110)&&(t->d2[x5+1]==121)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x059"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//ny->Y
                    if ((t->d2[x5]==115)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x053"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//sh->S
                    if ((t->d2[x5]==122)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x05A"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit2;}//zh->Z
                  }//x5
                }//1


                static unsigned char txtlog_gm;
                static unsigned char txtlog_pm;
                static unsigned char txtlog_vm;
                txtlog_pm=0; txtlog_gm=0; txtlog_vm=0;

                if (t->d2[0]==4){ //global message
                  txtlog_gm=1; txtright(t,t->l-1);
                }
                if (t->d2[0]==5){ //private message
                  txtlog_pm=1; txtright(t,t->l-1);
                }
                if (t->d2[t->l-1]==255){//voice message
                  txtlog_vm=1; txtleft(t,t->l-1);
                }


                //text
                x=56/2;
                y=256-32-x4*32+2;
                //display (red)text and port
text_continue_con:


                txtset(t2,"@"); z=txtsearch(t,t2);
                txtsetchar(t2,6); z2=txtsearch(t,t2);
                if (z2){
                  if (z){
                    if (z2<z) z=z2;
                  }else{
                    z=z2;
                  }
                }
                if (z){ txtset(t3,t); txtleft(t,z-1);}


                for (z3=0;z3<t->l;z3++){
                  if (t->d2[z3]==7) t->d2[z3]=64;
                  if (t->d2[z3]==8) t->d2[z3]=92;
                }

                z3=rgb(255,255,255);
                if (txtlog_gm) z3=rgb(96,128,255);//global message
                if (txtlog_pm) z3=rgb(255,64,128);//private message
                if (txtlog_vm) z3=rgb(64,224,64);//voice message
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                {
                    HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                    GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                    SelectObject(taghdc, _old);
                }
                ps->s->ReleaseDC(taghdc);


                lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1-1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1+1,t); txtout(con_frm_img->graphic,x+1-1,y+1+1,t);
                txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1,y+1-1,t); txtout(con_frm_img->graphic,x+1-1,y+1,t); txtout(con_frm_img->graphic,x+1+1,y+1,t); txtout(con_frm_img->graphic,x+1,y+1+1,t);
                txtfnt=lastfont;

                txtcol=z3;
                txtout(con_frm_img->graphic,x+1,y+1,t);
                x+=tagxy.cx;

                if (z){ //red

                  if (t3->d2[z-1]==64){//@ (red text)
                    txtright(t3,t3->l-z);
                    z=65536;
                    txtset(t2," "); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,","); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,"."); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,"?"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,")"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    txtset(t2,"!"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                    if (z==65536) z=0;
                    txtset(t,t3);
                    if (z) txtleft(t,z-1);


                    lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1-1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1+1,t); txtout(con_frm_img->graphic,x+1-1,y+1+1,t);
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1,y+1-1,t); txtout(con_frm_img->graphic,x+1-1,y+1,t); txtout(con_frm_img->graphic,x+1+1,y+1,t); txtout(con_frm_img->graphic,x+1,y+1+1,t);
                    txtfnt=lastfont;

                    txtcol=rgb(255,64,32); txtout(con_frm_img->graphic,x+1,y+1,t);

                    tagxy.cx=0; tagxy.cy=0;
                    ps->s->GetDC(&taghdc);
                    {
                        HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                        GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                        SelectObject(taghdc, _old);
                    }
                    ps->s->ReleaseDC(taghdc);
                    x+=tagxy.cx;
                    if (z){ txtright(t3,t3->l-z+1); txtset(t,t3); goto text_continue_con; }
                    goto txtconlog_done;
                  }

                  if (t3->d2[z-1]==6){//coloured name
                    z3=t3->d2[z];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    z3<<=3; if (z3==248) z3=255;
                    z2=z3;

                    z3=t3->d2[z+1];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    z3<<=3; if (z3==248) z3=255;
                    z2+=(z3<<8);

                    z3=t3->d2[z+2];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    z3<<=3; if (z3==248) z3=255;
                    z2+=(z3<<16);


                    z3=t3->d2[z+3];
                    if (z3>=97){
                      z3=z3-97+26+10;
                    }else{
                      if (z3>=65){
                        z3=z3-65+10;
                      }else{
                        if (z3>=48) z3=z3-48;
                      }
                    }
                    txtright(t3,t3->l-z-4);//remove header and other info
                    z=z3;

                    txtset(t,t3);
                    txtleft(t,z);


                    lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1-1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1-1,t); txtout(con_frm_img->graphic,x+1+1,y+1+1,t); txtout(con_frm_img->graphic,x+1-1,y+1+1,t);
                    txtcol=rgb(8,8,8); txtout(con_frm_img->graphic,x+1,y+1-1,t); txtout(con_frm_img->graphic,x+1-1,y+1,t); txtout(con_frm_img->graphic,x+1+1,y+1,t); txtout(con_frm_img->graphic,x+1,y+1+1,t);
                    txtfnt=lastfont;


                    txtcol=z2; txtout(con_frm_img->graphic,x+1,y+1,t);

                    tagxy.cx=0; tagxy.cy=0;
                    ps->s->GetDC(&taghdc);
                    {
                        HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                        GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                        SelectObject(taghdc, _old);
                    }
                    ps->s->ReleaseDC(taghdc);
                    x+=tagxy.cx;


                    if (t3->l>z){ txtright(t3,t3->l-z); txtset(t,t3); goto text_continue_con; }


                    goto txtconlog_done;
                  }






                }//z
txtconlog_done:;





              }
            }




            if (textdisplayupdate) goto updatetextdisplay2;


          }
          else{




            if (z){
              txtleft(t,z-1);
            }

            //rune font?
            txtfnt=fnt1;
            if (t->d2[0]==3){
              txtright(t,t->l-1);
              txtfnt=fnt5;
              //edit special characters!
runeedit:
              for (x5=0;x5<t->l;x5++){
                if ((t->d2[x5]==84)&&(t->d2[x5+1]==72)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C4"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//TH
                if ((t->d2[x5]==69)&&(t->d2[x5+1]==69)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C1"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//EE
                if ((t->d2[x5]==78)&&(t->d2[x5+1]==71)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C2"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//NG
                if ((t->d2[x5]==69)&&(t->d2[x5+1]==65)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C0"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//EA
                if ((t->d2[x5]==83)&&(t->d2[x5+1]==84)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x0C3"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto runeedit;}//ST
              }//x5
            }//3

            if (t->d2[0]==1){
              txtright(t,t->l-1);
              txtfnt=fnt6;
              //edit special characters!
gargedit:
              for (x5=0;x5<t->l;x5++){
                if ((t->d2[x5]==99)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x043"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//ch->C
                if ((t->d2[x5]==103)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x047"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//gl->G
                if ((t->d2[x5]==104)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x048"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//hl->H
                if ((t->d2[x5]==107)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04B"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//kl->K
                if ((t->d2[x5]==110)&&(t->d2[x5+1]==103)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04E"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//ng->N
                if ((t->d2[x5]==110)&&(t->d2[x5+1]==108)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x04C"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//nl->L
                if ((t->d2[x5]==110)&&(t->d2[x5+1]==121)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x059"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//ny->Y
                if ((t->d2[x5]==115)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x053"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//sh->S
                if ((t->d2[x5]==122)&&(t->d2[x5+1]==104)) {txtset(t2,t); txtleft(t2,x5); txtadd(t2,"\x05A"); txtright(t,t->l-x5-2); txtadd(t2,t); txtset(t,t2); goto gargedit;}//zh->Z
              }//x5
            }//1


            //display (red)text and port
            //make t2, remove "@"
            z3=0;
            for (z2=0;z2<t->l;z2++){
              if (t->d2[z2]!=64) z3++;
            }
            txtNEWLEN(t2,z3);
            z3=0;
            for (z2=0;z2<t->l;z2++){
              if (t->d2[z2]!=64) t2->d2[z3++]=t->d2[z2];
            }
            //get text dimentions (using t2)
            tagxy.cx=0; tagxy.cy=0;
            ps->s->GetDC(&taghdc);
            {
                HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                GetTextExtentPoint32(taghdc, t2->d, t2->l, &tagxy);
                SelectObject(taghdc, _old);
            }
            ps->s->ReleaseDC(taghdc);
            x=(sfx[i3].x-tpx)*32-16;
            y=(sfx[i3].y-tpy)*32-8;
            if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)){
              x=480-16;
              y=352-32*6-8;
            }

            if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)){
              x=480-16;
              y=352-32*4-8;
            }



            if ((x+56+tagxy.cx)>1024) x=1024-56-tagxy.cx; if (x<0) x=0;
            if (y<0) y=0; if (y>704) y=704;

            img0(ps,x,y,getportrait(sfx[i3].more));


            static unsigned char txt_gm;
            static unsigned char txt_pm;
            static unsigned char txt_vm;
            txt_pm=0; txt_gm=0; txt_vm=0;

            if ((sfx[i3].x==2047)&&(sfx[i3].y==1023)) txt_gm=1;//global message
            if ((sfx[i3].x==2048)&&(sfx[i3].y==1023)) txt_pm=1;//private message
            if (t->d2[t->l-1]==255){//voice message
              txt_vm=1; txtleft(t,t->l-1);
            }



            x=x+56; y=y+21;
text_continue:


            txtset(t2,"@"); z=txtsearch(t,t2);
            txtsetchar(t2,6); z2=txtsearch(t,t2);
            if (z2){
              if (z){
                if (z2<z) z=z2;
              }else{
                z=z2;
              }
            }
            if (z){ txtset(t3,t); txtleft(t,z-1);}

            for (z3=0;z3<t->l;z3++){
              if (t->d2[z3]==7) t->d2[z3]=64;
              if (t->d2[z3]==8) t->d2[z3]=92;
            }



            z3=rgb(255,255,255);
            if (txt_gm) z3=rgb(96,128,255);//global message
            if (txt_pm) z3=rgb(255,64,128);//private message
            if (txt_vm) z3=rgb(64,224,64);//voice message
            tagxy.cx=0; tagxy.cy=0;
            ps->s->GetDC(&taghdc);
            {
                HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                SelectObject(taghdc, _old);
            }
            ps->s->ReleaseDC(taghdc);




            lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
            txtcol=rgb(0,0,0); txtout(ps,x+1-1,y+1-1,t); txtout(ps,x+1+1,y+1-1,t); txtout(ps,x+1+1,y+1+1,t); txtout(ps,x+1-1,y+1+1,t);
            txtcol=rgb(0,0,0); txtout(ps,x+1,y+1-1,t); txtout(ps,x+1-1,y+1,t); txtout(ps,x+1+1,y+1,t); txtout(ps,x+1,y+1+1,t);
            txtfnt=lastfont;

            txtcol=z3;
            txtout(ps,x+1,y+1,t);
            x+=tagxy.cx;


            if (z){//special text

              if (t3->d2[z-1]==64){//@ (red text)
                txtright(t3,t3->l-z);
                z=65536;
                txtset(t2," "); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,","); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,"."); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,"?"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,")"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                txtset(t2,"!"); z2=txtsearch(t3,t2); if ((z2<z)&&(z2!=0)) z=z2;
                if (z==65536) z=0;
                txtset(t,t3);
                if (z) txtleft(t,z-1);

                lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                txtcol=rgb(0,0,0); txtout(ps,x+1-1,y+1-1,t); txtout(ps,x+1+1,y+1-1,t); txtout(ps,x+1+1,y+1+1,t); txtout(ps,x+1-1,y+1+1,t);
                txtcol=rgb(0,0,0); txtout(ps,x+1,y+1-1,t); txtout(ps,x+1-1,y+1,t); txtout(ps,x+1+1,y+1,t); txtout(ps,x+1,y+1+1,t);
                txtfnt=lastfont;


                txtcol=rgb(255,64,32); txtout(ps,x+1,y+1,t);
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                {
                    HGDIOBJ _old = SelectObject(taghdc, txtfnt);
                    GetTextExtentPoint32(taghdc, t->d, t->l, &tagxy);
                    SelectObject(taghdc, _old);
                }
                ps->s->ReleaseDC(taghdc);
                x+=tagxy.cx;
                if (z){ txtright(t3,t3->l-z+1); txtset(t,t3); goto text_continue; }
                goto txtsf_done;
              }

              if (t3->d2[z-1]==6){//coloured name

                z3=t3->d2[z];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                z3<<=3; if (z3==248) z3=255;
                z2=z3;

                z3=t3->d2[z+1];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                z3<<=3; if (z3==248) z3=255;
                z2+=(z3<<8);

                z3=t3->d2[z+2];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                z3<<=3; if (z3==248) z3=255;
                z2+=(z3<<16);


                z3=t3->d2[z+3];
                if (z3>=97){
                  z3=z3-97+26+10;
                }else{
                  if (z3>=65){
                    z3=z3-65+10;
                  }else{
                    if (z3>=48) z3=z3-48;
                  }
                }
                txtright(t3,t3->l-z-4);//remove header and other info
                z=z3;

                txtset(t,t3);
                txtleft(t,z);



                lastfont=txtfnt; if (lastfont==fnt1) txtfnt=fnt1naa;
                txtcol=rgb(0,0,0); txtout(ps,x+1-1,y+1-1,t); txtout(ps,x+1+1,y+1-1,t); txtout(ps,x+1+1,y+1+1,t); txtout(ps,x+1-1,y+1+1,t);
                txtcol=rgb(0,0,0); txtout(ps,x+1,y+1-1,t); txtout(ps,x+1-1,y+1,t); txtout(ps,x+1+1,y+1,t); txtout(ps,x+1,y+1+1,t);
                txtfnt=lastfont;


                txtcol=z2; txtout(ps,x+1,y+1,t);
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                SelectObject(taghdc,txtfnt);
                GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                ps->s->ReleaseDC(taghdc);
                x+=tagxy.cx;


                if (t3->l>z){ txtright(t3,t3->l-z); txtset(t,t3); goto text_continue; }


                goto txtsf_done;
              }


            }//z
          }//?

txtsf_done:
          txtfnt=fnt1;

          goto donesf;
        }



        if (i4==4){ //sound effect

          //unsigned short more; //distance bolt travels, ...


		  // s222 new sound for door open/close
          //if (sfx[i3].more==1) soundplay2(u6osound[1],u6osound_volume[1]);
          //if (sfx[i3].more==2) soundplay2(u6osound[2],u6osound_volume[2]);
          if (sfx[i3].more==1) {
			  soundplay2(u6osound[SOUND_DOOROPEN], u6osound_volume[SOUND_DOOROPEN]);
		  }
		  if (sfx[i3].more == 2) {
			  soundplay2(u6osound[SOUND_DOORCLOSE], u6osound_volume[SOUND_DOORCLOSE]);
		  }

          if (sfx[i3].more==3) soundplay2(u6osound[4],u6osound_volume[4]);
		  // s222 new sound for lever
          //if (sfx[i3].more==4) soundplay2(u6osound[6],u6osound_volume[6]);
		  if (sfx[i3].more == 4) {
			  soundplay2(u6osound[SOUND_LEVER], u6osound_volume[SOUND_LEVER]);
		  }

          if (sfx[i3].more==5) soundplay2(u6osound[19],u6osound_volume[19]);
          if (sfx[i3].more==6) soundplay2(u6osound[31],u6osound_volume[31]);
          if (sfx[i3].more==7) soundplay2(u6osound[32],u6osound_volume[32]);

          sfx[i3].type=0;




          goto donesf;
        }//4

        if (i4==21){//stolen item warning
          stolenitemwarningn++;
          stolenitemwarningx[stolenitemwarningn]=sfx[i3].x;
          stolenitemwarningy[stolenitemwarningn]=sfx[i3].y;
          stolenitemwarningtype[stolenitemwarningn]=sfx[i3].more;


          goto donesf;
        }//21

        if (i4==22){//multiple fireballs!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=8+x3;//fireball
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multiplefireballs_added;
            }}//type==0,i5
multiplefireballs_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//22

        if (i4==23){//multiple green balls!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=18+x3;//green ball
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multiplegreenballs_added;
            }}//type==0,i5
multiplegreenballs_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//23

        if (i4==24){//multiple lightning!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=5+x3;//lightning
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multiplelightning_added;
            }}//type==0,i5
multiplelightning_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//24

        if (i4==25){//multiple death bolts!
          for (x2=0;x2<=4;x2++){
            for (i5=0;i5<=255;i5++){ if (sfx[i5].type==0){
              x3=rnd*8+1;
              x3<<=8;
              if (x2==0) x3=0;
              sfx[i5].type=15+x3;//lightning
              sfx[i5].x=sfx[i3].x;
              sfx[i5].y=sfx[i3].y;
              sfx[i5].x2=sfx[i3].x2;
              sfx[i5].y2=sfx[i3].y2;
              sfx[i5].more=sfx[i3].more;
              sfx[i5].wait=sfx[i3].wait;
              goto multipledeathbolts_added;
            }}//type==0,i5
multipledeathbolts_added:;
          }//x2
          sfx[i3].type=0;
          goto donesf;
        }//25

        if (i4==26){//tremor
          if (sfx[i3].wait==1.0f) soundplay2(u6osound[35],u6osound_volume[35]);
          x2=1;
          if (sfx[i3].wait>=0.4f) x2=2;
          if (sfx[i3].wait>=0.8f) x2=3;
          if (tremor<x2) tremor=x2;
          goto donesf;
        }//26

        if (i4==27){//inventory add
          x2=sfx[i3].wait*32;
          img0(ps,(sfx[i3].x-tpx)*32+8,(sfx[i3].y-tpy)*32+8+x2,inventoryadd_icon);
          static object inventoryadd_tempobj;
          ZeroMemory(&inventoryadd_tempobj,sizeof(object));
          inventoryadd_tempobj.type=sfx[i3].x2;
          getspr(&inventoryadd_tempobj);
          img(bt16,bt32);
          img0(ps,(sfx[i3].x-tpx)*32+8,(sfx[i3].y-tpy)*32+8+x2,bt16);
          goto donesf;
        }//27

donesf:
        if (sfx[i3].wait<=0) sfx[i3].type=0; //remove sfx
        sfx[i3].wait-=et;
        if (sfx[i3].wait<=0) sfx[i3].wait=0;
donesf2:;

        //check for more sf
      }//=!0
    }//i3


    lookdisplay=0;
    if (STATUSMESSwait){
      txtset(t,STATUSMESSdisplaying);

      // Check if this is a "look" message.
      txtset(t3, "Thou dost see");
      if (txtsearch(t, t3) > 0) {
        // f: defer the floating "look" text to the post-UI pass so it renders
        // ABOVE the sidebar/panels and is clamped fully on-screen. Capture the
        // text now; the actual draw happens after the FRAME panels are drawn
        // (see the `lookdisplay` block).
        if (!looktext) looktext=txtnew();
        txtset(looktext,t);
        lookdisplay=1;
      } else {
        x = 0;
        y = 768-32;

        txtfnt=fnt1naa;
        txtcol=rgb(0,0,0);
        txtout(ps,x,y,t);
        txtout(ps,x+2,y+2,t);
        txtout(ps,x+2,y,t);
        txtout(ps,x,y+2,t);
        txtout(ps,x+1,y,t);
        txtout(ps,x+2,y+1,t);
        txtout(ps,x,y+1,t);
        txtout(ps,x+1,y+2,t);
        txtcol=rgb(255,255,255);

	  // s333 change color of combat info text
	  if (combatinfo) {
		  if (statusmessagechanged) {
			  statusmessagechanged = 0;
			  txtcolprev = txtcol;

			  txtset(t3, "P:");
			  if ((txtsearch(t, t3) == 1)) {
				  txtcolprev = rgb(255, 80, 80); // red
			  }

			  txtset(t3, "C:");
			  if ((txtsearch(t, t3) == 1)) {
				  txtcolprev = rgb(150, 255, 150); // green
			  }

			  txtset(t3, "O:");
			  if ((txtsearch(t, t3) == 1)) {
				  txtcolprev = rgb(255, 255, 80); // yellow
			  }

			  txtset(t3, "I:");
			  if ((txtsearch(t, t3) == 1)) {
				  txtcolprev = rgb(0, 255, 255); // cyan
			  }
		  }

		  txtcol = txtcolprev;
	  }

        //txtfnt=fnt1;
        txtout(ps,x+1,y+1,t);
      }
    }



    if (U6O_WALKTHRU){
walkthru_pos_skip0:

      // RW-P4 follow-up: dr/walkthru.txt coordinates were recorded as legacy
      // 32x24 screen offsets (pre-RW-P4.1 tpx/tpy; the WALK HERE icon's world
      // tile is walkthru_x+15, walkthru_y+11). The client's dynamic tpx/tpy
      // origin shifts with the window size, so the proximity check must use
      // the legacy reference frame (same shim as the mover/sobj decoders) or
      // the walkthrough never advances once the view is no longer 32x24.
      static long wt_tpx_legacy, wt_tpy_legacy;
      getscreenoffset_legacy(tplayer->x,tplayer->y,&wt_tpx_legacy,&wt_tpy_legacy);

      i=0;
      if (!walkthru_x) i=1;
      x=abs(wt_tpx_legacy-walkthru_x); y=abs(wt_tpy_legacy-walkthru_y);
      if ((x<=3)&&(y<=3)) i=1;
      if (walkthru_pos_skip) i=1;

      if (i){ //i==1 to load a new location


        i2=seek(walkthru_fh);
        if (seek(walkthru_fh)<lof(walkthru_fh)){
          txtfilein(t,walkthru_fh); if (walkthru_pos_skip) walkthru_pos_skip--;
          walkthru_pos++;
          if ((t->d2[0]>=48)&&(t->d2[0]<=57)){
            walkthru_x=txtnum(t);
            txtfilein(t,walkthru_fh); if (walkthru_pos_skip) walkthru_pos_skip--;
            walkthru_pos++;
            walkthru_y=txtnum(t);
            goto walkthru_newxy;

          }else{






            if (!walkthru_pos_skip){
              if (x||y){
                seek(walkthru_fh,i2); walkthru_pos--;
              }else{
                //display text now
                for (i3=0;i3<=255;i3++){ if (sfx[i3].type==0){
                  sfx[i3].type=3;
                  sfx[i3].x=walkthru_x+15;
                  sfx[i3].y=walkthru_y+11+2;
                  sfx[i3].x2=t->l;
                  static txt *dbgt5;
                  dbgt5=txtnew();
                  txtset(dbgt5,t);
                  sfx[i3].p=dbgt5;
                  z=t->l;
                  f=(float)z/10.0f;
                  if (f<1)f=1;
                  if(f>4)f=4;
                  sfx[i3].wait=f;
                  sfx[i3].more=187;
shiftdown2:
                  for (i2=0;i2<=255;i2++){ if ((sfx[i2].type==3)&&(i2!=i3)){
                    if ((sfx[i3].y==sfx[i2].y)||(sfx[i3].y==(sfx[i2].y+1))||(sfx[i3].y==(sfx[i2].y-1))) {sfx[i3].y=sfx[i2].y+2; goto shiftdown2;}
                  }}

                  //save walkthru pos
                  txtset(t,".\\dr\\walkthru.pos");
                  tfh=open2(t,OF_READWRITE|OF_SHARE_COMPAT|OF_CREATE);
                  put(tfh,&walkthru_pos,4); //lines to skip of the walkthru
                  close(tfh);

                  goto walkthru_txtdone;
                }} //i3, type==0
              }
            }//!walkthru_pos_skip

          }
        }else{//seek(walkthru_fh)<lof(walkthru_fh)
          U6O_WALKTHRU=FALSE;
        }
      }//i
walkthru_txtdone:
      if ((unsigned long)(ett*4.0f)&1){
walkthru_newxy:
        if (!walkthru_pos_skip){
          img(bt32,-32*15,-32*3,sfx8);
          img0(ps,(walkthru_x-tpx+15)*32,(walkthru_y-tpy+11)*32,bt32);
        }//!walkthru_pos_skip
      }
    }//U6O_WALKTHRU
    if (walkthru_pos_skip) goto walkthru_pos_skip0;






  } //(mess_UPDATEps->l&&clientframe)
