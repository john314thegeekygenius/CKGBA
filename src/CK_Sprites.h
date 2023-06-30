/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

/* 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __CK_SPRITES__
#define __CK_SPRITES__

typedef struct spritegfx_t {
    bool changed;
    bool isStatic;
    bool removed;
    bool wasRendered;
    uint16_t sprnum, sprinit, refs;
    CK_SpriteType sprType;
    unsigned int gfxoffset;
}spritegfx;

// Sprites should never have that many gfx sheets
#define NULL_SPRITE 0xFFFF

extern GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS+1]; // Plus dummy sprite
extern unsigned int CK_NumOfObjects;

extern const unsigned int CK_GFX_NULL;

DONT_OPTIMISE objtype * GetNewObj(boolean dummy);

void CK_SetSprite(objsprite **spr, CK_SpriteType type);

void CK_FixSpriteGraphics(objsprite *sprite);

void CK_RemakeSprite(objsprite **spr, CK_SpriteType type);

void CK_SetSpriteGfx(objsprite **spr, CK_SpriteType type);

void CK_ResetSprGraphicsOffset();

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
