#ifndef COMPONENT_TYPE_H
#define COMPONENT_TYPE_H

#include <SDL2/SDL.h>

// 组件类型枚举
typedef enum {
    COMPONENT_TYPE_TRANSFORM = 0,
    COMPONENT_TYPE_SPRITE = 1,
    COMPONENT_TYPE_TEXT = 2,
    COMPONENT_TYPE_PHYSICS = 3,
    COMPONENT_TYPE_AUDIO = 4,
    COMPONENT_TYPE_SCRIPT = 5,
    COMPONENT_TYPE_CUSTOM = 99
} ComponentType;

// 属性类型
typedef enum {
    PROPERTY_TYPE_INT,
    PROPERTY_TYPE_FLOAT,
    PROPERTY_TYPE_BOOL,
    PROPERTY_TYPE_STRING,
    PROPERTY_TYPE_VECTOR2,
    PROPERTY_TYPE_COLOR
} PropertyType;

// 属性描述
typedef struct PropertyInfo {
    const char *name;
    PropertyType type;
    size_t offset;
    const char *description;
} PropertyInfo;

// 组件类型描述
typedef struct ComponentTypeDescriptor {
    const char *name;
    ComponentType type;
    size_t size;
    PropertyInfo *properties;
    int propertyCount;
    void (*create)(void *component);
    void (*destroy)(void *component);
    void (*update)(void *component, float deltaTime);
} ComponentTypeDescriptor;

// 获取组件类型描述
ComponentTypeDescriptor *ComponentType_GetDescriptor(ComponentType type);

// 注册组件类型
void ComponentType_Register(ComponentTypeDescriptor *descriptor);

// 查询组件属性
PropertyInfo *ComponentType_FindProperty(ComponentTypeDescriptor *descriptor, const char *name);

// 设置属性值
int ComponentType_SetProperty(void *component, PropertyInfo *property, void *value);

// 获取属性值
int ComponentType_GetProperty(void *component, PropertyInfo *property, void *value);

// 获取所有组件类型
int ComponentType_GetAllTypes(ComponentTypeDescriptor **descriptors, int maxCount);

#endif // COMPONENT_TYPE_H
