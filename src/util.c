#include "util.h"

void safe_free(void **ptr){
    if(ptr && *ptr){
        free(*ptr);
        *ptr = NULL;
    }
}

void cap_frame_rate(Uint64 current_time){
    Uint64 elapsed_time = SDL_GetTicks() - current_time;
    if(elapsed_time < FRAME_TIME){
        SDL_Delay(FRAME_TIME - elapsed_time);
    }
}

void cleanup_snake(GameContext *cxt){

    if(!cxt || !cxt->player_data) return;

    Snake *temp, *snake = cxt->player_data->snake_tail;
    while(snake != NULL){
        temp = snake;
        snake = snake->prev;
        safe_free((void **)&temp);
    }
    
    cxt->player_data->snake_head = NULL;
    cxt->player_data->snake_tail = NULL;

}

void cleanup_player_data(GameContext *cxt){

    if(!cxt) return;

    cleanup_snake(cxt);

    safe_free((void **)&cxt->player_data);

}

void cleanup_sdl(GameContext *cxt){

    if(!cxt) return;

    if(cxt->app.renderer){
        SDL_DestroyRenderer(cxt->app.renderer);
        cxt->app.renderer = NULL;
    }

    if(cxt->app.window){
        SDL_DestroyWindow(cxt->app.window);
        cxt->app.window = NULL;
    }

    SDL_Quit();
}

void cleanup_state_stack(GameStateManager *manager){

    if(!manager) return;

    GameStateNode *state_node, *tmp;
    state_node = manager->state_head.next;
    while(state_node != NULL){
        tmp = state_node;
        state_node = state_node->next;
        safe_free((void **)&tmp);
    }

    manager->state_head.prev = NULL;
    manager->state_head.next = NULL;
    manager->state_head.state = NULL;

    manager->state_tail = &manager->state_head;
}

void cleanup_state(GameState *state){

    if(!state) return;
    
    state->name = NULL;
    state->state_enter = NULL;
    state->state_handle_events = NULL;
    state->state_update = NULL;
    state->state_render = NULL;
    state->state_exit = NULL;

    safe_free((void **)&state->button_pool);

    safe_free((void **)&state);
}

void cleanup_state_pool(GameStateManager *manager){

    if(!manager || manager->state_counter == 0) return;

    for(int i = 0; i < STATE_COUNT; i++){
        cleanup_state(manager->state_pool[i]);
        manager->state_counter--;
    }

}

void cleanup_state_manager(GameStateManager *manager){

    if(!manager) return;

    cleanup_state_stack(manager);
    cleanup_state_pool(manager);

    safe_free((void **)&manager);
}

void cleanup_game_context(GameContext *cxt){

    if(!cxt) return;

    cleanup_player_data(cxt);

    cleanup_state_manager(cxt->game_state_manager);

    cleanup_sdl(cxt);

    safe_free((void **)&cxt);

}

void reset_snake(GameContext *cxt){

    if(!cxt) return;

    cleanup_snake(cxt);

    cxt->player_data->snake_head = malloc(sizeof(Snake));
    if(!cxt->player_data->snake_head){
        SDL_Log("ERROR::Failed To Allocate Snake Head In Function Reset Snake\n");
        return;
    }
    memset(cxt->player_data->snake_head, 0, sizeof(Snake));

    cxt->player_data->dir = UP;
    cxt->player_data->directional_input = -1;
    cxt->player_data->snake_head->pos = (vec2){(BOARD_WIDTH - 2), (BOARD_HEIGHT - 2)};
    cxt->player_data->snake_head->next = NULL;
    cxt->player_data->snake_head->prev = NULL;

    cxt->player_data->snake_tail = cxt->player_data->snake_head;
    cxt->current_score = 0;
    SDL_Log("Current Score: %d\n", cxt->current_score);
}

void clear_board(GameContext *cxt){

    if(!cxt) return;

    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            cxt->board[i][j] = 0;
        }
    }

}