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
    Texture2D rightTaskWindowLights;
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

    // Music
    Music introMusic;
    Music m1Track;
    Music monoV1;
    Music monoV2;
    Music backgroundNoise;

    // Sound Effects
    Sound arrowClick;
    Sound suspenseSound;
    Sound hasWon;
    Sound hasLost;
    Sound incorrect;

    // Left task
    Sound knockingOnWindow;
    Sound eatingCookie;
    // Center task
    Sound writingOnBoard;
    // Rigth task
    Sound girlLaugh;
    Sound doorClosed;
} Ts_resources;

void resourcesLoad();

void resourcesUnload();

const Ts_resources *resourcesGet();