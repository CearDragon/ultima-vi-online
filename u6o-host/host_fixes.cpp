//
// Created by Cocoa-AP on 1/26/2025.
//

#include "host_fixes.h"

void RunFixes() {
    houseFixes();
    autoSetupAndMapFixes();
    lordBritishCastleDrawbridge();
    minocInnDoorsFix();
    questItemFix();
    treasureRoomFix();
    sutexFix();
    sutexDrawbridgeFix();
    codexFix();
    viewRedirectorFixes();
    nuggetsFix();
    smithFix();
    pushMePullYouFix();
    potionFix();
    doorsAndSecretLevers();
    fixedObjectSystem();
    tobjfixedSystem();
    visibilityFix();
    generateClientBtFile();
    treasureChestFix();
}

void addNewObj(unsigned long x, unsigned long y, unsigned short type, NewObjectOptions *options) {
    object *newObj = OBJnew();
    newObj->type = type;

    if (options != nullptr) {
        if (options->info != nullptr) {
            newObj->info |= *options->info;
        }

        if (options->more2 != nullptr) {
            newObj->more2 = *options->more2;
        }
    }

    OBJadd(x, y, newObj);
}

NewObjectOptions configureNewObjectOptions(int *info, int *more2) {
    NewObjectOptions options = {};

    if (info != nullptr) {
        options.info = info;
    }

    if (more2 != nullptr) {
        options.more2 = more2;
    }

    return options;
}

void houseFixes() {
    OBJremove(od[346][400]);
    OBJremove(od[347][400]);
}

void autoSetupAndMapFixes() {
    //2005: autosetup & map fixes
    use_basic_toggle(nullptr, od[383][303]); //lever in LB's castle!!!
    use_basic_toggle(nullptr, od[393][327]); //mint!!!
    use_basic_toggle(nullptr, od[752][399]); //trisnic
}

void lordBritishCastleDrawbridge() {
    object *myobj = od[384][303]; //crank
    object *myobj2 = od[385][303];
    myobj->more = myobj2;
    myobj = od[384][311]; //crank
    myobj->more = myobj2;
    //use crank to remove the "drawbridge up" items before linking the other parts.
    use_basic_toggle(nullptr, myobj);

    for (myobj2 = myobj; myobj2; myobj2 = (object *) myobj2->more) {
        myobj = myobj2;
    }
    for (int x = 304; x <= 310; x++) {
        for (int y = 386; y <= 390; y++) {
            myobj2 = od[y][x];
            myobj->more = myobj2;
            myobj = myobj2;
        }
    }
}

void minocInnDoorsFix() {
    //minoc inn doors locked -> npc's stuck fix (unlock the doors)
    object *innDoorObj = OBJfindlastall(561, 84);
    innDoorObj->type = OBJ_OAKEN_DOOR + 4 * 1024;
    innDoorObj = od[84][562];
    innDoorObj->type = OBJ_OAKEN_DOOR + 1024 + 4 * 1024;
    innDoorObj = OBJfindlastall(566, 84);
    innDoorObj->type = OBJ_OAKEN_DOOR + 4 * 1024;
    innDoorObj = od[84][567];
    innDoorObj->type = OBJ_OAKEN_DOOR + 1024 + 4 * 1024;
}

void questItemFix() {
    unsigned long questItemX[] = {1971, 2040, 1259, 1499, 81};
    unsigned long questItemY[] = {221, 40, 53, 147, 493};
    bool removeOld[] = {true, false, true, true, true};
    unsigned short questItemType[] = {OBJ_BALLOON_PLANS,
                                      OBJ_PART_OF_A_MAP_0,
                                      OBJ_PART_OF_A_MAP_3,
                                      OBJ_RUNE_OF_HONESTY,
                                      OBJ_RUNE_OF_SPIRITUALITY};
    object *lastQuestObj;
    object *questObjFix;

    for (int i = 0; i < 5; i++) {
        if (removeOld[i]) {
            lastQuestObj = OBJfindlast(questItemX[i], questItemY[i]);
            OBJremove(lastQuestObj);
        }
        questObjFix = OBJnew();
        questObjFix->type = questItemType[i];
        questObjFix->info |= OBJ_QUEST_ITEM;
        OBJadd(questItemX[i], questItemY[i], questObjFix);

        if (removeOld[i]) {
            OBJadd(questItemX[i], questItemY[i], lastQuestObj);
        }
    }
}

void treasureRoomFix() {
    object *treasureRoomLock = OBJfindlastall(1947, 229);
    treasureRoomLock->more2 = 0; //release treasure room's lock
}

void sutexFix() {
    //Sutek FIX
    object *lastObj = OBJfindlastall(787, 964);
    lastObj->more2 = 0; //release Sutek's lock
    // Cocoa: I believe these are drawbridge parts
    addNewObj(786, 960, 269 + 3 * 1024, nullptr);
    addNewObj(786, 961, 269 + 3 * 1024, nullptr);
    addNewObj(787, 960, 269 + 4 * 1024, nullptr);
    addNewObj(787, 961, 269 + 4 * 1024, nullptr);
    addNewObj(788, 960, 269 + 5 * 1024, nullptr);
    addNewObj(788, 961, 269 + 5 * 1024, nullptr);
    addNewObj(786, 962, 269 + 0 * 1024, nullptr);
    addNewObj(787, 962, 269 + 1 * 1024, nullptr);
    addNewObj(788, 962, 269 + 2 * 1024, nullptr);
}

void sutexDrawbridgeFix() {
    object *crankObj = od[959][785]; //crank
    object *drawBridgePart;
    crankObj->more = od[960][786];
    crankObj = od[959][789]; //crank
    for (int x = 786; x <= 788; x++) {
        for (int y = 960; y <= 962; y++) {
            drawBridgePart = od[y][x];
            crankObj->more = drawBridgePart;
            crankObj = drawBridgePart;
        }
    }

    crankObj = od[959][789]; //crank
    //use crank to remove the "drawbridge down" items before linking the other parts.
    use_basic_toggle(nullptr, crankObj);
    crankObj = drawBridgePart;

    for (int x = 785; x <= 788; x++) {
        drawBridgePart = OBJfindlastall(x, 959);
        crankObj->more = drawBridgePart;
        crankObj = drawBridgePart;
    }
}

void codexFix() {
    int questItem = OBJ_QUEST_ITEM;
    NewObjectOptions codexOptions = configureNewObjectOptions(&questItem, nullptr);

    addNewObj(921, 850, 394, &codexOptions);
    addNewObj(920, 850, 396, &codexOptions);
    addNewObj(925, 850, 62, &codexOptions);
}

void viewRedirectorFixes() {
    addNewObj(921, 850, OBJ_NOTHING, nullptr);
    addNewObj(920, 850, OBJ_NOTHING, nullptr);
    addNewObj(925, 850, OBJ_NOTHING, nullptr);
}

void nuggetsFix() {
    int more2 = 20;
    NewObjectOptions nuggetsOptions = {};
    nuggetsOptions.more2 = &more2;

    addNewObj(1642, 65, OBJ_GOLD_NUGGET, &nuggetsOptions);
}

void smithFix() {
    OBJremove(od[232][200]);
}

void pushMePullYouFix() {
    OBJremove(od[243][1475]);
}

void potionFix() {
    unsigned long potionX[] = {1109, 1105, 1099, 1638, 1661, 1640, 1690, 1612, 402};
    unsigned long potionY[] = {109, 105, 114, 82, 74, 66, 155, 126, 406};

    for (int i = 0; i < 9; i++) {
        object *potion = OBJnew();
        potion->type = 275 + 2 * 1024;
        potion->info |= OBJ_QUEST_ITEM;
        potion->more2 = 1 << i;
        OBJadd(potionX[i], potionY[i], potion);
    }

    //Avatar's room in LB's castle fix
    object *potion = od[367][297];
    potion->more = nullptr;
    potion->info -= 128;
    potion = od[367][298];
    potion->more = nullptr;
    potion->info -= 128;
    potion = (object *) od[365][296]->more;
    potion->info -= 128;
    potion = (object *) potion->next;
    potion->info -= 128;
}

void doorsAndSecretLevers() {
    // Loop over the grid
    for (int y = 0; y <= 1023; y++) {
        for (int x = 0; x <= 2047; x++) {

            // Quick skip if no objects
            object *myobj = od[y][x];
            if (!myobj) {
                continue;
            }

            // Enumerate all objects in this cell
            while (myobj) {
                // Precompute base type so we don't keep masking
                int baseType = myobj->Type();
                bool hasDoorBit = (myobj->type & 1024) != 0;  // The "walkable/main part of door" flag

                switch (baseType) {
                    // -----------------------------
                    // Check if it's a door
                    // (OBJ_X_DOOR range)
                    // -----------------------------
                    case OBJ_OAKEN_DOOR:
                    case OBJ_WINDOWED_DOOR:
                    case OBJ_CEDAR_DOOR:
                    case OBJ_STEEL_DOOR:
                        if (hasDoorBit) {
                            // Door main part => fill door close arrays
                            doorclose_last++;
                            doorclose_wait[doorclose_last] = 0;
                            doorclose_obj[doorclose_last] = myobj;
                            doorclose_oldtype[doorclose_last] = myobj->type;
                            auto *linkedObj = (object *) myobj->more;
                            doorclose_obj2[doorclose_last] = linkedObj;
                            doorclose_oldtype2[doorclose_last] = (linkedObj) ? linkedObj->type : 0;
                            myobj = (object *) myobj->next;
                            continue;
                        }
                        break; // If hasDoorBit not set, do nothing special

                        // -----------------------------
                        // Secret door
                        // -----------------------------
                    case OBJ_SECRET_DOOR:
                        doorclose_last++;
                        doorclose_wait[doorclose_last] = 0;
                        doorclose_obj[doorclose_last] = myobj;
                        doorclose_oldtype[doorclose_last] = myobj->type;
                        doorclose_obj2[doorclose_last] = nullptr;
                        myobj = (object *) myobj->next;
                        continue;

                        // -----------------------------
                        // Lever
                        // -----------------------------
                    case OBJ_LEVER:
                        leverchange_last++;
                        leverchange_wait[leverchange_last] = 0;
                        leverchange_obj[leverchange_last] = myobj;
                        leverchange_oldtype[leverchange_last] = myobj->type;
                        myobj = (object *) myobj->next;
                        continue;

                        // -----------------------------
                        // Otherwise, fall through
                        // (e.g. if you want to handle cranks or others)
                        // -----------------------------
                    default:
                        // TODO: handle cranks?
                        break;
                }

                // Move on if it didn't match anything that used "continue"
                myobj = (object *) myobj->next;
            }
        }
    }
}

static inline void storeFixedObject(int y, int x, unsigned short indexSlot, unsigned short objType)
{
    objfixed_index[y][x] = indexSlot;          // Mark which slot is used for (y,x)
    objfixed_type[indexSlot]++;                // Bump count of objects stored at this slot
    objfixed_type[indexSlot + objfixed_type[indexSlot]] = objType;
}

static unsigned short resolveBuildClass(unsigned short x5)
{
    // The original code repeatedly does: while(tclass_build[x5] & (1+2+4)) x5 += 1024;
    // We can turn that into a simple while loop:
    while (tclass_build[x5] & 0x07) {  // (1 + 2 + 4 = 0x07)
        x5 += 1024;
        // Potentially check bounds here if needed
    }
    return x5;
}

void fixedObjectSystem() {
    // Clear out the arrays
    // (Adjust sizes if your arrays differ from these byte sizes.)
    memset(objfixed_type,  0, 65536 * 2);
    memset(objfixed_index, 0, 2048  * 1024 * 2);

    objfixed_next = 1;

    // Loop over map rows and columns
    for (int y = 0; y <= 1023; y++) {
        for (int x = 0; x <= 2047; x++) {

            // Each cell starts off using the "next" slot in objfixed_type
            unsigned short fixedSlot = objfixed_next;
            object* obj = od[y][x];

            // If there's an object in this cell...
            if (obj) {
                // If it's a "view redirector" (type == 416) and has a valid .more2 location:
                if (obj->type == 416 && obj->more2 != 0) {
                    object* redirected = od[obj->more2 >> 16][obj->more2 & 0xFFFF];
                    // If the redirect leads to NULL, skip all further processing for this cell
                    if (!redirected) {
                        // If something was already stored at fixedSlot, we still finalize it
                        if (objfixed_type[fixedSlot]) {
                            objfixed_next += (objfixed_type[fixedSlot] + 1);
                        }
                        continue; // move to next (x)
                    }
                    // Otherwise, update obj to the redirected one
                    obj = redirected;
                }

                // Now iterate any linked objects
                for (object* current = obj; current; current = (object*)current->next) {

                    // 1) If type is in tclass_fixed, record it
                    if (tclass_fixed[current->type]) {
                        storeFixedObject(y, x, fixedSlot, current->type);
                    }

                    // 2) If there's no 'more' pointer and it's not a raft/skiff (414,415):
                    if (!current->more) {
                        unsigned short baseType = (current->Type());
                        if (baseType != 414 && baseType != 415) {
                            // The old "unknownclass" logic: keep adding 1024 while tclass_build[x5]&7 is true
                            unsigned short x5 = current->type;
                            x5 = resolveBuildClass(x5);

                            // If it's in tclass_build[x5] & 8 or tclass_mover[x5], store it
                            if ((tclass_build[x5] & 8) || (tclass_mover[x5] != 0)) {
                                storeFixedObject(y, x, fixedSlot, current->type);
                            }
                        }
                    }
                } // end for each linked object
            } // end if (obj)

            // If we stored anything at fixedSlot, bump objfixed_next so the next cell has a new slot
            if (objfixed_type[fixedSlot]) {
                objfixed_next += (objfixed_type[fixedSlot] + 1);
            }
        } // end for x
    } // end for y

    file *tfh = open2(".\\dr\\objfixed.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    put(tfh, &objfixed_index, 2048 * 1024 * 2);
    put(tfh, &objfixed_type, 65536 * 2);
    close(tfh);
}

void tobjfixedSystem() {
    // Clear memory (adjust sizes if your arrays are different)
    memset(tobjfixed_type,  0, 65536 * 2);
    memset(tobjfixed_index, 0, 2048  * 1024 * 2);

    tobjfixed_next = 1;

    // Loop over the map (y rows, x columns)
    for (int y = 0; y <= 1023; y++) {
        for (int x = 0; x <= 2047; x++) {

            unsigned short x3 = tobjfixed_next;
            object* myobj = od[y][x];

            if (myobj) {
                // Check if it's a "view redirector" (type == 416)
                if (myobj->type == 416 && myobj->more2 != 0) {
                    // Move the pointer to the redirected location
                    object* redirected = od[myobj->more2 >> 16][myobj->more2 & 0xFFFF];
                    if (!redirected) {
                        // If the redirect leads to a NULL object, skip further processing
                        // but still account for any partial usage of x3
                        if (tobjfixed_type[x3]) {
                            tobjfixed_next += (tobjfixed_type[x3] + 1);
                        }
                        // Move on to the next x
                        continue;
                    }
                    // Update myobj to the redirected object
                    myobj = redirected;
                }

                // Iterate all linked objects in this cell
                for (object* curr = myobj; curr != nullptr; curr = (object*)curr->next) {
                    // If this object's type is in tclass_object
                    if (tclass_object[curr->type]) {
                        // And if it's not considered "floating" by checking objfloatflags
                        int floatIndex = (curr->type >> 10) + sprlnk[curr->type & 0x3FF];
                        if (objfloatflags[floatIndex] == 0) {
                            // Record in our fixed arrays
                            tobjfixed_index[y][x] = x3;
                            tobjfixed_type[x3]++;
                            // The newly added object is stored at index x3 + count
                            tobjfixed_type[x3 + tobjfixed_type[x3]] = curr->type;
                        }
                    }
                }
            } // end if (myobj)

            // If we stored anything in this slot, move tobjfixed_next
            if (tobjfixed_type[x3]) {
                tobjfixed_next += (tobjfixed_type[x3] + 1);
            }
        } // end for x
    } // end for y

    // Open the file to store the fixed object data
    file *tfh = open2(".\\dr\\tobjfix.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    // Write the index and type arrays to the file
    put(tfh, &tobjfixed_index, 2048 * 1024 * 2);
    put(tfh, &tobjfixed_type, 65536 * 2);
    // Close the file
    close(tfh);
}

void visibilityFix() {
    // Open the visibility file for read/write, share compatibility, and create if it doesn't exist
    file *tfh = open2(".\\dr\\vis.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);

    // Iterate over the grid columns
    for (int x = 0; x <= 2047; x++) {
        // Iterate over the grid rows
        for (int y = 0; y <= 1023; y++) {
            // Get the object at the current grid position
            object *obj = od[y][x];

            // Check if the object exists and is a view redirector (type 416)
            if (obj && obj->type == 416) {
                bool needsVisibility = false;

                // Check the surrounding cells to determine if visibility is needed
                for (int x2 = -1; x2 <= 1 && !needsVisibility; x2++) {
                    for (int y2 = -1; y2 <= 1 && !needsVisibility; y2++) {
                        object *obj2 = od[y + y2][x + x2];
                        if (!obj2 || obj2->type != 416) {
                            needsVisibility = true;
                        }
                    }
                }

                // If visibility is needed, write the coordinates to the file
                if (needsVisibility) {
                    put(tfh, &x, 2);
                    put(tfh, &y, 2);
                }
            }
        }
    }

    // Close the visibility file
    close(tfh);
}

void generateClientBtFile() {
    // Create bt.bin for client to reference
    file *tfh = open2("bt.bin", OF_READWRITE | OF_SHARE_COMPAT | OF_CREATE);
    put(tfh, &bt, 1024 * 2048 * 2);
    close(tfh);
    // Later we will compress and encrypt this
}

void treasureChestFix() {
    if (NEThost) { // Can't add objects as a client
        // Random treasure chest
        unsigned long chestX = randomchestlocation(false);
        unsigned long chestY = chestX >> 10;
        chestX = chestX & 1023;

        // Objects
        int randObjectType = 2 + rnd * 2;
        object *item000001 = OBJaddnew(chestX, chestY,
                                       98 + 1024 * randObjectType,
                                       16384, 0); // Chest

        for (int x = 0; x < 16; x++) {
            int x2 = 1;
            int x3 = 0;

            object *item000002 = OBJaddnew(chestX, chestY, x2 + 1024 * 0, 16384, x3);
            OBJremove(item000002); // Remove from the map (so it can go in a container)
            OBJaddtocontainer(item000001, item000002); // Put this item inside its parent
        }
    } // NEThost
}

