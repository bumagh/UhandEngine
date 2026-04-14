/**
 * Text Example
 *
 * 演示如何使用 TextComponent 实际渲染文本
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/UI/TextComponent.h"
#include <SDL2/SDL.h>
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

    // 初始化 SDL_ttf
    if (TTF_Init() < 0)
    {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Text Example",
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
    TTF_Font *font = TTF_OpenFont("fzpix.ttf", 48);
    if (!font)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        printf("Will render rectangles instead of text\n");
    }
    else
    {
        printf("Font loaded successfully\n");
    }

    printf("=== Text Example ===\n\n");

    // 创建 TextComponent
    TextComponent *textComp1 = NULL;
    newTextComponent(&textComp1);

    SDL_Color color1 = {255, 255, 255, 255}; // 白色
    if (textComp1 && font)
    {
        textComp1->init(textComp1, renderer, font, "UhandEngine 文本渲染演示", color1);
        textComp1->dstRect.x = 50;
        textComp1->dstRect.y = 50;
        printf("Created TextComponent: %s\n", textComp1->base.name);
    }

    // 创建第二个 TextComponent
    TextComponent *textComp2 = NULL;
    newTextComponent(&textComp2);

    SDL_Color color2 = {255, 100, 100, 255}; // 红色
    if (textComp2 && font)
    {
        textComp2->init(textComp2, renderer, font, "按 Q 键退出", color2);
        textComp2->dstRect.x = 50;
        textComp2->dstRect.y = 550;
        printf("Created TextComponent: %s\n", textComp2->base.name);
    }

    // 创建第三个 TextComponent (大字体)
    TextComponent *textComp3 = NULL;
    newTextComponent(&textComp3);

    SDL_Color color3 = {100, 255, 255, 255}; // 青色
    if (textComp3 && font)
    {
        textComp3->init(textComp3, renderer, font, "你好，世界！", color3);
        textComp3->dstRect.x = 250;
        textComp3->dstRect.y = 250;
        printf("Created TextComponent: %s\n", textComp3->base.name);
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

        // 如果字体加载成功，绘制 TextComponent
        if (font)
        {
            if (textComp1 && textComp1->base.draw)
            {
                textComp1->base.draw((Component *)textComp1, renderer);
            }
            if (textComp2 && textComp2->base.draw)
            {
                textComp2->base.draw((Component *)textComp2, renderer);
            }
            if (textComp3 && textComp3->base.draw)
            {
                textComp3->base.draw((Component *)textComp3, renderer);
            }
        }
        else
        {
            // 字体加载失败，绘制彩色矩形作为备用
            SDL_Rect rect1 = {50, 50, 300, 50};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect1);

            SDL_Rect rect2 = {50, 550, 200, 40};
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
            SDL_RenderFillRect(renderer, &rect2);

            SDL_Rect rect3 = {250, 250, 200, 50};
            SDL_SetRenderDrawColor(renderer, 100, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect3);
        }

        // 渲染
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // 清理
    if (textComp1 && textComp1->base.free)
    {
        textComp1->base.free((Component *)textComp1);
    }
    if (textComp2 && textComp2->base.free)
    {
        textComp2->base.free((Component *)textComp2);
    }
    if (textComp3 && textComp3->base.free)
    {
        textComp3->base.free((Component *)textComp3);
    }

    if (font)
    {
        TTF_CloseFont(font);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    printf("Text Example ended\n");
    return 0;
}
