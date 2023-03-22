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

// GB Palette defs
#define GB_COL_0 RGBCONV(0x081820)
#define GB_COL_1 RGBCONV(0x346856)
#define GB_COL_2 RGBCONV(0x88c070)
#define GB_COL_3 RGBCONV(0xe0f8d0)

const unsigned short COMMANDER_KEEN_PALETTE[] = {
	// Normal EGA palette (0)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x0000aa), RGBCONV(0x00aa00), RGBCONV(0x00aaaa), RGBCONV(0xaa0055), RGBCONV(0xaa5500), RGBCONV(0xaaaaaa), RGBCONV(0x555555), RGBCONV(0x5555ff), RGBCONV(0x55ff55), RGBCONV(0x55ffff), RGBCONV(0xff5555), RGBCONV(0xff55ff), RGBCONV(0xffff55), RGBCONV(0xffffff), 
	// Greyscale palette (1)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x393939), RGBCONV(0x393939), RGBCONV(0x717171), RGBCONV(0x555555), RGBCONV(0x555555), RGBCONV(0xaaaaaa), RGBCONV(0x555555), RGBCONV(0x8e8e8e), RGBCONV(0x8e8e8e), RGBCONV(0xc6c6c6), RGBCONV(0x8e8e8e), RGBCONV(0xc6c6c6), RGBCONV(0xc6c6c6), RGBCONV(0xffffff), 
	// Old Gameboy palette (2)
	RGBCONV(0xccffcc), GB_COL_0, GB_COL_1, GB_COL_1, GB_COL_1, GB_COL_1, GB_COL_2, GB_COL_2, GB_COL_1, GB_COL_2, GB_COL_3, GB_COL_3, GB_COL_3, GB_COL_3, GB_COL_2, GB_COL_3,
	// Vibrent EGA (3)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x0000ff), RGBCONV(0x00ff00), RGBCONV(0x00ffff), RGBCONV(0xbf007f), RGBCONV(0xff7f00), RGBCONV(0xc0c0c0), RGBCONV(0x7f7f7f), RGBCONV(0x007fff), RGBCONV(0x7fff00), RGBCONV(0x85ffff), RGBCONV(0xff007f), RGBCONV(0xffb6c1), RGBCONV(0xffff00), RGBCONV(0xffffff), 
	// C64 (4)
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x5d00ab), RGBCONV(0x44c14e), RGBCONV(0x44c14e), RGBCONV(0x9f4238), RGBCONV(0xa16a1d), RGBCONV(0xb3b3b3), RGBCONV(0x606060), RGBCONV(0x9758df), RGBCONV(0x44c14e), RGBCONV(0x6cc8cf), RGBCONV(0xce7c73), RGBCONV(0x9758df), RGBCONV(0xc9ec7a), RGBCONV(0xffffff), 
};

const unsigned short PALETTE_SHIFT[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 1, 1, 1, 1, 1, 1, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 5, 6, 7,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

// Palettes are 0-4, dynamic palette is 5
unsigned short CK_PaletteSet = 0;

extern const unsigned short CK_DynamicPalIndex[GAMELEVELS+1];
extern const unsigned short CK_DYNAMIC_PALS[];
extern boolean 				CtlPanelDone;

void CK_FixPalette(){
	if(CK_PaletteSet == 5){
		unsigned short dynamicIndex = 0;
		// Handle the dynamic palette
		if( CtlPanelDone && (ingame || DemoMode == demo_Playback) ){
			dynamicIndex = CK_DynamicPalIndex[gamestate.mapon+1];
		}else{
			dynamicIndex = CK_DynamicPalIndex[0];
		}

		// Copy the palette
		for(int i = 0; i < 16; i++){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START+(i*16),(uint16_t*)CK_DYNAMIC_PALS+(dynamicIndex*16),16);
		// Copy the palette (foreground / sprites)
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START+(i*16),(uint16_t*)CK_DYNAMIC_PALS+(dynamicIndex*16),16);
		}
		// Copy over a completely white palette for sprite flashing
		for(int i = 0; i < 16; i++){
			*((uint16_t*)GBA_PAL_SPR_START+(16)+i) = CK_DYNAMIC_PALS[(dynamicIndex*16)+15];
		}
		return;
	}
	// Copy the palette
	for(int i = 0; i < 16; i++){
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START+(i*16),(uint16_t*)COMMANDER_KEEN_PALETTE+(CK_PaletteSet*16),16);
	// Copy the palette (foreground / sprites)
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START+(i*16),(uint16_t*)COMMANDER_KEEN_PALETTE+(CK_PaletteSet*16),16);
	}

	// Copy over a completely white palette for sprite flashing
	for(int i = 0; i < 16; i++){
		*((uint16_t*)GBA_PAL_SPR_START+(16)+i) = COMMANDER_KEEN_PALETTE[(CK_PaletteSet*16)+15];
	}
};

void VW_FadeIn(){
	uint16_t* paletteA = (uint16_t*)GBA_PAL_BG_START;
	uint16_t* paletteB = (uint16_t*)GBA_PAL_SPR_START;
	// Copy the palette
	for(int fade = 3; fade >= 0; fade--){
		for(int i = 0; i < 16; i++){
			uint16_t* keenpalette = (uint16_t*)COMMANDER_KEEN_PALETTE+(CK_PaletteSet*16);
			if(CK_PaletteSet == 5){
				unsigned short dynamicIndex = 0;
				// Handle the dynamic palette
				if( CtlPanelDone && (ingame || DemoMode == demo_Playback) ){
					dynamicIndex = CK_DynamicPalIndex[gamestate.mapon+1];
				}else{
					dynamicIndex = CK_DynamicPalIndex[0];
				}
				keenpalette = (uint16_t*)CK_DYNAMIC_PALS+(dynamicIndex*16);
			}
			for(int e = 0; e < 16; e++){
				*paletteA = keenpalette[PALETTE_SHIFT[e+ (fade<<4)]];
				*paletteB = keenpalette[PALETTE_SHIFT[e+ (fade<<4)]];
				paletteA++;
				paletteB++;
			}
		}
		GBA_Delay(250);
	}
	CK_FixPalette();
};

void VW_FadeOut(){
	uint16_t* paletteA = (uint16_t*)GBA_PAL_BG_START;
	uint16_t* paletteB = (uint16_t*)GBA_PAL_SPR_START;
	// Copy the palette
	for(int fade = 0; fade < 4; fade++){
		for(int i = 0; i < 16; i++){
			uint16_t* keenpalette = (uint16_t*)COMMANDER_KEEN_PALETTE+(CK_PaletteSet*16);
			if(CK_PaletteSet == 5){
				unsigned short dynamicIndex = 0;
				// Handle the dynamic palette
				if( CtlPanelDone && (ingame || DemoMode == demo_Playback) ){
					dynamicIndex = CK_DynamicPalIndex[gamestate.mapon+1];
				}else{
					dynamicIndex = CK_DynamicPalIndex[0];
				}
				keenpalette = (uint16_t*)CK_DYNAMIC_PALS+(dynamicIndex*16);
			}
			for(int e = 0; e < 16; e++){
				*paletteA = keenpalette[PALETTE_SHIFT[e+ (fade<<4)]];
				*paletteB = keenpalette[PALETTE_SHIFT[e+ (fade<<4)]];
				paletteA++;
				paletteB++;
			}
		}
		GBA_Delay(250);
	}
};


uint16_t *TILEMAP_0 = (uint16_t*)GBA_SCREEN_BLOCK(30);
uint16_t *TILEMAP_1 = (uint16_t*)GBA_SCREEN_BLOCK(31);
uint16_t *TILEMAP_2 = (uint16_t*)GBA_SCREEN_BLOCK(29);

uint16_t* TILESTART_0 = (uint16_t*)GBA_VRAM;
uint16_t* TILESTART_1 = (uint16_t*)(GBA_VRAM+0x6000)-1024;

bool screenchanged = false;

void CK_InitVideo(){
	// Setup the video
	*(volatile unsigned int*)GBA_REG_DISPCNT = GBA_MODE_0 | GBA_ENABLE_BG0 | GBA_ENABLE_BG1;
	
	// Set the palette
	CK_FixPalette();

	screenchanged = false;

};

boolean		screenfaded;

// Differs from CA_FixGraphics - doesn't clear the tiles
void VW_FixGraphics(){
	// Set the map to a constant 
	for(int i = 0; i < 32*22; i++){
		*(uint16_t*)(TILEMAP_0+i) = i;
		*(uint16_t*)(TILEMAP_1+i) = i+0xC0;
		*(uint16_t*)(TILEMAP_2+i) = 0x380; // This tile must ALWAYS be clear!
	}
    // Clear the special tile
    GBA_DMA_MemSet32((volatile uint32_t *)TILESTART_1+0x1600, CK_EGA_CLEAR, 8);

	// Finish the render of the background
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | TILEMAP_MAP_0 | TILEMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | TILEMAP_MAP_1 | TILEMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG2_4BIT(GBA_BG_FRONT | TILEMAP_MAP_2 | TILEMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
	*(volatile unsigned int*)GBA_REG_DISPCNT |= GBA_ENABLE_BG2;

//	VW_ClearScroll();
};

void CA_FixGraphics(){
	VW_FixGraphics();
	// Clear the memory
    GBA_DMA_MemSet32((volatile uint32_t *)TILESTART_1, CK_EGA_CLEAR, 32*22*8);
};


void VW_ClearVideo(unsigned int color){
	GBA_DMA_MemSet32((unsigned int *)GBA_VRAM, color, 32*32*12);
};

void VW_ClearScroll(){
	originxglobal = 0;
	originyglobal = 0;

	CK_CameraX = 0;
	CK_CameraY = 0;

	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;

	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;

	*(volatile uint16_t*)GBA_REG_BG2HOFS = 0;
	*(volatile uint16_t*)GBA_REG_BG2VOFS = 0;
};

void RF_SetOfs(int x, int y){
	*(volatile uint16_t*)GBA_REG_BG0HOFS = x;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = y;

	*(volatile uint16_t*)GBA_REG_BG1HOFS = x;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = y;

	*(volatile uint16_t*)GBA_REG_BG2HOFS = x;
	*(volatile uint16_t*)GBA_REG_BG2VOFS = y;
};

void RF_FixOfs(int x, int y){
//	originxglobal = CONVERT_PIXEL_TO_GLOBAL(x);
//	originyglobal = CONVERT_PIXEL_TO_GLOBAL(y);

	CK_CameraX = x;
	CK_CameraY = y;

	RF_SetOfs(x,y);
};

extern const unsigned char TIL_8_UNMASKED[];
extern const unsigned char TIL_8_MASKED[];

void VWB_DrawTile8(unsigned int x, unsigned int y, unsigned short tile){
	x >>= 3; y >>= 3;
	GBA_DMA_Copy32((unsigned int *)TILESTART_1+(x<<3)+(y<<8),(unsigned int *) TIL_8_UNMASKED+(tile<<3), 8);
};

void VWB_DrawTile8M(unsigned int x, unsigned int y, unsigned short tile){
	x >>= 3; y >>= 3;
	GBA_DMA_Copy32((unsigned int *)TILESTART_1+(x<<3)+(y<<8),(unsigned int *) TIL_8_MASKED+(tile<<3), 8);
};

#include "CK_BMPS.h"

void VWB_DrawPic (int x, int y, int chunknum)
{
	unsigned short height,width;
	unsigned int *source;
// mostly copied from drawpic
	int	picnum = chunknum;

	source = CK_BITMAP_MAPPER[chunknum*3];
	width = *CK_BITMAP_MAPPER[(chunknum*3)+1];
	width *= 2; // ???
	height = *CK_BITMAP_MAPPER[(chunknum*3)+2];

	x >>= 3; y >>= 3;

	width >>= 3;
	height >>= 3;
	for(int i = 0; i < height; i++){
		GBA_DMA_Copy32((unsigned int *)TILESTART_0+(x<<3)+((i+y)<<8), source, width<<3);
		source += width<<3;
	}
};

void VWB_DrawMPic (int x, int y, int chunknum)
{
	unsigned short height,width;
	unsigned int *source;
// mostly copied from drawpic
	int	picnum = chunknum;

	source = CK_BITMAP_MAPPER[chunknum*3];
	width = *CK_BITMAP_MAPPER[(chunknum*3)+1];
	width *= 2; // ???
	height = *CK_BITMAP_MAPPER[(chunknum*3)+2];

	x >>= 3; y >>= 3;

	width >>= 3;
	height >>= 3;
	for(int i = 0; i < height; i++){
		GBA_DMA_Copy32((unsigned int *)TILESTART_1+(x<<3)+((i+y)<<8), source, width<<3);
		source += width<<3;
	}
};


void VWB_DrawFullPic(unsigned char *data){
	GBA_DMA_Copy32((unsigned int *)TILESTART_0, (unsigned int *)data, 32*20*8);
	GBA_DMA_MemSet32((unsigned int *)TILESTART_1, (unsigned int *)CK_EGA_CLEAR, 32*20*8);
};

void VWB_Bar(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char c){
	const uint32_t col = CK_TXTCOL(c);
	const uint32_t colc = CK_TXTCOL(CK_EGA_CLEAR);
	x >>= 3; y >>= 3;
	w >>= 3; h >>= 3;
	for(int i = 0; i < h; i++){
		GBA_DMA_MemSet32((unsigned int *)TILESTART_0+(x<<3)+((i+y)<<8), col, w<<3);
		GBA_DMA_MemSet32((unsigned int *)TILESTART_1+(x<<3)+((i+y)<<8), colc, w<<3);
	}
};

unsigned char * vw_plotplane = NULL;

void VWB_Plot (int x, int y, unsigned char color)
{
	// Find the block to write to
	int bx = x>>3;
	int by = y>>3;
	int subx = x-(bx<<3);
	int suby = y-(by<<3);
	// Plot pixels in the backplane
	unsigned char * tileblock = vw_plotplane + (bx<<5) + (by<<10) + (suby<<2) + (subx>>1);

	if(x&1){
		*tileblock &= 0xF;
		*tileblock |= (color&0xF0);
		return;
	}
	*tileblock &= 0xF0;
	*tileblock |= (color&0xF);
}

void VWB_Hlin (int x1, int x2, int y, int color)
{
	vw_plotplane = (unsigned char *)TILESTART_0;
	color &= 0xF;
	color |= color<<4;
	for(; x1 <= x2; x1++){
		VWB_Plot(x1,y, color&0xFF);
	}
}

void VWB_Vlin (int y1, int y2, int x, int color)
{
	vw_plotplane = (unsigned char *)TILESTART_0;
	color &= 0xF;
	color |= color<<4;
	for(; y1 <= y2; y1++){
		VWB_Plot(x,y1, color&0xFF);
	}
}

void VWB_Hlin2 (int x1, int x2, int y, int color)
{
	vw_plotplane = (unsigned char *)TILESTART_1;
	color &= 0xF;
	color |= color<<4;
	for(; x1 <= x2; x1++){
		VWB_Plot(x1,y, color&0xFF);
	}
}

void VWB_Vlin2 (int y1, int y2, int x, int color)
{
	vw_plotplane = (unsigned char *)TILESTART_1;
	color &= 0xF;
	color |= color<<4;
	for(; y1 <= y2; y1++){
		VWB_Plot(x,y1, color&0xFF);
	}
}

DONT_OPTIMISE void VW_WaitVBL(unsigned int vbls){
	while(vbls--){
		GBA_WAIT_VBLANK
	}
};


void VW_UpdateScreen(){
	screenchanged = true;
};

// Sprites to be used with DrawSprite
objsprite *VW_DummySprites[10];
int vw_sprcnt = 0;

void VWB_ClearSpriteCache(){
	vw_sprcnt = 0;
	CK_RemoveDummySprites();
};

void VWB_DrawSprite(int x, int y, int chunknum, CK_SpriteType type){
	VW_DummySprites[vw_sprcnt] = CK_GetNewSprite();
	CK_SetSprite(&VW_DummySprites[vw_sprcnt], type);

	x = CONVERT_PIXEL_TO_GLOBAL(x)+originxglobal;
	y = CONVERT_PIXEL_TO_GLOBAL(y)+originyglobal;

	x+=CK_CameraX;
	y+=CK_CameraY;

	RF_PlaceSprite(&VW_DummySprites[vw_sprcnt], x, y, chunknum, spritedraw, 1);
	vw_sprcnt++;
};
