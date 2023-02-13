/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"



extern const unsigned int CKS_Demo_width;
extern const unsigned int CKS_Demo_height;
extern const unsigned int CKS_Demo_size;
extern const unsigned char CKS_Demo_data[];

void CK_SetupSprites(){
    for(int i = 0; i < CKS_Demo_height; i+=8)
    	GBA_DMA_Copy32((uint32_t*)GBA_VSRAM+(i*32), (uint32_t*)CKS_Demo_data+(i*(CKS_Demo_width>>3)), CKS_Demo_width);

    // Clear any software sprites
    GBA_ResetSprites();

	// Tell the GBA we want sprites
    unsigned int CurReg = *(volatile unsigned int*)GBA_REG_DISPCNT;
    CurReg |= GBA_SPRITE_ENABLE | GBA_SPRITE_MAP_2D;
    *(volatile unsigned int*)GBA_REG_DISPCNT = CurReg;

    // Create a new sprite
    GBA_SpriteIndex_t sprid = GBA_CreateSprite(88,8,GBA_SPR_32_32,0,0,0);
    GBA_SpriteIndex_t sprid2 = GBA_CreateSprite(88+32,8,GBA_SPR_32_32,4,0,0);

    // Update the sprite list
    GBA_UPDATE_SPRITES()
};
