#include "transform.h"
#include <stdio.h>
#include <string.h>

void setPosition(Transform *self, float x, float y)
{
    self->x = x;
    self->y = y;
}

void setRotation(Transform *self, float angle)
{
    self->rotation = angle;
}

void setScale(Transform *self, float scaleX, float scaleY)
{
    self->scaleX = scaleX;
    self->scaleY = scaleY;
}

void setOrigin(Transform *self, float originX, float originY)
{
    self->originX = originX;
    self->originY = originY;
}

void printPosition(Transform *self)
{
    printf("Transform: position=(%f, %f), rotation=%f, scale=(%f, %f), origin=(%f, %f)\n",
           self->x, self->y, self->rotation, self->scaleX, self->scaleY, self->originX, self->originY);
}

// 创建Transform组件
Transform *createTransform(float x, float y)
{
    Transform *t = (Transform *)malloc(sizeof(Transform));
    if (t == NULL)
    {
        printf("Error: Could not allocate memory for Transform\n");
        return NULL;
    }
    t->base.name = strdup("Transform");
    t->base.draw = NULL;
    t->base.destroy = freeTransform;
    t->base.update = NULL;
    t->base.next = NULL;

    // 初始化 2D first 属性
    t->x = x;
    t->y = y;
    t->z = 0; // 预留
    t->rotation = 0;
    t->scaleX = 1;
    t->scaleY = 1;
    t->originX = 0.5; // 默认中心
    t->originY = 0.5;

    // 设置函数指针
    t->setPosition = setPosition;
    t->setRotation = setRotation;
    t->setScale = setScale;
    t->setOrigin = setOrigin;
    t->printPosition = printPosition;

    return t;
}

void freeTransform(Component *component)
{
    if (component != NULL)
    {
        if (component->name != NULL)
        {
            free((void *)component->name); // 释放name
        }
        component->draw = NULL;
        component->update = NULL;
        component->destroy = NULL;
        free(component);
        component = NULL;
    }
}
