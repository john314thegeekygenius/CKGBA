
void CK_LoadSpectators(){
    // Load all the sprites
    for(int y = 0; y < CK_CurLevelHeight; y++){
        for(int x = 0; x < CK_CurLevelWidth; x++){
            const uint32_t offset = (y*CK_CurLevelWidth)+x+(CK_CurLevelSize*2);
            // Copy the level data over
            uint32_t tileid = CK_LevelInfo[(CK_CurLevelIndex*3)+2][offset];
            if(tileid == 0) continue; 
            switch(tileid){
                case 1:
                    // Commander Keen (Right)
                case 2:
                    // Commander Keen (Left)
                    CK_MoveCamera((x-7)<<4, (y-5)<<4); // Move the camera there
                    break;
                case 3:
                    // Commander Keen (Map)
                    CK_MoveCamera((x-7)<<4, (y-5)<<4); // Move the camera there
                    break;
            }
        }
    }
    CK_UpdateRendering = true;
};
