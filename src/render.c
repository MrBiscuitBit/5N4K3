#include "render.h"

void prepare_scene(GameContext *cxt){
    if(!cxt) return;
    SDL_SetRenderDrawColor(cxt->app.renderer, 155, 155, 155, 155);
    SDL_RenderClear(cxt->app.renderer);
}

void present_scene(GameContext *cxt){
    if(!cxt) return;
    SDL_RenderPresent(cxt->app.renderer);
}

void draw_board(GameContext *cxt){

    if(!cxt) return;

    SDL_SetRenderDrawColor(cxt->app.renderer, 0, 0, 0, 255);

    SDL_FRect cell;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){

            switch(cxt->board[i][j]){
                case EMPTY:
                    continue;
                    break;
                case SNAKE:
                    SDL_SetRenderDrawColor(cxt->app.renderer, 0, 255, 0, 255);
                    break;
                case APPLE:
                    SDL_SetRenderDrawColor(cxt->app.renderer, 255, 0, 0, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(cxt->app.renderer, 0, 0, 0, 255);
                    break;
            }
            
            cell.w = cell.h = CELL_SIZE;
            cell.x = (j * CELL_SIZE) + BOARD_OFFSET_X;
            cell.y = (i * CELL_SIZE) + BOARD_OFFSET_Y;

            SDL_RenderFillRect(cxt->app.renderer, &cell);
        }
    }

    cell.w = BOARD_WIDTH * CELL_SIZE;
    cell.h = BOARD_HEIGHT * CELL_SIZE;
    cell.x = BOARD_OFFSET_X;
    cell.y = BOARD_OFFSET_Y;

    SDL_SetRenderDrawColor(cxt->app.renderer, 0, 0, 255, 255);
    SDL_RenderRect(cxt->app.renderer, &cell);
}