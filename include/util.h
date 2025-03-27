#ifndef UTIL_H
#define UTIL_H

#include "structs.h"

void safe_free(void **ptr);
void cap_frame_rate(Uint64 current_time);
bool vec4_compare(vec4 x, vec4 y, float epsilon);
vec4 vec4_lerp(vec4 vec_start, vec4 vec_target, float portion);
void vec4_snap_threshold(vec4 *vec_start, vec4 vec_target, float difference_threshold);
vec4 vec4_add(vec4 x, vec4 y);
void cleanup_sdl(GameContext *cxt);
void cleanup_game_context(GameContext *cxt);

#endif