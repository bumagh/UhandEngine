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

    scene->renderQueue = RenderQueue_Create();
    if (scene->renderQueue == NULL)
    {
        printf("Failed to create RenderQueue for Scene\n");
        free(scene->gameObjectList);
        free(scene);
        return NULL;
    }

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
    UIComponent *current = scene->uiComponents;
    while (current != NULL)
    {
        UIComponent *next = (UIComponent *)current->base.next;
        if (current->base.free)
        {
            current->base.free((Component *)current);
        }
        current = next;
    }
    scene->uiComponents = NULL;

    if (scene->renderQueue)
    {
        RenderQueue_Destroy(scene->renderQueue);
        scene->renderQueue = NULL;
    }

    free(scene);
}

// GameObject 管理
void Scene_AddGameObject(Scene *scene, GameObject *go)
{
    if (scene == NULL || scene->gameObjectList == NULL || go == NULL)
        return;

    GameObjectList_Add(scene->gameObjectList, go);

    // 同时添加到渲染队列
    if (scene->renderQueue)
    {
        RenderQueue_Add(scene->renderQueue, go);
    }
}

void Scene_RemoveGameObject(Scene *scene, GameObject *obj)
{
    if (scene == NULL || scene->gameObjectList == NULL || obj == NULL)
        return;

    GameObjectList_Remove(scene->gameObjectList, obj);
}

// UIComponent 管理（临时 MVP 支持）
void Scene_AddUIComponent(Scene *scene, UIComponent *uiComponent)
{
    if (scene == NULL || uiComponent == NULL)
        return;

    uiComponent->base.next = (Component *)scene->uiComponents;
    scene->uiComponents = uiComponent;
}

void Scene_RemoveUIComponent(Scene *scene, UIComponent *uiComponent)
{
    if (scene == NULL || uiComponent == NULL)
        return;

    UIComponent **current = &scene->uiComponents;
    while (*current)
    {
        if (*current == uiComponent)
        {
            *current = (UIComponent *)uiComponent->base.next;
            uiComponent->base.next = NULL;
            return;
        }
        current = (UIComponent **)&(*current)->base.next;
    }
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

    UIComponent *current = scene->uiComponents;
    while (current != NULL)
    {
        if (current->base.update)
        {
            current->base.update((Component *)current);
        }
        current = (UIComponent *)current->base.next;
    }
}

void Scene_RenderUI(Scene *scene, SDL_Renderer *renderer)
{
    if (scene == NULL || renderer == NULL)
        return;

    UIComponent *current = scene->uiComponents;
    while (current != NULL)
    {
        if (current->base.draw)
        {
            current->base.draw((Component *)current, renderer);
        }
        current = (UIComponent *)current->base.next;
    }
}

// 检查 GameObject 及其父对象是否可见
int IsGameObjectVisible(GameObject *go)
{
    if (go == NULL)
        return 0;

    // 检查自身可见性
    if (!go->visible)
        return 0;

    // 递归检查父对象可见性
    if (go->parent != NULL)
    {
        return IsGameObjectVisible(go->parent);
    }

    return 1;
}

void Scene_RenderGameObjects(Scene *scene, SDL_Renderer *renderer)
{
    if (scene == NULL || renderer == NULL || scene->gameObjectList == NULL)
        return;

    GameObject *current = scene->gameObjectList->head;

    // 第一遍：收集可见且激活的 GameObject
    // TODO: 后续优化为按 depth 排序
    while (current != NULL)
    {
        // 检查 active 和 visible 状态
        if (current->active && IsGameObjectVisible(current))
        {
            if (current->render)
            {
                current->render(current, renderer, NULL);
            }
        }
        current = current->next;
    }
}

// 使用 RenderQueue 渲染 Scene（按 depth 排序）
void Scene_Render(Scene *scene, SDL_Renderer *renderer, void *context)
{
    if (scene == NULL || renderer == NULL)
        return;

    // 排序渲染队列
    if (scene->renderQueue)
    {
        RenderQueue_Sort(scene->renderQueue);

        // 渲染队列中的所有对象
        RenderQueue_Render(scene->renderQueue, renderer, context);
    }
}

void Scene_DestroyAll(Scene *scene)
{
    if (scene == NULL || scene->gameObjectList == NULL)
        return;

    GameObjectList_CallDestroy(scene->gameObjectList);
}
