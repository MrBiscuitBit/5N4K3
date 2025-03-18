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
                    if(cxt->player_data->dir != DOWN)
                        cxt->player_data->dir = UP;
                    break;
                case SDL_SCANCODE_RIGHT:
                    if(cxt->player_data->dir != LEFT)
                        cxt->player_data->dir = RIGHT;
                    break;
                case SDL_SCANCODE_DOWN:
                    if(cxt->player_data->dir != UP)
                        cxt->player_data->dir = DOWN;
                    break;
                case SDL_SCANCODE_LEFT:
                    if(cxt->player_data->dir != RIGHT)
                        cxt->player_data->dir = LEFT;
                    break;
                case SDL_SCANCODE_SPACE:
                    if(cxt->game_over){
                        reset_snake(cxt);
                        cxt->game_over = 0;
                    }
                    break;
                default:
                    break;
            }
        }

    }

}