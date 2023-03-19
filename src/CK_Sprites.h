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

void CK_SetSprite(objsprite **spr, CK_SpriteType type);

void CK_RemakeSprite(objsprite **spr, CK_SpriteType type);

void CK_UpdateSpriteGraphics(objsprite *spr);

signed short *CK_GetSprShape(objsprite *obj);

uint32_t *CK_GetSpriteGfxOffset(objsprite *sprite, int spriteid);

void CK_UpdateSprites();

void CK_DrawSprite(objsprite *sprite);

void CK_SetupSprites();

void CK_RemoveSprites();

void CK_RemoveDummySprites();

void RF_RemoveSprite (void **user, bool cache);

#endif
