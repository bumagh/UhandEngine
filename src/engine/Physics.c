#include "Physics.h"
#include <stdlib.h>
#include <math.h>

// AABB 碰撞检测
int Physics_CheckAABB(float x1, float y1, float w1, float h1,
                     float x2, float y2, float w2, float h2)
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

// 圆形碰撞检测
int Physics_CheckCircleCollision(float x1, float y1, float r1,
                                float x2, float y2, float r2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = sqrtf(dx * dx + dy * dy);
    return distance < (r1 + r2);
}

// 矩形与圆形碰撞检测
int Physics_CheckRectCircleCollision(float rx, float ry, float rw, float rh,
                                    float cx, float cy, float cr)
{
    // 找到矩形上离圆心最近的点
    float closestX = cx;
    float closestY = cy;

    if (cx < rx) closestX = rx;
    else if (cx > rx + rw) closestX = rx + rw;

    if (cy < ry) closestY = ry;
    else if (cy > ry + rh) closestY = ry + rh;

    // 计算最近点到圆心的距离
    float dx = cx - closestX;
    float dy = cy - closestY;
    float distance = sqrtf(dx * dx + dy * dy);

    return distance < cr;
}

// 通用碰撞检测
int Physics_CheckCollision(Collider *a, Collider *b)
{
    if (!a || !b) return 0;

    if (a->type == COLLIDER_RECT && b->type == COLLIDER_RECT)
    {
        return Physics_CheckAABB(a->rect.x, a->rect.y, a->rect.width, a->rect.height,
                               b->rect.x, b->rect.y, b->rect.width, b->rect.height);
    }
    else if (a->type == COLLIDER_CIRCLE && b->type == COLLIDER_CIRCLE)
    {
        return Physics_CheckCircleCollision(a->circle.x, a->circle.y, a->circle.radius,
                                          b->circle.x, b->circle.y, b->circle.radius);
    }
    else if (a->type == COLLIDER_RECT && b->type == COLLIDER_CIRCLE)
    {
        return Physics_CheckRectCircleCollision(a->rect.x, a->rect.y, a->rect.width, a->rect.height,
                                               b->circle.x, b->circle.y, b->circle.radius);
    }
    else if (a->type == COLLIDER_CIRCLE && b->type == COLLIDER_RECT)
    {
        return Physics_CheckRectCircleCollision(b->rect.x, b->rect.y, b->rect.width, b->rect.height,
                                               a->circle.x, a->circle.y, a->circle.radius);
    }

    return 0;
}

// 创建矩形碰撞体
Collider *Physics_CreateRectCollider(float x, float y, float width, float height)
{
    Collider *collider = (Collider *)malloc(sizeof(Collider));
    if (!collider)
    {
        return NULL;
    }

    collider->type = COLLIDER_RECT;
    collider->rect.x = x;
    collider->rect.y = y;
    collider->rect.width = width;
    collider->rect.height = height;

    return collider;
}

// 创建圆形碰撞体
Collider *Physics_CreateCircleCollider(float x, float y, float radius)
{
    Collider *collider = (Collider *)malloc(sizeof(Collider));
    if (!collider)
    {
        return NULL;
    }

    collider->type = COLLIDER_CIRCLE;
    collider->circle.x = x;
    collider->circle.y = y;
    collider->circle.radius = radius;

    return collider;
}

// 销毁碰撞体
void Physics_DestroyCollider(Collider *collider)
{
    if (collider)
    {
        free(collider);
    }
}
