/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

#include "romstuffs/CK_SpriteDefs.h"

GBA_IN_EWRAM objtype CK_ObjectList[MAXACTORS];
unsigned int CK_NumOfObjects = 0;
unsigned int CK_SprGfxOffset = 0;
unsigned int CK_SprTileOffset = 0;

objtype dummyobj;

DONT_OPTIMISE objtype *GetNewObj(boolean dummy){
    bool foundobj = false;
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(obj->isFree) {
            ck_newobj = obj;
            foundobj = true;
            break;
        }
    }
    if(!foundobj){
        // Ummm... Will break if more than MAX_OBJS are spawned... :S
        ck_newobj = &CK_ObjectList[CK_NumOfObjects++];
        if(CK_NumOfObjects > MAXACTORS){
            Quit("GetNewObj: No free spots in objarray!");
        }
        ck_newobj->gfxoffset = 0xFFFFFFFF;
        ck_newobj->ck_sprType = CKS_EOL;
        ck_newobj->gbaSpriteCount = 0;
        ck_newobj->uuid = CK_NumOfObjects-1;
    }
    ck_newobj->isFree = false;
    ck_newobj->shapenum = 0;

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
        // TODO:
        // Handle stuned objects???
        Quit("RemoveObj() : Temp Sprite Removed!");
		RF_RemoveSprite(ob->temp3);
	}

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
    objtype *gfxobj = NULL;
    // See if an old object already has the needed sprite space
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *oldobj = &CK_ObjectList[i];
        if(oldobj != obj)
            if(!oldobj->isFree) continue;
        if(oldobj->ck_sprType != type) continue;
        if(oldobj->gfxoffset == 0xFFFFFFFF) continue;
        gfxobj = oldobj;
    }
    obj->ck_sprType = type;
    obj->isFree = false; // ???
    if(gfxobj != NULL){
        obj->gbaSpriteCount = *CK_SpritePtrs[(type*5)+1];
        obj->gfxoffset = gfxobj->gfxoffset;
        for(int i = 0; i < obj->gbaSpriteCount; i++){
            obj->sprtype[i] = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)];
            obj->gbaSprites[i] = GBA_CreateSprite(GBA_SPR_OFFX,GBA_SPR_OFFY,obj->sprtype[i],CK_SprTileOffset,0,0);
            CK_SprTileOffset += CK_SpriteSizes[obj->sprtype[i]]>>3;
        }
        return;
    }
    // Only add new sprites if needed???
  //  if(ck_newobj->gfxoffset == 0xFFFFFFFF && obj->isFree == false){
        obj->gbaSpriteCount = *CK_SpritePtrs[(type*5)+1];
        obj->gfxoffset = CK_SprGfxOffset;
        for(int i = 0; i < obj->gbaSpriteCount; i++){
            obj->sprtype[i] = CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)];
            obj->gbaSprites[i] = GBA_CreateSprite(GBA_SPR_OFFX,GBA_SPR_OFFY,obj->sprtype[i],CK_SprTileOffset,0,0);
            CK_SprTileOffset += CK_SpriteSizes[obj->sprtype[i]]>>3;
        }
        CK_SprGfxOffset += (*CK_SpritePtrs[(obj->ck_sprType*5)+3])>>3;
    //}
};


void CK_RemakeSprite(objtype *obj, CK_SpriteType type){
    // Remove all GBA sprites
     for(int i = 0; i < obj->gbaSpriteCount; i++){
        GBA_RemoveSprite(obj->gbaSprites[i]);
     }
//    obj->gfxoffset = 0xFFFFFFFF;
    obj->isFree = true;
    obj->gbaSpriteCount = 0;
//    obj->ck_sprType = CKS_EOL;
    // Create new ones
    CK_SetSprite(obj, type);
    // Update the GBA
    GBA_UPDATE_SPRITES()
};

void CK_SetDummySprite(objtype *obj){
    obj->ck_sprType = CKS_EOL;
    obj->gbaSpriteCount = 0;
    obj->gfxoffset = 0;
    obj->shapenum = 0;
};

void CK_UpdateObjGraphics(objtype *obj){
    if(!obj) return;
    if(obj->isFree) return;
    if(obj->ck_sprType == CKS_EOL) return;
    
    // Copy the graphics memory
    uint32_t* vidmem = (uint32_t*)GBA_VSRAM+obj->gfxoffset;

    uint32_t blocksize = (32*16)>>3;//CK_SpritePtrs[(obj->ck_sprType<<2)][(i*4)+3]<<3;
    for(int i = 0; i < obj->gbaSpriteCount; i++){
        uint32_t sprsize = CK_SpriteSizes[obj->sprtype[i]];
        uint32_t* sprmem = (uint32_t*)CK_SpritePtrs[(obj->ck_sprType*5)+2]+(sprsize*obj->shapenum) + (CK_SpritePtrs[(obj->ck_sprType*5)][(i*4)+3]<<3);
        GBA_DMA_Copy32(vidmem, sprmem, sprsize);
        vidmem += sprsize;
    }
};

void CK_UpdateObjects(){
    // Fix every object position
    for(int i = 0; i < CK_NumOfObjects; i++){
        objtype *obj = &CK_ObjectList[i];
        if(!obj) return;
        if(obj->isFree) return;
        if(obj->ck_sprType == CKS_EOL) return;

        CK_UpdateObjGraphics(obj);

        signed int dx = obj->deltax;
        signed int dy = obj->deltay;

        signed short *shape = CK_GetShape(obj->ck_sprType, obj->shapenum);

        dx -= originxglobal;
        dy -= originyglobal;

        dx = CONVERT_GLOBAL_TO_PIXEL(dx);
        dy = CONVERT_GLOBAL_TO_PIXEL(dy);

        dx += shape[4];
        dy += shape[5];

        CK_DrawObject(obj, dx, dy);
    }
    // Update the sprite list
    GBA_UPDATE_SPRITES()
};

void CK_DrawObject(objtype *obj, signed int dx, signed int dy){
    if(!obj) return;
    if(obj->isFree) return;
    if(obj->ck_sprType == CKS_EOL) return;
   
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
    if(obj == NULL || obj->isFree || (unsigned int)obj->ck_sprType>=CKS_EOL) return CK_DUMMY_RECT; // ???
    return (signed short*)(CK_SpritePtrs[(obj->ck_sprType*5)+4]) + (obj->shapenum*6);
};

signed short *CK_GetShape(unsigned int sprtype, unsigned short shapenumber){
    if(sprtype==-1) return CK_DUMMY_RECT; // ???
    return (signed short*)(CK_SpritePtrs[(sprtype*5)+4]) + (shapenumber*6);
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
        CK_ObjectList[i].isFree = true;
        CK_ObjectList[i].gfxoffset = 0xFFFFFFFF;
        CK_ObjectList[i].ck_sprType = CKS_EOL;
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
    /*
	if (!spritenumber || spritenumber == (unsigned)-1)
	{
		RF_RemoveSprite (user);
		return;
	}*/
    if(!obj) return;
    if(obj->isFree) return;
    if(obj->ck_sprType == CKS_EOL) return;

    CK_UpdateObjGraphics(obj);

    obj->deltax = globalx;
    obj->deltay = globaly;
    obj->drawtype = draw;

    signed int dx = globalx;
    signed int dy = globaly;    

    signed short *shape = CK_GetShape(obj->ck_sprType, spritenumber);

    dx -= originxglobal;
    dy -= originyglobal;

    dx = CONVERT_GLOBAL_TO_PIXEL(dx);
    dy = CONVERT_GLOBAL_TO_PIXEL(dy);

	dx += shape[4];
	dy += shape[5];

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
    // Remove the GBA sprites???
     for(int i = 0; i < ob->gbaSpriteCount; i++){
        GBA_RemoveSprite(ob->gbaSprites[i]);
     }
//    ob->gfxoffset = 0xFFFFFFFF;
    ob->isFree = true;
//    ob->gbaSpriteCount = 0;
//    ob->ck_sprType = CKS_EOL;
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




