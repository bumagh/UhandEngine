#ifndef gameObject_h
#define gameObject_h

#ifdef pc_build
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_ttf.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COMPONENTS 10
#include "component.h"
#include "transform.h"

// GameObject 类型枚举
typedef enum
{
    GAMEOBJECT_TYPE_BASE,      // 基础 GameObject
    GAMEOBJECT_TYPE_SPRITE,    // Sprite 图像对象
    GAMEOBJECT_TYPE_TEXT,      // Text 文本对象
    GAMEOBJECT_TYPE_CONTAINER  // Container 容器对象
} GameObjectType;

// 定义GameObject结构体
typedef struct GameObject
{
    // 基础属性
    GameObjectType type;  // GameObject 类型
    const char *name;
    const char *id;

    // 2D 基础属性
    int visible;      // 可见性 (1=可见, 0=不可见)
    int active;       // 激活状态 (1=激活, 0=未激活)
    int depth;        // 渲染层级 (z-index)

    // 父子关系
    struct GameObject *parent;      // 父对象
    struct GameObject *children;    // 子对象链表头

    // 组件
    Component *components;   // 指向组件链表的头指针
    Transform *transform;    // Transform 组件

    // 链表节点 (用于 Scene 的 GameObjectList)
    struct GameObject *next; // 链表中的下一个节点

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
GameObject *createGameObjectWithType(const char *name, GameObjectType type);
void newGameObject(struct GameObject **self);

// 创建特定类型的 GameObject 辅助函数
GameObject *createSprite(const char *name, const char *texturePath, float x, float y);
GameObject *createText(const char *name, const char *text, TTF_Font *font, SDL_Color color, float x, float y);
GameObject *createContainer(const char *name);

// 向 GameObject 添加组件
void addComponent(struct GameObject *go, Component *comp); // TODO:升级2级指针
// 获取指定类型的组件
Component *getComponent(struct GameObject *go, const char *componentName);
// 更新 GameObject 的所有组件
void updateGameObject(struct GameObject *go);
void renderGameObject(struct GameObject *go, SDL_Renderer *renderer, void *context);
// Transform 组件的 update 函数
void updateTransform(Component *comp);

// 父子关系管理
void addChild(struct GameObject *parent, struct GameObject *child);
void removeChild(struct GameObject *parent, struct GameObject *child);

// 属性管理
void setVisible(struct GameObject *go, int visible);
void setActive(struct GameObject *go, int active);
void setDepth(struct GameObject *go, int depth);

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
