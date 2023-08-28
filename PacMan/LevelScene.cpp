#include "LevelScene.h"

LevelScene::LevelScene(LWindow& win, bool& quitFlag)
    : _window(win), _quitFlag(quitFlag)
{
    _walls.emplace_back(SDL_FRect{ 100, 20, 200, 100 });
}

LevelScene::~LevelScene()
{
}

bool LevelScene::init()
{
    if (!_player.init(_window.renderer, _window.getSDLWindow()))
    {
        std::cout << "Coild not init player!" << std::endl;
        return false;
    }
    return true;
}

bool LevelScene::loadMedia()
{
    //Load fonts
   
    //Load textures
    _bgTexture.initRenderer(_window.renderer);
    if (!_bgTexture.loadFromFile("Assets/Textures/map.png", _window.getSDLWindow(), 255, 0, 255))
    {
        std::cout << "Couldnt open map.png. Error: " << IMG_GetError() << std::endl;
    }
   
    //Load text textures

    return true;
}

void LevelScene::handleEvents(SDL_Event& e)
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            _quitFlag = true;

        //Handle window events
        _window.handleEvent(e);

        //Handle input for the player
        if (_gameState == GameState::RUNNING)
            _player.handleEvent(e);

        //Special key input
        if (e.type == SDL_KEYDOWN)
        {
            if (_gameState == GameState::GAME_LOST) //If game is lost
            {
                //If space is pressed while game is lost
                if (e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0)
                {
                    restart();
                }
            }
            else if (_gameState == GameState::GAME_WON) //If game is won
            {
                //If space is pressed while game is won
                if (e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0)
                {
                    _gameState = GameState::GO_TO_NEXT_SCENE;
                }
            }
        }
    }
}

void LevelScene::update()
{
    if (_gameState == GameState::RUNNING)
    {
        //Calculate time step
        float deltaTime = _deltaTimer.getTicks() / 1000.f;

        //Update
        _player.update(deltaTime, _walls);

        //Restart step timer
        _deltaTimer.start();

        //Center the camera over the player

        //Keep the camera in bounds 
        if (_camera.x < 0)
        {
            _camera.x = 0;
        }
        if (_camera.y < 0)
        {
            _camera.y = 0;
        }
        if (_camera.x > LEVEL_WIDTH - _camera.w)
        {
            _camera.x = LEVEL_WIDTH - _camera.w;
        }
        if (_camera.y > LEVEL_HEIGHT - _camera.h)
        {
            _camera.y = LEVEL_HEIGHT - _camera.h;
        }
    }
}

void LevelScene::draw()
{
    //Clear screen
    SDL_SetRenderDrawColor(_window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_window.renderer);

    //Draw textures
    _bgTexture.render(0, 0);

    _player.render(_camera.x, _camera.y);
    

    //Update screen
    SDL_RenderPresent(_window.renderer);
}

void LevelScene::quit()
{
    //Free fonts
}

void LevelScene::restart()
{
    _gameState = GameState::RUNNING;
}
