#include "Player.h"

Player::Player()
{
    //Move collider relative to the circle
    shiftColliders();
}

bool Player::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //init texture
    _playerSpriteSheet.initRenderer(renderer);
    if(!_playerSpriteSheet.loadFromFile("Assets/Textures/PacMan.png", window))
    {
        printf("Could not load Player texture\n");
        return false;
    }

    //init spriteclips
    int textureWidth = PLAYER_WIDTH / TEXTURE_SCALE;
    int textureHeight = PLAYER_HEIGHT / TEXTURE_SCALE;
    _SpriteClips[0] = { 0,0, textureWidth, textureHeight };
    _SpriteClips[1] = { textureWidth,0, textureWidth, textureHeight };
    _SpriteClips[2] = { textureWidth * 2,0, textureWidth, textureHeight };
    _SpriteClips[3] = { textureWidth * 3,0, textureWidth, textureHeight };
    _SpriteClips[4] = { textureWidth * 4,0, textureWidth, textureHeight };
    _SpriteClips[5] = { textureWidth * 5,0, textureWidth, textureHeight };
    _SpriteClips[6] = { textureWidth * 6,0, textureWidth, textureHeight };
    _SpriteClips[7] = { textureWidth * 7,0, textureWidth, textureHeight };

    return true;
}

void Player::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: _yVel -= PLAYER_VEL; break;
        case SDLK_DOWN: _yVel += PLAYER_VEL; break;
        case SDLK_LEFT: _xVel -= PLAYER_VEL; break;
        case SDLK_RIGHT: _xVel += PLAYER_VEL; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: _yVel += PLAYER_VEL; break;
        case SDLK_DOWN: _yVel -= PLAYER_VEL; break;
        case SDLK_LEFT: _xVel += PLAYER_VEL; break;
        case SDLK_RIGHT: _xVel -= PLAYER_VEL; break;
        }
    }
}

void Player::update(float  deltaTime)
{
    //Move the Player left or right
    _xPos += _xVel * deltaTime;
    shiftColliders();


    //Move the Player up or down
    _yPos += _yVel * deltaTime;
    shiftColliders();

    
}

void Player::render(int camX, int camY)
{
    _playerSpriteSheet.render(_xPos - camX, _yPos - camY, &_SpriteClips[0], TEXTURE_SCALE);
}

void Player::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}
