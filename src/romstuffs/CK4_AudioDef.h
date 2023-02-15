#pragma once

// Include all the episode music
#ifdef CK4
extern const unsigned int SND1_156_size;
extern const unsigned char SND1_156_data[];
extern const unsigned int SND1_157_size;
extern const unsigned char SND1_157_data[];
extern const unsigned int SND1_158_size;
extern const unsigned char SND1_158_data[];
extern const unsigned int SND1_159_size;
extern const unsigned char SND1_159_data[];
extern const unsigned int SND1_160_size;
extern const unsigned char SND1_160_data[];
extern const unsigned int SND1_161_size;
extern const unsigned char SND1_161_data[];

const unsigned char ** CK_MusicChunks = {
    &SND1_156_data,
    &SND1_157_data,
    &SND1_158_data,
    &SND1_159_data,
    &SND1_160_data,
    &SND1_161_data,
};

const GBA_SoundSample CKM_GBA_Samples [] = {
    {&ck_music_0 , ck_music_0_bytes, GBA_INV_11025, GBA_SAMP_RATE_11025 }
    
};


// TODO:
// Include all the sounds :E
#endif

