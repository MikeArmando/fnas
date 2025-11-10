#pragma once

#include "raylib.h"
#include <stdbool.h>

typedef struct _Resources
{
    // Backgrounds
    Texture2D classroomWindow;
    Texture2D classroomCenterTaskWindow;
    Texture2D classroomLeftTaskWindow;
    Texture2D classroomRightTaskWindow;
    Texture2D centerTaskWindow;
    Texture2D leftTaskWindow;
    Texture2D rightTaskWindow;
    Texture2D tutorialWindow;

    // Background task indicators
    Texture2D classroomCenterIndicator;
    Texture2D classroomLeftIndicator;
    Texture2D classroomRightIndicator;

    // Objects
    Texture2D centerArrow;
    Texture2D leftArrow;
    Texture2D rightArrow;
    Texture2D snail;

    // Music
    Music introMusic;
    Music m1Track;
    Music monoV1;
    Music monoV2;
} Ts_resources;

void resources_load();

void resources_unload();

const Ts_resources *resources_get();