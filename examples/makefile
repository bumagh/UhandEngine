CC=gcc
SDL2_INC_PATH=../include
SDL2_LIB_PATH=../lib
CFLAGS=-g -Wall -O0 -Dpc_build -D_GNU_SOURCE=1 -D_REENTRANT -pedantic -lm
SFLAGS= -I$(SDL2_INC_PATH)  -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2
FONTS_FLAGS=-lSDL2_ttf -L$(SDL2_LIB_PATH)/SDL2_ttf_2.20.2
MIXER_FLAGS=-lSDL2_mixer -L$(SDL2_LIB_PATH)/SDL2_mixer_2.6.3

# 引擎核心源文件（移除 util.c 和 draw.c 因为它们依赖 SDL）
ENGINE_SOURCES = ../src/engine/Scene.c \
                 ../src/engine/gameObjectList.c \
                 ../src/engine/gameObject.c \
                 ../src/engine/algorithm.c \
                 ../src/engine/transform.c \
                 ../src/engine/component.c \
                 ../src/engine/list.c

ENGINE_OBJECTS=$(ENGINE_SOURCES:.c=.o)

# 示例程序
EXAMPLES = sprite_example text_example container_example properties_example scene_example render_example

.PHONY: all clean $(EXAMPLES)

all: $(EXAMPLES)

# 编译引擎核心对象
../src/%.o: ../src/%.c
	$(CC) -c $^ $(CFLAGS) $(SFLAGS) $(FONTS_FLAGS) $(MIXER_FLAGS) -o $@

# 编译各个示例（链接 SDL）
sprite_example: sprite_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

text_example: text_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

container_example: container_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

properties_example: properties_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

scene_example: scene_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

render_example: render_example.c
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -lSDL2_ttf -L$(SDL2_LIB_PATH)/SDL2_ttf_2.20.2 -mconsole

clean:
	del /Q $(EXAMPLES) 2>nul || true
	del /Q $(ENGINE_OBJECTS) 2>nul || true
