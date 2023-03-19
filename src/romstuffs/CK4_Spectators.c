
const char l0n[] = "Shadowlands";
const char l1n[] = "Border Village";
const char l2n[] = "Slug Village";
const char l3n[] = "The Perilous Pit";
const char l4n[] = "Cave of the Descendents";
const char l5n[] = "Chasm of Chills";
const char l6n[] = "Crystalus";
const char l7n[] = "Hillville";
const char l8n[] = "Sand Yego";
const char l9n[] = "Miragia";
const char l10n[] = "Lifewater Oasis";
const char l11n[] = "Pyramid of the Moons";
const char l12n[] = "Pyramid of Shadows";
const char l13n[] = "Pyramid of the\nGnosticene Ancients";
const char l14n[] = "Pyramid of the Forbidden";
const char l15n[] = "Isle of Tar";
const char l16n[] = "Isle of Fire";
const char l17n[] = "Well of Wishes";
const char l18n[] = "Bean-with-Bacon\nMegarocket";

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
const char l12e[] = "Keen move silently in\nthe Pyramid of Shadows";	// sic!
const char l13e[] = "Keen reverently enters\nthe Pyramid of the\nGnosticene Ancients";
const char l14e[] = "Keen hesitantly crosses\ninto the Pyramid of the\nForbidden";
const char l15e[] = "Keen mucks along the\nIsle of Tar";
const char l16e[] = "Keen blazes across the\nIsle of Fire";
const char l17e[] = "Keen hopefully enters\nthe Well of Wishes";
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

const char *lindseytext[2] =
{
	"There's gear to help\n"
	"you swim in Three-Tooth\n"
	"Lake. It is hidden in\n"
	"Miragia.\n"
	,
	"The way to the Pyramid\n"
	"of the Forbidden lies\n"
	"under the Pyramid of\n"
	"Moons.\n"
};

const char *klindseytext[2] =
{
	"Thanks, your Highness!"
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
	US_CPrint("Princess Lindsey says:\n");
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

const char jantext1[] =
	"Thanks for going to all\n"
	"that trouble, but I'm\n"
	"just the janitor for the\n"
	"High Council.";

const char jantext2[] =
	"I tried to tell the\n"
	"Shikadi that but they\n"
	"just wouldn't listen...";

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
	SD_WaitSoundDone();
	// BUG: haven't made anything purgable here, caching the pic may cause an "out of memory" crash
//	CA_CacheGrChunk(KEENTALK1PIC);

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

const char *keentext[] = {
	"No sweat, oh guardian\n"
	"of wisdom!"
	,
	"Sounds like a plan,\n"
	"bearded one!"
	,
	"No problemo."
	,
	"Great.  You know, you\n"
	"look a lot like the\n"
	"last guy I rescued..."
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
	if (mapon == 17)
	{
		US_CPrint(
			"Ggoh thig you sogh mg\n"
			"fgor regscuing mgge!\n"
			"I'gll regur tgo the\n"
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
	VWB_DrawPic(WindowX+WindowW-48, WindowY, KEENTALK1PIC);
	WindowW -= 48;
	PrintY += 12;
	US_CPrint(keentext[gamestate.rescued]);

	VW_WaitVBL(30);
	IN_ClearKeysDown();
	IN_Ack();

	VWB_DrawPic(WindowX+WindowW, WindowY, KEENTALK2PIC);
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
	CK_SetSprite(&player->sprite, CKS_KEENSWIM);
	NewState(player, &s_keenswimslow1);
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
	CK_SetSprite(&ck_newobj->sprite, CKS_SBUBBLE);
	switch (US_RndT() / 64)
	{
	case 0:
		NewState(ck_newobj, &s_kbubble1);
		break;
	case 1:
		NewState(ck_newobj, &s_kbubble2);
		break;
	case 2:
		NewState(ck_newobj, &s_kbubble3);
		break;
	case 3:
		NewState(ck_newobj, &s_kbubble4);
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
