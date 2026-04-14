/**
 * Input Example
 *
 * 演示输入系统的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/InputManager.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Input Example",
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

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("=== Input Example ===\n\n");
    printf("Controls:\n");
    printf("- Arrow keys: Move rectangle\n");
    printf("- Space: Change color\n");
    printf("- Mouse click: Show position\n");
    printf("- Q: Quit\n\n");

    // 创建输入管理器
    InputManager *input = InputManager_Create();

    // 矩形位置和颜色
    int rectX = 400, rectY = 300;
    SDL_Color rectColor = {100, 200, 255, 255};

    // 主循环
    int running = 1;
    SDL_Event event;

    while (running)
    {
        // 处理事件
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

            // 更新输入管理器
            InputManager_Update(input, &event);
        }

        // 更新输入状态
        InputManager_UpdateState(input);

        // 处理输入
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_UP))
        {
            rectY -= 5;
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_DOWN))
        {
            rectY += 5;
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_LEFT))
        {
            rectX -= 5;
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_RIGHT))
        {
            rectX += 5;
        }

        if (InputManager_IsKeyPressed(input, SDL_SCANCODE_SPACE))
        {
            rectColor.r = rand() % 256;
            rectColor.g = rand() % 256;
            rectColor.b = rand() % 256;
            printf("Color changed: (%d, %d, %d)\n", rectColor.r, rectColor.g, rectColor.b);
        }

        if (InputManager_IsMouseButtonPressed(input, MOUSE_BUTTON_LEFT))
        {
            int mx, my;
            InputManager_GetMousePosition(input, &mx, &my);
            printf("Mouse clicked at: (%d, %d)\n", mx, my);
        }

        // 获取鼠标滚轮
        int wheel = InputManager_GetMouseWheel(input);
        if (wheel != 0)
        {
            printf("Mouse wheel: %d\n", wheel);
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制矩形
        SDL_Rect rect = {rectX, rectY, 64, 64};
        SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        // 绘制鼠标位置
        int mx, my;
        InputManager_GetMousePosition(input, &mx, &my);
        SDL_Rect mouseRect = {mx, my, 8, 8};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &mouseRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // 清理
    InputManager_Destroy(input);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Input Example ended\n");
    return 0;
}
