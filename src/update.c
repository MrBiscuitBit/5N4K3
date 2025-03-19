#include "update.h"

void update_board(GameContext *cxt){
    
    if(!cxt) return;

    Snake *snake = cxt->player_data->snake_head;
    while(snake != NULL){
        if(snake->pos.x >= 0 && snake->pos.x < BOARD_WIDTH && snake->pos.y >= 0 && snake->pos.y < BOARD_HEIGHT){
            cxt->board[(int)snake->pos.y][(int)snake->pos.x] = SNAKE;
        }
        snake = snake->next;
    }
    cxt->board[(int)cxt->apple.y][(int)cxt->apple.x] = APPLE;

}

void spawn_apple(GameContext *cxt){

    if(!cxt) return;

    cxt->apple.x = rand() % BOARD_WIDTH;
    cxt->apple.y = rand() % BOARD_HEIGHT;

    bool apple_position_accepted = 0;
    while(!apple_position_accepted){

        if(cxt->board[(int)cxt->apple.y][(int)cxt->apple.x] == EMPTY){
            cxt->board[(int)cxt->apple.y][(int)cxt->apple.x] = APPLE;
            apple_position_accepted = 1;
        }
        else{
            cxt->apple.x = rand() % BOARD_WIDTH;
            cxt->apple.y = rand() % BOARD_HEIGHT;
        }
    }

}

void add_snake_node(GameContext *cxt){

    Snake *new_snake = malloc(sizeof(Snake));
    if(!new_snake){
        SDL_Log("ERROR::Failed To Allocate Snake Node\n");
        return;
    }
    memset(new_snake, 0, sizeof(Snake));
    new_snake->prev = cxt->player_data->snake_tail;
    cxt->player_data->snake_tail->next = new_snake;
    cxt->player_data->snake_tail = new_snake;

    new_snake->pos = new_snake->prev->pos;
    new_snake->next = NULL;

}

void update_apple(GameContext *cxt){

    if(!cxt) return;

    if(cxt->player_data->snake_head->pos.x == cxt->apple.x && cxt->player_data->snake_head->pos.y == cxt->apple.y){
        spawn_apple(cxt);
        add_snake_node(cxt);
    }

}

void update_snake_direction(GameContext *cxt){

    if(!cxt) return;

    if(cxt->player_data->directional_input == -1) return;

    switch(cxt->player_data->directional_input){
        case UP:
            if(cxt->player_data->dir != DOWN)
                cxt->player_data->dir = UP;
            break;
        case RIGHT:
            if(cxt->player_data->dir != LEFT)
                cxt->player_data->dir = RIGHT;
            break;
        case DOWN:
            if(cxt->player_data->dir != UP)
                cxt->player_data->dir = DOWN;
            break;
        case LEFT:
            if(cxt->player_data->dir != RIGHT)
                cxt->player_data->dir = LEFT;
            break;
        default:
            break;
    }

}

void update_snake(GameContext *cxt){

    if(!cxt) return;

    update_snake_direction(cxt);

    //Move Snake Head Forward
    cxt->player_data->prev_head_pos = cxt->player_data->snake_head->pos;
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

    //Move The Tail Up To The Head
    Snake *snake = cxt->player_data->snake_tail;
    while(snake->prev){
        snake->pos = snake->prev->pos;
        snake = snake->prev;
    }

}
