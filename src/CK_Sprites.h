/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_SPRITES__
#define __CK_SPRITES__


extern GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS+1]; // Plus dummy sprite
extern unsigned int CK_NumOfObjects;

extern const unsigned int CK_GFX_NULL;

DONT_OPTIMISE objtype * GetNewObj(boolean dummy);

objtype * CK_GetObj(int index);

void CK_SetSprite(objtype *obj, CK_SpriteType type);

void CK_RemakeSprite(objtype *obj, CK_SpriteType type);

void CK_SetDummySprite(objtype *obj);

void CK_UpdateObjGraphics(objtype *obj);

void CK_UpdateObjects();

signed short *CK_GetSprShape(objtype *obj);

uint32_t *CK_GetObjGfxOffset(objtype *obj, int spriteid);

void CK_DrawObject(objtype *obj, signed int dx, signed int dy);

void CK_SetupSprites();

void CK_RemoveSprites();

// DEBUG FUNCTION
void CK_PrintObjInfo();

#endif
