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

void CK_SetSprite(objtype *obj, CK_SpriteType type);

void CK_UpdateObjGraphics(objtype *obj);

void CK_UpdateObjects();

signed short *CK_GetSprShape(objtype *obj);

void CK_DrawObject(objtype *obj, unsigned int dx, unsigned int dy);

void CK_SetupSprites();

void CK_RemoveSprites();

// DEBUG FUNCTION
void CK_PrintObjInfo();

#endif
