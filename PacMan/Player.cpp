#include "Player.h"

Player::Player()
{
    //Move collider relative to the circle
    shiftColliders();
}

bool Player::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //init texture
    _PlayerTexture.initRenderer(renderer);
    if(!_PlayerTexture.loadFromFile("Assets/Textures/PacMan.png", window))
    {
        printf("Could not load Player texture\n");
        return false;
    }

    //init spriteclips
    
    _SpriteClips[0] = { 0,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[1] = { (int)PLAYER_WIDTH,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[2] = { (int)PLAYER_WIDTH * 2,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[3] = { (int)PLAYER_WIDTH * 3,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[4] = { (int)PLAYER_WIDTH * 4,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[5] = { (int)PLAYER_WIDTH * 5,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[6] = { (int)PLAYER_WIDTH * 6,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };
    _SpriteClips[7] = { (int)PLAYER_WIDTH * 7,0, (int)PLAYER_WIDTH, (int)PLAYER_HEIGHT };

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
    _PlayerTexture.render(_xPos - camX, _yPos - camY, &_SpriteClips[0], TEXTURE_SCALE);
}

void Player::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}
