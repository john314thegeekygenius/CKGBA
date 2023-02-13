
#ifndef __GBA_DEFS__
#define __GBA_DEFS__


#define MULTIBOOT_ROM int __gba_multiboot;

#define DONT_OPTIMISE __attribute__((optimize("O0"))) 

// We don't want to mix audio right now
//#define GBA_MIX_MY_AUDIO 1

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;


#define GBA_ABS(x) ((x)<0?-(x):(x))

int GBA_StrLen(char*str);
void GBA_Delay(uint32_t ms);

void GBA_SRand(uint32_t s);
uint32_t GBA_Rand();

#ifndef NULL
#define NULL ((void*)0)
#endif

extern unsigned int GBA_VSyncCounter;


//locations for all the different types of memory

#define GBA_WRAM			((uint16_t *)0x2000000)
#define GBA_IRAM			((uint16_t *)0x3000000)
//#define GBA_PAL_BG			((uint16_t *)0x5000000)
//#define GBA_PAL_OBJ			((uint16_t *)0x5000200)
//#define GBA_VRAM			((uint16_t *)0x6000000)
//#define GBA_OAM_MEM			((uint16_t *)0x6010000)

#define GBA_IN_EWRAM __attribute__((section (".ewram")))
#define GBA_IN_IWRAM __attribute__((section (".iwram")))


#define GBA_GAMEPAK_RAM_START 0x0E000000

////////////// DMA Stuff

#define GBA_DMA_SRC  0x40000D4
#define GBA_DMA_DEST 0x40000D8
#define GBA_DMA_COUNT 0x40000DC

#define GBA_DMA1_SRC  0x40000BC
#define GBA_DMA1_DEST 0x40000C0
#define GBA_DMA1_COUNT 0x40000C4

#define GBA_DMA2_SRC  0x40000C8
#define GBA_DMA2_DEST 0x40000CC
#define GBA_DMA2_COUNT 0x40000D0


// Flag for turning on DMA
#define GBA_DMA_ENABLE 0x80000000

// flags for the sizes to transfer, 16 or 32 bits
#define GBA_DMA_16 0x00000000
#define GBA_DMA_32 0x04000000


/* this causes the DMA destination to be the same each time rather than increment */
#define GBA_DMA_DEST_FIXED 0x400000

/* this causes the DMA source to be the same each time rather than increment */
#define GBA_DMA_SRC_FIXED 0x1000000

/* this causes the DMA to repeat the transfer automatically on some interval */
#define GBA_DMA_REPEAT 0x2000000

/* this causes the DMA repeat interval to be synced with timer 0 */
#define GBA_DMA_SNC_TO_TIMER 0x30000000

/* this causes the DMA transfer at VBlank */
#define GBA_DMA_SNC_TO_VBLANK 0x1000
/* this causes the DMA transfer at HBlank */
#define GBA_DMA_SNC_TO_VBLANK 0x2000
/* this causes the DMA transfer at line draw for DMA3??? */
#define GBA_DMA_SNC_TO_LINE 0x3000


// Copy data using DMA 
void GBA_DMA_Copy16(uint16_t* dest, uint16_t* source, int amount);
inline void GBA_DMA_Copy32(uint32_t* dest, uint32_t* source, int amount) ;

void GBA_DMA_MemSet16(uint16_t* dest, uint16_t val, int len);
void GBA_DMA_MemSet32(uint32_t* dest, uint32_t val, int len);

// ASM Copy

void GBA_ASM_Copy16(uint16_t* dest, uint16_t* source, int amount);
void GBA_ASM_Copy32(uint32_t* dest, uint32_t* source, int amount);

//void GBA_ASM_Memset16(uint16_t* dest, uint16_t* source, int amount);
//void GBA_ASM_Memset32(uint32_t* dest, uint32_t* source, int amount);


/////////// Interupts


/* the global interrupt enable register */
#define GBA_INT_ENABLE 0x4000208

/* this register stores the individual interrupts we want */
#define GBA_INT_SELECT 0x4000200

/* this registers stores which interrupts if any occured */
#define GBA_INT_STATE 0x4000202

/* the address of the function to call when an interrupt occurs */
#define GBA_INT_CALLBACK 0x3007FFC

/* this register needs a bit set to tell the hardware to send the vblank interrupt */
#define GBA_DISP_INT 0x4000004

/* the interrupts are identified by number, we only care about this one */
#define GBA_INT_VBLANK 0x1



////////////// Video Stuff

#define GBA_SCREEN_WIDTH 240
#define GBA_SCREEN_HEIGHT 160
#define GBA_SCREEN_SIZE (240*160)

#define GBA_SCR_MEMSET_SIZE32 9600
#define GBA_SCR_MEMSET_SIZE16 19200

#define GBA_MODE_0 0x00 // 4 Text backgrounds
#define GBA_MODE_1 0x01 // 3 Text backgrounds
#define GBA_MODE_2 0x02 // Tiles backgrounds
#define GBA_MODE_3 0x03 // Bitmap 240x160x32768
#define GBA_MODE_4 0x04 // Bitmap 240x160x256
#define GBA_MODE_5 0x05 // Bitmap 160x128x32768


/*
Sets whether sprites stored in VRAM use 1 dimension or 2.
1 - 1d: tiles are are stored sequentially 
0 - 2d: each row of tiles is stored 32 x 64 bytes in from the start of the
previous row.
*/
#define GBA_SPR_IN_VRAM 0x40

#define GBA_FORCE_BLANK 0x80 // Force screen to blank
#define GBA_ENABLE_BG0 0x100 // Enable BG0
#define GBA_ENABLE_BG1 0x200 // Enable BG1
#define GBA_ENABLE_BG2 0x400 // Enable BG2
#define GBA_ENABLE_BG3 0x800 // Enable BG3
#define GBA_ENABLE_OAM 0x1000 // Enable OAM sprites
#define GBA_ENABLE_WIN0 0x2000 // Enable Window 0
#define GBA_ENABLE_WIN1 0x4000 // Enable Window 1
#define GBA_ENABLE_SPRWIN 0x8000 // Enable Sprite Windows

#define GBA_REG_DISPCNT 0x04000000 // Use above values to set


#define GBA_REG_DISPSTAT 0x04000004
#define GBA_REG_VCOUNT 0x04000006 // READ ONLY


#define GBA_BG_SIZE_32x32 0x0000 // 32x32 tiles (256x256 pixels)
#define GBA_BG_SIZE_64x32 0x4000 // 64x32 tiles (512x256 pixels)
#define GBA_BG_SIZE_32x64 0x8000 // 32x64 tiles (256x512 pixels)
#define GBA_BG_SIZE_64x64 0xC000 // 64x64 tiles (512x512 pixels)

#define GBA_BG_TOP   0x00 // Draw background layer on top
#define GBA_BG_FRONT 0x01 // Next layer down
#define GBA_BG_MID   0x02 // Second to last layer
#define GBA_BG_BACK  0x03 // Drawn behind all other layers

#define GBA_SHOW_BACK 0x10 // Show the back buffer for bitmap mode

#define GBA_BG_WRAP 0x2000 // Should the background layer wrap around
#define GBA_BG_MOSAIC 0x40 // Make the background have mosaic effect
#define GBA_BG_256COLORS 0x80 // Make the BG palette 256 colors of 8 bits

#define GBA_REG_VTRACE 0x4000006
#define GBA_REG_BG0CNT 0x4000008
#define GBA_REG_BG1CNT 0x400000A
#define GBA_REG_BG2CNT 0x400000C
#define GBA_REG_BG3CNT 0x400000E

#define GBA_REG_BGMOSAC 0x400004C // Modify the mosaic size

#define GBA_SET_MOSAIC(x,y) *(volatile uint16_t*) GBA_REG_BGMOSAC = (((y)&0xF)<<4)+((x)&0xF);


/*
Address: 0x4000010 -  Horizontal scroll co-ordinate for BG0 (Write Only)
Address: 0x4000012 -  Vertical scroll co-ordinate for BG0 (Write Only)
Address: 0x4000014 -  Horizontal scroll co-ordinate for BG1 (Write Only)
Address: 0x4000016 -  Vertical scroll co-ordinate for BG1 (Write Only)
Address: 0x4000018 -  Horizontal scroll co-ordinate for BG2 (Write Only)
Address: 0x400001A -  Vertical scroll co-ordinate for BG2 (Write Only)
Address: 0x400001C -  Horizontal scroll co-ordinate for BG3 (Write Only)
Address: 0x400001E -  Vertical scroll co-ordinate for BG3 (Write Only)
*/
#define GBA_REG_BG0HOFS 0x4000010
#define GBA_REG_BG0VOFS 0x4000012
#define GBA_REG_BG1HOFS 0x4000014
#define GBA_REG_BG1VOFS 0x4000016
#define GBA_REG_BG2HOFS 0x4000018
#define GBA_REG_BG2VOFS 0x400001A
#define GBA_REG_BG3HOFS 0x400001C
#define GBA_REG_BG3VOFS 0x400001E


/*
F E D C  B A 9 8  7 6 5 4  3 2 1 0 
S I I I  I I I I  F F F F  F F F F 

0-7 (F) = Fraction 
8-E (I) = Integer 
F   (S) = Sign bit 

These registers apply only to Rotate/Scale backgrounds. Individual descriptions follow:

Address: 0x4000020 - REG_BG2PA (BG2 Read Source Pixel X Increment)(Write Only)
Address: 0x4000030 - REG_BG3PA (BG3 Read Source Pixel X Increment) (Write Only)

The effect of these registers is to scale the background (relative to the upper left corner) in the x direction by an amount equal to 1/(register value).

Address: 0x4000022 - REG_BG2PB (BG2 Write Destination Pixel X Increment) (Write Only)
Address: 0x4000032 - REG_BG3PB (BG3 Write Destination Pixel X Increment) (Write Only)

The effect of these registers is to shear the x coordinates of the background over y, relative to the upper left corner. A value of 0 will result in no shearing, a value of 1.00 will make the background appear to be sheared left as you go down the screen, and a value of -1 will make the background appear sheared right as you go down the screen.

Address: 0x4000024 - REG_BG2PC (BG2 Read Source Pixel Y Increment) (Write Only)
Address: 0x4000034 - REG_BG3PC (BG3 Read Source Pixel Y Increment) (Write Only)

The effect of these registers is to shear the y coordinates of the background over x, relative to the upper left corner. A value of 0 will result in no shearing, a value of 1.00 will make the background appear to be sheared upwards to the right, and a value of -1 will make the background appear sheared downwards and to the right.

Address: 0x4000026 - REG_BG2PD (BG2 Write Destination Pixel Y Increment) (Write Only)
Address: 0x4000036 - REG_BG3PD (BG3 Write Destination Pixel Y Increment) (Write Only)

The effect of these registers is to scale the background in the y direction (relative to the upper left corner) by an amount equal to 1/(register value).


Address: 0x4000028 - REG_BG2X (X Coordinate for BG2 Rotational Background)(Write Only)
Address: 0x4000038 - REG_BG3X (X Coordinate for BG3 Rotational Background)(Write Only)

Address: 0x400002C - REG_BG2Y (Y Coordinate for BG2 Rotational Background)(Write Only)
Address: 0x400003C - REG_BG3Y (Y Coordinate for BG3 Rotational Background)(Write Only)

31 30 29 28  27 26 25 24  23 22 21 20  19 18 17 16  15 14 13 12  11 10 9 8  7 6 5 4  3 2 1 0
X  X  X  X   S  I  I  I   I  I  I  I   I  I  I  I   I  I  I  I   I  I  I I  F F F F  F F F F 

0-7  (F) - Fraction 
8-26 (I) - Integer 
27   (S) - Sign bit 
*/


#define GBA_REG_BG2PA 0x4000020
#define GBA_REG_BG2PB 0x4000022
#define GBA_REG_BG2PC 0x4000024
#define GBA_REG_BG2PD 0x4000026
#define GBA_REG_BG2PX 0x4000028
#define GBA_REG_BG2PY 0x400002C

#define GBA_REG_WIN0H 0x4000040
#define GBA_REG_WIN1H 0x4000042
#define GBA_REG_WIN0V 0x4000044
#define GBA_REG_WIN1V 0x4000046
#define GBA_REG_WININ 0x4000048


#define GBA_REG_WIN0H 0x4000040
#define GBA_REG_WIN1H 0x4000042
#define GBA_REG_WIN0V 0x4000044
#define GBA_REG_WIN1V 0x4000046
#define GBA_REG_WININ 0x4000048
//#define GBA_
//#define GBA_


#define GBA_PAL_SIZE 0x100
#define GBA_PAL_START 0x05000000
#define GBA_PAL_BG_START 0x05000000
#define GBA_PAL_SPR_START 0x05000200

#define GBA_VRAM 0x06000000
#define GBA_VRAMBUFF 0x0600A000
#define GBA_VSRAM 0x06010000
#define GBA_ORAM 0x07000000


#define GBA_CHAR_BLOCK_SIZE 0x4000 // (128*128)

// return a pointer to one of the 4 character blocks (0-3)
// they are each 16K big 
#define GBA_CHAR_BLOCK(block) (GBA_VRAM + ((block) * 0x4000))

// Locations:
// 0x06000000
// 0x06004000
// 0x06008000
// 0x0600C000

// End by:
// 0x07000000

// return a pointer to one of the 32 screen blocks (0-31)
// they are each 2K big 
#define GBA_SCREEN_BLOCK(block) (GBA_VRAM + ((block) * 0x800))
// Locations:
// map0 - 0x06000000  : tiles1 - 0x06004000
// map2 - 0x06001000  : tiles2 - 0x06008000
// map4 - 0x06002000  : tiles3 - 0x0600C000
// map6 - 0x06003000  : tiles3 - 0x0600C000
// 
// 
// 
// 
// 
// These overwrite sprite data!!
// 0x06010000  -- block 20 (20<<8) 1400h
// 0x06010800  -- block 21 (21<<8) 1500h
// 0x06011000  -- block 22 (22<<8) 1600h
// 0x06011800  -- block 23 (23<<8) 1700h

#define GBA_WAIT_VBLANK while (*(volatile uint16_t*) GBA_REG_VTRACE < 160) { }

#define GBA_WAIT_VBLANKN(n) while (*(volatile uint16_t*) GBA_REG_VTRACE < n) { }

#define GBA_WAIT_FOR_VBLANKN(n) while (*(volatile uint16_t*) GBA_REG_VTRACE != n) { }


// set all control the bits in this register
#define GBA_FINISH_BG0(flags) *(volatile uint16_t*)GBA_REG_BG0CNT = flags | GBA_BG_256COLORS;
#define GBA_FINISH_BG1(flags) *(volatile uint16_t*)GBA_REG_BG1CNT = flags | GBA_BG_256COLORS;
#define GBA_FINISH_BG2(flags) *(volatile uint16_t*)GBA_REG_BG2CNT = flags | GBA_BG_256COLORS;
#define GBA_FINISH_BG3(flags) *(volatile uint16_t*)GBA_REG_BG3CNT = flags | GBA_BG_256COLORS;

#define GBA_FINISH_BG0_4BIT(flags) *(volatile uint16_t*)GBA_REG_BG0CNT = flags ;
#define GBA_FINISH_BG1_4BIT(flags) *(volatile uint16_t*)GBA_REG_BG1CNT = flags ;
#define GBA_FINISH_BG2_4BIT(flags) *(volatile uint16_t*)GBA_REG_BG2CNT = flags ;
#define GBA_FINISH_BG3_4BIT(flags) *(volatile uint16_t*)GBA_REG_BG3CNT = flags ;

/*
    	GBA_BG_BACK         | /* priority, 0 is highest, 3 is lowest 
        (CHAR_BLOCK << 2)  | /* the char block the image data is stored in 
        GBA_BG_MOSAIC      | /* the mosaic flag 
        GBA_BG_256COLORS   | /* color mode, 0 is 16 colors, 1 is 256 colors 
        (SCREEN_BLOCK << 8)| /* the screen block the tile data is stored in 
        GBA_BG_WRAP        | /* wrapping flag 
        GBA_BG_SIZE_32x32;  /* bg size, 0 is 256x256 
*/

#define GBA_REG_BLDMOD 0x4000050

#define GBA_BLEND_BGS(flags) *(volatile uint16_t*)GBA_REG_BLDMOD = flags;


// Source
#define GBA_BLD_S_BG0 0x01
#define GBA_BLD_S_BG1 0x02
#define GBA_BLD_S_BG2 0x04
#define GBA_BLD_S_BG3 0x08
#define GBA_BLD_S_SPR 0x10
#define GBA_BLD_S_BKD 0x20

// Blend Type
#define GBA_BLDM_NONE  0x00
#define GBA_BLDM_APLHA 0x40
#define GBA_BLDM_LIGHT 0x80
#define GBA_BLDM_DARK  0xC0

// Destination
#define GBA_BLD_D_BG0 0x0100
#define GBA_BLD_D_BG1 0x0200
#define GBA_BLD_D_BG2 0x0400
#define GBA_BLD_D_BG3 0x0800
#define GBA_BLD_D_SPR 0x1000
#define GBA_BLD_D_BKD 0x2000

#define GBA_SPRITE_MAP_2D 0x00
#define GBA_SPRITE_MAP_1D 0x40
#define GBA_SPRITE_ENABLE 0x1000

#define GBA_NUM_SPRITES 128
#define GBA_SPRITE_START 0x7000000
#define GBA_SPRITE_256 0x2000 // Make sure the sprite is 256 colors

#define GBA_SPRITE_FLIPH 0x1000 // Flip the sprite horizontaly
#define GBA_SPRITE_FLIPV 0x2000 // Flip the sprite virticaly

#define GBA_SPRITE_ZTOP   0x000 // Render the sprite on top
#define GBA_SPRITE_ZFRONT 0x400 // 
#define GBA_SPRITE_ZMID   0x800 // 
#define GBA_SPRITE_ZBACK  0xC00 // Render the sprite on bottom


#define GBA_SPRITE_RNORM   0x000 // Render the sprite solid
#define GBA_SPRITE_RTANS   0x400 // Render the sprite transparent
#define GBA_SPRITE_ROBJW   0x800 // Render the sprite in window ???
#define GBA_SPRITE_RDONT   0xC00 // Do not use!


// 1024 chunks of 32 bytes
#define GBA_SPRGFX_START 0x6010000

typedef struct GBA_Sprite {
	uint16_t a0;
	uint16_t a1;
	uint16_t a2;
	uint16_t a3;
}GBA_Sprite;


// The different sizes of sprites which are possible
typedef enum GBA_SpriteSizes {
    GBA_SPR_8_8,
    GBA_SPR_16_16,
    GBA_SPR_32_32,
    GBA_SPR_64_64,
    GBA_SPR_16_8,
    GBA_SPR_32_8,
    GBA_SPR_32_16,
    GBA_SPR_64_32,
    GBA_SPR_8_16,
    GBA_SPR_8_32,
    GBA_SPR_16_32,
    GBA_SPR_32_64
}GBA_SpriteSizes;

extern GBA_Sprite GBA_SpriteList[GBA_NUM_SPRITES];
extern uint16_t GBA_SpriteIndex;

typedef uint16_t GBA_SpriteIndex_t;

// Function to create a new sprite in the list
// returns index into sprite array
GBA_SpriteIndex_t GBA_CreateSprite(int x, int y, GBA_SpriteSizes size, uint16_t tileIndex, int zLayer, int palette);
void GBA_RemakeSprite(GBA_SpriteIndex_t index, int x, int y, GBA_SpriteSizes size, uint16_t tileIndex, int zLayer, int palette);

#define GBA_SET_SPRITE_CLEAR(index) \
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a0 = GBA_SCREEN_WIDTH+32; \
	GBA_SpriteList[(index)].a1 = GBA_SCREEN_HEIGHT+32; \
	GBA_SpriteList[(index)].a2 = 0; \
	GBA_SpriteList[(index)].a3 = 0; \
	}

#define GBA_SET_SPRITE_PAL(index,palette)\
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a2 &= 0xFFF; \
	GBA_SpriteList[(index)].a2 |= ((palette)<<12); \
	}
	
#define GBA_SET_SPRITE_POSITION(index,x,y) \
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a0 &= 0xFF00; \
	GBA_SpriteList[(index)].a0 |= (y)&0xFF; \
	GBA_SpriteList[(index)].a1 &= 0xFE00; \
	GBA_SpriteList[(index)].a1 |= (x)&0x1FF;\
	}

#define GBA_SET_FLIPV(index,flip) \
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a1 &= 0xDFFF; \
	GBA_SpriteList[(index)].a1 |= (flip)<<13;\
	}

#define GBA_SET_FLIPH(index,flip) \
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a1 &= 0xEFFF; \
	GBA_SpriteList[(index)].a1 |= (flip)<<12;\
	}

#define GBA_SET_SPRITE_TILES(index,tileindex) \
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a2 &= 0xFC00; \
	GBA_SpriteList[(index)].a2 |= (tileindex)&0x3FF;\
	}

#define GBA_SET_RENDER(index,rend) \
	if((index)>=0&&(index)<128){\
	GBA_SpriteList[(index)].a1 &= 0xF3FF; \
	GBA_SpriteList[(index)].a1 |= (rend);\
	}

#define GBA_UPDATE_SPRITES() GBA_DMA_Copy32((uint32_t*)GBA_SPRITE_START, (uint32_t*)GBA_SpriteList, GBA_NUM_SPRITES << 1);

#define GBA_UPDATE_SPRITE(index) if((index)>=0&&(index)<128) GBA_DMA_Copy16((uint16_t*)GBA_SPRITE_START+(index<<2), (uint16_t*)&GBA_SpriteList[index], 4);

// Moves all the sprites off the screen to hide them
void GBA_ResetSprites();
void GBA_HideSprites();
void GBA_RemoveSprite(uint16_t id);

//////// Input

#define GBA_BUTTON_REG 0x04000130

#define GBA_BUTTON_A 		0x01
#define GBA_BUTTON_B 		0x02
#define GBA_BUTTON_SELECT 	0x04
#define GBA_BUTTON_START 	0x08
#define GBA_BUTTON_RIGHT 	0x10
#define GBA_BUTTON_LEFT 	0x20
#define GBA_BUTTON_UP 		0x40
#define GBA_BUTTON_DOWN 	0x80
#define GBA_BUTTON_RSHOLDER 0x100
#define GBA_BUTTON_LSHOLDER 0x200

#define GBA_BUTTONS (*(volatile uint16_t*)GBA_BUTTON_REG)&0x3FF
#define GBA_INV_BUTTONS (~GBA_BUTTONS)
#define GBA_TEST_BUTTONS(buttons) (GBA_INV_BUTTONS & (buttons)) == (buttons)


///// Sound

#define GBA_SOUND1_L 0x04000060 // Sound 1 sweep control
#define GBA_SOUND1_H 0x04000062 // Sound 1 length, wave duty, and evelope control
#define GBA_SOUND1_X 0x04000064 // Sound 1 frequency, reset and loop control

#define GBA_SOUND2_L 0x04000068 // Sound 2 length, wave duty, and evelope control
#define GBA_SOUND2_H 0x0400006C // Sound 2 frequency, reset and loop control

#define GBA_SOUND3_L 0x04000070 // Sound 3 enable and wave ram bank control
#define GBA_SOUND3_H 0x04000072 // Sound 3 sound length and output level control 
#define GBA_SOUND3_X 0x04000074 // Sound 3 frequency, reset and loop control

#define GBA_SOUND4_L 0x04000078 // Sound 4 sound length, output level, and evelope control 
#define GBA_SOUND4_H 0x0400007C // Sound 4 noise parameters, reset and loop control

#define GBA_SOUNDCNT_L 0x04000080 // Sound 1-4 output level and stereo control
#define GBA_SOUNDCNT_H 0x04000082 // Direct sound control and sound 1-4 output ratio
#define GBA_SOUNDCNT_X 0x04000084 // Master sound enable and sound 1-4 play status

#define GBA_SOUND_MASTER_ENABLE 0x80

#define GBA_SND_1L (1<<8)
#define GBA_SND_1R (1<<12)
#define GBA_SND_2L (2<<8)
#define GBA_SND_2R (2<<12)
#define GBA_SND_3L (4<<8)
#define GBA_SND_3R (4<<12)
#define GBA_SND_4L (8<<8)
#define GBA_SND_4R (8<<12)

#define GBA_RESET_SOUND 0x8000
#define GBA_LOOP_SOUND 0x4000

#define GBA_SOUND_BIAS 0x04000088 // Sound bias and amplitude resolution control

#define GBA_SOUND1_DUTY_12 0x00
#define GBA_SOUND1_DUTY_25 0x40
#define GBA_SOUND1_DUTY_50 0x80
#define GBA_SOUND1_DUTY_75 0xC0



#define GBA_SOUND_RATIO_25 0
#define GBA_SOUND_RATIO_50 1
#define GBA_SOUND_RATIO_100 2

#define GBA_SOUND_RATIO_UNKNOWN 3 // Dont use this

/*
4-6 (T) = Sweep Time. This is the delay between sweep shifts. After each delay,
          the frequency increments or decrements.
          000:  Disable sweep function
          001:  Ts=1 / 128khz (7.8 ms)
          010:  Ts=2 / 128khz (15.6 ms)
          011:  Ts=3 / 128 khz (23.4 ms)
          100:  Ts=4 / 128 khz (31.3 ms)
          101:  Ts=5 / 128 khz (39.1 ms)
          110:  Ts=6 / 128 khz (46.9 ms)
          111:  Ts=7 / 128 khz (54.7 ms)
*/

#define GBA_SWEEP_NONE 0
#define GBA_SWEEP_7    (1<<4)
#define GBA_SWEEP_15   (2<<4)
#define GBA_SWEEP_23   (3<<4)
#define GBA_SWEEP_31   (4<<4)
#define GBA_SWEEP_39   (5<<4)
#define GBA_SWEEP_46   (6<<4)
#define GBA_SWEEP_57   (7<<4)

// value*(1/64) seconds.

#define GBA_ENV_0 0
#define GBA_ENV_1 (1<<8)
#define GBA_ENV_2 (2<<8)
#define GBA_ENV_3 (3<<8)
#define GBA_ENV_4 (4<<8)
#define GBA_ENV_5 (5<<8)
#define GBA_ENV_6 (6<<8)
#define GBA_ENV_7 (7<<8)

/*
Envelope mode. Controls if the envelope is to increase or decrease in
          volume over time.
          0 - Envelope decreases
          1 - Envelope increases
*/

#define GBA_ENVM_INC (1<<11)




#define GBA_AUDIO_RATE 0x400000
#define GBA_SOUND_RATE 0x20000



#define GBA_VOL_MAX 7

void GBA_InitAudio(void);
void GBA_SetGlobalVolume(short L_Volume, short R_Volume);
void GBA_PlaySound(short id);
void GBA_StopSound(short id);
void GBA_ResetSound(short id);
void GBA_SetSoundSqWav(short id);
void GBA_SetSoundFreq(short id, int freq);

#define GBA_HZ_2_REG(hz) (-((GBA_SOUND_RATE / hz)-2048))

/* bit patterns for the sound control register */
#define GBA_DSND_A_RATIO      0x04
#define GBA_DSND_A_RIGHT      0x100
#define GBA_DSND_A_LEFT       0x200
#define GBA_DSND_A_FIFO_RESET 0x800

#define GBA_DSND_B_RATIO      0x08
#define GBA_DSND_B_RIGHT      0x1000
#define GBA_DSND_B_LEFT       0x2000
#define GBA_DSND_B_FIFO_RESET 0x8000

#define GBA_DSND_TIMER0 0x0000
#define GBA_DSND_TIMER1 0x4000



/* the location of where sound samples are placed for each channel */
#define GBA_FIFO_BUFF_A  0x40000A0
#define GBA_FIFO_BUFF_B  0x40000A4


/*
*(volatile unsigned int*)GBA_DMA_SRC   = (unsigned int) sound;
*(volatile unsigned int*)GBA_DMA_DEST  = (unsigned int) GBA_FIFO_BUFF_A;
*(volatile unsigned int*)GBA_DMA_COUNT = GBA_DMA_DEST_FIXED | GBA_DMA_REPEAT | GBA_DMA_32 | GBA_DMA_SNC_TO_TIMER | GBA_DMA_ENABLE;

*/

/////////// Timers

/* make defines for the bit positions of the control register */
#define GBA_TIMER_FREQ_1    0x00
#define GBA_TIMER_FREQ_64   0x02
#define GBA_TIMER_FREQ_256  0x03
#define GBA_TIMER_FREQ_1024 0x04
#define GBA_TIMER_ENABLE    0x80

/* define the timer control registers */
#define GBA_TIMER0_DATA    0x4000100
#define GBA_TIMER0_CONTROL 0x4000102

/* define the timer control registers */
#define GBA_TIMER1_DATA    0x4000104
#define GBA_TIMER1_CONTROL 0x4000106

/* define the timer control registers */
#define GBA_TIMER2_DATA    0x4000108
#define GBA_TIMER2_CONTROL 0x400010A


#define GBA_CLOCK 16777216
#define GBA_VBLANK_CLOCK 280806
#define GBA_REFRESH_RATE 59.6781927

#define GBA_INSTRUCTIONS_PER_VSYNC 4705.336862522
#define GBA_INSTRUCTIONS_PER_SEC 78.422281042

////// Sample stuff


// InvSampR = 59.6781927 / SampleR

#define GBA_INVERSE_VCOUNT 0.000003561

#define GBA_INV_11025 0.005412988
#define GBA_INV_22050 0.002709921
#define GBA_INV_44100 0.001353247

#define GBA_INV_16K_22050 43.358736
#define GBA_INV_16K_44100 21.65088

#define GBA_NOLOOP_SAMPLE 0
#define GBA_LOOP_SAMPLE 1

#define GBA_CHANNEL_A 0
#define GBA_CHANNEL_B 1

// SampR = 16758000 / SampleR

#define GBA_SAMP_RATE_11025   1520
#define GBA_SAMP_RATE_22050   761
#define GBA_SAMP_RATE_44100   380


typedef struct GBA_SoundSample {
    unsigned char *sample;
    int num_samples;
    float sample_rate;
    int rate;
}GBA_SoundSample;

void GBA_PlaySample(GBA_SoundSample *sample, char loop, char channel);
void GBA_StopChannel(char channel);
void GBA_PauseChannel(char channel);
void GBA_PlayChannel(char channel);
int GBA_SamplePlaying(int channel);

////////// Sound Mixer


#define MIX_MAX_CHANNELS 4


typedef enum _MIX_FREQ
{
	MIX_FREQ_5734,
	MIX_FREQ_10512,
	MIX_FREQ_13379,
	MIX_FREQ_18157, // Recommened sample rate
	MIX_FREQ_21024,
	MIX_FREQ_26758,
	MIX_FREQ_31536,
	MIX_FREQ_36314,
	MIX_FREQ_40137,
	MIX_FREQ_42048,
	MIX_FREQ_43959,

	MIX_FREQ_NUM
} MIX_FREQ;



typedef struct _MIXER_CHANNEL
{
	char *data;
	unsigned int pos;
	unsigned int inc;
	unsigned int vol;
	unsigned int length;
	unsigned int loopLength;
	unsigned int endLength;

} MIXER_CHANNEL;

typedef struct _MIXER_VARS
{
	char		*mixBufferBase;
	char		*curMixBuffer;
	unsigned int mixBufferSize;
	char		activeBuffer;

} MIXER_VARS;




void GBA_InitMixer(MIX_FREQ sampleRate);
void GBA_MixAudio();
void GBA_VSyncIRQ();
void GBA_MixerPlaySample(int channel, GBA_SoundSample * sample, char loop);


////////// Link stuff

#define GBA_REG_SCD0 0x4000120
#define GBA_REG_SCD1 0x4000122
#define GBA_REG_SCD2 0x4000124
#define GBA_REG_SCD3 0x4000126

/*
Normal Mode (0x4000120 and 0x4000122 only)

31 30 29 28  27 26 25 24  23 22 21 20  19 18 17 16  15 14 13 12  11 10 9 8  7 6 5 4  3 2 1 0
D  D  D  D   D  D  D  D   D  D  D  D   D  D  D  D   D  D  D  D   D  D  D D  D D D D  D D D D

0-31 (D) = The 32-bit data to sent/received over the link cable.


Multi-Player Mode

R R R R  R R R R  R R R R  R R R R
F E D C  B A 9 8  7 6 5 4  3 2 1 0 
D D D D  D D D D  D D D D  D D D D

0-F (D) = The data received.

- SCD0 contains the data sent by the master (also called slave 0)
- SCD1 contains the data sent by the first slave (slave1)
- SCD2 contains the data sent by the second slave (slave2)
- SCD3 contains the data sent by the last slave (slave3)

These registers are automatically reset ot 0xFFFF on transfer start. 

*/

#define GBA_REG_SCCNT_L 0x4000128 // Serial Communication channel control register
#define GBA_REG_SCCNT_H 0x400012A // Serial Communication Source Register

#define GBA_REG_RCNT 0x4000134


// OR with GBA_REG_SCCNT_L

#define GBA_COM_BAUD_9600 0x00
#define GBA_COM_BAUD_38400 0x01
#define GBA_COM_BAUD_57600 0x02
#define GBA_COM_BAUD_115200 0x03

#define GBA_SERIAL_TIMEOUT 0x500 // ~ 5 seconds???

#define GBA_SI_TERMINAL 0x04 // READ ONLY

#define GBA_LINK_ERROR     0x01
#define GBA_TIMEOUT_ERROR  0x02
#define GBA_NOSERIAL_ERROR 0x03

// Returns 0 if error, 1 if all GBAs ready
#define GBA_COM_STATUS 0x08 // READ ONLY

// 0 - master   1-3 - slave 1-3
#define GBA_COM_ID 0x30 // READ ONLY

// Returns 1 on error
#define GBA_COM_ERROR 0x40 // READ ONLY

// Set to 1 as master to start transfer
#define GBA_COM_BUSY 0x80 // READ ONLY IF SLAVE
#define GBA_COM_START 0x80 // Same as busy bit

#define GBA_COM_DISCONNECTED 0xFFFF
#define GBA_COM_NODATA 0x00
#define GBA_COM_NOGBAS 0x0002 // From No$GBA ???

#define GBA_COM_8BIT 0x00
#define GBA_COM_16BIT 0x1000
#define GBA_COM_MULTI 0x2000
#define GBA_COM_UART 0x3000


#define GBA_OPPONENT_SO_HI 0x04
#define GBA_CLOCK_INTERNAL 0x01
#define GBA_MHZ_2 0x02

#define GBA_S_MAX_WAIT 80 // The maximum amount of time needed to wait for the data to get thru

extern unsigned short GBA_GameBoysConnected[4];
extern unsigned short GBA_NGameBoysConnected;
extern unsigned short GBA_SerialID;
extern unsigned short GBA_SerialData[4];
extern unsigned char GBA_SerialError;
extern unsigned short GBA_SerialAvailable;
extern int GBA_SerialWaitTime;

void GBA_InitSerial(unsigned short baud);
void GBA_SetMultiplayer();
void GBA_FindGBAs();
void GBA_RepairConnection();
short GBA_WaitSerial(void);
void GBA_UpdateSerial(void);
void GBA_Serial_SendWord(unsigned short word); 
void GBA_ClearSerial();

/*

Normal Mode

                             R                 
F E D C  B A 9 8  7 6 5 4  3 2 1 0
X I M M  X X X X  S X X X  N L C B

0   (B) = Shift Clock (SC) (0 - External, 1 - Internal)
1   (C) = Internal Shift Clock (0 - 256khz, 1 - 2 MHz)
2   (L) = SI State (opponents SO) (0 - Low, 1 - High/None)
3   (N) = SO during inactivity (0 - Low, 1 - High)
7   (S) = Start/Busy bit (0 - inactive/ready, 1 - start/active)
C-D (M) = Comm Mode [8bit=00,32bit=01,Multiplayer=10,UART=11]
E   (I) = Interrupt on completion





Multi-Player Mode

                  * R R R  R R                 
F E D C  B A 9 8  7 6 5 4  3 2 1 0
X I M M  X X X X  S E D D  N L B B

0-1 (B) = Baud rate [00=9600,01=38400,10=57600,11=115200]
2   (L) = SI-Terminal (0 - Parent, 1- Child)
3   (N) = SD-Terminal (0 - Bad connection, 1 - All GBAs Ready)
4-5 (D) = ID of GBA [00=master,01=slave1,10=slave2,11=slave3]
6   (E) = Error (1 on error)
7   (S) = Start/Busy bit (1 triggers the start on the master, signifies busy on slaves)
          * Read only on slaves
C-D (M) = Comm Mode [8bit=00,32bit=01,Multiplayer=10,UART=11]
E   (I) = Interrupt on completion


UART Mode

                    R R R
F E D C  B A 9 8  7 6 5 4  3 2 1 0
X I M M  U T A F  S E R S  P C B B

0-1 (B) = Baud rate [00=9600,01=38400,10=57600,11=115200]
2   (C) = CTS Flag (0 - Send always, 1- Send only when SC = LOW)
3   (P) = Parity Control (0 - Even, 1 - Odd)
4   (S) = Send Data Flag (0 - Not full, 1 - Full)
5   (R) = Receive Data Flag (0 - Not empty, 1 - Empty)
6   (E) = Error (1 on error)
7   (L) = Data Length (0 - 7 bits, 1 - 8 bits)
8   (F) = FIFO Enable Flag
9   (A) = Parity Enable Flag
10  (T) = Send Enable Flag
11  (U) = Receieve Enable Flag
C-D (M) = Comm Mode [8bit=00,32bit=01,Multiplayer=10,UART=11]
E   (I) = Interrupt (0 - Disable, 1 - IRQ when bits 4,5, or 6 become set)



*/


/////////////////////////////////////////////////////////////////////
//        MULTI_BOOT Stuff


// Special thanks to Xilefian on the GBADev Discord
// 
// https://github.com/felixjones/agbabi/tree/multiboot
//
// Basically all the code is based / riped from them.
//

typedef struct MultiBootParam {
    unsigned int reserved1[5];
    unsigned char handshake_data;
    unsigned char padding;
    unsigned short handshake_timeout;
    unsigned char probe_count;
    unsigned char client_data[3];
    unsigned char palette_data;
    unsigned char response_bit;
    unsigned char client_bit;
    unsigned char reserved2;
    const void* boot_srcp;
    const void* boot_endp;
    const void* masterp;
    const void* reserved3[3];
    unsigned int system_work2[4];
    unsigned char sendflag;
    unsigned char probe_target_bit;
    unsigned char check_wait;
    unsigned char server_type;
} MultiBootParam;

void GBA_MB_SetMultiboot();
int GBA_MB_GBAConnected();
int GBA_MB_SendGameROM(unsigned char *ROM,unsigned int ROM_Size);
void GBA_MB_Transfer(unsigned int data);


// Error codes
#define GBAMB_NOT_MASTER 	0x80
#define GBAMB_CANCELED 		0x81
#define GBAMB_FAILED 		0x82

#endif
