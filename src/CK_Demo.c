/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


boolean scorescreenkludge;

/*
=============================================================================

							TERMINATOR INTRO

=============================================================================
*/

/*
============================
=
= Terminator
=
============================
*/


void Terminator(void)
{/*
	Uint16 i, shift, bufsize;
	Sint16 far *source;
	Uint8 far *dest;
	Uint16 srcseg, destseg;
	boolean pagefinished;
	Uint16 rowofs[200];

	pagefinished = false;
	CA_SetAllPurge();
	SetPaletteEx(colors[0]);	// all black
	VW_ClearVideo(BLACK);
	VW_SetLineWidth(248);	// 1984 pixels total, we're using 992 per "page"

	CA_CacheGrChunk(TITLEPICPIC);
	CA_CacheGrChunk(BIGCOMMANDER);
	CA_CacheGrChunk(BIGKEEN);
	keen = grsegs[BIGKEEN];
	commander = grsegs[BIGCOMMANDER];

	EGAMAPMASK(1);

	keenstart = keen->width + 200;
	VW_SetScreen((keenstart/8)+1, 0);

	//
	// draw the "KEEN" pic (to first "page")
	//
	for (i=0; i<200; i++)
	{
		source = (Sint16 far *)((byte _seg *)keen + keen->rowofs[i]);
		dest = MK_FP(0xA000, ylookup[i]);
		dest += 25;	// 25 bytes -> 200 pixels
		DrawScan(source, dest);
	}
	//
	// copy pic from first "page" to second "page"
	//
	VW_ScreenToScreen(0, 124, 109, 200);

	//
	// create pre-shifted image buffers for the "COMMANDER" pic
	// (only 100 pixels high instead of 200 pixels to save memory)
	//
	commanderbwide = (commander->width + 7) / 8;
	commanderbwide = (commanderbwide + 3) & ~1;
	bufsize = commanderbwide * 100;	// half height
	for (shift = 0; shift < 8; shift++)
	{
		MM_GetPtr(&cmdrshifts[shift], bufsize);
	}

	//
	// re-assign shape pointers (memory manager might have moved the buffers)
	//
	keen = grsegs[BIGKEEN];
	commander = grsegs[BIGCOMMANDER];

	//
	// draw the first (unshifted) version of the "COMMANDER" pic to the buffer
	//
	for (i=0; i<100; i++)
	{
		rowofs[i*2] = rowofs[i*2+1] = i * commanderbwide;
		source = (Sint16 far *)((byte _seg *)commander + commander->rowofs[i*2]);
		dest = (Uint8 _seg *)cmdrshifts[0] + rowofs[i*2];
		DrawScan(source, dest);
	}

	//
	// create the shifted versions of the "COMMANDER" pic
	//
	for (shift = 1; shift < 8; shift++)
	{
		srcseg = FP_SEG(cmdrshifts[shift-1]);
		destseg = FP_SEG(cmdrshifts[shift]);
		asm {
			mov	ds, srcseg;
			mov	es, destseg;
			mov	cx, bufsize;
			clc;
			xor	si, si;
			xor	di, di;
		}
l1:
		asm {
			lodsb;
			rcr	al, 1;
			stosb;
			loop	l1;
			mov	ax, ss;
			mov	ds, ax;
		}
	}

	//
	// prepare (and set) the palettes
	//
	termcolors[16] = termcolors2[16] = termcolors[16] = bordercolor;
	SetPalette(termcolors);

	//
	// cache the credits pics (they are converted into a special
	// format to make shifted drawing easier during the animation)
	//
	for (i=0; i<4; i++)
	{
		LoadPlaque(i);
	}

	//
	// play the animation
	//
	plaque = lastframe = 0;
	plaquephase = -1;
	SlideLetters();

	//
	// free some of the buffers
	// (shrink animation needs additional memory)
	//
	for (i=0; i<4; i++)
	{
		MM_FreePtr(&basepl[i]);
	}
	for (shift=0; shift<8; shift++)
	{
		MM_FreePtr(&cmdrshifts[shift]);
	}

	//
	// do the shrinking and fizzle fade animations
	// (if intro wasn't aborted)
	//
	if (!LastScan)
	{
		ScaleDown();
	}

	if (!LastScan)
	{
		FinishPage();
		pagefinished = true;
	}

	//
	// free the remaining buffers
	//
	MM_SetPurge(&grsegs[BIGCOMMANDER], 3);
	MM_SetPurge(&grsegs[BIGKEEN], 3);

	//
	// switch back to default graphics settings
	//
	VW_ClearVideo(BLACK);
	VW_SetLineWidth(SCREENWIDTH);
	VW_SetDefaultColors();
	RF_FixOfs();
	CA_ClearMarks();

	//
	// handle input and main menu stuff
	//
	if (LastScan == sc_None)
	{
		return;
	}
#ifndef KEEN6
	if (LastScan == sc_F1)
	{
		HelpScreens();
		return;
	}
#endif
	if (!pagefinished)
	{
		RF_FixOfs();	//redundant
		CA_CacheGrChunk(TITLEPICPIC);
		VW_DrawPic(0, 0, TITLEPICPIC);
		VW_SetScreen(bufferofs, 0);
		IN_Ack();
		CA_ClearMarks();
		if (storedemo)
		{
			playstate = ex_resetgame;
			restartgame = gd_Normal;
			IN_ClearKeysDown();
			NewGame();
			return;
		}
	}

	US_ControlPanel();
	if (restartgame)
	{
		playstate = ex_resetgame;
	}
	else if (loadedgame)
	{
		playstate = ex_loadedgame;
	}*/
}



/*
=============================================================================

							STAR WARS TEXT CRAWL

=============================================================================
*/

/*
============================
=
= StarWars
=
============================
*/

void StarWars(void)
{/*
	SetPaletteEx(colors[0]);	// all black
	VW_ClearVideo(BLACK);
	VW_SetLineWidth(42);	// 336 pixels
	VW_SetScreen(0, 0);
	pageon = pageofs = 0;
	CA_SetAllPurge();
	CA_CacheGrChunk(STARTFONT+2);
	fontnumber = 2;
	DrawSWText();
	fontnumber = 0;

	CA_CacheGrChunk(SW_BACKGROUNDPIC);
	bufferofs = 0x8000;
	VW_DrawPic(0, 0, SW_BACKGROUNDPIC);
	CA_SetAllPurge();
	SetPaletteEx(starcolors);
	bufferofs = 0;
	CompileSWUpdate();

	if (!LastScan)
	{
		StartMusic(STARWARSMUSIC);
		ScrollSWText();
		StopMusic();
	}

	MM_FreePtr(&linecode);
	MM_FreePtr(&bittables);
	MM_FreePtr(&sourcepic);

	VW_ClearVideo(BLACK);
	VW_SetLineWidth(SCREENWIDTH);
	VW_SetDefaultColors();
	RF_FixOfs();
	CA_ClearMarks();

	CheckLastScan();*/
}

//===========================================================================

/*
============================
=
= ShowTitle
=
============================
*/

void ShowTitle(void)
{/*
	panadjust = 0;
	CA_CacheGrChunk(TITLEPICPIC);
	VW_DrawPic(0, 0, TITLEPICPIC);
#if GRMODE == CGAGR
	VW_UpdateScreen();
#else
	VW_SetScreen(displayofs, 0);
	VW_ScreenToScreen(bufferofs, displayofs, 42, 224);
#endif
	IN_UserInput(6*TickBase, false);
	CA_ClearMarks();
	CheckLastScan();*/
}

//===========================================================================

/*
============================
=
= RunDemo
=
============================
*/

void RunDemo(Sint16 num)
{
	Uint16 *demodata;
	
	NewGame();

    /*
	num += DEMO0;
	demodata = grsegs[num];
	gamestate.mapon = demodata[0];
	DemoSize = demodata[1];
    //DemoBuffer = grsegs[num]+4;
	IN_StartDemoPlayback(DemoBuffer, DemoSize);*/
	SetupGameLevel(true);/*
	if (scorescreenkludge)
	{
		DrawHighScores();
	}*/

	PlayLoop();
	/*IN_StopDemo();
	MM_FreePtr(&(memptr)DemoBuffer);
	VW_FixRefreshBuffer();
	CA_ClearMarks();
	CheckLastScan();*/
}

//===========================================================================

/*
============================
=
= DrawHighScores
=
============================
*/

void DrawHighScores(void)
{
    /*
	Uint16 i, n;
	Uint16 width, height;
	HighScore *entry;
	Uint16 oldbufferofs;
	char buf[16], *bufptr;
	
	RF_NewPosition(0, 0);
	oldbufferofs = bufferofs;
	bufferofs = masterofs;
#ifdef KEEN5
#if GRMODE == CGAGR
	fontcolor = 2;
#else
	fontcolor = BLUE ^ LIGHTMAGENTA;	// blue text on light magenta background (XOR draw mode!)
#endif
#endif
	for (i=0, entry=&Scores[0]; i<MaxScores; i++, entry++)
	{
		PrintY = i*16 + HIGHSCORE_TOP;
		PrintX = HIGHSCORE_LEFT;
		US_Print(entry->name);
#ifdef KEEN4
		PrintX = 152;
		for (n=0; n<entry->completed; n++)
		{
			VWB_DrawTile8(PrintX, PrintY+1, 71);
			PrintX += 8;
		}
#endif
		ultoa(entry->score, buf, 10);
		for (bufptr=buf; *bufptr; bufptr++)
		{
			*bufptr = *bufptr + 81;
		}
		USL_MeasureString(buf, &width, &height);
		PrintX = HIGHSCORE_RIGHT - width;
		US_Print(buf);
	}
	fontcolor = WHITE;	// back to default color
	bufferofs = oldbufferofs;*/
}

//===========================================================================

/*
============================
=
= CheckHighScore
=
============================
*/

void CheckHighScore(Sint32 score, Sint16 completed)
{/*
	Uint16 i, n;
	Sint16 index;
	HighScore entry;
	
	strcpy(entry.name, "");	//Note: 'entry.name[0] = 0;' would be more efficient
	entry.score = score;
	entry.completed = completed;
	for (i=0, index=-1; i<MaxScores; i++)
	{
		if (Scores[i].score < entry.score ||
			(Scores[i].score == entry.score && Scores[i].completed < entry.completed))
		{
			n=MaxScores;
			while (--n > i)
			{
				Scores[n] = Scores[n-1];
			}
			Scores[i] = entry;
			index = i;
			HighScoresDirty = true;
			break;
		}
	}
	if (index != -1)
	{
		scorescreenkludge = true;
		gamestate.mapon = HIGHSCORE_MAP;
		SetupGameLevel(true);
		DrawHighScores();
#ifdef KEEN5
#if GRMODE == CGAGR
		fontcolor = 2;
#else
		fontcolor = BLUE ^ LIGHTMAGENTA;	// blue text on light magenta background (XOR draw mode!)
#endif
#endif
		RF_Refresh();
		RF_Refresh();
		PrintY = i*16 + HIGHSCORE_TOP;
		PrintX = HIGHSCORE_LEFT;
		US_LineInput(PrintX, PrintY, Scores[index].name, NULL, true, MaxHighName, 112);
		scorescreenkludge = false;
	}
#ifdef KEEN5
	fontcolor = 15;	// back to default color (white)
#endif*/
}

//===========================================================================

/*
============================
=
= ShowHighScores
=
============================
*/

void ShowHighScores(void)
{/*
	scorescreenkludge = true;
	IN_ClearKeysDown();
	RunDemo(4);
	scorescreenkludge = false;*/
}

