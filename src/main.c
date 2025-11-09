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
	Texture2D classroomBackground = LoadTexture("backgrounds/defualtClassroom.png");
	Texture2D centerBackground = LoadTexture("backgrounds/ventana.jpg");
	Texture2D leftBackground = LoadTexture("backgrounds/ventana.jpg");
	Texture2D rightBackground = LoadTexture("backgrounds/ventana.jpg");
	Texture2D tutorialWindow = LoadTexture("backgrounds/tutorial.jpg");

	// Objects
	Texture2D centerArrow = LoadTexture("objects/arrow.png");
	Texture2D leftArrow = LoadTexture("objects/arrow.png");
	Texture2D rightArrow = LoadTexture("objects/arrow.png");
	Texture2D snail = LoadTexture("objects/snail.png");

	// Music
	Music m1Track = LoadMusicStream("music/m2.mp3");
	Music introMusic = LoadMusicStream("music/intro.mp3");
	Music monoV1 = LoadMusicStream("music/monoV1.mp3");
	Music monoV2 = LoadMusicStream("music/monoV2.mp3");

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

	// --------------------- Objects Config ---------------------
	// Scale to pixel size
	float arrowSize = 60.0f;
	float snailSize = 120.0f;

	// Center Arrow
	float scaleCenterArrow = arrowSize / (float)centerArrow.width;
	Vector2 centerArrowPos = {550, 480};
	// Left Arrow
	float scaleLeftArrow = arrowSize / (float)leftArrow.width;
	Vector2 leftArrowPos = {120, 500};
	// Rigth Arrow
	float scaleRightArrow = arrowSize / (float)rightArrow.width;
	Vector2 rightArrowPos = {950, 500};
	// Snail
	float scaleSnail = snailSize / (float)snail.width;
	Vector2 SnailPos = {50, 630};

	// --------------------- Background on objects position for clicking ---------------------
	Rectangle centerArrowRec = {centerArrowPos.x, centerArrowPos.y, centerArrow.width * scaleCenterArrow, centerArrow.height * scaleCenterArrow};
	Rectangle leftArrowRec = {leftArrowPos.x, leftArrowPos.y, leftArrow.width * scaleLeftArrow, leftArrow.height * scaleLeftArrow};
	Rectangle rightArrowRec = {rightArrowPos.x, rightArrowPos.y, rightArrow.width * scaleRightArrow, rightArrow.height * scaleRightArrow};
	Rectangle snailRec = {SnailPos.x, SnailPos.y, snail.width * scaleSnail, snail.height * scaleSnail};

	// --------------------- Music config ---------------------
	SetMusicVolume(introMusic, .8f);
	SetMusicVolume(m1Track, .5f);
	SetMusicVolume(monoV1, 1.0f);
	SetMusicVolume(monoV2, 1.0f);

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

		UpdateMusicStream(introMusic);
		UpdateMusicStream(m1Track);
		UpdateMusicStream(monoV1);
		UpdateMusicStream(monoV2);

		float monoV1Played = GetMusicTimePlayed(monoV1);
		float monoV1Len = GetMusicTimeLength(monoV1);

		float monoV2Played = GetMusicTimePlayed(monoV2);
		float monoV2Len = GetMusicTimeLength(monoV2);

		// ----------------- Logic Section -----------------
		PlayMusicStream(m1Track);

		if (IsKeyPressed(KEY_ENTER))
		{
			StopMusicStream(introMusic);
			PlayMusicStream(m1Track);
			PlayMusicStream(monoV1);

			isStartScreen = 0;
			isClassroom = 1;
		}

		if (isClassroom == 1)
		{
			if (monoV1Len > 0.001f && monoV1Played + 0.05f >= monoV1Len)
			{
				StopMusicStream(monoV1);
				PlayMusicStream(monoV2);
			}
			if (monoV2Len > 0.001f && monoV2Played + 0.05f >= monoV2Len)
			{
				StopMusicStream(monoV2);
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
		}

		// ----------------- Drawing Section -----------------
		BeginDrawing();
		if (isStartScreen)
		{
			ClearBackground(BLACK);
			DrawText("Five Nigths At School", 150, 200, 60, WHITE);
			DrawText("Presiona enter para comenzar . . .", 150, 270, 20, WHITE);
		}
		if (isClassroom)
		{
			DrawTextureEx(classroomBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
			DrawTextureEx(centerArrow, centerArrowPos, 0.0f, scaleCenterArrow, WHITE);
			DrawTextureEx(leftArrow, leftArrowPos, 0.0f, scaleLeftArrow, WHITE);
			DrawTextureEx(rightArrow, rightArrowPos, 0.0f, scaleRightArrow, WHITE);
			DrawTextureEx(snail, SnailPos, 0.0f, scaleSnail, WHITE);
		}
		if (isCenterWindow == 1)
		{
			DrawTextureEx(centerBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
		}
		if (isLeftWindow == 1)
		{
			DrawTextureEx(leftBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
		}
		if (isRightWindow == 1)
		{
			DrawTextureEx(rightBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
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
			DrawTextureEx(tutorialWindow, tutorialWindowPos, 0.0f, 0.5f, WHITE);
		}

		EndDrawing();
	}

	// --------------------- Clean Up ---------------------
	UnloadTexture(classroomBackground);
	UnloadTexture(centerBackground);
	UnloadTexture(leftBackground);
	UnloadTexture(rightBackground);
	UnloadTexture(tutorialWindow);
	UnloadTexture(centerArrow);
	UnloadTexture(leftArrow);
	UnloadTexture(rightArrow);
	UnloadTexture(snail);

	UnloadMusicStream(m1Track);
	UnloadMusicStream(introMusic);
	UnloadMusicStream(monoV1);
	UnloadMusicStream(monoV2);

	CloseAudioDevice();

	CloseWindow();
	return 0;
}