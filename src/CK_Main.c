/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

int main(){

	CK_InitVideo();
	
	CK_SetupLevelGBAMaps();

	int chngLvl = 0;
	int chngLvlV = 0;
	int curLvlID = 0;
	int levelX = 0;
	int levelY = 0;

	CK_LoadLevel(curLvlID);
	
	while(1){

		if(GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT)){
			levelX += 1;
		}
		if(GBA_TEST_BUTTONS(GBA_BUTTON_LEFT)){
			levelX -= 1;
		}
		if(GBA_TEST_BUTTONS(GBA_BUTTON_UP)){
			levelY -= 1;
		}
		if(GBA_TEST_BUTTONS(GBA_BUTTON_DOWN)){
			levelY += 1;
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
			chngLvl = 0;
			levelX = levelY = 0;
		}

		CK_MoveCamera(levelX, levelY);
		
		CK_UpdateLevel();

		CK_RenderLevel();

//		GBA_Delay(50);
	};

	while(1); // Make the game run forever
	return 0;
};

