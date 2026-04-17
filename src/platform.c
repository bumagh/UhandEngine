// platform.c
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 全局平台上下文
static PlatformContext *g_platformContext = NULL;
static int g_targetFPS = 60;

// 平台初始化
int Platform_Init(PlatformContext *ctx, const char *title, int width, int height) {
    if (!ctx) return 0;
    
    // 初始化随机数种子
    srand(time(NULL));
    
    // 初始化音频
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Warning: Failed to initialize audio\n");
    }
    Mix_AllocateChannels(16);
    
    // 初始化 TTF
    if (TTF_Init() == -1) {
        printf("TTF unable to initialize! Error: %s\n", TTF_GetError());
        return 0;
    }
    
    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // 创建窗口
    ctx->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
    if (!ctx->window) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 0;
    }
    
    // 创建渲染器
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
    if (!ctx->renderer) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->window);
        return 0;
    }
    
    ctx->defaultFont = NULL;
    
    #ifdef __EMSCRIPTEN__
    ctx->isWebPlatform = 1;
    #endif
    
    g_platformContext = ctx;
    return 1;
}

// 平台清理
void Platform_Shutdown(PlatformContext *ctx) {
    if (!ctx) return;
    
    // 释放字体
    if (ctx->defaultFont) {
        TTF_CloseFont(ctx->defaultFont);
        ctx->defaultFont = NULL;
    }
    
    // 销毁渲染器和窗口
    if (ctx->renderer) {
        SDL_DestroyRenderer(ctx->renderer);
        ctx->renderer = NULL;
    }
    
    if (ctx->window) {
        SDL_DestroyWindow(ctx->window);
        ctx->window = NULL;
    }
    
    // 退出 SDL
    TTF_Quit();
    SDL_Quit();
    
    g_platformContext = NULL;
}

// 主循环控制
void Platform_RunMainLoop(void (*tickFunc)(void), void (*quitFunc)(void)) {
    #ifdef __EMSCRIPTEN__
    // Web 平台使用 emscripten 主循环
    emscripten_set_main_loop(tickFunc, 0, 1);
    #else
    // Native 平台使用 while 循环
    while (1) {
        tickFunc();
        Platform_DelayFrame(g_targetFPS);
        
        // 检查是否需要退出
        // 这里需要从全局状态获取退出标志
        // 暂时简化处理
        SDL_Event e;
        if (SDL_PollEvent(&e) && e.type == SDL_QUIT) {
            break;
        }
    }
    
    if (quitFunc) {
        quitFunc();
    }
    #endif
}

// 资源加载
int Platform_LoadFont(PlatformContext *ctx, const char *fontPath, int fontSize) {
    if (!ctx) return 0;
    
    ctx->defaultFont = TTF_OpenFont(fontPath, fontSize);
    if (!ctx->defaultFont) {
        printf("Failed to load font! TTF Error: %s\n", TTF_GetError());
        return 0;
    }
    
    return 1;
}

// 帧率控制
void Platform_DelayFrame(int targetFPS) {
    g_targetFPS = targetFPS;
    
    #ifdef __EMSCRIPTEN__
    // Web 平台由浏览器控制帧率
    #else
    static Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    Uint32 frameTime = 1000 / targetFPS;
    Uint32 elapsed = currentTime - lastTime;
    
    if (elapsed < frameTime) {
        SDL_Delay(frameTime - elapsed);
    }
    
    lastTime = SDL_GetTicks();
    #endif
}

// 平台特定函数
#ifdef __EMSCRIPTEN__
int Platform_IsWebPlatform(PlatformContext *ctx) {
    return ctx ? ctx->isWebPlatform : 0;
}
#endif
