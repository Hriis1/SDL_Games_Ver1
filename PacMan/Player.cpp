#include "Player.h"
#include <Utils.h>

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
            break;
        case SDLK_DOWN:
            _yVel += PLAYER_VEL;
            break;
        case SDLK_LEFT:
            _xVel -= PLAYER_VEL;
            break;
        case SDLK_RIGHT:
            _xVel += PLAYER_VEL; 
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

void Player::update(float  deltaTime, float& score, std::vector<Collectable>& collectables, const std::vector<SDL_FRect>& level)
{
    updateDirection();

    //Move the player
    _xPos += _xVel * deltaTime;
    _yPos += _yVel * deltaTime;
    shiftColliders();

    //Collide with level
    collideWithLevel(_collisionRect, _xPos, _yPos, level);
    shiftColliders();

    //Collide with coins
    for (auto coinIt = collectables.begin(); coinIt != collectables.end(); coinIt++)
    {
        if (checkCollision(_collisionRect, coinIt->getCollider())) //if the player is colliding with coin
        {
            if (coinIt->getType() == CollectableType::BIG)
            {
                //Trigger eating mode
            }

            //Update the score
            score += coinIt->getScoreWorth();

            coinIt = collectables.erase(coinIt); //Remove the coin on collision

            //Resolves a crash on collecting the last added coin
            if (coinIt == collectables.end())
            {
                break;
            }
        }
    }

    
}

void Player::reset()
{
    //Reset position
    _xPos = X_START_POS;
    _yPos = Y_START_POS;

    //Reset direction
    _direction = DIRECTION::RIGHT;
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

void Player::updateDirection()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) //if left arrow or right arrow is being pressed
    {
        _direction = DIRECTION::LEFT;
    }
    else if (state[SDL_SCANCODE_RIGHT]) //if left arrow or right arrow is being pressed
    {
        _direction = DIRECTION::RIGHT;
    }
    else if (state[SDL_SCANCODE_DOWN]) //if left arrow or right arrow is being pressed
    {
        _direction = DIRECTION::DPWN;
    }
    else if (state[SDL_SCANCODE_UP]) //if left arrow or right arrow is being pressed
    {
        _direction = DIRECTION::UP;
    }
}
