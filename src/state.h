#pragma once
#include "raylib.h"
#include "resources.h"

#define MAX_ANSWERS 4

// ----------------- initialization -----------------
typedef struct _GameState Ts_GameState;
typedef void (*GameLogicFunction)(const Ts_resources *res, Ts_GameState *state);
typedef void (*GameDrawFunction)(const Ts_resources *res, Ts_GameState *state);

typedef struct _Helper
{
    float scaleHelper;
    float scaleHelperWindow;
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

    // Background scale
    float scaleFit;
    Vector2 blinkPos;
    Rectangle blinkRec;
} Ts_BlinkEffect;

typedef struct _MathTaskData
{
    int num1;
    int num2;
    char op;
    int correctAnswer;
    int choicesList[MAX_ANSWERS];
    int correctIndex;
    bool isActive;
} Ts_MathTaskData;

typedef struct _Jumpscare
{
    float jumpscareTimer;
    int jumpscareAudioPlayed;
    GameDrawFunction lastDrawFunction;
} Ts_Jumpscare;

typedef struct _ArrowLayout
{
    float scaleArrow;
    float scaleReturnArrow;

    Vector2 centerArrowPos;
    Vector2 leftArrowPos;
    Vector2 rightArrowPos;
    Vector2 returnArrowPos;
    Vector2 RightReturnArrowPos;

    Rectangle centerArrowRec;
    Rectangle leftArrowRec;
    Rectangle rightArrowRec;
    Rectangle returnArrowRec;
    Rectangle RightReturnArrowRec;
} Ts_ArrowLayout;

typedef struct _Layout
{
    float scaleFit;
    float scaleUI;
    float offsetX;
    float offsetY;
    Vector2 positionAbsolute;

    Rectangle flashlightRec;

    // Math Window
    Rectangle bookMathAnswersRec[MAX_ANSWERS];
    Rectangle chalkboardMathAnswersRec[MAX_ANSWERS];

    // Book Math Section
    int bookFontSizeQuestion;
    int bookFontSizeAnswer;
    Vector2 MathOpLine1Pos;
    Vector2 MathOpLine2Pos;

    // Book Math Section
    int chalkboardFontSizeQuestion;
    int chalkboardFontSizeAnswer;
    Vector2 chalkboardMathQuestionPos;

    // Won/Lost Screen
    Vector2 startTitlePos;
    float instructionY;
    float creditsTitleY;
    float credit1Y;
    float credit2Y;
    float gameResults;

    // Best time
    float bestTimeFontSize;
    Vector2 bestTimeTextPos;

    int fontSizeTitle;
    int fontSizeInstruction;
    int fontSizeResults;
    int fontSizeCredits;
} Ts_Layout;

typedef struct _TutorialLayout
{
    // Tutorial btn
    int tutorialBtnFontSize;
    Vector2 tutorialBtnPos;
    Rectangle tutorialBtnRec;

    // Next btn
    int nextBtnFontSize;
    Vector2 nextBtnPos;
    Rectangle nextBtnRec;

    // Tutorial Pages
    int currentPageIndex;
    int maxPages;
} Ts_TutorialLayout;

typedef struct _Player
{
    int correctPoints;
    int incorrectPoints;
    float playerTotalTime;
    float bestTime;
    float idleTime;
    float previousIdleTime;
    float safeTime;
} Ts_Player;

typedef struct _Task
{
    int chanceLeft;
    int chanceCenter;
    int chanceRight;
    float interval;
    float currentTime;
    int maxCorrectPoints;
    int maxIncorrectPoints;
} Ts_Task;

typedef struct _GameState
{
    bool gameInit;

    Ts_Layout layout;

    Ts_ArrowLayout arrow;

    Ts_MathTaskData leftMathTask;
    Ts_MathTaskData centerMathTask;
    Ts_MathTaskData rightMathTask;

    Ts_Jumpscare Jumpscare;

    Ts_Player player;

    Ts_Helper helper;

    Ts_BlinkEffect blinkState;

    Ts_Task task;

    Ts_TutorialLayout tutorial;

    // Task Window
    int isLeftTaskTrue;
    int isCenterTaskTrue;
    int isRightTaskTrue;

    // Monologue length
    float monoLen;
} Ts_GameState;

// -------------------------- AUX FUNCTION --------------------------
void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw);
GameLogicFunction GetCurrentLogic();
GameDrawFunction GetCurrentDraw();

// -------------------------- Initial Game Config --------------------------
void GameStateConfig(const Ts_resources *res, Ts_GameState *state);
void ResourcesLayout(const Ts_resources *res, Ts_GameState *state);
void CalculateGlobalScale(const Ts_resources *res, Ts_GameState *state);
void CalculateObjectPositions(Ts_GameState *state);
void CalculateHitboxes(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Start Section --------------------------
void LogicStartScreen(const Ts_resources *res, Ts_GameState *state);
void DrawStartScreen(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Classroom Section --------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state);
void DrawClassroom(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Center Task --------------------------
void LogicCenterTask(const Ts_resources *res, Ts_GameState *state);
void DrawCenterTask(const Ts_resources *res, Ts_GameState *state);

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Right Task --------------------------
void LogicRightTask(const Ts_resources *res, Ts_GameState *state);
void DrawRightTask(const Ts_resources *res, Ts_GameState *state);

void LogicRightTaskLight(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskLight(const Ts_resources *res, Ts_GameState *state);

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Left Task --------------------------
void LogicLeftTask(const Ts_resources *res, Ts_GameState *state);
void DrawLeftTask(const Ts_resources *res, Ts_GameState *state);

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state);
void DrawStartLeftTask(const Ts_resources *res, Ts_GameState *state);

// -------------------------- Lost & Won Section --------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state);
void DrawWonScreen(const Ts_resources *res, Ts_GameState *state);

void LogicLostScreen(const Ts_resources *res, Ts_GameState *state);
void DrawLostScreen(const Ts_resources *res, Ts_GameState *state);

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
void TriggerBlink(float duration, Color color, float frequency);
void UpdateBlink(void);
void DrawBlink(const Ts_GameState *state);

// -------------------------- Tutorial --------------------------
void LogicTutorial(const Ts_resources *res, Ts_GameState *state);
void DrawTutorial(const Ts_resources *res, Ts_GameState *state);