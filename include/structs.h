#ifndef STRUCTS_H
#define STRUCTS_H

#include "common.h"

typedef struct App{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
}App;

typedef struct GameContext{
    App app;
}GameContext;

#endif