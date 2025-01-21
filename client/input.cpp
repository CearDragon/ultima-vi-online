/*


void OnKeyAction(int keyAction, bool keyDown, bool gotKeyRelease) {
    int i = U6OK[keyAction][0];
    if (keyDown)
        keyon[i] = TRUE;
    key[i] = TRUE;
    if (gotKeyRelease)
        key_gotrelease[i] = TRUE;
}

// R666BarAction - Top action bar actions
//
// Attack
// Talk
// Look
// Use
// Set drop location
void R666ActionBarOne(int i) {
    if (!smallwindow) {
        if (actionpending) {
            actionlast = actionpending;

            switch (actionpending) {
                // Attack
                case 1:
                    OnKeyAction(U6OK_ATTACK, false, false);
                    break;
                    // Talk
                case 2:
                case 3:
                case 6:
                    OnKeyAction(U6OK_TALK, true, true);
                    if (actionpending == 6) actionTalkFillText = 1;
                    break;
                    break;
                case 4:
                case 100:
                    OnKeyAction(U6OK_USE, true, true);
                    if (actionpending == 100) actionpending = 501;
                    break;
                case 5:
                    minimaptype = (minimaptype + 1) % (minimaptypemax + 1);
                    SetMiniMapType(minimaptype);
                    break;
                case 7:
                    i = U6OK[U6OK_QUIT][0];
                    keyon[i] = TRUE;
                    key[i] = TRUE;
                    key_gotrelease[i] = TRUE;
                    break;
                case 501:
                    actionpending = 502;
                    break;
                case 503:
                    actionpending = 0;
                    actionreset = 1;
                    itemtoinv = 1;
                    break;
                case 599:
                    actionpending = 0;
                    actionreset = 1;
                    break;
                default:
                    break;
            }
            if (actionpending < 500) actionpending = 0;
        } else if (actionreset != 0) {
            actionreset = 0;
            actionlast = 0;
        } else {
            hituipaneli = -5;
            hituiwidgeti = -5;

            // s444 process mouse clicks on worldmap and worldmapbar
            if (mbclick) {
                if (showworldmapn1 > 0) {
                    hituipaneli = gethituipaneli(omx3, omy3);
                    //hituiwidgeti = -1;

                    // s444 cancel all mouse clicks on world map
                    if (hituipaneli == g_UI_WorldmapPanelId) {
                        hituipaneli = -2;
                        mbclick = 0;
                        mbheld = 0;
                    } else if (hituipaneli == g_UI_WorldmapBarPanelId) {
                        if (mbclick & 1)
                            hituiwidgeti = gethituipanelwidgeti(omx3, omy3, g_UI_WorldmapBarPanelId);
                        else {
                            // s444 cancel non-left mouse clicks on world map bar
                            hituipaneli = -5;
                            mbclick = 0;
                            mbheld = 0;
                        }
                    } else if (!(mbclick & 1)) {
                        hituipaneli = -5;
                    }

                    // s444 cancel right mouse clicks on world map bar
                    */
/*
                    if (hituiwidgeti > 0) {
                        if (!(mbclick & 1)) {
                            hituipaneli = -2;
                            hituiwidgeti = -2;
                            mbclick = 0;
                            mbheld = 0;
                        }
                    }
                    *//*


                    // s444 worldmapbar
                    if (hituiwidgeti > 0) {
                        mbclick = 0;

                        if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6CLOTH) {
                            if (worldmapindexn1 != 1)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 1;

                            if (soundn1 >= 2)
                                soundplay2(u6osound[SOUND_UIACTION], u6osound_volume[SOUND_UIACTION]);
                        }
                        else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6P) {
                            if (worldmapindexn1 != 2)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 2;

                            if (soundn1 >= 2)
                                soundplay2(u6osound[SOUND_UIACTION], u6osound_volume[SOUND_UIACTION]);
                        }
                        else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6G) {
                            if (worldmapindexn1 != 3)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 3;

                            if (soundn1 >= 2)
                                soundplay2(u6osound[SOUND_UIACTION], u6osound_volume[SOUND_UIACTION]);
                        }
                        else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6RUNE) {
                            if (worldmapindexn1 != 4)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 4;

                            if (soundn1 >= 2)
                                soundplay2(u6osound[SOUND_UIACTION], u6osound_volume[SOUND_UIACTION]);
                        }
                        else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_CLOSE) {
                            showworldmapn1 = 0;
                            g_UI_PanelClickable[g_UI_WorldmapPanelId][UI_WIDGET_DEF][UI_STATE_DEF] = 0;

                            if (soundn1 >= 2)
                                soundplay2(u6osound[SOUND_UIACTION], u6osound_volume[SOUND_UIACTION]);
                        }
                    }
                }
            } // mbclick
}
*/
