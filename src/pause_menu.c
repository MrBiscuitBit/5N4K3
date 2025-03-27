#include "pause_menu.h"

static void enter_state_pause_menu(GameContext *cxt){
    return;
}

static void handle_state_events_pause_menu(GameContext *cxt, SDL_Event *event){

    if(!cxt || !event) return;

    if(event->type == SDL_EVENT_KEY_DOWN){
        switch(event->key.scancode){
            case SDL_SCANCODE_ESCAPE:
                commit_pending_action(cxt->game_state_manager, STATE_ACTION_POP, NULL);
                break;
            default:
                break;
        }
    }

}

static void update_state_pause_menu(GameContext *cxt){
    return;
}

static void render_state_pause_menu(GameContext *cxt){

    SDL_FRect rect;
    rect.w = (WND_WIDTH * 3) / 4;
    rect.h = (WND_HEIGHT * 3) / 4;
    rect.x = (WND_WIDTH / 2) - (rect.w / 2);
    rect.y = (WND_HEIGHT / 2) - (rect.h / 2);

    SDL_SetRenderDrawColor(cxt->app.renderer, 255, 51, 255, 255);
    SDL_RenderFillRect(cxt->app.renderer, &rect);

}

static void exit_state_pause_menu(GameContext *cxt){
    GameState *current_state = cxt->game_state_manager->state_pool[STATE_GAME_PLAYING];
    for(int i = 0; i < current_state->button_count; i++){
        current_state->button_pool[i].button_flags.hovered = 0;
        current_state->button_pool[i].button_flags.pressed = 0;
    }
}

static void on_click_button_resume(GameContext *cxt){
    if(!cxt || !cxt->game_state_manager) return;
    commit_pending_action(cxt->game_state_manager, STATE_ACTION_POP, NULL);
}

static void on_click_button_exit(GameContext *cxt){
    if(!cxt || !cxt->game_state_manager) return;
    commit_pending_action(cxt->game_state_manager, STATE_ACTION_POP_SWAP, cxt->game_state_manager->state_pool[STATE_MAIN_MENU]);
}

GameState *init_state_pause_menu(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return NULL;

    GameState *pause_menu = malloc(sizeof(GameState));
    if(!pause_menu){
        SDL_Log("ERROR::Failed To Allocate State Pause Menu\n");
        return NULL;
    }
    memset(pause_menu, 0, sizeof(GameState));

    pause_menu->state_enter = enter_state_pause_menu;
    pause_menu->state_handle_events = handle_state_events_pause_menu;
    pause_menu->state_update = update_state_pause_menu;
    pause_menu->state_render = render_state_pause_menu;
    pause_menu->state_exit = exit_state_pause_menu;
    pause_menu->cleanup_data = NULL;

    pause_menu->state_data = NULL;
    
    pause_menu->button_count = 2;
    pause_menu->button_pool = (pause_menu->button_count <= 0)? NULL: malloc(sizeof(Button) * pause_menu->button_count);
    memset(pause_menu->button_pool, 0, (sizeof(Button) * pause_menu->button_count));

    pause_menu->button_pool[0] = init_button((vec2){WND_WIDTH / 2, WND_HEIGHT / 2}, (vec2){300, 100}, "RESUME");
    pause_menu->button_pool[0].on_click = on_click_button_resume;

    pause_menu->button_pool[1] = init_button((vec2){WND_WIDTH / 2, (WND_HEIGHT / 2) + 150}, (vec2){300, 100}, "EXIT");
    pause_menu->button_pool[1].on_click = on_click_button_exit; 

    pause_menu->name = "PAUSE_MENU";

    return pause_menu;
}