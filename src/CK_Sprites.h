/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_SPRITES__
#define __CK_SPRITES__


#define CK_MAX_OBJS 8
#define MAX_SPRITES 16

typedef struct CK_Object {
    // Commander Keen data
    signed int x;
    signed int y;
    signed int zLayer;

    unsigned int animation;
    unsigned int var1;
    unsigned int var2;
    unsigned int var3;

    // GBA Memory Stuff
    unsigned char isFree;
    GBA_SpriteIndex_t gbaSprites[MAX_SPRITES];
    unsigned int gbaSpriteCount;
    unsigned int gfxoffset;
    CK_SpriteType ck_sprType;
    unsigned int sprtype[MAX_SPRITES];
}CK_Object;



CK_Object *CK_GetObj();

void CK_SetSprite(CK_Object *obj, CK_SpriteType type);

void CK_UpdateObjGraphics(CK_Object *obj);

void CK_UpdateObjects();

void CK_SetupSprites();

void CK_MoveDopey();


#endif
