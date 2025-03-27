#include "game_state.h"

void push_state(GameContext*cxt, GameState *state){

    if(!cxt || !cxt->game_state_manager || !state) return;

    if(cxt->game_state_manager->state_counter >= MAX_STATES){
        SDL_Log("WARNING::State Overflow; Cannot Push State (%s)\n", state->name);
        return;
    }

    if(!cxt->game_state_manager->state_tail) return;
        
    GameStateNode *state_node = malloc(sizeof(GameStateNode));
    cxt->game_state_manager->state_tail->next = state_node;
    state_node->prev = cxt->game_state_manager->state_tail;
    cxt->game_state_manager->state_tail = state_node;
    cxt->game_state_manager->state_tail->next = NULL;

    cxt->game_state_manager->state_tail->state = state;

    if(state->state_enter)
        state->state_enter(cxt);

    cxt->game_state_manager->state_counter++;
    //SDL_Log("- State Pushed (%s) State Count: %d -\n", state->name, cxt->game_state_manager->state_counter);
}

void pop_state(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return;

    if(cxt->game_state_manager->state_tail == &cxt->game_state_manager->state_head){
        SDL_Log("WARNING::Stack Empty; Cannot Pop State\n");
        return;
    }

    //const char *state_name;
    GameStateNode *tmp;
    if(cxt->game_state_manager->state_tail && cxt->game_state_manager->state_tail->state)
    {
        //state_name = cxt->game_state_manager->state_tail->state->name;

        if(cxt->game_state_manager->state_tail->state->state_exit)
            cxt->game_state_manager->state_tail->state->state_exit(cxt);

        tmp = cxt->game_state_manager->state_tail;
        cxt->game_state_manager->state_tail = cxt->game_state_manager->state_tail->prev;
        cxt->game_state_manager->state_tail->next = NULL;
        safe_free((void **)&tmp);
    }
    
    cxt->game_state_manager->state_counter--;
    //SDL_Log("- State Popped (%s) State Count: %d", state_name, cxt->game_state_manager->state_counter);
}

void call_state_action(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return;

    switch(cxt->game_state_manager->pending_action){
        case STATE_ACTION_NONE:
            cxt->game_state_manager->pending_action = STATE_ACTION_NONE;
            break;
        case STATE_ACTION_PUSH:
            if(cxt->game_state_manager->pending_state != NULL){
               push_state(cxt, cxt->game_state_manager->pending_state);
            }
            cxt->game_state_manager->pending_action = STATE_ACTION_NONE;
            cxt->game_state_manager->pending_state = NULL;
            break;
        case STATE_ACTION_POP:
            pop_state(cxt);
            break;
        case STATE_ACTION_SWAP:
            if(cxt->game_state_manager->pending_state != NULL){
                pop_state(cxt);
                push_state(cxt, cxt->game_state_manager->pending_state);
            }
            cxt->game_state_manager->pending_action = STATE_ACTION_NONE;
            cxt->game_state_manager->pending_state = NULL;
            break;
        case STATE_ACTION_POP_SWAP:
            if(cxt->game_state_manager->pending_state != NULL){
                while(cxt->game_state_manager->state_counter > 0)
                    pop_state(cxt);
                push_state(cxt, cxt->game_state_manager->pending_state);
            }
            cxt->game_state_manager->pending_action = STATE_ACTION_NONE;
            cxt->game_state_manager->pending_state = NULL;
            break;
        default:
            cxt->game_state_manager->pending_action = STATE_ACTION_NONE;
            cxt->game_state_manager->pending_state = NULL;
            break;
    }

    cxt->game_state_manager->pending_action = STATE_ACTION_NONE;
    cxt->game_state_manager->pending_state = NULL;
}

void commit_pending_action(GameStateManager *manager, int state_action, GameState *state){

    if(!manager) return;

    switch(state_action){
        case STATE_ACTION_NONE:
            manager->pending_action = STATE_ACTION_NONE;
            break;
        case STATE_ACTION_PUSH:
            if(state != NULL){
                manager->pending_action = STATE_ACTION_PUSH;
                manager->pending_state = state;
            }
            else{
                SDL_Log("WARNING::STATE ACTION PUSH FAILED TO COMMIT; Pending State Not Given\n");
                manager->pending_action = STATE_ACTION_NONE;
            }
            break;
        case STATE_ACTION_POP:
            manager->pending_action = STATE_ACTION_POP;
            break;
        case STATE_ACTION_SWAP:
            if(state != NULL){
                manager->pending_action = STATE_ACTION_SWAP;
                manager->pending_state = state;
            }
            else{
                SDL_Log("WARNING::STATE ACTION SWITCH FAILED TO COMMIT; Pending State Not Given\n");
                manager->pending_action = STATE_ACTION_NONE;
            }
            break;
        case STATE_ACTION_POP_SWAP:
            if(state != NULL){
                manager->pending_action = STATE_ACTION_POP_SWAP;
                manager->pending_state = state;
            }
            else{
                SDL_Log("WARNING::STATE ACTION POP AND SWITCH FAILED TO COMMIT; Pending State Not Given\n");
                manager->pending_action = STATE_ACTION_NONE;
            }
            break;
        default:
            manager->pending_action = STATE_ACTION_NONE;
            manager->pending_state = NULL;
            break;
    }

}

void update_states(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return;

    call_state_action(cxt);

    check_states_current_button(cxt);

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
            render_state_buttons(cxt);
        }

        state_node = state_node->next;
    }

}
