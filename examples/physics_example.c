/**
 * Physics Example
 *
 * 演示物理碰撞检测系统的使用
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/Physics.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    float x, y;
    float vx, vy;
    float width, height;
    int isCircle;
    float radius;
    SDL_Color color;
} PhysicsObject;

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Physics Example",
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

    printf("=== Physics Example ===\n\n");
    printf("Controls:\n");
    printf("- WASD: Move player\n");
    printf("- Space: Switch player shape\n");
    printf("- R: Reset positions\n");
    printf("- Q: Quit\n\n");

    // 创建对象
    PhysicsObject player = {100, 300, 0, 0, 50, 50, 0, 0, {100, 200, 255}};
    PhysicsObject obstacles[5];
    
    for (int i = 0; i < 5; i++)
    {
        obstacles[i].x = 300 + i * 100;
        obstacles[i].y = 200 + (i % 2) * 200;
        obstacles[i].vx = (i % 2 == 0) ? 2 : -2;
        obstacles[i].vy = (i % 3 == 0) ? 1 : -1;
        obstacles[i].width = 40 + rand() % 20;
        obstacles[i].height = 40 + rand() % 20;
        obstacles[i].isCircle = i % 2;
        obstacles[i].radius = obstacles[i].width / 2;
        obstacles[i].color = (SDL_Color){255, 100, 100, 255};
    }

    // 主循环
    int running = 1;
    SDL_Event event;
    float speed = 5.0f;

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
                    player.isCircle = !player.isCircle;
                    player.radius = player.width / 2;
                    printf("Player shape: %s\n", player.isCircle ? "Circle" : "Rect");
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    player.x = 100;
                    player.y = 300;
                    printf("Positions reset\n");
                }
            }
        }

        // 玩家移动
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
            player.y -= speed;
        if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
            player.y += speed;
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
            player.x -= speed;
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
            player.x += speed;

        // 限制玩家在屏幕内
        if (player.x < 0) player.x = 0;
        if (player.x > 750) player.x = 750;
        if (player.y < 0) player.y = 0;
        if (player.y > 550) player.y = 550;

        // 更新障碍物
        for (int i = 0; i < 5; i++)
        {
            obstacles[i].x += obstacles[i].vx;
            obstacles[i].y += obstacles[i].vy;

            // 边界反弹
            if (obstacles[i].x < 0 || obstacles[i].x > 750)
                obstacles[i].vx *= -1;
            if (obstacles[i].y < 0 || obstacles[i].y > 550)
                obstacles[i].vy *= -1;
        }

        // 碰撞检测
        Collider *playerCollider = player.isCircle 
            ? Physics_CreateCircleCollider(player.x + player.radius, player.y + player.radius, player.radius)
            : Physics_CreateRectCollider(player.x, player.y, player.width, player.height);

        int collisionCount = 0;
        for (int i = 0; i < 5; i++)
        {
            Collider *obstacleCollider = obstacles[i].isCircle
                ? Physics_CreateCircleCollider(obstacles[i].x + obstacles[i].radius, 
                                               obstacles[i].y + obstacles[i].radius, 
                                               obstacles[i].radius)
                : Physics_CreateRectCollider(obstacles[i].x, obstacles[i].y, 
                                             obstacles[i].width, obstacles[i].height);

            if (Physics_CheckCollision(playerCollider, obstacleCollider))
            {
                obstacles[i].color = (SDL_Color){255, 255, 100, 255};
                collisionCount++;
            }
            else
            {
                obstacles[i].color = (SDL_Color){255, 100, 100, 255};
            }

            Physics_DestroyCollider(obstacleCollider);
        }

        Physics_DestroyCollider(playerCollider);

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制玩家
        SDL_SetRenderDrawColor(renderer, player.color.r, player.color.g, player.color.b, player.color.a);
        if (player.isCircle)
        {
            // 绘制圆形（简化为多边形）
            for (int angle = 0; angle < 360; angle += 10)
            {
                float rad = angle * 3.14159265f / 180;
                float x1 = player.x + player.radius + cosf(rad) * player.radius;
                float y1 = player.y + player.radius + sinf(rad) * player.radius;
                float x2 = player.x + player.radius + cosf((angle + 10) * 3.14159265f / 180) * player.radius;
                float y2 = player.y + player.radius + sinf((angle + 10) * 3.14159265f / 180) * player.radius;
                SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
            }
        }
        else
        {
            SDL_Rect rect = {(int)player.x, (int)player.y, (int)player.width, (int)player.height};
            SDL_RenderFillRect(renderer, &rect);
        }

        // 绘制障碍物
        for (int i = 0; i < 5; i++)
        {
            SDL_SetRenderDrawColor(renderer, obstacles[i].color.r, obstacles[i].color.g, obstacles[i].color.b, obstacles[i].color.a);
            if (obstacles[i].isCircle)
            {
                for (int angle = 0; angle < 360; angle += 10)
                {
                    float rad = angle * 3.14159265f / 180;
                    float x1 = obstacles[i].x + obstacles[i].radius + cosf(rad) * obstacles[i].radius;
                    float y1 = obstacles[i].y + obstacles[i].radius + sinf(rad) * obstacles[i].radius;
                    float x2 = obstacles[i].x + obstacles[i].radius + cosf((angle + 10) * 3.14159265f / 180) * obstacles[i].radius;
                    float y2 = obstacles[i].y + obstacles[i].radius + sinf((angle + 10) * 3.14159265f / 180) * obstacles[i].radius;
                    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
                }
            }
            else
            {
                SDL_Rect rect = {(int)obstacles[i].x, (int)obstacles[i].y, 
                               (int)obstacles[i].width, (int)obstacles[i].height};
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        // 显示信息
        printf("\rPlayer: (%.0f, %.0f) | Shape: %s | Collisions: %d",
               player.x, player.y, player.isCircle ? "Circle" : "Rect", collisionCount);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Physics Example ended\n");
    return 0;
}
