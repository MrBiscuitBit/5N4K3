#include "game_state.h"

void push_state(GameContext*cxt, GameState *state){

    if(!cxt || !cxt->game_state_manager || !state) return;

    if(cxt->game_state_manager->state_counter >= MAX_STATES){
        SDL_Log("WARNING::State Overflow; Cannot Push State (%s)\n");
        return;
    }

    cxt->game_state_manager->state_tail->next = state;
    cxt->game_state_manager->state_tail->next->prev = cxt->game_state_manager->state_tail;
    cxt->game_state_manager->state_tail = state;
    cxt->game_state_manager->state_tail->next = NULL;

    if(state->state_enter){
        state->state_enter(cxt);
    }

    cxt->game_state_manager->state_counter++;
}

void pop_state(GameContext *cxt){

    if(!cxt || cxt->game_state_manager) return;

    if(cxt->game_state_manager->state_tail == &cxt->game_state_manager->state_head){
        SDL_Log("WARNING::Stack Empty; Cannot Pop State\n");
        return;
    }

    GameStateNode *tmp;
    if(cxt->game_state_manager->state_tail && cxt->game_state_manager->state_tail->state && cxt->game_state_manager->state_tail->state->state_exit)
    {
        cxt->game_state_manager->state_tail->state->state_exit(cxt);
        tmp = cxt->game_state_manager->state_tail;
        cxt->game_state_manager->state_tail = cxt->game_state_manager->state_tail->prev;
        cxt->game_state_manager->state_tail->next = NULL;
        safe_free((void **)&tmp);
    }
    
}

void update_states(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return;

    if(cxt->game_state_manager->state_tail->state && cxt->game_state_manager->state_tail->state->state_update){
        cxt->game_state_manager->state_tail->state->state_update(cxt);
    }

}

void render_states(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return;

    GameStateNode *state_node = cxt->game_state_manager->state_head.next;
    while(state_node != NULL){

        if(state_node->state && state_node->state->state_render){
            state_node->state->state_render(cxt);
        }

        state_node = state_node->next;
    }

}