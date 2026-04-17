// platform.h
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef pc_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// 平台上下文结构体
typedef struct PlatformContext {
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *defaultFont;
    
    // 平台特定
    #ifdef __EMSCRIPTEN__
    int isWebPlatform;
    #endif
} PlatformContext;

// 平台初始化
int Platform_Init(PlatformContext *ctx, const char *title, int width, int height);

// 平台清理
void Platform_Shutdown(PlatformContext *ctx);

// 主循环控制
void Platform_RunMainLoop(void (*tickFunc)(void), void (*quitFunc)(void));

// 资源加载
int Platform_LoadFont(PlatformContext *ctx, const char *fontPath, int fontSize);

// 帧率控制
void Platform_DelayFrame(int targetFPS);

// 平台特定函数
#ifdef __EMSCRIPTEN__
int Platform_IsWebPlatform(PlatformContext *ctx);
#endif

#endif // PLATFORM_H
