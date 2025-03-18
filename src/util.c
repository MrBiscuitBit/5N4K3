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

void cleanup_snake(GameContext *cxt){

    if(!cxt) return;

    safe_free((void**)&cxt->player_data->snake_head);
    
    cxt->player_data->snake_tail = NULL;
}

void cleanup_player_data(GameContext *cxt){

    if(!cxt) return;

    cleanup_snake(cxt);

    safe_free((void **)&cxt->player_data);

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

void cleanup_game_context(GameContext *cxt){

    if(!cxt) return;

    cleanup_player_data(cxt);

    cleanup_sdl(cxt);

    safe_free((void **)&cxt);

}

void reset_snake(GameContext *cxt){

    if(!cxt) return;

    cleanup_snake(cxt);

    cxt->player_data->snake_head = malloc(sizeof(Snake));
    if(!cxt->player_data->snake_head){
        SDL_Log("ERROR::Failed To Allocate Snake Head In Function Reset Snake\n");
        return;
    }
    memset(cxt->player_data->snake_head, 0, sizeof(Snake));
    
    cxt->player_data->dir = UP;
    cxt->player_data->snake_head->pos = (vec2){(BOARD_WIDTH - 2), (BOARD_HEIGHT - 2)};
    cxt->player_data->snake_head->next = NULL;

    cxt->player_data->snake_tail = cxt->player_data->snake_head;

}