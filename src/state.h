#pragma once
#include "raylib.h"
#include "resources.h"

// ----------------- initialization -----------------
typedef struct _GameState Ts_GameState;
typedef void (*GameLogicFunction)(const Ts_resources *res, Ts_GameState *state);
typedef void (*GameDrawFunction)(const Ts_resources *res, Ts_GameState *state);

typedef struct _Helper
{
    float scaleHelper;
    Vector2 helperPos;
    Rectangle helperRec;
    Vector2 tutorialWindowPos;
    int isHelpWindow;
} Ts_Helper;

typedef struct _BlinkEffect
{
    bool active;
    float currentTimer;
    float duration;
    float frequency;
    bool blinkOnce;
    Color color;
} Ts_BlinkEffect;

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

typedef struct _Jumpscare
{
    float jumpscareTimer;
    int jumpscareAudioPlayed;
    GameDrawFunction lastDrawFunction;
} Ts_Jumpscare;

typedef struct _Layout
{
    float scaleFit;
    Vector2 backgroundPosFit;

    float scaleArrow;
    float scaleReturnArrow;

    Vector2 centerArrowPos;
    Vector2 leftArrowPos;
    Vector2 rightArrowPos;
    Vector2 returnArrowPos;

    Rectangle centerArrowRec;
    Rectangle leftArrowRec;
    Rectangle rightArrowRec;
    Rectangle returnArrowRec;

    Vector2 RightTaskReturnArrowPos;
    Rectangle RightTaskReturnArrowRec;

    Rectangle flashlightRec;

    Rectangle leftMathAnswersRec[4];
    Rectangle centerMathAnswersRec[4];
    Rectangle rightMathAnswersRec[4];
} Ts_Layout;

typedef struct _Player
{
    int correctPoints;
    int incorrectPoints;
    float playerTotalTime;
    float idleTime;
    float previousIdleTime;
    float safeTime;
} Ts_Player;

typedef struct _GameState
{
    // Points for player to win or lose
    int maxCorrectPoints;
    int maxIncorrectPoints;

    Ts_Layout layoutConfig;

    Ts_Helper helperConfig;

    // Task Chance and Clock
    int chanceLeft;
    int chanceCenter;
    int chanceRight;
    float interval;
    float currentTime;

    // Task Window
    int isLeftTaskTrue;
    int isCenterTaskTrue;
    int isRightTaskTrue;

    // Math Problem
    Ts_MathTaskData leftMathTask;
    Ts_MathTaskData centerMathTask;
    Ts_MathTaskData rightMathTask;

    // Jumpscare
    Ts_Jumpscare JumpscareConfig;

    // Monologue length
    float monoLen;

    Ts_Player player;

    // Blink effect
    Ts_BlinkEffect blinkState;

} Ts_GameState;

// -------------------------- Prototyps --------------------------
Ts_GameState GameStateConfig(const Ts_resources *res);
Ts_GameState ResourcesLayout(Ts_GameState *state, const Ts_resources *res);

// -------------------------- Start Section --------------------------
void LogicStartScreen(const Ts_resources *res, Ts_GameState *state);
void DrawStartScreen(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Classroom Section --------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state);
void DrawClassroom(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Center Task --------------------------
void LogicCenterTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawCenterTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Right Task --------------------------
void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state);

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Left Task --------------------------
void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartLeftTask(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Lost & Won Section --------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state);
void DrawWonScreen(const Ts_resources *res, Ts_GameState *state);

void LogicLostScreen(const Ts_resources *res, Ts_GameState *state);
void DrawLostScreen(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Save Game Results --------------------------
void SaveGameResults(Ts_GameState *state, int playerWon);

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
void TriggerBlink(float duration, Color color, float frequency);
void UpdateBlink(void);
void DrawBlink(void);

// -------------------------- Other --------------------------
void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw);
GameLogicFunction GetCurrentLogic();
GameDrawFunction GetCurrentDraw();
