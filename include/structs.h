#ifndef STRUCTS_H
#define STRUCTS_H

#include "common.h"

typedef struct GameState GameState;
typedef struct GameStateNode GameStateNode;
typedef struct GameContext GameContext;

typedef struct vec2{
    float x, y;
}vec2;

typedef struct vec4{
    float r, g, b, a;
}vec4;

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

typedef struct ButtonFlags{
    bool hovered;
    bool pressed;
}ButtonFlags;

typedef struct Button{
    vec4 color, hover_color;
    vec2 pos, size;
    const char *label;
    ButtonFlags button_flags;
    void(*on_click)(GameContext *cxt);
}Button;

typedef struct GameState{
    Button *button_pool;
    int button_count;
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
    int pending_action;
    GameState *pending_state;
    int state_counter;
}GameStateManager;

typedef struct App{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    Uint64 delta_time;
    float mouse_x, mouse_y;
}App;

typedef struct GameContext{
    App app;
    GameStateManager *game_state_manager;
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    PlayerData *player_data;
    int game_stop;
    vec2 apple;
    int current_score;
}GameContext;

#endif