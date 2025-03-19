#include "update.h"

void update_board(GameContext *cxt){
    
    if(!cxt) return;

    if(cxt->player_data->snake_head->pos.x < 0 || cxt->player_data->snake_head->pos.x >= BOARD_WIDTH ||
       cxt->player_data->snake_head->pos.y < 0 || cxt->player_data->snake_head->pos.y >= BOARD_HEIGHT)
    {
        SDL_Log("SNAKE POSITION OUT OF BOUNDS\n");   
        return;
    }

    cxt->board[(int)cxt->player_data->snake_head->pos.y][(int)cxt->player_data->snake_head->pos.x] = SNAKE;
    cxt->board[(int)cxt->apple.y][(int)cxt->apple.x] = APPLE;

}

void spawn_apple(GameContext *cxt){

    if(!cxt) return;

    cxt->apple.x = rand() % BOARD_WIDTH;
    cxt->apple.y = rand() % BOARD_HEIGHT;

    bool apple_position_accepted = 0;
    while(!apple_position_accepted){

        if(cxt->board[(int)cxt->apple.y][(int)cxt->apple.y] == EMPTY){
            apple_position_accepted = 1;
        }
        else{
            cxt->apple.x = rand() % BOARD_WIDTH;
            cxt->apple.y = rand() % BOARD_HEIGHT;
        }
    }

}

void update_apple(GameContext *cxt){

    if(!cxt) return;

    if(cxt->player_data->snake_head->pos.x == cxt->apple.x && cxt->player_data->snake_head->pos.y == cxt->apple.y){
        spawn_apple(cxt);
    }

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
