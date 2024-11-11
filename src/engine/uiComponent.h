#ifndef UIComponent_h
#define UIComponent_h

#include "component.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct UIComponent
{
    Component base;           // 继承自 Component
    SDL_Rect rect;            // UI 元素的位置和大小
    SDL_Color color;          // UI 元素的背景颜色
    SDL_Color bgColor;  // 半透明黑色背景
    const char *text;         // 显示的文本
    TTF_Font *font;           // 字体
    SDL_Color textColor;      // 文本颜色
    void (*onClick)(void*);   // 点击事件处理函数指针
} UIComponent;

// 创建一个新的 UI 组件
UIComponent *createUIComponent(int x, int y, int w, int h, SDL_Color color, const char *text, TTF_Font *font, SDL_Color textColor, void (*onClick)(void*));

// 更新和渲染 UI 组件
void updateUIComponent(Component *comp);
void renderUIComponent(Component *comp, SDL_Renderer *renderer);

#endif
