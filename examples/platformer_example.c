/**
 * Platformer Game Example
 *
 * 一个简单的平台跳跃游戏，展示物理、碰撞、动画系统的集成
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/InputManager.h"
#include "../src/engine/Physics.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

// 游戏状态
typedef struct {
    float x, y;
    float vx, vy;
    float width, height;
    int isJumping;
    int grounded;
    SDL_Color color;
} Player;

typedef struct {
    float x, y;
    float width, height;
    SDL_Color color;
} Platform;

typedef struct {
    float x, y;
    float width, height;
    int collected;
    SDL_Color color;
} Coin;

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Platformer Game Example",
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

    printf("=== Platformer Game Example ===\n\n");
    printf("Controls:\n");
    printf("- A/D or Left/Right: Move\n");
    printf("- Space or W/Up: Jump\n");
    printf("- R: Restart\n");
    printf("- Q: Quit\n");
    printf("\nObjective: Collect all coins!\n\n");

    // 创建输入系统
    InputManager *input = InputManager_Create();

    // 玩家
    Player player = {100, 400, 0, 0, 32, 48, 0, 0, {100, 200, 255}};
    float gravity = 0.5f;
    float jumpForce = -12.0f;
    float moveSpeed = 5.0f;

    // 平台
    Platform platforms[6] = {
        {0, 550, 800, 50, {100, 100, 100}},    // 地面
        {0, 400, 200, 20, {150, 150, 150}},    // 左平台
        {300, 350, 200, 20, {150, 150, 150}},  // 中平台
        {600, 300, 200, 20, {150, 150, 150}},  // 右平台
        {200, 200, 150, 20, {150, 150, 150}},  // 高平台
        {500, 150, 150, 20, {150, 150, 150}}   // 顶平台
    };

    // 金币
    Coin coins[5] = {
        {100, 360, 20, 20, 0, {255, 215, 0}},
        {400, 310, 20, 20, 0, {255, 215, 0}},
        {700, 260, 20, 20, 0, {255, 215, 0}},
        {275, 160, 20, 20, 0, {255, 215, 0}},
        {575, 110, 20, 20, 0, {255, 215, 0}}
    };

    int score = 0;
    int gameOver = 0;
    int win = 0;

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
                else if (event.key.keysym.sym == SDLK_r)
                {
                    // 重置游戏
                    player.x = 100;
                    player.y = 400;
                    player.vx = 0;
                    player.vy = 0;
                    score = 0;
                    gameOver = 0;
                    win = 0;
                    for (int i = 0; i < 5; i++)
                    {
                        coins[i].collected = 0;
                    }
                    printf("Game restarted!\n");
                }
            }

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        if (!gameOver)
        {
            // 水平移动
            if (InputManager_IsKeyDown(input, SDL_SCANCODE_A) || InputManager_IsKeyDown(input, SDL_SCANCODE_LEFT))
            {
                player.vx = -moveSpeed;
            }
            else if (InputManager_IsKeyDown(input, SDL_SCANCODE_D) || InputManager_IsKeyDown(input, SDL_SCANCODE_RIGHT))
            {
                player.vx = moveSpeed;
            }
            else
            {
                player.vx *= 0.8f; // 摩擦力
                if (fabsf(player.vx) < 0.1f) player.vx = 0;
            }

            // 跳跃
            if ((InputManager_IsKeyDown(input, SDL_SCANCODE_SPACE) || 
                 InputManager_IsKeyDown(input, SDL_SCANCODE_W) || 
                 InputManager_IsKeyDown(input, SDL_SCANCODE_UP)) && 
                player.grounded)
            {
                player.vy = jumpForce;
                player.grounded = 0;
                player.isJumping = 1;
            }

            // 应用重力
            player.vy += gravity;

            // 更新位置
            player.x += player.vx;
            player.y += player.vy;

            // 平台碰撞检测
            player.grounded = 0;
            for (int i = 0; i < 6; i++)
            {
                Collider *playerCollider = Physics_CreateRectCollider(player.x, player.y, player.width, player.height);
                Collider *platformCollider = Physics_CreateRectCollider(platforms[i].x, platforms[i].y, 
                                                                        platforms[i].width, platforms[i].height);

                if (Physics_CheckCollision(playerCollider, platformCollider))
                {
                    // 简单的碰撞响应：只处理从上方落下的情况
                    if (player.vy > 0 && player.y + player.height - player.vy <= platforms[i].y)
                    {
                        player.y = platforms[i].y - player.height;
                        player.vy = 0;
                        player.grounded = 1;
                        player.isJumping = 0;
                    }
                }

                Physics_DestroyCollider(playerCollider);
                Physics_DestroyCollider(platformCollider);
            }

            // 屏幕边界
            if (player.x < 0) player.x = 0;
            if (player.x > 800 - player.width) player.x = 800 - player.width;
            if (player.y > 600)
            {
                player.y = 400;
                player.x = 100;
                player.vx = 0;
                player.vy = 0;
                printf("Fell off screen! Respawning...\n");
            }

            // 金币收集
            for (int i = 0; i < 5; i++)
            {
                if (!coins[i].collected)
                {
                    Collider *playerCollider = Physics_CreateRectCollider(player.x, player.y, player.width, player.height);
                    Collider *coinCollider = Physics_CreateRectCollider(coins[i].x, coins[i].y, coins[i].width, coins[i].height);

                    if (Physics_CheckCollision(playerCollider, coinCollider))
                    {
                        coins[i].collected = 1;
                        score += 10;
                        printf("Coin collected! Score: %d\n", score);
                    }

                    Physics_DestroyCollider(playerCollider);
                    Physics_DestroyCollider(coinCollider);
                }
            }

            // 胜利条件
            if (score >= 50)
            {
                gameOver = 1;
                win = 1;
                printf("You Win! Final Score: %d\n", score);
            }
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制平台
        for (int i = 0; i < 6; i++)
        {
            SDL_Rect rect = {(int)platforms[i].x, (int)platforms[i].y, 
                           (int)platforms[i].width, (int)platforms[i].height};
            SDL_SetRenderDrawColor(renderer, platforms[i].color.r, platforms[i].color.g, platforms[i].color.b, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }

        // 绘制金币
        for (int i = 0; i < 5; i++)
        {
            if (!coins[i].collected)
            {
                SDL_Rect rect = {(int)coins[i].x, (int)coins[i].y, (int)coins[i].width, (int)coins[i].height};
                SDL_SetRenderDrawColor(renderer, coins[i].color.r, coins[i].color.g, coins[i].color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }

        // 绘制玩家
        SDL_Rect playerRect = {(int)player.x, (int)player.y, (int)player.width, (int)player.height};
        SDL_SetRenderDrawColor(renderer, player.color.r, player.color.g, player.color.b, 255);
        SDL_RenderFillRect(renderer, &playerRect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &playerRect);

        // 显示信息
        printf("\rScore: %d/50 | %s", score, win ? "YOU WIN!" : (gameOver ? "GAME OVER" : "Playing"));

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    InputManager_Destroy(input);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Platformer Game Example ended\n");
    return 0;
}
