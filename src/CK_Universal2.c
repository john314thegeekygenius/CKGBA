
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

// From ID_US_2.C
extern const unsigned int CK_CWRIST_size;
extern const unsigned char CK_CWRIST[];



//      Special imports
extern  short         showscorebox;
#ifdef  KEEN
extern	boolean		jerk;
extern  boolean         oldshooting;
extern  ScanCode        firescan;
void	USL_CheckSavedGames(void);
#else
		ScanCode        firescan;
#endif
extern boolean godmode;
extern boolean jumpcheat;
extern boolean infiniteammo;
extern boolean infinitelives;
extern boolean debugok;

//      Global variables
		boolean         ingame,abortgame,loadedgame;
		GameDiff        restartgame = gd_Continue;
boolean 				CtlPanelDone; // Moved from internal

//      Internal variables
static  boolean         GameIsDirty,
					QuitToDos;

#ifdef KEEN6
	int		listindex = -1;
	boolean		checkpassed;
#endif

//      Forward reference prototypes
static void     USL_SetupCard(void);

//      Control panel data

#define CtlPanelSX      50 // 74
#define CtlPanelSY      32 // 48
#define CtlPanelEX      208 // 234
#define CtlPanelEY      134 // 150
#define CtlPanelW       (CtlPanelEX - CtlPanelSX)
#define CtlPanelH       (CtlPanelEY - CtlPanelSY)

#define TileBase        92
#define BackColor               8
#define HiliteColor     (BackColor ^ 2)
#define NohiliteColor   ((BackColor ^ 10)+1)

typedef enum
		{
			uc_None,
			uc_Return,
			uc_Abort,
			uc_Quit,
			uc_Loaded,
			uc_SEasy,
			uc_SNormal,
			uc_SHard,
			// Added
			uc_WipeRom,
			uc_Saving,
			uc_C_Rumble,
			//// Cheats
			uc_C_GodMode,
			uc_C_InfAmmo,
			uc_C_InfLives,
			uc_C_JumpCheat,
			uc_C_AllItems,
		} UComm;
typedef enum
		{
			uii_Bad,
			uii_Button,uii_RadioButton,uii_Folder
		} UIType;
typedef enum
		{
			ui_Normal = 0,
			ui_Pushed = 1,
			ui_Selected = 2,
			ui_Disabled = 4,
			ui_Separated = 8
		} UIFlags;
#define UISelectFlags (ui_Pushed | ui_Selected | ui_Disabled)

typedef enum
		{
			uic_SetupCard,uic_DrawCard,uic_TouchupCard,
			uic_DrawIcon,uic_Draw,uic_Hit
		} UserCall;

typedef struct  UserItem
		{
				UIType                  type;
				UIFlags                 flags;
				ScanCode                hotkey;
				char                    *text;
				UComm                   comm;
				void                    *child;     // Should be (UserItemGroup *)

				word                    x,y;
		} UserItem;
typedef struct  UserItemGroup
		{
				word                    x,y;
				graphicnums             title;
				ScanCode                hotkey;
				UserItem                *items;
				boolean                 (*custom)(UserCall,struct UserItem *);      // Custom routine

				word                    cursor;
		struct  UserItemGroup   *parent;
		} UserItemGroup;

static  char            *BottomS1,*BottomS2,*BottomS3;
static  UComm           Communication;
static  ScanCode        *KeyMaps[] =
					{
						GBA_BUTTON_A,
						GBA_BUTTON_B,
						GBA_BUTTON_A,
						(GBA_BUTTON_UP|GBA_BUTTON_LEFT),
						GBA_BUTTON_UP,
						(GBA_BUTTON_UP|GBA_BUTTON_RIGHT),
						GBA_BUTTON_RIGHT,
						(GBA_BUTTON_DOWN|GBA_BUTTON_RIGHT),
						GBA_BUTTON_DOWN,
						(GBA_BUTTON_DOWN|GBA_BUTTON_LEFT),
						GBA_BUTTON_LEFT
					};

// Custom routine prototypes
static  boolean USL_ConfigCustom(UserCall call,struct UserItem *item),
				USL_KeyCustom(UserCall call,struct UserItem *item),
				USL_KeySCustom(UserCall call,struct UserItem *item),
				USL_Joy1Custom(UserCall call,struct UserItem *item),
				USL_Joy2Custom(UserCall call,struct UserItem *item),
				USL_JoyGCustom(UserCall call,struct UserItem *item),
				USL_LoadCustom(UserCall call,struct UserItem *item),
				USL_SaveCustom(UserCall call,struct UserItem *item),
				USL_ScoreCustom(UserCall call,struct UserItem *item),
				USL_CompCustom(UserCall call,struct UserItem *item),
				USL_SmoothCustom(UserCall call,struct UserItem *item),
#ifdef KEEN
				USL_TwoCustom(UserCall call,struct UserItem *item),
#endif
				USL_PongCustom(UserCall call,struct UserItem *item),
				USL_ResetCustom(UserCall call,struct UserItem *item),
				USL_VideoCustom(UserCall call,struct UserItem *item),
				USL_CheatCustom(UserCall call,struct UserItem *item);


#define DefButton(key,text)                             uii_Button,ui_Normal,key,text
#define DefRButton(key,text)                    uii_RadioButton,ui_Normal,key,text
#define DefFolder(key,text,child)               uii_Folder,ui_Normal,key,text,uc_None,child
#define CustomGroup(title,key,custom)   0,0,title,key,0,custom
	UserItem holder[] =
	{
		{DefButton(0,"DEBUG")},
		{uii_Bad}
	};
	UserItemGroup   holdergroup = {0,0,CP_MAINMENUPIC,0,holder};

	// Sound menu
	UserItem soundi[] =
	{
		{DefRButton(0,"NO SOUND")},
		{DefRButton(0,"PC SPEAKER")},
		{DefRButton(0,"ADLIB")},
		{uii_Bad}
	};
	UserItemGroup   soundgroup = {0,8,CP_SOUNDMENUPIC,0,soundi};

	// Music menu
	UserItem musici[] =
	{
		{DefRButton(0,"NO MUSIC")},
		{DefRButton(0,"ADLIB")},
//		{DefRButton(0,"FANCY")},
		{uii_Bad}
	};
	UserItemGroup   musicgroup = {0,8,CP_MUSICMENUPIC,0,musici};

	// New game menu
	UserItem newgamei[] =
	{
		{DefButton(0,"BEGIN EASY GAME"),uc_SEasy},
		{DefButton(0,"BEGIN NORMAL GAME"),uc_SNormal},
		{DefButton(0,"BEGIN HARD GAME"),uc_SHard},
		{uii_Bad}
	};
	UserItemGroup   newgamegroup = {0,8,CP_NEWGAMEMENUPIC,0,newgamei,0,1};

	// Load/Save game menu
	UserItem loadsavegamei[] =
	{
#ifdef CAT3D
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
#else
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
		{uii_Button,ui_Normal,0},
//		{uii_Button,ui_Normal,0},
//		{uii_Button,ui_Normal,0},
//		{uii_Button,ui_Normal,0},
#endif
		{uii_Bad}
	};
	UserItemGroup   loadgamegroup = {4,8,CP_LOADMENUPIC,0,loadsavegamei,USL_LoadCustom};
	UserItemGroup   savegamegroup = {4,8,CP_SAVEMENUPIC,0,loadsavegamei,USL_SaveCustom};

	// Options menu
	UserItemGroup   scoregroup = {0,0,0,0,0,USL_ScoreCustom};
//#ifdef KEEN
//	UserItemGroup   twogroup = {0,0,0,0,0,USL_TwoCustom};
//#endif
#if GRMODE != CGAGR
	UserItemGroup   smoothgroup = {0,0,0,0,0,USL_SmoothCustom};
	UserItemGroup   compgroup = {0,0,0,0,0,USL_CompCustom};
#endif

	UserItem optionsi[] =
	{
		{DefFolder(0,"",&scoregroup)}, // Scorebox
		{uii_Button,ui_Normal,0,"",uc_C_Rumble}, // Rumble
//#ifdef KEEN
//		{DefFolder(0,"",&twogroup)},
//#endif
#if GRMODE != CGAGR
		{DefFolder(0,"",&smoothgroup)},
		{DefFolder(0,"",&compgroup)},
#endif
		{uii_Bad}
	};
	UserItemGroup  optionsgroup = {0,8,CP_OPTIONSMENUPIC,0,optionsi};

	// Keyboard menu
	UserItem keyi[] =
	{
		{DefButton(0,"UP & LEFT")},
		{DefButton(0,"UP")},
		{DefButton(0,"UP & RIGHT")},
		{DefButton(0,"RIGHT")},
		{DefButton(0,"DOWN & RIGHT")},
		{DefButton(0,"DOWN")},
		{DefButton(0,"DOWN & LEFT")},
		{DefButton(0,"LEFT")},
		{uii_Bad}
	};
	UserItemGroup  keygroup = {8,8,CP_KEYMOVEMENTPIC,0,keyi,USL_KeyCustom};
	UserItem keybi[] =
	{
#ifdef  KEEN
		{DefButton(0,"JUMP")},
		{DefButton(0,"POGO")},
		{DefButton(0,"FIRE")},
#endif
#ifdef  CAT3D
		{DefButton(0,"FIRE")},
		{DefButton(0,"STRAFE")},
#endif
#ifdef  CPD
		{DefButton(0,"SHOOT")},
		{DefButton(0,"BOMB")},
#endif
		{uii_Bad}
	};
	UserItemGroup   keybgroup = {8,8,CP_KEYBUTTONPIC,0,keybi,USL_KeyCustom};

	UserItem videoi[] =
	{
		{DefRButton(0,"EGA PALETTE")},
		{DefRButton(0,"GREY PALETTE")},
		{DefRButton(0,"GB PALETTE")},
		{DefRButton(0,"BRIGHT PALETTE")},
		{DefRButton(0,"C64 PALETTE")},
#ifdef CK_DYNAMIC_PAL
		{DefRButton(0,"DYNAMIC PALETTE")},
#endif
		{uii_Bad}
	};
	UserItemGroup  videogroup = {8,8,CP_VIDEOMOVEMENTPIC,0,videoi,USL_VideoCustom};

	UserItem cheati[] =
	{
		{uii_Button,ui_Normal,0,"GOD MODE",uc_C_GodMode},
		{uii_Button,ui_Normal,0,"JUMP CHEAT",uc_C_JumpCheat},
		{uii_Button,ui_Normal,0,"INFIN AMMO",uc_C_InfAmmo},
		{uii_Button,ui_Normal,0,"INFIN LIVES",uc_C_InfLives},
		{uii_Button,ui_Normal,0,"FULL ITEMS",uc_C_AllItems},
		{uii_Bad}
	};
	UserItemGroup  cheatgroup = {8,8,CP_CONFIGMENUPIC,0,cheati,USL_CheatCustom};
	

	UserItem  keysi[] =
	{
		{DefFolder(0,"MOVEMENT",&keygroup)},
		{DefFolder(0,"BUTTONS",&keybgroup)},
		{uii_Bad}
	};
	UserItemGroup   keysgroup = {8,8,CP_KEYBOARDMENUPIC,0,keysi,USL_KeySCustom};

	UserItemGroup  resetgroup = {0,0,0,0,0,USL_ResetCustom};

	// Joystick #1 & #2
	UserItemGroup   joy1group = {CustomGroup(CP_JOYSTICKMENUPIC,0,USL_Joy1Custom)};
	UserItemGroup   joy2group = {CustomGroup(CP_JOYSTICKMENUPIC,0,USL_Joy2Custom)};
	UserItemGroup   gravisgroup = {CustomGroup(CP_JOYSTICKMENUPIC,0,USL_JoyGCustom)};

	// Config menu
	UserItem configi[] =
	{
		{DefFolder(0,"SOUND",&soundgroup)},
		{DefFolder(0,"MUSIC",&musicgroup)},
		{DefFolder(0,"OPTIONS",&optionsgroup)},
		{DefFolder(0,"VIDEO",&videogroup)},
		{uii_Folder,ui_Separated,0,"RESET ROM",uc_None,&resetgroup},
		{uii_Bad}
	};
#ifdef CAT3D
	UserItemGroup   configgroup = {8,0,CP_CONFIGMENUPIC,0,configi,USL_ConfigCustom};
#else
	UserItemGroup   configgroup = {0,8,CP_CONFIGMENUPIC,0,configi,USL_ConfigCustom};
#endif

	// Main menu
	UserItemGroup  ponggroup = {0,0,0,0,0,USL_PongCustom};
#ifndef NOCHEATS
	UserItem rootcheati = {DefFolder(0,"CHEATS",&cheatgroup)};
#endif
	UserItem rooti[] =
	{
		{DefFolder(0,"NEW GAME",&newgamegroup)},
		{DefFolder(0,"LOAD GAME",&loadgamegroup)},
		{DefFolder(0,"SAVE GAME",&savegamegroup)},
		{DefFolder(0,"CONFIGURE",&configgroup)},
		{DefButton(0,nil),uc_Return},        // Return to Game/Demo
		{DefButton(0,"END GAME"),uc_Abort},
#ifdef KEEN
		{DefFolder(0,"PADDLE WAR",&ponggroup)},
#elif defined CAT3D
		{DefFolder(0,"SKULL 'N' BONES",&ponggroup)},
#endif
#ifdef CK_INTR_ENDER
		{DefButton(0,"QUIT"),uc_Quit},
#endif
#ifndef NOCHEATS
		{uii_Bad}, // Dummy item for cheats menu
#endif
		{uii_Bad}
	};
	UserItemGroup  rootgroup = {16,8,CP_MAINMENUPIC,0,rooti};
#undef  DefButton
#undef  DefFolder

#define MaxCards        7
	word                    cstackptr;
	UserItemGroup   *cardstack[MaxCards],
					 *topcard;


boolean cheatsEnabled = false;

//      Card stack code
static void
USL_SetupStack(void)
{
	cstackptr = 0;
	cardstack[0] = topcard = &rootgroup;
}

static void
USL_PopCard(void)
{
	if (!cstackptr)
		return;

	topcard = cardstack[--cstackptr];
}

static void
USL_PushCard(UserItemGroup *card)
{
	if (cstackptr == MaxCards - 1)
		return;

	topcard = cardstack[++cstackptr] = card;
}

static void
USL_DrawItemIcon(UserItem *item)
{
	word    flags,tile;

	if (topcard->custom && topcard->custom(uic_DrawIcon,item))
		return;

	flags = item->flags;
	if (flags & ui_Disabled)
		tile = TileBase + ((flags & ui_Selected)? 5 : 4);
	else if ((item->type == uii_RadioButton) && (!(flags & ui_Pushed)))
		tile = TileBase + ((flags & ui_Selected)? 3 : 2);
	else
		tile = TileBase + ((flags & ui_Selected)? 1 : 0);
	VWB_DrawTile8(item->x,item->y,tile);
}

static void
USL_DrawItem(UserItem *item)
{
	if (topcard->custom && topcard->custom(uic_Draw,item))
		return;

	VWB_Bar(CtlPanelSX + 1,item->y,
			CtlPanelEX - CtlPanelSX - 1,8,BackColor);       // Clear out background
	USL_DrawItemIcon(item);
	if ((item->flags & ui_Selected) && !(item->flags & ui_Disabled))
		fontcolor = HiliteColor;
	else
		fontcolor = NohiliteColor;
	PrintX = item->x + 8;
	PrintY = item->y + 1;
	USL_DrawString(item->text);
	fontcolor = F_BLACK;
}

#define MyLine(y)       VWB_Hlin(CtlPanelSX + 3,CtlPanelEX - 3,y,10);

static void
USL_DrawBottom(void)
{
	word    w,h;

	fontcolor = NohiliteColor;

	PrintX = CtlPanelSX + 8;
	PrintY = CtlPanelEY - 16;
	USL_DrawString(BottomS1);

	USL_MeasureString(BottomS2,&w,&h);
	PrintX = CtlPanelEX - 8 - w;
	USL_DrawString(BottomS2);

	USL_MeasureString(BottomS3,&w,&h);
	PrintX = CtlPanelSX + ((CtlPanelEX - CtlPanelSX - w) / 2);
	PrintY += h + 1;
	USL_DrawString(BottomS3);

	fontcolor = F_WHITE;
	MyLine(CtlPanelEY - 22);
}

static void
USL_DrawCtlPanelContents(void)
{
	int                             x,y;
	UserItem                 *item;

	if (topcard->custom && topcard->custom(uic_DrawCard,nil))
		return;

	if (topcard->title)
	{
		// Draw the title
		MyLine(CtlPanelSY + 7);
		VWB_DrawPic(CtlPanelSX + 6,CtlPanelSY,topcard->title);
	}

	USL_DrawBottom();

	if (!topcard->items)
		return;

	x = topcard->x + CtlPanelSX;
	if (x % 8)
		x += 8 - (x % 8);
	y = topcard->y + CtlPanelSY + 8;

	for (item = topcard->items;item->type != uii_Bad;item++)
	{
		if (item->flags & ui_Separated)
			y += 8;

		item->x = x;
		item->y = y;
		USL_DrawItem(item);
		y += 8;
	}
	if (topcard->custom)
		topcard->custom(uic_TouchupCard,nil);
}

static void
USL_DrawCtlPanel(void)
{
	if (topcard->items || topcard->title)
	{
		// Draw the backdrop
		VWB_DrawFullPic(CK_CWRIST);

		// Draw the contents
		USL_DrawCtlPanelContents();
	}

}

static void
USL_DialogSetup(word w,word h,word *x,word *y)
{
	VWB_DrawMPic(CtlPanelSX,CtlPanelSY,CP_MENUMASKPICM);

	*x = CtlPanelSX + ((CtlPanelW - w) / 2);
	*y = CtlPanelSY + ((CtlPanelH - h) / 2);
	VWB_Bar(*x,*y,w + 1,h + 1, BackColor);
	VWB_Hlin2(*x - 6,*x + w - 5,*y - 8,NohiliteColor);
	VWB_Hlin2(*x - 6,*x + w - 5,*y + h - 7,NohiliteColor);
	VWB_Vlin2(*y - 8,*y + h - 7,*x - 6,NohiliteColor);
	VWB_Vlin2(*y - 8,*y + h - 7,*x + w - 5,NohiliteColor);
}

static void
USL_ShowLoadSave(char *s,char *name)
{
	word    x,y,
			w,h,
			tw,sw;
	char    msg[MaxGameName + 4];

	strcpy(msg,"'");
	strcat(msg,name);
	strcat(msg,"'");
	USL_MeasureString(s,&sw,&h);
	USL_MeasureString(msg,&w,&h);
	tw = ((sw > w)? sw : w) + 8;
	USL_DialogSetup(tw,(h * 2) + 10,&x,&y);
	PrintY = y ;
	PrintX = x + ((tw - sw) / 2);
	USL_DrawString(s);
	PrintY += h;
	PrintX = x + ((tw - w) / 2);
	USL_DrawString(msg);

#ifdef CAT3D
	IN_UserInput(100, true);
#endif
}

DONT_OPTIMISE static boolean
USL_CtlDialog(char *s1,char *s2,char *s3)
{
	word            w,h,sh,
				w1,w2,w3,
				x,y;
	ScanCode        c;
	CursorInfo      cursorinfo;

	USL_MeasureString(s1,&w1,&h);
	USL_MeasureString(s2,&w2,&h);
	if (s3)
		USL_MeasureString(s3,&w3,&h);
	else
		w3 = 0;
	w = (w1 > w2)? ((w1 > w3)? w1 : w3) : ((w2 > w3)? w2 : w3);
	w += 7;
	sh = h;
	h *= s3? 5 : 4;
	
	y += 8;
	USL_DialogSetup(w,h,&x,&y);
	y -= 8;

	fontcolor = HiliteColor;
	PrintX = x + ((w - w1) / 2);
	PrintY = y + sh + 1;
	USL_DrawString(s1);
	PrintY += (sh * 2) - 1;

	VWB_Hlin(x,x + w - 9,PrintY-3,NohiliteColor);
	PrintY += 2;

	fontcolor = NohiliteColor;
	PrintX = x + ((w - w2) / 2);
	USL_DrawString(s2);
	PrintY += sh;

	if (s3)
	{
		PrintX = x + ((w - w3) / 2);
		USL_DrawString(s3);
	}

	IN_ClearKeysDown();
	do
	{
		c = LastScan;
	} while (c == 0);

	IN_ClearKeysDown();
	USL_DrawCtlPanel();
	return (c == GBA_BUTTON_A);
}

static boolean
USL_ConfirmComm(UComm comm)
{
	boolean confirm,dialog;
	char    *s1,*s2,*s3;

	if (!comm)
		Quit("USL_ConfirmComm() - empty comm");

	confirm = true;
	dialog = false;
	s3 = "B TO BACK OUT";
	switch (comm)
	{
	case uc_Abort:
		s1 = "REALLY END CURRENT GAME?";
		s2 = "PRESS A TO END IT";
		if (ingame && GameIsDirty)
			dialog = true;
		break;
	case uc_Quit:
		s1 = "REALLY QUIT?";
		s2 = "PRESS A TO QUIT";
		dialog = true;
		break;
	case uc_Loaded:
		s1 = "YOU'RE IN A GAME";
		s2 = "PRESS A TO LOAD GAME";
		if (ingame && GameIsDirty)
			dialog = true;
		break;
	case uc_Saving:
		s1 = "OVERWRITE SAVE FILE?";
		s2 = "PRESS A TO SAVE GAME";
		if (ingame && GameIsDirty)
			dialog = true;
		break;
	case uc_SEasy:
	case uc_SNormal:
	case uc_SHard:
		s1 = "YOU'RE IN A GAME";
		s2 = "PRESS A FOR NEW GAME";
		if (ingame && GameIsDirty)
			dialog = true;
		break;
	// Added
	case uc_WipeRom:
		s1 = "THIS WILL REMOVE ALL";
		s2 = "SAVES AND CONFIGS";
		dialog = true;
		break;

	case uc_C_Rumble:
		s1 = "RUMBLE";
		enableRumble = !enableRumble;
		if(enableRumble){
			s2 = "ENABLED";
			optionsi[1].text = "RUMBLE (ON)";
		}else {
			s2 = "DISABLED";
			optionsi[1].text = "RUMBLE (OFF)";
		}
		s3 = "PRESS KEY";
		USL_CtlDialog(s1,s2,s3);
		dialog = false;
		return false;
		break;

	case uc_C_GodMode:
		s1 = "GOD MODE";
		godmode = !godmode;
		if(godmode){
			s2 = "ENABLED";
		}else {
			s2 = "DISABLED";
		}
		s3 = "PRESS KEY";
		USL_CtlDialog(s1,s2,s3);
		dialog = false;
		return false;
		break;
	case uc_C_JumpCheat:
		s1 = "JUMP CHEAT";
		jumpcheat = !jumpcheat;
		if(jumpcheat){
			s2 = "ENABLED";
		}else {
			s2 = "DISABLED";
		}
		s3 = "PRESS KEY";
		USL_CtlDialog(s1,s2,s3);
		dialog = false;
		return false;
		break;
	case uc_C_InfAmmo:
		s1 = "INFINITE AMMO";
		infiniteammo = !infiniteammo;
		if(infiniteammo){
			s2 = "ENABLED";
		}else {
			s2 = "DISABLED";
		}
		s3 = "PRESS KEY";
		USL_CtlDialog(s1,s2,s3);
		dialog = false;
		return false;
		break;
	case uc_C_InfLives:
		s1 = "INFINITE LIVES";
		infinitelives = !infinitelives;
		if(infinitelives){
			s2 = "ENABLED";
		}else {
			s2 = "DISABLED";
		}
		s3 = "PRESS KEY";
		USL_CtlDialog(s1,s2,s3);
		dialog = false;
		return false;
		break;
	case uc_C_AllItems:
		s1 = "ITEMS ADDED";
		gamestate.ammo = 99;
		gamestate.keys[0] = gamestate.keys[1] = 
			gamestate.keys[2] = gamestate.keys[3] = 99;
		gamestate.lives = 99;
		gamestate.score += 20000;
#ifdef KEEN4
		gamestate.wetsuit = true;
#elif defined CK5
		gamestate.keycard = true;
#elif defined CK6
		gamestate.sandwichstate = 1; 
		gamestate.hookstate = 1; 
		gamestate.passcardstate = 1; 
		gamestate.rocketstate = 1;
#endif
		s2 = "";
		s3 = "PRESS KEY";
		USL_CtlDialog(s1,s2,s3);
		dialog = false;
		return false;
		break;

	}

	confirm = dialog? USL_CtlDialog(s1,s2,s3) : true;
	if (confirm)
	{
		if(comm!=uc_Saving){
			Communication = comm;
			CtlPanelDone = true;
		}
	}
	return(confirm);
}

#define ENOMEM 1
#define EINVFMT 2
///////////////////////////////////////////////////////////////////////////
//
//      USL_HandleError() - Handles telling the user that there's been an error
//
///////////////////////////////////////////////////////////////////////////
static void
USL_HandleError(int num)
{
	char    buf[64];

	strcpy(buf,"Error: ");
	if (num < 0)
		strcat(buf,"Unknown");
	else if (num == ENOMEM)
		strcat(buf,"Disk is Full");
	else if (num == EINVFMT)
		strcat(buf,"Save file corrupt");
//		strcat(buf,"File is Incomplete");

	USL_CtlDialog(buf,"PRESS ANY KEY",nil);

	IN_ClearKeysDown();
	IN_Ack();
}

//      Custom routines
#if 0
static boolean
USL_GenericCustom(UserCall call,UserItem far *item)
{
	boolean result;

	result = false;
	switch (call)
	{
	}
	return(result);
}
#endif

static void
USL_SetOptionsText(void)
{
	optionsi[0].text = showscorebox? ((showscorebox==2)?"SCORE BOX (GBA)" : "SCORE BOX (ON)") : "SCORE BOX (OFF)";
	if(enableRumble){
		optionsi[1].text = "RUMBLE (ON)";
	}else{
		optionsi[1].text = "RUMBLE (OFF)";
	}
//	optionsi[1].text = oldshooting? "TWO-BUTTON FIRING (ON)" : "TWO-BUTTON FIRING (OFF)";
/*
#if GRMODE != CGAGR
	optionsi[2].text = jerk? "FIX JERKY MOTION (ON)" : "FIX JERKY MOTION (OFF)";
	optionsi[3].text = compatability? "SVGA COMPATIBILITY (ON)" : "SVGA COMPATIBILITY (OFF)";
#endif

	keybi[2].flags &= ~ui_Disabled;
	if (oldshooting)
		keybi[2].flags |= ui_Disabled;
	// gravis option is only enabled when a joystick is selected
	configi[6].flags |= ui_Disabled;
	if (Controls[0] == ctrl_Joystick1 || Controls[0] == ctrl_Joystick2)
		configi[6].flags &= ~ui_Disabled;

	configi[6].text = GravisGamepad? "USE GRAVIS GAMEPAD (ON)" : "USE GRAVIS GAMEPAD (OFF)";
*/}

const char * scoreboxdialogs[] = {
	"Score box off",
	"Score box on",
	"Score box gba"
};

#pragma argsused
static boolean
USL_ScoreCustom(UserCall call,UserItem *item)
{
	if (call != uic_SetupCard)
		return(false);

	showscorebox ++;
	showscorebox %= 3;
	scoreboxwiped = true; // Dirty scorebox sprite now!
	USL_CtlDialog(scoreboxdialogs[showscorebox], "Press any key",nil);

	// Fix the scorebox sprite
	if (!DemoMode)
	{
		if(scoreobj && scoreobj->sprite){
			switch(showscorebox){
				default:
				case CK_DISP_SCORE_DOS:
					CK_SetSpriteGfx(&scoreobj->sprite, CKS_SCOREBOXDOS);
					break;
				case CK_DISP_SCORE_GBA:
					CK_SetSpriteGfx(&scoreobj->sprite, CKS_SCOREBOXGBA);
					break;
			}
		}
	}
	
	USL_SetOptionsText();
	return(true);
}

#pragma argsused
static boolean
USL_SmoothCustom(UserCall call,UserItem *item)
{
	if (call != uic_SetupCard)
		return(false);
/*
	jerk ^= true;
	USL_CtlDialog(jerk? "Jerky motion fix enabled" : "Jerky motion fix disabled",
					"Press any key",nil);
					*/
	USL_SetOptionsText();
	return(true);
}

#pragma argsused
static boolean
USL_CompCustom(UserCall call,UserItem *item)
{
	if (call != uic_SetupCard)
		return(false);
/*
	compatability ^= true;
	USL_CtlDialog(compatability? "SVGA compatibility now on" : "SVGA compatibility now off",
					"Press any key",nil);
					*/
	USL_SetOptionsText();
	return(true);
}
/*
#ifdef  KEEN
#pragma argsused
static boolean
USL_TwoCustom(UserCall call,UserItem *item)
{
	if (call != uic_SetupCard)
		return(false);

	oldshooting ^= true;
	USL_CtlDialog(oldshooting? "Two-button firing now on" : "Two-button firing now off",
					"Press any key",nil);
	USL_SetOptionsText();
	return(true);
}
#endif
*/
static boolean
USL_ConfigCustom(UserCall call,UserItem *item)
{
static  char    *CtlNames[] = {"KEYBOARD","KEYBOARD","JOYSTICK #1","JOYSTICK #2","MOUSE"};
		char    *s;
		word    w,h,
				tw;

	if (call == uic_TouchupCard)
	{
		s = "CONTROL: ";
		USL_MeasureString(s,&w,&h);
		tw = w;
		/*
		USL_MeasureString(CtlNames[Controls[0]],&w,&h);
		tw += w;
		py = CtlPanelEY - 18 - h;
		px = CtlPanelSX + ((CtlPanelW - tw) / 2);
		fontcolor = NohiliteColor;
		USL_DrawString(s);
		USL_DrawString(CtlNames[Controls[0]]);
		*/
	}
	item++; // Shut the compiler up
	return(false);
}

static void
USL_CKSetKey(UserItem *item,word i)
{
	boolean         on;
	word            j;
	ScanCode        scan;
	longword        time;
	CursorInfo      cursorinfo;
/*
	on = false;
	time = 0;
	LastScan = 0;
	fontcolor = HiliteColor;
	do
	{
		if (TimeCount >= time)
		{
			on ^= true;
			VWB_Bar(item->x + 90,item->y,40,8,fontcolor ^ BackColor);
			VWB_Bar(item->x + 90 + 1,item->y + 1,40 - 2,8 - 2,BackColor);
			if (on)
				VWB_DrawTile8(item->x + 90 + 16,item->y,TileBase + 8);

			time = TimeCount + (TickBase / 2);
		}

		IN_ReadCursor(&cursorinfo);
		while (cursorinfo.button0 || cursorinfo.button1)
		{
			IN_ReadCursor(&cursorinfo);
			LastScan = sc_Escape;
		}

	asm     pushf
	asm     cli
		if (LastScan == sc_LShift)
			LastScan = sc_None;
	asm     popf
	} while (!(scan = LastScan));

	if (scan != sc_Escape)
	{
		for (j = 0,on = false;j < 11;j++)
		{
			if (j == i)
				continue;
			if (*(KeyMaps[j]) == scan)
			{
				on = true;
				break;
			}
		}
		if (on)
#ifdef KEEN
			USL_CtlDialog("Key already used","Press any key",nil);
#else
			USL_CtlDialog("Key already used","Press a key",nil);
#endif
		else
			*(KeyMaps[i]) = scan;
	}*/
	IN_ClearKeysDown();
}

#pragma argsused
static boolean
USL_KeySCustom(UserCall call,UserItem *item)
{/*
	if (call == uic_SetupCard)
	{
		Controls[0] = ctrl_Keyboard;
		GravisGamepad = false;
		USL_SetOptionsText();
	}*/
	return(false);
}

#pragma argsused
static boolean
USL_KeyCustom(UserCall call,UserItem *item)
{
	boolean result = false;
	word    i;
/*
	i = (topcard == &keygroup)? (3 + (item - keyi)) : (item - keybi);
	switch (call)
	{
	case uic_SetupCard:
		Controls[0] = ctrl_Keyboard;
		break;
	case uic_Draw:
		VWB_Bar(CtlPanelSX + 1,item->y,
				CtlPanelEX - CtlPanelSX - 1,8,BackColor);       // Clear out background
		USL_DrawItemIcon(item);
		fontcolor = (item->flags & ui_Selected)? HiliteColor : NohiliteColor;
		px = item->x + 8;
		py = item->y + 1;
		USL_DrawString(item->text);
		VWB_Bar(item->x + 90,item->y,40,8,fontcolor ^ BackColor);
		VWB_Bar(item->x + 90 + 1,item->y + 1,40 - 2,8 - 2,BackColor);
		px = item->x + 90 + 6;
		py = item->y + 1;
		USL_DrawString(IN_GetScanName(*KeyMaps[i]));
		result = true;
		break;
	case uic_Hit:
		USL_KeyCustom(uic_Draw,item);
		USL_CKSetKey(item,i);
		USL_DrawCtlPanel();
		result = true;
		break;
	}*/
	return(result);
}

static void
USL_CJDraw(char *s1,char *s2)
{
	word    w,h;
/*
	USL_MeasureString(s1,&w,&h);
	px = CtlPanelSX + ((CtlPanelW - w) / 2);
	py = CtlPanelEY - 34;
	VWB_Bar(CtlPanelSX + 1,py,CtlPanelW - 2,h * 2,BackColor);
	fontcolor = HiliteColor;
	USL_DrawString(s1);
	py += h;
	USL_MeasureString(s2,&w,&h);
	px = CtlPanelSX + ((CtlPanelW - w) / 2);
	USL_DrawString(s2);*/
}

static boolean
USL_CJGet(word joy,word button,word x,word y,word *xaxis,word *yaxis)
{
	boolean         on;
	longword        time;
/*
	while (IN_GetJoyButtonsDB(joy))
		if (LastScan == sc_Escape)
			return(false);

	on = false;
	time = 0;
	while (!(IN_GetJoyButtonsDB(joy) & (1 << button)))
	{
		if (TimeCount >= time)
		{
			on ^= true;
			time = TimeCount + (TickBase / 2);
			VWB_DrawTile8(x,y,TileBase + on);
		}

		if (LastScan == sc_Escape)
			return(false);
	}
	IN_GetJoyAbs(joy,xaxis,yaxis);
	return(true);*/
	return false;
}

static boolean
USL_ConfigJoystick(word joy)
{
	word    x,y,
			minx,miny,
			maxx,maxy;
/*
	BottomS1 = BottomS2 = "";
#ifdef KEEN
	BottomS3 = "ESC to back out";
#else
	BottomS3 = "Esc to back out";
#endif
	USL_DrawCtlPanel();
	x = CtlPanelSX + 60;
	y = CtlPanelSY + 19;
	VWB_DrawPic(x,y,CP_JOYSTICKPIC);

	USL_CJDraw("Move Joystick to upper left","and press button #1");
	VWB_DrawTile8(x + 24,y + 8,TileBase + 6);
	VWB_DrawTile8(x + 8,y + 8,TileBase + 1);
	VWB_DrawTile8(x + 8,y + 24,TileBase + 0);

	if (!USL_CJGet(joy,0,x + 8,y + 8,&minx,&miny))
		return(false);

	USL_CJDraw("Move Joystick to lower right","and press button #2");
	VWB_DrawTile8(x + 24,y + 8,TileBase - 25);
	VWB_DrawTile8(x + 40,y + 24,TileBase + 7);
	VWB_DrawTile8(x + 8,y + 8,TileBase + 0);
	VWB_DrawTile8(x + 8,y + 24,TileBase + 1);

	if (!USL_CJGet(joy,1,x + 8,y + 24,&maxx,&maxy))
		return(false);

	while (IN_GetJoyButtonsDB(joy))
		;

#ifdef KEEN
	if (minx != maxx && miny != maxy)
	{
		IN_SetupJoy(joy,minx,maxx,miny,maxy);
		return(true);
	}
	return(false);
#else
	IN_SetupJoy(joy,minx,maxx,miny,maxy);
	return(true);
#endif*/
	return false;
}

#pragma argsused
static boolean
USL_Joy1Custom(UserCall call,UserItem *item)
{/*
	if (call == uic_SetupCard)
	{
		if (USL_ConfigJoystick(0))
		{
			Controls[0] = ctrl_Joystick1;
			USL_CtlDialog("USING JOYSTICK #1","PRESS ANY KEY",nil);
			USL_SetOptionsText();
		}
		return(true);
	}
	else*/
		return(false);
}

#pragma argsused
static boolean
USL_Joy2Custom(UserCall call,UserItem *item)
{/*
	if (call == uic_SetupCard)
	{
		if (USL_ConfigJoystick(1))
		{
			Controls[0] = ctrl_Joystick2;
			USL_CtlDialog("USING JOYSTICK #2","PRESS ANY KEY",nil);
			USL_SetOptionsText();
		}
		return(true);
	}
	else*/
		return(false);
}

static void
USL_CGDraw(char *s1, char *s2, int buttonsDone)
{/*
	static char *GButtonNames[4] = {"Red","Blue","Yellow","Green"};
	static char *GActionNames[4] = {"Jump","Pogo","Fire","Status"};

	int	i, n;
	char	*actionstr;
	word	w, h;

	VWB_Bar(CtlPanelSX+1, CtlPanelSY+16, CtlPanelW-2, 68, BackColor);
	px = CtlPanelSX+8;
	py = CtlPanelSY+16;
	USL_DrawString("Make sure that the button");
	px = CtlPanelSX+8;
	py = CtlPanelSY+24;
	USL_DrawString("switch is set for 4 buttons");

	for (i=0; i<4; i++)
	{
		px = CtlPanelSX+8;
		py = i*8 + CtlPanelSY+40;
		USL_DrawString(GButtonNames[i]);
		USL_DrawString(":");
		actionstr = "?";
		for (n=0;n<buttonsDone;n++)
		{
			if (GravisMap[n] == i)
				actionstr = GActionNames[n];
		}
		px = CtlPanelSX+56;
		USL_DrawString(actionstr);
	}

	USL_MeasureString(s1, &w, &h);
	px = w;
	USL_MeasureString(s2, &w, &h);
	px = (CtlPanelW-px-w)/2 + CtlPanelSX;
	py = CtlPanelSY+76;
	USL_DrawString(s1);
	USL_DrawString(s2);
	*/
}

static boolean
USL_CGGet(int buttonsDone, char *action)
{/*
	word	buttons, i, n;

	USL_CGDraw("PRESS BUTTON FOR ", action, buttonsDone);

redo:
	do
	{
		if (LastScan == sc_Escape)
		{
			Keyboard[sc_Escape] = false;
			if (LastScan == sc_Escape)
				LastScan = sc_None;
			return false;
		}

		buttons = IN_GetJoyButtonsDB(2);
		if (!buttons)
			continue;

		for(i=n=0; i<4; i++)
		{
			if (buttons & (1 << i))
				n += i+1;
		}

		if (!n || n >= 5)
			continue;

		n--;
		for (i=0; i<buttonsDone; i++)
		{
			if (GravisMap[i] == n)
				goto redo;
		}

		GravisMap[buttonsDone] = n;
		return true;
	} while (true);*/
	return false;
}

#pragma argsused
static boolean
USL_JoyGCustom(UserCall call,UserItem *item)
{/*
	if (call == uic_SetupCard)
	{
		if (GravisGamepad)
		{
			GravisGamepad = false;
		}
		else
		{
			BottomS1 = BottomS2 = "";
			BottomS3 = "ESC to back out";
			USL_DrawCtlPanel();
			fontcolor = HiliteColor;
			px = CtlPanelSX + 8;
			py = CtlPanelSX + 8;
			fontcolor = HiliteColor;	// redundant...
			IN_ClearKeysDown();
			if (  USL_CGGet(0, "JUMP")
				&& USL_CGGet(1, "POGO")
				&& USL_CGGet(2, "FIRE")
				&& USL_CGGet(3, "STATUS") )
			{
				GravisGamepad = true;
				USL_CGDraw("PRESS ANY KEY", "", 4);
				IN_Ack();
			}
			else
			{
				GravisGamepad = false;
			}
		}
		USL_SetOptionsText();
		return true;
	}*/
	return false;
}

static void
USL_DrawFileIcon(UserItem *item)
{
	word    color;

	item->y = topcard->y + CtlPanelSY + 16;
	item->y += (item - loadsavegamei) * 16;
	// HACK:
	// For colour blind people
	fontcolor = NohiliteColor;
	if(item->flags & ui_Selected){
		fontcolor = HiliteColor;
		if(CK_PaletteSet >= 3) {
			fontcolor += 1;
		}
	}
	color = fontcolor; // (fontcolor ^ BackColor);  // Blech!
	VWB_Hlin(item->x,item->x + (CtlPanelW - 12),item->y,color);
	VWB_Hlin(item->x,item->x + (CtlPanelW - 12),item->y + 9,color);
	VWB_Vlin(item->y,item->y + 9,item->x,color);
	VWB_Vlin(item->y,item->y + 9,item->x + (CtlPanelW - 12),color);
}

extern SaveGame        Games[MaxSaveGames];
extern const unsigned short ck_file_slots[];


static void
USL_DoLoadGame(UserItem *item)
{
	word            n,
				err;
	int                     file;
	SaveGame        *game;

	if (!USL_ConfirmComm(uc_Loaded))
		return;

	n = item - loadsavegamei;
	game = &Games[n];

	USL_ShowLoadSave("Loading",game->name);

	err = 0;
	if ((file = openHandle(ck_file_slots[n], FileIO_Read, CKFB_SLOT_S)) != -1)
	{
		if (readHandle(&file,game,sizeof(*game)) == sizeof(*game))
		{
			if (!LoadTheGame(file))
				USL_HandleError(err = EINVFMT);
		}
		else
			USL_HandleError(err = EINVFMT);
	}
	else
		USL_HandleError(err = 3);
	if (err)
	{
		abortgame = true;
		Communication = uc_None;
		CtlPanelDone = false;
	}
	else
		loadedgame = true;
	game->present = true;

	if (loadedgame)
		Paused = true;


	USL_DrawCtlPanel();

}

static boolean
USL_LoadCustom(UserCall call,UserItem *item)
{
	boolean result;
	word    i;
	
	result = false;
	switch (call)
	{
	case uic_SetupCard:
		USL_CheckSavedGames();
		for (i = 0;i < MaxSaveGames;i++)
		{
			if (Games[i].present)
				loadsavegamei[i].flags &= ~ui_Disabled;
			else
				loadsavegamei[i].flags |= ui_Disabled;
		}
		break;
	case uic_DrawIcon:
		USL_DrawFileIcon(item);
		result = true;
		break;
	case uic_Draw:
		USL_DrawFileIcon(item);
		VWB_Bar(item->x + 1,item->y + 2,CtlPanelW - 12 - 2,7,BackColor);

		i = item - loadsavegamei;
		if (Games[i].present)
			PrintX = item->x + 24;
		else
			PrintX = item->x + 60;
		PrintY = item->y+4;
		USL_DrawString(Games[i].present? Games[i].name : "Empty");
		result = true;
		break;
	case uic_Hit:
		USL_DoLoadGame(item);
		result = true;
		break;
	}
	return(result);
}

static void
USL_DoSaveGame(UserItem *item)
{
	boolean         ok;
	char            *filename;
	word            n,err;
	FileHandle       file;
	SaveGame        *game;

	USL_DrawCtlPanel();

	n = item - loadsavegamei;
	game = &Games[n];
	fontcolor = HiliteColor;
	VWB_Bar(item->x + 1,item->y + 2,CtlPanelW - 12 - 2,7,BackColor);
/*	game->oldtest = &PrintX;
	ok = US_LineInput(item->x + 2,item->y + 2,
						game->name,game->present? game->name : nil,
						true,MaxGameName,
						CtlPanelW - 22);
	if (!strlen(game->name))*/
	ok = USL_ConfirmComm(uc_Saving);

	if (ok)
	{
		_ck_strcpy(game->name,"SaveGame ");
		game->name[9] = '1'+n;
		game->name[10] = 0;

		USL_ShowLoadSave("Saving",game->name);

		err = 0;
		if ((file = openHandle(ck_file_slots[n], FileIO_Write, CKFB_SLOT_S)) != -1)
		{
			if (writeHandle(&file,game,sizeof(*game)) == sizeof(*game))
			{
				ok = SaveTheGame(file);
				if (!ok)
					USL_HandleError(err);
			}
			else
				USL_HandleError(err = ENOMEM);
		}
		else
			USL_HandleError(err = ENOMEM );
		if (err)
		{
			ok = false;
		}

	}

	if (!game->present)
		game->present = ok;

	if (ok)
		GameIsDirty = false;
	USL_SetupCard();
}

static boolean
USL_SaveCustom(UserCall call,UserItem *item)
{
	word    i;

	switch (call)
	{
	case uic_SetupCard:
		USL_CheckSavedGames();
		for (i = 0;i < MaxSaveGames;i++)
			loadsavegamei[i].flags &= ~ui_Disabled;
		return(false);
	case uic_Hit:
		USL_DoSaveGame(item);
		return(true);
//              break;
	}
	return(USL_LoadCustom(call,item));
}

#define PaddleMinX      (CtlPanelSX + 4)
#define PaddleMaxX      (CtlPanelEX - 15)
#define BallMinX        (CtlPanelSX + 4)
#define BallMinY        (CtlPanelSY + 12 + 2)
#define BallMaxX        (CtlPanelEX - 6)
#define BallMaxY        (CtlPanelEY - 13)
#define CPaddleY        (BallMinY + 4)
#define KPaddleY        (BallMaxY - 2)
void
USL_DrawPongScore(word k,word c)
{
	fontcolor = HiliteColor;
	PrintY = CtlPanelSY + 4;
	PrintX = CtlPanelSX + 4;
	VWB_Bar(PrintX,PrintY,42,6,BackColor);
	USL_DrawString("KEEN:");
	PrintX += 5<<3;
	US_PrintUnsigned(k);
	PrintX = CtlPanelSX + 108;
	VWB_Bar(PrintX,PrintY,50,6,BackColor);
	USL_DrawString("COMP:");
	PrintX += 5<<3;
	US_PrintUnsigned(c);
}

DONT_OPTIMISE void
USL_PlayPong(void)
{
	boolean         ball,killball,revdir,done,lastscore;
	word            cycle,
				x,y,
				kx,cx,
				rx,
				bx,by,
				oldkx,oldcx,oldbx,oldby,
				kscore,cscore,
				speedup;
	int                     bdx,bdy;
	longword        balltime,lasttime,waittime;
	CursorInfo      cursorinfo;

	kx = cx = PaddleMinX + ((PaddleMaxX - PaddleMinX) / 2);
	bx = by = bdx = bdy = 0;
	oldbx = oldcx = oldkx = PaddleMinX;
	oldby = BallMinY;
	kscore = cscore = 0;
	USL_DrawPongScore(0,0);
	cycle = 0;
	revdir = false;
	killball = true;
	done = false;
	lastscore = false;
	lasttime = TimeCount;

	// Added:
	objsprite *ballspr, *kpaddlespr, *cpaddlespr;

	// Bad! Do not run!
	// CK_ResetSprGraphicsOffset();
	ballspr = CK_GetNewSprite(CKS_BALL);
	kpaddlespr = CK_GetNewSprite(CKS_PADDLE);
	cpaddlespr = CK_GetNewSprite(CKS_PADDLE);

	CK_SetSprite(&ballspr, CKS_BALL);
	CK_SetSprite(&kpaddlespr, CKS_PADDLE);
	CK_SetSprite(&cpaddlespr, CKS_PADDLE);

	RF_StoreOrg();
	originxglobal = originyglobal = 0;

	do
	{

		while ((waittime = TimeCount - lasttime) == 0)
			;

		lasttime = TimeCount;
		if (waittime > 4)
			waittime = 4;

		while (waittime-- && !done && LastScan != GBA_BUTTON_B)
		{
			if (IN_KeyDown(GBA_BUTTON_LEFT) && (kx > PaddleMinX))
				kx -= 2;
			else if (IN_KeyDown(GBA_BUTTON_RIGHT) && (kx < PaddleMaxX))
				kx += 2;

			if (killball)
			{
				ball = false;
				balltime = TickBase;
				speedup = 10;
				killball = false;
				//VWB_Bar(oldbx,oldby,5,5,BackColor);
				RF_DrawSprite(&ballspr, -16, -16,BALLSPR);
			}

			if (ball && (cycle++ % 3))
			{
				x = (bx >> 2);
				if (!(x & 1))
					x += (US_RndT() & 1);

				if ((cx + 6 < x) && (cx < PaddleMaxX))
					cx += 1;
				else if ((cx + 6 > x) && (cx > PaddleMinX))
					cx -= 1;
			}

			if (!ball && !--balltime)
			{
				ball = true;
				bdx = 1 - (US_RndT() % 3);
				bdy = 3;
				if (lastscore)
					bdy = -bdy;
				bx = (BallMinX + ((BallMaxX - BallMinX) / 2)) << 2;
				by = (BallMinY + ((BallMaxY - BallMinY) / 2)) << 2;
			}

			if (ball)
			{
				if
				(
					(((bx + bdx) >> 2) > BallMaxX)
				||      (((bx + bdx) >> 2) < BallMinX)
				)
				{
					SD_PlaySound(BALLBOUNCESND);
					bdx = -bdx;
				}
				bx += bdx;

				if (((by + bdy) >> 2) > BallMaxY)
				{
					killball = true;
					lastscore = false;
					cscore++;
					SD_PlaySound(COMPSCOREDSND);
					USL_DrawPongScore(kscore,cscore);
					if (cscore == 21)
					{
						USL_CtlDialog("You lost!","Press any key",nil);
						done = true;
						continue;
					}
				}
				else if (((by + bdy) >> 2) < BallMinY)
				{
					killball = true;
					lastscore = true;
					kscore++;
					SD_PlaySound(KEENSCOREDSND);
					USL_DrawPongScore(kscore,cscore);
					if (kscore == 21)
					{
						USL_CtlDialog("You won!","Press any key",nil);
						done = true;
						continue;
					}
				}
				by += bdy;

				x = bx >> 2;
				y = by >> 2;
				if (!killball)
				{
					if
					(
						(bdy < 0)
					&&      ((y >= CPaddleY) && (y < CPaddleY + 3))
					&&      ((x >= (cx - 5)) && (x < (cx + 11)))
					)
					{
						rx = cx;
						revdir = true;
						SD_PlaySound(COMPPADDLESND);
					}
					else if
					(
						(bdy > 0)
					&&      ((y >= (KPaddleY - 3)) && (y < KPaddleY))
					&&      ((x >= (kx - 5)) && (x < (kx + 11)))
					)
					{
						if (((bdy >> 2) < 3) && !(--speedup))
						{
							bdy++;
							speedup = 10;
						}
						rx = kx;
						revdir = true;
						SD_PlaySound(KEENPADDLESND);
					}
					if (revdir)
					{
						bdy = -bdy;
						bdx = ((x + 5 - rx) >> 1) - (1 << 2);
						if (!bdx)
							bdx--;
						revdir = false;
					}
				}
			}
			CK_UpdateSprites();
		}

		if (ball)
		{
			//VWB_Bar(oldbx,oldby,5,5,BackColor);
			oldbx = x;
			oldby = y;
			RF_DrawSprite(&ballspr, x-8,y,(x & 1)? BALL1PIXELTOTHERIGHTSPR : BALLSPR);
		}
		//VWB_Bar(oldcx-3,CPaddleY,16,3,BackColor);
		oldcx = cx;
		RF_DrawSprite(&kpaddlespr,cx-8,CPaddleY,PADDLESPR);
		//VWB_Bar(oldkx-3,KPaddleY,16,3,BackColor);
		oldkx = kx;
		RF_DrawSprite(&cpaddlespr,kx-8,KPaddleY,PADDLESPR);

		CK_UpdateSprites();

	} while ((LastScan != GBA_BUTTON_B) && !done);

	RF_RestoreOrg();

	IN_ClearKeysDown();
    // Clear any software sprites
	RF_RemoveSprite(&ballspr, false);
	RF_RemoveSprite(&kpaddlespr, false);
	RF_RemoveSprite(&cpaddlespr, false);
    GBA_ResetSprites();
}

#pragma argsused
static boolean
USL_PongCustom(UserCall call,struct UserItem *item)
{
	if (call != uic_SetupCard)
		return(false);

	VWB_DrawFullPic(CK_CWRIST);
	VWB_DrawPic(CtlPanelSX + 56,CtlPanelSY,CP_PADDLEWARPIC);
	VWB_Hlin(CtlPanelSX + 3,CtlPanelEX - 3,CtlPanelSY + 12,(HiliteColor ^ BackColor)+1);
	VWB_Hlin(CtlPanelSX + 3,CtlPanelEX - 3,CtlPanelEY - 7,(HiliteColor ^ BackColor)+1);

	USL_PlayPong();

	return(true);
}

static boolean
USL_ResetCustom(UserCall call,struct UserItem *item)
{
	if (!USL_ConfirmComm(uc_WipeRom))
		return (true);

	WipeSRam();

	CtlPanelDone = false; // Fix the flag???

    // Wait for 3 seconds
#ifdef __EZ_FLASH
    GBA_Delay(3000);
#endif 

	return(true);
}

//      Flag management stuff
static void
USL_ClearFlags(UserItemGroup *node)
{
	UserItem        *i;

	if (!node->items)
		return;

	for (i = node->items;i->type != uii_Bad;i++)
	{
		i->flags &= ~UISelectFlags;
		if (i->child)
			USL_ClearFlags((UserItemGroup *)i->child);
	}
}

static int
USL_FindPushedItem(UserItemGroup *group)
{
	word            i;
	UserItem        *item;

	for (item = group->items,i = 0;item->type != uii_Bad;item++,i++)
		if (item->flags & ui_Pushed)
			return(i);
	return(-1);
}

static void
USL_SelectItem(UserItemGroup *group,word index,boolean draw)
{
	UserItem        *item;

	if (index != group->cursor)
	{
		item = &group->items[group->cursor];
		item->flags &= ~ui_Selected;
		if (draw)
			USL_DrawItem(item);
	}

	group->cursor = index;
	item = &group->items[group->cursor];
	group->items[group->cursor].flags |= ui_Selected;
	if (draw)
		USL_DrawItem(item);
}

static void
USL_PushItem(UserItemGroup *group,word index,boolean draw)
{
	word            i;
	UserItem        *item;

	USL_SelectItem(group,index,draw);
	for (item = group->items,i = 0;item->type != uii_Bad;item++,i++)
	{
		if (item->type != uii_RadioButton)
			continue;

		if (i == index)
		{
			item->flags |= ui_Pushed;
			if (draw)
				USL_DrawItem(item);
		}
		else if (item->flags & ui_Pushed)
		{
			item->flags &= ~ui_Pushed;
			if (draw)
				USL_DrawItem(item);
		}
	}
}

static void
USL_NextItem(void)
{
	if (topcard->items[topcard->cursor + 1].type == uii_Bad)
		return;
	USL_SelectItem(topcard,topcard->cursor + 1,true);
}

static void
USL_PrevItem(void)
{
	if (!topcard->cursor)
		return;
	USL_SelectItem(topcard,topcard->cursor - 1,true);
}

static void
USL_SetupCard(void)
{
	BottomS1 = "D-PAD move";
	BottomS2 = cstackptr? "B back" : "B quit";
	BottomS3 = "A selects";

	USL_SelectItem(topcard,topcard->cursor,false);
	USL_DrawCtlPanel();     // Contents?
}

static void
USL_DownLevel(UserItemGroup *group)
{
	if (!group)
		Quit("USL_DownLevel() - nil card");
	USL_PushCard(group);
	if (group->custom && group->custom(uic_SetupCard,nil))
		USL_PopCard();
	USL_SetupCard();
}

static void
USL_UpLevel(void)
{
#ifdef CK_INTR_ENDER
	if (!cstackptr)
	{
		USL_ConfirmComm(uc_Quit);
		return;
	}
#endif

	if (topcard->items)
		topcard->items[topcard->cursor].flags &= ~ui_Selected;
	USL_PopCard();
	USL_SetupCard();
}

static void
USL_DoItem(void)
{
	// DEBUG - finish this routine
	UserItem                *item;

	item = &topcard->items[topcard->cursor];
	if (item->flags & ui_Disabled){
		SD_PlaySound(NOWAYSND);
		CK_TimedRumble(15); // Rumble for 15 ticks
	}
	else
	{
		switch (item->type)
		{
		case uii_Button:
			if (!(topcard->custom && topcard->custom(uic_Hit,item)))
				USL_ConfirmComm(item->comm);
			break;
		case uii_RadioButton:
			USL_PushItem(topcard,topcard->cursor,true);
			break;
		case uii_Folder:
			USL_DownLevel(item->child);
			break;
		}
	}
}

static void
USL_SetControlValues(void)
{
	int sndindex;

	sndindex = SoundMode;
//	if (sndindex == sdm_AdLib && QuietFX)
//		sndindex++;

	USL_PushItem(&soundgroup,sndindex,false);
	USL_PushItem(&musicgroup,MusicMode,false);

	USL_PushItem(&videogroup,CK_PaletteSet,false);
/*	if (!AdLibPresent)
	{
		soundi[2].flags |= ui_Disabled; // AdLib sound effects
		soundi[3].flags |= ui_Disabled; // Quiet AdLib sound effects
		musici[1].flags |= ui_Disabled; // AdLib music
	}
*/
/*#ifdef CAT3D
	if (!JoysPresent[0])
		configi[3].flags |= ui_Disabled;
	if (!JoysPresent[1])
		configi[4].flags |= ui_Disabled;
#else
	if (!JoysPresent[0])
		configi[4].flags |= ui_Disabled;
	if (!JoysPresent[1])
		configi[5].flags |= ui_Disabled;
	if (!JoysPresent[0] && !JoysPresent[1])
		configi[6].flags |= ui_Disabled;
#endif
*/
/*	configi[4].flags |= ui_Disabled;
	configi[5].flags |= ui_Disabled;
	configi[6].flags |= ui_Disabled;
*/
	rooti[4].text = ingame? "RETURN TO GAME" : "RETURN TO DEMO";
	if (!ingame)
	{
		rooti[2].flags |= ui_Disabled;  // Save Game
		rooti[5].flags |= ui_Disabled;  // End Game
	}
#ifndef NOCHEATS
	// Add in the cheat menu if needed 
	if(debugok)
		rooti[7] = rootcheati;
#endif

	rootgroup.cursor = ingame? 4 : 0;
	USL_SetOptionsText();
	// DEBUG - write the rest of this
}

static boolean
USL_VideoCustom(UserCall call,struct UserItem *item)
{
	int     i = 0;
	// DEBUG - write the rest of this
	i = USL_FindPushedItem(&videogroup);

	if(CK_PaletteSet != i){
		CK_PaletteSet = i;
		CK_FixPaletteU();
	}

	return(false);
}

static boolean
USL_CheatCustom(UserCall call,struct UserItem *item)
{
	int     i = 0;
	// DEBUG - write the rest of this
	i = USL_FindPushedItem(&cheatgroup);

	return(false);
}


void US_EnableCheatMenu(){
	cheatsEnabled = true;
};

///////////////////////////////////////////////////////////////////////////
//
//      USL_SetUpCtlPanel() - Sets the states of the UserItems to reflect the
//              values of all the appropriate variables
//
///////////////////////////////////////////////////////////////////////////
static void
USL_SetUpCtlPanel(void)
{
	int     i;
	// Cache in all of the stuff for the control panel
	CA_CacheMarks("Control Panel");
	// Move this here?
	CA_FixGraphics();
	CK_FixPaletteU();

	// Do some other setup
	fontcolor = F_BLACK;
#ifdef CAT3D
	VWB_Bar (0,0,320,200,3); // CAT3D patch
#else
//	VW_ClearVideo(3);
#endif
	RF_SetOffs(8,0);

	Communication = uc_None;
	USL_ClearFlags(&rootgroup);
	USL_SetControlValues();
	USL_SetupStack();
	USL_SetupCard();

	if (ingame)
		GameIsDirty = true;

	IN_ClearKeysDown();
}

static void
USL_HandleComm(UComm comm)
{
	switch (comm)
	{
	case uc_Loaded:
	case uc_Saving:
	case uc_C_Rumble:
	case uc_Return:
	case uc_WipeRom:
	case uc_C_GodMode:
	case uc_C_JumpCheat:
	case uc_C_InfAmmo:
	case uc_C_InfLives:
	case uc_C_AllItems:
		break;
	case uc_Abort:
		abortgame = true;
		break;
	case uc_Quit:
#ifdef CK_INTR_ENDER
		QuitToDos = true;
#endif
		break;
	case uc_SEasy:
		restartgame = gd_Easy;
		break;
	case uc_SNormal:
		restartgame = gd_Normal;
		break;
	case uc_SHard:
		restartgame = gd_Hard;
		break;

	default:
		Quit("USL_HandleComm() - unknown");
		break;
	}
}

static void
USL_GetControlValues(void)
{
	int     i;

	// DEBUG - write the rest of this
	i = USL_FindPushedItem(&soundgroup);
	if (i == 3)
	{
//		QuietFX = true;
		i--;
	}
	else
	{
//		QuietFX = false;
	}
	if (i != SoundMode)
		SD_SetSoundMode(i);

	i = USL_FindPushedItem(&musicgroup);
	if (i != MusicMode)
		SD_SetMusicMode(i);
}

///////////////////////////////////////////////////////////////////////////
//
//      USL_TearDownCtlPanel() - Given the state of the control panel, sets the
//              modes and values as appropriate
//
///////////////////////////////////////////////////////////////////////////
static void
USL_TearDownCtlPanel(void)
{
	USL_GetControlValues();
	if (Communication)
		USL_HandleComm(Communication);

	fontcolor = F_BLACK;
	if (restartgame)
		ResetGame();
#ifdef CK_INTR_ENDER
	else if (QuitToDos)
	{
		US_CenterWindow(20,3);
		fontcolor = F_SECONDCOLOR;
		US_PrintCentered("Quitting...");
		fontcolor = F_BLACK;
		GBA_Delay(500);
		Quit(nil);
	}
#endif
	IN_ClearKeysDown();
	SD_WaitSoundDone();
#ifdef CAT3D
	VWB_Bar (0,0,320,200,3); // CAT3D patch
#else
	VW_ClearVideo( CK_TXTCOL(CK_EGA_BLACK)); // BUG ? Should be CK_EGA_CYAN
#endif
	CA_FixGraphics();
	CK_FixPalette();

	// Write the config file now?
	US_Shutdown();
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
extern void HelpScreens(void);

	boolean         resetitem,on;
	word            i;
	int                     ydelta;
	longword        flashtime;
	UserItem         *item;
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

	if (LastScan == GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER)
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

		if (LastScan)
		{
			switch (LastScan)
			{
			case GBA_BUTTON_UP:
				USL_PrevItem();
				resetitem = true;
				break;
			case GBA_BUTTON_DOWN:
				USL_NextItem();
				resetitem = true;
				break;
			case GBA_BUTTON_A:
				USL_DoItem();
				resetitem = true;
				break;
			case GBA_BUTTON_B:
				USL_UpLevel();
				resetitem = true;
				break;
#ifndef KEEN6
			case (GBA_BUTTON_LSHOLDER | GBA_BUTTON_RSHOLDER):
				HelpScreens();
				USL_DrawCtlPanel();
				resetitem = true;
				break;
#endif
			}

			if
			(
				(!resetitem) && ( (LastScan == GBA_BUTTON_A) || 
								  (LastScan == GBA_BUTTON_B))
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

		/*
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
		}*/
	}

	USL_TearDownCtlPanel();
}

#ifdef KEEN6
typedef struct {
	char *name;
	int shapenum;
	int x, y;
} creatureinfo;

const static creatureinfo creature_list[] = {
	{"BIP",       BIPSHIPRSPR,        -2,  0},
	{"BABOBBA",   BABOBBAR1SPR,        0,  0},
	{"BLORB",     BLORB1SPR,          -2,  0},
	{"GIK",       GIKWALKR1SPR,       -1,  0},
	{"CEILICK",   CEILICK1SPR,         0,  0},
	{"BLOOGLET",  RBLOOGLETWALKR1SPR, -2,  0},
	{"BLOOGUARD", BLOOGUARDWALKL1SPR, -3, -1},
	{"FLECT",     FLECTSTANDSPR,      -1,  0},
	{"BOBBA",     BOBBAR1SPR,         -2,  0},
	{"NOSPIKE",   NOSPIKESTANDSPR,    -2,  0},
	{"ORBATRIX",  ORBATRIXR1SPR,      -2,  1},
	{"FLEEX",     FLEEXWALKR1SPR,     -2,  0}
};

boolean US_ManualCheck(void)
{

	return true;
	boolean correct;
	char *name;
	char c;
	char *ptr;
	unsigned spriteheight, spritewidth;
	int x, y;
	int editwidth;
	creatureinfo info;
	char strbuf[16];

	if (checkpassed)
		return true;

	correct = false;
	if (listindex == -1) {
		/*
		_AH = 0x2C;	// get time
		geninterrupt(0x21);
		x = _CH;	// store hours
		_AH = 0x2A;	// get date
		geninterrupt(0x21);
		y = _DL;	// store day

		listindex = (x + y) % (int)(sizeof(creature_list)/sizeof(creatureinfo));
		*/
		// TODO:
		listindex = 0;
	}

	info = creature_list[listindex];
	name = info.name;
//	CA_ClearMarks();
//	CA_MarkGrChunk(info.shapenum);
//	CA_CacheMarks(NULL);

	VWB_Bar(0, 0, 320, 200, BackColor);
	US_CenterWindow(30, 16);
	PrintY = WindowY + 2;
	US_CPrint("What is the name of this creature?");

	x = WindowX + (WindowW-spritewidth)/2 + info.x*8;
	y = WindowY + 15;
	if (info.shapenum == CEILICK1SPR)
	{
		y++;
	}
	else
	{
		y += info.y * 8;
	}
	VWB_DrawSprite(x, y, info.shapenum);

	y = WindowY + WindowH - 16;
	editwidth = 100;
	x = WindowX + (WindowW - 100) / 2;
	VWB_Bar(x, y, editwidth, 14, BLACK);
	VWB_Bar(x+1, y+1, editwidth-2, 12, WHITE);
	x += 2;
	y += 2;
	editwidth -= 8;

	if (US_LineInput(x, y, strbuf, NULL, true, sizeof(strbuf), editwidth))
	{
		ptr = strbuf;
		correct = true;
		while (*name)
		{
			c = *ptr;
			if (_ck_toupper(c) != *name)
			{
				correct = false;
			}

			ptr++;
			name++;
		}
		if (*ptr)
		{
			correct = false;
		}

		if (!correct)
		{
			VWB_Bar(0, 0, 320, 200, BackColor);
			US_CenterWindow(35, 5);
			PrintY += 11;
			US_CPrint("Sorry, that's not quite right.");
			US_CPrint("Please check your manual and try again.");
			IN_Ack();
		}
	}

	VWB_Bar(0, 0, 320, 200, BackColor);
	checkpassed = correct;
	return correct;
}

#endif
