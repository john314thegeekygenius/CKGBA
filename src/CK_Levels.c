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
	for(int i = 0; i < 32*22; i++){
		*(uint16_t*)(TILEMAP_0+i) = i;
		*(uint16_t*)(TILEMAP_1+i) = i+0xC0;
		*(uint16_t*)(TILEMAP_2+i) = 0x380; // This tile must ALWAYS be clear!
	}

    // Clear the special tile
    GBA_DMA_MemSet32((volatile uint32_t *)TILESTART_1+0x1600, CK_EGA_CLEAR, 8);

	// Finish the render of the background
	GBA_FINISH_BG0_4BIT(GBA_BG_BACK | TILEMAP_MAP_0 | TILEMAP_BLOCK_0 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG1_4BIT(GBA_BG_MID | TILEMAP_MAP_1 | TILEMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
	GBA_FINISH_BG2_4BIT(GBA_BG_FRONT | TILEMAP_MAP_2 | TILEMAP_BLOCK_1 | GBA_BG_SIZE_32x32);
	*(volatile unsigned int*)GBA_REG_DISPCNT |= GBA_ENABLE_BG2;
    
};



unsigned short CK_CurLevelWidth = 0;
unsigned short CK_CurLevelHeight = 0;
unsigned short CK_CurLevelSize = 0;
unsigned short CK_CurLevelIndex = 0;
unsigned int CK_CurLevelAniTick = 0; // Make it big

struct CK_TileAnimation {
    uint16_t map_offset;
    uint16_t tile_to;
    signed short ani_time;
    bool active;
};


// Must be in External Work Ram or else it's too much for the 32K on board
GBA_IN_EWRAM uint16_t CK_CurLevelData[32768]; // Use 64K of memory for the levels (Max level size of 128x128)
GBA_IN_EWRAM struct CK_TileAnimation CK_CurLevelAnimations[2048][2]; // Use 6K or memory for level animations [offset, newtile, time]
//GBA_IN_EWRAM uint16_t CK_UpdatePOI[32*20][2]; // Tiles to update on the screen
//unsigned int CK_POICount = 0; // How many POI points are there?

GBA_IN_EWRAM struct CK_InfoBlock CK_InfoPlaneBlocks[512]; // Use 1K of memory for the info plane updation
unsigned int CK_InfoPlaneBlockCount = 0;

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

#ifdef CK4
#include "romstuffs/CK4_Spectators.c"
#endif
#ifdef CK5
#include "romstuffs/CK5_Spectators.c"
#endif
#ifdef CK6
#include "romstuffs/CK6_Spectators.c"
#endif

unsigned short CK_GetInfo(unsigned int offset){
    for(int i = 0; i < CK_InfoPlaneBlockCount; i++){
        if(CK_InfoPlaneBlocks[i].map_offset == offset){
            return CK_InfoPlaneBlocks[i].new_tile;
        }
    }
    return CK_LevelInfo[(CK_CurLevelIndex*3)+2][offset+(CK_CurLevelSize*2)];
};

void CK_SetInfo(unsigned int offset, unsigned short tile){
    for(int i = 0; i < CK_InfoPlaneBlockCount; i++){
        if(CK_InfoPlaneBlocks[i].map_offset == offset){
            CK_InfoPlaneBlocks[i].new_tile = tile;
            return;
        }
    }
    CK_InfoPlaneBlocks[CK_InfoPlaneBlockCount].map_offset = offset;
    CK_InfoPlaneBlocks[CK_InfoPlaneBlockCount].new_tile = tile;
    ++CK_InfoPlaneBlockCount;
};


void CK_ForceLevelRedraw(){
    CK_UpdateRendering = true;
    CK_RenderLevel();
    CK_UpdateSprites();
};

void CK_AddAnimationBlock(int offset, int tileoff, int p){
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
        CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].active = true;

        if(p == 0){
            CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].tile_to = (signed char)CK_TileInfo[p][tileoff+CK_TileInfo_BGTiles];
        }
        if(p == 1){
            CK_CurLevelAnimations[CK_NumOfAnimations[p]][p].tile_to = (signed char)CK_TileInfo[p][tileoff+(CK_TileInfo_FGTiles*4)];
        }

        ++CK_NumOfAnimations[p];
    }

};

void CK_FixAnimationBlocks(){
    // Remove any animations
    CK_NumOfAnimations[0] = CK_NumOfAnimations[1] = 0;
    GBA_DMA_MemSet16((uint16_t*)CK_CurLevelAnimations, 0, 2048);

    for(int p = 0; p < 2; p++){
        for(int y = 0; y < CK_CurLevelHeight; y++){
            for(int x = 0; x < CK_CurLevelWidth; x++){
                const uint32_t offset = (y*CK_CurLevelWidth)+x+(CK_CurLevelSize*p);
                // Copy the level data over
                CK_AddAnimationBlock(offset, CK_CurLevelData[offset], p);
            }
        }
    }

};

void CK_LoadLevel(unsigned short lvlNumber){
    if(lvlNumber > GAMELEVELS) lvlNumber = 0;
	int lvloff = lvlNumber*3;
	CK_CurLevelIndex = lvlNumber;
	CK_CurLevelWidth = *CK_LevelInfo[lvloff];
	CK_CurLevelHeight = *CK_LevelInfo[lvloff+1];
	// Precalculate the level size for SPEEEEEEED!!!!
	CK_CurLevelSize = CK_CurLevelWidth * CK_CurLevelHeight;

    CK_InfoPlaneBlockCount = 0;

    for(int y = 0; y < CK_CurLevelHeight; y++){
        for(int x = 0; x < CK_CurLevelWidth; x++){
            const uint32_t offset = (y*CK_CurLevelWidth)+x;
            // Copy the level data over
            CK_CurLevelData[offset] = CK_LevelInfo[lvloff+2][offset];
            CK_CurLevelData[offset+CK_CurLevelSize] = CK_LevelInfo[lvloff+2][offset+CK_CurLevelSize];
        }
    }
    
    CK_FixAnimationBlocks();

    // Tell the engine to render the level
    CK_UpdateRendering = true;


	originxmin = originymin = MAPBORDER*TILEGLOBAL;

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

	lasttimecount = TimeCount;		// setup for adaptive timing
	tics = 1;

    CK_CurLevelAniTick = 0; // Reset to 0

};

void CK_MoveCamera(signed int x, signed int y){
    RFL_BoundNewOrigin(x,y);
    CK_CameraMoved = true;
    CK_UpdateRendering = true;
};

void CK_FixCamera(){

    CK_GlobalCameraX = CONVERT_GLOBAL_TO_PIXEL(originxglobal);
    CK_GlobalCameraY = CONVERT_GLOBAL_TO_PIXEL(originyglobal);

    if(CK_GlobalCameraLX != CK_GlobalCameraX || CK_GlobalCameraLY != CK_GlobalCameraY){
        CK_CameraMoved = true;
    }

    CK_GlobalCameraLX = CK_GlobalCameraX;
    CK_GlobalCameraLY = CK_GlobalCameraY;

    uint32_t CK_NCameraX = CONVERT_GLOBAL_TO_TILE(originxglobal);
    uint32_t CK_NCameraY = CONVERT_GLOBAL_TO_TILE(originyglobal);

    if(CK_CameraX != CK_NCameraX){
        CK_UpdateRendering = true;
    }
    if(CK_CameraY != CK_NCameraY){
        CK_UpdateRendering = true;
    }

    CK_CameraX = CK_GlobalCameraX%16;
    CK_CameraY = CK_GlobalCameraY%16;

    CK_CameraBlockX = CK_NCameraX;
    CK_CameraBlockY = CK_NCameraY;

};


void CK_ScrollCamera(signed int x, signed int y ){
    RFL_BoundScroll(x,y);
};

void CK_ResetCamera(){
    CK_GlobalCameraX = 0;
    CK_GlobalCameraY = 0;

    CK_GlobalCameraLX = 0;
    CK_GlobalCameraLY = 0;

    CK_CameraX = 0;
    CK_CameraY = 0;

    CK_CameraBlockX = 0;
    CK_CameraBlockY = 0;
    CK_CameraMoved = false;
};

#define UMTILESET_WIDTH_VALD (UMTILESET_WIDTH_VAL<<1)
#define MTILESET_WIDTH_VALD (MTILESET_WIDTH_VAL<<1)

#define CKTILEBG(x,y) (((y)*(UMTILESET_WIDTH_VALD)) + ((x)<<4))
#define CKTILEFG(x,y) (((y)*(MTILESET_WIDTH_VALD)) + ((x)<<4))

GBA_IN_IWRAM uint32_t *CK_BlitBuffer[16*11][3];

void RFL_RemoveAnimsInBlock(unsigned int x, unsigned int y, unsigned short width, unsigned short height){
    for(int p = 0; p < 2; p++){
        unsigned int map_offs = (y*CK_CurLevelWidth) + x + (CK_CurLevelSize*p);
        for(int ani = 0; ani < CK_NumOfAnimations[p]; ani++){
            struct CK_TileAnimation * ck_ani = &CK_CurLevelAnimations[ani][p];
            if(!ck_ani->active) continue;
            for(int ychk = 0; ychk < height*CK_CurLevelWidth; ychk+=CK_CurLevelWidth)
                if(ck_ani->map_offset >= map_offs+ychk && ck_ani->map_offset < map_offs+width+ychk)
                    ck_ani->active = false;
        }
    }
};

void CK_SetMapTile(unsigned short x, unsigned short y, unsigned short tile, unsigned short plane){
    
    RFL_RemoveAnimsInBlock (x,y,1,1);

    // Assumes that plane is never anything other than 0 and 1
    uint32_t doffset = (y*CK_CurLevelWidth)+x;
    if(plane == 2){
        CK_SetInfo(doffset, tile);
        return;
    }
    CK_CurLevelData[doffset+(CK_CurLevelSize*plane)] = tile;
};

void RF_MapToMap(unsigned short srcx, unsigned short srcy, 
                unsigned short dstx, unsigned short dsty, 
                unsigned short width, unsigned short height){
    // We could use DMA, but that's kinda overkill for small copies
    uint32_t soffset = (srcy*CK_CurLevelWidth)+srcx;
    uint32_t doffset = (dsty*CK_CurLevelWidth)+dstx;
    uint16_t * smap = CK_CurLevelData + soffset;
    uint16_t * dmap = CK_CurLevelData + doffset;
    // Remove old animations
    RFL_RemoveAnimsInBlock (dstx,dsty,width,height);

    // Will bug out if map src and dst overlap???
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            int offset = (y*CK_CurLevelWidth) + x;
            // Copy all the tiles over
            dmap[offset] = smap[offset];
            dmap[offset + CK_CurLevelSize] = smap[offset+CK_CurLevelSize];
            CK_SetInfo(doffset+offset, CK_GetInfo(soffset+offset));
            // Handle any animations too
            CK_AddAnimationBlock(doffset+offset, dmap[offset], 0);
            CK_AddAnimationBlock(doffset+offset+CK_CurLevelSize, dmap[offset+CK_CurLevelSize], 1);
        }
    }

};


void RF_MemToMap(uint16_t *data, uint16_t plane, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
    RFL_RemoveAnimsInBlock (x,y,w,h);
    for(int e = 0; e < h; e++){
        uint32_t doffset = ((y+e)*CK_CurLevelWidth)+x+(CK_CurLevelSize*plane);
        for(int i = 0; i < w; i++){
            CK_CurLevelData[doffset] = data[(e*w)+i];
            // Handle any animations too
            CK_AddAnimationBlock(doffset, CK_CurLevelData[doffset], plane);
            doffset++;
        }
    }
};

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
        int blitoffset = 0;
        // Pregenerate a new map
        for(int i = 0; i < 11; ++i){
            for(int e = 0; e < 16; ++e){
                keenTile = CK_CurLevelData[doffset];
                tileY = (keenTile/18);
                tileX = keenTile%18;
                BlitTile = CKTILEBG(tileX,tileY);

                blitoffset = (i*16)+e;

                // Copy the background tiles
                CK_BlitBuffer[blitoffset][0] = (volatile uint32_t *)CK_TILESET_UNMASKED+BlitTile;

                keenTile = CK_CurLevelData[doffset+CK_CurLevelSize];
//                if(keenTile==0) goto skipdraw2; // Skip blank tiles???????
                tileY = (keenTile/18);
                tileX = keenTile%18;
                BlitTile = CKTILEFG(tileX,tileY);

                // Copy the foreground tiles
                CK_BlitBuffer[blitoffset][2] = 0;
                CK_BlitBuffer[blitoffset][1] = (volatile uint32_t *)CK_TILESET_MASKED+BlitTile;
                // If it covers sprites, draw it in the super foreground
                if( (unsigned int)CK_TileInfoFG[keenTile + INTILE] & INTILE_FOREGROUND){
                    CK_BlitBuffer[blitoffset][2] = (volatile uint32_t *)CK_TILESET_MASKED+BlitTile;
                }
                ++doffset;
            }
            doffset += (CK_CurLevelWidth-16);
        }
        // TODO:
        // Make this optional for extra speed???
        // Note : This changing this will cause screen tearing
        GBA_WAIT_FOR_VBLANKN(160)

        // Move the camera if need be
        if(CK_CameraMoved){
            CK_CameraMoved = false;
            *(volatile uint16_t*)GBA_REG_BG0HOFS = (uint16_t)CK_CameraX&0x1FF;
            *(volatile uint16_t*)GBA_REG_BG0VOFS = (uint16_t)CK_CameraY&0x1FF;

            *(volatile uint16_t*)GBA_REG_BG1HOFS = (uint16_t)CK_CameraX&0x1FF;
            *(volatile uint16_t*)GBA_REG_BG1VOFS = (uint16_t)CK_CameraY&0x1FF;

            *(volatile uint16_t*)GBA_REG_BG2HOFS = (uint16_t)CK_CameraX&0x1FF;
            *(volatile uint16_t*)GBA_REG_BG2VOFS = (uint16_t)CK_CameraY&0x1FF;
        }
        volatile uint16_t* tilemem = TILEMAP_2;
        int tileoffset = 0;
        // Do the data transfer
        for(int i = 0; i < 11; ++i){
            for(int e = 0; e < 16; ++e){
                blitoffset = (i<<4)+e;
                tileoffset = (i<<6)+(e<<1);
                GBA_DMA_Copy32(vptr, CK_BlitBuffer[blitoffset][0], 16);
                GBA_DMA_Copy32(vptr+VSHIFT, CK_BlitBuffer[blitoffset][0]+(UMTILESET_WIDTH_VAL), 16);

                GBA_DMA_Copy32(vptr2, CK_BlitBuffer[blitoffset][1], 16);
                GBA_DMA_Copy32(vptr2+VSHIFT, CK_BlitBuffer[blitoffset][1]+(MTILESET_WIDTH_VAL), 16);

                if(CK_BlitBuffer[blitoffset][2]){
                    // Copy some tile ids over the the top map
                    *tilemem = tileoffset+0xC0;
                    *(tilemem+1) = tileoffset+1+0xC0;
                    *(tilemem+32) = tileoffset+32+0xC0;
                    *(tilemem+33) = tileoffset+33+0xC0;
                }else{
                    *tilemem = 0x380;
                    *(tilemem+1) = 0x380;
                    *(tilemem+32) = 0x380;
                    *(tilemem+33) = 0x380;
                }

                vptr += 16;
                vptr2 += 16;
                tilemem+=2;
            }
            tilemem+=32;
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
};

// MODDERS:
// What tiles play what sounds
#ifdef KEEN6

#define SOUNDTILES 2

typedef struct {
    uint16_t tileid;
    uint16_t soundid;
}tilesoundtype;

tilesoundtype soundtiles[SOUNDTILES] = {
	{2152, SND_STOMP},
    {2208, SND_FLAME}
};
#endif


void CK_UpdateLevel(){
    uint32_t voff_s = ((CK_CameraBlockY)*CK_CurLevelWidth)+CK_CameraBlockX;
    uint32_t voff_e = ((10+CK_CameraBlockY)*CK_CurLevelWidth)+15+CK_CameraBlockX;

    CK_CurLevelAniTick ++; // Increment the animaiton count

    for(int p = 0; p < 2; p++){
        for(int ani = 0; ani < CK_NumOfAnimations[p]; ani++){
            struct CK_TileAnimation * ck_ani = &CK_CurLevelAnimations[ani][p];
            if(!ck_ani->active) continue; // Don't worry about inactive animations
            ck_ani->ani_time -= tics;
            if(ck_ani->ani_time < 1){
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
#ifdef KEEN6
                for(int sndid = 0; sndid < SOUNDTILES; sndid++){
                    if(*tile == soundtiles[sndid].tileid){
                        SD_PlaySound(soundtiles[sndid].soundid);
                    }
                }
#endif
                    CK_UpdateRendering = true;
                }
            }
        }
        voff_s += CK_CurLevelSize;
        voff_e += CK_CurLevelSize;
    }

};

void CK_FixTileAnimations(unsigned int toTick){
    // Update the level background tiles toTick times
    for(int ani = 0; ani < CK_NumOfAnimations[0]; ani++){
        struct CK_TileAnimation * ck_ani = &CK_CurLevelAnimations[ani][0];
        for(int i = 0; i < toTick; i++){
            if(!ck_ani->active) continue; // Don't worry about inactive animations
            ck_ani->ani_time -= 1;
            if(ck_ani->ani_time < 1){
                // Update the tile
                uint16_t *tile = &CK_CurLevelData[ck_ani->map_offset];
                *tile += (signed short)ck_ani->tile_to;

                ck_ani->tile_to = (signed char)CK_TileInfo[0][*tile+CK_TileInfo_BGTiles];
                ck_ani->ani_time = CK_TileInfo[0][*tile];
            }
        }
    }
};

void CK_SnapScroll(){
    // We moved the camera a bit
    CK_CameraMoved = true;
    // Find closest 8x8 chunk
    CK_CameraX >>= 3;
    CK_CameraX <<= 3;

    CK_CameraY >>= 3;
    CK_CameraY <<= 3;

    *(volatile uint16_t*)GBA_REG_BG0HOFS = (uint16_t)CK_CameraX&0x1FF;
    *(volatile uint16_t*)GBA_REG_BG0VOFS = (uint16_t)CK_CameraY&0x1FF;

    *(volatile uint16_t*)GBA_REG_BG1HOFS = (uint16_t)CK_CameraX&0x1FF;
    *(volatile uint16_t*)GBA_REG_BG1VOFS = (uint16_t)CK_CameraY&0x1FF;

    *(volatile uint16_t*)GBA_REG_BG2HOFS = (uint16_t)CK_CameraX&0x1FF;
    *(volatile uint16_t*)GBA_REG_BG2VOFS = (uint16_t)CK_CameraY&0x1FF;    
}

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
    CK_NukeObjectsSprites(); // Remove any and all old objects
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
	int	bcheck,newxtile,newytile;

	originxglobal += x;
	originyglobal += y;

	newxtile= originxglobal >> G_T_SHIFT;
	newytile = originyglobal >> G_T_SHIFT;

	if (x>0)
	{
		newxtile+=SCREENTILESWIDE;
		for (bcheck=0;bcheck<vscrollblocks;bcheck++)
			if (vscrolledge[bcheck] == newxtile)
			{
				originxglobal = originxglobal&0xff00;
				break;
			}
	}
	else if (x<0)
	{
		for (bcheck=0;bcheck<vscrollblocks;bcheck++)
			if (vscrolledge[bcheck] == newxtile)
			{
				originxglobal = (originxglobal&0xff00)+0x100;
				break;
			}
	}


	if (y>0)
	{
		newytile+=SCREENTILESHIGH;
		for (bcheck=0;bcheck<hscrollblocks;bcheck++)
			if (hscrolledge[bcheck] == newytile)
			{
				originyglobal = originyglobal&0xff00;
				break;
			}
	}
	else if (y<0)
	{
		for (bcheck=0;bcheck<hscrollblocks;bcheck++)
			if (hscrolledge[bcheck] == newytile)
			{
				originyglobal = (originyglobal&0xff00)+0x100;
				break;
			}
	}


	RFL_CalcOriginStuff (originxglobal, originyglobal);
}


/*
=====================
=
= RFL_BoundNewOrigin
=
= Copies a string of tiles from main memory to the map,
= accounting for animating tiles
=
=====================
*/

void RFL_BoundNewOrigin (unsigned orgx,unsigned orgy)
{
	int	check,edge;

//
// calculate new origin related globals
//
	if (orgx<originxmin)
	  orgx=originxmin;
	else if (orgx>originxmax)
	  orgx=originxmax;

	if (orgy<originymin)
	  orgy=originymin;
	else if (orgy>originymax)
	  orgy=originymax;

	originxtile = orgx>>G_T_SHIFT;
	originytile = orgy>>G_T_SHIFT;

	for (check=0;check<vscrollblocks;check++)
	{
		edge = vscrolledge[check];
		if (edge>=originxtile && edge <=originxtile+CHECK_LX)
		{
			orgx = (edge+1)*TILEGLOBAL;
			break;
		}
		if (edge>=originxtile+CHECK_LX+1 && edge <=originxtile+CHECK_RX)
		{
			orgx = (edge-CHECK_RX)*TILEGLOBAL;
			break;
		}
	}

	for (check=0;check<hscrollblocks;check++)
	{
		edge = hscrolledge[check];
		if (edge>=originytile && edge <=originytile+CHECK_TY)
		{
			orgy = (edge+1)*TILEGLOBAL;
			break;
		}
		if (edge>=originytile+CHECK_TY+1 && edge <=originytile+CHECK_BY)
		{
			orgy = (edge-CHECK_BY)*TILEGLOBAL;
			break;
		}
	}


	RFL_CalcOriginStuff (orgx,orgy);
}


