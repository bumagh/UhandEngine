#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#ifdef pc_build
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
int w, h;
#define SDL_DELAY 12
#define FRAMERATE 30
#define MARGIN 50
#define PI 3.14159
#define LENGTH(x, y) SDL_sqrt((x) * (x) + (y) * (y))
#define WINDOW_W 960
#define WINDOW_H 540
#define MARGIN 50

#include <math.h>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 540
#define JOYSTICK_RADIUS 100

typedef struct {
    int active;            // 是否激活
    float centerX, centerY; // 摇杆中心
    float innerX, innerY;   // 摇杆内圈位置
    float radius;           // 摇杆半径
    float h, v;             // 水平和垂直偏移量
    SDL_FingerID fingerId;  // 控制摇杆的手指ID
} Joystick;

Joystick leftJoystick;
Joystick rightJoystick1, rightJoystick2, rightJoystick3;

// 初始化摇杆
void initJoystick(Joystick *joystick, float centerX, float centerY) {
    joystick->centerX = centerX;
    joystick->centerY = centerY;
    joystick->innerX = centerX;
    joystick->innerY = centerY;
    joystick->radius = JOYSTICK_RADIUS;
    joystick->h = 0;
    joystick->v = 0;
    joystick->active = 0;
    joystick->fingerId = -1; // 表示没有手指控制
}

// 渲染摇杆
void renderJoystick(SDL_Renderer *renderer, Joystick *joystick) {
    if (joystick->active) {
        // 绘制摇杆外圈
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect outerCircle = {
            (int)(joystick->centerX - joystick->radius),
            (int)(joystick->centerY - joystick->radius),
            (int)(joystick->radius * 2),
            (int)(joystick->radius * 2)
        };
        SDL_RenderDrawRect(renderer, &outerCircle);

        // 绘制摇杆内圈
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect innerCircle = {
            (int)(joystick->innerX - joystick->radius / 2),
            (int)(joystick->innerY - joystick->radius / 2),
            (int)(joystick->radius),
            (int)(joystick->radius)
        };
        SDL_RenderDrawRect(renderer, &innerCircle);
    }
}

// 处理触摸事件
void handleTouchEvent(SDL_Event *event, Joystick *leftJoystick, Joystick *rightJoystick1, Joystick *rightJoystick2, Joystick *rightJoystick3) {
    if (event->type == SDL_FINGERDOWN || event->type == SDL_FINGERMOTION) {
        float touchX = event->tfinger.x * SCREEN_WIDTH;
        float touchY = event->tfinger.y * SCREEN_HEIGHT;

        Joystick *joystick = NULL;

        // 左侧摇杆
        if (touchX < SCREEN_WIDTH / 2) {
            joystick = leftJoystick;
        } else {
            // 右侧三个摇杆区域：分别处理三个右侧摇杆
            if (touchY < SCREEN_HEIGHT / 3) {
                joystick = rightJoystick1; // 顶部摇杆（技能1）
            } else if (touchY < 2 * SCREEN_HEIGHT / 3) {
                joystick = rightJoystick2; // 中间摇杆（技能2）
            } else {
                joystick = rightJoystick3; // 底部摇杆（技能3）
            }
        }

        // 检查是否已有手指控制该摇杆，如果有则不覆盖
        if (joystick->fingerId == -1 || joystick->fingerId == event->tfinger.fingerId) {
            float dx = touchX - joystick->centerX;
            float dy = touchY - joystick->centerY;
            float distance = sqrtf(dx * dx + dy * dy);

            joystick->fingerId = event->tfinger.fingerId; // 绑定手指ID

            if (distance < joystick->radius) {
                joystick->innerX = touchX;
                joystick->innerY = touchY;
                joystick->h = dx / joystick->radius;
                joystick->v = dy / joystick->radius;
                joystick->active = 1;
            } else {
                joystick->innerX = joystick->centerX + dx / distance * joystick->radius;
                joystick->innerY = joystick->centerY + dy / distance * joystick->radius;
                joystick->h = dx / joystick->radius;
                joystick->v = dy / joystick->radius;
                joystick->active = 1;
            }
        }
    }

    if (event->type == SDL_FINGERUP) {
        // 手指松开时释放摇杆控制
        if (leftJoystick->fingerId == event->tfinger.fingerId) {
            leftJoystick->innerX = leftJoystick->centerX;
            leftJoystick->innerY = leftJoystick->centerY;
            leftJoystick->h = 0;
            leftJoystick->v = 0;
            leftJoystick->active = 0;
            leftJoystick->fingerId = -1; // 释放控制
        }

        if (rightJoystick1->fingerId == event->tfinger.fingerId) {
            rightJoystick1->innerX = rightJoystick1->centerX;
            rightJoystick1->innerY = rightJoystick1->centerY;
            rightJoystick1->h = 0;
            rightJoystick1->v = 0;
            rightJoystick1->active = 0;
            rightJoystick1->fingerId = -1; // 释放控制
        }

        if (rightJoystick2->fingerId == event->tfinger.fingerId) {
            rightJoystick2->innerX = rightJoystick2->centerX;
            rightJoystick2->innerY = rightJoystick2->centerY;
            rightJoystick2->h = 0;
            rightJoystick2->v = 0;
            rightJoystick2->active = 0;
            rightJoystick2->fingerId = -1; // 释放控制
        }

        if (rightJoystick3->fingerId == event->tfinger.fingerId) {
            rightJoystick3->innerX = rightJoystick3->centerX;
            rightJoystick3->innerY = rightJoystick3->centerY;
            rightJoystick3->h = 0;
            rightJoystick3->v = 0;
            rightJoystick3->active = 0;
            rightJoystick3->fingerId = -1; // 释放控制
        }
    }
}
// 全局或静态变量，用于管理弹窗状态
int isPopupVisible = 0; // 弹窗是否可见
int success = 1;
SDL_Event e;
int gQuit = 0;
float mouseX = 0;
float mouseY = 0;
int fingerSupport = 1;
int numFrames = 0;
int startTime = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int init()
{
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = 0;
    }
    else
    {
        window = SDL_CreateWindow("testtwojoystick", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
            // Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = 0;
            }
        }
    }
    return success;
}

void tick()
{
    SDL_RenderClear(renderer);
    while (SDL_PollEvent(&e) != 0)
    {
        // 处理触摸事件
            handleTouchEvent(&e, &leftJoystick, &rightJoystick1, &rightJoystick2, &rightJoystick3);
        SDL_KeyCode mkey = e.key.keysym.sym;
        switch (e.type)
        {
        case SDL_QUIT:
            gQuit = 1;
            break;
        case SDL_KEYDOWN:
        {
            if (mkey == 'q')
            {
                gQuit = 1;
            }
        }
        }
    }
    // 处理触摸事件
    SDL_SetRenderDrawColor(renderer, 117, 117, 117, 255);
    SDL_RenderClear(renderer);
      // 渲染左侧摇杆
        renderJoystick(renderer, &leftJoystick);

        // 渲染右侧三个摇杆
        renderJoystick(renderer, &rightJoystick1);
        renderJoystick(renderer, &rightJoystick2);
        renderJoystick(renderer, &rightJoystick3);
    SDL_RenderPresent(renderer);
}
void quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void gameInit()
{
    SDL_GetWindowSize(window, &w, &h);
    SDL_Rect fbrect = {w - MARGIN * 2, h - MARGIN * 5, 80, 80};
     // 初始化左侧摇杆
    initJoystick(&leftJoystick, 150, SCREEN_HEIGHT - 150);

    // 初始化右侧三个摇杆
    initJoystick(&rightJoystick1, SCREEN_WIDTH - 150, SCREEN_HEIGHT / 6);  // 顶部
    initJoystick(&rightJoystick2, SCREEN_WIDTH - 150, SCREEN_HEIGHT / 2);  // 中间
    initJoystick(&rightJoystick3, SCREEN_WIDTH - 150, 5 * SCREEN_HEIGHT / 6);  // 底部
}
void loop()
{
    startTime = SDL_GetTicks();
#if __EMSCRIPTEN__
    emscripten_set_main_loop(tick, 0, 0);
#else
    while (!gQuit)
    {
        tick();
        long current = SDL_GetTicks();
        long cost = current - startTime;
        long frame = 1000 / FRAMERATE;
        long delay = frame - cost;
        if (delay > 0)
            SDL_Delay(delay);
    }
    quit();

#endif
}

#ifndef __EMSCRIPTEN__
#undef main
#endif
int main(int argc, char *argv[])
{
    if (!init())
    {
        printf("SDL could not be initialized\n");
        return 1;
    }
    gameInit();
    loop();
    return 0;
}
