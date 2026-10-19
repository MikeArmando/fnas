#include "config.h"
#include "state.h"
#include "resources.h"
#include "gameplay.h"
#include <math.h>

#define BASE_WIDTH 1250.0f
#define BASE_HEIGHT 750.0f

// With a smaller range assignWrongChoices can run out of candidates (e.g. correct answer 0) and never end.
#define MIN_DISTRACTOR_RANGE 3
// Seconds between the end of the safe time and the first task of a night.
#define FIRST_SPAWN_DELAY 8.0f

// --------------------------------------- LEVELS ---------------------------------------
// One row per night. Night 1 reproduces the original behavior.
static const Ts_LevelConfig levels[] = {
    {
        .name = "NOCHE 1",
        .hasNarration = true,
        .safeTime = 52.0f,
        .spawnMin = 5,
        .spawnMax = 10,
        .spawnWeight = {30, 50}, // right takes the remainder (20)
        .maxCorrectPoints = 10,
        .maxIncorrectPoints = 5,
        .idleLimit = 60.0f,
        .maxOperand = {100, 10, 100}, // left sum, center mult, right sub
        .distractorRange = 5,
    },
    {
        .name = "NOCHE 2",
        .hasNarration = false,
        .safeTime = 8.0f,
        .spawnMin = 5,
        .spawnMax = 10,
        .spawnWeight = {30, 50},
        .maxCorrectPoints = 8,
        .maxIncorrectPoints = 5,
        .idleLimit = 60.0f,
        .maxOperand = {150, 10, 150},
        .distractorRange = 5,
    },
    {
        .name = "NOCHE 3",
        .hasNarration = false,
        .safeTime = 8.0f,
        .spawnMin = 5,
        .spawnMax = 10,
        .spawnWeight = {25, 35},
        .maxCorrectPoints = 8,
        .maxIncorrectPoints = 4,
        .idleLimit = 60.0f,
        .maxOperand = {150, 12, 150},
        .distractorRange = 5,
        .patience = {65.0f, 65.0f, 80.0f},
    },
    {
        .name = "NOCHE 4",
        .hasNarration = false,
        .safeTime = 8.0f,
        .spawnMin = 4,
        .spawnMax = 8,
        .spawnWeight = {25, 35},
        .maxCorrectPoints = 8,
        .maxIncorrectPoints = 4,
        .idleLimit = 60.0f,
        .maxOperand = {200, 12, 200},
        .distractorRange = 3,
        .patience = {60.0f, 60.0f, 75.0f},
    },
    {
        .name = "NOCHE 5",
        .hasNarration = false,
        .safeTime = 8.0f,
        .spawnMin = 3,
        .spawnMax = 7,
        .spawnWeight = {30, 40},
        .maxCorrectPoints = 8,
        .maxIncorrectPoints = 3,
        .idleLimit = 60.0f,
        .maxOperand = {300, 12, 300},
        .distractorRange = 3,
        .patience = {55.0f, 55.0f, 70.0f},
    },
};

int GetLevelCount(void)
{
    return (int)(sizeof(levels) / sizeof(levels[0]));
}

// --------------------------------------- GAME CONFIG ---------------------------------------
void GameStateConfig(const Ts_resources *res, Ts_GameState *state)
{
    // ------------------ TASK INITIAL SETUP ------------------
    // One entry per task: what varies between tasks lives here, the logic is shared.
    // spawnWeight comes from the level table (set below).
    state->mathTask[TASK_LEFT] = (Ts_MathTaskData){
        .pickOperands = PickSumOperands,
        .spawnSound = &res->sound.knockingOnWindow,
        .successSound = &res->sound.eatingCookie,
        .jumpscareMessage = "No le diste galletas...",
        .returnLogic = LogicClassroom,
        .returnDraw = DrawClassroom};

    state->mathTask[TASK_CENTER] = (Ts_MathTaskData){
        .pickOperands = PickMultOperands,
        .spawnSound = &res->sound.ghostWriting,
        .successSound = &res->sound.answerOnChalkboard,
        .jumpscareMessage = "El fantasma se canso de esperar...",
        .returnLogic = LogicClassroom,
        .returnDraw = DrawClassroom};

    state->mathTask[TASK_RIGHT] = (Ts_MathTaskData){
        .pickOperands = PickSubOperands,
        // no spawn sound: the girl is only revealed with the flashlight
        .successSound = &res->sound.girlLaugh,
        .jumpscareMessage = "Su risa fue lo ultimo que escuchaste...",
        .returnLogic = LogicRightTask,
        .returnDraw = DrawRightTask};

    // ------------------ LEVEL SETUP ------------------
    StartLevel(state, 0);

    state->layout.startAndLost.currentGameOverMessage = "Bro?? Todo bien?";

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

// --------------------------------------- LEVEL START ---------------------------------------
// Selects the level at levelIndex, applies it to the runtime fields and resets everything a night depends on.
void StartLevel(Ts_GameState *state, int levelIndex)
{
    state->currentLevel = levelIndex;
    state->level = &levels[levelIndex];
    state->levelIntroTimer = 0.0f;

    const Ts_LevelConfig *level = state->level;

    state->task.maxCorrectPoints = level->maxCorrectPoints;
    state->task.maxIncorrectPoints = level->maxIncorrectPoints;

    int distractorRange = level->distractorRange;
    if (distractorRange < MIN_DISTRACTOR_RANGE)
    {
        TraceLog(LOG_WARNING, "LEVEL: distractorRange %d is below %d, using %d", distractorRange, MIN_DISTRACTOR_RANGE, MIN_DISTRACTOR_RANGE);
        distractorRange = MIN_DISTRACTOR_RANGE;
    }

    for (int i = 0; i < TASK_COUNT; i++)
    {
        Ts_MathTaskData *task = &state->mathTask[i];
        task->maxOperand = level->maxOperand[i];
        task->distractorRange = distractorRange;
        task->patience = level->patience[i];
        task->waitTime = 0.0f;
        task->isActive = false;

        // The last task has no weight: it takes the remainder.
        if (i < TASK_COUNT - 1)
        {
            task->spawnWeight = level->spawnWeight[i];
        }
    }

    state->task.currentTime = 0.0f;
    state->task.interval = FIRST_SPAWN_DELAY;

    state->player.incorrectPoints = 0;
    state->player.correctPoints = 0;
    state->player.idleTime = 0.0f;
    state->player.safeTime = 0.0f;

    state->helper.isHelpWindow = 0;
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