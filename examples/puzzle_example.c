/**
 * Puzzle Game Example
 *
 * 一个简单的拼图游戏，展示鼠标交互、拖拽、逻辑
 */

#define SDL_MAIN_HANDLED
#include "../src/engine/InputManager.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 3
#define TILE_SIZE 100
#define BOARD_OFFSET_X 250
#define BOARD_OFFSET_Y 150

typedef struct {
    int x, y;
    int targetX, targetY;
    int number;
    SDL_Color color;
} Tile;

// 洗牌函数
void shuffleTiles(Tile *tiles, int *emptyIndex, int *moves, int *solved)
{
    srand(time(NULL));
    for (int i = 0; i < 100; i++)
    {
        int emptyRow = *emptyIndex / GRID_SIZE;
        int emptyCol = *emptyIndex % GRID_SIZE;
        
        // 随机选择一个相邻的块
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int validDirs[4];
        int validCount = 0;
        
        for (int d = 0; d < 4; d++)
        {
            int newRow = emptyRow + directions[d][0];
            int newCol = emptyCol + directions[d][1];
            if (newRow >= 0 && newRow < GRID_SIZE && newCol >= 0 && newCol < GRID_SIZE)
            {
                validDirs[validCount++] = d;
            }
        }
        
        if (validCount > 0)
        {
            int dir = validDirs[rand() % validCount];
            int newRow = emptyRow + directions[dir][0];
            int newCol = emptyCol + directions[dir][1];
            int targetIndex = newRow * GRID_SIZE + newCol;
            
            // 交换位置
            int tempX = tiles[*emptyIndex].x;
            int tempY = tiles[*emptyIndex].y;
            tiles[*emptyIndex].x = tiles[targetIndex].x;
            tiles[*emptyIndex].y = tiles[targetIndex].y;
            tiles[targetIndex].x = tempX;
            tiles[targetIndex].y = tempY;
            
            // 更新索引
            *emptyIndex = targetIndex;
        }
    }
    
    *moves = 0;
    *solved = 0;
    printf("Puzzle shuffled!\n");
}

// 检查是否解决
void checkSolved(Tile *tiles, int *solved, int moves)
{
    *solved = 1;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
    {
        if (tiles[i].number != i)
        {
            *solved = 0;
            break;
        }
    }
    
    if (*solved)
    {
        printf("Puzzle solved! Moves: %d\n", moves);
    }
}

int main(int argc, char *argv[])
{
    system("chcp 65001 > nul");

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Puzzle Game Example",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("=== Puzzle Game Example ===\n\n");
    printf("Controls:\n");
    printf("- Mouse Click: Select tile\n");
    printf("- R: Shuffle puzzle\n");
    printf("- Q: Quit\n");
    printf("\nObjective: Arrange tiles in order 1-8!\n\n");

    InputManager *input = InputManager_Create();

    // 初始化拼图
    Tile tiles[GRID_SIZE * GRID_SIZE];
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
    {
        tiles[i].number = i;
        tiles[i].targetX = BOARD_OFFSET_X + (i % GRID_SIZE) * TILE_SIZE;
        tiles[i].targetY = BOARD_OFFSET_Y + (i / GRID_SIZE) * TILE_SIZE;
        tiles[i].x = tiles[i].targetX;
        tiles[i].y = tiles[i].targetY;
        
        // 生成颜色
        int hue = (i * 360) / (GRID_SIZE * GRID_SIZE);
        tiles[i].color = (SDL_Color){
            (Uint8)(hue < 120 ? 255 : hue < 240 ? 255 - (hue - 120) * 2 : 0),
            (Uint8)(hue < 120 ? hue * 2 : hue < 240 ? 255 : 255 - (hue - 240) * 2),
            (Uint8)(hue < 120 ? 0 : hue < 240 ? 0 : (hue - 240) * 2),
            255
        };
    }

    // 空白块（最后一个）
    int emptyIndex = GRID_SIZE * GRID_SIZE - 1;
    tiles[emptyIndex].number = -1;
    tiles[emptyIndex].color = (SDL_Color){32, 32, 32, 255};

    int moves = 0;
    int solved = 0;

    // 主循环
    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_q)
                {
                    running = 0;
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    shuffleTiles(tiles, &emptyIndex, &moves, &solved);
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    InputManager_GetMousePosition(input, &mouseX, &mouseY);
                    
                    // 检查点击了哪个块
                    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
                    {
                        if (i == emptyIndex) continue;
                        
                        if (mouseX >= tiles[i].x && mouseX < tiles[i].x + TILE_SIZE &&
                            mouseY >= tiles[i].y && mouseY < tiles[i].y + TILE_SIZE)
                        {
                            // 检查是否与空白块相邻
                            int tileRow = tiles[i].y / TILE_SIZE - BOARD_OFFSET_Y / TILE_SIZE;
                            int tileCol = tiles[i].x / TILE_SIZE - BOARD_OFFSET_X / TILE_SIZE;
                            int emptyRow = tiles[emptyIndex].y / TILE_SIZE - BOARD_OFFSET_Y / TILE_SIZE;
                            int emptyCol = tiles[emptyIndex].x / TILE_SIZE - BOARD_OFFSET_X / TILE_SIZE;
                            
                            if ((abs(tileRow - emptyRow) == 1 && tileCol == emptyCol) ||
                                (abs(tileCol - emptyCol) == 1 && tileRow == emptyRow))
                            {
                                // 交换位置
                                int tempX = tiles[i].x;
                                int tempY = tiles[i].y;
                                tiles[i].x = tiles[emptyIndex].x;
                                tiles[i].y = tiles[emptyIndex].y;
                                tiles[emptyIndex].x = tempX;
                                tiles[emptyIndex].y = tempY;
                                
                                // 更新索引
                                emptyIndex = i;
                                moves++;
                                
                                checkSolved(tiles, &solved, moves);
                            }
                            break;
                        }
                    }
                }
            }

            InputManager_Update(input, &event);
        }

        InputManager_UpdateState(input);

        // 清屏
        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        // 绘制边框
        SDL_Rect border = {BOARD_OFFSET_X - 5, BOARD_OFFSET_Y - 5, 
                          GRID_SIZE * TILE_SIZE + 10, GRID_SIZE * TILE_SIZE + 10};
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderDrawRect(renderer, &border);

        // 绘制拼图块
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
        {
            if (tiles[i].number == -1) continue; // 空白块
            
            SDL_Rect tileRect = {tiles[i].x, tiles[i].y, TILE_SIZE, TILE_SIZE};
            SDL_SetRenderDrawColor(renderer, tiles[i].color.r, tiles[i].color.g, tiles[i].color.b, 255);
            SDL_RenderFillRect(renderer, &tileRect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &tileRect);
        }

        // 显示信息
        printf("\rMoves: %d | %s", moves, solved ? "SOLVED!" : "Playing");

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    printf("\n");

    InputManager_Destroy(input);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Puzzle Game Example ended\n");
    return 0;
}
