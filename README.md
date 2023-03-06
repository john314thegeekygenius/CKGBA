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
