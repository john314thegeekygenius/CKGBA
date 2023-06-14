# CKGBA Tools

## bmp2ckgba
- Converts bitmap images to a useable format

## ckdemomk
- Converts the Commander Keen Galaxy Demo files into a useable format

## ckgenmusic
- Converts the Commander Keen Galaxy IMF files into a useable format

## ckgensound
- Converts the Commander Keen Galaxy SND files into a useable format

## ckmap2gba

## ckspr2sheet
- Converts exported [Keen Graph](https://keenwiki.shikadi.net/wiki/Keengraph) bitmaps into
a usable format using the LIST.TXT file for information
- You need to rename all the sprite graphics in the format CK_SPRxxxx.BMP where the x's are the sprite number

## cktil2gba
- Converts the Commander Keen Galaxy tileinfo file into a useable format

## genckburn
- Generates a "screen burn" animation for the intro (code can be changed to produce a different effect) 
- Not needed for a mod port

## genckfont
- Converts the keen font to a "list" of 8x8 pixel characters
- Not needed for a mod port unless a custom font is desired

## fixdialogtxt.py
- Fixes the string lengths in a block of code to fit on the screen of the GBA
- See dialogs.txt for some example code

## makestatelist.py
- Makes an array of all the object states in the code base - requires all of the state declarations to be in a file

## mkgfxh.py
- Converts a (typically) IGRAB-ed C-header file into a new usable header for CKGBA
- Might need to use if the mod uses a different amount of sprites than normal keen (like Atroxian Relms)


