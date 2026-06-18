// LCS-P2.2: part_panel_hittest — second head chunk of the former
// loop_client.cpp monolith. Mouse position scaling to the 1024 reference
// resolution, panel/frame hit-testing + click routing
// (checkpanel:/checkobj:/skipbaseframe:/maskcheckfailed:/checkdone:), and the
// new-resolution inventory-click simulation. Pure relocation — do not edit.
// See docs/plans/plan-loopClientSplit.md.
// rrr map/scale mouse location to 1024 resolution (because all logic and graphic is tied to that mode!!!)
if
(smallwindow) {
    if ((mx != omx2) || (my != omy2)) {
        //double multiplierx;
        //double multipliery;
        omx3 = mx;
        omy3 = my;

        // r999 new
        hituipaneli = -1;
        //selectedpartymembern1 = tplay->selected_partymember;

        // r999
        //panelmx[0] = (omx3 - panelx[0]) * panelscalex[0];
        //panelmy[0] = (omy3 - panely[0]) * panelscaley[0];

        /*
		if (windowsizecyclenum == 0) {
			multiplierx = (double) resxo / resxs;
			multipliery = (double) resyo / resys;
//			mx *= multiplierx; my *= multipliery;
		}
		else if (windowsizecyclenum > 0) {
			multiplierx = (double) resxo / resxz;
			multipliery = (double) resyo / resyz;
		}
		*/

        // the scale is only (re)calculated when the resolution changes; in "void refresh(surf* s)" in myddraw.cpp

        //		mx *= multiplierx; my *= multipliery;
        mx *= scalexm;
        my *= scaleym;
        omx2 = mx;
        omy2 = my;
    }

    // r666 actionbar functionality
    if (windowsizecyclenum == 1) {
        if (actionpending > 0) {
            actionlast = actionpending;

            if (actionpending == 1) {
                //U6OK[U6OK_CANCEL][0];
                i = U6OK[U6OK_ATTACK][0];
                //keyon[i]=TRUE;
                key[i] = TRUE;
                //key_gotrelease[i]=TRUE;

                //MessageBox(NULL,"attack!","Ultima 6 Online",MB_OK);
                //txtset(t, "attack!");
                //LOGadd(t);
            } else if (actionpending == 2) {
                i = U6OK[U6OK_TALK][0];
                keyon[i] = TRUE;
                key[i] = TRUE;
                key_gotrelease[i] = TRUE;
            } else if (actionpending == 3) {
                i = U6OK[U6OK_LOOK][0];
                keyon[i] = TRUE;
                key[i] = TRUE;
                key_gotrelease[i] = TRUE;
            } else if (actionpending == 4) {
                i = U6OK[U6OK_USE][0];
                keyon[i] = TRUE;
                key[i] = TRUE;
                key_gotrelease[i] = TRUE;
            } else if (actionpending == 5) {
                minimaptype++;
                if (minimaptype > minimaptypemax)
                    minimaptype = 0;

                if (minimaptype == 1) {
                    minimaptilexstart = 1;
                    minimaptilexend = 3;
                    minimaptileystart = 1;
                    minimaptileyend = 3;
                    minimaptilesurf = minimaptilesurf1;
                    //minimapplayerx = minimapnewx+128-13;
                    //minimapplayery = minimapnewy+128-24;
                    minimapplayerx = 128 - 13;
                    minimapplayery = 128 - 24;
                    minimapstepsize = 4.9f;
                } else if (minimaptype == 2) {
                    minimaptilexstart = 0;
                    minimaptilexend = 4;
                    minimaptileystart = 0;
                    minimaptileyend = 4;
                    minimaptilesurf = minimaptilesurf2;
                    //minimapplayerx = minimapnewx+128-9;
                    //minimapplayery = minimapnewy+128-21;
                    minimapplayerx = 128 - 9;
                    minimapplayery = 128 - 21;
                    minimapstepsize = 2.45f;
                }
            } else if (actionpending == 6) {
                i = U6OK[U6OK_TALK][0];
                keyon[i] = TRUE;
                key[i] = TRUE;
                key_gotrelease[i] = TRUE;
                actiontalkfilltext = 1;
            } else if (actionpending == 7) {
                i = U6OK[U6OK_QUIT][0];
                keyon[i] = TRUE;
                key[i] = TRUE;
                key_gotrelease[i] = TRUE;
            } else if (actionpending == 100) {
                // r777 use item on self: step 1
                i = U6OK[U6OK_USE][0];
                keyon[i] = TRUE;
                key[i] = TRUE;
                key_gotrelease[i] = TRUE;
                actionpending = 501;
                //MessageBox(NULL,"a 100","Ultima 6 Online",MB_OK);
            } else if (actionpending == 501) {
                // r777 click on self
                actionpending = 502;
                //MessageBox(NULL,"a 501","Ultima 6 Online",MB_OK);
            } else if (actionpending == 503) {
                // r777 send item to inventory
                actionpending = 0;
                actionreset = 1;
                itemtoinv = 1;
                //MessageBox(NULL,"a 503","Ultima 6 Online",MB_OK);
            } else if (actionpending == 599) {
                // r777 reset action
                actionpending = 0;
                actionreset = 1;
            }

            if (actionpending < 500)
                actionpending = 0;
        } else if (actionreset != 0) {
            actionreset = 0;
            actionlast = 0;
            //} else if (mbclick & 1) {
        } else {
            hituipaneli = -5;
            hituiwidgeti = -5;

            // s444 process mouse clicks on worldmap and worldmapbar
            if (mbclick) {
                if (showworldmapn1 > 0) {
                    hituipaneli = gethituipaneli(omx3, omy3);
                    //hituiwidgeti = -1;

                    // s444 cancel all mouse clicks on world map
                    if (hituipaneli == uipanelworldmap) {
                        hituipaneli = -2;
                        mbclick = 0;
                        mbheld = 0;
                    } else if (hituipaneli == uipanelworldmapbar) {
                        if (mbclick & 1)
                            hituiwidgeti = gethituipanelwidgeti(omx3, omy3, uipanelworldmapbar);
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
                    /*
					if (hituiwidgeti > 0) {
						if (!(mbclick & 1)) {
							hituipaneli = -2;
							hituiwidgeti = -2;
							mbclick = 0;
							mbheld = 0;
						}
					}
					*/

                    // s444 worldmapbar
                    if (hituiwidgeti > 0) {
                        mbclick = 0;

                        if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6CLOTH) {
                            if (worldmapindexn1 != 1)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 1;
                        } else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6P) {
                            if (worldmapindexn1 != 2)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 2;
                        } else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6G) {
                            if (worldmapindexn1 != 3)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 3;
                        } else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_U6RUNE) {
                            if (worldmapindexn1 != 4)
                                updateworldmapn1 = 1;

                            worldmapindexn1 = 4;
                        } else if (hituiwidgeti == UI_WIDGET_MAPBUTTON_CLOSE) {
                            showworldmapn1 = 0;
                            uipanelhitenable[uipanelworldmap][UI_WIDGET_DEF][UI_STATE_DEF] = 0;
                        }
                    }
                }
            } // mbclick

            // r999 new
            //hituipaneli = gethituipaneli(omx3, omy3);
            //hituiwidgeti = -1;

            // r999 new only process left mouse clicks if it was not previously canceled
            if ((mbclick & 1) && (hituipaneli < -4)) {
                hituipaneli = gethituipaneli(omx3, omy3);
                //hituiwidgeti = -1;

                if (hituipaneli < 0)
                    actionreset = 1;
            }

            // s444 cancel mouse clicks on world map
            /*
			if (hituipaneli == uipanelworldmap) {
				hituipaneli = -1;
				mbclick = 0;
			}
			*/

            // process left mouse clicks on new ui
            if (hituipaneli > 0) {
                hituiwidgeti = -1;

                // r999 new actionbar1
                if (hituipaneli == uipanelactionbar1) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_ATTACK) {
                            // attack
                            actionreset = 0;
                            if (actionlast != 1) {
                                i = U6OK[U6OK_CANCEL][0];
                                keyon[i] = TRUE;
                                key[i] = TRUE;
                                key_gotrelease[i] = TRUE;
                            } else
                                actionreset = 1;

                            actionpending = 1;

                            //MessageBox(NULL,"attack!","Ultima 6 Online",MB_OK);
                            //txtset(t, "attack!");
                            //LOGadd(t);
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_TALK) {
                            // talk
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;

                            if (actionlast != 2)
                                actionpending = 2;
                            else
                                actionreset = 1;

                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_LOOK) {
                            // look
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;

                            if (actionlast != 3)
                                actionpending = 3;
                            else
                                actionreset = 1;

                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_USE) {
                            // use
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;

                            if (actionlast != 4) {
                                if ((keyon[VK_SHIFT]) && CLIENTplayer->mobj) {
                                    // r777 set use item on self action
                                    actionpending = 100;
                                } else
                                    actionpending = 4;
                            } else
                                actionreset = 1;

                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_DROP) {
                            // drop item
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;

                            if (CLIENTplayer->mobj)
                                actionpending = 510;
                            else {
                                if (setdroplocation)
                                    setdroplocation = 0;
                                else
                                    setdroplocation = 1;
                            }

                            mbclick = 0;
                        }
                    }
                } // actionbar1
                // actionbar2
                else if (hituipaneli == uipanelactionbar2) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_FOOD) {
                            // food (use)
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;

                            if (CLIENTplayer->mobj) {
                                // r777 set use item on self action
                                actionpending = 100;
                            } else
                                actionreset = 1;

                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_WORLDMAP) {
                            // worldmap
                            showworldmapn1++;
                            if (showworldmapn1 > 1) {
                                showworldmapn1 = 0;
                                uipanelhitenable[uipanelworldmap][UI_WIDGET_DEF][UI_STATE_DEF] = 0;
                            } else {
                                uipanelhitenable[uipanelworldmap][UI_WIDGET_DEF][UI_STATE_DEF] = 1;
                            }
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONBUTTON_COMBATLOG) {
                            // combatlog
                            combatinfo++;
                            txtsetchar(t3, 255);

                            if (combatinfo > 1) {
                                combatinfo = 0;
                                STATUSMESSadd("Combat log disabled.", 1);
                            } else {
                                STATUSMESSadd("Combat log enabled. (experimental)  Warning: may cause game to crash.",
                                              1);
                            }
                        }
                    }
                } // actionbar2
                // optionbar1
                else if (hituipaneli == uipaneloptionbar1) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        if (hituiwidgeti == UI_WIDGET_OPTIONBUTTON_ENHANCE) {
                            // game enhancement
                            enhancen1++;
                            if (enhancen1 > 2) {
                                enhancen1 = 0;
                                STATUSMESSadd("Game option: set to default", 1);
                            } else
                                STATUSMESSadd("Game option: set to alternate ", 1, enhancen1);

                            //updateoptioninfo();
                            updatepartyframen1 = 1;
                        } else if (hituiwidgeti == UI_WIDGET_OPTIONBUTTON_MINIMAP) {
                            // minimap toggle
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 5;
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_OPTIONBUTTON_QUIT) {
                            // quit
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 7;
                            mbclick = 0;
                            //MessageBox(NULL,"quit","Ultima 6 Online",MB_OK);
                        }
                    }
                } // optionbar1
                // actiontalkbar1
                else if (hituipaneli == uipanelactiontalkbar1) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_NAME) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "name");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_JOB) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "job");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_HEAL) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "heal");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_CURE) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "cure");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_RESURRECT) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "resurrect");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_INSURANCE) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "insurance");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_SPELL) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "spell");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_REAGENT) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "reagent");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_POTION) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "potion");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_STAFF) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "staff");
                            mbclick = 0;
                        }
                    }
                } // actiontalkbar1
                // actiontalkbar2
                else if (hituipaneli == uipanelactiontalkbar2) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_YES) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "yes");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_NO) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "no");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_SELL) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "sell");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_BULK) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "bulk");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_BUY) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "buy");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_ARMOR) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "armor");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_ARMS) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "arms");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_WEAPON) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "weapon");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_ARROWS) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "arrows");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_BOLTS) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "bolts");
                            mbclick = 0;
                        }
                    }
                } // actiontalkbar2
                // actiontalkbar3
                else if (hituipaneli == uipanelactiontalkbar3) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_DEPOSIT) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "deposit");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_WITHDRAW) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "withdraw");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_BALANCE) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "balance");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_EXCHANGE) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "exchange");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_HOUSE) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "house");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_PAYMENT) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "payment");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_JOIN) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "join");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_LEAVE) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "leave");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_BAGS) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1, "bags");
                            mbclick = 0;
                        } else if (hituiwidgeti == UI_WIDGET_ACTIONTALKBUTTON_HELP) {
                            actionreset = 0;
                            i = U6OK[U6OK_CANCEL][0];
                            keyon[i] = TRUE;
                            key[i] = TRUE;
                            key_gotrelease[i] = TRUE;
                            actionpending = 6;
                            txtset(newt1,
                                   "kal lor <-- replace with all uppercase (i.e. \"KAL LOR\") and press Enter. WARNING: you will LOSE some (i.e. 1/16) of ... your (TOTAL) experience!  This help request will teleport you to the starting location.");
                            mbclick = 0;
                        }
                    }
                } // actiontalkbar3
                // partymemberbar1
                else if (hituipaneli == uipanelpartymemberbar1) {
                    hituiwidgeti = gethituipanelwidgeti(omx3, omy3, hituipaneli);

                    if (hituiwidgeti > 0) {
                        selectedpartymemberframen1 = hituiwidgeti - 1;
                    }
                } // partymemberbar1
                // party member inventory frame
                else if (hituipaneli == uipanelpartymember0) {
                    // partymemberlock
                    if (testhituipanel(omx3, omy3, uipanelpartymemberlock)) {
                        selectedpartymemberframelock++;

                        if (selectedpartymemberframelock > 1)
                            selectedpartymemberframelock = 0;
                    } // partymemberlock
                } // party member inventory frame
            }

            /*
			txtset(t4, "hittest uipi=");
			txtnumint(t5, hituipaneli);
			txtadd(t4, t5);
			txtset(t5, " uiwi=");
			txtadd(t4, t5);
			txtnumint(t5, hituiwidgeti);
			txtadd(t4, t5);
			LOGadd(t4);
			*/
        }
        //if (mb&1){
    }
}


//check for midi keys that need to be cleared
i3=
0;
for
(i2=
0;
i2
<=
15;
i2
++
)
{
  for (i=0;i<=255;i++){

    if (midikeystack[i2][i]){
      i3=1;
      if (midikeywait[i2][i]<=(ett-8.0f)){
        midiOutShortMsg(midiout_handle,0x00000080+i*256+i2);

        midikeystack[i2][i]=0;
      }}}
}

if
((i3
==
0
)
&&
midipause
)
{
  if (playinstrument==0){
    midipause=0;

    if (U6O_DISABLEMUSIC==FALSE){
      if (u6omidivolume) u6omidi->Play();
    }

  }
}


clientframe=
!
clientframe;
//if (!NEThost) clientframe=1;
clientframe=
1;

f=
1.0f;
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
  if (xray) xray--;
  if (peer) peer--;
  if (tmap) tmap--;
}
ktar_display
-=
et;
if
(ktar_display<0.0f)
ktar_display=
0.0f;
wizardeyetimeleft
-=
et;
if
(wizardeyetimeleft<0.0f)
wizardeyetimeleft=
0.0f;

isit=TRUE;

if
(

!
NEThost
)
 {
  if (lastsecond==-1) lastsecond=ett; x=ett; //detect frame rate
  if (x!=lastsecond){
    lastsecond=x; framerate=framecount; framecount=0;
#ifdef _DEBUG
    // RW-P0.2: per-second client-metrics log to make it easy to verify
    // window-resize plumbing during development. Cheap (one OutputDebugString
    // per second) and Debug-only so it never ships to release builds.
    {
      char dbgbuf[256];
      _snprintf(dbgbuf, sizeof(dbgbuf),
        "[u6o] client=%ldx%ld src=%ux%u blit_off=%ld,%ld blit_scale=%.3f "
        "wsc=%u smallwin=%d hWnd=%p\n",
        clientW, clientH, resxo, resyo,
        blit_offx, blit_offy, blit_scale,
        windowsizecyclenum, (int)smallwindow,
        (void*)hWnd);
      dbgbuf[sizeof(dbgbuf)-1] = '\0';
      OutputDebugStringA(dbgbuf);
    }
#endif
  }
  framecount++;
}

// RW-P1.4: react to WM_SIZE-driven client-area changes outside of WndProc.
// At this point the renderer is unchanged (back buffer is still 1024x768),
// so the only useful work this hook does today is give the rest of the
// codebase one well-defined seam through which all future surface/UI
// re-layout work in RW-P2..RW-P4 will be plumbed.
if
(dirtyClientSize) {
    dirtyClientSize = false;
    // refresh()/blit_letterbox already recomputes blit_offx/offy/scale every
    // frame from GetClientRect, so input mapping stays correct without any
    // extra work here. Reserved for future surface recreation (RW-P2.2) and
    // UI anchor recomputation (RW-P3.3).
    // RW-P2.2: ask the viewport seam to re-create back buffers at the new
    // client size, then reposition anchored UI panels. (windowResize gate
    // removed 2026-05-27 — always-on.)
    recreateBackbuffers((int) clientW, (int) clientH);
    // RW-P3.3: re-anchor the five static UI panels. NOTE: until P2.2
    // grows the back buffer with the window, panels must anchor against
    // the back-buffer dimensions, not the client-window dimensions ?
    // they draw onto `ps` which is still 1024x768 regardless of how
    // big the window has become. Anchoring to clientW/H would push
    // panels off the back-buffer edge and they'd disappear behind the
    // letterbox bars. Once the back buffer follows clientW/H, this can
    // change to clientW/clientH for true edge-following.
    RepositionAnchoredPanels(backbufferW(), backbufferH());
#ifdef _DEBUG
    {
        char dbgbuf[160];
        _snprintf(dbgbuf, sizeof(dbgbuf),
                  "[u6o] OnClientResized: %ldx%ld\n",
                  clientW, clientH);
        dbgbuf[sizeof(dbgbuf) - 1] = '\0';
        OutputDebugStringA(dbgbuf);
    }
#endif
}

// rrr moved fix mouse logic to (near) top of source file
//fix mouse 512x384
/*
if (smallwindow){
  if ((mx!=omx2)||(my!=omy2)){
    mx*=2; my*=2;
    omx2=mx; omy2=my;
  }
}
*/


omb=FRAME_mb; //find old button value using FRAME_mb (which now contains old value)
FRAME_mb=mb; //backup physical mouse value
if
((mb&

1
)
==
0
)
FRAME_mbl [0]=FALSE; //disabled mouse locks
if
((mb&

2
)
==
0
)
FRAME_mbl [1]=FALSE;
if
(FRAME_mbl[0]
==
TRUE
)
{ //mask physical values if locked
  if (mb&1) mb=mb-1;
}
if
(FRAME_mbl[1]
==
TRUE
)
{
  if (mb&2) mb=mb-2;
}
pn=firstpanel;
checkpanel :
if
(pn
!=
NULL
)
{ //frame available
  if ((pn->graphic!=NULL)&&(pn->size_x==0)&&(pn->size_y==0)){
    x3=pn->offset_x; y3=pn->offset_y;
    x4=x3+pn->graphic->d.dwWidth; y4=y3+pn->graphic->d.dwHeight;
  }else{
    x3=pn->offset_x; y3=pn->offset_y;
    x4=x3+pn->size_x; y4=y3+pn->size_y;
  }
  if (mx>=x3){ if (my>=y3){ if (mx<x4){ if (my<y4){ //mouse in within rectangle
    if (pn->graphic_mask!=NULL){ //graphic_mask check
      if (point(pn->graphic_mask,mx-pn->offset_x,my-pn->offset_y)==0) goto maskcheckfailed;
    }
    if ((FRAME_mbl[0]==FALSE)&&((mb&1)==1)){
      FRAME_mbl[0]=TRUE; //lock mouse button
      FRAME_mblf[0]=pn;
    }
    if ((FRAME_mbl[1]==FALSE)&&((mb&2)==2)){
      FRAME_mbl[1]=TRUE; //lock mouse button2
      FRAME_mblf[1]=pn;
    }
    if ((FRAME_mbl[0]==TRUE)&&(FRAME_mblf[0]==pn)) mb=mb|1;
    if ((FRAME_mbl[1]==TRUE)&&(FRAME_mblf[1]==pn)) mb=mb|2;

    x2=0; //=1 to select

    FRM_type=(FRM_TYPE*)pn->firstobject;
checkobj:
    if (FRM_type!=NULL){
      x=FRM_type->type;
      if (x==1){ //process: FRM_ONOFF type
        tonoff=(FRM_ONOFF*)FRM_type;
        if (tonoff->value==0){ //rectangle of current button icon
          x3=pn->offset_x+tonoff->offset_x; y3=pn->offset_y+tonoff->offset_y;
          x4=x3+tonoff->graphic_off->d.dwWidth; y4=y3+tonoff->graphic_off->d.dwHeight;
        }
        else{
          x3=pn->offset_x+tonoff->offset_x+tonoff->offset2_x; y3=pn->offset_y+tonoff->offset_y+tonoff->offset2_y;
          x4=x3+tonoff->graphic_on->d.dwWidth; y4=y3+tonoff->graphic_on->d.dwHeight;
        }
        if (mx>=x3){ if (my>=y3){ if (mx<x4){ if (my<y4){ //mouse in within rectangle
          if (tonoff->hold==FALSE){ //button does not need to be held
            if ((omb&1)==0&&(mb&1)==1){ //button has just been pressed
              if (tonoff->value==0) soundplay(tonoff->sound_on);
              if (tonoff->value==1) soundplay(tonoff->sound_off);
              tonoff->value++; if(tonoff->value==2) tonoff->value=0;
              x2=1;
            }
          }
          else{ //button must be held
            if ((omb&1)==0&&(mb&1)==1){ //button has just been pressed
              soundplay(tonoff->sound_on);
              tonoff->value=1;
              x2=1; //update
              ONOFF_hold=tonoff; //set external disable pointer
            }
          }
          goto skipbaseframe; //skip further processing
        }}}} //mouse within button
      } //process finish: FRM_ONOFF type
      if (x==4){ //process: FRM_INPUT type
        tinp=(FRM_INPUT*)FRM_type;
        x3=pn->offset_x+tinp->offset_x; y3=pn->offset_y+tinp->offset_y;
        x4=x3+tinp->graphic->d.dwWidth; y4=y3+tinp->graphic->d.dwHeight;
        if (mx>=x3){ if (my>=y3){ if (mx<x4){ if (my<y4){ //mouse in within rectangle
          if ((omb&1)==0&&(mb&1)==1){ //button has just been pressed
            tinp->enterpressed=FALSE;
            GETINPUT_setup(tinp->input,&tinp->enterpressed,tinp->length_limit);
            x2=1; //update
          }
          goto skipbaseframe; //skip further processing
        }}}} //mouse within graphic/area
      } //process finish: FRM_INPUT type
      FRM_type=(FRM_TYPE*)FRM_type->next; goto checkobj;
    }

    // r222 this is where mouse click is checked! no changes are made here

    pn->mouse_over=TRUE;
    if (pn == vf && tplay && tplay->x) {
      long tpx_legacy, tpy_legacy;
      getscreenoffset_legacy(tplay->x, tplay->y, &tpx_legacy, &tpy_legacy);
      pn->mouse_x = (short)((tpx - tpx_legacy) * 32 + (mx - pn->offset_x));
      pn->mouse_y = (short)((tpy - tpy_legacy) * 32 + (my - pn->offset_y));
    } else {
      pn->mouse_x=mx-pn->offset_x;
      pn->mouse_y=my-pn->offset_y;
    }


    if (mbclick&1) pn->mouse_click|=1;
    if (mb&1){



      pn->mouse_pressed=TRUE;
    }

    if (pn==vf){
      if (mb&2){
        vf_mb2_x=mx; vf_mb2_y=my;
      }
    }

    if (pn->move==TRUE){
      if ((omb&1)==0&&(mb&1)==1){ //button has been pressed
        if (firstpanel!=pn) soundplay(pn->sound_select);
        x2=1; //update
      }
      if ((omb&2)==0&&(mb&2)==2){ //button2 has been pressed
        drg=pn; //set external drag pointer
        x2=1; //update
        FRAME_drg_begin=FALSE;
      }
    }
skipbaseframe:

    if (pn->move==TRUE){

      if (pn->no_focus==FALSE){
        if (x2==1&&firstpanel!=pn){ //update: bring frame to top
          pn2=(FRAME*)pn->previous;
          pn3=(FRAME*)pn->next;
          pn2->next=pn3;
          if (pn3!=NULL) pn3->previous=pn2;
          pn->previous=NULL;
          pn->next=firstpanel;
          firstpanel->previous=pn;
          firstpanel=pn;
        }
      } //no focus
    }

    mb=0; //clear all button information
    goto checkdone; //skip further frame processing
maskcheckfailed:;
  }}}}
  pn=(FRAME*)pn->next; goto checkpanel; //next frame pointer
}
checkdone :
