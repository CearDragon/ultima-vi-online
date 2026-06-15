// LCS-P2.4: part_refresh_tail — TAIL chunk of the former loop_client.cpp
// monolith (former lines ~12676..13074, through EOF). The shared
// `intro_refresh:` refresh block, MIDI/WAV info loops (midiinfo_next:,
// wavinfo_next:), status-message timing, and the font-leak workaround. MUST
// remain the LAST include in loop_client_all.cpp: `intro_refresh:` is the jump
// target for both the intro state machine and the in-game block, and the block
// ends with `goto mainloop` (exit to u6o7.cpp). Pure relocation — do not edit.
// See docs/plans/plan-loopClientSplit.md.
intro_refresh : refreshcount
++;


if
((gotfocus
==
FALSE
)
&&
(dxrefresh)
)
goto
skiprefresh;

refresh();

skiprefresh : mb
=
FRAME_mb; //restore physical mouse values
omx=mx;
omy=my; //set old mouse values
isit=FALSE;

//BACKGROUND/FOREGROND MIDI
x=
0; //set volume
if
(midiinfo_loaded) {
    if (u6omidivolume) {
        //midi cannot be processed if volume==NULL

        if (midi_foreground != -1) {
            //set midi_foreground as loaded midi
            if (midi_foreground != midi_loaded) {
                midi_loaded = midi_foreground;
                u6omidi->LoadMidiFromFile(u6omidi_filename[midi_loaded]->d, TRUE);
                if (midipause == 0) {
                    u6omidi->Play();
                isplayingwait0:
                    if (u6omidi->IsPlaying() == S_FALSE) goto isplayingwait0;
                } //midipause==0
                x = 1;
            } //midi_foreground!=midi_loaded
            midi_foreground = -1;
        } //midi_foreground!=-1

        if (midi_foreground_wait) {
            midi_foreground_wait -= et;
            if (midi_foreground_wait < 0.0f) midi_foreground_wait = 0.0f;
            if (u6omidi->IsPlaying() == S_FALSE) midi_foreground_wait = 0.0f; //midi finished playing
            if (midi_foreground_wait == 0.0f) {
                u6omidi->Stop(); //stop playing foreground midi
            isplayingwait2:
                if (u6omidi->IsPlaying() == S_OK) goto isplayingwait2;
            }
        }

        if (midi_foreground_wait == 0.0f) {
            //no foreground midi is playing
            if (midi_loaded != midi_background) {
                //load correct midi
                if (u6omidi->IsPlaying() == S_FALSE) {
                    //midi is not playing
                    midi_loaded = midi_background;
                    u6omidi->LoadMidiFromFile(u6omidi_filename[midi_loaded]->d, TRUE);
                }
            } //midi_loaded!=midi_background

            if (u6omidi->IsPlaying() == S_FALSE) {
                //midi is not playing
                if (midipause == 0) {
                    u6omidi->Play();
                isplayingwait1:
                    if (u6omidi->IsPlaying() == S_FALSE) goto isplayingwait1;
                } //midipause==0
                x = 1;
            } //==S_FALSE
        } //midi_foreground_wait==0.0f

        if (x) {
            f = u6omidi_volume[midi_loaded];
            f = f * (float) u6omidivolume / 255.0f;
            f = 255 - f;
            f = f * 0.25f;
            f *= f;
            //DMUS_VOLUME_MAX     2000         +20 dB
            //DMUS_VOLUME_MIN   -20000        -200 dB
            u6omidi->SetMasterVolume(-f);
        }
    } //u6omidivolume
} //midiinfo_loaded


for
(i=
0;
i
<=
7;
i
++
)
{ //mouse flag cancel
  pmf=party_frame[i]; pmf->mouse_pressed=FALSE; pmf->mouse_over=FALSE; pmf->mouse_click=FALSE;
  pmf=party_spellbook_frame[i]; pmf->mouse_pressed=FALSE; pmf->mouse_over=FALSE; pmf->mouse_click=FALSE;
}
vf
->
mouse_click=FALSE;
vf
->
mouse_over=FALSE;
vf
->
mouse_pressed=FALSE;
con_frm
->
mouse_over=FALSE;
inpf
->
mouse_over=FALSE;
volcontrol
->
mouse_over=FALSE;
qkstf
->
mouse_over=FALSE;
con_frm
->
mouse_over=FALSE;
musickeyboard
->
mouse_over=FALSE;
inpf
->
mouse_over=FALSE;


ZeroMemory (&key
,
65536
);

if
(mb_release) {
    if ((mb_release & 1) && (mb & 1)) mb -= 1;
    if ((mb_release & 2) && (mb & 2)) mb -= 2;
    mb_release = 0;
}

mbclick=
0;

if
(U6O_DISABLEMUSIC
==
FALSE
)
{
  if (midiinfo_loaded==FALSE){
    //load midiinfo.txt
    midiinfo_loaded=TRUE;
    txtset(t3," ");
    tfh=open(".\\midi\\midiinfo.txt");
midiinfo_next:
    txtfilein(t,tfh);
    if (t->l){
      txtucase(t);
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      i=txtnum(t2);
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      txtset(t4,".\\midi\\"); txtadd(t4,t2); txtadd(t4,".mid");
      if (u6omidi_filename[i]==NULL) u6omidi_filename[i]=txtnew();
      txtset(u6omidi_filename[i],t4);

      x2=100; //volume
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      if (t2->l){
        x2=txtnum(t2);
        if (x2<0) x=0;
        if (x2>100) x=100;
      }
      x2=(float)x2*2.55f;
      u6omidi_volume[i]=x2;


      goto midiinfo_next;
    }
    close(tfh);
    u6omidi->LoadMidiFromFile(u6omidi_filename[0]->d,TRUE);
  }//midiinfo_loaded
}

// s222 sound additions (no changes here; to add sounds, edit wavinfo.txt in wav subfolder)
if
(wavinfo_loaded
==
FALSE
)
{
  //load wavinfo.txt
  wavinfo_loaded=TRUE;
  u6osoundtype_volume[0]=255; //UNKNOWN type 100%
  txtset(t3," ");
  tfh=open(".\\wav\\wavinfo.txt");
wavinfo_next:
  txtfilein(t,tfh);
  if (t->l){
    txtucase(t);
    x4=0;
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    //check for catagory definitions
    txtset(t4,"COMBAT"); if (txtsame(t2,t4)) x4=1;
    txtset(t4,"FOREGROUND"); if (txtsame(t2,t4)) x4=2;
    txtset(t4,"BACKGROUND"); if (txtsame(t2,t4)) x4=3;
    txtset(t4,"BACKGROUND_CONTINUOUS"); if (txtsame(t2,t4)) x4=4;
    if (x4){
      if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
      f=txtnum(t2)*2.55f;
      x=f;
      if (x<0) x=0;
      if (x>255) x=255;
      u6osoundtype_volume[x4]=x;
      goto wavinfo_next;
    }
    i=txtnum(t2);
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    txtset(t4,".\\wav\\"); txtadd(t4,t2); txtadd(t4,".wav");
    if (u6osound[i]==0) u6osound[i]=soundload(t4->d);
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    u6osound_type[i]=0; //UNKNOWN
    txtset(t4,"COMBAT"); if (txtsame(t2,t4)) u6osound_type[i]=1;
    txtset(t4,"FOREGROUND"); if (txtsame(t2,t4)) u6osound_type[i]=2;
    txtset(t4,"BACKGROUND"); if (txtsame(t2,t4)) u6osound_type[i]=3;
    txtset(t4,"BACKGROUND_CONTINUOUS"); if (txtsame(t2,t4)) u6osound_type[i]=4;
    u6osound_volumechange[i]=0;
    if (x=txtsearch(t,t3)){txtset(t2,t); txtleft(t2,x-1); txtright(t,t->l-x);}else{txtset(t2,t); txtset(t,"");}
    if (t2->l){
      if (t2->d2[0]==43) txtright(t2,t2->l-1); //remove "+"
      x=txtnum(t2);
      if (x<-100) x=-100;
      if (x>100) x=100;
      u6osound_volumechange[i]=x;
    }//t->l
    f=u6osoundtype_volume[u6osound_type[i]];
    f2=u6osound_volumechange[i];
    f2=f2/100.0f+1.0f;
    x=f*f2;
    if (x<0) x=0;
    if (x>255) x=255;
    u6osound_volume[i]=x;
    goto wavinfo_next;
  }
  close(tfh);

}//wavinfo_loaded


if
(STATUSMESSwait) {
    STATUSMESSwait -= (et * (1.0f + (float) STATUSMESSpending->l * 0.005f));
    if (STATUSMESSwait <= 0.0f) {
        //add to the buffer
        if (statusmessage_loggedearly) {
            // f: this "look" message was already pushed to the log when it
            // first displayed; don't add it again, just clear the flag.
            statusmessage_loggedearly = 0;
        } else {
            for (i = 7; i >= 1; i--) {
                txtset(STATUSMESSprev[i], STATUSMESSprev[i - 1]);
            } //i
            txtset(STATUSMESSprev[0], STATUSMESSdisplaying);
        }
        STATUSMESSwait = 0.0f;
        txtset(STATUSMESSdisplaying, "");
    } //STATUSMESSwait<=0.0f
} //STATUSMESSwait

if
(STATUSMESSpending
->
l
)
{
	// f333 check pending status messages if new messages were added
	if (enhancen1) {
		if (STATUSMESSpending->l > statusmessagependingprevlen) {
			statusmessagependingprevlen = STATUSMESSpending->l;
			//checkpendingstatusmessage = 1;
		//}

			// f333 if there is a pending "look" status message, skip status messages until it is displayed
		//if (checkpendingstatusmessage) {
			//checkpendingstatusmessage = 0;
			txtset(t3, "Thou dost see");
			if (txtsearch(STATUSMESSpending, t3) > 0) {
				STATUSMESSskipok = 0;
				STATUSMESSwait = -1.0f;
			}
		}
	}


  if (STATUSMESSdisplaying->l==0){
    txtsetchar(t,13);
    if (i=txtsearch(STATUSMESSpending,t)){
      txtset(STATUSMESSdisplaying,STATUSMESSpending);
      txtleft(STATUSMESSdisplaying,i-1);
      txtright(STATUSMESSpending,STATUSMESSpending->l-i);
    }else{
      txtset(STATUSMESSdisplaying,STATUSMESSpending);
      txtset(STATUSMESSpending,"");
    }
    STATUSMESSwait=1.0f+(float)STATUSMESSdisplaying->l/16.0f;
    STATUSMESSskipok=0;
    if (STATUSMESSdisplaying->d2[0]==255){
      txtright(STATUSMESSdisplaying,STATUSMESSdisplaying->l-1);
      STATUSMESSskipok=1;
    }

	// f333 "look" status message will not skip unless there is another pending "look"
	if (enhancen1) {
		statusmessagechanged = 1;
		statusmessagependingprevlen = STATUSMESSpending->l;
		if (STATUSMESSskipok) {
			txtset(t3, "Thou dost see");
			if (txtsearch(STATUSMESSdisplaying, t3) > 0) {
				STATUSMESSskipok = 0;
			}
		} else {
			// f444 staff enchant message is skippable
			txtset(t3, "Staff successfully enchanted with");
			if (txtsearch(STATUSMESSdisplaying, t3) > 0) {
				STATUSMESSskipok = 1;
			}
		}

		// f: push "look" text into the status log the instant it starts
		// displaying, instead of waiting for the floating text to disappear.
		// statusmessage_loggedearly suppresses the duplicate add that the
		// timer-expiry path would otherwise perform for this same message.
		txtset(t3, "Thou dost see");
		if (txtsearch(STATUSMESSdisplaying, t3) > 0) {
			for (i = 7; i >= 1; i--) txtset(STATUSMESSprev[i], STATUSMESSprev[i - 1]);
			txtset(STATUSMESSprev[0], STATUSMESSdisplaying);
			statusmessage_loggedearly = 1;
		}
	}
  }else{
    if (STATUSMESSskipok){
      STATUSMESSskipok=0;
      STATUSMESSwait=0.01f;
    }
  }
}


if
(portraitlook_wait) {
    portraitlook_wait -= et;
    if (portraitlook_wait < 0.0f) {
        portraitlook_wait = 0.0f;
        if (statusmessage_viewnpc->offset_x < kPanelHideThresholdX) statusmessage_viewnpc->offset_x += kPanelHideDeltaX;
    }
}

keyon [0xD8]=FALSE;
keyon [0xD9]=FALSE; //release mousewheel "buttons"

//not a real fix and that is why command line parameter is needed to use this "hidden" "fix"
if
(leak) {
    DeleteObject(fnt1);
    DeleteObject(fnt1naa);
    DeleteObject(fnt2);
    DeleteObject(fnt3);
    DeleteObject(fnt4);
    DeleteObject(fnt5);
    DeleteObject(fnt6);
    DeleteObject(fnt7);
    fnt1 = CreateFont(22, NULL, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Avatar");
    fnt1naa = CreateFont(22, NULL, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                         NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Avatar");
    fnt2 = CreateFont(16, NULL, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Avatar");
    fnt3 = CreateFont(8, NULL, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Terminal");
    fnt4 = CreateFont(22, 8, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Avatar");
    fnt5 = CreateFont(22, NULL, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Ultima Runes");
    fnt6 = CreateFont(22, 0, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Gargish");
    fnt7 = CreateFont(24, 9, 0, 0, 0, NULL, NULL, NULL, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "PerryGothic");
}
