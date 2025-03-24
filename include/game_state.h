#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "structs.h"
#include "render.h"
#include "update.h"

void push_state(GameContext*cxt, GameState *state);
void pop_state(GameContext *cxt);
void commit_pending_action(GameStateManager *manager, int state_action, GameState *state);
void update_states(GameContext *cxt);
void render_states(GameContext *cxt);

#endif