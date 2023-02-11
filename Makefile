
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

OBJ_SRC := $(wildcard obj/*.o)

LVL_FILES=$(wildcard src/romstuffs/LEVELS/*.c)
OBJ_LVLS=$(addprefix obj/rom/lvls/, $(notdir $(LVL_FILES:.c=.o)))

GFX_FILES=$(wildcard src/romstuffs/GFX/*.c)
OBJ_GFX=$(addprefix obj/rom/gfx/, $(notdir $(GFX_FILES:.c=.o)))

some: build_src build

all: mkdirs build_src build

build_src:
	arm-none-eabi-gcc -c src/GBA_Defs.c -o obj/GBA_Defs.o $(CFLAGS)
	arm-none-eabi-gcc -c src/CK_Levels.c -o obj/CK_Levels.o $(CFLAGS) 
	arm-none-eabi-gcc -c src/CK_Graphics.c -o obj/CK_Graphics.o $(CFLAGS) 
	arm-none-eabi-gcc -c src/CK_Main.c -o obj/CK_Main.o $(CFLAGS) 

build: $(OBJ_LVLS) $(OBJ_GFX)
	arm-none-eabi-gcc crt0.s $(OBJ_SRC) $(OBJ_LVLS) $(OBJ_GFX) $(CFLAGS)
	arm-none-eabi-objcopy -v -O binary a.out bin/$(GB_GBA).gba
	gbafix bin/$(GB_GBA).gba -t $(GB_GBA)

obj/rom/lvls/%.o: src/romstuffs/LEVELS/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

obj/rom/gfx/%.o: src/romstuffs/GFX/%.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

mkdirs:
	mkdir -p bin
	mkdir -p obj/rom/gfx
	mkdir -p obj/rom/lvls

.PHONY: clean
clean:
	rm -rf obj/rom/gfx/*.o
	rm -rf obj/rom/lvls/*.o

