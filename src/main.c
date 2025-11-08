#include "raylib.h"
#include "resource_dir.h"
#include <math.h> // fminf, fmaxf

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetTargetFPS(60);

	InitWindow(900, 590, "Five Nigths At School");

	SearchAndSetResourceDir("resources");

	InitAudioDevice();

	// ----------------- Load resources ---------------------
	// Backgrounds
	Texture2D classroomBackground = LoadTexture("clase.jpg");
	Texture2D windowBackground = LoadTexture("ventana.jpg");

	// Objects
	Texture2D arrowCenter = LoadTexture("arrow.png");
	Texture2D arrowLeft = LoadTexture("arrow.png");

	// Music
	Music mainMusic = LoadMusicStream("horrorMain.mp3");
	Music introMusic = LoadMusicStream("intro.mp3");

	// --------------------- Background Config ---------------------
	float sx = (float)GetScreenWidth() / (float)classroomBackground.width;
	float sy = (float)GetScreenHeight() / (float)classroomBackground.height;

	// FIT
	float scaleFit = fminf(sx, sy);
	Vector2 backgroundPosFit = {(GetScreenWidth() - classroomBackground.width * scaleFit) / 2.0f, (GetScreenHeight() - classroomBackground.height * scaleFit) / 2.0f};

	// COVER
	float scaleCover = fmaxf(sx, sy);
	Vector2 bgPosCover = {(GetScreenWidth() - classroomBackground.width * scaleCover) / 2.0f, (GetScreenHeight() - classroomBackground.height * scaleCover) / 2.0f};

	// --------------------- Objects ---------------------
	// Escalar a un tamaño pixel exacto
	float desiredWidth = 60.0f;

	// Arrow Center
	float scaleArrowCenter = desiredWidth / (float)arrowCenter.width;
	Vector2 arrowCenterPos = {424, 330};
	// Arrow left
	float scaleArrowLeft = desiredWidth / (float)arrowLeft.width;
	Vector2 arrowLeftPos = {180, 340};
	// Arrow rigth

	// --------------------- Draw a rectangle on objects position ---------------------
	Rectangle centerArrowSide = {arrowCenterPos.x, arrowCenterPos.y, arrowCenter.width * scaleArrowCenter, arrowCenter.height * scaleArrowCenter};
	Rectangle leftArrowSide = {arrowLeftPos.x, arrowLeftPos.y, arrowLeft.width * scaleArrowLeft, arrowLeft.height * scaleArrowLeft};

	// --------------------- Music config ---------------------
	PlayMusicStream(introMusic);
	PlayMusicStream(mainMusic);

	SetMusicVolume(introMusic, 1.0f);
	SetMusicVolume(mainMusic, 1.0f);

	// --------------------- Variables ---------------------
	int isEnterKeyPressed = 0;
	int isLeftSide = 0;
	int goBack = 0;

	// --------------------- Game loop ---------------------
	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();

		UpdateMusicStream(introMusic);

		if (IsKeyPressed(KEY_ENTER))
		{
			isEnterKeyPressed = 1;
			PauseMusicStream(introMusic);
		}

		if (isEnterKeyPressed == 1)
		{
			UpdateMusicStream(mainMusic);
			BeginDrawing();
			// background
			DrawTextureEx(classroomBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
			// Arrows
			DrawTextureEx(arrowCenter, arrowCenterPos, 0.0f, scaleArrowCenter, WHITE);
			DrawTextureEx(arrowLeft, arrowLeftPos, 0.0f, scaleArrowLeft, WHITE);

			if (CheckCollisionPointRec(mousePos, leftArrowSide))
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

			EndDrawing();
		}
		else
		{
			BeginDrawing();

			ClearBackground(BLACK);

			DrawText("Five Nigths At School", 150, 200, 40, WHITE);
			DrawText("Presiona enter para comenzar . . .", 150, 250, 20, WHITE);

			EndDrawing();
		}
	}

	// --------------------- Clean Up ---------------------
	UnloadTexture(classroomBackground);
	UnloadTexture(windowBackground);
	UnloadTexture(arrowCenter);
	UnloadTexture(arrowLeft);

	UnloadMusicStream(mainMusic);
	UnloadMusicStream(introMusic);

	CloseAudioDevice(); // Close audio device (music streaming is automatically stopped)

	CloseWindow();
	return 0;
}