#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

// 动画帧
typedef struct AnimationFrame {
    SDL_Texture *texture;  // 帧纹理
    int duration;          // 帧持续时间（毫秒）
} AnimationFrame;

// 动画
typedef struct Animation {
    char *name;                    // 动画名称
    AnimationFrame *frames;         // 帧数组
    int frameCount;                // 帧数
    int loop;                      // 是否循环
    int frameWidth;                // 帧宽度
    int frameHeight;               // 帧高度
} Animation;

// 动画播放器
typedef struct Animator {
    Animation *currentAnimation;   // 当前动画
    int currentFrame;              // 当前帧索引
    int frameTime;                 // 当前帧已播放时间
    int playing;                   // 是否正在播放
    int speed;                     // 播放速度（1.0 = 正常）
} Animator;

// 函数声明
Animation *Animation_Create(const char *name, int frameWidth, int frameHeight, int loop);
void Animation_Destroy(Animation *animation);
void Animation_AddFrame(Animation *animation, SDL_Texture *texture, int duration);

Animator *Animator_Create();
void Animator_Destroy(Animator *animator);
void Animator_Play(Animator *animator, Animation *animation);
void Animator_Stop(Animator *animator);
void Animator_Pause(Animator *animator);
void Animator_Resume(Animator *animator);
void Animator_SetSpeed(Animator *animator, float speed);
void Animator_Update(Animator *animator, int deltaTime);
SDL_Texture *Animator_GetCurrentTexture(Animator *animator);
SDL_Rect Animator_GetCurrentFrameRect(Animator *animator);

#endif // ANIMATION_H
