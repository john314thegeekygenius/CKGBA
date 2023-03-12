
#ifndef __CK4_DEFS__
#define __CK4_DEFS__



// K4_DEF.H stuff

// ???
#define GAMELEVELS 25

// TODO:
// Make this correct
#define STARPALETTE {}

#define TERMPALETTE { 0x0, 0x1, 0x2, 0x9, 0x11, 0x5, 0x6, 0x8, 0x7, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf}

#define WORLDMAPNAME "Shadowlands"
#define DROPSNAME "DROPS"

// TODO:
// Make this correct
#define HIGHSCORE_LEFT	24
#define HIGHSCORE_TOP	51
#define HIGHSCORE_RIGHT	296
#define HIGHSCORE_MAP	19

#define STARWARSMUSIC	12
#define ENDINGMUSIC	7

// levels in this range can NOT be re-entered (BWB level should be > MAXDONELEVEL)
#define MINDONELEVEL 1
#define MAXDONELEVEL 17

#define INACTIVATEDIST 4

// Graphics Stuff

#include "CK4_GFX.h"

/*
=============================================================================

						K4_SPEC DEFINITIONS

=============================================================================
*/

extern const char *levelnames[GAMELEVELS];
extern const char *levelenter[GAMELEVELS];

void ScanInfoPlane(void);
void PrincessLindsey(void);
void RescueJanitor(void);
void CantSwim(void);
void GotScuba(void);
void RescuedMember(void);

extern statetype s_keenswimslow1;
extern statetype s_keenswimslow2;
extern statetype s_keenswim1;
extern statetype s_keenswim2;
extern statetype s_kbubble1;
extern statetype s_kbubble2;
extern statetype s_kbubble3;
extern statetype s_kbubble4;
void SpawnSwimKeen(Sint16 x, Sint16 y);
void SpawnKbubble(objtype *ob);
void T_KeenSwimSlow(objtype *ob);
void T_KeenSwim(objtype *ob);
void C_KeenSwim(objtype *ob, objtype *hit);
void R_KeenSwim(objtype *ob);

/*
=============================================================================

						K4_ACT1 DEFINITIONS

=============================================================================
*/

extern const statetype s_miragia0;
extern const statetype s_miragia1;
extern const statetype s_miragia2;
extern const statetype s_miragia3;
extern const statetype s_miragia4;
extern const statetype s_miragia5;
extern const statetype s_miragia6;
extern const statetype s_miragia7;
void SpawnMiragia(Sint16 x, Sint16 y);
void T_Miragia0(objtype *ob);
void T_Miragia1(objtype *ob);
void T_Miragia2(objtype *ob);
void T_Miragia3(objtype *ob);
void T_Miragia4(objtype *ob);
void T_Miragia5(objtype *ob);
void T_Miragia6(objtype *ob);
void T_Miragia7(objtype *ob);

extern const statetype s_bonus1;
extern const statetype s_bonus2;
extern const statetype s_bonusrise;
extern const statetype s_splash1;
extern const statetype s_splash2;
extern const statetype s_splash3;
extern const Uint16 bonusshape[];
extern const CK_SpriteType CK_BonusShadows[];
void SpawnBonus(Sint16 x, Sint16 y, Sint16 type);
void SpawnSplash(Sint16 x, Sint16 y);
void T_Bonus(objtype *ob);

extern const statetype s_councilwalk1;
extern const statetype s_councilwalk2;
extern const statetype s_councilstand;
void SpawnCouncil(Sint16 x, Sint16 y);
void T_Council(objtype *ob);

extern const statetype s_slugwalk1;
extern const statetype s_slugwalk2;
extern const statetype s_slugpiss1;
extern const statetype s_slugstun;
extern const statetype s_slugstunalt;
extern const statetype s_slugslime;
extern const statetype s_slugslime2;
void SpawnSlug(Sint16 x, Sint16 y);
void T_Slug(objtype *ob);
void T_SlugPiss(objtype *ob);
void C_Slug(objtype *ob, objtype *hit);

extern const statetype s_mushroom1;
extern const statetype s_mushroom2;
void SpawnMadMushroom(Sint16 x, Sint16 y);
void T_Mushroom(objtype *ob);
void C_Mushroom(objtype *ob, objtype *hit);
void R_Mushroom(objtype *ob);

extern const statetype s_egg;
extern const statetype s_eggbroke;
extern const statetype s_eggchip1;
extern const statetype s_eggchip2;
extern const statetype s_eggchip3;
extern const statetype s_eggbirdpause;
extern const statetype s_eggbirdwalk1;
extern const statetype s_eggbirdwalk2;
extern const statetype s_eggbirdwalk3;
extern const statetype s_eggbirdwalk4;
extern const statetype s_eggbirdfly1;
extern const statetype s_eggbirdfly2;
extern const statetype s_eggbirdfly3;
extern const statetype s_eggbirdfly4;
extern const statetype s_eggbirddrop;
extern const statetype s_eggbirdstun;
extern const statetype s_eggbirdstun2;
extern const statetype s_eggbirdstun3;
extern const statetype s_eggbirdstun4;
extern const statetype s_eggbirdstun5;
void SpawnEggbird(Sint16 x, Sint16 y);
void T_EggUnstun(objtype *ob);
void SpawnEggbirdOut(Sint16 x, Sint16 y);
void C_Egg(objtype *ob, objtype *hit);
void T_Eggbird(objtype *ob);
void T_EggbirdFly(objtype *ob);
void C_Eggbird(objtype *ob, objtype *hit);
void C_EggbirdStun(objtype *ob, objtype *hit);
void R_Eggbird(objtype *ob);
void R_EggbirdDrop(objtype *ob);
void R_Chip(objtype *ob);
void R_Eggbirdfly(objtype *ob);

extern const statetype s_arach1;
extern const statetype s_arach2;
extern const statetype s_arach3;
extern const statetype s_arach4;
extern const statetype s_arachstun;
extern const statetype s_arachstun2;
extern const statetype s_arachstun3;
extern const statetype s_arachstun4;
extern const statetype s_arachstun5;
void SpawnArachnut(Sint16 x, Sint16 y);
void T_Arach(objtype *ob);
void C_Arach(objtype *ob, objtype *hit);
void C_ArachStun(objtype *ob, objtype *hit);

extern const statetype s_pestfly1;
extern const statetype s_pestfly2;
extern const statetype s_squashedpest;
extern const statetype s_pestrest1;
extern const statetype s_pestrest2;
extern const statetype s_pestrest3;
extern const statetype s_pestrest4;
extern const statetype s_pestrest5;
extern const statetype s_pestrest6;
extern const statetype s_pestrest7;
extern const statetype s_pestrest8;
extern const statetype s_pestrest9;
extern const statetype s_pestrest10;
extern const statetype s_pestrest11;
extern const statetype s_pestrest12;
extern const statetype s_pestrest13;
extern const statetype s_pestrest14;
extern const statetype s_pestrest15;
extern const statetype s_pestrest16;
extern const statetype s_pestrest17;
void SpawnSkypest(Sint16 x, Sint16 y);
void T_PestFly(objtype *ob);
void C_PestFly(objtype *ob, objtype *hit);
void C_Squashable(objtype *ob, objtype *hit);
void T_PestRest(objtype *ob);
void R_Pest(objtype *ob);

/*
=============================================================================

						K4_ACT2 DEFINITIONS

=============================================================================
*/

extern const statetype s_worm;
extern const statetype s_wormpeek1;
extern const statetype s_wormpeek2;
extern const statetype s_wormpeek3;
extern const statetype s_wormpeek4;
extern const statetype s_wormpeek5;
extern const statetype s_wormpeek6;
extern const statetype s_wormpeek7;
extern const statetype s_wormpeek8;
extern const statetype s_wormbite1;
extern const statetype s_wormbite2;
extern const statetype s_wormbite3;
extern const statetype s_wormbite4;
extern const statetype s_wormbite5;
extern const statetype s_wormstun;
void SpawnWormMouth(Sint16 x, Sint16 y);
void T_WormLookRight(objtype *ob);
void T_WormLook(objtype *ob);
void T_WormLookLeft(objtype *ob);
void T_Worm(objtype *ob);
void C_Worm(objtype *ob, objtype *hit);
void C_WormKill(objtype *ob, objtype *hit);

extern const statetype s_cloudsleep;
extern const statetype s_cloudwake;
extern const statetype s_cloud;
extern const statetype s_cloudalign;
extern const statetype s_cloudcharge;
extern const statetype s_cloudattack1;
extern const statetype s_cloudattack2;
extern const statetype s_cloudattack3;
extern const statetype s_cloudattack4;
extern const statetype s_cloudattack5;
extern const statetype s_cloudattack6;
extern const statetype s_cloudattack7;
extern const statetype s_cloudattack8;
extern const statetype s_cloudattack9;
extern const statetype s_bolt1;
extern const statetype s_bolt2;
extern const statetype s_bolt3;
extern const statetype s_bolt4;
extern const statetype s_bolt5;
extern const statetype s_bolt6;
void SpawnCloudster(Sint16 x, Sint16 y);
void T_Cloud(objtype *ob);
void T_CloudAlign(objtype *ob);
void R_Cloud(objtype *ob);
void T_CloudShoot(objtype *ob);
void C_CloudSleep(objtype *ob, objtype *hit);

extern const statetype s_berkefloat1;
extern const statetype s_berkefloat2;
extern const statetype s_berkefloat3;
extern const statetype s_berkefloat4;
extern const statetype s_berkethrow1;
extern const statetype s_berkethrow2;
extern const statetype s_berkethrow3;
extern const statetype s_berkethrow4;
extern const statetype s_berkethrow5;
extern const statetype s_berkethrow6;
extern const statetype s_berkethrow7;
extern const statetype s_berkethrow8;
extern const statetype s_berkethrow9;
extern const statetype s_berkethrow10;
extern const statetype s_berkethrow11;
extern const statetype s_berkethrow12;
extern const statetype s_fire1;
extern const statetype s_fire2;
extern const statetype s_fireland1;
extern const statetype s_fireland2;
extern const statetype s_fireland3;
extern const statetype s_fireland4;
extern const statetype s_fireland5;
extern const statetype s_fireland6;
extern const statetype s_fireland7;
extern const statetype s_fireland8;
extern const statetype s_fireland9;
void SpawnBerkeloid(Sint16 x, Sint16 y);
void BerkeThink(objtype *ob);
void BerkeThrowThink(objtype *ob);
void BerkeThrowDone(objtype *ob);
void C_Berke(objtype *ob, objtype *hit);
void FireReact(objtype *ob);
void BerkeDrawReact(objtype *ob);
void BerkeWalkReact(objtype *ob);

extern const statetype s_footsmoke1;
extern const statetype s_footsmoke2;
extern const statetype s_footsmoke3;
extern const statetype s_footsmoke4;
extern const statetype s_inch1;
extern const statetype s_inch2;
extern const statetype s_footchange;	//never used!
extern const statetype s_footwait;
void SpawnInchworm(Sint16 x, Sint16 y);
void SpawnFoot(Sint16 x, Sint16 y);
void InchThink(objtype *ob);
void InchContact(objtype *ob, objtype *hit);
void FootContact(objtype *ob, objtype *hit);


extern const statetype s_bounderup1;
extern const statetype s_bounderup2;
extern const statetype s_bounderside1;
extern const statetype s_bounderside2;
extern const statetype s_bounderstun;
extern const statetype s_bounderstun2;
void SpawnBounder(Sint16 x, Sint16 y);
void C_Bounder(objtype *ob, objtype *hit);
void R_Bounder(objtype *ob);

extern const statetype s_lick1;
extern const statetype s_lick2;
extern const statetype s_lick3;
extern const statetype s_lick4;
extern const statetype s_licklick1;
extern const statetype s_licklick2;
extern const statetype s_licklick3;
extern const statetype s_licklick4;
extern const statetype s_licklick5;
extern const statetype s_licklick6;
extern const statetype s_licklick7;
extern const statetype s_licklick8;
extern const statetype s_lickstun;
extern const statetype s_lickstun2;
void SpawnLick(Sint16 x, Sint16 y);
void LickJumpThink(objtype *ob);
void LickContact(objtype *ob, objtype *hit);
void LickKillContact(objtype *ob, objtype *hit);
void LickAirReact(objtype *ob);

extern const statetype s_platform;
void SpawnPlatform(Sint16 x, Sint16 y, Sint16 dir);
void T_Platform(objtype *ob);
void R_Platform(objtype *ob);

extern const statetype s_dropplatsit;
extern const statetype s_dropplatfall;
extern const statetype s_dropplatrise;
void SpawnDropPlat(Sint16 x, Sint16 y);
void T_DropPlatSit(objtype *ob);
void T_DropPlatFall(objtype *ob);
void T_DropPlatRise(objtype *ob);


/*
=============================================================================

						K4_ACT3 DEFINITIONS

=============================================================================
*/

extern const statetype s_eaterstand1;
extern const statetype s_eaterstand2;
extern const statetype s_eatertport1;
extern const statetype s_eatertport2;
extern const statetype s_eatertport3;
extern const statetype s_eatertport4;
extern const statetype s_eatertport5;
extern const statetype s_eatertport6;
extern const statetype s_eatertport7;
extern const statetype s_eatertport8;
extern const statetype s_eaterjump1;
extern const statetype s_eaterjump2;
extern const statetype s_eaterjump3;
extern const statetype s_eaterjump4;
extern const statetype s_eaterstun;
extern const statetype s_eaterstun2;
extern const statetype s_eatenbonus1;
extern const statetype s_eatenbonus2;
extern const statetype s_eatenbonus3;
extern const statetype s_eatenbonus4;
void SpawnEater(Sint16 x, Sint16 y);
void T_EaterJump(objtype *ob);
void T_EaterTeleport(objtype *ob);
void C_Eater(objtype *ob, objtype *hit);
void EaterInTile(objtype *ob);
void R_EaterAir(objtype *ob);

extern const statetype s_mimrock;
extern const statetype s_mimsneak1;
extern const statetype s_mimsneak2;
extern const statetype s_mimsneak3;
extern const statetype s_mimsneak4;
extern const statetype s_mimsneak5;
extern const statetype s_mimsneak6;
extern const statetype s_mimbonk1;
extern const statetype s_mimbonk2;
extern const statetype s_mimbonk3;
extern const statetype s_mimbounce;
extern const statetype s_mimstun;
extern const statetype s_mimstun2;
void SpawnMimrock(Sint16 x, Sint16 y);
void T_MimrockWait(objtype *ob);
void T_MimrockSneak(objtype *ob);
void C_Mimrock(objtype *ob, objtype *hit);
void C_MimLethal(objtype *ob, objtype *hit);
void R_MimAir(objtype *ob);
void R_MimBounce(objtype *ob);

extern const statetype s_dopefish1;
extern const statetype s_dopefish2;
extern const statetype s_dopeattack;
extern const statetype s_dopeeat;
extern const statetype s_dopeburp1;
extern const statetype s_dopeburp2;
extern const statetype s_dopereturn;
extern const statetype s_dopefood;
extern const statetype s_keendopefood;
extern const statetype s_keendieslow;
extern const statetype s_bubble1;
extern const statetype s_bubble2;
extern const statetype s_bubble3;
extern const statetype s_bubble4;
void SpawnDopefish(Sint16 x, Sint16 y);
void T_EatenKeen(objtype *ob);
void T_Dope(objtype *ob);
void T_DopeHunt(objtype *ob);
void T_DopeReturn(objtype *ob);
void T_Burp(objtype *ob);
void T_Bubble(objtype *ob);
void C_Dope(objtype *ob, objtype *hit);
void R_Fish(objtype *ob);

extern const statetype s_schoolfish1;
extern const statetype s_schoolfish2;
void SpawnSchoolfish(Sint16 x, Sint16 y);
void T_SchoolFish(objtype *ob);

extern const statetype s_pixie;
extern const statetype s_pixielook;
extern const statetype s_pixieshoot;
extern const statetype s_pixieshoot2;
extern const statetype s_pixiefire1;
extern const statetype s_pixiefire2;
extern const statetype s_pixiefire3;
extern const statetype s_pixiefire4;
void SpawnPixie(Sint16 x, Sint16 y);
void T_Pixie(objtype *ob);
void T_PixieCheck(objtype *ob);
void T_PixieShoot(objtype *ob);
void R_Mshot(objtype *ob);

extern const statetype s_mine;
extern const statetype s_mineboom1;
extern const statetype s_mineboom2;
void SpawnMine(Sint16 x, Sint16 y, Sint16 dir);
void C_Mine(objtype *ob, objtype *hit);

extern const statetype s_lindsey1;
extern const statetype s_lindsey2;
extern const statetype s_lindsey3;
extern const statetype s_lindsey4;
void SpawnLindsey(Sint16 x, Sint16 y);
void T_Lindsey(objtype *ob);

extern const statetype s_dartthrower;
extern const statetype s_dart1;
extern const statetype s_dart2;
extern const statetype s_dartup1;
extern const statetype s_dartup2;
extern const statetype s_dartdown1;
extern const statetype s_dartdown2;
void SpawnDartShooter(Sint16 x, Sint16 y, Sint16 dir);
void T_DartShoot(objtype *ob);
void R_DartThrower(objtype *ob);

extern const statetype s_scuba;
void SpawnScuba(Sint16 x, Sint16 y);
void C_Scuba(objtype *ob, objtype *hit);


#endif //__CK4_DEFS__
