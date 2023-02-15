/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"
#include "romstuffs/CK4_AudioDef.h"

void CK_SD_InitAudio(){
    GBA_InitAudio();
};

void CK_SD_PlayMusic(uint32_t chunk, uint32_t asLoop){
    if(chunk >= 0 && chunk < LASTMUSIC){
        GBA_StopChannel(GBA_CHANNEL_A);
        GBA_PlaySample(&CKM_GBA_Samples[chunk], asLoop, GBA_CHANNEL_A);
    }
};
