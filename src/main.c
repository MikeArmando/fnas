#include "raylib.h"
#include "resource_dir.h"
#include "resources.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetTargetFPS(60);

	InitWindow(1250, 750, "Five Nigths At School");

	SearchAndSetResourceDir("resources");

	InitAudioDevice();

	// ----------------- Load Resources ---------------------
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
	float arrowSize = 80.0f;
	float helperSize = 120.0f;
	float entitySize = 60.0f;

	// Center Arrow
	float scaleCenterArrow = arrowSize / (float)res->centerArrow.width;
	Vector2 centerArrowPos = {580, 480};
	// Left Arrow
	float scaleLeftArrow = arrowSize / (float)res->leftArrow.width;
	Vector2 leftArrowPos = {120, 530};
	// Rigth Arrow
	float scaleRightArrow = arrowSize / (float)res->rightArrow.width;
	Vector2 rightArrowPos = {1100, 500};
	// Return Arrow
	Vector2 returnArrowPos = {550, 650};
	// Snail
	float scaleHelper = helperSize / (float)res->helper.width;
	Vector2 helperPos = {50, 630};
	// Entity
	float scaleEntity = entitySize / (float)res->helper.width;
	Vector2 LeftEntityPos = {50, 430};
	Vector2 centerEntityPos = {550, 400};
	Vector2 rightEntityPos = {1040, 430};

	// --------------------- Background on objects position for clicking ---------------------
	Rectangle centerArrowRec = {centerArrowPos.x, centerArrowPos.y, res->centerArrow.width * scaleCenterArrow, res->centerArrow.height * scaleCenterArrow};
	Rectangle leftArrowRec = {leftArrowPos.x, leftArrowPos.y, res->leftArrow.width * scaleLeftArrow, res->leftArrow.height * scaleLeftArrow};
	Rectangle rightArrowRec = {rightArrowPos.x, rightArrowPos.y, res->rightArrow.width * scaleRightArrow, res->rightArrow.height * scaleRightArrow};
	Rectangle returnArrowRec = {returnArrowPos.x, returnArrowPos.y, res->centerArrow.width * scaleCenterArrow, res->centerArrow.height * scaleCenterArrow};
	Rectangle snailRec = {helperPos.x, helperPos.y, res->helper.width * scaleHelper, res->helper.height * scaleHelper};

	// --------------------- Screen Variables ---------------------
	int isStartScreen = 1;
	int isLostScreen = 0;
	int isWonScreen = 0;
	int isClassroom = 0;
	int isCenterTaskWindow = 0;
	int isLeftTaskWindow = 0;
	int isRightTaskWindow = 0;
	int isSnailWindow = 0;
	int startTask = 0;

	// Entity list
	Texture2D entityList[3];
	entityList[0] = LoadTexture("objects/monster.jpg");
	entityList[1] = LoadTexture("objects/snail.png");
	entityList[2] = LoadTexture("objects/girl.jpg");

	const float interval = 5.0f; // Seconds
	float timer = 0.0f;
	int newIndex;

	// --------------------- Inicial Music ---------------------
	PlayMusicStream(res->introMusic); // Starting music (Needs to be here)

	// --------------------- Game loop ---------------------
	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();
		if (!isStartScreen)
		{
			// Timer
			float deltatime = GetFrameTime();
			timer += deltatime;
			if (timer >= interval)
			{
				timer -= interval;
				newIndex = GetRandomValue(0, 2);
				startTask = 1;
			}
		}

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
				isCenterTaskWindow = 1;
			}
		}
		if (CheckCollisionPointRec(mousePos, leftArrowRec))
		{
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				isClassroom = 0;
				isLeftTaskWindow = 1;
			}
		}
		if (CheckCollisionPointRec(mousePos, rightArrowRec))
		{
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				isClassroom = 0;
				isRightTaskWindow = 1;
			}
		}
		if (CheckCollisionPointRec(mousePos, returnArrowRec))
		{
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
			{
				isCenterTaskWindow = 0;
				isLeftTaskWindow = 0;
				isRightTaskWindow = 0;
				isClassroom = 1;
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
			DrawTextureEx(res->helper, helperPos, 0.0f, scaleHelper, WHITE);
		}
		if (startTask)
		{
			if (newIndex == 0)
			{
				DrawTextureEx(entityList[newIndex], LeftEntityPos, 0.0f, scaleEntity, WHITE);
			}
			if (newIndex == 1)
			{
				DrawTextureEx(entityList[newIndex], centerEntityPos, 0.0f, scaleEntity, WHITE);
			}
			if (newIndex == 2)
			{
				PlaySound(res->doorKnocking);
				DrawTextureEx(entityList[newIndex], rightEntityPos, 0.0f, scaleEntity, WHITE);
			}

			startTask = 0; // ONCE ITS COMPLETED
		}

		if (isCenterTaskWindow)
		{
			DrawTextureEx(res->helper, helperPos, 0.0f, scaleHelper, WHITE);
			DrawTextureEx(res->centerTaskWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
			DrawTextureEx(res->returnArrow, returnArrowPos, 0.0f, scaleCenterArrow, WHITE);
			DrawText("5 + 5 = ?", 500, 340, 50, WHITE);
		}
		if (isLeftTaskWindow)
		{
			DrawTextureEx(res->helper, helperPos, 0.0f, scaleHelper, WHITE);
			DrawTextureEx(res->leftTaskWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
			DrawTextureEx(res->returnArrow, returnArrowPos, 0.0f, scaleCenterArrow, WHITE);
		}
		if (isRightTaskWindow)
		{
			DrawTextureEx(res->helper, helperPos, 0.0f, scaleHelper, WHITE);
			DrawTextureEx(res->rightTaskWindow, backgroundPosFit, 0.0f, scaleFit, WHITE);
			DrawTextureEx(res->returnArrow, returnArrowPos, 0.0f, scaleCenterArrow, WHITE);
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