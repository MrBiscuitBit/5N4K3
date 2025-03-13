#include "init.h"

static int init_sdl(GameContext *cxt){

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Init Failed: %s\n", SDL_GetError());
        SDL_ClearError();
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer("SNAKE", WND_WIDTH, WND_HEIGHT, 0, &cxt->app.window, &cxt->app.renderer)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Create Window And Renderer Failed: %s\n", SDL_GetError());
        SDL_ClearError();
        SDL_Quit();
        return 2;
    }

    return 0;
}

GameContext *init_game_context(){

    GameContext *new_game_cxt = malloc(sizeof(GameContext));
    if(!new_game_cxt){
        SDL_Log("ERROR::Init Game Context Failed\n");
        return NULL;
    }
    memset(new_game_cxt, 0, sizeof(GameContext));

    if(init_sdl(new_game_cxt)){
        SDL_Log("ERROR::Init SDL Failed\n");
        cleanup_game_context(new_game_cxt);
        return NULL;
    }

    return new_game_cxt;
}