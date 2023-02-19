/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/
#ifndef __CK_HEADS__
#define __CK_HEADS__

//
//	ID Engine
//	Types.h - Generic types, #defines, etc.
//	v1.0d1
//

#ifndef	__TYPES__
#define	__TYPES__

typedef	enum	{false,true}	boolean;
typedef	unsigned	char		byte;
typedef	unsigned	int			word;
typedef	unsigned	long		longword;
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

#include "CK_Defs.h"

#include "CK_Graphics.h"
#include "CK_Sprites.h"
#include "CK_Audio.h"
#include "CK_Levels.h"
#include "CK_Universal.h"


#endif
