#include "UIComponent.h"

UIComponent *createUIComponent(int x, int y, int w, int h, SDL_Color color, const char *text, TTF_Font *font, SDL_Color textColor, void (*onClick)(void *))
{
    UIComponent *uiComp = (UIComponent *)malloc(sizeof(UIComponent));
    if (uiComp)
    {
        // 初始化基本的 Component 信息
        uiComp->base.name = "UIComponent";
        uiComp->base.update = updateUIComponent;
        uiComp->base.draw = renderUIComponent;
        uiComp->base.free = NULL; // UIComponent 使用标准 free
        uiComp->base.next = NULL;
        uiComp->mousePressed = 0; // 初始化鼠标状态
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

    int isHovering = mouseX > uiComp->rect.x && mouseX < (uiComp->rect.x + uiComp->rect.w) &&
                     mouseY > uiComp->rect.y && mouseY < (uiComp->rect.y + uiComp->rect.h);

    int isPressed = (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    // 检测鼠标按下瞬间（从未按下到按下）
    if (isPressed && !uiComp->mousePressed && isHovering)
    {
        // 调用点击回调函数
        if (uiComp->onClick)
        {
            uiComp->onClick(comp);
        }
    }

    uiComp->mousePressed = isPressed;
}

void renderUIComponent(Component *component, SDL_Renderer *renderer)
{
    UIComponent *uiComp = (UIComponent *)component;

    // 设置弹窗背景阴影
    SDL_Rect shadowRect = {uiComp->rect.x + 5, uiComp->rect.y + 5, uiComp->rect.w, uiComp->rect.h};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // 半透明的黑色阴影
    SDL_RenderFillRect(renderer, &shadowRect);

    // 使用纯色背景
    SDL_SetRenderDrawColor(renderer, uiComp->color.r, uiComp->color.g, uiComp->color.b, uiComp->color.a);
    SDL_RenderFillRect(renderer, &uiComp->rect);

    // 添加弹窗边框
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // 深灰色边框
    SDL_RenderDrawRect(renderer, &uiComp->rect);

    // 渲染文字
    if (uiComp->text && uiComp->font)
    {
        SDL_Surface *surface = TTF_RenderText_Solid(uiComp->font, uiComp->text, uiComp->textColor);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture)
            {
                // 居中文字
                SDL_Rect textRect = {
                    uiComp->rect.x + (uiComp->rect.w - surface->w) / 2,
                    uiComp->rect.y + (uiComp->rect.h - surface->h) / 2,
                    surface->w,
                    surface->h
                };
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
}
