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
    if(!cxt) return;
    return;
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

    main_menu->button_count = 1;
    main_menu->button_pool = malloc(sizeof(Button) * main_menu->button_count);
    if(!main_menu->button_pool){
        SDL_Log("ERROR::Failed To Allocate Button Pool\n");
        return NULL;
    }
    memset(main_menu->button_pool, 0, sizeof(sizeof(Button) * main_menu->button_count));

    main_menu->button_pool[0] = init_button((vec2){(WND_WIDTH / 2), (WND_HEIGHT / 2)}, (vec2){300, 100}, "QUIT_GAME");

    main_menu->name = "Main_Menu_State";

    return main_menu;
}