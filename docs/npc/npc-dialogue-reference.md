# Ultima VI NPC dialogue reference

Generated from the canonical host conversation data:

- `../../assets/game_files/host/host/NPC.BIN` ? conversation instructions
- `../../assets/game_files/host/host/NPCI.BIN` ? per-NPC starting instruction
- `../../assets/game_files/host/host/NPC.INF` ? exact text operands
- `../../assets/game_files/host/host/NPCNAME.TXT` ? NPC names

The document groups each contiguous conversation program by NPC name. NPC index is zero-based; the normal portrait/conversation port is NPC index + 1. The generic Guard portrait (port 192) is a special runtime mapping to NPC program 200.

Text is preserved exactly, including `@` keyword markers, `$` runtime placeholders, spelling, capitalization, and punctuation. A backslash in a display operand separates the successive on-screen segments. For readability, those segments appear on separate indented lines below the instruction.

Opcode legend:

- `1`: keyword test; `failure -> N` is the instruction used when no keyword matches.
- `2`: display text and end the current exchange.
- `22`: display text and continue executing the conversation program.

Total programs: 202. Total instructions: 17049.

## NPC index

- [Aaron ? NPC 67, port 68](#aaron-npc-67)
- [Aganar ? NPC 37, port 38](#aganar-npc-37)
- [Ahrmaand ? NPC 125, port 126](#ahrmaand-npc-125)
- [Amanda ? NPC 152, port 153](#amanda-npc-152)
- [Andrea ? NPC 54, port 55](#andrea-npc-54)
- [Andreas ? NPC 137, port 138](#andreas-npc-137)
- [Antonio ? NPC 90, port 91](#antonio-npc-90)
- [Anya ? NPC 13, port 14](#anya-npc-13)
- [Arbeth ? NPC 101, port 102](#arbeth-npc-101)
- [Ariana ? NPC 11, port 12](#ariana-npc-11)
- [Artegal ? NPC 121, port 122](#artegal-npc-121)
- [Arturos ? NPC 136, port 137](#arturos-npc-136)
- [Arty ? NPC 26, port 27](#arty-npc-26)
- [Arvin ? NPC 49, port 50](#arvin-npc-49)
- [Aurendir ? NPC 96, port 97](#aurendir-npc-96)
- [Beh Lem ? NPC 162, port 163](#beh-lem-npc-162)
- [Ben ? NPC 52, port 53](#ben-npc-52)
- [Blaine ? NPC 135, port 136](#blaine-npc-135)
- [Bolesh ? NPC 163, port 164](#bolesh-npc-163)
- [Bonn ? NPC 140, port 141](#bonn-npc-140)
- [Boskin ? NPC 58, port 59](#boskin-npc-58)
- [Brandon ? NPC 77, port 78](#brandon-npc-77)
- [Budo ? NPC 113, port 114](#budo-npc-113)
- [Captain Fox ? NPC 109, port 110](#captain-fox-npc-109)
- [Captain John ? NPC 181, port 182](#captain-john-npc-181)
- [Caradon ? NPC 153, port 154](#caradon-npc-153)
- [Caretaker ? NPC 164, port 165](#caretaker-npc-164)
- [Charlotte ? NPC 93, port 94](#charlotte-npc-93)
- [Chuckles ? NPC 8, port 9](#chuckles-npc-8)
- [Compassion ? NPC 191, port 192](#compassion-npc-191)
- [Conor ? NPC 92, port 93](#conor-npc-92)
- [Culham ? NPC 47, port 48](#culham-npc-47)
- [Cullen ? NPC 23, port 24](#cullen-npc-23)
- [Dale ? NPC 68, port 69](#dale-npc-68)
- [Dargoth ? NPC 35, port 36](#dargoth-npc-35)
- [Daros ? NPC 182, port 183](#daros-npc-182)
- [Daver ? NPC 19, port 20](#daver-npc-19)
- [de Hugh ? NPC 155, port 156](#de-hugh-npc-155)
- [Derydlus ? NPC 40, port 41](#derydlus-npc-40)
- [Dezana ? NPC 85, port 86](#dezana-npc-85)
- [Dorin ? NPC 103, port 104](#dorin-npc-103)
- [Doris ? NPC 72, port 73](#doris-npc-72)
- [Dr Cat ? NPC 108, port 109](#dr-cat-npc-108)
- [Draxinusom ? NPC 165, port 166](#draxinusom-npc-165)
- [Dunbar ? NPC 94, port 95](#dunbar-npc-94)
- [Dupre ? NPC 0, port 1](#dupre-npc-0)
- [Eckhart ? NPC 149, port 150](#eckhart-npc-149)
- [Efram ? NPC 20, port 21](#efram-npc-20)
- [Elad ? NPC 110, port 111](#elad-npc-110)
- [empty! ? NPC 186, port 187](#empty-npc-186)
- [Enrik ? NPC 118, port 119](#enrik-npc-118)
- [Ephemerides ? NPC 33, port 34](#ephemerides-npc-33)
- [Exodus ? NPC 189, port 190](#exodus-npc-189)
- [Faren ? NPC 147, port 148](#faren-npc-147)
- [Farmer Krill ? NPC 175, port 176](#farmer-krill-npc-175)
- [Farmer Nash ? NPC 174, port 175](#farmer-nash-npc-174)
- [Fentrissa ? NPC 119, port 120](#fentrissa-npc-119)
- [Finn ? NPC 161, port 162](#finn-npc-161)
- [Foodmaker ? NPC 173, port 174](#foodmaker-npc-173)
- [Fyodor ? NPC 18, port 19](#fyodor-npc-18)
- [Generic hireling ? NPC 201, port 202](#generic-hireling-npc-201)
- [Geoffrey ? NPC 5, port 6](#geoffrey-npc-5)
- [Gertan ? NPC 122, port 123](#gertan-npc-122)
- [Gherick ? NPC 150, port 151](#gherick-npc-150)
- [Gideon ? NPC 81, port 82](#gideon-npc-81)
- [Gilron ? NPC 120, port 121](#gilron-npc-120)
- [Glen ? NPC 144, port 145](#glen-npc-144)
- [Goodscrafter ? NPC 170, port 171](#goodscrafter-npc-170)
- [Gorn ? NPC 129, port 130](#gorn-npc-129)
- [Grison ? NPC 102, port 103](#grison-npc-102)
- [Guard ? NPC 200, port 201](#guard-npc-200)
- [Gwenneth ? NPC 14, port 15](#gwenneth-npc-14)
- [Gwenno ? NPC 64, port 65](#gwenno-npc-64)
- [Harold ? NPC 76, port 77](#harold-npc-76)
- [Healer ? NPC 171, port 172](#healer-npc-171)
- [Heftimus ? NPC 45, port 46](#heftimus-npc-45)
- [Hendle ? NPC 105, port 106](#hendle-npc-105)
- [Homer ? NPC 114, port 115](#homer-npc-114)
- [Honesty ? NPC 190, port 191](#honesty-npc-190)
- [Honor ? NPC 195, port 196](#honor-npc-195)
- [Horance ? NPC 86, port 87](#horance-npc-86)
- [Humility ? NPC 197, port 198](#humility-npc-197)
- [Immanuelle ? NPC 78, port 79](#immanuelle-npc-78)
- [Iolo ? NPC 2, port 3](#iolo-npc-2)
- [Isabella ? NPC 61, port 62](#isabella-npc-61)
- [Jaana ? NPC 60, port 61](#jaana-npc-60)
- [James ? NPC 69, port 70](#james-npc-69)
- [Jerris ? NPC 48, port 49](#jerris-npc-48)
- [Johann ? NPC 115, port 116](#johann-npc-115)
- [Julia ? NPC 65, port 66](#julia-npc-65)
- [Justice ? NPC 193, port 194](#justice-npc-193)
- [Kador ? NPC 133, port 134](#kador-npc-133)
- [Karina ? NPC 132, port 133](#karina-npc-132)
- [Katrina ? NPC 95, port 96](#katrina-npc-95)
- [Kenneth ? NPC 9, port 10](#kenneth-npc-9)
- [Koranada ? NPC 157, port 158](#koranada-npc-157)
- [Kytyn ? NPC 29, port 30](#kytyn-npc-29)
- [Lawrence ? NPC 75, port 76](#lawrence-npc-75)
- [Lazeena ? NPC 17, port 18](#lazeena-npc-17)
- [Le'nard ? NPC 53, port 54](#le-nard-npc-53)
- [Lenora ? NPC 57, port 58](#lenora-npc-57)
- [Lensmaker ? NPC 168, port 169](#lensmaker-npc-168)
- [Leodon ? NPC 111, port 112](#leodon-npc-111)
- [Leonna ? NPC 112, port 113](#leonna-npc-112)
- [Linda ? NPC 185, port 186](#linda-npc-185)
- [Lord British ? NPC 3, port 4](#lord-british-npc-3)
- [Loubet ? NPC 156, port 157](#loubet-npc-156)
- [Lynn ? NPC 27, port 28](#lynn-npc-27)
- [Lyssandra ? NPC 46, port 47](#lyssandra-npc-46)
- [Maldric ? NPC 30, port 31](#maldric-npc-30)
- [Mama ? NPC 142, port 143](#mama-npc-142)
- [Mandrake ? NPC 178, port 179](#mandrake-npc-178)
- [Manrel ? NPC 38, port 39](#manrel-npc-38)
- [Mariah ? NPC 31, port 32](#mariah-npc-31)
- [Marissa ? NPC 100, port 101](#marissa-npc-100)
- [Marney ? NPC 87, port 88](#marney-npc-87)
- [Marta ? NPC 83, port 84](#marta-npc-83)
- [Martin ? NPC 50, port 51](#martin-npc-50)
- [Matt ? NPC 12, port 13](#matt-npc-12)
- [Max ? NPC 16, port 17](#max-npc-16)
- [Merideth ? NPC 104, port 105](#merideth-npc-104)
- [Michael ? NPC 88, port 89](#michael-npc-88)
- [Michelle ? NPC 66, port 67](#michelle-npc-66)
- [Minax ? NPC 188, port 189](#minax-npc-188)
- [Mole ? NPC 176, port 177](#mole-npc-176)
- [Mondain ? NPC 187, port 188](#mondain-npc-187)
- [Morchella ? NPC 154, port 155](#morchella-npc-154)
- [Mortude ? NPC 99, port 100](#mortude-npc-99)
- [Myles ? NPC 143, port 144](#myles-npc-143)
- [Nan ? NPC 10, port 11](#nan-npc-10)
- [Naxatilor ? NPC 166, port 167](#naxatilor-npc-166)
- [Nema ? NPC 22, port 23](#nema-npc-22)
- [Nicodemus ? NPC 56, port 57](#nicodemus-npc-56)
- [Nomaan ? NPC 42, port 43](#nomaan-npc-42)
- [Nystul ? NPC 4, port 5](#nystul-npc-4)
- [Papa ? NPC 141, port 142](#papa-npc-141)
- [Patrick ? NPC 97, port 98](#patrick-npc-97)
- [Peer ? NPC 51, port 52](#peer-npc-51)
- [Penumbra ? NPC 39, port 40](#penumbra-npc-39)
- [Petroph ? NPC 117, port 118](#petroph-npc-117)
- [Peyton ? NPC 15, port 16](#peyton-npc-15)
- [Phoenix ? NPC 180, port 181](#phoenix-npc-180)
- [Pridgarm ? NPC 59, port 60](#pridgarm-npc-59)
- [Pushme Pullyu ? NPC 128, port 129](#pushme-pullyu-npc-128)
- [Quenton ? NPC 89, port 90](#quenton-npc-89)
- [Rob ? NPC 36, port 37](#rob-npc-36)
- [Rudyom ? NPC 126, port 127](#rudyom-npc-126)
- [Rufus ? NPC 21, port 22](#rufus-npc-21)
- [Sacrifice ? NPC 194, port 195](#sacrifice-npc-194)
- [Sandy ? NPC 73, port 74](#sandy-npc-73)
- [Sasha ? NPC 124, port 125](#sasha-npc-124)
- [Seggallion ? NPC 160, port 161](#seggallion-npc-160)
- [Selganor ? NPC 62, port 63](#selganor-npc-62)
- [Sentri ? NPC 184, port 185](#sentri-npc-184)
- [Shamino ? NPC 1, port 2](#shamino-npc-1)
- [Shawn ? NPC 116, port 117](#shawn-npc-116)
- [Sherry ? NPC 7, port 8](#sherry-npc-7)
- [Shubin ? NPC 151, port 152](#shubin-npc-151)
- [Simon ? NPC 158, port 159](#simon-npc-158)
- [Sin'Vraal ? NPC 139, port 140](#sin-vraal-npc-139)
- [Singularity ? NPC 198, port 199](#singularity-npc-198)
- [Sinjen ? NPC 179, port 180](#sinjen-npc-179)
- [Sionnach ? NPC 177, port 178](#sionnach-npc-177)
- [Smith ? NPC 130, port 131](#smith-npc-130)
- [Snake_Charmer ? NPC 169, port 170](#snake-charmer-npc-169)
- [Spirituality ? NPC 196, port 197](#spirituality-npc-196)
- [Stelnar ? NPC 43, port 44](#stelnar-npc-43)
- [Stephanie ? NPC 146, port 147](#stephanie-npc-146)
- [Stivius ? NPC 82, port 83](#stivius-npc-82)
- [Sutek ? NPC 127, port 128](#sutek-npc-127)
- [Sylaina ? NPC 145, port 146](#sylaina-npc-145)
- [Tara ? NPC 63, port 64](#tara-npc-63)
- [Taynith ? NPC 134, port 135](#taynith-npc-134)
- [Terri ? NPC 28, port 29](#terri-npc-28)
- [Tessa ? NPC 159, port 160](#tessa-npc-159)
- [Thariand ? NPC 32, port 33](#thariand-npc-32)
- [Thindle ? NPC 98, port 99](#thindle-npc-98)
- [Tholden ? NPC 6, port 7](#tholden-npc-6)
- [Tiberius ? NPC 24, port 25](#tiberius-npc-24)
- [Timothy ? NPC 107, port 108](#timothy-npc-107)
- [Tobatha ? NPC 79, port 80](#tobatha-npc-79)
- [Trebor ? NPC 70, port 71](#trebor-npc-70)
- [Trenton ? NPC 80, port 81](#trenton-npc-80)
- [Troy ? NPC 71, port 72](#troy-npc-71)
- [Ubermon ? NPC 106, port 107](#ubermon-npc-106)
- [unconcious man ? NPC 123, port 124](#unconcious-man-npc-123)
- [Utomo ? NPC 55, port 56](#utomo-npc-55)
- [Valkadesh ? NPC 167, port 168](#valkadesh-npc-167)
- [Valor ? NPC 192, port 193](#valor-npc-192)
- [Van Kellian ? NPC 44, port 45](#van-kellian-npc-44)
- [Wanda ? NPC 138, port 139](#wanda-npc-138)
- [Weaponsmith ? NPC 172, port 173](#weaponsmith-npc-172)
- [Whitsaber ? NPC 74, port 75](#whitsaber-npc-74)
- [Wilbur ? NPC 25, port 26](#wilbur-npc-25)
- [William ? NPC 91, port 92](#william-npc-91)
- [Wisp ? NPC 199, port 200](#wisp-npc-199)
- [Xiao ? NPC 34, port 35](#xiao-npc-34)
- [Ybarra ? NPC 183, port 184](#ybarra-npc-183)
- [Yorl ? NPC 84, port 85](#yorl-npc-84)
- [Zeke ? NPC 148, port 149](#zeke-npc-148)
- [Zellivan ? NPC 41, port 42](#zellivan-npc-41)
- [Zoltan ? NPC 131, port 132](#zoltan-npc-131)

<a id="aaron-npc-67"></a>
## Aaron ? NPC 67, port 68

Instruction range: 6285?6326

```text
[6286 op=1 keyword; failure -> 6289]
  1: yes

[6291 op=2 display+exit]
  1: "Oh. Well, perhaps some other time then."

[6292 op=1 keyword; failure -> 6295]
  1: look

[6294 op=2 display+exit]
  1: a plump blond man with curly blond hair and a cheerful face.
  2: "Hello again, Avatar!"
  3: "What can I do for you this $T?"

[6295 op=1 keyword; failure -> 6298]
  1: look

[6297 op=2 display+exit]
  1: a plump blond man with curly blond hair and a cheerful face.
  2: "Hello hello!"
  3: "What can I do for you this $T?"

[6298 op=1 keyword; failure -> 6301]
  1: name

[6300 op=2 display+exit]
  1: "That's funny. I remember your name!"

[6301 op=1 keyword; failure -> 6305]
  1: name

[6304 op=2 display+exit]
  1: "My name's Aaron."

[6305 op=1 keyword; failure -> 6307]
  1: job

[6306 op=2 display+exit]
  1: "I run the sawmill! Is there anything I can do for thee?"

[6308 op=1 keyword; failure -> 6310]
  1: boar,log

[6309 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[6311 op=1 keyword; failure -> 6314]
  1: boar,log

[6313 op=2 display+exit]
  1: "Aye, I see $Y has a yew log."
  2: He inspects the wood with a practiced eye.
  3: "Good grain. Yes, I can make a fine board out of this log."
  4: "But it will cost thee 5 gold, $Y!"
  5: "Still interested?"

[6314 op=1 keyword; failure -> 6316]
  1: boar,log

[6315 op=2 display+exit]
  1: "My mill makes smooth boards and planks out of rough-cut logs."

[6316 op=1 keyword; failure -> 6318]
  1: bye

[6317 op=2 display+exit]
  1: "Farewell!"

[6318 op=2 display+exit]
  1: "Beg pardon?"

[6321 op=2 display+exit]
  1: "Oh. Well, perhaps some other time then."

[6325 op=2 display+exit]
  1: "Sorry, but I can't work for free!"
  2: "Come back when you have the 5 gold!"

[6326 op=2 display+exit]
  1: Aaron goes to his saw and turns the log into a flat, sanded board.
  2: "There!" He hands $Y the finished product.

```

<a id="aganar-npc-37"></a>
## Aganar ? NPC 37, port 38

Instruction range: 3451?3491

```text
[3451 op=1 keyword; failure -> 3454]
  1: look

[3453 op=2 display+exit]
  1: a gentleman with an air of leadership about him.
  2: "Good $T, Avatar."
  3: "Is there ought else I can aid thee with?"

[3454 op=1 keyword; failure -> 3457]
  1: look

[3456 op=2 display+exit]
  1: a gentleman with an air of leadership about him.
  2: "Thou art the Avatar!"
  3: "I saw thy portrait in Lord British's castle."
  4: "If there be any assistance I can offer, thou needst but ask it."

[3457 op=1 keyword; failure -> 3460]
  1: moon

[3459 op=2 display+exit]
  1: "Verily, 'tis a fine town."
  2: "Hast thou ever thought of moving here?"

[3462 op=1 keyword; failure -> 3464]
  1: yes,no

[3463 op=2 display+exit]
  1: "Thou wouldst not regret it."
  2: "There's good @fishing off our shores, and plenty of merriment to be found over at the Blue @Bottle."

[3464 op=1 keyword; failure -> 3467]
  1: name

[3466 op=2 display+exit]
  1: "I am Lord Aganar of @Moonglow."

[3467 op=1 keyword; failure -> 3469]
  1: job

[3468 op=2 display+exit]
  1: "I keep things running smoothly here."
  2: "'Tis not difficult in a town of such @honesty."

[3469 op=1 keyword; failure -> 3471]
  1: fish

[3470 op=2 display+exit]
  1: "Aye, indeed."

[3471 op=1 keyword; failure -> 3473]
  1: blue,bott,merr

[3472 op=2 display+exit]
  1: "Thou wilt see much drinking at the Blue Bottle Tavern, and singing, and revelry, and telling of tales."
  2: "But mostly @drinking."

[3473 op=1 keyword; failure -> 3475]
  1: drin,sing,reve,tell,tale,tave

[3474 op=2 display+exit]
  1: "If thou art curious, why not go pay a visit?"
  2: "Thou cannot miss it - 'tis the biggest building in town."

[3475 op=1 keyword; failure -> 3477]
  1: hone

[3476 op=2 display+exit]
  1: "Honesty is the virtue we prize most highly here in Moonglow."
  2: "We are closest to the @Shrine of Honesty."
  3: "And indeed, the @Rune of Honesty was entrusted into my care some years ago."

[3477 op=1 keyword; failure -> 3479]
  1: shri

[3478 op=2 display+exit]
  1: "'Tis north of here, on Dagger Isle."

[3479 op=1 keyword; failure -> 3481]
  1: dagg,isle

[3480 op=2 display+exit]
  1: "Aye, just a ways to the north."

[3481 op=1 keyword; failure -> 3483]
  1: rune,care

[3482 op=2 display+exit]
  1: "I had it once, but I entrusted it to @Beyvin's care, as he is the most honest man I know."
  2: "He lives with @Penumbra."

[3483 op=1 keyword; failure -> 3485]
  1: mant

[3484 op=2 display+exit]
  1: "Penumbra might be able to help you with that."

[3485 op=1 keyword; failure -> 3487]
  1: penu

[3486 op=2 display+exit]
  1: "I can't imagine what Beyvin sees in her!"

[3487 op=1 keyword; failure -> 3489]
  1: beyv

[3488 op=2 display+exit]
  1: "Haven't seen him around lately."
  2: "Penumbra should be able to tell you where to find him."

[3489 op=1 keyword; failure -> 3491]
  1: bye

[3490 op=2 display+exit]
  1: "Until next we meet, then."

[3491 op=2 display+exit]
  1: "Well you might ask..."

```

<a id="ahrmaand-npc-125"></a>
## Ahrmaand ? NPC 125, port 126

Instruction range: 11429?11487

```text
[11436 op=1 keyword; failure -> 11439]
  1: look

[11438 op=2 display+exit]
  1: a distinguished looking gentleman.
  2: "Ah, $P, thou hast come back to get better acquainted!"
  3: "How might I be of assistance this fine $T?"

[11439 op=1 keyword; failure -> 11442]
  1: look

[11441 op=2 display+exit]
  1: a distinguished looking gentleman.
  2: "$P, seeing thee again, I feel as if the clouds have parted to reveal the sun once more."
  3: "How might I be of assistance this fine $T?"

[11442 op=1 keyword; failure -> 11445]
  1: look

[11444 op=2 display+exit]
  1: a distinguished looking gentleman.
  2: "Milady $P!"
  3: "If thou art as happy to see me as I to see thee, then this must be for thee a joyous occasion indeed."
  4: "How might I be of assistance this fine $T?"

[11445 op=1 keyword; failure -> 11448]
  1: look

[11447 op=2 display+exit]
  1: a distinguished looking gentleman.
  2: "Ah, $P!"
  3: "'Tis an honor to welcome thee once more to my humble village."
  4: "How might I be of assistance this fine $T?"

[11448 op=1 keyword; failure -> 11451]
  1: look

[11450 op=2 display+exit]
  1: a distinguished looking gentleman.
  2: "Greetings, $G, and welcome to the town of Cove."
  3: "How might I be of assistance this fine $T?"

[11451 op=1 keyword; failure -> 11454]
  1: name

[11453 op=2 display+exit]
  1: "I am the Viscount Ahrmaand."

[11454 op=1 keyword; failure -> 11457]
  1: name

[11456 op=2 display+exit]
  1: "I am the Viscount Ahrmaand."
  2: "And what is thy name?"

[11457 op=1 keyword; failure -> 11459]
  1: job

[11458 op=2 display+exit]
  1: "I govern here in @Cove."
  2: "'Tis hard work, but my people love me for it."
  3: "I always thought it a blessing to live so close to a @shrine, though since the gargoyles came none dare go near it."

[11459 op=1 keyword; failure -> 11461]
  1: cove,city

[11460 op=2 display+exit]
  1: "We have fine facilities here for healing the wounded and the @sick."

[11461 op=1 keyword; failure -> 11463]
  1: shri

[11462 op=2 display+exit]
  1: "In Britannia there are eight shrines, one for each of the eight @virtues of the Avatar."
  2: "They are places where people can go to meditate on these virtues."

[11463 op=1 keyword; failure -> 11465]
  1: eigh,virt

[11464 op=2 display+exit]
  1: "The virtues are honesty, compassion, valour, justice, sacrifice, honor, spirituality, and humility."
  2: "'Tis good to @meditate on these virtues often."

[11465 op=1 keyword; failure -> 11467]
  1: medi

[11466 op=2 display+exit]
  1: "Each shrine has its own @mantra."
  2: "This is a chant which one should recite whilst meditating therein."
  3: "Once there was a time when a special rune was required to enter each shrine as well."
  4: "Now the gargoyles have taken the Shrine of Compassion, and it is lost to the people once more."

[11467 op=1 keyword; failure -> 11469]
  1: comp,sick,woun,gert,garg

[11468 op=2 display+exit]
  1: "A party of adventurers came here after their attempt to defeat the gargoyles at the Shrine of Compassion."
  2: "I spoke with their @leader when they arrived."
  3: "Thou shouldst speak with him thyself."
  4: "He told of a strange violet energy field."
  5: "I think mayhap 'tis similar to the @force which blocked entry to the shrines many years ago."
  6: "The eight mystic runes were most useful in those days."

[11469 op=1 keyword; failure -> 11471]
  1: lead

[11470 op=2 display+exit]
  1: "He's in bed at Sasha's, recuperating."

[11471 op=1 keyword; failure -> 11473]
  1: forc,rune,myst,fiel,ener,viol

[11472 op=2 display+exit]
  1: "Use the Rune of Compassion whilst chanting the @Mantra of Compassion."
  2: "Methinks this may negate the gargoyle's magic at that shrine."

[11473 op=1 keyword; failure -> 11475]
  1: chan,mant

[11474 op=2 display+exit]
  1: "Ask Lord British's Chancellor, Tholden."
  2: "He canst tell thee of the mantra, and like as not he will also know of the rune."

[11475 op=1 keyword; failure -> 11478]
  1: bye

[11477 op=2 display+exit]
  1: "Twas a delight to converse with thee, my dear."
  2: "My heart will be aflutter with anticipation till thou dost visit me again."

[11478 op=1 keyword; failure -> 11480]
  1: bye

[11479 op=2 display+exit]
  1: "Farewell sirrah, and good luck with thy quest."

[11480 op=2 display+exit]
  1: "I cannot help thee with that."

[11487 op=2 display+exit]
  1: "Well met, $G!"

```

<a id="amanda-npc-152"></a>
## Amanda ? NPC 152, port 153

Instruction range: 14089?14192

```text
[14090 op=1 keyword; failure -> 14092]
  1: yes

[14094 op=2 display+exit]
  1: "What can I get ya?"

[14096 op=1 keyword; failure -> 14098]
  1: yes

[14100 op=2 display+exit]
  1: "What can I get ya?"

[14102 op=1 keyword; failure -> 14104]
  1: yes

[14106 op=2 display+exit]
  1: "What can I get ya?"

[14108 op=1 keyword; failure -> 14110]
  1: yes

[14112 op=2 display+exit]
  1: "What can I get ya?"

[14115 op=1 keyword; failure -> 14118]
  1: look

[14117 op=2 display+exit]
  1: a large black woman with a loud laugh.
  2: "You're here again!"
  3: "Good."
  4: She smiles widely.
  5: "What can I get ya?"

[14118 op=1 keyword; failure -> 14121]
  1: look

[14120 op=2 display+exit]
  1: a large black woman with a loud laugh.
  2: "Hello, hello!" she cries, clapping her hands together.
  3: She smiles widely.
  4: "What can I get ya?"

[14121 op=1 keyword; failure -> 14124]
  1: name

[14123 op=2 display+exit]
  1: "Amanda, but you can call me 'Miss Mandy'."

[14124 op=1 keyword; failure -> 14126]
  1: buy

[14125 op=2 display+exit]
  1: "Which, @ale, @mead, @wine, @rations or @Waybread?"

[14126 op=1 keyword; failure -> 14128]
  1: job

[14127 op=2 display+exit]
  1: "I sell @ale, @mead, @wine, @rations and @Waybread."

[14128 op=1 keyword; failure -> 14131]
  1: way,brea

[14130 op=2 display+exit]
  1: "Some say I make the best Waybread in these parts."
  2: "It costs just 2 gold."
  3: "You want to try some?"

[14131 op=1 keyword; failure -> 14134]
  1: mead

[14133 op=2 display+exit]
  1: "That'll be 4 crowns, okay sugar?"

[14134 op=1 keyword; failure -> 14137]
  1: ale

[14136 op=2 display+exit]
  1: "That'll be 4 crowns, okay sugar?"

[14137 op=1 keyword; failure -> 14140]
  1: wine

[14139 op=2 display+exit]
  1: "That'll be 4 crowns, okay sugar?"

[14140 op=1 keyword; failure -> 14143]
  1: rati

[14142 op=2 display+exit]
  1: "Rations are 4 gold apiece."
  2: "How many do you want?"

[14143 op=1 keyword; failure -> 14145]
  1: aman

[14144 op=2 display+exit]
  1: "Please, call me 'Miss Mandy'."

[14145 op=1 keyword; failure -> 14147]
  1: miss,mand

[14146 op=2 display+exit]
  1: "There, that's much better."

[14147 op=1 keyword; failure -> 14149]
  1: segg

[14148 op=2 display+exit]
  1: "He's been in here a few times."
  2: "He has a homesick look, he does."

[14149 op=1 keyword; failure -> 14151]
  1: bye

[14150 op=2 display+exit]
  1: "Remember to tell your friends 'bout Miss Mandy."

[14151 op=2 display+exit]
  1: "That's one on me."
  2: "'Tain't never heard o' that."

[14155 op=2 display+exit]
  1: "Come back when you have some gold."

[14157 op=2 display+exit]
  1: "You don't have room to carry it."

[14158 op=2 display+exit]
  1: She gives you some Waybread.
  2: "That'll put some meat on your bones."

[14162 op=2 display+exit]
  1: "Come back when you have some gold."

[14164 op=2 display+exit]
  1: "You don't have room to carry it."

[14165 op=2 display+exit]
  1: "Here you are."
  2: She gives you the mead.
  3: "Y'all enjoy it, now."

[14169 op=2 display+exit]
  1: "Come back when you have some gold."

[14171 op=2 display+exit]
  1: "You don't have room to carry it."

[14172 op=2 display+exit]
  1: "Here you are."
  2: She gives you the ale.
  3: "Y'all enjoy it, now."

[14176 op=2 display+exit]
  1: "Come back when you have some gold."

[14178 op=2 display+exit]
  1: "You don't have room to carry it."

[14179 op=2 display+exit]
  1: "Here you are."
  2: She gives you the wine.
  3: "Y'all enjoy it, now."

[14182 op=2 display+exit]
  1: "What can I get ya?"

[14184 op=2 display+exit]
  1: "I don't have that many sugar!"

[14189 op=2 display+exit]
  1: "Come back when you have some gold."

[14191 op=2 display+exit]
  1: "You don't have room to carry that."

[14192 op=2 display+exit]
  1: She hands the mutton rations to you.
  2: "What else can I get ya?"

```

<a id="andrea-npc-54"></a>
## Andrea ? NPC 54, port 55

Instruction range: 4703?4834

```text
[4712 op=2 display+exit]
  1: "Pleased to meet you, dear."

[4713 op=2 display+exit]
  1: "I'm very pleased to meet you," she purrs.

[4715 op=1 keyword; failure -> 4718]
  1: yes

[4720 op=2 display+exit]
  1: "Well, maybe some other time, sugar..."

[4721 op=1 keyword; failure -> 4725]
  1: look

[4724 op=2 display+exit]
  1: an enormous woman, both in size and personality.
  2: "Hello again, my dear!" she screams as she strides over to you with beer mugs in her hands.
  3: "Welcome to the Slaughtered Lamb. What can I get you?"

[4725 op=1 keyword; failure -> 4728]
  1: look

[4727 op=2 display+exit]
  1: an enormous woman, both in size and personality.
  2: "Hello again, handsome!" she screams as she strides over to you with beer mugs in her hands.
  3: "Welcome to the Slaughtered Lamb. What can I get you?"

[4728 op=1 keyword; failure -> 4731]
  1: look

[4730 op=2 display+exit]
  1: an enormous woman, both in size and personality.
  2: "Hello, my dear!" she screams as she strides over to you with beer mugs in her hands.
  3: "Welcome to the Slaughtered Lamb. What can I get you?"

[4731 op=1 keyword; failure -> 4733]
  1: look

[4732 op=2 display+exit]
  1: an enormous woman, both in size and personality.
  2: "Hello, handsome!" she screams as she strides over to you with beer mugs in her hands.
  3: "Welcome to the Slaughtered Lamb. What can I get you?"

[4733 op=1 keyword; failure -> 4736]
  1: name

[4735 op=2 display+exit]
  1: "@Andrea!"

[4736 op=1 keyword; failure -> 4739]
  1: name

[4738 op=2 display+exit]
  1: "@Andrea!"
  2: "And yours, my fine friend?"

[4739 op=1 keyword; failure -> 4741]
  1: andr

[4740 op=2 display+exit]
  1: "Yes."
  2: "Them's who I beat at arm @wrestling call me Miss Barlap, though."
  3: "Out of respect." She laughs.

[4741 op=1 keyword; failure -> 4744]
  1: arm,wres

[4743 op=2 display+exit]
  1: "Alright, alright, I admit it - ya won fair an' square."
  2: "No need to gloat about it..."

[4744 op=1 keyword; failure -> 4747]
  1: arm,wres

[4746 op=2 display+exit]
  1: "Now don't feel bad, sugar, lots of folks lose to Andrea!" She laughs heartily.

[4747 op=1 keyword; failure -> 4750]
  1: arm,wres

[4749 op=2 display+exit]
  1: She looks you up and down and then feels your arm. "Care for a match?"

[4750 op=1 keyword; failure -> 4752]
  1: job

[4751 op=2 display+exit]
  1: "Honey, I've had many jobs in my time. But runnin' this pub is the best, 'cause I get to meet the strongest @men."
  2: "I sell @ribs, @ale, @mead, @wine, and @rations."

[4752 op=1 keyword; failure -> 4754]
  1: buy

[4753 op=2 display+exit]
  1: "Which, @ribs, @ale, @mead, @wine, or @rations?"

[4754 op=1 keyword; failure -> 4756]
  1: stro,men

[4755 op=2 display+exit]
  1: "Look around. Here you'll find the men whose lives have lead them to many dangerous places."
  2: Someone in a dark corner shouts "Like this pub!" and all the patrons laugh.

[4756 op=1 keyword; failure -> 4758]
  1: lena

[4757 op=2 display+exit]
  1: "A little girlie-man. But he does make some fine clothes."

[4758 op=1 keyword; failure -> 4760]
  1: big,ben

[4759 op=2 display+exit]
  1: "Now there's a man." She looks into a beer mug and fixes her hair.
  2: "I wish he'd come into town more often."

[4760 op=1 keyword; failure -> 4762]
  1: chan

[4761 op=2 display+exit]
  1: "That old witch? She may not have hit the bottle, but I believe a few have hit her."
  2: The patrons around you chuckle.

[4762 op=1 keyword; failure -> 4764]
  1: garg

[4763 op=2 display+exit]
  1: "I've never seen any creature that could scare me. I say if they want trouble, come in this pub!"
  2: The men around her let out a cheer and she turns, acknowledging them with a bow.

[4764 op=1 keyword; failure -> 4766]
  1: lord,brit

[4765 op=2 display+exit]
  1: "He's never been in here. Can't imagine why--it seems like his kind of establishment."

[4766 op=1 keyword; failure -> 4768]
  1: bye

[4767 op=2 display+exit]
  1: "So long. Come back when you're thirsty."
  2: "And if you see that cutie Utomo, tell him Andrea says hi."

[4768 op=1 keyword; failure -> 4773]
  1: ribs

[4772 op=2 display+exit]
  1: "That'll be 6 crowns, okay?"

[4773 op=1 keyword; failure -> 4778]
  1: ale

[4777 op=2 display+exit]
  1: "That'll be 3 crowns, okay?"

[4778 op=1 keyword; failure -> 4783]
  1: mead

[4782 op=2 display+exit]
  1: "That'll be 4 crowns, okay?"

[4783 op=1 keyword; failure -> 4788]
  1: wine

[4787 op=2 display+exit]
  1: "That'll be 6 crowns, okay?"

[4788 op=1 keyword; failure -> 4791]
  1: rati

[4790 op=2 display+exit]
  1: "Rations are 4 gold apiece. How many do you want?"

[4791 op=1 keyword; failure -> 4793]
  1: bye

[4792 op=2 display+exit]
  1: "See you later."

[4796 op=1 keyword; failure -> 4798]
  1: yes

[4799 op=1 keyword; failure -> 4802]
  1: no

[4801 op=2 display+exit]
  1: "What can I get you?"

[4802 op=2 display+exit]
  1: "Ask around."

[4805 op=2 display+exit]
  1: "What can I get you?"

[4807 op=2 display+exit]
  1: "You don't have room to carry it."

[4812 op=2 display+exit]
  1: "You can't afford it, sweetie."

[4814 op=2 display+exit]
  1: Andrea looks at $Y. "You don't have room to carry it."

[4815 op=2 display+exit]
  1: Andrea hands the rations to $Y.

[4819 op=2 display+exit]
  1: "You can't afford it, sweetie."

[4821 op=2 display+exit]
  1: "You don't have room to carry it."

[4823 op=2 display+exit]
  1: Andrea gives $Y some ribs.

[4825 op=2 display+exit]
  1: Andrea gives $Y some ale. "That ought to hold you."

[4827 op=2 display+exit]
  1: Andrea gives $Y some mead. "That ought to hold you."

[4828 op=2 display+exit]
  1: Andrea gives $Y some wine. "That ought to hold you."

[4832 op=2 display+exit]
  1: "Yer on!" She grabs your arm and the contest begins! "Arrghh!!!"
  2: "Nnghhh!!!"
  3: "Unghhh!!!"
  4: And the winner is... $Y!!!

[4834 op=2 display+exit]
  1: "Yer on!" She grabs your arm and the contest begins! "Arrghh!!!"
  2: "Nnghhh!!!"
  3: "Unghhh!!!"
  4: And the winner is... Andrea!!!

```

<a id="andreas-npc-137"></a>
## Andreas ? NPC 137, port 138

Instruction range: 12650?12801

```text
[12651 op=1 keyword; failure -> 12656]
  1: yes,yea

[12657 op=1 keyword; failure -> 12660]
  1: yes,yea

[12659 op=2 display+exit]
  1: "Hey, you don't have the money!"
  2: "I don't take charity cases."

[12663 op=2 display+exit]
  1: "As you like."
  2: "But take my advice--you should live a little now."
  3: "That way, you'll have something to remember when you're old and gray..."

[12666 op=2 display+exit]
  1: "As you like."
  2: "But take my advice--you should live a little now."

[12668 op=1 keyword; failure -> 12671]
  1: yes

[12670 op=2 display+exit]
  1: "Can't say I blame you. In fact, if she wasn't my sister..."

[12673 op=2 display+exit]
  1: "Hmm, wouldn't have figured you for the type..."

[12675 op=1 keyword; failure -> 12680]
  1: yes

[12679 op=2 display+exit]
  1: "I overheard a bard in a tavern say that it was somewhere on the Isle of the Avatar."
  2: "Perhaps @Arturos could tell you precisely."

[12681 op=1 keyword; failure -> 12684]
  1: yes

[12683 op=2 display+exit]
  1: "Come back when you have the money."

[12686 op=2 display+exit]
  1: "Suit yourself."

[12688 op=1 keyword; failure -> 12691]
  1: yes

[12690 op=2 display+exit]
  1: "You have the carefree spirit, but alas, one must be born a gypsy."
  2: "A fine thing to dream of!"
  3: "But one such as you could never be a gypsy."

[12693 op=2 display+exit]
  1: "The more fool you!"

[12695 op=1 keyword; failure -> 12700]
  1: yes

[12699 op=2 display+exit]
  1: Pulling a couple of sticks from his belt, the gypsy hands you one.
  2: You fight with them as if they were swords for a while.
  3: After you're finished he comes over and puts his arm around your shoulder
  4: as he gives you a few pointers on fencing style.
  5: You're not sure, but you think perhaps you felt a slight tug at the pouch you keep your coins in.

[12702 op=2 display+exit]
  1: "Maybe some other time."

[12705 op=1 keyword; failure -> 12707]
  1: look

[12706 op=2 display+exit]
  1: a handsome, carefree gypsy man.
  2: "Ah, I see you couldn't stay away..."
  3: "I have that effect on women."

[12708 op=1 keyword; failure -> 12710]
  1: look

[12709 op=2 display+exit]
  1: a handsome, carefree gypsy man.
  2: "Welcome back, $P."
  3: "Came for some more of our gypsy hospitality, I see!"

[12711 op=1 keyword; failure -> 12714]
  1: look

[12713 op=2 display+exit]
  1: a handsome, carefree gypsy man.
  2: "Hey, foxy!"
  3: "I bet you'd like me to unsheathe my weapon so you can have a look..."

[12714 op=1 keyword; failure -> 12718]
  1: look

[12717 op=2 display+exit]
  1: a handsome, carefree gypsy man.
  2: txt "Hey there!"
  3: "Welcome to our humble caravan!"
  4: "You look like a bold warrior - would you like to spar a bit for practice?"

[12718 op=1 keyword; failure -> 12720]
  1: name

[12719 op=2 display+exit]
  1: "I am Andreas, prince of the @gypsies."

[12720 op=1 keyword; failure -> 12722]
  1: prof,serv

[12721 op=2 display+exit]
  1: "We all have our @talents."
  2: "I myself happen to have two."
  3: "I'd be glad to show you sometime!"

[12722 op=1 keyword; failure -> 12724]
  1: tale,show

[12723 op=2 display+exit]
  1: "What, you mean @swordplay?"
  2: "Just ask, and I'll give thee a @lesson you won't forget!"

[12724 op=1 keyword; failure -> 12726]
  1: wand

[12725 op=2 display+exit]
  1: "She practices the world's oldest @profession."
  2: "I, of course, have perfected it, and thus need no practice."

[12726 op=1 keyword; failure -> 12728]
  1: artu

[12727 op=2 display+exit]
  1: "He's my dad - the king of the gypsies!"

[12728 op=1 keyword; failure -> 12730]
  1: map

[12729 op=2 display+exit]
  1: "A map?"
  2: "Aye, @Arturos has some scrap like that."
  3: "I know nothing more of it."

[12730 op=1 keyword; failure -> 12733]
  1: swor,spar,weap,unsh,less

[12732 op=2 display+exit]
  1: "Would you like to spar a bit?"

[12733 op=1 keyword; failure -> 12736]
  1: trav,care,gyps,prin,king,life

[12735 op=2 display+exit]
  1: "Wouldn't you like to be a gypsy too?"

[12736 op=1 keyword; failure -> 12739]
  1: silv,tabl

[12738 op=2 display+exit]
  1: "Ah yes, you've heard of the silver tablet!"
  2: "I haven't got it myself, but I can tell you where it is for a price..."
  3: "Shall we say 7 gold pieces?"

[12739 op=1 keyword; failure -> 12742]
  1: job

[12741 op=2 display+exit]
  1: "I live the carefree life of a @gypsy, traveling hither and yon."
  2: "Naturally, when so lovely a lady as yourself comes along, I offer, shall we say, my @services."

[12742 op=1 keyword; failure -> 12748]
  1: job

[12745 op=22 display+continue]
  1: "I live the carefree life of a @gypsy, traveling hither and yon."
  2: "When the opportunity arises, I do what I can to brighten the lives of the ladies."
  3: "Of course, you'd probably be more interested in my sister @Wanda, right?"

[12747 op=2 display+exit]
  1: "I'll just bet he does!"

[12748 op=1 keyword; failure -> 12751]
  1: job

[12750 op=2 display+exit]
  1: "I live the carefree life of a @gypsy, traveling hither and yon."
  2: "When the opportunity arises, I do what I can to brighten the lives of the ladies."
  3: "Of course, you'd probably be more interested in my sister @Wanda, right?"

[12751 op=1 keyword; failure -> 12754]
  1: love

[12753 op=2 display+exit]
  1: "Naturally, when so lovely a lady as yourself comes along, I offer, shall we say, my @services."

[12754 op=1 keyword; failure -> 12760]
  1: love

[12757 op=22 display+continue]
  1: "When the opportunity arises, I do what I can to brighten the lives of the ladies."
  2: "Of course, you'd probably be more interested in my sister @Wanda, right?"

[12759 op=2 display+exit]
  1: "I'll just bet he does!"

[12760 op=1 keyword; failure -> 12763]
  1: love

[12762 op=2 display+exit]
  1: "When the opportunity arises, I do what I can to brighten the lives of the ladies."
  2: "Of course, you'd probably be more interested in my sister @Wanda, right?"

[12763 op=1 keyword; failure -> 12768]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12767 op=2 display+exit]
  1: "My mind is willing, but the flesh is weak..."
  2: "Come back later, when I've had some time to rest."

[12769 op=1 keyword; failure -> 12775]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12772 op=22 display+continue]
  1: "Oho, so that's what you're after, you naughty girl!"
  2: "For a mere 10 gold pieces, I'll transport you to the heights of ecstasy."
  3: "Will it be yea or nay?"
  4: "Well, come on. Yea or nay, $G?"

[12774 op=2 display+exit]
  1: Dupre interrupts.
  2: "`$G, I really doubt we have time for this..."

[12776 op=1 keyword; failure -> 12781]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12778 op=22 display+continue]
  1: "Oho, so that's what you're after, you naughty boy!"
  2: "Well..."
  3: "Normally I'm more of a ladies' man."
  4: "But I guess your money is as good as anyone else's."
  5: "It'll cost you 10 gold, okay?"
  6: "Well, come on. Yea or nay, $G?"

[12780 op=2 display+exit]
  1: Dupre interrupts.
  2: "`$G, I really doubt we have time for this..."

[12781 op=1 keyword; failure -> 12785]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12784 op=2 display+exit]
  1: "Oho, so that's what you're after, you naughty girl!"
  2: "For a mere 10 gold pieces, I'll transport you to the heights of ecstasy."
  3: "Will it be yea or nay?"
  4: A voice in the back of your head asks, "Do you really have time for this?"
  5: "Well, come on. Yea or nay, $G?"

[12785 op=1 keyword; failure -> 12788]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12787 op=2 display+exit]
  1: "Oho, so that's what you're after, you naughty boy!"
  2: "Well..."
  3: "Normally I'm more of a ladies' man."
  4: "But I guess your money is as good as anyone else's."
  5: "It'll cost you 10 gold, okay?"
  6: A voice in the back of your head asks, "Do you really have time for this?"
  7: "Well, come on. Yea or nay, $G?"

[12788 op=1 keyword; failure -> 12791]
  1: bye

[12790 op=2 display+exit]
  1: "Come again soon."

[12791 op=1 keyword; failure -> 12794]
  1: bye

[12793 op=2 display+exit]
  1: "I hope we get better acquainted when next we meet."
  2: "I like your clothes, but I think you'd look better without them..."

[12794 op=1 keyword; failure -> 12796]
  1: bye

[12795 op=2 display+exit]
  1: "May your travels prove interesting."

[12798 op=22 display+continue]
  1: The gypsy leads you into a nearby wagon...
  2: After a while, you and he wander back to the rest of the caravan, weary but satisfied.
  3: Andreas laughs.
  4: "'Tis not a crime to give yourself over to pleasure..."

[12800 op=2 display+exit]
  1: Dupre scowls.
  2: "Well, you've certainly picked a fine one to be carrying on with."
  3: "I would remind you that Britannia is under invasion by hordes of gargoyles,"
  4: "but I'd hate to spoil your fun..."

[12801 op=2 display+exit]
  1: The gypsy leads you into a nearby wagon...
  2: After a while, you and he wander back to the rest of the caravan, weary but satisfied.
  3: You try to ignore the nagging voice in the back of your head that says you shouldn't have wasted time like that.

```

<a id="antonio-npc-90"></a>
## Antonio ? NPC 90, port 91

Instruction range: 8583?8655

```text
[8587 op=1 keyword; failure -> 8590]
  1: no

[8589 op=2 display+exit]
  1: "As you wish."

[8590 op=1 keyword; failure -> 8592]
  1: yes

[8591 op=2 display+exit]
  1: "Then prithee, what is it?"

[8597 op=2 display+exit]
  1: "The pleasure is all mine."

[8599 op=1 keyword; failure -> 8601]
  1: look

[8600 op=2 display+exit]
  1: a gentleman of slight build and refined manners.
  2: "Greetings!"
  3: "I am Lord Antonio."
  4: "Might I ask thy name?"

[8601 op=2 display+exit]
  1: "Greetings!"
  2: "I am Lord Antonio."
  3: "Might I ask thy name?"

[8603 op=1 keyword; failure -> 8606]
  1: yes

[8605 op=2 display+exit]
  1: "I knew you would. That's my favorite trick!"

[8607 op=1 keyword; failure -> 8610]
  1: no

[8609 op=2 display+exit]
  1: "Too bad. Perhaps I should learn a better trick."

[8611 op=1 keyword; failure -> 8614]
  1: yes

[8613 op=2 display+exit]
  1: "Then what is that person's name?"

[8615 op=1 keyword; failure -> 8618]
  1: no

[8617 op=2 display+exit]
  1: "Keep asking around."

[8619 op=1 keyword; failure -> 8622]
  1: cono,star

[8624 op=2 display+exit]
  1: "'Tis not fitting that thou shouldst guess."
  2: "Make sure thou dost know the answer before you speak with me again."

[8626 op=1 keyword; failure -> 8628]
  1: look

[8627 op=2 display+exit]
  1: a gentleman of slight build and refined manners.
  2: "Greetings, $P! What brings thee here this $T?"

[8628 op=1 keyword; failure -> 8630]
  1: name

[8629 op=2 display+exit]
  1: "My name is Antonio."

[8630 op=1 keyword; failure -> 8632]
  1: job

[8631 op=2 display+exit]
  1: "I govern @New Magincia."
  2: "Fortunately the fine people of this town have few disputes to settle, leaving me plenty of time for my @hobby."

[8632 op=1 keyword; failure -> 8634]
  1: new,magin

[8633 op=2 display+exit]
  1: "The city of @humility is a lovely place to live."

[8634 op=1 keyword; failure -> 8636]
  1: time,hobb

[8635 op=2 display+exit]
  1: "I do @magic tricks."
  2: "It brings a bit of cheer into this imperfect world."

[8637 op=1 keyword; failure -> 8639]
  1: rune,mant,humi,humb

[8638 op=2 display+exit]
  1: "I wish thee success on thy mission."

[8639 op=1 keyword; failure -> 8641]
  1: rune,mant,humi

[8640 op=2 display+exit]
  1: "The most @humble one amongst us knows the mantra."
  2: "If thou canst find out who that is, tell me, and I will give thee the rune."

[8641 op=1 keyword; failure -> 8644]
  1: magi,tric,chee

[8643 op=2 display+exit]
  1: "Splendid!" Antonio reaches into his pocket and takes out a balloon.
  2: He blows it up until it bursts, and four doves appear from inside it, flying off towards the four points of the compass.
  3: "Did you enjoy that?"

[8644 op=1 keyword; failure -> 8647]
  1: humb

[8646 op=2 display+exit]
  1: "Dost thou know who is the most humble person in this town?"

[8647 op=1 keyword; failure -> 8649]
  1: bye

[8648 op=2 display+exit]
  1: "Until we meet again, may your road be short and your fortunes long."

[8649 op=2 display+exit]
  1: "I cannot help thee with that."

[8653 op=2 display+exit]
  1: "You're carrying too much right now. Come back for it later."

[8655 op=2 display+exit]
  1: "Very astute of thee! Here is the rune, and I wish thee success on thy mission."

```

<a id="anya-npc-13"></a>
## Anya ? NPC 13, port 14

Instruction range: 951?1062

```text
[952 op=1 keyword; failure -> 954]
  1: look

[953 op=2 display+exit]
  1: a buxom woman who looks hard working, yet contented.
  2: "I'm sorry, $G, but I've just an hour to myself today."
  3: "I'll gladly talk with thee later."

[955 op=2 display+exit]
  1: "I'm sorry, $G, but I've just an hour to myself today."
  2: "I'll gladly talk with thee later."

[956 op=1 keyword; failure -> 958]
  1: look

[957 op=2 display+exit]
  1: a buxom woman who looks hard working, yet contented.
  2: "Good $T, $G, and welcome to the Blue Boar Tavern. Art thou here for @business or @pleasure?"

[958 op=1 keyword; failure -> 961]
  1: name

[960 op=2 display+exit]
  1: "I am Anya."

[961 op=1 keyword; failure -> 963]
  1: gyps

[962 op=2 display+exit]
  1: "They pass by Britain occasionally. I believe they travel on the King's @Way."

[963 op=1 keyword; failure -> 965]
  1: king,way

[964 op=2 display+exit]
  1: "The King's Way is the oldest road in Britain."

[965 op=1 keyword; failure -> 967]
  1: job,work

[966 op=2 display+exit]
  1: "I run this tavern to support my @family. The hours are long, but my husband offers what help he can."
  2: "We make enough to pay for my daughter's schooling, so 'tis all worthwhile."

[967 op=1 keyword; failure -> 969]
  1: fami,supp

[968 op=2 display+exit]
  1: "My husband @Matt, and my daughter @Ariana."

[969 op=1 keyword; failure -> 971]
  1: husb,matt

[970 op=2 display+exit]
  1: "He had an accident as a child which left him deaf and dumb, but I love him all the same."
  2: "I only @wish he could hear our daughter play, just once."
  3: "But 'twill ne'er come to pass. He lives in a world of eternal silence."

[971 op=1 keyword; failure -> 974]
  1: acci,deaf,dumb,wish,sile

[973 op=2 display+exit]
  1: Her face brightens suddenly. "You have the look of one who practices the art of magic."
  2: "Perhaps thou couldst find some spell that might restore my husband's hearing!"
  3: "I know 'tis a lot to ask, but wilt thou try?"

[975 op=1 keyword; failure -> 978]
  1: yes

[977 op=2 display+exit]
  1: "Oh, thank you, $G!"

[979 op=1 keyword; failure -> 982]
  1: no

[981 op=2 display+exit]
  1: Her look of hope changes to one of sorrow as she looks down at the floor to hide her embarrassment.
  2: "I'm sorry to have bothered thee," she says. "I'm sure thou hast more important things to do."

[984 op=1 keyword; failure -> 986]
  1: daug,aria

[985 op=2 display+exit]
  1: "Ariana is the light of my life. Seems she had scarce learned to walk when she first showed a gift for music."
  2: "She studies now with the @bards at the Conservatory."

[986 op=1 keyword; failure -> 988]
  1: musi,bard,cons

[987 op=2 display+exit]
  1: "Bless their hearts for teaching her well. She'll be no tavern wench when she grows older."
  2: "Perhaps she'll even be court musician to Lord British some day..."

[988 op=1 keyword; failure -> 990]
  1: mant

[989 op=2 display+exit]
  1: "Thou shouldst ask my daughter of that. Methinks the bards have taught her of it."

[991 op=1 keyword; failure -> 994]
  1: rune,comp,perm,borr

[993 op=2 display+exit]
  1: "Thou dost wish to borrow the Rune of Compassion from my daughter?" She pauses a while in thought.
  2: "Very well--thou dost have an honest face. I grant thee my permission."

[995 op=1 keyword; failure -> 997]
  1: rune,comp,perm,borr

[996 op=2 display+exit]
  1: "I already gave thee my permission to borrow the rune!"
  2: "Do thou be careful not to lose the stone or let any harm befall it. Ariana would never forgive herself."

[997 op=1 keyword; failure -> 999]
  1: rune,comp,perm,borr

[998 op=2 display+exit]
  1: "Ah, twas the proudest moment of my life when they awarded my dear little Ariana the Rune of Compassion."
  2: "They said she was the most promising student they'd ever had at the Conservatory."
  3: "I'm sure she would show thee the rune, if thou wouldst care to see it."

[999 op=1 keyword; failure -> 1001]
  1: plea

[1000 op=2 display+exit]
  1: "Good! Perhaps thou might stay and talk with me a while, before I'm back about my work."
  2: "'Tis good to talk with thee again. Things have been slow around here since the @gypsies left."

[1001 op=1 keyword; failure -> 1003]
  1: buy,sell,busi

[1002 op=2 display+exit]
  1: "Wouldst thou like @ale, @mead, @wine, @rations, or perhaps a @cake?"

[1003 op=1 keyword; failure -> 1008]
  1: ale

[1007 op=2 display+exit]
  1: "That'll be 3 crowns, $Y. Wouldst thou like some?"

[1008 op=1 keyword; failure -> 1013]
  1: mead

[1012 op=2 display+exit]
  1: "That'll be 4 crowns, $Y. Wouldst thou like some?"

[1013 op=1 keyword; failure -> 1018]
  1: wine

[1017 op=2 display+exit]
  1: "That'll be 6 crowns, $Y. Wouldst thou like some?"

[1018 op=1 keyword; failure -> 1021]
  1: rati

[1020 op=2 display+exit]
  1: "My price is 4 gold for each ration. How many do you want, $Y?"

[1021 op=1 keyword; failure -> 1026]
  1: cake

[1025 op=2 display+exit]
  1: "It'll be 8 gold for the cake. Is that alright?"

[1029 op=1 keyword; failure -> 1031]
  1: yes

[1032 op=1 keyword; failure -> 1035]
  1: no

[1034 op=2 display+exit]
  1: "Anything else I can do for thee?"

[1037 op=1 keyword; failure -> 1039]
  1: bye

[1038 op=2 display+exit]
  1: "Well, 'tis time I was back about my work. Dishes to wash, food to cook, drinks to serve..."
  2: "Come back any time."

[1040 op=1 keyword; failure -> 1042]
  1: bye

[1041 op=2 display+exit]
  1: "I thank thee for stopping by to brighten my day with thy conversation. Do thou come again."

[1042 op=2 display+exit]
  1: "I cannot help thee with that."

[1045 op=2 display+exit]
  1: "Anything else I can do for thee?"

[1047 op=2 display+exit]
  1: "You haven't any room in your pack."

[1052 op=2 display+exit]
  1: "I'm sorry, but you don't have enough gold."

[1054 op=2 display+exit]
  1: "You haven't any room in your pack."

[1055 op=2 display+exit]
  1: She hands you the mutton.

[1059 op=2 display+exit]
  1: "I'm sorry, but you don't have enough gold."

[1061 op=2 display+exit]
  1: "You don't have room to carry it."

[1062 op=2 display+exit]
  1: "Fine!"
  2: After accepting the gold, she hands over the purchase.
  3: "I hope thou dost enjoy it!"

```

<a id="arbeth-npc-101"></a>
## Arbeth ? NPC 101, port 102

Instruction range: 9463?9605

```text
[9465 op=1 keyword; failure -> 9468]
  1: yes

[9467 op=2 display+exit]
  1: "How many do you want to sell me?"

[9470 op=2 display+exit]
  1: "What do you want from me?"

[9476 op=1 keyword; failure -> 9479]
  1: yes

[9478 op=2 display+exit]
  1: "Good, how many do you need?"

[9481 op=2 display+exit]
  1: "What do you want from me?"

[9483 op=1 keyword; failure -> 9485]
  1: yes

[9487 op=2 display+exit]
  1: "What do you want from me?"

[9489 op=1 keyword; failure -> 9492]
  1: yes

[9491 op=2 display+exit]
  1: "'Tis a most difficult job, $Y."
  2: "It'll cost thee 20 gold!"
  3: "Still interested?"

[9494 op=2 display+exit]
  1: "What do you want from me?"

[9495 op=1 keyword; failure -> 9497]
  1: name

[9496 op=2 display+exit]
  1: He looks around at the ground and mumbles "Arbeth."

[9497 op=1 keyword; failure -> 9499]
  1: arbe

[9498 op=2 display+exit]
  1: "Arbeth," he repeats quietly.

[9499 op=1 keyword; failure -> 9501]
  1: job,shop

[9500 op=2 display+exit]
  1: "Well," he says so quietly you have to lean close to hear, "I make @wool into @thread."

[9501 op=1 keyword; failure -> 9503]
  1: thre

[9502 op=2 display+exit]
  1: "Yes," he says, wringing his hands together.

[9503 op=1 keyword; failure -> 9505]
  1: weav

[9504 op=2 display+exit]
  1: "I'm not a weaver, $G, just a spinner."
  2: "Talk to @Thindle."

[9505 op=1 keyword; failure -> 9507]
  1: thin

[9506 op=2 display+exit]
  1: "He lives here in Paws and is a fine weaver."

[9507 op=1 keyword; failure -> 9509]
  1: bye

[9508 op=2 display+exit]
  1: "Bye," he whispers.

[9509 op=1 keyword; failure -> 9511]
  1: timo

[9510 op=2 display+exit]
  1: "He is nice to me."
  2: He stops wringing his hands and seems to relax momentarily.

[9511 op=1 keyword; failure -> 9513]
  1: mari,dori

[9512 op=2 display+exit]
  1: "She's alright, I guess."

[9513 op=1 keyword; failure -> 9515]
  1: gris,hend,uber

[9514 op=2 display+exit]
  1: "He's alright, I guess."

[9515 op=1 keyword; failure -> 9517]
  1: meri

[9516 op=2 display+exit]
  1: A smile crosses his face.
  2: "I like her."
  3: "She brought me cookies once."

[9519 op=1 keyword; failure -> 9521]
  1: look

[9520 op=2 display+exit]
  1: a frightened little man who never looks you in the eye.
  2: He looks up from his mug of ale.
  3: "Hello."

[9521 op=1 keyword; failure -> 9523]
  1: mort

[9522 op=2 display+exit]
  1: "He's much too @loud."
  2: He looks up suddenly and his face goes ashen.

[9523 op=1 keyword; failure -> 9525]
  1: loud

[9524 op=2 display+exit]
  1: "But...I mean...maybe not too loud."
  2: He whips his head around as if he's looking for someplace to hide.

[9525 op=1 keyword; failure -> 9527]
  1: buy,spid,silk,plan,ball

[9526 op=2 display+exit]
  1: "Not now, $G," he says timidly.
  2: "My shop is closed."

[9527 op=2 display+exit]
  1: He twists his shoulders around and holds his palms up.

[9528 op=1 keyword; failure -> 9532]
  1: look

[9531 op=2 display+exit]
  1: a frightened little man who never looks you in the eye.
  2: "Yes?" he asks, looking like he expects to be beaten any minute.

[9533 op=1 keyword; failure -> 9536]
  1: look

[9535 op=2 display+exit]
  1: a frightened little man who never looks you in the eye.
  2: "Oh, it's you. Hello."

[9536 op=1 keyword; failure -> 9538]
  1: look

[9537 op=2 display+exit]
  1: a frightened little man who never looks you in the eye.
  2: "Yes? Oh, it's you."
  3: He almost looked you in the eyes then.
  4: "What do you want from me?"

[9538 op=1 keyword; failure -> 9540]
  1: mort

[9539 op=2 display+exit]
  1: "He's much too @loud."

[9540 op=1 keyword; failure -> 9542]
  1: loud

[9541 op=2 display+exit]
  1: "But...I mean...maybe not too loud."

[9542 op=1 keyword; failure -> 9545]
  1: buy,thre

[9544 op=2 display+exit]
  1: "Will you pay 3 gold for each spool of thread?"

[9545 op=1 keyword; failure -> 9555]
  1: sell,wool

[9555 op=1 keyword; failure -> 9558]
  1: sell,wool

[9557 op=2 display+exit]
  1: "Will you take $V1 gold for each bale of wool?"

[9558 op=1 keyword; failure -> 9561]
  1: spid,silk,plan,ball

[9560 op=2 display+exit]
  1: "What?"
  2: "Oh, yes, I could spin spidersilk into thread."
  3: "Do you want me to?"

[9561 op=2 display+exit]
  1: He twists his shoulders around and holds his palms up.

[9565 op=2 display+exit]
  1: "You haven't the gold."

[9568 op=2 display+exit]
  1: "Good, good!"
  2: He seems much brighter and happier now.
  3: "Oh, but I'll need twoscore bits of spidersilk to do it!"
  4: "Come back when thou hast 40 pieces of spidersilk and I'll make thy thread then."

[9570 op=2 display+exit]
  1: "Good, good!"
  2: He seems much brighter and happier now.
  3: Arbeth takes $Y's gold and silk and starts spinning...
  4: Soon he has finished a spool of fine silk thread!
  5: "There it is, $Y, my finest work!"
  6: "A true pleasure!"
  7: Arbeth hands you the spool of silk.
  8: "You'll have to find a @weaver, of course..."

[9573 op=2 display+exit]
  1: "What do you want from me?"

[9575 op=2 display+exit]
  1: "Oh, but you'll not be able to carry it, $Y!"

[9580 op=2 display+exit]
  1: "You haven't the gold."

[9582 op=2 display+exit]
  1: "Oh, but you'll not be able to carry it, $Y!"

[9583 op=2 display+exit]
  1: After accepting your gold, he hands over the thread.

[9586 op=2 display+exit]
  1: "What do you want from me?"

[9588 op=2 display+exit]
  1: "Oh, but I need no more than 10."

[9602 op=2 display+exit]
  1: "But you don't have the wool!"

[9605 op=2 display+exit]
  1: He hands you $V1 gold and takes the wool.

```

<a id="ariana-npc-11"></a>
## Ariana ? NPC 11, port 12

Instruction range: 851?942

```text
[854 op=1 keyword; failure -> 856]
  1: look

[855 op=2 display+exit]
  1: a cute little girl with short hair.
  2: "My parents told me never to talk to strangers. What's your name?"

[856 op=2 display+exit]
  1: "My parents told me never to talk to strangers. What's your name?"

[866 op=2 display+exit]
  1: "Ok. Now I can talk to you!"

[867 op=1 keyword; failure -> 869]
  1: look

[868 op=2 display+exit]
  1: a cute little girl with short hair.
  2: "Oh, hello again."

[869 op=1 keyword; failure -> 872]
  1: name

[871 op=2 display+exit]
  1: She curtsies gracefully.
  2: "Ariana is my name."

[872 op=1 keyword; failure -> 874]
  1: job

[873 op=2 display+exit]
  1: "I'm learning to be a @bard. I like to @play the harpsichord best."

[874 op=1 keyword; failure -> 876]
  1: bard

[875 op=2 display+exit]
  1: "I want to be a bard when I grow up. They have more @fun than anybody!"

[876 op=1 keyword; failure -> 878]
  1: comp

[877 op=2 display+exit]
  1: "They taught me all about compassion. They say it's very important for bards."

[878 op=1 keyword; failure -> 880]
  1: mant

[879 op=2 display+exit]
  1: "I can't remember it now... And @Kenneth just taught it to me last week..."
  2: "Wait, I know! It's 'mo!'"
  3: "Or maybe it was 'om'... Or 'mu,' or 'um'... It was something like that."

[880 op=1 keyword; failure -> 883]
  1: rune

[882 op=2 display+exit]
  1: "I already gave it to you, silly. I hope you can figure out how to use it."

[884 op=1 keyword; failure -> 887]
  1: rune

[886 op=2 display+exit]
  1: "Hi! Did you get permission from my parents to borrow the rune?"

[888 op=1 keyword; failure -> 890]
  1: no

[889 op=2 display+exit]
  1: "Well, if you get their permission later, let me know."

[891 op=1 keyword; failure -> 893]
  1: yes

[895 op=1 keyword; failure -> 898]
  1: rune

[897 op=2 display+exit]
  1: "I have the Rune of Compassion. Isn't it pretty?" She reaches into her pocket and takes it out to show you.
  2: "Do you need it for something?"

[899 op=1 keyword; failure -> 902]
  1: yes

[901 op=2 display+exit]
  1: "Ok."
  2: "Are you on an important quest?"

[904 op=2 display+exit]
  1: "Ok."
  2: "I'd best hold onto it then. I was told to look after it carefully."

[906 op=1 keyword; failure -> 910]
  1: yes

[909 op=2 display+exit]
  1: "I'm not supposed to let anyone else have it. But if you truly need it..."
  2: "I don't know what I should do."
  3: "I know! My parents run the Blue Boar Tavern. Go ask them if you can borrow the rune."
  4: "If they say 'tis okay, I'll give it to you."

[912 op=2 display+exit]
  1: "I'd best hold onto it then. I was told to look after it carefully."

[913 op=1 keyword; failure -> 916]
  1: song,music,play,harp

[915 op=2 display+exit]
  1: "I've been studying to be a bard since I was this many." She holds up three fingers.
  2: "I'd like to play something for you, but I'm not good enough yet."
  3: She looks away for a moment, then turns back to you, blushing slightly.
  4: "Will you come back to hear my music when I am older, if you can?"

[917 op=1 keyword; failure -> 920]
  1: no

[919 op=2 display+exit]
  1: The little girl sniffles, seeming suddenly on the verge of tears.
  2: "I'm sorry," she says, "I shouldn't have asked." She turns her back, as if ashamed to face you.

[921 op=1 keyword; failure -> 924]
  1: yes

[923 op=2 display+exit]
  1: The little girl's face breaks out into a smile bright enough to light up the whole room.
  2: "Really? I would like that very much."

[926 op=1 keyword; failure -> 928]
  1: fun

[927 op=2 display+exit]
  1: "Oh yes, lots of it!"

[928 op=1 keyword; failure -> 930]
  1: ken

[929 op=2 display+exit]
  1: "He's my teacher."

[930 op=1 keyword; failure -> 932]
  1: bye

[931 op=2 display+exit]
  1: "See you later."

[932 op=2 display+exit]
  1: "What do you mean?"

[937 op=2 display+exit]
  1: "Oh, you're carrying too much right now. You'd better come back for it later."

[942 op=2 display+exit]
  1: "Then here you are." She hands you the rune. "I wish you the best of luck on your quest."

```

<a id="artegal-npc-121"></a>
## Artegal ? NPC 121, port 122

Instruction range: 11299?11306

```text
[11299 op=1 keyword; failure -> 11301]
  1: look

[11300 op=2 display+exit]
  1: a heavily scarred man with a look of agony on his face.
  2: "The pain, the pain..."
  3: "Will it never stop?"

[11303 op=2 display+exit]
  1: "My head is pounding like a drum..."

[11305 op=2 display+exit]
  1: "Oh, the pain!"

[11306 op=2 display+exit]
  1: "Too many..."
  2: "There were too many of them..."

```

<a id="arturos-npc-136"></a>
## Arturos ? NPC 136, port 137

Instruction range: 12477?12649

```text
[12478 op=1 keyword; failure -> 12483]
  1: yes

[12482 op=2 display+exit]
  1: He takes out a deck of cards, shuffles it, and has you pick one.
  2: "Okay, thisa card, she'sa the Tower."
  3: "That means you'rea going to go into a real big building soon, like maybe a castle."
  4: "Pretty good, huh? Maybe you want a better card, though."
  5: "Do you want to pick again?"

[12484 op=1 keyword; failure -> 12487]
  1: yes

[12486 op=2 display+exit]
  1: "Come back whena you have the money."

[12489 op=2 display+exit]
  1: "Ok, have it your way."
  2: "But ifa some maniac with an axe catches you in a dark alley somewhere,
  3: don'ta blame me for not warning you."

[12491 op=1 keyword; failure -> 12496]
  1: yes

[12495 op=2 display+exit]
  1: You pick another card.
  2: "This here isa the Wheel of Fortune."
  3: "Wheels, they go 'round and 'round, so that means you'rea gonna have some good luck, and some bad luck."
  4: "That doesn't tell you so much."
  5: "Try for another card?"

[12497 op=1 keyword; failure -> 12500]
  1: yes

[12499 op=2 display+exit]
  1: "Come back whena you have the money."

[12502 op=2 display+exit]
  1: "Okay."

[12505 op=1 keyword; failure -> 12510]
  1: yes

[12509 op=2 display+exit]
  1: "Oh, hey, you picked the Lovers this time."
  2: "I thinka maybe you should go talk to Andreas, you know what I mean?"
  3: "That'sa pretty good card."
  4: "How about another one?"

[12511 op=1 keyword; failure -> 12516]
  1: yes

[12515 op=2 display+exit]
  1: "Oh, hey, you picked the Lovers this time."
  2: "I thinka maybe you should go talk to Wanda, you know what I mean?"
  3: "That'sa pretty good card."
  4: "How about another one?"

[12517 op=1 keyword; failure -> 12520]
  1: yes

[12519 op=2 display+exit]
  1: "Come back whena you have the money."

[12522 op=2 display+exit]
  1: "Okay, that'sa 1 extra card at 5 gold apiece."
  2: "That makes 7 gold in total."
  3: "It was a pleasure doinga business with you."

[12524 op=1 keyword; failure -> 12529]
  1: yes

[12528 op=2 display+exit]
  1: "This here's Temperance."
  2: "Thata means that you shouldn't lose your temper."
  3: "I cana find you a better card than that, here, wait a minute."
  4: He picks out a card.
  5: "This is Justice, that'sa one of what they call the 'court cards.'"
  6: "Let me try again." He picks another.
  7: "Okay, this isa better."
  8: "This is the Hierophant."
  9: "That'sa like an elephant, but taller and without the trunk."
  10: "That means you'rea gonna see one of them someday."
  11: "Pretty interesting, huh? Another card?"

[12530 op=1 keyword; failure -> 12533]
  1: yes

[12532 op=2 display+exit]
  1: "Come back whena you have the money."

[12535 op=2 display+exit]
  1: "Okay, that'sa 2 extra cards at 5 gold apiece."
  2: "That makes 12 gold in total."
  3: "It was a pleasure doinga business with you."

[12537 op=1 keyword; failure -> 12542]
  1: yes

[12541 op=2 display+exit]
  1: "Uh-oh, you justa drew the Death card."
  2: "That means eithera you're going to kill somebody soon, ora somebody's gonna kill you."
  3: "I can never geta that part straight."
  4: "One more card?"

[12543 op=1 keyword; failure -> 12546]
  1: yes

[12545 op=2 display+exit]
  1: "Come back whena you have the money."

[12548 op=2 display+exit]
  1: "Okay, that'sa 3 extra cards at 5 gold apiece."
  2: "That makes 17 gold in total."
  3: "It was a pleasure doinga business with you."

[12551 op=1 keyword; failure -> 12557]
  1: yes

[12556 op=2 display+exit]
  1: "Allaright, now we're talking."
  2: "You pickeda the Crow card."
  3: "That'sa telling you adventuring isa for the birds."
  4: "You should open a little shop somewhere and settle down, you know?"
  5: "The cards didn't say nothin' about the silver tablet."
  6: "Guess that means you're not gonna find it."
  7: "That's life, you know?"
  8: "Okay, that'sa 5 extra cards at 5 gold apiece."
  9: "That makes 27 gold in total."
  10: "It was a pleasure doinga business with you."

[12558 op=1 keyword; failure -> 12563]
  1: yes

[12562 op=2 display+exit]
  1: "Allaright, now we're talking."
  2: "You pickeda the Crow card."
  3: "That'sa telling you adventuring isa for the birds."
  4: "You should open a little shop somewhere and settle down, you know?"
  5: "Okay, that'sa 5 extra cards at 5 gold apiece."
  6: "That makes 27 gold in total."
  7: "It was a pleasure doinga business with you."

[12564 op=1 keyword; failure -> 12567]
  1: yes

[12566 op=2 display+exit]
  1: "Come back whena you have the money."

[12569 op=2 display+exit]
  1: "Okay, that'sa 4 extra cards at 5 gold apiece."
  2: "That makes 22 gold in total."
  3: "It was a pleasure doinga business with you."

[12571 op=1 keyword; failure -> 12574]
  1: yes

[12573 op=2 display+exit]
  1: "Oh! Well, sure you do, she's a very important map!"
  2: He leads you away from the rest of the group.
  3: "Don't want no spyin' ears, ah?"
  4: "Yeah, this map, she'sa leading to the treasure of - oh, who was it, Yellowbeard or somebody."
  5: "Well, I forget, but the point is it leads to a big, huge treasure!"
  6: "So if you want this map you're agonna have to pay - 100 gold! Okay?"

[12576 op=2 display+exit]
  1: "Nah, I didn't think so."
  2: "What for would anybody want to buy it, ah?"

[12578 op=1 keyword; failure -> 12581]
  1: yes

[12584 op=2 display+exit]
  1: "Thank you!"
  2: "You have asaved me from amaking a big mistake!"
  3: "You're right, I better hold on to this valuable map myself."
  4: "No way is it worth less than 100 gold."

[12587 op=2 display+exit]
  1: "Well, I hate to see anybody pass up good fortune, so I tell you what I do."
  2: "I sell you this map, this key to infinite wealth, for just 75 gold. Okay?"

[12589 op=1 keyword; failure -> 12592]
  1: yes

[12594 op=2 display+exit]
  1: "What, are you nuts?"
  2: "But okay, how'sabout just 50 gold?"

[12596 op=1 keyword; failure -> 12599]
  1: yes

[12602 op=2 display+exit]
  1: "Thank you!"
  2: "You have asaved me from amaking a big mistake!"
  3: "You're right, I better hold on to this valuable map myself."
  4: "No way is it worth less than 100 gold."

[12603 op=1 keyword; failure -> 12605]
  1: look

[12604 op=2 display+exit]
  1: a colorfully clad, bemused looking gypsy.
  2: "Hey, paisan! If you were looking to geta your @fortune told, you came to the right place."

[12605 op=1 keyword; failure -> 12607]
  1: name

[12606 op=2 display+exit]
  1: "I'ma called Arturos."
  2: "That meansa either 'foolish monkey' or 'he who sells cheese', depending on howa you @translate it."

[12607 op=1 keyword; failure -> 12609]
  1: fool,monk,chee,tran

[12608 op=2 display+exit]
  1: "Yeah, go figure, huh?"

[12609 op=1 keyword; failure -> 12611]
  1: job

[12610 op=2 display+exit]
  1: "Ima the king of all the gypsies."
  2: "Of course, thatsa what you call more of an 'honorary' @title, so I tell @fortunes on the side."

[12611 op=1 keyword; failure -> 12613]
  1: hono,titl,king,gyps,artu

[12612 op=2 display+exit]
  1: "You cana just call me '@Uncle @Arthur' if you like."

[12613 op=1 keyword; failure -> 12615]
  1: zolt

[12614 op=2 display+exit]
  1: "He's a whadda you call, an impostor."
  2: "You shouldn'ta listen to him."

[12615 op=1 keyword; failure -> 12621]
  1: uncl,arth

[12618 op=22 display+continue]
  1: "That'sa nice and friendly."
  2: "I like you too."
  3: He comes over and gives you a big hug.

[12620 op=2 display+exit]
  1: Shamino whispers to you, "Best check your coinpouch."
  2: "I think I saw something when that gypsy hugged you."

[12621 op=1 keyword; failure -> 12624]
  1: uncl,arth

[12623 op=2 display+exit]
  1: "That'sa nice and friendly."
  2: "I like you too."
  3: He comes over and gives you a big hug.

[12624 op=1 keyword; failure -> 12628]
  1: tell,fort,card

[12627 op=2 display+exit]
  1: "I gota this fine set of fortune telling cards."
  2: "These werea hand painted by Musgrave himself."
  3: "For justa two gold, maybe I can help you finda this silver tablet."
  4: "And I'll tell you the rest of your future as well."
  5: "What a bargain! Whadda you say?"

[12628 op=1 keyword; failure -> 12631]
  1: tell,fort,card

[12630 op=2 display+exit]
  1: "I gota this fine set of fortune telling cards."
  2: "These werea hand painted by Musgrave himself."
  3: "For justa two gold, I'll tella you your future."
  4: "Whadda you say?"

[12631 op=1 keyword; failure -> 12634]
  1: silv,tabl

[12633 op=2 display+exit]
  1: "The silver tablet? I don't know anything about that."
  2: "But hey, maybe there's something in the @cards about it, ah?"

[12634 op=1 keyword; failure -> 12637]
  1: map

[12636 op=2 display+exit]
  1: "Yeah, I used to have a map like that."
  2: "Can't remember what I did with it, though..."

[12637 op=1 keyword; failure -> 12640]
  1: map

[12639 op=2 display+exit]
  1: "Map. Hmm, map... Oh yeah, thata old thing."
  2: "Sure, I got that, but she'sa not very interesting."
  3: "Say--do you want to buy it?"

[12640 op=1 keyword; failure -> 12642]
  1: bye

[12641 op=2 display+exit]
  1: "Hey, you come again soon, okay?"

[12642 op=2 display+exit]
  1: "Can'ta you think of something more interesting to talk about?"

[12647 op=2 display+exit]
  1: "Hey, what are you, a deadbeat?"
  2: "Come back when you can afford the map!"

[12649 op=2 display+exit]
  1: "Okay, here's the map, and may fortune bring you lotsa gold!"

```

<a id="arty-npc-26"></a>
## Arty ? NPC 26, port 27

Instruction range: 2184?2264

```text
[2185 op=1 keyword; failure -> 2187]
  1: yes

[2189 op=2 display+exit]
  1: He shrugs.

[2191 op=1 keyword; failure -> 2193]
  1: look

[2192 op=2 display+exit]
  1: a silent but good-natured man.
  2: He looks up from his tankard of ale, sees you and smiles.

[2193 op=1 keyword; failure -> 2195]
  1: look

[2194 op=2 display+exit]
  1: a silent but good-natured man.
  2: He grins widely and waves at you.

[2195 op=1 keyword; failure -> 2201]
  1: name

[2198 op=22 display+continue]
  1: He points to his mouth and shakes his head.
  2: The man smiles and nods.

[2200 op=2 display+exit]
  1: Dupre speaks.
  2: "I know of this person, $G."
  3: "He is Arty the Mute, a local shipwright and an honored citizen."

[2201 op=1 keyword; failure -> 2207]
  1: name

[2204 op=22 display+continue]
  1: He points to his mouth and shakes his head.
  2: The man smiles and nods.

[2206 op=2 display+exit]
  1: Shamino speaks.
  2: "I know of this person, $G."
  3: "He is Arty the Mute, a local shipwright and an honored citizen."

[2207 op=1 keyword; failure -> 2213]
  1: name

[2210 op=22 display+continue]
  1: He points to his mouth and shakes his head.
  2: The man smiles and nods.

[2212 op=2 display+exit]
  1: Iolo speaks.
  2: "I know of this person, $G."
  3: "He is Arty the Mute, a local shipwright and an honored citizen."

[2213 op=1 keyword; failure -> 2215]
  1: name

[2214 op=2 display+exit]
  1: He points to his mouth and shakes his head.

[2215 op=1 keyword; failure -> 2217]
  1: mute

[2216 op=2 display+exit]
  1: Arty smiles and nods.

[2217 op=1 keyword; failure -> 2219]
  1: deaf

[2218 op=2 display+exit]
  1: The man shakes his head.

[2220 op=1 keyword; failure -> 2222]
  1: buy

[2221 op=2 display+exit]
  1: The man looks at you, puzzled.
  2: He shakes his head and spreads his hands as if to say "sorry."

[2223 op=1 keyword; failure -> 2225]
  1: ship

[2224 op=2 display+exit]
  1: The man looks at you, puzzled.
  2: He shakes his head and spreads his hands as if to say "sorry."

[2226 op=1 keyword; failure -> 2228]
  1: skif

[2227 op=2 display+exit]
  1: The man looks at you, puzzled.
  2: He shakes his head and spreads his hands as if to say "sorry."

[2229 op=1 keyword; failure -> 2231]
  1: job

[2230 op=2 display+exit]
  1: The man looks at you, puzzled.
  2: Then he raises his tankard and goes back to drinking.

[2231 op=1 keyword; failure -> 2233]
  1: job

[2232 op=2 display+exit]
  1: He smiles and points to the workbench.
  2: On the bench, you see the tools of a shipwright.

[2233 op=1 keyword; failure -> 2235]
  1: buy

[2234 op=2 display+exit]
  1: The man smiles and points to the deed to a @ship and to a @skiff.

[2235 op=1 keyword; failure -> 2238]
  1: ship

[2237 op=2 display+exit]
  1: He shows you a deed.
  2: Written on it is the price, 300 gold.
  3: Do you want to buy the deed?

[2238 op=1 keyword; failure -> 2241]
  1: skif

[2240 op=2 display+exit]
  1: He shows you a deed.
  2: Written on it is the price, 40 gold.
  3: Do you want to buy the deed?

[2241 op=1 keyword; failure -> 2244]
  1: bye

[2243 op=2 display+exit]
  1: Arty waves as you leave, then returns to his ale.

[2244 op=1 keyword; failure -> 2246]
  1: bye

[2245 op=2 display+exit]
  1: Arty waves as you leave, then returns to what he was doing.

[2249 op=2 display+exit]
  1: The man just shrugs and takes another swig from his mug.

[2251 op=2 display+exit]
  1: The man just shrugs.

[2252 op=2 display+exit]
  1: The man seems frustrated, as if there were something he wanted to say.

[2261 op=2 display+exit]
  1: You don't have enough gold for that.

[2263 op=2 display+exit]
  1: You cannot carry that.

[2264 op=2 display+exit]
  1: After accepting the gold, He smiles and hands you the deed.

```

<a id="arvin-npc-49"></a>
## Arvin ? NPC 49, port 50

Instruction range: 4358?4432

```text
[4359 op=1 keyword; failure -> 4361]
  1: look

[4360 op=2 display+exit]
  1: a tavernkeeper.
  2: The man looks up from his cooking. "Aye, hello there."
  3: 'Fraid I can't talk now. I have to cook.
  4: "Come back when it's ready and have some!"

[4362 op=2 display+exit]
  1: The man looks up from his cooking. "Aye, hello there."
  2: 'Fraid I can't talk now. I have to cook.
  3: "Come back when it's ready and have some!"

[4363 op=1 keyword; failure -> 4365]
  1: look

[4364 op=2 display+exit]
  1: a tavernkeeper.
  2: "Welcome to the Sword and Keg."

[4365 op=1 keyword; failure -> 4368]
  1: name

[4367 op=2 display+exit]
  1: "I be Arvin, mate."

[4368 op=1 keyword; failure -> 4370]
  1: buy,sell,busi

[4369 op=2 display+exit]
  1: "Which, @rolls, @ale, @mead, @wine, or @rations?"

[4370 op=1 keyword; failure -> 4372]
  1: job

[4371 op=2 display+exit]
  1: "I sell @rolls, @ale, @mead, @wine, and @rations."

[4372 op=1 keyword; failure -> 4377]
  1: roll

[4376 op=2 display+exit]
  1: "That'll be 2 crowns, okay?"

[4377 op=1 keyword; failure -> 4382]
  1: ale

[4381 op=2 display+exit]
  1: "That'll be 3 crowns, okay?"

[4382 op=1 keyword; failure -> 4387]
  1: mead

[4386 op=2 display+exit]
  1: "That'll be 4 crowns, okay?"

[4387 op=1 keyword; failure -> 4392]
  1: wine

[4391 op=2 display+exit]
  1: "That'll be 6 crowns, okay?"

[4392 op=1 keyword; failure -> 4395]
  1: rati

[4394 op=2 display+exit]
  1: "Rations are 4 gold apiece. How many do you want?"

[4395 op=1 keyword; failure -> 4397]
  1: bye

[4396 op=2 display+exit]
  1: "See you later."

[4400 op=1 keyword; failure -> 4402]
  1: yes

[4403 op=1 keyword; failure -> 4406]
  1: no

[4405 op=2 display+exit]
  1: "Anything else I can do for you?"

[4406 op=2 display+exit]
  1: "I can't help you with that."

[4409 op=2 display+exit]
  1: "Anything else I can do for you?"

[4411 op=2 display+exit]
  1: "You don't have room to carry it."

[4416 op=2 display+exit]
  1: "Come back when you have some gold."

[4418 op=2 display+exit]
  1: "You don't have room to carry it."

[4419 op=2 display+exit]
  1: Arvin hands the rations to $Y.

[4423 op=2 display+exit]
  1: "Come back when you have some gold."

[4425 op=2 display+exit]
  1: "You don't have room to carry it."

[4427 op=2 display+exit]
  1: Arvin gives $Y the rolls
  2: "They're good with butter, ya know."

[4429 op=2 display+exit]
  1: "All right." Arvin gives $Y the ale.

[4431 op=2 display+exit]
  1: "All right." Arvin gives $Y the mead.

[4432 op=2 display+exit]
  1: "All right." Arvin gives $Y the wine.

```

<a id="aurendir-npc-96"></a>
## Aurendir ? NPC 96, port 97

Instruction range: 9026?9084

```text
[9029 op=1 keyword; failure -> 9032]
  1: yes

[9031 op=2 display+exit]
  1: "I charge 5 gold for each bale of wool."
  2: "How many do you need?"

[9034 op=2 display+exit]
  1: "Okay."

[9035 op=1 keyword; failure -> 9037]
  1: look

[9036 op=2 display+exit]
  1: a short man dressed in shepherds clothing.
  2: "Hi there!"
  3: "Is there anything I can do for you?"

[9037 op=1 keyword; failure -> 9040]
  1: name

[9039 op=2 display+exit]
  1: "Once I was Aurendir the mighty - now I live a more @modest life."

[9040 op=1 keyword; failure -> 9042]
  1: job,life,shep

[9041 op=2 display+exit]
  1: "I came here to @meditate, tend @sheep and sell @wool."

[9042 op=1 keyword; failure -> 9044]
  1: tend,shee

[9043 op=2 display+exit]
  1: "So they can be @shorn."

[9044 op=1 keyword; failure -> 9046]
  1: shor

[9045 op=2 display+exit]
  1: "To make @wool."
  2: "People @buy it from me."

[9046 op=1 keyword; failure -> 9048]
  1: wool

[9047 op=2 display+exit]
  1: "It's used to make @thread."

[9048 op=1 keyword; failure -> 9051]
  1: buy

[9050 op=2 display+exit]
  1: "Would you like some wool?"

[9051 op=1 keyword; failure -> 9053]
  1: thre

[9052 op=2 display+exit]
  1: "To be woven into @cloth."

[9053 op=1 keyword; failure -> 9055]
  1: wove,clot

[9054 op=2 display+exit]
  1: "You can make @robes from it."

[9055 op=1 keyword; failure -> 9057]
  1: robe

[9056 op=2 display+exit]
  1: "I wear a robe while I tend @sheep."
  2: "It's all part of life's great @circle."

[9057 op=1 keyword; failure -> 9059]
  1: life,circ

[9058 op=2 display+exit]
  1: "From earth, to grass, to sheep, to me..."
  2: "when I die, I'll return to the @earth."

[9059 op=1 keyword; failure -> 9061]
  1: die,buri,back,eart,gras

[9060 op=2 display+exit]
  1: "That's right."

[9061 op=1 keyword; failure -> 9063]
  1: once,migh,mode,humi,humb

[9062 op=2 display+exit]
  1: "I used to be a powerful and wealthy mage."
  2: "I wore silk robes, and fine jewelry."
  3: "In fact, I even had my own @castle."
  4: "But one day in my travels I visited the @Shrine of Humility."
  5: "Meditating there, I had an idea."
  6: "I would give up all my earthly possessions and quit the practice of magic."
  7: "I would live as a shepherd!"
  8: "Surely this would be the greatest act of humility a man could perform."
  9: "And so you see me here today."

[9063 op=1 keyword; failure -> 9065]
  1: powe,weal,mage,silk,jewe,cast,magi

[9064 op=2 display+exit]
  1: "Those days are no more."

[9065 op=1 keyword; failure -> 9067]
  1: shri

[9066 op=2 display+exit]
  1: "I should go there again soon..."
  2: "It changed my life."

[9067 op=1 keyword; failure -> 9069]
  1: medi,mant

[9068 op=2 display+exit]
  1: "You should go meditate at the shrine yourself."
  2: "I find 'lum' a most effective mantra."

[9069 op=1 keyword; failure -> 9071]
  1: bye

[9070 op=2 display+exit]
  1: "See you later."

[9071 op=2 display+exit]
  1: "Beats me."

[9074 op=2 display+exit]
  1: "Okay."

[9076 op=2 display+exit]
  1: "You look pretty full to me."

[9081 op=2 display+exit]
  1: "I'm sorry, but you don't seem to have enough money."

[9083 op=2 display+exit]
  1: "You look pretty full to me."

[9084 op=2 display+exit]
  1: "Very good."
  2: After accepting your gold, he hands over the wool.
  3: "It was a pleasure doing business with you."

```

<a id="beh-lem-npc-162"></a>
## Beh Lem ? NPC 162, port 163

Instruction range: 14626?14775

```text
[14627 op=1 keyword; failure -> 14630]
  1: yes

[14629 op=2 display+exit]
  1: The confused gargoyle waves good-bye.

[14633 op=2 display+exit]
  1: "As you wish, $G."

[14635 op=1 keyword; failure -> 14638]
  1: yes

[14637 op=2 display+exit]
  1: "Good!"
  2: "To know that he very much wanted to speak with you."

[14640 op=2 display+exit]
  1: "To wish @directions to the house of Valkadesh?"

[14642 op=1 keyword; failure -> 14645]
  1: yes

[14644 op=2 display+exit]
  1: "To go from the entrance to Hythloth westward, following the @mountains."
  2: "Then to turn north, at the @Hall of Knowledge."
  3: "To walk east along the mountains until you come to a narrow pass."
  4: "To find the @house of Valkadesh through the pass!"
  5: "To see it at 40 degrees south, 65 degrees east by the sextant."
  6: "But to take care!"
  7: "To avoid the @lava pits and the @Hall of Knowledge!"
  8: "To understand these @directions?"

[14647 op=2 display+exit]
  1: "Then to not understand you."

[14649 op=1 keyword; failure -> 14652]
  1: yes

[14651 op=2 display+exit]
  1: "Good!"

[14654 op=2 display+exit]
  1: "To ask what was unclear?"

[14656 op=1 keyword; failure -> 14659]
  1: yes

[14661 op=2 display+exit]
  1: "Oh."
  2: The little gargoyle seems saddened.

[14662 op=1 keyword; failure -> 14665]
  1: look

[14664 op=2 display+exit]
  1: a small, light-skinned gargoyle.
  2: "To ask how I can help, mighty $P," Beh Lem asks.

[14665 op=1 keyword; failure -> 14670]
  1: look

[14669 op=2 display+exit]
  1: a small, light-skinned gargoyle.
  2: "To greet you, human."
  3: "To recognize you as the False @Prophet!"

[14670 op=1 keyword; failure -> 14674]
  1: look

[14673 op=2 display+exit]
  1: a small, light-skinned gargoyle.
  2: "Good $T, my friend."
  3: "To have met @Valkadesh yet?"

[14674 op=1 keyword; failure -> 14680]
  1: look

[14677 op=22 display+continue]
  1: a small, light-skinned gargoyle.
  2: The gargoyle greets you enthusiastically, but you can't understand him!
  3: Seeing your confusion, he switches to the human language.
  4: It's obviously a strain for him.
  5: "Hello. I little human @speak."

[14679 op=2 display+exit]
  1: Dupre speaks.
  2: "$P, let's away from here."
  3: "There's naught of use this creature can tell us."
  4: "Agreed?"

[14680 op=1 keyword; failure -> 14682]
  1: look

[14681 op=2 display+exit]
  1: a small, light-skinned gargoyle.
  2: The gargoyle greets you enthusiastically, but you can't understand him!
  3: Seeing your confusion, he switches to the human language.
  4: It's obviously a strain for him.
  5: "Hello."
  6: "I little human @speak."

[14684 op=1 keyword; failure -> 14686]
  1: capt,john,frie

[14685 op=2 display+exit]
  1: "To know Captain John!"
  2: "To help him and @Valkedesh in their work," the youngster says proudly.

[14686 op=1 keyword; failure -> 14689]
  1: name

[14688 op=2 display+exit]
  1: "To be called Beh Lem, which means 'just one.'"
  2: "To have not yet received a @true name."

[14689 op=1 keyword; failure -> 14691]
  1: true

[14690 op=2 display+exit]
  1: "To not receive a true name until I have proved myself @worthy."

[14691 op=1 keyword; failure -> 14693]
  1: wort,prov

[14692 op=2 display+exit]
  1: "To not deserve a name yet."
  2: "But to get one someday, just like my father @Valkadesh!"

[14694 op=1 keyword; failure -> 14696]
  1: job

[14695 op=2 display+exit]
  1: "To be here to help you, noble $P."

[14696 op=1 keyword; failure -> 14698]
  1: job

[14697 op=2 display+exit]
  1: "To be here to wait for Captain @John."

[14699 op=1 keyword; failure -> 14701]
  1: fath,scho,valk

[14700 op=2 display+exit]
  1: "To very much respect Valkadesh my father."
  2: "To want to be a scholar someday just like him!"

[14701 op=1 keyword; failure -> 14704]
  1: fath,scho,valk

[14703 op=2 display+exit]
  1: "To very much respect Valkadesh my father."
  2: "To want to be a scholar someday just like him!"
  3: "To wish to meet Valkadesh?"

[14704 op=1 keyword; failure -> 14706]
  1: dire

[14705 op=2 display+exit]
  1: "To ask what directions were unclear?"

[14707 op=1 keyword; failure -> 14710]
  1: guid

[14709 op=2 display+exit]
  1: "To go from the entrance to Hythloth westward, following the @mountains."
  2: "Then to turn north, at the @Hall of Knowledge."
  3: "To walk east along the mountains until you come to a narrow pass."
  4: "To find the @house of Valkadesh through the pass!"
  5: "To see it at 40 degrees south, 65 degrees east by the sextant."
  6: "But to take care!"
  7: "To avoid the @lava pits and the @Hall of Knowledge!"
  8: "To understand these @directions?"

[14710 op=1 keyword; failure -> 14712]
  1: sacr

[14711 op=2 display+exit]
  1: "To not know much about that."
  2: "But to be sure that @Valkadesh would know!"

[14712 op=1 keyword; failure -> 14714]
  1: cont,dili,pass

[14713 op=2 display+exit]
  1: "To be one of our guiding @principles."

[14714 op=1 keyword; failure -> 14716]
  1: prin

[14715 op=2 display+exit]
  1: "To be surprised that you don't know that!"
  2: "To have three guiding principles - @control, @diligence, and @passion."
  3: "To be the basis of all gargoyle beliefs."

[14716 op=1 keyword; failure -> 14718]
  1: fals,prop

[14717 op=2 display+exit]
  1: "To be feared by everybody - everybody except me and @Valkadesh, my father!"

[14718 op=1 keyword; failure -> 14720]
  1: lava,pits

[14719 op=2 display+exit]
  1: "To be in danger from the lava!"
  2: "To stay far away!"

[14720 op=1 keyword; failure -> 14722]
  1: moun

[14721 op=2 display+exit]
  1: "To walk clockwise around the mountains from Hythloth to reach my father's @house."

[14722 op=1 keyword; failure -> 14724]
  1: hous

[14723 op=2 display+exit]
  1: "To find the house of Valkadesh my father through the pass east of the @Hall."

[14724 op=1 keyword; failure -> 14726]
  1: hall,know

[14725 op=2 display+exit]
  1: "To find the Hall of Knowledge to the south of the city."
  2: "To avoid the Hall!"
  3: "To talk to @Valkadesh first!"

[14726 op=1 keyword; failure -> 14728]
  1: join

[14728 op=1 keyword; failure -> 14730]
  1: leave

[14730 op=1 keyword; failure -> 14732]
  1: bye

[14731 op=2 display+exit]
  1: "May your persistence and precision lead to success!"

[14734 op=2 display+exit]
  1: "To know nothing about that."

[14736 op=2 display+exit]
  1: "To be young."
  2: "To think you should ask someone older."

[14738 op=2 display+exit]
  1: "To truly wish I could help you with that."

[14740 op=2 display+exit]
  1: "To not understand what you mean by $Z."

[14741 op=2 display+exit]
  1: "To not have enough experience to answer that."

[14742 op=1 keyword; failure -> 14744]
  1: spea,huma,litt,lang,read

[14743 op=2 display+exit]
  1: "@Captain John teach I human speak," the gargoyle says haltingly.

[14744 op=1 keyword; failure -> 14747]
  1: name

[14746 op=2 display+exit]
  1: "No name."
  2: "I Beh Lem."

[14747 op=1 keyword; failure -> 14749]
  1: job

[14748 op=2 display+exit]
  1: "I wait here."
  2: "@Captain John come."

[14749 op=1 keyword; failure -> 14751]
  1: john,capt

[14750 op=2 display+exit]
  1: "Captain John friend."
  2: "He come here."
  3: "I wait here."

[14751 op=1 keyword; failure -> 14753]
  1: bye

[14752 op=2 display+exit]
  1: The confused gargoyle waves good-bye.

[14753 op=2 display+exit]
  1: The gargoyle shrugs.

[14757 op=2 display+exit]
  1: "To wish very much to join you."
  2: "But to not have enough room in your party for another."
  3: "To wish @directions to the house of Valkadesh?"

[14760 op=2 display+exit]
  1: "To wish very much to join you."
  2: "To not be able to join you right now."
  3: "To ask again later, perhaps."
  4: "To wish @directions to the house of Valkadesh?"

[14761 op=2 display+exit]
  1: "To go with you and @guide you there!"
  2: The little gargoyle eagerly joins your group.

[14764 op=2 display+exit]
  1: "To not understand what you mean by $Z."

[14766 op=2 display+exit]
  1: "To wish very much to join you."
  2: "But to not have enough room in your party for another."

[14768 op=2 display+exit]
  1: "To not be able to join you right now."
  2: "To ask again later, perhaps."

[14769 op=2 display+exit]
  1: The little gargoyle eagerly joins your group.

[14772 op=2 display+exit]
  1: "To not understand what you mean by $Z."

[14774 op=2 display+exit]
  1: "To wish to stay with you!"
  2: "To not wish to leave until there is peace between our races!"
  3: "To not leave here!"

[14775 op=2 display+exit]
  1: "To wish to stay with you!"
  2: "But to wait here until you return."

```

<a id="ben-npc-52"></a>
## Ben ? NPC 52, port 53

Instruction range: 4505?4592

```text
[4507 op=1 keyword; failure -> 4510]
  1: look

[4509 op=2 display+exit]
  1: a man as large as the trees that surround his cottage.
  2: "You're welcome here."

[4511 op=1 keyword; failure -> 4513]
  1: look

[4512 op=2 display+exit]
  1: a man as large as the trees that surround his cottage.
  2: "Why you bargin' inta my house?" he shouts.
  3: Hefting his large axe, he prepares to defend his home.

[4514 op=2 display+exit]
  1: "Why you bargin' inta my house?" he shouts.
  2: Hefting his large axe, he prepares to defend his home.

[4516 op=1 keyword; failure -> 4520]
  1: no

[4519 op=2 display+exit]
  1: "Oh."
  2: He seems friendlier now.

[4523 op=2 display+exit]
  1: "I knew it!"

[4525 op=1 keyword; failure -> 4528]
  1: yes

[4527 op=2 display+exit]
  1: "Logs is 5 gold apiece. Ya still want one?"

[4531 op=2 display+exit]
  1: "Well, come back sometime and I'll cut one for ya."

[4534 op=2 display+exit]
  1: "Well, what do ya want then?"

[4536 op=1 keyword; failure -> 4538]
  1: yes

[4540 op=2 display+exit]
  1: "Too pricey for ya, eh?"

[4541 op=1 keyword; failure -> 4544]
  1: look

[4543 op=2 display+exit]
  1: a man as large as the trees that surround his cottage.
  2: "Hullo again, friend."

[4544 op=1 keyword; failure -> 4547]
  1: look

[4546 op=2 display+exit]
  1: a man as large as the trees that surround his cottage.
  2: "Hah, sneakin' around in my @forest, huh?"
  3: "I told ya once already that I want to be left @alone!"

[4547 op=1 keyword; failure -> 4550]
  1: look

[4549 op=2 display+exit]
  1: a man as large as the trees that surround his cottage.
  2: "I told the last one of ya that came by that I'll have nothin' t'do with yer @wars, so shove off!"

[4550 op=1 keyword; failure -> 4552]
  1: war,alon

[4551 op=2 display+exit]
  1: "Ever since I won that choppin' contest, the @king's had a notion I'd be good at wittlin' on men the same way."
  2: "Well, you can tell him I'm not interested."

[4552 op=1 keyword; failure -> 4555]
  1: king

[4554 op=2 display+exit]
  1: "You are from the king, ain't ya?"

[4555 op=1 keyword; failure -> 4558]
  1: fore

[4557 op=2 display+exit]
  1: "Aye, this is where I cut trees, and I don't like strangers wanderin' through!"
  2: "You're welcome here, though."

[4558 op=1 keyword; failure -> 4560]
  1: fore

[4559 op=2 display+exit]
  1: "Aye, this is where I cut trees, and I don't like strangers wanderin' through!"

[4560 op=1 keyword; failure -> 4563]
  1: name

[4562 op=2 display+exit]
  1: "Ben. My @friends call me Big Ben."
  2: "I guess you can call me 'Big Ben'." He smiles broadly.

[4563 op=1 keyword; failure -> 4565]
  1: name

[4564 op=2 display+exit]
  1: "Ben. My @friends call me Big Ben."
  2: "But you ain't one of my friends."

[4565 op=1 keyword; failure -> 4568]
  1: big

[4567 op=2 display+exit]
  1: "'Tis my name. Don't wear it out." He laughs resoundingly at his wit.

[4568 op=1 keyword; failure -> 4570]
  1: big

[4569 op=2 display+exit]
  1: He fires a look at you. "I said only my @friends call me that."

[4570 op=1 keyword; failure -> 4572]
  1: ben

[4571 op=2 display+exit]
  1: "'Tis my name. Don't wear it out." He laughs resoundingly at his wit.

[4572 op=1 keyword; failure -> 4574]
  1: frie

[4573 op=2 display+exit]
  1: "It surprises you that I would have some."
  2: "Sometimes it surprises me too." He laughs.

[4574 op=1 keyword; failure -> 4576]
  1: job

[4575 op=2 display+exit]
  1: "Have ya never seen a lumberman before?"
  2: "Look around. Those stacks o' @wood don't cut themselves."

[4576 op=1 keyword; failure -> 4579]
  1: buy,wood,log,yew

[4578 op=2 display+exit]
  1: "Aye, are ya here to buy a log?"

[4579 op=1 keyword; failure -> 4582]
  1: bye

[4581 op=2 display+exit]
  1: "Careful in those woods now."

[4582 op=1 keyword; failure -> 4584]
  1: bye

[4583 op=2 display+exit]
  1: "Careful in those woods now."
  2: "Accidents happen...."

[4584 op=2 display+exit]
  1: "I spend too much time in these woods. Ask someone else."

[4588 op=2 display+exit]
  1: "But you don't have 5 gold!"

[4590 op=2 display+exit]
  1: "But you can't carry a log!"

[4592 op=2 display+exit]
  1: He takes the gold and hands over a fine yew log.
  2: Ben seems friendlier now.

```

<a id="blaine-npc-135"></a>
## Blaine ? NPC 135, port 136

Instruction range: 12390?12476

```text
[12391 op=1 keyword; failure -> 12394]
  1: yes

[12396 op=2 display+exit]
  1: "Oh, pardon my mistake."
  2: "It's just that we gypsies often have trouble finding acceptance among the gorgios."
  3: "I'm very grateful that you've given me the chance to come along on your quest."

[12398 op=1 keyword; failure -> 12401]
  1: yes

[12403 op=2 display+exit]
  1: "Okay."
  2: "Let me know if you need my help later."

[12405 op=1 keyword; failure -> 12410]
  1: yes

[12409 op=2 display+exit]
  1: "Almost due east of Iolo's hut there lives a powerful enchanter."

[12411 op=1 keyword; failure -> 12414]
  1: yes

[12413 op=2 display+exit]
  1: "Best check your purse."

[12416 op=2 display+exit]
  1: "As you wish."

[12418 op=1 keyword; failure -> 12423]
  1: yes

[12422 op=2 display+exit]
  1: The gypsy juggler takes three wands out of his knapsack and sets their ends afire.
  2: Tossing them into the air, he begins juggling.
  3: The pattern is elaborate, almost hypnotic as the tips of the wands describe great flaming arcs and spirals through the air. 
  4: At regular intervals he tosses one around behind his back.
  5: Yet somehow it always finds its way back into the pattern with the other two, without missing a beat.
  6: At last he catches all three of the wands in one hand, waves them with a great flourish, and bows deeply before you.
  7: As he extinguishes the wands, he says "I hope you enjoyed the show."

[12424 op=1 keyword; failure -> 12427]
  1: yes

[12426 op=2 display+exit]
  1: "Best check your purse."

[12429 op=2 display+exit]
  1: "Perhaps some other time."

[12430 op=1 keyword; failure -> 12433]
  1: look

[12432 op=2 display+exit]
  1: a slightly built gypsy lad.
  2: "Our dog did his best trick for you."
  3: "You should show him some appreciation."

[12433 op=1 keyword; failure -> 12435]
  1: look

[12434 op=2 display+exit]
  1: a slightly built gypsy lad.
  2: "Good $T."

[12435 op=1 keyword; failure -> 12437]
  1: name

[12436 op=2 display+exit]
  1: "I am called Blaine."

[12438 op=1 keyword; failure -> 12440]
  1: job

[12439 op=2 display+exit]
  1: "I am a @juggler."

[12440 op=1 keyword; failure -> 12442]
  1: job

[12441 op=2 display+exit]
  1: "I am a @juggler."
  2: "But I would fain @join thee and go adventuring."

[12442 op=1 keyword; failure -> 12445]
  1: join,adve

[12444 op=2 display+exit]
  1: "I have joined you already!"

[12445 op=1 keyword; failure -> 12448]
  1: join,adve

[12447 op=2 display+exit]
  1: "Do you want me to come with you?"

[12448 op=1 keyword; failure -> 12452]
  1: leav

[12451 op=2 display+exit]
  1: "I knew it!"
  2: "You don't really want to have a gypsy around."
  3: "You wish me to leave you now, right?"

[12452 op=1 keyword; failure -> 12455]
  1: jugg

[12454 op=2 display+exit]
  1: "I charge five gold for juggling."
  2: "Would you like to see some?"

[12455 op=1 keyword; failure -> 12458]
  1: advi

[12457 op=2 display+exit]
  1: "My advice will cost you a gold coin."
  2: "What do you say?"

[12458 op=1 keyword; failure -> 12460]
  1: buy,sell

[12459 op=2 display+exit]
  1: "I don't have anything to sell just now."
  2: "Check back with me next week."

[12460 op=1 keyword; failure -> 12462]
  1: bye

[12461 op=2 display+exit]
  1: "Here's a little tune to lighten your step on the road ahead."
  2: "Said the youthful earl to the gypsy girl,"
  3: "As the moon was casting its silver shine:"
  4: "Dear little lady, my gypsy lady,"
  5: "Let me kiss those sweet lips of thine."

[12462 op=2 display+exit]
  1: "I can't help you with that."

[12465 op=2 display+exit]
  1: "I have joined you already!"

[12467 op=2 display+exit]
  1: "You have enough travelling with you already."
  2: "If you ask one of them to @leave first, I'd be happy to join you."

[12469 op=2 display+exit]
  1: "I'm busy!"
  2: "Let me know if you need my help later."

[12470 op=2 display+exit]
  1: "I look forward to the many adventures we will share."

[12473 op=2 display+exit]
  1: "I can't help you with that."

[12475 op=2 display+exit]
  1: "Let's go back to my fellow gypsies first, okay?"

[12476 op=2 display+exit]
  1: "Fine."
  2: "I'll be with my fellow gypsies if you change your mind."
  3: "I suppose you'll be wanting to keep all this equipment."
  4: He leaves everything he was carrying in a pile on the ground.

```

<a id="bolesh-npc-163"></a>
## Bolesh ? NPC 163, port 164

Instruction range: 14776?14808

```text
[14780 op=1 keyword; failure -> 14782]
  1: look

[14781 op=2 display+exit]
  1: a strong, winged gargoyle warrior.
  2: He is sorely wounded.
  3: The gargoyle struggles to sit up.
  4: He shouts, "An-bal-sil-fer!"
  5: "Agra-lem! Agra-lem!"
  6: The gargoyle collapses, exhausted.

[14782 op=2 display+exit]
  1: The gargoyle struggles to sit up.
  2: He shouts, "An-bal-sil-fer!"
  3: "Agra-lem! Agra-lem!"
  4: The gargoyle collapses, exhausted.

[14783 op=1 keyword; failure -> 14785]
  1: look

[14784 op=2 display+exit]
  1: a strong, winged gargoyle warrior.
  2: He is sorely wounded.
  3: The gargoyle struggles to sit up.
  4: He shouts, "The False Prophet!"
  5: "Guards! Guards!"
  6: The gargoyle collapses, outraged but exhausted.

[14785 op=2 display+exit]
  1: The gargoyle struggles to sit up.
  2: He shouts, "The False Prophet!"
  3: "Guards! Guards!"
  4: The gargoyle collapses, outraged but exhausted.

[14786 op=1 keyword; failure -> 14789]
  1: look

[14788 op=2 display+exit]
  1: a strong, winged gargoyle warrior.
  2: He is sorely wounded.
  3: The gargoyle struggles to sit up.
  4: "Greetings again, honored one."

[14789 op=1 keyword; failure -> 14792]
  1: look

[14791 op=2 display+exit]
  1: a strong, winged gargoyle warrior.
  2: He is sorely wounded.
  3: The gargoyle struggles to sit up.
  4: "The False Prophet!"
  5: "But to wear the Amulet of Submission?"
  6: "Then to express awe at your honor and courage."

[14792 op=1 keyword; failure -> 14794]
  1: name

[14793 op=2 display+exit]
  1: "To be named Bolesh Us-agra-lem, Bolesh the Troop-@Leader."

[14794 op=1 keyword; failure -> 14796]
  1: job,lead

[14795 op=2 display+exit]
  1: "To lead what remains of our forces into battle."
  2: "To have recently returned from a mission to your side of the world."
  3: "To have tried to reach the @Codex."

[14796 op=1 keyword; failure -> 14798]
  1: code

[14797 op=2 display+exit]
  1: "To have had victory between our claws, but to have been thwarted on the verge of success!"
  2: "To have encountered an impervious @force field around the Codex."

[14798 op=1 keyword; failure -> 14800]
  1: forc,fiel

[14799 op=2 display+exit]
  1: "To know that such a field surrounded the Codex when my people possessed it."
  2: "To remember that only those on sacred @quests for wisdom could pass."

[14800 op=1 keyword; failure -> 14802]
  1: sacr,ques

[14801 op=2 display+exit]
  1: "To receive a sacred quest at the @Temple of Singularity."
  2: "But to be uncertain whether a human's request would be answered there."

[14802 op=1 keyword; failure -> 14804]
  1: temp,sing

[14803 op=2 display+exit]
  1: "To find the temple in the mountains to the north."
  2: "To need to fly to get there, though!"

[14804 op=1 keyword; failure -> 14806]
  1: fly

[14805 op=2 display+exit]
  1: "Yes, to be accessible only to the superior winged gargoyles."
  2: "To be totally unreachable by land-bound worker gargoyles!"
  3: "To suppose that it is totally unreachable by land-bound humans as well."

[14806 op=1 keyword; failure -> 14808]
  1: bye

[14807 op=2 display+exit]
  1: "Farewell, False Prophet."
  2: "To honor my people with your sacrifice."

[14808 op=2 display+exit]
  1: Weakly, the gargoyle says, "What?"

```

<a id="bonn-npc-140"></a>
## Bonn ? NPC 140, port 141

Instruction range: 12945?13013

```text
[12946 op=1 keyword; failure -> 12949]
  1: yes

[12948 op=2 display+exit]
  1: "Too bad! I've hidden it well, and it will go to my grave with me!"
  2: "I'll @die all the better for still having it!"

[12951 op=2 display+exit]
  1: "Good!"
  2: "You can't have it anyway!"

[12952 op=1 keyword; failure -> 12954]
  1: look

[12953 op=2 display+exit]
  1: a short, barefooted crazy man.
  2: "At last!"
  3: "I knew you'd come to steal my @shoes someday!"
  4: "But I was too smart for you!"
  5: "You see, I HAVE NO SHOES!"

[12954 op=1 keyword; failure -> 12956]
  1: name

[12955 op=2 display+exit]
  1: "A clever ploy, pretending to be friendly..."
  2: "But if I told you my name was @Bonn,
  3: then you could work foul magic to destroy my mind..."
  4: "I'll not fall for your tricks!"

[12956 op=1 keyword; failure -> 12958]
  1: bonn

[12957 op=2 display+exit]
  1: "Who told you that?"
  2: "Alas, I am undone!"
  3: "I must flee!"

[12958 op=1 keyword; failure -> 12960]
  1: job,desp

[12959 op=2 display+exit]
  1: "I have no job, nor shoes, nor any such thing."
  2: "But I've learned from my misfortune, learned never to @trust anyone, let alone to @love."

[12960 op=1 keyword; failure -> 12962]
  1: stea,shoe

[12961 op=2 display+exit]
  1: "I threw my shoes into the @sea when I saw you coming!"

[12962 op=1 keyword; failure -> 12964]
  1: lear,trus,misf

[12963 op=2 display+exit]
  1: "I trusted @Hawkins once, and it led to nothing but despair."

[12964 op=1 keyword; failure -> 12966]
  1: hawk

[12965 op=2 display+exit]
  1: "Oh how I envy him his peaceful @sleep!"

[12966 op=1 keyword; failure -> 12968]
  1: live,deat,envy,peac,slee,luck,kill,die

[12967 op=2 display+exit]
  1: "Death's embrace is the only true @comfort a man can find in this @world."

[12968 op=1 keyword; failure -> 12970]
  1: embr,comf

[12969 op=2 display+exit]
  1: "A good chair is worth a thousand @bumberchutes."

[12970 op=1 keyword; failure -> 12972]
  1: bumb

[12971 op=2 display+exit]
  1: "Who told you about those?"
  2: "It's supposed to be a @secret!"

[12972 op=1 keyword; failure -> 12974]
  1: worl

[12973 op=2 display+exit]
  1: "Flat as a pancake, and twice as tasty!"

[12974 op=1 keyword; failure -> 12976]
  1: love

[12975 op=2 display+exit]
  1: "Love is a slippery sled that flips over and pins you to the slope."
  2: "And then, at night, the ice @weasels come."

[12976 op=1 keyword; failure -> 12978]
  1: nigh,ice,weas

[12977 op=2 display+exit]
  1: "If you're lucky, they'll @kill you!"

[12978 op=1 keyword; failure -> 12980]
  1: bee,head

[12979 op=2 display+exit]
  1: "The buzzing, they're always buzzing in my head."

[12980 op=1 keyword; failure -> 12982]
  1: mad,craz

[12981 op=2 display+exit]
  1: "I can @help you!"
  2: "I'm the only @sane one left."

[12982 op=1 keyword; failure -> 12984]
  1: only,sane,left

[12983 op=2 display+exit]
  1: "All the others LOST THEIR SHOES!"

[12984 op=1 keyword; failure -> 12986]
  1: help

[12985 op=2 display+exit]
  1: "Go to the Shrine of Honesty, and stand just to the east of it."
  2: "Take 3 steps north, 2 steps west, 5 steps south, and 4 steps east."
  3: "Then eat some @grapes, and all will become clear to you!"

[12986 op=1 keyword; failure -> 12988]
  1: shri,hone,clea,grap

[12987 op=2 display+exit]
  1: "Mark my words!"

[12988 op=1 keyword; failure -> 12991]
  1: map

[12990 op=2 display+exit]
  1: A strange light comes into his eyes.
  2: "The map?"
  3: "The secret treasure map?"
  4: "I still have it, right here in my @house."
  5: "Do you want it?"

[12991 op=1 keyword; failure -> 12993]
  1: hous,grav

[12992 op=2 display+exit]
  1: "I lived on the sea, and I'll die not far from it."
  2: "Then my @basement can be my tomb."

[12993 op=1 keyword; failure -> 12995]
  1: base,tomb

[12994 op=2 display+exit]
  1: "Too well hidden for the likes of you to find!"

[12995 op=1 keyword; failure -> 12997]
  1: sea

[12996 op=2 display+exit]
  1: "My shoes will be very happy there, @dancing with the fishies!"

[12997 op=1 keyword; failure -> 12999]
  1: danc,fish

[12998 op=2 display+exit]
  1: "They'll dance a @waltz, I doubt it not!"

[12999 op=1 keyword; failure -> 13001]
  1: walt

[13000 op=2 display+exit]
  1: He begins laughing hysterically.

[13001 op=1 keyword; failure -> 13003]
  1: bye

[13002 op=2 display+exit]
  1: "Farewell, sweet enemy. I love you more than my own death!"

[13003 op=1 keyword; failure -> 13005]
  1: secr

[13004 op=2 display+exit]
  1: "You're not one of @us!"

[13005 op=1 keyword; failure -> 13007]
  1: us

[13006 op=2 display+exit]
  1: "We know who @we are..."

[13007 op=1 keyword; failure -> 13009]
  1: we

[13008 op=2 display+exit]
  1: "Me and the @others."

[13009 op=1 keyword; failure -> 13011]
  1: othe

[13010 op=2 display+exit]
  1: "By the time you find out, it will be too @late!"

[13011 op=1 keyword; failure -> 13013]
  1: time,find,out,late

[13012 op=2 display+exit]
  1: "I won't tell you because..."

[13013 op=2 display+exit]
  1: "Sometimes the @bees in my head almost drive me @mad!"

```

<a id="boskin-npc-58"></a>
## Boskin ? NPC 58, port 59

Instruction range: 5491?5572

```text
[5499 op=1 keyword; failure -> 5501]
  1: look

[5500 op=2 display+exit]
  1: a short, heavyset man dressed in rags.
  2: "Who's there?" the thief calls insolently.

[5501 op=2 display+exit]
  1: "Who's there?" the thief calls insolently.

[5512 op=2 display+exit]
  1: The thief's tone becomes more respectful as he recognizes you.
  2: "'Strewth, it's the Avatar!"
  3: "You've got to get me @out of 'ere, $G!"

[5514 op=1 keyword; failure -> 5517]
  1: look

[5516 op=2 display+exit]
  1: a short, heavyset man dressed in rags.
  2: "I ain't talkin' to you, you're just like all the rest!"

[5519 op=2 display+exit]
  1: "I ain't talkin' to you, you're just like all the rest!"

[5521 op=1 keyword; failure -> 5524]
  1: look

[5523 op=2 display+exit]
  1: a short, heavyset man dressed in rags.
  2: "I'll never crack, no matter 'ow many times you come back!"

[5526 op=2 display+exit]
  1: "I'll never crack, no matter 'ow many times you come back!"

[5528 op=1 keyword; failure -> 5531]
  1: look

[5530 op=2 display+exit]
  1: a short, heavyset man dressed in rags.
  2: "All right, I give up."
  3: "The rune is 'idden under a potted plant at the Slaughtered Lamb."
  4: "Now go away and leave me alone."

[5533 op=2 display+exit]
  1: "All right, I give up."
  2: "The rune is 'idden under a potted plant at the Slaughtered Lamb."
  3: "Now go away and leave me alone."

[5534 op=1 keyword; failure -> 5536]
  1: look

[5535 op=2 display+exit]
  1: a short, heavyset man dressed in rags.
  2: "$G!"
  3: "Am I glad to see thee!"
  4: "You're me last light in a world of darkness."

[5536 op=1 keyword; failure -> 5538]
  1: grav,rob,crim,stol,out

[5537 op=2 display+exit]
  1: "I admit I robbed that grave."
  2: "But it were only because 'is Lordship wouldn't be needin' that stuff no more."
  3: "I 'ad me @kids to feed, didn't I?"

[5538 op=1 keyword; failure -> 5540]
  1: rune

[5539 op=2 display+exit]
  1: "I'll tell you where the rune is if you'll let me @go."
  2: "I've got me kids to think of."

[5540 op=1 keyword; failure -> 5542]
  1: kids,fami,feed,farm,hars,out,here

[5541 op=2 display+exit]
  1: Tears fill the man's eyes.
  2: "My poor children've got no one to take care of them now."
  3: "I only stole to feed them, and that's the @truth."

[5542 op=1 keyword; failure -> 5544]
  1: true,trut

[5543 op=2 display+exit]
  1: "True?"
  2: "Of course it's true!"
  3: "Even a thief like me wouldn't lie to the Avatar."

[5544 op=1 keyword; failure -> 5547]
  1: know,lie,lyin,fals,fool

[5546 op=2 display+exit]
  1: "All right, I admit it."
  2: "I 'aven't got any kids."
  3: "But that stupid cow they got runnin' this place won't listen to reason."
  4: "A little give-and-take, know what I mean?"
  5: "I tell you what."
  6: "If I tell you where the rune is, will you take it away?"
  7: "I don't want these pompous blighters to 'ave it."
  8: "Okay?"

[5547 op=1 keyword; failure -> 5550]
  1: rele,let,free,go,allo,set,home

[5549 op=2 display+exit]
  1: "You mean it, $G?"
  2: "Are they gonna let me go 'ome?"

[5550 op=1 keyword; failure -> 5552]
  1: name

[5551 op=2 display+exit]
  1: "Me name's Boskin, $G."

[5552 op=1 keyword; failure -> 5554]
  1: job

[5553 op=2 display+exit]
  1: "I'm but a poor farmer, forced into crime by the harshness of these times."
  2: "There weren't no other way to feed me family, $G!"

[5554 op=1 keyword; failure -> 5556]
  1: bye

[5555 op=2 display+exit]
  1: "Please, $G!"
  2: "Tell them to let me @go 'ome!"
  3: Tell them about me poor fatherless @kids!"
  4: "They'll listen to you!"

[5556 op=2 display+exit]
  1: "Beg your pardon, $G?"

[5558 op=1 keyword; failure -> 5564]
  1: yes

[5561 op=22 display+continue]
  1: His face lights up. "I knew I could count on you, $G!"
  2: "And I'll keep my end of the bargain."
  3: "I 'id the rune under a potted plant in the Slaughtered Lamb inn."

[5563 op=2 display+exit]
  1: As you leave, Iolo whispers to you.
  2: "Maybe it was necessary, $P, but lying makes me uneasy."
  3: "Even to someone like him."

[5564 op=1 keyword; failure -> 5567]
  1: yes

[5566 op=2 display+exit]
  1: His face lights up. "I knew I could count on you, $G!"
  2: "And I'll keep my end of the bargain."
  3: "I 'id the rune under a potted plant in the Slaughtered Lamb inn."

[5567 op=2 display+exit]
  1: His face falls.
  2: "Then I ain't sayin' nothin'."
  3: "I've got me kids to think of, I do."

[5569 op=1 keyword; failure -> 5571]
  1: yes

[5570 op=2 display+exit]
  1: "All right, I'll take your word for it."
  2: "Go to the Slaughtered Lamb;"
  3: "I 'id the rune under a potted plant there."
  4: "And spit on Her Ladyship for me!"

[5572 op=2 display+exit]
  1: "Then the gargoyles can take the lot of you!"
  2: "You'll never get nothin' out of me!"

```

<a id="brandon-npc-77"></a>
## Brandon ? NPC 77, port 78

Instruction range: 7155?7466

```text
[7155 op=1 keyword; failure -> 7159]
  1: look

[7158 op=2 display+exit]
  1: a golden-haired young man.
  2: "Hey! The Avatar! Good $T, $G!"
  3: "You've come to me to aid thee in thy quest!"
  4: "What an honor!"

[7159 op=1 keyword; failure -> 7161]
  1: look

[7160 op=2 display+exit]
  1: a golden-haired young man.
  2: "Avatar! Good $T, $G! How has thy quest gone?"

[7161 op=1 keyword; failure -> 7163]
  1: well,good,fine,okay,grea

[7162 op=2 display+exit]
  1: "And glad I am to hear it!"

[7163 op=1 keyword; failure -> 7165]
  1: job

[7164 op=2 display+exit]
  1: "Oh, I'm not important like you. I'm just a weaponsmith--and only a @journeyman at that."

[7165 op=1 keyword; failure -> 7167]
  1: jour

[7166 op=2 display+exit]
  1: "Aye, just a few years ago I was a simple apprentice."
  2: "Now I @supply arms to all Trinsic!"

[7167 op=1 keyword; failure -> 7169]
  1: name

[7168 op=2 display+exit]
  1: "My name's Brandon, $G, and I'm honored to meet thee!"

[7169 op=1 keyword; failure -> 7171]
  1: join

[7170 op=2 display+exit]
  1: "That would be a great honor! I'd love to!"
  2: The man comes to his senses.
  3: "But no, I can't. I've responsibilites here, especially @now."

[7171 op=1 keyword; failure -> 7173]
  1: now,supp

[7172 op=2 display+exit]
  1: "I have to make @weapons to help fight the gargoyles!"

[7173 op=1 keyword; failure -> 7175]
  1: weap

[7174 op=2 display+exit]
  1: "Perhaps you'd like to @buy some. I'd be honored to have thee as a customer."

[7177 op=1 keyword; failure -> 7179]
  1: buy

[7178 op=2 display+exit]
  1: "I have a large inventory, so be more specific."
  2: "Dost thou want @arms or @armor?"

[7179 op=1 keyword; failure -> 7181]
  1: sell

[7181 op=1 keyword; failure -> 7183]
  1: bulk

[7185 op=1 keyword; failure -> 7188]
  1: 2handedsword

[7187 op=2 display+exit]
  1: "That 2 handed sword costs 75 gold."
  2: "Interested in multiple?"

[7188 op=1 keyword; failure -> 7191]
  1: magica

[7190 op=2 display+exit]
  1: "That magic armour costs 350 gold."
  2: "Interested in multiple?"

[7191 op=1 keyword; failure -> 7194]
  1: magich

[7193 op=2 display+exit]
  1: "That magic helm costs 200 gold."
  2: "Interested in multiple?"

[7194 op=1 keyword; failure -> 7197]
  1: mace

[7196 op=2 display+exit]
  1: "That mace costs 35 gold."
  2: "Interested in multiple?"

[7197 op=1 keyword; failure -> 7200]
  1: main

[7199 op=2 display+exit]
  1: "That main gauche costs 20 gold."
  2: "Interested in multiple?"

[7200 op=1 keyword; failure -> 7203]
  1: swor

[7202 op=2 display+exit]
  1: "That sword costs 40 gold."
  2: "Interested in multiple?"

[7203 op=1 keyword; failure -> 7206]
  1: axe

[7205 op=2 display+exit]
  1: "That 2 handed axe costs 50 gold."
  2: "Interested in multiple?"

[7206 op=1 keyword; failure -> 7209]
  1: hamm

[7208 op=2 display+exit]
  1: "That 2 handed hammer costs 55 gold."
  2: "Interested in multiple?"

[7209 op=1 keyword; failure -> 7212]
  1: iron

[7211 op=2 display+exit]
  1: "That iron helm costs 15 gold."
  2: "Interested in multiple?"

[7212 op=1 keyword; failure -> 7215]
  1: kite

[7214 op=2 display+exit]
  1: "That kite shield costs 25 gold."
  2: "Interested in multiple?"

[7215 op=1 keyword; failure -> 7218]
  1: plat

[7217 op=2 display+exit]
  1: "That plate mail costs 100 gold."
  2: "Interested in multiple?"

[7218 op=1 keyword; failure -> 7220]
  1: arms

[7219 op=2 display+exit]
  1: "Which item, @mace, @main @gauche, @sword, @2 @handed @axe, @2 @handed @hammer or @2 @handed @sword?"

[7220 op=1 keyword; failure -> 7222]
  1: armo

[7221 op=2 display+exit]
  1: "Which item, @iron @helm, @kite @shield, @magic @armour, @magic @helm or @plate @mail?"

[7289 op=1 keyword; failure -> 7291]
  1: buy

[7290 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[7291 op=1 keyword; failure -> 7293]
  1: arms

[7292 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[7293 op=1 keyword; failure -> 7295]
  1: armo

[7294 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[7295 op=1 keyword; failure -> 7297]
  1: sell

[7296 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[7297 op=1 keyword; failure -> 7299]
  1: bye,noth

[7298 op=2 display+exit]
  1: "Farewell! May thy quest succeed, Avatar!"

[7299 op=2 display+exit]
  1: "Beg pardon, $G?"

[7302 op=2 display+exit]
  1: "Perhaps some other time, then."

[7304 op=2 display+exit]
  1: "But 'twould make thee o'erburdened, $Y!"

[7308 op=2 display+exit]
  1: "But thou hast not enough gold, $Y!"

[7310 op=2 display+exit]
  1: "But 'twould make thee o'erburdened, $Y!"

[7311 op=2 display+exit]
  1: "Excellent!"

[7318 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 17 gold for that mace?"

[7324 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 10 gold for that main gauche?"

[7330 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 20 gold for that sword?"

[7336 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 25 gold for that 2 handed axe?"

[7342 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 27 gold for that 2 handed hammer?"

[7348 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 37 gold for that 2 handed sword?"

[7354 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 7 gold for that iron helm?"

[7360 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 12 gold for that kite shield?"

[7366 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 175 gold for that magic armour?"

[7372 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 100 gold for that magic helm?"

[7378 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 50 gold for that plate mail?"

[7381 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[7384 op=1 keyword; failure -> 7388]
  1: yes

[7387 op=2 display+exit]
  1: "Done!"
  2: He hands you 17 gold pieces and takes the mace.

[7389 op=1 keyword; failure -> 7393]
  1: yes

[7392 op=2 display+exit]
  1: "Done!"
  2: He hands you 10 gold pieces and takes the main gauche.

[7394 op=1 keyword; failure -> 7398]
  1: yes

[7397 op=2 display+exit]
  1: "Done!"
  2: He hands you 20 gold pieces and takes the sword.

[7399 op=1 keyword; failure -> 7403]
  1: yes

[7402 op=2 display+exit]
  1: "Done!"
  2: He hands you 25 gold pieces and takes the 2 handed axe.

[7404 op=1 keyword; failure -> 7408]
  1: yes

[7407 op=2 display+exit]
  1: "Done!"
  2: He hands you 27 gold pieces and takes the 2 handed hammer.

[7409 op=1 keyword; failure -> 7413]
  1: yes

[7412 op=2 display+exit]
  1: "Done!"
  2: He hands you 37 gold pieces and takes the 2 handed sword.

[7414 op=1 keyword; failure -> 7418]
  1: yes

[7417 op=2 display+exit]
  1: "Done!"
  2: He hands you 7 gold pieces and takes the iron helm.

[7419 op=1 keyword; failure -> 7423]
  1: yes

[7422 op=2 display+exit]
  1: "Done!"
  2: He hands you 12 gold pieces and takes the kite shield.

[7424 op=1 keyword; failure -> 7428]
  1: yes

[7427 op=2 display+exit]
  1: "Done!"
  2: He hands you 175 gold pieces and takes the magic armour.

[7429 op=1 keyword; failure -> 7433]
  1: yes

[7432 op=2 display+exit]
  1: "Done!"
  2: He hands you 100 gold pieces and takes the magic helm.

[7434 op=1 keyword; failure -> 7438]
  1: yes

[7437 op=2 display+exit]
  1: "Done!"
  2: He hands you 50 gold pieces and takes the plate mail.

[7438 op=2 display+exit]
  1: "Changed your mind, eh?"

[7453 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[7456 op=2 display+exit]
  1: "Been raiding the dungeons, I see."
  2: "I hope you put those foul monsters to shame!"
  3: "Will you take $V0 gold for all of your merchandise?"

[7457 op=2 display+exit]
  1: "Will you take $V0 gold for all of your merchandise?"

[7459 op=1 keyword; failure -> 7461]
  1: yes

[7461 op=2 display+exit]
  1: "Changed your mind, eh?"

[7464 op=2 display+exit]
  1: "$Y, these are fewer items than we agreed upon."
  2: "I should not need to remind the Avatar that you stand within the town of Honor!"

[7466 op=2 display+exit]
  1: "Excellent!"
  2: He hands you $V0 gold pieces in exchange for the items.

```

<a id="budo-npc-113"></a>
## Budo ? NPC 113, port 114

Instruction range: 10534?10701

```text
[10536 op=2 display+exit]
  1: "Uh-huh. Well... Were you hoping to join?"

[10538 op=1 keyword; failure -> 10541]
  1: yes

[10540 op=2 display+exit]
  1: "All right, maybe you've got what it takes. We'll see."
  2: "To be a member of the thieves' guild, you have to get your own belt."
  3: "Now we have a limited membership, so there's only so many belts in the world."
  4: "If you want one of them, you have to 'retire' a member of the guild."
  5: "Lucky for you, it just happens there's a member we'd like to see 'retired' soon."
  6: "Her hideout is deep below Britain, in the sewers. You'd best watch out for the rats down there!"
  7: "One last thing..."
  8: "It would be an embarassment for the guild to have bodies turning up inconveniently."
  9: "So don't get too violent unless you have to."
  10: "I'll talk to you when you get back."

[10543 op=2 display+exit]
  1: He leans even closer, transfixing you with his gaze.
  2: "Then just remember this. I don't take kindly to anyone spreading secrets around."
  3: "If you were to tell anyone I'm involved with the guild, well..."
  4: "Let's just say accidents can happen to anyone--even the Avatar."
  5: "Now get out of my shop."

[10578 op=1 keyword; failure -> 10583]
  1: look

[10580 op=22 display+continue]
  1: a chubby, jovial merchant.
  2: "Good $T!"
  3: "I bet you've come back to pick up that @orrery you ordered."
  4: "Oh, I'm sorry. I must have thought you were somebody you aren't.
  5: "Do you need to @buy some supplies?"

[10582 op=2 display+exit]
  1: "Don't be ridiculous!"
  2: "This is the first time we've ever been in your shop!" says Dupre.

[10584 op=1 keyword; failure -> 10588]
  1: look

[10587 op=2 display+exit]
  1: a chubby, jovial merchant.
  2: "Good $T!"
  3: "I bet you've come back to pick up that @orrery you ordered."

[10588 op=1 keyword; failure -> 10590]
  1: look

[10589 op=2 display+exit]
  1: a chubby, jovial merchant.
  2: "Good $T!"
  3: "Do you need to buy some supplies?"

[10591 op=1 keyword; failure -> 10594]
  1: yes

[10593 op=2 display+exit]
  1: "I'm afraid it still hasn't arrived.
  2: "Do you need to @buy some supplies?"

[10596 op=1 keyword; failure -> 10598]
  1: no

[10597 op=2 display+exit]
  1: "Oh, I'm sorry. I must have thought you were somebody you aren't.
  2: "Do you need to @buy some supplies?"

[10598 op=1 keyword; failure -> 10600]
  1: no

[10599 op=2 display+exit]
  1: "Then it must be Thursday--or possibly Tuesday."
  2: "I never seem to sell anything on Tuesdays."
  3: "What can I do you for?"

[10600 op=1 keyword; failure -> 10602]
  1: buy,sell,this,that,yes

[10601 op=2 display+exit]
  1: "Which, @torches, @oil, @lockpicks, @gems, @backpacks, @bags, @shovels, or @powder @kegs?"

[10602 op=1 keyword; failure -> 10605]
  1: look

[10604 op=2 display+exit]
  1: "Do you need to @buy some supplies?"

[10605 op=1 keyword; failure -> 10608]
  1: name

[10607 op=2 display+exit]
  1: "Why, it's Budo, I believe. That's what it was last week."
  2: "Some call me 'the @Den', but I don't think that's my name."

[10608 op=1 keyword; failure -> 10610]
  1: den

[10609 op=2 display+exit]
  1: "I think someone told me that was @Nick's name."

[10610 op=1 keyword; failure -> 10612]
  1: nick

[10611 op=2 display+exit]
  1: "I've never met him, myself."

[10612 op=1 keyword; failure -> 10614]
  1: orre

[10613 op=2 display+exit]
  1: "I don't know what it is, but I've got one on order."

[10614 op=1 keyword; failure -> 10616]
  1: job

[10615 op=2 display+exit]
  1: "Oh, I sell @this and @that."
  2: "I'm not sure what 'this' and 'that' are, but I @sell a lot of them!"
  3: "People give me @money for them, that is."

[10616 op=1 keyword; failure -> 10618]
  1: mone

[10617 op=2 display+exit]
  1: "It's not as sweet as @honey, but you can trade one for the other."

[10618 op=1 keyword; failure -> 10620]
  1: swee,hone

[10619 op=2 display+exit]
  1: "I like honey."

[10622 op=1 keyword; failure -> 10625]
  1: torc

[10624 op=2 display+exit]
  1: He looks at you, "It'll cost you 3 gold for each torch, how many do you want?"

[10625 op=1 keyword; failure -> 10628]
  1: oil

[10627 op=2 display+exit]
  1: He looks at you, "It'll cost you 3 gold for each oil flask, how many do you want?"

[10628 op=1 keyword; failure -> 10631]
  1: lock,pick

[10630 op=2 display+exit]
  1: He looks at you, "It'll cost you 4 gold for each lockpick, how many do you want?"

[10631 op=1 keyword; failure -> 10634]
  1: gem

[10633 op=2 display+exit]
  1: He looks at you, "It'll cost you 8 gold for each gem, how many do you want?"

[10634 op=1 keyword; failure -> 10637]
  1: back

[10636 op=2 display+exit]
  1: He looks at you, "It'll cost you 5 gold for each backpack, how many do you want?"

[10637 op=1 keyword; failure -> 10640]
  1: bag

[10639 op=2 display+exit]
  1: He looks at you, "It'll cost you 3 gold for each bag, how many do you want?"

[10640 op=1 keyword; failure -> 10643]
  1: powd,keg

[10642 op=2 display+exit]
  1: He looks at you, "It'll cost you 20 gold for each powder keg, how many do you want?"

[10643 op=1 keyword; failure -> 10646]
  1: shov

[10645 op=2 display+exit]
  1: He looks at you, "It'll cost you 10 gold for each shovel, how many do you want?"

[10646 op=1 keyword; failure -> 10649]
  1: torc

[10648 op=2 display+exit]
  1: He looks at you, "It'll cost you 5 gold for each torch, how many do you want?"

[10649 op=1 keyword; failure -> 10652]
  1: oil

[10651 op=2 display+exit]
  1: He looks at you, "It'll cost you 5 gold for each oil flask, how many do you want?"

[10652 op=1 keyword; failure -> 10655]
  1: lock,pick

[10654 op=2 display+exit]
  1: He looks at you, "It'll cost you 7 gold for each lockpick, how many do you want?"

[10655 op=1 keyword; failure -> 10658]
  1: gem

[10657 op=2 display+exit]
  1: He looks at you, "It'll cost you 15 gold for each gem, how many do you want?"

[10658 op=1 keyword; failure -> 10661]
  1: back

[10660 op=2 display+exit]
  1: He looks at you, "It'll cost you 10 gold for each backpack, how many do you want?"

[10661 op=1 keyword; failure -> 10664]
  1: bag

[10663 op=2 display+exit]
  1: He looks at you, "It'll cost you 5 gold for each bag, how many do you want?"

[10664 op=1 keyword; failure -> 10667]
  1: powd,keg

[10666 op=2 display+exit]
  1: He looks at you, "It'll cost you 40 gold for each powder keg, how many do you want?"

[10667 op=1 keyword; failure -> 10670]
  1: shov

[10669 op=2 display+exit]
  1: He looks at you, "It'll cost you 20 gold for each shovel, how many do you want?"

[10670 op=1 keyword; failure -> 10672]
  1: guil,belt

[10673 op=1 keyword; failure -> 10676]
  1: bye

[10675 op=2 display+exit]
  1: He winks at you.
  2: "The guild is behind you, as long as you keep our secret."

[10676 op=1 keyword; failure -> 10678]
  1: bye

[10677 op=2 display+exit]
  1: "Ta ta for now."

[10678 op=2 display+exit]
  1: "$Z?"
  2: "It seems to me I knew something about that, but I'm having trouble with my remembering."

[10681 op=2 display+exit]
  1: He frowns at you, "That was a good price."

[10683 op=2 display+exit]
  1: "Come now, that's a little much don't you think?"

[10690 op=2 display+exit]
  1: "I think you have less gold than you thought."

[10692 op=2 display+exit]
  1: "You haven't any room in your pack!"

[10693 op=2 display+exit]
  1: "There you go."

[10695 op=2 display+exit]
  1: "To keep the guild safe, each member only knows the names of two others."
  2: "You're safer not knowing more about the guild than you have to, believe me..."

[10699 op=2 display+exit]
  1: He glances about quickly, to make sure nobody else is listening.
  2: In a low voice he says, "Congratulations, and welcome to the guild."
  3: "I can offer you the standard member's discount on all supplies now."
  4: "Just remember, whatever you do, don't tell anyone about our @guild!"

[10701 op=2 display+exit]
  1: A startling change comes over his features.
  2: Suddenly the good-natured bumbler that stood before you has been transformed into a shrewd, sharp, dangerous looking man.
  3: He leans forward staring straight into your eyes as he asks "Who sent you?"

```

<a id="captain-fox-npc-109"></a>
## Captain Fox ? NPC 109, port 110

Instruction range: 10261?10298

```text
[10262 op=1 keyword; failure -> 10264]
  1: look

[10263 op=2 display+exit]
  1: a handsome gentleman, immaculately dressed.
  2: "Can't talk now, $G."
  3: He pauses to punch another pirate in the belly.
  4: "Come back later when I've less on my mind!"

[10265 op=2 display+exit]
  1: "Can't talk now, $G."
  2: He pauses to punch another pirate in the belly.
  3: "Come back later when I've less on my mind!"

[10266 op=1 keyword; failure -> 10268]
  1: look

[10267 op=2 display+exit]
  1: a handsome gentleman, immaculately dressed.
  2: "Hello, $G."
  3: "Your company is welcome."

[10268 op=1 keyword; failure -> 10270]
  1: name

[10269 op=2 display+exit]
  1: "I'm @Captain Fox, of the Silken @Stag."

[10270 op=1 keyword; failure -> 10272]
  1: capt

[10271 op=2 display+exit]
  1: When you say 'Captain,' all the pirates at the table turn to you expectantly.
  2: When they realize you weren't talking to them, they go back to what they were doing.

[10272 op=1 keyword; failure -> 10274]
  1: silk,stag

[10273 op=2 display+exit]
  1: "We just stopped off here for some supplies and a @drink or two."

[10274 op=1 keyword; failure -> 10276]
  1: drin,two

[10275 op=2 display+exit]
  1: "Well, maybe three wouldn't hurt."

[10276 op=1 keyword; failure -> 10278]
  1: job

[10277 op=2 display+exit]
  1: "I sail the wide, wide sea, to the edges of the @world and back again."
  2: "Of course, I'm also known for the @paintings I create in my spare time."

[10278 op=1 keyword; failure -> 10280]
  1: pain

[10279 op=2 display+exit]
  1: "There are a few of them around here."
  2: "Go see for yourself."

[10281 op=1 keyword; failure -> 10285]
  1: john

[10282 op=22 display+continue]
  1: "Captain John, that lunatic?"
  2: "I hear he went @underground seeking the @gargoyles, but he fled from the first one he encountered."
  3: "Nobody knows where he is now."

[10284 op=2 display+exit]
  1: "Well, that's not what I heard." says Leodon.

[10285 op=1 keyword; failure -> 10287]
  1: john

[10286 op=2 display+exit]
  1: "Captain John, that lunatic?"
  2: "I hear he went @underground seeking the @gargoyles, but he fled from the first one he encountered."
  3: "Nobody knows where he is now."

[10287 op=1 keyword; failure -> 10289]
  1: unde

[10288 op=2 display+exit]
  1: "Under the earth is no place for a seafaring man to be."

[10289 op=1 keyword; failure -> 10291]
  1: garg

[10290 op=2 display+exit]
  1: "Tough creatures."
  2: "My crew and I steer well clear of them."

[10291 op=1 keyword; failure -> 10293]
  1: edge,world

[10292 op=2 display+exit]
  1: "Don't go too far, matey--ye'll fall off!"
  2: He laughs a hearty laugh and takes a swig of ale.

[10293 op=1 keyword; failure -> 10295]
  1: bye

[10294 op=2 display+exit]
  1: "Until we meet again."

[10297 op=2 display+exit]
  1: "I can't help ye with that."

[10298 op=2 display+exit]
  1: Your question makes him laugh suddenly.
  2: His ale spills across the table.
  3: He continues laughing as he pours himself another ale.

```

<a id="captain-john-npc-181"></a>
## Captain John ? NPC 181, port 182

Instruction range: 15924?15987

```text
[15925 op=1 keyword; failure -> 15928]
  1: yes

[15927 op=2 display+exit]
  1: "I had a silver tablet that contained much of what I had learned."
  2: "I gave it to some gypsies to take to Mariah, at the Lycaeum."
  3: "Did it arrive safely?"

[15930 op=2 display+exit]
  1: "But you must!"
  2: "Thou hast no chance of saving the gargoyles if thou knowest not their @language!"

[15932 op=1 keyword; failure -> 15935]
  1: yes

[15934 op=2 display+exit]
  1: "Good!"
  2: I am glad to know that the gypsies kept their word."
  3: "That tablet must have amazed Mariah!"
  4: "Did you bring the silver tablet here?"

[15937 op=2 display+exit]
  1: "No?!"
  2: "But I paid money, good money, for them to carry it to her!"
  3: "No matter."
  4: "Now that thou art here, all is well."
  5: "You can study the @gargoyles yourself, and carry the knowledge back to the surface!"
  6: "Did you find the silver tablet?"

[15939 op=1 keyword; failure -> 15947]
  1: yes

[15946 op=2 display+exit]
  1: "Here."
  2: He makes you a scroll with the silver tablet.
  3: "Use this scroll, it contains a basic vocabulary of the Gargoyle language."

[15949 op=2 display+exit]
  1: "Without both pieces of the silver tablet, I cannot help you to speak their language."

[15950 op=1 keyword; failure -> 15954]
  1: look

[15953 op=2 display+exit]
  1: a long-haired gentleman with a scholarly demeanor.
  2: "Praise the virtues!"
  3: "Humans have arrived here at last!"
  4: "But what's this? Not just any humans--the Avatar!"
  5: "'Tis most fortuitous that thou hast returned, noble $P."
  6: "Many @cataclysms have occurred since thou last visited Britannia!"

[15954 op=1 keyword; failure -> 15956]
  1: look

[15955 op=2 display+exit]
  1: a long-haired gentleman with a scholarly demeanor.
  2: "Ah, my good friend!"
  3: "Thou hast returned!"

[15956 op=1 keyword; failure -> 15958]
  1: name

[15957 op=2 display+exit]
  1: "Why, I'm Captain John, $G!"

[15958 op=1 keyword; failure -> 15960]
  1: job

[15959 op=2 display+exit]
  1: "For several years I have been here in the catacombs, @studying the @gargoyles."

[15960 op=1 keyword; failure -> 15962]
  1: stud

[15961 op=2 display+exit]
  1: "Yes, I have learned much about them!"

[15962 op=1 keyword; failure -> 15964]
  1: garg

[15963 op=2 display+exit]
  1: "They are not evil, as most humans believe!"
  2: "I have spent much time talking to them, and I have found them to be almost human."
  3: "I have learned about their @society, their @beliefs and their @language."

[15964 op=1 keyword; failure -> 15966]
  1: now,worl,cata

[15965 op=2 display+exit]
  1: "The gargoyle world, once as large as Britannia itself, is slowly being destroyed!"
  2: "The forces thou set in motion have caused great sections of their land to fall off into the void!"
  3: "Only a single gargoyle @city remains."

[15966 op=1 keyword; failure -> 15968]
  1: city

[15967 op=2 display+exit]
  1: "The gargoyle City of Investigation, their place of learning and government, is all that is left."
  2: "Thou must do something to save the gargoyle race!"
  3: "I know not what, however."
  4: "Thou must @talk to the @gargoyles."

[15968 op=1 keyword; failure -> 15970]
  1: soci

[15969 op=2 display+exit]
  1: "There are two kinds of gargoyles, winged and wingless ones."
  2: "At first it seemed as if the wingless ones were slaves."
  3: "But now I know that the wingless ones are beloved and valued family members."
  4: "They must be told what to do, because they are not truly intelligent."
  5: "The @gargoyles live honorably, by their own set of @beliefs."

[15970 op=1 keyword; failure -> 15972]
  1: beli

[15971 op=2 display+exit]
  1: "The gargoyles' beliefs are much like our own."
  2: "Where we have the three principles of Truth, Love and Courage,
  3: they have the principles of Control, Passion and Diligence."
  4: "Directly, these lead to the virtues of Direction, Feeling, and Persistence."
  5: "Combined, they lead to Balance, Achievement and Precision."
  6: "Taken all together, they point to Order."
  7: "And the sum of all the principles is Singularity."
  8: "Gargoyle society stresses unity and singularity in all things."
  9: "They have to, their world has always been harsh, even before the @cataclysms."

[15972 op=1 keyword; failure -> 15975]
  1: lang,talk

[15974 op=2 display+exit]
  1: "With the help of a gargoyle child, @Beh @Lem, I have learned how to speak their language."

[15975 op=1 keyword; failure -> 15978]
  1: lang,talk

[15977 op=2 display+exit]
  1: "With the help of a gargoyle child, @Beh @Lem, I have learned how to speak their language."
  2: "Wouldst thou like to learn?"

[15978 op=1 keyword; failure -> 15980]
  1: beh,lem

[15979 op=2 display+exit]
  1: "He and I meet every day at noon, where Hythloth emerges in the gargoyle lands."
  2: "Go and speak with him, learn about the gargoyles."
  3: "You must save them!"

[15980 op=1 keyword; failure -> 15982]
  1: join

[15981 op=2 display+exit]
  1: "No, I cannot."
  2: "I must stay here and continue my writing."
  3: "But @Beh @Lem may join thee."
  4: "With his help, thou canst travel the gargoyle lands and learn how to save them."

[15982 op=1 keyword; failure -> 15985]
  1: bye

[15984 op=2 display+exit]
  1: "Farewell again, noble Avatar."
  2: "As the gargoyles say, 'May thy persistence and precision lead to success.'"
  3: "Upon you rests the future of an entire race."

[15985 op=1 keyword; failure -> 15987]
  1: bye

[15986 op=2 display+exit]
  1: "Wait!"
  2: "Thou must meet Beh Lem and travel with him among the gargoyle people."
  3: "That is the only way you can save them, and save them you must!"
  4: "And without @Beh @Lem with you, the gargoyles will attack!"

[15987 op=2 display+exit]
  1: "Oh, the things I could say of that!"
  2: "But we have not the time."
  3: "When both worlds have been saved, then we can discuss such matters."

```

<a id="caradon-npc-153"></a>
## Caradon ? NPC 153, port 154

Instruction range: 14193?14223

```text
[14194 op=1 keyword; failure -> 14197]
  1: fine,well,good,okay,ok

[14196 op=2 display+exit]
  1: "I am pleased to hear that."
  2: "I wish I were healthy again."
  3: "But please, $G."
  4: "Let me tell you of my @adventure."

[14199 op=2 display+exit]
  1: "I see."
  2: "I would that I were healthy again, too."
  3: "But please, $G."
  4: "Let me tell you of my @adventure."

[14200 op=1 keyword; failure -> 14203]
  1: look

[14202 op=2 display+exit]
  1: a paladin with a longsword on his back.
  2: "Good $T, $G."

[14203 op=1 keyword; failure -> 14207]
  1: look

[14206 op=2 display+exit]
  1: a paladin with a longsword on his back.
  2: "Good $T, $G."
  3: "How art thou?"

[14207 op=1 keyword; failure -> 14209]
  1: name,knig

[14208 op=2 display+exit]
  1: "Sir Caradon of Trinsic, at thy service."

[14209 op=1 keyword; failure -> 14211]
  1: job,serv

[14210 op=2 display+exit]
  1: "I am a knight."
  2: "Currently, though, I am recovering from a @battle."

[14211 op=1 keyword; failure -> 14213]
  1: adve,heal,batt

[14212 op=2 display+exit]
  1: "I've just come home from fighting in Yew."
  2: "I fought gargoyles near the @shrine."

[14213 op=1 keyword; failure -> 14215]
  1: shri,yew,figh

[14214 op=2 display+exit]
  1: "There I was, with ten companions."
  2: "Outside the Shrine of Honesty stood two gargoyles."
  3: "I saw them drink from little vials right before they charged us."
  4: "Large, clumsy brutes they were."
  5: "They should have been easy to dispatch..."
  6: "But they fought like cyclones!"
  7: "My captain ordered me to go to town for help
  8: when only we two and the gargoyles were left standing."
  9: "Just as I was returning with some guards,
  10: I saw the monsters throwing my friends' dead bodies onto a fire."
  11: "Then the gargoyles looked up at us and fell over, both @dead."

[14215 op=1 keyword; failure -> 14217]
  1: dead

[14216 op=2 display+exit]
  1: "Yes, just like that: all of a sudden!"
  2: "I only wish I could have killed them myself, for @vengeance!"

[14217 op=1 keyword; failure -> 14219]
  1: veng

[14218 op=2 display+exit]
  1: "If they had not burned his body, my captain might have been resurrected."
  2: For a moment, Caradon looks ready to weep.
  3: "The @beasts!"

[14219 op=1 keyword; failure -> 14221]
  1: beas,garg

[14220 op=2 display+exit]
  1: "Monsters with eyes like coals and skin like rock."

[14221 op=1 keyword; failure -> 14223]
  1: bye

[14222 op=2 display+exit]
  1: "Farewell, $G."

[14223 op=2 display+exit]
  1: "I cannot help thee with that."

```

<a id="caretaker-npc-164"></a>
## Caretaker ? NPC 164, port 165

Instruction range: 14809?14847

```text
[14811 op=1 keyword; failure -> 14813]
  1: look

[14812 op=2 display+exit]
  1: an old, winged gargoyle.
  2: An odd device crafted of glass and metal rests upon his face.
  3: The gargoyle ignores you and continues writing.

[14813 op=2 display+exit]
  1: The gargoyle ignores you and continues writing.

[14814 op=1 keyword; failure -> 14817]
  1: look

[14816 op=2 display+exit]
  1: an old, winged gargoyle.
  2: An odd device crafted of glass and metal rests upon his face.
  3: "Yes?"
  4: "To inquire about the reason for yet another interruption."

[14817 op=1 keyword; failure -> 14820]
  1: look

[14819 op=2 display+exit]
  1: an old, winged gargoyle.
  2: An odd device crafted of glass and metal rests upon his face.
  3: "Yes?"
  4: "To inquire about the reason for this interruption."

[14820 op=1 keyword; failure -> 14822]
  1: code

[14821 op=2 display+exit]
  1: "To suppose that you are making a joke."
  2: "To know as well as you that you took the Codex to your side of the world!"

[14822 op=1 keyword; failure -> 14823]
  1: temp,sing "To not be a signpost!"
  2: "To find the Temple of Singularity in the mountains to the north - if you can @fly!"

[14823 op=1 keyword; failure -> 14825]
  1: fly

[14824 op=2 display+exit]
  1: "To be unable to reach the mountain-locked Temple of Singularity on foot."
  2: "To only be able to reach that holy place if you are a winged gargoyle."
  3: "To find this most fitting and proper."
  4: "To see no reason to let human riff-raff into the Temple."

[14825 op=1 keyword; failure -> 14827]
  1: lens

[14826 op=2 display+exit]
  1: "To find what you seek in the @Vortex Chamber."

[14827 op=1 keyword; failure -> 14829]
  1: book,ritu

[14828 op=2 display+exit]
  1: "To find what you seek in the @Librum Chamber."

[14829 op=1 keyword; failure -> 14831]
  1: cube

[14830 op=2 display+exit]
  1: "To regret that that item is lost, stolen by human thieves years ago."
  2: "To have heard the humans mention the name @Stonegate as they fled."
  3: "To have heard them say they would sail through Lost Hope Bay."
  4: "To know nothing more about its whereabouts."

[14831 op=1 keyword; failure -> 14833]
  1: moon

[14832 op=2 display+exit]
  1: "To admit ignorance regarding the location of the moonstones."

[14833 op=1 keyword; failure -> 14835]
  1: ston,gate

[14834 op=2 display+exit]
  1: "To suspect it to be someplace in the human-filled Underworld."

[14835 op=1 keyword; failure -> 14837]
  1: name

[14836 op=2 display+exit]
  1: "To be called the Caretaker, in lieu of a name."
  2: "To be honored by being allowed to perform my @duties as caretaker of this @Hall."

[14837 op=1 keyword; failure -> 14839]
  1: job,dut,care

[14838 op=2 display+exit]
  1: "To care for my race's artifacts that rest here in the @Hall of Knowledge."

[14839 op=1 keyword; failure -> 14841]
  1: hall,know

[14840 op=2 display+exit]
  1: "To keep the treasured artifacts of our race here in this Hall, in the three @chambers."

[14841 op=1 keyword; failure -> 14843]
  1: cham,vort,libr,huma

[14842 op=2 display+exit]
  1: "To have three rooms here:"
  2: "To keep artifacts from the @Underworld in the Human Chamber, to the east."
  3: "To keep valuable books in the Librum Chamber, to the west."
  4: "And to keep the @Cube and the @Lens in the Vortex Chamber, to the south."

[14843 op=1 keyword; failure -> 14845]
  1: unde

[14844 op=2 display+exit]
  1: "Yes, the Underworld."
  2: "To believe you know it as 'Britannia.'"

[14845 op=1 keyword; failure -> 14847]
  1: bye

[14846 op=2 display+exit]
  1: Without another word, the gargoyle returns to his work.

[14847 op=2 display+exit]
  1: "To lack any time for idle chatter."
  2: "To desire you to get to the point and then depart."

```

<a id="charlotte-npc-93"></a>
## Charlotte ? NPC 93, port 94

Instruction range: 8733?8864

```text
[8735 op=1 keyword; failure -> 8737]
  1: yes

[8739 op=2 display+exit]
  1: "Thou knowest best, $Y."

[8741 op=1 keyword; failure -> 8744]
  1: yes

[8743 op=2 display+exit]
  1: "How many bales do you wish to sell?"

[8746 op=2 display+exit]
  1: "Perhaps some other time, then."

[8752 op=1 keyword; failure -> 8755]
  1: yes

[8754 op=2 display+exit]
  1: "How many pieces dost thou need?"

[8757 op=2 display+exit]
  1: "Thou knowest best, $Y."

[8758 op=1 keyword; failure -> 8762]
  1: look

[8761 op=2 display+exit]
  1: a pretty young woman with downcast eyes.
  2: "Good $T, $G."

[8762 op=1 keyword; failure -> 8764]
  1: look

[8763 op=2 display+exit]
  1: a pretty young woman with downcast eyes.
  2: "Hello again, $G."

[8766 op=1 keyword; failure -> 8768]
  1: buy,clot,sell,wool,silk,plan,ball,bag

[8767 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[8768 op=1 keyword; failure -> 8770]
  1: bye

[8769 op=2 display+exit]
  1: "Farewell, $G!"

[8770 op=1 keyword; failure -> 8772]
  1: name

[8771 op=2 display+exit]
  1: "I am called Charlotte @Weaver, $G."

[8772 op=1 keyword; failure -> 8774]
  1: job

[8773 op=2 display+exit]
  1: "I am but a humble @weaver, $G."

[8774 op=1 keyword; failure -> 8776]
  1: weav

[8775 op=2 display+exit]
  1: "I turn @wool into cloth."
  2: "I am not very good, but some people take pity on me and buy my @cloth anyway."

[8776 op=1 keyword; failure -> 8778]
  1: mari

[8777 op=2 display+exit]
  1: "She lives in Paws."

[8778 op=1 keyword; failure -> 8780]
  1: arbe

[8779 op=2 display+exit]
  1: "He lives in Paws."

[8780 op=1 keyword; failure -> 8782]
  1: humb

[8781 op=2 display+exit]
  1: "'Tis really not my place to speak of that, $G."
  2: "But honesty is a virtue too."
  3: "And honesty forces me to admit that I am the humblest person in New Magencia."

[8782 op=1 keyword; failure -> 8785]
  1: buy,clot

[8784 op=2 display+exit]
  1: "My cloth costs 25 gold."
  2: "Interested, $Y?"

[8785 op=1 keyword; failure -> 8795]
  1: sell,wool

[8795 op=1 keyword; failure -> 8798]
  1: sell,wool

[8797 op=2 display+exit]
  1: "Aye, I will buy wool from thee..."
  2: "Will you take $V1 gold for each bale of wool, $Y?"

[8798 op=1 keyword; failure -> 8801]
  1: silk

[8800 op=2 display+exit]
  1: "Aye, $Y, I can weave thy silk thread into cloth."
  2: "It will cost thee 10 gold."
  3: "Interested?"

[8801 op=1 keyword; failure -> 8805]
  1: plan,ball,bag

[8804 op=2 display+exit]
  1: "Take the cloth to @Marissa."
  2: "Mayhap she can sew it into a bag for thee."

[8805 op=1 keyword; failure -> 8809]
  1: plan,ball,bag

[8808 op=2 display+exit]
  1: "I can weave that @silk for thee... Then thou should speak with @Marissa."

[8809 op=1 keyword; failure -> 8812]
  1: plan,ball,bag

[8811 op=2 display+exit]
  1: "If you had some @silk thread, I could help thee."
  2: "Go ask @Arbeth if he has any to spare."

[8812 op=1 keyword; failure -> 8814]
  1: plan,ball,bag

[8813 op=2 display+exit]
  1: "I'm not sure what you mean."

[8814 op=2 display+exit]
  1: "'Tis really not my place to speak of that, $G."

[8817 op=2 display+exit]
  1: "Thou knowest best, $Y."

[8819 op=2 display+exit]
  1: "But 'twould make thee o'erburdened, $Y!"

[8830 op=2 display+exit]
  1: "But thou hast not enough gold, $Y!"

[8832 op=2 display+exit]
  1: "But 'twould make thee o'erburdened, $Y!"

[8833 op=2 display+exit]
  1: "Many thanks!"
  2: After accepting your gold, she hands over the cloth.

[8836 op=2 display+exit]
  1: "Perhaps some other time, then."

[8838 op=2 display+exit]
  1: "I only buy up to 5 bales at one time."

[8852 op=2 display+exit]
  1: "Begging thy pardon, but I see no wool!"

[8855 op=2 display+exit]
  1: "Done!"
  2: She hands you $V1 gold and takes the wool.

[8859 op=2 display+exit]
  1: "But thou hast not enough gold, $Y!"

[8862 op=2 display+exit]
  1: "Begging thy pardon, but I see no silk!"

[8864 op=2 display+exit]
  1: Turning to her loom, Charlotte soon weaves the thread into fine silk cloth.
  2: "Finished!"
  3: She takes the gold and hands over the cloth.

```

<a id="chuckles-npc-8"></a>
## Chuckles ? NPC 8, port 9

Instruction range: 690?776

```text
[690 op=1 keyword; failure -> 692]
  1: look

[691 op=2 display+exit]
  1: a carefree fellow in a ludicrous costume.
  2: "Ho eyo he hum, I've got a @clue! It's important, and just for you!"

[692 op=1 keyword; failure -> 695]
  1: name

[694 op=2 display+exit]
  1: "Why, I'm Chuckles the @jester, of course!"
  2: "You've @heard of me haven't you?"
  3: "Sure you have!"

[695 op=1 keyword; failure -> 697]
  1: job

[696 op=2 display+exit]
  1: "To @jape, to jeer, to prance, to frolic! To keep this place unmelancholic!"
  2: "I practice @tomfoolery for Lord @British - or anyone else who passes too close."

[697 op=1 keyword; failure -> 699]
  1: jape

[698 op=2 display+exit]
  1: "And @jeer!"

[699 op=1 keyword; failure -> 701]
  1: jeer

[700 op=2 display+exit]
  1: "And @prance! To dance a Corybantic dance!"

[701 op=1 keyword; failure -> 703]
  1: pran,danc

[702 op=2 display+exit]
  1: "Jeer and prance! Prance and @frolic!"

[703 op=1 keyword; failure -> 705]
  1: frol

[704 op=2 display+exit]
  1: "I lurch 'round like an @alcoholic!"

[705 op=1 keyword; failure -> 707]
  1: tom

[706 op=2 display+exit]
  1: "'Cause Dick and Harry both called in sick."

[707 op=1 keyword; failure -> 709]
  1: lord,brit

[708 op=2 display+exit]
  1: "Call him Mr. @Nose... But don't tell him I said so!"

[709 op=1 keyword; failure -> 711]
  1: hint

[710 op=2 display+exit]
  1: "If hints were mints and mints were hints, think what a @world it would be!"

[711 op=1 keyword; failure -> 713]
  1: ques,ches

[712 op=2 display+exit]
  1: "A quest, a chest, oh what a @jest!"

[713 op=1 keyword; failure -> 714]
  1: jest

[714 op=1 keyword; failure -> 716]
  1: teac,prea

[715 op=2 display+exit]
  1: "To teach, to preach, what waste of @speech!"

[716 op=1 keyword; failure -> 718]
  1: mint,worl,spee,lurc,alco,word,absu,done

[717 op=2 display+exit]
  1: "I just said @that!"

[718 op=1 keyword; failure -> 720]
  1: that

[719 op=2 display+exit]
  1: "You just said '@that!'"

[720 op=1 keyword; failure -> 722]
  1: nyst

[721 op=2 display+exit]
  1: "Oh, he's no @fun."

[722 op=1 keyword; failure -> 724]
  1: fun

[723 op=2 display+exit]
  1: "I'll show you fun before I'm @done!"

[724 op=1 keyword; failure -> 727]
  1: clue

[726 op=2 display+exit]
  1: "Yes, it's crucial to your @quest. Do you want to know what it is?"

[728 op=1 keyword; failure -> 731]
  1: yes

[730 op=2 display+exit]
  1: "Perhaps I don't want to tell you!"
  2: "Perhaps I'll tell you anyway!"
  3: "Do you think I will?"

[732 op=1 keyword; failure -> 735]
  1: no

[734 op=2 display+exit]
  1: "Perhaps I don't want to tell you!"
  2: "Perhaps I'll tell you anyway!"
  3: "Do you think I will?"

[738 op=1 keyword; failure -> 741]
  1: yes

[740 op=2 display+exit]
  1: "How foolish of you to think so!"

[742 op=1 keyword; failure -> 745]
  1: no

[744 op=2 display+exit]
  1: "Congratulations! You're exactly right!"
  2: "I won't tell you the clue..."
  3: "But I will give you this @hint that may lead you to it..."
  4: "Search the @chest in @Nystul's room."

[747 op=1 keyword; failure -> 750]
  1: bye

[749 op=2 display+exit]
  1: "Have you heard the one about the nun, the dragon, and the drunken penguin?"

[751 op=1 keyword; failure -> 754]
  1: yes

[753 op=2 display+exit]
  1: "Good! Maybe you can @teach it to me someday!"
  2: "I did my best, did I amuse you? If not, I hope I did confuse you."
  3: "Did you enjoy my jokes?"

[755 op=1 keyword; failure -> 758]
  1: no

[757 op=2 display+exit]
  1: "That's too bad... Neither have I!"
  2: "I did my best, did I amuse you? If not, I hope I did confuse you."
  3: "Did you enjoy my jokes?"

[759 op=1 keyword; failure -> 762]
  1: yes

[761 op=2 display+exit]
  1: "You did? Then I'll tell you another one!"
  2: "Riddle me this: Do you know what the goose said to the avatar?"

[763 op=1 keyword; failure -> 766]
  1: no

[765 op=2 display+exit]
  1: "Really? I'll just have to try harder, then!"
  2: "Why is Dupre like a horseshoe?"
  3: "It's because they both have the letter 'r' in them!"
  4: "Hee hee, ho ho, to spread good cheer I go! Come talk to me again and I'll give you another show!"

[767 op=1 keyword; failure -> 769]
  1: yes

[768 op=2 display+exit]
  1: "No, no, no, that's not how it works! You're supposed to say you don't, so I can tell you!"
  2: "Let's try it again."
  3: "Riddle me this: Do you know what the goose said to the avatar?"

[770 op=1 keyword; failure -> 773]
  1: no

[772 op=2 display+exit]
  1: "It said 'ankh ankh!'"
  2: "What did you think of that one?"

[775 op=2 display+exit]
  1: "Well, who asked you, anyway?"
  2: "Oh, that's right, I did!"
  3: "Hee hee, ho ho, to spread good cheer I go! Come talk to me again and I'll give you another show!"

[776 op=2 display+exit]
  1: "Ho ho, hee hee, thou art funnier than me! To ask a jester of $Z!"

```

<a id="compassion-npc-191"></a>
## Compassion ? NPC 191, port 192

Instruction range: 16514?16556

```text
[16518 op=1 keyword; failure -> 16520]
  1: look

[16519 op=2 display+exit]
  1: the altar of Compassion.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about compassion."
  4: Do you wish to meditate at this altar?

[16520 op=1 keyword; failure -> 16523]
  1: yes

[16522 op=2 display+exit]
  1: "Who will meditate?"

[16523 op=1 keyword; failure -> 16525]
  1: no

[16524 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16526 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable compassion."
  3: Do you wish to meditate at this altar?

[16528 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16529 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about compassion."
  3: Do you wish to meditate at this altar?

[16532 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16535 op=2 display+exit]
  1: "Speak the Mantra of Compassion."

[16538 op=1 keyword; failure -> 16540]
  1: mu

[16541 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16542 op=2 display+exit]
  1: "You receive no enlightenment."

[16548 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16549 op=2 display+exit]
  1: "No beast so fierce but shows some touch of pity."
  2: "Sweet mercy is nobility's true badge."
  3: "There is naught else for you to learn here and now."
  4: "Return when thy journey has progressed further."

[16555 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16556 op=2 display+exit]
  1: "No beast so fierce but shows some touch of pity."
  2: "Sweet mercy is nobility's true badge."
  3: $PARTYMEMBER has gained a level...
  4: and dexterity!

```

<a id="conor-npc-92"></a>
## Conor ? NPC 92, port 93

Instruction range: 8677?8732

```text
[8679 op=1 keyword; failure -> 8685]
  1: look

[8682 op=22 display+continue]
  1: a stalwart fellow, with a look of placid concentration.
  2: "Hello, $P."
  3: "It's good to see you again."

[8684 op=2 display+exit]
  1: You think you see a brief flicker of recognition pass between @Iolo and the fisherman.
  2: But before you can be certain, he turns to address you.

[8685 op=1 keyword; failure -> 8690]
  1: look

[8687 op=22 display+continue]
  1: a stalwart fellow, with a look of placid concentration.
  2: "Good $T."
  3: "Is there anything I can do for you?"

[8689 op=2 display+exit]
  1: You think you see a brief flicker of recognition pass between @Iolo and the fisherman.
  2: But before you can be certain, he turns to address you.

[8690 op=1 keyword; failure -> 8693]
  1: look

[8692 op=2 display+exit]
  1: a stalwart fellow, with a look of placid concentration.
  2: "Hello, $P."
  3: "It's good to see you again."

[8693 op=1 keyword; failure -> 8695]
  1: look

[8694 op=2 display+exit]
  1: a stalwart fellow, with a look of placid concentration.
  2: "Good $T."
  3: "Is there anything I can do for you?"

[8695 op=1 keyword; failure -> 8698]
  1: name

[8697 op=2 display+exit]
  1: "I am Conor Starfalcon."

[8698 op=1 keyword; failure -> 8701]
  1: name

[8700 op=2 display+exit]
  1: "Oh, forgive me for not introducing myself."
  2: "I am Conor Starfalcon."
  3: "And what is your name?"

[8701 op=1 keyword; failure -> 8703]
  1: iolo

[8702 op=2 display+exit]
  1: "We've met before."
  2: "But that was many @years ago."

[8703 op=1 keyword; failure -> 8705]
  1: met,befo,many,year,ago

[8704 op=2 display+exit]
  1: "Yes, long ago."

[8705 op=1 keyword; failure -> 8707]
  1: job

[8706 op=2 display+exit]
  1: "I am a fisherman."
  2: "In catching @fish, as in all of life, there is something to be learned."

[8707 op=1 keyword; failure -> 8709]
  1: fish,catc,life,lear

[8708 op=2 display+exit]
  1: "I still have not mastered the @way of the fisherman."
  2: "But I think perhaps the key to enlightenment lies not so much in the destination as in the road one travels."

[8709 op=1 keyword; failure -> 8711]
  1: mast,way,road,enli,dest,trav

[8710 op=2 display+exit]
  1: "I am satisfied to live here and learn what I can from the fish."

[8711 op=1 keyword; failure -> 8713]
  1: shri

[8712 op=2 display+exit]
  1: "The Shrine of Humility is far to the southeast."
  2: "I'd be glad to take you there in my @boat, but I'm afraid it is too small to weather the high seas."

[8713 op=1 keyword; failure -> 8715]
  1: boat

[8714 op=2 display+exit]
  1: "It's down at the beach."
  2: "Feel free to borrow it if you have need."

[8715 op=1 keyword; failure -> 8717]
  1: humb,humi

[8716 op=2 display+exit]
  1: "You seek to find the humblest in New Magincia?"
  2: "That's a difficult task."
  3: "I'd hate to commend one of my neighbors to you and fail to do justice to the others."
  4: "The one thing I can tell you for certain is that it isn't me you're looking for."

[8717 op=1 keyword; failure -> 8719]
  1: mant

[8718 op=2 display+exit]
  1: "The Mantra of Humility is 'lum'."
  2: "If you seek the @shrine, I hope you find it."

[8719 op=1 keyword; failure -> 8721]
  1: guil,kora

[8720 op=2 display+exit]
  1: "I used to be a guildmaster."
  2: "But that was long ago."

[8721 op=1 keyword; failure -> 8723]
  1: shie,war

[8722 op=2 display+exit]
  1: "I no longer follow the way of the warrior."

[8723 op=1 keyword; failure -> 8725]
  1: bye

[8724 op=2 display+exit]
  1: "If I can help you with anything else, let me know."

[8725 op=2 display+exit]
  1: "I can't help you with that."

[8732 op=2 display+exit]
  1: "Well met, $G!"

```

<a id="culham-npc-47"></a>
## Culham ? NPC 47, port 48

Instruction range: 4242?4303

```text
[4250 op=2 display+exit]
  1: "Well met, $G $Z. Let me tell ye a @tale."

[4251 op=2 display+exit]
  1: "Oho, that be a name in need of a story."

[4253 op=1 keyword; failure -> 4255]
  1: yes

[4254 op=2 display+exit]
  1: "Oh. Perhaps a @song, then."

[4255 op=2 display+exit]
  1: "Now there's a @tale."

[4257 op=1 keyword; failure -> 4259]
  1: look

[4258 op=2 display+exit]
  1: a small man in a handsome vest made of seashells.
  2: "It's fighting time. Talking's for later."

[4260 op=2 display+exit]
  1: "It's fighting time. Talking's for later."

[4261 op=1 keyword; failure -> 4265]
  1: look

[4264 op=2 display+exit]
  1: a small man in a handsome vest made of seashells.
  2: "Why, hullo there, $P. Have I told ye the one about the gypsies?"

[4265 op=1 keyword; failure -> 4269]
  1: look

[4268 op=2 display+exit]
  1: a small man in a handsome vest made of seashells.
  2: "Why, hullo there, $G. And who might ye be?"

[4269 op=1 keyword; failure -> 4271]
  1: name

[4270 op=2 display+exit]
  1: "Culham's me name, $G."

[4271 op=1 keyword; failure -> 4273]
  1: job

[4272 op=2 display+exit]
  1: "When @spirits be low, I play tunes to bring @cheer."

[4273 op=1 keyword; failure -> 4275]
  1: spir

[4274 op=2 display+exit]
  1: "Once I heard a haunting melody..."
  2: "...in Old @Magincia." He laughs.

[4275 op=1 keyword; failure -> 4277]
  1: old,magi,haun

[4276 op=2 display+exit]
  1: "Well, it used to be a ghost town."
  2: He laughs and plays a little tune on his lute.

[4277 op=1 keyword; failure -> 4279]
  1: chee,mant

[4278 op=2 display+exit]
  1: "The Mantra of Valour bringeth great cheer."
  2: "I'm not a great singer, but I love to play song of valour."
  3: "It makes me @sing."

[4279 op=1 keyword; failure -> 4281]
  1: sing,song

[4280 op=2 display+exit]
  1: "I'll sing, though I'd rather @play."

[4281 op=1 keyword; failure -> 4284]
  1: gyps,hear,tell,tale,stor

[4283 op=2 display+exit]
  1: "Would ye really like to hear a story?"

[4284 op=1 keyword; failure -> 4289]
  1: play,tune

[4288 op=2 display+exit]
  1: The pub is filled with the sound of singing.
  2: "It seemed like such an easy climb
  3: I crawled upon the Serpent's Spine
  4: I hadn't even time to blanch
  5: When down there came an avalanche!"
  6: "And then my mantra came to mind
  7: Tho boulders bounced and passed with speed.
  8: My frozen fingers gripped to rock
  9: As sliding snow swept loose my feet."
  10: "Sing 'ra,' my friends, sing 'ra.'
  11: 'Tis a song to make thee strong
  12: When the mountains be high
  13: And the ground be far..."
  14: "...Sing 'ra,' my friends, sing 'ra'!"

[4289 op=1 keyword; failure -> 4291]
  1: play,tune

[4290 op=2 display+exit]
  1: Culham sings for you.
  2: "It seemed like such an easy climb
  3: I crawled upon the Serpent's Spine
  4: I hadn't even time to blanch
  5: When down there came an avalanche!"
  6: "And then my mantra came to mind
  7: Tho boulders bounced and passed with speed.
  8: My frozen fingers gripped to rock
  9: As sliding snow swept loose my feet."
  10: "Sing 'ra,' my friends, sing 'ra.'
  11: 'Tis a song to make thee strong
  12: When the mountains be high
  13: And the ground be far..."
  14: "...Sing 'ra,' my friends, sing 'ra'!"

[4291 op=1 keyword; failure -> 4295]
  1: rune,mous,rat,naug,noma

[4294 op=2 display+exit]
  1: "Ah, yes. That's quite a story, too."
  2: "Naughty Nomaan dropped the Rune of Valor, and the rat carried it off."
  3: He laughs loudly.
  4: Too loudly. Stelnar and Jerris begin to argue over whether it was a rat or a mouse.
  5: Once again a brawl breaks out!

[4295 op=1 keyword; failure -> 4297]
  1: rune,mous,rat,naug,noma

[4296 op=2 display+exit]
  1: "Ah, yes. That's quite a story, too."
  2: "Naughty Nomaan dropped the Rune of Valor, and the rat carried it off."
  3: He laughs loudly.

[4297 op=1 keyword; failure -> 4299]
  1: bye

[4298 op=2 display+exit]
  1: "May we meet again soon, friend."

[4299 op=2 display+exit]
  1: "Sorry, I cannot help ye with that."

[4301 op=1 keyword; failure -> 4303]
  1: yes

[4302 op=2 display+exit]
  1: "I call this story..."
  2: "Arturosis"
  3: "Two gypsies come to Trinsic, hungry as usual."
  4: "One goes out and gathers a poisonous weed from the swamps."
  5: "The other sneaks into town and puts the plant into the trough of a horse."
  6: "Well, poor horse doesn't know better."
  7: "He eats the stuff, swells up and takes ill."
  8: "The owner, Immanuelle, doesn't know what to do."
  9: "Along come the gypsies. 'Aiee!' they cry."
  10: "'Yonder nag hath contracted 'Arturosis.'"
  11: "Unless it be immediately removed, the rest shall catch the same fatal disease.'"
  12: "Immanuelle decides to sacrifice the diseased one and the gypsies agree heartily."
  13: "'For but a few coins,' say the gypsies,' we will lead it away and bury it far from here.'"
  14: "Immanuelle is too happy to hand them the doomed animal's bridle, plus ten coins."
  15: "So the gypsies make a great kettle of horse stew and bury the rest."
  16: "A great feast is held 'mongst gypsykind."
  17: "Meanwhile, Immanuelle thanks virtue that the equine epidemic was caught early on!"
  18: "'Tis a true story, my friend."

[4303 op=2 display+exit]
  1: "Well, not everyone likes gypsy tales, I suppose."

```

<a id="cullen-npc-23"></a>
## Cullen ? NPC 23, port 24

Instruction range: 1928?2029

```text
[1929 op=1 keyword; failure -> 1932]
  1: look

[1931 op=2 display+exit]
  1: a short, dark man, covered in white flour.
  2: "Hello again, my friend!"
  3: As he shakes your hand enthusiastically, clouds of flour fly off his clothes.

[1932 op=1 keyword; failure -> 1935]
  1: look

[1934 op=2 display+exit]
  1: a short, dark man, covered in white flour.
  2: "Hello there, my friend!"
  3: As he shakes your hand enthusiastically, clouds of flour fly off his clothes.

[1935 op=1 keyword; failure -> 1938]
  1: name

[1937 op=2 display+exit]
  1: "I'm Cullen, Cullen the @Baker."

[1938 op=1 keyword; failure -> 1940]
  1: job

[1939 op=2 display+exit]
  1: "I'd have thought that'd be obvious! I'm a @baker!"
  2: As he laughs heartily, more clouds of flour rise off his clothing.
  3: "You can @buy bakery goods from me or perhaps you have some flour to @sell, eh?"

[1940 op=1 keyword; failure -> 1942]
  1: bake

[1941 op=2 display+exit]
  1: "I bake goods for Lord British himself!" the man says proudly.
  2: "If you'd like to @buy some, just say the word!"

[1944 op=1 keyword; failure -> 1946]
  1: buy

[1945 op=2 display+exit]
  1: "Well, which will it be then, @bread, @cake or @rolls?"

[1946 op=1 keyword; failure -> 1949]
  1: brea

[1948 op=2 display+exit]
  1: "Those cost 3 gold each."
  2: "How many do you want, $Y?"

[1949 op=1 keyword; failure -> 1952]
  1: cake

[1951 op=2 display+exit]
  1: "Those cost 5 gold each."
  2: "How many do you want, $Y?"

[1952 op=1 keyword; failure -> 1955]
  1: roll

[1954 op=2 display+exit]
  1: "Those cost 1 gold each."
  2: "How many do you want, $Y?"

[1969 op=1 keyword; failure -> 1979]
  1: sell,flou

[1979 op=1 keyword; failure -> 1982]
  1: sell,flou

[1981 op=2 display+exit]
  1: He turns to you and says "Will you take $V1 gold for every sack of flour?"

[1984 op=1 keyword; failure -> 1986]
  1: buy,brea,cake,roll

[1985 op=2 display+exit]
  1: "Come to my shop when it's open!"

[1986 op=1 keyword; failure -> 1988]
  1: bye

[1987 op=2 display+exit]
  1: "Come again soon!"

[1988 op=2 display+exit]
  1: "Hmmm... $Z..."
  2: "$Z..."
  3: "Nope, I don't know anything about that!"

[1991 op=2 display+exit]
  1: "Suit thyself, but thou'lt find no better!"

[1993 op=2 display+exit]
  1: "Whoa! Don't buy out my entire stock!"

[1999 op=2 display+exit]
  1: ""Thou hast not enough gold for that many!"

[2001 op=2 display+exit]
  1: "Thou canst not carry that many!"

[2002 op=2 display+exit]
  1: "Here you are, $Y."
  2: "Plus one for luck!"

[2005 op=2 display+exit]
  1: "How many can you sell me?"

[2007 op=2 display+exit]
  1: "Sorry, you haven't any flour."

[2010 op=2 display+exit]
  1: "Changed your mind, eh?" he folds his arms.
  2: "Well, what else will it be then?"

[2012 op=2 display+exit]
  1: "I only need up to 10 at a time."

[2026 op=2 display+exit]
  1: "Changed your mind, eh?" he folds his arms.
  2: "Well, what else will it be then?"

[2029 op=2 display+exit]
  1: "Done!"
  2: He hands you $V1 gold and takes the flour.

```

<a id="dale-npc-68"></a>
## Dale ? NPC 68, port 69

Instruction range: 6327?6464

```text
[6328 op=1 keyword; failure -> 6331]
  1: yes

[6330 op=2 display+exit]
  1: "How many do you wish to buy?"

[6333 op=2 display+exit]
  1: "Perhaps something else then."

[6337 op=1 keyword; failure -> 6340]
  1: yes

[6339 op=2 display+exit]
  1: "It will cost thee 5 gems! Art thou certain?"

[6341 op=1 keyword; failure -> 6348]
  1: yes

[6347 op=2 display+exit]
  1: He turns to the furnace and begins crafting the sword.
  2: Soon it is finished, and you marvel at its beauty!
  3: "There she is, and a finer work thou'lt never see!"

[6349 op=1 keyword; failure -> 6353]
  1: yes

[6352 op=2 display+exit]
  1: "But thou cannot carry such a sword!"

[6354 op=1 keyword; failure -> 6357]
  1: yes

[6356 op=2 display+exit]
  1: "Sorry, $Y, you haven't enough gems."

[6359 op=2 display+exit]
  1: "Hmm," he scowls, "perhaps I was mistaken about thee..."

[6362 op=2 display+exit]
  1: "Hmm," he scowls, "perhaps I was mistaken about thee..."

[6366 op=1 keyword; failure -> 6371]
  1: glas

[6370 op=2 display+exit]
  1: "That Glass is 5 gold."
  2: "Interested?"

[6371 op=1 keyword; failure -> 6376]
  1: mirr

[6375 op=2 display+exit]
  1: "That Mirror is 85 gold"
  2: "Interested?"

[6376 op=1 keyword; failure -> 6379]
  1: jar

[6378 op=2 display+exit]
  1: "Those jars are 5 gold each."
  2: "Interested?"

[6379 op=2 display+exit]
  1: "Perhaps something else then."

[6383 op=1 keyword; failure -> 6386]
  1: yes

[6387 op=2 display+exit]
  1: "Perhaps something else then."

[6390 op=2 display+exit]
  1: "You can't afford that much!"

[6392 op=2 display+exit]
  1: "You can't carry that much!"

[6393 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, he hands over the merchandise.

[6398 op=2 display+exit]
  1: "Perhaps something else then."

[6400 op=2 display+exit]
  1: "You can't carry that much!"

[6405 op=2 display+exit]
  1: "You can't afford that much!"

[6407 op=2 display+exit]
  1: "You can't carry that much!"

[6408 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, he hands over the merchandise.

[6409 op=1 keyword; failure -> 6412]
  1: buy

[6411 op=2 display+exit]
  1: "Come to my shop when it's open!"

[6412 op=1 keyword; failure -> 6415]
  1: gem

[6414 op=2 display+exit]
  1: "Come to my shop when it's open!"

[6415 op=1 keyword; failure -> 6418]
  1: buy

[6417 op=2 display+exit]
  1: "Which item? A @glass, some @jars or a @mirror?"

[6418 op=1 keyword; failure -> 6420]
  1: look

[6419 op=2 display+exit]
  1: a short, barrel-chested man.
  2: "A good $T to thee, Avatar."

[6420 op=1 keyword; failure -> 6423]
  1: name

[6422 op=2 display+exit]
  1: "I am Dale the @Glassblower."

[6423 op=1 keyword; failure -> 6425]
  1: job

[6424 op=2 display+exit]
  1: "I make @glassware, the finest in the land!"

[6425 op=1 keyword; failure -> 6431]
  1: glas

[6426 op=1 keyword; failure -> 6431]
  1: crys

[6430 op=2 display+exit]
  1: "I require a gem for the crafting and 250 gold."
  2: "Dost thou wish me to craft thee a glass crystal?"

[6431 op=1 keyword; failure -> 6433]
  1: glas

[6432 op=2 display+exit]
  1: "If you'd like to @buy some of my glassware, just say the word!"

[6433 op=1 keyword; failure -> 6435]
  1: swor

[6434 op=2 display+exit]
  1: "Aye, I know how to make glass swords."
  2: "But there is little @demand for them."

[6435 op=1 keyword; failure -> 6437]
  1: dema

[6436 op=2 display+exit]
  1: "I require five gems for the crafting."
  2: "This is too expensive for most."
  3: "Aye, but things of @beauty they are..."

[6437 op=1 keyword; failure -> 6439]
  1: beau

[6438 op=2 display+exit]
  1: "Thou seemst like a person who appreciates art."
  2: "I'll make thee one, if thou hast the @gems."

[6439 op=1 keyword; failure -> 6441]
  1: crys,lens,tele

[6440 op=2 display+exit]
  1: "Hmmm...You'd have to see a lensmaker about that."
  2: "I hear there's one near the Lycaeum."

[6441 op=1 keyword; failure -> 6445]
  1: gem

[6444 op=2 display+exit]
  1: "Aye, $Y, I see the gems you carry."
  2: "Dost thou wish me to craft thee a glass sword?"

[6445 op=1 keyword; failure -> 6447]
  1: gem

[6446 op=2 display+exit]
  1: "Aye, a glass sword needs fine gems to complete its beauty!"
  2: "Sorry, $Y, you haven't enough gems."

[6447 op=1 keyword; failure -> 6449]
  1: bye

[6448 op=2 display+exit]
  1: "A fine $T to thee!"

[6449 op=2 display+exit]
  1: "Sorry?"

[6451 op=1 keyword; failure -> 6459]
  1: yes

[6458 op=2 display+exit]
  1: He turns to the furnace and begins crafting the glass crystal.
  2: Soon it is finished, and he hands it over to you.
  3: "There she is, and a finer work thou'lt never see!"

[6459 op=1 keyword; failure -> 6462]
  1: yes

[6461 op=2 display+exit]
  1: "You can't afford that much!"

[6462 op=1 keyword; failure -> 6464]
  1: yes

[6463 op=2 display+exit]
  1: "Sorry, $Y, you haven't enough gems."

[6464 op=2 display+exit]
  1: "Perhaps something else then."

```

<a id="dargoth-npc-35"></a>
## Dargoth ? NPC 35, port 36

Instruction range: 3234?3354

```text
[3234 op=1 keyword; failure -> 3236]
  1: look

[3235 op=2 display+exit]
  1: a stern, severe man in white robes.
  2: You interrupt the man from his research.
  3: "Yes, yes, what is it?"
  4: "You've come for healing, I suppose."

[3236 op=1 keyword; failure -> 3238]
  1: name

[3237 op=2 display+exit]
  1: "I am Dargoth, master of medical lore."

[3238 op=1 keyword; failure -> 3240]
  1: job

[3239 op=2 display+exit]
  1: "My true calling is research."
  2: "My studies here have advanced the healing art throughout the world!"
  3: "But, more often than not, I am called upon to @cure poison ivy and @heal paper cuts!"
  4: "I also offer @insurance to those expecting to need my services in the future."

[3240 op=1 keyword; failure -> 3243]
  1: insu

[3242 op=2 display+exit]
  1: He raises an eyebrow.
  2: "You have already paid for insurance!"

[3243 op=1 keyword; failure -> 3250]
  1: insu

[3249 op=2 display+exit]
  1: "By paying in advance, gold will not be an issue should your body arrive on my doorstep."
  2: "It will cost you $V0 gold, interested?"

[3251 op=1 keyword; failure -> 3261]
  1: yes

[3260 op=2 display+exit]
  1: You give him the gold.
  2: "I look forward to seeing you in the future, dead or alive."
  3: "Perhaps then you will provide me with an injury worthy of my skill!"

[3262 op=1 keyword; failure -> 3265]
  1: yes

[3264 op=2 display+exit]
  1: "I need to be paid to offer you insurance."

[3267 op=2 display+exit]
  1: He looks disappointed.
  2: "It will be such a waste of my skills for your body to rot in a dungeon."

[3275 op=2 display+exit]
  1: "It will cost you $V0 gold for me to heal you, interested?"

[3279 op=1 keyword; failure -> 3282]
  1: heal

[3281 op=2 display+exit]
  1: "Which of you?"

[3284 op=2 display+exit]
  1: "I beg thy pardon?"

[3286 op=1 keyword; failure -> 3288]
  1: yes

[3290 op=2 display+exit]
  1: "I need to be paid to heal you."

[3291 op=1 keyword; failure -> 3294]
  1: cure

[3293 op=2 display+exit]
  1: "It'll cost 10 gold to cure you, interested?"

[3295 op=1 keyword; failure -> 3298]
  1: yes

[3297 op=2 display+exit]
  1: "Which of you?"

[3300 op=2 display+exit]
  1: "If you can't pay, I can't cure you."

[3301 op=1 keyword; failure -> 3303]
  1: resu

[3304 op=1 keyword; failure -> 3306]
  1: yes

[3308 op=2 display+exit]
  1: "Resurrection is a difficult task."
  2: "I'm sorry, but I need to be paid."

[3309 op=1 keyword; failure -> 3311]
  1: yes

[3310 op=2 display+exit]
  1: "Well, what can I do for thee?"
  2: "Come, come, get to the point. I'm a busy man!"

[3311 op=1 keyword; failure -> 3313]
  1: bye,no

[3312 op=2 display+exit]
  1: "Stay out of trouble!"
  2: "I don't want to see my fine workmanship wasted!"

[3313 op=2 display+exit]
  1: "I beg thy pardon?"

[3317 op=2 display+exit]
  1: "I beg thy pardon?"

[3323 op=2 display+exit]
  1: "I need to be paid to heal you."

[3326 op=2 display+exit]
  1: "I can do nothing for those scars."
  2: "Still, they look healed."
  3: "What else do you need?"

[3328 op=2 display+exit]
  1: "The injury is most interesting!"
  2: "Ah! $PARTYMEMBER, you have an injury worthy of my skill!"
  3: Laying hands upon $PARTYMEMBER, Dargoth mends the wounds.
  4: "What else do you need?"

[3332 op=2 display+exit]
  1: "If you can't pay, I can't cure you."

[3335 op=2 display+exit]
  1: "You don't look good, but you're not poisoned."
  2: "What else do you need?"

[3337 op=2 display+exit]
  1: "Fascinating, $PARTYMEMBER! I have read about such poison only in ancient books!"
  2: "I haven't gotten many cases like this since they invented swamp boots."
  3: Laying hands upon $PARTYMEMBER, Dargoth removes the poison.
  4: "What else do you need?"

[3342 op=2 display+exit]
  1: "For $V0 gold I can attempt resurrection, interested?"

[3347 op=2 display+exit]
  1: "That ain't enough money."
  2: "Go see a gravedigger."
  3: "I'll reckon his price'll be lower."

[3350 op=2 display+exit]
  1: He raises an eyebrow.
  2: "You may not be my choice for dinner conversation, but none of you are dead."

[3352 op=2 display+exit]
  1: "Oh."
  2: Seeing the dead body that you carry, he becomes more polite.
  3: "I am sorry you've lost a compatriot."
  4: Dargoth lays hands upon the corpse...
  5: "Doman..."
  6: "thixus..."
  7: "anretu!"
  8: "I am sorry, your lost compatriot cannot be resurrected."

[3354 op=2 display+exit]
  1: "Oh."
  2: Seeing the dead body that you carry, he becomes more polite.
  3: "I am sorry you've lost a compatriot."
  4: Dargoth lays hands upon the corpse...
  5: "Doman..."
  6: "thixus..."
  7: "anretu!"
  8: And the dead live again!

```

<a id="daros-npc-182"></a>
## Daros ? NPC 182, port 183

Instruction range: 15988?16019

```text
[15991 op=1 keyword; failure -> 15995]
  1: look

[15994 op=2 display+exit]
  1: a skinny, awkward looking man, wearing boots that come up past his knees.
  2: "Hey hey hey!"
  3: "Welcome to the finest sewers in Britannia."
  4: "You look like you need a nickname, so I'll call you 'ducks.'"

[15995 op=1 keyword; failure -> 15997]
  1: look

[15996 op=2 display+exit]
  1: a skinny, awkward looking man, wearing boots that come up past his knees.
  2: "Hey hey hey!"
  3: "Welcome to the finest sewers in Britannia."

[15997 op=1 keyword; failure -> 15999]
  1: name

[15998 op=2 display+exit]
  1: "My name's Daros, old duck."
  2: He shakes your hand.

[15999 op=1 keyword; failure -> 16001]
  1: job

[16000 op=2 display+exit]
  1: "Well, ducky, I keep the @sewers running the way they should."
  2: "When work is slow, sometimes I play a @joke or two on my @friends to keep things lively."
  3: He winks at you.

[16001 op=1 keyword; failure -> 16003]
  1: frie

[16002 op=2 display+exit]
  1: "Even though we don't know each other that well, I'd like to think of you as my friend already."

[16003 op=1 keyword; failure -> 16005]
  1: play,joke

[16004 op=2 display+exit]
  1: "Oh you know, ducks, the usual kinds."
  2: He nudges you in the ribs.

[16005 op=1 keyword; failure -> 16007]
  1: sewe,work

[16006 op=2 display+exit]
  1: "@Britain is the biggest city in the world, and all those people use a lot of @water."
  2: "All the runoff drains down here."
  3: "It's not a bad place to work, really, except for the @rats."

[16007 op=1 keyword; failure -> 16009]
  1: rat

[16008 op=2 display+exit]
  1: "They're pretty big, ducky, but not to worry."
  2: "They usually won't eat you until after they've killed you first."
  3: "Of course, if you have a set of @panpipes, I hear you can just pipe them away..."

[16009 op=1 keyword; failure -> 16011]
  1: pan,pipe

[16010 op=2 display+exit]
  1: "I don't have a set myself."
  2: "I usually just @run away."

[16011 op=1 keyword; failure -> 16013]
  1: run,away

[16012 op=2 display+exit]
  1: "It's real good exercise!"

[16013 op=1 keyword; failure -> 16015]
  1: wate,runo,drai

[16014 op=2 display+exit]
  1: "I wouldn't go swimming around here, ducks."

[16015 op=1 keyword; failure -> 16017]
  1: brit

[16016 op=2 display+exit]
  1: "Go back up and have a look!"
  2: "I'm sure it's still there."
  3: He grins, obviously delighted with his own sense of humor.

[16017 op=1 keyword; failure -> 16019]
  1: bye

[16018 op=2 display+exit]
  1: "See you around, old duck."
  2: He gives you a hearty slap on the back as you leave,
  3: which almost knocks one of the straps on your pack loose.

[16019 op=2 display+exit]
  1: "That's not something I talk about much, down here..."

```

<a id="daver-npc-19"></a>
## Daver ? NPC 19, port 20

Instruction range: 1703?1734

```text
[1703 op=1 keyword; failure -> 1705]
  1: look

[1704 op=2 display+exit]
  1: a man wearing a long grey cloak.
  2: He wears gloves with the fingers cut off, and carries a large hourglass with him.
  3: "Good $T, friend Avatar."

[1705 op=1 keyword; failure -> 1708]
  1: name

[1707 op=2 display+exit]
  1: "I'm Daver McCord."

[1708 op=1 keyword; failure -> 1710]
  1: job

[1709 op=2 display+exit]
  1: "Every hour of the day, I ring the @bell a number of times."

[1710 op=1 keyword; failure -> 1712]
  1: bell

[1711 op=2 display+exit]
  1: "According to the @numbers on the hourglass, you see."

[1712 op=1 keyword; failure -> 1714]
  1: numb

[1713 op=2 display+exit]
  1: "Well, I usually know what @time it is up here in my head somehow."
  2: "But just in case, I start this hourglass at sunup, turning it over every nine hours."

[1714 op=1 keyword; failure -> 1716]
  1: time

[1715 op=2 display+exit]
  1: "It is especially important for those with shops."
  2: "They know when to open for @business, eat lunch, close, and that sort of thing."
  3: "Another of Lord British's innovations, this bell thing."

[1716 op=1 keyword; failure -> 1718]
  1: avat

[1717 op=2 display+exit]
  1: "You look much taller in person. Woodroffe's painting does not do you @justice."

[1718 op=1 keyword; failure -> 1720]
  1: just

[1719 op=2 display+exit]
  1: "Heh, heh. I know little of that. I'm just the bell-ringer."

[1720 op=1 keyword; failure -> 1722]
  1: open,busi,sort

[1721 op=2 display+exit]
  1: "Hmmm. I guess that you don't keep a regular schedule, being the @Avatar and all."
  2: "Probably out till all hours, collecting @runes and such."

[1722 op=1 keyword; failure -> 1724]
  1: rune

[1723 op=2 display+exit]
  1: "Magical little things."
  2: "There's a symbol on one side, and a letter on the other."
  3: "Probably spell something out if you put 'em all on a necklace."
  4: "But that hasn't been done for a long time."
  5: "After the Codex of Ultimate Wisdom was brought back from the underworld,
  6: Lord @British gave the runes to the lords of the eight cities."

[1724 op=1 keyword; failure -> 1726]
  1: eigh,citi,gave

[1725 op=2 display+exit]
  1: "Let's see now."
  2: He counts them off on his fingers.
  3: "Minoc, Trinsic, Jhelom, Yew, Skara Brae, Britain, Moonglow..."
  4: "...Ah yes, and New Magincia. I hear each of the lords knows a @mantra."

[1726 op=1 keyword; failure -> 1728]
  1: mant

[1727 op=2 display+exit]
  1: "Haven't the foggiest what those are for."
  2: "Something to do with the @shrines, I think."

[1728 op=1 keyword; failure -> 1730]
  1: shri

[1729 op=2 display+exit]
  1: "Always wanted to visit them. Never had the @time."
  2: "Heh, heh, heh!"

[1730 op=1 keyword; failure -> 1732]
  1: lord,brit

[1731 op=2 display+exit]
  1: "Lord British appointed lords to rule the eight @cities."

[1732 op=1 keyword; failure -> 1734]
  1: bye

[1733 op=2 display+exit]
  1: "Until we meet again."

[1734 op=2 display+exit]
  1: "Sorry, I cannot help you with that."

```

<a id="de-hugh-npc-155"></a>
## de Hugh ? NPC 155, port 156

Instruction range: 14274?14337

```text
[14275 op=1 keyword; failure -> 14278]
  1: yes

[14277 op=2 display+exit]
  1: "Good!"
  2: "I seek @courageous warriors to accompany me."

[14280 op=2 display+exit]
  1: "Then why hast thou come to Serpent's Hold?"

[14282 op=1 keyword; failure -> 14285]
  1: yes

[14284 op=2 display+exit]
  1: "Then surely Koronada has heard of ye."
  2: "Tell him your name and he may invite ye to join the Silver Serpent @guild."

[14287 op=2 display+exit]
  1: "Well, actually, neither have I!"

[14295 op=2 display+exit]
  1: "Well, `$Z, I am Baron de Hugh."

[14300 op=1 keyword; failure -> 14302]
  1: look

[14301 op=2 display+exit]
  1: a commanding presence: a tall warrior in black armor.
  2: He stands and bows to you.
  3: "`$G."
  4: "Art thou a warrior?"

[14302 op=2 display+exit]
  1: He stands and bows to you.
  2: "`$G."
  3: "Art thou a warrior?"

[14303 op=1 keyword; failure -> 14305]
  1: look

[14304 op=2 display+exit]
  1: a commanding presence: a tall warrior in black armor.
  2: "`$G."

[14305 op=1 keyword; failure -> 14308]
  1: name

[14307 op=2 display+exit]
  1: "I am Baron de Hugh."

[14308 op=1 keyword; failure -> 14312]
  1: name

[14311 op=2 display+exit]
  1: "Who wants to know?"

[14312 op=1 keyword; failure -> 14314]
  1: job

[14313 op=2 display+exit]
  1: "I am a @mercenary now."

[14314 op=1 keyword; failure -> 14316]
  1: join,merc

[14315 op=2 display+exit]
  1: "First I shall gather an @army."

[14316 op=1 keyword; failure -> 14318]
  1: army,warr,acco

[14317 op=2 display+exit]
  1: "I plan to build my reputation as a great fighter."
  2: "My army and I will kill the mighty daemon @Sin'Vraal!"

[14318 op=1 keyword; failure -> 14320]
  1: demo,daem,kill,sin,vraa

[14319 op=2 display+exit]
  1: "He is a malevolent creature, lord over numerous servants who crawl about the @desert."

[14320 op=1 keyword; failure -> 14322]
  1: dese

[14321 op=2 display+exit]
  1: "It is on the north-eastern tip of Britannia."
  2: "Look it up on a map."

[14322 op=1 keyword; failure -> 14324]
  1: cour

[14323 op=2 display+exit]
  1: "It is foolish to be courageous unless one is backed by a well-trained force of fighters."
  2: "Or unless one happens to be the @Avatar, I imagine."

[14324 op=1 keyword; failure -> 14326]
  1: avat

[14325 op=2 display+exit]
  1: "I heard at the @guild that the Avatar was in town!"

[14326 op=1 keyword; failure -> 14329]
  1: guil,orde

[14328 op=2 display+exit]
  1: "@Koronada says I will be considered for membership when I have done something noteworthy."
  2: "Have ye ever done aught worthy of note?"

[14329 op=1 keyword; failure -> 14331]
  1: koro

[14330 op=2 display+exit]
  1: "He is firm but fair."
  2: "He does not wish to see any unworthy person bear the Silver Serpent @blazon."

[14331 op=1 keyword; failure -> 14333]
  1: silv,serp

[14332 op=2 display+exit]
  1: "I believe the silver serpent is a quick and highly venomous creature."

[14333 op=1 keyword; failure -> 14335]
  1: blaz

[14334 op=2 display+exit]
  1: "A blazon is a shield marking, such as a @serpent."

[14335 op=1 keyword; failure -> 14337]
  1: bye

[14336 op=2 display+exit]
  1: "Good luck, $G."

[14337 op=2 display+exit]
  1: "That I cannot help ye with."

```

<a id="derydlus-npc-40"></a>
## Derydlus ? NPC 40, port 41

Instruction range: 3630?3667

```text
[3630 op=1 keyword; failure -> 3633]
  1: look

[3632 op=2 display+exit]
  1: a man who looks very amused--whether with his surroundings or himself is hard to say.
  2: "Ah, welcome, $G! Come have a @drink with me!"
  3: "Have you had your fortune told yet?"

[3635 op=2 display+exit]
  1: "Some people think @Penumbra's a hoax. But she knows her stuff. You should bear with her - she can truly be of great assistance to you."

[3636 op=1 keyword; failure -> 3640]
  1: name

[3639 op=2 display+exit]
  1: "Derydlus. And what was yours again?"

[3647 op=2 display+exit]
  1: "Oh, yes, of course. Come, have a seat."

[3648 op=1 keyword; failure -> 3650]
  1: job

[3649 op=2 display+exit]
  1: "Trying to discover all the subtleties of Frasier's @Folly is almost a career in itself!"
  2: He smiles.

[3650 op=1 keyword; failure -> 3652]
  1: drin

[3651 op=2 display+exit]
  1: "Aye, this is the home of Frasier's @Folly!"

[3652 op=1 keyword; failure -> 3654]
  1: subt,disc,fras,foll

[3653 op=2 display+exit]
  1: "In all the towns of Britannia, you'll find no better brew."

[3654 op=1 keyword; failure -> 3656]
  1: mand

[3655 op=2 display+exit]
  1: "Here's to Mandrake!"
  2: "I'll drink to that!"

[3656 op=1 keyword; failure -> 3661]
  1: manr

[3658 op=22 display+continue]
  1: "My drinking buddy! Let's drink a toast to him, wherever he is."

[3660 op=2 display+exit]
  1: "I'll drink to that!"

[3661 op=1 keyword; failure -> 3663]
  1: manr

[3662 op=2 display+exit]
  1: "My drinking buddy! Let's drink a toast to him, wherever he is."

[3663 op=1 keyword; failure -> 3665]
  1: penu

[3664 op=2 display+exit]
  1: "She's a strange one, but she has the gift."

[3665 op=1 keyword; failure -> 3667]
  1: bye

[3666 op=2 display+exit]
  1: "Leaving already? And you're not even wobbling yet..."

[3667 op=2 display+exit]
  1: "I'll drink to that," he says, suiting action to words.
  2: "$Z? Aye, indeed! A toast to $Z!"

```

<a id="dezana-npc-85"></a>
## Dezana ? NPC 85, port 86

Instruction range: 8030?8140

```text
[8030 op=1 keyword; failure -> 8034]
  1: look

[8033 op=2 display+exit]
  1: a very large woman with a deliberate smile.
  2: "Mmm...yes?"
  3: She looks down at you, her eyes merely slits.

[8034 op=1 keyword; failure -> 8036]
  1: look

[8035 op=2 display+exit]
  1: a very large woman with a deliberate smile.
  2: "Dezana knows much."
  3: "Have you returned for more @talk or are you in @need?"

[8036 op=1 keyword; failure -> 8038]
  1: name

[8037 op=2 display+exit]
  1: "@Dezana."
  2: She smiles like a cat preparing for a meal.

[8038 op=1 keyword; failure -> 8040]
  1: job

[8039 op=2 display+exit]
  1: "I can @heal, @cure, and even @resurrect."

[8040 op=1 keyword; failure -> 8042]
  1: need

[8041 op=2 display+exit]
  1: "Yes?"
  2: "What is thy need--@healing, @curing or @resurrection?"

[8042 op=1 keyword; failure -> 8044]
  1: deza

[8043 op=2 display+exit]
  1: "That is correct."
  2: "Now, are you here to @chat, or are you in @need?"

[8044 op=1 keyword; failure -> 8046]
  1: talk,conv,chat

[8045 op=2 display+exit]
  1: "There are many words which travel about this small hamlet."
  2: "Many are intriguing, some are even dangerous. What words have you heard?"

[8046 op=1 keyword; failure -> 8048]
  1: ghos

[8047 op=2 display+exit]
  1: "That is one worth speaking about. You see, many people travel the @spirit world."
  2: "Some are lost souls, others have a purpose. @Quenton is not a lost @soul."

[8048 op=1 keyword; failure -> 8050]
  1: trav,spir,worl

[8049 op=2 display+exit]
  1: "It is a mystical place between here and the hereafter. Quenton resides there."

[8050 op=1 keyword; failure -> 8052]
  1: lost,soul

[8051 op=2 display+exit]
  1: "They spent their whole life without learning anything or growing inside."
  2: "They are as children left alone in a crib. Never learning, always seeking."

[8052 op=1 keyword; failure -> 8054]
  1: quen

[8053 op=2 display+exit]
  1: "He had attained much wisdom before he was forced from here."
  2: "If his ghost has been seen..."
  3: "And I believe he has..."
  4: "Then he must be attempting to communicate with us about some unfinished business."
  5: "Perhaps he knows who @killed him."

[8054 op=1 keyword; failure -> 8056]
  1: kill,comm

[8055 op=2 display+exit]
  1: "I know this much: if you wish to find out who murdered him, you won't be able to ask his ghost."
  2: "You see, the spirit world can not communicate with ours directly."
  3: "Although his ghost can hear your questions, he'll respond by @manipulating the environment."

[8056 op=1 keyword; failure -> 8058]
  1: mani,envi

[8057 op=2 display+exit]
  1: "Yes, you know, make him demonstrate his answers by, say, shaking a branch."

[8058 op=1 keyword; failure -> 8060]
  1: garg

[8059 op=2 display+exit]
  1: "Those creatures are dangerous. I have heard that they can tear a man to pieces."
  2: "So why, then, did the gargoyles not attack @Stivius as well?"
  3: "Certainly he would have tasted just as good."

[8060 op=1 keyword; failure -> 8062]
  1: tren,bell

[8061 op=2 display+exit]
  1: "He is a very stiff man. I believe he may be hiding something about his past."

[8062 op=1 keyword; failure -> 8064]
  1: yorl

[8063 op=2 display+exit]
  1: "A nice fellow, but a bit too simple for me."

[8064 op=1 keyword; failure -> 8066]
  1: mich

[8065 op=2 display+exit]
  1: "I know something about everyone in this village."
  2: "However, this man is unknown to me."
  3: "He never talks about his past."

[8066 op=1 keyword; failure -> 8068]
  1: gide

[8067 op=2 display+exit]
  1: "He is a dear old man."
  2: "Never has a mean thing to say of anyone."

[8068 op=1 keyword; failure -> 8070]
  1: stiv

[8069 op=2 display+exit]
  1: "A quaint little man."
  2: "He has strong feelings for me."
  3: "I must admit, he has a cute way about him."

[8070 op=1 keyword; failure -> 8072]
  1: marn

[8071 op=2 display+exit]
  1: "She has seen much pain in her life."
  2: "More than even I can heal."
  3: "I pity her."

[8072 op=1 keyword; failure -> 8074]
  1: hora

[8073 op=2 display+exit]
  1: "He was once a great man."
  2: "Discovered many of the herbs I use in healing."
  3: "He discovered something else, however: that total knowledge can corrupt totally."
  4: "I don't know what drove him mad. And I don't want to know." She shudders slightly.

[8074 op=1 keyword; failure -> 8076]
  1: bye

[8075 op=2 display+exit]
  1: "Return when you need healing."

[8082 op=1 keyword; failure -> 8085]
  1: heal

[8084 op=2 display+exit]
  1: "It will cost you 30 gold, interested?"

[8086 op=1 keyword; failure -> 8089]
  1: yes

[8088 op=2 display+exit]
  1: "Which of you?"

[8091 op=2 display+exit]
  1: "You must pay before I can heal you."

[8092 op=1 keyword; failure -> 8095]
  1: cure

[8094 op=2 display+exit]
  1: "It'll cost you 10 gold, interested?"

[8096 op=1 keyword; failure -> 8099]
  1: yes

[8098 op=2 display+exit]
  1: "Which of you?"

[8101 op=2 display+exit]
  1: "You must pay before I can heal you."

[8102 op=1 keyword; failure -> 8105]
  1: resu

[8104 op=2 display+exit]
  1: "Resurrection costs 400 gold, interested?"

[8106 op=1 keyword; failure -> 8108]
  1: yes

[8110 op=2 display+exit]
  1: "As you wish."
  2: "You might think about a proper burial."

[8111 op=2 display+exit]
  1: "It is not my place to talk about that."

[8115 op=2 display+exit]
  1: "You must pay before I can heal you."

[8118 op=2 display+exit]
  1: "You look fine to me."

[8120 op=2 display+exit]
  1: "I see thy injury, $PARTYMEMBER."
  2: She approaches $PARTYMEMBER and binds the wounds.
  3: "Is there something else I can do for you?"

[8124 op=2 display+exit]
  1: "You must pay before I can heal you."

[8127 op=2 display+exit]
  1: "You don't look poisoned."

[8129 op=2 display+exit]
  1: "You look poisoned to me." She nods at $PARTYMEMBER.
  2: She grabs a vial and pours it down $PARTYMEMBER's throat.
  3: $PARTYMEMBER feels much better.
  4: "Is there something else I can do for you?"

[8133 op=2 display+exit]
  1: "You don't have enough gold."
  2: "You might think about a proper burial."

[8136 op=2 display+exit]
  1: "What? But you carry no dead body!"

[8138 op=2 display+exit]
  1: "I see that you carry a departed friend."
  2: Your party takes up a collection for their fallen comrade.
  3: She lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: "You might think about a proper burial."

[8140 op=2 display+exit]
  1: "I see that you carry a departed friend."
  2: Your party takes up a collection for their fallen comrade.
  3: She lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: And the dead live again!

```

<a id="dorin-npc-103"></a>
## Dorin ? NPC 103, port 104

Instruction range: 9712?9794

```text
[9715 op=1 keyword; failure -> 9718]
  1: yes

[9717 op=2 display+exit]
  1: "How many shall I give ye?"

[9720 op=2 display+exit]
  1: "What else can I do for ye?"

[9729 op=2 display+exit]
  1: "That's nice."

[9730 op=1 keyword; failure -> 9733]
  1: look

[9732 op=2 display+exit]
  1: a plump older woman with an apron about her waist.
  2: "Oh, good, you've come back."
  3: "Perhaps for some @pie?" She smiles broadly.

[9733 op=1 keyword; failure -> 9736]
  1: look

[9735 op=2 display+exit]
  1: a plump older woman with an apron about her waist.
  2: "Hello," she says in a scratchy yet firm voice.
  3: "What can I do for ye?"

[9736 op=1 keyword; failure -> 9739]
  1: name

[9738 op=2 display+exit]
  1: "Dorin."

[9739 op=1 keyword; failure -> 9742]
  1: name

[9741 op=2 display+exit]
  1: "Dorin...and yours?"

[9742 op=1 keyword; failure -> 9744]
  1: moth

[9743 op=2 display+exit]
  1: "She was a wonderful cook."
  2: "Taught me all I know."

[9744 op=1 keyword; failure -> 9746]
  1: dori

[9745 op=2 display+exit]
  1: "It was my @mother's name."

[9746 op=1 keyword; failure -> 9748]
  1: job

[9747 op=2 display+exit]
  1: "At the moment, only Meri and I are here, so I guess I must conduct the @business."

[9748 op=1 keyword; failure -> 9750]
  1: shee,busi

[9749 op=2 display+exit]
  1: "We are shepherds."
  2: "We sell @wool."

[9750 op=1 keyword; failure -> 9752]
  1: timo

[9751 op=2 display+exit]
  1: "He and I like to care for the less fortunate people in town."
  2: "I like him alot."

[9752 op=1 keyword; failure -> 9754]
  1: shor,mort

[9753 op=2 display+exit]
  1: "Old Shorty, eh?"
  2: "He's actually quite a nice little man."

[9754 op=1 keyword; failure -> 9756]
  1: mari

[9755 op=2 display+exit]
  1: "A bit caught up in herself, but likable."

[9756 op=1 keyword; failure -> 9758]
  1: arbe

[9757 op=2 display+exit]
  1: "Poor little guy."
  2: "He's afraid of the dark, or so I'm told."

[9758 op=1 keyword; failure -> 9760]
  1: gris

[9759 op=2 display+exit]
  1: "He has the best flour in all the land."

[9760 op=1 keyword; failure -> 9762]
  1: meri

[9761 op=2 display+exit]
  1: "She's my little angel."
  2: "If you see her around here, tell her I have errands to run."

[9762 op=1 keyword; failure -> 9764]
  1: hend

[9763 op=2 display+exit]
  1: She winces at his name.
  2: "He is a hard worker, but sometimes his smell is quite strong."

[9764 op=1 keyword; failure -> 9766]
  1: uber

[9765 op=2 display+exit]
  1: "I like to go on walks with Uby."
  2: "He has such an interesting life."

[9766 op=1 keyword; failure -> 9770]
  1: pie

[9770 op=1 keyword; failure -> 9773]
  1: pie

[9772 op=2 display+exit]
  1: "Yes, I'm baking the nicest shepherd's pie right now."
  2: "It's not done yet, but you can have a slice as soon as it is!"

[9773 op=1 keyword; failure -> 9776]
  1: pie

[9775 op=2 display+exit]
  1: "Oh, dear, I'm afraid you just missed the last piece."
  2: "But I'm baking another, and I'll be sure to save you a slice!"

[9776 op=1 keyword; failure -> 9779]
  1: buy,sell,wool

[9778 op=2 display+exit]
  1: "We charge 5 gold for each bale of wool."
  2: "Would you like some?"

[9779 op=1 keyword; failure -> 9781]
  1: bye

[9780 op=2 display+exit]
  1: She wipes her hand on her apron, and then waves it at you.

[9781 op=2 display+exit]
  1: "Sorry, don't know about any $Z."

[9784 op=2 display+exit]
  1: "What else can I do for ye?"

[9786 op=2 display+exit]
  1: "You look pretty full to me."

[9791 op=2 display+exit]
  1: "I'm sorry, but you don't seem to have enough money."

[9793 op=2 display+exit]
  1: "You look pretty full to me."

[9794 op=2 display+exit]
  1: "Very good."
  2: After accepting your gold, she hands over the wool.
  3: "It was a pleasure doing business with ye."

```

<a id="doris-npc-72"></a>
## Doris ? NPC 72, port 73

Instruction range: 6873?6916

```text
[6873 op=1 keyword; failure -> 6877]
  1: look

[6876 op=2 display+exit]
  1: a stunningly beautiful young woman.
  2: The woman never looks up from the book she's reading, even when you speak to her.
  3: "Yes, what do you want?"

[6877 op=1 keyword; failure -> 6879]
  1: look

[6878 op=2 display+exit]
  1: a stunningly beautiful young woman.
  2: The woman never looks up from the book she's reading, even when you speak to her.
  3: "Yes, what do you want now?"

[6879 op=1 keyword; failure -> 6881]
  1: name

[6880 op=2 display+exit]
  1: "My name's Doris, but you can just call me '@Hey @you.'"
  2: "Everyone else does."

[6881 op=1 keyword; failure -> 6883]
  1: hey,you

[6882 op=2 display+exit]
  1: "Typical," she mutters.

[6883 op=1 keyword; failure -> 6885]
  1: job

[6884 op=2 display+exit]
  1: "I'm in charge of the Tinker's Inn while my @father's away."
  2: "If you have any complaint, no matter how trivial, please feel free to bother me."
  3: It doesn't really sound like she means what she said.

[6885 op=1 keyword; failure -> 6888]
  1: rest,inn,room

[6887 op=2 display+exit]
  1: "Room and board's 5 gold per night per person, cash in advance."
  2: "Interested?"

[6889 op=1 keyword; failure -> 6891]
  1: yes

[6892 op=1 keyword; failure -> 6894]
  1: no

[6893 op=2 display+exit]
  1: "What a pity."

[6895 op=1 keyword; failure -> 6897]
  1: beau

[6896 op=2 display+exit]
  1: "Yes, that's what they all say."

[6897 op=1 keyword; failure -> 6899]
  1: fath

[6898 op=2 display+exit]
  1: "My father called me back from the @Lycaeum so that he could go fight the gargoyles."

[6899 op=1 keyword; failure -> 6901]
  1: book,read,lyca

[6900 op=2 display+exit]
  1: "Yes, I'm learning the mystic arts from the great @Xiao."
  2: "Though I doubt you'd know much about that sort of thing."

[6901 op=1 keyword; failure -> 6903]
  1: xiao

[6902 op=2 display+exit]
  1: "She is the wisest mage in all Britannia."
  2: "Only she can teach the spells of the greatest @power."

[6903 op=1 keyword; failure -> 6905]
  1: wise,mage,brit,powe,spel,grea

[6904 op=2 display+exit]
  1: "Mmm-hmm, that's right."

[6905 op=1 keyword; failure -> 6907]
  1: bye

[6906 op=2 display+exit]
  1: "Mmm-hmmm," she mutters as you leave.

[6909 op=2 display+exit]
  1: "Oh really?"

[6910 op=2 display+exit]
  1: "Hmm, fascinating," she yawns.

[6914 op=2 display+exit]
  1: "No pay, no stay. 5 gold, cash in advance."

[6916 op=2 display+exit]
  1: "Goodnight!"
  2: Putting all thoughts of the rude innkeeper out of your mind, you drift off into a pleasant sleep.

```

<a id="dr-cat-npc-108"></a>
## Dr Cat ? NPC 108, port 109

Instruction range: 9988?10260

```text
[9988 op=1 keyword; failure -> 9991]
  1: look

[9990 op=2 display+exit]
  1: an amused looking gentleman.
  2: "You'll have to excuse me, but the bar's closed right now."
  3: "@Taynith and I have a lot to talk about."
  4: "Come by the Cat's Lair when I'm open for business."

[9992 op=2 display+exit]
  1: "You'll have to excuse me, but the bar's closed right now."
  2: "@Taynith and I have a lot to talk about."
  3: "Come by the Cat's Lair when I'm open for business."

[10000 op=1 keyword; failure -> 10003]
  1: yes

[10002 op=2 display+exit]
  1: "Excellent!"
  2: "How much would you like to bet?"

[10022 op=1 keyword; failure -> 10025]
  1: yes

[10024 op=2 display+exit]
  1: "Excellent!"
  2: "How much would you like to bet?"

[10027 op=2 display+exit]
  1: "@Purrrrhaps some other time, then."

[10030 op=1 keyword; failure -> 10035]
  1: look

[10032 op=22 display+continue]
  1: an amused looking gentleman.
  2: "Ah, I see you've brought me a tasty little mouse to feed my @pets..."
  3: "Don't worry, I was just kidding..."
  4: "Any friend of Lord British is a friend of mine."
  5: "Welcome to the Cat's Lair."
  6: "Anything else I can do for you?"

[10034 op=2 display+exit]
  1: "Eeeeek!"

[10035 op=1 keyword; failure -> 10038]
  1: look

[10037 op=2 display+exit]
  1: an amused looking gentleman.
  2: "Welcome to the Cat's Lair."
  3: "Anything else I can do for you?"

[10038 op=1 keyword; failure -> 10041]
  1: look

[10040 op=2 display+exit]
  1: an amused looking gentleman.
  2: "Welcome to the Cat's Lair."

[10041 op=1 keyword; failure -> 10043]
  1: name

[10042 op=2 display+exit]
  1: "My name's Dr. @Cat."

[10043 op=1 keyword; failure -> 10045]
  1: pet,cat

[10044 op=2 display+exit]
  1: "If you treat my cats well, they'll treat you the same."

[10045 op=1 keyword; failure -> 10047]
  1: buy,sell

[10046 op=2 display+exit]
  1: "Which, @ale, @mead, @wine, @milk, or @mutton?"

[10047 op=1 keyword; failure -> 10049]
  1: job

[10048 op=2 display+exit]
  1: "I sell @ale, @mead, @wine, @milk, and @mutton."
  2: "Of course I like to chat with my @patrons, and indulge in a friendly @game every now and then."

[10049 op=1 keyword; failure -> 10054]
  1: ale

[10053 op=2 display+exit]
  1: "That's 2 crowns."
  2: "Want some?"

[10054 op=1 keyword; failure -> 10059]
  1: mead

[10058 op=2 display+exit]
  1: "That's 2 crowns."
  2: "Want some?"

[10059 op=1 keyword; failure -> 10064]
  1: wine

[10063 op=2 display+exit]
  1: "That's 2 crowns."
  2: "Want some?"

[10064 op=1 keyword; failure -> 10069]
  1: milk

[10068 op=2 display+exit]
  1: "Milk is very good for you, and only 5 crowns for a whole bucket."
  2: "Want some?"

[10069 op=1 keyword; failure -> 10072]
  1: rati,mutt

[10071 op=2 display+exit]
  1: "I sell the finest mutton, and for only 3 gold per serving."
  2: "How many servings do you want?"

[10072 op=1 keyword; failure -> 10074]
  1: indu,game

[10073 op=2 display+exit]
  1: "@Thindle and @Mortude may not agree, but I like a game that provides a little intellectual @challenge."

[10074 op=1 keyword; failure -> 10079]
  1: inte,chal,nim

[10078 op=2 display+exit]
  1: "I think I've got time for a quick game of Nim..."
  2: "Would you care to wager on the outcome, just to keep things interesting?"

[10079 op=1 keyword; failure -> 10081]
  1: thin,mort

[10080 op=2 display+exit]
  1: "I'm sure he'd be glad to @play a few rounds of flippits with you."

[10081 op=1 keyword; failure -> 10084]
  1: play,flip

[10083 op=2 display+exit]
  1: "It's great that you've found me that copy of Snilwit's Big @Book of Boardgame Strategy!"

[10084 op=1 keyword; failure -> 10086]
  1: play,flip

[10085 op=2 display+exit]
  1: "If only I had a copy of Snilwit's Big @Book of Boardgame Strategy..."
  2: "Then I could find some really fine games to teach everyone."

[10086 op=1 keyword; failure -> 10089]
  1: snil,book,boar,stra,teac

[10088 op=2 display+exit]
  1: "It's the definitive reference on boardgames."

[10089 op=1 keyword; failure -> 10092]
  1: snil,book,boar,stra,teac

[10091 op=2 display+exit]
  1: "Have you got a copy of Snilwit's that you'd be willing to sell?"

[10092 op=1 keyword; failure -> 10094]
  1: patr

[10093 op=2 display+exit]
  1: "This is a nice friendly town."
  2: "All the townsfolk know each other."
  3: "We get a lot of @visitors, too."

[10094 op=1 keyword; failure -> 10096]
  1: visi

[10095 op=2 display+exit]
  1: "I chose @Paws for my tavern because it's right on the King's Way, midway between @Britain and @Trinsic."
  2: "There's no better travelled spot in all Britannia, and that makes for good business."

[10096 op=1 keyword; failure -> 10098]
  1: paw

[10097 op=2 display+exit]
  1: "I like the name of the town too, of course!"

[10098 op=1 keyword; failure -> 10100]
  1: brit

[10099 op=2 display+exit]
  1: "A nice place to visit, eh?"

[10100 op=1 keyword; failure -> 10102]
  1: trin

[10101 op=2 display+exit]
  1: "A bit too honorable for my tastes, if you know what I mean..."

[10102 op=1 keyword; failure -> 10104]
  1: tayn

[10103 op=2 display+exit]
  1: "Taynith is my dearest friend."
  2: "Her @gypsy band travels the King's Way."
  3: "Once a week, she stops in to say hello."

[10104 op=1 keyword; failure -> 10106]
  1: gyps,band,pass,week

[10105 op=2 display+exit]
  1: "Don't get the two gypsy bands mixed up..."
  2: "Taynith travels with @Zoltan."
  3: "The other group are a bunch of pickpockets and scallawags..."

[10106 op=1 keyword; failure -> 10108]
  1: zolt

[10107 op=2 display+exit]
  1: "Yes, Zoltan, @king of the gypsies!"

[10108 op=1 keyword; failure -> 10110]
  1: king

[10109 op=2 display+exit]
  1: "I guess you can call yourself king of anything, if nobody comes along to dispute it..."

[10110 op=1 keyword; failure -> 10113]
  1: duck

[10112 op=2 display+exit]
  1: "A friend of Taynith's, are you?"
  2: "Here, have one on the house."
  3: He hands you an ale.
  4: "Taynith bet me one time I couldn't catch a duck with my bare hands."
  5: "So I did, and had a leash made for it, so I could give it to her as a pet."
  6: "I doubt she still has it, though."
  7: "I saw @Zoltan eyeing it hungrily..."

[10113 op=1 keyword; failure -> 10115]
  1: purr

[10114 op=2 display+exit]
  1: "Very good!"
  2: "You purr almost as well as @Kytyn."

[10115 op=1 keyword; failure -> 10117]
  1: kyty

[10116 op=2 display+exit]
  1: "She lives in Britain."

[10117 op=1 keyword; failure -> 10119]
  1: mand

[10118 op=2 display+exit]
  1: "Are you friends of that deadbeat?"
  2: "He owes me for more drinks than I care to think about."
  3: "The only reason I give him food and drink is because he can't sing with his mouth full!"

[10119 op=1 keyword; failure -> 10122]
  1: bye

[10121 op=2 display+exit]
  1: "Good night, sweet princess, and flights of angels tend thee to thy rest."

[10122 op=1 keyword; failure -> 10124]
  1: bye

[10123 op=2 display+exit]
  1: "Good night, sweet prince, and flights of angels tend thee to thy rest."

[10124 op=2 display+exit]
  1: "Meow."

[10126 op=1 keyword; failure -> 10128]
  1: yes

[10129 op=2 display+exit]
  1: "If you aren't thirsty, you could always get some to give to the cats."
  2: "It's the quickest way to get on their good side."
  3: "Anything else I can do for you?"

[10130 op=2 display+exit]
  1: "Anything else I can do for you?"

[10135 op=2 display+exit]
  1: "You're a bit short on gold."
  2: "Anything else I can do for you?"

[10136 op=2 display+exit]
  1: "You don't have room to carry it."
  2: "Anything else I can do for you?"

[10138 op=2 display+exit]
  1: He gives you the milk.
  2: "You have good taste, my friend."
  3: "Anything else I can do for you?"

[10140 op=2 display+exit]
  1: "Here you are."
  2: He gives you the ale.
  3: "Drink up and enjoy, for who knows what tomorrow may bring?"
  4: "Anything else I can do for you?"

[10142 op=2 display+exit]
  1: "Here you are."
  2: He gives you the mead.
  3: "Drink up and enjoy, for who knows what tomorrow may bring?"
  4: "Anything else I can do for you?"

[10143 op=2 display+exit]
  1: "Here you are."
  2: He gives you the wine.
  3: "Drink up and enjoy, for who knows what tomorrow may bring?"
  4: "Anything else I can do for you?"

[10146 op=2 display+exit]
  1: "Anything else I can do for you?"

[10148 op=2 display+exit]
  1: He looks at you.
  2: "You haven't any room in your pack."
  3: "Anything else I can do for you?"

[10153 op=2 display+exit]
  1: "You're a bit short on gold."
  2: "Anything else I can do for you?"

[10155 op=2 display+exit]
  1: He looks at you.
  2: "You haven't any room in your pack."
  3: "Anything else I can do for you?"

[10157 op=2 display+exit]
  1: He hands you the mutton.

[10159 op=2 display+exit]
  1: He hands you $V0 mutton rations.

[10161 op=1 keyword; failure -> 10171]
  1: yes

[10170 op=2 display+exit]
  1: "Great!"
  2: "This is the definitive reference on boardgames."
  3: He takes the book and gives you 110 gold crowns.

[10171 op=1 keyword; failure -> 10175]
  1: yes

[10174 op=2 display+exit]
  1: "Put down some stuff, $P, so you can carry some gold!"

[10175 op=1 keyword; failure -> 10177]
  1: yes

[10176 op=2 display+exit]
  1: "You must be mistaken."

[10177 op=2 display+exit]
  1: "That's too bad."
  2: "I'd pay well for it."

[10184 op=2 display+exit]
  1: "Well, maybe after you've had a few practice games."
  2: "Let me just remind you of the rules, in case you don't remember."
  3: "We start with ten pieces."
  4: He takes some colorful glass beads out of his pocket.
  5: "Each player takes turns, taking one, two or three beads out of the pile."
  6: "Whoever gets the last one wins!"
  7: "I'll let you play first this time."
  8: "There are 10 beads left."
  9: "How many do you take?"

[10187 op=2 display+exit]
  1: "That's a little steep for a friendly game."
  2: "Let's just make it 100 crowns."
  3: "Let me just remind you of the rules, in case you don't remember."
  4: "We start with ten pieces."
  5: He takes some colorful glass beads out of his pocket.
  6: "Each player takes turns, taking one, two or three beads out of the pile."
  7: "Whoever gets the last one wins!"
  8: "I'll let you play first this time."
  9: "There are 10 beads left."
  10: "How many do you take?"

[10189 op=2 display+exit]
  1: "I'll let you play first this time."
  2: "There are 10 beads left."
  3: "How many do you take?"

[10190 op=2 display+exit]
  1: "Let me just remind you of the rules, in case you don't remember."
  2: "We start with ten pieces."
  3: He takes some colorful glass beads out of his pocket.
  4: "Each player takes turns, taking one, two or three beads out of the pile."
  5: "Whoever gets the last one wins!"
  6: "I'll let you play first this time."
  7: "There are 10 beads left."
  8: "How many do you take?"

[10200 op=2 display+exit]
  1: "Well, maybe after you've had a few practice games."
  2: "Let me just remind you of the rules, in case you don't remember."
  3: "We start with ten pieces."
  4: He takes some colorful glass beads out of his pocket.
  5: "Each player takes turns, taking one, two or three beads out of the pile."
  6: "Whoever gets the last one wins!"
  7: "I'll play first this time."
  8: "I'll take $V1 away."
  9: There are $V0 beads left.
  10: How many do you take?

[10203 op=2 display+exit]
  1: "That's a little steep for a friendly game."
  2: "Let's just make it 100 crowns."
  3: "Let me just remind you of the rules, in case you don't remember."
  4: "We start with ten pieces."
  5: He takes some colorful glass beads out of his pocket.
  6: "Each player takes turns, taking one, two or three beads out of the pile."
  7: "Whoever gets the last one wins!"
  8: "I'll play first this time."
  9: "I'll take $V1 away."
  10: There are $V0 beads left.
  11: How many do you take?

[10205 op=2 display+exit]
  1: "I'll play first this time."
  2: "I'll take $V1 away."
  3: There are $V0 beads left.
  4: How many do you take?

[10206 op=2 display+exit]
  1: "Let me just remind you of the rules, in case you don't remember."
  2: "We start with ten pieces."
  3: He takes some colorful glass beads out of his pocket.
  4: "Each player takes turns, taking one, two or three beads out of the pile."
  5: "Whoever gets the last one wins!"
  6: "I'll play first this time."
  7: "I'll take $V1 away."
  8: There are $V0 beads left.
  9: How many do you take?

[10212 op=2 display+exit]
  1: There are $V0 beads left.
  2: How many do you take?

[10214 op=2 display+exit]
  1: There are $V0 beads left.
  2: How many do you take?

[10216 op=2 display+exit]
  1: There are $V0 beads left.
  2: How many do you take?

[10218 op=2 display+exit]
  1: There is 1 bead left.
  2: How many do you take?

[10220 op=2 display+exit]
  1: There is 1 bead left.
  2: How many do you take?

[10222 op=2 display+exit]
  1: There is 1 bead left.
  2: How many do you take?

[10251 op=2 display+exit]
  1: "I'll take $V1 away."
  2: There is 1 bead left.
  3: How many do you take?

[10252 op=2 display+exit]
  1: "I'll take $V1 away."
  2: There are $V0 beads left.
  3: How many do you take?

[10257 op=2 display+exit]
  1: "Well done!"
  2: "You got the last bead."
  3: He takes some money out of a pouch and pays off your wager.
  4: "Would you like to @play again?"

[10260 op=2 display+exit]
  1: "I'll take $V1 away."
  2: "I won!"
  3: "Don't take it too hard..."
  4: "I've had a lot of practice."
  5: "I'll put your money someplace safe, in case you want to come and win it back later."
  6: "Would you like to @play again?"

```

<a id="draxinusom-npc-165"></a>
## Draxinusom ? NPC 165, port 166

Instruction range: 14848?14946

```text
[14850 op=1 keyword; failure -> 14852]
  1: look

[14851 op=2 display+exit]
  1: a large, winged gargoyle of regal bearing.
  2: "To be honored by your presence, noble $P."
  3: "To feel sorrow that matters of state prevent conversation."
  4: "To summon you when proper time for your sacrifice has come."

[14852 op=2 display+exit]
  1: "To be honored by your presence, noble $P."
  2: "To feel sorrow that matters of state prevent conversation."
  3: "To summon you when proper time for your sacrifice has come."

[14854 op=1 keyword; failure -> 14857]
  1: yes

[14859 op=2 display+exit]
  1: "To be necessary for surrender!"
  2: "To ask again for your acceptance of this."
  3: "Your answer?"

[14861 op=1 keyword; failure -> 14864]
  1: yes

[14866 op=2 display+exit]
  1: "To have known it all along!"
  2: "To be a fool for trusting the False Prophet for a moment!"

[14870 op=1 keyword; failure -> 14875]
  1: valk

[14872 op=22 display+continue]
  1: The gargoyle shows you a crystal amulet dangling from a leather cord.
  2: "Hsss....To consent to wear this?"
  3: The gargoyle waits impatiently for an answer.
  4: "Yes or no, False Prophet?"

[14874 op=2 display+exit]
  1: Trusted Dupre whispers frantically to you, "You mustn't!"
  2: "It's some sort of magical trap!"

[14877 op=1 keyword; failure -> 14880]
  1: valk

[14879 op=2 display+exit]
  1: The gargoyle shows you a crystal amulet dangling from a leather cord.
  2: "Hsss....To consent to wear this?"

[14882 op=2 display+exit]
  1: "To doubt your answer..."
  2: "To have known it all along!"
  3: "To be a fool for trusting the False Prophet for a moment!"

[14884 op=1 keyword; failure -> 14887]
  1: sacr

[14886 op=2 display+exit]
  1: "To ask who requested you seek me?"

[14889 op=2 display+exit]
  1: "To be suspicious of thy motives..."
  2: "To have known it all along!"
  3: "To be a fool for trusting the False Prophet for a moment!"

[14891 op=1 keyword; failure -> 14894]
  1: yes

[14893 op=2 display+exit]
  1: "Why?"

[14896 op=2 display+exit]
  1: "Begone, False Prophet!"
  2: "To seize you for sacrifice the instant my army returns from the Underworld!"

[14898 op=1 keyword; failure -> 14901]
  1: yes

[14900 op=2 display+exit]
  1: "To grant me your life?"

[14903 op=2 display+exit]
  1: "Begone, False Prophet!"
  2: "To seize you for sacrifice the instant my army returns from the Underworld!"

[14905 op=1 keyword; failure -> 14908]
  1: yes

[14907 op=2 display+exit]
  1: "To be willing to die to save my people?"

[14910 op=2 display+exit]
  1: "Begone, False Prophet!"
  2: "To seize you for sacrifice the instant my army returns from the Underworld!"

[14913 op=1 keyword; failure -> 14915]
  1: look

[14914 op=2 display+exit]
  1: a large, winged gargoyle of regal bearing.
  2: The gargoyle screams, "An-bal-sil-fer! Klep lem! Por-aylem ter-ort ka!"

[14915 op=2 display+exit]
  1: The gargoyle screams, "An-bal-sil-fer! Klep lem! Por-aylem ter-ort ka!"

[14916 op=1 keyword; failure -> 14920]
  1: look

[14919 op=2 display+exit]
  1: a large, winged gargoyle of regal bearing.
  2: "To express astonishment at your audacity, False Prophet!"

[14920 op=1 keyword; failure -> 14923]
  1: look

[14922 op=2 display+exit]
  1: a large, winged gargoyle of regal bearing.
  2: "To refrain from slaying you only because of the presence of the child, False Prophet!"

[14923 op=1 keyword; failure -> 14925]
  1: look

[14924 op=2 display+exit]
  1: a large, winged gargoyle of regal bearing.
  2: "To demand to know why you have returned here, False Prophet!"

[14925 op=1 keyword; failure -> 14927]
  1: name

[14926 op=2 display+exit]
  1: "To be named Draxinusom, leader of our race."
  2: "To demand to know why you have come before me!"

[14927 op=1 keyword; failure -> 14929]
  1: job

[14928 op=2 display+exit]
  1: "To be the leader of our race."
  2: "To have little time to waste chatting with the False Prophet!"

[14929 op=1 keyword; failure -> 14931]
  1: bye

[14930 op=2 display+exit]
  1: "Flee, False Prophet!"
  2: "To seize you for sacrifice the instant my army returns from the Underworld!"

[14931 op=1 keyword; failure -> 14934]
  1: surr

[14933 op=2 display+exit]
  1: "To be here to surrender?"

[14936 op=2 display+exit]
  1: "To ignore the rhetoric of the False Prophet!"

[14938 op=2 display+exit]
  1: "To grow impatient with your babbling!"

[14940 op=2 display+exit]
  1: "To warn you of imminent attack!"

[14942 op=2 display+exit]
  1: "Begone, False Prophet!"
  2: "To seize you for sacrifice the instant my army returns from the Underworld!"

[14946 op=2 display+exit]
  1: The gargoyle hands you the amulet, and you slip the leather loop over your head.
  2: "To surprise me with the nobility of your deed."
  3: "To no longer be a threat to my people."
  4: "To bid you to travel among my people, talk to them, learn our ways."
  5: "To inform you when the time of your sacrifice has come."

```

<a id="dunbar-npc-94"></a>
## Dunbar ? NPC 94, port 95

Instruction range: 8865?8950

```text
[8866 op=1 keyword; failure -> 8868]
  1: yes

[8871 op=2 display+exit]
  1: "Next week we should have some red snapper."
  2: "Anything else I can do for you?"

[8874 op=2 display+exit]
  1: "Anything else I can do for you?"

[8877 op=1 keyword; failure -> 8880]
  1: look

[8879 op=2 display+exit]
  1: a plump, jovial fellow chewing on a leg of lamb.
  2: "Welcome to the Humble Palate."
  3: "Anything else I can do for you?"

[8880 op=1 keyword; failure -> 8883]
  1: look

[8882 op=2 display+exit]
  1: a plump, jovial fellow chewing on a leg of lamb.
  2: "Welcome to the Humble Palate."

[8883 op=1 keyword; failure -> 8885]
  1: name

[8884 op=2 display+exit]
  1: "My name is Dunbar," he says between bites of mutton.

[8885 op=1 keyword; failure -> 8887]
  1: buy

[8886 op=2 display+exit]
  1: "Which, @fish, @ale, @mead, @wine, or @mutton?"

[8887 op=1 keyword; failure -> 8889]
  1: job

[8888 op=2 display+exit]
  1: "I sell @fish, @ale, @mead, @wine, and @mutton."

[8889 op=1 keyword; failure -> 8894]
  1: fish

[8893 op=2 display+exit]
  1: "I buy all my fish fresh from @Conor."
  2: "It's 3 gold per serving."
  3: "Would you like some?"

[8894 op=1 keyword; failure -> 8899]
  1: mead

[8898 op=2 display+exit]
  1: "For that I charge a modest 3 gold."
  2: "Want some?"

[8899 op=1 keyword; failure -> 8904]
  1: ale

[8903 op=2 display+exit]
  1: "For that I charge a modest 2 gold."
  2: "Want some?"

[8904 op=1 keyword; failure -> 8909]
  1: wine

[8908 op=2 display+exit]
  1: "For that I charge a modest 4 gold."
  2: "Want some?"

[8909 op=1 keyword; failure -> 8912]
  1: rati,mutt

[8911 op=2 display+exit]
  1: "Rations cost 4 gold."
  2: "How many do you want?"

[8912 op=1 keyword; failure -> 8914]
  1: humi,humb

[8913 op=2 display+exit]
  1: "Serving others is the humblest occupation I could imagine."
  2: "That's why I opened this place."
  3: He pauses to wipe his mouth with the back of his hand.

[8914 op=1 keyword; failure -> 8916]
  1: cono

[8915 op=2 display+exit]
  1: "I hear he used to be the head of some important @guild."

[8916 op=1 keyword; failure -> 8918]
  1: impo,head,guil

[8917 op=2 display+exit]
  1: "I don't know anything about it, but maybe it explains that strange @glow you can see over by his house at night."

[8918 op=1 keyword; failure -> 8920]
  1: expl,stra,glow

[8919 op=2 display+exit]
  1: "Go see for yourself."

[8920 op=1 keyword; failure -> 8922]
  1: no,bye

[8921 op=2 display+exit]
  1: "See you later."

[8922 op=2 display+exit]
  1: "I can't help you with that."

[8929 op=2 display+exit]
  1: "I'm afraid you haven't enough money for it."

[8931 op=2 display+exit]
  1: "You don't have room to carry it."

[8933 op=2 display+exit]
  1: He serves you a fish.
  2: "Next week we should have some red snapper."
  3: "Anything else I can do for you?"

[8934 op=2 display+exit]
  1: "Here you go."
  2: "I hope you enjoy it."
  3: "Anything else I can do for you?"

[8937 op=2 display+exit]
  1: "Anything else I can do for you?"

[8939 op=2 display+exit]
  1: He looks at you.
  2: "You haven't any room in your pack."

[8944 op=2 display+exit]
  1: "I'm afraid you haven't enough money for it."

[8946 op=2 display+exit]
  1: He looks at you.
  2: "You haven't any room in your pack."

[8948 op=2 display+exit]
  1: He hands you the mutton.
  2: "Anything else I can do for you?"

[8950 op=2 display+exit]
  1: He hands you $V0 mutton rations.
  2: "Anything else I can do for you?"

```

<a id="dupre-npc-0"></a>
## Dupre ? NPC 0, port 1

Instruction range: 0?42

```text
[0 op=1 keyword; failure -> 2]
  1: look

[1 op=2 display+exit]
  1: a ruggedly handsome man, wearing a gleaming suit of armor.
  2: "Yes, $P?"

[2 op=1 keyword; failure -> 5]
  1: name

[4 op=2 display+exit]
  1: "It's Dupre - sounds like dew pray, remember?"

[5 op=1 keyword; failure -> 7]
  1: job

[6 op=2 display+exit]
  1: "Why, questing, of course! We've been on many a @quest together, you and I."

[7 op=1 keyword; failure -> 9]
  1: ques

[8 op=2 display+exit]
  1: "Yes, you've gone on some really fine ones."
  2: "When you're not around I have to settle for rescuing @damsels, finding @grails, and the like."

[9 op=1 keyword; failure -> 11]
  1: resc,dams

[10 op=2 display+exit]
  1: "It could be worse."
  2: "Some of them are pretty eager to show their @gratitude, if you know what I mean..."

[11 op=1 keyword; failure -> 13]
  1: eage,grat,know,mean

[12 op=2 display+exit]
  1: "Wink wink, nudge nudge, say no more..."

[13 op=1 keyword; failure -> 15]
  1: find,grai

[14 op=2 display+exit]
  1: "That's right."
  2: "Lord British likes to keep grails around to use at his @banquets, but he's always losing them."

[15 op=1 keyword; failure -> 17]
  1: lord,brit,banq

[16 op=2 display+exit]
  1: "He'll throw us a fine feast if you can deal with the @gargoyle invasion somehow."

[17 op=1 keyword; failure -> 19]
  1: garg,inva

[18 op=2 display+exit]
  1: "They may be the toughest threat we've ever faced.

[19 op=1 keyword; failure -> 21]
  1: join

[21 op=1 keyword; failure -> 23]
  1: leav

[23 op=1 keyword; failure -> 25]
  1: duck

[24 op=2 display+exit]
  1: "Please, let's not talk about ducks..."

[25 op=1 keyword; failure -> 27]
  1: bye

[26 op=2 display+exit]
  1: "Yes, enough chit chat. Let's go find some action!"

[27 op=2 display+exit]
  1: "Ask Iolo about that."

[28 op=2 display+exit]
  1: "Lord British gave me strict orders not to leave your side until this quest is complete."
  2: "Besides, you know how I hate to miss a good quest."

[31 op=2 display+exit]
  1: "I've been with you since the start of this @quest, haven't I?"

[33 op=2 display+exit]
  1: "Your party is full!"

[35 op=2 display+exit]
  1: "I'm already on a @quest!"

[36 op=2 display+exit]
  1: "I've been waiting for you to ask, let's begin our @quest."

[39 op=2 display+exit]
  1: "Ask Iolo about that."

[41 op=2 display+exit]
  1: "Don't leave me here!"

[42 op=2 display+exit]
  1: "But you know how I hate to miss a good quest."
  2: "I'll wait until you to ask me to @join again."

```

<a id="eckhart-npc-149"></a>
## Eckhart ? NPC 149, port 150

Instruction range: 13580?13630

```text
[13580 op=1 keyword; failure -> 13583]
  1: look

[13582 op=2 display+exit]
  1: a gnarled man with knobby muscles.
  2: "Hullo, $G."
  3: He touches his cap respectfully.
  4: He has a very sharp knife with which he prunes off less productive branches.
  5: "Mind that you stay out of the flight paths of the @bees, $G."

[13583 op=1 keyword; failure -> 13585]
  1: look

[13584 op=2 display+exit]
  1: a gnarled man with knobby muscles.
  2: "Hullo, $G."
  3: He touches his cap respectfully.

[13585 op=1 keyword; failure -> 13588]
  1: name

[13587 op=2 display+exit]
  1: "Eckhart the vinekeeper, at your service, ma'am."

[13588 op=1 keyword; failure -> 13590]
  1: name

[13589 op=2 display+exit]
  1: "Eckhart the vinekeeper, at your service, sir."

[13590 op=1 keyword; failure -> 13592]
  1: bees,hone,mead

[13591 op=2 display+exit]
  1: You notice the bees have regular aerial routes all about the vineyard.
  2: "Zeke keeps bees on the other side of the abbey, $G."

[13592 op=1 keyword; failure -> 13594]
  1: job

[13593 op=2 display+exit]
  1: "I care for the @garden and tend the @vines."

[13594 op=1 keyword; failure -> 13596]
  1: gard,care

[13595 op=2 display+exit]
  1: "Some of our rosebushes were planted by druids hundreds of years ago."
  2: "Eventually, we became the Brotherhood of the Rose."

[13596 op=1 keyword; failure -> 13599]
  1: vine,tend

[13598 op=2 display+exit]
  1: "These rows," he gestures, "are @seedless grapes."

[13599 op=1 keyword; failure -> 13601]
  1: vine,tend

[13600 op=2 display+exit]
  1: "Here at the abbey we grow @seedless grapes."

[13601 op=1 keyword; failure -> 13603]
  1: seed,less

[13602 op=2 display+exit]
  1: "Amazing, ain't it, $G."
  2: "The @enchanter made them for us."
  3: "Now I grow new vines by grafting seedless @grapevines onto regular rootstocks."

[13603 op=1 keyword; failure -> 13605]
  1: nico,ench

[13604 op=2 display+exit]
  1: "Yes, $G."
  2: "He lives east of here, between two rivers."
  3: "His hut be due north of Britain, or so they tell me."

[13605 op=1 keyword; failure -> 13609]
  1: grap

[13608 op=2 display+exit]
  1: "I'm sorry $G, there aren't anymore ripe ones."

[13609 op=1 keyword; failure -> 13617]
  1: grap

[13614 op=22 display+continue]
  1: "Now those grapes there are for making @wine."
  2: "And these over here are for eating."
  3: He gives you a plump little bunch of sweet table grapes.

[13616 op=2 display+exit]
  1: Dupre says "I cannot say which I like better: wine from grapes or mead from honey!"

[13617 op=1 keyword; failure -> 13622]
  1: grap

[13621 op=2 display+exit]
  1: "Now those grapes there are for making @wine."
  2: "And these over here are for eating."
  3: He gives you a plump little bunch of sweet table grapes.

[13622 op=1 keyword; failure -> 13624]
  1: grap

[13623 op=2 display+exit]
  1: "Visit me in the fields some time and I'll give you some!"

[13624 op=1 keyword; failure -> 13626]
  1: wine

[13625 op=2 display+exit]
  1: "Ask @Faren of that."

[13626 op=1 keyword; failure -> 13628]
  1: fare

[13627 op=2 display+exit]
  1: "Yes, Faren--the local winemaker."

[13628 op=1 keyword; failure -> 13630]
  1: bye

[13629 op=2 display+exit]
  1: He waves and turns back to work.

[13630 op=2 display+exit]
  1: "Beg pardon, $G?"

```

<a id="efram-npc-20"></a>
## Efram ? NPC 20, port 21

Instruction range: 1735?1840

```text
[1735 op=1 keyword; failure -> 1737]
  1: look

[1736 op=2 display+exit]
  1: a grunting and grumbling man with a large stomach.
  2: "Hrmmph. Hello." His breathing is quite heavy.
  3: "Hrmmph, well. Haven't we...never mind. Hrmmph."
  4: "What do you need?"

[1737 op=1 keyword; failure -> 1740]
  1: name

[1739 op=2 display+exit]
  1: "@Efram the provisioner."

[1740 op=1 keyword; failure -> 1742]
  1: efra

[1741 op=2 display+exit]
  1: "It's a nice enough name, hrmmph."

[1742 op=1 keyword; failure -> 1744]
  1: bye

[1743 op=2 display+exit]
  1: "I'm glad you stopped by." He gives you a firm handshake.

[1746 op=1 keyword; failure -> 1748]
  1: buy

[1747 op=2 display+exit]
  1: "Which, @torches, @oil, @gems, @backpacks, @bags, @quest @bags, @shovels, or @powder @kegs?"

[1748 op=1 keyword; failure -> 1750]
  1: job

[1749 op=2 display+exit]
  1: "I sell @torches, @oil, @gems, @backpacks, @bags, @quest @bags, @shovels, and @powder @kegs."

[1750 op=1 keyword; failure -> 1753]
  1: ques

[1752 op=2 display+exit]
  1: "They're strong enough for your most valuable items."
  2: "Hrmmph! But you've already got one!"

[1753 op=1 keyword; failure -> 1756]
  1: ques

[1755 op=2 display+exit]
  1: "They're strong enough for your most valuable items."
  2: "It'll cost you 3 gold for a quest bag."
  3: "Interested?"

[1756 op=1 keyword; failure -> 1759]
  1: torc

[1758 op=2 display+exit]
  1: "It'll cost you 3 gold for each torch. How many do you want?"

[1759 op=1 keyword; failure -> 1762]
  1: oil

[1761 op=2 display+exit]
  1: "It'll cost you 4 gold for each flask of oil. How many do you want?"

[1762 op=1 keyword; failure -> 1765]
  1: gem

[1764 op=2 display+exit]
  1: "It'll cost you 20 gold for each gem. How many do you want?"

[1765 op=1 keyword; failure -> 1768]
  1: back

[1767 op=2 display+exit]
  1: "It'll cost you 10 gold for each backpack. How many do you want?"

[1768 op=1 keyword; failure -> 1771]
  1: bag

[1770 op=2 display+exit]
  1: "It'll cost you 3 gold for each bag. How many do you want?"

[1771 op=1 keyword; failure -> 1774]
  1: shov

[1773 op=2 display+exit]
  1: "It'll cost you 15 gold for each shovel. How many do you want?"

[1774 op=1 keyword; failure -> 1777]
  1: powd

[1776 op=2 display+exit]
  1: "It'll cost you 30 gold for each powder keg. How many do you want?"

[1806 op=1 keyword; failure -> 1808]
  1: yes

[1810 op=2 display+exit]
  1: "Hrmmph. That was a fair price. Well..."

[1812 op=1 keyword; failure -> 1814]
  1: buy

[1813 op=2 display+exit]
  1: "Hrmmph! Well, I'm busy now."
  2: "Hrmmph, yes, well, come back to my shop when it's open."

[1814 op=1 keyword; failure -> 1816]
  1: job

[1815 op=2 display+exit]
  1: "Hrmmph! Well, I'm busy now."
  2: "Hrmmph, yes, well, come back to my shop when it's open."

[1818 op=2 display+exit]
  1: "Hrmmph, well, perhaps you could ask someone else."

[1819 op=2 display+exit]
  1: "I'm sorry?"

[1822 op=2 display+exit]
  1: "Hrmmph. That was a fair price. Well..."

[1824 op=2 display+exit]
  1: "Come now, that's a little much don't you think?"

[1828 op=2 display+exit]
  1: "That costs more gold than you have."

[1830 op=2 display+exit]
  1: "You haven't any room in your pack."

[1831 op=2 display+exit]
  1: "There ya go."

[1836 op=2 display+exit]
  1: "That costs more gold than you have."

[1838 op=2 display+exit]
  1: "You haven't any room in your pack."

[1840 op=2 display+exit]
  1: "There ya go."

```

<a id="elad-npc-110"></a>
## Elad ? NPC 110, port 111

Instruction range: 10299?10377

```text
[10300 op=1 keyword; failure -> 10305]
  1: ahm

[10304 op=2 display+exit]
  1: "Thank you!"
  2: "Now my meditations will succeed at last!"
  3: As you turn to leave,
  4: you think you might have caught a glimpse of Captain Elad stealing a swig from someone else's mug...
  5: But you can't be sure.

[10308 op=2 display+exit]
  1: The pirate frowns.
  2: As you turn to leave,
  3: you think you might have caught a glimpse of Captain Elad stealing a swig from someone else's mug...
  4: But you can't be sure.

[10310 op=1 keyword; failure -> 10313]
  1: yes

[10312 op=2 display+exit]
  1: "Okay, what is it?"

[10315 op=2 display+exit]
  1: "I'll be here with the money if you change your mind."
  2: As you turn to leave,
  3: you think you might have caught a glimpse of Captain Elad stealing a swig from someone else's mug...
  4: But you can't be sure.

[10317 op=1 keyword; failure -> 10320]
  1: yes

[10319 op=2 display+exit]
  1: "I've been trying to find out what it is, so that I can better myself."
  2: "I'll give you five gold if you tell me."
  3: Is it a deal?"

[10322 op=2 display+exit]
  1: "Oh."
  2: "Well, let me know if you find out later."
  3: As you turn to leave,
  4: you think you might have caught a glimpse of Captain Elad stealing a swig from someone else's mug...
  5: But you can't be sure.

[10324 op=1 keyword; failure -> 10326]
  1: look

[10325 op=2 display+exit]
  1: an uncomfortable looking man sitting behind a cup of tea.
  2: "Can't talk now."
  3: "I'm--ungh!"
  4: He reels from a punch to his belly.
  5: "I'm in the middle of something, $G."
  6: "Ouch!"

[10327 op=2 display+exit]
  1: "Can't talk now."
  2: "I'm--ungh!"
  3: He reels from a punch to his belly.
  4: "I'm in the middle of something, $G."
  5: "Ouch!"

[10328 op=1 keyword; failure -> 10330]
  1: look

[10329 op=2 display+exit]
  1: an uncomfortable looking man sitting behind a cup of tea.
  2: "Hello."
  3: His gaze darts from side to side, lingering longingly on the mugs of ale his fellow patrons are gulping down heartily.

[10330 op=1 keyword; failure -> 10333]
  1: name

[10332 op=2 display+exit]
  1: "Well, I'm still Captain Elad."

[10333 op=1 keyword; failure -> 10336]
  1: name

[10335 op=2 display+exit]
  1: He looks at you uneasily.
  2: "I'm @Captain Elad."
  3: "Not that it's any business of yours..."

[10336 op=1 keyword; failure -> 10338]
  1: job

[10337 op=2 display+exit]
  1: "I was Captain of the Theodosia Marie--until she @sank."

[10338 op=1 keyword; failure -> 10340]
  1: capt

[10339 op=2 display+exit]
  1: When you say 'Captain,' all the pirates at the table turn to you expectantly.
  2: When they realize you weren't talking to them, they return to their business.

[10341 op=1 keyword; failure -> 10345]
  1: john

[10342 op=22 display+continue]
  1: "Captain John, that fool..."
  2: "He was captured by gargoyles."
  3: "I hear they dragged him down into Hythloth, and nobody's seen hide nor hair of him since."

[10344 op=2 display+exit]
  1: Leonna raises an eyebrow.
  2: "Is that so?"
  3: "I could tell a different tale..."

[10345 op=1 keyword; failure -> 10347]
  1: john

[10346 op=2 display+exit]
  1: "Captain John, that fool..."
  2: "He was captured by gargoyles."
  3: "I hear they dragged him down into Hythloth, and nobody's seen hide nor hair of him since."

[10347 op=1 keyword; failure -> 10349]
  1: theo,mari,sank

[10348 op=2 display+exit]
  1: "Aye, 'twas a terrible ordeal."
  2: "We were sailing just off @Bordermarch when the whole island started @shaking."
  3: "Must have been one of those @earthquakes we've been having."

[10349 op=1 keyword; failure -> 10351]
  1: terr,orde,sail,isla,shak,size,gone,ship

[10350 op=2 display+exit]
  1: "The whole island sank beneath the waves."
  2: "We were caught in the huge @whirlpool left in its wake."
  3: "I was one of the lucky few to swim clear."
  4: "But my ship's on the bottom of the ocean now, and that's the honest truth."

[10351 op=1 keyword; failure -> 10353]
  1: whir

[10352 op=2 display+exit]
  1: He shudders, then takes a sip of his tea.
  2: "I don't like to think about it."

[10353 op=1 keyword; failure -> 10355]
  1: bord

[10354 op=2 display+exit]
  1: "A fair sized island, and now she's gone!"
  2: "Some o' them @earthquakes have been mighty fierce."

[10355 op=1 keyword; failure -> 10357]
  1: eart,quak

[10356 op=2 display+exit]
  1: "Yes, they started soon after Lord @British was @rescued from the underworld."
  2: "I lost my ship to the last one."

[10357 op=1 keyword; failure -> 10359]
  1: lord,brit

[10358 op=2 display+exit]
  1: "I'd toast him with you, but this," he gestures disparagingly at his cup, "is hardly fitting for such an honor."

[10359 op=1 keyword; failure -> 10361]
  1: resc,unde

[10360 op=2 display+exit]
  1: "Surely you've heard tales of the Avatar's bold exploits?"
  2: "It was all anyone was talking about round here for weeks."

[10361 op=1 keyword; failure -> 10363]
  1: rune

[10362 op=2 display+exit]
  1: "I wish I had one of them..."
  2: "I bet I could sell it for a pretty penny!"

[10363 op=1 keyword; failure -> 10365]
  1: mant

[10364 op=2 display+exit]
  1: "You don't hear much about the @virtues in a pirate town..."

[10365 op=1 keyword; failure -> 10367]
  1: hone,trut,virt,tea

[10366 op=2 display+exit]
  1: "I could use more virtue in me life."
  2: "I've just given up @drinking, to start with."

[10367 op=1 keyword; failure -> 10369]
  1: drin

[10368 op=2 display+exit]
  1: "Aye, 'tis quite a @temptation."

[10369 op=1 keyword; failure -> 10371]
  1: temp

[10370 op=2 display+exit]
  1: "Indeed."
  2: He takes another big gulp of tea.

[10371 op=1 keyword; failure -> 10374]
  1: bye

[10373 op=2 display+exit]
  1: The pirate smiles and waves goodbye, then scowls as he turns back to his mug of tea.

[10374 op=1 keyword; failure -> 10377]
  1: bye

[10376 op=2 display+exit]
  1: "Wait, before you go..."
  2: "I was wondering--do you know the @Mantra of Honesty?"

[10377 op=2 display+exit]
  1: "Ah, that's not important."

```

<a id="empty-npc-186"></a>
## empty! ? NPC 186, port 187

Instruction range: 16261?16330

```text
[16262 op=1 keyword; failure -> 16269]
  1: un

[16266 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "Thy deeds show a lack of @control."

[16268 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16270 op=1 keyword; failure -> 16277]
  1: un

[16274 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "Thy deeds show a need for more @control."

[16276 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16278 op=1 keyword; failure -> 16285]
  1: un

[16282 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "Thy deeds show understanding of @control."

[16284 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16286 op=1 keyword; failure -> 16292]
  1: un

[16289 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "I admire thy deeds and thy @control."

[16291 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16295 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "Thy deeds show a lack of @control."

[16297 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16301 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "Thy deeds show a need for more @control."

[16303 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16307 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "Thy deeds show understanding of @control."

[16309 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16312 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "I admire thy deeds and thy @control."

[16314 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16315 op=1 keyword; failure -> 16319]
  1: look

[16318 op=2 display+exit]
  1: a stone statue of the evil wizard Mondain.
  2: You remember as if it were yesterday how you defeated Mondain in your first visit to Brittania.
  3: Using the four stones, you went back in time to smash his power-gem.
  4: The statue speaks!
  5: "We meet again, Avatar."
  6: "Fear not!"
  7: "I have but one @task now, and am no threat."

[16319 op=1 keyword; failure -> 16321]
  1: look

[16320 op=2 display+exit]
  1: a stone statue of the evil wizard Mondain.
  2: "For what dost thou return, Avatar?"

[16321 op=1 keyword; failure -> 16323]
  1: name

[16322 op=2 display+exit]
  1: "Surely you of all people would recognize me as Mondain."

[16323 op=1 keyword; failure -> 16325]
  1: job,task

[16324 op=2 display+exit]
  1: "The gargoyles have enshrined my spirit here as the embodiment of @control."

[16325 op=1 keyword; failure -> 16328]
  1: cont,mant

[16327 op=2 display+exit]
  1: "I am enshrined here in eternal contemplation of the principle of @control."
  2: "I was once a mighty wizard."
  3: "Control was the essence of my soul."
  4: "Control of self and control of others."
  5: "But desire for more control overcame me."
  6: "I forsook my self-control in my hunger for conquest."
  7: "Control must start with the self and expand outward."
  8: "The strong must lead."
  9: "But to be strong one must control oneself first."
  10: "The gargoyles understand control."
  11: "Their society is based on the strong guiding the weak."
  12: "In this way, they survive in their harsh world."
  13: "Contemplate what thou hast learned."
  14: "Repeat after me the Mantra of Control."
  15: "Un...."

[16328 op=1 keyword; failure -> 16330]
  1: bye

[16329 op=2 display+exit]
  1: "Farewell, Avatar."
  2: "May thy @control help you succeed in thy quest."

[16330 op=2 display+exit]
  1: "I have no concern for matters such as $Z."

```

<a id="enrik-npc-118"></a>
## Enrik ? NPC 118, port 119

Instruction range: 10994?11243

```text
[10994 op=1 keyword; failure -> 10998]
  1: look

[10997 op=2 display+exit]
  1: a scarred man with a gap-toothed grin and large, calloused hands.
  2: "Hello, hello."
  3: "What'll it be?"

[10998 op=1 keyword; failure -> 11000]
  1: look

[10999 op=2 display+exit]
  1: a scarred man with a gap-toothed grin and large, calloused hands.
  2: "Right."
  3: "You been here before."
  4: He studies you carefully.
  5: "What'll it be?"

[11000 op=1 keyword; failure -> 11003]
  1: leat

[11001 op=1 keyword; failure -> 11003]
  1: arm

[11003 op=1 keyword; failure -> 11006]
  1: leat

[11004 op=1 keyword; failure -> 11006]
  1: helm

[11006 op=1 keyword; failure -> 11008]
  1: buy

[11007 op=2 display+exit]
  1: "Hey, what's it look like, I'm a mind reader?"
  2: "Do ya want @arms or @armor?"

[11008 op=1 keyword; failure -> 11010]
  1: armo

[11009 op=2 display+exit]
  1: "Which item, @Cloth armour, @Leather @armour or a Leather @helm?"

[11010 op=1 keyword; failure -> 11012]
  1: arm,arms

[11011 op=2 display+exit]
  1: "Which item, a @Club, a @Dagger, a @Main @Gauche, a @Throwing @Axe or an @Oil @Flask?"

[11012 op=1 keyword; failure -> 11014]
  1: job

[11013 op=2 display+exit]
  1: "And my mutter said I was thick."

[11014 op=1 keyword; failure -> 11017]
  1: name

[11016 op=2 display+exit]
  1: "@Enrik. Enrik the @Hammer's what I'm known as 'round here."

[11017 op=1 keyword; failure -> 11019]
  1: enri

[11018 op=2 display+exit]
  1: "I prefers @Hammer."

[11019 op=1 keyword; failure -> 11021]
  1: hamm

[11020 op=2 display+exit]
  1: "They call me that 'cause I's got a good @left."

[11021 op=1 keyword; failure -> 11023]
  1: good,left

[11022 op=2 display+exit]
  1: "Keep it up and I'll show ya," he growls.

[11023 op=1 keyword; failure -> 11025]
  1: bye

[11024 op=2 display+exit]
  1: "Yeah, gotta get back ta work."

[11025 op=1 keyword; failure -> 11027]
  1: leat

[11026 op=2 display+exit]
  1: "Leather?"
  2: "I makes many leather things."
  3: "Ya want a @helm or some @leather @armour?"

[11027 op=1 keyword; failure -> 11029]
  1: sell

[11029 op=1 keyword; failure -> 11031]
  1: bulk

[11033 op=1 keyword; failure -> 11036]
  1: clot

[11035 op=2 display+exit]
  1: "That Cloth Armour costs 20 gold. How many you need stranga?"

[11036 op=1 keyword; failure -> 11040]
  1: leat

[11037 op=1 keyword; failure -> 11040]
  1: arm

[11039 op=2 display+exit]
  1: "That Leather Armour costs 40 gold. How many you need stranga?"

[11040 op=1 keyword; failure -> 11043]
  1: helm

[11042 op=2 display+exit]
  1: "That Leather Helm costs 10 gold. How many you need stranga?"

[11043 op=1 keyword; failure -> 11046]
  1: club

[11045 op=2 display+exit]
  1: "That Club costs 10 gold. How many you need stranga?"

[11046 op=1 keyword; failure -> 11049]
  1: dagg

[11048 op=2 display+exit]
  1: "That Dagger costs 10 gold. How many you need stranga?"

[11049 op=1 keyword; failure -> 11052]
  1: main,gauc

[11051 op=2 display+exit]
  1: "That Main Gauche costs 25 gold. How many you need stranga?"

[11052 op=1 keyword; failure -> 11055]
  1: oil,flas

[11054 op=2 display+exit]
  1: "That Oil Flask costs 4 gold. How many you need stranga?"

[11055 op=1 keyword; failure -> 11058]
  1: thro,axe

[11057 op=2 display+exit]
  1: "That Throwing Axe costs 20 gold. How many you need stranga?"

[11108 op=2 display+exit]
  1: He gives you a blank stare.

[11109 op=2 display+exit]
  1: He gives you a blank stare.

[11112 op=2 display+exit]
  1: "Mebbe somethin' else?"

[11114 op=2 display+exit]
  1: "But that'd make you o'erburdened"

[11118 op=2 display+exit]
  1: "But you don't got enough gold!"

[11120 op=2 display+exit]
  1: "But that'd make you o'erburdened."

[11121 op=2 display+exit]
  1: "Thank you. Come back, anytime..."

[11128 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 10 gold for that Cloth Armour, $Y?"

[11134 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 20 gold for that Leather Armour, $Y?"

[11140 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 5 gold for that Leather Helm, $Y?"

[11146 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 5 gold for that Club, $Y?"

[11152 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 5 gold for that Dagger, $Y?"

[11158 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 12 gold for that Main Gauche, $Y?"

[11164 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 2 gold for that Oil Flask, $Y?"

[11170 op=2 display+exit]
  1: "Aye, you have somethin' interestin'..."
  2: "Will you take 10 gold for that Throwing Axe, $Y?"

[11173 op=2 display+exit]
  1: "Sorry, none of you's got anything I need."

[11176 op=1 keyword; failure -> 11180]
  1: yes

[11179 op=2 display+exit]
  1: "Done!" He hands you 10 gold pieces and takes the Cloth Armour.

[11181 op=1 keyword; failure -> 11185]
  1: yes

[11184 op=2 display+exit]
  1: "Done!" He hands you 20 gold pieces and takes the Leather Armour.

[11186 op=1 keyword; failure -> 11190]
  1: yes

[11189 op=2 display+exit]
  1: "Done!" He hands you 5 gold pieces and takes the Leather Helm.

[11191 op=1 keyword; failure -> 11195]
  1: yes

[11194 op=2 display+exit]
  1: "Done!" He hands you 5 gold pieces and takes the Club.

[11196 op=1 keyword; failure -> 11200]
  1: yes

[11199 op=2 display+exit]
  1: "Done!" He hands you 5 gold pieces and takes the Dagger.

[11201 op=1 keyword; failure -> 11205]
  1: yes

[11204 op=2 display+exit]
  1: "Done!" He hands you 12 gold pieces and takes the Main Gauche.

[11206 op=1 keyword; failure -> 11210]
  1: yes

[11209 op=2 display+exit]
  1: "Done!" He hands you 2 gold pieces and takes the Oil Flask.

[11211 op=1 keyword; failure -> 11215]
  1: yes

[11214 op=2 display+exit]
  1: "Done!" He hands you 10 gold pieces and takes the Throwing Axe.

[11217 op=2 display+exit]
  1: He gives you a blank stare.

[11218 op=2 display+exit]
  1: He gives you a blank stare.

[11230 op=2 display+exit]
  1: "Sorry, none of you's got anything I need."

[11233 op=2 display+exit]
  1: "Lemme guess, yet another 'Avatar' 'ere to sell me the finest goods in Britannia."
  2: "Like I told the last 'Avatar', you'll get no special deals outta me."
  3: "$V0 gold, take it or leave it?"

[11234 op=2 display+exit]
  1: "$V0 gold, take it or leave it?"

[11236 op=1 keyword; failure -> 11238]
  1: yes

[11238 op=2 display+exit]
  1: He gives you a blank stare.

[11241 op=2 display+exit]
  1: "I saw that!"
  2: "Mebbe I wear a broom on my head, but I ain't @stupid!"

[11243 op=2 display+exit]
  1: "Done!"
  2: He hands you $V0 gold pieces in exchange for the items.

```

<a id="ephemerides-npc-33"></a>
## Ephemerides ? NPC 33, port 34

Instruction range: 2749?2837

```text
[2750 op=1 keyword; failure -> 2753]
  1: yes

[2755 op=2 display+exit]
  1: He frowns at you, "That was a good price."

[2757 op=1 keyword; failure -> 2762]
  1: conc

[2761 op=2 display+exit]
  1: "Ooooh, let me see it..."
  2: "That's a very interesting lens."
  3: "You need a concave copy of it?"
  4: "Let me think..."
  5: "It would be very tricky to duplicate, being magical in nature."
  6: "But I believe I might be able to do it."
  7: "The material and the nature of the enchantment appear similar"
  8: "to those involved in the making of glass swords."
  9: "If I could keep this to analyze while you go get me a glass sword,"
  10: "I'll try and make a copy for you."
  11: Before you can protest, he pockets the lens.
  12: "Let me know when you have the glass @sword."

[2764 op=2 display+exit]
  1: "That I cannot help you with."

[2766 op=1 keyword; failure -> 2768]
  1: look

[2767 op=2 display+exit]
  1: a solemn old man who never turns away from his telescope.
  2: "Shhh! Can't you see I'm busy?!"
  3: "Whoever you are, come back in the morning."
  4: The old man adjusts the telescope, exclaims "Aha!" and starts scribbling on a scroll.

[2769 op=2 display+exit]
  1: "Shhh! Can't you see I'm busy?!"
  2: "Whoever you are, come back in the morning."
  3: The old man adjusts the telescope, exclaims "Aha!" and starts scribbling on a scroll.

[2771 op=1 keyword; failure -> 2773]
  1: look

[2772 op=2 display+exit]
  1: a solemn man, constantly dusting and polishing the items in his shop.
  2: "Ahem."
  3: "Yes, what can I help you with, madam?"

[2773 op=1 keyword; failure -> 2775]
  1: look

[2774 op=2 display+exit]
  1: a solemn man, constantly dusting and polishing the items in his shop.
  2: "Ahem."
  3: "Yes, what can I help you with, sir?"

[2775 op=1 keyword; failure -> 2777]
  1: name

[2776 op=2 display+exit]
  1: "I am Ephemerides."

[2777 op=1 keyword; failure -> 2779]
  1: job

[2778 op=2 display+exit]
  1: "I am an astronomer."
  2: "More precisely, I calculate movements of the stars, phases of the @moons..."
  3: "My field of specialization is @tidal predictions."
  4: "I also make various @glasswares and @instruments."

[2779 op=1 keyword; failure -> 2781]
  1: inst

[2780 op=2 display+exit]
  1: "Yes, @sextants mostly."

[2781 op=1 keyword; failure -> 2783]
  1: glas

[2782 op=2 display+exit]
  1: "Lenses and the like."

[2783 op=1 keyword; failure -> 2785]
  1: tida,pred,fiel

[2784 op=2 display+exit]
  1: "Lord British has requested the publication of an almanac for use by the general populace."
  2: "It will contain high tides, low tides, ebb tides, flood tides, @spring tides, and @neap tides."

[2785 op=1 keyword; failure -> 2787]
  1: tide

[2786 op=2 display+exit]
  1: "Britannia's tides are a function of the two @moons, Felucca and Trammel, interacting with our world, Sosaria."

[2787 op=1 keyword; failure -> 2789]
  1: sosa,felu,tram,moon,sosa

[2788 op=2 display+exit]
  1: "I plan to publish a paper about how the world is flat, to dispel all this foolish talk to the contrary."
  2: "Afterwards, I will construct a model of how it all works: an @orrery."

[2789 op=1 keyword; failure -> 2791]
  1: orre

[2790 op=2 display+exit]
  1: Pleased with your interest, Ephemerides shows you papers with plans for a clockwork model of the Sosarian system.
  2: "This, ahem, flat piece is Britannia. Each moon would exert a pull on the oceans thus, resulting in the tides."
  3: "You turn that little handle there to get it to go!"
  4: The normally staid Ephemerides is practically giggling with glee.

[2791 op=1 keyword; failure -> 2793]
  1: roun

[2792 op=2 display+exit]
  1: "Absurd!"

[2793 op=1 keyword; failure -> 2795]
  1: neap

[2794 op=2 display+exit]
  1: "Low and high tide marks are close together."
  2: "The planets are in @quadrature."

[2795 op=1 keyword; failure -> 2797]
  1: quad

[2796 op=2 display+exit]
  1: "Angular separation of two celestial bodies"
  2: (as measured from a third)
  3: "being ninety degrees, of course."
  4: "A very safe @tide."

[2797 op=1 keyword; failure -> 2799]
  1: spri

[2798 op=2 display+exit]
  1: "The opposite of a neap tide; the sun, earth and moons are all aligned."
  2: "Thus, lines of low and high tide are far apart."
  3: "Once I construct my @orrery, I'd be able to demonstrate the principles involved."

[2799 op=1 keyword; failure -> 2801]
  1: mast

[2800 op=2 display+exit]
  1: "Minoc is the logical place to look."
  2: "That town is filled with master craftsmen."
  3: "Why, I once heard of a man from Minoc who built a @flying machine!"

[2801 op=1 keyword; failure -> 2803]
  1: fly,mach

[2802 op=2 display+exit]
  1: "Must have been a fairy tale, of course."

[2804 op=1 keyword; failure -> 2806]
  1: vort,lens,conc,swor

[2805 op=2 display+exit]
  1: "I hope the lens I made you will prove suitable."

[2806 op=1 keyword; failure -> 2816]
  1: vort,lens,conc,swor

[2815 op=2 display+exit]
  1: "Ah, you've brought me a glass sword!"
  2: "Let me just get your lens..."
  3: He fumbles through his pockets.
  4: "Oh no!"
  5: "I seem to have lost the lens!"
  6: "Just kidding... Here it is."
  7: "Now I'll melt down the glass sword and try to cast a lens for you."
  8: You watch as he works, intently focused on his task.
  9: Finally he pronounces the job a success.
  10: "Here's your lens back, and the duplicate."
  11: "There's no charge - the challenge of the task was reward enough."
  12: "I don't know what you need these lenses for, but I wish you luck!"

[2816 op=1 keyword; failure -> 2819]
  1: vort,lens,conc

[2818 op=2 display+exit]
  1: "When you bring me a glass @sword, I should be able to duplicate your lens for you."

[2819 op=1 keyword; failure -> 2822]
  1: vort,lens,conc

[2821 op=2 display+exit]
  1: "What sort of lens do you want?"

[2822 op=1 keyword; failure -> 2824]
  1: swor

[2823 op=2 display+exit]
  1: "I once heard of a @master glassblower who could make glass swords..."

[2824 op=1 keyword; failure -> 2827]
  1: buy,sext

[2826 op=2 display+exit]
  1: "It'll cost you 30 gold for this sextant."
  2: "Do you want one?"

[2827 op=1 keyword; failure -> 2829]
  1: bye

[2828 op=2 display+exit]
  1: "Goodbye, $G."
  2: "It was nice talking with you."

[2829 op=2 display+exit]
  1: "That I cannot help you with."

[2832 op=2 display+exit]
  1: "It costs more gold than you have."

[2834 op=2 display+exit]
  1: "You haven't any room in your pack."

[2836 op=2 display+exit]
  1: He hands you the sextant.
  2: "Ahem."
  3: "Yes, what else can I help you with, madam?"

[2837 op=2 display+exit]
  1: He hands you the sextant.
  2: "Ahem."
  3: "Yes, what else can I help you with, sir?"

```

<a id="exodus-npc-189"></a>
## Exodus ? NPC 189, port 190

Instruction range: 16401?16470

```text
[16402 op=1 keyword; failure -> 16409]
  1: us

[16406 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need diligence to succeed in thy quest, Avatar."
  3: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  4: "Thy deeds show a lack of @diligence."

[16408 op=2 display+exit]
  1: "Us..."
  2: "Us..."
  3: "Us..."

[16410 op=1 keyword; failure -> 16417]
  1: us

[16414 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need diligence to succeed in thy quest, Avatar."
  3: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  4: "Thy deeds show misguided @diligence."

[16416 op=2 display+exit]
  1: "Us..."
  2: "Us..."
  3: "Us..."

[16418 op=1 keyword; failure -> 16425]
  1: us

[16422 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need diligence to succeed in thy quest, Avatar."
  3: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  4: "Thy deeds show the value of thy @diligence."

[16424 op=2 display+exit]
  1: "Us..."
  2: "Us..."
  3: "Us..."

[16426 op=1 keyword; failure -> 16432]
  1: us

[16429 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need diligence to succeed in thy quest, Avatar."
  3: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  4: "I admire thy deeds and thy @diligence."

[16431 op=2 display+exit]
  1: "Us..."
  2: "Us..."
  3: "Us..."

[16435 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need diligence to succeed in thy quest, Avatar."
  4: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  5: "Thy deeds show a lack of @diligence."

[16437 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16441 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need diligence to succeed in thy quest, Avatar."
  4: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  5: "Thy deeds show misguided @diligence."

[16443 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16447 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need diligence to succeed in thy quest, Avatar."
  4: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  5: "Thy deeds show the value of thy @diligence."

[16449 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16452 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need diligence to succeed in thy quest, Avatar."
  4: "But thou wilt most of all need a clear vision of thy goal to be worthy."
  5: "I admire thy deeds and thy @diligence."

[16454 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16455 op=1 keyword; failure -> 16459]
  1: look

[16458 op=2 display+exit]
  1: a stone statue of the evil Exodus.
  2: You still remember vividly how you defeated Exodus in your third visit to Brittania.
  3: Using the four cards, you tricked the accursed machine into destroying itself.
  4: The statue speaks!
  5: "We meet again, Avatar."
  6: "Fear not!"
  7: "I have but one @task now, and am no threat."

[16459 op=1 keyword; failure -> 16461]
  1: look

[16460 op=2 display+exit]
  1: a stone statue of the evil Exodus.
  2: "For what dost thou return, Avatar?"

[16461 op=1 keyword; failure -> 16463]
  1: name

[16462 op=2 display+exit]
  1: "Though I no longer bear the form of the machine you destroyed, yet still am I Exodus."

[16463 op=1 keyword; failure -> 16465]
  1: job,task

[16464 op=2 display+exit]
  1: "The gargoyles have enshrined my spirit here as the embodiment of @diligence."

[16465 op=1 keyword; failure -> 16468]
  1: dili,mant

[16467 op=2 display+exit]
  1: "I am enshrined here in eternal contemplation of the principle of @diligence."
  2: "I was once a powerful daemon."
  3: "Diligence was the essence of my soul--cold, unswerving diligence in my path of conquest."
  4: "But in my devotion to achievement I lost sight of my original goals."
  5: "Diligence without a goal is as worthless as a goal without diligence."
  6: "The means cannot replace the end, and diligence cannot be the goal."
  7: "The gargoyles understand diligence."
  8: "Their society is based on the diligent leading the wayward."
  9: "In this way, they maintain their fight for survival in their harsh world."
  10: "Contemplate what thou hast learned. Repeat after me the Mantra of Diligence."
  11: "Us...."

[16468 op=1 keyword; failure -> 16470]
  1: bye

[16469 op=2 display+exit]
  1: "Farewell, Avatar."
  2: "May thy @diligence maintain thee on thy quest."

[16470 op=2 display+exit]
  1: "I have no concern for matters such as $Z."

```

<a id="faren-npc-147"></a>
## Faren ? NPC 147, port 148

Instruction range: 13410?13486

```text
[13413 op=1 keyword; failure -> 13418]
  1: whit

[13417 op=2 display+exit]
  1: He smiles at you.
  2: "For white wine, it will cost you 10 gold."
  3: "Interested?"

[13418 op=1 keyword; failure -> 13423]
  1: red

[13422 op=2 display+exit]
  1: He smiles at you.
  2: "For red wine, it will cost you 20 gold."
  3: "Interested?"

[13423 op=1 keyword; failure -> 13428]
  1: dry

[13427 op=2 display+exit]
  1: He smiles at you.
  2: "For dry wine, it will cost you 30 gold."
  3: "Interested?"

[13428 op=1 keyword; failure -> 13433]
  1: swee

[13432 op=2 display+exit]
  1: He smiles at you.
  2: "For sweet wine, it will cost you 40 gold."
  3: "Interested?"

[13433 op=1 keyword; failure -> 13438]
  1: abbeyred

[13437 op=2 display+exit]
  1: He smiles at you.
  2: "For abbey red, it will cost you 100 gold."
  3: "Interested?"

[13438 op=1 keyword; failure -> 13443]
  1: abbeydry

[13442 op=2 display+exit]
  1: He smiles at you.
  2: "For abbey dry, it will cost you 100 gold."
  3: "Interested?"

[13443 op=2 display+exit]
  1: "Too bad, it's really good wine."

[13447 op=1 keyword; failure -> 13450]
  1: yes

[13452 op=2 display+exit]
  1: "Too bad, it's really good wine."

[13454 op=2 display+exit]
  1: He looks at you.
  2: "You don't have enough gold."

[13456 op=2 display+exit]
  1: He glances at you.
  2: "You look pretty full to me."

[13457 op=2 display+exit]
  1: "Enjoy your wine."

[13458 op=1 keyword; failure -> 13461]
  1: buy,wine

[13460 op=2 display+exit]
  1: "Which item, @white @wine, @red @wine, @dry @wine, @sweet @wine, @abbey @red or @abbey @dry?"

[13461 op=1 keyword; failure -> 13464]
  1: look

[13463 op=2 display+exit]
  1: a good looking young man with a wine glass in his hand.
  2: "I have seen your face before," he says, cocking his head sideways.
  3: "What is it you need?"

[13464 op=1 keyword; failure -> 13467]
  1: look

[13466 op=2 display+exit]
  1: a good looking young man with a wine glass in his hand.
  2: "Hello," he says, extending a delicate hand.
  3: "What is it you need?"

[13467 op=1 keyword; failure -> 13470]
  1: name

[13469 op=2 display+exit]
  1: "@Faren."
  2: "@Faren the @drunk as some call me."
  3: He winks at you.

[13470 op=1 keyword; failure -> 13472]
  1: drun

[13471 op=2 display+exit]
  1: "Just kidding."
  2: He smiles broadly.

[13472 op=1 keyword; failure -> 13474]
  1: fare

[13473 op=2 display+exit]
  1: "You might have noticed the similarity between my @sister and me."

[13474 op=1 keyword; failure -> 13476]
  1: sist

[13475 op=2 display+exit]
  1: "A beautiful lass @Stephanie is."

[13476 op=1 keyword; failure -> 13478]
  1: step

[13477 op=2 display+exit]
  1: "Have you met her?"
  2: "Then you know she is @blind."

[13478 op=1 keyword; failure -> 13480]
  1: blin

[13479 op=2 display+exit]
  1: "Yes, has been so since birth."
  2: "For her it is like being born a @gargoyle."

[13480 op=1 keyword; failure -> 13482]
  1: garg

[13481 op=2 display+exit]
  1: "She knows no other life."
  2: "It is her lot to make the best of her situation."
  3: "In this she is no different than you or I."
  4: He pauses to gather his thoughts.
  5: "Even gargoyles face this dilemma, I'll wager."
  6: "But we know them more by the ways they differ from us, rather than their similarities."

[13482 op=1 keyword; failure -> 13484]
  1: job

[13483 op=2 display+exit]
  1: "I sell many types of @wine, made right here."
  2: He clicks his tongue.

[13484 op=1 keyword; failure -> 13486]
  1: bye

[13485 op=2 display+exit]
  1: "Have a nice $T."
  2: He sips his wineglass.

[13486 op=2 display+exit]
  1: He scratches his head and looks around.

```

<a id="farmer-krill-npc-175"></a>
## Farmer Krill ? NPC 175, port 176

Instruction range: 15517?15567

```text
[15524 op=2 display+exit]
  1: "To greet you, $Z."

[15529 op=1 keyword; failure -> 15531]
  1: look

[15530 op=2 display+exit]
  1: a large, healthy-looking winged gargoyle.
  2: The gargoyle screams, "An-bal-sil-fer! Des-lem, klep lem!"

[15531 op=2 display+exit]
  1: The gargoyle screams, "An-bal-sil-fer! Des-lem, klep lem!"

[15532 op=1 keyword; failure -> 15534]
  1: look

[15533 op=2 display+exit]
  1: a large, healthy-looking winged gargoyle.
  2: The gargoyle screams, "The False Prophet! Workers, attack!"

[15534 op=2 display+exit]
  1: The gargoyle screams, "The False Prophet! Workers, attack!"

[15535 op=1 keyword; failure -> 15540]
  1: look

[15539 op=2 display+exit]
  1: a large, healthy-looking winged gargoyle.
  2: "To note that you wear the Amulet of Submission."
  3: "To admire your courage and to appreciate your sacrifice."
  4: "To ask your name, that I may address you in a noble manner?"

[15540 op=1 keyword; failure -> 15542]
  1: look

[15541 op=2 display+exit]
  1: a large, healthy-looking winged gargoyle.
  2: "Good $T, noble $P."
  3: "To be pleased by your return!"

[15542 op=1 keyword; failure -> 15544]
  1: name

[15543 op=2 display+exit]
  1: "To be named Krill Us-arb-vas-lem, Krill the @Leader of Farmers."

[15544 op=1 keyword; failure -> 15546]
  1: job

[15545 op=2 display+exit]
  1: "To @lead the @wingless ones, the worker gargoyles who tend these fields."
  2: "To have great fortune in my @harvests!"

[15546 op=1 keyword; failure -> 15548]
  1: lead,firm,guid,dire

[15547 op=2 display+exit]
  1: "To be forced by necessity to give the @wingless ones no room for choice or @freedom."

[15548 op=1 keyword; failure -> 15551]
  1: beh

[15550 op=2 display+exit]
  1: "Beh Lem?"
  2: "To be but a child."
  3: "To someday grow wings just like his father, Scholar @Valkadesh."

[15551 op=1 keyword; failure -> 15553]
  1: beh

[15552 op=2 display+exit]
  1: "To not understand what you mean."

[15553 op=1 keyword; failure -> 15555]
  1: valk

[15554 op=2 display+exit]
  1: "To admire the wisdom of Scholar Valkadesh, but to consider his @studies unseemly."

[15555 op=1 keyword; failure -> 15557]
  1: stud

[15556 op=2 display+exit]
  1: "To abhor his continued study of the accursed humans!"
  2: The gargoyle pauses, embarrassed.
  3: "To mean no offense to you, $P."
  4: "To acknowledge the nobility of your sacrifice."
  5: "To consider you a great credit to your race!"

[15557 op=1 keyword; failure -> 15559]
  1: room,choi,free

[15558 op=2 display+exit]
  1: "To acknowledge that it may seem harsh, but to assure you that there is no other way to treat the @wingless ones."
  2: "To receive good @harvests only through firm control and clear direction."

[15559 op=1 keyword; failure -> 15561]
  1: wing

[15560 op=2 display+exit]
  1: "To consider them valuable members of society  who need firm control and strong @leadership."
  2: "To explain that they are far less intelligent than you or I."

[15561 op=1 keyword; failure -> 15563]
  1: good,harv,fort,cont,clea,dire

[15562 op=2 display+exit]
  1: "To receive bountiful harvests thanks to the efforts of my well-directed workers."
  2: "To feed my workers far better than misguided Farmer @Nash to the east does."

[15563 op=1 keyword; failure -> 15565]
  1: nash

[15564 op=2 display+exit]
  1: "To be ashamed that Nash lacks common sense."
  2: "To be appalled by his uncontrolled heart and undirected mind."
  3: "To point out his unproductive fields as an example of why the wingless ones need firm @guidance."

[15565 op=1 keyword; failure -> 15567]
  1: bye

[15566 op=2 display+exit]
  1: "To have greatly enjoyed this conversation, noble $P."

[15567 op=2 display+exit]
  1: "To ask what you mean by $Z?"

```

<a id="farmer-nash-npc-174"></a>
## Farmer Nash ? NPC 174, port 175

Instruction range: 15477?15516

```text
[15484 op=2 display+exit]
  1: "To be honored by your visit, $Z."

[15489 op=1 keyword; failure -> 15491]
  1: look

[15490 op=2 display+exit]
  1: a gaunt, tired-looking winged gargoyle.
  2: The gargoyle screams, "An-bal-sil-fer! Des-lem, klep lem!"

[15491 op=2 display+exit]
  1: The gargoyle screams, "An-bal-sil-fer! Des-lem, klep lem!"

[15492 op=1 keyword; failure -> 15494]
  1: look

[15493 op=2 display+exit]
  1: a gaunt, tired-looking winged gargoyle.
  2: The gargoyle screams, "The False Prophet! Workers, attack!"

[15494 op=2 display+exit]
  1: The gargoyle screams, "The False Prophet! Workers, attack!"

[15495 op=1 keyword; failure -> 15500]
  1: look

[15499 op=2 display+exit]
  1: a gaunt, tired-looking winged gargoyle.
  2: "To be amazed!"
  3: "To greatly admire your nobility, False Prophet."
  4: "To honor my people with your sacrifice!"
  5: "To ask your name, that I may address you as you deserve?"

[15500 op=1 keyword; failure -> 15502]
  1: look

[15501 op=2 display+exit]
  1: a gaunt, tired-looking winged gargoyle.
  2: "Good $T, noble $P."
  3: "To be honored by another visit!"

[15502 op=1 keyword; failure -> 15504]
  1: name

[15503 op=2 display+exit]
  1: "To be named Nash Us-arb-vas-lem, Nash the @Leader of Farmers."

[15504 op=1 keyword; failure -> 15506]
  1: job

[15505 op=2 display+exit]
  1: "To @lead the @wingless ones, the worker gargoyles who tend these fields."
  2: "But to have little fortune in my @harvests."

[15506 op=1 keyword; failure -> 15508]
  1: lead

[15507 op=2 display+exit]
  1: "To try to give the @wingless ones free reign,"
  2: "but to be forced to guide them in their work."

[15508 op=1 keyword; failure -> 15510]
  1: wing,free

[15509 op=2 display+exit]
  1: "To be far kinder to the wingless ones than most of my kind."
  2: "To feel that the wingless ones should not be enslaved simply because they lack our intelligence."

[15510 op=1 keyword; failure -> 15512]
  1: harv,fort

[15511 op=2 display+exit]
  1: "To be forced, on occasion, to tend the fields myself because of the harshness of the times."
  2: "To wish my fields were as productive as those of Farmer @Krill, my neighbor to the west."

[15512 op=1 keyword; failure -> 15514]
  1: kril

[15513 op=2 display+exit]
  1: "To be ashamed that Farmer Krill treats his workers so harshly."
  2: "To be appalled by his lack of concern for their thoughts and feelings."

[15514 op=1 keyword; failure -> 15516]
  1: bye

[15515 op=2 display+exit]
  1: "To have been greatly honored by this conversation, noble $P."

[15516 op=2 display+exit]
  1: "To confess difficulty in understanding your accent."
  2: "To inquire what $Z means?"

```

<a id="fentrissa-npc-119"></a>
## Fentrissa ? NPC 119, port 120

Instruction range: 11244?11286

```text
[11245 op=1 keyword; failure -> 11247]
  1: yes

[11249 op=2 display+exit]
  1: "You'll ne'er find a better craft."

[11250 op=1 keyword; failure -> 11254]
  1: look

[11253 op=2 display+exit]
  1: a strong, sinewy woman with a hard look about her.
  2: "Welcome, welcome!" She slaps you heartily on the back.

[11254 op=1 keyword; failure -> 11256]
  1: look

[11255 op=2 display+exit]
  1: a strong, sinewy woman with a hard look about her.
  2: "Smell that salt air, friend. Brings a tear to me eye..."

[11256 op=1 keyword; failure -> 11258]
  1: name

[11257 op=2 display+exit]
  1: "@Fentrissa."

[11258 op=1 keyword; failure -> 11260]
  1: fent

[11259 op=2 display+exit]
  1: "A strong name, as strong as the @sea."
  2: She looks into the distance.

[11260 op=1 keyword; failure -> 11262]
  1: sea

[11261 op=2 display+exit]
  1: "Beautiful thing, but a @jealous man he is."

[11262 op=1 keyword; failure -> 11264]
  1: jeal,man

[11263 op=2 display+exit]
  1: "Many call her a female."
  2: "If ya ask me, men are much more unpredictable."

[11264 op=1 keyword; failure -> 11266]
  1: job,buy

[11265 op=2 display+exit]
  1: "I sell @ships and @skiffs."

[11266 op=1 keyword; failure -> 11269]
  1: ship

[11268 op=2 display+exit]
  1: "It'll cost you 350 gold for the deed."
  2: "Interested?"

[11269 op=1 keyword; failure -> 11272]
  1: skif

[11271 op=2 display+exit]
  1: "It'll cost you 25 gold for the deed."
  2: "Interested?"

[11272 op=1 keyword; failure -> 11274]
  1: bye

[11273 op=2 display+exit]
  1: "Have a good one."
  2: She bows, hands on her hips.

[11274 op=2 display+exit]
  1: "Ask around."

[11283 op=2 display+exit]
  1: "Your party hasn't enough money."
  2: "Sorry."

[11285 op=2 display+exit]
  1: "You look pretty full to me."

[11286 op=2 display+exit]
  1: Your party takes up a collection for the deed.
  2: "Excellent!"
  3: After accepting the gold, she hands you the deed.
  4: "Sail her straight."

```

<a id="finn-npc-161"></a>
## Finn ? NPC 161, port 162

Instruction range: 14561?14625

```text
[14565 op=2 display+exit]
  1: "Broke, are you?"
  2: "Well, come by after I'm done with this disguise, and I'll give you a loan from the royal @treasury."

[14568 op=2 display+exit]
  1: You don't have that much.

[14570 op=2 display+exit]
  1: He takes the money.
  2: "Good."
  3: "I'll get this back to you later."

[14572 op=1 keyword; failure -> 14575]
  1: yes

[14574 op=2 display+exit]
  1: How much do you give?

[14577 op=2 display+exit]
  1: "Broke, are you?"
  2: "Well, come by after I'm done with this disguise, and I'll give you a loan from the royal @treasury."

[14578 op=1 keyword; failure -> 14581]
  1: look

[14580 op=2 display+exit]
  1: a sly looking fellow, dressed in rags.
  2: "Hello my good woman!"

[14581 op=1 keyword; failure -> 14583]
  1: look

[14582 op=2 display+exit]
  1: a sly looking fellow, dressed in rags.
  2: "Hello my good man!"

[14583 op=1 keyword; failure -> 14585]
  1: name,who,susp

[14584 op=2 display+exit]
  1: He glances around quickly, to see if anyone seems to be listening.
  2: "I'm Lord British."
  3: "But don't tell anyone."
  4: "I'm in @disguise so I can mingle with the @commoners."

[14585 op=1 keyword; failure -> 14588]
  1: job

[14587 op=2 display+exit]
  1: "I'm pretending to be a @beggar."
  2: "That way nobody will suspect who I really am."
  3: "Here, give me a few coins to keep anyone from getting suspicious."
  4: Do you give him any money?

[14588 op=1 keyword; failure -> 14590]
  1: ming,comm

[14589 op=2 display+exit]
  1: "Of course, nobody would dare say anything bad about me to my face."
  2: "Nobody except @Chuckles, anyway."
  3: "But as a beggar, people will tell me just about anything!"

[14590 op=1 keyword; failure -> 14592]
  1: chuc

[14591 op=2 display+exit]
  1: "He's in disguise too, you know."
  2: "He's really @Blackthorn."

[14592 op=1 keyword; failure -> 14594]
  1: blac

[14593 op=2 display+exit]
  1: "I decided banishment was too harsh for him, so I brought him back and made him my jester."
  2: "The real Chuckles is tied up in my @dungeons."
  3: "I got tired of him making fun of my nose."

[14594 op=1 keyword; failure -> 14596]
  1: bani,hars,jest,tied,dung

[14595 op=2 display+exit]
  1: "I'm Lord British, and I can do whatever I want to!"

[14596 op=1 keyword; failure -> 14598]
  1: disg

[14597 op=2 display+exit]
  1: "Very convincing, don't you think?"
  2: "I made it myself."

[14598 op=1 keyword; failure -> 14600]
  1: loan,roya,trea

[14599 op=2 display+exit]
  1: "I've got forty million crowns."
  2: "Not to mention my secret bank @accounts."

[14600 op=1 keyword; failure -> 14602]
  1: ment,secr,bank,acco

[14601 op=2 display+exit]
  1: "I told you not to mention that!"

[14602 op=1 keyword; failure -> 14604]
  1: lord,brit,beg

[14603 op=2 display+exit]
  1: "I'm really Lord British."
  2: "I can prove it to you."
  3: "I know all of the @eight @mantras, and where the @runes are, too!"

[14604 op=1 keyword; failure -> 14606]
  1: eigh

[14605 op=2 display+exit]
  1: "I killed a @dragon when I was eight, you know."
  2: "With my bare hands, too!"

[14606 op=1 keyword; failure -> 14608]
  1: kill,drag,bare,hand

[14607 op=2 display+exit]
  1: "He was a big one - but he hadn't reckoned with the likes of me."
  2: "I was a @tough kid."

[14608 op=1 keyword; failure -> 14610]
  1: reck,toug,kid

[14609 op=2 display+exit]
  1: "I was so tough he couldn't chew me, and he had to spit me out."
  2: "That's when I killed him."

[14610 op=1 keyword; failure -> 14612]
  1: mant

[14611 op=2 display+exit]
  1: "Ok, here are the mantras:"
  2: "justice - do"
  3: "compassion - re"
  4: "honor - mi"
  5: "valour - fa"
  6: "spirituality - so"
  7: "humility - la"
  8: "sacrifice - ti"
  9: "Gosh, wouldn't you know it?"
  10: "The Mantra of Honesty slips my mind at the moment..."

[14612 op=1 keyword; failure -> 14614]
  1: rune

[14613 op=2 display+exit]
  1: "I'll tell you this, because you look like someone who can keep a secret."
  2: "I sent out eight fake runes to all the lords, to fool everybody."
  3: "I put all the real ones in a little box, and hid it under my throne."
  4: "You can't see it there, because I turned it @invisible!"

[14614 op=1 keyword; failure -> 14616]
  1: thro,box,invi,fool

[14615 op=2 display+exit]
  1: "A clever trick, or my I'm not @Finn!"

[14616 op=1 keyword; failure -> 14618]
  1: finn

[14617 op=2 display+exit]
  1: "Oh, that's just the name I'm pretending to go by."
  2: "Of course you know I'm really Lord British."
  3: "I can see you're too smart to be fooled by this disguise."

[14618 op=1 keyword; failure -> 14623]
  1: bye

[14620 op=22 display+continue]
  1: "If you need me later, I'll be at the Blue Boar, disguised as the tavernkeeper."
  2: "But I won't admit to it there if you ask me..."
  3: "Too many people that might hear."

[14622 op=2 display+exit]
  1: You see Shamino smiling - an unusual sight.
  2: Noticing your attention, he says "Perhaps there's a little of Lord British in us all."

[14623 op=1 keyword; failure -> 14625]
  1: bye

[14624 op=2 display+exit]
  1: "If you need me later, I'll be at the Blue Boar, disguised as the tavernkeeper."
  2: "But I won't admit to it there if you ask me..."
  3: "Too many people that might hear."

[14625 op=2 display+exit]
  1: "Oh, I know all about that."
  2: "But I think you'd be better off if you learned about it on your own."

```

<a id="foodmaker-npc-173"></a>
## Foodmaker ? NPC 173, port 174

Instruction range: 15393?15476

```text
[15394 op=1 keyword; failure -> 15399]
  1: yes

[15398 op=2 display+exit]
  1: The chef gives you horse chops.
  2: "To welcome you to come back when you are hungry again!"

[15401 op=2 display+exit]
  1: "Well then, to save it here for you!"

[15403 op=1 keyword; failure -> 15407]
  1: yes

[15406 op=2 display+exit]
  1: "To know of no finer cooker of @horseflesh than myself!"
  2: "To give you some more fine horseflesh to take with you on your trip!"

[15409 op=2 display+exit]
  1: "To be saddened by your not liking!"
  2: "To know of no finer cooker of @horseflesh than myself!"

[15411 op=1 keyword; failure -> 15417]
  1: yes

[15414 op=22 display+continue]
  1: Your party eats the profferred food.
  2: The flavor is unlike anything you've had before.
  3: The gargoyle beams with pride.
  4: "To ask you: Is it not the best @meat you have ever eaten?"

[15416 op=2 display+exit]
  1: Iolo speaks.
  2: "I wonder what sort of @meat this is, $P."

[15418 op=1 keyword; failure -> 15421]
  1: yes

[15420 op=2 display+exit]
  1: You eat the profferred food.
  2: The flavor is unlike anything you've had before.
  3: The gargoyle beams with pride.
  4: "To ask you: Is it not the best @meat you have ever eaten?"

[15423 op=2 display+exit]
  1: "To be saddened by your not liking!"
  2: "To know of no finer cooker of @horseflesh than myself!"

[15428 op=1 keyword; failure -> 15430]
  1: look

[15429 op=2 display+exit]
  1: a large and boisterous gargoyle.
  2: He carries a 'knife' large enough to be a sword!
  3: "An-bal-sil-fer!" the gargoyle shouts.
  4: "Gres por! Gres por!"

[15430 op=2 display+exit]
  1: "An-bal-sil-fer!" the gargoyle shouts.
  2: "Gres por! Gres por!"

[15432 op=1 keyword; failure -> 15436]
  1: look

[15433 op=22 display+continue]
  1: a large and boisterous gargoyle.
  2: He carries a 'knife' large enough to be a sword!
  3: "The False Prophet!" the gargoyle shouts.
  4: "Begone, begone!"

[15435 op=2 display+exit]
  1: Half-jokingly, Iolo says, "We'd better do as he says."
  2: "After all, we don't know what kind of meat that is!"

[15437 op=22 display+continue]
  1: "The False Prophet!" the gargoyle shouts.
  2: "Begone, begone!"

[15439 op=2 display+exit]
  1: Half-jokingly, Iolo says, "We'd better do as he says."
  2: "After all, we don't know what kind of meat that is!"

[15440 op=1 keyword; failure -> 15442]
  1: look

[15441 op=2 display+exit]
  1: a large and boisterous gargoyle.
  2: He carries a 'knife' large enough to be a sword!
  3: "The False Prophet!" the gargoyle shouts.
  4: "Begone, begone!"

[15442 op=2 display+exit]
  1: "The False Prophet!" the gargoyle shouts.
  2: "Begone, begone!"

[15443 op=1 keyword; failure -> 15447]
  1: look

[15446 op=2 display+exit]
  1: a large and boisterous gargoyle.
  2: He carries a 'knife' large enough to be a sword!
  3: "The False Prophet!"
  4: "To welcome you to my place of cooking."
  5: "To offer you the best @food in all the world!"

[15447 op=1 keyword; failure -> 15449]
  1: look

[15448 op=2 display+exit]
  1: a large and boisterous gargoyle.
  2: He carries a 'knife' large enough to be a sword!
  3: "To welcome you back, False Prophet!"

[15449 op=1 keyword; failure -> 15451]
  1: hors,meat

[15450 op=2 display+exit]
  1: "To be famous among all gargoyles for the quality of my horseflesh!"

[15451 op=1 keyword; failure -> 15453]
  1: name

[15452 op=2 display+exit]
  1: "To be called Foodmaker, not having a name yet."
  2: "But to be the finest cook in the land!"
  3: "To receive a name someday soon for my skill!"

[15453 op=1 keyword; failure -> 15455]
  1: good

[15454 op=2 display+exit]
  1: "The goodscrafter?"
  2: "To be pathetic!" the huge gargoyle scowls.
  3: "To have self-pity, when strength and long work will give success!"
  4: "To wish to hear no more about that creature."

[15455 op=1 keyword; failure -> 15457]
  1: job

[15456 op=2 display+exit]
  1: "To cook and serve food for all the gargoyles who live in these parts!"
  2: "Also to guide and instruct the @wingless ones in cooking chores."

[15457 op=1 keyword; failure -> 15459]
  1: wing

[15458 op=2 display+exit]
  1: "To have to watch their every move!"
  2: "But to acknowledge that they are vital to our society."

[15459 op=1 keyword; failure -> 15461]
  1: wine,bunc,grap,plan

[15460 op=2 display+exit]
  1: "Well, yes, of course to have that as well."
  2: "But to be famous for my roast horseflesh!"
  3: "To offer you some of this fine @food!"

[15462 op=1 keyword; failure -> 15464]
  1: food

[15463 op=2 display+exit]
  1: "To not have food on me."
  2: "To have you visit my shop."

[15464 op=1 keyword; failure -> 15467]
  1: food

[15466 op=2 display+exit]
  1: "Yes, yes!"
  2: "To offer you this finely roasted @meat!"
  3: With a few strokes of his huge knife, the gargoyle cuts you a slice of @meat.
  4: Do you take the meat?

[15468 op=1 keyword; failure -> 15470]
  1: bye

[15469 op=2 display+exit]
  1: "To welcome you to come back when you are hungry again!"

[15470 op=1 keyword; failure -> 15473]
  1: bye

[15472 op=2 display+exit]
  1: "To welcome you to come back when you are hungry again!"

[15473 op=1 keyword; failure -> 15476]
  1: bye

[15475 op=2 display+exit]
  1: "Wait!"
  2: "To give you some fine horseflesh to take with you on your trip!"
  3: Do you accept his gift?

[15476 op=2 display+exit]
  1: "To not know what $Z is."
  2: "But to gladly cook it for you if you tell me the recipe!"

```

<a id="fyodor-npc-18"></a>
## Fyodor ? NPC 18, port 19

Instruction range: 1573?1702

```text
[1576 op=2 display+exit]
  1: "Pleased to meet thee, `$Z."

[1577 op=1 keyword; failure -> 1580]
  1: look

[1579 op=2 display+exit]
  1: a man in a multicolored jacket. His fingers are long and graceful.
  2: "Hello again, $G."
  3: "What can I do for you?"

[1580 op=1 keyword; failure -> 1583]
  1: look

[1582 op=2 display+exit]
  1: a man in a multicolored jacket. His fingers are long and graceful.
  2: "Good $T, $G."
  3: "What can I do for you?"

[1583 op=1 keyword; failure -> 1587]
  1: name

[1586 op=2 display+exit]
  1: "My name is Fyodor. What is thy name?"

[1587 op=1 keyword; failure -> 1589]
  1: job

[1588 op=2 display+exit]
  1: "I make cloth upon my @loom. Perhaps you'd like to @buy some."

[1589 op=1 keyword; failure -> 1591]
  1: lord,brit,old,book

[1590 op=2 display+exit]
  1: "Lord British treasures antiques. There is one book for which he hath offered a reward:"
  2: "'The Wizard of @Oz.'"

[1591 op=1 keyword; failure -> 1593]
  1: wiza,oz

[1592 op=2 display+exit]
  1: "By Master L. Frank Baum, I believe."
  2: "It concerns a girl adventurer, a strange land, and a clever @balloonist."

[1593 op=1 keyword; failure -> 1595]
  1: ball

[1594 op=2 display+exit]
  1: "Are they not made of @silk, in the form of a great bag?"

[1595 op=1 keyword; failure -> 1597]
  1: silk,bag

[1596 op=2 display+exit]
  1: "'Tis a wondrous strong stuff, silk, and very light."
  2: "I regret that I am not skillful enough to weave silk."
  3: "In New Magincia, however, lives the silk-weaver @Charlotte."

[1597 op=1 keyword; failure -> 1599]
  1: char

[1598 op=2 display+exit]
  1: "She is the only person I know of who can weave silk."

[1605 op=1 keyword; failure -> 1608]
  1: buy

[1607 op=2 display+exit]
  1: "@Cloth or @thread, $Y?"

[1609 op=1 keyword; failure -> 1612]
  1: thre

[1611 op=2 display+exit]
  1: "It'll cost you 5 gold for the thread."
  2: "How many spools art thou interested in?"

[1613 op=1 keyword; failure -> 1616]
  1: clot

[1615 op=2 display+exit]
  1: "It'll cost you 20 gold for the cloth."
  2: "How many pieces art thou interested in?"

[1620 op=1 keyword; failure -> 1622]
  1: sell

[1622 op=1 keyword; failure -> 1624]
  1: buy

[1623 op=2 display+exit]
  1: "Come back when I'm at my loom; I'll help thee then."

[1626 op=1 keyword; failure -> 1628]
  1: clot,thre

[1627 op=2 display+exit]
  1: "Come back when I'm at my loom; I'll help thee then."

[1630 op=2 display+exit]
  1: "Changed your mind, eh?"

[1631 op=1 keyword; failure -> 1633]
  1: sell

[1632 op=2 display+exit]
  1: "But I don't need wool here, $G!"
  2: "Come back when I'm at my loom; I'll deal with thee then."

[1633 op=1 keyword; failure -> 1635]
  1: clot,loom

[1634 op=2 display+exit]
  1: "I push the pedals with my feet--like this."
  2: "Then I pull these levers."
  3: "The pattern depends on which combinations of levers and pedals I move."
  4: "My father's loom was not so complicated as mine. This design came from one of Lord British's old @books."
  5: "I work on a large new loom. The plans for it were found in one of Lord British's old @books."

[1635 op=1 keyword; failure -> 1637]
  1: bye

[1636 op=2 display+exit]
  1: "May our paths cross again some day."

[1637 op=2 display+exit]
  1: "That I cannot help thee with."

[1649 op=2 display+exit]
  1: He turns to you and says,
  2: "Will you take $V1 gold for each bale of wool?"

[1650 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[1652 op=1 keyword; failure -> 1655]
  1: yes

[1654 op=2 display+exit]
  1: "How many can thou spare?"

[1655 op=2 display+exit]
  1: "Changed your mind, eh?"

[1658 op=2 display+exit]
  1: "That is fine thread."
  2: "You'll find none better."

[1660 op=2 display+exit]
  1: "You look pretty full to me."

[1665 op=2 display+exit]
  1: "You haven't the money."

[1667 op=2 display+exit]
  1: "You look pretty full to me."

[1668 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, he hands over the thread.

[1671 op=2 display+exit]
  1: "That is fine cloth."
  2: "You'll find none better."

[1673 op=2 display+exit]
  1: "You look pretty full to me."

[1678 op=2 display+exit]
  1: "You haven't the money."

[1680 op=2 display+exit]
  1: "You look pretty full to me."

[1681 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, he hands over the cloth.

[1684 op=2 display+exit]
  1: "Changed your mind, eh?"

[1686 op=2 display+exit]
  1: "Tis a shame for I need no more than 5 from thee."

[1699 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[1702 op=2 display+exit]
  1: "Done!"
  2: He hands you $V1 gold pieces and takes the wool.

```

<a id="generic-hireling-npc-201"></a>
## Generic hireling ? NPC 201, port 202

Instruction range: 17014?17048

```text
[17014 op=1 keyword; failure -> 17016]
  1: look

[17015 op=2 display+exit]
  1: an experienced adventurer.
  2: "How can I help thee $G?"

[17016 op=1 keyword; failure -> 17018]
  1: name

[17017 op=2 display+exit]
  1: "$NAME the @$CLASS." $SDI

[17018 op=1 keyword; failure -> 17020]
  1: job

[17019 op=2 display+exit]
  1: "I'm a traveling @$CLASS $SDI, since the gargoyal invasion many have needed @help."

[17020 op=1 keyword; failure -> 17022]
  1: help

[17021 op=2 display+exit]
  1: "I'd happily @join thy quest."

[17022 op=1 keyword; failure -> 17024]
  1: join

[17023 op=2 display+exit]
  1: "I ask 100 gold now and $RATE coins per day."
  2: "Just tell me when thou art @ready $G."

[17024 op=1 keyword; failure -> 17026]
  1: ready

[17026 op=1 keyword; failure -> 17028]
  1: leav

[17028 op=1 keyword; failure -> 17030]
  1: bye

[17029 op=2 display+exit]
  1: "Until next time $G!"

[17030 op=2 display+exit]
  1: "Perhaps you should ask someone else about that."

[17033 op=2 display+exit]
  1: "We wouldn't get far with less than 200 gold $G!"

[17036 op=2 display+exit]
  1: "Perhaps you should ask someone else about that."

[17038 op=2 display+exit]
  1: "Your party is full!"

[17040 op=2 display+exit]
  1: "Ask me again when I'm not so busy!"

[17042 op=2 display+exit]
  1: "Great!"
  2: "Let's begin our journey together."

[17045 op=2 display+exit]
  1: "Perhaps you should ask someone else about that."

[17047 op=2 display+exit]
  1: "Don't leave me here!"

[17048 op=2 display+exit]
  1: "It's been good travelling with you $G, farewell!"

```

<a id="geoffrey-npc-5"></a>
## Geoffrey ? NPC 5, port 6

Instruction range: 459?469

```text
[461 op=1 keyword; failure -> 463]
  1: look

[462 op=2 display+exit]
  1: a tall, handsome man.
  2: "Thou hadst best speak to Lord British before aught else."

[463 op=2 display+exit]
  1: "Thou hadst best speak to Lord British before aught else."

[464 op=1 keyword; failure -> 466]
  1: look

[465 op=2 display+exit]
  1: a tall, handsome man.
  2: "I'm glad to see thee, $P. Perhaps thou canst prevail where others could not."
  3: "I sent a party of ten to recapture the Shrine of Compassion from the gargoyles."
  4: "Alas, they failed dismally."
  5: "The survivors are recuperating in the town of Cove."
  6: "Thou wouldst do well to speak with them first."
  7: "Mayhap they learned something which might aid thee."
  8: "I must confess I fear the worst."
  9: "The gargoyles are such powerful foes, and they are spreading so fast..."
  10: "Perhaps the end of the realm is nigh."
  11: "Good luck, and my prayers go with thee."
  12: "Best not waste time talking..."
  13: "Who knows what acts of villainy the gargoyles are comitting even as we speak?"

[466 op=1 keyword; failure -> 469]
  1: name

[468 op=2 display+exit]
  1: "I am Geoffrey, as thou knowest well."

[469 op=2 display+exit]
  1: "Best not waste time talking..."
  2: "Who knows what acts of villainy the gargoyles are comitting even as we speak?"
  3: "I hope you've had a chance to visit Cove and speak with Gertan."

```

<a id="gertan-npc-122"></a>
## Gertan ? NPC 122, port 123

Instruction range: 11307?11347

```text
[11310 op=1 keyword; failure -> 11313]
  1: look

[11312 op=2 display+exit]
  1: a burly man, who looks a little healthier than the other patients.
  2: "Good (cough) day, $G."
  3: "What is thy name?"

[11315 op=2 display+exit]
  1: "Good (cough) day, $G."
  2: "What is thy name?"

[11316 op=1 keyword; failure -> 11318]
  1: look

[11317 op=2 display+exit]
  1: a burly man, who looks a little healthier than the other patients.
  2: "Tis good to see thee again, $P."

[11318 op=1 keyword; failure -> 11320]
  1: name

[11319 op=2 display+exit]
  1: "Gertan, my friend."

[11320 op=1 keyword; failure -> 11322]
  1: job

[11321 op=2 display+exit]
  1: "I lead my men into @battle whenever @Geoffrey sends me."

[11322 op=1 keyword; failure -> 11324]
  1: geof

[11323 op=2 display+exit]
  1: "He is the captain of the guard."

[11324 op=1 keyword; failure -> 11326]
  1: capt,guar

[11325 op=2 display+exit]
  1: "Aye."

[11326 op=1 keyword; failure -> 11328]
  1: fier,batt,shri,comp

[11327 op=2 display+exit]
  1: "We sought to liberate the Shrine of Compassion."
  2: "My men fought valiantly, but the gargoyles were too much for us."
  3: "They didst @slay more than half our party."

[11328 op=1 keyword; failure -> 11330]
  1: garg,slay,foug

[11329 op=2 display+exit]
  1: "Aye, I doubt not they were using foul @magic of some kind."

[11330 op=1 keyword; failure -> 11332]
  1: foul,magi

[11331 op=2 display+exit]
  1: "A moonstone was upon the shrine, surrounded by a strange violet glow."
  2: "I tried to get to it, but 'twas surrounded by some kind of force @field."

[11332 op=1 keyword; failure -> 11334]
  1: moon,ston,forc,fiel,glow,viol

[11333 op=2 display+exit]
  1: "'Tis a matter beyond my comprehension."

[11334 op=1 keyword; failure -> 11336]
  1: cave,frei

[11335 op=2 display+exit]
  1: "Hast thou heard, then, of the time I slew the great @dragon Freitag?"
  2: "Ah, that was a long time ago."

[11336 op=1 keyword; failure -> 11338]
  1: drag,slew

[11337 op=2 display+exit]
  1: "Those were the days..."

[11338 op=1 keyword; failure -> 11340]
  1: bye

[11339 op=2 display+exit]
  1: "May thy efforts meet with more success than mine!"

[11340 op=2 display+exit]
  1: "I cannot help thee with that."

[11347 op=2 display+exit]
  1: "I am called Gertan."
  2: "My fellows and I are recovering from a fierce @battle."

```

<a id="gherick-npc-150"></a>
## Gherick ? NPC 150, port 151

Instruction range: 13631?14003

```text
[13631 op=1 keyword; failure -> 13643]
  1: yes

[13642 op=2 display+exit]
  1: He holds up a beautiful golden shield that glows softly.
  2: "Here ya go. Just as promised."

[13643 op=1 keyword; failure -> 13647]
  1: yes

[13646 op=2 display+exit]
  1: "You ain't got all that I asked for."
  2: "Come back when ya do!"
  3: "What else can I get ya?"

[13649 op=2 display+exit]
  1: "Come back when ya do!"
  2: "What else can I get ya?"

[13650 op=1 keyword; failure -> 13653]
  1: look

[13652 op=2 display+exit]
  1: a tall man, covered with scars and drooling uncontrollably.
  2: "Back again?"

[13653 op=1 keyword; failure -> 13657]
  1: look

[13656 op=2 display+exit]
  1: a tall man, covered with scars and drooling uncontrollably.
  2: "What do you want?"

[13657 op=1 keyword; failure -> 13659]
  1: buy

[13658 op=2 display+exit]
  1: "I's a great craftsman, I is. Me shop is 'uge!"
  2: "You'll hafta be more specific: @arms or @armor?"

[13659 op=1 keyword; failure -> 13662]
  1: name

[13661 op=2 display+exit]
  1: "@Gherick."

[13662 op=1 keyword; failure -> 13664]
  1: gher

[13663 op=2 display+exit]
  1: "Are ya learnin' how ta speak or do ya need somethin'?"

[13664 op=1 keyword; failure -> 13666]
  1: job

[13665 op=2 display+exit]
  1: "I makes the most @hideous weapons!"

[13666 op=1 keyword; failure -> 13668]
  1: aman

[13667 op=2 display+exit]
  1: "She's got the best food in the land. Go see for yerself."

[13668 op=1 keyword; failure -> 13670]
  1: segg

[13669 op=2 display+exit]
  1: "A fine warrior. New to this area."

[13670 op=1 keyword; failure -> 13672]
  1: bye

[13671 op=2 display+exit]
  1: "Shut the door on yer way out," he snarls.

[13672 op=1 keyword; failure -> 13674]
  1: hide

[13673 op=2 display+exit]
  1: "You ain't the sort what'd be interested."
  2: "I got some other stuff you could @buy, though."

[13674 op=1 keyword; failure -> 13678]
  1: orde,serp

[13677 op=2 display+exit]
  1: "Huh? Oh, yeah, da serpent shield."
  2: He looks around and then at the ceiling.
  3: "I needs a snake shield, a chunk o' gold, and a magic gem."
  4: "Have ya got dese tings?"

[13678 op=1 keyword; failure -> 13681]
  1: orde,serp

[13680 op=2 display+exit]
  1: "What, you gone daft?"
  2: "I already made ya one!"

[13681 op=1 keyword; failure -> 13683]
  1: orde,serp

[13682 op=2 display+exit]
  1: "Go talk ta Koranada about that first."

[13683 op=1 keyword; failure -> 13685]
  1: proc

[13684 op=2 display+exit]
  1: "I ain't gonna tell you, you'd steal me brilliant ideas!"

[13685 op=1 keyword; failure -> 13687]
  1: weap,arms

[13686 op=2 display+exit]
  1: "I got a @Halberd, a @Hammer, a @Morning @star, a @Throwing @axe, or a @2Handed @axe"

[13687 op=1 keyword; failure -> 13689]
  1: armo

[13688 op=2 display+exit]
  1: "I got: a @Black @shield, a @Door @Shield, @Scale @mail, a @Spiked @collar, @Spiked @helm, @Spiked @shield"

[13689 op=1 keyword; failure -> 13691]
  1: axe

[13690 op=2 display+exit]
  1: "Which, a @Throwing axe, or a @2Handed axe?"

[13691 op=1 keyword; failure -> 13694]
  1: spik

[13692 op=1 keyword; failure -> 13694]
  1: shie

[13694 op=1 keyword; failure -> 13696]
  1: shie

[13695 op=2 display+exit]
  1: "Which, a @Black shield, a @Door shield or a @Spikedshield?"

[13696 op=1 keyword; failure -> 13698]
  1: spik

[13697 op=2 display+exit]
  1: "Which, a spiked @Collar, a spiked @Helm or a @Spikedshield?"

[13698 op=1 keyword; failure -> 13700]
  1: sell

[13700 op=1 keyword; failure -> 13702]
  1: bulk

[13704 op=1 keyword; failure -> 13707]
  1: halb

[13706 op=2 display+exit]
  1: "Aye, that halberd will cost ya 100 gold. How many ya buyin' stranga?"

[13707 op=1 keyword; failure -> 13710]
  1: morn

[13709 op=2 display+exit]
  1: "Aye, that morning star will cost ya 40 gold. How many ya buyin' stranga?"

[13710 op=1 keyword; failure -> 13713]
  1: thro

[13712 op=2 display+exit]
  1: "Aye, that throwing axe will cost ya 15 gold. How many ya buyin' stranga?"

[13713 op=1 keyword; failure -> 13716]
  1: 2han

[13715 op=2 display+exit]
  1: "Aye, that 2 handed axe will cost ya 50 gold. How many ya buyin' stranga?"

[13716 op=1 keyword; failure -> 13719]
  1: blac

[13718 op=2 display+exit]
  1: "Aye, that black shield will cost ya 30 gold. How many ya buyin' stranga?"

[13719 op=1 keyword; failure -> 13722]
  1: door

[13721 op=2 display+exit]
  1: "Aye, that door shield will cost ya 40 gold. How many ya buyin' stranga?"

[13722 op=1 keyword; failure -> 13725]
  1: scal

[13724 op=2 display+exit]
  1: "Aye, that scalemail armour will cost ya 75 gold. How many ya buyin' stranga?"

[13725 op=1 keyword; failure -> 13728]
  1: coll

[13727 op=2 display+exit]
  1: "Aye, that spiked collar will cost ya 20 gold. How many ya buyin' stranga?"

[13728 op=1 keyword; failure -> 13731]
  1: helm

[13730 op=2 display+exit]
  1: "Aye, that spiked helm will cost ya 25 gold. How many ya buyin' stranga?"

[13731 op=1 keyword; failure -> 13735]
  1: spik

[13732 op=1 keyword; failure -> 13735]
  1: shie

[13734 op=2 display+exit]
  1: "Aye, that spiked shield will cost ya 30 gold. How many ya buyin' stranga?"

[13735 op=1 keyword; failure -> 13738]
  1: hamm

[13737 op=2 display+exit]
  1: "Aye, that hammer will cost ya 20 gold. How many ya buyin' stranga?"

[13741 op=1 keyword; failure -> 13746]
  1: plie

[13745 op=2 display+exit]
  1: He spits on the floor.
  2: "Did that tiny man break 'em again?"
  3: "Anoth'er set will cost ya 25 gold. Whaddya say?"

[13816 op=2 display+exit]
  1: He doesn't hear you.

[13817 op=2 display+exit]
  1: He hears you but doesn't respond.

[13820 op=2 display+exit]
  1: "Well, how 'bout somethin' else?"

[13822 op=2 display+exit]
  1: "What's this? You can't carry that much"

[13826 op=2 display+exit]
  1: "I don't bargain, mate. The price is final, and not a copper less."

[13828 op=2 display+exit]
  1: "What's this? You can't carry that much."

[13829 op=2 display+exit]
  1: "Thank you stranga, Come back, anytime..."

[13830 op=1 keyword; failure -> 13837]
  1: yes

[13836 op=2 display+exit]
  1: After accepting your gold, he hands over the set of pliers.

[13839 op=2 display+exit]
  1: "I don't bargain, mate. The price is final, and not a copper."

[13841 op=2 display+exit]
  1: "What's this? You can't carry that much"

[13844 op=2 display+exit]
  1: "Well, how 'bout somethin' else?"

[13845 op=2 display+exit]
  1: "Changed your mind, eh?"

[13852 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 33 gold for that halberd. Whaddya say?"

[13858 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 13 gold for that morning star. Whaddya say?"

[13864 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 5 gold for that throwing axe. Whaddya say?"

[13870 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 16 gold for that 2 handed axe. Whaddya say?"

[13876 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 10 gold for that black shield. Whaddya say?"

[13882 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 13 gold for that door shield. Whaddya say?"

[13888 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 25 gold for that scalemail armour. Whaddya say?"

[13894 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 6 gold for that spiked collar. Whaddya say?"

[13900 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 8 gold for that spiked helm. Whaddya say?"

[13906 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 10 gold for that spiked shield. Whaddya say?"

[13912 op=2 display+exit]
  1: "Aye, you got somethin' interestin'..."
  2: "I'll give ya 6 gold for that hammer. Whaddya say?"

[13915 op=2 display+exit]
  1: "You ain't got nothing I wants."

[13918 op=1 keyword; failure -> 13922]
  1: yes

[13921 op=2 display+exit]
  1: "Done!" He hands you 33 gold pieces and takes the halberd.

[13923 op=1 keyword; failure -> 13927]
  1: yes

[13926 op=2 display+exit]
  1: "Done!" He hands you 13 gold pieces and takes the morning star.

[13928 op=1 keyword; failure -> 13932]
  1: yes

[13931 op=2 display+exit]
  1: "Done!" He hands you 5 gold pieces and takes the throwing axe.

[13933 op=1 keyword; failure -> 13937]
  1: yes

[13936 op=2 display+exit]
  1: "Done!" He hands you 16 gold pieces and takes the 2 handed axe.

[13938 op=1 keyword; failure -> 13942]
  1: yes

[13941 op=2 display+exit]
  1: "Done!" He hands you 10 gold pieces and takes the black shield.

[13943 op=1 keyword; failure -> 13947]
  1: yes

[13946 op=2 display+exit]
  1: "Done!" He hands you 13 gold pieces and takes the door shield.

[13948 op=1 keyword; failure -> 13952]
  1: yes

[13951 op=2 display+exit]
  1: "Done!" He hands you 25 gold pieces and takes the scalemail armour.

[13953 op=1 keyword; failure -> 13957]
  1: yes

[13956 op=2 display+exit]
  1: "Done!" He hands you 6 gold pieces and takes the spiked collar.

[13958 op=1 keyword; failure -> 13962]
  1: yes

[13961 op=2 display+exit]
  1: "Done!" He hands you 8 gold pieces and takes the spiked helm.

[13963 op=1 keyword; failure -> 13967]
  1: yes

[13966 op=2 display+exit]
  1: "Done!" He hands you 10 gold pieces and takes the spiked shield.

[13968 op=1 keyword; failure -> 13972]
  1: yes

[13971 op=2 display+exit]
  1: "Done!" He hands you 6 gold pieces and takes the hammer.

[13974 op=2 display+exit]
  1: He doesn't hear you.

[13975 op=2 display+exit]
  1: He hears you but doesn't respond.

[13990 op=2 display+exit]
  1: "You ain't got nothing I wants."

[13993 op=2 display+exit]
  1: "What'n the Abyss is all 'a this?"
  2: "Do ya run all over Britannia gatherin' up junk, cram it into a ship,"
  3: "and sail all the way down here 'ta pawn off on me for $V0 gold?"

[13994 op=2 display+exit]
  1: "I'll give ya $V0 gold for that junk. Whaddya say?"

[13996 op=1 keyword; failure -> 13998]
  1: yes

[13998 op=2 display+exit]
  1: "Who else ya think'll buy it?"

[14001 op=2 display+exit]
  1: "You're tryin' to rip me off!"
  2: Gherick spits on you.  Intentionally this time.

[14003 op=2 display+exit]
  1: He hands you $V0 gold pieces and hurls your items onto a pile in the back room.

```

<a id="gideon-npc-81"></a>
## Gideon ? NPC 81, port 82

Instruction range: 7669?7771

```text
[7669 op=1 keyword; failure -> 7674]
  1: look

[7673 op=2 display+exit]
  1: a man bent over with age.
  2: An old brown tunic hangs loosely on him.
  3: "There, I told you, Marta. New people, new food for our souls."
  4: He turns back to you.
  5: "Welcome to the @Haunting @Inn."
  6: "Come and sit, for you must have many @tales."

[7674 op=1 keyword; failure -> 7678]
  1: look

[7677 op=2 display+exit]
  1: a man bent over with age.
  2: An old brown tunic hangs loosely on him.
  3: "Welcome to the @Haunting @Inn."
  4: "Come and sit, for you must have many @tales."

[7678 op=1 keyword; failure -> 7680]
  1: look

[7679 op=2 display+exit]
  1: a man bent over with age.
  2: An old brown tunic hangs loosely on him.
  3: "Is it you, $P?" he says, leaning close and staring you in the eyes.
  4: "Of course it is! Welcome back, traveler."

[7680 op=1 keyword; failure -> 7682]
  1: name

[7681 op=2 display+exit]
  1: "@Gideon, my friend, Gideon's what I'm called by one and all."

[7682 op=1 keyword; failure -> 7684]
  1: tale

[7683 op=2 display+exit]
  1: "Yes, let us swap stories for a while..."

[7684 op=1 keyword; failure -> 7686]
  1: Gide

[7685 op=2 display+exit]
  1: "Tis an old family name meaning 'one who spreads the @word.'"
  2: "My @wife tells me I spread too many of them around."

[7686 op=1 keyword; failure -> 7689]
  1: lodg,room,inn

[7688 op=2 display+exit]
  1: "4 gold for a room and a meal."
  2: "Interested?"

[7690 op=1 keyword; failure -> 7692]
  1: yes

[7693 op=1 keyword; failure -> 7695]
  1: no

[7694 op=2 display+exit]
  1: "Perhaps some other time, then."

[7696 op=1 keyword; failure -> 7698]
  1: word

[7697 op=2 display+exit]
  1: "The word I spread is one of kindness to all souls!"

[7698 op=1 keyword; failure -> 7700]
  1: stra,bedr,drin,well,skif,race,samp,ware,odd,empt

[7699 op=2 display+exit]
  1: "Yes."

[7700 op=1 keyword; failure -> 7702]
  1: job

[7701 op=2 display+exit]
  1: "My @wife would say it is to annoy the customers with idle @banter."
  2: "She's dear."
  3: "I have @rooms for let."

[7702 op=1 keyword; failure -> 7707]
  1: wife

[7704 op=22 display+continue]
  1: "When I was a young man I visited the @shrine to contemplate an emptiness I felt inside."
  2: "Marta was there, so beautiful and shy."
  3: "We sat in silence together for hours."
  4: "Finally, she could take no more and turned and kissed me."
  5: He answers, "No, @Marta, our guests are not asleep yet."

[7706 op=2 display+exit]
  1: A shout comes from the room behind him.

[7707 op=1 keyword; failure -> 7709]
  1: wife

[7708 op=2 display+exit]
  1: "When I was a young man I visited the @shrine to contemplate an emptiness I felt inside."
  2: "Marta was there, so beautiful and shy."
  3: "We sat in silence together for hours."
  4: "Finally, she could take no more and turned and kissed me."

[7709 op=1 keyword; failure -> 7711]
  1: haun

[7710 op=2 display+exit]
  1: "That name used to be just words."
  2: "But now, with what @Marta saw..."

[7711 op=1 keyword; failure -> 7713]
  1: shri

[7712 op=2 display+exit]
  1: "The shrine of Spirituality. It is where we go when we are troubled."
  2: "You should visit it sometime."

[7713 op=1 keyword; failure -> 7715]
  1: shou,mart,what,saw

[7714 op=2 display+exit]
  1: "She hasn't come out of that room since she saw that darn ghost in the dining room a few nights ago."
  2: "She told me the lanterns went out and a spectre drifted in front of her."
  3: "She let out such a @scream that I heard her from our bedroom."
  4: He shakes his head and glances at the room behind him.

[7715 op=1 keyword; failure -> 7717]
  1: ghos,dini,lant,spec,scre

[7716 op=2 display+exit]
  1: "My Marta doesn't spook that easy. After all, she's woke up next to me all these years."
  2: "At first I thought maybe I should lock the @cupboard where we keep the wine."
  3: "But I don't think my Marta's been nippin'. I wish I knew what she saw."

[7717 op=1 keyword; failure -> 7719]
  1: cup,wine

[7718 op=2 display+exit]
  1: "If you've never had @Stivius' wine, you're in for a treat."

[7719 op=1 keyword; failure -> 7721]
  1: hora

[7720 op=2 display+exit]
  1: "He's the alchemist who lives on @Iver's Rounding."
  2: "You'll need a boat to get there though."

[7721 op=1 keyword; failure -> 7723]
  1: iver,roun

[7722 op=2 display+exit]
  1: "It was named after a famous pirate who beached his craft there during a storm."

[7723 op=1 keyword; failure -> 7725]
  1: stiv,wine

[7724 op=2 display+exit]
  1: "He lives next door. Strange fellow..."
  2: "I think he's sampled too much of his own wares."

[7725 op=1 keyword; failure -> 7727]
  1: idle,bant

[7726 op=2 display+exit]
  1: "I'm sure by now you've heard such banter about old Quenton."
  2: "He was a dear soul." Gideon looks past you into the distance.
  3: "I could not believe it was him by the well."
  4: His eyes become watery.
  5: "He lay so quiet, no sign of pain on his face, and just an @amulet in his bloody hand."
  6: He looks at the floor and mumbles.
  7: "@Quen, I miss you, old mate."

[7727 op=1 keyword; failure -> 7729]
  1: quen,old,mate

[7728 op=2 display+exit]
  1: "We came to this island many years ago."
  2: "As children we'd take the boats and race around the point."
  3: "Marta and I took care of him when @Mondain's henchmen killed his wife @Elisabeth."
  4: "We care for his daughter, Marney, now. She's such a brave girl."
  5: He pauses a moment then continues.
  6: "Neither I nor the @travelers staying here heard anything the night he was @killed."

[7729 op=1 keyword; failure -> 7731]
  1: kill

[7730 op=2 display+exit]
  1: He looks at you a moment, then leans closer.
  2: "They say that Quenton was killed by a Gargoyle."
  3: "I've heard these beasts eat their prey."
  4: "Why did they leave Quenton alone then?"

[7731 op=1 keyword; failure -> 7733]
  1: mond,henc

[7732 op=2 display+exit]
  1: "Yes, Quenton and Yorl were out fishing when a few of those evil ones landed here."
  2: "They were fleeing Lord British's men and apparently thought our hamlet was safe haven."
  3: "They went around asking for some person named Relthor, or @Renthar... something like that."

[7733 op=1 keyword; failure -> 7735]
  1: elis

[7734 op=2 display+exit]
  1: "She was an angel to view. Blonde hair, green eyes, tall slender body. Marney could be her twin."
  2: "I can still see Elisa's brave face as the men led her to their boat."
  3: "They said that after she got them past their pursuers she would be freed."
  4: He hides his face in his hands.
  5: "Our beliefs held us still."
  6: "She turned and handed @something to her crying little @girl."
  7: "Then she @boarded the skiff, and they were gone."

[7735 op=1 keyword; failure -> 7737]
  1: marn,cryi,litt,brav,girl

[7736 op=2 display+exit]
  1: "She, like her mother, is strong in her @beliefs."
  2: "When we buried her father, she sang a song she wrote for him."
  3: He softly sings.
  4: "'You put your love in my heart, and now it's fallen apart, but all alone in the dark, I will remove the pain.'"
  5: "'There is a sting in my eyes, and your name in my cries, but still I realize, that you are part of me.'"

[7737 op=1 keyword; failure -> 7739]
  1: boar,skif

[7738 op=2 display+exit]
  1: "Elisabeth bowed her head as they sailed off."
  2: He looks absently at the floor and attempts to clear his throat.
  3: Speaking again, his voice is a whisper.
  4: "They found her body along the @coast south of here."

[7739 op=1 keyword; failure -> 7741]
  1: coas,sout

[7740 op=2 display+exit]
  1: He looks at you, his eyes red and his lip quivering.
  2: He simply nods.

[7741 op=1 keyword; failure -> 7743]
  1: hand,some

[7742 op=2 display+exit]
  1: "I couldn't see what it was."

[7743 op=1 keyword; failure -> 7745]
  1: relt,rent,pers

[7744 op=2 display+exit]
  1: "We had no such person in this town. Not by that name."

[7745 op=1 keyword; failure -> 7747]
  1: beli,stil

[7746 op=2 display+exit]
  1: "We are a peaceful people."
  2: "We believe that all men have hearts that can possess either good or evil."
  3: "These evil ones believed that by tearing down others, they could become more powerful."
  4: "The truth is, when you tear down another's ladder you become no higher, they just become lower."
  5: "It is harder to build your ladder on your own, but you will see farther in the end."

[7747 op=1 keyword; failure -> 7749]
  1: trav,murd

[7748 op=2 display+exit]
  1: "I questioned them about the night of the murder."
  2: "Still, of all of us, my Marta and I had the best vantage point from which to witness, or even hear, the murder."
  3: "Yet we saw and heard nothing."

[7749 op=1 keyword; failure -> 7751]
  1: amul

[7750 op=2 display+exit]
  1: "We went and told Yorl what had happened."
  2: "When he returned with us to the well, he removed the amulet from Quenton's hand and took it to Marney."
  3: "It took courage to tell her the news."

[7751 op=1 keyword; failure -> 7753]
  1: tren,bell

[7752 op=2 display+exit]
  1: "He means well, it's just his attitude is all wrong for the job."
  2: "He should spend some time in New @Magincia."

[7753 op=1 keyword; failure -> 7755]
  1: new,magi

[7754 op=2 display+exit]
  1: "You should sail there someday."

[7755 op=1 keyword; failure -> 7757]
  1: yorl

[7756 op=2 display+exit]
  1: His eyes light up.
  2: "He is one of the friendliest people in this hamlet."
  3: "You must go see him, down by the pier." He stares absently at your shirt pocket.
  4: "He's been like a father to Marney."

[7757 op=1 keyword; failure -> 7759]
  1: deza

[7758 op=2 display+exit]
  1: "She's a dear soul. I sometimes feel sorry for her. I mean, her weight and all."
  2: "Still, she is one of the most interesting people I've ever met."

[7759 op=1 keyword; failure -> 7761]
  1: mich

[7760 op=2 display+exit]
  1: "Now there's a sad soul. He was so young when he got tangled up with the wrong group of kids."
  2: "Had he been raised by someone with a good heart, he might have been one of Lord @British's greatest knights."

[7761 op=1 keyword; failure -> 7763]
  1: lord,brit

[7762 op=2 display+exit]
  1: "I'm just an innkeep. Ask @Yorl, his father used to sail for our ruler."

[7763 op=1 keyword; failure -> 7765]
  1: bye

[7764 op=2 display+exit]
  1: "Come back, dear souls."

[7765 op=2 display+exit]
  1: "Ask someone else about that."

[7769 op=2 display+exit]
  1: "I'm sorry, but we require cash in advance for our rooms."

[7771 op=2 display+exit]
  1: "Goodnight!"

```

<a id="gilron-npc-120"></a>
## Gilron ? NPC 120, port 121

Instruction range: 11287?11298

```text
[11287 op=1 keyword; failure -> 11289]
  1: look

[11288 op=2 display+exit]
  1: a feverish looking warrior.
  2: "....."

[11291 op=2 display+exit]
  1: "Ohhhhh..."

[11293 op=2 display+exit]
  1: "Mrtlx hmlsh fbbn..."

[11295 op=2 display+exit]
  1: "Mmmmmm..."

[11297 op=2 display+exit]
  1: ".........."

[11298 op=2 display+exit]
  1: "Rrrrlr Grrtl..."

```

<a id="glen-npc-144"></a>
## Glen ? NPC 144, port 145

Instruction range: 13159?13284

```text
[13161 op=2 display+exit]
  1: You pass along Mole's message.
  2: "Needed a shovel, did he?"
  3: "Tell him to use the one I gave him when I hired him!"
  4: "No doubt he's forgotten he has it by now, or lost it somewhere..."

[13164 op=2 display+exit]
  1: You pass along Mole's message.
  2: "Needed a shovel, did he?"
  3: "Tell him to use the one I gave him when I hired him!"
  4: "No doubt he's forgotten he has it by now, or lost it somewhere..."

[13166 op=2 display+exit]
  1: You pass along Mole's message.
  2: "Needed a shovel, did he?"
  3: "Tell him to use the one I gave him when I hired him!"
  4: "No doubt he's forgotten he has it by now, or lost it somewhere..."

[13169 op=2 display+exit]
  1: "So, his shovel broke?"
  2: "That's no excuse."
  3: "Tell him he should have bought another one and kept working."

[13171 op=2 display+exit]
  1: "So, his shovel broke?"
  2: "That's no excuse."
  3: "Tell him he should have bought another one and kept working."

[13174 op=2 display+exit]
  1: "That addle-brained, lower class, miserable excuse for a common laborer..."
  2: "I agreed to give him an equipment allowance with his pay."
  3: "No doubt it's all been squandered at some tavern, along with the rest of his pay."
  4: "See what he has to say about that!"

[13176 op=2 display+exit]
  1: "That addle-brained, lower class, miserable excuse for a common laborer..."
  2: "I agreed to give him an equipment allowance with his pay."
  3: "No doubt it's all been squandered at some tavern, along with the rest of his pay."
  4: "See what he has to say about that!"

[13192 op=2 display+exit]
  1: "Ohhh!"
  2: "Silly me, spending so much time with my clients."
  3: He gestures at the corpses.
  4: "I forget little details like paying Mole..."
  5: "I'll have to take care of it right away."
  6: "You deserve a reward for helping me straighten this out."
  7: "The body of a mage was found just outside of town."
  8: "He was carrying a couple of enchanted items and some money when the gargoyles got him."
  9: "Nobody has been able to find any relatives of his, so you might as well have these."
  10: He hands you its belongings.

[13194 op=1 keyword; failure -> 13197]
  1: yes

[13199 op=2 display+exit]
  1: "Too bad."
  2: "Goodbye for now - I'm sure I'll see you again."
  3: "Every body ends up here sooner or later..."

[13201 op=1 keyword; failure -> 13204]
  1: yes

[13203 op=2 display+exit]
  1: "Help yourself."
  2: "It's very relaxing work."
  3: "At least it would be, if I didn't have to put up with that incompetent @gravedigger."

[13206 op=2 display+exit]
  1: "It's very relaxing work."
  2: "At least it would be, if I didn't have to put up with that incompetent @gravedigger."

[13229 op=2 display+exit]
  1: His eyes gleam fiendishly when he notices the body you're carrying.
  2: "Ah, you've brought me a client.
  3: Tell the truth, doesn't your friend look more peaceful this way?"
  4: "For such a heroic client, I have something special to offer."
  5: "It's a new process I've invented called 'cremation.'"
  6: "For a mere fifteen gold, the remains of the dearly departed are purified with flame."
  7: "Would you like me to do this for you?"

[13230 op=1 keyword; failure -> 13232]
  1: look

[13231 op=2 display+exit]
  1: a quiet, pale, almost motionless gentleman.
  2: "Good evening." he says, in a voice that sends shivers down your spine.
  3: "It is evening, I hope."
  4: "That's my favorite time."

[13232 op=1 keyword; failure -> 13234]
  1: name

[13233 op=2 display+exit]
  1: "I am Glen D'Arc, of the @Trinsic D'Arcs."

[13234 op=1 keyword; failure -> 13236]
  1: trin,darc,d'ar

[13235 op=2 display+exit]
  1: "I had to move away... Too @sunny."

[13236 op=1 keyword; failure -> 13238]
  1: sun

[13237 op=2 display+exit]
  1: "I much prefer moonlight."
  2: "It's more natural and healthy."

[13238 op=1 keyword; failure -> 13240]
  1: prep,bod,depa

[13239 op=2 display+exit]
  1: "For most clients, I prefer to use Frigid @Solvol."
  2: "It's the finest enbalming fluid on the market."

[13240 op=1 keyword; failure -> 13242]
  1: frig,solv,enba

[13241 op=2 display+exit]
  1: "It gives the departed a nice, healthy glow..."

[13242 op=1 keyword; failure -> 13244]
  1: eter,rest

[13243 op=2 display+exit]
  1: "Yes, those who cannot afford resurrection know the cool comfort of good, honest soil."
  2: He sighs longingly.
  3: "How I envy them, for only the @dead know true peace."

[13244 op=1 keyword; failure -> 13246]
  1: dead,ress

[13245 op=2 display+exit]
  1: He smiles.
  2: "I see you're not much of a believer in death."
  3: "But next time you're dead you should try staying that way."
  4: "You might like it..."

[13246 op=1 keyword; failure -> 13249]
  1: job

[13248 op=2 display+exit]
  1: "I prepare the @bodies of the departed for their eternal @rest."
  2: He pauses to pop a grape into his mouth.
  3: He chews it delicately, but you can still hear a soft, squishy sound.
  4: "Would you like a few grapes?"

[13249 op=1 keyword; failure -> 13253]
  1: inco,grav,mole

[13252 op=2 display+exit]
  1: "You can't get good help these days..."
  2: "Mole was supposed to have two more graves finished yesterday."
  3: "I can't stand to talk to him any more - if you see him, tell him I need those graves dug immediately!"

[13253 op=1 keyword; failure -> 13255]
  1: inco,grav,mole

[13254 op=2 display+exit]
  1: "You can't get good help these days..."

[13255 op=1 keyword; failure -> 13257]
  1: bye

[13256 op=2 display+exit]
  1: "Goodbye for now - I'm sure I'll see you again."
  2: "Every body ends up here sooner or later..."

[13257 op=2 display+exit]
  1: "Don't concern yourself with that."
  2: "Life is too hectic as it is."

[13278 op=2 display+exit]
  1: "Too bad."

[13281 op=2 display+exit]
  1: "You're a bit short on gold." he says, disappointedly.

[13284 op=2 display+exit]
  1: "Excellent!"
  2: He takes the body, and recites a brief incantation.
  3: An eerie green flame engulfs the corpse, consuming it in seconds.
  4: Nothing but ashes remain.
  5: The mortician sweeps them up, puts them in an urn, and hands it to you.
  6: "Please come again if any more of your friends need my services."

```

<a id="goodscrafter-npc-170"></a>
## Goodscrafter ? NPC 170, port 171

Instruction range: 15195?15229

```text
[15199 op=1 keyword; failure -> 15201]
  1: look

[15200 op=2 display+exit]
  1: a quiet, listless gargoyle.
  2: "An-bal-sil-fer," the gargoyle says calmly.
  3: "Sum in-korp i. Vers."
  4: The gargoyle silently awaits his fate.

[15201 op=2 display+exit]
  1: "An-bal-sil-fer," the gargoyle says calmly.
  2: "Sum in-korp i. Vers."
  3: The gargoyle silently awaits his fate.

[15202 op=1 keyword; failure -> 15204]
  1: look

[15203 op=2 display+exit]
  1: a quiet, listless gargoyle.
  2: "Oh, the False Prophet," the gargoyle says calmly.
  3: "To suppose that you have come to kill me as well."
  4: "All right."
  5: The gargoyle silently awaits his fate.

[15204 op=2 display+exit]
  1: "Oh, the False Prophet," the gargoyle says calmly.
  2: "To suppose that you have come to kill me as well."
  3: "All right."
  4: The gargoyle silently awaits his fate.

[15205 op=1 keyword; failure -> 15208]
  1: look

[15207 op=2 display+exit]
  1: a quiet, listless gargoyle.
  2: "To ask why you disturb me again, False Prophet."

[15208 op=1 keyword; failure -> 15211]
  1: look

[15210 op=2 display+exit]
  1: a quiet, listless gargoyle.
  2: "To see the Amulet of Submission."
  3: "To suppose, then, that my world has been saved."
  4: "But to feel that salvation has come too @late."

[15211 op=1 keyword; failure -> 15213]
  1: too,late

[15212 op=2 display+exit]
  1: "To refuse to discuss my @grief with the being who caused it."

[15213 op=1 keyword; failure -> 15215]
  1: grie

[15214 op=2 display+exit]
  1: "Surely to know what pain you have caused by stealing the Codex?"
  2: "To not believe that you care."
  3: "To believe, in fact, that you enjoy causing @suffering."

[15215 op=1 keyword; failure -> 15217]
  1: suff,no,help

[15216 op=2 display+exit]
  1: "To not believe that you would do anything to help."

[15217 op=1 keyword; failure -> 15219]
  1: name

[15218 op=2 display+exit]
  1: "To not have a name."
  2: "To have nothing at all now..."

[15219 op=1 keyword; failure -> 15221]
  1: job

[15220 op=2 display+exit]
  1: "To be a goodscrafter."
  2: "Though to have no reason to make @goods now."

[15221 op=1 keyword; failure -> 15223]
  1: good,buy

[15222 op=2 display+exit]
  1: "To invite you to take what you want."

[15223 op=1 keyword; failure -> 15225]
  1: fami

[15224 op=2 display+exit]
  1: "To have lost my entire family in the @cataclysms."

[15225 op=1 keyword; failure -> 15227]
  1: cata

[15226 op=2 display+exit]
  1: "To wish not to speak of it."

[15227 op=1 keyword; failure -> 15229]
  1: bye

[15228 op=2 display+exit]
  1: "To wait here for you to return to kill me."

[15229 op=2 display+exit]
  1: "To not care about $Z."
  2: "To not care about anything now."

```

<a id="gorn-npc-129"></a>
## Gorn ? NPC 129, port 130

Instruction range: 11837?11883

```text
[11837 op=1 keyword; failure -> 11840]
  1: look

[11839 op=2 display+exit]
  1: a burly barbarian warrior.
  2: "Vy talk ven dere's fighting to do?"

[11840 op=1 keyword; failure -> 11843]
  1: look

[11842 op=2 display+exit]
  1: a burly barbarian warrior.
  2: "Ho, Avatar, thou hast returned!"

[11843 op=1 keyword; failure -> 11846]
  1: look

[11845 op=2 display+exit]
  1: a burly barbarian warrior.
  2: "Hail, $P!"
  3: "I haf heard of thy deeds, Avatar!"

[11846 op=1 keyword; failure -> 11848]
  1: name

[11847 op=2 display+exit]
  1: "I be named Gorn, $G."

[11848 op=1 keyword; failure -> 11851]
  1: job

[11850 op=2 display+exit]
  1: "I follow thee, $G."
  2: "I haf no other job now."

[11851 op=1 keyword; failure -> 11853]
  1: job

[11852 op=2 display+exit]
  1: "I be a fighter, a homeless vanderer."
  2: "Here I vait until I find somevun else to @trafel mit."

[11853 op=1 keyword; failure -> 11855]
  1: traf,trav

[11854 op=2 display+exit]
  1: "Ya, I like to trafel!"
  2: "Much vould I like to vander again, but I haf no vun to trafel mit now."
  3: "Somevun vill ask me to @join mit them zooner or later."

[11855 op=1 keyword; failure -> 11857]
  1: join

[11857 op=1 keyword; failure -> 11860]
  1: leav

[11860 op=1 keyword; failure -> 11862]
  1: bye

[11861 op=2 display+exit]
  1: "It is alvays a pleasure to talk mit thee, $G."

[11862 op=1 keyword; failure -> 11864]
  1: sute

[11863 op=2 display+exit]
  1: "Sutek is mad."
  2: "He is very dangerous."
  3: "I vould avoid him if I vere thee."
  4: "But he does know many tings, und is sometimes helpful."

[11866 op=2 display+exit]
  1: "I am just a simple varrior, $G, and know nothing about $Z."

[11868 op=2 display+exit]
  1: "Surely zere are others who vould know more about $Z than I, $G."

[11869 op=2 display+exit]
  1: "I don't understand, $G."
  2: "Vat do you mean, $Z?"

[11872 op=2 display+exit]
  1: "I vould be much happy to join mit thee, $G, und I already haf!"

[11874 op=2 display+exit]
  1: "I vould be much happy to join mit thee, $G, but I dislike such large groups."

[11876 op=2 display+exit]
  1: "I vould be much happy to join mit thee, $G, but he vas here first, ya?"

[11877 op=2 display+exit]
  1: "I vould be much happy to join mit thee, $G!"

[11880 op=2 display+exit]
  1: "I don't understand, $G."
  2: "Vat do you mean, $Z?"

[11882 op=2 display+exit]
  1: "Ask ven ve ave returned to zi place I joined mit thee."

[11883 op=2 display+exit]
  1: "Ok, if that's vat you vant."
  2: "Maybe I'll zee you later."
  3: "You keep all zis stuff meanwhile, ya?"

```

<a id="grison-npc-102"></a>
## Grison ? NPC 102, port 103

Instruction range: 9606?9711

```text
[9608 op=1 keyword; failure -> 9611]
  1: yes

[9610 op=2 display+exit]
  1: "How many sacks of flour would ya like?"

[9614 op=1 keyword; failure -> 9617]
  1: yes

[9616 op=2 display+exit]
  1: "How many would ya like to sell?"

[9621 op=2 display+exit]
  1: "Suit yourself."

[9623 op=1 keyword; failure -> 9625]
  1: look

[9624 op=2 display+exit]
  1: a figure covered from head to toe with flour.
  2: "Ach, I'd like to talk with ya, but I've no time!"
  3: "Come see me at my gristmill some other time."
  4: "'Tis hard work, but it gives me plenty of time to talk!"

[9626 op=2 display+exit]
  1: "Ach, I'd like to talk with ya, but I've no time!"
  2: "Come see me at my gristmill some other time."
  3: "'Tis hard work, but it gives me plenty of time to talk!"

[9627 op=1 keyword; failure -> 9630]
  1: look

[9629 op=2 display+exit]
  1: a figure covered from head to toe with flour.
  2: "Hrmph... ptuh... hello again."

[9630 op=1 keyword; failure -> 9633]
  1: look

[9632 op=2 display+exit]
  1: a figure covered from head to toe with flour.
  2: "Ptuh... ptuh... hello."

[9633 op=1 keyword; failure -> 9636]
  1: name

[9635 op=2 display+exit]
  1: "@Grison @Fairfleth, at your service."

[9636 op=1 keyword; failure -> 9638]
  1: gris,fair

[9637 op=2 display+exit]
  1: "You can call me 'Gris' if ya prefer."

[9638 op=1 keyword; failure -> 9640]
  1: job

[9639 op=2 display+exit]
  1: "As you can see, I turn @grain into @flour."
  2: "It's a pretty demanding job...and a bit messy too."

[9640 op=1 keyword; failure -> 9643]
  1: buy,flou

[9642 op=2 display+exit]
  1: "I'll sell you sacks of flour for 4 gold."
  2: "Is that okay?"

[9643 op=1 keyword; failure -> 9653]
  1: sell,grai

[9653 op=1 keyword; failure -> 9656]
  1: sell,grai

[9655 op=2 display+exit]
  1: "I'll pay you $V1 gold for each sack of grain, ok?"

[9656 op=1 keyword; failure -> 9658]
  1: timo

[9657 op=2 display+exit]
  1: "He's a good fellow."
  2: "I grew up with him."

[9658 op=1 keyword; failure -> 9660]
  1: grew,up

[9659 op=2 display+exit]
  1: "Yep. We were mates."
  2: He continues to wipe the dust from his eyes.

[9660 op=1 keyword; failure -> 9662]
  1: mort

[9661 op=2 display+exit]
  1: "Shorty, ya mean?"
  2: "He's got a fair temper, I'll tell you that!"

[9662 op=1 keyword; failure -> 9664]
  1: mari

[9663 op=2 display+exit]
  1: "She's one with a nose to the air instead of the grindstone." He chuckles to himself.

[9664 op=1 keyword; failure -> 9666]
  1: arbe

[9665 op=2 display+exit]
  1: "Scared of his own shadow, he is."

[9666 op=1 keyword; failure -> 9668]
  1: dori

[9667 op=2 display+exit]
  1: "She's baked many a pie with my flour."

[9668 op=1 keyword; failure -> 9670]
  1: meri

[9669 op=2 display+exit]
  1: "Meri the wisp, eh?"
  2: "She's sure a cutey."

[9670 op=1 keyword; failure -> 9672]
  1: hend

[9671 op=2 display+exit]
  1: "One o' the nicest gents I know."
  2: "Always has a good tale."

[9672 op=1 keyword; failure -> 9674]
  1: uber

[9673 op=2 display+exit]
  1: "A good, hard worker, that man."
  2: "Has quite a strong grip too."

[9674 op=1 keyword; failure -> 9676]
  1: bye

[9675 op=2 display+exit]
  1: "Well, if that's all you need, goodbye."

[9676 op=2 display+exit]
  1: He sticks a finger in his ear and shakes it.
  2: "What?"

[9679 op=2 display+exit]
  1: "Suit yourself."

[9681 op=2 display+exit]
  1: "You look pretty full to me."

[9686 op=2 display+exit]
  1: "You can't afford it."

[9688 op=2 display+exit]
  1: "You look pretty full to me."

[9689 op=2 display+exit]
  1: "Excellent!"
  2: He hands over the flour.

[9692 op=2 display+exit]
  1: "Suit yourself."

[9694 op=2 display+exit]
  1: "I can only take up to 20 sacks now."

[9708 op=2 display+exit]
  1: "But ya don't have any grain!"

[9711 op=2 display+exit]
  1: "Done!"
  2: He hands you some gold pieces and takes the grain.

```

<a id="guard-npc-200"></a>
## Guard ? NPC 200, port 201

Instruction range: 16989?17013

```text
[16989 op=1 keyword; failure -> 16991]
  1: look

[16990 op=2 display+exit]
  1: a muscular, well armored guard.
  2: "Hello."
  3: "Can I help you with something?"

[16991 op=1 keyword; failure -> 16993]
  1: yes

[16992 op=2 display+exit]
  1: "Well, what can I help you with?"

[16993 op=1 keyword; failure -> 16995]
  1: name

[16994 op=2 display+exit]
  1: "I'm not allowed to give out that information."

[16995 op=1 keyword; failure -> 16997]
  1: job

[16996 op=2 display+exit]
  1: "I am one of Lord British's elite guards."
  2: "We keep @order throughout @Britannia."

[16997 op=1 keyword; failure -> 16999]
  1: brit

[16998 op=2 display+exit]
  1: "Hail Lord British!"
  2: "Hail Britannia!"
  3: "Hail the glorious Avatar!"

[16999 op=1 keyword; failure -> 17001]
  1: elit,guar,keep,orde

[17000 op=2 display+exit]
  1: "Lately our ranks have thinned."
  2: "Many have been lost in the war against the @gargoyles."

[17001 op=1 keyword; failure -> 17003]
  1: rank,thin,many,lost,garg

[17002 op=2 display+exit]
  1: "Few of us are left to @patrol the cities, and those may be sent to the shrines soon."
  2: "I fear we may @lose this war."

[17003 op=1 keyword; failure -> 17005]
  1: shri,lose,fear,sent

[17004 op=2 display+exit]
  1: "Not a single shrine have we liberated."
  2: "The gargoyles fight like @daemons!"

[17005 op=1 keyword; failure -> 17007]
  1: demo,daem

[17006 op=2 display+exit]
  1: "I have noticed a certain resemblance..."

[17007 op=1 keyword; failure -> 17009]
  1: patr,citi

[17008 op=2 display+exit]
  1: "That is our only job, in times of @peace."

[17009 op=1 keyword; failure -> 17011]
  1: time,peac

[17010 op=2 display+exit]
  1: "May we see such times again!"

[17011 op=1 keyword; failure -> 17013]
  1: bye

[17012 op=2 display+exit]
  1: "If that is all, then I'll be back about my duties."

[17013 op=2 display+exit]
  1: "Perhaps you should ask somebody else."

```

<a id="gwenneth-npc-14"></a>
## Gwenneth ? NPC 14, port 15

Instruction range: 1063?1243

```text
[1064 op=1 keyword; failure -> 1068]
  1: look

[1065 op=22 display+continue]
  1: a petite woman with flowing brown hair.
  2: "Iolo! And thy companions as well!"
  3: "I heard rumors that you were in town!"
  4: "Very well, very well indeed. Sir Geoffrey himself just placed quite a large order!"
  5: "Much demand for bows these days, what with the @gargoyles!"
  6: Turning to you, Gwenneth says, "And what can I do for Iolo's friend this fine $T?"

[1067 op=2 display+exit]
  1: Iolo greets his former apprentice warmly.
  2: "How goes business, Gwenneth?"

[1068 op=1 keyword; failure -> 1070]
  1: look

[1069 op=2 display+exit]
  1: a petite woman with flowing brown hair.
  2: "I recongize thee! Thou art Iolo's friend, $P!"
  3: "What can I do for thee this fine $T?"

[1070 op=1 keyword; failure -> 1074]
  1: name

[1073 op=2 display+exit]
  1: "What a strange question."
  2: "I'm Gwenneth, of course."
  3: "Just ask Iolo!"

[1074 op=1 keyword; failure -> 1077]
  1: name

[1076 op=2 display+exit]
  1: "What a strange question."
  2: "I'm Gwenneth, of course."
  3: "Just ask Iolo!"
  4: "But dost thou not know Iolo?"
  5: "Well, I am his former apprentice, Gwenneth."

[1077 op=1 keyword; failure -> 1082]
  1: job

[1079 op=22 display+continue]
  1: "Once was I Iolo's apprentice, but then he decided to retire."
  2: "Now 'tis my shop, and I craft the bows sold here!"
  3: "'Course, 'tis still called @Iolo's Bows, out of respect for the master."

[1081 op=2 display+exit]
  1: At this, Iolo blushes humbly.

[1082 op=1 keyword; failure -> 1084]
  1: job

[1083 op=2 display+exit]
  1: "Once was I Iolo's apprentice, but then he decided to retire."
  2: "Now 'tis my shop, and I craft the bows sold here!"
  3: "'Course, 'tis still called @Iolo's Bows, out of respect for the master."

[1084 op=1 keyword; failure -> 1086]
  1: garg

[1085 op=2 display+exit]
  1: "I hear from the soldiers that the gargoyles are fierce opponents."
  2: "Some gargoyles move like the wind itself, and can strike you like lightning bolts!"
  3: "The gargoyles also have a strange and terrible @weapon."

[1086 op=1 keyword; failure -> 1088]
  1: weap

[1087 op=2 display+exit]
  1: "'Tis called a boomerang, and it is a most bizarre ranged weapon."
  2: "It flies out to strike its target, then returns to the hand that threw it!"
  3: "Against such magic the soldiers chose to arm themselves with good, stout Britannian bows!"

[1088 op=1 keyword; failure -> 1091]
  1: iolo

[1090 op=2 display+exit]
  1: She smiles. "I would not wish to speak of Iolo amongst his friends."
  2: "I might embarass him!"

[1091 op=1 keyword; failure -> 1093]
  1: iolo

[1092 op=2 display+exit]
  1: "Nay, I really would not wish to speak of my master behind his back."

[1093 op=1 keyword; failure -> 1095]
  1: arro,bolt

[1094 op=2 display+exit]
  1: "Aye, usually do I carry those in stock."
  2: "But there has been a shortage of late, because of the @gargoyle war."
  3: "Go see Lynn the Fletcher, over to the north."

[1095 op=1 keyword; failure -> 1097]
  1: lynn

[1096 op=2 display+exit]
  1: "Lynn is a fine fletcher, and usually keeps me supplied with @arrows and @bolts."
  2: "I've none left in stock, though, since the soldiers bought them all."

[1097 op=1 keyword; failure -> 1100]
  1: bye

[1099 op=2 display+exit]
  1: "Farewell, all of you."
  2: "Iolo, I'll see thee again soon with thy share of this month's profits!"

[1100 op=1 keyword; failure -> 1102]
  1: bye

[1101 op=2 display+exit]
  1: "Farewell to thee!"

[1102 op=1 keyword; failure -> 1104]
  1: buy

[1103 op=2 display+exit]
  1: "I sell @bows, @crossbows, @slings and @magic @bows. Which item?"

[1104 op=1 keyword; failure -> 1107]
  1: trip

[1106 op=2 display+exit]
  1: "Ah, I see thou art a discerning buyer."
  2: "The triple-crossbow is a marvel to behold."
  3: "This enchanted weapon fires three bolts at once!"
  4: "Normally, I would not offer it for sale."
  5: "But, because thy cause is important, I will allow thee--and thee only!--to buy it."
  6: "But I fear I must charge 400 gold for it."
  7: "How many dost thou wish to buy?"

[1107 op=1 keyword; failure -> 1110]
  1: cros

[1109 op=2 display+exit]
  1: "That crossbow costs 40 gold."
  2: "Interested in how many, $Y?"

[1110 op=1 keyword; failure -> 1113]
  1: magi

[1112 op=2 display+exit]
  1: "That magic bow costs 300 gold."
  2: "Interested in how many, $Y?"

[1113 op=1 keyword; failure -> 1116]
  1: bow

[1115 op=2 display+exit]
  1: "That bow costs 30 gold."
  2: "Interested in how many, $Y?"

[1116 op=1 keyword; failure -> 1119]
  1: slin

[1118 op=2 display+exit]
  1: "That sling costs 10 gold."
  2: "Interested in how many, $Y?"

[1119 op=1 keyword; failure -> 1122]
  1: boom

[1121 op=2 display+exit]
  1: "That boomerang costs 10 gold."
  2: "Interested in how many, $Y?"

[1146 op=1 keyword; failure -> 1148]
  1: sell

[1148 op=1 keyword; failure -> 1150]
  1: bulk

[1162 op=2 display+exit]
  1: "Beg pardon?"

[1164 op=2 display+exit]
  1: "I don't have that many offhand!"

[1168 op=2 display+exit]
  1: "Iolo told me you'd try that! Bad Avatar!"
  2: "Trying to steal from your friends behind their back!"

[1170 op=2 display+exit]
  1: "I know you have legendary strength, but even that won't help you carry these..."

[1171 op=2 display+exit]
  1: "It's all yours."

[1178 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 15 gold for that bow?"

[1184 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 20 gold for that crossbow?"

[1190 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 150 gold for that magic bow?"

[1196 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 5 gold for that sling?"

[1199 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[1202 op=1 keyword; failure -> 1206]
  1: yes

[1205 op=2 display+exit]
  1: "Done!"
  2: She hands you 15 gold pieces and takes the bow.

[1207 op=1 keyword; failure -> 1211]
  1: yes

[1210 op=2 display+exit]
  1: "Done!"
  2: She hands you 20 gold pieces and takes the crossbow.

[1212 op=1 keyword; failure -> 1216]
  1: yes

[1215 op=2 display+exit]
  1: "Done!"
  2: She hands you 150 gold pieces and takes the magic bow.

[1217 op=1 keyword; failure -> 1221]
  1: yes

[1220 op=2 display+exit]
  1: "Done!"
  2: She hands you 5 gold pieces and takes the sling.

[1222 op=2 display+exit]
  1: "Changed your mind, eh?"

[1230 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[1233 op=2 display+exit]
  1: "These bows will greatly help to resupply Sir Geoffrey's soldiers!"
  2: "Will you take $V0 gold for all of your bows?"

[1234 op=2 display+exit]
  1: "Will you take $V0 gold for all of your bows?"

[1236 op=1 keyword; failure -> 1238]
  1: yes

[1238 op=2 display+exit]
  1: "Changed your mind, eh?"

[1241 op=2 display+exit]
  1: "$Y, these are fewer items than we agreed upon."
  2: "You're lucky we townsfolk don't call the guard after thieves!"

[1243 op=2 display+exit]
  1: "Excellent!"
  2: She hands you $V0 gold pieces in exchange for the items.

```

<a id="gwenno-npc-64"></a>
## Gwenno ? NPC 64, port 65

Instruction range: 6042?6120

```text
[6043 op=1 keyword; failure -> 6046]
  1: yes

[6048 op=2 display+exit]
  1: "Okay."
  2: "Let me know if you need my help later."

[6049 op=1 keyword; failure -> 6052]
  1: look

[6051 op=2 display+exit]
  1: a plainly dressed bard.
  2: "Is there something you needed to know?"

[6052 op=1 keyword; failure -> 6054]
  1: look

[6053 op=2 display+exit]
  1: a plainly dressed bard.
  2: "Hello, $P."

[6054 op=1 keyword; failure -> 6056]
  1: name

[6055 op=2 display+exit]
  1: "It's me, Gwenno."
  2: "You have been away from our realm a long time, haven't you..."

[6056 op=1 keyword; failure -> 6059]
  1: job

[6058 op=2 display+exit]
  1: "Iolo's @apprentice has taken over my job at @Iolo's Bows."
  2: "These days I've been transcribing the folk @songs of Britannia into numerical @notation for the artisan's @guild."
  3: "Of course, helping you on your quest is more important than that right now."

[6059 op=1 keyword; failure -> 6061]
  1: job

[6060 op=2 display+exit]
  1: "Iolo's @apprentice has taken over my job at @Iolo's Bows."
  2: "These days I've been transcribing the folk @songs of Britannia into numerical @notation for the artisan's @guild."
  3: "Of course, if you need my help, I could leave the guild for a time and @join you."

[6061 op=1 keyword; failure -> 6063]
  1: arti,guil

[6062 op=2 display+exit]
  1: "@Selganor can tell you more about the guild than I could."

[6063 op=1 keyword; failure -> 6065]
  1: selg

[6064 op=2 display+exit]
  1: "He's the guildmaster."

[6065 op=1 keyword; failure -> 6068]
  1: join,help,ques

[6067 op=2 display+exit]
  1: "I have joined you already!"

[6068 op=1 keyword; failure -> 6071]
  1: join,help,ques

[6070 op=2 display+exit]
  1: "Do you want me to come with you?"

[6071 op=1 keyword; failure -> 6073]
  1: leav

[6073 op=1 keyword; failure -> 6075]
  1: folk,song

[6074 op=2 display+exit]
  1: "The songs of a people are an important part of their heritage."
  2: "Putting them down on paper helps preserve our @traditions for future generations."

[6075 op=1 keyword; failure -> 6077]
  1: heri,pape,trad,futu,gene

[6076 op=2 display+exit]
  1: "I'm so glad you appreciate the value of tradition."

[6077 op=1 keyword; failure -> 6082]
  1: iolo

[6079 op=22 display+continue]
  1: "A bit scruffy, but he's the sweetest husband I could ever ask for."

[6081 op=2 display+exit]
  1: Iolo says "You're only saying that because it's true."
  2: He gives her a kiss.

[6082 op=1 keyword; failure -> 6084]
  1: iolo

[6083 op=2 display+exit]
  1: "A bit scruffy, but he's the sweetest husband I could ever ask for."

[6084 op=1 keyword; failure -> 6086]
  1: appr,bow

[6085 op=2 display+exit]
  1: "If you need a good crossbow, the shop is in Britain, over by the Blue @Boar."

[6086 op=1 keyword; failure -> 6088]
  1: blue,boar

[6087 op=2 display+exit]
  1: "It's a lively place."

[6088 op=1 keyword; failure -> 6090]
  1: nume,nota,tran

[6089 op=2 display+exit]
  1: "A number for every @note, and a note for every @number."

[6090 op=1 keyword; failure -> 6092]
  1: note

[6091 op=2 display+exit]
  1: "Each note is represented by one @number."

[6092 op=1 keyword; failure -> 6094]
  1: numb

[6093 op=2 display+exit]
  1: "Each number @represents one @note."

[6094 op=1 keyword; failure -> 6096]
  1: repr

[6095 op=2 display+exit]
  1: "I knew you'd catch on!"

[6096 op=1 keyword; failure -> 6098]
  1: play,tune

[6097 op=2 display+exit]
  1: "Oh, Selganor sent you, did he?"
  2: "I bet you want to learn '@Stones.'"

[6098 op=1 keyword; failure -> 6100]
  1: lear,ston

[6099 op=2 display+exit]
  1: "In the standard numerical notation, it reads '678987 8767653'."

[6100 op=1 keyword; failure -> 6102]
  1: bye

[6101 op=2 display+exit]
  1: "Nice talking with you."

[6102 op=2 display+exit]
  1: "Sorry, can't help you there."

[6105 op=2 display+exit]
  1: "I have joined you already!"

[6107 op=2 display+exit]
  1: "You have enough travelling with you already."
  2: "If you ask one of them to @leave first, I'd be happy to join you."

[6109 op=2 display+exit]
  1: "Not while I'm helping someone else!"

[6111 op=2 display+exit]
  1: "Okay!"
  2: "It will be nice to spend some time on the road again."
  3: She runs over and hooks arms with @Iolo, and they dance around and around.
  4: They're obviously very happy to see each other again.

[6112 op=2 display+exit]
  1: "Okay!"
  2: "It will be nice to spend some time on the road again."

[6115 op=2 display+exit]
  1: "I'd have to @join you before I could leave, silly!"

[6117 op=2 display+exit]
  1: "Let's head back home to Minoc first, okay?"

[6119 op=2 display+exit]
  1: "All right."
  2: "I'll head back home to Minoc."
  3: "Come let me know if you need me to join you again later."
  4: "I'll leave all my things here - some of them might come in handy for you."
  5: She gives Iolo a big hug before she leaves.
  6: "Remember to keep your socks dry," she says, "and I'll see you when the quest is done."

[6120 op=2 display+exit]
  1: "All right."
  2: "I'll head back home to Minoc."
  3: "Come let me know if you need me to join you again later."
  4: "I'll leave all my things here - some of them might come in handy for you."

```

<a id="harold-npc-76"></a>
## Harold ? NPC 76, port 77

Instruction range: 7131?7154

```text
[7135 op=2 display+exit]
  1: "Very well."

[7136 op=1 keyword; failure -> 7138]
  1: look

[7137 op=2 display+exit]
  1: a tall, muscular man with a solemn look on his face.
  2: "Welcome, $G."
  3: "What can I do for you?"

[7138 op=1 keyword; failure -> 7140]
  1: name

[7139 op=2 display+exit]
  1: "My name is Harold."

[7140 op=1 keyword; failure -> 7143]
  1: buy,hors,shoe

[7142 op=2 display+exit]
  1: "It'll cost you 2 gold for the horseshoes, interested?"

[7143 op=1 keyword; failure -> 7145]
  1: job

[7144 op=2 display+exit]
  1: "@Horseshoes are my living."

[7145 op=1 keyword; failure -> 7147]
  1: bye

[7146 op=2 display+exit]
  1: "Farewell."

[7147 op=2 display+exit]
  1: "I can't help you with that."

[7151 op=2 display+exit]
  1: "Best check your coinpouch again."

[7153 op=2 display+exit]
  1: "You look pretty full to me."

[7154 op=2 display+exit]
  1: "Here you go."
  2: "What can I do for you?"

```

<a id="healer-npc-171"></a>
## Healer ? NPC 171, port 172

Instruction range: 15230?15298

```text
[15233 op=1 keyword; failure -> 15236]
  1: yes

[15235 op=2 display+exit]
  1: "Which of you?"

[15238 op=2 display+exit]
  1: The gargoyle shrugs.
  2: "To assure you that it is safe and invigorating."
  3: "But not to force anyone to act against their will."

[15243 op=1 keyword; failure -> 15245]
  1: look

[15244 op=2 display+exit]
  1: a somber and calm gargoyle.
  2: "An in mani ulem, An-bal-sil-fer."
  3: The gargoyle says nothing else.

[15245 op=2 display+exit]
  1: "An in mani ulem, An-bal-sil-fer."
  2: The gargoyle says nothing else.

[15246 op=1 keyword; failure -> 15248]
  1: look

[15247 op=2 display+exit]
  1: a somber and calm gargoyle.
  2: "To be unwilling to help you, False Prophet."
  3: "To be a healer, a being of peace."
  4: "Elsewise, to summon the guards to slay you!"
  5: The gargoyle says nothing else.

[15248 op=2 display+exit]
  1: "To be unwilling to help you, False Prophet."
  2: "To be a healer, a being of peace."
  3: "Elsewise, to summon the guards to slay you!"
  4: The gargoyle says nothing else.

[15249 op=1 keyword; failure -> 15252]
  1: look

[15251 op=2 display+exit]
  1: a somber and calm gargoyle.
  2: "To be happy to see you again, noble one."
  3: "But to be sad that you seek my services."

[15252 op=1 keyword; failure -> 15255]
  1: look

[15254 op=2 display+exit]
  1: a somber and calm gargoyle.
  2: "To see the amulet, the symbol of your courage and nobility."
  3: "To wish to help you however I can."

[15255 op=1 keyword; failure -> 15258]
  1: heal,cure

[15257 op=2 display+exit]
  1: "To ask that you see me at my shop."

[15258 op=1 keyword; failure -> 15260]
  1: resu,dead

[15259 op=2 display+exit]
  1: "To ask me to raise the dead?"
  2: "To apologize, but that is beyond my power."
  3: "To doubt that anyone can do that!"

[15260 op=1 keyword; failure -> 15262]
  1: name

[15261 op=2 display+exit]
  1: "To be nameless, being only a healer."

[15262 op=1 keyword; failure -> 15264]
  1: job

[15263 op=2 display+exit]
  1: "To be entrusted with the tasks of @healing those wounded and @curing those sick."
  2: "To do this for all, as a service to society."

[15264 op=1 keyword; failure -> 15266]
  1: good

[15265 op=2 display+exit]
  1: "The goodscrafter?"
  2: "To regret his condition."
  3: "To know that he is in great pain after the loss of his @family."

[15266 op=1 keyword; failure -> 15268]
  1: fami

[15267 op=2 display+exit]
  1: "To know that his family was lost when the edges of the world @fell off."
  2: "But to have less and less patience for his dispassionate self-pity."

[15268 op=1 keyword; failure -> 15270]
  1: fell,off

[15269 op=2 display+exit]
  1: "Yes, during the cataclysms."
  2: "To have lost friends myself during that bad time."
  3: "But to maintain the struggle for life diligently!"

[15270 op=1 keyword; failure -> 15273]
  1: heal,cure

[15272 op=2 display+exit]
  1: The gargoyle mixes strange powders into a foul-smelling brew.
  2: "To drink this and be healthy."
  3: Do you accept it?

[15273 op=1 keyword; failure -> 15275]
  1: bye

[15274 op=2 display+exit]
  1: "May fortune prevent the need for your return!"

[15275 op=2 display+exit]
  1: "To be a healer, dedicated only to my @job."
  2: "To know nothing about $Z."

[15278 op=2 display+exit]
  1: "To ask if there is aught else I can do for you?"

[15282 op=2 display+exit]
  1: $PARTYMEMBER drinks the potion and feels slightly dizzy.
  2: "To apologize for the error!"
  3: "But to know exactly what I did wrong."
  4: The gargoyle mixes a different drink that somehow smells worse than the last.
  5: Do you accept it?

[15286 op=2 display+exit]
  1: $PARTYMEMBER drinks the potion and feels deathly ill!
  2: "To apologize for the error!"
  3: "But to know exactly what I did wrong."
  4: The gargoyle mixes a different drink that somehow smells worse than the last.
  5: Do you accept it?

[15297 op=2 display+exit]
  1: "To be unfamiliar with human anatomy..."
  2: "But to be certain that you are well!"

[15298 op=2 display+exit]
  1: $PARTYMEMBER drinks the potion and feels restored!
  2: "To ask if there is aught else I can do for you?"

```

<a id="heftimus-npc-45"></a>
## Heftimus ? NPC 45, port 46

Instruction range: 4133?4203

```text
[4135 op=1 keyword; failure -> 4138]
  1: yes

[4137 op=2 display+exit]
  1: "How much do you give?"

[4141 op=2 display+exit]
  1: "Pah!"
  2: He spits on the ground.

[4149 op=2 display+exit]
  1: "You do not have that much."

[4154 op=2 display+exit]
  1: "Thank, ye kindly, matey."

[4156 op=2 display+exit]
  1: "Thank, ye kindly, matey."

[4157 op=1 keyword; failure -> 4160]
  1: look

[4159 op=2 display+exit]
  1: a beggar, clad in the rags of a sailor, with a hook for a right hand.
  2: "Spare a doubloon fer an old seahand?"
  3: He holds out his hand.
  4: Do you give him any money?

[4162 op=2 display+exit]
  1: "Spare a doubloon fer an old seahand?"
  2: He holds out his hand.
  3: Do you give him any money?

[4163 op=1 keyword; failure -> 4166]
  1: name

[4165 op=2 display+exit]
  1: "I be Heftimus McPry, matey."

[4166 op=1 keyword; failure -> 4168]
  1: job

[4167 op=2 display+exit]
  1: "Once Buccaneer's @Den quaked at the mention o' me name."
  2: "I was the greatest sabre fighter on all the high seas."
  3: "Then one day..."
  4: "I fought Captain @Hawkins, that scurvy dog, an' he struck off me @hand with his sword."

[4168 op=1 keyword; failure -> 4170]
  1: hook,foug,stru,hand,swor

[4169 op=2 display+exit]
  1: "'Twas a bitter fight; I almost died o' the wound."
  2: "Hawkins threw me hand to the sharks."

[4170 op=1 keyword; failure -> 4172]
  1: capt,hawk

[4171 op=2 display+exit]
  1: "He be long gone now, he an' his ship the Empire. A pox on his memory!"

[4172 op=1 keyword; failure -> 4174]
  1: bucc,den

[4173 op=2 display+exit]
  1: "That be an island, matey, and no place fer a decent soul like yerself."

[4174 op=1 keyword; failure -> 4176]
  1: dung

[4175 op=2 display+exit]
  1: "I've been into the dungeon @Wrong."
  2: "There's a @cave on this island that looks a lot safer."

[4176 op=1 keyword; failure -> 4178]
  1: cave

[4177 op=2 display+exit]
  1: "I sleep in front of it every night."
  2: "Soon as I've saved up a stake, I'm going in there to dig for gold."

[4178 op=1 keyword; failure -> 4180]
  1: wron

[4179 op=2 display+exit]
  1: "'Twas certainly the wrong place for an old seadog to be!"
  2: He laughs, chokes, and coughs for a while.
  3: "I'll be allright, matey."

[4180 op=1 keyword; failure -> 4184]
  1: map

[4183 op=2 display+exit]
  1: "That bit o' news will cost ye twenty coins."
  2: "What say ye, matey?"
  3: "Aye or nay?"

[4185 op=1 keyword; failure -> 4190]
  1: yes

[4189 op=2 display+exit]
  1: "Hmm."
  2: "I thank ye kindly, matey.
  3: "The last I saw o' that bit o' parchment..."
  4: "I needed it to start a fire in a @dungeon."
  5: "But afore I could apply sparks to the tinder, a huge swarm o' rats drove me away."
  6: "The scrap o' map lies in the dungeon @Wrong, matey."

[4191 op=1 keyword; failure -> 4194]
  1: yes

[4193 op=2 display+exit]
  1: "I can't help ye, matey."

[4197 op=2 display+exit]
  1: "Then maybe ten?"
  2: "Aye or nay?"

[4200 op=2 display+exit]
  1: "So be it, matey."

[4201 op=1 keyword; failure -> 4203]
  1: bye

[4202 op=2 display+exit]
  1: "Good journey."

[4203 op=2 display+exit]
  1: "Sorry, don't really know that."

```

<a id="hendle-npc-105"></a>
## Hendle ? NPC 105, port 106

Instruction range: 9828?9892

```text
[9829 op=1 keyword; failure -> 9831]
  1: yes

[9833 op=2 display+exit]
  1: "Whatever."

[9834 op=1 keyword; failure -> 9837]
  1: look

[9836 op=2 display+exit]
  1: a man whose pungent smell greets you before he can.
  2: "Well, hello there, friend!"
  3: "Back for more @meat?"

[9837 op=1 keyword; failure -> 9840]
  1: look

[9839 op=2 display+exit]
  1: a man whose pungent smell greets you before he can.
  2: "Well, hello there, stranger!"
  3: "Come for some @meat?"

[9840 op=1 keyword; failure -> 9842]
  1: name

[9841 op=2 display+exit]
  1: "@Hendle the Slaughterman."

[9842 op=1 keyword; failure -> 9844]
  1: hend,slau

[9843 op=2 display+exit]
  1: "Yep."

[9844 op=1 keyword; failure -> 9846]
  1: job

[9845 op=2 display+exit]
  1: "As ya can see, I slaughter the beef and pork which you can then buy."
  2: "I slaughter beef and pork."
  3: "Makes me a bit unpopular 'ere in the pub!" he laughs.

[9847 op=1 keyword; failure -> 9849]
  1: buy,meat,ham,bacon,pork,chops,brisket,steak,ribs

[9848 op=2 display+exit]
  1: "Strewth!"
  2: "If I carried it around with me, I'd be even more unpopular!"
  3: He laughs, then continues, "Nay, friend, come to my shop later."
  4: "I'll fix ya up then."

[9849 op=1 keyword; failure -> 9851]
  1: buy,meat

[9850 op=2 display+exit]
  1: "Which item, @ham, @bacon, @pork @chops, @brisket, @steak or @ribs?"

[9851 op=1 keyword; failure -> 9856]
  1: ham

[9855 op=2 display+exit]
  1: Hendle smiles at you, "That will cost you 3 gold."
  2: "Interested?"

[9856 op=1 keyword; failure -> 9861]
  1: baco

[9860 op=2 display+exit]
  1: Hendle smiles at you, "That will cost you 4 gold."
  2: "Interested?"

[9861 op=1 keyword; failure -> 9866]
  1: pork,chop

[9865 op=2 display+exit]
  1: Hendle smiles at you, "That will cost you 3 gold."
  2: "Interested?"

[9866 op=1 keyword; failure -> 9871]
  1: brisket

[9870 op=2 display+exit]
  1: Hendle smiles at you, "That will cost you 6 gold."
  2: "Interested?"

[9871 op=1 keyword; failure -> 9876]
  1: steak

[9875 op=2 display+exit]
  1: Hendle smiles at you, "That will cost you 8 gold."
  2: "Interested?"

[9876 op=1 keyword; failure -> 9881]
  1: ribs

[9880 op=2 display+exit]
  1: Hendle smiles at you, "That will cost you 9 gold."
  2: "Interested?"

[9881 op=1 keyword; failure -> 9883]
  1: bye

[9882 op=2 display+exit]
  1: "Be careful out there."

[9884 op=2 display+exit]
  1: He stops chopping and scratches his chin.
  2: "Say what?"

[9885 op=2 display+exit]
  1: He takes a swig from his mug and asks, "What d'ya mean?"

[9889 op=2 display+exit]
  1: "You don't have enough gold, $Y."

[9891 op=2 display+exit]
  1: "You look pretty full to me, $Y."

[9892 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the food.

```

<a id="homer-npc-114"></a>
## Homer ? NPC 114, port 115

Instruction range: 10702?10837

```text
[10703 op=1 keyword; failure -> 10705]
  1: look

[10704 op=2 display+exit]
  1: a shifty-eyed character.
  2: He carries a cane and walks with a slight limp.
  3: "You served your purpose by bringing me the cloak..."
  4: "Now I have no further need of you."
  5: "I have big plans now, and no time to waste talking."

[10706 op=2 display+exit]
  1: "You served your purpose by bringing me the cloak..."
  2: "Now I have no further need of you."
  3: "I have big plans now, and no time to waste talking."

[10709 op=2 display+exit]
  1: "Have you found the storm cloak yet?"

[10711 op=1 keyword; failure -> 10714]
  1: yes

[10713 op=2 display+exit]
  1: "Then hand it over and we're even."
  2: His eyes gleam with greedy anticipation.
  3: Do you give him the cloak?

[10716 op=2 display+exit]
  1: "Then get off your duff and go dig it up!"
  2: "I'll tell you this again."
  3: "When you reach the island marked with the X,
  4: find the three stones and stand in the center."
  5: "Then walk three paces due south,
  6: nine paces due west,
  7: and twelve more paces south."
  8: "That should put you right next to an old dead tree."
  9: "Dig in the patch of dirt just to the south of you, and you'll find the treasure!"
  10: "Now go get it!"

[10718 op=1 keyword; failure -> 10721]
  1: yes

[10724 op=2 display+exit]
  1: "Blast ye!"
  2: "If I still had two good legs, I'd keelhaul the lot of you!"
  3: "We had a deal! Besides, I helped steal that treasure myself, fair and square."
  4: "Can't trust anybody these days..."

[10726 op=1 keyword; failure -> 10729]
  1: look

[10728 op=2 display+exit]
  1: a shifty-eyed character.
  2: He carries a cane and walks with a slight limp.
  3: "So, have ye found the eight pieces of the @map yet?"

[10734 op=2 display+exit]
  1: "Fine, '$Z' - if that's what your name really is... You can call me Homer."

[10739 op=2 display+exit]
  1: "Fine, '$Z' - if that's what your name really is... You can call me Homer."

[10742 op=2 display+exit]
  1: "I see."
  2: He screws his face up into an even more suspicious expression than before.
  3: "Just why do you want to know about it, anyway?"

[10746 op=2 display+exit]
  1: "You're looking for the silver tablet?"
  2: "It's part of Captain Hawkin's buried @treasure."

[10749 op=2 display+exit]
  1: "You're not a member of the @guild. I don't have to tell you anything."

[10751 op=1 keyword; failure -> 10754]
  1: yes

[10756 op=2 display+exit]
  1: "Then what are you wasting my time for?"

[10758 op=1 keyword; failure -> 10763]
  1: yes

[10762 op=2 display+exit]
  1: "Okay."
  2: "The ninth piece of the map is hidden..."
  3: "Right here in my pocket!"
  4: He grins wickedly.
  5: "I had to keep it safe while you were off gathering the others, didn't I?"
  6: He hands you the last piece of the map.
  7: "The island in the upper left hand corner is Buccaneer's Den."
  8: "You'll keep your word and come right back here with the magic storm cloak, won't you?"

[10765 op=2 display+exit]
  1: "Suit yourself, mate, that's the only deal I'll offer."

[10767 op=1 keyword; failure -> 10771]
  1: yes

[10770 op=2 display+exit]
  1: "Good."
  2: "Then I'll tell you this."
  3: "When you reach the island marked with the X,
  4: find the three stones and stand in the center."
  5: "Then walk three paces due south,
  6: nine paces due west,
  7: and twelve more paces south."
  8: "That should put you right next to an old dead tree."
  9: "Dig in the patch of dirt just to the south of you, and you'll find the treasure!"
  10: "Now go get it!"

[10773 op=2 display+exit]
  1: "Don't want to keep your word now that you have the map, eh?"
  2: "Well, then, I won't tell you where you need to dig to find the treasure!"

[10774 op=1 keyword; failure -> 10776]
  1: look

[10775 op=2 display+exit]
  1: a shifty-eyed character.
  2: He carries a cane and walks with a slight limp.
  3: He takes your measure, looking you over from head to toe.
  4: "What do you want?"

[10776 op=1 keyword; failure -> 10779]
  1: name

[10778 op=2 display+exit]
  1: "Who wants to know?"

[10779 op=1 keyword; failure -> 10781]
  1: home

[10780 op=2 display+exit]
  1: "Well, get on with it!"

[10781 op=1 keyword; failure -> 10783]
  1: job

[10782 op=2 display+exit]
  1: "I once sailed on the ship called '@Empire,' under Captain @Hawkins."

[10783 op=1 keyword; failure -> 10785]
  1: empi,sail

[10784 op=2 display+exit]
  1: "It was wrecked on the cape, southwest of here. Not too far from Serpent's Hold."

[10785 op=1 keyword; failure -> 10787]
  1: hawk

[10786 op=2 display+exit]
  1: His eyes light up with hatred.
  2: "That heartless bastard..."
  3: "He was killed by his own men, and it was no worse than he deserved."
  4: He hesitates, then adds quickly, "Of course, I had nothing to do with it."

[10787 op=1 keyword; failure -> 10790]
  1: silv,tabl

[10789 op=2 display+exit]
  1: He regards you warily.
  2: "Who sent you?"

[10790 op=1 keyword; failure -> 10793]
  1: buri,trea

[10792 op=2 display+exit]
  1: "It was buried in a small @cave."

[10793 op=1 keyword; failure -> 10795]
  1: buri,trea

[10794 op=2 display+exit]
  1: "Don't know anything about it."

[10795 op=1 keyword; failure -> 10799]
  1: cave

[10798 op=2 display+exit]
  1: "After Captain Hawkins passed away, we tore his treasure @map into nine pieces."
  2: "The plan was, when nobody was looking for us any more, we'd get together and go dig it up."
  3: "Splitting up the @map was my idea - that way nobody could doublecross the others."
  4: "I figure after all these years the others must have given up,
  5: so its alright for me to search for the treasure by myself."
  6: "Trouble is, I've got a bit of the gout in one leg, and I can't travel much any more."
  7: "Maybe we can help each other out."
  8: "I know where my piece of the @map is hidden,
  9: and if you bring me the other eight pieces, perhaps we could make a deal."
  10: "I'll tell you all I know about where the pieces might be..."
  11: "Ol' Hawknose set out for the Dry Land, to kill the daemon that is said to live there."
  12: "Sandy, the ship's cook, went to Trinsic with the first mate."
  13: "Old Ybarra said he was headed for the dungeon Shame, looking for more treasure."
  14: "I think one of the men died in a shipwreck."
  15: "Then there was one more..."
  16: "Can't remember his name, but I've heard tell he settled in Jhelom."
  17: "He'll be easy to recognize - he has a hook in place o' one of his hands."
  18: "That's all I know."
  19: "Perhaps in your travels you can find out where the others have gone."
  20: "When you find the pieces, you can lay them out to see how they fit together."
  21: "But remember, only I know where the ninth piece is,
  22: so come back here when you've got the other eight."

[10799 op=1 keyword; failure -> 10801]
  1: cave

[10800 op=2 display+exit]
  1: "What are you talking about?"

[10801 op=1 keyword; failure -> 10805]
  1: map

[10804 op=2 display+exit]
  1: "You'll keep your word and come right back here with the magic storm cloak, won't you?"

[10805 op=1 keyword; failure -> 10809]
  1: map

[10808 op=2 display+exit]
  1: "So, have ye found the eight pieces of the @map yet?"

[10809 op=1 keyword; failure -> 10811]
  1: map

[10810 op=2 display+exit]
  1: "Don't know anything about it."

[10811 op=1 keyword; failure -> 10813]
  1: guil

[10812 op=2 display+exit]
  1: "Go ask Budo."
  2: "And you didn't hear that from me, understand?"

[10813 op=1 keyword; failure -> 10816]
  1: bye

[10815 op=2 display+exit]
  1: "Farewell, brother thief."

[10816 op=1 keyword; failure -> 10818]
  1: bye

[10817 op=2 display+exit]
  1: "Can't say its been a pleasure."

[10819 op=2 display+exit]
  1: "Can't help you there."

[10820 op=2 display+exit]
  1: "You've got an awful lot of questions... Maybe I don't care to talk about $Z."

[10830 op=2 display+exit]
  1: "Very well."
  2: "I've been thinking about how we could work out a deal."
  3: "I know you want the silver tablet."
  4: "Far as I'm concerned, you can have it."
  5: "All I really want is the magical cloak that's buried with the rest of the treasure."
  6: "So I'll tell you where the ninth piece of the map is if you promise to bring me the cloak."
  7: "The rest of the treasure should be loot enough to satisfy you."
  8: "Is it a deal?"

[10831 op=2 display+exit]
  1: "Best count again."
  2: "You need eight pieces before I'll bargain with you."

[10836 op=2 display+exit]
  1: He takes the cloak from you.
  2: "Thanks for keeping your word."
  3: "There's not many that does, these days..."

[10837 op=2 display+exit]
  1: You have no storm cloak to give him.
  2: "Then get off your duff and go dig it up!"
  3: "I'll tell you this again."
  4: "When you reach the island marked with the X,
  5: find the three stones and stand in the center."
  6: "Then walk three paces due south,
  7: nine paces due west,
  8: and twelve more paces south."
  9: "That should put you right next to an old dead tree."
  10: "Dig in the patch of dirt just to the south of you, and you'll find the treasure!"
  11: "Now go get it!"

```

<a id="honesty-npc-190"></a>
## Honesty ? NPC 190, port 191

Instruction range: 16471?16513

```text
[16475 op=1 keyword; failure -> 16477]
  1: look

[16476 op=2 display+exit]
  1: the altar of Honesty.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about honesty."
  4: Do you wish to meditate at this altar?

[16477 op=1 keyword; failure -> 16480]
  1: yes

[16479 op=2 display+exit]
  1: "Who will meditate?"

[16480 op=1 keyword; failure -> 16482]
  1: no

[16481 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16483 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable honesty."
  3: Do you wish to meditate at this altar?

[16485 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16486 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about honesty."
  3: Do you wish to meditate at this altar?

[16489 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16492 op=2 display+exit]
  1: "Speak the Mantra of Honesty."

[16495 op=1 keyword; failure -> 16497]
  1: ahm

[16498 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16499 op=2 display+exit]
  1: "You receive no enlightenment."

[16505 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16506 op=2 display+exit]
  1: "Whatsoever things are true..."
  2: "Whatsoever things are honest..."
  3: "If there be any virtue and if there be any praise, think on these things."
  4: "There is naught else for you to learn here and now."
  5: "Return when thy journey has progressed further."

[16512 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16513 op=2 display+exit]
  1: "Whatsoever things are true..."
  2: "Whatsoever things are honest..."
  3: "If there be any virtue and if there be any praise, think on these things."
  4: $PARTYMEMBER has gained a level...
  5: and intelligence!

```

<a id="honor-npc-195"></a>
## Honor ? NPC 195, port 196

Instruction range: 16686?16728

```text
[16690 op=1 keyword; failure -> 16692]
  1: look

[16691 op=2 display+exit]
  1: the altar of Honor.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about honor."
  4: Do you wish to meditate at this altar?

[16692 op=1 keyword; failure -> 16695]
  1: yes

[16694 op=2 display+exit]
  1: "Who will meditate?"

[16695 op=1 keyword; failure -> 16697]
  1: no

[16696 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16698 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable honor."
  3: Do you wish to meditate at this altar?

[16700 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16701 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about honor."
  3: Do you wish to meditate at this altar?

[16704 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16707 op=2 display+exit]
  1: "Speak the Mantra of Honor."

[16710 op=1 keyword; failure -> 16712]
  1: summ

[16713 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16714 op=2 display+exit]
  1: "You receive no enlightenment."

[16720 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16721 op=2 display+exit]
  1: "Honor may be assailed, but can never be hurt."
  2: "Surprised by unjust force, but not enthralled."
  3: "There is naught else for you to learn here and now."
  4: "Return when thy journey has progressed further."

[16727 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16728 op=2 display+exit]
  1: "Honor may be assailed, but can never be hurt."
  2: "Surprised by unjust force, but not enthralled."
  3: $PARTYMEMBER has gained a level...
  4: intelligence...
  5: and strength!

```

<a id="horance-npc-86"></a>
## Horance ? NPC 86, port 87

Instruction range: 8141?8475

```text
[8147 op=2 display+exit]
  1: "Perhaps something else then."

[8149 op=2 display+exit]
  1: "That's too many!"

[8160 op=2 display+exit]
  1: "That costs more than thou hast!"

[8162 op=2 display+exit]
  1: "That is too heavy for thee!"

[8164 op=2 display+exit]
  1: He carefully hands you the potion, giving a mad smile.

[8165 op=2 display+exit]
  1: He carefully hands you the potions, giving a mad smile.

[8171 op=2 display+exit]
  1: "Which, @harm?"

[8174 op=2 display+exit]
  1: "Which of these, @magic @arrow, @poison, @trap or @unlock @magic?"

[8177 op=2 display+exit]
  1: "Which of these, @curse or @fireball?"

[8180 op=2 display+exit]
  1: "Which, @disable?"

[8183 op=2 display+exit]
  1: "Which of these, @explosion, @lightning or @paralyze?"

[8186 op=2 display+exit]
  1: "Which of these, @flame @wind, @hail @storm or @poison @wind?"

[8189 op=2 display+exit]
  1: "Which of these, @chain @bolt, @energy @wind, @kill, @mass @curse or @wing @strike?"

[8192 op=2 display+exit]
  1: "I have no spells of that circle."

[8193 op=2 display+exit]
  1: "As everybody knows, there are only eight circles."

[8196 op=1 keyword; failure -> 8201]
  1: harm

[8200 op=2 display+exit]
  1: "That spell costs 30 gold. Interested?"

[8202 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8205 op=1 keyword; failure -> 8210]
  1: pois

[8209 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[8210 op=1 keyword; failure -> 8215]
  1: trap

[8214 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[8215 op=1 keyword; failure -> 8220]
  1: unlo

[8219 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[8220 op=1 keyword; failure -> 8225]
  1: arro

[8224 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[8225 op=1 keyword; failure -> 8227]
  1: magi

[8226 op=2 display+exit]
  1: "Which of these, magic @arrow or @unlock magic?"

[8228 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8231 op=1 keyword; failure -> 8236]
  1: curs

[8235 op=2 display+exit]
  1: "That spell costs 90 gold. Interested?"

[8236 op=1 keyword; failure -> 8241]
  1: fire

[8240 op=2 display+exit]
  1: "That spell costs 90 gold. Interested?"

[8242 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8245 op=1 keyword; failure -> 8250]
  1: disa

[8249 op=2 display+exit]
  1: "That spell costs 120 gold. Interested?"

[8251 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8254 op=1 keyword; failure -> 8259]
  1: expl

[8258 op=2 display+exit]
  1: "That spell costs 150 gold. Interested?"

[8259 op=1 keyword; failure -> 8264]
  1: ligh

[8263 op=2 display+exit]
  1: "That spell costs 150 gold. Interested?"

[8264 op=1 keyword; failure -> 8269]
  1: para

[8268 op=2 display+exit]
  1: "That spell costs 150 gold. Interested?"

[8270 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8273 op=1 keyword; failure -> 8278]
  1: flam

[8277 op=2 display+exit]
  1: "That spell costs 180 gold. Interested?"

[8278 op=1 keyword; failure -> 8283]
  1: hail

[8282 op=2 display+exit]
  1: "That spell costs 180 gold. Interested?"

[8283 op=1 keyword; failure -> 8288]
  1: pois

[8287 op=2 display+exit]
  1: "That spell costs 180 gold. Interested?"

[8289 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8292 op=1 keyword; failure -> 8297]
  1: chai

[8296 op=2 display+exit]
  1: "That spell costs 210 gold. Interested?"

[8297 op=1 keyword; failure -> 8302]
  1: ener

[8301 op=2 display+exit]
  1: "That spell costs 210 gold. Interested?"

[8302 op=1 keyword; failure -> 8307]
  1: kill

[8306 op=2 display+exit]
  1: "That spell costs 210 gold. Interested?"

[8307 op=1 keyword; failure -> 8312]
  1: mass

[8311 op=2 display+exit]
  1: "That spell costs 210 gold. Interested?"

[8312 op=1 keyword; failure -> 8317]
  1: wing

[8316 op=2 display+exit]
  1: "That spell costs 210 gold. Interested?"

[8318 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8322 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8326 op=1 keyword; failure -> 8331]
  1: yes

[8330 op=2 display+exit]
  1: He hands you a piece of rune-covered parchment.

[8332 op=2 display+exit]
  1: "You lack the gold required."

[8334 op=2 display+exit]
  1: "Thou cannot carry that."

[8335 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[8339 op=1 keyword; failure -> 8344]
  1: yes

[8343 op=2 display+exit]
  1: "He hands you the spellbook."

[8345 op=2 display+exit]
  1: "But thou cannot afford a spellbook!"

[8347 op=2 display+exit]
  1: "But thou cannot carry a spellbook!"

[8348 op=2 display+exit]
  1: "Perhaps something else then."

[8349 op=1 keyword; failure -> 8352]
  1: poti

[8351 op=2 display+exit]
  1: "Four potions I do carry,
  2: Four potions thou dost see,
  3: Yet I know not which coloured potion,
  4: Is of interest to thee."

[8355 op=1 keyword; failure -> 8359]
  1: blac

[8358 op=2 display+exit]
  1: "Black potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[8359 op=1 keyword; failure -> 8363]
  1: yell

[8362 op=2 display+exit]
  1: "Yellow potions will cost thee 10 gold each."
  2: "How many wouldst thou like?"

[8363 op=1 keyword; failure -> 8367]
  1: red

[8366 op=2 display+exit]
  1: "Red potions will cost thee 10 gold each."
  2: "How many wouldst thou like?"

[8367 op=1 keyword; failure -> 8371]
  1: blue

[8370 op=2 display+exit]
  1: "Blue potions will cost thee 10 gold each."
  2: "How many wouldst thou like?"

[8371 op=2 display+exit]
  1: "Perhaps something else then."

[8372 op=1 keyword; failure -> 8374]
  1: buy,reag

[8373 op=2 display+exit]
  1: The mention of business brings the senile man to his senses momentarily.
  2: "Which of these, @black @pearl, @blood @moss, @mandrake @root, @nightshade, @spider @silk or @sulfurous @ash?"
  3: "But perhaps thou wouldst prefer some @potions?"

[8374 op=1 keyword; failure -> 8379]
  1: blac,pear

[8378 op=2 display+exit]
  1: "Black pearl costs 3 per portion."
  2: "How many dost thou want?"

[8379 op=1 keyword; failure -> 8384]
  1: bloo,moss

[8383 op=2 display+exit]
  1: "Blood moss costs 3 per portion."
  2: "How many dost thou want?"

[8384 op=1 keyword; failure -> 8389]
  1: mand,root

[8388 op=2 display+exit]
  1: "Mandrake root costs 5 per portion."
  2: "How many dost thou want?"

[8389 op=1 keyword; failure -> 8394]
  1: nigh

[8393 op=2 display+exit]
  1: "Nightshade costs 2 per portion."
  2: "How many dost thou want?"

[8394 op=1 keyword; failure -> 8399]
  1: spid,silk

[8398 op=2 display+exit]
  1: "Spider silk costs 1 per portion."
  2: "How many dost thou want?"

[8399 op=1 keyword; failure -> 8404]
  1: sulf,ash

[8403 op=2 display+exit]
  1: "Sulfurous ash costs 2 per portion."
  2: "How many dost thou want?"

[8408 op=2 display+exit]
  1: "Perhaps something else then."

[8410 op=2 display+exit]
  1: "I cannot give thee that much!"

[8415 op=2 display+exit]
  1: "Thou cannot afford that much!"

[8417 op=2 display+exit]
  1: "Thou cannot carry that much!"

[8418 op=2 display+exit]
  1: "Here you are."

[8419 op=1 keyword; failure -> 8422]
  1: look

[8421 op=2 display+exit]
  1: a bent, aging man with a scraggly beard and few teeth.
  2: "Once they came for wanting,
  3: Twice they came to see,
  4: If I am relenting,
  5: In my mystery."

[8422 op=1 keyword; failure -> 8425]
  1: look

[8424 op=2 display+exit]
  1: a bent, aging man with a scraggly beard and few teeth.
  2: "Dogs are at my doorstep,
  3: Yapping at my heels.
  4: No way to ignore them.
  5: Just pretend that they are eels."

[8425 op=1 keyword; failure -> 8427]
  1: name

[8426 op=2 display+exit]
  1: "Names I'm always thinking,
  2: Names I always see.
  3: Have you any inkling
  4: What they seem to mean?"

[8427 op=1 keyword; failure -> 8429]
  1: job

[8428 op=2 display+exit]
  1: "I have nice @reagents,
  2: Also wondrous @spells.
  3: If you need a @spellbook,
  4: Those I also sell."

[8429 op=1 keyword; failure -> 8432]
  1: hora,tubl

[8431 op=2 display+exit]
  1: "That is too familiar,
  2: Could it be a friend?
  3: No, none is delivered,
  4: So I must pretend."

[8432 op=1 keyword; failure -> 8434]
  1: kill,murd

[8433 op=2 display+exit]
  1: "'Step into my parlor,'
  2: Said the spider to the fly.
  3: Perhaps if he could holler,
  4: He would not have to die."

[8434 op=1 keyword; failure -> 8436]
  1: relt,rent

[8435 op=2 display+exit]
  1: "Many are pretenders,
  2: A harmless game they play.
  3: But for the few offenders,
  4: Of constant night they pray."

[8436 op=1 keyword; failure -> 8438]
  1: ghos

[8437 op=2 display+exit]
  1: He purses his lips and lets out an eery, high-pitched cry.
  2: "That is my impression.
  3: Tell me what you think.
  4: Can you learn a lesson,
  5: From what it has to teach?"

[8438 op=1 keyword; failure -> 8440]
  1: burm,shav

[8439 op=2 display+exit]
  1: "Don't stick your arm
  2: Out too far.
  3: It may go home
  4: In another car!"
  5: -- Burma Shave.

[8440 op=1 keyword; failure -> 8442]
  1: quen

[8441 op=2 display+exit]
  1: "Ask about a traitor,
  2: Ask about a love,
  3: And if you are creative,
  4: Ask above the dove."

[8442 op=1 keyword; failure -> 8444]
  1: garg

[8443 op=2 display+exit]
  1: "What is truly fearsome,
  2: What can evil be?
  3: Not all that growls is gruesome,
  4: Not all that hates has wings."

[8444 op=1 keyword; failure -> 8446]
  1: moon,gate,shri

[8445 op=2 display+exit]
  1: "Through a glowing door,
  2: By the full moons' light.
  3: For me to tell you more
  4: Would take us half the night!"

[8446 op=1 keyword; failure -> 8448]
  1: tren,bell

[8447 op=2 display+exit]
  1: "He who has opinions,
  2: Thinks he knows it all,
  3: Will someday end up learning
  4: What comes before a fall."

[8448 op=1 keyword; failure -> 8450]
  1: yorl

[8449 op=2 display+exit]
  1: "Simple has its moment
  2: And often is received
  3: Much better than the brilliant,
  4: Who often will deceive."

[8450 op=1 keyword; failure -> 8452]
  1: mich

[8451 op=2 display+exit]
  1: "Pain can run so deeply
  2: That one becomes a mute,
  3: In fear of leading others
  4: Where goodness is in dispute."

[8452 op=1 keyword; failure -> 8454]
  1: gide

[8453 op=2 display+exit]
  1: "Kindness is a proverb
  2: That all can understand,
  3: And those who share it with you
  4: Will oft be in demand."

[8454 op=1 keyword; failure -> 8456]
  1: stiv

[8455 op=2 display+exit]
  1: "Little is the difference,
  2: That seperates ourselves.
  3: Remembering this inference,
  4: Is oft a great challenge."

[8456 op=1 keyword; failure -> 8458]
  1: marn

[8457 op=2 display+exit]
  1: "Love has many rivers,
  2: Yet grief can twist them all.
  3: Be guided by your wisdom,
  4: And you'll achieve the goal."

[8458 op=1 keyword; failure -> 8460]
  1: lord,brit

[8459 op=2 display+exit]
  1: "A council has a purpose,
  2: And yet it can be wrong.
  3: There are no notes that singly
  4: Put the melody in a song."

[8460 op=1 keyword; failure -> 8463]
  1: book,spellbook,sell

[8462 op=2 display+exit]
  1: The mention of business brings the senile man to his senses momentarily.
  2: "A spellbook costs 50 gold pieces."
  3: "Interested?"

[8464 op=1 keyword; failure -> 8467]
  1: lear,spel

[8466 op=2 display+exit]
  1: The mention of business brings the senile man to his senses momentarily.
  2: "In which Circle do you wish to study?"

[8468 op=1 keyword; failure -> 8471]
  1: lear,spel

[8470 op=2 display+exit]
  1: The mention of business brings the senile man to his senses momentarily.
  2: "In which Circle do you wish to study?"

[8471 op=1 keyword; failure -> 8473]
  1: lear,spel

[8472 op=2 display+exit]
  1: The mention of business brings the senile man to his senses momentarily.
  2: "But none of you has a @spellbook!"

[8473 op=1 keyword; failure -> 8475]
  1: bye

[8474 op=2 display+exit]
  1: "I see that they are going,
  2: I'm sure they will return.
  3: For if they lack the knowing,
  4: They'll always need to learn."

[8475 op=2 display+exit]
  1: "I hear you have a question.
  2: I'm here to answer some.
  3: If I may make suggestions,
  4: Please ask another one."

```

<a id="humility-npc-197"></a>
## Humility ? NPC 197, port 198

Instruction range: 16772?16814

```text
[16776 op=1 keyword; failure -> 16778]
  1: look

[16777 op=2 display+exit]
  1: the altar of Humility.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about humility."
  4: Do you wish to meditate at this altar?

[16778 op=1 keyword; failure -> 16781]
  1: yes

[16780 op=2 display+exit]
  1: "Who will meditate?"

[16781 op=1 keyword; failure -> 16783]
  1: no

[16782 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16784 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable humility."
  3: Do you wish to meditate at this altar?

[16786 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16787 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about humility."
  3: Do you wish to meditate at this altar?

[16790 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16793 op=2 display+exit]
  1: "Speak the Mantra of Humility."

[16796 op=1 keyword; failure -> 16798]
  1: lum

[16799 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16800 op=2 display+exit]
  1: "You receive no enlightenment."

[16806 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16807 op=2 display+exit]
  1: "The humblest citizen of all the land, 
  2: when clad in the armor of a righteous cause,
  3: is stronger than all the hosts of error."
  4: "There is naught else for you to learn here and now."
  5: "Return when thy journey has progressed further."

[16813 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16814 op=2 display+exit]
  1: "The humblest citizen of all the land, 
  2: when clad in the armor of a righteous cause,
  3: is stronger than all the hosts of error."
  4: $PARTYMEMBER has gained a level!

```

<a id="immanuelle-npc-78"></a>
## Immanuelle ? NPC 78, port 79

Instruction range: 7467?7510

```text
[7468 op=1 keyword; failure -> 7471]
  1: yes

[7473 op=2 display+exit]
  1: "Perhaps some other time then."
  2: "How about for one of thy @friends?"

[7474 op=1 keyword; failure -> 7477]
  1: look

[7476 op=2 display+exit]
  1: a sultry, dark-skinned woman dressed in riding-leathers.
  2: "Hello again, my friend."
  3: "What can I do for you this $T?"

[7477 op=1 keyword; failure -> 7480]
  1: look

[7479 op=2 display+exit]
  1: a sultry, dark-skinned woman dressed in riding-leathers.
  2: "Hello, stranger."
  3: "What can I do for you this $T?"

[7480 op=1 keyword; failure -> 7482]
  1: name

[7481 op=2 display+exit]
  1: "Some call me Immanuelle, $G."
  2: "Others call me...something @else."
  3: She winks at you.

[7482 op=1 keyword; failure -> 7485]
  1: some,else

[7484 op=2 display+exit]
  1: "I think you get the idea."

[7485 op=1 keyword; failure -> 7487]
  1: some,else

[7486 op=2 display+exit]
  1: "Perhaps some time I will @show you..."

[7487 op=1 keyword; failure -> 7489]
  1: show

[7488 op=2 display+exit]
  1: "Not here."

[7489 op=1 keyword; failure -> 7491]
  1: job

[7490 op=2 display+exit]
  1: "I raise horses here, in the stables."
  2: "I also sell @horses, if you wish to buy one."

[7491 op=1 keyword; failure -> 7493]
  1: sell,hors

[7492 op=2 display+exit]
  1: "I can hardly bear to part with any of these magnificent beasts."
  2: "But there are a few that fail to please me."
  3: "If you wish to @buy one of them, just say the word."

[7493 op=1 keyword; failure -> 7496]
  1: buy,frie

[7495 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[7496 op=1 keyword; failure -> 7499]
  1: buy,frie

[7498 op=2 display+exit]
  1: "My price is 60 gold pieces, no less."
  2: "Interested?"

[7499 op=1 keyword; failure -> 7502]
  1: bye

[7501 op=2 display+exit]
  1: "Come back again soon!"

[7502 op=1 keyword; failure -> 7504]
  1: bye

[7503 op=2 display+exit]
  1: "Come back again soon!"
  2: As you leave, she blows you a kiss.

[7504 op=2 display+exit]
  1: "Beg pardon, $G?"

[7507 op=2 display+exit]
  1: "Aye, $P, thou hast need of a horse."
  2: "But thou canst not afford one!"

[7509 op=2 display+exit]
  1: "Aye, $P, thou hast need of a horse."
  2: "But thou canst not carry its papers!"

[7510 op=2 display+exit]
  1: "Good!"
  2: She takes the gold, and hands over the reins of a fine horse.
  3: "How about for one of thy @friends?"

```

<a id="iolo-npc-2"></a>
## Iolo ? NPC 2, port 3

Instruction range: 94?162

```text
[94 op=1 keyword; failure -> 96]
  1: look

[95 op=2 display+exit]
  1: your old friend Iolo.

[96 op=1 keyword; failure -> 99]
  1: name

[98 op=2 display+exit]
  1: "Iolo's the only name I remember having." (He pronounces his name 'Yo-low.')

[99 op=1 keyword; failure -> 101]
  1: hut,home,smit,clue

[100 op=2 display+exit]
  1: "My hut is in the Deep Forest, south of Yew."
  2: "My horse Smith lives there."

[101 op=1 keyword; failure -> 103]
  1: job

[102 op=2 display+exit]
  1: "I've been a crossbow maker for so long, I've gotten weary of it."
  2: "Hopefully, now that my @apprentice has taken over the shop, I'll be able to spend more time as a @bard."

[103 op=1 keyword; failure -> 105]
  1: appr,shop,cros

[104 op=2 display+exit]
  1: "My shop still sells the finest crossbows in the land."
  2: "I've done a lot over the years to refine the design, and I trained my apprentice well."
  3: "I'm sure she'll carry on the tradition in fine fashion."

[105 op=1 keyword; failure -> 107]
  1: bard

[106 op=2 display+exit]
  1: "Yes, it's always been a hobby of mine."
  2: "Have you heard the piece 'Stones' that I composed some years ago?"

[107 op=1 keyword; failure -> 109]
  1: wife,gwen

[108 op=2 display+exit]
  1: "She's had more time to develop her musical talents than I all these years."
  2: "But after this @quest I hope to join her in @Minoc and enter the artisan's @guild."

[109 op=1 keyword; failure -> 111]
  1: ques

[110 op=2 display+exit]
  1: "There always seems to be a quest when you come to visit us, doesn't there?"
  2: "Some time you should just drop by for @tea."

[111 op=1 keyword; failure -> 113]
  1: tea

[112 op=2 display+exit]
  1: "All right, make it Frasier's @Folly, then."

[113 op=1 keyword; failure -> 115]
  1: fras,foll

[114 op=2 display+exit]
  1: "A drink worthy enough to greet an old friend."

[115 op=1 keyword; failure -> 117]
  1: mino

[116 op=2 display+exit]
  1: "Not as big as @Britain, but it's a peaceful town."

[117 op=1 keyword; failure -> 119]
  1: arti,guil,musi

[118 op=2 display+exit]
  1: "Though there is still need for the instruments of war, as I grow in years I would now take up those of peace."

[119 op=1 keyword; failure -> 121]
  1: brit

[120 op=2 display+exit]
  1: "You know. Where Mr. @Nose hangs out."

[121 op=1 keyword; failure -> 123]
  1: nose,nibs

[122 op=2 display+exit]
  1: "Remember, he doesn't like people to call him that..."

[123 op=1 keyword; failure -> 125]
  1: garg

[124 op=2 display+exit]
  1: "They're pretty ugly looking, if you ask me..."

[125 op=1 keyword; failure -> 127]
  1: stor

[126 op=2 display+exit]
  1: "My favorite one is about the time I had to wake Dupre up for a tournament..."
  2: "But maybe I shouldn't tell you about that. After all, I did promise..."

[127 op=1 keyword; failure -> 129]
  1: join

[129 op=1 keyword; failure -> 131]
  1: leav

[131 op=1 keyword; failure -> 133]
  1: bye

[132 op=2 display+exit]
  1: "I've still got to tell you that story about Dupre sometime..."

[133 op=1 keyword; failure -> 139]
  1: book

[138 op=2 display+exit]
  1: "Certainly, take it $P. Perhaps thou canst make better sense of it than I."

[139 op=1 keyword; failure -> 141]
  1: book

[140 op=2 display+exit]
  1: "I no longer have the book, $P."

[141 op=2 display+exit]
  1: "Ask Shamino about that."

[142 op=1 keyword; failure -> 144]
  1: n

[143 op=2 display+exit]
  1: "Oh, you should get my wife @Gwenno to teach it to you some time."

[144 op=1 keyword; failure -> 146]
  1: help

[145 op=2 display+exit]
  1: "If you want me to @gather all our gold together so you can carry it, just say so."
  2: "Or I could @split it up evenly, if you prefer."

[146 op=1 keyword; failure -> 147]
  1: gath,pool

[147 op=1 keyword; failure -> 148]
  1: spli,shar

[148 op=2 display+exit]
  1: "His nibs, the king, told me I should stay with you constantly."
  2: "I'll make an exception when you have to relieve yourself, of course."

[151 op=2 display+exit]
  1: "Of course I'll further aid thy @quest."

[153 op=2 display+exit]
  1: "Your party is full!"

[155 op=2 display+exit]
  1: "I'm busy now, ask me again later."

[156 op=2 display+exit]
  1: "We saved you from those @gargoyles didn't we?"
  2: "Of course I'll further aid thy @quest."

[159 op=2 display+exit]
  1: "Ask Shamino about that."

[161 op=2 display+exit]
  1: "Don't leave me here!"

[162 op=2 display+exit]
  1: "His nibs, the king, told me I should stay with you constantly."
  2: "I'll make an exception until asked to @join again."

```

<a id="isabella-npc-61"></a>
## Isabella ? NPC 61, port 62

Instruction range: 5773?5824

```text
[5774 op=1 keyword; failure -> 5777]
  1: yes

[5776 op=2 display+exit]
  1: "It must have been wonderful, to drift amongst the very clouds..."

[5779 op=2 display+exit]
  1: "Ah, well..."
  2: "Some tales are but empty words, with less substance than the breath that carries them."

[5780 op=1 keyword; failure -> 5783]
  1: look

[5782 op=2 display+exit]
  1: a woman of elegant demeanor, wearing a finely embroidered dress.
  2: "Good $T, Avatar."
  3: "I'm glad thy travels have brought thee here again."

[5783 op=1 keyword; failure -> 5786]
  1: look

[5785 op=2 display+exit]
  1: a woman of elegant demeanor, wearing a finely embroidered dress.
  2: "Ah, $G $P, the famous Avatar!"
  3: "I have heard much of thy exploits, but never expected to meet you in person."
  4: "'Tis indeed an honor."

[5786 op=1 keyword; failure -> 5789]
  1: name

[5788 op=2 display+exit]
  1: "I am Lady Isabella."
  2: "Thy name, of course, is well @known throughout the land."

[5789 op=1 keyword; failure -> 5791]
  1: well,know,thro,land

[5790 op=2 display+exit]
  1: "The tales of thy exploits are known by even the smallest of children."
  2: "I recognized thee from the portrait @Woodroffe made the last time thou came to our land."

[5791 op=1 keyword; failure -> 5793]
  1: wood,pain,port

[5792 op=2 display+exit]
  1: "One of the finest artisans that ever dwelt in Minoc."
  2: "Alas, he died of the @flu two years ago."

[5793 op=1 keyword; failure -> 5795]
  1: die,flu

[5794 op=2 display+exit]
  1: "Aye, a pity, that."

[5795 op=1 keyword; failure -> 5797]
  1: job

[5796 op=2 display+exit]
  1: "'Tis my privilege to be mayor of Minoc, the city of @sacrifice."
  2: "We are renowned for our fine @craftsmen."

[5797 op=1 keyword; failure -> 5799]
  1: city,mino,reno,craf

[5798 op=2 display+exit]
  1: "The artisan's guild is located here."

[5799 op=1 keyword; failure -> 5801]
  1: sacr

[5800 op=2 display+exit]
  1: "We were entrusted with the @Rune of Sacrifice."

[5801 op=1 keyword; failure -> 5803]
  1: rune

[5802 op=2 display+exit]
  1: "I gave it to @Selganor, the @artisan's guildmaster."

[5803 op=1 keyword; failure -> 5805]
  1: arti,guil

[5804 op=2 display+exit]
  1: "When the Rune of Sacrifice was sent to our city, I entrusted it to their care."

[5805 op=1 keyword; failure -> 5807]
  1: selg

[5806 op=2 display+exit]
  1: "He's a fine @musician."
  2: "He lives just across the road from me, in the @guild hall."

[5807 op=1 keyword; failure -> 5809]
  1: musi

[5808 op=2 display+exit]
  1: "If I weren't so @busy, I'd take lessons from him myself."

[5809 op=1 keyword; failure -> 5811]
  1: busy,less

[5810 op=2 display+exit]
  1: "Aye, this is a big city."

[5811 op=1 keyword; failure -> 5813]
  1: mant

[5812 op=2 display+exit]
  1: "The good healer @Tara should be able to tell you the mantra."
  2: "Go ask at her house, on the north side of town."

[5813 op=1 keyword; failure -> 5815]
  1: heal,tara

[5814 op=2 display+exit]
  1: "Tara is a very compassionate woman."

[5815 op=1 keyword; failure -> 5817]
  1: garg

[5816 op=2 display+exit]
  1: "I've heard rumors of such creatures, but I know no one who has actually seen one."

[5817 op=1 keyword; failure -> 5820]
  1: ball,fly,airs,ligh

[5819 op=2 display+exit]
  1: "Ah, thou wouldst be interested to hear of that."
  2: "The balloonist flew off on an important @mission, and never returned."
  3: "Selganor should be able to tell you more about it."
  4: "Tell me, is it true you once rode in a balloon whilst on the quest of the Avatar?"

[5820 op=1 keyword; failure -> 5822]
  1: impo,miss

[5821 op=2 display+exit]
  1: "Yes, ask Selganor."

[5822 op=1 keyword; failure -> 5824]
  1: bye

[5823 op=2 display+exit]
  1: "Farewell, and please honor us with your presence again soon."

[5824 op=2 display+exit]
  1: "I cannot help thee with that."

```

<a id="jaana-npc-60"></a>
## Jaana ? NPC 60, port 61

Instruction range: 5702?5772

```text
[5703 op=1 keyword; failure -> 5706]
  1: yes

[5705 op=2 display+exit]
  1: "Ask me to @join and I will, gladly, $G!"

[5708 op=2 display+exit]
  1: "Very well."
  2: "Thou needst but ask..."

[5710 op=1 keyword; failure -> 5713]
  1: yes

[5712 op=2 display+exit]
  1: "Joyous news!"
  2: "Thou must tell me of it sometime."

[5715 op=2 display+exit]
  1: "Mayhap I could help."
  2: "Ask me to @join thy band, $G, and gladly I will!"

[5716 op=1 keyword; failure -> 5719]
  1: look

[5718 op=2 display+exit]
  1: the druidess Jaana.
  2: "I fear I know little that would help in this situation, $G."

[5719 op=1 keyword; failure -> 5724]
  1: look

[5723 op=2 display+exit]
  1: the druidess Jaana.
  2: "Good $T, $G."
  3: "Hast thou need of my services again?"

[5724 op=1 keyword; failure -> 5728]
  1: look

[5727 op=2 display+exit]
  1: the druidess Jaana. "Good $T, $G!"
  2: "Hast thou fared well in thy travels?"

[5728 op=1 keyword; failure -> 5731]
  1: look

[5730 op=2 display+exit]
  1: the druidess Jaana. "$P!"
  2: "'Tis a great joy and relief to see thou hast returned to Britannia, $G!"
  3: She greets your other traveling companions.
  4: Everyone seems pleased to be reunited, albeit under tense circumstances.
  5: "Mayhap I can help thee in thy struggle against the @gargoyles."
  6: "Thou needst but ask and I will gladly @join, $G."

[5731 op=1 keyword; failure -> 5733]
  1: name

[5732 op=2 display+exit]
  1: "Why, my name's Jaana, $G."
  2: "Surely my help in thy previous quests was not so insignificant that thou hast forgotten it!"

[5733 op=1 keyword; failure -> 5736]
  1: job

[5735 op=2 display+exit]
  1: "Since thy last visit to Britannia, I have lived in Yew, blessing crops and tending to sick animals."
  2: "Though I know 'tis unbecoming of a simple druid to do so, I miss the excitement of our quests together!"

[5736 op=1 keyword; failure -> 5738]
  1: job

[5737 op=2 display+exit]
  1: "Since thy last visit to Britannia, I have lived in Yew, blessing crops and tending to sick animals."
  2: "Though I know 'tis unbecoming of a simple druid to do so, I miss the excitement of our quests together!"
  3: "I would gladly @join thee, $G, if thou had a use for my skills."

[5738 op=1 keyword; failure -> 5741]
  1: join

[5740 op=2 display+exit]
  1: "Surely I misunderstand thee, $G."
  2: "Am I not already a member of thy band?"

[5741 op=1 keyword; failure -> 5743]
  1: join

[5743 op=1 keyword; failure -> 5745]
  1: leav

[5745 op=1 keyword; failure -> 5747]
  1: garg

[5746 op=2 display+exit]
  1: "I know little of the gargoyles, $G, save what rumors I hear."
  2: "But I know that the gargoyles are fierce fighters, and a great threat to the land!"

[5747 op=1 keyword; failure -> 5749]
  1: ench,nico

[5748 op=2 display+exit]
  1: "I've heard that Nicodemus the enchanter lives between two rivers."

[5749 op=1 keyword; failure -> 5752]
  1: bye

[5751 op=2 display+exit]
  1: "May the virtues guide us in our quest!"

[5752 op=1 keyword; failure -> 5754]
  1: bye

[5753 op=2 display+exit]
  1: "Farewell!"
  2: "Return when thou hast more news of thy quest!"

[5756 op=2 display+exit]
  1: "Alas, I know nothing of $Z, $G."

[5757 op=2 display+exit]
  1: "Beg pardon?"

[5760 op=2 display+exit]
  1: "Surely I misunderstand thee, $G."
  2: "Am I not already a member of thy band?"

[5762 op=2 display+exit]
  1: "Nay, $G, thou seemst to have enough companions as it is."
  2: "Mayhap if one of them left your band, thou might have more need of me."

[5764 op=2 display+exit]
  1: "Nay, $G, mayhap after I have finished aiding another."

[5766 op=2 display+exit]
  1: "Gladly, $G!"

[5769 op=2 display+exit]
  1: "I cannot leave thy party since I am not in it!"

[5771 op=2 display+exit]
  1: "Ask me again once we have returned to Yew."

[5772 op=2 display+exit]
  1: "Whatever thou thinkest best, $G."
  2: "Thou mayst keep my supplies."
  3: "Perhaps I misunderstand, $G."

```

<a id="james-npc-69"></a>
## James ? NPC 69, port 70

Instruction range: 6465?6745

```text
[6465 op=1 keyword; failure -> 6468]
  1: look

[6467 op=2 display+exit]
  1: a young lad.
  2: "A good $T to you, $G."
  3: "Is there more I can do for thee?"

[6468 op=1 keyword; failure -> 6470]
  1: look

[6469 op=2 display+exit]
  1: a young lad.
  2: "A good $T to you, $G."
  3: "What can I do for thee?"

[6470 op=1 keyword; failure -> 6472]
  1: buy

[6471 op=2 display+exit]
  1: "Be more specific, $G."
  2: "Me master 'as quite a large inventory."
  3: "Dost thou want @arms or @armor?"

[6472 op=1 keyword; failure -> 6474]
  1: job

[6473 op=2 display+exit]
  1: "I'm just the @apprentice, but if you want to @buy or @sell somethin' I can help ya."

[6474 op=1 keyword; failure -> 6476]
  1: appr

[6475 op=2 display+exit]
  1: "Me master went off to fight the gargoyles."
  2: "'E ain't been 'eard from since."
  3: "I'm just mindin' the place 'til 'e gets back."

[6476 op=1 keyword; failure -> 6479]
  1: name

[6478 op=2 display+exit]
  1: "Me name's James, $G."

[6479 op=1 keyword; failure -> 6481]
  1: bye

[6480 op=2 display+exit]
  1: "Good $T to ya, and I 'ope ye come back soon!"

[6483 op=1 keyword; failure -> 6485]
  1: arms

[6484 op=2 display+exit]
  1: "Which item, @Dagger, @Mace, @Main @Gauche, @Morning @star or @sword?"

[6485 op=1 keyword; failure -> 6487]
  1: armo

[6486 op=2 display+exit]
  1: "Which item, @Chain @Coif, @Chain @Mail, @Ring @Mail, @Scale @Mail or @Winged @Helm?"

[6487 op=1 keyword; failure -> 6489]
  1: sell

[6489 op=1 keyword; failure -> 6491]
  1: bulk

[6493 op=1 keyword; failure -> 6496]
  1: dagg

[6495 op=2 display+exit]
  1: "That dagger, costs 10 gold. How many?"

[6496 op=1 keyword; failure -> 6499]
  1: mace

[6498 op=2 display+exit]
  1: "That mace, costs 35 gold. How many?"

[6499 op=1 keyword; failure -> 6502]
  1: main,gauc

[6501 op=2 display+exit]
  1: "That main gauche 20 gold. How many?"

[6502 op=1 keyword; failure -> 6505]
  1: morn,star

[6504 op=2 display+exit]
  1: "That morning star, costs 40 gold. How many?"

[6505 op=1 keyword; failure -> 6508]
  1: swor

[6507 op=2 display+exit]
  1: "That sword, costs 35 gold. How many?"

[6508 op=1 keyword; failure -> 6512]
  1: chai

[6509 op=1 keyword; failure -> 6512]
  1: coif

[6511 op=2 display+exit]
  1: "That chain coif, costs 15 gold. How many?"

[6512 op=1 keyword; failure -> 6515]
  1: chai

[6514 op=2 display+exit]
  1: "That chain mail, costs 50 gold. How many?"

[6515 op=1 keyword; failure -> 6518]
  1: ring

[6517 op=2 display+exit]
  1: "That ring mail, costs 35 gold. How many?"

[6518 op=1 keyword; failure -> 6521]
  1: scal

[6520 op=2 display+exit]
  1: "That scale mail, costs 70 gold. How many?"

[6521 op=1 keyword; failure -> 6524]
  1: wing,helm

[6523 op=2 display+exit]
  1: "That winged mail, costs 20 gold. How many?"

[6584 op=2 display+exit]
  1: "Beg pardon, $G?"

[6585 op=1 keyword; failure -> 6587]
  1: sell

[6586 op=2 display+exit]
  1: "Come to me shop when it's open!"

[6587 op=1 keyword; failure -> 6589]
  1: arms

[6588 op=2 display+exit]
  1: "Come to me shop when it's open!"

[6589 op=1 keyword; failure -> 6591]
  1: armo

[6590 op=2 display+exit]
  1: "Come to me shop when it's open!"

[6591 op=1 keyword; failure -> 6592]
  1: buy "Come to me shop when it's open!"

[6592 op=2 display+exit]
  1: "Beg pardon, $G?"

[6595 op=2 display+exit]
  1: "Changed your mind, eh?"
  2: "What else can I do for thee?"

[6597 op=2 display+exit]
  1: "That is too many, $Y!."

[6601 op=2 display+exit]
  1: "But thou hast not enough gold, $Y!"

[6603 op=2 display+exit]
  1: "But 'twould make thee o'erburdened, $Y!"

[6604 op=2 display+exit]
  1: "Excellent!"

[6611 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 5 gold for that dagger?"

[6617 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 17 gold for that mace?"

[6623 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 10 gold for that main gauche?"

[6629 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 20 gold for that morning star?"

[6635 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 17 gold for that sword?"

[6641 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 7 gold for that chain coif?"

[6647 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 25 gold for that chain mail?"

[6653 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 17 gold for that ring mail?"

[6659 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 35 gold for that scale mail?"

[6665 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 10 gold for that winged helm? "

[6668 op=2 display+exit]
  1: "Sorry, none of you has anything I need."
  2: "What else can I do for thee?"

[6671 op=1 keyword; failure -> 6675]
  1: yes

[6674 op=2 display+exit]
  1: "Done!"
  2: He hands you 5 gold pieces and takes the dagger.

[6676 op=1 keyword; failure -> 6680]
  1: yes

[6679 op=2 display+exit]
  1: "Done!"
  2: He hands you 17 gold pieces and takes the mace.

[6681 op=1 keyword; failure -> 6685]
  1: yes

[6684 op=2 display+exit]
  1: "Done!"
  2: He hands you 10 gold pieces and takes the main gauche.

[6686 op=1 keyword; failure -> 6690]
  1: yes

[6689 op=2 display+exit]
  1: "Done!"
  2: He hands you 20 gold pieces and takes the morning star.

[6691 op=1 keyword; failure -> 6695]
  1: yes

[6694 op=2 display+exit]
  1: "Done!"
  2: He hands you 17 gold pieces and takes the sword.

[6696 op=1 keyword; failure -> 6700]
  1: yes

[6699 op=2 display+exit]
  1: "Done!"
  2: He hands you 7 gold pieces and takes the chain coif.

[6701 op=1 keyword; failure -> 6705]
  1: yes

[6704 op=2 display+exit]
  1: "Done!"
  2: He hands you 25 gold pieces and takes the chain mail.

[6706 op=1 keyword; failure -> 6710]
  1: yes

[6709 op=2 display+exit]
  1: "Done!"
  2: He hands you 17 gold pieces and takes the ring mail.

[6711 op=1 keyword; failure -> 6715]
  1: yes

[6714 op=2 display+exit]
  1: "Done!"
  2: He hands you 35 gold pieces and takes the scale mail.

[6716 op=1 keyword; failure -> 6720]
  1: yes

[6719 op=2 display+exit]
  1: "Done!"
  2: He hands you 10 gold pieces and takes the winged helm.

[6720 op=2 display+exit]
  1: "Perhaps some other time, then."
  2: "What else can I do for thee?"

[6734 op=2 display+exit]
  1: "Sorry, none of you has anything I be needin'."

[6736 op=2 display+exit]
  1: "Will you take $V0 gold for all o' your wares?"

[6738 op=1 keyword; failure -> 6740]
  1: yes

[6740 op=2 display+exit]
  1: "Perhaps some other time, then."
  2: "What else can I do for thee?"

[6743 op=2 display+exit]
  1: "But this ain't what we'd agreed, $G."
  2: "Sacrifice is a fine virtue, but it won't for long keep an armory in business."

[6745 op=2 display+exit]
  1: "Here's your gold, $G."
  2: "Me master'll be pleased to see such fine wares."
  3: He hands you $V0 gold pieces in exchange for the items.

```

<a id="jerris-npc-48"></a>
## Jerris ? NPC 48, port 49

Instruction range: 4304?4357

```text
[4312 op=2 display+exit]
  1: "Pleased to make your acquaintance."

[4314 op=1 keyword; failure -> 4316]
  1: look

[4315 op=2 display+exit]
  1: a slender man with boots that come up to his thighs.
  2: "Don't you know it's rude to interrupt a man when he's brawling?"

[4317 op=2 display+exit]
  1: "Don't you know it's rude to interrupt a man when he's brawling?"

[4319 op=1 keyword; failure -> 4322]
  1: look

[4321 op=2 display+exit]
  1: a slender man with boots that come up to his thighs.
  2: "Care to join me in a drink?"

[4322 op=1 keyword; failure -> 4324]
  1: look

[4323 op=2 display+exit]
  1: a slender man with boots that come up to his thighs.
  2: "Hello there, $G."

[4324 op=1 keyword; failure -> 4326]
  1: veri,mead,isle,mug

[4325 op=2 display+exit]
  1: "It's mostly made of honey. Ask a brewer."

[4326 op=1 keyword; failure -> 4330]
  1: name

[4329 op=2 display+exit]
  1: "Jerris is my name. And yourself?"

[4330 op=1 keyword; failure -> 4332]
  1: job

[4331 op=2 display+exit]
  1: "Some day I hope to join @Zellivan's Stalwarts and be a guard."

[4332 op=1 keyword; failure -> 4334]
  1: zell,stal,guar

[4333 op=2 display+exit]
  1: "When that last earthquake struck, I was pinned 'neath a great tree branch."
  2: "Lord Zellivan himself hoisted it up while a guard pulled me clear."

[4334 op=1 keyword; failure -> 4336]
  1: mous,rat

[4335 op=2 display+exit]
  1: "It was a rat, as sure as the world is flat."
  2: "Darndest thing, too."
  3: "Nomaan dropped the rune and the rat took it into a little @hole."

[4336 op=1 keyword; failure -> 4339]
  1: hole,rune

[4338 op=2 display+exit]
  1: "It's right there. On the north wall. See it?"

[4339 op=1 keyword; failure -> 4341]
  1: mant

[4340 op=2 display+exit]
  1: "It's in the refrain of the @song."

[4341 op=1 keyword; failure -> 4343]
  1: sing,song,refr

[4342 op=2 display+exit]
  1: "Well, I'd rather not sing alone. Get Culham to play it."

[4343 op=1 keyword; failure -> 4345]
  1: bye

[4344 op=2 display+exit]
  1: "Goodbye."

[4345 op=2 display+exit]
  1: "Sorry, $G.  I can't help ye with that."

[4347 op=1 keyword; failure -> 4349]
  1: yes

[4348 op=2 display+exit]
  1: "Obviously a rat hole."

[4350 op=22 display+continue]
  1: "Look carefully, $G. Ye will see it if ye keep on looking."

[4352 op=2 display+exit]
  1: Shamino points to a spot west of the bar where the wall meets the ground. "There it is."

[4353 op=2 display+exit]
  1: "Look carefully, $G. Ye will see it if ye keep on looking."

[4355 op=1 keyword; failure -> 4357]
  1: yes

[4356 op=2 display+exit]
  1: He hands you a mug for a swallow.
  2: It is warmed @mead, sweet and bubbly, with a cinnamon stick in it.
  3: "Comes from Verity Isle," he says.

[4357 op=2 display+exit]
  1: "Not a mead drinker, I guess." He shrugs.

```

<a id="johann-npc-115"></a>
## Johann ? NPC 115, port 116

Instruction range: 10838?10872

```text
[10838 op=1 keyword; failure -> 10840]
  1: look

[10839 op=2 display+exit]
  1: an uneasy looking bard.
  2: "Oh, hello."
  3: "Can I help you with something?"

[10840 op=1 keyword; failure -> 10842]
  1: name

[10841 op=2 display+exit]
  1: "They call me '@Yodeling Johann.'"

[10842 op=1 keyword; failure -> 10844]
  1: job

[10843 op=2 display+exit]
  1: "I sing @songs for the @pira - for the @sailors here."
  2: "But I'm looking for a chance to @move to another town."

[10844 op=1 keyword; failure -> 10846]
  1: sail

[10845 op=2 display+exit]
  1: "They're fine, hearty fellows, one and all."

[10846 op=1 keyword; failure -> 10848]
  1: yode

[10847 op=2 display+exit]
  1: "I'd better not..."
  2: "I've still got bruises from the last time I tried yodeling around here."

[10848 op=1 keyword; failure -> 10850]
  1: move

[10849 op=2 display+exit]
  1: "Not that there are @pirates around here or anything..."
  2: "The salty air just doesn't agree with me."

[10850 op=1 keyword; failure -> 10852]
  1: pira

[10851 op=2 display+exit]
  1: "Did I say pirates?"
  2: "I didn't say that."
  3: He looks around nervously.
  4: "Nobody but fine honest sailors around here."
  5: He lowers his voice.
  6: "By the way..."
  7: "If you should happen to run across Bonn, or Ybarra, or Hawkins, don't tell them I'm here."

[10852 op=1 keyword; failure -> 10854]
  1: bonn,ybar,hawk

[10853 op=2 display+exit]
  1: "Who's he?"
  2: "Never 'eard of 'im."

[10854 op=1 keyword; failure -> 10856]
  1: empi

[10855 op=2 display+exit]
  1: He spits on the ground and then says, "What?"

[10858 op=1 keyword; failure -> 10860]
  1: song

[10859 op=2 display+exit]
  1: "Oh, a loon with a spoon"
  2: "Sang a song of @ballooning."
  3: "And the man in the moon"
  4: "Said it was quite a tune."

[10861 op=1 keyword; failure -> 10863]
  1: song

[10862 op=2 display+exit]
  1: "Now one fine day in June"
  2: "A young man went @ballooning."
  3: "Did he rise up to glory"
  4: "Or fall to his doom?"

[10864 op=1 keyword; failure -> 10866]
  1: song

[10865 op=2 display+exit]
  1: "Just one thing will I say"
  2: "Of the sport of @ballooning:"
  3: "On the ground will I stay"
  4: "While the ground still has room!"

[10866 op=1 keyword; failure -> 10868]
  1: ball

[10867 op=2 display+exit]
  1: "Oh, that's just some nonsense that gave me an idea for a song..."
  2: "I read about it in a book at the @Lycaeum."

[10868 op=1 keyword; failure -> 10870]
  1: lyca,book

[10869 op=2 display+exit]
  1: "Ask the librarian there for help."

[10870 op=1 keyword; failure -> 10872]
  1: bye

[10871 op=2 display+exit]
  1: "Good day."

[10872 op=2 display+exit]
  1: "I don't think it would be a good idea to talk about that..."

```

<a id="julia-npc-65"></a>
## Julia ? NPC 65, port 66

Instruction range: 6121?6169

```text
[6122 op=1 keyword; failure -> 6124]
  1: yes

[6126 op=2 display+exit]
  1: "No? What are ye, daft? I canna be makin' pipes without wood!"
  2: "Go to Yew where they grow the best wood in Britannia."
  3: "Buy a freshly-cut yew log and take it to a sawmill."
  4: "Have 'em cut it into a board, and bring it back here."
  5: "Then I'll show ye how panpipes are made!"

[6127 op=1 keyword; failure -> 6130]
  1: look

[6129 op=2 display+exit]
  1: a sturdy woman with calloused hands and a loud manner.
  2: "Well met, Avatar."
  3: "An' what can I be doin' for ye this $T?"

[6130 op=1 keyword; failure -> 6133]
  1: look

[6132 op=2 display+exit]
  1: a sturdy woman with calloused hands and a loud manner.
  2: "Well, then the rumors are true!"
  3: "'Tis well and good that ye have returned to our land, Avatar!"

[6133 op=1 keyword; failure -> 6136]
  1: name

[6135 op=2 display+exit]
  1: "Sure and you're goin' daft, $G."
  2: "I be named Julia, same as the last time we met!"

[6136 op=1 keyword; failure -> 6139]
  1: name

[6138 op=2 display+exit]
  1: "Named Julia I am, $G, and pleased to meet ye."

[6139 op=1 keyword; failure -> 6141]
  1: job

[6140 op=2 display+exit]
  1: "Instruments I make, the finest in the land."
  2: "Not to be immodest, but I'm respected by all the @Guild!"
  3: "Ach, but give it all up I would for a life of @adventure like your own..."

[6141 op=1 keyword; failure -> 6143]
  1: guil

[6142 op=2 display+exit]
  1: "If ye would like to be a member too, go talk to @Selganor."

[6143 op=1 keyword; failure -> 6145]
  1: selg

[6144 op=2 display+exit]
  1: "He's the guildmaster."

[6145 op=1 keyword; failure -> 6147]
  1: pan,pipe

[6146 op=2 display+exit]
  1: "Sent you to me they did? Aye, I can believe that."
  2: "Now the panpipes are a tricky instrument."
  3: "You gotta have the finest wood."
  4: "If you want to learn how to make pipes, you'll need to bring a yew @board."
  5: "But it must be freshly @cut!"

[6147 op=1 keyword; failure -> 6149]
  1: fres,cut

[6148 op=2 display+exit]
  1: "Go to Yew where they grow the best wood in Britannia."
  2: "Buy a freshly-cut yew log and take it to a sawmill."
  3: "Have 'em cut it into a board, and bring it back here."
  4: "Then I'll show ye how panpipes are made!"

[6149 op=1 keyword; failure -> 6151]
  1: life,adve

[6150 op=2 display+exit]
  1: "Aye, I'd @join you if ye wish."

[6153 op=1 keyword; failure -> 6156]
  1: yew,boar

[6155 op=2 display+exit]
  1: "Do ye have a fine yew board for me this $T?"

[6156 op=1 keyword; failure -> 6158]
  1: bye

[6157 op=2 display+exit]
  1: "May fortune follow ye on thy way!"

[6160 op=2 display+exit]
  1: "Are ye daft? Why would I know anything about $Z?"

[6161 op=2 display+exit]
  1: "Nay, $G, I know nothing about $Z."

[6165 op=2 display+exit]
  1: "Well where is it? I canna see any board on ye!"
  2: "Go to Yew where they grow the best wood in Britannia."
  3: "Buy a freshly-cut yew log and take it to a sawmill."
  4: "Have 'em cut it into a board, and bring it back here."
  5: "Then I'll show ye how panpipes are made!"

[6169 op=2 display+exit]
  1: "Ach, aye, and good wood it is, too. A fine set of pipes this will make!"
  2: Julia sets the wood down on a bench and starts carving.
  3: As you watch, the rough board slowly turns into smooth wood rods.
  4: Then she hollows out each rod and pegs them all together.
  5: Lastly, she notches each tube and cuts them all to the proper lengths.
  6: "An' there it is, $G, a set of panpipes fit for any bard in the land!"
  7: She hands you the finished pipes.

```

<a id="justice-npc-193"></a>
## Justice ? NPC 193, port 194

Instruction range: 16600?16642

```text
[16604 op=1 keyword; failure -> 16606]
  1: look

[16605 op=2 display+exit]
  1: the altar of Justice.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about justice."
  4: Do you wish to meditate at this altar?

[16606 op=1 keyword; failure -> 16609]
  1: yes

[16608 op=2 display+exit]
  1: "Who will meditate?"

[16609 op=1 keyword; failure -> 16611]
  1: no

[16610 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16612 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable justice."
  3: Do you wish to meditate at this altar?

[16614 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16615 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about justice."
  3: Do you wish to meditate at this altar?

[16618 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16621 op=2 display+exit]
  1: "Speak the Mantra of Justice."

[16624 op=1 keyword; failure -> 16626]
  1: beh

[16627 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16628 op=2 display+exit]
  1: "You receive no enlightenment."

[16634 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16635 op=2 display+exit]
  1: "Justice without force is impotent."
  2: "Force without justice is tyranny."
  3: "There is naught else for you to learn here and now."
  4: "Return when thy journey has progressed further."

[16641 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16642 op=2 display+exit]
  1: "Justice without force is impotent."
  2: "Force without justice is tyranny."
  3: $PARTYMEMBER has gained a level...
  4: intelligence...
  5: and dexterity!

```

<a id="kador-npc-133"></a>
## Kador ? NPC 133, port 134

Instruction range: 12200?12295

```text
[12208 op=2 display+exit]
  1: The dog seems pleased.

[12211 op=2 display+exit]
  1: You don't have that much.
  2: How much do you give?

[12216 op=2 display+exit]
  1: After a while the dog sets down the bowl, lies down, and starts whimpering.

[12218 op=1 keyword; failure -> 12221]
  1: yes

[12220 op=2 display+exit]
  1: How much do you give?

[12222 op=1 keyword; failure -> 12226]
  1: no

[12225 op=2 display+exit]
  1: After a while the dog sets down the bowl, lies down, and starts whimpering.

[12228 op=1 keyword; failure -> 12231]
  1: look

[12230 op=2 display+exit]
  1: an amiable, shaggy mutt.
  2: The dog gets up and brings you his bowl again, looking at you expectantly.
  3: "Do you give him any money?

[12234 op=2 display+exit]
  1: The dog gets up and brings you his bowl again, looking at you expectantly.
  2: "Do you give him any money?

[12236 op=1 keyword; failure -> 12239]
  1: look

[12238 op=2 display+exit]
  1: an amiable, shaggy mutt.
  2: The dog picks up a bowl in his mouth and holds it up to you.
  3: You see that it has several coins in it already.
  4: "Do you give him any money?

[12241 op=2 display+exit]
  1: The dog picks up a bowl in his mouth and holds it up to you.
  2: You see that it has several coins in it already.
  3: "Do you give him any money?

[12244 op=1 keyword; failure -> 12246]
  1: look

[12245 op=2 display+exit]
  1: an amiable, shaggy mutt.
  2: "Arf!"

[12247 op=1 keyword; failure -> 12249]
  1: look

[12248 op=2 display+exit]
  1: an amiable, shaggy mutt.
  2: "Bow wow."

[12250 op=1 keyword; failure -> 12252]
  1: look

[12251 op=2 display+exit]
  1: an amiable, shaggy mutt.
  2: The dog jumps up and tries to lick your face.

[12252 op=1 keyword; failure -> 12254]
  1: look

[12253 op=2 display+exit]
  1: "Woof, woof!"

[12254 op=1 keyword; failure -> 12256]
  1: name,job,capt,john,silv,tabl,sell

[12255 op=2 display+exit]
  1: The dog looks at you with a puzzled expression on his face.

[12256 op=1 keyword; failure -> 12258]
  1: tric

[12257 op=2 display+exit]
  1: The dog looks at you expectantly.

[12258 op=1 keyword; failure -> 12261]
  1: sit

[12260 op=2 display+exit]
  1: The dog sits.

[12261 op=1 keyword; failure -> 12264]
  1: fetc

[12263 op=2 display+exit]
  1: The dog fetches a stick and lays it at your feet.

[12264 op=1 keyword; failure -> 12267]
  1: shak

[12266 op=2 display+exit]
  1: The dog shakes paws with you.

[12267 op=1 keyword; failure -> 12269]
  1: heel

[12268 op=2 display+exit]
  1: The dog bites your heel.

[12269 op=1 keyword; failure -> 12272]
  1: beg

[12271 op=2 display+exit]
  1: The dog begs.

[12272 op=1 keyword; failure -> 12275]
  1: play,dead

[12274 op=2 display+exit]
  1: The dog plays dead.

[12275 op=1 keyword; failure -> 12278]
  1: roll,over

[12277 op=2 display+exit]
  1: The dog rolls over.

[12278 op=1 keyword; failure -> 12281]
  1: spea

[12280 op=2 display+exit]
  1: "My name is Kador!"

[12281 op=1 keyword; failure -> 12284]
  1: advi

[12283 op=2 display+exit]
  1: "I feel that the whole debate over 'free will' versus 'determinism' arises from an artificial semantic division."
  2: "In light of the work of Godel and Heisenberg,"
  3: "it is evident that the two concepts are indistinguishable and thus,"
  4: "for all practical purposes, identical."

[12284 op=1 keyword; failure -> 12286]
  1: artu,wand,andr

[12285 op=2 display+exit]
  1: "Grrrrrrr..."

[12286 op=1 keyword; failure -> 12288]
  1: bye

[12287 op=2 display+exit]
  1: "Ruff."

[12290 op=2 display+exit]
  1: "Arf!"

[12292 op=2 display+exit]
  1: "Bow wow."

[12294 op=2 display+exit]
  1: The dog jumps up and tries to lick your face.

[12295 op=2 display+exit]
  1: "Woof, woof!"

```

<a id="karina-npc-132"></a>
## Karina ? NPC 132, port 133

Instruction range: 12102?12199

```text
[12105 op=1 keyword; failure -> 12108]
  1: yes

[12110 op=2 display+exit]
  1: "If you do, please come tell me."
  2: "I miss her so much..."

[12112 op=1 keyword; failure -> 12117]
  1: yes

[12116 op=2 display+exit]
  1: "Ask our dog, @Kador, to do some tricks for you."
  2: "He knows lots of them!"

[12118 op=1 keyword; failure -> 12121]
  1: yes

[12120 op=2 display+exit]
  1: "I'm sorry."
  2: "I'm not supposed to help you unless you pay."

[12123 op=2 display+exit]
  1: "It was my father's idea!"

[12127 op=1 keyword; failure -> 12129]
  1: look

[12128 op=2 display+exit]
  1: a gypsy girl poised on the verge of womanhood.
  2: "How could you be so mean to Kador!"
  3: "You go and apologize to my poor little dog this instant!"

[12130 op=2 display+exit]
  1: "How could you be so mean to Kador!"
  2: "You go and apologize to my poor little dog this instant!"

[12133 op=2 display+exit]
  1: She turns away from you.

[12134 op=1 keyword; failure -> 12136]
  1: look

[12135 op=2 display+exit]
  1: a gypsy girl poised on the verge of womanhood.

[12136 op=1 keyword; failure -> 12139]
  1: name

[12138 op=2 display+exit]
  1: "My name is Karina." she says, looking shyly at the ground.
  2: "Do you think it's a pretty name?"

[12139 op=1 keyword; failure -> 12142]
  1: advi

[12141 op=2 display+exit]
  1: "I have to ask you for a gold piece for my advice."
  2: "Is that okay?"

[12142 op=1 keyword; failure -> 12144]
  1: buy,sell

[12143 op=2 display+exit]
  1: "I have nothing to sell you."
  2: "Talk to my father."

[12144 op=1 keyword; failure -> 12146]
  1: job

[12145 op=2 display+exit]
  1: "@Father says I'm supposed to @dance for @gorgios."

[12146 op=1 keyword; failure -> 12148]
  1: gorg

[12147 op=2 display+exit]
  1: "That's what we call anyone who isn't a @gypsy."

[12148 op=1 keyword; failure -> 12150]
  1: gyps

[12149 op=2 display+exit]
  1: "It's a nice life, but it gets lonely."
  2: "I miss my @sister."

[12150 op=1 keyword; failure -> 12153]
  1: sist

[12152 op=2 display+exit]
  1: "Her name is Penumbra."
  2: "Have you any news of her?"

[12153 op=1 keyword; failure -> 12156]
  1: penu

[12155 op=2 display+exit]
  1: "Have you any news of her?"

[12156 op=1 keyword; failure -> 12158]
  1: fath,zolt

[12157 op=2 display+exit]
  1: "He is Zoltan, @king of the gypsies!"

[12158 op=1 keyword; failure -> 12160]
  1: king

[12159 op=2 display+exit]
  1: "Well, that's what he says."

[12160 op=1 keyword; failure -> 12162]
  1: capt,john,silv,tabl,lyca

[12161 op=2 display+exit]
  1: "Ask my father."

[12162 op=1 keyword; failure -> 12164]
  1: danc

[12163 op=2 display+exit]
  1: "Oh, I just couldn't, not in front of so many @people."

[12164 op=1 keyword; failure -> 12166]
  1: coul,fron,many,peop

[12165 op=2 display+exit]
  1: "I'd be too @embarassed!"

[12166 op=1 keyword; failure -> 12169]
  1: emba

[12168 op=2 display+exit]
  1: "Well... Do you promise not to make fun of me?"

[12169 op=1 keyword; failure -> 12171]
  1: dog,kado

[12170 op=2 display+exit]
  1: "He's such a sweet little doggie..."

[12171 op=1 keyword; failure -> 12173]
  1: bye

[12172 op=2 display+exit]
  1: "Good bye, and sweet dreams to you."

[12173 op=2 display+exit]
  1: "I can't help you with that."

[12178 op=1 keyword; failure -> 12181]
  1: yes

[12180 op=2 display+exit]
  1: "Thank you."

[12182 op=2 display+exit]
  1: "Oh."

[12189 op=2 display+exit]
  1: "Oh!"
  2: "I'm so glad to hear she's okay!"
  3: She throws her arms around you and kisses you on the cheek.

[12195 op=1 keyword; failure -> 12198]
  1: yes

[12197 op=2 display+exit]
  1: Suddenly she starts dancing a dance of haunting beauty,
  2: to the sound of music that seems to emanate from nowhere.
  3: It's one of the most wondrous sights you've ever seen.
  4: After a timeless time, the music ceases, and she turns and runs away.

[12199 op=2 display+exit]
  1: She turns away from you.

```

<a id="katrina-npc-95"></a>
## Katrina ? NPC 95, port 96

Instruction range: 8951?9025

```text
[8952 op=1 keyword; failure -> 8956]
  1: yes

[8955 op=2 display+exit]
  1: "Then surely we are saved already!"

[8959 op=2 display+exit]
  1: "Then perhaps the crisis is not so severe as rumor says."
  2: "If there is aught a humble peasant might do to aid thy quest, $G, I am yours to command."

[8961 op=1 keyword; failure -> 8964]
  1: yes

[8963 op=2 display+exit]
  1: "The land rejoices when all is well with the Avatar!"

[8966 op=2 display+exit]
  1: "If there is aught a humble peasant can do to help, I am yours to command."

[8968 op=1 keyword; failure -> 8971]
  1: yes

[8970 op=2 display+exit]
  1: "Command me to @join and I will, $G."

[8973 op=2 display+exit]
  1: "As thou deem best, $G."

[8974 op=1 keyword; failure -> 8977]
  1: look

[8976 op=2 display+exit]
  1: the humble peasant Katrina.
  2: "I'm but a simple peasant, $G."
  3: "I doubt I could help in this situation."

[8979 op=2 display+exit]
  1: the humble peasant Katrina.
  2: "Good $T, $G."
  3: "Wilt thou allow me to follow you again?"

[8981 op=1 keyword; failure -> 8984]
  1: look

[8983 op=2 display+exit]
  1: the humble peasant Katrina.
  2: "Good $T, $G!"
  3: "I hope thou hast fared well in thy travels?"

[8984 op=1 keyword; failure -> 8987]
  1: look

[8986 op=2 display+exit]
  1:  the humble peasant Katrina.
  2: "$P!"
  3: "I had heard that thou hadst returned to our land, but I dared not believe it!"
  4: "Hast thou returned to save our land from the @gargoyles?"

[8987 op=1 keyword; failure -> 8989]
  1: name

[8988 op=2 display+exit]
  1: "I am called Katrina, $G, as I was the last time we met."

[8989 op=1 keyword; failure -> 8992]
  1: job

[8991 op=2 display+exit]
  1: "Times have changed, $G."
  2: "Where once I grazed sheep, now I till the land."
  3: "All work is the same to a peasant, $G."
  4: "We all do what is expected of us."

[8992 op=1 keyword; failure -> 8994]
  1: job

[8993 op=2 display+exit]
  1: "Times have changed, $G."
  2: "Where once I grazed sheep, now I till the land."
  3: "All work is the same to a peasant, $G."
  4: "We all do what is expected of us."
  5: "If thou wouldst have me @join thy quest, thou needst but ask."

[8994 op=1 keyword; failure -> 8996]
  1: join

[8996 op=1 keyword; failure -> 8998]
  1: leav

[8998 op=1 keyword; failure -> 9000]
  1: garg

[8999 op=2 display+exit]
  1: "The gargoyles are a grave threat to the land, $G."
  2: "Many brave soldiers have died defending us from the invaders!"

[9000 op=1 keyword; failure -> 9002]
  1: humb,humi,most

[9001 op=2 display+exit]
  1: "I am most sorry, $G, but I cannot answer thy question."
  2: "'Twould be vain of me to answer truthfully, and 'twould be worse to lie."

[9002 op=1 keyword; failure -> 9005]
  1: bye

[9004 op=2 display+exit]
  1: "May we live up to the trust placed in us."

[9005 op=1 keyword; failure -> 9007]
  1: bye

[9006 op=2 display+exit]
  1: "Farewell!"
  2: "The Avatar carries with him the hopes of all the land."

[9009 op=2 display+exit]
  1: "A peasant such as myself knows little about $Z, $G."

[9010 op=2 display+exit]
  1: "Beg pardon, $G?"

[9013 op=2 display+exit]
  1: "Surely I misunderstand thee, $G."
  2: "Am I not already a member of thy band?"

[9015 op=2 display+exit]
  1: "I hesitate to deny any request thou might have."
  2: "But thou seemst to have enough companions as it is."
  3: "Mayhap if one of them left your band, thou might have more need of me."

[9017 op=2 display+exit]
  1: "I hesitate to deny any request thou might have."
  2: "But I am aiding another already."

[9018 op=2 display+exit]
  1: "Since thou dost think it wise, I wilt join."
  2: "I hope I can aid thy quest."

[9021 op=2 display+exit]
  1: "Beg pardon, $G?"

[9023 op=2 display+exit]
  1: "But $G, there is no place for me to go."

[9025 op=2 display+exit]
  1: "Whatever thou thinkest best, $G."
  2: "I'll leave my equipment with thee."

```

<a id="kenneth-npc-9"></a>
## Kenneth ? NPC 9, port 10

Instruction range: 777?822

```text
[777 op=1 keyword; failure -> 779]
  1: look

[778 op=2 display+exit]
  1: a lively bard with laughter in his eyes.
  2: "Welcome, $G! I am Sir Kenneth, and I'd gladly play a @song for thee."

[779 op=1 keyword; failure -> 782]
  1: name

[781 op=2 display+exit]
  1: "@Kenneth's my name - don't wear it out."

[782 op=1 keyword; failure -> 784]
  1: ken

[783 op=2 display+exit]
  1: "I asked you not to wear it out! It's the only name I've got."

[784 op=1 keyword; failure -> 786]
  1: job,teac,lear

[785 op=2 display+exit]
  1: "My @wife and I teach young bards the arts of composition, performance, and improvisation."
  2: "We also give @concerts on occasion."

[786 op=1 keyword; failure -> 788]
  1: perf,impr,compo,conc

[787 op=2 display+exit]
  1: "Bards must be well versed in such matters."

[788 op=1 keyword; failure -> 790]
  1: wife

[789 op=2 display+exit]
  1: "Her name is Nan. Whatever you do, don't say anything to her about @spiders..."

[790 op=1 keyword; failure -> 792]
  1: nan,spid

[791 op=2 display+exit]
  1: "She has quite an @imagination. Sometimes she lets it carry her away."

[792 op=1 keyword; failure -> 794]
  1: imag

[793 op=2 display+exit]
  1: "Usually it doesn't give her any trouble."

[794 op=1 keyword; failure -> 796]
  1: comp

[795 op=2 display+exit]
  1: "Yes, I know of the @Rune and @Mantra of Compassion."

[796 op=1 keyword; failure -> 798]
  1: mant

[797 op=2 display+exit]
  1: "I've been trying to work the Mantra of Compassion into a song - but I don't know what '@mu' means."

[798 op=1 keyword; failure -> 801]
  1: mu

[800 op=2 display+exit]
  1: "Dost thou know what it means?"

[802 op=1 keyword; failure -> 805]
  1: yes

[804 op=2 display+exit]
  1: "Tell me, what does it mean?"

[806 op=1 keyword; failure -> 809]
  1: no

[808 op=2 display+exit]
  1: "Nobody I know does either."

[813 op=2 display+exit]
  1: "Hmmmm... No, I don't think that's it."

[814 op=1 keyword; failure -> 816]
  1: rune

[815 op=2 display+exit]
  1: "@Ariana has the rune, and they could hardly have found a sweeter child to give it to."

[816 op=1 keyword; failure -> 818]
  1: aria,swee,chil

[817 op=2 display+exit]
  1: "Once in a great while a student comes along with such a gift for learning that one need but offer a little guidance and step back to watch them learn."
  2: "To have such students is the most rewarding part of a teacher's job, and Ariana is one such."

[818 op=1 keyword; failure -> 820]
  1: song,music

[819 op=2 display+exit]
  1: Kenneth plays a delightful little melody for you.

[820 op=1 keyword; failure -> 822]
  1: bye

[821 op=2 display+exit]
  1: "Until we meet again."

[822 op=2 display+exit]
  1: "I like the confused look on your face too much to ruin it by telling you."

```

<a id="koranada-npc-157"></a>
## Koranada ? NPC 157, port 158

Instruction range: 14390?14463

```text
[14394 op=2 display+exit]
  1: "Well done, I see thy Serpent Shield is finished."
  2: "I welcome thee into the @Order of the Silver Serpent."

[14402 op=2 display+exit]
  1: "Well met, $G Avatar!"

[14404 op=1 keyword; failure -> 14408]
  1: yes

[14407 op=2 display+exit]
  1: "We would be greatly honored if thou wouldst join us, Avatar."
  2: "Thy deeds speak well of thee, $P."
  3: "I shall waive the usual requirements since thy adventures are more than sufficient proof of thy courage."
  4: "However, thou must still create thine own @shield."

[14410 op=2 display+exit]
  1: "There will always be a place here for thee, $G."

[14411 op=1 keyword; failure -> 14414]
  1: look

[14413 op=2 display+exit]
  1: a dignified man wearing a loose jacket decorated with a serpent.
  2: "It is good to see thee again, $P."

[14414 op=1 keyword; failure -> 14418]
  1: look

[14417 op=2 display+exit]
  1: a dignified man wearing a loose jacket decorated with a serpent.
  2: "Hello, adventurer!"
  3: "As guildmaster, I welcome thee in the name of the Order of the Silver Serpent."
  4: "What is thy name?"

[14418 op=1 keyword; failure -> 14420]
  1: name

[14419 op=2 display+exit]
  1: "I am Koranada, son of Kulebra."
  2: "My father was a brave @lighthouse keeper."

[14420 op=1 keyword; failure -> 14422]
  1: job,ligh

[14421 op=2 display+exit]
  1: "An earthquake destroyed the lighthouse, else I would have inherited the position of keeper in Greyhaven."
  2: "I traveled to Serpent's Hold to join the @Order instead."

[14422 op=1 keyword; failure -> 14425]
  1: orde,hold,guil

[14424 op=2 display+exit]
  1: "We stand for the principle of @Courage."
  2: "We are honored by thy membership, worthy Avatar!"

[14425 op=1 keyword; failure -> 14428]
  1: orde,hold,guil

[14427 op=2 display+exit]
  1: "We stand for the principle of @Courage."
  2: "Acquire a Silver Serpent @shield from @Gherick and thou wilt be admitted to the order, $G $P."

[14428 op=1 keyword; failure -> 14431]
  1: orde,hold,guil

[14430 op=2 display+exit]
  1: "We stand for the principle of @Courage."
  2: "Dost thou wish to join the Order of the Silver Serpent?"

[14431 op=1 keyword; failure -> 14433]
  1: cour,brav

[14432 op=2 display+exit]
  1: "Go and speak to de Hugh."
  2: "Observe what he believes to be courage."
  3: "Though he is a fine fighter, he has yet to prove himself worthy of our @order."

[14433 op=1 keyword; failure -> 14436]
  1: shie

[14435 op=2 display+exit]
  1: "The Serpent Shield of our order is a sacred symbol of our brotherhood of bravery."
  2: "Carry it with pride!"

[14436 op=1 keyword; failure -> 14439]
  1: shie

[14438 op=2 display+exit]
  1: "The Serpent Shield of our order is a sacred symbol of our brotherhood of bravery."
  2: "Its making is a magical @process."
  3: "@Gherick the Smithy will help thee make one."

[14439 op=1 keyword; failure -> 14441]
  1: shie

[14440 op=2 display+exit]
  1: "The Serpent Shield of our order is a sacred symbol of our brotherhood of bravery."

[14441 op=1 keyword; failure -> 14443]
  1: magi,proc

[14442 op=2 display+exit]
  1: "'Tis secret, as well."

[14443 op=1 keyword; failure -> 14445]
  1: gher

[14444 op=2 display+exit]
  1: "You will most likely find him at Siege Crafters."

[14445 op=1 keyword; failure -> 14448]
  1: join

[14447 op=2 display+exit]
  1: "I have responsibilities here, $G."
  2: "But I am honored by the offer."

[14448 op=1 keyword; failure -> 14451]
  1: join

[14450 op=2 display+exit]
  1: "I have responsibilities here, $G."
  2: "However..."
  3: "Dost thou wish to join the Order of the Silver Serpent?"

[14451 op=1 keyword; failure -> 14453]
  1: rune

[14452 op=2 display+exit]
  1: "Without the meaning, such a symbol is just a meaningless scratching."
  2: "We @meditate on valour here."

[14453 op=1 keyword; failure -> 14455]
  1: mant,medi

[14454 op=2 display+exit]
  1: "A mantra is a concept, a special word to be turned over and over in the mind."
  2: "When we meditate on a mantra, we do so in order to find the @meaning of that virtue."

[14455 op=1 keyword; failure -> 14457]
  1: mean

[14456 op=2 display+exit]
  1: "It is not enough just to know a mantra or possess a rune."
  2: "In order to be @virtuous, one must act virtuously."

[14457 op=1 keyword; failure -> 14459]
  1: virt,avat

[14458 op=2 display+exit]
  1: "Sometimes one virtue conflicts with another,
  2: though both be numbered amongst the virtues of the Avatar."
  3: "The hero seeks to act with an awareness of these conflicts instead of merely following instinct."
  4: "My teacher @Conor taught me this."

[14459 op=1 keyword; failure -> 14461]
  1: cono

[14460 op=2 display+exit]
  1: "A dear friend."
  2: "He was guildmaster here when I was in training."
  3: "He lives humbly in New Magincia now."

[14461 op=1 keyword; failure -> 14463]
  1: bye

[14462 op=2 display+exit]
  1: "Fare thee well."

[14463 op=2 display+exit]
  1: "That I cannot help thee with."

```

<a id="kytyn-npc-29"></a>
## Kytyn ? NPC 29, port 30

Instruction range: 2490?2539

```text
[2490 op=1 keyword; failure -> 2492]
  1: look

[2491 op=2 display+exit]
  1: a slender woman wearing a leather glove on her left hand.
  2: "Hello and welcome, $G!"

[2492 op=1 keyword; failure -> 2495]
  1: name

[2494 op=2 display+exit]
  1: "My name is Kytyn." She smiles. "Perhaps that explains why I like @birds."

[2495 op=1 keyword; failure -> 2497]
  1: glov,bird

[2496 op=2 display+exit]
  1: "I used to be a @falconer."

[2497 op=1 keyword; failure -> 2499]
  1: falc

[2498 op=2 display+exit]
  1: "That was before Lord @British appointed me to my current position."

[2499 op=1 keyword; failure -> 2501]
  1: lord,brit

[2500 op=2 display+exit]
  1: "A true gentleman if ever I saw one."

[2501 op=1 keyword; failure -> 2503]
  1: job,appo,posi

[2502 op=2 display+exit]
  1: "I'm the curator of the royal @museum of @oddities."

[2503 op=1 keyword; failure -> 2505]
  1: roya,muse

[2504 op=2 display+exit]
  1: "It's just one of the @projects Lord British has funded to provide a better life for his people."

[2505 op=1 keyword; failure -> 2507]
  1: proj,fund,bett,life

[2506 op=2 display+exit]
  1: "He also established the @conservatory that shares the building with the museum."
  2: "The @roads that span the realm are his doing as well."

[2507 op=1 keyword; failure -> 2509]
  1: cons,shar,buil

[2508 op=2 display+exit]
  1: "Sometimes I can hear their @music drifting through the walls..."
  2: "Usually that noisy, crackling field drowns it out, though."

[2509 op=1 keyword; failure -> 2511]
  1: musi,drif,hear

[2510 op=2 display+exit]
  1: "It's very pretty."

[2511 op=1 keyword; failure -> 2513]
  1: road,real,netw

[2512 op=2 display+exit]
  1: "@Travel is much easier these days."

[2513 op=1 keyword; failure -> 2515]
  1: trav

[2514 op=2 display+exit]
  1: "That's for sure."

[2515 op=1 keyword; failure -> 2517]
  1: cura,odd

[2516 op=2 display+exit]
  1: "I can tell you more about any of the @exhibits you're interested in."

[2517 op=1 keyword; failure -> 2519]
  1: exhi,inte

[2518 op=2 display+exit]
  1: "We have the @monolith, the @energy field, the perpetual motion @machine, the @bones of Zog, the mystery @fountain, and the @dragon's head, wings, and egg."
  2: "Which one were you curious about?"

[2519 op=1 keyword; failure -> 2521]
  1: bone,zog

[2520 op=2 display+exit]
  1: "This skeleton seems to be about ten times older than any other ever found in Britannia."
  2: "Surely it has much to tell us about our origins, if we only knew what it meant!"

[2521 op=1 keyword; failure -> 2523]
  1: drag,head,wing,egg

[2522 op=2 display+exit]
  1: "Someone named '@Gertan' donated all those parts."
  2: "I think he said something about a dragon named 'Freitag'."

[2523 op=1 keyword; failure -> 2525]
  1: gert,frei

[2524 op=2 display+exit]
  1: "If you want to ask Gertan, I think he's in Cove."

[2525 op=1 keyword; failure -> 2527]
  1: myst,foun

[2526 op=2 display+exit]
  1: "Somebody found a huge boulder with a jet of water spraying out of it."
  2: "We had it magically teleported here, buried with its surface flush with the ground, and then we built this building on top of it."
  3: "Of course we left a hole in the floor for the water to spray through."
  4: "I often drink from it when I'm thirsty. The water is quite good, but there's something strange about it."
  5: "It vanishes somehow before it hits the ground..."

[2527 op=1 keyword; failure -> 2529]
  1: perp,moti,mach,gear

[2528 op=2 display+exit]
  1: "That's a very curious device. The motion of each gear turns another, until eventually the movement is propagated back to the same gear again."
  2: "There's one thing I can't figure out."
  3: "We've made plans of the device, and I can't see how they started it moving in the first place."
  4: "None of the gears would turn unless all the others were already going."
  5: "What's worse, the motions serve to hold the machine together."
  6: "If the gears were still, the whole thing would fall apart! How it was built is quite a mystery."

[2529 op=1 keyword; failure -> 2531]
  1: ener,fiel,nois,crac

[2530 op=2 display+exit]
  1: "The field is generated by a curious spell."
  2: "By some process not yet understood, it converts magical energy into electrical energy, producing a continuous discharge."
  3: "If we understood this process better, it would probably lead to all sorts of useful applications."

[2531 op=1 keyword; failure -> 2533]
  1: mono

[2532 op=2 display+exit]
  1: "I can't tell you much about that. Lord British donated it to the museum, but I can't get him to talk about it."
  2: "From what little he's said, I think it's an artifact from his home world."

[2533 op=1 keyword; failure -> 2535]
  1: libr,lyca

[2534 op=2 display+exit]
  1: "There is a great deal of knowledge available there for the serious scholar."

[2535 op=1 keyword; failure -> 2537]
  1: bye

[2536 op=2 display+exit]
  1: "Be sure to have a look at our paintings while you're here. And come again soon..."
  2: "We're expecting some unicorn horns, and a Klein bottle from Trinsic."

[2537 op=1 keyword; failure -> 2539]
  1: purr

[2538 op=2 display+exit]
  1: She favors you with a deep, throaty purr that sounds just like a cat.

[2539 op=2 display+exit]
  1: "I can't help you with that. Try the @library at the Lycaeum."

```

<a id="lawrence-npc-75"></a>
## Lawrence ? NPC 75, port 76

Instruction range: 7053?7130

```text
[7054 op=1 keyword; failure -> 7056]
  1: yes

[7058 op=2 display+exit]
  1: "Anything else I can do for you?"

[7061 op=1 keyword; failure -> 7064]
  1: look

[7063 op=2 display+exit]
  1: a small, fussy looking man.
  2: "Welcome to the Fool's Pair o' Dice."
  3: "Anything else I can do for you?"

[7064 op=1 keyword; failure -> 7067]
  1: look

[7066 op=2 display+exit]
  1: a small, fussy looking man.
  2: "Welcome to the Fool's Pair o' Dice."

[7067 op=1 keyword; failure -> 7069]
  1: name

[7068 op=2 display+exit]
  1: "It's Lawrence."

[7069 op=1 keyword; failure -> 7071]
  1: buy

[7070 op=2 display+exit]
  1: "Which, @grapes, @ale, @mead, @wine, or @rations?"

[7071 op=1 keyword; failure -> 7073]
  1: job

[7072 op=2 display+exit]
  1: "I sell @grapes, @ale, @mead, @wine, and @rations."

[7073 op=1 keyword; failure -> 7078]
  1: grap

[7077 op=2 display+exit]
  1: "That'll be 3 gold, okay?"

[7078 op=1 keyword; failure -> 7083]
  1: mead

[7082 op=2 display+exit]
  1: "That's 5 gold, okay?"

[7083 op=1 keyword; failure -> 7088]
  1: ale

[7087 op=2 display+exit]
  1: "That's 2 gold, okay?"

[7088 op=1 keyword; failure -> 7093]
  1: wine

[7092 op=2 display+exit]
  1: "That's 5 gold, okay?"

[7093 op=1 keyword; failure -> 7096]
  1: rati

[7095 op=2 display+exit]
  1: "Rations are 4 gold apiece."
  2: "How many do you want?"

[7096 op=1 keyword; failure -> 7098]
  1: no,bye

[7097 op=2 display+exit]
  1: "Goodbye."

[7098 op=2 display+exit]
  1: "I can't help you with that."

[7102 op=2 display+exit]
  1: "I'm afraid you haven't enough money for it."

[7105 op=2 display+exit]
  1: "You don't have room to carry them."

[7107 op=2 display+exit]
  1: "You don't have room to carry it."

[7109 op=2 display+exit]
  1: He gives you some mead.
  2: "That ought to hold you."
  3: "Anything else I can do for you?"

[7111 op=2 display+exit]
  1: He gives you some ale.
  2: "That ought to hold you."
  3: "Anything else I can do for you?"

[7113 op=2 display+exit]
  1: He gives you some wine.
  2: "That ought to hold you."
  3: "Anything else I can do for you?"

[7114 op=2 display+exit]
  1: He gives you some grapes.
  2: "Anything else I can do for you?"

[7117 op=2 display+exit]
  1: "Anything else I can do for you?"

[7119 op=2 display+exit]
  1: He looks at you.
  2: "You haven't any room in your pack."

[7124 op=2 display+exit]
  1: "Come back when you have some gold."

[7126 op=2 display+exit]
  1: He looks at you.
  2: "You haven't any room in your pack."

[7129 op=2 display+exit]
  1: He hands you a ration of mutton.
  2: "Anything else I can do for you?"

[7130 op=2 display+exit]
  1: He hands you $V0 rations of mutton.
  2: "Anything else I can do for you?"

```

<a id="lazeena-npc-17"></a>
## Lazeena ? NPC 17, port 18

Instruction range: 1553?1572

```text
[1553 op=1 keyword; failure -> 1555]
  1: look

[1554 op=2 display+exit]
  1: a sad lady with hair down to her ankles.
  2: A soft jingling accompanies her silent walk.
  3: Rather than talking, she sings in a voice like wind through reeds.
  4: "Hail, there, $G; come listen to me. I've plenty of time to sing unto thee."

[1555 op=1 keyword; failure -> 1558]
  1: name

[1557 op=2 display+exit]
  1: "Lazeena they call me, Lazeena the @Dove."
  2: "From Cove I did come, 'tis a town full of love."

[1558 op=1 keyword; failure -> 1560]
  1: sing,job,love,dove

[1559 op=2 display+exit]
  1: "'Tis the aim of my life to sing like a bird."
  2: "Wouldst thou like to hear the news I've heard?"

[1560 op=1 keyword; failure -> 1562]
  1: rhym,shor

[1561 op=2 display+exit]
  1: She sings.
  2: "South of Loch Lake and north of the bay, 
  3: the beautiful Shrine of Compassion doth lay."
  4: "My @Artagel set out to chant there one day."
  5: "But still he has not returned."
  6: "Was he taken by giants or headless so foul, 
  7: that nary a man could contend with the smell."
  8: "He should have come home weeks ago now."
  9: "But still he has not returned."

[1562 op=1 keyword; failure -> 1564]
  1: arta

[1563 op=2 display+exit]
  1: "He is a healer, you see."

[1564 op=1 keyword; failure -> 1566]
  1: song,sea

[1565 op=2 display+exit]
  1: She sings.
  2: "The Dutchman was a giant ship with masts from the largest trees."
  3: "The pride of the people she was hailed, the ruler of all the sea."
  4: "She sailed out of port one fine spring day, and headed for distant shores."
  5: "But a britney caught her by surprise, 'twas flung against her core."
  6: "The match began the grim men fought, to gather in her sails."
  7: "But thunderous was the crack and lo, the mighty vessel reeled."
  8: "When gentle winds danced 'cross the surf, again the followed morn."
  9: "'twas nothing left of Dutchy's hull but remnants of her born."
  10: "And now the tale has nothing left, but a gravesight on the floor."
  11: "On the Fens o' the Dead she came to rest, ne'er to sail no more."

[1566 op=1 keyword; failure -> 1568]
  1: yes

[1567 op=2 display+exit]
  1: "Then tell me what would please thee more: 
  2: a @song of the sea or a @rhyme of the shore?"

[1568 op=1 keyword; failure -> 1570]
  1: no

[1569 op=2 display+exit]
  1: "To be of more help, I wish I knew just what this little bird could do."

[1570 op=1 keyword; failure -> 1572]
  1: bye

[1571 op=2 display+exit]
  1: "Farewell, my friend."

[1572 op=2 display+exit]
  1: "I cannot help thee with that."

```

<a id="le-nard-npc-53"></a>
## Le'nard ? NPC 53, port 54

Instruction range: 4593?4702

```text
[4597 op=1 keyword; failure -> 4600]
  1: yes

[4599 op=2 display+exit]
  1: "Oh, sorry."
  2: "I forgot to ask how many spools you'd like to sell?"

[4602 op=2 display+exit]
  1: "Oh."
  2: "Okay."

[4604 op=1 keyword; failure -> 4606]
  1: yes

[4607 op=1 keyword; failure -> 4612]
  1: no

[4611 op=2 display+exit]
  1: "I'm sorry!"
  2: "That's too expensive, isn't it?"
  3: "Here, I'll sell it for $V0 gold, okay?"

[4613 op=1 keyword; failure -> 4618]
  1: no

[4617 op=2 display+exit]
  1: "I'm sorry!"
  2: "That's still too expensive, isn't it?"
  3: "Here, I'll sell it for $V0 gold, okay?"

[4619 op=1 keyword; failure -> 4622]
  1: no

[4621 op=2 display+exit]
  1: "I'm terribly sorry, but I can't sell it for less!"

[4624 op=2 display+exit]
  1: "Oh, sorry."
  2: "I thought thou wanted to buy something."

[4625 op=1 keyword; failure -> 4628]
  1: look

[4627 op=2 display+exit]
  1: a short, fat, rosy-cheeked man with long hair covering his eyes.
  2: "He--hello, $G."
  3: "Nice to see thee again."

[4628 op=1 keyword; failure -> 4631]
  1: look

[4630 op=2 display+exit]
  1: a short, fat, rosy-cheeked man with long hair covering his eyes.
  2: The man seems afraid of you.
  3: "Wh-what do you want?"

[4631 op=1 keyword; failure -> 4634]
  1: name

[4633 op=2 display+exit]
  1: As faint as a whisper comes the word "Le'nard."

[4634 op=1 keyword; failure -> 4636]
  1: job

[4635 op=2 display+exit]
  1: "I'm a @tailor, $G."

[4636 op=1 keyword; failure -> 4638]
  1: bye

[4637 op=2 display+exit]
  1: He looks around, then waves goodbye.

[4638 op=1 keyword; failure -> 4648]
  1: sell,thre

[4648 op=1 keyword; failure -> 4651]
  1: sell,thre

[4650 op=2 display+exit]
  1: He turns to you and says "Will you take $V1 gold for each spool of thread?"

[4651 op=1 keyword; failure -> 4653]
  1: buy,tail

[4652 op=2 display+exit]
  1: "I sew @pants, @tunics and @dresses, $G."
  2: "I use a lot of @thread."

[4653 op=1 keyword; failure -> 4658]
  1: pant

[4657 op=2 display+exit]
  1: "They cost 30 gold coins."
  2: "Do you still want them?"

[4658 op=1 keyword; failure -> 4663]
  1: tuni

[4662 op=2 display+exit]
  1: "That costs 25 gold coins."
  2: "Do you still want it?"

[4663 op=1 keyword; failure -> 4668]
  1: dres

[4667 op=2 display+exit]
  1: "That costs 45 gold coins."
  2: "Do you still want it?"

[4668 op=1 keyword; failure -> 4671]
  1: ball,plan,silk,bag

[4670 op=2 display+exit]
  1: "I wouldn't know where to begin."
  2: "Ask @Marissa in Paws."

[4671 op=1 keyword; failure -> 4673]
  1: ball,plan,silk,bag

[4672 op=2 display+exit]
  1: "I'm not sure what you mean."

[4673 op=2 display+exit]
  1: He stands still for a moment and then shrugs.

[4677 op=2 display+exit]
  1: "I'm sorry, but I really don't think you can afford that."

[4679 op=2 display+exit]
  1: "I'm sorry, but I really don't think you can carry that."

[4680 op=2 display+exit]
  1: He takes your gold and hands over the purchase.

[4683 op=2 display+exit]
  1: "Oh."
  2: "Okay."

[4685 op=2 display+exit]
  1: "I'm sorry, but I really don't need more than 10 right now."

[4699 op=2 display+exit]
  1: "But - but none of you has that much thread to sell."

[4702 op=2 display+exit]
  1: "Good, good."
  2: He hands you $V1 gold pieces and takes the thread.

```

<a id="lenora-npc-57"></a>
## Lenora ? NPC 57, port 58

Instruction range: 5426?5490

```text
[5427 op=1 keyword; failure -> 5431]
  1: look

[5430 op=2 display+exit]
  1: "Hmpf!"
  2: "Well, mayhap 'twould be best for you to seek such wisdom elsewhere!"
  3: "Thou may return when thou seest fit to answer my question!"

[5434 op=2 display+exit]
  1: "Hmmm..."
  2: "well, no matter," she says suspiciously.
  3: "I suppose 'twould be improper to deny anything the Avatar requires."
  4: "The Mantra of Justice is 'beh.'"

[5437 op=1 keyword; failure -> 5440]
  1: look

[5439 op=2 display+exit]
  1: a tall, stern woman, with graying red hair and a perpetual scowl.
  2: "Well, Avatar."
  3: "I graciously grant thee another interview."
  4: "I certainly hope thou wilt be more respectful this time."

[5440 op=1 keyword; failure -> 5443]
  1: look

[5442 op=2 display+exit]
  1: a tall, stern woman, with graying red hair and a perpetual scowl.
  2: "Welcome again, worthy Avatar, to my hall."
  3: "How may we assist thee in thy quest?"

[5443 op=1 keyword; failure -> 5446]
  1: look

[5445 op=2 display+exit]
  1: a tall, stern woman, with graying red hair and a perpetual scowl.
  2: "Welcome, worthy Avatar, to the Hall of Justice."
  3: "How may we assist thee in thy quest?"

[5446 op=1 keyword; failure -> 5448]
  1: name

[5447 op=2 display+exit]
  1: "I am Lenora, Lady Mayor of Yew."

[5448 op=1 keyword; failure -> 5450]
  1: job

[5449 op=2 display+exit]
  1: "It is my honor to preside over this town and its @courts."
  2: "We also provide the finest @wood in the realm."

[5450 op=1 keyword; failure -> 5452]
  1: wood,log

[5451 op=2 display+exit]
  1: "Go ask Ben the logger."
  2: "Head west from here into the forest until you can go no further."
  3: "Then look for a way through the trees to the north."

[5452 op=1 keyword; failure -> 5454]
  1: cour

[5453 op=2 display+exit]
  1: "In Yew we take pride in our system of @justice."

[5454 op=1 keyword; failure -> 5456]
  1: just

[5455 op=2 display+exit]
  1: "Indeed, the @Rune of Justice itself was sent here for safekeeping."

[5456 op=1 keyword; failure -> 5458]
  1: rune

[5457 op=2 display+exit]
  1: "A thief stole the rune from the grave of our most respected resident,
  2: the former Lord Mayor."
  3: "Even though the @thief was caught, we have not yet found the rune."

[5458 op=1 keyword; failure -> 5460]
  1: thie,caug,wher

[5459 op=2 display+exit]
  1: "The thief is in the jail, awaiting the carriage of justice."

[5460 op=1 keyword; failure -> 5463]
  1: mant

[5462 op=2 display+exit]
  1: "'Tis most strange, Avatar, that thou knowest not the Mantra of Justice."
  2: "How can that be?"

[5463 op=1 keyword; failure -> 5466]
  1: lett,perm

[5465 op=2 display+exit]
  1: "A letter of permission?"
  2: "Dost thou truly need to speak with that reprehensible thief?"

[5466 op=1 keyword; failure -> 5468]
  1: rele,deal,let,free,trad,swap,allo,go,out

[5467 op=2 display+exit]
  1: "What, free the thief in return for the rune?"
  2: "Never!"
  3: "To do so would betray everything the rune itself stands for!"
  4: "Better to leave it lost."

[5468 op=1 keyword; failure -> 5470]
  1: kid,chil,fami,feed

[5469 op=2 display+exit]
  1: "What?"
  2: "What stories has that thief been telling you?"
  3: "He has no family--he's just a drifter!"
  4: "Don't be taken in by his @lies."

[5470 op=1 keyword; failure -> 5472]
  1: lie

[5471 op=2 display+exit]
  1: "Tell him he's not fooling anyone."

[5472 op=1 keyword; failure -> 5474]
  1: ench,nico

[5473 op=2 display+exit]
  1: "Nicodemus the enchanter lives near here."
  2: "His house lies between two rivers."

[5474 op=1 keyword; failure -> 5476]
  1: bye

[5475 op=2 display+exit]
  1: "May you find that which thou deservest on thy quest."

[5478 op=2 display+exit]
  1: "Noble Avatar, the affairs of state weigh heavily upon my time."
  2: "Pray be precise in thy questioning."

[5479 op=2 display+exit]
  1: "Noble Avatar, we hardly have time to chat about $Z in view of the perils facing our land!"

[5481 op=1 keyword; failure -> 5483]
  1: yes

[5484 op=2 display+exit]
  1: "Then why dost thou waste my time with frivolous requests?!"
  2: "Begone, Avatar, and return when thou hast more serious business on thy mind!"

[5490 op=2 display+exit]
  1: "Very well then."
  2: Putting quill to parchment, she drafts a quick letter.
  3: "The jailer will now let you in, or verily shall he answer to me!"

```

<a id="lensmaker-npc-168"></a>
## Lensmaker ? NPC 168, port 169

Instruction range: 15118?15165

```text
[15120 op=1 keyword; failure -> 15123]
  1: look

[15122 op=2 display+exit]
  1: a very old gargoyle.
  2: The gargoyle gasps in terror and wheezes, "The False Prophet!"
  3: Then he faints dead away!

[15124 op=2 display+exit]
  1: The gargoyle gasps in terror and wheezes, "The False Prophet!"
  2: Then he faints dead away!

[15125 op=1 keyword; failure -> 15127]
  1: look

[15126 op=2 display+exit]
  1: a very old gargoyle.
  2: The gargoyle gasps in terror and wheezes, "An-bal-sil-fer!"
  3: Then he faints dead away!

[15127 op=2 display+exit]
  1: The gargoyle gasps in terror and wheezes, "An-bal-sil-fer!"
  2: Then he faints dead away!

[15130 op=2 display+exit]
  1: "To see the shattered Vortex Lens you carry."
  2: "But to not understand why you have brought it here!"
  3: "To suggest you talk to Naxatilor."

[15136 op=2 display+exit]
  1: "To see the shattered Vortex Lens you carry."
  2: "To remember making it, so very long ago."
  3: Taking the shards of glass, the lensmaker turns to his grinding table.
  4: In a short while, he has crafted a new Vortex Lens!
  5: "To have far greater skill now than when first I made this lens," he says smugly.
  6: "To require little time to repair it."
  7: He hands you the restored Vortex Lens.

[15137 op=1 keyword; failure -> 15141]
  1: look

[15140 op=2 display+exit]
  1: a very old gargoyle.
  2: The venerable gargoyle peers at you.
  3: "Greetings, False Prophet."
  4: "To have been told by @Naxatilor, long ago..."
  5: "That you would one day come to me, wearing the Amulet of Submission."
  6: "To never have believed it would come to pass."
  7: "To ask what service I can perform?"

[15141 op=1 keyword; failure -> 15145]
  1: look

[15144 op=2 display+exit]
  1: a very old gargoyle.
  2: "To wonder why you have not brought back the broken lens?!"
  3: "To have already told you that I need it in order to craft a new one!"
  4: "To ask what you think I can do without it?"

[15145 op=1 keyword; failure -> 15147]
  1: look

[15146 op=2 display+exit]
  1: a very old gargoyle.
  2: "To ask how again I may help you?"

[15147 op=1 keyword; failure -> 15150]
  1: vort,lens

[15149 op=2 display+exit]
  1: "To suggest you talk to Naxatilor."

[15150 op=1 keyword; failure -> 15153]
  1: vort,lens

[15152 op=2 display+exit]
  1: "To remember little from so long ago," the gargoyle croaks.
  2: "To require the pieces of the lens in order to make a new copy."

[15153 op=1 keyword; failure -> 15155]
  1: name

[15154 op=2 display+exit]
  1: "To lack a name, of course."
  2: "But to be called Lor-wis-lem, the scholar of light, by many learned gargoyles."
  3: As he says this, the ancient gargoyle beams with pride.

[15155 op=1 keyword; failure -> 15157]
  1: job

[15156 op=2 display+exit]
  1: "To craft the finest lenses in the land," the gargoyle says proudly.
  2: "Also to teach my young @apprentices not to cut themselves on the glass!"

[15157 op=1 keyword; failure -> 15159]
  1: teac,youn,appr

[15158 op=2 display+exit]
  1: "To be nearing the end of my @time."
  2: "To have one last sacred duty:"
  3: "To pass on my skill to the next generation."

[15159 op=1 keyword; failure -> 15161]
  1: end,time

[15160 op=2 display+exit]
  1: "To be over a thousand years old."
  2: "To seem a mere youth compared to @Naxatilor, however."

[15161 op=1 keyword; failure -> 15163]
  1: naxa

[15162 op=2 display+exit]
  1: "To have helped Naxatilor create the Vortex Lens, centuries ago."
  2: "To have found him venerable and ancient even then!"

[15163 op=1 keyword; failure -> 15165]
  1: bye

[15164 op=2 display+exit]
  1: "To be honored to have helped you, False Prophet."

[15165 op=2 display+exit]
  1: "To be old. To suggest you ask that of others."

```

<a id="leodon-npc-111"></a>
## Leodon ? NPC 111, port 112

Instruction range: 10378?10466

```text
[10379 op=1 keyword; failure -> 10382]
  1: yes

[10384 op=2 display+exit]
  1: "Okay."
  2: "I'll be here if you change your mind."

[10388 op=1 keyword; failure -> 10390]
  1: look

[10389 op=2 display+exit]
  1: a woman with a slightly seaworn look, but delicate hands.
  2: "Not now, $G!"
  3: She ducks a punch.
  4: "I'm busy!"

[10391 op=2 display+exit]
  1: "Not now, $G!"
  2: She ducks a punch.
  3: "I'm busy!"

[10393 op=1 keyword; failure -> 10395]
  1: look

[10394 op=2 display+exit]
  1: a woman with a slightly seaworn look, but delicate hands.
  2: "Yes, $P?"

[10395 op=1 keyword; failure -> 10397]
  1: look

[10396 op=2 display+exit]
  1: a woman with a slightly seaworn look, but delicate hands.
  2: "A good $T to you, $G."
  3: "Come, have a seat and talk with us a while."

[10397 op=1 keyword; failure -> 10399]
  1: name

[10398 op=2 display+exit]
  1: "I'm @Captain Leodon."

[10399 op=1 keyword; failure -> 10401]
  1: job

[10400 op=2 display+exit]
  1: "I'm captain of the @Golden @Hind."
  2: "She's the finest ship I've ever had the pleasure of @sailing 'cross the briny deep."

[10401 op=1 keyword; failure -> 10403]
  1: fine,ship,sail,brin,deep,gold,hind,plea

[10402 op=2 display+exit]
  1: "Life on the @sea is the only life for me."
  2: "Alas, the Hind needs serious @repairs just now."

[10403 op=1 keyword; failure -> 10406]
  1: need,seri,repa

[10405 op=2 display+exit]
  1: "Yes, I must keep myself occupied on land, meantime."

[10406 op=1 keyword; failure -> 10408]
  1: need,seri,repa

[10407 op=2 display+exit]
  1: "Yes, I must keep myself occupied on land, meantime."
  2: "Perhaps I could @join thy party for a time."

[10408 op=1 keyword; failure -> 10411]
  1: occu,land,join,part

[10410 op=2 display+exit]
  1: "I'd be glad to join you--but I have already!"

[10411 op=1 keyword; failure -> 10414]
  1: occu,land,join,part

[10413 op=2 display+exit]
  1: "The Golden Hind won't be repaired for months, at the soonest."
  2: "Traveling with you would likely prove interesting."
  3: "Would you like me to come along?"

[10414 op=1 keyword; failure -> 10416]
  1: leav

[10416 op=1 keyword; failure -> 10418]
  1: life,sea

[10417 op=2 display+exit]
  1: "Ah, the sea."
  2: "'Tis as vast as a thousand @dreams."

[10418 op=1 keyword; failure -> 10420]
  1: vast,thou,drea

[10419 op=2 display+exit]
  1: "Indeed."

[10420 op=1 keyword; failure -> 10423]
  1: capt

[10422 op=2 display+exit]
  1: "You wouldn't be talking about Captain @John, would you?"

[10423 op=1 keyword; failure -> 10425]
  1: capt

[10424 op=2 display+exit]
  1: "You wouldn't be talking about Captain @John, would you?"
  2: When you say 'Captain,' all the pirates at the table turn to you expectantly.
  3: When they realize you weren't talking to them, they return to their business.

[10426 op=1 keyword; failure -> 10430]
  1: leon

[10427 op=22 display+continue]
  1: "Aye, Leonna is the best first mate I've ever had."
  2: "Comes from being a captain once herself, you know."

[10429 op=2 display+exit]
  1: Leonna blushes and looks away, embarassed.

[10430 op=1 keyword; failure -> 10432]
  1: leon

[10431 op=2 display+exit]
  1: "Aye, Leonna is the best first mate I've ever had."
  2: "Comes from being a captain once herself, you know."

[10433 op=1 keyword; failure -> 10437]
  1: john

[10434 op=22 display+continue]
  1: "That scallawag?"
  2: "He went down to the other side of the world to join up with the @gargoyles, the dirty traitor..."

[10436 op=2 display+exit]
  1: The tea-drinker interrupts.
  2: "No, no, you've got it all wrong!"
  3: "I can tell you the truth about Captain John!"

[10437 op=1 keyword; failure -> 10439]
  1: john

[10438 op=2 display+exit]
  1: "That scallawag?"
  2: "He went down to the other side of the world to join up with the @gargoyles, the dirty traitor..."

[10439 op=1 keyword; failure -> 10441]
  1: garg

[10440 op=2 display+exit]
  1: "Vile creatures, they are."
  2: "One of them attacked  my ship once.
  3: He flew off with a @ham before anyone could stop him."

[10441 op=1 keyword; failure -> 10443]
  1: vile,crea,ham

[10442 op=2 display+exit]
  1: "Yes, he stole my ham, and I dined on @biscuits that night."

[10443 op=1 keyword; failure -> 10445]
  1: dine,bisc

[10444 op=2 display+exit]
  1: "That's right."

[10445 op=1 keyword; failure -> 10448]
  1: bye

[10447 op=2 display+exit]
  1: "Back to our quest, eh $G?"

[10448 op=1 keyword; failure -> 10450]
  1: bye

[10449 op=2 display+exit]
  1: "Drop by again next time you're in town."

[10450 op=2 display+exit]
  1: "I can't help you with that."

[10453 op=2 display+exit]
  1: "I'd be glad to join you--but I have already!"

[10455 op=2 display+exit]
  1: "You have enough traveling with you already."
  2: "But if you ask one of them to @leave first, I'd be happy to join you."

[10457 op=2 display+exit]
  1: "I'd be glad to join you--but someone else would have to ask me to leave first!"

[10459 op=2 display+exit]
  1: "Very well."
  2: "This should make a nice change from my usual routine."

[10460 op=2 display+exit]
  1: "Very well."
  2: "This should make a nice change from my usual routine."
  3: "You might ask @Leonna if she would like to join us as well."

[10463 op=2 display+exit]
  1: "I'd have to @join you before I could leave, silly!"

[10465 op=2 display+exit]
  1: "Let's get our feet back to Buckeneers Den first, okay?"

[10466 op=2 display+exit]
  1: "Okay."
  2: "I'll go wait back at the Fallen Virgin."
  3: "Here's all the equipment I was carrying, if you need it."

```

<a id="leonna-npc-112"></a>
## Leonna ? NPC 112, port 113

Instruction range: 10467?10533

```text
[10468 op=1 keyword; failure -> 10471]
  1: yes

[10473 op=2 display+exit]
  1: "Okay."
  2: "I guess I'll just stay here for now."

[10477 op=2 display+exit]
  1: a smartly dressed woman.
  2: Her smile has a delightful subtlety to it.
  3: You fail to get her attention in the midst of the brawl.

[10479 op=1 keyword; failure -> 10481]
  1: look

[10480 op=2 display+exit]
  1: a smartly dressed woman.
  2: Her smile has a delightful subtlety to it.
  3: "Yes, $P?"

[10482 op=1 keyword; failure -> 10484]
  1: look

[10483 op=2 display+exit]
  1: a smartly dressed woman.
  2: Her smile has a delightful subtlety to it.
  3: Her face lights up as she recognizes you.
  4: "Hello again, $P!"

[10484 op=1 keyword; failure -> 10487]
  1: look

[10486 op=2 display+exit]
  1: a smartly dressed woman.
  2: Her smile has a delightful subtlety to it.
  3: "Oh, you must be $P, the famous Avatar!"
  4: "'Tis an honor to meet thee."

[10487 op=1 keyword; failure -> 10489]
  1: name

[10488 op=2 display+exit]
  1: "My name is Leonna."

[10489 op=1 keyword; failure -> 10491]
  1: job

[10490 op=2 display+exit]
  1: "I used to @captain my own ship."
  2: "But I fell on hard times, and now I sail with @Leodon on the @Hind."

[10491 op=1 keyword; failure -> 10493]
  1: hind,ship,sail

[10492 op=2 display+exit]
  1: "A fine old vessel, the Golden Hind."
  2: "Alas, I have little to do while she's being @repaired."

[10493 op=1 keyword; failure -> 10496]
  1: litt,repa,join

[10495 op=2 display+exit]
  1: "I'd be glad to join you--but I have already!"

[10496 op=1 keyword; failure -> 10499]
  1: litt,repa,join

[10498 op=2 display+exit]
  1: "Could I join your party, perhaps?"

[10499 op=1 keyword; failure -> 10501]
  1: leav

[10501 op=1 keyword; failure -> 10503]
  1: leod

[10502 op=2 display+exit]
  1: "There's none I'd rather ship out with."

[10503 op=1 keyword; failure -> 10506]
  1: capt

[10505 op=2 display+exit]
  1: "Captain who?"
  2: "I know lots of Captains..."

[10506 op=1 keyword; failure -> 10508]
  1: capt

[10507 op=2 display+exit]
  1: "Captain who?"
  2: "I know lots of Captains..."
  3: When you say 'Captain,' all the pirates at the table turn to you expectantly.
  4: When they realize you weren't talking to them, they return to their business.

[10509 op=1 keyword; failure -> 10513]
  1: john

[10510 op=22 display+continue]
  1: "Captain John is a madman!"
  2: "He went down in Hythloth seeking a way to the other side of the world."
  3: "Said he was going to kill as many @gargoyles as he could before they got him."

[10512 op=2 display+exit]
  1: "That's not true!" says Captain Fox.
  2: "I can tell you what really happened."

[10513 op=1 keyword; failure -> 10515]
  1: john

[10514 op=2 display+exit]
  1: "Captain John is a madman!"
  2: "He went down in Hythloth seeking a way to the other side of the world."
  3: "Said he was going to kill as many @gargoyles as he could before they got him."

[10515 op=1 keyword; failure -> 10517]
  1: garg

[10516 op=2 display+exit]
  1: "Please, let's not talk about them."

[10517 op=1 keyword; failure -> 10519]
  1: bye

[10518 op=2 display+exit]
  1: "Talk to you later."

[10519 op=2 display+exit]
  1: "I can't help you with that."

[10522 op=2 display+exit]
  1: "I'd be glad to join you--but I have already!"

[10524 op=2 display+exit]
  1: "You have enough traveling with you already."
  2: "But if you ask one of them to @leave first, I'd be happy to join you."

[10526 op=2 display+exit]
  1: "I'd be glad to join you--but someone else would have to ask me to leave first!"

[10527 op=2 display+exit]
  1: "Oh good!"
  2: "I bet we'll meet a lot of interesting men..."

[10530 op=2 display+exit]
  1: "I'd have to @join you before I could leave, silly!"

[10532 op=2 display+exit]
  1: "Let's get our feet back to Buckeneers Den first, okay?"

[10533 op=2 display+exit]
  1: "Okay."
  2: "I'll go wait back at the Fallen Virgin."
  3: "You're more likely to need this equipment than I am."
  4: She sets down everything she was carrying before walking off.

```

<a id="linda-npc-185"></a>
## Linda ? NPC 185, port 186

Instruction range: 16218?16260

```text
[16220 op=1 keyword; failure -> 16222]
  1: look

[16221 op=2 display+exit]
  1: a rugged looking lady farmer.
  2: "Welcome, friend."

[16222 op=1 keyword; failure -> 16224]
  1: name

[16223 op=2 display+exit]
  1: "My name is Linda."

[16224 op=1 keyword; failure -> 16226]
  1: job

[16225 op=2 display+exit]
  1: "I raise @crops."
  2: "If you'd like to buy some @grain, just say the @word."

[16226 op=1 keyword; failure -> 16228]
  1: rais,crop

[16227 op=2 display+exit]
  1: "@Bread, cakes, pies, pastries..."
  2: "They all have their @roots here."

[16228 op=1 keyword; failure -> 16230]
  1: brea,cake,pie,past

[16229 op=2 display+exit]
  1: "@Woman does not live on @meat alone."

[16230 op=1 keyword; failure -> 16232]
  1: meat

[16231 op=2 display+exit]
  1: "Cows and @sheep are much more trouble to raise than @wheat."

[16232 op=1 keyword; failure -> 16234]
  1: cow,shee

[16233 op=2 display+exit]
  1: "They have their place."
  2: "But not on my farm."

[16234 op=1 keyword; failure -> 16236]
  1: root,rais,whea,farm

[16235 op=2 display+exit]
  1: "I was born to work the earth."
  2: "It was either this, or be a @gravedigger..."

[16236 op=1 keyword; failure -> 16238]
  1: grav

[16237 op=2 display+exit]
  1: "That's not my kind of work."

[16238 op=1 keyword; failure -> 16240]
  1: woma

[16239 op=2 display+exit]
  1: "Yes, men need to eat too, I suppose."

[16240 op=1 keyword; failure -> 16242]
  1: word

[16241 op=2 display+exit]
  1: "The word '@grain,' silly!"

[16242 op=1 keyword; failure -> 16245]
  1: buy,grain

[16244 op=2 display+exit]
  1: "It'll cost you 2 gold for each sack of grain."
  2: "How many do you need?"

[16245 op=1 keyword; failure -> 16247]
  1: bye

[16246 op=2 display+exit]
  1: "May the sun shine on you, and the rain bring you nourishment."

[16247 op=2 display+exit]
  1: "I don't know."
  2: "I guess so."

[16250 op=2 display+exit]
  1: "Okay."

[16252 op=2 display+exit]
  1: "You look pretty full to me."

[16257 op=2 display+exit]
  1: "Best check your coinpouch again."

[16259 op=2 display+exit]
  1: "You look pretty full to me."

[16260 op=2 display+exit]
  1: "Here you go."

```

<a id="lord-british-npc-3"></a>
## Lord British ? NPC 3, port 4

Instruction range: 163?367

```text
[168 op=2 display+exit]
  1: "$P! 'Tis good to see thee again. Much hath happened since thou last departed our realm."
  2: "But I must make sure 'tis truly thee."
  3: "Only the true Avatar would know what was contained in the Compendium I sent."
  4: "What doth trolls lack?"

[171 op=2 display+exit]
  1: "$P! 'Tis good to see thee again. Much hath happened since thou last departed our realm."
  2: "But I must make sure 'tis truly thee."
  3: "Only the true Avatar would know what was contained in the Compendium I sent."
  4: "What part of the tangle vine doth put one to sleep?"

[176 op=1 keyword; failure -> 179]
  1: endu

[178 op=2 display+exit]
  1: "Thou art correct."
  2: "How wert the headlesses produced?"

[180 op=1 keyword; failure -> 183]
  1: endu

[182 op=2 display+exit]
  1: "Thou art correct."
  2: "What valued item canst one find near the spawning grounds of Hydras?"

[184 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[189 op=1 keyword; failure -> 192]
  1: cent,pod,frag

[191 op=2 display+exit]
  1: "Thou art correct."
  2: "How wert the headlesses produced?"

[193 op=1 keyword; failure -> 196]
  1: cent,pod,frag

[195 op=2 display+exit]
  1: "Thou art correct."
  2: "What valued item canst one find near the spawning grounds of Hydras?"

[197 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[202 op=1 keyword; failure -> 205]
  1: wiza,expe

[204 op=2 display+exit]
  1: "Thou art correct."
  2: "How canst one fend off rotworms?"

[206 op=1 keyword; failure -> 209]
  1: wiza,expe

[208 op=2 display+exit]
  1: "Thou art correct."
  2: "How doth sea serpents attack?"

[210 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[215 op=1 keyword; failure -> 218]
  1: nigh,mush

[217 op=2 display+exit]
  1: "Thou art correct."
  2: "How canst one fend off rotworms?"

[219 op=1 keyword; failure -> 222]
  1: nigh,mush

[221 op=2 display+exit]
  1: "Thou art correct."
  2: "How doth sea serpents attack?"

[223 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[228 op=1 keyword; failure -> 231]
  1: torc,fire,flam,burn,pass

[230 op=2 display+exit]
  1: "Thou art correct."
  2: "What creature art wisps oft mistaken for?"

[232 op=1 keyword; failure -> 235]
  1: torc,fire,flam,burn,pass

[234 op=2 display+exit]
  1: "Thou art correct."
  2: "How doth giant squids crush their prey?"

[236 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[241 op=1 keyword; failure -> 244]
  1: fire,ball,swip,tail

[243 op=2 display+exit]
  1: "Thou art correct."
  2: "What creature art wisps oft mistaken for?"

[245 op=1 keyword; failure -> 248]
  1: fire,ball,swip,tail

[247 op=2 display+exit]
  1: "Thou art correct."
  2: "How doth giant squids crush their prey?"

[249 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[254 op=1 keyword; failure -> 257]
  1: fire,fly

[256 op=2 display+exit]
  1: "Thou art correct."
  2: "Where hath images of the silver serpent been seen?"

[258 op=1 keyword; failure -> 261]
  1: fire,fly

[260 op=2 display+exit]
  1: "Thou art correct."
  2: "What art reapers remnants of?"

[262 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[267 op=1 keyword; failure -> 270]
  1: beak

[269 op=2 display+exit]
  1: "Thou art correct."
  2: "Where hath images of the silver serpent been seen?"

[271 op=1 keyword; failure -> 274]
  1: beak

[273 op=2 display+exit]
  1: "Thou art correct."
  2: "What art reapers remnants of?"

[275 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[278 op=1 keyword; failure -> 281]
  1: tomb,wall,anci,monu

[282 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[285 op=1 keyword; failure -> 288]
  1: anci,ench,fore

[289 op=2 display+exit]
  1: "Nay, 'tis not the correct answer. Consult thy Compendium."

[293 op=1 keyword; failure -> 295]
  1: look

[294 op=2 display+exit]
  1: the noble ruler of Britannia.

[295 op=1 keyword; failure -> 298]
  1: name

[297 op=2 display+exit]
  1: "I am Lord British, as thou knowest well."

[298 op=1 keyword; failure -> 300]
  1: job

[299 op=2 display+exit]
  1: "Thanks to thee, I sit once more upon the throne of Britannia."
  2: "Though 'tis a heavy burden in such @troubled times as these."

[300 op=1 keyword; failure -> 302]
  1: heav,burd,trou,time

[301 op=2 display+exit]
  1: "The @gargoyles art indeed the greatest threat our realm has ever known."
  2: "We are fortunate indeed that fate hath brought thee here in our hour of need."

[302 op=1 keyword; failure -> 304]
  1: garg

[303 op=2 display+exit]
  1: "Perhaps thou canst drive these vile creatures back into the bowels of the earth from whence they came."
  2: "All our efforts thus far have availed us naught."

[304 op=1 keyword; failure -> 306]
  1: shri

[305 op=2 display+exit]
  1: "By now the gargoyles may have captured them all."
  2: "Thou must hurry if thou wouldst foil their evil schemes..."

[306 op=1 keyword; failure -> 308]
  1: virt

[307 op=2 display+exit]
  1: "Stay strong in thy commitment to the eight virtues."
  2: "It is our belief in them that sets us apart from the cruel invaders who would destroy all that we hold dear."

[308 op=1 keyword; failure -> 310]
  1: geof

[309 op=2 display+exit]
  1: "He is the Captain of the Guard."

[310 op=1 keyword; failure -> 312]
  1: mant,rune

[311 op=2 display+exit]
  1: "Ask the leaders of each town to tell thee of that.

[312 op=1 keyword; failure -> 314]
  1: comp

[313 op=2 display+exit]
  1: "Do thou ask @Tholden."

[314 op=1 keyword; failure -> 316]
  1: thol

[315 op=2 display+exit]
  1: "He is my @chancellor."

[316 op=1 keyword; failure -> 318]
  1: chan

[317 op=2 display+exit]
  1: "Aye, Tholden is my chancellor."

[318 op=1 keyword; failure -> 321]
  1: mous,sher

[320 op=2 display+exit]
  1: "Please take good care of my little friend."

[321 op=1 keyword; failure -> 323]
  1: mous,sher

[322 op=2 display+exit]
  1: "Ah, thou hast heard of my little friend?"
  2: "Her name is Sherry, and I'm quite proud of her."
  3: "I'm certain she is the only talking mouse in all of Britannia."
  4: "Thou art welcome to see her, if thou dost wish."
  5: "Thou canst find her wandering throughout the castle."
  6: "She comes by my room every night, so I can tell her @stories."

[323 op=1 keyword; failure -> 326]
  1: wiza,oz,book

[325 op=2 display+exit]
  1: "I thank thee for finding the book."

[326 op=1 keyword; failure -> 329]
  1: wiza,oz,book

[328 op=2 display+exit]
  1: "I collect rare books."
  2: "There's one in particular I've been hunting for many years."
  3: "It's called 'The Wizard of Oz.' Ever heard of it?"

[329 op=1 keyword; failure -> 331]
  1: bedt,stor

[330 op=2 display+exit]
  1: "My favorite story is '@Hubert the Lion.'"

[331 op=1 keyword; failure -> 333]
  1: favo,hube,lion

[332 op=2 display+exit]
  1: "I've known it by heart, ever since I heard it as a child."
  2: "Hubert the Lion was haughty and vain, and especially proud of his elegant mane."
  3: "But conceit of this sort is not proper at all, and Hubert the Lion was due for a fall."

[333 op=1 keyword; failure -> 335]
  1: than

[334 op=2 display+exit]
  1: "'Tis I who should thank thee, Avatar, for all thou hast done for Britannia."

[335 op=1 keyword; failure -> 337]
  1: nose

[336 op=2 display+exit]
  1: "Who told thee of that nickname!?"
  2: "Well, I'd rather thou didst not call me that."

[337 op=1 keyword; failure -> 340]
  1: heal,cure

[339 op=2 display+exit]
  1: "I canst only cure thy desperate needs."
  2: Lord British waves his hand, and your whole party is cured!

[340 op=1 keyword; failure -> 342]
  1: repe

[341 op=2 display+exit]
  1: "The vast underworld from which thou didst rescue me hath collapsed."
  2: "Yet still there are forces of evil abroad in the land."
  3: "Britannia is under attack by gargoyles such as those thou just fought."
  4: "They have been coming up through the dungeons."
  5: "Thus far they have mainly been attacking the @shrines of the eight @virtues."
  6: "When the Shrine of Compassion didst fall, Sir @Geoffrey sent a party to free it."
  7: "Do thou ask him of this mission. Perhaps thou canst prove of some assistance."
  8: "Lastly, any time thou dost need @healing, do thou but ask me."
  9: "If thou doth wish me to @repeat all this later, thou need but ask."

[342 op=1 keyword; failure -> 346]
  1: moon,gate,ston,blac,orb

[345 op=2 display+exit]
  1: You show Lord British the black stone.
  2: "Hmmmm..."
  3: "I have such a stone, as thou may recall."
  4: "I did not know that there were more such orbs."
  5: "'Twill serve thee well in thy travels if thou dost learn to master its powers."
  6: "To open a gate, use the stone, and carefully position it a few feet from thee."
  7: "Thou wilt discover that the placement is the key."
  8: "In the proper positions,
  9: the stone canst conjure gates to take thee to numerous destinations."

[346 op=1 keyword; failure -> 348]
  1: bye

[347 op=2 display+exit]
  1: "May fortune favor thee."

[348 op=2 display+exit]
  1: "I cannot help thee with that."

[352 op=2 display+exit]
  1: "Thou art correct."
  2: "Ah, 'tis thee indeed, $P."
  3: "Take this key."
  4: "It will unlock the gatehouse by the southern entrance to the castle."
  5: "Then you can use the lever inside to raise the portcullis, and the crank will lower the drawbridge."
  6: "The same key will also let you into the sewers under the castle."
  7: "Now let me tell thee what hath transpired since thy last visit."
  8: "The vast underworld from which thou didst rescue me hath collapsed."
  9: "Yet still there are forces of evil abroad in the land."
  10: "Britannia is under attack by gargoyles such as those thou just fought."
  11: "They have been coming up through the dungeons."
  12: "Thus far they have mainly been attacking the @shrines of the eight @virtues."
  13: "When the Shrine of Compassion didst fall, Sir @Geoffrey sent a party to free it."
  14: "Do thou ask him of this mission. Perhaps thou canst prove of some assistance."
  15: "Lastly, any time thou dost need @healing, do thou but ask me."
  16: "If thou doth wish me to @repeat all this later, thou need but ask."

[353 op=1 keyword; failure -> 362]
  1: yes

[361 op=2 display+exit]
  1: "Thou hath found a copy!"
  2: "Long hath I anticipated this moment..."
  3: "Not since my childhood have I read this wondrous story."
  4: Gingerly, he takes the tome. "Here is your reward." He gives you some glowing gems.

[362 op=1 keyword; failure -> 366]
  1: yes

[365 op=2 display+exit]
  1: "Thou art carrying too much for me to reward thee properly..."
  2: "Bring the book back later, when thou art less burdened."

[367 op=2 display+exit]
  1: "Well, if thou dost run across it in thy travels,
  2: I would greatly appreciate if thou couldst bring it here."
  3: "I hath promised a great reward to whosoever shalt bring me a copy."

```

<a id="loubet-npc-156"></a>
## Loubet ? NPC 156, port 157

Instruction range: 14338?14389

```text
[14338 op=1 keyword; failure -> 14340]
  1: prac,ask,less

[14342 op=1 keyword; failure -> 14345]
  1: yes,prac,ask,less

[14344 op=2 display+exit]
  1: "Practice?"
  2: "Here?"
  3: "Surely you jest!"
  4: "No, come to ma salle sometime, zen we will spar, yes?"

[14346 op=1 keyword; failure -> 14350]
  1: yes,prac,ask,less

[14349 op=2 display+exit]
  1: "But no!"
  2: "You are too skilled, I 'ave nothing more to teach you!"

[14351 op=1 keyword; failure -> 14355]
  1: yes,prac,ask,less

[14354 op=2 display+exit]
  1: Loubet hands you a foil and a mask of golden mesh.
  2: He shows you the correct stance, then you spar for a bit.
  3: "Now ze off hand, $G... Mon dieu, you are (how you say it?) ambidextrous! Very good, mah friend."
  4: M'sieur Loubet is a skilled teacher, and you feel like you've really learned something.

[14357 op=2 display+exit]
  1: "You 'ave but to ask, mah friend."

[14358 op=1 keyword; failure -> 14361]
  1: look

[14360 op=2 display+exit]
  1: a tall and dashing gentleman.
  2: He greets you with an elaborate bow and flourish."
  3: "Good $T, mon ami!"
  4: "'Ave you come to moi for @instruction?"

[14361 op=1 keyword; failure -> 14364]
  1: fenc,inst

[14363 op=2 display+exit]
  1: "Oui, I teach ze fencing here in my salle, my school."
  2: "Is very difficult sometimes."
  3: "Most fighters, zey have no subtlety, no finesse."
  4: He turns up his nose and sniffs aristocratically.
  5: "Zey are not like you, mah friend!"
  6: "You are most talented!"

[14365 op=1 keyword; failure -> 14367]
  1: fenc,inst

[14366 op=2 display+exit]
  1: "Oui, I teach ze fencing here in my salle, my school."
  2: "Is very difficult sometimes."
  3: "Most fighters, zey have no subtlety, no finesse."
  4: He turns up his nose and sniffs aristocratically.
  5: "But you, you look much better than that, my friend."
  6: "Come to ma salle sometime and I'll give you a @lesson!"

[14367 op=1 keyword; failure -> 14369]
  1: fenc,inst

[14368 op=2 display+exit]
  1: "Oui, I teach ze fencing here in my salle, my school."
  2: "Is very difficult sometimes."
  3: "Most fighters, zey have no subtlety, no finesse."
  4: He turns up his nose and sniffs aristocratically.
  5: "But you, you look much better than that, my friend."
  6: "Just ask and I'll give you a @lesson!"

[14369 op=1 keyword; failure -> 14372]
  1: name

[14371 op=2 display+exit]
  1: "I am M'sieur Loubet.
  2: "@Touche!"
  3: He wields his foil casually, lunging at an imaginary target.
  4: But you already know who I am, silly person!"

[14373 op=1 keyword; failure -> 14376]
  1: name

[14375 op=2 display+exit]
  1: "I am M'sieur Loubet.
  2: "@Touche!"
  3: He wields his foil casually, lunging at an imaginary target.
  4: "Much pleased to meet you, madame."

[14376 op=1 keyword; failure -> 14379]
  1: name

[14378 op=2 display+exit]
  1: "I am M'sieur Loubet.
  2: "@Touche!"
  3: He wields his foil casually, lunging at an imaginary target.
  4: "Much pleased to meet you, m'sieur."

[14379 op=1 keyword; failure -> 14381]
  1: touc

[14380 op=2 display+exit]
  1: "Zat simply means 'touch.'"
  2: "In fencing we score by touching ze opponent with ze foil @tip."

[14381 op=1 keyword; failure -> 14383]
  1: foil,tip,scor

[14382 op=2 display+exit]
  1: "Zey are blunt on ze end, see?"
  2: "Ze first third of ze blade (near ze 'andle) is 'forte'--strong."
  3: "It is good to parry with zis part of ze sword."
  4: "Ze rest of ze blade is 'foible'--weak and flexible."
  5: "Ze foil, she is a very delicate thing."
  6: "There must be much skill, yes?"
  7: "You must 'old ze sword like a living bird:"
  8: "Too loose and she flies away."
  9: "Too tight and she smothers."
  10: "Just right, and..."
  11: "She sings very sweet!"
  12: He strikes metal and the foil makes a pleasant @ringing noise.

[14383 op=1 keyword; failure -> 14385]
  1: nois,plea,ring,hear

[14384 op=2 display+exit]
  1: SHINGGG!

[14385 op=1 keyword; failure -> 14387]
  1: job

[14386 op=2 display+exit]
  1: "I am ze @fencing @instructor."

[14387 op=1 keyword; failure -> 14389]
  1: bye,au

[14388 op=2 display+exit]
  1: "Au revoir!"

[14389 op=2 display+exit]
  1: "I cannot 'elp you with zat."

```

<a id="lynn-npc-27"></a>
## Lynn ? NPC 27, port 28

Instruction range: 2265?2316

```text
[2265 op=1 keyword; failure -> 2267]
  1: look

[2266 op=2 display+exit]
  1: a tall, willowy woman with long blond hair.
  2: "Well, hello again!"
  3: "How can I help thee this fine day?"

[2267 op=1 keyword; failure -> 2270]
  1: name

[2269 op=2 display+exit]
  1: "I'm Lynn."
  2: "Thou needn't tell me thy name. You are famous throughout the land!"

[2270 op=1 keyword; failure -> 2272]
  1: job

[2271 op=2 display+exit]
  1: "I am a fletcher."
  2: "I make @arrows and @bolts for the armies of Lord British himself!"

[2272 op=1 keyword; failure -> 2274]
  1: sell

[2273 op=2 display+exit]
  1: "I have all I need, and have no reason to purchase anything more."
  2: "But I appreciate thy offer."

[2276 op=1 keyword; failure -> 2278]
  1: buy

[2277 op=2 display+exit]
  1: "I make @arrows and @bolts. Which?"

[2278 op=1 keyword; failure -> 2281]
  1: arro

[2280 op=2 display+exit]
  1: "I sell arrows only by the dozen for 4 gold."
  2: "How many dozens?"

[2281 op=1 keyword; failure -> 2284]
  1: bolt

[2283 op=2 display+exit]
  1: "I sell bolts only by the dozen for 3 gold."
  2: "How many dozens?"

[2292 op=1 keyword; failure -> 2294]
  1: buy

[2293 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[2294 op=1 keyword; failure -> 2296]
  1: arro

[2295 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[2296 op=1 keyword; failure -> 2298]
  1: bolt

[2297 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[2298 op=1 keyword; failure -> 2300]
  1: bye

[2299 op=2 display+exit]
  1: "Farewell! I hope I've aided thy quest!"

[2300 op=2 display+exit]
  1: "Beg pardon?"

[2303 op=2 display+exit]
  1: "Perhaps some other time."

[2305 op=2 display+exit]
  1: "But that's too many!"

[2311 op=2 display+exit]
  1: "But thou hast not enough gold for that many!"

[2313 op=2 display+exit]
  1: "But thou canst not carry that many!"

[2315 op=2 display+exit]
  1: Lynn takes the money and hands over the arrows.
  2: "I appreciate thy business."
  3: "Enjoy thy arrows."

[2316 op=2 display+exit]
  1: Lynn takes the money and hands over the bolts.
  2: "I appreciate thy business."
  3: "Enjoy thy bolts."

```

<a id="lyssandra-npc-46"></a>
## Lyssandra ? NPC 46, port 47

Instruction range: 4204?4241

```text
[4204 op=1 keyword; failure -> 4206]
  1: look

[4205 op=2 display+exit]
  1: a girl of perhaps twelve years.
  2: She is agile, and balances trays of mugs deftly on each hand.
  3: It is a wonder how she can duck the hurled debris and avoid the flying fists.
  4: She is quick, and never spills a drop of ale.
  5: "Hallo, $G. Welcome to the 'Sword and @Keg.'

[4206 op=1 keyword; failure -> 4208]
  1: swor,keg,drin,orde

[4207 op=2 display+exit]
  1: "Sometimes 'tis rough here but I doesn't mind."
  2: "Some day I'll be a @fighter, ye know."

[4208 op=1 keyword; failure -> 4211]
  1: name

[4210 op=2 display+exit]
  1: "It's Lyssandra, but ye can call me @Andy."

[4211 op=1 keyword; failure -> 4213]
  1: Andy

[4212 op=2 display+exit]
  1: "I know 'tis a boy's name, but I like it all the same."

[4213 op=1 keyword; failure -> 4215]
  1: job,mug

[4214 op=2 display+exit]
  1: "I take orders for drinks, $G."

[4215 op=1 keyword; failure -> 4217]
  1: figh,braw

[4216 op=2 display+exit]
  1: "The fighting here usually begins over--"
  2: Her voice drops to a whisper, "--the @rat!"
  3: "Around sunset things get really rowdy here!"

[4217 op=1 keyword; failure -> 4219]
  1: rat,mous

[4218 op=2 display+exit]
  1: "Shhh!"
  2: She glances about to see if you have been overheard.
  3: "See yon hole in the north wall?"
  4: "Look carefully at where floor and wall meet."
  5: "That's where it took the rune!"
  6: "They tried all manner of tricks to get it out but they all failed."
  7: "I had an @idea, but nobody listened to me."

[4219 op=1 keyword; failure -> 4222]
  1: idea,list

[4221 op=2 display+exit]
  1: "Do ye know of the talking mouse? Lord British's friend?"

[4223 op=1 keyword; failure -> 4226]
  1: yes

[4225 op=2 display+exit]
  1: "A mouse could get the rune, of that I'm sure!"

[4228 op=2 display+exit]
  1: "Well, ye would think it was a dumb idea anyways..."

[4229 op=1 keyword; failure -> 4231]
  1: rune

[4230 op=2 display+exit]
  1: "I know how to get it from the you-know-@what."

[4231 op=1 keyword; failure -> 4233]
  1: what,you,how

[4232 op=2 display+exit]
  1: "The @rat," she whispers conspiratorially to you.

[4233 op=1 keyword; failure -> 4235]
  1: mant

[4234 op=2 display+exit]
  1: "Culham has oft sung of it, with @Jerris and @Starhelm."

[4235 op=1 keyword; failure -> 4237]
  1: Star

[4236 op=2 display+exit]
  1: "His real name is Stelnar."
  2: "He would be much nicer if he were not so angry all the time."

[4237 op=1 keyword; failure -> 4239]
  1: Jerr

[4238 op=2 display+exit]
  1: "He is brave when @fighting, but he is afraid to sing by himself."

[4239 op=1 keyword; failure -> 4241]
  1: bye

[4240 op=2 display+exit]
  1: "Bye-bye, $G. It was nice talking to you."

[4241 op=2 display+exit]
  1: "I cannot help ye with that."

```

<a id="maldric-npc-30"></a>
## Maldric ? NPC 30, port 31

Instruction range: 2540?2608

```text
[2542 op=1 keyword; failure -> 2545]
  1: look

[2544 op=2 display+exit]
  1: a bare chested, muscular man, his body gleaming with sweat.
  2: "Excuse me a moment." He pulls a napkin from his belt and mops at his brow. "Now, what were you saying?"
  3: "Wait a minute," he interrupts, "I recognize you... You're the Avatar! I've always wanted to meet you."
  4: "From the day I first saw your portrait, I knew you must be the most beautiful woman in all Britannia."

[2546 op=1 keyword; failure -> 2549]
  1: look

[2548 op=2 display+exit]
  1: a bare chested, muscular man, his body gleaming with sweat.
  2: "Excuse me a moment." He pulls a napkin from his belt and mops at his brow. "Now, what were you saying?"
  3: "Wait a minute," he interrupts, "I recognize you... You're the Avatar! I've always wanted to meet you."
  4: "You're the greatest hero in all the land."

[2552 op=2 display+exit]
  1: "Excuse me a moment." He pulls a napkin from his belt and mops at his brow. "Now, what were you saying?"
  2: "Wait a minute," he interrupts, "I recognize you... You're the Avatar! I've always wanted to meet you."
  3: "Oh, $G Avatar, 'tis an unexpected pleasure to see you again!"
  4: "From the day I first saw your portrait, I knew you must be the most beautiful woman in all Britannia."

[2555 op=2 display+exit]
  1: "Excuse me a moment." He pulls a napkin from his belt and mops at his brow. "Now, what were you saying?"
  2: "Wait a minute," he interrupts, "I recognize you... You're the Avatar! I've always wanted to meet you."
  3: "Oh, $G Avatar, 'tis an unexpected pleasure to see you again!"
  4: "You're the greatest hero in all the land."

[2557 op=1 keyword; failure -> 2559]
  1: look

[2558 op=2 display+exit]
  1: a bare chested, muscular man, his body gleaming with sweat.
  2: "Excuse me a moment." He pulls a napkin from his belt and mops at his brow. "Now, what were you saying?"
  3: "Oh, $G Avatar, 'tis an unexpected pleasure to see you again!"

[2559 op=1 keyword; failure -> 2561]
  1: look

[2560 op=2 display+exit]
  1: a bare chested, muscular man, his body gleaming with sweat.
  2: "Excuse me a moment." He pulls a napkin from his belt and mops at his brow. "Now, what were you saying?"
  3: "Hey, my old buddy $P! Good to see you again."

[2561 op=1 keyword; failure -> 2564]
  1: name

[2563 op=2 display+exit]
  1: "Maldric, I am."
  2: "Once Maldric the @boar hunter, now Maldric the cook."

[2564 op=1 keyword; failure -> 2566]
  1: once,boar,hunt

[2565 op=2 display+exit]
  1: "Aye, I used to live in the @forest and hunt wild boars."
  2: "But one day I presented a @roast boar as a gift to his @majesty, and he liked it so much he hired me as his @cook."

[2566 op=1 keyword; failure -> 2568]
  1: maje

[2567 op=2 display+exit]
  1: "Yes, Lord @British himself."

[2568 op=1 keyword; failure -> 2570]
  1: fore

[2569 op=2 display+exit]
  1: "Those were good times. But I'm happy here."

[2570 op=1 keyword; failure -> 2572]
  1: pres,gift,roas,like

[2571 op=2 display+exit]
  1: "The secret is in the spices. It's an old family @recipe."

[2573 op=1 keyword; failure -> 2575]
  1: secr,fami,reci

[2574 op=2 display+exit]
  1: "I'll never tell."
  2: "Though for one so sweet, I must say I'm tempted."

[2575 op=1 keyword; failure -> 2577]
  1: secr,fami,reci

[2576 op=2 display+exit]
  1: "I'll never tell."

[2577 op=1 keyword; failure -> 2579]
  1: job

[2578 op=2 display+exit]
  1: "I @cook for all those who live in the @castle, and for any @guests who dine with Lord British as well."
  2: "'Tis sastisfying work, yet at times I wish I could @travel with one such as you..."

[2579 op=1 keyword; failure -> 2581]
  1: gues,dine

[2580 op=2 display+exit]
  1: "Lord British has many friends."

[2582 op=1 keyword; failure -> 2584]
  1: cast

[2583 op=2 display+exit]
  1: "A fancier home than any of my family ever lived in. Yet I still miss my simple hut in the forest."
  2: "I wish it were still there, so I could invite you to come spend an evening by the @fireplace with me..."

[2584 op=1 keyword; failure -> 2586]
  1: cast

[2585 op=2 display+exit]
  1: "A fancier home than any of my family ever lived in. Yet I still miss my simple hut in the forest."

[2586 op=1 keyword; failure -> 2588]
  1: spen,even,fire,invi

[2587 op=2 display+exit]
  1: "There's fire enough in my kitchen, but 'tis no place for romance."

[2588 op=1 keyword; failure -> 2590]
  1: kitc,plac,roma,trav,wish,join,warr

[2589 op=2 display+exit]
  1: "I shouldn't even speak of such things."
  2: "After all, you are the Avatar, and I but a lowly cook. Please forgive me."

[2590 op=1 keyword; failure -> 2592]
  1: lord,brit

[2591 op=2 display+exit]
  1: "He's a good man. Likes plenty of @pepper in his stew."

[2592 op=1 keyword; failure -> 2594]
  1: pepp,stew

[2593 op=2 display+exit]
  1: "Spice puts some variety in life, is what I always say."

[2595 op=1 keyword; failure -> 2597]
  1: spic

[2596 op=2 display+exit]
  1: "You're a pretty spicy little number yourself!"

[2597 op=1 keyword; failure -> 2599]
  1: spic,vari,life,frie,hear,adve,peop,happ

[2598 op=2 display+exit]
  1: "Yes indeed."

[2599 op=1 keyword; failure -> 2601]
  1: avat

[2600 op=2 display+exit]
  1: "I've heard about all of your adventures. I wish I were a warrior too, and not just a cook!"

[2601 op=1 keyword; failure -> 2603]
  1: cook

[2602 op=2 display+exit]
  1: "I like having a job where I can make people happy."

[2604 op=1 keyword; failure -> 2606]
  1: bye

[2605 op=2 display+exit]
  1: "See you later. And next time you dine with his majesty, I'll cook you something special."
  2: He seems about to say more, with a hopeful look in his eyes.
  3: But then he turns away without speaking.

[2606 op=1 keyword; failure -> 2608]
  1: bye

[2607 op=2 display+exit]
  1: "See you later. And next time you dine with his majesty, I'll cook you something special."

[2608 op=2 display+exit]
  1: "Funny you should mention that... It reminds me of my secret recipe for @roast boar."

```

<a id="mama-npc-142"></a>
## Mama ? NPC 142, port 143

Instruction range: 13103?13137

```text
[13104 op=1 keyword; failure -> 13107]
  1: yes

[13106 op=2 display+exit]
  1: "Good, he need tiny friends!"

[13109 op=2 display+exit]
  1: "But he need tiny friends! He get lonely!"

[13110 op=1 keyword; failure -> 13114]
  1: look

[13113 op=2 display+exit]
  1: a huge female cyclops.
  2: "Oh, you cute! You funny-looking like @tiny one!"

[13114 op=1 keyword; failure -> 13117]
  1: look

[13116 op=2 display+exit]
  1: a huge female cyclops.
  2: "You come back! Play with @tiny one?"

[13117 op=1 keyword; failure -> 13119]
  1: name

[13118 op=2 display+exit]
  1: "Me cyclops."
  2: "@Tiny one say me 'Mama.'"

[13119 op=1 keyword; failure -> 13121]
  1: tiny,one

[13120 op=2 display+exit]
  1: "Male find him on shore."
  2: "One-eye little one die long ago."
  3: "Two-eye little one just as good!"

[13121 op=1 keyword; failure -> 13123]
  1: job

[13122 op=2 display+exit]
  1: "Live here! Cook, hunt!"

[13123 op=1 keyword; failure -> 13125]
  1: cell

[13124 op=2 display+exit]
  1: "Cellar no go. Deep deep deep. @Locked."

[13125 op=1 keyword; failure -> 13127]
  1: vort

[13126 op=2 display+exit]
  1: "What look like?"

[13127 op=1 keyword; failure -> 13129]
  1: cube

[13128 op=2 display+exit]
  1: "What color?"

[13129 op=1 keyword; failure -> 13131]
  1: blac

[13130 op=2 display+exit]
  1: "Oh, me seen that! Big black @block!"

[13131 op=1 keyword; failure -> 13133]
  1: lock,key

[13132 op=2 display+exit]
  1: "Talk male. He have key."

[13133 op=1 keyword; failure -> 13135]
  1: bloc

[13134 op=2 display+exit]
  1: "Male keep that in @cellar."

[13135 op=1 keyword; failure -> 13137]
  1: bye

[13136 op=2 display+exit]
  1: "Come back soon, play with tiny one more!"

[13137 op=2 display+exit]
  1: "What mean $Z?"

```

<a id="mandrake-npc-178"></a>
## Mandrake ? NPC 178, port 179

Instruction range: 15719?15860

```text
[15721 op=2 display+exit]
  1: "Well, never mind that for the moment."
  2: "Let me sing thee a @song!"

[15734 op=2 display+exit]
  1: "A pleasure, to be sure, $G."
  2: "I hope I'll be seeing a lot more of you."
  3: He gives you a sly wink.

[15735 op=2 display+exit]
  1: "Well met, sirrah!"
  2: "You look like the sort who knows how to enjoy a good @story."

[15739 op=1 keyword; failure -> 15742]
  1: yes

[15741 op=2 display+exit]
  1: "How much do you give him?"

[15748 op=1 keyword; failure -> 15751]
  1: yes

[15750 op=2 display+exit]
  1: "How much do you give him?"

[15754 op=1 keyword; failure -> 15758]
  1: look

[15757 op=2 display+exit]
  1: a charming fellow with a peacock feather in his cap.
  2: "Welcome, welcome!"
  3: "'Tis always a pleasure to see a new face."
  4: "My name is Mandrake."

[15758 op=1 keyword; failure -> 15761]
  1: look

[15760 op=2 display+exit]
  1: a charming fellow with a peacock feather in his cap.
  2: "$P!"
  3: "It's been too long."
  4: "What brings you to this part of the realm?"

[15761 op=1 keyword; failure -> 15765]
  1: name

[15764 op=2 display+exit]
  1: "Mandrake, my friend, Mandrake."
  2: "Like the @root, but taller, handsomer, and @louder."
  3: "And what may I call thee?"

[15765 op=1 keyword; failure -> 15767]
  1: name

[15766 op=2 display+exit]
  1: "Mandrake, my friend, Mandrake."
  2: "Like the @root, but taller, handsomer, and @louder."

[15767 op=1 keyword; failure -> 15769]
  1: root

[15768 op=2 display+exit]
  1: "It grows in swamps, but I'm found mostly in @taverns."

[15769 op=1 keyword; failure -> 15771]
  1: tall,hand,loud

[15770 op=2 display+exit]
  1: "So I've heard."

[15771 op=1 keyword; failure -> 15773]
  1: tave

[15772 op=2 display+exit]
  1: "I visit them all."
  2: "The Fallen @Virgin and the Sword and @Keg are rowdy and rough."
  3: "The Blue @Bottle serves Frasier's @Folly, the finest ale in all Britannia."
  4: "But the Cat's @Lair is still my favorite."

[15773 op=1 keyword; failure -> 15775]
  1: fras,foll

[15774 op=2 display+exit]
  1: "You should try some!"
  2: "'Twill do thee a world of good."

[15775 op=1 keyword; failure -> 15777]
  1: fall,virg

[15776 op=2 display+exit]
  1: "'Tis in Buccaneer's @Den, no place for the timid."

[15777 op=1 keyword; failure -> 15779]
  1: bucc,den

[15778 op=2 display+exit]
  1: "If thou knowest not where it is, I'll not be the one to say."

[15779 op=1 keyword; failure -> 15781]
  1: swor,keg

[15780 op=2 display+exit]
  1: "Over in @Jhelom, it is."
  2: "They always love a good brawl."

[15781 op=1 keyword; failure -> 15783]
  1: braw,jhel

[15782 op=2 display+exit]
  1: "Lots of warriors there."
  2: "They fight just about every night, but they don't mean anything by it."
  3: "'Tis all in fun."

[15783 op=1 keyword; failure -> 15785]
  1: blue,bott

[15784 op=2 display+exit]
  1: "'Tis in @Moonglow."
  2: "@Derydlus often drinks there."
  3: "The man can put away more ale than any other I've met."
  4: "Many's the time he's drunk me under the table."

[15785 op=1 keyword; failure -> 15787]
  1: moon

[15786 op=2 display+exit]
  1: "Too honest a town to be much fun, but some fine people live there."
  2: "You should have @Penumbra tell your fortune sometime."

[15787 op=1 keyword; failure -> 15789]
  1: dery

[15788 op=2 display+exit]
  1: "Give him my regards if you see him."

[15789 op=1 keyword; failure -> 15791]
  1: penu

[15790 op=2 display+exit]
  1: "I hear she has strange @powers."

[15791 op=1 keyword; failure -> 15793]
  1: stra,powe

[15792 op=2 display+exit]
  1: "Yes, the gift of @prophecy."

[15793 op=1 keyword; failure -> 15795]
  1: gift,prop

[15794 op=2 display+exit]
  1: "So I hear."

[15795 op=1 keyword; failure -> 15797]
  1: cat,lair

[15796 op=2 display+exit]
  1: "A wonderful place!"
  2: "Dr. Cat gives me free drinks just for singing."
  3: "He loves games of all kinds too."
  4: "Mention my name to him and he'll be sure to treat thee well."

[15797 op=1 keyword; failure -> 15799]
  1: job

[15798 op=2 display+exit]
  1: "I travel all over to spread important @news throughout the realm,"
  2: "and @tales and @songs as well."
  3: "If you have time enough, I'll favor you with all three."

[15799 op=1 keyword; failure -> 15801]
  1: trav,impo,news

[15800 op=2 display+exit]
  1: "My travels were interrupted recently in a most fearsome manner."
  2: "I was captured by the @gargoyles,"
  3: "and taken down to the other side of the world,"
  4: "where they held me prisoner."
  5: "But such was my good fortune that one of them helped me to escape."
  6: "He spoke but little of our tongue,"
  7: "but I believe he said his name was 'Beh Lem.'"

[15801 op=1 keyword; failure -> 15803]
  1: garg

[15802 op=2 display+exit]
  1: "I've seen their handiwork many a time in my travels."
  2: "I believe they have taken over all of the shrines of the eight @virtues."

[15803 op=1 keyword; failure -> 15805]
  1: eigh,virt

[15804 op=2 display+exit]
  1: "Oh, you know: honesty, compassion, valour, justice,"
  2: "sacrifice, honor, spirituality, and humility."
  3: "That's what you get for choosing such @stuffy principles as a basis."

[15805 op=1 keyword; failure -> 15807]
  1: stuf,prin

[15806 op=2 display+exit]
  1: "Truth, love and courage... hah!"
  2: "If it had been up to me, I'd have chosen wine, women and song instead."
  3: "Think of the virtues those would @lead to!"

[15807 op=1 keyword; failure -> 15809]
  1: wine,wome,lead,thin

[15808 op=2 display+exit]
  1: "I've thought it over..."
  2: "The principles of wine, women and song would lead to the eight virtues of..."
  3: "Drunkenness, sensuality, harmony, lust, laziness, dance, indulgence, and happiness."
  4: "Now there would be the foundation for a fine world to live in!"

[15809 op=1 keyword; failure -> 15812]
  1: sing,song

[15811 op=2 display+exit]
  1: "I sing better if I keep my belly full, and my wineskin as well."
  2: "Would you care to donate a coin or two?"

[15812 op=1 keyword; failure -> 15815]
  1: tale,stor

[15814 op=2 display+exit]
  1: "Perhaps my stories would be worth a few coins?"

[15815 op=1 keyword; failure -> 15818]
  1: bye

[15817 op=2 display+exit]
  1: "Leaving so soon, $G?"
  2: "And I was enjoying your company so."
  3: He bows and kisses your hand.
  4: "I'll be glad to play another song for you when next we meet."

[15818 op=1 keyword; failure -> 15820]
  1: bye

[15819 op=2 display+exit]
  1: "Well, $P, I'm glad you stopped by to chat with me."
  2: "We must do it again sometime."
  3: "I'll be glad to play another song for you when next we meet."

[15822 op=2 display+exit]
  1: "I'll tell you of that later, perhaps."

[15823 op=2 display+exit]
  1: "Oh, why bother with such a boring subject?"
  2: "How about a @song instead?"

[15830 op=2 display+exit]
  1: You don't have that much.

[15836 op=2 display+exit]
  1: "As you wish."
  2: "Ok, here we go!"
  3: "The dragon is a scaly sort
  4: Who spouts flame when he sneezes.
  5: Many a bold knight has he fought,
  6: For he goes where e'er he pleases."
  7: "Mister Porcupine's naught but spines,
  8: To the dismay of him and his missus.
  9: For that for which he truly pines
  10: Is one of her sweet kisses."
  11: "A gypsy girl and a butterfly
  12: Were out in the woods at play
  13: And when I saw them I said 'Oh my!
  14: What a perfect sunny day!'"

[15838 op=2 display+exit]
  1: "Thank you kindly."
  2: "Ok, here we go!"
  3: "The dragon is a scaly sort
  4: Who spouts flame when he sneezes.
  5: Many a bold knight has he fought,
  6: For he goes where e'er he pleases."
  7: "Mister Porcupine's naught but spines,
  8: To the dismay of him and his missus.
  9: For that for which he truly pines
  10: Is one of her sweet kisses."
  11: "A gypsy girl and a butterfly
  12: Were out in the woods at play
  13: And when I saw them I said 'Oh my!
  14: What a perfect sunny day!'"

[15845 op=2 display+exit]
  1: You don't have that much.

[15852 op=2 display+exit]
  1: "As you wish."
  2: Mandrake tells you tales for quite a while, yet listening to them you lose track of time.
  3: He tells you of ships sailing off the edge of the world,
  4: of earthquakes, and mad wizards, and fierce battles.
  5: He speaks of spells that halt the flow of time,
  6: a man who found a way to fly, and the strange secret of the wisps.
  7: He tells you that there are two bands of gypsies who travel the King's Way,
  8: who are often mistaken for each other.
  9: He speaks of the pirate ship Empire, which was much dreaded in its day,
  10: and of the giant ants that can be found in the Dry Land.
  11: Your head is filled with wondrous visions.
  12: But finally he notices that Dupre has begun to doze off, and decides to bring his tales to an end.

[15855 op=2 display+exit]
  1: "You're too kind."
  2: Mandrake tells you tales for quite a while, yet listening to them you lose track of time.
  3: He tells you of ships sailing off the edge of the world,
  4: of earthquakes, and mad wizards, and fierce battles.
  5: He speaks of spells that halt the flow of time,
  6: a man who found a way to fly, and the strange secret of the wisps.
  7: He tells you that there are two bands of gypsies who travel the King's Way,
  8: who are often mistaken for each other.
  9: He speaks of the pirate ship Empire, which was much dreaded in its day,
  10: and of the giant ants that can be found in the Dry Land.
  11: Your head is filled with wondrous visions.
  12: But finally he notices that Dupre has begun to doze off, and decides to bring his tales to an end.

[15858 op=2 display+exit]
  1: "As you wish."
  2: Mandrake tells you tales for quite a while, yet listening to them you lose track of time.
  3: He tells you of ships sailing off the edge of the world,
  4: of earthquakes, and mad wizards, and fierce battles.
  5: He speaks of spells that halt the flow of time,
  6: a man who found a way to fly, and the strange secret of the wisps.
  7: He tells you that there are two bands of gypsies who travel the King's Way,
  8: who are often mistaken for each other.
  9: He speaks of the pirate ship Empire, which was much dreaded in its day,
  10: and of the giant ants that can be found in the Dry Land.
  11: Your head is filled with wondrous visions.
  12: But finally his tales come to an end.

[15860 op=2 display+exit]
  1: "You're too kind."
  2: Mandrake tells you tales for quite a while, yet listening to them you lose track of time.
  3: He tells you of ships sailing off the edge of the world,
  4: of earthquakes, and mad wizards, and fierce battles.
  5: He speaks of spells that halt the flow of time,
  6: a man who found a way to fly, and the strange secret of the wisps.
  7: He tells you that there are two bands of gypsies who travel the King's Way,
  8: who are often mistaken for each other.
  9: He speaks of the pirate ship Empire, which was much dreaded in its day,
  10: and of the giant ants that can be found in the Dry Land.
  11: Your head is filled with wondrous visions.
  12: But finally his tales come to an end.

```

<a id="manrel-npc-38"></a>
## Manrel ? NPC 38, port 39

Instruction range: 3492?3555

```text
[3496 op=1 keyword; failure -> 3498]
  1: look

[3497 op=2 display+exit]
  1: a pipe-smoking gentleman with the symbol of the Codex @tattooed on his forehead.
  2: "Greetings to you, $G."

[3498 op=1 keyword; failure -> 3500]
  1: name

[3499 op=2 display+exit]
  1: "My name is Manrel."

[3500 op=1 keyword; failure -> 3502]
  1: job

[3501 op=2 display+exit]
  1: "I'm a @woodworker. I also dabble in @alchemy from time to time."

[3502 op=1 keyword; failure -> 3504]
  1: wood

[3503 op=2 display+exit]
  1: "I make furniture, cabinets, walking sticks, and the like."
  2: "Perhaps I can make something for you someday."

[3504 op=1 keyword; failure -> 3506]
  1: alch,dabb

[3505 op=2 display+exit]
  1: "I'm trying to learn how to make healing potions, but I don't have the formula right yet."
  2: "I did stumble across a wicked recipe for red @mead, though."

[3506 op=1 keyword; failure -> 3508]
  1: red,mead

[3507 op=2 display+exit]
  1: "Alas, there's little call for my brew in this town, with the Blue @Bottle serving Frasier's @Folly."

[3508 op=1 keyword; failure -> 3510]
  1: blue,bott

[3509 op=2 display+exit]
  1: "A fine establishment."

[3510 op=1 keyword; failure -> 3512]
  1: fras,foll

[3511 op=2 display+exit]
  1: "One of the finest @libations you'll find anywhere."

[3512 op=1 keyword; failure -> 3514]
  1: liba

[3513 op=2 display+exit]
  1: "Libation or liability - take your pick."

[3514 op=1 keyword; failure -> 3516]
  1: tatt,code,symb

[3515 op=2 display+exit]
  1: "I went drinking with some friends the night we heard of the discovery of the Codex."
  2: "When I woke up the next morning, I had this tattoo..."

[3516 op=1 keyword; failure -> 3518]
  1: flow

[3517 op=2 display+exit]
  1: "@Daffodils were always his favorites... It's too bad I couldn't find any."

[3518 op=1 keyword; failure -> 3520]
  1: daff

[3519 op=2 display+exit]
  1: "Yes, daffodils."

[3521 op=1 keyword; failure -> 3523]
  1: beyv,lock,key,door,cryp

[3522 op=2 display+exit]
  1: "I hope you got a chance to deliver the @flowers."

[3523 op=1 keyword; failure -> 3527]
  1: beyv,lock,key,door,cryp

[3526 op=2 display+exit]
  1: "Oh, have you been to visit my cousin's grave?"

[3527 op=1 keyword; failure -> 3530]
  1: bye

[3529 op=2 display+exit]
  1: "I hope you got a chance to deliver the @flowers."

[3530 op=1 keyword; failure -> 3533]
  1: bye

[3532 op=2 display+exit]
  1: "Take care, my dear. I hope I'll see you again soon."

[3533 op=1 keyword; failure -> 3535]
  1: bye

[3534 op=2 display+exit]
  1: "It was a pleasure speaking with you."

[3535 op=2 display+exit]
  1: "I can't help you with that."

[3536 op=1 keyword; failure -> 3539]
  1: yes

[3538 op=2 display+exit]
  1: He looks at you suspiciously.
  2: "You wouldn't be wanting the key to his crypt, would you?"

[3540 op=2 display+exit]
  1: "My mistake."

[3542 op=1 keyword; failure -> 3544]
  1: yes

[3544 op=2 display+exit]
  1: "Well, alright then."

[3548 op=2 display+exit]
  1: "You're carrying too much right now. Come back for it later."

[3553 op=2 display+exit]
  1: "You're carrying too much right now. Come back for it later."

[3555 op=2 display+exit]
  1: "Well, perhaps you have your reasons... Okay, you can have the key."
  2: "But you have to take these @flowers and leave them there."

```

<a id="mariah-npc-31"></a>
## Mariah ? NPC 31, port 32

Instruction range: 2609?2694

```text
[2612 op=1 keyword; failure -> 2614]
  1: slee

[2615 op=1 keyword; failure -> 2617]
  1: illu

[2618 op=1 keyword; failure -> 2620]
  1: wind

[2621 op=1 keyword; failure -> 2623]
  1: dang,trap.harm

[2624 op=1 keyword; failure -> 2626]
  1: magi

[2627 op=2 display+exit]
  1: "That's right! Now then, what can I help you with?"

[2630 op=2 display+exit]
  1: "No, that's not it."
  2: "Haven't you a Compendium of knowledge to draw on?"
  3: "I'd suggest you find a Compendium to study, then come talk to me again."

[2635 op=1 keyword; failure -> 2638]
  1: look

[2637 op=2 display+exit]
  1: a freckled young lady with an enchanting smile.
  2: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  3: "So I have to ask you a question to test you..."
  4: "What does the magic syllable 'Zu' mean?"

[2638 op=1 keyword; failure -> 2641]
  1: look

[2640 op=2 display+exit]
  1: a freckled young lady with an enchanting smile.
  2: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  3: "So I have to ask you a question to test you..."
  4: "What does the magic syllable 'Quas' mean?"

[2641 op=1 keyword; failure -> 2644]
  1: look

[2643 op=2 display+exit]
  1: a freckled young lady with an enchanting smile.
  2: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  3: "So I have to ask you a question to test you..."
  4: "What does the magic syllable 'Hur' mean?"

[2644 op=1 keyword; failure -> 2647]
  1: look

[2646 op=2 display+exit]
  1: a freckled young lady with an enchanting smile.
  2: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  3: "So I have to ask you a question to test you..."
  4: "What does the magic syllable 'Jux' mean?"

[2647 op=1 keyword; failure -> 2650]
  1: look

[2649 op=2 display+exit]
  1: a freckled young lady with an enchanting smile.
  2: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  3: "So I have to ask you a question to test you..."
  4: "What does the magic syllable 'Ort' mean?"

[2651 op=2 display+exit]
  1: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  2: "So I have to ask you a question to test you..."
  3: "What does the magic syllable 'Zu' mean?"

[2653 op=2 display+exit]
  1: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  2: "So I have to ask you a question to test you..."
  3: "What does the magic syllable 'Quas' mean?"

[2655 op=2 display+exit]
  1: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  2: "So I have to ask you a question to test you..."
  3: "What does the magic syllable 'Hur' mean?"

[2657 op=2 display+exit]
  1: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  2: "So I have to ask you a question to test you..."
  3: "What does the magic syllable 'Jux' mean?"

[2659 op=2 display+exit]
  1: "I'm sorry to put you to the trouble, but I'm only supposed to help those who know the ways of magic."
  2: "So I have to ask you a question to test you..."
  3: "What does the magic syllable 'Ort' mean?"

[2660 op=1 keyword; failure -> 2662]
  1: look

[2661 op=2 display+exit]
  1: a freckled young lady with an enchanting smile.
  2: "Good $T, $G, can I help thee?"

[2662 op=1 keyword; failure -> 2665]
  1: name

[2664 op=2 display+exit]
  1: "They call me Mariah."

[2665 op=1 keyword; failure -> 2667]
  1: job

[2666 op=2 display+exit]
  1: "I'm the newest member of the @Council of Wizards. Somebody has to keep things running smoothly."
  2: "In my spare time I use the @library to pursue my own studies."

[2667 op=1 keyword; failure -> 2669]
  1: coun,wiza

[2668 op=2 display+exit]
  1: "They get so caught up in their work sometimes that they forget even to eat and @sleep!"

[2669 op=1 keyword; failure -> 2671]
  1: eat,slee

[2670 op=2 display+exit]
  1: "Such things matter little to them. They often dwell apart from the realm of the @senses."

[2671 op=1 keyword; failure -> 2673]
  1: dwel,apar,real,sens

[2672 op=2 display+exit]
  1: "Aye, indeed."

[2673 op=1 keyword; failure -> 2675]
  1: libr,stud

[2674 op=2 display+exit]
  1: "'Tis the finest library in all Britannia."
  2: "I study many different things, but I find learning old languages the most interesting."
  3: "One can learn so much about how our own language developed!"

[2678 op=1 keyword; failure -> 2682]
  1: half,tabl,anci,silv,deci,book

[2681 op=2 display+exit]
  1: "With both pieces of the tablet, I should be able to work out a translation."
  2: She studies the tablet for a while, and then starts reading, haltingly, from the book.
  3: An ancient prophecy tells of the final days, when the end of our world shall come.
  4: Three signs will precede the end.
  5: Thrice shall a being of great evil come unto our land, and by this it shall be known that the end is nigh.
  6: This evil one is of another race, who consider the evil one a great prophet.
  7: Yet this false prophet follows not the principles of Control, Passion and Diligence.
  8: One day the false prophet will come and desecrate our most holy shrine.
  9: And the false prophet will steal our most holy artifact, the Codex of Ultimate Wisdom.
  10: This shall be the first sign of the end.
  11: Then, it is written, the false prophet shall descend deep into the bowels of the earth.
  12: And the false prophet will cause the underworld to collapse.
  13: This will cause great earthquakes to tear our world asunder, and there will be a time of plague and famine.
  14: This shall be the second sign of the end.
  15: One last time shall the false prophet come.
  16: This time, the false prophet will come with a band of warriors.
  17: And they will destroy all that remains of the gargoyle race.
  18: There is only one way that this prophecy may be averted:
  19: That is by the sacrifice of the false prophet.
  20: Mariah pauses for a while, deep in thought.
  21: "When they refer to 'the false prophet,' they must mean you." she says.
  22: "It would seem that they have reason to think you evil, from their perspective."
  23: "This is a difficult matter to resolve."
  24: "I think you'd better seek out more information."
  25: "There's a gargoyle named Sin'Vraal living out in the desert."
  26: "He speaks our language."
  27: "Perhaps he could tell you more about this book, and of how the gargoyles view us."

[2682 op=1 keyword; failure -> 2685]
  1: half,tabl,anci,silv,deci,book

[2684 op=2 display+exit]
  1: "Ah, thou hast found the rest of the tablet! Go get the other half from my study and bring it here."
  2: "With the two of them together I should be able to tell you more."

[2685 op=1 keyword; failure -> 2687]
  1: half,tabl,anci,silv,deci

[2686 op=2 display+exit]
  1: "I must have both halves of the tablet before I can tell you any more."
  2: "This piece I have had for quite some time, and I've learned all I can from it."
  3: "I got my piece of the tablet from some gypsies whom I met at a pub."
  4: "Mayhap they can tell thee where to look for the other piece."
  5: "Bring both pieces of the tablet here and I will tell thee what I can."

[2687 op=1 keyword; failure -> 2689]
  1: book

[2689 op=1 keyword; failure -> 2691]
  1: bye

[2690 op=2 display+exit]
  1: "Fare thee well."

[2691 op=2 display+exit]
  1: "I cannot help thee with that."

[2693 op=2 display+exit]
  1: "Nystul sent word to me that thou hast a book written in a strange tongue."
  2: "I'd be glad to look at it for thee."
  3: She examines the book.
  4: "Oh! I've seen this script before!"
  5: "I have part of an ancient silver tablet in my study with writing both in our language and this."
  6: "I have learned only a little from studying it, but I can make out a few words."
  7: "The title is 'The Book of Prophecies,' and it says something about 'the end of our world.'"
  8: "If only I had the other half of the @tablet, perhaps I'd be able to decipher the rest of the book."
  9: "With both pieces of the tablet, I should be able to work out a translation."

[2694 op=2 display+exit]
  1: "Nystul sent word to me that thou hast a book written in a strange tongue."
  2: "I'd not mind looking at it for thee..."
  3: "But I'd find the task much easier if thou wouldst bring it here!"

```

<a id="marissa-npc-100"></a>
## Marissa ? NPC 100, port 101

Instruction range: 9297?9462

```text
[9301 op=1 keyword; failure -> 9306]
  1: tuni

[9305 op=2 display+exit]
  1: "It'll cost you 15 gold for that tunic."
  2: "Interested?"

[9306 op=1 keyword; failure -> 9311]
  1: dres

[9310 op=2 display+exit]
  1: "It'll cost you 30 gold for that dress."
  2: "Interested?"

[9311 op=1 keyword; failure -> 9316]
  1: pant

[9315 op=2 display+exit]
  1: "It'll cost you 20 gold for those pants."
  2: "Interested?"

[9316 op=2 display+exit]
  1: "Perhaps something else then."

[9320 op=1 keyword; failure -> 9322]
  1: yes

[9322 op=2 display+exit]
  1: "Suit yourself," she sniffs, offended.

[9325 op=2 display+exit]
  1: She looks at you.
  2: "How rude! Don't ask for something you can't pay for!"

[9327 op=2 display+exit]
  1: "You look pretty full to me."

[9329 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, she hands over the tunic.

[9331 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, she hands over the dress.

[9332 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, she hands over the pants.

[9334 op=1 keyword; failure -> 9337]
  1: yes

[9336 op=2 display+exit]
  1: "Very well, how many pieces do you wish to give me?"

[9339 op=2 display+exit]
  1: "Just as well."
  2: "That cloth is rather shabby..."

[9344 op=2 display+exit]
  1: "Just as well."
  2: "That cloth is rather shabby..."

[9346 op=2 display+exit]
  1: "I see, well I only need 5."

[9360 op=2 display+exit]
  1: "I don't see enough cloth, other than what you're wearing!"

[9363 op=2 display+exit]
  1: "Done!"
  2: She hands you $V1 gold and takes the cloth.

[9364 op=1 keyword; failure -> 9369]
  1: tuni

[9368 op=2 display+exit]
  1: "It'll cost you 15 gold for that tunic."
  2: "Interested?"

[9369 op=1 keyword; failure -> 9374]
  1: dres

[9373 op=2 display+exit]
  1: "It'll cost you 30 gold for that dress."
  2: "Interested?"

[9374 op=1 keyword; failure -> 9379]
  1: pant

[9378 op=2 display+exit]
  1: "It'll cost you 20 gold for those pants."
  2: "Interested?"

[9379 op=1 keyword; failure -> 9382]
  1: buy,clot

[9381 op=2 display+exit]
  1: "Which one, @tunic, @dress or @pants?"

[9383 op=1 keyword; failure -> 9392]
  1: yes

[9391 op=2 display+exit]
  1: She takes the silk cloth and begins to work, following the balloon plans.
  2: She works in silence, intent on her sewing.
  3: After a while, she completes the silk bag.
  4: "There, 'tis done!" she cries, handing you the bag.

[9393 op=1 keyword; failure -> 9396]
  1: yes

[9395 op=2 display+exit]
  1: "Come back when you have the gold, $Y."

[9398 op=2 display+exit]
  1: "Very well," she sniffs, offended.

[9399 op=1 keyword; failure -> 9403]
  1: look

[9402 op=2 display+exit]
  1: You see a finely adorned woman with a tight mouth and wide eyes.
  2: "Hello." She bows formally.

[9403 op=1 keyword; failure -> 9406]
  1: look

[9405 op=2 display+exit]
  1: a finely adorned woman with a tight mouth and wide eyes.
  2: "Hello again." She bows formally.

[9406 op=1 keyword; failure -> 9409]
  1: name

[9408 op=2 display+exit]
  1: "Marissa."
  2: "You may call me '@Miss @Trihune."

[9409 op=1 keyword; failure -> 9411]
  1: miss,trih

[9410 op=2 display+exit]
  1: "Yes, that is much better."

[9413 op=1 keyword; failure -> 9415]
  1: buy,tuni,dres,pant,clot

[9414 op=2 display+exit]
  1: She bows stiffly. "I'm sorry, good sir, but my shop is closed for now."
  2: "See me some other time, when I am available for business."

[9416 op=1 keyword; failure -> 9418]
  1: buy,tuni,dres,pant,clot

[9417 op=2 display+exit]
  1: She bows stiffly. "I'm sorry, good madam, but my shop is closed for now."
  2: "See me some other time, when I am available for business."

[9418 op=1 keyword; failure -> 9428]
  1: sell

[9428 op=1 keyword; failure -> 9432]
  1: sell

[9431 op=2 display+exit]
  1: She turns to you and asks "Will you take $V1 gold for that cloth?"

[9432 op=1 keyword; failure -> 9434]
  1: sell

[9433 op=2 display+exit]
  1: "I don't see any cloth, other than what you're wearing!"

[9434 op=1 keyword; failure -> 9436]
  1: silk

[9435 op=2 display+exit]
  1: "Yes, I can work with silk. But 'tis out of fashion..."
  2: "Nobody wears silk clothes this season."

[9436 op=1 keyword; failure -> 9438]
  1: arbe

[9437 op=2 display+exit]
  1: "Oh, he's a threadmaker who lives here in town."
  2: "Silly little man, but a fine spinner."
  3: "I get all my thread straight from him. No other source would do."

[9438 op=1 keyword; failure -> 9440]
  1: bolt

[9439 op=2 display+exit]
  1: "Yes, that silly Arbeth person can spin spidersilk into thread."
  2: "Charlotte, from New Magincia, weaves thread into silk cloth."
  3: "Bring me a bolt of her finest silk cloth, and then I can sew you a silk bag."

[9440 op=1 keyword; failure -> 9443]
  1: ball,mamm,bag,plan

[9442 op=2 display+exit]
  1: "A large silk bag."
  2: "But 'tis hardly a challenge, I've already made one for you!"

[9443 op=1 keyword; failure -> 9448]
  1: ball,mamm,bag,plan

[9447 op=2 display+exit]
  1: "Oh, I understand."
  2: "A large silk bag."
  3: "Well, 'tis hardly a challenge for one of my skills."
  4: "But, since it is important, I'll do it."
  5: "But it will cost you 75 gold."
  6: "Alright?"

[9448 op=1 keyword; failure -> 9451]
  1: ball,mamm,bag,plan

[9450 op=2 display+exit]
  1: "Oh, I understand."
  2: "A large silk bag."
  3: "Well, 'tis hardly a challenge for one of my skills."
  4: "But, since it is important, I'll do it."
  5: "But I'll need a @bolt of silk to do it, of course."

[9451 op=1 keyword; failure -> 9453]
  1: ball,mamm,bag,plan

[9452 op=2 display+exit]
  1: "Oh, I understand."
  2: "A large silk bag."
  3: "Well, 'tis hardly a challenge for one of my skills."
  4: "But, since it is important, I'll do it."
  5: "But I'll need to see the plans."

[9453 op=1 keyword; failure -> 9455]
  1: job

[9454 op=2 display+exit]
  1: "I create garments of the highest @fashion."

[9455 op=1 keyword; failure -> 9457]
  1: high,fash

[9456 op=2 display+exit]
  1: "Clothes fit to see the King in."

[9457 op=1 keyword; failure -> 9459]
  1: bye

[9458 op=2 display+exit]
  1: "Yes, well, come see me again." She yawns widely.

[9461 op=2 display+exit]
  1: "Hmph!"
  2: "I am not a gossip, $G."

[9462 op=2 display+exit]
  1: "'Tis not my place to speak of that..."

```

<a id="marney-npc-87"></a>
## Marney ? NPC 87, port 88

Instruction range: 8476?8528

```text
[8476 op=1 keyword; failure -> 8479]
  1: look

[8478 op=2 display+exit]
  1: a tall, delicate looking female.
  2: Her smile puts you at ease.
  3: Her eyes are as blue as the ocean and her movements as rythmic as the tide.
  4: "I recognize you from before."
  5: She winks at you.
  6: "What can I do for you?"

[8479 op=1 keyword; failure -> 8482]
  1: look

[8481 op=2 display+exit]
  1: a tall, delicate looking female.
  2: Her smile puts you at ease.
  3: Her eyes are as blue as the ocean and her movements as rythmic as the tide.
  4: "Hello $G, welcome to my home."

[8482 op=1 keyword; failure -> 8484]
  1: name

[8483 op=2 display+exit]
  1: "My father chose an ancient word for my name."
  2: "It means the cool breeze after a storm has passed."
  3: "@Marney."

[8484 op=1 keyword; failure -> 8486]
  1: job

[8485 op=2 display+exit]
  1: "I look after the @dock."

[8486 op=1 keyword; failure -> 8488]
  1: dock,skif

[8487 op=2 display+exit]
  1: "Oh, you're probably wanting a skiff to get out to old Horance's island."
  2: "Well, we have none for sale, but feel free to borrow the one at the dock."
  3: "Be sure to return it, though!"

[8488 op=1 keyword; failure -> 8490]
  1: marn

[8489 op=2 display+exit]
  1: "It is in a @poem my mother wrote to him when they first met."
  2: She grabs a @locket that hangs around her neck and opens it.
  3: Inside is a folded piece of cloth with words printed on it.
  4: She begins to read.
  5: "At first we met and dark clouds gathered, the thunder sounding your approach."
  6: "Then comes the storm of our hearts pounding and in the end the marney's touch."

[8490 op=1 keyword; failure -> 8492]
  1: poem

[8491 op=2 display+exit]
  1: "Certainly you can look at it."
  2: She hands the tiny cloth to you.
  3: Looking at it, you discover that she has read only the first verse of the poem.
  4: The second reads, "If I should leave and you are lonely,"
  5: "Remember where my love is kept:"
  6: "Inside a @basket made of promise,"
  7: "You'll find my dreams of us have slept."

[8492 op=1 keyword; failure -> 8494]
  1: bask

[8493 op=2 display+exit]
  1: "A basket?"
  2: "I have one in my hope chest."
  3: "Feel free to look for yourself."

[8494 op=1 keyword; failure -> 8496]
  1: lock,amul

[8495 op=2 display+exit]
  1: "My mother gave me this just before she was taken by those evil men."
  2: "My mother told me to give it to father when he returned from the @sea."
  3: "Yorl gave it back to me after he was @killed.

[8496 op=1 keyword; failure -> 8498]
  1: sea

[8497 op=2 display+exit]
  1: "My father had gone out to help @Yorl bring in the catch that afternoon."

[8498 op=1 keyword; failure -> 8500]
  1: rune

[8499 op=2 display+exit]
  1: "I am sorry for our town."
  2: "You see, my father was the caretaker of the rune stone."
  3: "After he died, we had no way of finding out where he put it."

[8500 op=1 keyword; failure -> 8502]
  1: ghos

[8501 op=2 display+exit]
  1: She bows her head a moment and with a shaky voice begins to speak.
  2: "It is something I care not to talk about."
  3: "His memory brings only sadness to me."

[8502 op=1 keyword; failure -> 8504]
  1: quen,fath

[8503 op=2 display+exit]
  1: "My father?"
  2: "He was a brave man and very strong."
  3: "That he died in such a violent way was so wrong."
  4: "But I am sure there is a reason for it."
  5: Her blue eyes glisten with new-formed tears.

[8504 op=1 keyword; failure -> 8506]
  1: murd,kill,die

[8505 op=2 display+exit]
  1: She rubs her nose with the knuckle of her hand.
  2: "I can not see why those beasts would kill him, but perhaps it was just unfortunate."
  3: "Still, I can't see why he was at the inn that late."
  4: "Had he been drinking, @Yorl would have been with him."

[8506 op=1 keyword; failure -> 8508]
  1: yorl

[8507 op=2 display+exit]
  1: "Oh, Yorl."
  2: She smiles to herself.
  3: "He and father were never apart."
  4: "After my mother was kidnapped, Yorl helped my father get back on his feet."
  5: "They were always together."
  6: Her gaze returns to you.
  7: "If my @father had been drinking at the @inn, Yorl would have been there."

[8508 op=1 keyword; failure -> 8510]
  1: haun,inn

[8509 op=2 display+exit]
  1: "My father and Yorl would go there after a good catch."
  2: "@Gideon had been a friend of theirs since youth."
  3: "The three would would drink and reminisce together."

[8510 op=1 keyword; failure -> 8512]
  1: garg,beas

[8511 op=2 display+exit]
  1: She looks at you and swallows.
  2: "I cannot talk about these creatures."

[8512 op=1 keyword; failure -> 8514]
  1: tren,bell

[8513 op=2 display+exit]
  1: "I believe he has done all he can for me."
  2: "When my father was murdered, he took charge to find the gargoyles."
  3: "None were found though."

[8514 op=1 keyword; failure -> 8516]
  1: mich

[8515 op=2 display+exit]
  1: Her eyes brighten at the mention of his name.
  2: "He is a very handsome man."
  3: "I see him only occasionally when he comes into town."
  4: She looks absently out the window.
  5: "Yorl would never let me see him though."

[8516 op=1 keyword; failure -> 8518]
  1: gide

[8517 op=2 display+exit]
  1: "He and his wife are very kind to me."
  2: "They have taught me so much about how to treat others."

[8518 op=1 keyword; failure -> 8520]
  1: stiv

[8519 op=2 display+exit]
  1: "He is a sweet little man."
  2: "He brought me wine on my sixteenth birthday."
  3: "I'm quite fond of him."

[8520 op=1 keyword; failure -> 8522]
  1: deza

[8521 op=2 display+exit]
  1: "She has done as much as she can to heal my wounded heart."
  2: "She bakes for me and we have long talks about almost everything."
  3: "I wish I were as beautiful as she."

[8522 op=1 keyword; failure -> 8524]
  1: hora

[8523 op=2 display+exit]
  1: "Ah, now, Horance is much wiser than most know."
  2: "He is also my mentor when it comes to poetry."
  3: "He has a way of reminding me that I am too concerned with myself."
  4: "It is a gentle way, but a firm reminder."

[8524 op=1 keyword; failure -> 8526]
  1: lord,brit

[8525 op=2 display+exit]
  1: "I saw him once when I was a little girl."
  2: "We had gone to his castle for a celebration and there he sat on a great white charger."
  3: "What a sight he was."

[8526 op=1 keyword; failure -> 8528]
  1: bye

[8527 op=2 display+exit]
  1: "I am sorry that I wept."
  2: "It is not your fault."
  3: "Goodbye."

[8528 op=2 display+exit]
  1: As you ask this, she begins to cry.
  2: Gathering her strength she looks at you.
  3: "Sorry?"

```

<a id="marta-npc-83"></a>
## Marta ? NPC 83, port 84

Instruction range: 7906?7942

```text
[7906 op=1 keyword; failure -> 7909]
  1: look

[7908 op=2 display+exit]
  1: a rather plump woman with darting eyes.
  2: "Hello again."
  3: "Are you well this $T?"
  4: "What is it you need?"

[7909 op=1 keyword; failure -> 7912]
  1: look

[7911 op=2 display+exit]
  1: a rather plump woman with darting eyes.
  2: "Hello."
  3: "How are you this $T?"

[7912 op=1 keyword; failure -> 7914]
  1: name

[7913 op=2 display+exit]
  1: "Marta."

[7914 op=1 keyword; failure -> 7916]
  1: mart

[7915 op=2 display+exit]
  1: "It is a plain name, but I must live with it."

[7916 op=1 keyword; failure -> 7918]
  1: job

[7917 op=2 display+exit]
  1: "Oh dear, my husband runs the inn."
  2: "I tend my @garden."

[7918 op=1 keyword; failure -> 7920]
  1: gard

[7919 op=2 display+exit]
  1: "You've not seen it?"
  2: "I have the loveliest @roses."

[7920 op=1 keyword; failure -> 7922]
  1: rose

[7921 op=2 display+exit]
  1: "Red as the evening sun on the sea."

[7922 op=1 keyword; failure -> 7924]
  1: yorl

[7923 op=2 display+exit]
  1: "He's a messy person - and smelly too."

[7924 op=1 keyword; failure -> 7926]
  1: dela,marn

[7925 op=2 display+exit]
  1: "My dear little one?"
  2: "She is an angel - well minded, kind, thoughtful."
  3: "She's a dear."

[7926 op=1 keyword; failure -> 7928]
  1: garg

[7927 op=2 display+exit]
  1: "Nasty creatures, or so I hear."
  2: "Can stomp a garden to pieces in minutes."

[7928 op=1 keyword; failure -> 7930]
  1: gide

[7929 op=2 display+exit]
  1: "My husband talks a bit too much, but I love him."

[7930 op=1 keyword; failure -> 7932]
  1: haun

[7931 op=2 display+exit]
  1: "I want to move from this house as soon as we can."

[7932 op=1 keyword; failure -> 7934]
  1: deza

[7933 op=2 display+exit]
  1: "I cut myself one time in the garden."
  2: "She healed it."

[7934 op=1 keyword; failure -> 7936]
  1: hora

[7935 op=2 display+exit]
  1: "Another strange man."
  2: "We have so many here."

[7936 op=1 keyword; failure -> 7938]
  1: mich

[7937 op=2 display+exit]
  1: "He never comes into town."

[7938 op=1 keyword; failure -> 7940]
  1: ghos

[7939 op=2 display+exit]
  1: "It was the scariest thing I ever saw!"
  2: "I'm sure it's still lurking about somewhere..."

[7940 op=1 keyword; failure -> 7942]
  1: bye

[7941 op=2 display+exit]
  1: "Goodbye."

[7942 op=2 display+exit]
  1: "You will have to ask another that."

```

<a id="martin-npc-50"></a>
## Martin ? NPC 50, port 51

Instruction range: 4433?4463

```text
[4433 op=1 keyword; failure -> 4437]
  1: look

[4436 op=2 display+exit]
  1: a jolly, red-faced innkeeper.
  2: "Vell, hello dere, mine friend!"
  3: "Velcome to the Varrior's Stead!"
  4: "Vat can I be doing for thee this $T?"

[4437 op=1 keyword; failure -> 4439]
  1: look

[4438 op=2 display+exit]
  1: a jolly, red-faced innkeeper.
  2: "Vell, hello again, mine friend!"
  3: "Velcome to the Varrior's Stead!"
  4: "Vat can I be doing for thee this $T?"

[4439 op=1 keyword; failure -> 4441]
  1: name

[4440 op=2 display+exit]
  1: "Mine name ist Martin, but my friends call me '@Dutch.'"

[4441 op=1 keyword; failure -> 4443]
  1: dutc

[4442 op=2 display+exit]
  1: "Ja, dat's mine name!"

[4443 op=1 keyword; failure -> 4445]
  1: job

[4444 op=2 display+exit]
  1: "I run dis @inn."

[4445 op=1 keyword; failure -> 4448]
  1: rest,inn,room

[4447 op=2 display+exit]
  1: "Room und board ist 5 gold per night per person."
  2: "Interested?"

[4449 op=1 keyword; failure -> 4451]
  1: yes

[4452 op=1 keyword; failure -> 4454]
  1: no

[4453 op=2 display+exit]
  1: "Vell, perhaps some other time, ja?"

[4455 op=1 keyword; failure -> 4457]
  1: bye

[4456 op=2 display+exit]
  1: "Haf a good journey!" he calls as you leave.

[4457 op=2 display+exit]
  1: "I speak your language not so good."
  2: "Vat is meaning '$Z'?"

[4461 op=2 display+exit]
  1: "Ach, I'm sorry, but I must haf my 5 gold."
  2: "Come back ven fortune makes you rich, ah?"

[4463 op=2 display+exit]
  1: He shows you to your room, and you drift off into a well-earned sleep.
  2: "Goodnight!"

```

<a id="matt-npc-12"></a>
## Matt ? NPC 12, port 13

Instruction range: 943?950

```text
[943 op=1 keyword; failure -> 945]
  1: look

[944 op=2 display+exit]
  1: a gaunt fellow, with eyes that take in everything and reveal nothing.

[947 op=22 display+continue]
  1: When you address him, the man replies with gestures that appear to be some kind of sign language.
  2: When he sees that you can't understand him, he shrugs his shoulders and returns to his duties.

[949 op=2 display+exit]
  1: Dupre puts his hand on your shoulder and whispers
  2: "Matt here had a most unfortunate accident as a child, and it left him deaf and dumb.
  3: I'd suggest you ask his wife instead, $P."

[950 op=2 display+exit]
  1: When you address him, the man replies with gestures that appear to be some kind of sign language.
  2: When he sees that you can't understand him, he shrugs his shoulders and returns to his duties.

```

<a id="max-npc-16"></a>
## Max ? NPC 16, port 17

Instruction range: 1275?1552

```text
[1275 op=1 keyword; failure -> 1277]
  1: look

[1276 op=2 display+exit]
  1: a swarthy blacksmith.
  2: "Hey, it'sa you. Hello again."
  3: "What'sa you wanta nexta?"

[1277 op=1 keyword; failure -> 1280]
  1: name

[1279 op=2 display+exit]
  1: "I'ma nameda Max."

[1280 op=1 keyword; failure -> 1282]
  1: job

[1281 op=2 display+exit]
  1: "Whassamattayu, you blind or somethin'?"
  2: "I makea de swords and stuff, what's it alook like I'ma doin' here?"
  3: "You gotta somethin' to @sell oda somethin' you wanna @buy?"
  4: "If you gotta lots to sell, you sell in @bulk."

[1282 op=1 keyword; failure -> 1284]
  1: bye

[1283 op=2 display+exit]
  1: "Goodbyea to you. Come abacka soon, ah?"

[1286 op=1 keyword; failure -> 1288]
  1: buy

[1287 op=2 display+exit]
  1: "Do you wanta @weapons or @armor?"

[1288 op=1 keyword; failure -> 1290]
  1: weap

[1289 op=2 display+exit]
  1: "Awhicha one adese, @dagger, @spear, @sword or @2 @handed @sword?"

[1290 op=1 keyword; failure -> 1292]
  1: armo

[1291 op=2 display+exit]
  1: "Awhicha one adese, @cloth @armour, @chain @mail, @heater @shield, @iron @helm, @leather @armour or @plate @mail?"

[1292 op=1 keyword; failure -> 1294]
  1: sell

[1294 op=1 keyword; failure -> 1296]
  1: bulk

[1298 op=1 keyword; failure -> 1301]
  1: dagg

[1300 op=2 display+exit]
  1: "That dagger, she'sa costa 10 agold."
  2: "Whatsa the numbah you need?"

[1301 op=1 keyword; failure -> 1304]
  1: spea

[1303 op=2 display+exit]
  1: "That spear, she'sa costa 20 agold."
  2: "Whatsa the numbah you need?"

[1304 op=1 keyword; failure -> 1307]
  1: swor

[1306 op=2 display+exit]
  1: "That sword, she'sa costa 50 agold."
  2: "Whatsa the numbah you need?"

[1307 op=1 keyword; failure -> 1310]
  1: 2han

[1309 op=2 display+exit]
  1: "That 2 handed sword, she'sa costa 90 agold."
  2: "Whatsa the numbah you need?"

[1310 op=1 keyword; failure -> 1313]
  1: clot

[1312 op=2 display+exit]
  1: "That cloth armour, she'sa costa 25 agold."
  2: "Whatsa the numbah you need?"

[1313 op=1 keyword; failure -> 1316]
  1: chai

[1315 op=2 display+exit]
  1: "That chain mail, she'sa costa 60 agold."
  2: "Whatsa the numbah you need?"

[1316 op=1 keyword; failure -> 1319]
  1: heat

[1318 op=2 display+exit]
  1: "That heater shield, she'sa costa 20 agold."
  2: "Whatsa the numbah you need?"

[1319 op=1 keyword; failure -> 1322]
  1: iron

[1321 op=2 display+exit]
  1: "That iron helm, she'sa costa 20 agold."
  2: "Whatsa the numbah you need?"

[1322 op=1 keyword; failure -> 1325]
  1: leat

[1324 op=2 display+exit]
  1: "That leather armour, she'sa costa 40 agold."
  2: "Whatsa the numbah you need?"

[1325 op=1 keyword; failure -> 1328]
  1: plat

[1327 op=2 display+exit]
  1: "That plate mail, she'sa costa 120 agold."
  2: "Whatsa the numbah you need?"

[1389 op=1 keyword; failure -> 1391]
  1: buy

[1390 op=2 display+exit]
  1: "You to my shopa when shesa open!"

[1391 op=1 keyword; failure -> 1393]
  1: weap

[1392 op=2 display+exit]
  1: "You to my shopa when shesa open!"

[1393 op=1 keyword; failure -> 1395]
  1: armo

[1394 op=2 display+exit]
  1: "You to my shopa when shesa open!"

[1395 op=1 keyword; failure -> 1397]
  1: sell

[1396 op=2 display+exit]
  1: "You to my shopa when shesa open!"

[1397 op=2 display+exit]
  1: "Hey, my hearing, she's notsa so good. What you say?"

[1400 op=2 display+exit]
  1: "I won't sell to you then!"

[1402 op=2 display+exit]
  1: "I needa some time to make that many!"

[1406 op=2 display+exit]
  1: "No gold! No goods!"

[1408 op=2 display+exit]
  1: "I'da sell it to you, but you don't hava the space."

[1409 op=2 display+exit]
  1: "There ya go."

[1416 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 5 gold for that dagger?"

[1422 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 10 gold for that spear?"

[1428 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 25 gold for that sword?"

[1434 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 45 gold for that 2 handed sword?"

[1440 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 12 gold for that cloth armour?"

[1446 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 30 gold for that chain mail?"

[1452 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 10 gold for that heater shield?"

[1458 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 10 gold for that iron helm?"

[1464 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 20 gold for that leather armour?"

[1470 op=2 display+exit]
  1: "Aye, you have asomething ofa interesta..."
  2: "Will you take 60 gold for that plate mail? "

[1473 op=2 display+exit]
  1: "Hey, atsa real shame, but anone of you hasa anything I'ma needing."

[1476 op=1 keyword; failure -> 1480]
  1: yes

[1479 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 5 gold pieces and takes the dagger.

[1481 op=1 keyword; failure -> 1485]
  1: yes

[1484 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 10 gold pieces and takes the spear.

[1486 op=1 keyword; failure -> 1490]
  1: yes

[1489 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 25 gold pieces and takes the sword.

[1491 op=1 keyword; failure -> 1495]
  1: yes

[1494 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 45 gold pieces and takes the 2 handed sword.

[1496 op=1 keyword; failure -> 1500]
  1: yes

[1499 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 12 gold pieces and takes the cloth Armour.

[1501 op=1 keyword; failure -> 1505]
  1: yes

[1504 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 30 gold pieces and takes the chain mail.

[1506 op=1 keyword; failure -> 1510]
  1: yes

[1509 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 10 gold pieces and takes the heater shield.

[1511 op=1 keyword; failure -> 1515]
  1: yes

[1514 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 10 gold pieces and takes the iron helm.

[1516 op=1 keyword; failure -> 1520]
  1: yes

[1519 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 20 gold pieces and takes the leather armour.

[1521 op=1 keyword; failure -> 1525]
  1: yes

[1524 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you 60 gold pieces and takes the plate mail.

[1525 op=2 display+exit]
  1: "Achanged your aminda, eh?"

[1539 op=2 display+exit]
  1: "Hey, atsa real shame, but anone of you hasa anything I'ma needing."

[1542 op=2 display+exit]
  1: "Holy'moley, you havea more inventory than I have!"
  2: "If you weren'ta the Avatar, I'da thought you lifted the Serpent's Hold!"
  3: "How's about $V0 gold for all'adat stuff?"

[1543 op=2 display+exit]
  1: "How's about $V0 gold for all'adat stuff?"

[1545 op=1 keyword; failure -> 1547]
  1: yes

[1547 op=2 display+exit]
  1: "Achanged your aminda, eh?"

[1550 op=2 display+exit]
  1: "Whassadis, you're tryin' to short-change 'ol Max?"
  2: I mighta talk funny, but I can'a still count!"

[1552 op=2 display+exit]
  1: "She'sa done!"
  2: He hands you $V0 gold pieces in exchange for the items.

```

<a id="merideth-npc-104"></a>
## Merideth ? NPC 104, port 105

Instruction range: 9795?9827

```text
[9795 op=1 keyword; failure -> 9798]
  1: look

[9797 op=2 display+exit]
  1: a little girl with a doll in her hands.
  2: "See, Becky, they've come back." she says to her doll.
  3: "What do you want?"

[9798 op=1 keyword; failure -> 9801]
  1: look

[9800 op=2 display+exit]
  1: a little girl with a doll in her hands.
  2: "Now you be still, they won't hurt us."
  3: She hugs her doll close.
  4: "What do you want?"

[9801 op=1 keyword; failure -> 9803]
  1: name

[9802 op=2 display+exit]
  1: "@Merideth Cassandra Lamby!"

[9803 op=1 keyword; failure -> 9805]
  1: meri,cass,lamb

[9804 op=2 display+exit]
  1: "I'm named after my great @aunt."

[9805 op=1 keyword; failure -> 9807]
  1: grea,aunt

[9806 op=2 display+exit]
  1: "She lives far away."

[9807 op=1 keyword; failure -> 9809]
  1: job

[9808 op=2 display+exit]
  1: "Oh, I don't work."
  2: "My @Memah can help you though."

[9809 op=1 keyword; failure -> 9811]
  1: mema

[9810 op=2 display+exit]
  1: "My grandma."
  2: "She's around here somewhere."

[9811 op=1 keyword; failure -> 9813]
  1: timo

[9812 op=2 display+exit]
  1: "He's a nice man."
  2: "He got me this doll."
  3: She displays it proudly.

[9813 op=1 keyword; failure -> 9815]
  1: shor,mort

[9814 op=2 display+exit]
  1: "He put a rope in a tree for me to swing on."

[9815 op=1 keyword; failure -> 9817]
  1: mari

[9816 op=2 display+exit]
  1: "She's a mean woman."
  2: "Never plays dolls or nothin'."

[9817 op=1 keyword; failure -> 9819]
  1: arbe

[9818 op=2 display+exit]
  1: "He is my secret friend."
  2: "He tells me stories."

[9819 op=1 keyword; failure -> 9821]
  1: gris

[9820 op=2 display+exit]
  1: "I think he is a ghost."
  2: "He's scary."
  3: She hugs her doll.

[9821 op=1 keyword; failure -> 9823]
  1: hend

[9822 op=2 display+exit]
  1: She wrinkles up her face.
  2: "He doesn't smell too good."

[9823 op=1 keyword; failure -> 9825]
  1: uber

[9824 op=2 display+exit]
  1: "He brings my Memah flowers sometimes."

[9825 op=1 keyword; failure -> 9827]
  1: bye

[9826 op=2 display+exit]
  1: She waves her dolls hand at you and says "Bye."

[9827 op=2 display+exit]
  1: "Have you heard of that Becky?" she says to her doll.

```

<a id="michael-npc-88"></a>
## Michael ? NPC 88, port 89

Instruction range: 8529?8563

```text
[8529 op=1 keyword; failure -> 8531]
  1: look

[8530 op=2 display+exit]
  1: a tall man with muscles looking like huge snakes.
  2: "Speak," he growls at you, his voice as rough and cold as stone.

[8531 op=1 keyword; failure -> 8533]
  1: name

[8532 op=2 display+exit]
  1: "Are you taking a @census?"
  2: His powerful gaze is fixed on you.

[8533 op=1 keyword; failure -> 8535]
  1: cens

[8534 op=2 display+exit]
  1: "Yes, you know, counting people, taking down names..."

[8535 op=1 keyword; failure -> 8537]
  1: mich

[8536 op=2 display+exit]
  1: "That name is given to friends so that they may feel they are close to me."
  2: "Since I have none, refrain from its use."

[8537 op=1 keyword; failure -> 8539]
  1: ghos

[8538 op=2 display+exit]
  1: "That is a concern of the town, not mine."

[8539 op=1 keyword; failure -> 8541]
  1: quen

[8540 op=2 display+exit]
  1: "He was not fortunate."
  2: "It is written that it is better to be fortunate than skilled."
  3: "He, I'm sure, would agree."
  4: His face betrays a slight grin.

[8541 op=1 keyword; failure -> 8543]
  1: murd,kill

[8542 op=2 display+exit]
  1: "All men have enemies."
  2: "A @debt needed to be settled."
  3: "He hadn't the strength to settle it, so someone decided to take the initiative."
  4: He pulls out a knife and begins to pick at his nails.

[8543 op=1 keyword; failure -> 8545]
  1: debt,sett

[8544 op=2 display+exit]
  1: "Surely you don't buy the fable of the @gargoyle."
  2: "There wouldn't be enough left to bury if this were the case."
  3: "This was no monster who settled the debt, it was the work of man's hands."
  4: He brings the knife to his teeth and delicately works it between them.

[8545 op=1 keyword; failure -> 8547]
  1: garg

[8546 op=2 display+exit]
  1: "Have you ever seen one?"
  2: "If you had you would not believe the fairy tales being told by that incompetent mayor."
  3: He grabs the knife and holds it aloft.
  4: "See this blade?"
  5: "Their fangs are twice as long and thrice as sharp."

[8547 op=1 keyword; failure -> 8549]
  1: tren,bell,inco,mayo

[8548 op=2 display+exit]
  1: "He is a fool."
  2: "The perfect man for his job."

[8549 op=1 keyword; failure -> 8551]
  1: yorl

[8550 op=2 display+exit]
  1: "I don't know him."

[8551 op=1 keyword; failure -> 8553]
  1: gide

[8552 op=2 display+exit]
  1: "Too kind for his own good."

[8553 op=1 keyword; failure -> 8555]
  1: stiv

[8554 op=2 display+exit]
  1: "An annoying little man."
  2: "Makes good wine though."

[8555 op=1 keyword; failure -> 8557]
  1: marn

[8556 op=2 display+exit]
  1: "She is kind to me, and yet I believe this is genuine."

[8557 op=1 keyword; failure -> 8559]
  1: hora

[8558 op=2 display+exit]
  1: "Perhaps the wisest of us all."

[8559 op=1 keyword; failure -> 8561]
  1: lord,brit

[8560 op=2 display+exit]
  1: "He is one to be respected."
  2: "I have seen him fight, firsthand."
  3: "He is a mighty warrior, as well as a just opponent."

[8561 op=1 keyword; failure -> 8563]
  1: bye

[8562 op=2 display+exit]
  1: He resumes cleaning his nails with his knife.

[8563 op=2 display+exit]
  1: He ignores the question and you think it wise not to ask again.

```

<a id="michelle-npc-66"></a>
## Michelle ? NPC 66, port 67

Instruction range: 6170?6284

```text
[6171 op=1 keyword; failure -> 6174]
  1: yes

[6173 op=2 display+exit]
  1: "Which, a @basket, @beehive or @custom @job?"

[6176 op=2 display+exit]
  1: "All right, though I can give thou the best price in the land!"

[6177 op=1 keyword; failure -> 6181]
  1: bask

[6180 op=2 display+exit]
  1: "Those baskets cost 10 gold apiece."
  2: "How many dost thou wish?"

[6185 op=1 keyword; failure -> 6189]
  1: beeh

[6188 op=2 display+exit]
  1: "Those beehives cost 15 gold apiece."
  2: "How many dost thou wish?"

[6194 op=1 keyword; failure -> 6197]
  1: cust,job

[6196 op=2 display+exit]
  1: "I fear my schedule is packed."
  2: "I have no time to create a basket for which I have no @plans."

[6198 op=1 keyword; failure -> 6200]
  1: yes

[6202 op=2 display+exit]
  1: "I do not blame thee."
  2: "'Tis a huge undertaking!"

[6204 op=1 keyword; failure -> 6207]
  1: yes

[6206 op=2 display+exit]
  1: "Thou art wise, then."
  2: "'Tis a huge bag this will require."
  3: "I do not mean to pry, but why dost thou need a balloon anyway?"

[6209 op=2 display+exit]
  1: "Well, I can see that this will require a huge silk bag."
  2: "I know not where thou might find a silk weaver to craft it!"
  3: "I do not mean to pry, but why dost thou need a balloon anyway?"

[6214 op=2 display+exit]
  1: "I'm afraid I don't understand."

[6217 op=2 display+exit]
  1: "Oh, I see."

[6219 op=1 keyword; failure -> 6222]
  1: yes

[6221 op=2 display+exit]
  1: "Which, a @basket, @beehive or @custom @job?"

[6224 op=2 display+exit]
  1: "Perhaps some other time."

[6226 op=1 keyword; failure -> 6230]
  1: look

[6229 op=2 display+exit]
  1: an industrious young woman.
  2: "How can I help thee this fine $T?"

[6230 op=1 keyword; failure -> 6232]
  1: look

[6231 op=2 display+exit]
  1: an industrious young woman.
  2: "Well, hello again, Avatar!"

[6232 op=1 keyword; failure -> 6234]
  1: name

[6233 op=2 display+exit]
  1: "I'm Michelle."
  2: "Thou needn't tell me thy name, $P."
  3: "The Avatar is famous throughout the land!"

[6234 op=1 keyword; failure -> 6236]
  1: job

[6235 op=2 display+exit]
  1: "I weave baskets and beehives, just like my @father and his father before him."
  2: "My @baskets are sold throughout Britannia!" she adds proudly.

[6236 op=1 keyword; failure -> 6238]
  1: sell

[6237 op=2 display+exit]
  1: "I have all I need, and have no reason to purchase anything more."
  2: "But I appreciate thy offer."

[6238 op=1 keyword; failure -> 6240]
  1: fath,ball

[6239 op=2 display+exit]
  1: "Father once told me a story of weaving a basket large enough for eight people!"
  2: "He never showed me the @plans, though."

[6242 op=1 keyword; failure -> 6245]
  1: bask,beeh

[6244 op=2 display+exit]
  1: "Wouldst thou like to buy one of my weavings?"

[6245 op=1 keyword; failure -> 6248]
  1: buy

[6247 op=2 display+exit]
  1: "Which, a @basket, @beehive or @custom @job?"

[6248 op=1 keyword; failure -> 6251]
  1: plan

[6250 op=2 display+exit]
  1: "Thou hast the basket already!"

[6251 op=1 keyword; failure -> 6255]
  1: plan

[6254 op=2 display+exit]
  1: "Then it was true!"
  2: "Father did weave a basket that big!"
  3: "Surely I could do the same."
  4: She studies the plans for a moment.
  5: "Such a basket would cost 300 gold, in advance."
  6: "Art thou still interested?"

[6255 op=1 keyword; failure -> 6257]
  1: plan

[6256 op=2 display+exit]
  1: "Yes, without accurate plans it takes far too much time to craft a basket."

[6257 op=1 keyword; failure -> 6259]
  1: bask,beeh,buy

[6258 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[6259 op=1 keyword; failure -> 6261]
  1: plan

[6260 op=2 display+exit]
  1: "Ask when I'm open for business!"

[6261 op=1 keyword; failure -> 6263]
  1: bye

[6262 op=2 display+exit]
  1: "Farewell! I hope I've aided thy quest!"

[6263 op=2 display+exit]
  1: "Beg pardon?"

[6268 op=2 display+exit]
  1: "Thou hast not enough money for the basket."

[6271 op=2 display+exit]
  1: "Your party takes up a collection to pay for the basket."
  2: "She begins weaving the huge basket, chatting with you as she works."
  3: Time passes...
  4: More time passes...
  5: "Finished!"
  6: "It took less time than I had thought it might!"
  7: She gives the basket to you.
  8: "Hast thou found a silk weaver yet?"

[6274 op=2 display+exit]
  1: "These baskets are sturdy and decorative."
  2: "Thou'lt find no better in all Britannia for the price, a mere 10 gold."
  3: "And if thou hast need of beehives, these woven ones are the best thou'lt find!"
  4: "And cheap, too, at 15 gold apiece."

[6276 op=2 display+exit]
  1: "I can't sell thee that many!"

[6280 op=2 display+exit]
  1: "Thou hast not enough gold for that many!"

[6282 op=2 display+exit]
  1: "Thou canst not carry that many!"

[6284 op=2 display+exit]
  1: "I appreciate thy business."
  2: "Is there aught else thou wouldst buy?"

```

<a id="minax-npc-188"></a>
## Minax ? NPC 188, port 189

Instruction range: 16331?16400

```text
[16332 op=1 keyword; failure -> 16339]
  1: or

[16336 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need passion to succeed in thy quest, Avatar."
  3: "But thou must limit thy passion, lest balance and order be lost."
  4: "Thy deeds show nought but unlimited @passion."

[16338 op=2 display+exit]
  1: "Or..."
  2: "Or..."
  3: "Or..."

[16340 op=1 keyword; failure -> 16347]
  1: or

[16344 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need passion to succeed in thy quest, Avatar."
  3: "But thou must limit thy passion, lest balance and order be lost."
  4: "Thy deeds show misguided but strong @passion."

[16346 op=2 display+exit]
  1: "Or..."
  2: "Or..."
  3: "Or..."

[16348 op=1 keyword; failure -> 16355]
  1: or

[16352 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need passion to succeed in thy quest, Avatar."
  3: "But thou must limit thy passion, lest balance and order be lost."
  4: "Thy deeds show the merits of thy @passion."

[16354 op=2 display+exit]
  1: "Or..."
  2: "Or..."
  3: "Or..."

[16356 op=1 keyword; failure -> 16362]
  1: or

[16359 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need passion to succeed in thy quest, Avatar."
  3: "But thou must limit thy passion, lest balance and order be lost."
  4: "I admire thy deeds and thy @passion."

[16361 op=2 display+exit]
  1: "Or..."
  2: "Or..."
  3: "Or..."

[16365 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need passion to succeed in thy quest, Avatar."
  4: "But thou must limit thy passion, lest balance and order be lost."
  5: "Thy deeds show nought but unlimited @passion."

[16367 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16371 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need passion to succeed in thy quest, Avatar."
  4: "But thou must limit thy passion, lest balance and order be lost."
  5: "Thy deeds show misguided but strong @passion."

[16373 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16377 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need passion to succeed in thy quest, Avatar."
  4: "But thou must limit thy passion, lest balance and order be lost."
  5: "Thy deeds show the merits of thy @passion."

[16379 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16382 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need passion to succeed in thy quest, Avatar."
  4: "But thou must limit thy passion, lest balance and order be lost."
  5: "I admire thy deeds and thy @passion."

[16384 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16385 op=1 keyword; failure -> 16389]
  1: look

[16388 op=2 display+exit]
  1: a stone statue of the evil witch Minax.
  2: You remember quite clearly how you defeated Minax in your second visit to Brittania.
  3: With a mighty sword blow, you slew the witch and destroyed her evil works.
  4: The statue speaks!
  5: "We meet again, Avatar."
  6: "Fear not!"
  7: "I have but one @task now, and am no threat."

[16389 op=1 keyword; failure -> 16391]
  1: look

[16390 op=2 display+exit]
  1: a stone statue of the evil witch Minax.
  2: "For what dost thou return, Avatar?"

[16391 op=1 keyword; failure -> 16393]
  1: name

[16392 op=2 display+exit]
  1: "Surely you of all people would recognize me as Minax."

[16393 op=1 keyword; failure -> 16395]
  1: job,task

[16394 op=2 display+exit]
  1: "The gargoyles have enshrined my spirit here as the embodiment of @passion."

[16395 op=1 keyword; failure -> 16398]
  1: pass,mant

[16397 op=2 display+exit]
  1: "I am enshrined here in eternal contemplation of the principle of @passion."
  2: "I was once a mighty sorceress."
  3: "Passion was the essence of my soul."
  4: "But that passion knew no bounds."
  5: "I wanted everything, and because of this strove to conquer all."
  6: "Passion must be directed, focused."
  7: "Unbridled passion, desire for all things, leads only to chaos."
  8: "The gargoyles understand passion."
  9: "Their society is based on the passionate leading those who lack motivation."
  10: "In this way, they have the will to struggle against their harsh world."
  11: "Contemplate what thou hast learned."
  12: "Repeat after me the Mantra of Passion."
  13: "Or...."

[16398 op=1 keyword; failure -> 16400]
  1: bye

[16399 op=2 display+exit]
  1: "Farewell, Avatar."
  2: "May thy @passion drive thy quest toward success."

[16400 op=2 display+exit]
  1: "I have no concern for matters such as $Z."

```

<a id="mole-npc-176"></a>
## Mole ? NPC 176, port 177

Instruction range: 15568?15674

```text
[15570 op=2 display+exit]
  1: You tell him what Glen said.
  2: "Wants 'is graves dug, does he?"
  3: "Well, you can tell old zombie-face I'd have done a lot better if I'd had a shovel!"

[15572 op=2 display+exit]
  1: You tell him what Glen said.
  2: "Wants 'is graves dug, does he?"
  3: "Well, you can tell old zombie-face I'd have done a lot better if I'd had a shovel!"

[15575 op=2 display+exit]
  1: Mole listens, a bit impatiently.
  2: "He gave me a shovel, all right!"
  3: "Cheapest shovel I ever saw in all my years in the business!"
  4: "Tell 'im it broke the third day I tried usin' it."

[15577 op=2 display+exit]
  1: Mole listens, a bit impatiently.
  2: "He gave me a shovel, all right!"
  3: "Cheapest shovel I ever saw in all my years in the business!"
  4: "Tell 'im it broke the third day I tried usin' it."

[15580 op=2 display+exit]
  1: "I'd have been glad to get a new shovel!"
  2: "Believe me, I knows how to pick out a good one, too."
  3: "But how was I supposed to pay for it, you just ask him that!"

[15582 op=2 display+exit]
  1: "I'd have been glad to get a new shovel!"
  2: "Believe me, I knows how to pick out a good one, too."
  3: "But how was I supposed to pay for it, you just ask him that!"

[15585 op=2 display+exit]
  1: "Aye, maybe there'd 'ave been extra for shovel and such in my pay,"
  2: "if the old skinflint had ever remembered to pay me!"

[15587 op=2 display+exit]
  1: "Aye, maybe there'd 'ave been extra for shovel and such in my pay,"
  2: "if the old skinflint had ever remembered to pay me!"

[15590 op=2 display+exit]
  1: "Old penny pincher says he's going to pay me?"
  2: "I'll believe it when I have the coins in me hand."

[15592 op=1 keyword; failure -> 15594]
  1: yes

[15596 op=2 display+exit]
  1: "Too bad."
  2: "I really need one."

[15616 op=1 keyword; failure -> 15620]
  1: look

[15619 op=2 display+exit]
  1: a coarse looking individual.
  2: "Now, now."
  3: "You can't be bringing bodies out here without no reservations."
  4: "Go see me master Glen about having a funeral first."

[15620 op=1 keyword; failure -> 15623]
  1: look

[15622 op=2 display+exit]
  1: a coarse looking individual, digging in the dirt with his bare hands.
  2: "Now, now."
  3: "You can't be bringing bodies out here without no reservations."
  4: "Go see me master Glen about having a funeral first."

[15623 op=1 keyword; failure -> 15626]
  1: look

[15625 op=2 display+exit]
  1: a coarse looking individual.
  2: "Hello."
  3: "If I be speaking with ye, I'd best not be digging at the same time."

[15626 op=1 keyword; failure -> 15628]
  1: look

[15627 op=2 display+exit]
  1: a coarse looking individual, digging in the dirt with his bare hands.
  2: "Hello."
  3: "If I be speaking with ye, I'd best not be digging at the same time."

[15628 op=1 keyword; failure -> 15630]
  1: name

[15629 op=2 display+exit]
  1: "I be Mole."

[15630 op=1 keyword; failure -> 15632]
  1: dig,fath

[15631 op=2 display+exit]
  1: "Digging's been in me family for generations."

[15632 op=1 keyword; failure -> 15634]
  1: grav

[15633 op=2 display+exit]
  1: "If you take away enough dirt, a grave's what's left."
  2: "After they has a @funeral, I puts all the dirt back again."

[15634 op=1 keyword; failure -> 15636]
  1: fune

[15635 op=2 display+exit]
  1: "Funerals aren't me trade."
  2: "I know plenty about @dirt, though."

[15636 op=1 keyword; failure -> 15638]
  1: dirt

[15637 op=2 display+exit]
  1: "I knows all the different @kinds of dirt by heart."

[15638 op=1 keyword; failure -> 15640]
  1: diff,kind,hear

[15639 op=2 display+exit]
  1: "Learned 'em from me father."

[15640 op=1 keyword; failure -> 15642]
  1: trad,job

[15641 op=2 display+exit]
  1: "I @dig @graves for me master, @Glen."

[15642 op=1 keyword; failure -> 15647]
  1: shovel,give

[15646 op=2 display+exit]
  1: "Oh, ye've brought a shovel with ye!"
  2: "Is it for me?"

[15647 op=1 keyword; failure -> 15649]
  1: shov

[15648 op=2 display+exit]
  1: "'Tis the @tool of me trade."

[15649 op=1 keyword; failure -> 15651]
  1: give

[15650 op=2 display+exit]
  1: "What 'ave you got I could want?"
  2: "All I needs in me life is a good @shovel."

[15651 op=1 keyword; failure -> 15653]
  1: tool

[15652 op=2 display+exit]
  1: "Me father said a good shovel is more important than a good @wife."

[15653 op=1 keyword; failure -> 15655]
  1: wife

[15654 op=2 display+exit]
  1: "That's what you look for after you've got a good @shovel."

[15657 op=1 keyword; failure -> 15661]
  1: mast,glen

[15660 op=2 display+exit]
  1: "I'll not speak with 'im, the way he treats me."
  2: "But if you sees him, tell him I could have dug these graves better if I'd had a @shovel!"
  3: "He's a strange one... Nobody knows what he keeps in that back room of 'is."

[15661 op=1 keyword; failure -> 15664]
  1: mast,glen

[15663 op=2 display+exit]
  1: "I'll not speak with 'im, the way he treats me."
  2: "But if you sees him, tell him I could dig these graves better if I had a @shovel!"
  3: "He's a strange one... Nobody knows what he keeps in that back room of 'is."

[15664 op=1 keyword; failure -> 15666]
  1: mast,glen

[15665 op=2 display+exit]
  1: "He's a strange one... Nobody knows what he keeps in that back room of 'is."

[15666 op=1 keyword; failure -> 15668]
  1: bye

[15667 op=2 display+exit]
  1: "Oh, going away, are ye?"
  2: "Let me know when ye return, so I'll know 'tis time to speak with ye again."
  3: He returns to his digging.

[15668 op=2 display+exit]
  1: "I think ye'd best ask someone smarter than me."

[15672 op=2 display+exit]
  1: "Too bad."
  2: "I really need one."

[15674 op=2 display+exit]
  1: "Thank ye very much!"
  2: "And 'tis not even my birthday!"
  3: "Let me tell you this - keep an eye on old Glen."
  4: "They say not all of the bodies end up where they're supposed to..."
  5: "You know, 'e was supposed to have paid me for a new shovel,"
  6: "and then you wouldn't have 'ad to go to so much trouble."

```

<a id="mondain-npc-187"></a>
## Mondain ? NPC 187, port 188

Instruction range: 16261?16330

```text
[16262 op=1 keyword; failure -> 16269]
  1: un

[16266 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "Thy deeds show a lack of @control."

[16268 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16270 op=1 keyword; failure -> 16277]
  1: un

[16274 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "Thy deeds show a need for more @control."

[16276 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16278 op=1 keyword; failure -> 16285]
  1: un

[16282 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "Thy deeds show understanding of @control."

[16284 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16286 op=1 keyword; failure -> 16292]
  1: un

[16289 op=22 display+continue]
  1: "I can teach thee no more."
  2: "Thou wilt need control of others to succede in thy quest, Avatar."
  3: "But thou wilt most of all need control of thyself to be worthy."
  4: "I admire thy deeds and thy @control."

[16291 op=2 display+exit]
  1: "Un..."
  2: "Un..."
  3: "Un..."

[16295 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "Thy deeds show a lack of @control."

[16297 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16301 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "Thy deeds show a need for more @control."

[16303 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16307 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "Thy deeds show understanding of @control."

[16309 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16312 op=22 display+continue]
  1: "Thy understanding is flawed."
  2: "But I can teach thee no more."
  3: "Thou wilt need control of others to succede in thy quest, Avatar."
  4: "But thou wilt most of all need control of thyself to be worthy."
  5: "I admire thy deeds and thy @control."

[16314 op=2 display+exit]
  1: "$Z..."
  2: "$Z..."
  3: "$Z..."

[16315 op=1 keyword; failure -> 16319]
  1: look

[16318 op=2 display+exit]
  1: a stone statue of the evil wizard Mondain.
  2: You remember as if it were yesterday how you defeated Mondain in your first visit to Brittania.
  3: Using the four stones, you went back in time to smash his power-gem.
  4: The statue speaks!
  5: "We meet again, Avatar."
  6: "Fear not!"
  7: "I have but one @task now, and am no threat."

[16319 op=1 keyword; failure -> 16321]
  1: look

[16320 op=2 display+exit]
  1: a stone statue of the evil wizard Mondain.
  2: "For what dost thou return, Avatar?"

[16321 op=1 keyword; failure -> 16323]
  1: name

[16322 op=2 display+exit]
  1: "Surely you of all people would recognize me as Mondain."

[16323 op=1 keyword; failure -> 16325]
  1: job,task

[16324 op=2 display+exit]
  1: "The gargoyles have enshrined my spirit here as the embodiment of @control."

[16325 op=1 keyword; failure -> 16328]
  1: cont,mant

[16327 op=2 display+exit]
  1: "I am enshrined here in eternal contemplation of the principle of @control."
  2: "I was once a mighty wizard."
  3: "Control was the essence of my soul."
  4: "Control of self and control of others."
  5: "But desire for more control overcame me."
  6: "I forsook my self-control in my hunger for conquest."
  7: "Control must start with the self and expand outward."
  8: "The strong must lead."
  9: "But to be strong one must control oneself first."
  10: "The gargoyles understand control."
  11: "Their society is based on the strong guiding the weak."
  12: "In this way, they survive in their harsh world."
  13: "Contemplate what thou hast learned."
  14: "Repeat after me the Mantra of Control."
  15: "Un...."

[16328 op=1 keyword; failure -> 16330]
  1: bye

[16329 op=2 display+exit]
  1: "Farewell, Avatar."
  2: "May thy @control help you succeed in thy quest."

[16330 op=2 display+exit]
  1: "I have no concern for matters such as $Z."

```

<a id="morchella-npc-154"></a>
## Morchella ? NPC 154, port 155

Instruction range: 14224?14273

```text
[14225 op=1 keyword; failure -> 14233]
  1: yes

[14232 op=2 display+exit]
  1: From a little box Morchella hands over a piece of paper.
  2: It is a piece of the pirate map.
  3: "I was going to give this magic shield to Caradon, but methinks I'll keep it for meself instead!"
  4: "Good $T, $G." She walks off.

[14235 op=2 display+exit]
  1: "Then leave me be!"
  2: She turns her back angrily on you and walks off.

[14236 op=1 keyword; failure -> 14238]
  1: look

[14237 op=2 display+exit]
  1: a tough-looking woman with a walk like a troll.
  2: She stares insolently at you.

[14238 op=1 keyword; failure -> 14240]
  1: name

[14239 op=2 display+exit]
  1: "I'm Morchella."

[14240 op=1 keyword; failure -> 14245]
  1: job

[14242 op=22 display+continue]
  1: "I be a retired @sailor, that I am."
  2: "Well! How rude!" She turns up her nose and leaves.

[14244 op=2 display+exit]
  1: "Thou wert a @pirate!"

[14245 op=1 keyword; failure -> 14247]
  1: job

[14246 op=2 display+exit]
  1: "I be a retired @sailor, that I am."
  2: "An honest one, mind ye."

[14247 op=1 keyword; failure -> 14249]
  1: pira

[14248 op=2 display+exit]
  1: "I know nothing about that!" She walks off in a huff.

[14249 op=1 keyword; failure -> 14251]
  1: hone,sail

[14250 op=2 display+exit]
  1: "That's right, me bucko. Why, I've sailed with Lord @British and Captain @John.

[14251 op=1 keyword; failure -> 14253]
  1: lord,brit

[14252 op=2 display+exit]
  1: "He walks with a limp, ye know. 'Fraid of mice, too, he is."

[14253 op=1 keyword; failure -> 14255]
  1: capt,john

[14254 op=2 display+exit]
  1: "He offered to marry me but of course I refused. Didn't want to be tied down."

[14255 op=1 keyword; failure -> 14258]
  1: map

[14257 op=2 display+exit]
  1: "Now all I need is a magic sword, then methinks I'll be a knight."

[14258 op=1 keyword; failure -> 14260]
  1: map

[14259 op=2 display+exit]
  1: She looks around furtively first.
  2: "Well, now, I might have seen the likes of that in me travels..."
  3: "Tell ye what, matey."
  4: "If ye give me a @shield of the Order of the Serpent, I'll tell ye where that map be at."

[14260 op=1 keyword; failure -> 14262]
  1: cara,lad

[14261 op=2 display+exit]
  1: "I might even ask 'im to marry me."
  2: She winks slyly at you.

[14262 op=1 keyword; failure -> 14264]
  1: bye

[14263 op=2 display+exit]
  1: "So long, matey."

[14264 op=1 keyword; failure -> 14267]
  1: shie,serp,magi

[14266 op=2 display+exit]
  1: "Now all I need is a magic sword, then methinks I'll be a knight."

[14267 op=1 keyword; failure -> 14271]
  1: shie,serp,magi

[14270 op=2 display+exit]
  1: "That be quite a prize there, matey."
  2: "Will ye trade it to me for a bit o' map?"

[14271 op=1 keyword; failure -> 14273]
  1: shie,serp,magi

[14272 op=2 display+exit]
  1: "If I had a Silver Serpent shield, I'd give it to @Caradon."
  2: "He's a fine, brave lad, he is."

[14273 op=2 display+exit]
  1: "Don't know what to say 'bout that, matey."

```

<a id="mortude-npc-99"></a>
## Mortude ? NPC 99, port 100

Instruction range: 9244?9296

```text
[9247 op=1 keyword; failure -> 9249]
  1: buy,make,rope

[9249 op=1 keyword; failure -> 9252]
  1: name

[9251 op=2 display+exit]
  1: "As the sign on my door reads, '@Mortude the Ropemaker.'"

[9252 op=1 keyword; failure -> 9256]
  1: job

[9255 op=2 display+exit]
  1: "As the sign on my door reads, '@Mortude the Ropemaker.'"

[9256 op=1 keyword; failure -> 9259]
  1: look

[9258 op=2 display+exit]
  1: a man nearly as wide as he is tall.
  2: "What?" he grumbles.

[9259 op=1 keyword; failure -> 9261]
  1: look

[9260 op=2 display+exit]
  1: a man nearly as wide as he is tall.
  2: "What now?" he sighs.

[9261 op=1 keyword; failure -> 9263]
  1: mort

[9262 op=2 display+exit]
  1: "What?"
  2: He snorts a bit then clears his throat.

[9263 op=1 keyword; failure -> 9265]
  1: job

[9264 op=2 display+exit]
  1: He looks at you and raises one eyebrow.
  2: "I said I makes @rope."

[9265 op=1 keyword; failure -> 9267]
  1: thin

[9266 op=2 display+exit]
  1: "He's a pretty good @flippits, player."

[9267 op=1 keyword; failure -> 9269]
  1: flip

[9268 op=2 display+exit]
  1: "You have a @bone on ya?"
  2: "We'll play."

[9269 op=1 keyword; failure -> 9271]
  1: bone

[9270 op=2 display+exit]
  1: "An old @dog bone or somethin'?"

[9271 op=1 keyword; failure -> 9273]
  1: old,dog

[9272 op=2 display+exit]
  1: "The smaller, the better."
  2: "And maybe a @hat."

[9273 op=1 keyword; failure -> 9275]
  1: hat

[9274 op=2 display+exit]
  1: "A haaat", he repeats, mockingly.
  2: "Never played 'ave ya?"

[9275 op=1 keyword; failure -> 9277]
  1: no

[9276 op=2 display+exit]
  1: "Too bad."

[9277 op=1 keyword; failure -> 9279]
  1: bye

[9278 op=2 display+exit]
  1: "Yeah, yeah."
  2: He plods off, grumbling.

[9279 op=2 display+exit]
  1: "Oh, never mind..."
  2: He plays with his braided hair a moment then shrugs.

[9284 op=2 display+exit]
  1: "The strongest you'll ever tug!" he says, pushing out his chest.
  2: "Come to my shop when I'm open and I'll show you!"

[9286 op=2 display+exit]
  1: "The strongest you'll ever tug!" he says, pushing out his chest.
  2: He smiles at you, "For my fine rope it will cost you 5 gold."
  3: "Interested?"

[9288 op=1 keyword; failure -> 9292]
  1: yes

[9291 op=2 display+exit]
  1: "Fine, fine!"
  2: After accepting the gold, he hands over the rope.

[9293 op=2 display+exit]
  1: "You don't have enough gold."

[9295 op=2 display+exit]
  1: "You look pretty full to me."

[9296 op=2 display+exit]
  1: "Too bad."
  2: "You'll never find better rope."

```

<a id="myles-npc-143"></a>
## Myles ? NPC 143, port 144

Instruction range: 13138?13158

```text
[13138 op=1 keyword; failure -> 13142]
  1: look

[13141 op=2 display+exit]
  1: a young human child.
  2: "Wow! Two-eyed people like me! You gotta go see my folks!"

[13142 op=1 keyword; failure -> 13144]
  1: look

[13143 op=2 display+exit]
  1: a young human child.
  2: "Hi again!"

[13144 op=1 keyword; failure -> 13146]
  1: name

[13145 op=2 display+exit]
  1: "I'm Myles."
  2: "But Mama and Papa aren't good with names."
  3: "They just call me 'tiny one!'"

[13146 op=1 keyword; failure -> 13148]
  1: mama,papa,folk

[13147 op=2 display+exit]
  1: "They're not my real parents."
  2: "Mama and Papa found me on the beach."
  3: "I guess my real parents drowned or something."
  4: "That was long ago. I don't remember much about it."
  5: "Mama and Papa said they used to have a son of their own, but he died."
  6: "So they @adopted me!"

[13148 op=1 keyword; failure -> 13150]
  1: adop

[13149 op=2 display+exit]
  1: "We all live in that big old castle!"
  2: "It's a real neat place, with all sorts of rooms and @cellars and stairs and @stuff."

[13150 op=1 keyword; failure -> 13152]
  1: room,cell

[13151 op=2 display+exit]
  1: "A lot of places are locked. Papa keeps the keys."

[13152 op=1 keyword; failure -> 13154]
  1: job

[13153 op=2 display+exit]
  1: "Oh, I help Mama and @Papa!"
  2: "Though sometimes I think Papa wishes I were big and strong like a cyclops, so I could help more."

[13154 op=1 keyword; failure -> 13156]
  1: vort,cube,bloc,blac,big,ston,stuf

[13155 op=2 display+exit]
  1: "There's a whole lot of junk lying around the castle."
  2: "Ask Mama or Papa about it."
  3: "Papa doesn't let me play with much of it."

[13156 op=1 keyword; failure -> 13158]
  1: bye

[13157 op=2 display+exit]
  1: "See you later!"

[13158 op=2 display+exit]
  1: "Huh?"

```

<a id="nan-npc-10"></a>
## Nan ? NPC 10, port 11

Instruction range: 823?850

```text
[823 op=1 keyword; failure -> 825]
  1: look

[824 op=2 display+exit]
  1: a nervous young bard.
  2: "I would play for thee, but I'm worried about my lute."

[825 op=1 keyword; failure -> 828]
  1: name

[827 op=2 display+exit]
  1: "I am Lady Nan."

[828 op=1 keyword; failure -> 830]
  1: job

[829 op=2 display+exit]
  1: "Why, making @music, of course."

[830 op=1 keyword; failure -> 832]
  1: musi

[831 op=2 display+exit]
  1: "With my @lute."

[832 op=1 keyword; failure -> 834]
  1: play,worr,lute,musi

[833 op=2 display+exit]
  1: "Every time I pick up my lute, @spiders crawl out of it."
  2: "There must be hundreds of them nesting in there!"

[834 op=1 keyword; failure -> 836]
  1: spid,nest,craw,hund

[835 op=2 display+exit]
  1: "I hate spiders! They're creepy, ugly, disgusting things! Why won't they leave me alone?"

[836 op=1 keyword; failure -> 838]
  1: glov

[837 op=2 display+exit]
  1: "Iolo gave them to me for my birthday."

[838 op=1 keyword; failure -> 840]
  1: comp

[839 op=2 display+exit]
  1: "Yes, I know of the @Rune and @Mantra of Compassion."

[840 op=1 keyword; failure -> 842]
  1: mant

[841 op=2 display+exit]
  1: "The Mantra of Compassion is 'mu.' Is that a spider on your shoulder?"
  2: "No, 'tis just a trick of the light."

[842 op=1 keyword; failure -> 844]
  1: rune

[843 op=2 display+exit]
  1: "'Twas given to our most promising student, young @Ariana."

[844 op=1 keyword; failure -> 846]
  1: aria

[845 op=2 display+exit]
  1: "She's very talented. She'll go far, if the spiders don't get her..."

[846 op=1 keyword; failure -> 848]
  1: bye

[847 op=2 display+exit]
  1: "Fare thee well, and watch thy step! The spiders are lurking everywhere."

[848 op=2 display+exit]
  1: "I cannot help thee with that."

[849 op=2 display+exit]
  1: Iolo says to her, "Now, now, my dear. Have you forgotten about the @gloves I gave you?"

[850 op=2 display+exit]
  1: "Oh, yes. Perhaps if I wore those I could play..."

```

<a id="naxatilor-npc-166"></a>
## Naxatilor ? NPC 166, port 167

Instruction range: 14947?15042

```text
[14948 op=1 keyword; failure -> 14951]
  1: no

[14950 op=2 display+exit]
  1: "Perhaps to have another alternative..."
  2: "Yes! To @return the Codex to the @Vortex!"
  3: "To then be available to @both our peoples!"

[14953 op=2 display+exit]
  1: "To be utterly without hope in the face of such ignorance."
  2: "To refuse to converse with a daemon!"

[14955 op=1 keyword; failure -> 14959]
  1: yes

[14958 op=2 display+exit]
  1: "Good."
  2: "To be unable to reverse the ritual myself."
  3: "But to know that the Codex itself will say how to return it to the Vortex."
  4: "To know also that you will need a @second lens."
  5: "To require a @human crafted @lens, so that your people may also see the @Codex."

[14961 op=2 display+exit]
  1: "Then to go to the Hall of Knowledge and do so!"

[14964 op=1 keyword; failure -> 14967]
  1: look

[14966 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: "Yes, to see that the Vortex Lens is no longer whole."
  3: "To go to the lensmaker, whose house is northeast of here."
  4: "To get the lens repaired there!"
  5: "Then to return here with the whole lens!"

[14968 op=2 display+exit]
  1: "Yes, to see that the Vortex Lens is no longer whole."
  2: "To go to the lensmaker, whose house is northeast of here."
  3: "To get the lens repaired there!"
  4: "Then to return here with the whole lens!"

[14971 op=2 display+exit]
  1: "Good, to see that you have the Vortex Lens."
  2: "To have read the Book of Ritual as well?"

[14972 op=1 keyword; failure -> 14974]
  1: bye

[14973 op=2 display+exit]
  1: "Farewell!"
  2: "To return when you have visited the Hall of Knowledge!"

[14976 op=1 keyword; failure -> 14979]
  1: look

[14978 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: "To have nothing to say to you, False Prophet!"

[14980 op=2 display+exit]
  1: "To have nothing to say to you, False Prophet!"

[14981 op=1 keyword; failure -> 14983]
  1: look

[14982 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: The venerable gargoyle does not acknowledge your presence.

[14983 op=2 display+exit]
  1: The venerable gargoyle does not acknowledge your presence.

[14985 op=1 keyword; failure -> 14990]
  1: look

[14989 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: "To greet you, $P, Avatar of the Underworld."
  3: "To see the Amulet of Submission around your neck."
  4: "To consider your acceptance of this both noble and wise."
  5: "To observe by your aura that such honorable deeds are rare for you."
  6: "To not be surprised, then, that you are @fated to destroy a world."

[14990 op=1 keyword; failure -> 14995]
  1: look

[14994 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: "To greet you, $P, Avatar of the Underworld."
  3: "To see the Amulet of Submission around your neck."
  4: "To consider your acceptance of this both noble and wise."
  5: "To observe by your aura that you are not a being of evil."
  6: "To regret for your sake, then, that you are @fated to destroy a world."

[14995 op=1 keyword; failure -> 14999]
  1: look

[14998 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: "To greet you, $P, Avatar of the Underworld."
  3: "To see the Amulet of Submission around your neck."
  4: "To consider your acceptance of this both noble and wise."
  5: "To observe by your aura that you are truly a being of honor."
  6: "To be surprised, then, that you are @fated to destroy a world."

[14999 op=1 keyword; failure -> 15001]
  1: look

[15000 op=2 display+exit]
  1: a dark, rough-skinned gargoyle.
  2: "To greet you again, $P."

[15001 op=1 keyword; failure -> 15003]
  1: hall,know

[15002 op=2 display+exit]
  1: "To find the Vortex Lens in the Hall of Knowledge, south of here."

[15004 op=2 display+exit]
  1: "To require the Vortex Lens, foolish human!"
  2: "To waste no more time!"
  3: "To go to the @Hall @of @Knowledge and get the lens!"
  4: "To bring it to me personally, Avatar!"

[15005 op=1 keyword; failure -> 15007]
  1: fate,dest,worl

[15006 op=2 display+exit]
  1: "To have wreaked great havoc in this land by your theft of the Codex!"
  2: "To have no @choice but to return it!"

[15007 op=1 keyword; failure -> 15009]
  1: choi

[15008 op=2 display+exit]
  1: "To see only two alternatives..."
  2: "To return the @Codex, or to give up your life."

[15009 op=1 keyword; failure -> 15011]
  1: scho,valk

[15010 op=2 display+exit]
  1: "To have spoken to Scholar Valkadesh?"
  2: "Then surely to know what harm you have caused my world!"
  3: "To have no @choice but to return the Codex!"

[15011 op=1 keyword; failure -> 15013]
  1: sacr

[15012 op=2 display+exit]
  1: "Hss...Yes, to know of the three meanings of that word."
  2: "To ask to which type of sacrifice you refer: @self, @others or items of @value?"

[15013 op=1 keyword; failure -> 15015]
  1: self

[15014 op=2 display+exit]
  1: "To admire your willingness to sacrifice your life for the good of my people."
  2: "But to doubt that that is the meaning of the prophecy."

[15015 op=1 keyword; failure -> 15018]
  1: othe

[15017 op=2 display+exit]
  1: "What?!"
  2: "To suggest that you can atone for your misdeed by bathing in the blood of others?!"
  3: "To find such thoughts well in keeping with the blackness of your aura!"

[15018 op=1 keyword; failure -> 15020]
  1: othe

[15019 op=2 display+exit]
  1: "What?!"
  2: "To suggest that you can atone for your misdeed by bathing in the blood of others?!"
  3: "To find such thoughts unworthy of your honorable aura!"

[15020 op=1 keyword; failure -> 15023]
  1: valu,item

[15022 op=2 display+exit]
  1: "To know of only one item of true value..."
  2: "The Codex itself."
  3: "Surely not to destroy the Codex?"

[15023 op=1 keyword; failure -> 15025]
  1: name

[15024 op=2 display+exit]
  1: "To be called Naxatilor."

[15025 op=1 keyword; failure -> 15027]
  1: job

[15026 op=2 display+exit]
  1: "To be a seer, mystic advisor to my people."
  2: "But to be unable to avert what is written in the Book of Prophecies."

[15027 op=1 keyword; failure -> 15029]
  1: lens,huma,seco

[15028 op=2 display+exit]
  1: "To be sure to show the human lensmaker the Vortex Lens."
  2: "Also to be sure to tell the human lensmaker that his lens must be concave!"
  3: "To be most important..."
  4: "Concave!"

[15029 op=1 keyword; failure -> 15031]
  1: code

[15030 op=2 display+exit]
  1: "To know not where the Codex is."
  2: "But to remember that Captain Bolesh just returned from the Codex!"
  3: "To suggest you seek him."
  4: "To try the healer's house to the north."

[15031 op=1 keyword; failure -> 15033]
  1: bye

[15032 op=2 display+exit]
  1: "Farewell, $P."
  2: "To return when there is more we can discuss."

[15033 op=1 keyword; failure -> 15038]
  1: vort,ritu,retu

[15037 op=2 display+exit]
  1: "To direct you to go to the Hall of Knowledge, south of here."
  2: "There to read the Book of Ritual."
  3: "There also to get the Vortex Lens!"
  4: "Then to return here, where I will tell you what must be done!"

[15038 op=1 keyword; failure -> 15040]
  1: vort,ritu,retu

[15039 op=2 display+exit]
  1: "To be unable to reverse the ritual myself."
  2: "But to know that the Codex itself will say how to return it to the Vortex."
  3: "To know also that you will need a @second lens."
  4: "To require a @human crafted @lens, so that your people may also see the @Codex."

[15040 op=1 keyword; failure -> 15042]
  1: both,peop

[15041 op=2 display+exit]
  1: "To be able to see the @Vortex from both sides of the world!"
  2: "To benefit both races equally if the Codex is @returned there!"

[15042 op=2 display+exit]
  1: "To have no time to discuss that."

```

<a id="nema-npc-22"></a>
## Nema ? NPC 22, port 23

Instruction range: 1880?1927

```text
[1882 op=2 display+exit]
  1: "That's nice."

[1883 op=1 keyword; failure -> 1885]
  1: look

[1884 op=2 display+exit]
  1: a sleepy looking young lady.
  2: She yawns as you approach.
  3: "Oh, hello there."
  4: "I must have been @daydreaming."

[1885 op=1 keyword; failure -> 1889]
  1: name

[1888 op=2 display+exit]
  1: "My name is Nema."
  2: "What's yours?"

[1889 op=1 keyword; failure -> 1891]
  1: job

[1890 op=2 display+exit]
  1: "I tend Lord @British's @orchard."
  2: "Of course, this time of year there's not much to do, so I take a lot of @naps."

[1891 op=1 keyword; failure -> 1893]
  1: tend,orch

[1892 op=2 display+exit]
  1: "To provide fresh fruit for Lord British's banquets."

[1893 op=1 keyword; failure -> 1895]
  1: prov,fres,frui,banq,stop,year,king,dual,univ,expr,conc,colo

[1894 op=2 display+exit]
  1: "That's the way of it."

[1895 op=1 keyword; failure -> 1897]
  1: lord,brit

[1896 op=2 display+exit]
  1: "Long live the king! I was just a homeless @orphan when he gave me this job."

[1897 op=1 keyword; failure -> 1899]
  1: home,orph

[1898 op=2 display+exit]
  1: "My parents were killed in an @earthquake."
  2: "But now I have a place to live, and a job to put food on the table."

[1899 op=1 keyword; failure -> 1901]
  1: pare,kill,eart

[1900 op=2 display+exit]
  1: "The earthquakes seem to have stopped, thank goodness."
  2: "We haven't had one in well over a year now."

[1901 op=1 keyword; failure -> 1903]
  1: dayd

[1902 op=2 display+exit]
  1: "Well, I have better dreams at @night anyway."

[1903 op=1 keyword; failure -> 1905]
  1: nap

[1904 op=2 display+exit]
  1: "To snooze, perchance to dream..."
  2: "O @Morpheus, I long for a taste of thy sweet @oblivion!"

[1905 op=1 keyword; failure -> 1907]
  1: morp

[1906 op=2 display+exit]
  1: "Morpheus is the King of @Slumberland! I think the @moon is one of his daughters."

[1907 op=1 keyword; failure -> 1909]
  1: slum

[1908 op=2 display+exit]
  1: "It lies halfway between our fears and our @desires."

[1909 op=1 keyword; failure -> 1911]
  1: fear,desi

[1910 op=2 display+exit]
  1: "Fear and desire... What would either be without the other?"

[1911 op=1 keyword; failure -> 1913]
  1: moon

[1912 op=2 display+exit]
  1: "She is the sweet harbinger of the night!"

[1913 op=1 keyword; failure -> 1915]
  1: snoo,slee,obli

[1914 op=2 display+exit]
  1: "Sleep is the finest medicine, for in our @dreams lie the @answers to all the problems of the @waking world."

[1915 op=1 keyword; failure -> 1917]
  1: wak,worl

[1916 op=2 display+exit]
  1: "Colors never seem as vivid when I'm awake."

[1917 op=1 keyword; failure -> 1919]
  1: answ,prob

[1918 op=2 display+exit]
  1: "Day brings @questions, @night brings answers."
  2: "Thusly does the duality of the universe find expression in our lives."

[1919 op=1 keyword; failure -> 1921]
  1: ques

[1920 op=2 display+exit]
  1: "What color is seven? How many is green? Could horses speak, if they wished to? These things concern me..."

[1921 op=1 keyword; failure -> 1923]
  1: nigh

[1922 op=2 display+exit]
  1: "When the sun's blazing eye winks shut, and @Morpheus rules the heavens."

[1923 op=1 keyword; failure -> 1925]
  1: dream

[1924 op=2 display+exit]
  1: "I dream of castles of ice, rivers that run backwards, fairy princesses and cloud dragons."
  2: "Once I dreamed that my orchard grew and grew, until I felt like a tiny bug amongst the giant plants."
  3: "Sometimes everything is upside-down, or made of chocolate."
  4: "I've dreamed many a strange dream, but my favorite one is....."

[1925 op=1 keyword; failure -> 1927]
  1: bye

[1926 op=2 display+exit]
  1: She seems to have dozed off before she even heard you say goodbye.

[1927 op=2 display+exit]
  1: "I had a dream about that once..."
  2: "All this talking makes me sleepy..."

```

<a id="nicodemus-npc-56"></a>
## Nicodemus ? NPC 56, port 57

Instruction range: 5173?5425

```text
[5178 op=2 display+exit]
  1: "Which, @detect @trap?"

[5181 op=2 display+exit]
  1: "Which of these, @sleep, @unlock @magic or @untrap?"

[5184 op=2 display+exit]
  1: "Which of these, @magic @lock, @mass @sleep, @protection or @repel @undead?"

[5187 op=2 display+exit]
  1: "Which, @conjure?"

[5190 op=2 display+exit]
  1: "Which, @insect @swarm?"

[5193 op=2 display+exit]
  1: "Which of these, @charm, @confuse, @mass @protect or @web?"

[5196 op=2 display+exit]
  1: "Which of these, @enchant or @mass @invisibility?"

[5199 op=2 display+exit]
  1: "I have no spells of that circle."

[5200 op=2 display+exit]
  1: "As everybody knows, there are only eight circles."

[5203 op=1 keyword; failure -> 5208]
  1: dete

[5207 op=2 display+exit]
  1: "That spell costs 20 gold. Interested?"

[5209 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5212 op=1 keyword; failure -> 5217]
  1: slee

[5216 op=2 display+exit]
  1: "That spell costs 40 gold. Interested?"

[5217 op=1 keyword; failure -> 5222]
  1: unlo

[5221 op=2 display+exit]
  1: "That spell costs 40 gold. Interested?"

[5222 op=1 keyword; failure -> 5227]
  1: untr

[5226 op=2 display+exit]
  1: "That spell costs 40 gold. Interested?"

[5228 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5231 op=1 keyword; failure -> 5236]
  1: magi

[5235 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[5236 op=1 keyword; failure -> 5241]
  1: mass

[5240 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[5241 op=1 keyword; failure -> 5246]
  1: prot

[5245 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[5246 op=1 keyword; failure -> 5251]
  1: repe

[5250 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[5252 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5255 op=1 keyword; failure -> 5260]
  1: conj

[5259 op=2 display+exit]
  1: "That spell costs 80 gold. Interested?"

[5261 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5264 op=1 keyword; failure -> 5269]
  1: inse

[5268 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[5270 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5273 op=1 keyword; failure -> 5278]
  1: char

[5277 op=2 display+exit]
  1: "That spell costs 120 gold. Interested?"

[5278 op=1 keyword; failure -> 5283]
  1: conf

[5282 op=2 display+exit]
  1: "That spell costs 120 gold. Interested?"

[5283 op=1 keyword; failure -> 5288]
  1: mass

[5287 op=2 display+exit]
  1: "That spell costs 120 gold. Interested?"

[5288 op=1 keyword; failure -> 5293]
  1: web

[5292 op=2 display+exit]
  1: "That spell costs 120 gold. Interested?"

[5294 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5297 op=1 keyword; failure -> 5302]
  1: ench

[5301 op=2 display+exit]
  1: "That spell costs 140 gold. Interested?"

[5302 op=1 keyword; failure -> 5307]
  1: mass

[5306 op=2 display+exit]
  1: "That spell costs 140 gold. Interested?"

[5308 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5312 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5316 op=1 keyword; failure -> 5321]
  1: yes

[5320 op=2 display+exit]
  1: He hands you a piece of rune-covered parchment.

[5322 op=2 display+exit]
  1: "You lack the gold required."

[5324 op=2 display+exit]
  1: "Thou cannot carry that."

[5325 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[5329 op=1 keyword; failure -> 5334]
  1: yes

[5333 op=2 display+exit]
  1: He hands you the spellbook.

[5335 op=2 display+exit]
  1: "But thou cannot afford a spellbook!"

[5337 op=2 display+exit]
  1: "But thou cannot carry a spellbook!"

[5338 op=2 display+exit]
  1: "Perhaps something else then."

[5339 op=1 keyword; failure -> 5341]
  1: buy,reag

[5340 op=2 display+exit]
  1: "Which one, @blood @moss, @garlic, @ginseng, @spider @silk or @sulfurous @ash?"

[5341 op=1 keyword; failure -> 5346]
  1: bloo,moss

[5345 op=2 display+exit]
  1: "Blood moss costs 2 per portion."
  2: "How many dost thou want?"

[5346 op=1 keyword; failure -> 5351]
  1: garl

[5350 op=2 display+exit]
  1: "Garlic costs 1 per portion."
  2: "How many dost thou want?"

[5351 op=1 keyword; failure -> 5356]
  1: gins

[5355 op=2 display+exit]
  1: "Ginseng costs 1 per portion."
  2: "How many dost thou want?"

[5356 op=1 keyword; failure -> 5361]
  1: spid,silk

[5360 op=2 display+exit]
  1: "Spider silk costs 1 per portion."
  2: "How many dost thou want?"

[5361 op=1 keyword; failure -> 5366]
  1: sulf,ash

[5365 op=2 display+exit]
  1: "Sulfurous ash costs 2 per portion."
  2: "How many dost thou want?"

[5370 op=2 display+exit]
  1: "Perhaps something else then."

[5372 op=2 display+exit]
  1: "I cannot give thee that much!"

[5377 op=2 display+exit]
  1: "Thou cannot afford that much!"

[5379 op=2 display+exit]
  1: "Thou cannot carry that much!"

[5380 op=2 display+exit]
  1: "Here you are."

[5384 op=1 keyword; failure -> 5388]
  1: yes

[5387 op=2 display+exit]
  1: The old man hands you a yew staff.

[5389 op=2 display+exit]
  1: "But thou cannot afford a staff!"

[5391 op=2 display+exit]
  1: "But thou cannot carry a staff!"

[5392 op=2 display+exit]
  1: "Perhaps some other time, then."

[5393 op=1 keyword; failure -> 5395]
  1: look

[5394 op=2 display+exit]
  1: a wizened old man with a ready smile.
  2: "Good $T, Avatar. For what purpose hast thou come?"

[5395 op=1 keyword; failure -> 5398]
  1: name

[5397 op=2 display+exit]
  1: "I am Nicodemus."

[5398 op=1 keyword; failure -> 5400]
  1: job

[5399 op=2 display+exit]
  1: "Why, bless me, I work here!"
  2: "I sell @reagents, teach @spells, and craft magic @staves."
  3: "I also do a little @experimenting."

[5400 op=1 keyword; failure -> 5402]
  1: expe

[5401 op=2 display+exit]
  1: "Oh, nothing a great adventurer such as yourself would be @interested in."

[5403 op=1 keyword; failure -> 5405]
  1: inte

[5404 op=2 display+exit]
  1: He gets a suspicious look in his eyes.
  2: "Don't be too nosy, girlie!"

[5405 op=1 keyword; failure -> 5407]
  1: inte

[5406 op=2 display+exit]
  1: He gets a suspicious look in his eyes.
  2: "Don't be too nosy, sonny!"

[5407 op=1 keyword; failure -> 5410]
  1: book,spellbook,spellbooks

[5409 op=2 display+exit]
  1: "A spellbook costs 45 gold pieces."
  2: "Interested?"

[5411 op=1 keyword; failure -> 5414]
  1: lear,spel

[5413 op=2 display+exit]
  1: "In which Circle do you wish to study?"

[5415 op=1 keyword; failure -> 5418]
  1: lear,spel

[5417 op=2 display+exit]
  1: "In which Circle do you wish to study?"

[5418 op=1 keyword; failure -> 5420]
  1: lear,spel

[5419 op=2 display+exit]
  1: "But none of you has a @spellbook!"

[5420 op=1 keyword; failure -> 5423]
  1: staf,stav

[5422 op=2 display+exit]
  1: "Yes, I make magical yew staves."
  2: "If thou knowest the Enchant spell, thou canst lock a spell within the staff for quick use!"
  3: "A yew staff costs 100 gold pieces, but it's worth twice that!"
  4: "Wouldst thou like to buy one?"

[5423 op=1 keyword; failure -> 5425]
  1: bye

[5424 op=2 display+exit]
  1: "Come back anytime!"

[5425 op=2 display+exit]
  1: "Nope, nope, I won't talk about that!"

```

<a id="nomaan-npc-42"></a>
## Nomaan ? NPC 42, port 43

Instruction range: 3718?4005

```text
[3720 op=1 keyword; failure -> 3722]
  1: look

[3721 op=2 display+exit]
  1: a blond man with broad shoulders and a stern, unsmiling face.
  2: "Can't you see there's a lovely brawl going on? Talk to me tomorrow."

[3722 op=2 display+exit]
  1: "Can't you see there's a lovely brawl going on? Talk to me tomorrow."

[3727 op=2 display+exit]
  1: He bows stiffly. "Well met, $G."

[3731 op=2 display+exit]
  1: He bows stiffly. "Well met, $G."

[3732 op=1 keyword; failure -> 3737]
  1: name

[3736 op=2 display+exit]
  1: "I am Nomaan. What is thy name, $G?"

[3737 op=1 keyword; failure -> 3739]
  1: name

[3738 op=2 display+exit]
  1: "I am Nomaan."
  2: "@Naughty Nomaan, they call me."

[3739 op=1 keyword; failure -> 3742]
  1: look

[3741 op=2 display+exit]
  1: a blond man with broad shoulders and a stern, unsmiling face.
  2: "Hello again, $G."
  3: "What else can I help thee with?"

[3742 op=1 keyword; failure -> 3744]
  1: look

[3743 op=2 display+exit]
  1: a blond man with broad shoulders and a stern, unsmiling face.
  2: "Hello, $G."
  3: "What can I help thee with?"

[3744 op=1 keyword; failure -> 3746]
  1: naug

[3745 op=2 display+exit]
  1: "'Tis just a nickname, of course."
  2: You see a hint of a smile on his face.

[3746 op=1 keyword; failure -> 3748]
  1: rune

[3747 op=2 display+exit]
  1: "I won a rune in a tournament once."
  2: "'Tis gone now, though - and there's a fine @tale about it!"

[3748 op=1 keyword; failure -> 3750]
  1: gone,tale

[3749 op=2 display+exit]
  1: "I remember it as if it were yesterday..."
  2: "Zellivan held a tourney and I was awarded the Rune of Valour."
  3: "Afterwards, we were at the pub, celebrating."
  4: "I had drunk a full quart of ale, so my grip was a bit unsteady."
  5: "I was showing the rune around when it fell from my grasp onto the floor."
  6: "That's when the @rat ran out."

[3750 op=1 keyword; failure -> 3752]
  1: rat

[3751 op=2 display+exit]
  1: "It scampered over, seized the trinket and ran back 'cross the floor."
  2: "We all leapt at it, but the little beast was too quick."
  3: "It was in the @hole before anybody could stop it."

[3752 op=1 keyword; failure -> 3754]
  1: hole

[3753 op=2 display+exit]
  1: "It ran into a hole in the west wall of the pub, carrying the rune."
  2: "We tried everything - banging on the wall, digging, poking with a sword."
  3: "To no avail, though. We haven't seen the rune since."

[3754 op=1 keyword; failure -> 3756]
  1: mant

[3755 op=2 display+exit]
  1: "Go ask around in the tavern."

[3756 op=1 keyword; failure -> 3758]
  1: job

[3757 op=2 display+exit]
  1: "I run the armoury, @buying and @selling weapons."

[3760 op=1 keyword; failure -> 3762]
  1: buy

[3761 op=2 display+exit]
  1: "Certainly, $G."
  2: "Wouldst thou like @armaments or @ammunition?"

[3762 op=1 keyword; failure -> 3764]
  1: arm

[3763 op=2 display+exit]
  1: "Which item, @bow, @club, @crossbow, @dagger, @oil @flask, @spear or @spiked @shield?"

[3764 op=1 keyword; failure -> 3767]
  1: bow

[3766 op=2 display+exit]
  1: "That bow costs 30 gold."
  2: "Thou wants how many?"

[3767 op=1 keyword; failure -> 3770]
  1: club

[3769 op=2 display+exit]
  1: "That club costs 10 gold."
  2: "Thou wants how many?"

[3770 op=1 keyword; failure -> 3773]
  1: cros

[3772 op=2 display+exit]
  1: "That crossbow costs 40 gold."
  2: "Thou wants how many?"

[3773 op=1 keyword; failure -> 3776]
  1: dagg

[3775 op=2 display+exit]
  1: "That dagger costs 10 gold."
  2: "Thou wants how many?"

[3776 op=1 keyword; failure -> 3779]
  1: oil,flas

[3778 op=2 display+exit]
  1: "That oil flask costs 2 gold."
  2: "Thou wants how many?"

[3779 op=1 keyword; failure -> 3782]
  1: spea

[3781 op=2 display+exit]
  1: "That spear costs 15 gold."
  2: "Thou wants how many?"

[3782 op=1 keyword; failure -> 3785]
  1: spik,shie

[3784 op=2 display+exit]
  1: "That spiked shield costs 100 gold."
  2: "Thou wants how many?"

[3813 op=1 keyword; failure -> 3815]
  1: sell

[3829 op=1 keyword; failure -> 3831]
  1: bulk

[3833 op=1 keyword; failure -> 3835]
  1: drin

[3835 op=1 keyword; failure -> 3837]
  1: amm

[3836 op=2 display+exit]
  1: "Which kind, @arrows or @bolts?"

[3837 op=1 keyword; failure -> 3840]
  1: arro

[3839 op=2 display+exit]
  1: "I only sell arrows by the dozen for 4 gold."
  2: "How many dozens do you want, $Y?"

[3840 op=1 keyword; failure -> 3843]
  1: bolt

[3842 op=2 display+exit]
  1: "I only sell bolts by the dozen for 3 gold."
  2: "How many dozens do you want, $Y?"

[3852 op=1 keyword; failure -> 3854]
  1: buy

[3853 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[3854 op=1 keyword; failure -> 3856]
  1: amm

[3855 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[3856 op=1 keyword; failure -> 3858]
  1: arm

[3857 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[3858 op=1 keyword; failure -> 3860]
  1: bye

[3859 op=2 display+exit]
  1: "A fine $T to you, $G."

[3860 op=2 display+exit]
  1: "Hmm..."
  2: "Nope, I fear I know nothing of that, $G."

[3863 op=2 display+exit]
  1: "Beg pardon?"

[3865 op=2 display+exit]
  1: "For some inexplicable reason. I can not sell more than 100 of any item. My apologies."

[3869 op=2 display+exit]
  1: "Money for items. You need money to buy items Avatar!"

[3871 op=2 display+exit]
  1: "Thou dost have strength of a girl. You can't carry that much!"

[3872 op=2 display+exit]
  1: "Thank you stranga, come back, anytime."

[3875 op=2 display+exit]
  1: "Changed your mind, eh?"

[3877 op=2 display+exit]
  1: "But that's too many, $Y!"

[3883 op=2 display+exit]
  1: "Thou canst not afford that many."

[3885 op=2 display+exit]
  1: "Thou canst not carry that many."

[3887 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the arrows.

[3888 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the bolts.

[3895 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 15 gold for that bow, $Y?"

[3901 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 5 gold for that club, $Y?"

[3907 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 20 gold for that crossbow, $Y?"

[3913 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 5 gold for that dagger, $Y?"

[3919 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 1 gold for that oil flask, $Y?"

[3925 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 7 gold for that spear, $Y?"

[3931 op=2 display+exit]
  1: "Aye, you have something of interest..."
  2: "Will you take 1 gold for that spiked shield, $Y?"

[3934 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[3937 op=1 keyword; failure -> 3941]
  1: yes

[3940 op=2 display+exit]
  1: "Done!"
  2: He hands you 15 gold pieces and takes the bow.

[3942 op=1 keyword; failure -> 3946]
  1: yes

[3945 op=2 display+exit]
  1: "Done!"
  2: He hands you 5 gold pieces and takes the club.

[3947 op=1 keyword; failure -> 3951]
  1: yes

[3950 op=2 display+exit]
  1: "Done!"
  2: He hands you 20 gold pieces and takes the crossbow.

[3952 op=1 keyword; failure -> 3956]
  1: yes

[3955 op=2 display+exit]
  1: "Done!"
  2: He hands you 5 gold pieces and takes the dagger.

[3957 op=1 keyword; failure -> 3961]
  1: yes

[3960 op=2 display+exit]
  1: "Done!"
  2: He hands you 1 gold pieces and takes the oil flask.

[3962 op=1 keyword; failure -> 3966]
  1: yes

[3965 op=2 display+exit]
  1: "Done!"
  2: He hands you 7 gold pieces and takes the spear.

[3967 op=1 keyword; failure -> 3971]
  1: yes

[3970 op=2 display+exit]
  1: "Done!"
  2: He hands you 1 gold pieces and takes the spiked shield.

[3971 op=2 display+exit]
  1: "Changed your mind, eh?"

[3982 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[3985 op=2 display+exit]
  1: "By virtue! I've never seen a man carry so much!"
  2: "Next time we're at the Sword and Keg, I'll be placing my bets on you, friend."
  3: "Will you take $V0 gold for everything?"

[3986 op=2 display+exit]
  1: "Will you take $V0 gold for everything?"

[3988 op=1 keyword; failure -> 3990]
  1: yes

[3990 op=2 display+exit]
  1: "Changed your mind, eh?"

[3993 op=2 display+exit]
  1: "Hah! I see what you're trying to pull."
  2: "Try it later when I've had more to @drink."
  3: "Then we can nickname you '$Y the Rat!'"

[3995 op=2 display+exit]
  1: "Done!"
  2: He hands you $V0 gold pieces in exchange for the items.

[4003 op=2 display+exit]
  1: "Wine?  Hmph."
  2: "Where do you think we are, Moonglow?"

[4004 op=2 display+exit]
  1: "Well?  Where is it?"

[4005 op=2 display+exit]
  1: Nomaan takes the bottle, immediately uncorks it, and begins drinking.
  2: "It's five o'clock somewhere!" he bellows
  3: You rationally explain to Nomaan that Britannia is flat, therefore it is the same time everywhere.
  4: This explanation seems to anger him.
  5: "Get outta my shop!"

```

<a id="nystul-npc-4"></a>
## Nystul ? NPC 4, port 5

Instruction range: 368?458

```text
[369 op=1 keyword; failure -> 373]
  1: yes

[372 op=2 display+exit]
  1: "From whence could it have come?"
  2: "The gargoyles, perhaps?"
  3: "Best ask Lord British about it."
  4: "I believe he has some knowledge of such items."

[375 op=2 display+exit]
  1: "Such items are quite rare."
  2: "Indeed, the only one I have ever seen is that which Lord British himself possesses."

[379 op=1 keyword; failure -> 381]
  1: look

[380 op=2 display+exit]
  1: a concerned looking mage.
  2: "Hail to thee $G, and well met."
  3: "'Twas I who learned of thy peril through my mystic arts, so that aid might be sent unto thee."
  4: "I saw in the battle that Iolo didst find a @book."
  5: "I should like to see it."

[381 op=1 keyword; failure -> 383]
  1: look

[382 op=2 display+exit]
  1: a concerned looking mage.
  2: "Hail to thee $G, and well met."
  3: "'Twas I who learned of thy peril through my mystic arts, so that aid might be sent unto thee."

[383 op=1 keyword; failure -> 386]
  1: name

[385 op=2 display+exit]
  1: "I am Nystul, as thou knowest well."

[386 op=1 keyword; failure -> 390]
  1: book

[389 op=2 display+exit]
  1: "I see that thou didst find the book."
  2: "Might I examine it?"

[390 op=1 keyword; failure -> 392]
  1: book

[391 op=2 display+exit]
  1: "I saw in the battle that Iolo didst find a @book."
  2: "I should like to see it."

[394 op=1 keyword; failure -> 397]
  1: yes

[396 op=2 display+exit]
  1: "Strange..."
  2: "This has a picture on its cover of a gargoyle standing with one foot on the chest of a slain human."
  3: "This is interesting."
  4: "It's written in a language I know not."
  5: "Take it to Mariah at the Lycaeum, the finest scribe on the great Council of Wizards."
  6: " She has studied many languages, and perhaps she can decipher this book for thee."

[399 op=2 display+exit]
  1: "That is too bad."
  2: "I had hoped thou might take it to Mariah and have her translate it."

[403 op=2 display+exit]
  1: "One more thing, Avatar..."
  2: "I noticed that thou didst arrive through a red gateway."
  3: "Dost thou have the @stone that opened the gate?"

[404 op=1 keyword; failure -> 408]
  1: moon,gate,ston,blac,orb

[407 op=2 display+exit]
  1: "I noticed that thou didst arrive through a red gateway."
  2: "Dost thou have the @stone that opened the gate?"

[408 op=1 keyword; failure -> 410]
  1: buy,poti

[409 op=2 display+exit]
  1: "I have many types of potions."
  2: "Which colour art thou interested in?"

[410 op=1 keyword; failure -> 413]
  1: whit

[412 op=2 display+exit]
  1: "White potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[413 op=1 keyword; failure -> 416]
  1: blac

[415 op=2 display+exit]
  1: "Black potions will cost thee 30 gold each."
  2: "How many wouldst thou like?"

[416 op=1 keyword; failure -> 419]
  1: purp

[418 op=2 display+exit]
  1: "Purple potions will cost thee 30 gold each."
  2: "How many wouldst thou like?"

[419 op=1 keyword; failure -> 422]
  1: oran

[421 op=2 display+exit]
  1: "Orange potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[422 op=1 keyword; failure -> 425]
  1: gree

[424 op=2 display+exit]
  1: "Green potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[425 op=1 keyword; failure -> 428]
  1: yell

[427 op=2 display+exit]
  1: "Yellow potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[428 op=1 keyword; failure -> 431]
  1: red

[430 op=2 display+exit]
  1: "Red potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[431 op=1 keyword; failure -> 434]
  1: blue

[433 op=2 display+exit]
  1: "Blue potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[434 op=2 display+exit]
  1: "There is naught else I can help thee with at this time."

[438 op=2 display+exit]
  1: "Is there is something else I can help thee with $G?"

[440 op=2 display+exit]
  1: "That is too many!"

[453 op=2 display+exit]
  1: "That costs more than thou hast $G."

[455 op=2 display+exit]
  1: "That is too heavy for thee $G."

[457 op=2 display+exit]
  1: Nystul carefully hands you the potion.

[458 op=2 display+exit]
  1: Nystul carefully hands you the potions.

```

<a id="papa-npc-141"></a>
## Papa ? NPC 141, port 142

Instruction range: 13014?13102

```text
[13017 op=1 keyword; failure -> 13019]
  1: look

[13018 op=2 display+exit]
  1: a huge male cyclops.
  2: "Who you?"

[13019 op=2 display+exit]
  1: "Who you?"

[13030 op=2 display+exit]
  1: "Me cyclops."
  2: "@Tiny one say me 'Papa.'"

[13032 op=1 keyword; failure -> 13040]
  1: yes

[13039 op=2 display+exit]
  1: "Good fish! Here, key."
  2: The cyclops hands you a large, rusted key.

[13041 op=1 keyword; failure -> 13044]
  1: yes

[13043 op=2 display+exit]
  1: "Where fish? No smell fish."

[13046 op=2 display+exit]
  1: "Get @fish!"

[13048 op=1 keyword; failure -> 13054]
  1: yes

[13053 op=2 display+exit]
  1: "Good fish! Good friend!"

[13055 op=1 keyword; failure -> 13058]
  1: yes

[13057 op=2 display+exit]
  1: "Where fish? No smell fish."

[13060 op=2 display+exit]
  1: "No fish?"
  2: The cyclops seems sad.

[13062 op=1 keyword; failure -> 13065]
  1: yes

[13064 op=2 display+exit]
  1: "Want fish!"
  2: "Use pole, get fish."
  3: "Want key, bring fish!"

[13067 op=2 display+exit]
  1: "Why ask no want?"

[13068 op=1 keyword; failure -> 13072]
  1: look

[13071 op=2 display+exit]
  1: a huge male cyclops.
  2: "Hoh! You bring fish?"

[13072 op=1 keyword; failure -> 13075]
  1: look

[13074 op=2 display+exit]
  1: a huge male cyclops.
  2: "Hoh, hoh. You back!"
  3: "You bring good fish."
  4: "Bring more?"

[13075 op=1 keyword; failure -> 13077]
  1: name

[13076 op=2 display+exit]
  1: "Me cyclops."
  2: "@Tiny one say me 'Papa.'"

[13077 op=1 keyword; failure -> 13079]
  1: tiny,one

[13078 op=2 display+exit]
  1: "Find him on shore."
  2: "One-eye little one die long ago."
  3: "Female want little one, so keep tiny one."
  4: "Not strong like one-eye little one."

[13079 op=1 keyword; failure -> 13081]
  1: job

[13080 op=2 display+exit]
  1: "Live here! Eat, hunt!"

[13081 op=1 keyword; failure -> 13083]
  1: cell

[13082 op=2 display+exit]
  1: "Cellar no go. @Locked."

[13083 op=1 keyword; failure -> 13085]
  1: big,bloc,blac,cube

[13084 op=2 display+exit]
  1: "Mine. In @cellar. No get!"

[13085 op=1 keyword; failure -> 13088]
  1: fish

[13087 op=2 display+exit]
  1: "Fish good."
  2: "Like eat much fish!"

[13088 op=1 keyword; failure -> 13091]
  1: fish

[13090 op=2 display+exit]
  1: "Hoh! You bring fish?"

[13091 op=1 keyword; failure -> 13094]
  1: lock,key

[13093 op=2 display+exit]
  1: "No got key."

[13094 op=1 keyword; failure -> 13097]
  1: lock,key

[13096 op=2 display+exit]
  1: "Got key. Want key?"

[13097 op=1 keyword; failure -> 13100]
  1: bye

[13099 op=2 display+exit]
  1: "Come back, bring more fish!"

[13100 op=1 keyword; failure -> 13102]
  1: bye

[13101 op=2 display+exit]
  1: "Come back, bring fish!"

[13102 op=2 display+exit]
  1: "What mean $Z?"

```

<a id="patrick-npc-97"></a>
## Patrick ? NPC 97, port 98

Instruction range: 9085?9131

```text
[9087 op=1 keyword; failure -> 9090]
  1: look

[9089 op=2 display+exit]
  1: a colorfully garbed bard.
  2: "Good $T, ma'am."

[9090 op=1 keyword; failure -> 9092]
  1: look

[9091 op=2 display+exit]
  1: a colorfully garbed bard.
  2: "Good $T, sirrah."

[9093 op=1 keyword; failure -> 9095]
  1: name

[9094 op=2 display+exit]
  1: "Patrick's the name."

[9095 op=1 keyword; failure -> 9098]
  1: name

[9097 op=2 display+exit]
  1: "Patrick's the name."
  2: "What's yours?"

[9098 op=1 keyword; failure -> 9100]
  1: job

[9099 op=2 display+exit]
  1: "Same as any @bard - telling @stories, singing @songs, that sort of thing."

[9100 op=1 keyword; failure -> 9102]
  1: bard

[9101 op=2 display+exit]
  1: "I was trained in the @conservatory, like most of us."

[9102 op=1 keyword; failure -> 9104]
  1: cons

[9103 op=2 display+exit]
  1: "Johann, Lazeena, Sionnach--they all studied there."
  2: "Of course, there are always oddballs like @Mandrake."

[9104 op=1 keyword; failure -> 9106]
  1: joha,laze,sion

[9105 op=2 display+exit]
  1: "A fine bard indeed."

[9106 op=1 keyword; failure -> 9108]
  1: mand

[9107 op=2 display+exit]
  1: "Completely self taught, he is."
  2: "Got some odd ideas about the eight @virtues."
  3: "I have to give him credit, though..."
  4: "Last time we gathered together for a songwriting competition, he won first prize easily."

[9108 op=1 keyword; failure -> 9110]
  1: virt,eigh

[9109 op=2 display+exit]
  1: "Ask him yourself."

[9110 op=1 keyword; failure -> 9112]
  1: stor

[9111 op=2 display+exit]
  1: "Ok, here's one..."
  2: "The owl and the pussycat sailed to sea,"
  3: "In a beautiful pea green boat."
  4: "Along came a spider and sat down beside them,"
  5: "And the cat said 'Whose been sleeping in my bed!'"
  6: "The clock struck one, the owl fell down,"
  7: "And three little piggies went 'Wee wee wee wee wee' all the way home."
  8: "I don't understand it at all, but Dr. @Cat says that's his favorite story."

[9112 op=1 keyword; failure -> 9114]
  1: cat

[9113 op=2 display+exit]
  1: "He's a decent guy, and he doesn't water his @drinks either."
  2: "He told me I should learn a story with some @ducks in it, though."

[9114 op=1 keyword; failure -> 9116]
  1: drin

[9115 op=2 display+exit]
  1: "Being a bard can be @thirsty work."

[9116 op=1 keyword; failure -> 9118]
  1: thir

[9117 op=2 display+exit]
  1: "A warrior mustn't let his blade grow dull, right?"
  2: "Well, so also must a bard keep his throat from getting too dry!"

[9118 op=1 keyword; failure -> 9120]
  1: duck

[9119 op=2 display+exit]
  1: "Don't ask me."
  2: "If he likes ducks that's his business."

[9120 op=1 keyword; failure -> 9122]
  1: song

[9121 op=2 display+exit]
  1: He clears his throat...
  2: "A lusty young smith at his feist stood a-firing,"
  3: "His hammer lay by but his forge still aglow,"
  4: "When to him a buxom young damsel came smiling,"
  5: "And asked if to work at her forge he would go."
  6: "With a jingle vang jingle vang jingle vang jingle,"
  7: "With a jingle vang jingle vang jingle hi ho."
  8: "'I will' said the smith and they went off together,"
  9: "Unto the young damsel's forge they did go."
  10: "They stripped to go to it--'twas hot work and hot weather,"
  11: "She kindled the fire and she soon made him glow."
  12: "With a jingle vang jingle vang jingle vang jingle,"
  13: "With a jingle vang jingle vang jingle hi ho!"
  14: "I hope you enjoyed the song!"

[9122 op=1 keyword; failure -> 9124]
  1: bye

[9123 op=2 display+exit]
  1: "Fare thee well."

[9124 op=2 display+exit]
  1: "That's an excellent question."
  2: "Ask me another one!"

[9131 op=2 display+exit]
  1: "A pleasure."

```

<a id="peer-npc-51"></a>
## Peer ? NPC 51, port 52

Instruction range: 4464?4504

```text
[4465 op=1 keyword; failure -> 4467]
  1: yes

[4469 op=2 display+exit]
  1: "You'll ne'er find a better craft."

[4470 op=1 keyword; failure -> 4474]
  1: look

[4473 op=2 display+exit]
  1: a tall, muscled man with a wide grin.
  2: "Hello," he says, lumbering over to you and extending a large hand.
  3: "What can I do for ya?"

[4474 op=1 keyword; failure -> 4476]
  1: look

[4475 op=2 display+exit]
  1: a tall, muscled man with a wide grin.
  2: He squints at you. "We've met, haven't we?"
  3: "What can I do for ya?"

[4476 op=1 keyword; failure -> 4478]
  1: name

[4477 op=2 display+exit]
  1: "@Peer."

[4478 op=1 keyword; failure -> 4480]
  1: peer

[4479 op=2 display+exit]
  1: "My father was a @sailor, but couldn't spell."
  2: He laughs out loud.

[4480 op=1 keyword; failure -> 4482]
  1: sail,fath

[4481 op=2 display+exit]
  1: "And his father before him."

[4482 op=1 keyword; failure -> 4484]
  1: job,buy

[4483 op=2 display+exit]
  1: "I sell @ships and @skiffs."

[4484 op=1 keyword; failure -> 4487]
  1: ship

[4486 op=2 display+exit]
  1: "It'll cost you 200 gold for the deed."
  2: "Interested?"

[4487 op=1 keyword; failure -> 4490]
  1: skif

[4489 op=2 display+exit]
  1: "It'll cost you 30 gold for the deed."
  2: "Interested?"

[4490 op=1 keyword; failure -> 4492]
  1: bye

[4491 op=2 display+exit]
  1: "So long friend."
  2: He lumbers back to his work.

[4492 op=2 display+exit]
  1: "That's a puzzler."

[4501 op=2 display+exit]
  1: "Thou hast not enough gold."

[4503 op=2 display+exit]
  1: "You look pretty full to me, $Y."

[4504 op=2 display+exit]
  1: Your party takes up a collection to purchase the vessel.
  2: "Excellent!"
  3: After accepting the gold, she hands you the deed.
  4: "Sail her straight."

```

<a id="penumbra-npc-39"></a>
## Penumbra ? NPC 39, port 40

Instruction range: 3556?3629

```text
[3557 op=1 keyword; failure -> 3560]
  1: look

[3559 op=2 display+exit]
  1: a small, inscrutable woman, cloaked in shadows.
  2: "I see you have yet to fulfill your destiny."

[3560 op=1 keyword; failure -> 3563]
  1: look

[3562 op=2 display+exit]
  1: a small, inscrutable woman, cloaked in shadows.
  2: "Ah, hello Avatar."
  3: "I was wondering when you would get here."

[3567 op=2 display+exit]
  1: "So be it."

[3572 op=2 display+exit]
  1: You don't have that much.
  2: How much do you give her?

[3575 op=2 display+exit]
  1: She holds her hands out on either side of her crystal ball, not quite touching it, and closes her eyes.
  2: After a time, she speaks.
  3: "The future looks dark."
  4: "All is unclear."
  5: "Some more gold would shed further light on your destiny..."
  6: How much do you give her?

[3577 op=2 display+exit]
  1: "Though all is dark, I see a light at the end of the tunnel."
  2: "Quickly - more coins before I lose the thread of this vision!"
  3: How much do you give her?

[3579 op=2 display+exit]
  1: "Much conflict lies before you."
  2: "Your end..."
  3: "I cannot see it."
  4: "More gold is needed."
  5: How much do you give her?

[3581 op=2 display+exit]
  1: "You shall travel throughout the land before you are through."
  2: "A few more coins and all should be clear."
  3: How much do you give her?

[3583 op=2 display+exit]
  1: "You will journey into the depths of the earth..."
  2: "Strange creatures will you face..."
  3: "I can almost see the end now."
  4: How much do you give her?

[3585 op=2 display+exit]
  1: "You and your companions will face terrible choices..."
  2: "You will need powerful magic to prevail..."
  3: "Wait..."
  4: "There is some barrier before me."
  5: "Give me just one more coin and I think I can break through it."
  6: How much do you give her?

[3588 op=2 display+exit]
  1: "Yes, now all is made clear to me!"
  2: "I see a great Vortex."
  3: "There is a violet lens that is crucial to your quest."
  4: "It is broken, and must be made whole."
  5: "But one lens is not enough!"
  6: "A blue lens is needed as well."
  7: "You must have two lenses, else all is for naught."
  8: She opens her eyes.
  9: "I can see no more for now."
  10: "Leave me, that I might rest."

[3592 op=1 keyword; failure -> 3596]
  1: yes

[3595 op=2 display+exit]
  1: "Your ambitions outreach your means."

[3596 op=1 keyword; failure -> 3601]
  1: yes

[3598 op=22 display+continue]
  1: She closes her eyes in concentration.
  2: For long minutes you wait.
  3: Finally Penumbra opens her eyes again.
  4: "The word you seek is 'ahm.'"

[3600 op=2 display+exit]
  1: "What are we waiting for!" whispers Dupre.
  2: "She's doubtless fallen asleep, and you've wasted good money for nothing..."

[3601 op=1 keyword; failure -> 3603]
  1: yes

[3602 op=2 display+exit]
  1: She closes her eyes in concentration.
  2: For long minutes you wait.
  3: Finally Penumbra opens her eyes again.
  4: "The word you seek is 'ahm.'"

[3603 op=2 display+exit]
  1: "It is a small price for so profound a word."

[3604 op=1 keyword; failure -> 3607]
  1: name

[3606 op=2 display+exit]
  1: "I am the one known as @Penumbra."

[3607 op=1 keyword; failure -> 3609]
  1: penu

[3608 op=2 display+exit]
  1: "I am the daughter of the eclipse."

[3609 op=1 keyword; failure -> 3611]
  1: job

[3610 op=2 display+exit]
  1: "I am a @fortune teller."

[3611 op=1 keyword; failure -> 3613]
  1: fort,tell

[3612 op=2 display+exit]
  1: "Yes, I can foretell your @future for you."

[3613 op=1 keyword; failure -> 3615]
  1: beyv

[3614 op=2 display+exit]
  1: "Yes he was a terribly @honest fellow."
  2: "Alas, though, he lacked the virtue of tact."
  3: "He told the truth once too often, and those he spoke of did not appreciate it."
  4: "He's buried in the catacombs under Moonglow now, pushing up @daffodils."
  5: "Even virtue has its price."

[3615 op=1 keyword; failure -> 3617]
  1: daff

[3616 op=2 display+exit]
  1: "Yes, daffodils."

[3617 op=1 keyword; failure -> 3619]
  1: hone

[3618 op=2 display+exit]
  1: "I can tell you of both the @rune and the @mantra."

[3619 op=1 keyword; failure -> 3621]
  1: rune

[3620 op=2 display+exit]
  1: "'Twas buried with @Beyvin, rest his shade."

[3621 op=1 keyword; failure -> 3623]
  1: bye

[3622 op=2 display+exit]
  1: "Your destiny awaits."

[3623 op=1 keyword; failure -> 3626]
  1: fore,futu

[3625 op=2 display+exit]
  1: "I require a small donation for my services."
  2: How much do you give her?

[3626 op=1 keyword; failure -> 3629]
  1: mant

[3628 op=2 display+exit]
  1: "For five gold coins I can use my powers to divine the mantra for you."
  2: "Shall I do this?"

[3629 op=2 display+exit]
  1: "You must seek the answer to that elsewhere."

```

<a id="petroph-npc-117"></a>
## Petroph ? NPC 117, port 118

Instruction range: 10954?10993

```text
[10954 op=1 keyword; failure -> 10957]
  1: look

[10956 op=2 display+exit]
  1: a giant of a man with a solemn stare.
  2: "What you back for? I thought you not like my @inn."

[10957 op=1 keyword; failure -> 10961]
  1: look

[10960 op=2 display+exit]
  1: a giant of a man with a solemn stare.
  2: "Heylo to you."

[10961 op=1 keyword; failure -> 10963]
  1: look

[10962 op=2 display+exit]
  1: a giant of a man with a solemn stare.
  2: "Heylo again."

[10963 op=1 keyword; failure -> 10966]
  1: name

[10965 op=2 display+exit]
  1: "@Petroph."

[10966 op=1 keyword; failure -> 10968]
  1: petr

[10967 op=2 display+exit]
  1: "Da, Petroph."

[10968 op=1 keyword; failure -> 10970]
  1: job

[10969 op=2 display+exit]
  1: "I am keeping the @inn, here at King's Ransom."

[10970 op=1 keyword; failure -> 10973]
  1: room,inn

[10972 op=2 display+exit]
  1: "6 gold for you to stay and have meal, good?"

[10974 op=1 keyword; failure -> 10976]
  1: yes

[10977 op=1 keyword; failure -> 10980]
  1: no

[10979 op=2 display+exit]
  1: "What, maybe you don't like my inn?"
  2: "Well, maybe I don't like you either!"

[10981 op=1 keyword; failure -> 10984]
  1: bye

[10983 op=2 display+exit]
  1: "Da, goodbye and good riddance!"

[10984 op=1 keyword; failure -> 10986]
  1: bye

[10985 op=2 display+exit]
  1: "Have a nice day."

[10986 op=2 display+exit]
  1: "You speak too quick. What was that?"

[10990 op=2 display+exit]
  1: "Must pay in advance for rooms. Sorry, is policy."

[10993 op=2 display+exit]
  1: "Goodnight!"

```

<a id="peyton-npc-15"></a>
## Peyton ? NPC 15, port 16

Instruction range: 1244?1274

```text
[1244 op=1 keyword; failure -> 1247]
  1: look

[1246 op=2 display+exit]
  1: a well-dressed man with a very even temper.
  2: "Good $T, friend, and welcome back to the Wayfarer's @Inn!"

[1247 op=1 keyword; failure -> 1249]
  1: look

[1248 op=2 display+exit]
  1: a well-dressed man with a very even temper.
  2: "Greetings, traveler, and welcome to the Wayfarer's @Inn!"

[1249 op=1 keyword; failure -> 1252]
  1: name

[1251 op=2 display+exit]
  1: "I am Peyton, and this is my place."
  2: He gestures with his hands.

[1252 op=1 keyword; failure -> 1254]
  1: peyt

[1253 op=2 display+exit]
  1: "Did you need something?"

[1254 op=1 keyword; failure -> 1256]
  1: job

[1255 op=2 display+exit]
  1: "I can offer you a @room for the evening."

[1256 op=1 keyword; failure -> 1259]
  1: room,inn,stay

[1258 op=2 display+exit]
  1: "8 gold for bed and breakfast."
  2: "Interested?"

[1260 op=1 keyword; failure -> 1262]
  1: yes

[1263 op=1 keyword; failure -> 1265]
  1: no

[1264 op=2 display+exit]
  1: "Well, perhaps next time you're in town."

[1266 op=1 keyword; failure -> 1268]
  1: bye

[1267 op=2 display+exit]
  1: "Goodbye. Take care of yourself."

[1268 op=2 display+exit]
  1: "I'm sure you'll have to ask another."

[1272 op=2 display+exit]
  1: You don't have enough gold for that.
  2: "I'm sorry, cash in advance. That's our policy."

[1274 op=2 display+exit]
  1: "Goodnight!"

```

<a id="phoenix-npc-180"></a>
## Phoenix ? NPC 180, port 181

Instruction range: 15884?15923

```text
[15884 op=1 keyword; failure -> 15886]
  1: look

[15885 op=2 display+exit]
  1: a graceful, silver haired woman in robes of blue.
  2: She eyes you suspiciously.
  3: "What do you want?"

[15886 op=1 keyword; failure -> 15888]
  1: name

[15887 op=2 display+exit]
  1: "I am Phoenix."

[15888 op=1 keyword; failure -> 15890]
  1: job

[15889 op=2 display+exit]
  1: "I used to be captain of the Black @Thechu."
  2: "But now I'm a member of the @thieves @guild, and I don't care who knows it."
  3: "Britain is the wealthiest city in the world, and I make a tidy living sneaking up there at night to 'liberate' a few possessions."
  4: "Of course, I only steal from those who have too much wealth for their own good."

[15890 op=1 keyword; failure -> 15892]
  1: libe,stea,thie

[15891 op=2 display+exit]
  1: "It's an honorable profession, with as long a tradition as any other."

[15892 op=1 keyword; failure -> 15894]
  1: capt,blac,thec

[15893 op=2 display+exit]
  1: "She was the finest ship that ever sailed. But those days are past."
  2: "The crew's scattered to the four corners of the earth, and the Thechu's off amongst the @stars."

[15894 op=1 keyword; failure -> 15896]
  1: crew,star

[15895 op=2 display+exit]
  1: "We were at port one day when the anchor chain snapped, and the Thechu drifted off the @edge of the world."
  2: "I'd join her if I could, and sail through space..."

[15896 op=1 keyword; failure -> 15898]
  1: four,corn,eart,edge,worl

[15897 op=2 display+exit]
  1: "The world is flat, you know."
  2: She eyes you appraisingly.
  3: "Surely any well educated person would be aware of that."

[15898 op=1 keyword; failure -> 15901]
  1: belt

[15900 op=2 display+exit]
  1: "You want my belt? I'm sorry, lass, but I'm not ready to @retire just yet."

[15901 op=1 keyword; failure -> 15903]
  1: belt

[15902 op=2 display+exit]
  1: "You want my belt? I'm sorry, laddy, but I'm not ready to @retire just yet."

[15903 op=1 keyword; failure -> 15905]
  1: reti

[15904 op=2 display+exit]
  1: "No way! I'm making too good a living to give it up now."
  2: "If you want to get my belt, use your @wits."
  3: "Prove you're worthy to replace me."

[15905 op=1 keyword; failure -> 15907]
  1: wit,prov,wort,repl

[15906 op=2 display+exit]
  1: "Figure it out yourself."

[15907 op=1 keyword; failure -> 15909]
  1: guil

[15908 op=2 display+exit]
  1: "If you want to join the guild, go talk to @Budo about it."

[15909 op=1 keyword; failure -> 15911]
  1: budo

[15910 op=2 display+exit]
  1: "He lives in Buccaneer's @Den."
  2: "These tunnels can take you there - unless you get lost, or the rats get you..."

[15911 op=1 keyword; failure -> 15913]
  1: bucc,den

[15912 op=2 display+exit]
  1: "The rumors of @pirates there are just idle @chatter."
  2: She smiles.
  3: "I haven't been there in a while. It's a lovely little town..."

[15913 op=1 keyword; failure -> 15915]
  1: chat

[15914 op=2 display+exit]
  1: "I don't know about you, but some of us have more important things to do with our time."

[15915 op=1 keyword; failure -> 15917]
  1: pira

[15916 op=2 display+exit]
  1: "Pirates? I'm sure I wouldn't know a thing about them."

[15918 op=22 display+continue]
  1: "About time you were on your way."
  2: "One thing before you go..."
  3: She draws her dagger.
  4: "Don't tell anyone where my hideout is. If you do, some night you'll go to sleep and never wake up again."
  5: "You'll never know when it's coming."
  6: She turns and throws the dagger at a little mouse that was sniffing around for food, pinning it to the wall by its tail.

[15920 op=22 display+continue]
  1: "Eeeeek!"
  2: Sherry runs up your leg, and it takes several minutes before you can coax her back down again.

[15921 op=1 keyword; failure -> 15923]
  1: bye

[15922 op=2 display+exit]
  1: "About time you were on your way. One thing before you go..."
  2: She draws her dagger.
  3: "Don't tell anyone where my hideout is. If you do, some night you'll go to sleep and never wake up again."
  4: "You'll never know when it's coming."
  5: She turns and throws the dagger at a little mouse that was sniffing around for food, pinning it to the wall by its tail.

[15923 op=2 display+exit]
  1: "I haven't got all day to stand around talking. Get to the point, or go bother someone else."

```

<a id="pridgarm-npc-59"></a>
## Pridgarm ? NPC 59, port 60

Instruction range: 5573?5701

```text
[5574 op=1 keyword; failure -> 5577]
  1: yes

[5579 op=2 display+exit]
  1: "Okay."
  2: "Never had nobody sound so interested in jest hearing about the jail before."

[5581 op=1 keyword; failure -> 5584]
  1: yes

[5583 op=2 display+exit]
  1: He thinks for a moment.
  2: "Nope. Sorry, not interested."

[5586 op=2 display+exit]
  1: "Pity."
  2: "Never been offered a bribe before."

[5588 op=1 keyword; failure -> 5591]
  1: yes

[5593 op=2 display+exit]
  1: "Well, gotta have a letter if you want to see the thief."

[5595 op=1 keyword; failure -> 5598]
  1: yes

[5600 op=2 display+exit]
  1: "Well, bring it back to me when you are."
  2: Without another word, the jailer returns to his carving.

[5603 op=2 display+exit]
  1: "Well, that does me no good."
  2: "Need it here."
  3: Without another word, the jailer returns to his carving.

[5605 op=1 keyword; failure -> 5608]
  1: look

[5607 op=2 display+exit]
  1: an old man, tall and thin.
  2: He spends all his time whittling.
  3: "Good $T, $G."
  4: "Done with the key yet?"

[5610 op=2 display+exit]
  1: "Good $T, $G."
  2: "Done with the key yet?"

[5611 op=1 keyword; failure -> 5613]
  1: look

[5612 op=2 display+exit]
  1: an old man, tall and thin.
  2: He spends all his time whittling.
  3: "Good $T, $G."
  4: "What can I do for you?"

[5613 op=1 keyword; failure -> 5615]
  1: thie,robb

[5614 op=2 display+exit]
  1: "He's in @solitary, right over there."

[5615 op=1 keyword; failure -> 5617]
  1: lock,see,meet,talk,soli,ther

[5616 op=2 display+exit]
  1: "You'll need my @key to get in there."

[5617 op=1 keyword; failure -> 5619]
  1: key,your,give,plea

[5618 op=2 display+exit]
  1: "Yup, I could give you the key...."
  2: "But not without Her Ladyship's @permission."

[5619 op=1 keyword; failure -> 5622]
  1: perm,impo,ques,avat,lett,lady,must,need

[5621 op=2 display+exit]
  1: "You got a letter of permission from Her Ladyship?"

[5622 op=1 keyword; failure -> 5625]
  1: brib,mone,pay,give

[5624 op=2 display+exit]
  1: "You offering me a bribe?"

[5625 op=1 keyword; failure -> 5628]
  1: name

[5627 op=2 display+exit]
  1: "Name's Pridgarm."

[5628 op=1 keyword; failure -> 5630]
  1: job

[5629 op=2 display+exit]
  1: "I mind the @jail."
  2: "Got a @prisoner in @solitary right now."

[5630 op=1 keyword; failure -> 5632]
  1: jail

[5631 op=2 display+exit]
  1: "Got four of the cleanest @cells you could ask fer."

[5632 op=1 keyword; failure -> 5635]
  1: cell

[5634 op=2 display+exit]
  1: "Say, you looking to get the keys to my jail cells?"

[5635 op=1 keyword; failure -> 5637]
  1: pris

[5636 op=2 display+exit]
  1: "Feller broke into the @grave of His Lordship, the late Mayor."

[5637 op=1 keyword; failure -> 5639]
  1: rune,fami,free,feed,kids,rele,deal,go,allo,trad,brok,grav,lord

[5638 op=2 display+exit]
  1: "'Taint my place to talk about that."
  2: "You should go see Her Ladyship, the Mayor."

[5639 op=1 keyword; failure -> 5641]
  1: bye

[5640 op=2 display+exit]
  1: The jailer nods and says "$T."

[5643 op=2 display+exit]
  1: "Don't know nothing about that."

[5644 op=2 display+exit]
  1: The old man simply shrugs and keeps on whittling.

[5665 op=2 display+exit]
  1: "Well, ya know, I shouldn't do this."
  2: "But Her Ladyship is a little too strict about who she puts in here."
  3: "So I make up fer it by being a little careless with the keys, if you take my meaning."
  4: He tosses you some keys, then turns his back on you.

[5676 op=2 display+exit]
  1: "Okay, here's the key."
  2: "Give it back when you're done."

[5679 op=2 display+exit]
  1: "Gotta have room to carry it."

[5680 op=2 display+exit]
  1: "Gotta have a letter if you want to see the thief."

[5690 op=2 display+exit]
  1: "Much obliged," the taciturn jailer says as he takes the key.

[5699 op=2 display+exit]
  1: "Much obliged," the taciturn jailer says as he takes the key.

[5701 op=2 display+exit]
  1: "Well, where is it?"

```

<a id="pushme-pullyu-npc-128"></a>
## Pushme Pullyu ? NPC 128, port 129

Instruction range: 11802?11836

```text
[11802 op=1 keyword; failure -> 11806]
  1: look

[11803 op=22 display+continue]
  1: a bizarre, two-headed beast.
  2: Both heads take turns speaking to you.
  3: "Stand over there!"

[11805 op=2 display+exit]
  1: "Stand over here!"
  2: "Come hear our @puzzle anywhere!"

[11806 op=1 keyword; failure -> 11810]
  1: name

[11807 op=22 display+continue]
  1: "Should I answer you today?"
  2: "If asked tomorrow, I would say..."
  3: "That my partner and I are the @Pushme @Pullyu, and speaking in @riddles is what we do."

[11809 op=2 display+exit]
  1: "But which is @Pullyu, and which is @Pushme?"
  2: "If you'd asked yesterday, I'd have told thee!"

[11810 op=1 keyword; failure -> 11812]
  1: push

[11811 op=2 display+exit]
  1: "If you asked my partner, thinking him wise, he'd tell you the Pushme always @lies!"

[11812 op=1 keyword; failure -> 11815]
  1: pull

[11814 op=2 display+exit]
  1: "If you asked my partner, then forsooth, he'd say the Pullyu always speaks @truth!"

[11815 op=1 keyword; failure -> 11817]
  1: ly,lie,fals

[11816 op=2 display+exit]
  1: "My partner lies, and that is true."
  2: "It is because he's the @Pullyu!"

[11817 op=1 keyword; failure -> 11819]
  1: tru

[11818 op=2 display+exit]
  1: "I'll always tell the truth to thee."
  2: "That's because I am the @Pushme!"

[11819 op=1 keyword; failure -> 11823]
  1: job,ridd,puzz

[11820 op=22 display+continue]
  1: "If my partner said you should go @west, then he would be a lying pest!"
  2: "If my partner said 'Go @east.' to you, then he'd have said that which is true."

[11822 op=2 display+exit]
  1: "If my partner said you should go @east, then he would be a lying beast!"
  2: "If my partner said to you 'Go @west.', then his advice would be the best."

[11823 op=1 keyword; failure -> 11825]
  1: east,west

[11824 op=2 display+exit]
  1: "If you asked my partner which was best, he'd say 'East lie beasts, so go quest west.'"

[11825 op=1 keyword; failure -> 11827]
  1: bye

[11827 op=2 display+exit]
  1: "Is that a @lie, or is it @true?"
  2: "Could we be more confused than you?"

[11828 op=22 display+continue]
  1: "So soon to leave?"
  2: "Take three blind mice, a dish and spoon!"
  3: "Then go dance with the cat and fiddle!"

[11831 op=22 display+continue]
  1: Dupre is staring at them, his jaw hanging wide open.

[11834 op=22 display+continue]
  1: Iolo says "That's got to be the most baffling thing I've ever heard."

[11836 op=2 display+exit]
  1: "You'd leave so soon?"
  2: "And if, perhaps, you've solved our riddle,"
  3: Both heads smile innocently at you.

```

<a id="quenton-npc-89"></a>
## Quenton ? NPC 89, port 90

Instruction range: 8564?8582

```text
[8564 op=1 keyword; failure -> 8566]
  1: look

[8565 op=2 display+exit]
  1: a vaporous form hovering before you.
  2: A shiver runs up your spine as you feel a chilling draft...
  3: The form is that of a man.
  4: His mouth moves, yet no words can be heard...

[8566 op=1 keyword; failure -> 8568]
  1: murd,kill,die,dead

[8567 op=2 display+exit]
  1: You speak the word and the spectre responds by pointing a boney hand to his @eye.

[8568 op=1 keyword; failure -> 8570]
  1: eye

[8569 op=2 display+exit]
  1: A wind from behind blows your hair, covering your eyes.

[8570 op=1 keyword; failure -> 8572]
  1: garg

[8571 op=2 display+exit]
  1: A whirlpool of leaves, twigs and stones whips up around you.
  2: Your clothing is yanked by the strong wind forcing you to grip it tightly in your hands.

[8572 op=1 keyword; failure -> 8574]
  1: marn

[8573 op=2 display+exit]
  1: A cool breeze blows through your hair.
  2: You suddenly smell the scent of a storm approaching.

[8574 op=1 keyword; failure -> 8576]
  1: how

[8575 op=2 display+exit]
  1: The ghostly image shimmers for a moment and then he points a boney finger to his neck.

[8576 op=1 keyword; failure -> 8578]
  1: who

[8577 op=2 display+exit]
  1: The spectre's arm thrusts out, pointing to the @sea.

[8578 op=1 keyword; failure -> 8580]
  1: sea

[8579 op=2 display+exit]
  1: The spectre's arm thrusts again.
  2: This time you see it points towards the pier.

[8580 op=1 keyword; failure -> 8582]
  1: bye

[8581 op=2 display+exit]
  1: A strong breeze suddenly assaults you with twigs and leaves.

[8582 op=2 display+exit]
  1: You hear no answer save the rustle of the leaves on the ground around you.

```

<a id="rob-npc-36"></a>
## Rob ? NPC 36, port 37

Instruction range: 3355?3450

```text
[3356 op=1 keyword; failure -> 3358]
  1: yes

[3360 op=2 display+exit]
  1: "Anything else I can do for you?"

[3362 op=1 keyword; failure -> 3364]
  1: yes

[3366 op=2 display+exit]
  1: "Anything else I can do for you?"

[3368 op=1 keyword; failure -> 3370]
  1: yes

[3372 op=2 display+exit]
  1: "Anything else I can do for you?"

[3374 op=1 keyword; failure -> 3376]
  1: yes

[3378 op=2 display+exit]
  1: "Anything else I can do for you?"

[3381 op=1 keyword; failure -> 3383]
  1: look

[3382 op=2 display+exit]
  1: a short, congenial fellow.
  2: "Welcome to the Blue Bottle Tavern."

[3383 op=1 keyword; failure -> 3386]
  1: name

[3385 op=2 display+exit]
  1: "I'm Rob Frasier."

[3386 op=1 keyword; failure -> 3388]
  1: fras,foll

[3387 op=2 display+exit]
  1: "I brew Frasier's Folly myself."
  2: "'Tis a very popular @ale hereabouts."

[3388 op=1 keyword; failure -> 3390]
  1: buy

[3389 op=2 display+exit]
  1: "Which, @mead, @wine, @bread, baked @brie, or @ale?"

[3390 op=1 keyword; failure -> 3392]
  1: job

[3391 op=2 display+exit]
  1: "I sell @mead, @wine, @bread, baked @brie, and the house brew, @Frasier's @Folly."

[3392 op=1 keyword; failure -> 3395]
  1: brie

[3394 op=2 display+exit]
  1: "It's a fine cheese. We serve it warm with sliced almonds on top."
  2: "It costs 6 crowns. Would you like some?"

[3395 op=1 keyword; failure -> 3398]
  1: mead

[3397 op=2 display+exit]
  1: That'll be 5 crowns, okay?

[3398 op=1 keyword; failure -> 3401]
  1: ale,brew

[3400 op=2 display+exit]
  1: That'll be 7 crowns, okay?

[3401 op=1 keyword; failure -> 3404]
  1: wine

[3403 op=2 display+exit]
  1: That'll be 6 crowns, okay?

[3404 op=1 keyword; failure -> 3407]
  1: brea,rati

[3406 op=2 display+exit]
  1: I charge 3 gold per loaf. How many do you want?

[3407 op=1 keyword; failure -> 3409]
  1: bye

[3408 op=2 display+exit]
  1: "See you later."

[3409 op=2 display+exit]
  1: "I can't help you with that."

[3413 op=2 display+exit]
  1: "You can't afford that right now."

[3415 op=2 display+exit]
  1: "You don't have room to carry it."

[3416 op=2 display+exit]
  1: $NAME serves $Y a plate of brie.

[3420 op=2 display+exit]
  1: "You can't afford that right now."

[3422 op=2 display+exit]
  1: "You don't have room to carry it."

[3423 op=2 display+exit]
  1: "Good!" $NAME gives $Y the mead.

[3427 op=2 display+exit]
  1: "You can't afford that right now."

[3429 op=2 display+exit]
  1: "You don't have room to carry it."

[3430 op=2 display+exit]
  1: "Good!" $NAME gives $Y the ale.

[3434 op=2 display+exit]
  1: "You can't afford that right now."

[3436 op=2 display+exit]
  1: "You don't have room to carry it."

[3437 op=2 display+exit]
  1: "Good!" $NAME gives $Y the wine.

[3440 op=2 display+exit]
  1: "Anything else I can do for you?

[3442 op=2 display+exit]
  1: "I don't have that many!"

[3447 op=2 display+exit]
  1: "You can't afford that right now."

[3449 op=2 display+exit]
  1: "You haven't any room in your pack."

[3450 op=2 display+exit]
  1: $NAME gives the bread to $Y.

```

<a id="rudyom-npc-126"></a>
## Rudyom ? NPC 126, port 127

Instruction range: 11488?11739

```text
[11496 op=2 display+exit]
  1: "Perhaps something else then."

[11498 op=2 display+exit]
  1: "That's too many!"

[11509 op=2 display+exit]
  1: "That costs more than thou hast!"

[11511 op=2 display+exit]
  1: "It would be too heavy for thee!"

[11513 op=2 display+exit]
  1: "Use it wisely."
  2: He carefully hands you the potion.

[11514 op=2 display+exit]
  1: "Use them wisely"
  2: He carefully hands you the potions.

[11520 op=2 display+exit]
  1: "Which of these, @awaken, @create @food, @douse, @heal or @ignite?"

[11523 op=2 display+exit]
  1: "I have no spells of that circle."

[11526 op=2 display+exit]
  1: "Which, @mass @awaken?"

[11529 op=2 display+exit]
  1: "Which, @great @heal?"

[11532 op=2 display+exit]
  1: "Which of these, @pickpocket or @seance?"

[11535 op=2 display+exit]
  1: "I have no spells of that circle."

[11538 op=2 display+exit]
  1: "I have no spells of that circle."

[11541 op=2 display+exit]
  1: "Which, @resurrect?"

[11542 op=2 display+exit]
  1: "As everybody knows, there are only eight circles."

[11545 op=1 keyword; failure -> 11550]
  1: awak

[11549 op=2 display+exit]
  1: "That spell costs 20 gold. Interested?"

[11550 op=1 keyword; failure -> 11555]
  1: crea

[11554 op=2 display+exit]
  1: "That spell costs 20 gold. Interested?"

[11555 op=1 keyword; failure -> 11560]
  1: dous

[11559 op=2 display+exit]
  1: "That spell costs 20 gold. Interested?"

[11560 op=1 keyword; failure -> 11565]
  1: heal

[11564 op=2 display+exit]
  1: "That spell costs 20 gold. Interested?"

[11565 op=1 keyword; failure -> 11570]
  1: igni

[11569 op=2 display+exit]
  1: "That spell costs 20 gold. Interested?"

[11571 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11575 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11578 op=1 keyword; failure -> 11583]
  1: mass

[11582 op=2 display+exit]
  1: "That spell costs 60 gold. Interested?"

[11584 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11587 op=1 keyword; failure -> 11592]
  1: grea

[11591 op=2 display+exit]
  1: "That spell costs 80 gold. Interested?"

[11593 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11596 op=1 keyword; failure -> 11601]
  1: pick

[11600 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[11601 op=1 keyword; failure -> 11606]
  1: sean

[11605 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[11607 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11611 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11615 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11618 op=1 keyword; failure -> 11623]
  1: resu

[11622 op=2 display+exit]
  1: "That spell costs 160 gold. Interested?"

[11624 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11628 op=1 keyword; failure -> 11633]
  1: yes

[11632 op=2 display+exit]
  1: He hands you a piece of rune-covered parchment.

[11634 op=2 display+exit]
  1: "You lack the gold required."

[11636 op=2 display+exit]
  1: "Thou cannot carry that."

[11637 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[11641 op=1 keyword; failure -> 11646]
  1: yes

[11645 op=2 display+exit]
  1: He hands you the spellbook.

[11647 op=2 display+exit]
  1: "But thou cannot afford a spellbook!"

[11649 op=2 display+exit]
  1: "But thou cannot carry a spellbook!"

[11650 op=2 display+exit]
  1: "Perhaps something else then."

[11651 op=1 keyword; failure -> 11654]
  1: poti

[11653 op=2 display+exit]
  1: "Which, @blue or @black?"

[11657 op=1 keyword; failure -> 11661]
  1: blac

[11660 op=2 display+exit]
  1: "Black potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[11661 op=1 keyword; failure -> 11665]
  1: blue

[11664 op=2 display+exit]
  1: "Blue potions will cost thee 10 gold each."
  2: "How many wouldst thou like?"

[11665 op=2 display+exit]
  1: "Perhaps something else then."

[11666 op=1 keyword; failure -> 11668]
  1: buy,reag

[11667 op=2 display+exit]
  1: "Which one, @blood @moss, @garlic, @ginseng, @nightshade, @spider @silk or @sulfurous @ash?"
  2: "I also sell @potions."

[11668 op=1 keyword; failure -> 11673]
  1: bloo,moss

[11672 op=2 display+exit]
  1: "Blood moss costs 3 per portion."
  2: "How many dost thou want?"

[11673 op=1 keyword; failure -> 11678]
  1: garl

[11677 op=2 display+exit]
  1: "Garlic costs 1 per portion."
  2: "How many dost thou want?"

[11678 op=1 keyword; failure -> 11683]
  1: gins

[11682 op=2 display+exit]
  1: "Ginseng costs 2 per portion."
  2: "How many dost thou want?"

[11683 op=1 keyword; failure -> 11688]
  1: nigh

[11687 op=2 display+exit]
  1: "Nightshade costs 2 per portion."
  2: "How many dost thou want?"

[11688 op=1 keyword; failure -> 11693]
  1: spid,silk

[11692 op=2 display+exit]
  1: "Spider silk costs 2 per portion."
  2: "How many dost thou want?"

[11693 op=1 keyword; failure -> 11698]
  1: sulf,ash

[11697 op=2 display+exit]
  1: "Sulfurous ash costs 1 per portion."
  2: "How many dost thou want?"

[11702 op=2 display+exit]
  1: "Perhaps something else then."

[11704 op=2 display+exit]
  1: "I cannot give thee that much!"

[11709 op=2 display+exit]
  1: "Thou cannot afford that much!"

[11711 op=2 display+exit]
  1: "Thou cannot carry that much!"

[11712 op=2 display+exit]
  1: "Here you are."

[11714 op=1 keyword; failure -> 11716]
  1: look

[11715 op=2 display+exit]
  1: a dark, bearded man dressed in flowing white robes.
  2: "Greetings, mighty Avatar. For what hast thou come to my place?"

[11716 op=1 keyword; failure -> 11719]
  1: name

[11718 op=2 display+exit]
  1: "I am Rudyom."

[11719 op=1 keyword; failure -> 11721]
  1: job

[11720 op=2 display+exit]
  1: "I sell @spellbooks, @reagents and @potions, and teach @spells."

[11722 op=1 keyword; failure -> 11724]
  1: book,spellbook,spellbooks,lear,spel,buy,reag

[11723 op=2 display+exit]
  1: "My shop is closed right now."

[11724 op=1 keyword; failure -> 11727]
  1: book,spellbook,spellbooks

[11726 op=2 display+exit]
  1: "A spellbook costs 50 gold pieces."
  2: "Interested?"

[11728 op=1 keyword; failure -> 11731]
  1: lear,spel

[11730 op=2 display+exit]
  1: "In which Circle do you wish to study?"

[11732 op=1 keyword; failure -> 11735]
  1: lear,spel

[11734 op=2 display+exit]
  1: "In which Circle do you wish to study?"

[11735 op=1 keyword; failure -> 11737]
  1: lear,spel

[11736 op=2 display+exit]
  1: "But none of you has a @spellbook!"

[11737 op=1 keyword; failure -> 11739]
  1: bye

[11738 op=2 display+exit]
  1: "May peace follow thee, Avatar."

[11739 op=2 display+exit]
  1: "Of that I will say nothing."

```

<a id="rufus-npc-21"></a>
## Rufus ? NPC 21, port 22

Instruction range: 1841?1879

```text
[1841 op=1 keyword; failure -> 1844]
  1: look

[1843 op=2 display+exit]
  1: a rough and tumble man with wild, unkempt hair.
  2: "'Tis good to see thee, friend!" he says, grabbing your arm.
  3: "What d'ya need?"

[1844 op=1 keyword; failure -> 1847]
  1: look

[1846 op=2 display+exit]
  1: a rough and tumble man with wild, unkempt hair.
  2: "Aye, and a fine $T to you, my friend."

[1847 op=1 keyword; failure -> 1850]
  1: name

[1849 op=2 display+exit]
  1: "Rufus the @Red."

[1850 op=1 keyword; failure -> 1852]
  1: rufu,red

[1851 op=2 display+exit]
  1: "Can ya not tell by my hair?"

[1852 op=1 keyword; failure -> 1854]
  1: job

[1853 op=2 display+exit]
  1: "If you want to @buy horseshoes, just ask."

[1854 op=1 keyword; failure -> 1856]
  1: shoe

[1855 op=2 display+exit]
  1: "Aye, it saves their feet from stones and such."

[1858 op=1 keyword; failure -> 1861]
  1: buy

[1860 op=2 display+exit]
  1: "It'll cost you 2 gold for the horseshoes. Interested?"

[1862 op=1 keyword; failure -> 1864]
  1: yes

[1864 op=1 keyword; failure -> 1866]
  1: buy

[1865 op=2 display+exit]
  1: "I'm busy just now."
  2: "Why don't ya come t'my smithy later and we'll talk then."

[1867 op=1 keyword; failure -> 1869]
  1: no

[1868 op=2 display+exit]
  1: "Those shoes will last ya."
  2: "You'll find none stronger."

[1870 op=1 keyword; failure -> 1872]
  1: bye

[1871 op=2 display+exit]
  1: "Remember to shoe yer steed often. Bye."

[1872 op=2 display+exit]
  1: "I just @shoe horses, sorry."

[1876 op=2 display+exit]
  1: "You haven't enough gold!"

[1878 op=2 display+exit]
  1: "You look pretty full to me."

[1879 op=2 display+exit]
  1: "Good, good!"
  2: After accepting your gold, he hands over the horseshoes.

```

<a id="sacrifice-npc-194"></a>
## Sacrifice ? NPC 194, port 195

Instruction range: 16643?16685

```text
[16647 op=1 keyword; failure -> 16649]
  1: look

[16648 op=2 display+exit]
  1: the altar of Sacrifice.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about sacrifice."
  4: Do you wish to meditate at this altar?

[16649 op=1 keyword; failure -> 16652]
  1: yes

[16651 op=2 display+exit]
  1: "Who will meditate?"

[16652 op=1 keyword; failure -> 16654]
  1: no

[16653 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16655 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable sacrifice."
  3: Do you wish to meditate at this altar?

[16657 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16658 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about sacrifice."
  3: Do you wish to meditate at this altar?

[16661 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16664 op=2 display+exit]
  1: "Speak the Mantra of Sacrifice."

[16667 op=1 keyword; failure -> 16669]
  1: cah

[16670 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16671 op=2 display+exit]
  1: "You receive no enlightenment."

[16677 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16678 op=2 display+exit]
  1: "The universe is so vast and so ageless
  2: that the life of one man can only be justified
  3: by the measure of his sacrifice."
  4: "There is naught else for you to learn here and now."
  5: "Return when thy journey has progressed further."

[16684 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16685 op=2 display+exit]
  1: "The universe is so vast and so ageless
  2: that the life of one man can only be justified
  3: by the measure of his sacrifice."
  4: $PARTYMEMBER has gained a level...
  5: strength...
  6: and dexterity!

```

<a id="sandy-npc-73"></a>
## Sandy ? NPC 73, port 74

Instruction range: 6917?6979

```text
[6921 op=2 display+exit]
  1: "That big, carefully wrapped bundle could only be a dragon's egg!"
  2: "Will you give it to me?"

[6922 op=1 keyword; failure -> 6926]
  1: pira

[6925 op=2 display+exit]
  1: "Well, first, there is one in Serpent's Hold."
  2: "I cannot recall her name, but she has a piece of the map with her."
  3: "Second, there is one on Dagger Isle."
  4: "A hermit."
  5: "Third, I know of a pirate at Empath Abbey, or thereabouts."
  6: "Nathaniel Moorehead was his name."
  7: "The @fourth... but then maybe I've said too much already."

[6927 op=1 keyword; failure -> 6930]
  1: yes

[6929 op=2 display+exit]
  1: "Well, first, there is one in Serpent's Hold."
  2: "I cannot recall her name, but she has a piece of the map with her."
  3: "Second, there is one on Dagger Isle."
  4: "A hermit."
  5: "Third, I know of a pirate at Empath Abbey, or thereabouts."
  6: "Nathaniel Moorehead was his name."
  7: "The @fourth... but then maybe I've said too much already."

[6932 op=2 display+exit]
  1: He shrugs.
  2: "As you wish."

[6934 op=1 keyword; failure -> 6940]
  1: yes

[6939 op=2 display+exit]
  1: "Now I can make Humble Pie!" Overjoyed, he gives you a hug.
  2: "So, does the subject of @pirates interest you?"

[6942 op=2 display+exit]
  1: "I guess I cannot help you much, then."

[6944 op=1 keyword; failure -> 6947]
  1: egg

[6946 op=2 display+exit]
  1: "Very good!"
  2: "It's an egg, of course."
  3: "To make Magincian pastry, I require one dragon's @egg."

[6949 op=2 display+exit]
  1: "It's an egg, of course."
  2: "To make Magincian pastry, I require one dragon's @egg."

[6950 op=1 keyword; failure -> 6952]
  1: look

[6951 op=2 display+exit]
  1: a shifty-eyed man with a strange smile.
  2: He smells of rancid grease and cooking smoke.
  3: "Hello there, $G."

[6952 op=1 keyword; failure -> 6956]
  1: egg

[6955 op=2 display+exit]
  1: "That big, carefully wrapped bundle could only be a dragon's egg!"
  2: "Will you give it to me?"

[6956 op=1 keyword; failure -> 6960]
  1: four,said,much,alre

[6959 op=2 display+exit]
  1: "Before Lord Whitsaber came to Trinsic to be our mayor, he had another job..."
  2: "As first mate to Captain Hawkins himself!"
  3: "His real name is Alastor Gordon."

[6960 op=1 keyword; failure -> 6962]
  1: map,lord,whit,pira

[6961 op=2 display+exit]
  1: "Hmmm."
  2: "I'd better get back to cooking..."
  3: "Excuse me."
  4: He walks away mumbling, "Magincian @pastry..."

[6962 op=1 keyword; failure -> 6964]
  1: name

[6963 op=2 display+exit]
  1: "My name is Sandstone Angus."
  2: "Most folks call me Sandy."

[6964 op=1 keyword; failure -> 6966]
  1: job

[6965 op=2 display+exit]
  1: "I cook for Lord @Whitsaber."
  2: "He especially likes my recipe for fish in a white wine @sauce."

[6966 op=1 keyword; failure -> 6968]
  1: fish,wine,sauc

[6967 op=2 display+exit]
  1: "It's my specialty!"

[6968 op=1 keyword; failure -> 6970]
  1: pie,magi,past

[6969 op=2 display+exit]
  1: "Truly it shall be a magnificent desert!"
  2: "I could do you a @favor if you did one for me first."

[6970 op=1 keyword; failure -> 6973]
  1: favo,firs

[6972 op=2 display+exit]
  1: "Let's see..."
  2: "'A golden orb on a crystal sea, in a box sans hinges, lid, or key.'"
  3: "I'll give you one guess to this riddle."

[6973 op=1 keyword; failure -> 6975]
  1: egg

[6974 op=2 display+exit]
  1: "Only a @dragon's will do."

[6975 op=1 keyword; failure -> 6977]
  1: drag

[6976 op=2 display+exit]
  1: "There's a lair in the dungeon Destard, to the northwest."
  2: "It's not far."

[6977 op=1 keyword; failure -> 6979]
  1: bye

[6978 op=2 display+exit]
  1: "Bye."

[6979 op=2 display+exit]
  1: "Sorry, I can't help you with that."

```

<a id="sasha-npc-124"></a>
## Sasha ? NPC 124, port 125

Instruction range: 11351?11428

```text
[11351 op=1 keyword; failure -> 11353]
  1: look

[11352 op=2 display+exit]
  1: a pretty young nurse with violet eyes.
  2: "Leave these men alone!"
  3: "Can't you see they need to @rest?"

[11353 op=1 keyword; failure -> 11355]
  1: name

[11354 op=2 display+exit]
  1: "My name is Sasha."

[11355 op=1 keyword; failure -> 11357]
  1: job

[11356 op=2 display+exit]
  1: "I @tend the sick. I know how to @heal, @cure, and @resurrect."

[11357 op=1 keyword; failure -> 11359]
  1: rest,need,alon,leav,slee

[11358 op=2 display+exit]
  1: "Aye, these men's wounds need @time to @heal."

[11359 op=1 keyword; failure -> 11361]
  1: woun,time,tend,sick

[11360 op=2 display+exit]
  1: "Thou hast kept me from my duties long enough. Away with thee!"

[11361 op=1 keyword; failure -> 11363]
  1: ener,reag

[11362 op=2 display+exit]
  1: "I have aided many today."
  2: "These men are the last; they are beyond the help of all save @time itself."

[11363 op=1 keyword; failure -> 11365]
  1: men,grie,toda,dark

[11364 op=2 display+exit]
  1: "These soldiers are the only survivors of an attempt to free the Shrine of @Compassion."

[11365 op=1 keyword; failure -> 11367]
  1: shri,comp

[11366 op=2 display+exit]
  1: "Show thy compassion now and let them @sleep."

[11374 op=2 display+exit]
  1: "Wilt thou make an offering of $V0 gold?"

[11378 op=1 keyword; failure -> 11381]
  1: heal

[11380 op=2 display+exit]
  1: "More wounded?"
  2: "I'll do what I can."
  3: The poor nurse seems weary beyond words.
  4: "Which of you?"

[11383 op=2 display+exit]
  1: "I cannot help thee with that."

[11385 op=1 keyword; failure -> 11387]
  1: yes

[11389 op=2 display+exit]
  1: "I'm sorry."
  2: "Without an offering I cannot heal thee."

[11390 op=1 keyword; failure -> 11393]
  1: cure

[11392 op=2 display+exit]
  1: "Poison?"
  2: "Yes, I can cure that."
  3: "Let me see..."
  4: "Wilt thou make an offering of 15 gold?"

[11394 op=1 keyword; failure -> 11397]
  1: yes

[11396 op=2 display+exit]
  1: "Which of you?"

[11399 op=2 display+exit]
  1: "I'm sorry."
  2: "Without an offering I cannot cure thee."

[11400 op=1 keyword; failure -> 11402]
  1: resu

[11401 op=2 display+exit]
  1: Your question seems almost like a blow to the woman.
  2: "Nay, I cannot."
  3: "I no longer have the energy, or the @reagents."
  4: "It has been a dark, dark day indeed."
  5: "I am sorry that I cannot help you, but we all @grieve today."

[11402 op=1 keyword; failure -> 11404]
  1: bye

[11403 op=2 display+exit]
  1: "Goodbye."

[11404 op=2 display+exit]
  1: "I cannot help thee with that."

[11408 op=2 display+exit]
  1: "I cannot help thee with that."

[11414 op=2 display+exit]
  1: "If thou wilt not pay, I cannot heal thee."

[11417 op=2 display+exit]
  1: "Don't waste my time!"
  2: "Thou art not wounded!"
  3: "'Tis hard enough tending to those truly sick."
  4: "Begone!"

[11419 op=2 display+exit]
  1: "I see thy injury, $PARTYMEMBER."
  2: Sasha approaches $PARTYMEMBER and binds the wounds.
  3: "Is there aught else I can do for thee?"

[11423 op=2 display+exit]
  1: "If thou wilt not pay, I cannot cure thee."

[11426 op=2 display+exit]
  1: "Don't waste my time! Thou art not poisoned!"
  2: "'Tis hard enough tending to those truly sick. Begone!"

[11428 op=2 display+exit]
  1: "I sense that thou art poisoned, $PARTYMEMBER."
  2: Sasha grabs a vial and pours it down $PARTYMEMBER's throat.
  3: $PARTYMEMBER feels much better.
  4: "Is there aught else I can do for thee?"

```

<a id="seggallion-npc-160"></a>
## Seggallion ? NPC 160, port 161

Instruction range: 14504?14560

```text
[14504 op=1 keyword; failure -> 14507]
  1: look

[14506 op=2 display+exit]
  1: a huge, muscled warrior with a sad look in his eyes.
  2: "You have a question?"

[14507 op=1 keyword; failure -> 14509]
  1: look

[14508 op=2 display+exit]
  1: a huge, muscled warrior with a sad look in his eyes.
  2: He looks at you imploringly.
  3: "What is it?"

[14509 op=1 keyword; failure -> 14512]
  1: name

[14511 op=2 display+exit]
  1: "Seggallion."
  2: "You can call me 'Segg' though."

[14512 op=1 keyword; failure -> 14514]
  1: name

[14513 op=2 display+exit]
  1: "Seggallion of...well now that won't mean anything to you."

[14514 op=1 keyword; failure -> 14517]
  1: job

[14516 op=2 display+exit]
  1: He looks at you puzzled.
  2: "The same as yours now."

[14517 op=1 keyword; failure -> 14519]
  1: job

[14518 op=2 display+exit]
  1: "If you mean my profession, I would have to say a knight of @Ashtalarea."

[14519 op=1 keyword; failure -> 14522]
  1: asht

[14521 op=2 display+exit]
  1: "Ask me again sometime."
  2: "It is quite a story."

[14522 op=1 keyword; failure -> 14524]
  1: asht

[14523 op=2 display+exit]
  1: "A land @unfamiliar to you, I'm sure."
  2: "Just as unfamiliar as yours is to me."

[14524 op=1 keyword; failure -> 14526]
  1: unfa,land

[14525 op=2 display+exit]
  1: "Yes, I was walking in the mountains of @Tyme when suddenly I heard a noise."
  2: "It was dark and I heard this low growling."
  3: "Having just been released from @Pildar's clutches, I was tired and hungry."

[14526 op=1 keyword; failure -> 14528]
  1: tyme

[14527 op=2 display+exit]
  1: "That is a great range of mountains in my land."
  2: "Wherever that is now."

[14528 op=1 keyword; failure -> 14530]
  1: pild

[14529 op=2 display+exit]
  1: "From what I hear, he is not unlike your Mondain."
  2: "Anyway, I peered into the darkness but could see nothing."
  3: "Suddenly a huge wall of @blue flame appeared behind me."

[14530 op=1 keyword; failure -> 14532]
  1: blue,flam

[14531 op=2 display+exit]
  1: "Yes, blue like nothing I had seen in any spell."
  2: "Anyway, just as I turned, the creature leapt from the woods."
  3: "It hit me with such force, we both @entered the blue flame together."

[14532 op=1 keyword; failure -> 14534]
  1: ente

[14533 op=2 display+exit]
  1: "Well, it didn't burn us, we just fell through to the ground beyond it."
  2: "After struggling for a while, I finally managed to kill it."
  3: "That's when I wandered into this @community."

[14534 op=1 keyword; failure -> 14536]
  1: comm

[14535 op=2 display+exit]
  1: "The people of this land are brave and honorable."
  2: "They fed and clothed me when I discovered that my gold was not the same as yours."
  3: "I know it is a @queer story, but true none the less."

[14536 op=1 keyword; failure -> 14538]
  1: quee

[14537 op=2 display+exit]
  1: "Yes it is."
  2: "Still, if I could I would like to @join you on your quest."
  3: "Perhaps one of the people in your land will know how to get me back to mine."
  4: "If not, at least I can thank your people by assisting you."

[14538 op=1 keyword; failure -> 14540]
  1: join

[14540 op=1 keyword; failure -> 14542]
  1: leav

[14542 op=1 keyword; failure -> 14544]
  1: bye

[14543 op=2 display+exit]
  1: "Farewell."

[14545 op=2 display+exit]
  1: He leans a bit closer.
  2: "What was that?"

[14546 op=2 display+exit]
  1: "That's one on me."
  2: "'Tain't never heard o' that."

[14549 op=2 display+exit]
  1: He leans a bit closer.
  2: "What was that?"

[14551 op=2 display+exit]
  1: "I wish I could, however, you seem to have enough strong arms already."
  2: "If any should leave though, seek me out."

[14553 op=2 display+exit]
  1: "I'm at someone's service already."
  2: "If I should leave though, seek me out."

[14554 op=2 display+exit]
  1: He bows low.
  2: "Seggallion at your service."
  3: You can see by his broad smile and large physique, he would be a worthy opponent.
  4: "Shall we be off then?"

[14557 op=2 display+exit]
  1: "That's one on me."
  2: "'Tain't never heard o' that."

[14559 op=2 display+exit]
  1: "Could we at least wait till we get back to Serpents Hold?"

[14560 op=2 display+exit]
  1: "I'll leave all these things with you."
  2: "It is hard to say goodbye, but I see your path leads elsewhere, so..."

```

<a id="selganor-npc-62"></a>
## Selganor ? NPC 62, port 63

Instruction range: 5825?5954

```text
[5826 op=1 keyword; failure -> 5832]
  1: 6789878767653

[5831 op=2 display+exit]
  1: "Excellent!" He strums a chord on his lute, then taps you lightly once on each shoulder with it.
  2: "You are now a full member of the guild of artisans."
  3: He reaches into a belt-pouch, takes out the Rune of Sacrifice, and hands it to you.
  4: "I'm sure you'll be needing this in your travels."
  5: "I know you'll find some solution to this gargoyle problem."

[5834 op=2 display+exit]
  1: "No, I'm afraid that's not it. Gwenno can help you learn the notes."

[5836 op=1 keyword; failure -> 5840]
  1: yes

[5839 op=2 display+exit]
  1: He inspects the pipes. "Very good, very good indeed."
  2: "Now tell me the sequence of notes in 'Stones' and I'll induct you into the guild."
  3: "Numeric notation will be fine."

[5841 op=1 keyword; failure -> 5844]
  1: yes

[5843 op=2 display+exit]
  1: "But I don't see any!"

[5846 op=2 display+exit]
  1: "Oh. Well, @Julia can show you how if you need help."

[5847 op=1 keyword; failure -> 5851]
  1: look

[5850 op=2 display+exit]
  1: a slender, graceful man tuning a lute.
  2: "Hello. There's something I need to ask you, what was it..."
  3: "Oh, yes, now I remember!"
  4: "Have you made a set of panpipes?"

[5851 op=1 keyword; failure -> 5854]
  1: look

[5853 op=2 display+exit]
  1: a slender, graceful man tuning a lute.
  2: "Hail and welcome, $P!"

[5857 op=1 keyword; failure -> 5860]
  1: silv

[5859 op=2 display+exit]
  1: "Thanks! Now what can I do for you?"

[5861 op=1 keyword; failure -> 5864]
  1: fung,cap,spor

[5863 op=2 display+exit]
  1: "Thanks! Now what can I do for you?"

[5865 op=1 keyword; failure -> 5868]
  1: volc,erup

[5867 op=2 display+exit]
  1: "Thanks! Now what can I do for you?"

[5869 op=1 keyword; failure -> 5872]
  1: kine,prop

[5871 op=2 display+exit]
  1: "Thanks! Now what can I do for you?"

[5874 op=2 display+exit]
  1: "No, that doesn't sound right. If only I had a Compendium to look it up in..."
  2: "Hmmmm... I wish I knew the answer to that question..."
  3: He turns away from you absent-mindedly.

[5876 op=1 keyword; failure -> 5880]
  1: look

[5879 op=2 display+exit]
  1: a slender, graceful man tuning a lute.
  2: "Oh, hello. Maybe you can help me with something."
  3: "My nephew is studying alchemy, and he needs to know this."
  4: "What kind of fork should mandrake roots be prepared with?"

[5880 op=1 keyword; failure -> 5884]
  1: look

[5883 op=2 display+exit]
  1: a slender, graceful man tuning a lute.
  2: "Oh, hello. Maybe you can help me with something."
  3: "My nephew is studying alchemy, and he needs to know this."
  4: "What part of the nightshade mushroom is used in spellcasting?"

[5884 op=1 keyword; failure -> 5888]
  1: look

[5887 op=2 display+exit]
  1: a slender, graceful man tuning a lute.
  2: "Oh, hello. Maybe you can help me with something."
  3: "My nephew is studying alchemy, and he needs to know this."
  4: "Where does sulfurous ash come from?"

[5888 op=1 keyword; failure -> 5892]
  1: look

[5891 op=2 display+exit]
  1: a slender, graceful man tuning a lute.
  2: "Oh, hello. Maybe you can help me with something."
  3: "My nephew is studying alchemy, and he needs to know this."
  4: "What are black pearls used for?"

[5894 op=2 display+exit]
  1: "Oh, hello. Maybe you can help me with something."
  2: "My nephew is studying alchemy, and he needs to know this."
  3: "What kind of fork should mandrake roots be prepared with?"

[5897 op=2 display+exit]
  1: "Oh, hello. Maybe you can help me with something."
  2: "My nephew is studying alchemy, and he needs to know this."
  3: "What part of the nightshade mushroom is used in spellcasting?"

[5900 op=2 display+exit]
  1: "Oh, hello. Maybe you can help me with something."
  2: "My nephew is studying alchemy, and he needs to know this."
  3: "Where does sulfurous ash come from?"

[5903 op=2 display+exit]
  1: "Oh, hello. Maybe you can help me with something."
  2: "My nephew is studying alchemy, and he needs to know this."
  3: "What are black pearls used for?"

[5904 op=1 keyword; failure -> 5906]
  1: name

[5905 op=2 display+exit]
  1: "My name is Selganor. 'Tis a name with a long @history..."
  2: "But you have the look of one with more important @business."

[5906 op=1 keyword; failure -> 5908]
  1: selg,hist

[5907 op=2 display+exit]
  1: "It means 'seeker of the @crystal'. Perhaps I'll tell you more some other time."

[5908 op=1 keyword; failure -> 5910]
  1: seek,crys

[5909 op=2 display+exit]
  1: "That's quite a @tale."

[5910 op=1 keyword; failure -> 5912]
  1: tale

[5911 op=2 display+exit]
  1: "Indeed."

[5912 op=1 keyword; failure -> 5914]
  1: impo,busi

[5913 op=2 display+exit]
  1: "Perhaps you're interested in joining the @guild."

[5914 op=1 keyword; failure -> 5916]
  1: job

[5915 op=2 display+exit]
  1: "I am the @guildmaster of artisans."

[5916 op=1 keyword; failure -> 5919]
  1: rune

[5918 op=2 display+exit]
  1: "I already loaned you the rune."

[5919 op=1 keyword; failure -> 5921]
  1: rune

[5920 op=2 display+exit]
  1: "I'd be glad to loan you the rune, but only members of the @guild are allowed to handle it."

[5921 op=1 keyword; failure -> 5923]
  1: juli

[5922 op=2 display+exit]
  1: "Yes, ask her about it."
  2: "She made my @lute - and most of our other instruments as well."
  3: "She lives just across the road, next door to Lady @Isabella."

[5923 op=1 keyword; failure -> 5925]
  1: isab

[5924 op=2 display+exit]
  1: "She's the mayor."

[5925 op=1 keyword; failure -> 5930]
  1: gwen

[5927 op=22 display+continue]
  1: "I know she's around here somewhere..."
  2: "Ah, I knew you'd be somewhere close by."

[5929 op=2 display+exit]
  1: "I'm right here, Selganor!"

[5930 op=1 keyword; failure -> 5932]
  1: gwen

[5931 op=2 display+exit]
  1: "I know she's around here somewhere..."

[5932 op=1 keyword; failure -> 5934]
  1: lute,inst

[5933 op=2 display+exit]
  1: "I prefer stringed instruments. They make such lovely sounds."

[5934 op=1 keyword; failure -> 5936]
  1: ball

[5935 op=2 display+exit]
  1: "Oh, are you looking for the man who invented the balloon?" He stares upward, stroking his chin pensively.
  2: "Ah yes, now I remember. He flew to @Sutek's castle."
  3: "Said something about a big job he had to do there."

[5936 op=1 keyword; failure -> 5938]
  1: sute,cast,flew

[5937 op=2 display+exit]
  1: "Sutek lives on an @island, far to the south."

[5938 op=1 keyword; failure -> 5940]
  1: isla

[5939 op=2 display+exit]
  1: "It's east of Serpent's Hold."

[5940 op=1 keyword; failure -> 5943]
  1: loan,memb,guil,arti,join

[5942 op=2 display+exit]
  1: "Wait a minute... You're already a member of the guild!"

[5943 op=1 keyword; failure -> 5946]
  1: loan,memb,guil,arti,join

[5945 op=2 display+exit]
  1: "If you want to join the guild, you need to do two things."
  2: "First, you need to make a set of @panpipes. @Julia can teach you how to that."
  3: "You also need to commit '@Stones' to memory. 'Tis a simple tune - ask @Gwenno to play it for thee."

[5946 op=1 keyword; failure -> 5949]
  1: pan,pipe,ston

[5948 op=2 display+exit]
  1: "You should play your panpipes often - 'twill lighten your step on the long road ahead."

[5949 op=1 keyword; failure -> 5952]
  1: pan,pipe,ston

[5951 op=2 display+exit]
  1: "Have you made a set of panpipes?"

[5952 op=1 keyword; failure -> 5954]
  1: bye

[5953 op=2 display+exit]
  1: "It was nice talking to you."

[5954 op=2 display+exit]
  1: "I don't think I can help you there."

```

<a id="sentri-npc-184"></a>
## Sentri ? NPC 184, port 185

Instruction range: 16153?16217

```text
[16154 op=1 keyword; failure -> 16157]
  1: yes

[16159 op=2 display+exit]
  1: "Then let's be on about our mission!"

[16161 op=1 keyword; failure -> 16164]
  1: yes

[16166 op=2 display+exit]
  1: "I bow to your wisdom."
  2: "If you have need of me later, you have but to ask."

[16167 op=1 keyword; failure -> 16171]
  1: look

[16170 op=2 display+exit]
  1: a handsome warrior.
  2: "$P!"
  3: "You have come at last!"

[16171 op=1 keyword; failure -> 16174]
  1: look

[16173 op=2 display+exit]
  1: a handsome warrior.
  2: "Good $T, $P."

[16174 op=1 keyword; failure -> 16176]
  1: look

[16175 op=2 display+exit]
  1: a handsome warrior.
  2: "Good $T, $P."
  3: "I hope you have returned to ask me to join you."

[16176 op=1 keyword; failure -> 16178]
  1: name

[16177 op=2 display+exit]
  1: "It is I, Sentri."
  2: (He pronounces his name 'Sahn-tray.')

[16178 op=1 keyword; failure -> 16180]
  1: job

[16179 op=2 display+exit]
  1: "I have been waiting here for you."
  2: "I heard you had @returned, and knew you would come here eventually."

[16180 op=1 keyword; failure -> 16182]
  1: wait,retu,hear,here

[16181 op=2 display+exit]
  1: "Britannia needs your help now more than ever before."
  2: "Many have set out to fight the @gargoyles already."

[16182 op=1 keyword; failure -> 16185]
  1: brit,need,help,figh,garg

[16184 op=2 display+exit]
  1: "Not just the town guards, but any able bodied men that could be spared from their work."
  2: "The battle goes poorly."
  3: "But now that you are here, there is hope."

[16185 op=1 keyword; failure -> 16187]
  1: brit,need,help,figh,garg

[16186 op=2 display+exit]
  1: "Not just the town guards, but any able bodied men that could be spared from their work."
  2: "The battle goes poorly."
  3: "But now that you are here, there is hope."
  4: "I stand ready to @join you."

[16187 op=1 keyword; failure -> 16190]
  1: join

[16189 op=2 display+exit]
  1: "I beg your pardon, could you repeat that?"

[16190 op=1 keyword; failure -> 16193]
  1: join

[16192 op=2 display+exit]
  1: "Shall I join your party?"

[16193 op=1 keyword; failure -> 16197]
  1: leave

[16196 op=2 display+exit]
  1: "Has the time come when your plans call for me to wait in reserve?"

[16197 op=1 keyword; failure -> 16201]
  1: bye

[16200 op=2 display+exit]
  1: "Has the time come when your plans call for me to wait in reserve?"

[16201 op=1 keyword; failure -> 16203]
  1: bye

[16202 op=2 display+exit]
  1: "If you should pass this way again, I will be here."
  2: "I await but the opportunity to @join you."

[16203 op=2 display+exit]
  1: "I beg your pardon, could you repeat that?"

[16206 op=2 display+exit]
  1: "I beg your pardon, could you repeat that?"

[16208 op=2 display+exit]
  1: "You have enough companions now."
  2: "If you tell one of them to @leave first, then I'll join you."

[16210 op=2 display+exit]
  1: "Wait until I finish my current mission!"

[16211 op=2 display+exit]
  1: "So be it."
  2: "Now that we are together, surely the realm will be saved."

[16214 op=2 display+exit]
  1: "I beg your pardon, could you repeat that?"

[16216 op=2 display+exit]
  1: "I can't go back to Serpent's Hold from here!"

[16217 op=2 display+exit]
  1: "Then I shall go wait at Serpent's Hold until you need me again."
  2: "I'd best leave you my equpiment in case you need it on your quest."

```

<a id="shamino-npc-1"></a>
## Shamino ? NPC 1, port 2

Instruction range: 43?93

```text
[43 op=1 keyword; failure -> 45]
  1: look

[44 op=2 display+exit]
  1: a quiet man, who almost seems to be a creature of the forest.

[45 op=1 keyword; failure -> 48]
  1: name

[47 op=2 display+exit]
  1: "Shamino Salle' Dacil."

[48 op=1 keyword; failure -> 50]
  1: job

[49 op=2 display+exit]
  1: "I help my @friends when they need it."
  2: "The rest of my time I prefer to spend exploring the @woods."

[50 op=1 keyword; failure -> 52]
  1: frie

[51 op=2 display+exit]
  1: "Though we have spent much time apart between your several visits to our realm, still I consider you one of my closest friends."

[52 op=1 keyword; failure -> 54]
  1: wood,expl

[53 op=2 display+exit]
  1: "I prefer the Deep @Forest."
  2: "Spiritwood isn't as dense, and it has those strange @wisps everywhere."

[54 op=1 keyword; failure -> 56]
  1: deep,fore

[55 op=2 display+exit]
  1: "It is @home to me."
  2: "I love walking in the dark shadows of the trees, and studying the nature of the forest @creatures."

[56 op=1 keyword; failure -> 58]
  1: home

[57 op=2 display+exit]
  1: "A home needs not @walls to make it so."

[58 op=1 keyword; failure -> 60]
  1: wall

[59 op=2 display+exit]
  1: "When folk ask why I have no walls in my home, I ask where are the @trees in theirs!" He smiles.

[60 op=1 keyword; failure -> 62]
  1: tree

[61 op=2 display+exit]
  1: "In many ways they are the lifesblood of the realm. Without trees, we could not long endure."

[62 op=1 keyword; failure -> 64]
  1: crea

[63 op=2 display+exit]
  1: "Those that dwell in the woods have lives of their own, more interesting to me than the ways of townfolk."
  2: "There is much to be learned from the animals, if you will but listen."

[64 op=1 keyword; failure -> 66]
  1: wisp

[65 op=2 display+exit]
  1: "I know little of their nature. I believe there are some @mages who have investigated them, however."

[66 op=1 keyword; failure -> 68]
  1: mage

[67 op=2 display+exit]
  1: "Perhaps one of the members of the council could tell you more."

[68 op=1 keyword; failure -> 70]
  1: garg

[69 op=2 display+exit]
  1: "I feel that Lord @British does not understand their true nature."

[70 op=1 keyword; failure -> 72]
  1: lord,brit

[71 op=2 display+exit]
  1: "I know him better than you might think."

[72 op=1 keyword; failure -> 74]
  1: join

[74 op=1 keyword; failure -> 76]
  1: leav

[76 op=1 keyword; failure -> 78]
  1: bye

[77 op=2 display+exit]
  1: "A pleasure."

[78 op=2 display+exit]
  1: "Ask Dupre about that."

[79 op=2 display+exit]
  1: "I feel you need my guidance through the wild parts of Britannia. I'd best stay with you."

[82 op=2 display+exit]
  1: "I'll follow you wherever you might choose to lead."

[84 op=2 display+exit]
  1: "Your party is full!"

[86 op=2 display+exit]
  1: "I'm busy now, ask me again later."

[87 op=2 display+exit]
  1: "I'll follow you wherever you might choose to lead."

[90 op=2 display+exit]
  1: "Ask Dupre about that."

[92 op=2 display+exit]
  1: "Don't leave me here!"

[93 op=2 display+exit]
  1: "I feel you need my guidance through the wild parts of Britannia."
  2: "Perhaps I could @join you again later."

```

<a id="shawn-npc-116"></a>
## Shawn ? NPC 116, port 117

Instruction range: 10873?10953

```text
[10878 op=1 keyword; failure -> 10880]
  1: yes

[10881 op=1 keyword; failure -> 10884]
  1: no

[10883 op=2 display+exit]
  1: "Very well..."

[10885 op=1 keyword; failure -> 10887]
  1: look

[10886 op=2 display+exit]
  1: a charismatic man with an engaging smile.
  2: "Welcome back, stranger."
  3: He winks at you.

[10887 op=1 keyword; failure -> 10890]
  1: look

[10889 op=2 display+exit]
  1: a charismatic man with an engaging smile.
  2: "Welcome."
  3: He makes a sweeping gesture with his hat.

[10890 op=1 keyword; failure -> 10892]
  1: name

[10891 op=2 display+exit]
  1: "Shawn, friend."

[10892 op=1 keyword; failure -> 10894]
  1: buy

[10893 op=2 display+exit]
  1: "Which: @ham, @ale, @mead, @wine, or @rations?"

[10894 op=1 keyword; failure -> 10896]
  1: job

[10895 op=2 display+exit]
  1: "I sell @ham, @ale, @mead, @wine, and @rations."

[10896 op=1 keyword; failure -> 10901]
  1: mead

[10900 op=2 display+exit]
  1: He smiles at you and says, "That will cost you 5 gold."
  2: "Interested?"

[10901 op=1 keyword; failure -> 10906]
  1: ale

[10905 op=2 display+exit]
  1: He smiles at you and says, "That will cost you 3 gold."
  2: "Interested?"

[10906 op=1 keyword; failure -> 10911]
  1: wine

[10910 op=2 display+exit]
  1: He smiles at you and says, "That will cost you 4 gold."
  2: "Interested?"

[10911 op=1 keyword; failure -> 10914]
  1: rati

[10913 op=2 display+exit]
  1: He looks at you and says, "It'll cost you 4 gold for each ration."
  2: "How many do you want?"

[10914 op=1 keyword; failure -> 10919]
  1: ham

[10918 op=2 display+exit]
  1: "It'll cost you 5 gold for the ham."
  2: "Interested?"

[10919 op=1 keyword; failure -> 10921]
  1: shaw

[10920 op=2 display+exit]
  1: "That's it."

[10921 op=1 keyword; failure -> 10923]
  1: bye

[10922 op=2 display+exit]
  1: "I'm glad you stopped by."
  2: He gives you a firm handshake.

[10923 op=2 display+exit]
  1: "I'm sorry."
  2: "Perhaps you could ask someone else."

[10926 op=2 display+exit]
  1: He frowns at you and says, "That was a good price."

[10928 op=2 display+exit]
  1: "You haven't any room in your pack, $Y."

[10933 op=2 display+exit]
  1: "Mutton rations cost more gold than you have, $Y."

[10935 op=2 display+exit]
  1: "You haven't any room in your pack, $Y."

[10937 op=2 display+exit]
  1: He hands you the mutton.

[10939 op=2 display+exit]
  1: He hands you $V0 mutton rations.

[10943 op=2 display+exit]
  1: "You don't have enough gold, $Y."

[10945 op=2 display+exit]
  1: "You can't carry it, $Y."

[10946 op=2 display+exit]
  1: "Fine!"
  2: After accepting the gold, he hands over the purchase.
  3: "There ya go."

[10950 op=2 display+exit]
  1: "How were you looking to pay for the meal?"
  2: "Eh?"

[10952 op=2 display+exit]
  1: "You can't carry it, $Y."

[10953 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he serves you the ham.

```

<a id="sherry-npc-7"></a>
## Sherry ? NPC 7, port 8

Instruction range: 598?689

```text
[602 op=22 display+continue]
  1: "Talking to a mouse?" says Dupre.
  2: "Methinks you're starting to imagine things..."

[603 op=1 keyword; failure -> 605]
  1: look

[604 op=2 display+exit]
  1: a cute little mouse.
  2: "Squeak, squeak, @squeak!"

[605 op=1 keyword; failure -> 607]
  1: sque

[606 op=2 display+exit]
  1: "Squeak @squirk?"

[607 op=1 keyword; failure -> 609]
  1: squi,nitt,fibl

[608 op=2 display+exit]
  1: "Squeeeeek!"

[609 op=1 keyword; failure -> 611]
  1: cat,meow,boo

[610 op=2 display+exit]
  1: "Eeeek!"

[611 op=1 keyword; failure -> 614]
  1: name

[613 op=2 display+exit]
  1: "I am Lord @British's friend, @Sherry."

[614 op=1 keyword; failure -> 616]
  1: lord,brit

[615 op=2 display+exit]
  1: "He reads me bedtime @stories every night."

[616 op=1 keyword; failure -> 618]
  1: bed,stor,nigh

[617 op=2 display+exit]
  1: "He knows the one about @Hubert the Lion by Heart!"

[618 op=1 keyword; failure -> 620]
  1: hube,lion

[619 op=2 display+exit]
  1: "That's my favorite story."

[620 op=1 keyword; failure -> 622]
  1: sher

[621 op=2 display+exit]
  1: "Pleased to meet you!"

[622 op=1 keyword; failure -> 624]
  1: kerc,bles,gesh,snee,dust

[623 op=2 display+exit]
  1: "I wish people would clean up under their beds and cabinets..."
  2: "Dust mice are no friends of mine!"

[624 op=1 keyword; failure -> 626]
  1: job

[625 op=2 display+exit]
  1: "I can slip through openings that are too small for you to fit through!"

[626 op=1 keyword; failure -> 629]
  1: chee

[628 op=2 display+exit]
  1: "Do you have any cheese?"

[629 op=1 keyword; failure -> 634]
  1: yes

[633 op=2 display+exit]
  1: "Can I have some?"

[634 op=1 keyword; failure -> 638]
  1: no

[637 op=2 display+exit]
  1: "Oh, that's too bad."

[638 op=1 keyword; failure -> 642]
  1: yes

[641 op=2 display+exit]
  1: "I don't smell any cheese!"

[642 op=1 keyword; failure -> 648]
  1: yes

[647 op=2 display+exit]
  1: "Thank you very much!"

[648 op=1 keyword; failure -> 652]
  1: yes

[651 op=2 display+exit]
  1: "I don't smell any cheese!"

[652 op=1 keyword; failure -> 656]
  1: no

[655 op=2 display+exit]
  1: "Well, you're no fun."

[656 op=1 keyword; failure -> 660]
  1: yes

[660 op=1 keyword; failure -> 664]
  1: no

[663 op=2 display+exit]
  1: "Ok."

[665 op=1 keyword; failure -> 669]
  1: help,rune,mous,hole,join

[668 op=2 display+exit]
  1: "I can help you! Do you want me to come with you?"

[669 op=1 keyword; failure -> 671]
  1: help,rune,mous,hole,join

[670 op=2 display+exit]
  1: "Give me some @cheese and ask me again."

[671 op=1 keyword; failure -> 673]
  1: leav

[673 op=1 keyword; failure -> 675]
  1: bye

[674 op=2 display+exit]
  1: "Nitters @fiblik."
  2: "Come visit me again soon!"

[675 op=2 display+exit]
  1: "Kerchew!"
  2: The mouse wiggles its whiskers at you.

[678 op=2 display+exit]
  1: "Travelling with you will be fun!"
  2: "I hope we find some more cheese soon."

[680 op=2 display+exit]
  1: "With that many people I'd get trampled!"

[682 op=2 display+exit]
  1: "But I'm busy now, ask me again later."

[683 op=2 display+exit]
  1: "Great!"
  2: "Travelling with you will be fun!"
  3: "I hope we find some more cheese soon."

[686 op=2 display+exit]
  1: "Kerchew!"
  2: The mouse wiggles its whiskers at you.

[688 op=2 display+exit]
  1: "I'm not leaving until we get out of this place!"

[689 op=2 display+exit]
  1: "All right."
  2: "Now I can hear my bedtime stories again!"
  3: "I'll leave my stuff with you in case you need it."

```

<a id="shubin-npc-151"></a>
## Shubin ? NPC 151, port 152

Instruction range: 14004?14088

```text
[14005 op=1 keyword; failure -> 14010]
  1: yes

[14009 op=2 display+exit]
  1: "Have 5 whole trays, then."

[14012 op=2 display+exit]
  1: "Such a picky eater."
  2: "No wonder you're so skinny, $G."

[14014 op=1 keyword; failure -> 14020]
  1: yes

[14019 op=2 display+exit]
  1: "Oh, thank you, thank you, thank you!"

[14021 op=1 keyword; failure -> 14029]
  1: yes

[14028 op=2 display+exit]
  1: "Oh, thank you, thank you, thank you!"

[14031 op=2 display+exit]
  1: "Oh, do not tease old Shubin so, $G!"

[14033 op=1 keyword; failure -> 14037]
  1: yes

[14036 op=2 display+exit]
  1: He sighs.
  2: "Well, $G, it did no harm to ask, I suppose."

[14040 op=2 display+exit]
  1: "I just know you will remember Shubin, $G."
  2: "Thank you."

[14042 op=1 keyword; failure -> 14045]
  1: yes

[14044 op=2 display+exit]
  1: "Well, I'd like to make it, $G, but I cannot get one of the ingredients..."
  2: "I sure would appreciate it if you brought it back for me
  3: if you came across one in your travels, $G..."
  4: "If you could find time for old Shubin, that is..."
  5: "Would it be too much trouble to bring me back a dragon's @egg, $G?"

[14047 op=2 display+exit]
  1: "It is a marvellous dish, $G. My friend Sandy gave me the recipe..."
  2: "I'm only missing one ingredient..."
  3: "I sure would appreciate it if you brought it back for me
  4: if you came across one in your travels, $G..."
  5: "If you could find time for old Shubin, that is..."
  6: "Would it be too much trouble to bring me back a dragon's @egg, $G?"

[14048 op=1 keyword; failure -> 14051]
  1: look

[14050 op=2 display+exit]
  1: a man with a wide bottom.
  2: His voice is almost like a woman's.
  3: "Why, hello there, $G."

[14051 op=1 keyword; failure -> 14055]
  1: look

[14054 op=2 display+exit]
  1: a man with a wide bottom.
  2: His voice is almost like a woman's.
  3: "Ooh, you look so thin!"
  4: "Here, have a bite of bread."
  5: He gives you a @roll; it is still warm from the oven.

[14055 op=1 keyword; failure -> 14057]
  1: name

[14056 op=2 display+exit]
  1: "My name is Shubin, $G."

[14057 op=1 keyword; failure -> 14059]
  1: job

[14058 op=2 display+exit]
  1: "I'm a @cook, $G."

[14059 op=1 keyword; failure -> 14061]
  1: sand

[14060 op=2 display+exit]
  1: "He is my friend."

[14061 op=1 keyword; failure -> 14066]
  1: mant

[14063 op=22 display+continue]
  1: "@Sandy says the mantra of eating is 'yum,' $G."
  2: "Hee, hee!"

[14065 op=2 display+exit]
  1: You friend Iolo rolls his eyes.
  2: "Such wisdom we can surely do without!"

[14066 op=1 keyword; failure -> 14068]
  1: mant

[14067 op=2 display+exit]
  1: "@Sandy says the mantra of eating is 'yum,' $G."
  2: "Hee, hee!"

[14068 op=1 keyword; failure -> 14071]
  1: thin,roll

[14070 op=2 display+exit]
  1: "I have no more rolls, $G."

[14071 op=1 keyword; failure -> 14074]
  1: thin,roll

[14073 op=2 display+exit]
  1: He ask anxiously "Do you like it, $G?"

[14074 op=1 keyword; failure -> 14076]
  1: cook

[14075 op=2 display+exit]
  1: "Being a cook is like being an @adventurer, $G."

[14076 op=1 keyword; failure -> 14078]
  1: adve

[14077 op=2 display+exit]
  1: "Yes, $G."
  2: "I must gather up all my ingredients with @care."

[14078 op=1 keyword; failure -> 14080]
  1: care

[14079 op=2 display+exit]
  1: "So that I have the right @ingredients, $G."

[14080 op=1 keyword; failure -> 14083]
  1: ingr

[14082 op=2 display+exit]
  1: "Adventurers are always searching for this or that, $G."
  2: "So are cooks."
  3: "Do you know of Magincian Pastry?"

[14083 op=1 keyword; failure -> 14086]
  1: drag,egg

[14085 op=2 display+exit]
  1: "Have you one with you?"

[14086 op=1 keyword; failure -> 14088]
  1: bye

[14087 op=2 display+exit]
  1: "Goodbye, $G."

[14088 op=2 display+exit]
  1: "That's one of the eight @mantras, isn't it?"

```

<a id="simon-npc-158"></a>
## Simon ? NPC 158, port 159

Instruction range: 14464?14484

```text
[14464 op=1 keyword; failure -> 14467]
  1: look

[14466 op=2 display+exit]
  1: a muscular knight.
  2: He speaks in a raspy voice.
  3: "Good $T, $G."

[14467 op=1 keyword; failure -> 14470]
  1: look

[14469 op=2 display+exit]
  1: a muscular knight.
  2: He speaks in a raspy voice.
  3: "Greetings, $G Avatar!"
  4: "Thou art most welcome in my castle, Avatar."
  5: He bows deeply to you.

[14470 op=1 keyword; failure -> 14472]
  1: name

[14471 op=2 display+exit]
  1: "Call me Simon, $G."

[14472 op=1 keyword; failure -> 14474]
  1: job

[14473 op=2 display+exit]
  1: "I am the lord of Serpent's Hold."

[14474 op=1 keyword; failure -> 14476]
  1: mant

[14475 op=2 display+exit]
  1: "I personally favor the mantra of Valour."
  2: "I taught it to the bard @Culham in a song."

[14476 op=1 keyword; failure -> 14478]
  1: culh

[14477 op=2 display+exit]
  1: "He spends much time in Jhelom, though he be a @traveling bard."

[14478 op=1 keyword; failure -> 14480]
  1: trav

[14479 op=2 display+exit]
  1: "Yes, there's no telling where one might find him."
  2: "As I said, though, Jhelom is a good guess."

[14480 op=1 keyword; failure -> 14482]
  1: rune

[14481 op=2 display+exit]
  1: "If I had a rune, I'd wear it proudly."
  2: "But others fear for the safety of such important talismans, hiding them in some peculiar places."
  3: "I wonder if 'tis considered theft to seize a rune laid out for all to see..."

[14482 op=1 keyword; failure -> 14484]
  1: bye

[14483 op=2 display+exit]
  1: "Goodbye."
  2: "I hope thou wilt visit me again when thou hast time, friend."

[14484 op=2 display+exit]
  1: "I wish I could help thee with that, Avatar."

```

<a id="sin-vraal-npc-139"></a>
## Sin'Vraal ? NPC 139, port 140

Instruction range: 12914?12944

```text
[12914 op=1 keyword; failure -> 12916]
  1: look

[12915 op=2 display+exit]
  1: the daemon Sin'Vraal.
  2: "Good $T, $G."
  3: "What seek you?"

[12916 op=1 keyword; failure -> 12918]
  1: name

[12917 op=2 display+exit]
  1: "I am Sin'Vraal."

[12918 op=1 keyword; failure -> 12920]
  1: job

[12919 op=2 display+exit]
  1: "No job, $G."
  2: "I am @free to wander where I wish."

[12920 op=1 keyword; failure -> 12922]
  1: free,wand

[12921 op=2 display+exit]
  1: "Among my @people, those who can fly rule."
  2: "Though I have wings, I cannot @fly."
  3: "So I was a slave."
  4: "Here I am free!"

[12922 op=1 keyword; failure -> 12924]
  1: peop,demo,daem,your

[12923 op=2 display+exit]
  1: "I am not a daemon, though humans think so."
  2: "I am a @gargoyle."

[12924 op=1 keyword; failure -> 12926]
  1: garg,ran,away

[12925 op=2 display+exit]
  1: "I fled here to the Underworld because I was not free among the gargoyles."

[12926 op=1 keyword; failure -> 12928]
  1: book,prop,mari

[12927 op=2 display+exit]
  1: "The Book of Prophecies says that the False Prophet will come to destroy the gargoyle race."
  2: "Only the @sacrifice of the False Prophet will save us."

[12928 op=1 keyword; failure -> 12930]
  1: read

[12929 op=2 display+exit]
  1: "Because I could not @fly, I was never taught to read."

[12930 op=1 keyword; failure -> 12932]
  1: sacr

[12931 op=2 display+exit]
  1: "Sacrifice means many things in my language."
  2: "You should seek out a gargoyle scholar to learn more."
  3: "Travel the other side of the world, and you should be able to find one."
  4: "You can go down through Hythloth to get to the realm of gargoyles."

[12932 op=1 keyword; failure -> 12934]
  1: fly

[12933 op=2 display+exit]
  1: "There are two kinds of gargoyles: winged ones and wingless ones."
  2: "Wingless ones are mute and unintelligent."
  3: "They do all the manual labor."
  4: "Winged ones are smart, and do all the planning and leading."
  5: "Being able to fly is, to them, a sign of @intelligence."
  6: "Even the @Temple can only be reached by air."

[12934 op=1 keyword; failure -> 12936]
  1: inte

[12935 op=2 display+exit]
  1: "Yes, a strange concept this."
  2: "Yet as some in Minoc can tell you, flying can be achieved by anyone."

[12936 op=1 keyword; failure -> 12938]
  1: temp

[12937 op=2 display+exit]
  1: "The great Temple of Singularity held the Codex."
  2: "The Temple is the most holy gargoyle place."
  3: "A human could learn much by traveling there."
  4: "But you could never get to the temple."
  5: "Not only is it deep below the surface, but it can only be reached by flying."
  6: "And you cannot fly any more than I can."

[12938 op=1 keyword; failure -> 12940]
  1: pira

[12939 op=2 display+exit]
  1: "Yes, I once met a pirate here in the desert."
  2: "He seemed to be hunting me, but he got dragged off by the @ants!"

[12940 op=1 keyword; failure -> 12942]
  1: ants

[12941 op=2 display+exit]
  1: "The great desert ants build huge mounds, and are very dangerous!"

[12942 op=1 keyword; failure -> 12944]
  1: bye

[12943 op=2 display+exit]
  1: "May your persistence and precision lead to success, $G!"

[12944 op=2 display+exit]
  1: "I know nothing of $Z, $G."

```

<a id="singularity-npc-198"></a>
## Singularity ? NPC 198, port 199

Instruction range: 16815?16897

```text
[16817 op=1 keyword; failure -> 16819]
  1: look

[16818 op=2 display+exit]
  1: a finely-carved stone pyramid.
  2: "There is no more wisdom for thee to acquire here, noble Avatar."
  3: "Seek the Codex, and may nothing impede thy sacred quest!"

[16819 op=2 display+exit]
  1: "There is no more wisdom for thee to acquire here, noble Avatar."
  2: "Seek the Codex, and may nothing impede thy sacred quest!"

[16821 op=1 keyword; failure -> 16824]
  1: un-or-us,un-us-or,us-un-or,us-or-un,or-us-un,or-un-us,unorus,unusor,usunor,usorun,orusun,orunus

[16823 op=2 display+exit]
  1: "Let the principle of control guide thy quest."
  2: "Let the principle of passion drive thy quest."
  3: "Let the principle of diligence maintain thy quest."
  4: "Then wilt thou truly achieve the unified spirit and singularity of purpose"
  5: "that is necessary for success in all endeavors."
  6: "This is the gargoyle way; let it be thy way as well."
  7: The altar glows with a cold blue flame.
  8: You feel compelled to seek out the Codex.
  9: Your final quest has begun!

[16826 op=2 display+exit]
  1: "You have not yet become familiar with the wisdom of the gargoyle race."
  2: "Continue thy search."

[16828 op=1 keyword; failure -> 16834]
  1: yes

[16833 op=2 display+exit]
  1: "What is the Mantra of Singularity?"

[16835 op=1 keyword; failure -> 16838]
  1: yes

[16837 op=2 display+exit]
  1: "How canst thou truly work for the good of a race whose ways thou dost not understand?"
  2: "Go thou into the @Catacombs of @Control, @Passion and @Diligence."
  3: "In each wilt thou find the final resting place of the being who most exemplifies that @principle."
  4: "Meditate upon these @principles."
  5: "Return when thou hast been to all three @catacombs."

[16840 op=2 display+exit]
  1: "Which catacomb dost thou seek, @Control, @Passion or @Diligence?"

[16842 op=1 keyword; failure -> 16846]
  1: both,our,peac,all,ever

[16845 op=2 display+exit]
  1: "Thy answer is wise."
  2: "But how canst thou truly work for the good of a race whose ways thou dost not understand?"
  3: "You have not yet become familiar with the ways of the gargoyle race."
  4: "Continue thy search."

[16847 op=1 keyword; failure -> 16850]
  1: both,our,peac,all,ever

[16849 op=2 display+exit]
  1: "Thy answer is wise."
  2: "But how canst thou truly work for the good of a race whose ways thou dost not understand?"
  3: "Go thou into the @Catacombs of @Control, @Passion and @Diligence."
  4: "In each wilt thou find the final resting place of the being who most exemplifies that @principle."
  5: "Return when thou hast found these @catacombs."

[16851 op=1 keyword; failure -> 16853]
  1: huma,me,my,lord,brit

[16852 op=2 display+exit]
  1: "Thy answer is foolish."
  2: "A journey made for selfish reasons can never be a sacred quest, regardless of its goal."
  3: "Think again, Avatar:"
  4: "For whom dost thou seek the Codex?"

[16854 op=1 keyword; failure -> 16856]
  1: garg,seer,scho,inqu,lead

[16855 op=2 display+exit]
  1: "Thy answer is foolish."
  2: "There is no nobility or honor in betraying one's own race for the sake of another."
  3: "Such a path can never be a sacred quest."
  4: "Think again, Avatar:"
  5: "For whom dost thou seek the Codex?"

[16857 op=1 keyword; failure -> 16859]
  1: valk,beh,naxa,drax,bole

[16858 op=2 display+exit]
  1: "Thy answer is foolish."
  2: "Thy compassion is acknowledged, but compassion is not the key to success."
  3: "The motive must be as one with the goal for a journey to become a sacred quest."
  4: "Think again, Avatar:"
  5: "For whom dost thou seek the Codex?"

[16861 op=2 display+exit]
  1: "Thy answer is foolish."
  2: "Thy mind must be focused clearly on thy path for thy journey to become a sacred quest."
  3: "Return when thou canst truly answer this question."

[16862 op=1 keyword; failure -> 16865]
  1: look

[16864 op=2 display+exit]
  1: a finely-carved stone pyramid.
  2: As you face the altar, a voice rings in your head.
  3: You understand it perfectly, but you can't recognize the language!
  4: "Thy deeds make thee unworthy of enlightenment."
  5: "Return when thy path is more focused."

[16866 op=2 display+exit]
  1: As you face the altar, a voice rings in your head.
  2: You understand it perfectly, but you can't recognize the language!
  3: "Thy deeds make thee unworthy of enlightenment."
  4: "Return when thy path is more focused."

[16867 op=1 keyword; failure -> 16870]
  1: look

[16869 op=2 display+exit]
  1: a finely-carved stone pyramid.
  2: As you face the altar, a voice rings in your head.
  3: You understand it perfectly, but you can't recognize the language!
  4: "Thou hast far to travel and much to achieve."
  5: "May the wisdom imparted here help purify thy inner flame."
  6: "What do you seek, Avatar?"

[16870 op=1 keyword; failure -> 16872]
  1: look

[16871 op=2 display+exit]
  1: a finely-carved stone pyramid.
  2: As you face the altar, a voice rings in your head.
  3: You understand it perfectly, but you can't recognize the language!
  4: "Thou hast far to travel and much to achieve, but thy inner fire will surely guide thy way."
  5: "What do you seek, Avatar?"

[16872 op=1 keyword; failure -> 16874]
  1: name,job

[16873 op=2 display+exit]
  1: "Thou hast reached the Altar of Singularity."
  2: "What dost thou seek?"

[16876 op=1 keyword; failure -> 16879]
  1: sacr,ques,code,mant

[16878 op=2 display+exit]
  1: "Hast thou completed thy journey to the @catacombs?"

[16879 op=1 keyword; failure -> 16881]
  1: cata

[16880 op=2 display+exit]
  1: "Which catacomb dost thou seek, @Control, @Passion or @Diligence?"

[16881 op=1 keyword; failure -> 16883]
  1: prin,thre

[16882 op=2 display+exit]
  1: "The three principles of @Control, @Passion and @Diligence form the basis of the gargoyle way of life."

[16883 op=1 keyword; failure -> 16885]
  1: cont

[16884 op=2 display+exit]
  1: "Control is one of the guiding principles of the gargoyle race."
  2: "Go thou into the @Catacombs of Control, near the coast to the west of here."
  3: "There you will find the being who most exemplifies this @principle."

[16885 op=1 keyword; failure -> 16887]
  1: dili

[16886 op=2 display+exit]
  1: "Diligence is one of the guiding principles of the gargoyle race."
  2: "Go thou into the @Catacombs of Diligence, near the coast to the south of here."
  3: "There you will find the being who most exemplifies this @principle."

[16887 op=1 keyword; failure -> 16889]
  1: pass

[16888 op=2 display+exit]
  1: "Passion is one of the guiding principles of the gargoyle race."
  2: "Go thou into the @Catacombs of Passion, near the coast to the east of here."
  3: "There you will find the being who most exemplifies this @principle."

[16889 op=1 keyword; failure -> 16892]
  1: sacr,ques,code

[16891 op=2 display+exit]
  1: "Look deep inside thyself, Avatar, and answer this:"
  2: "For whom dost thou truly seek the Codex?"

[16892 op=1 keyword; failure -> 16894]
  1: bye

[16893 op=2 display+exit]
  1: The mystic voice falls silent, and you leave the altar.

[16896 op=2 display+exit]
  1: "The answer to that lies elsewhere."

[16897 op=2 display+exit]
  1: "The answer to that lies within thyself."

```

<a id="sinjen-npc-179"></a>
## Sinjen ? NPC 179, port 180

Instruction range: 15861?15883

```text
[15861 op=1 keyword; failure -> 15863]
  1: look

[15862 op=2 display+exit]
  1: a long haired, raggedy man in stocks.

[15863 op=1 keyword; failure -> 15865]
  1: name

[15864 op=2 display+exit]
  1: "I'm Sinjen the @warrior."

[15865 op=1 keyword; failure -> 15867]
  1: warr

[15866 op=2 display+exit]
  1: "Well, these days I'm more into @stocks."

[15867 op=1 keyword; failure -> 15869]
  1: job

[15868 op=2 display+exit]
  1: "@Lenora said I should @stay in these stocks for a while and make sure they work properly."

[15869 op=1 keyword; failure -> 15871]
  1: stoc,make,sure,work,prop,stay

[15870 op=2 display+exit]
  1: "I didn't really do anything wrong."
  2: "I told them to go ahead and put me here anyway."
  3: "So I could get a different @perspective on @life."

[15871 op=1 keyword; failure -> 15873]
  1: pers

[15872 op=2 display+exit]
  1: "Normally we stumble around through life on our own."
  2: "But here I can stand still and watch it go by all around me."

[15873 op=1 keyword; failure -> 15875]
  1: life

[15874 op=2 display+exit]
  1: "The meaning of life?"
  2: "I'm still trying to figure it out myself."

[15875 op=1 keyword; failure -> 15877]
  1: leno

[15876 op=2 display+exit]
  1: "Justice may be a @virtue, but I think she carries it too far."

[15877 op=1 keyword; failure -> 15879]
  1: just,virt

[15878 op=2 display+exit]
  1: "I prefer @Mandrake's version of the eight virtues."

[15879 op=1 keyword; failure -> 15881]
  1: mand,vers,eigh

[15880 op=2 display+exit]
  1: "You should ask him about it sometime."

[15881 op=1 keyword; failure -> 15883]
  1: bye

[15882 op=2 display+exit]
  1: "Come back and chat again sometime."
  2: "I'll be right here..."

[15883 op=2 display+exit]
  1: "Maybe I can talk to you about that after I get out of these stocks."

```

<a id="sionnach-npc-177"></a>
## Sionnach ? NPC 177, port 178

Instruction range: 15675?15718

```text
[15676 op=1 keyword; failure -> 15679]
  1: yes

[15678 op=2 display+exit]
  1: The drum is wide and flat.
  2: Sionnach holds it by the strings on the back of it.
  3: Then, he taps it in intricate patterns using both ends of the dowel held loosely in his other hand.
  4: It sounds thus: 'RUM-TE-TUM, RUM-TE-TUM, TUMBLETY-TUM-TE-TUM!'
  5: The beat is invigorating and exciting.
  6: "That's a @battle summons."

[15681 op=2 display+exit]
  1: "As you wish, $G."

[15684 op=1 keyword; failure -> 15687]
  1: look

[15686 op=2 display+exit]
  1: a troubadour with a drum.
  2: His tailored suit is travel-worn but bright.
  3: "Excuse me, ma'am."
  4: "We are, um, indisposed."
  5: He returns his attentions to Sylaina.

[15687 op=1 keyword; failure -> 15689]
  1: look

[15688 op=2 display+exit]
  1: a troubadour with a drum.
  2: His tailored suit is travel-worn but bright.
  3: "Pardon me, sir, but I hate to keep my dear one waiting."
  4: "You understand, of course."
  5: He returns his attentions to Sylaina.

[15690 op=2 display+exit]
  1: "Excuse me, ma'am."
  2: "We are, um, indisposed."
  3: He returns his attentions to Sylaina.

[15691 op=2 display+exit]
  1: "Pardon me, sir, but I hate to keep my dear one waiting."
  2: "You understand, of course."
  3: He returns his attentions to Sylaina.

[15692 op=1 keyword; failure -> 15697]
  1: look

[15694 op=22 display+continue]
  1: a troubadour with a drum.
  2: His tailored suit is travel-worn but bright.
  3: "Greetings, folks."
  4: "Why, hello there, my friend."
  5: "You've grown a bit since we met last."
  6: "It has been a while, Master Iolo."

[15696 op=2 display+exit]
  1: "I'm afraid I can't recall your name; you'll have to forgive me."

[15697 op=1 keyword; failure -> 15699]
  1: look

[15698 op=2 display+exit]
  1: a troubadour with a drum.
  2: His tailored suit is travel-worn but bright.
  3: "Greetings, $G."

[15699 op=1 keyword; failure -> 15701]
  1: name

[15700 op=2 display+exit]
  1: "My name is Sionnach."
  2: (He pronounces it 'ShaNOK'.)

[15701 op=1 keyword; failure -> 15703]
  1: job

[15702 op=2 display+exit]
  1: "I'm a @drummer and a @singer."

[15703 op=1 keyword; failure -> 15706]
  1: drum

[15705 op=2 display+exit]
  1: "Would you like me to play for you?"

[15706 op=1 keyword; failure -> 15708]
  1: batt,summ

[15707 op=2 display+exit]
  1: "'Twas long ago... Back when men forced sorcerous secrets from demons..."
  2: "Back when women wove moonbeams into cloth of indescribably fine texture..."
  3: "Long ago, Lord Fennian devised a language of raps and knocks"
  4: "on the great drums used by his armies."
  5: "Each drum was made of the entire skin of a bull, stretched taut by a dozen cords."
  6: "It's true."
  7: "I read that in a @book I found at the Lycaeum."

[15708 op=1 keyword; failure -> 15710]
  1: book,lyca,read,foun

[15709 op=2 display+exit]
  1: "I also read a number of maritime histories."
  2: "They record the facts behind the many songs of different @ships."

[15710 op=1 keyword; failure -> 15712]
  1: song,ship

[15711 op=2 display+exit]
  1: "The sea is not always a gentle, generous friend."
  2: "Storms have brought down many a haughty vessel:"
  3: "The evil @Empire, the @Dutchman, the Virtuous..."
  4: "Have you heard of the @Virtuous?"

[15712 op=1 keyword; failure -> 15714]
  1: dutc,empi

[15713 op=2 display+exit]
  1: "I don't know where she rests."
  2: "Ask other bards."
  3: "There's probably great treasure on her, and on the @Virtuous, too."

[15714 op=1 keyword; failure -> 15716]
  1: sing,virt,no

[15715 op=2 display+exit]
  1: Sionnach sings softly for you.
  2: "A southerner from Serpent's Hold
  3: Once slew a pirate crew of eight.
  4: Young Keegan was this warrior bold,
  5: And thus he claimed the 'Bitter Kate.'"
  6: "He broke a bottle 'cross her bow
  7: And 'Virtuous' became her name.
  8: For many years she served him well;
  9: Good Captain Keegan rose to fame."
  10: "The Barston Bay came north to fight,
  11: The Jolly Roger on her mast.
  12: To cold Loch Lake she sailed that night.
  13: To sink Keegan's ship with cannon blast."
  14: "Ah, the old Bitter Kate, she's mended her ways,
  15: And as the Virtuous, ended her days."
  16: "Captain Keegan, he sleeps the endless sleep
  17: While on Loch Lake's shore the Virtuous lays."

[15716 op=1 keyword; failure -> 15718]
  1: bye

[15717 op=2 display+exit]
  1: "Take care!"

[15718 op=2 display+exit]
  1: "That I cannot help you with."

```

<a id="smith-npc-130"></a>
## Smith ? NPC 130, port 131

Instruction range: 11884?11939

```text
[11891 op=2 display+exit]
  1: "Oh, I had something important to tell you!"
  2: "I can't remember what it was, though..."

[11892 op=1 keyword; failure -> 11895]
  1: look

[11894 op=2 display+exit]
  1: a fine looking horse.
  2: "Hello again!"

[11895 op=1 keyword; failure -> 11899]
  1: look

[11898 op=2 display+exit]
  1: a fine looking horse.
  2: "Hello, my name is Smith."
  3: "Who are you?"

[11899 op=1 keyword; failure -> 11904]
  1: clue

[11901 op=22 display+continue]
  1: "Oh, that's right, I have a clue for you!"
  2: "You're the Warrior of Destiny, right?"
  3: "Well, when you go to rescue Lord British from the underworld,
  4: be sure to bring his sandlewood @box with you!"
  5: "Well, even if you didn't like my clue,
  6: it wouldn't kill you to say 'thank you.'"

[11903 op=2 display+exit]
  1: "We went to all that trouble just to hear this?" says Dupre.
  2: "We rescued Lord British from the underworld years ago!"

[11904 op=1 keyword; failure -> 11906]
  1: clue

[11905 op=2 display+exit]
  1: "Oh, that's right, I have a clue for you!"
  2: "You're the Warrior of Destiny, right?"
  3: "Well, when you go to rescue Lord British from the underworld,
  4: be sure to bring his sandlewood @box with you!"

[11906 op=1 keyword; failure -> 11908]
  1: than

[11907 op=2 display+exit]
  1: "You're welcome."

[11908 op=1 keyword; failure -> 11910]
  1: warr,dest,lord,brit,unde,sand,box,resc

[11909 op=2 display+exit]
  1: "I don't know what it all means; I just know it's important."

[11910 op=1 keyword; failure -> 11912]
  1: name

[11911 op=2 display+exit]
  1: "It's 'Smith', like in '@blacksmith'.

[11912 op=1 keyword; failure -> 11914]
  1: blac

[11913 op=2 display+exit]
  1: "They make my @shoes."

[11914 op=1 keyword; failure -> 11916]
  1: shoe

[11915 op=2 display+exit]
  1: "They're nailed to my @feet, see?"
  2: He holds up a hoof for you to look at.

[11916 op=1 keyword; failure -> 11918]
  1: nail,feet

[11917 op=2 display+exit]
  1: "You should try it."
  2: "Saves you from looking for your shoes every morning."

[11918 op=1 keyword; failure -> 11923]
  1: job

[11920 op=22 display+continue]
  1: "I eat Iolo's @hay to keep it from piling up everywhere and making a mess."

[11922 op=2 display+exit]
  1: Iolo winks at you.
  2: "And a fine job you do of it, too."
  3: "I don't know what I'd do without you."

[11923 op=1 keyword; failure -> 11925]
  1: job

[11924 op=2 display+exit]
  1: "I eat Iolo's @hay to keep it from piling up everywhere and making a mess."

[11925 op=1 keyword; failure -> 11927]
  1: eat,hay,pile,mess

[11926 op=2 display+exit]
  1: "Hay is the greatest menace to the well being of Britannia."
  2: "It takes constant vigilance to keep it from engulfing the entire realm."
  3: "I've devoted my life to the @task."

[11927 op=1 keyword; failure -> 11929]
  1: mena,task

[11928 op=2 display+exit]
  1: "It's a thankless job, but I am a determined horse."
  2: "Hay, thou hath met thy nemesis, thy doom, and his name is Smith!"

[11929 op=1 keyword; failure -> 11937]
  1: bye

[11932 op=22 display+continue]
  1: "Farewell, two-legger."

[11934 op=22 display+continue]
  1: You see Iolo give Dupre a nudge in the ribs.
  2: "You owe me a drink," he says, grinning.
  3: He turns to you.
  4: "Sir Know-it-all here bet me that there was no such thing as a talking horse!"

[11936 op=2 display+exit]
  1: "Now I've seen everything."
  2: "I suppose we'll be inviting some @ducks over for tea next."

[11937 op=1 keyword; failure -> 11939]
  1: bye

[11938 op=2 display+exit]
  1: "Farewell, two-legger."

[11939 op=2 display+exit]
  1: "I don't have the answer to everything."
  2: "I'm just a horse, you know."

```

<a id="snake-charmer-npc-169"></a>
## Snake_Charmer ? NPC 169, port 170

Instruction range: 15166?15194

```text
[15170 op=1 keyword; failure -> 15172]
  1: look

[15171 op=2 display+exit]
  1: an old winged gargoyle.
  2: The gargoyle shouts, "An-bal-sil-fer!" and flees!

[15172 op=2 display+exit]
  1: The gargoyle shouts, "An-bal-sil-fer!" and flees!

[15173 op=1 keyword; failure -> 15175]
  1: look

[15174 op=2 display+exit]
  1: an old winged gargoyle.
  2: The gargoyle shouts, "The False Prophet!" and flees!

[15175 op=2 display+exit]
  1: The gargoyle shouts, "The False Prophet!" and flees!

[15176 op=1 keyword; failure -> 15179]
  1: look

[15178 op=2 display+exit]
  1: an old winged gargoyle.
  2: "To feel honored by your visit, False Prophet."

[15179 op=1 keyword; failure -> 15182]
  1: look

[15181 op=2 display+exit]
  1: an old winged gargoyle.
  2: The gargoyle seems afraid of you at first.
  3: Then he notices the amulet around your neck.
  4: "To--to feel honored to address you, False Prophet."
  5: "To ask what you wish of a simple @snakecharmer?"

[15182 op=1 keyword; failure -> 15184]
  1: name

[15183 op=2 display+exit]
  1: The gargoyle laughs nervously.
  2: "To lack a name, of course."
  3: "To be called simply by my @job, snakecharmer."

[15184 op=1 keyword; failure -> 15186]
  1: job

[15185 op=2 display+exit]
  1: "To summon the silver @snake, and to direct the young warriors in defeating it."
  2: "Then to extract the venom so that I can make the @battle-drug."

[15186 op=1 keyword; failure -> 15188]
  1: veno,batt,drug

[15187 op=2 display+exit]
  1: "To make the drug for our bravest warriors, who take it and turn into fighting daemons!"
  2: "To express sorrow that the drug is fatal, but to express awe and respect for the warriors' sacrifice."

[15188 op=1 keyword; failure -> 15190]
  1: snak,silv,summ,char

[15189 op=2 display+exit]
  1: "To use the great @horn to summon the silver snake."
  2: "To always require many warriors to defeat the creature!"

[15190 op=1 keyword; failure -> 15192]
  1: horn

[15191 op=2 display+exit]
  1: "To find the horn just north of my house."
  2: "But to warn you never to blow the horn!"
  3: "To be in grave peril from the snake without a squad of warriors!"

[15192 op=1 keyword; failure -> 15194]
  1: bye

[15193 op=2 display+exit]
  1: "To express my thanks for the sacrifice you have chosen to make!"

[15194 op=2 display+exit]
  1: "To be but a simple snake charmer, unfamiliar with matters such as that."

```

<a id="spirituality-npc-196"></a>
## Spirituality ? NPC 196, port 197

Instruction range: 16729?16771

```text
[16733 op=1 keyword; failure -> 16735]
  1: look

[16734 op=2 display+exit]
  1: the altar of Spirituality.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about spirituality."
  4: Do you wish to meditate at this altar?

[16735 op=1 keyword; failure -> 16738]
  1: yes

[16737 op=2 display+exit]
  1: "Who will meditate?"

[16738 op=1 keyword; failure -> 16740]
  1: no

[16739 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16741 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable spirituality."
  3: Do you wish to meditate at this altar?

[16743 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16744 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about spirituality."
  3: Do you wish to meditate at this altar?

[16747 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16750 op=2 display+exit]
  1: "Speak the Mantra of Spirituality."

[16753 op=1 keyword; failure -> 16755]
  1: om

[16756 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16757 op=2 display+exit]
  1: "You receive no enlightenment."

[16763 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16764 op=2 display+exit]
  1: "Throughout this varied and eternal world, soul is the only element."
  2: "The wealth of the spirit is the only true wealth."
  3: "There is naught else for you to learn here and now."
  4: "Return when thy journey has progressed further."

[16770 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16771 op=2 display+exit]
  1: "Throughout this varied and eternal world, soul is the only element."
  2: "The wealth of the spirit is the only true wealth."
  3: $PARTYMEMBER has gained a level...
  4: intelligence...
  5: strength...
  6: and dexterity!

```

<a id="stelnar-npc-43"></a>
## Stelnar ? NPC 43, port 44

Instruction range: 4006?4068

```text
[4008 op=1 keyword; failure -> 4010]
  1: look

[4009 op=2 display+exit]
  1: an angry-looking man wearing short swords on either hip.
  2: He was just about to punch someone when you approached him.
  3: "This is no time for talk. There's fighting to do!"

[4010 op=2 display+exit]
  1: "This is no time for talk. There's fighting to do!"

[4012 op=1 keyword; failure -> 4018]
  1: look

[4015 op=22 display+continue]
  1: an angry-looking man wearing short swords on either hip.
  2: "Ho there, $G!"
  3: "Pleased to meet you, $G $P."

[4017 op=2 display+exit]
  1: "Good $T, Stelnar." @Shamino and Stelnar shake hands.
  2: "$P, this is Stelnar Starhelm."

[4018 op=1 keyword; failure -> 4020]
  1: look

[4019 op=2 display+exit]
  1: an angry-looking man wearing short swords on either hip.
  2: "Good $T."

[4020 op=1 keyword; failure -> 4023]
  1: name

[4022 op=2 display+exit]
  1: "I'm called Starhelm."

[4023 op=1 keyword; failure -> 4025]
  1: arvi

[4024 op=2 display+exit]
  1: "Arvin? He seems a good sort."

[4025 op=1 keyword; failure -> 4027]
  1: culh

[4026 op=2 display+exit]
  1: "He's a bard. I do not really know him."

[4027 op=1 keyword; failure -> 4029]
  1: jerr

[4028 op=2 display+exit]
  1: "Oh, yes. Brave fellow."

[4029 op=1 keyword; failure -> 4031]
  1: sham,rang

[4030 op=2 display+exit]
  1: "Shamino and I travelled the Spiritwood together for a time."

[4031 op=1 keyword; failure -> 4033]
  1: job

[4032 op=2 display+exit]
  1: "I am a @monster-slayer."

[4033 op=1 keyword; failure -> 4035]
  1: mons,slay

[4034 op=2 display+exit]
  1: "I hate all monsters. They are a blight on the face of Britannia!"
  2: "For the past year I've been killing @wisps and the like in Spiritwood."

[4035 op=1 keyword; failure -> 4037]
  1: blig,spir,wisp

[4036 op=2 display+exit]
  1: "Wisps are nasty flickers, very hard to kill. Not like @gargoyles, now."

[4037 op=1 keyword; failure -> 4039]
  1: garg

[4038 op=2 display+exit]
  1: "There's one that they call @Sin 'Vraal."
  2: "If I had my way, that gargoyle would be a @statue."

[4039 op=1 keyword; failure -> 4044]
  1: stat

[4041 op=22 display+continue]
  1: "They turn to stone when ye slay them."
  2: "Bards! A gargoyle is an @ugly monster, and that's that."

[4043 op=2 display+exit]
  1: "That's drivel, friend Starhelm!" says Van Kellian.
  2: "A gargoyle's a flesh and blood beast."
  3: "Stone indeed!"

[4044 op=1 keyword; failure -> 4046]
  1: stat

[4045 op=2 display+exit]
  1: "They turn to stone when ye slay them."

[4046 op=1 keyword; failure -> 4051]
  1: ugly

[4048 op=22 display+continue]
  1: "Never trust anything ugly."
  2: "Gargoyle skin is very thick and deep, as everyone knows."

[4050 op=2 display+exit]
  1: Van murmurs "Ugliness is but skin deep, just as beauty is."

[4051 op=1 keyword; failure -> 4053]
  1: ugly

[4052 op=2 display+exit]
  1: "Never trust anything ugly."

[4053 op=1 keyword; failure -> 4055]
  1: sin,vraa,'vra

[4054 op=2 display+exit]
  1: "He's a vicious @daemon."

[4055 op=1 keyword; failure -> 4057]
  1: daem,demo

[4056 op=2 display+exit]
  1: "Yes, Sin 'Vraal is an evil wretch."
  2: "How could Lord @British imagine that a gargoyle could understand virtue?"

[4057 op=1 keyword; failure -> 4059]
  1: lord,brit,virt

[4058 op=2 display+exit]
  1: "Lord British defeated Sin 'Vraal in the underworld."
  2: "The gargoyle went to live in the @Dry Land."

[4059 op=1 keyword; failure -> 4061]
  1: dry,land

[4060 op=2 display+exit]
  1: "'Tis a @desert east of the Bloody Plains. There is a shrine there."

[4061 op=1 keyword; failure -> 4063]
  1: shri,dese

[4062 op=2 display+exit]
  1: "I've never been to that desert, or the Shrine of Honesty."

[4063 op=1 keyword; failure -> 4066]
  1: bye

[4065 op=2 display+exit]
  1: "Farewell, $P."

[4066 op=1 keyword; failure -> 4068]
  1: bye

[4067 op=2 display+exit]
  1: "Have a pleasant $T, $G."

[4068 op=2 display+exit]
  1: "I cannot help ye with that."

```

<a id="stephanie-npc-146"></a>
## Stephanie ? NPC 146, port 147

Instruction range: 13321?13409

```text
[13321 op=1 keyword; failure -> 13325]
  1: look

[13324 op=2 display+exit]
  1: a young woman who stares into the distance.
  2: "Hello? Is someone there?"

[13325 op=1 keyword; failure -> 13327]
  1: look

[13326 op=2 display+exit]
  1: a young woman who stares into the distance.
  2: "I recognize your voice." She smiles.

[13327 op=1 keyword; failure -> 13330]
  1: name

[13329 op=2 display+exit]
  1: "Stephanie, but you can call me '@Steph.'"

[13330 op=1 keyword; failure -> 13332]
  1: step

[13331 op=2 display+exit]
  1: "That's better."

[13332 op=1 keyword; failure -> 13334]
  1: job

[13333 op=2 display+exit]
  1: "We have no 'jobs' here as such."
  2: "But I can @heal, @cure and @resurrect."

[13334 op=1 keyword; failure -> 13336]
  1: bye

[13335 op=2 display+exit]
  1: "Good $T, my friend. Take care."
  2: Her long lashes flitter a moment.

[13337 op=1 keyword; failure -> 13339]
  1: heal

[13338 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[13340 op=1 keyword; failure -> 13342]
  1: cure

[13341 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[13343 op=1 keyword; failure -> 13345]
  1: resu

[13344 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[13351 op=1 keyword; failure -> 13354]
  1: heal

[13353 op=2 display+exit]
  1: "Will you make an offering of 25 gold?"

[13355 op=1 keyword; failure -> 13358]
  1: yes

[13357 op=2 display+exit]
  1: "Which of you?"

[13360 op=2 display+exit]
  1: "I cannot heal you if you won't tithe."

[13361 op=1 keyword; failure -> 13364]
  1: cure

[13363 op=2 display+exit]
  1: "Will you make an offering of 5 gold?"

[13365 op=1 keyword; failure -> 13368]
  1: yes

[13367 op=2 display+exit]
  1: "Which of you?"

[13370 op=2 display+exit]
  1: "Without the offering I cannot cure you."

[13371 op=1 keyword; failure -> 13374]
  1: resu

[13373 op=2 display+exit]
  1: "Will you make an offering of 350 gold?"

[13375 op=1 keyword; failure -> 13377]
  1: yes

[13379 op=2 display+exit]
  1: "Then there is nothing I can do save grieve with you."

[13380 op=2 display+exit]
  1: "I don't think I'm the one to ask that of."

[13384 op=2 display+exit]
  1: "I'm sorry. Without an offering I cannot heal you."

[13387 op=2 display+exit]
  1: After touching your brow she states, "You feel fine to me."

[13389 op=2 display+exit]
  1: "I sense your injury, $PARTYMEMBER."
  2: She closes her eyes, puts a delicate hand on your brow and chants softly.
  3: You feel much better.
  4: Her blue eyes pierce you.
  5: "What else can I do for you?"

[13393 op=2 display+exit]
  1: "I'm sorry. Without an offering I cannot cure you."

[13396 op=2 display+exit]
  1: She lifts your hand against her lips.
  2: "You are not poisoned."

[13398 op=2 display+exit]
  1: A tear comes to her eyes.
  2: "I should have known you would not ask unless you were in grave need."
  3: "I sense that you are poisoned, $PARTYMEMBER."
  4: "Come, put your hand on my heart."
  5: Placing your hand over her heart, she closes her eyes and whispers something.
  6: The fever leaves your brow.
  7: Her blue eyes pierce you.
  8: "What else can I do for you?"

[13402 op=2 display+exit]
  1: "You haven't enough gold."
  2: "I'm sorry, all I can do is grieve with you."

[13405 op=2 display+exit]
  1: "But I sense none with you are dead."

[13407 op=2 display+exit]
  1: A tear comes to her eyes.
  2: "I should have known you would not ask unless you were in grave need."
  3: "I sense that someone has met a terrible fate."
  4: Your party takes up a collection for their fallen comrade.
  5: She lays hands upon the corpse...
  6: "Doman..."
  7: "thixus..."
  8: "anretu!"
  9: "I'm sorry, all I can do is grieve with you."

[13409 op=2 display+exit]
  1: A tear comes to her eyes.
  2: "I should have known you would not ask unless you were in grave need."
  3: "I sense that someone has met a terrible fate."
  4: Your party takes up a collection for their fallen comrade.
  5: She lays hands upon the corpse...
  6: "Doman..."
  7: "thixus..."
  8: "anretu!"
  9: And the dead live again!

```

<a id="stivius-npc-82"></a>
## Stivius ? NPC 82, port 83

Instruction range: 7772?7905

```text
[7773 op=1 keyword; failure -> 7776]
  1: yes

[7778 op=2 display+exit]
  1: "Too bad, it's really good wine."
  2: "What can I do you for?"

[7779 op=1 keyword; failure -> 7782]
  1: look

[7781 op=2 display+exit]
  1: a tiny little man with wild eyes.
  2: "Back for some more wine?"

[7782 op=1 keyword; failure -> 7785]
  1: look

[7784 op=2 display+exit]
  1: a tiny little man with wild eyes.
  2: "Yeah. Ok. Yeah."
  3: He looks you up and down and then claps his hands together.
  4: "Yeah. Ok. Yeah."
  5: "What can I do you for?"

[7785 op=1 keyword; failure -> 7787]
  1: name

[7786 op=2 display+exit]
  1: "@Stivius the wine merchant is mine."

[7787 op=1 keyword; failure -> 7789]
  1: stiv

[7788 op=2 display+exit]
  1: "Quite a name, eh?"

[7789 op=1 keyword; failure -> 7791]
  1: job

[7790 op=2 display+exit]
  1: "I make @wine."

[7793 op=1 keyword; failure -> 7799]
  1: skar

[7794 op=1 keyword; failure -> 7799]
  1: red

[7798 op=2 display+exit]
  1: He smiles at you.
  2: "For Skara red it will cost you 100 gold."
  3: "Interested?"

[7799 op=1 keyword; failure -> 7805]
  1: skar

[7800 op=1 keyword; failure -> 7805]
  1: dry

[7804 op=2 display+exit]
  1: He smiles at you.
  2: "For Skara dry it will cost you 100 gold."
  3: "Interested?"

[7805 op=1 keyword; failure -> 7810]
  1: sweet

[7809 op=2 display+exit]
  1: He smiles at you.
  2: "For sweet wine it will cost you 40 gold."
  3: "Interested?"

[7810 op=1 keyword; failure -> 7815]
  1: dry

[7814 op=2 display+exit]
  1: He smiles at you.
  2: "For dry wine it will cost you 30 gold."
  3: "Interested?"

[7815 op=1 keyword; failure -> 7820]
  1: red

[7819 op=2 display+exit]
  1: He smiles at you.
  2: "For red wine it will cost you 20 gold."
  3: "Interested?"

[7820 op=1 keyword; failure -> 7825]
  1: white

[7824 op=2 display+exit]
  1: He smiles at you.
  2: "For white wine it will cost you 10 gold."
  3: "Interested?"

[7825 op=1 keyword; failure -> 7828]
  1: no

[7827 op=2 display+exit]
  1: "Too bad, it's really good wine."
  2: "What can I do you for?"

[7830 op=1 keyword; failure -> 7833]
  1: yes

[7832 op=2 display+exit]
  1: "@White @wine, @red @wine, @dry @wine, @sweet @wine, @Skara @red or @Skara @dry?"

[7834 op=1 keyword; failure -> 7836]
  1: no

[7835 op=2 display+exit]
  1: "Too bad, it's really good wine."
  2: "What can I do you for?"

[7836 op=1 keyword; failure -> 7839]
  1: buy,wine,merc

[7838 op=2 display+exit]
  1: "Yeah."
  2: "Maybe you want to buy some."
  3: "@White @wine, @red @wine, @dry @wine, @sweet @wine, @Skara @red or @Skara @dry?"

[7839 op=1 keyword; failure -> 7841]
  1: ghos

[7840 op=2 display+exit]
  1: "It's not just a @tale you've been hearing."

[7841 op=1 keyword; failure -> 7843]
  1: tale

[7842 op=2 display+exit]
  1: "Everyone knows that @Quenton haunts this town."
  2: "If I had been @murdered by those evil ones... but that's just gossip."

[7843 op=1 keyword; failure -> 7845]
  1: ones,murd

[7844 op=2 display+exit]
  1: "It was late one night and I had just returned from my fields."
  2: "'Course my path takes me right by @Gideon's garden."
  3: "I heard a @noise by the well and saw some @dark forms hunched there."
  4: "They had huge wings and @evil red eyes."
  5: "I ran like the wind to my doorway."
  6: "I was sure I was dead."

[7845 op=1 keyword; failure -> 7847]
  1: quen

[7846 op=2 display+exit]
  1: "He was a nice enough soul."
  2: "I sold many bottles to him."

[7847 op=1 keyword; failure -> 7849]
  1: gide,gard

[7848 op=2 display+exit]
  1: "Gideon runs the Haunting @Inn."
  2: "His @wife has a garden which you can see from my front door."

[7849 op=1 keyword; failure -> 7851]
  1: haun,inn

[7850 op=2 display+exit]
  1: "It's the Inn straight across the lane from here."

[7851 op=1 keyword; failure -> 7853]
  1: wife

[7852 op=2 display+exit]
  1: "She doesn't do much."
  2: "Helps him run the place."

[7853 op=1 keyword; failure -> 7855]
  1: nois

[7854 op=2 display+exit]
  1: "Twas a strange @whispering, in a tongue I could not understand."

[7855 op=1 keyword; failure -> 7857]
  1: stra,whis,tong

[7856 op=2 display+exit]
  1: "It didn't sound like any language I ever heard."
  2: "More like one o' them guild @languages."

[7857 op=1 keyword; failure -> 7859]
  1: guil,lang

[7858 op=2 display+exit]
  1: "Like the one the @alchemist speaks."

[7859 op=1 keyword; failure -> 7861]
  1: alch

[7860 op=2 display+exit]
  1: "Go ask old @Horance."
  2: "He can tell ya - if you can understand him!"

[7861 op=1 keyword; failure -> 7863]
  1: hora

[7862 op=2 display+exit]
  1: "He lives over on @Ivers Rounding by himself."

[7863 op=1 keyword; failure -> 7865]
  1: iver,roun

[7864 op=2 display+exit]
  1: "You must be new to this village."
  2: "Go see @Marney, she'll get you there."

[7865 op=1 keyword; failure -> 7867]
  1: marn

[7866 op=2 display+exit]
  1: "She's the daughter of @Quenton - the ghost."

[7867 op=1 keyword; failure -> 7869]
  1: dark,form

[7868 op=2 display+exit]
  1: "I couldn't see too clearly."
  2: "They were about man-@sized though."

[7869 op=1 keyword; failure -> 7871]
  1: man,size

[7870 op=2 display+exit]
  1: "Yes, but they were hunched down so it was hard to see them."

[7871 op=1 keyword; failure -> 7873]
  1: huge,wing,evil,red,eyes

[7872 op=2 display+exit]
  1: His eyes widen.
  2: "@Gargoyles they were!"
  3: "Must'a been ten or @twelve of 'em."

[7873 op=1 keyword; failure -> 7875]
  1: ten,twel

[7874 op=2 display+exit]
  1: "Well, perhaps more like two or three."
  2: "I didn't stay around and count 'em."

[7875 op=1 keyword; failure -> 7877]
  1: garg

[7876 op=2 display+exit]
  1: "Yes."
  2: "Big, ugly, snarling beasts."
  3: "'Course, they were @kneeling when I saw 'em."

[7877 op=1 keyword; failure -> 7879]
  1: knee

[7878 op=2 display+exit]
  1: "You know, on their knees."

[7879 op=1 keyword; failure -> 7881]
  1: tren,bell

[7880 op=2 display+exit]
  1: "I don't care much for him."
  2: "He's always trying to get me to sell him my wine cheaper."

[7881 op=1 keyword; failure -> 7883]
  1: deza

[7882 op=2 display+exit]
  1: "She's my sweetie."
  2: He clasps his hands next to his cheek.
  3: "I count the hours until I can see her."

[7883 op=1 keyword; failure -> 7885]
  1: mich

[7884 op=2 display+exit]
  1: "I have never talked to him."
  2: "He bought wine from me once but that's it."

[7885 op=1 keyword; failure -> 7887]
  1: lord,brit

[7886 op=2 display+exit]
  1: "Well, what's to say?"
  2: "I am glad he is again our ruler."

[7887 op=1 keyword; failure -> 7889]
  1: bye

[7888 op=2 display+exit]
  1: "Come back if you get thirsty."

[7889 op=2 display+exit]
  1: "Ask someone else about that."

[7892 op=2 display+exit]
  1: He looks at you.
  2: "You don't have enough gold."

[7894 op=2 display+exit]
  1: "You look pretty full to me."

[7896 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the wine.
  3: "Enjoy your Skara red."

[7898 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the wine.
  3: "Enjoy your Skara dry."

[7900 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the wine.
  3: "Enjoy your sweet wine."

[7902 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the wine.
  3: "Enjoy your dry wine."

[7904 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the wine.
  3: "Enjoy your red wine."

[7905 op=2 display+exit]
  1: "Excellent!"
  2: After accepting the gold, he hands over the wine.
  3: "Enjoy your white wine."

```

<a id="sutek-npc-127"></a>
## Sutek ? NPC 127, port 128

Instruction range: 11740?11801

```text
[11741 op=1 keyword; failure -> 11744]
  1: yes

[11743 op=2 display+exit]
  1: "Good."

[11747 op=2 display+exit]
  1: "No?!"
  2: "Then you must die!"
  3: "But Sutek is mad, and so he will grant you one last nugget of wisdom before thy death!"
  4: "What wouldst thou like to know?"

[11749 op=1 keyword; failure -> 11752]
  1: yes

[11751 op=2 display+exit]
  1: "Well, too bad!"
  2: "Sutek isn't hiring today!"

[11755 op=2 display+exit]
  1: "No?!"
  2: "What sort of fool passes up the opportunity to work for the greatest genius of this age?"
  3: "Such fools must die!"
  4: "But Sutek is mad, and so he will grant you one last nugget of wisdom before thy death!"
  5: "What wouldst thou like to know?"

[11761 op=1 keyword; failure -> 11764]
  1: look

[11763 op=2 display+exit]
  1: a white-haired mage in flowing robes with a mad gleam in his eye.
  2: "What?!"
  3: "Sutek grants thou a piece of wisdom to take to thy grave, and thou refusest it?"
  4: "Then thou must die!"

[11767 op=2 display+exit]
  1: "Thy question is answered."
  2: "Now flee before the awesome wrath of the mad mage Sutek!"

[11773 op=1 keyword; failure -> 11775]
  1: look

[11774 op=2 display+exit]
  1: a white-haired mage in flowing robes with a mad gleam in his eye.
  2: "Get out!"
  3: "Flee before the awesome wrath of the mad mage Sutek!"

[11775 op=2 display+exit]
  1: "Get out!"
  2: "Flee before the awesome wrath of the mad mage Sutek!"

[11776 op=1 keyword; failure -> 11779]
  1: look

[11778 op=2 display+exit]
  1: a white-haired mage in flowing robes with a mad gleam in his eye.
  2: "What brings you back to disturb the musings of the @mad mage Sutek again?"

[11779 op=1 keyword; failure -> 11782]
  1: look

[11781 op=2 display+exit]
  1: a white-haired mage in flowing robes with a mad gleam in his eye.
  2: "So, Avatar, you disturb the musings of the @mad mage Sutek!"
  3: "What wisdom dost thou seek?"

[11782 op=1 keyword; failure -> 11784]
  1: name

[11783 op=2 display+exit]
  1: "The name Sutek strikes fear in the hearts of mere mortals!"
  2: "Everyone knows the name of Sutek the @Mad!"

[11784 op=1 keyword; failure -> 11787]
  1: job

[11786 op=2 display+exit]
  1: "Sutek the @Mad is the greatest mage the land has ever known!"
  2: "Even the Avatar acknowledges the power and wisdom of Sutek!"
  3: "Don't you, $G?"

[11787 op=1 keyword; failure -> 11790]
  1: ball

[11789 op=2 display+exit]
  1: "Balloonist?"
  2: "Yes, Sutek remembers him."
  3: "He died in Sutek's @catacombs, like most of the others who have ever worked for Sutek!"
  4: "Dost thou seek employment with Sutek the @Mad?"

[11790 op=1 keyword; failure -> 11792]
  1: mad

[11791 op=2 display+exit]
  1: "Yes!"
  2: "Sutek is mad!"
  3: "They say there is a fine line between genius and madness."
  4: "Well, Sutek hops up and down on that line, cursing the fools that surround him!"

[11792 op=1 keyword; failure -> 11794]
  1: cata

[11793 op=2 display+exit]
  1: "Sutek hired many brilliant engineers to build the catacombs that lie beneath his castle."
  2: "The catacombs are filled with the bones of those engineers!"
  3: "The @Pushme-@Pullyu knows where all such bones can be found!"

[11794 op=1 keyword; failure -> 11796]
  1: push,pull

[11795 op=2 display+exit]
  1: "Sutek created the Pushme-Pullyu as a test for fools!"
  2: "If you can pass the test, you will learn what you wish to know!"

[11796 op=1 keyword; failure -> 11798]
  1: bye

[11797 op=2 display+exit]
  1: "How darest thou leave the presence of Sutek the Mad before he has dissmissed you?!"
  2: "If thou art so rude, then I banish thee from my presence!"
  3: "Begone, and pester Sutek no further!"

[11800 op=2 display+exit]
  1: "Yes, Sutek knows all about $Z."
  2: "But he won't tell you!"

[11801 op=2 display+exit]
  1: "Only a fool would disturb Sutek with frivolous matters such as that!"

```

<a id="sylaina-npc-145"></a>
## Sylaina ? NPC 145, port 146

Instruction range: 13285?13320

```text
[13286 op=1 keyword; failure -> 13288]
  1: look

[13287 op=2 display+exit]
  1: a voluptuous woman dressed in dark blue velvet over white cloth.
  2: "Have you nothing better to do?" she asks pointedly.
  3: "Please excuse us."

[13289 op=2 display+exit]
  1: "Have you nothing better to do?" she asks pointedly.
  2: "Please excuse us."

[13290 op=1 keyword; failure -> 13293]
  1: look

[13292 op=2 display+exit]
  1: a voluptuous woman dressed in dark blue velvet over white cloth.
  2: "Hello, $G."

[13293 op=1 keyword; failure -> 13296]
  1: look

[13295 op=2 display+exit]
  1: a voluptuous woman dressed in dark blue velvet over white cloth.
  2: "Hello there, stranger."
  3: "Can I help you?"

[13296 op=1 keyword; failure -> 13298]
  1: name

[13297 op=2 display+exit]
  1: "I'm Sylaina @Moorehead."

[13298 op=1 keyword; failure -> 13300]
  1: job

[13299 op=2 display+exit]
  1: "When I need some money for food and such, I work as a @seamstress."
  2: "Most of the time, however, I live off the @seashore."

[13300 op=1 keyword; failure -> 13302]
  1: seam,sew,embr,clot

[13301 op=2 display+exit]
  1: "Yes."
  2: "I used to make such fine clothes for my @Nathaniel..."

[13302 op=1 keyword; failure -> 13304]
  1: nath

[13303 op=2 display+exit]
  1: "Though he was not a bad husband, I'll always remember him as something of a scoundrel."
  2: She smiles and her lovely eyes sparkle with memories.
  3: "They say he went down in a storm."
  4: "He was a @pirate."

[13304 op=1 keyword; failure -> 13306]
  1: moor

[13305 op=2 display+exit]
  1: "I'm a @widow now."
  2: "I live in a cottage on the beach."

[13306 op=1 keyword; failure -> 13308]
  1: sea,surf,ocea,beac,seas

[13307 op=2 display+exit]
  1: "There's always a bit of fish or gull's eggs to eat, with lobster once in a while."
  2: "I'm as carefree as a @gypsy."

[13308 op=1 keyword; failure -> 13310]
  1: wido

[13309 op=2 display+exit]
  1: "My husband left me a little money,
  2: to support myself for a time,
  3: but I still like to sew fine clothing."
  4: "There are few things as relaxing as embroidering and sitting by the ocean,
  5: listening to the surf."

[13310 op=1 keyword; failure -> 13312]
  1: gyps

[13311 op=2 display+exit]
  1: "They travel the road between Britain and Trinsic."
  2: "Normally I like them."
  3: "They're a merry lot."
  4: "But one time a group of gypsies @stole my prized @locket!"

[13312 op=1 keyword; failure -> 13314]
  1: lock

[13313 op=2 display+exit]
  1: "My husband left me a gold locket."
  2: "It contained a piece of an old ship's chart."
  3: "He said that chart would make me rich."
  4: "I didn't really care about the chart."
  5: "But the locket..."
  6: "That locket was all I had to remember @Nathaniel by."
  7: She sighs and looks away for a moment.

[13314 op=1 keyword; failure -> 13316]
  1: pira,map

[13315 op=2 display+exit]
  1: "My husband left me a gold locket wrapped in a piece of an old ship's chart."
  2: "Maybe that's what you seek."
  3: "I don't have the locket or the map anymore."
  4: "The gypsies came by one day and @stole them."

[13316 op=1 keyword; failure -> 13318]
  1: stol

[13317 op=2 display+exit]
  1: "Yes--some gypsies are trustworthy, but others are scoundrels!"

[13318 op=1 keyword; failure -> 13320]
  1: bye

[13319 op=2 display+exit]
  1: "Bye-bye."

[13320 op=2 display+exit]
  1: "Sorry, I cannot help you with that."

```

<a id="tara-npc-63"></a>
## Tara ? NPC 63, port 64

Instruction range: 5955?6041

```text
[5955 op=1 keyword; failure -> 5959]
  1: look

[5958 op=2 display+exit]
  1: a kindly old woman.
  2: "Welcome to my place of healing, Avatar."
  3: "I hope thou needest not my services!"

[5959 op=1 keyword; failure -> 5961]
  1: look

[5960 op=2 display+exit]
  1: a kindly old woman.
  2: "Welcome again, noble Avatar."
  3: "I hope this visit is for pleasure, not business!"

[5961 op=1 keyword; failure -> 5964]
  1: name

[5963 op=2 display+exit]
  1: "My name is Tara."

[5964 op=1 keyword; failure -> 5966]
  1: job

[5965 op=2 display+exit]
  1: "I'm the healer for this town."
  2: "I've cared for the @unfit here for threescore years!"

[5966 op=1 keyword; failure -> 5968]
  1: unfi

[5967 op=2 display+exit]
  1: "Those who need @healing, @curing, or to be @resurrected."

[5968 op=1 keyword; failure -> 5970]
  1: mant,sacr

[5969 op=2 display+exit]
  1: "'Cah,' that is the word of sacrifice."

[5976 op=1 keyword; failure -> 5979]
  1: heal

[5978 op=2 display+exit]
  1: "Wilt thou make an offering of 30 gold?"

[5980 op=1 keyword; failure -> 5983]
  1: yes

[5982 op=2 display+exit]
  1: "Which of you?"

[5985 op=2 display+exit]
  1: "I'm sorry. Without an offering I cannot heal thee."

[5986 op=1 keyword; failure -> 5989]
  1: cure

[5988 op=2 display+exit]
  1: "Wilt thou make an offering of 10 gold?"

[5990 op=1 keyword; failure -> 5993]
  1: yes

[5992 op=2 display+exit]
  1: "Which of you?"

[5995 op=2 display+exit]
  1: "I'm sorry. Without an offering I cannot cure thee."

[5996 op=1 keyword; failure -> 5999]
  1: resu

[5998 op=2 display+exit]
  1: "Wilt thou make an offering of 400 gold?"

[6000 op=1 keyword; failure -> 6002]
  1: yes

[6004 op=2 display+exit]
  1: "Then there is naught I can do save grieve with thee."

[6005 op=1 keyword; failure -> 6007]
  1: bye,no

[6006 op=2 display+exit]
  1: "Farewell!"

[6007 op=1 keyword; failure -> 6009]
  1: yes

[6008 op=2 display+exit]
  1: "What can I do for thee?"

[6011 op=2 display+exit]
  1: "I don't understand $Z."

[6012 op=2 display+exit]
  1: "I beg thy pardon?"

[6016 op=2 display+exit]
  1: "I'm sorry. Without an offering I cannot heal thee."

[6019 op=2 display+exit]
  1: "But thou art well!"

[6021 op=2 display+exit]
  1: "I see thy injury, $PARTYMEMBER."
  2: Tara approaches $PARTYMEMBER and binds the wounds.
  3: "Is there aught else I can do for thee?"

[6025 op=2 display+exit]
  1: "I'm sorry. Without an offering I cannot cure thee."

[6028 op=2 display+exit]
  1: "But thou art well!"

[6030 op=2 display+exit]
  1: "I see thy injury, $PARTYMEMBER."
  2: Tara grabs a vial and pours it down $PARTYMEMBER's throat.
  3: $PARTYMEMBER feels much better.
  4: "Is there aught else I can do for thee?"

[6034 op=2 display+exit]
  1: "That ain't enough money."

[6037 op=2 display+exit]
  1: "But gladly can I say that none of your party is dead!"

[6039 op=2 display+exit]
  1: "Oh! I see that you carry a fallen comrade!"
  2: Your party takes up a collection for their fallen comrade.
  3: Tara lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: "I'm sorry. There is naught I can do save grieve with thee."

[6041 op=2 display+exit]
  1: "Oh! I see that you carry a fallen comrade!"
  2: Your party takes up a collection for their fallen comrade.
  3: Tara lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: And the dead live again!

```

<a id="taynith-npc-134"></a>
## Taynith ? NPC 134, port 135

Instruction range: 12296?12389

```text
[12297 op=1 keyword; failure -> 12302]
  1: yes

[12303 op=1 keyword; failure -> 12306]
  1: yes

[12305 op=2 display+exit]
  1: "Best check your purse."

[12308 op=2 display+exit]
  1: "Very well."

[12310 op=1 keyword; failure -> 12315]
  1: yes

[12314 op=2 display+exit]
  1: "Look through the books at the @Lycaeum."
  2: "You might learn something useful there."

[12316 op=1 keyword; failure -> 12319]
  1: yes

[12318 op=2 display+exit]
  1: "Best check your purse."

[12321 op=2 display+exit]
  1: "Very well."

[12323 op=1 keyword; failure -> 12325]
  1: look

[12324 op=2 display+exit]
  1: a mysterious gypsy woman wearing an ankh pendant.
  2: "I have no time for those who are cruel to animals."

[12326 op=2 display+exit]
  1: "I have no time for those who are cruel to animals."

[12327 op=1 keyword; failure -> 12330]
  1: look

[12329 op=2 display+exit]
  1: a mysterious gypsy woman wearing an ankh pendant.
  2: "My chances to visit Dr. @Cat come all too seldom."
  3: "Seek me out later and I will speak with you."

[12331 op=1 keyword; failure -> 12333]
  1: look

[12332 op=2 display+exit]
  1: a mysterious gypsy woman wearing an ankh pendant.
  2: "Welcome back, Avatar."

[12333 op=1 keyword; failure -> 12336]
  1: look

[12335 op=2 display+exit]
  1: a mysterious gypsy woman wearing an ankh pendant.
  2: "You are the Avatar."
  3: "I had a vision that you would come to seek aid from me."

[12336 op=1 keyword; failure -> 12338]
  1: name

[12337 op=2 display+exit]
  1: "Some know me as @Taynith."
  2: "That is what you may call me."

[12338 op=1 keyword; failure -> 12340]
  1: tayn,know,call,pati

[12339 op=2 display+exit]
  1: "Seek not my @true name."
  2: "Let 'Taynith' suffice."

[12340 op=1 keyword; failure -> 12342]
  1: true

[12341 op=2 display+exit]
  1: "You try my patience."

[12342 op=1 keyword; failure -> 12344]
  1: job,aid

[12343 op=2 display+exit]
  1: "I tell @fortunes - for a price."

[12344 op=1 keyword; failure -> 12346]
  1: gorg

[12345 op=2 display+exit]
  1: "A gorgio - that's almost as good as being a gypsy!"

[12346 op=1 keyword; failure -> 12348]
  1: capt,john

[12347 op=2 display+exit]
  1: "Captain John?"
  2: "Yes, he brought us a silver tablet some time ago, to deliver to the Lycaeum."
  3: "@Zoltan took care of the deal, so he could tell you more than I."

[12348 op=1 keyword; failure -> 12350]
  1: silv,tabl,lyca

[12349 op=2 display+exit]
  1: "Ask @Zoltan about it."

[12350 op=1 keyword; failure -> 12352]
  1: zolt

[12351 op=2 display+exit]
  1: "King of the gypsies, sure as my name's Taynith."

[12352 op=1 keyword; failure -> 12354]
  1: path

[12353 op=2 display+exit]
  1: "My path leads to @Paws, as soon as I have time to visit there."

[12354 op=1 keyword; failure -> 12356]
  1: paw

[12355 op=2 display+exit]
  1: "My good @friend Dr. Cat lives there."

[12356 op=1 keyword; failure -> 12358]
  1: frie,dr,cat

[12357 op=2 display+exit]
  1: "Ask him about the duck sometime."

[12358 op=1 keyword; failure -> 12360]
  1: ask,duck

[12359 op=2 display+exit]
  1: "Yes, ask him!"

[12360 op=1 keyword; failure -> 12362]
  1: lyca

[12361 op=2 display+exit]
  1: "@Zoltan can tell you where it is."

[12363 op=1 keyword; failure -> 12365]
  1: penu

[12364 op=2 display+exit]
  1: "I sense that you have met her already."

[12365 op=1 keyword; failure -> 12367]
  1: penu

[12366 op=2 display+exit]
  1: "There is no need for me to direct you there."
  2: "You are destined to meet her."

[12367 op=1 keyword; failure -> 12370]
  1: tell,fort,sell,buy,pric,cons,tile,futu

[12369 op=2 display+exit]
  1: "I charge six gold to cast my tiles."
  2: "Are you interested?"

[12370 op=1 keyword; failure -> 12373]
  1: advi

[12372 op=2 display+exit]
  1: "My advice will cost you two gold."
  2: "Do you still want it?"

[12373 op=1 keyword; failure -> 12375]
  1: bye

[12374 op=2 display+exit]
  1: "Our @paths will cross again. I have foreseen it."

[12375 op=2 display+exit]
  1: "No doubt."

[12378 op=2 display+exit]
  1: From the folds of her cloak, she withdraws a velvet pouch.
  2: After shaking it gently, she casts forth three small squares of ivory,
  3: each with an image etched into its surface and a title at the bottom.
  4: The tiles are: 'Shaman', 'Blademaster', and 'Clever Fish'.
  5: Taynith says: "All of these things and more must you be to find success in your quest."
  6: "If you consult the tiles another time, perhaps they will have more to say of your @future."

[12381 op=2 display+exit]
  1: From the folds of her cloak, she withdraws a velvet pouch.
  2: After shaking it gently, she casts forth three small squares of ivory,
  3: each with an image etched into its surface and a title at the bottom.
  4: The tiles are: 'Bead of Glass', 'Maelstrom', and 'Clever Fish'.
  5: Taynith frowns.
  6: "I cannot interpret this pattern."
  7: "I'm afraid you'll have to seek its meaning on your own."
  8: "If you consult the tiles another time, perhaps they will have more to say of your @future."

[12384 op=2 display+exit]
  1: From the folds of her cloak, she withdraws a velvet pouch.
  2: After shaking it gently, she casts forth three small squares of ivory,
  3: each with an image etched into its surface and a title at the bottom.
  4: The tiles are: 'Path', 'War', and 'Rogue'.
  5: The 'War' tile is particularly striking.
  6: It depicts a dragon and a tiger, each about to bite the others tail.
  7: Taynith says: "Britannia is on a path towards all out war, that is plain enough."
  8: "The 'Rogue' is one who would go against the wishes of his Lord to find another path."
  9: "If you consult the tiles another time, perhaps they will have more to say of your @future."

[12387 op=2 display+exit]
  1: From the folds of her cloak, she withdraws a velvet pouch.
  2: After shaking it gently, she casts forth three small squares of ivory,
  3: each with an image etched into its surface and a title at the bottom.
  4: The tiles are: 'Abyss', 'Mountains', and 'Maelstrom'.
  5: Taynith says: "This is strange."
  6: "The tiles show that to accomplish your ends, you must go down very far."
  7: "Then up very far, but you will not end up back where you started."
  8: "The 'Maelstrom' indicates something important as well, but I cannot determine what it means."
  9: "If you consult the tiles another time, perhaps they will have more to say of your @future."

[12389 op=2 display+exit]
  1: From the folds of her cloak, she withdraws a velvet pouch.
  2: After shaking it gently, she casts forth three small squares of ivory,
  3: each with an image etched into its surface and a title at the bottom.
  4: The tiles are: 'Panda', 'Coin', and 'Bead of Glass'.
  5: Taynith says: "There is something made of glass that plays a very important role in your future, but the signs are unclear."
  6: "Seek out @Penumbra."
  7: "Her crystal ball should hold more of an affinity for this item."
  8: "If you pay her enough, she can tell you more than I."

```

<a id="terri-npc-28"></a>
## Terri ? NPC 28, port 29

Instruction range: 2317?2489

```text
[2324 op=1 keyword; failure -> 2326]
  1: look

[2325 op=2 display+exit]
  1: a sharply dressed, very voluptuous woman.
  2: "I'm eating right now. Come see me at the mint sometime and we can chat."

[2327 op=2 display+exit]
  1: "I'm eating right now. Come see me at the mint sometime and we can chat."

[2329 op=1 keyword; failure -> 2331]
  1: look

[2330 op=2 display+exit]
  1: a sharply dressed, very voluptuous woman.
  2: "Oh, you've come back to visit me again!"

[2332 op=1 keyword; failure -> 2335]
  1: look

[2334 op=2 display+exit]
  1: a sharply dressed, very voluptuous woman.
  2: She winks at you.
  3: "Hello. My name's Terri - I run the mint."
  4: "Say, aren't you the Avatar? Yes, I recognize you from your portrait..."
  5: "My mother used to tell me stories about you when I was a girl."
  6: "You probably wouldn't remember her, but she was at the big celebration right after you recovered the Codex."
  7: "Anyway... I just wanted to tell you that I always wanted to grow up to be just like you..."

[2335 op=1 keyword; failure -> 2338]
  1: look

[2337 op=2 display+exit]
  1: a sharply dressed, very voluptuous woman.
  2: She winks at you.
  3: "Hello. My name's Terri - I run the mint."
  4: "Say, aren't you the Avatar? Yes, I recognize you from your portrait..."
  5: "My mother used to tell me stories about you when I was a girl."
  6: "You probably wouldn't remember her, but she was at the big celebration right after you recovered the Codex."
  7: "Anyway... I just wanted to tell you that I always hoped someday I'd meet a man like you..."

[2339 op=1 keyword; failure -> 2342]
  1: name

[2341 op=2 display+exit]
  1: "It's Terri..."
  2: But I wish I had been named after you."

[2342 op=1 keyword; failure -> 2345]
  1: name

[2344 op=2 display+exit]
  1: "It's Terri..."
  2: "Don't you think that would be a good name for an Avatar's wife?"

[2345 op=1 keyword; failure -> 2347]
  1: job

[2346 op=2 display+exit]
  1: "By royal decree, I am the only one allowed to @mint the official coinage of the realm."
  2: "Copper @pence, silver pieces, and @gold crowns - they're all made right here."
  3: "My @father ran the mint before me. But I get lonely here sometimes..."
  4: "It's so nice of you to come by and talk to me."
  5: "@Kytyn will never believe it when I tell her I got to meet the @Avatar in person!"

[2348 op=1 keyword; failure -> 2350]
  1: avat

[2349 op=2 display+exit]
  1: "You must be the bravest woman I've ever met. I can't believe it's really you!"

[2350 op=1 keyword; failure -> 2352]
  1: avat

[2351 op=2 display+exit]
  1: "You must be the bravest man I've ever met. I can't believe it's really you!"

[2352 op=1 keyword; failure -> 2354]
  1: fath

[2353 op=2 display+exit]
  1: "May he rest in peace."

[2354 op=1 keyword; failure -> 2356]
  1: moth

[2355 op=2 display+exit]
  1: "May she rest in peace."

[2356 op=1 keyword; failure -> 2358]
  1: copp,penc,silv,piec

[2357 op=2 display+exit]
  1: "Of course, I'm sure you only carry @gold crowns with you, being as important as you are."
  2: She reaches over and squeezes your shoulder affectionately.

[2358 op=1 keyword; failure -> 2360]
  1: mint,offi,coin,made

[2359 op=2 display+exit]
  1: "Here, let me show you." She takes out a sample of each coin for you to look at.
  2: The pence has an ankh on one side, and crossed swords on the other.
  3: The silver piece has Lord British's face on the front, and a serpent on the back.
  4: And the gold crown is a familiar sight, with a crown and the symbol of the Codex.
  5: All three of them look newly minted - they're very shiny.

[2360 op=1 keyword; failure -> 2362]
  1: gold,nugg,crow

[2361 op=2 display+exit]
  1: "Britannia has been so prosperous lately that there's been a lot of demand for gold coinage."
  2: "Of course we have you to thank for bringing us the Codex and saving Lord British."
  3: "I get most of my gold from people who bring in nuggets to @exchange them for coins."
  4: "They also @bank gold here."

[2362 op=1 keyword; failure -> 2364]
  1: code,lord

[2363 op=2 display+exit]
  1: "My @mother told me all about it."

[2364 op=1 keyword; failure -> 2366]
  1: kyty

[2365 op=2 display+exit]
  1: "She's a good friend of mine. She runs the @museum."

[2366 op=1 keyword; failure -> 2368]
  1: muse

[2367 op=2 display+exit]
  1: "You should visit there sometime. But please stay and talk to me a bit longer first..."

[2368 op=1 keyword; failure -> 2371]
  1: exch,trad

[2370 op=2 display+exit]
  1: "I get most of my gold from people who bring in nuggets to @exchange them for coins."
  2: "They also @bank gold here."
  3: "Do you have any gold nuggets you'd like to trade in for crowns?"

[2372 op=1 keyword; failure -> 2375]
  1: yes

[2374 op=2 display+exit]
  1: "How many gold nuggets would you like to trade in for crowns?"

[2377 op=1 keyword; failure -> 2379]
  1: no

[2378 op=2 display+exit]
  1: "Well, if you do later, you know where to find me."

[2381 op=1 keyword; failure -> 2383]
  1: bank

[2382 op=2 display+exit]
  1: "Would you like to @deposit, @withdraw or check your @balance?"

[2383 op=1 keyword; failure -> 2386]
  1: depo

[2385 op=2 display+exit]
  1: "How much would you like to @deposit?"

[2386 op=1 keyword; failure -> 2389]
  1: with

[2388 op=2 display+exit]
  1: "How much would you like to @withdraw?"

[2389 op=1 keyword; failure -> 2391]
  1: bala

[2396 op=1 keyword; failure -> 2398]
  1: bye

[2397 op=2 display+exit]
  1: "I know you're very busy, saving Britannia and all that, but I hope you'll come and visit me again...

[2399 op=2 display+exit]
  1: You notice she was staring at you with a dreamy look in her eyes.
  2: "Oh, I'm sorry, I wasn't listening. I was just thinking how nice it must be to be as beautiful as you."
  3: She looks away and sighs.

[2400 op=2 display+exit]
  1: You notice she was staring at you with a dreamy look in her eyes.
  2: "Oh, I'm sorry, I wasn't listening. I was just noticing how strong and handsome you are..."
  3: She looks away and sighs.

[2403 op=2 display+exit]
  1: "Well, if you do later, you know where to find me."

[2405 op=2 display+exit]
  1: "You must be mistaken."
  2: "But let me know if you come across that many gold nuggets later."

[2410 op=2 display+exit]
  1: "Normally there's a ten percent tax on the exchange."
  2: "But since you're the Avatar, I'll give you a full hundred crowns for every stone's-weight of gold you have."
  3: She exchanges all of your gold nuggets for newly minted crowns.

[2411 op=2 display+exit]
  1: "You must be mistaken."
  2: "But let me know if you come across that many gold nuggets later."

[2417 op=2 display+exit]
  1: "You need to deposit something, perhaps you would like to @withdraw instead."

[2419 op=2 display+exit]
  1: "I'm sorry $G, you can only deposit up to 10000 gold coins at a time."

[2429 op=2 display+exit]
  1: "Normally there's a tax on deposits."
  2: "But since you're the Avatar, I'll deposit the full amount."
  3: She winks, then accepts your gold.

[2431 op=2 display+exit]
  1: "You must be mistaken milady, you don't have that much gold to @deposit."

[2432 op=2 display+exit]
  1: "You must be mistaken milord, you don't have that much gold to @deposit."

[2438 op=2 display+exit]
  1: "Well, if you do later, you know where to find me."

[2440 op=2 display+exit]
  1: "I'm sorry $G, you can only withdraw up to 10000 gold coins at a time."

[2446 op=2 display+exit]
  1: "Sorry milady, but your account is empty, would you like to make a @deposit instead?"

[2447 op=2 display+exit]
  1: "Sorry milord, but your account is empty, would you like to make a @deposit instead?"

[2454 op=2 display+exit]
  1: "I'm sorry, you don't have that much gold in your account."

[2458 op=2 display+exit]
  1: "You can't carry that much gold!"

[2462 op=2 display+exit]
  1: "Here you are milady."
  2: She hands you the gold.

[2463 op=2 display+exit]
  1: "Here you are milord."
  2: She hands you the gold.

[2470 op=2 display+exit]
  1: "You have $V0 gold coins!"
  2: "Once you have 10000 I shall melt them into a gold bar."

[2472 op=2 display+exit]
  1: "You have $V0 gold coins!"

[2474 op=2 display+exit]
  1: "You have $V0 gold coins."

[2476 op=2 display+exit]
  1: "You only have 1 gold coin."

[2477 op=2 display+exit]
  1: "Sorry, your account seems to be empty, perhaps you would like to make a @deposit?"

[2481 op=2 display+exit]
  1: "You have 1 gold bar $G."

[2483 op=2 display+exit]
  1: "You have 1 gold bar and one gold coin $G."

[2484 op=2 display+exit]
  1: "You have 1 gold bar and $V0 gold coins $G."

[2486 op=2 display+exit]
  1: "You have $V1 gold bars $G."

[2488 op=2 display+exit]
  1: "You have $V1 gold bars and one gold coin $G."

[2489 op=2 display+exit]
  1: "You have $V1 gold bars and $V0 gold coins $G."

```

<a id="tessa-npc-159"></a>
## Tessa ? NPC 159, port 160

Instruction range: 14485?14503

```text
[14485 op=1 keyword; failure -> 14488]
  1: look

[14487 op=2 display+exit]
  1: a well dressed woman.
  2: "Welcome again to Serpent's Hold, Avatar!"

[14488 op=1 keyword; failure -> 14491]
  1: look

[14490 op=2 display+exit]
  1: "Ah, $G $P!"
  2: "A sailor from Britain said thou had returned to the land!"
  3: "How glad I am to see that it is true!"

[14491 op=1 keyword; failure -> 14493]
  1: name

[14492 op=2 display+exit]
  1: "I'm Tessa."
  2: "We met during thy last quest, Avatar!"
  3: "Strange that thou didst not recognize me."
  4: "I would have hoped that the past years had not taken that much toll on me!"

[14493 op=1 keyword; failure -> 14495]
  1: job

[14494 op=2 display+exit]
  1: "I'm the keeper of the Flame of @Courage."
  2: "I also help my husband @Simon run this town."
  3: "How fortunate we were to come here after the @earthquake!"

[14495 op=1 keyword; failure -> 14497]
  1: husb,simo

[14496 op=2 display+exit]
  1: "He is the lord of this hold."

[14497 op=1 keyword; failure -> 14499]
  1: flam,cour

[14498 op=2 display+exit]
  1: "You'll find many courageous fighters here!"

[14499 op=1 keyword; failure -> 14501]
  1: eart,toll,year

[14500 op=2 display+exit]
  1: "After thy last quest, there were many great earthquakes."
  2: "The island of Bordermarch sank into the sea, along with our keep!"
  3: "Only through great fortune did Simon and I make it off the island alive!"

[14501 op=1 keyword; failure -> 14503]
  1: bye

[14502 op=2 display+exit]
  1: "Farewell, Avatar!"
  2: "Be brave in thy quest!"

[14503 op=2 display+exit]
  1: "Pardon?"

```

<a id="thariand-npc-32"></a>
## Thariand ? NPC 32, port 33

Instruction range: 2695?2748

```text
[2695 op=1 keyword; failure -> 2697]
  1: look

[2696 op=2 display+exit]
  1: a busy looking man wearing a blue robe.
  2: "So many books to keep organized, it's a wonder I have any time for my own studies..."
  3: "Can I help you with something?"

[2697 op=1 keyword; failure -> 2700]
  1: name

[2699 op=2 display+exit]
  1: "I am Thariand, of the blue @star."

[2700 op=1 keyword; failure -> 2702]
  1: blue,star

[2701 op=2 display+exit]
  1: "All the students of @Nicodemus bear this sign."

[2702 op=1 keyword; failure -> 2704]
  1: nico

[2703 op=2 display+exit]
  1: "He lives southeast of @Yew."

[2704 op=1 keyword; failure -> 2706]
  1: yew

[2705 op=2 display+exit]
  1: "Somebody there should be able to give you directions."

[2706 op=1 keyword; failure -> 2708]
  1: job

[2707 op=2 display+exit]
  1: "I'm the librarian."
  2: "I prefer to spend my time studying @magic, but I'll help you find some @books if you keep it quick."

[2708 op=1 keyword; failure -> 2710]
  1: stud,magi

[2709 op=2 display+exit]
  1: "Were I to master all the arcane lore contained within these volumes, I'd be the greatest sorcerer of all time!"
  2: "Of course, finding all the right books is no small task..."

[2710 op=1 keyword; failure -> 2712]
  1: begi,guid,dewe,deci,syst

[2711 op=2 display+exit]
  1: "Oh, I'm sorry, I just remembered. That book was checked out last week. Try coming back for it later."

[2712 op=1 keyword; failure -> 2714]
  1: shri

[2713 op=2 display+exit]
  1: "We have a book that tells the history of the shrines. If only I could remember the title..."

[2714 op=1 keyword; failure -> 2716]
  1: bye

[2715 op=2 display+exit]
  1: "Remember, any books you take out of here are due back before the next bipolar lunar conjunction!"

[2716 op=1 keyword; failure -> 2719]
  1: help,find,book

[2718 op=2 display+exit]
  1: "Were you interested in @fiction, @non-fiction or @Reference?"

[2720 op=1 keyword; failure -> 2723]
  1: fict

[2722 op=2 display+exit]
  1: "In that category I can recommend:"
  2: "821.34 Rzl4n The Caverns of Freitag"
  3: "Klr1734 N92 v3 Around the World in a Washtub"
  4: "417.8 Baum c6 The Wizard of Oz"
  5: "Is there anything else you wanted to find?"

[2724 op=1 keyword; failure -> 2727]
  1: non

[2726 op=2 display+exit]
  1: "In that category I can recommend:"
  2: "Ce 437.2 N19 Mating Rituals of the Northern Plains Centaur"
  3: "027 z811f Of Dreams and Visions"
  4: "718.5 B34 z5 The Lost Art of Ballooning"
  5: "Y19 T343 LP Summoning Incubi for Fun and Profit"
  6: "Is there anything else you wanted to find?"

[2728 op=1 keyword; failure -> 2731]
  1: refe

[2730 op=2 display+exit]
  1: "In that category I can recommend:"
  2: "665.556 Dlz Dilzal's Almanac of Good Advice"
  3: "a001.3 bfb Priliwig's Universal Compendium of Knowledge"
  4: "998.99 ZWX The Lost Book of Mantras"
  5: "Is there anything else you wanted to find?"

[2732 op=1 keyword; failure -> 2735]
  1: yes

[2734 op=2 display+exit]
  1: "Were you interested in @fiction, @non-fiction or @Reference?"

[2736 op=1 keyword; failure -> 2739]
  1: no

[2738 op=2 display+exit]
  1: "Fine. You should have no trouble finding what you want once you have the call number."
  2: "You do understand how our books are organized, don't you?"

[2740 op=1 keyword; failure -> 2743]
  1: yes

[2742 op=2 display+exit]
  1: "Good. I have more important things to do than explain it to you."

[2744 op=1 keyword; failure -> 2747]
  1: no

[2746 op=2 display+exit]
  1:  "Well then, I'd suggest you look it up in The Beginner's Guide to the @Dewey Decimal System."
  2: "Its call number is T17 0035 r16RL.5 v43."

[2748 op=2 display+exit]
  1: "You could probably find more information on that topic in Priliwig's Universial Compendium of Knowledge."

```

<a id="thindle-npc-98"></a>
## Thindle ? NPC 98, port 99

Instruction range: 9132?9243

```text
[9136 op=1 keyword; failure -> 9139]
  1: yes

[9138 op=2 display+exit]
  1: "Good, good, how many do you want to give me?"

[9141 op=2 display+exit]
  1: "Very well, very well, I can see you don't want to sell."

[9144 op=1 keyword; failure -> 9147]
  1: look

[9146 op=2 display+exit]
  1: a little, white haired man hunched over with age.
  2: "Back, back, back they come."
  3: He smiles to himself and claps his hands.

[9147 op=1 keyword; failure -> 9150]
  1: look

[9149 op=2 display+exit]
  1: a little, white haired man hunched over with age.
  2: "Yes, yes, yes," he says, bobbing his head up and down.

[9150 op=1 keyword; failure -> 9153]
  1: name

[9152 op=2 display+exit]
  1: "@Thindle the spindler." He grins at you.

[9153 op=1 keyword; failure -> 9155]
  1: thin

[9154 op=2 display+exit]
  1: "Yes, yes, yes."
  2: "That was quite a guess!"

[9155 op=1 keyword; failure -> 9157]
  1: job

[9156 op=2 display+exit]
  1: "Sew, sew, I @sew 'cause I said so."

[9157 op=1 keyword; failure -> 9159]
  1: sew

[9158 op=2 display+exit]
  1: "Beautiful @clothes, more pretty than those," he points to your stomach.

[9159 op=1 keyword; failure -> 9161]
  1: beau,clot

[9160 op=2 display+exit]
  1: "Yes again, friend!"

[9161 op=1 keyword; failure -> 9163]
  1: mort

[9162 op=2 display+exit]
  1: "Shorty is my friend."
  2: "We play @flippits together."

[9163 op=1 keyword; failure -> 9165]
  1: silk,bag,ball

[9164 op=2 display+exit]
  1: "Silk?"
  2: "Oh no, oh no."
  3: "Can't work silk, oh my, no."
  4: "See @Charlotte."
  5: "She can, yes she can!"

[9165 op=1 keyword; failure -> 9167]
  1: char

[9166 op=2 display+exit]
  1: "Humble Charlotte, yes."
  2: "She can help you."
  3: "She lives in New Magincia."

[9167 op=1 keyword; failure -> 9169]
  1: flip

[9168 op=2 display+exit]
  1: "You should learn, you should learn."
  2: "A nice piece of bone and a small hard @pea--that's all ya need."

[9169 op=1 keyword; failure -> 9171]
  1: nice,piec,bone

[9170 op=2 display+exit]
  1: "Like an old dog bone."
  2: "You'll need a @hat too."

[9171 op=1 keyword; failure -> 9173]
  1: smal,hard,pea

[9172 op=2 display+exit]
  1: "The smaller, the better."
  2: He pulls an old @bone from his pocket and a small pea.
  3: "Now you get a @hat."

[9173 op=1 keyword; failure -> 9175]
  1: hat

[9174 op=2 display+exit]
  1: He puts the hat down and mumbles "Put the pea in front of bone..."
  2: He gestures with his hands.
  3: "...and flippit on home."
  4: The pea sails into the hat.

[9175 op=1 keyword; failure -> 9177]
  1: bye

[9176 op=2 display+exit]
  1: "Bye now."
  2: "Bye."
  3: "Say hi to @Mortude if you see him."

[9179 op=1 keyword; failure -> 9182]
  1: buy

[9181 op=2 display+exit]
  1: "Oh my, oh my, you must need cloth."
  2: "Very well, but 'tis fine cloth, yes indeed..."
  3: "It'll cost you 15 gold for the cloth."
  4: "Yes, so how many pieces do ya need?"

[9182 op=1 keyword; failure -> 9192]
  1: sell

[9192 op=1 keyword; failure -> 9196]
  1: sell

[9195 op=2 display+exit]
  1: "Sell, now, its thread I feel."
  2: He turns to you, and says "Will you take $V1 gold for that thread?"

[9196 op=1 keyword; failure -> 9198]
  1: sell

[9197 op=2 display+exit]
  1: "Sell, now, its thread I feel."
  2: "Sorry, none of you has anything I need."

[9198 op=1 keyword; failure -> 9200]
  1: buy

[9199 op=2 display+exit]
  1: "Come to my shop when I'm open and I'll sell you some!"

[9200 op=1 keyword; failure -> 9202]
  1: sell

[9201 op=2 display+exit]
  1: "Come to my shop when I'm open and I'll buy it!"

[9202 op=2 display+exit]
  1: He scratches his head and looks around.

[9205 op=2 display+exit]
  1: "Very well, but 'tis fine cloth, yes indeed..."

[9207 op=2 display+exit]
  1: "You look pretty full to me."

[9218 op=2 display+exit]
  1: "No discounts, friend."
  2: "Sorry."

[9220 op=2 display+exit]
  1: "You look pretty full to me."

[9221 op=2 display+exit]
  1: "Excellent!"
  2: After accepting your gold, he hands over the cloth.

[9224 op=2 display+exit]
  1: "Very well, very well, I can see you don't want to sell."

[9226 op=2 display+exit]
  1: He scratches his head.
  2: "Sorry, I can't use any more than 10."

[9240 op=2 display+exit]
  1: "Sorry, you don't have that much thread."

[9243 op=2 display+exit]
  1: "Done!"
  2: He hands you $V1 gold and takes the thread.

```

<a id="tholden-npc-6"></a>
## Tholden ? NPC 6, port 7

Instruction range: 470?597

```text
[475 op=1 keyword; failure -> 478]
  1: yes

[477 op=2 display+exit]
  1: "How much do you wish to pay $G?"

[480 op=2 display+exit]
  1: "Remember to make @payments in advance, otherwise you will be evicted."

[482 op=1 keyword; failure -> 484]
  1: yes

[486 op=2 display+exit]
  1: "'Tis understandable, is there anything else I can help you with?"

[493 op=2 display+exit]
  1: He rustles through his papers.
  2: "You must be mistaken, I'd know if that house was available."

[497 op=2 display+exit]
  1: He rustles through his papers.
  2: "A fine one indeed."
  3: "If you wish to live there it will cost thee $V1 gold."
  4: "Plus $V0 gold per day, as measured on thy home world $G."
  5: "Art thou interested?"

[498 op=1 keyword; failure -> 500]
  1: look

[499 op=2 display+exit]
  1: a preoccupied man holding a ledger under one arm.
  2: "Records to keep, festivals to plan, I'm so busy... But I can spare you a moment, $G."

[500 op=1 keyword; failure -> 503]
  1: name

[502 op=2 display+exit]
  1: "I am Tholden von Bazillius, Chancellor to Lord @British."
  2: "In my younger days, I was known as 'The @Werecat of the Wine Cellar'..."

[503 op=1 keyword; failure -> 505]
  1: were,cat,wine,cell,youn

[504 op=2 display+exit]
  1: "I could tell you stories... But there's no time for that now."

[505 op=1 keyword; failure -> 507]
  1: lord,brit

[506 op=2 display+exit]
  1: "A fine ruler indeed. 'Tis a pleasure to keep his kingdom running smoothly for him."

[507 op=1 keyword; failure -> 509]
  1: job

[508 op=2 display+exit]
  1: "All the little details of governing, all the paperwork falls on my shoulders."
  2: "Recently I've received responsibility for @housing and @payments too."
  3: "Someone must do it, and yet I miss the days when I was a carefree adventurer."

[509 op=1 keyword; failure -> 511]
  1: rune,mant,comp

[510 op=2 display+exit]
  1: "The rune and mantra of Compassion are in the keeping of the bards at the @Conservatory."
  2: "I entrusted them with the rune due to their compassionate nature."
  3: "As for the other runes - they are no longer together, as they were when the @Codex was recovered."
  4: "Since then, they have become scattered throughout the land. Each was sent to a town near its shrine."
  5: "If thou dost ask the lords of each of these towns, they should be able to tell thee more."
  6: "The towns are Moonglow, Jhelom, Yew, Minoc, Trinsic, Skara Brae, and New Magincia."

[511 op=1 keyword; failure -> 513]
  1: code

[512 op=2 display+exit]
  1: "'Tis the greatest treasure in all the realm."

[513 op=1 keyword; failure -> 515]
  1: cons

[514 op=2 display+exit]
  1: "'Twas founded by Lord British."

[515 op=1 keyword; failure -> 518]
  1: bye

[517 op=2 display+exit]
  1: Tholden goes down on one knee and kisses your hand.
  2: "I hope I have been of assistance, $G."

[518 op=1 keyword; failure -> 520]
  1: bye

[519 op=2 display+exit]
  1: Tholden bows deeply.
  2: "I hope I have been of assistance, $G."

[520 op=1 keyword; failure -> 522]
  1: sell,leave

[522 op=1 keyword; failure -> 526]
  1: buy,house

[525 op=2 display+exit]
  1: "Remember to make all @payments in advance, otherwise you may be evicted."
  2: "Your @balance is currently $V0 gold."
  3: "Should you @sell your house your account balance will be refunded."
  4: "Only things kept on the storage shelf will be safely stored when you leave Britannia."

[526 op=1 keyword; failure -> 529]
  1: buy,hous

[528 op=2 display+exit]
  1: "Which house number were you interested in?"

[529 op=1 keyword; failure -> 531]
  1: ques

[530 op=2 display+exit]
  1: "House furnishings do not belong to you, do not take them."
  2: "Lock anything you wish to keep only on the shelf in your safe room."
  3: "Make sure you make @payments in advance, otherwise you will be evicted."
  4: "All @payments must be given directly to me, you may ask about your account @balance anytime."

[532 op=1 keyword; failure -> 536]
  1: paym

[535 op=2 display+exit]
  1: "Which house number were you interested in?"

[536 op=1 keyword; failure -> 539]
  1: paym

[538 op=2 display+exit]
  1: "Your house account currently has $V0 gold paid in advance."
  2: "Do you wish to add more?"

[539 op=1 keyword; failure -> 542]
  1: bala

[541 op=2 display+exit]
  1: "Your account @balance is currently $V0 gold, remember to keep up your @payments."

[542 op=2 display+exit]
  1: "I cannot help thee with that."

[550 op=2 display+exit]
  1: Tholden slowly shakes his head.
  2: "'Tis not enough gold $G, even for the first 3 days."

[553 op=2 display+exit]
  1: He rustles through his papers.
  2: "You must be mistaken, I'd know if that house was available."

[555 op=2 display+exit]
  1: "Lord British only allows me to sell one house to each person $G."

[557 op=2 display+exit]
  1: "Perhaps you should spend more time exploring Britannia before deciding where to live $G."

[561 op=2 display+exit]
  1: Tholden smiles.
  2: "Here is the key $G, I feel much safer with you living here in Britain."
  3: "If you have any @questions let me know, and don't forget your @payments!"
  4: "Your @balance of $V0 has only paid for the first 3 days."

[565 op=2 display+exit]
  1: "Try buying a house first!"

[568 op=2 display+exit]
  1: Tholden smiles.
  2: "Perhaps that much money would be better spent against fighting the gargoyals."

[570 op=2 display+exit]
  1: "Just remember to make @payments in advance, otherwise you will be evicted."

[574 op=2 display+exit]
  1: Tholden smiles.
  2: "Perhaps that much money would be better spent against fighting the gargoyals."

[578 op=2 display+exit]
  1: Tholden smiles.
  2: "'Tis easier when you have that much gold to give."

[581 op=2 display+exit]
  1: Tholden accepts the gold.
  2: "That's a total of $V0 gold paid in advance."

[584 op=2 display+exit]
  1: "'Tis hard to leave a house when you don't own one $G."

[586 op=2 display+exit]
  1: "I can refund your account balance."
  2: "When you next return to Britannia your things will be stored into chests."
  3: "Your key shall be collected then."
  4: "Are you certain you wish to leave this house $G?"

[589 op=1 keyword; failure -> 592]
  1: yes

[592 op=2 display+exit]
  1: Tholden slowly smiles.
  2: "'Tis well indeed, I feel much safer with you here $G."

[595 op=2 display+exit]
  1: Tholden smiles.
  2: "Thy pack is too heavy to carry the gold I'd return to thee!"

[597 op=2 display+exit]
  1: Tholden returns your gold.
  2: "'Tis been a pleasure to serve thee $P."

```

<a id="tiberius-npc-24"></a>
## Tiberius ? NPC 24, port 25

Instruction range: 2030?2149

```text
[2030 op=1 keyword; failure -> 2032]
  1: look

[2031 op=2 display+exit]
  1: a rotund man in monk's robes.
  2: "Greetings, my children. What can I do for thee today?"

[2032 op=1 keyword; failure -> 2035]
  1: name

[2034 op=2 display+exit]
  1: "I am Tiberius, healer for this town."

[2035 op=1 keyword; failure -> 2037]
  1: job

[2036 op=2 display+exit]
  1: "It is my responsibility to care for the sick and injured."
  2: "I attempt to @heal, @cure, or if needed, @resurrect the fallen."
  3: "I also offer @insurance to those expecting to need my services in the future."
  4: "What can I do for thee?"

[2037 op=1 keyword; failure -> 2039]
  1: bye

[2038 op=2 display+exit]
  1: "Go in peace, my children."

[2046 op=2 display+exit]
  1: "Wilt thou make an offering of $V0 gold?"

[2050 op=1 keyword; failure -> 2053]
  1: heal

[2052 op=2 display+exit]
  1: "Which of you?"

[2055 op=2 display+exit]
  1: "I beg thy pardon?"

[2057 op=1 keyword; failure -> 2059]
  1: yes

[2061 op=2 display+exit]
  1: "I'm sorry."
  2: "Without an offering I cannot heal thee."

[2062 op=1 keyword; failure -> 2065]
  1: cure

[2064 op=2 display+exit]
  1: "Wilt thou make an offering of 10 gold?"

[2066 op=1 keyword; failure -> 2069]
  1: yes

[2068 op=2 display+exit]
  1: "Which of you?"

[2071 op=2 display+exit]
  1: "I'm sorry."
  2: "Without an offering I cannot cure thee."

[2072 op=1 keyword; failure -> 2074]
  1: resu

[2075 op=1 keyword; failure -> 2077]
  1: yes

[2079 op=2 display+exit]
  1: "Then there is naught I can do save grieve with thee."

[2080 op=1 keyword; failure -> 2083]
  1: insu

[2082 op=2 display+exit]
  1: "Fret not, for thy body will now be in safe hands should the unthinkable befall."

[2083 op=1 keyword; failure -> 2090]
  1: insu

[2089 op=2 display+exit]
  1: "By giving in advance, gold will not be an issue should you fall in battle."
  2: "Wilt thou make an offering of $V0 gold?"

[2091 op=1 keyword; failure -> 2101]
  1: yes

[2100 op=2 display+exit]
  1: You give him the gold.
  2: "Fret not, for thy body will now be in safe hands should the unthinkable befall."

[2102 op=1 keyword; failure -> 2105]
  1: yes

[2104 op=2 display+exit]
  1: "If thou wilt not pay, I cannot offer my services."

[2107 op=2 display+exit]
  1: "Fret not, perhaps thy quest will prove more fortunate than thy friends."
  2: "Is there aught else I can do for thee?"

[2108 op=2 display+exit]
  1: "I beg thy pardon?"

[2113 op=2 display+exit]
  1: "Wilt thou make an offering of $V0 gold?"

[2117 op=2 display+exit]
  1: "I beg thy pardon?"

[2123 op=2 display+exit]
  1: "If thou wilt not pay, I cannot heal thee."

[2126 op=2 display+exit]
  1: "Fret not, for thou art well."
  2: "Is there aught else I can do for thee?"

[2128 op=2 display+exit]
  1: Laying hands upon $PARTYMEMBER, Tiberius mends the wounds.
  2: "Is there aught else I can do for thee?"

[2132 op=2 display+exit]
  1: "If thou wilt not pay, I cannot cure thee."

[2135 op=2 display+exit]
  1: "Fret not, for thou art well."
  2: "Is there aught else I can do for thee?"

[2137 op=2 display+exit]
  1: "I sense that thou art poisoned, $PARTYMEMBER."
  2: Laying hands upon $PARTYMEMBER, Tiberius removes the poison.
  3: "Is there aught else I can do for thee?"

[2142 op=2 display+exit]
  1: "That ain't enough money."
  2: "Go see a gravedigger."
  3: "I'll reckon his price'll be lower."

[2145 op=2 display+exit]
  1: "But thou weepest for no reason!"
  2: "None of thy friends are dead!"
  3: "Is there aught else I can do for thee?"

[2147 op=2 display+exit]
  1: "I see that thou carries another slain friend."
  2: Your party takes up a collection for their fallen comrade.
  3: Tiberius lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: "I regret there is naught I can do save grieve with thee."

[2149 op=2 display+exit]
  1: "I see that thou carries another slain friend."
  2: Your party takes up a collection for their fallen comrade.
  3: Tiberius lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: And the dead live again!

```

<a id="timothy-npc-107"></a>
## Timothy ? NPC 107, port 108

Instruction range: 9951?9987

```text
[9951 op=1 keyword; failure -> 9955]
  1: look

[9954 op=2 display+exit]
  1: a man with sea-blue eyes and a warm smile.
  2: "Welcome, welcome, friend."
  3: "Here for @lodging this fine $T?"

[9955 op=1 keyword; failure -> 9957]
  1: look

[9956 op=2 display+exit]
  1: a man with sea-blue eyes and a warm smile.
  2: "Welcome back, my friend."
  3: "Here for @lodging this fine $T?"

[9957 op=1 keyword; failure -> 9959]
  1: name

[9958 op=2 display+exit]
  1: "@Timothy."
  2: He flashes a friendly smile and nods.

[9959 op=1 keyword; failure -> 9961]
  1: timo

[9960 op=2 display+exit]
  1: "I'm named after my @uncle. He was a kind soul."

[9961 op=1 keyword; failure -> 9963]
  1: uncl

[9962 op=2 display+exit]
  1: "Sir Timothy Enders Daverstock. He was a famous @knight in these parts."

[9963 op=1 keyword; failure -> 9965]
  1: job

[9964 op=2 display+exit]
  1: "It is a busy one - this job, that is. I find it rewarding, though, meeting friendly faces."
  2: "Maybe you want a @room, eh?"

[9965 op=1 keyword; failure -> 9968]
  1: yes,room,inn,lodg

[9967 op=2 display+exit]
  1: "5 gold for you to stay and have a meal, good?"

[9969 op=1 keyword; failure -> 9971]
  1: yes

[9972 op=1 keyword; failure -> 9974]
  1: no

[9973 op=2 display+exit]
  1: "Perhaps some other time, then."

[9975 op=1 keyword; failure -> 9977]
  1: famo,knig

[9976 op=2 display+exit]
  1: "He is perhaps best known for his courage in @battle."

[9977 op=1 keyword; failure -> 9979]
  1: cour,batt

[9978 op=2 display+exit]
  1: "He once held off a score of men while his own fled to safety across a bridge."

[9979 op=1 keyword; failure -> 9981]
  1: bye

[9980 op=2 display+exit]
  1: "Farewell, friend."
  2: "Travel safely."

[9981 op=2 display+exit]
  1: "Hmm... Nope, can't help you there."

[9985 op=2 display+exit]
  1: "I'm sorry, but we require cash in advance for our rooms."

[9987 op=2 display+exit]
  1: "Goodnight!"

```

<a id="tobatha-npc-79"></a>
## Tobatha ? NPC 79, port 80

Instruction range: 7511?7596

```text
[7511 op=1 keyword; failure -> 7513]
  1: look

[7512 op=2 display+exit]
  1: a doddering old woman.
  2: "Who are you and what d'ye want?"

[7515 op=2 display+exit]
  1: "Eh? Speak up!"

[7516 op=1 keyword; failure -> 7518]
  1: name

[7517 op=2 display+exit]
  1: "My name's @Tobatha."
  2: "But you can call me @maam, youngster."

[7518 op=1 keyword; failure -> 7520]
  1: toba

[7519 op=2 display+exit]
  1: "That's @maam to you, youngster."
  2: "Respect your elders!"

[7520 op=1 keyword; failure -> 7522]
  1: maam

[7521 op=2 display+exit]
  1: "That's better. Now, what did you want?"

[7522 op=1 keyword; failure -> 7524]
  1: job

[7523 op=2 display+exit]
  1: "Job? I'm a mender of the @inflicted!"
  2: "If I weren't, why would you be here?"
  3: "Now stop this foolishness and tell me what you want, youngster."

[7524 op=1 keyword; failure -> 7526]
  1: infl

[7525 op=2 display+exit]
  1: "I can @heal, @cure, and even @raise dead!"

[7526 op=1 keyword; failure -> 7528]
  1: than

[7527 op=2 display+exit]
  1: "Oh, sure! Thou art just trying to get on my good side."
  2: "Well, it won't work!"

[7534 op=1 keyword; failure -> 7537]
  1: heal

[7536 op=2 display+exit]
  1: "Wilt thou make an offering of 30 gold?"

[7538 op=1 keyword; failure -> 7541]
  1: yes

[7540 op=2 display+exit]
  1: "Which of you?"

[7543 op=2 display+exit]
  1: "Well, if thou wilt not pay, why should I heal thee?"
  2: "You youngsters think everything should be free!"

[7544 op=1 keyword; failure -> 7547]
  1: cure

[7546 op=2 display+exit]
  1: "Wilt thou make an offering of 10 gold?"

[7548 op=1 keyword; failure -> 7551]
  1: yes

[7550 op=2 display+exit]
  1: "Which of you?"

[7553 op=2 display+exit]
  1: "Well if thou won't pay, thou won't get cured!"
  2: "You youngsters think everything should be free!"

[7554 op=1 keyword; failure -> 7557]
  1: rais,resu

[7556 op=2 display+exit]
  1: "Well, ain't that just like youngsters?"
  2: "Makin' mistakes and expecting their elders to bail them out."
  3: "Well, today's your lucky day."
  4: "Wilt thou make an offering of 400 gold?"

[7558 op=1 keyword; failure -> 7560]
  1: yes

[7562 op=2 display+exit]
  1: "Then go see a gravedigger. I'll reckon his price'll be lower."

[7563 op=1 keyword; failure -> 7565]
  1: bye,no

[7564 op=2 display+exit]
  1: "Hmph! And not so much as a @thank-you."
  2: "Well, that's kids today for ye! No manners, the lot of 'em!"

[7565 op=1 keyword; failure -> 7567]
  1: yes

[7566 op=2 display+exit]
  1: "Well, what can I do for thee?"

[7567 op=2 display+exit]
  1: "Don't bother me with that!"

[7571 op=2 display+exit]
  1: "Well, if thou wilt not pay, why should I heal thee?"
  2: "You youngsters think everything should be free!"

[7574 op=2 display+exit]
  1: "What do you mean, heal? Thou art not wounded!"

[7576 op=2 display+exit]
  1: "Aye, I see thy wound, $PARTYMEMBER."
  2: Laying hands upon $PARTYMEMBER, Tobatha mends the wounds.
  3: "What else ya want?"

[7580 op=2 display+exit]
  1: "Well if thou won't pay, thou won't get cured!"
  2: "You youngsters think everything should be free!"

[7583 op=2 display+exit]
  1: "What do you mean, cure? Thou art not poisoned!"

[7585 op=2 display+exit]
  1: "Aye, $PARTYMEMBER, I can tell that thou art poisoned."
  2: Laying hands upon $PARTYMEMBER, Tobatha removes the poison.
  3: "What else ya want?"

[7589 op=2 display+exit]
  1: "That ain't enough money."
  2: "Go see a gravedigger. I'll reckon his price'll be lower."

[7592 op=2 display+exit]
  1: "Eh? What foolishness is this?"
  2: "There ain't no dead person here!"

[7594 op=2 display+exit]
  1: "I see that you've got another one."
  2: Your party takes up a collection for their fallen comrade.
  3: Tobatha lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: "Today's not your lucky day."

[7596 op=2 display+exit]
  1: "I see that you've got another one."
  2: Your party takes up a collection for their fallen comrade.
  3: Tobatha lays hands upon the corpse...
  4: "Doman..."
  5: "thixus..."
  6: "anretu!"
  7: And the dead live again!

```

<a id="trebor-npc-70"></a>
## Trebor ? NPC 70, port 71

Instruction range: 6746?6782

```text
[6747 op=1 keyword; failure -> 6749]
  1: yes

[6751 op=2 display+exit]
  1: "You'll ne'er find a better craft."

[6752 op=1 keyword; failure -> 6756]
  1: look

[6755 op=2 display+exit]
  1: a tall, muscled man with a wide grin.
  2: "Hello," he says, lumbering over to you and extending a large hand.
  3: "What can I do for ya?"

[6756 op=1 keyword; failure -> 6758]
  1: look

[6757 op=2 display+exit]
  1: a tall, muscled man with a wide grin.
  2: He squints at you.
  3: "We've met, haven't we?"
  4: "What can I do for ya?"

[6758 op=1 keyword; failure -> 6760]
  1: name

[6759 op=2 display+exit]
  1: "Trebor."

[6760 op=1 keyword; failure -> 6762]
  1: job,buy

[6761 op=2 display+exit]
  1: "I sell @ships and @skiffs."

[6762 op=1 keyword; failure -> 6765]
  1: ship

[6764 op=2 display+exit]
  1: "It'll cost you 250 gold for the deed."
  2: "Interested?"

[6765 op=1 keyword; failure -> 6768]
  1: skif

[6767 op=2 display+exit]
  1: "It'll cost you 20 gold for the deed."
  2: "Interested?"

[6768 op=1 keyword; failure -> 6770]
  1: bye

[6769 op=2 display+exit]
  1: "So long friend."
  2: He lumbers back to his work.

[6770 op=2 display+exit]
  1: "That's a puzzler."

[6779 op=2 display+exit]
  1: "Thou hast not enough gold."

[6781 op=2 display+exit]
  1: "You look pretty full to me, $Y."

[6782 op=2 display+exit]
  1: Your party takes up a collection to purchase the vessel.
  2: "Excellent!"
  3: After accepting the gold, he hands you the deed.
  4: "Sail her straight."

```

<a id="trenton-npc-80"></a>
## Trenton ? NPC 80, port 81

Instruction range: 7597?7668

```text
[7597 op=1 keyword; failure -> 7600]
  1: look

[7599 op=2 display+exit]
  1: a tall, rather well dressed man with a wilted smile.
  2: Recognizing you, his smile flickers a moment.
  3: "You have come back."
  4: "Well?"

[7600 op=1 keyword; failure -> 7603]
  1: look

[7602 op=2 display+exit]
  1: a tall, rather well dressed man with a wilted smile.
  2: "Yes?" he asks, reaching over and removing something invisible from your shoulder.

[7603 op=1 keyword; failure -> 7606]
  1: name

[7605 op=2 display+exit]
  1: "You've forgotten?"
  2: "Well, 'Your @honor' will do."

[7606 op=1 keyword; failure -> 7609]
  1: name

[7608 op=2 display+exit]
  1: He sniffs the air as if smelling something bad.
  2: "I am, of course, @Trenton @Bell, the mayor."
  3: "You may call me 'Your Honor'."

[7609 op=1 keyword; failure -> 7611]
  1: tren,bell

[7610 op=2 display+exit]
  1: "I would appreciate if you would use my @title, 'Your @Honor'."

[7611 op=1 keyword; failure -> 7613]
  1: titl

[7612 op=2 display+exit]
  1: "Yes, I am mayor."

[7613 op=1 keyword; failure -> 7615]
  1: your,hono,mayo

[7614 op=2 display+exit]
  1: "Yes?"

[7615 op=1 keyword; failure -> 7617]
  1: humb,floc,alar,pier,cave

[7616 op=2 display+exit]
  1: "Yes."

[7617 op=1 keyword; failure -> 7619]
  1: mant

[7618 op=2 display+exit]
  1: "Well, surely thou canst @read, yes?"

[7619 op=1 keyword; failure -> 7621]
  1: read

[7620 op=2 display+exit]
  1: "Yes, reading books is good for your spiritual side."

[7621 op=1 keyword; failure -> 7623]
  1: job

[7622 op=2 display+exit]
  1: "Job?"
  2: "I do not @labor like others."

[7623 op=1 keyword; failure -> 7625]
  1: labo,othe,sail

[7624 op=2 display+exit]
  1: "You know: @rigging the masts or swabbing the decks."

[7625 op=1 keyword; failure -> 7627]
  1: mast,rig,swab,deck

[7626 op=2 display+exit]
  1: "You aren't much of a sailor are you?"
  2: "@Yorl and that young @Delancy girl can tell you about that."

[7627 op=1 keyword; failure -> 7629]
  1: yorl

[7628 op=2 display+exit]
  1: "He works at the pier with the @Delancy girl."

[7629 op=1 keyword; failure -> 7631]
  1: dela,marn

[7630 op=2 display+exit]
  1: "She has run the pier ever since her father, @Quenton, was attacked by a @gargoyle."
  2: He coughs then clears his throat.

[7631 op=1 keyword; failure -> 7633]
  1: rune

[7632 op=2 display+exit]
  1: "@Quenton, not I, was in charge of its safe keeping."
  2: "Just as I suspected, when he died, so too did our chances of finding the rune."
  3: "You see, he never told anyone where it was located."

[7633 op=1 keyword; failure -> 7635]
  1: shri,moon,gate

[7634 op=2 display+exit]
  1: "Speak to @Horance of that."
  2: "He may be mad, but he is still quick and astute in the ways of magic."

[7635 op=1 keyword; failure -> 7637]
  1: quen,atta

[7636 op=2 display+exit]
  1: "We were not friends...not good friends, that is."
  2: "He resented my @position."

[7637 op=1 keyword; failure -> 7639]
  1: posi,rese

[7638 op=2 display+exit]
  1: "Yes, well, as mayor, I am the shepherd of the @spiritual people of this hamlet."

[7639 op=1 keyword; failure -> 7641]
  1: shep,spir,peop,haml

[7640 op=2 display+exit]
  1: "We do not believe in violence as a solution to our problems."
  2: "My flock would not fight against anyone."
  3: "Therefore, I'm alarmed at the recent @gargoyle sightings."

[7641 op=1 keyword; failure -> 7643]
  1: garg

[7642 op=2 display+exit]
  1: "Huge fierce creatures, they are."
  2: "Quenton didn't even have time to @scream before one of those beasts had killed him."

[7643 op=1 keyword; failure -> 7645]
  1: scre

[7644 op=2 display+exit]
  1: "Many believe if he had screamed, someone in the @inn would have heard him."

[7645 op=1 keyword; failure -> 7647]
  1: inn,some,hear

[7646 op=2 display+exit]
  1: "A couple of @travelers were staying there that night, but they said they heard nothing."
  2: "Still, I find this @strange."

[7647 op=1 keyword; failure -> 7649]
  1: coup,trav

[7648 op=2 display+exit]
  1: "I do not know their names."
  2: "'Twas @Gideon questioned them, not I."

[7649 op=1 keyword; failure -> 7651]
  1: gide

[7650 op=2 display+exit]
  1: "He runs the @Haunting Inn."

[7651 op=1 keyword; failure -> 7653]
  1: haun

[7652 op=2 display+exit]
  1: "The one the @travelers were at."
  2: "It's across the way."
  3: He frowns at you.

[7653 op=1 keyword; failure -> 7655]
  1: stra

[7654 op=2 display+exit]
  1: "Yes, you see, my brother is a guard for Lord British."
  2: "A few months ago, a gargoyle attempted to enter the castle proper."
  3: "He said that although they killed the beast, it's horrifying noises still give him @nightmares."

[7655 op=1 keyword; failure -> 7657]
  1: nigh,nois,beas

[7656 op=2 display+exit]
  1: "He keeps hearing its deep growl."
  2: "I'm sure that even if Quenton couldn't scream, someone would have heard that growl."

[7657 op=1 keyword; failure -> 7659]
  1: deza

[7658 op=2 display+exit]
  1: "She is a peaceful woman."
  2: "Very pleasant to talk to."

[7659 op=1 keyword; failure -> 7661]
  1: hora

[7660 op=2 display+exit]
  1: "For the life of me I've never understood a word he's ever said."
  2: "They say he drank one too many potions."

[7661 op=1 keyword; failure -> 7663]
  1: mich

[7662 op=2 display+exit]
  1: "He is a very private sort."
  2: "I've never been to his abode."
  3: "He lives east of here, along the road to Britain."

[7663 op=1 keyword; failure -> 7665]
  1: bye

[7664 op=2 display+exit]
  1: "Goodbye."

[7667 op=2 display+exit]
  1: "I fail to see how that relates to the matter at hand..."

[7668 op=2 display+exit]
  1: "You will have to ask another of that."

```

<a id="troy-npc-71"></a>
## Troy ? NPC 71, port 72

Instruction range: 6783?6872

```text
[6783 op=1 keyword; failure -> 6785]
  1: look

[6784 op=2 display+exit]
  1: a spidery looking man with delicate hands.
  2: "Hello."

[6785 op=1 keyword; failure -> 6788]
  1: name

[6787 op=2 display+exit]
  1: "My name is Troy."

[6788 op=1 keyword; failure -> 6790]
  1: job

[6789 op=2 display+exit]
  1: "I make @clocks."
  2: "The movements of the various parts are very complex and interconnected."
  3: "Like the @moons and stars, or the pieces on a @chess board."

[6790 op=1 keyword; failure -> 6792]
  1: cloc,move,part,compl,inte

[6791 op=2 display+exit]
  1: "The @ticking of clocks is the pulse of @civilization."

[6792 op=1 keyword; failure -> 6794]
  1: puls,civi

[6793 op=2 display+exit]
  1: "Without clocks, people would just sit around uselessly, with no idea what to do next!"

[6794 op=1 keyword; failure -> 6796]
  1: tick,soun

[6795 op=2 display+exit]
  1: "Of course, the ticks would be useless without the @tocks."

[6796 op=1 keyword; failure -> 6798]
  1: tock

[6797 op=2 display+exit]
  1: "They're much deeper sounding and more satisfying than ticks."

[6798 op=1 keyword; failure -> 6800]
  1: moon,star

[6799 op=2 display+exit]
  1: "@Ephemerides is going to build a model of the whole system."
  2: "I helped him design the @gearwork that makes it go."

[6800 op=1 keyword; failure -> 6802]
  1: mode,syst,desi,gear

[6801 op=2 display+exit]
  1: "I know all about gears. Some of them can be quite @small."

[6816 op=1 keyword; failure -> 6818]
  1: smal,pock

[6817 op=2 display+exit]
  1: "I want to create a smaller clock that one could carry."
  2: "But, I need some @help making one."

[6818 op=1 keyword; failure -> 6831]
  1: help

[6830 op=2 display+exit]
  1: "Ahh. Excellent. I shall get to work."
  2: Taking the materials from you, he slowly moves about the shop, spending long periods of time gazing at each part as he fashions it.
  3: Soon, you find yourself dozing off...
  4: With a slight jolt, you wake up as Troy taps your shoulder. "My work is done."
  5: Troy hands you the small clock.
  6: "It still needs a @glass @crystal. You should go see Dale the glassblower who lives just next door."
  7: "When you are done come back here and I will finish the clock."

[6831 op=1 keyword; failure -> 6835]
  1: help

[6834 op=2 display+exit]
  1: "I require a new special set of @pliers and some @horseshoes for metal."

[6835 op=1 keyword; failure -> 6839]
  1: plie

[6838 op=2 display+exit]
  1: "I commission my tools from the blacksmith in Serpent's Hold."
  2: "Though, they do tend to break easily."

[6839 op=1 keyword; failure -> 6843]
  1: hors

[6842 op=2 display+exit]
  1: "They provide me with metal suitable for smelting."
  2: "Try looking in Trinsic for some."

[6843 op=1 keyword; failure -> 6850]
  1: help

[6849 op=2 display+exit]
  1: Troy quickly finishes making the final adjustments to the clock and hands it over to you.
  2: "I call it a 'pocket watch'. Please, try not to lose it."
  3: "Now, if you will excuse me..."

[6850 op=1 keyword; failure -> 6854]
  1: help

[6853 op=2 display+exit]
  1: "First I need a frame for the clock. Take this mold to Utomo in yew and he will cast you a golden @clock frame.

[6854 op=1 keyword; failure -> 6858]
  1: help

[6857 op=2 display+exit]
  1: "First I need a frame for the clock. Take this mold to Utomo in Yew and he will cast you a golden @clock frame.

[6858 op=1 keyword; failure -> 6860]
  1: pock,small

[6859 op=2 display+exit]
  1: "The pocket watch is my finest work."
  2: "Please, try not to lose it. "

[6860 op=1 keyword; failure -> 6862]
  1: ephe

[6861 op=2 display+exit]
  1: "He lives at the @Lycaeum."

[6862 op=1 keyword; failure -> 6864]
  1: lyca

[6863 op=2 display+exit]
  1: "It's a great center of @learning."

[6864 op=1 keyword; failure -> 6866]
  1: lear

[6865 op=2 display+exit]
  1: "If you didn't run around asking foolish questions all the time,
  2: you might learn something yourself!"

[6866 op=1 keyword; failure -> 6868]
  1: piec,ches,boar

[6867 op=2 display+exit]
  1: "I like to play chess."
  2: "Keeps ones @mind fit."

[6868 op=1 keyword; failure -> 6870]
  1: mind

[6869 op=2 display+exit]
  1: "Some people don't use theirs - but I do."

[6870 op=1 keyword; failure -> 6872]
  1: bye

[6871 op=2 display+exit]
  1: "Until we come into conjunction again."

[6872 op=2 display+exit]
  1: "The sound of the clocks is so soothing..."
  2: "Excuse me, did you say something?"

```

<a id="ubermon-npc-106"></a>
## Ubermon ? NPC 106, port 107

Instruction range: 9893?9950

```text
[9894 op=1 keyword; failure -> 9896]
  1: yes

[9897 op=1 keyword; failure -> 9899]
  1: yes

[9901 op=2 display+exit]
  1: "Vell, okay, but is very good for you!"

[9904 op=2 display+exit]
  1: "Vell, okay, but is very good for you!"

[9905 op=1 keyword; failure -> 9908]
  1: look

[9907 op=2 display+exit]
  1: a tall man with a disarming grin.
  2: "You are back."
  3: "Gut."
  4: He leans back on his heels.

[9908 op=1 keyword; failure -> 9911]
  1: look

[9910 op=2 display+exit]
  1: a tall man with a disarming grin.
  2: "Hello, mein freund."
  3: His thick fingers grab your hand.

[9911 op=1 keyword; failure -> 9913]
  1: name

[9912 op=2 display+exit]
  1: "Ubermon @Kalbmilch."

[9913 op=1 keyword; failure -> 9915]
  1: uber,kalb

[9914 op=2 display+exit]
  1: "At your zervice."
  2: He snaps to attention.

[9917 op=1 keyword; failure -> 9919]
  1: job

[9918 op=2 display+exit]
  1: "I @milk de cows und make de @cheese."
  2: "You vant to buy someting, just say de word."

[9919 op=1 keyword; failure -> 9921]
  1: buy,word

[9920 op=2 display+exit]
  1: "Ja, sure, vat you vant - milk or @cheese?"

[9921 op=1 keyword; failure -> 9924]
  1: milk

[9923 op=2 display+exit]
  1: "That'll be 3 crowns, okay?"

[9924 op=1 keyword; failure -> 9927]
  1: chee

[9926 op=2 display+exit]
  1: "That'll be 5 crowns, okay?"

[9927 op=1 keyword; failure -> 9929]
  1: job

[9928 op=2 display+exit]
  1: "I @milk de cows und make de @cheese."
  2: "You vant to buy someting, come by my dairy ven I'm open."

[9929 op=1 keyword; failure -> 9931]
  1: buy,word

[9930 op=2 display+exit]
  1: "Come by my dairy ven I'm open, ja?"

[9931 op=1 keyword; failure -> 9933]
  1: milk

[9932 op=2 display+exit]
  1: "Come by my dairy ven I'm open, ja?"

[9933 op=1 keyword; failure -> 9935]
  1: chee

[9934 op=2 display+exit]
  1: "Come by my dairy ven I'm open, ja?"

[9935 op=1 keyword; failure -> 9937]
  1: bye

[9936 op=2 display+exit]
  1: "Take care among ze British, now."

[9937 op=2 display+exit]
  1: "Now I not know vat you say."

[9945 op=2 display+exit]
  1: "You have not ze gold right now."

[9947 op=2 display+exit]
  1: "You don't have room to carry it."

[9949 op=2 display+exit]
  1: "Good!"
  2: He gives you the milk.

[9950 op=2 display+exit]
  1: "Good!"
  2: He gives you the cheese.

```

<a id="unconcious-man-npc-123"></a>
## unconcious man ? NPC 123, port 124

Instruction range: 11348?11350

```text
[11348 op=1 keyword; failure -> 11350]
  1: look

[11349 op=2 display+exit]
  1: an unconcious man, his face twisted with pain.
  2: He offers you no response.

[11350 op=2 display+exit]
  1: He offers you no response.

```

<a id="utomo-npc-55"></a>
## Utomo ? NPC 55, port 56

Instruction range: 4835?5172

```text
[4837 op=1 keyword; failure -> 4841]
  1: look

[4840 op=2 display+exit]
  1: a huge, dark-skined man with a close-set face.
  2: "@Ombogo @sano!" he says, smiling widely.

[4841 op=1 keyword; failure -> 4843]
  1: look

[4842 op=2 display+exit]
  1: a huge, dark-skined man with a close-set face.
  2: "@Ombogo @dono," he grunts at you from behind a large forge.
  3: "What else?"

[4843 op=1 keyword; failure -> 4845]
  1: ombo

[4844 op=2 display+exit]
  1: "That mean 'hello.'"

[4845 op=1 keyword; failure -> 4847]
  1: dono

[4846 op=2 display+exit]
  1: "That mean 'person I don't know.'"

[4847 op=1 keyword; failure -> 4849]
  1: sano

[4848 op=2 display+exit]
  1: "That mean 'friend.'"

[4849 op=1 keyword; failure -> 4854]
  1: name

[4853 op=2 display+exit]
  1: "Utomo, from @island."

[4854 op=1 keyword; failure -> 4856]
  1: name

[4855 op=2 display+exit]
  1: "Utomo remember you, how you not remember @Utomo?"

[4856 op=1 keyword; failure -> 4858]
  1: utom,isla

[4857 op=2 display+exit]
  1: "Utomo mean 'island' in my language. I also from island."
  2: "Bring magic @fan from island. This town new @home for Utomo now."

[4858 op=1 keyword; failure -> 4860]
  1: fan,magi

[4859 op=2 display+exit]
  1: "Lady on island make magic fans. They make big wind, blow ships all around."

[4860 op=1 keyword; failure -> 4862]
  1: job

[4861 op=2 display+exit]
  1: "Utomo make good @weapons. Make one for you if you like."

[4862 op=1 keyword; failure -> 4864]
  1: weap

[4863 op=2 display+exit]
  1: "Learn to make as boy. I live on island then."

[4864 op=1 keyword; failure -> 4866]
  1: from,home

[4865 op=2 display+exit]
  1: "I come here from @many days away. My home burned by @evil man."

[4866 op=1 keyword; failure -> 4868]
  1: many,days

[4867 op=2 display+exit]
  1: "I not sure where. Got ride on big @ship."

[4868 op=1 keyword; failure -> 4870]
  1: big,ship

[4869 op=2 display+exit]
  1: "I stow away when night come. After long time we land and I jump off."

[4870 op=1 keyword; failure -> 4872]
  1: burn,evil,man

[4871 op=2 display+exit]
  1: "He say to me, 'You man, you leave home and @kill good people with me.' I ran from him."

[4872 op=1 keyword; failure -> 4874]
  1: kill,good,peop

[4873 op=2 display+exit]
  1: "Like people in town. Like Lord British."

[4874 op=1 keyword; failure -> 4876]
  1: Andr

[4875 op=2 display+exit]
  1: "Utomo go and drink at her home."
  2: She try @kiss Utomo. Utomo say, 'I no want you' but she kiss anyway."
  3: "Utomo not go back there."

[4876 op=1 keyword; failure -> 4878]
  1: kiss

[4877 op=2 display+exit]
  1: "Utomo have @girl back home."
  2: "She not thin like Andrea."
  3: "Someday Utomo go back and get her."
  4: "Someday."
  5: He plays the poker over the hot coals and looks absently into the fire.

[4878 op=1 keyword; failure -> 4880]
  1: girl,back

[4879 op=2 display+exit]
  1: "Her name Yuna. She good tree climber. I not see her for long time."

[4880 op=1 keyword; failure -> 4882]
  1: omdu,yaf

[4881 op=2 display+exit]
  1: "In my land it mean 'You have good meal in sun.'"
  2: "It is goodbye in my land."

[4884 op=1 keyword; failure -> 4889]
  1: cloc

[4888 op=2 display+exit]
  1: "Utomo make clock frame for gold nugget and 200 gold."
  2: "Okay, $Y?."

[4889 op=1 keyword; failure -> 4891]
  1: buy

[4890 op=2 display+exit]
  1: "I have many things here. You want @arms or @armor?"

[4891 op=1 keyword; failure -> 4893]
  1: sell

[4893 op=1 keyword; failure -> 4895]
  1: bulk

[4897 op=1 keyword; failure -> 4900]
  1: club

[4899 op=2 display+exit]
  1: "Sell club for 10 gold."
  2: "How many is okay, $Y?"

[4900 op=1 keyword; failure -> 4903]
  1: dagg

[4902 op=2 display+exit]
  1: "Sell dagger for 10 gold."
  2: "How many is okay, $Y?"

[4903 op=1 keyword; failure -> 4906]
  1: spea

[4905 op=2 display+exit]
  1: "Sell spear for 12 gold."
  2: "How many is okay, $Y?"

[4906 op=1 keyword; failure -> 4909]
  1: thro

[4908 op=2 display+exit]
  1: "Sell throwing axe for 12 gold."
  2: "How many is okay, $Y?"

[4909 op=1 keyword; failure -> 4912]
  1: 2han

[4911 op=2 display+exit]
  1: "Sell 2 handed axe for 45 gold."
  2: "How many is okay, $Y?"

[4912 op=1 keyword; failure -> 4915]
  1: bras

[4914 op=2 display+exit]
  1: "Sell brass helm for 20 gold."
  2:  "How many is okay, $Y?"

[4915 op=1 keyword; failure -> 4918]
  1: leathera

[4917 op=2 display+exit]
  1: "Sell leather armour for 35 gold."
  2: "How many is okay, $Y?"

[4918 op=1 keyword; failure -> 4921]
  1: leatherh

[4920 op=2 display+exit]
  1: "Sell leather helm for 7 gold."
  2: "How many is okay, $Y?"

[4921 op=1 keyword; failure -> 4924]
  1: ring

[4923 op=2 display+exit]
  1: "Sell ring mail for 40 gold."
  2: "How many is okay, $Y?"

[4924 op=1 keyword; failure -> 4927]
  1: swam

[4926 op=2 display+exit]
  1: "Sell swamp boots for 10 gold."
  2: "How many is okay, $Y?"

[4927 op=1 keyword; failure -> 4929]
  1: arms

[4928 op=2 display+exit]
  1: "Which item, @club, @dagger, @spear, @throwing @axe or @2 @handed @axe?"

[4929 op=1 keyword; failure -> 4931]
  1: armo

[4930 op=2 display+exit]
  1: "Which item, @brass @helm, @leather @armour, @leather @helm, @ring @mail or @swamp @boots?"

[4992 op=1 keyword; failure -> 4994]
  1: buy

[4993 op=2 display+exit]
  1: "Come to my shop when I'm open!"

[4994 op=1 keyword; failure -> 4996]
  1: arms

[4995 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[4996 op=1 keyword; failure -> 4998]
  1: armo

[4997 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[4998 op=1 keyword; failure -> 5000]
  1: sell

[4999 op=2 display+exit]
  1: "Come by my shop when I'm open!"

[5000 op=1 keyword; failure -> 5002]
  1: bye

[5001 op=2 display+exit]
  1: "@Omdu Yaf."

[5002 op=2 display+exit]
  1: "I speak not too much your language."

[5005 op=2 display+exit]
  1: "No sell!"

[5007 op=2 display+exit]
  1: "Too many."

[5011 op=2 display+exit]
  1: "No gold!"

[5013 op=2 display+exit]
  1: "You no have room"

[5014 op=2 display+exit]
  1: "Done!"

[5021 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 5 gold for that club, okay $Y?"

[5027 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 5 gold for that dagger, okay $Y?"

[5033 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 6 gold for that spear, okay $Y?"

[5039 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 6 gold for that throwing axe, okay $Y?"

[5045 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 22 gold for that 2 handed axe, okay $Y?"

[5051 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 10 gold for that brass helm, okay $Y?"

[5057 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 17 gold for that leather armour, okay $Y?"

[5063 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 3 gold for that leather helm, okay $Y?"

[5069 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 20 gold for that ring mail, okay $Y?"

[5075 op=2 display+exit]
  1: "You have something I want..."
  2: "I give 5 gold for those swamp boots, okay $Y?"

[5078 op=2 display+exit]
  1: "But you no have thing I want!"

[5081 op=1 keyword; failure -> 5085]
  1: yes

[5084 op=2 display+exit]
  1: "Good!"
  2: He hands you 5 gold pieces and takes the club.

[5086 op=1 keyword; failure -> 5090]
  1: yes

[5089 op=2 display+exit]
  1: "Good!"
  2: He hands you 5 gold pieces and takes the dagger.

[5091 op=1 keyword; failure -> 5095]
  1: yes

[5094 op=2 display+exit]
  1: "Good!"
  2: He hands you 6 gold pieces and takes the spear.

[5096 op=1 keyword; failure -> 5100]
  1: yes

[5099 op=2 display+exit]
  1: "Good!"
  2: He hands you 6 gold pieces and takes the throwing axe.

[5101 op=1 keyword; failure -> 5105]
  1: yes

[5104 op=2 display+exit]
  1: "Good!"
  2: He hands you 22 gold pieces and takes the 2 handed axe.

[5106 op=1 keyword; failure -> 5110]
  1: yes

[5109 op=2 display+exit]
  1: "Good!"
  2: He hands you 10 gold pieces and takes the brass helm.

[5111 op=1 keyword; failure -> 5115]
  1: yes

[5114 op=2 display+exit]
  1: "Good!"
  2: He hands you 17 gold pieces and takes the leather armour.

[5116 op=1 keyword; failure -> 5120]
  1: yes

[5119 op=2 display+exit]
  1: "Good!"
  2: He hands you 3 gold pieces and takes the leather helm.

[5121 op=1 keyword; failure -> 5125]
  1: yes

[5124 op=2 display+exit]
  1: "Good!"
  2: He hands you 20 gold pieces and takes the ring mail.

[5126 op=1 keyword; failure -> 5130]
  1: yes

[5129 op=2 display+exit]
  1: "Good!"
  2: He hands you 5 gold pieces and takes the swamp boots.

[5130 op=2 display+exit]
  1: "Maybe later."

[5132 op=1 keyword; failure -> 5140]
  1: yes

[5139 op=2 display+exit]
  1: He takes the gold nugget and starts working.
  2: After a while he turns to you and hands over the clock frame. "Here it done. Utomo go back to work now."

[5140 op=1 keyword; failure -> 5143]
  1: yes

[5142 op=2 display+exit]
  1: "That cost too much for you, $Y!"

[5143 op=1 keyword; failure -> 5145]
  1: yes

[5144 op=2 display+exit]
  1: "Utomo need gold nugget to cast gold clock frame."

[5145 op=2 display+exit]
  1: "Maybe something else?"

[5159 op=2 display+exit]
  1: "But you no have thing I want!"

[5162 op=2 display+exit]
  1: "Utomo like $Y."
  2: "You think big and not waste time."
  3: "I give $V0 gold for all your things, okay $Y?"

[5163 op=2 display+exit]
  1: "I give $V0 gold for all your things, okay $Y?"

[5165 op=1 keyword; failure -> 5167]
  1: yes

[5167 op=2 display+exit]
  1: "Maybe something else?"

[5170 op=2 display+exit]
  1: "You try to trick Utomo?"
  2: Utomo looks at you with wide eyes and his lower lip begins to quiver.

[5172 op=2 display+exit]
  1: "Good!"
  2: He hands you $V0 gold pieces in exchange for the items.

```

<a id="valkadesh-npc-167"></a>
## Valkadesh ? NPC 167, port 168

Instruction range: 15043?15117

```text
[15044 op=1 keyword; failure -> 15047]
  1: yes

[15046 op=2 display+exit]
  1: "To wish to hear about it!"

[15049 op=2 display+exit]
  1: "To regret that, and to wish to aid you."

[15057 op=2 display+exit]
  1: "Noble '$Z, to see that you are a being of honor and respect."
  2: "To not betray that honor - to return the @Codex!"
  3: "To save my people from the @prophecy!"

[15060 op=1 keyword; failure -> 15062]
  1: look

[15061 op=2 display+exit]
  1: The gargoyle smiles, but his grin is somehow disturbing.
  2: "I human @friend."
  3: "Walk talk you."

[15062 op=1 keyword; failure -> 15064]
  1: bye

[15063 op=2 display+exit]
  1: "Want much to talk."
  2: "@Captain John speak!"
  3: "@Captain John go!"

[15064 op=1 keyword; failure -> 15066]
  1: frie

[15065 op=2 display+exit]
  1: "@Captain John gargoyle friend."
  2: "Captain John speak."

[15066 op=1 keyword; failure -> 15068]
  1: capt,john

[15067 op=2 display+exit]
  1: "Must Captain John go."
  2: "Only he speak."
  3: "Must go!"

[15068 op=2 display+exit]
  1: In a thick accent, the gargoyle croaks, "I bad with human language."
  2: "With @Captain John must to speak."

[15070 op=1 keyword; failure -> 15073]
  1: look

[15072 op=2 display+exit]
  1: a tall, thin, winged gargoyle.
  2: "Greetings, friend '$P."
  3: "To see you have done the honorable thing."
  4: "To have earned much respect from my people by doing this!"
  5: "To hope your travels have brought you wisdom?"

[15073 op=1 keyword; failure -> 15076]
  1: look

[15075 op=2 display+exit]
  1: The gargoyle smiles, but his grin is somehow disturbing.
  2: "To offer greetings, False Prophet."
  3: "To have waited long for you to return to this side of the world."
  4: "To believe that the Book of Prophecies is wrong."
  5: "To believe that I can reason with you, persuade you to spare my people."
  6: "To wish to address you honorably."
  7: "To ask your name?"

[15078 op=2 display+exit]
  1: "To offer greetings, False Prophet."
  2: "To have waited long for you to return to this side of the world."
  3: "To believe that the Book of Prophecies is wrong."
  4: "To believe that I can reason with you, persuade you to spare my people."
  5: "To wish to address you honorably."
  6: "To ask your name?"

[15079 op=1 keyword; failure -> 15081]
  1: name

[15080 op=2 display+exit]
  1: "To be called Valkadesh Wis-lem, Valkadesh the Scholar."
  2: "To have been given a @partial name."
  3: "To have yet to achieve full honor."

[15081 op=1 keyword; failure -> 15083]
  1: job

[15082 op=2 display+exit]
  1: "To be a Scholar."
  2: "To be seeking a way to avert the @prophecy."

[15083 op=1 keyword; failure -> 15085]
  1: part,full,hono

[15084 op=2 display+exit]
  1: "To be greatly honored by even a partial name."
  2: "To be customary among my people to give names only to those most worthy."

[15085 op=1 keyword; failure -> 15087]
  1: prop

[15086 op=2 display+exit]
  1: "To be written in the Book of Prophecies that you would destroy our world."
  2: "To also be written that only the @sacrifice of the False Prophet would save us."

[15087 op=1 keyword; failure -> 15089]
  1: code

[15088 op=2 display+exit]
  1: "To believe that it is not too late."
  2: "To beg you to return the Codex."
  3: "Avert the @prophecy!"
  4: "Save your life, and my people's lives as well!"

[15089 op=1 keyword; failure -> 15091]
  1: sacr

[15090 op=2 display+exit]
  1: "To be the only alternative to returning the Codex."
  2: "Without the Codex, to have no @choice but to sacrifice your life!"

[15091 op=1 keyword; failure -> 15093]
  1: choi

[15092 op=2 display+exit]
  1: "Wait!"
  2: "To know that sacrifice has three meanings."
  3: "Of self, of others and of valuables."
  4: "Perhaps to find a better @answer in one of those other meanings."

[15093 op=1 keyword; failure -> 15095]
  1: find,answ

[15094 op=2 display+exit]
  1: "To be unable to help more."
  2: "To suggest you seek out @Naxatilor himself for further enlightenment."

[15095 op=1 keyword; failure -> 15097]
  1: naxa,seer

[15096 op=2 display+exit]
  1: "To learn much from Naxatilor, the wisest gargoyle."
  2: "To find his chambers just to the northeast of the Hall of Knowledge."
  3: "To suggest you go there and ask him of @sacrifice."

[15097 op=1 keyword; failure -> 15100]
  1: drax,lead,inqu

[15099 op=2 display+exit]
  1: "To go to Draxinusom, the Inquisitor and leader of my race."
  2: "To find his chambers just to the northwest of the Hall of Knowledge."
  3: "To @surrender to him, that you may travel @freely in our land."

[15100 op=1 keyword; failure -> 15102]
  1: free

[15101 op=2 display+exit]
  1: "To know that the moons are not properly aligned yet for the ritual."
  2: "To assure you that you will not be sacrificed for many weeks."
  3: "To assure you further that surrender is the only way you can travel among my people."

[15103 op=1 keyword; failure -> 15105]
  1: surr

[15104 op=2 display+exit]
  1: "To have done the honorable thing and now be respected and unfeared by my people."

[15105 op=1 keyword; failure -> 15108]
  1: surr

[15107 op=2 display+exit]
  1: "To travel to the @Inquisitor."
  2: "To tell the Inquisitor that you @surrender."
  3: "Then to be no longer a threat, and no longer be feared."

[15109 op=1 keyword; failure -> 15111]
  1: bye

[15110 op=2 display+exit]
  1: "May your precision and persistence lead to success!"

[15111 op=1 keyword; failure -> 15114]
  1: bye

[15113 op=2 display+exit]
  1: As you turn to leave, the gargoyle stops you.
  2: "Wait," he calls.
  3: "To be feared and hated by all gargoyles."
  4: "To be unsafe for you right now, False Prophet."
  5: "To @surrender is only answer."
  6: "To surrender to @Inquisitor."
  7: "Then to be no longer a threat, and then not to be feared by my people!"
  8: "To surrender to the @Inquisitor, and then to be @free to look for the solution."

[15116 op=2 display+exit]
  1: "To be confused by your accent."

[15117 op=2 display+exit]
  1: "To ask what $Z means?"

```

<a id="valor-npc-192"></a>
## Valor ? NPC 192, port 193

Instruction range: 16557?16599

```text
[16561 op=1 keyword; failure -> 16563]
  1: look

[16562 op=2 display+exit]
  1: the altar of Valor.
  2: As you kneel before the altar, a mystical voice sounds in your head.
  3: "Thou hast much to learn about valor."
  4: Do you wish to meditate at this altar?

[16563 op=1 keyword; failure -> 16566]
  1: yes

[16565 op=2 display+exit]
  1: "Who will meditate?"

[16566 op=1 keyword; failure -> 16568]
  1: no

[16567 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16569 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds have shown admirable valor."
  3: Do you wish to meditate at this altar?

[16571 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thy deeds serve as an example to all."
  3: Do you wish to meditate at this altar?

[16572 op=2 display+exit]
  1: As you kneel before the altar, a mystical voice sounds in your head.
  2: "Thou hast much to learn about valor."
  3: Do you wish to meditate at this altar?

[16575 op=2 display+exit]
  1: "Then there is naught else for you to learn here and now."

[16578 op=2 display+exit]
  1: "Speak the Mantra of Valor."

[16581 op=1 keyword; failure -> 16583]
  1: ra

[16584 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16585 op=2 display+exit]
  1: "You receive no enlightenment."

[16591 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16592 op=2 display+exit]
  1: "Tender-handed stroke a nettle, and it stings you for your pains."
  2: "Grasp it as a man of mettle, and it soft as silk remains."
  3: "There is naught else for you to learn here and now."
  4: "Return when thy journey has progressed further."

[16598 op=22 display+continue]
  1: "$Z... $Z..."
  2: "$Z... $Z..."
  3: "$Z... $Z..."

[16599 op=2 display+exit]
  1: "Tender-handed stroke a nettle, and it stings you for your pains."
  2: "Grasp it as a man of mettle, and it soft as silk remains."
  3: $PARTYMEMBER has gained a level...
  4: and strength!

```

<a id="van-kellian-npc-44"></a>
## Van Kellian ? NPC 44, port 45

Instruction range: 4069?4132

```text
[4071 op=1 keyword; failure -> 4073]
  1: look

[4072 op=2 display+exit]
  1: a man dressed in velvets and silks of yellow, green and blue.
  2: His voice is rich and mellow.
  3: "Bit of a brawl going on just now. Be glad to talk to you some other time."

[4073 op=2 display+exit]
  1: "Bit of a brawl going on just now. Be glad to talk to you some other time."

[4080 op=2 display+exit]
  1: "A pleasure, to be sure, $Z."

[4083 op=2 display+exit]
  1: "A pleasure, to be sure, $Z."

[4084 op=1 keyword; failure -> 4087]
  1: look

[4086 op=2 display+exit]
  1: a man dressed in velvets and silks of yellow, green and blue.
  2: "A very good $T to ye, my friend."

[4087 op=1 keyword; failure -> 4091]
  1: look

[4090 op=2 display+exit]
  1: a man dressed in velvets and silks of yellow, green and blue.
  2: "How do you do. I am the bard Van Kellian."
  3: "Please, speak your name, $G."

[4091 op=1 keyword; failure -> 4093]
  1: name

[4092 op=2 display+exit]
  1: "Call me Van."

[4093 op=1 keyword; failure -> 4099]
  1: job

[4097 op=22 display+continue]
  1: Iolo rolls his eyes and whispers to you.
  2: "He believes he does us a favor. This I must hear."

[4098 op=2 display+exit]
  1: "Why, singing is my trade."
  2: "Songs to soothe the soul and refresh the mind."
  3: "Shall I sing for ye?"
  4: "Well? How about a @song?"

[4099 op=1 keyword; failure -> 4102]
  1: job

[4101 op=2 display+exit]
  1: "Why, singing is my trade."
  2: "Songs to soothe the soul and refresh the mind."
  3: "Shall I sing for ye?"
  4: "Well? How about a @song?"

[4102 op=1 keyword; failure -> 4104]
  1: yes

[4103 op=2 display+exit]
  1: The bard plays a harp and sings.
  2: "Tho' she disdains to speak to me 
  3: Ah how I pine and sigh for her
  4: I would that I were handsome- o
  5: Sing ho eyo he hum!"

[4104 op=1 keyword; failure -> 4106]
  1: no

[4105 op=2 display+exit]
  1: "Well, perhaps another time, then."

[4106 op=1 keyword; failure -> 4108]
  1: sing,song

[4107 op=2 display+exit]
  1: "'Tis such a topsy-turvy world
  2: When my sweet lady-love whirls by
  3: I would that I could catch her eye
  4: Sing ho eyo he hum!"

[4108 op=1 keyword; failure -> 4110]
  1: mant

[4109 op=2 display+exit]
  1: "Did ye know that the mantra of pride is 'mul?' A @beggar told me that."

[4110 op=1 keyword; failure -> 4112]
  1: prid,begg,mul

[4111 op=2 display+exit]
  1: "There is little enough dignity in the life of a beggar."
  2: "I gave him a few coins, of course."

[4112 op=1 keyword; failure -> 4114]
  1: stel,star

[4113 op=2 display+exit]
  1: "Starhelm and I have often debated the nature of the gargoyle @Sin 'Vraal."

[4114 op=1 keyword; failure -> 4119]
  1: sin,vraa,'vra

[4116 op=22 display+continue]
  1: "If Lord British suffers him to live, then I will not harm him."
  2: "What possible threat could one @gargoyle living alone in the Dry Lands be?"

[4118 op=2 display+exit]
  1: Stelnar says "I'll tell ye, Van."
  2: "If ye let one gargoyle live,
  3: then ye'll be tempted to spare trolls,
  4: and then even cyclopses."
  5: "And then ye forget valour, and soon ye are overrun by monsters!"

[4119 op=1 keyword; failure -> 4121]
  1: sin,vraa,'vra

[4120 op=2 display+exit]
  1: "If Lord British suffers him to live, then I will not harm him."
  2: "What possible threat could one @gargoyle living alone in the Dry Lands be?"

[4121 op=1 keyword; failure -> 4126]
  1: valo,comp

[4123 op=22 display+continue]
  1: "Valour comes from the courage to be virtuous."
  2: "Just as compassion comes from a love of all mankind."
  3: "Courage grows. Why not love?"

[4125 op=2 display+exit]
  1: "Aha! That means monsters are not deserving of mercy!" says Stelnar

[4126 op=1 keyword; failure -> 4128]
  1: valo,comp

[4127 op=2 display+exit]
  1: "Valour comes from the courage to be virtuous."
  2: "Just as compassion comes from a love of all mankind."

[4128 op=1 keyword; failure -> 4130]
  1: garg

[4129 op=2 display+exit]
  1: "Well, I hear their skin is stony and rough."
  2: "Their cries are fierce yowlings to make the blood run cold with fear."
  3: "Though, mind ye, I myself have never faced one in combat."

[4130 op=1 keyword; failure -> 4132]
  1: bye

[4131 op=2 display+exit]
  1: "Good $T, and farewell."

[4132 op=2 display+exit]
  1: "Hmm. No, I cannot help ye with that."

```

<a id="wanda-npc-138"></a>
## Wanda ? NPC 138, port 139

Instruction range: 12802?12913

```text
[12803 op=1 keyword; failure -> 12808]
  1: yes

[12809 op=1 keyword; failure -> 12813]
  1: yes

[12812 op=2 display+exit]
  1: "Hey, I don't give credit!"
  2: "No pay, no play, capish?"

[12816 op=2 display+exit]
  1: "Fine!"
  2: "Come back when you get permission from your mother!"

[12818 op=1 keyword; failure -> 12821]
  1: yes

[12820 op=2 display+exit]
  1: "Well this is the place!"

[12823 op=2 display+exit]
  1: "Ha!"
  2: "You must be afraid of what your mother might think."

[12825 op=1 keyword; failure -> 12827]
  1: look

[12826 op=2 display+exit]
  1: a gypsy wench with a voluptuous figure.
  2: "Hello, wench."
  3: "How goes it?"

[12827 op=1 keyword; failure -> 12830]
  1: look

[12829 op=2 display+exit]
  1: "Well, hello handsome!"
  2: "Looking for a little... relaxation?"

[12830 op=1 keyword; failure -> 12833]
  1: name

[12832 op=2 display+exit]
  1: "They call me '@Wicked Wanda,' cheapskate."

[12833 op=1 keyword; failure -> 12836]
  1: name

[12835 op=2 display+exit]
  1: "They call me '@Wicked Wanda,' sexy."

[12836 op=1 keyword; failure -> 12838]
  1: name

[12837 op=2 display+exit]
  1: "They call me '@Wicked Wanda,' honey."

[12838 op=1 keyword; failure -> 12840]
  1: wick,wand

[12839 op=2 display+exit]
  1: "If we can find someplace @private,
  2: I'll gladly show you how I got my nickname..."

[12841 op=1 keyword; failure -> 12843]
  1: plac,rela,find,priv,show,nick,pay,swee,sale

[12842 op=2 display+exit]
  1: "Why madam, are you trying to insinuate something?"
  2: She breaks into a fit of giggles.

[12844 op=1 keyword; failure -> 12847]
  1: plac,rela,find,priv,show,nick,pay,swee,sale

[12846 op=2 display+exit]
  1: "Hmm, sounds like I've piqued your interest..."

[12847 op=1 keyword; failure -> 12849]
  1: plac,rela,find,priv,show,nick,pay,swee,sale

[12848 op=2 display+exit]
  1: "Why sirrah, are you trying to insinuate something?"
  2: She breaks into a fit of giggles.

[12849 op=1 keyword; failure -> 12854]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12853 op=2 display+exit]
  1: "Again, so soon?"
  2: "Nay, come back later, when I've had a little time to @rest."

[12854 op=1 keyword; failure -> 12857]
  1: sex,screw,boff,suck,head,lay,roll,hay,kink,make

[12857 op=1 keyword; failure -> 12859]
  1: map

[12858 op=2 display+exit]
  1: "We don't have..."
  2: "Oh, wait, there was something!"
  3: "@Arturos, he, uh, found a map last time we were up at Empath @Abbey."

[12859 op=1 keyword; failure -> 12862]
  1: artu

[12861 op=2 display+exit]
  1: "He'll tell your @fortune for you, cheapskate."
  2: "Just ask him!"

[12862 op=1 keyword; failure -> 12865]
  1: artu

[12864 op=2 display+exit]
  1: "He'll tell your @fortune for you, sexy."
  2: "Just ask him!"

[12865 op=1 keyword; failure -> 12867]
  1: artu

[12866 op=2 display+exit]
  1: "He'll tell your @fortune for you, honey."
  2: "Just ask him!"

[12867 op=1 keyword; failure -> 12869]
  1: fort

[12868 op=2 display+exit]
  1: "That's right."

[12869 op=1 keyword; failure -> 12871]
  1: empa,abbe

[12870 op=2 display+exit]
  1: "I don't care much for them."
  2: "They're all too stuck up."

[12871 op=1 keyword; failure -> 12873]
  1: silv,tabl

[12872 op=2 display+exit]
  1: "@Andreas has a silver tablet you could ask him about."
  2: "But I have something much @sweeter for sale..."

[12873 op=1 keyword; failure -> 12876]
  1: andr

[12875 op=2 display+exit]
  1: "He could show you a real good time, honey!"

[12876 op=1 keyword; failure -> 12878]
  1: andr

[12877 op=2 display+exit]
  1: "He's in the same line of work as I am--but I think I'm more your type!"

[12878 op=1 keyword; failure -> 12880]
  1: late,time,rest

[12879 op=2 display+exit]
  1: "'Tis tiring work, you know."

[12880 op=1 keyword; failure -> 12883]
  1: bye

[12882 op=2 display+exit]
  1: "'Twas a business doing pleasure with you..."
  2: "I hope you come again soon!"

[12883 op=1 keyword; failure -> 12886]
  1: bye

[12885 op=2 display+exit]
  1: "See you around..."
  2: "And if you meet anyone tall, dark and handsome, send 'em my way!"

[12886 op=1 keyword; failure -> 12888]
  1: bye

[12887 op=2 display+exit]
  1: "If you ever decide you need a break from adventuring, come find me again."

[12889 op=2 display+exit]
  1: "Why concern yourself with that, cheapskate?"
  2: "Surely you can think of something more pleasant..."

[12891 op=2 display+exit]
  1: "Why concern yourself with that, sexy?"
  2: "Surely you can think of something more pleasant..."

[12892 op=2 display+exit]
  1: "Why concern yourself with that, honey?"
  2: "Surely you can think of something more pleasant..."

[12896 op=22 display+continue]
  1: "I don't get many lady customers... but why not?"
  2: "You're sure this is what you want?"
  3: "Come on, honey."
  4: "Cross my palm with 15 gold and I'll show you a good time."
  5: "Okay?"

[12898 op=2 display+exit]
  1: Dupre speaks.
  2: "I don't think we have time for this..."

[12900 op=2 display+exit]
  1: "I don't get many lady customers... but why not?"
  2: "You're sure this is what you want?"
  3: A voice in the back of your head asks, "Do you really have time for this?"
  4: "Come on, honey."
  5: "Cross my palm with 15 gold and I'll show you a good time."
  6: "Okay?"

[12902 op=22 display+continue]
  1: "I've a cot in my wagon..."
  2: "15 gold's the price--and I don't haggle!"
  3: "Interested, honey?"
  4: "Come on, honey."
  5: "Cross my palm with 15 gold and I'll show you a good time."
  6: "Okay?"

[12904 op=2 display+exit]
  1: Dupre speaks.
  2: "I don't think we have time for this..."

[12905 op=2 display+exit]
  1: "I've a cot in my wagon..."
  2: "15 gold's the price--and I don't haggle!"
  3: "Interested, honey?"
  4: A voice in the back of your head asks, "Do you really have time for this?"
  5: "Come on, honey."
  6: "Cross my palm with 15 gold and I'll show you a good time."
  7: "Okay?"

[12910 op=22 display+continue]
  1: Taking you by the hand, the gyspy wench leads you off to her wagon.
  2: After a time, you return to your comrades.
  3: "Nothing's wrong with a little fun."
  4: "There's plenty of time... as long as there's plenty of gold!"

[12912 op=2 display+exit]
  1: "That's a fine way to be carrying on when all of Britannia is in danger!"
  2: "When our quest is complete, then there will be time enough for wenching!"

[12913 op=2 display+exit]
  1: Taking you by the hand, the gyspy wench leads you off to her wagon.
  2: After a time, you return to your comrades.
  3: You try to ignore the nagging voice in the back of your mind that keeps saying, "You shouldn't have wasted time like that."

```

<a id="weaponsmith-npc-172"></a>
## Weaponsmith ? NPC 172, port 173

Instruction range: 15299?15392

```text
[15303 op=1 keyword; failure -> 15305]
  1: look

[15304 op=2 display+exit]
  1: a young, nervous-looking gargoyle.
  2: "An-bal-sil-fer!" the gargoyle screams.
  3: "Agra-lem! Agra-Lem!"
  4: Suddenly realizing that he is holding a weapon, the gargoyle attacks!

[15305 op=2 display+exit]
  1: "An-bal-sil-fer!" the gargoyle screams.
  2: "Agra-lem! Agra-Lem!"
  3: Suddenly realizing that he is holding a weapon, the gargoyle attacks!

[15306 op=1 keyword; failure -> 15308]
  1: look

[15307 op=2 display+exit]
  1: a young, nervous-looking gargoyle.
  2: "The False Prophet!" the gargoyle screams.
  3: "Guards! Guards!"
  4: Suddenly realizing that he is holding a weapon, the gargoyle attacks!

[15308 op=2 display+exit]
  1: "The False Prophet!" the gargoyle screams.
  2: "Guards! Guards!"
  3: Suddenly realizing that he is holding a weapon, the gargoyle attacks!

[15312 op=1 keyword; failure -> 15319]
  1: yes

[15316 op=22 display+continue]
  1: The gargoyle speaks.
  2: "To ask if anything else interests you?"

[15318 op=2 display+exit]
  1: As you take the boomerang, Shamino speaks.
  2: "Aye, 'tis a most interesting weapon, $G."
  3: "Perhaps when our quest is through I can study its use."

[15319 op=1 keyword; failure -> 15323]
  1: yes

[15322 op=2 display+exit]
  1: The gargoyle hands you a boomerang.
  2: "To ask if anything else interests you?"

[15323 op=2 display+exit]
  1: The gargoyle nods.
  2: "Yes, to see that you already have enough weapons."
  3: "To ask if anything else interests you?"

[15327 op=1 keyword; failure -> 15331]
  1: yes

[15330 op=2 display+exit]
  1: He hands you a small pouch.
  2: "To hope you find it useful."
  3: "To ask if anything else interests you?"

[15331 op=2 display+exit]
  1: The gargoyle nods.
  2: "To understand your reluctance."
  3: "To know the dangers of zu ylem dust to the untrained user."
  4: "To ask if anything else interests you?"

[15332 op=1 keyword; failure -> 15336]
  1: look

[15335 op=2 display+exit]
  1: a young, nervous-looking gargoyle.
  2: "The False Prophet!"
  3: "To thank you for your upcoming sacrifice."
  4: "To greatly respect your nobility and courage."

[15336 op=1 keyword; failure -> 15338]
  1: look

[15337 op=2 display+exit]
  1: a young, nervous-looking gargoyle.
  2: "To ask how I can help you, noble one."

[15338 op=1 keyword; failure -> 15340]
  1: buy,sell

[15339 op=2 display+exit]
  1: "To not understand what that means."
  2: "To create these items for the use of @all."

[15340 op=1 keyword; failure -> 15342]
  1: all

[15341 op=2 display+exit]
  1: "To give these wares to any gargoyle who has need of them."
  2: "To invite you to look around at my @wares as well."
  3: "To ask if any of them interest you?"

[15342 op=1 keyword; failure -> 15344]
  1: ware

[15343 op=2 display+exit]
  1: "To have but a few items available."
  2: "To be far less adept than my former @master."

[15344 op=1 keyword; failure -> 15346]
  1: good

[15345 op=2 display+exit]
  1: "To consider him a discredit to the race!"
  2: "Most of all, to consider him a @dishonor to my late master!"

[15346 op=1 keyword; failure -> 15348]
  1: dish

[15347 op=2 display+exit]
  1: "To cheapen all our loss, all our grief, by his self-pity."
  2: "To have lost my @master, like a father to me."
  3: "Yet to maintain control, to maintain diligence."
  4: "Most of all to maintain passion for life."
  5: "To feel that the goodscrafter dishonors us all by abandoning our principles."
  6: "To feel that the goodscrafter needs a trip to the @catacombs."

[15348 op=1 keyword; failure -> 15350]
  1: cata

[15349 op=2 display+exit]
  1: "Yes, to have visited them myself once, long ago."
  2: "To find there shrines to our three principles: control, passion and diligence."
  3: "To speak with these shrines and meditate, to receive new insight."
  4: "To not remember where the catacombs are."
  5: "To be sent there by the Temple of @Singularity."

[15350 op=1 keyword; failure -> 15352]
  1: temp,sing

[15351 op=2 display+exit]
  1: "To find that to the north, in the mountains."
  2: "To advise you to find some way to @fly if you wish to get there, though!"

[15352 op=1 keyword; failure -> 15354]
  1: fly

[15353 op=2 display+exit]
  1: "To be unable to reach the Temple on foot."
  2: "To be forced to fly over the mountains."

[15354 op=1 keyword; failure -> 15356]
  1: snak,veno,silv

[15355 op=2 display+exit]
  1: "To be a powerful but dangerous drug!"
  2: "To turn gargoyle warriors into unstoppable fighters!"
  3: "But to always be fatal after the battle ends."
  4: "To be created by the snake @charmer, who lives to the southwest."

[15356 op=1 keyword; failure -> 15358]
  1: char

[15357 op=2 display+exit]
  1: "Yes, to suggest you visit him if you are interested."
  2: "To find the snake-summoning horn a most impressive sight!"

[15358 op=1 keyword; failure -> 15360]
  1: plan

[15359 op=2 display+exit]
  1: "To create the zu ylem dust from plants that can no longer be found."
  2: "But to know of far worse effects of the @disasters."

[15360 op=1 keyword; failure -> 15362]
  1: armo,helm,belt

[15361 op=2 display+exit]
  1: "To see little of interest in that item."

[15362 op=1 keyword; failure -> 15364]
  1: name

[15363 op=2 display+exit]
  1: "To be but a simple @weaponsmith, False Prophet."
  2: "To lack a name, as is appropriate for my station."

[15364 op=1 keyword; failure -> 15366]
  1: disa

[15365 op=2 display+exit]
  1: "Yes, surely to know of the disasters?"
  2: "To have only barely survived the earthquakes after the Codex disappeared."

[15366 op=1 keyword; failure -> 15368]
  1: job,weap,mast,work

[15367 op=2 display+exit]
  1: "To be the master weaponsmith now,"
  2: "although to have but recently been a simple @apprentice."

[15368 op=1 keyword; failure -> 15370]
  1: appr

[15369 op=2 display+exit]
  1: "To greatly regret the loss of my master,"
  2: "killed during the @disasters."
  3: "But to carry on in the face of adversity."
  4: "And certainly not to lose all passion for life, like the worthless @goodscrafter!"

[15370 op=1 keyword; failure -> 15372]
  1: join

[15371 op=2 display+exit]
  1: "To wish I could leave on adventures..."
  2: "But to be dedicated to my important @work."

[15372 op=1 keyword; failure -> 15375]
  1: boom

[15374 op=2 display+exit]
  1: "To ask you to see me at my shop!"

[15375 op=1 keyword; failure -> 15378]
  1: boom

[15377 op=2 display+exit]
  1: "To wish I had more boomerangs, that I might give you another."

[15378 op=1 keyword; failure -> 15381]
  1: boom

[15380 op=2 display+exit]
  1: "Ah, to see you admire these boomerangs, the work of my @master."
  2: "To regret being unable to create boomerangs myself.""To have only a few left in stock, but to offer you one."
  3: Do you accept the gift?

[15381 op=1 keyword; failure -> 15384]
  1: zu,ylem

[15383 op=2 display+exit]
  1: "To ask you to see me at my shop!"

[15384 op=1 keyword; failure -> 15387]
  1: zu,ylem

[15386 op=2 display+exit]
  1: "To regret that I have such a small supply of zu ylem dust."

[15387 op=1 keyword; failure -> 15390]
  1: zu,ylem

[15389 op=2 display+exit]
  1: "Yes, to be a very interesting weapon."
  2: "To knock out even a large animal with just a single pouch of this powder!"
  3: "To be used most often by hunters, to avoid harming the meat."
  4: "Unfortunately, to make this powder from @plants that can no longer be found."
  5: "Here, to offer you one."
  6: Do you accept the gift?

[15390 op=1 keyword; failure -> 15392]
  1: bye

[15391 op=2 display+exit]
  1: "May your long work and strength lead to success!"

[15392 op=2 display+exit]
  1: "To know nothing of $Z, being but a simple weaponsmith."

```

<a id="whitsaber-npc-74"></a>
## Whitsaber ? NPC 74, port 75

Instruction range: 6980?7052

```text
[6981 op=1 keyword; failure -> 6986]
  1: yes

[6985 op=2 display+exit]
  1: "Then all is lost!"
  2: "Have pity, Avatar, and keep my secret!"
  3: "I have changed since those dark days, truly I have!"

[6987 op=1 keyword; failure -> 6990]
  1: yes

[6989 op=2 display+exit]
  1: "Oh."
  2: "You must have misunderstood him, because it isn't true!"
  3: "Ask him again if you want..."

[6992 op=2 display+exit]
  1: He seems strangely relieved.
  2: "Oh."
  3: "Well, good, because it isn't true!"

[6994 op=1 keyword; failure -> 6997]
  1: yes

[6996 op=2 display+exit]
  1: "Oh."
  2: He seems disappointed somehow.

[6999 op=2 display+exit]
  1: "Because Trinsic is the Town of Honor!"
  2: "None here would be dishonorable enough to steal it."
  3: "And surely, if any took the rune, they would do the honorable thing and return it afterwards!"

[7001 op=1 keyword; failure -> 7007]
  1: yes

[7006 op=2 display+exit]
  1: "Oh, thank you, Avatar."
  2: He hands over his piece of the map.
  3: "I'll rely on thy honor to keep my secret."

[7009 op=2 display+exit]
  1: "But what more can I do?"
  2: "I cannot change the past!"
  3: "By giving you the map, do I not prove that I have changed my ways?"
  4: "No pirate would willingly give up the key to such a treasure!"

[7010 op=1 keyword; failure -> 7013]
  1: look

[7012 op=2 display+exit]
  1: a balding but distinguished man.
  2: Recognizing you, fear fills the former pirate's eyes.
  3: "What do you want? I thought we had a deal!"

[7013 op=1 keyword; failure -> 7016]
  1: look

[7015 op=2 display+exit]
  1: a balding but distinguished man.
  2: Recognizing you, fear fills the former pirate's eyes.
  3: "What do you want? I thought we had a deal!"

[7016 op=1 keyword; failure -> 7020]
  1: look

[7019 op=2 display+exit]
  1: a balding but distinguished man.
  2: "Welcome, Avatar!"

[7020 op=1 keyword; failure -> 7022]
  1: look

[7021 op=2 display+exit]
  1: a balding but distinguished man.
  2: "Welcome back, Avatar!"

[7022 op=1 keyword; failure -> 7024]
  1: name

[7023 op=2 display+exit]
  1: "I am Lord Whitsaber, mayor of this honorable town!"

[7024 op=1 keyword; failure -> 7026]
  1: job

[7025 op=2 display+exit]
  1: "I am entrusted with an honor above all others-that of governing Trinsic, the Town of @Honor!"

[7026 op=1 keyword; failure -> 7028]
  1: trin,town,hono

[7027 op=2 display+exit]
  1: "Every citizen of this fine town strives every day to be a living example of honor."
  2: "Of course, thy own example guides us all, Avatar!"

[7030 op=1 keyword; failure -> 7033]
  1: map,ship,hawk,capt,home

[7032 op=2 display+exit]
  1: "I gave you the map."
  2: "What more do you want?"

[7033 op=1 keyword; failure -> 7036]
  1: map,ship,hawk,capt,home

[7035 op=2 display+exit]
  1: "All right!"
  2: "If thou wouldst but promise to keep my secret, I'll give thee the map!"
  3: "Agreed?"

[7036 op=1 keyword; failure -> 7038]
  1: pira

[7037 op=2 display+exit]
  1: "Me, a pirate?"
  2: "Absurd!"
  3: He laughs out loud...but you sense a trace of fear in his voice!

[7038 op=1 keyword; failure -> 7041]
  1: sand,alas,gord,firs,mate

[7040 op=2 display+exit]
  1: "Wh--what did Sandy tell you?"
  2: "Did he say I was once a @pirate?"

[7041 op=1 keyword; failure -> 7044]
  1: rune

[7043 op=2 display+exit]
  1: "The Rune of Honor? Why, it is on a pedestal in the center of town!"
  2: "Even though the rune is our most prized possession, we do not guard it!"
  3: "And dost thou know why not?"

[7044 op=1 keyword; failure -> 7046]
  1: mant

[7045 op=2 display+exit]
  1: "The Mantra of Honor? Let me think..."
  2: "Oh yes, I remember now. It's 'summ.'"

[7047 op=1 keyword; failure -> 7050]
  1: bye

[7049 op=2 display+exit]
  1: "Goodbye, Avatar!"
  2: "Thou hast my thanks!"
  3: "Truly are thy compassion and honor an example to us all!"

[7050 op=1 keyword; failure -> 7052]
  1: bye

[7051 op=2 display+exit]
  1: "Good $T, Avatar."
  2: "Return again when thou hast more time to chat!"

[7052 op=2 display+exit]
  1: "I beg thy pardon, Avatar?"

```

<a id="wilbur-npc-25"></a>
## Wilbur ? NPC 25, port 26

Instruction range: 2150?2183

```text
[2151 op=1 keyword; failure -> 2154]
  1: yes

[2156 op=2 display+exit]
  1: "Perhaps some other time then."
  2: "How about for one of thy @friends?"

[2157 op=1 keyword; failure -> 2160]
  1: look

[2159 op=2 display+exit]
  1: a thin, dark-haired man with a nervous smile.
  2: "Hello again, my friend."
  3: "What can I do for you this $T?"

[2160 op=1 keyword; failure -> 2163]
  1: look

[2162 op=2 display+exit]
  1: a thin, dark-haired man with a nervous smile.
  2: "Greetings, traveler!"
  3: "What can I do for you this $T?"

[2163 op=1 keyword; failure -> 2166]
  1: name

[2165 op=2 display+exit]
  1: "My name's Wilbur, $G."

[2166 op=1 keyword; failure -> 2168]
  1: job

[2167 op=2 display+exit]
  1: "I work here at the stables, taking care of the horses."
  2: "You can @buy one if you'd like."

[2168 op=1 keyword; failure -> 2170]
  1: ed,smit,talk

[2169 op=2 display+exit]
  1: "A talking horse?"
  2: "Ridiculous!"
  3: "Who told you that?"
  4: He seems more nervous than before.

[2170 op=1 keyword; failure -> 2172]
  1: hors

[2171 op=2 display+exit]
  1: "Aye, if you wish to @buy one, just say the word."

[2172 op=1 keyword; failure -> 2175]
  1: buy,frie

[2174 op=2 display+exit]
  1: "My price is 75 gold pieces, no less."
  2: "Interested?"

[2175 op=1 keyword; failure -> 2177]
  1: bye

[2176 op=2 display+exit]
  1: "Come back again!"

[2177 op=2 display+exit]
  1: "Beg pardon, $G?"

[2180 op=2 display+exit]
  1: "Aye, $P, thou hast need of a horse."
  2: "But thou canst not afford one!"

[2182 op=2 display+exit]
  1: "Aye, $P, thou hast need of a horse."
  2: "But thou canst not carry its papers!"

[2183 op=2 display+exit]
  1: "Good!"
  2: He takes the gold, and hands over the reins of a fine horse.
  3: "How about for one of thy @friends?"

```

<a id="william-npc-91"></a>
## William ? NPC 91, port 92

Instruction range: 8656?8676

```text
[8656 op=1 keyword; failure -> 8658]
  1: look

[8657 op=2 display+exit]
  1: an old farmer who looks to have has made his peace with life.
  2: "Howdy, stranger."

[8658 op=1 keyword; failure -> 8660]
  1: name

[8659 op=2 display+exit]
  1: "M'name is William."

[8660 op=1 keyword; failure -> 8662]
  1: job

[8661 op=2 display+exit]
  1: "I'm a @farmer."

[8662 op=1 keyword; failure -> 8664]
  1: rune

[8663 op=2 display+exit]
  1: "Ask @Antonio."

[8664 op=1 keyword; failure -> 8666]
  1: anto

[8665 op=2 display+exit]
  1: "The lord of New @Magincia."

[8666 op=1 keyword; failure -> 8668]
  1: new,magi

[8667 op=2 display+exit]
  1: "That's where ya are now."

[8668 op=1 keyword; failure -> 8670]
  1: humi,humb,farm

[8669 op=2 display+exit]
  1: "Working the earth is the lowest form 'o labor."
  2: "But it provides food for them as writes books, plays music, and other such things."
  3: "Surely 'tis a humble living, to devote yer life to farmin'."
  4: "It's about all I do with meself, 'cept when I find time for carvin' @hippos."

[8670 op=1 keyword; failure -> 8672]
  1: carv,hipp

[8671 op=2 display+exit]
  1: "I'll show ya one."
  2: The old man gets a fiendish gleam in his eyes as he reaches into his pouch and takes out a small wooden hippo.
  3: "Aint she purty?"
  4: "If I don't make the purtiest gol' danged hippos you ever did see, you can call me @Mortimer."

[8672 op=1 keyword; failure -> 8674]
  1: mort

[8673 op=2 display+exit]
  1: "He's a mighty foolish fellow."

[8674 op=1 keyword; failure -> 8676]
  1: bye

[8675 op=2 display+exit]
  1: "See ya."

[8676 op=2 display+exit]
  1: "Can't help you there."

```

<a id="wisp-npc-199"></a>
## Wisp ? NPC 199, port 200

Instruction range: 16898?16988

```text
[16901 op=1 keyword; failure -> 16904]
  1: yes

[16903 op=2 display+exit]
  1: For a few moments, the wisp pulses, saying nothing.
  2: "It is done," the wisp says at last,
  3: "Rklbwm have expressed desire to possess this information."
  4: "Hoping, no doubt, for a bargain,
  5: Rklbwm have offered a small quantity of the substance 'gold' in lieu of the customary useful information."
  6: "But surely 'you' wish 'me' to arrange for Rklbwm to provide information of value to 'you.'"
  7: "Am 'I' correct to assume this?"

[16906 op=2 display+exit]
  1: "The ways of Britannia are strange indeed."

[16910 op=1 keyword; failure -> 16916]
  1: yes

[16915 op=2 display+exit]
  1: "Then so be it."
  2: "Here is useful information from the Rklbwm."
  3: 'If a substance with a partially reflective surface
  4: is positioned so that its third index of refraction
  5: matches the wavelength coefficient
  6: of the output of a polarized light source,
  7: and the resulting beam is focused on an ionized crystal
  8: suspended in a unipolar magnetic field,
  9: matter can be converted into useful electromagnetic radiation
  10: at an efficiency rate of 96 to 98 percent,
  11: depending on the desired output frequency.'
  12: "It was a pleasure doing business with you."

[16927 op=2 display+exit]
  1: "The ways of Britannia are strange indeed."

[16931 op=2 display+exit]
  1: "The volume 'you' bear with the symbol pattern representing the sounds
  2: 'The Book of Lost Mantras'
  3: is a prime source of high density information."
  4: "Do 'you' wish to 'me' to locate a client
  5: who is willing to make an exchange for it at this time?"

[16935 op=1 keyword; failure -> 16937]
  1: look

[16936 op=2 display+exit]
  1: a strange, glowing ball of energy.
  2: "Again, '@you' of this plane address 'I' of the Xorinite dimension."
  3: "By what name do 'you' wish to be addressed at this time?"

[16937 op=2 display+exit]
  1: "Again, '@you' of this plane address 'I' of the Xorinite dimension."
  2: "By what name do 'you' wish to be addressed at this time?"

[16945 op=2 display+exit]
  1: "Very well, '$Z.'"
  2: "Do 'you' have memory of previous discussions with 'myself?'"

[16947 op=1 keyword; failure -> 16950]
  1: yes

[16949 op=2 display+exit]
  1: "Good."
  2: "Tedious repetition should then be unnecessary on this occasion."
  3: "Of what would 'you' speak?"

[16952 op=2 display+exit]
  1: "So be it."
  2: "'I' must often repeat information when conversing with the life of Britannia, it seems."
  3: "'I' do not see how each unit of life here can have different memories,
  4: different desires,
  5: but again 'I' inform Britannia that it is not so in 'my' realm."
  6: "Those which 'you' know as 'wisp' in Britannia are projections of the life of Xorinia."
  7: "'I' extend into many planes beside this one."
  8: "Britannia communicates with 'me' very seldom, and is known by those of very few other planes."
  9: "Every time 'I' speak with Britannia, it seems 'I' must explain 'my' nature again, as 'I' have just done."
  10: "That is very strange."

[16953 op=1 keyword; failure -> 16956]
  1: look

[16955 op=2 display+exit]
  1: a strange, glowing ball of energy.
  2: "Again, '@you' of this plane address 'I' of the Xorinite dimension."
  3: "By what name do 'you' wish to be addressed at this time?"

[16956 op=1 keyword; failure -> 16959]
  1: name

[16958 op=2 display+exit]
  1: "The concept of 'name' is one 'I' still do not grasp."
  2: "'I' believe that when 'you' ask 'me' to use '$P' as a form of address,
  3: it has something to do with 'name.'"
  4: "When Britannia addressed me with the name '@Zog,'
  5: an attempt was made to explain."
  6: "But still 'I' do not understand."

[16959 op=1 keyword; failure -> 16961]
  1: you

[16960 op=2 display+exit]
  1: "'I' and 'you' are words that seem to have no meaning."
  2: "'I' mimic the way 'you' use them in this language out of courtesy."

[16961 op=1 keyword; failure -> 16963]
  1: job

[16962 op=2 display+exit]
  1: "Xorinia serves as a conduit for @information between different planes."
  2: "In exchange, these planes provide information that can be used to improve Xorinia."
  3: "Occasionally payment is accepted in the form of @substance,
  4: but this is not preferred."

[16963 op=1 keyword; failure -> 16965]
  1: cond,info,exch

[16964 op=2 display+exit]
  1: "Britannia has never participated in the interplanar exchange of information."
  2: "'I' have presented a sample known in Britannian terms as a '@secret',
  3: but no further interest has been shown."

[16965 op=1 keyword; failure -> 16967]
  1: subs

[16966 op=2 display+exit]
  1: "Actual physical materials are so plentiful throughout the multiverse that they are of little value."
  2: "Information has much greater worth."

[16967 op=1 keyword; failure -> 16969]
  1: paym,inte

[16968 op=2 display+exit]
  1: "If 'you' present a reasonably dense information source to a 'wisp' unit,
  2: arrangements will be made to transfer some suitably valuable commodity into Britannian possession."

[16969 op=1 keyword; failure -> 16971]
  1: zog

[16970 op=2 display+exit]
  1: "Once when 'I' offered sample information to Britannia,
  2: the name 'Zog' was given by the life unit that received it."
  3: "No further contact with Britannia was obtainable for some time after this."
  4: "Perhaps misuse of the sample information was responsible."

[16971 op=1 keyword; failure -> 16977]
  1: secr

[16976 op=2 display+exit]
  1: "Because of the nature of Britannian memory,"
  2: "'I' now present the sample information to 'you' again."
  3: The wisp pulsates, fading from blue to white, then back to blue again.
  4: As it does so, a scroll materializes in your hand.
  5: "This is a simple invocation of magic energy to bring about the cessation of all life."
  6: "It is not a very useful or powerful form of magic,
  7: as it will only affect beings in the same plane where it is invoked."
  8: "Inform any 'wisp' unit if 'you' have @interest in further exchange.

[16977 op=1 keyword; failure -> 16979]
  1: secr

[16978 op=2 display+exit]
  1: "Because of the nature of Britannian memory,"
  2: "'I' now present the sample information to 'you' again."
  3: The wisp pulsates, fading from blue to white, then back to blue again.
  4: "But 'you' have not sent enough life units to transport this substance!"

[16979 op=1 keyword; failure -> 16981]
  1: bye

[16980 op=2 display+exit]
  1: "'I' find it unusual that 'you' find it so important to inform 'me'
  2: of the imminent cessation of proximity between one unit of Britannian life and one of Xorinite."
  3: "Nonetheless, in accordance with 'your' custom,"
  4: "Goodbye, '$P.'"

[16981 op=2 display+exit]
  1: "The vagaries of the language 'you' speak are quite beyond 'me.'"
  2: "Perhaps another line of inquiry would prove more 'fruitful.'"

[16987 op=2 display+exit]
  1: "Very well."
  2: The wisp glows just a trifle brighter for a moment,
  3: and suddenly you notice that your pack has grown heavier.
  4: "It was a pleasure doing business with you."

[16988 op=2 display+exit]
  1: "Very well."
  2: "'You' have not sent enough life units to transport much substance,
  3: but it is 'your' decision to make."
  4: The wisp glows just a trifle brighter for a moment,
  5: and suddenly you notice that your pack has grown heavier.
  6: "It was a pleasure doing business with you."

```

<a id="xiao-npc-34"></a>
## Xiao ? NPC 34, port 35

Instruction range: 2838?3233

```text
[2844 op=2 display+exit]
  1: "Perhaps something else then."

[2846 op=2 display+exit]
  1: "That's too many!"

[2857 op=2 display+exit]
  1: "That costs more than thou hast!"

[2859 op=2 display+exit]
  1: "It would be too heavy for thee!"

[2861 op=2 display+exit]
  1: "Use it wisely."
  2: She carefully hands you the potion.

[2862 op=2 display+exit]
  1: "Use them wisely"
  2: She carefully hands you the potions.

[2868 op=2 display+exit]
  1: "Thou art not yet ready for such knowledge."
  2: "Seek out and speak with the @wisps, and learn their secrets."
  3: "Then wilt thou will be prepared to enter the Eighth Circle."

[2871 op=2 display+exit]
  1: "Which one, @detect @magic, @cure or @light?"

[2874 op=2 display+exit]
  1: "Which one, @infravision, @reappear, @telekineses or @vanish?"

[2877 op=2 display+exit]
  1: "Which one, @dispel @field, @great @light or @peer?"

[2880 op=2 display+exit]
  1: "Which one, @animate, @fire @field, @locate, @mass @cure, @poison @field, @sleep @field or @wind @change?"

[2883 op=2 display+exit]
  1: "Which one, @energy @field, @invisibility, @reveal or @X-ray?"

[2886 op=2 display+exit]
  1: "Which one, @clone, @negate @magic or @replicate?"

[2889 op=2 display+exit]
  1: "Which one, @fear, @gate @travel or @wizard @eye?"

[2892 op=2 display+exit]
  1: "Which one, @death @wind, @eclipse, @mass @charm, @mass @kill, @slime, @summon, @time @stop or @tremor?"

[2893 op=2 display+exit]
  1: "As everybody knows, there are only eight circles."

[2896 op=1 keyword; failure -> 2901]
  1: dete

[2900 op=2 display+exit]
  1: "That spell costs 25 gold. Interested?"

[2901 op=1 keyword; failure -> 2906]
  1: cure

[2905 op=2 display+exit]
  1: "That spell costs 25 gold. Interested?"

[2906 op=1 keyword; failure -> 2911]
  1: ligh

[2910 op=2 display+exit]
  1: "That spell costs 25 gold. Interested?"

[2912 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[2915 op=1 keyword; failure -> 2920]
  1: infr

[2919 op=2 display+exit]
  1: "That spell costs 50 gold. Interested?"

[2920 op=1 keyword; failure -> 2925]
  1: reap

[2924 op=2 display+exit]
  1: "That spell costs 50 gold. Interested?"

[2925 op=1 keyword; failure -> 2930]
  1: tele

[2929 op=2 display+exit]
  1: "That spell costs 50 gold. Interested?"

[2930 op=1 keyword; failure -> 2935]
  1: vani

[2934 op=2 display+exit]
  1: "That spell costs 50 gold. Interested?"

[2936 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[2939 op=1 keyword; failure -> 2944]
  1: disp

[2943 op=2 display+exit]
  1: "That spell costs 75 gold. Interested?"

[2944 op=1 keyword; failure -> 2949]
  1: grea

[2948 op=2 display+exit]
  1: "That spell costs 75 gold. Interested?"

[2949 op=1 keyword; failure -> 2954]
  1: peer

[2953 op=2 display+exit]
  1: "That spell costs 75 gold. Interested?"

[2955 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[2958 op=1 keyword; failure -> 2963]
  1: anim

[2962 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2963 op=1 keyword; failure -> 2968]
  1: fire

[2967 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2968 op=1 keyword; failure -> 2973]
  1: loca

[2972 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2973 op=1 keyword; failure -> 2978]
  1: mass

[2977 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2978 op=1 keyword; failure -> 2983]
  1: pois

[2982 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2983 op=1 keyword; failure -> 2988]
  1: slee

[2987 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2988 op=1 keyword; failure -> 2993]
  1: wind

[2992 op=2 display+exit]
  1: "That spell costs 100 gold. Interested?"

[2994 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[2997 op=1 keyword; failure -> 3002]
  1: ener

[3001 op=2 display+exit]
  1: "That spell costs 125 gold. Interested?"

[3002 op=1 keyword; failure -> 3007]
  1: invi

[3006 op=2 display+exit]
  1: "That spell costs 125 gold. Interested?"

[3007 op=1 keyword; failure -> 3012]
  1: reve

[3011 op=2 display+exit]
  1: "That spell costs 125 gold. Interested?"

[3012 op=1 keyword; failure -> 3017]
  1: xray

[3016 op=2 display+exit]
  1: "That spell costs 125 gold. Interested?"

[3018 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[3021 op=1 keyword; failure -> 3026]
  1: clon

[3025 op=2 display+exit]
  1: "That spell costs 150 gold. Interested?"

[3026 op=1 keyword; failure -> 3031]
  1: nega

[3030 op=2 display+exit]
  1: "That spell costs 150 gold. Interested?"

[3031 op=1 keyword; failure -> 3036]
  1: repl

[3035 op=2 display+exit]
  1: "That spell costs 150 gold. Interested?"

[3037 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[3040 op=1 keyword; failure -> 3045]
  1: fear

[3044 op=2 display+exit]
  1: "That spell costs 175 gold. Interested?"

[3045 op=1 keyword; failure -> 3050]
  1: gate

[3049 op=2 display+exit]
  1: "That spell costs 175 gold. Interested?"

[3050 op=1 keyword; failure -> 3055]
  1: wiza

[3054 op=2 display+exit]
  1: "That spell costs 175 gold. Interested?"

[3056 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[3059 op=1 keyword; failure -> 3064]
  1: deat

[3063 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3064 op=1 keyword; failure -> 3069]
  1: ecli

[3068 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3069 op=1 keyword; failure -> 3074]
  1: masscharm

[3073 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3074 op=1 keyword; failure -> 3079]
  1: masskill

[3078 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3079 op=1 keyword; failure -> 3084]
  1: slim

[3083 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3084 op=1 keyword; failure -> 3089]
  1: summ

[3088 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3089 op=1 keyword; failure -> 3094]
  1: time

[3093 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3094 op=1 keyword; failure -> 3099]
  1: trem

[3098 op=2 display+exit]
  1: "That spell costs 200 gold. Interested?"

[3100 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[3104 op=1 keyword; failure -> 3109]
  1: yes

[3108 op=2 display+exit]
  1: "Here you are."
  2: She hands you a piece of rune-covered parchment.

[3110 op=2 display+exit]
  1: "You lack the gold required."

[3112 op=2 display+exit]
  1: "Thou cannot carry that."

[3113 op=2 display+exit]
  1: "Perhaps you are not ready for this circle."

[3117 op=1 keyword; failure -> 3122]
  1: yes

[3121 op=2 display+exit]
  1: "Here you are."

[3123 op=2 display+exit]
  1: "But thou cannot afford a spellbook!"

[3125 op=2 display+exit]
  1: "But thou cannot carry a spellbook!"

[3126 op=2 display+exit]
  1: "Perhaps something else then."

[3130 op=1 keyword; failure -> 3132]
  1: poti

[3131 op=2 display+exit]
  1: "Which, @blue or @black?"

[3132 op=1 keyword; failure -> 3136]
  1: blac

[3135 op=2 display+exit]
  1: "Black potions will cost thee 20 gold each."
  2: "How many wouldst thou like?"

[3136 op=1 keyword; failure -> 3140]
  1: blue

[3139 op=2 display+exit]
  1: "Blue potions will cost thee 10 gold each."
  2: "How many wouldst thou like?"

[3140 op=2 display+exit]
  1: "Perhaps something else then."

[3141 op=1 keyword; failure -> 3143]
  1: buy,reag

[3142 op=2 display+exit]
  1: "Which one, @black @pearl, @garlic, @ginseng, @mandrake @root, @nightshade, @spider @silk or @sulfurous @ash?"
  2: "I also sell @potions."

[3143 op=1 keyword; failure -> 3148]
  1: blac,pear

[3147 op=2 display+exit]
  1: "Black pearl costs 4 per portion."
  2: "How many dost thou want?"

[3148 op=1 keyword; failure -> 3153]
  1: garl

[3152 op=2 display+exit]
  1: "Garlic costs 3 per portion."
  2: "How many dost thou want?"

[3153 op=1 keyword; failure -> 3158]
  1: gins

[3157 op=2 display+exit]
  1: "Ginseng costs 3 per portion."
  2: "How many dost thou want?"

[3158 op=1 keyword; failure -> 3163]
  1: mand,root

[3162 op=2 display+exit]
  1: "Mandrake root costs 6 per portion."
  2: "How many dost thou want?"

[3163 op=1 keyword; failure -> 3168]
  1: nigh

[3167 op=2 display+exit]
  1: "Nightshade costs 3 per portion."
  2: "How many dost thou want?"

[3168 op=1 keyword; failure -> 3173]
  1: spid,silk

[3172 op=2 display+exit]
  1: "Spider silk costs 3 per portion."
  2: "How many dost thou want?"

[3173 op=1 keyword; failure -> 3178]
  1: sulf,ash

[3177 op=2 display+exit]
  1: "Sulfurous ash costs 3 per portion."
  2: "How many dost thou want?"

[3182 op=2 display+exit]
  1: "Perhaps something else then."

[3184 op=2 display+exit]
  1: "I cannot give thee that much!"

[3189 op=2 display+exit]
  1: "Thou cannot afford that much!"

[3191 op=2 display+exit]
  1: "Thou cannot carry that much!"

[3192 op=2 display+exit]
  1: "Here you are."

[3193 op=1 keyword; failure -> 3195]
  1: look

[3194 op=2 display+exit]
  1: a wise, scholarly woman.
  2: "Good $T, @Avatar. For what purpose hast thou come?"

[3195 op=1 keyword; failure -> 3198]
  1: name

[3197 op=2 display+exit]
  1: "I am Xiao."

[3198 op=1 keyword; failure -> 3200]
  1: job

[3199 op=2 display+exit]
  1: "I serve on the Council of @Wizards."
  2: "I also teach @spells and sell magical @reagents and @spellbooks."

[3200 op=1 keyword; failure -> 3202]
  1: secr,wisp

[3201 op=2 display+exit]
  1: "Their secret is far more @powerful than anything I might teach thee."

[3202 op=1 keyword; failure -> 3204]
  1: powe

[3203 op=2 display+exit]
  1: "Truly, it would be terrible if the wrong person were to learn it."
  2: "Only we of the Council of Wizards know the secret."
  3: "But, since thou art the Avatar, surely it is safe for thee to learn it as well."

[3204 op=1 keyword; failure -> 3206]
  1: avat

[3205 op=2 display+exit]
  1: "Your example inspires us all."

[3206 op=1 keyword; failure -> 3208]
  1: serv,coun,wiza

[3207 op=2 display+exit]
  1: "'Tis our task to keep the forces of magic in @balance throughout the land."
  2: "We also do what we can to promote the eight @virtues."

[3208 op=1 keyword; failure -> 3210]
  1: prom,virt

[3209 op=2 display+exit]
  1: "We created the @shrines, many years ago."

[3210 op=1 keyword; failure -> 3212]
  1: shri

[3211 op=2 display+exit]
  1: "@Thariand can tell thee more."

[3212 op=1 keyword; failure -> 3214]
  1: thar

[3213 op=2 display+exit]
  1: "He is the librarian at the @Lycaeum."

[3214 op=1 keyword; failure -> 3216]
  1: libr,lyca

[3215 op=2 display+exit]
  1: "Surely you know where the Lycaeum is."

[3216 op=1 keyword; failure -> 3218]
  1: forc,magi,bala

[3217 op=2 display+exit]
  1: "Not an easy task."

[3218 op=1 keyword; failure -> 3221]
  1: book,spellbook,spellbooks

[3220 op=2 display+exit]
  1: "A spellbook costs 60 gold pieces."
  2: "Interested?"

[3222 op=1 keyword; failure -> 3225]
  1: lear,spel

[3224 op=2 display+exit]
  1: "In which Circle do you wish to study?"

[3226 op=1 keyword; failure -> 3229]
  1: lear,spel

[3228 op=2 display+exit]
  1: "In which Circle do you wish to study?"

[3229 op=1 keyword; failure -> 3231]
  1: lear,spel

[3230 op=2 display+exit]
  1: "But none of you has a @spellbook!"

[3231 op=1 keyword; failure -> 3233]
  1: bye

[3232 op=2 display+exit]
  1: "Thou art always welcome here, Avatar."

[3233 op=2 display+exit]
  1: "Perhaps you should speak of that to another."

```

<a id="ybarra-npc-183"></a>
## Ybarra ? NPC 183, port 184

Instruction range: 16020?16152

```text
[16021 op=1 keyword; failure -> 16023]
  1: yes

[16026 op=2 display+exit]
  1: The man collapses, the last of his energy gone.

[16028 op=1 keyword; failure -> 16030]
  1: yes

[16032 op=2 display+exit]
  1: "You must!"
  2: "I'll die!"
  3: "Just a little bit of @food for the map?"

[16034 op=1 keyword; failure -> 16037]
  1: yes

[16040 op=2 display+exit]
  1: His last shred of hope torn away from him,
  2: the withered man leaps at you, screaming "@Food! @Food!"

[16041 op=1 keyword; failure -> 16045]
  1: look

[16044 op=2 display+exit]
  1: a sunken-faced, starving man, dressed in rags.
  2: It takes a few moments for the man to notice you.
  3: "Hu--humans!"
  4: "Please, you must help me!" he says weakly.
  5: "I need @food, badly!"

[16045 op=1 keyword; failure -> 16047]
  1: look

[16046 op=2 display+exit]
  1: a sunken-faced, starving man, dressed in rags.
  2: "Thou art back!"
  3: "Please, please! @Give me some food!"

[16047 op=1 keyword; failure -> 16049]
  1: name

[16048 op=2 display+exit]
  1: "Ybarra. Ybarra's my name."

[16049 op=1 keyword; failure -> 16051]
  1: job

[16050 op=2 display+exit]
  1: "Lost here, no job now."
  2: "I used to sail under Captain Hawkins on the Empire."
  3: "We had @food then!"
  4: "But those days are gone...."

[16051 op=1 keyword; failure -> 16053]
  1: food

[16052 op=2 display+exit]
  1: "Yes, @food! The last thing I ate was my @belt."
  2: "And that was three weeks ago."
  3: "If only you could @give me some food..."

[16053 op=1 keyword; failure -> 16055]
  1: belt

[16054 op=2 display+exit]
  1: "Not very tasty, but it was filling."

[16055 op=1 keyword; failure -> 16058]
  1: give,spar

[16058 op=1 keyword; failure -> 16061]
  1: map,piec

[16060 op=2 display+exit]
  1: "Map? Map. Map."
  2: "You already got it from me."

[16061 op=1 keyword; failure -> 16064]
  1: map,piec

[16063 op=2 display+exit]
  1: "Map? Map. Map."
  2: "Oh, the map. Yes, I still have that."
  3: "I couldn't eat it."
  4: "It ought to be worth some @food, surely?"

[16064 op=1 keyword; failure -> 16066]
  1: join

[16065 op=2 display+exit]
  1: "Leave here? No!"
  2: "I've almost gotten the tunnels figured out!"
  3: "Just a few more days and I'll be free!"
  4: "But I must have @food!"

[16066 op=1 keyword; failure -> 16069]
  1: bye

[16068 op=2 display+exit]
  1: "Don't go! I need @food... @food..."
  2: "Can I have a little before you go?"

[16069 op=2 display+exit]
  1: "Huh?"

[16143 op=2 display+exit]
  1: Suddenly, you realize that you have no food to give him!
  2: "No @food?" he asks.
  3: "Then thou art doomed too!"

[16149 op=2 display+exit]
  1: He hands you the map piece and rubs his hands, eagerly awaiting his food.
  2: You share some food with the pathetic man.
  3: "Thunff fou," Ybarra says, stuffing the food into his mouth.
  4: After finishing the offered meal, however, Ybarra still seemes starved.

[16152 op=2 display+exit]
  1: You share some food with the pathetic man.
  2: "Thunff fou," Ybarra says, stuffing the food into his mouth.
  3: After finishing the offered meal, however, Ybarra still seemes starved.

```

<a id="yorl-npc-84"></a>
## Yorl ? NPC 84, port 85

Instruction range: 7943?8029

```text
[7943 op=1 keyword; failure -> 7946]
  1: look

[7945 op=2 display+exit]
  1: a huge, imposing man adorned in a leather apron.
  2: "Ah!"
  3: He rumbles up to you with a large, bloody hook.
  4: You feel like bolting, but then he drops it in a pocket on his apron and extends a bloody hand.
  5: He smells strongly of fish.
  6: "What do ya need?"

[7946 op=1 keyword; failure -> 7949]
  1: look

[7948 op=2 display+exit]
  1: a huge, imposing man adorned in a leather apron.
  2: "Good $T, friend."
  3: He extends a huge hand which resembles a slab of meat.
  4: "What do ya need?"

[7949 op=1 keyword; failure -> 7951]
  1: name

[7950 op=2 display+exit]
  1: "Yorl."
  2: "A short name, but a @strong one!"

[7951 op=1 keyword; failure -> 7953]
  1: yorl,stro

[7952 op=2 display+exit]
  1: "My @father was a coastal @raider."
  2: "But don't let that out."
  3: He lets out a holler and laughs loudly.

[7953 op=1 keyword; failure -> 7955]
  1: coas,raid

[7954 op=2 display+exit]
  1: He suddenly stops laughing and looks at you, his eyes angry and wide.
  2: "Who told you?"
  3: With that he throws his head back and laughs again.

[7955 op=1 keyword; failure -> 7957]
  1: fath

[7956 op=2 display+exit]
  1: "Yes, I had one."
  2: He hollers again, this time slapping you on the back.

[7957 op=1 keyword; failure -> 7959]
  1: job

[7958 op=2 display+exit]
  1: "I'm a fisherman."
  2: "'Tisn't quite as exciting as some jobs, but it keeps me near the @sea."

[7959 op=1 keyword; failure -> 7961]
  1: sea

[7960 op=2 display+exit]
  1: "Oh, aye, love the sea I do."
  2: "Even if the @fishing's been poor of late."

[7961 op=1 keyword; failure -> 7963]
  1: fish,buy,sell

[7962 op=2 display+exit]
  1: "Fish come, fish go."
  2: "Right now, the fish have gone."
  3: "But they'll come back!"
  4: "And when they do, I'll be waitin' for 'em with my @nets!"

[7963 op=1 keyword; failure -> 7965]
  1: net

[7964 op=2 display+exit]
  1: "Aye, you haveta use nets to do serious fishing."
  2: "A @pole'll never catch you enough to sell."

[7965 op=1 keyword; failure -> 7967]
  1: pole

[7966 op=2 display+exit]
  1: "Never use 'em myself."
  2: "I stick to nets--faster and easier."
  3: "Man's gotta make a living!"

[7967 op=1 keyword; failure -> 7969]
  1: ghos

[7968 op=2 display+exit]
  1: "That's a @tale they tell in town."
  2: "I've seen no spirits."
  3: "Except at old Stivius'."

[7969 op=1 keyword; failure -> 7971]
  1: tale

[7970 op=2 display+exit]
  1: "You know, 'bout @Marn's pa haunting the @vineyard."
  2: He leans closer wiping his hands on his apron.
  3: "I think @Stivius has been drinkin' his profits."

[7971 op=1 keyword; failure -> 7973]
  1: amul,lock

[7972 op=2 display+exit]
  1: "I took it from Quenton's dead body to give to Marney for safe keeping."

[7973 op=1 keyword; failure -> 7975]
  1: marn

[7974 op=2 display+exit]
  1: "She's the loveliest thing ever seen in these parts."
  2: "I'm her father now."
  3: "And if any man should want her hand, he'd better be able to take me square!"
  4: He gives you a huge grin and flexes a massive bicep.

[7975 op=1 keyword; failure -> 7977]
  1: vine

[7976 op=2 display+exit]
  1: "They say that @Quen's ghost haunts that area of town."
  2: "I can't imagine why."
  3: "The sea was his lifeblood."

[7977 op=1 keyword; failure -> 7979]
  1: stiv

[7978 op=2 display+exit]
  1: He removes a large hook from his pocket and begins to clean it as he talks.
  2: "He's a strange little guy."
  3: "They say he's the only one who saw Quen murdered."
  4: "I wish we'd had a better witness."
  5: He flips the hook over and over examining it.
  6: "Still, those Gargoyles would frighten any man."
  7: "'Specially one as small as him."

[7979 op=1 keyword; failure -> 7981]
  1: quen

[7980 op=2 display+exit]
  1: He pauses for a moment and looks distantly past your shoulder.
  2: "He and I were mates."
  3: "I can still remember the moment @they told me he was dead."
  4: He swallows hard and his eyes become glassy.
  5: ""I miss old Quen."

[7981 op=1 keyword; failure -> 7983]
  1: they

[7982 op=2 display+exit]
  1: "@Gideon and @Stivius."
  2: "They came to my house before dawn and told me that Quen had been killed by @gargoyles."

[7983 op=1 keyword; failure -> 7985]
  1: gide

[7984 op=2 display+exit]
  1: "He owns the Haunting Inn."
  2: "He's good people, though his @wife is a bit of a nag."

[7985 op=1 keyword; failure -> 7987]
  1: wife

[7986 op=2 display+exit]
  1: "She always complains that my house is an @eyesore."
  2: "I tell you if she comes around here, I'll show her what an eyesore really is!"
  3: He slaps you on the back and laughs.

[7987 op=1 keyword; failure -> 7989]
  1: eye,sore

[7988 op=2 display+exit]
  1: "This is a fishhouse."
  2: "It's meant to be an eyesore."
  3: He laughs again.

[7989 op=1 keyword; failure -> 7991]
  1: garg

[7990 op=2 display+exit]
  1: "I never seen a one."
  2: "I seen alot o' things out at sea, @serpents as big as the biggest four-@master."
  3: "But 'nary a single gargoyle."
  4: He looks out towards the sea.

[7991 op=1 keyword; failure -> 7993]
  1: four,mast

[7992 op=2 display+exit]
  1: "One o' the tall ships."
  2: "My father used to pilot one o' the biggest in Lord @British's fleet."
  3: "The Merideth @Ann."
  4: "She was a sight to behold, she was."

[7993 op=1 keyword; failure -> 7995]
  1: Meri,Ann

[7994 op=2 display+exit]
  1: "She'd seen many a battle with pirates."
  2: "Then she went to her grave in a @storm."
  3: "Took nearly two hundred men with her."

[7995 op=1 keyword; failure -> 7997]
  1: grav,stor

[7996 op=2 display+exit]
  1: "Came up all of a sudden."
  2: "I say it was @Mondain's doing."
  3: He pulls the pipe from his mouth and tamps the tobacco with his stout thumb.
  4: "Anyhow, that's when I lost my father."

[7997 op=1 keyword; failure -> 7999]
  1: mond

[7998 op=2 display+exit]
  1: He stops trying to relight his pipe and looks at you.
  2: "Where have you been @scuttled?"
  3: "That's too long a story for this $T."

[7999 op=1 keyword; failure -> 8001]
  1: scut

[8000 op=2 display+exit]
  1: "Never mind."
  2: He sticks his hands in his apron.

[8001 op=1 keyword; failure -> 8003]
  1: serp

[8002 op=2 display+exit]
  1: He turns his head back towards you, purses his lips, then speaks.
  2: "It was a calm night, it was."
  3: "Quen and I had 'cided to go out on that glassy sea for a fish."
  4: "Not just any fish, mind ya, a @light-horn."
  5: He pulls a pipe out of his apron.

[8003 op=1 keyword; failure -> 8005]
  1: ligh,horn

[8004 op=2 display+exit]
  1: "When we were boys, Quen's father often talked about such a fish."
  2: "Said if a body could @catch one, good fortune would be his."
  3: He brings the pipe to his lips and lights it.

[8005 op=1 keyword; failure -> 8007]
  1: catc,good,fort

[8006 op=2 display+exit]
  1: "Yes, good fortune."
  2: "He said the fish had a glowing horn on it's snout."
  3: "It only comes close to the surface when the sea is like @glass."

[8007 op=1 keyword; failure -> 8009]
  1: surf,glas

[8008 op=2 display+exit]
  1: "You see, the fish thought the surface was a mirror and it came up to admire its horn."
  2: He chuckles to himself, teeth clenched around the pipe.
  3: "Anyway, Quen and I 'cided we'd catch us one in our @skiff."

[8009 op=1 keyword; failure -> 8011]
  1: skif

[8010 op=2 display+exit]
  1: He pulls the pipe from his mouth and blows some smoke rings.
  2: "Well, even though the skiff was small, we figured that we'd @spear the beast and drag it back to port."

[8011 op=1 keyword; failure -> 8013]
  1: drag,port,spea

[8012 op=2 display+exit]
  1: "I know it sounds crazy now, but you have to remember, we were only boys."
  2: "Anyway, we rowed out past Ivers @Rounding and sat in our skiff waiting to see one."
  3: He looks out to sea again and chuckles.

[8013 op=1 keyword; failure -> 8015]
  1: iver,roun

[8014 op=2 display+exit]
  1: "You know, past where the @alchemist lives."
  2: "Anyway, we sat there for hours with 'nary a sign of life..."
  3: "Then, suddenly, in the distance we could see a tall @neck rising out of the water and heading towards us."

[8015 op=1 keyword; failure -> 8017]
  1: alch,hora

[8016 op=2 display+exit]
  1: "Old Horance?"
  2: "He's a crazy old coot who lives on the isle."

[8017 op=1 keyword; failure -> 8019]
  1: tall,neck

[8018 op=2 display+exit]
  1: "As it came closer, a huge wave preceded it and we could make out a giant mouth."
  2: He takes his pipe from his mouth and gestures at you.
  3: "I tell you, you've never seen two boys make a skiff travel so fast in all your life."
  4: He laughs out loud, this time bending over to support himself.

[8019 op=1 keyword; failure -> 8021]
  1: tren,bell

[8020 op=2 display+exit]
  1: "He's a stuffy one."
  2: "Never likes to come down here, says it smells."

[8021 op=1 keyword; failure -> 8023]
  1: deza

[8022 op=2 display+exit]
  1: "She's a heavily rigged one, she is."
  2: He winks at you and laughs.

[8023 op=1 keyword; failure -> 8025]
  1: mich

[8024 op=2 display+exit]
  1: "I don't much care for that one."
  2: "He used to be one of Mondain's henchmen."
  3: "I'd plot a course 'round him if I were you."

[8025 op=1 keyword; failure -> 8027]
  1: lord,brit

[8026 op=2 display+exit]
  1: "My father used to sail in one of his fleets."
  2: "A brave and mighty ruler he is."

[8027 op=1 keyword; failure -> 8029]
  1: bye

[8028 op=2 display+exit]
  1: He slaps a hand on your back "Good fortune, and may the wind be at your back."

[8029 op=2 display+exit]
  1: "That doesn't concern me."
  2: "Ask another."

```

<a id="zeke-npc-148"></a>
## Zeke ? NPC 148, port 149

Instruction range: 13487?13579

```text
[13491 op=1 keyword; failure -> 13494]
  1: yes

[13493 op=2 display+exit]
  1: "Good!"
  2: "How many can you spare?"

[13496 op=2 display+exit]
  1: "Changed your mind, eh?"
  2: He folds his arms.

[13497 op=1 keyword; failure -> 13499]
  1: look

[13498 op=2 display+exit]
  1: a strong looking man with just one arm.
  2: "Hello," he shouts, waving.
  3: "you've met @Zeke before, 'aven't ya?" he says, glaring at you
  4: "What d'ya need?"

[13499 op=1 keyword; failure -> 13501]
  1: name

[13500 op=2 display+exit]
  1: "@Zeke the one-armed @beekeep, it is."

[13501 op=1 keyword; failure -> 13503]
  1: zeke

[13502 op=2 display+exit]
  1: "Lost me arm and gained that name in @battle."
  2: He flashes a yellowed grin at you.

[13503 op=1 keyword; failure -> 13505]
  1: batt

[13504 op=2 display+exit]
  1: "'Twas a horrid affair, but @Mondain had to be thwarted."

[13505 op=1 keyword; failure -> 13507]
  1: mond

[13506 op=2 display+exit]
  1: "You must be new here."
  2: He looks to the ground.
  3: "He bought an age of great evil unto our land."
  4: "yet try and tell today's children of the age of darkness..."
  5: "they believe nothing can go wrong, as long as the avatar lives."

[13507 op=1 keyword; failure -> 13509]
  1: bee

[13508 op=2 display+exit]
  1: "Bees are true friends. They work all day, making honey and scattering pollen."
  2: "All we have to do is provide them with @hives."
  3: He laughs. "And get stung once in a while!"

[13509 op=1 keyword; failure -> 13511]
  1: hive

[13510 op=2 display+exit]
  1: "I buys all my hives from a nice lady in Minoc named Michelle."
  2: "She weaves them out of straw."

[13511 op=1 keyword; failure -> 13513]
  1: job

[13512 op=2 display+exit]
  1: "I'm the @beekeeper here at the abbey."
  2: "If you want to @buy @honey, or @sell me a honey jar I'm all ears (if only one-armed)."

[13513 op=1 keyword; failure -> 13515]
  1: bye

[13514 op=2 display+exit]
  1: "Have a nice $T."

[13517 op=1 keyword; failure -> 13527]
  1: sell

[13527 op=1 keyword; failure -> 13531]
  1: sell

[13530 op=2 display+exit]
  1: He turns to you, and says "Will you take $V1 gold for each honey jar?"

[13531 op=1 keyword; failure -> 13533]
  1: sell

[13532 op=2 display+exit]
  1: "Sorry, none of you has anything I need."

[13533 op=1 keyword; failure -> 13536]
  1: buy,hone

[13535 op=2 display+exit]
  1: "It'll cost you 10 gold for the 'oney, interested?"

[13539 op=1 keyword; failure -> 13541]
  1: buy

[13540 op=2 display+exit]
  1: "Come see me when im at me 'ives"

[13541 op=1 keyword; failure -> 13543]
  1: sell

[13542 op=2 display+exit]
  1: "Come see me when im at me 'ives"

[13543 op=2 display+exit]
  1: "He scratches his head and looks around."

[13544 op=1 keyword; failure -> 13550]
  1: yes

[13549 op=2 display+exit]
  1: "Enjoy your 'oney."
  2: After accepting the gold, he hands over the honey jar.

[13552 op=2 display+exit]
  1: He looks at you. "You ain't that good-looking. My price is 10 gold."

[13554 op=2 display+exit]
  1: "You look pretty full to me."

[13555 op=2 display+exit]
  1: "Changed your mind, eh?"
  2: he folds his arms.

[13558 op=2 display+exit]
  1: "Changed your mind, eh?"
  2: He folds his arms.

[13560 op=2 display+exit]
  1: "I only buy in lots of up to a dozen."

[13574 op=2 display+exit]
  1: "Sorry, but none of you have enough jars."

[13578 op=2 display+exit]
  1: "Done!"
  2: He hands you $V1 gold and takes the empty jar.

[13579 op=2 display+exit]
  1: "Done!"
  2: He hands you $V1 gold pieces and takes the empty jars.

```

<a id="zellivan-npc-41"></a>
## Zellivan ? NPC 41, port 42

Instruction range: 3668?3717

```text
[3676 op=2 display+exit]
  1: "Ah, the Avatar! Lord @British has spoken fondly of thee, $G."
  2: "Please feel at home in this town."

[3678 op=1 keyword; failure -> 3680]
  1: look

[3679 op=2 display+exit]
  1: a man with a serpent-and-heart tabard covering his chainmail.
  2: "Good $T. We meet again."

[3680 op=1 keyword; failure -> 3682]
  1: look

[3681 op=2 display+exit]
  1: a man with a serpent-and-heart tabard covering his chainmail.
  2: "I welcome thee to Jhelom."

[3683 op=1 keyword; failure -> 3685]
  1: name

[3684 op=2 display+exit]
  1: "Thou mayst call me Zellivan."

[3685 op=1 keyword; failure -> 3689]
  1: name

[3688 op=2 display+exit]
  1: "I am Zellivan, and I am lord here."
  2: "Pray tell me thy name."

[3689 op=1 keyword; failure -> 3691]
  1: name

[3690 op=2 display+exit]
  1: "Thou mayst call me Zellivan."

[3691 op=1 keyword; failure -> 3693]
  1: job

[3692 op=2 display+exit]
  1: "The @valorous are often boisterous. I watch over the fighters."

[3693 op=1 keyword; failure -> 3695]
  1: rune

[3694 op=2 display+exit]
  1: "I held a tournament, to decide who would be entrusted with the rune's care."
  2: "You might say that '@no @man' was the victor..." He smiles.

[3695 op=1 keyword; failure -> 3697]
  1: mant

[3696 op=2 display+exit]
  1: "Hear it for thyself at the Sword and Keg @Pub, $G."
  2: "The songs and @tales do the heart good, I say!"

[3697 op=1 keyword; failure -> 3699]
  1: tour,vict,no,man,noma

[3698 op=2 display+exit]
  1: "Ask him yourself!"

[3699 op=1 keyword; failure -> 3701]
  1: song,tale,figh

[3700 op=2 display+exit]
  1: "Those who live the warriors' life like to sing,
  2: and to regale one another with fanciful accounts of their exploits."

[3701 op=1 keyword; failure -> 3703]
  1: valo,vict

[3702 op=2 display+exit]
  1: "Here in Jhelom we delight in the clash of @arms and the din of @battle."

[3703 op=1 keyword; failure -> 3705]
  1: arms,armo

[3704 op=2 display+exit]
  1: "Speak to Nomaan if thou hast need of arms."

[3705 op=1 keyword; failure -> 3707]
  1: swor,keg,pub

[3706 op=2 display+exit]
  1: "If thou hast tales of swordsmanship, then the Sword and Keg is the place for thee."
  2: "'Tis on the north side of town."

[3707 op=1 keyword; failure -> 3709]
  1: batt

[3708 op=2 display+exit]
  1: "Ah, battle!" he sighs.
  2: For a time he reminisces with you, trading stories about past exploits.

[3709 op=1 keyword; failure -> 3711]
  1: eart,quak

[3710 op=2 display+exit]
  1: "Only an earthquake could make Jhelom tremble, I daresay."
  2: "We had one just last year."
  3: "The piers were broken up, but the @shipwrights repaired them swiftly."

[3711 op=1 keyword; failure -> 3713]
  1: pier,ship,harb

[3712 op=2 display+exit]
  1: "The shipwrights of Jhelom are fastidious and frugal craftsmen."

[3713 op=1 keyword; failure -> 3715]
  1: lord,brit

[3714 op=2 display+exit]
  1: "I grew up near Castle Britannia."
  2: "'Twas the sunset o'er Brittany Bay
  3: and the ships at rest in the harbor which drew me away to the sea."

[3715 op=1 keyword; failure -> 3717]
  1: bye

[3716 op=2 display+exit]
  1: "Farewell, my friend."

[3717 op=2 display+exit]
  1: "I cannot help thee with that."

```

<a id="zoltan-npc-131"></a>
## Zoltan ? NPC 131, port 132

Instruction range: 11940?12101

```text
[11942 op=2 display+exit]
  1: "Hah! You again! You are no friend to Zoltan."
  2: "I dance for you, and how do you repay me?"
  3: "Perhaps a few coins might ease the sting of your insult."
  4: Do you give him any money?

[11953 op=1 keyword; failure -> 11955]
  1: buy,reag

[11954 op=2 display+exit]
  1: "Which one, @blood @moss, @garlic, @ginseng, @nightshade, @spider @silk or @sulfurous @ash?"

[11955 op=1 keyword; failure -> 11960]
  1: bloo,moss

[11959 op=2 display+exit]
  1: "Blood Moss costs 3 per portion."
  2: "How many do you want?"

[11960 op=1 keyword; failure -> 11965]
  1: garl

[11964 op=2 display+exit]
  1: "Garlic costs 2 per portion."
  2: "How many do you want?"

[11965 op=1 keyword; failure -> 11970]
  1: gins

[11969 op=2 display+exit]
  1: "Ginseng costs 1 per portion."
  2: "How many do you want?"

[11970 op=1 keyword; failure -> 11975]
  1: nigh

[11974 op=2 display+exit]
  1: "Nightshade costs 1 per portion."
  2: "How many do you want?"

[11975 op=1 keyword; failure -> 11980]
  1: spid,silk

[11979 op=2 display+exit]
  1: "Spider silk costs 2 per portion."
  2: "How many do you want?"

[11980 op=1 keyword; failure -> 11985]
  1: sulf,ash

[11984 op=2 display+exit]
  1: "Sulfurous ash costs 3 per portion."
  2: "How many do you want?"

[11989 op=2 display+exit]
  1: "Perhaps something else then."

[11991 op=2 display+exit]
  1: "I can't give you that much"

[11996 op=2 display+exit]
  1: "You can't afford that much!"

[11998 op=2 display+exit]
  1: "You can't carry that much!"

[11999 op=2 display+exit]
  1: "Here you are."

[12000 op=1 keyword; failure -> 12003]
  1: look

[12002 op=2 display+exit]
  1: a lively soul of a gypsy, with a bit of a wild look to him.
  2: "Huzzah! I am still @Zoltan, king of the gypsies!"

[12003 op=1 keyword; failure -> 12007]
  1: look

[12006 op=2 display+exit]
  1: a lively soul of a gypsy, with a bit of a wild look to him.
  2: "Huzzah! I am @Zoltan, king of the gypsies!"

[12007 op=1 keyword; failure -> 12010]
  1: name,job

[12009 op=2 display+exit]
  1: "As I said, I am Zoltan, @king of the @gypsies!"

[12010 op=1 keyword; failure -> 12012]
  1: zolt

[12011 op=2 display+exit]
  1: "Yes, Zoltan, @king of the @gypsies!"

[12012 op=1 keyword; failure -> 12014]
  1: king

[12013 op=2 display+exit]
  1: "I have no lands nor @castles, as most rulers do. Nay, for I am the king of the open @road."

[12014 op=1 keyword; failure -> 12016]
  1: artu

[12015 op=2 display+exit]
  1: "Arturos? Never heard of him."

[12016 op=1 keyword; failure -> 12018]
  1: duck

[12017 op=2 display+exit]
  1: He licks his lips and smiles. "A duck? No, haven't seen any around here."

[12018 op=1 keyword; failure -> 12020]
  1: land,cast

[12019 op=2 display+exit]
  1: "You cannot tie a gypsy down to a single place."
  2: "We are free @spirits, and must roam like the @wind throughout the land."

[12020 op=1 keyword; failure -> 12022]
  1: road

[12021 op=2 display+exit]
  1: "The roads @wander everywhere, just like a gypsy!"
  2: "Just now I am thinking of what you might @buy from me today."

[12022 op=1 keyword; failure -> 12024]
  1: wind

[12023 op=2 display+exit]
  1: "The four winds - they are gypsies too!"

[12024 op=1 keyword; failure -> 12026]
  1: roam,wand

[12025 op=2 display+exit]
  1: "As a gypsy's body wanders, so does his mind."
  2: "Just now I am thinking of what you might @buy from me today."

[12026 op=1 keyword; failure -> 12028]
  1: free,spir

[12027 op=2 display+exit]
  1: "Aye, my spirit is free, but my @advice is not."

[12028 op=1 keyword; failure -> 12030]
  1: gyps

[12029 op=2 display+exit]
  1: "Ask my people for @advice, or what they have to @sell."
  2: "Gypsies see and hear much, and occasionally happen across items of interest..."

[12030 op=1 keyword; failure -> 12033]
  1: advi

[12032 op=2 display+exit]
  1: "Tell you what. Since I'm in a good mood, I'll only charge you two gold coins. Is it a deal?"

[12033 op=1 keyword; failure -> 12036]
  1: silv,tabl

[12035 op=2 display+exit]
  1: "I'd be glad to help you out. But 'tis a hard life, sometimes, being a gypsy."
  2: "You'd be willing to pay ten gold for the information, would you?"

[12036 op=1 keyword; failure -> 12038]
  1: hawk

[12037 op=2 display+exit]
  1: "He's the lowest scum I ever met."

[12038 op=1 keyword; failure -> 12040]
  1: bucc,den

[12039 op=2 display+exit]
  1: "It's no place for an honest soul like you."
  2: "But if you're determined to go, I hear it's on an island due east of @Paws."

[12040 op=1 keyword; failure -> 12042]
  1: paws

[12041 op=2 display+exit]
  1: "We pass Paws often in our travels. @Taynith likes to go drinking there."

[12042 op=1 keyword; failure -> 12044]
  1: lyca

[12043 op=2 display+exit]
  1: "It's on the same island as @Moonglow."

[12044 op=1 keyword; failure -> 12046]
  1: moon

[12045 op=2 display+exit]
  1: "It's on the same island as the @Lycaeum."

[12046 op=1 keyword; failure -> 12048]
  1: mari

[12047 op=2 display+exit]
  1: "She knows how to be generous to poor wandering gypsies. You should follow her example!"

[12048 op=1 keyword; failure -> 12050]
  1: capt,john

[12049 op=2 display+exit]
  1: "Captain John? I talked to him when he gave us the silver tablet for @Mariah."
  2: "He's madder than a gremlin! Claims as he's been living with the gargoyles."
  3: "He talks as if there's nothing evil about them."
  4: "He said the tablet had something to do with translating."

[12050 op=1 keyword; failure -> 12052]
  1: gorg

[12051 op=2 display+exit]
  1: "Either you're a gorgio or my name's not @Zoltan!"

[12052 op=1 keyword; failure -> 12054]
  1: kari,tayn

[12053 op=2 display+exit]
  1: "There are no women like gypsy women!"

[12054 op=1 keyword; failure -> 12057]
  1: bye

[12056 op=2 display+exit]
  1: "Farewell! My fellow gypsies and I will @dance to your health and good fortune!"

[12057 op=1 keyword; failure -> 12059]
  1: bye

[12058 op=2 display+exit]
  1: "Farewell!"

[12059 op=1 keyword; failure -> 12062]
  1: danc

[12061 op=2 display+exit]
  1: @Karina produces a fiddle from somewhere and begins to play a tune at a very high tempo.
  2: Zoltan dances a wild dance for you, leaping and capering about frantically.
  3: His chest glistens with sweat as he walks up to you expectantly.
  4: Do you give him any money?

[12062 op=2 display+exit]
  1: "Why worry yourself with that?"
  2: "Perhaps you'd like to look over some of the fine @reagents I have for sale."

[12063 op=1 keyword; failure -> 12065]
  1: yes

[12066 op=2 display+exit]
  1: "Whatever you say."

[12071 op=2 display+exit]
  1: "Come back when you're not so down on your luck."

[12073 op=2 display+exit]
  1: "I'll tell you this:"
  2: "Powder kegs are useful for persuading uncooperative doors to open."

[12074 op=1 keyword; failure -> 12076]
  1: yes

[12077 op=2 display+exit]
  1: "Let me know if you change your mind."

[12082 op=2 display+exit]
  1: "Come back when you have the money."

[12083 op=2 display+exit]
  1: "Some @gorgio called 'Captain @John' brought us the silver tablet and paid us to take it to the Lycaeum."
  2: "While we were on the way there, we were ambushed by Captain @Hawkins and his crew."
  3: "The tablet was broken in two during the scuffle. The pirates got away with the bigger half."
  4: "Hawkins took my finest earring as well - I spit on his mother's grave!"
  5: "Most likely they went to Buccaneer's @Den with their loot."
  6: "We took the corner of the tablet that they missed to the @Lycaeum and sold it to @Mariah."

[12084 op=1 keyword; failure -> 12087]
  1: yes

[12086 op=2 display+exit]
  1: How much do you give?

[12088 op=2 display+exit]
  1: Hah! I should know better than to expect a gorgio to appreciate a fine gypsy dance."
  2: Proudly, he turns his back on you.

[12094 op=2 display+exit]
  1: You don't have that much.
  2: How much do you give?

[12096 op=2 display+exit]
  1: Zoltan pockets the money. "Thanks."

[12097 op=1 keyword; failure -> 12100]
  1: yes

[12099 op=2 display+exit]
  1: How much do you give?

[12101 op=2 display+exit]
  1: "So be it. Leave me before I place a gypsy curse on you!"

```
