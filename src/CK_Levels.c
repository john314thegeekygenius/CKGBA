/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"
#include "CK_Levels.h"

extern const unsigned char CK_TILESET_MASKED[];
extern const unsigned char CK_TILESET_UNMASKED[];


#define TILEMAP_BLOCK_0 0x00
#define TILEMAP_BLOCK_1 0x04

#define TILEMAP_MAP_0 0x1E00
#define TILEMAP_MAP_1 0x1F00

volatile uint16_t *TILEMAP_0 = (volatile uint16_t*)GBA_SCREEN_BLOCK(30);
volatile uint16_t *TILEMAP_1 = (volatile uint16_t*)GBA_SCREEN_BLOCK(31);

volatile uint16_t* TILESTART_0 = (volatile uint16_t*)GBA_VRAM;
volatile uint16_t* TILESTART_1 = (volatile uint16_t*)(GBA_VRAM+0x6000)-1024;

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
    uint16_t ani_time;
};

// Must be in External Work Ram or else it's too much for the 32K on board
GBA_IN_EWRAM uint16_t CK_CurLevelData[32768]; // Use 64K of memory for the levels (Max level size of 128x128)
GBA_IN_EWRAM struct CK_TileAnimation CK_CurLevelAnimations[2048][2]; // Use 6K or memory for level animations [offset, newtile, time]
//GBA_IN_EWRAM uint16_t CK_UpdatePOI[32*20][2]; // Tiles to update on the screen
//unsigned int CK_POICount = 0; // How many POI points are there?

const char *CK_TileInfo[2] = { CK_TileInfoBG, CK_TileInfoFG };


unsigned int CK_NumOfAnimations[2];

signed int CK_CameraX = 0;
signed int CK_CameraY = 0;
signed int CK_CameraBlockX = 0;
signed int CK_CameraBlockY = 0;

const int CK_CameraWidth = 30>>1;
const int CK_CameraHeight = 20>>1;

bool CK_UpdateRendering = false;

uint32_t CK_UpdateTick = 0;



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

    // Move the camera
    CK_MoveCamera(0,0);
};


void CK_MoveCamera(int x,int y){

    if(CK_CameraX != (x%16) && (x%16)==0) CK_UpdateRendering = true;
    if(CK_CameraY != (y%16) && (y%16)==0) CK_UpdateRendering = true;

    CK_CameraX = x%16;
    CK_CameraY = y%16;
    CK_CameraBlockX = x/16;
    CK_CameraBlockY = y/16;

    // Clamp the camera
    if(CK_CameraBlockX <= 0) { CK_CameraBlockX = 0; if(CK_CameraX < 0) {CK_CameraX = 0; } }
    if(CK_CameraBlockX <= 0) { CK_CameraBlockX = 0; if(CK_CameraY < 0) {CK_CameraY = 0; } }
    if(CK_CameraBlockX >= (CK_CurLevelWidth - CK_CameraWidth)){
        CK_CameraBlockX = (CK_CurLevelWidth - CK_CameraWidth);
        CK_CameraX = 0;
    }
    if(CK_CameraBlockY >= (CK_CurLevelHeight - CK_CameraHeight)){
        CK_CameraY = 0;
        CK_CameraBlockY = (CK_CurLevelHeight - CK_CameraHeight);
    }

};

#define UMTILESET_WIDTH_VAL (288)
#define MTILESET_WIDTH_VAL (288)

#define CKTILEBG(x,y) (((y)*(UMTILESET_WIDTH_VAL<<1)) + ((x)<<4))
#define CKTILEFG(x,y) (((y)*(MTILESET_WIDTH_VAL<<1)) + ((x)<<4))

void CK_RenderLevel(){
    // Removed because it's useless
//	if(CK_CurLevelData == NULL) return; // Nothing to do

    volatile uint32_t *vptr = (volatile uint32_t *)TILESTART_0;
    volatile uint32_t *vptr2 = (volatile uint32_t *)TILESTART_1;
    volatile uint32_t *tileptr = NULL;
    
    uint32_t doffset;
    uint16_t keenTile;
    uint32_t tileY, tileX;
    uint32_t BlitTile;

    if(CK_UpdateRendering) {

        // Load the tiles in real time
        doffset = ((CK_CameraBlockY)*CK_CurLevelWidth)+CK_CameraBlockX;
        for(int i = 0; i < 11; ++i){
            for(int e = 0; e < 16; ++e){
                
                keenTile = CK_CurLevelData[doffset];
                tileY = (keenTile/18);
                tileX = keenTile-(tileY*18);
                BlitTile = CKTILEBG(tileX,tileY);

                // Copy the background tiles
                tileptr = (volatile uint32_t *)CK_TILESET_UNMASKED+BlitTile;
                GBA_DMA_Copy32(vptr, tileptr, 16);
                GBA_DMA_Copy32(vptr+(32<<3), tileptr+(UMTILESET_WIDTH_VAL), 16);

                keenTile = CK_CurLevelData[doffset+CK_CurLevelSize];
//                if(keenTile==0) goto skipdraw2; // Skip blank tiles???????
                tileY = (keenTile/18);
                tileX = keenTile-(tileY*18);
                BlitTile = CKTILEFG(tileX,tileY);

                // Copy the background tiles
                tileptr = (volatile uint32_t *)CK_TILESET_MASKED+BlitTile;
                GBA_DMA_Copy32(vptr2, tileptr, 16);
                GBA_DMA_Copy32(vptr2+(32<<3), tileptr+(MTILESET_WIDTH_VAL), 16);
                skipdraw2:
                vptr += 16;
                vptr2 += 16;
                ++doffset;
            }
            doffset += (CK_CurLevelWidth-16);
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
    GBA_WAIT_VBLANK

    // Move the camera if need be
    *(volatile uint32_t*)GBA_REG_BG0HOFS = CK_CameraX;
    *(volatile uint32_t*)GBA_REG_BG0VOFS = CK_CameraY;

    *(volatile uint32_t*)GBA_REG_BG1HOFS = CK_CameraX;
    *(volatile uint32_t*)GBA_REG_BG1VOFS = CK_CameraY;

};

void CK_UpdateLevel(){
    CK_UpdateTick ++;
    if(CK_UpdateTick == 24*2){
        CK_UpdateTick = 0;
        uint32_t voff_s = ((CK_CameraBlockY)*CK_CurLevelWidth)+CK_CameraBlockX;
        uint32_t voff_e = ((10+CK_CameraBlockY)*CK_CurLevelWidth)+15+CK_CameraBlockX;
                    
        for(int p = 0; p < 2; p++){
            for(int ani = 0; ani < CK_NumOfAnimations[p]; ani++){
                struct CK_TileAnimation * ck_ani = &CK_CurLevelAnimations[ani][p];
                if(ck_ani->ani_time > 0){
                    ck_ani->ani_time--;
                    continue;
                }
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
            }
            voff_s += CK_CurLevelSize;
            voff_e += CK_CurLevelSize;
        }
    }
};

