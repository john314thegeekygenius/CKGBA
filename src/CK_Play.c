/*
    Commander Keen Galaxy for the Gameboy Advance
    Written by John314
    Feb 2023
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


// ck_newobj is known as new in OG code
objtype *ck_newobj, *check, *player, *scoreobj;

objtype dummyobj;

Sint16 invincible;

boolean oldshooting, showscorebox, joypad;

Sint16 groundslam;

boolean debugok;


//===========================================================================


