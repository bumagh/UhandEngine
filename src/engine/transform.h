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

    // Local Position (2D first)
    float x, y;

    // Rotation (弧度)
    float rotation;

    // Scale
    float scaleX, scaleY;

    // Anchor Point (0-1, 相对于自身宽高，Phaser 风格)
    float anchorX, anchorY;

    // Size (用于精灵尺寸)
    float width, height;

    // Skew (Phaser 风格)
    float skewX, skewY;

    // World Position (计算得出，用于父子变换)
    float worldX, worldY;
    float worldRotation;
    float worldScaleX, worldScaleY;

    // Function pointers
    void (*setPosition)(struct Transform *self, float x, float y);
    void (*setRotation)(struct Transform *self, float angle);
    void (*setScale)(struct Transform *self, float scaleX, float scaleY);
    void (*setAnchor)(struct Transform *self, float anchorX, float anchorY);
    void (*setSize)(struct Transform *self, float width, float height);
    void (*setSkew)(struct Transform *self, float skewX, float skewY);
    void (*updateWorldTransform)(struct Transform *self);
    void (*printTransform)(struct Transform *self);

} Transform;

// 创建函数
Transform *createTransform(float x, float y);

// 设置函数
void setPosition(Transform *self, float x, float y);
void setRotation(Transform *self, float angle);
void setScale(Transform *self, float scaleX, float scaleY);
void setAnchor(Transform *self, float anchorX, float anchorY);
void setSize(Transform *self, float width, float height);
void setSkew(Transform *self, float skewX, float skewY);
void updateWorldTransform(Transform *self);
void printTransform(Transform *self);

// 释放函数
void freeTransform(Component *component);
#endif