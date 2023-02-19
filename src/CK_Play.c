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
ControlInfo c;

// ck_newobj is known as new in OG code
objtype *ck_newobj, *check, *player, *scoreobj;

objtype dummyobj;

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

    if(ob->x < CK_GlobalCameraX + 7*TILEGLOBAL){
        xscroll = ob->x - (CK_GlobalCameraX + 7*TILEGLOBAL);
    }else if(ob->x+ 16 > CK_GlobalCameraX + 9*TILEGLOBAL){
        xscroll = ob->x + 16 - (CK_GlobalCameraX + 9*TILEGLOBAL);
    }else{
        xscroll = 0;
    }

    if(ob->y < CK_GlobalCameraY + 4*TILEGLOBAL){
        yscroll = ob->y - (CK_GlobalCameraY + 4*TILEGLOBAL);
    }else if(ob->y > CK_GlobalCameraY + 5*TILEGLOBAL){
        yscroll = ob->y - (CK_GlobalCameraY + 5*TILEGLOBAL);
    }else{
        yscroll = 0;
    }
    /*
	if (ob->left < CK_GlobalCameraX + 9*TILEGLOBAL)
	{
		xscroll = ob->left - (CK_GlobalCameraX + 9*TILEGLOBAL);
	}
	else if (ob->right > CK_GlobalCameraX + 12*TILEGLOBAL)
	{
		xscroll = ob->right + 16 - (CK_GlobalCameraX + 12*TILEGLOBAL);
	}
	else
	{
		xscroll = 0;
	}

	if (ob->top < CK_GlobalCameraY + 5*TILEGLOBAL)
	{
		yscroll = ob->top - (CK_GlobalCameraY + 5*TILEGLOBAL);
	}
	else if (ob->bottom > CK_GlobalCameraY + 7*TILEGLOBAL)
	{
		yscroll = ob->bottom - (CK_GlobalCameraY + 7*TILEGLOBAL);
	}
	else
	{
		yscroll = 0;
	}*/

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
};

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
}


//===========================================================================

/*
=====================
=
= CheckKeys
=
=====================
*/

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
	objtype *check;

	StartMusic(gamestate.mapon);
	fireheld = pogoheld = upheld = jumpheld = false;
	ingame = true;
	playstate = ex_stillplaying;
	invincible = keenkilled = oldfirecount = 0;

//TODO: Find out what this does
//	CenterActor(player);

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
		for (int i = 0; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];

			if (!obj->active )/*&& obj->tileright >= originxtile-1
				&& obj->tileleft <= originxtilemax+1 && obj->tiletop <= originytilemax+1
				&& obj->tilebottom >= originytile-1)*/
			{
				obj->needtoreact = true;
				obj->active = ac_yes;
			}
			if (obj->active)
			{
				/*if (obj->tileright < inactivateleft
					|| obj->tileleft > inactivateright
					|| obj->tiletop > inactivatebottom
					|| obj->tilebottom < inactivatetop)*/
				{
					if (obj->active == ac_removable)
					{
						RemoveObj(obj);
						continue;
					}
					else if (obj->active != ac_allways)
					{
						if (US_RndT() < tics*2 )//|| screenfaded || loadedgame)
						{
                            // TODO: Make this remove the sprites???
							//RF_RemoveSprite(&obj->sprite);
							//if (obj->obclass == stunnedobj)
							//	RF_RemoveSprite((void **)&obj->temp3);
							obj->active = ac_no;
							continue;
						}
					}
				}
				StateMachine(obj);
			}
		}

/*
		if (gamestate.riding)
		{
			HandleRiding(player);
		}

//
// check for and handle collisions between objects
//
		for (obj=player; obj; obj=obj->next)
		{
			if (obj->active)
			{
				for (check=obj->next; check; check=check->next)
				{
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
*/

//
// react to whatever happened, and post sprites to the refresh manager
//
		for (int i = 0; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
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
//			ScrollScreen(player);
		}
		else
		{
			WorldScrollScreen(player);
		}
        
        CK_UpdateLevel();

		UpdateScore(scoreobj);

/*		if (loadedgame)
		{
			loadedgame = false;
		}
*/
//
// update the screen and calculate the number of tics it took to execute
// this cycle of events (for adaptive timing of next cycle)
//
        CK_FixCamera();
        CK_RenderLevel();

        RF_CalcTics();

/*
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
// single step debug mode
//
		if (singlestep)
		{
			VW_WaitVBL(14);	//reduces framerate to 5 fps on VGA or 4.3 fps on EGA cards
			lasttimecount = TimeCount;
		}
//
// extra VBLs debug mode
//
		if (extravbls)
		{
			VW_WaitVBL(extravbls);
		}
*/
//
// handle user inputs
//
		if (DemoMode == demo_Playback)
		{/*
			if (!screenfaded && IN_IsUserInput())
			{*/
				playstate = ex_completed;
				/*if (LastScan != sc_F1)
				{
					LastScan = sc_Space;
				}
			}*/
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