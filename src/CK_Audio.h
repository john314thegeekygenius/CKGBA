/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_AUDIO__
#define __CK_AUDIO__

// Header from reconstructed source code
#include "romstuffs/AUDIOCK4.H"

#define CKM_LastSong LASTMUSIC

typedef	enum {
    sdm_Off,
    sdm_PC,sdm_AdLib,
}	SDMode;

typedef	enum {
    smm_Off,smm_AdLib
}	SMMode;

extern SDMode SoundMode;
extern SMMode MusicMode;
extern longword TimeCount;


void SD_InitAudio();

void SD_PlayMusic(uint32_t chunk, uint32_t asLoop);

void SD_MusicOff();

#endif
