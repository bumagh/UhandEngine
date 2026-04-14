#include "gameObject.h"
#include "transform.h"
#include "algorithm.h"
#include <string.h>
// 创建GameObject
GameObject *createGameObject(const char *name)
{
    GameObject *go = (GameObject *)malloc(sizeof(GameObject));
    go->name = strdup(name);
    // 分配并生成 UUID
    char *uuid = malloc(37); // UUID 的长度为 36 个字符加上一个空终止符
    if (uuid != NULL)
    {
        generateUUID(uuid);
        go->id = strdup(uuid);
    }
    else
    {
        printf("Failed to allocate memory for UUID\n");
    }
    go->components = NULL; // 初始化为空链表
    go->next = NULL;

    // 初始化 2D 基础属性
    go->visible = 1;    // 默认可见
    go->active = 1;     // 默认激活
    go->depth = 0;      // 默认层级
    go->parent = NULL;  // 默认无父对象
    go->children = NULL; // 默认无子对象

    // 生命周期函数初始化
    go->Awake = NULL;
    go->Start = NULL;
    go->Update = NULL;
    go->LateUpdate = NULL;
    go->FixedUpdate = NULL;
    go->Destroy = NULL;
    go->free = freeGameObject;
    go->getComponent = getComponent;
    go->render = renderGameObject;
    Component *comp = (Component *)createTransform(0, 0);
    addComponent(go, comp);
    go->transform = (Transform *)comp;
    return go;
}
void initFunc(GameObject *go)
{
    go->Awake = DefaultAwake;
    go->Start = DefaultStart;
    go->Update = DefaultUpdate;
    go->LateUpdate = DefaultLateUpdate;
    go->FixedUpdate = DefaultFixedUpdate;
    go->Destroy = DefaultDestroy;
}
// 向GameObject添加组件
void addComponent(GameObject *go, Component *comp)
{
    if (go->components == NULL)
    {
        go->components = comp; // 如果链表为空，直接将该组件作为链表头
    }
    else
    {
        // 否则将组件插入链表的末尾
        Component *current = go->components;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = comp;
    }
    comp->next = NULL; // 确保新添加的组件成为链表的末尾节点
}

// 更新GameObject的所有组件
void updateGameObject(GameObject *go)
{
    Component *current = go->components;
    while (current != NULL)
    {
        if (current->update != NULL)
        {
            current->update(current); // 调用每个组件的更新函数
        }
        current = current->next;
    }
}

// 实现Transform组件的update函数
void updateTransform(Component *comp)
{
    Transform *transform = (Transform *)comp;
    printf("Transform Update: Position (%.2f, %.2f, %.2f)\n", transform->x, transform->y, transform->z);
}

void freeGameObject(GameObject *go)
{

    // 释放组件链表
    Component *current = go->components;
    while (current != NULL)
    {
        Component *next = current->next;
        if (current->free != NULL)
            current->free(current);
        current = next;
    }
    // printf("freeGameObject quit 1\n");

    free(go->name); // 释放 GameObject 名称
    // printf("freeGameObject quit 2\n");

    free(go->id); // 释放 GameObject 名称
    // printf("freeGameObject quit 3\n");

    go->components = NULL;
    go->next = NULL;
    go->Awake = NULL;
    go->Start = NULL;
    go->Update = NULL;
    go->LateUpdate = NULL;
    go->FixedUpdate = NULL;
    go->Destroy = NULL;
    go->free = NULL;
    go->getComponent = NULL;
    go->render = NULL;
    free(go); // 释放 GameObject
    // printf("freeGameObject quit ok\n");
}
// 实现Renderer组件的update函数
// void updateRenderer(Component* comp) {
//     Renderer* renderer = (Renderer*)comp;
//     printf("Renderer Update: Color %s\n", renderer->color);
// }

// // 创建Renderer组件
// Renderer* createRenderer(const char* color) {
//     Renderer* r = (Renderer*)malloc(sizeof(Renderer));
//     r->base.name = "Renderer";
//     r->base.update = updateRenderer;
//     r->color = color;
//     return r;
// }
// 获取指定类型的组件
Component *getComponent(GameObject *go, const char *componentName)
{
    Component *current = go->components;
    while (current != NULL)
    {
        if (strcmp(current->name, componentName) == 0)
        {
            return current; // 找到匹配的组件并返回
        }
        current = current->next; // 遍历下一个组件
    }
    return NULL; // 如果没有找到匹配的组件，返回NULL
}
// 默认生命周期函数
void DefaultAwake(GameObject *self)
{
    printf("GameObject %s: Awake\n", self->name);
}

void DefaultStart(GameObject *self)
{
    printf("GameObject %s: Start\n", self->name);
}

void DefaultUpdate(GameObject *self)
{
    printf("GameObject %s: Update\n", self->name);
}

void DefaultLateUpdate(GameObject *self)
{
    printf("GameObject %s: LateUpdate\n", self->name);
}

void DefaultFixedUpdate(GameObject *self)
{
    printf("GameObject %s: FixedUpdate\n", self->name);
}

void DefaultDestroy(GameObject *self)
{
    printf("GameObject %s: Destroy\n", self->name);
}

// 获取组件链表的长度
int getComponentCount(GameObject *go)
{
    int count = 0;
    Component *current = go->components;
    while (current != NULL)
    {
        count++; // 计数每一个组件
        current = current->next;
    }
    return count; // 返回组件数量
}
void printAllComponent(GameObject *go)
{
    Component *current = go->components;
    int index = 1;
    printf("All Component:\n");
    while (current != NULL)
    {
        index++; // 计数每一个组件
        printf("Component%d:name->%s\n", index, current->name);
        current = current->next;
    }
}
void renderGameObject(GameObject *go, SDL_Renderer *renderer, void *context)
{
    if (!go || !renderer)
    {
        return; // 确保参数有效
    }

    Component *current = go->components;

    while (current != NULL)
    {
        if (current->draw != NULL)
        {
            current->draw(current, renderer); // 调用每个组件的渲染方法
        }
        current = current->next; // 遍历组件链表
    }
}

// 父子关系管理
void addChild(struct GameObject *parent, struct GameObject *child)
{
    if (parent == NULL || child == NULL)
    {
        return;
    }

    // 如果子对象已经有父对象，先从原父对象移除
    if (child->parent != NULL)
    {
        removeChild(child->parent, child);
    }

    // 设置新的父对象
    child->parent = parent;

    // 将子对象添加到父对象的子对象链表头部
    child->next = parent->children;
    parent->children = child;
}

void removeChild(struct GameObject *parent, struct GameObject *child)
{
    if (parent == NULL || child == NULL || child->parent != parent)
    {
        return;
    }

    // 从父对象的子对象链表中移除
    GameObject *current = parent->children;
    GameObject *prev = NULL;

    while (current != NULL)
    {
        if (current == child)
        {
            if (prev == NULL)
            {
                // 移除的是头节点
                parent->children = child->next;
            }
            else
            {
                prev->next = child->next;
            }
            break;
        }
        prev = current;
        current = current->next;
    }

    // 清除子对象的父对象引用
    child->parent = NULL;
    child->next = NULL;
}

// 属性管理
void setVisible(struct GameObject *go, int visible)
{
    if (go != NULL)
    {
        go->visible = visible;
    }
}

void setActive(struct GameObject *go, int active)
{
    if (go != NULL)
    {
        go->active = active;
    }
}

void setDepth(struct GameObject *go, int depth)
{
    if (go != NULL)
    {
        go->depth = depth;
    }
}

