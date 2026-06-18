// LCS-P4.12: part_panel_draw — LAST chunk of the in-game per-frame block.
// LCS brace-seam: CONTINUES + CLOSES the in-game `{ ... }` block opened in
// part_game_open. Contains per-frame panel draw (checkpanel2:/displaypanel:/
// displayobj:), endgame message, the SFX shift (shiftdown3:), and the DEBUG
// INFO block, then the trailing `}` that balances the in-game block opener.
// ⚠️ Touches FRAME globals & cltset (see docs/ui/README.md) — move only.
// Pure relocation — do not edit. See docs/plans/plan-loopClientSplit.md.


if
(qkstf_update) {
    qkstf_update = FALSE;
    //refresh qkstf
    x = 0;
    for (i = 0; i <= 7; i++) {
        if (CLIENTplayer->party[i] != NULL) {
            x++; //count party members
        }
    }
    if (qkstf->graphic->d.dwHeight != (x * 64 + 32)) {
        free(qkstf->graphic);
        qkstf->graphic = newsurf(128, x * 64 + 32, SURF_SYSMEM16); //adjust frame size
    }

    for (i = 0; i <= x; i++) {
        img(qkstf->graphic, 0, i * 64 + 32, mini_2);
    }
    img(qkstf->graphic, 0, x * 64, mini_3);

    i = -1;
    for (i2 = 0; i2 <= 7; i2++) {
        if (CLIENTplayer->party[i2] != NULL) {
            i++;
            tnpc = (npc *) CLIENTplayer->party[i2]->more; //shortcut

            img0(qkstf->graphic, 4, i * 64 - 2 + 32, getportrait(tnpc->port));


            txtset(t, tnpc->name);
            x = 8;
            y = i * 64 + 32 + 16 - 8 + 32;
            txtcol = rgb(0, 0, 0);
            txtout(qkstf->graphic, x, y, t);
            txtout(qkstf->graphic, x + 2, y + 2, t);
            txtout(qkstf->graphic, x + 2, y, t);
            txtout(qkstf->graphic, x, y + 2, t);
            txtcol = rgb(255, 255, 255);
            if (i2 == 0) txtcol = namecolour;
            txtout(qkstf->graphic, x + 1, y + 1, t);
            txtcol = rgb(255, 255, 255);

            x2 = tnpc->hp & 32767;
            txtnumint(t, x2);
            x = 64 + 32 - 16 + 4;
            y = i * 64 + 4 + 32;
            if (x2 < 100) x = x + 10;
            if (x2 < 10) x = x + 10;


            txtcol = rgb(168, 30, 24);
            if (tnpc->hp & 32768) txtcol = rgb(24, 168, 24);
            txtout(qkstf->graphic, x, y, t);
            txtout(qkstf->graphic, x + 2, y + 2, t);
            txtout(qkstf->graphic, x + 2, y, t);
            txtout(qkstf->graphic, x, y + 2, t);
            txtcol = rgb(255, 64, 32);
            if (tnpc->hp & 32768) txtcol = rgb(64, 255, 64);
            txtout(qkstf->graphic, x + 1, y + 1, t);

            txtnumint(t, tnpc->mp);
            x = 64 + 32 - 16 + 4;
            y = i * 64 + 4 + 16 + 4 + 32;
            if (tnpc->mp < 100) x = x + 10;
            if (tnpc->mp < 10) x = x + 10;


            txtcol = rgb(72, 72, 168);
            txtout(qkstf->graphic, x, y, t);
            txtout(qkstf->graphic, x + 2, y + 2, t);
            txtout(qkstf->graphic, x + 2, y, t);
            txtout(qkstf->graphic, x, y + 2, t);
            txtcol = rgb(96, 128, 255);
            txtout(qkstf->graphic, x + 1, y + 1, t);


            x2 = 0;
            for (x = 0; x <= 255; x++) {
                if (spell[i2][x]) x2 = 1;
            }
            if (x2) {
                if (client_spellwait[i2]) {
                    img0(qkstf->graphic, 4 + 56 - 2 - 2, i * 64 - 16 + 32 + 32 + 1, spellbookmini2);
                } else {
                    img0(qkstf->graphic, 4 + 56 - 2 - 2, i * 64 - 16 + 32 + 32 + 1, spellbookmini);
                }
            }
        }
    }
} //upflags_update==TRUE;

if
(qkstf
->
mouse_click
)
{
    qkstf->mouse_click=FALSE;
    i2=(qkstf->mouse_y-32)/64;
    x=qkstf->mouse_x; y=qkstf->mouse_y-32-64*i2;
    for (i=0;i<=7;i++){ if (CLIENTplayer->party[i]!=NULL){
      if (i2==0){

        if ((x>=(56+4))&&(x<(56+4+32))){ //hide/unhide spellbook!
          if ((y>=(16))&&(y<(16+32))){

            if (party_spellbook_frame[i]->offset_y<kPanelHideThresholdY){
              party_spellbook_frame[i]->offset_y+=kPanelHideDeltaY;
            }else{
              if (party_spellbook_frame[i]->offset_y>=kPanelHideThresholdY) party_spellbook_frame[i]->offset_y-=kPanelHideDeltaY;
            }


          }}


        if (x<(56+4)){ //hide/unhide!
          if (party_frame[i]->offset_y<kPanelHideThresholdY){
            party_frame[i]->offset_y+=kPanelHideDeltaY;
          }else{
            if (party_frame[i]->offset_y>=kPanelHideThresholdY) party_frame[i]->offset_y-=kPanelHideDeltaY;
          }
        }//hide/unhide

        goto qkstf_mcdone;
      }
      i2--;
    }}
qkstf_mcdone:;
  }







img(tmini_1, 0, 0, mini_1); //copy original background

//underground?
moonlight=
0;
if
(AMBIENTLIGHT_SHOWSUN
==
FALSE
)
{
    img0(tmini_1,7,4,cave); goto underground;
  }


//use btime
if
((btime
>
5
)
&&
(btime<20)
)
{
    x=(float)128.0f-((btime-5.0f)*6.5f)-32+8+4-8;
    y=16+4-(sin((btime-5.0f)/4.8f)*14.0f)-8;
    if ((btime>7)&&(btime<18)){
      img0(tmini_1,x,y,sun);
    }else{
      img0(tmini_1,x,y,sun2);
    }
  }


if
(moon1
==
NULL
)
 {moon1=OBJnew_local(); moon1->type=73;}

//display moons!
f=btime2
*
1.125f
+
3.0f; //first moon fixed offset sun+4 hours
x=f
/
24;
f
-=
(
float
)
x *
24.0f;
if
((f
>
5
)
&&
(f<20)
)
{

    if (f>=12.0f) f3=-(f-12.0f); else f3=12.0f-f;
    f3/=1.125f;
    f2=btime+f3;

    f4=f; f=12.0f;
    if (f2>=24.0f) f2-=24.0f;
    if (f2<0.0f) f2+=24.0f;

    if (f2>=f) f2=f2-f; else f2=f2+(24.0f-f);
    f2+=1.5f; if (f2>=24.0f) f2-=24.0f;
    //f2=distance LEFT of the sun 0-24.0f
    x2=f2/3.0f;

    moonlight=4-abs(4-x2);

    x2*=1024;



    x2+=73;
    moon1->type=x2;




    f=f4;
    x=(float)128.0f-((f-5.0f)*6.5f)-32+8+4-8;
    y=16+4-(sin((f-5.0f)/4.8f)*14.0f)-8;
    getspr(moon1);
    img0(tmini_1,x,y,bt32);
  }

//display moons! (lol this will be interesting!)
f=btime2
*
1.1875f
+
5.0f; //first moon fixed offset sun+4 hours
x=f
/
24;
f
-=
(
float
)
x *
24.0f;
if
((f
>
5
)
&&
(f<20)
)
{

    //calculate moon phase!
    if (f>=12.0f) f3=-(f-12.0f); else f3=12.0f-f;
    f3/=1.1875f;

    f2=btime+f3;
    f4=f; f=12.0f;
    if (f2>=24.0f) f2-=24.0f;
    if (f2<0.0f) f2+=24.0f;

    if (f2>=f) f2=f2-f; else f2=f2+(24.0f-f);
    f2+=1.5f; if (f2>=24.0f) f2-=24.0f;
    //f2=distance LEFT of the sun 0-24.0f
    x2=f2/3.0f;

    if (moonlight<(4-abs(4-x2))) moonlight=4-abs(4-x2);

    x2*=1024;


    x2+=73;
    moon1->type=x2;

    f=f4;
    x=(float)128.0f-((f-5.0f)*6.5f)-32+8+4-8;
    y=16+4-(sin((f-5.0f)/4.8f)*14.0f)-8;
    getspr(moon1);
    img0(tmini_1,x,y,bt32);
  }




if
((btime
>
7
)
&&
(btime<18)
)
{


    img0(tmini_1,0,16,horizon);//GPFGPF
    //img0(tmini_1,0,16,horizontesting);//GPFGPF testing
  }else
{
    img0(tmini_1,0,16,horizon2);
  }



txtset(t, "");
if
(windy2
==
1
)
txtset(t, "N");
if
(windy2
==
-
1
)
txtset(t, "S");
if
(windx2
==
1
)
txtadd(t, "W");
if
(windx2
==
-
1
)
txtadd(t, "E");
if
(t
->
l
)
{
    txtfnt=fnt2;
    x=128-8-t->l*8; y=2;
    txtcol=rgb(168,60,0);
    txtout(tmini_1,x+1,y+1,t);
    txtfnt=fnt1;
  }

underground : img(qkstf->graphic, 0, 0, tmini_1); //moon and sun on qk stat

img(volcontrol_surf, 0, 0, volcontrol_background);
img(volcontrol_surf, 46 + u6ovolume * 20 / 72 - 4, 8 + 3, volcontrol_tab1);
img(volcontrol_surf, 46 + u6omidivolume * 20 / 72 - 4, 32 + 8 - 5, volcontrol_tab3);
img(volcontrol_surf, 46 + u6ovoicevolume * 20 / 72 - 4, 32 + 8 - 5 + 24, volcontrol_tab2);
if
(u6ovoicevolume
==
0
)
{
    img0(volcontrol_surf,12,60,vm_volmmute);
  }




if
(musickeyboard
->
mouse_click
)
{
    //calculate key pressed
    x=musickeyboard->mouse_x;
    //black key?
    if (musickeyboard->mouse_y<=25){
      //9,7b,9,7b,17,7b,9,7b,9,7b,17...
      y2=0; y3=1; x3=8;
nextblackkey:
      if ((x>=x3)&&(x<(x3+9))){
        x2=y3; goto gotkey;
      }
      x3+=16; x4=x3; y3+=2;
      if (y2==1) x3+=8;
      if (y2==4) x3+=8;
      if (y2==6) x3+=8;
      if (y2==9) x3+=8;
      if (y2==11) x3+=8;
      if (y2==14) x3+=8;
      if (y2==16) x3+=8;
      if (y2==19) x3+=8;
      if (y2==21) x3+=8;
      if (y2==24) x3+=8;
      if (y2==26) x3+=8;
      if (y2==29) x3+=8;
      if (y2==31) x3+=8;
      if (x4!=x3) y3++;
      if (y2==34) goto blackkeycheck_done;
      y2++; goto nextblackkey;
    }
blackkeycheck_done:
    //white keys
    f=x; f/=13.7f; x2=f; x3=x2;
    for (y2=1;y2<=x3;y2++){
      x2++;
      if (y2==3) x2--;
      if (y2==7) x2--;
      if (y2==10) x2--;
      if (y2==14) x2--;
      if (y2==17) x2--;
      if (y2==21) x2--;
      if (y2==24) x2--;
      if (y2==28) x2--;
      if (y2==31) x2--;
      if (y2==35) x2--;
      if (y2==38) x2--;
      if (y2==42) x2--;
      if (y2==45) x2--;
    }
gotkey: //x2 is value of key

    mididown(0,24+x2); musickeyboard_set=24+x2;

    txtset(t,"Press computer key to assign to music keyboard note("); txtnumint(t2,x2); txtadd(t,t2); txtadd(t,")");

    STATUSMESSadd(t);
    musickeyboard->mouse_click=0;
  }




if
(portraitlook_wait) {
    static object *portraitlook_obj = OBJnew_local();
    if (portraitlook_equip) {
        for (i = 0; i < 8; i++) {
            if (portraitlook_obj->type = portraitlook_type[i]) {
                getspr(portraitlook_obj);


                if (i == 0) img0(statusmessage_viewnpc->graphic, helmx + 112, helmy - 128, bt32);
                if (i == 1) img0(statusmessage_viewnpc->graphic, wep_rightx + 112, wep_righty - 128, bt32);
                if (i == 2) img0(statusmessage_viewnpc->graphic, wep_leftx + 112, wep_lefty - 128, bt32);
                if (i == 3) img0(statusmessage_viewnpc->graphic, armourx + 112, armoury - 128, bt32);
                if (i == 4) img0(statusmessage_viewnpc->graphic, bootsx + 112, bootsy - 128, bt32);
                if (i == 5) img0(statusmessage_viewnpc->graphic, ring_rightx + 112, ring_righty - 128, bt32);
                if (i == 6) img0(statusmessage_viewnpc->graphic, ring_leftx + 112, ring_lefty - 128, bt32);
                if (i == 7) img0(statusmessage_viewnpc->graphic, neckx + 112, necky - 128, bt32);

                if (portraitlook_plusbonus[i]) {
                    if (i == 0) {
                        x = helmx;
                        y = helmy;
                    }
                    if (i == 1) {
                        x = wep_rightx;
                        y = wep_righty;
                    }
                    if (i == 2) {
                        x = wep_leftx;
                        y = wep_lefty;
                    }
                    if (i == 3) {
                        x = armourx;
                        y = armoury;
                    }
                    if (i == 4) {
                        x = bootsx;
                        y = bootsy;
                    }
                    if (i == 5) {
                        x = ring_rightx;
                        y = ring_righty;
                    }
                    if (i == 6) {
                        x = ring_leftx;
                        y = ring_lefty;
                    }
                    if (i == 7) {
                        x = neckx;
                        y = necky;
                    }
                    x += 112;
                    y -= 128;
                    x += 24;
                    txtset(t, "+");
                    txtnumint(t2, portraitlook_plusbonus[i]);
                    txtadd(t, t2);
                    txtfnt = fnt3;
                    txtcol = 0;
                    txtout(statusmessage_viewnpc->graphic, x, y - 1, t);
                    txtout(statusmessage_viewnpc->graphic, x - 1, y, t);
                    txtout(statusmessage_viewnpc->graphic, x + 1, y, t);
                    txtout(statusmessage_viewnpc->graphic, x, y + 1, t);
                    txtout(statusmessage_viewnpc->graphic, x - 1, y - 1, t);
                    txtout(statusmessage_viewnpc->graphic, x + 1, y - 1, t);
                    txtout(statusmessage_viewnpc->graphic, x + 1, y + 1, t);
                    txtout(statusmessage_viewnpc->graphic, x - 1, y + 1, t);
                    txtcol = rgb(32, 255, 32);
                    txtout(statusmessage_viewnpc->graphic, x, y, t);
                    txtfnt = fnt1;
                    txtcol = rgb(255, 255, 255);
                }
            } //portraitlook_type[i]
        } //i
    } //portraitlook_equip

    txtset(t, portraitlook_name);
    x = 56 - 4 * t->l;
    y = 128 - 16 - 8;
    txtcol = rgb(0, 0, 0);
    txtout(statusmessage_viewnpc->graphic, x, y, t);
    txtout(statusmessage_viewnpc->graphic, x + 2, y + 2, t);
    txtout(statusmessage_viewnpc->graphic, x + 2, y, t);
    txtout(statusmessage_viewnpc->graphic, x, y + 2, t);
    txtcol = portraitlook_namecolour;
    txtout(statusmessage_viewnpc->graphic, x + 1, y + 1, t);
    txtcol = rgb(255, 255, 255);
}


for
(i=
0;
i
<=
23;
i
++
)
{
    if ((i>=0)&&(i<=7)) pmf=party_frame[i];
    if ((i>=8)&&(i<=15)) pmf=party_spellbook_frame[i-8];
    if (i==16) pmf=musickeyboard;
    if (i==17) pmf=inpf;
    if (i==18) pmf=con_frm;
    if (i==19) pmf=volcontrol;
    if (i==20) pmf=qkstf;
    if (i==21) pmf=minimap_frame;
    if (i==22) pmf=tmap_frame;
    if (i==23) pmf=statusmessage_viewprev;
    x=pmf->offset_x; y=pmf->offset_y;
    //get dimentions of frame as x2,y2
    if (pmf->graphic&&(pmf->size_x==0)&&(pmf->size_y==0)){
      x2=pmf->graphic->d.dwWidth; y2=pmf->graphic->d.dwHeight;
    }else{
      x2=pmf->size_x; y2=pmf->size_y;
    }
    // RW-P4.10: "onscreen" here means "not parked at the hide sentinel"
    // (i.e. the panel is actually visible to the user, vs. hidden by
    // adding kPanelHideDeltaX/Y to its offset). Old values 2048/1536
    // matched the legacy hide thresholds; now they track the
    // kPanelHideThresholdX/Y constants in viewport.h.
    if ((x<=(kPanelHideThresholdX-x2))&&(y<=(kPanelHideThresholdY-y2))){ //onscreen (or supposed to be onscreen!)
      // RW: clamp dragged panels to the *live* back-buffer extents instead of the
      // legacy 1024x768 floor, otherwise maximized windows visually trap every
      // widget at x~=1016 / y~=760 even though the cursor moves further.
      // (windowResize ternary removed 2026-05-27 — always use the live size.)
      const long bb_w = (long)backbufferW();
      const long bb_h = (long)backbufferH();
      x3=8-x2; if (x<x3) pmf->offset_x=x3;
      y3=8-y2; if (y<y3) pmf->offset_y=y3;
      x3=bb_w-8; if (x>x3) pmf->offset_x=x3;
      y3=bb_h-8; if (y>y3) pmf->offset_y=y3;
      if (i==16){cltset.musickeyboard_offset_x=pmf->offset_x; cltset.musickeyboard_offset_y=pmf->offset_y;}
      if (i==17){cltset.inpf_offset_x=pmf->offset_x; cltset.inpf_offset_y=pmf->offset_y;}
      // RW: deliberately do NOT refresh the qkstf/volcontrol
      // user-positioned cache from `pmf->offset_x` here. The live
      // offset has just been auto-clamped to `backbufferW()`, which
      // for a smaller-than-saved window would shrink the cache and
      // permanently lose the user's far-right position from a
      // previous maximized session. The cache is updated exclusively
      // by a real user drag (see the drag handler near line 1376).
    }//onscreen
    //update default frame settings
    if ((i>=0)&&(i<=7)){
      cltset.party_frame_offset_x[i]=pmf->offset_x; cltset.party_frame_offset_y[i]=pmf->offset_y;
      if (cltset.party_frame_offset_x[i]>kPanelHideThresholdX) cltset.party_frame_offset_x[i]-=kPanelHideDeltaX;
    }
    if ((i>=8)&&(i<=15)){
      cltset.party_spellbook_frame_offset_x[i-8]=pmf->offset_x; cltset.party_spellbook_frame_offset_y[i-8]=pmf->offset_y;
      if (cltset.party_spellbook_frame_offset_x[i-8]>kPanelHideThresholdX) cltset.party_spellbook_frame_offset_x[i-8]-=kPanelHideDeltaX;
    }
    if (i==18){cltset.con_frm_offset_x=pmf->offset_x; cltset.con_frm_offset_y=pmf->offset_y;}
    // RW: mirror qkstf/volcontrol to cltset from the user-positioned
    // cache, NOT from pmf->offset_x — when volcontrol is hidden the
    // live offset is parked in the off-screen sentinel range and would
    // overwrite a previously-saved user position. Writing 32767 when
    // the panel is at its anchored default keeps the load path's
    // "default vs. user" distinction working across restarts.
    if (i==20){
      if (u6o::client::g_qkstf_user_positioned){
        cltset.qkstf_offset_x=u6o::client::g_qkstf_user_x;
        cltset.qkstf_offset_y=u6o::client::g_qkstf_user_y;
      } else {
        cltset.qkstf_offset_x=32767;
      }
    }
    if (i==19){
      if (u6o::client::g_volcontrol_user_positioned){
        cltset.volcontrol_offset_x=u6o::client::g_volcontrol_user_x;
        cltset.volcontrol_offset_y=u6o::client::g_volcontrol_user_y;
      } else {
        cltset.volcontrol_offset_x=32767;
      }
    }
    if (i==21){cltset.minimap_offset_x=pmf->offset_x; cltset.minimap_offset_y=pmf->offset_y;}
    if (i==22){cltset.tmap_offset_x=pmf->offset_x; cltset.tmap_offset_y=pmf->offset_y;}
    // RW: mirror statusmessage_viewprev from the user-positioned cache (same
    // rule as qkstf/volcontrol). The live offset is auto-clamped above to the
    // current back-buffer, so reading the cache preserves a far-right/maximized
    // position across a smaller-window session. 32767 = "no override".
    if (i==23){
      if (u6o::client::g_statusprev_user_positioned){
        cltset.statusprev_offset_x=u6o::client::g_statusprev_user_x;
        cltset.statusprev_offset_y=u6o::client::g_statusprev_user_y;
      } else {
        cltset.statusprev_offset_x=32767;
      }
    }
  }//i (frame)
clientsettingsvalid=TRUE;


// r333 no changes are made here
//frame: display
pn=firstpanel;
checkpanel2 : 
if
(pn
!=
NULL
)
{
    lpn=pn; pn=(FRAME*)pn->next; goto checkpanel2;
  }
pn=lpn;
displaypanel : 
if
(pn
!=
NULL
)
{
    if (pn->graphic!=NULL){
      if (pn->graphic_use0pixel==TRUE){


        if (pn->mouse_over||(pn->mouse_over_transparent==FALSE)){
          img(ps,pn->offset_x,pn->offset_y,pn->graphic);
        }else{

          if (pn->mouse_over_transparent==1){
            imgt(ps,pn->offset_x,pn->offset_y,pn->graphic);
          }else{
            img75t(ps,pn->offset_x,pn->offset_y,pn->graphic);

          }

        }


      }else{

        if (pn->mouse_over||(pn->mouse_over_transparent==FALSE)){
          img0(ps,pn->offset_x,pn->offset_y,pn->graphic);
        }else{


          if (pn->mouse_over_transparent==1){
            imgt0(ps,pn->offset_x,pn->offset_y,pn->graphic);
          }else{

            img75t0(ps,pn->offset_x,pn->offset_y,pn->graphic);
          }



        }





      }

	  // r333 no changes are made here
	  //img(pspartynew, resxn1-256, 0, pn->graphic);
	  //refresh(pspartynew);
	}
    FRM_type=(FRM_TYPE*)pn->firstobject;
displayobj:
    if (FRM_type!=NULL)
    {
      x=FRM_type->type;
      if (x==1){ //FRM_ONOFF
        tonoff=(FRM_ONOFF*)FRM_type;
        if (tonoff->value==0) img0(ps,pn->offset_x+tonoff->offset_x,pn->offset_y+tonoff->offset_y,tonoff->graphic_off);
        if (tonoff->value==1) img0(ps,pn->offset_x+tonoff->offset_x+tonoff->offset2_x,pn->offset_y+tonoff->offset_y+tonoff->offset2_y,tonoff->graphic_on);
      }
      if (x==2){ //FRM_IMAGE
        timage=(FRM_IMAGE*)FRM_type;
        if (timage->graphic_use0pixel==TRUE){
          img(ps,pn->offset_x+timage->offset_x,pn->offset_y+timage->offset_y,timage->graphic);
        }else{
          img0(ps,pn->offset_x+timage->offset_x,pn->offset_y+timage->offset_y,timage->graphic);
        }
      }
      if (x==3){ //FRM_TXT
        ttxt=(FRM_TXT*)FRM_type;
        if (ttxt->timer!=0.0f){
          if (ttxt->timer<=0.015625f){
            ttxt->timer=0.015625f;
          }else{
            ttxt->timer-=et;
          }}
        if (ttxt->timer!=0.015625f){
          thfont=txtfnt;
          if (ttxt->font!=NULL) txtfnt=ttxt->font;
          if (ttxt->special&1){
            txtcol=0;
            txtout(ps,pn->offset_x+ttxt->offset_x-1,pn->offset_y+ttxt->offset_y-1,ttxt->text);
            txtout(ps,pn->offset_x+ttxt->offset_x+1,pn->offset_y+ttxt->offset_y+1,ttxt->text);
            txtout(ps,pn->offset_x+ttxt->offset_x+1,pn->offset_y+ttxt->offset_y-1,ttxt->text);
            txtout(ps,pn->offset_x+ttxt->offset_x-1,pn->offset_y+ttxt->offset_y+1,ttxt->text);
          }
          txtcol=ttxt->col;
          txtout(ps,pn->offset_x+ttxt->offset_x,pn->offset_y+ttxt->offset_y,ttxt->text);
          txtcol=0xFFFFFF; txtfnt=thfont;
        }
      }
      if (x==4){ //FRM_INPUT
        tinp=(FRM_INPUT*)FRM_type;
        thfont=txtfnt;
        if (tinp->col!=0xFFFFFF) txtcol=tinp->col;
        if (tinp->font!=NULL) txtfnt=tinp->font;
        if (tinp->prefix!=NULL){txtset(t,tinp->prefix);}else{txtset(t,"");}
        txtadd(t,tinp->input);
        if (GETINPUT_current()==tinp->input){
          i=ett*tinp->cursor_blinkrate;
          if (!(i&1)){
            txtNEWLEN(t,t->l+1);
            t->d[t->l-1]=tinp->cursor;
          }
          else
          {
            txtadd(t," ");
          }
        }
        if (tinp->suffix!=NULL) txtadd(t,tinp->suffix);
        //optional graphic
        img0(ps,pn->offset_x+tinp->offset_x,pn->offset_y+tinp->offset_y,tinp->graphic);


        //inpf2->input
        //check if input 't' will go beyond screen boundry
        if (tinp==inpf2){
          if (t->l>2){
            tagxy.cx=0; tagxy.cy=0;
            ps->s->GetDC(&taghdc);
            SelectObject(taghdc,txtfnt);
            GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
            ps->s->ReleaseDC(taghdc);
            if (pn->offset_x+tinp->offset_x+tagxy.cx>=1024){
inpf2crop:
              txtset(t2,t); txtset(t,"..."); txtadd(t,t2);
              if (t->l>=6){
                tagxy.cx=0; tagxy.cy=0;
                ps->s->GetDC(&taghdc);
                SelectObject(taghdc,txtfnt);
                GetTextExtentPoint32(taghdc,t->d,t->l,&tagxy);
                ps->s->ReleaseDC(taghdc);
                if (pn->offset_x+tinp->offset_x+tagxy.cx>=1024){
                  txtright(t,t->l-4);
                  goto inpf2crop;
                }//...+tagxy.cx>=1024
              }//>=6
            }//...+tagxy.cx>=1024
          }//>2
        }//inpf2

        if (tinp->special&1){
          txtcol=0;
          txtout(ps,pn->offset_x+tinp->offset_x-1,pn->offset_y+tinp->offset_y-1,t);
          txtout(ps,pn->offset_x+tinp->offset_x+1,pn->offset_y+tinp->offset_y+1,t);
          txtout(ps,pn->offset_x+tinp->offset_x+1,pn->offset_y+tinp->offset_y-1,t);
          txtout(ps,pn->offset_x+tinp->offset_x-1,pn->offset_y+tinp->offset_y+1,t);
        }
        txtcol=tinp->col;
        txtout(ps,pn->offset_x+tinp->offset_x,pn->offset_y+tinp->offset_y,t);
        txtcol=0xFFFFFF; txtfnt=thfont;
      }
      FRM_type=(FRM_TYPE*)FRM_type->next; goto displayobj;
    }
    pn=(FRAME*)pn->previous; goto displaypanel;
  }


if
(shiftnum_show) {
    x = 14 * 32;
    y = 12 * 32;
    txtset(t, "How many? ");
    txtadd(t, tshiftnum);
    txtcol = 0;
    txtout(ps, x, y, t);
    txtout(ps, x + 2, y + 2, t);
    txtout(ps, x + 2, y, t);
    txtout(ps, x, y + 2, t);
    txtcol = rgb(255, 255, 255);
    txtout(ps, x + 1, y + 1, t);
}


if
(endgame) {
    if (endgame_timer <= 140.0f) {
        x = 0;
        txtset(t5, "");
        x5 = 187;


        if (endgame_message == 0) {
            STATUSMESSadd("The Codex has vanished!");
            endgame_message++;
        }

        if (endgame_message == 1) {
            if (endgame_timer >= 4.0f) {
                txtset(t5, "A glowing portal springs from the floor!");
                endgame_message++;
            }
        }

        if (endgame_message == 2) {
            x = 1;
            if (endgame_timer >= 8.0f) {
                STATUSMESSadd("You have gained strength, dexterity and intelligence!");
                txtset(t5, "From its crimson depths Lord British emerges, trailed by the mage Nystul.");
                endgame_message++;
            }
        }

        if (endgame_message == 3) {
            x = 2;
            if (endgame_timer >= 12.0f) {
                x5 = 5;
                txtset(t5, "Anguish and disbelief prevail on the royal seer's face.");
                endgame_message++;
            }
        }

        if (endgame_message == 4) {
            x = 2;
            if (endgame_timer >= 16.0f) {
                x5 = 4;
                txtset(
                    t5,
                    "But Lord British directs his stony gaze at you and speaks as if to a wayward child.\\\"Thou didst have just cause to burgle our Codex, I trust\", his Majesty says.\\\"But for Virtue's sake...\"\\\"WHAT HAST THOU DONE WITH IT?\"");
                endgame_message++;
            }
        }

        if (endgame_message == 5) {
            x = 2;
            if (endgame_timer >= 32.0f) {
                if (tplay->party[0]) {
                    tnpc = (npc *) tplay->party[0]->more;
                    x5 = tnpc->port;
                } else x5 = portlast;
                txtset(
                    t5,
                    "\"Was the book ever truly ours, your majesty?\"\\\"Was it written for Britannia alone?\"\\\"Thou dost no longer hold the Codex, but is its wisdom indeed lost?\"\\\"Look into the Vortex, and let the Codex answer for itself!\"\\You pick up the concave lens and pass it to the King.");
                endgame_message++;
            }
        }

        if (endgame_message == 6) {
            x = 2;
            if (endgame_timer >= 52.0f) {
                x5 = 4;
                txtset(t5, "Lord British holds the glass before the wall.");
                endgame_message++;
            }
        }

        if (endgame_message == 7) {
            x = 3;
            if (endgame_timer >= 56.0f) {
                txtset(
                    t5,
                    "The Codex of Ultimate Wisdom wavers into view against a myriad of swimming stars!\\Yet the book remains closed.");
                endgame_message++;
            }
        }

        if (endgame_message == 8) {
            x = 3;
            if (endgame_timer >= 64.0f) {
                txtset(t5, "And waves of heat shimmer in the air, heralding the birth of another red gate!");
                endgame_message++;
            }
        }

        if (endgame_message == 9) {
            x = 4;
            if (endgame_timer >= 68.0f) {
                x5 = 166;
                txtset(
                    t5,
                    "King Draxinusom of the Gargoyles strides forward.\\Flanked by a small army of wingless attendants.\\Like Lord British, he seems to suppress his rage only through a heroic effort of will.\\His scaly hand grasps your shoulder, and your Amulet of Submission grows very warm.\\\"Thy time hath come, Thief\", he says.");
                endgame_message++;
            }
        }

        if (endgame_message == 10) {
            x = 5;
            if (endgame_timer >= 88.0f) {
                if (tplay->party[0]) {
                    tnpc = (npc *) tplay->party[0]->more;
                    x5 = tnpc->port;
                } else x5 = portlast;
                txtset(
                    t5,
                    "\"Join my Lord in his search for peace, I beg thee.\"\\Quickly you reach down to seize the convex lens...\\...and you press it into the hand of the towering Gargoyle king, meeting his sunken eyes.");
                endgame_message++;
            }
        }

        if (endgame_message == 11) {
            x = 6;
            if (endgame_timer >= 100.0f) {
                x5 = 166;
                txtset(t5, "At your urging, King Draxinusom reluctantly raises his lens to catch the light.");
                endgame_message++;
            }
        }

        if (endgame_message == 12) {
            x = 8;
            if (endgame_timer >= 104.0f) {
                txtset(
                    t5,
                    "As Lord British holds up his own lens, every eye in the room, human and Gargoyle alike,\\fixes upon the image of the Codex which shines upon the wall.");
                endgame_message++;
            }
        }

        if (endgame_message == 13) {
            x = 8;
            if (endgame_timer >= 112.0f) {
                STATUSMESSadd("Your Amulet of Submission has shattered!");
                soundplay2(u6osound[18], u6osound_volume[18]);
                txtset(
                    t5,
                    "The ancient book opens.\\Both kings gaze upon its pages in spellbound silence, as the eloquence of Ultimate Wisdom is revealed\\in the tongues of each lord's domain.\\You, too, can read the answers the Codex gives...\\...and when its wisdom is gleaned, when Lord British  and King Draxinusom turn to each other as friends,\\hating no longer, fearing no more, you know that your mission in Britannia has ended at last.");
                endgame_message++;
            }
        }

        if (endgame_message == 14) {
            x = 9;
        }

        if (t5->l) {
            for (i3 = 0; i3 <= 255; i3++) {
                if (sfx[i3].type == 0) {
                    sfx[i3].type = 3;
                    sfx[i3].x = 920;
                    sfx[i3].y = 851 - 3;
                    sfx[i3].x2 = t5->l;
                    static txt *dbgt5;
                    dbgt5 = txtnew();
                    txtset(dbgt5, t5);
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
                    sfx[i3].more = x5;
                shiftdown3:
                    for (i2 = 0; i2 <= 255; i2++) {
                        if ((sfx[i2].type == 3) && (i2 != i3)) {
                            if ((sfx[i3].y == sfx[i2].y) || (sfx[i3].y == (sfx[i2].y + 1)) || (
                                    sfx[i3].y == (sfx[i2].y - 1))) {
                                sfx[i3].y = sfx[i2].y + 2;
                                goto shiftdown3;
                            }
                        }
                    }
                    goto endgame_donemessage;
                }
            }
        } //t5->l
    endgame_donemessage:

        img(ps, 608, 476, endgame_image[x]);
        endgame_timer += et;
    } //timer<=140.0f
} //endgame


// Left-clicking the "view previous status message" arrow toggles whether
// the status-message log stays drawn constantly. Hovering still shows it
// temporarily; the pin makes it persist until clicked off again. (Button-2
// is reserved for dragging the arrow, so only consume button-1 clicks here.)
if
(statusmessage_viewprev
->
mouse_click &
1
)
{
    statusmessage_viewprev->mouse_click=NULL;
    if (drg!=statusmessage_viewprev) statusmessage_logpinned=!statusmessage_logpinned;
  }
if
(statusmessage_viewprev
->
mouse_over
||
statusmessage_logpinned
)
{
    statusmessage_viewprev->mouse_over=FALSE;
    if (drg!=statusmessage_viewprev){
      // Scratch buffers for draw-time word wrapping (lazily allocated once).
      static txt *statuswrap[16]={0};
      if (!statuswrap[0]){ for (int si=0;si<16;si++) statuswrap[si]=txtnew(); }
      x=statusmessage_viewprev->offset_x;
      y=statusmessage_viewprev->offset_y-24;
      // Wrap budget: keep text inside the live back buffer with an 8px margin.
      // The arrow is draggable, so a line that fit at the left edge can run off
      // the right edge once moved — re-wrap each stored line to fit from x.
      long statuswrapmax=(long)backbufferW()-x-8;
      txtfnt=fnt1naa;
      for (i=0;i<=7;i++){
        if (STATUSMESSprev[i]->l){
          // Resolve this message's colour once (combat-info prefixes), then
          // apply it to every wrapped physical line of the message.
          DWORD linecol=rgb(255,255,255);
          if (combatinfo){
            txtset(t,STATUSMESSprev[i]);
            txtset(t3,"P:"); if (txtsearch(t,t3)==1) linecol=rgb(255,80,80);   // red
            txtset(t3,"C:"); if (txtsearch(t,t3)==1) linecol=rgb(150,255,150); // green
            txtset(t3,"O:"); if (txtsearch(t,t3)==1) linecol=rgb(255,255,80);  // yellow
            txtset(t3,"I:"); if (txtsearch(t,t3)==1) linecol=rgb(0,255,255);   // cyan
          }
          int nseg=STATUSMESSwrapline(STATUSMESSprev[i],statuswrapmax,statuswrap,16);
          // Draw the message block so its first segment is on top (reads
          // top-to-bottom) while the whole log still grows upward, newest line
          // nearest the arrow.
          long ytop=y-24*(nseg-1);
          for (int k=0;k<nseg;k++){
            long yk=ytop+24*k;
            txtset(t,statuswrap[k]);
            txtcol=rgb(0,0,0);
            txtout(ps,x,yk,t);
            txtout(ps,x+2,yk+2,t);
            txtout(ps,x+2,yk,t);
            txtout(ps,x,yk+2,t);
            txtout(ps,x+1,yk,t);
            txtout(ps,x+2,yk+1,t);
            txtout(ps,x,yk+1,t);
            txtout(ps,x+1,yk+2,t);
            txtcol=linecol;
            txtout(ps,x+1,yk+1,t);
          }
          y=ytop-24;
        } else {
          y-=24;
        }
      }//i
    }
  }

// f: draw the floating "look" text above the UI and clamped fully on-screen.
// It was captured during the world/sfx pass (see the `lookdisplay` capture
// above) but is drawn here, after the FRAME panels are composited, so it is
// never hidden behind the sidebar/panels and never runs off the game window.
if
(lookdisplay &&looktext) {
    static SIZE looksz;
    long lookw, lookh, lookbx, lookby, lookmaxx, lookmaxy;
    ps->s->GetDC(&taghdc);
    SelectObject(taghdc, fnt1naa);
    looksz.cx = 0;
    looksz.cy = 0;
    GetTextExtentPoint32(taghdc, looktext->d, looktext->l, &looksz);
    ps->s->ReleaseDC(taghdc);
    lookw = looksz.cx;
    lookh = (looksz.cy > 0) ? looksz.cy : 24;
    // Clamp so the whole string plus its 2px outline stays inside the live
    // back buffer (the visible game screen).
    lookbx = lookatx;
    lookby = lookaty;
    lookmaxx = (long) backbufferW() - lookw - 2;
    lookmaxy = (long) backbufferH() - lookh - 2;
    if (lookbx > lookmaxx) lookbx = lookmaxx;
    if (lookby > lookmaxy) lookby = lookmaxy;
    if (lookbx < 0) lookbx = 0;
    if (lookby < 0) lookby = 0;
    txtset(t, looktext);
    txtfnt = fnt1naa;
    txtcol = rgb(0, 0, 0);
    txtout(ps, lookbx, lookby, t);
    txtout(ps, lookbx + 2, lookby + 2, t);
    txtout(ps, lookbx + 2, lookby, t);
    txtout(ps, lookbx, lookby + 2, t);
    txtout(ps, lookbx + 1, lookby, t);
    txtout(ps, lookbx + 2, lookby + 1, t);
    txtout(ps, lookbx, lookby + 1, t);
    txtout(ps, lookbx + 1, lookby + 2, t);
    txtcol = rgb(255, 255, 255);
    txtout(ps, lookbx + 1, lookby + 1, t);
}

// r333 this is where mouse/cursor object is displayed
if
(CLIENTplayer
->
mobj
!=
NULL
)
{ //show mobject: active player
    getspr(CLIENTplayer->mobj);
    img0(ps,mx-16,my-16,bt32);

	// use omx3, omy3 to display object on mouse cursor (onto new mode surface) for new mode.
	if (smallwindow && windowsizecyclenum == 1) {
		if ( (omx3 > resxn1m) || (omy3 > resyn1m) )
			img0(psnew1b, omx3 - 16, omy3 - 16, bt32);
	}

//    if (keyon[VK_SPACE]) {
    if (keyon[VK_SPACE]&&U6O_DEBUG) {
      txtnumint(t,CLIENTplayer->mobj->type&1023); txtadd(t,"<MouseObject type"); txtout(ps,0,16,t);
      txtnumint(t,CLIENTplayer->mobj->type>>10); txtadd(t,"<MouseObject sub-index"); txtout(ps,0,48,t);
      txtnumint(t,CLIENTplayer->mobj->info); txtadd(t,"<MouseObject info"); txtout(ps,0,80,t);
      txtnumint(t,CLIENTplayer->mobj->more2); txtadd(t,"<MouseObject more"); txtout(ps,0,112,t);
    }

	// r777 send cursor item to inventory
	if (enhancen1 >= 2) {
		if (keyon[VK_CONTROL]) {
			//itemtoinv = 1;
			actionpending = 510; // drop item
			//MessageBox(NULL,"item to inv 1","Ultima 6 Online",MB_OK);
		}
	}

  }


//DISPLAY DEBUG INFO WHEN SPACE IS HELD (ONLY IF U6O_DEBUG IS TRUE)
// rrr
//  if (keyon[VK_SPACE]) {
if
(keyon[VK_SPACE]
&&
U6O_DEBUG
)
{
    txtnumint(t,btimeh); txtout(ps,512,32,t);
//    txtnumint(t,framerate); txtout(ps,512,16,t);
	txtset(t2, "FPS: "); txtnumint(t, framerate); txtadd(t2, t); txtout(ps, 512, 16, t2);

    txtnum(t,dv); txtadd(t,"<debug value 1"); txtout(ps,512+64,16,t);
    txtnum(t,dv2); txtadd(t,"<debug value 2"); txtout(ps,512+64,16+64+32,t);

    txtnumint(t,bt[tpy+my/32][tpx+mx/32]&1023); txtadd(t,"<bt"); txtout(ps,512+64,16+32,t);

    x=tpx+mx/32; txtnumint(t,x); txtadd(t,"<x"); txtout(ps,1024-64,0,t);
    y=tpy+my/32; txtnumint(t,y); txtadd(t,"<y"); txtout(ps,1024-64,32,t);

	txtnumint(t, mx); txtadd(t, "<mx"); txtout(ps, 1024 - 64, 64, t);
	txtnumint(t, my); txtadd(t, "<my"); txtout(ps, 1024 - 64, 96, t);

    if (NEThost){
      txtset(t,"Network Status: HOST");
    }else{
      txtset(t,"Network Status: CLIENT");
    }
    txtout(ps,1024-128-64,768-32,t);

    //#ifdef HOST
    //txtnumint(t,vlnkb_last); txtadd(t,"<vlnkb_last (number of allocated VLNK slots MAX:65536)"); txtout(ps,0,128,t);
    //txtnumint(t,objb_last); txtadd(t,"<objb_last (number of allocated OBJ slots MAX:524228)"); txtout(ps,0,128+32*1,t);
    //#endif
  }//END DEBUG INFO

}