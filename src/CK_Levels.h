/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_LEVELS__
#define __CK_LEVELS__

#ifdef CK4
#define CK_NumOfLevels  20
#endif



// From ID_RF.C
extern	unsigned	tics;
extern	long		lasttimecount;

#define	PORTTILESWIDE		30      // all drawing takes place inside a
#define	PORTTILESHIGH		20		// non displayed port of this size

extern	unsigned	originxglobal,originyglobal;
extern	unsigned	originxtile,originytile;
extern	unsigned	originxscreen,originyscreen;
extern	unsigned	originxmin,originxmax,originymin,originymax;

#define	SCREENTILESWIDE	15
#define	SCREENTILESHIGH	10

#define MAXSCROLLEDGES	6
#define SX_T_SHIFT      1	// screen x >> ?? = tile EGA = 1, CGA = 2;
#define	SY_T_SHIFT		4	// screen y >> ?? = tile



extern unsigned short CK_CurLevelWidth;
extern unsigned short CK_CurLevelHeight;
extern unsigned short CK_CurLevelSize;
extern unsigned short CK_CurLevelIndex;

extern GBA_IN_EWRAM uint16_t CK_CurLevelData[32768];
extern const unsigned short* CK_LevelInfo[];
extern const char *CK_TileInfo[2];
extern const uint16_t CK_TileInfo_BGTiles ;
extern const uint16_t CK_TileInfo_FGTiles ;

extern signed int CK_GlobalCameraX ;
extern signed int CK_GlobalCameraY ;

extern signed int CK_GlobalCameraLX ;
extern signed int CK_GlobalCameraLY ;

extern uint32_t CK_CameraX;
extern uint32_t CK_CameraY;


// Functions

void CK_SetupLevelGBAMaps();

void CK_ForceLevelRedraw();

void CK_LoadLevel(unsigned short lvlNumber);

void CK_RenderLevel();

void CK_MoveCamera(signed int x,signed int y);

void CK_FixCamera();

void CK_ScrollCamera(signed int x, signed int y );

void CK_UpdateLevel();


//==========================================================================

void ScanInfoPlane(void);

#endif
