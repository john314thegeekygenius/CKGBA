# CKGBA
Commander Keen Galaxy Engine for the GBA

# Notes
This project uses some code from: http://k1n9duk3.shikadi.net/files/modding/keensource456.zip
More info can be found: https://pckf.com/viewtopic.php?t=11505

# Adding / Modifying Actors
All cases where a new object is created via GetNewObj(), 
if the object name 'new' it must be changed to 'ck_newobj'

Also, the function CK_SetSprite() must be called after the object is created, but before
NewState() is called, in order to set the sprite type correctly.
