#include "config.h"
#include <math.h>

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
    state.centerArrowPos = (Vector2){602, 500};
    // Left Arrow
    state.scaleLeftArrow = arrowSize / (float)res->leftArrow.width;
    state.leftArrowPos = (Vector2){122, 500};
    // Rigth Arrow
    state.scaleRightArrow = arrowSize / (float)res->rightArrow.width;
    state.rightArrowPos = (Vector2){1050, 500};
    // Return Arrow
    state.scaleReturnArrow = returnArrowSize / (float)res->centerArrow.width;
    state.returnArrowPos = (Vector2){602, 640};
    // Snail
    state.scaleHelper = helperSize / (float)res->helper.width;
    state.helperPos = (Vector2){50, 660};

    // Colision Objects position
    state.centerArrowRec = (Rectangle){state.centerArrowPos.x, state.centerArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.leftArrowRec = (Rectangle){state.leftArrowPos.x, state.leftArrowPos.y, res->leftArrow.width * state.scaleLeftArrow, res->leftArrow.height * state.scaleLeftArrow};
    state.rightArrowRec = (Rectangle){state.rightArrowPos.x, state.rightArrowPos.y, res->rightArrow.width * state.scaleRightArrow, res->rightArrow.height * state.scaleRightArrow};
    state.returnArrowRec = (Rectangle){state.returnArrowPos.x, state.returnArrowPos.y, res->centerArrow.width * state.scaleCenterArrow, res->centerArrow.height * state.scaleCenterArrow};
    state.helperRec = (Rectangle){state.helperPos.x, state.helperPos.y, res->helper.width * state.scaleHelper, res->helper.height * state.scaleHelper};

    // Timer Setup
    state.interval = 10.0f; // Seconds
    state.currentTime = 0.0f;

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
    if (IsKeyPressed(KEY_ENTER))
    {
        StopMusicStream(res->introMusic);
        PlayMusicStream(res->m1Track);
        PlayMusicStream(res->monoV1);

        ChangeGameState(LogicClassroom, DrawClassroom);
    }
}

void DrawStartScreen(const Ts_resources *res, Ts_GameState *state)
{
    ClearBackground(BLACK);
    DrawText("Five Nigths At School", 150, 200, 60, WHITE);
    DrawText("Presiona [ENTER] para comenzar . . .", 150, 270, 20, WHITE);
}

// ------------------------------------------------------------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, state->centerArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicCenterTaskWindow, DrawCenterTaskWindow);
            return;
        }
    }
    if (CheckCollisionPointRec(mousePos, state->leftArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->arrowClick);
            ChangeGameState(LogicLeftTaskWindow, DrawLeftTaskWindow);
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

    // Start task
    StartTimerTask(res, state);
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

// ------------------------------------------------------------------------------------------
void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
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

void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    if (state->isLeftTaskTrue)
    {
        ChangeGameState(LogicStartLeftTaskTrue, DrawStartLeftTaskTrue);
    }
    else
    {
        DrawTextureEx(res->leftTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    }
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
}

void LogicStartLeftTaskTrue(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, state->returnArrowRec))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }
}

void DrawStartLeftTaskTrue(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->leftTaskTrue, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->taskList[0], state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
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

void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
    DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
    DrawTextureEx(res->rightTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
    DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleReturnArrow, WHITE);
    if (state->isHelpWindow)
    {
        DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
    }
}

// ------------------------------------------------------------------------------------------
void LogicWonScreen(const Ts_resources *res, Ts_GameState *state)
{
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
    if (IsKeyPressed(KEY_ENTER))
    {
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
void StartTimerTask(const Ts_resources *res, Ts_GameState *state)
{
    // Clock
    float deltatime = GetFrameTime();
    state->currentTime += deltatime;
    if (state->currentTime >= state->interval)
    {
        state->currentTime -= state->interval;
        state->newIndex = GetRandomValue(0, 2);
        state->startTask = 1;
    }

    if (state->startTask == 1)
    {
        if (state->newIndex == 0)
        {
            state->isLeftTaskTrue = 1;
        }
        if (state->newIndex == 1)
        {
            PlaySound(res->writingOnBoard);
            state->isCenterTaskTrue = 1;
        }
        if (state->newIndex == 2)
        {
            PlaySound(res->girlLaugh);
            state->isRightTaskTrue = 1;
        }
    }
}
