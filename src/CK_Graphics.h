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

const enum {
	CK_EGA_CLEAR = 0,
	CK_EGA_BLACK = 1,
	CK_EGA_DBLUE = 2,
	CK_EGA_DGREEN = 3,
	CK_EGA_CYAN = 4,
	CK_EGA_DRED = 5,
	CK_EGA_PURPLE = 5,
	CK_EGA_BROWN = 6,
	CK_EGA_GREY = 7,
	CK_EGA_DGREY = 8,
	CK_EGA_BLUE = 9,
	CK_EGA_GREEN = 10,
	CK_EGA_LBLUE = 11,
	CK_EGA_RED = 12,
	CK_EGA_PINK = 13,
	CK_EGA_YELLOW = 14,
	CK_EGA_WHITE = 15,
};

// Converts a color to a usable format
#define CK_TXTCOL(c) ( c | c<<4 | c<<8 | c<<12 | c<<16 | c<<20 | c<<24 | c<<28)

#define TILEMAP_BLOCK_0 0x00
#define TILEMAP_BLOCK_1 0x04

#define TILEMAP_MAP_0 0x1E00
#define TILEMAP_MAP_1 0x1F00


extern volatile uint16_t *TILEMAP_0;
extern volatile uint16_t *TILEMAP_1;

extern volatile uint16_t* TILESTART_0;
extern volatile uint16_t* TILESTART_1;

extern const unsigned short COMMANDER_KEEN_PALETTE[];

extern unsigned short CK_PaletteSet;

void CK_InitVideo();
void CK_FixPalette();

extern boolean	screenfaded;

void VW_ClearVideo(unsigned int color);
void VW_ClearScroll();
void VW_Bar(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char c);

#define VW_SetDefaultColors() CK_FixPalette()

#endif
