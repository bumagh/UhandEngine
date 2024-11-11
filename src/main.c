#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#ifdef pc_build

#include <SDL2/SDL.h>
#include <SDL2/SDL_platform.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_platform.h"

#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "util.h"
#include "draw.h"
#include "global.h"
#include "./engine/list.h"
#include "./engine/uiComponent.h"
#include "./engine/gameObjectList.h"
int w, h;
#define SDL_DELAY 12
#define FRAMERATE 30
// 全局或静态变量，用于管理弹窗状态
int isPopupVisible = 0;    // 弹窗是否可见
UIComponent *popup = NULL; // 存储弹窗的指针
int success = 1;
SDL_Event e;
int gQuit = 0;
float mouseX = 0;
float mouseY = 0;
int fingerSupport = 1;
int numFrames = 0;
int startTime = 0;
GameObjectList *goList;
// 全局变量用于存储时间
Uint32 lastTime = 0;
Uint32 currentTime = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_bool full_screen = SDL_FALSE;
TTF_Font *font;
SDL_Color textMenuColor = {0, 0, 0, 255};

int gMusicCondition = 1;
int init()
{
    srand(time(NULL));
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_AllocateChannels(16);
    if (TTF_Init() == -1)
    {
        SDL_Log("TTF unable to initialize! Error: %s\n", TTF_GetError());
        success = 0;
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = 0;
    }
    else
    {
        window = SDL_CreateWindow("UhandEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE);
        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
            // Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = 0;
            }
        }
    }
    return success;
}

void handleButtons()
{

    SDL_KeyCode mkey = e.key.keysym.sym;
    switch (e.type)
    {
    case SDL_QUIT:
        gQuit = 1;
        break;
    case SDL_KEYDOWN:
    {

        if (mkey == 'q')
        {
            gQuit = 1;
        }
    }
    break;
    case SDL_FINGERDOWN:

        break;
    case SDL_MOUSEBUTTONDOWN:

        break;
    case SDL_MOUSEMOTION:
        break;
    case SDL_MOUSEBUTTONUP:

        break;
    }
}

void tick()
{
    // 获取当前时间（单位是毫秒）
    currentTime = SDL_GetTicks();
    // 计算 deltaTime（单位：秒）
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    GameObjectList_CallUpdate(goList);
    while (SDL_PollEvent(&e) != 0)
    {
        handleButtons();
    }
    SDL_SetRenderDrawColor(renderer, 117, 117, 117, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // 更新 lastTime，准备下一帧使用
    lastTime = currentTime;
}
void loadMedia()
{
#if defined(pc_build) || defined(__EMSCRIPTEN__)
    font = TTF_OpenFont("assets/fzpix.ttf", 16);
#else
    font = TTF_OpenFont("assets/fzpix.ttf", 16);
#endif
}
void quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // 释放字体
    if (font)
    {
        TTF_CloseFont(font);
        font = NULL;
    }

    // 调用销毁函数并释放 GameObjectList 资源
    if (goList)
    {
        GameObjectList_CallDestroy(goList);
        free(goList);
        goList = NULL;
    }

    TTF_Quit();
    printf("quit ok\n");
}

void gameInit()
{
    SDL_GetWindowSize(window, &w, &h);

    goList = createGameObjectList();
    // 调用生命周期函数
    GameObjectList_CallAwake(goList);
    GameObjectList_CallStart(goList);
}
void loop()
{
    startTime = SDL_GetTicks();
#if __EMSCRIPTEN__
    emscripten_set_main_loop(tick, 0, 0);
#else
    while (!gQuit)
    {
        tick();
        long current = SDL_GetTicks();
        long cost = current - startTime;
        long frame = 1000 / FRAMERATE;
        long delay = frame - cost;
        if (delay > 0)
            SDL_Delay(delay);
    }
    quit();

#endif
}
#ifndef __EMSCRIPTEN__
#undef main
#endif
int main(int argc, char *argv[])
{
    if (!init())
    {
        printf("SDL could not be initialized\n");
        return 1;
    }
    loadMedia();
    gameInit();
    loop();
    return 0;
}
