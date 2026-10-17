#include <stdio.h>
#include "state.h"
#include <math.h>
#include "gameplay.h"

#define BASE_WIDTH 1250.0f
#define BASE_HEIGHT 750.0f

static GameLogicFunction currentLogic = 0;
static GameDrawFunction currentDraw = 0;

// --------------------------------------- UPDATE GAME STATE ---------------------------------------
void UpdateGameplaySystems(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->music.monoV1);
    UpdateMusicStream(res->music.background);

    playerIdleTimer(res, state);
    StartTimerTask(res, state);

    if (state->monoLen > 0.001f)
    {
        float monoTimePlayed = GetMusicTimePlayed(res->music.monoV1);

        if (monoTimePlayed + 0.05f >= state->monoLen)
        {
            StopMusicStream(res->music.monoV1);
        }
    }
}

bool IsGameplayState(GameLogicFunction logicToCheck)
{
    if (logicToCheck == LogicStartScreen)
    {
        return false;
    }
    if (logicToCheck == LogicTutorial)
    {
        return false;
    }
    if (logicToCheck == LogicLostScreen)
    {
        return false;
    }
    if (logicToCheck == LogicWonScreen)
    {
        return false;
    }

    return true;
}

// --------------------------------------- CHANGE GAME STATE ---------------------------------------
void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw)
{
    currentLogic = newLogic;
    currentDraw = newDraw;
}

GameLogicFunction GetCurrentLogic()
{
    return currentLogic;
}

GameDrawFunction GetCurrentDraw()
{
    return currentDraw;
}

// --------------------------------------- START SECTION ---------------------------------------
void LogicStartScreen(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    UpdateMusicStream(res->music.intro);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->music.intro);
        PlayMusicStream(res->music.monoV1);
        PlayMusicStream(res->music.background);

        state->player.playerTotalTime = 0;
        state->player.incorrectPoints = 0;
        state->player.correctPoints = 0;
        state->player.idleTime = 0.0f;
        state->player.safeTime = 0.0f;
        for (int i = 0; i < TASK_COUNT; i++)
        {
            state->mathTask[i].isActive = false;
        }
        state->layout.tutorial.silenceBtnCurrentIndex = 1;

        TriggerBlink(state, 0.6f, BLACK, 0.0f);
        ChangeGameState(LogicClassroom, DrawClassroom);
        return;
    }

    if (CheckCollisionPointRec(mousePos, state->layout.tutorial.tutorialBtnRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(state, 0.2f, BLACK, 0.0F);
            ChangeGameState(LogicTutorial, DrawTutorial);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->layout.tutorial.silenceBtnRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            if (state->layout.tutorial.silenceBtnCurrentIndex == 0)
            {
                PlayMusicStream(res->music.intro);
                state->layout.tutorial.silenceBtnCurrentIndex = 1;
            }
            else
            {
                StopMusicStream(res->music.intro);
                state->layout.tutorial.silenceBtnCurrentIndex = 0;
            }
        }
    }

    if (isMouseOverClickable)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawStartScreen(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layout;

    DrawText("Five Nigths At School", layout->startAndLost.startTitlePos.x, layout->startAndLost.startTitlePos.y, layout->startAndLost.fontSizeTitle, WHITE);
    DrawText("Presiona [ENTER] para comenzar . . .", layout->startAndLost.startTitlePos.x, layout->startAndLost.instructionY, layout->startAndLost.fontSizeInstruction, WHITE);
    DrawText("PRESENTADO POR:", layout->startAndLost.startTitlePos.x, layout->startAndLost.creditsTitleY, layout->startAndLost.fontSizeCredits, WHITE);
    DrawText("Perez Aguirre Mextli Citlali - Diseño y Arte", layout->startAndLost.startTitlePos.x, layout->startAndLost.credit1Y, layout->startAndLost.fontSizeCredits, WHITE);
    DrawText("Montano Valencia Mike Armando - Desarrollo de Software", layout->startAndLost.startTitlePos.x, layout->startAndLost.credit2Y, layout->startAndLost.fontSizeCredits, WHITE);
    DrawTextureEx(res->texture.menuBtns[layout->tutorial.silenceBtnCurrentIndex], state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);

    if (state->player.bestTime > 0.0f)
    {
        char bestTimeText[50];
        sprintf(bestTimeText, "MEJOR TIEMPO: %.2f seg", state->player.bestTime);
        DrawText(bestTimeText, layout->startAndLost.bestTimeTextPos.x, layout->startAndLost.bestTimeTextPos.y, layout->startAndLost.bestTimeFontSize, GOLD);
    }
}

// --------------------------------------- TUTORIAL SECTION ---------------------------------------
void LogicTutorial(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    UpdateMusicStream(res->music.intro);

    if (CheckCollisionPointRec(mousePos, state->layout.tutorial.nextBtnRec))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(state, 0.2f, BLACK, 0.0F);
            PlaySound(res->sound.arrowClick);
            if (state->layout.tutorial.currentPageIndex < state->layout.tutorial.maxPages - 1)
            {
                state->layout.tutorial.currentPageIndex++;
            }
            else
            {
                state->layout.tutorial.currentPageIndex = 0;
                ChangeGameState(LogicStartScreen, DrawStartScreen);
                return;
            }
        }
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawTutorial(const Ts_resources *res, Ts_GameState *state)
{
    Texture2D currentPageTexture = res->texture.tutorialPages[state->layout.tutorial.currentPageIndex];
    DrawTextureEx(currentPageTexture, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
}

// --------------------------------------- CLASSROOM SECTION---------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.leftArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicLeftTask, DrawLeftTask);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layout.arrow.centerArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicCenterTask, DrawCenterTask);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layout.arrow.rightArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.doorOpening);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicRightTask, DrawRightTask);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helper.helperRec))
    {
        isMouseOverClickable = 1;
        state->helper.isHelpWindow = 1;
    }
    else
    {
        state->helper.isHelpWindow = 0;
    }

    if (isMouseOverClickable)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawClassroom(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->texture.classroom, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.centerArrow, state->layout.arrow.centerArrowPos, 0.0f, state->layout.arrow.scaleArrow, WHITE);
    DrawTextureEx(res->texture.leftArrow, state->layout.arrow.leftArrowPos, 0.0f, state->layout.arrow.scaleArrow, WHITE);
    DrawTextureEx(res->texture.rightArrow, state->layout.arrow.rightArrowPos, 0.0f, state->layout.arrow.scaleArrow, WHITE);

    if (state->mathTask[TASK_LEFT].isActive)
    {
        DrawTextureEx(res->texture.leftIndicator, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    }
    if (state->mathTask[TASK_CENTER].isActive)
    {
        DrawTextureEx(res->texture.centerIndicator, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    }
    if (state->helper.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperGeneralWindow, state->helper.tutorialWindowPos, 0.0f, state->helper.scaleHelperWindow, WHITE);
    }
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);
}

// --------------------------------------- SHARED TASK HELPERS ---------------------------------------
static bool IsClicked(Vector2 mousePos, Rectangle rec, bool *isOver)
{
    if (!CheckCollisionPointRec(mousePos, rec))
    {
        return false;
    }

    *isOver = true;
    return IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

static void ApplyCursor(bool isOver)
{
    SetMouseCursor(isOver ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);
}

static void UpdateHelperHover(Ts_GameState *state, bool *isOver)
{
    if (CheckCollisionPointRec(GetMousePosition(), state->helper.helperRec))
    {
        *isOver = true;
        state->helper.isHelpWindow = 1;
    }
    else
    {
        state->helper.isHelpWindow = 0;
    }
}

// Returns true when an answer was selected (the caller must stop processing the frame).
static bool HandleAnswerClicks(const Ts_resources *res, Ts_GameState *state, Ts_MathTaskData *task,
                               const Rectangle recs[MAX_ANSWERS], bool *isOver)
{
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < MAX_ANSWERS; i++)
    {
        if (IsClicked(mousePos, recs[i], isOver))
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            HandleMathAnswer(res, state, task, i);
            return true;
        }
    }
    return false;
}

static void DrawMathAnswers(const Ts_MathTaskData *task, const Rectangle recs[MAX_ANSWERS],
                            int fontSize, Color color, bool boxed)
{
    for (int i = 0; i < MAX_ANSWERS; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", task->choicesList[i]);

        if (boxed)
        {
            DrawRectangleLinesEx(recs[i], 3, color);
        }

        int textWidth = MeasureText(answersOnScreen, fontSize);

        int answerPosX = (int)(recs[i].x + (recs[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(recs[i].y + (recs[i].height / 2) - (fontSize / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, fontSize, color);
    }
}

// Vertical-operation layout on the book (used by the left and right tasks).
static void DrawBookTask(const Ts_resources *res, Ts_GameState *state, const Ts_MathTaskData *task,
                         Texture2D background, Vector2 returnArrowPos, Texture2D helperWindow)
{
    Ts_Layout *layout = &state->layout;

    DrawTextureEx(background, layout->positionAbsolute, 0.0f, layout->scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, returnArrowPos, 0.0f, layout->arrow.scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);

    char mathNum1[16];
    sprintf(mathNum1, "%d", task->num1);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", task->op, task->num2);

    int widthNum1 = MeasureText(mathNum1, layout->math.bookFontSizeQuestion);
    int widthLine2 = MeasureText(mathNum2, layout->math.bookFontSizeQuestion);

    DrawText(mathNum1,
             (int)(layout->math.MathOpLine1Pos.x - widthNum1),
             (int)layout->math.MathOpLine1Pos.y,
             layout->math.bookFontSizeQuestion, BLACK);

    DrawText(mathNum2,
             (int)(layout->math.MathOpLine2Pos.x - widthLine2),
             (int)layout->math.MathOpLine2Pos.y,
             layout->math.bookFontSizeQuestion, BLACK);

    DrawMathAnswers(task, layout->math.bookMathAnswersRec, layout->math.bookFontSizeAnswer, BLACK, false);

    if (state->helper.isHelpWindow)
    {
        DrawTextureEx(helperWindow, state->helper.tutorialWindowPos, 0.0f, state->helper.scaleHelperWindow, WHITE);
    }
}

// --------------------------------------- LEFT SECTION ---------------------------------------
void LogicLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (state->mathTask[TASK_LEFT].isActive)
    {
        PlaySound(res->sound.suspenseSound);
        ChangeGameState(LogicStartLeftTask, DrawStartLeftTask);
        return;
    }

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.returnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (isMouseOverClickable)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->texture.leftTaskWindow, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layout.arrow.returnArrowPos, 0.0f, state->layout.arrow.scaleReturnArrow, WHITE);
}

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->mathTask[TASK_LEFT];
    bool isOver = false;

    if (IsClicked(mousePos, state->layout.arrow.returnArrowRec, &isOver))
    {
        TriggerBlink(state, 0.2f, BLACK, 0.0f);
        ChangeGameState(LogicClassroom, DrawClassroom);
        return;
    }

    if (IsClicked(mousePos, state->layout.noseRec, &isOver))
    {
        PlaySound(res->sound.CatMeow);
        return;
    }

    UpdateHelperHover(state, &isOver);

    if (HandleAnswerClicks(res, state, currentTask, state->layout.math.bookMathAnswersRec, &isOver))
    {
        return;
    }

    ApplyCursor(isOver);
}

void DrawStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawBookTask(res, state, &state->mathTask[TASK_LEFT], res->texture.leftTaskTrue,
                 state->layout.arrow.returnArrowPos, res->texture.helperSumWindow);
}

// --------------------------------------- CENTER SECTION ---------------------------------------
void LogicCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (state->mathTask[TASK_CENTER].isActive)
    {
        ChangeGameState(LogicStartCenterTask, DrawStartCenterTask);
        return;
    }

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.returnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (isMouseOverClickable)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->texture.centerTaskWindow, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layout.arrow.returnArrowPos, 0.0f, state->layout.arrow.scaleReturnArrow, WHITE);
}

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->mathTask[TASK_CENTER];
    bool isOver = false;

    if (IsClicked(mousePos, state->layout.arrow.returnArrowRec, &isOver))
    {
        TriggerBlink(state, 0.2f, BLACK, 0.0f);
        ChangeGameState(LogicClassroom, DrawClassroom);
        return;
    }

    UpdateHelperHover(state, &isOver);

    if (HandleAnswerClicks(res, state, currentTask, state->layout.math.chalkboardMathAnswersRec, &isOver))
    {
        return;
    }

    ApplyCursor(isOver);
}

void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Ts_MathTaskData *currentTask = &state->mathTask[TASK_CENTER];
    Ts_Layout *layout = &state->layout;

    DrawTextureEx(res->texture.centerTaskWindow, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->arrow.returnArrowPos, 0.0f, layout->arrow.scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);

    char questionStr[16];
    sprintf(questionStr, "%d %c %d %s", currentTask->num1, currentTask->op, currentTask->num2, "= ?");
    DrawText(questionStr, layout->math.chalkboardMathQuestionPos.x, layout->math.chalkboardMathQuestionPos.y, layout->math.chalkboardFontSizeQuestion, LIGHTGRAY);

    DrawMathAnswers(currentTask, layout->math.chalkboardMathAnswersRec, layout->math.chalkboardFontSizeAnswer, LIGHTGRAY, true);

    if (state->helper.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperMultWindow, state->helper.tutorialWindowPos, 0.0f, state->helper.scaleHelperWindow, WHITE);
    }
}

// --------------------------------------- RIGHT SECTION ---------------------------------------
void LogicRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.RightReturnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.doorOpening);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->layout.flashlightRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            ChangeGameState(LogicRightTaskLight, DrawRightTaskLight);
            return;
        }
    }

    if (isMouseOverClickable)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawRightTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->texture.rightTaskWindow, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layout.arrow.RightReturnArrowPos, 0.0f, state->layout.arrow.scaleReturnArrow, WHITE);
}

void LogicRightTaskLight(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.RightReturnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.doorOpening);
            TriggerBlink(state, 0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->layout.flashlightRec))
    {
        isMouseOverClickable = 1;

        if (state->mathTask[TASK_RIGHT].isActive)
        {
            PlaySound(res->sound.girlSuspense);
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            ChangeGameState(LogicRightTask, DrawRightTask);
            return;
        }
    }

    if (state->mathTask[TASK_RIGHT].isActive)
    {
        ChangeGameState(LogicStartRightTask, DrawStartRightTask);
        return;
    }

    if (isMouseOverClickable)
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawRightTaskLight(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->texture.rightTaskWindowLights, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layout.arrow.RightReturnArrowPos, 0.0f, state->layout.arrow.scaleReturnArrow, WHITE);
}

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->mathTask[TASK_RIGHT];
    bool isOver = false;

    if (IsClicked(mousePos, state->layout.arrow.RightReturnArrowRec, &isOver))
    {
        PlaySound(res->sound.doorOpening);
        TriggerBlink(state, 0.2f, BLACK, 0.0f);
        ChangeGameState(LogicClassroom, DrawClassroom);
        return;
    }

    if (IsClicked(mousePos, state->layout.flashlightRec, &isOver))
    {
        PlaySound(res->sound.arrowClick);
        ChangeGameState(LogicRightTask, DrawRightTask);
        return;
    }

    UpdateHelperHover(state, &isOver);

    if (HandleAnswerClicks(res, state, currentTask, state->layout.math.bookMathAnswersRec, &isOver))
    {
        return;
    }

    ApplyCursor(isOver);
}

void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawBookTask(res, state, &state->mathTask[TASK_RIGHT], res->texture.rightTaskTrue,
                 state->layout.arrow.RightReturnArrowPos, res->texture.helperSubWindow);
}

// --------------------------------------- WON SECTION ---------------------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->music.won);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->music.won);
        PlayMusicStream(res->music.intro);
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }

    StopMusicStream(res->music.monoV1);
    StopMusicStream(res->music.background);
    StopSound(res->sound.eatingCookie);
    StopSound(res->sound.ghostWriting);
    StopSound(res->sound.girlLaugh);
}

void DrawWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layout;

    char resultsText[50];
    sprintf(resultsText, "Tiempo Sobrevivido: %.2f seg", state->player.playerTotalTime);

    DrawText("BUEN TRABAJO!", layout->startAndLost.startTitlePos.x, layout->startAndLost.startTitlePos.y, layout->startAndLost.fontSizeTitle, WHITE);
    DrawText("Sobreviviste la noche", layout->startAndLost.startTitlePos.x, layout->startAndLost.instructionY, layout->startAndLost.fontSizeInstruction, WHITE);
    DrawText(resultsText, layout->startAndLost.startTitlePos.x, layout->startAndLost.gameResults, layout->startAndLost.fontSizeResults, YELLOW);
}

// --------------------------------------- LOST SECTION ---------------------------------------
void LogicLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->music.ending);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->music.ending);
        PlayMusicStream(res->music.intro);
        TriggerBlink(state, 2.2f, BLACK, 0.0f);
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }

    StopMusicStream(res->music.monoV1);
    StopMusicStream(res->music.background);
    StopSound(res->sound.eatingCookie);
    StopSound(res->sound.ghostWriting);
    StopSound(res->sound.girlLaugh);
}

void DrawLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layout;

    const char *deathReason = layout->startAndLost.currentGameOverMessage;

    char causeOfDeathText[100];
    sprintf(causeOfDeathText, "Causa de Muerte: %s", deathReason);

    char resultsText[50];
    sprintf(resultsText, "Tiempo Sobrevivido: %.2f seg", state->player.playerTotalTime);

    DrawText("JUEGO TERMINADO", layout->startAndLost.startTitlePos.x, layout->startAndLost.startTitlePos.y, layout->startAndLost.fontSizeTitle, WHITE);
    DrawText(causeOfDeathText, layout->startAndLost.startTitlePos.x, layout->startAndLost.instructionY, layout->startAndLost.fontSizeInstruction, WHITE);
    DrawText(resultsText, layout->startAndLost.startTitlePos.x, layout->startAndLost.gameResults, layout->startAndLost.fontSizeResults, YELLOW);
}
