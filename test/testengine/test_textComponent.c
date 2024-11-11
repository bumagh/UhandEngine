#include "../../src/engine/UI/TextComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#undef main
int main(int argc, char *argv[])
{
    // 初始化 SDL 和 TTF
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("TextComponent Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("fzpix.ttf", 24);
    if (!font)
    {
        printf("TTF_OpenFont failed: %s\n", TTF_GetError());
        return;
    }
    // 初始化 TextComponent
    TextComponent textComponent;
    SDL_Color color = {255, 0, 0, 255};
    TextComponent_init(&textComponent, renderer, font, "Hello, SDL!", color);

    // 游戏循环
    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        SDL_RenderClear(renderer);
        textComponent.draw(&textComponent);
        SDL_RenderPresent(renderer);
    }

    // 释放资源
    textComponent.free(&textComponent);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
