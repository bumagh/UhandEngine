// Scene.c
#include "Scene.h"
#include <stdio.h>

void Scene_addGameObject(Scene *scene, GameObject *go) {
    go->next = scene->gameObjects;
    scene->gameObjects = go;
}

void Scene_removeGameObject(Scene *scene, const char *name) {
    GameObject *prev = NULL;
    GameObject *current = scene->gameObjects;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                scene->gameObjects = current->next;
            } else {
                prev->next = current->next;
            }
            current->free(current);  // 释放 GameObject
            return;
        }
        prev = current;
        current = current->next;
    }
}

void Scene_update(Scene *scene) {
    // GameObject *current = scene->gameObjects;
    // while (current != NULL) {
    //     current->update(current);
    //     current = current->next;
    // }
}
