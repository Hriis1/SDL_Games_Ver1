#include "WinScene.h"

WinScene::WinScene(LWindow& win, bool& quitFlag)
    : _window(win), _quitFlag(quitFlag)
{
}

WinScene::~WinScene()
{
}

bool WinScene::init()
{
   
    return true;
}

bool WinScene::loadMedia()
{
    //Load fonts
    _font = TTF_OpenFont("../_Engine/fonts/lazy.ttf", 50);
    if (_font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    //Load textures
    _bgTexture.initRenderer(_window.renderer);
    if (!_bgTexture.loadFromFile("Assets/Textures/blackbg.jpg", _window.getSDLWindow(), 255, 0, 255))
    {
        std::cout << "Couldnt open map.png. Error: " << IMG_GetError() << std::endl;
    }

    //Load text textures
    _gameWonText.initRenderer(_window.renderer);
    if (!_gameWonText.loadFromRenderedText("You win, gj :)!", { 0, 0 , 255 }, _font))
    {
        printf("Failed to load _gameWonText! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void WinScene::handleEvents(SDL_Event& e)
{
    while (SDL_PollEvent(&e))
    {
        //Only while debugging/developing
        if (DEVELOPER_MODE)
        {
            if (e.type == SDL_KEYDOWN)
            {

                //If X is pressed
                if (e.key.keysym.sym == SDLK_x && e.key.repeat == 0)
                {
                    //toggle developer vision
                    _developerVision = !_developerVision;
                }
            }
        }

        if (e.type == SDL_QUIT)
            _quitFlag = true;

        //Handle window events
        _window.handleEvent(e);


        //Special key input
        if (e.type == SDL_KEYDOWN)
        {
            if (_gameState == GameState::GAME_LOST) //If game is lost
            {
                
            }
            else if (_gameState == GameState::GAME_WON) //If game is won
            {
                //If space is pressed while game is won
               
            }
            else if (_gameState == GameState::RUNNING) //If game is won
            {
           
            }
        }
    }
}

void WinScene::update()
{
    if (_gameState == GameState::RUNNING)
    {
        
    }
}

void WinScene::draw()
{
    //Clear screen
    SDL_SetRenderDrawColor(_window.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_window.renderer);

    //Draw textures
    _bgTexture.render(0, 0);

    _gameWonText.render(SCREEN_WIDTH / 2 - _gameWonText.getWidth() / 2, SCREEN_HEIGHT / 2 - _gameWonText.getHeight() / 2.0f);


    //Update screen
    SDL_RenderPresent(_window.renderer);
}

void WinScene::quit()
{
    //Free fonts
    TTF_CloseFont(_font);
    _font = NULL;
}

void WinScene::restart()
{
    _gameState = GameState::RUNNING;
    reset();
}



void WinScene::reset()
{
}
