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

bool point_rect_collision(vec2 point, vec2 rect_pos, vec2 rect_size){

    bool collision_x = point.x < rect_pos.x + (rect_size.x / 2) && point.x > rect_pos.x - (rect_size.x / 2);
    bool collision_y = point.y < rect_pos.y + (rect_size.y / 2) && point.y > rect_pos.y - (rect_size.y / 2);

    return collision_x && collision_y;
}

vec4 vec4_lerp(vec4 vec_start, vec4 vec_target, float lerp_speed){

    vec4 new_vec4;
    new_vec4.r = vec_start.r + (vec_target.r - vec_start.r) * lerp_speed;
    new_vec4.g = vec_start.g + (vec_target.g - vec_start.g) * lerp_speed;
    new_vec4.b = vec_start.b + (vec_target.b - vec_start.b) * lerp_speed;
    new_vec4.a = vec_start.a + (vec_target.a - vec_start.a) * lerp_speed;

    return new_vec4;
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