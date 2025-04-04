#include "init.h"

static int init_sdl(GameContext *cxt){

    if(!cxt) return 1;

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Init Failed: %s\n", SDL_GetError());
        SDL_ClearError();
        return 2;
    }

    if(!SDL_CreateWindowAndRenderer("SNAKE", WND_WIDTH, WND_HEIGHT, 0, &cxt->app.window, &cxt->app.renderer)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Create Window And Renderer Failed: %s\n", SDL_GetError());
        SDL_ClearError();
        SDL_Quit();
        return 3;
    }

    return 0;
}

int init_game_state_manager(GameContext *cxt){

    if(!cxt) return 1;

    cxt->game_state_manager = malloc(sizeof(GameStateManager));
    if(!cxt->game_state_manager){
        SDL_Log("ERROR::Failed To Allocate Game State Manager\n");
        return 2;
    }

    cxt->game_state_manager->state_pool[STATE_MAIN_MENU] = init_state_main_menu(cxt);
    if(!cxt->game_state_manager->state_pool[STATE_MAIN_MENU]){
        SDL_Log("ERROR::Init Main Menu State Failed\n");
        return 3;
    }

    cxt->game_state_manager->state_pool[STATE_GAME_PLAYING] = init_state_game_playing(cxt);
    if(!cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]){
        SDL_Log("ERROR::Init Game Playing State Failed\n");
        return 4;
    }

    cxt->game_state_manager->state_pool[STATE_PAUSE_MENU] = init_state_pause_menu(cxt);
    if(!cxt->game_state_manager->state_pool[STATE_PAUSE_MENU]){
        SDL_Log("ERROR::Init Pause Menu State Failed\n");
        return 5;
    }

    cxt->game_state_manager->state_head.state = NULL;
    cxt->game_state_manager->state_head.next  = NULL;

    cxt->game_state_manager->state_tail = &cxt->game_state_manager->state_head;

    return 0;
}

GameContext *init_game_context(){

    GameContext *new_game_cxt = malloc(sizeof(GameContext));
    if(!new_game_cxt){
        SDL_Log("ERROR::Init Game Context Failed\n");
        return NULL;
    }
    memset(new_game_cxt, 0, sizeof(GameContext));

    if(init_sdl(new_game_cxt)){
        SDL_Log("ERROR::Init SDL Failed\n");
        cleanup_game_context(new_game_cxt);
        return NULL;
    }

    if(init_game_state_manager(new_game_cxt)){
        SDL_Log("ERROR::Init Game State Manager Failed\n");
        cleanup_game_context(new_game_cxt);
        return NULL;
    }

    return new_game_cxt;
}