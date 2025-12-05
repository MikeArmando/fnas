#include "state.h"
#include <math.h>
#include <stdio.h>

static GameLogicFunction currentLogic = 0;
static GameDrawFunction currentDraw = 0;
static Ts_BlinkEffect sysBlink = {0};

Ts_GameState GameStateConfig(const Ts_resources *res)
{
    Ts_GameState state = {0};

    state.maxCorrectPoints = 15;
    state.maxIncorrectPoints = 5;

    // Task Chances
    state.chanceLeft = 30;
    state.chanceCenter = 50;
    state.chanceRight = 20;

    // Task Timer
    state.interval = 8.0f;
    state.currentTime = 0.0f;

    // initialize jumpScare config
    state.JumpscareConfig.jumpscareTimer = 0.0f;
    state.JumpscareConfig.jumpscareAudioPlayed = 0;

    // Monologue length
    state.monoLen = GetMusicTimeLength(res->monoV1);

    ResourcesLayout(&state, res);
    return state;
}

Ts_GameState ResourcesLayout(Ts_GameState *state, const Ts_resources *res)
{
    // ------------------ BACKGROUND ------------------
    float sx = (float)GetScreenWidth() / (float)res->classroomWindow.width;
    float sy = (float)GetScreenHeight() / (float)res->classroomWindow.height;
    state->layoutConfig.scaleFit = fminf(sx, sy);

    state->layoutConfig.backgroundPosFit = (Vector2){(GetScreenWidth() - res->classroomWindow.width * state->layoutConfig.scaleFit) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * state->layoutConfig.scaleFit) / 2.0f};

    // ------------------ OBJECTS ------------------
    state->helperConfig.tutorialWindowPos = (Vector2){300, 200};

    // Objects Config
    float arrowSize = 60.0f;
    float returnArrowSize = 90.0f;
    float helperSize = 90.0f;

    // Arrow Scale
    state->layoutConfig.scaleArrow = arrowSize / (float)res->centerArrow.width;
    state->layoutConfig.scaleReturnArrow = returnArrowSize / (float)res->centerArrow.width;

    // Arrow Position
    state->layoutConfig.centerArrowPos = (Vector2){588, 500};
    state->layoutConfig.leftArrowPos = (Vector2){122, 500};
    state->layoutConfig.rightArrowPos = (Vector2){1050, 500};
    state->layoutConfig.returnArrowPos = (Vector2){588, 640};
    state->layoutConfig.RightTaskReturnArrowPos = (Vector2){200, 640};

    // Snail
    state->helperConfig.scaleHelper = helperSize / (float)res->helper.width;
    state->helperConfig.helperPos = (Vector2){50, 660};

    // Flashlight
    state->layoutConfig.flashlightRec = (Rectangle){532, 640, 160, 160};

    // ------------------ COLISION OBJECTS POSITIONS ------------------
    state->layoutConfig.centerArrowRec = (Rectangle){state->layoutConfig.centerArrowPos.x, state->layoutConfig.centerArrowPos.y, res->centerArrow.width * state->layoutConfig.scaleArrow, res->centerArrow.height * state->layoutConfig.scaleArrow};
    state->layoutConfig.leftArrowRec = (Rectangle){state->layoutConfig.leftArrowPos.x, state->layoutConfig.leftArrowPos.y, res->leftArrow.width * state->layoutConfig.scaleArrow, res->leftArrow.height * state->layoutConfig.scaleArrow};
    state->layoutConfig.rightArrowRec = (Rectangle){state->layoutConfig.rightArrowPos.x, state->layoutConfig.rightArrowPos.y, res->rightArrow.width * state->layoutConfig.scaleArrow, res->rightArrow.height * state->layoutConfig.scaleArrow};
    state->layoutConfig.returnArrowRec = (Rectangle){state->layoutConfig.returnArrowPos.x, state->layoutConfig.returnArrowPos.y, res->centerArrow.width * state->layoutConfig.scaleArrow, res->centerArrow.height * state->layoutConfig.scaleArrow};
    state->layoutConfig.RightTaskReturnArrowRec = (Rectangle){state->layoutConfig.RightTaskReturnArrowPos.x, state->layoutConfig.RightTaskReturnArrowPos.y, res->centerArrow.width * state->layoutConfig.scaleArrow, res->centerArrow.height * state->layoutConfig.scaleArrow};
    state->helperConfig.helperRec = (Rectangle){state->helperConfig.helperPos.x, state->helperConfig.helperPos.y, res->helper.width * state->helperConfig.scaleHelper, res->helper.height * state->helperConfig.scaleHelper};

    // ------------------ MATH OP POSITION LEFT SIDE ------------------
    float columnWidth = 150;
    float columnHeight = 60;
    float columnPosX = 920;
    float columnPosY = 390;
    float gapY = 76.0f;

    for (int i = 0; i < 4; i++)
    {
        state->layoutConfig.leftMathAnswersRec[i] = (Rectangle){
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
        state->layoutConfig.centerMathAnswersRec[i] = (Rectangle){
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
        state->layoutConfig.rightMathAnswersRec[i] = (Rectangle){
            rightcolumnPosX,
            rightcolumnPosY + (i * rightgapY),
            rightcolumnWidth,
            rightcolumnHeight};
    }
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

        state->player.playerTotalTime = 0;
        state->player.incorrectPoints = 0;
        state->player.correctPoints = 0;
        state->player.idleTime = 0.0f;
        state->player.safeTime = 0.0f;
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
    DrawText("Five Nigths At School", 100, 200, 60, WHITE);
    DrawText("Presiona [ENTER] para comenzar . . .", 100, 270, 20, WHITE);
    DrawText("PRESENTADO POR:", 100, 650, 15, WHITE);
    DrawText("Perez Aguirre Mextli Citlali - Diseño y Arte", 100, 680, 15, WHITE);
    DrawText("Montano Valencia Mike Armando - Desarrollo de Software", 100, 700, 15, WHITE);
}

// --------------------------------------- CLASSROOM SECTION ---------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.leftArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicLeftTaskWindow, DrawLeftTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.centerArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicCenterTaskWindow, DrawCenterTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.rightArrowRec))
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
    DrawTextureEx(res->classroomWindow, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->centerArrow, state->layoutConfig.centerArrowPos, 0.0f, state->layoutConfig.scaleArrow, WHITE);
    DrawTextureEx(res->leftArrow, state->layoutConfig.leftArrowPos, 0.0f, state->layoutConfig.scaleArrow, WHITE);
    DrawTextureEx(res->rightArrow, state->layoutConfig.rightArrowPos, 0.0f, state->layoutConfig.scaleArrow, WHITE);
    if (state->isLeftTaskTrue)
    {
        DrawTextureEx(res->leftIndicator, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    }
    if (state->isCenterTaskTrue)
    {
        DrawTextureEx(res->centerIndicator, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
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
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
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
    DrawTextureEx(res->leftTaskWindow, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->layoutConfig.returnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->leftMathTask;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
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
        if (CheckCollisionPointRec(mousePos, state->layoutConfig.leftMathAnswersRec[i]) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            state->player.idleTime = 0.0f;

            if (i == currentTask->correctIndex)
            {
                currentTask->isActive = false;
                state->isLeftTaskTrue = 0;
                state->player.correctPoints++;

                if (state->player.correctPoints >= state->maxCorrectPoints)
                {
                    PlaySound(res->hasWon);
                    PlayMusicStream(res->won);
                    TriggerBlink(7.0f, BLACK, 0.0f);
                    SaveGameResults(state, 1);
                    ChangeGameState(LogicWonScreen, DrawWonScreen);
                    return;
                }
                PlaySound(res->eatingCookie);
                TriggerBlink(0.2f, BLACK, 0.0f);
                ChangeGameState(LogicClassroom, DrawClassroom);
            }
            else
            {
                state->player.incorrectPoints++;

                if (state->player.incorrectPoints >= state->maxIncorrectPoints)
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
    DrawTextureEx(res->leftTaskTrue, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);

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

        int answerPosX = (int)(state->layoutConfig.leftMathAnswersRec[i].x + (state->layoutConfig.leftMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->layoutConfig.leftMathAnswersRec[i].y + (state->layoutConfig.leftMathAnswersRec[i].height / 2) - (40 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 40, BLACK);
    }

    DrawTextureEx(res->returnArrow, state->layoutConfig.returnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
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

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
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
    DrawTextureEx(res->centerTaskWindow, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->layoutConfig.returnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->centerMathTask;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
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
        if (CheckCollisionPointRec(mousePos, state->layoutConfig.centerMathAnswersRec[i]) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            state->player.idleTime = 0.0f;

            if (i == currentTask->correctIndex)
            {
                currentTask->isActive = false;
                state->isCenterTaskTrue = 0;
                state->player.correctPoints++;

                if (state->player.correctPoints >= state->maxCorrectPoints)
                {
                    PlaySound(res->hasWon);
                    PlayMusicStream(res->won);
                    TriggerBlink(7.0f, BLACK, 0.0f);
                    SaveGameResults(state, 1);
                    ChangeGameState(LogicWonScreen, DrawWonScreen);
                    return;
                }
                PlaySound(res->answerOnChalkboard);
                TriggerBlink(0.2f, BLACK, 0.0f);
                ChangeGameState(LogicClassroom, DrawClassroom);
            }
            else
            {
                state->player.incorrectPoints++;

                if (state->player.incorrectPoints >= state->maxIncorrectPoints)
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
    DrawTextureEx(res->centerTaskWindow, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->layoutConfig.returnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);

    Ts_MathTaskData *currentTask = &state->centerMathTask;

    char questionString[16];
    sprintf(questionString, "%d %c %d %s", currentTask->num1, currentTask->op, currentTask->num2, "= ?");
    DrawText(questionString, 250, 250, 70, LIGHTGRAY);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        DrawRectangleLinesEx(state->layoutConfig.centerMathAnswersRec[i], 3, LIGHTGRAY);

        int textWidth = MeasureText(answersOnScreen, 60);

        int answerPosX = (int)(state->layoutConfig.centerMathAnswersRec[i].x + (state->layoutConfig.centerMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->layoutConfig.centerMathAnswersRec[i].y + (state->layoutConfig.centerMathAnswersRec[i].height / 2) - (60 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 60, LIGHTGRAY);
    }
}

// --------------------------------------- RIGHT SECTION ---------------------------------------
void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.RightTaskReturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.flashlightRec))
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
    DrawTextureEx(res->rightTaskWindow, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->layoutConfig.RightTaskReturnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicRightTaskWindowLight(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.RightTaskReturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.flashlightRec))
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
    DrawTextureEx(res->rightTaskWindowLights, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->layoutConfig.RightTaskReturnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->rightMathTask;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.RightTaskReturnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->doorOpening);
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.flashlightRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layoutConfig.rightMathAnswersRec[i]) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            state->player.idleTime = 0.0f;

            if (i == currentTask->correctIndex)
            {
                currentTask->isActive = false;
                state->isRightTaskTrue = 0;
                state->player.correctPoints++;

                if (state->player.correctPoints >= state->maxCorrectPoints)
                {
                    PlaySound(res->hasWon);
                    PlayMusicStream(res->won);
                    TriggerBlink(7.0f, BLACK, 0.0f);
                    SaveGameResults(state, 1);
                    ChangeGameState(LogicWonScreen, DrawWonScreen);
                    return;
                }
                PlaySound(res->girlLaugh);
                TriggerBlink(0.2f, BLACK, 0.0f);
                ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
            }
            else
            {
                state->player.incorrectPoints++;

                if (state->player.incorrectPoints >= state->maxIncorrectPoints)
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

void DrawStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->rightTaskTrue, state->layoutConfig.backgroundPosFit, 0.0f, state->layoutConfig.scaleFit, WHITE);

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

        int answerPosX = (int)(state->layoutConfig.rightMathAnswersRec[i].x + (state->layoutConfig.rightMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(state->layoutConfig.rightMathAnswersRec[i].y + (state->layoutConfig.rightMathAnswersRec[i].height / 2) - (35 / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, 35, BLACK);
    }

    DrawTextureEx(res->returnArrow, state->layoutConfig.RightTaskReturnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

// --------------------------------------- WON SECTION ---------------------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->won);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->won);
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }

    StopMusicStream(res->monoV1);
    StopMusicStream(res->background);
    StopSound(res->eatingCookie);
    StopSound(res->ghostWriting);
    StopSound(res->girlLaugh);
}

void DrawWonScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("BUEN TRABAJO!", 100, 200, 60, WHITE);
    DrawText("Sobreviviste la noche", 100, 265, 20, WHITE);

    char resultsText[50];
    sprintf(resultsText, "Tiempo Sobrevivido: %.2f seg", state->player.playerTotalTime);
    DrawText(resultsText, 100, 700, 20, YELLOW);
}

// --------------------------------------- LOST SECTION ---------------------------------------
void LogicLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    UpdateMusicStream(res->ending);

    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->ending);
        PlayMusicStream(res->intro);
        TriggerBlink(2.2f, BLACK, 0.0f);
        ChangeGameState(LogicStartScreen, DrawStartScreen);
        return;
    }

    StopMusicStream(res->monoV1);
    StopMusicStream(res->background);
    StopSound(res->eatingCookie);
    StopSound(res->ghostWriting);
    StopSound(res->girlLaugh);
}

void DrawLostScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("JUEGO TERMINADO", 100, 200, 60, WHITE);
    DrawText("Causa de muerte: Fue difícil saber cuánto es 1 + 1", 100, 265, 20, WHITE);

    char resultsText[50];
    sprintf(resultsText, "Tiempo Sobrevivido: %.2f seg", state->player.playerTotalTime);

    DrawText(resultsText, 100, 700, 20, YELLOW);
}

// --------------------------- SAVE GAME RESULTS ---------------------------
void SaveGameResults(Ts_GameState *state, int playerWon)
{
    FILE *textFile;
    textFile = fopen("resultado.txt", "a");

    fprintf(textFile, "Estado: %s \n", playerWon ? "GANASTE" : "PERDISTE");
    fprintf(textFile, "Tiempo: %.2f segundos\n", state->player.playerTotalTime);
    fprintf(textFile, "Respuestas correctas: %d\n", state->player.correctPoints);
    fprintf(textFile, "Respuestas incorrectas: %d\n\n", state->player.incorrectPoints);

    fclose(textFile);

    FILE *binFile;
    binFile = fopen("resultado.dat", "ab");

    fwrite(&playerWon, sizeof(int), 1, binFile);
    fwrite(&state->player.playerTotalTime, sizeof(float), 1, binFile);
    fwrite(&state->player.correctPoints, sizeof(float), 1, binFile);
    fwrite(&state->player.incorrectPoints, sizeof(float), 1, binFile);

    fclose(binFile);
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
        PlaySound(res->girlJumpscare);
        TriggerJumpscare(state);
    }
}

// --------------------------------------- ASSIGN TASK SECTION ---------------------------------------
void StartTimerTask(const Ts_resources *res, Ts_GameState *state)
{
    if (state->player.safeTime < 55.0f)
    {
        return;
    }

    float deltatime = GetFrameTime();
    state->currentTime += deltatime;

    if (state->currentTime >= state->interval)
    {
        state->currentTime -= state->interval;
        state->interval = (float)GetRandomValue(5, 10);
        AssignTask(res, state);
    }
}

void AssignTask(const Ts_resources *res, Ts_GameState *state)
{
    int roll = GetRandomValue(0, 99);

    int limitLeft = state->chanceLeft;
    int limitCenter = state->chanceLeft + state->chanceCenter;

    if (roll < limitLeft)
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
    if (roll < limitCenter)
    {
        if (state->isCenterTaskTrue)
        {
            return;
        }
        Ts_MathTaskData *currentTask = &state->centerMathTask;
        centerMathTaskGenerator(currentTask);
        PlaySound(res->ghostWriting);
        state->isCenterTaskTrue = 1;
        return;
    }

    if (state->isRightTaskTrue)
    {
        return;
    }
    Ts_MathTaskData *currentTask = &state->rightMathTask;
    rightMathTaskGenerator(currentTask);
    state->isRightTaskTrue = 1;
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
        StopSound(res->ghostWriting);
        StopSound(res->girlLaugh);

        state->JumpscareConfig.jumpscareAudioPlayed = 1;
    }

    if (state->JumpscareConfig.jumpscareTimer >= 1.0f)
    {
        TriggerBlink(6.0f, BLACK, 0.0f);
        PlayMusicStream(res->ending);
        SaveGameResults(state, 0);
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