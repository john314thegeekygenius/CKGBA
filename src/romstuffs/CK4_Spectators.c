/*
char far l0n[] = "Shadowlands";
char far l1n[] = "Border Village";
char far l2n[] = "Slug Village";
char far l3n[] = "The Perilous Pit";
char far l4n[] = "Cave of the Descendents";
char far l5n[] = "Chasm of Chills";
char far l6n[] = "Crystalus";
char far l7n[] = "Hillville";
char far l8n[] = "Sand Yego";
char far l9n[] = "Miragia";
char far l10n[] = "Lifewater Oasis";
char far l11n[] = "Pyramid of the Moons";
char far l12n[] = "Pyramid of Shadows";
char far l13n[] = "Pyramid of the\nGnosticene Ancients";
char far l14n[] = "Pyramid of the Forbidden";
char far l15n[] = "Isle of Tar";
char far l16n[] = "Isle of Fire";
char far l17n[] = "Well of Wishes";
char far l18n[] = "Bean-with-Bacon\nMegarocket";

char far l0e[] = "Keen enters the\nShadowlands";
char far l1e[] = "Keen makes a run for\nthe Border Village";
char far l2e[] = "Keen slips into\nSlug Village";
char far l3e[] = "Keen plummets into\nthe The Perilous Pit";	// sic!
char far l4e[] = "Keen plods down into\nthe Cave of the\nDescendents";
char far l5e[] = "Keen shivers along\nthe Chasm of Chills";
char far l6e[] = "Keen reflects upon\nentering Crystalus";
char far l7e[] = "Keen stumbles upon\nHillville";
char far l8e[] = "Keen grits his teeth\nand enters Sand Yego";
char far l9e[] = "Keen disappears into\nMiragia";
char far l10e[] = "Keen crawls into\nLifewater Oasis";
char far l11e[] = "Keen backs into the\nPyramid of the Moons";
char far l12e[] = "Keen move silently in\nthe Pyramid of Shadows";	// sic!
char far l13e[] = "Keen reverently enters\nthe Pyramid of the\nGnosticene Ancients";
char far l14e[] = "Keen hesitantly crosses\ninto the Pyramid of the\nForbidden";
char far l15e[] = "Keen mucks along the\nIsle of Tar";
char far l16e[] = "Keen blazes across the\nIsle of Fire";
char far l17e[] = "Keen hopefully enters\nthe Well of Wishes";
char far l18e[] = "Keen launches into the\nBean-with-Bacon\nMegarocket";

char far *levelnames[GAMELEVELS] = {
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

char far *levelenter[GAMELEVELS] = {
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
};*/

void CK_LoadSpectators(){
    // Load all the sprites
    for(int y = 0; y < CK_CurLevelHeight; y++){
        for(int x = 0; x < CK_CurLevelWidth; x++){
            const uint32_t offset = (y*CK_CurLevelWidth)+x+(CK_CurLevelSize*2);
            // Copy the level data over
            uint32_t tileid = CK_LevelInfo[(CK_CurLevelIndex*3)+2][offset];
            if(tileid == 0) continue; 
            switch(tileid){
                case 1:
                    // Commander Keen (Right)
                    SpawnKeen(x, y, 1);
                    CK_MoveCamera((x-7)<<4, (y-5)<<4); // Move the camera there
		    		SpawnScore();
                    break;
                case 2:
                    // Commander Keen (Left)
                    SpawnKeen(x, y, -1);
                    CK_MoveCamera((x-7)<<4, (y-5)<<4); // Move the camera there
	    			SpawnScore();
                    break;
                case 3:
                    // Commander Keen (Map)
                    CK_MoveCamera((x-7)<<4, (y-5)<<4); // Move the camera there
                    SpawnWorldKeen(x, y);
    				SpawnScore();
                    break;
            }
        }
    }
    CK_UpdateRendering = true;

};
/*

void ScanInfoPlane(void)
{
	objtype *ob;
	Uint16 i, x, y, chunk;
	Sint16 info;
	Uint16 far *map;

	map = mapsegs[2];

	for (y=0; y<mapheight; y++)
	{
		for (x=0; x<mapwidth; x++)
		{
			info = *map++;

			if (info == 0)
				continue;

			switch (info)
			{
			case 1:
				SpawnKeen(x, y, 1);
				SpawnScore();
				lumpneeded[KEEN_LUMP] = true;
				CA_MarkGrChunk(SCOREBOXSPR);
				break;

			case 2:
				SpawnKeen(x, y, -1);
				SpawnScore();
				lumpneeded[KEEN_LUMP] = true;
				CA_MarkGrChunk(SCOREBOXSPR);
				break;

			case 3:
				SpawnWorldKeen(x, y);
				SpawnScore();
				lumpneeded[WOLRDKEEN_LUMP] = true;
				CA_MarkGrChunk(SCOREBOXSPR);
				break;

			case 4:
				SpawnCouncil(x, y);
				lumpneeded[COUNCIL_LUMP] = true;
				break;

			case 50:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 49:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 5:
				SpawnBerkeloid(x, y);
				lumpneeded[BERKELOID_LUMP] = true;
				break;

			case 6:
				SpawnLindsey(x, y);
				lumpneeded[LINDSEY_LUMP] = true;
				break;

			case 52:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 51:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 7:
				SpawnWormMouth(x, y);
				lumpneeded[WORMOUTH_LUMP] = true;
				break;

			case 46:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 45:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 8:
				SpawnSkypest(x, y);
				lumpneeded[SKYPEST_LUMP] = true;
				break;

			case 9:
				SpawnCloudster(x, y);
				lumpneeded[THUNDERCLOUD_LUMP] = true;
				break;

			case 10:
				SpawnFoot(x, y);
				lumpneeded[INCHWORM_LUMP] = true;	// lump includes the foot sprite
				// Note: The smoke sprites aren't actually required for the foot!
				for (i=SMOKE1SPR; i<=SMOKE4SPR; i++)
				{
					CA_MarkGrChunk(i);
				}
				break;

			case 11:
				SpawnInchworm(x, y);
				lumpneeded[INCHWORM_LUMP] = true;
				for (i=SMOKE1SPR; i<=SMOKE4SPR; i++)
				{
					CA_MarkGrChunk(i);
				}
				break;

			case 12:
				SpawnBounder(x, y);
				lumpneeded[BOUNDER_LUMP] = true;
				break;

			case 13:
				SpawnEggbird(x, y);
				lumpneeded[EGGBIRD_LUMP] = true;
				lumpneeded[EGG_LUMP] = true;
				break;

			case 48:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 47:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 14:
				SpawnLick(x, y);
				lumpneeded[LICK_LUMP] = true;
				break;

			case 88:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 87:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 15:
				SpawnDopefish(x, y);
				lumpneeded[DOPEFISH_LUMP] = true;
				break;

			case 16:
				SpawnSchoolfish(x, y);
				lumpneeded[SCHOOLFISH_LUMP] = true;
				break;

			case 24:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 23:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 17:
				SpawnPixie(x, y);
				lumpneeded[SPRITE_LUMP] = true;
				break;

			case 18:
				SpawnEater(x, y);
				lumpneeded[EATER_LUMP] = true;
				break;

			case 19:
				SpawnMimrock(x, y);
				lumpneeded[MIMROCK_LUMP] = true;
				break;

			case 74:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 73:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 20:
				SpawnArachnut(x, y);
				lumpneeded[ARACHNUT_LUMP] = true;
				break;

			case 21:
				SpawnMadMushroom(x, y);
				lumpneeded[MADMUSHROOM_LUMP] = true;
				break;

			case 44:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 43:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 22:
				SpawnSlug(x, y);
				lumpneeded[SLUG_LUMP] = true;
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
				lumpneeded[PLATFORM_LUMP] = true;;
				break;

			case 32:
				SpawnDropPlat(x, y);
				lumpneeded[PLATFORM_LUMP] = true;
				break;

			case 33:
				SpawnMiragia(x, y);
				break;

			case 34:
				if (gamestate.ammo < 5)
				{
					SpawnBonus(x, y, 11);
					lumpneeded[bonuslump[11]] = true;
				}
				break;

			case 35:
				SpawnScuba(x, y);
				CA_MarkGrChunk(SCUBASPR);
				break;

			case 42:
				SpawnSwimKeen(x, y);
				SpawnScore();
				lumpneeded[SCUBAKEEN_LUMP] = true;
				//mark pickup shapes:
				for (i=BONUS100SPR; i<=BONUSCLIPSPR; i++)
				{
					CA_MarkGrChunk(i);
				}
				CA_MarkGrChunk(SCOREBOXSPR);
				break;

			case 83:
			case 84:
			case 85:
			case 86:
				if (gamestate.difficulty < gd_Hard)
					break;
				SpawnDartShooter(x, y, info-83);
				lumpneeded[DARTS_LUMP] = true;
				break;

			case 79:
			case 80:
			case 81:
			case 82:
				if (gamestate.difficulty < gd_Normal)
					break;
				SpawnDartShooter(x, y, info-79);
				lumpneeded[DARTS_LUMP] = true;
				break;

			case 53:
			case 54:
			case 55:
			case 56:
				SpawnDartShooter(x, y, info-53);
				lumpneeded[DARTS_LUMP] = true;
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
				lumpneeded[bonuslump[info-57]] = true;
				break;

			case 69:
			case 70:
			case 71:
			case 72:
				SpawnMine(x, y, info-69);
				lumpneeded[MINE_LUMP] = true;
				break;

			case 75:
				lumpneeded[MOON_LUMP] = true;
				break;

			case 78:
				if (gamestate.difficulty < gd_Hard)
					break;
			case 77:
				if (gamestate.difficulty < gd_Normal)
					break;
			case 76:
				SpawnEggbirdOut(x, y);
				lumpneeded[EGGBIRD_LUMP] = true;
				break;
			}
		}
	}

	for (ob = player; ob; ob = ob->next)
	{
		if (ob->active != ac_allways)
			ob->active = ac_no;
	}

	for (i = 0; i < NUMLUMPS; i++)
	{
		if (lumpneeded[i])
		{
			for (chunk = lumpstart[i]; chunk <= lumpend[i]; chunk++)
			{
				CA_MarkGrChunk(chunk);
			}
		}
	}
}

*/