#pragma once
#include "raylib.h"
#include "resources.h"

typedef struct _MathTaskData
{
    int num1;
    int num2;
    char op;
    int correctAnswer;
    int choicesList[4];
    int correctIndex;
    bool isActive;
} Ts_MathTaskData;

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

    Vector2 RightreturnRightArrowPos;
    Rectangle RightreturnArrowRec;

    Rectangle flashlightRec;

    // Helper positions and scale
    float scaleHelper;
    Vector2 helperPos;
    Rectangle helperRec;
    Vector2 tutorialWindowPos;
    int isHelpWindow;

    // Task Clock
    float interval;
    float currentTime;

    // Task Window
    int isLeftTaskTrue;
    int isCenterTaskTrue;
    int isRightTaskTrue;

    // Tasks options position and scale
    float optionScale;
    Vector2 optionPos;
    Rectangle optionRec;

    // Math Problem
    Ts_MathTaskData leftMathTask;
    Ts_MathTaskData centerMathTask;

    Rectangle leftMathAnswersRec[4];
    Rectangle centerMathAnswersRec[4];

    // Won or Lost
    int correctPoints;
    int incorrectPoints;

    // idle time
    float idleTime;
    float previousIdleTime;
    float safeTime;

    // Monologue length
    float monoLen;

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

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state);

void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartLeftTask(const Ts_resources *res, Ts_GameState *state);

void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskWindowLigth(const Ts_resources *res, Ts_GameState *state);

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state);

void LogicLostScreen(const Ts_resources *res, Ts_GameState *state);
void DrawLostScreen(const Ts_resources *res, Ts_GameState *state);

void LogicWonScreen(const Ts_resources *res, Ts_GameState *state);
void DrawWonScreen(const Ts_resources *res, Ts_GameState *state);

void StartTimerTask(const Ts_resources *res, Ts_GameState *state);
void AssignTask(const Ts_resources *res, Ts_GameState *state);

void playerIdleTimer(const Ts_resources *res, Ts_GameState *state);

void leftMathTaskGenerator(Ts_MathTaskData *task);
void centerMathTaskGenerator(Ts_MathTaskData *task);

void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw);
GameLogicFunction GetCurrentLogic();
GameDrawFunction GetCurrentDraw();