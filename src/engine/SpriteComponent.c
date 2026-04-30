#include "SpriteComponent.h"
#include <stdlib.h>
#include <stdio.h>

// SpriteComponent 的 draw 函数包装器
static void SpriteComponent_Draw(Component *comp, SDL_Renderer *renderer)
{
    if (!comp || !renderer)
        return;

    SpriteComponent *sprite = (SpriteComponent *)comp;

    // 暂时使用固定位置，后续需要通过 GameObject 获取 Transform 信息
    float x = 100;
    float y = 100;

    SpriteComponent_Render(sprite, renderer, x, y, 1.0f, 1.0f, 0.0f);
}

SpriteComponent *SpriteComponent_Create(SDL_Texture *texture, int width, int height)
{
    SpriteComponent *sprite = (SpriteComponent *)malloc(sizeof(SpriteComponent));
    if (!sprite)
    {
        printf("Failed to allocate memory for SpriteComponent\n");
        return NULL;
    }

    // 初始化基类
    sprite->base.type = COMPONENT_TYPE_SPRITE;
    sprite->base.name = "SpriteComponent";
    sprite->base.destroy = (void (*)(Component *))SpriteComponent_Destroy;
    sprite->base.update = NULL;
    sprite->base.draw = SpriteComponent_Draw;

    // 初始化属性
    sprite->texture = texture;
    sprite->width = width;
    sprite->height = height;
    sprite->frameWidth = width;
    sprite->frameHeight = height;
    sprite->currentFrame = 0;
    sprite->totalFrames = 1;
    sprite->flipX = 0;
    sprite->flipY = 0;
    sprite->alpha = 1.0f;

    return sprite;
}

void SpriteComponent_Destroy(SpriteComponent *sprite)
{
    if (sprite)
    {
        // 注意：不释放 texture，由 AssetManager 管理
        free(sprite);
    }
}

void SpriteComponent_SetTexture(SpriteComponent *sprite, SDL_Texture *texture)
{
    if (sprite)
    {
        sprite->texture = texture;
    }
}

void SpriteComponent_SetFrameSize(SpriteComponent *sprite, int frameWidth, int frameHeight)
{
    if (sprite)
    {
        sprite->frameWidth = frameWidth;
        sprite->frameHeight = frameHeight;
        sprite->totalFrames = (sprite->width / frameWidth) * (sprite->height / frameHeight);
    }
}

void SpriteComponent_SetFrame(SpriteComponent *sprite, int frame)
{
    if (sprite)
    {
        sprite->currentFrame = frame;
        if (sprite->currentFrame >= sprite->totalFrames)
        {
            sprite->currentFrame = sprite->totalFrames - 1;
        }
    }
}

void SpriteComponent_GetSourceRect(SpriteComponent *sprite, SDL_Rect *rect)
{
    if (sprite && rect)
    {
        int framesPerRow = sprite->width / sprite->frameWidth;
        int row = sprite->currentFrame / framesPerRow;
        int col = sprite->currentFrame % framesPerRow;

        rect->x = col * sprite->frameWidth;
        rect->y = row * sprite->frameHeight;
        rect->w = sprite->frameWidth;
        rect->h = sprite->frameHeight;
    }
}

void SpriteComponent_Render(SpriteComponent *sprite, SDL_Renderer *renderer, float x, float y, float scaleX, float scaleY, float rotation)
{
    if (!sprite || !renderer || !sprite->texture)
    {
        return;
    }

    // 获取源矩形
    SDL_Rect srcRect;
    SpriteComponent_GetSourceRect(sprite, &srcRect);

    // 计算目标矩形
    int destWidth = (int)(sprite->frameWidth * scaleX);
    int destHeight = (int)(sprite->frameHeight * scaleY);
    SDL_Rect destRect = {(int)x, (int)y, destWidth, destHeight};

    // 设置透明度
    SDL_SetTextureAlphaMod(sprite->texture, (Uint8)(sprite->alpha * 255));

    // 设置翻转
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (sprite->flipX) flip |= SDL_FLIP_HORIZONTAL;
    if (sprite->flipY) flip |= SDL_FLIP_VERTICAL;

    // 渲染
    SDL_RenderCopyEx(renderer, sprite->texture, &srcRect, &destRect, rotation, NULL, flip);
}

void SpriteComponent_SetFlipX(SpriteComponent *sprite, int flip)
{
    if (sprite)
    {
        sprite->flipX = flip;
    }
}

void SpriteComponent_SetFlipY(SpriteComponent *sprite, int flip)
{
    if (sprite)
    {
        sprite->flipY = flip;
    }
}

void SpriteComponent_SetAlpha(SpriteComponent *sprite, float alpha)
{
    if (sprite)
    {
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;
        sprite->alpha = alpha;
    }
}
