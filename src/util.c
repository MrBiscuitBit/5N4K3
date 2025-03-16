#include "util.h"

void safe_free(void **ptr){
    if(ptr && *ptr){
        free(*ptr);
        *ptr = NULL;
    }
}

void cleanup_snake(GameContext *cxt){

    if(!cxt) return;

    safe_free((void**)&cxt->snake_head);
    
}

void cleanup_sdl(GameContext *cxt){

    if(!cxt) return;

    if(cxt->app.renderer){
        SDL_DestroyRenderer(cxt->app.renderer);
        cxt->app.renderer = NULL;
    }

    if(cxt->app.window){
        SDL_DestroyWindow(cxt->app.window);
        cxt->app.window = NULL;
    }

    SDL_Quit();
}

void cleanup_game_context(GameContext *cxt){

    if(!cxt) return;

    cleanup_snake(cxt);

    cleanup_sdl(cxt);

    safe_free((void **)&cxt);

}