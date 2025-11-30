#include "resources.h"
#include <string.h>

static Ts_resources res;

void resourcesLoad()
{
    // set struct at 0
    memset(&res, 0, sizeof(Ts_resources));

    // Background
    res.classroomWindow = LoadTexture("backgrounds/classroom_default.png");
    res.centerTaskWindow = LoadTexture("backgrounds/center_task.png");
    res.leftTaskWindow = LoadTexture("backgrounds/left_task_window.png");
    res.rightTaskWindow = LoadTexture("backgrounds/right_task_window.png");
    res.rightTaskWindowLights = LoadTexture("backgrounds/right_task_window_light_on.png");
    res.tutorialWindow = LoadTexture("backgrounds/tutorial.jpg");

    // Background task indicators
    res.centerIndicator = LoadTexture("objects/center_task_indicator.png");
    res.leftIndicator = LoadTexture("objects/left_task_indicator.png");

    // Background task true
    res.leftTaskTrue = LoadTexture("backgrounds/left_task_true.png");
    res.centerTaskTrue = LoadTexture("backgrounds/left_task_true.png");
    res.rightTaskTrue = LoadTexture("backgrounds/left_task_true.png");

    // Objects
    res.centerArrow = LoadTexture("objects/arrow_up.png");
    res.leftArrow = LoadTexture("objects/arrow_left.png");
    res.rightArrow = LoadTexture("objects/arrow_right.png");
    res.returnArrow = LoadTexture("objects/arrow_down.png");
    res.helper = LoadTexture("objects/helper.png");

    // Music
    res.introMusic = LoadMusicStream("music/intro.mp3");
    res.monoV1 = LoadMusicStream("music/mono_v1.mp3");
    res.backgroundNoise = LoadMusicStream("music/background_music.mp3");

    // Sound Effect
    res.arrowClick = LoadSound("soundEffects/click.mp3");
    res.suspenseSound = LoadSound("soundEffects/suspense.mp3");
    res.hasWon = LoadSound("soundEffects/has_won.mp3");
    res.hasLost = LoadSound("soundEffects/has_won.mp3");
    res.incorrect = LoadSound("soundEffects/incorrect.mp3");

    // Left task
    res.knockingOnWindow = LoadSound("soundEffects/knocking_window.mp3");
    res.eatingCookie = LoadSound("soundEffects/eating_effect.mp3");

    // Center task
    res.writingOnBoard = LoadSound("soundEffects/writing_board.mp3");

    // Right task
    res.girlLaugh = LoadSound("soundEffects/girl_laugh.mp3");
    res.doorClosed = LoadSound("soundEffects/closed_door_effect.mp3");

    // Volume Config
    SetMusicVolume(res.introMusic, 0.8f);
    SetMusicVolume(res.m1Track, 0.3f);
    SetMusicVolume(res.monoV1, 0.8f);
    SetMusicVolume(res.backgroundNoise, 0.8f);

    SetSoundVolume(res.knockingOnWindow, 0.8f);
    SetSoundVolume(res.suspenseSound, 0.3f);
    SetSoundVolume(res.eatingCookie, 0.8f);
    SetSoundVolume(res.girlLaugh, 0.8f);
    SetSoundVolume(res.doorClosed, 0.8f);
    SetSoundVolume(res.writingOnBoard, 0.8f);
    SetSoundVolume(res.incorrect, 0.8f);
}

void resourcesUnload()
{
    // Music
    StopMusicStream(res.introMusic);
    StopMusicStream(res.m1Track);
    StopMusicStream(res.monoV1);

    UnloadMusicStream(res.introMusic);
    UnloadMusicStream(res.m1Track);
    UnloadMusicStream(res.monoV1);
    UnloadMusicStream(res.backgroundNoise);

    // Sound Effect
    UnloadSound(res.knockingOnWindow);
    UnloadSound(res.eatingCookie);
    UnloadSound(res.girlLaugh);
    UnloadSound(res.doorClosed);
    UnloadSound(res.writingOnBoard);
    UnloadSound(res.suspenseSound);
    UnloadSound(res.hasWon);
    UnloadSound(res.incorrect);

    // Texture
    UnloadTexture(res.classroomWindow);
    UnloadTexture(res.centerTaskWindow);
    UnloadTexture(res.leftTaskWindow);
    UnloadTexture(res.rightTaskWindow);
    UnloadTexture(res.rightTaskWindowLights);
    UnloadTexture(res.tutorialWindow);

    UnloadTexture(res.leftTaskTrue);
    UnloadTexture(res.centerTaskTrue);
    UnloadTexture(res.rightTaskTrue);

    UnloadTexture(res.centerIndicator);
    UnloadTexture(res.leftIndicator);

    UnloadTexture(res.centerArrow);
    UnloadTexture(res.leftArrow);
    UnloadTexture(res.rightArrow);
    UnloadTexture(res.returnArrow);

    UnloadTexture(res.helper);

    // Zero out the struct for safety
    memset(&res, 0, sizeof(Ts_resources));
}

const Ts_resources *resourcesGet()
{
    return &res;
}
