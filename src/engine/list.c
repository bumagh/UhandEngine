#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 初始化列表
List* list_init(size_t initialCapacity) {
    List* list = (List*)malloc(sizeof(List));
    if (list) {
        list->items = (void**)malloc(sizeof(void*) * initialCapacity);
        list->size = 0;
        list->capacity = initialCapacity;
    }
    return list;
}

// 释放列表
void list_free(List* list) {
    if (list) {
        if (list->items) {
            free(list->items);
        }
        free(list);
    }
}

// 动态增加列表容量
void list_resize(List* list, size_t newCapacity) {
    void** newItems = (void**)realloc(list->items, sizeof(void*) * newCapacity);
    if (newItems) {
        list->items = newItems;
        list->capacity = newCapacity;
    }
}

// 向列表中添加元素
void list_add(List* list, void* item) {
    if (list->size == list->capacity) {
        list_resize(list, list->capacity * 2); // 如果列表满了，扩大容量
    }
    list->items[list->size] = item;
    list->size++;
}

// 获取列表中指定索引的元素
void* list_get(List* list, size_t index) {
    if (index < list->size) {
        return list->items[index];
    }
    return NULL;
}

// 删除列表中指定索引的元素
void list_remove_at(List* list, size_t index) {
    if (index < list->size) {
        for (size_t i = index; i < list->size - 1; i++) {
            list->items[i] = list->items[i + 1];
        }
        list->size--;
        // 如果需要，可以缩小列表容量
    }
}

// 返回列表的大小
size_t list_size(List* list) {
    return list->size;
}

// 获取指定元素的索引，未找到则返回 -1
int list_index_of(struct List* list, void* item) {
    for (size_t i = 0; i < list->size; i++) {
        if (list->items[i] == item) {
            return i;
        }
    }
    return -1; // 未找到
}

// 删除指定元素
void list_remove(struct List* list, void* item) {
    int index = list_index_of(list, item);
    if (index != -1) {
        list_remove_at(list, index);
    }
}