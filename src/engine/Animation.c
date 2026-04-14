#include "Animation.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 创建动画
Animation *Animation_Create(const char *name, int frameWidth, int frameHeight, int loop)
{
    Animation *animation = (Animation *)malloc(sizeof(Animation));
    if (!animation)
    {
        printf("Failed to allocate memory for Animation\n");
        return NULL;
    }

    animation->name = strdup(name);
    animation->frames = NULL;
    animation->frameCount = 0;
    animation->loop = loop;
    animation->frameWidth = frameWidth;
    animation->frameHeight = frameHeight;

    return animation;
}

// 销毁动画
void Animation_Destroy(Animation *animation)
{
    if (animation)
    {
        if (animation->name)
        {
            free(animation->name);
        }
        if (animation->frames)
        {
            free(animation->frames);
        }
        free(animation);
    }
}

// 添加帧
void Animation_AddFrame(Animation *animation, SDL_Texture *texture, int duration)
{
    if (!animation)
        return;

    animation->frameCount++;
    animation->frames = (AnimationFrame *)realloc(animation->frames, sizeof(AnimationFrame) * animation->frameCount);

    animation->frames[animation->frameCount - 1].texture = texture;
    animation->frames[animation->frameCount - 1].duration = duration;
}

// 创建动画播放器
Animator *Animator_Create()
{
    Animator *animator = (Animator *)malloc(sizeof(Animator));
    if (!animator)
    {
        printf("Failed to allocate memory for Animator\n");
        return NULL;
    }

    animator->currentAnimation = NULL;
    animator->currentFrame = 0;
    animator->frameTime = 0;
    animator->playing = 0;
    animator->speed = 1.0f;

    return animator;
}

// 销毁动画播放器
void Animator_Destroy(Animator *animator)
{
    if (animator)
    {
        free(animator);
    }
}

// 播放动画
void Animator_Play(Animator *animator, Animation *animation)
{
    if (!animator || !animation)
        return;

    animator->currentAnimation = animation;
    animator->currentFrame = 0;
    animator->frameTime = 0;
    animator->playing = 1;
}

// 停止动画
void Animator_Stop(Animator *animator)
{
    if (!animator)
        return;

    animator->playing = 0;
    animator->currentFrame = 0;
    animator->frameTime = 0;
}

// 暂停动画
void Animator_Pause(Animator *animator)
{
    if (!animator)
        return;

    animator->playing = 0;
}

// 恢复动画
void Animator_Resume(Animator *animator)
{
    if (!animator)
        return;

    animator->playing = 1;
}

// 设置播放速度
void Animator_SetSpeed(Animator *animator, float speed)
{
    if (animator && speed > 0)
    {
        animator->speed = speed;
    }
}

// 更新动画
void Animator_Update(Animator *animator, int deltaTime)
{
    if (!animator || !animator->playing || !animator->currentAnimation)
        return;

    Animation *animation = animator->currentAnimation;
    animator->frameTime += deltaTime * animator->speed;

    // 检查是否需要切换到下一帧
    if (animator->frameTime >= animation->frames[animator->currentFrame].duration)
    {
        animator->frameTime -= animation->frames[animator->currentFrame].duration;
        animator->currentFrame++;

        // 检查是否到达最后一帧
        if (animator->currentFrame >= animation->frameCount)
        {
            if (animation->loop)
            {
                animator->currentFrame = 0;
            }
            else
            {
                animator->currentFrame = animation->frameCount - 1;
                animator->playing = 0;
            }
        }
    }
}

// 获取当前纹理
SDL_Texture *Animator_GetCurrentTexture(Animator *animator)
{
    if (!animator || !animator->currentAnimation)
        return NULL;

    return animator->currentAnimation->frames[animator->currentFrame].texture;
}

// 获取当前帧矩形
SDL_Rect Animator_GetCurrentFrameRect(Animator *animator)
{
    SDL_Rect rect = {0, 0, 0, 0};

    if (!animator || !animator->currentAnimation)
        return rect;

    rect.w = animator->currentAnimation->frameWidth;
    rect.h = animator->currentAnimation->frameHeight;

    return rect;
}
