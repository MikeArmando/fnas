#include "raylib.h"
#include "resource_dir.h"
#include <math.h>
#include "resources.h"

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetTargetFPS(60);

	InitWindow(1175, 730, "Five Nigths At School");

	SearchAndSetResourceDir("resources");

	InitAudioDevice();

	// ----------------- Load resources ---------------------
	resources_load();
	const Ts_resources *res = resources_get();

	// --------------------- Background Config ---------------------
	float sx = (float)GetScreenWidth() / (float)res->classroomWindow.width;
	float sy = (float)GetScreenHeight() / (float)res->classroomWindow.height;

	// FIT
	float scaleFit = fminf(sx, sy);
	Vector2 backgroundPosFit = {(GetScreenWidth() - res->classroomWindow.width * scaleFit) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * scaleFit) / 2.0f};

	// COVER
	float scaleCover = fmaxf(sx, sy);
	Vector2 bgPosCover = {(GetScreenWidth() - res->classroomWindow.width * scaleCover) / 2.0f, (GetScreenHeight() - res->classroomWindow.height * scaleCover) / 2.0f};

	// Tutorial Window
	Vector2 tutorialWindowPos = {300, 200};

	// --------------------- Objects Config ---------------------
	// Scale to pixel size
	float arrowSize = 60.0f;
	float snailSize = 120.0f;

	// Center Arrow
	float scaleCenterArrow = arrowSize / (float)res->centerArrow.width;
	Vector2 centerArrowPos = {550, 480};
	// Left Arrow
	float scaleLeftArrow = arrowSize / (float)res->leftArrow.width;
	Vector2 leftArrowPos = {120, 500};
	// Rigth Arrow
	float scaleRightArrow = arrowSize / (float)res->rightArrow.width;
	Vector2 rightArrowPos = {950, 500};
	// Snail
	float scaleSnail = snailSize / (float)res->snail.width;
	Vector2 SnailPos = {50, 630};

	// --------------------- Background on objects position for clicking ---------------------
	Rectangle centerArrowRec = {centerArrowPos.x, centerArrowPos.y, res->centerArrow.width * scaleCenterArrow, res->centerArrow.height * scaleCenterArrow};
	Rectangle leftArrowRec = {leftArrowPos.x, leftArrowPos.y, res->leftArrow.width * scaleLeftArrow, res->leftArrow.height * scaleLeftArrow};
	Rectangle rightArrowRec = {rightArrowPos.x, rightArrowPos.y, res->rightArrow.width * scaleRightArrow, res->rightArrow.height * scaleRightArrow};
	Rectangle snailRec = {SnailPos.x, SnailPos.y, res->snail.width * scaleSnail, res->snail.height * scaleSnail};

	// --------------------- Music Config ---------------------
	PlayMusicStream(res->introMusic); // Starting music (Needs to be here)

	// --------------------- Screen Variables ---------------------
	int isStartScreen = 1;
	int isLostScreen = 0;
	int isWonScreen = 0;
	int isClassroom = 0;
	int isCenterWindow = 0;
	int isLeftWindow = 0;
	int isRightWindow = 0;
	int isSnailWindow = 0;

	// --------------------- Game loop ---------------------
	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();

		UpdateMusicStream(res->introMusic);
		UpdateMusicStream(res->m1Track);
		UpdateMusicStream(res->monoV1);

		// ----------------- Logic Section -----------------
		if (IsKeyPressed(KEY_ENTER))
		{
			StopMusicStream(res->introMusic);
			PlayMusicStream(res->m1Track);
			PlayMusicStream(res->monoV1);

			isStartScreen = 0;
			isClassroom = 1;
		}

		if (isClassroom)
		{
		}

		if (CheckCollisionPointRec(mousePos, centerArrowRec))
		{
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				isClassroom = 0;
				isCenterWindow = 1;
			}
		}
		if (CheckCollisionPointRec(mousePos, leftArrowRec))
		{
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				isClassroom = 0;
				isLeftWindow = 1;
			}
		}
		if (CheckCollisionPointRec(mousePos, rightArrowRec))
		{
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				isClassroom = 0;
				isRightWindow = 1;
			}
		}
		if (CheckCollisionPointRec(mousePos, snailRec))
		{
			isSnailWindow = 1;
		}
		else
		{
			isSnailWindow = 0;
		}

		float monoV1Played = GetMusicTimePlayed(res->monoV1);
		float monoV1Len = GetMusicTimeLength(res->monoV1);
		if (monoV1Len > 0.001f && monoV1Played + 0.05f >= monoV1Len)
		{
			StopMusicStream(res->monoV1);
		}

		// ----------------- Drawing Section -----------------
		BeginDrawing();
		if (isStartScreen)
		{
			ClearBackground(BLACK);
			DrawText("Five Nigths At School", 150, 200, 60, WHITE);
			DrawText("Presiona [ENTER] para comenzar . . .", 150, 270, 20, WHITE);
		}
		if (isClassroom)
		{
			DrawTextureEx(res->classroomWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
			DrawTextureEx(res->centerArrow, centerArrowPos, 0.0f, scaleCenterArrow, WHITE);
			DrawTextureEx(res->leftArrow, leftArrowPos, 0.0f, scaleLeftArrow, WHITE);
			DrawTextureEx(res->rightArrow, rightArrowPos, 0.0f, scaleRightArrow, WHITE);
			DrawTextureEx(res->snail, SnailPos, 0.0f, scaleSnail, WHITE);
		}
		if (isCenterWindow)
		{
			DrawTextureEx(res->centerTaskWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
		}
		if (isLeftWindow)
		{
			DrawTextureEx(res->leftTaskWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
		}
		if (isRightWindow)
		{
			DrawTextureEx(res->rightTaskWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
		}
		if (isLostScreen)
		{
			ClearBackground(BLACK);
			DrawText("GAME OVER", 150, 200, 60, WHITE);
			DrawText("Cause of death: you suck at math bruah", 150, 265, 20, WHITE);
		}
		if (isWonScreen)
		{
			ClearBackground(BLACK);
			DrawText("Good job kid!", 150, 200, 60, WHITE);
			DrawText("You survived the night", 150, 265, 20, WHITE);
		}
		if (isSnailWindow)
		{
			DrawTextureEx(res->tutorialWindow, tutorialWindowPos, 0.0f, 0.5f, WHITE);
		}

		EndDrawing();
	}

	// --------------------- Clean Up ---------------------
	resources_unload();

	CloseAudioDevice();

	CloseWindow();
	return 0;
}