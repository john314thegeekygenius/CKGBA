/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


//===========================================================================

/*
==========================
=
= InitGame
=
= Load a few things right away
=
==========================
*/

void InitGame(void)
{
	CK_InitVideo();
	SD_InitAudio();
	CK_SetupSprites();

	US_TextScreen();

	VW_ClearVideo(BLACK);
};

//===========================================================================

/*
==========================
=
= Quit
=
==========================
*/

void Quit(char *error)
{
//	Uint16 finscreen;
/*
	if (!error)
	{
		CA_SetAllPurge();
		CA_CacheGrChunk(ORDERSCREEN);
		finscreen = (Uint16)grsegs[ORDERSCREEN];
	}

	VW_ClearVideo(BLACK);
	VW_SetLineWidth(40);

	ShutdownId();
	if (error && *error)
	{
		puts(error);
		if (tedlevel)
		{
			getch();
			execlp("TED5.EXE", "TED5.EXE", "/LAUNCH", NULL);
		}
		else if (US_ParmPresent("windows"))
		{
			bioskey(0);
		}
		exit(1);
	}

	if (!NoWait)
	{
		movedata(finscreen, 7, 0xB800, 0, 4000);
		gotoxy(1, 24);
		if (US_ParmPresent("windows"))
		{
			bioskey(0);
		}
	}

	exit(0);*/
	VW_ClearVideo(0x49786234);
	while(1); // Make the game run forever
};

//===========================================================================

/*
=====================
=
= DemoLoop
=
=====================
*/

void DemoLoop(void)
{
	Sint16 i, state;
	Sint16 level;
	DemoMode = demo_Off;

//
// demo loop
//
	state = 0;
	playstate = ex_stillplaying;
	playstate = ex_resetgame;
	while (1)
	{
		switch (state++)
		{
		case 0:
			Terminator();
			break;
		case 1:
			RunDemo(0);
			break;

		case 2:
			StarWars();
			break;

		case 3:
			RunDemo(1);
			break;

		case 4:
			ShowHighScores();
			break;

		case 5:
			RunDemo(2);
			break;

		case 6:
			state = 0;
			RunDemo(3);
			break;
		}

		while (playstate == ex_resetgame || playstate == ex_loadedgame)
		{
			GameLoop();
			ShowHighScores();
			if (playstate == ex_resetgame || playstate == ex_loadedgame)
			{
				continue;	// don't show title screen, go directly to GameLoop();
			}
			ShowTitle();
			///////////////
			// this is completely useless:
			if (playstate == ex_resetgame || playstate == ex_loadedgame)
			{
				continue;
			}
			///////////////
		}
	}
}


//===========================================================================


/*
==========================
=
= main
=
==========================
*/

int main(){
	InitGame();
	DemoLoop();
	Quit("Demo loop exited???");
	return 0;
};
