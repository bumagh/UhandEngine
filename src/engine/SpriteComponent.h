#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "../engine/component.h"
#include <SDL2/SDL.h>

// SpriteComponent 是用于渲染精灵图像的组件
typedef struct SpriteComponent {
    Component base;
    SDL_Texture *texture;
    int width;
    int height;
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int totalFrames;
    int flipX;  // 水平翻转
    int flipY;  // 垂直翻转
    float alpha; // 透明度 (0.0 - 1.0)
} SpriteComponent;

// 函数声明
SpriteComponent *SpriteComponent_Create(SDL_Texture *texture, int width, int height);
void SpriteComponent_Destroy(SpriteComponent *sprite);

// 设置纹理
void SpriteComponent_SetTexture(SpriteComponent *sprite, SDL_Texture *texture);

// 设置帧尺寸（用于精灵表）
void SpriteComponent_SetFrameSize(SpriteComponent *sprite, int frameWidth, int frameHeight);

// 设置当前帧
void SpriteComponent_SetFrame(SpriteComponent *sprite, int frame);

// 获取当前帧的源矩形
void SpriteComponent_GetSourceRect(SpriteComponent *sprite, SDL_Rect *rect);

// 渲染函数
void SpriteComponent_Render(SpriteComponent *sprite, SDL_Renderer *renderer, float x, float y, float scaleX, float scaleY, float rotation);

// 翻转控制
void SpriteComponent_SetFlipX(SpriteComponent *sprite, int flip);
void SpriteComponent_SetFlipY(SpriteComponent *sprite, int flip);

// 透明度控制
void SpriteComponent_SetAlpha(SpriteComponent *sprite, float alpha);

#endif // SPRITE_COMPONENT_H
