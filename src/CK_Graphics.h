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

#define TILEMAP_BLOCK_0 0x00
#define TILEMAP_BLOCK_1 0x04

#define TILEMAP_MAP_0 0x1E00
#define TILEMAP_MAP_1 0x1F00


extern volatile uint16_t *TILEMAP_0;
extern volatile uint16_t *TILEMAP_1;

extern volatile uint16_t* TILESTART_0;
extern volatile uint16_t* TILESTART_1;

extern const unsigned short COMMANDER_KEEN_PALETTE[];

void CK_InitVideo();
void CK_FixPalette();

extern boolean	screenfaded;

void VW_ClearVideo(unsigned int color);

#define VW_SetDefaultColors() CK_FixPalette()

#endif
