#include "button.h"

Button init_button(vec2 pos, vec2 size, const char *label){

    Button new_button;
    new_button.pos = pos;
    new_button.size = size;
    new_button.base_color = (vec4){0, 200, 0 , 255};
    new_button.hover_color = (vec4){0, 255, 0, 255};
    new_button.current_color = (vec4)new_button.base_color;
    new_button.label = label;

    return new_button;
}

bool check_button_pointer_collision(GameContext *cxt, Button *current_button){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_tail) return 0;

    vec2 mouse = {cxt->app.mouse_x, cxt->app.mouse_y};

    float button_left = current_button->pos.x - (current_button->size.x / 2);
    float button_right = current_button->pos.x + (current_button->size.x / 2);
    float button_top = current_button->pos.y - (current_button->size.y / 2);
    float button_bottom = current_button->pos.y + (current_button->size.y / 2);

    bool collision_x = mouse.x > button_left && mouse.x < button_right;
    bool collision_y = mouse.y > button_top && mouse.y < button_bottom;

    return collision_x && collision_y;
}

void check_states_current_button(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_tail) return;

    GameState *current_state = cxt->game_state_manager->state_tail->state;
    if(!current_state || !current_state->button_pool) return;
    for(int i = 0; i < current_state->button_count; i++){
        Button *current_button = &current_state->button_pool[i];
        if(check_button_pointer_collision(cxt, current_button)){
            current_button->button_flags.hovered = 1;
        }
        else
            current_button->button_flags.hovered = 0;
    }

}

void render_state_buttons(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_tail) return;

    SDL_FRect button_rect;
    GameStateNode *state_node = cxt->game_state_manager->state_head.next;
    while(state_node != NULL){

        GameState *current_state = state_node->state;
        if(!current_state || !current_state->button_pool) return;
        for(int i = 0; i < current_state->button_count; i++){
            Button *current_button = &current_state->button_pool[i];

            button_rect.x = current_button->pos.x - (current_button->size.x / 2);
            button_rect.y = current_button->pos.y - (current_button->size.y / 2);
            button_rect.w = current_button->size.x;
            button_rect.h = current_button->size.y;

            if(current_state != cxt->game_state_manager->state_tail->state){
                current_button->button_flags.hovered = 0;
                current_button->button_flags.pressed = 0;
            }

            float lerp_speed = 0.13f;
            if(current_button->button_flags.hovered && !current_button->button_flags.pressed){    
                current_button->current_color = vec4_lerp(current_button->current_color, current_button->hover_color, lerp_speed);
            }
            else{
                current_button->current_color = vec4_lerp(current_button->current_color, current_button->base_color, lerp_speed);
            }
        
            SDL_SetRenderDrawColor(cxt->app.renderer, current_button->current_color.r, current_button->current_color.g, current_button->current_color.b, current_button->current_color.a);
            SDL_RenderFillRect(cxt->app.renderer, &button_rect);
        }

        state_node = state_node->next;
    }

}