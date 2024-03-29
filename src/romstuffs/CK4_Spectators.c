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

// MODDERS:
// List of every object state for saving / loading

// State list generated by makestatelist.py
const statetype* CK_StateList[] = {
	// Keen
	&s_keenstand, &s_keenpauselook, &s_keenwait1, &s_keenwait2, &s_keenwait3, &s_keenwait4, &s_keenwait5, &s_keenwait6, 
#ifdef KEEN4
	&s_keenmoon1, &s_keenmoon2, &s_keenmoon3, 
#endif
	&s_keenread, &s_keenread2, &s_keenread3, &s_keenread4, &s_keenread5, 
	&s_keenread6, &s_keenread7, &s_keenstopread, &s_keenstopread2, &s_keenstopread3, &s_keenlookup, &s_keenlookup2, &s_keenlookdown, &s_keenlookdown2, &s_keenlookdown3, &s_keenlookdown4, &s_keendrop, &s_keendead, &s_keendie1, &s_keendie2, 
#ifdef KEEN4
	&s_keensuitdie1, &s_keensuitdie2, 
#endif
	&s_keenshoot1, &s_keenshoot2, &s_keenshootup1, &s_keenshootup2, &s_keenswitch, &s_keenswitch2, &s_keenkey, &s_keenlineup, &s_keenenter1, &s_keenenter2, &s_keenenter3, &s_keenenter4, &s_keenenter5, &s_keenpole, &s_keenclimb1, &s_keenclimb2, 
	&s_keenclimb3, &s_keenslide1, &s_keenslide2, &s_keenslide3, &s_keenslide4, &s_keenpoleshoot1, &s_keenpoleshoot2, &s_keenpoleshootup1, &s_keenpoleshootup2, &s_keenpoleshootdown1, &s_keenpoleshootdown2, &s_keenwalk1, &s_keenwalk2, &s_keenwalk3, &s_keenwalk4, &s_keenpogodown, 
	&s_keenpogo, &s_keenpogo2, &s_keenjump1, &s_keenjump2, &s_keenjump3, &s_keenjump4, &s_keenairshoot1, &s_keenairshoot2, &s_keenairshoot3, &s_keenairshootup1, &s_keenairshootup2, &s_keenairshootup3, &s_keenairshootdown1, &s_keenairshootdown2, &s_keenairshootdown3, &s_keenholdon, 
	&s_keenholdon2, &s_keenclimbup, &s_keenclimbup2, &s_keenclimbup3, &s_keenclimbup4, &s_keenclimbup5, 
#ifdef KEEN5
	&s_keenenter0,
	&s_keenteleport,
#endif
#ifdef KEEN5
	&s_keenenter6,
#endif
	// Score Box
	&s_score, &s_demo, 
	// World Keen
	&s_worldkeen, &s_worldkeenwave1, &s_worldkeenwave2, &s_worldkeenwave3, &s_worldkeenwave4, &s_worldkeenwave5, &s_worldkeenwalk, 
#ifdef KEEN4
	&s_keenonfoot1, &s_keenonfoot2, &s_worldswim, 
#endif
#ifdef KEEN5
	&s_worldelevate, 
#endif
	// World Flag
	&s_flagwave1, &s_flagwave2, &s_flagwave3, &s_flagwave4, 
#ifndef KEEN5
	&s_throwflag0, &s_throwflag1, &s_throwflag2, &s_throwflag3, &s_throwflag4, &s_throwflag5, &s_throwflag6, 
#endif
	// Stunner
	&s_stunray1, &s_stunray2, &s_stunray3, &s_stunray4, &s_stunhit, &s_stunhit2, 
	// Door
	&s_door1, &s_door2, &s_door3, 
#ifdef KEEN5
	&s_carddoor, 
#endif
	// Act1
	&s_miragia0, &s_miragia1, &s_miragia2, &s_miragia3, &s_miragia4, &s_miragia5, &s_miragia6, &s_miragia7, &s_bonus1, &s_bonus2, &s_bonusrise, &s_splash1, &s_splash2, &s_splash3, &s_councilwalk1, &s_councilwalk2, 
	&s_councilstand, &s_slugwalk1, &s_slugwalk2, &s_slugpiss1, &s_slugstun, &s_slugstunalt, &s_slugslime, &s_slugslime2, &s_mushroom1, &s_mushroom2, &s_egg, &s_eggbroke, &s_eggchip1, &s_eggchip2, &s_eggchip3, &s_eggbirdpause, 
	&s_eggbirdwalk1, &s_eggbirdwalk2, &s_eggbirdwalk3, &s_eggbirdwalk4, &s_eggbirdfly1, &s_eggbirdfly2, &s_eggbirdfly3, &s_eggbirdfly4, &s_eggbirddrop, &s_eggbirdstun, &s_eggbirdstun2, &s_eggbirdstun3, &s_eggbirdstun4, &s_eggbirdstun5, &s_arach1, &s_arach2, 
	&s_arach3, &s_arach4, &s_arachstun, &s_arachstun2, &s_arachstun3, &s_arachstun4, &s_arachstun5, &s_pestfly1, &s_pestfly2, &s_squashedpest, &s_pestrest1, &s_pestrest2, &s_pestrest3, &s_pestrest4, &s_pestrest5, &s_pestrest6, 
	&s_pestrest7, &s_pestrest8, &s_pestrest9, &s_pestrest10, &s_pestrest11, &s_pestrest12, &s_pestrest13, &s_pestrest14, &s_pestrest15, &s_pestrest16, &s_pestrest17, 
	// Act 2
	&s_worm, &s_wormpeek1, &s_wormpeek2, &s_wormpeek3, &s_wormpeek4, &s_wormpeek5, &s_wormpeek6, &s_wormpeek7, &s_wormpeek8, &s_wormbite1, &s_wormbite2, &s_wormbite3, &s_wormbite4, &s_wormbite5, &s_wormstun, &s_cloudsleep, 
	&s_cloudwake, &s_cloud, &s_cloudalign, &s_cloudcharge, &s_cloudattack1, &s_cloudattack2, &s_cloudattack3, &s_cloudattack4, &s_cloudattack5, &s_cloudattack6, &s_cloudattack7, &s_cloudattack8, &s_cloudattack9, &s_bolt1, &s_bolt2, &s_bolt3, 
	&s_bolt4, &s_bolt5, &s_bolt6, &s_berkefloat1, &s_berkefloat2, &s_berkefloat3, &s_berkefloat4, &s_berkethrow1, &s_berkethrow2, &s_berkethrow3, &s_berkethrow4, &s_berkethrow5, &s_berkethrow6, &s_berkethrow7, &s_berkethrow8, &s_berkethrow9, 
	&s_berkethrow10, &s_berkethrow11, &s_berkethrow12, &s_fire1, &s_fire2, &s_fireland1, &s_fireland2, &s_fireland3, &s_fireland4, &s_fireland5, &s_fireland6, &s_fireland7, &s_fireland8, &s_fireland9, &s_footsmoke1, &s_footsmoke2, 
	&s_footsmoke3, &s_footsmoke4, &s_inch1, &s_inch2, &s_footchange, &s_footwait, &s_bounderup1, &s_bounderup2, &s_bounderside1, &s_bounderside2, &s_bounderstun, &s_bounderstun2, &s_lick1, &s_lick2, &s_lick3, &s_lick4, 
	&s_licklick1, &s_licklick2, &s_licklick3, &s_licklick4, &s_licklick5, &s_licklick6, &s_licklick7, &s_licklick8, &s_lickstun, &s_lickstun2, &s_platform, &s_dropplatsit, &s_dropplatfall, &s_dropplatrise, 
	// Act 3
	&s_eaterstand1, &s_eaterstand2, &s_eatertport1, &s_eatertport2, &s_eatertport3, &s_eatertport4, &s_eatertport5, &s_eatertport6, &s_eatertport7, &s_eatertport8, &s_eaterjump1, &s_eaterjump2, &s_eaterjump3, &s_eaterjump4, &s_eaterstun, &s_eaterstun2, 
	&s_eatenbonus1, &s_eatenbonus2, &s_eatenbonus3, &s_eatenbonus4, &s_mimrock, &s_mimsneak1, &s_mimsneak2, &s_mimsneak3, &s_mimsneak4, &s_mimsneak5, &s_mimsneak6, &s_mimbonk1, &s_mimbonk2, &s_mimbonk3, &s_mimbounce, &s_mimstun, 
	&s_mimstun2, &s_dopefish1, &s_dopefish2, &s_dopeattack, &s_dopeeat, &s_dopeburp1, &s_dopeburp2, &s_dopereturn, &s_dopefood, &s_keendopefood, &s_keendieslow, &s_bubble1, &s_bubble2, &s_bubble3, &s_bubble4, &s_schoolfish1, 
	&s_schoolfish2, &s_pixie, &s_pixielook, &s_pixieshoot, &s_pixieshoot2, &s_pixiefire1, &s_pixiefire2, &s_pixiefire3, &s_pixiefire4, &s_mine, &s_mineboom1, &s_mineboom2, &s_lindsey1, &s_lindsey2, &s_lindsey3, &s_lindsey4, 
	&s_dartthrower, &s_dart1, &s_dart2, &s_dartup1, &s_dartup2, &s_dartdown1, &s_dartdown2, &s_scuba, 
	// Spectators
	&s_keenswimslow1, &s_keenswimslow2, &s_keenswim1, &s_keenswim2,
	&s_kbubble1, &s_kbubble2, &s_kbubble3, &s_kbubble4,
	// These should never be saved??
	&sc_deadstate, &sc_badstate,
	nil // Needed for end of list
};

// DEPRECATED
// Dynamic Palettes - Number of levels + default
#ifdef CK_DYNAMIC_PAL
const unsigned short CK_DYNAMIC_PALS[] = {
	// Normal EGA palette 0
	RGBCONV(0xccffcc), RGBCONV(0x000000), RGBCONV(0x0000aa), RGBCONV(0x00aa00), RGBCONV(0x00aaaa), RGBCONV(0xaa0055), RGBCONV(0xaa5500), RGBCONV(0xaaaaaa), RGBCONV(0x555555), RGBCONV(0x5555ff), RGBCONV(0x55ff55), RGBCONV(0x55ffff), RGBCONV(0xff5555), RGBCONV(0xff55ff), RGBCONV(0xffff55), RGBCONV(0xffffff), 
	// Shade (Green)      1
	RGBCONV(0xccffcc), RGBCONV(0x44a1a), RGBCONV(0x44a7d), RGBCONV(0x48d1a), RGBCONV(0x48d7d), RGBCONV(0x7b4a42), RGBCONV(0x7b5f1a), RGBCONV(0x7b8d7d), RGBCONV(0x3c5f42), RGBCONV(0x3c5fbb), RGBCONV(0x3cc542), RGBCONV(0x3cc5bb), RGBCONV(0xb95f42), RGBCONV(0xb95fbb), RGBCONV(0xb9c542), RGBCONV(0xb9c5bb), 
	// Shade (Blue)       2
	RGBCONV(0xccffcc), RGBCONV(0x4a8ca0), RGBCONV(0x4a8cc3), RGBCONV(0x4ab4a0), RGBCONV(0x4ab4c3), RGBCONV(0x8d8ca9), RGBCONV(0x8d96a0), RGBCONV(0x8db4c3), RGBCONV(0x5f96a9), RGBCONV(0x5f96ec), RGBCONV(0x5fe0a9), RGBCONV(0x5fe0ec), RGBCONV(0xc596a9), RGBCONV(0xc596ec), RGBCONV(0xc5e0a9), RGBCONV(0xc5e0ec), 
	// UnderGround        3
	RGBCONV(0xccffcc), RGBCONV(0xa133e), RGBCONV(0xa1388), RGBCONV(0xa7c3e), RGBCONV(0xa7c88), RGBCONV(0x7b1356), RGBCONV(0x7b403e), RGBCONV(0x7b7c88), RGBCONV(0x3d4056), RGBCONV(0x3d40c1), RGBCONV(0x3dba56), RGBCONV(0x3dbac1), RGBCONV(0xba4056), RGBCONV(0xba40c1), RGBCONV(0xbaba56), RGBCONV(0xbabac1), 
	// Temple             4
	RGBCONV(0xccffcc), RGBCONV(0x411403), RGBCONV(0x41147b), RGBCONV(0x417c03), RGBCONV(0x417c7b), RGBCONV(0x89143c), RGBCONV(0x894003), RGBCONV(0x897c7b), RGBCONV(0x58403c), RGBCONV(0x5840b9), RGBCONV(0x58ba3c), RGBCONV(0x58bab9), RGBCONV(0xc2403c), RGBCONV(0xc240b9), RGBCONV(0xc2ba3c), RGBCONV(0xc2bab9), 
	// Water              5
	RGBCONV(0xccffcc), RGBCONV(0x233298), RGBCONV(0x2332bd), RGBCONV(0x238398), RGBCONV(0x2383bd), RGBCONV(0x7f32a1), RGBCONV(0x7f4e98), RGBCONV(0x7f83bd), RGBCONV(0x464ea1), RGBCONV(0x464ee7), RGBCONV(0x46bfa1), RGBCONV(0x46bfe7), RGBCONV(0xbc4ea1), RGBCONV(0xbc4ee7), RGBCONV(0xbcbfa1), RGBCONV(0xbcbfe7), 
	// Fire / Hot         6
	RGBCONV(0xccffcc), RGBCONV(0x7e380a), RGBCONV(0x7e387b), RGBCONV(0x7e850a), RGBCONV(0x7e857b), RGBCONV(0xab383d), RGBCONV(0xab520a), RGBCONV(0xab857b), RGBCONV(0x8a523d), RGBCONV(0x8a52ba), RGBCONV(0x8ac03d), RGBCONV(0x8ac0ba), RGBCONV(0xd9523d), RGBCONV(0xd952ba), RGBCONV(0xd9c03d), RGBCONV(0xd9c0ba),
};

const unsigned short CK_DynamicPalIndex[GAMELEVELS+1] = {
	0, // Default (CWrist / Help)
	0, 1, 1, 
	1, 3, 3, 
	2, 1, 6, 
	2, 4, 4, 
	4, 4, 1, 
	6, 5, 1, 
	0, 0, 0, 0, 0, 0, 0, // Unused
};
#endif

// MODDERS:

// From orginial
const char l0n[] = "Shadowlands";
const char l1n[] = "Border Village";
const char l2n[] = "Slug Village";
const char l3n[] = "The Perilous Pit";
const char l4n[] = "Cave of the\nDescendents";
const char l5n[] = "Chasm of Chills";
const char l6n[] = "Crystalus";
const char l7n[] = "Hillville";
const char l8n[] = "Sand Yego";
const char l9n[] = "Miragia";
const char l10n[] = "Lifewater Oasis";
const char l11n[] = "Pyramid of the Moons";
const char l12n[] = "Pyramid of Shadows";
const char l13n[] = "Pyramid of the\nGnosticene Ancients";
const char l14n[] = "Pyramid of the\nForbidden";
const char l15n[] = "Isle of Tar";
const char l16n[] = "Isle of Fire";
const char l17n[] = "Well of Wishes";
const char l18n[] = "Bean-with-Bacon\nMegarocket";


// Lines cannot be more than 20 characters in length
const char l0e[] = "Keen enters the\nShadowlands";
const char l1e[] = "Keen makes a run for\nthe Border Village";
const char l2e[] = "Keen slips into\nSlug Village";
const char l3e[] = "Keen plummets into\nthe The Perilous Pit";	// sic!
const char l4e[] = "Keen plods down into\nthe Cave of the\nDescendents";
const char l5e[] = "Keen shivers along\nthe Chasm of Chills";
const char l6e[] = "Keen reflects upon\nentering Crystalus";
const char l7e[] = "Keen stumbles upon\nHillville";
const char l8e[] = "Keen grits his teeth\nand enters Sand Yego";
const char l9e[] = "Keen disappears into\nMiragia";
const char l10e[] = "Keen crawls into\nLifewater Oasis";
const char l11e[] = "Keen backs into the\nPyramid of the Moons";
const char l12e[] = "Keen move silently\nin the Pyramid \nof Shadows";	// sic!
const char l13e[] = "Keen reverently \nenters the Pyramid\nof the\nGnosticene Ancients";
const char l14e[] = "Keen hesitantly\ncrosses into the\nPyramid of the\nForbidden";
const char l15e[] = "Keen mucks along the\nIsle of Tar";
const char l16e[] = "Keen blazes across\nthe Isle of Fire";
const char l17e[] = "Keen hopefully\nenters the\nWell of Wishes";
const char l18e[] = "Keen launches\ninto the\nBean-with-Bacon\nMegarocket";

const char *levelnames[GAMELEVELS] = {
	l0n,
	l1n,
	l2n,
	l3n,
	l4n,
	l5n,
	l6n,
	l7n,
	l8n,
	l9n,
	l10n,
	l11n,
	l12n,
	l13n,
	l14n,
	l15n,
	l16n,
	l17n,
	l18n
};

const char *levelenter[GAMELEVELS] = {
	l0e,
	l1e,
	l2e,
	l3e,
	l4e,
	l5e,
	l6e,
	l7e,
	l8e,
	l9e,
	l10e,
	l11e,
	l12e,
	l13e,
	l14e,
	l15e,
	l16e,
	l17e,
	l18e
};

void CK_LoadSpectators(){
    // Load all the sprites
    for(int y = 0; y < CK_CurLevelHeight; y++){
        for(int x = 0; x < CK_CurLevelWidth; x++){
            const uint32_t offset = (y*CK_CurLevelWidth)+x;//+(CK_CurLevelSize*2);
            // Copy the level data over
            uint16_t info = CK_GetInfo(offset);//CK_LevelInfo[(CK_CurLevelIndex*3)+2][offset];
            if(info == 0) continue; 
            switch(info){
                case 1:
				SpawnKeen(x, y, 1);
				SpawnScore();
				break;

				case 2:
					SpawnKeen(x, y, -1);
					SpawnScore();
					break;

				case 3:
					SpawnWorldKeen(x, y);
					SpawnScore();
					break;
				case 4:
					SpawnCouncil(x, y);
					break;

				case 50:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 49:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 5:
					SpawnBerkeloid(x, y);
					break;

				case 6:
					SpawnLindsey(x, y);
					break;

				case 52:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 51:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 7:
					SpawnWormMouth(x, y);
					break;

				case 46:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 45:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 8:
					SpawnSkypest(x, y);
					break;

				case 9:
					SpawnCloudster(x, y);
					break;

				case 10:
					SpawnFoot(x, y);
					break;

				case 11:
					SpawnInchworm(x, y);
					break;

				case 12:
					SpawnBounder(x, y);
					break;

				case 13:
					SpawnEggbird(x, y);
					break;

				case 48:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 47:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 14:
					SpawnLick(x, y);
					break;

				case 88:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 87:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 15:
					SpawnDopefish(x, y);
					break;

				case 16:
					SpawnSchoolfish(x, y);
					break;

				case 24:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 23:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 17:
					SpawnPixie(x, y);
					break;

				case 18:
					SpawnEater(x, y);
					break;

				case 19:
					SpawnMimrock(x, y);
					break;

				case 74:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 73:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 20:
					SpawnArachnut(x, y);
					break;

				case 21:
					SpawnMadMushroom(x, y);
					break;

				case 44:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 43:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 22:
					SpawnSlug(x, y);
					break;

				case 25:
					RF_SetScrollBlock(x, y, 1);
					break;

				case 26:
					RF_SetScrollBlock(x, y, 0);
					break;

				case 27:
				case 28:
				case 29:
				case 30:
					SpawnPlatform(x, y, info-27);
					break;

				case 32:
					SpawnDropPlat(x, y);
					break;

				case 33:
					SpawnMiragia(x, y);
					break;

				case 34:
					if (gamestate.ammo < 5)
					{
						SpawnBonus(x, y, 11);
					}
					break;

				case 35:
					SpawnScuba(x, y);
					break;

				case 42:
					SpawnSwimKeen(x, y);
					SpawnScore();
					break;

				case 83:
				case 84:
				case 85:
				case 86:
					if (gamestate.difficulty < gd_Hard)
						break;
					SpawnDartShooter(x, y, info-83);
					break;

				case 79:
				case 80:
				case 81:
				case 82:
					if (gamestate.difficulty < gd_Normal)
						break;
					SpawnDartShooter(x, y, info-79);
					break;

				case 53:
				case 54:
				case 55:
				case 56:
					SpawnDartShooter(x, y, info-53);
					break;

				case 57:
				case 58:
				case 59:
				case 60:
				case 61:
				case 62:
				case 63:
				case 64:
				case 65:
				case 66:
				case 67:
				case 68:
					SpawnBonus(x, y, info-57);
					break;
				case 69:
				case 70:
				case 71:
				case 72:
					SpawnMine(x, y, info-69);
					break;

				case 75:
					break;

				case 78:
					if (gamestate.difficulty < gd_Hard)
						break;
				case 77:
					if (gamestate.difficulty < gd_Normal)
						break;
				case 76:
					SpawnEggbirdOut(x, y);
					break;
            }
        }
    }
    CK_UpdateRendering = true;

	for (int i = 0; i < CK_NumOfObjects; i++)
	{
		objtype *ob = &CK_ObjectList[i];
		if (ob->active != ac_allways)
			ob->active = ac_no;
	}
};


//============================================================================

/*
===========================
=
= PrincessLindsey
=
===========================
*/
// MODDERS:
const char *lindseytext[2] =
{
	"There's gear to help\n"
	"you swim in \n"
	"Three-Tooth Lake.\n"
	"It is hidden in \n"
	"Miragia."
	,
	"The way to\n"
	"the Pyramid of\n"
	"the Forbidden lies\n"
	"under the Pyramid\n"
	"of Moons.\n"
};

const char *klindseytext[2] =
{
	"Thanks,\nyour Highness!"
	,
	"Thanks for the\n"
	"mysterious clue,\n"
	"Princess!\n"
};

void PrincessLindsey(void)
{
	SD_WaitSoundDone();
	StopMusic();

	CA_CacheMarks(NULL);
//	VW_FixRefreshBuffer();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX, WindowY, LINDSEYPIC);
	PrintY += 6;
	WindowW -= 48;
	WindowX += 48;
	// MODDERS:
	US_CPrint("Princess Lindsey\nsays:\n");
	if (mapon == 7)
	{
		US_CPrint(lindseytext[0]);
	}
	else
	{
		US_CPrint(lindseytext[1]);
	}

	SD_PlaySound(SND_MAKEFOOT);
	VW_WaitVBL(60);
	IN_ClearKeysDown();
	IN_Ack();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	WindowW -= 48;
	PrintY += 12;
	if (mapon == 7)
	{
		US_CPrint(klindseytext[0]);
	}
	else
	{
		US_CPrint(klindseytext[1]);
	}

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	VWB_DrawPic(WindowX+WindowW, WindowY, KEENTALK2PIC);

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	StartMusic(gamestate.mapon);
	//reset scorebox (sprite may have been re-cached by CA_DownLevel)
	scoreobj->temp2 = -1;
	scoreobj->temp1 = -1;
	scoreobj->temp3 = -1;
	scoreobj->temp4 = -1;

}

//============================================================================

/*
===========================
=
= RescueJanitor
=
===========================
*/
// MODDERS:
const char jantext1[] =
	"Thanks for going to\n"
	"all that trouble,\n"
	"but I'm just the \n"
	"janitor for the\n"
	"High Council.";

const char jantext2[] =
	"I tried to tell the\n"
	"Shikadi that but \n"
	"they just wouldn't\n"
	"listen...";

const char keenjantext[] =
	"This had better\n"
	"be a joke.";

const char jantext3[] =
	"Sorry.  You aren't\n"
	"mad, are you?";

void RescueJanitor(void)
{
	SD_WaitSoundDone();
	CA_CacheMarks(NULL);
//	VW_FixRefreshBuffer();
	StartMusic(-1);
	
	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX, WindowY, ORACLEPIC);
	PrintY += 6;
	WindowW -= 48;
	WindowX += 48;
	US_CPrint(jantext1);

	VW_WaitVBL(60);
	IN_ClearKeysDown();
	IN_Ack();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX, WindowY, ORACLEPIC);
	PrintY += 6;
	WindowW -= 48;
	WindowX += 48;

	US_CPrint(jantext2);

	VW_WaitVBL(60);
	IN_ClearKeysDown();
	IN_Ack();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	WindowW -= 48;
	PrintY += 12;

	US_CPrint(keenjantext);

	VW_WaitVBL(60);
	IN_ClearKeysDown();
	IN_Ack();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX, WindowY, ORACLEPIC);
	PrintY += 6;
	WindowW -= 48;
	WindowX += 48;

	US_CPrint(jantext3);

	VW_WaitVBL(60);
	IN_ClearKeysDown();
	IN_Ack();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	VWB_DrawPic(WindowX+WindowW-40, WindowY+24, KEENMADPIC);

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	StopMusic();

	StartMusic(gamestate.mapon);

	//BUG: scorebox needs to be reset here (sprite may have been re-cached by CA_DownLevel)
}

//============================================================================

/*
===========================
=
= CanitSwim
=
===========================
*/

void CantSwim(void)
{
	CK_TimedRumble(15); // Rumble for 15 ticks
	SD_WaitSoundDone();
	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	WindowW -= 48;
	PrintY += 12;
	US_CPrint("I can't swim!");
	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	//Note: scorebox sprite has not been re-cached here (didn't use CA_CacheMarks or anything else that would have made the sprite purgable)
}

//============================================================================

/*
===========================
=
= GotScuba
=
===========================
*/

void GotScuba(void)
{
	SD_WaitSoundDone();

	CA_CacheMarks(NULL);

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	WindowW -= 48;
	PrintY += 12;
	// MODDERS:
	US_CPrint(
		"Cool!  I can breathe\n"
		"under water now!"
		);

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	VWB_DrawPic(WindowX+WindowW, WindowY, KEENTALK2PIC);

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	//Note: scorebox sprite may have been re-cached by CA_DownLevel, but the level ends after this anyway
}

//============================================================================

/*
===========================
=
= RescuedMember
=
===========================
*/
// MODDERS:
const char *keentext[] = {
	"No sweat, oh\n"
	"guardian of wisdom!"
	,
	"Sounds like a plan,\n"
	"bearded one!"
	,
	"No problemo."
	,
	"Great. You know, you\n"
	"look a lot like the\n"
	"last guy I rescued.." // Should have 3 periods, but so it fits one is removed
	,
	"Good idea, Gramps."
	,
	"May the road rise\n"
	"to meet your feet,\n"
	"Mr. Member."
	,
	"Wise plan of action,\n"
	"your ancientness."
	,
	"You're the last one,\n"
	"fella.  Let's both\n"
	"get back to the\n"
	"Oracle chamber!"
};

void RescuedMember(void)
{
	SD_WaitSoundDone();

	StartMusic(-1);
//	VW_FixRefreshBuffer();

	US_CenterWindow(26, 8);
	VWB_DrawPic(WindowX, WindowY, ORACLEPIC);
	PrintY += 6;
	WindowW -= 48;
	WindowX += 48;
	// MODDERS:
	if (mapon == 17)
	{
		US_CPrint(
			"Ggoh thig you sogh\n"
			"mg fgor regscuing\n"
			"mgge! I'gll regur\n"
			"tgo the\n"
			"Goracle chagber\n"
			"igmediatggely. Blub."
			);
	}
	else
	{
		US_CPrint(
			"Oh thank you so much\n"
			"for rescuing me!\n"
			"I'll return to the\n"
			"Oracle chamber\n"
			"immediately."
			);
	}

	VW_WaitVBL(60);
	IN_ClearKeysDown();
	IN_Ack();

	US_CenterWindow(26, 8);
	if (mapon == 17){
		VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1WATERPIC);
	}else{
		VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	}
	WindowW -= 48;
	PrintY += 12;
	US_CPrint(keentext[gamestate.rescued]);

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	if (mapon == 17){
		VWB_DrawPic(WindowX+WindowW, WindowY, KEENTALK2WATERPIC);
	}else{
		VWB_DrawPic(WindowX+WindowW, WindowY, KEENTALK2PIC);
	}
	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	gamestate.rescued++;
	StopMusic();

	//Note: scorebox sprite may have been re-cached by CA_DownLevel, but the level ends after this anyway
}

/*
=============================================================================

						  SWIMMING KEEN

temp4 = counter for spawning bubbles

=============================================================================
*/

statetype s_keenswimslow1 = {SCUBAKEENL1SPR, SCUBAKEENR1SPR, stepthink, false, false, 50, 0, 0, T_KeenSwimSlow, C_KeenSwim, R_KeenSwim, &s_keenswimslow2};
statetype s_keenswimslow2 = {SCUBAKEENL2SPR, SCUBAKEENR2SPR, stepthink, false, false, 50, 0, 0, T_KeenSwimSlow, C_KeenSwim, R_KeenSwim, &s_keenswimslow1};
statetype s_keenswim1     = {SCUBAKEENL1SPR, SCUBAKEENR1SPR, stepthink, false, false, 50, 0, 0, T_KeenSwim, C_KeenSwim, R_KeenSwim, &s_keenswimslow2};
statetype s_keenswim2     = {SCUBAKEENL2SPR, SCUBAKEENR2SPR, stepthink, false, false, 50, 0, 0, T_KeenSwim, C_KeenSwim, R_KeenSwim, &s_keenswimslow1};
//Note: the die states for swimming Keen are in CK_KEEN.C and K4_ACT3.C (dopefish section)

statetype s_kbubble1  = {SMALLBUBBLE1SPR, SMALLBUBBLE1SPR, think, false, false, 20, 0, 24, T_Bubble, NULL, R_Draw, &s_kbubble1};
statetype s_kbubble2  = {SMALLBUBBLE2SPR, SMALLBUBBLE2SPR, think, false, false, 20, 0, 24, T_Bubble, NULL, R_Draw, &s_kbubble2};
statetype s_kbubble3  = {SMALLBUBBLE3SPR, SMALLBUBBLE3SPR, think, false, false, 20, 0, 24, T_Bubble, NULL, R_Draw, &s_kbubble3};
statetype s_kbubble4  = {SMALLBUBBLE4SPR, SMALLBUBBLE4SPR, think, false, false, 20, 0, 24, T_Bubble, NULL, R_Draw, &s_kbubble4};

/*
===========================
=
= SpawnSwimKeen
=
===========================
*/

void SpawnSwimKeen(Sint16 x, Sint16 y)
{
	player->obclass = keenobj;
	player->active = ac_allways;
	player->priority = 1;
	player->x = CONVERT_TILE_TO_GLOBAL(x);
	player->y = CONVERT_TILE_TO_GLOBAL(y);
	player->xdir = 1;
	player->ydir = 1;
	player->needtoclip = cl_fullclip;
	NewState(player, &s_keenswimslow1, CKS_KEENSWIM);
}

/*
===========================
=
= SpawnKbubble
=
===========================
*/

void SpawnKbubble(objtype *ob)
{
	ob->temp4 = 0;
	GetNewObj(true);
	if (ob->xdir == -1)
	{
		ck_newobj->x = ob->x;
	}
	else
	{
		ck_newobj->x = ob->x + 24*PIXGLOBAL;
	}
	ck_newobj->y = ob->y;
	ck_newobj->obclass = inertobj;
	ck_newobj->priority = 3;
	ck_newobj->active = ac_removable;
	ck_newobj->needtoclip = cl_noclip;
	ck_newobj->yspeed = -24;
	ck_newobj->xspeed = 4;
	switch (US_RndT() / 64)
	{
	case 0:
		NewState(ck_newobj, &s_kbubble1, CKS_SBUBBLE);
		break;
	case 1:
		NewState(ck_newobj, &s_kbubble2, CKS_SBUBBLE);
		break;
	case 2:
		NewState(ck_newobj, &s_kbubble3, CKS_SBUBBLE);
		break;
	case 3:
		NewState(ck_newobj, &s_kbubble4, CKS_SBUBBLE);
		break;
	}
	SD_PlaySound(SND_BLUB);
}

/*
===========================
=
= T_KeenSwimSlow
=
===========================
*/

void T_KeenSwimSlow(objtype *ob)
{
	Sint32 i;
	Sint16 vx, vy, xc, yc;

	xc = ob->xspeed < 0;
	yc = ob->yspeed < 4;

	ob->temp4 = ob->temp4 + tics;
	if (ob->temp4 > 60)
		SpawnKbubble(ob);

	if (jumpbutton && !jumpheld)
	{
		jumpheld = true;
		if (c.xaxis)
			ob->xspeed = c.xaxis * 18;
		if (c.yaxis)
			ob->yspeed = c.yaxis * 18;
		ob->state = ob->state->nextstate;
	}
	if (c.xaxis)
		ob->xdir = c.xaxis;

	for (i = lasttimecount-tics; i < lasttimecount; i++)
	{
		if ((i & 7) == 0)
		{
			if (ob->xspeed > 12)
			{
				vx = -3;
			}
			else if (ob->xspeed > 0)
			{
				vx = -1;
			}
			else if (ob->xspeed > -12)
			{
				vx = 1;
			}
			else
			{
				vx = 3;
			}
			vx += c.xaxis;
			vx += c.xaxis;
			ob->xspeed += vx;

			if (c.xaxis == 0 && (ob->xspeed < 0) != xc)
				ob->xspeed = 0;

			if (ob->yspeed > 12)
			{
				vy = -3;
			}
			else if (ob->yspeed > 4)
			{
				vy = -1;
			}
			else if (ob->yspeed > -12)
			{
				vy = 1;
			}
			else
			{
				vy = 3;
			}
			vy += c.yaxis;
			vy += c.yaxis;
			ob->yspeed += vy;

			if (c.yaxis == 0 && ob->yspeed > 4 && yc)
				ob->yspeed = 0;
		}
		xtry += ob->xspeed;
		ytry += ob->yspeed;
	}
}

/*
===========================
=
= T_KeenSwim
=
===========================
*/

void T_KeenSwim(objtype *ob)	//never actually used
{
	ob->temp4 = ob->temp4 + tics;
	if (ob->temp4 > 60)
		SpawnKbubble(ob);

	if (jumpbutton && !jumpheld)
	{
		jumpheld = true;
		ob->xspeed = c.xaxis * 18;
		if (c.yaxis)
			ob->yspeed = c.yaxis * 18;

		if (ob->state == &s_keenswim1)
		{
			ob->state = &s_keenswim2;
		}
		else
		{
			ob->state = &s_keenswim1;
		}
	}

	xtry = xtry + ob->xspeed * tics;
	ytry = ytry + ob->yspeed * tics;
	if (xtry > 0)
	{
		ob->xdir = 1;
	}
	else if (xtry < 0)
	{
		ob->xdir = -1;
	}

	ytry = ytry + tics*4;
}

/*
===========================
=
= C_KeenSwim
=
===========================
*/

void C_KeenSwim(objtype *ob, objtype *hit)
{
	switch (hit->obclass)
	{
	case bonusobj:
		switch (hit->temp1)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			SD_PlaySound(bonussound[hit->temp1]);
			hit->obclass = inertobj;
			hit->priority = 3;
			hit->shapenum = bonussprite[hit->temp1];
			GivePoints(bonuspoints[hit->temp1]);
			if (hit->temp1 < 4)
			{
				gamestate.keys[hit->temp1]++;
			}
			else if (hit->temp1 == 10)
			{
				gamestate.lives++;
			}
			else if (hit->temp1 == 11)
			{
				gamestate.ammo += shotsinclip[gamestate.difficulty];
			}
			ChangeState(hit, &s_bonusrise);
			CK_RemakeSprite(&hit->sprite, CK_BonusShadows[hit->temp1]);
			break;
		}
		break;

	case oracleobj:
		playstate = ex_rescued;
		break;
	}
	//ob++;			// shut up compiler
}

/*
===========================
=
= R_KeenSwim
=
===========================
*/

void R_KeenSwim(objtype *ob)
{
	if (ob->hiteast && ob->xspeed < 0 || ob->hitwest && ob->xspeed > 0)
		ob->xspeed = 0;

	if (ob->hitnorth && ob->yspeed > 0 || ob->hitsouth && ob->yspeed < 0)
		ob->yspeed = 0;

	RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);
	
}
