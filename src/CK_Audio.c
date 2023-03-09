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
static	word SoundNumber,SoundPriority;

void SD_InitAudio(){
    GBA_InitAudio();
    SoundMode = sdm_AdLib;
    MusicMode = smm_AdLib;
    SoundNumber = SoundPriority = 0;
};



///////////////////////////////////////////////////////////////////////////
//
//	SD_PlaySound() - plays the specified sound on the appropriate hardware
//
///////////////////////////////////////////////////////////////////////////
void
SD_PlaySound(soundnames sound)
{
	unsigned short s_priority = 0;

	if ((SoundMode == sdm_Off) || (sound == -1))
		return;

	if (s_priority < SoundPriority)
		return;

	switch (SoundMode)
	{
	case sdm_PC:
		//SDL_PCPlaySound((void far *)s);
		break;
	case sdm_AdLib:
		GBA_PlaySample(&CKS_GBA_Samples[sound], 0, GBA_CHANNEL_B);
		break;
	}

	SoundNumber = sound;
	SoundPriority = s_priority;
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SoundPlaying() - returns the sound number that's playing, or 0 if
//		no sound is playing
//
///////////////////////////////////////////////////////////////////////////
word
SD_SoundPlaying(void)
{
	boolean	result = false;

	switch (SoundMode)
	{
	case sdm_PC:
		//result = pcSound? true : false;
		break;
	case sdm_AdLib:
		result = GBA_SamplePlaying(GBA_CHANNEL_B)? true : false;
		break;
	}

	if (result)
		return (SoundNumber+1);
	else
		return(false);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
void
SD_StopSound(void)
{
	switch (SoundMode)
	{
	case sdm_PC:
//		SDL_PCStopSound();
		break;
	case sdm_AdLib:
    	GBA_StopChannel(GBA_CHANNEL_B);
		break;
	}

	GBA_StopChannel(GBA_CHANNEL_B);
    // Handle GBA square wave here???
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_WaitSoundDone() - waits until the current sound is done playing
//
///////////////////////////////////////////////////////////////////////////
void
SD_WaitSoundDone(void)
{
	while (SD_SoundPlaying());
}


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
