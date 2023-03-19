/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

Sint16 singlegravity;	// left over from Keen Dreams, not used in Keen 4-6

const Uint16 bounceangle [8][8] =
{
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 7,  6,  5,  4,  3,  2,  1,  0},
	{ 5,  4,  3,  2,  1,  0, 15, 14},
	{ 5,  4,  3,  2,  1,  0, 15, 14},
	{ 3,  2,  1,  0, 15, 14, 13, 12},
	{ 9,  8,  7,  6,  5,  4,  3,  2},
	{ 9,  8,  7,  6,  5,  4,  3,  2},
	{11, 10,  9,  8,  7,  6,  5,  4}
};

#ifndef KEEN4
const arrowdirtype arrowflip[] = {arrow_South, arrow_West, arrow_North, arrow_East, arrow_SouthWest, arrow_NorthWest, arrow_NorthEast, arrow_SouthEast};
#endif

const statetype s_keenstand     = {KEENSTANDLSPR, KEENSTANDRSPR, stepthink, false, true, 4, 0, 32, KeenPauseThink, KeenContact, KeenStandReact, &s_keenstand};

#ifdef KEEN5
const statetype s_keenride     = {KEENONPLATSPR, KEENONPLATSPR, stepthink, false, true, 4, 0, 32, KeenStandThink, KeenContact, KeenStandReact, &s_keenride};
#endif

const statetype s_keenpauselook = {KEENLOOKUSPR, KEENLOOKUSPR, stepthink, false, true, 60, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenstand};

const statetype s_keenwait1    = {KEENWAITR2SPR, KEENWAITR2SPR, stepthink, false, true, 90, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenwait2};
const statetype s_keenwait2    = {KEENWAITR1SPR, KEENWAITR1SPR, stepthink, false, true, 10, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenwait3};
const statetype s_keenwait3    = {KEENWAITR2SPR, KEENWAITR2SPR, stepthink, false, true, 90, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenwait4};
const statetype s_keenwait4    = {KEENWAITR1SPR, KEENWAITR1SPR, stepthink, false, true, 10, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenwait5};
const statetype s_keenwait5    = {KEENWAITR2SPR, KEENWAITR2SPR, stepthink, false, true, 90, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenwait6};
const statetype s_keenwait6    = {KEENWAITR3SPR, KEENWAITR3SPR, stepthink, false, true, 70, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenstand};

#ifdef KEEN4
const statetype s_keenmoon1     = {KEENMOON1SPR, KEENMOON1SPR, stepthink, false, true, 20, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenmoon2};
const statetype s_keenmoon2     = {KEENMOON2SPR, KEENMOON2SPR, stepthink, false, true, 90, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenmoon3};
const statetype s_keenmoon3     = {KEENMOON1SPR, KEENMOON1SPR, stepthink, false, true, 20, 0, 0, KeenPauseThink, KeenContact, KeenStandReact, &s_keenstand};
#endif

const statetype s_keenread      = {KEENSITREAD1SPR, KEENSITREAD1SPR, step, false, true, 16, 0, 0, 0, KeenContact, KeenStandReact, &s_keenread2};
const statetype s_keenread2     = {KEENSITREAD2SPR, KEENSITREAD2SPR, step, false, true, 16, 0, 0, 0, KeenContact, KeenStandReact, &s_keenread3};
const statetype s_keenread3     = {KEENSITREAD3SPR, KEENSITREAD3SPR, step, false, true, 16, 0, 0, 0, KeenContact, KeenStandReact, &s_keenread4};
const statetype s_keenread4     = {KEENSITREAD4SPR, KEENSITREAD4SPR, step, false, true, 16, 0, 0, 0, KeenContact, KeenStandReact, &s_keenread5};
const statetype s_keenread5     = {KEENREAD1SPR, KEENREAD1SPR, stepthink, false, true, 300, 0, 0, KeenReadThink, KeenContact, KeenStandReact, &s_keenread6};
const statetype s_keenread6     = {KEENREAD2SPR, KEENREAD2SPR, stepthink, false, true, 16, 0, 0, KeenReadThink, KeenContact, KeenStandReact, &s_keenread7};
const statetype s_keenread7     = {KEENREAD3SPR, KEENREAD3SPR, stepthink, false, true, 16, 0, 0, KeenReadThink, KeenContact, KeenStandReact, &s_keenread5};
const statetype s_keenstopread  = {KEENSTOPREAD1SPR, KEENSTOPREAD1SPR, step, false, true, 12, 0, 0, 0, KeenContact, KeenStandReact, &s_keenstopread2};
const statetype s_keenstopread2 = {KEENSTOPREAD2SPR, KEENSTOPREAD2SPR, step, false, true, 12, 0, 0, 0, KeenContact, KeenStandReact, &s_keenstopread3};
const statetype s_keenstopread3 = {KEENSITREAD2SPR, KEENSITREAD2SPR, step, false, true, 12, 0, 0, 0, KeenContact, KeenStandReact, &s_keenstand};

const statetype s_keenlookup    = {KEENLOOKUSPR, KEENLOOKUSPR, stepthink, false, true, 30, 0, 0, KeenLookUpThink, KeenContact, KeenStandReact, &s_keenlookup2};
const statetype s_keenlookup2   = {KEENLOOKUSPR, KEENLOOKUSPR, think, false, true, 0, 0, 0, KeenLookUpThink, KeenPosContact, KeenStandReact, NULL};

const statetype s_keenlookdown  = {KEENLOOKD1SPR, KEENLOOKD1SPR, stepthink, false, true, 6, 0, 0, KeenLookDownThink, KeenContact, KeenStandReact, &s_keenlookdown2};
const statetype s_keenlookdown2 = {KEENLOOKD2SPR, KEENLOOKD2SPR, stepthink, false, true, 24, 0, 0, KeenLookDownThink, KeenPosContact, KeenStandReact, &s_keenlookdown3};
const statetype s_keenlookdown3 = {KEENLOOKD2SPR, KEENLOOKD2SPR, think, false, true, 0, 0, 0, KeenLookDownThink, KeenPosContact, KeenStandReact, NULL};
const statetype s_keenlookdown4 = {KEENLOOKD1SPR, KEENLOOKD1SPR, step, false, true, 6, 0, 0, 0, KeenContact, KeenStandReact, &s_keenstand};

const statetype s_keendrop   = {KEENLOOKD1SPR, KEENLOOKD1SPR, step, false, false, 0, 0, 0, KeenDropDownThink, KeenContact, KeenSimpleReact, &s_keenjump3};
const statetype s_keendead   = {-1, -1, think, false, false, 10, 0, 0, 0, 0, R_Draw, NULL};

const statetype s_keendie1     = {KEENDIE1SPR, KEENDIE1SPR, think, false, false, 100, 0, 0, KeenDieThink, 0, R_Draw, &s_keendie1};
const statetype s_keendie2     = {KEENDIE2SPR, KEENDIE2SPR, think, false, false, 100, 0, 0, KeenDieThink, 0, R_Draw, &s_keendie2};

#ifdef KEEN4
const statetype s_keensuitdie1 = {SCUBAKEENDEAD1SPR, SCUBAKEENDEAD1SPR, think, false, false, 100, 0, 0, KeenDieThink, NULL, R_Draw, &s_keensuitdie1};
const statetype s_keensuitdie2 = {SCUBAKEENDEAD2SPR, SCUBAKEENDEAD2SPR, think, false, false, 100, 0, 0, KeenDieThink, NULL, R_Draw, &s_keensuitdie2};
#endif

const statetype s_keenshoot1    = {KEENSHOOTLSPR, KEENSHOOTRSPR, step, false, true, 9, 0, 0, KeenShootThink, KeenContact, KeenStandReact, &s_keenshoot2};
const statetype s_keenshoot2    = {KEENSHOOTLSPR, KEENSHOOTRSPR, step, false, true, 6, 0, 0, 0, KeenContact, KeenStandReact, &s_keenstand};

const statetype s_keenshootup1  = {KEENSHOOTUSPR, KEENSHOOTUSPR, step, false, true, 9, 0, 0, KeenShootThink, KeenContact, KeenStandReact, &s_keenshootup2};
const statetype s_keenshootup2  = {KEENSHOOTUSPR, KEENSHOOTUSPR, step, false, true, 6, 0, 0, 0, KeenContact, KeenStandReact, &s_keenlookup};

const statetype s_keenswitch  = {KEENENTER1SPR, KEENENTER1SPR, step, false, true, 8, 0, 0, KeenSwitchThink, NULL, KeenStandReact, &s_keenswitch2};
const statetype s_keenswitch2 = {KEENENTER1SPR, KEENENTER1SPR, step, false, true, 8, 0, 0, 0, 0, KeenStandReact, &s_keenstand};
const statetype s_keenkey     = {KEENENTER1SPR, KEENENTER1SPR, step, false, true, 6, 0, 0, KeenKeyThink, NULL, KeenStandReact, &s_keenswitch2};

const statetype s_keenlineup  = {KEENENTER1SPR, KEENENTER1SPR, think, false, false, 0, 0, 0, T_LineUp, 0, R_Draw, NULL};
#ifdef KEEN5
const statetype s_keenenter0   = {KEENENTER1SPR, KEENENTER1SPR, step, false, false, 45, 0, -64, NULL, NULL, R_Draw, &s_keenenter1};
const statetype s_keenteleport = {KEENWAITR2SPR, KEENWAITR2SPR, think, false, false, 0, 0, 0, NULL, NULL, R_Draw, NULL};
#endif

const statetype s_keenenter1 = {KEENENTER1SPR, KEENENTER1SPR, step, false, false, 9, 0, -64, WalkSound1, NULL, R_Draw, &s_keenenter2};
const statetype s_keenenter2 = {KEENENTER2SPR, KEENENTER2SPR, step, false, false, 9, 0, -64, WalkSound2, NULL, R_Draw, &s_keenenter3};
const statetype s_keenenter3 = {KEENENTER3SPR, KEENENTER3SPR, step, false, false, 9, 0, -64, WalkSound1, NULL, R_Draw, &s_keenenter4};
const statetype s_keenenter4 = {KEENENTER4SPR, KEENENTER4SPR, step, false, false, 9, 0, -64, WalkSound2, NULL, R_Draw, &s_keenenter5};
const statetype s_keenenter5 = {KEENENTER5SPR, KEENENTER5SPR, step, false, false, 9, 0, -64, KeenEnterThink, NULL, R_Draw, &s_keenstand};
#ifdef KEEN5
const statetype s_keenenter6 = {-1, -1, step, false, false, 9, 0, -64, KeenEnterThink, 0, R_Draw, &s_keenstand};
#endif

const statetype s_keenpole      = {KEENSHINNYL1SPR, KEENSHINNYR1SPR, think, false, false, 0, 0, 0, KeenPoleThink, KeenPosContact, KeenSimpleReact, &s_keenpole};

const statetype s_keenclimb1 = {KEENSHINNYL1SPR, KEENSHINNYR1SPR, slidethink, false, false, 8, 0, 8, KeenClimbThink, KeenPosContact, KeenSimpleReact, &s_keenclimb2};
const statetype s_keenclimb2 = {KEENSHINNYL2SPR, KEENSHINNYR2SPR, slidethink, false, false, 8, 0, 8, KeenClimbThink, KeenPosContact, KeenSimpleReact, &s_keenclimb3};
const statetype s_keenclimb3 = {KEENSHINNYL3SPR, KEENSHINNYR3SPR, slidethink, false, false, 8, 0, 8, KeenClimbThink, KeenPosContact, KeenSimpleReact, &s_keenclimb1};

const statetype s_keenslide1 = {KEENSLIDED1SPR, KEENSLIDED1SPR, slide, false, false, 8, 0, 24, KeenDropThink, KeenPosContact, KeenPoleReact, &s_keenslide2};
const statetype s_keenslide2 = {KEENSLIDED2SPR, KEENSLIDED2SPR, slide, false, false, 8, 0, 24, KeenDropThink, KeenPosContact, KeenPoleReact, &s_keenslide3};
const statetype s_keenslide3 = {KEENSLIDED3SPR, KEENSLIDED3SPR, slide, false, false, 8, 0, 24, KeenDropThink, KeenPosContact, KeenPoleReact, &s_keenslide4};
const statetype s_keenslide4 = {KEENSLIDED4SPR, KEENSLIDED4SPR, slide, false, false, 8, 0, 24, KeenDropThink, KeenPosContact, KeenPoleReact, &s_keenslide1};

const statetype s_keenpoleshoot1 = {KEENPSHOOTLSPR, KEENPSHOOTRSPR, step, false, false, 9, 0, 0, KeenShootThink, KeenPosContact, KeenSimpleReact, &s_keenpoleshoot2};
const statetype s_keenpoleshoot2 = {KEENPSHOOTLSPR, KEENPSHOOTRSPR, step, false, false, 6, 0, 0, 0, KeenPosContact, KeenSimpleReact, &s_keenpole};

const statetype s_keenpoleshootup1 = {KEENPLSHOOTUSPR, KEENPRSHOOTUSPR, step, false, false, 9, 0, 0, KeenShootThink, KeenPosContact, KeenSimpleReact, &s_keenpoleshootup2};
const statetype s_keenpoleshootup2 = {KEENPLSHOOTUSPR, KEENPRSHOOTUSPR, step, false, false, 6, 0, 0, 0, KeenPosContact, KeenSimpleReact, &s_keenpole};

const statetype s_keenpoleshootdown1 = {KEENPLSHOOTDSPR, KEENPRSHOOTDSPR, step, false, false, 9, 0, 0, KeenShootThink, KeenPosContact, KeenSimpleReact, &s_keenpoleshootdown2};
const statetype s_keenpoleshootdown2 = {KEENPLSHOOTDSPR, KEENPRSHOOTDSPR, step, false, false, 6, 0, 0, 0, KeenPosContact, KeenSimpleReact, &s_keenpole};

const statetype s_keenwalk1     = {KEENRUNL1SPR, KEENRUNR1SPR, slidethink, true, true, 6, 24, 0, KeenWalkThink, KeenContact, KeenWalkReact, &s_keenwalk2};
const statetype s_keenwalk2     = {KEENRUNL2SPR, KEENRUNR2SPR, slidethink, true, true, 6, 24, 0, KeenWalkThink, KeenContact, KeenWalkReact, &s_keenwalk3};
const statetype s_keenwalk3     = {KEENRUNL3SPR, KEENRUNR3SPR, slidethink, true, true, 6, 24, 0, KeenWalkThink, KeenContact, KeenWalkReact, &s_keenwalk4};
const statetype s_keenwalk4     = {KEENRUNL4SPR, KEENRUNR4SPR, slidethink, true, true, 6, 24, 0, KeenWalkThink, KeenContact, KeenWalkReact, &s_keenwalk1};

const statetype s_keenpogodown  = {KEENPOGOL2SPR, KEENPOGOR2SPR, step, true, false, 1, 0, 0, KeenBounceThink, KeenContact, KeenPogoReact, &s_keenpogo};
const statetype s_keenpogo      = {KEENPOGOL2SPR, KEENPOGOR2SPR, think, true, false, 0, 0, 0, KeenPogoThink, KeenContact, KeenPogoReact, &s_keenpogo2};
const statetype s_keenpogo2     = {KEENPOGOL1SPR, KEENPOGOR1SPR, think, true, false, 0, 0, 0, KeenPogoThink, KeenContact, KeenPogoReact, NULL};

const statetype s_keenjump1     = {KEENJUMPL1SPR, KEENJUMPR1SPR, think, false, false, 0, 0, 0, KeenAirThink, KeenContact, KeenAirReact, &s_keenjump2};
const statetype s_keenjump2     = {KEENJUMPL2SPR, KEENJUMPR2SPR, think, false, false, 0, 0, 0, KeenAirThink, KeenContact, KeenAirReact, &s_keenjump3};
const statetype s_keenjump3     = {KEENJUMPL3SPR, KEENJUMPR3SPR, stepthink, false, false, 50, 0, 0, KeenAirThink, KeenContact, KeenAirReact, &s_keenjump4};
const statetype s_keenjump4     = {KEENJUMPL2SPR, KEENJUMPR2SPR, stepthink, false, false, 40, 0, 0, KeenAirThink, KeenContact, KeenAirReact, &s_keenjump3};

const statetype s_keenairshoot1 = {KEENJLSHOOTLSPR, KEENJRSHOOTRSPR, stepthink, false, false, 9, 0, 0, T_Projectile, KeenContact, KeenAirReact, &s_keenairshoot2};
const statetype s_keenairshoot2 = {KEENJLSHOOTLSPR, KEENJRSHOOTRSPR, stepthink, true, false, 1, 0, 0, KeenShootThink, KeenContact, KeenAirReact, &s_keenairshoot3};
const statetype s_keenairshoot3 = {KEENJLSHOOTLSPR, KEENJRSHOOTRSPR, stepthink, false, false, 6, 0, 0, T_Projectile, KeenContact, KeenAirReact, &s_keenjump3};

const statetype s_keenairshootup1 = {KEENJSHOOTUSPR, KEENJSHOOTUSPR, stepthink, false, false, 9, 0, 0, T_Projectile, KeenContact, KeenAirReact, &s_keenairshootup2};
const statetype s_keenairshootup2 = {KEENJSHOOTUSPR, KEENJSHOOTUSPR, stepthink, true, false, 1, 0, 0, KeenShootThink, KeenContact, KeenAirReact, &s_keenairshootup3};
const statetype s_keenairshootup3 = {KEENJSHOOTUSPR, KEENJSHOOTUSPR, stepthink, false, false, 6, 0, 0, T_Projectile, KeenContact, KeenAirReact, &s_keenjump3};

const statetype s_keenairshootdown1 = {KEENJSHOOTDSPR, KEENJSHOOTDSPR, stepthink, false, false, 9, 0, 0, T_Projectile, KeenContact, KeenAirReact, &s_keenairshootdown2};
const statetype s_keenairshootdown2 = {KEENJSHOOTDSPR, KEENJSHOOTDSPR, stepthink, true, false, 1, 0, 0, KeenShootThink, KeenContact, KeenAirReact, &s_keenairshootdown3};
const statetype s_keenairshootdown3 = {KEENJSHOOTDSPR, KEENJSHOOTDSPR, stepthink, false, false, 6, 0, 0, T_Projectile, KeenContact, KeenAirReact, &s_keenjump3};

const statetype s_keenholdon  = {KEENHANGLSPR, KEENHANGRSPR, step, false, false, 12, 0, 0, 0, KeenPosContact, KeenSimpleReact, &s_keenholdon2};
const statetype s_keenholdon2 = {KEENHANGLSPR, KEENHANGRSPR, think, false, false, 0, 0, 0, KeenHoldThink, KeenPosContact, KeenSimpleReact, NULL};

const statetype s_keenclimbup  = {KEENCLIMBEDGEL1SPR, KEENCLIMBEDGER1SPR, step, false, false, 10, 0, 0, T_PullUp1, KeenPosContact, KeenSimpleReact, &s_keenclimbup2};
const statetype s_keenclimbup2 = {KEENCLIMBEDGEL2SPR, KEENCLIMBEDGER2SPR, step, false, false, 10, 0, 0, T_PullUp2, KeenPosContact, KeenSimpleReact, &s_keenclimbup3};
const statetype s_keenclimbup3 = {KEENCLIMBEDGEL3SPR, KEENCLIMBEDGER3SPR, step, false, false, 10, 0, 0, T_PullUp3, KeenPosContact, KeenSimpleReact, &s_keenclimbup4};
const statetype s_keenclimbup4 = {KEENCLIMBEDGEL4SPR, KEENCLIMBEDGER4SPR, step, false, false, 10, 0, 0, T_PulledUp, KeenPosContact, KeenSimpleReact, &s_keenclimbup5};
const statetype s_keenclimbup5 = {KEENSTANDLSPR, KEENSTANDRSPR, step, false, false, 6, 0, 0, 0, KeenPosContact, KeenSimpleReact, &s_keenstand};

const Sint16 slopespeed[8] = {0, 0, 4, 4, 8, -4, -4, -8};
const Sint16 polexspeed[3] = {-8, 0, 8};

const Sint16 shotsinclip[4] = {0, 8, 5, 5};
const Sint16 bonussound[] = {
	SND_GETKEY,SND_GETKEY,SND_GETKEY,SND_GETKEY,
	SND_GETPOINTS,SND_GETPOINTS,SND_GETPOINTS,
	SND_GETPOINTS,SND_GETPOINTS,SND_GETPOINTS,
	SND_EXTRAKEEN,
	SND_GETAMMO
#ifdef KEEN5
	,SND_GETKEYCARD
#endif
};
const Sint16 bonuspoints[] = {
	0, 0, 0, 0,
	100, 200, 500,
	1000, 2000, 5000,
	0,
	0
#ifdef KEEN5
	,0
#endif
};
const Sint16 bonussprite[] = {
	BONUSGEMSPR, BONUSGEMSPR, BONUSGEMSPR, BONUSGEMSPR,
	BONUS100SPR, BONUS200SPR, BONUS500SPR,
	BONUS1000SPR, BONUS2000SPR, BONUS5000SPR,
	BONUS1UPSPR,
	BONUSCLIPSPR
#ifdef KEEN5
	,BONUSCARDSPR
#endif
};

Uint16 zeromap = 0;

// uninitialized variables:

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

Sint16 jumptime;
Sint32 leavepoletime;
Sint16 moonok;

/*
=============================================================================

								KEEN

player->temp1 = pausetime / pointer to zees when sleeping
player->temp2 = pausecount / stagecount
player->temp3 =
player->temp4 =

=============================================================================
*/


/*
===============
=
= SpawnKeen
=
===============
*/

void SpawnKeen(Sint16 x, Sint16 y, Sint16 dir)
{
	player->obclass = keenobj;
	player->active = ac_allways;
	player->priority = 1;
	player->x = CONVERT_TILE_TO_GLOBAL(x) ;
	player->y = CONVERT_TILE_TO_GLOBAL(y) - 0xF1;	//TODO: weird

	player->xdir = dir;
	player->ydir = 1;
    CK_SetSprite(&player->sprite, CKS_KEEN);
	NewState(player, &s_keenstand);

}

//==========================================================================

/*
======================
=
= CheckGrabPole
=
======================
*/

boolean CheckGrabPole(objtype *ob)
{
	Uint16 *map;

//
// kludgy bit to not let you grab a pole the instant you jump off it
//
	if (lasttimecount < leavepoletime)
	{
		leavepoletime = 0;
	}
	else if (lasttimecount-leavepoletime < 19)
	{
		return false;
	}

	if (c.yaxis == -1)
	{
		map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((ob->top+6*PIXGLOBAL)/TILEGLOBAL)*CK_CurLevelWidth;
	}
	else
	{
		map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (ob->tilebottom+1)*CK_CurLevelWidth;
	}

	map += ob->tilemidx;

	if ((CK_TileInfo[1][INTILE + *map] & 0x7F) == INTILE_POLE)
	{
		ob->x = CONVERT_TILE_TO_GLOBAL(ob->tilemidx-1) + 8*PIXGLOBAL;
		xtry = 0;
		ytry = c.yaxis * 32;
		ob->needtoclip = cl_noclip;		// can climb through pole holes
		ob->state = &s_keenpole;
		return true;
	}
	return false;
}

//==========================================================================

/*
======================
=
= CheckEnterHouse
=
= Checks for tiles that Keen can interact with by pressing up
=
======================
*/

boolean CheckEnterHouse(objtype *ob)
{
	Uint16 temp;
#ifdef KEEN5
	Uint16 infoval;
#endif
	Uint16 intile, intile2;

	intile = CK_TileInfo[1][INTILE + *((Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((ob->tiletop*CK_CurLevelWidth)) + ob->tilemidx)];
	if (intile == INTILE_SWITCH0 || intile == INTILE_SWITCH1 || intile == INTILE_BRIDGESWITCH)
	{
		temp = CONVERT_TILE_TO_GLOBAL(ob->tilemidx) - 4*PIXGLOBAL;
		if (ob->x != temp)
		{
			ob->temp1 = temp;
			ob->state = &s_keenlineup;
		}
		else
		{
			ob->state = &s_keenswitch;
		}
		upheld = true;
		return true;
	}
	else if (intile == INTILE_DOOR || intile == INTILE_KEYCARDDOOR)
	{
		temp = CONVERT_TILE_TO_GLOBAL(ob->tilemidx) + 6*PIXGLOBAL;
		intile2 = CK_TileInfo[1][INTILE + *((Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((ob->tiletop*CK_CurLevelWidth)) + ob->tilemidx-1)];
		if (intile2 == 2 || intile2 == 32)
			temp -= TILEGLOBAL;

		// BUG:
		//
		// The s_keenenter? states rely on Keen's ydir being set to 1,
		// which may not always be the case (e.g. if Keen was pushed off
		// a pole by another actor in the level).
		// If ydir is not 1, Keen will not move up during that animation
		// which means the teleport coordinates won't be read from the
		// intended tile position and Keen might end up teleporting to
		// position 0, 0 in the map and thus win the current level.
		// 
		// That can easily be avoided by setting ob->ydir to 1 when
		// changing ob->state to s_keenenter0 or s_keenenter1.

		if (ob->x != temp)
		{
			ob->temp1 = temp;
			ob->state = &s_keenlineup;
		}
#ifdef KEEN5
		else if (intile == INTILE_KEYCARDDOOR)
		{
			if (gamestate.keycard)
			{
				gamestate.keycard = false;
				SD_PlaySound(SND_OPENCARDDOOR);
				GetNewObj(false);
				ck_newobj->x = ob->tilemidx - 2;
				ck_newobj->y = ob->tilebottom - 4;
				ck_newobj->active = ac_allways;
				ck_newobj->needtoclip = cl_noclip;
				ck_newobj->obclass = inertobj;
				NewState(ck_newobj, &s_carddoor);
				// Note: no invincibility here - card doors were always used as level exits in Keen 5
				ob->state = &s_keenenter0;
				ob->priority = 0;
				upheld = true;
				return true;
			}
			else
			{
				SD_PlaySound(SND_NOWAY);
				ob->state = &s_keenstand;
				upheld = true;
				return false;
			}
		}
#endif
		else
		{
			invincible = 110;	//about 1.57 seconds
			ob->state = &s_keenenter1;
			ob->priority = 0;
#ifdef KEEN5
			{
				infoval = *(mapsegs[2]+mapbwidthtable[ob->tiletop]/2+ob->tilemidx);
				if (!infoval)
					SpawnTeleport();
			}
#endif
		}
		upheld = true;
		return true;
	}
	return false;
}

//==========================================================================

/*
============================
=
= WalkSound1
=
============================
*/

void WalkSound1(objtype *ob)
{
	SD_PlaySound(SND_WORLDWALK1);
	//ob++;			// shut up compiler
}

/*
============================
=
= WalkSound2
=
============================
*/

void WalkSound2(objtype *ob)
{
	SD_PlaySound(SND_WORLDWALK2);
	//ob++;			// shut up compiler
}

//==========================================================================

/*
============================
=
= KeenStandThink
=
============================
*/

void KeenStandThink(objtype *ob)
{
#ifdef KEEN5
	if (ob->hitnorth == 25 && ob->state != &s_keenride)
	{
		ob->state = &s_keenride;
	}
#endif

	if (c.xaxis)
	{
	// started walking
		ob->state = &s_keenwalk1;
		KeenWalkThink(ob);
		xtry = (Sint16)(ob->xdir * ob->state->xmove * tics) / 4;
	}
	else if (firebutton && !fireheld)
	{
	// shoot current xdir
		fireheld = true;
		if (c.yaxis == -1)
		{
			ob->state = &s_keenshootup1;
		}
		else
		{
			ob->state = &s_keenshoot1;
		}
	}
	else if (jumpbutton && ! jumpheld)
	{
	// jump straight up
		jumpheld = true;
		SD_PlaySound(SND_JUMP);
		ob->xspeed = 0;
		ob->yspeed = -40;
		ytry = 0;
		jumptime = 18;
		ob->state = &s_keenjump1;
	}
	else if (pogobutton && !pogoheld)
	{
	// get on pogo
		pogoheld = true;
		SD_PlaySound(SND_JUMP);
		ob->state = &s_keenpogodown;
		ob->xspeed = 0;
		ob->yspeed = -48;
		ytry = 0;
		jumptime = 24;
	}
	else
	{
		switch (c.yaxis)
		{
		case -1:
			if (CheckGrabPole(ob))
				break;
			if (upheld || !CheckEnterHouse(ob))
			{
				ob->state = &s_keenlookup;
			}
			break;
		case 1:
			if (CheckGrabPole(ob))
				break;
			ob->state = &s_keenlookdown;
		}
		return;
	}
}

//===========================================================================

/*
=======================
=
= KeenPauseThink
=
= Do special animations in time
=
=======================
*/

void KeenPauseThink(objtype *ob)
{
#ifdef KEEN5
	if (ob->hitnorth == 25 && ob->state != &s_keenride)
	{
		ob->state = &s_keenride;
	}
#endif

	if (c.dir != dir_None || jumpbutton || pogobutton || firebutton)
	{
		ob->temp1 = ob->temp2 = 0;			// not paused any more
		ob->state = &s_keenstand;
		KeenStandThink(ob);
	}
	else
	{
		//only increase idle counter when NOT standing on a sprite:
		if ((ob->hitnorth & ~7) != 0x18)
			ob->temp1 = ob->temp1 + tics;

		switch (ob->temp2)
		{
		case 0:
			if (ob->temp1 > 200)
			{
				ob->temp2++;
				ob->state = &s_keenpauselook;
				ob->temp1 = 0;
			}
			break;
		case 1:
			if (ob->temp1 > 300)
			{
				ob->temp2++;
				ob->temp1 = 0;
#ifdef KEEN4
				if (moonok == 1)
				{
					moonok = 2;	//don't moon again unless the level is restarted
					ob->state = &s_keenmoon1;
				}
				else
#endif
				{
					ob->state = &s_keenwait1;
				}
			}
			break;
		case 2:
			if (ob->temp1 > 700)
			{
				ob->temp2++;
				ob->state = &s_keenread;
				ob->temp1 = 0;
			}
			break;
		}
	}
}

//===========================================================================

/*
=======================
=
= KeenReadThink
=
=======================
*/

void KeenReadThink(objtype *ob)
{
	if (c.dir != dir_None || jumpbutton || pogobutton)
	{
		ob->temp1 = ob->temp2 = 0;
		ob->state = &s_keenstopread;
	}
}

//===========================================================================

/*
=======================
=
= KeenLookUpThink
=
=======================
*/

void KeenLookUpThink(objtype *ob)
{
	if (c.yaxis != -1 || c.xaxis
		|| (jumpbutton && !jumpheld)
		|| (pogobutton && !pogoheld)
		|| firebutton)
	{
		ob->state = &s_keenstand;
		KeenStandThink(ob);
	}
}

//===========================================================================

/*
=======================
=
= KeenLookDownThink
=
=======================
*/

void KeenLookDownThink(objtype *ob)
{
	Uint16 *map;
	Sint16 y, ymove;
	Uint16 tile;

	if (jumpbutton && ! jumpheld && (ob->hitnorth & 7) == 1)
	{
	//
	// drop down a level
	//
		jumpheld = true;

		y = ob->tilebottom;
		map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((y*CK_CurLevelWidth)) + ob->tilemidx;
		tile = *map;
		if (CK_TileInfo[1][WESTWALL+tile] || CK_TileInfo[1][EASTWALL+tile] || CK_TileInfo[1][SOUTHWALL+tile])
			return;				// wall prevents drop down

		map += CK_CurLevelWidth;
		tile = *map;
		if (CK_TileInfo[1][WESTWALL+tile] || CK_TileInfo[1][EASTWALL+tile] || CK_TileInfo[1][SOUTHWALL+tile])
			return;				// wall prevents drop down

		ymove = _ck_max(4, tics) * PIXGLOBAL;
		if (gamestate.riding)
			ymove += gamestate.riding->ymove;
		ob->bottom += ymove;
		gamestate.riding = NULL;
		ob->y += ymove;
		xtry = ytry = 0;
		ob->state = &s_keenjump3;
		ob->xspeed = ob->yspeed = 0;
		SD_PlaySound(SND_PLUMMET);
	}
	else if (c.yaxis != 1 || c.xaxis
		|| (jumpbutton && !jumpheld)
		|| (pogobutton && !pogoheld))
	{
		ob->state = &s_keenlookdown4;
	}
}

//===========================================================================

/*
=======================
=
= KeenWalkThink
=
=======================
*/

void KeenWalkThink(objtype *ob)
{
	Sint16 xmove;

	if (c.xaxis == 0)
	{
	//
	// stopped running
	//
		ob->state = &s_keenstand;
		KeenStandThink(ob);
		return;
	}

	ob->xdir = c.xaxis;

	switch (c.yaxis)
	{
	case -1:
		if (CheckGrabPole(ob))
			return;
		if (upheld)
			return;
		if (!CheckEnterHouse(ob))
			break;;
		return;

	case 1:
		if (!CheckGrabPole(ob))
			break;
		return;
	}

	if (firebutton && !fireheld)
	{
	//
	// shoot
	//
		fireheld = true;
		if (c.yaxis == -1)
		{
			ob->state = &s_keenshootup1;
		}
		else
		{
			ob->state = &s_keenshoot1;
		}
		return;
	}

	if (jumpbutton && !jumpheld)
	{
	//
	// running jump
	//
		jumpheld = true;
		SD_PlaySound(SND_JUMP);
		ob->xspeed = ob->xdir * 16;
		ob->yspeed = -40;
		xtry = ytry = 0;
		jumptime = 18;
		ob->state = &s_keenjump1;
	}

	if (pogobutton && !pogoheld)
	{
	//
	// get on pogo
	//
		pogoheld = true;
		ob->state = &s_keenpogodown;
		SD_PlaySound(SND_JUMP);
		ob->xspeed = ob->xdir * 16;
		ob->yspeed = -48;
		xtry = 0;
		jumptime = 24;
		return;
	}

	//
	// give speed for slopes
	//
	xmove = slopespeed[ob->hitnorth & 7] * tics;
	xtry += xmove;

	//
	// handle walking sounds
	//
	if (ob->state == &s_keenwalk1 && ob->temp3 == 0)
	{
		SD_PlaySound(SND_WORLDWALK1);
		ob->temp3 = 1;
	}
	else if (ob->state == &s_keenwalk3 && ob->temp3 == 0)
	{
		SD_PlaySound(SND_WORLDWALK2);
		ob->temp3 = 1;
	}
	else if (ob->state == &s_keenwalk2 ||ob->state == &s_keenwalk4)
	{
		ob->temp3 = 0;
	}
}

//===========================================================================

/*
=======================
=
= T_LineUp
=
= Lines up Keen's position for interacting with tiles (temp1 is desired x)
=
=======================
*/

void T_LineUp(objtype *ob)
{
	Sint16 xmove;

	xmove = ob->temp1 - ob->x;
	if (xmove < 0)
	{
		xtry = xtry - tics * 16;
		if (xtry > xmove)
			return;
	}
	else if (xmove > 0)
	{
		xtry = xtry + tics * 16;
		if (xtry < xmove)
			return;
	}
	xtry = xmove;
	ob->temp1 = 0;
	if (!CheckEnterHouse(ob))
		ob->state = &s_keenstand;
}

//===========================================================================

/*
=======================
=
= KeenEnterThink
=
=======================
*/

void KeenEnterThink(objtype *ob)
{
	Uint16 info;

	info = CK_GetInfo((ob->tilebottom*CK_CurLevelWidth) + ob->tileleft);
#ifdef KEEN5
	if (!info)
	{
		playstate = ex_portout;
		ob->state = &s_keenenter6;
		return;
	}
	else if (info == 0xB1B1)
	{
		playstate = ex_completed;
		ob->state = &s_keenenter6;
		return;
	}
#endif
	ob->y = (CONVERT_TILE_TO_GLOBAL(info & 0xFF) - TILEGLOBAL) + 15;
	ob->x = CONVERT_TILE_TO_GLOBAL(info >> 8);
	ob->priority = 1;
	ob->needtoclip = cl_noclip;
	ChangeState(ob, ob->state->nextstate);
	ob->needtoclip = cl_midclip;
	CenterActor(ob);
}

//===========================================================================

/*
=======================
=
= KeenSwitchThink
=
=======================
*/

void KeenSwitchThink(objtype *ob)
{
	Uint16 intile, maptile, newtile, info, sx, sy, tileoff;
	Uint16 *map;
	Uint16 tile, x, y;
	Sint8 manim;

	tileoff = ((ob->tiletop)*CK_CurLevelWidth) + ob->tilemidx;
	maptile = CK_CurLevelData[tileoff + CK_CurLevelSize];
	newtile = maptile + (Sint8)CK_TileInfo[1][MANIM + maptile];
	info = CK_GetInfo(tileoff);
	sx = info >> 8;
	sy = info & 0xFF;
	intile = CK_TileInfo[1][INTILE + maptile];

	CK_SetMapTile(ob->tilemidx, ob->tiletop, newtile, 1);
	SD_PlaySound(SND_USESWITCH);
	if (intile == INTILE_BRIDGESWITCH)
	{
		//toggle bridge:
		for (y = sy; sy+2 > y; y++)
		{
			map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (y*CK_CurLevelWidth) + sx - (y != sy);
			for (x = sx - (y != sy); x < CK_CurLevelWidth; x++)
			{
				tile = *(map++);
				manim = CK_TileInfo[1][MANIM + tile];
				if (!manim)
					break;

				tile += manim;
				CK_SetMapTile(x, y, tile, 1);
			}
		}
	}
	else
	{
		//toggle platform blocker:
		tileoff = (sy*CK_CurLevelWidth) + sx;
		tile = CK_GetInfo(tileoff);
#ifdef KEEN5
		if (tile >= DIRARROWSTART && tile < DIRARROWEND)
		{
			Quit("KeenSwitchThink : Handle this!");
			*map = arrowflip[tile-DIRARROWSTART]+DIRARROWSTART;
			return;
		}
#endif
		tile ^= PLATFORMBLOCK;
		CK_SetInfo(tileoff, tile);
	}
}

//===========================================================================

/*
=======================
=
= KeenKeyThink
=
=======================
*/

void KeenKeyThink(objtype *ob)
{
	Uint16 newtile, info, x, y, tileoff;
	Uint16 *map;
	Uint16 tile, h;

	tileoff = ((ob->tilebottom)*CK_CurLevelWidth) + ob->tilemidx;
	newtile = CK_CurLevelData[tileoff+CK_CurLevelSize] + 18;
	info = CK_GetInfo(tileoff);
	x = info >> 8;
	y = info & 0xFF;
	CK_SetMapTile(ob->tilemidx, ob->tilebottom, newtile, 1);
	SD_PlaySound(SND_OPENDOOR);
	GetNewObj(false);
	ck_newobj->x = x;
	ck_newobj->y = y;

	if (x > CK_CurLevelWidth || x < 2 || y > CK_CurLevelHeight || y < 2)
		Quit("Keyholder points to a bad spot!");

	map = CK_CurLevelData +CK_CurLevelSize + (y*CK_CurLevelWidth) + x;
	tile = *map;
	h = 1;
	map += CK_CurLevelWidth;
	while (*map == tile)
	{
			h++;
			map += CK_CurLevelWidth;
	}
	ck_newobj->temp1 = h;
	ck_newobj->active = ac_allways;
	ck_newobj->needtoclip = cl_noclip;
	ck_newobj->obclass = inertobj;
	NewState(ck_newobj, &s_door1);
}

//===========================================================================

/*
=======================
=
= KeenAirThink
=
=======================
*/

void KeenAirThink(objtype *ob)
{
	if (jumpcheat && jumpbutton)
	{
		ob->yspeed = -40;
		jumptime = 18;
		jumpheld = true;
	}
	if (jumptime)
	{
		if (jumptime <= tics)
		{
			ytry = ob->yspeed * jumptime;
			jumptime = 0;
		}
		else
		{
			ytry = ob->yspeed * tics;
			if (!jumpcheat)
				jumptime = jumptime - tics;
		}
		if (!jumpbutton)
			jumptime = 0;

		if (jumptime == 0 && ob->state->nextstate)
			ob->state = ob->state->nextstate;	// switch to second jump stage
	}
	else
	{
		if (gamestate.difficulty == gd_Easy)
		{
			DoWeakGravity(ob);
		}
		else
		{
			DoGravity(ob);
		}

		if (ob->yspeed > 0 && ob->state != &s_keenjump3 && ob->state != &s_keenjump4)
		{
			ob->state = ob->state->nextstate;	// switch to third jump stage
		}
	}

//-------------

	if (c.xaxis)
	{
		ob->xdir = c.xaxis;
		AccelerateX(ob, c.xaxis*2, 24);
	}
	else
	{
		FrictionX(ob);
	}

	if (ob->hitsouth == 17)		// going through a pole hole
	{
		ob->xspeed = xtry = 0;
	}

	if (firebutton && !fireheld)
	{
		fireheld = true;
	//
	// shoot
	//
		switch (c.yaxis)
		{
		case -1:
			ob->state = &s_keenairshootup1;
			return;
		case 0:
			ob->state = &s_keenairshoot1;
			return;
		case 1:
			ob->state = &s_keenairshootdown1;
			return;
		}
	}

	if (pogobutton && !pogoheld)
	{
		pogoheld = true;
		ob->state = &s_keenpogo;
		jumptime = 0;
		return;
	}

	if (c.yaxis == -1)
		CheckGrabPole(ob);
}

//===========================================================================

/*
=======================
=
= KeenBounceThink
=
= Gives an extra bit of height on the first pogo bounce and creates
= the "impossible pogo trick" when the jump key is held down
=
=======================
*/

void KeenBounceThink(objtype *ob)
{
	ob->yspeed = -48;
	ytry = ob->yspeed * 6;
	jumptime = 24;
	SD_PlaySound(SND_POGOBOUNCE);
}

//===========================================================================

/*
=======================
=
= KeenPogoThink
=
=======================
*/

void KeenPogoThink(objtype *ob)
{
	if (jumptime)
	{
		if (jumpbutton || jumptime <= 9)
		{
			DoTinyGravity(ob);
		}
		else
		{
			DoGravity(ob);
		}
		if (jumptime <= tics)
		{
			jumptime = 0;
		}
		else
		{
			jumptime = jumptime - tics;
		}
		if (jumptime == 0 && ob->state->nextstate)
			ob->state = ob->state->nextstate;
	}
	else
	{
		if (gamestate.difficulty == gd_Easy)
		{
			DoWeakGravity(ob);
		}
		else
		{
			DoGravity(ob);
		}
	}

	if (c.xaxis)
	{
		if (ob->xspeed == 0)
			ob->xdir = c.xaxis;
		AccelerateX(ob, c.xaxis, 24);
	}
	else
	{
		xtry = xtry + ob->xspeed * tics;
		if (ob->xspeed > 0)
		{
			ob->xdir = 1;
		}
		else if (ob->xspeed < 0)
		{
			ob->xdir = -1;
		}
	}

	if (ob->hitsouth == 17)		// going through a pole hole
	{
		ob->xspeed = xtry = 0;
	}

	if (firebutton && !fireheld)
	{
		fireheld = true;
		switch (c.yaxis)
		{
		case -1:
			ob->state = &s_keenairshootup1;
			return;
		case 0:
			ob->state = &s_keenairshoot1;
			return;
		case 1:
			ob->state = &s_keenairshootdown1;
			return;
		}
	}

	if (pogobutton && !pogoheld)
	{
		pogoheld = true;
		ob->state = &s_keenjump3;
	}
}

//===========================================================================

/*
=======================
=
= PoleActions
=
=======================
*/

void PoleActions(objtype *ob)
{
	if (c.xaxis)
		ob->xdir = c.xaxis;

	if (firebutton && !fireheld)
	{
		fireheld = true;
		switch (c.yaxis)
		{
		case -1:
			ob->state = &s_keenpoleshootup1;
			break;
		case 0:
			ob->state = &s_keenpoleshoot1;
			break;
		case 1:
			ob->state = &s_keenpoleshootdown1;
			break;
		}
	}

	if (jumpbutton && !jumpheld)		// jump off the pole
	{
		jumpheld = true;
		SD_PlaySound(SND_JUMP);
		ob->xspeed = polexspeed[c.xaxis+1];
		ob->yspeed = -20;
		ob->needtoclip = cl_midclip;
		jumptime = 10;
		ob->state = &s_keenjump1;
		ob->ydir = 1;
		leavepoletime = lasttimecount;
	}
}

/*
=======================
=
= KeenPoleThink
=
=======================
*/

void KeenPoleThink(objtype *ob)
{
	Uint16 tile;
	Uint16 *map;

	switch (c.yaxis)
	{
	case -1:
		ob->state = &s_keenclimb1;
		ob->ydir = -1;
		return;
	case 1:
		ob->state = &s_keenslide1;
		ob->ydir = 1;
		KeenDropThink(ob);
		return;
	}

	if (c.xaxis)
	{
	//
	// walk off pole if right next to ground
	//
		map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((ob->tilebottom+1)*CK_CurLevelWidth) + ob->tilemidx;
		tile = *map;
		if (CK_TileInfo[1][NORTHWALL+tile])
		{
			ob->xspeed = 0;
			ob->yspeed = 0;
			ob->needtoclip = cl_midclip;
			jumptime = 0;
			ob->state = &s_keenjump3;
			ob->ydir = 1;
			SD_PlaySound(SND_PLUMMET);
			return;
		}
	}
	PoleActions(ob);
}

/*
=======================
=
= KeenClimbThink
=
=======================
*/

void KeenClimbThink(objtype *ob)
{
	Uint16 *map;

	map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (ob->tiletop*CK_CurLevelWidth) + ob->tilemidx;
	
	if ((CK_TileInfo[1][INTILE+*map] & 0x7F) != INTILE_POLE)
	{
		ytry = 0;
		ob->state = &s_keenpole;		// ran out of pole
		PoleActions(ob);
		return;
	}

	switch (c.yaxis)
	{
	case 0:
		ob->state = &s_keenpole;
		ob->ydir = 0;
		break;

	case 1:
		ob->state = &s_keenslide1;
		ob->ydir = 1;
		KeenDropThink(ob);
		break;
	}

	PoleActions(ob);
}

/*
=======================
=
= KeenDropThink
=
=======================
*/

void KeenDropThink(objtype *ob)
{
	Uint16 *map;
	Uint16 y;

	y = CONVERT_GLOBAL_TO_TILE(ob->bottom - 4*PIXGLOBAL);
	map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (y*CK_CurLevelWidth) + ob->tilemidx;

	if ((CK_TileInfo[1][INTILE+*map] & 0x7F) != INTILE_POLE)
	{
		SD_PlaySound(SND_PLUMMET);
		ob->state = &s_keenjump3;		// ran out of pole
		jumptime = 0;
		ob->xspeed = polexspeed[c.xaxis+1];
		ob->yspeed = 0;
		ob->needtoclip = cl_midclip;
		ob->tilebottom--;
		return;
	}

	switch (c.yaxis)
	{
	case -1:
		ob->state = &s_keenclimb1;
		ob->ydir = -1;
		break;

	case 0:
		ob->state = &s_keenpole;
		ob->ydir = 0;
		break;
	}

	PoleActions(ob);
}

//===========================================================================

/*
=======================
=
= KeenDropDownThink
=
=======================
*/

void KeenDropDownThink(objtype *ob)
{
	ob->needtoclip = cl_midclip;
}

//===========================================================================

/*
=======================
=
= KeenHoldThink
=
=======================
*/

void KeenHoldThink(objtype *ob)
{
	Uint16 tile;

	if (c.yaxis == -1 || ob->xdir == c.xaxis)
	{
		ob->state = &s_keenclimbup;
		if (ob->xdir == 1)
		{
			tile = *((Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((ob->tiletop-1)*CK_CurLevelWidth) + ob->tileright);
		}
		else
		{
			tile = *((Uint16 *)CK_CurLevelData + CK_CurLevelSize + ((ob->tiletop-1)*CK_CurLevelWidth) + ob->tileleft);
		}
		if (ob->xdir == 1)
		{
			ytry = -16*PIXGLOBAL;
		}
		else
		{
			ytry = -8*PIXGLOBAL;
		}
		if (!(CK_TileInfo[1][INTILE+tile] & INTILE_FOREGROUND))
			ob->priority = 3;
	}
	else if (c.yaxis == 1 || c.xaxis && ob->xdir != c.xaxis)
	{
		ob->state = &s_keenjump3;
		ob->needtoclip = cl_midclip;
	}
}

//===========================================================================

/*
=======================
=
= KeenShootThink
=
=======================
*/

void KeenShootThink(objtype *ob)
{
// can't use &<var> in a switch statement...

	if (ob->state == &s_keenshoot1)
	{
		if (ob->xdir == 1)
		{
			SpawnShot(ob->x + 16*PIXGLOBAL, ob->y + 4*PIXGLOBAL, dir_East);
		}
		else
		{
			SpawnShot(ob->x - 8*PIXGLOBAL, ob->y + 4*PIXGLOBAL, dir_West);
		}
	}
	if (ob->state == &s_keenairshoot2)
	{
		if (ob->xdir == 1)
		{
			SpawnShot(ob->x + 16*PIXGLOBAL, ob->y + 2*PIXGLOBAL, dir_East);
		}
		else
		{
			SpawnShot(ob->x, ob->y + 2*PIXGLOBAL, dir_West);
		}
	}
	if (ob->state == &s_keenairshootdown2)
	{
		SpawnShot(ob->x + 8*PIXGLOBAL, ob->y + 18*PIXGLOBAL, dir_South);
	}
	if (ob->state == &s_keenairshootup2)
	{
		SpawnShot(ob->x + 5*PIXGLOBAL, ob->y - 10*PIXGLOBAL, dir_North);
	}
	if (ob->state == &s_keenshootup1)
	{
		SpawnShot(ob->x + 5*PIXGLOBAL, ob->y - 10*PIXGLOBAL, dir_North);
	}
	if (ob->state == &s_keenpoleshoot1)
	{
		if (ob->xdir == 1)
		{
			SpawnShot(ob->x + 16*PIXGLOBAL, ob->y + 4*PIXGLOBAL, dir_East);
		}
		else
		{
			SpawnShot(ob->x - 8*PIXGLOBAL, ob->y + 4*PIXGLOBAL, dir_West);
		}
	}
	if (ob->state == &s_keenpoleshootup1)
	{
		if (ob->xdir == 1)
		{
			SpawnShot(ob->x + 6*PIXGLOBAL, ob->y + 4*PIXGLOBAL, dir_North);
		}
		else
		{
			SpawnShot(ob->x + 12*PIXGLOBAL, ob->y + 4*PIXGLOBAL, dir_North);
		}
	}
	if (ob->state == &s_keenpoleshootdown1)
	{
		if (ob->xdir == 1)
		{
			SpawnShot(ob->x + 6*PIXGLOBAL, ob->y + 24*PIXGLOBAL, dir_South);
		}
		else
		{
			SpawnShot(ob->x + 12*PIXGLOBAL, ob->y + 24*PIXGLOBAL, dir_South);
		}
	}
}

//===========================================================================

/*
=======================
=
= T_PullUp1
=
=======================
*/

void T_PullUp1(objtype *ob)
{
	if (ob->xdir == 1)
	{
		xtry = 8*PIXGLOBAL;
	}
	else
	{
		ytry = -8*PIXGLOBAL;
	}
}

/*
=======================
=
= T_PullUp2
=
=======================
*/

void T_PullUp2(objtype *ob)
{
	if (ob->xdir == 1)
	{
		xtry = 8*PIXGLOBAL;
		ytry = -8*PIXGLOBAL;
	}
	else
	{
		xtry = -8*PIXGLOBAL;
		ytry = -8*PIXGLOBAL;
	}
}

/*
=======================
=
= T_PullUp3
=
=======================
*/

#pragma argsused
void T_PullUp3(objtype *ob)
{
	ytry = -8*PIXGLOBAL;
}

/*
=======================
=
= T_PulledUp
=
=======================
*/

void T_PulledUp(objtype *ob)
{
	ob->needtoclip = cl_midclip;
	ob->priority = 1;
	ytry = 8*PIXGLOBAL;
}

//===========================================================================

/*
=======================
=
= KeenDieThink
=
=======================
*/

void KeenDieThink(objtype *ob)
{
	DoWeakGravity(ob);
	xtry = ob->xspeed * tics;
	if (!OnScreen(ob))
		playstate = ex_died;
}

/*
=============================================================================

						CONTACT ROUTINES

=============================================================================
*/

/*
============================
=
= KillKeen
=
============================
*/

void KillKeen(void)
{
	if (invincible || godmode)
		return;

	if (player->state != &s_keendead)
	{

		moonok = 0;
		invincible = 30;	//0.43 seconds
		keenkilled = true;
		player->needtoclip = cl_noclip;
		player->priority = 3;
#ifdef KEEN4
		if (mapon == 17)
		{
			if (US_RndT() < 0x80)
			{
				ChangeState(player, &s_keensuitdie1);
			}
			else
			{
				ChangeState(player, &s_keensuitdie2);
			}
		}
		else
#endif
		{
			if (US_RndT() < 0x80)
			{
				ChangeState(player, &s_keendie1);
			}
			else
			{
				ChangeState(player, &s_keendie2);
			}
		}
		SD_PlaySound(SND_KEENDEAD);
		player->yspeed = -40;
		player->xspeed = 16;
	}
}

/*
============================
=
= KeenContact
=
============================
*/


void KeenContact(objtype *ob, objtype *hit)
{
	switch (hit->obclass)
	{
	case bonusobj:
		switch (hit->temp1)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
#ifdef KEEN5
		case 12:
#endif
			SD_PlaySound(bonussound[hit->temp1]);
			hit->obclass = inertobj;
			hit->priority = 3;
			hit->shapenum = bonussprite[hit->temp1];
			GivePoints(bonuspoints[hit->temp1]);
			if (hit->temp1 < 4)
			{
				gamestate.keys[hit->temp1]++;
			}
			else if (hit->temp1 == 10)
			{
				gamestate.lives++;
			}
			else if (hit->temp1 == 11)
			{
				gamestate.ammo += shotsinclip[gamestate.difficulty];
			}
#ifdef KEEN5
			else if (hit->temp1 == 12)
			{
				gamestate.keycard = true;
			}
#endif
			ChangeState(hit, &s_bonusrise);
			CK_RemakeSprite(&hit->sprite, CK_BonusShadows[hit->temp1]);
		}
		break;

#if defined KEEN4
	case oracleobj:
		if (!ob->hitnorth)
			break;

		if (mapon == 14)
		{
			RescueJanitor();
			RF_ForceRefresh();
			RemoveObj(hit);
		}
		else
		{
			SD_PlaySound(SND_LINDSEY);
			playstate = ex_rescued;
		}
		break;
	case stunnedobj:
		if (hit->temp4 != bounderobj)
			break;
		//no break here -- drop through to platformobj is intended!
	case platformobj:
		if (!gamestate.riding)
		{
			ClipToSpriteTop(ob, hit);
		}
		else
			return;
		break;
	case bounderobj:
		ClipToSprite(ob, hit, false);
		break;
	case lindseyobj:
		PrincessLindsey();
		RemoveObj(hit);
		RF_ForceRefresh();
		break;
	case footobj:
		playstate = ex_foot;
		break;
#elif defined KEEN5
	case platformobj:
		if (!gamestate.riding)
			ClipToSpriteTop(ob, hit);
		break;
#elif defined KEEN6
	case stunshotobj:
		if (hit->temp4)
		{
			ExplodeShot(hit);
			ChangeState(ob, &s_keenstun);
		}
		// BUG: there is no break here - this causes the impossible bullet bug
	case platformobj:
		if (!gamestate.riding)
			ClipToSpriteTop(ob, hit);
		break;
#endif
	}
}

/*
============================
=
= KeenPosContact
=
============================
*/

void KeenPosContact(objtype *ob, objtype *hit)
{
	switch (hit->obclass)
	{
#if defined KEEN4
	case platformobj:
		// BUG: priority is not reset here
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_keenjump3);
		jumptime = ob->xspeed = ob->yspeed = 0;
		ClipToSpriteTop(ob, hit);
		break;
	case madmushroomobj:
	case arachnutobj:
	case berkeloidobj:
		KillKeen();
		break;
	case bounderobj:
		ob->priority = 1;
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_keenjump3);
		jumptime = ob->xspeed = ob->yspeed = 0;
		ClipToSprite(ob, hit, false);
		break;
#elif defined KEEN5
	case platformobj:
		// BUG: priority is not reset here
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_keenjump3);
		jumptime = ob->xspeed = ob->yspeed = 0;
		ClipToSpriteTop(ob, hit);
		break;
	case amptonobj:
	case scottieobj:
		ob->priority = 1;
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_keenjump3);
		jumptime = ob->xspeed = ob->yspeed = 0;
		break;
#elif defined KEEN6
	case platformobj:
	case gikobj:
	case flectobj:
	case bloogletobj:
		ob->priority = 1;
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_keenjump3);
		jumptime = ob->xspeed = ob->yspeed = 0;
		ClipToSpriteTop(ob, hit);	// BUG: allows Keen to stand on Blooglets and Flects
		break;
#endif
	}
}

/*
============================
=
= HandleRiding
=
============================
*/

void HandleRiding(objtype *ob)
{
	objtype *plat;

	plat = gamestate.riding;
	if (ob->right < plat->left || ob->left > plat->right)
	{
		gamestate.riding = NULL;
	}
	else if (ob->ymove < 0)
	{
		gamestate.riding = NULL;
		if (plat->ymove < 0)
		{
			xtry = 0;
			ytry = plat->ymove;
			PushObj(ob);
		}
	}
	else
	{
		xtry = plat->xmove;
		ytry = plat->top - ob->bottom - 16;
		PushObj(ob);

#if GRMODE == CGAGR
		if (ob->xmove == plat->xmove)
		{
			ob->x &= ~0x3F;
			ob->x |= plat->x & 0x3F;
		}
#else
		if (nopan)
		{
			ob->x &= ~0x7F;
			ob->x |= plat->x & 0x7F;
		}
		else
		{
			ob->x |= plat->x & 0x1F;
		}
#endif

		if (ob->hitsouth)
		{
			gamestate.riding = NULL;
		}
		else
		{
			ob->hitnorth = 25;
		}
	}
}

/*
============================
=
= TileBonus
=
============================
*/

void TileBonus(Uint16 x, Uint16 y, Uint16 bonus)
{
	CK_SetMapTile(x, y, 0, 1);
	SD_PlaySound(bonussound[bonus]);
	GivePoints(bonuspoints[bonus]);
	if (bonus < 4)
	{
		gamestate.keys[bonus]++;
	}
	else if (bonus == 10)
	{
		gamestate.lives++;
	}
	else if (bonus == 11)
	{
		gamestate.ammo += shotsinclip[gamestate.difficulty];
	}
	GetNewObj(true);
	ck_newobj->obclass = inertobj;
	ck_newobj->priority = 3;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	ck_newobj->ydir = -1;
	ck_newobj->temp2 = ck_newobj->shapenum = bonussprite[bonus];
	CK_SetSprite(&ck_newobj->sprite, CK_BonusShadows[bonus]);
	NewState(ck_newobj, &s_bonusrise);
	ck_newobj->needtoclip = cl_noclip;
}

/*
============================
=
= GiveDrop
=
============================
*/

void GiveDrop(Uint16 x, Uint16 y)
{
	CK_SetMapTile(x, y, 0, 1);
	SD_PlaySound(SND_GETWATER);
	SpawnSplash(x, y);
	if (++gamestate.drops == 100)
	{
		gamestate.drops = 0;
		SD_PlaySound(SND_EXTRAKEEN);
		gamestate.lives++;
		GetNewObj(true);
		ck_newobj->obclass = inertobj;
		ck_newobj->priority = 3;
		ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
		ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y-1);
		ck_newobj->ydir = -1;
		ck_newobj->temp2 = ck_newobj->shapenum = BONUS100UPSPR;
		CK_SetSprite(&ck_newobj->sprite, CKS_SHADOWONEUP);
		NewState(ck_newobj, &s_bonusrise);
		ck_newobj->needtoclip = cl_noclip;
	}
}

/*
============================
=
= CheckInTiles
=
============================
*/

void CheckInTiles(objtype *ob)
{
	Uint16 x, y;
	Uint16 *map;
	Uint16 rowdelta, intile, midx;

	if (moonok == 1)
		moonok = 0;

	map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (ob->tiletop*CK_CurLevelWidth) + ob->tileleft;
	rowdelta = CK_CurLevelWidth - (ob->tileright - ob->tileleft + 1);
	for (y = ob->tiletop; y <= ob->tilebottom; y++, map += rowdelta)
	{
		for (x = ob->tileleft; x <= ob->tileright; x++, map++)
		{
			if ((intile = CK_TileInfo[1][INTILE + *map] & INTILE_TYPEMASK) != 0)
			{
				switch (intile)
				{
				case INTILE_DEADLY:
					KillKeen();
					break;

				case INTILE_DROP:
					GiveDrop(x, y);
					break;

				case INTILE_GEMSOCKET0:
				case INTILE_GEMSOCKET1:
				case INTILE_GEMSOCKET2:
				case INTILE_GEMSOCKET3:
					if (ob->tilebottom != y || !ob->hitnorth
						|| ob->state == &s_keenkey
						|| !gamestate.keys[intile-INTILE_GEMSOCKET0])
					{
						return;
					}

					midx = CONVERT_TILE_TO_GLOBAL(x) + -4*PIXGLOBAL;
					if (ob->x != midx)
					{
						ob->temp1 = midx;
						ob->state = &s_keenlineup;
						return;
					}
					else
					{
						gamestate.keys[intile-INTILE_GEMSOCKET0]--;
						ChangeState(ob, &s_keenkey);
					}
					break;

				case INTILE_MOON:
					if (moonok == 0)
						moonok = 1;
					break;

				case INTILE_BONUS100:
				case INTILE_BONUS200:
				case INTILE_BONUS500:
				case INTILE_BONUS1000:
				case INTILE_BONUS2000:
				case INTILE_BONUS5000:
				case INTILE_EXTRALIFE:
				case INTILE_AMMO:
					TileBonus(x, y, (intile-INTILE_BONUS100)+4);
					break;
				}
			}
		}
	}
}

/*
=============================================================================

						 REACTION ROUTINES

=============================================================================
*/


/*
============================
=
= KeenSimpleReact
=
============================
*/

void KeenSimpleReact(objtype *ob)
{
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}


/*
============================
=
= KeenStandReact
=
============================
*/

void KeenStandReact(objtype *ob)
{
	if (!ob->hitnorth)
	{
	//
	// walked off an edge
	//
		SD_PlaySound(SND_PLUMMET);
		ob->xspeed = ob->xdir * 8;
		ob->yspeed = 0;
		ChangeState(ob, &s_keenjump3);
		jumptime = 0;
	}
	else if ((ob->hitnorth & ~7) == 8)	// deadly floor!
	{
		KillKeen();
	}
	else if (ob->hitnorth == 41)
	{
		xtry = tics * 8;
		ytry = 0;
		ob->temp1 = 0;
		ClipToWalls(ob);
	}
	else if (ob->hitnorth == 49)
	{
		xtry = tics * -8;
		ytry = 0;
		ob->temp1 = 0;
		ClipToWalls(ob);
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);

}

/*
============================
=
= KeenWalkReact
=
============================
*/

void KeenWalkReact(objtype *ob)
{
	if (!ob->hitnorth)
	{
	//
	// walked off an edge
	//
		SD_PlaySound(SND_PLUMMET);
		ob->xspeed = ob->xdir * 8;
		ob->yspeed = 0;
		ChangeState(ob, &s_keenjump3);
		jumptime = 0;
	}
	else if ((ob->hitnorth & ~7) == 8)	// deadly floor!
	{
		KillKeen();
	}
	else if (ob->hitnorth == 41)
	{
		xtry = tics * 8;
		ytry = 0;
		ClipToWalls(ob);
	}
	else if (ob->hitnorth == 49)
	{
		xtry = tics * -8;
		ytry = 0;
		ClipToWalls(ob);
	}
	else if (ob->hiteast && ob->xdir == -1 || ob->hitwest && ob->xdir == 1)
	{
	//
	// ran into a wall
	//
		ob->ticcount = 0;
		ob->state = &s_keenstand;
		ob->shapenum = ob->xdir == 1? s_keenstand.rightshapenum : s_keenstand.leftshapenum;
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);

}

/*
============================
=
= KeenAirReact
=
============================
*/

void KeenAirReact(objtype *ob)
{
	Uint16 *map;
	Uint16 oldtop, graby, ty, tile;

	if (ob->hiteast && ob->xdir == -1 || ob->hitwest && ob->xdir == 1)
		ob->xspeed = 0;

	if (ob->hitsouth)
	{
		if (ob->hitsouth == 17)	// jumping up through a pole hole
		{
			ob->y -= 2*PIXGLOBAL;
			ob->top -= 2*PIXGLOBAL;
			ob->xspeed = 0;
			ob->x = CONVERT_TILE_TO_GLOBAL(ob->tilemidx) - 2*PIXGLOBAL;
		}
		else
		{
#ifdef KEEN6
			if (ob->hitsouth == 33)
			{
				FlipBigSwitch(ob, false);
			}
#endif
			if (!jumpcheat)
			{
				SD_PlaySound(SND_HELMETHIT);
				if (ob->hitsouth > 1)
				{
					ob->yspeed += 16;
					if (ob->yspeed < 0)	// push away from slopes
						ob->yspeed = 0;
				}
				else
				{
					ob->yspeed = 0;
				}
				jumptime = 0;
			}
		}
	}

	if (ob->hitnorth)
	{
		ob->ymove = 0;
		if ((ob->hitnorth & ~7) == 8)	// deadly floor!
		{
			KillKeen();
		}
		else
		{
#if defined KEEN5
			if (ob->hitnorth == 57)
			{
				SD_PlaySound(SND_LANDONFUSE);
			}
#elif defined KEEN6
			if (ob->hitnorth == 33)
			{
				FlipBigSwitch(ob, true);
			}
#endif
			if (ob->hitnorth != 25 || !jumptime)	// KLUDGE to allow jumping off
			{
				ob->temp1 = ob->temp2 = 0;
				if (ob->state == &s_keenairshoot1)
				{
					ChangeState(ob, &s_keenshoot1);
				}
				else if (ob->state == &s_keenairshootup1)
				{
					ChangeState(ob, &s_keenshootup1);
				}
				else if (c.xaxis)
				{
					ChangeState(ob, &s_keenwalk1);
				}
				else
				{
					ChangeState(ob, &s_keenstand);
				}
				SD_PlaySound(SND_LAND);
			}
		}
	}
	else if (ob->ymove > 0)
	{
//
// check if there is an edge to grab
//
		oldtop = ob->top - ob->ymove;
		graby = ((ob->top - 4*PIXGLOBAL) & 0xFF00) + 4*PIXGLOBAL;
		ty = CONVERT_GLOBAL_TO_TILE(graby) - 1;
		if (oldtop < graby && ob->top >= graby)
		{
			if (c.xaxis == -1)
			{
				map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (ty*CK_CurLevelWidth) + ob->tileleft;
				if (ob->hiteast)
					map--;
				tile = *map;
				if (!CK_TileInfo[1][EASTWALL + tile] && !CK_TileInfo[1][WESTWALL + tile]
					&& !CK_TileInfo[1][NORTHWALL + tile] && !CK_TileInfo[1][SOUTHWALL + tile]
					&& CK_TileInfo[1][EASTWALL + map[CK_CurLevelWidth]] && CK_TileInfo[1][NORTHWALL + map[CK_CurLevelWidth]]
					)
				{
					ob->xdir = -1;
					ob->needtoclip = cl_noclip;
					ob->x = (ob->x & 0xFF00) + 8*PIXGLOBAL;
					ob->y = graby - 4*PIXGLOBAL;
					ob->yspeed = ob->ymove = 0;
					ChangeState(ob, &s_keenholdon);
				}
			}
			else if (c.xaxis == 1)
			{
				map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (ty*CK_CurLevelWidth) + ob->tileright;

				if (ob->hitwest)
					map++;
				tile = *map;
				if (!CK_TileInfo[1][EASTWALL + tile] && !CK_TileInfo[1][WESTWALL + tile]
					&& !CK_TileInfo[1][NORTHWALL + tile] && !CK_TileInfo[1][SOUTHWALL + tile]
					&& CK_TileInfo[1][WESTWALL + map[CK_CurLevelWidth]] && CK_TileInfo[1][NORTHWALL + map[CK_CurLevelWidth]]
					)
				{
					ob->xdir = 1;
					ob->needtoclip = cl_noclip;
					ob->x = (ob->x & 0xFF00) + 16*PIXGLOBAL;
					ob->y = graby - 4*PIXGLOBAL;
					ob->yspeed = ob->ymove = 0;
					ChangeState(ob, &s_keenholdon);
				}
			}
		}
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}

#ifdef KEEN5
/*
============================
=
= BreakFuse
=
============================
*/

void BreakFuse(Uint16 tileX, Uint16 tileY)
{
	Uint16 tiles[] = {1932, 1950};	// should be 'static' for less overhead

	// The original disassembly had some code here equivalent to this:
	//
	// _AX = tiles[0];
	// _DX = 4;
	//
	// As it turned out, that was just a compiler quirk.

	SpawnFuseFlash(tileX, tileY);
	if (--gamestate.numfuses == 0)
	{
		SpawnDeadMachine();
	}
	RF_MemToMap(tiles, 1, tileX, tileY, 1, 2);
}
#endif

/*
============================
=
= KeenPogoReact
=
============================
*/

void KeenPogoReact(objtype *ob)
{
	if (ob->hiteast && ob->xdir == -1 || ob->hitwest && ob->xdir == 1)
		ob->xspeed = 0;

	if (ob->hitsouth)
	{
		if (ob->hitsouth == 17)	// jumping up through a pole hole
		{
			ob->y -= 2*PIXGLOBAL;
			ob->top -= 2*PIXGLOBAL;
			ob->xspeed = 0;
			ob->x = CONVERT_TILE_TO_GLOBAL(ob->tilemidx) - 2*PIXGLOBAL;
		}
		else
		{
#ifdef KEEN6
			if (ob->hitsouth == 33)
			{
				FlipBigSwitch(ob, false);
			}
#endif
			if (!jumpcheat)
			{
				SD_PlaySound(SND_HELMETHIT);
				if (ob->hitsouth > 1)
				{
					ob->yspeed += 16;
					if (ob->yspeed < 0)	// push away from slopes
						ob->yspeed = 0;
				}
				else
				{
					ob->yspeed = 0;
				}
				jumptime = 0;
			}
		}
	}

	if (ob->hitnorth)
	{
		ob->ymove = 0;
		if ((ob->hitnorth & ~7) == 8)	// deadly floor!
		{
			KillKeen();
		}
		else
		{
#if defined KEEN5
			if (ob->hitnorth == 57)
			{
				if (ob->yspeed < 48)
				{
					SD_PlaySound(SND_LANDONFUSE);
				}
				else
				{
					BreakFuse(ob->tilemidx, ob->tilebottom);
					RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
					return;
				}
			}
#elif defined KEEN6
			if (ob->hitnorth == 33)
			{
				FlipBigSwitch(ob, true);
			}
			else if (ob->hitnorth == 41)
			{
				ob->xspeed += 8;
				if (ob->xspeed > 8)
					ob->xspeed = 8;
			}
			else if (ob->hitnorth == 49)
			{
				ob->xspeed -= 8;
				if (ob->xspeed < -8)
					ob->xspeed = -8;
			}
#endif
			if (ob->hitnorth != 25 || !jumptime)	// KLUDGE to allow jumping off
			{
				ob->yspeed = -48;
				jumptime = 24;
				SD_PlaySound(SND_POGOBOUNCE);
				ChangeState(ob, &s_keenpogo);
			}
		}
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}

/*
============================
=
= KeenPoleReact
=
============================
*/

void KeenPoleReact(objtype *ob)
{
	Uint16 *map;
	Uint16 ymove;

	map = (Uint16 *)CK_CurLevelData + CK_CurLevelSize + (ob->tilebottom*CK_CurLevelWidth) + ob->tilemidx;
	if (CK_TileInfo[1][NORTHWALL + *map] == 1)
	{
		ymove = (ob->bottom & 0xFF) + 1;
		ob->y -= ymove;
		ob->bottom -= ymove;
		ob->tilebottom--;
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_keenlookdown);
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}

