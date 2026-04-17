// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"
#include "RenderQueue.h"
#include <SDL2/SDL.h>

// Scene 结构体
typedef struct Scene
{
    // 场景标识
    const char *name;
    const char *id;

    // 对象管理（使用根对象替代 GameObjectList）
    GameObject *rootObject;  // 根对象

    // 渲染队列
    RenderQueue *renderQueue;

    // 生命周期状态
    int isAwake;
    int isStarted;

    // 场景特定数据
    void *userData;

    // 生命周期函数指针
    void (*Awake)(struct Scene *self);
    void (*Start)(struct Scene *self);
    void (*Update)(struct Scene *self);
    void (*Destroy)(struct Scene *self);

    // 渲染函数
    void (*Render)(struct Scene *self, SDL_Renderer *renderer);

    // 场景切换回调
    void (*OnEnter)(struct Scene *self, struct Scene *from);
    void (*OnExit)(struct Scene *self, struct Scene *to);
} Scene;

// 函数声明
Scene *Scene_Create();
void Scene_Destroy(Scene *scene);

// GameObject 管理
void Scene_AddGameObject(Scene *scene, GameObject *gameObject);
void Scene_RemoveGameObject(Scene *scene, GameObject *gameObject);

// 生命周期方法
void Scene_Awake(Scene *scene);
void Scene_Start(Scene *scene);
void Scene_Update(Scene *scene);

// 渲染相关
void Scene_RenderGameObjects(Scene *scene, SDL_Renderer *renderer);
void Scene_Render(Scene *scene, SDL_Renderer *renderer, void *context);

#endif // SCENE_H
