/**
 * Integration Example
 *
 * 演示所有系统的集成使用：Scene, GameObject, Transform, Input, Camera, Tween
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/InputManager.h"
#include "../src/engine/Camera.h"
#include "../src/engine/Tween.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// 自定义渲染函数
void integrationRender(GameObject *go, SDL_Renderer *renderer, void *context)
{
    if (!go || !renderer)
        return;

    // 根据 depth 使用不同颜色
    SDL_Color color;
    switch (go->depth % 5)
    {
        case 0: color = (SDL_Color){255, 100, 100, 255}; break;
        case 1: color = (SDL_Color){100, 255, 100, 255}; break;
        case 2: color = (SDL_Color){100, 100, 255, 255}; break;
        case 3: color = (SDL_Color){255, 255, 100, 255}; break;
        case 4: color = (SDL_Color){255, 100, 255, 255}; break;
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

    SDL_Window *window = SDL_CreateWindow("Integration Example",
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

    printf("=== Integration Example ===\n\n");
    printf("Systems integrated:\n");
    printf("- Scene: GameObject lifecycle\n");
    printf("- GameObject: Entity management\n");
    printf("- Transform: 2D position\n");
    printf("- InputManager: Keyboard/Mouse\n");
    printf("- Camera: Viewport control\n");
    printf("- Tween: Animation\n\n");
    printf("Controls:\n");
    printf("- WASD/Arrows: Move camera\n");
    printf("- +/-: Zoom\n");
    printf("- Space: Animate player\n");
    printf("- Q: Quit\n\n");

    // 创建系统
    Scene *scene = Scene_Create();
    InputManager *input = InputManager_Create();
    Camera *camera = Camera_Create(800, 600);

    // 创建 GameObject
    GameObject *player = createGameObject("Player");
    GameObject *enemy = createGameObject("Enemy");
    GameObject *item = createGameObject("Item");

    // 设置属性
    setDepth(player, 10);
    setDepth(enemy, 5);
    setDepth(item, 15);

    // 设置位置
    player->transform->setPosition(player->transform, 100, 300);
    enemy->transform->setPosition(enemy->transform, 400, 300);
    item->transform->setPosition(item->transform, 700, 300);

    // 设置渲染函数
    player->render = integrationRender;
    enemy->render = integrationRender;
    item->render = integrationRender;

    // 添加到 Scene
    Scene_AddGameObject(scene, player);
    Scene_AddGameObject(scene, enemy);
    Scene_AddGameObject(scene, item);

    // 创建 Tween
    Tween *playerTween = Tween_Create();
    Tween_SetLoop(playerTween, 1);
    Tween_SetYoyo(playerTween, 1);

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
                    Tween_Restart(playerTween);
                    printf("Player animation started\n");
                }
            }

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        // 处理输入 - 相机移动
        float camSpeed = 5.0f;
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_W) || InputManager_IsKeyDown(input, SDL_SCANCODE_UP))
        {
            Camera_Move(camera, 0, -camSpeed);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_S) || InputManager_IsKeyDown(input, SDL_SCANCODE_DOWN))
        {
            Camera_Move(camera, 0, camSpeed);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_A) || InputManager_IsKeyDown(input, SDL_SCANCODE_LEFT))
        {
            Camera_Move(camera, -camSpeed, 0);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_D) || InputManager_IsKeyDown(input, SDL_SCANCODE_RIGHT))
        {
            Camera_Move(camera, camSpeed, 0);
        }

        // 缩放
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_EQUALS) || InputManager_IsKeyDown(input, SDL_SCANCODE_KP_PLUS))
        {
            Camera_Zoom(camera, 0.02f);
        }
        if (InputManager_IsKeyDown(input, SDL_SCANCODE_MINUS) || InputManager_IsKeyDown(input, SDL_SCANCODE_KP_MINUS))
        {
            Camera_Zoom(camera, -0.02f);
        }

        // 更新 Tween
        Tween_Update(playerTween, deltaTime);

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

        // 使用 Scene 渲染（带坐标转换）
        RenderQueue *queue = scene->renderQueue;
        RenderQueue_Sort(queue);

        RenderQueueNode *current = queue->head;
        while (current)
        {
            GameObject *go = current->gameObject;

            if (go && go->visible && go->active)
            {
                int screenX, screenY;
                Camera_WorldToScreen(camera, go->transform->x, go->transform->y, &screenX, &screenY);

                // 临时修改位置用于渲染
                float originalX = go->transform->x;
                float originalY = go->transform->y;
                go->transform->x = screenX;
                go->transform->y = screenY;

                if (go->render)
                {
                    go->render(go, renderer, NULL);
                }

                // 恢复位置
                go->transform->x = originalX;
                go->transform->y = originalY;
            }

            current = current->next;
        }

        // 显示信息
        float camX, camY;
        Camera_GetPosition(camera, &camX, &camY);
        printf("\rCamera: (%.1f, %.1f) Zoom: %.2f | Player: (%.1f, %.1f)",
               camX, camY, Camera_GetZoom(camera),
               player->transform->x, player->transform->y);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    // 清理
    Tween_Destroy(playerTween);
    Camera_Destroy(camera);
    InputManager_Destroy(input);
    Scene_Destroy(scene);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Integration Example ended\n");
    return 0;
}
