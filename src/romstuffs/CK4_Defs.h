
#ifndef __CK4_DEFS__
#define __CK4_DEFS__



// K4_DEF.H stuff

// TODO:
// Make this correct
#define STARPALETTE {}

#define TERMPALETTE { 0x0, 0x1, 0x2, 0x9, 0x11, 0x5, 0x6, 0x8, 0x7, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf}

#define WORLDMAPNAME "Shadowlands"
#define DROPSNAME "DROPS"

// TODO:
// Make this correct
#define HIGHSCORE_LEFT	24
#define HIGHSCORE_TOP	51
#define HIGHSCORE_RIGHT	296
#define HIGHSCORE_MAP	19

#define STARWARSMUSIC	12
#define ENDINGMUSIC	7

// levels in this range can NOT be re-entered (BWB level should be > MAXDONELEVEL)
#define MINDONELEVEL 1
#define MAXDONELEVEL 17

#define INACTIVATEDIST 4

// Graphics Stuff

#include "CK4_GFX.h"

typedef enum CK_SpriteType{
    CKS_PADDLE,
    CKS_BALL,
    CKS_DEMO,
    CKS_KEEN,
    CKS_STARS,
    CKS_KSHOT,
    CKS_DROPLET,
    CKS_1UP,
    CKS_SHADOW,
    CKS_SODA,
    CKS_TTGUM,
    CKS_CANDYBAR,
    CKS_JBREAKER,
    CKS_DOUGHNUT,
    CKS_ICECREAM,
    CKS_LIFEWATER,
    CKS_DOOR,
    CKS_RKEY,
    CKS_YKEY,
    CKS_BKEY,
    CKS_GKEY,
    CKS_KEYSHADOW,
    CKS_NSTUNNER,
    CKS_SCOREBOX,
    CKS_MAPKEEN,
    CKS_MAPFOOT,
    CKS_MAPFLAGFLIP,
    CKS_MAPFLAGIDLE,
    CKS_KEENSWIM,
    CKS_POISONSLUG,
    CKS_POISONPOO,
    CKS_MADMUSH,
    CKS_LINDSEY,
    CKS_INCHWORM,
    CKS_FOOT,
    CKS_TEATER,
    CKS_PVAPER,
    CKS_TESMOKE,
    CKS_EGG,
    CKS_EGGPARTS,
    CKS_BIRDWALK,
    CKS_BIRDFLY,
    CKS_DART,
    CKS_MIMROCK,
    CKS_DOPEFISH,
    CKS_LITTLEBUBBLE,
    CKS_SMALLBUBBLE,
    CKS_MEDIUMBUBBLE,
    CKS_FISHY,
    CKS_ARACHNUT,
    CKS_SWIMSUIT,
    CKS_SPRITE,
    CKS_EBLAST,
    CKS_MINE,
    CKS_SKYPEST,
    CKS_WORMMOUTH,
    CKS_LICK,
    CKS_ELEVATOR,
    CKS_EFIRE,
    CKS_BOUNDER,
    CKS_TCLOUD,
    CKS_LBOLT,
    CKS_BERKELOID,
    CKS_FLAME,
}CK_SpriteType;

#endif //__CK4_DEFS__
