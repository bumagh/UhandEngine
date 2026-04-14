#include "RenderQueue.h"
#include <stdlib.h>
#include <stdio.h>

// 创建渲染队列
RenderQueue *RenderQueue_Create()
{
    RenderQueue *queue = (RenderQueue *)malloc(sizeof(RenderQueue));
    if (!queue)
    {
        printf("Failed to allocate memory for RenderQueue\n");
        return NULL;
    }
    queue->head = NULL;
    return queue;
}

// 销毁渲染队列
void RenderQueue_Destroy(RenderQueue *queue)
{
    if (!queue)
        return;

    RenderQueue_Clear(queue);
    free(queue);
}

// 添加到渲染队列
void RenderQueue_Add(RenderQueue *queue, GameObject *gameObject)
{
    if (!queue || !gameObject)
        return;

    RenderQueueNode *node = (RenderQueueNode *)malloc(sizeof(RenderQueueNode));
    if (!node)
    {
        printf("Failed to allocate memory for RenderQueueNode\n");
        return;
    }

    node->gameObject = gameObject;
    node->next = queue->head;
    queue->head = node;
}

// 按 depth 排序（从小到大）- 使用冒泡排序
void RenderQueue_Sort(RenderQueue *queue)
{
    if (!queue || !queue->head)
        return;

    int swapped;
    RenderQueueNode *ptr1;
    RenderQueueNode *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = queue->head;

        while (ptr1->next != lptr)
        {
            if (ptr1->gameObject->depth > ptr1->next->gameObject->depth)
            {
                // 交换 GameObject 指针
                GameObject *temp = ptr1->gameObject;
                ptr1->gameObject = ptr1->next->gameObject;
                ptr1->next->gameObject = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// 渲染队列中的所有对象
void RenderQueue_Render(RenderQueue *queue, SDL_Renderer *renderer, void *context)
{
    if (!queue || !renderer)
        return;

    RenderQueueNode *current = queue->head;
    while (current)
    {
        GameObject *go = current->gameObject;

        // 检查可见性和激活状态
        if (go && go->visible && go->active)
        {
            // 检查父对象可见性
            int parentVisible = 1;
            GameObject *parent = go->parent;
            while (parent)
            {
                if (!parent->visible)
                {
                    parentVisible = 0;
                    break;
                }
                parent = parent->parent;
            }

            if (parentVisible)
            {
                // 调用 GameObject 的渲染方法
                if (go->render)
                {
                    go->render(go, renderer, context);
                }
            }
        }

        current = current->next;
    }
}

// 清空队列
void RenderQueue_Clear(RenderQueue *queue)
{
    if (!queue)
        return;

    RenderQueueNode *current = queue->head;
    while (current)
    {
        RenderQueueNode *next = current->next;
        free(current);
        current = next;
    }
    queue->head = NULL;
}
