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

#define	PORTTILESWIDE		30      // all drawing takes place inside a
#define	PORTTILESHIGH		20		// non displayed port of this size

extern	unsigned	originxglobal,originyglobal;
extern	unsigned	originxtile,originytile;
extern	unsigned	originxscreen,originyscreen;
extern	unsigned	originxmin,originxmax,originymin,originymax;

// From ID_IN.C
extern int LastScan;

// From ID_US.H

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

#define	MaxGameName		32
#define	MaxSaveGames	6
typedef	struct
		{
			char	signature[4];
			word	*oldtest;
			boolean	present;
			char	name[MaxGameName + 1];
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

void US_Setup();

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
				US_PrintUnsigned(longword n),
				US_PrintSigned(long n),
				US_ControlPanel(void),
				US_CheckHighScore(long score, word other);
//				US_DisplayHighScores(int which);


// From Assembly Code
unsigned int US_InitRndT();
unsigned int US_RndT();


#endif
