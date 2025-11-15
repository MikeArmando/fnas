#pragma once
#include "raylib.h"
#include <stdbool.h>

typedef struct _Resources
{
    // Backgrounds
    Texture2D classroomWindow;
    Texture2D leftTaskWindow;
    Texture2D centerTaskWindow;
    Texture2D rightTaskWindow;
    Texture2D tutorialWindow;

    // Background task indicators
    Texture2D centerIndicator;
    Texture2D leftIndicator;

    // Background task true
    Texture2D leftTaskTrue;
    Texture2D centerTaskTrue;
    Texture2D rightTaskTrue;

    // Objects
    Texture2D centerArrow;
    Texture2D leftArrow;
    Texture2D rightArrow;
    Texture2D returnArrow;
    Texture2D helper;

    // tasks books
    Texture2D taskList[20];

    // Music
    Music introMusic;
    Music m1Track;
    Music monoV1;
    Music monoV2;

    // Sound Effects
    Sound arrowClick;

    // Left task
    Sound knockingOnWindow;
    Sound eatingCookie;
    // Center task
    Sound writingOnBoard;
    // Rigth task
    Sound girlLaugh;
    Sound doorClosed;

    Image icon;
} Ts_resources;

void resources_load();

void resources_unload();

const Ts_resources *resources_get();