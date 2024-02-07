The LIST.TXT file from KeenGraph must be placed in the directory with the execuatble

Must add the following lines to the LIST.txt file (whitespace included):
3141: 4SPR3142.BMP                             1
3142: 4SPR3141.BMP                   [0,3]     1
9999: 4SPR9999.BMP                   [0,0]     1

SPRITES.txt entries use the bitmap value (i.e 4SPR0124.BMP) would be:
CKS_Sprite 124

The following must be added to the list:
CKS_ScoreBoxDOS 3141
CKS_ScoreBoxGBA 3142
CKS_DUMMY 9999
CKS_EOL 9999

DO NOT USE THE EXTRACTED SCORE BOX GRAPHIC!

Lumps in the GFXE_CKx.H file need to be split to corrolate 
with the sprites defined in the SPRITES.txt file.

Sprite Graphics defined in GFXE_CKx.H are numbered sequencaly
only in lumps. If the enum value is not in a lump block, it will have a
value of 1, even if there are values above it.  

so:

'''
START_LUMP(ONEUP_LUMP_START, __ONEUPSTART)  <--- Starts the lump
ONEUPASPR,                   // 239         <--- == 1
ONEUPBSPR,                   // 240         <--- == 2
END_LUMP(ONEUP_LUMP_END, __ONEUPEND)        <--- Ends the lump

DOORSPR,                     // 241         <--- == 1
SCOREBOXSPR,                 // 253         <--- == 1
'''


