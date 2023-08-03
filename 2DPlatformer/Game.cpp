#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::init()
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

    if (!_dot.init(_window.renderer, _window.getSDLWindow()))
    {
        return false;
    }

    return true;
}

bool Game::loadMedia()
{
}

void Game::run()
{
}

void Game::quit()
{
}
