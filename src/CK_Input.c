/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"


unsigned short ck_def_button0 = GBA_BUTTON_A, ck_def_button1 = GBA_BUTTON_RSHOLDER, ck_def_button2 = GBA_BUTTON_B;
Demo	DemoMode;

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
	{/*
		dbyte = DemoBuffer[DemoOffset + 1];
		my = (dbyte & 3) - 1;
		mx = ((dbyte >> 2) & 3) - 1;
		buttons = (dbyte >> 4) & 3;

		if (!(--DemoBuffer[DemoOffset]))
		{
			DemoOffset += 2;
			if (DemoOffset >= DemoSize)
				DemoMode = demo_PlayDone;
		}
*/
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

	if (realdelta)
	{
		mx = (dx < 0)? motion_Left : ((dx > 0)? motion_Right : motion_None);
		my = (dy < 0)? motion_Up : ((dy > 0)? motion_Down : motion_None);
	}
	else
	{
		dx = mx * 127;
		dy = my * 127;
	}

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
