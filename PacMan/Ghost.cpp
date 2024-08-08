#include "Ghost.h"
#include <Utils.h>
#include <A_Star.h>
#include <cmath>
//#include <iostream>

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
    shiftColliders();
}

void Ghost::update(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level)
{
    chasePlayer(deltaTime, player, level); //Go after the player

    //Collide with level
    collideWithLevel(_collisionRect, _xPos, _yPos, level);
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

void Ghost::pathFindToPlayerAStar(const Level& level, const Player& player)
{
    //Get the positions of the player and ghost
    SDL_Point playerGridPos = level.getGridPos(player.getPos<SDL_Point, int>());
    A_Point playerGridAPos = { playerGridPos.x, playerGridPos.y };
    SDL_Point ghostGridPos = level.getGridPos({(int)_xPos, (int)_yPos});
    A_Point ghostGridAPos = { ghostGridPos.x, ghostGridPos.y };
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

void Ghost::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}

