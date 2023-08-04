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
    //Load fonts
    _font = TTF_OpenFont("../_Engine/fonts/lazy.ttf", 28);
    if (_font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    //Load textures

    //Load tiles
    if (!Tile::loadTiles("tileMapScaled0.5.map", _tiles, 0.5f))
    {
        printf("Failed to load tile set!\n");
        return false;
    }

    return true;
}

void Game::run()
{
    //The camera area
    SDL_FRect camera = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };

    std::vector<SDL_FRect> boxes;
    std::vector<Circle<float>> circles;

    //Current input point
    int currentData = 0;

    //Start the game loop and poll events
    SDL_Event e;
    bool quit = false;

    while (quit == false)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;

            //Handle window events
            _window.handleEvent(e);

            //Handle input for the dot
            _dot.handleEvent(e);

            //Special key input
            if (e.type == SDL_KEYDOWN)
            {

            }
        }

        //Only draw when not minimized
        if (!_window.isMinimized())
        {
            //Update
            _dot.update(boxes, circles, _tiles);

            //Center the camera over the dot
            camera.x = (_dot.getXPos()) - SCREEN_WIDTH / 2;
            camera.y = (_dot.getYPos()) - SCREEN_HEIGHT / 2;

            //Keep the camera in bounds 
            if (camera.x < 0)
            {
                camera.x = 0;
            }
            if (camera.y < 0)
            {
                camera.y = 0;
            }
            if (camera.x > LEVEL_WIDTH - camera.w)
            {
                camera.x = LEVEL_WIDTH - camera.w;
            }
            if (camera.y > LEVEL_HEIGHT - camera.h)
            {
                camera.y = LEVEL_HEIGHT - camera.h;
            }

            //Clear screen
            SDL_SetRenderDrawColor(_window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(_window.renderer);

            //Render level
            for (int i = 0; i < _tiles.size(); ++i)
            {
                _tiles[i]->render(camera);
            }

            //Render the textures
            _dot.render(camera.x, camera.y);

            //Update screen
            SDL_RenderPresent(_window.renderer);
        }
    }
}

void Game::quit()
{
    //Free global font
    TTF_CloseFont(_font);
    _font = NULL;

    //Destroy window
    _window.free();

    //Quit SDL systems
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
