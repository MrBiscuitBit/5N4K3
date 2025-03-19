#include "init.h"
#include "input.h"
#include "render.h"
#include "update.h"

int main(int argc, char *argv[]){

    GameContext *game = init_game_context();

    Uint64 previous_time, current_time;
    previous_time = SDL_GetTicks();

    game->game_over = 0;
    game->app.running = 1;
    while(game->app.running){

        current_time = SDL_GetTicks();
        game->app.delta_time = current_time - previous_time;
        previous_time = current_time;

        prepare_scene(game);

        handle_events(game);

        if(!game->game_over){
            clear_board(game);
            if((SDL_GetTicks() - game->player_data->last_move_time) >= game->player_data->move_delay){
                update_snake(game);
                game->player_data->last_move_time = SDL_GetTicks();
                update_apple(game);
            }
            update_board(game);
        }

        draw_board(game);

        present_scene(game);
        
        cap_frame_rate(current_time);
    }

    cleanup_game_context(game);
    return 0;
}