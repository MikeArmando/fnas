#include "state.h"
#include <math.h>
#include <stdio.h>

static GameLogicFunction currentLogic = 0;
static GameDrawFunction currentDraw = 0;

Ts_GameState GameState_config(const Ts_resources *res)
{
    Ts_GameState state = {0};

    // Background Config
    float sx = (float)GetScreenWidth() / (float)res->classroomWindow.width;
    float sy = (float)GetScreenHeight() / (float)res->classroomWindow.height;
    state.scaleFit = fminf(sx, sy);
    state.backgroundPosFit = (Vector2){(GetScreenWidth() - res->classroomWindow.width * state.scaleFit) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * state.scaleFit) / 2.0f};

    // Tutorial Window
    state.tutorialWindowPos = (Vector2){300, 200};

    // Objects Config
    float arrowSize = 60.0f;
    float returnArrowSize = 90.0f;
    float helperSize = 90.0f;

    // Center Arrow
    state.scaleCenterArrow = arrowSize / (float)res->centerArrow.width;
    state.centerArrowPos = (Vector2){588, 500};
    // Left Arrow
    state.scaleLeftArrow = arrowSize / (float)res->leftArrow.width;
    state.leftArrowPos = (Vector2){122, 500};
    // Rigth Arrow
    state.scaleRightArrow = arrowSize / (float)res->rightArrow.width;
    state.rightArrowPos = (Vector2){1050, 500};
    // Return Arrow
    state.scaleReturnArrow = returnArrowSize / (float)res->centerArrow.width;
    state.returnArrowPos = (Vector2){588, 640};
    // Return right Arrow
    state.RightreturnRightArrowPos = (Vector2){200, 640};

    // Snail
    state.scaleHelper = helperSize / (float)res->helper.width;
    state.helperPos = (Vector2){50, 660};

    // Flashlight
    state.flashlightRec = (Rectangle){532, 640, 160, 160};

    // Colision Objects position
    state.centerArrowRec = (Rectangle){state.centerArrowPos.x, state.centerArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.leftArrowRec = (Rectangle){state.leftArrowPos.x, state.leftArrowPos.y, res->leftArrow.width * state.scaleLeftArrow, res->leftArrow.height * state.scaleLeftArrow};
    state.rightArrowRec = (Rectangle){state.rightArrowPos.x, state.rightArrowPos.y, res->rightArrow.width * state.scaleRightArrow, res->rightArrow.height * state.scaleRightArrow};
    state.returnArrowRec = (Rectangle){state.returnArrowPos.x, state.returnArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.RightreturnArrowRec = (Rectangle){state.RightreturnRightArrowPos.x, state.RightreturnRightArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.helperRec = (Rectangle){state.helperPos.x, state.helperPos.y, res->helper.width * state.scaleHelper, res->helper.height * state.scaleHelper};

    // MATH TASK
    // ------------------ POSITIONS FOR WINDOW ------------------
    float columnWidth = 150;
    float columnHeight = 60;
    float columnPosX = 920;
    float columnPosY = 390;
    float gapY = 76.0f;

    for (int i = 0; i < 4; i++)
    {
        state.leftMathAnswersRec[i] = (Rectangle){
            columnPosX,
            columnPosY + (i * gapY),
            columnWidth,
            columnHeight};
    }

    // ------------------ POSITIONS FOR CHALK BOARD ------------------
    float newColumnWidth = 200.0f;
    float newColumnHeight = 80.0f;
    float newColumnPosX = 800.0f;
    float newColumnPosY = 150.0f;
    float newSeparation = 15.0f;

    float newGapY = newColumnHeight + newSeparation;

    for (int i = 0; i < 4; i++)
    {
        state.centerMathAnswersRec[i] = (Rectangle){
            newColumnPosX,
            newColumnPosY + (i * newGapY), // Posición Y = Inicio + (índice * salto total)
            newColumnWidth,
            newColumnHeight};
    }

    // Timer Setup
    state.interval = 10.0f; // Seconds
    state.currentTime = 0.0f;

    // Monologue length
    state.monoLen = GetMusicTimeLength(res->monoV1);

    return state;
}

// ------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------
void LogicStartScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->introMusic);
    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->introMusic);
        PlayMusicStream(res->monoV1);

        ChangeGameState(LogicClassroom, DrawClassroom);
    }
}

void DrawStartScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("Five Nigths At School", 150, 200, 60, WHITE);
    DrawText("Presiona [ENTER] para comenzar . . .", 150, 270, 20, WHITE);
    DrawText("PRESENTADO POR:", 100, 650, 15, WHITE);
    DrawText("Montano Valencia Mike Armando", 100, 680, 15, WHITE);
    DrawText("Perez Aguirre Mextli Citlali", 100, 700, 15, WHITE);
}

// ------------------------------------------------------------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, state->leftArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicLeftTaskWindow, DrawLeftTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->centerArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicCenterTaskWindow, DrawCenterTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->rightArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperRec))
    {
        state->isHelpWindow = 1;
    }
    else
    {
        state->isHelpWindow = 0;
    }
}

void DrawClassroom(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->classroomWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);

    DrawTextureEx(res->centerArrow, state->centerArrowPos, 0.0f, state->scaleCenterArrow, WHITE);
    DrawTextureEx(res->leftArrow, state->leftArrowPos, 0.0f, state->scaleLeftArrow, WHITE);
    DrawTextureEx(res->rightArrow, state->rightArrowPos, 0.0f, state->scaleRightArrow, WHITE);
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }

    if (state->isLeftTaskTrue)
    {
        DrawTextureEx(res->leftIndicator, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    }
    if (state->isCenterTaskTrue)
    {
        DrawTextureEx(res->centerIndicator, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    }
    if (state->isRightTaskTrue)
    {
    }
}

// ------------------------------------------------------------------------------------------
void LogicCenterTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    if (state->isCenterTaskTrue)
    {
        ChangeGameState(LogicStartCenterTask, DrawStartCenterTask);
        return;
    }

    if (CheckCollisionPointRec(mousePos, state->returnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
}

void DrawCenterTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->centerTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
}

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->centerMathTask;

    if (CheckCollisionPointRec(mousePos, state->returnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->centerMathAnswersRec[i]) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            state->idleTime = 0.0f; // Restarts idleTimer

            if (i == currentTask->correctIndex)
            {
                PlaySound(res->writingOnBoard);
                currentTask->isActive = false;
                state->isCenterTaskTrue = 0;
                state->correctPoints++;
                ChangeGameState(LogicClassroom, DrawClassroom);
            }
            else
            {
                PlaySound(res->incorrect);
                state->incorrectPoints++;
            }
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperRec))
    {
        state->isHelpWindow = 1;
    }
    else
    {
        state->isHelpWindow = 0;
    }
}

void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->centerTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }

    Ts_MathTaskData *currentTask = &state->centerMathTask;

    char questionString[16];
    sprintf(questionString, "%d %c %d %s", currentTask->num1, currentTask->op, currentTask->num2, "= ?");
    DrawText(questionString, 250, 250, 70, LIGHTGRAY);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        DrawRectangleLinesEx(state->centerMathAnswersRec[i], 3, LIGHTGRAY);

        int textWidth = MeasureText(answersOnScreen, 60);

        int answerPosX = (int)(state->centerMathAnswersRec[i].x + (state->centerMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->centerMathAnswersRec[i].y + (state->centerMathAnswersRec[i].height / 2) - (60 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 60, LIGHTGRAY);
    }
}

// ------------------------------------------------------------------------------------------
void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (state->isLeftTaskTrue)
    {
        PlaySound(res->suspenseSound);
        ChangeGameState(LogicStartLeftTask, DrawStartLeftTask);
        return;
    }
    if (CheckCollisionPointRec(mousePos, state->returnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->helperRec))
    {
        state->isHelpWindow = 1;
    }
    else
    {
        state->isHelpWindow = 0;
    }
}

void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->leftTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    Ts_MathTaskData *currentTask = &state->leftMathTask;

    if (CheckCollisionPointRec(mousePos, state->returnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->leftMathAnswersRec[i]) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            state->idleTime = 0.0f; // Restarts idleTimer

            if (i == currentTask->correctIndex)
            {
                PlaySound(res->eatingCookie);
                currentTask->isActive = false;
                state->isLeftTaskTrue = 0;
                state->correctPoints++;
                ChangeGameState(LogicClassroom, DrawClassroom);
            }
            else
            {
                PlaySound(res->incorrect);
                state->incorrectPoints++;
            }
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperRec))
    {
        state->isHelpWindow = 1;
    }
    else
    {
        state->isHelpWindow = 0;
    }
}

void DrawStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->leftTaskTrue, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);

    Ts_MathTaskData *currentTask = &state->leftMathTask;

    char mathNum1[16];
    sprintf(mathNum1, "%d", currentTask->num1);
    int widthNum1 = MeasureText(mathNum1, 50);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", currentTask->op, currentTask->num2);
    int widthLine2 = MeasureText(mathNum2, 50);

    char mathQuestion[16] = "?";
    int widthQuestion = MeasureText(mathQuestion, 50);

    DrawText(mathNum1, 820 - widthNum1, 470, 50, BLACK);
    DrawText(mathNum2, 820 - widthLine2, 470 + 50, 50, BLACK);
    DrawText(mathQuestion, 820 - widthQuestion, 500 + (40 * 2), 50, BLACK);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, 40);

        DrawRectangleLinesEx(state->leftMathAnswersRec[i], 3, BLACK);

        int answerPosX = (int)(state->leftMathAnswersRec[i].x + (state->leftMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->leftMathAnswersRec[i].y + (state->leftMathAnswersRec[i].height / 2) - (40 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 40, BLACK);
    }

    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
}

// ------------------------------------------------------------------------------------------
void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, state->RightreturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->flashlightRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            ChangeGameState(LogicRightTaskWindowLight, DrawRightTaskWindowLigth);
        }
    }
}

void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->rightTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->RightreturnRightArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
}

void LogicRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, state->RightreturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->flashlightRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
        }
    }
}

void DrawRightTaskWindowLigth(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->rightTaskWindowLights, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->RightreturnRightArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
}

// ------------------------------------------------------------------------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    PlaySound(res->hasWon);
    if (IsKeyPressed(KEY_ENTER))
    {
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }
}

void DrawWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("Good job kid!", 150, 200, 60, WHITE);
    DrawText("You survived the night", 150, 265, 20, WHITE);
}

// ------------------------------------------------------------------------------------------
void LogicLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    StopMusicStream(res->monoV1);
    UpdateMusicStream(res->backgroundNoise);

    if (IsKeyPressed(KEY_ENTER))
    {
        state->idleTime = 0;
        StopMusicStream(res->backgroundNoise);
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }
}

void DrawLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("GAME OVER", 150, 200, 60, WHITE);
    DrawText("Cause of death: you suck at math bruah", 150, 265, 20, WHITE);
}

// ------------------------------------------------------------------------------------------
void playerIdleTimer(const Ts_resources *res, Ts_GameState *state)
{
    float deltatime = GetFrameTime();

    if (state->safeTime < 30.0f)
    {
        state->safeTime += deltatime;
        return;
    }

    state->idleTime += deltatime;

    if (state->idleTime >= 30.0f)
    {
        state->safeTime = 0.0f;
        state->idleTime = 0.0f;
        ChangeGameState(LogicLostScreen, DrawLostScreen);
    }
}

void StartTimerTask(const Ts_resources *res, Ts_GameState *state)
{
    if (state->safeTime > 18.0f)
    {
        float deltatime = GetFrameTime();
        state->currentTime += deltatime;
        if (state->currentTime >= state->interval)
        {
            state->currentTime -= state->interval;
            AssignTask(res, state);
        }
    }
}

void AssignTask(const Ts_resources *res, Ts_GameState *state)
{
    int taskSelecctor = GetRandomValue(0, 2);

    if (taskSelecctor == 0)
    {
        if (state->isLeftTaskTrue)
        {
            return;
        }
        Ts_MathTaskData *currentTask = &state->leftMathTask;
        leftMathTaskGenerator(currentTask);
        state->isLeftTaskTrue = 1;
        return;
    }
    if (taskSelecctor == 1)
    {
        if (state->isCenterTaskTrue)
        {
            return;
        }
        Ts_MathTaskData *currentTask = &state->centerMathTask;
        centerMathTaskGenerator(currentTask);
        PlaySound(res->writingOnBoard);
        state->isCenterTaskTrue = 1;
        return;
    }
    if (taskSelecctor == 2)
    {
        if (state->isRightTaskTrue)
        {
            return;
        }
        PlaySound(res->girlLaugh);
        state->isRightTaskTrue = 1;
        return;
    }
    return;
}

void leftMathTaskGenerator(Ts_MathTaskData *task)
{
    task->num1 = GetRandomValue(10, 200);
    task->num2 = GetRandomValue(10, 200);
    task->op = '+';
    task->correctAnswer = task->num1 + task->num2;
    task->isActive = true;

    task->correctIndex = GetRandomValue(0, 3);
    task->choicesList[task->correctIndex] = task->correctAnswer;

    for (int i = 0; i < 4; i++)
    {
        if (i == task->correctIndex)
        {
            continue;
        }

        int wrongAnswer;
        do
        {
            wrongAnswer = task->correctAnswer + GetRandomValue(-5, 5);
        } while (wrongAnswer == task->correctAnswer);

        task->choicesList[i] = wrongAnswer;
    }
    return;
}

void centerMathTaskGenerator(Ts_MathTaskData *task)
{
    task->num1 = GetRandomValue(0, 100);
    task->num2 = GetRandomValue(1, 100);
    task->op = 'x';
    task->correctAnswer = task->num1 * task->num2;
    task->isActive = true;

    task->correctIndex = GetRandomValue(0, 3);
    task->choicesList[task->correctIndex] = task->correctAnswer;

    for (int i = 0; i < 4; i++)
    {
        if (i == task->correctIndex)
        {
            continue;
        }

        int wrongAnswer;
        do
        {
            wrongAnswer = task->correctAnswer + GetRandomValue(-5, 5);
        } while (wrongAnswer == task->correctAnswer);

        task->choicesList[i] = wrongAnswer;
    }
    return;
}