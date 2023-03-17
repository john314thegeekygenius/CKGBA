
/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
*/

#include "CK_Heads.h"

// From ID_US_2.C


///////////////////////////////////////////////////////////////////////////
//
//      USL_SetUpCtlPanel() - Sets the states of the UserItems to reflect the
//              values of all the appropriate variables
//
///////////////////////////////////////////////////////////////////////////
static void
USL_SetUpCtlPanel(void)
{/*
	int     i;

	// Cache in all of the stuff for the control panel
	CA_UpLevel();
	for (i = CONTROLS_LUMP_START;i <= CONTROLS_LUMP_END;i++)
		CA_MarkGrChunk(i);
	for (i = PADDLE_LUMP_START;i <= PADDLE_LUMP_END;i++)
		CA_MarkGrChunk(i);
	CA_MarkGrChunk(STARTFONT+1);            // Little font
	CA_MarkGrChunk(CP_MENUMASKPICM);        // Mask for dialogs
	CA_CacheMarks("Control Panel");
	CA_LoadAllSounds();

	// Do some other setup
	fontnumber = 1;
	US_SetPrintRoutines(VW_MeasurePropString,VWB_DrawPropString);
	fontcolor = F_BLACK;
#ifdef CAT3D
	VW_Bar (0,0,320,200,3); // CAT3D patch
#else
	VW_ClearVideo(3);
#endif
	RF_FixOfs();
	VW_InitDoubleBuffer();

	Communication = uc_None;
	USL_ClearFlags(&rootgroup);
	USL_SetControlValues();
	USL_SetupStack();
	USL_SetupCard();

	if (ingame)
		GameIsDirty = true;
*/
	IN_ClearKeysDown();
}


///////////////////////////////////////////////////////////////////////////
//
//      USL_TearDownCtlPanel() - Given the state of the control panel, sets the
//              modes and values as appropriate
//
///////////////////////////////////////////////////////////////////////////
static void
USL_TearDownCtlPanel(void)
{/*
	USL_GetControlValues();
	if (Communication)
		USL_HandleComm(Communication);

	fontnumber = 0; // Normal font
	fontcolor = F_BLACK;
	if (restartgame && USL_ResetGame)
		USL_ResetGame();
	else if (QuitToDos)
	{
		if (tedlevel)
			TEDDeath();
		else
		{
			US_CenterWindow(20,3);
			fontcolor = F_SECONDCOLOR;
			US_PrintCentered("Quitting...");
			fontcolor = F_BLACK;
			VW_UpdateScreen();
			Quit(nil);
		}
	}

	IN_ClearKeysDown();
	SD_WaitSoundDone();
#ifdef CAT3D
	VW_Bar (0,0,320,200,3); // CAT3D patch
#else
	VW_ClearVideo(3);
#endif
	CA_DownLevel();
	CA_LoadAllSounds();*/
}

///////////////////////////////////////////////////////////////////////////
//
//      US_ControlPanel() - This is the main routine for the control panel
//
///////////////////////////////////////////////////////////////////////////
#define MoveMin 40
void
US_ControlPanel(void)
{
/*
	extern void HelpScreens(void);

	boolean         resetitem,on;
	word            i;
	int                     ydelta;
	longword        flashtime;
	UserItem        far *item;
	CursorInfo      cursorinfo;

#if 0
	// DEBUG!!!
	{
		USL_SetUpCtlPanel();
		Communication = uc_Loaded;
		CtlPanelDone = true;
		loadedgame = true;
		USL_TearDownCtlPanel();
		return;
	}
#endif

	if ((LastScan < sc_F1) || (LastScan > sc_F10))
		IN_ClearKeysDown();

	USL_SetUpCtlPanel();
	USL_DrawCtlPanel();

	ydelta = 0;
	for (CtlPanelDone = false,resetitem = on = true;!CtlPanelDone;)
	{
		item = &(topcard->items[topcard->cursor]);

		if (resetitem)
		{
			flashtime = TimeCount + (TickBase / 2);
			resetitem = false;
		}

		if (TimeCount >= flashtime)
		{
			on ^= true;
			resetitem = true;
			if (!on)
				item->flags &= ~ui_Selected;
			USL_DrawItemIcon(item);
			item->flags |= ui_Selected;
		}

		VW_UpdateScreen();

		if (LastScan)
		{
			switch (LastScan)
			{
			case sc_UpArrow:
				USL_PrevItem();
				resetitem = true;
				break;
			case sc_DownArrow:
				USL_NextItem();
				resetitem = true;
				break;
			case sc_Return:
				USL_DoItem();
				resetitem = true;
				break;
			case sc_Escape:
				USL_UpLevel();
				resetitem = true;
				break;
#ifndef KEEN6
			case sc_F1:
				HelpScreens();
				USL_DrawCtlPanel();
				resetitem = true;
				break;
#endif
			}

			if
			(
				(!resetitem)
			&&      (
					(LastScan == KbdDefs[0].button0)
				||      (LastScan == KbdDefs[0].button1)
				)
			)
			{
				USL_DoItem();
				resetitem = true;
			}

			if (!resetitem)
			{
				for (item = topcard->items,i = 0;item->type != uii_Bad;item++,i++)
				{
					if (item->hotkey == LastScan)
					{
						USL_SelectItem(topcard,i,true);
						resetitem = true;
						break;
					}
				}
			}

			IN_ClearKeysDown();
		}
		else
		{
			IN_ReadCursor(&cursorinfo);
			ydelta += cursorinfo.y;
			if (cursorinfo.button0)
			{
				do
				{
					IN_ReadCursor(&cursorinfo);
				} while (cursorinfo.button0);
				USL_DoItem();
				resetitem = true;
			}
			else if (cursorinfo.button1)
			{
				do
				{
					IN_ReadCursor(&cursorinfo);
				} while (cursorinfo.button1);
				USL_UpLevel();
				resetitem = true;
			}
			else if (ydelta < -MoveMin)
			{
				ydelta += MoveMin;
				USL_PrevItem();
				resetitem = true;
			}
			else if (ydelta > MoveMin)
			{
				ydelta -= MoveMin;
				USL_NextItem();
				resetitem = true;
			}
		}
	}*/
	USL_TearDownCtlPanel();
	NewGame();
}
