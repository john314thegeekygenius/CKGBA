/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_UNIVERSAL__
#define __CK_UNIVERSAL__

// From ID_RF.C
extern	unsigned	tics;
extern	long		lasttimecount;


// From ID_US.H
typedef	enum
{
    gd_Continue,
    gd_Easy,
    gd_Normal,
    gd_Hard
} GameDiff;

extern	boolean		ingame,		// Set by game code if a game is in progress
					abortgame,	// Set if a game load failed
					loadedgame,	// Set if the current game was loaded
#ifdef KEEN6
					checkpassed,
#endif
					NoWait,
					HighScoresDirty;
extern	GameDiff	restartgame;	// Normally gd_Continue, else starts game
extern	word		PrintX,PrintY;	// Current printing location in the window
extern	word		WindowX,WindowY,// Current location of window
					WindowW,WindowH;// Current size of window

void US_TextScreen();


// From Assembly Code
unsigned int US_InitRndT();
unsigned int US_RndT();

#endif
