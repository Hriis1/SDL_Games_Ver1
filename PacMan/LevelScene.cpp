#include "LevelScene.h"

LevelScene::LevelScene(LWindow& win, bool& quitFlag)
    : _window(win), _quitFlag(quitFlag)
{
}

LevelScene::~LevelScene()
{
}

bool LevelScene::init()
{
    return true;
}

bool LevelScene::loadMedia()
{
    //Load fonts
   
    //Load textures
   
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

        //Handle input for the dot
        if (_gameState == GameState::RUNNING);

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

        //Restart step timer
        _deltaTimer.start();

        //Center the camera over the player

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

void LevelScene::draw()
{
    //Clear screen
    SDL_SetRenderDrawColor(_window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_window.renderer);

    

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
