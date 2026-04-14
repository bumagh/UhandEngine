// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"
#include "gameObjectList.h"
#include "component.h"
#include <SDL2/SDL.h>

typedef struct Scene
{
    GameObjectList *gameObjectList; // 场景中的 GameObject 链表容器
    Component *uiComponents;       // 场景中的 UI 组件链表（临时用于 MVP）
} Scene;

// Scene 创建和销毁
Scene *Scene_Create();
void Scene_Destroy(Scene *scene);

// GameObject 管理
void Scene_AddGameObject(Scene *scene, GameObject *go);
void Scene_RemoveGameObject(Scene *scene, GameObject *obj);

// UIComponent 管理（临时 MVP 支持）
void Scene_AddUIComponent(Scene *scene, Component *uiComponent);

// Scene 生命周期方法
void Scene_Awake(Scene *scene);
void Scene_Start(Scene *scene);
void Scene_Update(Scene *scene);
void Scene_UpdateUI(Scene *scene);
void Scene_RenderGameObjects(Scene *scene, SDL_Renderer *renderer);
void Scene_RenderUI(Scene *scene, SDL_Renderer *renderer);
void Scene_DestroyAll(Scene *scene);

#endif
