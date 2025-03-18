#ifndef UTIL_H
#define UTIL_H

#include "structs.h"

void safe_free(void **ptr);
void cap_frame_rate(Uint64 current_time);
void cleanup_snake(GameContext *cxt);
void cleanup_sdl(GameContext *cxt);
void cleanup_game_context(GameContext *cxt);

#endif