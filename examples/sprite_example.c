/**
 * Sprite Example
 *
 * 演示如何创建和使用 Sprite GameObject
 *
 * 注意：当前 Sprite 是简化实现，需要资源管理系统支持纹理加载
 * 本示例展示 API 设计，实际运行需要先实现资源管理
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void sprite_example()
{
    printf("=== Sprite Example ===\n\n");

    // 创建一个 Sprite GameObject
    GameObject *sprite = createSprite("PlayerSprite", "assets/player.png", 100.0f, 200.0f);

    if (sprite)
    {
        printf("Created Sprite: %s\n", sprite->name);
        printf("Type: %d (GAMEOBJECT_TYPE_SPRITE = %d)\n", sprite->type, GAMEOBJECT_TYPE_SPRITE);

        if (sprite->transform)
        {
            printf("Position: (%.2f, %.2f)\n", sprite->transform->x, sprite->transform->y);

            // 设置旋转
            sprite->transform->setRotation(sprite->transform, 1.5708f); // 90度
            printf("Rotation: %.4f rad\n", sprite->transform->rotation);

            // 设置缩放
            sprite->transform->setScale(sprite->transform, 2.0f, 2.0f);
            printf("Scale: (%.2f, %.2f)\n", sprite->transform->scaleX, sprite->transform->scaleY);

            // 设置原点
            sprite->transform->setOrigin(sprite->transform, 0.5f, 0.5f);
            printf("Origin: (%.2f, %.2f)\n", sprite->transform->originX, sprite->transform->originY);
        }

        printf("Visible: %d\n", sprite->visible);
        printf("Active: %d\n", sprite->active);
        printf("Depth: %d\n\n", sprite->depth);

        // TODO: 需要实现 SpriteComponent 和资源管理才能实际渲染
        printf("Note: 实际渲染需要先实现 SpriteComponent 和资源管理系统\n");

        // 释放 (在实际使用中应该通过 Scene 管理)
        // sprite->free(sprite);
    }
    else
    {
        printf("Failed to create Sprite\n");
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
    SDL_Window *window = SDL_CreateWindow("Sprite Example",
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
    sprite_example();

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

        // 绘制 sprite（用矩形表示）
        SDL_Rect spriteRect = {100, 200, 64, 64};
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_RenderFillRect(renderer, &spriteRect);

        // 绘制边框
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &spriteRect);

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

