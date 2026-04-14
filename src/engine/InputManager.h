#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include <stdio.h>

// 按键状态
typedef enum {
    KEY_STATE_UP,
    KEY_STATE_DOWN,
    KEY_STATE_PRESSED,
    KEY_STATE_RELEASED
} KeyState;

// 鼠标按钮
typedef enum {
    MOUSE_BUTTON_LEFT = 1,
    MOUSE_BUTTON_MIDDLE = 2,
    MOUSE_BUTTON_RIGHT = 3
} MouseButton;

// 输入管理器
typedef struct InputManager {
    KeyState keys[SDL_NUM_SCANCODES];      // 键盘状态
    KeyState mouseButtons[5];               // 鼠标按钮状态
    int mouseX, mouseY;                    // 鼠标位置
    int mouseRelX, mouseRelY;              // 鼠标相对移动
    int mouseWheel;                        // 鼠标滚轮
    KeyState prevKeys[SDL_NUM_SCANCODES];  // 上一帧键盘状态
    KeyState prevMouseButtons[5];          // 上一帧鼠标按钮状态
} InputManager;

// 函数声明
InputManager *InputManager_Create();
void InputManager_Destroy(InputManager *input);

// 每帧更新
void InputManager_Update(InputManager *input, SDL_Event *event);
void InputManager_UpdateState(InputManager *input);

// 键盘查询
int InputManager_IsKeyDown(InputManager *input, SDL_Scancode scancode);
int InputManager_IsKeyPressed(InputManager *input, SDL_Scancode scancode);
int InputManager_IsKeyReleased(InputManager *input, SDL_Scancode scancode);

// 鼠标查询
int InputManager_IsMouseButtonDown(InputManager *input, int button);
int InputManager_IsMouseButtonPressed(InputManager *input, int button);
int InputManager_IsMouseButtonReleased(InputManager *input, int button);
void InputManager_GetMousePosition(InputManager *input, int *x, int *y);
void InputManager_GetMouseRelative(InputManager *input, int *x, int *y);
int InputManager_GetMouseWheel(InputManager *input);

#endif // INPUT_MANAGER_H
