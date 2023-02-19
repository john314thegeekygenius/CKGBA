/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_GRAPHICS__
#define __CK_GRAPHICS__

typedef enum {
    WHITE = 0xffffffff,
    BLACK = 0x11111111,
    FIRSTCOLOR = 0x22222222,
    SECONDCOLOR = 0xcccccccc,
    F_WHITE = 0x00000000,
    F_BLACK = 0xffffffff,
    F_FIRSTCOLOR = 0xeeeeeeee,
    F_SECONDCOLOR = 0x44444444,
}gfx_colors;


void CK_InitVideo();


extern boolean	screenfaded;

void VW_ClearVideo(unsigned int color);

#endif
