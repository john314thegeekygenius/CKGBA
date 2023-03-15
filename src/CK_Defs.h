/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_DEF__
#define __CK_DEF__


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

#define TILEGLOBAL			256 // from 256
#define PIXGLOBAL			16
#define HITGLOBAL			16

#define	G_T_SHIFT			8		// global >> ?? = tile 
#define	G_P_SHIFT			4		// global >> ?? = pixels
#define P_T_SHIFT			4		// pixels >> ?? = tile

#define G_EGASX_SHIFT	7	// global >> ?? = screen x
#define G_SY_SHIFT		4	// global >> ?? = screen y

//===========================================================================

typedef enum {spritedraw,maskdraw} drawtype;


#define CK_DISP_SCORE_DOS 0
#define CK_DISP_SCORE_GBA 1


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

#define PLATFORMBLOCK 31
#define DIRARROWSTART 91
#define DIRARROWEND   (DIRARROWSTART+arrow_None)

/*
=============================================================================

						   GLOBAL TYPES

=============================================================================
*/

//SDL-style integer types - just to make future SDL ports easier
typedef unsigned short Uint16; // Fixed from int
typedef signed short Sint16; // Fixed from int
typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned int Uint32; // Fixed from long
typedef signed int Sint32; // Fixed from long
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

#define INTILE_TYPEMASK (INTILE_FOREGROUND-1)

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
	//struct objstruct *next, *prev; // Originally used for saveing / loading
    ///////////////////////////////////////////////////
    // Added for GBA port
    unsigned char isFree;
	unsigned int uuid;
	unsigned char sprAvailable;
	bool removed;
    GBA_SpriteIndex_t gbaSprites[MAX_GBA_SPRITES];
    unsigned int sprsizes[MAX_GBA_SPRITES];
    unsigned int gbaSpriteCount;
    unsigned int gfxoffset;
	Sint32 deltax, deltay;
	uint8_t drawtype;
    CK_SpriteType ck_sprType;
} objtype;

void ClearGameState();

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
	// added
	Sint16 scoreboxdisp; // how to display the scorebox

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

extern const unsigned char CK_DEMO0_data[];
extern const unsigned char CK_DEMO1_data[];
extern const unsigned char CK_DEMO2_data[];
extern const unsigned char CK_DEMO3_data[];
extern const unsigned char CK_DEMO4_data[];



// Intro text
extern const unsigned int CK_INT0_width;
extern const unsigned int CK_INT0_height;
extern const unsigned char CK_INT0[];
extern const unsigned int CK_INT1_width;
extern const unsigned int CK_INT1_height;
extern const unsigned char CK_INT1[];
extern const unsigned int CK_INT2_width;
extern const unsigned int CK_INT2_height;
extern const unsigned char CK_INT2[];
extern const unsigned int CK_INT3_width;
extern const unsigned int CK_INT3_height;
extern const unsigned char CK_INT3[];


extern const unsigned int CK_TERM_0_size;
extern const unsigned int CK_TERM_0_width;
extern const unsigned int CK_TERM_0_height;
extern const unsigned char CK_TERM_0[];

extern const unsigned int CK_TERM_1_size;
extern const unsigned int CK_TERM_1_width;
extern const unsigned int CK_TERM_1_height;
extern const unsigned char CK_TERM_1[];

extern const unsigned int CK_TITLESCREEN_size;
extern const unsigned char CK_TITLESCREEN[];

extern const unsigned char CK_BURNBITS[];
extern const unsigned short CK_BURNBITS_LINEHEIGHT;

void Terminator(void);

/*
=============================================================================

						CK_PLAY DEFINITIONS

=============================================================================
*/

#define CK_HALF_SCREENY 140

extern boolean singlestep, jumpcheat, godmode, keenkilled;
extern exittype playstate;
extern gametype gamestate;
extern ControlInfo c;
extern objtype *ck_newobj, *player, *scoreobj;
extern Uint16 originxtilemax, originytilemax;
extern objtype dummyobj;
extern Sint16 invincible;
extern boolean debugok;

extern boolean jumpbutton, jumpheld, pogobutton, pogoheld, firebutton, fireheld, upheld;

void Quit(char *error);


// From CK_Game.c
extern int	mapon;

void InitObjArray(void);


void CheckKeys(void);
void StatusWindow(void);
void CenterActor(objtype *ob);
void WorldScrollScreen(objtype *ob);
void ScrollScreen(objtype *ob);

void GivePoints(Uint16 points);
void StopMusic(void);
void StartMusic(Uint16 num);

void PlayLoop(void);
/*
=============================================================================

						CK_KEEN DEFINITIONS

=============================================================================
*/

extern const Uint16 bounceangle[8][8];
#ifndef KEEN4
extern const arrowdirtype arrowflip[];
#endif

extern const statetype s_keenstand;
extern const statetype s_keenpauselook;
extern const statetype s_keenwait1;
extern const statetype s_keenwait2;
extern const statetype s_keenwait3;
extern const statetype s_keenwait4;
extern const statetype s_keenwait5;
extern const statetype s_keenwait6;
extern const statetype s_keenmoon1;
extern const statetype s_keenmoon2;
extern const statetype s_keenmoon3;
extern const statetype s_keenread;
extern const statetype s_keenread2;
extern const statetype s_keenread3;
extern const statetype s_keenread4;
extern const statetype s_keenread5;
extern const statetype s_keenread6;
extern const statetype s_keenread7;
extern const statetype s_keenstopread;
extern const statetype s_keenstopread2;
extern const statetype s_keenstopread3;
extern const statetype s_keenlookup;
extern const statetype s_keenlookup2;
extern const statetype s_keenlookdown;
extern const statetype s_keenlookdown2;
extern const statetype s_keenlookdown3;
extern const statetype s_keenlookdown4;
extern const statetype s_keendrop;
extern const statetype s_keendead;
extern const statetype s_keendie1;
extern const statetype s_keendie2;
#ifdef KEEN4
extern const statetype s_keensuitdie1;
extern const statetype s_keensuitdie2;
#endif
extern const statetype s_keenshoot1;
extern const statetype s_keenshoot2;
extern const statetype s_keenshootup1;
extern const statetype s_keenshootup2;
extern const statetype s_keenswitch;
extern const statetype s_keenswitch2;
extern const statetype s_keenkey;
extern const statetype s_keenlineup;
extern const statetype s_keenenter1;
extern const statetype s_keenenter2;
extern const statetype s_keenenter3;
extern const statetype s_keenenter4;
extern const statetype s_keenenter5;
extern const statetype s_keenpole;
extern const statetype s_keenclimb1;
extern const statetype s_keenclimb2;
extern const statetype s_keenclimb3;
extern const statetype s_keenslide1;
extern const statetype s_keenslide2;
extern const statetype s_keenslide3;
extern const statetype s_keenslide4;
extern const statetype s_keenpoleshoot1;
extern const statetype s_keenpoleshoot2;
extern const statetype s_keenpoleshootup1;
extern const statetype s_keenpoleshootup2;
extern const statetype s_keenpoleshootdown1;
extern const statetype s_keenpoleshootdown2;
extern const statetype s_keenwalk1;
extern const statetype s_keenwalk2;
extern const statetype s_keenwalk3;
extern const statetype s_keenwalk4;
extern const statetype s_keenpogodown;
extern const statetype s_keenpogo;
extern const statetype s_keenpogo2;
extern const statetype s_keenjump1;
extern const statetype s_keenjump2;
extern const statetype s_keenjump3;
extern const statetype s_keenjump4;
extern const statetype s_keenairshoot1;
extern const statetype s_keenairshoot2;
extern const statetype s_keenairshoot3;
extern const statetype s_keenairshootup1;
extern const statetype s_keenairshootup2;
extern const statetype s_keenairshootup3;
extern const statetype s_keenairshootdown1;
extern const statetype s_keenairshootdown2;
extern const statetype s_keenairshootdown3;
extern const statetype s_keenholdon;
extern const statetype s_keenholdon2;
extern const statetype s_keenclimbup;
extern const statetype s_keenclimbup2;
extern const statetype s_keenclimbup3;
extern const statetype s_keenclimbup4;
extern const statetype s_keenclimbup5;

extern const Sint16 slopespeed[8];
extern const Sint16 polexspeed[3];

extern const Sint16 shotsinclip[4];
extern const Sint16 bonussound[];
extern const Sint16 bonuspoints[];
extern const Sint16 bonussprite[];

extern Uint16 zeromap;

extern Sint16 singlegravity;
extern Sint16 jumptime;
extern Sint32 leavepoletime;
extern Sint16 moonok;

void SpawnKeen(Sint16 x, Sint16 y, Sint16 dir);
boolean CheckGrabPole(objtype *ob);
boolean CheckEnterHouse(objtype *ob);
void WalkSound1(objtype *ob);
void WalkSound2(objtype *ob);
void KeenStandThink(objtype *ob);
void KeenPauseThink(objtype *ob);
void KeenReadThink(objtype *ob);
void KeenLookUpThink(objtype *ob);
void KeenLookDownThink(objtype *ob);
void KeenWalkThink(objtype *ob);
void T_LineUp(objtype *ob);
void KeenEnterThink(objtype *ob);
void KeenSwitchThink(objtype *ob);
void KeenKeyThink(objtype *ob);
void KeenAirThink(objtype *ob);
void KeenBounceThink(objtype *ob);
void KeenPogoThink(objtype *ob);
void PoleActions(objtype *ob);
void KeenPoleThink(objtype *ob);
void KeenClimbThink(objtype *ob);
void KeenDropThink(objtype *ob);
void KeenDropDownThink(objtype *ob);
void KeenHoldThink(objtype *ob);
void KeenShootThink(objtype *ob);
void T_PullUp1(objtype *ob);
void T_PullUp2(objtype *ob);
void T_PullUp3(objtype *ob);
void T_PulledUp(objtype *ob);
void KeenDieThink(objtype *ob);
void KillKeen(void);
void KeenContact(objtype *ob, objtype *hit);
void KeenPosContact(objtype *ob, objtype *hit);
void HandleRiding(objtype *ob);
void TileBonus(Uint16 x, Uint16 y, Uint16 bonus);
void GiveDrop(Uint16 x, Uint16 y);
void CheckInTiles(objtype *ob);
void KeenSimpleReact(objtype *ob);
void KeenStandReact(objtype *ob);
void KeenWalkReact(objtype *ob);
void KeenAirReact(objtype *ob);
void KeenPogoReact(objtype *ob);
void KeenPoleReact(objtype *ob);



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

						CK_TEXT DEFINITIONS

=============================================================================
*/


void HelpScreens(void);
void FinaleLayout(void);

/*
=============================================================================

						CK_STATE DEFINITIONS

=============================================================================
*/


#define NORTHWALL (CK_TileInfo_FGTiles*0)
#define EASTWALL (CK_TileInfo_FGTiles*1)
#define SOUTHWALL (CK_TileInfo_FGTiles*2)
#define WESTWALL (CK_TileInfo_FGTiles*3)
#define MANIM	(CK_TileInfo_FGTiles*4)
#define INTILE	(CK_TileInfo_FGTiles*5)
#define MSPEED	(CK_TileInfo_FGTiles*6)

extern const Sint16 wallclip[8][16];

extern Sint16 xtry;
extern Sint16 ytry;
extern boolean playerkludgeclipcancel;

void MoveObjVert(objtype *ob, Sint16 ymove);
void MoveObjHoriz(objtype *ob, Sint16 xmove);
void PlayerBottomKludge(objtype *ob);
void PlayerTopKludge(objtype *ob);
void ClipToEnds(objtype *ob);
void ClipToSides(objtype *ob);
boolean CheckPosition(objtype *ob);
boolean StatePositionOk(objtype *ob, statetype *state);

#ifdef KEEN5
void CalcBounds(objtype *ob);
#endif

void ClipToWalls(objtype *ob);
void FullClipToWalls(objtype *ob);
void PushObj(objtype *ob);
void ClipToSpriteSide(objtype *push, objtype *solid);
void ClipToSpriteTop(objtype *push, objtype *solid);
void ClipToSprite(objtype *push, objtype *solid, boolean squish);
Sint16 DoActor(objtype *ob, Sint16 numtics);
void StateMachine(objtype *ob);
void NewState(objtype *ob, statetype *state);
void ChangeState(objtype *ob, statetype *state);
boolean OnScreen(objtype *ob);
void DoGravity(objtype *ob);
void DoWeakGravity(objtype *ob);
void DoTinyGravity(objtype *ob);
void AccelerateX(objtype *ob, Sint16 dir, Sint16 maxspeed);
void AccelerateXv(objtype *ob, Sint16 dir, Sint16 maxspeed);
void AccelerateY(objtype *ob, Sint16 dir, Sint16 maxspeed);
void FrictionX(objtype *ob);
void FrictionY(objtype *ob);
void StunObj(objtype *ob, objtype *shot, statetype *stunstate);
void T_Projectile(objtype *ob);
void T_WeakProjectile(objtype *ob);
void ProjectileThink1(objtype *ob);
void T_Velocity(objtype *ob);
void SetReactThink(objtype *ob);
void T_Stunned(objtype *ob);
void C_Lethal(objtype *ob, objtype *hit);
void R_Draw(objtype *ob);
void R_Walk(objtype *ob);
void R_WalkNormal(objtype *ob);
void BadState(void);
void R_Stunned(objtype *ob);

extern statetype sc_deadstate;
extern statetype sc_badstate;


#endif
