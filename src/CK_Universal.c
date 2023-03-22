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

extern const unsigned char CK_TEXT_SCREENS[];

void US_ShowScreen(unsigned short screen){
	// Fix the screen
	CA_FixGraphics();
	// Fix the offsets
	RF_FixOfs(8,0);
	GBA_DMA_Copy32((unsigned int*)TILESTART_0, (unsigned int*)CK_TEXT_SCREENS+(screen*32*20*8), 32*20*8);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

//      Global variables
word            PrintX,PrintY;
word            WindowX,WindowY,WindowW,WindowH;

boolean         NoWait,
			HighScoresDirty;

const unsigned short ck_file_slots[] = {
	CKF_SLOT1, CKF_SLOT2, CKF_SLOT3, 
	CKF_SLOT4,// CKF_SLOT5, CKF_SLOT6, 
};

SaveGame        Games[MaxSaveGames];
HighScore       Scores[MaxScores] =
			{
#if defined CAT3D
				{"Sir Lancelot",500,3},
				{"",0},
				{"",0},
				{"",0},
				{"",0},
				{"",0},
				{"",0},
#elif defined GOODTIMES
				{"Id Software",10000,0},
				{"Adrian Carmack",10000,0},
				{"John Carmack",10000,0},
				{"Kevin Cloud",10000,0},
				{"Shawn Green",10000,0},
				{"Tom Hall",10000,0},
				{"John Romero",10000,0},
				{"Jay Wilbur",10000,0},
#else
				{"Id Software - '91",10000,0},
				{"",10000,0},
				{"Jason Blochowiak",10000,0},
				{"Adrian Carmack",10000,0},
				{"John Carmack",10000,0},
				{"Tom Hall",10000,0},
				{"John Romero",10000,0},
				{"John314",10000,0}, // Added for clout
#endif
			};


///////////////////////////////////////////////////////////////////////////
//
//      USL_ReadConfig() - Reads the configuration file, if present, and sets
//              things up accordingly. If it's not present, uses defaults. This file
//              includes the high scores.
//
///////////////////////////////////////////////////////////////////////////
static void
USL_ReadConfig(void)
{
	bool gotit = false;
	word            version;
	FileHandle      file;
	SDMode          sd;
	SMMode          sm;
	word 			pal;
	word 			sbox;

	if ((file = openHandle(CKF_CONFIG, FileIO_Read, CKFB_CONFIG_S)) != -1)
	{
		readHandle(&file, &version, sizeof(version));
		if (version != ConfigVersion)
		{
			goto rcfailed;
		}
		readHandle(&file,Scores,sizeof(HighScore) * MaxScores);
		readHandle(&file,&sd,sizeof(sd));
		readHandle(&file,&sm,sizeof(sm));
		readHandle(&file,&pal,sizeof(pal));
		readHandle(&file,&sbox,sizeof(sbox));
				
// TODO:
// Store quiet Adlib sounds
// Store button reconfig?
//		readHandle(file,&ctl,sizeof(ctl));
//		readHandle(file,&(KbdDefs[0]),sizeof(KbdDefs[0]));
		readHandle(&file,&showscorebox,sizeof(showscorebox));
//		readHandle(file,&compatability,sizeof(compatability));
//		readHandle(file,&QuietFX,sizeof(QuietFX));
//		readHandle(file,&hadAdLib,sizeof(hadAdLib));
//		readHandle(file,&jerk,sizeof(jerk));
//#ifdef KEEN
//		readHandle(file,&oldshooting,sizeof(oldshooting));
//		readHandle(file,&firescan,sizeof(firescan));
//#endif

		HighScoresDirty = false;
		gotit = true;
	}
	else
	{
rcfailed:
		sd = sdm_AdLib;
		sm = sdm_AdLib;
		pal = 0;
		sbox = 2;

		gotit = false;
		HighScoresDirty = true;
	}

	SD_SetMusicMode(sm);
	SD_SetSoundMode(sd);
	CK_PaletteSet = pal;
	showscorebox = sbox;

}

///////////////////////////////////////////////////////////////////////////
//
//      USL_WriteConfig() - Writes out the current configuration, including the
//              high scores.
//
///////////////////////////////////////////////////////////////////////////
static void
USL_WriteConfig(void)
{
	word    version;
	FileHandle             file;

	version = ConfigVersion;
	file = openHandle(CKF_CONFIG, FileIO_Read | FileIO_Write, CKFB_CONFIG_S);
	if (file != -1)
	{
		writeHandle(&file,&version,sizeof(version));
		writeHandle(&file,Scores,sizeof(HighScore) * MaxScores);
		writeHandle(&file,&SoundMode,sizeof(SoundMode));
		writeHandle(&file,&MusicMode,sizeof(MusicMode));
		writeHandle(&file,&CK_PaletteSet,sizeof(CK_PaletteSet));
		writeHandle(&file,&showscorebox,sizeof(showscorebox));
		
//		write(file,&(Controls[0]),sizeof(Controls[0]));
//		write(file,&(KbdDefs[0]),sizeof(KbdDefs[0]));
//		write(file,&compatability,sizeof(compatability));
//		write(file,&QuietFX,sizeof(QuietFX));
//		write(file,&AdLibPresent,sizeof(AdLibPresent));
//		write(file,&jerk,sizeof(jerk));
//#ifdef KEEN
//		write(file,&oldshooting,sizeof(oldshooting));
//		write(file,&firescan,sizeof(firescan));
//#endif
//		write(file,&GravisGamepad,sizeof(GravisGamepad));
//		write(file,&GravisMap,sizeof(GravisMap));
	}
}

///////////////////////////////////////////////////////////////////////////
//
//      USL_CheckSavedGames() - Checks to see which saved games are present
//              & valid
//
///////////////////////////////////////////////////////////////////////////
#ifdef KEEN
void
#else
static void
#endif
USL_CheckSavedGames(void)
{
	boolean         ok;
	char            *filename;
	word            i;
	FileHandle		file;
	SaveGame        *game;

	for (i = 0,game = Games;i < MaxSaveGames;i++,game++)
	{
		ok = false;
		if ((file = openHandle(ck_file_slots[i], FileIO_Read, CKFB_SLOT_S)) != -1)
		{
			if( (readHandle(&file,game,sizeof(*game)) == sizeof(*game)) /*&&
				(game->oldtest == &PrintX)*/ )
				ok = true;
		}

		if (ok)
			game->present = true;
		else
		{
			game->present = false;
			strcpy(game->name,"Empty");
		}
	}
}


///////////////////////////////////////////////////////////////////////////
//
//      US_Setup() - Does the disk access part of the User Mgr's startup
//
///////////////////////////////////////////////////////////////////////////

void US_Setup(){
	WindowX = 0;
	WindowY = 0;
	USL_ReadConfig();               // Read config file
	USL_CheckSavedGames();  // Check which saved games are present
	// Hmmm
	scorescreenkludge = false;
};


///////////////////////////////////////////////////////////////////////////
//
//      US_Shutdown() - Shuts down the User Mgr
//
///////////////////////////////////////////////////////////////////////////
void US_Shutdown(void){
	USL_WriteConfig();
}


void USL_MeasureString(char *str, word *w, word *h){
	*h = 8;
	*w = 0;
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

char printbuffer[256];

void
US_Print(char *s)
{
	char    c,*se;
	word    w,h;

	char *sbuff = printbuffer;
	_ck_strcpy(sbuff, s);

	while (*sbuff)
	{
		se = sbuff;
		while ((c = *se) && (c != '\n'))
			se++;
		*se = '\0';

		USL_MeasureString(sbuff,&w,&h);
		USL_DrawString(sbuff);

		sbuff = se;
		if (c)
		{
			*se = c;
			sbuff++;

			PrintX = WindowX;
			PrintY += h;
		}
		else
			PrintX += w;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//      US_SafePrint() - Prints a string in a 30x20 window. Newlines are
//              supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_SafePrint(char *s)
{
	char    c,*se;
	word    w,h;
	char *sbuff = printbuffer;
	_ck_strcpy(sbuff, s);

	while (*sbuff)
	{
		se = sbuff;

		int chcount = PrintX/8;
		while ((c = *se) && (c != '\n') && chcount < 30){
			se++;
			chcount++;
		}
		*se = '\0';

		USL_MeasureString(sbuff,&w,&h);
		USL_DrawString(sbuff);

		sbuff = se;
		if (c)
		{
			*se = c;
			sbuff++;

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

int _ck_strcpy(char *dest, char *src){
	while(*src){
		*(dest++) = *(src++);
	}
	*dest = 0; // Add end byte
	return 0;
};

unsigned int _ck_strlen(char *str){
	unsigned int len = 0;
	while(*(str++)) len++;
	return len;	
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
{
	word    w,h,
			rw,rh;

	USL_MeasureString(s,&w,&h);
	rw = r.lr.x - r.ul.x;
	rh = r.lr.y - r.ul.y;

	PrintX = r.ul.x + ((rw - w) / 2);
	PrintY = r.ul.y + ((rh - h) / 2);
	USL_DrawString(s);
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
{
	word    w,h;

	USL_MeasureString(s,&w,&h);
	//if (w > WindowW) Quit(s);
//		Quit("US_CPrintLine() - String exceeds width");

	PrintX = WindowX + ((WindowW - w) / 2);
	USL_DrawString(s);
	PrintY += h;
}

///////////////////////////////////////////////////////////////////////////
//
//      US_CPrint() - Prints a string in the current window. Newlines are
//              supported.
//
///////////////////////////////////////////////////////////////////////////
void
US_CPrint(char *s)
{
	char    c,*se;
	char *sbuff = printbuffer;
	_ck_strcpy(sbuff, s);

	while (*sbuff)
	{
		se = sbuff;
		while ((c = *se) && (c != '\n'))
			se++;
		*se = '\0';

		US_CPrintLine(sbuff);

		sbuff = se;
		if (c)
		{
			*se = c;
			sbuff++;
		}
	}
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
//	VW_FixNeededTiles();
	VWB_Bar(WindowX,WindowY,WindowW,WindowH,WHITE); // spawn white tiles
	PrintX = WindowX;
	PrintY = WindowY;
}

///////////////////////////////////////////////////////////////////////////
//
//      US_DrawWindow() - Draws a frame and sets the current window parms
//
///////////////////////////////////////////////////////////////////////////
void
US_DrawWindow(word x,word y,word w,word h)
{
	word    i,
			sx,sy,sw,sh;

	// Hide all the sprites
	GBA_HideSprites();
	// Fix the scroll
	CK_SnapScroll();

	WindowX = x * 8;
	WindowY = y * 8;
	WindowW = w * 8;
	WindowH = h * 8;

	WindowX += CK_CameraX;
	WindowY += CK_CameraY;

	sx = (x - 1) * 8;
	sy = (y - 1) * 8;
	sw = (w + 1) * 8;
	sh = (h + 1) * 8;

	sx += CK_CameraX;
	sy += CK_CameraY;

	US_ClearWindow();

	VWB_DrawTile8M(sx,sy,0),VWB_DrawTile8M(sx,sy + sh,6);
	for (i = sx + 8;i <= sx + sw - 8;i += 8)
		VWB_DrawTile8M(i,sy,1),VWB_DrawTile8M(i,sy + sh,7);
	VWB_DrawTile8M(i,sy,2),VWB_DrawTile8M(i,sy + sh,8);

	for (i = sy + 8;i <= sy + sh - 8;i += 8)
		VWB_DrawTile8M(sx,i,3),VWB_DrawTile8M(sx + sw,i,5);

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
	US_DrawWindow(((MaxX / 8) - w) / 2,((MaxY / 8) - h) / 2,w,h);
}




// From ID_RF.C
unsigned	tics = 1;
long		lasttimecount = 0;

// From ID_IN.C
int LastScan;
int CurScan;

// From Game.c
extern bool fadeinhook;
extern Uint16 fadecount;
// From 
extern bool screenchanged;
// Sound
bool irqcount = 0;

extern unsigned char *PC_SoundPtr;
extern unsigned int PC_SoundLen;
extern unsigned int PC_SoundCount;

void GBA_UserIRQ(){
	if ((*(volatile uint16_t*)GBA_INT_STATE & GBA_INT_TIMER2) == GBA_INT_TIMER2) {
		if(PC_SoundPtr){
			if(PC_SoundLen){
				unsigned short fq = 0;
				unsigned int f = PC_SoundPtr[PC_SoundCount]*60;
				if(f > 0){
					unsigned short fd = f/9;
					if(fd > 2048){
						fd = 2048;
					}
					if(fd<1){
						fd = 1;
					}
					fq = 2048-fd;
					
					//enable sound 1 to left and right
					*(volatile uint16_t*)GBA_SOUNDCNT_L |= GBA_SND_1L | GBA_SND_1R ;

					// set the frequency
					*(volatile uint16_t*)GBA_SOUND1_X = fq | GBA_RESET_SOUND;
				}else{
					//disable sound 1 to left and right
					*(volatile uint16_t*)GBA_SOUNDCNT_L &= ~(GBA_SND_1L | GBA_SND_1R);
				}

				PC_SoundCount ++;
				if(PC_SoundCount > PC_SoundLen){
					// done
					PC_SoundCount = 0;
					PC_SoundLen = 0;
					PC_SoundPtr = NULL;
					//disable sound 1 to left and right
					*(volatile uint16_t*)GBA_SOUNDCNT_L &= ~(GBA_SND_1L | GBA_SND_1R);
				}
			}
		}
		if((++irqcount)&0x1){
			TimeCount += 1;
		}

		// Makes the user have to release the button???
		if(CurScan != GBA_INV_BUTTONS)
			LastScan = CurScan;
		CurScan = GBA_INV_BUTTONS;

		if(screenchanged){
			if(fadeinhook){
				if(++fadecount == 2){
					VW_FadeIn(); // Might cause problems?? DMA
					fadeinhook = false;
				}
			}
			screenchanged = false;
		}

	}

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

