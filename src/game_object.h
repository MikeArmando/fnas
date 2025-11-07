// game_object.h
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"

typedef struct
{
    Texture2D texture;
    float desiredWidth; // ancho deseado en píxeles (escala uniforme según ancho)
    float scale;        // escala actual (derived)
    Vector2 position;
    float rotation; // grados
    Vector2 origin; // para rotar (por defecto centro)
} GameObject;

GameObject LoadGameObject(const char *filePath, float desiredWidth, Vector2 pos);
void UnloadGameObject(GameObject *obj);
void DrawGameObject(const GameObject *obj);
void SetGameObjectSize(GameObject *obj, float desiredWidth);
void SetGameObjectPosition(GameObject *obj, Vector2 pos);
void SetGameObjectRotation(GameObject *obj, float degrees);
bool GameObject_IsMouseOver(const GameObject *obj);
bool GameObject_IsClicked(const GameObject *obj, int mouseButton);

#endif // GAME_OBJECT_H
