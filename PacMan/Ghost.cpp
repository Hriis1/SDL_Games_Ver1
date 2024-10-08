#include "Ghost.h"
#include <cmath>
#include <iostream>

#include <Utils.h>
#include <LBinarySearch.h>

const float Ghost::TEXTURE_SCALE = 2.0f;
const float Ghost::GHOST_HEIGHT = 16 * Ghost::TEXTURE_SCALE;
const float Ghost::GHOST_WIDTH = 16 * Ghost::TEXTURE_SCALE;
const float Ghost::GHOST_VEL = 175.0f;
SDL_Rect Ghost::_spriteClips[Ghost_ANIMATION_FRAMES];

std::vector<int> Ghost::_movementModeIntervals = {7,20,7,20,5,20,5}; //intervals to change mode from scatter to chase or from chase to scatter

Ghost::Ghost(float xPos, float yPos, float pathFindInterval)
    : _xPos(xPos), _yPos(yPos), _pathFindInterval(pathFindInterval)
{
    static bool firstCall = false;
    if (!firstCall) //if its the 1st time calling the constructor init the sprite clips
    {
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
    }
    //Start the timers
    _pathFindTimer.start(); 
    _movementModeTimer.start();

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

void Ghost::update(float deltaTime, const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts)
{
    handleMovementMode();
    changeScatterTileTarget(level);

    if (_pathFindTimer.getTicks() > _pathFindInterval) //Executes _pathFindInterval milisecs
    {
        //Find path to player
        auto newPath = pathFind(level, player, ghosts);
        if (newPath.size() > 0) //only update the path if it found one
        {
            _pathToFollow = newPath;
        }
        

        //Print the path for debug
        //printPath(level, _pathToFollow);

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

            //Set recent x and y dirs
            if (_ghostMovementDir.x != 0)
                _ghostRecentMovementDirs.x = _ghostMovementDir.x;
            if (_ghostMovementDir.y != 0)
                _ghostRecentMovementDirs.y = _ghostMovementDir.y;
        }
    }

    ghostMove(deltaTime, level, pathPosIdx); //Move the ghost at the direction determined by the path

    //Collide with level
    collideWithLevel(_collisionRect, _xPos, _yPos, level.getCollisionWalls());
    shiftColliders();
}

void Ghost::render(const LWindow& window, const Level& level, int camX, int camY)
{
    //Render the current clip
    SDL_Rect* currentClip = &_spriteClips[_animationFrame / 12];
    _spriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);

    //Go to next frame
    _animationFrame++;

    //Cycle animation
    if (_animationFrame / 12 >= Ghost_ANIMATION_FRAMES)
    {
        _animationFrame = 0;
    }

    fillTargetTile(window, level);
}

void Ghost::fillTargetTile(const LWindow& window, const Level& level) const
{
    SDL_SetRenderDrawColor(window.renderer, _fillTileColor.r, _fillTileColor.g, _fillTileColor.b, _fillTileColor.a);
    level.fillTile(window, _targetGridPos);
}

void Ghost::handleMovementMode()
{
    if (_movementModeIdx < _movementModeIntervals.size()) //if movement mode is still being changed
    {
        if (_movementModeTimer.getTicks() > _movementModeIntervals[_movementModeIdx] * 1000) //if its time to swap modes
        {
            //Increment the idx
            _movementModeIdx++;

            //Change the movement mode
            _movementMode = static_cast<GhostMovementMode>(-static_cast<int>(_movementMode));

            //Restart the timer
            _movementModeTimer.start();
        }
    }
    else if (_movementModeIdx == _movementModeIntervals.size()) //If the movement mode hit the final stage
    {
        //Stop the timer
        _movementModeTimer.stop();
        _movementModeIdx++;
    }
}

void Ghost::changeScatterTileTarget(const Level& level)
{
    //set the scatter tile on the 1st call
    if (!_scatterTileInited)
    {
        int currIdx = _targetScatterTileIdx % 2;
        SDL_Point targetWorldPos = level.getWorldPos(_targetScatterTiles[currIdx]);
        _currScatterTile = { (float)targetWorldPos.x, (float)targetWorldPos.y, (float)level.getTileSize(), (float)level.getTileSize() };
        _scatterTileInited = true;
    }

    if (checkCollision(_collisionRect, _currScatterTile)) //if ghost reaches the current scatter tile
    {
        //Update the scatter tile
        _targetScatterTileIdx++;
        int currIdx = _targetScatterTileIdx % 2;
        SDL_Point targetWorldPos = level.getWorldPos(_targetScatterTiles[currIdx]);
        _currScatterTile = { (float)targetWorldPos.x, (float)targetWorldPos.y, (float)level.getTileSize(), (float)level.getTileSize() };
    }
}

void Ghost::ghostMove(float deltaTime, const Level& level, int pathPosIdx)
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
        //SDL_Point dirToPlayer = { gridPos.x <  posToChase.x ? 1 : -1, gridPos.y < posToChase.y ? 1 : -1 }; //the direction of the player

        if (xMovement &&  abs(futureIntersection.w) < abs(futureIntersection.h)) //if the original direction was on the x axis and the inteersaction is on the x axis
        {
            xMovement = 0; //dont move to the x axis
            yMovement = _ghostRecentMovementDirs.y * GHOST_VEL * deltaTime; //move to the y axis instead
        }
        else if(yMovement && abs(futureIntersection.w) > abs(futureIntersection.h)) //if the original direction was on the y axis and the inteersaction is on the y axis
        {
            yMovement = 0; //dont move to the y axis
            xMovement = _ghostRecentMovementDirs.x * GHOST_VEL * deltaTime; //move to the x axis instead
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

