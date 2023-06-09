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

#include "CK_Heads.h"

// From CK_PLAY.C
/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

boolean singlestep, jumpcheat, godmode, keenkilled;

exittype playstate;
gametype gamestate;

Uint16 originxtilemax;
Uint16 originytilemax;

ControlInfo c;

// ck_newobj is known as 'new' in OG code
GBA_IN_IWRAM objtype *ck_newobj = NULL, *player = NULL, *scoreobj = NULL;

Sint16 invincible;

short showscorebox;

Sint16 groundslam;

boolean debugok;

boolean jumpbutton, jumpheld, pogobutton, pogoheld, firebutton, fireheld, upheld;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/
Sint16 oldfirecount;

Uint16 centerlevel;
Sint16 scrollcount;

Sint16 inactivateleft;
Sint16 inactivateright;
Sint16 inactivatetop;
Sint16 inactivatebottom;


//===========================================================================


/*
=====================
=
= CheckKeys
=
=====================
*/
bool HITBITTON = 0;
void CheckKeys(void){
	if (screenfaded)			// don't do anything with a faded screen
	{
		return;
	}

//
// Enter for status screen
//
	if (LastScan == GBA_BUTTON_SELECT)
	{
		StatusWindow();
		IN_ClearKeysDown();
		RF_ForceRefresh();
		lasttimecount = TimeCount;	// BUG: should be the other way around

	}

//
// pause key wierdness can't be checked as a scan code
//
	if (Paused)
	{
		SD_MusicOff();
//		VW_FixRefreshBuffer();
		US_CenterWindow(8, 3);
		US_PrintCentered("PAUSED");
		VW_UpdateScreen();
		IN_Ack();
//		RF_ForceRefresh();
		Paused = false;
		SD_MusicOn();
	}

#ifndef KEEN6
//
// F1 to enter help screens
//
	if (LastScan == (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER) )
	{
		StopMusic();
		HelpScreens();
		StartMusic(gamestate.mapon);
		if (showscorebox)
		{
			scoreobj->temp2 = -1;
			scoreobj->temp1 = -1;
			scoreobj->temp3 = -1;
			scoreobj->temp4 = -1;
		}
		RF_ForceRefresh();
	}
#endif
//
// F2-F7/ESC to enter control panel
//
		if (LastScan == GBA_BUTTON_START)
		{
			StopMusic();
			US_ControlPanel();
			RF_FixOfs(0,0);
			StartMusic(gamestate.mapon);
			if (!showscorebox && scoreobj->sprite)
			{
				RF_RemoveSprite(&scoreobj->sprite, true);
			}
			if (showscorebox)
			{
				scoreobj->temp2 = -1;
				scoreobj->temp1 = -1;
				scoreobj->temp3 = -1;
				scoreobj->temp4 = -1;
			}
			IN_ClearKeysDown();
			if (restartgame)
			{
				playstate = ex_resetgame;
			}
			else if (!loadedgame)
			{
				RF_ForceRefresh();
			}
			if (abortgame)
			{
				abortgame = false;
				playstate = ex_abortgame;
			}
			if (loadedgame)
			{
				playstate = ex_loadedgame;
			}
			lasttimecount = TimeCount;	// BUG: should be the other way around
		}

}


//===========================================================================

/*
==================
=
= PrintNumbers
=
==================
*/

void PrintNumbers(Sint16 x, Sint16 y, Sint16 maxlen, Sint16 basetile, Sint32 number)
{
	register Sint16 i;
	Sint16 len;
	char buffer[20];

	_ck_ltoa(number, buffer, 10);
	len = strlen(buffer);
	i = maxlen;
	while (i>len)
	{
		VWB_DrawTile8(x, y, basetile);
		i--;
		x += 8;
	}
	while (i>0)
	{
		VWB_DrawTile8(x, y, basetile+buffer[len-i]+(1-'0'));
		i--;
		x += 8;
	}
}

/*
==================
=
= DrawStatusWindow
=
==================
*/

#define BACKCOLOR CK_EGA_GREY
#define TEXTBACK WHITE
#define NUMBERBACK BLACK

void DrawStatusWindow(void)
{
	Sint16 off, x, y, w, h, i;
	Uint16 width, height;
	Sint16 offx = -32, offy = -8;
	x = 64+offx;
	y = 16+offy;
	w = 184;
	h = 144;
	VWB_DrawTile8(x, y, 54);
	VWB_DrawTile8(x, y+h, 60);
	for (i=x+8; i<=x+w-8; i+=8)
	{
		VWB_DrawTile8(i, y, 55);
		VWB_DrawTile8(i, y+h, 61);
	}
	VWB_DrawTile8(i, y, 56);
	VWB_DrawTile8(i, y+h, 62);
	for (i=y+8; i<=y+h-8; i+=8)
	{
		VWB_DrawTile8(x, i, 57);
		VWB_DrawTile8(x+w, i, 59);
	}
	VWB_Bar(72+offx, 24+offy, 176, 136, BACKCOLOR);

	PrintY = 28+offy;
	WindowX = 80+offx;
	WindowW = 160;
	US_CPrint("LOCATION");
	VWB_Bar(80+offx, 38+offy, 162, 24, TEXTBACK);

	char str[32];
#ifdef KEEN5
	if (mapon == 0 && player->y > 100*TILEGLOBAL)
		_ck_strcpy(str, levelnames[13]);
	else
		_ck_strcpy(str, levelnames[gamestate.mapon]);
#else
	_ck_strcpy(str, levelnames[gamestate.mapon]);
#endif
	SizeText(str, &width, &height);
	PrintY = (20-height)/2+40-2+offy;
	US_CPrint(str);

	PrintY = 61+offy;
	WindowX = 80+offx;
	WindowW = 64;
	US_CPrint(" SCORE");
	VWB_Bar(80+offx, 70+offy, 66, 10, NUMBERBACK);
	PrintNumbers(80+offx, 70+offy, 8, 41, gamestate.score);

	PrintY = 61+offy;
	WindowX = 176+offx;
	WindowW = 64;
	US_CPrint("EXTRA");
	VWB_Bar(175+offx, 71+offy, 66, 10, NUMBERBACK);
	PrintNumbers(175+offx, 71+offy, 8, 41, gamestate.nextextra);

#if defined KEEN4
	PrintY = 80+offy;
	WindowX = 80+offx;
	WindowW = 64;
	US_CPrint(" RESCUED");
	VWB_Bar(80+offx, 95+offy, 66, 10, NUMBERBACK);
	for (i = 0; i < gamestate.rescued; i++, off+=8)
	{
		VWB_DrawTile8(i*8 + 80+offx, 95+offy, 40);
	}
#elif defined KEEN5
	PrintY = 92+offy;
	PrintX = 80+offx;
	US_Print("KEYCARD");
	VWB_Bar(135+offx, 91+offy, 10, 10, NUMBERBACK);
	if (gamestate.keycard)
	{
		VWB_DrawTile8(136+offx, 92+offy, 40);
	}
#endif

	PrintY = 85+offy;
	WindowX = 176+offx;
	WindowW = 64;
	US_CPrint(" LEVEL");
	VWB_Bar(178+offx, 95+offy, 66, 10, TEXTBACK);
	PrintY = 90+offy;
	WindowX = 176+offx;
	WindowW = 64;
	switch (gamestate.difficulty)
	{
	case gd_Easy:
		US_CPrint("Easy");
		break;
	case gd_Normal:
		US_CPrint("Normal");
		break;
	case gd_Hard:
		US_CPrint("Hard");
		break;
	}

#ifdef KEEN6
	PrintX = 80+offx;
	PrintY = 96+offy;
	US_Print("ITEMS");
	VWB_Bar(127+offx, 95+offy, 26, 10, NUMBERBACK);
	if (gamestate.sandwichstate == 1)
	{
		VWB_DrawTile8(128+offx, 96+offy, 2);
	}
	else
	{
		VWB_DrawTile8(128+offx, 96+offy, 1);
	}
	if (gamestate.hookstate == 1)
	{
		VWB_DrawTile8(136+offx, 96+offy, 4);
	}
	else
	{
		VWB_DrawTile8(136+offx, 96+offy, 3);
	}
	if (gamestate.passcardstate == 1)
	{
		VWB_DrawTile8(144+offx, 96+offy, 6);
	}
	else
	{
		VWB_DrawTile8(144+offx, 96+offy, 5);
	}
#endif

	PrintX = 80+offx;
	PrintY = 110+offy;
	US_Print("KEYS");
	VWB_Bar(120+offx, 108+offy, 34, 10, NUMBERBACK);
	for (i = 0; i < 4; i++)
	{
		if (gamestate.keys[i])
		{
			VWB_DrawTile8(i*8+120+offx, 108+offy, 36+i);
		}
	}

	PrintX = 176+offx;
	PrintY = 112+offy;
	US_Print("AMMO");
	VWB_Bar(216+offx, 112+offy, 26, 10, NUMBERBACK);
	PrintNumbers(216+offx, 112+offy, 3, 41, gamestate.ammo);

	PrintX = 80+offx;
	PrintY = 128+offy;
	US_Print("KEENS");
	VWB_Bar(128+offx, 128+offy, 18, 10, NUMBERBACK);
	PrintNumbers(128+offx, 128+offy, 2, 41, gamestate.lives);

	PrintX = 176+offx;
	PrintY = 128+offy;
	US_Print(DROPSNAME);
	VWB_Bar(224+offx, 128+offy, 16, 10, NUMBERBACK);
	PrintNumbers(224+offx, 128+offy, 2, 41, gamestate.drops);

#ifdef KEEN4
	VWB_Bar(80+offx, 144+offy, 66, 10, TEXTBACK);
	PrintY = 144+offy;
	WindowX = 80+offx;
	WindowW = 64;
	if (gamestate.wetsuit)
	{
		US_CPrint("Wetsuit");
	}
	else
	{
		US_CPrint("???");
	}
#endif

	// draw the tiles for "PRESS A KEY":
	for (i = 0; i < 10; i++)
	{
		VWB_DrawTile8(i*8+STATUS_PRESSKEY_X+offx, 140+offy, i+72);
		VWB_DrawTile8(i*8+STATUS_PRESSKEY_X+offx, 148+offy, i+82);
	}
}

/*
==================
=
= ScrollStatusWindow
=
==================
*/

void ScrollStatusWindow(void)
{/*
	Uint16 source, dest;
	Sint16 height;

	if (vislines > 152)
	{
		height = vislines - 152;
		source = windowofs + panadjust + 8;
		dest = bufferofs + panadjust + 8;
		VW_ScreenToScreen(source, dest, 192/BYTEPIXELS, height);
		VW_ClipDrawMPic((pansx+136)/BYTEPIXELS, -(16-height)+pansy, METALPOLEPICM);
		source = windowofs + panadjust + 16*SCREENWIDTH + 8*CHARWIDTH;
		dest = bufferofs + panadjust + height*SCREENWIDTH + 8;
		height = 152;
	}
	else
	{
		source = windowofs + panadjust + (152-vislines)*SCREENWIDTH + 16*SCREENWIDTH + 8*CHARWIDTH;
		dest = bufferofs + panadjust + 8;
		height = vislines;
	}
	if (height > 0)
	{
		VW_ScreenToScreen(source, dest, 192/BYTEPIXELS, height);
	}
	if (scrollup)
	{
		height = 168-vislines;
		source = masterofs + panadjust + vislines*SCREENWIDTH + 8;
		dest = bufferofs + panadjust + vislines*SCREENWIDTH + 8;
		VW_ScreenToScreen(source, dest, 192/BYTEPIXELS, height);
		height = vislines;
		source = windowofs + panadjust + 8 - 24/BYTEPIXELS;
		dest = bufferofs + panadjust + 8 - 24/BYTEPIXELS;
		if (height > 0)
			VW_ScreenToScreen(source, dest, 24/BYTEPIXELS, height);
	}
	else
	{
		height = vislines + -72;
		if (height > 0)
		{
			source = windowofs + panadjust + 8 - 24/BYTEPIXELS;
			dest = bufferofs + panadjust + 8 - 24/BYTEPIXELS;
			if (height > 0)
				VW_ScreenToScreen(source, dest, 24/BYTEPIXELS, height);
		}
	}
	if (vislines >= 72)
	{
		VW_ClipDrawMPic((pansx+40)/BYTEPIXELS, vislines-168+pansy, CORDPICM);
	}
	VW_UpdateScreen();*/
}

/*
==================
=
= StatusWindow
=
==================
*/

void StatusWindow(void)
{
	// Draw it like CGA mode
//#if GRMODE == CGAGR
/*
	if (Keyboard[sc_A] && Keyboard[sc_2])
	{
		US_CenterWindow(20, 2);
		PrintY += 2;
		US_Print("Debug keys active");
		VW_UpdateScreen();
		IN_Ack();
		debugok = true;
	}
*/
	// Fix the scroll offsets
	VW_FixGraphics();
	// Hide all the sprites
	GBA_HideSprites();
	// Fix the scroll
	CK_SnapScroll();

	WindowX = 0;
	WindowW = 240;
	WindowY = 0;
	WindowH = 160;
	DrawStatusWindow();
	VW_UpdateScreen();
	IN_ClearKeysDown();
	IN_Ack();

//#else
#ifdef STATIS_SCROL
	Uint16 oldbufferofs;

	WindowX = 0;
	WindowW = 320;
	WindowY = 0;
	WindowH = 200;

	if (Keyboard[sc_A] && Keyboard[sc_2])
	{
		US_CenterWindow(20, 2);
		PrintY += 2;
		US_Print("Debug keys active");
		VW_UpdateScreen();
		IN_Ack();
		debugok = true;
	}

	RF_Refresh();
	RFL_InitAnimList();
	oldbufferofs = bufferofs;
	bufferofs = windowofs = RF_FindFreeBuffer();
	VW_ScreenToScreen(displayofs, displayofs, 44, 224);	// useless (source and dest offsets are identical)
	VW_ScreenToScreen(displayofs, masterofs, 44, 224);
	VW_ScreenToScreen(displayofs, bufferofs, 44, 168);
	DrawStatusWindow();
	bufferofs = oldbufferofs;
	RF_Refresh();

	SD_PlaySound(SND_SHOWSTATUS);
	vislines = 16;
	scrollup = false;
	RF_SetRefreshHook(ScrollStatusWindow);

	while (true)
	{
		RF_Refresh();
		if (vislines == 168)
			break;
		vislines = vislines + tics*8;
		if (vislines > 168)
			vislines = 168;
	}

	RF_Refresh();
	RF_SetRefreshHook(NULL);
	IN_ClearKeysDown();
	IN_Ack();

	SD_PlaySound(SND_HIDESTATUS);
	vislines -= 16;
	scrollup = true;
	RF_SetRefreshHook(ScrollStatusWindow);

	while (true)
	{
		RF_Refresh();
		if (vislines == 0)
			break;
		vislines = vislines - tics*8;
		if (vislines < 0)
			vislines = 0;
	}

	RF_SetRefreshHook(NULL);

	scoreobj->x = 0;	//force scorebox to redraw?

#endif*/
}


//===========================================================================

		
/*
==================
=
= CenterActor
=
==================
*/

void CenterActor(objtype *ob)
{
	Uint16 orgx, orgy;

	centerlevel = CK_HALF_SCREENY;
	if (ob->x < 114*PIXGLOBAL) // 152
	{
		orgx = 0;
	}
	else
	{
		orgx = ob->x - 114*PIXGLOBAL; // 152
	}
	if (mapon == 0)
	{
		if (ob->y < 60*PIXGLOBAL) // 80
		{
			orgy = 0;
		}
		else
		{
			orgy = ob->y - 60*PIXGLOBAL; // 80
		}
	}
	else
	{
		if (ob->bottom < CK_HALF_SCREENY*PIXGLOBAL)
		{
			orgy = 0;
		}
		else
		{
			orgy = ob->bottom - CK_HALF_SCREENY*PIXGLOBAL;
		}
	}
	if (!scorescreenkludge)
	{
		CK_MoveCamera(orgx, orgy);
		CK_ForceLevelRedraw();
	}

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}
}


//===========================================================================

/*
==================
=
= WorldScrollScreen
=
= Scroll if Keen is nearing an edge
=
==================
*/

void WorldScrollScreen(objtype *ob)
{
	Sint16 xscroll, yscroll;

	if (keenkilled)
		return;

	if (ob->left < originxglobal + 9*TILEGLOBAL)
	{
		xscroll = ob->left - (originxglobal + 9*TILEGLOBAL);
	}
	else if (ob->right > originxglobal + 12*TILEGLOBAL)
	{
		xscroll = ob->right + 16 - (originxglobal + 12*TILEGLOBAL);
	}
	else
	{
		xscroll = 0;
	}

	if (ob->top < originyglobal + 5*TILEGLOBAL)
	{
		yscroll = ob->top - (originyglobal + 5*TILEGLOBAL);
	}
	else if (ob->bottom > originyglobal + 7*TILEGLOBAL)
	{
		yscroll = ob->bottom - (originyglobal + 7*TILEGLOBAL);
	}
	else
	{
		yscroll = 0;
	}

	if (!xscroll && !yscroll)
		return;

//
// don't scroll more than one tile per frame
//
	if (xscroll >= 0x100)
	{
		xscroll = 0xFF;
	}
	else if (xscroll <= -0x100)
	{
		xscroll = -0xFF;
	}
	if (yscroll >= 0x100)
	{
		yscroll = 0xFF;
	}
	else if (yscroll <= -0x100)
	{
		yscroll = -0xFF;
	}

    CK_ScrollCamera(xscroll, yscroll);

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}

}

//===========================================================================

/*
==================
=
= ScrollScreen
=
= Scroll if Keen is nearing an edge
= Set playstate to ex_completes
=
==================
*/

void ScrollScreen(objtype *ob)
{
	Sint16 xscroll, yscroll, pix, speed;
	Uint16 bottom;

	if (keenkilled)
		return;

//
// walked off edge of map?
//
	if (ob->left < originxmin || ob->right > originxmax + GBA_SCREEN_WIDTH*PIXGLOBAL)
	{
		playstate = ex_completed;
		return;
	}

//
// fallen off bottom of world?
//
	if (ob->bottom > originymax + 13*TILEGLOBAL)
	{
		ob->y -= ob->bottom - (originymax + 13*TILEGLOBAL);
		SD_PlaySound(SND_PLUMMET);
		godmode = false;
		KillKeen();
		return;
	}

	xscroll=yscroll=0;

	if (ob->x < originxglobal + 7*TILEGLOBAL) // 9
	{
		xscroll = ob->x - (originxglobal + 7*TILEGLOBAL); // 9
	}
	else if (ob->x > originxglobal + 9*TILEGLOBAL) // 12
	{
		xscroll = ob->x - (originxglobal + 9*TILEGLOBAL); // 12
	}
	if (ob->state == &s_keenlookup2)
	{
		if (centerlevel+tics > 167) // CK_HALF_SCREENY
		{
			pix = 167-centerlevel;
		}
		else
		{
			pix = tics;
		}
		centerlevel += pix;
		yscroll = CONVERT_PIXEL_TO_GLOBAL(-pix);
	}
	else if (ob->state == &s_keenlookdown3)
	{
		if (centerlevel-tics < 33)
		{
			pix = centerlevel + -33;
		}
		else
		{
			pix = tics;
		}
		centerlevel -= pix;
		yscroll = CONVERT_PIXEL_TO_GLOBAL(pix);
	}

#ifdef KEEN6
	if (groundslam)
	{
		const static Sint16 shaketable[] = {0,
			 -64,  -64,  -64,  64,  64,  64,
			-200, -200, -200, 200, 200, 200,
			-250, -250, -250, 250, 250, 250,
			-250, -250, -250, 250, 250, 250
		};
		yscroll = yscroll + (bottom - (ob->bottom + shaketable[groundslam]));	// BUG: 'bottom' has not been initialized yet!
	}
	else
#endif
	// Allows for keen to look up/down
	// makes scrolling slow
	//centerlevel = 0;
	if ( (ob->hitnorth || !ob->needtoclip || ob->state == &s_keenholdon))
	{
		if (  ob->state != &s_keenclimbup
			&& ob->state != &s_keenclimbup2
			&& ob->state != &s_keenclimbup3
			&& ob->state != &s_keenclimbup4)
		{
			yscroll += ob->ymove;
			bottom = originyglobal + yscroll + CONVERT_PIXEL_TO_GLOBAL(centerlevel);
			if (ob->bottom == bottom)
			{
				// player didn't move, no additional scrolling
			}
			else
			{
				if (ob->bottom < bottom)
				{
					pix = bottom - ob->bottom;
				}
				else
				{
					pix = ob->bottom - bottom;
				}
				speed = CONVERT_PIXEL_TO_GLOBAL(pix) >> 7;
				if (speed > 0x30)
				{
					speed = 0x30;
				}
				speed *= tics;
				if (speed < 0x10)
				{
					if (pix < 0x10)
					{
						speed = pix;
					}
					else
					{
						speed = 0x10;
					}
				}
				if (ob->bottom < bottom)
				{
					yscroll -= speed;
				}
				else
				{
					yscroll += speed;
				}
			}
		}
	}
	else
	{
		centerlevel = CK_HALF_SCREENY;
	}

	pix = (ob->bottom-32*PIXGLOBAL)-(originyglobal+yscroll+32*PIXGLOBAL);
	if (pix < 0)
	{
		yscroll += pix;
	}
	pix = (ob->bottom+32*PIXGLOBAL)-(originyglobal+yscroll+200*PIXGLOBAL);
	if (pix > 0)
	{
		yscroll += pix;
	}

	if (xscroll == 0 && yscroll == 0)
		return;

//
// don't scroll more than one tile per frame
//
	if (xscroll >= 0x100)
	{
		xscroll = 0xFF;
	}
	else if (xscroll <= -0x100)
	{
		xscroll = -0xFF;
	}
	if (yscroll >= 0x100)
	{
		yscroll = 0xFF;
	}
	else if (yscroll <= -0x100)
	{
		yscroll = -0xFF;
	}

	CK_ScrollCamera(xscroll, yscroll);

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}
}


void ScrollScreen_Custom(objtype *ob)
{
	Sint16 xscroll, yscroll, pix, speed;
	Uint16 bottom;

	if (keenkilled)
		return;

//
// walked off edge of map?
//
	if (ob->left < originxmin || ob->right > originxmax + GBA_SCREEN_WIDTH*PIXGLOBAL)
	{
		playstate = ex_completed;
		return;
	}

//
// fallen off bottom of world?
//
	if (ob->bottom > originymax + 13*TILEGLOBAL)
	{
		ob->y -= ob->bottom - (originymax + 13*TILEGLOBAL);
		SD_PlaySound(SND_PLUMMET);
		godmode = false;
		KillKeen();
		return;
	}

	xscroll=yscroll=0;

	if (ob->left < originxglobal - 128 + 8*TILEGLOBAL)
	{
		xscroll = ob->left - (originxglobal - 128 + 8*TILEGLOBAL);
	}
	else if (ob->right > originxglobal + 128 + (9*TILEGLOBAL))
	{
		xscroll = ob->right + 16 - (originxglobal + 128 + (9*TILEGLOBAL));
	}
	else
	{
		xscroll = 0;
	}

	if (ob->top < originyglobal + 4*TILEGLOBAL)
	{
		yscroll = ob->top - (originyglobal + 4*TILEGLOBAL);
	}
	else if (ob->bottom > originyglobal + 6*TILEGLOBAL)
	{
		yscroll = ob->bottom - (originyglobal + 6*TILEGLOBAL);
	}
	else
	{
		yscroll = 0;
	}

	if (ob->state == &s_keenlookup2)
	{
		scrollcount++;
		if(scrollcount > 16){
			pix = scrollcount-16;
		}
		if(scrollcount > 48){
			pix = 32;
		}
		yscroll -= CONVERT_PIXEL_TO_GLOBAL(pix)*3;
	}
	else if (ob->state == &s_keenlookdown3)
	{
		scrollcount++;
		if(scrollcount > 16){
			pix = scrollcount-16;
		}
		if(scrollcount > 48){
			pix = 32;
		}
		yscroll += CONVERT_PIXEL_TO_GLOBAL(pix)*3;
	}else{
		scrollcount = 0;
	}

#ifdef KEEN6
	if (groundslam)
	{
		const static Sint16 shaketable[] = {0,
			 -64,  -64,  -64,  64,  64,  64,
			-200, -200, -200, 200, 200, 200,
			-250, -250, -250, 250, 250, 250,
			-250, -250, -250, 250, 250, 250
		};
		bottom = 0; // Fixed!
		yscroll = yscroll + (bottom - (ob->bottom + shaketable[groundslam]));	// BUG: 'bottom' has not been initialized yet!
	}
	else
#endif
	if ( (ob->hitnorth || !ob->needtoclip || ob->state == &s_keenholdon))
	{
		if (  ob->state != &s_keenclimbup
			&& ob->state != &s_keenclimbup2
			&& ob->state != &s_keenclimbup3
			&& ob->state != &s_keenclimbup4)
		{
			yscroll += ob->ymove;
		}
	}

	if (xscroll == 0 && yscroll == 0)
		return;

//
// don't scroll more than one tile per frame
//
	if (xscroll >= 0x100)
	{
		xscroll = 0xFF;
	}
	else if (xscroll <= -0x100)
	{
		xscroll = -0xFF;
	}
	if (yscroll >= 0x100)
	{
		yscroll = 0xFF;
	}
	else if (yscroll <= -0x100)
	{
		yscroll = -0xFF;
	}

	CK_ScrollCamera(xscroll, yscroll);

//
// update limits for onscreen and inactivate checks
//
	originxtilemax = (originxtile + PORTTILESWIDE) - 1;
	originytilemax = (originytile + PORTTILESHIGH) - 1;
	inactivateleft = originxtile - INACTIVATEDIST;
	if (inactivateleft < 0)
	{
		inactivateleft = 0;
	}
	inactivateright = originxtilemax + INACTIVATEDIST;
	if (inactivateright < 0)
	{
		inactivateright = 0;
	}
	inactivatetop = originytile - INACTIVATEDIST;
	if (inactivatetop < 0)
	{
		inactivatetop = 0;
	}
	inactivatebottom = originytilemax + INACTIVATEDIST;
	if (inactivatebottom < 0)
	{
		inactivatebottom = 0;
	}
}

/*
=========================
=
= InitObjArray
=
= Call to clear out the entire object list, returning them all to the free
= list.  Allocates a special spot for the player.
=
=========================
*/

void InitObjArray(void) {

    // Remove any old objects
    CK_NukeObjectsSprites();

//
// give the player and score the first free spots
//
	GetNewObj(false);
	player = ck_newobj;
	GetNewObj(false);
	scoreobj = ck_newobj;

};

//===========================================================================

/*
====================
=
= GivePoints
=
= Grants extra men at 20k,40k,80k,160k,320k
=
====================
*/

void GivePoints(Uint16 points)
{
	gamestate.score += points;
	if (!DemoMode && gamestate.score >= gamestate.nextextra)
	{
		SD_PlaySound(SND_EXTRAKEEN);
		gamestate.lives++;
		gamestate.nextextra *= 2;
	}
}


/*
===================
=
= PollControls
=
===================
*/

void PollControls(void)
{
	IN_ReadControl(0, &c);
	if (c.yaxis != -1)
		upheld = false;

	if (DemoMode)
	{
		if (c.button0 && c.button1)
		{
			firebutton = true;
			jumpbutton = pogobutton = jumpheld = pogoheld = false;
		}
		else
		{
			firebutton = fireheld = false;
			if (c.button0)
			{
				jumpbutton = true;
			}
			else
			{
				jumpbutton = jumpheld = false;
			}
			if (c.button1)
			{
				if (oldfirecount <= 8)
				{
					oldfirecount = oldfirecount + tics;
				}
				else
				{
					pogobutton = true;
				}
			}
			else
			{
				if (oldfirecount != 0)
				{
					pogobutton = true;
				}
				else
				{
					pogobutton = pogoheld = false;
				}
				oldfirecount = 0;
			}
		}
	}
	else
	{
		jumpbutton = c.button0;
		pogobutton = c.button1;
		firebutton = c.button2;
		if (!jumpbutton)
			jumpheld = false;
		if (!pogobutton)
			pogoheld = false;
		if (!firebutton)
			fireheld = false;
	}
}


//===========================================================================


/*
=================
=
= StopMusic
=
=================
*/

void StopMusic(void) {
	SD_MusicOff();
};

//==========================================================================

/*
=================
=
= StartMusic
=
=================
*/

extern const unsigned int CK_NumberOfSongs;
extern const unsigned int CK_LevelAudio[];

void StartMusic(Uint16 num)
{
	Sint16 song;
	boolean wasfaded;


	SD_MusicOff();

#ifdef KEEN4
	if (num == 0xFFFF || num > CK_NumberOfSongs) {
		song = WONDER_MUS;
	} else {
		song = CK_LevelAudio[num];
	}
#else
    song = CK_LevelAudio[num];
#endif

	if (song == -1 || MusicMode != smm_AdLib)
	{
		return;
	}
    SD_PlayMusic(song, 1);
}

//==========================================================================


/*
===================
=
= PlayLoop
=
===================
*/

void PlayLoop(void)
{
	objtype *check = NULL;

	StartMusic(gamestate.mapon);
	fireheld = pogoheld = upheld = jumpheld = false;
	ingame = true;
	playstate = ex_stillplaying;
	invincible = keenkilled = oldfirecount = 0;

	CK_ResetCamera();

	CenterActor(player);
	// Hmm....
	if(gamestate.mapon != 0)
		ScrollScreen_Custom(player);

	if (DemoMode)
	{
		US_InitRndT(false);
	}
	else
	{
		US_InitRndT(true);
	}
	TimeCount = lasttimecount = tics = 3;

	do
	{

		PollControls();

        //
        // go through state changes and propose movements
        //
		if(!player) Quit("PlayLoop : No player!");

		for (int i = player->uuid; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
			if(obj->removed) continue;

			if (!obj->active && obj->tileright >= originxtile-1
				&& obj->tileleft <= originxtilemax+1 && obj->tiletop <= originytilemax+1
				&& obj->tilebottom >= originytile-1)
			{
				obj->needtoreact = true;
				obj->active = ac_yes;
			}
			if (obj->active)
			{
				if (obj->tileright < inactivateleft
					|| obj->tileleft > inactivateright
					|| obj->tiletop > inactivatebottom
					|| obj->tilebottom < inactivatetop)
				{
					if (obj->active == ac_removable)
					{
						RemoveObj(obj);
						continue;
					}
					else if (obj->active != ac_allways)
					{
						if (US_RndT() < tics*2 || screenfaded || loadedgame)
						{
							// TODO:
							// Could be dangerous
							RF_RemoveSprite(&obj->sprite, true);
							if (obj->obclass == stunnedobj){
								RF_RemoveSprite(&obj->temp3, true);
							}
							obj->active = ac_no;
							continue;
						}
					}
				}
				StateMachine(obj);
			}
		}


		if (gamestate.riding)
		{
			HandleRiding(player);
		}

//
// check for and handle collisions between objects
//
		for (int i = player->uuid; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
			if(obj->removed) continue;
			if (obj->active)
			{
				for(int oi = i+1; oi < CK_NumOfObjects; oi++){
        			check = &CK_ObjectList[oi];

					if(check->removed) continue;

					if (!check->active)
					{
						continue;
					}
					if (obj->right > check->left && obj->left < check->right
						&& obj->top < check->bottom && obj->bottom > check->top)
					{
						if (obj->state->contact)
						{
							obj->state->contact(obj, check);
						}
						if (check->state->contact)
						{
							check->state->contact(check, obj);
						}
						if (obj->obclass == nothing)	//useless -- obclass is NOT set to nothing by RemoveObj
						{
							break;
						}
					}
				}
			}
		}

//
// check intiles
//
		if (mapon != 0)
		{
			CheckInTiles(player);
		}
		else
		{
			CheckWorldInTiles(player);
		}


//
// react to whatever happened, and post sprites to the refresh manager
//
		for (int i = player->uuid; i < CK_NumOfObjects; i++)
		{
            objtype *obj = &CK_ObjectList[i];
			if(obj->removed) continue;

			if (!obj->active)
			{
				continue;
			}
			if (obj->tilebottom >= CK_CurLevelHeight-1)
			{
				if (obj->obclass == keenobj)
				{
					playstate = ex_died;
				}
				else
				{
					RemoveObj(obj);
				}
				continue;
			}
			if (obj->needtoreact && obj->state->react)
			{
				obj->needtoreact = false;
				obj->state->react(obj);
			}
		}
//
// scroll the screen and update the score box
//
#ifdef KEEN4
		if (mapon != 0 && mapon != 17)
#else
		if (mapon != 0)
#endif
		{
			//ScrollScreen(player);
			ScrollScreen_Custom(player);
		}
		else
		{
			WorldScrollScreen(player);
		}
        
        CK_UpdateLevel();

		UpdateScore(scoreobj);

		if (loadedgame)
		{
			loadedgame = false;
		}

//
// update the screen and calculate the number of tics it took to execute
// this cycle of events (for adaptive timing of next cycle)
//
        CK_FixCamera();
        CK_RenderLevel();
		CK_UpdateSprites();
		VW_UpdateScreen();

        RF_CalcTics();


		if (invincible)
		{
			if ((invincible = invincible - tics) < 0)
				invincible = 0;
		}

#ifdef KEEN6
		if (groundslam)
		{
			if ((groundslam = groundslam - tics) < 0)
				groundslam = 0;
		}
#endif

//
// handle user inputs
//
		if (DemoMode == demo_Playback)
		{
			if (!screenfaded && IN_IsUserInput())
			{
				playstate = ex_completed;
				/*if (LastScan != sc_F1)
				{
					LastScan = sc_Space;
				}*/
			}
		}
		else if (DemoMode == demo_PlayDone)
		{
			playstate = ex_completed;
		}
		else
		{
			CheckKeys();
		}
/*
//
// E-N-D cheat
//
		if (Keyboard[sc_E] && Keyboard[sc_N] && Keyboard[sc_D])
		{
#if defined KEEN4
			gamestate.rescued = 7;
			playstate = ex_rescued;
#elif defined KEEN5
			playstate = ex_qedbroke;
#elif defined KEEN6
			playstate = ex_molly;
#endif
		}
*/
    
	} while (playstate == ex_stillplaying);

	ingame = false;
	StopMusic();
}