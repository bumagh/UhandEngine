#ifndef list_h
#define list_h

#include <stdlib.h>

// 列表结构体
typedef struct List
{
    void **items;    // 存储元素的指针数组
    size_t size;     // 当前存储元素的数量
    size_t capacity; // 当前容量
} List;

// 初始化列表
List *list_init(size_t initialCapacity);

// 释放列表
void list_free(struct List *list);

// 向列表中添加元素
void list_add(struct List *list, void *item);

// 获取列表中指定索引的元素
void *list_get(struct List *list, size_t index);

// 删除列表中指定索引的元素
void list_remove_at(struct List *list, size_t index);

// 返回列表的大小
size_t list_size(struct List *list);
// 获取指定元素的索引，未找到则返回 -1
int list_index_of(struct List *list, void *item);

// 删除指定元素
void list_remove(struct List *list, void *item);
#endif
