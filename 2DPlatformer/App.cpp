#include "App.h"
#include <Tile.h>
#include "Level1Scene.h"

bool App::init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    //Create windows and create renderers
    if (!_window.init())
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize PNG  and JPG loading
    int imgFlags = IMG_INIT_PNG;
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

    if (!Tile::initTiles(_window.renderer, _window.getSDLWindow()))
    {
        printf("Could not init tiles\n");
        return false;
    }

    return true;
}

void App::initScenes()
{
    _scenes.push_back(new Level1Scene(_window, _quit));
}

void App::run()
{
    if (!_scenes[_currentScene]->loadMedia())
    {
        printf("Could not initial scene\n");
        return;
    }

    if (!_scenes[_currentScene]->init())
    {
        printf("Could not init initial scene\n");
        return;
    }

    //Start the game loop and poll events
    SDL_Event e;

    while (_quit == false)
    {
        _scenes[_currentScene]->handleEvents(e);

        //Only update when not minimized and game is running
        if (!_window.isMinimized())
        {
            _scenes[_currentScene]->update();
            
            //Check if we have to go to next scene
            if (_scenes[_currentScene]->getGameState() == GameState::GO_TO_NEXT_SCENE)
            {
                _scenes[_currentScene]->quit();

                _currentScene++;
                if (!_scenes[_currentScene]->loadMedia())
                {
                    printf("Could not load media of next scene\n");
                    return;
                }
                if (!_scenes[_currentScene]->init())
                {
                    printf("Could not init next scene\n");
                    return;
                }
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
