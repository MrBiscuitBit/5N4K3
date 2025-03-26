#include "game_playing.h"

static void enter_state_game_playing(GameContext *cxt){
    if(!cxt) return;    
    return;
}

static void handle_state_events_game_playing(GameContext *cxt, SDL_Event *event){

    if(!cxt) return;

    if(event->type == SDL_EVENT_KEY_DOWN){

        switch(event->key.scancode){
            case SDL_SCANCODE_ESCAPE:
                if(cxt->game_over){
                    commit_pending_action(cxt->game_state_manager, STATE_ACTION_SWITCH, cxt->game_state_manager->state_pool[STATE_MAIN_MENU]);
                    break;
                }
                cxt->game_over = 1;
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

static void update_state_game_playing(GameContext *cxt){

    if(!cxt) return;

    if(!cxt->game_over){
        if((SDL_GetTicks() - cxt->player_data->last_move_time) >= cxt->player_data->move_delay){
            update_snake(cxt);
            cxt->player_data->last_move_time = SDL_GetTicks();
        }
        clear_board(cxt);
        update_board(cxt);
        update_apple(cxt);
    }
    
}

static void render_state_game_playing(GameContext *cxt){

    if(!cxt) return;

    draw_board(cxt);

}

static void exit_state_game_playing(GameContext *cxt){
    if(!cxt) return;
    return;
}

GameState *init_state_game_playing(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return NULL;

    GameState *game_playing;
    game_playing = malloc(sizeof(GameState));
    if(!game_playing){
        SDL_Log("ERROR::Failed To Allocate Game Playing State\n");
        return NULL;
    }
    memset(game_playing, 0, sizeof(GameState));

    game_playing->state_enter = enter_state_game_playing;
    game_playing->state_handle_events = handle_state_events_game_playing;
    game_playing->state_update = update_state_game_playing;
    game_playing->state_render = render_state_game_playing;
    game_playing->state_exit = exit_state_game_playing;

    game_playing->button_count = 0;
    game_playing->button_pool = NULL;
    
    game_playing->name = "Game_Playing_State";

    return game_playing;
}