# CKGBA
Commander Keen Galaxy Engine for the GBA

# Notes
This project uses some code from: http://k1n9duk3.shikadi.net/files/modding/keensource456.zip
More info can be found: https://pckf.com/viewtopic.php?t=11505

# Adding / Modifying Actors
When ever RF_PlaceSprite is called, the first argument must be changed to the object pointer
So, if the code is:
```RF_PlaceSprite(&ob->sprite, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);```
The code should be changed to:
```RF_PlaceSprite(ob, ob->x, ob->y, ob->shapenum, spritedraw, ob->priority);```
This goes for the function RF_RemoveSprite:
```RF_RemoveSprite((void**)&ob->sprite);```
should be
```RF_RemoveSprite(ob);```

All cases where a new object is created via GetNewObj(), 
if the object name 'new' it must be changed to 'ck_newobj'

Also, the function CK_SetSprite() must be called after the object is created in order
to set the sprite type.
