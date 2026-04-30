#include "transform.h"
#include <stdio.h>
#include <string.h>

void setPosition(Transform *self, float x, float y)
{
    self->x = x;
    self->y = y;
    self->updateWorldTransform(self);
}

void setRotation(Transform *self, float angle)
{
    self->rotation = angle;
    self->updateWorldTransform(self);
}

void setScale(Transform *self, float scaleX, float scaleY)
{
    self->scaleX = scaleX;
    self->scaleY = scaleY;
    self->updateWorldTransform(self);
}

void setAnchor(Transform *self, float anchorX, float anchorY)
{
    self->anchorX = anchorX;
    self->anchorY = anchorY;
}

void setSize(Transform *self, float width, float height)
{
    self->width = width;
    self->height = height;
}

void setSkew(Transform *self, float skewX, float skewY)
{
    self->skewX = skewX;
    self->skewY = skewY;
}

void updateWorldTransform(Transform *self)
{
    // 简化版本：暂时只复制 local 到 world
    // 后续需要支持父子变换时，需要递归计算父对象的变换
    self->worldX = self->x;
    self->worldY = self->y;
    self->worldRotation = self->rotation;
    self->worldScaleX = self->scaleX;
    self->worldScaleY = self->scaleY;
}

void printTransform(Transform *self)
{
    printf("Transform: local=(%f, %f), rotation=%f, scale=(%f, %f), anchor=(%f, %f), size=(%f, %f), skew=(%f, %f)\n",
           self->x, self->y, self->rotation, self->scaleX, self->scaleY,
           self->anchorX, self->anchorY, self->width, self->height, self->skewX, self->skewY);
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
    t->rotation = 0;
    t->scaleX = 1;
    t->scaleY = 1;
    t->anchorX = 0.5; // 默认中心
    t->anchorY = 0.5;
    t->width = 0;
    t->height = 0;
    t->skewX = 0;
    t->skewY = 0;

    // 初始化 world transform
    t->worldX = x;
    t->worldY = y;
    t->worldRotation = 0;
    t->worldScaleX = 1;
    t->worldScaleY = 1;

    // 设置函数指针
    t->setPosition = setPosition;
    t->setRotation = setRotation;
    t->setScale = setScale;
    t->setAnchor = setAnchor;
    t->setSize = setSize;
    t->setSkew = setSkew;
    t->updateWorldTransform = updateWorldTransform;
    t->printTransform = printTransform;

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
