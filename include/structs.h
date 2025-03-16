#ifndef STRUCTS_H
#define STRUCTS_H

#include "common.h"

typedef struct vec2{
    float x, y;
}vec2;

typedef struct Snake{
    vec2 pos;
    struct Snake *next;
}Snake;

typedef struct App{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
}App;

typedef struct GameContext{
    App app;
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    Snake *snake_head;
}GameContext;

#endif