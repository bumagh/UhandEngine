#include "gameObjectList.h"
// 初始化链表
void GameObjectList_Init(GameObjectList *list)
{
    list->head = NULL;
}

// 添加 GameObject 到链表中
void GameObjectList_Add(GameObjectList *list, GameObject *obj)
{
    obj->next = list->head;
    list->head = obj;
}

// 从链表中移除 GameObject
void GameObjectList_Remove(GameObjectList *list, GameObject *obj)
{
    GameObject **current = &list->head;

    while (*current)
    {
        if (*current == obj)
        {
            *current = obj->next;
            free(obj);
            return;
        }
        current = &(*current)->next;
    }
}

// 调用所有对象的生命周期函数
void GameObjectList_CallAwake(GameObjectList *list)
{
    GameObject *current = list->head;
    while (current)
    {
        if (current->Awake)
            current->Awake(current);
        current = current->next;
    }
}

void GameObjectList_CallStart(GameObjectList *list)
{
    GameObject *current = list->head;
    while (current)
    {
        if (current->Start)
            current->Start(current);
        current = current->next;
    }
}

void GameObjectList_CallUpdate(GameObjectList *list)
{
    GameObject *current = list->head;
    while (current)
    {

        if (current->Update)
        {
            current->Update(current);
        }
        current = current->next;
    }
}

void GameObjectList_CallLateUpdate(GameObjectList *list)
{
    GameObject *current = list->head;
    while (current)
    {
        if (current->LateUpdate)
            current->LateUpdate(current);
        current = current->next;
    }
}

void GameObjectList_CallFixedUpdate(GameObjectList *list)
{
    GameObject *current = list->head;
    while (current)
    {
        if (current->FixedUpdate)
            current->FixedUpdate(current);
        current = current->next;
    }
}

void GameObjectList_CallDestroy(GameObjectList *list)
{
    GameObject *current = list->head;
    while (current)
    {
        if (current->Destroy)
            current->Destroy(current);
        else
            current = current->next;
    }
}
// 创建 GameObjectList
GameObjectList *createGameObjectList()
{
    // 为 GameObjectList 分配内存
    GameObjectList *list = (GameObjectList *)malloc(sizeof(GameObjectList));

    if (list == NULL)
    {
        printf("Failed to allocate memory for GameObjectList\n");
        return NULL;
    }

    // 初始化 GameObjectList 链表的头节点为空
    GameObjectList_Init(list);
    // 返回初始化后的 GameObjectList 指针
    return list;
}