
#ifndef __CK4_GFX__
#define __CK4_GFX__

// Enum generated from GFXE_CK4.H and mkgfxh.py
typedef enum {
    PADDLESPR = 0,
	BALLSPR = 1,
	BALL1PIXELTOTHERIGHTSPR = 2,
	BALL2PIXELSTOTHERIGHTSPR = 3,
	BALL3PIXELSTOTHERIGHTSPR = 4,
	DEMOPLAQUESPR = 0,
	KEENSTANDRSPR = 0,
	KEENRUNR1SPR = 1,
	KEENRUNR2SPR = 2,
	KEENRUNR3SPR = 3,
	KEENRUNR4SPR = 4,
	KEENJUMPR1SPR = 5,
	KEENJUMPR2SPR = 6,
	KEENJUMPR3SPR = 7,
	KEENSTANDLSPR = 8,
	KEENRUNL1SPR = 9,
	KEENRUNL2SPR = 10,
	KEENRUNL3SPR = 11,
	KEENRUNL4SPR = 12,
	KEENJUMPL1SPR = 13,
	KEENJUMPL2SPR = 14,
	KEENJUMPL3SPR = 15,
	KEENLOOKUSPR = 16,
	KEENWAITR1SPR = 17,
	KEENWAITR2SPR = 18,
	KEENWAITR3SPR = 19,
	KEENSITREAD1SPR = 20,
	KEENSITREAD2SPR = 21,
	KEENSITREAD3SPR = 22,
	KEENSITREAD4SPR = 23,
	KEENREAD1SPR = 24,
	KEENREAD2SPR = 25,
	KEENREAD3SPR = 26,
	KEENSTOPREAD1SPR = 27,
	KEENSTOPREAD2SPR = 28,
	KEENWATCHSPR = 29,
	KEENLOOKD1SPR = 30,
	KEENLOOKD2SPR = 31,
	KEENDIE1SPR = 32,
	KEENDIE2SPR = 33,
	STUNSTARS1SPR = 34,
	STUNSTARS2SPR = 35,
	STUNSTARS3SPR = 36,
	KEENSHOOTLSPR = 37,
	KEENJLSHOOTLSPR = 38,
	KEENJSHOOTDSPR = 39,
	KEENJSHOOTUSPR = 40,
	KEENSHOOTUSPR = 41,
	KEENSHOOTRSPR = 42,
	KEENJRSHOOTRSPR = 43,
	STUN1SPR = 44,
	STUN2SPR = 45,
	STUN3SPR = 46,
	STUN4SPR = 47,
	STUNHIT1SPR = 48,
	STUNHIT2SPR = 49,
	KEENSHINNYR1SPR = 50,
	KEENSHINNYR2SPR = 51,
	KEENSHINNYR3SPR = 52,
	KEENSLIDED1SPR = 53,
	KEENSLIDED2SPR = 54,
	KEENSLIDED3SPR = 55,
	KEENSLIDED4SPR = 56,
	KEENSHINNYL1SPR = 57,
	KEENSHINNYL2SPR = 58,
	KEENSHINNYL3SPR = 59,
	KEENPLSHOOTUSPR = 60,
	KEENPRSHOOTUSPR = 61,
	KEENPRSHOOTDSPR = 62,
	KEENPLSHOOTDSPR = 63,
	KEENPSHOOTLSPR = 64,
	KEENPSHOOTRSPR = 65,
	KEENENTER1SPR = 66,
	KEENENTER2SPR = 67,
	KEENENTER3SPR = 68,
	KEENENTER4SPR = 69,
	KEENENTER5SPR = 70,
	KEENHANGLSPR = 71,
	KEENHANGRSPR = 72,
	KEENCLIMBEDGEL1SPR = 73,
	KEENCLIMBEDGEL2SPR = 74,
	KEENCLIMBEDGEL3SPR = 75,
	KEENCLIMBEDGEL4SPR = 76,
	KEENCLIMBEDGER1SPR = 77,
	KEENCLIMBEDGER2SPR = 78,
	KEENCLIMBEDGER3SPR = 79,
	KEENCLIMBEDGER4SPR = 80,
	KEENPOGOR1SPR = 81,
	KEENPOGOR2SPR = 82,
	KEENPOGOL1SPR = 83,
	KEENPOGOL2SPR = 84,
	DROPSPLASH1SPR = 85,
	DROPSPLASH2SPR = 86,
	DROPSPLASH3SPR = 87,
	BONUS100UPSPR = 88,
	BONUS100SPR = 89,
	BONUS200SPR = 90,
	BONUS500SPR = 91,
	BONUS1000SPR = 92,
	BONUS2000SPR = 93,
	BONUS5000SPR = 94,
	BONUS1UPSPR = 95,
	BONUSCLIPSPR = 96,
	SUGAR1ASPR = 0,
	SUGAR1BSPR = 1,
	SUGAR2ASPR = 0,
	SUGAR2BSPR = 1,
	SUGAR3ASPR = 0,
	SUGAR3BSPR = 1,
	SUGAR4ASPR = 0,
	SUGAR4BSPR = 1,
	SUGAR5ASPR = 0,
	SUGAR5BSPR = 1,
	SUGAR6ASPR = 0,
	SUGAR6BSPR = 1,
	ONEUPASPR = 0,
	ONEUPBSPR = 1,
	DOORSPR = 0,
	REDGEM1SPR = 0,
	REDGEM2SPR = 1,
	YELLOWGEM1SPR = 2,
	YELLOWGEM2SPR = 3,
	BLUEGEM1SPR = 4,
	BLUEGEM2SPR = 5,
	GREENGEM1SPR = 6,
	GREENGEM2SPR = 7,
	BONUSGEMSPR = 8,
	STUNCLIP1SPR = 0,
	STUNCLIP2SPR = 1,
	SCOREBOXSPR = 0,
	WORLDKEENL1SPR = 0,
	WORLDKEENL2SPR = 1,
	WORLDKEENL3SPR = 2,
	WORLDKEENR1SPR = 3,
	WORLDKEENR2SPR = 4,
	WORLDKEENR3SPR = 5,
	WORLDKEENU1SPR = 6,
	WORLDKEENU2SPR = 7,
	WORLDKEENU3SPR = 8,
	WORLDKEEND1SPR = 9,
	WORLDKEEND2SPR = 10,
	WORLDKEEND3SPR = 11,
	WORLDKEENDR1SPR = 12,
	WORLDKEENDR2SPR = 13,
	WORLDKEENDR3SPR = 14,
	WORLDKEENDL1SPR = 15,
	WORLDKEENDL2SPR = 16,
	WORLDKEENDL3SPR = 17,
	WORLDKEENUL1SPR = 18,
	WORLDKEENUL2SPR = 19,
	WORLDKEENUL3SPR = 20,
	WORLDKEENUR1SPR = 21,
	WORLDKEENUR2SPR = 22,
	WORLDKEENUR3SPR = 23,
	WORLDKEENWAVE1SPR = 24,
	WORLDKEENWAVE2SPR = 25,
	WORLDKEENSWIMU1SPR = 26,
	WORLDKEENSWIMU2SPR = 27,
	WORLDKEENSWIMR1SPR = 28,
	WORLDKEENSWIMR2SPR = 29,
	WORLDKEENSWIMD1SPR = 30,
	WORLDKEENSWIMD2SPR = 31,
	WORLDKEENSWIML1SPR = 32,
	WORLDKEENSWIML2SPR = 33,
	WORLDKEENSWIMUR1SPR = 34,
	WORLDKEENSWIMUR2SPR = 35,
	WORLDKEENSWIMDR1SPR = 36,
	WORLDKEENSWIMDR2SPR = 37,
	WORLDKEENSWIMDL1SPR = 38,
	WORLDKEENSWIMDL2SPR = 39,
	WORLDKEENSWIMUL1SPR = 40,
	WORLDKEENSWIMUL2SPR = 41,
	WOLRDKEENRIDE1SPR = 42,
	WOLRDKEENRIDE2SPR = 43,
	FLAGFLIP1SPR = 44,
	FLAGFLIP2SPR = 45,
	FLAGFLIP3SPR = 46,
	FLAGFLIP4SPR = 47,
	FLAGFLIP5SPR = 48,
	FLAGFALL1SPR = 49,
	FLAGFALL2SPR = 50,
	FLAGFLAP1SPR = 51,
	FLAGFLAP2SPR = 52,
	FLAGFLAP3SPR = 53,
	FLAGFLAP4SPR = 54,
	SCUBAKEENL1SPR = 0,
	SCUBAKEENL2SPR = 1,
	SCUBAKEENR1SPR = 2,
	SCUBAKEENR2SPR = 3,
	SCUBAKEENDEAD1SPR = 4,
	SCUBAKEENDEAD2SPR = 5,
	SLUGWALKR1SPR = 0,
	SLUGWALKR2SPR = 1,
	SLUGPISSRSPR = 2,
	SLUGSTUN1SPR = 3,
	SLUGSTUN2SPR = 4,
	SLUGWALKL1SPR = 5,
	SLUGWALKL2SPR = 6,
	SLUGPISSLSPR = 7,
	SLUGSLIME1SPR = 8,
	SLUGSLIME2SPR = 9,
	MADMUSHROOML1SPR = 0,
	MADMUSHROOML2SPR = 1,
	MADMUSHROOMR1SPR = 2,
	MADMUSHROOMR2SPR = 3,
	LINDSEY1SPR = 0,
	LINDSEY2SPR = 1,
	LINDSEY3SPR = 2,
	LINDSEY4SPR = 3,
	INCHWORMR1SPR = 0,
	INCHWORMR2SPR = 1,
	INCHWORML1SPR = 2,
	INCHWORML2SPR = 3,
	FOOTSPR = 4,
	EATERSTAND1SPR = 0,
	EATERSTAND2SPR = 1,
	EATERJUMPR1SPR = 2,
	EATERJUMPR2SPR = 3,
	EATERJUMPR3SPR = 4,
	EATERJUMPL1SPR = 5,
	EATERJUMPL2SPR = 6,
	EATERJUMPL3SPR = 7,
	EATENBONUS1SPR = 8,
	EATENBONUS2SPR = 9,
	EATENBONUS3SPR = 10,
	EATENBONUS4SPR = 11,
	SMOKE1SPR = 12,
	SMOKE2SPR = 13,
	SMOKE3SPR = 14,
	SMOKE4SPR = 15,
	SMOKE5SPR = 16,
	EATERSTUNSPR = 17,
	COUNCILWALKR1SPR = 0,
	COUNCILWALKR2SPR = 1,
	COUNCILWALKL1SPR = 2,
	COUNCILWALKL2SPR = 3,
	COUNCILTHINKLSPR = 4,
	COUNCILTHINKRSPR = 5,
	EGGSPR = 0,
	EGGBROKESPR = 1,
	EGGCHIP1SPR = 2,
	EGGCHIP2SPR = 3,
	EGGCHIP3SPR = 4,
	BIRDWALKR1SPR = 0,
	BIRDWALKR2SPR = 1,
	BIRDWALKR3SPR = 2,
	BIRDWALKR4SPR = 3,
	BIRDWALKL1SPR = 4,
	BIRDWALKL2SPR = 5,
	BIRDWALKL3SPR = 6,
	BIRDWALKL4SPR = 7,
	BIRDFLY1SPR = 8,
	BIRDFLY2SPR = 9,
	BIRDFLY3SPR = 10,
	BIRDFLY4SPR = 11,
	BIRDSTUNSPR = 12,
	DARTU1SPR = 0,
	DARTU2SPR = 1,
	DARTD1SPR = 2,
	DARTD2SPR = 3,
	DARTR1SPR = 4,
	DARTR2SPR = 5,
	DARTL1SPR = 6,
	DARTL2SPR = 7,
	MIMROCKSPR = 0,
	MIMROCKWALKL1SPR = 1,
	MIMROCKWALKL2SPR = 2,
	MIMROCKWALKL3SPR = 3,
	MIMROCKWALKL4SPR = 4,
	MIMROCKWALKR1SPR = 5,
	MIMROCKWALKR2SPR = 6,
	MIMROCKWALKR3SPR = 7,
	MIMROCKWALKR4SPR = 8,
	MIMROCKJUMPR1SPR = 9,
	MIMROCKJUMPR2SPR = 10,
	MIMROCKJUMPR3SPR = 11,
	MIMROCKJUMPL1SPR = 12,
	MIMROCKJUMPL2SPR = 13,
	MIMROCKJUMPL3SPR = 14,
	MINROCKSTUNSPR = 15,
	DOPEFISHSWIMR1SPR = 0,
	DOPEFISHSWIMR2SPR = 1,
	DOPEFISHHUNGRYRSPR = 2,
	DOPEFISHBURP1SPR = 3,
	DOPEFISHBURP2SPR = 4,
	BIGBUBBLE1SPR = 5,
	BIGBUBBLE2SPR = 6,
	BIGBUBBLE3SPR = 7,
	BIGBUBBLE4SPR = 8,
	SMALLBUBBLE1SPR = 9,
	SMALLBUBBLE2SPR = 10,
	SMALLBUBBLE3SPR = 11,
	SMALLBUBBLE4SPR = 12,
	MEDIUMBUBBLESPR = 13,
	DOPEFISHSWIML1SPR = 14,
	DOPEFISHSWIML2SPR = 15,
	DOPEFISHHUNGRYLSPR = 16,
	SCHOOLFISHL1SPR = 0,
	SCHOOLFISHL2SPR = 1,
	SCHOOLFISHR1SPR = 2,
	SCHOOLFISHR2SPR = 3,
	ARACHNUTWALK1SPR = 0,
	ARACHNUTWALK2SPR = 1,
	ARACHNUTWALK3SPR = 2,
	ARACHNUTWALK4SPR = 3,
	ARACHNUTSTUNSPR = 4,
	SCUBASPR = 0,
	SPRITEFLOATSPR = 0,
	SPRITEAIMLSPR = 1,
	SPRITESHOOTLSPR = 2,
	SPRITEAIMRSPR = 3,
	SPRITESHOOTRSPR = 4,
	SPRITESHOT1SPR = 5,
	SPRITESHOT2SPR = 6,
	SPRITESHOT3SPR = 7,
	SPRITESHOT4SPR = 8,
	MINESPR = 0,
	MINEEXPLODE1SPR = 1,
	MINEEXPLODE2SPR = 2,
	SKYPESTFLYL1SPR = 0,
	SKYPESTFLYL2SPR = 1,
	SKYPESTFLYR1SPR = 2,
	SKYPESTFLYR2SPR = 3,
	SKYPESTSIT1SPR = 4,
	SKYPESTSIT2SPR = 5,
	SKYPESTSIT3SPR = 6,
	SKYPESTSIT4SPR = 7,
	SKYPESTSIT5SPR = 8,
	SKYPESTSIT6SPR = 9,
	SKYPESTSIT7SPR = 10,
	SKYPESTSIT8SPR = 11,
	SKYPESTSIT9SPR = 12,
	SKYPESTSQUASHEDSPR = 13,
	WORMOUTHSPR = 0,
	WORMOUTHPEEKR1SPR = 1,
	WORMOUTHPEEKR2SPR = 2,
	WORMOUTHPEEKL1SPR = 3,
	WORMOUTHPEEKL2SPR = 4,
	WORMOUTHBITER1SPR = 5,
	WORMOUTHBITER2SPR = 6,
	WORMOUTHBITER3SPR = 7,
	WORMOUTHBITEL1SPR = 8,
	WORMOUTHBITEL2SPR = 9,
	WORMOUTHBITEL3SPR = 10,
	WORMOUTHSTUNSPR = 11,
	LICKMOVER1SPR = 0,
	LICKMOVER2SPR = 1,
	LICKMOVER3SPR = 2,
	LICKMOVER4SPR = 3,
	LICKMOVEL1SPR = 4,
	LICKMOVEL2SPR = 5,
	LICKMOVEL3SPR = 6,
	LICKMOVEL4SPR = 7,
	LICKATTACKR1SPR = 8,
	LICKATTACKR2SPR = 9,
	LICKATTACKR3SPR = 10,
	LICKATTACKL1SPR = 11,
	LICKATTACKL2SPR = 12,
	LICKATTACKL3SPR = 13,
	LICKSTUNSPR = 14,
	PLATFORMSPR = 0,
	PLATSIDETHRUST1SPR = 1,
	PLATSIDETHRUST2SPR = 2,
	PLATRTHRUST1SPR = 3,
	PLATRTHRUST2SPR = 4,
	PLATLTHRUST1SPR = 5,
	PLATLTHRUST2SPR = 6,
	BOUNDERL1SPR = 0,
	BOUNDERL2SPR = 1,
	BOUNDERR1SPR = 2,
	BOUNDERR2SPR = 3,
	BOUNDERC1SPR = 4,
	BOUNDERC2SPR = 5,
	BOUNDERSTUNSPR = 6,
	CLOUDSPR = 0,
	CLOUDACTIVESPR = 1,
	CLOUDCHARGESPR = 2,
	BOLT1SPR = 3,
	BOLT2SPR = 4,
	BERKEWALKL1SPR = 0,
	BERKEWALKL2SPR = 1,
	BERKEWALKL3SPR = 2,
	BERKEWALKL4SPR = 3,
	BERKEWALKR1SPR = 4,
	BERKEWALKR2SPR = 5,
	BERKEWALKR3SPR = 6,
	BERKEWALKR4SPR = 7,
	BERKETHROWL1SPR = 8,
	BERKETHROWL2SPR = 9,
	BERKETHROWR1SPR = 10,
	BERKETHROWR2SPR = 11,
	FIREBALL1SPR = 12,
	FIREBALL2SPR = 13,
	FIREBALL3SPR = 14,
	FIREBALL4SPR = 15,
	KEENMOON1SPR = 0,
	KEENMOON2SPR = 1,

}graphicnums;

#endif