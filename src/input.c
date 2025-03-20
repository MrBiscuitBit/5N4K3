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
                case SDL_SCANCODE_UP:
                    cxt->player_data->directional_input = UP;
                    break;
                case SDL_SCANCODE_RIGHT:
                    cxt->player_data->directional_input = RIGHT;
                    break;
                case SDL_SCANCODE_DOWN:
                    cxt->player_data->directional_input = DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    cxt->player_data->directional_input = LEFT;
                    break;
                case SDL_SCANCODE_SPACE:
                    if(cxt->game_over){
                        clear_board(cxt);
                        reset_snake(cxt);
                        cxt->apple = (vec2){1, 1};
                        cxt->game_over = 0;
                    }
                    break;
                default:
                    break;
            }
        }

    }

}
