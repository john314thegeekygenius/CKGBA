# The offical guide to porting Commander Keen to the Gameboy Advance

## Step 1
- Accuire an fully functional keen game - this could be the original or a mod

## Step 2:
- Convert the original files to useable files:<pre>
	Sound:
	-- PC Speaker sound effects
	-- Sound Blaster sound effects
	Music
	-- Adlib music
	Maps
	-- Levels
	-- Tile Info
	Graphics
	-- Tilesets
	-- Sprites
	-- Bitmaps
	Texts
	-- Help
	-- Ending
	Demos
- A list of tools and how to use each are [here](TOOLS.md)

## Step 3:
- Make any needed / wanted changes to the source code
- Noteable changes needed will be headed with a comment<pre>// MODDERS:

## Step 4:
- Compile the ROM

## Step 5:
- Play the ROM on a GBA or emulator


# Adding / Modifying Actors

All cases where a new object is created via GetNewObj(), 
if the object name 'new' it must be changed to 'ck_newobj'

Also, the function NewState() now takes an extra parameter -
The type of sprite denoted with 'CKS_'
