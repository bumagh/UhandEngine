#ifndef draw_h
#define draw_h
#ifdef pc_build
#include <SDL2/SDL.h>

#else
#include "SDL.h"

#endif
void drawCircle(SDL_Renderer *renderer,float x, float y, float r);
void drawAttackLine(SDL_Renderer *renderer,float x, float y, float r, float angle);
void fillCircle(SDL_Renderer *renderer, float x, float y, float r);
void fillCircleByAngle(SDL_Renderer *renderer, float x, float y, float r, float angle);
#endif