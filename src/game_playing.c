#include "game_playing.h"


static void update_board(GamePlayData *game_play_data);
static void update_snake(GamePlayData *game_play_data);
static void reset_snake(GameContext *cxt);
void clear_board(GamePlayData *game_play_data);
static void draw_board(GameContext *cxt);

static void enter_state_game_playing(GameContext *cxt){
    if(!cxt) return;
    reset_snake(cxt);
    GamePlayData *game_play_data = (GamePlayData *)cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data;
    if(game_play_data){
        game_play_data->game_stop = 1;
        clear_board(game_play_data);
        update_board(game_play_data);
    }
    draw_board(cxt);
    return;
}

static void handle_state_events_game_playing(GameContext *cxt, SDL_Event *event){

    if(!cxt) return;

    GamePlayData *data = (GamePlayData *)cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data;
    if(data && event->type == SDL_EVENT_KEY_DOWN){

        switch(event->key.scancode){
            case SDL_SCANCODE_ESCAPE:
                if(data->game_stop){
                    commit_pending_action(cxt->game_state_manager, STATE_ACTION_SWAP, cxt->game_state_manager->state_pool[STATE_MAIN_MENU]);
                    break;
                }
                data->game_stop = 1;
                break;
            case SDL_SCANCODE_UP:
                data->player_data->directional_input = UP;
                break;
            case SDL_SCANCODE_RIGHT:
                data->player_data->directional_input = RIGHT;
                break;
            case SDL_SCANCODE_DOWN:
                data->player_data->directional_input = DOWN;
                break;
            case SDL_SCANCODE_LEFT:
                data->player_data->directional_input = LEFT;
                break;
            case SDL_SCANCODE_SPACE:
                if(data->game_stop){
                    clear_board(data);
                    if(data->snake_dead){
                        reset_snake(cxt);
                        data->apple = (vec2){1, 1};
                    }
                    data->game_stop = 0;
                    data->snake_dead = 0;
                }
                break;
            default:
                break;
        }
        
    }

}

static void update_board(GamePlayData *game_play_data){
    
    if(!game_play_data) return;

    Snake *snake = game_play_data->player_data->snake_head;
    while(snake != NULL){
        if(snake->pos.x >= 0 && snake->pos.x < BOARD_WIDTH && snake->pos.y >= 0 && snake->pos.y < BOARD_HEIGHT){
            game_play_data->board[(int)snake->pos.y][(int)snake->pos.x] = SNAKE;
        }
        snake = snake->next;
    }
    if(game_play_data->apple.x != -1 && game_play_data->apple.y != -1)
        game_play_data->board[(int)game_play_data->apple.y][(int)game_play_data->apple.x] = APPLE;

}


static void spawn_apple(GamePlayData *game_play_data){

    if(!game_play_data) return;

    game_play_data->apple.x = rand() % BOARD_WIDTH;
    game_play_data->apple.y = rand() % BOARD_HEIGHT;

    bool apple_position_accepted = 0;
    while(!apple_position_accepted){

        if(game_play_data->board[(int)game_play_data->apple.y][(int)game_play_data->apple.x] == EMPTY){
            game_play_data->board[(int)game_play_data->apple.y][(int)game_play_data->apple.x] = APPLE;
            apple_position_accepted = 1;
        }
        else{
            game_play_data->apple.x = rand() % BOARD_WIDTH;
            game_play_data->apple.y = rand() % BOARD_HEIGHT;
        }
    }

}


static void add_snake_node(GamePlayData *game_play_data){

    if(!game_play_data) return;

    Snake *new_snake = malloc(sizeof(Snake));
    if(!new_snake){
        SDL_Log("ERROR::Failed To Allocate Snake Node\n");
        return;
    }
    memset(new_snake, 0, sizeof(Snake));

    new_snake->prev = game_play_data->player_data->snake_tail;
    game_play_data->player_data->snake_tail->next = new_snake;
    game_play_data->player_data->snake_tail = new_snake;

    new_snake->pos = new_snake->prev->pos;
    new_snake->next = NULL;

}

static void update_apple(GamePlayData *game_play_data){

    if(!game_play_data || (game_play_data->current_score == (BOARD_WIDTH * BOARD_HEIGHT) - 1)) return;

    if(game_play_data->player_data->snake_head->pos.x == game_play_data->apple.x && game_play_data->player_data->snake_head->pos.y == game_play_data->apple.y){
        spawn_apple(game_play_data);
        add_snake_node(game_play_data);
        SDL_Log("Current Score: %d\n", ++game_play_data->current_score);
    }

    if(game_play_data->current_score == (BOARD_WIDTH * BOARD_HEIGHT) - 1){
        game_play_data->apple = (vec2){-1, -1};
        update_snake(game_play_data);
        clear_board(game_play_data);
        update_board(game_play_data);
        game_play_data->game_stop = 1;
        SDL_Log("Player Wins!");
    }
}

static void update_snake_direction(GamePlayData *game_play_data){

    if(!game_play_data) return;

    if(game_play_data->player_data->directional_input == -1) return;

    switch(game_play_data->player_data->directional_input){
        case UP:
            if(game_play_data->player_data->dir != DOWN)
                game_play_data->player_data->dir = UP;
            break;
        case RIGHT:
            if(game_play_data->player_data->dir != LEFT)
                game_play_data->player_data->dir = RIGHT;
            break;
        case DOWN:
            if(game_play_data->player_data->dir != UP)
                game_play_data->player_data->dir = DOWN;
            break;
        case LEFT:
            if(game_play_data->player_data->dir != RIGHT)
                game_play_data->player_data->dir = LEFT;
            break;
        default:
            break;
    }

}

static void update_snake(GamePlayData *game_play_data){

    if(!game_play_data) return;

    update_snake_direction(game_play_data);

    //Move Snake Head Forward
    
    game_play_data->player_data->prev_head_pos = game_play_data->player_data->snake_head->pos;
    switch(game_play_data->player_data->dir){
        case UP:
            if(game_play_data->player_data->snake_head->pos.y > 0 &&
               game_play_data->board[(int)game_play_data->player_data->snake_head->pos.y - 1][(int)game_play_data->player_data->snake_head->pos.x] != SNAKE)
                game_play_data->player_data->snake_head->pos.y--;
            else{
                game_play_data->game_stop = 1;
                game_play_data->snake_dead = 1;
            }
            break;
        case RIGHT:
            if(game_play_data->player_data->snake_head->pos.x < (BOARD_WIDTH - 1) &&
               game_play_data->board[(int)game_play_data->player_data->snake_head->pos.y][(int)game_play_data->player_data->snake_head->pos.x + 1] != SNAKE)
                game_play_data->player_data->snake_head->pos.x++;
            else{
                game_play_data->game_stop = 1;
                game_play_data->snake_dead = 1;
            }
            break;
        case DOWN:
            if(game_play_data->player_data->snake_head->pos.y < (BOARD_HEIGHT - 1) &&
               game_play_data->board[(int)game_play_data->player_data->snake_head->pos.y + 1][(int)game_play_data->player_data->snake_head->pos.x] != SNAKE)
                game_play_data->player_data->snake_head->pos.y++;
            else{
                game_play_data->game_stop = 1;
                game_play_data->snake_dead = 1;
            }
            break;
        case LEFT:
            if(game_play_data->player_data->snake_head->pos.x > 0 &&
               game_play_data->board[(int)game_play_data->player_data->snake_head->pos.y][(int)game_play_data->player_data->snake_head->pos.x - 1] != SNAKE)
                game_play_data->player_data->snake_head->pos.x--;
            else{
                game_play_data->game_stop = 1;
                game_play_data->snake_dead = 1;
            }        
            default:
            break;
    }

    if(game_play_data->game_stop){
        //SDL_Log("Game Over!\n");
        return;
    }

    //Move The Tail Up To The Head
    Snake *snake = game_play_data->player_data->snake_tail;
    while(snake->prev){
        if(snake->prev == game_play_data->player_data->snake_head)
            snake->pos = game_play_data->player_data->prev_head_pos;
        else
            snake->pos = snake->prev->pos;
        snake = snake->prev;
    }

}

static void update_state_game_playing(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager->state_pool[STATE_GAME_PLAYING] || !cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data) return;

    GamePlayData *game_play_data = (GamePlayData *)cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data;
    if(!game_play_data->game_stop){
        if((SDL_GetTicks() - game_play_data->player_data->last_move_time) >= game_play_data->player_data->move_delay){
            update_snake(game_play_data);
            game_play_data->player_data->last_move_time = SDL_GetTicks();
        }
        clear_board(game_play_data);
        update_board(game_play_data);
        update_apple(game_play_data);
    }
    
}

static void draw_board(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]) return;

    GamePlayData *game_play_data = (GamePlayData *)cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data;

    SDL_SetRenderDrawColor(cxt->app.renderer, 0, 0, 0, 255);

    SDL_FRect cell;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){

            switch(game_play_data->board[i][j]){
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
                    continue;
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

static void render_state_game_playing(GameContext *cxt){

    if(!cxt) return;

    draw_board(cxt);

}

static void exit_state_game_playing(GameContext *cxt){
    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]) return;

    GameState *current_state = cxt->game_state_manager->state_pool[STATE_GAME_PLAYING];
    for(int i = 0; i < current_state->button_count; i++){
        current_state->button_pool[i].button_flags.hovered = 0;
        current_state->button_pool[i].button_flags.pressed = 0;
    }

    GamePlayData *game_play_data = (GamePlayData *)current_state->state_data;
    if(game_play_data){
        game_play_data->game_stop = 1;
    }

    
    return;
}

void cleanup_snake(GamePlayData *game_play_data){

    if(!game_play_data || !game_play_data->player_data) return;

    Snake *temp, *snake = game_play_data->player_data->snake_tail;
    while(snake != NULL){
        temp = snake;
        snake = snake->prev;
        safe_free((void **)&temp);
    }
    
    game_play_data->player_data->snake_head = NULL;
    game_play_data->player_data->snake_tail = NULL;

}

void cleanup_player_data(GamePlayData *game_play_data){

    if(!game_play_data || !game_play_data->player_data) return;

    cleanup_snake(game_play_data);

    safe_free((void **)&game_play_data->player_data);

}

static void cleanup_state_game_playing_data(GameState *state){

    if(!state || !state->state_data) return;

    GamePlayData *game_play_data = state->state_data;
    cleanup_player_data(game_play_data);

}

static int init_player_data(GamePlayData *game_play_data){

    if(!game_play_data) return 1;

    game_play_data->player_data = malloc(sizeof(PlayerData));
    if(!game_play_data->player_data){
        SDL_Log("ERROR::Failed To Allocate Player Data\n");
        return 1;
    }
    memset(game_play_data->player_data, 0, sizeof(PlayerData));
    
    game_play_data->player_data->snake_head = malloc(sizeof(Snake));
    if(!game_play_data->player_data->snake_head){
        SDL_Log("ERROR::Failed To Allocate Snake Head\n");
        return 2;
    }
    memset(game_play_data->player_data->snake_head, 0, sizeof(Snake));
    
    game_play_data->player_data->dir = UP;
    game_play_data->player_data->snake_head->pos = (vec2){(BOARD_WIDTH - 2), (BOARD_HEIGHT - 2)};
    game_play_data->player_data->snake_head->next = NULL;
    game_play_data->player_data->snake_head->prev = NULL;

    game_play_data->player_data->snake_tail = game_play_data->player_data->snake_head;

    //Timer For Movement
    game_play_data->player_data->move_delay = 85;

    return 0;
}

static int init_game_play_data(GameState *state){

    if(!state) return 1;

    state->state_data = malloc(sizeof(GamePlayData));
    GamePlayData *new_data = (GamePlayData *)state->state_data;
    if(!new_data){
        SDL_Log("ERROR::Failed To Allocate Game Play Data\n");
        return 2;
    }
    memset(new_data, 0, sizeof(GamePlayData));

    if(init_player_data(new_data)){
        SDL_Log("ERROR::Init Player Data Failed\n");
        return 3;
    }

    new_data->apple = (vec2){1, 1};
    new_data->game_stop = 1;

    return 0;
}

static void on_click_button_pause(GameContext *cxt){
    if(!cxt) return;
    commit_pending_action(cxt->game_state_manager, STATE_ACTION_PUSH, cxt->game_state_manager->state_pool[STATE_PAUSE_MENU]);
    GamePlayData *game_play_data = (GamePlayData *)cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data;
    game_play_data->game_stop = 1;
}

GameState *init_state_game_playing(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return NULL;

    GameState *game_playing;
    game_playing = malloc(sizeof(GameState));
    if(!game_playing){
        SDL_Log("ERROR::Failed To Allocate Game Playing State\n");
        return NULL;
    }
    memset(game_playing, 0, sizeof(GameState));

    game_playing->state_enter = enter_state_game_playing;
    game_playing->state_handle_events = handle_state_events_game_playing;
    game_playing->state_update = update_state_game_playing;
    game_playing->state_render = render_state_game_playing;
    game_playing->state_exit = exit_state_game_playing;
    game_playing->cleanup_data = cleanup_state_game_playing_data;

    game_playing->button_count = 1;
    game_playing->button_pool = (game_playing->button_count <= 0)? NULL: malloc(sizeof(Button) * game_playing->button_count);
    memset(game_playing->button_pool, 0, (sizeof(Button) * game_playing->button_count));

    game_playing->button_pool[0] = init_button((vec2){75, 75}, (vec2){100, 100}, "PAUSE_BUTTON");
    game_playing->button_pool[0].on_click = on_click_button_pause;
    
    init_game_play_data(game_playing);
    if(!game_playing->state_data){
        SDL_Log("ERROR::Init Game Play Data Failed\n");
        return NULL;
    }

    game_playing->name = "Game_Playing_State";

    return game_playing;
}

static void reset_snake(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]) return;

    GamePlayData *game_play_data = (GamePlayData *)cxt->game_state_manager->state_pool[STATE_GAME_PLAYING]->state_data;
    cleanup_snake(game_play_data);
    game_play_data->player_data->snake_head = malloc(sizeof(Snake));
    if(!game_play_data->player_data->snake_head){
        SDL_Log("ERROR::Failed To Allocate Snake Head In Function Reset Snake\n");
        return;
    }
    memset(game_play_data->player_data->snake_head, 0, sizeof(Snake));

    game_play_data->player_data->dir = UP;
    game_play_data->player_data->directional_input = -1;
    game_play_data->player_data->snake_head->pos = (vec2){(BOARD_WIDTH - 2), (BOARD_HEIGHT - 2)};
    game_play_data->player_data->snake_head->next = NULL;
    game_play_data->player_data->snake_head->prev = NULL;

    game_play_data->player_data->snake_tail = game_play_data->player_data->snake_head;
    game_play_data->current_score = 0;
    SDL_Log("Current Score: %d\n", game_play_data->current_score);
}

void clear_board(GamePlayData *game_play_data){

    if(!game_play_data) return;

    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            game_play_data->board[i][j] = EMPTY;
        }
    }

}