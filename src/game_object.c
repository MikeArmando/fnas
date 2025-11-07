// game_object.c
#include "game_object.h"
#include <stdlib.h>

GameObject LoadGameObject(const char *filePath, float desiredWidth, Vector2 pos)
{
    GameObject o = {0};
    o.texture = LoadTexture(filePath);
    o.desiredWidth = desiredWidth;
    // calcula escala uniforme por ancho (si texture.width == 0, evita div/0)
    o.scale = (o.texture.width > 0) ? (desiredWidth / (float)o.texture.width) : 1.0f;
    o.position = pos;
    o.rotation = 0.0f;
    // origin por defecto al centro del destino (se calcula a partir del desiredWidth/height en Draw)
    o.origin = (Vector2){0, 0}; // lo ajustaremos en Draw si está a {0,0}
    return o;
}

void UnloadGameObject(GameObject *obj)
{
    if (obj == NULL)
        return;
    UnloadTexture(obj->texture);
    obj->texture.id = 0;
}

void DrawGameObject(const GameObject *obj)
{
    if (obj == NULL)
        return;
    // escala uniforme: scale ya calculada; tamaño final en px:
    float finalW = obj->texture.width * obj->scale;
    float finalH = obj->texture.height * obj->scale;
    // si origin es 0, usa el centro
    Vector2 origin = obj->origin;
    if (origin.x == 0 && origin.y == 0)
        origin = (Vector2){finalW / 2.0f, finalH / 2.0f};
    // usando DrawTexturePro con src toda la textura y dest rect con tamaño final
    Rectangle src = {0, 0, (float)obj->texture.width, (float)obj->texture.height};
    Rectangle dest = {obj->position.x, obj->position.y, finalW, finalH};
    DrawTexturePro(obj->texture, src, dest, origin, obj->rotation, WHITE);
}

void SetGameObjectSize(GameObject *obj, float desiredWidth)
{
    if (!obj)
        return;
    obj->desiredWidth = desiredWidth;
    obj->scale = (obj->texture.width > 0) ? (desiredWidth / (float)obj->texture.width) : 1.0f;
}

void SetGameObjectPosition(GameObject *obj, Vector2 pos)
{
    if (!obj)
        return;
    obj->position = pos;
}

void SetGameObjectRotation(GameObject *obj, float degrees)
{
    if (!obj)
        return;
    obj->rotation = degrees;
}
