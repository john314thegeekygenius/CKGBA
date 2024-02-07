NAME := cktxted

LIB_NAME := libRRasterLib
RETRO_DIR := /media/jonathan/AcerDisk/Ubuntu/Programing/RetroRasterLib

CXX_FLAGS := -Wall -O3 -std=c++20 -Iinclude -I/usr/include/SDL2 -I$(RETRO_DIR)/include
CXX_LIBS := -lSDL2main -lSDL2 -lSDL2_image
LINK_FLAGS := -Wl

# Where the library will be
LIB_DIR := $(RETRO_DIR)/lib
# Library Flags
SHARED_RRLIB_FLAGS := -L$(LIB_DIR)/static -l:$(LIB_NAME).a

CC := g++

SRC_FILES=$(wildcard src/*.cpp)
OBJ_SRC=$(addprefix obj/, $(notdir $(SRC_FILES:.cpp=.o)))

TINY_FILES=$(wildcard src/tinyfiledialogs/*.c)
OBJ_TINY=$(addprefix obj/, $(notdir $(TINY_FILES:.c=.o)))

.PHONY: src 
src: clean_src $(OBJ_SRC) build

all: mkdirs clean src

build: $(OBJ_SRC) $(OBJ_TINY)
	$(CC) $(OBJ_SRC) $(OBJ_TINY) $(CXX_FLAGS) -o bin/$(NAME) $(SHARED_RRLIB_FLAGS) $(CXX_LIBS)

obj/%.o: src/%.cpp
	$(CC) $(CXX_FLAGS) -c $< -o $@

obj/%.o: src/tinyfiledialogs/%.c
	$(CC) $(CXX_FLAGS) -c $< -o $@

mkdirs:
	mkdir -p bin
	mkdir -p obj

.PHONY: clean
clean:
	rm -rf obj/*.o

.PHONY: clean_src
clean_src:
	rm -rf obj/*.o

