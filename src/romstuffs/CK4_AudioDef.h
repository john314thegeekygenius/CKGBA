#pragma once

// Include all the episode music
#ifdef CK4
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

const unsigned int CK_LevelAudio[] = {
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
	VEGGIES_MUS,
	OASIS_MUS,

	// WONDER_MUS Used for interaction
};

// TODO:
// Include all the sounds :E
#endif

