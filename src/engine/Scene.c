// Scene.c
#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 递归遍历 GameObject 树
static void UpdateGameObjectTree(GameObject *obj);

// Scene 创建和销毁
Scene *Scene_Create()
{
    Scene *scene = (Scene *)malloc(sizeof(Scene));
    if (scene == NULL)
    {
        printf("Failed to allocate memory for Scene\n");
        return NULL;
    }

    // 初始化所有字段
    memset(scene, 0, sizeof(Scene));

    // 创建根对象
    scene->rootObject = createGameObject("SceneRoot");
    if (scene->rootObject == NULL)
    {
        printf("Failed to create root GameObject for Scene\n");
        free(scene);
        return NULL;
    }

    scene->renderQueue = RenderQueue_Create();
    if (scene->renderQueue == NULL)
    {
        printf("Failed to create RenderQueue for Scene\n");
        freeGameObject(scene->rootObject);
        free(scene);
        return NULL;
    }

    return scene;
}

void Scene_Destroy(Scene *scene)
{
    if (scene == NULL)
        return;

    // 销毁根对象（会递归销毁所有子对象）
    if (scene->rootObject)
    {
        if (scene->rootObject->Destroy)
        {
            scene->rootObject->Destroy(scene->rootObject);
        }
        freeGameObject(scene->rootObject);
        scene->rootObject = NULL;
    }

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
    if (scene == NULL || scene->rootObject == NULL || go == NULL)
        return;

    // 将对象添加为根对象的子对象
    addChild(scene->rootObject, go);

    // 同时添加到渲染队列
    if (scene->renderQueue)
    {
        RenderQueue_Add(scene->renderQueue, go);
    }
}

void Scene_RemoveGameObject(Scene *scene, GameObject *obj)
{
    if (scene == NULL || obj == NULL)
        return;

    // 从父对象中移除
    if (obj->parent)
    {
        removeChild(obj->parent, obj);
    }
}

// Scene 生命周期方法
void Scene_Awake(Scene *scene)
{
    if (scene == NULL || scene->rootObject == NULL)
        return;

    scene->isAwake = 1;

    // 递归调用所有对象的 Awake
    UpdateGameObjectTree(scene->rootObject);

    // 调用场景自定义 Awake
    if (scene->Awake)
    {
        scene->Awake(scene);
    }
}

void Scene_Start(Scene *scene)
{
    if (scene == NULL || scene->rootObject == NULL)
        return;

    scene->isStarted = 1;

    // 递归调用所有对象的 Start
    GameObject *obj = scene->rootObject->firstChild;
    while (obj)
    {
        if (obj->Start)
        {
            obj->Start(obj);
        }
        obj = obj->nextSibling;
    }

    // 调用场景自定义 Start
    if (scene->Start)
    {
        scene->Start(scene);
    }
}

void Scene_Update(Scene *scene)
{
    if (scene == NULL || scene->rootObject == NULL)
        return;

    // 递归更新所有对象
    GameObject *obj = scene->rootObject->firstChild;
    while (obj)
    {
        UpdateGameObjectTree(obj);
        obj = obj->nextSibling;
    }

    // 调用场景自定义 Update
    if (scene->Update)
    {
        scene->Update(scene);
    }
}

// 递归更新 GameObject 树
static void UpdateGameObjectTree(GameObject *obj)
{
    if (!obj || !obj->active)
        return;

    // 更新当前对象
    if (obj->Update)
    {
        obj->Update(obj);
    }

    // 更新组件
    updateGameObject(obj);

    // 递归更新子对象
    GameObject *child = obj->firstChild;
    while (child)
    {
        UpdateGameObjectTree(child);
        child = child->nextSibling;
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

// 递归收集 GameObject 树到渲染队列（按 depth 排序）
static void CollectGameObjectsForRender(GameObject *obj, RenderQueue *queue)
{
    if (!obj || !obj->visible)
        return;

    // 先收集子对象
    GameObject *child = obj->firstChild;
    while (child)
    {
        CollectGameObjectsForRender(child, queue);
        child = child->nextSibling;
    }

    // 将当前对象添加到渲染队列（如果它有渲染能力）
    if (obj->active && (obj->render || obj->components))
    {
        RenderQueue_Add(queue, obj);
    }
}

void Scene_RenderGameObjects(Scene *scene, SDL_Renderer *renderer)
{
    if (scene == NULL || renderer == NULL || scene->rootObject == NULL)
        return;

    // 使用 RenderQueue 进行 depth 排序渲染
    if (scene->renderQueue == NULL)
    {
        scene->renderQueue = RenderQueue_Create();
    }

    // 清空队列
    RenderQueue_Clear(scene->renderQueue);

    // 收集所有 GameObject 到渲染队列
    CollectGameObjectsForRender(scene->rootObject, scene->renderQueue);

    // 按 depth 排序并渲染
    RenderQueue_Sort(scene->renderQueue);
    RenderQueue_Render(scene->renderQueue, renderer, NULL);
}

// 使用 RenderQueue 渲染 Scene（按 depth 排序）
void Scene_Render(Scene *scene, SDL_Renderer *renderer, void *context)
{
    if (scene == NULL || renderer == NULL)
        return;

    // 调用场景自定义 Render
    if (scene->Render)
    {
        scene->Render(scene, renderer);
    }
    else
    {
        // 默认渲染逻辑
        Scene_RenderGameObjects(scene, renderer);
    }

    // 排序并渲染渲染队列
    if (scene->renderQueue)
    {
        RenderQueue_Sort(scene->renderQueue);
        RenderQueue_Render(scene->renderQueue, renderer, context);
    }
}
