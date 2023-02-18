/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


/*
CK_KEEN2.C
==========

Contains the following actor types (in this order):

- Score Box & Demo sprites
- Keen (world map)
- Flags (world map)
- Neural Stunner Shots
- Gem Door Opener
- Card Door Opener (Keen 5 only)

*/


const Direction opposite[8] = {dir_South, dir_SouthWest, dir_West, dir_NorthWest, dir_North, dir_NorthEast, dir_East, dir_SouthEast};

/*
=============================================================================

						 SCORE BOX ROUTINES

=============================================================================
*/

const statetype s_score = {  0,   0, think, false, false, 0, 0, 0, NULL, NULL, NULL, NULL};
const statetype s_demo  = {DEMOPLAQUESPR, DEMOPLAQUESPR, think, false, false, 0, 0, 0, NULL, NULL, NULL, NULL};

/*
======================
=
= SpawnScore
=
======================
*/

void SpawnScore(void)
{
	scoreobj->obclass = inertobj;
	scoreobj->priority = 3;
	scoreobj->active = ac_allways;
	scoreobj->needtoclip = cl_noclip;
	scoreobj->temp2 = -1;
	scoreobj->temp1 = -1;
	scoreobj->temp3 = -1;
	scoreobj->temp4 = -1;
	if (scorescreenkludge)
	{
		scoreobj->state = &sc_deadstate;
	}
	else if (!DemoMode)
	{
		NewState(scoreobj, &s_score);
	    CK_SetSprite(scoreobj, CKS_SCOREBOX);
	}
	else
	{
		NewState(scoreobj, &s_demo);
	    CK_SetSprite(scoreobj, CKS_DEMO);
		//CA_MarkGrChunk(DEMOPLAQUESPR); // Unused
	}
}




/*
===============
=
= UpdateScore
=
===============
*/

void UpdateScore(objtype *ob)
{
	if (scorescreenkludge)
		return;

	if(ob->state != &sc_deadstate){
	    CK_UpdateObjGraphics(ob);
	}

	if (DemoMode)
	{
		DrawDemoPlaque(ob);
		return;
	}

	CK_DrawObject(ob, ob->x + 4, ob->y + 4);

	/*
	char		str[10],*ch;
	spritetype	_seg	*block;
	Uint8		far *dest;
	Uint16	i, length, width, planesize, number;
	boolean changed;

	if (scorescreenkludge)
		return;

	if (DemoMode)
	{
		DrawDemoPlaque(ob);
		return;
	}

	if (!showscorebox)
		return;

	changed = false;

//code below is a combination of ScoreThink and ScoreReact from Keen Dreams with minor changes

//
// score changed
//
	if ((gamestate.score>>16) != ob->temp1
		|| (Uint16)gamestate.score != ob->temp2 )
	{
		block = (spritetype _seg *)grsegs[SCOREBOXSPR];
		width = block->width[0];
		planesize = block->planesize[0];
		dest = (Uint8 far *)grsegs[SCOREBOXSPR]+block->sourceoffset[0]
			+ planesize + width*4;

		ltoa (gamestate.score,str,10);

		// erase leading spaces
		length = strlen(str);
		for (i=9;i>length;i--)
			MemDrawChar (41,dest+=CHARWIDTH,width,planesize);

		// draw digits
		ch = str;
		while (*ch)
			MemDrawChar (*ch++ - '0'+42,dest+=CHARWIDTH,width,planesize);

#if GRMODE == EGAGR
		ShiftScore ();
#endif
		ob->needtoreact = true;
		ob->temp1 = gamestate.score>>16;
		ob->temp2 = gamestate.score;

		changed = true;
	}

//
// ammo changed
//
	number = gamestate.ammo;
	if (number != ob->temp3)
	{
		block = (spritetype _seg *)grsegs[SCOREBOXSPR];
		width = block->width[0];
		planesize = block->planesize[0];
		dest = (byte far *)grsegs[SCOREBOXSPR]+block->sourceoffset[0]
			+ planesize + width*20 + 7*CHARWIDTH;

		if (number > 99)
			strcpy (str,"99");
		else
			ltoa (number,str,10);

		// erase leading spaces
		length = strlen(str);
		for (i=2;i>length;i--)
			MemDrawChar (41,dest+=CHARWIDTH,width,planesize);

		// draw digits
		ch = str;
		while (*ch)
			MemDrawChar (*ch++ - '0'+42,dest+=CHARWIDTH,width,planesize);

#if GRMODE == EGAGR
		ShiftScore ();
#endif
		ob->needtoreact = true;
		ob->temp3 = number;

		changed = true;
	}

//
// lives changed
//
	if (gamestate.lives != ob->temp4)
	{
		block = (spritetype _seg *)grsegs[SCOREBOXSPR];
		width = block->width[0];
		planesize = block->planesize[0];
		dest = (byte far *)grsegs[SCOREBOXSPR]+block->sourceoffset[0]
			+ planesize + width*20 + 2*CHARWIDTH;

		if (gamestate.lives > 99)
			strcpy (str,"99");
		else
			ltoa (gamestate.lives,str,10);

		// erase leading spaces
		length = strlen(str);
		for (i=2;i>length;i--)
			MemDrawChar (41,dest+=CHARWIDTH,width,planesize);

		// draw digits
		ch = str;
		while (*ch)
			MemDrawChar (*ch++ - '0'+42,dest+=CHARWIDTH,width,planesize);

#if GRMODE == EGAGR
		ShiftScore ();
#endif
		ob->needtoreact = true;
		ob->temp4 = gamestate.lives;

		changed = true;
	}
*/
/*
Note:
-----

It would be more efficient to use

	if (changed)
		ShiftScore();

here instead of the individual ShiftScore() calls above. Because if the player
gains a life by collecting points items, both the score and lives numbers need
to be updated, which means the sprite would be shifted twice. And if the player
fires a shot during the same frame, the ammo number also needs to be updated,
leading to up to three shifts in one frame.
*/
/*
	if (ob->x != originxglobal || ob->y != originyglobal)
	{
		ob->x = originxglobal;
		ob->y = originyglobal;
		changed = true;
	}

	if (changed)
#if GRMODE == EGAGR
		RF_PlaceSprite(&ob->sprite, ob->x+4*PIXGLOBAL, ob->y+4*PIXGLOBAL, SCOREBOXSPR, spritedraw, 3);
#elif GRMODE == CGAGR
		RF_PlaceSprite(&ob->sprite, ob->x+8*PIXGLOBAL, ob->y+8*PIXGLOBAL, SCOREBOXSPR, spritedraw, 3);
#endif*/
}

/*
===============
=
= DrawDemoPlaque
=
===============
*/

void DrawDemoPlaque(objtype *ob){
	CK_DrawObject(ob, ob->x + 120 - 32, ob->y + 4);
	/*
	if (ob->x != originxglobal || ob->y != originyglobal)
	{
		ob->x = originxglobal;
		ob->y = originyglobal;
		RF_PlaceSprite(&ob->sprite, ob->x + 160*PIXGLOBAL - 32*PIXGLOBAL, ob->y + 8*PIXGLOBAL, DEMOPLAQUESPR, spritedraw, 3);
	}*/
}



/*
=============================================================================

							   MINI KEEN

player->temp1 = dir
player->temp2 = animation stage

=============================================================================
*/

#ifdef KEEN4
const statetype s_keenonfoot1 = {WOLRDKEENRIDE1SPR, WOLRDKEENRIDE1SPR, stepthink, false, false, 30, 0, 0, T_FootFly, NULL, R_Draw, &s_keenonfoot2};
const statetype s_keenonfoot2 = {WOLRDKEENRIDE2SPR, WOLRDKEENRIDE2SPR, stepthink, false, false, 30, 0, 0, T_FootFly, NULL, R_Draw, &s_keenonfoot1};
const statetype s_worldswim = {0, 0, slide, true, false, 6, 16, 16, T_KeenWorldSwim, NULL, R_Draw, &s_worldswim};
#endif

#ifdef KEEN5
const statetype s_worldelevate = {-1, -1, think, true, false, 6, 16, 16, T_Elevate, NULL, R_Draw, NULL};
#endif

const statetype s_worldkeen     = {0, 0, stepthink, false, false, 360, 0, 0, T_KeenWorld, NULL, R_Draw, &s_worldkeenwave1};

const statetype s_worldkeenwave1 = {WORLDKEENWAVE1SPR, WORLDKEENWAVE1SPR, stepthink, false, false, 20, 0, 0, T_KeenWorld, NULL, R_Draw, &s_worldkeenwave2};
const statetype s_worldkeenwave2 = {WORLDKEENWAVE2SPR, WORLDKEENWAVE2SPR, stepthink, false, false, 20, 0, 0, T_KeenWorld, NULL, R_Draw, &s_worldkeenwave3};
const statetype s_worldkeenwave3 = {WORLDKEENWAVE1SPR, WORLDKEENWAVE1SPR, stepthink, false, false, 20, 0, 0, T_KeenWorld, NULL, R_Draw, &s_worldkeenwave4};
const statetype s_worldkeenwave4 = {WORLDKEENWAVE2SPR, WORLDKEENWAVE2SPR, stepthink, false, false, 20, 0, 0, T_KeenWorld, NULL, R_Draw, &s_worldkeenwave5};
const statetype s_worldkeenwave5 = {WORLDKEENWAVE1SPR, WORLDKEENWAVE1SPR, stepthink, false, false, 20, 0, 0, T_KeenWorldWalk, NULL, R_Draw, &s_worldkeen};

const statetype s_worldkeenwalk = {0, 0, slide, true, false, 4, 24, 24, T_KeenWorldWalk, NULL, R_Draw, &s_worldkeenwalk};

const Sint16 worldshapes[8] = {WORLDKEENU1SPR-1, WORLDKEENUR1SPR-1, WORLDKEENR1SPR-1, WORLDKEENDR1SPR-1, WORLDKEEND1SPR-1, WORLDKEENDL1SPR-1, WORLDKEENL1SPR-1, WORLDKEENUL1SPR-1};	//-1 to everything because worldanims values are 1-based
const Sint16 worldanims[4] = {2, 3, 1, 3};
#ifdef KEEN4
const Sint16 swimshapes[8] = {WORLDKEENSWIMU1SPR, WORLDKEENSWIMUR1SPR, WORLDKEENSWIMR1SPR, WORLDKEENSWIMDR1SPR, WORLDKEENSWIMD1SPR, WORLDKEENSWIMDL1SPR, WORLDKEENSWIML1SPR, WORLDKEENSWIMUL1SPR};
#endif
#ifndef KEEN6
const Sint16 tiledir[4] = {dir_South, dir_West, dir_North, dir_East};
#endif

/*
======================
=
= SpawnWorldKeen
=
======================
*/

void SpawnWorldKeen(Sint16 x, Sint16 y)
{
#ifdef KEEN4
	if (playstate == ex_foot)
	{
		player->needtoclip = cl_noclip;
		player->obclass = keenobj;
		player->x = gamestate.worldx;
		player->y = gamestate.worldy;
		player->active = ac_allways;
		player->priority = 3;
		player->xdir = 0;
		player->ydir = 0;
		if (gamestate.worldx < 20*TILEGLOBAL)
		{
			player->temp1 = 280;
			player->xspeed = (30*TILEGLOBAL - player->x)/280 + 1;
			player->yspeed = (55*TILEGLOBAL - player->y)/280 + 1;
		}
		else
		{
			player->temp1 = 140;
			player->xspeed = (Sint16)(16*TILEGLOBAL - player->x)/140 + 1;
			player->yspeed = (Sint16)(47*TILEGLOBAL - player->y)/140 + 1;
		}
		NewState(player, &s_keenonfoot1);
		return;
	}
#endif

	player->obclass = keenobj;
	if (gamestate.worldx == 0)
	{
		player->x = CONVERT_TILE_TO_GLOBAL(x);
		player->y = CONVERT_TILE_TO_GLOBAL(y);
	}
	else
	{
		player->x = gamestate.worldx;
		player->y = gamestate.worldy;
	}
	player->active = ac_allways;
	player->priority = 1;
	player->xdir = 0;
	player->ydir = 0;
	player->temp1 = dir_West;
	player->temp2 = 3;
	player->temp3 = 0;
	player->shapenum = WORLDKEENL3SPR;
	NewState(player, &s_worldkeen);
}


/*
======================
=
= T_KeenWorld
=
======================
*/

void T_KeenWorld(objtype *ob)
{
/*	if (c.dir != dir_None)
	{
		ob->state = &s_worldkeenwalk;
		ob->temp2 = 0;
		T_KeenWorldWalk(ob);
	}
	if (jumpbutton || pogobutton || firebutton)
	{
		CheckEnterLevel(ob);
	}*/
}

/*
======================
=
= T_KeenWorldWalk
=
======================
*/

void T_KeenWorldWalk(objtype *ob)
{
	if (ob->temp3)
	{
		ob->temp3 -= 4;
		if (ob->temp3 < 0)
			ob->temp3 = 0;
	}
	else
	{/*
		ob->xdir = c.xaxis;
		ob->ydir = c.yaxis;
		if (pogobutton || firebutton || jumpbutton)
		{
			CheckEnterLevel(ob);
		}
		if (c.dir == dir_None)
		{
			ob->state = &s_worldkeen;
			ob->shapenum = worldshapes[ob->temp1] + 3;
			return;
		}
		ob->temp1 = c.dir;*/
	}
	if (++ob->temp2 == 4)
		ob->temp2 = 0;
	ob->shapenum = worldshapes[ob->temp1] + worldanims[ob->temp2];

	if (ob->temp2 == 1)
	{
		//SD_PlaySound(SND_WORLDWALK1);
	}
	else if (ob->temp2 == 3)
	{
		//SD_PlaySound(SND_WORLDWALK2);
	}
}

#ifdef KEEN4
/*
======================
=
= T_FootFly
=
======================
*/

void T_FootFly(objtype *ob)
{
	ob->temp1 = ob->temp1 - tics;
	xtry = ob->xspeed * tics;
	ytry = ob->yspeed * tics;
	if (ob->temp1 <= 0)
	{
		xtry -= ob->xspeed * -ob->temp1;
		ytry -= ob->yspeed * -ob->temp1;
		ob->priority = 1;
		ob->temp1 = dir_West;
		ob->temp2 = 3;
		ob->temp3 = 0;
		player->xdir = 0;
		player->ydir = 0;
		ob->state = &s_worldkeen;
		ob->shapenum = WORLDKEENL3SPR;
		ob->needtoclip = cl_midclip;
	}
}

/*
======================
=
= T_KeenWorldSwim
=
======================
*/

void T_KeenWorldSwim(objtype *ob)
{
	if (ob->temp3)
	{
		ob->temp3 -= 6;
		if (ob->temp3 < 0)
			ob->temp3 = 0;
	}
	else
	{/*
		ob->xdir = c.xaxis;
		ob->ydir = c.yaxis;
		if (c.xaxis || c.yaxis)
			ob->temp1 = c.dir;*/
	}
	ob->shapenum = swimshapes[ob->temp1] + ob->temp2;
	if (++ob->temp2 == 2)
		ob->temp2 = 0;

	if (ob->temp2 == 0)
	{
		//SD_PlaySound(SND_SWIM1);
	}
	else
	{
		//SD_PlaySound(SND_SWIM2);
	}
}

#endif	// NOT Keen 4 (i.e. Keen 5 & 6):
