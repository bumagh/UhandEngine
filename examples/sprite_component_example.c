/**
 * SpriteComponent Example
 *
 * 演示 SpriteComponent 的使用，支持纹理渲染和精灵表
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/AssetManager.h"
#include "../src/engine/SpriteComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SpriteComponent Example",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    printf("=== SpriteComponent Example ===\n\n");
    printf("Controls:\n");
    printf("- Space: Change sprite frame\n");
    printf("- F: Toggle flip X\n");
    printf("- UP/DOWN: Adjust alpha\n");
    printf("- Q: Quit\n\n");

    // 创建 AssetManager
    AssetManager *assetManager = AssetManager_Create(".");

    // 创建一个简单的纹理（如果没有图片文件）
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 64, 64, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 100, 200, 255));
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // 创建 GameObject
    GameObject *sprite = createGameObject("Sprite");
    sprite->transform->setPosition(sprite->transform, 368, 268);
    setDepth(sprite, 10);

    // 创建 SpriteComponent
    SpriteComponent *spriteComp = SpriteComponent_Create(texture, 64, 64);
    
    // 设置为精灵表（2x2 帧）
    SpriteComponent_SetFrameSize(spriteComp, 32, 32);
    SpriteComponent_SetFrame(spriteComp, 0);

    // 主循环
    int running = 1;
    SDL_Event event;
    int currentFrame = 0;
    float alpha = 1.0f;
    int flipX = 0;

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
                else if (event.key.keysym.sym == SDLK_SPACE)
                {
                    currentFrame = (currentFrame + 1) % 4;
                    SpriteComponent_SetFrame(spriteComp, currentFrame);
                    printf("Frame: %d\n", currentFrame);
                }
                else if (event.key.keysym.sym == SDLK_f)
                {
                    flipX = !flipX;
                    SpriteComponent_SetFlipX(spriteComp, flipX);
                    printf("Flip X: %s\n", flipX ? "On" : "Off");
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    alpha = (alpha + 0.1f) > 1.0f ? 1.0f : alpha + 0.1f;
                    SpriteComponent_SetAlpha(spriteComp, alpha);
                    printf("Alpha: %.1f\n", alpha);
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    alpha = (alpha - 0.1f) < 0.0f ? 0.0f : alpha - 0.1f;
                    SpriteComponent_SetAlpha(spriteComp, alpha);
                    printf("Alpha: %.1f\n", alpha);
                }
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 渲染 SpriteComponent
        SpriteComponent_Render(spriteComp, renderer, 
                              sprite->transform->x, 
                              sprite->transform->y,
                              sprite->transform->scaleX,
                              sprite->transform->scaleY,
                              sprite->transform->rotation);

        // 显示信息
        printf("\rFrame: %d | Alpha: %.1f | Flip X: %s",
               currentFrame, alpha, flipX ? "On" : "Off");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    // 清理
    SpriteComponent_Destroy(spriteComp);
    freeGameObject(sprite);
    SDL_DestroyTexture(texture);
    AssetManager_Destroy(assetManager);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    printf("SpriteComponent Example ended\n");
    return 0;
}
