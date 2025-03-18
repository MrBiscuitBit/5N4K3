#include "update.h"

void clear_board(GameContext *cxt){

    if(!cxt) return;

    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            cxt->board[i][j] = 0;
        }
    }

}

void update_board(GameContext *cxt){
    
    if(!cxt) return;

    if(cxt->player_data->snake_head->pos.x < 0 || cxt->player_data->snake_head->pos.x >= BOARD_WIDTH ||
       cxt->player_data->snake_head->pos.y < 0 || cxt->player_data->snake_head->pos.y >= BOARD_HEIGHT)
    {
        SDL_Log("SNAKE POSITION OUT OF BOUNDS\n");   
        return;
    }

    cxt->board[(int)cxt->player_data->snake_head->pos.y][(int)cxt->player_data->snake_head->pos.x] = 1;

}

void update_snake(GameContext *cxt){

    if(!cxt) return;

    //Move Snake Head Forward
    cxt->player_data->prev_pos = cxt->player_data->snake_head->pos;
    switch(cxt->player_data->dir){
        case UP:
            if(cxt->player_data->snake_head->pos.y > 0)
                cxt->player_data->snake_head->pos.y--;
            else cxt->game_over = 1;
            break;
        case RIGHT:
            if(cxt->player_data->snake_head->pos.x < (BOARD_WIDTH - 1))
                cxt->player_data->snake_head->pos.x++;
            else cxt->game_over = 1;
            break;
        case DOWN:
            if(cxt->player_data->snake_head->pos.y < (BOARD_HEIGHT - 1))
                cxt->player_data->snake_head->pos.y++;
            else cxt->game_over = 1;
            break;
        case LEFT:
            if(cxt->player_data->snake_head->pos.x > 0)
                cxt->player_data->snake_head->pos.x--;
            else cxt->game_over = 1;
        default:
            break;
    }
    
}