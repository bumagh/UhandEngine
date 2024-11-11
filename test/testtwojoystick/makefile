CC=gcc
SDL2_INC_PATH=../../include
SDL2_LIB_PATH=../../lib
CFLAGS=-g -Wall -O0 -Dpc_build -D_GNU_SOURCE=1 -D_REENTRANT -pedantic  -lm
SFLAGS= -I$(SDL2_INC_PATH)  -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2

SOURCES = fourjoystick.c
OBJECTS=$(SOURCES:.c=.o)
BINARIES=testtwojoystick
BINARIES_PATH=../../bin/$(BINARIES)

WEB_PATH=../../web
all: $(BINARIES)

testtwojoystick: $(OBJECTS)
	$(CC) -o $(BINARIES_PATH)/$(BINARIES) $(OBJECTS) $(CFLAGS) $(SFLAGS)

%.o: %.c
	$(CC) -c $^  $(CFLAGS) $(SFLAGS)  -o $@

.PHONY:emclean clean emcc emccd emrun emre
emcc: $(SOURCES)
	emcc $(SOURCES) -Wno-int-conversion -Wno-implicit-function-declaration -Wno-incompatible-function-pointer-types  -s USE_SDL=2 -s WASM=1 -s USE_OGG=1 -s USE_VORBIS=1 -s ALLOW_MEMORY_GROWTH=1  -o $(WEB_PATH)/$(BINARIES)/index.html  -std=c99 -s --shell-file $(WEB_PATH)/html_template/shell.html
emccd: $(SOURCES) 
	emcc $(SOURCES) -Wno-int-conversion -Wno-implicit-function-declaration -Wno-incompatible-function-pointer-types  -s USE_SDL=2 -s WASM=1   -s USE_OGG=1 -s USE_VORBIS=1 -s ALLOW_MEMORY_GROWTH=1  -o $(WEB_PATH)/$(BINARIES)/index.html -std=c99	
emrun: 
	emrun --no_browser --port 3000 $(WEB_PATH)/$(BINARIES)/index.html
emre: $(SOURCES)
	make emcc &&make emrun

emclean:
	$(RM) *.o $(WEB_PATH)/$(BINARIES)/*
clean:
	$(RM) *.o $(BINARIES_PATH)/$(BINARIES)
	
