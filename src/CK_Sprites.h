/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_SPRITES__
#define __CK_SPRITES__

extern objtype *ck_newobj;

unsigned short CK_GetNewObj(boolean dummy);

void CK_SetNewSprite(CK_SpriteType type);

void CK_UpdateObjGraphics(objtype *obj);

void CK_UpdateObjects();

void CK_SetupSprites();


#endif
