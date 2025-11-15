#include "raylib.h"
#include "resource_dir.h"
#include "resources.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(60);
	InitWindow(1250, 750, "Five Nigths At School");
	SearchAndSetResourceDir("resources");
	InitAudioDevice();

	// ----------------- Load Resources And Config---------------------
	resources_load();
	const Ts_resources *res = resources_get();
	Ts_GameState gameState = GameState_config(res);

	// --------------------- Inicial Music ---------------------
	PlayMusicStream(res->introMusic);

	// --------------------- Inicialize State ---------------------
	ChangeGameState(LogicStartScreen, DrawStartScreen);

	// --------------------- Game Loop ---------------------
	while (!WindowShouldClose())
	{
		UpdateMusicStream(res->introMusic);
		UpdateMusicStream(res->m1Track);
		UpdateMusicStream(res->monoV1);

		// ----------------- Logic Section -----------------
		GameLogicFunction currentLogic = GetCurrentLogic();
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
		GameDrawFunction currentDraw = GetCurrentDraw();
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