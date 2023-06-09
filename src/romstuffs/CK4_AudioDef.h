#pragma once

// MODDERS:
// Sizes must be manually moved from the C source files
const unsigned int SND1_156_size = 222073;
extern const unsigned char SND1_156_data[];
const unsigned int SND1_157_size = 568880;
extern const unsigned char SND1_157_data[];
const unsigned int SND1_158_size = 172084;
extern const unsigned char SND1_158_data[];
const unsigned int SND1_159_size = 482297;
extern const unsigned char SND1_159_data[];
const unsigned int SND1_160_size = 348727;
extern const unsigned char SND1_160_data[];
const unsigned int SND1_161_size = 136554;
extern const unsigned char SND1_161_data[];

#define NUM_OF_SONGS 6

const GBA_SoundSample CKM_GBA_Samples [] = {
    {&SND1_156_data , SND1_156_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },
    {&SND1_157_data , SND1_157_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },
    {&SND1_158_data , SND1_158_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },
    {&SND1_159_data , SND1_159_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },
    {&SND1_160_data , SND1_160_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },
    {&SND1_161_data , SND1_161_size, GBA_INV_11025, GBA_SAMP_RATE_11025 },
};

// MODDERS:
// Number of songs = Number of levels
const unsigned int CK_NumberOfSongs = 20;
const unsigned int CK_LevelAudio[] = {
    SHADOWS_MUS, // Lvl 0 - Shadowlands
	KICKPANT_MUS, // Lvl 1 - Border Village
	OASIS_MUS, // Lvl 2 - Slug Village
	OASIS_MUS, // Lvl 3 - The Perilous Pit
	TOOHOT_MUS, // Lvl 4 - Cave of the Descendents
	TOOHOT_MUS, // Lvl 5 - Chasm of Chills
	KICKPANT_MUS, // Lvl 6 - Crystalus
	OASIS_MUS, // Lvl 7 - Hillville
	VEGGIES_MUS, // Lvl 8 - Sand Yego
	VEGGIES_MUS, // Lvl 9 - Miragia
	VEGGIES_MUS, // Lvl 10 - Lifewater Oasis
	TOOHOT_MUS, // Lvl 11 - Pyramid of the Moons
	TOOHOT_MUS, // Lvl 12 - Pyramid of Shadows
	TOOHOT_MUS, // Lvl 13 - Pyramid of the Gnosticene Ancients
	TOOHOT_MUS, // Lvl 14 - Pyramid of the Forbidden (Secret Level)
	TOOHOT_MUS, // Lvl 15 - Isle of Tar
    TOOHOT_MUS, // Lvl 16 - Isle of Fire
	VEGGIES_MUS, // Lvl 17 - Well of Wishes
	OASIS_MUS, // Lvl 18 - Bean-with-Bacon
    WONDER_MUS, // Lvl 19 - High Scores
};

/*
	static Sint16 songs[] =
	{
#if defined KEEN4
		SHADOWS_MUS,
		KICKPANT_MUS,
		OASIS_MUS,
		OASIS_MUS,
		TOOHOT_MUS,
		TOOHOT_MUS,
		KICKPANT_MUS,
		OASIS_MUS,
		VEGGIES_MUS,
		VEGGIES_MUS,
		VEGGIES_MUS,
		TOOHOT_MUS,
		TOOHOT_MUS,
		TOOHOT_MUS,
		TOOHOT_MUS,
		TOOHOT_MUS,
		TOOHOT_MUS,
		VEGGIES_MUS,
		OASIS_MUS,
		-1
#elif defined KEEN5
		ROBOROCK_MUS,
		WEDNESDY_MUS,
		BREATHE_MUS,
		SPHEREFUL_MUS,
		TIGHTER_MUS,
		SPHEREFUL_MUS,
		TIGHTER_MUS,
		SPHEREFUL_MUS,
		TIGHTER_MUS,
		SPHEREFUL_MUS,
		TIGHTER_MUS,
		SNOOPING_MUS,
		FEARSOME_MUS,
		BAGPIPES_MUS,
		FANFARE_MUS,
		SKATING_MUS,
		ROCK_ME_MUS,
		HAVING_T_MUS,
		CAMEIN_MUS,
		SHIKAIRE_MUS,
#elif defined KEEN6
		ALIENATE_MUS,
		FASTER_MUS,
		BRERTAR_MUS,
		MAMSNAKE_MUS,
		MAMSNAKE_MUS,
		MAMSNAKE_MUS,
		METAL_MUS,
		TOFUTURE_MUS,
		METAL_MUS,
		BRERTAR_MUS,
		FASTER_MUS,
		TOFUTURE_MUS,
		BRERTAR_MUS,
		SPACFUNK_MUS,
		SPACFUNK_MUS,
		OMINOUS_MUS,
		TOFUTURE_MUS,
		WONDER_MUS,
		WONDER_MUS,
		WONDER_MUS
#endif
	};
*/

// Include all the sounds

#include "CK4_AdlibDefs.h"
#include "CK4_PCDefs.h"

