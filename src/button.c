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
    if(!current_state->button_pool) return;
    for(int i = 0; i < current_state->button_count; i++){
        Button *current_button = &current_state->button_pool[i];
        if(check_button_pointer_collision(cxt, current_button)){
            current_button->button_flags.hovered = 1;
        }
        else
            current_button->button_flags.hovered = 0;
    }

}

void render_states_current_button(GameContext *cxt){

    if(!cxt || !cxt->game_state_manager || !cxt->game_state_manager->state_tail) return;

    SDL_FRect button_rect;
    GameState *current_state = cxt->game_state_manager->state_tail->state;
    if(!current_state->button_pool) return;
    for(int i = 0; i < current_state->button_count; i++){
        Button *current_button = &current_state->button_pool[i];

        button_rect.x = current_button->pos.x - (current_button->size.x / 2);
        button_rect.y = current_button->pos.y - (current_button->size.y / 2);
        button_rect.w = current_button->size.x;
        button_rect.h = current_button->size.y;

    ///*
        float color_epsilon = 0.000005f;
        float lerp_portion = 0.13f;
        if(current_button->button_flags.hovered && !vec4_compare(current_button->current_color, current_button->hover_color, color_epsilon)){
            if(!vec4_compare(current_button->current_color, current_button->hover_color, color_epsilon))
                current_button->current_color = vec4_lerp(current_button->current_color, current_button->hover_color, lerp_portion);
        }
        else if(!vec4_compare(current_button->current_color, current_button->base_color, color_epsilon)){
            if(!vec4_compare(current_button->current_color, current_button->base_color, color_epsilon))
                current_button->current_color = vec4_lerp(current_button->current_color, current_button->base_color, lerp_portion);
        }
        SDL_SetRenderDrawColor(cxt->app.renderer, current_button->current_color.r, current_button->current_color.g,
                               current_button->current_color.b, current_button->current_color.a);
    //*/
    /*
        if(current_button->button_flags.hovered)
            SDL_SetRenderDrawColor(cxt->app.renderer, current_button->hover_color.r, current_button->hover_color.g, current_button->hover_color.b, current_button->hover_color.a);
        else
            SDL_SetRenderDrawColor(cxt->app.renderer, current_button->base_color.r, current_button->base_color.g, current_button->base_color.b, current_button->base_color.a);
    */
    
        SDL_SetRenderDrawColor(cxt->app.renderer, current_button->current_color.r, current_button->current_color.g, current_button->current_color.b, current_button->current_color.a);
        SDL_RenderFillRect(cxt->app.renderer, &button_rect);
    }

}