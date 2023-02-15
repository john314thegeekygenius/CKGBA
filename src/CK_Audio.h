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

void CK_SD_InitAudio();

void CK_SD_PlayMusic(uint32_t chunk, uint32_t asLoop);

#endif
