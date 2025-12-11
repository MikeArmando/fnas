#include "config.h"
#include "state.h"
#include "resources.h"
#include "gameplay.h"
#include <math.h>

#define BASE_WIDTH 1250.0f
#define BASE_HEIGHT 750.0f

// --------------------------------------- GAME CONFIG ---------------------------------------
void GameStateConfig(const Ts_resources *res, Ts_GameState *state)
{
    // ------------------ TASK INITIAL SETUP ------------------
    state->task.chanceLeft = 30;
    state->task.chanceCenter = 50;
    state->task.chanceRight = 20;

    state->task.interval = 8.0f;
    state->task.currentTime = 0.0f;

    state->task.maxCorrectPoints = 15;
    state->task.maxIncorrectPoints = 5;

    state->leftMathTask.successSound = &res->sound.eatingCookie;
    state->centerMathTask.successSound = &res->sound.answerOnChalkboard;
    state->rightMathTask.successSound = &res->sound.girlLaugh;

    state->leftMathTask.jumpscareMessage = "No le diste galletas...";
    state->centerMathTask.jumpscareMessage = "El fantasma se canso de esperar...";
    state->rightMathTask.jumpscareMessage = "Su risa fue lo ultimo que escuchaste...";
    state->layout.startAndLost.currentGameOverMessage = "Bro?? Todo bien?";

    state->leftMathTask.returnLogic = LogicClassroom;
    state->leftMathTask.returnDraw = DrawClassroom;

    state->centerMathTask.returnLogic = LogicClassroom;
    state->centerMathTask.returnDraw = DrawClassroom;

    state->rightMathTask.returnLogic = LogicRightTask;
    state->rightMathTask.returnDraw = DrawRightTask;

    state->player.bestTime = LoadBestTime();

    // ------------------ TUTORIAL SETUP ------------------
    state->layout.tutorial.currentPageIndex = 0;
    state->layout.tutorial.maxPages = 6;
    state->layout.tutorial.silenceBtnCurrentIndex = 1;

    // ------------------ JUMPSCARE INITIAL SETUP ------------------
    state->Jumpscare.jumpscareTimer = 0.0f;
    state->Jumpscare.jumpscareAudioPlayed = 0;

    // ------------------ MONOLOGUE INITIAL SETUP ------------------
    state->monoLen = GetMusicTimeLength(res->music.monoV1);

    CalculateGlobalScale(res, state);
    CalculateObjectPositions(state);
    CalculateHitboxes(res, state);
}

void CalculateGlobalScale(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layout;

    // ------------------ BACKGROUND SCALE ------------------
    float scaleX = (float)GetScreenWidth() / BASE_WIDTH;
    float scaleY = (float)GetScreenHeight() / BASE_HEIGHT;
    float scale = fminf(scaleX, scaleY);

    // OFFSET FOR BACKGROUND IMAGE CENTER
    float offsetX = (GetScreenWidth() - (BASE_WIDTH * scale)) * 0.5f;
    float offsetY = (GetScreenHeight() - (BASE_HEIGHT * scale)) * 0.5f;

    // Sets Scale
    layout->scaleUI = scale;
    layout->offsetX = offsetX;
    layout->offsetY = offsetY;
    layout->positionAbsolute = (Vector2){offsetX, offsetY};

    float imgScaleFactor = BASE_WIDTH / (float)res->texture.classroom.width;
    layout->scaleFit = scale * imgScaleFactor;

    // ------------------ OBJECTS ------------------
    float arrowSize = 60.0f;
    float returnArrowSize = 90.0f;
    float helperSize = 90.0f;
    float helperWindowSize = 80.0f;
    float bestTimeFontSize = 18.0f;

    layout->startAndLost.fontSizeTitle = (int)(60.0f * layout->scaleUI);
    layout->startAndLost.fontSizeInstruction = (int)(20.0f * layout->scaleUI);
    layout->startAndLost.fontSizeResults = (int)(20.0f * layout->scaleUI);
    layout->startAndLost.fontSizeCredits = (int)(15.0f * layout->scaleUI);

    layout->math.bookFontSizeQuestion = (int)(45.0f * layout->scaleUI);
    layout->math.bookFontSizeAnswer = (int)(40.0f * layout->scaleUI);
    layout->math.chalkboardFontSizeQuestion = (int)(70.0f * layout->scaleUI);
    layout->math.chalkboardFontSizeAnswer = (int)(60.0f * layout->scaleUI);

    // Object Scale
    layout->arrow.scaleArrow = (arrowSize / (float)res->texture.centerArrow.width) * scale;
    layout->arrow.scaleReturnArrow = (returnArrowSize / (float)res->texture.centerArrow.width) * scale;
    state->helper.scaleHelper = (helperSize / (float)res->texture.helper.width) * scale;
    state->helper.scaleHelperWindow = (helperWindowSize / (float)res->texture.helper.width) * scale;

    // Fonts
    layout->startAndLost.bestTimeFontSize = (int)(bestTimeFontSize * scale);
}

void CalculateObjectPositions(Ts_GameState *state)
{
    Ts_Layout *layout = &state->layout;
    Ts_Arrow *arrow = &state->layout.arrow;

    float scale = layout->scaleUI;
    float offsetX = layout->offsetX;
    float offsetY = layout->offsetY;

    // Position
    float tutorialBtnX = 987.0f;
    float tutorialBtnY = 40.0f;

    float silenceBtnX = 1117.0f;
    float silenceBtnY = 40.0f;

    float bestTimeTextX = 970.0f;
    float bestTimeTextY = 700.0f;

    // Text
    layout->startAndLost.startTitlePos = (Vector2){offsetX + (100.0f * scale), offsetY + (200.0f * scale)};
    layout->startAndLost.instructionY = offsetY + (270.0f * scale);
    layout->startAndLost.creditsTitleY = offsetY + (650.0f * scale);
    layout->startAndLost.credit1Y = offsetY + (680.0f * scale);
    layout->startAndLost.credit2Y = offsetY + (700.0f * scale);
    layout->startAndLost.gameResults = offsetY + (700.0f * scale);
    layout->startAndLost.bestTimeTextPos = (Vector2){offsetX + (bestTimeTextX * scale), offsetY + (bestTimeTextY * scale)};

    // Objects
    arrow->centerArrowPos = (Vector2){offsetX + (588.0f * scale), offsetY + (500.0f * scale)};
    arrow->leftArrowPos = (Vector2){offsetX + (122.0f * scale), offsetY + (500.0f * scale)};
    arrow->rightArrowPos = (Vector2){offsetX + (1050.0f * scale), offsetY + (500.0f * scale)};
    arrow->returnArrowPos = (Vector2){offsetX + (588.0f * scale), offsetY + (640.0f * scale)};
    arrow->RightReturnArrowPos = (Vector2){offsetX + (200.0f * scale), offsetY + (640.0f * scale)};
    state->helper.helperPos = (Vector2){offsetX + (50.0f * scale), offsetY + (660.0f * scale)};
    state->helper.tutorialWindowPos = (Vector2){offsetX + (300.0f * scale), offsetY + (200.0f * scale)};
    layout->tutorial.tutorialBtnPos = (Vector2){offsetX + (tutorialBtnX * scale), offsetY + (tutorialBtnY * scale)};
    layout->tutorial.silenceBtnPos = (Vector2){offsetX + (silenceBtnX * scale), offsetY + (silenceBtnY * scale)};

    // Math Question
    layout->math.MathOpLine1Pos = (Vector2){offsetX + (950.0f * scale), offsetY + (470.0f * scale)};
    layout->math.MathOpLine2Pos = (Vector2){offsetX + (950.0f * scale), offsetY + ((470.0f + 50.0f) * scale)};

    layout->math.chalkboardMathQuestionPos = (Vector2){offsetX + (250.0f * scale), offsetY + (250.0f * scale)};
}

void CalculateHitboxes(const Ts_resources *res, Ts_GameState *state)
{
    Ts_Layout *layout = &state->layout;
    Ts_Arrow *arrow = &state->layout.arrow;

    float scale = layout->scaleUI;
    float offsetX = layout->offsetX;
    float offsetY = layout->offsetY;

    int flashlightBase = 160.0f;
    int flashlightHeight = 160.0f;

    int tutorialBase = 117.0f;
    int tutorialHeight = 48.0f;

    int silenceBase = 48.0f;
    int silenceHeight = 48.0f;

    int noseBase = 70.0f;
    int noseHeight = 415.0f;

    // ------------------ COLISION OBJECTS POSITIONS ------------------
    arrow->centerArrowRec = (Rectangle){
        arrow->centerArrowPos.x,
        arrow->centerArrowPos.y,
        res->texture.centerArrow.width * arrow->scaleArrow,
        res->texture.centerArrow.height * arrow->scaleArrow};

    arrow->leftArrowRec = (Rectangle){
        arrow->leftArrowPos.x,
        arrow->leftArrowPos.y,
        res->texture.centerArrow.width * arrow->scaleArrow,
        res->texture.centerArrow.height * arrow->scaleArrow};

    arrow->rightArrowRec = (Rectangle){
        arrow->rightArrowPos.x,
        arrow->rightArrowPos.y,
        res->texture.centerArrow.width * arrow->scaleArrow,
        res->texture.centerArrow.height * arrow->scaleArrow};

    arrow->returnArrowRec = (Rectangle){
        arrow->returnArrowPos.x,
        arrow->returnArrowPos.y,
        res->texture.returnArrow.width * arrow->scaleReturnArrow,
        res->texture.returnArrow.height * arrow->scaleReturnArrow};

    arrow->RightReturnArrowRec = (Rectangle){
        arrow->RightReturnArrowPos.x,
        arrow->RightReturnArrowPos.y,
        res->texture.returnArrow.width * arrow->scaleReturnArrow,
        res->texture.returnArrow.height * arrow->scaleReturnArrow};

    state->helper.helperRec = (Rectangle){
        state->helper.helperPos.x,
        state->helper.helperPos.y,
        res->texture.helper.width * state->helper.scaleHelper,
        res->texture.helper.height * state->helper.scaleHelper};

    layout->flashlightRec = (Rectangle){
        offsetX + (532.0f * scale),
        offsetY + (640.0f * scale),
        flashlightBase * scale,
        flashlightHeight * scale};

    layout->noseRec = (Rectangle){
        offsetX + (266.0f * scale),
        offsetY + (68.0f * scale),
        noseBase * scale,
        noseHeight * scale};

    layout->tutorial.silenceBtnRec = (Rectangle){
        layout->tutorial.silenceBtnPos.x,
        layout->tutorial.silenceBtnPos.y,
        silenceBase * scale,
        silenceHeight * scale};

    layout->tutorial.tutorialBtnRec = (Rectangle){
        layout->tutorial.tutorialBtnPos.x,
        layout->tutorial.tutorialBtnPos.y,
        tutorialBase * scale,
        tutorialHeight * scale};

    layout->tutorial.nextBtnRec = (Rectangle){
        offsetX + (45.0f * scale),
        offsetY + (280.0f * scale),
        147.0f * scale,
        147.0f * scale};

    state->blinkState.blinkRec = (Rectangle){
        offsetX,
        offsetY,
        BASE_WIDTH * scale,
        BASE_HEIGHT * scale};

    // ------------------ MATH OP WINDOW POSITION BOOK ------------------
    float columnWidth = 150;
    float columnHeight = 60;
    float columnPosX = 1010;
    float columnPosY = 440;
    float gapY = 62.0f;

    for (int i = 0; i < 4; i++)
    {
        layout->math.bookMathAnswersRec[i] = (Rectangle){
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
        layout->math.chalkboardMathAnswersRec[i] = (Rectangle){
            offsetX + (centerColumnPosX * scale),
            offsetY + ((centerColumnPosY + (i * centerGapY)) * scale),
            centerColumnWidth * scale,
            centerColumnHeight * scale};
    }
}
