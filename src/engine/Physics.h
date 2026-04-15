#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL2/SDL.h>

// 碰撞体类型
typedef enum {
    COLLIDER_NONE,
    COLLIDER_RECT,      // 矩形碰撞体
    COLLIDER_CIRCLE     // 圆形碰撞体
} ColliderType;

// 矩形碰撞体
typedef struct RectCollider {
    float x, y;      // 位置
    float width, height; // 尺寸
} RectCollider;

// 圆形碰撞体
typedef struct CircleCollider {
    float x, y;      // 圆心位置
    float radius;    // 半径
} CircleCollider;

// 碰撞体
typedef struct Collider {
    ColliderType type;
    union {
        RectCollider rect;
        CircleCollider circle;
    };
} Collider;

// AABB 碰撞检测
int Physics_CheckAABB(float x1, float y1, float w1, float h1,
                     float x2, float y2, float w2, float h2);

// 圆形碰撞检测
int Physics_CheckCircleCollision(float x1, float y1, float r1,
                                float x2, float y2, float r2);

// 矩形与圆形碰撞检测
int Physics_CheckRectCircleCollision(float rx, float ry, float rw, float rh,
                                    float cx, float cy, float cr);

// 通用碰撞检测
int Physics_CheckCollision(Collider *a, Collider *b);

// 创建矩形碰撞体
Collider *Physics_CreateRectCollider(float x, float y, float width, float height);

// 创建圆形碰撞体
Collider *Physics_CreateCircleCollider(float x, float y, float radius);

// 销毁碰撞体
void Physics_DestroyCollider(Collider *collider);

#endif // PHYSICS_H
