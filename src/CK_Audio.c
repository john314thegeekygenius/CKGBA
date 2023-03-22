/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

/* 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

unsigned char *PC_SoundPtr = NULL;
unsigned int PC_SoundLen = 0;
unsigned int PC_SoundCount = 0;

void SD_InitAudio(){
    GBA_InitAudio();
	GBA_SetSoundSqWav(1);
	// Default to adlib sound / music
    SoundMode = sdm_AdLib;
    MusicMode = smm_AdLib;
    SoundNumber = SoundPriority = 0;

	PC_SoundPtr = NULL;
	PC_SoundCount = 0;
	PC_SoundLen = 0;

	// Setup interupt to do other thing
	*(volatile uint16_t*)GBA_INT_ENABLE = 0;
    *(volatile uint16_t*)GBA_INT_SELECT |= GBA_INT_TIMER2; // Enable the timer interupt
	*(volatile uint16_t*)GBA_INT_ENABLE = 1;

	// Make it update every 1/140 of a second
	*(volatile unsigned short*)GBA_TIMER2_DATA = _140_HZ;
	*(volatile unsigned short*)GBA_TIMER2_CONTROL = GBA_TIMER_ENABLE | GBA_TIMER_FREQ_64 | GBA_TIMER_INTERUPT;

	
};



///////////////////////////////////////////////////////////////////////////
//
//	SD_PlaySound() - plays the specified sound on the appropriate hardware
//
///////////////////////////////////////////////////////////////////////////
void
SD_PlaySound(soundnames sound)
{
#ifndef CK_DISABLE_SOUND	
	if(!SD_SoundPlaying())
		SoundPriority = 0;
	if ((SoundMode == sdm_Off) || (sound == -1))
		return;

	unsigned short s_priority = *CKS_SamplePriorities[sound];

	if (s_priority < SoundPriority)
		return;

	switch (SoundMode)
	{
	case sdm_PC:
		PC_SoundPtr = CKS_PC_Samples[sound];
		PC_SoundCount = 0;
		PC_SoundLen = *CKS_PC_SampleLens[sound];
		break;
	case sdm_AdLib:
		GBA_StopChannel(GBA_CHANNEL_B);
		GBA_PlaySample(&CKS_GBA_Samples[sound], 0, GBA_CHANNEL_B);
		break;
	}

	SoundNumber = sound;
	SoundPriority = s_priority;
#endif
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
#ifndef CK_DISABLE_SOUND
	switch (SoundMode)
	{
	case sdm_PC:
		result = (PC_SoundPtr&&PC_SoundLen)? true : false;
		break;
	case sdm_AdLib:
		result = GBA_SamplePlaying(GBA_CHANNEL_B)? true : false;
		break;
	}
#endif
	if (result)
		return (SoundNumber+1);
	else{
		SoundNumber = SoundPriority = 0;
		return(false);
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_StopSound() - if a sound is playing, stops it
//
///////////////////////////////////////////////////////////////////////////
void
SD_StopSound(void)
{
#ifndef CK_DISABLE_SOUND
	switch (SoundMode)
	{
	case sdm_PC:
		//disable sound 1 to left and right
		*(volatile uint16_t*)GBA_SOUNDCNT_L &= ~(GBA_SND_1L | GBA_SND_1R);		
		break;
	case sdm_AdLib:
    	GBA_StopChannel(GBA_CHANNEL_B);
		break;
	}
#endif
    SoundNumber = SoundPriority = 0;
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


///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOn() - turns on the sequencer
//
///////////////////////////////////////////////////////////////////////////
void
SD_MusicOn(void)
{
	switch (MusicMode)
	{
	case smm_AdLib:
#ifndef CK_DISABLE_MUSIC
	GBA_PlayChannel(GBA_CHANNEL_A);
#endif    
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicOff() - turns off the sequencer and any playing notes
//
///////////////////////////////////////////////////////////////////////////
void
SD_MusicOff(void)
{
	word	i;

	switch (MusicMode)
	{
	case smm_AdLib:
#ifndef CK_DISABLE_MUSIC
    GBA_PauseChannel(GBA_CHANNEL_A);
#endif    
		break;
	}
	
}


///////////////////////////////////////////////////////////////////////////
//
//	SD_FadeOutMusic() - starts fading out the music. Call SD_MusicPlaying()
//		to see if the fadeout is complete
//
///////////////////////////////////////////////////////////////////////////
void
SD_FadeOutMusic(void)
{
	switch (MusicMode)
	{
	case smm_AdLib:
		#ifndef CK_DISABLE_MUSIC
			GBA_StopChannel(GBA_CHANNEL_A);
		#endif    
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_MusicPlaying() - returns true if music is currently playing, false if
//		not
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_MusicPlaying(void)
{
	boolean	result;

	switch (MusicMode)
	{
	case smm_AdLib:
		result = false;
		// DEBUG - not written in original (will break?)
		result = GBA_SamplePlaying(GBA_CHANNEL_A)? true : false;
		break;
	default:
		result = false;
	}

	return(result);
}



//	Public routines

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetSoundMode() - Sets which sound hardware to use for sound effects
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_SetSoundMode(SDMode mode)
{
	boolean	result;
	word	tableoffset;

	SD_StopSound();

	switch (mode)
	{
	case sdm_Off:
		result = true;
		break;
	case sdm_PC:
		result = true;
		break;
	case sdm_AdLib:
		result = true;
		break;
	default:
		result = false;
		break;
	}

	if (result && (mode != SoundMode)) {
		SoundMode = mode;
	}

	return(result);
}

///////////////////////////////////////////////////////////////////////////
//
//	SD_SetMusicMode() - sets the device to use for background music
//
///////////////////////////////////////////////////////////////////////////
boolean
SD_SetMusicMode(SMMode mode)
{
	boolean	result;

	SD_FadeOutMusic();
	while (SD_MusicPlaying());

	switch (mode)
	{
	case smm_Off:
		result = true;
		break;
	case smm_AdLib:
		result = true;
		break;
	default:
		result = false;
		break;
	}

	if (result)
		MusicMode = mode;

	return(result);
}
