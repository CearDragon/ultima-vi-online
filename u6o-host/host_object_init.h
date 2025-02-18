//
// Created by Cocoa-AP on 2/2/2025.
//

#ifndef HOST_OBJECT_INIT_H
#define HOST_OBJECT_INIT_H

#include "data_host.h"
#include "../data_both.h" /* schedule* etc. */
#include "../function_host.h" /* host functions */

// misc ladders with no label
#define h_miscLadderXa 291
#define h_miscLadderYa 347
#define h_miscLadderExitA 1099 + (83 << 16)

#define h_miscLadderXb 1099
#define h_miscLadderYb 107
#define h_miscLadderExitB 291 + (347 << 16)

#define h_miscLadderXc 323
#define h_miscLadderYc 347
#define h_miscLadderExitC 1107 + (83 << 16)

#define h_miscLadderXd 1107
#define h_miscLadderYd 83
#define h_miscLadderExitD 323 + (347 << 16)

#define h_miscLadderXe 323
#define h_miscLadderYe 379
#define h_miscLadderExitE 1107 + (91 << 16)

#define h_miscLadderXf 1107
#define h_miscLadderYf 91
#define h_miscLadderExitF 323 + (379 << 16)

#define h_miscLadderXg 291
#define h_miscLadderYg 379
#define h_miscLadderExitG 1099 + (91 << 16)

#define h_miscLadderXh 1099
#define h_miscLadderYh 91
#define h_miscLadderExitH 291 + (379 << 16)

// Treasure Hole ladder position check more2 values
#define h_treasureHoleXa 707
#define h_treasureHoleYa 835
#define h_treasureHoleXb 1203
#define h_treasureHoleYb 211

// Harpsichord ladder position check more2 values
#define h_cellarXa 73
#define h_cellarYa 539
#define h_cellarXb 1041
#define h_cellarYb 131

// Britain Hotel ladder position check more2 values
#define h_britainHotelXa 340
#define h_britainHotelYa 433
#define h_britainHotelXb 1108
#define h_britainHotelYb 105

// Harpsichord ladder Ladder position check more2 values
#define h_harpLadderXa 930
#define h_harpLadderYa 210
#define h_harpLadderXb 1258
#define h_harpLadderYb 50

// Catacomb ladder position check more2 values
#define h_catacombXa 868
#define h_catacombYa 347
#define h_catacombXb 1244
#define h_catacombYb 83

// Library ladder position check more2 values
#define h_libraryXa 891
#define h_libraryYa 435
#define h_libraryXb 1243
#define h_libraryYb 107

// Sutek's Castle ladder position check more2 values
#define h_sutekCastleXa 787
#define h_sutekCastleYa 948
#define h_sutekCastleXb 1219
#define h_sutekCastleYb 236

// Cyclops 1 ladder position check more2 values
#define h_cyclops1Xa 605
#define h_cyclops1Ya 285
#define h_cyclops1Xb 1173
#define h_cyclops1Yb 69

// Cyclops 2 ladder position check more2 values
#define h_cyclops2Xa 609
#define h_cyclops2Ya 285
#define h_cyclops2Xb 1177
#define h_cyclops2Yb 69

// Cyclops 3 ladder position check more2 values
#define h_cyclops3Xa 607
#define h_cyclops3Ya 287
#define h_cyclops3Xb 1175
#define h_cyclops3Yb 71

// Cyclops 4 ladder position check more2 values
#define h_cyclops4Xa 605
#define h_cyclops4Ya 289
#define h_cyclops4Xb 1173
#define h_cyclops4Yb 73

// Cyclops 5 ladder position check more2 values
#define h_cyclops5Xa 609
#define h_cyclops5Ya 289
#define h_cyclops5Xb 1177
#define h_cyclops5Yb 73

// Britain West Mine ladder position check more2 values
#define h_britainWestMineXa 234
#define h_britainWestMineYa 409
#define h_britainWestMineXb 1082
#define h_britainWestMineYb 97

// Britain West Cave ladder position check more2 values
#define h_britainWestCaveXa 185
#define h_britainWestCaveYa 436
#define h_britainWestCaveXb 1065
#define h_britainWestCaveYb 108

// Spider Cave ladder position check more2 values
#define h_spiderCaveXa 92
#define h_spiderCaveYa 250
#define h_spiderCaveXb 1044
#define h_spiderCaveYb 58

// West Wrong ladder position check more2 values
#define h_westWrongXa 500
#define h_westWrongYa 81
#define h_westWrongXb 1148
#define h_westWrongYb 17

// East Wrong ladder position check more2 values
#define h_eastWrongXa 627
#define h_eastWrongYa 113
#define h_eastWrongXb 1179
#define h_eastWrongYb 25

// West Ant Hole ladder position check more2 values
#define h_westAntHoleXa 835
#define h_westAntHoleYa 195
#define h_westAntHoleXb 1235
#define h_westAntHoleYb 51

// East Ant Hole ladder position check more2 values
#define h_eastAntHoleXa 867
#define h_eastAntHoleYa 187
#define h_eastAntHoleXb 1243
#define h_eastAntHoleYb 43

// Spike and Trap Cave ladder position check more2 values
#define h_spikeAndTrapCaveXa 964
#define h_spikeAndTrapCaveYa 306
#define h_spikeAndTrapCaveXb 1268
#define h_spikeAndTrapCaveYb 74

// Hythloth ladder position check more2 values
#define h_hythlothXa 948
#define h_hythlothYa 930
#define h_hythlothXb 1260
#define h_hythlothYb 234

// Heroes Hole ladder position check more2 values
#define h_heroesHoleXa 348
#define h_heroesHoleYa 809
#define h_heroesHoleXb 1108
#define h_heroesHoleYb 201

// Heftimus Cave ladder position check more2 values
#define h_heftimusCaveXa 132
#define h_heftimusCaveYa 857
#define h_heftimusCaveXb 1060
#define h_heftimusCaveYb 209

// Destard position ladder check more2 values
#define h_destardXa 284
#define h_destardYa 657
#define h_destardXb 1092
#define h_destardYb 161

// Despise ladder position check more2 values
#define h_despiseXa 365
#define h_despiseYa 265
#define h_despiseXb 1117
#define h_despiseYb 65

// Swamp Cave position ladder check more2 values
#define h_swampCaveXa 611
#define h_swampCaveYa 363
#define h_swampCaveXb 1179
#define h_swampCaveYb 91

// Buccaneer's Den ladder position check more2 values
#define h_buccaneersDenXa 564
#define h_buccaneersDenYa 594
#define h_buccaneersDenXb 1164
#define h_buccaneersDenYb 146

// Remains ladder position check more2 values
#define h_remainsXa 942
#define h_remainsYa 521
#define h_remainsXb 1262
#define h_remainsYb 129

object *Obj_Staff(object *staffObj);
object *Obj_Spell(object *spellObj, long objListIndex);

void Init_Obj_Blood(int x, int y, object *blood);

void Init_Obj_Ladder(int x, int y, int subType, object *ladder);
void Init_Obj_MineShaft(int x, int y, object *mineShaft);
void Init_Obj_HoleEntrance(int x, int y, object *holeEntrance);

void Init_Obj_Bed(int x, int y, object *bed);

void Init_Obj_Egg(object *eggObj, long x, long y);
void ProcessBoatParts(int x, int y, int dir, object *boatRef);

#endif //HOST_OBJECT_INIT_H
