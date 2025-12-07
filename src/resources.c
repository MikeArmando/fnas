#include "resources.h"
#include <string.h>

static Ts_resources res;

void resourcesLoad()
{
    // set struct at 0
    memset(&res, 0, sizeof(Ts_resources));

    // Background
    res.texture.classroom = LoadTexture("backgrounds/classroom_default.png");
    res.texture.centerTaskWindow = LoadTexture("backgrounds/center_task.png");
    res.texture.leftTaskWindow = LoadTexture("backgrounds/left_task_window.png");
    res.texture.rightTaskWindow = LoadTexture("backgrounds/right_task_window.png");
    res.texture.rightTaskWindowLights = LoadTexture("backgrounds/right_task_window_light_on.png");
    res.texture.tutorialWindow = LoadTexture("backgrounds/tutorial.jpg");

    // Tutorial
    res.texture.tutorialPages[0] = LoadTexture("backgrounds/tutorial1.png");
    res.texture.tutorialPages[1] = LoadTexture("backgrounds/tutorial2.png");
    res.texture.tutorialPages[2] = LoadTexture("backgrounds/tutorial3.png");
    res.texture.tutorialPages[3] = LoadTexture("backgrounds/tutorial4.png");
    res.texture.tutorialPages[4] = LoadTexture("backgrounds/tutorial5.png");
    res.texture.tutorialPages[5] = LoadTexture("backgrounds/tutorial6.png");

    res.texture.tutorialPart1 = LoadTexture("backgrounds/tutorial1.png");
    res.texture.tutorialPart2 = LoadTexture("backgrounds/tutorial.png");
    res.texture.tutorialPart3 = LoadTexture("backgrounds/tutorial.png");
    res.texture.tutorialPart4 = LoadTexture("backgrounds/tutorial.png");
    res.texture.tutorialPart5 = LoadTexture("backgrounds/tutorial.png");

    // Background task indicators
    res.texture.centerIndicator = LoadTexture("objects/center_task_indicator.png");
    res.texture.leftIndicator = LoadTexture("objects/left_task_indicator.png");

    // Background task true
    res.texture.leftTaskTrue = LoadTexture("backgrounds/left_task_true.png");
    res.texture.centerTaskTrue = LoadTexture("backgrounds/left_task_true.png");
    res.texture.rightTaskTrue = LoadTexture("backgrounds/right_task_window_true.png");

    // Objects
    res.texture.centerArrow = LoadTexture("objects/arrow_up.png");
    res.texture.leftArrow = LoadTexture("objects/arrow_left.png");
    res.texture.rightArrow = LoadTexture("objects/arrow_right.png");
    res.texture.returnArrow = LoadTexture("objects/arrow_down.png");
    res.texture.helper = LoadTexture("objects/helper.png");

    // Music
    res.music.intro = LoadMusicStream("music/intro.mp3");
    res.music.monoV1 = LoadMusicStream("music/mono_v1.mp3");
    res.music.ending = LoadMusicStream("music/ending.mp3");
    res.music.won = LoadMusicStream("music/won.mp3");
    res.music.background = LoadMusicStream("music/background.mp3");

    // Sound Effect
    res.sound.arrowClick = LoadSound("soundEffects/click.mp3");
    res.sound.suspenseSound = LoadSound("soundEffects/suspense.mp3");
    res.sound.hasWon = LoadSound("soundEffects/has_won.mp3");
    res.sound.hasLost = LoadSound("soundEffects/has_won.mp3");
    res.sound.incorrect = LoadSound("soundEffects/incorrect.mp3");
    res.sound.doorOpening = LoadSound("soundEffects/door_opening.mp3");
    res.sound.girlJumpscare = LoadSound("soundEffects/girl_jumpscare.mp3");

    // Left task
    res.sound.eatingCookie = LoadSound("soundEffects/eating_effect.mp3");

    // Center task
    res.sound.answerOnChalkboard = LoadSound("soundEffects/answer_chalkboard.mp3");
    res.sound.ghostWriting = LoadSound("soundEffects/writing_board.mp3");

    // Right task
    res.sound.girlLaugh = LoadSound("soundEffects/girl_laugh.mp3");

    // Volume Config
    SetMusicVolume(res.music.intro, 0.8f);
    SetMusicVolume(res.music.m1Track, 0.3f);
    SetMusicVolume(res.music.monoV1, 0.8f);
    SetMusicVolume(res.music.ending, 0.7f);
    SetMusicVolume(res.music.won, 0.8f);
    SetMusicVolume(res.music.background, 2.0f);

    SetSoundVolume(res.sound.arrowClick, 0.5f);
    SetSoundVolume(res.sound.suspenseSound, 0.3f);
    SetSoundVolume(res.sound.eatingCookie, 0.7f);
    SetSoundVolume(res.sound.girlLaugh, 0.8f);
    SetSoundVolume(res.sound.ghostWriting, 0.8f);
    SetSoundVolume(res.sound.answerOnChalkboard, 0.6f);
    SetSoundVolume(res.sound.incorrect, 0.7f);
}

void resourcesUnload()
{
    // Music
    StopMusicStream(res.music.intro);
    StopMusicStream(res.music.m1Track);
    StopMusicStream(res.music.monoV1);
    StopMusicStream(res.music.ending);
    StopMusicStream(res.music.won);
    StopMusicStream(res.music.background);

    UnloadMusicStream(res.music.intro);
    UnloadMusicStream(res.music.m1Track);
    UnloadMusicStream(res.music.monoV1);
    UnloadMusicStream(res.music.ending);
    UnloadMusicStream(res.music.won);
    UnloadMusicStream(res.music.background);

    // Sound Effect
    UnloadSound(res.sound.knockingOnWindow);
    UnloadSound(res.sound.eatingCookie);
    UnloadSound(res.sound.girlLaugh);
    UnloadSound(res.sound.doorClosed);
    UnloadSound(res.sound.ghostWriting);
    UnloadSound(res.sound.answerOnChalkboard);
    UnloadSound(res.sound.suspenseSound);
    UnloadSound(res.sound.hasWon);
    UnloadSound(res.sound.incorrect);

    // Texture
    UnloadTexture(res.texture.classroom);
    UnloadTexture(res.texture.centerTaskWindow);
    UnloadTexture(res.texture.leftTaskWindow);
    UnloadTexture(res.texture.rightTaskWindow);
    UnloadTexture(res.texture.rightTaskWindowLights);
    UnloadTexture(res.texture.tutorialWindow);

    UnloadTexture(res.texture.leftTaskTrue);
    UnloadTexture(res.texture.centerTaskTrue);
    UnloadTexture(res.texture.rightTaskTrue);

    UnloadTexture(res.texture.centerIndicator);
    UnloadTexture(res.texture.leftIndicator);

    UnloadTexture(res.texture.centerArrow);
    UnloadTexture(res.texture.leftArrow);
    UnloadTexture(res.texture.rightArrow);
    UnloadTexture(res.texture.returnArrow);

    UnloadTexture(res.texture.helper);

    // Zero out the struct for safety
    memset(&res, 0, sizeof(Ts_resources));
}

const Ts_resources *resourcesGet()
{
    return &res;
}
