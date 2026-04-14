/**
 * Sprite Example
 * 
 * 演示如何创建和使用 Sprite GameObject
 * 
 * 注意：当前 Sprite 是简化实现，需要资源管理系统支持纹理加载
 * 本示例展示 API 设计，实际运行需要先实现资源管理
 */

#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <stdio.h>

void sprite_example()
{
    printf("=== Sprite Example ===\n\n");

    // 创建一个 Sprite GameObject
    GameObject *sprite = createSprite("PlayerSprite", "assets/player.png", 100.0f, 200.0f);
    
    if (sprite)
    {
        printf("Created Sprite: %s\n", sprite->name);
        printf("Type: %d (GAMEOBJECT_TYPE_SPRITE = %d)\n", sprite->type, GAMEOBJECT_TYPE_SPRITE);
        
        if (sprite->transform)
        {
            printf("Position: (%.2f, %.2f)\n", sprite->transform->x, sprite->transform->y);
            
            // 设置旋转
            sprite->transform->setRotation(sprite->transform, 1.5708f); // 90度
            printf("Rotation: %.4f rad\n", sprite->transform->rotation);
            
            // 设置缩放
            sprite->transform->setScale(sprite->transform, 2.0f, 2.0f);
            printf("Scale: (%.2f, %.2f)\n", sprite->transform->scaleX, sprite->transform->scaleY);
            
            // 设置原点
            sprite->transform->setOrigin(sprite->transform, 0.5f, 0.5f);
            printf("Origin: (%.2f, %.2f)\n", sprite->transform->originX, sprite->transform->originY);
        }
        
        printf("Visible: %d\n", sprite->visible);
        printf("Active: %d\n", sprite->active);
        printf("Depth: %d\n\n", sprite->depth);
        
        // TODO: 需要实现 SpriteComponent 和资源管理才能实际渲染
        printf("Note: 实际渲染需要先实现 SpriteComponent 和资源管理系统\n");
        
        // 释放 (在实际使用中应该通过 Scene 管理)
        // sprite->free(sprite);
    }
    else
    {
        printf("Failed to create Sprite\n");
    }
}

int main()
{
    sprite_example();
    return 0;
}
