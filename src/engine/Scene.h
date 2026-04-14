// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include "gameObjectList.h"
#include "uiComponent.h"
#include "RenderQueue.h"
#include <SDL2/SDL.h>

// Scene 结构体
typedef struct Scene
{
    GameObjectList *gameObjectList; // GameObject 列表
    UIComponent *uiComponents;     // UI 组件链表
    RenderQueue *renderQueue;      // 渲染队列
} Scene;

// 函数声明
Scene *Scene_Create();
void Scene_Destroy(Scene *scene);

// GameObject 管理
void Scene_AddGameObject(Scene *scene, GameObject *gameObject);
void Scene_RemoveGameObject(Scene *scene, GameObject *gameObject);

// UIComponent 管理
void Scene_AddUIComponent(Scene *scene, UIComponent *uiComponent);
void Scene_RemoveUIComponent(Scene *scene, UIComponent *uiComponent);

// 生命周期方法
void Scene_Awake(Scene *scene);
void Scene_Start(Scene *scene);
void Scene_Update(Scene *scene);

// 渲染相关
void Scene_RenderUI(Scene *scene, SDL_Renderer *renderer);
void Scene_RenderGameObjects(Scene *scene, SDL_Renderer *renderer);
void Scene_Render(Scene *scene, SDL_Renderer *renderer, void *context);

#endif // SCENE_H
