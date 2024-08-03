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

    _player.reset();
    _level.init(21,"Assets/level_map");
    initCollectables(); //Init the coins
    initGhosts(); //Init the ghosts

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
            else if (_gameState == GameState::RUNNING) //If game isrunning
            {
                //If R is pressed
                if (e.key.keysym.sym == SDLK_r && e.key.repeat == 0)
                {
                    restart();
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

       
        if (deltaTime <= 0.1f) //Only update if last update was less than 0.1 secs ago
        {
            //Update
            std::vector<SDL_FRect> _levelCollidors = _level.getCollisionWalls();
            //Update player
            _player.update(deltaTime, _score, _coins, _levelCollidors);

            //Update ghosts
            for (size_t i = 0; i < _ghosts.size(); i++)
            {
                _ghosts[i].update(deltaTime, _player, _levelCollidors);
            }

            //Check the win condition
            if (_coins.size() == 0)
            {
                _gameState = GameState::GAME_WON;
            }

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
        else //if last update was more than 0.1 secs ago just reset the timer
        {
            //Restart step timer
            _deltaTimer.start();
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
    if (_developerVision)
    {
        //Draw test tiles
        SDL_SetRenderDrawColor(_window.renderer, 0, 255, 0, 255);
        _level.drawTileGrid(_window);

        //Draw walls
        SDL_SetRenderDrawColor(_window.renderer, 255, 0, 0, 255);

        std::vector<SDL_FRect> _levelCollidors = _level.getCollisionWalls();
        for (size_t i = 0; i < _levelCollidors.size(); i++)
        {
            SDL_RenderDrawRectF(_window.renderer, &_levelCollidors[i]);
        }

        //Fill the players tile
        _level.fillPlayerTile(_window, _player);
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
    
    //Return the objs to original state
    reset();
}

void LevelScene::restart()
{
    //Return the objs to original state
    reset();

    //Reinit objects
    _level.init(21, "Assets/level_map");
    initCollectables();
    initGhosts();
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
   /* _ghosts.emplace_back(675.0f, 286.0f, GhostType::GREEN);
    _ghosts.emplace_back(575.0f, 396.0f, GhostType::BLUE);
    _ghosts.emplace_back(675.0f, 396.0f, GhostType::ORANGE);
    _ghosts.emplace_back(625.0f, 340.0f, GhostType::YELLOW);*/
}

void LevelScene::reset()
{
    //Reset the game state
    _gameState = GameState::RUNNING;

    //Reset chosen scene idx
    _chosenSceneIdx = -1;

    //Empty coins
    _coins.clear();

    //Empty ghosts
    _ghosts.clear();

    //Reset the score
    _score = 0.0f;

    //Reset the player
    _player.reset();
}
