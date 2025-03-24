#include "init.h"
#include "input.h"

int main(int argc, char *argv[]){

    GameContext *game = init_game_context();

    push_state(game, game->game_state_manager->state_pool[STATE_MAIN_MENU]);
    
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

        update_states(game);

        render_states(game);
        
        present_scene(game);
        
        cap_frame_rate(current_time);
    }

    cleanup_game_context(game);
    return 0;
}