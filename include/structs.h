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

typedef struct PlayerData{
    int dir;
    vec2 prev_pos;
    int move_delay;
    Uint64 last_move_time;
    Snake *snake_head, *snake_tail;
}PlayerData;

typedef struct App{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    Uint64 delta_time;
}App;

typedef struct GameContext{
    App app;
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    PlayerData *player_data;
    int game_over;
}GameContext;

#endif