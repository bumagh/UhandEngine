#ifndef gameobjectlist_h
#define gameobjectlist_h
#include "gameObject.h"

// 定义一个链表来存储所有的 GameObject
typedef struct GameObjectList
{
    GameObject *head; // 链表头指针
} GameObjectList;
// 初始化链表
void GameObjectList_Init(GameObjectList *list);

// 添加 GameObject 到链表中
void GameObjectList_Add(GameObjectList *list, GameObject *obj);

// 从链表中移除 GameObject
void GameObjectList_Remove(GameObjectList *list, GameObject *obj);

// 调用所有对象的 Awake 生命周期函数
void GameObjectList_CallAwake(GameObjectList *list);

// 调用所有对象的 Start 生命周期函数
void GameObjectList_CallStart(GameObjectList *list);

// 调用所有对象的 Update 生命周期函数
void GameObjectList_CallUpdate(GameObjectList *list);

// 调用所有对象的 LateUpdate 生命周期函数
void GameObjectList_CallLateUpdate(GameObjectList *list);

// 调用所有对象的 FixedUpdate 生命周期函数
void GameObjectList_CallFixedUpdate(GameObjectList *list);

// 调用所有对象的 Destroy 生命周期函数
void GameObjectList_CallDestroy(GameObjectList *list);
GameObjectList *createGameObjectList();

#endif