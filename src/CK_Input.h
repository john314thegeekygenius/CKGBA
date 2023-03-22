/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

/* Reconstructed Commander Keen 4-6 Source Code
 * Copyright (C) 2021 K1n9_Duk3
 *
 * This file is primarily based on:
 * Catacomb 3-D Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
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


#ifndef __CK_INPUT__
#define __CK_INPUT__

////////////////////////////////////////
//// Pulled from ID_IN.H
////
typedef	enum {
    demo_Off,demo_Record,demo_Playback,demo_PlayDone
} Demo;

typedef	enum {
    motion_Left = -1,motion_Up = -1,
    motion_None = 0,
    motion_Right = 1,motion_Down = 1
} Motion;

typedef	enum {
    dir_North,dir_NorthEast,
    dir_East,dir_SouthEast,
    dir_South,dir_SouthWest,
    dir_West,dir_NorthWest,
    dir_None
} Direction;

typedef	struct {
    boolean		button0,button1, button2; // button2 is "space"
    int			x,y;
    Motion		xaxis,yaxis;
    Direction	dir;
} CursorInfo;
typedef	CursorInfo	ControlInfo;

typedef	word ScanCode;

#define MAX_DEMO_BUFFER 0x1FF
extern	Demo		DemoMode;
extern GBA_IN_EWRAM byte  DemoBuffer[MAX_DEMO_BUFFER];
extern word DemoOffset, DemoSize;
extern boolean		Paused;

void IN_ReadControl(int player, ControlInfo *info);

ScanCode IN_WaitForKey(void);
DONT_OPTIMISE void IN_AckBack(void);
DONT_OPTIMISE void IN_Ack(void);
DONT_OPTIMISE boolean IN_UserInput(longword delay,boolean clear);
bool IN_KeyDown(unsigned int key);

void IN_StartDemoPlayback(byte *buffer,word bufsize);
void IN_StopDemo(void);

#endif
