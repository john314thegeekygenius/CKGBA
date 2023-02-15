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

// Functions

void CK_SetupLevelGBAMaps();

void CK_LoadLevel(unsigned short lvlNumber);

void CK_RenderLevel();

void CK_MoveCamera(int x,int y);

void CK_FixCamera();

void CK_UpdateLevel();

#endif
