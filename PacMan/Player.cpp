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

void Player::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            _yVel -= PLAYER_VEL;
            _direction = DIRECTION::UP;
            break;
        case SDLK_DOWN:
            _yVel += PLAYER_VEL;
            _direction = DIRECTION::DPWN;
            break;
        case SDLK_LEFT:
            _xVel -= PLAYER_VEL;
            _direction = DIRECTION::LEFT;
            break;
        case SDLK_RIGHT:
            _xVel += PLAYER_VEL; 
            _direction = DIRECTION::RIGHT;
            break;
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
    SDL_Rect* currentClip = &_spriteClips[_animationFrame / 12];

    
    if (_direction == DIRECTION::LEFT)
    {
        _playerSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else if(_direction == DIRECTION::RIGHT)
    {
        _playerSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_direction == DIRECTION::DPWN)
    {
        _playerSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 90.0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        _playerSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, -90.0, NULL, SDL_FLIP_NONE);
    }
          
    //Go to next frame
    _animationFrame++;

    //Cycle animation
    if (_animationFrame / 12 >= PLAYER_ANIMATION_FRAMES)
    {
        _animationFrame = 0;
    }
}

void Player::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}
