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

static int init_player_data(GameContext *cxt){

    if(!cxt) return 1;

    cxt->player_data = malloc(sizeof(PlayerData));
    if(!cxt->player_data){
        SDL_Log("ERROR::Failed To Allocate Player Data\n");
        cleanup_game_context(cxt);
        return 1;
    }
    memset(cxt->player_data, 0, sizeof(PlayerData));
    
    cxt->player_data->snake_head = malloc(sizeof(Snake));
    if(!cxt->player_data->snake_head){
        SDL_Log("ERROR::Failed To Allocate Snake Head\n");
        cleanup_game_context(cxt);
        return 2;
    }
    memset(cxt->player_data->snake_head, 0, sizeof(Snake));
    
    cxt->player_data->dir = UP;
    cxt->player_data->snake_head->pos = (vec2){(BOARD_WIDTH - 2), (BOARD_HEIGHT - 2)};
    cxt->player_data->snake_head->next = NULL;
    cxt->player_data->snake_head->prev = NULL;

    cxt->player_data->snake_tail = cxt->player_data->snake_head;

    //Timer For Movement
    cxt->player_data->move_delay = 85;

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
        SDL_Log("ERROR::Init Main Main Menu State Failed\n");
        return 3;
    }

    cxt->game_state_manager->state_pool[STATE_GAME_PLAYING] = init_state_game_playing(cxt);
    if(!cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]){
        SDL_Log("ERROR::Init Main Game Playing State Failed\n");
        return 4;
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

    if(init_player_data(new_game_cxt)){
        SDL_Log("ERROR::Init Player Data Failed\n");
        cleanup_game_context(new_game_cxt);
        return NULL;
    }

    new_game_cxt->apple = (vec2){1, 1};

    return new_game_cxt;
}