/**
 * Simple Game Example
 *
 * 一个简单的收集游戏，展示引擎系统的实际应用
 * 玩家控制角色收集物品，避开敌人
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/InputManager.h"
#include "../src/engine/Camera.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// 游戏状态
typedef struct {
    int score;
    int gameOver;
    int win;
} GameState;

// 自定义渲染函数
void renderPlayer(GameObject *go, SDL_Renderer *renderer, void *context)
{
    SDL_Rect rect = {(int)go->transform->x, (int)go->transform->y, 40, 40};
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void renderEnemy(GameObject *go, SDL_Renderer *renderer, void *context)
{
    SDL_Rect rect = {(int)go->transform->x, (int)go->transform->y, 40, 40};
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);
}

void renderItem(GameObject *go, SDL_Renderer *renderer, void *context)
{
    SDL_Rect rect = {(int)go->transform->x, (int)go->transform->y, 20, 20};
    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
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

    SDL_Window *window = SDL_CreateWindow("Simple Game Example",
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

    printf("=== Simple Game Example ===\n\n");
    printf("Controls:\n");
    printf("- WASD/Arrows: Move player\n");
    printf("- R: Restart game\n");
    printf("- Q: Quit\n");
    printf("\nObjective: Collect yellow items, avoid red enemies!\n\n");

    srand(time(NULL));

    // 创建系统
    Scene *scene = Scene_Create();
    InputManager *input = InputManager_Create();

    // 游戏状态
    GameState gameState = {0, 0, 0};

    // 创建玩家
    GameObject *player = createGameObject("Player");
    player->transform->setPosition(player->transform, 400, 300);
    player->render = renderPlayer;
    setDepth(player, 10);
    Scene_AddGameObject(scene, player);

    // 创建敌人
    GameObject *enemies[5];
    for (int i = 0; i < 5; i++)
    {
        enemies[i] = createGameObject("Enemy");
        enemies[i]->transform->setPosition(enemies[i]->transform, 
                                         rand() % 700 + 50, 
                                         rand() % 500 + 50);
        enemies[i]->render = renderEnemy;
        setDepth(enemies[i], 5);
        Scene_AddGameObject(scene, enemies[i]);
    }

    // 创建物品
    GameObject *items[10];
    for (int i = 0; i < 10; i++)
    {
        items[i] = createGameObject("Item");
        items[i]->transform->setPosition(items[i]->transform,
                                       rand() % 700 + 50,
                                       rand() % 500 + 50);
        items[i]->render = renderItem;
        setDepth(items[i], 15);
        Scene_AddGameObject(scene, items[i]);
    }

    // 主循环
    int running = 1;
    SDL_Event event;
    float playerSpeed = 5.0f;
    float enemySpeed = 2.0f;

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
                else if (event.key.keysym.sym == SDLK_r && gameState.gameOver)
                {
                    // 重置游戏
                    gameState.score = 0;
                    gameState.gameOver = 0;
                    gameState.win = 0;
                    player->transform->setPosition(player->transform, 400, 300);
                    for (int i = 0; i < 5; i++)
                    {
                        enemies[i]->transform->setPosition(enemies[i]->transform,
                                                         rand() % 700 + 50,
                                                         rand() % 500 + 50);
                        enemies[i]->visible = 1;
                    }
                    for (int i = 0; i < 10; i++)
                    {
                        items[i]->transform->setPosition(items[i]->transform,
                                                           rand() % 700 + 50,
                                                           rand() % 500 + 50);
                        items[i]->visible = 1;
                    }
                    printf("Game restarted!\n");
                }
            }

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        if (!gameState.gameOver)
        {
            // 玩家移动
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_W) || InputManager_IsKeyDown(input, SDL_SCANCODE_UP))
            {
                player->transform->y -= playerSpeed;
            }
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_S) || InputManager_IsKeyDown(input, SDL_SCANCODE_DOWN))
            {
                player->transform->y += playerSpeed;
            }
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_A) || InputManager_IsKeyDown(input, SDL_SCANCODE_LEFT))
            {
                player->transform->x -= playerSpeed;
            }
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_D) || InputManager_IsKeyDown(input, SDL_SCANCODE_RIGHT))
            {
                player->transform->x += playerSpeed;
            }

            // 限制玩家在屏幕内
            if (player->transform->x < 0) player->transform->x = 0;
            if (player->transform->x > 760) player->transform->x = 760;
            if (player->transform->y < 0) player->transform->y = 0;
            if (player->transform->y > 560) player->transform->y = 560;

            // 敌人移动（简单追踪）
            for (int i = 0; i < 5; i++)
            {
                if (!enemies[i]->visible) continue;

                float dx = player->transform->x - enemies[i]->transform->x;
                float dy = player->transform->y - enemies[i]->transform->y;
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist > 0)
                {
                    enemies[i]->transform->x += (dx / dist) * enemySpeed;
                    enemies[i]->transform->y += (dy / dist) * enemySpeed;
                }

                // 碰撞检测（玩家 vs 敌人）
                if (dist < 40)
                {
                    gameState.gameOver = 1;
                    printf("Game Over! Score: %d\n", gameState.score);
                }
            }

            // 物品收集
            int itemsCollected = 0;
            for (int i = 0; i < 10; i++)
            {
                if (!items[i]->visible) continue;

                float dx = player->transform->x - items[i]->transform->x;
                float dy = player->transform->y - items[i]->transform->y;
                float dist = sqrtf(dx*dx + dy*dy);

                if (dist < 30)
                {
                    items[i]->visible = 0;
                    gameState.score += 10;
                    printf("Item collected! Score: %d\n", gameState.score);
                }
                else
                {
                    itemsCollected++;
                }
            }

            // 胜利条件
            if (itemsCollected == 0)
            {
                gameState.gameOver = 1;
                gameState.win = 1;
                printf("You Win! Final Score: %d\n", gameState.score);
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 渲染所有对象
        RenderQueue *queue = scene->renderQueue;
        RenderQueue_Sort(queue);

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

        // 显示分数
        printf("\rScore: %d | Items: %d/10 | %s",
               gameState.score,
               10 - (gameState.score / 10),
               gameState.gameOver ? (gameState.win ? "WIN!" : "GAME OVER") : "Playing");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    // 清理
    InputManager_Destroy(input);
    Scene_Destroy(scene);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Simple Game Example ended\n");
    return 0;
}
