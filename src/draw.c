#include<SDL2/SDL.h>
#include"util.h"


void drawCircle(SDL_Renderer *renderer,float x, float y, float r){
      for (float i = 0; i < 360; i += 2)
        SDL_RenderDrawPoint(renderer,
                            r * SDL_cos(i / 180 * PI) + x,
                           r * SDL_sin(i / 180 * PI) + y);
                          
}

void drawAttackLine(SDL_Renderer *renderer,float x, float y, float r, float angle){
  for(float ri = 0;ri<r; ri+=2){
        SDL_RenderDrawPoint(renderer,
                            ri * SDL_cos(angle / 180 * PI) + x,
                           ri * SDL_sin(angle / 180 * PI) + y);
}
}
void fillCircle(SDL_Renderer *renderer, float x, float y, float r){
    for(float ri = 0;ri<r; ri+=2){
 for (float i = 0; i < 360; i += 2)
        SDL_RenderDrawPoint(renderer,
                            ri * SDL_cos(i / 180 * PI) + x,
                           ri * SDL_sin(i / 180 * PI) + y);
    }
}
void fillCircleByAngle(SDL_Renderer *renderer, float x, float y, float r, float angle){
    for(float ri = 0;ri<r; ri+=2){
 for (float i = 0; i < angle; i += 2)
        SDL_RenderDrawPoint(renderer,
                            ri * SDL_cos(i / 180 * PI) + x,
                           ri * SDL_sin(i / 180 * PI) + y);
    }
}