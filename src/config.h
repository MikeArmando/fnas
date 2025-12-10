#pragma once
#include "raylib.h"
#include "resources.h"
#include "state.h"

typedef struct _GameState Ts_GameState;

// -------------------------- Initial Game Config And Layout --------------------------
void GameStateConfig(const Ts_resources *res, Ts_GameState *state);
void CalculateGlobalScale(const Ts_resources *res, Ts_GameState *state);
void CalculateObjectPositions(Ts_GameState *state);
void CalculateHitboxes(const Ts_resources *res, Ts_GameState *state);