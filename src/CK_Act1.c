/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

/* Reconstructed Commander Keen 4-6 Source Code
 * Copyright (C) 2021 K1n9_Duk3
 *
 * This file is primarily based on:
 * Catacomb 3-D Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
K4_ACT1.C
=========

Contains the following actor types (in this order):

- Miragia (world map)
- Bonus Items
- Council Member
- Poison Slug
- Mad Mushroom
- Egg & Eggbird
- Arachnut
- Skypest

*/

#include "CK_Heads.h"



/*
=============================================================================

						  MIRAGIA

=============================================================================
*/

const statetype s_miragia0 = {0, 0, step, false, false, 300, 0, 0, T_Miragia0, NULL, NULL, &s_miragia1};
const statetype s_miragia1 = {0, 0, step, false, false,  30, 0, 0, T_Miragia1, NULL, NULL, &s_miragia2};
const statetype s_miragia2 = {0, 0, step, false, false,  30, 0, 0, T_Miragia2, NULL, NULL, &s_miragia3};
const statetype s_miragia3 = {0, 0, step, false, false,  30, 0, 0, T_Miragia3, NULL, NULL, &s_miragia4};
const statetype s_miragia4 = {0, 0, step, false, false, 300, 0, 0, T_Miragia4, NULL, NULL, &s_miragia5};
const statetype s_miragia5 = {0, 0, step, false, false,  30, 0, 0, T_Miragia5, NULL, NULL, &s_miragia6};
const statetype s_miragia6 = {0, 0, step, false, false,  30, 0, 0, T_Miragia6, NULL, NULL, &s_miragia7};
const statetype s_miragia7 = {0, 0, step, false, false,  30, 0, 0, T_Miragia7, NULL, NULL, &s_miragia0};

/*
===========================
=
= SpawnMiragia
=
===========================
*/

void SpawnMiragia(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = inertobj;
	ck_newobj->active = ac_allways;
	ck_newobj->x = x;
	ck_newobj->y = y;
	ck_newobj->state = &s_miragia0;
}

/*
===========================
=
= T_Miragia0
=
===========================
*/

void T_Miragia0(objtype *ob)
{
	if (player->tileright >= ob->x && player->tileleft <= ob->x+5
		&& player->tiletop >= ob->y && player->tilebottom <= ob->y+4)
	{
		//don't let miragia appear if player is in the area, so player won't get stuck
		ob->state = &s_miragia7;
	}
	else
	{
		RF_MapToMap(8, 60, ob->x, ob->y, 6, 4);
	}
}

/*
===========================
=
= T_Miragia1
=
===========================
*/

void T_Miragia1(objtype *ob)
{
	RF_MapToMap(14, 60, ob->x, ob->y, 6, 4);
}

/*
===========================
=
= T_Miragia2
=
===========================
*/

void T_Miragia2(objtype *ob)
{
	RF_MapToMap(20, 60, ob->x, ob->y, 6, 4);
}

/*
===========================
=
= T_Miragia3
=
===========================
*/

void T_Miragia3(objtype *ob)
{
	RF_MapToMap(26, 60, ob->x, ob->y, 6, 4);
}

/*
===========================
=
= T_Miragia4
=
===========================
*/

void T_Miragia4(objtype *ob)
{
	RF_MapToMap(20, 60, ob->x, ob->y, 6, 4);
}

/*
===========================
=
= T_Miragia5
=
===========================
*/

void T_Miragia5(objtype *ob)
{
	RF_MapToMap(14, 60, ob->x, ob->y, 6, 4);
}

/*
===========================
=
= T_Miragia6
=
===========================
*/

void T_Miragia6(objtype *ob)
{
	RF_MapToMap(8, 60, ob->x, ob->y, 6, 4);
}

/*
===========================
=
= T_Miragia7
=
===========================
*/

void T_Miragia7(objtype *ob)
{
	RF_MapToMap(2, 60, ob->x, ob->y, 6, 4);
}


/*
=============================================================================

						  BONUS ITEMS

temp1 = bonus type
temp2 = base shape number
temp3 = last animated shape number +1

=============================================================================
*/

const statetype s_bonus1 = {0, 0, step, false, false, 20, 0, 0, T_Bonus, NULL, R_Draw, &s_bonus2};
const statetype s_bonus2 = {0, 0, step, false, false, 20, 0, 0, T_Bonus, NULL, R_Draw, &s_bonus1};
const statetype s_bonusrise = {0, 0, slide, false, false, 40, 0, 8, NULL, NULL, R_Draw, NULL};

const statetype s_splash1 = {DROPSPLASH1SPR, DROPSPLASH1SPR, step, false, false, 10, 0, 0, NULL, NULL, R_Draw, &s_splash2};
const statetype s_splash2 = {DROPSPLASH2SPR, DROPSPLASH2SPR, step, false, false, 10, 0, 0, NULL, NULL, R_Draw, &s_splash3};
const statetype s_splash3 = {DROPSPLASH3SPR, DROPSPLASH3SPR, step, false, false, 10, 0, 0, NULL, NULL, R_Draw, NULL};

const Uint16 bonusshape[] = {REDGEM1SPR, YELLOWGEM1SPR, BLUEGEM1SPR, GREENGEM1SPR, SUGAR1ASPR, SUGAR2ASPR, SUGAR3ASPR, SUGAR4ASPR, SUGAR5ASPR, SUGAR6ASPR, ONEUPASPR, STUNCLIP1SPR};
const CK_SpriteType ckbonussprites[] = {CKS_RKEY, CKS_YKEY, CKS_BKEY, CKS_GKEY, CKS_SODA, CKS_TTGUM, CKS_CBAR, CKS_JBREAKER, CKS_DOUGHNUT, CKS_ICECREAM, CKS_LWFLASK, CKS_NSTUNNER};
const CK_SpriteType CK_BonusShadows[] = {CKS_KEYSHADOW, CKS_KEYSHADOW, CKS_KEYSHADOW, CKS_KEYSHADOW, CKS_SHADOW100, CKS_SHADOW200, CKS_SHADOW500, CKS_SHADOW1K, CKS_SHADOW2K, CKS_SHADOW5K, CKS_SHADOW1UP, CKS_SHADOWSTUN};

/*
===========================
=
= SpawnBonus
=
===========================
*/

void SpawnBonus(Sint16 x, Sint16 y, Sint16 type)
{
	GetNewObj(false);
	ck_newobj->needtoclip = cl_noclip;
	ck_newobj->priority = 2;
	ck_newobj->obclass = bonusobj;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	ck_newobj->ydir = -1;
	ck_newobj->temp1 = type;
	ck_newobj->temp2 = ck_newobj->shapenum = bonusshape[type];
	ck_newobj->temp3 = ck_newobj->temp2 + 2;
	NewState(ck_newobj, &s_bonus1, ckbonussprites[type]);
}

/*
===========================
=
= SpawnSplash
=
===========================
*/

void SpawnSplash(Sint16 x, Sint16 y)
{	
	GetNewObj(true);
	ck_newobj->needtoclip = cl_noclip;
	ck_newobj->priority = 3;
	ck_newobj->obclass = inertobj;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y);
	NewState(ck_newobj, &s_splash1, CKS_DROPLETS);
}

/*
===========================
=
= T_Bonus
=
===========================
*/

void T_Bonus(objtype *ob)
{
	ob->shapenum++;
	if (ob->shapenum == ob->temp3)
		ob->shapenum = ob->temp2;
}

/*
=============================================================================

						  COUNCIL MEMBER

=============================================================================
*/

const statetype s_councilwalk1 = {COUNCILWALKL1SPR, COUNCILWALKR1SPR, step, false, true,   10,  64, 0, T_Council, NULL, R_Walk, &s_councilwalk2};
const statetype s_councilwalk2 = {COUNCILWALKL2SPR, COUNCILWALKR2SPR, step, false, true,   10,  64, 0, T_Council, NULL, R_Walk, &s_councilwalk1};
const statetype s_councilstand = {COUNCILTHINKLSPR, COUNCILTHINKRSPR, step, false, false, 120, 128, 0, NULL, NULL, R_Draw, &s_councilwalk1};

/*
===========================
=
= SpawnCouncil
=
===========================
*/

void SpawnCouncil(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = oracleobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) -0x171;	//TODO: wierd
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	NewState(ck_newobj, &s_councilwalk1, CKS_CMEMBER);
}

/*
===========================
=
= T_Council
=
===========================
*/

void T_Council(objtype *ob)
{
	Uint16 randnum;

	randnum = US_RndT();
	if (tics*8 > randnum)
	{
		// BUG: might be about to move off a ledge, causing it to get stuck
		// after standing (the stand state doesn't use R_Walk)!
		// To avoid that, set xtry to 0 here.
#ifdef FIX_BUGS
		xtry = 0; // TOOD: See if this works
#endif
		ob->state = &s_councilstand;
	}
}


/*
=============================================================================

						  POINSON SLUG

=============================================================================
*/

const statetype s_slugwalk1   = {SLUGWALKL1SPR,   SLUGWALKR1SPR,   step,  false, true,    8, 64, 0, NULL, C_Slug, R_WalkNormal, &s_slugwalk2};
const statetype s_slugwalk2   = {SLUGWALKL2SPR,   SLUGWALKR2SPR,   step,  false, true,    8, 64, 0, T_Slug, C_Slug, R_WalkNormal, &s_slugwalk1};
const statetype s_slugpiss1   = {SLUGPISSLSPR,    SLUGPISSRSPR,    step,  false, true,   60, 64, 0, T_SlugPiss, C_Slug, R_WalkNormal, &s_slugwalk1};
const statetype s_slugstun    = {SLUGSTUN1SPR,    SLUGSTUN1SPR,    think, false, false,   0,  0, 0, T_Projectile, NULL, R_Stunned, NULL};
const statetype s_slugstunalt = {SLUGSTUN2SPR,    SLUGSTUN2SPR,    think, false, false,   0,  0, 0, T_Projectile, NULL, R_Stunned, NULL};
const statetype s_slugslime   = {SLUGSLIME1SPR,   SLUGSLIME1SPR,   step,  false, false, 300,  0, 0, NULL, C_Lethal, R_Draw, &s_slugslime2};
const statetype s_slugslime2  = {SLUGSLIME2SPR,   SLUGSLIME2SPR,   step,  false, false,  60,  0, 0, NULL, NULL, R_Draw, NULL};

/*
===========================
=
= SpawnSlug
=
===========================
*/

void SpawnSlug(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = slugobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) - 0x71;
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	NewState(ck_newobj, &s_slugwalk1, CKS_POISONSLUG);
}

/*
===========================
=
= T_Slug
=
===========================
*/

void T_Slug(objtype *ob)
{
	if (US_RndT() < 16 || LastScan == GBA_BUTTON_A)
	{
		if (ob->x < player->x)
		{
			ob->xdir = 1;
		}
		else
		{
			ob->xdir = -1;
		}
		ob->state = &s_slugpiss1;
		SD_PlaySound(SND_SLUGPOO);
		// TODO:  Push the slig into the ground so it doesn't get stuck on slopes??
		
		// Note: might be a good idea to set xtry to 0 here
#ifdef FIX_BUGS
		xtry = 0; // TOOD: Might be intended to not have this???
#endif
	}
}

/*
===========================
=
= T_SlugPiss
=
===========================
*/

void T_SlugPiss(objtype *ob)
{
	GetNewObj(true);
	ck_newobj->obclass = inertobj;
	ck_newobj->active = ac_removable;
	ck_newobj->priority = 0;
	ck_newobj->x = ob->x;
	ck_newobj->y = ob->bottom - 8*PIXGLOBAL;
	NewState(ck_newobj, &s_slugslime, CKS_POISONPOO);
}

/*
===========================
=
= C_Slug
=
===========================
*/

void C_Slug(objtype *ob, objtype *hit)
{
	if (hit->obclass == keenobj)
	{
		KillKeen();
	}
	else if (hit->obclass == stunshotobj)
	{
		if (US_RndT() < 0x80)
		{
			StunObj(ob, hit, &s_slugstun);
		}
		else
		{
			StunObj(ob, hit, &s_slugstunalt);
		}
		ob->yspeed = -24;
		ob->xspeed = ob->xdir*8;
	}
}


/*
=============================================================================

						  MAD MUSHROOM

temp1 = jump counter

=============================================================================
*/

const statetype s_mushroom1 = {MADMUSHROOML1SPR, MADMUSHROOMR1SPR, stepthink, false, false, 8, 0, 0, T_Mushroom, C_Mushroom, R_Mushroom, &s_mushroom2};
const statetype s_mushroom2 = {MADMUSHROOML2SPR, MADMUSHROOMR2SPR, stepthink, false, false, 8, 0, 0, T_Mushroom, C_Mushroom, R_Mushroom, &s_mushroom1};

/*
===========================
=
= SpawnMadMushroom
=
===========================
*/

void SpawnMadMushroom(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = madmushroomobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) - 0xF1;
	ck_newobj->xdir = 1;
	ck_newobj->ydir = 1;
	NewState(ck_newobj, &s_mushroom1, CKS_MADMUSHROOM);
}

/*
===========================
=
= T_Mushroom
=
===========================
*/

void T_Mushroom(objtype *ob)
{
	if (player->x < ob->x)
	{
		ob->xdir = -1;
	}
	else
	{
		ob->xdir = 1;
	}

	// BUG: this might be executed twice during the same frame if the
	// object's animation/state changed during that frame, causing the
	// object to move at twice the speed during that frame!
	// To avoid that, return if ytry is not 0.
	DoWeakGravity(ob);
}

/*
===========================
=
= C_Mushroom
=
===========================
*/

void C_Mushroom(objtype *ob, objtype *hit)
{
	//ob++;			// shut up compiler
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
= R_Mushroom
=
===========================
*/

void R_Mushroom(objtype *ob)
{
	if (ob->hitsouth)
		ob->yspeed = 0;

	if (ob->hitnorth)
	{
		ob->yspeed = 0;
		if (++ob->temp1 == 3)
		{
			ob->temp1 = 0;
			ob->yspeed = -68;
			SD_PlaySound(SND_BOUNCE2);
		}
		else
		{
			SD_PlaySound(SND_BOUNCE1);
			ob->yspeed = -40;
		}
	}
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}


/*
=============================================================================

						  EGGBIRD

temp1 = blocked flag for flying eggbird (cannot change xdir to chase Keen
        while this is non-zero)

=============================================================================
*/

const statetype s_egg      = {EGGSPR,      EGGSPR,      think, false, true,      8, 0, 0, NULL, C_Egg, R_Draw, NULL};
const statetype s_eggbroke = {EGGBROKESPR, EGGBROKESPR, step,  false, false, 30000, 0, 0, NULL, NULL, R_Draw, NULL};
const statetype s_eggchip1 = {EGGCHIP1SPR, EGGCHIP1SPR, think, false, false,     0, 0, 0, T_Projectile, NULL, R_Chip, NULL};
const statetype s_eggchip2 = {EGGCHIP2SPR, EGGCHIP2SPR, think, false, false,     0, 0, 0, T_Projectile, NULL, R_Chip, NULL};
const statetype s_eggchip3 = {EGGCHIP3SPR, EGGCHIP3SPR, think, false, false,     0, 0, 0, T_Projectile, NULL, R_Chip, NULL};

const statetype s_eggbirdpause = {BIRDWALKL1SPR, BIRDWALKR1SPR, step,       false, true,  120, 128, 0, NULL, C_EggbirdStun, R_Eggbird, &s_eggbirdwalk2};
const statetype s_eggbirdwalk1 = {BIRDWALKL1SPR, BIRDWALKR1SPR, step,       false, true,    7, 128, 0, NULL, C_Eggbird, R_Eggbird, &s_eggbirdwalk2};
const statetype s_eggbirdwalk2 = {BIRDWALKL2SPR, BIRDWALKR2SPR, step,       false, true,    7, 128, 0, NULL, C_Eggbird, R_Eggbird, &s_eggbirdwalk3};
const statetype s_eggbirdwalk3 = {BIRDWALKL3SPR, BIRDWALKR3SPR, step,       false, true,    7, 128, 0, NULL, C_Eggbird, R_Eggbird, &s_eggbirdwalk4};
const statetype s_eggbirdwalk4 = {BIRDWALKL4SPR, BIRDWALKR4SPR, step,       false, true,    7, 128, 0, T_Eggbird, C_Eggbird, R_Eggbird, &s_eggbirdwalk1};
const statetype s_eggbirdfly1  = {BIRDFLY1SPR,   BIRDFLY1SPR,   slidethink, false, false,   8,   0, 0, T_EggbirdFly, C_Eggbird, R_Eggbirdfly, &s_eggbirdfly2};
const statetype s_eggbirdfly2  = {BIRDFLY2SPR,   BIRDFLY2SPR,   slidethink, false, false,   8,   0, 0, T_EggbirdFly, C_Eggbird, R_Eggbirdfly, &s_eggbirdfly3};
const statetype s_eggbirdfly3  = {BIRDFLY3SPR,   BIRDFLY3SPR,   slidethink, false, false,   8,   0, 0, T_EggbirdFly, C_Eggbird, R_Eggbirdfly, &s_eggbirdfly4};
const statetype s_eggbirdfly4  = {BIRDFLY4SPR,   BIRDFLY4SPR,   slidethink, false, false,   8,   0, 0, T_EggbirdFly, C_Eggbird, R_Eggbirdfly, &s_eggbirdfly1};
const statetype s_eggbirddrop  = {BIRDFLY4SPR,   BIRDFLY4SPR,   think,      false, false,   8, 128, 0, T_WeakProjectile, C_Eggbird, R_EggbirdDrop, NULL};
const statetype s_eggbirdstun  = {BIRDSTUNSPR,   BIRDSTUNSPR,   stepthink,  false, false, 240,   0, 0, T_Projectile, C_EggbirdStun, R_Draw, &s_eggbirdstun2};
const statetype s_eggbirdstun2 = {BIRDWALKL1SPR, BIRDWALKR1SPR, step,       false, true,   20,   0, 0, NULL, C_EggbirdStun, R_Draw, &s_eggbirdstun3};
const statetype s_eggbirdstun3 = {BIRDSTUNSPR,   BIRDSTUNSPR,   step,       false, true,   20,   0, 0, NULL, C_EggbirdStun, R_Draw, &s_eggbirdstun4};
const statetype s_eggbirdstun4 = {BIRDWALKL1SPR, BIRDWALKR1SPR, step,       false, true,   20,   0, 0, NULL, C_EggbirdStun, R_Draw, &s_eggbirdstun5};
const statetype s_eggbirdstun5 = {BIRDSTUNSPR,   BIRDSTUNSPR,   step,       false, true,   20,   0, 0, T_EggUnstun, C_EggbirdStun, R_Draw, &s_eggbirdwalk1};

/*
===========================
=
= SpawnEggbird
=
===========================
*/

void SpawnEggbird(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = eggobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) - 0x71;
	ck_newobj->xdir = 1;
	ck_newobj->ydir = 1;
	NewState(ck_newobj, &s_egg, CKS_EGG);
}

/*
===========================
=
= T_EggUnstun
=
===========================
*/

void T_EggUnstun(objtype *ob)
{
	ob->obclass = eggbirdobj;
}

/*
===========================
=
= SpawnEggbirdOut
=
===========================
*/

void SpawnEggbirdOut(Sint16 x, Sint16 y)
{
	GetNewObj(true);
	ck_newobj->obclass = eggbirdobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 2;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) - 0xF1;
	if (ck_newobj->x < player->x)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	NewState(ck_newobj, &s_eggbirdpause, CKS_BBIRD);

}

/*
===========================
=
= C_Egg
=
===========================
*/

void C_Egg(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj || hit->obclass == keenobj)
	{
		if (hit->obclass == stunshotobj)
			ExplodeShot(hit);

		ob->obclass = inertobj;
		ob->active = ac_removable;
		ChangeState(ob, &s_eggbroke);

		GetNewObj(true);
		ck_newobj->obclass = eggbirdobj;
		ck_newobj->active = ac_yes;
		ck_newobj->x = ob->x;
		ck_newobj->y = ob->y - 8*PIXGLOBAL;
		if (ob->x < player->x)
		{
			ck_newobj->xdir = 1;
		}
		else
		{
			ck_newobj->xdir = -1;
		}
		ck_newobj->ydir = 1;
		NewState(ck_newobj, &s_eggbirdpause, CKS_BBIRD);

		GetNewObj(true);
		ck_newobj->obclass = inertobj;
		ck_newobj->active = ac_removable;
		ck_newobj->x = ob->x;
		ck_newobj->y = ob->y;
		ck_newobj->xspeed = -28;
		ck_newobj->yspeed = -40;
		NewState(ck_newobj, &s_eggchip1, CKS_EGGPARTICLES);

		GetNewObj(true);
		ck_newobj->obclass = inertobj;
		ck_newobj->active = ac_removable;
		ck_newobj->x = ob->x;
		ck_newobj->y = ob->y;
		ck_newobj->xspeed = 28;
		ck_newobj->yspeed = -40;
		NewState(ck_newobj, &s_eggchip2, CKS_EGGPARTICLES);

		GetNewObj(true);
		ck_newobj->obclass = inertobj;
		ck_newobj->active = ac_removable;
		ck_newobj->x = ob->x;
		ck_newobj->y = ob->y;
		ck_newobj->xspeed = 0;
		ck_newobj->yspeed = -56;
		NewState(ck_newobj, &s_eggchip3, CKS_EGGPARTICLES);
	}
}



/*
===========================
=
= T_Eggbird
=
===========================
*/

void T_Eggbird(objtype *ob)
{
	if (ob->x < player->x)
	{
		ob->xdir = 1;
	}
	else
	{
		ob->xdir = -1;
	}
	if (ob->bottom >= player->bottom + 3*TILEGLOBAL && player->hitnorth
		&& StatePositionOk(ob, &s_eggbirdfly1))	// BUG: StatePositionOk() only works for normal clipping, not for full clipping
	{
		// Note: might be a good idea to set xtry to 0 here
		ob->state = &s_eggbirdfly1;
		ob->needtoclip = cl_fullclip;
		ob->yspeed = -8;
		ob->temp1 = 0;
	}
}

/*
===========================
=
= T_EggbirdFly
=
===========================
*/

void T_EggbirdFly(objtype *ob)
{
	if (ob->temp1 == 0)
	{
		if (ob->x < player->x)
		{
			ob->xdir = 1;
		}
		else
		{
			ob->xdir = -1;
		}
	}
	AccelerateXv(ob, ob->xdir, 16);
	if (ob->y < player->y)
	{
		AccelerateY(ob, 1, 16);
	}
	else
	{
		AccelerateY(ob, -1, 16);
	}
}

/*
===========================
=
= C_Eggbird
=
===========================
*/

void C_Eggbird(objtype *ob, objtype *hit)
{
	if (hit->obclass == keenobj)
	{
		KillKeen();
	}
	else if (hit->obclass == stunshotobj)
	{
		ob->xspeed = 0;
		ob->needtoclip = cl_midclip;
		StunObj(ob, hit, &s_eggbirdstun);
	}
}

/*
===========================
=
= C_EggbirdStun
=
===========================
*/

void C_EggbirdStun(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj)
	{
		ob->xspeed = 0;
		StunObj(ob, hit, &s_eggbirdstun);
	}
}

/*
===========================
=
= R_Eggbird
=
===========================
*/

void R_Eggbird(objtype *ob)
{
	if (ob->xdir == 1 && ob->hitwest)
	{
		ob->x -= ob->xmove;
		ob->xdir = -1;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state); // ???
	}
	else if (ob->xdir == -1 && ob->hiteast)
	{
		ob->x -= ob->xmove;
		ob->xdir = 1;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state); // ???
	}
	if (!ob->hitnorth)
	{
		ob->yspeed = -16;
		ob->needtoclip = cl_fullclip;
		ChangeState(ob, &s_eggbirdfly1);
	}
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}

/*
===========================
=
= R_EggbirdDrop
=
===========================
*/

void R_EggbirdDrop(objtype *ob)	//never actually used
{
	if (ob->hitnorth)
	{
		ChangeState(ob, &s_eggbirdwalk1);
	}
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}

/*
===========================
=
= R_Chip
=
===========================
*/

void R_Chip(objtype *ob)
{
	if (ob->hitnorth)
		ob->xspeed = ob->yspeed = 0;

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}

/*
===========================
=
= R_Eggbirdfly
=
===========================
*/

void R_Eggbirdfly(objtype *ob)
{
	statetype *oldstate;

	if ((ob->hitsouth || ob->hitnorth) && !ob->temp1)
		ob->temp1++;

	if (ob->hiteast && ob->xspeed < 0 || ob->hitwest && ob->xspeed > 0)
	{
		ob->xspeed = 0;
		ob->xdir = -ob->xdir;
	}
	if (ob->hitnorth == 1 && player->bottom - ob->bottom < 8*PIXGLOBAL)	// BUG? unsigned comparison!
	{
		oldstate = ob->state;
		ob->needtoclip = cl_midclip;
		ChangeState(ob, &s_eggbirdwalk1);
		xtry = 0;
		ytry = 8*PIXGLOBAL;
		PushObj(ob);
		if (!ob->hitnorth)
		{
			ob->needtoclip = cl_fullclip;
			ChangeState(ob, oldstate);
		}
		return;	// BUG: sprite isn't updated
	}

	if (!ob->hitsouth && !ob->hitnorth)
		ob->temp1 = 0;

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}

/*
=============================================================================

						  ARACHNUT

=============================================================================
*/

const statetype s_arach1     = {ARACHNUTWALK1SPR, ARACHNUTWALK4SPR, step, false, true,   6, 128, 0, NULL, C_Arach, R_Walk, &s_arach2};
const statetype s_arach2     = {ARACHNUTWALK2SPR, ARACHNUTWALK3SPR, step, false, true,   6, 128, 0, NULL, C_Arach, R_Walk, &s_arach3};
const statetype s_arach3     = {ARACHNUTWALK3SPR, ARACHNUTWALK2SPR, step, false, true,   6, 128, 0, NULL, C_Arach, R_Walk, &s_arach4};
const statetype s_arach4     = {ARACHNUTWALK4SPR, ARACHNUTWALK1SPR, step, false, true,   6, 128, 0, T_Arach, C_Arach, R_Walk, &s_arach1};
const statetype s_arachstun  = {ARACHNUTSTUNSPR,  ARACHNUTSTUNSPR,  step, false, true, 240,   0, 0, NULL, C_ArachStun, R_Draw, &s_arachstun2};
const statetype s_arachstun2 = {ARACHNUTWALK1SPR, ARACHNUTWALK1SPR, step, false, true,  20,   0, 0, NULL, C_ArachStun, R_Draw, &s_arachstun3};
const statetype s_arachstun3 = {ARACHNUTSTUNSPR,  ARACHNUTSTUNSPR,  step, false, true,  20,   0, 0, NULL, C_ArachStun, R_Draw, &s_arachstun4};
const statetype s_arachstun4 = {ARACHNUTWALK1SPR, ARACHNUTWALK1SPR, step, false, true,  20,   0, 0, NULL, C_ArachStun, R_Draw, &s_arachstun5};
const statetype s_arachstun5 = {ARACHNUTSTUNSPR,  ARACHNUTSTUNSPR,  step, false, true,  20,   0, 0, NULL, C_ArachStun, R_Draw, &s_arach1};

/*
===========================
=
= SpawnArachnut
=
===========================
*/

void SpawnArachnut(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = arachnutobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
	ck_newobj->x = CONVERT_TILE_TO_GLOBAL(x);
	ck_newobj->y = CONVERT_TILE_TO_GLOBAL(y) - 0x171;
	if (US_RndT() < 0x80)
	{
		ck_newobj->xdir = 1;
	}
	else
	{
		ck_newobj->xdir = -1;
	}
	ck_newobj->ydir = 1;
	NewState(ck_newobj, &s_arach1, CKS_ARACHNUT);
}

/*
===========================
=
= T_Arach
=
===========================
*/

void T_Arach(objtype *ob)
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
= C_Arach
=
===========================
*/

void C_Arach(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj)
	{
		StunObj(ob, hit, &s_arachstun);
	}
	else if (hit->obclass == keenobj)
	{
		KillKeen();
	}
}

/*
===========================
=
= C_ArachStun
=
===========================
*/

void C_ArachStun(objtype *ob, objtype *hit)
{
	if (hit->obclass == stunshotobj)
	{
		StunObj(ob, hit, &s_arachstun);
	}
}

/*
=============================================================================

						  SKYPEST

=============================================================================
*/

const statetype s_pestfly1 = {SKYPESTFLYL1SPR, SKYPESTFLYR1SPR, stepthink, true, false, 5, 0, 0, T_PestFly, C_PestFly, R_Pest, &s_pestfly2};
const statetype s_pestfly2 = {SKYPESTFLYL2SPR, SKYPESTFLYR2SPR, stepthink, true, false, 5, 0, 0, T_PestFly, C_PestFly, R_Pest, &s_pestfly1};
const statetype s_squashedpest = {SKYPESTSQUASHEDSPR, SKYPESTSQUASHEDSPR, think, false, false, 0, 0, 0, NULL, NULL, R_Draw, &s_squashedpest};
const statetype s_pestrest1  = {SKYPESTSIT9SPR, SKYPESTSIT9SPR, step, false, false, 100, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest2};
const statetype s_pestrest2  = {SKYPESTSIT1SPR, SKYPESTSIT1SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest3};
const statetype s_pestrest3  = {SKYPESTSIT2SPR, SKYPESTSIT2SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest4};
const statetype s_pestrest4  = {SKYPESTSIT3SPR, SKYPESTSIT3SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest5};
const statetype s_pestrest5  = {SKYPESTSIT4SPR, SKYPESTSIT4SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest6};
const statetype s_pestrest6  = {SKYPESTSIT3SPR, SKYPESTSIT3SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest7};
const statetype s_pestrest7  = {SKYPESTSIT2SPR, SKYPESTSIT2SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest8};
const statetype s_pestrest8  = {SKYPESTSIT1SPR, SKYPESTSIT1SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest9};
const statetype s_pestrest9  = {SKYPESTSIT9SPR, SKYPESTSIT9SPR, step, false, false, 60, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest10};
const statetype s_pestrest10 = {SKYPESTSIT5SPR, SKYPESTSIT5SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest11};
const statetype s_pestrest11 = {SKYPESTSIT6SPR, SKYPESTSIT6SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest12};
const statetype s_pestrest12 = {SKYPESTSIT7SPR, SKYPESTSIT7SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest13};
const statetype s_pestrest13 = {SKYPESTSIT8SPR, SKYPESTSIT8SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest14};
const statetype s_pestrest14 = {SKYPESTSIT7SPR, SKYPESTSIT7SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest15};
const statetype s_pestrest15 = {SKYPESTSIT6SPR, SKYPESTSIT6SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest16};
const statetype s_pestrest16 = {SKYPESTSIT5SPR, SKYPESTSIT5SPR, step, false, false, 10, 0, 0, NULL, C_Squashable, R_Draw, &s_pestrest17};
const statetype s_pestrest17 = {SKYPESTSIT9SPR, SKYPESTSIT9SPR, step, false, false, 100, 0, 0, T_PestRest, C_Squashable, R_Draw, &s_pestfly1};

/*
===========================
=
= SpawnSkypest
=
===========================
*/

void SpawnSkypest(Sint16 x, Sint16 y)
{
	GetNewObj(false);
	ck_newobj->obclass = skypestobj;
	ck_newobj->active = ac_yes;
	ck_newobj->priority = 0;
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
	if (US_RndT() < 0x80)
	{
		ck_newobj->ydir = 1;
	}
	else
	{
		ck_newobj->ydir = -1;
	}
	NewState(ck_newobj, &s_pestfly1, CKS_SKYPEST);
}

/*
===========================
=
= T_PestFly
=
===========================
*/

void T_PestFly(objtype *ob)
{
	// BUG: this might be executed twice during the same frame if the
	// object's animation/state changed during that frame, causing the
	// object to move at twice the speed during that frame!
	// To avoid that, return if xtry is not 0 or ytry is not 0.

	if (US_RndT() < tics)
		ob->xdir = -ob->xdir;

	if (ob->ydir == -1 && US_RndT() < tics)
		ob->ydir = 1;

	if (ob->ydir == 1 && US_RndT() < tics*2)
		ob->ydir = -ob->ydir;

	AccelerateX(ob, ob->xdir, 20);
	AccelerateY(ob, ob->ydir, 20);
}

/*
===========================
=
= C_PestFly
=
===========================
*/

void C_PestFly(objtype *ob, objtype *hit)
{
	if (hit->obclass == keenobj)
		KillKeen();

	if (hit->obclass == stunshotobj)
	{
		if (hit->xdir == 1)
		{
			ob->xspeed = 20;
		}
		else if (hit->xdir == -1)
		{
			ob->xspeed = -20;
		}
		else if (hit->ydir == 1)
		{
			ob->yspeed = 20;
		}
		else if (hit->ydir == -1)
		{
			ob->yspeed = -20;
		}
		ExplodeShot(hit);
	}
}

/*
===========================
=
= C_Squashable
=
===========================
*/

void C_Squashable(objtype *ob, objtype *hit)
{
	if (hit->state == &s_keenpogodown || hit->state == &s_keenpogo || hit->state == &s_keenpogo2)
	{
		ChangeState(ob, &s_squashedpest);
		SD_PlaySound(SND_SQUISH);
		ob->obclass = inertobj;
	}
}

/*
===========================
=
= T_PestRest
=
===========================
*/

void T_PestRest(objtype *ob)
{
	ob->ydir = -1;
	ob->yspeed = -16;
	ytry = -144;
}

/*
===========================
=
= R_Pest
=
===========================
*/

void R_Pest(objtype *ob)
{
	if (ob->hitsouth)
	{
		ob->yspeed = 8;
		ob->ydir = 1;
	}
	if (ob->hitnorth && !ob->hiteast && !ob->hitwest)
	{
		ob->y += 8*PIXGLOBAL;
		ChangeState(ob, &s_pestrest1);
	}
	if (ob->hitwest)
	{
		ob->xspeed = 0;
		ob->xdir = -1;
	}
	if (ob->hiteast)
	{
		ob->xspeed = 0;
		ob->xdir = 1;
	}
	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}

