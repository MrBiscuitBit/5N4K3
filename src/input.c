#include "input.h"

void handle_events(GameContext *cxt){

    if(!cxt) return;

    SDL_Event event;
    while(SDL_PollEvent(&event)){

        if(event.type == SDL_EVENT_QUIT){
            cxt->app.running = 0;
            return;
        }
        if(cxt->game_state_manager->state_tail && cxt->game_state_manager->state_tail->state &&
           cxt->game_state_manager->state_tail->state->state_handle_events)
        {
            cxt->game_state_manager->state_tail->state->state_handle_events(cxt, &event);
        }

    }

}
