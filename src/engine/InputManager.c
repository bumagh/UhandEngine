#include "InputManager.h"
#include <stdlib.h>
#include <string.h>

// 创建输入管理器
InputManager *InputManager_Create()
{
    InputManager *input = (InputManager *)malloc(sizeof(InputManager));
    if (!input)
    {
        printf("Failed to allocate memory for InputManager\n");
        return NULL;
    }

    memset(input, 0, sizeof(InputManager));
    return input;
}

// 销毁输入管理器
void InputManager_Destroy(InputManager *input)
{
    if (input)
    {
        free(input);
    }
}

// 每帧更新
void InputManager_Update(InputManager *input, SDL_Event *event)
{
    if (!input || !event)
        return;

    // 处理事件
    switch (event->type)
    {
        case SDL_KEYDOWN:
            input->keys[event->key.keysym.scancode] = KEY_STATE_DOWN;
            break;
        case SDL_KEYUP:
            input->keys[event->key.keysym.scancode] = KEY_STATE_UP;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button < 5)
            {
                input->mouseButtons[event->button.button] = KEY_STATE_DOWN;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event->button.button < 5)
            {
                input->mouseButtons[event->button.button] = KEY_STATE_UP;
            }
            break;
        case SDL_MOUSEMOTION:
            input->mouseX = event->motion.x;
            input->mouseY = event->motion.y;
            input->mouseRelX = event->motion.xrel;
            input->mouseRelY = event->motion.yrel;
            break;
        case SDL_MOUSEWHEEL:
            input->mouseWheel = event->wheel.y;
            break;
    }
}

// 更新状态（在帧结束时调用）
void InputManager_UpdateState(InputManager *input)
{
    if (!input)
        return;

    // 更新键盘状态
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if (input->prevKeys[i] == KEY_STATE_DOWN && input->keys[i] == KEY_STATE_UP)
        {
            input->keys[i] = KEY_STATE_RELEASED;
        }
        else if (input->prevKeys[i] == KEY_STATE_UP && input->keys[i] == KEY_STATE_DOWN)
        {
            input->keys[i] = KEY_STATE_PRESSED;
        }
        else if (input->keys[i] == KEY_STATE_PRESSED)
        {
            input->keys[i] = KEY_STATE_DOWN;
        }
        else if (input->keys[i] == KEY_STATE_RELEASED)
        {
            input->keys[i] = KEY_STATE_UP;
        }

        input->prevKeys[i] = input->keys[i];
    }

    // 更新鼠标按钮状态
    for (int i = 0; i < 5; i++)
    {
        if (input->prevMouseButtons[i] == KEY_STATE_DOWN && input->mouseButtons[i] == KEY_STATE_UP)
        {
            input->mouseButtons[i] = KEY_STATE_RELEASED;
        }
        else if (input->prevMouseButtons[i] == KEY_STATE_UP && input->mouseButtons[i] == KEY_STATE_DOWN)
        {
            input->mouseButtons[i] = KEY_STATE_PRESSED;
        }
        else if (input->mouseButtons[i] == KEY_STATE_PRESSED)
        {
            input->mouseButtons[i] = KEY_STATE_DOWN;
        }
        else if (input->mouseButtons[i] == KEY_STATE_RELEASED)
        {
            input->mouseButtons[i] = KEY_STATE_UP;
        }

        input->prevMouseButtons[i] = input->mouseButtons[i];
    }

    // 重置鼠标相对移动和滚轮
    input->mouseRelX = 0;
    input->mouseRelY = 0;
    input->mouseWheel = 0;
}

// 键盘查询
int InputManager_IsKeyDown(InputManager *input, SDL_Scancode scancode)
{
    if (!input || scancode >= SDL_NUM_SCANCODES)
        return 0;
    return input->keys[scancode] == KEY_STATE_DOWN || input->keys[scancode] == KEY_STATE_PRESSED;
}

int InputManager_IsKeyPressed(InputManager *input, SDL_Scancode scancode)
{
    if (!input || scancode >= SDL_NUM_SCANCODES)
        return 0;
    return input->keys[scancode] == KEY_STATE_PRESSED;
}

int InputManager_IsKeyReleased(InputManager *input, SDL_Scancode scancode)
{
    if (!input || scancode >= SDL_NUM_SCANCODES)
        return 0;
    return input->keys[scancode] == KEY_STATE_RELEASED;
}

// 鼠标查询
int InputManager_IsMouseButtonDown(InputManager *input, int button)
{
    if (!input || button >= 5)
        return 0;
    return input->mouseButtons[button] == KEY_STATE_DOWN || input->mouseButtons[button] == KEY_STATE_PRESSED;
}

int InputManager_IsMouseButtonPressed(InputManager *input, int button)
{
    if (!input || button >= 5)
        return 0;
    return input->mouseButtons[button] == KEY_STATE_PRESSED;
}

int InputManager_IsMouseButtonReleased(InputManager *input, int button)
{
    if (!input || button >= 5)
        return 0;
    return input->mouseButtons[button] == KEY_STATE_RELEASED;
}

void InputManager_GetMousePosition(InputManager *input, int *x, int *y)
{
    if (input)
    {
        if (x) *x = input->mouseX;
        if (y) *y = input->mouseY;
    }
}

void InputManager_GetMouseRelative(InputManager *input, int *x, int *y)
{
    if (input)
    {
        if (x) *x = input->mouseRelX;
        if (y) *y = input->mouseRelY;
    }
}

int InputManager_GetMouseWheel(InputManager *input)
{
    if (input)
        return input->mouseWheel;
    return 0;
}
