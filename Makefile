ifeq ($(OS), Windows_NT)
CC = g++
CFLAGS = -Wall -Wextra -ggdb -pedantic -std=c11 -lraylib -lopengl32 -lgdi32 -lwinmm -lm 
LIBS = -Lvendor/raylib/lib/windows
else
CC = g++
CFLAGS = -Wall -Wextra -ggdb -pedantic -std=c11 -lraylib -lGL -lm -lpthread -ldl -lX11 
LIBS = -Lvendor/raylib/lib/linux
endif

CONSOLE = bin/yafacon
BUILDER = bin/yafabuild

INCLUDE = -Iinclude/ -Ivendor/raylib/include/ -Ivendor/cJSON/ -Ivendor/binn/
CONSOLE_SOURCE = $(wildcard src/console/*.cpp) $(wildcard src/console/**/*.cpp) $(wildcard vendor/binn/*.c) $(wildcard vendor/cJSON/*.c)
BUILDER_SOURCE = $(wildcard src/builder/*.cpp) $(wildcard src/builder/**/*.cpp) $(wildcard vendor/binn/*.c) $(wildcard vendor/cJSON/*.c)

console: $(wildcard src/console/*.cpp) $(wildcard src/console/**/*.cpp) $(wildcard include/*.h) $(wildcard include/**/*.h)
	$(CC) $(LIBS) $(INCLUDE) -o $(CONSOLE) $(CONSOLE_SOURCE) $(CFLAGS)

builder: $(wildcard src/builder/*.cpp) $(wildcard src/builder/**/*.cpp) $(wildcard include/*.h) $(wildcard include/**/*.h)
	$(CC) $(LIBS) $(INCLUDE) -o $(BUILDER) $(BUILDER_SOURCE) $(CFLAGS)

pch:
	$(CC) $(LIBS) $(INCLUDE) include/pch.h -x $(CFLAGS)
