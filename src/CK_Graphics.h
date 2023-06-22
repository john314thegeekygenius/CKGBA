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


// MODDERS:
// Can change number of bitmaps here
#define CK_BITMAP_START 6

typedef enum {
	H_HELPPIC,                   // 6
	H_LARROWPIC,                 // 7
	H_RARROWPIC,                 // 8
	H_ESCPIC,                    // 9
	H_ENTERPIC,                  // 10
	DUMMYPIC,                    // 11
	H_STORY1PIC,                 // 12
	H_STORY2PIC,                 // 13
	H_STORY3PIC,                 // 14
	H_STORY4PIC,                 // 15
	STORY5PIC,                   // 16
	STORY6PIC,                   // 17
	STORY7PIC,                   // 18
	STORY8PIC,                   // 19
	ITEM1PIC,                    // 20
	ITEM2PIC,                    // 21
	ITEM3PIC,                    // 22
	ITEM4PIC,                    // 23
	ITEM5PIC,                    // 24
	ITEM6PIC,                    // 25
	ITEM7PIC,                    // 26
	ITEM8PIC,                    // 27
	ITEM9PIC,                    // 28
	ARACHNUTPIC,                 // 29
	BERKELOISPIC,                // 30
	BOUNDERPIC,                  // 31
	COUNCILMEMBERPIC,            // 32
	DOPEFISHPIC,                 // 33
	INCHWORMPIC,                 // 34
	LICKPIC,                     // 35
	MADMUSHROOMPIC,              // 36
	POISONSLIGPIC,               // 37
	PRINCESSLINDSEYPIC,          // 38
	SCHOOLFISHPIC,               // 39
	SKYPESTPIC,                  // 40
	SPRITEPIC,                   // 41
	WORMOUTHPIC,                 // 42
	ENDOFTEXTPIC,                // 43
	H_MCPIC,                     // 44
	H_HANDPIC,                   // 45
	H_VISAPIC,                   // 46
	H_FLASHARROW1PIC,            // 47
	H_FLASHARROW2PIC,            // 48
	ENDINDG1PIC,                 // 49
	ENDINDG2PIC,                 // 50
	ENDINDG3PIC,                 // 51
	ENDINDG4PIC,                 // 52
	ENDINDG5PIC,                 // 53
	ENDINDG6PIC,                 // 54
	ENDINDG7PIC,                 // 55
	ENDINDG8PIC,                 // 56
	ENDINDG9PIC,                 // 57
	ENDINDG10PIC,                // 58
	ENDINDG11PIC,                // 59
	ENDINDG12PIC,                // 60
	ENDINDG13PIC,                // 61
	ENDINDG14PIC,                // 62
	ENDINDG15PIC,                // 63
	ENDINDG16PIC,                // 64
	ENDINDG17PIC,                // 65
	ENDINDG18PIC,                // 66
	ENDINDG19PIC,                // 67
	ENDINDG20PIC,                // 68
	ENDINDG21PIC,                // 69
	ENDINDG22PIC,                // 70
	ENDINDG23PIC,                // 71
	ENDINDG24PIC,                // 72
	ENDINDG25PIC,                // 73
	ENDINDG26PIC,                // 74
	ENDINDG27PIC,                // 75
	ENDINDG28PIC,                // 76
	ENDINDG29PIC,                // 77
	ENDINDG30PIC,                // 78
	H_IDLOGOPIC,                 // 79
//	H_KEEN5PIC,                  // 86
//	H_KEEN6PIC,                  // 87

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
	KEENTALK1WATERPIC,                // 122
	KEENTALK2WATERPIC,                // 123

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
void CK_FixPaletteU(); // For Universal.c

extern boolean	screenfaded;

void VW_FixGraphics();
void VW_ClearVideo(unsigned int color);
void VW_ClearScroll();
void RF_FixOfs(int x, int y);
void RF_RestoreOfs();

void VWB_Bar(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char c);
void VWB_DrawTile8M(unsigned int x, unsigned int y, unsigned short tile);
void VWB_DrawPic (int x, int y, int chunknum);
void VWB_DrawPicStory(int x, int y, int chunknum);
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
