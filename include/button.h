#ifndef BUTTON_H
#define BUTTON_H

#include "structs.h"

Button init_button(vec2 pos, vec2 size, const char *label);
bool check_button_pointer_collision(GameContext *cxt, Button *current_button);
void check_states_current_button(GameContext *cxt);
void render_states_current_button(GameContext *cxt);

#endif