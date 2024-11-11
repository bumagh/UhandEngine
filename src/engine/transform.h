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
    float x, y, z;  // 坐标
    void (*setPosition)(struct Transform *self, float x, float y, float z);
    void (*printPosition)(struct Transform *self);

} Transform;
void setPosition(struct Transform *self, float x, float y, float z);
void printPosition(struct Transform *self);
void freeTransform(Component *component);
#endif