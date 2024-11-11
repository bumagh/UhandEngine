CC=gcc
SDL2_INC_PATH=../include
SDL2_LIB_PATH=../lib
CFLAGS=-g -Wall -O0 -D_GNU_SOURCE=1 -D_REENTRANT -pedantic  -lm
SFLAGS= -I$(SDL2_INC_PATH)  -L$(SDL2_LIB_PATH)/SDL2_2.0.20 -lSDL2main -lSDL2
FONTS_FLAGS=-lSDL2_ttf -L$(SDL2_LIB_PATH)/SDL2_ttf_2.20.0
IMAGE_FLAGS=-lSDL2_image -L$(SDL2_LIB_PATH)/SDL2_image_2.0.5
MIXER_FLAGS=-lSDL2_mixer -L$(SDL2_LIB_PATH)/SDL2_mixer_2.0.4


SOURCES= $(wildcard ./*.c)
OBJECTS=$(SOURCES:.c=.o)
BINARIES=testemccoopc
BINARIES_PATH=../bin/$(BINARIES)

WEB_PATH=../web
all: $(BINARIES)

testemccoopc: $(OBJECTS)
	$(CC) -o $(BINARIES_PATH)/$(BINARIES) $(OBJECTS) $(CFLAGS) $(SFLAGS) $(FONTS_FLAGS) ${MIXER_FLAGS}

%.o: %.c
	$(CC) -c $^ $(CFLAGS) $(SFLAGS) $(FONTS_FLAGS) ${MIXER_FLAGS} -o $@
# -lglib-2.0 -lgobject-2.0

.PHONY:emclean clean emcc emrun emre
emcc: $(SOURCES)
	emcc $(SOURCES) -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -fms-extensions -ID:\2-work\1-IT\c\4-libs\glib-dev_2.26.1-1_win64\include\glib-2.0 -LD:\2-work\1-IT\c\4-libs\glib-dev_2.26.1-1_win64\lib -c -o test.o
emrun: 
	emrun --no_browser --port 3000 index.html
emre: $(SOURCES)
	make emcc &&make emrun

emclean:
	$(RM) *.o
clean:
	$(RM) *.o
	