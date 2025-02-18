#include "spell_code.h"
#include "../use_code/use_code.h"
#include "../define_host.h"
#include "../function_host.h"
#include "../function_both.h"

/* First Circle */
U6O_SPELL_FUNCTION(create_food) {
    if (stormcloakcheck2(x, y, myplr)) {
        return SPELL_INVALID;
    }

    object *myobj2 = OBJfindlast(x, y);
    if (myobj2 && !(objectInfo[sprlnk[myobj2->Type()] + (myobj2->type >> 10)].flags & 512)) {
        return SPELL_INVALID;
    }

    int amount = static_cast<int>(rnd * (3 + (caster->intelligence >> 4) + 1));
    if (amount) {
        myobj2 = OBJnew();
        myobj2->type = 129; // meat
        myobj2->more2 = amount;
        myobj2->info |= 112; // set flags as temp OBJ
        OBJadd(x, y, myobj2);
        return SPELL_SUCCESS;
    }

    return SPELL_FAILURE;
}

U6O_SPELL_FUNCTION(detect_magic) {
    return SPELL_NOTDONE;
}

U6O_SPELL_FUNCTION(detect_trap) {
    int x3, y3, traps_found = 0;
    object *myobj2;
    static txt *t = txtnew();
    static txt *t2 = txtnew();

    if (stormcloakcheck2(x, y, myplr)) {
        return SPELL_INVALID;
    }

    if ((int) (rnd * (8 + 5 * 1)) > (int) (rnd * (caster->intelligence + 1))) {
        return SPELL_FAILURE;
    }

    for (int spellx = -20; spellx <= 20; spellx++) {
        for (int spelly = -16; spelly <= 16; spelly++) {
            x3 = x + spellx;
            y3 = y + spelly;
            if (x3 >= 0 && y3 >= 0 && x3 <= 2047 && y3 <= 1023) {
                if ((myobj2 = od[y3][x3]) && myobj2->type == 173) { // trap
                    traps_found++;
                    myobj2->info |= (1 << 9); // make visible
                }
            }
        }
    }

    txtset(t, "?");
    t->d2[0] = 8;
    if (traps_found) {
        txtnumint(t2, traps_found);
        txtadd(t, t2);
        txtadd(t, traps_found == 1 ? " trap detected!" : " traps detected!");
    } else {
        txtadd(t, "No traps detected.");
    }
    NET_send(NETplayer, myplr->net, t);
    return SPELL_SUCCESS;
}

U6O_SPELL_FUNCTION(dispel_magic) {
    return SPELL_NOTDONE;
}

U6O_SPELL_FUNCTION(douse) {
    object *myobj2;
    unsigned long special_effect;
    int x1;
    int x2;
    int x3;
    int x4;

    if (stormcloakcheck2(x, y, myplr)) {
        return SPELL_INVALID;
    }
    if (myobj2 = OBJfindlast(x, y)) {
        x3 = myobj2->Type();
        x4 = myobj2->type >> 10;

        switch (x3) {
            case 206: //brazier
                if (x4 > 1) {
                    //holy flames, no touch
                    return SPELL_FAILURE;
                }
                //fall through on purpose
            case 253: //campfire
            case 164: //fireplace
            case 122: //candle
            case 145: //candelabra
            case 223: //powder keg
                x1 = (int) (rnd * (8 + 5 * 1));
                x2 = (int) (rnd * (caster->intelligence + 1));
                if (x2 < x1) {
                    //fail
                    return SPELL_FAILURE;
                }
                OBJsave(myobj2->x, myobj2->y);
                if (x4 & 1) {
                    if (x3 == 223) { //powder keg special more value.
                        myobj2->more2 = myplr->id;
                    }
                    use_basic_toggle(NULL, myobj2);
                }
                break;
            default:
                return SPELL_INVALID;
        }
        special_effect = SFnew(x, y); //destination is more important than the source
        special_effects[special_effect].type = SF_BLUE_BOLT;

        if ((myobj2 = npc_to_obj(caster, myplr))) { // get the corresponding npc pointer.
            special_effects[special_effect].x = myobj2->x; /* the caster's x and y */
            special_effects[special_effect].y = myobj2->y;
        } else {
            special_effects[special_effect].x = x; /* appear right at the target instead */
            special_effects[special_effect].y = y;
        }
        special_effects[special_effect].x2 = x;
        special_effects[special_effect].y2 = y;
        special_effects[special_effect].more = 0xFFFF;
        special_effects[special_effect].wait = 1;
        return SPELL_SUCCESS;
    }
    return SPELL_INVALID;
}

U6O_SPELL_FUNCTION(explosion) {
    return 0;
}

U6O_SPELL_FUNCTION(fire_field) {
    return 0;
}

U6O_SPELL_FUNCTION(harm) {
    return 0;
}

U6O_SPELL_FUNCTION(heal) {
    return 0;
}

U6O_SPELL_FUNCTION(help) {
    return 0;
}

U6O_SPELL_FUNCTION(ignite) {
    object *myobj2;
    unsigned long special_effect;
    int x1;
    int x2;
    int x3;
    int x4;

    if (stormcloakcheck2(x, y, myplr)) {
        return SPELL_INVALID;
    }
    myobj2 = OBJfindlast(x, y);
    if (myobj2) {
        x3 = myobj2->Type();
        x4 = myobj2->type >> 10;

        switch (x3) {
            case 206: //brazier
                if (x4 > 1) {
                    //holy flames, no touch
                    return SPELL_FAILURE;
                }
                //fall through on purpose
            case 253: //campfire
            case 164: //fireplace
            case 122: //candle
            case 145: //candelabra
            case 223: //powder keg
            case 53: //web
                x1 = (int) (rnd * (8 + 5 * 1));
                x2 = (int) (rnd * (caster->intelligence + 1));
                if (x2 < x1) {
                    //fail
                    return SPELL_SUCCESS;
                }
                OBJsave(myobj2->x, myobj2->y);
                //if (!x4&1){ //original thing. suspect operator precendence issues
                //if (!x4&1 || ((x3==164) && (x4==2))){ //fix for fireplace
                if (!(x4 &
                      1)) { // fix for operator precedence thing. we want to check if bit 1 of x4 (0,1,2,3) is not set, not if bit 1 is set on the negated version of x4 (which will be 1 or 0)

                    if (x3 == 223) { //powder keg
                        myobj2->info |= 7680; //1111000000000 obj reserved timer
                        myobj2->more2 = tplayer->id;
                        stealing(tplayer, myobj2);
                    } else if (x3 ==
                               53) { //web  /* luteijn: does x4&1 have to be 0?  mose: doesn't have to be 0, but atm it doesn't get another values, but in future it might change so its probably a good idea to keep it inside the check */
                        VLNKremove(myobj2);
                        OBJremove(myobj2);
                        myobj2 = NULL;
                    } else {
                        use_basic_toggle(NULL, myobj2);
                    }
                }
                break;
            default:
                return SPELL_INVALID;
        }
        special_effect = SFnew(x, y); //destination is more important than the source
        special_effects[special_effect].type = SF_RED_BOLT;
        if ((myobj2 = npc_to_obj(caster, myplr))) { // get the corresponding npc pointer.
            special_effects[special_effect].x = myobj2->x; /* the caster's x and y */
            special_effects[special_effect].y = myobj2->y;
        } else {
            special_effects[special_effect].x = x; /* appear right at the target instead */
            special_effects[special_effect].y = y;
        }
        special_effects[special_effect].x2 = x;
        special_effects[special_effect].y2 = y;
        special_effects[special_effect].more = 0xFFFF;
        special_effects[special_effect].wait = 1;
        return SPELL_SUCCESS;
    }
    return SPELL_INVALID;
}

U6O_SPELL_FUNCTION(light) {
    return 0;
}

/* Second Circle */
U6O_SPELL_FUNCTION(infravision) {
    return 0;
}

U6O_SPELL_FUNCTION(magic_arrow) {
    return 0;
}

U6O_SPELL_FUNCTION(poison) {
    return 0;
}

U6O_SPELL_FUNCTION(reappear) {
    return 0;
}

U6O_SPELL_FUNCTION(sleep) {
    return 0;
}

U6O_SPELL_FUNCTION(telekinesis) {
    object *myobj2;
    int x3;
    int x4;

    if (stormcloakcheck2(x, y, myplr)) {
        return SPELL_INVALID;
    }
    myobj2 = OBJfindlast(x, y);
    if (myobj2) {
        switch (myobj2->Type()) {
            case 268: //lever
            case 174: //switch
            case 288: //crank
                x3 = (int) rnd * (8 + 5 * 2);
                x4 = (int) rnd * (caster->intelligence + 1);
                if (x4 >= x3) {
                    use_toggle(NULL, myobj2);
                    return SPELL_SUCCESS;
                }
                return SPELL_FAILURE;
                break; //lever, switch or crank

            default: //other objects are not yet implemented. refund.
                return SPELL_INVALID;
        }
    }//myobj2
    return SPELL_INVALID; //should be targeting an object.
}

U6O_SPELL_FUNCTION(trap) {
    return 0;
}

U6O_SPELL_FUNCTION(unlock_magic) {
    return 0;
}

U6O_SPELL_FUNCTION(untrap) {
    return 0;
}

U6O_SPELL_FUNCTION(vanish) {
    return 0;
}

/* Third Circle */
/* Fourth Circle */
/* Fifth Circle */
/* Sixth Circle */
/* Seventh Circle */
/* Eight Circle */
U6O_SPELL_FUNCTION(armageddon) {
    /* placeholder */
    return 0;
}

int fieldSpell(player* myplr , npc * caster, object* myobj, unsigned short x, unsigned short y, int spell_code, int dc) {
    if (stormcloakcheck2(x, y, myplr)) {
        return SPELL_INVALID;
    }

    object *spellTarget;
    int targetType = 0;
    int difficultyCheck = 0;
    int playerInt = 0;

    spellTarget = OBJfindlast(tpx + x, tpy + y);

    if (spellTarget) {
        targetType = spellTarget->type & OBJ_TYPE_BITMASK;
    }
    if ((targetType == OBJ_FIRE_FIELD) ||
        (targetType == OBJ_POISON_FIELD) ||
        (targetType == OBJ_SLEEP_FIELD) ||
        (targetType == OBJ_TRAP) ||
        (targetType == OBJ_WEB)) {
        return SPELL_INVALID;
    }

    // check if location is walkable(1024)
    int walkable = bt[tpy + y][tpx + x] & 1024;

    if (walkable) {
        difficultyCheck = rnd * dc; // roll rand difficulty check
        playerInt = rnd * (tnpc->intelligence + 1); // get the caster's intelligence

        if (playerInt >= difficultyCheck) { // success
            object *fieldSpell = OBJnew();
            fieldSpell->type = spell_code;
            fieldSpell->more2 = tnpc->player->id;
            OBJadd(tpx + x, tpy + y, fieldSpell);
            fieldSpell->info += (2 << 4);
            OBJcheckflags(fieldSpell->x, fieldSpell->y);
            return SPELL_SUCCESS;
        } else {
            return SPELL_INVALID;
        }
    }
    return SPELL_INVALID;
}

/* Original spells and reagents (using originals reagent bit values) 
# First Circle
# Create Food  IMY  0x61 # Harm         AM   0x12
# Detect Magic WO   0x82 # Heal         IM   0x50
# Detect Trap  WJ   0x82 # Help         KL   0x00
# Dispel Magic AJO  0x60 # Ignite       IF   0x84
# Douse        AF   0x24 # Light        IL   0x80
#
# Second Circle
# Infravision  QL   0x82 # Telekinesis  OPY  0x0d
# Magic Arrow  OJ   0x84 # Trap         IJ   0x12
# Poison       INP  0x0e # Unlock Magic EP   0x88
# Reappear     IY   0x1c # Untrap       AJ   0x88
# Sleep        IZ   0x16 # Vanish       AY   0x2c
#
# Third Circle
# Curse        AS   0xa2 # Mass Awaken  AVZ  0x60
# Dispel Field AG   0x84 # Mass Sleep   VZ   0x52
# Fireball     PF   0x84 # Peer         VWY  0x03
# Great Light  VL   0x81 # Protection   IS   0xe0
# Lock         AP   0xa8 # Repel Undead AXC  0xa0
#
# Fourth Circle
# Animate      OY   0x89 # Locate       IW   0x02
# Conjure      KX   0x11 # Mass Dispel  VAJO 0x60
# Disable      AVM  0x13 # Poison Field ING  0x56
# Fire Field   IFG  0x94 # Sleep Field  IZG  0x54
# Great Heal   VM   0x51 # Wind Change  RH   0x88
#
# Fifth Circle
# Energy Field ISG  0x15 # Paralyze     AXP  0x96
# Explosion    VPF  0x8d # Pickpocket   PY   0x1a
# Insect Swarm KBX  0x98 # Reveal       ASL  0x13
# Invisibility SL   0x0a # Seance       KMC  0x9b
# Lightning    OG   0x85 # X-ray        WY   0x81
#
# Sixth Circle
# Charm        AXE  0x16 # Mass Protect VIS  0xe8
# Clone        IQX  0xdb # Negate Magic AO   0xa1
# Confuse      VQ   0x03 # Poison Wind  NH   0x8a
# Flame Wind   FH   0x89 # Replicate    IQY  0xda
# Hail Storm   KDY  0x0d # Web          IDP  0x10
#
# Seventh Circle
# Chain Bolt   VOG  0x8d # Kill         IC   0x86
# Enchant      IOY  0x91 # Mass Curse   VAS  0xa3
# Energy Wind  GH   0x8b # Mass Invis   VSL  0x0f
# Fear         QC   0x23 # Wing Strike  KOX  0x99
# Gate Travel  VRP  0x85 # Wizard Eye   POW  0x9f
#
# Eighth Circle
# Armageddon   VCBM 0xff # Resurrect    IMC  0xf9
# Death Wind   CH   0x8b # Slime        VRX  0x0b
# Eclipse      VAL  0xab # Summon       KXC  0x39
# Mass Charm   VAXE 0x17 # Time Stop    AT   0x29
# Mass Kill    VC   0x87 # Tremor       VPY  0x89
*/


