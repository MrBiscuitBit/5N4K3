#ifndef STRUCTS_H
#define STRUCTS_H

#include "common.h"

typedef struct vec2{
    float x, y;
}vec2;

typedef struct Snake{
    vec2 pos;
    struct Snake *next, *prev;
}Snake;

typedef struct PlayerData{
    int dir;
    vec2 prev_head_pos;
    int move_delay;
    Uint64 last_move_time;
    int directional_input;
    Snake *snake_head, *snake_tail;
}PlayerData;

typedef struct App{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    Uint64 delta_time;
}App;

typedef struct GameState{
    void(*state_enter)(GameContext *cxt);
    void(*state_handle_events)(GameContext *cxt, SDL_Event *event);
    void(*state_update)(GameContext *cxt);
    void(*state_render)(GameContext *cxt);
    void(*state_exit)(GameContext *cxt);
    const char *name;
}GameState;

typedef struct GameStateNode{
    GameState *state;
    struct GameStateNode *prev, *next;
}GameStateNode;

typedef struct GameStateManager{
    GameState *state_pool[STATE_COUNT];
    GameStateNode state_head, *state_tail;
    int state_counter;
}GameStateManager;

typedef struct GameContext{
    App app;
    GameStateManager *game_state_manager;
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    PlayerData *player_data;
    int game_over;
    vec2 apple;
    int current_score;
}GameContext;

#endif