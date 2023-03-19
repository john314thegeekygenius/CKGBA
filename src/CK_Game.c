/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


// Pulled from CK_GAME.C


/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

Uint16 fadecount;
bool fadeinhook;
Sint16 levelcompleted;
Sint32 chunkcount, chunkmax, handpic;

// From ID_CA.C
int	mapon;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

void FadeAndUnhook(void);

//===========================================================================

/*
============================
=
= FreeGraphics
=
============================
*/

// Should never be used
//void FreeGraphics(void){}

//===========================================================================

void ClearGameState(){
	gamestate.worldx = gamestate.worldy = 0;
	for(int i = 0; i < GAMELEVELS; i++)
		gamestate.leveldone[i] = false;
	gamestate.score = gamestate.nextextra = 0;
	gamestate.ammo = gamestate.drops = 0;
#if defined CK4
	gamestate.wetsuit = 0;
	gamestate.rescued = 0;
#elif defined CK5
	gamestate.keycard = false;
	gamestate.destroyed = 0;	// never used
	gamestate.numfuses = 0;
#elif defined CK6
	gamestate.sandwichstate = gamestate.hookstate = gamestate.passcardstate = gamestate.rocketstate;
#endif
	gamestate.keys[0] = gamestate.keys[1] = gamestate.keys[2] = gamestate.keys[3] = 0;
	gamestate.mapon = -1;
	gamestate.lives = 0;
	gamestate.difficulty = 0;
	gamestate.riding = NULL;
};

/*
=====================
=
= NewGame
=
= Set up new game to start from the beginning
=
=====================
*/

void NewGame(void)
{
	ClearGameState();
	gamestate.nextextra = 20000;
	gamestate.lives = 3;
	gamestate.ammo = 5;
}

//===========================================================================

#ifndef KEEN5
/*
============================
=
= GameOver
=
============================
*/

void GameOver(void)
{
//	VW_FixRefreshBuffer();
	US_CenterWindow(16, 3);
	US_PrintCentered("Game Over!");

	IN_ClearKeysDown();
	IN_UserInput(4*TickBase, false);
}
#endif

//===========================================================================

/*
============================
=
= SaveTheGame
=
============================
*/

boolean SaveTheGame(Sint16 handle){
    // TODO:
    // Make this compress & save all game data

	Uint16	i,compressed,expanded;
	objtype	*ob;

	gamestate.riding = NULL;

/*	if (!CA_FarWrite(handle, (byte far *)&gamestate, sizeof(gamestate)))
		return false;

	expanded = mapwidth * mapheight * 2;
	MM_GetPtr(&bigbuffer, expanded);

	for (i = 0; i < 3; i++)
	{
		compressed = CA_RLEWCompress(mapsegs[i], expanded, (Uint16 huge *)bigbuffer+1, RLETAG);
		*(Uint16 huge *)bigbuffer = compressed;
		if (!CA_FarWrite(handle, bigbuffer, compressed+2))
		{
			MM_FreePtr(&bigbuffer);
			return false;
		}
	}
	for (ob = player; ob; ob=ob->next)
	{
		if (!CA_FarWrite(handle, (byte far *)ob, sizeof(objtype)))
		{
			MM_FreePtr(&bigbuffer);
			return false;
		}
	}*/
	return true;
}


//===========================================================================

/*
============================
=
= LoadTheGame
=
============================
*/

boolean LoadTheGame(Sint16 handle)
{
    // TODO:
    // Make this compress & save all game data
	return false;
    /*
	Uint16	i;
	objtype	*prev,*next,*followed;
	Uint16	compressed,expanded;
	memptr	bigbuffer;
#ifdef KEEN5
	Sint16	numfuses;
#endif

	if (!CA_FarRead(handle, (byte far *)&gamestate, sizeof(gamestate)))
		return false;

#ifdef KEEN5
	//
	// remember the fuses value for later - SetupGameLevel calls
	// ScanInfoPlane, which resets this part of the gamestate
	//
	numfuses = gamestate.numfuses;
#endif

	ca_levelbit >>= 1;
	ca_levelnum--;
	SetupGameLevel(false);
	if (mmerror)
	{
		mmerror = false;
		US_CenterWindow(20, 8);
		PrintY += 20;
		US_CPrint("Not enough memory\nto load game!");

		IN_Ack();
		return false;
	}
	ca_levelbit <<= 1;
	ca_levelnum++;

	expanded = mapwidth * mapheight * 2;
	MM_BombOnError(true);	//BUG: this should use false to avoid an instant crash
	MM_GetPtr(&bigbuffer, expanded);
	MM_BombOnError(false);	//BUG: this should use true to force an instant crash
	if (mmerror)
	{
		mmerror = false;
		US_CenterWindow(20, 8);
		PrintY += 20;
		US_CPrint("Not enough memory\nto load game!");

		IN_Ack();
		return false;
	}
	for (i = 0; i < 3; i++)
	{
		if (!CA_FarRead(handle, (byte far *)&compressed, sizeof(compressed)))
		{
			MM_FreePtr(&bigbuffer);
			return false;
		}
		if (!CA_FarRead(handle, (byte far *)bigbuffer, compressed))
		{
			MM_FreePtr(&bigbuffer);
			return false;
		}
		CA_RLEWexpand(bigbuffer, mapsegs[i], expanded, RLETAG);
	}
	MM_FreePtr(&bigbuffer);

	InitObjArray();
	new = player;
	prev = new->prev;
	next = new->next;
	if (!CA_FarRead(handle, (byte far *)new, sizeof(objtype)))
	{
		return false;
	}
	new->prev = prev;
	new->next = next;
	new->needtoreact = true;
	new->sprite = NULL;
	new = scoreobj;
	while (true)
	{
		prev = new->prev;
		next = new->next;
		if (!CA_FarRead(handle, (byte far *)new, sizeof(objtype)))
		{
			return false;
		}
		followed = new->next;
		new->prev = prev;
		new->next = next;
		new->needtoreact = true;
		new->sprite = NULL;
		if (new->obclass == stunnedobj)
		{
			new->temp3 = 0;	//clear sprite ptr for the stars
		}
#if defined KEEN4
		else if (new->obclass == platformobj)
		{
			new->temp2 = new->temp3 = 0;	//clear sprite ptrs
		}
#elif defined KEEN5
		else if (new->obclass == mineobj)
		{
			new->temp4 = 0;	//clear sprite ptr
		}
		else if (new->obclass == spherefulobj)
		{
			new->temp1 = new->temp2 = new->temp3 = new->temp4 = 0;	//clear sprite ptrs
		}
#elif defined KEEN6
		else if (new->obclass == platformobj)
		{
			new->temp3 = 0;	//clear sprite ptr
		}
#endif
		if (followed)
		{
			GetNewObj(false);
		}
		else
		{
			break;
		}
	}
	scoreobj->temp2 = -1;
	scoreobj->temp1 = -1;
	scoreobj->temp3 = -1;
	scoreobj->temp4 = -1;
#ifdef KEEN5
	gamestate.numfuses = numfuses;	// put value from saved game back in place 
#endif*/
	return true;
}

//===========================================================================

/*
============================
=
= ResetGame
=
============================
*/

void ResetGame(void)
{
	NewGame();
    /*
	ca_levelnum--;
	ca_levelbit >>= 1;
	CA_ClearMarks();
	ca_levelbit <<= 1;
	ca_levelnum++;*/
}

//===========================================================================



/*
==========================
=
= PatchWorldMap
=
= Takes out blocking squares and spawns flags
=
==========================
*/

void PatchWorldMap(void)
{
	Uint16 x, y, planeoff, info, level, tag;

	planeoff = 0;

	for (y = 0; y < CK_CurLevelHeight; y++)
	{
		for (x = 0; x < CK_CurLevelWidth; x++, planeoff++)
		{
			info = CK_GetInfo((y*CK_CurLevelWidth)+x);
			level = info & 0xFF;
			if (level >= MINDONELEVEL && level <= MAXDONELEVEL && gamestate.leveldone[level])
			{
				tag = info >> 8;
				CK_SetInfo((y*CK_CurLevelWidth)+x,0);	// BUG: infoplane value should only be set to 0 if tag == 0xC0
				if (tag == 0xD0)
				{
					CK_CurLevelData[CK_CurLevelSize + planeoff] = 0;
				}
				else if (tag == 0xF0)
				{
#ifdef KEEN5
					SpawnFlag(x, y);
#else
					if (levelcompleted == level)
					{
						SpawnThrowFlag(x, y);
					}
					else
					{
						SpawnFlag(x, y);
					}
#endif
				}
			}
		}
	}
}

//===========================================================================

/*
==========================
=
= DelayedFade
=
= Fades out and latches FadeAndUnhook onto the refresh
=
==========================
*/

void DelayedFade(void)
{
	VW_FadeOut();
	fadecount = 0;
	FadeAndUnhook();
//	RF_SetRefreshHook(&FadeAndUnhook);
}

/*
==========================
=
= FadeAndUnhook
=
= Latch this onto the refresh so the screen only gets faded in after two
= refreshes.  This lets all actors draw themselves to both pages before
= fading the screen in.
=
==========================
*/

void FadeAndUnhook(void)
{
	fadeinhook = true;
	TimeCount = lasttimecount;	// don't adaptively time the fade
}

//===========================================================================


/*
==========================
=
= SetupGameLevel
=
= Load in map mapon and cache everything needed for it
=
==========================
*/

extern const char *levelenter[GAMELEVELS];

void SetupGameLevel(boolean loadnow)
{
	CK_SetupLevelGBAMaps();


//
// randomize if not a demo
//
	if (DemoMode)
	{
		US_InitRndT(false);
		gamestate.difficulty = gd_Normal;
	}
	else
	{
		US_InitRndT(true);
	}

	// Handle invalid map id's here:
	if(gamestate.mapon < 0) gamestate.mapon = 0;
	if(gamestate.mapon > GAMELEVELS) gamestate.mapon = 0;
	mapon = gamestate.mapon;

//
// load the level header and three map planes
//
	CK_LoadLevel(mapon);

	ScanInfoPlane();
	if (mapon == 0)
	{
		PatchWorldMap();
	}

	if (loadnow)
	{
		if (scorescreenkludge)
		{
			CA_CacheMarks(NULL);
		}
		else if (DemoMode)
		{
			CA_CacheMarks("DEMO");
		}
#ifdef KEEN5
		else if (mapon == 0 && player->tiletop > 100)
		{
			CA_CacheMarks("Keen steps out\nonto Korath III");
		}
#endif
		else
		{
			if(mapon < GAMELEVELS)
				CA_CacheMarks(levelenter[mapon]);
		}
	}
	if (loadnow)
	{
		DelayedFade();
	}
};

//===========================================================================


/*
==========================
=
= DialogDraw
=
==========================
*/

void DialogDraw(char *title)
{

	Sint16 i;
	Uint16 width, height;

	handpic = 0;

	VW_FixGraphics();

	US_CenterWindow(26, 8);

	VWB_DrawPic(WindowX, WindowY, KEENCOUNT1PIC);

	WindowW -= 48;
	WindowX += 48;
	SizeText(title, &width, &height);
	PrintY += (WindowH-height)/2 - 4;
	US_CPrint(title);
}

/*
==========================
=
= DialogUpdate
=
==========================
*/

void DialogUpdate(void)
{
	if (handpic > 4)
		return;
	VWB_DrawPic(WindowX-24, WindowY+40, handpic+KEENCOUNT2PIC);
	handpic++;
}

/*
==========================
=
= DialogFinish
=
==========================
*/

void DialogFinish(void)
{
	//this is empty
}




/*
======================
=
= CA_CacheMarks
=
======================
*/
#define MAXEMPTYREAD	1024

void CA_CacheMarks (char *title)
{
	if(title == NULL) return;

	DialogDraw(title);

//
// go through and load in anything still needed
//

	for (int i=0;i<6*3;i++){
//
// update thermometer
//
		DialogUpdate();
		// Add some simulated delay
		GBA_Delay(250);
	}

//
// finish up any thermometer remnants
//
	DialogFinish();
};


//==========================================================================

/*
==========================
=
= HandleDeath
=
==========================
*/

void HandleDeath(void)
{

	Uint16 y, color, top, bottom, selection, selectionl, w, h;

	SizeText(levelnames[mapon], &w, &h);

	memset(gamestate.keys, 0, sizeof(gamestate.keys));
	gamestate.lives--;

	if (gamestate.lives >= 0)
	{
//		VW_FixRefreshBuffer();
		US_CenterWindow(24, 8);
		PrintY += 3;
		US_CPrint("You didn't make it past");
		top = PrintY+19;
		if (h < 15)
			PrintY += 4;
		US_CPrint(levelnames[mapon]);
		PrintY = top+2;
		US_CPrint("Try Again");
		PrintY += 8;
		bottom = PrintY-3;
		US_CPrint("Exit to "WORLDMAPNAME);

		IN_ClearKeysDown();
		selectionl = 0;
		selection = 0;
		while (true)
		{
			if (selection)
			{
				y = bottom;
			}
			else
			{
				y = top;
			}

// draw select bar
			if ((TimeCount / 16) & 1)
			{
				color = SECONDCOLOR;
			}
			else
			{
				color = FIRSTCOLOR;
			}

			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y, color);
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+1, color);
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+12, color);
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+13, color);
			VWB_Vlin(y+1, y+11, WindowX+4, color);
			VWB_Vlin(y+1, y+11, WindowX+5, color);
			VWB_Vlin(y+1, y+11, WindowX+WindowW-4, color);
			VWB_Vlin(y+1, y+11, WindowX+WindowW-5, color);


			if (c.button1)
			{
				gamestate.mapon = 0;		// exit to world map
				IN_ClearKeysDown();
				return;
			}

			IN_ReadControl(0, &c);
			if (c.button0 )
			{
				if (selection)
					gamestate.mapon = 0;		// exit to world map
				return;
			}
			if (c.yaxis == -1)
			{
				selection = 0;
			}
			else if (c.yaxis == 1)
			{
				selection = 1;
			}
			// erase select bar
			if(selectionl != selection){
				selectionl = selection;
				VWB_Hlin(WindowX+4, WindowX+WindowW-4, y, WHITE);
				VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+1, WHITE);
				VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+12, WHITE);
				VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+13, WHITE);
				VWB_Vlin(y+1, y+11, WindowX+4, WHITE);
				VWB_Vlin(y+1, y+11, WindowX+5, WHITE);
				VWB_Vlin(y+1, y+11, WindowX+WindowW-4, WHITE);
				VWB_Vlin(y+1, y+11, WindowX+WindowW-5, WHITE);
			}

		}
	}
	gamestate.mapon = 0;		// exit to world map
	IN_ClearKeysDown();
}

//==========================================================================

/*
============================
=
= GameLoop
=
= A game has just started (after the cinematic or load game)
=
============================
*/

void GameLoop(void)
{

	Uint16 temp;
#ifdef KEEN6
	Uint16 i;
#endif

#ifdef KEEN6
	if (!storedemo)
	{
		if (!US_ManualCheck())
		{
			loadedgame = false;
			restartgame = gd_Continue;
			return;
		}
	}
#endif

	// Uh
	scorescreenkludge = false;

	if (playstate == ex_loadedgame)
	{
		goto loaded;
	}
reset:
	gamestate.difficulty = restartgame;
	restartgame = gd_Continue;
	do
	{
startlevel:
		SetupGameLevel(true);

loaded:
		keenkilled = false;
		SD_WaitSoundDone();

		PlayLoop();

		if (playstate != ex_loadedgame)
		{
			// DEBUG : Gem Count / Keys
			gamestate.keys[0] = gamestate.keys[1] = gamestate.keys[2] = gamestate.keys[3] = 0;
#ifdef KEEN5
			gamestate.keycard = false;
#endif
		}
		
		levelcompleted = -1;
		switch (playstate)
		{
		case ex_resetgame:
			goto reset;

		case ex_loadedgame:
			goto loaded;

		case ex_died:
			HandleDeath();
			break;

#if defined KEEN4
		case ex_rescued:
			if (mapon != 0)
			{
				SD_PlaySound(SND_LEVELDONE);
			}
			levelcompleted = mapon;
			gamestate.leveldone[mapon] = true;
			RescuedMember();
			if (gamestate.rescued != 8)
			{
				gamestate.mapon = 0;
			}
			else
			{
				RF_FixOfs(0,0);
				FinaleLayout();
				CheckHighScore(gamestate.score, gamestate.rescued);
				return;
			}
			break;

#elif defined KEEN5
		case ex_fusebroke:
			SD_PlaySound(SND_LEVELDONE);
			levelcompleted = mapon;
			gamestate.leveldone[mapon] = ex_fusebroke;
			FinishedFuse();
			gamestate.mapon = 0;
			break;

		case ex_qedbroke:
			FreeGraphics();
			RF_FixOfs(0,0);
			VW_FixRefreshBuffer();
			FinaleLayout();
			CheckHighScore(gamestate.score, 0);
			return;

#elif defined KEEN6
		case ex_hook:
			GotHook();
			goto completed;

		case ex_sandwich:
			GotSandwich();
			goto completed;

		case ex_card:
			GotPasscard();
			goto completed;

		case ex_molly:
			FreeGraphics();
			RF_FixOfs(0,0);
			VW_FixRefreshBuffer();
			FinaleLayout();
			goto check_score;

#endif
		case ex_completed:
		case ex_foot:
		case ex_portout:
completed:
			if (mapon != 0)
			{
				SD_PlaySound(SND_LEVELDONE);
				gamestate.mapon = 0;
				levelcompleted = mapon;
				gamestate.leveldone[mapon] = true;
			}
			else
			{
				US_CenterWindow(26, 8);
				PrintY += 25;
				US_CPrint("One moment");
			}
			break;

		case ex_abortgame:
			IN_ClearKeysDown();
			return;
		}
	} while (gamestate.lives >= 0);

GameOver();

check_score:
#if defined KEEN4
CheckHighScore(gamestate.score, gamestate.rescued);
;
#else
	temp = 0;
#if defined KEEN6
	for (i = 0; i < GAMELEVELS; i++)
	{
		if (gamestate.leveldone[i])
			temp++;
	}
#endif
	CheckHighScore(gamestate.score, temp);
#endif

};

