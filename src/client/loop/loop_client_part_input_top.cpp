// LCS-P2.1: part_input_top — FIRST chunk of the former loop_client.cpp
// monolith (former lines ~1..221). Equip-slot coordinate #define overrides,
// pre-frame input/mouse-wheel/MIDI-keyboard setup, and the exit-request
// handler. MUST remain the first include in loop_client_all.cpp: the
// equip-slot macros defined here are referenced by later parts. Pure
// relocation — do not edit. See docs/plans/plan-loopClientSplit.md.
#include "ui_layout.h"
// Override slot-coordinate macros to use kEquipSlotLayout for RW-P3.4 implementation
#ifdef helmx
#undef helmx
#endif
#define helmx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Helm].x

#ifdef helmy
#undef helmy
#endif
#define helmy u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Helm].y

#ifdef wep_rightx
#undef wep_rightx
#endif
#define wep_rightx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::WepRight].x

#ifdef wep_righty
#undef wep_righty
#endif
#define wep_righty u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::WepRight].y

#ifdef wep_leftx
#undef wep_leftx
#endif
#define wep_leftx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::WepLeft].x

#ifdef wep_lefty
#undef wep_lefty
#endif
#define wep_lefty u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::WepLeft].y

#ifdef armourx
#undef armourx
#endif
#define armourx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Armour].x

#ifdef armoury
#undef armoury
#endif
#define armoury u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Armour].y

#ifdef bootsx
#undef bootsx
#endif
#define bootsx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Boots].x

#ifdef bootsy
#undef bootsy
#endif
#define bootsy u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Boots].y

#ifdef ring_rightx
#undef ring_rightx
#endif
#define ring_rightx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::RingRight].x

#ifdef ring_righty
#undef ring_righty
#endif
#define ring_righty u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::RingRight].y

#ifdef ring_leftx
#undef ring_leftx
#endif
#define ring_leftx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::RingLeft].x

#ifdef ring_lefty
#undef ring_lefty
#endif
#define ring_lefty u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::RingLeft].y

#ifdef neckx
#undef neckx
#endif
#define neckx u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Neck].x

#ifdef necky
#undef necky
#endif
#define necky u6o::client::kEquipSlotLayout[(int)u6o::client::EquipSlotId::Neck].y

if
(wheel_move) {
    if (wheel_move > 0) {
        wheel_move--;
        i = 0xD8;
        keyon[i] = TRUE;
        key[i] = TRUE;
        key_gotrelease[i] = FALSE;
    } else {
        wheel_move++;
        i = 0xD9;
        keyon[i] = TRUE;
        key[i] = TRUE;
        key_gotrelease[i] = FALSE;
    }
}

if
(playinstrument) {
    txtset(t, "?");
    t->d2[0] = 25; //play MIDI keys
    txtset(t2, "?");
    //multiple keys can point to a single sound
    for (i = 0; i <= 255; i++) {
        if (keyhit(i)) {
            if (t->l < 9) {
                if (midikeyboard2[i] != 255) {
                    if (midikeyboard2_keyon[i]) {
                        //if release not received for key, release now
                        txtset(t3, "??");
                        t3->d2[0] = 26; //stop MIDI keys
                        t3->d2[1] = midikeyboard2[i];
                        NET_send(NETplayer, NULL, t3);
                    }
                    t2->d2[0] = midikeyboard2[i];
                    txtadd(t, t2);
                    midikeyboard2_keyon[i] = 1;
                } //!=255
            }
        } //keyhit
    } //i
    if (t->l > 1) {
        NET_send(NETplayer, NULL, t);
    }

    txtset(t, "?");
    t->d2[0] = 26; //stop MIDI keys
    txtset(t2, "?");
    for (i = 0; i <= 255; i++) {
        if (keyon[i] == NULL) {
            if (midikeyboard2_keyon[i]) {
                t2->d2[0] = midikeyboard2[i];
                txtadd(t, t2);
                midikeyboard2_keyon[i] = 0;
            }
        }
    }
    if (t->l > 1) {
        NET_send(NETplayer, NULL, t);
    }
} //playinstrument

if
(setupfail) {
    DestroyWindow(hWnd);
}

if
(exitrequest) {
    exitrequest = FALSE;

    // r777 fix quit sometimes not working bug
    i = U6OK[U6OK_QUIT][0];
    keyon[i] = FALSE;
    key[i] = FALSE;
    key_gotrelease[i] = TRUE;

    if (exitrequest_noconfirm) {
        DestroyWindow(hWnd);
    } else {
        static txt *exitrequest_t = txtnew();
        static long i;
        for (i = 1; i <= 7; i++) {
            if (tplay->party[i]) {
                txtset(exitrequest_t, "Dupre");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Dupre cannot stay in your party, if you leave his items will be lost!",
                        "Ultima 6 Online",
                        MB_OK);
                txtset(exitrequest_t, "Shamino");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Shamino cannot stay in your party, if you leave his items will be lost!",
                        "Ultima 6 Online",
                        MB_OK);
                txtset(exitrequest_t, "Iolo");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Iolo cannot stay in your party, if you leave his items will be lost!", "Ultima 6 Online",
                        MB_OK);
                txtset(exitrequest_t, "Beh Lem");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Beh Lem cannot stay in your party, if you leave his items will be lost!",
                        "Ultima 6 Online",
                        MB_OK);
                txtset(exitrequest_t, "Sentri");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Sentri cannot stay in your party, if you leave his items will be lost!",
                        "Ultima 6 Online",
                        MB_OK);
                txtset(exitrequest_t, "Sherry");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Sherry cannot stay in your party, if you leave her items will be lost!",
                        "Ultima 6 Online",
                        MB_OK);
                txtset(exitrequest_t, "Seggallion");
                if (txtsame(((npc *) tplay->party[i]->more)->name, exitrequest_t))
                    MessageBox(
                        NULL, "Seggallion cannot stay in your party, if you leave his items will be lost!",
                        "Ultima 6 Online", MB_OK);
            }
        }
        if (IDYES == MessageBox(NULL, "Leave Britannia?", "Ultima 6 Online", MB_YESNO)) {
            //send "leaving Britannia message"
            //note: Sent directly skipping NET_send(...) routine to be sure it is sent before exit.
            txtsetshort(exitrequest_t, 1);
            txtaddchar(exitrequest_t, 244);
            send(socketclient[0], (const char *) &exitrequest_t->d2[0], exitrequest_t->l, 0);
            DestroyWindow(hWnd);
        }
    }
}

//end exit request

