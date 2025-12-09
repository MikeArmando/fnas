#include "raylib.h"
#include "resource_dir.h"
#include "resources.h"
#include "state.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);
	InitWindow(1250, 750, "Five Nigths At School");
	SearchAndSetResourceDir("resources");
	InitAudioDevice();

	// ----------------- Load Resources And State -----------------
	resourcesLoad();
	const Ts_resources *res = resourcesGet();

	Ts_GameState state;
	GameStateConfig(res, &state);

	// ----------------- Inicialize Music -----------------
	PlayMusicStream(res->music.intro);
	PlayMusicStream(res->music.background);

	// ----------------- Inicialize State -----------------
	ChangeGameState(LogicStartScreen, DrawStartScreen);

	// ----------------- Game Loop -----------------
	while (!WindowShouldClose())
	{
		if (IsWindowResized())
		{
			ResourcesLayout(res, &state);
		}

		// ----------------- Logic Section -----------------
		GameLogicFunction currentLogic = GetCurrentLogic();
		if (currentLogic)
		{
			currentLogic(res, &state);
		}

		bool skipGameLogic = (currentLogic == LogicStartScreen || currentLogic == LogicLostScreen ||
							  currentLogic == LogicWonScreen || currentLogic == LogicTutorial);

		if (!skipGameLogic)
		{
			UpdateMusicStream(res->music.monoV1);

			UpdateMusicStream(res->music.background);

			playerIdleTimer(res, &state);

			StartTimerTask(res, &state);
		}

		// Monologue
		float monoTimePlayed = GetMusicTimePlayed(res->music.monoV1);
		if (state.monoLen > 0.001f)
		{
			if (monoTimePlayed + 0.05f >= state.monoLen)
			{
				StopMusicStream(res->music.monoV1);
			}
		}

		UpdateBlink();

		// ----------------- Drawing Section -----------------
		BeginDrawing();
		ClearBackground(BLACK);
		GameDrawFunction currentDraw = GetCurrentDraw();
		if (currentDraw)
		{
			currentDraw(res, &state);
			DrawBlink(&state);
		}
		EndDrawing();
	}

	// ----------------- Clean Up -----------------
	resourcesUnload();
	CloseAudioDevice();
	CloseWindow();
	return 0;
}