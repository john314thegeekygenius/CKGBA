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


#include "CK_Heads.h"


unsigned short ck_def_button0 = GBA_BUTTON_A, ck_def_button1 = GBA_BUTTON_RSHOLDER, ck_def_button2 = GBA_BUTTON_B;
Demo	DemoMode;
GBA_IN_EWRAM byte  DemoBuffer[MAX_DEMO_BUFFER];
word DemoOffset, DemoSize;
boolean		Paused;

const static Direction	DirTable[] =		// Quick lookup for total direction
					{
						dir_NorthWest,	dir_North,	dir_NorthEast,
						dir_West,		dir_None,	dir_East,
						dir_SouthWest,	dir_South,	dir_SouthEast
					};


///////////////////////////////////////////////////////////////////////////
//
//	IN_ReadControl() - Reads the device associated with the specified
//		player and fills in the control info struct
//
///////////////////////////////////////////////////////////////////////////
void
IN_ReadControl(int player, ControlInfo *info)
{
			boolean		realdelta;
			byte		dbyte;
			word		buttons;
			int			i;
			int			dx,dy;
			Motion		mx,my;

	dx = dy = 0;
	mx = my = motion_None;
	buttons = 0;

	if (DemoMode == demo_Playback)
	{
		dbyte = DemoBuffer[DemoOffset + 1];
		my = (dbyte & 3) - 1;
		mx = ((dbyte >> 2) & 3) - 1;
		buttons = (dbyte >> 4) & 3;

		if (!(--DemoBuffer[DemoOffset]))
		{
			DemoOffset += 2;
			if (DemoOffset >= DemoSize || DemoOffset >= MAX_DEMO_BUFFER)
				DemoMode = demo_PlayDone;
		}

		realdelta = false;
	}
	else if (DemoMode == demo_PlayDone)
		Quit("Demo playback exceeded");
	else
	{
        if (GBA_TEST_BUTTONS(GBA_BUTTON_LEFT|GBA_BUTTON_UP))
            mx = motion_Left,my = motion_Up;
        else if (GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT|GBA_BUTTON_UP))
            mx = motion_Right,my = motion_Up;
        else if (GBA_TEST_BUTTONS(GBA_BUTTON_LEFT|GBA_BUTTON_DOWN))
            mx = motion_Left,my = motion_Down;
        else if (GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT|GBA_BUTTON_DOWN))
            mx = motion_Right,my = motion_Down;

        if (GBA_TEST_BUTTONS(GBA_BUTTON_UP))
            my = motion_Up;
        else if (GBA_TEST_BUTTONS(GBA_BUTTON_DOWN))
            my = motion_Down;

        if (GBA_TEST_BUTTONS(GBA_BUTTON_LEFT))
            mx = motion_Left;
        else if (GBA_TEST_BUTTONS(GBA_BUTTON_RIGHT))
            mx = motion_Right;

        if (GBA_TEST_BUTTONS(ck_def_button0))
            buttons += 1 << 0;
        if (GBA_TEST_BUTTONS(ck_def_button1))
            buttons += 1 << 1;
        if (GBA_TEST_BUTTONS(ck_def_button2))
            buttons += 1 << 2;
        realdelta = false;

	}

	dx = mx * 127;
	dy = my * 127;

	info->x = dx;
	info->xaxis = mx;
	info->y = dy;
	info->yaxis = my;
	info->button0 = buttons & (1 << 0);
	info->button1 = buttons & (1 << 1);
	info->button2 = buttons & (1 << 2);
	info->dir = DirTable[((my + 1) * 3) + (mx + 1)];

    // Removed Demo recording code

}



///////////////////////////////////////////////////////////////////////////
//
//	IN_IsUserInput() - Returns true if a key has been pressed or a button
//		is down
//
///////////////////////////////////////////////////////////////////////////
boolean IN_IsUserInput(void) {
	boolean result = false;
	if (GBA_TEST_BUTTONS(GBA_BUTTON_A))
		result = true;
	if (GBA_TEST_BUTTONS(GBA_BUTTON_B))
		result = true;
	if (GBA_TEST_BUTTONS(GBA_BUTTON_START))
		result = true;
	if (GBA_TEST_BUTTONS(GBA_BUTTON_SELECT))
		result = true;
	if (GBA_TEST_BUTTONS(GBA_BUTTON_LSHOLDER))
		result = true;
	if (GBA_TEST_BUTTONS(GBA_BUTTON_RSHOLDER))
		result = true;
	return result;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_ClearKeyDown() - Clears the keyboard array
//
///////////////////////////////////////////////////////////////////////////
void
IN_ClearKeysDown(void)
{
	int	i;

	LastScan = 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_WaitForKey() - Waits for a scan code, then clears LastScan and
//		returns the scan code
//
///////////////////////////////////////////////////////////////////////////
ScanCode IN_WaitForKey(void)
{
	ScanCode	result;

	while (!(result = LastScan))
		;
	LastScan = 0;
	return(result);
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_AckBack() - Waits for either an ASCII keypress or a button press
//
///////////////////////////////////////////////////////////////////////////
DONT_OPTIMISE void IN_AckBack(void)
{
	word	i;

	while (!LastScan) { GBA_WAIT_VBLANK };
	LastScan = 0x00;
};

///////////////////////////////////////////////////////////////////////////
//
//	IN_Ack() - Clears user input & then calls IN_AckBack()
//
///////////////////////////////////////////////////////////////////////////
DONT_OPTIMISE void IN_Ack(void)
{
	while (LastScan) { GBA_WAIT_VBLANK };
	LastScan = 0x00;
	IN_AckBack();
}


///////////////////////////////////////////////////////////////////////////
//
//	IN_UserInput() - Waits for the specified delay time (in ticks) or the
//		user pressing a key or a mouse button. If the clear flag is set, it
//		then either clears the key or waits for the user to let the mouse
//		button up.
//
///////////////////////////////////////////////////////////////////////////
DONT_OPTIMISE boolean IN_UserInput(longword delay,boolean clear)
{
	longword	lasttime;

	lasttime = TimeCount;
	do
	{
		if (IN_IsUserInput())
		{
			if (clear)
				IN_AckBack();
			return(true);
		}
		// Needed because it gets stuck otherwise????
		GBA_WAIT_VBLANK
	} while (TimeCount - lasttime < delay);
	return(false);
}

bool IN_KeyDown(unsigned int key){
	if(GBA_TEST_BUTTONS(key)) return true;
	return false;
};


///////////////////////////////////////////////////////////////////////////
//
//	IN_StartDemoPlayback() - Plays back the demo pointed to of the given size
//
///////////////////////////////////////////////////////////////////////////
void
IN_StartDemoPlayback(byte *buffer,word bufsize)
{
//	DemoBuffer = buffer;
	DemoMode = demo_Playback;
	DemoSize = bufsize & ~1;
	DemoOffset = 0;
}

///////////////////////////////////////////////////////////////////////////
//
//	IN_StopDemo() - Turns off demo mode
//
///////////////////////////////////////////////////////////////////////////
void
IN_StopDemo(void)
{
	DemoMode = demo_Off;
}
