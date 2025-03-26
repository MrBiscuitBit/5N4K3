#include "main_menu.h"

static void enter_state_main_menu(GameContext *cxt){
    if(!cxt) return;
    return;
}

static void handle_state_events_main_menu(GameContext *cxt, SDL_Event *event){

    if(!cxt) return;

    if(event->type == SDL_EVENT_KEY_DOWN){

        switch(event->key.scancode){
            case SDL_SCANCODE_ESCAPE:
                cxt->app.running = 0;
                break;
            case SDL_SCANCODE_SPACE:
                commit_pending_action(cxt->game_state_manager, STATE_ACTION_SWITCH, cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]);
                break;
            default:
                break;
        }
        
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
        GameState *current_state = cxt->game_state_manager->state_tail->state;
        if(current_state && current_state->button_pool){

            for(int i = 0; i < current_state->button_count; i++){
                Button *current_button = &current_state->button_pool[i];
                if(current_button->button_flags.hovered && current_button->button_flags.pressed && current_button->on_click){
                    current_button->on_click(cxt);
                }
                current_button->button_flags.pressed = 0;
            }

        }
    }

}

static void update_state_main_menu(GameContext *cxt){
    if(!cxt) return;
    return;
}

static void render_state_main_menu(GameContext *cxt){
    if(!cxt) return;
    return;
}

static void exit_state_main_menu(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_tail) return;

    GameState *current_state = cxt->game_state_manager->state_tail->state;
    for(int i = 0; i < current_state->button_count; i++){
        current_state->button_pool[i].button_flags.hovered = 0;
        current_state->button_pool[i].button_flags.pressed = 0;
    }
    
}

static void on_click_button_play(GameContext *cxt){
    if(!cxt || !cxt->game_state_manager) return;
    commit_pending_action(cxt->game_state_manager, STATE_ACTION_SWITCH, cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]);
}

static void on_click_button_quit(GameContext *cxt){
    if(!cxt) return;
    cxt->app.running = 0;
}

GameState *init_state_main_menu(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return NULL;

    GameState *main_menu;
    main_menu = malloc(sizeof(GameState));
    if(!main_menu){
        SDL_Log("ERROR::Failed To Allocate MAIN MENU STATE\n");
        return NULL;
    }
    memset(main_menu, 0, sizeof(GameState));

    main_menu->state_enter = enter_state_main_menu;
    main_menu->state_handle_events = handle_state_events_main_menu;
    main_menu->state_update = update_state_main_menu;
    main_menu->state_render = render_state_main_menu;
    main_menu->state_exit = exit_state_main_menu;

    main_menu->button_count = 2;
    main_menu->button_pool = malloc(sizeof(Button) * main_menu->button_count);
    if(!main_menu->button_pool){
        SDL_Log("ERROR::Failed To Allocate Button Pool\n");
        return NULL;
    }
    memset(main_menu->button_pool, 0, sizeof(sizeof(Button) * main_menu->button_count));

    main_menu->button_pool[0] = init_button((vec2){(WND_WIDTH / 2), (WND_HEIGHT / 2)}, (vec2){300, 100}, "PLAY_GAME");
    main_menu->button_pool[0].on_click = on_click_button_play;

    main_menu->button_pool[1] = init_button((vec2){(WND_WIDTH / 2), (WND_HEIGHT / 2) + 150}, (vec2){300, 100}, "QUIT_BUTTON");
    main_menu->button_pool[1].on_click = on_click_button_quit;

    main_menu->name = "Main_Menu_State";

    return main_menu;
}