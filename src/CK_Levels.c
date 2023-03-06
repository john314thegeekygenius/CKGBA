/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"
#ifdef CK4
#include "romstuffs/CK4_LDefs.h"
#endif
#ifdef CK5
#include "romstuffs/CK5_LDefs.h"
#endif
#ifdef CK6
#include "romstuffs/CK6_LDefs.h"
#endif

extern const unsigned char CK_TILESET_MASKED[];
extern const unsigned char CK_TILESET_UNMASKED[];


void RFL_ClearScrollBlocks (void);
void RF_SetScrollBlock (int x, int y, boolean horizontal);

int		hscrollblocks,vscrollblocks;
int		hscrolledge[MAXSCROLLEDGES],vscrolledge[MAXSCROLLEDGES];


void CK_SetupLevelGBAMaps(){
	// Set the map to a constant 
	for(int i = 0; i < 32*32; i++){
		*(uint16_t*)(TILEMAP_0+i) = i;
		*(uint16_t*)(TILEMAP_1+i) = i+0xC0;
	}
	
	// Finish the render of the background
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | TILEMAP_MAP_0 | TILEMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | TILEMAP_MAP_1 | TILEMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
};



unsigned short CK_CurLevelWidth = 0;
unsigned short CK_CurLevelHeight = 0;
unsigned short CK_CurLevelSize = 0;
unsigned short CK_CurLevelIndex = 0;

struct CK_TileAnimation {
    uint16_t map_offset;
    uint16_t tile_to;
    signed short ani_time;
};

// Must be in External Work Ram or else it's too much for the 32K on board
GBA_IN_EWRAM uint16_t CK_CurLevelData[32768]; // Use 64K of memory for the levels (Max level size of 128x128)
GBA_IN_EWRAM struct CK_TileAnimation CK_CurLevelAnimations[2048][2]; // Use 6K or memory for level animations [offset, newtile, time]
//GBA_IN_EWRAM uint16_t CK_UpdatePOI[32*20][2]; // Tiles to update on the screen
//unsigned int CK_POICount = 0; // How many POI points are there?

const char *CK_TileInfo[2] = { CK_TileInfoBG, CK_TileInfoFG };


unsigned int CK_NumOfAnimations[2];

signed int CK_GlobalCameraX = 0;
signed int CK_GlobalCameraY = 0;

signed int CK_GlobalCameraLX = 0;
signed int CK_GlobalCameraLY = 0;

uint32_t CK_CameraX = 0;
uint32_t CK_CameraY = 0;

signed int CK_CameraBlockX = 0;
signed int CK_CameraBlockY = 0;
bool CK_CameraMoved = false;

const int CK_CameraWidth = 30>>1;
const int CK_CameraHeight = 20>>1;

bool CK_UpdateRendering = false;

uint32_t CK_UpdateTick = 0;

#include "romstuffs/CK4_Spectators.c"

void CK_ForceLevelRedraw(){
    CK_UpdateRendering = true;
    CK_RenderLevel();
};

void CK_LoadLevel(unsigned short lvlNumber){
	int lvloff = lvlNumber*3;
	CK_CurLevelIndex = lvlNumber;
	CK_CurLevelWidth = *CK_LevelInfo[lvloff];
	CK_CurLevelHeight = *CK_LevelInfo[lvloff+1];
	// Precalculate the level size for SPEEEEEEED!!!!
	CK_CurLevelSize = CK_CurLevelWidth * CK_CurLevelHeight;

    // Remove any animations
    CK_NumOfAnimations[0] = CK_NumOfAnimations[1] = 0;

    GBA_DMA_MemSet16((uint16_t*)CK_CurLevelAnimations, 0, sizeof(CK_CurLevelAnimations));
//    GBA_DMA_MemSet16((uint16_t*)CK_UpdatePOI, 0, sizeof(CK_UpdatePOI));
//    CK_POICount = 0;



    for(int p = 0; p < 2; p++){
        for(int y = 0; y < CK_CurLevelHeight; y++){
            for(int x = 0; x < CK_CurLevelWidth; x++){
                const uint32_t offset = (y*CK_CurLevelWidth)+x+(CK_CurLevelSize*p);
                // Copy the level data over
                uint32_t tileoff = CK_CurLevelData[offset] = CK_LevelInfo[lvloff+2][offset];
                // Setup any animations 
                uint8_t animation_time;
                if(p==0) {
                    animation_time = CK_TileInfo[p][tileoff];
                }
                if(p==1){ 
                    animation_time = CK_TileInfo[p][tileoff+(CK_TileInfo_FGTiles*6)];
                }
                
                if(animation_time){
                    CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].map_offset = offset;
                    CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].ani_time = animation_time;

                    if(p == 0){
                        CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].tile_to = (signed char)CK_TileInfo[p][tileoff+CK_TileInfo_BGTiles];
                    }
                    if(p == 1){
                        CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].tile_to = (signed char)CK_TileInfo[p][tileoff+(CK_TileInfo_FGTiles*4)];
                    }

                    ++CK_NumOfAnimations[p];
                }
            }
        }
    }

    // Tell the engine to render the level
    CK_UpdateRendering = true;



//
// the y max value clips off the bottom half of a tile so a map that is
// 13 + MAPBORDER*2 tile high will not scroll at all vertically
//
	originxmax = (CK_CurLevelWidth-MAPBORDER-SCREENTILESWIDE)*TILEGLOBAL;
	originymax = (CK_CurLevelHeight-MAPBORDER-SCREENTILESHIGH)*TILEGLOBAL;
	if (originxmax<originxmin)		// for very small maps
		originxmax=originxmin;
	if (originymax<originymin)
		originymax=originymin;

//
// clear out the lists
//
	RFL_ClearScrollBlocks ();
	RF_SetScrollBlock (0,MAPBORDER-1,true);
	RF_SetScrollBlock (0,CK_CurLevelHeight-MAPBORDER,true);
	RF_SetScrollBlock (MAPBORDER-1,0,false);
	RF_SetScrollBlock (CK_CurLevelWidth-MAPBORDER,0,false);


    tics = 1;

};

void CK_MoveCamera(signed int x, signed int y){
    originxglobal = 0;
    originyglobal = 0;
    // Move the camera in chunks so it "clips" correctly
    if(x > 0){
        while(x > 0xFF){
            RFL_BoundScroll(0xFF,0);
            x -= 0xFF;
        }
    }else{
        while(x < -0xFF){
            RFL_BoundScroll(-0xFF,0);
            x += 0xFF;
        }
    }
    if(y > 0){
        while(y > 0xFF){
            RFL_BoundScroll(0,0xFF);
            y -= 0xFF;
        }
    }else{
        while(y < -0xFF){
            RFL_BoundScroll(0,-0xFF);
            y += 0xFF;
        }
    }
    RFL_BoundScroll(x,y);
    CK_CameraMoved = true;
};

void CK_FixCamera(){

    CK_GlobalCameraX = CONVERT_GLOBAL_TO_PIXEL(originxglobal);
    CK_GlobalCameraY = CONVERT_GLOBAL_TO_PIXEL(originyglobal);

    if(CK_GlobalCameraLX != CK_GlobalCameraX || CK_GlobalCameraLY != CK_GlobalCameraY){
        CK_CameraMoved = true;
    }

    CK_GlobalCameraLX = CK_GlobalCameraX;
    CK_GlobalCameraLY = CK_GlobalCameraY;

    uint32_t CK_NCameraX = CK_GlobalCameraX%16;
    uint32_t CK_NCameraY = CK_GlobalCameraY%16;

    if(CK_CameraX != CK_NCameraX){
        if((CK_NCameraX==0|| CK_NCameraX==15)) CK_UpdateRendering = true;
    }
    if(CK_CameraY != CK_NCameraY){
        if((CK_NCameraY==0 || CK_NCameraY==15)) CK_UpdateRendering = true;
    }

    CK_CameraX = CK_NCameraX;
    CK_CameraY = CK_NCameraY;

    CK_CameraBlockX = CONVERT_GLOBAL_TO_TILE(originxglobal);
    CK_CameraBlockY = CONVERT_GLOBAL_TO_TILE(originyglobal);

};


void CK_ScrollCamera(signed int x, signed int y ){
    RFL_BoundScroll(x,y);
};

#define UMTILESET_WIDTH_VAL (288)
#define MTILESET_WIDTH_VAL (288)

#define UMTILESET_WIDTH_VALD (UMTILESET_WIDTH_VAL<<1)
#define MTILESET_WIDTH_VALD (MTILESET_WIDTH_VAL<<1)

#define CKTILEBG(x,y) (((y)*(UMTILESET_WIDTH_VALD)) + ((x)<<4))
#define CKTILEFG(x,y) (((y)*(MTILESET_WIDTH_VALD)) + ((x)<<4))

GBA_IN_IWRAM uint32_t *CK_BlitBuffer[16*11][2];

void CK_RenderLevel(){
    // Removed because it's useless
//	if(CK_CurLevelData == NULL) return; // Nothing to do

    volatile uint32_t *vptr = (volatile uint32_t *)TILESTART_0;
    volatile uint32_t *vptr2 = (volatile uint32_t *)TILESTART_1;
    volatile uint32_t *tileptr = (volatile uint32_t *)CK_TILESET_UNMASKED+128;
    
    uint32_t doffset;
    uint16_t keenTile;
    uint32_t tileY, tileX;
    uint32_t BlitTile;

    if(CK_UpdateRendering) {

        // Load the tiles in real time
        doffset = ((CK_CameraBlockY)*CK_CurLevelWidth)+CK_CameraBlockX;
        const unsigned int VSHIFT = (32<<3);
        // Pregenerate a new map
        for(int i = 0; i < 11; ++i){
            for(int e = 0; e < 16; ++e){
                keenTile = CK_CurLevelData[doffset];
                tileY = (keenTile/18);
                tileX = keenTile%18;
                BlitTile = CKTILEBG(tileX,tileY);

                // Copy the background tiles
                CK_BlitBuffer[(i*16)+e][0] = (volatile uint32_t *)CK_TILESET_UNMASKED+BlitTile;

                keenTile = CK_CurLevelData[doffset+CK_CurLevelSize];
//                if(keenTile==0) goto skipdraw2; // Skip blank tiles???????
                tileY = (keenTile/18);
                tileX = keenTile%18;
                BlitTile = CKTILEFG(tileX,tileY);

                // Copy the foreground tiles
                CK_BlitBuffer[(i*16)+e][1] = (volatile uint32_t *)CK_TILESET_MASKED+BlitTile;
                // If it covers sprites, draw it in the foreground, else background
//                if(CK_TileInfoFG[keenTile+(CK_TileInfo_FGTiles*5)&0x80]){
/*                }else{
                    GBA_DMA_Copy32(vptr, tileptr, 16);
                    GBA_DMA_Copy32(vptr+(32<<3), tileptr+(MTILESET_WIDTH_VAL), 16);
                }*/
                ++doffset;
            }
            doffset += (CK_CurLevelWidth-16);
        }

        GBA_WAIT_FOR_VBLANKN(160)

        // Move the camera if need be
        if(CK_CameraMoved){
            CK_CameraMoved = false;
            *(volatile uint16_t*)GBA_REG_BG0HOFS = (uint16_t)CK_CameraX&0x1FF;
            *(volatile uint16_t*)GBA_REG_BG0VOFS = (uint16_t)CK_CameraY&0x1FF;

            *(volatile uint16_t*)GBA_REG_BG1HOFS = (uint16_t)CK_CameraX&0x1FF;
            *(volatile uint16_t*)GBA_REG_BG1VOFS = (uint16_t)CK_CameraY&0x1FF;
        }

        // Do the data transfer
        for(int i = 0; i < 11; ++i){
            for(int e = 0; e < 16; ++e){
                GBA_DMA_Copy32(vptr, CK_BlitBuffer[(i<<4)+e][0], 16);
                GBA_DMA_Copy32(vptr+VSHIFT, CK_BlitBuffer[(i<<4)+e][0]+(UMTILESET_WIDTH_VAL), 16);

                GBA_DMA_Copy32(vptr2, CK_BlitBuffer[(i<<4)+e][1], 16);
                GBA_DMA_Copy32(vptr2+VSHIFT, CK_BlitBuffer[(i<<4)+e][1]+(MTILESET_WIDTH_VAL), 16);

                vptr += 16;
                vptr2 += 16;
            }
            vptr += 256;
            vptr2 += 256;
        }

        CK_UpdateRendering = false;
//        CK_POICount = 0; // Reset this
    }/*else{
        while(CK_POICount){
            uint16_t scroffset = CK_UpdatePOI[CK_POICount][0];
            --CK_POICount;
        }
    }*/

    // Move the camera if need be
    if(CK_CameraMoved){
        CK_CameraMoved = false;
        *(volatile uint16_t*)GBA_REG_BG0HOFS = (uint16_t)CK_CameraX&0x1FF;
        *(volatile uint16_t*)GBA_REG_BG0VOFS = (uint16_t)CK_CameraY&0x1FF;

        *(volatile uint16_t*)GBA_REG_BG1HOFS = (uint16_t)CK_CameraX&0x1FF;
        *(volatile uint16_t*)GBA_REG_BG1VOFS = (uint16_t)CK_CameraY&0x1FF;
    }

};

void CK_UpdateLevel(){
    uint32_t voff_s = ((CK_CameraBlockY)*CK_CurLevelWidth)+CK_CameraBlockX;
    uint32_t voff_e = ((10+CK_CameraBlockY)*CK_CurLevelWidth)+15+CK_CameraBlockX;


    for(int p = 0; p < 2; p++){
        for(int ani = 0; ani < CK_NumOfAnimations[p]; ani++){
            struct CK_TileAnimation * ck_ani = &CK_CurLevelAnimations[ani][p];
            ck_ani->ani_time -= tics;
            while(ck_ani->ani_time < 1){
                // Update the tile
                uint16_t *tile = &CK_CurLevelData[ck_ani->map_offset];
                *tile += (signed short)ck_ani->tile_to;
                if(p == 0){
                    ck_ani->tile_to = (signed char)CK_TileInfo[p][*tile+CK_TileInfo_BGTiles];
                    ck_ani->ani_time = CK_TileInfo[p][*tile];
                }
                if(p == 1){
                    ck_ani->tile_to = (signed char)CK_TileInfo[p][(*tile)+(CK_TileInfo_FGTiles*4)];
                    ck_ani->ani_time = CK_TileInfo[p][*tile+(CK_TileInfo_FGTiles*6)];
                }
                if(ck_ani->map_offset >= voff_s && ck_ani->map_offset <= voff_e){
                    /*
                    uint16_t tiley = ck_ani->map_offset / CK_CurLevelWidth;
                    uint16_t tilex = ck_ani->map_offset - (tiley*CK_CurLevelWidth);
                    tilex -= CK_CameraBlockX;
                    tiley -= CK_CameraBlockY;

                    CK_UpdatePOI[CK_POICount][p] = 0;
                    CK_POICount++;
    //                */CK_UpdateRendering = true;
                }
#ifdef KEEN6
                if (anim->visible && anim->current == anim->soundtile && anim->sound != -1)
                {
                    SD_PlaySound(anim->sound);
                }
#endif
            }
        }
        voff_s += CK_CurLevelSize;
        voff_e += CK_CurLevelSize;
    }

};

//==========================================================================

/*
==========================
=
= ScanInfoPlane
=
= Spawn all actors and mark down special places
=
==========================
*/

void ScanInfoPlane(void){
    InitObjArray(); // start spawning things with a clean slate
    CK_LoadSpectators();
};




//
// Global : Actor coordinates are in this, at 1/16 th of a pixel, to allow
// for fractional movement and acceleration.
//
// Tiles  : Tile offsets from the upper left corner of the current map.
//
// Screen : Graphics level offsets from map origin, x in bytes, y in pixels.
// originxscreen is the same spot as originxtile, just with extra precision
// so graphics don't need to be done in tile boundaries.
//

unsigned	originxglobal,originyglobal;
unsigned	originxtile,originytile;
unsigned	originxscreen,originyscreen;
unsigned	originmap;
unsigned	originxmin,originxmax,originymin,originymax;


/*
=================
=
= RFL_CalcOriginStuff
=
= Calculate all the global variables for a new position
= Long parms so position can be clipped to a maximum near 64k
=
=================
*/

void RFL_CalcOriginStuff (long x, long y)
{
	originxglobal = x;
	originyglobal = y;
	originxtile = originxglobal>>G_T_SHIFT;
	originytile = originyglobal>>G_T_SHIFT;
	originxscreen = originxtile<<SX_T_SHIFT;
	originyscreen = originytile<<SY_T_SHIFT;
	originmap = (CK_CurLevelWidth*originytile) + originxtile*2;

    // Calculate pan
    CK_FixCamera();
}


/*
=================
=
= RFL_ClearScrollBlocks
=
=================
*/

void RFL_ClearScrollBlocks (void)
{
	hscrollblocks = vscrollblocks = 0;
}


/*
=================
=
= RF_SetScrollBlock
=
= Sets a horizontal or vertical scroll block
= a horizontal block is ----, meaning it blocks up/down movement
=
=================
*/

void RF_SetScrollBlock (int x, int y, boolean horizontal)
{
	if (horizontal)
	{
		hscrolledge[hscrollblocks] = y;
		if (hscrollblocks++ == MAXSCROLLEDGES)
			Quit ("RF_SetScrollBlock: Too many horizontal scroll blocks");
	}
	else
	{
		vscrolledge[vscrollblocks] = x;
		if (vscrollblocks++ == MAXSCROLLEDGES)
			Quit ("RF_SetScrollBlock: Too many vertical scroll blocks");
	}
}


/*
=================
=
= RFL_BoundScroll
=
= Bound a given x/y movement to scroll blocks
=
=================
*/

void RFL_BoundScroll (int x, int y)
{
	int	check,newxtile,newytile;

	originxglobal += x;
	originyglobal += y;

	newxtile= originxglobal >> G_T_SHIFT;
	newytile = originyglobal >> G_T_SHIFT;

	if (x>0)
	{
		newxtile+=SCREENTILESWIDE;
		for (check=0;check<vscrollblocks;check++)
			if (vscrolledge[check] == newxtile)
			{
				originxglobal = originxglobal&0xff00;
				break;
			}
	}
	else if (x<0)
	{
		for (check=0;check<vscrollblocks;check++)
			if (vscrolledge[check] == newxtile)
			{
				originxglobal = (originxglobal&0xff00)+0x100;
				break;
			}
	}


	if (y>0)
	{
		newytile+=SCREENTILESHIGH;
		for (check=0;check<hscrollblocks;check++)
			if (hscrolledge[check] == newytile)
			{
				originyglobal = originyglobal&0xff00;
				break;
			}
	}
	else if (y<0)
	{
		for (check=0;check<hscrollblocks;check++)
			if (hscrolledge[check] == newytile)
			{
				originyglobal = (originyglobal&0xff00)+0x100;
				break;
			}
	}


	RFL_CalcOriginStuff (originxglobal, originyglobal);
}

