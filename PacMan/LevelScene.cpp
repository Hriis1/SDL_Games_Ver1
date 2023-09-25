#include "LevelScene.h"

LevelScene::LevelScene(LWindow& win, bool& quitFlag)
    : _window(win), _quitFlag(quitFlag)
{

    initWalls(); //Init th=e walls physics objects
    initCollectables(); //Init the coins
    initGhosts(); //Init the ghosts
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

    if (!Collectable::init(_window.renderer, _window.getSDLWindow()))
    {
        std::cout << "Coild not init collectables!" << std::endl;
        return false;
    }

    if (!Ghost::init(_window.renderer, _window.getSDLWindow()))
    {
        std::cout << "Coild not init ghosts!" << std::endl;
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
        _player.update(deltaTime, _score, _coins ,_walls);

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
   
    //Draw coins
    for (size_t i = 0; i < _coins.size(); i++)
    {
        _coins[i].render(_camera.x, _camera.y);
    }

    //Draw ghosts
    for (size_t i = 0; i < _ghosts.size(); i++)
    {
        _ghosts[i].render(_camera.x, _camera.y);
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

void LevelScene::initWalls()
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

    _walls.emplace_back(SDL_FRect{ 632, 214, 17, 59 });
    _walls.emplace_back(SDL_FRect{ 632, 444, 17, 59 });

    _walls.emplace_back(SDL_FRect{ 551, 261, 17, 73 });
    _walls.emplace_back(SDL_FRect{ 713, 261, 17, 73 });
    _walls.emplace_back(SDL_FRect{ 551, 382, 17, 73 });
    _walls.emplace_back(SDL_FRect{ 713, 382, 17, 73 });

    _walls.emplace_back(SDL_FRect{ 566, 321, 43, 13 });
    _walls.emplace_back(SDL_FRect{ 672, 321, 43, 13 });
    _walls.emplace_back(SDL_FRect{ 566, 382, 43, 13 });
    _walls.emplace_back(SDL_FRect{ 672, 382, 43, 13 });
}

void LevelScene::initCollectables()
{
    //Big coins
    _coins.emplace_back(300.0f, 38.0f, CollectableType::BIG);
    _coins.emplace_back(950.0f, 38.0f, CollectableType::BIG);
    _coins.emplace_back(300.0f, 642.0f, CollectableType::BIG);
    _coins.emplace_back(950.0f, 642.0f, CollectableType::BIG);

    //Small coins

    //1st row
    for (size_t i = 350; i <= 900; i+=50)
    {
        _coins.emplace_back(i, 38.0f, CollectableType::SMALL);
    }

    //2nd row
    for (size_t i = 300; i <= 950; i += 50)
    {
        _coins.emplace_back(i, 100.0f, CollectableType::SMALL);
    }

    //3rd row
    for (size_t i = 300; i <= 950; i += 50)
    {
        _coins.emplace_back(i, 162.0f, CollectableType::SMALL);
    }

    //4th row
    for (size_t i = 300; i <= 950; i += 50)
    {
        if(i!=450 && i != 800)
            _coins.emplace_back(i, 224.0f, CollectableType::SMALL);
    }

    //5th row
    _coins.emplace_back(425.0f, 286.0f, CollectableType::SMALL);
    _coins.emplace_back(500.0f, 286.0f, CollectableType::SMALL);
    _coins.emplace_back(575.0f, 286.0f, CollectableType::SMALL);
    _coins.emplace_back(625.0f, 286.0f, CollectableType::SMALL);
    _coins.emplace_back(675.0f, 286.0f, CollectableType::SMALL);
    _coins.emplace_back(750.0f, 286.0f, CollectableType::SMALL);
    _coins.emplace_back(825.0f, 286.0f, CollectableType::SMALL);

    //6th row
    for (size_t i = 300; i <= 950; i += 50)
    {
        _coins.emplace_back(i, 340.0f, CollectableType::SMALL);
    }

    //7th row
    _coins.emplace_back(425.0f, 396.0f, CollectableType::SMALL);
    _coins.emplace_back(500.0f, 396.0f, CollectableType::SMALL);
    _coins.emplace_back(575.0f, 396.0f, CollectableType::SMALL);
    _coins.emplace_back(625.0f, 396.0f, CollectableType::SMALL);
    _coins.emplace_back(675.0f, 396.0f, CollectableType::SMALL);
    _coins.emplace_back(750.0f, 396.0f, CollectableType::SMALL);
    _coins.emplace_back(825.0f, 396.0f, CollectableType::SMALL);

    //8th row
    for (size_t i = 300; i <= 950; i += 50)
    {
        if (i != 450 && i != 800)
            _coins.emplace_back(i, 456.0f, CollectableType::SMALL);
    }

    //9th row
    for (size_t i = 300; i <= 950; i += 50)
    {
        _coins.emplace_back(i, 518.0f, CollectableType::SMALL);
    }

    //10th row
    for (size_t i = 300; i <= 950; i += 50)
    {
        _coins.emplace_back(i, 580.0f, CollectableType::SMALL);
    }

    //11th row
    for (size_t i = 350; i <= 900; i += 50)
    {
        _coins.emplace_back(i, 642.0f, CollectableType::SMALL);
    }





}

void LevelScene::initGhosts()
{
    _ghosts.emplace_back(575.0f, 286.0f, GhostType::RED);
    _ghosts.emplace_back(675.0f, 286.0f, GhostType::GREEN);
    _ghosts.emplace_back(575.0f, 396.0f, GhostType::BLUE);
    _ghosts.emplace_back(675.0f, 396.0f, GhostType::ORANGE);
    _ghosts.emplace_back(625.0f, 340.0f, GhostType::YELLOW);
}
