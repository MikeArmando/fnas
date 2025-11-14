#include "raylib.h"
#include "resource_dir.h"
#include "resources.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

typedef struct _GameState
{
	// Config drawings
	float scaleFit;
	Vector2 backgroundPosFit;
	Rectangle centerArrowRec;
	Rectangle leftArrowRec;
	Rectangle rightArrowRec;
	Rectangle returnArrowRec;
	Rectangle snailRec;

	// Positions and scale
	float scaleCenterArrow;
	Vector2 centerArrowPos;

	float scaleLeftArrow;
	Vector2 leftArrowPos;

	float scaleRightArrow;
	Vector2 rightArrowPos;

	Vector2 returnArrowPos;

	float scaleHelper;
	Vector2 helperPos;

	Vector2 tutorialWindowPos;

	float scaleEntity;
	Vector2 LeftEntityPos;
	Vector2 centerEntityPos;
	Vector2 rightEntityPos;

	// Variables of state
	int isSnailWindow;
	int startTask;
	float interval;
	float timer;
	int newIndex;
	Texture2D entityList[3];

} Ts_GameState;

// ------------ Prototyps ------------
typedef void (*GameLogicFunction)(const Ts_resources *res, Ts_GameState *state);
typedef void (*GameDrawFunction)(const Ts_resources *res, Ts_GameState *state);

void LogicStartScreen(const Ts_resources *res, Ts_GameState *state);
void DrawStartScreen(const Ts_resources *res, Ts_GameState *state);

void LogicClassroom(const Ts_resources *res, Ts_GameState *state);
void DrawClassroom(const Ts_resources *res, Ts_GameState *state);

void LogicCenterTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawCenterTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state);
void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state);

void LogicLostScreen(const Ts_resources *res, Ts_GameState *state);
void DrawLostScreen(const Ts_resources *res, Ts_GameState *state);

void LogicWonScreen(const Ts_resources *res, Ts_GameState *state);
void DrawWonScreen(const Ts_resources *res, Ts_GameState *state);

void startTask(const Ts_resources *res, Ts_GameState *state);

void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw);

GameLogicFunction currentLogic = 0;
GameDrawFunction currentDraw = 0;

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetTargetFPS(60);

	InitWindow(1250, 750, "Five Nigths At School");

	SearchAndSetResourceDir("resources");

	InitAudioDevice();

	Ts_GameState gameState = {0};

	// ----------------- Load Resources ---------------------
	resources_load();
	const Ts_resources *res = resources_get();

	// --------------------- Background Config ---------------------
	float sx = (float)GetScreenWidth() / (float)res->classroomWindow.width;
	float sy = (float)GetScreenHeight() / (float)res->classroomWindow.height;

	// FIT
	gameState.scaleFit = fminf(sx, sy);

	gameState.backgroundPosFit = (Vector2){(GetScreenWidth() - res->classroomWindow.width * gameState.scaleFit) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * gameState.scaleFit) / 2.0f};

	// COVER
	float scaleCover = fmaxf(sx, sy);
	Vector2 bgPosCover = {(GetScreenWidth() - res->classroomWindow.width * scaleCover) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * scaleCover) / 2.0f};

	// Tutorial Window
	gameState.tutorialWindowPos = (Vector2){300, 200};

	// --------------------- Objects Config ---------------------
	// Scale to pixel size
	float arrowSize = 50.0f;
	float helperSize = 90.0f;
	float entitySize = 60.0f;

	// Center Arrow
	gameState.scaleCenterArrow = arrowSize / (float)res->centerArrow.width;
	gameState.centerArrowPos = (Vector2){602, 500};
	// Left Arrow
	gameState.scaleLeftArrow = arrowSize / (float)res->leftArrow.width;
	gameState.leftArrowPos = (Vector2){122, 500};
	// Rigth Arrow
	gameState.scaleRightArrow = arrowSize / (float)res->rightArrow.width;
	gameState.rightArrowPos = (Vector2){1050, 500};
	// Return Arrow
	gameState.returnArrowPos = (Vector2){602, 650};
	// Snail
	gameState.scaleHelper = helperSize / (float)res->helper.width;
	gameState.helperPos = (Vector2){50, 660};
	// Entity
	gameState.scaleEntity = entitySize / (float)res->helper.width;
	gameState.LeftEntityPos = (Vector2){50, 430};
	gameState.centerEntityPos = (Vector2){550, 400};
	gameState.rightEntityPos = (Vector2){1040, 430};

	// --------------------- Colision Objects position ---------------------
	gameState.centerArrowRec = (Rectangle){gameState.centerArrowPos.x, gameState.centerArrowPos.y, res->centerArrow.width * gameState.scaleCenterArrow, res->centerArrow.height * gameState.scaleCenterArrow};
	gameState.leftArrowRec = (Rectangle){gameState.leftArrowPos.x, gameState.leftArrowPos.y, res->leftArrow.width * gameState.scaleLeftArrow, res->leftArrow.height * gameState.scaleLeftArrow};
	gameState.rightArrowRec = (Rectangle){gameState.rightArrowPos.x, gameState.rightArrowPos.y, res->rightArrow.width * gameState.scaleRightArrow, res->rightArrow.height * gameState.scaleRightArrow};
	gameState.returnArrowRec = (Rectangle){gameState.returnArrowPos.x, gameState.returnArrowPos.y, res->centerArrow.width * gameState.scaleCenterArrow, res->centerArrow.height * gameState.scaleCenterArrow};
	gameState.snailRec = (Rectangle){gameState.helperPos.x, gameState.helperPos.y, res->helper.width * gameState.scaleHelper, res->helper.height * gameState.scaleHelper};

	// Entity list
	Texture2D entityList[3];
	entityList[0] = LoadTexture("objects/monster.jpg");
	entityList[1] = LoadTexture("objects/snail.png");
	entityList[2] = LoadTexture("objects/girl.jpg");

	// Timer Setup
	gameState.interval = 5.0f; // Seconds
	gameState.timer = 0.0f;
	gameState.newIndex;

	// --------------------- Inicial Music ---------------------
	PlayMusicStream(res->introMusic); // Starting music (Needs to be here)
	ChangeGameState(LogicStartScreen, DrawStartScreen);

	// --------------------- Game loop ---------------------
	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();

		UpdateMusicStream(res->introMusic);
		UpdateMusicStream(res->m1Track);
		UpdateMusicStream(res->monoV1);

		// ----------------- Logic Section -----------------

		if (currentLogic != NULL)
		{
			currentLogic(res, &gameState);
		}

		float monoV1Played = GetMusicTimePlayed(res->monoV1);
		float monoV1Len = GetMusicTimeLength(res->monoV1);
		if (monoV1Len > 0.001f && monoV1Played + 0.05f >= monoV1Len)
		{
			StopMusicStream(res->monoV1);
		}

		// ----------------- Drawing Section -----------------

		BeginDrawing();

		if (currentDraw != NULL)
		{
			currentDraw(res, &gameState);
		}

		EndDrawing();
	}

	// --------------------- Clean Up ---------------------
	resources_unload();

	CloseAudioDevice();

	CloseWindow();
	return 0;
}

// ------------------------------------------------------------------
void ChangeGameState(GameLogicFunction newLogic, GameDrawFunction newDraw)
{
	currentLogic = newLogic;
	currentDraw = newDraw;
}

// ------------------------------------------------------------------
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

// ------------------------------------------------------------------
void LogicClassroom(const Ts_resources *res, Ts_GameState *state)
{
	Vector2 mousePos = GetMousePosition();

	if (CheckCollisionPointRec(mousePos, state->centerArrowRec))
	{
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			ChangeGameState(LogicCenterTaskWindow, DrawCenterTaskWindow);
			return;
		}
	}
	if (CheckCollisionPointRec(mousePos, state->leftArrowRec))
	{
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			ChangeGameState(LogicLeftTaskWindow, DrawLeftTaskWindow);
			return;
		}
	}
	if (CheckCollisionPointRec(mousePos, state->rightArrowRec))
	{
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
		{
			ChangeGameState(LogicRightTaskWindow, DrawRightTaskWindow);
			return;
		}
	}

	if (CheckCollisionPointRec(mousePos, state->snailRec))
	{
		state->isSnailWindow = 1;
	}
	else
	{
		state->isSnailWindow = 0;
	}

	// Start task
	startTask(res, state);
}

void DrawClassroom(const Ts_resources *res, Ts_GameState *state)
{
	DrawTextureEx(res->classroomWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
	DrawTextureEx(res->centerArrow, state->centerArrowPos, 0.0f, state->scaleCenterArrow, WHITE);
	DrawTextureEx(res->leftArrow, state->leftArrowPos, 0.0f, state->scaleLeftArrow, WHITE);
	DrawTextureEx(res->rightArrow, state->rightArrowPos, 0.0f, state->scaleRightArrow, WHITE);
	DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
	if (state->isSnailWindow)
	{
		DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
	}
}

// ------------------------------------------------------------------
void LogicCenterTaskWindow(const Ts_resources *res, Ts_GameState *state)
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

void DrawCenterTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
	DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
	DrawTextureEx(res->centerTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
	DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleCenterArrow, WHITE);
	if (state->isSnailWindow)
	{
		DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
	}
}

// ------------------------------------------------------------------
void LogicLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
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

void DrawLeftTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
	DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
	DrawTextureEx(res->leftTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
	DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleCenterArrow, WHITE);
	if (state->isSnailWindow)
	{
		DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
	}
}

// ------------------------------------------------------------------
void LogicRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
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

void DrawRightTaskWindow(const Ts_resources *res, Ts_GameState *state)
{
	DrawTextureEx(res->helper, state->helperPos, 0.0f, state->scaleHelper, WHITE);
	DrawTextureEx(res->rightTaskWindow, state->backgroundPosFit, 0.0f, state->scaleFit, WHITE);
	DrawTextureEx(res->returnArrow, state->returnArrowPos, 0.0f, state->scaleCenterArrow, WHITE);
	if (state->isSnailWindow)
	{
		DrawTextureEx(res->tutorialWindow, state->tutorialWindowPos, 0.0f, 0.5f, WHITE);
	}
}

// ------------------------------------------------------------------
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

// ------------------------------------------------------------------
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

// ------------------------------------------------------------------
void startTask(const Ts_resources *res, Ts_GameState *state)
{
	// Clock
	float deltatime = GetFrameTime();
	state->timer += deltatime;
	if (state->timer >= state->interval)
	{
		state->timer -= state->interval;
		state->newIndex = GetRandomValue(0, 2);
		state->startTask = 1;
	}

	if (state->startTask == 1)
	{
		if (state->newIndex == 0)
		{
			PlaySound(res->knockingOnWindow);
		}
		if (state->newIndex == 1)
		{
			PlaySound(res->writingOnBoard);
		}
		if (state->newIndex == 2)
		{
			PlaySound(res->girlLaugh);
		}
	}
}
