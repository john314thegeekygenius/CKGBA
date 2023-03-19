/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/



/*
K4_ACT2.C
=========

Contains the following actor types (in this order):

- Wormouth
- Thundercloud & Lightning
- Berkeloid & Fireball
- Inchworm & Foot (in-level)
- Bounder
- Lick
- Platform
- Dropping Platform

*/


#include "CK_Heads.h"


/*
=============================================================================

						  WORMOUTH

=============================================================================
*/

const statetype s_worm      = {WORMOUTHSPR,       WORMOUTHSPR,       slide, true,  true,  4, 16, 0, T_Worm,          NULL,                  R_Walk,    &s_worm};
const statetype s_wormpeek1 = {WORMOUTHPEEKL1SPR, WORMOUTHPEEKL1SPR, step,  false, false, 20,  0, 0, NULL,                   C_Worm,   R_Draw,    &s_wormpeek2};
const statetype s_wormpeek2 = {WORMOUTHPEEKL2SPR, WORMOUTHPEEKL2SPR, step,  false, false,  8,  0, 0, NULL,                   C_Worm,   R_Draw,    &s_wormpeek3};
const statetype s_wormpeek3 = {WORMOUTHPEEKL1SPR, WORMOUTHPEEKL1SPR, step,  false, false, 20,  0, 0, T_WormLookLeft,  C_Worm,   R_Draw,    &s_wormpeek4};
const statetype s_wormpeek4 = {WORMOUTHSPR,       WORMOUTHSPR,       step,  false, false,  8,  0, 0, NULL,                   C_Worm,   R_Draw,    &s_wormpeek5};
const statetype s_wormpeek5 = {WORMOUTHPEEKR1SPR, WORMOUTHPEEKR1SPR, step,  false, false, 20,  0, 0, NULL,                   C_Worm,   R_Draw,    &s_wormpeek6};
const statetype s_wormpeek6 = {WORMOUTHPEEKR2SPR, WORMOUTHPEEKR2SPR, step,  false, false,  8,  0, 0, NULL,                   C_Worm,   R_Draw,    &s_wormpeek7};
const statetype s_wormpeek7 = {WORMOUTHPEEKR1SPR, WORMOUTHPEEKR1SPR, step,  false, false, 20,  0, 0, T_WormLookRight, C_Worm,   R_Draw,    &s_wormpeek8};
const statetype s_wormpeek8 = {WORMOUTHSPR,       WORMOUTHSPR,       step,  false, false,  8,  0, 0, T_WormLook,      NULL,                  R_Draw,    &s_worm};
const statetype s_wormbite1 = {WORMOUTHBITEL1SPR, WORMOUTHBITER1SPR, step,  false, false,  8,  0, 0, NULL,                   C_WormKill, R_Draw,    &s_wormbite2};
const statetype s_wormbite2 = {WORMOUTHBITEL2SPR, WORMOUTHBITER2SPR, step,  false, false,  8,  0, 0, NULL,                   C_WormKill, R_Draw,    &s_wormbite3};
const statetype s_wormbite3 = {WORMOUTHBITEL3SPR, WORMOUTHBITER3SPR, step,  false, false, 16,  0, 0, NULL,                   C_WormKill, R_Draw,    &s_wormbite4};
const statetype s_wormbite4 = {WORMOUTHBITEL2SPR, WORMOUTHBITER2SPR, step,  false, false,  8,  0, 0, NULL,                   C_WormKill, R_Draw,    &s_wormbite5};
const statetype s_wormbite5 = {WORMOUTHBITEL1SPR, WORMOUTHBITER1SPR, step,  false, false,  8,  0, 0, NULL,                   C_WormKill, R_Draw,    &s_worm};
const statetype s_wormstun  = {WORMOUTHSTUNSPR,   WORMOUTHSTUNSPR,   think, false, false, 10,  0, 0, T_Projectile,       NULL,                  R_Stunned, NULL};

/*
===========================
=
= SpawnWormMouth
=
===========================
*/

void SpawnWormMouth(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = wormouthobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) + 0x8F;
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	CK_SetSprite(&ck_newobj->sprite, CKS_WORMOUTH);
	NewState(ck_newobj, &s_worm);
}

/*
===========================
=
= T_WormLookRight
=
===========================
*/

void T_WormLookRight(objtype *ob)
{
	if (player->x > ob->x)
	{
		ob->xdir = 1;
		ob->state = &s_worm;
	}
}

/*
===========================
=
= T_WormLook
=
===========================
*/

void T_WormLook(objtype *ob)
{
	if (player->x > ob->x)
	{
		ob->xdir = 1;
	}
	else
	{
		ob->xdir = -1;
	}
}

/*
===========================
=
= T_WormLookLeft
=
===========================
*/

void T_WormLookLeft(objtype *ob)
{
	if (player->x < ob->x)
	{
		ob->xdir = -1;
		ob->state = &s_worm;
	}
}

/*
===========================
=
= T_Worm
=
===========================
*/

void T_Worm(objtype *ob)
{
	Sint16 xdist, ydist;

	xdist = player->x - ob->x;
	ydist = player->bottom - ob->bottom;
	if ((xdist < -3*TILEGLOBAL || xdist > 3*TILEGLOBAL) && US_RndT() < 6)
	{
		ob->state = &s_wormpeek1;
	}
	else if (ydist >= -TILEGLOBAL && ydist <= TILEGLOBAL)
	{
		if (ob->xdir == 1 && xdist > 8*PIXGLOBAL && xdist < 24*PIXGLOBAL
			|| ob->xdir == -1 && xdist < -8*PIXGLOBAL && xdist > -32*PIXGLOBAL)
		{
			SD_PlaySound(SND_WORMOUTHATTACK);
			ob->state = &s_wormbite1;
		}
	}
}

/*
===========================
=
= C_Worm
=
===========================
*/

void C_Worm(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj)
	{
		StunObj(ob, hit, &s_wormstun);
	}
}

/*
===========================
=
= C_WormKill
=
===========================
*/

void C_WormKill(objtype *ob, objtype *hit)
{
	if (hit->obclass == keenobj)
	{
		if (!(ob->xdir == 1 && ob->x > hit->x || ob->xdir == -1 && ob->x < hit->x))
		{
			KillKeen();
		}
		return;
	}
	else
	{
		C_Worm(ob, hit);
	}
}

/*
=============================================================================

						  THUNDERCLOUD

=============================================================================
*/

const statetype s_cloudsleep   = {CLOUDSPR,       CLOUDSPR,       think,     false, false,  20, 0, 0, NULL,         C_CloudSleep, R_Draw,  NULL};
const statetype s_cloudwake    = {CLOUDACTIVESPR, CLOUDACTIVESPR, step,      false, false, 100, 0, 0, NULL,         NULL,         R_Cloud, &s_cloud};
const statetype s_cloud        = {CLOUDACTIVESPR, CLOUDACTIVESPR, think,     false, false,  20, 0, 0, T_Cloud,      NULL,         R_Cloud, NULL};
const statetype s_cloudalign   = {CLOUDACTIVESPR, CLOUDACTIVESPR, think,     false, false,  20, 0, 0, T_CloudAlign, NULL,         R_Cloud, NULL};
const statetype s_cloudcharge  = {CLOUDACTIVESPR, CLOUDACTIVESPR, stepthink, false, false,  60, 0, 0, T_Velocity,   NULL,         R_Cloud, &s_cloud};
const statetype s_cloudattack1 = {CLOUDCHARGESPR, CLOUDCHARGESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack2};
const statetype s_cloudattack2 = {CLOUDACTIVESPR, CLOUDACTIVESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack3};
const statetype s_cloudattack3 = {CLOUDCHARGESPR, CLOUDCHARGESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack4};
const statetype s_cloudattack4 = {CLOUDACTIVESPR, CLOUDACTIVESPR, step,      false, false,  10, 0, 0, T_CloudShoot, NULL,         R_Draw,  &s_cloudattack5};
const statetype s_cloudattack5 = {CLOUDCHARGESPR, CLOUDCHARGESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack6};
const statetype s_cloudattack6 = {CLOUDACTIVESPR, CLOUDACTIVESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack7};
const statetype s_cloudattack7 = {CLOUDCHARGESPR, CLOUDCHARGESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack8};
const statetype s_cloudattack8 = {CLOUDACTIVESPR, CLOUDACTIVESPR, step,      false, false,  10, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudattack9};
const statetype s_cloudattack9 = {CLOUDCHARGESPR, CLOUDCHARGESPR, step,      false, false,  48, 0, 0, NULL,         NULL,         R_Draw,  &s_cloudcharge};

const statetype s_bolt1 = {BOLT1SPR, BOLT1SPR, step, false, false, 8, 0, 0, NULL, C_Lethal, R_Draw, &s_bolt2};
const statetype s_bolt2 = {BOLT2SPR, BOLT2SPR, step, false, false, 8, 0, 0, NULL, C_Lethal, R_Draw, &s_bolt3};
const statetype s_bolt3 = {BOLT1SPR, BOLT1SPR, step, false, false, 8, 0, 0, NULL, C_Lethal, R_Draw, &s_bolt4};
const statetype s_bolt4 = {BOLT2SPR, BOLT2SPR, step, false, false, 8, 0, 0, NULL, C_Lethal, R_Draw, &s_bolt5};
const statetype s_bolt5 = {BOLT1SPR, BOLT1SPR, step, false, false, 8, 0, 0, NULL, C_Lethal, R_Draw, &s_bolt6};
const statetype s_bolt6 = {BOLT2SPR, BOLT2SPR, step, false, false, 8, 0, 0, NULL, C_Lethal, R_Draw, NULL};

/*
===========================
=
= SpawnCloudster
=
===========================
*/

void SpawnCloudster(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = thundercloudobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	ck_newobj->ydir = ck_newobj->xdir = 1;
	CK_SetSprite(&ck_newobj->sprite, CKS_THUNDERCLOUD);
	NewState(ck_newobj, &s_cloudsleep);
}

/*
===========================
=
= T_Cloud
=
===========================
*/

void T_Cloud(objtype *ob)
{
	if (US_RndT() < tics)
		ob->xdir = ob->x > player->x? -1 : 1;

	AccelerateX(ob, ob->xdir, 10);
	if (player->bottom < ob->bottom || (Sint16)(player->top - ob->bottom) > 64*PIXGLOBAL)
		return;
	if (ob->left < player->right && ob->right > player->left)
	{
		ob->state = &s_cloudalign;
	}
}

/*
===========================
=
= T_CloudAlign
=
===========================
*/

void T_CloudAlign(objtype *ob)
{
	AccelerateX(ob, ob->xdir, 10);
	if (xtry < 0 && (Sint16)((ob->x & 0x7F) + xtry) <= 0)
	{
		xtry = -(ob->x & 0x7F);
		ob->state = &s_cloudattack1;
	}
	if (xtry > 0 && (ob->x & 0x7F) + xtry >= 8*PIXGLOBAL)
	{
		xtry = 8*PIXGLOBAL - (ob->x & 0x7F);
		ob->state = &s_cloudattack1;
	}
}

/*
===========================
=
= R_Cloud
=
===========================
*/

void R_Cloud(objtype *ob)
{
	if (ob->hitwest)
	{
		ob->xspeed = 0;
		ob->xdir = -1;
	}
	else if (ob->hiteast)
	{
		ob->xspeed = 0;
		ob->xdir = 1;
	}
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}

/*
===========================
=
= T_CloudShoot
=
===========================
*/

void T_CloudShoot(objtype *ob)
{
	GetNewObj(true);
	ck_newobj->obclass = lightningobj;
	ck_newobj->active = ac_removable;
	ck_newobj->needtoclip = cl_noclip;
	ck_newobj->x = ob->x + TILEGLOBAL;
	ck_newobj->y = ob->y + TILEGLOBAL;
	CK_SetSprite(&ck_newobj->sprite, CKS_LIGHTNINGBOLT);
	NewState(ck_newobj, &s_bolt1);
	SD_PlaySound(SND_THUNDER);
}

/*
===========================
=
= C_CloudSleep
=
===========================
*/

void C_CloudSleep(objtype *ob, objtype *hit)
{
	if (hit->obclass == keenobj)
	{
		ChangeState(ob, &s_cloudwake);
	}
}

/*
=============================================================================

						  BERKELOID

temp1 = float offset, in global units (added to ob->y when drawing the sprite)
temp2 = float speed, in global units per tic (added to temp1 for every tic)

=============================================================================
*/

const statetype s_berkefloat1  = {BERKEWALKL1SPR,  BERKEWALKR1SPR,  slide, false, true,  6, 8, 0, BerkeThink, C_Berke, BerkeWalkReact, &s_berkefloat2};
const statetype s_berkefloat2  = {BERKEWALKL2SPR,  BERKEWALKR2SPR,  slide, false, true,  6, 8, 0, BerkeThink, C_Berke, BerkeWalkReact, &s_berkefloat3};
const statetype s_berkefloat3  = {BERKEWALKL3SPR,  BERKEWALKR3SPR,  slide, false, true,  6, 8, 0, BerkeThink, C_Berke, BerkeWalkReact, &s_berkefloat4};
const statetype s_berkefloat4  = {BERKEWALKL4SPR,  BERKEWALKR4SPR,  slide, false, true,  6, 8, 0, BerkeThink, C_Berke, BerkeWalkReact, &s_berkefloat1};
const statetype s_berkethrow1  = {BERKETHROWL1SPR, BERKETHROWR1SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow2};
const statetype s_berkethrow2  = {BERKETHROWL1SPR, BERKETHROWR1SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow3};
const statetype s_berkethrow3  = {BERKETHROWL1SPR, BERKETHROWR1SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow4};
const statetype s_berkethrow4  = {BERKETHROWL1SPR, BERKETHROWR1SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow5};
const statetype s_berkethrow5  = {BERKETHROWL1SPR, BERKETHROWR1SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow6};
const statetype s_berkethrow6  = {BERKETHROWL1SPR, BERKETHROWR1SPR, step,  false, false, 6, 0, 0, BerkeThrowThink, C_Berke, BerkeDrawReact, &s_berkethrow7};
const statetype s_berkethrow7  = {BERKETHROWL2SPR, BERKETHROWR2SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow8};
const statetype s_berkethrow8  = {BERKETHROWL2SPR, BERKETHROWR2SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow9};
const statetype s_berkethrow9  = {BERKETHROWL2SPR, BERKETHROWR2SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow10};
const statetype s_berkethrow10 = {BERKETHROWL2SPR, BERKETHROWR2SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow11};
const statetype s_berkethrow11 = {BERKETHROWL2SPR, BERKETHROWR2SPR, step,  false, false, 6, 0, 0, SetReactThink, C_Berke, BerkeDrawReact, &s_berkethrow12};
const statetype s_berkethrow12 = {BERKETHROWL2SPR, BERKETHROWR2SPR, step,  false, false, 6, 0, 0, BerkeThrowDone, C_Berke, BerkeDrawReact, &s_berkefloat1};

const statetype s_fire1     = {FIREBALL1SPR, FIREBALL1SPR, stepthink, false, false, 6, 0, 0, T_WeakProjectile, C_Lethal, FireReact, &s_fire2};
const statetype s_fire2     = {FIREBALL2SPR, FIREBALL2SPR, stepthink, false, false, 6, 0, 0, T_WeakProjectile, C_Lethal, FireReact, &s_fire1};
const statetype s_fireland1 = {FIREBALL1SPR, FIREBALL1SPR, step, false, false,  6, 0, 0, NULL, C_Berke, R_Draw, &s_fireland2};
const statetype s_fireland2 = {FIREBALL3SPR, FIREBALL3SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland3};
const statetype s_fireland3 = {FIREBALL4SPR, FIREBALL4SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland4};
const statetype s_fireland4 = {FIREBALL3SPR, FIREBALL3SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland5};
const statetype s_fireland5 = {FIREBALL4SPR, FIREBALL4SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland6};
const statetype s_fireland6 = {FIREBALL1SPR, FIREBALL1SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland7};
const statetype s_fireland7 = {FIREBALL2SPR, FIREBALL2SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland8};
const statetype s_fireland8 = {FIREBALL1SPR, FIREBALL1SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, &s_fireland9};
const statetype s_fireland9 = {FIREBALL2SPR, FIREBALL2SPR, step, false, false, 12, 0, 0, NULL, C_Berke, R_Draw, NULL};

/*
===========================
=
= SpawnBerkeloid
=
===========================
*/

void SpawnBerkeloid(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = berkeloidobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) + -2*TILEGLOBAL;
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	ck_newobj->temp2 = 8;
	CK_SetSprite(&ck_newobj->sprite, CKS_BERKELOID);
	NewState(ck_newobj, &s_berkefloat1);
}


/*
===========================
=
= BerkeThink
=
===========================
*/

void BerkeThink(objtype *ob)
{
	Sint16 xdist, ydist;

	if (US_RndT() < 0x20)
		ob->xdir = player->x < ob->x? -1 : 1;

	if (US_RndT() < 8)
	{
		// BUG: might be about to move off a ledge, causing it to get stuck
		// after throwing (the throw states don't use BerkeWalkReact)!
		// To avoid that, set xtry to 0 here.

		ob->state = &s_berkethrow1;
		// BUG? this doesn't play the attack sound
	}
	else if (US_RndT() <= 0x40)
	{
		xdist = player->x - ob->x;
		ydist = player->y - ob->y;
		if (ydist >= -TILEGLOBAL && ydist <= TILEGLOBAL
			&& (ob->xdir == 1 && xdist > 0 || ob->xdir == -1 && xdist < 0))
		{
			// BUG: might be about to move off a ledge, causing it to get stuck
			// after throwing (the throw states don't use BerkeWalkReact)!
			// To avoid that, set xtry to 0 here.

			ob->state = &s_berkethrow1;
			SD_PlaySound(SND_BERKELOIDATTACK);
		}
	}
}

/*
===========================
=
= BerkeThrowThink
=
===========================
*/

void BerkeThrowThink(objtype *ob)
{
	GetNewObj(true);
	ck_newobj->active = ac_removable;
	ck_newobj->obclass = berkeloidobj;
	ck_newobj->y = ob->y + 8*PIXGLOBAL;
	ck_newobj->yspeed = -8;
	if (ob->xdir == 1)
	{
		ck_newobj->xspeed = 48;
		ck_newobj->x = ob->x + 32*PIXGLOBAL;
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xspeed = -48;
		ck_newobj->x = ob->x - 16*PIXGLOBAL;
		ck_newobj->xdir = -1;
	}
	CK_SetSprite(&ck_newobj->sprite, CKS_FLAME);
	NewState(ck_newobj, &s_fire1);
	ob->needtoreact = true;
}

/*
===========================
=
= BerkeThrowDone
=
===========================
*/

void BerkeThrowDone(objtype *ob)
{
	ob->nothink = 4;
	ob->needtoreact = true;
}

/*
===========================
=
= C_Berke
=
===========================
*/

void C_Berke(objtype *ob, objtype *hit)
{
	ob++;			// shut up compiler
	if (hit->obclass == stunshotobj)
	{
		ExplodeShot(hit);
	}
	else if (hit->obclass == keenobj)
	{
		KillKeen();
	}
}

/*
===========================
=
= FireReact
=
===========================
*/

void FireReact(objtype *ob)
{
	if (ob->hiteast || ob->hitwest)
		ob->xspeed = 0;

	if (ob->hitnorth)
	{
		SD_PlaySound(SND_FIREBALLLAND);
		ChangeState(ob, &s_fireland1);
	}
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}

/*
===========================
=
= BerkeDrawReact
=
===========================
*/

void BerkeDrawReact(objtype *ob)
{
	//float up & down:
	ob->temp1 = ob->temp1 + ob->temp2 * tics;
	if (ob->temp1 > 0)
	{
		ob->temp1 = 0;
		ob->temp2 = -8;
	}
	else if (ob->temp1 < -TILEGLOBAL)
	{
		ob->temp1 = -TILEGLOBAL;
		ob->temp2 = 8;
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y+ob->temp1, ob->shapenum, spritedraw, 0);
}

/*
===========================
=
= BerkeWalkReact
=
===========================
*/

void BerkeWalkReact(objtype *ob)
{
	if (ob->xdir == 1 && ob->hitwest)
	{
		ob->x -= ob->xmove;
		ob->xdir = -1;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state);
	}
	else if (ob->xdir == -1 && ob->hiteast)
	{
		ob->x -= ob->xmove;
		ob->xdir = 1;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state);
	}
	else if (!ob->hitnorth)
	{
		ob->x -= ob->xmove*2;
		ob->xdir = -ob->xdir;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state);
	}
	BerkeDrawReact(ob);
}

/*
=============================================================================

						  INCHWORM & FOOT

temp1 = last lasttimecount (for resetting the touch counter after each frame)
temp2 = touch counter

=============================================================================
*/

statetype const s_footsmoke1 = {SMOKE1SPR, SMOKE1SPR, step, false, false, 12, 0, 0, NULL, NULL, R_Draw, &s_footsmoke2};
statetype const s_footsmoke2 = {SMOKE2SPR, SMOKE2SPR, step, false, false, 12, 0, 0, NULL, NULL, R_Draw, &s_footsmoke3};
statetype const s_footsmoke3 = {SMOKE3SPR, SMOKE3SPR, step, false, false, 12, 0, 0, NULL, NULL, R_Draw, &s_footsmoke4};
statetype const s_footsmoke4 = {SMOKE4SPR, SMOKE4SPR, step, false, false, 12, 0, 0, NULL, NULL, R_Draw, NULL};
statetype const s_inch1      = {INCHWORML1SPR, INCHWORMR1SPR, step, false, true, 30, 128, 0, InchThink, InchContact, R_Walk, &s_inch2};
statetype const s_inch2      = {INCHWORML2SPR, INCHWORMR2SPR, step, false, true, 30, 128, 0, InchThink, InchContact, R_Walk, &s_inch1};
statetype const s_footchange = { -1,  -1, step, false, false, 48, 0, 0, NULL, NULL, R_Draw, &s_footwait};	//never used!
statetype const s_footwait   = {FOOTSPR, FOOTSPR, think, false, false, 30000, 0, 0, NULL, FootContact, R_Draw, NULL};

/*
===========================
=
= SpawnInchworm
=
===========================
*/

void SpawnInchworm(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = inchwormobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	CK_SetSprite(&ck_newobj->sprite, CKS_INCHWORM);
	NewState(ck_newobj, &s_inch1);
	ck_newobj->ticcount = US_RndT() / 32;
}

/*
===========================
=
= SpawnFoot
=
===========================
*/

void SpawnFoot(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = footobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y-3);
	CK_SetSprite(&ck_newobj->sprite, CKS_FOOT);
	NewState(ck_newobj, &s_footwait);
}

/*
===========================
=
= InchThink
=
===========================
*/

void InchThink(objtype *ob)
{
	if (ob->x > player->x)
	{
		ob->xdir = -1;
	}
	else
	{
		ob->xdir = 1;
	}
}

/*
===========================
=
= InchContact
=
===========================
*/

void InchContact(objtype *ob, objtype *hit)
{
	objtype *ob2;

	if (hit->obclass != inchwormobj)
		return;

	if (ob->temp1 != (Sint16)lasttimecount)
	{
		ob->temp1 = (Sint16)lasttimecount;
		ob->temp2 = 0;
	}

	if (++ob->temp2 != 11)	//11 instead of 12 since the object can't contact itself
		return;

	//change current inchworm into a foot:
	SD_PlaySound(SND_MAKEFOOT);
	ob->y -= 5*TILEGLOBAL;
	ob->obclass = footobj;
	ChangeState(ob, &s_footwait);
	CK_RemakeSprite(&ob->sprite, CKS_FOOT);

	//Note: It would make more sense to remove the remaining inchworm BEFORE
	//spawning the smoke, just in case there are not enough free spots in the
	//objarray to spawn the smoke. The game won't crash either way, though.

	//spawn smoke:
	GetNewObj(true);
	ck_newobj->x = ob->x -  8*PIXGLOBAL;
	ck_newobj->y = ob->y + 16*PIXGLOBAL;
	ck_newobj->priority = 3;
	CK_SetSprite(&ck_newobj->sprite, CKS_TESMOKE);
	NewState(ck_newobj, &s_footsmoke1);

	GetNewObj(true);
	ck_newobj->x = ob->x + 16*PIXGLOBAL;
	ck_newobj->y = ob->y + 24*PIXGLOBAL;
	ck_newobj->priority = 3;
	CK_SetSprite(&ck_newobj->sprite, CKS_TESMOKE);
	NewState(ck_newobj, &s_footsmoke1);

	GetNewObj(true);
	ck_newobj->x = ob->x + 40*PIXGLOBAL;
	ck_newobj->y = ob->y + 16*PIXGLOBAL;
	ck_newobj->priority = 3;
	CK_SetSprite(&ck_newobj->sprite, CKS_TESMOKE);
	NewState(ck_newobj, &s_footsmoke1);

	GetNewObj(true);
	ck_newobj->x = ob->x;
	ck_newobj->y = ob->y - 8*PIXGLOBAL;
	ck_newobj->priority = 3;
	CK_SetSprite(&ck_newobj->sprite, CKS_TESMOKE);
	NewState(ck_newobj, &s_footsmoke1);

	//remove ALL inchworm from the level:
	for(int i = player->uuid; i < CK_NumOfObjects; i++){
        ob2 = &CK_ObjectList[i];
        if(ob2->removed) continue;
		if (ob2->obclass == inchwormobj)
			RemoveObj(ob2);
	}
}

/*
===========================
=
= FootContact
=
===========================
*/

void FootContact(objtype *ob, objtype *hit)	//completely useless
{
//	ob++;			// shut up compiler
//	hit++;			// shut up compiler
}


/*
=============================================================================

						  BOUNDER

temp1 = if non-zero, pick a new (random) direction when hitting the ground
        Makes the Bounder jump straight up at least once after having jumped
		  left/right (unless Keen is riding the Bounder)
temp2 = jump counter to make the Bounder jump straight up at least twice
        after Keen has fallen/jumped off

=============================================================================
*/

const statetype s_bounderup1   = {BOUNDERC1SPR,   BOUNDERC1SPR,   stepthink, false, false, 20, 0, 0, T_Projectile, C_Bounder, R_Bounder, &s_bounderup2};
const statetype s_bounderup2   = {BOUNDERC2SPR,   BOUNDERC2SPR,   stepthink, false, false, 20, 0, 0, T_Projectile, C_Bounder, R_Bounder, &s_bounderup1};
const statetype s_bounderside1 = {BOUNDERL1SPR,   BOUNDERR1SPR,   stepthink, false, false, 20, 0, 0, T_Projectile, C_Bounder, R_Bounder, &s_bounderside2};
const statetype s_bounderside2 = {BOUNDERL2SPR,   BOUNDERR2SPR,   stepthink, false, false, 20, 0, 0, T_Projectile, C_Bounder, R_Bounder, &s_bounderside1};
const statetype s_bounderstun  = {BOUNDERC1SPR,   BOUNDERC1SPR,   think,     false, false,  0, 0, 0, T_Projectile, NULL, R_Stunned, &s_bounderstun2};
const statetype s_bounderstun2 = {BOUNDERSTUNSPR, BOUNDERSTUNSPR, think,     false, false,  0, 0, 0, T_Projectile, NULL, R_Stunned, NULL};

/*
===========================
=
= SpawnBounder
=
===========================
*/

void SpawnBounder(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = bounderobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) - 8*PIXGLOBAL;
	ck_newobj->ydir = 1;
	ck_newobj->xdir = 0;
    CK_SetSprite(&ck_newobj->sprite, CKS_BOUNDER);
	NewState(ck_newobj, &s_bounderup1);
}

/*
===========================
=
= C_Bounder
=
===========================
*/

void C_Bounder(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj)
	{
		//basically StunObj(), but in different order:
		ob->temp1 = 0;
		ob->temp2 = 0;
		ob->temp3 = 0;
		ob->temp4 = ob->obclass;
		ExplodeShot(hit);
		ChangeState(ob, &s_bounderstun);
		ob->obclass = stunnedobj;

		ob->yspeed -= 32;
	}
}

/*
===========================
=
= R_Bounder
=
===========================
*/

void R_Bounder(objtype *ob)
{
	Sint16 randnum;

	if (ob->hitsouth)
		ob->yspeed = 0;

	if (ob->hitnorth)
	{
		ob->temp2++;
		if (OnScreen(ob))
			SD_PlaySound(SND_BOUNCE2);

		ob->yspeed = -50;
		if (gamestate.riding == ob)
		{
			ob->temp2 = 0;
			if (player->left < ob->left-4*PIXGLOBAL)
			{
				ob->xdir = -1;
			}
			else if (player->right > ob->right+4*PIXGLOBAL)
			{
				ob->xdir = 1;
			}
			else
			{
				ob->xdir = 0;
			}
			ob->xspeed = ob->xdir * 24;
		}
		else if (ob->temp2 <= 2 || ob->temp1 == 0)
		{
			ob->temp1 = 1;
			ob->xdir = ob->xspeed = 0;
			ChangeState(ob, &s_bounderup1);
		}
		else
		{
			ob->temp1 = 0;
			randnum = US_RndT();
			if (randnum < 100)
			{
				ob->xdir = -1;
			}
			else if (randnum < 200)
			{
				ob->xdir = 1;
			}
			else
			{
				ob->xdir = 0;
			}
			ob->xspeed = ob->xdir * 24;
		}

		if (ob->xdir)
		{
			ChangeState(ob, &s_bounderside1);
		}
		else
		{
			ChangeState(ob, &s_bounderup1);
		}
	}

	if (ob->hiteast || ob->hitwest)
	{
		ob->xdir = -ob->xdir;
		ob->xspeed = -ob->xspeed;
	}

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}



/*
=============================================================================

						  LICK

=============================================================================
*/

const statetype s_lick1     = {LICKMOVEL1SPR,   LICKMOVER1SPR,   step,  false, false, 10, 0, 0, LickJumpThink, LickContact, R_Draw, &s_lick2};
const statetype s_lick2     = {LICKMOVEL2SPR,   LICKMOVER2SPR,   think, false, false,  0, 0, 0, T_Projectile, LickContact, LickAirReact, &s_lick3};
const statetype s_lick3     = {LICKMOVEL3SPR,   LICKMOVER3SPR,   think, false, false,  0, 0, 0, T_Projectile, LickContact, LickAirReact, NULL};
const statetype s_lick4     = {LICKMOVEL4SPR,   LICKMOVER4SPR,   step,  false, false, 10, 0, 0, NULL, LickContact, R_Draw, &s_lick1};
const statetype s_licklick1 = {LICKATTACKL1SPR, LICKATTACKR1SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick2};
const statetype s_licklick2 = {LICKATTACKL2SPR, LICKATTACKR2SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick3};
const statetype s_licklick3 = {LICKATTACKL3SPR, LICKATTACKR3SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick4};
const statetype s_licklick4 = {LICKATTACKL2SPR, LICKATTACKR2SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick5};
const statetype s_licklick5 = {LICKATTACKL1SPR, LICKATTACKR1SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick6};
const statetype s_licklick6 = {LICKATTACKL2SPR, LICKATTACKR2SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick7};
const statetype s_licklick7 = {LICKATTACKL3SPR, LICKATTACKR3SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_licklick8};
const statetype s_licklick8 = {LICKATTACKL2SPR, LICKATTACKR2SPR, step,  true,  false,  4, 0, 0, NULL, LickKillContact, R_Draw, &s_lick3};
const statetype s_lickstun  = {LICKSTUNSPR,     LICKSTUNSPR,     think, false, false,  0, 0, 0, T_Projectile, NULL, R_Stunned, &s_lickstun2};
const statetype s_lickstun2 = {LICKSTUNSPR,     LICKSTUNSPR,     think, false, false,  0, 0, 0, T_Projectile, NULL, R_Stunned, NULL};

/*
===========================
=
= SpawnLick
=
===========================
*/

void SpawnLick(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = lickobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	ck_newobj->nothink = US_RndT() / 64;
	CK_SetSprite(&ck_newobj->sprite, CKS_LICK);
	NewState(ck_newobj, &s_lick3);
}

/*
===========================
=
= LickJumpThink
=
===========================
*/

void LickJumpThink(objtype *ob)
{
	Sint16 xdist, ydist;

	if (ob->x > player->x)
	{
		ob->xdir = -1;
	}
	else
	{
		ob->xdir = 1;
	}

	xdist = player->x - ob->x;
	ydist = player->y - ob->y;

	if (ydist >= -TILEGLOBAL && ydist <= TILEGLOBAL	&& 
		( ob->xdir == 1 && xdist > -2*PIXGLOBAL && xdist < 24*PIXGLOBAL
		 || ob->xdir == -1 && xdist < 2*PIXGLOBAL && xdist > -32*PIXGLOBAL ) )
	{
		SD_PlaySound(SND_LICKATTACK);
		ob->state = &s_licklick1;
	}
	else if (abs(xdist) > 3*TILEGLOBAL)
	{
		ob->xspeed = ob->xdir * 32;
		ob->yspeed = -32;
	}
	else
	{
		ob->xspeed = (ob->xdir * 32)/2;
		ob->yspeed = -16;
	}
}

/*
===========================
=
= LickContact
=
===========================
*/

void LickContact(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj)
	{
		StunObj(ob, hit, &s_lickstun);
		ob->yspeed -= 16;
	}
}

/*
===========================
=
= LickKillContact
=
===========================
*/

void LickKillContact(objtype *ob, objtype *hit)
{
	if (hit->obclass == keenobj)
	{
		if (ob->xdir == 1 && player->x > ob->x 
			|| ob->xdir == -1 && player->x < ob->x)
		{
			KillKeen();
		}
	}
	else
	{
		LickContact(ob, hit);
	}
}

/*
===========================
=
= LickAirReact
=
===========================
*/

void LickAirReact(objtype *ob)
{
	if (ob->hitnorth)
		ChangeState(ob, &s_lick4);

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}

/*
=============================================================================

						  PLATFORM

temp2 = additional sprite pointer for thruster sprites
temp3 = additional sprite pointer for thruster sprites

=============================================================================
*/

const statetype s_platform = {PLATFORMSPR, PLATFORMSPR, think, false, false, 0, 0, 0, T_Platform, NULL, R_Platform, NULL};

/*
===========================
=
= SpawnPlatform
=
===========================
*/

void SpawnPlatform(Sint16 x, Sint16 y, Sint16 dir)
{
	GetNewObj(false);
	ck_newobj->obclass = platformobj;
	ck_newobj->active = ac_allways;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	switch (dir)
	{
	case 0:
		ck_newobj->xdir = 0;
		ck_newobj->ydir = -1;
		break;
	case 1:
		ck_newobj->xdir = 1;
		ck_newobj->ydir = 0;
		break;
	case 2:
		ck_newobj->xdir = 0;
		ck_newobj->ydir = 1;
		break;
	case 3:
		ck_newobj->xdir = -1;
		ck_newobj->ydir = 0;
		break;
	}
	CK_SetSprite(&ck_newobj->sprite, CKS_ELEVATOR);
	NewState(ck_newobj, &s_platform);
}

/*
===========================
=
= T_Platform
=
===========================
*/

void T_Platform(objtype *ob)
{
	Uint16 newpos, newtile;

	xtry = ob->xdir * 12 * tics;
	ytry = ob->ydir * 12 * tics;

	if (ob->xdir == 1)
	{
		newpos = ob->right + xtry;
		newtile = CONVERT_GLOBAL_TO_TILE(newpos);
		if (ob->tileright != newtile)
		{
			if (CK_GetInfo((ob->tiletop*CK_CurLevelWidth)+newtile) == 31)
			{
				ob->xdir = -1;
				xtry = xtry - (newpos & 0xFF);
			}
		}
	}
	else if (ob->xdir == -1)
	{
		newpos = ob->left + xtry;
		newtile = CONVERT_GLOBAL_TO_TILE(newpos);
		if (ob->tileleft != newtile)
		{
			if (CK_GetInfo((ob->tiletop*CK_CurLevelWidth)+newtile) == 31)
			{
				ob->xdir = 1;
				xtry = xtry + (0x100 - (newpos & 0xFF));
			}
		}
	}
	else if (ob->ydir == 1)
	{
		newpos = ob->bottom + ytry;
		newtile = CONVERT_GLOBAL_TO_TILE(newpos);
		if (ob->tilebottom != newtile)
		{
			if (CK_GetInfo((newtile*CK_CurLevelWidth)+ob->tileleft) == 31)
			{
				if (CK_GetInfo(((newtile-2)*CK_CurLevelWidth)+ob->tileleft) == 31)
				{
					ytry = 0;
					ob->needtoreact = true;
				}
				else
				{
					ob->ydir = -1;
					ytry = ytry - (newpos & 0xFF);
				}
			}
		}
	}
	else if (ob->ydir == -1)
	{
		newpos = ob->top + ytry;
		newtile = CONVERT_GLOBAL_TO_TILE(newpos);
		if (ob->tiletop != newtile)
		{
			if (CK_GetInfo((newtile*CK_CurLevelWidth)+ob->tileleft) == 31)
			{
				if (CK_GetInfo(((newtile+2)*CK_CurLevelWidth)+ob->tileleft) == 31)
				{
					ytry = 0;
					ob->needtoreact = true;
				}
				else
				{
					ob->ydir = 1;
					ytry = ytry + (0x100 - (newpos & 0xFF));
				}
			}
		}
	}
}

/*
===========================
=
= R_Platform
=
===========================
*/

void R_Platform(objtype *ob)
{
	Uint16 frame;

	//place platform sprite:
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);

	// Generate the sprites if needed
	if(!ob->temp2){
		ob->temp2 = (Sint32)CK_GetNewSprite();
		CK_SetSprite((objsprite**)(&ob->temp2), CKS_EFIRE);
	}
	if(!ob->temp3){
		ob->temp3 = (Sint32)CK_GetNewSprite();
		CK_SetSprite((objsprite**)(&ob->temp3), CKS_EFIRE);
	}

	//place (or remove) thruster sprites:
	frame = (lasttimecount >> 2) & 1;
	if (ob->xdir == 1)
	{
		RF_PlaceSprite((objsprite**)&ob->temp2, ob->x-1*PIXGLOBAL, ob->y+3*PIXGLOBAL, frame+PLATSIDETHRUST1SPR, spritedraw, 0);
		if (ob->temp3)
			RF_RemoveSprite(&ob->temp3, true);
	}
	else if (ob->xdir == -1)
	{
		if (ob->temp2)
			RF_RemoveSprite(&ob->temp2, true);
		RF_PlaceSprite(&ob->temp3, ob->x+48*PIXGLOBAL, ob->y+5*PIXGLOBAL, frame+PLATSIDETHRUST1SPR, spritedraw, 1);
	}
	else if (ob->ydir == -1)
	{
		RF_PlaceSprite(&ob->temp2, ob->x+2*PIXGLOBAL, ob->y+9*PIXGLOBAL, frame+PLATLTHRUST1SPR, spritedraw, 0);
		RF_PlaceSprite(&ob->temp3, ob->x+46*PIXGLOBAL, ob->y+8*PIXGLOBAL, frame+PLATRTHRUST1SPR, spritedraw, 0);
	}
	else if (ob->ydir == 1)
	{
		if (frame)
		{
			RF_PlaceSprite(&ob->temp2, ob->x+2*PIXGLOBAL, ob->y+9*PIXGLOBAL, frame+PLATLTHRUST1SPR, spritedraw, 0);
			RF_PlaceSprite(&ob->temp3, ob->x+46*PIXGLOBAL, ob->y+8*PIXGLOBAL, frame+PLATRTHRUST1SPR, spritedraw, 0);
		}
		else
		{
			if (ob->temp2)
				RF_RemoveSprite(&ob->temp2, true);
			if (ob->temp3)
				RF_RemoveSprite(&ob->temp3, true);
		}
	}
}

/*
=============================================================================

						  DROPPING PLATFORM

temp1 = initial y position

=============================================================================
*/

const statetype s_dropplatsit  = {PLATFORMSPR, PLATFORMSPR, think,      false, false, 0, 0,   0, T_DropPlatSit, NULL, R_Draw, NULL};
const statetype s_dropplatfall = {PLATFORMSPR, PLATFORMSPR, think,      false, false, 0, 0,   0, T_DropPlatFall, NULL, R_Draw, NULL};
const statetype s_dropplatrise = {PLATFORMSPR, PLATFORMSPR, slidethink, false, false, 0, 0, -32, T_DropPlatRise, NULL, R_Draw, NULL};

/*
===========================
=
= SpawnDropPlat
=
===========================
*/

void SpawnDropPlat(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = platformobj;
	ck_newobj->active = ac_allways;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = ck_newobj->temp1 = CONVERT_TILE_TO_GLOBAL(y);
	ck_newobj->xdir = 0;
	ck_newobj->ydir = 1;
	ck_newobj->needtoclip = cl_noclip;
	CK_SetSprite(&ck_newobj->sprite, CKS_ELEVATOR);
	NewState(ck_newobj, &s_dropplatsit);
}

/*
===========================
=
= T_DropPlatSit
=
===========================
*/

void T_DropPlatSit(objtype *ob)
{
	if (gamestate.riding == ob)
	{
		ytry = tics * 16;
		ob->yspeed = 0;
		if (ob->y + ytry - ob->temp1 >= 8*PIXGLOBAL)
			ob->state = &s_dropplatfall;
	}
}

/*
===========================
=
= T_DropPlatFall
=
===========================
*/

void T_DropPlatFall(objtype *ob)
{
	Uint16 newy, ty;

	DoGravity(ob);

	if (ytry >= 15*PIXGLOBAL)
		ytry = 15*PIXGLOBAL;

	newy = ob->bottom + ytry;
	ty = CONVERT_GLOBAL_TO_TILE(newy);
	if (ob->tilebottom != ty)
	{
		if (CK_GetInfo((ty*CK_CurLevelWidth)+ob->tileleft) == 31)
		{
			ytry = 0xFF - (ob->bottom & 0xFF);
			if (gamestate.riding != ob)
				ob->state = &s_dropplatrise;
		}
	}
}

/*
===========================
=
= T_DropPlatRise
=
===========================
*/

void T_DropPlatRise(objtype *ob)
{
	if (gamestate.riding == ob)
	{
		ob->yspeed = 0;
		ob->state = &s_dropplatfall;
	}
	else if (ob->y <= ob->temp1)
	{
		ytry = ob->temp1 - ob->y;
		ob->state = &s_dropplatsit;
	}
}
