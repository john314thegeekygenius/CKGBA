/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
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

extern	Demo		DemoMode;

void IN_ReadControl(int player, ControlInfo *info);


#endif
