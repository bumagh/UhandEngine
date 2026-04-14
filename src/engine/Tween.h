#ifndef TWEEN_H
#define TWEEN_H

#include <SDL2/SDL.h>

// 补间类型
typedef enum {
    TWEEN_EASE_LINEAR,
    TWEEN_EASE_QUAD_IN,
    TWEEN_EASE_QUAD_OUT,
    TWEEN_EASE_QUAD_INOUT,
    TWEEN_EASE_CUBIC_IN,
    TWEEN_EASE_CUBIC_OUT,
    TWEEN_EASE_CUBIC_INOUT,
    TWEEN_EASE_ELASTIC_OUT
} TweenEase;

// 补间属性
typedef enum {
    TWEEN_PROPERTY_X,
    TWEEN_PROPERTY_Y,
    TWEEN_PROPERTY_SCALE_X,
    TWEEN_PROPERTY_SCALE_Y,
    TWEEN_PROPERTY_ROTATION,
    TWEEN_PROPERTY_ALPHA
} TweenProperty;

// 补间节点
typedef struct TweenNode {
    TweenProperty property;
    float from;
    float to;
    float *target;
    TweenEase ease;
    struct TweenNode *next;
} TweenNode;

// 补间
typedef struct Tween {
    TweenNode *nodes;
    int duration;        // 毫秒
    int elapsedTime;     // 已播放时间
    int playing;         // 是否播放
    int loop;           // 是否循环
    int yoyo;           // 是否往返
    int yoyoDirection;  // 往返方向（1 = 正向，-1 = 反向）
    void (*onComplete)(struct Tween *tween);
    void (*onUpdate)(struct Tween *tween, float progress);
    void *userData;
} Tween;

// 函数声明
Tween *Tween_Create();
void Tween_Destroy(Tween *tween);

// 属性补间
void Tween_To(Tween *tween, float *target, float to, int duration, TweenEase ease);
void Tween_FromTo(Tween *tween, float *target, float from, float to, int duration, TweenEase ease);

// 控制函数
void Tween_Play(Tween *tween);
void Tween_Pause(Tween *tween);
void Tween_Stop(Tween *tween);
void Tween_Restart(Tween *tween);
void Tween_SetLoop(Tween *tween, int loop);
void Tween_SetYoyo(Tween *tween, int yoyo);

// 回调函数
void Tween_SetOnComplete(Tween *tween, void (*callback)(Tween *tween));
void Tween_SetOnUpdate(Tween *tween, void (*callback)(Tween *tween, float progress));

// 更新函数
void Tween_Update(Tween *tween, int deltaTime);

// 缓动函数
float Tween_Ease_Linear(float t);
float Tween_Ease_QuadIn(float t);
float Tween_Ease_QuadOut(float t);
float Tween_Ease_QuadInOut(float t);
float Tween_Ease_CubicIn(float t);
float Tween_Ease_CubicOut(float t);
float Tween_Ease_CubicInOut(float t);
float Tween_Ease_ElasticOut(float t);

#endif // TWEEN_H
