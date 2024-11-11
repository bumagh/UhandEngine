#include "UIComponent.h"

UIComponent *createUIComponent(int x, int y, int w, int h, SDL_Color color, const char *text, TTF_Font *font, SDL_Color textColor, void (*onClick)(void *))
{
    UIComponent *uiComp = (UIComponent *)malloc(sizeof(UIComponent));
    if (uiComp)
    {
        // 初始化基本的 Component 信息
        uiComp->base.update = updateUIComponent;
        SDL_Color bgColor = {0, 0, 0, 200}; // 半透明黑色背景
        // 初始化 UI 组件属性
        uiComp->rect.x = x;
        uiComp->rect.y = y;
        uiComp->rect.w = w;
        uiComp->rect.h = h;
        uiComp->color = color;
        uiComp->bgColor = bgColor;
        uiComp->text = text;
        uiComp->font = font;
        uiComp->textColor = textColor;
        uiComp->onClick = onClick;
    }
    return uiComp;
}

void updateUIComponent(Component *comp)
{
    // 更新逻辑 (这里可以添加事件处理，如鼠标点击等)
    UIComponent *uiComp = (UIComponent *)comp;
    // 检查是否有点击事件触发
    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) &&
        mouseX > uiComp->rect.x && mouseX < (uiComp->rect.x + uiComp->rect.w) &&
        mouseY > uiComp->rect.y && mouseY < (uiComp->rect.y + uiComp->rect.h))
    {
        // 调用点击回调函数
        if (uiComp->onClick)
        {
            uiComp->onClick(comp);
        }
    }
}

void renderUIComponent(Component *component, SDL_Renderer *renderer)
{
    UIComponent *uiComp = (UIComponent *)component;

    // 设置弹窗背景阴影
    SDL_Rect shadowRect = {uiComp->rect.x + 10, uiComp->rect.y + 10, uiComp->rect.w, uiComp->rect.h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // 半透明的黑色阴影
    SDL_RenderFillRect(renderer, &shadowRect);

    // 渐变背景
    SDL_Rect popupRect = uiComp->rect;
    SDL_Color startColor = {200, 200, 200, 255}; // 渐变的起始颜色
    SDL_Color endColor = {255, 255, 255, 255};   // 渐变的结束颜色

    // 简单的垂直渐变
    for (int i = 0; i < popupRect.h; ++i)
    {
        Uint8 r = startColor.r + (endColor.r - startColor.r) * i / popupRect.h;
        Uint8 g = startColor.g + (endColor.g - startColor.g) * i / popupRect.h;
        Uint8 b = startColor.b + (endColor.b - startColor.b) * i / popupRect.h;

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, popupRect.x, popupRect.y + i, popupRect.x + popupRect.w, popupRect.y + i);
    }

    // 添加弹窗边框
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // 深灰色边框
    SDL_RenderDrawRect(renderer, &popupRect);

    // 渲染文字
    if (uiComp->text && uiComp->font)
    {
        SDL_Surface *surface = TTF_RenderText_Solid(uiComp->font, uiComp->text, uiComp->textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect = {uiComp->rect.x, uiComp->rect.y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}
