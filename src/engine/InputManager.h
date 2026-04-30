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

// Pointer 类型（统一鼠标和触摸）
typedef enum {
    POINTER_TYPE_MOUSE,
    POINTER_TYPE_TOUCH
} PointerType;

// Pointer 按钮（统一抽象）
typedef enum {
    POINTER_BUTTON_LEFT = 0,
    POINTER_BUTTON_RIGHT = 1,
    POINTER_BUTTON_MIDDLE = 2
} PointerButton;

// Pointer 状态
typedef struct {
    int x, y;              // 位置
    int prevX, prevY;      // 上一帧位置
    int isDown;            // 是否按下
    int isPressed;         // 是否刚刚按下
    int isReleased;        // 是否刚刚释放
    int deltaX, deltaY;     // 相对移动
} PointerState;

// 输入管理器
typedef struct InputManager {
    KeyState keys[SDL_NUM_SCANCODES];      // 键盘状态
    KeyState mouseButtons[5];               // 鼠标按钮状态
    int mouseX, mouseY;                    // 鼠标位置
    int mouseRelX, mouseRelY;              // 鼠标相对移动
    int mouseWheel;                        // 鼠标滚轮
    KeyState prevKeys[SDL_NUM_SCANCODES];  // 上一帧键盘状态
    KeyState prevMouseButtons[5];          // 上一帧鼠标按钮状态
    
    // 统一的 Pointer 抽象（Phaser 风格）
    PointerState primaryPointer;           // 主指针（鼠标或第一个触摸点）
    PointerState pointers[10];             // 支持最多10个触摸点
    int activePointerCount;               // 活跃指针数量
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

// 统一的 Pointer 查询（Phaser 风格）
int InputManager_IsPointerDown(InputManager *input);
int InputManager_IsPointerPressed(InputManager *input);
int InputManager_IsPointerReleased(InputManager *input);
void InputManager_GetPointerPosition(InputManager *input, int *x, int *y);
void InputManager_GetPointerDelta(InputManager *input, int *x, int *y);

#endif // INPUT_MANAGER_H
