#ifndef UTIL_H
#define UTIL_H

#include "structs.h"

void safe_free(void **ptr);
void cleanup_sdl(GameContext *cxt);
void cleanup_game_context(GameContext *cxt);

#endif