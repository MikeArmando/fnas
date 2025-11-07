// background.c
#include "background.h"
#include <math.h> // fminf, fmaxf
#include <stdlib.h>

Background LoadBackground(const char *filePath, BackgroundMode mode)
{
    Background bg = {0};
    bg.texture = LoadTexture(filePath);
    bg.mode = mode;
    bg.lastScale = 1.0f;
    bg.lastPos = (Vector2){0, 0};
    return bg;
}

void UnloadBackground(Background *bg)
{
    if (bg == NULL)
        return;
    UnloadTexture(bg->texture);
    bg->texture.id = 0;
}

void RecalculateBackground(const Background *bg, int winW, int winH, float *outScale, Vector2 *outPos)
{
    if (bg == NULL || outScale == NULL || outPos == NULL)
        return;
    float sx = (float)winW / (float)bg->texture.width;
    float sy = (float)winH / (float)bg->texture.height;

    if (bg->mode == BG_MODE_FIT)
    {
        float scale = fminf(sx, sy);
        *outScale = scale;
        outPos->x = (winW - bg->texture.width * scale) / 2.0f;
        outPos->y = (winH - bg->texture.height * scale) / 2.0f;
    }
    else
    { // COVER
        float scale = fmaxf(sx, sy);
        *outScale = scale;
        outPos->x = (winW - bg->texture.width * scale) / 2.0f;
        outPos->y = (winH - bg->texture.height * scale) / 2.0f;
    }
}

void DrawBackground(const Background *bg)
{
    if (bg == NULL)
        return;
    // recalcula cada frame por si la ventana cambió
    float scale;
    Vector2 pos;
    RecalculateBackground(bg, GetScreenWidth(), GetScreenHeight(), &scale, &pos);
    // guarda resultados útiles en caso de quererlos (no modificamos const, simplemente dibujamos)
    DrawTextureEx(bg->texture, pos, 0.0f, scale, WHITE);
}

void SetBackgroundMode(Background *bg, BackgroundMode mode)
{
    if (bg)
        bg->mode = mode;
}
