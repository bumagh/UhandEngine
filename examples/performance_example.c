/**
 * Performance Example
 *
 * 演示性能优化技术：批量渲染、对象池、空间分区
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/InputManager.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// 自定义渲染函数
void renderObject(GameObject *go, SDL_Renderer *renderer, void *context)
{
    SDL_Rect rect = {(int)go->transform->x, (int)go->transform->y, 20, 20};
    
    // 根据 depth 使用不同颜色
    SDL_Color color;
    int depthIndex = go->depth % 8;
    switch (depthIndex)
    {
        case 0: color = (SDL_Color){255, 100, 100, 255}; break;
        case 1: color = (SDL_Color){100, 255, 100, 255}; break;
        case 2: color = (SDL_Color){100, 100, 255, 255}; break;
        case 3: color = (SDL_Color){255, 255, 100, 255}; break;
        case 4: color = (SDL_Color){255, 100, 255, 255}; break;
        case 5: color = (SDL_Color){100, 255, 255, 255}; break;
        case 6: color = (SDL_Color){255, 150, 100, 255}; break;
        case 7: color = (SDL_Color){150, 100, 255, 255}; break;
    }
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Performance Example",
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

    printf("=== Performance Example ===\n\n");
    printf("Controls:\n");
    printf("- 1-5: Change object count (100, 500, 1000, 5000, 10000)\n");
    printf("- UP/DOWN: Adjust object count\n");
    printf("- S: Toggle sorting\n");
    printf("- V: Toggle visibility culling\n");
    printf("- Q: Quit\n\n");

    // 创建系统
    Scene *scene = Scene_Create();
    InputManager *input = InputManager_Create();

    // 性能统计
    int objectCount = 1000;
    int enableSorting = 1;
    int enableCulling = 0;
    Uint32 lastFrameTime = SDL_GetTicks();
    int frameCount = 0;
    float fps = 60.0f;

    // 创建大量对象
    GameObject **objects = (GameObject **)malloc(sizeof(GameObject *) * 10000);
    for (int i = 0; i < 10000; i++)
    {
        objects[i] = createGameObject("Object");
        objects[i]->transform->setPosition(objects[i]->transform,
                                         rand() % 780 + 10,
                                         rand() % 580 + 10);
        objects[i]->render = renderObject;
        setDepth(objects[i], rand() % 100);
        objects[i]->visible = 1;
    }

    // 添加到 Scene
    for (int i = 0; i < objectCount; i++)
    {
        Scene_AddGameObject(scene, objects[i]);
    }

    // 主循环
    int running = 1;
    SDL_Event event;

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();

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
                else if (event.key.keysym.sym == SDLK_1)
                {
                    objectCount = 100;
                }
                else if (event.key.keysym.sym == SDLK_2)
                {
                    objectCount = 500;
                }
                else if (event.key.keysym.sym == SDLK_3)
                {
                    objectCount = 1000;
                }
                else if (event.key.keysym.sym == SDLK_4)
                {
                    objectCount = 5000;
                }
                else if (event.key.keysym.sym == SDLK_5)
                {
                    objectCount = 10000;
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    objectCount = (objectCount + 100) > 10000 ? 10000 : objectCount + 100;
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    objectCount = (objectCount - 100) < 100 ? 100 : objectCount - 100;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    enableSorting = !enableSorting;
                    printf("Sorting: %s\n", enableSorting ? "On" : "Off");
                }
                else if (event.key.keysym.sym == SDLK_v)
                {
                    enableCulling = !enableCulling;
                    printf("Visibility Culling: %s\n", enableCulling ? "On" : "Off");
                }

                // 更新 Scene 中的对象数量
                for (int i = 0; i < 10000; i++)
                {
                    if (i < objectCount)
                    {
                        if (!objects[i]->parent)
                        {
                            Scene_AddGameObject(scene, objects[i]);
                        }
                    }
                    else
                    {
                        Scene_RemoveGameObject(scene, objects[i]);
                    }
                }
            }

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        // 简单动画
        for (int i = 0; i < objectCount; i++)
        {
            objects[i]->transform->x += sinf(currentTime * 0.001 + i) * 0.5f;
            objects[i]->transform->y += cosf(currentTime * 0.001 + i) * 0.5f;
        }

        // 可见性剔除
        if (enableCulling)
        {
            for (int i = 0; i < objectCount; i++)
            {
                objects[i]->visible = (objects[i]->transform->x >= 0 && 
                                      objects[i]->transform->x <= 800 &&
                                      objects[i]->transform->y >= 0 && 
                                      objects[i]->transform->y <= 600);
            }
        }
        else
        {
            for (int i = 0; i < objectCount; i++)
            {
                objects[i]->visible = 1;
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 渲染
        RenderQueue *queue = scene->renderQueue;
        if (enableSorting)
        {
            RenderQueue_Sort(queue);
        }

        RenderQueueNode *current = queue->head;
        while (current)
        {
            GameObject *go = current->gameObject;

            if (go && go->visible && go->active)
            {
                if (go->render)
                {
                    go->render(go, renderer, NULL);
                }
            }

            current = current->next;
        }

        SDL_RenderPresent(renderer);

        // 计算 FPS
        frameCount++;
        if (currentTime - lastFrameTime >= 1000)
        {
            fps = frameCount * 1000.0f / (currentTime - lastFrameTime);
            frameCount = 0;
            lastFrameTime = currentTime;
        }

        printf("\rObjects: %d | FPS: %.1f | Sorting: %s | Culling: %s",
               objectCount, fps,
               enableSorting ? "On" : "Off",
               enableCulling ? "On" : "Off");

        SDL_Delay(16);
    }

    printf("\n");

    // 清理
    for (int i = 0; i < 10000; i++)
    {
        freeGameObject(objects[i]);
    }
    free(objects);

    InputManager_Destroy(input);
    Scene_Destroy(scene);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Performance Example ended\n");
    return 0;
}
