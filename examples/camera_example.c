/**
 * Camera Example
 *
 * 演示摄像机系统的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Camera.h"
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

    SDL_Window *window = SDL_CreateWindow("Camera Example",
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

    printf("=== Camera Example ===\n\n");
    printf("Controls:\n");
    printf("- WASD/Arrow keys: Move camera\n");
    printf("- +/- or Mouse wheel: Zoom in/out\n");
    printf("- R: Reset camera\n");
    printf("- Q: Quit\n\n");

    // 创建摄像机
    Camera *camera = Camera_Create(800, 600);
    InputManager *input = InputManager_Create();

    // 世界中的对象位置
    float objectX = 0.0f, objectY = 0.0f;

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

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        // 处理输入
        float moveSpeed = 5.0f;
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_W) || InputManager_IsKeyDown(input, SDL_SCANCODE_UP))
        {
            Camera_Move(camera, 0, -moveSpeed);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_S) || InputManager_IsKeyDown(input, SDL_SCANCODE_DOWN))
        {
            Camera_Move(camera, 0, moveSpeed);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_A) || InputManager_IsKeyDown(input, SDL_SCANCODE_LEFT))
        {
            Camera_Move(camera, -moveSpeed, 0);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_D) || InputManager_IsKeyDown(input, SDL_SCANCODE_RIGHT))
        {
            Camera_Move(camera, moveSpeed, 0);
        }

        if (InputManager_IsKeyPressed(input, SDL_SCANCODE_R))
        {
            Camera_SetPosition(camera, 0, 0);
            Camera_SetZoom(camera, 1.0f);
            Camera_SetRotation(camera, 0.0f);
            printf("Camera reset\n");
        }

        if (InputManager_IsKeyDown(input, SDL_SCANCODE_EQUALS) || InputManager_IsKeyDown(input, SDL_SCANCODE_KP_PLUS))
        {
            Camera_Zoom(camera, 0.02f);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_MINUS) || InputManager_IsKeyDown(input, SDL_SCANCODE_KP_MINUS))
        {
            Camera_Zoom(camera, -0.02f);
        }

        int wheel = InputManager_GetMouseWheel(input);
        if (wheel != 0)
        {
            Camera_Zoom(camera, wheel * 0.1f);
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制网格（世界坐标）
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        for (int x = -1000; x <= 1000; x += 100)
        {
            int screenX1, screenY1, screenX2, screenY2;
            Camera_WorldToScreen(camera, x, -1000, &screenX1, &screenY1);
            Camera_WorldToScreen(camera, x, 1000, &screenX2, &screenY2);
            SDL_RenderDrawLine(renderer, screenX1, screenY1, screenX2, screenY2);
        }
        for (int y = -1000; y <= 1000; y += 100)
        {
            int screenX1, screenY1, screenX2, screenY2;
            Camera_WorldToScreen(camera, -1000, y, &screenX1, &screenY1);
            Camera_WorldToScreen(camera, 1000, y, &screenX2, &screenY2);
            SDL_RenderDrawLine(renderer, screenX1, screenY1, screenX2, screenY2);
        }

        // 绘制世界原点标记
        int originX, originY;
        Camera_WorldToScreen(camera, 0, 0, &originX, &originY);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, originX - 20, originY, originX + 20, originY);
        SDL_RenderDrawLine(renderer, originX, originY - 20, originX, originY + 20);

        // 绘制对象
        int objScreenX, objScreenY;
        Camera_WorldToScreen(camera, objectX, objectY, &objScreenX, &objScreenY);
        int size = (int)(64 * Camera_GetZoom(camera));
        SDL_Rect objRect = {objScreenX - size/2, objScreenY - size/2, size, size};
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_RenderFillRect(renderer, &objRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &objRect);

        // 显示摄像机信息
        float camX, camY;
        Camera_GetPosition(camera, &camX, &camY);
        printf("\rCamera: (%.1f, %.1f) Zoom: %.2f", camX, camY, Camera_GetZoom(camera));

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    Camera_Destroy(camera);
    InputManager_Destroy(input);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Camera Example ended\n");
    return 0;
}
