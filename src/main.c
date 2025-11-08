#include "raylib.h"
#include "resource_dir.h"
#include <math.h> // fminf, fmaxf

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetTargetFPS(60);

	InitWindow(1175, 730, "Five Nigths At School");

	SearchAndSetResourceDir("resources");

	InitAudioDevice();

	// ----------------- Load resources ---------------------
	// Backgrounds
	Texture2D classroomBackground = LoadTexture("defualtClassroom.png");
	Texture2D windowBackground = LoadTexture("ventana.jpg");
	Texture2D tutorialWindow = LoadTexture("tutorial.jpg");

	// Objects
	Texture2D centerArrow = LoadTexture("arrow.png");
	Texture2D leftArrow = LoadTexture("arrow.png");
	Texture2D rightArrow = LoadTexture("arrow.png");
	Texture2D snail = LoadTexture("snail.png");

	// Music
	Music mainMusic = LoadMusicStream("horrorMain.mp3");
	Music introMusic = LoadMusicStream("intro.mp3");
	Music v1 = LoadMusicStream("v1.mp3");

	// --------------------- Background Config ---------------------
	float sx = (float)GetScreenWidth() / (float)classroomBackground.width;
	float sy = (float)GetScreenHeight() / (float)classroomBackground.height;

	// FIT
	float scaleFit = fminf(sx, sy);
	Vector2 backgroundPosFit = {(GetScreenWidth() - classroomBackground.width * scaleFit) / 2.0f, (GetScreenHeight() - classroomBackground.height * scaleFit) / 2.0f};

	// COVER
	float scaleCover = fmaxf(sx, sy);
	Vector2 bgPosCover = {(GetScreenWidth() - classroomBackground.width * scaleCover) / 2.0f, (GetScreenHeight() - classroomBackground.height * scaleCover) / 2.0f};

	// Tutorial window
	Vector2 tutorialWindowPos = {300, 200};

	// --------------------- Objects ---------------------
	// Escalar a un tamaño pixel exacto
	float arrowWidth = 60.0f;
	float snailWidth = 130.0f;

	// Arrow Center
	float scaleCenterArrow = arrowWidth / (float)centerArrow.width;
	Vector2 centerArrowPos = {550, 480};
	// Arrow left
	float scaleLeftArrow = arrowWidth / (float)leftArrow.width;
	Vector2 leftArrowPos = {120, 500};
	// Arrow rigth
	float scaleRightArrow = arrowWidth / (float)rightArrow.width;
	Vector2 rightArrowPos = {950, 500};
	// Snail
	float scaleSnail = snailWidth / (float)snail.width;
	Vector2 SnailPos = {50, 620};

	// --------------------- Draw a rectangle on objects position for clicking ---------------------
	Rectangle centerArrowRec = {centerArrowPos.x, centerArrowPos.y, centerArrow.width * scaleCenterArrow, centerArrow.height * scaleCenterArrow};
	Rectangle leftArrowRec = {leftArrowPos.x, leftArrowPos.y, leftArrow.width * scaleLeftArrow, leftArrow.height * scaleLeftArrow};
	Rectangle rightArrowRec = {rightArrowPos.x, rightArrowPos.y, rightArrow.width * scaleRightArrow, rightArrow.height * scaleRightArrow};
	Rectangle snailRec = {SnailPos.x, SnailPos.y, snail.width * scaleSnail, snail.height * scaleSnail};

	// --------------------- Music config ---------------------
	PlayMusicStream(introMusic);
	PlayMusicStream(mainMusic);
	PlayMusicStream(v1);

	SetMusicVolume(introMusic, .8f);
	SetMusicVolume(mainMusic, .8f);
	SetMusicVolume(v1, 1.0f);

	// --------------------- Variables ---------------------
	int isStartScreen = 1;
	int isLostScreen = 0;
	int isWonScreen = 0;
	int isClassroom = 0;
	int isLeftSide = 0;

	// --------------------- Game loop ---------------------
	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();

		UpdateMusicStream(introMusic);

		if (IsKeyPressed(KEY_ENTER))
		{
			isStartScreen = 0;
			isClassroom = 1;
			PauseMusicStream(introMusic);
		}

		if (isStartScreen)
		{
			BeginDrawing();

			ClearBackground(BLACK);

			DrawText("Five Nigths At School", 150, 200, 60, WHITE);
			DrawText("Presiona enter para comenzar . . .", 150, 270, 20, WHITE);

			EndDrawing();
		}

		if (isClassroom == 1)
		{
			// UpdateMusicStream(v1);
			// UpdateMusicStream(mainMusic);
			BeginDrawing();
			// background
			DrawTextureEx(classroomBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
			// objects
			DrawTextureEx(centerArrow, centerArrowPos, 0.0f, scaleCenterArrow, WHITE);
			DrawTextureEx(leftArrow, leftArrowPos, 0.0f, scaleLeftArrow, WHITE);
			DrawTextureEx(rightArrow, rightArrowPos, 0.0f, scaleRightArrow, WHITE);
			DrawTextureEx(snail, SnailPos, 0.0f, scaleSnail, WHITE);

			if (CheckCollisionPointRec(mousePos, leftArrowRec))
			{
				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
				{
					isLeftSide = 1;
				}
			}
			if (isLeftSide == 1)
			{
				DrawTextureEx(windowBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
			}
			if (CheckCollisionPointRec(mousePos, snailRec))
			{
				DrawTextureEx(tutorialWindow, tutorialWindowPos, 0.0f, 0.5f, WHITE);
			}

			EndDrawing();
		}

		if (isLostScreen)
		{
			BeginDrawing();

			ClearBackground(BLACK);

			DrawText("GAME OVER", 150, 200, 60, WHITE);
			DrawText("Cause of death: you suck at math bruah", 150, 265, 20, WHITE);

			EndDrawing();
		}

		if (isWonScreen)
		{
			BeginDrawing();

			ClearBackground(BLACK);

			DrawText("Good job kid!", 150, 200, 60, WHITE);
			DrawText("You survived the night", 150, 265, 20, WHITE);

			EndDrawing();
		}
	}

	// --------------------- Clean Up ---------------------
	UnloadTexture(classroomBackground);
	UnloadTexture(windowBackground);
	UnloadTexture(tutorialWindow);
	UnloadTexture(centerArrow);
	UnloadTexture(leftArrow);
	UnloadTexture(rightArrow);
	UnloadTexture(snail);

	UnloadMusicStream(mainMusic);
	UnloadMusicStream(introMusic);
	UnloadMusicStream(v1);

	CloseAudioDevice(); // Close audio device (music streaming is automatically stopped)

	CloseWindow();
	return 0;
}