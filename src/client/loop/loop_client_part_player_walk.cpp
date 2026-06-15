// LCS-P4.11: part_player_walk — post-render movement resolution chunk.
// LCS brace-seam: CONTINUES the in-game `{ ... }` block. Begins at the
// skiprefresh2: label; contains walk-through / movement resolution, tmap, and
// upflags update. Pure relocation — do not edit. See docs/plans/plan-loopClientSplit.md.
skiprefresh2:



  if (nodisplay) goto skiprefresh;
  if (!clientframe) goto skiprefresh;


  // RW-P3.3 (2026-06-02): first-show placement keyed on FRAME::positioned, not
  // the offset==4096 sentinel. The inclusive minimap/tmap hide-show toggle
  // above (kPanelHideThresholdX/DeltaX == 4096) could consume the 4096 sentinel
  // before this ran, and a cltset2-restored position saved on a larger window
  // could land off the right edge. placeFloatingPanelFirstShow clamps the home
  // fully on screen and parks it shown/hidden per peer/tmap.
  if (!minimap_frame->positioned){
    int hx=0, hy=0; //default
    if (cltset2_restored){ if (cltset2.minimap_offset_x!=32767){
      hx=cltset2.minimap_offset_x; hy=cltset2.minimap_offset_y;
      cltset2.minimap_offset_x=32767;
    }}
    placeFloatingPanelFirstShow(minimap_frame, hx, hy, peer);
  }

  if (!tmap_frame->positioned){
    int hx=0, hy=0; //default
    if (cltset2_restored){ if (cltset2.tmap_offset_x!=32767){
      hx=cltset2.tmap_offset_x; hy=cltset2.tmap_offset_y;
      cltset2.tmap_offset_x=32767;
    }}
    placeFloatingPanelFirstShow(tmap_frame, hx, hy, tmap);
  }

  for (i=0;i<=7;i++){


    if (CLIENTplayer->party[i]!=NULL){

      pmf=party_frame[i]; //shortcut

      if (pmf->offset_x==4096){
        pmf->offset_x=i*32; pmf->offset_y=128-16+i*32; //default
        if (cltset2_restored){ if (cltset2.party_frame_offset_x[i]!=32767){
          pmf->offset_x=cltset2.party_frame_offset_x[i]; pmf->offset_y=cltset2.party_frame_offset_y[i];
          cltset2.party_frame_offset_x[i]=32767;
        }}
      }

	  // r222 if we want to be able to move the party member frame offscreen, we may need to do something here. no changes are made here.
		if (pmf->offset_x>=kPanelHideThresholdX) {
			pmf->offset_x-=kPanelHideDeltaX;
		}

      tnpc=(npc*)CLIENTplayer->party[i]->more; //shortcut

	  // s666 update party frame if needed
      //if (tnpc->upflags){ //upflags
      if (tnpc->upflags || updatepartyframen1){ //upflags
		updatepartyframen1 = 0;

		  // r222 no changes are made here; but it's possible to scale (bigger or smaller) the party member frames by mapping it onto another (different size) surface.
		  //pspartyorg = party_frame[i]->graphic;
		  //party_frame[i]->graphic = pspartytemp;
		  //updatepartyframe1(party_frame[i], i, partyresscale);


        img(party_frame[i]->graphic,status8); //clear frame


		// s666
		txtfnt = fnt1;

        img0(party_frame[i]->graphic,0,0,getportrait_doublesize(tnpc->port));


        txtset(t,tnpc->name);
        x=56-4*t->l; y=128-16-8;
        txtcol=rgb(0,0,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,255,255);
        if (i==0) txtcol=namecolour;
        txtout(party_frame[i]->graphic,x+1,y+1,t);
        txtcol=rgb(255,255,255);

        txtset(t,"Health");
        x=128-16; y=12-2;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        x2=tnpc->hp&32767;
        txtnumint(t,x2);
        x=128-16+64-8+4-2; y=12-2;
        if (x2<100) x=x+10;
        if (x2<10) x=x+10;


        txtcol=rgb(168,30,24);

        if (tnpc->hp&32768) txtcol=rgb(24,168,24);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,64,32);
        if (tnpc->hp&32768) txtcol=rgb(64,255,64);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"/");
        txtnumint(t2,tnpc->hp_max);
        txtadd(t,t2);
        x=128-16+64-8+4+32-3; y=12-2;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"Magic");
        x=128-16; y=12-2+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);



        txtnumint(t,tnpc->mp);
        x=128-16+64-8+4-2; y=12-2+18;
        if (tnpc->mp<100) x=x+10;
        if (tnpc->mp<10) x=x+10;

        txtcol=rgb(72,72,168);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(96,128,255);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"/");
        txtnumint(t2,tnpc->mp_max);
        txtadd(t,t2);
        x=128-16+64-8+4+32-3; y=12-2+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"STR  ");
        txtnumint(t2,tnpc->s);
        txtadd(t,t2);
        x=128-16; y=12+16+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"DEX  ");
        txtnumint(t2,tnpc->d);
        txtadd(t,t2);
        x=128-16+64+8-4; y=12+16+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"INT  ");
        txtnumint(t2,tnpc->i);
        txtadd(t,t2);
        x=128-16; y=12+16+18+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

        txtset(t,"LEV  ");
        txtnumint(t2,tnpc->lev);
        txtadd(t,t2);
        x=128-16+64+8-4; y=12+16+18+18;
        txtcol=rgb(192,126,0);
        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);


        txtset(t,"EXP ");
        txtnumint(t2,tnpc->exp);
        txtadd(t,t2);

		// c111 exp on screen
		/*
        x=800;
        if (tnpc->lev>=2) x=1600;
        if (tnpc->lev>=3) x=3200;
        if (tnpc->lev>=4) x=6400;
        if (tnpc->lev>=5) x=12800;
        if (tnpc->lev>=6) x=25600;
        if (tnpc->lev>=7) x=51200;
        if (tnpc->lev>=8) goto skiplevnext;
		*/
		if (easymodehostn1) {
			x = new1_getexprequired(tnpc);
		}
		else { // original
			x=800;
			if (tnpc->lev>=2) x=1600;
			if (tnpc->lev>=3) x=3200;
			if (tnpc->lev>=4) x=6400;
			if (tnpc->lev>=5) x=12800;
			if (tnpc->lev>=6) x=25600;
			if (tnpc->lev>=7) x=51200;
			if (tnpc->lev>=8) goto skiplevnext;
		}

        txtset(t2,"/");
        txtadd(t,t2);
        txtnumint(t2,x);
        txtadd(t,t2);




skiplevnext:


        x=128-16; y=12+16+18+18+18;
        txtcol=rgb(192,126,0);

		// s666 fix for experience numbers getting cut off (too big)
		if (enhancen1 >= 2 && tnpc->exp > 9999) {
			txtfntoldn1 = txtfnt;

			if (tnpc->exp > 999999) {
				txtfnt = fnt2;
				x += 1;
				y += 3;
			} else
				txtfnt = fnt4;
		}

        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(255,128+64+16,0);
        txtout(party_frame[i]->graphic,x+1,y+1,t);

		// s666 fix for experience numbers getting cut off (too big)
		if (enhancen1 >= 2 && tnpc->exp > 9999) {
			txtfnt = txtfntoldn1;
		}



        txtset(t,"E:");
        txtnumint(t2,tnpc->wt2/100); txtadd(t,t2);
        txtset(t2,"/"); txtadd(t,t2);
        txtnumint(t2,tnpc->wt2_max/100); txtadd(t,t2);

        x=128-16; y=12+16+18+18+18+18;
        txtcol=rgb(192,126,0);

        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(252,244,192);
        txtout(party_frame[i]->graphic,x+1,y+1,t);



        txtset(t,"I:");
        txtnumint(t2,tnpc->wt/100); txtadd(t,t2);
        txtset(t2,"/"); txtadd(t,t2);
        txtnumint(t2,tnpc->wt_max/100); txtadd(t,t2);

        x=128-16+64+8-4; y=12+16+18+18+18+18;
        txtcol=rgb(192,126,0);

        txtout(party_frame[i]->graphic,x,y,t);
        txtout(party_frame[i]->graphic,x+2,y+2,t);
        txtout(party_frame[i]->graphic,x+2,y,t);
        txtout(party_frame[i]->graphic,x,y+2,t);
        txtcol=rgb(252,244,192);
        txtout(party_frame[i]->graphic,x+1,y+1,t);




        spellbook_flags[i]=0;
        if (spellbook_page[i]){
          img(party_spellbook_frame[i]->graphic,0,0,spellbook);

          img0(party_spellbook_frame[i]->graphic,212-28,0,getportrait_halfsize(tnpc->port));


          i3=spellbook_page[i]-1;
          //port

          //name
          txtset(t,tnpc->name);
          x=132; y=4;
          txtcol=rgb(8,8,8);
          txtout(party_spellbook_frame[i]->graphic,x,y,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
          txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
          txtcol=rgb(255,255,255);
          if (i==0) txtcol=namecolour;
          txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);
          txtcol=rgb(255,255,255);

          //spellcircle
          x=(32-spellcircle[i3]->d.dwHeight)>>1;
          img0(party_spellbook_frame[i]->graphic,115-spellcircle[i3]->d.dwWidth,x,spellcircle[i3]);

          txtset(t,"LEV ?");
          t->d2[4]=49+i3;
          x=33+4; y=4;
          txtcol=rgb(8,8,8);
          txtout(party_spellbook_frame[i]->graphic,x,y,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
          txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
          txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
          txtcol=rgb(255,255,255);
          txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);


          //do we need dogears??

          if (i3){
            i4=0;
            for (i5=0;i5<(i3<<4);i5++){
              if (spell[i][i5]) i4=1;
            }
            if (i4){
              img(party_spellbook_frame[i]->graphic,20,2,dogearl);
              spellbook_flags[i]|=1;
            }
          }

          if (i3!=7){
            i4=0;
            for (i5=((i3+1)<<4);i5<=255;i5++){
              if (spell[i][i5]) i4=1;
            }
            if (i4){
              img(party_spellbook_frame[i]->graphic,212,2,dogearr);
              spellbook_flags[i]|=2;
            }
          }





          i4=0; i5=0; //i5=side! //i4=vertical line
          for (i2=0;i2<=15;i2++){
            if (spell[i][(i3<<4)+i2]){ //valid
              if (i5) img(party_spellbook_frame[i]->graphic,130,28+i4*24+2+1,spellbookline); else img(party_spellbook_frame[i]->graphic,20-2,28+i4*24+2+1,spellbookline);
              i4++; if (i4==5) {i4=0; i5++;}
            }
          }

          i4=0; i5=0; //i5=side! //i4=vertical line
          for (i2=0;i2<=15;i2++){
            if (spell[i][(i3<<4)+i2]){ //valid

              if (spellicon[(i3<<4)+i2]){
                if (i5==0) img0(party_spellbook_frame[i]->graphic,112-20-spellicon[(i3<<4)+i2]->d.dwWidth,28+i4*24+24-spellicon[(i3<<4)+i2]->d.dwHeight+5,spellicon[(i3<<4)+i2]);
                if (i5) img0(party_spellbook_frame[i]->graphic,225-20-spellicon[(i3<<4)+i2]->d.dwWidth,28+i4*24+24-spellicon[(i3<<4)+i2]->d.dwHeight+5,spellicon[(i3<<4)+i2]);
              }//icon

              txtfnt=fnt4;
              txtset(t,spellname[(i3<<4)+i2]);
              x=18; y=28+i4*24-2;
              if (i5) x=130;
              txtcol=rgb(8,8,8);
              txtout(party_spellbook_frame[i]->graphic,x,y,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
              txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
              txtcol=rgb(255,255,255);
              txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);

              txtfnt=fnt1;
              txtnumint(t,spell[i][(i3<<4)+i2]-1);
              x=112-8*t->l; y=28+i4*24-2;
              if (i5) x=225-8*t->l;

              txtcol=rgb(72,72,168);
              txtout(party_spellbook_frame[i]->graphic,x,y,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
              txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
              txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
              txtcol=rgb(96,128,255);
              txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);

              txtfnt=fnt3;
              for (x2=0;x2<=7;x2++){
                if (spellrecall_partymember[x2]==(i+1)){
                  if (spellrecall_i[x2]==((i3<<4)+i2)){

                    txtset(t,"F"); txtnumint(t2,x2+5); txtadd(t,t2);

                    x=18; y=28+i4*24-2;



                    x=18+48; y=28+i4*24-2+8;


                    if (i5) x=130;

                    txtcol=rgb(8,8,8);
                    txtout(party_spellbook_frame[i]->graphic,x,y-1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y-1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y-1,t);
                    txtout(party_spellbook_frame[i]->graphic,x,y+3,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y+3,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y+3,t);
                    txtout(party_spellbook_frame[i]->graphic,x-1,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x-1,y+1,t);
                    txtout(party_spellbook_frame[i]->graphic,x-1,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x+3,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+3,y+1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+3,y+2,t);
                    txtcol=rgb(0,96+32,96+32);
                    txtout(party_spellbook_frame[i]->graphic,x,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y+1,t);
                    txtout(party_spellbook_frame[i]->graphic,x+2,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x,y+2,t);
                    txtout(party_spellbook_frame[i]->graphic,x,y+1,t);
                    txtcol=rgb(0,192,192);
                    txtout(party_spellbook_frame[i]->graphic,x+1,y+1,t);


                  }
                }
              }
              txtfnt=fnt1;


              i4++; if (i4==5) {i4=0; i5++;}
            }
          }//i2

          txtfnt=fnt1;





        }//display spellbook!


        //clear spellbook buttons

        qkstf_update=TRUE;
        tnpc->upflags=0; //clear
      }

      //player inventory
      myobj=(object*)tnpc->baseitem;
      myobj=(object*)myobj->more;
      x=120; y=120; x3=tnpc->baseitem_offset;
      y3=0;
dni:
      if (myobj!=NULL){
        if (x3>0) {x3--;goto skipbio1;}
        getspr(myobj);

        if (not4sale_flags[i]&(1<<y3)) img0(party_frame[i]->graphic,x+14,y+8,not4salemask);

        img0(party_frame[i]->graphic,x,y,bt32);


        if (not4sale_flags[i]&(1<<y3)) imgt0(party_frame[i]->graphic,x+14,y+8,not4sale);
        y3++;

        //display quantity & key numbers
        if ((myobj->type&1023)==64){ //key (1=A, etc.)
          x2=myobj->more2;
          if (x2<=26){txtset(t,"?"); t->d2[0]=x2+64; goto keyjmp;}
          txtset(t,"??"); t->d2[1]=(x2%26)+64; t->d2[0]=(x2/26)+64; goto keyjmp;
        }
        else if (obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].flags&4096){
          txtnumint(t,myobj->more2);
keyjmp:
          txtfnt=fnt2;
          txtcol=rgb(8,8,8);


          x2=x+2;
          if (t->l==1) x2=x+12;
          if (t->l==2) x2=x+8;
          if (t->l==3) x2=x+4;

          txtout(party_frame[i]->graphic,x2,y+32-12,t);
          txtfnt=fnt1;
        }
        //display item +
		 else if ((obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v1 ||
          obji[sprlnk[myobj->type&1023]+(myobj->type>>10)].v2 || (myobj->type&1023)==448) && myobj->more2) {
            x2=x+16;
            y2=y+2;
            txtset(t,"+");
            txtnumint(t2,myobj->more2);
            txtadd(t,t2);

            txtfnt=fnt3; txtcol=0;
            txtout(party_frame[i]->graphic,x2,y2-1,t);
            txtout(party_frame[i]->graphic,x2-1,y2,t);
            txtout(party_frame[i]->graphic,x2+1,y2,t);
            txtout(party_frame[i]->graphic,x2,y2+1,t);
            txtout(party_frame[i]->graphic,x2-1,y2-1,t);
            txtout(party_frame[i]->graphic,x2+1,y2-1,t);
            txtout(party_frame[i]->graphic,x2+1,y2+1,t);
            txtout(party_frame[i]->graphic,x2-1,y2+1,t);
            txtcol=rgb(32,255,32);
            txtout(party_frame[i]->graphic,x2,y2,t);
            txtfnt=fnt1;
        }
        x=x+32; if (x==248) {x=120;y+=32;}
        if (y==248){
          if (myobj->next!=NULL) img0(party_frame[i]->graphic,112-1,128-8-2+32+16+60+2,darrow);
          goto diskip;
        }
        if (myobj->next!=NULL){
skipbio1:
          myobj=(object*)myobj->next;
          goto dni;
        }
      }
diskip:

      //display up arrow as required in inventory
      if (tnpc->baseitem_offset>0) img0(party_frame[i]->graphic,112-1,128-8-2+32+16+60+2-32+2-1,uarrow);


      //player readied items
      if (tnpc->helm!=NULL){
        myobj=tnpc->helm;
        getspr(myobj);
        img0(party_frame[i]->graphic,helmx,helmy,bt32);
      }
      if (tnpc->wep_right!=NULL){
        myobj=tnpc->wep_right;
        getspr(myobj);
        img0(party_frame[i]->graphic,wep_rightx,wep_righty,bt32);
        if (obji[sprlnk[tnpc->wep_right->type&1023]+(tnpc->wep_right->type>>10)].flags&16){
          img0(party_frame[i]->graphic,wep_leftx+8,wep_lefty+8,dhno);
        }
      }
      if (tnpc->wep_left!=NULL){
        myobj=tnpc->wep_left;
        getspr(myobj);
        img0(party_frame[i]->graphic,wep_leftx,wep_lefty,bt32);
        if (obji[sprlnk[tnpc->wep_left->type&1023]+(tnpc->wep_left->type>>10)].flags&16){
          img0(party_frame[i]->graphic,wep_rightx+8,wep_righty+8,dhno);
        }
      }
      if (tnpc->armour!=NULL){
        myobj=tnpc->armour;
        getspr(myobj);
        img0(party_frame[i]->graphic,armourx,armoury,bt32);
      }
      if (tnpc->boots!=NULL){
        myobj=tnpc->boots;
        getspr(myobj);
        img0(party_frame[i]->graphic,bootsx,bootsy,bt32);
      }
      if (tnpc->neck!=NULL){
        myobj=tnpc->neck;
        getspr(myobj);
        img0(party_frame[i]->graphic,neckx,necky,bt32);
      }
      if (tnpc->ring_right!=NULL){
        myobj=tnpc->ring_right;
        getspr(myobj);
        img0(party_frame[i]->graphic,ring_rightx,ring_righty,bt32);
      }
      if (tnpc->ring_left!=NULL){
        myobj=tnpc->ring_left;
        getspr(myobj);
        img0(party_frame[i]->graphic,ring_leftx,ring_lefty,bt32);
      }


      if (tnpc->baseitem!=tnpc->items){
        getspr(tnpc->baseitem);
        img(bt16,bt32);
        img0(party_frame[i]->graphic,90,123+8,bt32);
      }




    } //party member active?
  } //0-7 for/next


	// r222 for new mode: display party member frame at the top right of window, outside the playing area.
	//party_frame[i]->graphic = pspartyorg;
	//img(pspartyorg, pspartytemp);
	//refresh(pspartyorg);

	if (smallwindow && windowsizecyclenum == 1) {
		//txtset(t, "2 img..........");
		//LOGadd(t);
		//img(psnew1b, statusbar_b255);

		// r999 fill background of panel (solid blue); can be replaced with image/graphics
  		///img(panelnew[panelsideui].graphic, statusbar_b255);

		/*
		// r999 fill background of panel (solid blue); can be replaced with image/graphics
		img(panelsurf[0], statusbar_b255);

		for (int i = 0; i < partyframenewmax; i++) {
			//img(psnew1b, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y + 260, treasuremap);
			//img(psnew1b, 1000, 0, intro_startup);
			//party_frame[i]->offset_x = 4000;
			//img(psnew1b, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
			//img(psnew1b, resxn1m + 3, 300, minimap_frame->graphic);

			//refresh(psnew1b);
			//refresh(party_frame[i]->graphic);

			//party_frame[i]->offset_x = 4000;
			//party_frame[i]->offset_y = 0;

			//party_frame_new[i]->offset_x = resxn1m + 3;
			//party_frame_new[i]->offset_y = 0;

			//img(psnew1b, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
			img(panelsurf[0], party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
	 		//img(panelnew[panelsideui].graphic, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
		}




		// r666 for new mode: display actionbar
		//img(psnew1b, actionbarx, actionbary, actionbarsurf);
		//img(psnew1b, actiontalkx, actiontalky, actiontalksurf);
		for (i = 0; i < ACTIONBAR_MAX; i++)
		//img(psnew1b, actionbarx[i], actionbary[i], actionbarsurf[i]);
		img(panelsurf[0], actionbarx[i], actionbary[i], actionbarsurf[i]);

		for (i = 0; i < ACTIONTALKBAR_MAX; i++)
		//img(psnew1b, actiontalkx[i], actiontalky[i], actiontalksurf[i]);
		img(panelsurf[0], actiontalkx[i], actiontalky[i], actiontalksurf[i]);

 		//img(panelnew[panelsideui].graphic, panelnew[panelactionbar1].offset_x, panelnew[panelactionbar1].offset_y, panelnew[panelactionbar1].graphic);
 		//img(panelnew[panelsideui].graphic, panelnew[panelactionbar2].offset_x, panelnew[panelactionbar2].offset_y, panelnew[panelactionbar2].graphic);

		// r888 for new mode: update food status
		if (foodstatus == 1)
		//img(psnew1b, actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][1]);
		img(panelsurf[0], actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][1]);
		else if (foodstatus >= 2)
		//img(psnew1b, actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][2]);
		img(panelsurf[0], actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][2]);

		// r777 display proper drop location action button
		if (setdroplocation)
			//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][0]);
			img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][0]);
		else {
			if (droplocation == 1)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][1]);
				img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][1]);
			else if (droplocation == 2)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][2]);
				img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][2]);
			else if (droplocation == 3)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][3]);
				img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][3]);
			else if (droplocation == 4)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][4]);
				img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][4]);
		}

		// r444 for new mode: display minimap
		//minimap tiles its done in a VERY stupid way, but looks like the asm code doesn't like other size surfaces, so this will have to do for now.
//			if(peer) {
		if (minimaptype != 0) {
		for (x5=minimaptilexstart;x5<minimaptilexend;x5++) { for (y5=minimaptileystart;y5<minimaptileyend;y5++) {
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
			//img(ps6,ps5);
			//img0(minimap_frame->graphic,8+60*x5,8+60*y5,ps6);

			img(minimaptilesurf,ps5);

			if (minimaptype == 1) {
				if (x5 == 1)
					minimapdeltax = -60;
				else
					minimapdeltax = 0;

				if (y5 == 1)
					minimapdeltay = -60;
				else
					minimapdeltay = 0;

//				if ( ((x5 > 0) && (x5 < 3)) && ((y5 > 0) && (y5 < 3)) )
				//img0(minimap_frame->graphic,8+60*x5+minimapdeltax,8+60*y5+minimapdeltay,minimaptilesurf);
				img0(minimap_surf_new,8+60*x5+minimapdeltax,8+60*y5+minimapdeltay,minimaptilesurf);
			}
			else {
				//img0(minimap_frame->graphic,8+60*x5,8+60*y5,minimaptilesurf);
				img0(minimap_surf_new,8+60*x5,8+60*y5,minimaptilesurf);
			}

		}}
//			}

		//minimapdeltax = party_frame_new[0]->offset_x;
		//minimapdeltay = resyn1w-256-2;

		//img(psnew1b, party_frame_new[0]->offset_x, resyn1w-256-2, minimap_frame->graphic);
		//img(psnew1b, minimapdeltax, minimapdeltay, minimap_frame->graphic);
		//img(psnew1b, minimapdeltax, minimapdeltay, minimap_surf_new);

		//img(psnew1b, minimapnewx, minimapnewy, minimap_surf_new);
		img0(minimap_surf_new, minimapplayerx, minimapplayery, darrow);
		img(panelsurf[0], minimapnewx, minimapnewy, minimap_surf_new);


		//img0(psnew1b, party_frame_new[0]->offset_x+minimapdeltax, resyn1w-256-2+minimapdeltay, darrow);
		//img0(psnew1b, minimapdeltax, minimapdeltay, darrow);
		//img0(psnew1b, minimapplayerx, minimapplayery, darrow);
		}

		//img(psnew1b, panelx[0], panely[0], panelsurf[0]);
		//img(psnew1b, panelsurf[0], panelx[0], panely[0], panelx2[0], panely2[0]);
		img(psnew1b, panelx[0], panely[0], panelsurf[0]);
		//img(psnew1b, panelnew[panelsideui].offset_x, panelnew[panelsideui].offset_y, panelnew[panelsideui].graphic);
		//img(psnew1b, panelnew[panelminimap].offset_x, panelnew[panelminimap].offset_y,panelnew[panelminimap].graphic);
	*/


		// s777 reset selected party member if it's not valid anymore (party member asked to leave, etc.).
		if (CLIENTplayer->party[selectedpartymembern1] == NULL) {
			selectedpartymembern1 = 0;
		}

		// s777 reset selected party member frame if it's not valid anymore (party member asked to leave, etc.).
		if (CLIENTplayer->party[selectedpartymemberframen1] == NULL) {
			selectedpartymemberframen1 = selectedpartymembern1;
		}

		// r999 new
		// r999 fill background of panel (solid blue); can be replaced with image/graphics
		//img(panelsurf[0], statusbar_b255);
		//img(uipanelsurf[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF], statusbar_b255);
		//img(psnew1b, uipanelx[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF], uipanely[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF], uipanelsurf[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF]);
		imguip(psnew1b, uipanelsidebar);
		if (uiscaling) {
			//img(uipanelsurf[uipanelpartymember0][UI_WIDGET_DEF][UI_STATE_DEF], party_frame[0]->graphic);
			img(uipanelsurf[uipanelpartymember0][UI_WIDGET_DEF][UI_STATE_DEF], party_frame[selectedpartymemberframen1]->graphic);
			imguip(psnew1b, uipanelpartymember0);
		} else
		//imguip(psnew1b, uipanelpartymember0, party_frame[0]->graphic);
		imguip(psnew1b, uipanelpartymember0, party_frame[selectedpartymemberframen1]->graphic);


		///for (int i = 0; i < partyframenewmax; i++) {
			//img(psnew1b, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y + 260, treasuremap);
			//img(psnew1b, 1000, 0, intro_startup);
			//party_frame[i]->offset_x = 4000;
			//img(psnew1b, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
			//img(psnew1b, resxn1m + 3, 300, minimap_frame->graphic);

			//refresh(psnew1b);
			//refresh(party_frame[i]->graphic);

			//party_frame[i]->offset_x = 4000;
			//party_frame[i]->offset_y = 0;

			//party_frame_new[i]->offset_x = resxn1m + 3;
			//party_frame_new[i]->offset_y = 0;

			//img(psnew1b, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
			//img(panelsurf[0], party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
	 		//img(panelnew[panelsideui].graphic, party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
			//img(panelsurf[0], party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
			///img(panelsurf[0], party_frame_new[i]->offset_x, party_frame_new[i]->offset_y, party_frame[i]->graphic);
		///}


		// s777 display party member lock
		if (selectedpartymemberframelock)
			imguiw(psnew1b, uipanelpartymemberlock, 1, 2);
		else
			imguiw(psnew1b, uipanelpartymemberlock, 1, 1);

		// s777 display party member bar
		imguip(psnew1b, uipanelpartymemberbar1);
		for (n1i1 = 0; n1i1 < 8; n1i1++) {
			if (CLIENTplayer->party[n1i1] != NULL) {
				uipanelhitenable[uipanelpartymemberbar1][n1i1+1][UI_STATE_DEF] = 1;
				n1i2 = 1;

				if (n1i1 == selectedpartymembern1)
					n1i2+=2;

				if (n1i1 == selectedpartymemberframen1)
					n1i2++;

				imguiw(psnew1b, uipanelpartymemberbar1, n1i1+1, n1i2);
			} else
				uipanelhitenable[uipanelpartymemberbar1][n1i1+1][UI_STATE_DEF] = 0;
		}

		// r666 for new mode: display actionbar
		//img(psnew1b, uipanelx[uipanelactionbar1][UI_WIDGET_DEF][UI_STATE_DEF], uipanely[uipanelactionbar1][UI_WIDGET_DEF][UI_STATE_DEF], uipanelsurf[uipanelactionbar1][UI_WIDGET_DEF][UI_STATE_DEF]);
		//img(psnew1b, uipanelx[uipanelactionbar2][UI_WIDGET_DEF][UI_STATE_DEF], uipanely[uipanelactionbar2][UI_WIDGET_DEF][UI_STATE_DEF], uipanelsurf[uipanelactionbar2][UI_WIDGET_DEF][UI_STATE_DEF]);
		imguip(psnew1b, uipanelactionbar1);
		imguip(psnew1b, uipanelactionbar2);
		imguip(psnew1b, uipaneloptionbar1);

		//img(psnew1b, uipanelx[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF], uipanely[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF], uipanelsurf[uipanelactiontalkbar1][UI_WIDGET_DEF][UI_STATE_DEF]);
		//img(psnew1b, uipanelx[uipanelactiontalkbar2][UI_WIDGET_DEF][UI_STATE_DEF], uipanely[uipanelactiontalkbar2][UI_WIDGET_DEF][UI_STATE_DEF], uipanelsurf[uipanelactiontalkbar2][UI_WIDGET_DEF][UI_STATE_DEF]);
		imguip(psnew1b, uipanelactiontalkbar1);
		imguip(psnew1b, uipanelactiontalkbar2);
		imguip(psnew1b, uipanelactiontalkbar3);

		//imguip(psnew1b, uipaneloptioninfo);

 		//img(panelnew[panelsideui].graphic, panelnew[panelactionbar1].offset_x, panelnew[panelactionbar1].offset_y, panelnew[panelactionbar1].graphic);
 		//img(panelnew[panelsideui].graphic, panelnew[panelactionbar2].offset_x, panelnew[panelactionbar2].offset_y, panelnew[panelactionbar2].graphic);

		// r888 for new mode: update food status
		if (foodstatus == 1)
		//img(psnew1b, actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][1]);
		//img(panelsurf[0], actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][1]);
		imguiw(psnew1b, uipanelactionbar2, UI_WIDGET_ACTIONBUTTON_FOOD, 1);
		else if (foodstatus >= 2)
		//img(psnew1b, actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][2]);
		//img(panelsurf[0], actionbuttonx[1][ACTIONBUTTON_FOOD], actionbuttony[1][ACTIONBUTTON_FOOD], actionbuttonsurf[ACTIONBUTTON_FOOD][2]);
		imguiw(psnew1b, uipanelactionbar2, UI_WIDGET_ACTIONBUTTON_FOOD, 2);
		//img(psnew1b, uipanelx[uipanelactionbar2][UI_WIDGET_ACTIONBUTTON_FOOD][UI_STATE_DEF], uipanely[uipanelactionbar2][UI_WIDGET_ACTIONBUTTON_FOOD][UI_STATE_DEF], uipanelsurf[uipanelactionbar2][0][0]);

		// r777 display proper drop location action button
		if (setdroplocation)
			//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][0]);
			//img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][0]);
			imguiw(psnew1b, uipanelactionbar1, UI_WIDGET_ACTIONBUTTON_DROP, 1);
		else {
			if (droplocation == 1)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][1]);
				//img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][1]);
				imguiw(psnew1b, uipanelactionbar1, UI_WIDGET_ACTIONBUTTON_DROP, 2);
			else if (droplocation == 2)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][2]);
				//img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][2]);
				imguiw(psnew1b, uipanelactionbar1, UI_WIDGET_ACTIONBUTTON_DROP, 3);
			else if (droplocation == 3)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][3]);
				//img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][3]);
				imguiw(psnew1b, uipanelactionbar1, UI_WIDGET_ACTIONBUTTON_DROP, 4);
			else if (droplocation == 4)
				//img(psnew1b, actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][4]);
				//img(panelsurf[0], actionbuttonx[0][ACTIONBUTTON_DROP], actionbuttony[0][ACTIONBUTTON_DROP], actionbuttonsurf[ACTIONBUTTON_DROP][4]);
				imguiw(psnew1b, uipanelactionbar1, UI_WIDGET_ACTIONBUTTON_DROP, 5);
		}

		// r999 new actionbar2 states
		if (showworldmapn1 > 0) { // s444 worldmap action button
			imguiw(psnew1b, uipanelactionbar2, UI_WIDGET_ACTIONBUTTON_WORLDMAP, showworldmapn1);
			//img0(psnew1b, uipanelx[uipanelactionbar2][UI_WIDGET_ACTIONBUTTON_WORLDMAP][UI_STATE_DEF], uipanely[uipanelactionbar2][UI_WIDGET_ACTIONBUTTON_WORLDMAP][UI_STATE_DEF], uiwidgetimgsurf[UI_IMGI_WIDGET + 13][1]);
		}

		if (combatinfo > 0)
		imguiw(psnew1b, uipanelactionbar2, UI_WIDGET_ACTIONBUTTON_COMBATLOG, combatinfo);

		// r999 new optionbar1 states
		if (enhancen1 > 0) {
			imguiw(psnew1b, uipaneloptionbar1, UI_WIDGET_OPTIONBUTTON_ENHANCE, enhancen1);
		}


		// r444 for new mode: display minimap
//			if(peer) {
		if (minimaptype > 0) {
		for (n1x5=minimaptilexstart;n1x5<minimaptilexend;n1x5++) { for (n1y5=minimaptileystart;n1y5<minimaptileyend;n1y5++) {
			for (n1y1=n1y5*24;n1y1<24+n1y5*24;n1y1++){ for (n1x1=n1x5*24;n1x1<24+n1x5*24;n1x1++){
				if (((n1y1+tpy-35)>=1024)||((n1y1+tpy-35)<0)||((n1x1+tpx-31)>=2048)||((n1x1+tpx-31)<0)) {
				n1i1=0;
				}
				else {
				n1i1=bt[n1y1+tpy-35][n1x1+tpx-31]&1023;
				}
				if ((n1i1>=8)&&(n1i1<48)){ //ocean and coast
				if (n1i1<=15) oceantiles++; else rivertiles++;
				n1i2=n1i1-8;
				n1x4=0; if (n1i2>=8) {n1i2=wateri[n1i2-8]; n1x4=1; }
				sf32(ps5,(n1x1-n1x5*24)*32,(n1y1-n1y5*24)*32,sfx8,n1i2+128);
				if (n1x4==1) g32z(ps5,(n1x1-n1x5*24)*32,(n1y1-n1y5*24)*32,bt8[0],n1i1);
				}else{//not ocean
				n1i2=0;
				if (n1i1==252){n1i1=14;}
				if (n1i1==253){n1i1=15;}
				if (n1i1==254){n1i1=0;}
				if ((n1i1>=221)&&(n1i1<=223)){n1i1=n1i1-210;}
				if ((n1i1>=217)&&(n1i1<=219)){n1i1=n1i1-209;}
				g32(ps5,(n1x1-n1x5*24)*32,(n1y1-n1y5*24)*32,bt8[0],n1i1);
				}
			}}
			//img(ps6,ps5);
			//img0(minimap_frame->graphic,8+60*x5,8+60*y5,ps6);

			img(minimaptilesurf,ps5);

			if (minimaptype == 1) {
				if (n1x5 == 1)
					minimapdeltax = -60;
				else
					minimapdeltax = 0;

				if (n1y5 == 1)
					minimapdeltay = -60;
				else
					minimapdeltay = 0;

//				if ( ((x5 > 0) && (x5 < 3)) && ((y5 > 0) && (y5 < 3)) )
				//img0(minimap_frame->graphic,8+60*x5+minimapdeltax,8+60*y5+minimapdeltay,minimaptilesurf);
				img0(minimap_surf_new,8+60*n1x5+minimapdeltax,8+60*n1y5+minimapdeltay,minimaptilesurf);
			}
			else {
				//img0(minimap_frame->graphic,8+60*x5,8+60*y5,minimaptilesurf);
				img0(minimap_surf_new,8+60*n1x5,8+60*n1y5,minimaptilesurf);
			}

		}}
//			}

		//minimapdeltax = party_frame_new[0]->offset_x;
		//minimapdeltay = resyn1w-256-2;

		//img(psnew1b, party_frame_new[0]->offset_x, resyn1w-256-2, minimap_frame->graphic);
		//img(psnew1b, minimapdeltax, minimapdeltay, minimap_frame->graphic);
		//img(psnew1b, minimapdeltax, minimapdeltay, minimap_surf_new);

		playeronscreenxn1 = tplay->x - tpx;
		playeronscreenyn1 = tplay->y - tpy;

		if (playeronscreenxn1 != 15) {
			minimapdeltax = (playeronscreenxn1 - 15) * minimapstepsize; // 4.9f;//(float)(128 / 61);
		} else
			minimapdeltax = 0;

		if (playeronscreenyn1 != 11) {
			minimapdeltay = (playeronscreenyn1 - 11) * minimapstepsize; //  4.9f;//(float)(128 / 61);
		} else
			minimapdeltay = 0;

		//img(psnew1b, minimapnewx, minimapnewy, minimap_surf_new);
		img0(minimap_surf_new, minimapplayerx+minimapdeltax, minimapplayery+minimapdeltay, darrow);
		//img(panelsurf[0], minimapnewx, minimapnewy, minimap_surf_new);
		if (uiscaling) {
			img(uipanelsurf[uipanelminimap][UI_WIDGET_DEF][UI_STATE_DEF], minimap_surf_new);
			imguip(psnew1b, uipanelminimap);
		} else
			imguip(psnew1b, uipanelminimap);

		imguiw(psnew1b, uipaneloptionbar1, UI_WIDGET_OPTIONBUTTON_MINIMAP, 1);


		//img0(psnew1b, party_frame_new[0]->offset_x+minimapdeltax, resyn1w-256-2+minimapdeltay, darrow);
		//img0(psnew1b, minimapdeltax, minimapdeltay, darrow);
		//img0(psnew1b, minimapplayerx, minimapplayery, darrow);
		}

		// r999 hover
		if (uihover) {
			if (hituipaneli < -4)
				hituipaneli = gethituipaneli(omx3, omy3);

			if ((hituipaneli == uipanelactionbar1) || (hituipaneli == uipanelactionbar2) || (hituipaneli == uipaneloptionbar1)) {
				if (hituiwidgeti < 0)
					hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

				if (hituiwidgeti > 0)
					img0(psnew1b, uipanelx[hituipaneli][hituiwidgeti][UI_STATE_DEF], uipanely[hituipaneli][hituiwidgeti][UI_STATE_DEF], uihoveractionbuttonsurf);
			} else if ((hituipaneli == uipanelactiontalkbar1) || (hituipaneli == uipanelactiontalkbar2) || (hituipaneli == uipanelactiontalkbar3)) {
				if (hituiwidgeti < 0)
					hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

				if (hituiwidgeti > 0)
					img0(psnew1b, uipanelx[hituipaneli][hituiwidgeti][UI_STATE_DEF], uipanely[hituipaneli][hituiwidgeti][UI_STATE_DEF], uihoveractiontalkbuttonsurf);
			} else if ((hituipaneli == uipanelpartymemberbar1)) {
				if (hituiwidgeti < 0)
					hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

				if (hituiwidgeti > 0)
					img0(psnew1b, uipanelx[hituipaneli][hituiwidgeti][UI_STATE_DEF], uipanely[hituipaneli][hituiwidgeti][UI_STATE_DEF], uihoverpartymemberbuttonsurf);
			}
		}

		//img(psnew1b, panelx[0], panely[0], panelsurf[0]);
		//img(psnew1b, panelsurf[0], panelx[0], panely[0], panelx2[0], panely2[0]);
		//img(psnew1b, panelnew[panelsideui].offset_x, panelnew[panelsideui].offset_y, panelnew[panelsideui].graphic);
		//img(psnew1b, panelnew[panelminimap].offset_x, panelnew[panelminimap].offset_y,panelnew[panelminimap].graphic);
		//img(psnew1b, panelx[0], panely[0], panelsurf[0]);
		//img(psnew1b, uipanelx[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF], uipanely[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF], uipanelsurf[uipanelsidebar][UI_WIDGET_DEF][UI_STATE_DEF]);

	}
