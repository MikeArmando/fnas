#include "raylib.h"
#include "resource_dir.h"
#include "resources.h"
#include "state.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(60);
	InitWindow(1250, 750, "Five Nigths At School");
	SearchAndSetResourceDir("resources");
	InitAudioDevice();

	// ----------------- Load Resources And State -----------------
	resourcesLoad();
	const Ts_resources *res = resourcesGet();
	Ts_GameState gameState = GameStateConfig(res);

	// ----------------- Inicialize Music -----------------
	PlayMusicStream(res->intro);
	PlayMusicStream(res->background);

	// ----------------- Inicialize State -----------------
	ChangeGameState(LogicStartScreen, DrawStartScreen);

	// ----------------- Game Loop -----------------
	while (!WindowShouldClose())
	{
		UpdateMusicStream(res->monoV1);

		// ----------------- Logic Section -----------------
		GameLogicFunction currentLogic = GetCurrentLogic();
		if (currentLogic)
		{
			currentLogic(res, &gameState);
		}

		if (currentLogic != LogicStartScreen)
		{
			if (currentLogic != LogicLostScreen)
			{
				if (currentLogic != LogicWonScreen)
				{
					UpdateMusicStream(res->background);

					// Start task
					StartTimerTask(res, &gameState);

					// Idle Timer
					playerIdleTimer(res, &gameState);
				}
			}
		}

		// Monologue
		float monoTimePlayed = GetMusicTimePlayed(res->monoV1);
		if (gameState.monoLen > 0.001f)
		{
			if (monoTimePlayed + 0.05f >= gameState.monoLen)
			{
				StopMusicStream(res->monoV1);
			}
		}

		UpdateBlink();

		// ----------------- Drawing Section -----------------
		BeginDrawing();
		GameDrawFunction currentDraw = GetCurrentDraw();
		if (currentDraw)
		{
			currentDraw(res, &gameState);
			DrawBlink();
		}
		EndDrawing();
	}

	// ----------------- Clean Up -----------------
	resourcesUnload();
	CloseAudioDevice();
	CloseWindow();
	return 0;
}