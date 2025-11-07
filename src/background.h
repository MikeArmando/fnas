// background.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"

typedef enum
{
    BG_MODE_FIT = 0,
    BG_MODE_COVER = 1
} BackgroundMode;

typedef struct
{
    Texture2D texture;
    BackgroundMode mode;
    float lastScale; // escala calculada
    Vector2 lastPos; // posición calculada
} Background;

// Carga textura; devuelve Background (asegúrate de llamar UnloadBackground)
Background LoadBackground(const char *filePath, BackgroundMode mode);

// Descarga recursos
void UnloadBackground(Background *bg);

// Dibuja según el modo guardado (fit o cover)
void DrawBackground(const Background *bg);

// Fuerza recalcular (por ejemplo al redimensionar la ventana)
void RecalculateBackground(const Background *bg, int winW, int winH, float *outScale, Vector2 *outPos);

// Opcional: cambiar modo
void SetBackgroundMode(Background *bg, BackgroundMode mode);

#endif // BACKGROUND_H
