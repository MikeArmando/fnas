#include "state.h"
#include <math.h>
#include <stdio.h>

static GameLogicFunction currentLogic = 0;
static GameDrawFunction currentDraw = 0;
static Ts_BlinkEffect sysBlink = {0};

Ts_GameState GameState_config(const Ts_resources *res)
{
    Ts_GameState state = {0};

    // initialize jumpScare config
    state.JumpscareConfig.jumpscareTimer = 0.0f;
    state.JumpscareConfig.jumpscareAudioPlayed = 0;

    // Background Config
    float sx = (float)GetScreenWidth() / (float)res->classroomWindow.width;
    float sy = (float)GetScreenHeight() / (float)res->classroomWindow.height;
    state.scaleFit = fminf(sx, sy);

    state.backgroundPosFit = (Vector2){(GetScreenWidth() - res->classroomWindow.width * state.scaleFit) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * state.scaleFit) / 2.0f};

    // Tutorial Window
    state.helperConfig.tutorialWindowPos = (Vector2){300, 200};

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
    state.helperConfig.scaleHelper = helperSize / (float)res->helper.width;
    state.helperConfig.helperPos = (Vector2){50, 660};

    // Flashlight
    state.flashlightRec = (Rectangle){532, 640, 160, 160};

    // Colision Objects position
    state.centerArrowRec = (Rectangle){state.centerArrowPos.x, state.centerArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.leftArrowRec = (Rectangle){state.leftArrowPos.x, state.leftArrowPos.y, res->leftArrow.width * state.scaleLeftArrow, res->leftArrow.height * state.scaleLeftArrow};
    state.rightArrowRec = (Rectangle){state.rightArrowPos.x, state.rightArrowPos.y, res->rightArrow.width * state.scaleRightArrow, res->rightArrow.height * state.scaleRightArrow};
    state.returnArrowRec = (Rectangle){state.returnArrowPos.x, state.returnArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.RightreturnArrowRec = (Rectangle){state.RightreturnRightArrowPos.x, state.RightreturnRightArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.helperConfig.helperRec = (Rectangle){state.helperConfig.helperPos.x, state.helperConfig.helperPos.y, res->helper.width * state.helperConfig.scaleHelper, res->helper.height * state.helperConfig.scaleHelper};

    // MATH TASK
    // ------------------ MATH OP POSITION LEFT SIDE ------------------
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

    // ------------------ MATH OP POSITION CENTER SIDE ------------------
    float centerColumnWidth = 200.0f;
    float centerColumnHeight = 80.0f;
    float centerColumnPosX = 800.0f;
    float centerColumnPosY = 150.0f;
    float centerSeparation = 15.0f;

    float centerGapY = centerColumnHeight + centerSeparation;

    for (int i = 0; i < 4; i++)
    {
        state.centerMathAnswersRec[i] = (Rectangle){
            centerColumnPosX,
            centerColumnPosY + (i * centerGapY), // Posición Y = Inicio + (índice * salto total)
            centerColumnWidth,
            centerColumnHeight};
    }

    // ------------------ MATH OP POSITION RIGHT SIDE ------------------
    float rightcolumnWidth = 150;
    float rightcolumnHeight = 60;
    float rightcolumnPosX = 1007;
    float rightcolumnPosY = 440;
    float rightgapY = 62.0f;

    for (int i = 0; i < 4; i++)
    {
        state.rightMathAnswersRec[i] = (Rectangle){
            rightcolumnPosX,
            rightcolumnPosY + (i * rightgapY),
            rightcolumnWidth,
            rightcolumnHeight};
    }
    // Timer Setup
    state.interval = 10.0f; // Seconds
    state.currentTime = 0.0f;

    // Monologue length
    state.monoLen = GetMusicTimeLength(res->monoV1);

    return state;
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
    UpdateMusicStream(res->intro);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->intro);
        PlayMusicStream(res->monoV1);
        PlayMusicStream(res->background);

        state->incorrectPoints = 0;
        state->correctPoints = 0;
        state->idleTime = 0.0f;
        state->safeTime = 0.0f;
        state->isLeftTaskTrue = 0;
        state->isCenterTaskTrue = 0;
        state->isRightTaskTrue = 0;

        TriggerBlink(0.6f, BLACK, 0.0f);
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

// --------------------------------------- CLASSROOM SECTION ---------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, state->leftArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicLeftTaskWindow, DrawLeftTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->centerArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicCenterTaskWindow, DrawCenterTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->rightArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperConfig.helperRec))
    {
        state->helperConfig.isHelpWindow = 1;
    }
    else
    {
        state->helperConfig.isHelpWindow = 0;
    }
}

void DrawClassroom(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->classroomWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->centerArrow, state->centerArrowPos, 0.0f, state->scaleCenterArrow, WHITE);
    DrawTextureEx(res->leftArrow, state->leftArrowPos, 0.0f, state->scaleLeftArrow, WHITE);
    DrawTextureEx(res->rightArrow, state->rightArrowPos, 0.0f, state->scaleRightArrow, WHITE);
    if (state->isLeftTaskTrue)
    {
        DrawTextureEx(res->leftIndicator, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    }
    if (state->isCenterTaskTrue)
    {
        DrawTextureEx(res->centerIndicator, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    }
    if (state->helperConfig.isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->helperConfig.tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
    DrawTextureEx(res->helper, state->helperConfig.helperPos, 0.0f, state->helperConfig.scaleHelper, WHITE);
}

// --------------------------------------- LEFT SECTION ---------------------------------------
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
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
}

void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->leftTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
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
            TriggerBlink(0.2f, BLACK, 0.0f);
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

                if (state->correctPoints >= 10)
                {
                    PlaySound(res->hasWon);
                    TriggerBlink(7.0f, BLACK, 0.0f);
                    ChangeGameState(LogicWonScreen, DrawWonScreen);
                    return;
                }
                TriggerBlink(0.2f, BLACK, 0.0f);
                ChangeGameState(LogicClassroom, DrawClassroom);
            }
            else
            {
                state->incorrectPoints++;

                if (state->incorrectPoints >= 5)
                {
                    PlayMusicStream(res->ending);
                    PlaySound(res->girlJumpscare);
                    TriggerJumpscare(state);
                    return;
                }
                TriggerBlink(0.1f, RED, 0.0f);
                PlaySound(res->incorrect);
            }
            return;
        }
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

    DrawText(mathNum1, 820 - widthNum1, 470, 50, BLACK);
    DrawText(mathNum2, 820 - widthLine2, 470 + 50, 50, BLACK);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, 40);

        int answerPosX = (int)(state->leftMathAnswersRec[i].x + (state->leftMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->leftMathAnswersRec[i].y + (state->leftMathAnswersRec[i].height / 2) - (40 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 40, BLACK);
    }

    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

// --------------------------------------- CENTER SECTION ---------------------------------------
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
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
}

void DrawCenterTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->centerTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->centerMathTask;

    if (CheckCollisionPointRec(mousePos, state->returnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(0.2f, BLACK, 0.0f);
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

                if (state->correctPoints >= 10)
                {
                    PlaySound(res->hasWon);
                    TriggerBlink(7.0f, BLACK, 0.0f);
                    ChangeGameState(LogicWonScreen, DrawWonScreen);
                    return;
                }
                TriggerBlink(0.2f, BLACK, 0.0f);
                ChangeGameState(LogicClassroom, DrawClassroom);
            }
            else
            {
                state->incorrectPoints++;

                if (state->incorrectPoints >= 5)
                {
                    PlayMusicStream(res->ending);
                    PlaySound(res->girlJumpscare);
                    TriggerJumpscare(state);
                    return;
                }
                TriggerBlink(0.1f, RED, 0.0f);
                PlaySound(res->incorrect);
            }
            return;
        }
    }
}

void DrawStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->centerTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);

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

// --------------------------------------- RIGHT SECTION ---------------------------------------
void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, state->RightreturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->flashlightRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicRightTaskWindowLight, DrawRightTaskWindowLight);
        }
    }
}

void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->rightTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->RightreturnRightArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

void LogicRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, state->RightreturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
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

    if (state->isRightTaskTrue)
    {
        ChangeGameState(LogicStartRightTask, DrawStartRightTask);
        return;
    }
}

void DrawRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->rightTaskWindowLights, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->RightreturnRightArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->rightMathTask;

    if (CheckCollisionPointRec(mousePos, state->RightreturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
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

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->rightMathAnswersRec[i]) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            state->idleTime = 0.0f;

            if (i == currentTask->correctIndex)
            {
                PlaySound(res->doorOpening);
                PlaySound(res->girlLaugh);
                currentTask->isActive = false;
                state->isRightTaskTrue = 0;
                state->correctPoints++;

                if (state->correctPoints >= 10)
                {
                    PlaySound(res->hasWon);
                    TriggerBlink(7.0f, BLACK, 0.0f);
                    ChangeGameState(LogicWonScreen, DrawWonScreen);
                    return;
                }
                TriggerBlink(0.2f, BLACK, 0.0f);
                ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
            }
            else
            {
                state->incorrectPoints++;

                if (state->incorrectPoints >= 5)
                {
                    PlayMusicStream(res->ending);
                    PlaySound(res->girlJumpscare);
                    TriggerJumpscare(state);
                    return;
                }
                PlaySound(res->incorrect);
                TriggerBlink(1.0f, RED, 0.0f);
            }
            return;
        }
    }
}

void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->rightTaskTrue, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);

    Ts_MathTaskData *currentTask = &state->rightMathTask;

    char mathNum1[16];
    sprintf(mathNum1, "%d", currentTask->num1);
    int widthNum1 = MeasureText(mathNum1, 50);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", currentTask->op, currentTask->num2);
    int widthLine2 = MeasureText(mathNum2, 50);

    DrawText(mathNum1, 950 - widthNum1, 470, 50, BLACK);
    DrawText(mathNum2, 950 - widthLine2, 470 + 50, 50, BLACK);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, 35);

        int answerPosX = (int)(state->rightMathAnswersRec[i].x + (state->rightMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->rightMathAnswersRec[i].y + (state->rightMathAnswersRec[i].height / 2) - (35 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 35, BLACK);
    }

    DrawTextureEx(res->returnArrow, state->RightreturnRightArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

// --------------------------------------- WON SECTION ---------------------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }

    StopMusicStream(res->monoV1);
    StopMusicStream(res->background);
    StopSound(res->eatingCookie);
    StopSound(res->writingOnBoard);
    StopSound(res->girlLaugh);
}

void DrawWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("Good job kid!", 150, 200, 60, WHITE);
    DrawText("You survived the night", 150, 265, 20, WHITE);
}

// --------------------------------------- LOST SECTION ---------------------------------------
void LogicLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->ending);

    if (IsKeyPressed(KEY_ENTER))
    {
        state->idleTime = 0;
        StopMusicStream(res->ending);
        PlayMusicStream(res->intro);
        TriggerBlink(2.2f, BLACK, 0.0f);
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

// --------------------------------------- TIME SECTION ---------------------------------------
void playerIdleTimer(const Ts_resources *res, Ts_GameState *state)
{
    float deltatime = GetFrameTime();
    state->idleTime += deltatime;

    if (state->safeTime < 30.0f)
    {
        state->safeTime += deltatime;
        return;
    }

    if (state->idleTime >= 80.0f)
    {
        state->safeTime = 0.0f;
        state->idleTime = 0.0f;
        TriggerJumpscare(state);
    }
}

// --------------------------------------- ASSIGN TASK SECTION ---------------------------------------
void StartTimerTask(const Ts_resources *res, Ts_GameState *state)
{
    if (state->safeTime > 10.0f)
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
        Ts_MathTaskData *currentTask = &state->rightMathTask;
        rightMathTaskGenerator(currentTask);
        state->isRightTaskTrue = 1;
        return;
    }
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

// --------------------------- BLINK EFFECT ---------------------------
void TriggerBlink(float duration, Color color, float frequency)
{
    sysBlink.active = true;
    sysBlink.duration = duration;
    sysBlink.currentTimer = duration;
    sysBlink.color = color;
    sysBlink.frequency = frequency;
}

void UpdateBlink(void)
{
    if (sysBlink.active)
    {
        sysBlink.currentTimer -= GetFrameTime();

        if (sysBlink.currentTimer <= 0)
        {
            sysBlink.active = false;
            sysBlink.currentTimer = 0;
        }
    }
}

void DrawBlink(void)
{
    if (sysBlink.active)
    {
        if (sysBlink.frequency <= 0.0f)
        {
            // A. LÓGICA DE DESVANECIMIENTO (FADE OUT)
            // Calculamos el alpha de 1.0 a 0.0
            float alpha = sysBlink.currentTimer / sysBlink.duration;
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(sysBlink.color, alpha));
            return;
        }

        float timeElapsed = sysBlink.duration - sysBlink.currentTimer;
        float cycleInterval = 1.0f / sysBlink.frequency;
        float cycleTime = fmodf(timeElapsed, cycleInterval);

        if (cycleTime < (cycleInterval / 2.0f))
        {
            // Mantenemos el alpha constante al 50%
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(sysBlink.color, 0.5f));
        }
    }
}

// --------------------------- JUMPSCARE EFFECT ---------------------------
void TriggerJumpscare(Ts_GameState *state)
{
    state->JumpscareConfig.lastDrawFunction = GetCurrentDraw();

    state->JumpscareConfig.jumpscareTimer = 0.0f;
    state->JumpscareConfig.jumpscareAudioPlayed = 0;

    ChangeGameState(LogicJumpscare, DrawJumpscare);
}

void LogicJumpscare(const Ts_resources *res, Ts_GameState *state)
{
    float deltatime = GetFrameTime();
    state->JumpscareConfig.jumpscareTimer += deltatime;

    if (!state->JumpscareConfig.jumpscareAudioPlayed)
    {
        StopMusicStream(res->monoV1);
        StopMusicStream(res->background);
        StopSound(res->eatingCookie);
        StopSound(res->writingOnBoard);
        StopSound(res->girlLaugh);

        state->JumpscareConfig.jumpscareAudioPlayed = 1;
    }

    if (state->JumpscareConfig.jumpscareTimer >= 1.0f)
    {
        TriggerBlink(6.0f, BLACK, 0.0f);
        PlayMusicStream(res->ending);
        ChangeGameState(LogicLostScreen, DrawLostScreen);
    }
}

void DrawJumpscare(const Ts_resources *res, Ts_GameState *state)
{
    if (state->JumpscareConfig.jumpscareTimer < 1.7f)
    {
        if (GetRandomValue(0, 10) > 4)
        {
            ClearBackground(BLACK);
        }
        else
        {
            state->JumpscareConfig.lastDrawFunction(res, state);
        }
    }
    else
    {
        ClearBackground(BLACK);
    }
}