/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


boolean scorescreenkludge;

const unsigned char *CK_DemoPtrs[] = {
	CK_DEMO0_data,
	CK_DEMO1_data,
	CK_DEMO2_data,
	CK_DEMO3_data,
	CK_DEMO4_data,
};

/*
=============================================================================

							TERMINATOR INTRO

=============================================================================
*/

GBA_IN_EWRAM unsigned char CK_BurnBitHandler[160]; // Store 160 bytes for the burn filter

const unsigned int CK_TERMPICOFF[] = {0x58, 0x48 + (1<<8), 0x48+ (2<<8), 0x48 + (3<<8)};

const unsigned int *CK_INT_PICS[] = { 
	&CK_INT0_width, &CK_INT0_height, (uint32_t*)CK_INT0,
	&CK_INT1_width, &CK_INT1_height, (uint32_t*)CK_INT1,
	&CK_INT2_width, &CK_INT2_height, (uint32_t*)CK_INT2,
	&CK_INT3_width, &CK_INT3_height, (uint32_t*)CK_INT3,
};



/*
============================
=
= CheckLastScan
=
============================
*/

void CheckLastScan(void)
{
	if (LastScan)
	{
#ifndef KEEN6
		if (LastScan == (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER))
		{
			HelpScreens();
		}
#endif
		US_ControlPanel();
		if (restartgame)
		{
			playstate = ex_resetgame;
		}
		else if (loadedgame)
		{
			playstate = ex_loadedgame;
		}
	}
}

/*
=============================================================================

							TERMINATOR INTRO

=============================================================================
*/


#define TERMMAP_BLOCK 0x08

/*
============================
=
= SlideLetters
=
============================
*/
void SlideLetters(){

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Run the TERMINATOR intro
	////////////////////////////////////////////////////////////////////////////

	uint32_t * gbaVRam ;
	uint32_t * terminatorRam1;
	uint32_t * terminatorRam2;
	uint32_t blitoffset = 0;
	uint32_t readx = 0;
	int terminatorX = 0;
	int CK_TextCount = 0;
	int CK_TextShowCount = 0;
	int CK_TermPic = 0;
	int CK_TextScroll = 16*8;
	char CK_DrawSplash = 1;
	#define CK_TXTMINTICS 16
	#define CK_TXTWAITTICS 24
	#define CK_TXTMAXTICS (CK_TXTWAITTICS+CK_TXTMINTICS)

	const int termwidth = ((CK_TERM_0_width>>2))+34;

	// Clear the second layer
	GBA_DMA_MemSet32((uint32_t*)GBA_VRAM+0x2000, 0x0, 32*20*8);
	// Change the palette so it looks nicer

#ifdef CK4
	*((uint16_t*)GBA_PAL_BG_START+7) = COMMANDER_KEEN_PALETTE[8]; // Dark grey
	*((uint16_t*)GBA_PAL_BG_START+15) = COMMANDER_KEEN_PALETTE[7]; // Light grey

	*((uint16_t*)GBA_PAL_BG_START+3) = COMMANDER_KEEN_PALETTE[9]; // Blue
	*((uint16_t*)GBA_PAL_BG_START+4) = COMMANDER_KEEN_PALETTE[11]; // Light blue
#endif
	int getx, getx2;

	// Copy the text blocks over
	while(terminatorX < termwidth ){
		readx = terminatorX;
		for(int blity = 0; blity < 20; blity++){
			for(int blitx = 0; blitx < 30; blitx++){
				getx = (160 - (CK_TERM_0_width>>2) + (terminatorX-blitx));//(30 + (CK_TERM_0_width>>2) - (terminatorX+blitx));
				getx2 = (30 + (CK_TERM_1_width>>2) - ((terminatorX>>1)+blitx));//(60 - (CK_TERM_1_width>>2) + ((terminatorX>>1)-blitx));
				gbaVRam = (uint32_t*) GBA_VRAM + (blity<<8) + ((29-blitx)<<3);
				terminatorRam1 = (uint32_t*) CK_TERM_0 + (blity*(CK_TERM_0_width<<1)) + (getx<<3);
				terminatorRam2 = (uint32_t*) CK_TERM_1 + (blity*(CK_TERM_1_width<<1)) + (getx2<<3);
				for(int blitc = 0; blitc < 8; blitc++){
					uint32_t blitc = 0;
					*gbaVRam = 0x11111111;
					if(getx < (CK_TERM_0_width>>2) && getx > 0) {
						blitc += *terminatorRam1;
						++terminatorRam1;
					}
					if(getx2 < (CK_TERM_1_width>>2) && getx2 > 0) {
						blitc += *terminatorRam2;
						++terminatorRam2;
					}
					*gbaVRam |= blitc;
					++gbaVRam;
				}
			}
		}
		++terminatorX;


		
		// Draw the popup texts
		if(CK_TermPic < 4){
			if(CK_DrawSplash){	
				// Clear the second layer
				GBA_DMA_MemSet32((uint32_t*) GBA_VRAM+0x2000, 0, 32*20*8);
				// Draw the picture
				uint32_t *intwidth = CK_INT_PICS[(CK_TermPic*3)];
				gbaVRam = (uint32_t*) GBA_VRAM+0x2000 + CK_TERMPICOFF[CK_TermPic] + (5<<8);
				for(int i = 0; i < (*CK_INT_PICS[(CK_TermPic*3)+1])>>3; i++){
					GBA_DMA_Copy32(gbaVRam+(i<<8), CK_INT_PICS[(CK_TermPic*3)+2]+(i*((*intwidth)<<1)), (*intwidth)<<1 );
				}
			}
			CK_DrawSplash = 0;
			++CK_TextShowCount;

			uint32_t INTTICS = (*CK_INT_PICS[(CK_TermPic*3)+1])>>3;

			if(CK_TextShowCount > CK_TXTMINTICS && CK_TextShowCount < CK_TXTWAITTICS){
			} else {
				CK_TextScroll += 8;
			}
			if(CK_TextShowCount > CK_TXTMAXTICS){
				CK_TextShowCount = 0;
				CK_TextCount = 0;
				CK_TermPic++;
				CK_DrawSplash = 1;
				CK_TextScroll = 16*8;
				CK_TextShowCount = 0;
			}

			*(volatile uint16_t*)GBA_REG_BG1VOFS = (uint16_t)CK_TextScroll&0x1FF;
		}

		GBA_WAIT_VBLANK
		GBA_Delay(150);

		//
		// handle input
		//
		if (IN_IsUserInput() && LastScan != (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER))
		{
			LastScan = GBA_BUTTON_B;
		}
		if (LastScan){
			 *(volatile uint16_t*)GBA_REG_BG1VOFS = 0;
			return;
		}
	}
	 *(volatile uint16_t*)GBA_REG_BG1VOFS = 0;
};

/*
============================
=
= FinishPage
=
============================
*/
void FinishPage(){
	uint32_t * gbaVRam ;

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Burn the screen away
	////////////////////////////////////////////////////////////////////////////

	// Remove the palette so it doesn't flash
	GBA_DMA_MemSet16(GBA_PAL_BG_START,0,256);

	// Copy the title screen over 
	GBA_DMA_Copy32((uint32_t*)GBA_VRAM, (uint32_t*)CK_TITLESCREEN, (32*20*8));

	// Clear the second layer
	GBA_DMA_MemSet32((uint32_t*)GBA_VRAM+0x2000, 0x11111111, 32*20*8);

	// Move the screen over a bit
	*(volatile uint32_t*)GBA_REG_BG0HOFS = 8;

	// Fix the palette
	CK_FixPalette();

	// Clear the bit filter
	for(int i = 0; i < 160; i++){
		CK_BurnBitHandler[i] = 0;
	}

	uint32_t * CK_BitMaskPtr = (uint32_t*)CK_BURNBITS;
	uint16_t skiplines = 0;

	for(int i = 0; i <= 160+(CK_BURNBITS_LINEHEIGHT<<2); i++){
		for(int line = skiplines; line < i; line++){
			if(line >= 160) break;
			// If offsetY > 160 continue
			// Get pointer to correct line
			gbaVRam = (uint32_t*) GBA_VRAM + 0x2000 + (line%8) + ((line>>3)<<8);
			CK_BitMaskPtr = (uint32_t*)CK_BURNBITS + ((CK_BurnBitHandler[line]>>2)*32) + ((line%40)*CK_BURNBITS_LINEHEIGHT*32);

			++CK_BurnBitHandler[line];
			if(CK_BurnBitHandler[line] == (CK_BURNBITS_LINEHEIGHT<<2)) {
				skiplines ++;
			}
			for(int e = 0; e < 32; e++){
				*gbaVRam = *CK_BitMaskPtr;
				CK_BitMaskPtr++;
				gbaVRam+=8;
			}
		}
		GBA_Delay(25);// so the fizzle animation won't go super fast

		if (IN_IsUserInput() && LastScan != (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER))
		{
			LastScan = GBA_BUTTON_B;
		}
		if (LastScan) {
			// Clear any remaining bits
			GBA_DMA_MemSet32((uint32_t*)GBA_VRAM+0x2000, 0x00, 32*20*8);
			return;
		}
	}
	// Clear any remaining bits
	GBA_DMA_MemSet32((uint32_t*)GBA_VRAM+0x2000, 0x00, 32*20*8);

	//
	// pause for 6 seconds
	//
	IN_UserInput(6 * TickBase, false);

};

/*
============================
=
= Terminator
=
============================
*/

const unsigned int CK_TERM_PAL[] = TERMPALETTE;

void Terminator(void)
{
	boolean pagefinished;

	pagefinished = false;

	// Remove any old sprites (???)
	CK_RemoveSprites();

	VW_ClearVideo(BLACK);

	//
	// prepare (and set) the palettes
	//
	GBA_DMA_MemSet16(GBA_PAL_BG_START,0,256); // all black

	// Set the map to a constant 
	for(int i = 0; i < 32*20; i++){
		*(uint16_t*)(TILEMAP_0+i) = i;
		*(uint16_t*)(TILEMAP_1+i) = i;
	}
	for(int i = 32*20; i < 32*32; i++){
		*(uint16_t*)(TILEMAP_0+i) = 0;
		*(uint16_t*)(TILEMAP_1+i) = 0;
	}
	
	
	// Finish the render of the background
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | TILEMAP_MAP_0 | TILEMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | TILEMAP_MAP_1 | TERMMAP_BLOCK | GBA_BG_SIZE_32x32);

	for(int i = 0; i < 16; i++)
		*((uint16_t*)GBA_PAL_BG_START+i) = COMMANDER_KEEN_PALETTE[CK_TERM_PAL[i]];

	//
	// play the animation
	//
	SlideLetters();

	//
	// do the fizzle fade animation
	// (if intro wasn't aborted)
	//

	if (!LastScan)
	{
		FinishPage();
		pagefinished = true;
	}

	//
	// switch back to default graphics settings
	//
	VW_ClearVideo(BLACK);

	CK_FixPalette();

	//
	// handle input and main menu stuff
	//
	if (LastScan == 0)
	{
	//	return;
	}
#ifndef KEEN6
	if (LastScan == (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER))
	{
		// TODO:
		// Add Help!
		//HelpScreens();
		return;
	}
#endif
	if (!pagefinished)
	{		
		// Remove the palette so it doesn't flash
		GBA_DMA_MemSet16(GBA_PAL_BG_START,0,256);

		// Set the map to a constant 
		for(int i = 0; i < 32*32; i++){
			*(uint16_t*)(TILEMAP_0+i) = i;
			*(uint16_t*)(TILEMAP_1+i) = i;
		}
		
		// Copy the title screen over 
		GBA_DMA_Copy32((uint32_t*)GBA_VRAM, (uint32_t*)CK_TITLESCREEN, (32*20*8));

		// Clear the second layer
		GBA_DMA_MemSet32((uint32_t*)GBA_VRAM+0x2000, 0, 32*20*8);

		// Move the screen over a bit
		*(volatile uint32_t*)GBA_REG_BG0HOFS = 8;
		*(volatile uint32_t*)GBA_REG_BG0VOFS = 0;

		CK_FixPalette();

		IN_Ack();
	}

	US_ControlPanel();
	if (restartgame)
	{
		playstate = ex_resetgame;
	}
	else if (loadedgame)
	{
		playstate = ex_loadedgame;
	}

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


extern const unsigned char STORYBG[];
extern const unsigned int STORYBG_size;

extern const unsigned char CK_STORYTXT[];
extern const unsigned int CK_STORYTXT_size;

#define SWMAP_BLOCK 0x08

// Constants
const uint16_t CK_StoryScrollSpeed = 8;
const uint32_t CK_StoryHeight = 400/8;


const uint32_t *CK_TXTVRAM = (uint32_t*)(GBA_VRAM+(0x8000));
const uint32_t *CK_STORYTXT32 = (uint32_t*)CK_STORYTXT;

/*
============================
=
= ScrollSWText
=
============================
*/

void ScrollSWText(void) {

	// Stuff
	uint16_t CK_StoryTextY = 0;
	uint16_t CK_StoryTextScroll = 0;
	uint16_t CK_StoryTicks = 42*TickBase;
	uint16_t CK_StoryTick = 0;
	uint16_t CK_StoryWait = 0;

	while(CK_StoryTick < CK_StoryTicks){

		GBA_WAIT_VBLANK
		if(CK_StoryTextY == 8){
			CK_StoryTextY = 1;
			++CK_StoryTextScroll;
			// Move any text down
			for(int i = 0; i < 20; ++i){			
				GBA_DMA_Copy32(CK_TXTVRAM+(i<<8), CK_TXTVRAM+((i+1)<<8), 256);
			}
			if(CK_StoryTextScroll < CK_StoryHeight){
				// Add new text
				GBA_DMA_Copy32(CK_TXTVRAM+(20<<8), CK_STORYTXT32+((CK_StoryTextScroll-1)*240), 256);
			}else{
				// Clear the screen
				GBA_DMA_MemSet16(CK_TXTVRAM+(20<<8), 0, 256);
			}
		}else{
			if(TimeCount > CK_StoryWait){
				CK_StoryWait = TimeCount + (4);
				++CK_StoryTextY;
			}
		}

		*(volatile uint32_t*)GBA_REG_BG1VOFS = CK_StoryTextY;

		if (IN_IsUserInput() && LastScan != (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER))
		{
			LastScan = GBA_BUTTON_B;
		}
		if (LastScan)
			break;

		// Update the tick
		CK_StoryTick += TimeCount - lasttimecount;
		lasttimecount = TimeCount;
	}
}


void StarWars(void)
{
	uint32_t *gfxbuffer;
	uint32_t *vram;
	uint32_t offset;

	// Remove any sprites
	CK_RemoveSprites();

	// Clear the screen to black
	GBA_DMA_MemSet32((uint32_t*)GBA_VRAM, 0x11111111, GBA_SCREEN_SIZE>>1);
	// Clear all text ram
	GBA_DMA_MemSet16(CK_TXTVRAM, 0x0, 32*24*16);

	// Set the map to a constant 
	for(int i = 0; i < 32*32; i++){
		*(uint16_t*)(TILEMAP_0+i) = i;
		*(uint16_t*)(TILEMAP_1+i) = i;
	}
	
	// Finish the render of the background
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | TILEMAP_MAP_0 | TILEMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | TILEMAP_MAP_1 | SWMAP_BLOCK | GBA_BG_SIZE_32x32);

	// Set the GBA scroll
	*(volatile uint32_t*)GBA_REG_BG0HOFS = 0;
	*(volatile uint32_t*)GBA_REG_BG0HOFS = 0;
	*(volatile uint32_t*)GBA_REG_BG1VOFS = 8;
	*(volatile uint32_t*)GBA_REG_BG1HOFS = 0;

	// Copy the story pic over slowly to simulate the original game
	for(int i = 0; i < 160; i++){
		offset = (i%8)+((i>>3)<<8);
		vram = (uint32_t*)GBA_VRAM+offset;//;
		gfxbuffer = (uint32_t*)STORYBG+offset;
		for(int e = 0; e < 32; e++){
			*vram = *gfxbuffer;
			vram += 8;
			gfxbuffer += 8;
		}
		GBA_Delay(10);
	}

	if (!LastScan)
	{
		StartMusic(STARWARSMUSIC);
		ScrollSWText();
		StopMusic();
	}

	VW_ClearVideo(BLACK);
	VW_SetDefaultColors();
	CheckLastScan();
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
{
	// Copy the title screen over 
	GBA_DMA_Copy32((uint32_t*)GBA_VRAM, (uint32_t*)CK_TITLESCREEN, (32*20*8));

	// Clear the second layer
	GBA_DMA_MemSet32((uint32_t*)GBA_VRAM+0x2000, 0x11111111, 32*20*8);

	// Move the screen over a bit
	*(volatile uint32_t*)GBA_REG_BG0HOFS = 8;
	*(volatile uint32_t*)GBA_REG_BG0VOFS = 0;

	IN_UserInput(6*TickBase, false);
	CheckLastScan();
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

	demodata = (Uint16 *)CK_DemoPtrs[num];
	gamestate.mapon = demodata[0];
	DemoSize = demodata[1];


    DemoBuffer = &CK_DemoPtrs[num];
	DemoBuffer += 4;
	IN_StartDemoPlayback(DemoBuffer, DemoSize);
	SetupGameLevel(true);
	if (scorescreenkludge)
	{
		DrawHighScores();
	}

	PlayLoop();
	IN_StopDemo();
	CheckLastScan();
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
{
	scorescreenkludge = true;
//	IN_ClearKeysDown();
	RunDemo(4);
	scorescreenkludge = false;
}

