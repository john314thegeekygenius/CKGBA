/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#include "romstuffs/CK_SpriteDefs.h"

GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS];
unsigned int CK_NumOfObjects = 0;
unsigned int CK_SprGfxOffset = 0;
unsigned int CK_SprTileOffset = 0;

unsigned short GetNewObj(boolean dummy){
    // Ummm... Will break if more than MAX_OBJS are spawned... :S
    ck_newobj = &CK_ObjectList[CK_NumOfObjects++];
    ck_newobj->gfxoffset = 0xFFFFFFFF;
    ck_newobj->isFree = false;

	ck_newobj->active = ac_yes;
	ck_newobj->needtoclip = cl_midclip;
    return 0;
};

void RemoveObj(objtype *ob){
	if (ob->obclass == stunnedobj){
        // TODO:
        // Handle stunned objects?
    }
    ob->isFree = true;
    // Remove the GBA sprites???
     for(int i = 0; i < ob->gbaSpriteCount; i++){
        GBA_RemoveSprite(ob->gbaSprites[i]);
     }
};

const unsigned int CK_SpriteSizes[] = {
    (8*8)>>3, (16*16)>>3, (32*32)>>3, (64*64)>>3,
    (16*8)>>3, (32*8)>>3, (32*16)>>3, (64*32)>>3,
    (8*16)>>3, (8*32)>>3, (16*32)>>3, (32*64)>>3
};

void CK_SetSprite(objtype *obj, CK_SpriteType type){
    if(ck_newobj->gfxoffset == 0xFFFFFFFF && obj->isFree == false){
        obj->gbaSpriteCount = *CK_SpritePtrs[(type<<2)+1];
        obj->ck_sprType = type;
        obj->gfxoffset = CK_SprGfxOffset;
        for(int i = 0; i < obj->gbaSpriteCount; i++){
            obj->sprtype[i] = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)];
            int sprx = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+1];
            int spry = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+2];
            obj->gbaSprites[i] = GBA_CreateSprite(sprx,spry,obj->sprtype[i],CK_SprTileOffset,0,0);
            CK_SprTileOffset += CK_SpriteSizes[obj->sprtype[i]]>>3;
        }
        CK_SprGfxOffset += (*CK_SpritePtrs[(obj->ck_sprType<<2)+3])>>3;
    }
};

void CK_UpdateObjGraphics(objtype *obj){
    if(obj->isFree) return;
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+obj->gfxoffset;

    uint32_t blocksize = (32*16)>>3;//CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+3]<<3;
    for(int i = 0; i < obj->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[obj->sprtype[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(obj->ck_sprType<<2)+2]+(sprsize*obj->shapenum) + (CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};

void CK_UpdateObjects(){
    
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(obj->isFree) continue;
        for(int i = 0; i < obj->gbaSpriteCount; i++){
            unsigned short sprx = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+1] + obj->x + obj->state->xmove;
            unsigned char spry = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+2] + obj->y + obj->state->xmove;
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], sprx, spry)
        }
    }
    // Update the sprite list
    GBA_UPDATE_SPRITES()
};

void CK_DrawObject(objtype *obj, unsigned int dx, unsigned int dy){
    if(obj->isFree) return;

    for(int i = 0; i < obj->gbaSpriteCount; i++){
        // Make the object work in global map space
        signed int sprx = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+1] + dx - CK_GlobalCameraX;
        signed int spry = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+2] + dy - CK_GlobalCameraY;
        // Make sure we can even see the sprite
        if(sprx > -64 && sprx < (240+64) && spry > -64 && spry < (160+64)){
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], sprx, spry)
            GBA_UPDATE_SPRITE(obj->gbaSprites[i])
        }
    }
};

void CK_SetupSprites(){

    // Clear any software sprites
    GBA_ResetSprites();

    // We have no sprites yet!
    CK_SprGfxOffset = 0;
    CK_NumOfObjects = 0;
    CK_SprTileOffset = 0;

	// Tell the GBA we want sprites
    unsigned int CurReg = *(volatile unsigned int*)GBA_REG_DISPCNT;
    CurReg |= GBA_SPRITE_ENABLE | GBA_SPRITE_MAP_1D;
    *(volatile unsigned int*)GBA_REG_DISPCNT = CurReg;

};

void CK_RemoveSprites(){

    // Clear any software sprites
    GBA_ResetSprites();

    // We have no sprites yet!
    CK_SprGfxOffset = 0;
    CK_NumOfObjects = 0;
    CK_SprTileOffset = 0;
};



