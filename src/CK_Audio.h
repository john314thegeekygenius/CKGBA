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

#ifndef __CK_AUDIO__
#define __CK_AUDIO__

// Header from reconstructed source code
#if defined CK4
#include "romstuffs/AUDIOCK4.H"
#elif defined CK5
#include "romstuffs/AUDIOCK5.H"
#elif defined CK6
#include "romstuffs/AUDIOCK6.H"
#endif

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

// Could be GBA_REFRESH_RATE ???
#define	TickBase	70		// 70Hz per tick - used as a base for timer 0

#define _140_HZ 0xF8AF // 64
// 256  - 0xFE2B
// 1024 - 0xFF8A

void SD_InitAudio();

void SD_PlayMusic(uint32_t chunk, uint32_t asLoop);

void SD_MusicOff();

extern	boolean	SD_MusicPlaying(void),
				SD_SetSoundMode(SDMode mode),
				SD_SetMusicMode(SMMode mode);
extern	word	SD_SoundPlaying(void);


#endif
