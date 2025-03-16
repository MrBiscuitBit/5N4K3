#include "update.h"

void update_board(GameContext *cxt){
    
    if(!cxt) return;

    if(cxt->snake_head->pos.x < 0 || cxt->snake_head->pos.x >= BOARD_WIDTH ||
       cxt->snake_head->pos.y < 0 || cxt->snake_head->pos.y >= BOARD_HEIGHT)
    {
        SDL_Log("SNAKE POSITION OUT OF BOUNDS\n");   
        return;
    }

    cxt->board[(int)cxt->snake_head->pos.y][(int)cxt->snake_head->pos.x] = 1;

}