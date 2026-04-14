/**
 * Text Example
 *
 * 演示如何创建和使用 Text GameObject
 *
 * 注意：当前 Text 是简化实现，需要集成 TextComponent
 * 本示例展示 API 设计，实际运行需要先完成 TextComponent 集成
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void text_example()
{
    printf("=== Text Example ===\n\n");

    // 假设已加载字体
    TTF_Font *font = NULL; // 占位符
    SDL_Color color = {255, 255, 255, 255};

    // 创建一个 Text GameObject
    GameObject *text = createText("ScoreText", "Score: 100", font, color, 50.0f, 50.0f);

    if (text)
    {
        printf("Created Text: %s\n", text->name);
        printf("Type: %d (GAMEOBJECT_TYPE_TEXT = %d)\n", text->type, GAMEOBJECT_TYPE_TEXT);

        if (text->transform)
        {
            printf("Position: (%.2f, %.2f)\n", text->transform->x, text->transform->y);
        }

        printf("Visible: %d\n", text->visible);
        printf("Active: %d\n", text->active);
        printf("Depth: %d\n\n", text->depth);

        // TODO: 需要集成 TextComponent 才能实际渲染
        printf("Note: 实际渲染需要先集成 TextComponent\n");

        // 释放 (在实际使用中应该通过 Scene 管理)
        // text->free(text);
    }
    else
    {
        printf("Failed to create Text\n");
    }
}

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

    // 创建窗口
    SDL_Window *window = SDL_CreateWindow("Text Example",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 运行示例
    text_example();

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

        // 渲染
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // ~60 FPS
    }

    // 清理
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
