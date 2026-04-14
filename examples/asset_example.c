/**
 * Asset Manager Example
 *
 * 演示资源管理系统的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/AssetManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // 设置控制台编码为 UTF-8
    system("chcp 65001 > nul");

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // 初始化 SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // 初始化 SDL_ttf
    if (TTF_Init() < 0)
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Asset Manager Example",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    printf("=== Asset Manager Example ===\n\n");

    // 创建资源管理器
    AssetManager *assetManager = AssetManager_Create("./");
    if (!assetManager)
    {
        printf("Failed to create AssetManager\n");
        return 1;
    }

    // 加载字体
    TTF_Font *font = AssetManager_LoadFont(assetManager, "default", "fzpix.ttf", 32);
    if (font)
    {
        printf("Font loaded successfully\n");
    }

    // 尝试加载纹理（如果有的话）
    SDL_Texture *texture = AssetManager_LoadTexture(assetManager, "test", "test.png", renderer);
    if (texture)
    {
        printf("Texture loaded successfully\n");
    }
    else
    {
        printf("Texture not found (expected if test.png doesn't exist)\n");
    }

    printf("\nPress Q to quit...\n");

    // 主循环
    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_q)
                {
                    running = 0;
                }
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 如果纹理存在，绘制纹理
        if (texture)
        {
            SDL_Rect destRect = {300, 200, 200, 200};
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
        }
        else
        {
            // 绘制占位矩形
            SDL_Rect placeholder = {300, 200, 200, 200};
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            SDL_RenderFillRect(renderer, &placeholder);
            SDL_RenderDrawRect(renderer, &placeholder);

            // 绘制文本
            if (font)
            {
                SDL_Color color = {255, 255, 255, 255};
                SDL_Surface *surface = TTF_RenderUTF8_Blended(font, "No texture loaded", color);
                if (surface)
                {
                    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (textTexture)
                    {
                        SDL_Rect textRect = {300, 250, surface->w, surface->h};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    SDL_FreeSurface(surface);
                }
            }
        }

        // 渲染
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // 清理资源管理器
    AssetManager_Destroy(assetManager);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    printf("Asset Manager Example ended\n");
    return 0;
}
