/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#define RGBCONV(x) ((((x)>>16)>>3) | (((((x)>>8)&0xFF)>>3)<<5) | ((((x)&0xFF)>>3)<<10))

const unsigned short COMMANDER_KEEN_PALETTE[] = {
	RGBCONV(0xccffcc), 0x00, RGBCONV(0x0000a8),RGBCONV(0x00a800),RGBCONV(0x00a8a8),RGBCONV(0xa80054),
	RGBCONV(0xa85400), RGBCONV(0xa8a8a8), RGBCONV(0x545454), RGBCONV(0x5454fc), RGBCONV(0x54fc54), RGBCONV(0x54fcfc), 
	RGBCONV(0xfc5454), RGBCONV(0xfc54fc), RGBCONV(0xfcfc54), RGBCONV(0xfcfcfc)
};

void CK_InitVideo(){
	// Setup the video
	*(volatile unsigned int*)GBA_REG_DISPCNT = GBA_MODE_0 | GBA_ENABLE_BG0 | GBA_ENABLE_BG1;// | GBA_BG_BACK;// | GBA_SHOW_BACK;
	
	// Copy the palette
	for(int i = 0; i < 16; i++){
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START+(i*16),(uint16_t*)COMMANDER_KEEN_PALETTE,16); // Background (tiles)
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START+(i*16),(uint16_t*)COMMANDER_KEEN_PALETTE,16); // Foreground (sprites)
	}

};

void VW_ClearVideo(unsigned int color){
	GBA_DMA_MemSet32((unsigned int *)GBA_VRAM, color, 32*32*8);
	GBA_DMA_MemSet32((unsigned int *)GBA_VRAM2, 0x00, 32*32*8);
};

