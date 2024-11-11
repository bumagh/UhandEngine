// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include "gameObject.h"

typedef struct Scene
{
    GameObject *gameObjects; // 指向场景中的 GameObject 链表
} Scene;

void Scene_addGameObject(Scene *scene, GameObject *go);
void Scene_removeGameObject(Scene *scene, const char *name);
void Scene_update(Scene *scene);

#endif
