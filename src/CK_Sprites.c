/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#include "romstuffs/CK_SpriteDefs.h"

GBA_IN_EWRAM CK_Object CK_Objects[CK_MAX_OBJS];
unsigned int CK_NumOfObjects = 0;
unsigned int CK_SprGfxOffset = 0;
unsigned int CK_SprTileOffset = 0;

CK_Object *CK_GetObj(){
    CK_Object *ck_free_obj = &CK_Objects;
    for(int i = 0; i < CK_NumOfObjects; i++){
        if(ck_free_obj->isFree){
            return ck_free_obj;
        }
    }
    ck_free_obj = &CK_Objects[CK_NumOfObjects];
    CK_NumOfObjects++; // Ummm... Will break if more than MAX_OBJS are spawned... :S
    ck_free_obj->isFree = 0;
    return ck_free_obj;
};

const unsigned int CK_SpriteSizes[] = {
    (8*8)>>3, (16*16)>>3, (32*32)>>3, (64*64)>>3,
    (16*8)>>3, (32*8)>>3, (32*16)>>3, (64*32)>>3,
    (8*16)>>3, (8*32)>>3, (16*32)>>3, (32*64)>>3
};

void CK_SetSprite(CK_Object *obj, CK_SpriteType type){
    // TODO:
    // Handle replacement of sprite???
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
};

void CK_UpdateObjGraphics(CK_Object *obj){
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+obj->gfxoffset;

    uint32_t blocksize = (32*16)>>3;//CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+3]<<3;
    for(int i = 0; i < obj->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[obj->sprtype[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(obj->ck_sprType<<2)+2]+(sprsize*obj->animation) + (CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};

void CK_UpdateObjects(){
    
    for(int i = 0; i < CK_NumOfObjects; i++){
        CK_Object *obj = &CK_Objects[i];
        for(int i = 0; i < obj->gbaSpriteCount; i++){
            unsigned short sprx = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+1] + obj->x;
            unsigned char spry = CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+2] + obj->y;
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], sprx, spry)
        }
    }
    // Update the sprite list
    GBA_UPDATE_SPRITES()
};


CK_Object *cks_demosign;
//CK_Object *ck_keenobj;
CK_Object *ck_dopefish;
CK_Object *ck_fishys[10];


void CK_SetupSprites(){

    // Clear any software sprites
    GBA_ResetSprites();

    // We have no sprites yet!
    CK_SprGfxOffset = 0;
    CK_NumOfObjects = 0;

	// Tell the GBA we want sprites
    unsigned int CurReg = *(volatile unsigned int*)GBA_REG_DISPCNT;
    CurReg |= GBA_SPRITE_ENABLE | GBA_SPRITE_MAP_1D;
    *(volatile unsigned int*)GBA_REG_DISPCNT = CurReg;

    // Create a new sprite
    cks_demosign = CK_GetObj();
    cks_demosign->x = 88;
    cks_demosign->y = 8;
    cks_demosign->animation = 0;
    CK_SetSprite(cks_demosign, CKS_DEMO);
    CK_UpdateObjGraphics(cks_demosign);

    ck_dopefish = CK_GetObj();
    ck_dopefish->x = 100;
    ck_dopefish->y = 50;
    ck_dopefish->animation = 0;
    CK_SetSprite(ck_dopefish, CKS_DOPEFISH);
    CK_UpdateObjGraphics(ck_dopefish);


    for(int i = 0; i < 10; i++){
        ck_fishys[i] = CK_GetObj();
        ck_fishys[i]->x = i*16;
        ck_fishys[i]->y = CK_US_RndT()>>1;
        ck_fishys[i]->var1 = (i%2)?1:-1;
        ck_fishys[i]->var2 = (i%3)?1:-1;
        ck_fishys[i]->animation = (i%3)?2:0;
        CK_SetSprite(ck_fishys[i], CKS_FISHY);
        CK_UpdateObjGraphics(ck_fishys[i]);
    }
/*
    ck_keenobj = CK_GetObj();
    ck_keenobj->x = 32;
    ck_keenobj->y = 80;
    CK_SetSprite(ck_keenobj, CKS_KEEN);
    CK_UpdateObjGraphics(ck_keenobj);
*/

};

int dopex = 1;
int dopey = 1;
int updatedopy = 0;
int doped = 0;
int movedopy = 0;
int dopylook = 0;

void CK_MoveDopey(){
    if(movedopy < 100){movedopy++; return;}
    movedopy = 0;
    ck_dopefish->x += dopex;
    ck_dopefish->y += dopey;

    for(int i = 0; i < 10; i++){
        ck_fishys[i]->x += ck_fishys[i]->var2;
        ck_fishys[i]->y += ck_fishys[i]->var1;
        if(ck_fishys[i]->x < 0){
            ck_fishys[i]->x = 0;
            ck_fishys[i]->var2 *= -1;
            ck_fishys[i]->animation = 2;
        }
        if(ck_fishys[i]->y < 0){
            ck_fishys[i]->y = 0;
            ck_fishys[i]->var1 *= -1;
        }
        if(ck_fishys[i]->x > 240-8){
            ck_fishys[i]->x = 240-8;
            ck_fishys[i]->var2 *= -1;
            ck_fishys[i]->animation = 0;
        }
        if(ck_fishys[i]->y > 160-8){
            ck_fishys[i]->y = 160-8;
            ck_fishys[i]->var1 *= -1;
        }
    }

    if(ck_dopefish->x < 0){
        ck_dopefish->x = 0;
        dopex = 1;
        doped = 0;
        ck_dopefish->animation = 0;
    }
    if(ck_dopefish->y < 0){
        ck_dopefish->y = 0;
        dopey = 1;
    }
    if(ck_dopefish->x > 240-80){
        ck_dopefish->x = 240-80;
        dopex = -1;
        doped = 1;
        ck_dopefish->animation = 5;
    }
    if(ck_dopefish->y > 160-64){
        ck_dopefish->y = 160-64;
        dopey = -1;
    }

    if(updatedopy < 3){updatedopy++; return;}
    updatedopy = 0;

    ck_dopefish->animation+=1;
    if(doped){
        if(ck_dopefish->animation > 6){ ck_dopefish->animation = 5;}
    }else{
        if(ck_dopefish->animation > 1) { ck_dopefish->animation = 0;}
    }
    for(int i = 0; i < 10; i++){
        if(ck_fishys[i]->animation == 0) ck_fishys[i]->animation = 1;
        else if(ck_fishys[i]->animation == 1) ck_fishys[i]->animation = 0;
        if(ck_fishys[i]->animation == 2) ck_fishys[i]->animation = 3;
        else if(ck_fishys[i]->animation == 3) ck_fishys[i]->animation = 2;
        CK_UpdateObjGraphics(ck_fishys[i]);
    }
    CK_UpdateObjGraphics(ck_dopefish);
};
