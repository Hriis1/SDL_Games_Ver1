#include "LevelScene.h"

LevelScene::LevelScene(LWindow& win, bool& quitFlag)
    : _window(win), _quitFlag(quitFlag)
{

    //Outside walls
    _walls.emplace_back(SDL_FRect{ 284, 18, 712, 15 });
    
    _walls.emplace_back(SDL_FRect{ 996, 18, 17, 680 });
    _walls.emplace_back(SDL_FRect{ 267, 18, 17, 680 });
    
    _walls.emplace_back(SDL_FRect{ 284, 260, 121, 15 });
    _walls.emplace_back(SDL_FRect{ 875, 260, 121, 15 });

    _walls.emplace_back(SDL_FRect{ 388, 275, 17, 44 });
    _walls.emplace_back(SDL_FRect{ 875, 275, 17, 44 });

    _walls.emplace_back(SDL_FRect{ 284, 319, 121, 15 });
    _walls.emplace_back(SDL_FRect{ 875, 319, 121, 15 });

    _walls.emplace_back(SDL_FRect{ 284, 382, 121, 15 });
    _walls.emplace_back(SDL_FRect{ 875, 382, 121, 15 });

    _walls.emplace_back(SDL_FRect{ 388, 395, 17, 45 });
    _walls.emplace_back(SDL_FRect{ 875, 395, 17, 45 });

    _walls.emplace_back(SDL_FRect{ 284, 440, 121, 15 });
    _walls.emplace_back(SDL_FRect{ 875, 440, 121, 15 });

    _walls.emplace_back(SDL_FRect{ 284, 684, 712, 15 });

    //Inside walls
    _walls.emplace_back(SDL_FRect{ 349, 78, 582, 15 });
    _walls.emplace_back(SDL_FRect{ 349, 623, 582, 15 });

    _walls.emplace_back(SDL_FRect{ 632, 93, 17, 59 });
    _walls.emplace_back(SDL_FRect{ 632, 564, 17, 59 });

    _walls.emplace_back(SDL_FRect{ 349, 140, 96, 13 });
    _walls.emplace_back(SDL_FRect{ 835, 140, 96, 13 });
    _walls.emplace_back(SDL_FRect{ 349, 563, 96, 13 });
    _walls.emplace_back(SDL_FRect{ 835, 563, 96, 13 });

    _walls.emplace_back(SDL_FRect{ 510, 140, 59, 13 });
    _walls.emplace_back(SDL_FRect{ 713, 140, 59, 13 });
    _walls.emplace_back(SDL_FRect{ 510, 563, 59, 13 });
    _walls.emplace_back(SDL_FRect{ 713, 563, 59, 13 });

    _walls.emplace_back(SDL_FRect{ 389, 153, 17, 60 });
    _walls.emplace_back(SDL_FRect{ 875, 153, 17, 60 });
    _walls.emplace_back(SDL_FRect{ 389, 503, 17, 60 });
    _walls.emplace_back(SDL_FRect{ 875, 503, 17, 60 });

    _walls.emplace_back(SDL_FRect{ 470, 200, 17, 134 });
    _walls.emplace_back(SDL_FRect{ 794, 200, 17, 134 });
    _walls.emplace_back(SDL_FRect{ 470, 382, 17, 134 });
    _walls.emplace_back(SDL_FRect{ 794, 382, 17, 134 });

    _walls.emplace_back(SDL_FRect{ 551, 201, 180, 13 });
    _walls.emplace_back(SDL_FRect{ 551, 504, 180, 13 });
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

    //Draw the walls for testing
    SDL_SetRenderDrawColor(_window.renderer, 255, 0, 0, 255);
    for (size_t i = 0; i < _walls.size(); i++)
    {
        SDL_RenderDrawRectF(_window.renderer, &_walls[i]);
    }
   

    //Draw the player
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
