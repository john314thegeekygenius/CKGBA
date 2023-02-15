/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/
#ifndef __CK_HEADS__
#define __CK_HEADS__

#ifndef bool
typedef enum {false, true } bool ;
#endif

#define CK4

#ifdef CK4
#include "romstuffs/CK4_Defs.h"
#endif
#ifdef CK5
#include "romstuffs/CK5_Defs.h"
#endif
#ifdef CK6
#include "romstuffs/CK6_Defs.h"
#endif

#include "GBA_Defs.h"
#include "CK_Graphics.h"
#include "CK_Sprites.h"
#include "CK_Audio.h"

#endif
