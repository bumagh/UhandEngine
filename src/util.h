#ifndef util_h
#define util_h
#ifdef pc_build
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif
#define PI 3.14159
#define LENGTH(x, y) SDL_sqrt((x) * (x) + (y) * (y))
#define WINDOW_W 960
#define WINDOW_H 572
#define MARGIN 50
// #define offsetof(type, member) ((size_t)&((type *)0)->member)

// #define container_of(ptr, type, member) ({          \
//     const typeof(((type *)0)->member) *__mptr = (ptr);    \
//     (type *)((char *)__mptr - offsetof(type, member)); })

float length(float x, float y);
int pointInRect(float x, float y, SDL_Rect *rect);
int pointInCircle(float x, float y, float x2, float y2, float r2);
float distance(float x1, float y1, float x2, float y2);
int circlesIntersect(float x1, float y1, float r1, float x2, float y2, float r2);
#endif