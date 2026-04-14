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
                 ../src/engine/list.c \
                 ../src/engine/RenderQueue.c

# TextComponent 单独编译，只有需要的示例链接
TEXTCOMPONENT_SOURCES = ../src/engine/UI/TextComponent.c

ENGINE_OBJECTS=$(ENGINE_SOURCES:.c=.o)

# 示例程序
EXAMPLES = sprite_example text_example container_example properties_example scene_example render_example asset_example renderqueue_example input_example camera_example animation_example tween_example

.PHONY: all clean $(EXAMPLES)

all: $(EXAMPLES)

# 编译引擎核心对象
../src/%.o: ../src/%.c
	$(CC) -c $^ $(CFLAGS) $(SFLAGS) $(FONTS_FLAGS) $(MIXER_FLAGS) -o $@

# 编译各个示例（链接 SDL）
sprite_example: sprite_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

text_example: text_example.c $(ENGINE_OBJECTS) $(TEXTCOMPONENT_SOURCES:.c=.o)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) $(TEXTCOMPONENT_SOURCES:.c=.o) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -lSDL2_ttf -L$(SDL2_LIB_PATH)/SDL2_ttf_2.20.2 -mconsole

container_example: container_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

properties_example: properties_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

scene_example: scene_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

render_example: render_example.c $(TEXTCOMPONENT_SOURCES:.c=.o)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(TEXTCOMPONENT_SOURCES:.c=.o) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -lSDL2_ttf -L$(SDL2_LIB_PATH)/SDL2_ttf_2.20.2 -mconsole

asset_example: asset_example.c ../src/engine/AssetManager.o
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< ../src/engine/AssetManager.o -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -lSDL2_image -L$(SDL2_LIB_PATH)/SDL2_image_2.6.3 -lSDL2_mixer -L$(SDL2_LIB_PATH)/SDL2_mixer_2.6.3 -lSDL2_ttf -L$(SDL2_LIB_PATH)/SDL2_ttf_2.20.2 -mconsole

renderqueue_example: renderqueue_example.c $(ENGINE_OBJECTS)
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< $(ENGINE_OBJECTS) -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

input_example: input_example.c ../src/engine/InputManager.o
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< ../src/engine/InputManager.o -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

camera_example: camera_example.c ../src/engine/Camera.o ../src/engine/InputManager.o
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< ../src/engine/Camera.o ../src/engine/InputManager.o -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

animation_example: animation_example.c ../src/engine/Animation.o
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< ../src/engine/Animation.o -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

tween_example: tween_example.c ../src/engine/Tween.o
	$(CC) -o $@ $(CFLAGS) -I$(SDL2_INC_PATH) $< ../src/engine/Tween.o -L$(SDL2_LIB_PATH)/SDL2_2.28.1 -lSDL2main -lSDL2 -mconsole

clean:
	del /Q $(EXAMPLES) 2>nul || true
	del /Q $(ENGINE_OBJECTS) 2>nul || true
