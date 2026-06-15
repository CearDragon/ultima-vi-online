// LCS-P3.2: part_intro_a — FIRST chunk of the intro / main-menu state machine.
// LCS brace-seam: this file OPENS the `if (intro) { ... }` block that is CLOSED
// in loop_client_part_intro_d.cpp. Contains intro states 100/101/102 (intro
// animation / backdrop). The trailing `} //intro==102` leaves the `if (intro)`
// block intentionally OPEN — intro_b continues it. Pure relocation — do not
// edit. See docs/plans/plan-loopClientSplit.md.
if
(intro) {
    f = intro_timer;


    //refresh backdrop
    if (intro != 100) {
        cls(ps, (2 << 11) + (27 << 5) + 18);
        x2 = (4 << 11) + (54 << 5) + 31;
        x3 = (3 << 11) + (46 << 5) + 27;
        x4 = (2 << 11) + (34 << 5) + 21;
        for (x = 0; x <= 255; x++) {
            ps->o2[intro_stary[x] * 1024 + intro_starx[x]] = x2;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] + 1] = x3;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] - 1] = x3;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] + 1024] = x3;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] - 1024] = x3;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] - 1025] = x4;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] - 1023] = x4;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] + 1023] = x4;
            ps->o2[intro_stary[x] * 1024 + intro_starx[x] + 1025] = x4;
        }
        img0(ps, 1024 - 656, 768 - 369, intro_startup);
    } else {
        cls(ps, 0);
    }


    static unsigned char intro_setup = 0;

    //exit request?
    if (fs->mouse_click) {
        if ((mx > (1024 - 21 - 4)) && (my < (21 + 4))) {
            exitrequest = TRUE;
            fs->mouse_click = NULL;
        }
    }


    //GYPSY
    static unsigned char vquan[8];
    static unsigned short vcol[8];
    static unsigned short vx[8]; //original x offset
    static char vaction = -1;
    static unsigned char vaction_done = 0;
    static unsigned char vques[28];
    static char vtext;


    if (intro == 100) {
        if (intro_setup == 0) {
            intro_setup = 1;
            f = 0;
            for (x = 0; x <= 7; x++) vquan[x] = 7;
            ZeroMemory(&vques, 28);
            vcol[0] = (0 >> 3 << 11) + (0 >> 16 << 5) + (252 >> 3);
            vcol[1] = (252 >> 3 << 11) + (252 >> 2 << 5) + (84 >> 3);
            vcol[2] = (252 >> 3 << 11) + (0 >> 2 << 5) + (0 >> 3);
            vcol[3] = (0 >> 3 << 11) + (252 >> 2 << 5) + (0 >> 3);
            vcol[4] = (252 >> 3 << 11) + (96 >> 2 << 5) + (0 >> 3);
            vcol[5] = (168 >> 3 << 11) + (0 >> 2 << 5) + (168 >> 3);
            vcol[6] = (252 >> 3 << 11) + (252 >> 2 << 5) + (252 >> 3);
            vcol[7] = (72 >> 3 << 11) + (72 >> 2 << 5) + (72 >> 3);
            vx[0] = 51;
            vx[1] = 72;
            vx[2] = 93;
            vx[3] = 114;
            vx[4] = 187;
            vx[5] = 208;
            vx[6] = 229;
            vx[7] = 250;

            img0_0key(intro_vial, 21);
            img0_0key(intro_hpl0, 21);
            img0_0key(intro_hpl2, 21);
            img0_0key(intro_hpl3, 21);
            img0_0key(intro_hpr0, 21);
            img0_0key(intro_hpr2, 21);
            img0_0key(intro_hpr3, 21);


            vtext = -2;
        } //intro_setup==0


        f += et / 1.0f; //action timer

        cls(ps320200, 0);
        img(ps320200, 0, 0, intro_gypsy); //backdrop!

        y5 = 1;
        if (vaction != -1) {
            f2 = f;
            if (f2 > 1.0f) {
                if (vaction_done == 0) {
                    vaction_done = 1;
                    vquan[vaction]--;
                }
                f2 = 2.0f - f;
            }


            y2 = 66 - f2 * 34.0f;


            if (vaction <= 3) {
                x2 = (132 - vx[vaction]) * f2 + vx[vaction];
                img0(ps320200, x2 - 12, y2 + 12, intro_arml);
                y5 = 0;
                img(ps320200, 0, 97, intro_gypsy2); //backdrop!
                cls(intro_hps0, vcol[vaction]);
                img0(intro_hps0, 0, 0, intro_hpl0);
                img0_0key(intro_hps0, 32);
                cls(intro_hps2, vcol[vaction]);
                img0(intro_hps2, 0, 0, intro_hpl2);
                img0_0key(intro_hps2, 32);
                cls(intro_hps3, vcol[vaction]);
                img0(intro_hps3, 0, 0, intro_hpl3);
                img0_0key(intro_hps3, 32);


                if (f2 <= 0.5f) img0(ps320200, x2 - 2, y2, intro_hps0);
                if ((f2 > 0.5) && (f2 <= 0.9)) img0(ps320200, x2 - 16, y2, intro_hps2);
                if (f2 > 0.9f) img0(ps320200, x2 - 16, y2, intro_hps3);
            } else {
                x2 = (132 + 32 + 8 - vx[vaction]) * f2 + vx[vaction];
                img0(ps320200, x2 - 8, y2 + 12, intro_armr);
                y5 = 0;
                img(ps320200, 0, 97, intro_gypsy2); //backdrop!
                cls(intro_hps0, vcol[vaction]);
                img0(intro_hps0, 0, 0, intro_hpr0);
                img0_0key(intro_hps0, 32);
                cls(intro_hps2, vcol[vaction]);
                img0(intro_hps2, 0, 0, intro_hpr2);
                img0_0key(intro_hps2, 32);
                cls(intro_hps3, vcol[vaction]);
                img0(intro_hps3, 0, 0, intro_hpr3);
                img0_0key(intro_hps3, 32);
                if (f2 <= 0.5f) img0(ps320200, x2 - 2, y2, intro_hps0);
                if ((f2 > 0.5) && (f2 <= 0.9)) img0(ps320200, x2 - 8, y2, intro_hps2);
                if (f2 > 0.9f) img0(ps320200, x2 - 10, y2, intro_hps3);
            }


            if (f > 2) {
                f = 0;
                vaction = -1;
            }
        }


        if (y5) img(ps320200, 0, 97, intro_gypsy2); //backdrop!


        for (x = 0; x <= 7; x++) {
            if (vaction != x) {
                cls(intro_s64, vcol[x]);
                cls(intro_s128, 0);
                img(intro_s128, 0, 41 - (float) vquan[x] * 4.3f, intro_s64);
                img0(intro_s128, 0, 0, intro_vial);
                img(intro_svial, 0, 0, intro_s128);
                img0_0key(intro_svial, 32 * 4);
                img0_0key(intro_svial, 32);
                img0(ps320200, vx[x], 66, intro_svial);
            }
        }


        //draw liquid in vial

        x2 = 0; //height
        for (x = 0; x <= 7; x++) {
            x2 += (7 - vquan[x]);
        }
        x2 = x2 + (x2 >> 1); //*1.5


        for (y = 103; y > (103 - x2); y--) {
            for (x = 148; x <= 171; x++) {
            vialmix:
                x3 = rnd * 8;
                x4 = rnd * 7;
                if (vquan[x3] > x4) goto vialmix;
                ps320200->o2[y * 320 + x] = vcol[x3];
            }
        }


        img0(ps320200, 142, 56, intro_bigvial);


        if (vtext >= 0) {
            img(ps320200, 279, 174, intro_ab);

            x2 = vtext;
            x = -1;
            x4 = 0;
            x3 = 0; //if (vques[vtext]==1) x3=1;

            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_COMPASSION } //i3,d3
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_VALOR } //i3,s3
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_JUSTICE } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_SACRIFICE } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_HONOR } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_SPIRITUALITY } //i3,a1
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_HUMILITY } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_VALOR } //d3,s3
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_JUSTICE } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_SACRIFICE } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_HONOR } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_SPIRITUALITY } //d3,a1
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_HUMILITY } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_JUSTICE } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_SACRIFICE } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_HONOR } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_SPIRITUALITY } //s3,a1
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_HUMILITY } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_SACRIFICE } //i,d
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_HONOR } //s,d
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_SPIRITUALITY } //X,a1
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_HUMILITY } //OK,X
            x++;
            if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_HONOR } //s,i
            x++;
            if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_SPIRITUALITY } //X,OK
            x++;
            if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_HUMILITY } //a1,X
            x++;
            if (x == x2) if (x3) { U6O2_HONOR } else { U6O2_SPIRITUALITY } //X,a1
            x++;
            if (x == x2) if (x3) { U6O2_HONOR } else { U6O2_HUMILITY } //OK,X
            x++;
            if (x == x2) if (x3) { U6O2_SPIRITUALITY } else { U6O2_HUMILITY } //a1,X
            x5 = x4;
            x2 = vtext;
            x = -1;
            x4 = 0;
            x3 = 1;

            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_COMPASSION } //i3,d3
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_VALOR } //i3,s3
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_JUSTICE } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_SACRIFICE } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_HONOR } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_SPIRITUALITY } //i3,a1
            x++;
            if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_HUMILITY } //i3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_VALOR } //d3,s3
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_JUSTICE } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_SACRIFICE } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_HONOR } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_SPIRITUALITY } //d3,a1
            x++;
            if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_HUMILITY } //d3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_JUSTICE } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_SACRIFICE } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_HONOR } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_SPIRITUALITY } //s3,a1
            x++;
            if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_HUMILITY } //s3,X
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_SACRIFICE } //i,d
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_HONOR } //s,d
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_SPIRITUALITY } //X,a1
            x++;
            if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_HUMILITY } //OK,X
            x++;
            if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_HONOR } //s,i
            x++;
            if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_SPIRITUALITY } //X,OK
            x++;
            if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_HUMILITY } //a1,X
            x++;
            if (x == x2) if (x3) { U6O2_HONOR } else { U6O2_SPIRITUALITY } //X,a1
            x++;
            if (x == x2) if (x3) { U6O2_HONOR } else { U6O2_HUMILITY } //OK,X
            x++;
            if (x == x2) if (x3) { U6O2_SPIRITUALITY } else { U6O2_HUMILITY } //a1,X
            if ((x4 <= 2) || (x4 == 6)) {
                x4 = 2;
                goto gotx4;
            }
            if (x4 != 7) {
                x4 = 1;
                goto gotx4;
            }
            x4 = 0;
        gotx4:
            if ((x5 <= 2) || (x5 == 6)) {
                x5 = 2;
                goto gotx5;
            }
            if (x5 != 7) {
                x5 = 1;
                goto gotx5;
            }
            x5 = 0;
        gotx5:
            x = ett * 2.0f;
            x &= 1;
            if (x) {
                if (x4 >= x5) img(ps320200, 279, 174, intro_aba);
            } else {
                if (x5 >= x4) img(ps320200, 279, 174, intro_abb);
            }
        }

        img(ps640400, ps320200);
        img(ps, 192, 184, ps640400);


        txtset(t4, "");
        switch (vtext) {
            case 0:
                txtset(t, "Entrusted to deliver an uncounted purse of gold, thou dost meet a poor");
                txtset(t2, "beggar. Dost thou:");
                txtset(t3, "A) Deliver the gold knowing the Trust in thee was well-placed; or");
                txtset(t4, "B) Show Compassion, giving the Beggar a coin, knowing it won't be missed?");
                break;
            case 1:
                txtset(t, "Thou hast been prohibited by thy absent Lord from joining thy friends in a");
                txtset(t2, "close pitched battle. Dost thou:");
                txtset(t3, "A) Refrain, so thou may Honestly claim obedience; or");
                txtset(t4, "B) Show Valor, and aid thy comrades, knowing thou may deny it later?");
                break;
            case 2:
                txtset(t, "A merchant owes thy friend money, now long past due. Thou dost see the same");
                txtset(t2, "merchant drop a purse of gold. Dost thou:");
                txtset(t3, "A) Honestly return the purse intact; or");
                txtset(t4, "B) Justly give thy friend a portion of the gold first?");
                break;
            case 3:
                txtset(t, "Thee and thy friend are valiant but penniless warriors. Thou both go out to");
                txtset(t2, "slay a mighty dragon. Thy friend thinks he slew it, thee did. When asked, dost thou:");
                txtset(t3, "A) Truthfully claim the gold; or");
                txtset(t4, "B) Allow thy friend the large reward?");
                break;
            case 4:
                txtset(t, "Thou art sworn to protect thy Lord at any cost, yet thou knowest he hast");
                txtset(t2, "committed a crime. Authorities ask the of the affair, dost thou:");
                txtset(t3, "A) Break thine oath by Honestly speaking; or");
                txtset(t4, "B) Uphold Honor by silently keeping thine oath?");
                break;
            case 5:
                txtset(t, "Thy friend seeks admittance to thy Spiritual order. Thou art asked to vouch");
                txtset(t2, "for his purity of Spirit, of which thou art unsure. Dost thou:");
                txtset(t3, "A) Honestly express thy doubt; or");
                txtset(t4, "B) Vouch for him, hoping for his Spiritual improvement?");
                break;
            case 6:
                txtset(t, "Thy Lord mistakenly believes he slew a dragon. Thou hast proof that thy lance");
                txtset(t2, "felled the beast. When asked, dost thou:");
                txtset(t3, "A) Honestly claim the kill and the prize; or");
                txtset(t4, "B) Humbly permit thy Lord his belief?");
                break;
            case 7:
                txtset(t, "Thou dost manage to disarm thy mortal enemy in a duel. He is at thy mercy.");
                txtset(t2, "Dost thou:");
                txtset(t3, "A) Show Compassion by permitting him to yield; or");
                txtset(t4, "B) Slay him as expected of a Valiant duelist?");
                break;
            case 8:
                txtset(t, "After 20 years thou hast found the slayer of thy best friends. The villain");
                txtset(t2, "proves to be a man who provides the sole support for a young girl. Dost thou:");
                txtset(t3, "A) Spare him in Compassion for the girl; or");
                txtset(t4, "B) Slay him in the name of Justice?");
                break;
            case 9:
                txtset(t, "Thee and thy friends have been routed and ordered to retreat. In defiance of");
                txtset(t2, "thy orders, dost thou:");
                txtset(t3, "A) Stop in Compassion to aid a wounded companion; or");
                txtset(t4, "B) Sacrifice thyself to slow the pursuing enemy, so others can escape?");
                break;
            case 10:
                txtset(t, "Thou art sworn to uphold a Lord who participates in the forbidden torture of");
                txtset(t2, "prisoners. Each night their cries of pain reach thee. Dost thou:");
                txtset(t3, "A) Show Compassion by reporting the deeds; or");
                txtset(t4, "B) Honor thy oath and ignore the deeds?");
                break;
            case 11:
                txtset(t, "Thou hast been taught to preserve all life as sacred. A man lies fatally");
                txtset(t2, "stung by a venomous serpent. He pleads for a merciful death. Dost thou:");
                txtset(t3, "A) Show Compassion and end his pain; or");
                txtset(t4, "B) Heed thy Spiritual beliefs and refuse?");
                break;
            case 12:
                txtset(t, "As one of the King's Guard, thy Captain has asked that one amongst you visit");
                txtset(t2, "a hospital to cheer the children with tales of thy valiant deeds. Dost thou:");
                txtset(t3, "A) Show thy Compassion and play the braggart; or");
                txtset(t4, "B) Humbly let another go?");
                break;
            case 13:
                txtset(t, "Thou hast been sent to secure a needed treaty with a distant Lord. Thy host");
                txtset(t2, "is agreeable to the proposal but insults thy country at dinner. Dost thou:");
                txtset(t3, "A) Valiantly bear the slurs; or");
                txtset(t4, "B) Justly rise and demand an apology?");
                break;
            case 14:
                txtset(t, "A mighty knight accosts thee and demands thy food. Dost thou:");
                txtset(t2, "A) Valiantly refuse and engage the knight; or");
                txtset(t3, "B) Sacrifice thy food unto the hungry knight?");
                break;
            case 15:
                txtset(t, "During battle thou art ordered to guard thy commmander's empty tent. The");
                txtset(t2, "battle goes poorly and thou dost yearn to aid thy fellows. Dost thou:");
                txtset(t3, "A) Valiantly enter the battle to aid thy companions; or");
                txtset(t4, "B) Honor thy post as guard?");
                break;
            case 16:
                txtset(t, "A local bully pushes for a fight. Dost thou:");
                txtset(t2, "A) Valiantly trounce the rogue; or");
                txtset(t3, "B) Decline, knowing in thy Spirit that no lasting good will come of it?");
                break;
            case 17:
                txtset(t, "Although a teacher of music, thou art a skillful wrestler. Thou hast been");
                txtset(t2, "asked to fight in a local championship. Dost thou:");
                txtset(t3, "A) Accept the invitation and Valiantly fight to win; or");
                txtset(t4, "B) Humbly decline knowing thou art sure to win?");
                break;
            case 18:
                txtset(t, "During a pitched battle, thou dost see a fellow desert his post, endangering");
                txtset(t2, "many. As he flees, he is set upon by several enemies. Dost thou:");
                txtset(t3, "A) Justly let him fight alone; or");
                txtset(t4, "B) Risk Sacrificing thine own life to aid him?");
                break;
            case 19:
                txtset(t, "Thou hast sworn to do thy Lord's bidding in all. He covets a piece of land");
                txtset(t2, "and orders the owner removed. Dost thou:");
                txtset(t3, "A) Serve Justice, refusing to act, thus being disgraced; or");
                txtset(t4, "B) Honor thine oath and unfairly evict the landowner?");
                break;
            case 20:
                txtset(t, "Thou dost believe that virtue resides in all people. Thou dost see a rogue");
                txtset(t2, "steal from thy Lord. Dost thou:");
                txtset(t3, "A) Call him to Justice; or");
                txtset(t4, "B) Personally try to sway him back to the Spiritual path of good?");
                break;
            case 21:
                txtset(t, "Unwitnessed, thou hast slain a great dragon in self defense. A poor warrior");
                txtset(t2, "claims the offered reward. Dost thou:");
                txtset(t3, "A) Justly step forward to claim the reward; or");
                txtset(t4, "B) Humbly go about life, secure in thy self-esteem?");
                break;
            case 22:
                txtset(t, "Thou art a bounty hunter sworn to return an alleged murderer. After his");
                txtset(t2, "capture, thou believest him to be innocent. Dost thou:");
                txtset(t3, "A) Sacrifice thy sizeable bounty for thy belief; or");
                txtset(t4, "B) Honor thy oath to return him as thou hast promised?");
                break;
            case 23:
                txtset(t, "Thou hast spent thy life in charitable and righteous work. Thine uncle the");
                txtset(t2, "innkeeper lies ill and asks you to take over his tavern. Dost thou:");
                txtset(t3, "A) Sacrifice thy life of purity to aid thy kin; or");
                txtset(t4, "B) Decline & follow thy Spirit's call?");
                break;
            case 24:
                txtset(t, "Thou art an elderly, wealthy eccentric. Thy end is near. Dost thou:");
                txtset(
                    t2,
                    "A) Donate all thy wealth to feed hundreds of starving children, and receive public adulation; or");
                txtset(t3, "B) Humbly live out thy life, willing thy fortune to thy heirs?");
                break;
            case 25:
                txtset(t, "In thy youth thou pledged to marry thy sweetheart. Now thou art on a sacred");
                txtset(t2, "quest in distant lands. Thy sweetheart asks thee to keep thy vow. Dost thou:");
                txtset(t3, "A) Honor thy pledge to wed; or");
                txtset(t4, "B) Follow thy Spiritual crusade?");
                break;
            case 26:
                txtset(t, "Thou art at a crossroads in thy life. Dost thou:");
                txtset(t2, "A) Choose the Honorable life of a Paladin, striving for Truth and Courage; or");
                txtset(t3, "B) Choose the Humble life of a Shepherd, and a world of simplicity and peace?");
                break;
            case 27:
                txtset(t, "Thy parents wish thee to become an apprentice. Two positions are available.");
                txtset(t2, "Dost thou:");
                txtset(t3, "A) Become an acolyte in the Spiritual order; or");
                txtset(t4, "B) Become an assistant to a humble village cobbler?");
                break;
            case -2:
                txtset(t, "\"At last thou hast come to fulfill thy destiny,\" the gypsy says.");
                txtset(t2, "She smiles, as if in great relief.");
                txtset(t3, "\"Sit before me now, and I shall pour the light of Virtue into the");
                txtset(t4, "shadows of thy future.\"");
                break;
            case -1:
                txtset(t, "On a wooden table eight bottles stand, a rainbow of bubbling liquids.");
                txtset(t2, "\"Behold the virtues of the Avatar\", the woman says.");
                txtset(t3, "\"Let us begin the casting!\"");
                txtset(t4, "(Recommended buttons for answers are blinking.)");
                break;
            case -3:
                txtset(t, "\"The path of the Avatar lies beneath thy feet, worthy ");
                txtadd(t, u6o_name);
                txtadd(t, ",\"");
                txtset(t2, "the gypsy intones. With a mysterious smile, she passes you the flask");
                txtset(t3, "of shimmering liquids. \"Drink of these waters and go forth among our");
                txtset(t4, "people, who shall receive thee in Joy!\"");
        }


        x = 192 + 32 - 16;
        y = 184 + 132 * 2 - 8 - 4;
        txtcol = rgb(255, 255, 255);
        txtout(ps, x + 1, y + 1, t);
        y += 20;
        txtcol = rgb(255, 255, 255);
        txtout(ps, x + 1, y + 1, t2);
        y += 20;
        txtcol = rgb(255, 255, 255);
        txtout(ps, x + 1, y + 1, t3);
        y += 20;
        txtcol = rgb(255, 255, 255);
        txtout(ps, x + 1, y + 1, t4);


        if (keyhit(65)) {
            //"A" key
            mx = (279 * 2) + 192;
            my = (174 * 2) + 184;
            goto abkey;
        }
        if (keyhit(66)) {
            //"B" key
            mx = ((279 + 17) * 2) + 192;
            my = (174 * 2) + 184;
            goto abkey;
        }

        //for compatibility, simulate a mouse click in the top left corner
        if (keyhit(VK_RETURN)) {
            mx = 0;
            my = 0;
            goto abkey;
        }

        if (fs->mouse_click) {
            fs->mouse_click = NULL;
        abkey:


            if (vtext < 0) {
                if (vtext == -3) {
                    intro = 101;
                    intro_setup = 0;
                    goto intro_done;
                }
                vtext++;
                vaction_done = 1;
                if (vtext == 0) vtext = rnd * 28;
            } else {
                if (vaction_done) {
                    x = (mx - 192) / 2;
                    y = (my - 184) / 2;
                    static unsigned char stats_warning = 1;

                    if ((x >= 279) && (x < (279 + 16)) && (y >= 174) && (y < (174 + 16))) {
                        vques[vtext] = 1;
                        if (x5 > x4) {
                            if (stats_warning) {
                                stats_warning = 0;
                                if (IDNO == MessageBox(
                                        NULL,
                                        "If you select a non-blinking answer the total of your strength, dexterity and intelligence will be lower. Are you sure?",
                                        "Ultima 6 Online", MB_YESNO))
                                    vques[vtext] = 0;
                            }
                        }
                    }

                    if ((x >= (279 + 17)) && (x < (279 + 17 + 16)) && (y >= 174) && (y < (174 + 16))) {
                        vques[vtext] = 2;
                        if (x4 > x5) {
                            if (stats_warning) {
                                stats_warning = 0;
                                if (IDNO == MessageBox(
                                        NULL,
                                        "If you select a non-blinking answer the total of your strength, dexterity and intelligence will be lower. Are you sure?",
                                        "Ultima 6 Online", MB_YESNO))
                                    vques[vtext] = 0;
                            }
                        }
                    }

                    if (vques[vtext] == 0) goto ab_noselection;


                    x2 = vtext;
                    x = -1;
                    x4 = 0;
                    x3 = 0;
                    if (vques[vtext] == 1) x3 = 1;
                    u6o_vq[vtext] = vques[vtext] - 1;
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_COMPASSION } //i3,d3
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_VALOR } //i3,s3
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_JUSTICE } //i3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_SACRIFICE } //i3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_HONOR } //i3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_SPIRITUALITY } //i3,a1
                    x++;
                    if (x == x2) if (x3) { U6O2_HONESTY } else { U6O2_HUMILITY } //i3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_VALOR } //d3,s3
                    x++;
                    if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_JUSTICE } //d3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_SACRIFICE } //d3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_HONOR } //d3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_SPIRITUALITY } //d3,a1
                    x++;
                    if (x == x2) if (x3) { U6O2_COMPASSION } else { U6O2_HUMILITY } //d3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_JUSTICE } //s3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_SACRIFICE } //s3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_HONOR } //s3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_SPIRITUALITY } //s3,a1
                    x++;
                    if (x == x2) if (x3) { U6O2_VALOR } else { U6O2_HUMILITY } //s3,X
                    x++;
                    if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_SACRIFICE } //i,d
                    x++;
                    if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_HONOR } //s,d
                    x++;
                    if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_SPIRITUALITY } //X,a1
                    x++;
                    if (x == x2) if (x3) { U6O2_JUSTICE } else { U6O2_HUMILITY } //OK,X
                    x++;
                    if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_HONOR } //s,i
                    x++;
                    if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_SPIRITUALITY } //X,OK
                    x++;
                    if (x == x2) if (x3) { U6O2_SACRIFICE } else { U6O2_HUMILITY } //a1,X
                    x++;
                    if (x == x2) if (x3) { U6O2_HONOR } else { U6O2_SPIRITUALITY } //X,a1
                    x++;
                    if (x == x2) if (x3) { U6O2_HONOR } else { U6O2_HUMILITY } //OK,X
                    x++;
                    if (x == x2) if (x3) { U6O2_SPIRITUALITY } else { U6O2_HUMILITY } //a1,X

                    //show vial action (needs to relate to question!)
                    vaction = x4;
                    f = 0;
                    vaction_done = 0;

                    for (x = 0; x <= 27; x++) if (vques[x] == 0) goto vquesnew;
                vquesnew:
                    if (x == 28) {
                        vtext = -3;
                    } else {
                    nextvques:
                        x = rnd * 28;
                        if (vques[x]) goto nextvques;
                        vtext = x;
                    } //28
                ab_noselection:;
                } //vaction_done
            }
        }
    } //intro==100


    if (intro == 101) {
        if (intro_setup == 0) {
            intro_setup = 1;
            f = 0;
        }
        img(ps320200, 0, 0, intro_flask);
        f += et;
        for (y = 34; y <= 128; y++) {
            for (x = 116; x <= 203; x++) {
            vialmix2:
                x3 = rnd * 8;
                x4 = rnd * 7;
                if (vquan[x3] > x4) goto vialmix2;
                if (ps320200->o2[y * 320 + x] == 0) ps320200->o2[y * 320 + x] = vcol[x3];
            }
        }

        img(ps640400, ps320200);
        img(ps, 192, 184, ps640400);

        x = 144;
        y = 640;
        txtset(t, "As you drink from the flask, vertigo overwhelms you. A soothing mist obscures the gypsy's face,");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x + 1, y + 1, t);
        txtset(t, "and you sink without fear into an untroubled sleep.");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x + 1, y + 1 + 20, t);

        if (fs->mouse_click || keyhit(VK_RETURN)) {
            //final backup of userinfo
            tfh = open2("userinfo.txt", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
            txtfileout(u6o_user_name, tfh);
            txtfileout(u6o_user_password, tfh);
            close(tfh);

            //save walkthru pos
            txtset(t, ".\\dr\\walkthru.pos");
            tfh = open2(t, OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
            put(tfh, &walkthru_pos, 4); //lines to skip of the walkthru
            close(tfh);
            walkthru_pos_skip = 0;

            tfh = open(".\\dr\\controls.bin");
            get(tfh, &U6OK, 512);
            get(tfh, &U6OK_FLAGS, 128);
            close(tfh);

            u6o_createcharacter = 1; //ALLOW CREATION OF NEW PLAYER

            fs->mouse_click = NULL;
            intro = 0;
            intro_setup = 0;
            fs->offset_x = 1024;
            fs->graphic = NULL;
        }
    } //intro==101

    if (intro == 102) {
        if (intro_setup == 0) {
            intro_setup = 1;
            f = 0;
        }
        x = f * 16.0f;
        if (x > 191) x = 191;
        img(ps320200, -x, 0, intro_caravan);

        f += et;
        img(ps640400, ps320200);
        img(ps, 192, 184, ps640400);

        x = 192 + 32 - 16;
        y = 616 - 16;
        txtset(t, "\"Welcome, O Seeker!\"");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "A lonely stroll along an unfamiliar forest path brings you upon a curious gypsy");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "wagon, its exotic colors dappled in the summer shade.");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "A woman's voice rings out with friendship, beckoning you into across the");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x, y, t);
        y += 32;
        txtset(t, "wagon's threshold and, as it happens, into another life....");
        txtcol = rgb(255, 255, 255);
        txtouts(ps, x, y, t);


        //txtset(t,"and you sink without fear into an untroubled sleep.");
        //txtcol=rgb(255,255,255); txtout(ps,x+1,y+1+20,t);


        if (fs->mouse_click || keyhit(VK_RETURN)) {
            fs->mouse_click = NULL;
            intro = 100;
            intro_setup = 0;
        }
    } //intro==102
