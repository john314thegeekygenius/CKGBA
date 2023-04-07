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

#ifndef __CK_UNIVERSAL__
#define __CK_UNIVERSAL__

#define ConfigVersion 0x0111 // v.0.1.1.1

// GBA File Blocks
typedef enum {
	CKF_CONFIG = 0xCC01,
	CKF_SLOT1 = 0xAA01,
	CKF_SLOT2 = 0xAA02,
	CKF_SLOT3 = 0xAA03,
//	CKF_SLOT4 = 0xAA04,
//	CKF_SLOT5 = 0xAA05,
//	CKF_SLOT6 = 0xAA06,
} CK_FILE_BLOCKS;


// GBA has a max of 64 KBytes in SRAM - 8bit Bus width

typedef enum {
	CKFB_CONFIG_S = 0x240, // bytes for the config file
	CKFB_SLOT_S = 0x5432, // bytes for save file (~21K)
} CK_FILE_BLOCKS_SIZES;


void CK_TimedRumble(unsigned short ticks);

// From ID_IN.C
extern int LastScan;

// From ID_US.H


#define _ck_max(a, b) ((a)>(b))?(a):(b)


#define	MaxX	GBA_SCREEN_WIDTH
#define	MaxY	GBA_SCREEN_HEIGHT

#define	MaxHelpLines	500

#define	MaxHighName	57
#ifdef CAT3D
#define	MaxScores	7
#else
#define	MaxScores	8
#endif
typedef	struct
		{
			char	name[MaxHighName + 1];
			long	score;
			word	completed;
		} HighScore;

#define	MaxGameName		16
#define	MaxSaveGames	3
typedef	struct
		{
			char	signature[4]; // 4
//			word	*oldtest;
			boolean	present; // ???
			char	name[MaxGameName + 1]; // 17
		} SaveGame;

#define	MaxString	128	// Maximum input string size

typedef	struct
		{
			int	x,y,
				w,h,
				px,py;
		} WindowRec;	// Record used to save & restore screen windows

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

extern	boolean		Button0,Button1,
					CursorBad;
extern	int			CursorX,CursorY;

#define	US_HomeWindow()	{PrintX = WindowX; PrintY = WindowY;}

// Should have been from ID_VW.C
extern unsigned char fontcolor;
extern void USL_DrawString(char*pstr);
void US_ShowScreen(unsigned short screen);


void US_Setup();
void US_Shutdown(void);

extern void US_TextScreen(), 
				US_UpdateTextScreen(void),
				US_FinishTextScreen(void),
                US_ControlPanel(void),
				US_DrawWindow(word x,word y,word w,word h),
				US_CenterWindow(word,word),
				US_PrintCentered(char *s),
				US_CPrint(char *s),
				US_CPrintLine(char *s),
				US_Print(char *s),
				US_SafePrint(char *s),
				US_PrintUnsigned(longword n),
				US_PrintSigned(long n),
				US_CheckHighScore(long score, word other);
//				US_DisplayHighScores(int which);

int _ck_strcpy(char *dest, char *src);
char *_ck_ultoa(unsigned int n, char*buff, int mode);
char *_ck_ltoa(int n, char*buff, int mode);
unsigned int _ck_strlen(char *str);

// From Assembly Code
unsigned int US_InitRndT();
unsigned int US_RndT();


#endif
