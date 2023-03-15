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

	US_InitRndT(true);              // Initialize the random number generator
	US_TextScreen();

	US_Setup();

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
	if (!error || !(*error)){
//		CA_SetAllPurge();
//		CA_CacheGrChunk(ORDERSCREEN);
//		finscreen = (Uint16)grsegs[ORDERSCREEN];
		return;
	}
//	ShutdownId();
	CK_RemoveSprites();
	SD_MusicOff();
	GBA_StopChannel(GBA_CHANNEL_A);
	GBA_StopChannel(GBA_CHANNEL_B);
	VW_ClearVideo(BLACK);
	VW_ClearScroll();
	PrintX = 0;
	PrintY = 0;
	fontcolor = CK_EGA_WHITE;
	US_SafePrint(error);
	PrintX = 0;
	PrintY += 8;
	US_SafePrint(CK_DOS_PROMPT);
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
	while (1)
	{
		playstate = ex_resetgame;
		DemoMode = demo_Off;
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
