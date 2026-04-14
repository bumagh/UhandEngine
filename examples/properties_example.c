/**
 * Properties Example
 *
 * 演示 GameObject 的 visible、active、depth 属性的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <SDL2/SDL.h>
#include <stdio.h>

void properties_example()
{
    printf("=== Properties Example ===\n\n");

    // 创建多个 GameObject
    GameObject *obj1 = createGameObject("Background");
    GameObject *obj2 = createGameObject("Player");
    GameObject *obj3 = createGameObject("UIOverlay");
    GameObject *obj4 = createGameObject("HiddenObject");

    if (obj1 && obj2 && obj3 && obj4)
    {
        // 设置不同的 depth (渲染层级)
        setDepth(obj1, 0);   // 背景在最底层
        setDepth(obj2, 10);  // 玩家在中间
        setDepth(obj3, 20);  // UI 在最上层
        setDepth(obj4, 15);  // 隐藏对象在玩家和UI之间

        printf("=== Depth Settings ===\n");
        printf("%s depth: %d (background)\n", obj1->name, obj1->depth);
        printf("%s depth: %d (player)\n", obj2->name, obj2->depth);
        printf("%s depth: %d (UI overlay)\n", obj3->name, obj3->depth);
        printf("%s depth: %d (hidden)\n", obj4->name, obj4->depth);
        printf("\n");

        // 设置可见性
        printf("=== Visibility Settings ===\n");
        printf("%s visible: %d (default visible)\n", obj1->name, obj1->visible);
        printf("%s visible: %d (default visible)\n", obj2->name, obj2->visible);
        printf("%s visible: %d (default visible)\n", obj3->name, obj3->visible);

        // 隐藏一个对象
        setVisible(obj4, 0);
        printf("%s visible: %d (set to invisible)\n", obj4->name, obj4->visible);
        printf("\n");

        // 设置激活状态
        printf("=== Active Settings ===\n");
        printf("%s active: %d (default active)\n", obj1->name, obj1->active);
        printf("%s active: %d (default active)\n", obj2->name, obj2->active);
        printf("%s active: %d (default active)\n", obj3->name, obj3->active);

        // 停用一个对象（不参与 Update）
        setActive(obj4, 0);
        printf("%s active: %d (set to inactive)\n", obj4->name, obj4->active);
        printf("\n");

        // 设置位置
        printf("=== Position Settings ===\n");
        if (obj1->transform)
        {
            obj1->transform->setPosition(obj1->transform, 0.0f, 0.0f);
            printf("%s position: (%.2f, %.2f)\n", obj1->name, obj1->transform->x, obj1->transform->y);
        }
        if (obj2->transform)
        {
            obj2->transform->setPosition(obj2->transform, 100.0f, 100.0f);
            printf("%s position: (%.2f, %.2f)\n", obj2->name, obj2->transform->x, obj2->transform->y);
        }
        if (obj3->transform)
        {
            obj3->transform->setPosition(obj3->transform, 0.0f, 0.0f);
            printf("%s position: (%.2f, %.2f)\n", obj3->name, obj3->transform->x, obj3->transform->y);
        }
        printf("\n");

        // 演示动态修改属性
        printf("=== Dynamic Property Changes ===\n");
        printf("Making player invisible...\n");
        setVisible(obj2, 0);
        printf("%s visible: %d\n", obj2->name, obj2->visible);

        printf("Pausing background updates...\n");
        setActive(obj1, 0);
        printf("%s active: %d\n", obj1->name, obj1->active);

        printf("Bringing UI to front...\n");
        setDepth(obj3, 100);
        printf("%s depth: %d\n", obj3->name, obj3->depth);
        printf("\n");

        printf("=== Summary ===\n");
        printf("visible: 控制对象是否渲染\n");
        printf("active: 控制对象是否参与 Update\n");
        printf("depth: 控制渲染顺序（值越大越靠前）\n");
        printf("\n");
        printf("Note: 实际使用中应该通过 Scene 管理 GameObject\n");
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
    SDL_Window *window = SDL_CreateWindow("Properties Example",
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
    properties_example();

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
