//
// Created by Cocoa-AP on 2/2/2025.
//

#include "host_object_init.h"

void Init_Obj_Ladder(int x, int y, int subType, object *ladder) {
    if ((x >= 1024) && (y <= 256)) { //dungeon ladders
        if (subType == 0) { //down
            ladder->more2 = ladder->x + 256 + (ladder->y << 16);
            if (x >= (1024 + 768))
                ladder->more2 = ladder->x - 768 + ((ladder->y + 256) << 16);
        } else { //up
            ladder->more2 = ladder->x - 256 + (ladder->y << 16);
        }
    } else {
        const long checkMiscX[] = {h_miscLadderXa, h_miscLadderXb, h_miscLadderXc,
                                   h_miscLadderXd, h_miscLadderXe, h_miscLadderXf,
                                   h_miscLadderXg, h_miscLadderXh};
        const long checkMiscY[] = {h_miscLadderYa, h_miscLadderYb, h_miscLadderYc,
                                   h_miscLadderYd, h_miscLadderYe, h_miscLadderYf,
                                   h_miscLadderYg, h_miscLadderYh};
        const long checkMiscExits[] = {h_miscLadderExitA, h_miscLadderExitB,
                                       h_miscLadderExitC, h_miscLadderExitD,
                                       h_miscLadderExitE, h_miscLadderExitF,
                                       h_miscLadderExitG, h_miscLadderExitH};

        for (int ii = 0; ii < 8; ii++) {
            if ((x == checkMiscX[ii]) && (y == checkMiscY[ii])) {
                ladder->more2 = checkMiscExits[ii];
                break;
            }
        }

        const long checkBothXa[] = {h_cellarXa, h_britainHotelXa, h_harpLadderXa, h_catacombXa,
                                    h_libraryXa, h_sutekCastleXa, h_cyclops1Xa, h_cyclops2Xa,
                                    h_cyclops3Xa, h_cyclops4Xa, h_cyclops5Xa, h_remainsXa};
        const long checkBothYa[] = {h_cellarYa, h_britainHotelYa, h_harpLadderYa, h_catacombYa,
                                    h_libraryYa, h_sutekCastleYa, h_cyclops1Ya, h_cyclops2Ya,
                                    h_cyclops3Ya, h_cyclops4Ya, h_cyclops5Ya, h_remainsYa};
        const long checkBothXb[] = {h_cellarXb, h_britainHotelXb, h_harpLadderXb, h_catacombXb,
                                    h_libraryXb, h_sutekCastleXb, h_cyclops1Xb, h_cyclops2Xb,
                                    h_cyclops3Xb, h_cyclops4Xb, h_cyclops5Xb, h_remainsXb};
        const long checkBothYb[] = {h_cellarYb, h_britainHotelYb, h_harpLadderYb, h_catacombYb,
                                    h_libraryYb, h_sutekCastleYb, h_cyclops1Yb, h_cyclops2Yb,
                                    h_cyclops3Yb, h_cyclops4Yb, h_cyclops5Yb, h_remainsYb};

        for (int ii = 0; ii < 12; ii++) {
            if ((x == checkBothXa[ii]) && (y == checkBothYa[ii])) {
                ladder->more2 = checkBothXb[ii] + (checkBothYb[ii] << 16);
                break;
            }
            if ((x == checkBothXb[ii]) && (y == checkBothYb[ii])) {
                ladder->more2 = checkBothXa[ii] + (checkBothYa[ii] << 16);
                break;
            }
        }

        const long checkXa[] = {h_treasureHoleXa, h_britainWestMineXa, h_britainWestCaveXa, h_spiderCaveXa,
                                h_westWrongXa,
                                h_eastWrongXa, h_westAntHoleXa, h_eastAntHoleXa, h_spikeAndTrapCaveXa, h_hythlothXa,
                                h_heroesHoleXa,
                                h_heftimusCaveXa, h_destardXa, h_despiseXa, h_swampCaveXa, h_buccaneersDenXa};
        const long checkYa[] = {h_treasureHoleYa, h_britainWestMineYa, h_britainWestCaveYa, h_spiderCaveYa,
                                h_westWrongYa,
                                h_eastWrongYa, h_westAntHoleYa, h_eastAntHoleYa, h_spikeAndTrapCaveYa, h_hythlothYa,
                                h_heroesHoleYa,
                                h_heftimusCaveYa, h_destardYa, h_despiseYa, h_swampCaveYa, h_buccaneersDenYa};
        const long checkXb[] = {h_treasureHoleXb, h_britainWestMineXb, h_britainWestCaveXb, h_spiderCaveXb,
                                h_westWrongXb,
                                h_eastWrongXb, h_westAntHoleXb, h_eastAntHoleXb, h_spikeAndTrapCaveXb, h_hythlothXb,
                                h_heroesHoleXb,
                                h_heftimusCaveXb, h_destardXb, h_despiseXb, h_swampCaveXb, h_buccaneersDenXb};
        const long checkYb[] = {h_treasureHoleYb, h_britainWestMineYb, h_britainWestCaveYb, h_spiderCaveYb,
                                h_westWrongYb,
                                h_eastWrongYb, h_westAntHoleYb, h_eastAntHoleYb, h_spikeAndTrapCaveYb, h_hythlothYb,
                                h_heroesHoleYb,
                                h_heftimusCaveYb, h_destardYb, h_despiseYb, h_swampCaveYb, h_buccaneersDenYb};

        for (int ii = 0; ii < 16; ii++) {
            if ((x == checkXb[ii]) && (y == checkYb[ii])) {
                ladder->more2 = checkXa[ii] + (checkYa[ii] << 16);
                break;
            }
        }
    }
}

void Init_Obj_MineShaft(int x, int y, object *mineShaft) {
    if (y >= 256)
        mineShaft->more2 = mineShaft->x + 768 + ((mineShaft->y - 256) << 16);
    else {
        const long checkXa[] = {h_britainWestMineXa, h_britainWestCaveXa, h_spiderCaveXa,
                                h_westWrongXa, h_eastWrongXa, h_spikeAndTrapCaveXa,
                                h_hythlothXa, h_heroesHoleXa,  h_heftimusCaveXa,
                                h_destardXa, h_despiseXa, h_swampCaveXa, h_buccaneersDenXa};
        const long checkYa[] = {h_britainWestMineYa, h_britainWestCaveYa, h_spiderCaveYa,
                                h_westWrongYa, h_eastWrongYa, h_spikeAndTrapCaveYa,
                                h_hythlothYa, h_heroesHoleYa, h_heftimusCaveYa,
                                h_destardYa, h_despiseYa, h_swampCaveYa, h_buccaneersDenYa};
        const long checkXb[] = {h_britainWestMineXb, h_britainWestCaveXb, h_spiderCaveXb,
                                h_westWrongXb, h_eastWrongXb, h_spikeAndTrapCaveXb,
                                h_hythlothXb, h_heroesHoleXb, h_heftimusCaveXb,
                                h_destardXb, h_despiseXb, h_swampCaveXb, h_buccaneersDenXb};
        const long checkYb[] = {h_britainWestMineYb, h_britainWestCaveYb, h_spiderCaveYb,
                                h_westWrongYb, h_eastWrongYb, h_spikeAndTrapCaveYb,
                                h_hythlothYb, h_heroesHoleYb, h_heftimusCaveYb,
                                h_destardYb, h_despiseYb, h_swampCaveYb, h_buccaneersDenYb};

        for (int ii = 0; ii < 13; ii++) {
            if ((x == checkXa[ii]) && (y == checkYa[ii])) {
                mineShaft->more2 = checkXb[ii] + (checkYb[ii] << 16);
                break;
            }
        }
    }
}

void Init_Obj_HoleEntrance(int x, int y, object *holeEntrance) {
    if ((x >= 1024) && (y <= 256)) { //dungeon holes
        holeEntrance->more2 = holeEntrance->x + 256 + (holeEntrance->y << 16);
        if (x >= (1024 + 768)) holeEntrance->more2 = holeEntrance->x - 768 + ((holeEntrance->y + 256) << 16);
    } else {
        const long holeCheckXa[] = {h_westAntHoleXa, h_eastAntHoleXa};
        const long holeCheckYa[] = {h_westAntHoleYa, h_eastAntHoleYa};
        const long holeCheckXb[] = {h_westAntHoleXb, h_eastAntHoleXb};
        const long holeCheckYb[] = {h_westAntHoleYb, h_eastAntHoleYb};

        for (int ii = 0; ii < 2; ii++) {
            if ((x == holeCheckXa[ii]) && (y == holeCheckYa[ii])) {
                holeEntrance->more2 = holeCheckXb[ii] + (holeCheckYb[ii] << 16);
                break;
            }
        }
    }
}

void Init_Obj_Blood(int x, int y, object *blood) {
    object *body = (object *) blood->prev;
    if (body != nullptr) {
        if (body->Type() == OBJ_DEAD_BODY) {
            OBJremove(blood);
            OBJremove(body);
            OBJadd(x, y, blood);
            OBJadd(x, y, body);
        }
    }
}

void addBoatPart(int x, int y, int type, object *boatObj, int offsetX, int offsetY) {
    object *myobj = OBJnew();
    myobj->type = type;
    myobj->info |= 8; // <-base object
    OBJadd(x + offsetX, y + offsetY, myobj);
    myobj->more = boatObj;
}

void ProcessBoatParts(int x, int y, int dir, object *boatRef) {
    object *boatObj = boatRef;

    static mlobj *multiObj;
    multiObj = (mlobj *) malloc(sizeof(object *) * 5);
    boatObj->more = multiObj;

    int offsets[4][5][2] = {
            {{-1, 0},  {0,  -1}, {-1, -1}, {0,  1},  {-1, 1}},
            {{0,  -1}, {1,  0},  {1,  -1}, {-1, 0},  {-1, -1}},
            {{-1, 0},  {0,  1},  {-1, 1},  {0,  -1}, {-1, -1}},
            {{0,  -1}, {-1, 0},  {-1, -1}, {1,  0},  {1,  -1}}
    };
    int types[5] = {-1 * 1024, -8 * 1024, -9 * 1024, 8 * 1024, 7 * 1024};

    for (int i = 0; i < 5; ++i) {
        object *boatPart = OBJnew();
        multiObj->obj[i] = boatPart;
        addBoatPart(x, y, objectList[i].type + types[i], boatObj, offsets[dir][i][0], offsets[dir][i][1]);
    }
}