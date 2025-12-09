#include "resources.h"
#include <string.h>

static Ts_resources res;

// --------------------------------------- LOAD RESOURCES ---------------------------------------
void resourcesLoad()
{
    memset(&res, 0, sizeof(Ts_resources));

    // Textures
    LoadAllTextures(&res.texture);

    // Music
    LoadAllMusic(&res.music);

    // Sounds
    LoadAllSounds(&res.sound);
}

void LoadAllTextures(Ts_Textures *textures)
{
    // Background
    textures->classroom = LoadTexture("backgrounds/classroom_default.png");
    textures->centerTaskWindow = LoadTexture("backgrounds/center_task.png");
    textures->leftTaskWindow = LoadTexture("backgrounds/left_task_window.png");
    textures->rightTaskWindow = LoadTexture("backgrounds/right_task_window.png");
    textures->rightTaskWindowLights = LoadTexture("backgrounds/right_task_window_light_on.png");

    // Tutorial
    for (int i = 0; i < 6; i++)
    {
        textures->tutorialPages[i] = LoadTexture(TextFormat("backgrounds/tutorial%d.png", i + 1));
    }

    // Background task indicators
    textures->centerIndicator = LoadTexture("objects/center_task_indicator.png");
    textures->leftIndicator = LoadTexture("objects/left_task_indicator.png");

    // Background task true
    textures->leftTaskTrue = LoadTexture("backgrounds/left_task_true.png");
    textures->centerTaskTrue = LoadTexture("backgrounds/left_task_true.png");
    textures->rightTaskTrue = LoadTexture("backgrounds/right_task_window_true.png");

    // Objects
    textures->centerArrow = LoadTexture("objects/arrow_up.png");
    textures->leftArrow = LoadTexture("objects/arrow_left.png");
    textures->rightArrow = LoadTexture("objects/arrow_right.png");
    textures->returnArrow = LoadTexture("objects/arrow_down.png");
    textures->helper = LoadTexture("objects/helper.png");
    textures->helperGeneralWindow = LoadTexture("objects/tutorial_general.png");
    textures->helperSumWindow = LoadTexture("objects/helper_addition.png");
    textures->helperSubWindow = LoadTexture("objects/helper_subtraction.png");
    textures->helperMultWindow = LoadTexture("objects/helper_mult.png");
}

void LoadAllMusic(Ts_Music *music)
{
    // Music
    music->intro = LoadMusicStream("music/intro.mp3");
    music->monoV1 = LoadMusicStream("music/mono_v1.mp3");
    music->ending = LoadMusicStream("music/ending.mp3");
    music->won = LoadMusicStream("music/won.mp3");
    music->background = LoadMusicStream("music/background.mp3");

    // Volume Config
    SetMusicVolume(music->intro, 0.6f);
    SetMusicVolume(music->m1Track, 0.3f);
    SetMusicVolume(music->monoV1, 0.6f);
    SetMusicVolume(music->ending, 0.6f);
    SetMusicVolume(music->won, 0.6f);
    SetMusicVolume(music->background, 2.0f);
}
void LoadAllSounds(Ts_SFX *sound)
{
    // General Sound Effects
    sound->arrowClick = LoadSound("soundEffects/click.mp3");
    sound->suspenseSound = LoadSound("soundEffects/suspense.mp3");
    sound->hasWon = LoadSound("soundEffects/has_won.mp3");
    sound->hasLost = LoadSound("soundEffects/has_won.mp3");
    sound->incorrect = LoadSound("soundEffects/incorrect.mp3");
    sound->doorOpening = LoadSound("soundEffects/door_opening.mp3");
    sound->girlJumpscare = LoadSound("soundEffects/girl_jumpscare.mp3");

    // tasks
    sound->knockingOnWindow = LoadSound("soundEffects/knocking_window.mp3");
    sound->eatingCookie = LoadSound("soundEffects/eating_effect.mp3");
    sound->answerOnChalkboard = LoadSound("soundEffects/answer_chalkboard.mp3");
    sound->ghostWriting = LoadSound("soundEffects/writing_board.mp3");
    sound->girlLaugh = LoadSound("soundEffects/girl_laugh.mp3");

    // Volume Config
    SetSoundVolume(sound->arrowClick, 0.5f);
    SetSoundVolume(sound->suspenseSound, 0.2f);
    SetSoundVolume(sound->knockingOnWindow, 0.6f);
    SetSoundVolume(sound->eatingCookie, 0.7f);
    SetSoundVolume(sound->girlLaugh, 0.8f);
    SetSoundVolume(sound->ghostWriting, 0.8f);
    SetSoundVolume(sound->answerOnChalkboard, 0.6f);
    SetSoundVolume(sound->incorrect, 0.7f);
}

// --------------------------------------- UNLOAD RESOURCES ---------------------------------------
void resourcesUnload()
{
    // Music
    UnloadAllMusic(&res.music);

    // Sound Effect
    UnloadAllSounds(&res.sound);

    // Texture
    UnloadAllTextures(&res.texture);

    memset(&res, 0, sizeof(Ts_resources));
}

void UnloadAllTextures(Ts_Textures *textures)
{
    UnloadTexture(textures->classroom);
    UnloadTexture(textures->centerTaskWindow);
    UnloadTexture(textures->leftTaskWindow);
    UnloadTexture(textures->rightTaskWindow);
    UnloadTexture(textures->rightTaskWindowLights);
    UnloadTexture(textures->helperGeneralWindow);

    UnloadTexture(textures->leftTaskTrue);
    UnloadTexture(textures->centerTaskTrue);
    UnloadTexture(textures->rightTaskTrue);

    UnloadTexture(textures->centerIndicator);
    UnloadTexture(textures->leftIndicator);

    UnloadTexture(textures->centerArrow);
    UnloadTexture(textures->leftArrow);
    UnloadTexture(textures->rightArrow);
    UnloadTexture(textures->returnArrow);

    UnloadTexture(textures->helper);
}

void UnloadAllMusic(Ts_Music *music)
{
    StopMusicStream(music->intro);
    StopMusicStream(music->m1Track);
    StopMusicStream(music->monoV1);
    StopMusicStream(music->ending);
    StopMusicStream(music->won);
    StopMusicStream(music->background);

    UnloadMusicStream(music->intro);
    UnloadMusicStream(music->m1Track);
    UnloadMusicStream(music->monoV1);
    UnloadMusicStream(music->ending);
    UnloadMusicStream(music->won);
    UnloadMusicStream(music->background);
}

void UnloadAllSounds(Ts_SFX *sound)
{
    UnloadSound(sound->knockingOnWindow);
    UnloadSound(sound->eatingCookie);
    UnloadSound(sound->girlLaugh);
    UnloadSound(sound->doorClosed);
    UnloadSound(sound->ghostWriting);
    UnloadSound(sound->answerOnChalkboard);
    UnloadSound(sound->suspenseSound);
    UnloadSound(sound->hasWon);
    UnloadSound(sound->incorrect);
}

const Ts_resources *resourcesGet()
{
    return &res;
}