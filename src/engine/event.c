#include "event.h"

// 初始化事件管理器
EventManager *createEventManager() {
    EventManager *manager = (EventManager *)malloc(sizeof(EventManager));
    manager->head = NULL;
    return manager;
}

// 注册事件
void on(EventManager *manager, const char *type, Callback callback, void *target) {
    Event *newEvent = (Event *)malloc(sizeof(Event));
    newEvent->type = strdup(type);  // 复制事件类型
    newEvent->callback = callback;
    newEvent->target = target;
    newEvent->next = NULL;

    // 将新事件添加到链表中
    if (manager->head == NULL) {
        manager->head = newEvent;
    } else {
        Event *current = manager->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newEvent;
    }
}

// 注销事件
void off(EventManager *manager, const char *type, Callback callback, void *target) {
    Event *current = manager->head;
    Event *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->type, type) == 0 && current->callback == callback && current->target == target) {
            if (prev == NULL) {
                manager->head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->type);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// 触发事件
void emit(EventManager *manager, const char *type, void **args, int argCount) {
    Event *current = manager->head;

    while (current != NULL) {
        if (strcmp(current->type, type) == 0) {
            current->callback(args, argCount);  // 调用回调函数
        }
        current = current->next;
    }
}

// 释放事件管理器及其所有事件
void freeEventManager(EventManager *manager) {
    Event *current = manager->head;
    while (current != NULL) {
        Event *next = current->next;
        free(current->type);
        free(current);
        current = next;
    }
    free(manager);
}
