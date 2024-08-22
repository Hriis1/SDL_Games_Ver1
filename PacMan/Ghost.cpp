#include "Ghost.h"
#include <cmath>
#include <iostream>

#include <Utils.h>
#include <LBinarySearch.h>

const float Ghost::TEXTURE_SCALE = 2.0f;
const float Ghost::GHOST_HEIGHT = 16 * Ghost::TEXTURE_SCALE;
const float Ghost::GHOST_WIDTH = 16 * Ghost::TEXTURE_SCALE;
const float Ghost::GHOST_VEL = 175.0f;
LTexture Ghost::_redGhostSpriteSheet;
LTexture Ghost::_greenGhostSpriteSheet;
LTexture Ghost::_blueGhostSpriteSheet;
LTexture Ghost::_orangeGhostSpriteSheet;
LTexture Ghost::_yellowGhostSpriteSheet;
SDL_Rect Ghost::_spriteClips[Ghost_ANIMATION_FRAMES];

Ghost::Ghost(float xPos, float yPos, GhostType type)
    : _xPos(xPos), _yPos(yPos), _type(type)
{
    _pathFindTimer.start(); //Start the pathfinding timer
    shiftColliders(); //Shift colliders
}

void Ghost::handleEvent(SDL_Event& e, const Level& level, const Player& player)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_g:
            //pathFindToPlayerAStar(level, player);
            break;
        }
    }
}

void Ghost::update(float deltaTime, const Level& level, const Player& player)
{
    if (_pathFindTimer.getTicks() > 500.0f) //Executes every 1 secs
    {
        //Find path to player
        auto newPath = pathFindToPlayerAStar(level, player);
        if (newPath.size() > 0) //only update the path if it found one
        {
            _pathToFollow = newPath;
        }
        

        //Print the path for debug
        printPath(level, _pathToFollow);

        //Restart the timer
        _pathFindTimer.start();
    }

    //get the grid pos
    SDL_Point ghostGridPos = level.getWalkableGridPos({ (int)(_xPos + GHOST_WIDTH / 2), (int)(_yPos + GHOST_HEIGHT / 2) });
    A_Point gridAPoint = { ghostGridPos.x, ghostGridPos.y };

    int pathPosIdx = linearSearch(_pathToFollow, gridAPoint);

    if (pathPosIdx != -1)
    {
        //Determine the moving direction
        if (_pathToFollow.size() > pathPosIdx + 1)
        {
            SDL_Point newMovementDir = { _pathToFollow[pathPosIdx + 1].x - _pathToFollow[pathPosIdx].x, _pathToFollow[pathPosIdx + 1].y - _pathToFollow[pathPosIdx].y };
            _ghostMovementDir = newMovementDir; //set the new direction
        }
    }

    ghostMove(deltaTime, level); //Move the ghost at the direction determined by the path

    //Collide with level
    collideWithLevel(_collisionRect, _xPos, _yPos, level.getCollisionWalls());
    shiftColliders();
}

bool Ghost::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //init texture
    _redGhostSpriteSheet.initRenderer(renderer);
    _greenGhostSpriteSheet.initRenderer(renderer);
    _blueGhostSpriteSheet.initRenderer(renderer);
    _orangeGhostSpriteSheet.initRenderer(renderer);
    _yellowGhostSpriteSheet.initRenderer(renderer);

    if (!_redGhostSpriteSheet.loadFromFile("Assets/Textures/redGhost.png", window))
    {
        printf("Could not load red ghost texture\n");
        return false;
    }
    if (!_greenGhostSpriteSheet.loadFromFile("Assets/Textures/greenGhost.png", window))
    {
        printf("Could not load green ghost texture\n");
        return false;
    }
    if (!_blueGhostSpriteSheet.loadFromFile("Assets/Textures/blueGhost.png", window))
    {
        printf("Could not load blue ghost texture\n");
        return false;
    }
    if (!_orangeGhostSpriteSheet.loadFromFile("Assets/Textures/orangeGhost.png", window))
    {
        printf("Could not load orange ghost texture\n");
        return false;
    }
    if (!_yellowGhostSpriteSheet.loadFromFile("Assets/Textures/yellowGhost.png", window))
    {
        printf("Could not load yellow ghost texture\n");
        return false;
    }

    //init spriteclips
    int textureWidth = GHOST_WIDTH / TEXTURE_SCALE;
    int textureHeight = GHOST_HEIGHT / TEXTURE_SCALE;
    _spriteClips[0] = { 0,0, textureWidth, textureHeight };
    _spriteClips[1] = { textureWidth,0, textureWidth, textureHeight };
    _spriteClips[2] = { textureWidth * 2,0, textureWidth, textureHeight };
    _spriteClips[3] = { textureWidth * 3,0, textureWidth, textureHeight };
    _spriteClips[4] = { textureWidth * 4,0, textureWidth, textureHeight };
    _spriteClips[5] = { textureWidth * 5,0, textureWidth, textureHeight };
    _spriteClips[6] = { textureWidth * 6,0, textureWidth, textureHeight };
    _spriteClips[7] = { textureWidth * 7,0, textureWidth, textureHeight };

    return true;
}

void Ghost::render(int camX, int camY)
{
    SDL_Rect* currentClip = &_spriteClips[_animationFrame / 12];

    if (_type == GhostType::RED)
    {
        _redGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if(_type == GhostType::GREEN)
    {
        _greenGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_type == GhostType::BLUE)
    {
        _blueGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_type == GhostType::ORANGE)
    {
        _orangeGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_type == GhostType::YELLOW)
    {
        _yellowGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }

    //Go to next frame
    _animationFrame++;

    //Cycle animation
    if (_animationFrame / 12 >= Ghost_ANIMATION_FRAMES)
    {
        _animationFrame = 0;
    }
}

std::vector<A_Point> Ghost::pathFindToPlayerAStar(const Level& level, const Player& player)
{
    //Get the positions of the player and ghost
    SDL_Point playerGridPos = level.getWalkableGridPos(player.getCenterPos<SDL_Point, int>());
    A_Point playerGridAPos = { playerGridPos.x, playerGridPos.y };
    SDL_Point ghostGridPos = level.getWalkableGridPos({ (int)(_xPos + GHOST_WIDTH / 2), (int)(_yPos + GHOST_HEIGHT / 2) });
    A_Point ghostGridAPos = { ghostGridPos.x, ghostGridPos.y };

    //The isWalkable function ptr
    auto isWalkable = std::bind(&Level::isWalkable, &level, std::placeholders::_1, std::placeholders::_2);

    //Get the path
    size_t mapWidth = level.getTileMapWidth();
    size_t mapHright = level.getTileMapHeight();
    auto path = aStar(ghostGridAPos, playerGridAPos, mapWidth, isWalkable);

    return path;
}

void Ghost::ghostMove(float deltaTime, const Level& level)
{
    //Get the potential future position
    float xMovement = _ghostMovementDir.x * GHOST_VEL * deltaTime;
    float yMovement = _ghostMovementDir.y * GHOST_VEL * deltaTime;

    //change the movement direction to the oposite axis still chasing the player
    SDL_FRect futurePos = { _collisionRect.x + xMovement, _collisionRect.y + yMovement, _collisionRect.w, _collisionRect.h }; //the future position after moving

    SDL_FRect futureIntersection = getIntersectionWithLevel(futurePos, level.getCollisionWalls());
    if (futureIntersection.w > 0 && futureIntersection.h > 0) //if the future position is colliding with level
    {
        A_Point posToChase = { 0, 0 };
        if (_pathToFollow.size() > 0)
            posToChase = _pathToFollow[_pathToFollow.size() - 1]; //the position of the player at the time of path finding

        SDL_Point gridPos = level.getGridPos(SDL_Point{(int)_xPos, (int)_yPos});
        SDL_Point dirToPlayer = { gridPos.x <  posToChase.x ? 1 : -1, gridPos.y < posToChase.y ? 1 : -1 }; //the direction of the player

        if (xMovement &&  abs(futureIntersection.w) < abs(futureIntersection.h)) //if the original direction was on the x axis and the inteersaction is on the x axis
        {
            xMovement = 0; //dont move to the x axis
            yMovement = dirToPlayer.y * GHOST_VEL * deltaTime; //move to the y axis instead
        }
        else if(yMovement && abs(futureIntersection.w) > abs(futureIntersection.h)) //if the original direction was on the y axis and the inteersaction is on the y axis
        {
            yMovement = 0; //dont move to the y axis
            xMovement = dirToPlayer.x * GHOST_VEL * deltaTime; //move to the x axis instead
        }
    }

    //update the position
    _xPos += xMovement;
    _yPos += yMovement;

    shiftColliders(); //Shift colliders afrer movement
}

void Ghost::chasePlayer(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level)
{
    SDL_FPoint playerPos = player.getPos<SDL_FPoint, float>(); //the player pos
    float xDist = std::abs(_xPos - playerPos.x); //the x distance between the ghost and the player

    //Define base dirs: right and up
    int xDir = 1;
    int yDir = -1;
    //Determine x dir
    if (_xPos > playerPos.x) //player is on the left of the ghost
    {
        xDir = -1;
    }

    //Determine y dir
    if (_yPos < playerPos.y) //if player is down relative to the ghost
    {
        yDir = 1;
    }

    //Update position
    SDL_FRect futurePos = { _xPos + (xDir * 9), _yPos, GHOST_WIDTH, GHOST_HEIGHT }; //Potential new pos after moving to the xDir
    SDL_FRect futureIntersection = getIntersectionWithLevel(futurePos, level);
    if ((futureIntersection.w > 0.0f && futureIntersection.h > 0.0f) || xDist <= 1.0f) //if new pos is coliding or the xDist is almsot identical move to yDir
    {
        futurePos = { _xPos, _yPos + (yDir * 9), GHOST_WIDTH, GHOST_HEIGHT };
        futureIntersection = getIntersectionWithLevel(futurePos, level);
        if (futureIntersection.w > 0.0f && futureIntersection.h > 0.0f)
        {
            if (xDist > 1.0f)
                _yDirReversal = -1;
        }

        _yPos += _yDirReversal * yDir * GHOST_VEL * deltaTime;
    }
    else //Move to xDir
    {
        _yDirReversal = 1;
        _xPos += xDir * GHOST_VEL * deltaTime;
    }
    shiftColliders(); //Shift colliders afrer movement
}

void Ghost::chasePlayerAStar(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level)
{
}

void Ghost::printPath(const Level& level, const std::vector<A_Point>& path)
{
    std::vector<int> tileMapCopy = level.getTileMapCopy();
    for (size_t i = 0; i < _pathToFollow.size(); i++)
    {
        if (tileMapCopy[_pathToFollow[i].y * level.getTileMapWidth() + _pathToFollow[i].x] == 1)
        {
            std::cout << "ERRORRRRRRRRRRRRRRRRRRRRR" << std::endl;
            return;
        }
        tileMapCopy[_pathToFollow[i].y * level.getTileMapWidth() + _pathToFollow[i].x] = 3;

    }

    for (size_t i = 0; i < level.getTileMapHeight(); i++)
    {
        for (size_t j = 0; j < level.getTileMapWidth(); j++)
        {
            std::cout << tileMapCopy[i * level.getTileMapWidth() + j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl << std::endl << std::endl;
}

void Ghost::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}

