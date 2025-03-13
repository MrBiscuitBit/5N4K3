#include "render.h"

void prepare_scene(GameContext *cxt){
    SDL_SetRenderDrawColor(cxt->app.renderer, 255, 255, 255, 255);
    SDL_RenderClear(cxt->app.renderer);
}

void present_scene(GameContext *cxt){
    SDL_RenderPresent(cxt->app.renderer);
}