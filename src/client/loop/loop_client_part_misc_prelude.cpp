// LCS-P2.3: part_misc_prelude — third head chunk of the former loop_client.cpp
// monolith (former lines ~1313..1605). The remaining small top-level blocks
// that run after the panel hit-test routine and before the intro/menu state
// machine (`if (intro) {`). Pure relocation — do not edit. See
// docs/plans/plan-loopClientSplit.md.
// r222 handle mouse click for party[0] (the player avatar) for new resolution mode top-right inventory window
// r222 we are just simulating the clicking/variables as if it is being performed on the actual inventory in the 1024 res.  the rest is handle by the original 1024 res code/logic.
if
(smallwindow &&windowsizecyclenum
==
1
)
 {
	//double multiplierx = (double)resxz / resxo;
	//double multipliery = (double)resyz / resyo;


	//if (newmodestatus >= 5) {
	if (itemtoinv) {
		// r777 send cursor item to inventory; simulate clicks
		itemtoinv = 0;

		//pmf = party_frame[0];
		pmf = party_frame[selectedpartymemberframen1];
		pmf->mouse_over = TRUE; // set this to true so that the original logic will run and handle the inventory actions/clicks.

		// simulate/set the position where the mouse is clicked in the inventory window
		pmf->mouse_x = pmf->offset_x+10;
		pmf->mouse_y = pmf->offset_y+10;

		pmf->mouse_click |= 1;
		//MessageBox(NULL,"item to inv 2","Ultima 6 Online",MB_OK);

	}
	else if (actionpending == 510) {
		// Drop item action: remove "focus" from party member frames to allow item to drop on ground
		for (i = 0; i <= 7; i++) {
			pmf = party_frame[i];
			pmf->mouse_over = FALSE;
		}
	}
	else {
		if (hituipaneli < -4)
			if (testhituipanel(omx3, omy3, uipanelpartymember0))
				hituipaneli = uipanelpartymember0;

		for (i = 0; i < partyframenewmax; i++) {
			party_frame_new[i]->mouse_over = FALSE;
			//pmf = party_frame[i];
			pmf = party_frame[selectedpartymemberframen1];

			//		if ((omx3 >= resxn1m + 3) && (omy3 <= 256))

			// not true anymore --> omx3 and omy3 does not need to be scaled because the (new) top-right inventory window is not scaled at all (it is always the 1024 res scaled-size; i.e. not scaled)
			//if ((panelmx[0] >= party_frame_new[i]->offset_x) && (panelmx[0] <= party_frame_new[i]->offset_x + pmf->graphic->dwWidth)
				//&& (panelmy[0] >= party_frame_new[i]->offset_y) && (panelmy[0] <= party_frame_new[i]->offset_y + pmf->graphic->dwHeight))
			if (hituipaneli == uipanelpartymember0)
				party_frame_new[i]->mouse_over = TRUE;
			//party_frame[0] = TRUE;
			if (party_frame_new[i]->mouse_over == TRUE) {
				pmf->mouse_over = TRUE; // set this to true so that the original logic will run and handle the inventory actions/clicks.
				//CLIENTplayer->mf = i;
				//CLIENTplayer->mx = pmf->mouse_x;
				//CLIENTplayer->my = pmf->mouse_y;
				//pmf->mouse_x = 130;
				//pmf->mouse_y = 130;
				//if (pmf->mouse_click == TRUE) CLIENTplayer->key |= KEYmbclick;
				//pmf->mouse_click = TRUE;
				//CLIENTplayer->key |= KEYmbclick;
				/*
				if (u6okeyhit(U6OK_SCROLLUP)) {
				CLIENTplayer->mx = 118;
				CLIENTplayer->my = 202;
				CLIENTplayer->key |= KEYmbclick;
				if (CLIENTplayer->key2&KEYmbclick) CLIENTplayer->key2 -= KEYmbclick;
				}
				if (u6okeyhit(U6OK_SCROLLDOWN)) {
				CLIENTplayer->mx = 118;
				CLIENTplayer->my = 240;
				CLIENTplayer->key |= KEYmbclick;
				if (CLIENTplayer->key2&KEYmbclick) CLIENTplayer->key2 -= KEYmbclick;
				}
				*/

				// simulate/set the position where the mouse is clicked in the inventory window
				//pmf->mouse_x = panelmx[0] - party_frame_new[i]->offset_x;
				//pmf->mouse_y = panelmy[0] - party_frame_new[i]->offset_y;
				pmf->mouse_x = omx3 - uipanelx[uipanelpartymember0][UI_WIDGET_DEF][UI_STATE_DEF];
				pmf->mouse_y = omy3 - uipanely[uipanelpartymember0][UI_WIDGET_DEF][UI_STATE_DEF];

				// inverse scaling
				if (uiscaling) {
					pmf->mouse_x = pmf->mouse_x / uipanelscalex[uipanelpartymember0][UI_WIDGET_DEF][UI_STATE_DEF];
					pmf->mouse_y = pmf->mouse_y / uipanelscaley[uipanelpartymember0][UI_WIDGET_DEF][UI_STATE_DEF];
				}


				//if (mbclick & 1) pmf->mouse_click |= 1;
				if (mbclick & 1) {
					pmf->mouse_click |= 1;
					actionreset = 1;
				}

				if (mb & 1) {
					pmf->mouse_pressed = TRUE;
				}
			}//pmf->mouse_over==TRUE

		/*
		txtset(t2, "pmf mx ");
		txtnumint(t3, pmf->mouse_x);
		txtadd(t2, t3);
		txtadd(t2, " // my ");
		txtnumint(t3, pmf->mouse_y);
		txtadd(t2, t3);
		LOGadd(t2);
		*/
		}//i
	}
} else
 {
	if (enhancen1 >= 2) {
		if (actionpending == 510) {
			// Drop item action: remove "focus" from party member frames to allow item to drop on ground
			for (i = 0; i <= 7; i++) {
				pmf = party_frame[i];
				pmf->mouse_over = FALSE;
			}
		}
	}
}


//external functions, results of button presses
if
((ONOFF_hold
!=
NULL
)
&&
((FRAME_mb&

1
)
==
0
)
)
{
  soundplay(ONOFF_hold->sound_off);
  ONOFF_hold->value=0;
  ONOFF_hold=NULL;
}
if
((FRAME_mb&

2
)
==
0
)
drg=NULL;

if
(drg
!=
NULL
)
{ //drag panel


  if (drg->move==TRUE){
    x=mx-omx; y=my-omy;
    if ((x!=0)||(y!=0)){
      if (FRAME_drg_begin==FALSE) soundplay(drg->sound_move);
      FRAME_drg_begin=TRUE;
      drg->offset_x+=x;
      drg->offset_y+=y;
      // RW: a real user drag of qkstf/volcontrol flips them into
      // "user-positioned" mode so RepositionAnchoredPanels stops
      // snapping them back to the anchored default on resize / show
      // toggle, and the per-frame cltset mirror starts persisting the
      // chosen offset to settings.bin. The cache is updated here
      // (from the post-drag, pre-clamp value) — NEVER from the
      // per-frame auto-clamp — so a far-right position saved while
      // maximized survives a session that opens at the default size.
      if (drg == qkstf) {
        u6o::client::g_qkstf_user_positioned = true;
        u6o::client::g_qkstf_user_x = drg->offset_x;
        u6o::client::g_qkstf_user_y = drg->offset_y;
      }
      if (drg == volcontrol) {
        u6o::client::g_volcontrol_user_positioned = true;
        u6o::client::g_volcontrol_user_x = drg->offset_x;
        u6o::client::g_volcontrol_user_y = drg->offset_y;
      }
      if (drg == statusmessage_viewprev) {
        u6o::client::g_statusprev_user_positioned = true;
        u6o::client::g_statusprev_user_x = drg->offset_x;
        u6o::client::g_statusprev_user_y = drg->offset_y;
      }
    }
  }
}
if
((FRAME_mbl[0]
==
FALSE
)
&&
((mb&

1
)
==
1
)
)
{
  FRAME_mbl[0]=TRUE; //lock mouse button for user
  FRAME_mblf[0]=NULL;
}
if
((FRAME_mbl[1]
==
FALSE
)
&&
((mb&

2
)
==
2
)
)
{
  FRAME_mbl[1]=TRUE; //lock mouse button2  for user
  FRAME_mblf[1]=NULL;
}
if
((FRAME_mbl[0]
==
TRUE
)
&&
(FRAME_mblf[0]
==
NULL
)
)
mb=mb
|
1;
if
((FRAME_mbl[1]
==
TRUE
)
&&
(FRAME_mblf[1]
==
NULL
)
)
mb=mb
|
2;

//CLIENT2HOST KEEPALIVE MESSAGE
f=
4.0f;
if
(

int ((ett
/
f
)
)
!=
int ((ett
/
f
)
-
(et
/
f
)
)
)
{
  txtset(t,"?"); t->d2[0]=251; NET_send(NETplayer,NULL,t);
}


