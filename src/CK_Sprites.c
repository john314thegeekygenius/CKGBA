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

#include "CK_Heads.h"

#include "romstuffs/CK_SpriteDefs.h"

GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS+1];
GBA_IN_EWRAM objsprite CK_SpriteList[MAXSPRITES];
GBA_IN_EWRAM spritegfx gfxhandler[MAXSPRITES];

unsigned int CK_NumOfSprites = 0;

unsigned int CK_NumOfObjects = 0;
unsigned int CK_SprGfxOffset = 0;
unsigned int CK_NumOfGfxLocks = 0;

const unsigned int CK_GFX_NULL = 0xDEADBEEF;

void CK_ClearSprite(objsprite *spr, bool overwrite){
    if(spr==NULL) return;
    spr->deltax = spr->deltay = 0;
    spr->drawtype = 0xF;
    // Overwrite these or else bad things happen
    spr->gbaSpriteCount = 0;
    for(int i = 0; i < 16; i++)
        spr->sprsizes[i] = 0;

    spr->rendered = false;

    if(overwrite){
        if(spr->gfxsprindx != NULL_SPRITE){
            gfxhandler[spr->gfxsprindx].sprnum = NULL_SPRITE;
            gfxhandler[spr->gfxsprindx].sprinit = NULL_SPRITE;
            gfxhandler[spr->gfxsprindx].gfxoffset = CK_GFX_NULL;
            gfxhandler[spr->gfxsprindx].sprType = CKS_EOL;
            gfxhandler[spr->gfxsprindx].changed = false;
            gfxhandler[spr->gfxsprindx].removed = true;
            gfxhandler[spr->gfxsprindx].isStatic = true;// Hmm
            gfxhandler[spr->gfxsprindx].refs -= 1;
        }
        spr->gfxsprindx = NULL_SPRITE;
    }
    spr->ck_sprType = CKS_EOL;
};

static void CK_FreeObject(objtype *obj){
    if(!obj) Quit("CK_FreeObject : Bad obj");

    obj->curSprType = CKS_EOL;

    obj->removed = true;

    obj->isFree = true;
};

static void CK_ClearObject(objtype *obj,bool overwrite){
    if(!obj) Quit("CK_ClearObject : Bad obj");
	obj->obclass = 0;
	obj->active = 0;
	obj->needtoreact = 0;
	obj->needtoclip = 0;
	obj->nothink = 0;
	obj->x = obj->y = 0;
	obj->xdir = obj->ydir = 0;
	obj->xmove = obj->ymove = 0;
	obj->xspeed = obj->yspeed = 0;
	obj->ticcount = 0;
	obj->state = NULL;
	obj->shapenum = 0;
	obj->priority = 0;
	obj->left = obj->top = obj->right = obj->bottom = obj->midx = 0;
	obj->tileleft = obj->tiletop = obj->tileright = obj->tilebottom = obj->tilemidx = 0;
	obj->hitnorth = obj->hiteast = obj->hitsouth = obj->hitwest = 0;
	obj->temp1 = obj->temp2 = obj->temp3 = obj->temp4 = 0;
    if(obj->sprite) Quit("Sprite wasn't removed!");
    obj->sprite = NULL;
	//struct objstruct *next, *prev; // Originally used for saveing / loading
    ///////////////////////////////////////////////////
    // Added for GBA port
    obj->curSprType = CKS_EOL;
    if(overwrite){
        obj->uuid = 0;
        obj->removed = true;
        // Hmmm
        obj->isFree = true;
    }
};

DONT_OPTIMISE objtype *GetNewObj(boolean usedummy){
    // Find an old object to use
    for(int i = 0; i < CK_NumOfObjects; i++){
        ck_newobj = &CK_ObjectList[i];
        //ck_newobj->obclass == 0 && 
        if(ck_newobj->removed && ck_newobj->isFree 
            && ck_newobj->curSprType == CKS_EOL){
            ck_newobj->sprite = NULL;
            ck_newobj->uuid = i;
            goto foundObj;
        }
    }

    ck_newobj = NULL;

    // Ummm... Will break if more than MAXACTORS are spawned... :S
    if(CK_NumOfObjects >= MAXACTORS-1){
        if (usedummy){
//            Quit("GetNewObj: Used Dummy Object!");
            ck_newobj = &CK_ObjectList[MAXACTORS-1]; // Dummy obj stored at end of list???
            ck_newobj->removed = false;
            ck_newobj->sprite = NULL;
            ck_newobj->uuid = MAXACTORS-1;
            ck_newobj->isFree = false;
            ck_newobj->removed = false;
            ck_newobj->shapenum = 0;
            return ck_newobj;
        }
        Quit("GetNewObj: No free spots in objarray!");
    }
    ck_newobj = &CK_ObjectList[CK_NumOfObjects++];
    ck_newobj->sprite = NULL;
    ck_newobj->uuid = CK_NumOfObjects-1;
foundObj:

    ck_newobj->isFree = false;
    ck_newobj->removed = false;
    ck_newobj->shapenum = 0;

	ck_newobj->active = ac_yes;
	ck_newobj->needtoclip = cl_midclip;

    return ck_newobj;
};

objsprite *CK_GetNewSprite(CK_SpriteType type){
    objsprite *spr ;

    // Find any similar sprites already alocated
    for(int si = 0; si < CK_NumOfSprites; si++){
        spr = &CK_SpriteList[si];
        if(spr->ck_prevType == type && spr->gbaSpriteCount==0 && spr->ck_sprType == CKS_EOL){
            CK_ClearSprite(spr, false);
            spr->ck_prevType = type;
            return spr;
        }
    }
    spr = &CK_SpriteList[CK_NumOfSprites++];
    if(CK_NumOfSprites > MAXSPRITES) Quit("CK_GetNewSprite() : No free sprites!");
    CK_ClearSprite(spr, true);
    spr->ck_prevType = type;
    return spr;
};

void RemoveObj(objtype *ob){
	if (ob == player)
		Quit("RemoveObj: Tried to remove the player!");

    if(!ob || ob->removed) return; // ????
//
// erase it from the refresh manager
//
	RF_RemoveSprite(&ob->sprite, false);
	if (ob->obclass == stunnedobj){
		RF_RemoveSprite(&ob->temp3, false);
	}

    CK_FreeObject(ob);

};


const unsigned int CK_SpriteSizes[] = {
    (8*8)>>3, (16*16)>>3, (32*32)>>3, (64*64)>>3,
    (16*8)>>3, (32*8)>>3, (32*16)>>3, (64*32)>>3,
    (8*16)>>3, (8*32)>>3, (16*32)>>3, (32*64)>>3
};

const unsigned int CK_GBA_SprSizes[12][2] = {
    {8,8},
    {16,16},
    {32,32},
    {64,64},
    {16,8},
    {32,8},
    {32,16},
    {64,32},
    {8,16},
    {8,32},
    {16,32},
    {32,64},
};

void CK_GetSprGfxMem(objsprite *spr){
    
    // Find any similar sprites already alocated
    for(int si = 0; si < CK_NumOfGfxLocks; si++){
        if(spr->gfxsprindx == si) continue; // We don't want the same one!
        if(gfxhandler[si].gfxoffset != CK_GFX_NULL && 
           gfxhandler[si].sprType == spr->ck_sprType){
            if(gfxhandler[si].isStatic == false){
                continue;
            }
            if(gfxhandler[si].removed){
                // We have a good sprite!
                spr->gfxsprindx = si;
                gfxhandler[spr->gfxsprindx].changed = true;
                gfxhandler[spr->gfxsprindx].removed = false;
                gfxhandler[spr->gfxsprindx].sprnum = spr->spritenum;
                gfxhandler[spr->gfxsprindx].refs = 1;
                return;
            }else if(gfxhandler[si].sprnum == spr->spritenum || gfxhandler[si].sprnum == NULL_SPRITE){
                // Reuse a sprite
                spr->gfxsprindx = si;
                gfxhandler[spr->gfxsprindx].changed = false;
                gfxhandler[spr->gfxsprindx].refs += 1;
                return;
            }else if(spr->rendered == false){
                // Reuse a sprite
                spr->gfxsprindx = si;
                gfxhandler[spr->gfxsprindx].changed = true;
                gfxhandler[spr->gfxsprindx].sprnum = spr->spritenum;
                gfxhandler[spr->gfxsprindx].refs += 1;
                return;
            }
        }
    }
    spr->gfxsprindx = CK_NumOfGfxLocks;
    gfxhandler[spr->gfxsprindx].sprType = spr->ck_sprType;
fixgfxjmp:
    gfxhandler[spr->gfxsprindx].changed = true;
    gfxhandler[spr->gfxsprindx].isStatic = true; // Assume this
    gfxhandler[spr->gfxsprindx].sprnum = spr->spritenum;
    gfxhandler[spr->gfxsprindx].sprinit = spr->spritenum;
    gfxhandler[spr->gfxsprindx].gfxoffset = CK_SprGfxOffset;
    gfxhandler[spr->gfxsprindx].removed = false;
    gfxhandler[spr->gfxsprindx].refs = 1;

    CK_NumOfGfxLocks += 1;
    if(CK_NumOfGfxLocks >= MAXSPRITES){
        Quit("CK_GetSprGfxMem : No free graphics locks left!");
    }

    if(CK_SprGfxOffset >= 0x8000){
        Quit("CK_GetSprGfxMem : No free graphics space left!");
    }

    CK_SprGfxOffset += (*CK_SpritePtrs[(spr->ck_sprType*5)+3])>>3;

};

void CK_SetSprite(objsprite **sprite, CK_SpriteType type){
    bool spriteHadGfx = false;
    if(type == CKS_EOL) return; // Uhh
    if(!(*sprite)) {
        *sprite = CK_GetNewSprite(type);
    }
    objsprite *spr = *sprite;

    if(spr == NULL) Quit("CK_SetSprite : Bad sprite!");

    spr->gbaSpriteCount = *CK_SpritePtrs[(type*5)+1];

    spr->ck_sprType = type;
    spr->spritenum = 0; // ???

    spr->gfxheight = spr->gfxwidth = 0;
    for(int i = 0; i < spr->gbaSpriteCount; i++){
        spr->sprsizes[i] = CK_SpritePtrs[(spr->ck_sprType*5)][(i*4)];
        int gw = CK_GBA_SprSizes[spr->sprsizes[i]][0];
        int gh = CK_GBA_SprSizes[spr->sprsizes[i]][1];
        int gx = CK_SpritePtrs[(spr->ck_sprType*5)][(i*4)+1];
        int gy = CK_SpritePtrs[(spr->ck_sprType*5)][(i*4)+2];
        if(gy >= spr->gfxheight){
            spr->gfxheight = gy + gh;
        }
        if(gx >= spr->gfxwidth){
            spr->gfxwidth = gx + gw;
        }
    }

    if(spr->gfxsprindx == NULL_SPRITE || 
            gfxhandler[spr->gfxsprindx].gfxoffset == CK_GFX_NULL || 
            spr->ck_prevType != type){
        CK_GetSprGfxMem(spr);
    }

    spr->ck_prevType = type;

    CK_UpdateSpriteGraphics(spr);
};

void CK_FixSpriteGraphics(objsprite *sprite) {
    // This should never happen???
    if(sprite == NULL) Quit("CK_FixSpriteGraphics : Bad sprite!");

    // Make sure the graphics memory is ok
    if(sprite->gfxsprindx != NULL_SPRITE){
        if(gfxhandler[sprite->gfxsprindx].refs>1){
            if(gfxhandler[sprite->gfxsprindx].sprnum != sprite->spritenum){
                // We need a new sprite
                gfxhandler[sprite->gfxsprindx].refs -= 1;
                CK_GetSprGfxMem(sprite);
                gfxhandler[sprite->gfxsprindx].isStatic = false;
                gfxhandler[sprite->gfxsprindx].sprnum = sprite->spritenum;
                gfxhandler[sprite->gfxsprindx].changed = true;
            }
        }else {
            if(gfxhandler[sprite->gfxsprindx].sprnum != sprite->spritenum){
                gfxhandler[sprite->gfxsprindx].isStatic = false;
                gfxhandler[sprite->gfxsprindx].sprnum = sprite->spritenum;
                gfxhandler[sprite->gfxsprindx].changed = true;
            }
        }
    }else{
        CK_GetSprGfxMem(sprite);
    }
};


void CK_RemakeSprite(objsprite **spr, CK_SpriteType type){
    if(!spr || !(*spr)) return;
    // Remove sprite
    CK_ClearSprite(*spr, false);
    // Create new one
    CK_SetSprite(spr, type);
};

void CK_SetSpriteGfx(objsprite **spr, CK_SpriteType type){
    if(!spr || !(*spr)) return;
    // Make sure to fake the last sprite type
    (*spr)->ck_prevType = type;
    // Reset the sprite
    CK_SetSprite(spr, type);
};

void CK_UpdateSpriteGraphics(objsprite *sprite){
    if(!sprite) return;
    if(sprite->ck_sprType == CKS_EOL) return;
    if(sprite->ck_sprType < 0 || sprite->ck_sprType > CKS_EOL) Quit("CK_UpdateSpriteGraphics : \n Bad Sprite Type!");
    if(sprite->gfxsprindx == NULL_SPRITE || gfxhandler[sprite->gfxsprindx].gfxoffset == CK_GFX_NULL) Quit("CK_UpdateSpriteGraphics : \nInvalid Graphics Memory used!");
    
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+gfxhandler[sprite->gfxsprindx].gfxoffset;

    for(int i = 0; i < sprite->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[sprite->sprsizes[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(sprite->ck_sprType*5)+2]+(sprsize*sprite->spritenum) + (CK_SpritePtrs[(sprite->ck_sprType*5)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};


uint32_t *CK_GetSpriteGfxOffset(objsprite *sprite, int spriteid){
    if(!sprite) Quit("CK_GetSpriteGfxOffset : \nSprite does not exist!");
    if(sprite->gfxsprindx == NULL_SPRITE || gfxhandler[sprite->gfxsprindx].gfxoffset == CK_GFX_NULL) Quit("CK_GetSpriteGfxOffset : \nInvalid Graphics Memory!");

    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+gfxhandler[sprite->gfxsprindx].gfxoffset;
    for(int i = 0; i < spriteid && i < sprite->gbaSpriteCount; i++){
        vidmem += CK_SpriteSizes[sprite->sprsizes[i]];
    }
    return vidmem;
};

void CK_UpdateSprites(){
    // Fix every object position
    GBA_ClearSpriteCache();
    for(int chkpr = 3; chkpr >= 0; chkpr--){
        for(int i = 0; i < CK_NumOfSprites; i++){
            objsprite *spr = &CK_SpriteList[i];
            if(spr->priority == chkpr && spr->drawtype != 0xF)
                CK_DrawSprite(spr);
        }
    }
    // Reset this incase the sprite isn't being drawn anymore
    for(int ckcnt = 0; ckcnt < CK_NumOfGfxLocks; ckcnt ++){
        gfxhandler[ckcnt].changed = false;
    }
    GBA_UPDATE_SPRITES()
};

void CK_DrawSprite(objsprite *sprite){
    if(!sprite) return;
    if(sprite->gfxsprindx == NULL_SPRITE || gfxhandler[sprite->gfxsprindx].gfxoffset == CK_GFX_NULL) return;
    if(sprite->ck_sprType == CKS_EOL) return;

    signed int sprx = sprite->deltax - originxglobal;
    signed int spry = sprite->deltay - originyglobal;
    sprx = CONVERT_GLOBAL_TO_PIXEL(sprx);
    spry = CONVERT_GLOBAL_TO_PIXEL(spry);
    uint32_t vidmem = gfxhandler[sprite->gfxsprindx].gfxoffset;

    //sprite->rendered = false;

    for(int i = 0; i < sprite->gbaSpriteCount; i++){
        signed int chkx = sprx + CK_SpritePtrs[(sprite->ck_sprType*5)][(i*4)+1];
        signed int chky = spry + CK_SpritePtrs[(sprite->ck_sprType*5)][(i*4)+2];
        if(chkx > -64 && chky > -64 && chkx < GBA_SCREEN_WIDTH && chky < GBA_SCREEN_HEIGHT){
            int gfxtile = vidmem>>3;
            int gba_prior = GBA_SPRITE_ZTOP;
            if(sprite->priority != 3) gba_prior = GBA_SPRITE_ZMID;
            int spriteid = GBA_CreateSpriteFast(chkx,chky,sprite->sprsizes[i], gfxtile,gba_prior,sprite->drawtype);
            //sprite->rendered = true;
        }
        vidmem += CK_SpriteSizes[sprite->sprsizes[i]];
    }
    if(GBA_SpriteIndex >= 127){
        Quit("CK_SetSprite: No free GBA sprites!");
    }
};

// For undefined sprites????
const signed short CK_DUMMY_RECT[] = { 0, 0, 0, 0, 0, 0 };

signed short *CK_GetSprShape(objsprite *spr){
    if(spr == NULL || spr->ck_sprType == CKS_EOL) Quit("CK_GetSprShape : Invalid sprite for shape rect");
    //return CK_DUMMY_RECT; // ???
    signed short* ssptr = (signed short*)(CK_SpritePtrs[(spr->ck_sprType*5)+4]);
    ssptr += (spr->spritenum*6);
    return ssptr; 
};

void CK_SetupSprites(){
    // Fix some pointer stuffs
    for(int i = 0; i < MAXACTORS; i++){
        CK_ObjectList[i].sprite = NULL;
        CK_ObjectList[i].state = NULL;
    }
    for(int i  = 0 ; i < MAXSPRITES; i++){
        CK_SpriteList[i].ck_prevType= CKS_EOL;
        CK_SpriteList[i].ck_sprType= CKS_EOL;
    }

    CK_NukeObjectsSprites();

	// Tell the GBA we want sprites
    unsigned int CurReg = *(volatile unsigned int*)GBA_REG_DISPCNT;
    CurReg |= GBA_SPRITE_ENABLE | GBA_SPRITE_MAP_1D;
    *(volatile unsigned int*)GBA_REG_DISPCNT = CurReg;

};


void CK_NukeObjectsSprites(){
    
    CK_RemoveSprites();

    for(int i = 0; i < MAXACTORS; i++){
        CK_ClearObject(&CK_ObjectList[i], true);
    }
    // We removed all the objects
    CK_NumOfObjects = 0;

    player = NULL;
    scoreobj = NULL;
    ck_newobj = NULL;
};

void CK_RemoveSprites(){
    
    // Clear any software sprites
    GBA_ResetSprites();

    // Clear the sprite memory???
    GBA_DMA_MemSet32(GBA_VSRAM,0x44444444,32*32*8);

    for(int i = 0; i < MAXACTORS; i++){
        RF_RemoveSprite(&CK_ObjectList[i].sprite, false);
    }
    for(int i = 0; i < MAXSPRITES; i++){
        CK_ClearSprite(&CK_SpriteList[i], true);
        gfxhandler[i].refs = 0;
    }

    CK_NumOfSprites = 0;
    // We have no sprites anymore!
    CK_SprGfxOffset = 0;
    CK_NumOfGfxLocks = 0;
};

void CK_ResetSprGraphicsOffset(){
    // We have no sprites anymore!
    CK_SprGfxOffset = 0;
    CK_NumOfGfxLocks = 0;
};

void CK_RemoveDummySprites(){
    
    // Clear any software sprites
    GBA_ClearSpriteCache();

    for(int i = 0; i < MAXSPRITES; i++){
        CK_ClearSprite(&CK_SpriteList[i], false);
    }

    CK_NumOfSprites = 0;

    // We have no sprites yet!
    CK_SprGfxOffset = 0;
    CK_NumOfGfxLocks = 0;
};




//===========================================================================

/*
=====================
=
= RF_CalcTics
=
=====================
*/

DONT_OPTIMISE void RF_CalcTics (void)
{
	long	newtime,oldtimecount;

//
// calculate tics since last refresh for adaptive timing
//
	if (lasttimecount > TimeCount)
		TimeCount = lasttimecount;		// if the game was paused a LONG time

	if (DemoMode)					// demo recording and playback needs
	{								// to be constant
//
// take DEMOTICS or more tics, and modify Timecount to reflect time taken
//
		oldtimecount = lasttimecount;
		while (TimeCount<oldtimecount+DEMOTICS*2) {}
		lasttimecount = oldtimecount + DEMOTICS;
		TimeCount = lasttimecount + DEMOTICS;
		tics = DEMOTICS;
	}
	else
	{
//
// non demo, so report actual time
//
		// Hmmmm
		tics = MINTICS;
		do
		{
			newtime = TimeCount;
			tics = newtime-lasttimecount;
		} while (tics<MINTICS);
		lasttimecount = newtime;

		if (tics>MAXTICS)
		{
			TimeCount -= (tics-MAXTICS);
			tics = MAXTICS;
		}
	}
}


// Draws the into text screen
DONT_OPTIMISE void US_TextScreen(){
    int tick = 0;
    int nextTic = 0;
    while(!LastScan){
        if(tick == 0){
            US_ShowScreen(0);
        }
        else if(tick == 5){
            US_ShowScreen(1);
        }
        else if(tick == 6){
            US_ShowScreen(2);
        }
        else if(tick == 7){
            US_ShowScreen(1);
        }
        else if(tick == 8){
            tick = 0;
            continue;
        }
        nextTic++;
        if(nextTic > 1){
            tick++;
            nextTic = 0;
        }
        RF_CalcTics();
    }
    LastScan = 0;
};

///////////////////////////////////////////////////////////////////////////////////////

void RF_PlaceSprite (void **user,unsigned globalx,unsigned globaly,
	unsigned spritenumber, drawtype draw, int priority)
{
    objsprite *spr = *((objsprite**)user);
    
    if(!spr) {
        Quit("RF_PlaceSprite : Invalid Sprite!");
        // Get a new one!
        *(objsprite**)user = CK_GetNewSprite(CKS_EOL);
        spr = *((objsprite**)user);
    }
	if (spritenumber == -1){
        // Invalid sprite number
		RF_RemoveSprite (user, true);
        return;
    }

    spr->spritenum = spritenumber-1;

    signed short *shape = CK_GetSprShape(spr);

    signed int dx = globalx;
    signed int dy = globaly;

	dx += shape[4]*PIXGLOBAL;
	dy += shape[5]*PIXGLOBAL;

    spr->deltax = dx;
    spr->deltay = dy;
    spr->drawtype = draw;
    spr->priority = priority;

    signed int sprx = spr->deltax - originxglobal;
    signed int spry = spr->deltay - originyglobal;
    sprx = CONVERT_GLOBAL_TO_PIXEL(sprx);
    spry = CONVERT_GLOBAL_TO_PIXEL(spry);
    signed int chkx = sprx + spr->gfxwidth;
    signed int chky = spry + spr->gfxheight;

    spr->rendered = false;
    if(chkx >= 0 && chky >= 0 && sprx < GBA_SCREEN_WIDTH && spry < GBA_SCREEN_HEIGHT){
        spr->rendered = true;
    }

    if(spr->rendered == true){
        CK_FixSpriteGraphics(spr);
        CK_UpdateSpriteGraphics(spr);
    }
}

void RF_PlaceSpriteNU(void **user,unsigned globalx,unsigned globaly,
	unsigned spritenumber, drawtype draw, int priority)
{
    objsprite *spr = *((objsprite**)user);
    
    if(!spr) {
        Quit("RF_PlaceSpriteNU : Invalid Sprite!");
        // Get a new one!
        *(objsprite**)user = CK_GetNewSprite(CKS_EOL);
        spr = *((objsprite**)user);
    }
	if (spritenumber == -1){
        // Invalid sprite number
		RF_RemoveSprite (user, true);
        return;
    }

    spr->spritenum = spritenumber-1;

    signed short *shape = CK_GetSprShape(spr);

    signed int dx = globalx;
    signed int dy = globaly;

	dx += shape[4]*PIXGLOBAL;
	dy += shape[5]*PIXGLOBAL;

    spr->deltax = dx;
    spr->deltay = dy;
    spr->drawtype = draw;
    spr->priority = priority;

};


//===========================================================================

/*
=====================
=
= RF_RemoveSprite  EGA
=
=====================
*/

void RF_RemoveSprite (void **user, bool cache){
    if(user == NULL) return; // ???
    if(*user == NULL) return;
    if(cache) {
        objsprite *spr = (*(objsprite**)user);
        spr->drawtype = 0xF; // Invalid draw type
        if(spr->gfxsprindx != NULL_SPRITE){
            gfxhandler[spr->gfxsprindx].isStatic = true; // Set this
            gfxhandler[spr->gfxsprindx].changed = false;
            gfxhandler[spr->gfxsprindx].sprnum = NULL_SPRITE;
            gfxhandler[spr->gfxsprindx].refs -= 1;
        }
        return;
    }
    CK_ClearSprite(*user, false);
    *user = NULL; // :D
//    Quit("RF_RemoveSprite() : Removed Sprite!");
};



/*
=====================
=
= RF_ForceRefresh
=
=====================
*/

void RF_ForceRefresh (void)
{
    // TODO:
    // Make this actually snap to keens location
	CK_MoveCamera (originxglobal,originyglobal);
	CK_FixCamera();
	CK_ForceLevelRedraw();
}




