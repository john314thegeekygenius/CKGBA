/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#include "romstuffs/CK_SpriteDefs.h"

GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS+1];

unsigned int CK_NumOfObjects = 0;
unsigned int CK_SprGfxOffset = 0;
unsigned int CK_SprTileOffset = 0;

const unsigned int CK_GFX_NULL = 0xDEADBEEF;


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
    obj->deltax = obj->deltay = 0;
    obj->drawtype = 0xF;
    // Overwrite these or else bad things happen
    obj->gbaSpriteCount = 0;
    for(int i = 0; i < 16; i++)
        obj->sprsizes[i] = 0;
    if(overwrite){
        obj->uuid = 0;
        obj->removed = true;
        // Hmmm
        obj->isFree = true;
        obj->gfxoffset = CK_GFX_NULL;
        obj->ck_sprType = CKS_EOL;
        ck_newobj->sprAvailable = 0;
    }
};

DONT_OPTIMISE objtype *GetNewObj(boolean usedummy){
    bool foundobj = false;/*
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(obj->removed) {
            CK_ClearObject(obj, false);
            ck_newobj = obj;
            foundobj = true;
            break;
        }
    }*/
    if(!foundobj){
        // Ummm... Will break if more than MAXACTORS are spawned... :S
        if(CK_NumOfObjects >= MAXACTORS-1){
            if (usedummy){
                ck_newobj = &CK_ObjectList[MAXACTORS-1]; // Dummy obj stored at end of list???
                ck_newobj->removed = false;
                ck_newobj->gfxoffset = CK_GFX_NULL;
                ck_newobj->ck_sprType = CKS_EOL;
                ck_newobj->gbaSpriteCount = 0;
                ck_newobj->uuid = MAXACTORS-1;
                ck_newobj->isFree = false;
                ck_newobj->removed = false;
                ck_newobj->shapenum = 0;
                ck_newobj->sprAvailable = 0;
                return ck_newobj;
            }
            Quit("GetNewObj: No free spots in objarray!");
        }
        ck_newobj = &CK_ObjectList[CK_NumOfObjects++];
        ck_newobj->gfxoffset = CK_GFX_NULL;
        ck_newobj->ck_sprType = CKS_EOL;
        ck_newobj->drawtype = 0xF;
        ck_newobj->gbaSpriteCount = 0;
        ck_newobj->uuid = CK_NumOfObjects-1;
    }
    ck_newobj->isFree = false;
    ck_newobj->removed = false;
    ck_newobj->shapenum = 0;
    ck_newobj->sprAvailable = 0;

	ck_newobj->active = ac_yes;
	ck_newobj->needtoclip = cl_midclip;

    return ck_newobj;
};

void RemoveObj(objtype *ob){
	if (ob == player)
		Quit("RemoveObj: Tried to remove the player!");

    if(!ob) return; // ????
//
// erase it from the refresh manager
//
	RF_RemoveSprite(ob);
	if (ob->obclass == stunnedobj)
	{
		RF_RemoveSprite(ob->temp3);
	}
    ob->sprAvailable = 1;
    ob->removed = true;
    ob->drawtype = 0xF;

};

objtype * CK_GetObj(int index){
    return &CK_ObjectList[index];
};

const unsigned int CK_SpriteSizes[] = {
    (8*8)>>3, (16*16)>>3, (32*32)>>3, (64*64)>>3,
    (16*8)>>3, (32*8)>>3, (32*16)>>3, (64*32)>>3,
    (8*16)>>3, (8*32)>>3, (16*32)>>3, (32*64)>>3
};

void CK_SetSprite(objtype *obj, CK_SpriteType type){
    if(!obj) return;

    obj->ck_sprType = type;
    obj->isFree = false; // ???
    obj->sprAvailable = 0;
    // Only add new sprites if needed???
  //  if(ck_newobj->gfxoffset == CK_GFX_NULL && obj->isFree == false){
    obj->gbaSpriteCount = *CK_SpritePtrs[(type*5)+1];
    obj->gfxoffset = CK_SprGfxOffset;
    if(CK_SprGfxOffset >= 0x8000){
        Quit("CK_SetSprite : No free graphics space left!");
    }
    for(int i = 0; i < obj->gbaSpriteCount; i++){
        obj->sprsizes[i] = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)];
        obj->gbaSprites[i] = GBA_CreateSprite(GBA_SPR_OFFX,GBA_SPR_OFFY,obj->sprsizes[i],CK_SprTileOffset,0,0);
        CK_SprTileOffset += CK_SpriteSizes[obj->sprsizes[i]]>>3;
        if(GBA_SpriteIndex >= 127){
            Quit("CK_SetSprite: No free GBA sprites!");
        }
    }
    CK_SprGfxOffset += (*CK_SpritePtrs[(obj->ck_sprType*5)+3])>>3;
    //}

};


void CK_RemakeSprite(objtype *obj, CK_SpriteType type){
    if(!obj) return;
    // Remove all GBA sprites
     for(int i = 0; i < obj->gbaSpriteCount; i++){
        GBA_RemoveSprite(obj->gbaSprites[i]);
     }
//    obj->gfxoffset = CK_GFX_NULL;
    obj->isFree = true;
//    obj->gbaSpriteCount = 0;
//    obj->ck_sprType = CKS_EOL;
    // Create new ones
    CK_SetSprite(obj, type);
    // Update the GBA
    GBA_UPDATE_SPRITES()
};

void CK_SetDummySprite(objtype *obj){
    obj->ck_sprType = CKS_EOL;
    obj->gbaSpriteCount = 0;
    obj->gfxoffset = CK_GFX_NULL;
    obj->drawtype = 0xF;
    obj->sprAvailable = 0;
    obj->shapenum = 0;
};

void CK_UpdateObjGraphics(objtype *obj){
    if(!obj) return;
    if(obj->isFree) return;
    if(obj->ck_sprType == CKS_EOL) return;
    if(obj->gfxoffset == CK_GFX_NULL) Quit("CK_UpdateObjGraphics : \nInvalid Graphics Memory used!");
    
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+obj->gfxoffset;

    for(int i = 0; i < obj->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[obj->sprsizes[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(obj->ck_sprType*5)+2]+(sprsize*obj->shapenum) + (CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};

uint32_t *CK_GetObjGfxOffset(objtype *obj, int spriteid){
    if(!obj) Quit("CK_GetObjGfxOffset : \nObject does not exist!");
    if(obj->gfxoffset == CK_GFX_NULL) Quit("CK_GetObjGfxOffset : \nInvalid Graphics Memory!");
    
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+obj->gfxoffset;

    for(int i = 0; i < spriteid && i < obj->gbaSpriteCount; i++){
        vidmem += CK_SpriteSizes[obj->sprsizes[i]];
    }
    return vidmem;
};

void CK_UpdateObjects(){
    // TODO:
    // Fix this function!!!!!!
    // Makes the app crash ??
    return;

    // Fix every object position
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(obj->removed) continue;
        if(obj->isFree) continue;
        if(obj->ck_sprType == CKS_EOL) continue;
        if(obj->drawtype == 0xF) continue;

        CK_UpdateObjGraphics(obj);

        signed int dx = obj->deltax;
        signed int dy = obj->deltay;

        dx -= originxglobal;
        dy -= originyglobal;

        dx = CONVERT_GLOBAL_TO_PIXEL(dx);
        dy = CONVERT_GLOBAL_TO_PIXEL(dy);

        CK_DrawObject(obj, dx, dy);
    }
};

void CK_DrawObject(objtype *obj, signed int dx, signed int dy){
    if(!obj) return;
    if(obj->removed) return;
    if(obj->isFree) return;
    if(obj->ck_sprType == CKS_EOL) return;

    /*
        TODO:
        Make this "spawn" gba sprites in realtime
        depending on what can be seen on the screen
        at any one frame.

        Object can store a "dummy" gba sprite that can be loaded into a real gba sprite
    */
   
    for(int i = 0; i < obj->gbaSpriteCount; i++){
        // Make the object work in global map space
        signed int sprx = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+1] + (dx) ;// + obj->state->xmove;
        signed int spry = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+2] + (dy) ;// + obj->state->ymove;

        // Make sure we can even see the sprite
        unsigned short gbax = (sprx+512)&0x1FF;
        unsigned short gbay = (spry+256)&0xFF;
        if(sprx > -64 && spry > -64 && sprx < GBA_SCREEN_WIDTH && spry < GBA_SCREEN_HEIGHT && 
            (gbax < 0x100 || gbax > 0x1C0) && (gbay < 0xA0 || gbay > 0xC0)){
            if(obj->drawtype != spritedraw){
                GBA_SET_SPRITE_PAL(obj->gbaSprites[i], 15)
            }else{
                GBA_SET_SPRITE_PAL(obj->gbaSprites[i], 0)
            }
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], gbax, gbay)
            GBA_UPDATE_SPRITE(obj->gbaSprites[i])
        }else{
            GBA_SET_SPRITE_POSITION(obj->gbaSprites[i], GBA_SPR_OFFX, GBA_SPR_OFFY)
            GBA_UPDATE_SPRITE(obj->gbaSprites[i])
        }
    }
};

void CK_PrintObjInfo(){
    PrintX = 0;
    PrintY = -8;
    VW_Bar(PrintX,PrintY,240,8, CK_EGA_WHITE);
    US_PrintUnsigned(CK_NumOfObjects);
       
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(obj->isFree) continue;
        // ID & Position
        PrintX = 0;
        PrintY = (i+1)*24;
        VW_Bar(PrintX,PrintY,240,8, CK_EGA_WHITE);
        US_PrintUnsigned(obj->obclass);
        PrintX += 16;
        US_PrintUnsigned(obj->x>>G_T_SHIFT);
        PrintX += 16;
        US_PrintUnsigned(obj->y>>G_T_SHIFT);
        PrintX += 16;
        US_PrintUnsigned(obj->x);
        PrintX += 16;
        US_PrintUnsigned(obj->y);
        // Hitbox stuff
        PrintX = 0;
        PrintY = 8+((i+1)*24);
        VW_Bar(PrintX,PrintY,240,8, CK_EGA_YELLOW);
        US_PrintUnsigned(obj->left);
        PrintX += 8;
        US_PrintUnsigned(obj->top);
        PrintX += 8;
        US_PrintUnsigned(obj->right);
        PrintX += 8;
        US_PrintUnsigned(obj->bottom);

    	signed short *shape = CK_GetSprShape(obj);
        PrintX = 0;
        PrintY = 16+((i+1)*24);
        VW_Bar(PrintX,PrintY,240,8, CK_EGA_BROWN);
        US_PrintSigned(shape[0]);
        PrintX += 8;
        US_PrintSigned(shape[1]);
        PrintX += 8;
        US_PrintSigned(shape[2]);
        PrintX += 8;
        US_PrintSigned(shape[3]);
        PrintX += 8;
        US_PrintSigned(shape[4]);
        PrintX += 8;
        US_PrintSigned(shape[5]);
    }
    // Camera stuff
    PrintX = 0;
    PrintY = 0;
    US_PrintSigned(originxmax);
    PrintX += 8;
    US_PrintSigned(originymax);
    PrintX = 0;
    PrintY = 8;
    US_PrintSigned(originxglobal);
    PrintX += 8;
    US_PrintSigned(originyglobal);
    
    // Controls
    PrintX = 12;
    PrintY = 16;
    VW_Bar(PrintX,PrintY,240,8, CK_EGA_CLEAR);
    US_PrintUnsigned(jumpbutton);
    PrintX += 8;
    US_PrintUnsigned(firebutton);


};

// For undefined sprites????
const signed short CK_DUMMY_RECT[] = { 0, 0, 0, 0, 0, 0 };

signed short *CK_GetSprShape(objtype *obj){
    if(obj == NULL || obj->removed ) Quit("CK_GetSprShape : Invalid object for shape rect");
    //return CK_DUMMY_RECT; // ???
    return (signed short*)(CK_SpritePtrs[(obj->ck_sprType*5)+4]) + (obj->shapenum*6);
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
    }

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
void US_TextScreen(){
	
};

///////////////////////////////////////////////////////////////////////////////////////

void RF_PlaceSprite (void *user,unsigned globalx,unsigned globaly,
	unsigned spritenumber, drawtype draw, int priority)
{
    objtype *obj = (objtype*)user;
    
    if(!obj) return;
	if (spritenumber == CKS_EOL)
	{
		RF_RemoveSprite (obj);
		return;
	}
    obj->isFree = false;

    CK_UpdateObjGraphics(obj);

    signed short *shape = CK_GetSprShape(obj);

    signed int dx = globalx;
    signed int dy = globaly;    

	dx += shape[4]<<G_P_SHIFT;
	dy += shape[5]<<G_P_SHIFT;

    obj->deltax = dx;
    obj->deltay = dy;
    obj->drawtype = draw;

    dx -= originxglobal;
    dy -= originyglobal;

    dx = CONVERT_GLOBAL_TO_PIXEL(dx);
    dy = CONVERT_GLOBAL_TO_PIXEL(dy);

    CK_DrawObject(obj, dx, dy);

}

//===========================================================================

/*
=====================
=
= RF_RemoveSprite  EGA
=
=====================
*/

void RF_RemoveSprite (void *user){
    objtype *ob = (objtype*)user;
    if(!ob) return;
    if(ob->removed) return; // ???
    if(ob->sprAvailable == 1) return; // Don't touch them???
    // Hide the GBA sprites so we can use them later??
     for(int i = 0; i < ob->gbaSpriteCount; i++){
        GBA_HideSprites(ob->gbaSprites[i]);
     }
    ob->isFree = true;
    ob->drawtype = 0xF;
    GBA_UPDATE_SPRITES()
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




