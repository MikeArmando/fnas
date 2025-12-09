#pragma once
#include "raylib.h"
#include <stdbool.h>

#define MAX_BOOK_PAGES 6

typedef struct _Music
{
    Music intro;
    Music m1Track;
    Music monoV1;
    Music monoV2;
    Music background;
    Music ending;
    Music won;
} Ts_Music;

typedef struct _SFX
{
    // General
    Sound arrowClick;
    Sound suspense;
    Sound hasWon;
    Sound hasLost;
    Sound incorrect;
    Sound doorOpening;
    Sound jumpscare;
    Sound suspenseSound;
    Sound CatMeow;

    // Task
    Sound knockingOnWindow;
    Sound eatingCookie;
    Sound ghostWriting;
    Sound answerOnChalkboard;
    Sound girlLaugh;
    Sound girlSinging;
    Sound doorClosed;
} Ts_SFX;

typedef struct _Textures
{
    // Start screen
    Texture2D startBtns;

    // Tutorial
    Texture2D tutorialPages[MAX_BOOK_PAGES];
    Texture2D classroom;

    // Tasks
    Texture2D leftTaskWindow;
    Texture2D centerTaskWindow;
    Texture2D rightTaskWindow;
    Texture2D rightTaskWindowLights;

    // Helper Window
    Texture2D helperGeneralWindow;
    Texture2D helperSumWindow;
    Texture2D helperSubWindow;
    Texture2D helperMultWindow;

    // Indicators
    Texture2D centerIndicator;
    Texture2D leftIndicator;

    // Tasks true
    Texture2D leftTaskTrue;
    Texture2D centerTaskTrue;
    Texture2D rightTaskTrue;

    // Interactive Objects
    Texture2D centerArrow;
    Texture2D leftArrow;
    Texture2D rightArrow;
    Texture2D returnArrow;
    Texture2D helper;
} Ts_Textures;

typedef struct _Resources
{
    Ts_Textures texture;

    Ts_Music music;

    Ts_SFX sound;
} Ts_resources;

// -------------------------- Load Resources --------------------------
void resourcesLoad();
void LoadAllTextures(Ts_Textures *textures);
void LoadAllMusic(Ts_Music *music);
void LoadAllSounds(Ts_SFX *sound);

// -------------------------- Unload Resources --------------------------
void resourcesUnload();
void UnloadAllTextures(Ts_Textures *textures);
void UnloadAllMusic(Ts_Music *music);
void UnloadAllSounds(Ts_SFX *sound);

// -------------------------- Get Resources --------------------------
const Ts_resources *resourcesGet();