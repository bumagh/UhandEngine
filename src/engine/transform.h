#ifndef transform_h
#define transform_h
#include "component.h"
#ifdef pc_build
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif
typedef struct Transform
{
    Component base; // 继承自Component

    // Position (2D first)
    float x, y;
    float z; // 预留，暂不使用

    // Rotation (弧度)
    float rotation;

    // Scale
    float scaleX, scaleY;

    // Origin / Pivot Point (0-1, 相对于自身宽高)
    float originX, originY;

    // Function pointers
    void (*setPosition)(struct Transform *self, float x, float y);
    void (*setRotation)(struct Transform *self, float angle);
    void (*setScale)(struct Transform *self, float scaleX, float scaleY);
    void (*setOrigin)(struct Transform *self, float originX, float originY);
    void (*printPosition)(struct Transform *self);

} Transform;

// 创建函数
Transform *createTransform(float x, float y);

// 设置函数
void setPosition(Transform *self, float x, float y);
void setRotation(Transform *self, float angle);
void setScale(Transform *self, float scaleX, float scaleY);
void setOrigin(Transform *self, float originX, float originY);
void printPosition(Transform *self);

// 释放函数
void freeTransform(Component *component);
#endif