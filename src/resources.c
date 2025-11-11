#include "resources.h"
#include <string.h>

static Ts_resources res;

void resources_load()
{
    // set struct at 0
    memset(&res, 0, sizeof(Ts_resources));

    // Background
    res.classroomWindow = LoadTexture("backgrounds/classroom_default.png");
    res.centerTaskWindow = LoadTexture("backgrounds/center_task_window.png");
    res.leftTaskWindow = LoadTexture("backgrounds/left_task_window.png");
    res.rightTaskWindow = LoadTexture("backgrounds/right_task_window.png");
    res.tutorialWindow = LoadTexture("backgrounds/tutorial.jpg");

    // Background task indicators
    res.classroomCenterIndicator = LoadTexture("backgrounds/monster.jpg");
    res.classroomLeftIndicator = LoadTexture("backgrounds/chalkboard.png");
    res.classroomRightIndicator = LoadTexture("backgrounds/girl.jpg");

    // Objects
    res.centerArrow = LoadTexture("objects/arrow.png");
    res.leftArrow = LoadTexture("objects/arrow.png");
    res.rightArrow = LoadTexture("objects/arrow.png");
    res.returnArrow = LoadTexture("objects/return_arrow.png");
    res.snail = LoadTexture("objects/snail.png");

    // Music
    res.introMusic = LoadMusicStream("music/intro.mp3");
    res.m1Track = LoadMusicStream("music/m2.mp3");
    res.monoV1 = LoadMusicStream("music/mono_v1.mp3");

    // MusSound Effectsic
    res.eatingCookie = LoadSound("soundEffects/eating_effect.mp3");
    res.doorKnocking = LoadSound("soundEffects/knocking_effect.mp3");
    res.doorClosed = LoadSound("soundEffects/closed_door_effect.mp3");

    // Volume Config
    SetMusicVolume(res.introMusic, 0.8f);
    SetMusicVolume(res.m1Track, 0.5f);
    SetMusicVolume(res.monoV1, 1.0f);

    SetSoundVolume(res.eatingCookie, 0.8f);
    SetSoundVolume(res.doorKnocking, 0.8f);
}

void resources_unload()
{
    StopMusicStream(res.introMusic);
    StopMusicStream(res.m1Track);
    StopMusicStream(res.monoV1);

    UnloadMusicStream(res.introMusic);
    UnloadMusicStream(res.m1Track);
    UnloadMusicStream(res.monoV1);

    UnloadTexture(res.classroomWindow);
    UnloadTexture(res.centerTaskWindow);
    UnloadTexture(res.leftTaskWindow);
    UnloadTexture(res.rightTaskWindow);
    UnloadTexture(res.tutorialWindow);
    UnloadTexture(res.centerArrow);
    UnloadTexture(res.leftArrow);
    UnloadTexture(res.rightArrow);
    UnloadTexture(res.snail);

    // Zero out the struct for safety
    memset(&res, 0, sizeof(Ts_resources));
}

const Ts_resources *resources_get()
{
    return &res;
}
