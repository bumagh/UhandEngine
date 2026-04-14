#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>

// 资源类型
typedef enum {
    ASSET_TYPE_TEXTURE,
    ASSET_TYPE_FONT,
    ASSET_TYPE_SOUND,
    ASSET_TYPE_MUSIC
} AssetType;

// 资源节点
typedef struct AssetNode {
    char *key;              // 资源键名
    AssetType type;         // 资源类型
    void *data;             // 资源数据
    int refCount;           // 引用计数
    struct AssetNode *next; // 下一个节点
} AssetNode;

// 资源管理器
typedef struct AssetManager {
    AssetNode *head;        // 资源链表头
    char *basePath;         // 基础路径
} AssetManager;

// 函数声明
AssetManager *AssetManager_Create(const char *basePath);
void AssetManager_Destroy(AssetManager *manager);

// 纹理管理
SDL_Texture *AssetManager_LoadTexture(AssetManager *manager, const char *key, const char *filename, SDL_Renderer *renderer);
SDL_Texture *AssetManager_GetTexture(AssetManager *manager, const char *key);
void AssetManager_UnloadTexture(AssetManager *manager, const char *key);

// 字体管理
TTF_Font *AssetManager_LoadFont(AssetManager *manager, const char *key, const char *filename, int size);
TTF_Font *AssetManager_GetFont(AssetManager *manager, const char *key);
void AssetManager_UnloadFont(AssetManager *manager, const char *key);

// 声音管理
Mix_Chunk *AssetManager_LoadSound(AssetManager *manager, const char *key, const char *filename);
Mix_Chunk *AssetManager_GetSound(AssetManager *manager, const char *key);
void AssetManager_UnloadSound(AssetManager *manager, const char *key);

// 音乐管理
Mix_Music *AssetManager_LoadMusic(AssetManager *manager, const char *key, const char *filename);
Mix_Music *AssetManager_GetMusic(AssetManager *manager, const char *key);
void AssetManager_UnloadMusic(AssetManager *manager, const char *key);

// 清理所有资源
void AssetManager_Clear(AssetManager *manager);

#endif // ASSET_MANAGER_H
