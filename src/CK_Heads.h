/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/
#ifndef __CK_HEADS__
#define __CK_HEADS__


// #define CK_DISABLE_MUSIC
#define CK4


//
//	ID Engine
//	Types.h - Generic types, #defines, etc.
//	v1.0d1
//

#ifndef	__TYPES__
#define	__TYPES__

typedef	enum	{false,true}	boolean;
typedef	unsigned	char		byte;
typedef	unsigned	short		word; // Fixed from int
typedef	unsigned	int		longword; // Fixed from long
typedef	byte *					Ptr;

typedef	struct
		{
			int	x,y;
		} Point;
typedef	struct
		{
			Point	ul,lr;
		} Rect;

#define	nil	((void *)0)

#endif
#ifndef bool
typedef boolean bool; // For sanity
#endif


#include "GBA_Defs.h"

#include "CK_Input.h"

#ifdef CK4
#define KEEN4
#include "romstuffs/CK4_Spectors.h"
#endif
#ifdef CK5
#define KEEN5
#include "romstuffs/CK5_Spectors.h"
#endif
#ifdef CK6
#define KEEN6
#include "romstuffs/CK6_Spectors.h"
#endif


#include "CK_Defs.h"


#ifdef CK4
#include "romstuffs/CK4_Defs.h"
#endif
#ifdef CK5
#include "romstuffs/CK5_Defs.h"
#endif
#ifdef CK6
#include "romstuffs/CK6_Defs.h"
#endif



#include "CK_Graphics.h"
#include "CK_Sprites.h"
#include "CK_Audio.h"
#include "CK_Levels.h"
#include "CK_Universal.h"


#endif
