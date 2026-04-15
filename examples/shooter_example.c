/**
 * Shooter Game Example
 *
 * 一个简单的射击游戏，展示子弹管理、敌人AI、分数系统
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/InputManager.h"
#include "../src/engine/Physics.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BULLETS 50
#define MAX_ENEMIES 10

typedef struct {
    float x, y;
    float vx, vy;
    float width, height;
    int active;
    SDL_Color color;
} Bullet;

typedef struct {
    float x, y;
    float width, height;
    float vx, vy;
    int active;
    int health;
    SDL_Color color;
} Enemy;

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Shooter Game Example",
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

    printf("=== Shooter Game Example ===\n\n");
    printf("Controls:\n");
    printf("- WASD/Arrows: Move player\n");
    printf("- Space: Shoot\n");
    printf("- R: Restart\n");
    printf("- Q: Quit\n");
    printf("\nObjective: Shoot enemies, avoid collisions!\n\n");

    srand(time(NULL));

    InputManager *input = InputManager_Create();

    // 玩家
    float playerX = 400, playerY = 500;
    float playerWidth = 40, playerHeight = 40;
    float playerSpeed = 5.0f;
    Uint32 lastShotTime = 0;
    Uint32 shootCooldown = 200; // 毫秒

    // 子弹
    Bullet bullets[MAX_BULLETS];
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = 0;
    }

    // 敌人
    Enemy enemies[MAX_ENEMIES];
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i].active = 0;
    }

    int score = 0;
    int gameOver = 0;
    Uint32 lastEnemySpawn = 0;
    Uint32 enemySpawnInterval = 2000;

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
                else if (event.key.keysym.sym == SDLK_r)
                {
                    // 重置游戏
                    playerX = 400;
                    playerY = 500;
                    score = 0;
                    gameOver = 0;
                    for (int i = 0; i < MAX_BULLETS; i++)
                    {
                        bullets[i].active = 0;
                    }
                    for (int i = 0; i < MAX_ENEMIES; i++)
                    {
                        enemies[i].active = 0;
                    }
                    printf("Game restarted!\n");
                }
            }

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        if (!gameOver)
        {
            // 玩家移动
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_W) || InputManager_IsKeyDown(input, SDL_SCANCODE_UP))
                playerY -= playerSpeed;
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_S) || InputManager_IsKeyDown(input, SDL_SCANCODE_DOWN))
                playerY += playerSpeed;
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_A) || InputManager_IsKeyDown(input, SDL_SCANCODE_LEFT))
                playerX -= playerSpeed;
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_D) || InputManager_IsKeyDown(input, SDL_SCANCODE_RIGHT))
                playerX += playerSpeed;

            // 限制玩家在屏幕内
            if (playerX < 0) playerX = 0;
            if (playerX > 760) playerX = 760;
            if (playerY < 0) playerY = 0;
            if (playerY > 560) playerY = 560;

            // 射击
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_SPACE))
            {
                if (currentTime - lastShotTime >= shootCooldown)
                {
                    // 找到可用的子弹
                    for (int i = 0; i < MAX_BULLETS; i++)
                    {
                        if (!bullets[i].active)
                        {
                            bullets[i].x = playerX + playerWidth / 2 - 2;
                            bullets[i].y = playerY;
                            bullets[i].vx = 0;
                            bullets[i].vy = -10;
                            bullets[i].width = 4;
                            bullets[i].height = 10;
                            bullets[i].active = 1;
                            bullets[i].color = (SDL_Color){255, 255, 100, 255};
                            lastShotTime = currentTime;
                            break;
                        }
                    }
                }
            }

            // 生成敌人
            if (currentTime - lastEnemySpawn >= enemySpawnInterval)
            {
                for (int i = 0; i < MAX_ENEMIES; i++)
                {
                    if (!enemies[i].active)
                    {
                        enemies[i].x = rand() % 700 + 50;
                        enemies[i].y = -50;
                        enemies[i].width = 30;
                        enemies[i].height = 30;
                        enemies[i].vx = (rand() % 3 - 1) * 2.0f;
                        enemies[i].vy = 2.0f + rand() % 2;
                        enemies[i].active = 1;
                        enemies[i].health = 1;
                        enemies[i].color = (SDL_Color){255, 100, 100, 255};
                        lastEnemySpawn = currentTime;
                        break;
                    }
                }
            }

            // 更新子弹
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    bullets[i].x += bullets[i].vx;
                    bullets[i].y += bullets[i].vy;

                    // 超出屏幕
                    if (bullets[i].y < -20)
                    {
                        bullets[i].active = 0;
                    }
                }
            }

            // 更新敌人
            for (int i = 0; i < MAX_ENEMIES; i++)
            {
                if (enemies[i].active)
                {
                    enemies[i].x += enemies[i].vx;
                    enemies[i].y += enemies[i].vy;

                    // 边界反弹
                    if (enemies[i].x < 0 || enemies[i].x > 770)
                        enemies[i].vx *= -1;

                    // 超出屏幕
                    if (enemies[i].y > 620)
                    {
                        enemies[i].active = 0;
                    }

                    // 玩家碰撞
                    Collider *playerCollider = Physics_CreateRectCollider(playerX, playerY, playerWidth, playerHeight);
                    Collider *enemyCollider = Physics_CreateRectCollider(enemies[i].x, enemies[i].y, 
                                                                       enemies[i].width, enemies[i].height);
                    if (Physics_CheckCollision(playerCollider, enemyCollider))
                    {
                        gameOver = 1;
                        printf("Game Over! Final Score: %d\n", score);
                    }
                    Physics_DestroyCollider(playerCollider);
                    Physics_DestroyCollider(enemyCollider);
                }
            }

            // 子弹与敌人碰撞
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active)
                {
                    for (int j = 0; j < MAX_ENEMIES; j++)
                    {
                        if (enemies[j].active)
                        {
                            Collider *bulletCollider = Physics_CreateRectCollider(bullets[i].x, bullets[i].y, 
                                                                               bullets[i].width, bullets[i].height);
                            Collider *enemyCollider = Physics_CreateRectCollider(enemies[j].x, enemies[j].y, 
                                                                               enemies[j].width, enemies[j].height);
                            if (Physics_CheckCollision(bulletCollider, enemyCollider))
                            {
                                bullets[i].active = 0;
                                enemies[j].health--;
                                if (enemies[j].health <= 0)
                                {
                                    enemies[j].active = 0;
                                    score += 10;
                                    printf("Enemy destroyed! Score: %d\n", score);
                                }
                            }
                            Physics_DestroyCollider(bulletCollider);
                            Physics_DestroyCollider(enemyCollider);
                        }
                    }
                }
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制玩家
        SDL_Rect playerRect = {(int)playerX, (int)playerY, (int)playerWidth, (int)playerHeight};
        SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &playerRect);

        // 绘制子弹
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].active)
            {
                SDL_Rect bulletRect = {(int)bullets[i].x, (int)bullets[i].y, 
                                     (int)bullets[i].width, (int)bullets[i].height};
                SDL_SetRenderDrawColor(renderer, bullets[i].color.r, bullets[i].color.g, bullets[i].color.b, 255);
                SDL_RenderFillRect(renderer, &bulletRect);
            }
        }

        // 绘制敌人
        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemies[i].active)
            {
                SDL_Rect enemyRect = {(int)enemies[i].x, (int)enemies[i].y, 
                                     (int)enemies[i].width, (int)enemies[i].height};
                SDL_SetRenderDrawColor(renderer, enemies[i].color.r, enemies[i].color.g, enemies[i].color.b, 255);
                SDL_RenderFillRect(renderer, &enemyRect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &enemyRect);
            }
        }

        // 显示信息
        printf("\rScore: %d | Bullets: %d | Enemies: %d | %s",
               score,
               MAX_BULLETS,
               MAX_ENEMIES,
               gameOver ? "GAME OVER" : "Playing");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    InputManager_Destroy(input);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Shooter Game Example ended\n");
    return 0;
}
