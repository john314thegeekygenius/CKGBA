
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS := -O3 -w -mcpu=arm7tdmi -nostartfiles -Tlnkscript

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

GFX_FILES=$(wildcard src/romstuffs/GFX/*.c)
OBJ_GFX=$(addprefix obj/rom/gfx/, $(notdir $(GFX_FILES:.c=.o)))

SPR_FILES=$(wildcard src/romstuffs/GFX/SPRITES/*.c)
OBJ_SPR=$(addprefix obj/rom/gfx/sprites/, $(notdir $(SPR_FILES:.c=.o)))

MUS_FILES=$(wildcard src/romstuffs/AUDIO/MUSIC/*.c)
OBJ_MUS=$(addprefix obj/rom/audio/music/, $(notdir $(MUS_FILES:.c=.o)))


.PHONY: src
src: $(OBJ_SRC)

.PHONY: music
music: $(OBJ_MUS)

all: mkdirs clean music src build

build: $(OBJ_SRC) $(OBJ_LVLS) $(OBJ_GFX) $(OBJ_SPR)
	arm-none-eabi-gcc crt0.s $(OBJ_SRC) $(OBJ_LVLS) $(OBJ_GFX) $(OBJ_SPR) $(OBJ_MUS) $(CFLAGS)
	arm-none-eabi-objcopy -v -O binary a.out bin/$(GB_GBA).gba
	gbafix bin/$(GB_GBA).gba -t $(GB_GBA)

obj/%.o: src/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/rom/lvls/%.o: src/romstuffs/LEVELS/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/rom/gfx/%.o: src/romstuffs/GFX/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/rom/gfx/sprites/%.o: src/romstuffs/GFX/SPRITES/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/rom/audio/music/%.o: src/romstuffs/AUDIO/MUSIC/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

mkdirs:
	mkdir -p bin
	mkdir -p obj/rom/gfx
	mkdir -p obj/rom/gfx/sprites
	mkdir -p obj/rom/lvls
	mkdir -p obj/rom/audio/music
	mkdir -p obj/rom/audio/sound

.PHONY: clean
clean:
	rm -rf obj/*.o
	rm -rf obj/rom/gfx/*.o
	rm -rf obj/rom/gfx/sprites/*.o
	rm -rf obj/rom/lvls/*.o
	rm -rf obj/rom/audio/music/*.o
	rm -rf obj/rom/audio/sound/*.o

