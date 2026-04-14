/**
 * Text Example
 *
 * 演示如何创建和使用 Text GameObject
 *
 * 注意：当前 Text 是简化实现，需要集成 TextComponent
 * 本示例展示 API 设计，实际运行需要先完成 TextComponent 集成
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/gameObject.h"
#include "../src/engine/transform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

void text_example()
{
    printf("=== Text Example ===\n\n");

    // 初始化 SDL 和 TTF (示例需要)
    // SDL_Init(SDL_INIT_VIDEO);
    // TTF_Init();

    // 假设已加载字体
    // TTF_Font *font = TTF_OpenFont("assets/fzpix.ttf", 24);
    TTF_Font *font = NULL; // 占位符
    SDL_Color color = {255, 255, 255, 255};

    // 创建一个 Text GameObject
    GameObject *text = createText("ScoreText", "Score: 100", font, color, 50.0f, 50.0f);
    
    if (text)
    {
        printf("Created Text: %s\n", text->name);
        printf("Type: %d (GAMEOBJECT_TYPE_TEXT = %d)\n", text->type, GAMEOBJECT_TYPE_TEXT);
        
        if (text->transform)
        {
            printf("Position: (%.2f, %.2f)\n", text->transform->x, text->transform->y);
        }
        
        printf("Visible: %d\n", text->visible);
        printf("Active: %d\n", text->active);
        printf("Depth: %d\n\n", text->depth);
        
        // TODO: 需要集成 TextComponent 才能实际渲染
        printf("Note: 实际渲染需要先集成 TextComponent\n");
        
        // 释放 (在实际使用中应该通过 Scene 管理)
        // text->free(text);
    }
    else
    {
        printf("Failed to create Text\n");
    }

    // TTF_CloseFont(font);
    // TTF_Quit();
    // SDL_Quit();
}

int main(int argc, char *argv[])
{
    text_example();
    return 0;
}
