#include "App.h"
#include "LevelScene.h"
#include "WinScene.h"
#include "PacManData.h"

bool App::init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    //Create windows and create renderers
    if (!_window.init(PACMAN_SCREEN_WIDTH, PACMAN_SCREEN_HEIGHT))
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize PNG  and JPG loading
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    //Initialize SDL_mixer
    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }*/

    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void App::initScenes()
{
    _scenes.push_back(std::make_unique<LevelScene>(_window, _quit));
    _scenes.push_back(std::make_unique<WinScene>(_window, _quit));
}

void App::run()
{
    //Load the initial scene
    loadCurrScene();

    //Start the game loop and poll events
    SDL_Event e;

    while (_quit == false)
    {
        _scenes[_currentScene]->handleEvents(e);

        //Only update when not minimized and game is running
        if (!_window.isMinimized())
        {
            _scenes[_currentScene]->update();
            
            //Check for scene change
            GameState currentSceneState = _scenes[_currentScene]->getGameState();
            if (currentSceneState == GameState::GO_TO_NEXT_SCENE)
            {
                //Quit the curr scene
                _scenes[_currentScene]->quit();

                //Load the chosen scene
                _currentScene++;
                loadCurrScene();

                continue;
            }
            else if (currentSceneState == GameState::GO_TO_PREV_SCENE)
            {
                //Quit the curr scene
                _scenes[_currentScene]->quit();

                //Load the chosen scene
                _currentScene--;
                loadCurrScene();

                continue;
            }
            else if (currentSceneState == GameState::GO_TO_CHOSEN_SCENE)
            {
                //Get the scene idx
                int sceneIdx = _scenes[_currentScene]->getChosenSceneIdx();
                if (sceneIdx < 0 || sceneIdx >= _scenes.size()) //If the sceenIdx is not valid
                {
                    printf("Tried to load sceen with invalid idx: %d\n", sceneIdx);
                    printf("There are currently %d scenes!\n", (int)_scenes.size());
                }

                //Quit the curr scene
                _scenes[_currentScene]->quit();

                //Load the chosen scene
                _currentScene = sceneIdx;
                loadCurrScene();

                continue;
            }

            _scenes[_currentScene]->draw();
        }
    }
}

void App::quit()
{
    //Destroy window
    _window.free();

    //Quit SDL systems
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void App::loadCurrScene()
{
    if (!_scenes[_currentScene]->loadMedia())
    {
        printf("Could not load media of scene with idx: %d\n", _currentScene);
        return;
    }

    if (!_scenes[_currentScene]->init())
    {
        printf("Could not init scene with idx: %d\n", _currentScene);
        return;
    }
}
