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

// RLEW Stuffs
#define WORDSIZE    2
#define CARMACK_MAGIC 0xABCD

// State stuff
extern const statetype* CK_StateList[];

struct CK_MapBlock {
	unsigned short offset;
	unsigned short newtile;
};

/*
============================
=
= SaveTheGame
=
============================
*/

boolean SaveTheGame(FileHandle handle){

	Uint16	i,compressed,expanded, version;
	objtype	*ob;

	gamestate.riding = NULL;

	if (writeHandle(&handle, &gamestate, sizeof(gamestate)) == File_WriteFail)
		return false;

	version = ConfigVersion;
	if (writeHandle(&handle, &version, sizeof(version)) == File_WriteFail)
		return false;
/*
	for (i = 0; i < 2; i++) {
		// Do Carmack RLEW
		// To not need a buffer, we write directly to the SRAM
		unsigned short *dataptr = CK_CurLevelData + (i*CK_CurLevelSize);
		unsigned short first, second, count, flag = CARMACK_MAGIC;

		first = *(dataptr++);
		second = *(dataptr++);

		expanded = CK_CurLevelWidth * CK_CurLevelHeight * WORDSIZE;

		// Write all the data
		while(expanded){
			if(first != second){
				// Write the first word
				if(writeHandle(&handle, &first, sizeof(Uint16)) == File_WriteFail)
					return false;
				expanded -= WORDSIZE;
				first = second;
				second = *(dataptr++);
				continue;
			}
			// Continue until a different word is found
			count = 1;
			while(first == second){
				second = *(dataptr++);
				count++;
			}
			// Write the magic tag
			if(writeHandle(&handle, &flag, sizeof(Uint16)) == File_WriteFail)
				return false;
			// Write the count
			if(writeHandle(&handle, &count, sizeof(Uint16)) == File_WriteFail)
				return false;
			// Write the value
			if(writeHandle(&handle, &first, sizeof(Uint16)) == File_WriteFail)
				return false;
			first = second;
			second = *(dataptr++);
			expanded -= count*WORDSIZE;
		}
	}*/
	// Save the current animation count
	if (writeHandle(&handle, &CK_CurLevelAniTick, sizeof(CK_CurLevelAniTick)) == File_WriteFail)
		return false;

	struct CK_MapBlock tempblock;
	// Skip the background layer of changed tiles because they should only be animations
	// TODO:
	// Will not work with custom code??? (i.e. allowing for background dynamic changing)
	for(int o = CK_CurLevelSize; o < CK_CurLevelSize*2; o++){
		if(CK_CurLevelData[o] != CK_LevelInfo[(CK_CurLevelIndex*3)+2][o]){
			// Set the info
			tempblock.offset = o;
			tempblock.newtile = CK_CurLevelData[o];
			// Write that block down
			if(writeHandle(&handle, &tempblock, sizeof(tempblock)) == File_WriteFail)
				return false;
		}
	}
	// Set to bad block
	tempblock.offset = 0xFFFF;
	// Write an end block down
	if(writeHandle(&handle, &tempblock.offset, sizeof(unsigned short)) == File_WriteFail)
		return false;

	// store infoblocks
	if(writeHandle(&handle, &CK_InfoPlaneBlockCount, sizeof(CK_InfoPlaneBlockCount)) == File_WriteFail)
		return false;

	for(int i = 0; i < CK_InfoPlaneBlockCount; i++){
		if(writeHandle(&handle, &CK_InfoPlaneBlocks[i], sizeof(struct CK_InfoBlock)) == File_WriteFail){
			return false;
		}
    }

	// store the objects
	unsigned short objectcount = 0;
	for (int i = 0; i < CK_NumOfObjects; i++){
		objtype *ob = &CK_ObjectList[i];
		if(ob->removed || ob->state == NULL) continue;
		objectcount++;
	}
	writeHandle(&handle, &objectcount, sizeof(Uint16));

	for (int i = 0; i < CK_NumOfObjects; i++){
		objtype *ob = &CK_ObjectList[i];
		if(ob->removed || ob->state == NULL) continue;
		// Fix the object
		if(ob->sprite) ob->curSprType = ((objsprite*)ob->sprite)->ck_sprType;
		else ob->curSprType = CKS_EOL;
		if (writeHandle(&handle, ob, sizeof(objtype)) == File_WriteFail) {
			return false;
		}
		Uint16 stateid = 0;
		// Find the state
		while(CK_StateList[stateid]){
			if(ob->state == CK_StateList[stateid]){
				break;
			}
			stateid++;
		}
		if(CK_StateList[stateid] == NULL){
			Quit("SaveTheGame : Need to add obj state!");
		}
		if (writeHandle(&handle, &stateid, sizeof(Uint16)) == File_WriteFail) {
			return false;
		}
	}

    // Wait for 3 seconds
#ifdef __EZ_FLASH
    GBA_Delay(3000);
#endif

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

boolean LoadTheGame(FileHandle handle)
{
	Uint16	i, stateid;
	Uint16	compressed,expanded;
	Uint16            version;
#ifdef KEEN5
	Sint16	numfuses;
#endif
	if (readHandle(&handle, &gamestate, sizeof(gamestate)) == File_ReadFail)
		return false;
	if (readHandle(&handle, &version, sizeof(version)) == File_ReadFail)
		return false;
	if (version != ConfigVersion){
		// Bad save file!
		return false;
	}

#ifdef KEEN5
	//
	// remember the fuses value for later - SetupGameLevel calls
	// ScanInfoPlane, which resets this part of the gamestate
	//
	numfuses = gamestate.numfuses;
#endif

	SetupGameLevel(false);
	/*
	for (i = 0; i < 2; i++) {
		// Do Carmack RLEW
		// To not need a buffer, we write directly to the level
		unsigned short *dataptr = CK_CurLevelData + (i*CK_CurLevelSize);
		unsigned short flag, count;

		expanded = CK_CurLevelWidth * CK_CurLevelHeight * WORDSIZE;

		// Write all the data
		while(expanded){
			if(readHandle(&handle, &flag, sizeof(Uint16))== File_ReadFail)
				return false;
			if(flag == CARMACK_MAGIC){
				if(readHandle(&handle, &count, sizeof(Uint16))== File_ReadFail)
					return false;
				if(readHandle(&handle, &flag, sizeof(Uint16))== File_ReadFail)
					return false;
				expanded -= count*WORDSIZE;
				while(count--){
					*(dataptr++) = flag;
				}
			}else{
				*(dataptr++) = flag;
				expanded -= WORDSIZE;
			}
		}
	}*/
	// Read the current animation count
	if (readHandle(&handle, &CK_CurLevelAniTick, sizeof(CK_CurLevelAniTick)) == File_ReadFail)
		return false;
	
	// TODO:
	// This takes a long time???
	CK_FixTileAnimations(CK_CurLevelAniTick); // Fix the animations to the number of ticks

	struct CK_MapBlock tempblock;
	// Skip the background layer of changed tiles because they should only be animations
	// TODO:
	// Will not work with custom code??? (i.e. allowing for background dynamic changing)
	for(int o = CK_CurLevelSize; o < CK_CurLevelSize*2; o++){
		if(readHandle(&handle, &tempblock.offset, sizeof(unsigned short)) == File_ReadFail)
			return false;
		if(tempblock.offset == 0xFFFF)
			break;
		if(readHandle(&handle, &tempblock.newtile, sizeof(unsigned short)) == File_ReadFail)
			return false;
		CK_CurLevelData[tempblock.offset] = tempblock.newtile;
	}

	CK_FixAnimationBlocks();

	// read infoblocks
	if(readHandle(&handle, &CK_InfoPlaneBlockCount, sizeof(CK_InfoPlaneBlockCount)) == File_ReadFail) {
		return false;
	}

	for(int i = 0; i < CK_InfoPlaneBlockCount; i++){
		if(readHandle(&handle, &CK_InfoPlaneBlocks[i], sizeof(struct CK_InfoBlock)) == File_ReadFail) {
			return false;
		}
    }

	InitObjArray();
	unsigned short objectCount = 0;
	
	if (readHandle(&handle, &objectCount, sizeof(Uint16)) == File_ReadFail) {
		return false;
	}
	if(objectCount > MAXACTORS) return false; // Uhhh

	// Load the objects
	ck_newobj = player;
	
	if (readHandle(&handle, ck_newobj, sizeof(objtype))== File_ReadFail) {
		return false;
	}
	if (readHandle(&handle, &stateid, sizeof(Uint16))== File_ReadFail) {
		return false;
	}

	ck_newobj->needtoreact = true;
	ck_newobj->sprite = NULL;

	CK_SetSprite(&ck_newobj->sprite, ck_newobj->curSprType);
	ck_newobj->state = CK_StateList[stateid];

	ck_newobj = scoreobj;

	for (int i = scoreobj->uuid; i < objectCount; i++){
		if (readHandle(&handle, ck_newobj, sizeof(objtype))== File_ReadFail) {
			return false;
		}
		if (readHandle(&handle, &stateid, sizeof(Uint16))== File_ReadFail) {
			return false;
		}
		
		ck_newobj->needtoreact = true;
		ck_newobj->sprite = NULL; // Sprite is bad
		CK_SetSprite(&ck_newobj->sprite, ck_newobj->curSprType);

		ck_newobj->state = CK_StateList[stateid];

		if (ck_newobj->obclass == stunnedobj)
		{
			ck_newobj->temp3 = 0;	//clear sprite ptr for the stars
		}
#if defined KEEN4
		else if (ck_newobj->obclass == platformobj)
		{
			ck_newobj->temp2 = ck_newobj->temp3 = 0;	//clear sprite ptrs
		}
#elif defined KEEN5
		else if (ck_newobj->obclass == mineobj)
		{
			ck_newobj->temp4 = 0;	//clear sprite ptr
		}
		else if (ck_newobj->obclass == spherefulobj)
		{
			ck_newobj->temp1 = ck_newobj->temp2 = ck_newobj->temp3 = ck_newobj->temp4 = 0;	//clear sprite ptrs
		}
#elif defined KEEN6
		else if (ck_newobj->obclass == platformobj)
		{
			ck_newobj->temp3 = 0;	//clear sprite ptr
		}
#endif
		if (i < objectCount) // TODO: Bug here??
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

	// Fix the graphics
	if(scoreobj && scoreobj->sprite){
		switch(showscorebox){
			default:
			case CK_DISP_SCORE_DOS:
				CK_SetSpriteGfx(&scoreobj->sprite, CKS_SCOREBOXDOS);
				break;
			case CK_DISP_SCORE_GBA:
				CK_SetSpriteGfx(&scoreobj->sprite, CKS_SCOREBOXGBA);
				break;
		}
	}

#ifdef KEEN5
	gamestate.numfuses = numfuses;	// put value from saved game back in place 
#endif
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
#ifdef FIX_BUGS
				if(tag == 0xC0){
#endif
				CK_SetInfo((y*CK_CurLevelWidth)+x,0);	// BUG: infoplane value should only be set to 0 if tag == 0xC0
#ifdef FIX_BUGS
				}
#endif
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
	GBA_HideSprites();
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

	for (int i=0;i<6;i++){
//
// update thermometer
//
		DialogUpdate();
		// Add some simulated delay
		GBA_Delay(250);
	}

	// Add some simulated delay
	GBA_Delay(250);

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

extern boolean infinitelives;

void HandleDeath(void)
{

	Uint16 y, color, top, bottom, selection, selectionl, w, h;

	SizeText(levelnames[mapon], &w, &h);

	memset(gamestate.keys, 0, sizeof(gamestate.keys));
	if(!infinitelives){
		gamestate.lives--;
	}

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
			RF_FixOfs(0,0);
			//VW_FixRefreshBuffer();
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
			RF_FixOfs(0,0);
			//VW_FixRefreshBuffer();
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

