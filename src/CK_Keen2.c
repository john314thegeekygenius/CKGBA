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
	}
	else
	{
		NewState(scoreobj, &s_demo);
		//CA_MarkGrChunk(DEMOPLAQUESPR); // Unused
	}
}


/*

    // Create a new sprite
    CK_GetNewObj(false);
    ck_newobj->x = 88;
    ck_newobj->y = 8;
    ck_newobj->state = &ck_demosignstate;
    CK_SetNewSprite(CKS_DEMO);
    CK_UpdateObjGraphics(ck_newobj);

*/