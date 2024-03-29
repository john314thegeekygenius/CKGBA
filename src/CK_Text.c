/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

/* 
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


// CK Font stuffs
extern const unsigned char CK_FONT[];
extern const unsigned int CK_FONT_size;

#define CK_TXTCOL(c) ( c | c<<4 | c<<8 | c<<12 | c<<16 | c<<20 | c<<24)

uint16_t CK_PrintY = 0;
uint16_t CK_PrintX = 0;
uint32_t CK_PrintC = CK_TXTCOL(1);


volatile uint16_t *HELPMAP_0 = (volatile uint16_t*)GBA_SCREEN_BLOCK(30);
volatile uint16_t *HELPMAP_1 = (volatile uint16_t*)GBA_SCREEN_BLOCK(31);

#define HELPMAP_BLOCK_0 0x00
#define HELPMAP_BLOCK_1 0x08

#define HELPMAP_MAP_0 0x1E00
#define HELPMAP_MAP_1 0x1F00


void CK_BlitCharHelp(char c, uint16_t x, uint16_t y, uint32_t col){
	uint32_t *vidmem = (uint32_t*)GBA_VRAM2+(y<<8)+(x<<3);
	for(int i = 0; i < 8; i++){
		*vidmem = *((uint32_t*)CK_FONT+i+(c*8)) & col;//(27*16);//(c*8*4)+(y*8);
		vidmem++;
	}
};

void CK_Print(char*pstr){
	while(*pstr!='\0'){
		CK_BlitCharHelp(*pstr,CK_PrintX,CK_PrintY,CK_PrintC);
		++CK_PrintX;
		++pstr;
	}
};


#include "romstuffs/TXT/CK_TxtDefs.h"

//
extern const unsigned char CK_HELP[];
extern const unsigned int CK_HELP_size;

#ifndef CK6
extern const unsigned char CK_BMP0006[];
extern const unsigned int CK_BMP0006_width;
extern const unsigned int CK_BMP0006_height;
extern const unsigned int CK_BMP0006_size;

#ifdef CK4
extern const unsigned char CK_BMP0045[];
extern const unsigned int CK_BMP0045_size;
extern const unsigned int CK_BMP0045_width;
extern const unsigned int CK_BMP0045_height;
#elif defined CK5
extern const unsigned char CK_BMP0024[];
extern const unsigned int CK_BMP0024_size;
extern const unsigned int CK_BMP0024_width;
extern const unsigned int CK_BMP0024_height;
#endif
#endif

///////////////////////////////////////////////
///////////////////////////////////////////////

extern const unsigned int CK_TXT0000_pages;
extern const unsigned char CK_TXT0000_text[];
extern const unsigned int CK_TXT0000_color[];
extern const unsigned int CK_TXT0000_graphics[];
// There's no help in Keen 6
#ifndef CK6
extern const unsigned int CK_TXT0001_pages;
extern const unsigned char CK_TXT0001_text[];
extern const unsigned int CK_TXT0001_color[];
extern const unsigned int CK_TXT0001_graphics[];

extern const unsigned int CK_TXT0002_pages;
extern const unsigned char CK_TXT0002_text[];
extern const unsigned int CK_TXT0002_color[];
extern const unsigned int CK_TXT0002_graphics[];

extern const unsigned int CK_TXT0003_pages;
extern const unsigned char CK_TXT0003_text[];
extern const unsigned int CK_TXT0003_color[];
extern const unsigned int CK_TXT0003_graphics[];

extern const unsigned int CK_TXT0004_pages;
extern const unsigned char CK_TXT0004_text[];
extern const unsigned int CK_TXT0004_color[];
extern const unsigned int CK_TXT0004_graphics[];
#ifdef CK5
extern const unsigned int CK_TXT0005_pages;
extern const unsigned char CK_TXT0005_text[];
extern const unsigned int CK_TXT0005_color[];
extern const unsigned int CK_TXT0005_graphics[];
#endif
#endif

const unsigned char * CK_TXTHELP_text[] = {
#ifndef CK6
	// Help Texts
#ifdef CK4
	CK_TXT0000_text,
	CK_TXT0002_text,
	CK_TXT0001_text,
	CK_TXT0003_text,
#elif defined CK5
	CK_TXT0000_text,
	CK_TXT0001_text,
	CK_TXT0002_text,
	CK_TXT0003_text,
#endif
	// Special texts
	CK_TXT0004_text,
#ifdef CK5
	CK_TXT0005_text,
#endif
#else
	// End Text
	CK_TXT0000_text,
#endif
};

const unsigned int * CK_TXTHELP_color[] = {
#ifndef CK6
	// Help Texts
#ifdef CK4
	CK_TXT0000_color,
	CK_TXT0002_color,
	CK_TXT0001_color,
	CK_TXT0003_color,
#elif defined CK5
	CK_TXT0000_color,
	CK_TXT0001_color,
	CK_TXT0002_color,
	CK_TXT0003_color,
#endif
	// Special texts
	CK_TXT0004_color,
#ifdef CK5
	CK_TXT0005_color,
#endif
#else
	// End Text
	CK_TXT0000_color,
#endif
};

const unsigned int * CK_TXTHELP_graphics[] = {
#ifndef CK6
	// Help Texts
#ifdef CK4
	CK_TXT0000_graphics,
	CK_TXT0002_graphics,
	CK_TXT0001_graphics,
	CK_TXT0003_graphics,
#elif defined CK5
	CK_TXT0000_graphics,
	CK_TXT0001_graphics,
	CK_TXT0002_graphics,
	CK_TXT0003_graphics,
#endif
	// Special texts
	CK_TXT0004_graphics,
#ifdef CK5
	CK_TXT0005_graphics,
#endif
#else
	// End Text
	CK_TXT0000_graphics,
#endif
};

const unsigned int * CK_TXTHELP_pages[] = {
#ifndef CK6
	// Help Texts
#ifdef CK4
	&CK_TXT0000_pages,
	&CK_TXT0002_pages,
	&CK_TXT0001_pages,
	&CK_TXT0003_pages,
#elif defined CK5
	&CK_TXT0000_pages,
	&CK_TXT0001_pages,
	&CK_TXT0002_pages,
	&CK_TXT0003_pages,
#endif
	// Special texts
	&CK_TXT0004_pages,
#ifdef CK5
	&CK_TXT0005_pages,
#endif
#else
	// End Text
	&CK_TXT0000_pages,
#endif
};

signed short CK_HelpLY = 0;
signed short CK_HelpY = 0;
int CK_MenuOn = 0;
int CK_PageOn = 0;

bool CK_UpdateHelpCursor = false;
// TODO:
// Move to a CK_CheckLast( BUTTON ) function
uint32_t lastButton = 0;

void CK_DrawHelpBorder(){
	// Set the map up
	for(int i = 0; i < 32*32; i++){
		*(uint16_t*)(HELPMAP_0+i) = 0;
		*(uint16_t*)(HELPMAP_1+i) = i;
	}

	// Remove all foreground elements
	uint32_t *vram = (uint32_t*)GBA_VRAM2;

	// Copy help stuff
	GBA_DMA_MemSet32(vram, (uint32_t*)0, 32*20*8);	
#ifndef KEEN6
	for(int i = 0; i < 30; i++){
		*(uint16_t*)(HELPMAP_0+i) = 0x5; // Top
		*(uint16_t*)(HELPMAP_0+i+(32*19)) = 0xB; // Bottom
	}
	for(int i = 0; i < 20; i++){
		*(uint16_t*)(HELPMAP_0+(i*32)) = 0x7; // Left
		*(uint16_t*)(HELPMAP_0+(i*32)+(29)) = 0x9; // Right
	}
	// Draw the corners
	*(uint16_t*)(HELPMAP_0) = 0x4;
	*(uint16_t*)(HELPMAP_0+29) = 0x6;
	*(uint16_t*)(HELPMAP_0+(32*19)) = 0x8;
	*(uint16_t*)(HELPMAP_0+(32*19)+29) = 0xA;
#endif
};

#ifndef CK6

void CK_DrawHelpMenu(){

	// Draw the border
	CK_DrawHelpBorder();
	
	// Draw the menu
	for(int e = 0; e < CK_BMP0006_height>>3; e++){
		for(int i = 0; i < CK_BMP0006_width>>2; i++){
			*(uint16_t*)(HELPMAP_0+i+7+(e<<5)) = 0x6C+i+(e*(CK_BMP0006_width>>2));
		}
	}
	// Draw help controls
	for(int i = 0; i < 26; i++){
		*(uint16_t*)(HELPMAP_0+i+(18*32)+2) = 0x30+i;
		*(uint16_t*)(HELPMAP_0+i+(19*32)+2) = 0x30+i+26;
	}

	CK_UpdateHelpCursor = true;
};

void CK_SetupHelp(){
	uint32_t *vram = (uint32_t*)GBA_VRAM;

	// Copy help stuff
	GBA_DMA_Copy32(vram, (uint32_t*)CK_HELP, CK_HELP_size>>2);
	vram += (CK_HELP_size>>2);
	GBA_DMA_Copy32(vram, (uint32_t*)CK_BMP0006, CK_BMP0006_size>>2);
	vram += (CK_BMP0006_size>>2);
#ifdef CK5
	GBA_DMA_Copy32(vram, (uint32_t*)CK_BMP0024, CK_BMP0024_size>>2);
	vram += (CK_BMP0024_size>>2);
#else
	GBA_DMA_Copy32(vram, (uint32_t*)CK_BMP0045, CK_BMP0045_size>>2);
	vram += (CK_BMP0045_size>>2);
#endif
	GBA_DMA_Copy32(vram, (uint32_t*)CK_FONT, CK_FONT_size>>2);

	CK_MenuOn = 0;
	CK_PageOn = 0;

	CK_HelpLY = 0;
	CK_HelpY = 0;

	CK_UpdateHelpCursor = true;
	lastButton = 0;


	CK_DrawHelpMenu();

};
#endif

int CK_PrintPage(unsigned int menuid, unsigned int pageid, bool isFinale){
	// Determine the text info
	const unsigned char *CK_PageText = CK_TXTHELP_text[menuid-1]+(pageid*CK_TXT_SIZE);
	const unsigned int *CK_PageColor = CK_TXTHELP_color[menuid-1]+(pageid*CK_TXT_SIZE);
	const unsigned int *CK_PageGraphic = CK_TXTHELP_graphics[menuid-1];
	int longestTime = 0;

	// Offset to the correct graphic page
	for(int i = 0; i < pageid; i++){
		int gfxcnt = *CK_PageGraphic;
		CK_PageGraphic++;
		CK_PageGraphic += gfxcnt*4;
	}
#ifndef CK6
	if(!isFinale){
		// Draw the border
		CK_DrawHelpBorder();
		// Add the special bottom part
		for(int i = 0; i < 12; i++){
			*(uint16_t*)(TILEMAP_0+i+(17*32)+2) = 0xC+i;
			*(uint16_t*)(TILEMAP_0+i+(18*32)+2) = 0x18+i;
			*(uint16_t*)(TILEMAP_0+i+(19*32)+2) = 0x24+i;

			// Other one
			*(uint16_t*)(TILEMAP_0+i+(17*32)+16) = 0xC+i;
														// Other one is blank
			*(uint16_t*)(TILEMAP_0+i+(19*32)+16) = 0x24+i;
		}
		*(uint16_t*)(TILEMAP_0+(18*32)+16) = 0x18;
		*(uint16_t*)(TILEMAP_0+(18*32)+27) = 0x18+11;
	}
#endif
	for(int e = 0; e < CK_TXT_HEIGHT; e++){
		for(int i = 0; i < CK_TXT_WIDTH; i++){
			CK_BlitCharHelp(CK_PageText[(e*28)+i], i+1, e+1, CK_PageColor[(e*28)+i]);
		}
	}

	int gfxcnt = *CK_PageGraphic;CK_PageGraphic++;
	for(int i = 0; i < gfxcnt; i++){
		int gx = *(CK_PageGraphic); CK_PageGraphic++;
		int gy = *(CK_PageGraphic); CK_PageGraphic++;
		int gid = *(CK_PageGraphic); CK_PageGraphic++;
		int gtime = *(CK_PageGraphic); CK_PageGraphic++;
		// Wait before drawing the picture
		if(gtime){
			longestTime ++;
			GBA_Delay(gtime*30);
		}
		VWB_DrawPicStory(gx,gy,gid-CK_BITMAP_START);
	}

	if(!isFinale){
		// Write the page info
		CK_PrintX = 0x11;
		CK_PrintY = 0x12;
		CK_PrintC = CK_TXTCOL(CK_EGA_YELLOW);
		char printBuffer[11] = "p  x of  x";

		// Replace the numbers
		pageid++; // Increase by one
		if(pageid >= 10)
			printBuffer[2] = '0'+(pageid/10);
		printBuffer[3] = '0'+(pageid%10);
		if(*CK_TXTHELP_pages[menuid-1] >= 10)
			printBuffer[8] = '0'+(*CK_TXTHELP_pages[menuid-1]/10);
		printBuffer[9] = '0'+(*CK_TXTHELP_pages[menuid-1]%10);

		CK_Print(printBuffer);
	}
	return longestTime;
};
#ifndef CK6

int CK_RunHelp(){
	if(CK_MenuOn == 0){

		if(CK_UpdateHelpCursor){
			CK_UpdateHelpCursor = false;

			// Draw the pointer
#ifdef CK5
			for(int i = 0; i < CK_BMP0024_width>>2; i++){
#else
			for(int i = 0; i < CK_BMP0045_width>>2; i++){
#endif
				*(uint16_t*)(TILEMAP_0+i+(((CK_HelpLY*3)+5)*32)+2) = 0; // Clear the last one
				*(uint16_t*)(TILEMAP_0+i+(((CK_HelpLY*3)+6)*32)+2) = 0; // Clear the last one
				*(uint16_t*)(TILEMAP_0+i+(((CK_HelpLY*3)+7)*32)+2) = 0; // Clear the last one
				
				*(uint16_t*)(TILEMAP_0+i+(((CK_HelpY*3)+5)*32)+2) = 0x19E + i; // Draw the new one
				*(uint16_t*)(TILEMAP_0+i+(((CK_HelpY*3)+6)*32)+2) = 0x1A2 + i; // Draw the new one
				*(uint16_t*)(TILEMAP_0+i+(((CK_HelpY*3)+7)*32)+2) = 0x1A6 + i; // Draw the new one
			}
		}

		if(GBA_TEST_BUTTONS(GBA_BUTTON_UP)){
			lastButton = GBA_BUTTON_UP;
		}else{
			if(lastButton == GBA_BUTTON_UP){
				CK_HelpLY = CK_HelpY;
				CK_HelpY -= 1;
				if(CK_HelpY < 0) CK_HelpY = 0;
				CK_UpdateHelpCursor = true;
				lastButton = 0;
			}
		}

		if(GBA_TEST_BUTTONS(GBA_BUTTON_DOWN)){
			lastButton = GBA_BUTTON_DOWN;
		}else{
			if(lastButton == GBA_BUTTON_DOWN){
				CK_HelpLY = CK_HelpY;
				CK_HelpY += 1;
				if(CK_HelpY > 3) CK_HelpY = 3;
				CK_UpdateHelpCursor = true;
				lastButton = 0;
			}
		}


		if(GBA_TEST_BUTTONS(GBA_BUTTON_A)){
			lastButton = GBA_BUTTON_A;
		}else{
			if(lastButton == GBA_BUTTON_A){
				CK_PageOn = 0;
				CK_MenuOn = CK_HelpY+1;
				CK_UpdateHelpCursor = true;
				lastButton = 0;
				// Redraw the screen
				CK_PrintPage(CK_MenuOn, CK_PageOn, false);
			}
		}
		if(GBA_TEST_BUTTONS(GBA_BUTTON_B)){
			lastButton = GBA_BUTTON_B;
		}else{
			if(lastButton == GBA_BUTTON_B){
				lastButton = 0;
				return 1;
			}
		}
	}else{

		if(GBA_TEST_BUTTONS(GBA_BUTTON_B)){
			lastButton = GBA_BUTTON_B;
		}else{
			if(lastButton == GBA_BUTTON_B){
				CK_PageOn = 0;
				CK_HelpY = CK_MenuOn-1;
				CK_MenuOn = 0;
				CK_DrawHelpMenu();
				lastButton = 0;
			}
		}


		if(GBA_TEST_BUTTONS(GBA_BUTTON_LEFT)){
			lastButton = GBA_BUTTON_LEFT;
		}else{
			if(lastButton == GBA_BUTTON_LEFT){
				if(CK_PageOn > 0){
					CK_PageOn--;
					// Redraw the screen
					CK_PrintPage(CK_MenuOn, CK_PageOn, false);
				}
				lastButton = 0;
			}
		}

		if(GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT)){
			lastButton = GBA_BUTTON_RIGHT;
		}else{
			if(lastButton == GBA_BUTTON_RIGHT){
				if(CK_PageOn < *CK_TXTHELP_pages[CK_MenuOn-1]-1){
					CK_PageOn++;
					// Redraw the screen
					CK_PrintPage(CK_MenuOn, CK_PageOn, false);
				}
				lastButton = 0;
			}
		}
	}

	return 0;
};

/*
=================
=
= HelpScreens
=
=================
*/

void HelpScreens(void)
{
	SD_MusicOff();

	RF_StoreOrg();

#ifdef CK5
	StartMusic(HELPMUSIC);
#endif

	// Fix the GBA backgrounds
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | HELPMAP_MAP_0 | HELPMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | HELPMAP_MAP_1 | HELPMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
	// Remove the second background
	*(volatile unsigned int*)GBA_REG_DISPCNT &= ~GBA_ENABLE_BG2;

	CK_SetupHelp();
	// Fix the scroll offsets

	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;

	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;

	*(volatile uint16_t*)GBA_REG_BG2HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG2VOFS = 0;
	// Hide all the sprites
	GBA_HideSprites();

	while(!CK_RunHelp()){};
	// Fix the graphics
	CA_FixGraphics();

	RF_RestoreOrg();
	RF_RestoreOffs(); // TODO: Hmmm
};
#endif

// MODDERS:
int FinaleTxt = 5; // Indexed from 1

void CK_SetupFinale(){
	uint32_t *vram = (uint32_t*)GBA_VRAM;

	// Copy help stuff
	GBA_DMA_Copy32(vram, (uint32_t*)CK_HELP, CK_HELP_size>>2);

#ifdef KEEN5
	if (gamestate.leveldone[13] == ex_fusebroke){
		// MODDERS:
		FinaleTxt = 6;
	}
#endif
#ifdef KEEN6
	FinaleTxt = 1;
#endif
	CK_PageOn = 0;
	lastButton = 0;
};

int CK_RunFinale(){
	// MODDERS: Can get rid of this:
	// Draw a green arrow
#ifdef KEEN6
	if(GBA_INV_BUTTONS){
		VWB_DrawPicStory(28,18,H_FLASHARROW2PIC);
	}else{
		VWB_DrawPicStory(28,18,H_FLASHARROW1PIC);
	}
#else
	if(GBA_INV_BUTTONS){
		VWB_DrawPicStory(27,18,H_FLASHARROW2PIC);
	}else{
		VWB_DrawPicStory(27,18,H_FLASHARROW1PIC);
	}
#endif
	if(GBA_INV_BUTTONS){
		lastButton = GBA_INV_BUTTONS;
	}else{
		if(lastButton && !(lastButton&GBA_INV_BUTTONS)){
			if(CK_PageOn < *CK_TXTHELP_pages[FinaleTxt-1]-1){
				CK_PageOn++;
				CK_PrintPage(FinaleTxt, CK_PageOn, true);
			}else{
				return 1; // We are done
			}
			lastButton = 0;
		}
	}

	return 0;
};

void FinaleLayout(void){
	SD_MusicOff();

	RF_StoreOrg();

	// Fix the GBA backgrounds
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | HELPMAP_MAP_0 | HELPMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | HELPMAP_MAP_1 | HELPMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
	// Remove the second background
	*(volatile unsigned int*)GBA_REG_DISPCNT &= ~GBA_ENABLE_BG2;

	StartMusic(ENDINGMUSIC);

    CK_SetupFinale();

	IN_ClearKeysDown();

	// Fix the scroll offsets

	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;

	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;

	*(volatile uint16_t*)GBA_REG_BG2HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG2VOFS = 0;
	// Hide all the sprites
	GBA_HideSprites();

	CK_DrawHelpBorder();

	// Draw the first page
	CK_PrintPage(FinaleTxt, CK_PageOn, true);
	while(!CK_RunFinale()){};
	// Fix the graphics
	CA_FixGraphics();

	RF_RestoreOrg();
	RF_RestoreOffs(); // TODO: Hmmm

	SD_MusicOff();

};
