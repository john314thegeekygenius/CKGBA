/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
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
