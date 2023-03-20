/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#include "romstuffs/CK_SpriteDefs.h"

GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS+1];
GBA_IN_EWRAM objsprite CK_SpriteList[MAXSPRITES];
unsigned int CK_NumOfSprites = 0;

unsigned int CK_NumOfObjects = 0;
unsigned int CK_SprGfxOffset = 0;
unsigned int CK_SprTileOffset = 0;

const unsigned int CK_GFX_NULL = 0xDEADBEEF;

static void CK_ClearSprite(objsprite *spr){
    if(spr==NULL) return;
    spr->deltax = spr->deltay = 0;
    spr->drawtype = 0xF;
    // Overwrite these or else bad things happen
    spr->gbaSpriteCount = 0;
    for(int i = 0; i < 16; i++)
        spr->sprsizes[i] = 0;
    spr->gfxoffset = CK_GFX_NULL;
    spr->ck_sprType = CKS_EOL;
};

static void CK_ClearObject(objtype *obj,bool overwrite){
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
    // Ummm... Will break if more than MAXACTORS are spawned... :S
    if(CK_NumOfObjects >= MAXACTORS-1){
        if (usedummy){
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

    ck_newobj->isFree = false;
    ck_newobj->removed = false;
    ck_newobj->shapenum = 0;

	ck_newobj->active = ac_yes;
	ck_newobj->needtoclip = cl_midclip;

    return ck_newobj;
};

objsprite *CK_GetNewSprite(){
    objsprite *spr = &CK_SpriteList[CK_NumOfSprites++];
    if(CK_NumOfSprites > MAXSPRITES) Quit("CK_GetNewSprite() : No free sprites!");
    CK_ClearSprite(spr);
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
    CK_ClearObject(ob, true);

};


const unsigned int CK_SpriteSizes[] = {
    (8*8)>>3, (16*16)>>3, (32*32)>>3, (64*64)>>3,
    (16*8)>>3, (32*8)>>3, (32*16)>>3, (64*32)>>3,
    (8*16)>>3, (8*32)>>3, (16*32)>>3, (32*64)>>3
};

void CK_SetSprite(objsprite **sprite, CK_SpriteType type){
    if(type == CKS_EOL) return; // Uhh
    if(!(*sprite)) {
        *sprite = CK_GetNewSprite();
    }
    objsprite *spr = *sprite;

    spr->ck_sprType = type;
    spr->gbaSpriteCount = *CK_SpritePtrs[(type*5)+1];
    spr->gfxoffset = CK_SprGfxOffset;
    if(CK_SprGfxOffset >= 0x8000){
        Quit("CK_SetSprite : No free graphics space left!");
    }
    for(int i = 0; i < spr->gbaSpriteCount; i++){
        spr->sprsizes[i] = CK_SpritePtrs[(spr->ck_sprType*5)][(i*4)];
    }
    CK_SprGfxOffset += (*CK_SpritePtrs[(spr->ck_sprType*5)+3])>>3;

};


void CK_RemakeSprite(objsprite **spr, CK_SpriteType type){
    if(!spr || !(*spr)) return;
    // Remove sprite
    CK_ClearSprite(*spr);
    // Create new one
    CK_SetSprite(spr, type);
};


void CK_UpdateSpriteGraphics(objsprite *sprite){
    if(!sprite) return;
    if(sprite->ck_sprType == CKS_EOL) return;
    if(sprite->gfxoffset == CK_GFX_NULL) Quit("CK_UpdateSpriteGraphics : \nInvalid Graphics Memory used!");
    
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+sprite->gfxoffset;

    for(int i = 0; i < sprite->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[sprite->sprsizes[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(sprite->ck_sprType*5)+2]+(sprsize*sprite->spritenum) + (CK_SpritePtrs[(sprite->ck_sprType*5)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};

uint32_t *CK_GetSpriteGfxOffset(objsprite *sprite, int spriteid){
    if(!sprite) Quit("CK_GetSpriteGfxOffset : \nSprite does not exist!");
    if(sprite->gfxoffset == CK_GFX_NULL) Quit("CK_GetSpriteGfxOffset : \nInvalid Graphics Memory!");

    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+sprite->gfxoffset;
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
    GBA_UPDATE_SPRITES()
};

void CK_DrawSprite(objsprite *sprite){
    if(!sprite) return;
    if(sprite->gfxoffset == CK_GFX_NULL) return;
    if(sprite->ck_sprType == CKS_EOL) return;
    signed int sprx = sprite->deltax - originxglobal;
    signed int spry = sprite->deltay - originyglobal;
    sprx = CONVERT_GLOBAL_TO_PIXEL(sprx);
    spry = CONVERT_GLOBAL_TO_PIXEL(spry);
    uint32_t vidmem = sprite->gfxoffset;
    for(int i = 0; i < sprite->gbaSpriteCount; i++){
        signed int chkx = sprx + CK_SpritePtrs[(sprite->ck_sprType*5)][(i*4)+1];
        signed int chky = spry + CK_SpritePtrs[(sprite->ck_sprType*5)][(i*4)+2];
        if(chkx > -64 && chky > -64 && chkx < GBA_SCREEN_WIDTH && chky < GBA_SCREEN_HEIGHT){
            int gfxtile = vidmem>>3;
            int gba_prior = GBA_SPRITE_ZTOP;
            if(sprite->priority != 3) gba_prior = GBA_SPRITE_ZMID;
            int spriteid = GBA_CreateSprite(chkx,chky,sprite->sprsizes[i], gfxtile,gba_prior,sprite->drawtype);
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
    if(spr == NULL ) Quit("CK_GetSprShape : Invalid sprite for shape rect");
    //return CK_DUMMY_RECT; // ???
    return (signed short*)(CK_SpritePtrs[(spr->ck_sprType*5)+4]) + (spr->spritenum*6);
};

void CK_SetupSprites(){

    CK_RemoveSprites();

	// Tell the GBA we want sprites
    unsigned int CurReg = *(volatile unsigned int*)GBA_REG_DISPCNT;
    CurReg |= GBA_SPRITE_ENABLE | GBA_SPRITE_MAP_1D;
    *(volatile unsigned int*)GBA_REG_DISPCNT = CurReg;

};

void CK_RemoveSprites(){
    
    // Clear any software sprites
    GBA_ResetSprites();

    // Clear the sprite memory???
    GBA_DMA_MemSet32(GBA_VSRAM,0,32*32*4);

    for(int i = 0; i < MAXACTORS; i++){
        CK_ClearObject(&CK_ObjectList[i], true);
        CK_ClearSprite(&CK_SpriteList[i]);
    }

    CK_NumOfSprites = 0;

    // We have no sprites yet!
    CK_SprGfxOffset = 0;
    CK_NumOfObjects = 0;
    CK_SprTileOffset = 0;
};


void CK_RemoveDummySprites(){
    
    // Clear any software sprites
    GBA_ClearSpriteCache();

    for(int i = 0; i < MAXACTORS; i++){
        CK_ClearSprite(&CK_SpriteList[i]);
    }

    CK_NumOfSprites = 0;

    // We have no sprites yet!
    CK_SprGfxOffset = 0;
    CK_NumOfObjects = 0;
    CK_SprTileOffset = 0;
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
		while (TimeCount<oldtimecount+DEMOTICS*2);
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
        // Get a new one!
        *(objsprite**)user = CK_GetNewSprite();
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

    CK_UpdateSpriteGraphics(spr);
}

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
        (*(objsprite**)user)->drawtype = 0xF; // Invalid draw type
        return;
    }
    CK_ClearSprite(*user);
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
	CK_MoveCamera (originxglobal,originyglobal);
	CK_FixCamera();
	CK_ForceLevelRedraw();
}




