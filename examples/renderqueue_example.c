/**
 * Render Queue Example
 *
 * 演示按 depth 排序的渲染队列
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/RenderQueue.h"
#include "../src/engine/gameObject.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// 自定义渲染函数
void customRender(GameObject *go, SDL_Renderer *renderer, void *context)
{
    if (!go || !renderer)
        return;

    // 根据 depth 使用不同颜色
    SDL_Color color;
    switch (go->depth % 5)
    {
        case 0: color = (SDL_Color){255, 100, 100, 255}; break; // 红色
        case 1: color = (SDL_Color){100, 255, 100, 255}; break; // 绿色
        case 2: color = (SDL_Color){100, 100, 255, 255}; break; // 蓝色
        case 3: color = (SDL_Color){255, 255, 100, 255}; break; // 黄色
        case 4: color = (SDL_Color){255, 100, 255, 255}; break; // 紫色
        default: color = (SDL_Color){255, 255, 255, 255}; break;
    }

    // 绘制矩形
    SDL_Rect rect = {(int)go->transform->x, (int)go->transform->y, 64, 64};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Render Queue Example",
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

    printf("=== Render Queue Example ===\n\n");

    // 创建渲染队列
    RenderQueue *queue = RenderQueue_Create();

    // 创建多个 GameObject，设置不同的 depth
    GameObject *obj1 = createGameObject("Object1");
    GameObject *obj2 = createGameObject("Object2");
    GameObject *obj3 = createGameObject("Object3");
    GameObject *obj4 = createGameObject("Object4");
    GameObject *obj5 = createGameObject("Object5");

    // 设置 depth（故意乱序）
    setDepth(obj1, 30);
    setDepth(obj2, 10);
    setDepth(obj3, 40);
    setDepth(obj4, 20);
    setDepth(obj5, 5);

    // 设置位置
    obj1->transform->setPosition(obj1->transform, 100, 100);
    obj2->transform->setPosition(obj2->transform, 200, 200);
    obj3->transform->setPosition(obj3->transform, 300, 300);
    obj4->transform->setPosition(obj4->transform, 400, 400);
    obj5->transform->setPosition(obj5->transform, 500, 500);

    // 设置渲染函数
    obj1->render = customRender;
    obj2->render = customRender;
    obj3->render = customRender;
    obj4->render = customRender;
    obj5->render = customRender;

    // 添加到队列
    RenderQueue_Add(queue, obj1);
    RenderQueue_Add(queue, obj2);
    RenderQueue_Add(queue, obj3);
    RenderQueue_Add(queue, obj4);
    RenderQueue_Add(queue, obj5);

    printf("Added 5 GameObjects with depths: 30, 10, 40, 20, 5\n");
    printf("Sorting by depth...\n");

    // 排序
    RenderQueue_Sort(queue);

    printf("Rendering in depth order (press Q to quit)\n\n");

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

        // 渲染队列
        RenderQueue_Render(queue, renderer, NULL);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // 清理
    RenderQueue_Destroy(queue);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Render Queue Example ended\n");
    return 0;
}
