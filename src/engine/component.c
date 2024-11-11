#include <stdlib.h>
#include <string.h>
#include "component.h"

// 添加新组件到链表
void add_component(Component** head, const char* name, void (*update_func)(Component*)) {
    Component* new_component = (Component*)malloc(sizeof(Component));
    new_component->name = strdup(name); // 复制字符串
    new_component->update = update_func;
    new_component->next = *head; // 将新组件链接到链表头
    *head = new_component; // 更新头指针
}

// 更新所有组件
void update_components(Component* head) {
    Component* current = head;
    while (current != NULL) {
        if (current->update != NULL) {
            current->update(current); // 调用更新函数
        }
        current = current->next; // 移动到下一个组件
    }
}
