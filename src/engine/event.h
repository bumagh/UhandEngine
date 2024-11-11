#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义回调函数的类型，接收可变参数
typedef void (*Callback)(void **args, int argCount);

// 定义事件结构体
typedef struct Event {
    char *type;          // 事件类型
    Callback callback;   // 回调函数
    void *target;        // 目标对象，若无目标可为 NULL
    struct Event *next;  // 链表指向下一个事件
} Event;

// 定义事件管理器结构体，存储所有事件
typedef struct EventManager {
    Event *head;  // 链表头指针
} EventManager;

// 初始化事件管理器
EventManager *createEventManager();

// 注册事件
void on(EventManager *manager, const char *type, Callback callback, void *target);

// 注销事件
void off(EventManager *manager, const char *type, Callback callback, void *target);

// 触发事件
void emit(EventManager *manager, const char *type, void **args, int argCount);

// 释放事件管理器及其所有事件
void freeEventManager(EventManager *manager);

#endif
