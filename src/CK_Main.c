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
	//////////////////////////////
	CK_InitVideo();
	SD_InitAudio();
	CK_SetupSprites();

	US_InitRndT(true);              // Initialize the random number generator
	US_Setup();
	CK_FixPalette();
#ifdef CK_INTR_ENDER
	US_TextScreen();
#endif

	VW_ClearVideo(BLACK);
};


//===========================================================================

/*
=====================
=
= SizeText
=
= Calculates width and height of a string that contains line breaks
= (Note: only the height is ever used, width is NOT calculated correctly)
=
=====================
*/

void SizeText(char *text, Uint16 *width, Uint16 *height)
{
	char *ptr;
	char c;
	Uint16 w, h;
	char strbuf[80];

	*width = *height = 0;
	ptr = &strbuf[0];
	while ((c=*(text++)) != '\0')
	{
		*(ptr++) = c;
		if (c == '\n' || !*text)
		{
			USL_MeasureString(strbuf, &w, &h);	// BUG: strbuf may not have a terminating '\0' at the end!
			*height += h;
			if (*width < w)
			{
				*width = w;
			}
			ptr = &strbuf[0];
		}
	}
}


/*
==========================
=
= Quit
=
==========================
*/

bool gameQuit = false;

void Quit(char *error)
{
	gameQuit = true;

	CK_NukeObjectsSprites();
	SD_MusicOff();
	GBA_StopChannel(GBA_CHANNEL_A);
	GBA_StopChannel(GBA_CHANNEL_B);
	VW_ClearVideo(BLACK);
	CK_FixPalette();
	// Remove the second background
	*(volatile unsigned int*)GBA_REG_DISPCNT &= ~GBA_ENABLE_BG2;
	VW_ClearScroll();

	if (!error || !(*error)){
#ifdef CK_INTR_ENDER
		US_ShowScreen(3);
#endif
		// Save the config data
		US_Shutdown();
		
		while(1); // Make the game run forever?
		return;
	}

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
	while (!gameQuit)
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
