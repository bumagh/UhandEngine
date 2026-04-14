#include "AssetManager.h"
#include <stdlib.h>

// 创建资源管理器
AssetManager *AssetManager_Create(const char *basePath)
{
    AssetManager *manager = (AssetManager *)malloc(sizeof(AssetManager));
    if (!manager)
    {
        printf("Failed to allocate memory for AssetManager\n");
        return NULL;
    }

    manager->head = NULL;
    if (basePath)
    {
        manager->basePath = strdup(basePath);
    }
    else
    {
        manager->basePath = strdup("./assets/");
    }

    printf("AssetManager created with base path: %s\n", manager->basePath);
    return manager;
}

// 销毁资源管理器
void AssetManager_Destroy(AssetManager *manager)
{
    if (!manager)
        return;

    // 清理所有资源
    AssetManager_Clear(manager);

    if (manager->basePath)
    {
        free(manager->basePath);
    }

    free(manager);
    printf("AssetManager destroyed\n");
}

// 查找资源节点
static AssetNode *FindAsset(AssetManager *manager, const char *key)
{
    AssetNode *current = manager->head;
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 添加资源节点
static AssetNode *AddAsset(AssetManager *manager, const char *key, AssetType type, void *data)
{
    AssetNode *node = (AssetNode *)malloc(sizeof(AssetNode));
    if (!node)
    {
        printf("Failed to allocate memory for AssetNode\n");
        return NULL;
    }

    node->key = strdup(key);
    node->type = type;
    node->data = data;
    node->refCount = 1;
    node->next = manager->head;
    manager->head = node;

    return node;
}

// 加载纹理
SDL_Texture *AssetManager_LoadTexture(AssetManager *manager, const char *key, const char *filename, SDL_Renderer *renderer)
{
    if (!manager || !key || !filename || !renderer)
        return NULL;

    // 检查是否已加载
    AssetNode *node = FindAsset(manager, key);
    if (node)
    {
        node->refCount++;
        printf("Texture %s already loaded (refCount: %d)\n", key, node->refCount);
        return (SDL_Texture *)node->data;
    }

    // 构建完整路径
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", manager->basePath, filename);

    // 加载图片
    SDL_Surface *surface = IMG_Load(fullPath);
    if (!surface)
    {
        printf("Failed to load image %s: %s\n", fullPath, IMG_GetError());
        return NULL;
    }

    // 创建纹理
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("Failed to create texture from %s: %s\n", fullPath, SDL_GetError());
        return NULL;
    }

    // 添加到管理器
    AddAsset(manager, key, ASSET_TYPE_TEXTURE, texture);
    printf("Loaded texture: %s from %s\n", key, fullPath);

    return texture;
}

// 获取纹理
SDL_Texture *AssetManager_GetTexture(AssetManager *manager, const char *key)
{
    AssetNode *node = FindAsset(manager, key);
    if (node && node->type == ASSET_TYPE_TEXTURE)
    {
        return (SDL_Texture *)node->data;
    }
    return NULL;
}

// 卸载纹理
void AssetManager_UnloadTexture(AssetManager *manager, const char *key)
{
    AssetNode **current = &manager->head;
    while (*current)
    {
        if (strcmp((*current)->key, key) == 0 && (*current)->type == ASSET_TYPE_TEXTURE)
        {
            AssetNode *node = *current;
            node->refCount--;
            if (node->refCount <= 0)
            {
                *current = node->next;
                SDL_DestroyTexture((SDL_Texture *)node->data);
                free(node->key);
                free(node);
                printf("Unloaded texture: %s\n", key);
            }
            else
            {
                printf("Texture %s refCount decreased to %d\n", key, node->refCount);
            }
            return;
        }
        current = &(*current)->next;
    }
}

// 加载字体
TTF_Font *AssetManager_LoadFont(AssetManager *manager, const char *key, const char *filename, int size)
{
    if (!manager || !key || !filename)
        return NULL;

    // 检查是否已加载
    AssetNode *node = FindAsset(manager, key);
    if (node)
    {
        node->refCount++;
        printf("Font %s already loaded (refCount: %d)\n", key, node->refCount);
        return (TTF_Font *)node->data;
    }

    // 构建完整路径
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", manager->basePath, filename);

    // 加载字体
    TTF_Font *font = TTF_OpenFont(fullPath, size);
    if (!font)
    {
        printf("Failed to load font %s: %s\n", fullPath, TTF_GetError());
        return NULL;
    }

    // 添加到管理器
    AddAsset(manager, key, ASSET_TYPE_FONT, font);
    printf("Loaded font: %s from %s (size: %d)\n", key, fullPath, size);

    return font;
}

// 获取字体
TTF_Font *AssetManager_GetFont(AssetManager *manager, const char *key)
{
    AssetNode *node = FindAsset(manager, key);
    if (node && node->type == ASSET_TYPE_FONT)
    {
        return (TTF_Font *)node->data;
    }
    return NULL;
}

// 卸载字体
void AssetManager_UnloadFont(AssetManager *manager, const char *key)
{
    AssetNode **current = &manager->head;
    while (*current)
    {
        if (strcmp((*current)->key, key) == 0 && (*current)->type == ASSET_TYPE_FONT)
        {
            AssetNode *node = *current;
            node->refCount--;
            if (node->refCount <= 0)
            {
                *current = node->next;
                TTF_CloseFont((TTF_Font *)node->data);
                free(node->key);
                free(node);
                printf("Unloaded font: %s\n", key);
            }
            else
            {
                printf("Font %s refCount decreased to %d\n", key, node->refCount);
            }
            return;
        }
        current = &(*current)->next;
    }
}

// 加载声音
Mix_Chunk *AssetManager_LoadSound(AssetManager *manager, const char *key, const char *filename)
{
    if (!manager || !key || !filename)
        return NULL;

    // 检查是否已加载
    AssetNode *node = FindAsset(manager, key);
    if (node)
    {
        node->refCount++;
        printf("Sound %s already loaded (refCount: %d)\n", key, node->refCount);
        return (Mix_Chunk *)node->data;
    }

    // 构建完整路径
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", manager->basePath, filename);

    // 加载声音
    Mix_Chunk *sound = Mix_LoadWAV(fullPath);
    if (!sound)
    {
        printf("Failed to load sound %s: %s\n", fullPath, Mix_GetError());
        return NULL;
    }

    // 添加到管理器
    AddAsset(manager, key, ASSET_TYPE_SOUND, sound);
    printf("Loaded sound: %s from %s\n", key, fullPath);

    return sound;
}

// 获取声音
Mix_Chunk *AssetManager_GetSound(AssetManager *manager, const char *key)
{
    AssetNode *node = FindAsset(manager, key);
    if (node && node->type == ASSET_TYPE_SOUND)
    {
        return (Mix_Chunk *)node->data;
    }
    return NULL;
}

// 卸载声音
void AssetManager_UnloadSound(AssetManager *manager, const char *key)
{
    AssetNode **current = &manager->head;
    while (*current)
    {
        if (strcmp((*current)->key, key) == 0 && (*current)->type == ASSET_TYPE_SOUND)
        {
            AssetNode *node = *current;
            node->refCount--;
            if (node->refCount <= 0)
            {
                *current = node->next;
                Mix_FreeChunk((Mix_Chunk *)node->data);
                free(node->key);
                free(node);
                printf("Unloaded sound: %s\n", key);
            }
            else
            {
                printf("Sound %s refCount decreased to %d\n", key, node->refCount);
            }
            return;
        }
        current = &(*current)->next;
    }
}

// 加载音乐
Mix_Music *AssetManager_LoadMusic(AssetManager *manager, const char *key, const char *filename)
{
    if (!manager || !key || !filename)
        return NULL;

    // 检查是否已加载
    AssetNode *node = FindAsset(manager, key);
    if (node)
    {
        node->refCount++;
        printf("Music %s already loaded (refCount: %d)\n", key, node->refCount);
        return (Mix_Music *)node->data;
    }

    // 构建完整路径
    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s%s", manager->basePath, filename);

    // 加载音乐
    Mix_Music *music = Mix_LoadMUS(fullPath);
    if (!music)
    {
        printf("Failed to load music %s: %s\n", fullPath, Mix_GetError());
        return NULL;
    }

    // 添加到管理器
    AddAsset(manager, key, ASSET_TYPE_MUSIC, music);
    printf("Loaded music: %s from %s\n", key, fullPath);

    return music;
}

// 获取音乐
Mix_Music *AssetManager_GetMusic(AssetManager *manager, const char *key)
{
    AssetNode *node = FindAsset(manager, key);
    if (node && node->type == ASSET_TYPE_MUSIC)
    {
        return (Mix_Music *)node->data;
    }
    return NULL;
}

// 卸载音乐
void AssetManager_UnloadMusic(AssetManager *manager, const char *key)
{
    AssetNode **current = &manager->head;
    while (*current)
    {
        if (strcmp((*current)->key, key) == 0 && (*current)->type == ASSET_TYPE_MUSIC)
        {
            AssetNode *node = *current;
            node->refCount--;
            if (node->refCount <= 0)
            {
                *current = node->next;
                Mix_FreeMusic((Mix_Music *)node->data);
                free(node->key);
                free(node);
                printf("Unloaded music: %s\n", key);
            }
            else
            {
                printf("Music %s refCount decreased to %d\n", key, node->refCount);
            }
            return;
        }
        current = &(*current)->next;
    }
}

// 清理所有资源
void AssetManager_Clear(AssetManager *manager)
{
    AssetNode *current = manager->head;
    while (current)
    {
        AssetNode *next = current->next;

        // 根据类型释放资源
        switch (current->type)
        {
            case ASSET_TYPE_TEXTURE:
                SDL_DestroyTexture((SDL_Texture *)current->data);
                break;
            case ASSET_TYPE_FONT:
                TTF_CloseFont((TTF_Font *)current->data);
                break;
            case ASSET_TYPE_SOUND:
                Mix_FreeChunk((Mix_Chunk *)current->data);
                break;
            case ASSET_TYPE_MUSIC:
                Mix_FreeMusic((Mix_Music *)current->data);
                break;
        }

        free(current->key);
        free(current);
        current = next;
    }

    manager->head = NULL;
    printf("All assets cleared\n");
}
