#include "raylib.h"
#include "resource_dir.h" // Utility header for SearchAndSetResourceDir
#include <math.h>		  // fminf, fmaxf

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	SetTargetFPS(60); // FPS set to 60

	// Create the window and OpenGL context
	InitWindow(900, 590, "Five Nigths At School");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load textures from the resources directory
	Texture2D arrow = LoadTexture("arrow.png");
	Texture2D classroomBackground = LoadTexture("clase.jpg"); // Remplazar con fondo correcto

	// ENTER COLOR CODE HERE, AND CHANGE THE COLOR NAME TO THE CORRECT COLOR (maybe also later move to color libery)
	Color green = {20, 160, 133, 255};

	int isEnterPressed = 0;

	// Game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		if (IsKeyPressed(KEY_ENTER)) // SI se presiona enter, se ejecuta el bloque de codigo, pero solo una vez, debido al loop
		{
			isEnterPressed = 1;
		}

		if (isEnterPressed == 1)
		{
			// ------- Backgrounds -------
			float sx = (float)GetScreenWidth() / (float)classroomBackground.width;
			float sy = (float)GetScreenHeight() / (float)classroomBackground.height;

			// FIT
			float scaleFit = fminf(sx, sy); // usar fminf para "fit"
			Vector2 backgroundPosFit = {(GetScreenWidth() - classroomBackground.width * scaleFit) / 2.0f, (GetScreenHeight() - classroomBackground.height * scaleFit) / 2.0f};

			// COVER
			float scaleCover = fmaxf(sx, sy); // usar fmaxf para "cover"
			Vector2 bgPosCover = {(GetScreenWidth() - classroomBackground.width * scaleCover) / 2.0f, (GetScreenHeight() - classroomBackground.height * scaleCover) / 2.0f};

			// ------- Objects -------
			// Escalar a un tamaño pixel exacto
			float desiredW = 60.0f;

			// Escala uniforme por ancho:
			float scaleUniform = desiredW / (float)arrow.width;
			Vector2 objPos = {424, 330};

			BeginDrawing(); // --------------------------------------------------------

			// Draw our texture to the screen
			DrawTextureEx(classroomBackground, backgroundPosFit, 0.0f, scaleFit, WHITE);
			DrawTextureEx(arrow, objPos, 0.0f, scaleUniform, WHITE);

			EndDrawing(); // --------------------------------------------------------
		}
		else
		{
			BeginDrawing(); // --------------------------------------------------------

			// Setup a background with color
			ClearBackground(BLACK);

			// draw some text using the default font
			DrawText("Five Nigths At School", 150, 200, 40, WHITE);
			DrawText("Presiona enter para comenzar . . .", 150, 250, 20, WHITE);

			EndDrawing(); // --------------------------------------------------------
		}
	}

	// ----------- Clean Up
	// unload textures so it can be cleaned up
	UnloadTexture(arrow);
	UnloadTexture(classroomBackground);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
