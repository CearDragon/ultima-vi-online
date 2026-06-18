// =============================================================================
// LHS-P2.1: part_open_housekeeping -- OPENS the host per-tick block.
// LHS brace-seam: this file OPENS the if(NEThost){ block that is CLOSED in
// loop_host_part_tail_npcframe.cpp. MUST stay FIRST in the umbrella.
// Contents: the define_both.h auto-completion helper, the block open, and head
// per-tick housekeeping (economy adjust, house payments, leverchange0,
// negatemagic_field, MIDI/nomidichange). Pure relocation.
// =============================================================================
#include "define_both.h" /* luteijn: won't really be included again, but helps with auto-completion of variable names. */



if
(NEThost) {
    //host
    //U6O_ERRORTAG

    btime2 += et / 150.0f; //non-rolled over btime!
    x = btime2 / 24;
    btime = btime2 - (float) x * 24.0f;
    x %= 7;
    bday = x + 1;

    if (lastsecond == -1) lastsecond = ett;
    x = ett; //detect frame rate
    if (x != lastsecond) {
        lastsecond = x;
        framerate = framecount;
        framecount = 0;
    }
    framecount++;

    //Adjust economy values
    for (i = 0; i <= 1023; i++) {
        for (i2 = 0; i2 <= 3; i2++) {
            if (economy_setup[i][i2] == TRUE) {
                if (economy_change[i][i2]) {
                    f2 = abs(economy_change[i][i2]);
                    f = 3600.0f / f2;
                    //f is how often it should occur without the exponential factor
                    f3 = ((float) economy_value[i][i2]) / ((float) economy_limit[i][i2]); //0-1
                    f3 *= 2.0f; //0-2
                    f3 += 1.0f; //1-3
                    //f3 is a multiplier to make it take 3 times as long to increase as value reaches limit
                    f *= f3;

                    if (int((ett / f)) != int((ett / f) - (et / f))) {
                        if (economy_change[i][i2] > 0) economy_value[i][i2]++;
                        if (economy_change[i][i2] < 0) economy_value[i][i2]--;
                        if (economy_value[i][i2] < 0) economy_value[i][i2] = 0;
                        if (economy_value[i][i2] > economy_limit[i][i2]) economy_value[i][i2] = economy_limit[i][i2];
                    } //f
                } //economy_change
            } //economy_setup==TRUE
        }
    } //i,i2


    //deduct house payments from housesav
    f = 300.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        housesav[0].flags += 5;
        if (housesav[0].flags >= 1440) {
            housesav[0].flags = 0;
            for (i = 1; i <= 255; i++) {
                if (housesav[i].flags & 1) {
                    //currently occupied
                    x = housesav[i].gold - housecost[i];
                    if (x <= 0) x = 0;
                    housesav[i].gold = x;
                    if (housesav[i].gold == 0) {
                        for (x2 = 0; x2 <= playerlist_last; x2++) {
                            if (playerlist[x2]) {
                                //if (playerlist[x2]->net!=INVALID_NET){
                                if (playerlist[x2]->GNPCflags[28] == i) goto skipevict; //can't evict online player
                                //}
                            }
                        } //x2,...
                        //evict owner
                        //MessageBox(NULL,"HOUSESTORAGECHECK: EVICT","Ultima 6 Online",MB_OK);
                        housesav[i].flags--; //remove house occupied flag
                        housesav_update();
                    } //gold==0
                skipevict:;
                }
            } //i
        } //housesav[0].flags>=1440
        housesav_update();
    } //f


    /* hireling respawn*/
    for (i = 0; i <= HIRELINGS_MAX - 1; i++) {
        hirl_wait[i] -= et;
        if (hirl_wait[i] <= 0) {
            if (hirl_obj[i]) {
                hirl_obj[i]->info |= 112;
                tnpc = (npc *) hirl_obj[i]->more;
                hirl_obj[i] = 0;
                addhireling(i, tnpc->schedule);
            } else {
                addhireling(i, 0);
            }
        }
    }


    //doorclose: relock door after waiting for 2 hours
    for (i = 0; i <= doorclose_last; i++) {
        if (doorclose_wait[i] >= 1.0f) {
            doorclose_wait[i] += et;
            if (doorclose_wait[i] >= 12288.0f) {
                doorclose_wait[i] = 0;
                doorclose_obj[i]->type = doorclose_oldtype[i];
                OBJcheckflags(doorclose_obj[i]->x, doorclose_obj[i]->y);
                if (doorclose_obj2[i]) {
                    doorclose_obj2[i]->type = doorclose_oldtype2[i];
                    OBJcheckflags(doorclose_obj2[i]->x, doorclose_obj2[i]->y);
                } //doorclose_obj2[i]
            } //wait>=8192.0f
        } //wait>=1.0f
    } //i

    //leverchange: revert lever position after waiting for 2 hours
    for (i = 0; i <= leverchange_last; i++) {
        if (leverchange_wait[i] >= 1.0f) {
            leverchange_wait[i] += et;
            if (leverchange_wait[i] >= 12288.0f) {
                leverchange_wait[i] = 0;
                if (leverchange_obj[i]->type != leverchange_oldtype[i]) {
                    use_basic_toggle(NULL, leverchange_obj[i]);
                }
            } //wait>=8192.0f
        } //wait>=1.0f
    } //i

    ouln = -1; //object update/active list num
    //add fixed objects to update
    if (f_ouln != -1) {
        for (i = 0; i <= f_ouln; i++) {
            myobj = f_oul[i];
            if (myobj->x || myobj->y) {
                ouln++;
                oul[ouln] = f_oul[i];
                oul[ouln]->info |= 32768;
            } //x&y not null
        }
    }

    /*
  for (i=0;i<=15;i++){
  if (stolenitemwarning[i]){
  stolenitemwarning_wait[i]-=et;
  if (stolenitemwarning_wait[i]<=0.0f) stolenitemwarning[i]=NULL;
  }//stolenitemwarning[i]
  }//i
  */

    //wind: change direction
    windnew = 0;
    f = 1.0f;
    if (int((ett / f)) != int((ett / f) - (et / f))) {
        if (rnd * 32.0f < 1.0f) {
            windnew = 1;
            x = rnd * 3;
            x--;
            windx = x;
            x = rnd * 3;
            x--;
            windy = x;
        }
    }

    //recalculate moon phase!
    moonphase = 255; //NULL moon phase
    f5 = 32;

    f = btime2 * 1.125f + 3.0f;
    x = f / 24;
    f -= (float) x * 24.0f;
    if ((f > 5) && (f < 20)) {
        if (f >= 12.0f) f3 = -(f - 12.0f);
        else f3 = 12.0f - f;
        if (f >= 12.0f) f5 = f - 12.0f;
        else f5 = 12.0f - f; //NEW
        f3 /= 1.125f;
        f2 = btime + f3;
        f4 = f;
        f = 12.0f;
        if (f2 >= 24.0f) f2 -= 24.0f;
        if (f2 < 0.0f) f2 += 24.0f;
        if (f2 >= f) f2 = f2 - f;
        else f2 = f2 + (24.0f - f);
        f2 += 1.5f;
        if (f2 >= 24.0f) f2 -= 24.0f;
        moonphase = f2 / 3.0f; //NEW
    }

    f = btime2 * 1.1875f + 5.0f;
    x = f / 24;
    f -= (float) x * 24.0f;
    if ((f > 5) && (f < 20)) {
        if (f >= 12.0f) f3 = -(f - 12.0f);
        else f3 = 12.0f - f;
        if (f >= 12.0f) f6 = f - 12.0f;
        else f6 = 12.0f - f; //NEW
        if (f6 < f5) {
            f3 /= 1.1875f;
            f2 = btime + f3;
            f4 = f;
            f = 12.0f;
            if (f2 >= 24.0f) f2 -= 24.0f;
            if (f2 < 0.0f) f2 += 24.0f;

            if (f2 >= f) f2 = f2 - f;
            else f2 = f2 + (24.0f - f);
            f2 += 1.5f;
            if (f2 >= 24.0f) f2 -= 24.0f;
            moonphase = f2 / 3.0f; //NEW
        } //f6<f5
    }

    if (moonphase != 255) {
        //check moongates!
        if (showmoongates == 0) {
            //object *moongate[8][2];
            //unsigned char showmoongates=0;
            //unsigned short moongatex[8],moongatey[8];
            for (i = 0; i <= 7; i++) {
                OBJadd(moongatex[i], moongatey[i], moongate[i][0]);
                OBJadd(moongatex[i] - 1, moongatey[i], moongate[i][1]);
            }
            showmoongates = 1;
        }
    } else {
        //255

        if (showmoongates) {
            for (i = 0; i <= 7; i++) {
                OBJremove(moongate[i][0]);
                OBJremove(moongate[i][1]);
            }
            showmoongates = 0;
        }
    } //showmoongates


    stormcloak_last = -1;


    sfbufferwait += et;
    f = 1.0f / 8.0f;
    if (sfbufferwait >= f) {
        sfbufferwait = 0.0f;
        sfbuffersend = 1;
    }


    wizardeyesi = 0;
    for (tpl = 0; tpl <= playerlist_last; tpl++) {
        if (playerlist[tpl]) {
            if (playerlist[tpl]->net) {
                tplayer = playerlist[tpl];
                if (tplayer->wizardeyetimeleft) {
                    wizardeyesi++;
                    wizardeyesx[wizardeyesi] = tplayer->wizardeyex;
                    wizardeyesy[wizardeyesi] = tplayer->wizardeyey;
                }
            }
        }
    }
