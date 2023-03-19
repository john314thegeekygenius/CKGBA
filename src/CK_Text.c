/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


/*
=================
=
= HelpScreens
=
=================
*/
void HelpScreens(void)
{
	// TODO:
};


void FinaleLayout(void){
    // TODO:
};

/*

#include "romstuffs/TXT/CK_TxtDefs.h"

//
extern const unsigned char CK_HELP[];
extern const unsigned int CK_HELP_size;

extern const unsigned char CK_BMP0006[];
extern const unsigned int CK_BMP0006_width;
extern const unsigned int CK_BMP0006_height;
extern const unsigned int CK_BMP0006_size;

extern const unsigned char CK_BMP0045[];
extern const unsigned int CK_BMP0045_size;
extern const unsigned int CK_BMP0045_width;
extern const unsigned int CK_BMP0045_height;

///////////////////////////////////////////////
///////////////////////////////////////////////

extern const unsigned int CK_TXT0000_pages;
extern const unsigned char CK_TXT0000_text[];
extern const unsigned int CK_TXT0000_color[];

extern const unsigned int CK_TXT0001_pages;
extern const unsigned char CK_TXT0001_text[];
extern const unsigned int CK_TXT0001_color[];

extern const unsigned int CK_TXT0002_pages;
extern const unsigned char CK_TXT0002_text[];
extern const unsigned int CK_TXT0002_color[];

extern const unsigned int CK_TXT0003_pages;
extern const unsigned char CK_TXT0003_text[];
extern const unsigned int CK_TXT0003_color[];

const unsigned char * CK_TXTHELP_text[] = {
	CK_TXT0000_text,
	CK_TXT0001_text,
	CK_TXT0002_text,
	CK_TXT0003_text,
};
const unsigned int * CK_TXTHELP_color[] = {
	CK_TXT0000_color,
	CK_TXT0001_color,
	CK_TXT0002_color,
	CK_TXT0003_color,
};
const unsigned int * CK_TXTHELP_pages[] = {
	&CK_TXT0000_pages,
	&CK_TXT0001_pages,
	&CK_TXT0002_pages,
	&CK_TXT0003_pages,
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
		*(uint16_t*)(TILEMAP_0+i) = 0;
		*(uint16_t*)(TILEMAP_1+i) = i;
	}

	// Remove all foreground elements
	uint32_t *vram = (uint32_t*)GBA_VRAM2;

	// Copy help stuff
	GBA_DMA_MemSet32(vram, (uint32_t*)0, 32*20*8);	

	for(int i = 0; i < 30; i++){
		*(uint16_t*)(TILEMAP_0+i) = 0x5; // Top
		*(uint16_t*)(TILEMAP_0+i+(32*19)) = 0xB; // Bottom
	}
	for(int i = 0; i < 20; i++){
		*(uint16_t*)(TILEMAP_0+(i*32)) = 0x7; // Left
		*(uint16_t*)(TILEMAP_0+(i*32)+(29)) = 0x9; // Right
	}
	// Draw the corners
	*(uint16_t*)(TILEMAP_0) = 0x4;
	*(uint16_t*)(TILEMAP_0+29) = 0x6;
	*(uint16_t*)(TILEMAP_0+(32*19)) = 0x8;
	*(uint16_t*)(TILEMAP_0+(32*19)+29) = 0xA;
};

void CK_DrawHelpMenu(){

	// Draw the border
	CK_DrawHelpBorder();
	
	// Draw the menu
	for(int e = 0; e < CK_BMP0006_height>>3; e++){
		for(int i = 0; i < CK_BMP0006_width>>2; i++){
			*(uint16_t*)(TILEMAP_0+i+7+(e<<5)) = 0x6C+i+(e*(CK_BMP0006_width>>2));
		}
	}
	// Draw help controls
	for(int i = 0; i < 26; i++){
		*(uint16_t*)(TILEMAP_0+i+(18*32)+2) = 0x30+i;
		*(uint16_t*)(TILEMAP_0+i+(19*32)+2) = 0x30+i+26;
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
	GBA_DMA_Copy32(vram, (uint32_t*)CK_BMP0045, CK_BMP0045_size>>2);
	vram += (CK_BMP0045_size>>2);
	GBA_DMA_Copy32(vram, (uint32_t*)CK_FONT, CK_FONT_size>>2);
	
	CK_DrawHelpMenu();

	CK_MenuOn = 0;
	CK_PageOn = 0;
};

void CK_PrintPage(unsigned int menuid, unsigned int pageid){
	// Determine the text info
	const unsigned char *CK_PageText = CK_TXTHELP_text[menuid-1]+(pageid*CK_TXT_SIZE);
	const unsigned int *CK_PageColor = CK_TXTHELP_color[menuid-1]+(pageid*CK_TXT_SIZE);

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

	// TODO:
	// Handle other pages
	for(int e = 0; e < CK_TXT_HEIGHT; e++){
		for(int i = 0; i < CK_TXT_WIDTH; i++){
			CK_BlitChar(CK_PageText[(e*28)+i], i+1, e+1, CK_PageColor[(e*28)+i]);
		}
	}
};

void CK_RunHelp(){
	if(CK_MenuOn == 0){

		if(CK_UpdateHelpCursor){
			CK_UpdateHelpCursor = false;

			// Draw the pointer
			for(int i = 0; i < CK_BMP0045_width>>2; i++){
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
				CK_PrintPage(CK_MenuOn, CK_PageOn);
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
					CK_PrintPage(CK_MenuOn, CK_PageOn);
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
					CK_PrintPage(CK_MenuOn, CK_PageOn);
				}
				lastButton = 0;
			}
		}



	}


};

*/