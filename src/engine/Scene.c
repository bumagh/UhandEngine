// Scene.c
#include "Scene.h"
#include "uiComponent.h"
#include <stdio.h>
#include <stdlib.h>

// Scene 创建和销毁
Scene *Scene_Create()
{
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    if (scene == NULL)
    {
        printf("Failed to allocate memory for Scene\n");
        return NULL;
    }

    scene->gameObjectList = createGameObjectList();
    if (scene->gameObjectList == NULL)
    {
        printf("Failed to create GameObjectList for Scene\n");
        free(scene);
        return NULL;
    }

    scene->uiComponents = NULL;

    return scene;
}

void Scene_Destroy(Scene *scene)
{
    if (scene == NULL)
        return;

    if (scene->gameObjectList)
    {
        GameObjectList_CallDestroy(scene->gameObjectList);
        free(scene->gameObjectList);
        scene->gameObjectList = NULL;
    }

    // 释放 UI 组件
    Component *current = scene->uiComponents;
    while (current != NULL)
    {
        Component *next = current->next;
        if (current->free)
        {
            current->free(current);
        }
        current = next;
    }
    scene->uiComponents = NULL;

    free(scene);
}

// GameObject 管理
void Scene_AddGameObject(Scene *scene, GameObject *go)
{
    if (scene == NULL || scene->gameObjectList == NULL || go == NULL)
        return;

    GameObjectList_Add(scene->gameObjectList, go);
}

void Scene_RemoveGameObject(Scene *scene, GameObject *obj)
{
    if (scene == NULL || scene->gameObjectList == NULL || obj == NULL)
        return;

    GameObjectList_Remove(scene->gameObjectList, obj);
}

// UIComponent 管理（临时 MVP 支持）
void Scene_AddUIComponent(Scene *scene, Component *uiComponent)
{
    if (scene == NULL || uiComponent == NULL)
        return;

    uiComponent->next = scene->uiComponents;
    scene->uiComponents = uiComponent;
}

// Scene 生命周期方法
void Scene_Awake(Scene *scene)
{
    if (scene == NULL || scene->gameObjectList == NULL)
        return;

    GameObjectList_CallAwake(scene->gameObjectList);
}

void Scene_Start(Scene *scene)
{
    if (scene == NULL || scene->gameObjectList == NULL)
        return;

    GameObjectList_CallStart(scene->gameObjectList);
}

void Scene_Update(Scene *scene)
{
    if (scene == NULL || scene->gameObjectList == NULL)
        return;

    GameObjectList_CallUpdate(scene->gameObjectList);
}

void Scene_UpdateUI(Scene *scene)
{
    if (scene == NULL)
        return;

    Component *current = scene->uiComponents;
    while (current != NULL)
    {
        if (current->update)
        {
            current->update(current);
        }
        current = current->next;
    }
}

void Scene_RenderUI(Scene *scene, SDL_Renderer *renderer)
{
    if (scene == NULL || renderer == NULL)
        return;

    Component *current = scene->uiComponents;
    while (current != NULL)
    {
        if (current->draw)
        {
            current->draw(current, renderer);
        }
        current = current->next;
    }
}

void Scene_DestroyAll(Scene *scene)
{
    if (scene == NULL || scene->gameObjectList == NULL)
        return;

    GameObjectList_CallDestroy(scene->gameObjectList);
}
