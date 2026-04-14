/**
 * Animation Example
 *
 * 演示动画系统的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Animation.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// 创建纯色纹理（用于模拟动画帧）
SDL_Texture *CreateColorTexture(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 64, 64, 32, 0, 0, 0, 0);
    if (!surface)
        return NULL;

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, color.r, color.g, color.b));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Animation Example",
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

    printf("=== Animation Example ===\n\n");
    printf("Controls:\n");
    printf("- Space: Pause/Resume\n");
    printf("- R: Restart\n");
    printf("- S: Change speed\n");
    printf("- Q: Quit\n\n");

    // 创建动画帧（用不同颜色的矩形模拟）
    SDL_Texture *frame1 = CreateColorTexture(renderer, (SDL_Color){255, 100, 100, 255});
    SDL_Texture *frame2 = CreateColorTexture(renderer, (SDL_Color){100, 255, 100, 255});
    SDL_Texture *frame3 = CreateColorTexture(renderer, (SDL_Color){100, 100, 255, 255});
    SDL_Texture *frame4 = CreateColorTexture(renderer, (SDL_Color){255, 255, 100, 255});

    // 创建动画
    Animation *animation = Animation_Create("color_cycle", 64, 64, 1);
    Animation_AddFrame(animation, frame1, 200);  // 200ms
    Animation_AddFrame(animation, frame2, 200);
    Animation_AddFrame(animation, frame3, 200);
    Animation_AddFrame(animation, frame4, 200);

    // 创建动画播放器
    Animator *animator = Animator_Create();
    Animator_Play(animator, animation);

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
                    if (animator->playing)
                    {
                        Animator_Pause(animator);
                        printf("Animation paused\n");
                    }
                    else
                    {
                        Animator_Resume(animator);
                        printf("Animation resumed\n");
                    }
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    Animator_Play(animator, animation);
                    printf("Animation restarted\n");
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    static float speeds[] = {0.5f, 1.0f, 2.0f, 4.0f};
                    static int speedIndex = 1;
                    speedIndex = (speedIndex + 1) % 4;
                    Animator_SetSpeed(animator, speeds[speedIndex]);
                    printf("Speed changed to %.1fx\n", speeds[speedIndex]);
                }
            }
        }

        // 更新动画
        Animator_Update(animator, deltaTime);

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 获取当前帧并绘制
        SDL_Texture *currentTexture = Animator_GetCurrentTexture(animator);
        if (currentTexture)
        {
            SDL_Rect destRect = {368, 268, 64, 64}; // 居中
            SDL_RenderCopy(renderer, currentTexture, NULL, &destRect);
        }

        // 显示信息
        printf("\rFrame: %d/%d | Speed: %.1fx | Playing: %s",
               animator->currentFrame + 1,
               animation->frameCount,
               (double)animator->speed,
               animator->playing ? "Yes" : "No");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    // 清理
    Animator_Destroy(animator);
    Animation_Destroy(animation);

    SDL_DestroyTexture(frame1);
    SDL_DestroyTexture(frame2);
    SDL_DestroyTexture(frame3);
    SDL_DestroyTexture(frame4);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Animation Example ended\n");
    return 0;
}
