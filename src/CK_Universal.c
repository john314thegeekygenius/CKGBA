/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

//      Global variables
boolean ingame,abortgame,loadedgame;
GameDiff restartgame = gd_Continue;

boolean         NoWait,
			HighScoresDirty;
word            PrintX,PrintY;
word            WindowX,WindowY,WindowW,WindowH;

// From ID_RF.C
unsigned	tics = 1;
long		lasttimecount = 0;

void GBA_UserIRQ(){
	TimeCount += 1;
};

//===========================================================================

/*
=====================
=
= RF_CalcTics
=
=====================
*/

void RF_CalcTics (void)
{
	long	newtime,oldtimecount;

//
// calculate tics since last refresh for adaptive timing
//
	if (lasttimecount > TimeCount)
		TimeCount = lasttimecount;		// if the game was paused a LONG time

	if (DemoMode)					// demo recording and playback needs
	{								// to be constant
//
// take DEMOTICS or more tics, and modify Timecount to reflect time taken
//
		oldtimecount = lasttimecount;
//		while (TimeCount<oldtimecount+DEMOTICS*2);
		lasttimecount = oldtimecount + DEMOTICS;
		TimeCount = lasttimecount + DEMOTICS;
		tics = DEMOTICS;
	}
	else
	{
//
// non demo, so report actual time
//
		do
		{
			newtime = TimeCount;
			tics = newtime-lasttimecount;
		} while (tics<MINTICS);
		lasttimecount = newtime;

		if (tics>MAXTICS)
		{
			TimeCount -= (tics-MAXTICS);
			tics = MAXTICS;
		}
	}
}


// Draws the into text screen
void US_TextScreen(){
	
};












/*
    Derrived from assembly code


;============================================================================
;
;                           RANDOM ROUTINES
;
;============================================================================

	FARDATA

rndindex	dw	?

	CODESEG

LastRnd		dw	?
*/

const unsigned char ck_rndtable[] = {0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66,
  74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36,
95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188,
52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224,
149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242,
145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0,
175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235,
25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113,
94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75,
136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196,
135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113,
80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241,
24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224,
145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95,
28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226,
71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36,
17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106,
197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136,
120, 163, 236, 249};

unsigned short ck_rndindex = 0;

/*

;=================================================
;
; void US_InitRndT (boolean randomize)
; Init table based RND generator
; if randomize is false, the counter is set to 0
;
;=================================================

PROC	US_InitRndT randomize:word
	uses	si,di
	public	US_InitRndT

	mov	ax,SEG rndtable
	mov	es,ax
	mov	ax,[randomize]
	or	ax,ax
	jne	@@timeit		;if randomize is true, really random

	mov	dx,0			;set to a definite value
	jmp	@@setit

@@timeit:
	mov	ah,2ch
	int	21h			;GetSystemTime
	and	dx,0ffh

@@setit:
	mov	[es:rndindex],dx
	ret

ENDP
*/
//unsigned int CK_US_InitRndT(unsigned short randomize){
unsigned int US_InitRndT(){
//    if(randomize){
        // Initiate with (time)
//        ck_rndindex = time(t);
//    }else{
        // Initiate with nothing
        ck_rndindex = 0;
//    }
};

/*

;=================================================
;
; int US_RndT (void)
; Return a random # between 0-255
; Exit : AX = value
;
;=================================================
PROC	US_RndT
	public	US_RndT

	mov	ax,SEG rndtable
	mov	es,ax
	mov	bx,[es:rndindex]
	inc	bx
	and	bx,0ffh
	mov	[es:rndindex],bx
	mov	al,[es:rndtable+BX]
	xor	ah,ah

	ret

ENDP
*/

unsigned int US_RndT(){
    return ck_rndtable[(ck_rndindex++)&0xFF];
};

