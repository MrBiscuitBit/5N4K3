#include "input.h"

void handle_events(GameContext *cxt){

    if(!cxt) return;

    SDL_Event event;
    while(SDL_PollEvent(&event)){

        if(event.type == SDL_EVENT_QUIT){
            cxt->app.running = 0;
            return;
        }
        else if(event.type == SDL_EVENT_KEY_DOWN){
            switch (event.key.scancode){
                case SDL_SCANCODE_ESCAPE:
                    cxt->app.running = 0;
                    break;
                default:
                    break;
            }
        }

    }

}