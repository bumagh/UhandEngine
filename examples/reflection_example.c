/**
 * Component Type Reflection Example
 *
 * 演示组件类型反射系统的使用，这是AI友好的基础
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/ComponentType.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 示例组件：Transform
typedef struct {
    float x, y;
    float rotation;
    float scaleX, scaleY;
} TransformComponent;

// 示例组件：Sprite
typedef struct {
    char texturePath[256];
    int width, height;
    int frame;
    float alpha;
} SpriteComponent;

// Transform 属性描述
static PropertyInfo transformProperties[] = {
    {"x", PROPERTY_TYPE_FLOAT, offsetof(TransformComponent, x), "X position"},
    {"y", PROPERTY_TYPE_FLOAT, offsetof(TransformComponent, y), "Y position"},
    {"rotation", PROPERTY_TYPE_FLOAT, offsetof(TransformComponent, rotation), "Rotation in degrees"},
    {"scaleX", PROPERTY_TYPE_FLOAT, offsetof(TransformComponent, scaleX), "X scale"},
    {"scaleY", PROPERTY_TYPE_FLOAT, offsetof(TransformComponent, scaleY), "Y scale"}
};

// Sprite 属性描述
static PropertyInfo spriteProperties[] = {
    {"texturePath", PROPERTY_TYPE_STRING, offsetof(SpriteComponent, texturePath), "Path to texture file"},
    {"width", PROPERTY_TYPE_INT, offsetof(SpriteComponent, width), "Sprite width"},
    {"height", PROPERTY_TYPE_INT, offsetof(SpriteComponent, height), "Sprite height"},
    {"frame", PROPERTY_TYPE_INT, offsetof(SpriteComponent, frame), "Current animation frame"},
    {"alpha", PROPERTY_TYPE_FLOAT, offsetof(SpriteComponent, alpha), "Transparency (0.0-1.0)"}
};

// Transform 类型描述
static ComponentTypeDescriptor transformDescriptor = {
    "Transform",
    COMPONENT_TYPE_TRANSFORM,
    sizeof(TransformComponent),
    transformProperties,
    sizeof(transformProperties) / sizeof(transformProperties[0]),
    NULL,
    NULL,
    NULL
};

// Sprite 类型描述
static ComponentTypeDescriptor spriteDescriptor = {
    "Sprite",
    COMPONENT_TYPE_SPRITE,
    sizeof(SpriteComponent),
    spriteProperties,
    sizeof(spriteProperties) / sizeof(spriteProperties[0]),
    NULL,
    NULL,
    NULL
};

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    printf("=== Component Type Reflection Example ===\n\n");
    printf("This example demonstrates AI-friendly component reflection.\n");
    printf("AI can query component types, properties, and modify them at runtime.\n\n");

    // 注册组件类型
    ComponentType_Register(&transformDescriptor);
    ComponentType_Register(&spriteDescriptor);

    printf("Registered component types:\n");
    ComponentTypeDescriptor *descriptors[32];
    int count = ComponentType_GetAllTypes(descriptors, 32);
    for (int i = 0; i < count; i++)
    {
        printf("  - %s (Type: %d, Size: %I64u bytes, Properties: %d)\n",
               descriptors[i]->name,
               descriptors[i]->type,
               (unsigned long long)descriptors[i]->size,
               descriptors[i]->propertyCount);
    }

    printf("\n");

    // 创建组件实例
    TransformComponent transform = {100.0f, 200.0f, 45.0f, 1.0f, 1.0f};
    SpriteComponent sprite = {0};
    strcpy(sprite.texturePath, "assets/player.png");
    sprite.width = 64;
    sprite.height = 64;
    sprite.frame = 0;
    sprite.alpha = 1.0f;

    printf("Initial Transform values:\n");
    printf("  x: %.1f, y: %.1f, rotation: %.1f\n", transform.x, transform.y, transform.rotation);

    // AI查询：获取Transform的x属性
    PropertyInfo *xProp = ComponentType_FindProperty(&transformDescriptor, "x");
    if (xProp)
    {
        float xValue;
        ComponentType_GetProperty(&transform, xProp, &xValue);
        printf("\nAI Query: Transform.x = %.1f\n", xValue);
    }

    // AI操作：设置Transform的x属性
    float newX = 300.0f;
    if (xProp)
    {
        ComponentType_SetProperty(&transform, xProp, &newX);
        printf("AI Action: Set Transform.x to %.1f\n", newX);
    }

    printf("\nUpdated Transform values:\n");
    printf("  x: %.1f, y: %.1f, rotation: %.1f\n", transform.x, transform.y, transform.rotation);

    // 列出所有Transform属性
    printf("\nTransform properties:\n");
    for (int i = 0; i < transformDescriptor.propertyCount; i++)
    {
        PropertyInfo *prop = &transformDescriptor.properties[i];
        printf("  - %s (%s): %s\n",
               prop->name,
               prop->type == PROPERTY_TYPE_FLOAT ? "float" : 
               prop->type == PROPERTY_TYPE_INT ? "int" : 
               prop->type == PROPERTY_TYPE_STRING ? "string" : "unknown",
               prop->description);
    }

    // 列出所有Sprite属性
    printf("\nSprite properties:\n");
    for (int i = 0; i < spriteDescriptor.propertyCount; i++)
    {
        PropertyInfo *prop = &spriteDescriptor.properties[i];
        printf("  - %s (%s): %s\n",
               prop->name,
               prop->type == PROPERTY_TYPE_FLOAT ? "float" : 
               prop->type == PROPERTY_TYPE_INT ? "int" : 
               prop->type == PROPERTY_TYPE_STRING ? "string" : "unknown",
               prop->description);
    }

    printf("\nThis reflection system enables AI to:\n");
    printf("  1. Discover available component types\n");
    printf("  2. Query component properties\n");
    printf("  3. Modify component values at runtime\n");
    printf("  4. Generate code based on type information\n");
    printf("  5. Validate changes before applying\n");

    printf("\nReflection Example ended\n");
    return 0;
}
