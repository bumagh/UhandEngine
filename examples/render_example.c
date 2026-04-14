/**
 * Render Example
 *
 * 演示实际渲染内容到窗口
 * 绘制矩形、圆形和文本
 */

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

    // 初始化 SDL_ttf
    if (TTF_Init() < 0)
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Render Example",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        TTF_Quit();
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
        SDL_Quit();
        return 1;
    }

    // 加载字体
    TTF_Font *font = TTF_OpenFont("assets/fzpix.ttf", 32);
    if (!font)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        printf("Using default system font...\n");
    }

    printf("=== Render Example ===\n");
    printf("Press Q to quit\n\n");

    // 主循环
    int running = 1;
    SDL_Event event;
    float angle = 0.0f;

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

        // 清屏 - 深灰色背景
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制红色矩形
        SDL_Rect rect1 = {100, 100, 200, 150};
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
        SDL_RenderFillRect(renderer, &rect1);

        // 绘制绿色矩形
        SDL_Rect rect2 = {400, 100, 200, 150};
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
        SDL_RenderFillRect(renderer, &rect2);

        // 绘制蓝色矩形
        SDL_Rect rect3 = {250, 300, 300, 200};
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        SDL_RenderFillRect(renderer, &rect3);

        // 绘制旋转的黄色矩形
        angle += 1.0f;
        SDL_Rect rotatingRect = {350, 250, 100, 100};
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
        SDL_RenderFillRect(renderer, &rotatingRect);

        // 绘制文本
        if (font)
        {
            SDL_Color textColor = {255, 255, 255, 255};
            const char *text = "UhandEngine Render Demo";
            SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, textColor);
            if (surface)
            {
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture)
                {
                    SDL_Rect dstRect = {50, 50, surface->w, surface->h};
                    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
                    SDL_DestroyTexture(texture);
                }
                SDL_FreeSurface(surface);
            }

            // 绘制第二行文本
            const char *text2 = "Press Q to quit";
            SDL_Surface *surface2 = TTF_RenderUTF8_Blended(font, text2, textColor);
            if (surface2)
            {
                SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
                if (texture2)
                {
                    SDL_Rect dstRect2 = {50, 550, surface2->w, surface2->h};
                    SDL_RenderCopy(renderer, texture2, NULL, &dstRect2);
                    SDL_DestroyTexture(texture2);
                }
                SDL_FreeSurface(surface2);
            }
        }

        // 渲染
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // 清理
    if (font)
    {
        TTF_CloseFont(font);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    printf("Render Example ended\n");
    return 0;
}
