/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

/* 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __CK_HEADS__
#define __CK_HEADS__


//#define CK_DISABLE_SOUND
//#define CK_DISABLE_MUSIC
//#define CK_INTR_ENDER

// For the EZ Flash cart
//#define __EZ_FLASH

#define CK4
#define KEEN

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
#include "GBA_File.h"
#include "CK_Input.h"

// Moved from Graphics.c
#define RGBCONV(x) ((((x)>>16)>>3) | (((((x)>>8)&0xFF)>>3)<<5) | ((((x)&0xFF)>>3)<<10))



#ifdef CK4
#define CK_DOS_PROMPT "C:\\KEEN4>"
#endif
#ifdef CK5
#define CK_DOS_PROMPT "C:\\KEEN5>"
#endif
#ifdef CK6
#define CK_DOS_PROMPT "C:\\KEEN6>"
#endif


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
