
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS := -O3 -w -mcpu=arm7tdmi -nostartfiles -Tlnkscript

CC := arm-none-eabi-gcc

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
#LIBS	:= -lmm -lgba
#CLIBS := 

GB_GBA = CK4GBA

SRC_FILES=$(wildcard src/*.c)
OBJ_SRC=$(addprefix obj/, $(notdir $(SRC_FILES:.c=.o)))

LVL_FILES=$(wildcard src/romstuffs/LEVELS/*.c)
OBJ_LVLS=$(addprefix obj/rom/lvls/, $(notdir $(LVL_FILES:.c=.o)))

DEMO_FILES=$(wildcard src/romstuffs/DEMOS/*.c)
OBJ_DEMO=$(addprefix obj/rom/demos/, $(notdir $(DEMO_FILES:.c=.o)))

GFX_FILES=$(wildcard src/romstuffs/GFX/*.c)
OBJ_GFX=$(addprefix obj/rom/gfx/, $(notdir $(GFX_FILES:.c=.o)))

GFX2_FILES=$(wildcard src/romstuffs/GFX/INTR/*.c)
OBJ_GFX2=$(addprefix obj/rom/gfx/, $(notdir $(GFX2_FILES:.c=.o)))

BMP_FILES=$(wildcard src/romstuffs/GFX/BMPS/*.c)
OBJ_BMP=$(addprefix obj/rom/gfx/bmps/, $(notdir $(BMP_FILES:.c=.o)))

TXT_FILES=$(wildcard src/romstuffs/TXT/*.c)
OBJ_TXT=$(addprefix obj/rom/txt/, $(notdir $(TXT_FILES:.c=.o)))

SPR_FILES=$(wildcard src/romstuffs/GFX/SPRITES/*.c)
OBJ_SPR=$(addprefix obj/rom/gfx/sprites/, $(notdir $(SPR_FILES:.c=.o)))

MUS_FILES=$(wildcard src/romstuffs/AUDIO/MUSIC/*.c)
OBJ_MUS=$(addprefix obj/rom/audio/music/, $(notdir $(MUS_FILES:.c=.o)))

ADLIB_FILES=$(wildcard src/romstuffs/AUDIO/ADLIB/*.c)
OBJ_ADLIB=$(addprefix obj/rom/audio/sound/, $(notdir $(ADLIB_FILES:.c=.o)))

PCS_FILES=$(wildcard src/romstuffs/AUDIO/PC/*.c)
OBJ_PCS=$(addprefix obj/rom/audio/sound/, $(notdir $(PCS_FILES:.c=.o)))


.PHONY: src 
src: clean_src $(OBJ_SRC) build

.PHONY: music
music: $(OBJ_MUS)

all: mkdirs clean music src build

build: $(OBJ_SRC) $(OBJ_LVLS) $(OBJ_GFX) $(OBJ_GFX2) $(OBJ_BMP) $(OBJ_TXT) $(OBJ_SPR) $(OBJ_DEMO) $(OBJ_ADLIB) $(OBJ_PCS)
	$(CC) crt0.s $(OBJ_SRC) $(OBJ_LVLS) $(OBJ_GFX) $(OBJ_GFX2) $(OBJ_BMP) $(OBJ_TXT) $(OBJ_DEMO) $(OBJ_SPR) $(OBJ_MUS) $(OBJ_ADLIB) $(OBJ_PCS) $(CFLAGS)
	arm-none-eabi-objcopy -v -O binary a.out bin/$(GB_GBA).gba
	gbafix bin/$(GB_GBA).gba -t $(GB_GBA)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/lvls/%.o: src/romstuffs/LEVELS/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/demos/%.o: src/romstuffs/DEMOS/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/gfx/%.o: src/romstuffs/GFX/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/gfx/bmps/%.o: src/romstuffs/GFX/BMPS/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/gfx/%.o: src/romstuffs/GFX/INTR/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/txt/%.o: src/romstuffs/TXT/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/gfx/sprites/%.o: src/romstuffs/GFX/SPRITES/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/audio/music/%.o: src/romstuffs/AUDIO/MUSIC/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/audio/sound/%.o: src/romstuffs/AUDIO/ADLIB/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/rom/audio/sound/%.o: src/romstuffs/AUDIO/PC/%.c
	$(CC) $(CFLAGS) -c $< -o $@

mkdirs:
	mkdir -p bin
	mkdir -p obj/rom/txt
	mkdir -p obj/rom/gfx/bmps
	mkdir -p obj/rom/gfx/sprites
	mkdir -p obj/rom/lvls
	mkdir -p obj/rom/demos
	mkdir -p obj/rom/audio/music
	mkdir -p obj/rom/audio/sound

.PHONY: clean
clean:
	rm -rf obj/*.o
	rm -rf obj/rom/txt/*.o
	rm -rf obj/rom/gfx/*.o
	rm -rf obj/rom/gfx/bmps/*.o
	rm -rf obj/rom/gfx/sprites/*.o
	rm -rf obj/rom/lvls/*.o
	rm -rf obj/rom/demos/*.o
	rm -rf obj/rom/audio/music/*.o
	rm -rf obj/rom/audio/sound/*.o

.PHONY: clean_src
clean_src:
	rm -rf obj/*.o
