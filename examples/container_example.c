/**
 * Container Example
 *
 * 演示如何创建 Container 并管理父子关系
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <stdio.h>

void container_example()
{
    printf("=== Container Example ===\n\n");

    // 创建一个 Container (作为父对象)
    GameObject *container = createContainer("MainContainer");
    
    if (container)
    {
        printf("Created Container: %s\n", container->name);
        printf("Type: %d (GAMEOBJECT_TYPE_CONTAINER = %d)\n", container->type, GAMEOBJECT_TYPE_CONTAINER);
        printf("Children: %s\n", container->children ? "has children" : "no children");
        
        // 设置 Container 位置
        if (container->transform)
        {
            container->transform->setPosition(container->transform, 100.0f, 100.0f);
            printf("Container Position: (%.2f, %.2f)\n", container->transform->x, container->transform->y);
        }
        printf("\n");
    }

    // 创建子对象
    GameObject *child1 = createGameObject("Child1");
    GameObject *child2 = createGameObject("Child2");
    
    if (child1 && child2)
    {
        printf("Created Child1: %s\n", child1->name);
        printf("Created Child2: %s\n\n", child2->name);
        
        // 设置子对象位置（相对于父对象）
        if (child1->transform)
        {
            child1->transform->setPosition(child1->transform, 50.0f, 50.0f);
        }
        if (child2->transform)
        {
            child2->transform->setPosition(child2->transform, 150.0f, 150.0f);
        }
    }

    // 将子对象添加到 Container
    if (container && child1 && child2)
    {
        addChild(container, child1);
        addChild(container, child2);
        
        printf("After adding children:\n");
        printf("Container %s has children: %s\n", container->name, container->children ? "yes" : "no");
        
        if (container->children)
        {
            printf("First child: %s\n", container->children->name);
            if (container->children->next)
            {
                printf("Second child: %s\n", container->children->next->name);
            }
        }
        
        // 检查父子关系
        printf("\nParent relationship:\n");
        printf("Child1 parent: %s\n", child1->parent ? child1->parent->name : "NULL");
        printf("Child2 parent: %s\n", child2->parent ? child2->parent->name : "NULL");
    }

    printf("\n=== Testing visibility inheritance ===\n");
    
    // 设置 Container 不可见
    if (container)
    {
        setVisible(container, 0);
        printf("Container visible: %d\n", container->visible);
        printf("Child1 should be invisible due to parent\n");
        printf("Child2 should be invisible due to parent\n");
    }

    printf("\n=== Testing removeChild ===\n");
    
    // 移除一个子对象
    if (container && child1)
    {
        removeChild(container, child1);
        printf("After removing Child1:\n");
        printf("Child1 parent: %s\n", child1->parent ? child1->parent->name : "NULL");
        printf("Container first child: %s\n", 
               container->children ? container->children->name : "NULL");
    }

    printf("\nNote: 实际使用中应该通过 Scene 管理 GameObject 生命周期\n");
}

int main(int argc, char *argv[])
{
    container_example();
    return 0;
}
