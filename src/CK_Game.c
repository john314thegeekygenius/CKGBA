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
Sint16 levelcompleted;
Sint32 chunkcount, chunkmax, handpic;

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
	GBA_DMA_MemSet32(&gamestate, 0, sizeof(gamestate)>>2); // TODO: Check this
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
    // TODO:
    // Make this display a game over screen
    /*
	VW_FixRefreshBuffer();
	US_CenterWindow(16, 3);
	US_PrintCentered("Game Over!");
	VW_UpdateScreen();
	IN_ClearKeysDown();
	IN_UserInput(4*TickBase, false);
*/}
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
    /*
	Uint16	i,compressed,expanded;
	objtype	*ob;
	memptr	bigbuffer;

	gamestate.riding = NULL;

	if (!CA_FarWrite(handle, (byte far *)&gamestate, sizeof(gamestate)))
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
	}
	MM_FreePtr(&bigbuffer);*/
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
		VW_UpdateScreen();
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
		VW_UpdateScreen();
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
    // TODO:
    // Make this fix the world map
    /*

	Uint16 x, y, planeoff, info, level, tag;
	Uint16 far *infoptr;

	planeoff = 0;
	infoptr = mapsegs[2];
	for (y = 0; y < mapheight; y++)
	{
		for (x = 0; x < mapwidth; x++, infoptr++, planeoff++)
		{
			info = *infoptr;
			level = info & 0xFF;
			if (level >= MINDONELEVEL && level <= MAXDONELEVEL && gamestate.leveldone[level])
			{
				tag = info >> 8;
				*infoptr = 0;	// BUG: infoplane value should only be set to 0 if tag == 0xC0
				if (tag == 0xD0)
				{
					mapsegs[1][planeoff] = 0;
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
	}*/
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
    // TODO:
    // Make this fade out
    /*

	VW_FadeOut();
	fadecount = 0;
	RF_SetRefreshHook(&FadeAndUnhook);
    */
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
    // TODO:
    // Make this do somthing??
    /*

	if (++fadecount == 2)
	{
		VW_FadeIn();
		RF_SetRefreshHook(NULL);
		TimeCount = lasttimecount;	// don't adaptively time the fade
	}*/
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

void SetupGameLevel(boolean loadnow)
{
    // TODO:
    // Make this "do the right thing"
    /*

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

//
// load the level header and three map planes
//
	CA_CacheMap(gamestate.mapon);

//
// let the refresh manager set up some variables
//
	RF_NewMap();

//
// decide which graphics are needed and spawn actors
//
	CA_ClearMarks();
	ScanInfoPlane();
	if (mapon == 0)
	{
		PatchWorldMap();
	}
	RF_MarkTileGraphics();

//
// have the caching manager load and purge stuff to make sure all marks
// are in memory
//
	MM_BombOnError(false);
	CA_LoadAllSounds();
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
			_fstrcpy(str, levelenter[mapon]);
			CA_CacheMarks(str);
		}
	}
	MM_BombOnError(true);

	if (!mmerror && loadnow)
	{
		DelayedFade();
	}*/
}

//===========================================================================


/*
==========================
=
= DialogDraw
=
==========================
*/

void DialogDraw(char *title, Uint16 numcache)
{
    // TODO:
    // Make this do somthing??
    /*

	Sint16 i;
	Uint16 width, height;
	Sint32 totalfree;

	totalfree = MM_TotalFree();
	if (totalfree < 2048)
	{
		handpic = 5;
	}
	else
	{
		handpic = 0;
		for (i = 0; i < 6; i++)
		{
			CA_CacheGrChunk(i+KEENCOUNT1PIC);
			CA_UnmarkGrChunk(i+KEENCOUNT1PIC);
			if (grsegs[i+KEENCOUNT1PIC])
			{
				MM_SetPurge(&grsegs[i+KEENCOUNT1PIC], PURGE_FIRST);
			}
			else
			{
				mmerror = false;
				handpic = 5;
				break;
			}
		}
	}
	US_CenterWindow(26, 8);
	if (grsegs[KEENCOUNT1PIC])
	{
		VWB_DrawPic(WindowX, WindowY, KEENCOUNT1PIC);
	}
	else
	{
		handpic = 5;
	}
	CA_UnmarkGrChunk(KEENCOUNT1PIC);	//redundant
	WindowW -= 48;
	WindowX += 48;
	SizeText(title, &width, &height);
	PrintY += (WindowH-height)/2 - 4;
	US_CPrint(title);
	VW_UpdateScreen();
	chunkmax = chunkcount = numcache / 6;
	if (!chunkmax && !handpic)
	{
		handpic = 5;
		if (grsegs[KEENCOUNT6PIC])
			VWB_DrawPic(WindowX-24, WindowY+40, KEENCOUNT6PIC);
		VW_UpdateScreen();
	}*/
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
    // TODO:
    // Make this do somthing??
    /*
	if (--chunkcount || handpic > 4)
		return;

	chunkcount = chunkmax;
	if (grsegs[handpic+KEENCOUNT2PIC])
	{
		VWB_DrawPic(WindowX-24, WindowY+40, handpic+KEENCOUNT2PIC);
	}
	VW_UpdateScreen();
	handpic++;*/
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

//==========================================================================

/*
==================
=
= StartDemoRecord
=
==================
*/

//void StartDemoRecord(void){}; // Unused

/*
==================
=
= EndDemoRecord
=
==================
*/

//void EndDemoRecord(void){}; // Unused

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
    // TODO:
    // Make this do somthing??
    /*

	Uint16 y, color, top, bottom, selection, w, h;

	_fstrcpy(str, levelnames[mapon]);
	SizeText(str, &w, &h);

	memset(gamestate.keys, 0, sizeof(gamestate.keys));
	gamestate.lives--;
	if (gamestate.lives >= 0)
	{
		VW_FixRefreshBuffer();
		US_CenterWindow(20, 8);
		PrintY += 3;
		US_CPrint("You didn't make it past");
		top = PrintY+22;
		if (h < 15)
			PrintY += 4;
		US_CPrint(str);
		PrintY = top+2;
		US_CPrint("Try Again");
		PrintY += 4;
		bottom = PrintY-2;
		US_CPrint("Exit to "WORLDMAPNAME);

		IN_ClearKeysDown();
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

			VW_UpdateScreen();

// erase select bar
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y, WHITE);
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+1, WHITE);
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+12, WHITE);
			VWB_Hlin(WindowX+4, WindowX+WindowW-4, y+13, WHITE);
			VWB_Vlin(y+1, y+11, WindowX+4, WHITE);
			VWB_Vlin(y+1, y+11, WindowX+5, WHITE);
			VWB_Vlin(y+1, y+11, WindowX+WindowW-4, WHITE);
			VWB_Vlin(y+1, y+11, WindowX+WindowW-5, WHITE);

			if (LastScan == sc_Escape)
			{
				gamestate.mapon = 0;		// exit to world map
				IN_ClearKeysDown();
				return;
			}

			IN_ReadControl(0, &c);
			if (c.button0 || c.button1 || LastScan == sc_Return || LastScan == sc_Space)
			{
				if (selection)
					gamestate.mapon = 0;		// exit to world map
				return;
			}
			if (c.yaxis == -1 || LastScan == sc_UpArrow)
			{
				selection = 0;
			}
			else if (c.yaxis == 1 || LastScan == sc_DownArrow)
			{
				selection = 1;
			}
		}
	}*/
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
    // TODO:
    // Make this do somthing??
    /*

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
		if (mmerror)
		{
			if (gamestate.mapon != 0)
			{
				mmerror = false;
				US_CenterWindow(20, 8);
				PrintY += 20;
				US_CPrint("Insufficient memory\nto load level!");
				VW_UpdateScreen();
				IN_Ack();
				gamestate.mapon = 0;		// exit to world map
				SetupGameLevel(true);
			}
			if (mmerror)
			{
				Quit("GameLoop: Insufficient memory to load world map!");
			}
		}
loaded:
		keenkilled = false;
		SD_WaitSoundDone();

		PlayLoop();

		if (playstate != ex_loadedgame)
		{
			memset(gamestate.keys, 0, sizeof(gamestate.keys));
#ifdef KEEN5
			gamestate.keycard = false;
#endif
		}
		VW_FixRefreshBuffer();

		if (tedlevel)
		{
			if (playstate == ex_loadedgame)
			{
				goto loaded;
			}
			else if (playstate == ex_died)
			{
				goto startlevel;
			}
			else
			{
				TEDDeath();
			}
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
				FreeGraphics();
				RF_FixOfs();
				VW_FixRefreshBuffer();
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
			RF_FixOfs();
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
			RF_FixOfs();
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
				if (storedemo && mapon == 2)
				{
					IN_ClearKeysDown();
					return;
				}
			}
			else
			{
#if GRMODE != CGAGR
				temp = bufferofs;
				bufferofs = displayofs;
#endif
				US_CenterWindow(26, 8);
				PrintY += 25;
				US_CPrint("One moment");
#if GRMODE == CGAGR
				VW_UpdateScreen();
#else
				bufferofs = temp;
#endif
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
*/
};

