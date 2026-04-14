#include "Tween.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// 创建补间
Tween *Tween_Create()
{
    Tween *tween = (Tween *)malloc(sizeof(Tween));
    if (!tween)
    {
        printf("Failed to allocate memory for Tween\n");
        return NULL;
    }

    tween->nodes = NULL;
    tween->duration = 1000;
    tween->elapsedTime = 0;
    tween->playing = 0;
    tween->loop = 0;
    tween->yoyo = 0;
    tween->yoyoDirection = 1;
    tween->onComplete = NULL;
    tween->onUpdate = NULL;
    tween->userData = NULL;

    return tween;
}

// 销毁补间
void Tween_Destroy(Tween *tween)
{
    if (tween)
    {
        TweenNode *current = tween->nodes;
        while (current)
        {
            TweenNode *next = current->next;
            free(current);
            current = next;
        }
        free(tween);
    }
}

// 添加补间节点
static void Tween_AddNode(Tween *tween, float *target, float from, float to, int duration, TweenEase ease)
{
    TweenNode *node = (TweenNode *)malloc(sizeof(TweenNode));
    if (!node)
    {
        printf("Failed to allocate memory for TweenNode\n");
        return;
    }

    node->target = target;
    node->from = from;
    node->to = to;
    node->ease = ease;
    node->next = tween->nodes;
    tween->nodes = node;

    tween->duration = duration;
}

// To 补间
void Tween_To(Tween *tween, float *target, float to, int duration, TweenEase ease)
{
    if (!tween || !target)
        return;

    Tween_AddNode(tween, target, *target, to, duration, ease);
}

// FromTo 补间
void Tween_FromTo(Tween *tween, float *target, float from, float to, int duration, TweenEase ease)
{
    if (!tween || !target)
        return;

    *target = from;
    Tween_AddNode(tween, target, from, to, duration, ease);
}

// 播放
void Tween_Play(Tween *tween)
{
    if (tween)
        tween->playing = 1;
}

// 暂停
void Tween_Pause(Tween *tween)
{
    if (tween)
        tween->playing = 0;
}

// 停止
void Tween_Stop(Tween *tween)
{
    if (tween)
    {
        tween->playing = 0;
        tween->elapsedTime = 0;
        tween->yoyoDirection = 1;
    }
}

// 重启
void Tween_Restart(Tween *tween)
{
    if (tween)
    {
        tween->elapsedTime = 0;
        tween->yoyoDirection = 1;
        tween->playing = 1;
    }
}

// 设置循环
void Tween_SetLoop(Tween *tween, int loop)
{
    if (tween)
        tween->loop = loop;
}

// 设置往返
void Tween_SetYoyo(Tween *tween, int yoyo)
{
    if (tween)
        tween->yoyo = yoyo;
}

// 设置完成回调
void Tween_SetOnComplete(Tween *tween, void (*callback)(Tween *tween))
{
    if (tween)
        tween->onComplete = callback;
}

// 设置更新回调
void Tween_SetOnUpdate(Tween *tween, void (*callback)(Tween *tween, float progress))
{
    if (tween)
        tween->onUpdate = callback;
}

// 缓动函数
float Tween_Ease_Linear(float t)
{
    return t;
}

float Tween_Ease_QuadIn(float t)
{
    return t * t;
}

float Tween_Ease_QuadOut(float t)
{
    return t * (2 - t);
}

float Tween_Ease_QuadInOut(float t)
{
    return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

float Tween_Ease_CubicIn(float t)
{
    return t * t * t;
}

float Tween_Ease_CubicOut(float t)
{
    t -= 1.0f;
    return t * t * t + 1;
}

float Tween_Ease_CubicInOut(float t)
{
    return t < 0.5f ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
}

float Tween_Ease_ElasticOut(float t)
{
    const float c4 = (2 * 3.14159265f) / 3;
    return t == 0 ? 0 : t == 1 ? 1 : powf(2, -10 * t) * sinf((t * 10 - 0.75) * c4) + 1;
}

// 获取缓动值
static float GetEasedValue(TweenEase ease, float t)
{
    switch (ease)
    {
        case TWEEN_EASE_LINEAR: return Tween_Ease_Linear(t);
        case TWEEN_EASE_QUAD_IN: return Tween_Ease_QuadIn(t);
        case TWEEN_EASE_QUAD_OUT: return Tween_Ease_QuadOut(t);
        case TWEEN_EASE_QUAD_INOUT: return Tween_Ease_QuadInOut(t);
        case TWEEN_EASE_CUBIC_IN: return Tween_Ease_CubicIn(t);
        case TWEEN_EASE_CUBIC_OUT: return Tween_Ease_CubicOut(t);
        case TWEEN_EASE_CUBIC_INOUT: return Tween_Ease_CubicInOut(t);
        case TWEEN_EASE_ELASTIC_OUT: return Tween_Ease_ElasticOut(t);
        default: return Tween_Ease_Linear(t);
    }
}

// 更新补间
void Tween_Update(Tween *tween, int deltaTime)
{
    if (!tween || !tween->playing)
        return;

    tween->elapsedTime += deltaTime * tween->yoyoDirection;

    // 计算进度
    float t = (float)tween->elapsedTime / tween->duration;

    // 检查是否完成
    if (t >= 1.0f)
    {
        if (tween->loop || tween->yoyo)
        {
            if (tween->yoyo)
            {
                tween->yoyoDirection *= -1;
                tween->elapsedTime = tween->duration;
            }
            else
            {
                tween->elapsedTime = 0;
            }
            t = 1.0f;
        }
        else
        {
            t = 1.0f;
            tween->playing = 0;

            // 设置最终值
            TweenNode *node = tween->nodes;
            while (node)
            {
                *(node->target) = node->to;
                node = node->next;
            }

            // 调用完成回调
            if (tween->onComplete)
            {
                tween->onComplete(tween);
            }
            return;
        }
    }
    else if (t <= 0.0f && tween->yoyoDirection < 0)
    {
        if (tween->yoyo)
        {
            tween->yoyoDirection *= -1;
            tween->elapsedTime = 0;
        }
        t = 0.0f;
    }

    // 更新所有节点
    TweenNode *node = tween->nodes;
    while (node)
    {
        float easedT = GetEasedValue(node->ease, t);
        float value = node->from + (node->to - node->from) * easedT;
        *(node->target) = value;
        node = node->next;
    }

    // 调用更新回调
    if (tween->onUpdate)
    {
        tween->onUpdate(tween, t);
    }
}
