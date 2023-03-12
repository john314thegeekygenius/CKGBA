/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_SPRITES__
#define __CK_SPRITES__


extern GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS];
extern unsigned int CK_NumOfObjects;


DONT_OPTIMISE objtype * GetNewObj(boolean dummy);

objtype * CK_GetObj(int index);

void CK_SetSprite(objtype *obj, CK_SpriteType type);

void CK_RemakeSprite(objtype *obj, CK_SpriteType type);

void CK_SetDummySprite(objtype *obj);

void CK_UpdateObjGraphics(objtype *obj);

void CK_UpdateObjects();

signed short *CK_GetSprShape(objtype *obj);

signed short *CK_GetShape(unsigned int sprtype, unsigned short shapenumber);

void CK_DrawObject(objtype *obj, signed int dx, signed int dy);

void CK_SetupSprites();

void CK_RemoveSprites();

// DEBUG FUNCTION
void CK_PrintObjInfo();

#endif
