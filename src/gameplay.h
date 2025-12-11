#pragma once
#include "raylib.h"
#include "resources.h"
#include "state.h"
#include <math.h>

typedef struct _GameState Ts_GameState;
typedef struct _MathTaskData Ts_MathTaskData;

// -------------------------- Task Handle --------------------------
void HandleMathAnswer(const Ts_resources *res, Ts_GameState *state, Ts_MathTaskData *currentTask, int answerIndex, int *taskFlag);

// -------------------------- Game Results --------------------------
void SaveGameResults(Ts_GameState *state, int playerWon);
float LoadBestTime();

// -------------------------- JumpScare --------------------------
void TriggerJumpscare(Ts_GameState *state);
void LogicJumpscare(const Ts_resources *res, Ts_GameState *state);
void DrawJumpscare(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Timer --------------------------
void StartTimerTask(const Ts_resources *res, Ts_GameState *state);
void AssignTask(const Ts_resources *res, Ts_GameState *state);
void playerIdleTimer(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Math Generator --------------------------
void leftMathTaskGenerator(Ts_MathTaskData *task);
void centerMathTaskGenerator(Ts_MathTaskData *task);
void rightMathTaskGenerator(Ts_MathTaskData *task);
void assignWrongChoices(Ts_MathTaskData *task);

// -------------------------- Blink Effect --------------------------
void TriggerBlink(Ts_GameState *state, float duration, Color color, float frequency);
void UpdateBlink(Ts_GameState *state);
void DrawBlink(Ts_GameState *state);