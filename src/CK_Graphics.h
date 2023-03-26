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

#ifndef __CK_GRAPHICS__
#define __CK_GRAPHICS__

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

typedef enum {
    WHITE = 0xffffffff,
    BLACK = 0x11111111,
    FIRSTCOLOR = 0x22222222,
    SECONDCOLOR = 0xcccccccc,
    F_WHITE = CK_EGA_WHITE,
    F_BLACK = CK_EGA_BLACK,
    F_FIRSTCOLOR = 0xe,
    F_SECONDCOLOR = 0x4,
}gfx_colors;


typedef enum {
	H_HELPPIC,                   // 6
	H_HANDPIC,                   // 45

	CP_MAINMENUPIC,              // 88
	CP_NEWGAMEMENUPIC,           // 89
	CP_LOADMENUPIC,              // 90
	CP_SAVEMENUPIC,              // 91
	CP_CONFIGMENUPIC,            // 92
	CP_SOUNDMENUPIC,             // 93
	CP_MUSICMENUPIC,             // 94
	CP_KEYBOARDMENUPIC,          // 95
	CP_KEYMOVEMENTPIC,           // 96
	CP_KEYBUTTONPIC,             // 97
	CP_JOYSTICKMENUPIC,          // 98
	CP_OPTIONSMENUPIC,           // 99
	CP_PADDLEWARPIC,             // 100
	CP_QUITPIC,                  // 101
	CP_VIDEOMOVEMENTPIC,		 // 102

	ORACLEPIC,                   // 110
	KEENTALK1PIC,                // 111
	KEENTALK2PIC,                // 112
	KEENMADPIC,                  // 113
	LINDSEYPIC,                  // 114
	KEENCOUNT1PIC,               // 115
	KEENCOUNT2PIC,               // 116
	KEENCOUNT3PIC,               // 117
	KEENCOUNT4PIC,               // 118
	KEENCOUNT5PIC,               // 119
	KEENCOUNT6PIC,               // 120
	//
	// MASKED PICS
	//

	CP_MENUMASKPICM,             // 121

}bitmap_gfxs;


// Converts a color to a usable format
#define CK_TXTCOL(c) ( c | c<<4 | c<<8 | c<<12 | c<<16 | c<<20 | c<<24 | c<<28)

#define TILEMAP_BLOCK_0 0x00
#define TILEMAP_BLOCK_1 0x04

#define TILEMAP_MAP_0 0x1E00
#define TILEMAP_MAP_1 0x1F00
#define TILEMAP_MAP_2 0x1D00


extern uint16_t *TILEMAP_0;
extern uint16_t *TILEMAP_1;
extern uint16_t *TILEMAP_2;

extern uint16_t* TILESTART_0;
extern uint16_t* TILESTART_1;

extern const unsigned short COMMANDER_KEEN_PALETTE[];

extern unsigned short CK_PaletteSet;

void CK_InitVideo();
void CA_FixGraphics();

void CK_FixPalette();

extern boolean	screenfaded;

void VW_FixGraphics();
void VW_ClearVideo(unsigned int color);
void VW_ClearScroll();
void RF_FixOfs(int x, int y);
void VWB_Bar(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char c);
void VWB_DrawTile8M(unsigned int x, unsigned int y, unsigned short tile);
void VWB_DrawPic (int x, int y, int chunknum);
void VWB_DrawMPic (int x, int y, int chunknum);
void VWB_DrawFullPic(unsigned char *data);


void VWB_Plot (int x, int y, unsigned char color);
void VWB_Hlin (int x1, int x2, int y, int color);
void VWB_Vlin (int y1, int y2, int x, int color);

DONT_OPTIMISE void VW_WaitVBL(unsigned int vbls);

void VW_UpdateScreen();
void VWB_ClearSpriteCache();
objsprite *VWB_GetTempSprite(CK_SpriteType type);
void VWB_DrawSprite(objsprite **sprite, int x, int y, int chunknum);

#define VW_SetDefaultColors() CK_FixPalette()


#endif
