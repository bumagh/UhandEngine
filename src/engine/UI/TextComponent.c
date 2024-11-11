#include "TextComponent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TextComponent_init(TextComponent *self, SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color)
{
    self->base.name = strdup("TextComponent");
    self->base.update = NULL; // 如果需要更新功能，可以赋值对应的函数指针
    self->base.draw = TextComponent_draw;
    self->base.free = TextComponent_free;
    // 保存字体、渲染器和颜色
    self->renderer = renderer;
    self->font = font;
    self->color = color;
    self->enabled = SDL_TRUE;
    // 分配并复制文本
    size_t len = strlen(text);
    self->text = (char *)malloc(len + 1);
    if (self->text == NULL)
    {
        printf("Memory allocation failed for text!\n");
        return;
    }
    strcpy(self->text, text);
    if (text == NULL || strlen(text) == 0)
    {
        printf("Text is null or empty\n");
        return;
    }
    // 创建SDL_Surface并生成SDL_Texture
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (!surface)
    {
        printf("Failed to create surface: %s\n", TTF_GetError());
        free(self->text);
        return;
    }

    self->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!self->texture)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        free(self->text);
        return;
    }

    // 设置目标矩形
    self->dstRect = (SDL_Rect){0, 0, surface->w, surface->h};
    SDL_FreeSurface(surface);
}

void TextComponent_draw(Component *self, SDL_Renderer *renderer)
{
    TextComponent *textComponent = (TextComponent *)self;
    if (textComponent->texture && textComponent->enabled == SDL_TRUE)
    {
        SDL_RenderCopy(textComponent->renderer, textComponent->texture, NULL, &textComponent->dstRect);
    }
}

void TextComponent_free(Component *self)
{
    TextComponent *textComponent = (TextComponent *)self;
    if (textComponent->texture)
    {
        SDL_DestroyTexture(textComponent->texture);
        textComponent->texture = NULL;
    }
    if (textComponent->text)
    {
        free(textComponent->text);
        textComponent->text = NULL;
    }
}
void newTextComponent(TextComponent **self)
{
    *self = (TextComponent *)malloc(sizeof(TextComponent));
    if (*self == NULL)
    {
        printf("Failed to allocate memory for TextComponent!\n");
        return;
    }
    (*self)->draw = TextComponent_draw;
    (*self)->free = TextComponent_free;
    (*self)->init = TextComponent_init;
}
