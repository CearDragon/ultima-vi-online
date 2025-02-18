//
// Created by Cocoa-AP on 1/26/2025.
//

#include "host_setup.h"

void host_setup::runHostSetup() {
    initHostData();
    loadBanList();
}

void host_setup::initHostData() {
    save_dump = 0;
    save_version = 0;
    login_dead_callback = 0;
    sfbuffersend = 0;
    sfbufferwait = 0.0f;
    cast_spell = 0;
    staff_cast_spell = 0;
    motd = NULL;
    inbritannia = NULL;
    inbritannia_totalplayers = 0;
    U6ONEWID = 1; //never 0
    //wind direction (global)
    windx = 0;
    windy = 0;
    spellattcrt = FALSE;
    autosetup = 0;
    autosetup_next = FALSE;
    autosetup_counter = 0;
    autospell = FALSE;
    format_usernames = 0;
    objr_x = 0;
    objr_y = 0;
    prev_special_effect = -1; //last special_effects
    oln = 0;
    objb_last = -1; //last objb
    objb_free_last = -1; //last free objb index
    doorclose_last = -1;
    leverchange_last = -1;
    showmoongates = 0;
    moonphase = 255;
    object *nuggetsfix = NULL;
    vlnkb_last = -1; //last
    vlnkb_free_last = -1; //last free vlnkb index
    nresu = -1;
    wizardeyesi2 = 0;
    wizardeyesadded = 0;
    wizardeyesobj = NULL;
    respawn_last = -1;
    playerlist_last = -1;
    ouln = -1;
    f_ouln = -1;
    tname = NULL;
    tusername = NULL;
    tuserpassword = NULL;
    tnewuserpassword = NULL;
    MOVERNEW_GLOBALFLAGS = 0;

#ifndef CLIENT
    //STUBS TO SUPPORT EXTERNAL LINKING (DATA NOT ACTUALLY USED!)
    u6omidisetup = NULL;
    hWnd = NULL;
    hWnd2 = NULL;
    hWnd3 = NULL;
    hWnd4 = NULL;
    smallwindow = NULL;
    u6ovolume = NULL;
#endif

    revive_infiniteloopexit_i = 0;
    revive_infiniteloopexit_i2 = 0xFFFFFFFF;
    revive_infiniteloopexit_i3 = 0;
    mycount = 0;
    newschedule2 = -1;
    newll = NULL;
    ol_tag = 0;
    ol_tag_prev = 0;

}

void host_setup::loadBanList() {
    file *tfh = open2("ban.txt", OF_READWRITE | OF_SHARE_COMPAT);
    if (tfh->h == HFILE_ERROR) {
        tfh = open2("ban.txt", OF_READWRITE | OF_CREATE);
    }
    close(tfh);
}

void host_setup::loadOrbOfTheMoonDestinations() {
    // Set default orb of the moons destinations
    for (int x = 0; x <= 4; ++x) {
        for (int y = 0; y <= 4; ++y) {
            orbx[x][y] = 368;
            orby[x][y] = 386;
        }
    }

    // Outer circle destinations
    int outer_coords[13][2] = {
            {899, 499},
            {935, 262},
            {435, 395},
            {503, 358},
            {147, 883},
            {159, 942},
            {227, 131},
            {295, 38},
            {667, 67},
            {831, 166},
            {387, 787},
            {327, 822},
            {75,  507}
    };

    for (int i = 0; i < 13; ++i) {
        orbx[i / 5][i % 5] = outer_coords[i][0];
        orby[i / 5][i % 5] = outer_coords[i][1];
    }

    // Inner circle destinations
    int inner_coords[6][2] = {
            {1092, 301},
            {307,  352},
            {1212, 301},
            {923,  876},
            {1132, 477},
            {1152, 342}
    };
    for (int i = 0; i < 6; ++i) {
        orbx[(i + 1) / 2][(i + 1) % 2 + 1] = inner_coords[i][0];
        orby[(i + 1) / 2][(i + 1) % 2 + 1] = inner_coords[i][1];
    }
}

void host_setup::loadCreatureEnums() {
    file *tfh = open(".\\host\\crtenum.bin");
    get(tfh, &crtenum, 1073 * 2);
    close(tfh);

    /// Cocoa - I believe this handles where creatures revive at
    ///         when they die, so I will leave it here.
    u6orevive_fh = open(".\\host\\revive.bin");
}

void loadMessageOfTheDay() {
    motd = txtnew();
    file *tfh = open("motd.txt");
    txtfilein(motd, tfh);
    close(tfh);
}

void loadStormCloakData() {
    file *tfh = open(".\\host\\sc888.raw");
    get(tfh, &stormcloak_mask, 64);
    close(tfh);
}

void loadBaseTileFlags() {
    file *tfh = open(".\\host\\u6bt.flg");
    get(tfh, &btflags, fileLen(tfh));
    close(tfh);
}

void loadObjectPassFlags() {
    FillMemory(&objpassflags, sizeof(objpassflags), 1);

    loadFromFile(".\\host\\objpass.flg", &objpassflags, MAXLONG);

    //update flags for world map (based on base tiles)
    for (int y = 0; y < 1024; y++) {
        for (int x = 0; x < 2048; x++) {
            OBJcheckflags(x, y);
        }
    }
}

void loadSchedules() {
    file *tfh = loadFromFileAndReturn(".\\host\\schedule.bin", &schedule, MAXLONG);

    long fLen = fileLen(tfh) / 256 - 1;

    close(tfh);

    for (int i = 0; i <= fLen; i++) {
        for (int j = 0; j <= 31; j++) {
            if (schedule[i][j].hour == 255) continue;

            switch (schedule[i][j].z) {
                case 1:
                    schedule[i][j].x += 1024;
                    break;
                case 2:
                    schedule[i][j].x += 1280;
                    break;
                case 3:
                    schedule[i][j].x += 1536;
                    break;
                case 4:
                    schedule[i][j].x += 1792;
                    break;
                case 5:
                    schedule[i][j].x += 1024;
                    schedule[i][j].y += 256;
                    break;
            }
        }
    }
}

// Cocoa - this is the old way, I made a new loadFromFile below in load_npc_objects()
//         to more cleanly handle leading data from files. It needs to be tested.
void load_npc_objects_old() {
    // load npc objects
    file *tfh = open(".\\host\\npcobj.bin");
    get(tfh, &npcobj, fileLen(tfh));
    close(tfh);

    // load object names
    tfh = open(".\\host\\objname.bin");
    objname = (unsigned long *) malloc(fileLen(tfh));
    objname2 = (unsigned char *) objname;
    get(tfh, objname, fileLen(tfh));
    close(tfh);


    // load npc binaries
    tfh = open(".\\host\\npc.bin");
    npcbin = (npcbin_i *) malloc(fileLen(tfh));
    get(tfh, npcbin, fileLen(tfh));
    close(tfh);

    // load npc info
    tfh = open(".\\host\\npc.inf");
    npcinf = (unsigned char *) malloc(fileLen(tfh));
    get(tfh, npcinf, fileLen(tfh));
    close(tfh);

    // load npc indices
    tfh = open(".\\host\\npci.bin");
    get(tfh, &npci, fileLen(tfh));
    close(tfh);
}

void loadNpcObjects() {
    loadFromFile(".\\host\\npcobj.bin", &npcobj, sizeof(npcobj));

    loadFromFileMalloc(".\\host\\objname.bin", &objname, sizeof(objname));
    objname2 = (unsigned char *) objname;

    loadFromFileMalloc(".\\host\\npc.bin", &npcbin, sizeof(npcbin));

    loadFromFileMalloc(".\\host\\npc.inf", &npcinf, sizeof(npcinf));

    loadFromFile(".\\host\\npci.bin", &npci, sizeof(npci));
}

void loadSignData() {
    loadFromFileMalloc(".\\host\\sign.bin", &tsign, MAXLONG);
}

void loadWorldOverlayData() {
    txt *t = txtNew("  ");
    txt *t2 = txtNew(".\\ultima6\\savegame\\objblk");
    txt *t3 = txtnew();
    file *tfh;


    for (int x = 0; x <= 7; x++) {
        for (int y = 0; y <= 7; y++) {
            t->d[0] = x + 65;
            t->d[1] = y + 65;
            txtset(t3, t2);
            txtadd(t3, t);
            tfh = open(t3);

            int i = 0;
            get(tfh, &i, 2);
            if (i > 0) {
                get(tfh, &objectList[oln], fileLen(tfh) - 2);
                oln += i;
            }
            close(tfh);

            if (y == 7 && x <= 4) {
                y++;
            }
        }
    }
}

struct u6oPos {
    unsigned long x;
    unsigned long y;
    unsigned long z;
};

u6oPos getU6OPos(objentry oe) {
    u6oPos pos;

    // get coordinates x,y
    pos.x = oe.unk_h;
    pos.y = oe.unk_d1;
    pos.z = oe.unk_d2 >> 4;

    // fix coordinates
    pos.x = pos.x + (pos.y & 3) * 256;
    pos.y = pos.y / 4;
    pos.y = pos.y + (oe.unk_d2 & 15) * 64;

    if (pos.x < 1024 && pos.y < 1024) { //avoid non-coordinate based objects
        switch (pos.z) {
            case 1:
            case 2:
            case 3:
            case 4:
                pos.x = pos.x + 1024 + ((pos.z - 1) * 256);
                break;
            case 5:
                pos.x = pos.x + 1024;
                pos.y = pos.y + 256;
                break;
        }
    }

    return pos;
}

bool isOverlayObject(unsigned short type, unsigned long x, unsigned long y) {
    switch (type) {
        case OBJ_EARTH:
        case OBJ_FLOOR_WOOD:
        case OBJ_FLOOR_TILE:
        case OBJ_FLOOR_BLUE:
        case OBJ_SHADOW_TABLE_SQUARE_TOP:
        case OBJ_SHADOW_TABLE_CIRCLE_TOP:
        case OBJ_SHADOW_TABLE_CENTRE:
        case OBJ_SHADOW_TABLE_ROUND:
        case OBJ_SHADOW_TABLE_SQUARE_BOTTOM:
        case OBJ_SHADOW_TABLE_BOTTOM:
        case OBJ_SHADOW_TABLE_LEG_RIGHT:
            return true;
        case OBJ_CARPET:
            if (x == 122 && y == 196) {
                return true;
            }
            break;
        default:
            return false;
    }

    return false;
}

void createWorldOverlayObjects() {
    unsigned long x;
    unsigned long y;
    unsigned long z;

    for (int i = oln - 1; i >= 0; i--) {
        // get coordinates x,y
        u6oPos pos = getU6OPos(objectList[i]);

        x = pos.x;
        y = pos.y;
        z = pos.z;

        if (x < 1024 && y < 1024) { //avoid non-coordinate based objects
            unsigned short type = objectList[i].type & 1023; //get object identity

            bool isOverlay = isOverlayObject(type, x, y);

            //*the healers bed has a table object on top!
            if (isOverlay) {
                object *myobj = OBJnew();
                myobj->type = objectList[i].type;
                OBJadd(x, y, myobj);
                unsigned short overlayType = objectList[i].type >> 10;
                unsigned long lnk = sprlnk[type];
                lnk += overlayType;

                /// Cocoa - I have no idea what either of these are for,
                ///         the next reference to them just overwrites them.
                unsigned long i4 = lnk;
                object *myobj2 = myobj;
            }
        }
    }
}


void setQuestItem(object *questObj) {
    questObj->info |= OBJ_QUEST_ITEM;
}

void loadObjectsToMap() {
    for (long i = oln - 1; i >= 0; i--) {
        objentry currObj = objectList[i];
        object *newObj = OBJnew();

        newObj->type = currObj.type;
        unsigned short objType = currObj.Type(); //get object identity

        if (objType > OBJ_LOAD_MAX)
            exit(901); //invalid range

        unsigned long x;
        unsigned long y;
        unsigned long z;

        u6oPos pos = getU6OPos(currObj);

        x = pos.x;
        y = pos.y;
        z = pos.z;

        bool skipObject = false;
        bool isOverlay = isOverlayObject(objType, x, y);

        isOverlay = isOverlay || (((objType & 1023) == 0) && (x == 1151) && (y == 309));

        /// Cocoa - we skip these for some reason.  I believe this check is done quite a bit.
        ///         Once I figure out if they're handled in some other way, I'll likely make
        ///         a func for this.
        if (objType == OBJ_STATUE_OF_MONDAIN || objType == OBJ_STATUE_OF_MINAX ||
            objType == OBJ_STATUE_OF_EXODUS || objType == OBJ_EFFECT ||
            objType == OBJ_RUNE_OF_SPIRITUALITY || objType == OBJ_RUNE_OF_HONESTY ||
            objType == OBJ_PART_OF_A_MAP_0 || objType == OBJ_PART_OF_A_MAP_3 ||
            objType == OBJ_PART_OF_A_MAP_6 || objType == OBJ_BALLOON_PLANS)
            skipObject = 1;

        if ((objType == OBJ_SHIP) && ((objectList[i].type >> 13) != 1))
            skipObject = 1; //non-core parts of boat


        if (objType == OBJ_TRAP) {
            currObj.status = 0; //trap
        }

        if (!skipObject) {
            //16=object in party inventory *skip
            if (!objectList[i].inPartyInventory()) {
                //8=container
                if (objectList[i].isContainer()) {
                    //object in a container
                    //2003: note: first pass objects won't be inside containers! check unnecessary

                    if ((objectList[i].status & 1) == 0)
                        newObj->info |= 128; //128 1=stealing, 0=ok to take

                    if (objectInfo[sprlnk[objType]].v4)
                        newObj->more2 = i; //*temp reference value

                    if (objectInfo[sprlnk[objType] + (newObj->type >> 10)].flags & 4096) {
                        newObj->more2 = objectList[i].qty; //set quantity in more2
                        if (newObj->more2 == 0) newObj->more2 = 1;
                    }

                    switch (objType) {
                        case OBJ_SPELL:
                            newObj->more2 = objectList[i].tag; //more2<-
                            if (spellname[newObj->more2]->l == 0)
                                newObj->more2 = (0 << 4) + 7;
                            break;
                        case OBJ_CODEX:
                            newObj->more2 = 128;
                            break;
                        case OBJ_KEY:
                        case OBJ_BOOK_OF_CIRCLES:
                        case OBJ_PICTURE:
                        case OBJ_BOOK:
                        case OBJ_SCROLL:
                        case OBJ_CROSS:
                        case OBJ_TOMBSTONE:
                        case OBJ_SIGN:
                        case OBJ_GARGOYLE_SIGN:
                            newObj->more2 = objectList[i].tag;
                            break;
                    }

                    if (newll == NULL) { //newll is a linked list, object is added to the start of this list
                        newll = newObj;
                    } else {
                        newObj->next = newll;
                        newll->prev = newObj;
                        newll = newObj;
                    }
                }
            }


            if (objectList[i].status <= 1) { //not in container, not party member item
                if (x < 1024 && y < 1024) {
                    if (y < 1024) {
                        if (!isOverlay) { //exclude first pass objects
                            if (x < 1024) {
                                if ((objectList[i].status & 1) == 0)
                                    newObj->info |= 128; //128 1=stealing, 0=ok to take
                            }

                            if ((x >= 288) && (x <= 326) && (y >= 344) && (y <= 382))
                                newObj->info |= 128; //castle
                            //cheese exception!
                            if ((x == 320) && (y == 351)) {
                                if (newObj->type == OBJ_CHEESE)
                                    newObj->info -= 128;
                            }

                            if ((x == 137) && (y == 185))
                                newObj->info |= 128; //abby loot

                            if ((x >= 883) && (x <= 899) && (y >= 426) && (y <= 445)) { //lyceam
                                if (!((x == 897) && (y == 443)))
                                    newObj->info |= 128;
                            }
                            if ((x >= 1257) && (x <= 1261) && (y >= 49) && (y <= 53)) { //bonn
                                newObj->info |= 128;
                            }

                            ol_tag_prev = ol_tag;
                            ol_tag = objectList[i].tag;

                            //if newll is set objects stored in temp array will be added
                            if (newll != NULL) { //add container's objects to container

                                newObj->more = newll;
                                newll->prev = newObj;

                                object *myobj2 = newll;
                                bool hasNext = true;

                                while (hasNext) {
                                    //castle_obj:
                                    if ((x >= 288) && (x <= 326) && (y >= 344) && (y <= 382))
                                        myobj2->info |= 128; //castle
                                    if ((x == 138) && (y == 185))
                                        myobj2->info |= 128; //abby loot
                                    if ((x == 140) && (y == 185))
                                        myobj2->info |= 128; //abby loot
                                    if ((x == 138) && (y == 189))
                                        myobj2->info |= 128; //abby loot
                                    if ((x == 139) && (y == 189))
                                        myobj2->info |= 128; //abby loot
                                    if ((x >= 883) && (x <= 899) && (y >= 426) && (y <= 445)) { //lyceam
                                        if (!((x == 897) && (y == 443)))
                                            myobj2->info |= 128;
                                    }
                                    if ((x >= 1024) && (myobj2->info & 128))
                                        myobj2->info -= 128; //dungeon
                                    if ((x >= 1257) && (x <= 1261) && (y >= 49) && (y <= 53)) { //bonn
                                        myobj2->info |= 128;
                                    }
                                    if ((x >= 1940) && (x <= 1954) && (y >= 212) && (y <= 218)) { //treasure room
                                        if (objectInfo[sprlnk[myobj2->Type()] + (myobj2->type >> 10)].flags & 4096) {
                                            myobj2->more2 /= 4;
                                            if (myobj2->more2 == 0) myobj2->more2 = 1;
                                        }
                                    }

                                    if ((x == 1642) && (y == 64)) myobj2->more2 = 10; //gold nuggets in theif's house

                                    ENHANCEnewn(myobj2, 8, 4);
                                    if (myobj2->next) {
                                        myobj2 = (object *) myobj2->next;
                                    } else {
                                        hasNext = false;
                                    }
                                }
                                newll = NULL;
                            }
                            ///  cocoa start here

                            if (newObj->type == OBJ_STAFF)
                                newObj->more = NULL; //staff cannot hold original ultima6 type spells

                            if (objectInfo[sprlnk[newObj->Type()] + (newObj->type >> 10)].flags & 4096) { //set quantity
                                newObj->more2 = objectList[i].qty; //more2<-
                                if (newObj->more2 == 0)
                                    newObj->more2 = 1;
                            }
                            if ((newObj->Type()) == 64) { //key, set type!
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 58) { //spell, set type!
                                myobj->more2 = objectList[i].tag; //more2<-
                                if (spellname[myobj->more2]->l == 0) myobj->more2 = (0 << 4) + 7;
                            }

                            ENHANCEnewn(myobj, 8, 4);
                            if ((x >= 1940) && (x <= 1954) && (y >= 212) && (y <= 218)) { //treasure room
                                if (objectInfo[sprlnk[myobj->Type()] + (myobj->type >> 10)].flags & 4096) {
                                    myobj->more2 /= 4;
                                    if (myobj->more2 == 0) myobj->more2 = 1;
                                }
                            }

                            switch (newObj->type) {
                                case OBJ_MOONSTONE:
                                    moongatex[newObj->type >> 10] = x;
                                    moongatey[newObj->type >> 10] = y - 1;
                                    moongate[newObj->type >> 10][0] = OBJnew();
                                    moongate[newObj->type >> 10][0]->type = 85 + 1024;
                                    moongate[newObj->type >> 10][1] = OBJnew();
                                    moongate[newObj->type >> 10][1]->type = 85;
                                case OBJ_VORTEX_CUBE:
                                case OBJ_RUNE_OF_HONESTY:
                                case OBJ_RUNE_OF_COMPASSION:
                                case OBJ_RUNE_OF_VALOR:
                                case OBJ_RUNE_OF_JUSTICE:
                                case OBJ_RUNE_OF_SACRIFICE:
                                case OBJ_RUNE_OF_HONOR:
                                case OBJ_RUNE_OF_SPIRITUALITY:
                                case OBJ_RUNE_OF_HUMILITY:
                                case OBJ_SILVER_TABLET:
                                case OBJ_SILVER_FRAGMENT:
                                case OBJ_BROKEN_LENS:
                                case OBJ_PART_OF_A_MAP_6:
                                    setQuestItem(newObj);
                                    break;
                            };


                            if ((myobj->Type()) == 182)
                                myobj->info |= (15 << 9); //beehive!


                            if ((myobj->Type()) == 332) { //sign
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 333) { //gargoyle sign
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 152) { //scroll
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 143) { //picture
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 151) { //book
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 333) { //gsign
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 59) { //codex
                                myobj->more2 = 128;
                            }
                            if ((myobj->Type()) == 61) { //circles
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 254) { //cross
                                myobj->more2 = objectList[i].tag; //more2<-
                            }
                            if ((myobj->Type()) == 255) { //tombstone
                                myobj->more2 = objectList[i].tag; //more2<-
                            }

                            //skiff (stealing or not?)
                            if ((myobj->Type()) == 414) {
                                //(horace's skiff, skiff on humility isle, skiff on north-west of Britannia, skiff for crossing to Justice town from east, skiffs underground)
                                if (x >= 1024) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 65) && (y == 492)) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 83) && (y == 177)) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 404) && (y == 90)) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 403) && (y == 94)) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 379) && (y == 89)) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 379) && (y == 93)) myobj->info |= (1 << 9); //free skiff!
                                if ((x == 753) && (y == 715)) myobj->info |= (1 << 9); //free skiff!
                                //more?
                            }

                            long x2 = 0; //skip add object to map
                            long i2;
                            long i4;
                            long i5;
                            long i6;
                            long x3;
                            long y3;
                            long x4;
                            long y4;
                            long x5;
                            long y5;
                            object *myobj2;
                            object *myobj3;
                            //electric field || portcullis || egg
                            if (i2 == OBJ_ELECTRIC_FIELD || i2 == OBJ_PORTCULLIS || i2 == OBJ_EGG) x2 = 1;

                            if (x2 == 0) OBJadd(x, y, myobj);

                            x2 = objectList[i].type >> 10; //x2 sub-index
                            i4 = sprlnk[i2] + x2;

                            if (myobj->Type() == OBJ_BLOOD) { //blood (remove&insert)
                                Init_Obj_Blood(x, y, myobj);
                                continue;
                            }

                            //fix ladders
                            if (i2 == OBJ_LADDER) {
                                Init_Obj_Ladder(x, y, x2, myobj);
                            }


                            if (i2 == OBJ_MINE_SHAFT) { //dungeon entrances
                                Init_Obj_MineShaft(x, y, myobj);
                            }

                            if (i2 == 308) { //hole entrances
                                Init_Obj_HoleEntrance(x, y, myobj);
                            }

                            myobj2 = myobj; //backup original object


                            if (i2 == OBJ_SHIP) { //boat
                                long direction = (x2 >> 1) & 3; //0-3
                                ProcessBoatParts(x, y, direction, myobj);
                            }

                            long xOffset = 0;
                            long yOffset = 0;

                            if (i4 == (sprlnk[163] + 1)) { //h bed
                                myobj = OBJnew();
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[163] + 7)) { //v bed
                                myobj = OBJnew();
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[176] + 1)) { //h draw
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj->info |= 8;
                            }
                            if (i4 == (sprlnk[176] + 3)) { //v draw
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj->info |= 8;
                            }

                            i5 = 85; //blue gate
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            i5 = 84; //red gate
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }

                            i5 = 325; //bookstand
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            i5 = 134; //roast horse carcass
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                myobj->info |= 8;
                                OBJadd(x - 1, y, myobj);
                            }

                            i5 = 168; //remains
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                myobj->info |= 8;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                myobj->info |= 8;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 136; //skewer
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            i5 = 326; //mineshaft
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }

                            i5 = 263; //stocks
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                if ((x == 253) && (y == 169)) {
                                    myobj2->type = 263 + 3 * 1024;
                                    myobj->type = 263 + 2 * 1024;
                                } //man in stocks
                            }
                            i5 = 327; //throne
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }

                            i5 = 312; //lions
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            i5 = 313; //horn
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }



                            i5 = 278;
                            if (i4 == (sprlnk[i5] + 2)) { //passthrough
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            i5 = 280;
                            if (i4 == (sprlnk[i5] + 2)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 309; //bones
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 4)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 311; //stone table
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }


                            i5 = 302; //archs
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }


                            //doors (oaken,...)
                            i5 = 297;
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }

                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);

                                //unlock peer's door
                                if ((x == 152) && (y == 891)) {
                                    myobj->type -= 4 * 1024;
                                    myobj2->type -= 4 * 1024;
                                }

                            }
                            if (i4 == (sprlnk[i5] + 13)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 15)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 298;
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                //exit(9);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 13)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 15)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 299;
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 13)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 15)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 300;
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                //if (objectList[i].tag==1) exit(y);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 13)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 15)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj2->more = myobj; //back link
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }



                            //lever pointer
                            i5 = 268; //lever type
                            if ((myobj2->Type()) == i5) {
                                if (objectList[i].tag != 0) {
                                    for (i6 = 0; i6 <= 15; i6++) {
                                        if (lever[objectList[i].tag][i6] == NULL) {
                                            lever[objectList[i].tag][i6] = myobj2;
                                            //goto mullev;
                                        }
                                    }
                                    // mullev:;
                                }
                            }
                            //switch pointer
                            i5 = 174; //switch type
                            if ((myobj2->Type()) == OBJ_SWITCH) {
                                if (objectList[i].tag != 0) {
                                    for (i6 = 0; i6 <= 15; i6++) {
                                        if (eswitch[objectList[i].tag][i6] == NULL) {
                                            eswitch[objectList[i].tag][i6] = myobj2;
                                            //goto mullev2;
                                        }
                                    }
                                    // mullev2:;
                                }
                            }

                            if ((myobj2->Type()) == OBJ_DOORWAY) {
                                if (objectList[i].tag != 0) {
                                    for (i6 = 0; i6 <= 15; i6++) {
                                        if (portcullis[objectList[i].tag][i6] == NULL) {
                                            portcullis[objectList[i].tag][i6] = myobj2;
                                            break;
                                        }
                                    }
                                }
                            }

                            //2003: doorway, doesn't need back linking, stores value of door lock in more2

                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj2->more2 = objectList[i - 1].tag;
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj2->more2 = objectList[i - 1].tag;
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj2->more2 = objectList[i - 1].tag;
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj2->more2 = objectList[i - 1].tag;
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj2->more2 = objectList[i - 1].tag;
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj2->more2 = objectList[i - 1].tag;
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->info |= 8;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }


                            i5 = 304; //fireplace
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            i5 = 304; //fireplace
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            i5 = 304; //fireplace
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }

                            i5 = 304; //fireplacev
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            i5 = 304; //fireplace
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            i5 = 304; //fireplace
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }

                            i5 = 144; //wall banner
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            i5 = 159; //clock
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            i5 = 162; //stove
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            i5 = 216; //bookcase
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }


                            i5 = 331; //sign
                            if (i4 == (sprlnk[i5] + 1)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 9)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 13)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 15)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 17)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 19)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 21)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x, y - 1, myobj);
                            }


                            i5 = 341; //4xdead flying gargoyle
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                myobj->info |= 8;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                myobj->info |= 8;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                myobj->info |= 8;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 340; //4xdead cyclops
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                myobj->info |= 8;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                myobj->info |= 8;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                myobj->info |= 8;
                                OBJadd(x - 1, y - 1, myobj);
                            }

                            i5 = 324; //4xpillar
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 322; //4xwater pool
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 329; //4xsingularity
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }

                            i5 = 323; //4xmonolith
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 328; //4xalter
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 321; //4xstatue
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 7)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }


                            i5 = 195;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 196;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 197;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 201;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 214;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 233;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 316;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 330;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 200;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }

                            i5 = 198;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            i5 = 199;
                            if (i4 == (sprlnk[i5] + 3)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }

                            i5 = 267; //gilotene
                            if (i4 == (sprlnk[i5] + 4)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }

                            i5 = 163; //nicebed
                            if (i4 == (sprlnk[i5] + 5)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            if (i4 == (sprlnk[i5] + 11)) {
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 1 * 1024;
                                OBJadd(x - 1, y, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 2 * 1024;
                                OBJadd(x, y - 1, myobj);
                                myobj = OBJnew();
                                myobj->more = myobj2;
                                myobj->type = objectList[i].type - 3 * 1024;
                                OBJadd(x - 1, y - 1, myobj);
                            }
                            // skipobj:;
                        }
                    }
                }
            }
        }
    }
}

void spreadCreature(long x, long y, object *obj) {
    unsigned short spreadX[32];
    unsigned short spreadY[32];
    long spreadCount = 0;

    for (long newY = y - 2; newY <= (y + 2); newY++) {
        for (long newX = x - 2; newX <= (x + 2); newX++) {
            if ((abs(newY - y) + abs(newX - x)) < 4) {
                if (isInBounds(newX, newY)) { //map boundaries check
                    if (!locationBlocked(newX, newY)) { //blocked location
                        if (locationWalkable(newX, newY)) { //walkable location
                            // OBJcheckbolt - This checks if you can get from one spot to
                            //                another without running into an obstruction.
                            //                Candidate for function rename.
                            if (OBJcheckbolt(x, y, newX, newY) == NULL) { //not blocked
                                spreadX[spreadCount] = newX;
                                spreadY[spreadCount] = newY;
                                spreadCount++;
                            }//OBJcheckbolt
                        }//walkable
                    }//od
                }
            }//<4
        }
    }

    if (spreadCount == 0)
        return;
    spreadCount = rnd * spreadCount;
    OBJadd(spreadX[spreadCount], spreadY[spreadCount], obj);
}



void func1(object *obj) {
    unsigned short objType = obj->Type();
    unsigned long x;
    unsigned long y;
    unsigned long z;

    // monster spawner
    if (objType == OBJ_EGG) {//extract egg
        //mimic box in Sutek's house
        bool isSutekHouseEgg = (x == 778) && (y == 937);

        if (!isSutekHouseEgg) {
            object *baseObj = newll;
            // extractobj2:
            object *nextObj = (object *) baseObj->next;

            object *newObj = nullptr;

            int quantityCheck = objectList[baseObj->more2].qty;
            int quantity = 0;

            if (quantity == 0) {
                quantity = 1;
            } else if (quantity > 8) {
                quantity = 8;
            } else if (quantity > 2) {
                quantity = int((quantity - 2) / 2) + 2;
            }

            // multiple_enemy:
            quantity--;
            if (quantity) {
                newObj = OBJnew();
                newObj->type = baseObj->type;
                newObj->more2 = newObj->more2;
            }

            baseObj->next = NULL;
            baseObj->prev = NULL;


            if (isCreatureOrNPC(baseObj)) {
                //EXCLUDE all multi-sprite creatures
                //        tangle vines(365), reapers(347),
                //        squid(345), serpent(346), silver serpent(413),
                //        all two part movers(426-431)
                bool exclusion = false;
                if (objType == OBJ_TANGLE_VINE_POD || objType == OBJ_REAPER ||
                        objType == OBJ_GIANT_SQUID || objType == OBJ_SEA_SERPENT ||
                        objType == OBJ_SILVER_SERPENT || (objType >= OBJ_GIANT_SCORPION && objType <= OBJ_HORSE_WITH_RIDER)) {
                    exclusion = true;
                }

                if (!exclusion) {
                    //check original square for a creature
                    if (locationBlocked(x, y)) {
                        object *spreadObj = od[y][x];
                        while(spreadObj) {
                            if (spreadObj->info & 4) {
                                spreadCreature(x, y, spreadObj);
                            }
                            spreadObj = (object *) spreadObj->next;
                        }
                    }
                }
            }

            //scatter if stationary enemy
            if (isCreateStationary(baseObj)) {
                bool scattered = false;

                while (!scattered) {
                    unsigned short newX = x + (unsigned short) (rnd * 7) - 3;
                    unsigned short newY = y + (unsigned short) (rnd * 7) - 3;

                    if (locationBlocked(newX, newY)) continue;
                    OBJcheckflags(newX, newY);
                    if ((bt[newY][newX] & 1024) == 0) continue;

                    OBJadd(newX, newY, baseObj);
                    scattered = true;
                }
            } else {
                OBJadd(x, y, baseObj);
            }

            // if not a creature or npc type
            if (!isCreatureOrNPC(baseObj)) { //gold nugget(s)/reagent(s)/..
                baseObj->info = 0;
                z = baseObj->more2;
                z = rnd * (z + 1);
                if (z == 0) z = 1;
                baseObj->more2 = z;
            } else {
                //2006: use movernew(unsigned short type,unsigned short x,unsigned short y,unsigned long flags)
                unsigned short newX = baseObj->x;
                unsigned short newY = baseObj->y;
                OBJremove(baseObj);//discard existing object

                //FIX for 4 part movers to not be ignored by movernew
                if (baseObj->type == OBJ_CYCLOPS)
                    baseObj->type += (3 * 1024);
                else if (baseObj->type == OBJ_GARGOYLE_WINGED)
                    baseObj->type += (3 * 1024);

                movernew(baseObj->type, newX, newY, 0);

                OBJrelease(baseObj);
            }

            if (newObj) {
                baseObj = newObj;
            }

            if (nextObj != nullptr) {
                baseObj = nextObj;
            }
        }
    }
}

