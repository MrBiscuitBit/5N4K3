#include "render.h"

void prepare_scene(GameContext *cxt){
    if(!cxt) return;
    SDL_SetRenderDrawColor(cxt->app.renderer, 255, 255, 255, 155);
    SDL_RenderClear(cxt->app.renderer);
}

void present_scene(GameContext *cxt){
    if(!cxt) return;
    SDL_RenderPresent(cxt->app.renderer);
}