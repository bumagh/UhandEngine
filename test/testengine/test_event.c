#include "../../src/engine/event.h"

// 定义一个回调函数
void onPlayerMove(void **args, int argCount) {
    if (argCount >= 1) {
        printf("Player moved to position: %d\n", *(int *)args[0]);
    }
}

int main() {
    EventManager *eventManager = createEventManager();

    // 注册事件
    on(eventManager, "PlayerMove", onPlayerMove, NULL);

    // 触发事件
    int position = 10;
    void *args[] = { &position };
    emit(eventManager, "PlayerMove", args, 1);

    // 注销事件
    off(eventManager, "PlayerMove", onPlayerMove, NULL);

    // 释放事件管理器
    freeEventManager(eventManager);

    return 0;
}
