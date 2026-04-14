/**
 * Tween Example
 *
 * 演示 Tween 补间动画系统的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Tween.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// 完成回调
void OnTweenComplete(Tween *tween)
{
    printf("Tween completed!\n");
}

// 更新回调
void OnTweenUpdate(Tween *tween, float progress)
{
    // 可以在这里添加额外逻辑
}

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Tween Example",
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

    printf("=== Tween Example ===\n\n");
    printf("Controls:\n");
    printf("- Space: Start/Restart tween\n");
    printf("- L: Toggle loop\n");
    printf("- Y: Toggle yoyo\n");
    printf("- 1-4: Change ease type\n");
    printf("- Q: Quit\n\n");

    // 矩形属性
    float rectX = 100.0f;
    float rectY = 300.0f;
    float rectScale = 1.0f;

    // 创建补间
    Tween *tween = Tween_Create();
    Tween_To(tween, &rectX, 700.0f, 2000, TWEEN_EASE_ELASTIC_OUT);
    Tween_SetOnComplete(tween, OnTweenComplete);
    Tween_SetOnUpdate(tween, OnTweenUpdate);

    // 当前缓动类型
    TweenEase currentEase = TWEEN_EASE_ELASTIC_OUT;
    const char *easeNames[] = {
        "Linear", "QuadIn", "QuadOut", "QuadInOut",
        "CubicIn", "CubicOut", "CubicInOut", "ElasticOut"
    };

    // 主循环
    int running = 1;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        int deltaTime = currentTime - lastTime;
        lastTime = currentTime;

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
                else if (event.key.keysym.sym == SDLK_SPACE)
                {
                    Tween_Restart(tween);
                    printf("Tween restarted\n");
                }
                else if (event.key.keysym.sym == SDLK_l)
                {
                    int loop = !tween->loop;
                    Tween_SetLoop(tween, loop);
                    printf("Loop: %s\n", loop ? "On" : "Off");
                }
                else if (event.key.keysym.sym == SDLK_y)
                {
                    int yoyo = !tween->yoyo;
                    Tween_SetYoyo(tween, yoyo);
                    printf("Yoyo: %s\n", yoyo ? "On" : "Off");
                }
                else if (event.key.keysym.sym >= SDLK_1 && event.key.keysym.sym <= SDLK_8)
                {
                    int index = event.key.keysym.sym - SDLK_1;
                    if (index >= 0 && index < 8)
                    {
                        currentEase = (TweenEase)index;
                        Tween_Destroy(tween);
                        tween = Tween_Create();
                        rectX = 100.0f;
                        Tween_To(tween, &rectX, 700.0f, 2000, currentEase);
                        printf("Ease: %s\n", easeNames[index]);
                    }
                }
            }
        }

        // 更新补间
        Tween_Update(tween, deltaTime);

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制矩形
        SDL_Rect rect = {(int)rectX, (int)rectY, (int)(64 * rectScale), (int)(64 * rectScale)};
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        // 显示信息
        printf("\rX: %.1f | Ease: %s | Loop: %s | Yoyo: %s",
               rectX,
               easeNames[currentEase],
               tween->loop ? "On" : "Off",
               tween->yoyo ? "On" : "Off");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    // 清理
    Tween_Destroy(tween);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Tween Example ended\n");
    return 0;
}
