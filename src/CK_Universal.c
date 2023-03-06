/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

// From OG code
unsigned char fontcolor = CK_EGA_BLACK; // Default to black

// CK Font stuffs
extern const unsigned char CK_FONT[];
extern const unsigned int CK_FONT_size;

void CK_BlitChar(char c, uint16_t x, uint16_t y, uint32_t col){
	uint32_t *vidmem = (uint32_t*)TILESTART_1+(y<<8)+(x<<3);
	for(int i = 0; i < 8; i++){
		*vidmem = *((uint32_t*)CK_FONT+i+(c*8)) & col;//(27*16);//(c*8*4)+(y*8);
		vidmem++;
	}
};

void USL_DrawString(char*pstr){
	unsigned short px,py;
	px = PrintX>>3;
	py = PrintY>>3;
	while(*pstr!='\0'){
		CK_BlitChar(*pstr,px,py,CK_TXTCOL(fontcolor));
		++px;
		++pstr;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//      Global variables
boolean ingame,abortgame,loadedgame;
GameDiff restartgame = gd_Continue;

boolean         NoWait,
			HighScoresDirty;
word            PrintX,PrintY;
word            WindowX,WindowY,WindowW,WindowH;


void US_Setup(){
	WindowX = 0;
	WindowY = 0;
};

void USL_MeasureString(char *str, word *w, word *h){
	*h = 8;
	*w=0;
	while(*str){
		str++;
		*w+=8;
	}
};

///////////////////////////////////////////////////////////////////////////
//
//      US_Print() - Prints a string in the current window. Newlines are
//              supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_Print(char *s)
{
	char    c,*se;
	word    w,h;

	while (*s)
	{
		se = s;
		while ((c = *se) && (c != '\n'))
			se++;
		*se = '\0';

		USL_MeasureString(s,&w,&h);
		USL_DrawString(s);

		s = se;
		if (c)
		{
			*se = c;
			s++;

			PrintX = WindowX;
			PrintY += h;
		}
		else
			PrintX += w;
	}
}


char _ck_hex_lookup[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

char *_ck_ultoa(unsigned int n, char*buff, int mode){
	char *dbuff = buff;
	char cbuff[32]; // Assumes only 32 characters can be generated!
	int i = 0;
	if(n==0){
		*dbuff++ = '0';
		*dbuff = 0;
		return buff;
	}
	// Function will crash if more than 31 characters are made (because of null char)
	if(mode == 10){
		while(n){
			cbuff[i] = _ck_hex_lookup[n%10];
			++i; // move the pointer
			n/=10; // Next nibble
		}
	}else if(mode == 16){
		while(n){
			cbuff[i] = _ck_hex_lookup[n&0xF];
			++i; // move the pointer
			n>>4; // Next nibble
		}
	}
	// Reverse the cbuffer into the buffer
	for(int e = i-1; e >= 0; --e){
		*dbuff = cbuff[e];
		++dbuff;
	}
	*dbuff = 0;
	return buff;
};


char *_ck_ltoa(int n, char*buff, int mode){
	char *dbuff = buff;
	char cbuff[32]; // Assumes only 32 characters can be generated!
	int i = 0;
	char neg = 0;
	if(n==0){
		*dbuff++ = '0';
		*dbuff = 0;
		return buff;
	}
	// Function will crash if more than 31 characters are made (because of null char)
	if(mode == 10){
		// Handle if negitive
		if(n<0){
			n = -n;
			neg = 1;
			++i;
		}
		while(n){
			cbuff[i] = _ck_hex_lookup[n%10];
			++i; // move the pointer
			n/=10; // Next nibble
		}
	}else if(mode == 16){
		while(n){
			cbuff[i] = _ck_hex_lookup[n&0xF];
			++i; // move the pointer
			n>>4; // Next nibble
		}
	}
	// If negitive
	if(neg){
	    *dbuff = '-';
	    ++dbuff;
	}
	// Reverse the cbuffer into the buffer
	for(int e = i-1; e >= 0; --e){
		*dbuff = cbuff[e];
		++dbuff;
	}
	*dbuff = 0;
	return buff;
};



///////////////////////////////////////////////////////////////////////////
//
//      US_PrintUnsigned() - Prints an unsigned long
//
///////////////////////////////////////////////////////////////////////////
void
US_PrintUnsigned(longword n)
{
	char    buffer[32];

	US_Print(_ck_ultoa(n,buffer,10));
}

///////////////////////////////////////////////////////////////////////////
//
//      US_PrintSigned() - Prints a signed long
//
///////////////////////////////////////////////////////////////////////////
void
US_PrintSigned(long n)
{
	char    buffer[32];

	US_Print(_ck_ltoa(n,buffer,10));
}

///////////////////////////////////////////////////////////////////////////
//
//      USL_PrintInCenter() - Prints a string in the center of the given rect
//
///////////////////////////////////////////////////////////////////////////
void
USL_PrintInCenter(char *s,Rect r)
{/*
	word    w,h,
			rw,rh;

	USL_MeasureString(s,&w,&h);
	rw = r.lr.x - r.ul.x;
	rh = r.lr.y - r.ul.y;

	px = r.ul.x + ((rw - w) / 2);
	py = r.ul.y + ((rh - h) / 2);
	USL_DrawString(s);*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_PrintCentered() - Prints a string centered in the current window.
//
///////////////////////////////////////////////////////////////////////////
void
US_PrintCentered(char *s)
{
	Rect    r;

	r.ul.x = WindowX;
	r.ul.y = WindowY;
	r.lr.x = r.ul.x + WindowW;
	r.lr.y = r.ul.y + WindowH;

	USL_PrintInCenter(s,r);
}

///////////////////////////////////////////////////////////////////////////
//
//      US_CPrintLine() - Prints a string centered on the current line and
//              advances to the next line. Newlines are not supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_CPrintLine(char *s)
{/*
	word    w,h;

	USL_MeasureString(s,&w,&h);

	if (w > WindowW)
		Quit("US_CPrintLine() - String exceeds width");
	px = WindowX + ((WindowW - w) / 2);
	py = PrintY;
	USL_DrawString(s);
	PrintY += h;*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_CPrint() - Prints a string in the current window. Newlines are
//              supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_CPrint(char *s)
{/*
	char    c,*se;

	while (*s)
	{
		se = s;
		while ((c = *se) && (c != '\n'))
			se++;
		*se = '\0';

		US_CPrintLine(s);

		s = se;
		if (c)
		{
			*se = c;
			s++;
		}
	}*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_ClearWindow() - Clears the current window to white and homes the
//              cursor
//
///////////////////////////////////////////////////////////////////////////
void
US_ClearWindow(void)
{
	/*VWB_Bar(WindowX,WindowY,WindowW,WindowH,WHITE);
	PrintX = WindowX;
	PrintY = WindowY;*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_DrawWindow() - Draws a frame and sets the current window parms
//
///////////////////////////////////////////////////////////////////////////
void
US_DrawWindow(word x,word y,word w,word h)
{/*
	word    i,
			sx,sy,sw,sh;

	WindowX = x * 8;
	WindowY = y * 8;
	WindowW = w * 8;
	WindowH = h * 8;

	PrintX = WindowX;
	PrintY = WindowY;

	sx = (x - 1) * 8;
	sy = (y - 1) * 8;
	sw = (w + 1) * 8;
	sh = (h + 1) * 8;

	US_ClearWindow();

	VWB_DrawTile8M(sx,sy,0),VWB_DrawTile8M(sx,sy + sh,6);
	for (i = sx + 8;i <= sx + sw - 8;i += 8)
		VWB_DrawTile8M(i,sy,1),VWB_DrawTile8M(i,sy + sh,7);
	VWB_DrawTile8M(i,sy,2),VWB_DrawTile8M(i,sy + sh,8);

	for (i = sy + 8;i <= sy + sh - 8;i += 8)
		VWB_DrawTile8M(sx,i,3),VWB_DrawTile8M(sx + sw,i,5);*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_CenterWindow() - Generates a window of a given width & height in the
//              middle of the screen
//
///////////////////////////////////////////////////////////////////////////
void
US_CenterWindow(word w,word h)
{
//	US_DrawWindow(((MaxX / 8) - w) / 2,((MaxY / 8) - h) / 2,w,h);
}


// From ID_US_2.C


///////////////////////////////////////////////////////////////////////////
//
//      USL_SetUpCtlPanel() - Sets the states of the UserItems to reflect the
//              values of all the appropriate variables
//
///////////////////////////////////////////////////////////////////////////
static void
USL_SetUpCtlPanel(void)
{/*
	int     i;

	// Cache in all of the stuff for the control panel
	CA_UpLevel();
	for (i = CONTROLS_LUMP_START;i <= CONTROLS_LUMP_END;i++)
		CA_MarkGrChunk(i);
	for (i = PADDLE_LUMP_START;i <= PADDLE_LUMP_END;i++)
		CA_MarkGrChunk(i);
	CA_MarkGrChunk(STARTFONT+1);            // Little font
	CA_MarkGrChunk(CP_MENUMASKPICM);        // Mask for dialogs
	CA_CacheMarks("Control Panel");
	CA_LoadAllSounds();

	// Do some other setup
	fontnumber = 1;
	US_SetPrintRoutines(VW_MeasurePropString,VWB_DrawPropString);
	fontcolor = F_BLACK;
#ifdef CAT3D
	VW_Bar (0,0,320,200,3); // CAT3D patch
#else
	VW_ClearVideo(3);
#endif
	RF_FixOfs();
	VW_InitDoubleBuffer();

	Communication = uc_None;
	USL_ClearFlags(&rootgroup);
	USL_SetControlValues();
	USL_SetupStack();
	USL_SetupCard();

	if (ingame)
		GameIsDirty = true;
*/
	IN_ClearKeysDown();
	restartgame = gd_Easy;
}


///////////////////////////////////////////////////////////////////////////
//
//      USL_TearDownCtlPanel() - Given the state of the control panel, sets the
//              modes and values as appropriate
//
///////////////////////////////////////////////////////////////////////////
static void
USL_TearDownCtlPanel(void)
{/*
	USL_GetControlValues();
	if (Communication)
		USL_HandleComm(Communication);

	fontnumber = 0; // Normal font
	fontcolor = F_BLACK;
	if (restartgame && USL_ResetGame)
		USL_ResetGame();
	else if (QuitToDos)
	{
		if (tedlevel)
			TEDDeath();
		else
		{
			US_CenterWindow(20,3);
			fontcolor = F_SECONDCOLOR;
			US_PrintCentered("Quitting...");
			fontcolor = F_BLACK;
			VW_UpdateScreen();
			Quit(nil);
		}
	}

	IN_ClearKeysDown();
	SD_WaitSoundDone();
#ifdef CAT3D
	VW_Bar (0,0,320,200,3); // CAT3D patch
#else
	VW_ClearVideo(3);
#endif
	CA_DownLevel();
	CA_LoadAllSounds();*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_ControlPanel() - This is the main routine for the control panel
//
///////////////////////////////////////////////////////////////////////////
#define MoveMin 40
void
US_ControlPanel(void)
{
/*
	extern void HelpScreens(void);

	boolean         resetitem,on;
	word            i;
	int                     ydelta;
	longword        flashtime;
	UserItem        far *item;
	CursorInfo      cursorinfo;

#if 0
	// DEBUG!!!
	{
		USL_SetUpCtlPanel();
		Communication = uc_Loaded;
		CtlPanelDone = true;
		loadedgame = true;
		USL_TearDownCtlPanel();
		return;
	}
#endif

	if ((LastScan < sc_F1) || (LastScan > sc_F10))
		IN_ClearKeysDown();

	USL_SetUpCtlPanel();
	USL_DrawCtlPanel();

	ydelta = 0;
	for (CtlPanelDone = false,resetitem = on = true;!CtlPanelDone;)
	{
		item = &(topcard->items[topcard->cursor]);

		if (resetitem)
		{
			flashtime = TimeCount + (TickBase / 2);
			resetitem = false;
		}

		if (TimeCount >= flashtime)
		{
			on ^= true;
			resetitem = true;
			if (!on)
				item->flags &= ~ui_Selected;
			USL_DrawItemIcon(item);
			item->flags |= ui_Selected;
		}

		VW_UpdateScreen();

		if (LastScan)
		{
			switch (LastScan)
			{
			case sc_UpArrow:
				USL_PrevItem();
				resetitem = true;
				break;
			case sc_DownArrow:
				USL_NextItem();
				resetitem = true;
				break;
			case sc_Return:
				USL_DoItem();
				resetitem = true;
				break;
			case sc_Escape:
				USL_UpLevel();
				resetitem = true;
				break;
#ifndef KEEN6
			case sc_F1:
				HelpScreens();
				USL_DrawCtlPanel();
				resetitem = true;
				break;
#endif
			}

			if
			(
				(!resetitem)
			&&      (
					(LastScan == KbdDefs[0].button0)
				||      (LastScan == KbdDefs[0].button1)
				)
			)
			{
				USL_DoItem();
				resetitem = true;
			}

			if (!resetitem)
			{
				for (item = topcard->items,i = 0;item->type != uii_Bad;item++,i++)
				{
					if (item->hotkey == LastScan)
					{
						USL_SelectItem(topcard,i,true);
						resetitem = true;
						break;
					}
				}
			}

			IN_ClearKeysDown();
		}
		else
		{
			IN_ReadCursor(&cursorinfo);
			ydelta += cursorinfo.y;
			if (cursorinfo.button0)
			{
				do
				{
					IN_ReadCursor(&cursorinfo);
				} while (cursorinfo.button0);
				USL_DoItem();
				resetitem = true;
			}
			else if (cursorinfo.button1)
			{
				do
				{
					IN_ReadCursor(&cursorinfo);
				} while (cursorinfo.button1);
				USL_UpLevel();
				resetitem = true;
			}
			else if (ydelta < -MoveMin)
			{
				ydelta += MoveMin;
				USL_PrevItem();
				resetitem = true;
			}
			else if (ydelta > MoveMin)
			{
				ydelta -= MoveMin;
				USL_NextItem();
				resetitem = true;
			}
		}
	}*/
	USL_TearDownCtlPanel();
}






// From ID_RF.C
unsigned	tics = 1;
long		lasttimecount = 0;

// From ID_IN.C
int LastScan;


void GBA_UserIRQ(){
	TimeCount += 1;

	LastScan = GBA_INV_BUTTONS;

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

