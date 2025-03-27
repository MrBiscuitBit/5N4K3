#include "input.h"

void handle_events(GameContext *cxt){

    if(!cxt) return;

    SDL_Event event;
    while(SDL_PollEvent(&event)){

        if(event.type == SDL_EVENT_QUIT){
            cxt->app.running = 0;
            return;
        }
        //Handle State Events
        if(cxt->game_state_manager->state_tail && cxt->game_state_manager->state_tail->state &&
           cxt->game_state_manager->state_tail->state->state_handle_events)
        {
            GameState *current_state = cxt->game_state_manager->state_tail->state;

            //Check Button Pressed On Mouse Down
            if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){

                if(current_state->button_pool != NULL){
                    for(int i = 0; i < current_state->button_count; i++){
                        Button *current_button = &current_state->button_pool[i];
                        if(current_button->button_flags.hovered)
                            current_button->button_flags.pressed = 1;
                    }
                }

            }
            if(event.type == SDL_EVENT_MOUSE_BUTTON_UP){
                GameState *current_state = cxt->game_state_manager->state_tail->state;
                if(current_state && current_state->button_pool){
        
                    for(int i = 0; i < current_state->button_count; i++){
                        Button *current_button = &current_state->button_pool[i];
                        if(current_button->button_flags.hovered && current_button->button_flags.pressed && current_button->on_click){
                            current_button->on_click(cxt);
                        }
                        current_button->button_flags.pressed = 0;
                    }
        
                }
            }

            //Call Handle State Events
            current_state->state_handle_events(cxt, &event);
        }

    }

}
