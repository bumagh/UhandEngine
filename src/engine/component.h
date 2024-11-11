#ifndef component_h
#define component_h
#ifdef pc_build
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif
typedef struct Component
{
    const char *name;
    struct Component *next;
    void (*update)(struct Component *);                           // update函数指针，模拟行为
    void (*draw)(struct Component *self, SDL_Renderer *renderer); // 渲染方法
    void (*free)(struct Component *self);
} Component;
// 函数原型
void add_component(Component **head, const char *name, void (*update_func)(Component *));
void update_components(Component *head);
#endif