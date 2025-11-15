#pragma once
#include "raylib.h"
#include "resources.h"

typedef struct _GameState
{
    // Background Config
    float scaleFit;
    Vector2 backgroundPosFit;

    // Arrow positions and scale
    float scaleCenterArrow;
    Vector2 centerArrowPos;
    Rectangle centerArrowRec;

    float scaleLeftArrow;
    Vector2 leftArrowPos;
    Rectangle leftArrowRec;

    float scaleRightArrow;
    Vector2 rightArrowPos;
    Rectangle rightArrowRec;

    float scaleReturnArrow;
    Vector2 returnArrowPos;
    Rectangle returnArrowRec;

    // Helper positions and scale
    float scaleHelper;
    Vector2 helperPos;
    Rectangle helperRec;
    Vector2 tutorialWindowPos;
    int isHelpWindow;

    // Task clock
    float interval;
    float currentTime;

    // Task selector
    int startTask;
    int newIndex;

    // Task Window
    int isLeftTaskTrue;
    int isCenterTaskTrue;
    int isRightTaskTrue;

} Ts_GameState;

// ----------------- Prototyps -----------------
typedef void (*GameLogicFunction)(const Ts_resources *res, Ts_GameState *state);
typedef void (*GameDrawFunction)(const Ts_resources *res, Ts_GameState *state);

Ts_GameState GameState_config(const Ts_resources *res);

void LogicStartScreen(const Ts_resources *res, Ts_GameState *state);
void DrawStartScreen(const Ts_resources *res, Ts_GameState *state);

void LogicClassroom(const Ts_resources *res, Ts_GameState *state);
void DrawClassroom(const Ts_resources *res, Ts_GameState *state);

void LogicCenterTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawCenterTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicLostScreen(const Ts_resources *res, Ts_GameState *state);
void DrawLostScreen(const Ts_resources *res, Ts_GameState *state);

void LogicWonScreen(const Ts_resources *res, Ts_GameState *state);
void DrawWonScreen(const Ts_resources *res, Ts_GameState *state);

void StartTimerTask(const Ts_resources *res, Ts_GameState *state);

void LogicStartLeftTaskTrue(const Ts_resources *res, Ts_GameState *state);
void DrawStartLeftTaskTrue(const Ts_resources *res, Ts_GameState *state);

void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw);
GameLogicFunction GetCurrentLogic();
GameDrawFunction GetCurrentDraw();