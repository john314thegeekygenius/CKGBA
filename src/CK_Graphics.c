/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#define RGBCONV(x) ((((x)>>16)>>3) | (((((x)>>8)&0xFF)>>3)<<5) | ((((x)&0xFF)>>3)<<10))

const unsigned short COMMANDER_KEEN_PALETTE[] = {
	// Normal EGA palette (0)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x0000aa), RGBCONV(0x00aa00), RGBCONV(0x00aaaa), RGBCONV(0xaa0055), RGBCONV(0xaa5500), RGBCONV(0xaaaaaa), RGBCONV(0x555555), RGBCONV(0x5555ff), RGBCONV(0x55ff55), RGBCONV(0x55ffff), RGBCONV(0xff5555), RGBCONV(0xff55ff), RGBCONV(0xffff55), RGBCONV(0xffffff), 
	// Greyscale palette (1)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x393939), RGBCONV(0x393939), RGBCONV(0x717171), RGBCONV(0x555555), RGBCONV(0x555555), RGBCONV(0xaaaaaa), RGBCONV(0x555555), RGBCONV(0x8e8e8e), RGBCONV(0x8e8e8e), RGBCONV(0xc6c6c6), RGBCONV(0x8e8e8e), RGBCONV(0xc6c6c6), RGBCONV(0xc6c6c6), RGBCONV(0xffffff), 
	// Old Gameboy palette (2)
	RGBCONV(0xccffcc), RGBCONV(0x0f380f), RGBCONV(0x306230), RGBCONV(0x306230), RGBCONV(0x306230), RGBCONV(0x306230), RGBCONV(0x8bac0f), RGBCONV(0x9bbc0f), RGBCONV(0x306230), RGBCONV(0x306230), RGBCONV(0x9bbc0f), RGBCONV(0x9bbc0f), RGBCONV(0x9bbc0f), RGBCONV(0x9bbc0f), RGBCONV(0x9bbc0f), RGBCONV(0x9bbc0f), 
	// Vibrent EGA (3)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x0000ff), RGBCONV(0x00ff00), RGBCONV(0x00ffff), RGBCONV(0xbf007f), RGBCONV(0xff7f00), RGBCONV(0xc0c0c0), RGBCONV(0x7f7f7f), RGBCONV(0x007fff), RGBCONV(0x7fff00), RGBCONV(0x85ffff), RGBCONV(0xff007f), RGBCONV(0xffb6c1), RGBCONV(0xffff00), RGBCONV(0xffffff), 
	// C64 (4)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x5d00ab), RGBCONV(0x44c14e), RGBCONV(0x44c14e), RGBCONV(0x9f4238), RGBCONV(0xa16a1d), RGBCONV(0xb3b3b3), RGBCONV(0x606060), RGBCONV(0x9758df), RGBCONV(0x44c14e), RGBCONV(0x6cc8cf), RGBCONV(0xce7c73), RGBCONV(0x9758df), RGBCONV(0xc9ec7a), RGBCONV(0xffffff), 

};


unsigned short CK_PaletteSet = 0;

void CK_FixPalette(){
	// Copy the palette
	for(int i = 0; i < 16; i++)
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START+(i*16),(uint16_t*)COMMANDER_KEEN_PALETTE+(CK_PaletteSet*16),16);

	// Copy the palette (foreground / sprites)
	for(int i = 0; i < 16; i++)
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START+(i*16),(uint16_t*)COMMANDER_KEEN_PALETTE+(CK_PaletteSet*16),16);

	// Copy over a completely white palette for sprite flashing
	for(int i = 0; i < 16; i++){
		*((uint16_t*)GBA_PAL_SPR_START+(15*16)+i) = COMMANDER_KEEN_PALETTE[(CK_PaletteSet*16)+15];
	}
};


volatile uint16_t *TILEMAP_0 = (volatile uint16_t*)GBA_SCREEN_BLOCK(30);
volatile uint16_t *TILEMAP_1 = (volatile uint16_t*)GBA_SCREEN_BLOCK(31);

volatile uint16_t* TILESTART_0 = (volatile uint16_t*)GBA_VRAM;
volatile uint16_t* TILESTART_1 = (volatile uint16_t*)(GBA_VRAM+0x6000)-1024;


void CK_InitVideo(){
	// Setup the video
	*(volatile unsigned int*)GBA_REG_DISPCNT = GBA_MODE_0 | GBA_ENABLE_BG0 | GBA_ENABLE_BG1;// | GBA_BG_BACK;// | GBA_SHOW_BACK;
	
	// Set the palette
	CK_FixPalette();
};


boolean		screenfaded;



void VW_ClearVideo(unsigned int color){
	GBA_DMA_MemSet32((unsigned int *)GBA_VRAM, color, 32*32*8);
	GBA_DMA_MemSet32((unsigned int *)GBA_VRAM2, 0x00, 32*32*8);
};

void VW_Bar(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char c){
	const uint32_t col = CK_TXTCOL(c);
	const uint32_t colc = CK_TXTCOL(CK_EGA_CLEAR);
	x >>= 3; y >>= 3;
	w >>= 3; h >>= 3;
	for(int i = 0; i < h; i++){
		GBA_DMA_MemSet32((unsigned int *)TILESTART_0+(x<<3)+(y<<8), col, w<<3);
		GBA_DMA_MemSet32((unsigned int *)TILESTART_1+(x<<3)+(y<<8), colc, w<<3);
	}
};

