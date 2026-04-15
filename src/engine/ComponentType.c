#include "ComponentType.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_COMPONENT_TYPES 32

static ComponentTypeDescriptor *typeRegistry[MAX_COMPONENT_TYPES] = {NULL};
static int typeCount = 0;

ComponentTypeDescriptor *ComponentType_GetDescriptor(ComponentType type)
{
    for (int i = 0; i < typeCount; i++)
    {
        if (typeRegistry[i] && typeRegistry[i]->type == type)
        {
            return typeRegistry[i];
        }
    }
    return NULL;
}

void ComponentType_Register(ComponentTypeDescriptor *descriptor)
{
    if (!descriptor || typeCount >= MAX_COMPONENT_TYPES)
    {
        return;
    }
    
    // 检查是否已注册
    for (int i = 0; i < typeCount; i++)
    {
        if (typeRegistry[i] && typeRegistry[i]->type == descriptor->type)
        {
            return;
        }
    }
    
    typeRegistry[typeCount++] = descriptor;
    printf("Registered component type: %s\n", descriptor->name);
}

PropertyInfo *ComponentType_FindProperty(ComponentTypeDescriptor *descriptor, const char *name)
{
    if (!descriptor || !name)
    {
        return NULL;
    }
    
    for (int i = 0; i < descriptor->propertyCount; i++)
    {
        if (strcmp(descriptor->properties[i].name, name) == 0)
        {
            return &descriptor->properties[i];
        }
    }
    
    return NULL;
}

int ComponentType_SetProperty(void *component, PropertyInfo *property, void *value)
{
    if (!component || !property || !value)
    {
        return 0;
    }
    
    void *target = (char *)component + property->offset;
    
    switch (property->type)
    {
        case PROPERTY_TYPE_INT:
            *(int *)target = *(int *)value;
            break;
        case PROPERTY_TYPE_FLOAT:
            *(float *)target = *(float *)value;
            break;
        case PROPERTY_TYPE_BOOL:
            *(int *)target = *(int *)value;
            break;
        case PROPERTY_TYPE_STRING:
            strncpy((char *)target, (char *)value, 255);
            break;
        case PROPERTY_TYPE_VECTOR2:
            memcpy(target, value, sizeof(float) * 2);
            break;
        case PROPERTY_TYPE_COLOR:
            memcpy(target, value, sizeof(SDL_Color));
            break;
        default:
            return 0;
    }
    
    return 1;
}

int ComponentType_GetProperty(void *component, PropertyInfo *property, void *value)
{
    if (!component || !property || !value)
    {
        return 0;
    }
    
    void *source = (char *)component + property->offset;
    
    switch (property->type)
    {
        case PROPERTY_TYPE_INT:
            *(int *)value = *(int *)source;
            break;
        case PROPERTY_TYPE_FLOAT:
            *(float *)value = *(float *)source;
            break;
        case PROPERTY_TYPE_BOOL:
            *(int *)value = *(int *)source;
            break;
        case PROPERTY_TYPE_STRING:
            strncpy((char *)value, (char *)source, 255);
            break;
        case PROPERTY_TYPE_VECTOR2:
            memcpy(value, source, sizeof(float) * 2);
            break;
        case PROPERTY_TYPE_COLOR:
            memcpy(value, source, sizeof(SDL_Color));
            break;
        default:
            return 0;
    }
    
    return 1;
}

int ComponentType_GetAllTypes(ComponentTypeDescriptor **descriptors, int maxCount)
{
    if (!descriptors || maxCount <= 0)
    {
        return 0;
    }
    
    int count = typeCount < maxCount ? typeCount : maxCount;
    memcpy(descriptors, typeRegistry, sizeof(ComponentTypeDescriptor *) * count);
    return count;
}
