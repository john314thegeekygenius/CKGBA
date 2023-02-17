/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#ifndef __CK_DEF__
#define __CK_DEF__

#define CK_DISABLE_MUSIC
#define CK4


// Define what a boolean is
#ifndef boolean
typedef enum {false, true } boolean;
#endif
#ifndef bool
typedef boolean bool; // For sanity
#endif

#ifdef CK4
#include "romstuffs/CK4_Defs.h"
#endif
#ifdef CK5
#include "romstuffs/CK5_Defs.h"
#endif
#ifdef CK6
#include "romstuffs/CK6_Defs.h"
#endif

#endif
