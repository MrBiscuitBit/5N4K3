#include "pause_menu.h"

static void enter_state_pause_menu(GameContext *cxt){
    return;
}

static void handle_state_events_pause_menu(GameContext *cxt, SDL_Event *event){

    if(!cxt || !event) return;

    if(event->type == SDL_EVENT_KEY_DOWN){
        switch(event->key.scancode){
            case SDL_SCANCODE_ESCAPE:
                commit_pending_action(cxt->game_state_manager, STATE_ACTION_POP, NULL);
                break;
            default:
                break;
        }
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){

        PauseMenuData *pause_data = (PauseMenuData *)cxt->game_state_manager->state_pool[STATE_PAUSE_MENU]->state_data;
        if(pause_data && pause_data->board_size_switches){
            for(int i = 0; i < pause_data->switch_count; i++){

                SwitchButton *current_switch = &pause_data->board_size_switches[i];
                if(point_rect_collision((vec2){cxt->app.mouse_x, cxt->app.mouse_y}, current_switch->pos, current_switch->size)){

                    current_switch->is_on = 1;

                    for(int j = 0; j < pause_data->switch_count; j++){
                        if(j != i)
                            pause_data->board_size_switches[j].is_on = 0;
                    }

                    break;
                }

            }
        }

    }

}

static void update_state_pause_menu(GameContext *cxt){


    return;
}

static void render_state_pause_menu(GameContext *cxt){

    SDL_FRect rect;
    rect.w = (WND_WIDTH * 3) / 4;
    rect.h = (WND_HEIGHT * 3) / 4;
    rect.x = (WND_WIDTH / 2) - (rect.w / 2);
    rect.y = (WND_HEIGHT / 2) - (rect.h / 2);

    SDL_SetRenderDrawColor(cxt->app.renderer, 255, 51, 255, 255);
    SDL_RenderFillRect(cxt->app.renderer, &rect);

    PauseMenuData *pause_data = (PauseMenuData *)cxt->game_state_manager->state_pool[STATE_PAUSE_MENU]->state_data;
    for(int i = 0; i < 3; i++){
        SwitchButton *current_switch = &pause_data->board_size_switches[i];

        if(current_switch->is_on)
            current_switch->current_color = current_switch->on_color;
        else
            current_switch->current_color = current_switch->off_color;

        SDL_SetRenderDrawColor(cxt->app.renderer, current_switch->current_color.r, current_switch->current_color.g,
                               current_switch->current_color.b, current_switch->current_color.a);

        rect.w = current_switch->size.x;
        rect.h = current_switch->size.y;
        rect.x = current_switch->pos.x - (rect.w / 2);
        rect.y = current_switch->pos.y - (rect.h / 2);
        SDL_RenderFillRect(cxt->app.renderer, &rect);
    }

}

static void exit_state_pause_menu(GameContext *cxt){
    GameState *current_state = cxt->game_state_manager->state_pool[STATE_GAME_PLAYING];
    for(int i = 0; i < current_state->button_count; i++){
        current_state->button_pool[i].button_flags.hovered = 0;
        current_state->button_pool[i].button_flags.pressed = 0;
    }
}

static void cleanup_pause_menu_data(GameState *state){
    if(!state || !state->state_data) return;
    safe_free((void **)&state->state_data);
}

static int init_pause_menu_data(GameState *state){

    if(!state) return 1;

    state->state_data = malloc(sizeof(PauseMenuData));
    if(!state->state_data){
        SDL_Log("ERROR::Failed To Allocate Pause Menu Data");
        return 2;
    }
    memset(state->state_data, 0, sizeof(PauseMenuData));

    PauseMenuData *pause_data = (PauseMenuData *)state->state_data;
    
    pause_data->switch_count = 3;
    pause_data->board_size_switches = (pause_data->switch_count <= 0)? NULL: malloc(sizeof(SwitchButton) * pause_data->switch_count);

    vec4 switch_off_color = (vec4){153, 255, 204, 255};
    pause_data->board_size_switches[0].off_color = switch_off_color;
    pause_data->board_size_switches[1].off_color = switch_off_color;
    pause_data->board_size_switches[2].off_color = switch_off_color;
    
    vec4 switch_on_color = (vec4){0, 255, 54, 255};
    pause_data->board_size_switches[0].on_color = switch_on_color;
    pause_data->board_size_switches[1].on_color = switch_on_color;
    pause_data->board_size_switches[2].on_color = switch_on_color;

    pause_data->board_size_switches[0].pos = (vec2){(WND_WIDTH / 2) - 300, (WND_HEIGHT / 2) - 175};
    pause_data->board_size_switches[1].pos = (vec2){(WND_WIDTH / 2), (WND_HEIGHT / 2) - 175};
    pause_data->board_size_switches[2].pos = (vec2){(WND_WIDTH / 2) + 300, (WND_HEIGHT / 2) - 175};

    pause_data->board_size_switches[0].size = (vec2){100, 100};
    pause_data->board_size_switches[1].size = (vec2){100, 100};
    pause_data->board_size_switches[2].size = (vec2){100, 100};

    pause_data->board_size_switches[1].is_on = 1;

    return 0;
}

static void on_click_button_resume(GameContext *cxt){
    if(!cxt || !cxt->game_state_manager) return;
    commit_pending_action(cxt->game_state_manager, STATE_ACTION_POP, NULL);
}

static void on_click_button_exit(GameContext *cxt){
    if(!cxt || !cxt->game_state_manager) return;
    commit_pending_action(cxt->game_state_manager, STATE_ACTION_POP_SWAP, cxt->game_state_manager->state_pool[STATE_MAIN_MENU]);
}

GameState *init_state_pause_menu(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager) return NULL;

    GameState *pause_menu = malloc(sizeof(GameState));
    if(!pause_menu){
        SDL_Log("ERROR::Failed To Allocate State Pause Menu\n");
        return NULL;
    }
    memset(pause_menu, 0, sizeof(GameState));

    pause_menu->state_enter = enter_state_pause_menu;
    pause_menu->state_handle_events = handle_state_events_pause_menu;
    pause_menu->state_update = update_state_pause_menu;
    pause_menu->state_render = render_state_pause_menu;
    pause_menu->state_exit = exit_state_pause_menu;
    pause_menu->cleanup_data = cleanup_pause_menu_data;

    init_pause_menu_data(pause_menu);
    if(!pause_menu->state_data){
        SDL_Log("WARNING::Init Pause Menu Data Failed\n");
    }
    
    pause_menu->button_count = 2;
    pause_menu->button_pool = (pause_menu->button_count <= 0)? NULL: malloc(sizeof(Button) * pause_menu->button_count);
    memset(pause_menu->button_pool, 0, (sizeof(Button) * pause_menu->button_count));

    pause_menu->button_pool[0] = init_button((vec2){WND_WIDTH / 2, WND_HEIGHT / 2}, (vec2){300, 100}, "RESUME");
    pause_menu->button_pool[0].on_click = on_click_button_resume;

    pause_menu->button_pool[1] = init_button((vec2){WND_WIDTH / 2, (WND_HEIGHT / 2) + 150}, (vec2){300, 100}, "EXIT");
    pause_menu->button_pool[1].on_click = on_click_button_exit;

    pause_menu->name = "PAUSE_MENU";

    return pause_menu;
}