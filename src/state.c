#include "state.h"
#include <math.h>
#include <stdio.h>

static GameLogicFunction currentLogic = 0;
static GameDrawFunction currentDraw = 0;
static Ts_BlinkEffect BlinkConfig = {0};

void GameStateConfig(const Ts_resources *res, Ts_GameState *state)
{
    // ------------------ TASK INITIAL SETUP ------------------
    state->taskConfig.maxCorrectPoints = 15;
    state->taskConfig.maxIncorrectPoints = 5;

    state->taskConfig.chanceLeft = 30;
    state->taskConfig.chanceCenter = 50;
    state->taskConfig.chanceRight = 20;

    state->taskConfig.interval = 8.0f;
    state->taskConfig.currentTime = 0.0f;

    // ------------------ TUTORIAL SETUP ------------------
    state->tutorialConfig.currentPageIndex = 0;
    state->tutorialConfig.maxPages = 6;

    // ------------------ JUMPSCARE INITIAL SETUP ------------------
    state->JumpscareConfig.jumpscareTimer = 0.0f;
    state->JumpscareConfig.jumpscareAudioPlayed = 0;

    // ------------------ MONOLOGUE INITIAL SETUP ------------------
    state->monoLen = GetMusicTimeLength(res->music.monoV1);

    ResourcesLayout(res, state);
}

void ResourcesLayout(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layoutConfig;

    // ------------------ BACKGROUND SCALE ------------------
    float baseWidth = 1250.0f;
    float baseHeight = 750.0f;

    float scaleX = (float)GetScreenWidth() / baseWidth;
    float scaleY = (float)GetScreenHeight() / baseHeight;
    float scale = fminf(scaleX, scaleY);

    state->layoutConfig.scaleFit = scale;

    // OFFSET FOR BACKGROUND IMAGE CENTER
    float offsetX = (GetScreenWidth() - (baseWidth * scale)) * 0.5f;
    float offsetY = (GetScreenHeight() - (baseHeight * scale)) * 0.5f;

    float imgScaleFactor = baseWidth / (float)res->texture.classroom.width;

    layout->scaleFit = scale * imgScaleFactor;

    layout->positionAbsolute = (Vector2){offsetX, offsetY};

    // SCALE FOR OBJECTS
    layout->scaleUI = scale;
    layout->offsetX = offsetX;
    layout->offsetY = offsetY;

    // ------------------ OBJECTS ------------------
    // Object Size
    float arrowSize = 60.0f;
    float returnArrowSize = 90.0f;
    float helperSize = 90.0f;
    float helperWindowSize = 80.0f;
    float tutorialBtnSize = 18.0f;

    // Object Position
    float tutorialBtnX = 1070.0f;
    float tutorialBtnY = 40.0f;
    const char *tutorialBtnText = "TUTORIAL";

    // Object Scale
    layout->scaleArrow = (arrowSize / (float)res->texture.centerArrow.width) * scale;
    layout->scaleReturnArrow = (returnArrowSize / (float)res->texture.centerArrow.width) * scale;
    state->helperConfig.scaleHelper = (helperSize / (float)res->texture.helper.width) * scale;
    state->helperConfig.scaleHelperWindow = (helperWindowSize / (float)res->texture.helper.width) * scale;
    state->tutorialConfig.tutorialBtnFontSize = (int)(tutorialBtnSize * scale);

    // Object Positions
    layout->centerArrowPos.x = offsetX + (588.0f * scale);
    layout->centerArrowPos.y = offsetY + (500.0f * scale);

    layout->leftArrowPos.x = offsetX + (122.0f * scale);
    layout->leftArrowPos.y = offsetY + (500.0f * scale);

    layout->rightArrowPos.x = offsetX + (1050.0f * scale);
    layout->rightArrowPos.y = offsetY + (500.0f * scale);

    layout->returnArrowPos.x = offsetX + (588.0f * scale);
    layout->returnArrowPos.y = offsetY + (640.0f * scale);

    layout->RightTaskReturnArrowPos.x = offsetX + (200.0f * scale);
    layout->RightTaskReturnArrowPos.y = offsetY + (640.0f * scale);

    state->helperConfig.helperPos.x = offsetX + (50.0f * scale);
    state->helperConfig.helperPos.y = offsetY + (660.0f * scale);

    state->helperConfig.tutorialWindowPos.x = offsetX + (300.0f * scale);
    state->helperConfig.tutorialWindowPos.y = offsetY + (200.0f * scale);

    state->tutorialConfig.tutorialBtnPos.x = offsetX + (tutorialBtnX * scale);
    state->tutorialConfig.tutorialBtnPos.y = offsetY + (tutorialBtnY * scale);

    int textWidth = MeasureText(tutorialBtnText, state->tutorialConfig.tutorialBtnFontSize);

    // ------------------ COLISION OBJECTS POSITIONS ------------------
    layout->centerArrowRec = (Rectangle){
        layout->centerArrowPos.x,
        layout->centerArrowPos.y,
        res->texture.centerArrow.width * layout->scaleArrow,
        res->texture.centerArrow.height * layout->scaleArrow};

    layout->leftArrowRec = (Rectangle){
        layout->leftArrowPos.x,
        layout->leftArrowPos.y,
        res->texture.centerArrow.width * layout->scaleArrow,
        res->texture.centerArrow.height * layout->scaleArrow};

    layout->rightArrowRec = (Rectangle){
        layout->rightArrowPos.x,
        layout->rightArrowPos.y,
        res->texture.centerArrow.width * layout->scaleArrow,
        res->texture.centerArrow.height * layout->scaleArrow};

    layout->returnArrowRec = (Rectangle){
        layout->returnArrowPos.x,
        layout->returnArrowPos.y,
        res->texture.centerArrow.width * layout->scaleArrow,
        res->texture.centerArrow.height * layout->scaleArrow};

    layout->RightTaskReturnArrowRec = (Rectangle){
        state->layoutConfig.RightTaskReturnArrowPos.x,
        state->layoutConfig.RightTaskReturnArrowPos.y,
        res->texture.centerArrow.width * layout->scaleArrow,
        res->texture.centerArrow.height * layout->scaleArrow};

    state->helperConfig.helperRec = (Rectangle){
        state->helperConfig.helperPos.x,
        state->helperConfig.helperPos.y,
        res->texture.helper.width * state->helperConfig.scaleHelper,
        res->texture.helper.height * state->helperConfig.scaleHelper};

    layout->flashlightRec = (Rectangle){
        offsetX + (532.0f * scale),
        offsetY + (640.0f * scale),
        160.0f * scale,
        160.0f * scale};

    state->tutorialConfig.tutorialBtnRec = (Rectangle){
        state->tutorialConfig.tutorialBtnPos.x,
        state->tutorialConfig.tutorialBtnPos.y,
        (float)textWidth,
        (float)state->tutorialConfig.tutorialBtnFontSize};

    state->tutorialConfig.nextBtnRec = (Rectangle){
        offsetX + (45.0f * scale),
        offsetY + (280.0f * scale),
        147.0f * scale,
        147.0f * scale};

    state->blinkState.blinkRec = (Rectangle){
        offsetX,
        offsetY,
        baseWidth * scale,
        baseHeight * scale};

    // ------------------ MATH OP WINDOW POSITION BOOK ------------------
    float columnWidth = 150;
    float columnHeight = 60;
    float columnPosX = 1010;
    float columnPosY = 440;
    float gapY = 62.0f;

    for (int i = 0; i < 4; i++)
    {
        layout->bookMathAnswersRec[i] = (Rectangle){
            offsetX + (columnPosX * scale),
            offsetY + ((columnPosY + (i * gapY)) * scale),
            columnWidth * scale,
            columnHeight * scale};
    }

    // ------------------ MATH OP WINDOW POSITION CHALK BOARD ------------------
    float centerColumnWidth = 200.0f;
    float centerColumnHeight = 80.0f;
    float centerColumnPosX = 800.0f;
    float centerColumnPosY = 151.0f;
    float centerSeparation = 15.0f;
    float centerGapY = centerColumnHeight + centerSeparation;

    for (int i = 0; i < 4; i++)
    {
        layout->chalkboardMathAnswersRec[i] = (Rectangle){
            offsetX + (centerColumnPosX * scale),
            offsetY + ((centerColumnPosY + (i * centerGapY)) * scale),
            centerColumnWidth * scale,
            centerColumnHeight * scale};
    }

    // ------------------ MATH WINDOW VALUES WITH SCALE ------------------
    // Book section
    layout->bookFontSizeQuestion = (int)(45.0f * scale);
    layout->bookFontSizeAnswer = (int)(40.0f * scale);

    layout->MathOpLine1Pos = (Vector2){
        offsetX + (950.0f * scale),
        offsetY + (470.0f * scale)};

    layout->MathOpLine2Pos = (Vector2){
        offsetX + (950.0f * scale),
        offsetY + ((470.0f + 50.0f) * scale)};

    // Chalkboard section
    layout->chalkboardFontSizeQuestion = (int)(70.0f * scale);
    layout->chalkboardFontSizeAnswer = (int)(60.0f * scale);

    layout->chalkboardMathQuestionPos = (Vector2){
        offsetX + (250.0f * scale),
        offsetY + (250.0f * scale)};
    ;

    // ------------------ START/LOST TEXT VALUES WITH SCALE ------------------
    layout->startTitlePos = (Vector2){
        offsetX + (100.0f * scale),
        offsetY + (200.0f * scale)};

    layout->instructionY = offsetY + (270.0f * scale);
    layout->creditsTitleY = offsetY + (650.0f * scale);
    layout->credit1Y = offsetY + (680.0f * scale);
    layout->credit2Y = offsetY + (700.0f * scale);
    layout->credit3Y = offsetY + (720.0f * scale);
    layout->gameResults = offsetY + (700.0f * scale);

    layout->fontSizeTitle = (int)(60.0f * scale);
    layout->fontSizeInstruction = (int)(20.0f * scale);
    layout->fontSizeResults = (int)(20.0f * scale);
    layout->fontSizeCredits = (int)(15.0f * scale);
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
        state->isLeftTaskTrue = 0;
        state->isCenterTaskTrue = 0;
        state->isRightTaskTrue = 0;

        TriggerBlink(0.6f, BLACK, 0.0f);
        ChangeGameState(LogicClassroom, DrawClassroom);
    }

    if (CheckCollisionPointRec(mousePos, state->tutorialConfig.tutorialBtnRec))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            ChangeGameState(LogicTutorial, DrawTutorial);
        }
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void DrawStartScreen(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layoutConfig;

    ClearBackground(BLACK);
    DrawText("Five Nigths At School", layout->startTitlePos.x, layout->startTitlePos.y, layout->fontSizeTitle, WHITE);
    DrawText("Presiona [ENTER] para comenzar . . .", layout->startTitlePos.x, layout->instructionY, layout->fontSizeInstruction, WHITE);
    DrawText("PRESENTADO POR:", layout->startTitlePos.x, layout->creditsTitleY, layout->fontSizeCredits, WHITE);
    DrawText("Perez Aguirre Mextli Citlali", layout->startTitlePos.x, layout->credit1Y, layout->fontSizeCredits, WHITE);
    DrawText("Meza Espinoza Kevin Andre", layout->startTitlePos.x, layout->credit2Y, layout->fontSizeCredits, WHITE);
    DrawText("Zazueta Medrano Aidan", layout->startTitlePos.x, layout->credit3Y, layout->fontSizeCredits, WHITE);
    DrawText("TUTORIAL", (int)state->tutorialConfig.tutorialBtnPos.x, (int)state->tutorialConfig.tutorialBtnPos.y, state->tutorialConfig.tutorialBtnFontSize, WHITE);
}

// --------------------------------------- TUTORIAL SECTION ---------------------------------------
void LogicTutorial(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    UpdateMusicStream(res->music.intro);

    if (CheckCollisionPointRec(mousePos, state->tutorialConfig.nextBtnRec))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            if (state->tutorialConfig.currentPageIndex < state->tutorialConfig.maxPages - 1)
            {
                state->tutorialConfig.currentPageIndex++;
            }
            else
            {
                state->tutorialConfig.currentPageIndex = 0;
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
    ClearBackground(BLACK);
    Texture2D currentPageTexture = res->texture.tutorialPages[state->tutorialConfig.currentPageIndex];
    DrawTextureEx(currentPageTexture, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
}

// --------------------------------------- CLASSROOM SECTION---------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.leftArrowRec))
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
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.centerArrowRec))
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
    if (CheckCollisionPointRec(mousePos, state->layoutConfig.rightArrowRec))
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

    if (CheckCollisionPointRec(mousePos, state->helperConfig.helperRec))
    {
        isMouseOverClickable = 1;
        state->helperConfig.isHelpWindow = 1;
    }
    else
    {
        state->helperConfig.isHelpWindow = 0;
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
    ClearBackground(BLACK);
    DrawTextureEx(res->texture.classroom, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.centerArrow, state->layoutConfig.centerArrowPos, 0.0f, state->layoutConfig.scaleArrow, WHITE);
    DrawTextureEx(res->texture.leftArrow, state->layoutConfig.leftArrowPos, 0.0f, state->layoutConfig.scaleArrow, WHITE);
    DrawTextureEx(res->texture.rightArrow, state->layoutConfig.rightArrowPos, 0.0f, state->layoutConfig.scaleArrow, WHITE);

    if (state->isLeftTaskTrue)
    {
        DrawTextureEx(res->texture.leftIndicator, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    }
    if (state->isCenterTaskTrue)
    {
        DrawTextureEx(res->texture.centerIndicator, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    }
    if (state->helperConfig.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperGeneralWindow, state->helperConfig.tutorialWindowPos, 0.0f, state->helperConfig.scaleHelperWindow, WHITE);
    }
    DrawTextureEx(res->texture.helper, state->helperConfig.helperPos, 0.0f, state->helperConfig.scaleHelper, WHITE);
}

// --------------------------------------- LEFT SECTION ---------------------------------------
void LogicLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (state->isLeftTaskTrue)
    {
        PlaySound(res->sound.suspenseSound);
        ChangeGameState(LogicStartLeftTask, DrawStartLeftTask);
        return;
    }

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
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
    ClearBackground(BLACK);
    DrawTextureEx(res->texture.leftTaskWindow, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layoutConfig.returnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicStartLeftTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    Ts_MathTaskData *currentTask = &state->leftMathTask;

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperConfig.helperRec))
    {
        isMouseOverClickable = 1;
        state->helperConfig.isHelpWindow = 1;
    }
    else
    {
        state->helperConfig.isHelpWindow = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layoutConfig.bookMathAnswersRec[i]))
        {
            isMouseOverClickable = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state->player.idleTime = 0.0f;

                if (i == currentTask->correctIndex)
                {
                    currentTask->isActive = false;
                    state->isLeftTaskTrue = 0;
                    state->player.correctPoints++;

                    if (state->player.correctPoints >= state->taskConfig.maxCorrectPoints)
                    {
                        PlaySound(res->sound.hasWon);
                        PlayMusicStream(res->music.won);
                        TriggerBlink(7.0f, BLACK, 0.0f);
                        SaveGameResults(state, 1);
                        ChangeGameState(LogicWonScreen, DrawWonScreen);
                        return;
                    }
                    PlaySound(res->sound.eatingCookie);
                    TriggerBlink(0.2f, BLACK, 0.0f);
                    ChangeGameState(LogicClassroom, DrawClassroom);
                }
                else
                {
                    state->player.incorrectPoints++;

                    if (state->player.incorrectPoints >= state->taskConfig.maxIncorrectPoints)
                    {
                        PlayMusicStream(res->music.ending);
                        PlaySound(res->sound.girlJumpscare);
                        TriggerJumpscare(state);
                        return;
                    }
                    TriggerBlink(0.1f, RED, 0.0f);
                    PlaySound(res->sound.incorrect);
                }
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
    Ts_Layout *layout = &state->layoutConfig;

    ClearBackground(BLACK);
    DrawTextureEx(res->texture.leftTaskTrue, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->returnArrowPos, 0.0f, layout->scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helperConfig.helperPos, 0.0f, state->helperConfig.scaleHelper, WHITE);

    // ------------------- DRAW QUESTION -------------------
    char mathNum1[16];
    sprintf(mathNum1, "%d", currentTask->num1);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", currentTask->op, currentTask->num2);

    int widthNum1 = MeasureText(mathNum1, layout->bookFontSizeQuestion);
    int widthLine2 = MeasureText(mathNum2, layout->bookFontSizeQuestion);

    DrawText(mathNum1,
             (int)(layout->MathOpLine1Pos.x - widthNum1),
             (int)layout->MathOpLine1Pos.y,
             layout->bookFontSizeQuestion, BLACK);

    DrawText(mathNum2,
             (int)(layout->MathOpLine2Pos.x - widthLine2),
             (int)layout->MathOpLine2Pos.y,
             layout->bookFontSizeQuestion, BLACK);

    // ------------------- DRAW ANSWERS -------------------
    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, layout->bookFontSizeAnswer);

        int answerPosX = (int)(layout->bookMathAnswersRec[i].x + (layout->bookMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(layout->bookMathAnswersRec[i].y + (layout->bookMathAnswersRec[i].height / 2) - (layout->bookFontSizeAnswer / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, layout->bookFontSizeAnswer, BLACK);
    }

    if (state->helperConfig.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperSumWindow, state->helperConfig.tutorialWindowPos, 0.0f, state->helperConfig.scaleHelperWindow, WHITE);
    }
}

// --------------------------------------- CENTER SECTION ---------------------------------------
void LogicCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (state->isCenterTaskTrue)
    {
        ChangeGameState(LogicStartCenterTask, DrawStartCenterTask);
        return;
    }

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
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
    ClearBackground(BLACK);
    DrawTextureEx(res->texture.centerTaskWindow, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layoutConfig.returnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicStartCenterTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->centerMathTask;

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.returnArrowRec))
    {
        isMouseOverClickable = 1;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            TriggerBlink(0.2f, BLACK, 0.0f);
            ChangeGameState(LogicClassroom, DrawClassroom);
            return;
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperConfig.helperRec))
    {
        isMouseOverClickable = 1;
        state->helperConfig.isHelpWindow = 1;
    }
    else
    {
        state->helperConfig.isHelpWindow = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layoutConfig.chalkboardMathAnswersRec[i]))
        {
            isMouseOverClickable = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state->player.idleTime = 0.0f;

                if (i == currentTask->correctIndex)
                {
                    currentTask->isActive = false;
                    state->isCenterTaskTrue = 0;
                    state->player.correctPoints++;

                    if (state->player.correctPoints >= state->taskConfig.maxCorrectPoints)
                    {
                        PlaySound(res->sound.hasWon);
                        PlayMusicStream(res->music.won);
                        TriggerBlink(7.0f, BLACK, 0.0f);
                        SaveGameResults(state, 1);
                        ChangeGameState(LogicWonScreen, DrawWonScreen);
                        return;
                    }
                    PlaySound(res->sound.answerOnChalkboard);
                    TriggerBlink(0.2f, BLACK, 0.0f);
                    ChangeGameState(LogicClassroom, DrawClassroom);
                }
                else
                {
                    state->player.incorrectPoints++;

                    if (state->player.incorrectPoints >= state->taskConfig.maxIncorrectPoints)
                    {
                        PlayMusicStream(res->music.ending);
                        PlaySound(res->sound.girlJumpscare);
                        TriggerJumpscare(state);
                        return;
                    }
                    TriggerBlink(0.1f, RED, 0.0f);
                    PlaySound(res->sound.incorrect);
                }
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
    Ts_Layout *layout = &state->layoutConfig;

    ClearBackground(BLACK);
    DrawTextureEx(res->texture.centerTaskWindow, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->returnArrowPos, 0.0f, layout->scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helperConfig.helperPos, 0.0f, state->helperConfig.scaleHelper, WHITE);

    char questionStr[16];
    sprintf(questionStr, "%d %c %d %s", currentTask->num1, currentTask->op, currentTask->num2, "= ?");
    DrawText(questionStr, layout->chalkboardMathQuestionPos.x, layout->chalkboardMathQuestionPos.y, layout->chalkboardFontSizeQuestion, LIGHTGRAY);

    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        DrawRectangleLinesEx(layout->chalkboardMathAnswersRec[i], 3, LIGHTGRAY);

        int textWidth = MeasureText(answersOnScreen, layout->chalkboardFontSizeAnswer);

        int answerPosX = (int)(layout->chalkboardMathAnswersRec[i].x + (layout->chalkboardMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(layout->chalkboardMathAnswersRec[i].y + (layout->chalkboardMathAnswersRec[i].height / 2) - (layout->chalkboardFontSizeAnswer / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, layout->chalkboardFontSizeAnswer, LIGHTGRAY);
    }
    if (state->helperConfig.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperMultWindow, state->helperConfig.tutorialWindowPos, 0.0f, state->helperConfig.scaleHelperWindow, WHITE);
    }
}

// --------------------------------------- RIGHT SECTION ---------------------------------------
void LogicRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.RightTaskReturnArrowRec))
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

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.flashlightRec))
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
    ClearBackground(BLACK);
    DrawTextureEx(res->texture.rightTaskWindow, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layoutConfig.RightTaskReturnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicRightTaskLight(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.RightTaskReturnArrowRec))
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

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.flashlightRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            ChangeGameState(LogicRightTask, DrawRightTask);
        }
    }

    if (state->isRightTaskTrue)
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
    ClearBackground(BLACK);
    DrawTextureEx(res->texture.rightTaskWindowLights, state->layoutConfig.positionAbsolute, 0.0f, state->layoutConfig.scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, state->layoutConfig.RightTaskReturnArrowPos, 0.0f, state->layoutConfig.scaleReturnArrow, WHITE);
}

void LogicStartRightTask(const Ts_resources *res, Ts_GameState *state)
{
    Vector2 mousePos = GetMousePosition();
    Ts_MathTaskData *currentTask = &state->rightMathTask;

    int isMouseOverClickable = 0;

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.RightTaskReturnArrowRec))
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

    if (CheckCollisionPointRec(mousePos, state->layoutConfig.flashlightRec))
    {
        isMouseOverClickable = 1;

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            PlaySound(res->sound.arrowClick);
            ChangeGameState(LogicRightTask, DrawRightTask);
        }
    }

    if (CheckCollisionPointRec(mousePos, state->helperConfig.helperRec))
    {
        isMouseOverClickable = 1;
        state->helperConfig.isHelpWindow = 1;
    }
    else
    {
        state->helperConfig.isHelpWindow = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        if (CheckCollisionPointRec(mousePos, state->layoutConfig.bookMathAnswersRec[i]))
        {
            isMouseOverClickable = 1;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                state->player.idleTime = 0.0f;

                if (i == currentTask->correctIndex)
                {
                    currentTask->isActive = false;
                    state->isRightTaskTrue = 0;
                    state->player.correctPoints++;

                    if (state->player.correctPoints >= state->taskConfig.maxCorrectPoints)
                    {
                        PlaySound(res->sound.hasWon);
                        PlayMusicStream(res->music.won);
                        TriggerBlink(7.0f, BLACK, 0.0f);
                        SaveGameResults(state, 1);
                        ChangeGameState(LogicWonScreen, DrawWonScreen);
                        return;
                    }
                    PlaySound(res->sound.girlLaugh);
                    TriggerBlink(0.2f, BLACK, 0.0f);
                    ChangeGameState(LogicRightTask, DrawRightTask);
                }
                else
                {
                    state->player.incorrectPoints++;

                    if (state->player.incorrectPoints >= state->taskConfig.maxIncorrectPoints)
                    {
                        PlayMusicStream(res->music.ending);
                        PlaySound(res->sound.girlJumpscare);
                        TriggerJumpscare(state);
                        return;
                    }
                    TriggerBlink(0.1f, RED, 0.0f);
                    PlaySound(res->sound.incorrect);
                }
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
    Ts_Layout *layout = &state->layoutConfig;

    ClearBackground(BLACK);
    DrawTextureEx(res->texture.rightTaskTrue, layout->positionAbsolute, 0.0f, layout->scaleFit, WHITE);
    DrawTextureEx(res->texture.returnArrow, layout->RightTaskReturnArrowPos, 0.0f, layout->scaleReturnArrow, WHITE);
    DrawTextureEx(res->texture.helper, state->helperConfig.helperPos, 0.0f, state->helperConfig.scaleHelper, WHITE);

    // ------------------- DRAW QUESTION -------------------
    char mathNum1[16];
    sprintf(mathNum1, "%d", currentTask->num1);

    char mathNum2[16];
    sprintf(mathNum2, "%c %d", currentTask->op, currentTask->num2);

    int widthNum1 = MeasureText(mathNum1, layout->bookFontSizeQuestion);
    int widthLine2 = MeasureText(mathNum2, layout->bookFontSizeQuestion);

    DrawText(mathNum1,
             (int)(layout->MathOpLine1Pos.x - widthNum1),
             (int)layout->MathOpLine1Pos.y,
             layout->bookFontSizeQuestion, BLACK);

    DrawText(mathNum2,
             (int)(layout->MathOpLine2Pos.x - widthLine2),
             (int)layout->MathOpLine2Pos.y,
             layout->bookFontSizeQuestion, BLACK);

    // ------------------- DRAW ANSWERS -------------------
    for (int i = 0; i < 4; i++)
    {
        char answersOnScreen[10];
        sprintf(answersOnScreen, "%d", currentTask->choicesList[i]);

        int textWidth = MeasureText(answersOnScreen, layout->bookFontSizeAnswer);

        int answerPosX = (int)(layout->bookMathAnswersRec[i].x + (layout->bookMathAnswersRec[i].width / 2) - (textWidth / 2));
        int answerPosY = (int)(layout->bookMathAnswersRec[i].y + (layout->bookMathAnswersRec[i].height / 2) - (layout->bookFontSizeAnswer / 2));

        DrawText(answersOnScreen, answerPosX, answerPosY, layout->bookFontSizeAnswer, BLACK);
    }
    if (state->helperConfig.isHelpWindow)
    {
        DrawTextureEx(res->texture.helperSubWindow, state->helperConfig.tutorialWindowPos, 0.0f, state->helperConfig.scaleHelperWindow, WHITE);
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
    Ts_Layout *layout = &state->layoutConfig;

    char resultsText[50];
    sprintf(resultsText, "Tiempo Sobrevivido: %.2f seg", state->player.playerTotalTime);

    ClearBackground(BLACK);
    DrawText("BUEN TRABAJO!", layout->startTitlePos.x, layout->startTitlePos.y, layout->fontSizeTitle, WHITE);
    DrawText("Sobreviviste la noche", layout->startTitlePos.x, state->layoutConfig.instructionY, layout->fontSizeInstruction, WHITE);
    DrawText(resultsText, layout->startTitlePos.x, layout->gameResults, layout->fontSizeResults, YELLOW);
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
    Ts_Layout *layout = &state->layoutConfig;

    char resultsText[50];
    sprintf(resultsText, "Tiempo Sobrevivido: %.2f seg", state->player.playerTotalTime);

    ClearBackground(BLACK);
    DrawText("JUEGO TERMINADO", layout->startTitlePos.x, layout->startTitlePos.y, layout->fontSizeTitle, WHITE);
    DrawText("Causa de muerte: Entraron las entidades al salón", layout->startTitlePos.x, state->layoutConfig.instructionY, layout->fontSizeInstruction, WHITE);
    DrawText(resultsText, layout->startTitlePos.x, layout->gameResults, layout->fontSizeResults, YELLOW);
}

// --------------------------------------- SAVE GAME RESULTS ---------------------------------------
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
        PlaySound(res->sound.girlJumpscare);
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
    state->taskConfig.currentTime += deltatime;

    if (state->taskConfig.currentTime >= state->taskConfig.interval)
    {
        state->taskConfig.currentTime -= state->taskConfig.interval;
        state->taskConfig.interval = (float)GetRandomValue(5, 10);
        AssignTask(res, state);
    }
}

void AssignTask(const Ts_resources *res, Ts_GameState *state)
{
    int roll = GetRandomValue(0, 99);

    int limitLeft = state->taskConfig.chanceLeft;
    int limitCenter = state->taskConfig.chanceLeft + state->taskConfig.chanceCenter;

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
        PlaySound(res->sound.ghostWriting);
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
    BlinkConfig.active = true;
    BlinkConfig.duration = duration;
    BlinkConfig.currentTimer = duration;
    BlinkConfig.color = color;
    BlinkConfig.frequency = frequency;
}

void UpdateBlink(void)
{
    if (BlinkConfig.active)
    {
        BlinkConfig.currentTimer -= GetFrameTime();

        if (BlinkConfig.currentTimer <= 0)
        {
            BlinkConfig.active = false;
            BlinkConfig.currentTimer = 0;
        }
    }
}

void DrawBlink(const Ts_GameState *state)
{
    if (BlinkConfig.active)
    {
        // Caso 1: Color sólido (sin parpadeo)
        if (BlinkConfig.frequency <= 0.0f)
        {
            float alpha = BlinkConfig.currentTimer / BlinkConfig.duration;

            // USAMOS EL RECTÁNGULO PRE-CALCULADO
            DrawRectangleRec(state->blinkState.blinkRec, Fade(BlinkConfig.color, alpha));
            return;
        }

        // Caso 2: Parpadeo intermitente
        float timeElapsed = BlinkConfig.duration - BlinkConfig.currentTimer;
        float cycleInterval = 1.0f / BlinkConfig.frequency;
        float cycleTime = fmodf(timeElapsed, cycleInterval);

        if (cycleTime < (cycleInterval / 2.0f))
        {
            // USAMOS EL RECTÁNGULO PRE-CALCULADO
            DrawRectangleRec(state->blinkState.blinkRec, Fade(BlinkConfig.color, 0.5f));
        }
    }
}

// --------------------------------------- JUMPSCARE EFFECT ---------------------------------------
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
        StopMusicStream(res->music.monoV1);
        StopMusicStream(res->music.background);
        StopSound(res->sound.eatingCookie);
        StopSound(res->sound.ghostWriting);
        StopSound(res->sound.girlLaugh);

        state->JumpscareConfig.jumpscareAudioPlayed = 1;
    }

    if (state->JumpscareConfig.jumpscareTimer >= 1.0f)
    {
        TriggerBlink(6.0f, BLACK, 0.0f);
        PlayMusicStream(res->music.ending);
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