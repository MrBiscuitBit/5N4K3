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

    if(state->state_data && state->cleanup_data)
        state->cleanup_data(state);

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

    cleanup_state_manager(cxt->game_state_manager);

    cleanup_sdl(cxt);

    safe_free((void **)&cxt);

}
