#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <SDL2/SDL.h>
#include "gameObject.h"

// 渲染队列节点
typedef struct RenderQueueNode {
    GameObject *gameObject;  // 渲染对象
    struct RenderQueueNode *next;
} RenderQueueNode;

// 渲染队列
typedef struct RenderQueue {
    RenderQueueNode *head;
} RenderQueue;

// 函数声明
RenderQueue *RenderQueue_Create();
void RenderQueue_Destroy(RenderQueue *queue);

// 添加到渲染队列
void RenderQueue_Add(RenderQueue *queue, GameObject *gameObject);

// 按 depth 排序（从小到大）
void RenderQueue_Sort(RenderQueue *queue);

// 渲染队列中的所有对象（需要传入 renderer 和 context）
void RenderQueue_Render(RenderQueue *queue, SDL_Renderer *renderer, void *context);

// 清空队列
void RenderQueue_Clear(RenderQueue *queue);

#endif // RENDER_QUEUE_H
