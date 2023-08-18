#include "Game.h"

Level1Scene::Level1Scene(LWindow& win, bool& quitFlag)
    : _window(win), _quitFlag(quitFlag)
{
}

Level1Scene::~Level1Scene()
{
}

bool Level1Scene::init()
{
    initPlayer();
}

bool Level1Scene::loadMedia()
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

void Level1Scene::handleEvents(SDL_Event& e)
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            _quitFlag = true;

        //Handle window events
        _window.handleEvent(e);

        //Handle input for the dot
        if (_gameState == GameState::RUNNING)
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
}

void Level1Scene::update()
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
        }
        //Win condition
        if (_player.getYPos() + _player.getHeight() <= _tiles[0]->getBox().x && checkCollision(_player.getCollider(), _tiles[0]->getBox()))
        {
            _gameState = GameState::GAME_WON;
            _deltaTimer.stop();
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
}

void Level1Scene::draw()
{
    //Clear screen
    SDL_SetRenderDrawColor(_window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_window.renderer);

    //Render background
    _bgTexture.render(0, 0);

    //If game is lost
    if (_gameState == GameState::GAME_LOST)
    {
        _gameOverText.render(SCREEN_WIDTH / 2 - _gameOverText.getWidth() / 2, SCREEN_HEIGHT / 2);
        _restartText.render(SCREEN_WIDTH / 2 - _restartText.getWidth() / 2, SCREEN_HEIGHT / 2 + _gameOverText.getHeight() + 10);
    }
    else if (_gameState == GameState::GAME_WON)
    {
        _gameWonText.render(SCREEN_WIDTH / 2 - _gameWonText.getWidth() / 2, SCREEN_HEIGHT / 2);
        _restartText.render(SCREEN_WIDTH / 2 - _restartText.getWidth() / 2, SCREEN_HEIGHT / 2 + _gameWonText.getHeight() + 10);
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

bool Level1Scene::initPlayer()
{
    _player.reset(_xStartingPos, _yStartingPos);
    return _player.init(_window.renderer, _window.getSDLWindow(), _font);
}

void Level1Scene::restart()
{
    _gameState = GameState::RUNNING;
    _player.reset(_xStartingPos, _yStartingPos);
}
