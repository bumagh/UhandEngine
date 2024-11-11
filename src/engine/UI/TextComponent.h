#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include "../component.h" // 继承自Component
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct TextComponent
{
    Component base; // 继承自Component
    char *text;
    SDL_Texture *texture;
    SDL_Rect dstRect;
    SDL_Color color;
    TTF_Font *font;
    SDL_Renderer *renderer;
    SDL_bool enabled;
    // 方法指针
    void (*init)(struct TextComponent *self, SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color);
    void (*draw)(Component *self, SDL_Renderer *renderer);
    void (*free)(Component *self);
} TextComponent;

// 函数声明
void TextComponent_init(TextComponent *self, SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color);
void TextComponent_draw(Component *self, SDL_Renderer *renderer);
void TextComponent_free(Component *self);
void newTextComponent(TextComponent **self);
#endif // TEXT_COMPONENT_H
