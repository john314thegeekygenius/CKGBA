/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

extern signed int CK_GlobalCameraX;
extern signed int CK_GlobalCameraY;

extern unsigned int CK_LevelAudio[];

int main(){

	CK_InitVideo();

	CK_SD_InitAudio();

	CK_SetupLevelGBAMaps();
	
	CK_SetupSprites();

	int chngLvl = 0;
	int chngLvlV = 0;
	int curLvlID = 0;
	int levelX = 0;
	int levelY = 0;

	CK_LoadLevel(curLvlID);
	if(curLvlID >= 0 && curLvlID < CKM_LastSong)
		CK_SD_PlayMusic(CK_LevelAudio[curLvlID], 1);

	int fps_limiter = 0;
	
	while(1){
		++fps_limiter;
		if(fps_limiter == 3){
			fps_limiter = 0;
			if(GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT)){
				CK_GlobalCameraX += 1;
			}
			if(GBA_TEST_BUTTONS(GBA_BUTTON_LEFT)){
				CK_GlobalCameraX -= 1;
			}
			if(GBA_TEST_BUTTONS(GBA_BUTTON_UP)){
				CK_GlobalCameraY -= 1;
			}
			if(GBA_TEST_BUTTONS(GBA_BUTTON_DOWN)){
				CK_GlobalCameraY += 1;
			}
		}

		if(GBA_TEST_BUTTONS(GBA_BUTTON_LSHOLDER)){
			chngLvlV = -1;
			chngLvl = 1;
		}else{
			if(chngLvl==1){
				chngLvl = 2;
			}
		}
		if(GBA_TEST_BUTTONS(GBA_BUTTON_RSHOLDER)){
			chngLvlV = 1;
			chngLvl = 3;
		}else{
			if(chngLvl==3){
				chngLvl = 2;
			}
		}

		if(chngLvl==2){
			curLvlID += chngLvlV;
			CK_LoadLevel(curLvlID);
			if(curLvlID >= 0 && curLvlID < CKM_LastSong)
				CK_SD_PlayMusic(CK_LevelAudio[curLvlID], 1);
			chngLvl = 0;
			CK_GlobalCameraX = CK_GlobalCameraY = 0;
		}
		
		CK_UpdateLevel();

		CK_FixCamera();

		CK_RenderLevel();

		//CK_MoveCamera(levelX, levelY);

//		GBA_Delay(50);
	};

	while(1); // Make the game run forever
	return 0;
};

