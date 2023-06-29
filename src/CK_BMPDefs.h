/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Jun 2023
*/

#ifndef __CK_BMPDEFS__H__
#define __CK_BMPDEFS__H__

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

#endif // __CK_BMPDEFS__H__