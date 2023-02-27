
#ifndef __CK4_DEFS__
#define __CK4_DEFS__



// K4_DEF.H stuff

// ???
#define GAMELEVELS 25

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

/*
=============================================================================

						K4_SPEC DEFINITIONS

=============================================================================
*/

extern const char *levelnames[GAMELEVELS];
extern const char *levelenter[GAMELEVELS];

void ScanInfoPlane(void);
void PrincessLindsey(void);
void RescueJanitor(void);
void CantSwim(void);
void GotScuba(void);
void RescuedMember(void);

extern statetype s_keenswimslow1;
extern statetype s_keenswimslow2;
extern statetype s_keenswim1;
extern statetype s_keenswim2;
/*extern statetype s_kbubble1;
extern statetype s_kbubble2;
extern statetype s_kbubble3;
extern statetype s_kbubble4;*/
void SpawnSwimKeen(Sint16 x, Sint16 y);
void SpawnKbubble(objtype *ob);
void T_KeenSwimSlow(objtype *ob);
void T_KeenSwim(objtype *ob);
void C_KeenSwim(objtype *ob, objtype *hit);
void R_KeenSwim(objtype *ob);



#endif //__CK4_DEFS__
