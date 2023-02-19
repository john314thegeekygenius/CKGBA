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

const Sint16 wallclip[8][16] = {			// the height of a given point in a tile
{ 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256, 256},
{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
{   0,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78},
{0x80,0x88,0x90,0x98,0xa0,0xa8,0xb0,0xb8,0xc0,0xc8,0xd0,0xd8,0xe0,0xe8,0xf0,0xf8},
{   0,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0xa0,0xb0,0xc0,0xd0,0xe0,0xf0},
{0x78,0x70,0x68,0x60,0x58,0x50,0x48,0x40,0x38,0x30,0x28,0x20,0x18,0x10,0x08,   0},
{0xf8,0xf0,0xe8,0xe0,0xd8,0xd0,0xc8,0xc0,0xb8,0xb0,0xa8,0xa0,0x98,0x90,0x88,0x80},
{0xf0,0xe0,0xd0,0xc0,0xb0,0xa0,0x90,0x80,0x70,0x60,0x50,0x40,0x30,0x20,0x10,   0}
};

Sint16 xtry, ytry;
boolean playerkludgeclipcancel;




/*
==================
=
= DoActor
=
= Moves an actor in its current state by a given number of tics.
= If that time takes it into the next state, it changes the state
= and returns the number of excess tics after the state change
=
==================
*/

Sint16 DoActor(objtype *ob, Sint16 numtics)
{
	Sint16 ticcount, usedtics, excesstics;
	statetype *state;
	
	state = ob->state;

	if (state->progress == think)
	{
		if (state->think)
		{
			if (ob->nothink)
			{
				ob->nothink--;
			}
			else
			{
				state->think(ob);
			}
		}
		return 0;
	}

	ticcount = ob->ticcount + numtics;

	if (state->tictime > ticcount || state->tictime == 0)
	{
		ob->ticcount = ticcount;
		if (state->progress == slide || state->progress == slidethink)
		{
			if (ob->xdir)
			{
				xtry += ob->xdir == 1? numtics*state->xmove : -numtics*state->xmove;
			}
			if (ob->ydir)
			{
				ytry += ob->ydir == 1? numtics*state->ymove : -numtics*state->ymove;
			}
		}
		if ((state->progress == slidethink || state->progress == stepthink) && state->think)
		{
			if (ob->nothink)
			{
				ob->nothink--;
			}
			else
			{
				state->think(ob);
			}
		}
		return 0;
	}
	else
	{
		usedtics = state->tictime - ob->ticcount;
		excesstics = ticcount - state->tictime;
		ob->ticcount = 0;
		if (state->progress == slide || state->progress == slidethink)
		{
			if (ob->xdir)
			{
				xtry += ob->xdir == 1? usedtics*state->xmove : -usedtics*state->xmove;
			}
			if (ob->ydir)
			{
				ytry += ob->ydir == 1? usedtics*state->ymove : -usedtics*state->ymove;
			}
		}
		else
		{
			if (ob->xdir)
			{
				xtry += ob->xdir == 1? state->xmove : -state->xmove;
			}
			if (ob->ydir)
			{
				ytry += ob->ydir == 1? state->ymove : -state->ymove;
			}
		}

		if (state->think)
		{
			if (ob->nothink)
			{
				ob->nothink--;
			}
			else
			{
				state->think(ob);
			}
		}

		if (state == ob->state)
		{
			ob->state = state->nextstate;	// go to next state
		}
		else if (!ob->state)
		{
			return 0;			// object removed itself
		}
		return excesstics;
	}
}

//==========================================================================


/*
====================
=
= StateMachine
=
= Change state and give directions
=
====================
*/

void StateMachine(objtype *ob)
{
	Sint16 excesstics, oldshapenum;
	statetype *state;
	
	ob->xmove=ob->ymove=xtry=ytry=0;
	oldshapenum = ob->shapenum;

	state = ob->state;

	excesstics = DoActor(ob, tics);
	if (ob->state != state)
	{
		ob->ticcount = 0;		// start the new state at 0, then use excess
		state = ob->state;
	}

	while (excesstics)
	{
	//
	// passed through to next state
	//
		if (!state->skippable && state->tictime <= excesstics)
		{
			excesstics = DoActor(ob, state->tictime-1);
		}
		else
		{
			excesstics = DoActor(ob, excesstics);
		}
		if (ob->state != state)
		{
			ob->ticcount = 0;		// start the new state at 0, then use excess
			state = ob->state;
		}
	}

	if (!state)			// object removed itself
	{
		RemoveObj(ob);
		return;
	}

	//
	// if state->rightshapenum == NULL, the state does not have a standard
	// shape (the think routine should have set it)
	//
	if (state->rightshapenum)
	{
		if (ob->xdir > 0)
		{
			ob->shapenum = state->rightshapenum;
		}
		else
		{
			ob->shapenum = state->leftshapenum;
		}
	}
	if ((Sint16)ob->shapenum == -1)
	{
		ob->shapenum = 0;		// make it invisable this time
	}

	if (xtry || ytry || ob->shapenum != oldshapenum || ob->hitnorth == 25)
	{
	//
	// actor moved or changed shape
	// make sure the movement is within limits (one tile)
	//
    
		if (ob->needtoclip == cl_fullclip)
		{
	//		FullClipToWalls(ob);
		}
		else
		{
		//	ClipToWalls(ob);
		}
	}
}

//==========================================================================


/*
====================
=
= NewState
=
====================
*/

void NewState(objtype *ob, statetype *state)
{
	Sint16 temp;
	
	ob->state = state;

	if (state->rightshapenum)
	{
		if (ob->xdir > 0)
		{
			ob->shapenum = state->rightshapenum;
		}
		else
		{
			ob->shapenum = state->leftshapenum;
		}
	}

	if ((Sint16)ob->shapenum == -1)
	{
		ob->shapenum = 0;
	}

	temp = ob->needtoclip;
	ob->needtoclip = cl_noclip;

	xtry = ytry = 0;					// no movement
    ob->needtoreact = true;
/*	ClipToWalls(ob);					// just calculate values

	ob->needtoclip = temp;

	if (ob->needtoclip == cl_fullclip)
	{
		FullClipToWalls(ob);
	}
	else if (ob->needtoclip == cl_midclip)
	{
		ClipToWalls(ob);
	}
*/}

//==========================================================================


/*
====================
=
= ChangeState
=
====================
*/

void ChangeState(objtype *ob, statetype *state)
{
	ob->state = state;
	ob->ticcount = 0;
	if (state->rightshapenum)
	{
		if (ob->xdir > 0)
		{
			ob->shapenum = state->rightshapenum;
		}
		else
		{
			ob->shapenum = state->leftshapenum;
		}
	}

	if ((Sint16)ob->shapenum == -1)
	{
		ob->shapenum = 0;
	}

	ob->needtoreact = true;			// it will need to be redrawn this frame

	xtry = ytry = 0;					// no movement

	if (ob->hitnorth != 25)
	{
        ob->needtoreact = true;
//		ClipToWalls(ob);
	}
}

//==========================================================================




/*
===============
=
= R_Draw
=
===============
*/

void R_Draw(objtype *ob) {
    CK_UpdateObjGraphics(ob);
    CK_DrawObject(ob, ob->x, ob->y);
//	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}


/*
===============
=
= R_Walk
=
===============
*/

void R_Walk(objtype *ob)
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
		ob->x -= ob->xmove;
		ob->xdir = -ob->xdir;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state);
	}
    CK_UpdateObjGraphics(ob);
    CK_DrawObject(ob, ob->x, ob->y);

//	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}


/*
===============
=
= R_WalkNormal
=
= Actor will not walk onto tiles with special (e.g. deadly) north walls
=
===============
*/

void R_WalkNormal(objtype *ob)
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
	else if (!ob->hitnorth || (ob->hitnorth & ~7))
	{
		ob->x -= ob->xmove*2;
		ob->xdir = -ob->xdir;
		ob->nothink = US_RndT() >> 5;
		ChangeState(ob, ob->state);
	}
    CK_UpdateObjGraphics(ob);
    CK_DrawObject(ob, ob->x, ob->y);

//	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
}


/*
===============
=
= BadState
=
===============
*/

void BadState(void){
    // TODO:
    // Make this work
    VW_ClearVideo(0xF1F1F1F1);
    while(1); // Just hang???
	//Quit("Object with bad state!");
};


//==========================================================================

statetype sc_deadstate = {0, 0, think, false, false, 0, 0, 0, NULL, NULL, NULL, NULL};
#pragma warn -sus	//BadState is not a valid think/contact/react function. Nobody cares.
statetype sc_badstate  = {0, 0, think, false, false, 0, 0, 0, &BadState, &BadState, &BadState, NULL};
#pragma warn +sus

