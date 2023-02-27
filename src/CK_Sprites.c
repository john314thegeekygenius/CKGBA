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
    ck_newobj->gbaSpriteCount = 0;
    ck_newobj->ck_sprType = 0;
    ck_newobj->uuid = CK_NumOfObjects;
    
	ck_newobj->active = ac_yes;
	ck_newobj->needtoclip = cl_midclip;
/*
    lastobj = new;

	objectcount++;
*/
    return 0;
};

void RemoveObj(objtype *ob){
/*
	if (ob == player)
		Quit("RemoveObj: Tried to remove the player!");

//
// erase it from the refresh manager
//
	RF_RemoveSprite(&ob->sprite);
	if (ob->obclass == stunnedobj)
	{
		RF_RemoveSprite((void **)&ob->temp3);
	}

//
// fix the next object's back link
//
	if (ob == lastobj)
	{
		lastobj = ob->prev;
	}
	else
	{
		ob->next->prev = ob->prev;
	}

//
// fix the previous object's forward link
//
	ob->prev->next = ob->next;

//
// add it back in to the free list
//
	ob->prev = objfreelist;
	objfreelist = ob;    
*/
    // Remove the GBA sprites???
     for(int i = 0; i < ob->gbaSpriteCount; i++){
        GBA_RemoveSprite(ob->gbaSprites[i]);
     }
    ob->gfxoffset = 0xFFFFFFFF;
    ob->isFree = true;
    ob->gbaSpriteCount = 0;
    ob->ck_sprType = 0;
    GBA_UPDATE_SPRITES()
};

const unsigned int CK_SpriteSizes[] = {
    (8*8)>>3, (16*16)>>3, (32*32)>>3, (64*64)>>3,
    (16*8)>>3, (32*8)>>3, (32*16)>>3, (64*32)>>3,
    (8*16)>>3, (8*32)>>3, (16*32)>>3, (32*64)>>3
};

void CK_SetSprite(objtype *obj, CK_SpriteType type){
    if(ck_newobj->gfxoffset == 0xFFFFFFFF && obj->isFree == false){
        obj->gbaSpriteCount = *CK_SpritePtrs[(type*5)+1];
        obj->ck_sprType = type;
        obj->gfxoffset = CK_SprGfxOffset;
        for(int i = 0; i < obj->gbaSpriteCount; i++){
            obj->sprtype[i] = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)];
            obj->gbaSprites[i] = GBA_CreateSprite(GBA_SPR_OFFX,GBA_SPR_OFFY,obj->sprtype[i],CK_SprTileOffset,0,0);
            CK_SprTileOffset += CK_SpriteSizes[obj->sprtype[i]]>>3;
        }
        CK_SprGfxOffset += (*CK_SpritePtrs[(obj->ck_sprType*5)+3])>>3;
    }
};

void CK_UpdateObjGraphics(objtype *obj){
    if(obj->isFree) return;
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+obj->gfxoffset;

    uint32_t blocksize = (32*16)>>3;//CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+3]<<3;
    for(int i = 0; i < obj->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[obj->sprtype[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(obj->ck_sprType*5)+2]+(sprsize*obj->shapenum) + (CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};

void CK_UpdateObjects(){
    // Fix every object position
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        CK_DrawObject(obj,obj->x,obj->y);
    }
    // Update the sprite list
    GBA_UPDATE_SPRITES()
};

void CK_DrawObject(objtype *obj, unsigned int dx, unsigned int dy){
    if(obj->isFree) return;

    for(int i = 0; i < obj->gbaSpriteCount; i++){
        // Make the object work in global map space
        signed int sprx = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+1] + dx - CK_GlobalCameraX + obj->state->xmove;
        signed int spry = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+2] + dy - CK_GlobalCameraY + obj->state->ymove;
        // Make sure we can even see the sprite
        if(sprx > -64 && sprx < (240+64) && spry > -64 && spry < (160+64)){
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], sprx, spry)
            GBA_UPDATE_SPRITE(obj->gbaSprites[i])
        }else{
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], GBA_SPR_OFFX, GBA_SPR_OFFY)
            GBA_UPDATE_SPRITE(obj->gbaSprites[i])
        }
    }
};

void CK_PrintObjInfo(){
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(obj->isFree) continue;
        // ID & Position
        PrintX = 0;
        PrintY = (i+1)*24;
        VW_Bar(PrintX,PrintY,240,8, CK_EGA_WHITE);
        US_PrintUnsigned(obj->obclass);
        PrintX += 16;
        US_PrintUnsigned(obj->x);
        PrintX += 16;
        US_PrintUnsigned(obj->y);
        // Hitbox stuff
        PrintX = 0;
        PrintY = 8+((i+1)*24);
        VW_Bar(PrintX,PrintY,240,8, CK_EGA_YELLOW);
        US_PrintUnsigned(obj->left);
        PrintX += 8;
        US_PrintUnsigned(obj->top);
        PrintX += 8;
        US_PrintUnsigned(obj->right);
        PrintX += 8;
        US_PrintUnsigned(obj->bottom);

    	unsigned short *shape = CK_GetSprShape(obj);
        PrintX = 0;
        PrintY = 16+((i+1)*24);
        VW_Bar(PrintX,PrintY,240,8, CK_EGA_BROWN);
        US_PrintUnsigned(shape[0]);
        PrintX += 8;
        US_PrintUnsigned(shape[1]);
        PrintX += 8;
        US_PrintUnsigned(shape[2]);
        PrintX += 8;
        US_PrintUnsigned(shape[3]);
    }
    // Camera stuff
    PrintX = 12;
    PrintY = 2;
    VW_Bar(PrintX,PrintY,240,8, CK_EGA_CLEAR);
    US_PrintUnsigned(jumpbutton);
    PrintX += 8;
    US_PrintUnsigned(firebutton);
    
    PrintX = 0;
    PrintY = 8;
    US_PrintUnsigned(originxglobal);
    PrintX += 8;
    US_PrintUnsigned(originyglobal);

};

unsigned short *CK_GetSprShape(objtype *obj){
    return (unsigned short*)&CK_SpritePtrs[(obj->ck_sprType*5)+4][obj->shapenum*6];
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



