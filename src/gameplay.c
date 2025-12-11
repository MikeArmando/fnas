#include <stdio.h>
#include <stdlib.h>
#include "gameplay.h"
#include <float.h>

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
            TriggerBlink(state, 7.0f, BLACK, 0.0f);

            SaveGameResults(state, 1);
            state->player.bestTime = LoadBestTime();
            ChangeGameState(LogicWonScreen, DrawWonScreen);
            return;
        }

        PlaySound(*currentTask->successSound);
        TriggerBlink(state, 0.2f, BLACK, 0.0f);
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

        TriggerBlink(state, 0.1f, RED, 0.0f);
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
    task->num2 = GetRandomValue(1, task->num1);
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

        } while (isDuplicate || wrongAnswer == task->correctAnswer || wrongAnswer < 0);

        task->choicesList[i] = wrongAnswer;
    }
}

// --------------------------------------- BLINK EFFECT ---------------------------------------
void TriggerBlink(Ts_GameState *state, float duration, Color color, float frequency)
{
    Ts_BlinkEffect *Blink = &state->blinkState;

    Blink->active = true;
    Blink->duration = duration;
    Blink->currentTimer = duration;
    Blink->color = color;
    Blink->frequency = frequency;
}

void UpdateBlink(Ts_GameState *state)
{
    Ts_BlinkEffect *Blink = &state->blinkState;

    if (Blink->active)
    {
        Blink->currentTimer -= GetFrameTime();

        if (Blink->currentTimer <= 0)
        {
            Blink->active = false;
            Blink->currentTimer = 0;
        }
    }
}

void DrawBlink(Ts_GameState *state)
{
    Ts_BlinkEffect *Blink = &state->blinkState;

    if (Blink->active)
    {
        if (Blink->frequency <= 0.0f)
        {
            float alpha = Blink->currentTimer / Blink->duration;

            DrawRectangleRec(state->blinkState.blinkRec, Fade(Blink->color, alpha));
            return;
        }

        float timeElapsed = Blink->duration - Blink->currentTimer;
        float cycleInterval = 1.0f / Blink->frequency;
        float cycleTime = fmodf(timeElapsed, cycleInterval);

        if (cycleTime < (cycleInterval / 2.0f))
        {
            DrawRectangleRec(state->blinkState.blinkRec, Fade(Blink->color, 0.5f));
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
        TriggerBlink(state, 6.0f, BLACK, 0.0f);
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