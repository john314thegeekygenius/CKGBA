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
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) + -8*PIXGLOBAL;
	ck_newobj->ydir = 1;
	ck_newobj->xdir = 0;
	NewState(ck_newobj, &s_bounderup1);
    CK_SetSprite(ck_newobj, CKS_BOUNDER);
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

	RF_PlaceSprite(ob, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}
