#include "transform.h"

void setPosition(Transform *self, float x, float y, float z)
{
    self->x = x;
    self->y = y;
    self->z = z;
}

void printPosition(struct Transform *self)
{
    printf("position:(x,y,z):(%f,%f,%f)\n", self->x, self->y, self->z);
}
// 创建Transform组件
Transform *createTransform(float x, float y, float z)
{
    Transform *t = (Transform *)malloc(sizeof(Transform));
    if (t == NULL)
    {
        printf("Error: Could not allocate memory for Transform\n");
        return NULL;
    }
    t->base.name = strdup("Transform");
    t->base.draw = NULL;
    t->base.free = freeTransform;
    t->base.update = NULL;
    t->x = x;
    t->y = y;
    t->z = z;
    t->setPosition = setPosition;
    t->printPosition = printPosition;
    return t;
}
void freeTransform(Component *component)
{
    if (component != NULL)
    {
        if (component->name != NULL)
        {
            free(component->name); // 释放name
        }
        component->draw = NULL;
        component->update = NULL;
        component->free = NULL;
        free(component);
        component = NULL;
    }
}
