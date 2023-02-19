/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_SPRITES__
#define __CK_SPRITES__

extern objtype *ck_newobj;

extern GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS];
extern unsigned int CK_NumOfObjects;


unsigned short GetNewObj(boolean dummy);

void CK_SetNewSprite(CK_SpriteType type);

void CK_UpdateObjGraphics(objtype *obj);

void CK_UpdateObjects();

void CK_DrawObject(objtype *obj, unsigned int dx, unsigned int dy);

void CK_SetupSprites();

void CK_RemoveSprites();

#endif
