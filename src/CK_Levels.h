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

extern unsigned short CK_CurLevelWidth;
extern unsigned short CK_CurLevelHeight;
extern unsigned short CK_CurLevelSize;
extern unsigned short CK_CurLevelIndex;

extern GBA_IN_EWRAM uint16_t CK_CurLevelData[32768];
extern const unsigned short* CK_LevelInfo[];

extern signed int CK_GlobalCameraX ;
extern signed int CK_GlobalCameraY ;

extern signed int CK_GlobalCameraLX ;
extern signed int CK_GlobalCameraLY ;

// Functions

void CK_SetupLevelGBAMaps();

void CK_LoadLevel(unsigned short lvlNumber);

void CK_RenderLevel();

void CK_MoveCamera(int x,int y);

void CK_FixCamera();

void CK_ScrollCamera(signed short x, signed short y );

void CK_UpdateLevel();


//==========================================================================

void ScanInfoPlane(void);

#endif
