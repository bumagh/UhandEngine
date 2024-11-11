#ifndef gameObject_h
#define gameObject_h

#ifdef pc_build
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COMPONENTS 10
#include "component.h"
#include "transform.h"

// 定义GameObject结构体
typedef struct GameObject
{
    // 其他 GameObject 的成员变量
    const char *name;
    const char *id;
    Component *components;   // 指向组件链表的头指针
    Transform *transform;    // 指向组件链表的头指针
    struct GameObject *next; // 链表中的下一个节点
    // void *owner;//用于存储指向宿主对象的指针
    void (*free)(struct GameObject *go);
    // 生命周期函数指针
    void (*Awake)(struct GameObject *self);
    void (*Start)(struct GameObject *self);
    void (*Update)(struct GameObject *self);
    void (*LateUpdate)(struct GameObject *self);
    void (*FixedUpdate)(struct GameObject *self);
    void (*Destroy)(struct GameObject *self);
    void (*render)(struct GameObject *go, SDL_Renderer *renderer, void *context);
    Component *(*getComponent)(struct GameObject *go, const char *componentName);
} GameObject;

// 创建 GameObject
GameObject *createGameObject(const char *name);
void newGameObject(struct GameObject **self);

// 向 GameObject 添加组件
void addComponent(struct GameObject *go, Component *comp); // TODO:升级2级指针
// 获取指定类型的组件
Component *getComponent(struct GameObject *go, const char *componentName);
// 更新 GameObject 的所有组件
void updateGameObject(struct GameObject *go);
void renderGameObject(struct GameObject *go, SDL_Renderer *renderer, void *context);
// Transform 组件的 update 函数
void updateTransform(Component *comp);

// 创建 Transform 组件
Transform *createTransform(float x, float y, float z);
void freeGameObject(GameObject *go);
int getComponentCount(GameObject *go);
void printAllComponent(GameObject *go); // 默认生命周期函数
void DefaultAwake(GameObject *self);
void DefaultStart(GameObject *self);
void DefaultUpdate(GameObject *self);
void DefaultLateUpdate(GameObject *self);
void DefaultFixedUpdate(GameObject *self);
void DefaultDestroy(GameObject *self);
#endif
