#ifndef DEFINE_BOTH_H
#define DEFINE_BOTH_H
#define U6O_DEBUG FALSE/* should probably put this on the compiler command line instead of here!! */
#include "random/random.h"

#define VIEW_X_SIZE 32; // to be 24 sooner or later, black area for stats screens etc.
#define VIEW_Y_SIZE 24;

#define U6O_VERSION 1.0 //version 5.5
#define SHA2_ENCRYPTION 1
#define PW_ENCRYPTION 1

#define rndold (((float)rand())/32768.0f)
//#define rnd ((float)((RandomMers()<<8)>>16)/65536.0f)
//#define rnd ((float)((Randomc()<<8)>>16)/65536.0f)
#define rnd Randomc()
//equipped item positions (REVISE) (warning: left and right refer to the character's hand hence they are reversed on screen)
#define helmx 52
#define helmy 132
#define wep_rightx 20
#define wep_righty 170
#define wep_leftx 84
#define wep_lefty 170
#define armourx 52
#define armoury 166
#define bootsx 52
#define bootsy 216
#define ring_rightx 22
#define ring_righty 202
#define ring_leftx 84
#define ring_lefty 202
#define neckx 20
#define necky 138
#define INVALID_NET 0xFFFFFFFF//used by net_send(),...
//mover flags
#define MVLISTLAST 2047
#define MV_LIGHTBRIGHT 1
#define MV_LIGHTGLOW 2
#define MV_INVISIBLE 4
#define MV_PARALYZE 8
#define MV_SLEEP 16
#define MV_SLEEPBEDVERTICAL 32
#define MV_SLEEPBEDHORIZONTAL 64
#define MV_PROTECT 128
#define MV_STORMCLOAK 256
#define MV_TYPING 512
#define MV_KEYBTARGET 1024
#define MV_SPEAKING 2048
//key flags (REVISE)
#define KEYup 1
#define KEYdown 2
#define KEYleft 4
#define KEYright 8
#define KEYmb1 16  //relates to mf
#define KEYu 32 //use
#define KEYl 64 //look
#define KEYmbclick 128 //relates to mf
#define KEYa 256 //attack
#define KEYup2 512
#define KEYdown2 1024
#define KEYleft2 2048
#define KEYright2 4096
#define KEYbreakcombat 8192
#define KEYquan 16384
#define KEYf1 32768
//number of possible socket threads for client and host (REVISE)
#define SOCKETLAST 65535
//movernew()
#define MOVERNEW_FLAG_NORESPAWN 1
#define MOVERNEW_FLAG_PASSIVE 2
#define MOVERNEW_FLAG_FRIENDLY 4
#define MOVERNEW_FLAG_AGRESSIVE 8
//wpf_pathfind()
#define PFleft 0
#define PFright 1
#define PFup 2
#define PFdown 3
#define WPF_NOPATHFOUND 0
#define WPF_PATHFOUND 1
//getinput return values
#define GETINPUT_TAB_PRESSED_STOP 0
#define GETINPUT_TAB_PRESSED_INIT 1
#define GETINPUT_TAB_PRESSED 2
//spell reagent flags /* luteijn: SIGH totally different than the original; hopefully these are consistantly used so we can eventually make them the same as the original */
#define BLAC 1
#define BLOO 2
#define GARL 4
#define GINS 8
#define MAND 16
#define NIGH 32
#define SPID 64
#define SULF 128
//used by gypsy to select correct vial
#define U6O2_HONESTY x4=0;
#define U6O2_COMPASSION x4=1;
#define U6O2_VALOR x4=2;
#define U6O2_JUSTICE x4=3;
#define U6O2_SACRIFICE x4=4;
#define U6O2_HONOR x4=5;
#define U6O2_SPIRITUALITY x4=6;
#define U6O2_HUMILITY x4=7;
//eXtra mouse buttons
#define WM_XBUTTONDOWN                  0x020B
#define WM_XBUTTONUP                    0x020C
#define XBUTTON1      0x0001
#define XBUTTON2      0x0002

/* special effect types TODO: use these everywhere */
#define SF_UNNAMED_0	(0) /* probably never used... */
#define SF_ATTACK_HIT	(1)
#define SF_ARROW	(2)
#define SF_TXT_PORTRAIT	(3)
#define SF_SOUND_EFFECT	(4)
#define SF_LIGHTNING	(5) 
#define SF_BOLT		(6)
#define SF_BOOMERANG	(7)
#define SF_FIRE_BOLT	(8)
#define SF_THROWN_OBJ	(9)
#define SF_DAGGER	(10)
#define SF_THROWING_AXE	(11)
#define SF_SPEAR	(12)
#define SF_SLING_STONE	(13)
#define SF_BLUE_BALL	(14)
#define SF_KILL_BOLT	(15)
#define SF_ATTACK_MISS	(16)
#define SF_CANNON_BALL	(17)
#define SF_GREEN_BALL	(18)
#define SF_BLUE_BOLT	(19)
#define SF_PINK_BALL	(20)
#define SF_STOLEN_ITEM	(21)
#define SF_FIREBALLS	(22)
#define SF_GREEN_BALLS	(23)
#define SF_LIGHTNINGS	(24)
#define SF_DEATH_BOLTS	(25)
#define SF_TREMOR	(26)
#define SF_INV_ADD	(27)
#define SF_RED_BOLT	(28)
#define SF_RED_BALL	(29)
/* unused so far */
#define SF_UNNAMED_30	(30)
#define SF_UNNAMED_31	(31)
#define SF_UNNAMED_32	(32)
#define SF_UNNAMED_33	(33)
#define SF_UNNAMED_34	(34)
#define SF_UNNAMED_35	(35)
#define SF_UNNAMED_36	(36)

/* protocol message types */

#define MSG_0			(0) /* probably never used */
#define MSG_OBSOLETE_SCENE	(1)
#define MSG_SF			(2)
#define MSG_CHARACTER		(3)
#define MSG_MOUSE_OBJECT	(4)
#define MSG_KEY_EVENT		(5)
#define MSG_LOCAL_TALK		(6)
#define MSG_DIRECTED_TALK	(7)
#define MSG_STATUS_MESSAGE	(8)
#define MSG_NAME_REQUEST	(9)
#define MSG_NAME_REPLY		(10)
#define MSG_BG_MIDI		(11)
#define MSG_GLOBAL_TALK		(12)
#define MSG_MULTIPLE_NUMBER	(13)
#define MSG_WIND_DIRECTION	(14)
/* TODO/FIXME etc, see messageformat.html and actual code! */

/* objects */

#define OBJ_TYPE_BITMASK (1023)
#define OBJ_QUEST_ITEM (256)

#define OBJ_NOTHING_0 (0)
#define OBJ_LEATHER_HELM (1)
#define OBJ_CHAIN_COIF (2)
#define OBJ_IRON_HELM (3)
#define OBJ_SPIKED_HELM (4)
#define OBJ_WINGED_HELM (5)
#define OBJ_BRASS_HELM (6)
#define OBJ_GARGOYLE_HELM (7)
#define OBJ_MAGIC_HELM (8)
#define OBJ_WOODEN_SHIELD (9)
#define OBJ_CURVED_HEATER (10)
#define OBJ_WINGED_SHIELD (11)
#define OBJ_KITE_SHIELD (12)
#define OBJ_SPIKED_SHIELD (13)
#define OBJ_BLACK_SHIELD (14)
#define OBJ_DOOR_SHIELD (15)
#define OBJ_MAGIC_SHIELD (16)
#define OBJ_CLOTH_ARMOUR (17)
#define OBJ_LEATHER_ARMOR (18)
#define OBJ_RING_MAIL (19)
#define OBJ_SCALE_MAIL (20)
#define OBJ_CHAIN_MAIL (21)
#define OBJ_PLATE_MAIL (22)
#define OBJ_MAGIC_ARMOUR (23)
#define OBJ_SPIKED_COLLAR (24)
#define OBJ_GUILD_BELT (25)
#define OBJ_GARGOYLE_BELT (26)
#define OBJ_LEATHER_BOOTS (27)
#define OBJ_SWAMP_BOOTS (28)
#define OBJ_EARTH (29)
#define OBJ_FLOOR_WOOD (30)
#define OBJ_FLOOR_TILE (31)
#define OBJ_FLOOR_BLUE (32)
#define OBJ_SLING (33)
#define OBJ_CLUB (34)
#define OBJ_MAIN_GAUCHE (35)
#define OBJ_SPEAR (36)
#define OBJ_THROWING_AXE (37)
#define OBJ_DAGGER (38)
#define OBJ_MACE (39)
#define OBJ_MORNING_STAR (40)
#define OBJ_BOW (41)
#define OBJ_CROSSBOW (42)
#define OBJ_SWORD (43)
#define OBJ_TWO_HANDED_HAMMER (44)
#define OBJ_TWO_HANDED_AXE (45)
#define OBJ_TWO_HANDED_SWORD (46)
#define OBJ_HALBERD (47)
#define OBJ_GLASS_SWORD (48)
#define OBJ_BOOMERANG (49)
#define OBJ_TRIPLE_CROSSBOW (50)
#define OBJ_FORCE_FIELD (51)
#define OBJ_WIZARD_EYE (52)
#define OBJ_WEB (53)
#define OBJ_MAGIC_BOW (54)
#define OBJ_ARROW (55)
#define OBJ_BOLT (56)
#define OBJ_SPELLBOOK (57)
#define OBJ_SPELL (58)
#define OBJ_CODEX (59)
#define OBJ_BOOK_OF_PROPHECIES (60)
#define OBJ_BOOK_OF_CIRCLES (61)
#define OBJ_VORTEX_CUBE (62)
#define OBJ_LOCK_PICK (63)
#define OBJ_KEY (64)
#define OBJ_BLACK_PEARL (65)
#define OBJ_BIT_OF_BLOOD_MOSS (66)
#define OBJ_BULB_OF_GARLIC (67)
#define OBJ_GINSENG_ROOT (68)
#define OBJ_MANDRAKE_ROOT (69)
#define OBJ_NIGHTSHADE_MUSHROOM (70)
#define OBJ_STRAND_OF_SPIDERSILK (71)
#define OBJ_BIT_OF_SULFUROUS_ASH (72)
#define OBJ_MOONSTONE (73)
#define OBJ_ANKH_AMULET (74)
#define OBJ_SNAKE_AMULET (75)
#define OBJ_AMULET_OF_SUBMISSION (76)
#define OBJ_GEM (77)
#define OBJ_STAFF (78)
#define OBJ_LIGHTNING_WAND (79)
#define OBJ_FIRE_WAND (80)
#define OBJ_STORM_CLOAK (81)
#define OBJ_RING (82)
#define OBJ_FLASK_OF_OIL (83)
#define OBJ_RED_GATE (84)
#define OBJ_MOONGATE (85)
#define OBJ_GAVEL (86)
#define OBJ_ORB_OF_THE_MOONS (87)
#define OBJ_GOLD_COIN (88)
#define OBJ_GOLD_NUGGET (89)
#define OBJ_TORCH (90)
#define OBJ_ZU_YLEM (91)
#define OBJ_SILVER_SNAKE_VENOM (92)
#define OBJ_SEXTANT (93)
#define OBJ_SPINNING_WHEEL (94)
#define OBJ_BUNCH_OF_GRAPES (95)
#define OBJ_BUTTER (96)
#define OBJ_GARGISH_VOCABULARY (97)
#define OBJ_CHEST (98)
#define OBJ_BACKPACK (99)
#define OBJ_SCYTHE (100)
#define OBJ_PITCHFORK (101)
#define OBJ_RAKE (102)
#define OBJ_PICK (103)
#define OBJ_SHOVEL (104)
#define OBJ_HOE (105)
#define OBJ_WOODEN_LADDER (106)
#define OBJ_YOKE (107)
#define OBJ_OVEN_SPATULA (108)
#define OBJ_ROLLING_PIN (109)
#define OBJ_SPATULA (110)
#define OBJ_LADLE (111)
#define OBJ_COOKING_SHEET (112)
#define OBJ_CLEAVER (113)
#define OBJ_KNIFE (114)
#define OBJ_WINE (115)
#define OBJ_MEAD (116)
#define OBJ_ALE (117)
#define OBJ_WINE_GLASS (118)
#define OBJ_PLATE (119)
#define OBJ_MUG (120)
#define OBJ_SILVERWARE (121)
#define OBJ_CANDLE (122)
#define OBJ_MIRROR (123)
#define OBJ_TUNIC (124)
#define OBJ_HANGER (125)
#define OBJ_DRESS (126)
#define OBJ_SKILLET (127)
#define OBJ_LOAF_OF_BREAD (128)
#define OBJ_PORTION_OF_MEAT (129)
#define OBJ_ROLLS (130)
#define OBJ_CAKE (131)
#define OBJ_CHEESE (132)
#define OBJ_HAM (133)
#define OBJ_HORSE_CARCASS (134)
#define OBJ_HORSE_CHOPS (135)
#define OBJ_SKEWER (136)
#define OBJ_PANTS (137)
#define OBJ_PLANT (138)
#define OBJ_FLOWERS (139)
#define OBJ_WALL_MOUNT (140)
#define OBJ_DECORATIVE_SWORD (141)
#define OBJ_DECORATIVE_SHIELD (142)
#define OBJ_PICTURE (143)
#define OBJ_TAPESTRY (144)
#define OBJ_CANDELABRA (145)
#define OBJ_PERSON_SLEEPING (146)
#define OBJ_CAULDRON (147)
#define OBJ_CAULDRON_FULL (148)
#define OBJ_SHIP_DEED (149)
#define OBJ_INKWELL (150)
#define OBJ_BOOK (151)
#define OBJ_SCROLL (152)
#define OBJ_PANPIPES (153)
#define OBJ_TELESCOPE (154)
#define OBJ_CRYSTAL_BALL (155)
#define OBJ_HARPSICHORD (156)
#define OBJ_HARP (157)
#define OBJ_LUTE (158)
#define OBJ_CLOCK (159)
#define OBJ_ENDTABLE (160)
#define OBJ_WATER_VASE (161)
#define OBJ_STOVE (162)
#define OBJ_BED (163)
#define OBJ_FIREPLACE (164)
#define OBJ_STALAGMITE (165)
#define OBJ_SACK_OF_GRAIN (166)
#define OBJ_SACK_OF_FLOUR (167)
#define OBJ_REMAINS (168)
#define OBJ_RUBBER_DUCKY (169)
#define OBJ_URN_OF_ASHES (170)
#define OBJ_FUMAROLE (171)
#define OBJ_SPIKES (172)
#define OBJ_TRAP (173)
#define OBJ_SWITCH (174)
#define OBJ_ELECTRIC_FIELD (175)
#define OBJ_CHEST_OF_DRAWERS (176)
#define OBJ_DESK (177)
#define OBJ_BUCKET (178)
#define OBJ_BUCKET_OF_WATER (179)
#define OBJ_BUCKET_OF_MILK (180)
#define OBJ_CHURN (181)
#define OBJ_BEEHIVE (182)
#define OBJ_HONEY_JAR (183)
#define OBJ_JAR_OF_HONEY (184)
#define OBJ_CLOTH (185)
#define OBJ_BARREL (186)
#define OBJ_JUG (187)
#define OBJ_BAG (188)
#define OBJ_CASK (189)
#define OBJ_BALE_OF_WOOL (190)
#define OBJ_BASKET (191)
#define OBJ_CRATE (192)
#define OBJ_SMALL_JUG (193)
#define OBJ_MILK_BOTTLE (194)
#define OBJ_WHEAT (195)
#define OBJ_VAT (196)
#define OBJ_WINE_CASK (197)
#define OBJ_CUTTING_TABLE (198)
#define OBJ_LOOM (199)
#define OBJ_HOOD (200)
#define OBJ_FIRE (201)
#define OBJ_HORSESHOES (202)
#define OBJ_PLIERS (203)
#define OBJ_HAMMER (204)
#define OBJ_WATER_TROUGH (205)
#define OBJ_BRAZIER (206)
#define OBJ_ROD (207)
#define OBJ_HOOK (208)
#define OBJ_MEAT (209)
#define OBJ_RIBS (210)
#define OBJ_DEAD_ANIMAL (211)
#define OBJ_FAN (212)
#define OBJ_MOUSE_HOLE (213)
#define OBJ_WINE_PRESS (214)
#define OBJ_STABLE (215)
#define OBJ_BOOKSHELF (216)
#define OBJ_ANVIL (217)
#define OBJ_BELLOWS (218)
#define OBJ_OVEN (219)
#define OBJ_FLAG (220)
#define OBJ_CANNON (221)
#define OBJ_CANNON_BALLS (222)
#define OBJ_POWDER_KEG (223)
#define OBJ_FOOT_RAIL (224)
#define OBJ_SPOOL_OF_THREAD (225)
#define OBJ_SPOOL_OF_SILK (226)
#define OBJ_PENNANT (227)
#define OBJ_TABLE_SQUARE_TOP (228)
#define OBJ_SHADOW_TABLE_SQUARE_TOP (229)
#define OBJ_TABLE_CIRCLE_TOP (230)
#define OBJ_SHADOW_TABLE_CIRCLE_TOP (231)
#define OBJ_SPITTOON (232)
#define OBJ_WELL (233)
#define OBJ_FOUNTAIN (234)
#define OBJ_SUNDIAL (235)
#define OBJ_BELL (236)
#define OBJ_TABLE_CENTRE (237)
#define OBJ_SHADOW_TABLE_CENTRE (238)
#define OBJ_TABLE_ROUND (239)
#define OBJ_SHADOW_TABLE_ROUND (240)
#define OBJ_SILK_CLOTH (241)
#define OBJ_RUNE_OF_HONESTY (242)
#define OBJ_RUNE_OF_COMPASSION (243)
#define OBJ_RUNE_OF_VALOR (244)
#define OBJ_RUNE_OF_JUSTICE (245)
#define OBJ_RUNE_OF_SACRIFICE (246)
#define OBJ_RUNE_OF_HONOR (247)
#define OBJ_RUNE_OF_SPIRITUALITY (248)
#define OBJ_RUNE_OF_HUMILITY (249)
#define OBJ_TABLE_SQUARE_BOTTOM (250)
#define OBJ_SHADOW_TABLE_SQUARE_BOTTOM (251)
#define OBJ_CHAIR (252)
#define OBJ_CAMPFIRE (253)
#define OBJ_CROSS (254)
#define OBJ_TOMBSTONE (255)
#define OBJ_PROTECTION_RING (256)
#define OBJ_REGENERATION_RING (257)
#define OBJ_INVISIBILITY_RING (258)
#define OBJ_TABLE_LEG_LEFT (259)
#define OBJ_SHADOW_TABLE_BOTTOM (260)
#define OBJ_TABLE_LEG_RIGHT (261)
#define OBJ_SHADOW_TABLE_LEG_RIGHT (262)
#define OBJ_STOCKS (263)
#define OBJ_FISHING_POLE (264)
#define OBJ_FISH (265)
#define OBJ_GRAVE (266)
#define OBJ_GUILLOTINE (267)
#define OBJ_LEVER (268)
#define OBJ_DRAWBRIDGE (269)
#define OBJ_BALLOON_PLANS (270)
#define OBJ_DOORSILL (271)
#define OBJ_STEPS_ENDS (272)
#define OBJ_TILE (273)
#define OBJ_YEW_LOG (274)
#define OBJ_POTION (275)
#define OBJ_STEPS_MAIN (276)
#define OBJ_YEW_BOARD (277)
#define OBJ_PASSTHROUGH_V (278)
#define OBJ_TABLE (279)
#define OBJ_PASSTHROUGH_H (280)
#define OBJ_FENCE (281)
#define OBJ_BARS (282)
#define OBJ_ANCHOR (283)
#define OBJ_ROPE (284)
#define OBJ_POLE (285)
#define OBJ_WALKWAY (286)
#define OBJ_WATER_WHEEL (287)
#define OBJ_CRANK (288)
#define OBJ_LOG_SAW (289)
#define OBJ_MILL_STONE (290)
#define OBJ_SHAFT (291)
#define OBJ_GEARWORK (292)
#define OBJ_CHAIN (293)
#define OBJ_LIGHTSOURCE (294)
#define OBJ_HEATSOURCE (295)
#define OBJ_XYLOPHONE (296)
#define OBJ_OAKEN_DOOR (297)
#define OBJ_WINDOWED_DOOR (298)
#define OBJ_CEDAR_DOOR (299)
#define OBJ_STEEL_DOOR (300)
#define OBJ_DOORWAY (301)
#define OBJ_ARCHWAY (302)
#define OBJ_CARPET (303)
#define OBJ_COOKFIRE (304)
#define OBJ_LADDER (305)
#define OBJ_TRELLIS (306)
#define OBJ_VOLCANO (307)
#define OBJ_HOLE (308)
#define OBJ_BONES (309)
#define OBJ_PORTCULLIS (310)
#define OBJ_STONE_TABLE (311)
#define OBJ_STONE_LION (312)
#define OBJ_SILVER_HORN (313)
#define OBJ_FLOOR_SLAB (314)
#define OBJ_STONE (315)
#define OBJ_LAMPPOST (316)
#define OBJ_FIRE_FIELD (317)
#define OBJ_POISON_FIELD (318)
#define OBJ_PROTECTION_FIELD (319)
#define OBJ_SLEEP_FIELD (320)
#define OBJ_STATUE (321)
#define OBJ_POOL (322)
#define OBJ_MONOLITH (323)
#define OBJ_PILLAR (324)
#define OBJ_BOOK_STAND (325)
#define OBJ_MINE_SHAFT (326)
#define OBJ_THRONE (327)
#define OBJ_ALTAR (328)
#define OBJ_ALTAR_OF_SINGULARITY (329)
#define OBJ_MAT (330)
#define OBJ_GOVERNMENT_SIGN (331)
#define OBJ_SIGN (332)
#define OBJ_GARGOYLE_SIGN (333)
#define OBJ_SECRET_DOOR (334)
#define OBJ_EGG (335)
#define OBJ_CHARGE (336)
#define OBJ_EFFECT (337)
#define OBJ_BLOOD (338)
#define OBJ_DEAD_BODY (339)
#define OBJ_DEAD_CYCLOPS (340)
#define OBJ_DEAD_GARGOYLE (341)
#define OBJ_GIANT_RAT (342)
#define OBJ_INSECTS (343)
#define OBJ_GIANT_BAT (344)
#define OBJ_GIANT_SQUID (345)
#define OBJ_SEA_SERPENT (346)
#define OBJ_REAPER (347)
#define OBJ_SHEEP (348)
#define OBJ_DOG (349)
#define OBJ_DEER (350)
#define OBJ_WOLF (351)
#define OBJ_GHOST (352)
#define OBJ_GREMLIN (353)
#define OBJ_MOUSE (354)
#define OBJ_GAZER (355)
#define OBJ_BIRD (356)
#define OBJ_CORPSER (357)
#define OBJ_SNAKE (358)
#define OBJ_RABBIT (359)
#define OBJ_ROT_WORMS (360)
#define OBJ_GIANT_SPIDER (361)
#define OBJ_GARGOYLE_WINGED (362)
#define OBJ_GARGOYLE_WINGLESS (363)
#define OBJ_ACID_SLUG (364)
#define OBJ_TANGLE_VINE_POD (365)
#define OBJ_TANGLE_VINE (366)
#define OBJ_DAEMON (367)
#define OBJ_SKELETON (368)
#define OBJ_DRAKE (369)
#define OBJ_HEADLESS (370)
#define OBJ_TROLL (371)
#define OBJ_MONGBAT (372)
#define OBJ_WISP (373)
#define OBJ_HYDRA_MOUTH (374)
#define OBJ_SLIME (375)
#define OBJ_FIGHTER (376)
#define OBJ_SWASHBUCKLER (377)
#define OBJ_MAGE (378)
#define OBJ_VILLAGER (379)
#define OBJ_MERCHANT (380)
#define OBJ_CHILD (381)
#define OBJ_GUARD (382)
#define OBJ_JESTER (383)
#define OBJ_PEASANT (384)
#define OBJ_FARMER (385)
#define OBJ_MUSICIAN (386)
#define OBJ_WOMAN (387)
#define OBJ_CAT (388)
#define OBJ_SILVER_TABLET (389)
#define OBJ_SILVER_FRAGMENT (390)
#define OBJ_FARMER_HOE (391)
#define OBJ_MUSICIAN_LUTE (392)
#define OBJ_SHRINE (393)
#define OBJ_BRITANNIAN_LENS (394)
#define OBJ_BROKEN_LENS (395)
#define OBJ_GARGOYLE_LENS (396)
#define OBJ_STATUE_OF_MONDAIN (397)
#define OBJ_STATUE_OF_MINAX (398)
#define OBJ_STATUE_OF_EXODUS (399)
#define OBJ_PART_OF_A_MAP_0 (400)
#define OBJ_PART_OF_A_MAP_1 (401)
#define OBJ_PART_OF_A_MAP_2 (402)
#define OBJ_PART_OF_A_MAP_3 (403)
#define OBJ_PART_OF_A_MAP_4 (404)
#define OBJ_PART_OF_A_MAP_5 (405)
#define OBJ_PART_OF_A_MAP_6 (406)
#define OBJ_PART_OF_A_MAP_7 (407)
#define OBJ_PART_OF_A_MAP_8 (408)
#define OBJ_LORD_BRITISH (409)
#define OBJ_AVATAR (410)
#define OBJ_DRAGON (411)
#define OBJ_SHIP (412)
#define OBJ_SILVER_SERPENT (413)
#define OBJ_SKIFF (414)
#define OBJ_RAFT (415)
#define OBJ_NOTHING (416)
#define OBJ_DRAGON_EGG (417)
#define OBJ_HATCHED_DRAGON_EGG (418)
#define OBJ_PULL_CHAIN (419)
#define OBJ_BALLOON_DEFLATED (420)
#define OBJ_MAMMOTH_SILK_BAG (421)
#define OBJ_BALLOON_BASKET (422)
#define OBJ_BALLOON (423)
#define OBJ_CYCLOPS (424)
#define OBJ_HYDRA_ARMS (425)
#define OBJ_GIANT_SCORPION (426)
#define OBJ_GIANT_ANT (427)
#define OBJ_COW (428)
#define OBJ_ALLIGATOR (429)
#define OBJ_HORSE (430)
#define OBJ_HORSE_WITH_RIDER (431)
#define OBJ_DEAD_GIANT_RAT (432)
#define OBJ_DEAD_GIANT_BAT (433)
#define OBJ_DEAD_REAPER (434)
#define OBJ_DEAD_SHEEP (435)
#define OBJ_DEAD_DOG (436)
#define OBJ_DEAD_DEER (437)
#define OBJ_DEAD_WOLF (438)
#define OBJ_DEAD_GREMLIN (439)
#define OBJ_DEAD_MOUSE (440)
#define OBJ_DEAD_BIRD (441)
#define OBJ_DEAD_SNAKE (442)
#define OBJ_DEAD_RABBIT (443)
#define OBJ_DEAD_SPIDER (444)
#define OBJ_DEAD_DRAKE (445)
#define OBJ_DEAD_MONGBAT (446)
#define OBJ_DEAD_CAT (447)
#define OBJ_HORSE_PAPERS (448)
#define OBJ_SCORPION_EGG (449)
#define OBJ_MULTI_LENS_TELESCOPE (450)
#define OBJ_TMAP (451)
#define OBJ_POCKET_WATCH (452)
/*
const char* getObjectName(unsigned short type) {
    switch(type) {
        case OBJ_NOTHING_0: return "Nothing";
        case OBJ_LEATHER_HELM: return "Leather Helm";
        case OBJ_CHAIN_COIF: return "Chain Coif";
        case OBJ_IRON_HELM: return "Iron Helm";
        case OBJ_SPIKED_HELM: return "Spiked Helm";
        case OBJ_WINGED_HELM: return "Winged Helm";
        case OBJ_BRASS_HELM: return "Brass Helm";
        case OBJ_GARGOYLE_HELM: return "Gargoyle Helm";
        case OBJ_MAGIC_HELM: return "Magic Helm";
        case OBJ_WOODEN_SHIELD: return "Wooden Shield";
        case OBJ_CURVED_HEATER: return "Curved Heater";
        case OBJ_WINGED_SHIELD: return "Winged Shield";
        case OBJ_KITE_SHIELD: return "Kite Shield";
        case OBJ_SPIKED_SHIELD: return "Spiked Shield";
        case OBJ_BLACK_SHIELD: return "Black Shield";
        case OBJ_DOOR_SHIELD: return "Door Shield";
        case OBJ_MAGIC_SHIELD: return "Magic Shield";
        case OBJ_CLOTH_ARMOUR: return "Cloth Armour";
        case OBJ_LEATHER_ARMOR: return "Leather Armor";
        case OBJ_RING_MAIL: return "Ring Mail";
        case OBJ_SCALE_MAIL: return "Scale Mail";
        case OBJ_CHAIN_MAIL: return "Chain Mail";
        case OBJ_PLATE_MAIL: return "Plate Mail";
        case OBJ_MAGIC_ARMOUR: return "Magic Armour";
        case OBJ_SPIKED_COLLAR: return "Spiked Collar";
        case OBJ_GUILD_BELT: return "Guild Belt";
        case OBJ_GARGOYLE_BELT: return "Gargoyle Belt";
        case OBJ_LEATHER_BOOTS: return "Leather Boots";
        case OBJ_SWAMP_BOOTS: return "Swamp Boots";
        case OBJ_EARTH: return "Earth";
        case OBJ_FLOOR_WOOD: return "Wooden Floor";
        case OBJ_FLOOR_TILE: return "Tiled Floor";
        case OBJ_FLOOR_BLUE: return "Blue Floor";
        case OBJ_SLING: return "Sling";
        case OBJ_CLUB: return "Club";
        case OBJ_MAIN_GAUCHE: return "Main Gauche";
        case OBJ_SPEAR: return "Spear";
        case OBJ_THROWING_AXE: return "Throwing Axe";
        case OBJ_DAGGER: return "Dagger";
        case OBJ_MACE: return "Mace";
        case OBJ_MORNING_STAR: return "Morning Star";
        case OBJ_BOW: return "Bow";
        case OBJ_CROSSBOW: return "Crossbow";
        case OBJ_SWORD: return "Sword";
        case OBJ_TWO_HANDED_HAMMER: return "Two Handed Hammer";
        case OBJ_TWO_HANDED_AXE: return "Two Handed Axe";
        case OBJ_TWO_HANDED_SWORD: return "Two Handed Sword";
        case OBJ_HALBERD: return "Halberd";
        case OBJ_GLASS_SWORD: return "Glass Sword";
        case OBJ_BOOMERANG: return "Boomerang";
        case OBJ_TRIPLE_CROSSBOW: return "Triple Crossbow";
        case OBJ_FORCE_FIELD: return "Force Field";
        case OBJ_WIZARD_EYE: return "Wizard Eye";
        case OBJ_WEB: return "Web";
        case OBJ_MAGIC_BOW: return "Magic Bow";
        case OBJ_ARROW: return "Arrow";
        case OBJ_BOLT: return "Bolt";
        case OBJ_SPELLBOOK: return "Spellbook";
        case OBJ_SPELL: return "Spell";
        case OBJ_CODEX: return "Codex";
        case OBJ_BOOK_OF_PROPHECIES: return "Book of Prophecies";
        case OBJ_BOOK_OF_CIRCLES: return "Book of Circles";
        case OBJ_VORTEX_CUBE: return "Vortex Cube";
        case OBJ_LOCK_PICK: return "Lock Pick";
        case OBJ_KEY: return "Key";
        case OBJ_BLACK_PEARL: return "Black Pearl";
        case OBJ_BIT_OF_BLOOD_MOSS: return "Bit of Blood Moss";
        case OBJ_BULB_OF_GARLIC: return "Bulb of Garlic";
        case OBJ_GINSENG_ROOT: return "Ginseng Root";
        case OBJ_MANDRAKE_ROOT: return "Mandrake Root";
        case OBJ_NIGHTSHADE_MUSHROOM: return "Nightshade Mushroom";
        case OBJ_STRAND_OF_SPIDERSILK: return "Strand of Spidersilk";
        case OBJ_BIT_OF_SULFUROUS_ASH: return "Bit of Sulfurous Ash";
        case OBJ_MOONSTONE: return "Moonstone";
        case OBJ_ANKH_AMULET: return "Ankh Amulet";
        case OBJ_SNAKE_AMULET: return "Snake Amulet";
        case OBJ_AMULET_OF_SUBMISSION: return "Amulet of Submission";
        case OBJ_GEM: return "Gem";
        case OBJ_STAFF: return "Staff";
        case OBJ_LIGHTNING_WAND: return "Lightning Wand";
        case OBJ_FIRE_WAND: return "Fire Wand";
        case OBJ_STORM_CLOAK: return "Storm Cloak";
        case OBJ_RING: return "Ring";
        case OBJ_FLASK_OF_OIL: return "Flask of Oil";
        case OBJ_RED_GATE: return "Red Gate";
        case OBJ_MOONGATE: return "Moongate";
        case OBJ_GAVEL: return "Gavel";
        case OBJ_ORB_OF_THE_MOONS: return "Orb of the Moons";
        case OBJ_GOLD_COIN: return "Gold Coin";
        case OBJ_GOLD_NUGGET: return "Gold Nugget";
        case OBJ_TORCH: return "Torch";
        case OBJ_ZU_YLEM: return "Zu Ylem";
        case OBJ_SILVER_SNAKE_VENOM: return "Silver Snake Venom";
        case OBJ_SEXTANT: return "Sextant";
        case OBJ_SPINNING_WHEEL: return "Spinning Wheel";
        case OBJ_BUNCH_OF_GRAPES: return "Bunch of Grapes";
        case OBJ_BUTTER: return "Butter";
        case OBJ_GARGISH_VOCABULARY: return "Gargish Vocabulary";
        case OBJ_CHEST: return "Chest";
        case OBJ_BACKPACK: return "Backpack";
        case OBJ_SCYTHE: return "Scythe";
        case OBJ_PITCHFORK: return "Pitchfork";
        case OBJ_RAKE: return "Rake";
        case OBJ_PICK: return "Pick";
        case OBJ_SHOVEL: return "Shovel";
        case OBJ_HOE: return "Hoe";
        case OBJ_WOODEN_LADDER: return "Wooden Ladder";
        case OBJ_YOKE: return "Yoke";
        case OBJ_OVEN_SPATULA: return "Oven Spatula";
        case OBJ_ROLLING_PIN: return "Rolling Pin";
        case OBJ_SPATULA: return "Spatula";
        case OBJ_LADLE: return "Ladle";
        case OBJ_COOKING_SHEET: return "Cooking Sheet";
        case OBJ_CLEAVER: return "Cleaver";
        case OBJ_KNIFE: return "Knife";
        case OBJ_WINE: return "Wine";
        case OBJ_MEAD: return "Mead";
        case OBJ_ALE: return "Ale";
        case OBJ_WINE_GLASS: return "Wine Glass";
        case OBJ_PLATE: return "Plate";
        case OBJ_MUG: return "Mug";
        case OBJ_SILVERWARE: return "Silverware";
        case OBJ_CANDLE: return "Candle";
        case OBJ_MIRROR: return "Mirror";
        case OBJ_TUNIC: return "Tunic";
        case OBJ_HANGER: return "Hanger";
        case OBJ_DRESS: return "Dress";
        case OBJ_SKILLET: return "Skillet";
        case OBJ_LOAF_OF_BREAD: return "Loaf of Bread";
        case OBJ_PORTION_OF_MEAT: return "Portion of Meat";
        case OBJ_ROLLS: return "Rolls";
        case OBJ_CAKE: return "Cake";
        case OBJ_CHEESE: return "Cheese";
        case OBJ_HAM: return "Ham";
        case OBJ_HORSE_CARCASS: return "Horse Carcass";
        case OBJ_HORSE_CHOPS: return "Horse Chops";
        case OBJ_SKEWER: return "Skewer";
        case OBJ_PANTS: return "Pants";
        case OBJ_PLANT: return "Plant";
        case OBJ_FLOWERS: return "Flowers";
        case OBJ_WALL_MOUNT: return "Wall Mount";
        case OBJ_DECORATIVE_SWORD: return "Decorative Sword";
        case OBJ_DECORATIVE_SHIELD: return "Decorative Shield";
        case OBJ_PICTURE: return "Picture";
        case OBJ_TAPESTRY: return "Tapestry";
        case OBJ_CANDELABRA: return "Candelabra";
        case OBJ_PERSON_SLEEPING: return "Person Sleeping";
        case OBJ_CAULDRON: return "Cauldron";
        case OBJ_CAULDRON_FULL: return "Cauldron Full";
        case OBJ_SHIP_DEED: return "Ship Deed";
        case OBJ_INKWELL: return "Inkwell";
        case OBJ_BOOK: return "Book";
        case OBJ_SCROLL: return "Scroll";
        case OBJ_PANPIPES: return "Panpipes";
        case OBJ_TELESCOPE: return "Telescope";
        case OBJ_CRYSTAL_BALL: return "Crystal Ball";
        case OBJ_HARPSICHORD: return "Harpsichord";
        case OBJ_HARP: return "Harp";
        case OBJ_LUTE: return "Lute";
        case OBJ_CLOCK: return "Clock";
        case OBJ_ENDTABLE: return "Endtable";
        case OBJ_WATER_VASE: return "Water Vase";
        case OBJ_STOVE: return "Stove";
        case OBJ_BED: return "Bed";
        case OBJ_FIREPLACE: return "Fireplace";
        case OBJ_STALAGMITE: return "Stalagmite";
        case OBJ_SACK_OF_GRAIN: return "Sack of Grain";
        case OBJ_SACK_OF_FLOUR: return "Sack of Flour";
        case OBJ_REMAINS: return "Remains";
        case OBJ_RUBBER_DUCKY: return "Rubber Ducky";
        case OBJ_URN_OF_ASHES: return "Urn of Ashes";
        case OBJ_FUMAROLE: return "Fumarole";
        case OBJ_SPIKES: return "Spikes";
        case OBJ_TRAP: return "Trap";
        case OBJ_SWITCH: return "Switch";
        case OBJ_ELECTRIC_FIELD: return "Electric Field";
        case OBJ_CHEST_OF_DRAWERS: return "Chest of Drawers";
        case OBJ_DESK: return "Desk";
        case OBJ_BUCKET: return "Bucket";
        case OBJ_BUCKET_OF_WATER: return "Bucket of Water";
        case OBJ_BUCKET_OF_MILK: return "Bucket of Milk";
        case OBJ_CHURN: return "Churn";
        case OBJ_BEEHIVE: return "Beehive";
        case OBJ_HONEY_JAR: return "Honey Jar";
        case OBJ_JAR_OF_HONEY: return "Jar of Honey";
        case OBJ_CLOTH: return "Cloth";
        case OBJ_BARREL: return "Barrel";
        case OBJ_JUG: return "Jug";
        case OBJ_BAG: return "Bag";
        case OBJ_CASK: return "Cask";
        case OBJ_BALE_OF_WOOL: return "Bale of Wool";
        case OBJ_BASKET: return "Basket";
        case OBJ_CRATE: return "Crate";
        case OBJ_SMALL_JUG: return "Small Jug";
        case OBJ_MILK_BOTTLE: return "Milk Bottle";
        case OBJ_WHEAT: return "Wheat";
        case OBJ_VAT: return "Vat";
        case OBJ_WINE_CASK: return "Wine Cask";
        case OBJ_CUTTING_TABLE: return "Cutting Table";
        case OBJ_LOOM: return "Loom";
        case OBJ_HOOD: return "Hood";
        case OBJ_FIRE: return "Fire";
        case OBJ_HORSESHOES: return "Horseshoes";
        case OBJ_PLIERS: return "Pliers";
        case OBJ_HAMMER: return "Hammer";
        case OBJ_WATER_TROUGH: return "Water Trough";
        case OBJ_BRAZIER: return "Brazier";
        case OBJ_ROD: return "Rod";
        case OBJ_HOOK: return "Hook";
        case OBJ_MEAT: return "Meat";
        case OBJ_RIBS: return "Ribs";
        case OBJ_DEAD_ANIMAL: return "Dead Animal";
        case OBJ_FAN: return "Fan";
        case OBJ_MOUSE_HOLE: return "Mouse Hole";
        case OBJ_WINE_PRESS: return "Wine Press";
        case OBJ_STABLE: return "Stable";
        case OBJ_BOOKSHELF: return "Bookshelf";
        case OBJ_ANVIL: return "Anvil";
        case OBJ_BELLOWS: return "Bellows";
        case OBJ_OVEN: return "Oven";
        case OBJ_FLAG: return "Flag";
        case OBJ_CANNON: return "Cannon";
        case OBJ_CANNON_BALLS: return "Cannon Balls";
        case OBJ_POWDER_KEG: return "Powder Keg";
        case OBJ_FOOT_RAIL: return "Foot Rail";
        case OBJ_SPOOL_OF_THREAD: return "Spool of Thread";
        case OBJ_SPOOL_OF_SILK: return "Spool of Silk";
        case OBJ_PENNANT: return "Pennant";
        case OBJ_TABLE_SQUARE_TOP: return "Square Table Top";
        case OBJ_SHADOW_TABLE_SQUARE_TOP: return "Shadow Table Square Top";
        case OBJ_TABLE_CIRCLE_TOP: return "Circle Table Top";
        case OBJ_SHADOW_TABLE_CIRCLE_TOP: return "Shadow Table Circle Top";
        case OBJ_SPITTOON: return "Spittoon";
        case OBJ_WELL: return "Well";
        case OBJ_FOUNTAIN: return "Fountain";
        case OBJ_SUNDIAL: return "Sundial";
        case OBJ_BELL: return "Bell";
        case OBJ_TABLE_CENTRE: return "Centre Table";
        case OBJ_SHADOW_TABLE_CENTRE: return "Shadow Table Centre";
        case OBJ_TABLE_ROUND: return "Round Table";
        case OBJ_SHADOW_TABLE_ROUND: return "Shadow Table Round";
        case OBJ_SILK_CLOTH: return "Silk Cloth";
        case OBJ_RUNE_OF_HONESTY: return "Rune of Honesty";
        case OBJ_RUNE_OF_COMPASSION: return "Rune of Compassion";
        case OBJ_RUNE_OF_VALOR: return "Rune of Valor";
        case OBJ_RUNE_OF_JUSTICE: return "Rune of Justice";
        case OBJ_RUNE_OF_SACRIFICE: return "Rune of Sacrifice";
        case OBJ_RUNE_OF_HONOR: return "Rune of Honor";
        case OBJ_RUNE_OF_SPIRITUALITY: return "Rune of Spirituality";
        case OBJ_RUNE_OF_HUMILITY: return "Rune of Humility";
        case OBJ_TABLE_SQUARE_BOTTOM: return "Square Table Bottom";
        case OBJ_SHADOW_TABLE_SQUARE_BOTTOM: return "Shadow Table Square Bottom";
        case OBJ_CHAIR: return "Chair";
        case OBJ_CAMPFIRE: return "Campfire";
        case OBJ_CROSS: return "Cross";
        case OBJ_TOMBSTONE: return "Tombstone";
        case OBJ_PROTECTION_RING: return "Protection Ring";
        case OBJ_REGENERATION_RING: return "Regeneration Ring";
        case OBJ_INVISIBILITY_RING: return "Invisibility Ring";
        case OBJ_TABLE_LEG_LEFT: return "Table Leg Left";
        case OBJ_SHADOW_TABLE_BOTTOM: return "Shadow Table Bottom";
        case OBJ_TABLE_LEG_RIGHT: return "Table Leg Right";
        case OBJ_SHADOW_TABLE_LEG_RIGHT: return "Shadow Table Leg Right";
        case OBJ_STOCKS: return "Stocks";
        case OBJ_FISHING_POLE: return "Fishing Pole";
        case OBJ_FISH: return "Fish";
        case OBJ_GRAVE: return "Grave";
        case OBJ_GUILLOTINE: return "Guillotine";
        case OBJ_LEVER: return "Lever";
        case OBJ_DRAWBRIDGE: return "Drawbridge";
        case OBJ_BALLOON_PLANS: return "Balloon Plans";
        case OBJ_DOORSILL: return "Doorsill";
        case OBJ_STEPS_ENDS: return "Steps Ends";
        case OBJ_TILE: return "Tile";
        case OBJ_YEW_LOG: return "Yew Log";
        case OBJ_POTION: return "Potion";
        case OBJ_STEPS_MAIN: return "Steps Main";
        case OBJ_YEW_BOARD: return "Yew Board";
        case OBJ_PASSTHROUGH_V: return "Passthrough V";
        case OBJ_TABLE: return "Table";
        case OBJ_PASSTHROUGH_H: return "Passthrough H";
        case OBJ_FENCE: return "Fence";
        case OBJ_BARS: return "Bars";
        case OBJ_ANCHOR: return "Anchor";
        case OBJ_ROPE: return "Rope";
        case OBJ_POLE: return "Pole";
        case OBJ_WALKWAY: return "Walkway";
        case OBJ_WATER_WHEEL: return "Water Wheel";
        case OBJ_CRANK: return "Crank";
        case OBJ_LOG_SAW: return "Log Saw";
        case OBJ_MILL_STONE: return "Mill Stone";
        case OBJ_SHAFT: return "Shaft";
        case OBJ_GEARWORK: return "Gearwork";
        case OBJ_CHAIN: return "Chain";
        case OBJ_LIGHTSOURCE: return "Lightsource";
        case OBJ_HEATSOURCE: return "Heatsource";
        case OBJ_XYLOPHONE: return "Xylophone";
        case OBJ_OAKEN_DOOR: return "Oaken Door";
        case OBJ_WINDOWED_DOOR: return "Windowed Door";
        case OBJ_CEDAR_DOOR: return "Cedar Door";
        case OBJ_STEEL_DOOR: return "Steel Door";
        case OBJ_DOORWAY: return "Doorway";
        case OBJ_ARCHWAY: return "Archway";
        case OBJ_CARPET: return "Carpet";
        case OBJ_COOKFIRE: return "Cookfire";
        case OBJ_LADDER: return "Ladder";
        case OBJ_TRELLIS: return "Trellis";
        case OBJ_VOLCANO: return "Volcano";
        case OBJ_HOLE: return "Hole";
        case OBJ_BONES: return "Bones";
        case OBJ_PORTCULLIS: return "Portcullis";
        case OBJ_STONE_TABLE: return "Stone Table";
        case OBJ_STONE_LION: return "Stone Lion";
        case OBJ_SILVER_HORN: return "Silver Horn";
        case OBJ_FLOOR_SLAB: return "Floor Slab";
        case OBJ_STONE: return "Stone";
        case OBJ_LAMPPOST: return "Lamppost";
        case OBJ_FIRE_FIELD: return "Fire Field";
        case OBJ_POISON_FIELD: return "Poison Field";
        case OBJ_PROTECTION_FIELD: return "Protection Field";
        case OBJ_SLEEP_FIELD: return "Sleep Field";
        case OBJ_STATUE: return "Statue";
        case OBJ_POOL: return "Pool";
        case OBJ_MONOLITH: return "Monolith";
        case OBJ_PILLAR: return "Pillar";
        case OBJ_BOOK_STAND: return "Book Stand";
        case OBJ_MINE_SHAFT: return "Mine Shaft";
        case OBJ_THRONE: return "Throne";
        case OBJ_ALTAR: return "Altar";
        case OBJ_ALTAR_OF_SINGULARITY: return "Altar of Singularity";
        case OBJ_MAT: return "Mat";
        case OBJ_GOVERNMENT_SIGN: return "Government Sign";
        case OBJ_SIGN: return "Sign";
        case OBJ_GARGOYLE_SIGN: return "Gargoyle Sign";
        case OBJ_SECRET_DOOR: return "Secret Door";
        case OBJ_EGG: return "Egg";
        case OBJ_CHARGE: return "Charge";
        case OBJ_EFFECT: return "Effect";
        case OBJ_BLOOD: return "Blood";
        case OBJ_DEAD_BODY: return "Dead Body";
        case OBJ_DEAD_CYCLOPS: return "Dead Cyclops";
        case OBJ_DEAD_GARGOYLE: return "Dead Gargoyle";
        case OBJ_GIANT_RAT: return "Giant Rat";
        case OBJ_INSECTS: return "Insects";
        case OBJ_GIANT_BAT: return "Giant Bat";
        case OBJ_GIANT_SQUID: return "Giant Squid";
        case OBJ_SEA_SERPENT: return "Sea Serpent";
        case OBJ_REAPER: return "Reaper";
        case OBJ_SHEEP: return "Sheep";
        case OBJ_DOG: return "Dog";
        case OBJ_DEER: return "Deer";
        case OBJ_WOLF: return "Wolf";
        case OBJ_GHOST: return "Ghost";
        case OBJ_GREMLIN: return "Gremlin";
        case OBJ_MOUSE: return "Mouse";
        case OBJ_GAZER: return "Gazer";
        case OBJ_BIRD: return "Bird";
        case OBJ_CORPSER: return "Corpser";
        case OBJ_SNAKE: return "Snake";
        case OBJ_RABBIT: return "Rabbit";
        case OBJ_ROT_WORMS: return "Rot Worms";
        case OBJ_GIANT_SPIDER: return "Giant Spider";
        case OBJ_GARGOYLE_WINGED: return "Gargoyle Winged";
        case OBJ_GARGOYLE_WINGLESS: return "Gargoyle Wingless";
        case OBJ_ACID_SLUG: return "Acid Slug";
        case OBJ_TANGLE_VINE_POD: return "Tangle Vine Pod";
        case OBJ_TANGLE_VINE: return "Tangle Vine";
        case OBJ_DAEMON: return "Daemon";
        case OBJ_SKELETON: return "Skeleton";
        case OBJ_DRAKE: return "Drake";
        case OBJ_HEADLESS: return "Headless";
        case OBJ_TROLL: return "Troll";
        case OBJ_MONGBAT: return "Mongbat";
        case OBJ_WISP: return "Wisp";
        case OBJ_HYDRA_MOUTH: return "Hydra Mouth";
        case OBJ_SLIME: return "Slime";
        case OBJ_FIGHTER: return "Fighter";
        case OBJ_SWASHBUCKLER: return "Swashbuckler";
        case OBJ_MAGE: return "Mage";
        case OBJ_VILLAGER: return "Villager";
        case OBJ_MERCHANT: return "Merchant";
        case OBJ_CHILD: return "Child";
        case OBJ_GUARD: return "Guard";
        case OBJ_JESTER: return "Jester";
        case OBJ_PEASANT: return "Peasant";
        case OBJ_FARMER: return "Farmer";
        case OBJ_MUSICIAN: return "Musician";
        case OBJ_WOMAN: return "Woman";
        case OBJ_CAT: return "Cat";
        case OBJ_SILVER_TABLET: return "Silver Tablet";
        case OBJ_SILVER_FRAGMENT: return "Silver Fragment";
        case OBJ_FARMER_HOE: return "Farmer Hoe";
        case OBJ_MUSICIAN_LUTE: return "Musician Lute";
        case OBJ_SHRINE: return "Shrine";
        case OBJ_BRITANNIAN_LENS: return "Britannian Lens";
        case OBJ_BROKEN_LENS: return "Broken Lens";
        case OBJ_GARGOYLE_LENS: return "Gargoyle Lens";
        case OBJ_STATUE_OF_MONDAIN: return "Statue of Mondain";
        case OBJ_STATUE_OF_MINAX: return "Statue of Minax";
        case OBJ_STATUE_OF_EXODUS: return "Statue of Exodus";
        case OBJ_PART_OF_A_MAP_0: return "Part of a Map 0";
        case OBJ_PART_OF_A_MAP_1: return "Part of a Map 1";
        case OBJ_PART_OF_A_MAP_2: return "Part of a Map 2";
        case OBJ_PART_OF_A_MAP_3: return "Part of a Map 3";
        case OBJ_PART_OF_A_MAP_4: return "Part of a Map 4";
        case OBJ_PART_OF_A_MAP_5: return "Part of a Map 5";
        case OBJ_PART_OF_A_MAP_6: return "Part of a Map 6";
        case OBJ_PART_OF_A_MAP_7: return "Part of a Map 7";
        case OBJ_PART_OF_A_MAP_8: return "Part of a Map 8";
        case OBJ_LORD_BRITISH: return "Lord British";
        case OBJ_AVATAR: return "Avatar";
        case OBJ_DRAGON: return "Dragon";
        case OBJ_SHIP: return "Ship";
        case OBJ_SILVER_SERPENT: return "Silver Serpent";
        case OBJ_SKIFF: return "Skiff";
        case OBJ_RAFT: return "Raft";
        case OBJ_NOTHING: return "Nothing";
        case OBJ_DRAGON_EGG: return "Dragon Egg";
        case OBJ_HATCHED_DRAGON_EGG: return "Hatched Dragon Egg";
        case OBJ_PULL_CHAIN: return "Pull Chain";
        case OBJ_BALLOON_DEFLATED: return "Balloon Deflated";
        case OBJ_MAMMOTH_SILK_BAG: return "Mammoth Silk Bag";
        case OBJ_BALLOON_BASKET: return "Balloon Basket";
        case OBJ_BALLOON: return "Balloon";
        case OBJ_CYCLOPS: return "Cyclops";
        case OBJ_HYDRA_ARMS: return "Hydra Arms";
        case OBJ_GIANT_SCORPION: return "Giant Scorpion";
        case OBJ_GIANT_ANT: return "Giant Ant";
        case OBJ_COW: return "Cow";
        case OBJ_ALLIGATOR: return "Alligator";
        case OBJ_HORSE: return "Horse";
        case OBJ_HORSE_WITH_RIDER: return "Horse with Rider";
        case OBJ_DEAD_GIANT_RAT: return "Dead Giant Rat";
        case OBJ_DEAD_GIANT_BAT: return "Dead Giant Bat";
        case OBJ_DEAD_REAPER: return "Dead Reaper";
        case OBJ_DEAD_SHEEP: return "Dead Sheep";
        case OBJ_DEAD_DOG: return "Dead Dog";
        case OBJ_DEAD_DEER: return "Dead Deer";
        case OBJ_DEAD_WOLF: return "Dead Wolf";
        case OBJ_DEAD_GREMLIN: return "Dead Gremlin";
        case OBJ_DEAD_MOUSE: return "Dead Mouse";
        case OBJ_DEAD_BIRD: return "Dead Bird";
        case OBJ_DEAD_SNAKE: return "Dead Snake";
        case OBJ_DEAD_RABBIT: return "Dead Rabbit";
        case OBJ_DEAD_SPIDER: return "Dead Spider";
        case OBJ_DEAD_DRAKE: return "Dead Drake";
        case OBJ_DEAD_MONGBAT: return "Dead Mongbat";
        case OBJ_DEAD_CAT: return "Dead Cat";
        case OBJ_HORSE_PAPERS: return "Horse Papers";
        case OBJ_SCORPION_EGG: return "Scorpion Egg";
        case OBJ_MULTI_LENS_TELESCOPE: return "Multi Lens Telescope";
        case OBJ_TMAP: return "Tmap";
        case OBJ_POCKET_WATCH: return "Pocket Watch";
        default: return "Unknown";
    }
}*/

// r666 (obsolete; r999 *mostly* replaced this)
/*
#define ACTIONBAR_MAX 2
#define ACTIONTALKBAR_MAX 2
#define ACTIONBUTTON_MAX 5
#define ACTIONTALKBUTTON_MAX 10
#define ACTIONBAR_HEIGHT 52

#define ACTIONBUTTON_ATTACK 0
#define ACTIONBUTTON_TALK 1
#define ACTIONBUTTON_LOOK 2
#define ACTIONBUTTON_USE 3
#define ACTIONBUTTON_DROP 4

#define ACTIONBUTTON_FOOD 0
#define ACTIONBUTTON_MINIMAP 3
#define ACTIONBUTTON_QUIT 4

#define ACTIONTALKBUTTON_NAME 0
#define ACTIONTALKBUTTON_JOB 1
#define ACTIONTALKBUTTON_BUY 2
#define ACTIONTALKBUTTON_SELL 3
#define ACTIONTALKBUTTON_YES 4
#define ACTIONTALKBUTTON_NO 5
#define ACTIONTALKBUTTON_BULK 6
#define ACTIONTALKBUTTON_SPELL 7
#define ACTIONTALKBUTTON_REAGENT 8
#define ACTIONTALKBUTTON_POTION 9

#define ACTIONTALKBUTTON_DEPOSIT 0
#define ACTIONTALKBUTTON_WITHDRAW 1
#define ACTIONTALKBUTTON_BALANCE 2
#define ACTIONTALKBUTTON_EXCHANGE 3
#define ACTIONTALKBUTTON_HOUSE 4
#define ACTIONTALKBUTTON_PAYMENT 5
#define ACTIONTALKBUTTON_HEAL 6
#define ACTIONTALKBUTTON_CURE 7
#define ACTIONTALKBUTTON_RESURRECT 8
#define ACTIONTALKBUTTON_HELP 9
*/

// r777 mx and my locations (mostly for 1024 res)
// DO NOT USE.  These locations are not always correct.
/*
#define MX_TILE 30 // tile size in pixel (1024 res)
#define MY_TILE 30 // tile size in pixel (1024 res)
#define MX_AVATAR 495 // 480 to 510
#define MY_AVATAR 365 // 350 to 380
#define MX_AVATAR_TOP MX_AVATAR
#define MY_AVATAR_TOP MY_AVATAR - MY_TILE
#define MX_AVATAR_BOTTOM MX_AVATAR
#define MY_AVATAR_BOTTOM MY_AVATAR + MY_TILE
#define MX_AVATAR_LEFT MX_AVATAR - MY_TILE
#define MY_AVATAR_LEFT MY_AVATAR
#define MX_AVATAR_RIGHT MX_AVATAR + MY_TILE
#define MY_AVATAR_RIGHT MY_AVATAR

#define POS_AVATAR_MX 476 // 480 to 510
#define POS_AVATAR_MY MY_AVATAR // 350 to 380
#define POS_AVATAR_MX1 508 // 480 to 510
#define POS_AVATAR_MY1 350 // 350 to 380
#define POS_AVATAR_MX2 510 // 480 to 510
#define POS_AVATAR_MY2 386 // 350 to 380
*/


// r999
//#define PANEL_MAX 10 // obsolete

// r999 new
#define UI_PANEL_MAX 20
#define UI_PANELWIDGET_MAX 10+1							// +1 because widget[0] is the panel itself
#define UI_WIDGETSTATE_MAX 6

#define UI_PANEL_IMG_MAX UI_PANEL_MAX
#define UI_PANELWIDGET_IMG_MAX 30						// increase this when adding more widget (images)
#define UI_WIDGETSTATE_IMG_MAX UI_WIDGETSTATE_MAX

#define UI_IMGI_ACTIONBAR 0								// starting img index for actionbar
//#define UI_IMGI_OPTIONBAR 5
#define UI_IMGI_ACTIONTALKBAR UI_IMGI_ACTIONBAR+10		// starting img index for actiontalkbar
#define UI_IMGI_WIDGET 0
#define UI_IMGI_HOVER UI_IMGI_WIDGET+13					// this must match what is in client_bmp.inc


#define UI_NOSCALE -1.0f
#define UI_WIDGET_DEF 0 // index of widgets
#define UI_STATE_DEF 0  // index of widget state

//#define ACTIONBAR_MAX 2
//#define ACTIONTALKBAR_MAX 2
#define UI_WIDGET_ACTIONBUTTON_MAX 5+1				// +1 because widget[0] is the panel itself
#define UI_WIDGET_ACTIONTALKBUTTON_MAX 10+1			// +1 because widget[0] is the panel itself
//#define UI_ACTIONBUTTON_SIZEX 51
//#define UI_ACTIONBUTTON_SIZEY 51

#define UI_SIDEBAR_COLOR 0x00001372 // converted from ((2 << 11) + (27 << 5) + 18) in function_client

#define UI_WIDGET_ACTIONBUTTON_ATTACK 1
#define UI_WIDGET_ACTIONBUTTON_TALK 2
#define UI_WIDGET_ACTIONBUTTON_LOOK 3
#define UI_WIDGET_ACTIONBUTTON_USE 4
#define UI_WIDGET_ACTIONBUTTON_DROP 5

#define UI_WIDGET_ACTIONBUTTON_FOOD 1
#define UI_WIDGET_ACTIONBUTTON_WORLDMAP 4
#define UI_WIDGET_ACTIONBUTTON_COMBATLOG 5

#define UI_WIDGET_OPTIONBUTTON_ENHANCE 1
#define UI_WIDGET_OPTIONBUTTON_SOUND 2
#define UI_WIDGET_OPTIONBUTTON_COMBATSOUND 3
#define UI_WIDGET_OPTIONBUTTON_MINIMAP 4
#define UI_WIDGET_OPTIONBUTTON_QUIT 5

#define UI_WIDGET_MAPBUTTON_U6CLOTH 1
#define UI_WIDGET_MAPBUTTON_U6P 2
#define UI_WIDGET_MAPBUTTON_U6G 3
#define UI_WIDGET_MAPBUTTON_U6RUNE 4
#define UI_WIDGET_MAPBUTTON_CLOSE 5

#define UI_WIDGET_ACTIONTALKBUTTON_NAME 1
#define UI_WIDGET_ACTIONTALKBUTTON_JOB 2
#define UI_WIDGET_ACTIONTALKBUTTON_HEAL 3
#define UI_WIDGET_ACTIONTALKBUTTON_CURE 4
#define UI_WIDGET_ACTIONTALKBUTTON_RESURRECT 5
#define UI_WIDGET_ACTIONTALKBUTTON_INSURANCE 6
#define UI_WIDGET_ACTIONTALKBUTTON_SPELL 7
#define UI_WIDGET_ACTIONTALKBUTTON_REAGENT 8
#define UI_WIDGET_ACTIONTALKBUTTON_POTION 9
#define UI_WIDGET_ACTIONTALKBUTTON_STAFF 10

#define UI_WIDGET_ACTIONTALKBUTTON_YES 1
#define UI_WIDGET_ACTIONTALKBUTTON_NO 2
#define UI_WIDGET_ACTIONTALKBUTTON_SELL 3
#define UI_WIDGET_ACTIONTALKBUTTON_BULK 4
#define UI_WIDGET_ACTIONTALKBUTTON_BUY 5
#define UI_WIDGET_ACTIONTALKBUTTON_ARMOR 6
#define UI_WIDGET_ACTIONTALKBUTTON_ARMS 7
#define UI_WIDGET_ACTIONTALKBUTTON_WEAPON 8
#define UI_WIDGET_ACTIONTALKBUTTON_ARROWS 9
#define UI_WIDGET_ACTIONTALKBUTTON_BOLTS 10

#define UI_WIDGET_ACTIONTALKBUTTON_DEPOSIT 1
#define UI_WIDGET_ACTIONTALKBUTTON_WITHDRAW 2
#define UI_WIDGET_ACTIONTALKBUTTON_BALANCE 3
#define UI_WIDGET_ACTIONTALKBUTTON_EXCHANGE 4
#define UI_WIDGET_ACTIONTALKBUTTON_HOUSE 5
#define UI_WIDGET_ACTIONTALKBUTTON_PAYMENT 6
#define UI_WIDGET_ACTIONTALKBUTTON_JOIN 7
#define UI_WIDGET_ACTIONTALKBUTTON_LEAVE 8
#define UI_WIDGET_ACTIONTALKBUTTON_BAGS 9
#define UI_WIDGET_ACTIONTALKBUTTON_HELP 10



// s222
#define SOUND_LEVER 6
#define SOUND_DOOROPEN 1
#define SOUND_DOORCLOSE 2
#define SOUND_FIELD 5

#define SOUND_COMBAT_HIT 0
#define SOUND_COMBAT_BOW 7
#define SOUND_COMBAT_CROSSBOW 8
#define SOUND_COMBAT_FIREBALL 20

#define SOUND_LEVERN1 45
#define SOUND_DOOROPENN1 47
#define SOUND_DOORCLOSEN1 48
#define SOUND_DOOROPENN2 54
#define SOUND_DOORCLOSEN2 55
#define SOUND_FIELDN1 61

#define SOUND_STATUSMESSAGE 59 // 49
#define SOUND_STEALINGWARNING 49
#define SOUND_STOLENITEM 62
#define SOUND_STATUSMESSAGELOWPRIORITY 51
#define SOUND_USERACTION 50
#define SOUND_USERACTIONDENIED 46
#define SOUND_UIACTION 50
#define SOUND_EAT 56
#define SOUND_DRINK 57
#define SOUND_DROP 58
#define SOUND_NOTIFY 59
#define SOUND_PLAYERENTERED 60

#define SOUND_COMBAT_BOWN1 44
#define SOUND_COMBAT_CROSSBOWN1 53
#define SOUND_COMBAT_FIREBALLN1 39
#define SOUND_COMBAT_MISS1N1 36
#define SOUND_COMBAT_MISS2N1 37
#define SOUND_COMBAT_PLAYERHURTN1 38
#define SOUND_COMBAT_HITN1 52

#define SOUND_MAGIC_READY 41
#define SOUND_MAGIC_NOTREADY 46
#define SOUND_MAGIC_FREEZE 42
#define SOUND_MAGIC_UNFREEZE 43
#define SOUND_MAGIC_FAILED1 62
#define SOUND_MAGIC_FAILED2 63
#define SOUND_MAGIC_FAILED3 64
#define SOUND_MAGIC_SUCCESS 65


// b111
#define WALL_NORTH 1
#define WALL_SOUTH 2
#define WALL_WEST 3
#define WALL_EAST 4

#define BUILD_TYPE_HORIZONTAL 10
#define BUILD_TYPE_VERTICAL 20
#define BUILD_TYPE_NORTH 1
#define BUILD_TYPE_SOUTH 2
#define BUILD_TYPE_WEST 3
#define BUILD_TYPE_EAST 4

#define BUILD_TYPE_H BUILD_TYPE_HORIZONTAL
#define BUILD_TYPE_V BUILD_TYPE_VERTICAL
#define BUILD_TYPE_N BUILD_TYPE_NORTH
#define BUILD_TYPE_S BUILD_TYPE_SOUTH
#define BUILD_TYPE_W BUILD_TYPE_WEST
#define BUILD_TYPE_E BUILD_TYPE_EAST

#define BUILD_WALLSTYLE_STONE1 1110
#define BUILD_WALLSTYLE_STONE2 1111

#define BUILD_FLOORSTYLE_BLUE1 2110
#define BUILD_FLOORSTYLE_BLUE2 2111

#define BUILD_SKIPSIDE_NONE 0
#define BUILD_SKIPSIDE_NORTH 1
#define BUILD_SKIPSIDE_SOUTH 2
#define BUILD_SKIPSIDE_WEST 4
#define BUILD_SKIPSIDE_EAST 8
#define BUILD_SKIPSIDE_ENDPOINTS 16


#define BUILD_TILEOPTION_D 1
#define BUILD_TILEOPTION_N 11
#define BUILD_TILEOPTION_S 12
#define BUILD_TILEOPTION_W 13
#define BUILD_TILEOPTION_E 14
#define BUILD_TILEOPTION_NW 21
#define BUILD_TILEOPTION_NE 22
#define BUILD_TILEOPTION_SW 23
#define BUILD_TILEOPTION_SE 24
#define BUILD_TILEOPTION_H 31
#define BUILD_TILEOPTION_V 32

#define BUILD_TABLEWITHSTORAGE_NO 0
#define BUILD_TABLEWITHSTORAGE_YES 1


#define BUILD_HOUSEID_MAX 50
#define BUILD_SECTIONID_MAX 50
#define BUILD_OBJPOINTER_MAX 10
#define BUILD_ARENAID_MAX 10

#define BUILD_HOUSEID_MANOR1 21
#define BUILD_HOUSEID_MANOR2 22
#define BUILD_SECTIONID_BASE 0
#define BUILD_SECTIONID_WALL 1
#define BUILD_SECTIONID_OBJ 11
#define BUILD_SECTIONID_STORAGE1 21

#define SPELL_LOCATE_CODE static_cast<long>(53)
#define SPELL_DISPEL_FIELD_CODE 33
#define SPELL_POISON_FIELD_CODE 55
#define SPELL_ENERGY_FIELD_CODE 64
#define SPELL_FIRE_FIELD_CODE 51
#define SPELL_CREATE_FOOD_CODE 0
#define SPELL_UNTRAP_CODE 24
#define SPELL_GATE_TRAVEL_CODE 100
#define SPELL_REPLICATE_CODE 88
#define SPELL_INSECT_SWARM_CODE 66
#define SPELL_CONJURE_CODE = static_cast<long>(49)

#endif /* DEFINE_BOTH_H */
