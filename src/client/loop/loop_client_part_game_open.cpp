// LCS-P4.2: part_game_open — FIRST chunk of the in-game per-frame block.
// LCS brace-seam: this file OPENS the in-game `{ ... }` block CLOSED in
// loop_client_part_panel_draw.cpp. Contains the block `{`, time/btime update,
// early per-frame setup, input/command handling, spell targeting, and
// active-player output message processing. Its trailing `} //...end of active
// player output message processing` leaves the in-game block OPEN — part_net
// continues it. Pure relocation — do not edit. See docs/plans/plan-loopClientSplit.md.

{


  //btime2+=et*0.00666667f; //non-rolled over btime!
  /*
  //luteijn: attempt 1, slow progress of time by 4 on the client (9600*framerate)
  if (NEThost) {
  btime2+=et/150.0; //non-rolled over btime!
  } else {
  btime2+=et/600.0; //non-rolled over btime!
  }
  */
  //luteijn: attempt 2, speed progress of time by 4 on the host.
  //would give /37.5 vs /150.0 doesn't look nice, so changed to 32 and 128

  if (!NEThost) {
    btime2+=et/150.0f; //non-rolled over btime!
    x=btime2/24;
    btime=btime2-(float)x*24.0f;
    x%=7; bday=x+1;
  }
  if ((btime_last<5.0f)&&(btime>=5.0f)) soundplay2(u6osound[28],u6osound_volume[28]);
  btime_last=btime;


  //set key frame
  i=ett*16.0; i=i&7; keyframe=i;
  i=ett*32.0; i=i&7; keyframe2=i;
  i=ett*4.0; i=i&15; keyframe15=i;
  i=ett*8.0; i=i&31; keyframe31=i; //4sec cycle! (8 frames per sec!)

  keyframe3=(unsigned long)(ett*24.0f)%10;

  if (u6okeyhit(U6OK_QUIT)){ //"Q" request exit
    exitrequest=TRUE;
  }

  if ((NEThost)&&(host_minimize)){
    // Programmatic minimize from host startup (or anywhere else that sets
    // the host_minimize flag). The actual `nodisplay` toggle is now driven
    // by WM_SIZE in WndProc, so all this needs to do is ask Windows to
    // minimize the window ? restore via the taskbar will resume the game.
    host_minimize=FALSE;
    ShowWindow(hWnd2,SW_MINIMIZE);
    for (i=0;i<=65535;i++) keyon[i]=FALSE;
  }
  // Option A follow-up (2026-05-20): the "M" / U6OK_MAXMIN hotkey is
  // removed entirely. Its previous behavior (cycle through window modes)
  // is meaningless now that only one mode exists, and the simplified
  // "minimize on M" version had a bug where pressing M to minimize froze
  // the game because nothing reset `nodisplay` on restore. Standard
  // window chrome (minimize button, taskbar) is the supported way to
  // minimize/restore ? that path is wired through WM_SIZE in WndProc.
  // The U6OK_MAXMIN keybinding in setup_client.inc is left in place but
  // is no longer consulted anywhere in the main loop.
maxminmini:


  if (u6okeyhit(U6OK_SOUND)){ //"S" sound on/off
    // (windowResize gate removed 2026-05-27 — always toggle the volume
    // panel visibility; the legacy slide-off-screen path is gone.)
    u6o::client::g_volcontrol_visible = !u6o::client::g_volcontrol_visible;
    RepositionAnchoredPanels(backbufferW(), backbufferH());
  }

  if (volcontrol->mouse_over){
    if (u6okeyhit(U6OK_SCROLLUP)){
      if (volcontrol->mouse_y<32){
        if ((u6ovolume+8)>255) u6ovolume=255; else u6ovolume+=8;
      }
      if ((volcontrol->mouse_y>=32)&&(volcontrol->mouse_y<=56)){
        if ((u6omidivolume+8)>255) u6omidivolume=255; else u6omidivolume+=8;
        goto u6omidivolume_changed;
      }
      if (volcontrol->mouse_y>56){
        if ((u6ovoicevolume+8)>255) u6ovoicevolume=255; else u6ovoicevolume+=8;
      }
    }
    if (u6okeyhit(U6OK_SCROLLDOWN)){
      if (volcontrol->mouse_y<32){
        if ((u6ovolume-8)>0) u6ovolume-=8; else u6ovolume=0;
      }
      if ((volcontrol->mouse_y>=32)&&(volcontrol->mouse_y<=56)){
        if ((u6omidivolume-8)>0) u6omidivolume-=8; else u6omidivolume=0;
        goto u6omidivolume_changed;
      }
      if (volcontrol->mouse_y>56){
        if ((u6ovoicevolume-8)>0) u6ovoicevolume-=8; else u6ovoicevolume=0;
      }
    }





  }//mouse_over
  if (volcontrol->mouse_pressed){
    if (volcontrol->mouse_x<46) volcontrol->mouse_x=46;
    if (volcontrol->mouse_x>117) volcontrol->mouse_x=117;
    if (volcontrol->mouse_y<32){
      u6ovolume=(volcontrol->mouse_x-46)*72/20;
    }
    if ((volcontrol->mouse_y>=32)&&(volcontrol->mouse_y<=56)){
      u6omidivolume=(volcontrol->mouse_x-46)*72/20;
u6omidivolume_changed:


      if (U6O_DISABLEMUSIC==FALSE){
        f=u6omidi_volume[midi_loaded];
        f=f*(float)u6omidivolume/255.0f;
        f=255-f; f=f*0.25f; f*=f;
        //DMUS_VOLUME_MAX     2000        /* +20 dB */
        //DMUS_VOLUME_MIN   -20000        /* -200 dB */
        u6omidi->SetMasterVolume(-f);
        if (u6omidivolume==0) u6omidi->Stop();

        if (midiout_setup){
          x=u6omidivolume/2;//change 0-255 to 0-127
          midiOutShortMsg(midiout_handle,0x000007B0+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B1+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B2+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B3+x*65536); //set volume
          midiOutShortMsg(midiout_handle,0x000007B4+x*65536); //set volume
        }

      }

    }
    if (volcontrol->mouse_y>56){
      u6ovoicevolume=(volcontrol->mouse_x-46)*72/20;
    }
    volcontrol->mouse_pressed=0;
  }


  if (mess_UPDATEps==NULL) mess_UPDATEps=txtnew();
  if (mess_SF==NULL) mess_SF=txtnew();
  if (mess1==NULL) mess1=txtnew();
  if (CLIENTplayer==NULL) CLIENTplayer=tplay;

  //CLIENT2HOST CONNECTION MESSAGE
  if (setup_message==FALSE){
    setup_message=TRUE;
    txtset(t,"?"); t->d2[0]=255; //setup
    txtset(t2,"??"); t2->ds[0]=U6O_VERSION; txtadd(t,t2); //version 5.5
    txtset(t2,"?"); t2->d2[0]=pw_encrypt; txtadd(t,t2); //PW encryption used
    txtset(t2,"?"); t2->d2[0]=u6o_name->l; txtadd(t,t2); txtadd(t,u6o_name); //name
    txtset(t2,"?"); t2->d2[0]=u6o_malefemale; txtadd(t,t2); //male/female
    txtset(t2,"??"); t2->ds[0]=u6o_portrait; txtadd(t,t2); //portrait
    txtset(t2,"?"); t2->d2[0]=u6o_type; txtadd(t,t2); //type (avatar, fighter,)
    txtNEWLEN(t2,28); for (i=0;i<=27;i++){t2->d2[i]=u6o_vq[i]; }
    txtadd(t,t2); //virtue answers
    txtset(t2,"?"); t2->d2[0]=u6o_user_name->l; txtadd(t,t2); txtadd(t,u6o_user_name); //user_name
    if (pw_encrypt==SHA2_ENCRYPTION) {
      SHA2(u6o_user_password,t3);
      txtset(t2,"?"); t2->d2[0]=t3->l; txtadd(t,t2);  txtadd(t,t3); //user_password
      txtset(t2,"?"); t2->d2[0]=u6o_createcharacter; txtadd(t,t2);
      SHA2(u6o_new_user_password,t3);
      txtset(t2,"?"); t2->d2[0]=t3->l; txtadd(t,t2); txtadd(t,t3); //user_password
    }
    else {
      txtset(t2,"?"); t2->d2[0]=u6o_user_password->l; txtadd(t,t2); txtadd(t,u6o_user_password); //user_password
      txtset(t2,"?"); t2->d2[0]=u6o_createcharacter; txtadd(t,t2);
      txtset(t2,"?"); t2->d2[0]=u6o_new_user_password->l; txtadd(t,t2); txtadd(t,u6o_new_user_password); //user_password
    }
    //...
    if (customportrait_upload){
      txtset(t2,"?"); t2->d2[0]=1; txtadd(t,t2);//extra info: upload character portrait
      txtNEWLEN(t2,3584*2); memcpy(&t2->d2[0],&customportrait,3584*2); txtadd(t,t2);
    }//customportrait_upload

    if (u6o_namecolour){
      txtaddchar(t,2);
      txtaddshort(t,u6o_namecolour&65535); txtaddchar(t,(u6o_namecolour>>16)&255);
    }//u6o_namecolour

#ifdef SECURE
#include "secret_a.inc"
#endif

    NET_send(NETplayer,NULL,t);
  }

  if (sysban){
    sysban+=et;
    if (sysban>=8.0f){
      MessageBox(NULL,"Leaving Britannia (Due to serious offenses administration no longer allows you to play u6o)","Ultima 6 Online",MB_OK);
      DestroyWindow(hWnd);
      sysban=0.0f;
    }
  }


  //assign mouse cursor
  // RW-P1.4: while the pointer is over the (enlarged) window resize border,
  // let Windows keep ownership of the cursor so its double-arrow resize icon
  // stays put. WndProc's WM_NCHITTEST/WM_SETCURSOR set cursorOverResizeBorder;
  // re-applying our custom cursor here every frame is exactly what made the
  // edge feel un-grabbable (the arrow flickered back to the play cursor).
  if (cursorOverResizeBorder){
    goto cursor_assign_done;
  }
  if ((vf_mb2_x==0xFFFF)&&(vf_mb2_y==0xFFFF)){
    if (cur_type==1) SetCursor (cur1);
  }
  if (cur_type==2) SetCursor (cur2);
  if (cur_type==3){
    if (CLIENTplayer->mobj) SetCursor (cur7); else SetCursor (cur3);
  }
  if (cur_type==4) SetCursor (cur4);
  if (cur_type==5) SetCursor (cur5);
  if (cur_type==6) SetCursor (cur6);
  if (cur_type==9) SetCursor (cur9);
cursor_assign_done:;

  //set mouse cursor type (based on previous loops state)
  cur_type=1;
  if (inprec) cur_type=2;
  if (userkey==1) cur_type=3;
  if (userkey==2) cur_type=4;
  if (userkey==3) cur_type=5;
  if (userkey==4) cur_type=6;
  if (userkey==5) cur_type=9;

  if (u6okeyhit(U6OK_CANCEL)){ //"ESC" cancel
userkey_cancel:
    userkey=0;
    txtNEWLEN(inpf2->input,0);
    inpf2->enterpressed=NULL;
    GETINPUT_txt=NULL;
    if (inpf->offset_x<kPanelHideThresholdX) inpf->offset_x+=kPanelHideDeltaX;
    if (inprec){
      txtset(t,"?"); t->d2[0]=20; NET_send(NETplayer,NULL,t); //typing... cancel
    }
    inprec=0;
  }



  if (autoscroll){
    autoscroll-=et;
    if (autoscroll<0.0f){
      autoscroll=0.0f;
      textdisplayi=0xFFFFFFF;
      textdisplayupdate=1;
      goto updatetextdisplay; //->
    }
  }

  if (con_frm->mouse_over==TRUE){
    if (textdisplayi!=-1){
      if (u6okeyhit(U6OK_SCROLLUP)){
        if (textdisplayi>0){
          textdisplayi--;
          textdisplayupdate=1;
          autoscroll=16.0f;
          goto updatetextdisplay; //->
        }
      }
      if (u6okeyhit(U6OK_SCROLLDOWN)){
        textdisplayi++;
        textdisplayupdate=1;
        autoscroll=16.0f;
        goto updatetextdisplay; //->
      }
    }//i!=-1
  }//con_frm->mouse_over==TRUE

  //text display up/down buttons
  if (con_frm->mouse_click){
    if (textdisplayi!=-1){
      if (con_frm->mouse_x>=17){
        textdisplayi++;
      }else{
        if (textdisplayi>0) textdisplayi--;
      }
      textdisplayupdate=1;
      autoscroll=16.0f;
      goto updatetextdisplay; //->
updatetextdisplay2: //<-
      textdisplayupdate=0;
    }
    con_frm->mouse_click=FALSE;
  }

  //CLIENT2HOST MESSAGES (ALLOWED EVEN IF DEAD)
  //global messages
  //send text message to NPC/player

  if (u6okeyhit(U6OK_RESPAWNFOLLOWERS)){
    txtsetchar(t,248); txtaddchar(t,254); NET_send(NETplayer,NULL,t);
  }

  if (u6okeyhit(U6OK_RESPAWN)){
    txtsetchar(t,248); NET_send(NETplayer,NULL,t);
  }

  if (inprec){
    if (inpf->mouse_over||vf->mouse_over){
      if (u6okeyhit(U6OK_SCROLLDOWN)){
        inpmess_selected++;
        static inpmess_index *inpmess_tmpi;
        inpmess_tmpi=inpmess_mostrecent;
        if (inpmess_tmpi->next==NULL) {inpmess_selected=-1; goto inpf_scroll_failed;}
        for (i=0;i<inpmess_selected;i++){
          if (inpmess_tmpi->next){
            if (inpmess_tmpi->next->next) inpmess_tmpi=inpmess_tmpi->next; else inpmess_selected=i-1;
          }
        }//i
        txtset(inpf2->input,inpmess_tmpi->t);
      }
      if (u6okeyhit(U6OK_SCROLLUP)){
        if (inpmess_selected>=1) inpmess_selected--; else inpmess_selected=0;
        static inpmess_index *inpmess_tmpi;
        inpmess_tmpi=inpmess_mostrecent;
        if (inpmess_tmpi->next==NULL) {inpmess_selected=-1; goto inpf_scroll_failed;}
        for (i=0;i<inpmess_selected;i++){
          if (inpmess_tmpi->next) inpmess_tmpi=inpmess_tmpi->next;
        }//i
        txtset(inpf2->input,inpmess_tmpi->t);
      }
    }//mouse_over
inpf_scroll_failed:
    if (u6okeyhit(U6OK_RETYPE)){
      txtadd(inpf2->input,inpmess_mostrecent->t);
    }
    if (CLIENTplayer->mf==255){
      if (CLIENTplayer->key&KEYmbclick){
        CLIENTplayer->key-=KEYmbclick;
        txtset(t,inpf2->input);
        txtmakeu6ocompatible(t);
        txtset(inpmess,t);
        if (t->l){
          if (!txtsame(inpmess_mostrecent->t,t)){
            static inpmess_index* inpmess_index_new;
            inpmess_index_new=(inpmess_index*)malloc(sizeof(inpmess_index)); inpmess_index_new->next=inpmess_mostrecent;
            txtset(inpmess_index_new->t=txtnew(),inpmess); inpmess_mostrecent=inpmess_index_new;
          }
        }
        inpmess_selected=-1;
        txtNEWLEN(inpf2->input,0);
        inpf2->enterpressed=NULL;
        if (inpf->offset_x<kPanelHideThresholdX) inpf->offset_x+=kPanelHideDeltaX;
        inprec=0;
        GETINPUT_txt=NULL;
        txtset(t2,"?"); t2->d2[0]=7; //message type 7
        txtset(t3,"????");
        t3->ds[0]=CLIENTplayer->mx;
        t3->ds[1]=CLIENTplayer->my;
        txtadd(t2,t3);
        txtadd(t2,inpmess);
        NET_send(NETplayer,NULL,t2);
        txtNEWLEN(inpmess,0);
      } //KEYmbclick
    } //mf==255
  }else{ //inprec
    u6okeyhit(U6OK_RETYPE); //dump the keyhit
  }


  //VOICE CHAT 1.0: DETECT AUDIO OUTPUT LEVEL
  f=1.0f; if (int((ett/f))!=int((ett/f)-(et/f))){


    //MIXER: variables

    static unsigned long MIXER_mastervolume;
    static unsigned long MIXER_wavevolume;
    static unsigned char hMixer_opened=FALSE;
    HMIXER hMixer=NULL;//handle to the mixer
    MIXERLINE ml;
    MIXERLINECONTROLS mlc;
    MIXERCONTROL mc;
    MIXERCONTROLDETAILS mcd;
    MIXERCONTROLDETAILS_UNSIGNED mcdu;

    //MIXER: open
    if (hMixer_opened==FALSE){ hMixer_opened=TRUE;
    mixerOpen(&hMixer, MIXER_OBJECTF_MIXER, 0, 0, 0);
    }

    //MIXER: get master volume line
    ZeroMemory(&ml,sizeof(ml));
    ml.cbStruct = sizeof(MIXERLINE);
    ml.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
    mixerGetLineInfo((HMIXEROBJ)hMixer,&ml,MIXER_GETLINEINFOF_COMPONENTTYPE);
    //MIXER: get MIXERCONTROL_CONTROLTYPE_VOLUME of master volume line
    ZeroMemory(&mlc,sizeof(mlc));
    ZeroMemory(&mc,sizeof(mc));
    mlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mlc.dwLineID = ml.dwLineID;
    mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    mlc.cControls = 1;
    mlc.pamxctrl = &mc;
    mlc.cbmxctrl = sizeof(MIXERCONTROL);
    mixerGetLineControls((HMIXEROBJ)hMixer,&mlc,MIXER_GETLINECONTROLSF_ONEBYTYPE);
    //MIXER: get volume level of master volume
    ZeroMemory(&mcd,sizeof(mcd));
    ZeroMemory(&mcdu,sizeof(mcdu));
    mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mcd.hwndOwner = 0;
    mcd.dwControlID = mc.dwControlID;
    mcd.paDetails = &mcdu;
    mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    mcd.cChannels = 1;
    mixerGetControlDetails((HMIXEROBJ)hMixer,&mcd,MIXER_SETCONTROLDETAILSF_VALUE);
    MIXER_mastervolume=mcdu.dwValue/256;

    //MIXER: get wave volume line
    ZeroMemory(&ml,sizeof(ml));
    ml.cbStruct = sizeof(MIXERLINE);
    ml.Target.dwType= MIXERLINE_TARGETTYPE_WAVEOUT;
    mixerGetLineInfo((HMIXEROBJ) hMixer,&ml,MIXER_GETLINEINFOF_TARGETTYPE);
    //MIXER: get MIXERCONTROL_CONTROLTYPE_VOLUME of wave volume line
    ZeroMemory(&mlc,sizeof(mlc));
    ZeroMemory(&mc,sizeof(mc));
    mlc.cbStruct = sizeof(MIXERLINECONTROLS);
    mlc.dwLineID = ml.dwLineID;
    mlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    mlc.cControls = 1;
    mlc.pamxctrl = &mc;
    mlc.cbmxctrl = sizeof(MIXERCONTROL);
    mixerGetLineControls((HMIXEROBJ)hMixer,&mlc,MIXER_GETLINECONTROLSF_ONEBYTYPE);
    //MIXER: get volume level of wave volume
    ZeroMemory(&mcd,sizeof(mcd));
    ZeroMemory(&mcdu,sizeof(mcdu));
    mcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
    mcd.hwndOwner = 0;
    mcd.dwControlID = mc.dwControlID;
    mcd.paDetails = &mcdu;
    mcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    mcd.cChannels = 1;
    mixerGetControlDetails((HMIXEROBJ)hMixer,&mcd,MIXER_SETCONTROLDETAILSF_VALUE);
    MIXER_wavevolume=mcdu.dwValue/256;







    x2=MIXER_wavevolume*MIXER_mastervolume*u6ovoicevolume;
    x2/=65536;
    if (x2!=mixer_volume){
      mixer_volume=x2;
      txtset(t2,"??"); t2->d2[0]=39;//update mixer volume
      t2->d2[1]=mixer_volume;


      NET_send(NETplayer,NULL,t2);


    }

  }//every ? seconds

  voicechat_mciwait-=et; if (voicechat_mciwait<0.0f) voicechat_mciwait=0.0f;
  voicechat_recordtime+=et;

  if (voicechat_recording){
    if (voicechat_recordtime<3.0f){
      f=voicechat_recordtime*10;
      x2=f;
      for (x3=1;x3<=x2;x3++){
        img(voicechat_frame->graphic,3+x3*13-13,3,voicechat_voice1);
      }
    }else{
      img(voicechat_frame->graphic,0,0,voicechat_voiceof);
    }
  }

  //VOICE CHAT 1.0+
  if (u6okeyon(U6OK_VOICECHAT)){
    if (voicechat_recording==0){
      if (playinstrument) goto voicechat_permissionrequestfinished;

      if (voicechat_permissionrequested==FALSE){
        voicechat_permissionrequested=TRUE;
        tfh=open2(".\\voice\\allow.bin",OF_READWRITE);
        if (tfh->h==HFILE_ERROR){
          if (IDYES==MessageBox(NULL,"Voice chat sends input from your microphone to other players when you hold this key. Do you allow U6O to use this feature?","Ultima 6 Online",MB_YESNO)){
            voicechat_permission=TRUE;
            if (IDYES==MessageBox(NULL,"Would you like to always start with voice chat allowed?","Ultima 6 Online",MB_YESNO)){
              tfh=open2(".\\voice\\allow.bin",OF_READWRITE|OF_CREATE); close(tfh);
            }
          }else{
            voicechat_permissionrequested=FALSE;
          }
          u6okeysetoff(U6OK_VOICECHAT);
          goto voicechat_permissionrequestfinished;
        }else{
          close(tfh);
          voicechat_permission=TRUE;
        }
      }//voicechat_permissionrequested==FALSE


      //begin recording
      voicechat_recording=1;
      voicechat_recordtime=0.0f;
      if (voicechat_frame->offset_x>=kPanelHideThresholdX) voicechat_frame->offset_x-=kPanelHideDeltaX;
      img(voicechat_frame->graphic,0,0,voicechat_voicebar);

      voicechat_listeningplayers=FALSE;//must be validated by returned message
      voicechat_listeningplayeri=0;
      txtset(t2,"?"); t2->d2[0]=40;//request voice volume levels of others(40)
      NET_send(NETplayer,NULL,t2);
      //returned by ID!


      dwMilliSeconds=3000;



      mciOpenParms.lpstrDeviceType = "waveaudio";
      mciOpenParms.lpstrElementName = "";




      if(mciSendCommand(0,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE|MCI_WAIT,(DWORD)(LPVOID)&mciOpenParms)) exit(7);
      wDeviceID = mciOpenParms.wDeviceID;


      /*
      MCI_WAVE_SET_PARMS mwspWaveFormParameters;
      mwspWaveFormParameters.ulSamplesPerSec = 11025;
      mwspWaveFormParameters.usBitsPerSample = 8;
      mciSendCommand (SoundDevice.usSoundDeviceID,
      MCI_SET,
      MCI_WAIT,
      MCI_WAVE_SET_SAMPLESPERSEC,
      MCI_WAVE_SET_BITSPERSAMPLE,
      (PVOID)&mwspWaveFormParameters,
      0);
      */

      /*
      MCI_WAVE_SET_PARMS mwspWaveFormParameters;
      ZeroMemory(&mwspWaveFormParameters,sizeof(mwspWaveFormParameters));
      mwspWaveFormParameters.nSamplesPerSec = 44100;
      mwspWaveFormParameters.wBitsPerSample = 16;
      if(x3=mciSendCommand (wDeviceID,MCI_SET,MCI_WAVE_SET_BITSPERSAMPLE|MCI_WAVE_SET_SAMPLESPERSEC,(DWORD)(LPVOID)&mwspWaveFormParameters)) return x3;
      */

      mciRecordParms.dwTo = dwMilliSeconds;
      if(mciSendCommand(wDeviceID,MCI_RECORD,MCI_TO,(DWORD)(LPVOID)&mciRecordParms)) exit(8);




    }
  }


  //finish recording?
  if (voicechat_recording){
    if (u6okeyon(U6OK_VOICECHAT)==FALSE){
      voicechat_recording=0;
      if (voicechat_frame->offset_x<kPanelHideThresholdX) voicechat_frame->offset_x+=kPanelHideDeltaX;
      if (voicechat_recordtime>3.0f){//recording has exceeded maximum allowable time!
        if(mciSendCommand(wDeviceID,MCI_CLOSE,MCI_WAIT,NULL)) exit(6);

        STATUSMESSadd("Voice chat is limited to 3 seconds!");
        goto getvoice_timeexceeded;
      }
      mciSendCommand(wDeviceID,MCI_PAUSE,MCI_WAIT,NULL);
      deletefile(".\\voice\\voice.wav");

      mciSaveParms.lpfilename = ".\\voice\\voice.wav";
      if(mciSendCommand(wDeviceID, MCI_SAVE,MCI_SAVE_FILE|MCI_WAIT,(DWORD)(LPVOID)&mciSaveParms)) exit(10);
      if(mciSendCommand(wDeviceID,MCI_CLOSE,MCI_WAIT,NULL)) exit(6);

      waitforfile(".\\voice\\voice.wav");

      deletefile(".\\voice\\voice.spx");
      PROCESS_INFORMATION pi;
      STARTUPINFO si;
      ZeroMemory(&pi,sizeof(pi)); ZeroMemory(&si,sizeof(si));
      si.cb=sizeof(STARTUPINFO);
      si.dwFlags=STARTF_USESIZE|STARTF_USESHOWWINDOW;
      si.wShowWindow=SW_HIDE;
      CreateProcess(NULL,".\\voice\\speexenc.exe -n --bitrate 8192 .\\voice\\voice.wav .\\voice\\voice.spx",NULL,NULL,FALSE, 0, NULL, NULL, &si, &pi);
      waitforfile(".\\voice\\voice.spx");

      //get buffered sound
      tfh=open(".\\voice\\voice.spx");
      txtNEWLEN(t2,lof(tfh));
      get(tfh,t2->d,lof(tfh));
      close(tfh);

      if (voicechat_listeningplayers){
        txtsetchar(t3,37);//send voice chat message (local)
        if (voicechat_listeningplayers&2) txtadd(t3,t2);
        NET_send(NETplayer,NULL,t3);
      }


      txtset(t4,""); tplay->GNPCflags[245]=namecolour;
      //simulate message as a local
      if (tplay->party[0]){
        tnpc=(npc*)tplay->party[0]->more;
        txtsetshort(t,tnpc->port);
        txtset(t5,tnpc->name); txtadd(t5,":");
        txtaddcolouredname(t4,t5,tplay);
        txtaddchar(t,t4->l);
        txtadd(t,t4);
      }else{
        txtsetshort(t,portlast);
        txtset(t5,namelast); txtadd(t5,":");
        txtaddcolouredname(t4,t5,tplay);
        txtaddchar(t,t4->l);
        txtadd(t,t4);
      }
      txtaddshort(t,tplay->x); txtaddshort(t,tplay->y);
      txtadd(t,t2);
      localvoicemessage_return=1;
      goto localvoicemessage_begin;
    }
  }
localvoicemessage_returnpoint:
getvoice_timeexceeded:
voicechat_permissionrequestfinished:

  if (tplay->party[0]==NULL){
    if (u6okeyhit(U6OK_TALK)){ //"T" talk
      talkprev=0;
      if (inprec==0){
        deadglobalmessage=1; goto dglobal;
      }

    }
    if (inpf2->enterpressed){
      deadglobalmessage=1; goto dglobal2; //process receiving input
    }
  }//tplay->party[0]==NULL
deadglobalmessage_return:

  if (inpmess->l){ //send input message to the host
    txtset(t2,"?"); t2->d2[0]=6;
    if (inprec_global){t2->d2[0]=12; client_globalwait=0;}//global message
    if (talkprev) t2->d2[0]=34;
    txtadd(t2,inpmess);
    NET_send(NETplayer,NULL,t2);
    txtNEWLEN(inpmess,0);
  }

  f=client_globalwait;
  client_globalwait+=et;
  if ((f<10)&&(client_globalwait>=10)){
    STATUSMESSadd("Ready to send next global message.");
  }

  //CLIENT2HOST MESSAGES (SKIPPED WHEN DEAD)
  if (tplay->party[0]){


    x=0;
    if (u6okeyhit(U6OK_TALK_GLOBAL)){x=2; talkprev=0;}
    if (u6okeyhit(U6OK_TALKPREV)){x=1; talkprev=1;}
    if (u6okeyhit(U6OK_TALK)){x=1; talkprev=0;}
    if (x){ //"T" talk
      if (inprec==0){
        if (x==2){
dglobal:
          if (client_globalwait<10){
            STATUSMESSadd("\x0FFWait 10 seconds between sending global messages!");
            goto skipglobalmessage;
          }
          inprec_global=1; shiftnum_show=0;
        }else inprec_global=0;
        txtset(t,"?"); t->d2[0]=19; NET_send(NETplayer,NULL,t); //typing... message
        userkey=0;
        inpf2->enterpressed=FALSE;

		// r666 auto set talk text
		if (actiontalkfilltext) {
			actiontalkfilltext = 0;
			txtset(inpf2->input, newt1);
		}

        GETINPUT_setup(inpf2->input,&inpf2->enterpressed,inpf2->length_limit);
        if (inpf->offset_x>=kPanelHideThresholdX) inpf->offset_x-=kPanelHideDeltaX;

        if (tplay->party[0]){
          tnpc=(npc*)tplay->party[0]->more;
          inpf->graphic=getportrait(tnpc->port);
        }else inpf->graphic=getportrait(portlast);

        if (inprec_global){
          inpft->col=rgb(96,128,255);
          inpf2->col=rgb(96,128,255);
        }else{
          inpft->col=rgb(255,255,255);
          inpf2->col=rgb(255,255,255);
        }
        inprec=1;
      }

    }
skipglobalmessage:
    if (inpf2->enterpressed){
dglobal2:
      txtset(t,inpf2->input);
      txtmakeu6ocompatible(t);

      txtset(t2,"/NOCLOUDS");
      if (txtsamewithoutcase(t,t2)){
        noclouds=TRUE;
        txtset(STATUSMESSt,"Clouds disabled."); STATUSMESSadd(STATUSMESSt);
        goto inpmess_skip;
      }

      txtset(t2,"/CLOUDS");
      if (txtsamewithoutcase(t,t2)){
        noclouds=FALSE;
        txtset(STATUSMESSt,"Clouds enabled."); STATUSMESSadd(STATUSMESSt);
        goto inpmess_skip;
      }


      i4=0;
      txtset(t3,t); txtset(t9,t3); txtucase(t3);
      txtset(t2,"/IGNORE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-8); txtright(t3,t3->l-8); goto ignoreremove;
      }
      txtset(t2,"/UNIGNORE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-10); txtright(t3,t3->l-10); goto ignoreremove;
      }
      txtset(t2,"/MUTE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-6); txtright(t3,t3->l-6); goto ignoreremove;
      }
      txtset(t2,"/UNMUTE "); if (txtsearch(t3,t2)==1){
        txtright(t9,t9->l-8); txtright(t3,t3->l-8); goto ignoreremove;
      }
      goto skipignoreremove;
ignoreremove:
      tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
ignoreremoveplayer:
      i=seek(tfh);
      txtfilein(t6,tfh); txtucase(t6);

      txtset(t2,"[MUTE]"); if (txtsearch(t6,t2)==1){
        txtadd(t2,t3); if (txtsame(t6,t2)){
          i2=seek(tfh);
          for (i3=0;i3<t2->l;i3++) t2->d2[i3]=42;
          seek(tfh,i); put(tfh,t2->d,t2->l);
          seek(tfh,i2);

          txtset(STATUSMESSt,t9); txtadd(STATUSMESSt," unmuted!"); STATUSMESSadd(STATUSMESSt);
        }
      }
      txtset(t2,"[IGNORE]"); if (txtsearch(t6,t2)==1){
        txtadd(t2,t3); if (txtsame(t6,t2)){
          i2=seek(tfh);
          for (i3=0;i3<t2->l;i3++) t2->d2[i3]=42;
          seek(tfh,i); put(tfh,t2->d,t2->l);
          seek(tfh,i2);

          txtset(STATUSMESSt,t9); txtadd(STATUSMESSt," unignored!"); STATUSMESSadd(STATUSMESSt);
        }
      }
      if (t6->l) goto ignoreremoveplayer;
      close(tfh);
      i4=1;
skipignoreremove:
      //client commands
      txtset(t2,"/IGNORE "); txtset(t3,t); txtucase(t3); if (txtsearch(t3,t2)==1){
        txtright(t3,t3->l-8);
        txtset(t2,"[IGNORE]"); txtadd(t2,t3);
        tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
        seek(tfh,lof(tfh));
        txtfileout(t2,tfh);
        close(tfh);
        txtright(t,t->l-8); txtadd(t," ignored!");

        STATUSMESSadd(t);
        goto inpmess_skip;
      }
      txtset(t2,"/MUTE "); txtset(t3,t); txtucase(t3); if (txtsearch(t3,t2)==1){
        txtright(t3,t3->l-6);
        txtset(t2,"[MUTE]"); txtadd(t2,t3);
        tfh=open2("ignore.txt",OF_READWRITE|OF_SHARE_COMPAT);
        seek(tfh,lof(tfh));
        txtfileout(t2,tfh);
        close(tfh);
        txtright(t,t->l-6); txtadd(t," muted!");

        STATUSMESSadd(t);
        goto inpmess_skip;
      }
      if (i4) goto inpmess_skip;





      txtset(t2,"load wavinfo.txt"); if (txtsame(t,t2)){

        STATUSMESSadd("loaded wavinfo.txt");
        wavinfo_loaded=FALSE; goto inpmess_skip;
      }
      txtset(t2,"load midiinfo.txt"); if (txtsame(t,t2)){

        STATUSMESSadd("loaded midiinfo.txt");
        midiinfo_loaded=FALSE; goto inpmess_skip;
      }
      txtset(t2,"set musickeyboard"); if (txtsame(t,t2)){
        midikeyboard_set=24;
        mididown(clientinstrument,midikeyboard_set);
        goto inpmess_skip;
      }
      txtset(inpmess,t);
      if (inpmess->l){
        if (!txtsame(inpmess_mostrecent->t,t)){
          static inpmess_index* inpmess_index_new;
          inpmess_index_new=(inpmess_index*)malloc(sizeof(inpmess_index)); inpmess_index_new->next=inpmess_mostrecent;
          txtset(inpmess_index_new->t=txtnew(),inpmess); inpmess_mostrecent=inpmess_index_new;
        }
        inpmess_selected=-1;
        if (U6O_WALKTHRU_REC) txtfileout(inpmess,walkthru_fh);
      }else{
inpmess_skip: txtset(t,"?"); t->d2[0]=20; NET_send(NETplayer,NULL,t); //typing... cancel
      }
      txtNEWLEN(inpf2->input,0);
      inpf2->enterpressed=NULL;
      if (inpf->offset_x<kPanelHideThresholdX) inpf->offset_x+=kPanelHideDeltaX;
      inprec=0;
    }
    if (deadglobalmessage) {deadglobalmessage=0; goto deadglobalmessage_return;}

    CLIENTplayer->key2=CLIENTplayer->key; //backup old key info
    CLIENTplayer->key=0; //NULL key info

    CLIENTplayer->mf=255; //assume default: primary surface
    CLIENTplayer->mx=vf->mouse_x;
    CLIENTplayer->my=vf->mouse_y;
    if (vf->mouse_click==TRUE) CLIENTplayer->key|=KEYmbclick;

	// r222 this is where some values are propagated/set relating to mouse over/click on a party member frame; no changes are made here.

    for (i=0;i<=7;i++){
      pmf=party_frame[i];
      if (pmf->mouse_over==TRUE){
        CLIENTplayer->mf=i;
        CLIENTplayer->mx=pmf->mouse_x;
        CLIENTplayer->my=pmf->mouse_y;

        if (pmf->mouse_click==TRUE) CLIENTplayer->key|=KEYmbclick;
        if (u6okeyhit(U6OK_SCROLLUP)){
          CLIENTplayer->mx=118;
          CLIENTplayer->my=202;
          CLIENTplayer->key|=KEYmbclick;
          if (CLIENTplayer->key2&KEYmbclick) CLIENTplayer->key2-=KEYmbclick;
        }
        if (u6okeyhit(U6OK_SCROLLDOWN)){
          CLIENTplayer->mx=118;
          CLIENTplayer->my=240;
          CLIENTplayer->key|=KEYmbclick;
          if (CLIENTplayer->key2&KEYmbclick) CLIENTplayer->key2-=KEYmbclick;
        }

		/*
		txtset(t2, "clientplayer mx ");
		txtnumint(t3, CLIENTplayer->mx);
		txtadd(t2, t3);
		txtadd(t2, " // my ");
		txtnumint(t3, CLIENTplayer->my);
		txtadd(t2, t3);
		LOGadd(t2);
		*/
	  }//pmf->mouse_over==TRUE
    }//i

    //turn spellbook pages
    for (i=0;i<=7;i++){
      pmf=party_spellbook_frame[i];
      if (pmf->mouse_over==TRUE){
        if (u6okeyhit(U6OK_SCROLLUP)){
          if (spellbook_flags[i]&1) goto turnspellpagel;
        }
        if (u6okeyhit(U6OK_SCROLLDOWN)){
          if (spellbook_flags[i]&2) goto turnspellpager;
        }
      }
      if (spellbook_flags[i]&1){
        if (pmf->mouse_click){
          if (pmf->mouse_x<(33+32)){
            if (pmf->mouse_y<32){
turnspellpagel:
              spellbook_page[i]--;
              i4=0;
              for (i5=0;i5<=15;i5++){
                if (spell[i][((spellbook_page[i]-1)<<4)+i5]) i4=1;
              }
              if (i4==0) goto turnspellpagel;
              tnpc=(npc*)tplay->party[i]->more; tnpc->upflags|=128;
            }}
        }
      }
      if (spellbook_flags[i]&2){
        if (pmf->mouse_click){
          if (pmf->mouse_x>=(212-32)){
            if (pmf->mouse_y<32){
turnspellpager:
              spellbook_page[i]++;
              i4=0;
              for (i5=0;i5<=15;i5++){
                if (spell[i][((spellbook_page[i]-1)<<4)+i5]) i4=1;
              }
              if (i4==0) goto turnspellpager;
              tnpc=(npc*)tplay->party[i]->more; tnpc->upflags|=128;
            }}
        }
      }
    }//i


    tnpc=(npc*)tplay->party[0]->more;


    if (tshiftnum==NULL) tshiftnum=txtnew();
    if (u6okeyon(U6OK_QUANTITY)){
      x=tshiftnum->l;
      if (u6okeyhit(U6OK_QUANTITY0)&&(tshiftnum->l!=0)) txtadd(tshiftnum,"0");
      if (u6okeyhit(U6OK_QUANTITY1)) txtadd(tshiftnum,"1");
      if (u6okeyhit(U6OK_QUANTITY2)) txtadd(tshiftnum,"2");
      if (u6okeyhit(U6OK_QUANTITY3)) txtadd(tshiftnum,"3");
      if (u6okeyhit(U6OK_QUANTITY4)) txtadd(tshiftnum,"4");
      if (u6okeyhit(U6OK_QUANTITY5)) txtadd(tshiftnum,"5");
      if (u6okeyhit(U6OK_QUANTITY6)) txtadd(tshiftnum,"6");
      if (u6okeyhit(U6OK_QUANTITY7)) txtadd(tshiftnum,"7");
      if (u6okeyhit(U6OK_QUANTITY8)) txtadd(tshiftnum,"8");
      if (u6okeyhit(U6OK_QUANTITY9)) txtadd(tshiftnum,"9");
      if (tshiftnum->l>5) txtleft(tshiftnum,5);
      if (x!=tshiftnum->l){
        shiftnum_show=1;
        x2=txtnum(tshiftnum);
        if (x2>65535) x2=65535;
        txtset(t,"?"); t->d2[0]=13; //update QUANtity value
        txtset(t2,"??"); t2->ds[0]=x2;
        txtadd(t,t2);
        NET_send(NETplayer,NULL,t);
      }//change made
    }else{
      shiftnum_show=0; txtset(tshiftnum,"");
    }


    if (u6okeyhit(U6OK_NOT4SALE)){ //"X" not4sale
      userkey=5;
      if (U6OK_FLAGS[U6OK_NOT4SALE]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click
    }

    if (u6okeyhit(U6OK_USE)){ //"U" use
      userkey=1;
      if (U6OK_FLAGS[U6OK_USE]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click


      goto skipaltuse;
    }//use
    if (u6okeyhit(U6OK_USE2)){ //"U" alt use
      userkey=1;
      if (U6OK_FLAGS[U6OK_USE2]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click

    }//use
skipaltuse:


    if (u6okeyhit(U6OK_LOOK)){ //"L" look
      userkey=2;
      if (U6OK_FLAGS[U6OK_LOOK]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click


      goto skipaltlook;
    }
    if (u6okeyhit(U6OK_LOOK2)){ //"L" alt look
      userkey=2;
      if (U6OK_FLAGS[U6OK_LOOK2]&1){//instant click: simulate mouse click on current surface
        CLIENTplayer->key|=KEYmbclick;
      }//instant click

    }
skipaltlook:




    if (u6okeyhit(U6OK_ATTACK)){ //"A" attack
      if (U6OK_FLAGS[U6OK_ATTACK]&1){//instant click: simulate mouse click on current surface
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        userkey=3;
        CLIENTplayer->key|=KEYmbclick;
        goto u6okattackdone;
      }
      if (userkey!=3){
        txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
        ktar_display=2.0f;
        userkey=3;
      }else{
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        goto userkey_cancel;
      }


      goto u6okattackdone;
    }
    if (u6okeyhit(U6OK_ATTACK2)){ //"A" alt attack
      if (U6OK_FLAGS[U6OK_ATTACK2]&1){//instant click: simulate mouse click on current surface
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        userkey=3;
        CLIENTplayer->key|=KEYmbclick;
        goto u6okattackdone;
      }
      if (userkey!=3){
        txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
        ktar_display=2.0f;
        userkey=3;
      }else{
        txtset(t,"?"); t->d2[0]=17; NET_send(NETplayer,NULL,t); //break off combat
        goto userkey_cancel;
      }


      goto u6okattackdone;
    }
u6okattackdone:

	// r777 simulate click on self
    if (actionpending == 502){ //"U" use
		actionpending = 503;

//      userkey=1;
//      if (U6OK_FLAGS[U6OK_USE]&1){//instant click: simulate mouse click on current surface
		//CLIENTplayer->mx = MX_AVATAR; // 495
		//CLIENTplayer->my = MY_AVATAR; // 365

		playeronscreenxn1 = tplay->x - tpx;
		playeronscreenyn1 = tplay->y - tpy;

		CLIENTplayer->mx = playeronscreenxn1*32+16;
		CLIENTplayer->my = playeronscreenyn1*32+16;
        CLIENTplayer->key|=KEYmbclick;

		/*
		txtset(t3, "111 x = ");
		txtnumint(t4, tplay->x);
		txtadd(t3, t4);
		txtadd(t3, " // y = ");
		txtnumint(t4, tplay->y);
		txtadd(t3, t4);

		txtadd(t3, " // tpx = ");
		txtnumint(t4, tpx);
		txtadd(t3, t4);
		txtadd(t3, " // tpy = ");
		txtnumint(t4, tpy);
		txtadd(t3, t4);

		txtadd(t3, " // ax = ");
		txtnumint(t4, (tplay->x-tpx) * 32 + 16); // 15
		txtadd(t3, t4);
		txtadd(t3, " // ay = ");
		txtnumint(t4, (tplay->y-tpy) * 32 + 16); // 11
		txtadd(t3, t4);

		txtadd(t3, " // px = ");
		txtnumint(t4, playeronscreenxn1); // 15
		txtadd(t3, t4);
		txtadd(t3, " // py = ");
		txtnumint(t4, playeronscreenyn1); // 11
		txtadd(t3, t4);

		txtadd(t3, " // mx = ");
		txtnumint(t4, CLIENTplayer->mx);
		txtadd(t3, t4);
		txtadd(t3, " // my = ");
		txtnumint(t4, CLIENTplayer->my);
		txtadd(t3, t4);

		LOGadd(t3);
		*/
//      }//instant click
    }//use

	// r777 simulate click on drop location
    if (actionpending == 510) {
		actionpending = 599;

		playeronscreenxn1 = tplay->x - tpx;
		playeronscreenyn1 = tplay->y - tpy;

		if (droplocation == 1) {
			// north
			CLIENTplayer->mx = playeronscreenxn1*32+16;
			CLIENTplayer->my = playeronscreenyn1*32+16-32;
		}
		else if (droplocation == 2) {
			// south
			CLIENTplayer->mx = playeronscreenxn1*32+16;
			CLIENTplayer->my = playeronscreenyn1*32+16+32;
		}
		else if (droplocation == 3) {
			// west
			CLIENTplayer->mx = playeronscreenxn1*32+16-32;
			CLIENTplayer->my = playeronscreenyn1*32+16;
		}
		else if (droplocation == 4) {
			// east
			CLIENTplayer->mx = playeronscreenxn1*32+16+32;
			CLIENTplayer->my = playeronscreenyn1*32+16;
		}

        CLIENTplayer->key|=KEYmbclick;
    }

    //mouse movement
    {
      static unsigned char directionalmove_mbheld=0;
      static unsigned char pathmove_mbheld=0;
      static float pathmove_wait=0;
      if ((mbheld&2)==0){
        directionalmove_mbheld=0; pathmove_mbheld=0;
      }
      if ((mbheld&2)||(mbclick&2)){
        if ((vf_mb2_x!=0xFFFF)&&(vf_mb2_y!=0xFFFF)){ //right mouse pressed on ps
          //x2,y2 pixel at middle of selected player sprite on screen
          x2=(ctpx2-ctpx)*32; y2=(ctpy2-ctpy)*32;
          x2=vf_mb2_x-(x2+16); y2=vf_mb2_y-(y2+16);
          x3=abs(x2); y3=abs(y2);
          if (directionalmove_only||wizardeyetimeleft) goto directionalmove_force;
          if (directionalmove_mbheld) goto directionalmove_force;
          if (pathmove_mbheld) goto pathmove_force;
          if ((x3>48)||(y3>48)){ //path finding range
pathmove_force:
            pathmove_mbheld=1;
            if (cur_type==1) SetCursor(cur8);
            if (omb&2){
              pathmove_wait+=et;
              if (pathmove_wait<=0.25f) goto mousemove_finish;
            }
            pathmove_wait=0;
            //send pathfinding message
            txtset(t,"?"); t->d2[0]=21; //client update message
            x=ctpx+vf_mb2_x/32; y=ctpy+vf_mb2_y/32;
            txtset(t2,"????"); t2->ds[0]=x; t2->ds[1]=y;
            txtadd(t,t2);
            NET_send(NETplayer,NULL,t);
            goto mousemove_finish;
          }
directionalmove_force:
          directionalmove_mbheld=1;
          x=0; y=0;
          if (x3>=y3) x=1;
          if (y3>=x3) y=1;
          if (y3>=(x3/2)){
            if (x3>=(y3/2)){
              x=1;
              y=1;
            }
          }
          if (x2<0) x=-x;
          if (y2<0) y=-y;
          if (x==1) CLIENTplayer->key|=KEYright;
          if (x==-1) CLIENTplayer->key|=KEYleft;
          if (y==1) CLIENTplayer->key|=KEYdown;
          if (y==-1) CLIENTplayer->key|=KEYup;
          if ((x==1)&&(y==0)) if (cur_type==1) SetCursor (cur_r);
          if ((x==1)&&(y==1)) if (cur_type==1) SetCursor (cur_rd);
          if ((x==0)&&(y==1)) if (cur_type==1) SetCursor (cur_d);
          if ((x==-1)&&(y==1)) if (cur_type==1) SetCursor (cur_ld);
          if ((x==-1)&&(y==0)) if (cur_type==1) SetCursor (cur_l);
          if ((x==-1)&&(y==-1)) if (cur_type==1) SetCursor (cur_lu);
          if ((x==0)&&(y==-1)) if (cur_type==1) SetCursor (cur_u);
          if ((x==1)&&(y==-1)) if (cur_type==1) SetCursor (cur_ru);
          if ((x==0)&&(y==0)) if (cur_type==1) SetCursor(cur1);
        }
      }
    }
mousemove_finish:
    vf_mb2_x=0xFFFF; vf_mb2_y=0xFFFF;


	if (u6okeyhit(U6OK_RIGHT)) {
		CLIENTplayer->key |= KEYright2;
		// r777 set drop location right
		if (setdroplocation) {
			setdroplocation = 0;
			droplocation = 4;
		}
	}
	else {
		if (u6okeyon(U6OK_RIGHT)) {
			CLIENTplayer->key |= KEYright;
			// r777 set drop location right
			if (setdroplocation) {
				setdroplocation = 0;
				droplocation = 4;
			}
		}

	}

	if (u6okeyhit(U6OK_LEFT)) {
		CLIENTplayer->key |= KEYleft2;
		// r777 set drop location left
		if (setdroplocation) {
			setdroplocation = 0;
			droplocation = 3;
		}
	}
	else {
		if (u6okeyon(U6OK_LEFT)) {
			CLIENTplayer->key |= KEYleft;
			// r777 set drop location left
			if (setdroplocation) {
				setdroplocation = 0;
				droplocation = 3;
			}
		}
	}

	if (u6okeyhit(U6OK_UP)) {
		CLIENTplayer->key |= KEYup2;
		// r777 set drop location up
		if (setdroplocation) {
			setdroplocation = 0;
			droplocation = 1;
		}
	}
	else {
		if (u6okeyon(U6OK_UP)) {
			CLIENTplayer->key |= KEYup;
			// r777 set drop location up
			if (setdroplocation) {
				setdroplocation = 0;
				droplocation = 1;
			}
		}
	}

	if (u6okeyhit(U6OK_DOWN)) {
		CLIENTplayer->key |= KEYdown2;
		// r777 set drop location down
		if (setdroplocation) {
			setdroplocation = 0;
			droplocation = 2;
		}
	}
	else {
		if (u6okeyon(U6OK_DOWN)) {
			CLIENTplayer->key |= KEYdown;
			// r777 set drop location down
			if (setdroplocation) {
				setdroplocation = 0;
				droplocation = 2;
			}
		}
	}


    //turn spell
    for (i=0;i<=7;i++){

      if (party_spellbook_frame[i]->mouse_click){
        if ((party_spellbook_frame[i]->mouse_y>=32)&&(party_spellbook_frame[i]->mouse_y<152)){ //5, 24pixel areas, after 32 pixels down
          if ((party_spellbook_frame[i]->mouse_x>=16)&&(party_spellbook_frame[i]->mouse_x<229)){ //16 pixels on each side
            if ((party_spellbook_frame[i]->mouse_x>=126)||(party_spellbook_frame[i]->mouse_x<118)){ //4|4 middle pixels
              //ok so now we must obtain a number, x, of which spell the player selected!
              x=(party_spellbook_frame[i]->mouse_y-32)/24;
              x2=party_spellbook_frame[i]->mouse_y-32-x*24;
              if (x2<16){
                if (party_spellbook_frame[i]->mouse_x>=126) x+=5;

                //is that spell in the spellbook at all???
                i3=spellbook_page[i]-1;

                i4=0;
                for (i2=0;i2<=15;i2++){
                  if (spell[i][(i3<<4)+i2]){ //valid


                    if (i4==x){
					  /*
                      if (spell[i][(i3<<4)+i2]!=1){ //can cast?
                        userkey=4;
                        userspell=(i3<<4)+i2;
                        userspellbook=i;
                        if (spelltarget[(i3<<4)+i2]==1){
                          txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
                          ktar_display=2.0f;
                        }
                        goto gotspell;

                      }
					  */

						// f222 prevent casting until it is ready (when clicking on spells in spellbook)
						if (enhancen1) {
							if (!client_spellwait[i]) {
								if (spell[i][(i3 << 4) + i2] != 1) { //can cast?
									userkey = 4;
									userspell = (i3 << 4) + i2;
									userspellbook = i;
									if (spelltarget[(i3 << 4) + i2] == 1) {
										txtset(t, "?"); t->d2[0] = 15; NET_send(NETplayer, NULL, t); //keyboard targeting request
										ktar_display = 2.0f;
									}
									goto gotspell;

								}
							}
						}
						else { // original
						  if (spell[i][(i3<<4)+i2]!=1){ //can cast?
							userkey=4;
							userspell=(i3<<4)+i2;
							userspellbook=i;
							if (spelltarget[(i3<<4)+i2]==1){
							  txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
							  ktar_display=2.0f;
							}
							goto gotspell;

						  }
						}
                    }

                    i4++;
                  }
                }
                //user didn't click on a spell!

gotspell:;

              }//x2<20
            }//4|4
          }//16
        }//32+5*24
      }

      party_spellbook_frame[i]->mouse_click=0;
    }//i


    if (userkey==4){
      x=-1;
      if (u6okeyhit(U6OK_SETCASTRECALL1)) x=0;
      if (u6okeyhit(U6OK_SETCASTRECALL2)) x=1;
      if (u6okeyhit(U6OK_SETCASTRECALL3)) x=2;
      if (u6okeyhit(U6OK_SETCASTRECALL4)) x=3;
      if (u6okeyhit(U6OK_SETCASTRECALL5)) x=4;
      if (u6okeyhit(U6OK_SETCASTRECALL6)) x=5;
      if (u6okeyhit(U6OK_SETCASTRECALL7)) x=6;
      if (u6okeyhit(U6OK_SETCASTRECALL8)) x=7;
      if (x!=-1){
        //remove other F-keys to this spell
        for (x2=0;x2<=7;x2++) if ((spellrecall_partymember[x2]==(userspellbook+1))&&(spellrecall_i[x2]==userspell)) spellrecall_partymember[x2]=0;
        spellrecall_partymember[x]=userspellbook+1;
        spellrecall_i[x]=userspell;
        //update ALL spellbooks
        for (x2=0;x2<=7;x2++){
          if (tplay->party[x2]){
            tnpc=(npc*)tplay->party[x2]->more; tnpc->upflags|=128;
          }//party
        }//x2
        userkey=0;
      }//!=-1


      x=-1;
      if (u6okeyhit(U6OK_KTAR1)) x=0;
      if (u6okeyhit(U6OK_KTAR2)) x=1;
      if (u6okeyhit(U6OK_KTAR3)) x=2;
      if (u6okeyhit(U6OK_KTAR4)) x=3;
      if (u6okeyhit(U6OK_KTAR5)) x=4;
      if (u6okeyhit(U6OK_KTAR6)) x=5;
      if (u6okeyhit(U6OK_KTAR7)) x=6;
      if (u6okeyhit(U6OK_KTAR8)) x=7;
      if (u6okeyhit(U6OK_KTAR9)) x=8;
      if (u6okeyhit(U6OK_KTAR0)) x=9;
      if (x!=-1){
        CLIENTplayer->mx=32768+x;
        CLIENTplayer->key|=KEYmbclick;
        goto ktarcast;
      }

      if (vf->mouse_click){
        CLIENTplayer->mx=vf->mouse_x/32;
ktarcast:
        userkey=0;
        CLIENTplayer->mf=8+userspellbook;
        CLIENTplayer->my=vf->mouse_y/32;
        CLIENTplayer->my|=(userspell<<8);
      }
    }

    //recall spell using f5-f12
    if (userkey==0){
      x=-1;
      if (u6okeyhit(U6OK_CASTRECALL1)) x=0;
      if (u6okeyhit(U6OK_CASTRECALL2)) x=1;
      if (u6okeyhit(U6OK_CASTRECALL3)) x=2;
      if (u6okeyhit(U6OK_CASTRECALL4)) x=3;
      if (u6okeyhit(U6OK_CASTRECALL5)) x=4;
      if (u6okeyhit(U6OK_CASTRECALL6)) x=5;
      if (u6okeyhit(U6OK_CASTRECALL7)) x=6;
      if (u6okeyhit(U6OK_CASTRECALL8)) x=7;
      if (x!=-1){
        if (spellrecall_partymember[x]){
			if (tplay->party[spellrecall_partymember[x] - 1]) {
				/*
				if (spell[spellrecall_partymember[x]-1][spellrecall_i[x]]>1){ //valid
				  userkey=4;
				  userspell=spellrecall_i[x];
				  userspellbook=spellrecall_partymember[x]-1;
				  if (spelltarget[userspell]==1){
					txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
					ktar_display=2.0f;
				  }//ktar
				}//valid
				*/

				// f222 prevent casting until it is ready (when using spell hotkeys)
				if (enhancen1) {
					if (!client_spellwait[spellrecall_partymember[x] - 1]) {
						if (spell[spellrecall_partymember[x] - 1][spellrecall_i[x]] > 1) { //valid
							userkey = 4;
							userspell = spellrecall_i[x];
							userspellbook = spellrecall_partymember[x] - 1;
							if (spelltarget[userspell] == 1) {
								txtset(t, "?"); t->d2[0] = 15; NET_send(NETplayer, NULL, t); //keyboard targeting request
								ktar_display = 2.0f;
							}//ktar
						}//valid
					}
				}
				else { // original
					if (spell[spellrecall_partymember[x]-1][spellrecall_i[x]]>1){ //valid
					  userkey=4;
					  userspell=spellrecall_i[x];
					  userspellbook=spellrecall_partymember[x]-1;
					  if (spelltarget[userspell]==1){
						txtset(t,"?"); t->d2[0]=15; NET_send(NETplayer,NULL,t); //keyboard targeting request
						ktar_display=2.0f;
					  }//ktar
					}//valid
				}
          }//party
        }//partymember
      }//!=-1
    }//0


    //keyb target
    if (userkey==3){
      x=-1;
      if (u6okeyhit(U6OK_KTAR1)) x=0;
      if (u6okeyhit(U6OK_KTAR2)) x=1;
      if (u6okeyhit(U6OK_KTAR3)) x=2;
      if (u6okeyhit(U6OK_KTAR4)) x=3;
      if (u6okeyhit(U6OK_KTAR5)) x=4;
      if (u6okeyhit(U6OK_KTAR6)) x=5;
      if (u6okeyhit(U6OK_KTAR7)) x=6;
      if (u6okeyhit(U6OK_KTAR8)) x=7;
      if (u6okeyhit(U6OK_KTAR9)) x=8;
      if (u6okeyhit(U6OK_KTAR0)) x=9;
      if (x!=-1){
        txtset(t,"??"); t->d2[0]=16; t->d2[1]=x; NET_send(NETplayer,NULL,t); //keyboard targeting request
        userkey=0;
      }
    }//userkey 3

    //change party member?
    x=0;
    if (u6okeyhit(U6OK_PARTY1)) x=1;
    if (u6okeyhit(U6OK_PARTY2)) x=2;
    if (u6okeyhit(U6OK_PARTY3)) x=3;
    if (u6okeyhit(U6OK_PARTY4)) x=4;
    if (u6okeyhit(U6OK_PARTY5)) x=5;
    if (u6okeyhit(U6OK_PARTY6)) x=6;
    if (u6okeyhit(U6OK_PARTY7)) x=7;
    if (u6okeyhit(U6OK_PARTY8)) x=8;
    if (x){
      CLIENTplayer->mf=15+x;
      CLIENTplayer->key|=KEYmbclick;

	  // s777 set selected party member
	  //if (tplay->party[x - 1])
	  if (CLIENTplayer->party[x - 1] != NULL) {
		  selectedpartymembern1 = x - 1;

		  if (!selectedpartymemberframelock) {
			  selectedpartymemberframen1 = selectedpartymembern1;
		  }
	  }
    }

    CLIENTplayer->action=0;


    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==5)){
      userkey=0; CLIENTplayer->key-=KEYmbclick;
      //send message to host
      if (CLIENTplayer->mf<=7){
        txtsetchar(t2,45); txtaddchar(t2,CLIENTplayer->mf);
        txtaddshort(t2,CLIENTplayer->mx); txtaddshort(t2,CLIENTplayer->my);
        NET_send(NETplayer,NULL,t2);
      }//CLIENTplayer->mf<=7
    }//not4sale


    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==1)){
      userkey=0;
      CLIENTplayer->key|=KEYu;
      CLIENTplayer->action=1;
    }

    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==2)){
      userkey=0;
      CLIENTplayer->key|=KEYl;
      CLIENTplayer->action=2;
      lookatx = mx;
      lookaty = my;

    }

    if ((CLIENTplayer->key&KEYmbclick)&&(userkey==3)){
      userkey=0;
      CLIENTplayer->key|=KEYa;
      CLIENTplayer->action=3;
    }



    if (u6okeyon(U6OK_QUANTITY)) CLIENTplayer->key|=KEYquan;

    //check if message is different from previous message (ignore mx,my)
    x=0; //send FALSE
    if (CLIENTplayer->key!=CLIENTplayer->key2) x=1;
    //?action
    if (x==1){ //send INPUT update message to host


      txtNEWLEN(t,(DWORD)&CLIENTplayer->mobj-(DWORD)&CLIENTplayer->mf);
      memcpy(t->d,&CLIENTplayer->mf,(DWORD)&CLIENTplayer->mobj-(DWORD)&CLIENTplayer->mf);
      txtset(t2,"?"); t2->d2[0]=5; txtadd(t2,t);
      NET_send(NETplayer,NULL,t2);
    } //x==1

  }//if (tplay->party[0]){ //end of active player output message processing
