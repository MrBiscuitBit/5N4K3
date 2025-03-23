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

    SDL_FRect rect = {0, 0, 100, 100};
    SDL_SetRenderDrawColor(cxt->app.renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(cxt->app.renderer, &rect);

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

    main_menu->state_enter = enter_state_main_menu;
    main_menu->state_update = update_state_main_menu;
    main_menu->state_render = render_state_main_menu;
    main_menu->state_exit = exit_state_main_menu;

    main_menu->name = "Main_Menu_State";

    return main_menu;
}