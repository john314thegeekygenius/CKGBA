/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

// From CK_PLAY.C
/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

boolean singlestep, jumpcheat, godmode, keenkilled;

exittype playstate;
gametype gamestate;

Uint16 originxtilemax;
Uint16 originytilemax;

ControlInfo c;

// ck_newobj is known as 'new' in OG code
GBA_IN_IWRAM objtype *ck_newobj = NULL, *player = NULL, *scoreobj = NULL;

Sint16 invincible;

boolean oldshooting, showscorebox, joypad;

Sint16 groundslam;

boolean debugok;

boolean jumpbutton, jumpheld, pogobutton, pogoheld, firebutton, fireheld, upheld;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/
Sint16 oldfirecount;

Uint16 centerlevel;

Sint16 inactivateleft;
Sint16 inactivateright;
Sint16 inactivatetop;
Sint16 inactivatebottom;


//===========================================================================


/*
=====================
=
= CheckKeys
=
=====================
*/
bool HITBITTON = 0;
void CheckKeys(void)
{
/*	if (screenfaded)			// don't do anything with a faded screen
	{
		return;
	}
*/
//
// Enter for status screen
//
/*	if (Keyboard[sc_Enter] || (GravisGamepad && GravisAction[ga_Status]))
	{
		StatusWindow();
		IN_ClearKeysDown();
		RF_ForceRefresh();
		lasttimecount = TimeCount;	// BUG: should be the other way around
	}
*/
	if(GBA_TEST_BUTTONS(GBA_BUTTON_SELECT)){
		HITBITTON = 1;
	}else{
		if(HITBITTON){
			HITBITTON = 0;
			CK_PaletteSet += 1;
			if(CK_PaletteSet > 4) CK_PaletteSet = 0;
			CK_FixPalette();
		}
	}
}

//===========================================================================

		
/*
==================
=
= CenterActor
=
==================
*/

void CenterActor(objtype *ob)
{
	Uint16 orgx, orgy;

	centerlevel = CK_HALF_SCREENY;
	if (ob->x < 152*PIXGLOBAL)
	{
		orgx = 0;
	}
	else
	{
		orgx = ob->x - 152*PIXGLOBAL;
	}
	if (mapon == 0)
	{
		if (ob->y < 80*PIXGLOBAL)
		{
			orgy = 0;
		}
		else
		{
			orgy = ob->y - 80*PIXGLOBAL;
		}
	}
	else
	{
		if (ob->bottom < CK_HALF_SCREENY*PIXGLOBAL)
		{
			orgy = 0;
		}
		else
		{
			orgy = ob->bottom - CK_HALF_SCREENY*PIXGLOBAL;
		}
	}
	if (!scorescreenkludge)
	{
		CK_MoveCamera(orgx, orgy);
		CK_ForceLevelRedraw();
	}

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}
}


//===========================================================================

/*
==================
=
= WorldScrollScreen
=
= Scroll if Keen is nearing an edge
=
==================
*/

void WorldScrollScreen(objtype *ob)
{
	Sint16 xscroll, yscroll;

	if (keenkilled)
		return;

	if (ob->left < originxglobal + 9*TILEGLOBAL)
	{
		xscroll = ob->left - (originxglobal + 9*TILEGLOBAL);
	}
	else if (ob->right > originxglobal + 12*TILEGLOBAL)
	{
		xscroll = ob->right + 16 - (originxglobal + 12*TILEGLOBAL);
	}
	else
	{
		xscroll = 0;
	}

	if (ob->top < originyglobal + 5*TILEGLOBAL)
	{
		yscroll = ob->top - (originyglobal + 5*TILEGLOBAL);
	}
	else if (ob->bottom > originyglobal + 7*TILEGLOBAL)
	{
		yscroll = ob->bottom - (originyglobal + 7*TILEGLOBAL);
	}
	else
	{
		yscroll = 0;
	}

	if (!xscroll && !yscroll)
		return;

//
// don't scroll more than one tile per frame
//
	if (xscroll >= 0x100)
	{
		xscroll = 0xFF;
	}
	else if (xscroll <= -0x100)
	{
		xscroll = -0xFF;
	}
	if (yscroll >= 0x100)
	{
		yscroll = 0xFF;
	}
	else if (yscroll <= -0x100)
	{
		yscroll = -0xFF;
	}

    CK_ScrollCamera(xscroll, yscroll);

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}

}

//===========================================================================

/*
==================
=
= ScrollScreen
=
= Scroll if Keen is nearing an edge
= Set playstate to ex_completes
=
==================
*/

void ScrollScreen(objtype *ob)
{
	Sint16 xscroll, yscroll, pix, speed;
	Uint16 bottom;

	if (keenkilled)
		return;

//
// walked off edge of map?
//
	if (ob->left < originxmin || ob->right > originxmax + GBA_SCREEN_WIDTH*PIXGLOBAL)
	{
		playstate = ex_completed;
		return;
	}

//
// fallen off bottom of world?
//
	if (ob->bottom > originymax + 13*TILEGLOBAL)
	{
		ob->y -= ob->bottom - (originymax + 13*TILEGLOBAL);
		SD_PlaySound(SND_PLUMMET);
		godmode = false;
		KillKeen();
		return;
	}

	xscroll=yscroll=0;

	if (ob->x < originxglobal + 7*TILEGLOBAL) // 9
	{
		xscroll = ob->x - (originxglobal + 7*TILEGLOBAL); // 9
	}
	else if (ob->x > originxglobal + 9*TILEGLOBAL) // 12
	{
		xscroll = ob->x - (originxglobal + 9*TILEGLOBAL); // 12
	}

	if (ob->state == &s_keenlookup2)
	{
		if (centerlevel+tics > 167)
		{
			pix = 167-centerlevel;
		}
		else
		{
			pix = tics;
		}
		centerlevel += pix;
		yscroll = CONVERT_PIXEL_TO_GLOBAL(-pix);
	}
	else if (ob->state == &s_keenlookdown3)
	{
		if (centerlevel-tics < 33)
		{
			pix = centerlevel + -33;
		}
		else
		{
			pix = tics;
		}
		centerlevel -= pix;
		yscroll = CONVERT_PIXEL_TO_GLOBAL(pix);
	}

#ifdef KEEN6
	if (groundslam)
	{
		const static Sint16 shaketable[] = {0,
			 -64,  -64,  -64,  64,  64,  64,
			-200, -200, -200, 200, 200, 200,
			-250, -250, -250, 250, 250, 250,
			-250, -250, -250, 250, 250, 250
		};
		yscroll = yscroll + (bottom - (ob->bottom + shaketable[groundslam]));	// BUG: 'bottom' has not been initialized yet!
	}
	else
#endif
	centerlevel = 0;
	if ( (ob->hitnorth || !ob->needtoclip || ob->state == &s_keenholdon))
	{
		if (  ob->state != &s_keenclimbup
			&& ob->state != &s_keenclimbup2
			&& ob->state != &s_keenclimbup3
			&& ob->state != &s_keenclimbup4)
		{
			yscroll += ob->ymove;
			bottom = originyglobal + yscroll + CONVERT_PIXEL_TO_GLOBAL(centerlevel);
			if (ob->bottom == bottom)
			{
				// player didn't move, no additional scrolling
			}
			else
			{
				if (ob->bottom < bottom)
				{
					pix = bottom - ob->bottom;
				}
				else
				{
					pix = ob->bottom - bottom;
				}
				speed = CONVERT_PIXEL_TO_GLOBAL(pix) >> 7;
				if (speed > 0x30)
				{
					speed = 0x30;
				}
				speed *= tics;
				if (speed < 0x10)
				{
					if (pix < 0x10)
					{
						speed = pix;
					}
					else
					{
						speed = 0x10;
					}
				}
				if (ob->bottom < bottom)
				{
					yscroll -= speed;
				}
				else
				{
					yscroll += speed;
				}
			}
		}
	}
	else
	{
		centerlevel = CK_HALF_SCREENY;
	}

	pix = (ob->bottom-32*PIXGLOBAL)-(originyglobal+yscroll+32*PIXGLOBAL);
	if (pix < 0)
	{
		yscroll += pix;
	}
	pix = (ob->bottom+32*PIXGLOBAL)-(originyglobal+yscroll+200*PIXGLOBAL);
	if (pix > 0)
	{
		yscroll += pix;
	}

	if (xscroll == 0 && yscroll == 0)
		return;

//
// don't scroll more than one tile per frame
//
	if (xscroll >= 0x100)
	{
		xscroll = 0xFF;
	}
	else if (xscroll <= -0x100)
	{
		xscroll = -0xFF;
	}
	if (yscroll >= 0x100)
	{
		yscroll = 0xFF;
	}
	else if (yscroll <= -0x100)
	{
		yscroll = -0xFF;
	}

	CK_ScrollCamera(xscroll, yscroll);

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}
}

/*
=========================
=
= InitObjArray
=
= Call to clear out the entire object list, returning them all to the free
= list.  Allocates a special spot for the player.
=
=========================
*/

void InitObjArray(void) {

    // Remove any old objects
    CK_RemoveSprites();

//
// give the player and score the first free spots
//
	GetNewObj(false);
	player = ck_newobj;
	GetNewObj(false);
	scoreobj = ck_newobj;

};

//===========================================================================

/*
====================
=
= GivePoints
=
= Grants extra men at 20k,40k,80k,160k,320k
=
====================
*/

void GivePoints(Uint16 points)
{
	gamestate.score += points;
	if (!DemoMode && gamestate.score >= gamestate.nextextra)
	{
		SD_PlaySound(SND_EXTRAKEEN);
		gamestate.lives++;
		gamestate.nextextra *= 2;
	}
}


/*
===================
=
= PollControls
=
===================
*/

void PollControls(void)
{
	IN_ReadControl(0, &c);
	if (c.yaxis != -1)
		upheld = false;

	if (oldshooting || DemoMode)
	{
		if (c.button0 && c.button1)
		{
			firebutton = true;
			jumpbutton = pogobutton = jumpheld = pogoheld = false;
		}
		else
		{
			firebutton = fireheld = false;
			if (c.button0)
			{
				jumpbutton = true;
			}
			else
			{
				jumpbutton = jumpheld = false;
			}
			if (c.button1)
			{
				if (oldfirecount <= 8)
				{
					oldfirecount = oldfirecount + tics;
				}
				else
				{
					pogobutton = true;
				}
			}
			else
			{
				if (oldfirecount != 0)
				{
					pogobutton = true;
				}
				else
				{
					pogobutton = pogoheld = false;
				}
				oldfirecount = 0;
			}
		}
	}
	else
	{
		jumpbutton = c.button0;
		pogobutton = c.button1;
		firebutton = c.button2;
		if (!jumpbutton)
			jumpheld = false;
		if (!pogobutton)
			pogoheld = false;
		if (!firebutton)
			fireheld = false;
	}
	if(GBA_TEST_BUTTONS(GBA_BUTTON_LSHOLDER))
		if (mapon != 0)
		{
			SD_PlaySound(SND_LEVELDONE);
			gamestate.mapon = 0;
			gamestate.leveldone[mapon] = true;
			playstate = ex_completed;
		}
}


//===========================================================================


/*
=================
=
= StopMusic
=
=================
*/

void StopMusic(void) {
	SD_MusicOff();
};

//==========================================================================

/*
=================
=
= StartMusic
=
=================
*/

extern const unsigned int CK_NumberOfSongs;
extern const unsigned int CK_LevelAudio[];

void StartMusic(Uint16 num)
{
	Sint16 song;
	boolean wasfaded;


	SD_MusicOff();

#ifdef KEEN4
	if (num == 0xFFFF || num > CK_NumberOfSongs) {
		song = WONDER_MUS;
	} else {
		song = CK_LevelAudio[num];
	}
#else
    song = CK_LevelAudio[num];
#endif

	if (song == -1 || MusicMode != smm_AdLib)
	{
		return;
	}
    SD_PlayMusic(song, 1);
}

//==========================================================================


/*
===================
=
= PlayLoop
=
===================
*/

void PlayLoop(void)
{
	objtype *check = NULL;

	StartMusic(gamestate.mapon);
	fireheld = pogoheld = upheld = jumpheld = false;
	ingame = true;
	playstate = ex_stillplaying;
	invincible = keenkilled = oldfirecount = 0;

	CenterActor(player);

	if (DemoMode)
	{
		US_InitRndT(false);
	}
	else
	{
		US_InitRndT(true);
	}
	TimeCount = lasttimecount = tics = 3;

	do
	{

		PollControls();

        //
        // go through state changes and propose movements
        //
		for (int i = player->uuid; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
			if(!obj) break;
			if(obj->removed) continue;

			if (!obj->active && obj->tileright >= originxtile-1
				&& obj->tileleft <= originxtilemax+1 && obj->tiletop <= originytilemax+1
				&& obj->tilebottom >= originytile-1)
			{
				obj->needtoreact = true;
				obj->active = ac_yes;
			}
			if (obj->active)
			{
				if (obj->tileright < inactivateleft
					|| obj->tileleft > inactivateright
					|| obj->tiletop > inactivatebottom
					|| obj->tilebottom < inactivatetop)
				{
					if (obj->active == ac_removable)
					{
						RemoveObj(obj);
						continue;
					}
					else if (obj->active != ac_allways)
					{
						if (US_RndT() < tics*2 || screenfaded || loadedgame)
						{
                            // TODO: Make this remove the sprites???
							RF_RemoveSprite(obj);
							if (obj->obclass == stunnedobj){
								RF_RemoveSprite(obj->temp3);
							}
							obj->active = ac_no;
							continue;
						}
					}
				}
				StateMachine(obj);
			}
		}


		if (gamestate.riding)
		{
			HandleRiding(player);
		}

//
// check for and handle collisions between objects
//
		for (int i = player->uuid; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
			if(!obj) break;
			if(obj->removed) continue;
			if (obj->active)
			{
				for(int oi = i+1; oi < CK_NumOfObjects; oi++){
        			check = &CK_ObjectList[oi];

					if(!check) break;
					if(check->removed) continue;

					if (!check->active)
					{
						continue;
					}
					if (obj->right > check->left && obj->left < check->right
						&& obj->top < check->bottom && obj->bottom > check->top)
					{
						if (obj->state->contact)
						{
							obj->state->contact(obj, check);
						}
						if (check->state->contact)
						{
							check->state->contact(check, obj);
						}
						if (obj->obclass == nothing)	//useless -- obclass is NOT set to nothing by RemoveObj
						{
							break;
						}
					}
				}
			}
		}

//
// check intiles
//
		if (mapon != 0)
		{
			CheckInTiles(player);
		}
		else
		{
			CheckWorldInTiles(player);
		}


//
// react to whatever happened, and post sprites to the refresh manager
//
		for (int i = player->uuid; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
			if(!obj) continue;
			if(obj->removed) continue;
			if (!obj->active)
			{
				continue;
			}
			if (obj->tilebottom >= CK_CurLevelHeight-1)
			{
				if (obj->obclass == keenobj)
				{
					playstate = ex_died;
				}
				else
				{
					RemoveObj(obj);
				}
				continue;
			}
			if (obj->needtoreact && obj->state->react)
			{
				obj->needtoreact = false;
				obj->state->react(obj);
			}
		}
//
// scroll the screen and update the score box
//
#ifdef KEEN4
		if (mapon != 0 && mapon != 17)
#else
		if (mapon != 0)
#endif
		{
			ScrollScreen(player);
		}
		else
		{
			WorldScrollScreen(player);
		}
        
        CK_UpdateLevel();

		UpdateScore(scoreobj);

		if (loadedgame)
		{
			loadedgame = false;
		}

//
// update the screen and calculate the number of tics it took to execute
// this cycle of events (for adaptive timing of next cycle)
//
        CK_FixCamera();
        CK_RenderLevel();
		//CK_PrintObjInfo();
		CK_UpdateObjects();

        RF_CalcTics();


		if (invincible)
		{
			if ((invincible = invincible - tics) < 0)
				invincible = 0;
		}

#ifdef KEEN6
		if (groundslam)
		{
			if ((groundslam = groundslam - tics) < 0)
				groundslam = 0;
		}
#endif

//
// handle user inputs
//
		if (DemoMode == demo_Playback)
		{
			if (!screenfaded && IN_IsUserInput())
			{
				playstate = ex_completed;
				/*if (LastScan != sc_F1)
				{
					LastScan = sc_Space;
				}*/
			}
		}
		else if (DemoMode == demo_PlayDone)
		{
			playstate = ex_completed;
		}
		else
		{
			CheckKeys();
		}
/*
//
// E-N-D cheat
//
		if (Keyboard[sc_E] && Keyboard[sc_N] && Keyboard[sc_D])
		{
#if defined KEEN4
			gamestate.rescued = 7;
			playstate = ex_rescued;
#elif defined KEEN5
			playstate = ex_qedbroke;
#elif defined KEEN6
			playstate = ex_molly;
#endif
		}
*/
    
	} while (playstate == ex_stillplaying);

	ingame = false;
	StopMusic();
}