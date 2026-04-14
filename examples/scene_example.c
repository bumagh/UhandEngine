/**
 * Scene Example
 * 
 * 演示如何在 Scene 中使用 GameObject 的 2D 基础能力
 */

#include "../src/engine/Scene.h"
#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void scene_example()
{
    printf("=== Scene Example ===\n\n");

    // 创建 Scene
    Scene *scene = Scene_Create();
    
    if (scene)
    {
        printf("Created Scene successfully\n\n");
        
        // 创建多个 GameObject 并添加到 Scene
        GameObject *background = createGameObject("Background");
        GameObject *player = createGameObject("Player");
        GameObject *ui = createContainer("UIContainer");
        
        if (background && player && ui)
        {
            // 设置属性
            setDepth(background, 0);
            setDepth(player, 10);
            setDepth(ui, 20);
            
            // 设置位置
            if (background->transform)
                background->transform->setPosition(background->transform, 0.0f, 0.0f);
            if (player->transform)
                player->transform->setPosition(player->transform, 100.0f, 100.0f);
            if (ui->transform)
                ui->transform->setPosition(ui->transform, 0.0f, 0.0f);
            
            // 添加到 Scene
            Scene_AddGameObject(scene, background);
            Scene_AddGameObject(scene, player);
            Scene_AddGameObject(scene, ui);
            
            printf("Added GameObjects to Scene:\n");
            printf("- %s (depth: %d)\n", background->name, background->depth);
            printf("- %s (depth: %d)\n", player->name, player->depth);
            printf("- %s (depth: %d)\n", ui->name, ui->depth);
            printf("\n");
            
            // 演示父子关系
            GameObject *healthBar = createGameObject("HealthBar");
            if (healthBar && ui)
            {
                addChild(ui, healthBar);
                printf("Added %s as child of %s\n", healthBar->name, ui->name);
                printf("%s parent: %s\n", healthBar->name, 
                       healthBar->parent ? healthBar->parent->name : "NULL");
                printf("\n");
            }
            
            // 演示可见性控制
            printf("=== Visibility Control ===\n");
            printf("Before: %s visible = %d\n", player->name, player->visible);
            setVisible(player, 0);
            printf("After: %s visible = %d\n", player->name, player->visible);
            printf("(Player will not be rendered)\n\n");
            
            // 演示激活状态控制
            printf("=== Active State Control ===\n");
            printf("Before: %s active = %d\n", background->name, background->active);
            setActive(background, 0);
            printf("After: %s active = %d\n", background->name, background->active);
            printf("(Background will not be updated)\n\n");
            
            // 演示 Transform 2D 属性
            printf("=== Transform 2D Properties ===\n");
            if (player->transform)
            {
                player->transform->setRotation(player->transform, 0.7854f); // 45度
                player->transform->setScale(player->transform, 1.5f, 1.5f);
                player->transform->setOrigin(player->transform, 0.5f, 0.5f);
                
                printf("%s Transform:\n", player->name);
                printf("  Position: (%.2f, %.2f)\n", player->transform->x, player->transform->y);
                printf("  Rotation: %.4f rad\n", player->transform->rotation);
                printf("  Scale: (%.2f, %.2f)\n", player->transform->scaleX, player->transform->scaleY);
                printf("  Origin: (%.2f, %.2f)\n", player->transform->originX, player->transform->originY);
                printf("\n");
            }
            
            printf("=== Scene Lifecycle ===\n");
            printf("Calling Scene_Awake...\n");
            Scene_Awake(scene);
            
            printf("Calling Scene_Start...\n");
            Scene_Start(scene);
            
            printf("Calling Scene_Update...\n");
            Scene_Update(scene);
            printf("\n");
            
            printf("Note: Scene_RenderGameObjects 需要 SDL_Renderer 才能实际渲染\n");
        }
        
        // 清理
        printf("=== Cleanup ===\n");
        Scene_Destroy(scene);
        printf("Scene destroyed\n");
    }
    else
    {
        printf("Failed to create Scene\n");
    }
}

int main()
{
    scene_example();
    return 0;
}
