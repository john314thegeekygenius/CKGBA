/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_DEF__
#define __CK_DEF__

// #define CK_DISABLE_MUSIC
#define CK4


#ifdef CK4
#define KEEN4
#include "romstuffs/CK4_Defs.h"
#endif
#ifdef CK5
#define KEEN5
#include "romstuffs/CK5_Defs.h"
#endif
#ifdef CK6
#define KEEN6
#include "romstuffs/CK6_Defs.h"
#endif



// Pulled from ID_RF.H
/*
=============================================================================

							CONSTANTS

=============================================================================
*/

#define	MINTICS				2
#define	MAXTICS				5
#define DEMOTICS			3

#define	MAPBORDER			2		// map border must be at least 1

#ifdef KEEN5

#define	MAXSPRITES			60		// max tracked sprites
#define	MAXANIMTILES		90		// max animating tiles on screen
#define MAXANIMTYPES		80		// max different unique anim tiles on map

#define	MAXMAPHEIGHT		250

#else

#define	MAXSPRITES			60		// max tracked sprites
#define	MAXANIMTILES		90		// max animating tiles on screen
#define MAXANIMTYPES		65		// max different unique anim tiles on map

#define	MAXMAPHEIGHT		200

#endif

#define	PRIORITIES			4
#define	MASKEDTILEPRIORITY	3		// planes go: 0,1,2,MTILES,3

#define TILEGLOBAL			16 // from 256
#define PIXGLOBAL			16

#define	G_T_SHIFT			4		// global >> ?? = tile // from 8
#define	G_P_SHIFT			4		// global >> ?? = pixels
#define P_T_SHIFT			4		// pixels >> ?? = tile


//===========================================================================

typedef enum {spritedraw,maskdraw} drawtype;


// Pulled from CK_DEF.H


/*
=============================================================================

						 GLOBAL CONSTANTS & MACROS

=============================================================================
*/

#define MAXACTORS 100

#define GAMELEVELS 25



#define CONVERT_GLOBAL_TO_TILE(x)  ((x)>>(G_T_SHIFT))
#define CONVERT_TILE_TO_GLOBAL(x)  ((x)<<(G_T_SHIFT))
#define CONVERT_GLOBAL_TO_PIXEL(x) ((x)>>(G_P_SHIFT))
#define CONVERT_PIXEL_TO_GLOBAL(x) ((x)<<(G_P_SHIFT))
#define CONVERT_PIXEL_TO_TILE(x)   ((x)>>(P_T_SHIFT))
#define CONVERT_TILE_TO_PIXEL(x)   ((x)<<(P_T_SHIFT))


#define MAX_GBA_SPRITES 16 // Theoretical max sprite size is thus 256x256 or 16 64x64 sprites

/*
=============================================================================

						   GLOBAL TYPES

=============================================================================
*/

//SDL-style integer types - just to make future SDL ports easier
typedef unsigned int Uint16;
typedef signed int Sint16;
typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned long Uint32;
typedef signed long Sint32;
//Note: only the game code (CK_*.C, K?_*.C) uses these!

typedef enum {
	arrow_North,     // 0
	arrow_East,      // 1
	arrow_South,     // 2
	arrow_West,      // 3
	arrow_NorthEast, // 4
	arrow_SouthEast, // 5
	arrow_SouthWest, // 6
	arrow_NorthWest, // 7
	arrow_None       // 8
} arrowdirtype;

typedef enum {
	ex_stillplaying, //  0
	ex_died,         //  1
	ex_completed,    //  2
	ex_rescued,      //  3, only in Keen 4
	ex_warped,       //  4
	ex_resetgame,    //  5
	ex_loadedgame,   //  6
	ex_foot,         //  7, only in Keen 4
	ex_abortgame,    //  8
	ex_sandwich,     //  9, only in Keen 6
	ex_hook,         // 10, only in Keen 6
	ex_card,         // 11, only in Keen 6
	ex_molly,        // 12, only in Keen 6
	ex_portout,      // 13, only in Keen 5
	ex_fusebroke,    // 14, only in Keen 5
	ex_qedbroke,     // 15, only in Keen 5
	NUMEXITTYPES
} exittype;

typedef enum
{
	INTILE_NOTHING,        //  0
	INTILE_POLE,           //  1
	INTILE_DOOR,           //  2
	INTILE_DEADLY,         //  3
	INTILE_DROP,           //  4
	INTILE_SWITCH0,        //  5
	INTILE_SWITCH1,        //  6
	INTILE_GEMSOCKET0,     //  7
	INTILE_GEMSOCKET1,     //  8
	INTILE_GEMSOCKET2,     //  9
	INTILE_GEMSOCKET3,     // 10
	INTILE_SHORESOUTH,     // 11
	INTILE_SHOREWEST,      // 12
	INTILE_SHORENORTH,     // 13
	INTILE_SHOREEAST,      // 14
	INTILE_BRIDGESWITCH,   // 15
	INTILE_MOON,           // 16
	INTILE_DIRARROW,       // 17 (not used in the code, but assigned to tiles in Keen 5 & 6)
	INTILE_BRIDGE,         // 18
	INTILE_FORCEFIELD,     // 19
	INTILE_TELEPORT,       // 20
	INTILE_BONUS100,       // 21
	INTILE_BONUS200,       // 22
	INTILE_BONUS500,       // 23
	INTILE_BONUS1000,      // 24
	INTILE_BONUS2000,      // 25
	INTILE_BONUS5000,      // 26
	INTILE_EXTRALIFE,      // 27
	INTILE_AMMO,           // 28
	INTILE_29,             // 29 (never used)
	INTILE_FORCEFIELDEND,  // 30
	INTILE_AMPTONCOMPUTER, // 31
	INTILE_KEYCARDDOOR,    // 32
	INTILE_ELEVATORLEFT,   // 33
	INTILE_ELEVATORRIGHT,  // 34

	INTILE_FOREGROUND = 0x80
} intiletype;

typedef enum
{
	nothing,         //  0
	inertobj,        //  1
	keenobj,         //  2
	stunshotobj,     //  3
#if defined CK4
	bonusobj,        //  4
	slugobj,         //  5
	oracleobj,       //  6
	classtype_7,     //  7, never used
	eggobj,          //  8
	madmushroomobj,  //  9
	arachnutobj,     // 10
	skypestobj,      // 11
	wormouthobj,     // 12
	thundercloudobj, // 13
	berkeloidobj,    // 14
	bounderobj,      // 15
	inchwormobj,     // 16
	footobj,         // 17
	lickobj,         // 18
	mimrockobj,      // 19
	platformobj,     // 20
	dopefishobj,     // 21
	schoolfishobj,   // 22
	pixieobj,        // 23
	lindseyobj,      // 24
	lightningobj,    // 25
	treasureeaterobj,// 26
	eggbirdobj,      // 27
	classtype_28,    // 28, never used
	classtype_29,    // 29, never used
	scubaobj,        // 30
	mshotobj,        // 31
	mineobj,         // 32
	stunnedobj,      // 33
	flagobj,         // 34
#elif defined CK5
	mshotobj,        //  4
	bonusobj,        //  5
	platformobj,     //  6
	stunnedobj,      //  7
	flagobj,         //  8
	sparkyobj,       //  9
	mineobj,         // 10
	slicestarobj,    // 11
	roboredobj,      // 12
	spirogripobj,    // 13
	amptonobj,       // 14
	cannonobj,       // 15
	volteobj,        // 16
	shelleyobj,      // 17, never used
	spindredobj,     // 18
	shikadimasterobj,// 19
	shikadiobj,      // 20
	petobj,          // 21
	spherefulobj,    // 22
	scottieobj,      // 23
	teleporterobj,   // 24
	qedobj,          // 25
#elif defined CK6
	mshotobj,        //  4
	bonusobj,        //  5
	platformobj,     //  6
	bloogobj,        //  7
	bloogletobj,     //  8
	classtype_9,     //  9, never used
	fleexobj,        // 10
	classtype_11,    // 11, never used
	mollyobj,        // 12
	babobbaobj,      // 13
	bobbaobj,        // 14
	classtype_15,    // 15
	nospikeobj,      // 16
	gikobj,          // 17
	cannonobj,       // 18
	orbatrixobj,     // 19
	bipobj,          // 20
	flectobj,        // 21
	blorbobj,        // 22
	ceilickobj,      // 23
	blooguardobj,    // 24
	stunnedobj,      // 25
	bipshipobj,      // 26
	sandwichobj,     // 27
	hookobj,         // 28
	passcardobj,     // 29
	grabbiterobj,    // 30
	rocketobj,       // 31
	grapplespotobj,  // 32
	satelliteobj,    // 33
	satellitestopobj,// 34
	flagobj,         // 35
#endif
	NUMCLASSTYPES
} classtype;

typedef struct statestruct
{
	Sint16 leftshapenum, rightshapenum;
	enum {step,slide,think,stepthink,slidethink} progress;
	boolean skippable;
	boolean pushtofloor;
	Sint16 tictime;
	Sint16 xmove;
	Sint16 ymove;
	void (*think) (struct objstruct*);
	void (*contact) (struct objstruct*, struct objstruct*);
	void (*react) (struct objstruct*);
	struct statestruct *nextstate;
} statetype;


typedef struct objstruct
{
	classtype obclass;
	enum {ac_no, ac_yes, ac_allways, ac_removable} active;
	boolean needtoreact;
	enum {cl_noclip, cl_midclip, cl_fullclip} needtoclip;
	Uint16 nothink;
	Uint16 x, y;
	Sint16 xdir, ydir;
	Sint16 xmove, ymove;
	Sint16 xspeed, yspeed;
	Sint16 ticcount;
	statetype *state;
	Uint16 shapenum;
	Uint16 priority;
	Uint16 left, top, right, bottom, midx;
	Uint16 tileleft, tiletop, tileright, tilebottom, tilemidx;
	Sint16 hitnorth, hiteast, hitsouth, hitwest;
	Sint16 temp1, temp2, temp3, temp4;
	void *sprite;
	struct objstruct *next, *prev;
    ///////////////////////////////////////////////////
    // Added for GBA port
    unsigned char isFree;
    GBA_SpriteIndex_t gbaSprites[MAX_GBA_SPRITES];
    unsigned int sprtype[MAX_GBA_SPRITES];
    unsigned int gbaSpriteCount;
    unsigned int gfxoffset;
    CK_SpriteType ck_sprType;
} objtype;

typedef struct
{
	Uint16 worldx, worldy;
	boolean leveldone[GAMELEVELS];
	Sint32 score, nextextra;
	Sint16 ammo, drops;
#if defined CK4
	Sint16 wetsuit;
	Sint16 rescued;
#elif defined CK5
	boolean keycard;
	Sint16 destroyed;	// never used
	Sint16 numfuses;
#elif defined CK6
	Sint16 sandwichstate, hookstate, passcardstate, rocketstate;
#endif
	Sint16 keys[4];
	Sint16 mapon;
	Sint16 lives;
	Sint16 difficulty;
	objtype *riding;
} gametype;

// Found in CK_Sprites.c
extern statetype sc_deadstate;
extern statetype sc_badstate;

/*
=============================================================================

						CK_DEMO DEFINITIONS

=============================================================================
*/

extern boolean scorescreenkludge;

void Terminator(void);

/*
=============================================================================

						CK_PLAY DEFINITIONS

=============================================================================
*/


extern boolean singlestep, jumpcheat, godmode, keenkilled;
extern exittype playstate;
extern gametype gamestate;
extern ControlInfo c;
extern objtype *ck_newobj, *check, *player, *scoreobj;
extern Uint16 originxtilemax, originytilemax;
extern objtype dummyobj;
extern Sint16 invincible;
extern boolean debugok;

extern boolean jumpbutton, jumpheld, pogobutton, pogoheld, firebutton, fireheld, upheld;

// From CK_Game.c
extern int	mapon;

void InitObjArray(void);

void StopMusic(void);
void StartMusic(Uint16 num);
void PlayLoop(void);


/*
=============================================================================

						CK_KEEN2 DEFINITIONS

=============================================================================
*/

extern const statetype s_score;
extern const statetype s_demo;
void SpawnScore(void);
void UpdateScore(objtype *ob);
void DrawDemoPlaque(objtype *ob);

extern const statetype s_worldkeen;
extern const statetype s_worldkeenwave1;
extern const statetype s_worldkeenwave2;
extern const statetype s_worldkeenwave3;
extern const statetype s_worldkeenwave4;
extern const statetype s_worldkeenwave5;
extern const statetype s_worldkeenwalk;
void SpawnWorldKeen(Sint16 x, Sint16 y);
#ifdef KEEN5
void SpawnWorldKeenPort(Uint16 tileX, Uint16 tileY);
#endif
void CheckEnterLevel(objtype *ob);
void T_KeenWorld(objtype *ob);
void T_KeenWorldWalk(objtype *ob);
void CheckWorldInTiles(objtype *ob);

#ifdef KEEN4
extern const statetype s_keenonfoot1;
extern const statetype s_keenonfoot2;
extern const statetype s_worldswim;
void T_FootFly(objtype *ob);
void T_KeenWorldSwim(objtype *ob);
#endif

#ifdef KEEN5
extern const statetype s_worldelevate;
void T_Elevate(objtype *ob);
#endif

extern const statetype s_flagwave1;
extern const statetype s_flagwave2;
extern const statetype s_flagwave3;
extern const statetype s_flagwave4;
void SpawnFlag(Sint16 x, Sint16 y);

#ifndef KEEN5
extern const statetype s_throwflag0;
extern const statetype s_throwflag1;
extern const statetype s_throwflag2;
extern const statetype s_throwflag3;
extern const statetype s_throwflag4;
extern const statetype s_throwflag5;
extern const statetype s_throwflag6;
void SpawnThrowFlag(Sint16 x, Sint16 y);
void TossThink(objtype *ob);
void PathThink(objtype *ob);
void FlagAlign(objtype *ob);
#endif

extern const statetype s_stunray1;
extern const statetype s_stunray2;
extern const statetype s_stunray3;
extern const statetype s_stunray4;
extern const statetype s_stunhit;
extern const statetype s_stunhit2;
void SpawnShot(Uint16 x, Uint16 y, Direction dir);
void ExplodeShot(objtype *ob);
void T_Shot(objtype *ob);
void R_Shot(objtype *ob);

extern const statetype s_door1;
extern const statetype s_door2;
extern const statetype s_door3;
void DoorOpen(objtype *ob);

#ifdef KEEN5
extern const statetype s_carddoor;
void CardDoorOpen(objtype *ob);
#endif


/*
=============================================================================

						CK_STATE DEFINITIONS

=============================================================================
*/
extern const Sint16 wallclip[8][16];

extern Sint16 xtry;
extern Sint16 ytry;
extern boolean playerkludgeclipcancel;

Sint16 DoActor(objtype *ob, Sint16 numtics);
void StateMachine(objtype *ob);
void NewState(objtype *ob, statetype *state);
void ChangeState(objtype *ob, statetype *state);


void R_Draw(objtype *ob);



#endif
