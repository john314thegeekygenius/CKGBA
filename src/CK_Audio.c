/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

// Include the audio definitions
#ifdef CK4
#include "romstuffs/CK4_AudioDef.h"
#endif

SDMode SoundMode;
SMMode MusicMode;
longword TimeCount;

void SD_InitAudio(){
    GBA_InitAudio();
    SoundMode = sdm_AdLib;
    MusicMode = smm_AdLib;
};

void SD_PlayMusic(uint32_t chunk, uint32_t asLoop){
#ifndef CK_DISABLE_MUSIC
    if(chunk >= 0 && chunk < LASTMUSIC){
        GBA_StopChannel(GBA_CHANNEL_A);
        GBA_PlaySample(&CKM_GBA_Samples[chunk], asLoop, GBA_CHANNEL_A);
    }
#endif
};

void SD_MusicOff(){
#ifndef CK_DISABLE_MUSIC
    GBA_StopChannel(GBA_CHANNEL_A);
#endif    
};

