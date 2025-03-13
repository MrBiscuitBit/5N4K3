#include "init.h"
#include "input.h"
#include "render.h"

int main(int argc, char *argv[]){
        
    GameContext *game = init_game_context();

    game->app.running = 1;
    while(game->app.running){

        prepare_scene(game);

        handle_events(game);

        present_scene(game);
        
    }

    cleanup_game_context(game);
    return 0;
}