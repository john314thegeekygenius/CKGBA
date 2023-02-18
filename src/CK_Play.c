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


// ck_newobj is known as new in OG code
objtype *ck_newobj, *check, *player, *scoreobj;

objtype dummyobj;

Sint16 invincible;

boolean oldshooting, showscorebox, joypad;

Sint16 groundslam;

boolean debugok;


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

void StartMusic(Uint16 num)
{
	Sint16 song;
	boolean wasfaded;


	SD_MusicOff();

#ifdef KEEN4
	if (num == 0xFFFF) {
		song = WONDER_MUS;
	} else {
		song = num;
	}
#else
	song = num;
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

	StartMusic(gamestate.mapon);/*
	fireheld = pogoheld = upheld = jumpheld = false;
	ingame = true;
	playstate = ex_stillplaying;
	invincible = keenkilled = oldfirecount = 0;

	CenterActor(player);
*/
	if (DemoMode)
	{
		US_InitRndT(false);
	}
	else
	{
		US_InitRndT(true);
	}
	TimeCount = lasttimecount = tics = 3;
    int fps_limiter = 0;
	do
	{
		++fps_limiter;
		if(fps_limiter == 3){
			fps_limiter = 0;
			if(GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT)){
				CK_GlobalCameraX += 1;
			}
			if(GBA_TEST_BUTTONS(GBA_BUTTON_LEFT)){
				CK_GlobalCameraX -= 1;
			}
			if(GBA_TEST_BUTTONS(GBA_BUTTON_UP)){
				CK_GlobalCameraY -= 1;
			}
			if(GBA_TEST_BUTTONS(GBA_BUTTON_DOWN)){
				CK_GlobalCameraY += 1;
			}
		}

        if(GBA_TEST_BUTTONS(GBA_BUTTON_LSHOLDER)){
            gamestate.mapon -= 1;
            SetupGameLevel(true);            
        }
        if(GBA_TEST_BUTTONS(GBA_BUTTON_RSHOLDER)){
            gamestate.mapon += 1;
            SetupGameLevel(true);            
        }

		/*PollControls();

//
// go through state changes and propose movements
//
		for (obj=player; obj; obj=obj->next)
		{
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
							RF_RemoveSprite(&obj->sprite);
							if (obj->obclass == stunnedobj)
								RF_RemoveSprite((void **)&obj->temp3);
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

//
// react to whatever happened, and post sprites to the refresh manager
//
		for (obj=player; obj; obj=obj->next)
		{
			if (!obj->active)
			{
				continue;
			}
			if (obj->tilebottom >= mapheight-1)
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
*/
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
//			WorldScrollScreen(player);
		}
        CK_UpdateLevel();
        CK_FixCamera();
        CK_RenderLevel();
		UpdateScore(scoreobj);
//        GBA_Delay(50);
/*		if (loadedgame)
		{
			loadedgame = false;
		}
*/
//
// update the screen and calculate the number of tics it took to execute
// this cycle of events (for adaptive timing of next cycle)
//
		//RF_Refresh();
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
			//CheckKeys();
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