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

    return true;
}

bool Game::loadMedia()
{
    //Load fonts
    _font = TTF_OpenFont("../_Engine/fonts/lazy.ttf", 18);
    if (_font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    _gameOverFont = TTF_OpenFont("../_Engine/fonts/lazy.ttf", 50);
    if (_gameOverFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    _restartFont = TTF_OpenFont("../_Engine/fonts/lazy.ttf", 30);
    if (_restartFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    //Load textures
    _bgTexture.initRenderer(_window.renderer);
    if (!_bgTexture.loadFromFile("textures/bg.png", _window.getSDLWindow()))
    {
        printf("Failed to load bg.png! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    //Load text textures
    _gameOverText.initRenderer(_window.renderer);
    if (!_gameOverText.loadFromRenderedText("Game Over!", { 255, 0 , 0 }, _gameOverFont))
    {
        printf("Failed to load _gameOverText! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    _gameWonText.initRenderer(_window.renderer);
    if (!_gameWonText.loadFromRenderedText("Congrats, you win!", { 0, 0 , 255 }, _gameOverFont))
    {
        printf("Failed to load _gameWonText! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    _restartText.initRenderer(_window.renderer);
    if (!_restartText.loadFromRenderedText("Press space to restart", { 0, 255 , 0 }, _restartFont))
    {
        printf("Failed to load _restartText! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    //Load tiles
    if (!Tile::loadTiles("tileMapScaled0.5.map", _tiles, 0.5f))
    {
        printf("Failed to load tile set!\n");
        return false;
    }

    return true;
}

bool Game::initPlayer()
{
    _player.reset(_xStartingPos, _yStartingPos);
    return _player.init(_window.renderer, _window.getSDLWindow(), _font);
}

void Game::run()
{
    //The camera area
    SDL_FRect camera = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };

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
            if(_gameState == GameState::RUNNING)
                _player.handleEvent(e);

            //Special key input
            if (e.type == SDL_KEYDOWN)
            {
                //If game is not running
                if (_gameState != GameState::RUNNING)
                {
                    //If space is pressed while game is not running
                    if (e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0)
                    {
                        restart();
                    }
                }
            }
        }
 
        //Only update when not minimized and game is running
        if (!_window.isMinimized())
        {
            if (_gameState == GameState::RUNNING)
            {
                //Calculate time step
                float deltaTime = _deltaTimer.getTicks() / 1000.f;

                //Update
                _player.update(_tiles, 9.5f, deltaTime);

                //Lose condition
                if (_player.getYPos() + _player.getHeight() >= LEVEL_HEIGHT)
                {
                    _gameState = GameState::GAME_LOST;
                    _deltaTimer.stop();
                    continue;
                }
                //Win condition
                if (_player.getYPos() + _player.getHeight() <= _tiles[0]->getBox().x && checkCollision(_player.getCollider(), _tiles[0]->getBox()))
                {
                    _gameState = GameState::GAME_WON;
                    _deltaTimer.stop();
                    continue;
                }

                //Restart step timer
                _deltaTimer.start();

                //Center the camera over the dot
                camera.x = (_player.getXPos()) - SCREEN_WIDTH / 2;
                camera.y = (_player.getYPos()) - SCREEN_HEIGHT / 2;

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
            }

            //Clear screen
            SDL_SetRenderDrawColor(_window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(_window.renderer);

            //Render background
            _bgTexture.render(0,0);

            //If game is lost
            if (_gameState == GameState::GAME_LOST)
            {
                _gameOverText.render(SCREEN_WIDTH / 2 - _gameOverText.getWidth() / 2, SCREEN_HEIGHT / 2);
                _restartText.render(SCREEN_WIDTH / 2 - _restartText.getWidth() / 2, SCREEN_HEIGHT / 2 + _gameOverText.getHeight() + 10);
            }
            else if (_gameState == GameState::GAME_WON)
            {

            }

            //Render level
            for (int i = 0; i < _tiles.size(); ++i)
            {
                _tiles[i]->render(camera);
            }

            //Render the textures
            _player.render(camera.x, camera.y);

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

void Game::restart()
{
    _gameState = GameState::RUNNING;
    _player.reset(_xStartingPos, _yStartingPos);
}
