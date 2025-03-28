#ifndef UTIL_H
#define UTIL_H

#include "structs.h"

void safe_free(void **ptr);
void cap_frame_rate(Uint64 current_time);
bool point_rect_collision(vec2 point, vec2 rect_pos, vec2 rect_size);
vec4 vec4_lerp(vec4 vec_start, vec4 vec_target, float portion);
void cleanup_sdl(GameContext *cxt);
void cleanup_game_context(GameContext *cxt);

#endif