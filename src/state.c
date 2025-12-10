#include <stdio.h>
#include "state.h"
#include <math.h>
#include <float.h>

#define BASE_WIDTH 1250.0f
#define BASE_HEIGHT 750.0f

static GameLogicFunction currentLogic = 0;
static GameDrawFunction currentDraw = 0;
static Ts_BlinkEffect Blink = {0};

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
        state->task.isLeftTaskTrue = 0;
        state->task.isCenterTaskTrue = 0;
        state->task.isRightTaskTrue = 0;
        state->layout.tutorial.silenceBtnCurrentIndex = 1;

        TriggerBlink(0.6f, BLACK, 0.0f);
        ChangeGameState(LogicClassroom, DrawClassroom);
    }

    if (CheckCollisionPointRec(mousePos, state->layout.tutorial.tutorialBtnRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(0.2f, BLACK, 0.0F);
            ChangeGameState(LogicTutorial, DrawTutorial);
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
            TriggerBlink(0.2f, BLACK, 0.0F);
            PlaySound(res->sound.arrowClick);
            if (state->layout.tutorial.currentPageIndex < state->layout.tutorial.maxPages - 1)
            {
                state->layout.tutorial.currentPageIndex++;
            }
            else
            {
                state->layout.tutorial.currentPageIndex = 0;
                ChangeGameState(LogicStartScreen, DrawStartScreen);
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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

    if (state->task.isLeftTaskTrue)
    {
        DrawTextureEx(res->texture.leftIndicator, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    }
    if (state->task.isCenterTaskTrue)
    {
        DrawTextureEx(res->texture.centerIndicator, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    }
    if (state->helper.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperGeneralWindow, state->helper.tutorialWindowPos, 0.0f, state->helper.scaleHelperWindow, WHITE);
    }
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);
}

// --------------------------------------- LEFT SECTION ---------------------------------------
void LogicLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (state->task.isLeftTaskTrue)
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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

    Ts_MathTaskData *currentTask = &state->leftMathTask;

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.returnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->layout.noseRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.CatMeow);
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

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layout.math.bookMathAnswersRec[i]))
        {
            isMouseOverClickable = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                HandleMathAnswer(res, state, currentTask, i, &state->task.isLeftTaskTrue);
                return;
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

void DrawStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Ts_MathTaskData *currentTask = &state->leftMathTask;
    Ts_Layout *layout = &state->layout;

    DrawTextureEx(res->texture.leftTaskTrue, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->arrow.returnArrowPos, 0.0f, layout->arrow.scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);

    // ------------------- DRAW QUESTION -------------------
    char mathNum1[16];
    sprintf(mathNum1, "%d", currentTask->num1);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", currentTask->op, currentTask->num2);

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

    // ------------------- DRAW ANSWERS -------------------
    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, layout->math.bookFontSizeAnswer);

        int answerPosX = (int)(layout->math.bookMathAnswersRec[i].x + (layout->math.bookMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(layout->math.bookMathAnswersRec[i].y + (layout->math.bookMathAnswersRec[i].height / 2) - (layout->math.bookFontSizeAnswer / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, layout->math.bookFontSizeAnswer, BLACK);
    }

    if (state->helper.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperSumWindow, state->helper.tutorialWindowPos, 0.0f, state->helper.scaleHelperWindow, WHITE);
    }
}

// --------------------------------------- CENTER SECTION ---------------------------------------
void LogicCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (state->task.isCenterTaskTrue)
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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
    Ts_MathTaskData *currentTask = &state->centerMathTask;

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.returnArrowRec))
    {
        isMouseOverClickable = 1;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
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

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layout.math.chalkboardMathAnswersRec[i]))
        {
            isMouseOverClickable = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                HandleMathAnswer(res, state, currentTask, i, &state->task.isCenterTaskTrue);
                return;
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

void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Ts_MathTaskData *currentTask = &state->centerMathTask;
    Ts_Layout *layout = &state->layout;

    DrawTextureEx(res->texture.centerTaskWindow, state->layout.positionAbsolute, 0.0f, state->layout.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->arrow.returnArrowPos, 0.0f, layout->arrow.scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);

    char questionStr[16];
    sprintf(questionStr, "%d %c %d %s", currentTask->num1, currentTask->op, currentTask->num2, "= ?");
    DrawText(questionStr, layout->math.chalkboardMathQuestionPos.x, layout->math.chalkboardMathQuestionPos.y, layout->math.chalkboardFontSizeQuestion, LIGHTGRAY);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        DrawRectangleLinesEx(layout->math.chalkboardMathAnswersRec[i], 3, LIGHTGRAY);

        int textWidth = MeasureText(answersOnScreen, layout->math.chalkboardFontSizeAnswer);

        int answerPosX = (int)(layout->math.chalkboardMathAnswersRec[i].x + (layout->math.chalkboardMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(layout->math.chalkboardMathAnswersRec[i].y + (layout->math.chalkboardMathAnswersRec[i].height / 2) - (layout->math.chalkboardFontSizeAnswer / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, layout->math.chalkboardFontSizeAnswer, LIGHTGRAY);
    }
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->layout.flashlightRec))
    {
        isMouseOverClickable = 1;

        if (state->task.isRightTaskTrue)
        {
            PlaySound(res->sound.girlSuspense);
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            ChangeGameState(LogicRightTask, DrawRightTask);
        }
    }

    if (state->task.isRightTaskTrue)
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
    Ts_MathTaskData *currentTask = &state->rightMathTask;

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layout.arrow.RightReturnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
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
            ChangeGameState(LogicRightTask, DrawRightTask);
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

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layout.math.bookMathAnswersRec[i]))
        {
            isMouseOverClickable = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                HandleMathAnswer(res, state, currentTask, i, &state->task.isRightTaskTrue);
                return;
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

void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Ts_MathTaskData *currentTask = &state->rightMathTask;
    Ts_Layout *layout = &state->layout;

    DrawTextureEx(res->texture.rightTaskTrue, layout->positionAbsolute, 0.0f, layout->scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->arrow.RightReturnArrowPos, 0.0f, layout->arrow.scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helper.helperPos, 0.0f, state->helper.scaleHelper, WHITE);

    // ------------------- DRAW QUESTION -------------------
    char mathNum1[16];
    sprintf(mathNum1, "%d", currentTask->num1);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", currentTask->op, currentTask->num2);

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

    // ------------------- DRAW ANSWERS -------------------
    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, layout->math.bookFontSizeAnswer);

        int answerPosX = (int)(layout->math.bookMathAnswersRec[i].x + (layout->math.bookMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(layout->math.bookMathAnswersRec[i].y + (layout->math.bookMathAnswersRec[i].height / 2) - (layout->math.bookFontSizeAnswer / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, layout->math.bookFontSizeAnswer, BLACK);
    }
    if (state->helper.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperSubWindow, state->helper.tutorialWindowPos, 0.0f, state->helper.scaleHelperWindow, WHITE);
    }
}

// --------------------------------------- WON SECTION ---------------------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->music.won);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->music.won);
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
        TriggerBlink(2.2f, BLACK, 0.0f);
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

// --------------------------------------- TASK HANDLE ---------------------------------------
void HandleMathAnswer(const Ts_resources *res, Ts_GameState *state, Ts_MathTaskData *currentTask, int answerIndex, int *taskFlag)
{
    state->player.idleTime = 0.0f;

    if (answerIndex == currentTask->correctIndex)
    {
        currentTask->isActive = false;
        *taskFlag = 0;

        state->player.correctPoints++;

        if (state->player.correctPoints >= state->task.maxCorrectPoints)
        {
            PlaySound(res->sound.hasWon);
            PlayMusicStream(res->music.won);
            TriggerBlink(7.0f, BLACK, 0.0f);

            SaveGameResults(state, 1);
            state->player.bestTime = LoadBestTime();
            ChangeGameState(LogicWonScreen, DrawWonScreen);
            return;
        }

        PlaySound(*currentTask->successSound);
        TriggerBlink(0.2f, BLACK, 0.0f);
        ChangeGameState(currentTask->returnLogic, currentTask->returnDraw);
    }
    else
    {
        state->player.incorrectPoints++;

        if (state->player.incorrectPoints >= state->task.maxIncorrectPoints)
        {
            state->layout.startAndLost.currentGameOverMessage = currentTask->jumpscareMessage;
            PlayMusicStream(res->music.ending);
            PlaySound(res->sound.jumpscare);
            TriggerJumpscare(state);
            return;
        }

        TriggerBlink(0.1f, RED, 0.0f);
        PlaySound(res->sound.incorrect);
    }
}

// --------------------------------------- GAME RESULTS ---------------------------------------
void SaveGameResults(Ts_GameState *state, int playerWon)
{
    FILE *textFile;
    textFile = fopen("resultado.txt", "a");

    fprintf(textFile, "Estado: %s \n", playerWon ? "GANASTE" : "PERDISTE");
    fprintf(textFile, "Tiempo: %.2f segundos\n", state->player.playerTotalTime);
    fprintf(textFile, "Respuestas correctas: %d\n", state->player.correctPoints);
    fprintf(textFile, "Respuestas incorrectas: %d\n\n", state->player.incorrectPoints);
    fclose(textFile);

    if (playerWon == 1)
    {
        FILE *binFile = fopen("resultado.dat", "ab");

        fwrite(&playerWon, sizeof(int), 1, binFile);
        fwrite(&state->player.playerTotalTime, sizeof(float), 1, binFile);
        fwrite(&state->player.correctPoints, sizeof(float), 1, binFile);
        fwrite(&state->player.incorrectPoints, sizeof(float), 1, binFile);
        fclose(binFile);
    }
}

float LoadBestTime()
{
    FILE *binFile;
    binFile = fopen("resultado.dat", "rb");

    if (binFile == NULL)
    {
        return 0.0f;
    }

    float minTime = FLT_MAX;
    int foundWin = 0;

    int tempWon;
    float tempTime;
    int tempIncorrect;
    int tempCorrect;

    while (fread(&tempWon, sizeof(int), 1, binFile) == 1)
    {
        fread(&tempTime, sizeof(float), 1, binFile);
        fread(&tempIncorrect, sizeof(int), 1, binFile);
        fread(&tempCorrect, sizeof(int), 1, binFile);

        if (tempTime < minTime)
        {
            minTime = tempTime;
            foundWin = 1;
        }
    }

    fclose(binFile);

    if (foundWin)
    {
        return minTime;
    }
    else
    {
        return 0.0f;
    }
}

// --------------------------------------- PLAYER TIME ---------------------------------------
void playerIdleTimer(const Ts_resources *res, Ts_GameState *state)
{
    float deltatime = GetFrameTime();

    state->player.playerTotalTime += deltatime;

    if (state->player.safeTime < 55.0f)
    {
        state->player.safeTime += deltatime;
        return;
    }

    state->player.idleTime += deltatime;
    if (state->player.idleTime >= 60.0f)
    {
        state->player.safeTime = 0.0f;
        state->player.idleTime = 0.0f;
        PlaySound(res->sound.jumpscare);
        TriggerJumpscare(state);
    }
}

// --------------------------------------- ASSIGN TASK SECTION ---------------------------------------
void StartTimerTask(const Ts_resources *res, Ts_GameState *state)
{
    if (state->player.safeTime < 52.0f)
    {
        return;
    }

    float deltatime = GetFrameTime();
    state->task.currentTime += deltatime;

    if (state->task.currentTime >= state->task.interval)
    {
        state->task.currentTime -= state->task.interval;
        state->task.interval = (float)GetRandomValue(5, 10);
        AssignTask(res, state);
    }
}

void AssignTask(const Ts_resources *res, Ts_GameState *state)
{
    int roll = GetRandomValue(0, 99);

    int limitLeft = state->task.chanceLeft;
    int limitCenter = state->task.chanceLeft + state->task.chanceCenter;

    if (roll < limitLeft)
    {
        if (state->task.isLeftTaskTrue)
        {
            return;
        }
        Ts_MathTaskData *currentTask = &state->leftMathTask;
        leftMathTaskGenerator(currentTask);
        PlaySound(res->sound.knockingOnWindow);
        state->task.isLeftTaskTrue = 1;
        return;
    }
    if (roll < limitCenter)
    {
        if (state->task.isCenterTaskTrue)
        {
            return;
        }
        Ts_MathTaskData *currentTask = &state->centerMathTask;
        centerMathTaskGenerator(currentTask);
        PlaySound(res->sound.ghostWriting);
        state->task.isCenterTaskTrue = 1;
        return;
    }

    if (state->task.isRightTaskTrue)
    {
        return;
    }
    Ts_MathTaskData *currentTask = &state->rightMathTask;
    rightMathTaskGenerator(currentTask);
    state->task.isRightTaskTrue = 1;
}

void leftMathTaskGenerator(Ts_MathTaskData *task)
{
    task->num1 = GetRandomValue(0, 100);
    task->num2 = GetRandomValue(1, 100);
    task->op = '+';
    task->correctAnswer = task->num1 + task->num2;
    task->isActive = true;

    task->correctIndex = GetRandomValue(0, 3);
    task->choicesList[task->correctIndex] = task->correctAnswer;

    assignWrongChoices(task);
}

void centerMathTaskGenerator(Ts_MathTaskData *task)
{
    task->num1 = GetRandomValue(1, 10);
    task->num2 = GetRandomValue(0, 10);
    task->op = 'x';
    task->correctAnswer = task->num1 * task->num2;
    task->isActive = true;

    task->correctIndex = GetRandomValue(0, 3);
    task->choicesList[task->correctIndex] = task->correctAnswer;

    assignWrongChoices(task);
}

void rightMathTaskGenerator(Ts_MathTaskData *task)
{
    task->num1 = GetRandomValue(0, 100);
    task->num2 = GetRandomValue(1, 100);
    task->op = '-';
    task->correctAnswer = task->num1 - task->num2;
    task->isActive = true;

    task->correctIndex = GetRandomValue(0, 3);
    task->choicesList[task->correctIndex] = task->correctAnswer;

    assignWrongChoices(task);
}

void assignWrongChoices(Ts_MathTaskData *task)
{
    for (int i = 0; i < 4; i++)
    {
        if (i == task->correctIndex)
        {
            continue;
        }

        int wrongAnswer;
        bool isDuplicate;

        do
        {
            wrongAnswer = task->correctAnswer + GetRandomValue(-5, 5);
            isDuplicate = false;

            for (int j = 0; j < i; j++)
            {
                if (task->choicesList[j] == wrongAnswer)
                {
                    isDuplicate = true;
                    break;
                }
            }

        } while (isDuplicate || wrongAnswer == task->correctAnswer);

        task->choicesList[i] = wrongAnswer;
    }
}

// --------------------------------------- BLINK EFFECT ---------------------------------------
void TriggerBlink(float duration, Color color, float frequency)
{
    Blink.active = true;
    Blink.duration = duration;
    Blink.currentTimer = duration;
    Blink.color = color;
    Blink.frequency = frequency;
}

void UpdateBlink(void)
{
    if (Blink.active)
    {
        Blink.currentTimer -= GetFrameTime();

        if (Blink.currentTimer <= 0)
        {
            Blink.active = false;
            Blink.currentTimer = 0;
        }
    }
}

void DrawBlink(const Ts_GameState *state)
{
    if (Blink.active)
    {
        if (Blink.frequency <= 0.0f)
        {
            float alpha = Blink.currentTimer / Blink.duration;

            DrawRectangleRec(state->blinkState.blinkRec, Fade(Blink.color, alpha));
            return;
        }

        float timeElapsed = Blink.duration - Blink.currentTimer;
        float cycleInterval = 1.0f / Blink.frequency;
        float cycleTime = fmodf(timeElapsed, cycleInterval);

        if (cycleTime < (cycleInterval / 2.0f))
        {
            DrawRectangleRec(state->blinkState.blinkRec, Fade(Blink.color, 0.5f));
        }
    }
}

// --------------------------------------- JUMPSCARE EFFECT ---------------------------------------
void TriggerJumpscare(Ts_GameState *state)
{
    state->Jumpscare.lastDrawFunction = GetCurrentDraw();

    state->Jumpscare.jumpscareTimer = 0.0f;
    state->Jumpscare.jumpscareAudioPlayed = 0;

    ChangeGameState(LogicJumpscare, DrawJumpscare);
}

void LogicJumpscare(const Ts_resources *res, Ts_GameState *state)
{
    float deltatime = GetFrameTime();
    state->Jumpscare.jumpscareTimer += deltatime;

    if (!state->Jumpscare.jumpscareAudioPlayed)
    {
        StopMusicStream(res->music.monoV1);
        StopMusicStream(res->music.background);
        StopSound(res->sound.eatingCookie);
        StopSound(res->sound.ghostWriting);
        StopSound(res->sound.girlLaugh);

        state->Jumpscare.jumpscareAudioPlayed = 1;
    }

    if (state->Jumpscare.jumpscareTimer >= 1.0f)
    {
        TriggerBlink(6.0f, BLACK, 0.0f);
        PlayMusicStream(res->music.ending);
        SaveGameResults(state, 0);
        ChangeGameState(LogicLostScreen, DrawLostScreen);
    }
}

void DrawJumpscare(const Ts_resources *res, Ts_GameState *state)
{
    if (state->Jumpscare.jumpscareTimer < 1.7f)
    {
        if (GetRandomValue(0, 10) > 4)
        {
            ClearBackground(BLACK);
        }
        else
        {
            state->Jumpscare.lastDrawFunction(res, state);
        }
    }
    else
    {
        ClearBackground(BLACK);
    }
}